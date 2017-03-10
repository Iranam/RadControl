/*----- PROTECTED REGION ID(RadCtrl.h) ENABLED START -----*/
#ifndef RadCtrl_H
#define RadCtrl_H

#include<tango.h>
#include"../DetectorData.h"
typedef unsigned int uint;
typedef unsigned char uchar;
/*----- PROTECTED REGION END -----*/	//	RadCtrl.h

/**
 *  RadCtrl class description:
 *    Controls gamma or neutron detectors
 */

namespace RadCtrl_ns
{
/*----- PROTECTED REGION ID(RadCtrl::Additional Class Declarations) ENABLED START -----*/
//	Additional Class Declarations
/*----- PROTECTED REGION END -----*/	//	RadCtrl::Additional Class Declarations

class RadCtrl : public TANGO_BASE_CLASS
{

/*----- PROTECTED REGION ID(RadCtrl::Data Members) ENABLED START -----*/
public:
  detector::DetectorData*data;//shared memory
/*----- PROTECTED REGION END -----*/	//	RadCtrl::Data Members


//	Attribute data members
public:
	Tango::DevULong	*attr_count_read;
	Tango::DevFloat	*attr_doserate_read;
	Tango::DevUShort	*attr_exposure_read;
	Tango::DevUChar	*attr_modbus_id_read;

//	Constructors and destructors
public:
	/**
	 * Constructs a newly device object.
	 *
	 *	@param cl	Class.
	 *	@param s 	Device Name
	 */
	RadCtrl(Tango::DeviceClass *cl,string &s);
	/**
	 * Constructs a newly device object.
	 *
	 *	@param cl	Class.
	 *	@param s 	Device Name
	 */
	RadCtrl(Tango::DeviceClass *cl,const char *s);
	/**
	 * Constructs a newly device object.
	 *
	 *	@param cl	Class.
	 *	@param s 	Device name
	 *	@param d	Device description.
	 */
	RadCtrl(Tango::DeviceClass *cl,const char *s,const char *d);
	/**
	 * The device object destructor.
	 */
	~RadCtrl() {delete_device();};


//	Miscellaneous methods
public:
	/*
	 *	will be called at device destruction or at init command.
	 */
	void delete_device();
	/*
	 *	Initialize the device
	 */
	virtual void init_device();
	/*
	 *	Always executed method before execution command method.
	 */
	virtual void always_executed_hook();


//	Attribute methods
public:
	//--------------------------------------------------------
	/*
	 *	Method      : RadCtrl::read_attr_hardware()
	 *	Description : Hardware acquisition for attributes.
	 */
	//--------------------------------------------------------
	virtual void read_attr_hardware(vector<long> &attr_list);
	//--------------------------------------------------------
	/*
	 *	Method      : RadCtrl::write_attr_hardware()
	 *	Description : Hardware writing for attributes.
	 */
	//--------------------------------------------------------
	virtual void write_attr_hardware(vector<long> &attr_list);

/**
 *	Attribute count related methods
 *	Description: Count during last exposition period
 *
 *	Data type:	Tango::DevULong
 *	Attr type:	Scalar
 */
	virtual void read_count(Tango::Attribute &attr);
	virtual bool is_count_allowed(Tango::AttReqType type);
/**
 *	Attribute doserate related methods
 *	Description: 
 *
 *	Data type:	Tango::DevFloat
 *	Attr type:	Scalar
 */
	virtual void read_doserate(Tango::Attribute &attr);
	virtual bool is_doserate_allowed(Tango::AttReqType type);
/**
 *	Attribute exposure related methods
 *	Description: 
 *
 *	Data type:	Tango::DevUShort
 *	Attr type:	Scalar
 */
	virtual void read_exposure(Tango::Attribute &attr);
	virtual void write_exposure(Tango::WAttribute &attr);
	virtual bool is_exposure_allowed(Tango::AttReqType type);
/**
 *	Attribute modbus_id related methods
 *	Description: 
 *
 *	Data type:	Tango::DevUChar
 *	Attr type:	Scalar
 */
	virtual void read_modbus_id(Tango::Attribute &attr);
	virtual bool is_modbus_id_allowed(Tango::AttReqType type);


	//--------------------------------------------------------
	/**
	 *	Method      : RadCtrl::add_dynamic_attributes()
	 *	Description : Add dynamic attributes if any.
	 */
	//--------------------------------------------------------
	void add_dynamic_attributes();




//	Command related methods
public:


	//--------------------------------------------------------
	/**
	 *	Method      : RadCtrl::add_dynamic_commands()
	 *	Description : Add dynamic commands if any.
	 */
	//--------------------------------------------------------
	void add_dynamic_commands();

/*----- PROTECTED REGION ID(RadCtrl::Additional Method prototypes) ENABLED START -----*/
//TODO
int read_register(Tango::Attribute&attr,uint16_t*dest,uint reg,uchar nwords=1);
void init(Tango::DeviceClass*cl);
bool assure_calibrated();
bool assure_configured();
bool assure_data_ready();
void determine_state();
/*----- PROTECTED REGION END -----*/	//	RadCtrl::Additional Method prototypes
};

/*----- PROTECTED REGION ID(RadCtrl::Additional Classes Definitions) ENABLED START -----*/
//	Additional Classes Definitions
/*----- PROTECTED REGION END -----*/	//	RadCtrl::Additional Classes Definitions

}	//	End of namespace

#endif   //	RadCtrl_H
