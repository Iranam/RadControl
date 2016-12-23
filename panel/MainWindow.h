#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include<QMainWindow>
#include<QLabel>
#include<QLineEdit>
#include<QGridLayout>
//#include<QPushButton>
#include<QTimer>
#include<QSignalMapper>
#include<vector>
using std::vector;
#include"DetectorData.h"
using namespace detector;

typedef unsigned int uint;

class MainWindow:public QMainWindow{Q_OBJECT
public:
  explicit MainWindow(QWidget *parent=0);
  ~MainWindow();
private:
  char*shmem;
  DetectorData*data;
  QLabel*labels;
  QLineEdit*readings;
  QLineEdit*exposition;
  QLineEdit*exposition_by_count;
	QWidget*central_widget;
	QTimer*timer;
public slots:
  void update();
  void set_exposition(int id);
  void set_exposition_by_count(int id);
};
/*
 * QPalette palette;
 * palette.setColor(QPalette::Base, Qt::black);
 * palette.setColor(QPalette::Background, Qt::black);
 * le.setPalette(palette);  
 */

#endif // MAINWINDOW_H
