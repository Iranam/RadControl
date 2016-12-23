#include"MainWindow.h"
#include<iostream>
using namespace std;
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/mman.h>
#include<unistd.h>
#include<fcntl.h>
#include<QStatusBar>
#include<QString>
#include<QGraphicsScene>
#include<QGraphicsView>
#include<QGridLayout>
//#include<QPushButton>
#include<QTimer>
//#include<QSignalMapper>
#include<QMessageBox>
#include<QGraphicsSvgItem>
#include"Detector.h"

uint NDETECTORS=9;
uint modbus_ids[]={1,2,3,4,5,6,7,20,21};

MainWindow::MainWindow(QWidget *parent):
  QMainWindow(parent){
  //Open shared data file, where detector data is stored
  int fd=open("/tmp/radcontrol",O_RDONLY);
  if(fd==-1){
    QMessageBox messageBox;
    messageBox.critical(this,"Error","Failed to open shared-memory file /tmp/radcontrol\nTo resolve this problem, make sure RadControl daemon is running");
    exit(-1);
  }
  shmem=(char*)
    mmap(NULL,4+NDETECTORS*sizeof(DetectorData),PROT_READ,MAP_SHARED,fd,0);
  if(shmem==MAP_FAILED){
    //TODO errorbox
    cerr<<"mmap failed"<<endl;
    exit(-1);
  }
  ::close(fd);
  DetectorData*data=(DetectorData*)(shmem+4);//first 4 bytes are number of detectors
	QWidget*central_widget=new QWidget(this);
	setCentralWidget(central_widget);
  QGridLayout*layout=new QGridLayout(central_widget);
  QGraphicsScene*scene=new QGraphicsScene(this);
  scene->setSceneRect(QRectF(0,0,800,512));
  QGraphicsView*view=new QGraphicsView(scene);
  view->setDragMode(QGraphicsView::ScrollHandDrag);
  layout->addWidget(view);
  setGeometry(0,0,864,640);
  //QGraphicsRectItem*rect=new QGraphicsRectItem(QRectF(50,50,150,150));
  //rect->setBrush(QBrush(Qt::green));
  //scene->addItem(rect);
  scene->addItem(new QGraphicsSvgItem("Background.svg"));
	QTimer*timer=new QTimer(this);
  Detector*d=new Detector(QPointF(96,64),data+0);
  scene->addItem(d);
  connect(timer,SIGNAL(timeout()),d,SLOT(update()));
  d=new Detector(QPointF(192,64),data+8);
  scene->addItem(d);
  connect(timer,SIGNAL(timeout()),d,SLOT(update()));
  d=new Detector(QPointF(448,64),data+1);
  scene->addItem(d);
  connect(timer,SIGNAL(timeout()),d,SLOT(update()));
  d=new Detector(QPointF(64,144),data+3);
  scene->addItem(d);
  connect(timer,SIGNAL(timeout()),d,SLOT(update()));
  d=new Detector(QPointF(160,144),data+7);
  scene->addItem(d);
  connect(timer,SIGNAL(timeout()),d,SLOT(update()));
  d=new Detector(QPointF(320,144),data+4);
  scene->addItem(d);
  connect(timer,SIGNAL(timeout()),d,SLOT(update()));
  d=new Detector(QPointF(512,144),data+5);
  scene->addItem(d);
  connect(timer,SIGNAL(timeout()),d,SLOT(update()));
  d=new Detector(QPointF(704,144),data+6);
  scene->addItem(d);
  connect(timer,SIGNAL(timeout()),d,SLOT(update()));
  d=new Detector(QPointF(128,272),data+2);
  scene->addItem(d);
  connect(timer,SIGNAL(timeout()),d,SLOT(update()));
  //scene->addItem(new Detector(QPointF(0,64)));
  //scene->addItem(new Detector(QPointF(100,100)));
	timer->start(1000);
}
/*
*MainWindow opens shared memory file and creates detector objects
*MainWindow is responsible for shared memory
*each Detector object gets pointer to its own DetectorData and works with it
*Detectors update color/label by slot update()
*/
MainWindow::~MainWindow(){
  munmap(shmem,4+NDETECTORS*sizeof(DetectorData));
}
