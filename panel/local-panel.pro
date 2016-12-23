SOURCES+=main.cc MainWindow.cc
HEADERS+=MainWindow.h
LIBS+=
TARGET=local-panel
QMAKE_CXXFLAGS=-std=c++11
CONFIG+=gui qt DEBUG
MOC_DIR=moc
OBJECTS_DIR=obj
INCLUDEPATH+=..

run.target=run
run.commands=./$TARGET
run.depends=all
