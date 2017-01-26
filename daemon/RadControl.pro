TEMPLATE=app
QT+=core sql
DEFINES+=DUMMY #DEBUG
DEFINES+=NODATABASE
CONFIG+=Debug #Release
MODBUS_INCLUDES=`pkg-config --cflags libmodbus`
MODBUS_LFLAGS=`pkg-config --libs libmodbus`
QMAKE_CXXFLAGS+=$$MODBUS_INCLUDES 
QMAKE_CXXFLAGS+=-std=c++11
LIBS+=$$MODBUS_LFLAGS
LIBS+=-lrt #message queues
MOC_DIR=moc
OBJECTS_DIR=obj
INCLUDEPATH+=..
HEADERS=../DetectorData.h\
../IPC.h\
Detector.h\
interpolant.h\
modbus_dummy.h
SOURCES=Detector.cc\
interpolant.cc\
modbus_dummy.cc\
main.cc

run.target=run
run.commands=./$TARGET
run.depends=all
QMAKE_ADDITIONAL_TARGETS+=run
