#include"MainWindow.h"
#include<QApplication>

int main(int argc, char *argv[]){
  QApplication app(argc,argv);
  (new MainWindow())->show();
  return app.exec();
}
