#ifndef DETECTOR_DATA_H
#define DETECTOR_DATA_H
#include<cstdint>
typedef unsigned int uint;
namespace detector{
enum class DetectorType{UNKNOWN_TYPE,GAMMA,NEUTRON};
enum class DetectorState{OK,DISABLED,INIT,NO_CONNECTION,NO_CALIBRATION,UNKNOWN};
//Memory is shared through memory-mapped file /tmp/radcontrol
//The file bytes are interpreted as:
//1-byte - state of RadControl daemon(WIP)
//1-byte - number of detectors
//Array of DetectorData
const uint8_t NUMBER_OFFSET=1;
const uint8_t DATA_OFFSET=2;
//OK means data is valid
//DISABLED means RadControl process has finished
//INIT means data is invalid, and has not yet been actually read from device
//NO_CONNECTION means data is invalid, and device is unavailable
//NO_CALIBRATION means all data except background is valid, and background is invalid
//UNKNOWN means something has gone terribly wrong
//DetectorData shall only be written by RadControl daemon
struct DetectorData{
  DetectorType type;
  DetectorState state;
  uint modbus_id;
  uint count;
  float background;
  uint16_t exposure,
       exposure_by_count;//neutron only
  float sensitivity;//neutron only
};
//RadControl daemon accepts configuration requests thorugh message queue called "/RadControl"
//command message has the following format:
//a command code(1 byte), then arguments;
//followed by command arguments(uint16_t = 2 bytes)
//Here are the commands
enum class Command:char{//arguments:
  SHUTDOWN=0,//turn off daemon; no arguments
  SET_EXPOSURE=1,//uchar modbus_id of the detector;uint16_t - new exposure in milliseconds
  SET_EXPOSURE_BY_COUNT=2,//same
  SET_SENSITIVITY=3//same
};
//therefore
const uint MESSAGE_MAX_SIZE=6;//in bytes
}
#endif
