#include"MainWindow.h"
#include<QStatusBar>
#include<QString>
#include<iostream>
using namespace std;
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/mman.h>
#include<unistd.h>
#include<fcntl.h>

uint NDETECTORS=9;
uint modbus_ids[]={1,2,3,4,5,6,7,20,21};

MainWindow::MainWindow(QWidget *parent):
  QMainWindow(parent){
  //Open shared data file, where detector data is stored
  int fd=open("/tmp/radcontrol",O_RDWR);
  //if(fd==-1)throw OPEN_MMAP_FILE_FAILED;
  shmem=(char*)
    mmap(NULL,4+NDETECTORS*sizeof(DetectorData),PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
  if(shmem==MAP_FAILED){
    cerr<<"mmap failed"<<endl;
    exit(-1);
  }
  ::close(fd);
  data=(DetectorData*)(shmem+4);//first 4 bytes are number of detectors
  //for now we ignore it
	setWindowTitle("Rad-Control");
	//resize(400,300);
	central_widget=new QWidget(this);
	setCentralWidget(central_widget);
  QGridLayout*layout=new QGridLayout();
	central_widget->setLayout(layout);
  QLabel*label_units=new QLabel(this);
  label_units->setText(QString("Background(%1Sv/h)").arg(QChar(0x03bc)));
  layout->addWidget(label_units,0,1);
  QLabel*label_exposition=new QLabel(this);
  label_exposition->setText("Exposition by time");
  layout->addWidget(label_exposition,0,2);
  QLabel*label_exposition_by_count=new QLabel(this);
  label_exposition_by_count->setText("Exposition by count");
  layout->addWidget(label_exposition_by_count,0,3);
  labels=new QLabel[NDETECTORS];
  readings=new QLineEdit[NDETECTORS];
  exposition=new QLineEdit[NDETECTORS];
  exposition_by_count=new QLineEdit[NDETECTORS];
	QSignalMapper*mapper=new QSignalMapper(this);
	QSignalMapper*mapper2=new QSignalMapper(this);
  for(uint i=0;i<NDETECTORS;++i){
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wwrite-strings"
    char*detector_name;
    switch(data[i].type){
      case DetectorType::NEUTRON:detector_name="Neutron";break;
      case DetectorType::GAMMA:detector_name="Gamma";break;
      default:detector_name="Unknown";
    }
    labels[i].setText(QString("%1 %2").arg(detector_name).arg(modbus_ids[i]));
    readings[i].setReadOnly(true);
    exposition[i].setText(QString("%1").arg(data[i].exposition));
    exposition_by_count[i].setText(QString("%1").arg(data[i].exposition_by_count));
    layout->addWidget(&labels[i],i+1,0);
    layout->addWidget(&readings[i],i+1,1);
    layout->addWidget(&exposition[i],i+1,2);
    layout->addWidget(&exposition_by_count[i],i+1,3);
		connect(&exposition[i],SIGNAL(returnPressed()),mapper,SLOT(map()));
		mapper->setMapping(&exposition[i],i);
		connect(&exposition_by_count[i],SIGNAL(returnPressed()),mapper2,SLOT(map()));
		mapper2->setMapping(&exposition_by_count[i],i);
#pragma GCC diagnostic pop
  }
	connect(mapper,SIGNAL(mapped(int)),this,SLOT(set_exposition(int)));
	connect(mapper2,SIGNAL(mapped(int)),this,SLOT(set_exposition_by_count(int)));
	//statusBar()->setSizeGripEnabled(false);
	timer=new QTimer(this);
  connect(timer,SIGNAL(timeout()),this,SLOT(update()));
	timer->start(3000);
  update();
}
/*
*MainWindow opens shared memory file and creates detector objects
*MainWindow is responsible for shared memory
*each Detector object gets pointer to its own DetectorData and works with it
*Detectors update color/label by slot update()
*/
MainWindow::~MainWindow(){
  delete timer;
  delete[]labels;
  delete[]readings;
  delete[]exposition;
  delete[]exposition_by_count;
  delete central_widget;
  munmap(shmem,4+NDETECTORS*sizeof(DetectorData));
}
void MainWindow::update(){
//  clog<<"update()"<<endl;
  for(uint i=0;i<NDETECTORS;++i){
    if(data[i].state==DetectorState::OK){
      QString text;
      float bg=data[i].background;
      text.sprintf("%.3f",bg);
      if(bg<0.09)readings[i].setStyleSheet("QLineEdit{color:black;background:rgb(138,226,52);}");
      else if(bg<10)readings[i].setStyleSheet("QLineEdit{color:black;background:yellow;}");
      else readings[i].setStyleSheet("QLineEdit{color:white;background:red;}");
      readings[i].setText(text);
    }else{
      readings[i].setStyleSheet("QLineEdit{color:black;background:gray;}");
      readings[i].setText("NO DATA");
    }
  }
}
void MainWindow::set_exposition(int i){
  clog<<"set_exposition()"<<i<<endl;
	bool ok;
	int val=exposition[i].text().toInt(&ok);
	if(ok&&val>=0){
		data[i].exposition=val;
		data[i].touched=true;
		msync(data+i*sizeof(DetectorData),sizeof(DetectorData),MS_ASYNC|MS_INVALIDATE);
	}
}
void MainWindow::set_exposition_by_count(int i){
  clog<<"set_exposition_by_count()"<<i<<endl;
	bool ok;
	int val=exposition_by_count[i].text().toInt(&ok);
	if(ok&&val>=0){
		data[i].exposition_by_count=val;
		data[i].touched=true;
		msync(data+i*sizeof(DetectorData),sizeof(DetectorData),MS_ASYNC|MS_INVALIDATE);
	}
}
