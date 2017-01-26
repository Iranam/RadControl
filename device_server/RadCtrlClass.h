/*----- PROTECTED REGION ID(RadCtrlClass.h) ENABLED START -----*/
#ifndef RadCtrlClass_H
#define RadCtrlClass_H

#include<tango.h>
#include<RadCtrl.h>
#include<mqueue.h>
/*----- PROTECTED REGION END -----*/	//	RadCtrlClass.h


namespace RadCtrl_ns
{
/*----- PROTECTED REGION ID(RadCtrlClass::classes for dynamic creation) ENABLED START -----*/
/*----- PROTECTED REGION END -----*/	//	RadCtrlClass::classes for dynamic creation

//=========================================
//	Define classes for attributes
//=========================================
//	Attribute count class definition
class countAttrib: public Tango::Attr
{
public:
	countAttrib():Attr("count",
			Tango::DEV_ULONG, Tango::READ) {};
	~countAttrib() {};
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
		{(static_cast<RadCtrl *>(dev))->read_count(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
		{return (static_cast<RadCtrl *>(dev))->is_count_allowed(ty);}
};

//	Attribute background class definition
class backgroundAttrib: public Tango::Attr
{
public:
	backgroundAttrib():Attr("background",
			Tango::DEV_FLOAT, Tango::READ) {};
	~backgroundAttrib() {};
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
		{(static_cast<RadCtrl *>(dev))->read_background(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
		{return (static_cast<RadCtrl *>(dev))->is_background_allowed(ty);}
};

//	Attribute exposure class definition
class exposureAttrib: public Tango::Attr
{
public:
	exposureAttrib():Attr("exposure",
			Tango::DEV_USHORT, Tango::READ) {};
	~exposureAttrib() {};
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
		{(static_cast<RadCtrl *>(dev))->read_exposure(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
		{return (static_cast<RadCtrl *>(dev))->is_exposure_allowed(ty);}
};

//	Attribute modbus_id class definition
class modbus_idAttrib: public Tango::Attr
{
public:
	modbus_idAttrib():Attr("modbus_id",
			Tango::DEV_UCHAR, Tango::READ) {};
	~modbus_idAttrib() {};
	virtual void read(Tango::DeviceImpl *dev,Tango::Attribute &att)
		{(static_cast<RadCtrl *>(dev))->read_modbus_id(att);}
	virtual bool is_allowed(Tango::DeviceImpl *dev,Tango::AttReqType ty)
		{return (static_cast<RadCtrl *>(dev))->is_modbus_id_allowed(ty);}
};


//=========================================
//	Define classes for commands
//=========================================
//	Command setExposure class definition
class setExposureClass : public Tango::Command
{
public:
	setExposureClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out,
				   const char        *in_desc,
				   const char        *out_desc,
				   Tango::DispLevel  level)
	:Command(name,in,out,in_desc,out_desc, level)	{};

	setExposureClass(const char   *name,
	               Tango::CmdArgType in,
				   Tango::CmdArgType out)
	:Command(name,in,out)	{};
	~setExposureClass() {};
	
	virtual CORBA::Any *execute (Tango::DeviceImpl *dev, const CORBA::Any &any);
	virtual bool is_allowed (Tango::DeviceImpl *dev, const CORBA::Any &any)
	{return (static_cast<RadCtrl *>(dev))->is_setExposure_allowed(any);}
};


/**
 *	The RadCtrlClass singleton definition
 */

#ifdef _TG_WINDOWS_
class __declspec(dllexport)  RadCtrlClass : public Tango::DeviceClass
#else
class RadCtrlClass : public Tango::DeviceClass
#endif
{
	/*----- PROTECTED REGION ID(RadCtrlClass::Additionnal DServer data members) ENABLED START -----*/
	public:
    uint8_t NDETECTORS;
	  char*shmem;
    mqd_t message_queue;
    detector::DetectorData*data;
	/*----- PROTECTED REGION END -----*/	//	RadCtrlClass::Additionnal DServer data members

	public:
		//	write class properties data members
		Tango::DbData	cl_prop;
		Tango::DbData	cl_def_prop;
		Tango::DbData	dev_def_prop;
	
		//	Method prototypes
		static RadCtrlClass *init(const char *);
		static RadCtrlClass *instance();
		~RadCtrlClass();
		Tango::DbDatum	get_class_property(string &);
		Tango::DbDatum	get_default_device_property(string &);
		Tango::DbDatum	get_default_class_property(string &);
	
	protected:
		RadCtrlClass(string &);
		static RadCtrlClass *_instance;
		void command_factory();
		void attribute_factory(vector<Tango::Attr *> &);
		void pipe_factory();
		void write_class_property();
		void set_default_property();
		void get_class_property();
		string get_cvstag();
		string get_cvsroot();
	
	private:
		void device_factory(const Tango::DevVarStringArray *);
		void create_static_attribute_list(vector<Tango::Attr *> &);
		void erase_dynamic_attributes(const Tango::DevVarStringArray *,vector<Tango::Attr *> &);
		vector<string>	defaultAttList;
		Tango::Attr *get_attr_object_by_name(vector<Tango::Attr *> &att_list, string attname);
};

}	//	End of namespace

#endif   //	RadCtrl_H
