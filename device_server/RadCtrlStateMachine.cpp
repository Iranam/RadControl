/*----- PROTECTED REGION ID(RadCtrlStateMachine.cpp) ENABLED START -----*/
static const char *RcsId = "$Id:  $";
//=============================================================================
//
// file :        RadCtrlStateMachine.cpp
//
// description : State machine file for the RadCtrl class
//
// project :     RadControl
//
// $Author:  $
//
// $Revision:  $
// $Date:  $
//
// $HeadURL:  $
//
//=============================================================================
//                This file is generated by POGO
//        (Program Obviously used to Generate tango Object)
//=============================================================================

#include <RadCtrl.h>

/*----- PROTECTED REGION END -----*/	//	RadCtrl::RadCtrlStateMachine.cpp

//================================================================
//  States  |  Description
//================================================================
//  ON      |  Detector is operational
//  FAULT   |  Failed to read data from physical device.
//          |  Make sure the detector is powered and connected to controlling computer
//          |  Read one of attributes to update state
//  INIT    |  


namespace RadCtrl_ns
{
//=================================================
//		Attributes Allowed Methods
//=================================================

//--------------------------------------------------------
/**
 *	Method      : RadCtrl::is_count_allowed()
 *	Description : Execution allowed for count attribute
 */
//--------------------------------------------------------
bool RadCtrl::is_count_allowed(TANGO_UNUSED(Tango::AttReqType type))
{

	//	Not any excluded states for count attribute in read access.
	/*----- PROTECTED REGION ID(RadCtrl::countStateAllowed_READ) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	RadCtrl::countStateAllowed_READ
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : RadCtrl::is_background_allowed()
 *	Description : Execution allowed for background attribute
 */
//--------------------------------------------------------
bool RadCtrl::is_background_allowed(TANGO_UNUSED(Tango::AttReqType type))
{

	//	Not any excluded states for background attribute in read access.
	/*----- PROTECTED REGION ID(RadCtrl::backgroundStateAllowed_READ) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	RadCtrl::backgroundStateAllowed_READ
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : RadCtrl::is_exposure_allowed()
 *	Description : Execution allowed for exposure attribute
 */
//--------------------------------------------------------
bool RadCtrl::is_exposure_allowed(TANGO_UNUSED(Tango::AttReqType type))
{
	//	Not any excluded states for exposure attribute in Write access.
	/*----- PROTECTED REGION ID(RadCtrl::exposureStateAllowed_WRITE) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	RadCtrl::exposureStateAllowed_WRITE

	//	Not any excluded states for exposure attribute in read access.
	/*----- PROTECTED REGION ID(RadCtrl::exposureStateAllowed_READ) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	RadCtrl::exposureStateAllowed_READ
	return true;
}

//--------------------------------------------------------
/**
 *	Method      : RadCtrl::is_modbus_id_allowed()
 *	Description : Execution allowed for modbus_id attribute
 */
//--------------------------------------------------------
bool RadCtrl::is_modbus_id_allowed(TANGO_UNUSED(Tango::AttReqType type))
{

	//	Not any excluded states for modbus_id attribute in read access.
	/*----- PROTECTED REGION ID(RadCtrl::modbus_idStateAllowed_READ) ENABLED START -----*/
	
	/*----- PROTECTED REGION END -----*/	//	RadCtrl::modbus_idStateAllowed_READ
	return true;
}


//=================================================
//		Commands Allowed Methods
//=================================================


/*----- PROTECTED REGION ID(RadCtrl::RadCtrlStateAllowed.AdditionalMethods) ENABLED START -----*/

//	Additional Methods

/*----- PROTECTED REGION END -----*/	//	RadCtrl::RadCtrlStateAllowed.AdditionalMethods

}	//	End of namespace
