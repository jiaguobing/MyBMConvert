// HOpCameraPan.h : interface of the HOpCameraPan class
// pans the camera of current view

#ifndef _HOpCameraWalk_H
#define _HOpCameraWalk_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"
#include "HBaseOperator.h"
#include "HObjects.h"
class HTClient;
class HImageItem;

//! The HOpCameraWalk allows the user to walk "inside" of the current scene with optional collision detection.
/*! 

	There are two different control schemes for camera manipulation which are keyboard or mouse based.

	<b>Keyboard based WASD Control:</b><br>
    This control scheme centers around the keyboard for movement and the mouse for changes in orientation.
	The "W" and "S" keys move the camera forward and backwards respectively while "A" and "D" allow for 
	left and right strafing/dollying. "Q" and "Z" move the camera up and down. Alternatively the up and down arrow 
	keys can be used for forward/backward, the left/right arrow keys for straffing and PageUp/PageDown 
	for up/down movement.
	Holding down the left mouse button and dragging the mouse across the screen changes the camera orientation
	Additionally collision detection can be activated programatically. To override collision detection while walking
	hold down the "SHIFT" key.


    <b>Mouse based Control:</b><br>
    By Pressing the left mouse button and dragging the mouse forward or backward the camera moves along 
    the current view vector. Dragging the mouse left and right rotates the camera around its current up-vector.
    Holding down the middle mouse button allows for up and down movement as well as straffing.
    The mouse wheel tilts the view around the horizontal camera axis

    The "Space" Key recenters the camera along the current ground plane.

   
*/
class MVO_API HOpCameraWalk : public HBaseOperator
{
public:

	/*!
	  Constructs an HOpCameraWalk object.
	  \param view A pointer to an HBaseView object.
	  \param DoRepeat An integer denoting whether this is a repeatable operator.  This parameter has been deprecated.
	  \param DoCapture An integer denoting whether the mouse state should be captured, which means that all mouse events 
	  should be received after a mousedown, even if it leaves the window.  This parameter has been deprecated.
	*/  	
     HOpCameraWalk(HBaseView* view, int DoRepeat=0, int DoCapture=1);
    virtual ~HOpCameraWalk();
    

    /*!
    \return A pointer to a character string denoting the name of the operator which is 'HOpCameraWalk'.
    */
    virtual const char * GetName();
    
	/*!  
	  The left button down handler defines the starting point for walk/rotation.
 	  \param hevent An HEventInfo object containing information about the current event
		\return An #HOperatorReturn indicating the status of the event.
	*/    
    virtual int OnLButtonDown (HEventInfo &hevent);

	/*!  
	  The left button down and move handler let the user drag the mouse forward and back to move the camera back and forth 
	  with a speed based on the distance to the initial mouse position. Moving the mouse horizontally rotates the camera.
 	  \param hevent An HEventInfo object containing information about the current event
		\return An #HOperatorReturn indicating the status of the event.
	*/    
    virtual int OnLButtonDownAndMove(HEventInfo &hevent);

    /*!  
	  Left button up handler ends the operation.
 	  \param hevent An HEventInfo object containing information about the current event
 		\return An #HOperatorReturn indicating the status of the event.
   */    
    virtual int OnLButtonUp (HEventInfo &hevent);
    
 	/*!
		SetLightFollowsCamera sets the lights to follow the camera or to stay in one place during orbit.  This has
		the effect of either lighting the scene during orbit or allowing the camera to move through shadow.  The default
		is off.
		\param follow Pass true to have the light follow the camera.
	*/
   void SetLightFollowsCamera(bool follow){m_LightFollowsCamera = follow;};
	/*! \return True if the light follows the camera or false if it doesn't. */
    bool GetLightFollowsCamera(){return m_LightFollowsCamera;};
 
    
    /*!  
	 The mouse wheel handler uses event information to tilt the camera around the horizontal axis.
 	  \param hevent An HEventInfo object containing information about the current event
		\return An #HOperatorReturn indicating the status of the event.
    */    
    virtual int OnMouseWheel(HEventInfo &hevent);


 	/*!  
	  The middle button down handler defines the starting point for straffing and up/down movement.
 	  \param hevent An HEventInfo object containing information about the current event
		\return An #HOperatorReturn indicating the status of the event.
	*/    
    virtual int OnMButtonDown(HEventInfo &hevent);

	/*!  
	  The middle button down and move handler let the user drag the mouse forward and backward to move the camera up/down
	  based on the distance to the initial mouse position. Moving the mouse horizontally strafes the camera left/right.
 	  \param hevent An HEventInfo object containing information about the current event
		\return An #HOperatorReturn indicating the status of the event.
	*/    
    virtual int OnMButtonDownAndMove(HEventInfo &hevent);

    /*!  
	  The middle button up handler uses event information to end the operation.
 	  \param hevent An HEventInfo object containing information about the current event
		\return An #HOperatorReturn indicating the status of the event.
    */    
    virtual int OnMButtonUp(HEventInfo &hevent); 	
       
 	/*! 
	  Creates a new operator which is a copy of the current operator.  The user is 
	  responsible for deleting the newly created operator.
	  \return A pointer to the newly created HOpCameraWalk object.
	*/
   virtual HBaseOperator * Clone();	
   
  
    /*!  
	 This is a mouse centric walking function called from timer callback.
 	  \param requested_time the time that the event had requested.
    	  \param actual_time the actual time    
    */    
    void	 OnWalk(float requested_time, float actual_time); 
    /*!  
	 This is the keyboard centric walking function called from timer callback.
 	  \param requested_time the time that the event had requested.
    	  \param actual_time The actual time that this method was called.
    */    
    void	 OnKeyWalk(float requested_time, float actual_time); 

	void	OnButtonWalk(char const * action, HButtonState buttonstate);

 
    /*!  
	 This is the walking function called from timer. 
 	  \param request_time the time that the event had requested.
    	  \param actual_time The actual time that this method was called.    
    	  \param user_data Optional user data.    
	  \return Success
    */    
    static bool WalkTimerClbk(float request_time, float actual_time, void * user_data);

    /*!  
		Use this method to indicate that the HBaseView object associated to this operator has been deleted.
     */    
    void ViewDeleted() { m_bViewDeleted = true; }
 
   /*!  
		Activates/deactivates keyboard centric walking.
		\param kw Pass true to activate walking via keyboard controls.
     */    
 	void SetKeyboardWalk(bool kw) { m_bKeyboardWalk = kw; }	
   /*!  
		Activates/Deactivates collision detection during walk.
		\param c Pass true to turn on collision detection.
     */    
	void SetCollision(bool c) { m_bCollision = c; }	
   /*!  
		Activates/Deactivates automatic adjustment of position to geometry "under" the camera.
		\param s Pass true to turn on snapping to the floor.
     */    
	void SetSnapToFloor(bool s) { m_bSnapToFloor = s; }	
   /*!  
		Sets distance from floor to camera position if floor snapping is on.
		\param ah The height of the camera from the floor.
     */    
	void SetAvatarHeight(float ah) { m_AvatarHeight = ah; 	m_bOldSnappedToGround = false; }
   /*!  
		Sets maximum "up" step height. While "walking" with collision detection, this value determines
		what maximum vertical distance increased allowed when moving from one object at a given elevation 
		to another one at a higher elevation.
		\param shu The maximum height increase of the camera when moving from one object to another.
     */    
	void SetStepHeightUp(float shu) { m_MaxUpGap = shu; }
   /*!  
		Sets maximum "down" step height. While "walking" with collision detection, this value determines
		the maximum vertical distance decrease when moving from one object at a given elevation to another one 
		at a lower elevation.
		\param shd The maximum height decrease of the camera when moving from one object to another.
     */    
	void SetStepHeightDown(float shd) { m_MaxDownGap = shd; }
	/*!  
 		\return The eye level height of camera in relation to the floor plane.
     */    
	float GetAvatarHeight() { return m_AvatarHeight; }
   /*!  
		Sets Movement Speed. This value hich is initially set to to a quarter of the diagonal of the world bounding box 
		represents the number of "units" the camera moves forward or strafes sideways per second.
		\param s The speed of the movement forward, back or across the scene.
     */    
	void SetSpeed(float s) { m_Speed = s; }
	float GetSpeed()	{ return m_Speed; }

	/*!
	*/
	static float CalculateExtents(HBaseView *view);  
	int OnKeyDown(HEventInfo &hevent);
	static int ImageAction(HImageRegion *ii,void *data, HButtonState keyup);

   /*!  
		Displays a navigation bar in the Window which users can then use to control
		the camera movement within the scene.
     */ 
	void ShowNavigationWindow(char *image_dir = 0);
	   /*!  
		Remove the navigation bar from the window.
     */ 
	void RemoveNavigationWindow();

	void FitControlsToCorner();

  protected:
	void SnapToGround();
	void AdjustCameraCollision(HCamera &oldcamera, bool noextrude = false);
	void AdjustHeight(HCamera &oldcamera);
 
    static bool signal_callback(int signal, void *signal_data, void *user_data);

 	void ReadjustPosition();	
	
	void SetupControls(char forward, char backward, char strafe_left, char strafe_right, char up, char down);

  private: 

	
	HCamera m_oldcamera;
	bool m_bOperatorCreated;
	void ResetPosition();  
	void Level();
	bool	m_LightFollowsCamera;
	HPoint camera,target, up;
 	float		m_time;
	unsigned	m_timer;
	float		m_max_world_extent;
	float		m_alpha;
	float		m_beta;
	float		m_rotate, m_move, m_tilt, m_updown, m_strafe;
	bool m_walkmode;
	int m_AxisMode;
	bool m_bChange;
	bool doOnce;
	bool m_bViewDeleted;
	HTClient*			m_pWalkTimer;
 	int m_cookie;													/*!< The key that needs to be passed to HBaseView::UnSetEventNotify() */
	bool m_bBlocked;
	bool m_bBlockedForward;
	bool m_bBlockedBackward;
	bool m_bSnappedToGround;
	bool m_bOldSnappedToGround;
	bool m_bKeyboardWalk;
	bool m_bCollision;
	bool m_bSnapToFloor;

	float m_MaxUpGap;
	float m_MaxDownGap;
	float m_AvatarDiameter;
	float m_AvatarHeight;
	float m_Speed;
	float m_totalrotate;
	float m_totalrotateold;
	char  m_c_forward;
	char  m_c_backward;
	char  m_c_strafe_left;
	char  m_c_strafe_right;
	char  m_c_up;
	char  m_c_down;
	bool  m_bNoKeyMove;
 	bool  m_bNavigationWindowActive;
};


#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif



