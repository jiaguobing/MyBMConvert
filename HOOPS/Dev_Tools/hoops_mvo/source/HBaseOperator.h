// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

/*! \file HBaseOperator.h
	Defines datatypes associated with the HBaseOperator class  */

#ifndef _HBASEOPERATOR_H
#define _HBASEOPERATOR_H


#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"
#include "HBaseModel.h"
#include "HUtility.h"
#include "HEventListener.h"


/*! \def EPSILON  Used for various mathematical operations */
#define EPSILON		0.00001F

class HDB;
class HBaseModel;
class HBaseView;
class HEventInfo;
class HMouseListener;

/*! \enum HOperatorReturn
	Indicates the return value of an operator
*/
enum HOperatorReturn 
{
	HOP_OK,					//!< The event has been handled.
	HOP_NOT_HANDLED,		//!< The event has not been handled.
	HOP_CANCEL,				//!< Processing was aborted.
	HOP_READY				//!< Event handler is ready for processing.
};

//! The HBaseOperator class serves as a base for classes which handle user input and operate on the scene. 
/*!
  HBaseOperator provides support for mapping user input to interaction logic. A custom operator object implements
  the virtual functions that are provided in order to handle mouse button and keypress events as required.  For example,
  if the application needed to orbit the camera by clicking the mouse button down, dragging the mouse, and then releasing the mouse
  button, the derived operator would implement the OnLButtonDown, OnLButtonDownAndMove, and OnLButtonUp methods to provide logic
  to orbit the camera.  
  
  The HOOPS/GUI module provides support for mapping platform-specific UI events to the corresponding method of the current 
  operator associated with the view.  Alternately, the application could manually map a platform-specific UI event to the 
  matching method in the current operator without relying on the HOOPS/GUI toolkit.
*/
class MVO_API HBaseOperator  : public HMouseListener 
{
protected:
	/*! 
	  Indicates whether the operator has been started; this should be set to true as soon as any of the event
	  handling methods have been called
	*/
	bool		m_bOpStarted;		//!< Indicates whether the operator has already 'started'
	int			m_doRepeat;			//!< Integer indicating if the operator is repeatable .  This parameter has been deprecated.
	int			m_doCapture;		//!< Integer indicating whether to capture mouse on button down and release on button up.  This parameter has been deprecated.

	HPoint		m_ptFirst;			//!< Initial selection point
	HPoint		m_ptNew;			//!< Current selection point

	HBaseView	*m_pView;			//!< The HOOPS view corresponding to this operator

    bool        m_bMapTouchesToMouseEvents; //!< Indicates if touch events are mapped to mouse events

	/*! This method has been deprecated.
	\returns True if the given key is special and requires that we instead grab its owner */
	bool IsSpecialKey( HC_KEY key );

	HShadowMode				m_TempShadowMode;				//!< stores original shadow mode
  	HCutGeometryVisibility	m_TempCutGeometryVisibility; 	//!< stores original cut geometry visibility

public:
	int Repeat() { return m_doRepeat; }   //!< Returns whether the operation is repeatable.  This method has been deprecated.
	int Capture() { return m_doCapture; } //!< Returns whether we are currently in mouse capture mode.  See this class' constructor for a more detailed description of what this means.  This method has been deprecated.


	/*! 
	  Creates a new operator which is associated with the same view as the current operator. 
	  The user is responsible for deleting the newly created operator.
	  \return A pointer to an HBaseOperator object
	*/
	virtual HBaseOperator * Clone();                                  

	void SetFirstPoint(HPoint const& new_point) { m_ptFirst = new_point; } //!< Sets the initial selection point
	HPoint const GetFirstPoint() { return m_ptFirst; }         //!< Returns the initial selection point

	void SetNewPoint(HPoint const& new_point) { m_ptNew = new_point; }     //!< Sets the current selection point
	HPoint const GetNewPoint() { return m_ptNew; }             //!< Returns the current selection point

	HBaseView * GetView() { return m_pView; }                    //!< Returns a pointer to the view that this operator is associated with
	void SetView(HBaseView* new_view) { m_pView = new_view; }         //!< Sets the view that this operator is associated with

	virtual bool OperatorStarted() { return m_bOpStarted; }          //!< Operator has been started if true, and has not been started if false
	void SetOperatorStarted(bool val) { m_bOpStarted = val; }          //!< Operator has been started if true, and has not been started if false

    void SetMapTouchesToMouseEvents(bool val) { m_bMapTouchesToMouseEvents = val; } //!< Touch Events are mapped to mouse events if true, and are not if false
	/*!
	  Constructs an HBaseOperator object
	  \param view A pointer to an HBaseView object,
	  \param DoRepeat An integer denoting whether this is a repeatable operator.  This parameter has been deprecated.
	  \param DoCapture An integer denoting whether the mouse state should be captured, which means that all mouse events should be received after a mousedown, even if it leaves the window.
	  This parameter has been deprecated.
	*/
	HBaseOperator(HBaseView* view, int DoRepeat=0, int DoCapture=1);          
	virtual ~HBaseOperator();


	/*!  
	  This method automatically checks the state of the left, middle and right mouse buttons (by looking at the HEventInfo object) 
	  and calls out to the appropriate 'combination' event, such as OnLButtonDownAndMove, OnNoButtonDownAndMove, etc...   

	  This function will be automatically called from the HOOPS/GUI toolkit when a mouse move occurs, and should be manually 
	  called by platform-specific UI code if HOOPS/GUI is not being used.

	  The derived operator may overload this function to intercept and handle a mouse move event; however, it should be noted that
	  the automatic dispatching of combination events would then no longer occur.

	  \param hevent An HEventInfo object containing information about the current event
	  \return An #HOperatorReturn indicating the status of the event.
	*/
	virtual int OnMouseMove(HEventInfo &hevent);   
	

	/*! \return A character pointer denoting the name of operator. 
	*/
	virtual const char * GetName();  

	// event methods; these will be overloaded by derived classes

	/*!  
	  The derived operator should overload this function to intercept and handle the case where the left mouse button has been 
	  pressed down. It will be automatically called from the HOOPS/GUI toolkit when such an event occurs, and should be manually
	  called by platform-specific UI code if HOOPS/GUI is not being used.
	  \param hevent An HEventInfo object containing information about the current event
	  \return An #HOperatorReturn indicating the status of the event.
	*/
	virtual int OnLButtonDown(HEventInfo &hevent);

	/*!  
	  The derived operator should overload this function to intercept and handle the case where the left mouse button has been 
	  released. It will be automatically called from the HOOPS/GUI toolkit when such an event occurs, and should be manually
	  called by platform-specific UI code if HOOPS/GUI is not being used.
	  \param hevent An HEventInfo object containing information about the current event
	  \return An #HOperatorReturn indicating the status of the event.
	*/
	virtual int OnLButtonUp(HEventInfo &hevent);

	/*!  
	  The derived operator should overload this function to intercept and handle the case where the left mouse button has been 
	  double-clicked. It will be automatically called from the HOOPS/GUI toolkit when such an event occurs, and should be manually
	  called by platform-specific UI code if HOOPS/GUI is not being used.
	  \param hevent An HEventInfo object containing information about the current event
	  \return An #HOperatorReturn indicating the status of the event.
	*/
	virtual int OnLButtonDblClk(HEventInfo &hevent);


	/*!  
	  The derived operator should overload this function to intercept and handle the case where the left mouse button is  
	  pressed down and the mouse has moved. It will be automatically called from the HOOPS/GUI toolkit when such an event occurs,
	  and should be manually called by platform-specific UI code if HOOPS/GUI is not being used.
	  \param hevent An HEventInfo object containing information about the current event
	  \return An #HOperatorReturn indicating the status of the event.
	*/
	virtual int OnLButtonDownAndMove(HEventInfo &hevent);


	/*!  
	  The derived operator should overload this function to intercept and handle the case where the middle mouse button has been 
	  pressed down. It will be automatically called from the HOOPS/GUI toolkit when such an event occurs, and should be manually
	  called by platform-specific UI code if HOOPS/GUI is not being used.
	  \param hevent An HEventInfo object containing information about the current event
	  \return An #HOperatorReturn indicating the status of the event.
	*/
	virtual int OnMButtonDown(HEventInfo &hevent);

	/*!  
	  The derived operator should overload this function to intercept and handle the case where the middle mouse button has been 
	  released. It will be automatically called from the HOOPS/GUI toolkit when such an event occurs, and should be manually
	  called by platform-specific UI code if HOOPS/GUI is not being used.
	  \param hevent An HEventInfo object containing information about the current event
	  \return An #HOperatorReturn indicating the status of the event.
	*/
	virtual int OnMButtonUp(HEventInfo &hevent);

	/*!  
	  The derived operator should overload this function to intercept and handle the case where the middle mouse button has been 
	  double-clicked. It will be automatically called from the HOOPS/GUI toolkit when such an event occurs, and should be manually
	  called by platform-specific UI code if HOOPS/GUI is not being used.
	  \param hevent An HEventInfo object containing information about the current event
	  \return An #HOperatorReturn indicating the status of the event.
	*/
	virtual int OnMButtonDblClk(HEventInfo &hevent);

	/*!  
	  The derived operator should overload this function to intercept and handle the case where the middle mouse 
	  button is pressed down and the mouse has moved. It will be automatically called from the HOOPS/GUI toolkit when such an 
	  event occurs, and should be manually called by platform-specific UI code if HOOPS/GUI is not being used.
	  \param hevent An HEventInfo object containing information about the current event
	  \return An #HOperatorReturn indicating the status of the event.
	*/
	virtual int OnMButtonDownAndMove(HEventInfo &hevent);


	/*!  
	  The derived operator should overload this function to intercept and handle the case where the right mouse button has been 
	  pressed down. It will be automatically called from the HOOPS/GUI toolkit when such an event occurs, and should be manually
	  called by platform-specific UI code if HOOPS/GUI is not being used.
	  \param hevent An HEventInfo object containing information about the current event
	  \return An #HOperatorReturn indicating the status of the event.
	*/
	virtual int OnRButtonDown(HEventInfo &hevent);

	/*!  
	  The derived operator should overload this function to intercept and handle the case where the right mouse button has been 
	  released. It will be automatically called from the HOOPS/GUI toolkit when such an event occurs, and should be manually
	  called by platform-specific UI code if HOOPS/GUI is not being used.
	  \param hevent An HEventInfo object containing information about the current event
	  \return An #HOperatorReturn indicating the status of the event.
	*/
	virtual int OnRButtonUp(HEventInfo &hevent);

	/*!  
	  The derived operator should overload this function to intercept and handle the case where the right mouse button has been 
	  double-clicked. It will be automatically called from the HOOPS/GUI toolkit when such an event occurs, and should be manually
	  called by platform-specific UI code if HOOPS/GUI is not being used.
	  \param hevent An HEventInfo object containing information about the current event
	  \return An #HOperatorReturn indicating the status of the event.
	*/
	virtual int OnRButtonDblClk(HEventInfo &hevent);

	/*!  
	  The derived operator should overload this function to intercept and handle the case where the right mouse button is  
	  pressed down and the mouse has moved. It will be automatically called from the HOOPS/GUI toolkit when such an event occurs,
	  and should be manually called by platform-specific UI code if HOOPS/GUI is not being used.
	  \param hevent An HEventInfo object containing information about the current event
	  \return An #HOperatorReturn indicating the status of the event.
	*/
	virtual int OnRButtonDownAndMove(HEventInfo &hevent);

	/*!  
	  The derived operator should overload this function to intercept and handle the case where both the left and right mouse 
	  button are pressed down and the mouse has moved. It will be automatically called from the HOOPS/GUI toolkit when such an 
	  event occurs, and should be manually called by platform-specific UI code if HOOPS/GUI is not being used.
	  \param hevent An HEventInfo object containing information about the current event
	  \return An #HOperatorReturn indicating the status of the event.
	*/
	virtual int OnLRButtonDownAndMove(HEventInfo &hevent);

	/*!  
	  The derived operator should overload this function to intercept and handle the case where no mouse buttons are  
	  pressed down and the mouse has moved. It will be automatically called from the HOOPS/GUI toolkit when such an event occurs,
	  and should be manually called by platform-specific UI code if HOOPS/GUI is not being used.
	  \param hevent An HEventInfo object containing information about the current event
	  \return An #HOperatorReturn indicating the status of the event.
	*/
	virtual int OnNoButtonDownAndMove(HEventInfo &hevent);

	/*!  
	  The derived operator should overload this function to intercept and handle the case where a timer event has occurred. 
	  It will be automatically called from the HOOPS/GUI toolkit when such an event occurs, and should be manually called by 
	  platform-specific UI code if HOOPS/GUI is not being used.
	  \param hevent An HEventInfo object containing information about the current event
	  \return An #HOperatorReturn indicating the status of the event.
	*/
	virtual int OnTimer(HEventInfo &hevent);

	/*!  
	  The derived operator should overload this function to intercept and handle the case where a keyboard key has been 
	  pressed down. It will be automatically called from the HOOPS/GUI toolkit when such an event occurs, and should be manually
	  called by platform-specific UI code if HOOPS/GUI is not being used.
	  \param hevent An HEventInfo object containing information about the current event
	  \return An #HOperatorReturn indicating the status of the event.
	*/
	virtual int OnKeyDown(HEventInfo &hevent);

	/*!  
	  The derived operator should overload this function to intercept and handle the case where a keyboard key has been 
	  released. It will be automatically called from the HOOPS/GUI toolkit when such an event occurs, and should be manually
	  called by platform-specific UI code if HOOPS/GUI is not being used.
	  \param hevent An HEventInfo object containing information about the current event
	  \return An #HOperatorReturn indicating the status of the event.
	*/
	virtual int OnKeyUp(HEventInfo &hevent);

	/*!  
	  The derived operator should overload this function to intercept and handle mouse wheel movement. It 
	  will be automatically called from the HOOPS/GUI toolkit when such an event
	  occurs, and should be manually called by platform-specific UI code if HOOPS/GUI is not being used.
	  \param hevent An HEventInfo object containing information about the current event
	  \return An #HOperatorReturn indicating the status of the event.
	*/
	virtual int OnMouseWheel(HEventInfo &hevent);
    
    
	/*!  
	  The derived operator should overload this function to intercept and handle the case where a touch down event has occured
	  It will be automatically called from the HOOPS/GUI toolkit when such an 
	  event occurs, and should be manually called by platform-specific UI code if HOOPS/GUI is not being used.
	  \param hevent An HEventInfo object containing information about the current event
	  \return An #HOperatorReturn indicating the status of the event.
	*/
    virtual int OnTouchesDown(HEventInfo &hevent);
     	
/*!  
	  The derived operator should overload this function to intercept and handle the case where a touch move event has occured
	  It will be automatically called from the HOOPS/GUI toolkit when such an 
	  event occurs, and should be manually called by platform-specific UI code if HOOPS/GUI is not being used.
	  \param hevent An HEventInfo object containing information about the current event
	  \return An #HOperatorReturn indicating the status of the event.
	*/
        virtual int OnTouchesMove(HEventInfo &hevent);
    
/*!  
	  The derived operator should overload this function to intercept and handle the case where a touch up event has occured
	  It will be automatically called from the HOOPS/GUI toolkit when such an 
	  event occurs, and should be manually called by platform-specific UI code if HOOPS/GUI is not being used.
	  \param hevent An HEventInfo object containing information about the current event
	  \return An #HOperatorReturn indicating the status of the event.
	*/
        virtual int OnTouchesUp(HEventInfo &hevent);
  
    
    

};

/////////////////////////////////////////////////////////////////////////////

#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif
