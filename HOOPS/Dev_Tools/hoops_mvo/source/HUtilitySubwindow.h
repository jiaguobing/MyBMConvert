// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

/*! \file HUtilitySubwindow.h 
	Interface of the HSubwindow class
	*/

#ifndef _H_HUtilitySubwindow_H
#define _H_HUtilitySubwindow_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"
#include "HEventListener.h"
#include "HBaseOperator.h"
#include "HEventManager.h"
 

#define SUBWINDOW_NOTYPE 0 //!< The basic window type.
#define SUBWINDOW_MAGNIFIER 1//!< The subwindow acts as a magnifying glass.
#define SUBWINDOW_SNAPSHOT 2//!< The subwindow can take snapshots of the scene.
#define SUBWINDOW_POINTER 3//!< The subwindow acts as a view into the scene where the directiona and extent can be controlled by a pointer.


#define SW_ACTION_NOACTION 0	//!< There is no subwindow action.
#define SW_ACTION_MOVE 1		//!< Subwindow action is move.
#define SW_ACTION_SIZING 2		//!< Subwindow action is resizing.
#define SW_ACTION_MAGNIFYING 3	//!< Subwindow action is manifying.
#define SW_ACTION_POINTERMOVE 4	//!< Subwindow action is moving the pointer.
#define SW_ACTION_DRAGGING 5	//!< Subwindow action is dragging the scene inside the subwindow.

#define SW_BORDER 1				//!< Subwindow has borders.
#define SW_SIZE_FIELD 2			//!< Subwindow has a resize area.
#define SW_MOVE_FIELD 4			//!< Subwindow can be moved.
#define SW_DRAG_FIELD 8			//!< Subwindow allows the scene to be moved inside the window.
#define SW_DELETE_FIELD 16		//!< Subwindow has a close button.
#define SW_MAGNIFIER_FIELD 32	//!< Subwindow has a magnifier slider.
#define SW_BACKGROUND_BOX 64	//!< Subwindow has a shadow.
 

class HBaseView;


//! The HSubwindow class encapsulates interactive floating subwindows.
/*! 
    Subwindows can be used for things like displaying legend bars to different views of a particular scene. They can live
    outside of a particular model or can be stored inside an HSF file while retaining their functionality. Similar to 
    GUI windows, they can be dragged, resized and deleted.

*/class  MVO_API HSubwindow : public HObject, public HBaseOperator, public HUpdateListener, public HFitWorldListener, public HObjectManipulationListener
{

public:
 
  	/*!
	Constructs an HSubwindow object.
	  \param view A pointer to an HBaseView object.
	  \param DoRepeat An integer denoting whether this is a repeatable operator.  This parameter has been deprecated.
	  \param DoCapture An integer denoting whether the mouse state should be captured, which means that all mouse events should be received after a mousedown, even if it leaves the window.
	  This parameter has been deprecated.
 	*/
    HSubwindow (HBaseView* view, int DoRepeat=0, int DoCapture=1);
 	/*!
	Constructs an HSubwindow object.
 	*/
     HSubwindow ();
	 virtual ~HSubwindow();


	/*!
	This method inserts a new HSubwindow object into the currently open segment.
	\param left The minimum horizontal coordinate of the window.  
	\param right The maxium horizontal coordinate of the window.
	\param bottom The maximum vertical coordinate of the window.
	\param top The minium vertical coordinate of the window.
	\param type The subwindow type which can be #SUBWINDOW_NOTYPE, #SUBWINDOW_MAGNIFIER, #SUBWINDOW_SNAPSHOT or #SUBWINDOW_POINTER.
	\param appearance Bitfields describing the window characteristics which can be any combination of the following: 
	#SW_BORDER , #SW_SIZE_FIELD, #SW_MOVE_FIELD, #SW_DRAG_FIELD, #SW_DELETE_FIELD, #SW_MAGNIFIER_FIELD, #SW_BACKGROUND_BOX.
 	*/	    
    virtual void Insert(float left, float right, float bottom, float top, int type = SUBWINDOW_NOTYPE, long appearance = 
	SW_BORDER | SW_SIZE_FIELD | SW_MOVE_FIELD | SW_DRAG_FIELD | SW_DELETE_FIELD | SW_MAGNIFIER_FIELD | SW_BACKGROUND_BOX);

    /*!
	 This method resets the window parameters so the HSubwindow object is disassociated from the subwindow segment key in 
	 the HOOPS database.
 	*/	    
    virtual void Reset();


    /*!
	  \return True if the subwindow is represented by a segment in the HOOPS database or false if it isn't.
 	*/	    
    virtual bool IsActive();

    /*!
	  This method retrieves the window characteristics from the given segment key.
	  \param key The segment key to get the window characteristics from.
 	*/	    
    virtual void GetFromKey(HC_KEY key);

    /*!	This method determines if the given segment is a subwindow.
	  \param key  The segment key to check if it is a subwindow.
	  \return True if the given segment is a subwindow segment.
 	*/	    
    virtual bool IsSubwindow(HC_KEY key);

   /*!	
   Given a 2D position, this method finds if the subwindow is in that position.
   \param view A pointer to the HBaseView object.
	  \param pos The 2d window position.
	  \param pointerselected Returns true if window maginifier pointer is found.
	  \return True if the subwindow was found or false if it was not.
 	*/	  
    virtual bool FindSubwindow(HBaseView *view, HPoint pos, bool &pointerselected);

    /*!	
	 Given a segment key, this method looks under the key to find if a subwindow exists.
	  \param key The segment key to look under.
	  \return The key to the top level subwindow segment or INVALID_KEY if no subwindow segment was found. 	
	*/	  
    virtual HC_KEY FindSubwindowFromSegment(HC_KEY key);
    
	/*!	
	  This method gets the subwindow's position.
	  \param x Returns the horizontal position in window space.
	  \param y Returns the vertical position in window space.
	*/	          
    virtual void GetPosition(float &x,float &y);
 
	/*!	
	  The method sets the subwindow position.
	  \param x The horizontal position in window space.
	  \param y The vertical position in window space.
	*/	      
    virtual void SetPosition(float x,float y);
    

	/*!	
	  \return The top level subwindow key.	
	*/	      
    virtual HC_KEY GetWindowKey();
   
	/*!	
	  This method gets the subwindow size.
	  \param x Returns the width of the subwindow in window space.
	  \param y Returns the height of the subwindow in window space.
	*/	         
    virtual void GetSize(float &x,float &y);

	/*!	
	  This method sets the subwindow size.
	  \param deltax The width of the subwindow in window space.
	  \param deltay The height of the subwindow in window space.
	*/	    
    virtual void SetSize(float deltax, float deltay);


	/*!	
	  This method resets the subwindow camera to the current camera in the view.
 	  \param view A pointer to the HBaseView object.
 	*/	        
    virtual void MakeCameraSnapshot(HBaseView *view);
 
	/*!	
	  This method converts a 2D point in the subwindow to a 2D point in the view.
	  \param in_point The 2D window space coordinates of the subwindow to be converted.
	  \param out_point Returns the 2D coordinates converted to window space for the view.
 	*/	     
    virtual void ConvertFromSubwindowToView(HPoint &in_point, HPoint &out_point);

	/*!	
	 This method converts a 2D point in the View to a 2D point in the subwindow.
	  \param in_point The 2D coordinate in the View in window space.
	  \param out_point Returns the 2D coordinated in windowspace for the subwindow.
 	*/	         
    virtual void ConvertFromViewToSubwindow(HPoint const & in_point, HPoint & out_point);

   
   	/*!	
	  This method updates the subwindow type and appearance.
	  \param type The Subwindow Type which can be #SUBWINDOW_NOTYPE, #SUBWINDOW_MAGNIFIER, #SUBWINDOW_SNAPSHOT or #SUBWINDOW_POINTER.
	  \param appearance Bitfields describing the window characteristics which can be any combination of the following: 
	#SW_BORDER, #SW_SIZE_FIELD, #SW_MOVE_FIELD, #SW_DRAG_FIELD, #SW_DELETE_FIELD, #SW_MAGNIFIER_FIELD, #SW_BACKGROUND_BOX.
 	*/	    
	virtual void ChangeType(int type, long appearance = SW_BORDER | SW_SIZE_FIELD | SW_MOVE_FIELD | SW_DRAG_FIELD | SW_DELETE_FIELD | SW_MAGNIFIER_FIELD | SW_BACKGROUND_BOX);
  
	/*!	
	 This method gets the position of the subwindow pointer.
	  \param x Returns the horizontal position in window space.
	  \param y Returns the vertical position in window space.	
	*/	      
    virtual void GetPointerPosition(float &x,float &y);
	
    /*!	
	 This method set the position of subwindow pointer.
	  \param x The horizontal position in window space.
	  \param y The vertical position in window space.	
	*/	      
    virtual void SetPointerPosition(float x,float y);

    /*!	
	 This method set the size of the subwindow.
	\param left The left horizontal position.
	\param right The right horizontal position.
	\param bottom The bottom vertical position.
	\param top The top vertical position.
    */	          
    virtual void SetSize(float left, float right, float bottom, float top);

    /*!	
	 This method converts the given coordinate into the the <i>out_system</i> while still taking into account the 
	 transformation and rotation matrices along the subwindow segment path.
	\param view A pointer to the HBaseView object.	
	\param in_system The <i>from</i> coordinate space as described in #HC_Compute_Coordinates.
	\param from The coordinates to be converted.
	\param out_system The coordinate space that <i>from</i> will be converted into as described in #HC_Compute_Coordinates.
	\param to Returns the newly converted coordinate.
     */	          
    virtual void ComputeCoordinates(HBaseView *view, const char *in_system, HPoint &from, const char *out_system, HPoint &to);

    
    /*!	
	 This method drags the subwindow from point <i>p1</i> to point <i>p2</i>.
 	\param view A pointer to the HBaseView object.
	\param p1 The point to drag from.
	\param p2 The point to drag to.
    */	          
    virtual void Drag(HBaseView *view, HPoint p1, HPoint p2);
 
    /*!	
	This method synchronizes the view camera to the subwindow camera.
 	\param view A pointer to the HBaseView object.
     */	              
    virtual void SyncViewToWindow(HBaseView *view);

    /*!	
	This method retrieves the subwindow type.
	\return The subwindow type which can be #SUBWINDOW_NOTYPE, #SUBWINDOW_MAGNIFIER, #SUBWINDOW_SNAPSHOT or #SUBWINDOW_POINTER. 
	*/	              
    virtual int GetType() { return m_Type; }

    /*!	
	This method retrieves the subwindow key.
	\return The subwindow key.
     */	              
    virtual HC_KEY GetKey() { return m_OuterKey; }
    
	/*!  
  	  \return The name of the object which is 'subwindow'.
	*/
    virtual const char * GetName();  
	/*!  
  	  \return The name of the object which is 'subwindow'.
	*/
    static  const char * GetStaticName();  

	/*!  
	  This method handles the left button down event by determining what type of action the user is trying to take 
	  depending on the position of the mouse pointer and the characteristics of the subwindow.
 	  \param hevent An HEventInfo object containing the information about the current event.
	  \return An #HOperatorReturn indicating the status of the event.
	*/    
    virtual int OnLButtonDown (HEventInfo &hevent);
	/*!  
	  This method handles the left button up event.  If the action is #SW_ACTION_MOVE or #SW_ACTION_MAGNIFYING, this method 
	  creates a new movement keyframe.  If the action is SW_ACTION_SIZING, this method creates a new scale keyframe.
 	  \param hevent An HEventInfo object containing the information about the current event.
	  \return An #HOperatorReturn indicating the status of the event.
	*/    
    virtual int OnLButtonUp (HEventInfo &hevent);
    
	/*!  
	 This method handles the event that is generated when the camera position changes by updating the scene 
	 according to the new camera position.
 	  \param view A pointer to the HBaseView object.
	  \return An #HOperatorReturn indicating the status of the event.
 	*/    
    virtual int CameraChangedEvent(HBaseView *view);
	/*!  
	 This method handles the event generated before a call to FitWorld by setting the subwindow's visiblity to off and 
	 locking the visibility attribute.
 	  \param view A pointer to the HBaseView object.
	  \return An #HOperatorReturn indicating the status of the event.
 	*/        
    virtual int PreFitWorldEvent(HBaseView *view);

	/*!  
	 This handles the event generate by a call to FitWorld by unsetting the subwindow's visibility which was set to 
	 off during the PreFitWorldEvent and the associated attribute lock.
 	  \param view A pointer to the HBaseView object.
	  \return An #HOperatorReturn indicating the status of the event.
 	*/        
    virtual int PostFitWorldEvent(HBaseView *view);


	/*!  
	  This method handles the left button down and move event by updating the window characteristics depending on the current 
	  action identified in the OnLButtonDown event.  For instance, if the action was determined to be #SW_ACTION_MOVE, then this event 
	  would cause the subwindow to move to the new position indicated in <i>hevent</i>.  If the action was #SW_ACTION_SIZING, then 
	  the subwindow would be resized.
 	  \param hevent An HEventInfo object containing the information about the current event.
	  \return An #HOperatorReturn indicating the status of the event.
	*/    
    virtual int OnLButtonDownAndMove(HEventInfo &hevent);

	/*!  
	 This method handles set up handles event.  Note that this implementation does not create handles for the subwindow.
 	  \param view A pointer to the HBaseView object.
	  \param key The key of the current object.
	  \param complex Pass true to create complex handles.
	  \return An #HOperatorReturn indicating the status of the event.
 	*/
    virtual int SetupHandlesEvent(HBaseView *view, HC_KEY key, bool complex);

    /*!  
	  This method handles the object moved event by updating the subwindow position to the new object position <i>p</i>.
 	  \param view A pointer to the HBaseView object.
	  \param key The key of the current object.
	  \param p The new object position.
	  \return An #HOperatorReturn indicating the status of the event.
 	*/    
    virtual int ObjectMovedEvent(HBaseView *view, HC_KEY key, HPoint *p);
	/*!  
	  This method handles the object scaled event by updating the size of the subwindow with the given <i>scale</i> parameter.
 	  \param view A pointer to the HBaseView object.
	  \param key The key of the current object.
	  \param scale The object scaling.
	  \return An #HOperatorReturn indicating the status of the event.
 	*/
    virtual int ObjectScaledEvent(HBaseView *view, HC_KEY key, HPoint &scale);
	
protected:
    
	/*!
		This method retrieves the window parameters from the window segment key.
	*/	    
    void GetParameters(); 
	/*!
	This method determine if the mouse pointer is in the resizing area.
 			\param point The position of the mouse pointer.
			\return True if it is in the resize area or false if it is not or the appearance does not have the #SW_SIZE_FIELD flag.
	*/	    
    bool IsInSizeArea(HPoint &point);
	/*!
	This method determines if the mouse pointer is in magnifying slider area.
 			\param point The position of the mouse pointer.
			\return True if it is in the magnifying slider area or false if it is not or the appearance doesn't have the #SW_MAGNIFIER_FIELD.
	*/	    
    bool IsInMagnifyArea(HPoint &point);
	/*!
	This method copies the light associated with the view to the subwindow.
 			\param view A pointer to the HBaseView object.
 	*/	    
    void CopyViewLight(HBaseView *view);
 	/*!
		This method constructs a magnifier slider for the window if the appearance has the #SW_MAGNIFIER_FIELD.
 			\param pos The initial position of the slider.
 	*/	    
    void BuildMagnifyController(float pos);
    
    
   	/*!	
	This method changes the subwindow's manification based on the position of the magnification slider.
 			\param view A pointer to the HBaseView object.
			\param p The slider position.
	*/	           
    void ChangeMagnification(HBaseView *view, HPoint & p);
   	/*!	
	This method performs magnification on the window.
 			\param view A pointer to the HBaseView object.
 	*/	           
    void Magnify(HBaseView *view);

   	/*!	
	This method constructs the magnification/view pointer.
  	*/	           
    void ShowPointer();   
	/*!
	This method determines if the mouse position is in the "close window" area.
 	\param point The mouse position.
	\return True if it is in the "close window" area or false if it is not or the appearance does not have the flag #SW_DELETE_FIELD.
	*/	    
    bool IsInCloseArea(HPoint &point);
	/*!
	This method determines if the mouse position is in the mouse drag area.
 			\param point The mouse position.
			\return True if it is in the magnifying slider area or false if it is not or the appearance does not have the #SW_DRAG_FIELD flag. 
	*/	    
    bool IsInDragArea(HPoint &point);

	/*!
	This method constructs a window border area if the appearance has the #SW_BORDER flag.
 	*/	    
    void BuildBorder();
 	/*!
	This method determines if the mouse pointer is in the move area.
 			\param point The position of the mouse pointer.
			\return True if the mouse is in the move area or false if it isn't or the appearance does not have the #SW_MOVE_FIELD flag.
	*/	    
    bool IsInMoveArea(HPoint &point);
	/*!
	This method adjusts the subwindow's magnification.
 			\param view A pointer to the HBaseView object.
			\param magnification The magnification level.
	*/	    
    void ChangeMagnification(HBaseView *view, float magnification);
	/*!
	This method creates a subwindow movement keyframe.
 	*/	    
    void MakeKeyframe();
 
	/*!
	This method creates a subwindow scaling keyframe.
 	*/	    
  	void MakeScaleKeyframe();

    HC_KEY m_OuterKey;					/*!< The top level key for the window. */
    HC_KEY m_WindowKey;					/*!< The key to the "window" segment. */


    float m_Left;						/*!< The minimum horizontal value of the subwindow in window coordinates. */
	float m_Right;						/*!< The maximum horizontal value of the subwindow in window coordinates.*/
	float m_Bottom;						/*!< The maximum vertical value of the subwindow in window coordinates.*/
	float m_Top;						/*!< The minimum vertical value of the subwindow in window coordinates. */

    int m_Type;							/*!< The window type which can be #SUBWINDOW_NOTYPE, #SUBWINDOW_MAGNIFIER, #SUBWINDOW_SNAPSHOT or #SUBWINDOW_POINTER. */
    float m_Magnification;				/*!< The window magnification level. */
    float m_OldMagnification;			/*!< The previous magnification field. */
    long m_Appearance;					/*!< The appearance bitfield which can be any combination of the following: #SW_BORDER, #SW_SIZE_FIELD, #SW_MOVE_FIELD, #SW_DRAG_FIELD, #SW_DELETE_FIELD, #SW_MAGNIFIER_FIELD, #SW_BACKGROUND_BOX.*/
    HPoint m_Pointer;					/*!< The location of the window pointer. */
    int m_Action;						/*!< The current window action which can be #SW_ACTION_NOACTION, #SW_ACTION_MOVE, #SW_ACTION_SIZING, #SW_ACTION_MAGNIFYING, #SW_ACTION_POINTERMOVE or #SW_ACTION_DRAGGING. */
    HPoint m_ptLast;					/*!< The last mouse position. */
};


#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif









