// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>



#include "HDB.h"

#include "HImManager.h"
#include "HIOManager.h"
#include "HTManager.h"
#include "HErrorManager.h"
#include "HMaterialLibrary.h"

#ifdef WINDOWS_SYSTEM
#	ifndef WIN32_LEAN_AND_MEAN
#		define WIN32_LEAN_AND_MEAN
#	endif
# include <windows.h>
#endif

#define HMUTEX_IMPLEMENT_MUTEX_FUNCTIONS

#include "hmutex.h"

#define UNSPECIFIED_ARGUMENTS ...
extern "C" { typedef void (HC_CDECL * Void_Routine) (UNSPECIFIED_ARGUMENTS); }

HDB * 	HDB::m_pHDB = 0;
HMutex	HDB::m_Mutex = 0;
int		HDB::m_InstanceCount = 0;
char*	HDB::m_pInstanceBase = 0; 
char*	HDB::m_pDriverType = 0;
bool	HDB::m_bCleanInclude = true;
bool	HDB::m_bUsingErrorManager = false;
bool	HDB::m_bOwnsSystem = true;
bool	HDB::m_bIsolatedDrivers = true;

unsigned long HDB::include_counter = 0;
unsigned long HDB::next_cookie = 0;

int HDB::m_nextUID = 1;

HImManager * HDB::m_pHImManager = 0;
HIOManager * HDB::m_pHIOManager = 0;
HTManager * HDB::m_pHTManager = 0;
HMaterialLibrary * HDB::m_pHMaterialLibrary = 0;

bool	HDB::m_bPublishExchangeLicenseSet = false;

bool	HDB::m_bPublishInitialized = false;

HDB::HDB () {

	if(m_pHDB) return;
	m_pHDB = this;

	CREATE_MUTEX(m_Mutex);
}		

HDB::~HDB () 
{

    if(m_InstanceCount == 0) {
		//reset to initial state
		m_pHDB = 0;
		H_SAFE_DELETE_ARRAY(m_pInstanceBase);
		H_SAFE_DELETE_ARRAY(m_pDriverType);
		m_bCleanInclude = true;
		H_SAFE_DELETE(m_pHImManager);
#if 1
		/* It seems like m_pHIOManager is the only real class which can only be created
		   once.  Not deleting this here allows HDB to be created and deleted multiple
		   time per application run.  Fixing HIOManager to be able to be created and
		   deleted would fix the problem, or we can just not delete it here. */
		H_SAFE_DELETE(m_pHIOManager);
#endif
		H_SAFE_DELETE(m_pHTManager);
		H_SAFE_DELETE(m_pHMaterialLibrary);

		if(m_bUsingErrorManager)
			HErrorManager::Cleanup();

		if(m_bOwnsSystem)
			HC_Reset_System();

 		DESTROY_MUTEX(m_Mutex);
		m_Mutex = 0;
    }
}

HDB * HDB::GetHDB()
{
    return m_pHDB;
}

void HDB::SetPublishExchangeLicenseSet(bool isSet) { m_bPublishExchangeLicenseSet = isSet; }

void HDB::SetPublishInitialized(bool initialized) { m_bPublishInitialized = initialized; }


void HDB::EnableErrorManager()
{
	m_bUsingErrorManager=true;
	HErrorManager::Init();
}
void HDB::DisableErrorManager()
{
	if(m_bUsingErrorManager){
		HErrorManager::Cleanup();
		m_bUsingErrorManager = false;
	}
}
 
void HDB::IncrementInstanceCount()
{
	m_InstanceCount++;
}

void HDB::DecrementInstanceCount()
{
	m_InstanceCount--;
}

const char* HDB::GetInstanceBase()
{
	return m_pInstanceBase;
}

const char* HDB::GetDriverType()
{
	return m_pDriverType;
}

void HDB::SetInstanceBase( const char* s)
{
	H_SAFE_DELETE_ARRAY(m_pInstanceBase);
	m_pInstanceBase = new char[strlen (s) + 1];
	strcpy(m_pInstanceBase, s);
}

void HDB::SetDriverType( const char* s )
{
	H_SAFE_DELETE_ARRAY(m_pDriverType);
	m_pDriverType = new char[strlen (s) + 1];
	strcpy(m_pDriverType, s);
}


void HDB::Init () {

	char tempstr[MVO_BUFFER_SIZE];

	GetHImManager();

	if (m_Mutex == 0)
		CREATE_MUTEX(m_Mutex);

	if(!m_pDriverType) {
		HC_Show_Pathname_Expansion ("?Picture", tempstr);
		m_pDriverType = new char[strlen (tempstr) + 1];
		HC_Parse_String (tempstr, "/", -2, m_pDriverType);

	    HC_Delete_Segment ("?Picture");
		HC_UnDefine_Alias ("?Picture");
	}

	if(!m_pInstanceBase) {
		m_pInstanceBase = new char[strlen (tempstr) + 1];
		HC_Parse_String (tempstr, "/", -1, m_pInstanceBase);
	}

	// setup a default global HOOPS colormap
	HC_QSet_Color_Map("/", "black, white, green, red, blue, cyan, magenta, green yellow, gray, light gray");

#ifndef _MSC_VER
#ifndef OSX_SYSTEM
	SetInstanceBase(getenv("DISPLAY") ? getenv("DISPLAY") : ":0.0");
#endif // NOT OSX_SYSTEM
#endif // NOT _MSC_VER
}


HImManager * HDB::GetHImManager()
{
	if(!m_pHImManager) {
		m_pHImManager = new HImManager();
		m_pHImManager->RegisterCallbacks();
	}

	return m_pHImManager;
}

HImManager * HDB::SetHImManager(HImManager * manager)
{
	HImManager * old_him_manager = m_pHImManager;
	m_pHImManager = manager;
	return old_him_manager;
}


HIOManager * HDB::GetHIOManager()
{
	if(!m_pHIOManager) {
		m_pHIOManager = new HIOManager();
		m_pHIOManager->RegisterHandlers();
	}

	return m_pHIOManager;
}

HIOManager * HDB::SetHIOManager(HIOManager * manager)
{
	HIOManager * old_hio_manager = m_pHIOManager;
	m_pHIOManager = manager;
	return old_hio_manager;
}


HMaterialLibrary * HDB::GetMaterialLibrary() {
	return m_pHMaterialLibrary;
}

void HDB::SetMaterialLibrary(HMaterialLibrary *hmat) {
	m_pHMaterialLibrary = hmat;
}



HTManager * HDB::GetHTManager()
{
	if(!m_pHTManager) {
		m_pHTManager = new HTManager(100);
	}

	return m_pHTManager;
}

HTManager * HDB::SetHTManager(HTManager * manager)
{
	HTManager * old_ht_manager = m_pHTManager;
	m_pHTManager = manager;
	return old_ht_manager;
}



HC_KEY HDB::GetUniqueIncludeKey()
{
	char buffer[MVO_BUFFER_SIZE];
	sprintf(buffer, "?Include Library/hdb_%lu", include_counter++);
	HC_KEY key = HC_KOpen_Segment(buffer);
	HC_Close_Segment();
	return key;
}

void HDB::ReportError( const char *message, int severity, int category, int specific )
{
	const char *tempstr = "MVO HDB::ReportError";

	if ( category == 0 )	// HEC_EXTERNAL
		category = 104;

	if ( specific == 0 )
		specific = 419;		// HES_EXTERNAL
	
	Void_Routine error_handler;

	HC_Begin_Error_Handler_Search();

		while ( HC_Find_Error_Handler(&error_handler) )
		{
			error_handler(category,specific,severity,1,&message,1,&tempstr);
		}

	HC_End_Error_Handler_Search();
	
}

 
void HDB::Lock()
{
	LOCK_MUTEX(m_Mutex);
}

void HDB::UnLock()
{
	UNLOCK_MUTEX(m_Mutex);
}





