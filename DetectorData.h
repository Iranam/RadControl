#ifndef DETECTOR_DATA_H
#define DETECTOR_DATA_H
typedef unsigned int uint;
namespace detector{
enum class DetectorType{UNKNOWN_TYPE,GAMMA,NEUTRON};
enum class DetectorState{OK,DISABLED,INIT,NO_CONNECTION,NO_CALIBRATION,UNKNOWN};
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
  uint exposure,
       exposure_by_count;//neutron only
  float sensitivity;//neutron only
};
}
#endif
