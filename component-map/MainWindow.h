#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include<QMainWindow>
#include<QGraphicsScene>
#include<QTimer>

typedef unsigned int uint;

class MainWindow:public QMainWindow{Q_OBJECT
public:
  static MainWindow*inst;
  explicit MainWindow(QWidget*parent=0);
  ~MainWindow();
  QGraphicsScene*scene;
	QTimer*timer;//other object connect themselves to this timer at creation
private:
  char*shmem;
};
extern MainWindow*mainwindow;
#endif // MAINWINDOW_H
