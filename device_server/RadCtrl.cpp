/*----- PROTECTED REGION ID(RadCtrl.cpp) ENABLED START -----*/
static const char *RcsId = "$Id:  $";

#include<RadCtrl.h>
#include<RadCtrlClass.h>
using namespace detector;
#include<string>
using std::string;
#include<mqueue.h>
extern mqd_t message_queue;//defined in main.cc

/*----- PROTECTED REGION END -----*/	//	RadCtrl.cpp

/**
 *  RadCtrl class description:
 *    Controls gamma or neutron detectors
 */

//================================================================
//  The following table gives the correspondence
//  between command and method names.
//
//  Command name  |  Method name
//================================================================
//  State         |  Inherited (no method)
//  Status        |  Inherited (no method)
//================================================================

//================================================================
//  Attributes managed are:
//================================================================
//  count       |  Tango::DevULong	Scalar
//  background  |  Tango::DevFloat	Scalar
//  exposure    |  Tango::DevUShort	Scalar
//  modbus_id   |  Tango::DevUChar	Scalar
//================================================================

namespace RadCtrl_ns
{
/*----- PROTECTED REGION ID(RadCtrl::namespace_starting) ENABLED START -----*/
/*----- PROTECTED REGION END -----*/	//	RadCtrl::namespace_starting

//--------------------------------------------------------
/**
 *	Method      : RadCtrl::RadCtrl()
 *	Description : Constructors for a Tango device
 *                implementing the classRadCtrl
 */
//--------------------------------------------------------
RadCtrl::RadCtrl(Tango::DeviceClass *cl, string &s)
 : TANGO_BASE_CLASS(cl, s.c_str())
{
	/*----- PROTECTED REGION ID(RadCtrl::constructor_1) ENABLED START -----*/
	init_device();
	/*----- PROTECTED REGION END -----*/	//	RadCtrl::constructor_1
}
//--------------------------------------------------------
RadCtrl::RadCtrl(Tango::DeviceClass *cl, const char *s)
 : TANGO_BASE_CLASS(cl, s)
{
	/*----- PROTECTED REGION ID(RadCtrl::constructor_2) ENABLED START -----*/
	init_device();
	/*----- PROTECTED REGION END -----*/	//	RadCtrl::constructor_2
}
//--------------------------------------------------------
RadCtrl::RadCtrl(Tango::DeviceClass *cl, const char *s, const char *d)
 : TANGO_BASE_CLASS(cl, s, d)
{
	/*----- PROTECTED REGION ID(RadCtrl::constructor_3) ENABLED START -----*/
	init_device();
	/*----- PROTECTED REGION END -----*/	//	RadCtrl::constructor_3
}

//--------------------------------------------------------
/**
 *	Method      : RadCtrl::delete_device()
 *	Description : will be called at device destruction or at init command
 */
//--------------------------------------------------------
void RadCtrl::delete_device()
{
	DEBUG_STREAM << "RadCtrl::delete_device() " << device_name << endl;
	/*----- PROTECTED REGION ID(RadCtrl::delete_device) ENABLED START -----*/
	//	Delete device allocated objects
	/*----- PROTECTED REGION END -----*/	//	RadCtrl::delete_device
	delete[] attr_count_read;
	delete[] attr_background_read;
	delete[] attr_exposure_read;
	delete[] attr_modbus_id_read;
}

//--------------------------------------------------------
/**
 *	Method      : RadCtrl::init_device()
 *	Description : will be called at device initialization.
 */
//--------------------------------------------------------
void RadCtrl::init_device()
{
	DEBUG_STREAM << "RadCtrl::init_device() create device " << device_name << endl;
	/*----- PROTECTED REGION ID(RadCtrl::init_device_before) ENABLED START -----*/
	//	Initialization before get_device_property() call
	/*----- PROTECTED REGION END -----*/	//	RadCtrl::init_device_before
	
	//	No device property to be read from database
	
	attr_count_read = new Tango::DevULong[1];
	attr_background_read = new Tango::DevFloat[1];
	attr_exposure_read = new Tango::DevUShort[1];
	attr_modbus_id_read = new Tango::DevUChar[1];
	/*----- PROTECTED REGION ID(RadCtrl::init_device) ENABLED START -----*/
	//	Initialize device
	/*----- PROTECTED REGION END -----*/	//	RadCtrl::init_device
}


//--------------------------------------------------------
/**
 *	Method      : RadCtrl::always_executed_hook()
 *	Description : method always executed before any command is executed
 */
//--------------------------------------------------------
void RadCtrl::always_executed_hook()
{
	DEBUG_STREAM << "RadCtrl::always_executed_hook()  " << device_name << endl;
	/*----- PROTECTED REGION ID(RadCtrl::always_executed_hook) ENABLED START -----*/
	//	code always executed before all requests
	/*----- PROTECTED REGION END -----*/	//	RadCtrl::always_executed_hook
}

//--------------------------------------------------------
/**
 *	Method      : RadCtrl::read_attr_hardware()
 *	Description : Hardware acquisition for attributes
 */
//--------------------------------------------------------
void RadCtrl::read_attr_hardware(TANGO_UNUSED(vector<long> &attr_list))
{
	DEBUG_STREAM << "RadCtrl::read_attr_hardware(vector<long> &attr_list) entering... " << endl;
	/*----- PROTECTED REGION ID(RadCtrl::read_attr_hardware) ENABLED START -----*/
	//	Add your own code
	/*----- PROTECTED REGION END -----*/	//	RadCtrl::read_attr_hardware
}
//--------------------------------------------------------
/**
 *	Method      : RadCtrl::write_attr_hardware()
 *	Description : Hardware writing for attributes
 */
//--------------------------------------------------------
void RadCtrl::write_attr_hardware(TANGO_UNUSED(vector<long> &attr_list))
{
	DEBUG_STREAM << "RadCtrl::write_attr_hardware(vector<long> &attr_list) entering... " << endl;
	/*----- PROTECTED REGION ID(RadCtrl::write_attr_hardware) ENABLED START -----*/
	
	//	Add your own code
	
	/*----- PROTECTED REGION END -----*/	//	RadCtrl::write_attr_hardware
}

//--------------------------------------------------------
/**
 *	Read attribute count related method
 *	Description: Count during last exposition period
 *
 *	Data type:	Tango::DevULong
 *	Attr type:	Scalar
 */
//--------------------------------------------------------
void RadCtrl::read_count(Tango::Attribute &attr)
{
	DEBUG_STREAM << "RadCtrl::read_count(Tango::Attribute &attr) entering... " << endl;
	/*----- PROTECTED REGION ID(RadCtrl::read_count) ENABLED START -----*/
  DetectorState S=data->state;
  if(S==DetectorState::OK||S==DetectorState::NO_CALIBRATION){
    *attr_count_read=data->count;
    attr.set_value(attr_count_read);
    attr.set_quality(Tango::ATTR_VALID);//TODO alarm and warning
  }else{
    attr.set_quality(Tango::ATTR_INVALID);
  }
  determine_state();
	/*----- PROTECTED REGION END -----*/	//	RadCtrl::read_count
}
//--------------------------------------------------------
/**
 *	Read attribute background related method
 *	Description: Estimate of radiation background.
 *
 *	Data type:	Tango::DevFloat
 *	Attr type:	Scalar
 */
//--------------------------------------------------------
void RadCtrl::read_background(Tango::Attribute &attr)
{
	DEBUG_STREAM << "RadCtrl::read_background(Tango::Attribute &attr) entering... " << endl;
	/*----- PROTECTED REGION ID(RadCtrl::read_background) ENABLED START -----*/
  DetectorState S=data->state;
  if(S==DetectorState::OK){
    *attr_background_read=data->background;
    attr.set_value(attr_background_read);
    attr.set_quality(Tango::ATTR_VALID);
  }else{
    attr.set_quality(Tango::ATTR_INVALID);
  }
  determine_state();
	/*----- PROTECTED REGION END -----*/	//	RadCtrl::read_background
}
//--------------------------------------------------------
/**
 *	Read attribute exposure related method
 *	Description: 
 *
 *	Data type:	Tango::DevUShort
 *	Attr type:	Scalar
 */
//--------------------------------------------------------
void RadCtrl::read_exposure(Tango::Attribute &attr)
{
	DEBUG_STREAM << "RadCtrl::read_exposure(Tango::Attribute &attr) entering... " << endl;
	/*----- PROTECTED REGION ID(RadCtrl::read_exposure) ENABLED START -----*/
  DetectorState S=data->state;
  if(S==DetectorState::OK||S==DetectorState::NO_CALIBRATION){
    *attr_exposure_read=data->exposure;
    attr.set_value(attr_exposure_read);
    attr.set_quality(Tango::ATTR_VALID);
  }else{
    attr.set_quality(Tango::ATTR_INVALID);
  }
  determine_state();
	/*----- PROTECTED REGION END -----*/	//	RadCtrl::read_exposure
}
//--------------------------------------------------------
/**
 *	Write attribute exposure related method
 *	Description: 
 *
 *	Data type:	Tango::DevUShort
 *	Attr type:	Scalar
 */
//--------------------------------------------------------
void RadCtrl::write_exposure(Tango::WAttribute &attr)
{
	DEBUG_STREAM<<"RadCtrl::write_exposure(Tango::WAttribute &attr) entering... "<<endl;
	//	Retrieve write value
	Tango::DevUShort w_val;
	attr.get_write_value(w_val);
	/*----- PROTECTED REGION ID(RadCtrl::write_exposure) ENABLED START -----*/
  char buf[4];
  buf[0]=char(detector::Command::SET_EXPOSURE);
  buf[1]=uchar(data->modbus_id);
  buf[2]=uchar(w_val);
  buf[3]=uchar(w_val>>8);
 	mq_send(message_queue,buf,sizeof(buf),1);
	/*----- PROTECTED REGION END -----*/	//	RadCtrl::write_exposure
}
//--------------------------------------------------------
/**
 *	Read attribute modbus_id related method
 *	Description: 
 *
 *	Data type:	Tango::DevUChar
 *	Attr type:	Scalar
 */
//--------------------------------------------------------
void RadCtrl::read_modbus_id(Tango::Attribute &attr)
{
	DEBUG_STREAM << "RadCtrl::read_modbus_id(Tango::Attribute &attr) entering... " << endl;
	/*----- PROTECTED REGION ID(RadCtrl::read_modbus_id) ENABLED START -----*/
  *attr_modbus_id_read=data->modbus_id;
	attr.set_value(attr_modbus_id_read);
  attr.set_quality(Tango::ATTR_VALID);
	/*----- PROTECTED REGION END -----*/	//	RadCtrl::read_modbus_id
}

//--------------------------------------------------------
/**
 *	Method      : RadCtrl::add_dynamic_attributes()
 *	Description : Create the dynamic attributes if any
 *                for specified device.
 */
//--------------------------------------------------------
void RadCtrl::add_dynamic_attributes()
{
	/*----- PROTECTED REGION ID(RadCtrl::add_dynamic_attributes) ENABLED START -----*/
	//	Add your own code to create and add dynamic attributes if any
	/*----- PROTECTED REGION END -----*/	//	RadCtrl::add_dynamic_attributes
}

//--------------------------------------------------------
/**
 *	Method      : RadCtrl::add_dynamic_commands()
 *	Description : Create the dynamic commands if any
 *                for specified device.
 */
//--------------------------------------------------------
void RadCtrl::add_dynamic_commands()
{
	/*----- PROTECTED REGION ID(RadCtrl::add_dynamic_commands) ENABLED START -----*/
	//	Add your own code to create and add dynamic commands if any
	/*----- PROTECTED REGION END -----*/	//	RadCtrl::add_dynamic_commands
}

/*----- PROTECTED REGION ID(RadCtrl::namespace_ending) ENABLED START -----*/
void RadCtrl::determine_state(){
	DEBUG_STREAM<<"determine_state:entering"<<endl;
  switch(data->state){
    case DetectorState::OK:{
      set_state(Tango::ON);
      set_status("Device is OK");
      return;
    }
    case DetectorState::DISABLED:{
      set_state(Tango::OFF);
      set_status("Device is disabled");
      return;
    }
    case DetectorState::INIT:{
      set_state(Tango::INIT);
			set_status("Waiting for the detector to finish first measurement");
      return;
    }
    case DetectorState::NO_CONNECTION:{
      set_state(Tango::FAULT);
      set_status("Device server has lost connection with the detector. Make sure the detector is powered and connected to the controlling computer.");
      return;
    }
    case DetectorState::NO_CALIBRATION:{
      set_state(Tango::FAULT);
			set_status("Server failed to load calibration data. Make sure file \"calibration/$slave_id.txt\" exists in device server's working directory");
      return;
    }
    default:{
      set_state(Tango::UNKNOWN);
			set_status("Device server is in an unexpected state");
      return;
    }
  }
}///*----- PROTECTED REGION END -----*/	//	RadCtrl::namespace_ending
	//	RadCtrl::namespace_ending
} //	namespace
