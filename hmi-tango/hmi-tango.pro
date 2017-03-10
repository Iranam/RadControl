QT+=svg
SOURCES+=main.cc Detector.cc MainWindow.cc
HEADERS+=MainWindow.h Detector.h
LIBS+=-ltango -llog4tango -lomniORB4 -lomniDynamic4 -lCOS4 -lomnithread -lzmq -lpthread
TARGET=comp-map
QMAKE_CXXFLAGS=-std=c++11
CONFIG+=gui qt DEBUG
MOC_DIR=moc
OBJECTS_DIR=obj
INCLUDEPATH+=/usr/local/include/tango ..

run.target=run
run.commands=./$TARGET
run.depends=all
QMAKE_ADDITIONAL_TARGETS+=run
