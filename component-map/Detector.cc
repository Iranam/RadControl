#include"Detector.h"
#include"MainWindow.h"
#include<QBrush>
#include<QGraphicsSimpleTextItem>
#include<QGraphicsProxyWidget>
#include<QGraphicsLinearLayout>
//Detector::Detector(const QPointF&pos,QGraphicsItem*parent):
const int HEIGHT=48;
const int WIDTH=96;
Detector::Detector(const QPointF&pos,DetectorData*ndata,QGraphicsItem*parent):
  QGraphicsWidget(parent){
  setGeometry(QRectF(pos,QSizeF(WIDTH,HEIGHT)));
  body=new QGraphicsRectItem(0,0,WIDTH,HEIGHT,this);
  body->setBrush(QBrush(qRgb(128,0,0)));
  data=ndata;
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
  char*detector_name;
  switch(data->type){
    case DetectorType::NEUTRON:detector_name="Neutron";break;
    case DetectorType::GAMMA:detector_name="Gamma";break;
    default:detector_name="Unknown";
  }
#pragma GCC diagnostic pop
  name->setText(QString("%1 %2").arg(detector_name).arg(data->modbus_id));
  QGraphicsSimpleTextItem*units=new QGraphicsSimpleTextItem(this);
  units->setPos(QPointF(64,26));
  units->setText(QString::fromUtf8("\u00B5Sv/h"));
  update();
  mainwindow->scene->addItem(this);
  connect(mainwindow->timer,SIGNAL(timeout()),this,SLOT(update()));
}
void Detector::update(){
  DetectorData d=*data;
  if(d.state==DetectorState::OK){
    body->setBrush(QBrush(qRgb(138,226,52)));
    float bg=d.background;
    QString text=QString::fromUtf8("%1").arg(bg,0,'f',3);
    if(bg<0.09)line->setStyleSheet("QLineEdit{color:black;background:rgb(138,226,52);}");
    else if(bg<10)line->setStyleSheet("QLineEdit{color:black;background:yellow;}");
    else line->setStyleSheet("QLineEdit{color:white;background:red;}");
    line->setText(text);
  }else{
    line->setStyleSheet("QLineEdit{color:black;background:gray;}");
    line->setText("NO DATA");
    QColor c;
    switch(d.state){
      case DetectorState::DISABLED:c=qRgb(200,200,200);break;
      case DetectorState::NO_CONNECTION:c=qRgb(243,57,27);break;
      case DetectorState::INIT:c=qRgb(133,181,232);break;
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
