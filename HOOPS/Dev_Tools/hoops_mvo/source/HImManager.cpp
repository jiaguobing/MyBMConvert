// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



#include "HImManager.h"

#include "hc.h"
#include "hic.h"

#include "HDB.h"

#include "HUtility.h"
#include "HImUtility.h"
#include "HImErrorHandler.h"
#ifndef TEST_RIG_1
#include "HConstantFrameRate.h"
#include "HImHideOverlappedText.h"
#include "HOpObjectAnnotate.h"
#include "HOpMarkupFreehand.h"
#include "HImCuttingGeometry.h"
#include "HDebugZBuffer.h"
#include "HUtilityAntialiasing.h"
#include "HUtilityAnnotation.h"
#include "HUtilityAnnotationMeasure.h"
#endif


HImManager::HImManager() 
{
;
}

HImManager::~HImManager()
{
;
}

HImManager* HImManager::GetCurrentHImManager()
{
	return HDB::GetHImManager();
}


#define HIM_FUNCTION(function) (void (HC_CDECL*)(HC_ANY_ARGS))(function)

void HImManager::RegisterOneCallback(const char * callback_name, void * callback)
{
	HC_Define_Callback_Name(callback_name, HIM_FUNCTION(callback));
}

void HImManager::SetCallback(const char * callback_type, const char * callback_name)
{
	char tmp[MVO_BUFFER_SIZE];
	sprintf(tmp,"%s = %s", callback_type, callback_name);
	HC_Set_Callback(tmp);
}

void HImManager::ShowCallback(const char *callback_type, char * callback)
{
  	char list[MVO_BUFFER_SIZE];
 	if (HC_Show_Existence("callbacks"))
	{
		HC_Show_Callback(list);
		if (strstr(list, callback_type))
		{
			HC_Show_One_Callback(callback_type, callback);
			return;
		}
	}
	strcpy(callback, "");
}


void HImManager::SetCallback(HC_KEY segment_key, const char * callback_type, const char * callback_name)
{
	char tmp[MVO_BUFFER_SIZE];
	sprintf(tmp,"%s = %s", callback_type, callback_name);
	HC_Open_Segment_By_Key(segment_key);	
	HC_Set_Callback(tmp);
	HC_Close_Segment();
}

void HImManager::UnSetOneCallback(const char * callback_type)
{
	HC_UnSet_One_Callback(callback_type);
}

void HImManager::UnSetOneCallback(HC_KEY segment_key, const char * callback_type)
{
	HC_Open_Segment_By_Key(segment_key);	
	HC_UnSet_One_Callback(callback_type);
	HC_Close_Segment();
}



void HImManager::RegisterCallbacks()
{
#ifndef TEST_RIG_1
	HFrSolidBBox::RegisterCallbacks();
	HFrWireframeBBox::RegisterCallbacks();
	HImHideOverlappedText::RegisterCallbacks();
	HImCuttingGeometry::RegisterCallbacks();
	HUtilityAntialiasing::RegisterCallbacks();
	HDebugZBuffer::RegisterCallbacks();
#endif
	HImErrorHandler::RegisterCallbacks();
	HImUtility::RegisterCallbacks();
 	HUtility::RegisterCallbacks();
}





