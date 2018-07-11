// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

//! Interface of the HImManager class
/*! \file HImManager.h
	Defines the HImManager class along with macros for registering, setting and unsetting callbacks
*/

#ifndef _HIM_MANAGER_H
#define _HIM_MANAGER_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"




/*!
    \def HImRegisterCallback(callback_name, callback_function) 
    This method associates a name with a function pointer.
*/
#define HImRegisterCallback(callback_name, callback_function) \
	do{ (HImManager::GetCurrentHImManager())->RegisterOneCallback( \
			(const char *)callback_name, (void*)callback_function); }while(0);
/*!
    \def HImSetCallback(callback_type, callback_name)
    This method associates a callback type with a registered callback function name in the currently open segment.
*/
#define HImSetCallback(callback_type, callback_name) \
	do{ (HImManager::GetCurrentHImManager())->SetCallback( \
			(const char *)callback_type, (const char *)callback_name); }while(0);
/*!
    \def HImSetCallbackByKey(segment_key, callback_type, callback_name)
    This method associates a callback type with a registered callback function name in the segment specified by segment_key.
*/
#define HImSetCallbackByKey(segment_key, callback_type, callback_name) \
	do{ (HImManager::GetCurrentHImManager())->SetCallback( \
			segment_key, (const char *)callback_type, (const char *)callback_name); }while(0);
/*!
    \def HImUnSetOneCallback(callback_type)  
    This method removes a callback assocation in the currently open segment.
*/
#define HImUnSetOneCallback(callback_type) \
	do{ (HImManager::GetCurrentHImManager())->UnSetOneCallback( \
			callback_type); }while(0);

/*!
    \def HImUnSetOneCallbackByKey(segment_key, callback_type)  
    This method removes the callback assocation in a segment specified by segment_key.
*/
#define HImUnSetOneCallbackByKey(segment_key, callback_type) \
	do{ (HImManager::GetCurrentHImManager())->UnSetOneCallback( \
			segment_key, (const char *)callback_type); }while(0);

/*!
    \def HImShowCallback(segment_name, callback_type)  
    This method writes the name of the callback_type assocation into the callback_name
    in the currently open segment.
*/
#define HImShowCallback(callback_name, callback_type) \
	do{ (HImManager::GetCurrentHImManager())->ShowCallback( \
			(const char *)callback_name, (char *)callback_type); }while(0);
 




//! The HImManager class serves as a manager for IM callbacks.  
/*!
	HImManager is created on demand by HDB and should be
	accessed with the HIm macro's rather than directly with 
	function calls.  These should be used instead of HIC_ 
	calls to allow MVO to function correctly.

	This class is currently not much more than a wrapper over HIC_
  	calls.  But should be used because future features may require it.

	See HIC_ documentation for greater understanding of HOOPS IM in general.
*/
class MVO_API HImManager
{

	public:

		HImManager();
		virtual ~HImManager();

		//! This sets the default MVO callbacks.
		virtual void RegisterCallbacks();

		/*! 
			This method sets one callback. It should not be used directly but
			through the HImRegisterCallback macro instead.

			\param callback_name A null terminated string with the callback name.
			\param callback A pointer to the callback function cast to a void *	
		*/
		virtual void RegisterOneCallback(const char * callback_name, void * callback);

		/*! 
			This is a wrapper around #HC_Set_Callback(). It should not be used directly but
			through the HImRegisterCallbackByKey macro instead.

			\param segment_key A segment to set the callback on.
			\param callback_type A null terminated string with the callback type these are documented with HIC.
			\param callback_name A null terminated string with the callback name.
		*/
		virtual void SetCallback(HC_KEY segment_key, const char * callback_type, const char * callback_name);
		
		/*! 
			This is a wrapper around #HC_Set_Callback(). It should not be used directly but
			through the HImRegisterCallback macro instead.  This sets the callback in 
			the currently open segment.

			\param callback_type A null terminated string with the callback type these are documented with HIC.
			\param callback_name A null terminated string with the callback name.
		*/
		virtual void SetCallback(const char * callback_type, const char * callback_name);

		/*! 
			This is a wrapper around #HC_Set_Callback(). It should not be used directly but
			through the HImUnSetOneCallbackByKey macro instead.

			\param segment_key A segment to unset the callback on.
			\param callback_type A null terminated string with the callback type these are documented with HIC.
		*/
		virtual void UnSetOneCallback(HC_KEY segment_key, const char * callback_type);

		/*! 
			This is a wrapper around #HC_Set_Callback(). It should not be used directly but
			through the HImUnSetOneCallback macro instead.

			\param callback_type A null terminated string with the callback type these are documented with HIC.
		*/
		virtual void UnSetOneCallback(const char * callback_type);

		/*! 
			This is a wrapper around #HC_Set_Callback(). It should not be used directly but
			thought the HImShowOneCallback macro instead.  It operates on the curently open segment.

			\param callback_type A null terminated string with the callback type these are documented with HIC.
			\param callback This is a write back string with the currently set callback for specified type 
		*/
		virtual void ShowCallback(const char *callback_type, char * callback);

		/*!
			This HImManager::GetCurrentHImManager() will create a HImManager if none exists.	
		
			\return A pointer to the current HImManager set on HDB.
		*/
		static HImManager *GetCurrentHImManager();

};


#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif







