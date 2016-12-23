#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include<QMainWindow>
#include"DetectorData.h"
using namespace detector;

typedef unsigned int uint;

class MainWindow:public QMainWindow{Q_OBJECT
public:
  explicit MainWindow(QWidget *parent=0);
  ~MainWindow();
private:
  char*shmem;
};
#endif // MAINWINDOW_H
