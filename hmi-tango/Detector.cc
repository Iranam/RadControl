#include"Detector.h"
#include"MainWindow.h"
#include<QBrush>
#include<QGraphicsSimpleTextItem>
#include<QGraphicsProxyWidget>
#include<QGraphicsLinearLayout>
//Detector::Detector(const QPointF&pos,QGraphicsItem*parent):
const int HEIGHT=48;
const int WIDTH=96;
Detector::Detector(const QPointF&pos,string attr_name,QGraphicsItem*parent):
  QGraphicsWidget(parent),attr(attr_name){
  setGeometry(QRectF(pos,QSizeF(WIDTH,HEIGHT)));
  body=new QGraphicsRectItem(0,0,WIDTH,HEIGHT,this);
  body->setBrush(QBrush(qRgb(128,0,0)));
  QGraphicsProxyWidget*line_proxy=new QGraphicsProxyWidget(this);
  line=new QLineEdit();
  line->setToolTip(QString("Background(%1Sv/h)").arg(QChar(0x03bc)));
  line_proxy->setWidget(line);
  line_proxy->setGeometry(QRectF(2,HEIGHT-26,WIDTH-34,18));
  QGraphicsSimpleTextItem*name=new QGraphicsSimpleTextItem(this);
 // name->setPos(QPointF(24,4));
  name->setPos(QPointF(4,4));
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wwrite-strings"
#pragma GCC diagnostic push
  attr_name.erase(attr_name.find_last_of('/'));
  attr_name.erase(0,attr_name.find_first_of('/')+1);
  name->setText(attr_name.c_str());
  QGraphicsSimpleTextItem*units=new QGraphicsSimpleTextItem(this);
  units->setPos(QPointF(64,26));
  units->setText(QString::fromUtf8("\u00B5Sv/h"));
  update();
  mainwindow->scene->addItem(this);
  connect(mainwindow->timer,SIGNAL(timeout()),this,SLOT(update()));
}
void Detector::update(){
  Tango::DevState state=attr.state();
  if(state==Tango::ON||state==Tango::ALARM){
    body->setBrush(QBrush(qRgb(138,226,52)));
    Tango::DeviceAttribute dev_attr=attr.read();
    float bg;
    dev_attr>>bg;
    QString text=QString::fromUtf8("%1").arg(bg,0,'f',3);
    if(bg<0.09)line->setStyleSheet("QLineEdit{color:black;background:rgb(138,226,52);}");
    else if(bg<10)line->setStyleSheet("QLineEdit{color:black;background:yellow;}");
    else line->setStyleSheet("QLineEdit{color:white;background:red;}");
    line->setText(text);
  }else{
    line->setStyleSheet("QLineEdit{color:black;background:gray;}");
    line->setText("NO DATA");
    QColor c;
    switch(state){
      case Tango::OFF:c=qRgb(180,180,180);break;
      case Tango::FAULT:c=qRgb(243,57,27);break;
      case Tango::INIT:c=qRgb(133,181,232);break;
      default:c=qRgb(255,0,154);
    }
    body->setBrush(QBrush(c));
  }
}
/*
void Detector::mousePressEvent(QGraphicsSceneMouseEvent*event){
  //QTIGNORE(event)
  body->setBrush(QBrush(qRgb(0,128,0)));
}
*/
