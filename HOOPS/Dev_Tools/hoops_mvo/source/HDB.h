// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef _HDB_H
#define _HDB_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"


class HImManager;
class HIOManager;
class HTManager;
class HMaterialLibrary;


//! The HDB class serves as a wrapper/utility class to manage an instance of the HOOPS/3dGS scene-graph
/*!
	HDB Stores the information of how many and which type of HOOPS driver instances the associated object 
	HBaseView creates.  
*/
class MVO_API HDB {

	
public:
	/*Constructs an HDB object.*/
	HDB ();
	virtual ~HDB ();

	/*!
		Init() initializes the HOOPS attributes for driver type and unsets the default HOOPS alias--"?Picture"--for the
		root segment of the given driver.  The function also sets up a default color map and registers the HOOPS I.M.
		callbacks that are listed in HDB::RegisterHoopsIMCallbacks().
	*/
	virtual void Init ();

	static HDB * GetHDB();  //!< \return A pointer to the HDB object.

	/*! This method augments the number of instances of a driver by one.*/
	static void IncrementInstanceCount(); 

	/*! This method decrements the number of instances of a driver by one.*/
	static void DecrementInstanceCount();

	/*! This method sets the base name for driver instances.*/
	static void SetInstanceBase(const char* s);

	/*! \return The base name for driver instances.*/
	static const char* GetInstanceBase();

	/*! This method sets the base driver type for the driver instances.*/
	static void SetDriverType(const char* s);

	/*! \return The base driver type for the driver instances.*/
	static const char* GetDriverType();

	/*! \return True if global includes should be moved to model specific includes during reading. */
	static bool GetCleanInclude() { return m_bCleanInclude; }

	/*! This method determines whether global includes should be moved to model specific includes during reading. 
	\param value Pass true to have global includes move to model specific includes during reading or false to keep the includes.*/
	static void SetCleanInclude(bool value) { m_bCleanInclude = value; }

	/*!	\return An integer denoting the number of instances of a driver.	*/
	static int GetInstanceCount() {return m_InstanceCount; }

	/*!	\return An integer that will be unique during the runtime of the application.	*/
	static int GetUniqueID() {return m_nextUID++; }

	/*!	This method sets the number of instances of a driver.	
	\param count The number of driver instances.
	*/
	static void SetInstanceCount(int count) {m_InstanceCount = count;}

	/*! \return A pointer to the current HImManager object (the HOOPS/3dGS I.M. manager).  This method creates one if none exists. */
	static HImManager * GetHImManager();

	/*! This method replaces the HOOPS/3dGS I.M. manager.
		\param manager  A pointer to an HImManager object.
		\return A pointer to the replaced HImManager.  
	*/
	static HImManager * SetHImManager(HImManager * manager);


	/*! \returns A pointer to the current HIOManager object (the input/output manager).  This method creates one if none exists.*/
	static HIOManager * GetHIOManager();

	/*! This method replaces the input/output manager.
		\param manager  A pointer to an HIOManager object.
		\return A pointer to the replaced HIOManager.  
	*/
	static HIOManager * SetHIOManager(HIOManager * manager);

	/*! \returns A pointer to the current HMaterialLibrary object (which manages materials resources from disk).  Return value is NULL if none exists.*/
	static HMaterialLibrary * GetMaterialLibrary();

	/*! \returns Sets the pointer to the material library. */
	static void SetMaterialLibrary(HMaterialLibrary *library);

	/*! \returns A pointer to the current HTManager object (the timer manager).  This method creates one if none exists. */
	static HTManager * GetHTManager();

	/*! This method replaces the timer manager.
		\param manager  A pointer to an HTManager object.
		\return A pointer to the replaced HTManager.  
	*/
	static HTManager * SetHTManager(HTManager * manager);

	/*! \return A key to a unique Include Library segment. */
	static HC_KEY GetUniqueIncludeKey();

	/*! \return A globally unique identifier. */
	static unsigned long GetNextCookie() { return next_cookie++; };

	/*! This method generates a HOOPS error.  Using this method, allows the error to go through HOOPS so that 
	it will be intercepted by whatever error handlers the app might have registered, and will use the reporting 
	mechanism appropriate to the specific platform.
		\param message The text to be posted.
		\param severity If the value is 0, the message is informational.  If the value is 1, the message is a warning.  If the value is 2, then the message is an error.  If the value is 3 or greater, the error is fatal. 
		\param category The category that this error falls under as described in #HC_Define_Error_Handler.
		\param specific The specific error under the category as described in #HC_Define_Error_Handler.
		*/
	static void ReportError( const char *message, int severity = 1, int category = 0, int specific = 0 );

	/*! This method initializes the error manager and puts it in use. */
	static void EnableErrorManager();
	/*! This method cleans up the error manager and then disables it. */
	static void DisableErrorManager();

	/*!  */
	static void SetOwnsSystem(bool owns) {m_bOwnsSystem=owns;};
	/*!  */
	static bool GetOwnsSystem() {return m_bOwnsSystem;};
 	/*!  Sets the "isolated" driver option. Allows a driver to be spawned on a separate thread. Used for background printing or multiple views.*/
	static void SetIsolatedDrivers(bool isolated) {m_bIsolatedDrivers=isolated;};
	/*! \return True if isolated driver is set */
	static bool GetIsolatedDrivers() {return m_bIsolatedDrivers;};

	static void Lock();

	static void UnLock();
 
	static bool GetPublishExchangeLicenseSet() { return m_bPublishExchangeLicenseSet; }

	static void SetPublishExchangeLicenseSet(bool isSet);

	static bool GetPublishInitialized() { return m_bPublishInitialized; }

	static void SetPublishInitialized(bool initialized);

private:

	static HDB * m_pHDB;
	static void *	m_Mutex;

	static int		m_InstanceCount;	//!< The number of driver instances.
	static int		m_nextUID;	//!< A number that will be unique each time its accessed
	static char*	m_pInstanceBase;	//!< The base name for the driver instances.
	static char*	m_pDriverType;		//!< The base driver type for the driver instances.
	static bool		m_bCleanInclude;	//!< move global includes to model specific include segments
	static bool		m_bUsingErrorManager;	//!< are we using the error manager
	static bool		m_bOwnsSystem;	//!< do we own the system
	static bool		m_bIsolatedDrivers;

	static HImManager * m_pHImManager; 

	static HIOManager * m_pHIOManager; 

	static HMaterialLibrary * m_pHMaterialLibrary;

	static HTManager * m_pHTManager; 

	static unsigned long include_counter; 
	static unsigned long next_cookie;	//!< the next identifier to be used when GetNextCookie is called

	static bool m_bPublishExchangeLicenseSet;

	static bool m_bPublishInitialized;


};


#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif






