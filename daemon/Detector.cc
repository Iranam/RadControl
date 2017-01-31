#include"Detector.h"
#ifdef DUMMY
#include"modbus_dummy.h"
#else
#include<modbus.h>
#undef ON
#endif
#include<string>
#include<iostream>
#include<unistd.h>
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QVariant>
using namespace std;
extern modbus_t*ctx;
extern QSqlDatabase db;
using namespace detector;
using namespace interpolant;
#ifdef DEBUG
#define debug_print(a) cerr<<"DEBUG:"#a"="<<a<<endl;
#else
#define debug_print(a)
#endif
void Detector::init(DetectorData*nd){
  d=nd;
  d->type=DetectorType::UNKNOWN_TYPE;
  d->state=DetectorState::INIT;
  d->count=0;
  d->background=0;
  d->exposure=0;
  d->exposure_by_count=0;
  //modbus_id must be set by main()
  configured=false;
  data_ready=false;
  problem_calibration=false;
  problem_connection=false;
  calibration=nullptr;
  time_to_update=0;
}
Detector::~Detector(){
  d->state=DetectorState::DISABLED;
  if(calibration!=nullptr)delete calibration;
}
void log(string msg){
#ifndef NODATABASE
  if(db.open()){
    QSqlQuery query;
    query.prepare("INSERT INTO log(message,time) VALUES (?,NOW())");
    query.bindValue(0,QVariant(msg.c_str()));
    query.exec();
  }else{
    cout<<msg<<endl;
  }
#else
  cout<<msg<<endl;
#endif
}
void Detector::detector_log(string msg){
  log("Detector"+to_string(d->modbus_id)+':'+msg);
}
enum UpdateException{LOST_CONNECTION,UNEXPECTED_BLOCK_SIZE,CALIBRATION_FAILED,BAD_SLAVE_ID};
void Detector::switch_slave(){
  if(modbus_set_slave(ctx,d->modbus_id)==-1)throw BAD_SLAVE_ID;
  usleep(50);//wait for slave timeout. Experiment shows wait times of 2mcs are sufficient.
  //libmodbus disregards modbus protocol timings, which may cause slaves to
  //miss requests if slave timout is not short enough.
  //check "man 7 libmodbus" for more info
}
void Detector::set_exposure(uint16_t val){
  d->exposure=val;
  switch_slave();
  if(d->type==DetectorType::GAMMA){
    //set exposure(register 125)(milliseconds)
    if(modbus_write_register(ctx,125,val)<0){
      handle_connection_loss();
      return;
    }
  }else if(d->type==DetectorType::NEUTRON){
    //set exposure(register 49)(seconds)
    if(modbus_write_register(ctx,49,val)<0){
      handle_connection_loss();
      return;
    }
  }else return;
	d->state=DetectorState::INIT;
	data_ready=false;
}
void Detector::set_exposure_by_count(uint16_t val){
  d->exposure_by_count=val;
  switch_slave();
  if(d->type==DetectorType::NEUTRON){
    //set exposure by count (register 21)(impulses)
    if(modbus_write_register(ctx,21,val)<0){
      handle_connection_loss();
      return;
    }
  }else return;//gamma detectors have no exposure by count functionality
	d->state=DetectorState::INIT;
	data_ready=false;
}
void Detector::set_sensitivity(float val){
  d->sensitivity=val;
  switch_slave();
  if(d->type==DetectorType::NEUTRON){
    union{
      uint16_t u[2];
      float uval;
    };
    uval=val;
    //set sensitivity(register 28-29)(impulses)
    if(modbus_write_registers(ctx,28,2,u)<0){
      handle_connection_loss();
      return;
    }
  }else return;//gamma detectors have no sensitivity registers
	d->state=DetectorState::INIT;
	data_ready=false;
}
void Detector::handle_connection_loss(){
  if(!problem_connection){
    detector_log("Connection lost");
    problem_connection=true;
  }
  configured=false;//if power went down, settings in detector registers are lost and must be re-set
  data_ready=false;
  d->state=DetectorState::NO_CONNECTION;
  time_to_update=10000;//next update in 10 seconds
}
void Detector::update(){
  time_to_update=1000;//default;
  switch_slave();
  try{
    //1.Determine type
    if(d->type==DetectorType::UNKNOWN_TYPE){
      uint16_t block;
      if(modbus_read_registers(ctx,3,1,&block)<0)throw LOST_CONNECTION;
      if(block==20){
				d->type=DetectorType::NEUTRON;
			  d->exposure=1;
			  d->exposure_by_count=0;
			}
      else if(block==100){
				d->type=DetectorType::GAMMA;
			  d->exposure=300;
			}
      else throw UNEXPECTED_BLOCK_SIZE;
    }
    //2.Configure
    if(!configured){
      //TODO set sensitivity, exposure....
      if(d->type==DetectorType::GAMMA){
        //set exposure(register 125) to default=300(milliseconds)
        if(modbus_write_register(ctx,125,d->exposure)<0)throw LOST_CONNECTION;
        if(calibration==nullptr){
          string path="calibration/"+to_string(d->modbus_id)+".txt";
          try{
            calibration=new Interpolant(path.c_str());
          }catch(...){
            throw CALIBRATION_FAILED;
          }
          if(problem_calibration){
            detector_log("Calibration file found");
            problem_calibration=false;
          }
        }
      }else{//NEUTRON
        //set exposure(register 49) to default=1(seconds)
        if(modbus_write_register(ctx,49,d->exposure)<0)throw LOST_CONNECTION;
        //set exposure by count (register 21) to default=100(impulses)
        if(modbus_write_register(ctx,21,d->exposure_by_count)<0)throw LOST_CONNECTION;
        //TODO sensitivity
        //if(modbus_write_register(ctx,36,1)<0)throw LOST_CONNECTION;
      }
			configured=true;
    }
    //3.assure data is ready
    if(!data_ready){
      if(d->type==DetectorType::GAMMA)data_ready=true;//Gamma is always ready?
      else{//NEUTRON
        uint16_t buf=0;
        //read bit 6(data ready) from status register(37)
        if(modbus_read_registers(ctx,37,1,&buf)<0)throw LOST_CONNECTION;
        data_ready=buf>>6&1;
				if(!data_ready)return;//if data is not ready, wait until next update
      }
    }
    //4.read data
    if(d->type==DetectorType::GAMMA){
      //we want to read registers:
      //104:105-count
      //106:107-background
      union{uint16_t buf[4];uint u;float f[2];};
      if(modbus_read_registers(ctx,104,4,buf)<0)throw LOST_CONNECTION;
      d->count=u;
      d->background=(*calibration)(f[1]);
    }else{//NEUTRON
      //we want to read registers:
      //38-count
      //43:44-equivalent dose(МЭД)
      //44-38=6=>read 7 registers
      uint16_t buf[7];
      if(modbus_read_registers(ctx,38,7,buf)<0)throw LOST_CONNECTION;
      d->count=buf[0];
      d->background=modbus_get_float(buf+5);
    }
    //5.write data to database
#ifndef NODATABASE
    if(db.open()){
      QSqlQuery query;
      query.prepare("INSERT INTO readings(modbus_id,EDR,count,exposition,exposition_by_count,time) VALUES (?,?,?,?,?,NOW())");
      query.bindValue(0,QVariant(d->modbus_id));
      query.bindValue(1,QVariant(d->background));
      query.bindValue(2,QVariant(d->count));
      query.bindValue(3,QVariant(d->exposure));
      query.bindValue(4,QVariant(d->exposure_by_count));
      //TODO Check if query was successful
      bool ok=query.exec();
    }
#endif
    //if this point is reached, everything is OK
    //Log regained connection
    if(problem_connection){
      detector_log("Connection restored");
      problem_connection=false;
    }
    d->state=DetectorState::OK;
    {int t=d->exposure;
    if(t<=100)t=300;
    if(t>=10000)t=10000;
    time_to_update=t;
    }
  }catch(UpdateException e){
    switch(e){
    case CALIBRATION_FAILED:{
      if(!problem_calibration){
        detector_log("Can not find calibration file");
        problem_calibration=true;
      }
      time_to_update=100000;//next update in 100 seconds
      break;
    }
    case LOST_CONNECTION:{
      handle_connection_loss();
      break;
    }
    case UNEXPECTED_BLOCK_SIZE:{
      d->type=DetectorType::UNKNOWN_TYPE;
      d->state=DetectorState::UNKNOWN;
      debug_print(UNEXPECTED_BLOCK_SIZE);
      time_to_update=100000;//next update in 100 seconds
      break;
    }
    case BAD_SLAVE_ID:{
      d->type=DetectorType::UNKNOWN_TYPE;
      d->state=DetectorState::UNKNOWN;
      debug_print(BAD_SLAVE_ID);
      time_to_update=100000;//next update in 100 seconds
      break;
    }
    default:{
      //something unexpected happend
      time_to_update=1000000;//next update in 1000 seconds;
    }
    //TODO rest of exceptions
    //TODO exception while sending to database, what if connection is lost or database is dropped?
  }}
}
