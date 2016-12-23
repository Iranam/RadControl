#ifndef QLED_H
#define QLED_H
#include<QWidget>
#include<QResizeEvent>

using uint=unsigned int;
class QLed:public QWidget{Q_OBJECT
public:
  QLed(QWidget *parent);
  ~QLed();
	uint radius;
  QColor color;
	int heightForWidth(int width)const;
	QSize sizeHint()const; 
	QSize minimumSizeHint()const;
protected:
	void paintEvent(QPaintEvent*event);
public slots:
  void set_color(QColor);
  void set_state(bool);
};

#endif // QLED_H
