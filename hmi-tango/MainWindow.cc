#include"MainWindow.h"
#include<iostream>
#include<string>
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
	QWidget*central_widget=new QWidget(this);
	setCentralWidget(central_widget);
  QGridLayout*layout=new QGridLayout(central_widget);
  scene=new QGraphicsScene(this);
  scene->setSceneRect(QRectF(0,0,WIDTH,HEIGHT));
  QGraphicsView*view=new QGraphicsView(scene);
  view->setDragMode(QGraphicsView::ScrollHandDrag);
  layout->addWidget(view);
  setGeometry(0,0,WIDTH+64,HEIGHT+64);
  scene->addItem(new QGraphicsSvgItem("Background.svg"));
  timer=new QTimer(this);
  //Create detectors
  //Detector constructor adds it to the scene and connects it to timer
  const string p="rd_ctrl/";
  const string s="/doserate";
  try{
  new Detector(QPointF(64,64),p+"gamma/1"+s);
  new Detector(QPointF(176,64),p+"neutron/21"+s);
  new Detector(QPointF(448,64),p+"gamma/2"+s);
  new Detector(QPointF(256,144),p+"gamma/4"+s);
  new Detector(QPointF(368,144),p+"neutron/20"+s);
  new Detector(QPointF(544,144),p+"gamma/5"+s);
  new Detector(QPointF(720,144),p+"gamma/6"+s);
  new Detector(QPointF(912,144),p+"gamma/7"+s);
  new Detector(QPointF(192,272),p+"gamma/3"+s);
  }catch(Tango::DevFailed&e){
    Tango::Except::print_exception(e);
    exit(-1);
  }
	timer->start(1000);
}
/*
*MainWindow opens shared memory file and creates detector objects
*MainWindow is responsible for shared memory
*each Detector object gets pointer to its own DetectorData and works with it
*Detectors update color/label by slot update()
*/
MainWindow::~MainWindow(){
  return;
}
