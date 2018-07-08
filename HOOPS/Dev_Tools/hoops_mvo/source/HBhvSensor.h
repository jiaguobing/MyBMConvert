// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef _H_HBhvSensor_H
#define _H_HBhvSensor_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"
#include "HEventListener.h"

class HBhvBehaviorManager;
class HBhvCondition;
class HBhvAction;
class HBhvTargetObject;
 


//! The HBhvSensor class encapsulates an animation sensor.
/*!	
	A HBhvSensor objects holds a list of conditions which are usually related to an object in the segment hierachy
	and actions that should be performed based on whether these conditions are met.
*/ 
class MVO_API HBhvSensor  
{
public:

	/*!
	Constructs an HBhvSensor object.
	\param name The name of the sensor.
	\param active The activity state.  Pass true if you want this sensor to be active by default. 
	\param BehaviorManager A pointer to the #HBhvBehaviorManager object.
  	*/
	HBhvSensor(const char *name, bool active, HBhvBehaviorManager *BehaviorManager);
	virtual ~HBhvSensor();

	/*!
	Adds a new condition to the condition list.
	\param condition A pointer to condition object to be added.
  	*/
	void AddCondition(HBhvCondition *condition);

	/*!
	This is XML read callback which parses XML data and then populates the member variables with the associated values.
  	*/
	static void *XMLCallback(HUtilityXMLTag *xt, bool open, void *m_pExtraData);

	/*!
	Writes XML data to a buffer.
  	*/
	void Serialize(HUtilityXMLGenerator *xmlgen);

	/*! \return A pointer to HBhvBehaviorManager object */
	HBhvBehaviorManager * GetBehaviorManager() { return m_pBehaviorManager; }

	/*!
	This method activates the sensor.  It fires the sensor activated event and registers the items in the
	conditions array.
	\param delay The delay time in ticks before activation.
  	*/
	void Activate(int delay = 0);

	/*!
	This method deactivates the sensor and unregisters all the items in the conditions array.
   	*/
	void DeActivate();

	/*! \return True if the sensor is active by default or false if the sensor is inactive by default. */
 	bool GetDefaultActive() { return m_bDefaultActive; }

	/*! \return True if the sensor is currently active or false if ensor is not currently active. */
	bool GetCurrentlyActive() { return m_bCurrentlyActive; }

	/*!
	Evaluates the sensor conditions.  If the conditions are met, then the actions list is executed.
	\return True if the conditions are met to true or false if conditions are not met.
    */
	bool Evaluate();

	/*!
	Adds a new item to the action list.
	\param action A pointer to the action object you want added.
  	*/
	void AddAction(HBhvAction *action);

	/*! \return A pointer to the name of the sensor. */
	const char * GetName() { return m_Name; }

	/*!
	Performs any time based sensor action.
   	*/
 	void Tick();


protected:
   	char					m_Name[256];			/*!< The sensor name*/
	HBhvBehaviorManager *	m_pBehaviorManager;		/*!< A pointer to behaviour manager for this sensor.*/
 
	struct vlist_s*			m_ConditionList;		/*!< The list of conditions.*/
	struct vlist_s*			m_ActionList;			/*!< The list of actions.*/

	bool m_bCurrentlyActive;						/*!< The value is true if the sensor is currently active or false if the sensor not active.*/
	bool m_bDefaultActive;							/*!< The value is true if the sensor is active by default, false if the sensor not active by default.*/

	int m_Delay;									/*!< The delay before the sensor activates.*/
};


//! The HBhvCondition class is the base class for all condition types.
class MVO_API HBhvCondition  
{
public:

	/*!
	Constructs an HBhvCondition object.
	\param target The segment path of the target for this condition.
	\param hevent The event type.
	\param invert Pass true to invert the condition.  In other words, this condition will return true if the condition evaluates to false 
	and false if the condition evaluates to true.
	\param sensor A pointer to the HBhvSensor object that this condition belongs to.
   	*/
	HBhvCondition(const char *target, const char *hevent, bool invert, HBhvSensor *sensor);
	virtual ~HBhvCondition();

	/*!
	Sets a target for this condition.
 	\param target The segment path of the targetfor this condition.
   	*/	
	virtual void SetTarget(const char *target);

	/*! \return A pointer to the sensor object. */
  	virtual HBhvSensor * GetSensor() { return m_pSensor; }

	/*!
	Registers the condition with event manager (if applicable)
   	*/	
	virtual void Register() { }

	/*!
	Unregisters condition from event manager (if applicable)
   	*/	
	virtual void UnRegister() { }

	/*!
	Evaluates the condition.
	\param and_flag Result of evaluation
	\param or_flag Result of evaluation
   	*/	
	virtual void Evaluate(bool &and_flag, bool &or_flag);

	/*!
	This is XML read callback which parses XML data and then populates the member variables with the associated values.
  	*/
	static void *XMLCallback(HUtilityXMLTag *xt, bool open, void *m_pExtraData);

	/*!
	Writes XML data to a buffer.
  	*/
	virtual void Serialize(HUtilityXMLGenerator *xmlgen, HUtilityXMLTag *xmlt);

	/*!
	Writes XML data to a buffer.
  	*/
	virtual void Serialize(HUtilityXMLGenerator *xmlgen);

 protected:
   	char					m_Event[256];				/*!< The event type.*/
	HBhvTargetObject*		m_pTarget;					/*!< A pointer to the target object.*/
	bool					m_bConditionMet;			/*!< The value is true if the condition was met or false if the condition was not met.*/
	HBhvSensor*				m_pSensor;					/*!< The parent sensor.*/
	bool					m_bInvert;					/*!< The value is true if you want the condition inverted.*/
	char					m_Value[256];				/*!< Extra data for the conditon.*/

};


//! The HBhvConditionAnimationRunning class evaluates if an animation is currently running for a given target.
class MVO_API HBhvConditionAnimationRunning : public HBhvCondition 
{
public:
	/*!
	Constructs an HBhvConditionAnimationRunning object.
	\param target The target string of the animation you want to test the condition on.  You can use HBhvTargetObject::CreateTargetString() to generate this string.
	\param hevent The event type.
	\param invert Pass true to invert the condition.  In other words, this condition will return true if the condition evaluates to false 
	and false if the condition evaluates to true.
	\param sensor A pointer to the HBhvSensor object that this condition belongs to.
   	*/
	HBhvConditionAnimationRunning(const char *target, const char *hevent, bool invert, HBhvSensor *sensor);

	/*!
	Evaluates the condition.
	\param and_flag Returns true if an animation is currently running for the given target object or false if no 
	animation is running.  Note that this flag will return the opposite values if the invert flag is set to <i>true</i>.
	\param or_flag Result of evaluation
   	*/	
	virtual void Evaluate(bool &and_flag, bool &or_flag);

	/*!
	Writes XML data to a buffer.
  	*/
	virtual void Serialize(HUtilityXMLGenerator *xmlgen);
	 
};

//! The HBhvConditionSensorActive class evaluates if the given sensor is currently active.
class MVO_API HBhvConditionSensorActive : public HBhvCondition 
{
public:

	/*!
	Constructs an HBhvConditionSensorActive object.
	\param target The descriptor of the target sensor.  You can use HBhvTargetObject::CreateTargetString() to generate this string.
	\param hevent The event type.
	\param invert Pass true to invert the condition.  In other words, this condition will return true if the condition evaluates to false 
	and false if the condition evaluates to true.
	\param sensor A pointer to the HBhvSensor object that this condition belongs to.
  	*/
	HBhvConditionSensorActive(const char *target, const char *hevent, bool invert, HBhvSensor *sensor);

	/*!
	Evaluates the condition.
	\param and_flag Returns true if the target sensor was active or false if it is inactive.  Note that this 
	flag will return the opposite values if the invert flag is set to <i>true</i>.
	\param or_flag Result of evaluation
   	*/
	virtual void Evaluate(bool &and_flag, bool &or_flag);

	/*!
	Writes XML data to a buffer.
  	*/
	virtual void Serialize(HUtilityXMLGenerator *xmlgen);
	 
protected:
};



//! The HBhvConditionMouse class is the base class for all mouse related conditions.
class MVO_API HBhvConditionMouse : public HBhvCondition, public HMouseListener  
{
public:

	/*!
	Constructs an HBhvConditionMouse object.
	\param target The descriptor of the target object. You can use HBhvTargetObject::CreateTargetString() to generate this string.
	\param hevent The event type.
	\param invert Pass true to invert the condition.  In other words, this condition will return true if the condition evaluates to false 
	and false if the condition evaluates to true.
	\param sensor A pointer to the HBhvSensor object that this condition belongs to.
   	*/
	HBhvConditionMouse(const char *target, const char *hevent, bool invert, HBhvSensor *sensor);

	/*!
	Registers the condition with event manager for mouse events.
   	*/	
 	virtual void Register();

	/*!
	Unregisters the condition so that it no longer gets mouse events.  
   	*/	
	virtual void UnRegister();


	/*!
	Determines if the given target object was selected.
	\param target The key of the target object.
	\param x The mouse x coordinate in window space.
	\param y The mouse y coordinate in window space.
	\return True if the given target was selected by the mouse or false if otherwise.
   	*/	
	bool IsClicked(HC_KEY target, float x, float y);
 
	 
};

//! The HBhvConditionAnimation class is the base class for all animation related conditions.
class MVO_API HBhvConditionAnimation : public HBhvCondition, public HAnimationListener  
{
public:

	/*!
	Constructs an HBhvConditionAnimation object.
	\param target The descriptor of the animation target object. You can use HBhvTargetObject::CreateTargetString() to generate this string.
	\param hevent The event type.
	\param invert Pass true to invert the condition.  In other words, this condition will return true if the condition evaluates to false 
	and false if the condition evaluates to true.
	\param sensor A pointer to the HBhvSensor object that this condition belongs to.
   	*/
	HBhvConditionAnimation(const char *target, const char *hevent, bool invert, HBhvSensor *sensor);
  
	/*!
	Registers condition with event manager to receive animation events.
   	*/	
 	virtual void Register();

	/*!
	Unregisters condition with event manager so that it no longer receives animation events.
   	*/	
	virtual void UnRegister();
protected:
};

//! The HBhvConditionSensor class is the base class for sensor related conditions.
class MVO_API HBhvConditionSensor : public HBhvCondition, public HSensorListener  
{
public:

	/*!
	Constructs an HBhvConditionSensor object,
	\param target The descriptor of the target sensor.  You can use HBhvTargetObject::CreateTargetString() to generate this string.
	\param hevent The event type.
	\param invert Pass true to invert the condition.  In other words, this condition will return true if the condition evaluates to false 
	and false if the condition evaluates to true.
	\param sensor A pointer to the HBhvSensor object that this condition belongs to.
   	*/
	HBhvConditionSensor(const char *target, const char *hevent, bool invert, HBhvSensor *sensor);
  
	/*!
	Registers the condition with event manager to receive sensor events.
   	*/	
 	virtual void Register();

	/*!
	Unregisters condition with event manager so that this condition will no longer receive sensor events.
   	*/	
	virtual void UnRegister();
};


//! The HBhvConditionSensorActivated class evaluates if a sensor has been activated.
class MVO_API HBhvConditionSensorActivated : public HBhvConditionSensor 
{
public:

	/*!
	Constructs an HBhvConditionSensorActivated object.
	\param target The descriptor of the target sensor.  You can use HBhvTargetObject::CreateTargetString() to generate this string.
	\param hevent The event type.
	\param invert Pass true to invert the condition.  In other words, this condition will return true if the condition evaluates to false 
	and false if the condition evaluates to true.
	\param sensor A pointer to the HBhvSensor object that this condition belongs to.
    	*/
	HBhvConditionSensorActivated(const char *target, const char *hevent, bool invert, HBhvSensor *sensor);

	/*!
	Called by the event manager if a sensor has been activated.  If the sensor that was activated is the target sensor, 
	this condition of this HBhvConditionSensorActivated associated sensor are evaluated.
   	*/
 	virtual int SensorActivatedEvent(HBhvSensor * ainst);

	/*!
	Writes XML data to a buffer.
  	*/
	virtual void Serialize(HUtilityXMLGenerator *xmlgen);

};



//! The HBhvConditionSensorAction class evaluates if the target sensor has performed an action.
class MVO_API HBhvConditionSensorAction : public HBhvConditionSensor 
{
public:
	/*!
	Constructs an HBhvConditionSensorAction object.
	\param target The descriptor of the target sensor.  You can use HBhvTargetObject::CreateTargetString() to generate this string.
	\param hevent The event type.
	\param invert Pass true to invert the condition.  In other words, this condition will return true if the condition evaluates to false 
	and false if the condition evaluates to true.
	\param sensor A pointer to the HBhvSensor object that this condition belongs to.
  	*/
	HBhvConditionSensorAction(const char *target, const char *hevent, bool invert, HBhvSensor *sensor);

	/*!
	Called by the event manager if a sensor action has been performed.  If the sensor that performed the action is 
	this condition's target sensor, then the other conditions of HBhvConditionSensorAction's sensor are evaluated.
   	*/
 	virtual int SensorActionEvent(const char * action);

	/*!
	Writes XML data to a buffer.
  	*/
	virtual void Serialize(HUtilityXMLGenerator *xmlgen);

};

//! The HBhvConditionAnimationFinished class evaluates if an animation has finished executing.
class MVO_API HBhvConditionAnimationFinished : public HBhvConditionAnimation  
{
public:
	/*!
	Constructs a HBhvConditionAnimationFinished object.
	\param target The descriptor of the target object. You can use HBhvTargetObject::CreateTargetString() to generate this string.
	\param hevent The event type.
	\param invert Pass true to invert the condition.  In other words, this condition will return true if the condition evaluates to false 
	and false if the condition evaluates to true.
	\param sensor A pointer to the HBhvSensor object that this condition belongs to.
	*/
	HBhvConditionAnimationFinished(const char *target, const char *hevent, bool invert, HBhvSensor *sensor);

	/*!
	Called by the event manager if an animation has been finished performing.  If the animation is the target animation,
	then the conditions of the associated sensor are evaluated.
   	*/ 	
	virtual int AnimationFinishedEvent(HBhvAnimation * animation);

	/*!
	Writes XML data to a buffer.
  	*/
	virtual void Serialize(HUtilityXMLGenerator *xmlgen);


};



//! The HBhvConditionONLCLICK class evaluates if the given target was selected when the left mouse button was pressed.
class MVO_API HBhvConditionONLCLICK : public HBhvConditionMouse  
{
public:

	/*!
	Constructs a HBhvConditionONLCLICK object. 
	\param target The descriptor of the target object. You can use HBhvTargetObject::CreateTargetString() to generate this string.
	\param hevent The event type.
	\param invert Pass true to invert the condition.  In other words, this condition will return true if the condition evaluates to false 
	and false if the condition evaluates to true.
  	\param sensor A pointer to the HBhvSensor object that this condition belongs to.
 	*/
	HBhvConditionONLCLICK(const char *target, const char *hevent, bool invert, HBhvSensor *sensor);

	/*!
	Called by the event manager if the left mouse button has been pressed.  This method checks to see
	if the condition was met by the mouse click event as well as all the conditions associated with this 
	conditions sensor.  
	\return An HLISTENER_CONSUME_EVENT if conditions of the associated sensor were met or HLISTENER_PASS_EVENT if they
	were not.
   	*/ 	
  	virtual int OnLButtonDown(HEventInfo &hevent);

	/*!
	Writes XML data to a buffer.
  	*/
	virtual void Serialize(HUtilityXMLGenerator *xmlgen);
	 
protected:
};



//! The HBhvConditionONMOUSEENTER class evaluates if the mouse pointer has entered the bounding box of the target object.
class MVO_API HBhvConditionONMOUSEENTER : public HBhvConditionMouse  
{
public:

	/*!
	Constructs an HBhvConditionONMOUSEENTER object.
	\param target The descriptor of the target object. You can use HBhvTargetObject::CreateTargetString() to generate this string.
	\param hevent The event type.
	\param invert Pass true to invert the condition.  In other words, this condition will return true if the condition evaluates to false 
	and false if the condition evaluates to true.
  	\param sensor A pointer to the HBhvSensor object that this condition belongs to.
   	*/
	HBhvConditionONMOUSEENTER(const char *target, const char *hevent, bool invert, HBhvSensor *sensor);

	/*!
	Called by the event manager if the mouse pointer position has changed.  This method checks to see if the mouse 
	has entered the bounding box of the target object.  If the condition was met, then the conditions of the associated
	sensor are evaluated.
   	*/ 	
  	virtual int OnMouseMove(HEventInfo &hevent);

	/*!
	Unregisters the condition so that it no longer gets mouse events.  
   	*/	
	void UnRegister();

	/*!
	Writes XML data to a buffer.
  	*/
	virtual void Serialize(HUtilityXMLGenerator *xmlgen);

protected:
	HC_KEY m_OldKey;		/*!< For internal use only.*/
};

//! The HBhvConditionONMOUSEENTER class evaluates if the mouse pointer has left the bounding box of the target object.
class MVO_API HBhvConditionONMOUSELEAVE : public HBhvConditionMouse  
{
public:

	/*!
	Constructs an HBhvConditionONMOUSELEAVE object.
	\param target The descriptor of the target object. You can use HBhvTargetObject::CreateTargetString() to generate this string.
	\param hevent The event type.
	\param invert Pass true to invert the condition.  In other words, this condition will return true if the condition evaluates to false 
	and false if the condition evaluates to true.
  	\param sensor A pointer to the HBhvSensor object that this condition belongs to.
  	*/
	HBhvConditionONMOUSELEAVE(const char *target, const char *hevent, bool invert, HBhvSensor *sensor);

	/*!
	Called by the event manager if the mouse pointer position has changed.  This method checks to see if the mouse 
	has left the bounding box of the target object.  If the condition was met, then the conditions of the associated
	sensor are evaluated.
   	*/ 	
  	virtual int OnMouseMove(HEventInfo &hevent);

	/*!
	Unregisters the condition so that it no longer gets mouse events.  
   	*/	
	void UnRegister();

	/*!
	Writes XML data to a buffer.
  	*/
	virtual void Serialize(HUtilityXMLGenerator *xmlgen);

protected:
	HC_KEY m_OldKey;		/*!< For internal use only.*/
};



//! The HBhvAction class encapsulates an animation action.
/*!	
	An action is executed based on the status of all conditions of the sensor it belongs to. Certain action types
	are directly related to specific target types.  The following is a list of action types and targets.

	Action Type: DOACTION<br>
	Associate Target Type: None<br>
	Action: Triggers a sensor action event.

	Action Type: LOADFILE<br>
	Associate Target Type: None<br>
	Action: Load a file with the path indicated the value paramater.

	Action Type: ACTIVATE<br>
	Associated Target Type: Animation<br>
	Action: Activate the target animation and then schedules it for execution.  A delay before execution can be indicated
	by passing the number ticks to the value parameter in the constructor.

	Action Type: CONTINUE<br>
	Associated Target Type: Animation<br>
	Action: Removes the target animation from the schedule animation list and then reschedules it for later.

	Action Type: DEACTIVATE<br>
	Associated Target Type: Animation<br>
	Action: Removes the target animation from the schedule animation list and set its currently running flag to false.

	Action Type: ACTIVATE<br>
	Associated Target Type: Sensor<br>
	Action: Activates the target sensor.  A delay before execution can be indicated
	by passing the number ticks to the value parameter in the constructor.

	Action Type: DEACTIVATE<br>
	Associated Target Type: Sensor<br>
	Action: Deactivates the target sensor.


 */ 
class MVO_API HBhvAction 
{
public:

	/*!
	Constructs an HBhvAction object.
	\param target The descriptor for the target object for this action.  You can use HBhvTargetObject::CreateTargetString() to generate this string.
	\param type The action type which are described in the detailed description of this class.
	\param value Additional data depending on the action type and target type.
	\param sensor A pointer to the HBhvSensor object that this action belongs to.
   	*/
	HBhvAction(const char *target, const char *type, const char *value, HBhvSensor *sensor);

	/*!
	Destructor for HBhvAction object.
  	*/
	virtual ~HBhvAction();

	
	/*!
	Sets target object for the action.
	\param target The descriptor for the target object for this action.  You can use HBhvTargetObject::CreateTargetString() to generate this string.
  	*/
	void SetTargetByPath(const char *targetpath);

	/*!
	This is XML read callback which parses XML data and then populates the member variables with the associated values.
  	*/
	static void *XMLCallback(HUtilityXMLTag *xt, bool open, void *m_pExtraData);


	/*!
	Performs action based on the action type.
   	*/
	void DoAction();

	/*!
	Writes XML data to a buffer.
  	*/
	void Serialize(HUtilityXMLGenerator *xmlgen);

protected:
   	char					m_Type[256];				/*!< The action type.*/
   	char					m_Value[MVO_BUFFER_SIZE];	/*!< The action value.*/
	HBhvSensor*				m_pSensor;					/*!< A pointer to the parent sensor.*/
	HBhvTargetObject*		m_pTarget;					/*!< The action's target object.*/


};




#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif
