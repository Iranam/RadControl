QT+=svg
SOURCES+=main.cc Detector.cc MainWindow.cc
HEADERS+=MainWindow.h Detector.h
LIBS+=
TARGET=comp-map
QMAKE_CXXFLAGS=-std=c++11
CONFIG+=gui qt DEBUG
MOC_DIR=moc
OBJECTS_DIR=obj
INCLUDEPATH+=..

run.target=run
run.commands=./$TARGET
run.depends=all
QMAKE_ADDITIONAL_TARGETS+=run
