/*----- PROTECTED REGION ID(RadCtrlClass.cpp) ENABLED START -----*/
static const char *RcsId      = "$Id:  $";
static const char *TagName    = "$Name:  $";
static const char *CvsPath    = "$Source:  $";
static const char *SvnPath    = "$HeadURL:  $";
static const char *HttpServer = "http://www.esrf.eu/computing/cs/tango/tango_doc/ds_doc/";
#include<RadCtrlClass.h>
#include<RadCtrl.h>
using namespace detector;
using namespace std;
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/mman.h>
#include<unistd.h>
#include<fcntl.h>
/*----- PROTECTED REGION END -----*/	//	RadCtrlClass.cpp

//-------------------------------------------------------------------
/**
 *	Create RadCtrlClass singleton and
 *	return it in a C function for Python usage
 */
//-------------------------------------------------------------------
extern "C" {
#ifdef _TG_WINDOWS_

__declspec(dllexport)

#endif

	Tango::DeviceClass *_create_RadCtrl_class(const char *name) {
		return RadCtrl_ns::RadCtrlClass::init(name);
	}
}

namespace RadCtrl_ns
{
//===================================================================
//	Initialize pointer for singleton pattern
//===================================================================
RadCtrlClass *RadCtrlClass::_instance = NULL;

//--------------------------------------------------------
/**
 * method : 		RadCtrlClass::RadCtrlClass(string &s)
 * description : 	constructor for the RadCtrlClass
 *
 * @param s	The class name
 */
//--------------------------------------------------------
RadCtrlClass::RadCtrlClass(string &s):Tango::DeviceClass(s)
{
	cout2 << "Entering RadCtrlClass constructor" << endl;
	set_default_property();
	write_class_property();

	/*----- PROTECTED REGION ID(RadCtrlClass::constructor) ENABLED START -----*/
  int fd=open("/tmp/radcontrol",O_RDONLY);
  if(fd==-1){
    cerr<<"Failed to open /tmp/radcontrol"<<endl;
    exit(-1);
  }
  int ret=lseek(fd,NUMBER_OFFSET,SEEK_SET);
  ret=read(fd,&NDETECTORS,1);//read number
  if(ret<1){
    cerr<<"Failed to read number of detectors"<<endl;
    exit(-1);
  }
  shmem=(char*)mmap(NULL,DATA_OFFSET+NDETECTORS*sizeof(DetectorData),PROT_READ,MAP_SHARED,fd,0);
  if(shmem==MAP_FAILED)throw "MMAP_FAILED";
  data=(DetectorData*)(shmem+DATA_OFFSET);
  close(fd);
	/*----- PROTECTED REGION END -----*/	//	RadCtrlClass::constructor

	cout2 << "Leaving RadCtrlClass constructor" << endl;
}

//--------------------------------------------------------
/**
 * method : 		RadCtrlClass::~RadCtrlClass()
 * description : 	destructor for the RadCtrlClass
 */
//--------------------------------------------------------
RadCtrlClass::~RadCtrlClass()
{
	/*----- PROTECTED REGION ID(RadCtrlClass::destructor) ENABLED START -----*/
  munmap(shmem,DATA_OFFSET+NDETECTORS*sizeof(DetectorData));
	/*----- PROTECTED REGION END -----*/	//	RadCtrlClass::destructor

	_instance = NULL;
}


//--------------------------------------------------------
/**
 * method : 		RadCtrlClass::init
 * description : 	Create the object if not already done.
 *                  Otherwise, just return a pointer to the object
 *
 * @param	name	The class name
 */
//--------------------------------------------------------
RadCtrlClass *RadCtrlClass::init(const char *name)
{
	if (_instance == NULL)
	{
		try
		{
			string s(name);
			_instance = new RadCtrlClass(s);
		}
		catch (bad_alloc &)
		{
			throw;
		}
	}
	return _instance;
}

//--------------------------------------------------------
/**
 * method : 		RadCtrlClass::instance
 * description : 	Check if object already created,
 *                  and return a pointer to the object
 */
//--------------------------------------------------------
RadCtrlClass *RadCtrlClass::instance()
{
	if (_instance == NULL)
	{
		cerr << "Class is not initialised !!" << endl;
		exit(-1);
	}
	return _instance;
}



//===================================================================
//	Command execution method calls
//===================================================================

//===================================================================
//	Properties management
//===================================================================
//--------------------------------------------------------
/**
 *	Method      : RadCtrlClass::get_class_property()
 *	Description : Get the class property for specified name.
 */
//--------------------------------------------------------
Tango::DbDatum RadCtrlClass::get_class_property(string &prop_name)
{
	for (unsigned int i=0 ; i<cl_prop.size() ; i++)
		if (cl_prop[i].name == prop_name)
			return cl_prop[i];
	//	if not found, returns  an empty DbDatum
	return Tango::DbDatum(prop_name);
}

//--------------------------------------------------------
/**
 *	Method      : RadCtrlClass::get_default_device_property()
 *	Description : Return the default value for device property.
 */
//--------------------------------------------------------
Tango::DbDatum RadCtrlClass::get_default_device_property(string &prop_name)
{
	for (unsigned int i=0 ; i<dev_def_prop.size() ; i++)
		if (dev_def_prop[i].name == prop_name)
			return dev_def_prop[i];
	//	if not found, return  an empty DbDatum
	return Tango::DbDatum(prop_name);
}

//--------------------------------------------------------
/**
 *	Method      : RadCtrlClass::get_default_class_property()
 *	Description : Return the default value for class property.
 */
//--------------------------------------------------------
Tango::DbDatum RadCtrlClass::get_default_class_property(string &prop_name)
{
	for (unsigned int i=0 ; i<cl_def_prop.size() ; i++)
		if (cl_def_prop[i].name == prop_name)
			return cl_def_prop[i];
	//	if not found, return  an empty DbDatum
	return Tango::DbDatum(prop_name);
}


//--------------------------------------------------------
/**
 *	Method      : RadCtrlClass::set_default_property()
 *	Description : Set default property (class and device) for wizard.
 *                For each property, add to wizard property name and description.
 *                If default value has been set, add it to wizard property and
 *                store it in a DbDatum.
 */
//--------------------------------------------------------
void RadCtrlClass::set_default_property()
{
	string	prop_name;
	string	prop_desc;
	string	prop_def;
	vector<string>	vect_data;

	//	Set Default Class Properties

	//	Set Default device Properties
}

//--------------------------------------------------------
/**
 *	Method      : RadCtrlClass::write_class_property()
 *	Description : Set class description fields as property in database
 */
//--------------------------------------------------------
void RadCtrlClass::write_class_property()
{
	//	First time, check if database used
	if (Tango::Util::_UseDb == false)
		return;

	Tango::DbData	data;
	string	classname = get_name();
	string	header;
	string::size_type	start, end;

	//	Put title
	Tango::DbDatum	title("ProjectTitle");
	string	str_title("RadControl");
	title << str_title;
	data.push_back(title);

	//	Put Description
	Tango::DbDatum	description("Description");
	vector<string>	str_desc;
	str_desc.push_back("Controls gamma or neutron detectors");
	description << str_desc;
	data.push_back(description);

	//	put cvs or svn location
	string	filename("RadCtrl");
	filename += "Class.cpp";

	// check for cvs information
	string	src_path(CvsPath);
	start = src_path.find("/");
	if (start!=string::npos)
	{
		end   = src_path.find(filename);
		if (end>start)
		{
			string	strloc = src_path.substr(start, end-start);
			//	Check if specific repository
			start = strloc.find("/cvsroot/");
			if (start!=string::npos && start>0)
			{
				string	repository = strloc.substr(0, start);
				if (repository.find("/segfs/")!=string::npos)
					strloc = "ESRF:" + strloc.substr(start, strloc.length()-start);
			}
			Tango::DbDatum	cvs_loc("cvs_location");
			cvs_loc << strloc;
			data.push_back(cvs_loc);
		}
	}

	// check for svn information
	else
	{
		string	src_path(SvnPath);
		start = src_path.find("://");
		if (start!=string::npos)
		{
			end = src_path.find(filename);
			if (end>start)
			{
				header = "$HeadURL: ";
				start = header.length();
				string	strloc = src_path.substr(start, (end-start));
				
				Tango::DbDatum	svn_loc("svn_location");
				svn_loc << strloc;
				data.push_back(svn_loc);
			}
		}
	}

	//	Get CVS or SVN revision tag
	
	// CVS tag
	string	tagname(TagName);
	header = "$Name: ";
	start = header.length();
	string	endstr(" $");
	
	end   = tagname.find(endstr);
	if (end!=string::npos && end>start)
	{
		string	strtag = tagname.substr(start, end-start);
		Tango::DbDatum	cvs_tag("cvs_tag");
		cvs_tag << strtag;
		data.push_back(cvs_tag);
	}
	
	// SVN tag
	string	svnpath(SvnPath);
	header = "$HeadURL: ";
	start = header.length();
	
	end   = svnpath.find(endstr);
	if (end!=string::npos && end>start)
	{
		string	strloc = svnpath.substr(start, end-start);
		
		string tagstr ("/tags/");
		start = strloc.find(tagstr);
		if ( start!=string::npos )
		{
			start = start + tagstr.length();
			end   = strloc.find(filename);
			string	strtag = strloc.substr(start, end-start-1);
			
			Tango::DbDatum	svn_tag("svn_tag");
			svn_tag << strtag;
			data.push_back(svn_tag);
		}
	}

	//	Get URL location
	string	httpServ(HttpServer);
	if (httpServ.length()>0)
	{
		Tango::DbDatum	db_doc_url("doc_url");
		db_doc_url << httpServ;
		data.push_back(db_doc_url);
	}

	//  Put inheritance
	Tango::DbDatum	inher_datum("InheritedFrom");
	vector<string> inheritance;
	inheritance.push_back("TANGO_BASE_CLASS");
	inher_datum << inheritance;
	data.push_back(inher_datum);

	//	Call database and and values
	get_db_class()->put_property(data);
}

//===================================================================
//	Factory methods
//===================================================================

//--------------------------------------------------------
/**
 *	Method      : RadCtrlClass::device_factory()
 *	Description : Create the device object(s)
 *                and store them in the device list
 */
//--------------------------------------------------------
void RadCtrlClass::device_factory(const Tango::DevVarStringArray *devlist_ptr)
{
	/*----- PROTECTED REGION ID(RadCtrlClass::device_factory_before) ENABLED START -----*/
	
	//	Add your own code
	
	/*----- PROTECTED REGION END -----*/	//	RadCtrlClass::device_factory_before

	//	Create devices and add it into the device list
	for (unsigned long i=0 ; i<devlist_ptr->length() ; i++)
	{
		cout4 << "Device name : " << (*devlist_ptr)[i].in() << endl;
		device_list.push_back(new RadCtrl(this, (*devlist_ptr)[i]));
	}

	//	Manage dynamic attributes if any
	erase_dynamic_attributes(devlist_ptr, get_class_attr()->get_attr_list());

	//	Export devices to the outside world
	for (unsigned long i=1 ; i<=devlist_ptr->length() ; i++)
	{
		//	Add dynamic attributes if any
		RadCtrl *dev = static_cast<RadCtrl *>(device_list[device_list.size()-i]);
		dev->add_dynamic_attributes();

		//	Check before if database used.
		if ((Tango::Util::_UseDb == true) && (Tango::Util::_FileDb == false))
			export_device(dev);
		else
			export_device(dev, dev->get_name().c_str());
	}

	/*----- PROTECTED REGION ID(RadCtrlClass::device_factory_after) ENABLED START -----*/
  //assign parts of shared memory to devices
  uint8_t N=NDETECTORS;
  uint8_t DN=devlist_ptr->length();
  if(DN<N)N=DN;
  for(uint i=0;i<N;++i){
    RadCtrl*dev=(RadCtrl*)device_list[i];
    dev->data=data+i;
    dev->determine_state();
  }
	/*----- PROTECTED REGION END -----*/	//	RadCtrlClass::device_factory_after
}
//--------------------------------------------------------
/**
 *	Method      : RadCtrlClass::attribute_factory()
 *	Description : Create the attribute object(s)
 *                and store them in the attribute list
 */
//--------------------------------------------------------
void RadCtrlClass::attribute_factory(vector<Tango::Attr *> &att_list)
{
	/*----- PROTECTED REGION ID(RadCtrlClass::attribute_factory_before) ENABLED START -----*/
	
	//	Add your own code
	
	/*----- PROTECTED REGION END -----*/	//	RadCtrlClass::attribute_factory_before
	//	Attribute : count
	countAttrib	*count = new countAttrib();
	Tango::UserDefaultAttrProp	count_prop;
	count_prop.set_description("Count during last exposition period");
	count_prop.set_label("Count");
	//	unit	not set for count
	//	standard_unit	not set for count
	//	display_unit	not set for count
	//	format	not set for count
	//	max_value	not set for count
	//	min_value	not set for count
	//	max_alarm	not set for count
	//	min_alarm	not set for count
	//	max_warning	not set for count
	//	min_warning	not set for count
	//	delta_t	not set for count
	//	delta_val	not set for count
	
	count->set_default_properties(count_prop);
	//	Not Polled
	count->set_disp_level(Tango::OPERATOR);
	//	Not Memorized
	att_list.push_back(count);

	//	Attribute : doserate
	doserateAttrib	*doserate = new doserateAttrib();
	Tango::UserDefaultAttrProp	doserate_prop;
	//	description	not set for doserate
	doserate_prop.set_label("dose rate");
	doserate_prop.set_unit("uSv/h");
	doserate_prop.set_standard_unit("uSv/h");
	doserate_prop.set_display_unit("uSv/h");
	doserate_prop.set_format("%f.3");
	//	max_value	not set for doserate
	doserate_prop.set_min_value("0");
	doserate_prop.set_max_alarm("0.3");
	//	min_alarm	not set for doserate
	doserate_prop.set_max_warning("0.2");
	//	min_warning	not set for doserate
	//	delta_t	not set for doserate
	//	delta_val	not set for doserate
	
	doserate->set_default_properties(doserate_prop);
	//	Not Polled
	doserate->set_disp_level(Tango::OPERATOR);
	//	Not Memorized
	att_list.push_back(doserate);

	//	Attribute : exposure
	exposureAttrib	*exposure = new exposureAttrib();
	Tango::UserDefaultAttrProp	exposure_prop;
	//	description	not set for exposure
	exposure_prop.set_label("Exposition");
	exposure_prop.set_unit("milliseconds");
	exposure_prop.set_standard_unit("ms");
	exposure_prop.set_display_unit("ms");
	//	format	not set for exposure
	//	max_value	not set for exposure
	//	min_value	not set for exposure
	//	max_alarm	not set for exposure
	//	min_alarm	not set for exposure
	//	max_warning	not set for exposure
	//	min_warning	not set for exposure
	//	delta_t	not set for exposure
	//	delta_val	not set for exposure
	
	exposure->set_default_properties(exposure_prop);
	//	Not Polled
	exposure->set_disp_level(Tango::OPERATOR);
	exposure->set_memorized();
	exposure->set_memorized_init(true);
	att_list.push_back(exposure);

	//	Attribute : modbus_id
	modbus_idAttrib	*modbus_id = new modbus_idAttrib();
	Tango::UserDefaultAttrProp	modbus_id_prop;
	//	description	not set for modbus_id
	//	label	not set for modbus_id
	//	unit	not set for modbus_id
	//	standard_unit	not set for modbus_id
	//	display_unit	not set for modbus_id
	//	format	not set for modbus_id
	//	max_value	not set for modbus_id
	//	min_value	not set for modbus_id
	//	max_alarm	not set for modbus_id
	//	min_alarm	not set for modbus_id
	//	max_warning	not set for modbus_id
	//	min_warning	not set for modbus_id
	//	delta_t	not set for modbus_id
	//	delta_val	not set for modbus_id
	
	modbus_id->set_default_properties(modbus_id_prop);
	//	Not Polled
	modbus_id->set_disp_level(Tango::OPERATOR);
	//	Not Memorized
	att_list.push_back(modbus_id);


	//	Create a list of static attributes
	create_static_attribute_list(get_class_attr()->get_attr_list());
	/*----- PROTECTED REGION ID(RadCtrlClass::attribute_factory_after) ENABLED START -----*/
	
	//	Add your own code
	
	/*----- PROTECTED REGION END -----*/	//	RadCtrlClass::attribute_factory_after
}
//--------------------------------------------------------
/**
 *	Method      : RadCtrlClass::pipe_factory()
 *	Description : Create the pipe object(s)
 *                and store them in the pipe list
 */
//--------------------------------------------------------
void RadCtrlClass::pipe_factory()
{
	/*----- PROTECTED REGION ID(RadCtrlClass::pipe_factory_before) ENABLED START -----*/
	
	//	Add your own code
	
	/*----- PROTECTED REGION END -----*/	//	RadCtrlClass::pipe_factory_before
	/*----- PROTECTED REGION ID(RadCtrlClass::pipe_factory_after) ENABLED START -----*/
	
	//	Add your own code
	
	/*----- PROTECTED REGION END -----*/	//	RadCtrlClass::pipe_factory_after
}
//--------------------------------------------------------
/**
 *	Method      : RadCtrlClass::command_factory()
 *	Description : Create the command object(s)
 *                and store them in the command list
 */
//--------------------------------------------------------
void RadCtrlClass::command_factory()
{
	/*----- PROTECTED REGION ID(RadCtrlClass::command_factory_before) ENABLED START -----*/
	
	//	Add your own code
	
	/*----- PROTECTED REGION END -----*/	//	RadCtrlClass::command_factory_before


	/*----- PROTECTED REGION ID(RadCtrlClass::command_factory_after) ENABLED START -----*/
	
	//	Add your own code
	
	/*----- PROTECTED REGION END -----*/	//	RadCtrlClass::command_factory_after
}

//===================================================================
//	Dynamic attributes related methods
//===================================================================

//--------------------------------------------------------
/**
 * method : 		RadCtrlClass::create_static_attribute_list
 * description : 	Create the a list of static attributes
 *
 * @param	att_list	the ceated attribute list
 */
//--------------------------------------------------------
void RadCtrlClass::create_static_attribute_list(vector<Tango::Attr *> &att_list)
{
	for (unsigned long i=0 ; i<att_list.size() ; i++)
	{
		string att_name(att_list[i]->get_name());
		transform(att_name.begin(), att_name.end(), att_name.begin(), ::tolower);
		defaultAttList.push_back(att_name);
	}

	cout2 << defaultAttList.size() << " attributes in default list" << endl;

	/*----- PROTECTED REGION ID(RadCtrlClass::create_static_att_list) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	RadCtrlClass::create_static_att_list
}


//--------------------------------------------------------
/**
 * method : 		RadCtrlClass::erase_dynamic_attributes
 * description : 	delete the dynamic attributes if any.
 *
 * @param	devlist_ptr	the device list pointer
 * @param	list of all attributes
 */
//--------------------------------------------------------
void RadCtrlClass::erase_dynamic_attributes(const Tango::DevVarStringArray *devlist_ptr, vector<Tango::Attr *> &att_list)
{
	Tango::Util *tg = Tango::Util::instance();

	for (unsigned long i=0 ; i<devlist_ptr->length() ; i++)
	{
		Tango::DeviceImpl *dev_impl = tg->get_device_by_name(((string)(*devlist_ptr)[i]).c_str());
		RadCtrl *dev = static_cast<RadCtrl *> (dev_impl);

		vector<Tango::Attribute *> &dev_att_list = dev->get_device_attr()->get_attribute_list();
		vector<Tango::Attribute *>::iterator ite_att;
		for (ite_att=dev_att_list.begin() ; ite_att != dev_att_list.end() ; ++ite_att)
		{
			string att_name((*ite_att)->get_name_lower());
			if ((att_name == "state") || (att_name == "status"))
				continue;
			vector<string>::iterator ite_str = find(defaultAttList.begin(), defaultAttList.end(), att_name);
			if (ite_str == defaultAttList.end())
			{
				cout2 << att_name << " is a UNWANTED dynamic attribute for device " << (*devlist_ptr)[i] << endl;
				Tango::Attribute &att = dev->get_device_attr()->get_attr_by_name(att_name.c_str());
				dev->remove_attribute(att_list[att.get_attr_idx()], true, false);
				--ite_att;
			}
		}
	}
	/*----- PROTECTED REGION ID(RadCtrlClass::erase_dynamic_attributes) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	RadCtrlClass::erase_dynamic_attributes
}

//--------------------------------------------------------
/**
 *	Method      : RadCtrlClass::get_attr_by_name()
 *	Description : returns Tango::Attr * object found by name
 */
//--------------------------------------------------------
Tango::Attr *RadCtrlClass::get_attr_object_by_name(vector<Tango::Attr *> &att_list, string attname)
{
	vector<Tango::Attr *>::iterator it;
	for (it=att_list.begin() ; it<att_list.end() ; ++it)
		if ((*it)->get_name()==attname)
			return (*it);
	//	Attr does not exist
	return NULL;
}


/*----- PROTECTED REGION ID(RadCtrlClass::Additional Methods) ENABLED START -----*/

/*----- PROTECTED REGION END -----*/	//	RadCtrlClass::Additional Methods
} //	namespace
