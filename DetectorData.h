#ifndef DETECTOR_DATA_H
#define DETECTOR_DATA_H
typedef unsigned int uint;
//typedef unsigned char uchar;
namespace detector{
enum class DetectorType{UNKNOWN_TYPE,GAMMA,NEUTRON};
enum class DetectorState{OK,DISABLED,INIT,NO_CONNECTION,NO_CALIBRATION,UNKNOWN};
//OK means data is valid
//DISABLED means RadControl process has finished
//INIT means data is invalid, and has not yet been actually read from device
//NO_CONNECTION means data is invalid, and device is unavailable
//NO_CALIBRATION means all data except background is valid, and background is invalid
//UNKNOWN means something has gone terribly wrong
struct DetectorData{
  DetectorType type;
  DetectorState state;
  uint modbus_id;
  uint count;
  float background;
  uint exposition, exposition_by_count;//TO BE REPLACED:writable
	bool touched;//TO BE REPLACED:client sets to true after setting writeable fields, daemon will reset to false and reconfigure device
};
//Memory is shared through memory-mapped file /tmp/radcontrol
//The file bytes are interpreted as:(WIP)
//4-byte int - number of detectors
//Array of DetectorData

//(WIP)Only RadControl daemon is supposed to write shared memory.
//RadControl daemon accepts configuration requests thorugh message queue called "/RadControl"
//command message has the following format:
//a command code(1 byte), followed by detector number(1 byte)(not modbus_id,
//but index in DetectprData* array),
//followed by command arguments(uint16_t = 2 bytes)
//Here are the commands
//TODO
enum class Command{//arguments:
  SET_EXPOSITION,//4-byte uint modbus_id of the detector;4-byte uint - new exposition in milliseconds
  SET_EXPOSITION_BY_COUNT,//same
  SET_SENSITIVITY//same
};
//therefore, message size is 4 bytes
}
#endif
