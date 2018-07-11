// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

/*! \file HEventListener.h
	Base class for event listening.
*/
 
#ifndef _HEVENTLISTENER_H
#define _HEVENTLISTENER_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"
#include "HEventInfo.h"
 
#include "HDB.h"

class HBhvAnimation;
class HBhvTargetObject;
class HEventListener;
class HBhvSensor;

#define HLISTENER_PASS_EVENT 0
#define HLISTENER_CONSUME_EVENT 999

/*! \def HLISTENER_PRIORITY_NORMAL This level indicates an event listener wishes to receive notice of an event in a timely manner. */
#define HLISTENER_PRIORITY_NORMAL  5
/*! \def HLISTENER_PRIORITY_HIGH This level indicates an event listener wishes to receive notice of an event before other event listeners. */
#define HLISTENER_PRIORITY_HIGH  10
/*! \def HLISTENER_PRIORITY_LOW This level indicates an event listener does not need to receive notice of an event before other event listeners. */
#define HLISTENER_PRIORITY_LOW  0

/*! \enum HEventListenerType
	Defines Event Listener Types
*/
enum HEventListenerType {
	HUserdefinerListenerType,			//!< self-explanatory
	HUpdateListenerType,				//!< Listens for update events like camera changed or view destroyed
	HMouseListenerType,					//!< Listens for mouse events like mouse moves and double clicks 
	HAnimationListenerType,				//!< Listens for animation events like when key frames are added or when objects collide
	HSensorListenerType,				//!< Listens when sensors are activated
	HFitWorldListenerType,				//!< Listens for when fit world event will happen
	HObjectManipulationListenerType,	//!< Listens for object manipulation events like when objects are moved or scaled
	HJoyStickListenerType				//!< Listens for 3DMouse/Joystick translation and rotation events
};

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#define HLISTENER_EVENT(ListenerType, manager, callback) \
	{ \
		ListenerType * listener = (ListenerType *)manager->GetEventListenerManager(ListenerType::GetType()); \
		if (listener) \
			(listener->callback); \
	}

#define HLISTENER_EVENT_STRING(ListenerType, manager, callback) \
	{ \
		ListenerType * listener = (ListenerType *)manager->GetEventListenerManager(ListenerType::GetStringType()); \
		if (listener) \
			(listener->callback); \
	}

#define HLISTENER_FUNCTION_0(Class, Method) \
	virtual int Method() { \
		if (!m_pEventListenerManager) \
			return 0; \
		m_pEventListenerManager->StartQuery(); \
		Class * listener; \
		while ((listener = (Class *)m_pEventListenerManager->GetEventListener()) != 0) \
			if (listener->Method() == HLISTENER_CONSUME_EVENT) \
				break; \
		return 1; \
	}

#define HLISTENER_FUNCTION_1(Class, Method, ParamType_1, param1) \
	virtual int Method(ParamType_1 param_1) { \
		if (!m_pEventListenerManager) \
			return 0; \
		m_pEventListenerManager->StartQuery(); \
		Class * listener; \
		while ((listener = (Class *)m_pEventListenerManager->GetEventListener()) != 0) \
			if (listener->Method(param_1) == HLISTENER_CONSUME_EVENT) \
				break; \
		return 1; \
	}

#define HLISTENER_FUNCTION_2(Class, Method, ParamType_1, param_1, ParamType_2, param_2) \
	virtual int Method(ParamType_1 param_1, ParamType_2 param_2) { \
		if (!m_pEventListenerManager) \
			return 0; \
		m_pEventListenerManager->StartQuery(); \
		Class * listener; \
		while ((listener = (Class *)m_pEventListenerManager->GetEventListener()) != 0) \
			if (listener->Method(param_1, param_2) == HLISTENER_CONSUME_EVENT) \
				break; \
		return 1; \
	}

#define HLISTENER_FUNCTION_3(Class, Method, ParamType_1, param_1, ParamType_2, param_2, ParamType_3, param_3) \
	virtual int Method(ParamType_1 param_1, ParamType_2 param_2, ParamType_3 param_3) { \
		if (!m_pEventListenerManager) \
			return 0; \
		m_pEventListenerManager->StartQuery(); \
		Class * listener; \
		while ((listener = (Class *)m_pEventListenerManager->GetEventListener()) != 0) \
			if (listener->Method(param_1, param_2, param_3) == HLISTENER_CONSUME_EVENT) \
				break; \
		return 1; \
	}

#define HLISTENER_SETUP_FUNCTIONS(ListenerType) \
	static HEventListenerType GetType() \
	{ \
		return ListenerType##Type; \
	} \
	virtual HEventListener * CreateListenerManager() \
	{ \
		HEventListener * listener = (HEventListener *)new ListenerType(); \
		listener->ConvertToManager(); \
		return listener; \
	}

#define HLISTENER_SETUP_FUNCTIONS_STRING(ListenerType, string_type) \
	HLISTENER_SETUP_FUNCTIONS(HUserdefinerListener); \
	static const char * GetStringType() \
	{ \
		return string_type; \
	}

#endif //DOXYGEN_SHOULD_SKIP_THIS



class HMouseListenerManager;
class HUpdateListenerManager;



//! The HEventListenerManager class is used to dispatch event for a specific type
class MVO_API HEventListenerManager
{
public:
	/*!
	Constructs an HEventListenerManager object.
	*/
	HEventListenerManager();
	virtual ~HEventListenerManager();


	/*!
	This method adds eventlistener to a list of subscribers who want to receive notification 
	when an event handled by this manager occurs.
	\param eventlistener Event Listener to add.
	\param priority Priority for this listener.  You can pass one of the following #HLISTENER_PRIORITY_NORMAL, #HLISTENER_PRIORITY_HIGH or #HLISTENER_PRIORITY_LOW.
 	*/	
	void AddEventListener(HEventListener * eventlistener, int priority);

	/*!
	This method stops the query process.  If you call GetEventListener() after a call to Reset(), GetEventListener
	will return a null pointer.
	*/
	void Reset();

	/*!
	This method starts the query process.  Once StartQuery() has been called,
	*/
	void StartQuery();

	/*!
	Use this method to iterate through the list of event listeners during a query.  If StartQuery() was not
	called first, then the method will return 0.
	\return A pointer to next Event Listener in the priority queue.  This method will return null if you are 
	at the end of queue or StartQuery was not called to begin the query process.
	*/
	HEventListener * GetEventListener();

	/*!
	This method removes eventlistener from the list of subscribers who want to receive notification 
	when an event handled by this manager occurs.
	\param eventlistener Event Listener to remove.
	*/
	bool RemoveEventListener(HEventListener *eventlistener);

protected:
	struct vlist_s * m_pEventListenerList;		/*!< List of Event Listeners */
	bool m_bQueryInProgress;					/*!< True if query in progress and False if query not in progress. */
};





//! The HEventListener class is the base class for all event  types
/*!
	By subclassing from HEventListener any object can register itself with the event manager and receive events
	from arbitrary senders.
*/
class MVO_API HEventListener
{
public:
	/*!
	Constructs an HEventListener object.
	*/
	HEventListener() { m_pEventListenerManager = 0; m_RequestedPriority = -1; }

	virtual ~HEventListener() { 
		if (m_pEventListenerManager)
			delete m_pEventListenerManager;
	}

	/*!
	Converts a listener object to a manager object.
	*/
	void ConvertToManager() {
		if (m_pEventListenerManager == 0)
			m_pEventListenerManager = new HEventListenerManager();
	}

	/*!
	Creates an HEventListenerManager for this type of HEventListener.
	\return A pointer to the HEventListener object that contains the HEventListenerManager that was created.
	*/
	virtual HEventListener * CreateListenerManager() { return 0; }

	/*!
	\return A pointer to Event Listener Manager Object.
	*/
	HEventListenerManager * GetEventListenerManager() { return m_pEventListenerManager; }

	/*! Use this method to indicate the priority in which you like to receive an event.  A high priority 
	means that you would like to receive this event before any other listener.
	\return The requested priority level which can be #HLISTENER_PRIORITY_NORMAL, #HLISTENER_PRIORITY_HIGH or #HLISTENER_PRIORITY_LOW.
	*/
	void SetRequestedPriority(int v) { m_RequestedPriority = v; }

	/*!
	\return The requested priority level which can be #HLISTENER_PRIORITY_NORMAL, #HLISTENER_PRIORITY_HIGH or #HLISTENER_PRIORITY_LOW.
	*/
	int GetRequestedPriority() { return m_RequestedPriority; }

protected:
	HEventListenerManager * m_pEventListenerManager;	/*!< A pointer to the event listener manager. */
	int m_RequestedPriority; /*!< The priority in which this event listener would like to receive notice of events.  The possible values are #HLISTENER_PRIORITY_NORMAL, #HLISTENER_PRIORITY_HIGH or #HLISTENER_PRIORITY_LOW. */
};



#ifndef DOXYGEN_SHOULD_SKIP_THIS


//!  HEventListener derived class used for Update events
class MVO_API HUpdateListener : public HEventListener
{
public:
	/*!
	Constructs a HUpdateListener object
	*/
	HLISTENER_SETUP_FUNCTIONS(HUpdateListener);
	HLISTENER_FUNCTION_1(HUpdateListener, CameraChangedEvent, HBaseView *, view);
	HLISTENER_FUNCTION_2(HUpdateListener, UpdateEvent, bool, antialias, bool, forceUpdate);
	HLISTENER_FUNCTION_1(HUpdateListener, ViewDestroyedEvent, HBaseView *, view);
	HLISTENER_FUNCTION_1(HUpdateListener, SmoothTransitionFinishedEvent, HBaseView *, view);
};

//!  HEventListener derived class used for FitWorld events
class MVO_API HFitWorldListener : public HEventListener
{
public:
	/*!
	Constructs a HFitWorldListener object
	*/
	HLISTENER_SETUP_FUNCTIONS(HFitWorldListener);
	HLISTENER_FUNCTION_1(HFitWorldListener, PreFitWorldEvent, HBaseView *, view);
	HLISTENER_FUNCTION_1(HFitWorldListener, PostFitWorldEvent, HBaseView *, view);
};

//!  HEventListener derived class used for Object Manipulation events
class MVO_API HObjectManipulationListener : public HEventListener
{
public:
	/*!
	Constructs a HObjectManipulationListener object
	*/
	HLISTENER_SETUP_FUNCTIONS(HObjectManipulationListener);
	HLISTENER_FUNCTION_3(HObjectManipulationListener, SetupHandlesEvent, HBaseView *, view, HC_KEY, key, bool, complex);
	HLISTENER_FUNCTION_3(HObjectManipulationListener, ObjectBeforeMoveEvent, HBaseView *, view, HC_KEY, key, HPoint *, p);
	HLISTENER_FUNCTION_3(HObjectManipulationListener, ObjectMovedEvent, HBaseView *, view, HC_KEY, key, HPoint *, p);
	HLISTENER_FUNCTION_3(HObjectManipulationListener, ObjectScaledEvent, HBaseView *, view, HC_KEY, key, HPoint &, p);
};


//!  HEventListener derived class used for animation events
class MVO_API HAnimationListener : public HEventListener
{
public:
	/*!
	Constructs a HAnimationListener object
	*/
	HLISTENER_SETUP_FUNCTIONS(HAnimationListener);

	HLISTENER_FUNCTION_1(HAnimationListener, AnimationFinishedEvent, HBhvAnimation *, ainst);
	HLISTENER_FUNCTION_1(HAnimationListener, KeyframeEditorExistsQuery, bool &, res);
	HLISTENER_FUNCTION_0(HAnimationListener, KeyframeAddedEvent);
	HLISTENER_FUNCTION_1(HAnimationListener, ObjectCollisionEvent, HBhvTargetObject *, tob);
	HLISTENER_FUNCTION_1(HAnimationListener, ObjectNoCollisionEvent, HBhvTargetObject *, tob);
};


//!  HEventListener derived class used for sensor events
class MVO_API HSensorListener : public HEventListener
{
public:
	/*!
	Constructs a HSensorListener object
	*/
	HLISTENER_SETUP_FUNCTIONS(HSensorListener);
	HLISTENER_FUNCTION_1(HSensorListener, SensorActivatedEvent, HBhvSensor *, sensor);
	HLISTENER_FUNCTION_1(HSensorListener, SensorActionEvent, const char *, action);
};



//!  HEventListener derived class used for mouse events
class MVO_API HMouseListener : public HEventListener
{
public:
	/*!
	Constructs a HMouseListener object
	*/
	HLISTENER_SETUP_FUNCTIONS(HMouseListener);

	HLISTENER_FUNCTION_1(HMouseListener, OnLButtonDown, HEventInfo &, hevent);
	HLISTENER_FUNCTION_1(HMouseListener, OnMouseMove, HEventInfo &, hevent);
	HLISTENER_FUNCTION_1(HMouseListener, OnLButtonUp, HEventInfo &, hevent);
	HLISTENER_FUNCTION_1(HMouseListener, OnLButtonDblClk, HEventInfo &, hevent);
	HLISTENER_FUNCTION_1(HMouseListener, OnMButtonDown, HEventInfo &, hevent);
	HLISTENER_FUNCTION_1(HMouseListener, OnMButtonUp, HEventInfo &, hevent);
	HLISTENER_FUNCTION_1(HMouseListener, OnMButtonDblClk, HEventInfo &, hevent);
	HLISTENER_FUNCTION_1(HMouseListener, OnRButtonDown, HEventInfo &, hevent);
	HLISTENER_FUNCTION_1(HMouseListener, OnRButtonUp, HEventInfo &, hevent);
	HLISTENER_FUNCTION_1(HMouseListener, OnRButtonDblClk, HEventInfo &, hevent);
	HLISTENER_FUNCTION_1(HMouseListener, OnMouseWheel, HEventInfo &, hevent);
	HLISTENER_FUNCTION_1(HMouseListener, OnKeyDown, HEventInfo &, hevent);
	HLISTENER_FUNCTION_1(HMouseListener, OnKeyUp, HEventInfo &, hevent);
	HLISTENER_FUNCTION_1(HMouseListener, OnTouchesDown, HEventInfo &, hevent);
	HLISTENER_FUNCTION_1(HMouseListener, OnTouchesMove, HEventInfo &, hevent);
	HLISTENER_FUNCTION_1(HMouseListener, OnTouchesUp, HEventInfo &, hevent);
};

//!  HEventListener derived class used for 3DMouse/Joystick events
class MVO_API HJoyStickListener : public HEventListener
{
public:
	/*!
	Constructs a HJoyStickListener object
	*/
	HLISTENER_SETUP_FUNCTIONS(HJoyStickListener);

	HLISTENER_FUNCTION_1(HJoyStickListener, OnJoyStickRotation, HEventInfo &, hevent);
	HLISTENER_FUNCTION_1(HJoyStickListener, OnJoyStickTranslation, HEventInfo &, hevent);
	HLISTENER_FUNCTION_1(HJoyStickListener, OnJoyStickButton, HEventInfo &, hevent);
};

#endif	//DOXYGEN_SHOULD_SKIP_THIS


#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif




