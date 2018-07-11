// HOpCameraWalk.cpp : implementation of the HSOpCameraPan class
//

  
#include <math.h>
#include <string.h>
#include <stdio.h>

#include "HBaseModel.h"
#include "HBaseView.h"
#include "HOpCameraWalk.h"
#include "HConstantFrameRate.h"
#include "HTools.h"
#include "HEventInfo.h"
#include "HEventManager.h"
#include "HUtility.h"
#include "HObjects.h"
#include "HBhvBehaviorManager.h"

#include "hc.h"
#include "HTManager.h"
#include "HDB.h"

/////////////////////////////////////////////////////////////////////////////
// HOpCameraWalk
//
// Operator for walking in a scene.  
// 
//
// left button down - initiates free viewing in all directions with mouse movement
//
// Left + Right Mouse button - movement in view direction (reversed by pressing shift)
// 


// timer callback to say walk further
bool HOpCameraWalk::WalkTimerClbk(float request_time, float actual_time, void * user_data)
{

	HOpCameraWalk* op = (HOpCameraWalk*) user_data;
	op->OnWalk(request_time, actual_time);
	return true;
}

HOpCameraWalk::HOpCameraWalk(HBaseView* view, int DoRepeat,int DoCapture) : HBaseOperator(view, DoRepeat, DoCapture)
{
 	

	m_bOperatorCreated = true;
	m_bNavigationWindowActive = false;
	m_oldcamera.GetFromView(view);

	
	m_RequestedPriority = HLISTENER_PRIORITY_HIGH + 10000;
	view->SetProjMode(ProjPerspective);
    
	HC_Begin_Shell_Selection();
    
    m_AxisMode = HUtility::CalculateMajorAxis(view);
    m_time = HUtility::GetTime();
    

 
    m_max_world_extent = 0.0f;
    
    m_rotate = 0;
    m_move = 0;
    m_tilt = 0;
    m_updown = 0;
    m_strafe = 0;

	m_c_forward = 'W';
	m_c_backward = 'S';
	m_c_strafe_left = 'A';
	m_c_strafe_right = 'D';
	m_c_up = 'Q';
	m_c_down = 'Z';
    
    HPoint t,p,delta;
    m_bNoKeyMove = false;

    HC_Open_Segment_By_Key(GetView()->GetSceneKey());
    HC_Show_Net_Camera_Target (&t.x, &t.y, &t.z);
    HC_Show_Net_Camera_Position (&p.x, &p.y, &p.z);
    HC_Close_Segment();
    delta.Set(t.x- p.x, t.y - p.y, t.z - p.z);
    float l1 = (float)HC_Compute_Vector_Length(&delta);
    if (m_AxisMode == 2)
	delta.Set(delta.x, delta.y, 0);
    else if (m_AxisMode == 1)
	delta.Set(0, delta.y, delta.z);
    else 
	delta.Set(delta.x, 0, delta.z);

    float l2 = (float)HC_Compute_Vector_Length(&delta);
    float deg = (float)H_ACOS(l2/l1) * (180.0f/3.1415927f);
    m_max_world_extent = CalculateExtents(GetView());
	m_MaxUpGap =  m_max_world_extent/200.0f;
	m_MaxDownGap =  m_max_world_extent/10.0f;
	m_AvatarDiameter = m_max_world_extent/700.0f;
	m_AvatarHeight = m_max_world_extent/60.0f;

    ResetPosition();
    
    switch (m_AxisMode)
    {
	case 2:
 	 p.z > t.z ? m_tilt = -deg : m_tilt = deg; 
	 break;

	case 1:
 	 p.x > t.x ? m_tilt = -deg : m_tilt = deg; 
	 break;
	case 0:
 	 p.y > t.y ? m_tilt = -deg : m_tilt = deg; 
	 break;
    }

    m_bViewDeleted = false;
    doOnce = false;                  
  
    m_cookie = GetView()->SetSignalNotify( HSignalDelete, HOpCameraWalk::signal_callback, this );
    
	m_bBlocked = false;
	m_bBlockedForward = false;
	m_bBlockedBackward = false;
	m_bSnappedToGround = false;
	m_bOldSnappedToGround = false;
	m_bKeyboardWalk = false;
	m_bCollision = true;
	m_bSnapToFloor = true;

	m_totalrotate = 0;
	m_totalrotateold = 0;
	m_Speed = m_max_world_extent / 2.0f;
	m_pWalkTimer = new HTClient( 0.005f, HTCS_PeriodicSkip, WalkTimerClbk, this );
	m_bChange = true;
    HDB::GetHTManager()->RegisterClient( m_pWalkTimer );
 //    OnWalk(0,0);

 
}
void HOpCameraWalk::ShowNavigationWindow(char *imagedirectory)
{
 	char walk_normal[4096];
	char walk_selected[4096];
	char walk_selectionarea[4096];
	char prefix[4096];

	if (imagedirectory)
	{
		strcpy(prefix, imagedirectory);
		strcat(prefix,"\\");
	}
	else
		strcpy(prefix,"");


	sprintf(walk_normal, "%s%s", prefix, "walk-normal.hmf");
	sprintf(walk_selected, "%s%s", prefix, "walk-selected.hmf");
	sprintf(walk_selectionarea, "%s%s", prefix, "walk-selectionarea.hmf");

 	if (!m_bNavigationWindowActive)
	{	
			FILE *fres = fopen(walk_normal,"r");
			if (fres)
			{
				fclose(fres);				
				HImagePanelManagerObject *ipo =   (HImagePanelManagerObject *) GetView()->GetHObjectManager()->GetHObject("imagepanel");
				if (!ipo)
				{				
					ipo = new HImagePanelManagerObject();
					GetView()->GetHObjectManager()->AddHObject(ipo);
				}
	
				HC_Open_Segment_By_Key(GetView()->GetViewKey());
				float xpos,ypos;
				HUtility::CalculateCornerImagePosition(GetView(), 274,130,BottomRight,xpos,ypos);
				HImagePanel * ipanel2 = new HImagePanel(GetView(),-1,274,130,xpos,ypos,"Walk Panel");
				HC_Close_Segment();
				
				ipanel2->SetImagesFromFile(walk_normal,walk_selected,
					walk_selectionarea);
				
				ipanel2->AddImageRegion(1, "forward_arrow");
				ipanel2->AddImageRegion(0, "left_arrow");
				ipanel2->AddImageRegion(2, "right_arrow");
				ipanel2->AddImageRegion(3, "down_arrow");
				ipanel2->AddImageRegion(4, "up_arrow");
				ipanel2->AddImageRegion(5, "right_strafe_arrow");
				ipanel2->AddImageRegion(7, "left_strafe_arrow");
				ipanel2->AddImageRegion(6, "back_arrow");
				ipanel2->AddImageRegion(8, "tilt_up");
				ipanel2->AddImageRegion(9, "tilt_down");
				ipanel2->AddImageRegion(10, "quit");
				ipo->AddImagePanel(ipanel2);
				ipanel2->SetImageAction(ImageAction, this);		
				m_bNavigationWindowActive = true;	
			}
		
	}
 
	GetView()->Update();		
}

void HOpCameraWalk::RemoveNavigationWindow()
{
	if (m_bNavigationWindowActive)
	{
		HImagePanelManagerObject *ipo =   (HImagePanelManagerObject *) GetView()->GetHObjectManager()->GetHObject("imagepanel");
		HImagePanel *ip = ipo->GetImagePanelByName("Walk Panel");
		if (ip)
		{
			ip->ZoomOut();
			ipo->DeleteImagePanel("Walk Panel", true);
		}
		m_bNavigationWindowActive = false;
	}
	GetView()->Update();

}

void HOpCameraWalk::FitControlsToCorner()
{
	float xpos,ypos;

	HC_Open_Segment_By_Key(GetView()->GetViewKey());		
		HUtility::CalculateCornerImagePosition(GetView(), 274,130,BottomRight,xpos,ypos);	
	HC_Close_Segment();

	HImagePanelManagerObject *ipo =   (HImagePanelManagerObject *) GetView()->GetHObjectManager()->GetHObject("imagepanel");
	HImagePanel *ip = ipo->GetImagePanelByName("Walk Panel");

	ip->SetPosition(xpos,ypos,0);
}

int HOpCameraWalk::ImageAction(HImageRegion *ii,void *data, HButtonState buttonstate)
{

	int res = 1;
	HOpCameraWalk *wop = (HOpCameraWalk *)data;
	if (strcmp(ii->GetName(), "quit") != 0)
		wop->OnButtonWalk(ii->GetName(), buttonstate);
	else
	{	
		wop->m_bNavigationWindowActive = false;
		res = 0;
	}
	return res;
}

//the timer function kills the platform independency of course...
void HOpCameraWalk::OnButtonWalk(char const *action, HButtonState buttonstate)
{
 


	HCamera newcamera;
	newcamera.GetFromView(GetView());
	if (newcamera.CameraDifferent(m_oldcamera) && !m_pView->GetModel()->GetBhvBehaviorManager()->IsPlaying())
		ReadjustPosition();	
 
	static HCamera lastCamera;
	m_bSnappedToGround = false;
	static bool first = true;
	double time_diff;	
  	if (1 || m_bChange)
	{
	if (first)
	{
		GetView()->GetCamera(&lastCamera);
		m_time = HUtility::GetTime();
		first = false;
	}
	time_diff = HUtility::GetTime() - m_time;
	// bump up "time diff" in case the button just went down;
	// we don't want to wait to start moving the camera.
	if (time_diff < .01f)
		time_diff = .01f;

	m_time = HUtility::GetTime();


	HCamera oldcamera;
	GetView()->GetCamera(&oldcamera);
	HC_Open_Segment_By_Key(GetView()->GetSceneKey());
 	ResetPosition();
  	 

	bool b_forward = (strcmp(action, "forward_arrow") == 0 && buttonstate == ButtonPressed);
	bool b_backward = (strcmp(action, "back_arrow") == 0 && buttonstate == ButtonPressed);
	bool b_left = (strcmp(action, "left_strafe_arrow") == 0 && buttonstate == ButtonPressed);
	bool r_right = (strcmp(action, "right_strafe_arrow") == 0 && buttonstate == ButtonPressed);
 	bool b_down = (strcmp(action, "down_arrow") == 0 && buttonstate == ButtonPressed);;
 	bool b_up = (strcmp(action, "up_arrow") == 0 && buttonstate == ButtonPressed);
#if 0
 	if (f & MVO_LEFT_SHIFT)
	{
		m_bBlockedForward = false;
		m_bBlockedBackward = false;
	}
#endif
	if (!b_forward && !b_backward && !b_left && !r_right && !b_up && !b_down)
	{
		 if (!m_bNoKeyMove)
			GetView()->PrepareForCameraChange();
		 m_bNoKeyMove = true;
	}
	else
		m_bNoKeyMove = false;


	if (!m_bBlockedForward && b_forward)
	{	
 		HC_Dolly_Camera(0.0,0.0,m_Speed*time_diff);
		ResetPosition();		 		 
	}
 
	if (!m_bBlockedBackward && b_backward)
	{	
 		HC_Dolly_Camera(0.0,0.0,-m_Speed*time_diff);
		ResetPosition();			 
	}
 
 	if (b_left)
	{	 
		if( GetView()->GetHandedness() == HandednessRight)	
			HC_Dolly_Camera(m_Speed*time_diff, 0.0, 0.0);
		else
			HC_Dolly_Camera(-m_Speed*time_diff, 0.0, 0.0);
		ResetPosition();			 

	}
 	if (r_right)

	{ 		
		if( GetView()->GetHandedness() == HandednessRight)	
			HC_Dolly_Camera(-m_Speed*time_diff, 0.0, 0.0);
		else
			HC_Dolly_Camera(m_Speed*time_diff, 0.0, 0.0);
		ResetPosition();		
	 	
	}


	
 	if (b_up)
	{		
		HC_Dolly_Camera(0.0, -m_Speed*time_diff,  0.0);
		ResetPosition();			
	 
	

	}
 	if (b_down)
	{	
 		HC_Dolly_Camera(0.0, m_Speed*time_diff, 0.0);
		ResetPosition();		
	 
		
	}

	
	if ((!m_bBlockedForward && b_forward) || (!m_bBlockedBackward &&  b_backward) || b_left || r_right || b_up || b_down)
	{
		m_bChange = true;
//		SnapToGround();
	}
	
	if ((m_bBlockedBackward && b_forward) || b_left || r_right || b_up || b_down)
		m_bBlockedBackward = false;
	if ((m_bBlockedForward && b_backward) || b_left || r_right || b_up || b_down)
		m_bBlockedForward = false;

	if (fabs (m_totalrotate - m_totalrotateold)>20)
	{
		m_bBlockedForward = false;
		m_bBlockedBackward = false;
		m_totalrotate = m_totalrotateold;
	}

 		
  	AdjustCameraCollision(oldcamera);

	if (!m_bBlocked)	
		SnapToGround();

	AdjustCameraCollision(oldcamera);

	AdjustCameraCollision(oldcamera,true);
	
	AdjustHeight(oldcamera);
	ResetPosition();	
 

	if (strcmp(action, "right_arrow" ) == 0)
	{
		m_bChange = true;
  		HC_Pan_Camera(-1,0);
		m_totalrotate-=1;
	}
	if (strcmp(action, "left_arrow" ) == 0)
	{
		m_bChange = true;
  		HC_Pan_Camera(1,0);
		m_totalrotate+= 1;
	}


	if (!m_bBlockedForward && !m_bBlockedBackward)
		m_totalrotate = m_totalrotateold;

	if (strcmp(action, "tilt_up" ) == 0)
	{
		m_tilt+=0.5;
		m_bChange = true;
	}
	if (strcmp(action, "tilt_down" ) == 0)
	{
		m_tilt-=0.5;
		m_bChange = true;
	}

 	HC_Pan_Camera(0.0f,m_tilt);
	m_rotate = 0;
	if (m_bBlocked &&  b_forward)
	{
		m_bBlockedForward = true;
		m_totalrotateold = m_totalrotate;
	}
 	if (m_bBlocked && b_backward)
	{
		m_bBlockedBackward = true;
		m_totalrotateold = m_totalrotate;
	}

	 
	m_bBlocked = false; 	
 	HC_Close_Segment();
	if (m_bChange)
	{
	    m_bChange = false;
	    GetView()->CameraPositionChanged(true);
	    GetView()->GetConstantFrameRateObject()->SetActivityType(GeneralActivity);


  	    GetView()->Update();
	}
	}
	m_bOldSnappedToGround = m_bSnappedToGround;
 	m_oldcamera.GetFromView(GetView());
}




HOpCameraWalk::~HOpCameraWalk()
{
		// get rid of the timer
	if( m_pWalkTimer )
	{
		HDB::GetHTManager()->UnRegisterClient( m_pWalkTimer );
		delete m_pWalkTimer;
		m_pWalkTimer = 0;
	}

	if (!m_bViewDeleted)	
	{
	    GetView()->UnSetSignalNotify( m_cookie );
	    HUtility::ReadjustTarget(GetView());	   
	}

	HC_End_Shell_Selection();
	RemoveNavigationWindow();
 
}


HBaseOperator * HOpCameraWalk::Clone()
{
	return new HOpCameraWalk(GetView());
}


 
const char * HOpCameraWalk::GetName() { return "HOpCameraWalk"; }


float HOpCameraWalk::CalculateExtents(HBaseView *view)
{

	float extents = 0;
	HPoint camera;
	HC_Open_Segment_By_Key (view->GetSceneKey());		
 		HC_Show_Net_Camera_Position (&camera.x, &camera.y, &camera.z);
 	HC_Close_Segment();

	HPoint min_world_corner;
	HPoint max_world_corner;
	HPoint world_extents;

	//we determine the size of the bounding box to make an educated guess on the step interval for
	//the forward movement

	// set some defaults in case no geometry is in the scene
	min_world_corner.Set(0.1f, 0.1f, 0.1f);
	max_world_corner.Set(-0.1f, -0.1f, -0.1f);
	
	// set the max world extent for use when moving the camera
	extents = 0.0f;

	view->GetSceneExtents(min_world_corner, max_world_corner);
 
	//we also take the distance to the scene into account
	HPoint middle;
	middle.x = (min_world_corner.x + max_world_corner.x)/2;
	middle.y = (min_world_corner.y + max_world_corner.y)/2;
	middle.z = (min_world_corner.z + max_world_corner.z)/2;

	world_extents.x = max_world_corner.x - min_world_corner.x;
 	if (world_extents.x > extents)
		extents = world_extents.x;

	world_extents.y = max_world_corner.y - min_world_corner.y;
 	if (world_extents.y > extents)
		extents = world_extents.y;

	world_extents.z = max_world_corner.z - min_world_corner.z;
 	if (world_extents.z > extents)
		extents = world_extents.z;
	return extents;

}
 
int HOpCameraWalk::OnLButtonDown(HEventInfo &event)
{
    

    if (!OperatorStarted()) 
    {
		GetView()->PrepareForCameraChange();

		if (!doOnce)
		{
			doOnce = true;
			GetView()->GetEventManager()->UnRegisterHandler((HMouseListener *)this, HMouseListener::GetType());
			GetView()->GetEventManager()->RegisterHandler((HMouseListener *)this, HMouseListener::GetType(), HLISTENER_PRIORITY_HIGH + 10);	    
		}
		
 		if (m_max_world_extent == 0.0f)
		{
			// world extents do not exist, don't bother moving the viewpoint.
			return -1;
		}
    }
	
	SetOperatorStarted(true);
	SetNewPoint(event.GetMouseWindowPos()); 
	SetFirstPoint(GetNewPoint());
 
	return HLISTENER_CONSUME_EVENT;
}

 

 
int HOpCameraWalk::OnLButtonDownAndMove(HEventInfo &event)
{
	if (!OperatorStarted()) return HBaseOperator::OnLButtonDownAndMove(event);

    SetNewPoint(event.GetMouseWindowPos());  
										
    HPoint delta(GetNewPoint() - GetFirstPoint());
 
    if (fabs(delta.x) < 0.00005f)
		m_rotate = 0;
    else
		m_rotate=delta.x;

    if (fabs(delta.y) < 0.00005f)
		m_move = 0;
    else
		m_move=(delta.y);

	if (m_bKeyboardWalk)
	{	
		m_tilt+= delta.y*100.0f;
		if (m_tilt < 0 && m_tilt < -89)
			m_tilt = -89;
		if (m_tilt > 0 && m_tilt> 89)
			m_tilt = 89;
		m_bChange = true;
		SetFirstPoint(GetNewPoint());
	}

//	m_bBlockedBackward = false;
//	m_bBlockedForward = false;
     return HOP_OK;

}





int HOpCameraWalk::OnLButtonUp(HEventInfo &event)
{
    if(!OperatorStarted()) return HBaseOperator::OnLButtonUp(event);
    GetView()->CameraPositionChanged(true, GetView()->GetSmoothTransition());
    SetOperatorStarted(false);
    m_rotate = 0;
    m_move = 0;
    return HOP_READY;
}

//the timer function kills the platform independency of course...
void HOpCameraWalk::OnWalk(float requested_time, float actual_time)
{
	int f = 0;
 
	if (m_pView->GetModel()->GetBhvBehaviorManager()->GetCameraUpdated() && m_pView->GetModel()->GetBhvBehaviorManager()->IsPlaying())
		return;
 	GetView()->GetKeyState(65,f);
	if (m_bKeyboardWalk)
	{	
		OnKeyWalk(requested_time, actual_time);
		return;
	}
	HCamera newcamera;
	if (m_bOperatorCreated)
	{
		newcamera = m_oldcamera;
		m_bOperatorCreated = false;
	}
	else
		newcamera.GetFromView(GetView());

	if (newcamera.CameraDifferentByAngle(m_oldcamera))
		ReadjustPosition();

	static HCamera lastCamera;
	m_bSnappedToGround = false;
	static bool first = true;
	double time_diff;	
  	if (OperatorStarted() || m_bChange || newcamera.CameraDifferentByAngle(m_oldcamera))
	{
		if (first)
		{
			GetView()->GetCamera(&lastCamera);
			m_time = HUtility::GetTime();
			first = false;
		}
		time_diff = HUtility::GetTime() - m_time;
		// bump up "time diff" in case the button just went down;
		// we don't want to wait to start moving the camera.
		if (time_diff < .01f)
			time_diff = .01f;

		m_time = HUtility::GetTime();


		HCamera oldcamera;
		GetView()->GetCamera(&oldcamera);
		HC_Open_Segment_By_Key(GetView()->GetSceneKey());
 		ResetPosition();
  		AdjustCameraCollision(oldcamera,true);
		if (m_bBlocked)
		{	
			HC_Open_Segment_By_Key(GetView()->GetSceneKey());
			HC_Set_Camera (&lastCamera.position, &lastCamera.target, &lastCamera.up_vector, lastCamera.field_width, oldcamera.field_height, lastCamera.projection);
			HC_Close_Segment();

			m_bBlocked = false;
		}

	 		
		if (!(m_bBlockedForward && m_move>0) &&  !(m_bBlockedBackward && m_move<0))
		{

			if (fabs(m_move) > 0 && OperatorStarted())
			{
				if (f & MVO_LEFT_SHIFT)
					HC_Dolly_Camera(0.0,0.0,(m_move*10.0f * time_diff * m_Speed));
				if (f & MVO_CONTROL)
					HC_Dolly_Camera(0.0,0.0,(m_move/10.0f * time_diff * m_Speed));
				else
					HC_Dolly_Camera(0.0,0.0,(m_move*time_diff * m_Speed));
			}

			ResetPosition();	
			SnapToGround();

		}
		if ((m_bBlockedBackward && m_move>0) || m_updown || m_strafe)
			m_bBlockedBackward = false;
		if ((m_bBlockedForward && m_move<0) || m_updown || m_strafe)
			m_bBlockedForward = false;


		if (fabs(m_updown) > 0 && OperatorStarted())
		HC_Dolly_Camera(0.0,(m_updown*time_diff * m_Speed), 0.0);

		if (fabs(m_strafe) > 0 && OperatorStarted())
		HC_Dolly_Camera((-m_strafe*time_diff * m_Speed), 0.0, 0.0);


  		AdjustCameraCollision(oldcamera);
	  	
		
		AdjustHeight(oldcamera);
		ResetPosition();	
 		HC_Pan_Camera(0.0f,m_tilt);

		if (OperatorStarted())
		{
			if (f & MVO_CONTROL)
  				HC_Pan_Camera(-m_rotate*2.5f,0);
			else
  				HC_Pan_Camera(-m_rotate*3.5f,0);
		}
		
		if (m_bBlocked && m_move >0)
			m_bBlockedForward = true;
		if (m_bBlocked && m_move <0)
			m_bBlockedBackward = true;

		m_bBlocked = false;
 			HC_Show_Net_Camera_Target (&target.x, &target.y, &target.z);
			HC_Show_Net_Camera_Position (&camera.x, &camera.y, &camera.z);
			HC_Show_Net_Camera_Up_Vector (&up.x, &up.y, &up.z);
 		HC_Close_Segment();
		if (OperatorStarted() || m_bChange || newcamera.CameraDifferentByAngle(m_oldcamera))
		{
			m_bChange = false;
			HCamera newcamera2;
			newcamera2.GetFromView(GetView());
			if (newcamera2.CameraDifferentByAngle(newcamera) || OperatorStarted())
				GetView()->CameraPositionChanged(true);
			GetView()->GetConstantFrameRateObject()->SetActivityType(GeneralActivity);

  			GetView()->Update();
		}
	}
	m_bOldSnappedToGround = m_bSnappedToGround;
	GetView()->GetCamera(&lastCamera);
 	m_oldcamera.GetFromView(GetView());
}


//the timer function kills the platform independency of course...
void HOpCameraWalk::OnKeyWalk(float requested_time, float actual_time)
{
	UNREFERENCED(requested_time);
	UNREFERENCED(actual_time);

	HCamera newcamera;
	newcamera.GetFromView(GetView());
	if (newcamera.CameraDifferent(m_oldcamera))
		ReadjustPosition();

 
 	
 
	static HCamera lastCamera;
	m_bSnappedToGround = false;
	static bool first = true;
	double time_diff;	
  	if (1 || m_bChange)
	{
	if (first)
	{
		GetView()->GetCamera(&lastCamera);
		m_time = HUtility::GetTime();
		first = false;
	}
	time_diff = HUtility::GetTime() - m_time;
	// bump up "time diff" in case the button just went down;
	// we don't want to wait to start moving the camera.
	if (time_diff < .01f)
		time_diff = .01f;

	m_time = HUtility::GetTime();


	HCamera oldcamera;
	GetView()->GetCamera(&oldcamera);
	HC_Open_Segment_By_Key(GetView()->GetSceneKey());
 	ResetPosition();
  	 

 
	int f;
	 
	bool b_forward = GetView()->GetKeyState(m_c_forward,f) || GetView()->GetKeyState(38,f);
	bool b_backward = GetView()->GetKeyState(m_c_backward,f) || GetView()->GetKeyState(40,f);
	bool b_left = GetView()->GetKeyState(m_c_strafe_left,f) || GetView()->GetKeyState(37,f);
	bool r_right = GetView()->GetKeyState(m_c_strafe_right,f) || GetView()->GetKeyState(39,f);
	bool b_down = GetView()->GetKeyState(m_c_up,f) || GetView()->GetKeyState(33,f);;
	bool b_up = GetView()->GetKeyState(m_c_down,f) || GetView()->GetKeyState(34,f);;
	if (f & MVO_LEFT_SHIFT)
	{
		m_bBlockedForward = false;
		m_bBlockedBackward = false;
	}
 
	if (!b_forward && !b_backward && !b_left && !r_right && !b_up && !b_down)
	{
		 if (!m_bNoKeyMove)
			GetView()->PrepareForCameraChange();
		 m_bNoKeyMove = true;
	}
	else
		m_bNoKeyMove = false;


	if (!m_bBlockedForward && b_forward)
	{	
 		HC_Dolly_Camera(0.0,0.0,m_Speed*time_diff);
		ResetPosition();		 		 
	}
 
	if (!m_bBlockedBackward && b_backward)
	{	
 		HC_Dolly_Camera(0.0,0.0,-m_Speed*time_diff);
		ResetPosition();			 
	}
 
 	if (b_left)
	{	 
		if( GetView()->GetHandedness() == HandednessRight)	
			HC_Dolly_Camera(m_Speed*time_diff, 0.0, 0.0);
		else
			HC_Dolly_Camera(-m_Speed*time_diff, 0.0, 0.0);
		ResetPosition();			 

	}
 	if (r_right)

	{ 		
		if( GetView()->GetHandedness() == HandednessRight)	
			HC_Dolly_Camera(-m_Speed*time_diff, 0.0, 0.0);
		else
			HC_Dolly_Camera(m_Speed*time_diff, 0.0, 0.0);
		ResetPosition();		
	 	
	}


	
 	if (b_up)
	{		
		HC_Dolly_Camera(0.0, -m_Speed*time_diff,  0.0);
		ResetPosition();			
	 
	

	}
 	if (b_down)
	{	
 		HC_Dolly_Camera(0.0, m_Speed*time_diff, 0.0);
		ResetPosition();		
	 
		
	}

	
	if ((!m_bBlockedForward && b_forward) || (!m_bBlockedBackward &&  b_backward) || b_left || r_right || b_up || b_down)
	{
		m_bChange = true;
//		SnapToGround();
	}
	
	if ((m_bBlockedBackward && b_forward) || b_left || r_right || b_up || b_down)
		m_bBlockedBackward = false;
	if ((m_bBlockedForward && b_backward) || b_left || r_right || b_up || b_down)
		m_bBlockedForward = false;

	if (fabs (m_totalrotate - m_totalrotateold)>20)
	{
		m_bBlockedForward = false;
		m_bBlockedBackward = false;
		m_totalrotate = m_totalrotateold;
	}

 		
  	AdjustCameraCollision(oldcamera);

	if (!m_bBlocked)	
		SnapToGround();

	AdjustCameraCollision(oldcamera);

	AdjustCameraCollision(oldcamera,true);
	
	AdjustHeight(oldcamera);
	ResetPosition();	
 

	if (OperatorStarted())
	{
  		HC_Pan_Camera(-m_rotate*150.0f,0);
		m_totalrotate+= (-m_rotate*150.0f);
	}


	if (!m_bBlockedForward && !m_bBlockedBackward)
		m_totalrotate = m_totalrotateold;

 	HC_Pan_Camera(0.0f,m_tilt);
	m_rotate = 0;
	if (m_bBlocked &&  b_forward)
	{
		m_bBlockedForward = true;
		m_totalrotateold = m_totalrotate;
	}
 	if (m_bBlocked && b_backward)
	{
		m_bBlockedBackward = true;
		m_totalrotateold = m_totalrotate;
	}

	 
	m_bBlocked = false; 	
 	HC_Close_Segment();
	if (m_bChange)
	{
	    m_bChange = false;
	    GetView()->CameraPositionChanged(true);
	    GetView()->GetConstantFrameRateObject()->SetActivityType(GeneralActivity);


  	    GetView()->Update();
	}
	}
	m_bOldSnappedToGround = m_bSnappedToGround;
 	m_oldcamera.GetFromView(GetView());
}


 

void HOpCameraWalk::ResetPosition()
{
		HPoint t,p,u,d;
		HC_Open_Segment_By_Key(GetView()->GetSceneKey());
			HC_Show_Net_Camera_Target (&t.x, &t.y, &t.z);
		HC_Show_Net_Camera_Position (&p.x, &p.y, &p.z);
		HC_Show_Net_Camera_Up_Vector (&u.x, &u.y, &u.z);
	 
		d.Set(t.x - p.x, t.y - p.y, t.z - p.z);
		float length = (float)HC_Compute_Vector_Length(&d);
		switch (m_AxisMode)
		{
		case 0: d.Set(d.x, 0, d.z); break;
		case 1: d.Set(0, d.y, d.z); break;
		case 2: d.Set(d.x, d.y,  0); break;
		}

		
		HC_Compute_Normalized_Vector(&d, &d);

		HC_Set_Camera_Target(p.x + length * d.x, p.y + length * d.y, p.z + length * d.z);
		switch (m_AxisMode)
		{
		case 0: HC_Set_Camera_Up_Vector(0, 1,0); break;
		case 1: HC_Set_Camera_Up_Vector(1, 0,0); break;
		case 2: HC_Set_Camera_Up_Vector(0, 0,1); break;
		}
 		HC_Close_Segment();
}

static void build_rotation_matrix(float m[4][4], float u[3], float v[3]) 
{ 
 float e = u[0]*v[0] + u[1]*v[1] + u[2]*v[2]; 


 // if e is nearly equal to 1, then the angle is 0 
 if(e > 0.999999f) 
   { 
    m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; 
    m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; 
    m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; 
   } 
 // if e is nearly equal to -1, then the angle is Pi. 
 else if(e < -0.999999f) 
   { 
    float x[3]; 
    x[0] = (u[0] < 0.0)? -u[0] : u[0]; 
    x[1] = (u[1] < 0.0)? -u[1] : u[1]; 
    x[2] = (u[2] < 0.0)? -u[2] : u[2]; 


    // x[0] is the minimum, use y = <1, 0, 0> 
    if((x[0] < x[1]) && (x[0] < x[2])) 
      { 
       float z[3] = { 0.0f, u[2], -u[1] }; 
       float d = (float)sqrt(z[1]*z[1] + z[2]*z[2]); 
       z[1] = z[1]/d; 
       z[2] = z[2]/d; 


       float dy = 2.0f*z[1]; 
       float dz = 2.0f*z[2]; 


       m[0][0] = -1.0f; 
       m[0][1] =  0.0f; 
       m[0][2] =  0.0f; 


       m[1][0] = 0.0f; 
       m[1][1] = dy*z[1] - 1.0f; 
       m[1][2] = dz*z[1]; 


       m[2][0] = 0.0f; 
       m[2][1] = dy*z[2]; 
       m[2][2] = dz*z[2] - 1.0f; 
      } 
    // x[1] is the minimum, use y = <0, 1, 0> 
    else if((x[1] < x[0]) && (x[1] < x[2])) 
      { 
       float z[3] = { -u[2], 0.0f, u[0] }; 
       float d = (float)sqrt(z[0]*z[0] + z[2]*z[2]); 
       z[0] = z[0]/d; 
       z[2] = z[2]/d; 


       float dx = 2.0f*z[0]; 
       float dz = 2.0f*z[2]; 


       m[0][0] = dx*z[0] - 1.0f; 
       m[0][1] = 0.0f; 
       m[0][2] = dz*z[0]; 


       m[1][0] =  0.0f; 
       m[1][1] = -1.0f; 
       m[1][2] =  0.0f; 


       m[2][0] = dx*z[2]; 
       m[2][1] = 0.0f; 
       m[2][2] = dz*z[2] - 1.0f; 
      } 
    // x[2] is the minimum, use y = <0, 0, 1> 
    else 
      { 
       float z[3] = { u[1], -u[0], 0.0f }; 
       float d = (float)sqrt(z[0]*z[0] + z[1]*z[1]); 
       z[0] = z[0]/d; 
       z[1] = z[1]/d; 


       float dx = 2.0f*z[0]; 
       float dy = 2.0f*z[1]; 


       m[0][0] = dx*z[0] - 1.0f; 
       m[0][1] = dy*z[0]; 
       m[0][2] = 0.0f; 


       m[1][0] = dx*z[1]; 
       m[1][1] = dy*z[1] - 1.0f; 
       m[1][2] = 0.0f; 


       m[2][0] =  0.0f; 
       m[2][1] =  0.0f; 
       m[2][2] = -1.0f; 
      } 
   } 
 else 
   { 
    // compute u x v 
    float c[3]; 
    c[0] = u[1]*v[2] - u[2]*v[1]; 
    c[1] = u[2]*v[0] - u[0]*v[2]; 
    c[2] = u[0]*v[1] - u[1]*v[0]; 


    float hc = (1.0f - e)/(c[0]*c[0] + c[1]*c[1] + c[2]*c[2]); 
    float hx = hc*c[0]; 
    float hy = hc*c[1]; 
    float hzz = hc*c[2]; 


    m[0][0] = hx*c[0] + e; 
    m[0][1] = hx*c[1] - c[2]; 
    m[0][2] = hx*c[2] + c[1]; 


    m[1][0] = hy*c[0] + c[2]; 
    m[1][1] = hy*c[1] + e; 
    m[1][2] = hy*c[2] - c[0]; 


    m[2][0] = hzz*c[0] - c[1]; 
    m[2][1] = hzz*c[1] + c[0]; 
    m[2][2] = hzz*c[2] + e; 
   } 


}

static HC_KEY ConstructExtrudedBox(HPoint &p1, HPoint &p2, HPoint &u, float width, float height, bool noextrude)
{
	UNREFERENCED(u);
	UNREFERENCED(height);

	HPoint pnew;
	HPoint delta;
	delta.Set(p2.x - p1.x, p2.y - p1.y, p2.z - p1.z);	
	HPoint org;
	if (!noextrude)
		org.Set(width/2.0f,width/2.0f,width/2.0f);
	else
		org.Set(width,width,width);

	float length = static_cast<float>(HC_Compute_Vector_Length(&delta));
	if (!noextrude)
		pnew.Set(-width/2.0f, -width/2.0f - length, -width/2.0f);
	else
		pnew.Set(-width, -width, -width);

	HC_KEY boxkey = HUtility::InsertBox(&org, &pnew);
	float m[4][4];
	float o[3];
	float x[3];
	o[0] = 0; o[1] = -1; o[2] = 0;
	x[0] = delta.x; x[1] = delta.y, x[2] = delta.z;
	HC_Compute_Normalized_Vector(x,x);
	
	if (length >0.000001f)
	{
		
		build_rotation_matrix(m,o, x);
		float matrix[16];
		HC_Compute_Identity_Matrix(matrix);	
		if (!noextrude)
		{
		matrix[0] = m[0][0];
		matrix[1] = m[1][0];
		matrix[2] = m[2][0];
		
		matrix[4] = m[0][1];
		matrix[5] = m[1][1];
		matrix[6] = m[2][1];
		
		matrix[8] = m[0][2];
		matrix[9] = m[1][2];
		matrix[10] = m[2][2];
		}
	
	
			HC_Set_Modelling_Matrix(matrix);
	}
	if (!noextrude)
		HC_Translate_Object(p1.x,p1.y,p1.z);
	else
		HC_Translate_Object(p2.x,p2.y,p2.z);

	return boxkey;
	
	
}

void HOpCameraWalk::AdjustHeight(HCamera &oldcamera)
{
	
	if (!m_bCollision || !m_bSnapToFloor)
		return;
	
	float distance = 0.0f;
	HPoint p;
	HC_Open_Segment_By_Key(GetView()->GetSceneKey());
 	HC_Show_Net_Camera_Position (&p.x, &p.y, &p.z);
	HC_Close_Segment();
 	int shift;
	GetView()->GetKeyState(87,shift);
	if (shift & MVO_LEFT_SHIFT)
		return;
  	
	switch (m_AxisMode)
	{
		case 0: distance = p.y - oldcamera.position.y; break;
		case 1: distance = p.x - oldcamera.position.x; break;
		case 2: distance = p.z - oldcamera.position.z; break;
	}
	if (distance != 0 )
		distance = distance;

//	(distance<0 && -distance > m_MaxDownGap)) && m_bOldSnappedToGround && m_bSnappedToGround)
//		(distance<0 && -distance > m_MaxDownGap)) || !m_bSnappedToGround)

 	if (((distance>0 && distance > m_MaxUpGap) || 
	(distance<0 && -distance > m_MaxDownGap)) && m_bOldSnappedToGround && m_bSnappedToGround)
	{	

			HC_Open_Segment_By_Key(GetView()->GetSceneKey());
				HC_Set_Camera (&oldcamera.position, &oldcamera.target, &oldcamera.up_vector, oldcamera.field_width, oldcamera.field_height, oldcamera.projection);
			HC_Close_Segment();
			m_bBlocked = true;

	}
}


void HOpCameraWalk::AdjustCameraCollision(HCamera &oldcamera, bool noextrude)
{
	HPoint t,p,u,d;
	
	if (!m_bCollision)
		return;
 	int shift;
	GetView()->GetKeyState(87,shift);
	if (shift & MVO_LEFT_SHIFT)
		return;
	
	HC_Open_Segment_By_Key(GetView()->GetSceneKey());
	HC_Show_Net_Camera_Target (&t.x, &t.y, &t.z);
	HC_Show_Net_Camera_Position (&p.x, &p.y, &p.z);
	HC_Show_Net_Camera_Up_Vector (&u.x, &u.y, &u.z);
	HC_Close_Segment();
	HPoint pold = p;
	int iterate_level = 0;
  iterate_closer:
	HPoint p1,p2;
	float size = 0.5f * m_AvatarDiameter;
	p1.Set(p.x - size, p.y - size, p.z - size);
	p2.Set(p.x + size, p.y + size, p.z + size);
	
	HPoint delta;
	delta.Set(p.x - oldcamera.position.x, p.y - oldcamera.position.y, p.z - oldcamera.position.z);	
	float length = static_cast<float>(HC_Compute_Vector_Length(&delta));
	HC_Compute_Normalized_Vector(&delta, &delta);
	
	HC_Open_Segment("/tempbox");
	HC_Flush_Contents(".", "everything");
	HC_Open_Segment("box");
	HC_KEY boxkey = ConstructExtrudedBox(oldcamera.position, p, oldcamera.up_vector, size,size, noextrude);
	HPoint points[8];
	int face_list[30];
	int pnum,fnum;
	float matrix[16];
	
	HC_Show_Shell(boxkey, &pnum, points, &fnum, face_list);
	HC_Show_Modelling_Matrix(matrix);	
	HC_Close_Segment();
	HC_Compute_Transformed_Points(8, points, matrix,points);
	HC_Flush_Contents(".", "everything");
	HC_Insert_Shell(pnum, points, fnum, face_list);
	HC_Close_Segment();
	
	
	HC_Open_Segment_By_Key(GetView()->GetViewKey());

	int selection_count = HC_Compute_Selection_By_Shell("v, selection level = entity", "./scene/overwrite", pnum, points, fnum, face_list);			

	HC_Close_Segment();
	
	if (selection_count)
		{		
			if (!noextrude)
				{
					if (iterate_level<3)
						{
							p.Set(p.x - delta.x * length/3.0f, p.y - delta.y * length/3.0f,
								  p.z - delta.z * length/3.0f);
							iterate_level++;
							goto iterate_closer;
							
						}
				}
			
			m_bBlocked = true;
			HC_Open_Segment_By_Key(GetView()->GetSceneKey());
			HC_Set_Camera (&oldcamera.position, &oldcamera.target, &oldcamera.up_vector, oldcamera.field_width, oldcamera.field_height, oldcamera.projection);
			HC_Close_Segment();
			
		}
	else
		{		
			if (iterate_level>0)
				{
					HC_Open_Segment_By_Key(GetView()->GetSceneKey());
					HC_Set_Camera_Position(p.x, p.y, p.z);
					HC_Set_Camera_Target(t.x + (p.x - pold.x), t.y + (p.y - pold.y),
										 t.z + (p.z - pold.z));
					HC_Close_Segment();
				}
		}
}

void  HOpCameraWalk::SnapToGround()
{
	HPoint t,p,u,d;
 	
	if (!m_bSnapToFloor)
		return;
	
 	int shift;
	GetView()->GetKeyState(87,shift);
	if (shift & MVO_LEFT_SHIFT)
		return;
	
	HC_Open_Segment_By_Key(GetView()->GetSceneKey());
	HC_Show_Net_Camera_Target (&t.x, &t.y, &t.z);
	HC_Show_Net_Camera_Position (&p.x, &p.y, &p.z);
	HC_Show_Net_Camera_Up_Vector (&u.x, &u.y, &u.z);
	HC_Close_Segment();
	HPoint up_invert;
	switch (m_AxisMode)
	{
	case 0: up_invert.Set(0,-1,0); break;
	case 1: up_invert.Set(-1,0,0);; break;
	case 2: up_invert.Set(0,0,-1);; break;
	}
	HC_Open_Segment_By_Key(GetView()->GetViewKey());

		int selection_count = HC_Compute_Selection_By_Ray("v, selection level = entity", "./scene/overwrite", &p, &up_invert);

	HC_Close_Segment();
	
	HPoint pos1,pos2;
	if (selection_count)
	{
		HC_Open_Segment_By_Key(GetView()->GetSceneKey());
		
		HC_KEY elkey;
		int o1,o2,o3;
		char keytype[4096];
		char segname[4096];
		HC_Show_Selection_Element(&elkey, &o1,&o2,&o3);
		HC_Show_Key_Type(elkey, keytype);
		HC_Show_Segment(HUtility::GrabSegment(elkey), segname);
		
		
		HC_Show_Selection_Position(&pos1.x, &pos1.y, &pos1.z, &pos2.x, &pos2.y, &pos2.z);
		
		HPoint delta;
		delta.Set(p.x - pos1.x, p.y - pos1.y, p.z - pos1.z);
		delta.Set(delta.x - u.x *m_AvatarHeight, delta.y - u.y * m_AvatarHeight, delta.z - u.z * m_AvatarHeight);
#if 0
		float dist;
		switch (m_AxisMode)
		{
		case 0: dist = delta.y; break;
		case 1: dist = delta.x; break;
		case 2: dist = delta.z; break;
		}


		if (dist>m_AvatarHeight/3.0f)
		{
			delta.Set(- up_invert.x * m_AvatarHeight/5.0f, 
				- up_invert.y * m_AvatarHeight/5.0f,
				- up_invert.z * m_AvatarHeight/5.0f);
		}
#endif

		HC_Set_Camera_Position(p.x - delta.x, p.y - delta.y, p.z - delta.z);
		HC_Set_Camera_Target(t.x - delta.x, t.y - delta.y, t.z - delta.z);
		HC_Close_Segment();
		m_bSnappedToGround = true;
	}
	else
		m_bSnappedToGround = false;
}


void HOpCameraWalk::Level()
{

		HPoint t,p,u,d;
		HC_Open_Segment_By_Key(GetView()->GetSceneKey());
			HC_Show_Net_Camera_Target (&t.x, &t.y, &t.z);
			HC_Show_Net_Camera_Position (&p.x, &p.y, &p.z);
			switch (m_AxisMode)
			{
				case 0: 
					HC_Set_Camera_Position(p.x, 0, p.z);
					HC_Set_Camera_Target(t.x, t.y - p.y, t.z);
					break;
				case 1: 
					HC_Set_Camera_Position(0, p.y, p.z);
					HC_Set_Camera_Target(t.x - p.x,t.y, t.z);
					break;
				case 2: 
					HC_Set_Camera_Position(p.x, p.y, 0);
					HC_Set_Camera_Target(t.x ,t.y, t.z - p.z);
					break;
			}
		HC_Close_Segment();
		m_rotate=0;
		m_move=0;
}



int HOpCameraWalk::OnMouseWheel(HEventInfo &event)
{
	int tick = event.GetMouseWheelDelta();
	if (!OperatorStarted())
	{
		if ((event.Shift() && !m_bKeyboardWalk) || (m_bKeyboardWalk))
		{
			if (tick>0)
				m_Speed *= 1.2f;
			else
				m_Speed /= 1.2f;
		}
		else
		{
			if (tick>0)
				m_tilt += 5 * GetView()->GetMouseWheelSensitivity();
			else
				m_tilt -= 5 * GetView()->GetMouseWheelSensitivity();
			m_bChange = true;
		}
	}
	
	return HLISTENER_CONSUME_EVENT;
}



int HOpCameraWalk::OnMButtonDown(HEventInfo &event)
{

	return OnLButtonDown(event);

}

int HOpCameraWalk::OnMButtonDownAndMove(HEventInfo &event)
{
	if (!OperatorStarted())
		return HBaseOperator::OnMButtonDownAndMove(event);

	SetNewPoint(event.GetMouseWindowPos());

	HPoint delta(GetNewPoint() - GetFirstPoint());

	if (fabs(delta.y) < 0.005f)
		m_updown = 0;
	else
		m_updown=delta.y;

	if (fabs(delta.x) < 0.005f)
		m_strafe = 0;
	else
		m_strafe=(delta.x);
	
	return HOP_OK;
}


int HOpCameraWalk::OnMButtonUp(HEventInfo &event)
{
    m_updown = 0;
    m_strafe = 0;
    GetView()->CameraPositionChanged(true, GetView()->GetSmoothTransition());

    return OnLButtonUp(event);
}

bool HOpCameraWalk::signal_callback(int signal, void *signal_data, void *user_data)
{
	UNREFERENCED(signal);
	UNREFERENCED(signal_data);

	((HOpCameraWalk*)user_data)->ViewDeleted();
	return true;
}


void HOpCameraWalk::ReadjustPosition()
{
    
    m_AxisMode = HUtility::CalculateMajorAxis(GetView());
        
    m_rotate = 0;
    m_move = 0;
    m_tilt = 0;
    m_updown = 0;
    m_strafe = 0;
    
    HPoint t,p;
    
    HC_Open_Segment_By_Key(GetView()->GetSceneKey());
    HC_Show_Net_Camera_Target (&t.x, &t.y, &t.z);
    HC_Show_Net_Camera_Position (&p.x, &p.y, &p.z);
    HC_Close_Segment();
    HPoint delta(t - p);
    float l1 = (float)HC_Compute_Vector_Length(&delta);
    if (m_AxisMode == 2)
		delta.Set(delta.x, delta.y, 0);
    else if (m_AxisMode == 1)
		delta.Set(0, delta.y, delta.z);
    else 
		delta.Set(delta.x, 0, delta.z);

    float l2 = (float)HC_Compute_Vector_Length(&delta);
    float deg = (float)H_ACOS(l2/l1) * (180.0f/3.1415927f);   

    ResetPosition();
    
    switch (m_AxisMode)
    {
	case 2:
 	 p.z > t.z ? m_tilt = -deg : m_tilt = deg; 
	 break;

	case 1:
 	 p.x > t.x ? m_tilt = -deg : m_tilt = deg; 
	 break;
	case 0:
 	 p.y > t.y ? m_tilt = -deg : m_tilt = deg; 
	 break;
    }

    m_bViewDeleted = false;
}


int HOpCameraWalk::OnKeyDown(HEventInfo &event)
{


	unsigned short the_char = static_cast<unsigned short>(event.GetChar());

	switch((int)the_char){

	case 32:
		m_tilt = 0;
		m_bChange = true;
  		break;
	
	case '\b':
  		break;
	  
	}//switch

	
 	GetView()->Update();
	return HOP_OK;

}


void HOpCameraWalk::SetupControls(char forward, char backward, char strafe_left, char strafe_right, char up, char down)
{
	m_c_forward = forward;
	m_c_backward = backward;
	m_c_strafe_left = strafe_left;
	m_c_strafe_right = strafe_right;
	m_c_up = up;
	m_c_down = down;

}


 





