// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef _HEVENTMANAGER_H
#define _HEVENTMANAGER_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"
#include "HEventListener.h"

class HEventListener;
class HBaseView;

//! The HEventListenerItem class encapsulates a pointer to an event listener
 
class HEventListenerItem
{
public:
	/*!
	Constructs an HEventListenerItem object
 	*/
	HEventListenerItem() { m_Priority = 0; m_pEventListener = 0; }
	int m_Priority;							/*!< Item Priority */
	HEventListener *m_pEventListener;		/*!< Pointer to Event Listener */
};



//! The HEventManager class dispatches arbitrary events
/*!
	By registering with the HEventManager, any class that subclasses from HEventListener can receive events
	from arbitrary sender. Each event type can have any number of recipients
*/ 
class MVO_API HEventManager  
{
public:
	/*!
	Constructs an HEventManager object.
 	*/
	HEventManager();
	virtual ~HEventManager();

	/*!
	Register handler for a user-defined event type.
	\param listener Event listener to receive event
	\param eventtype A user-defined event type.
	\param priority Event Priority
 	*/	
	void RegisterHandler(HEventListener *listener, const char * eventtype, int priority);
	/*!
	Register handler for an event type.
	\param listener Event listener to receive event
	\param eventtype One of the enumerations from #HEventListenerType.
	\param priority Event Priority
 	*/	
	void RegisterHandler(HEventListener *listener, HEventListenerType eventtype, int priority);

	/*!
	Get event listener manager for specified user-defined event type.
 	\param eventtype A user-defined event type.
	\return Event Listener Manager
  	*/	
	HEventListener *GetEventListenerManager(const char *eventtype);
	/*!
	Get event listener manager for specified event type.
 	\param eventtype One of the enumerations from #HEventListenerType.
	\return A pointer to the associatend event listener manager.
  	*/	
	HEventListener *GetEventListenerManager(HEventListenerType eventtype);

	/*!
	Unregister specified event listener from the event manager for the specified user-defined event type.
  	\param listener Event Listener
 	\param eventtype A user-defined event type.
  	*/	
	bool UnRegisterHandler(HEventListener *listener, const char * eventtype);
	/*!
	Unregister specified event listener from Event Manager.
  	\param listener Event Listener
 	\param eventtype One of the enumerations from #HEventListenerType.
  	*/	
	bool UnRegisterHandler(HEventListener *listener, HEventListenerType eventtype);
	/*!
	Unregister specified event listener from Event Manager for all event types.
  	\param listener Event Listener
   	*/	
	void UnRegisterHandler(HEventListener *listener);


protected:


	struct vhash_s *	m_pEventListenerListHash;			/*!< Event Listener Hash for event types defined in #HEventListenerType. */
	struct vhash_s *	m_pEventListenerListHashString;		/*!< Event Listener Hash for user-defined event types.*/

};

//! The HObject is the base class for generic MVO objects.
/*!
 
   HObject instances need to register themselves with the ObjectManager. They can receive events and allow for the
   persistent storage of data.
*/ 

class MVO_API HObject
{
public:
	/*!
	Constructs an HObject object 	*/

    HObject() {} ;
    virtual  ~HObject() {} ;

	/*! \return A character pointer denoting the name of the object. 
	*/
    virtual const char *GetName() = 0;
protected:
};


//! The HObjectManager class keeps track of generic MVO objects
/*!
	By registering with the HObjectManager, a class derived from HObject can store persistent data and receive events.

 */ 

class MVO_API HObjectManager  
{
public:
	/*!
	Constructs an HObjectManager object.
 	*/	
	HObjectManager();
	virtual ~HObjectManager();

	/*!
	This method adds a new object to the object list.
  	\param hobject A pointer to the HObject to add.
 	\return  A pointer to the HObject that was replaced by the passed object if there was an object with 
	the same name already or null if there was no object with the same name already in the list.  
  	*/	
 	HObject * AddHObject(HObject *hobject);
	/*!
	This method removes an object from the object list.
  	\param name The name of the object to be removed.
 	\return  A pointer to the removed object or null if none was found.
  	*/	
 	HObject *RemoveHObject(const char *name);
	/*!
	This method retrieves an object from object list based on object name.
  	\param name The name of the object to be retrieved.
 	\return A pointer to the retrieved HObject or null if none was found.
  	*/	
 	HObject *GetHObject(const char *name);
	
	/*!
	Given a segment key, this methods finds the next object segment that this segment is contained in.
  	\param segkey The segment to search from.
	\param res If non-zero, returns name of object.
	\param ores If non-zero, returns object information.
 	\return The segment key corresponding to object segment or invalid key.
  	*/	
	static HC_KEY FindHObjectSegment(HC_KEY segkey, char *res = 0, char *ores = 0);

	
	/*!
	This method is called when a file finishes loading.  This function checks whether there are any object segments.
	\param segkey The segment to search from.
	*/
	static void FileLoadHObjectCheck(HC_KEY segkey);

	/*!
	This method declares the currently open segment an object segment and associates an object name with it.
   	\param objecttype The name which you want to label the object as.
   	*/	
	static void SetHObjectSegment(const char *objecttype);
	/*!
	The method declares the currently open segment an object segment, associates an object name with 
	it if it doesn't have one and set the object info.
   	\param objectinfo A pointer to the infodata for object.
   	*/	
	static void SetHObjectInfo(const char *objectinfo);


protected:

	struct vhash_s *	m_pObjectHash;	    	/*!< Object Hash */

	static long	object_segment_count;

};


#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif



