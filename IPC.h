#ifndef IPC_H
#define IPC_H
#include"DetectorData.h"
typedef unsigned int uint;
//typedef unsigned char uchar;
namespace detector{
//Memory is shared through memory-mapped file /tmp/radcontrol
//The file bytes are interpreted as:
//1-byte - state of RadControl daemon(WIP)
//1-byte - number of detectors
//Array of DetectorData

const char*MQNAME="/RadControl";
const uint8_t NUMBER_OFFSET=1;
const uint8_t DATA_OFFSET=2;
//RadControl daemon accepts configuration requests thorugh message queue called "/RadControl"
//command message has the following format:
//a command code(1 byte), then arguments;
//followed by command arguments(uint16_t = 2 bytes)
//Here are the commands
//TODO
enum class Command:char{//arguments:
  SHUTDOWN=0,//turn off daemon; no arguments
  SET_EXPOSURE=1,//uchar modbus_id of the detector;uint32 - new exposure in milliseconds
  SET_EXPOSURE_BY_COUNT=2,//same
  SET_SENSITIVITY=3//same
};
//therefore
const uint MESSAGE_MAX_SIZE=6;//in bytes
}
#endif
