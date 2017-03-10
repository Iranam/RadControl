#include<stdio.h>
#ifdef DUMMY
#include"modbus_dummy.h"
#else
#include<modbus.h>
#undef ON
#endif
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/mman.h>
#include<unistd.h>
#include<fcntl.h>
#include<signal.h>
#include<time.h>
#include<mqueue.h>
#include<thread>
#include<string.h>
#include<string>
using std::string;
#include"IPC.h"
#include"Detector.h"
using namespace detector;
#include<QSqlDatabase>
#include<QSqlQuery>

modbus_t*ctx;
uint8_t NDETECTORS=9;
uint default_modbus_ids[]={1,2,3,4,5,6,7,20,21};
uint*modbus_ids=default_modbus_ids;
QSqlDatabase db;
Detector*detectors=nullptr;
char*shmem;
mqd_t message_queue;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wwrite-strings"
char*node_name="/dev/ttyS0";
#pragma GCC diagnostic pop

timespec operator+(timespec t,uint dt){//dt in milliseconds
  t.tv_sec+=dt/1000;
  t.tv_nsec+=1000000*(dt%1000);
  if(t.tv_nsec>1000000000){
    t.tv_sec+=1;
    t.tv_nsec-=1000000000;
  }
  return t;
}

int operator-(timespec t1,timespec t2){//returns difference in milliseconds
  return (t1.tv_sec-t2.tv_sec)*1000+(t1.tv_nsec-t2.tv_nsec)/1000000;
}

void load_options(){
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"
  //WIP
  FILE*file=fopen("RadControl.cfg","r");
  if(file==NULL)return;
  uint n=0;
  uint id;
  fscanf(file,"MODBUS_IDS=");
  while(fscanf(file,"%d",&id)==1)++n;
  if(n==0)return;
  NDETECTORS=n;
#ifdef DEBUG
  printf("%d DETECTORS\n",NDETECTORS);
#endif
  modbus_ids=new uint[NDETECTORS];
  rewind(file);
  fscanf(file,"MODBUS_IDS=");
  for(uint i=0;i<NDETECTORS;++i)fscanf(file,"%d",modbus_ids+i);
  fscanf(file,"\nDEVICE_NODE=%ms",&node_name);
#ifdef DEBUG
  printf("DEBUG=%s\n",node_name);
#endif
  fclose(file);
#pragma GCC diagnostic pop
}

void cleanup(){
  if(detectors!=nullptr)delete[]detectors;
  mq_close(message_queue);
  mq_unlink(MQNAME);
  *shmem=0x00;//Set status byte to show that daemon is not running
  munmap(shmem,NDETECTORS*sizeof(DetectorData)+DATA_OFFSET);
#ifndef NODATABASE
  if(db.open())db.close();
#endif
  modbus_close(ctx);
  modbus_free(ctx);
}

void signal_handler(int signum){
  string msg="Stopped";
  switch(signum){
    case SIGINT:msg+=":interrupted from terminal";break;
    case SIGSEGV:msg+=":SEGFAULT";break;
    case SIGTERM:msg+=":killed";break;
  }
  log(msg);
  cleanup();
  exit(signum);
}

void handle_message(char*mbuf){
  Command cmd=Command(mbuf[0]);
  if(cmd==Command::SHUTDOWN){
    log("Stopped:received a SHUTDOWN message");
    cleanup();
    exit(0);
  }
  if(cmd==Command::SET_EXPOSURE||cmd==Command::SET_EXPOSURE_BY_COUNT||cmd==Command::SET_SENSITIVITY){
  uchar modbus_id=mbuf[1];
  for(uint i=0;i<NDETECTORS;++i)if(detectors[i].d->modbus_id==modbus_id){
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch"
  switch(cmd){
    case Command::SET_EXPOSURE:{
      uint16_t arg;
      memcpy(&arg,mbuf+2,sizeof(arg));
      detectors[i].set_exposure(arg);
#ifdef DEBUG
printf("DEBUG:SET EXPOSURE %i\n",arg);
printf("DEBUG:%i %i\n",mbuf[3],mbuf[4]);
#endif
      break;
    }case Command::SET_EXPOSURE_BY_COUNT:{
      uint16_t arg;
      memcpy(&arg,mbuf+2,sizeof(arg));
      detectors[i].set_exposure_by_count(arg);
      break;
    }case Command::SET_SENSITIVITY:{
      float arg;
      memcpy(&arg,mbuf+2,4);
      detectors[i].set_sensitivity(arg);
      break;
    }
  }
#pragma GCC diagnostic pop
  break;   
  }
}
}

enum class Exception{MODBUS_CONNECTION_FAILED,OPEN_MMAP_FILE_FAILED,RESIZE_MMAP_FILE_FAILED,MMAP_FAILED,OPEN_MESSAGE_QUEUE_FAILED};
int main(/*int argc,char**argv*/){
  signal(SIGINT,signal_handler);
  signal(SIGSEGV,signal_handler);
  signal(SIGTERM,signal_handler);
  load_options();
try{
  //Connect to database
#ifndef NODATABASE
  db=QSqlDatabase::addDatabase("QMYSQL");
  db.setHostName("localhost");
  db.setDatabaseName("rad_control");
  db.setUserName("rad_control_bot");
  db.setPassword("hflrjynhjkm");
  if(!db.open())printf("WARNING:database is not available\n");
#else
  printf("WARNING:The program has been compiled with NODATABASE option\n");
#endif
  //Open modbus connection
  ctx=modbus_new_rtu(node_name,57600,'N',8,2);
  if(modbus_connect(ctx)==-1)throw Exception::MODBUS_CONNECTION_FAILED;
	{struct timeval timeout;
	timeout.tv_sec=0;
	timeout.tv_usec=100000;//0.1 sec
	modbus_set_response_timeout(ctx,&timeout);}
  //Create shared file, where detector data will be stored
  int fd=open("/tmp/radcontrol",O_RDWR|O_CREAT,0644);//user can read/write, other can only read
  if(fd==-1)throw Exception::OPEN_MMAP_FILE_FAILED;
  //Make sure the file is large enough, resize it using lseek and write:
  lseek(fd,NDETECTORS*sizeof(DetectorData)-1+DATA_OFFSET,SEEK_SET);
  if(write(fd,"",1)!=1)throw Exception::RESIZE_MMAP_FILE_FAILED;//write \0, resizing the file
  shmem=(char*)mmap(NULL,NDETECTORS*sizeof(DetectorData)+DATA_OFFSET,PROT_WRITE|PROT_READ,MAP_SHARED,fd,0);
  if(shmem==MAP_FAILED)throw Exception::MMAP_FAILED;
  close(fd);
  //Create detector objects and initialize detector data
  *((char*)shmem+NUMBER_OFFSET)=NDETECTORS;//write number of detectors to shared memory
  DetectorData*data=(DetectorData*)(shmem+DATA_OFFSET);//other bytes are array of DetectorData
  detectors=new Detector[NDETECTORS];
  //Open message queue for incoming commands
  {mq_attr attr;
  attr.mq_flags=0;//blocking mode
  attr.mq_maxmsg=10;
  attr.mq_msgsize=MESSAGE_MAX_SIZE;
  mq_unlink(MQNAME);//delete message queue if it exists
  message_queue=mq_open(MQNAME,O_RDONLY|O_CREAT,00666,&attr);
  if(message_queue==-1)throw Exception::OPEN_MESSAGE_QUEUE_FAILED;
  }
  //Initialize detectors:
  for(uint i=0;i<NDETECTORS;++i){
    data[i].modbus_id=modbus_ids[i];
    detectors[i].init(&data[i]);
		msync(shmem,NDETECTORS*sizeof(DetectorData),MS_ASYNC|MS_INVALIDATE);
  }
  //Set status byte to show that daemon is running:
  {char status=1;
  if(!db.open())status|=2;
  *shmem=status;
  }
#ifdef DUMMY
  log("Started in DUMMY MODE");
#else
  log("Started");
#endif
  //Main loop:
  {
  timespec last_update;
  clock_gettime(CLOCK_REALTIME,&last_update);//TODO:will the program behave correctly if time suddenly changes?
  //mq_timedreceive requires real time, not monotonic
  while(true){
    //TimePoint now=steady_clock::now();
    timespec now;
    clock_gettime(CLOCK_REALTIME,&now);
    int dt=now-last_update;//milliseconds
    last_update=now;
    uint min=10000;//wait no more than 10 seconds
    //loop through detectors, calling update() on those that need it
    //and determine the time before any of detectors needs to be updated next time
    for(uint i=0;i<NDETECTORS;++i){
      Detector&d=detectors[i];
      int R=d.time_to_update-dt;
      if(R>0)d.time_to_update=R;
      else{
        d.update();
        R=d.time_to_update;
      }
      if(uint(R)<min)min=R;
    }
    //int wait_time=duration_cast<milliseconds>(last_update-steady_clock::now()).count()+min;
    //if(wait_time>0)this_thread::sleep_for(milliseconds(wait_time));
    //int next_tv_sec=duration_cast<milliseconds>(last_update-steady_clock::now()).count()+min;
    //WIP
    timespec timeout=last_update+min;
    char mbuf[MESSAGE_MAX_SIZE];
    while(mq_timedreceive(message_queue,mbuf,sizeof(mbuf),NULL,&timeout)!=-1){
      handle_message(mbuf);
    }
    //mq_timedreceive returns -1 on timeout
    //we suppose mq_timedreceive may return -1 only in that case
  }
  }
}catch(Exception e){
  switch(e){
    case Exception::MODBUS_CONNECTION_FAILED:
      perror("MODBUS_CONNECTION_FAILED");
      log("Failed to start:failed to connect to modbus");
      break;
    case Exception::OPEN_MMAP_FILE_FAILED:
      perror("OPEN_MMAP_FILE_FAILED:failed to open \"/tmp/radcontrol\"");
      log("Failed to start:failed to open \"/tmp/radcontrol\"");
      break;
    case Exception::RESIZE_MMAP_FILE_FAILED:
      perror("RESIZE_MMAP_FILE_FAILED:failed to resize \"/tmp/radcontrol\"");
      log("Failed to start:failed to resize \"/tmp/radcontrol\"");
      break;
    case Exception::MMAP_FAILED:
      perror("MMAP_FAILED");
      log("Failed to start:failed to create memory mapping");
      break;
    case Exception::OPEN_MESSAGE_QUEUE_FAILED:
      perror("OPEN_MESSAGE_QUEUE_FAILED");
      log("Failed to start:failed to create message queue");
      break;
  }
  modbus_free(ctx);
  return -1;
}catch(std::bad_alloc&e){
  perror("bad_alloc");
  log("Exception bad_alloc");
  return -1;
}
}
