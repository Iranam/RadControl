#ifndef DETECTOR_H
#define DETECTOR_H
#include<QGraphicsWidget>
#include<QGraphicsRectItem>
#include<QLineEdit>
#include"DetectorData.h"
using namespace detector;

//How do layouts work for graphics scenes?

class Detector:public QGraphicsWidget{Q_OBJECT
public:
  Detector(const QPointF&pos,DetectorData*data,QGraphicsItem*parent=0);
  //Detector(const QPointF&pos,DetectorData*data,QGraphicsItem*parent=0);
  //Detector(DetectorData*data,QWidget*parent=0);
  //~Detector();
protected:
  //void mousePressEvent(QGraphicsSceneMouseEvent*event);
private:
  QLineEdit*line;
  QGraphicsRectItem*body;
  DetectorData*data;
public slots:
  void update();
};

/*
class DetectorWindow:public QWidget{Q_OBJECT
public:
  DetectorWindow();
private:
  QLineEdit*line_exp,*line_expc;
  DetectorData*data;
public slots:
  void update();
  void setExp();//set exposition
  void setExpc();//set exposition by count
//how to connect DetectorWindow to timer?
}
*/
#endif // DETECTOR_H
