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
#include<QGraphicsView>
#include<QGridLayout>
//#include<QPushButton>
//#include<QSignalMapper>
#include<QMessageBox>
#include<QGraphicsSvgItem>
#include"IPC.h"
using namespace detector;
#include"Detector.h"

uint NDETECTORS=9;
uint modbus_ids[]={1,2,3,4,5,6,7,20,21};
MainWindow*mainwindow;
static const int WIDTH=1024;
static const int HEIGHT=512;

MainWindow::MainWindow(QWidget *parent):
  QMainWindow(parent){
  mainwindow=this;
  //Open shared data file, where detector data is stored
  int fd=open("/tmp/radcontrol",O_RDONLY);
  if(fd==-1){
    QMessageBox messageBox;
    messageBox.critical(this,"Error","Failed to open shared-memory file /tmp/radcontrol\nTo resolve this problem, make sure RadControl daemon is running");
    exit(-1);
  }
  shmem=(char*)
    mmap(NULL,DATA_OFFSET+NDETECTORS*sizeof(DetectorData),PROT_READ,MAP_SHARED,fd,0);
  if(shmem==MAP_FAILED){
    //TODO errorbox
    cerr<<"mmap failed"<<endl;
    exit(-1);
  }
  ::close(fd);
  DetectorData*data=(DetectorData*)(shmem+DATA_OFFSET);
	QWidget*central_widget=new QWidget(this);
	setCentralWidget(central_widget);
  QGridLayout*layout=new QGridLayout(central_widget);
  scene=new QGraphicsScene(this);
  scene->setSceneRect(QRectF(0,0,WIDTH,HEIGHT));
  QGraphicsView*view=new QGraphicsView(scene);
  view->setDragMode(QGraphicsView::ScrollHandDrag);
  layout->addWidget(view);
  setGeometry(0,0,WIDTH+64,HEIGHT+64);
  //QGraphicsRectItem*rect=new QGraphicsRectItem(QRectF(50,50,150,150));
  //rect->setBrush(QBrush(Qt::green));
  //scene->addItem(rect);
  scene->addItem(new QGraphicsSvgItem("Background.svg"));
  timer=new QTimer(this);
  //Create detectors
  //Detector constructor adds it to the scene and connects it to timer
  new Detector(QPointF(64,64),data+0);
  new Detector(QPointF(176,64),data+8);
  new Detector(QPointF(448,64),data+1);
  new Detector(QPointF(256,144),data+3);
  new Detector(QPointF(368,144),data+7);
  new Detector(QPointF(544,144),data+4);
  new Detector(QPointF(720,144),data+5);
  new Detector(QPointF(912,144),data+6);
  new Detector(QPointF(192,272),data+2);
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
