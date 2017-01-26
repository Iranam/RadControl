#ifndef DETECTOR_H
#define DETECTOR_H
#include"DetectorData.h"
#include"interpolant.h"
#include<string>
namespace detector{
typedef unsigned char uchar;
class Detector{
public:
  ~Detector();
  void init(DetectorData*);
  void update();
  void set_exposure(uint val);
  void set_exposure_by_count(uint val);
  void set_sensitivity(float val);
  int time_to_update;//milliseconds, set by update(), decreased by main()
  DetectorData*d;
private:
  bool configured;
  bool data_ready;
  bool problem_calibration;
  bool problem_connection;
  interpolant::Interpolant*calibration;//only for gamma
  void detector_log(std::string msg);
  void switch_slave();
  void handle_connection_loss();
};
}
void log(std::string msg);
#endif
