// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <wchar.h>
#include <algorithm>
#include <vector>
#include <cmath>
#include <map>
#include <string>

#include "hc.h"

#include "HBaseView.h"

#include "HDB.h"
#include "HEventInfo.h"
#include "HBaseOperator.h"
#include "HSelectionSet.h"
#include "HUtility.h"
#include "HConstantFrameRate.h"
#include "HUtilityGeometryCreation.h"
#include "HSharedKey.h"
#include "HImCuttingGeometry.h"
#include "HImHideOverlappedText.h"
#include "HImManager.h"
#include "HIOManager.h"
#include "HUtilityGeomHandle.h"
#include "HUtilityAntialiasing.h"
#include "HMarkupManager.h"
#include "HOpMoveHandle.h"
#include "HOpCameraManipulate.h"
#include "HUtilityShadow.h"
#include "HEventManager.h"
#include "HUndoManager.h"
#include "HBhvBehaviorManager.h"
#include "HOpCameraOrbit.h"
#include "HDebugZBuffer.h"
#include "HStream.h"
#include "HErrorManager.h"
#include "HUtilityAnnotation.h"
#include "HUtilityLocaleString.h"
#include "HSelectionItem.h"


#define bool2BOOL(trueOrfalse)	\
	(trueOrfalse) ? true : false

/////////////////////////////////////////////////////////////////////////////
// HBaseView constructor creates a default HOOPS segment hierarchy which is 
// common to most applications

//message notification
struct signal_request {
	int signal;
	HSignalNotifyFunction callback;
	void *user_data;
	unsigned long cookie;
};

static bool use_hzb(const char *driver) 
{
	return strieq(driver,"opengl")
		|| strieq(driver,"opengl2")
		|| strieq(driver,"direct3d")
		|| strieq(driver,"dx9")
		|| strieq(driver,"dx11")
		|| strieq(driver,"3ddriver")
		|| strieq(driver,"xgl")
		|| strieq(driver,"sbx");
}


#define MVO_FAKE_HLR_STYLE_SEGMENT	"?Style Library/mvo_fake_hlr"

HBaseView::HBaseView(
	HBaseModel * model,
	const char * alias,	
	const char * driver_type,
	const char * instance_name,
	void * window_handle,
	void * colormap,
	void * clip_override,
	void * window_handle_2,
	const char * driver_path)
{
	m_ViewKey = 0;
	m_pModel = model;
	m_pOperator = 0;
	m_pEvent = 0;
	m_pSelection = 0;
	m_bDynamicHighlighting = false;
	m_ViewMode = HViewXY;
	m_RenderMode = HRenderGouraud;
	m_ShadowMode = HShadowNone;
	for(int i=0; i<HBaseView::MaxLights; ++i) m_SceneLight[i] = INVALID_KEY;
	m_DisplayListType = DisplayListGeometry;
	m_LightCount = 1;
	m_bCuttingPlanesVisibility = true;
	m_bCPLineVisibility = true;
	m_bCPFaceVisibility = true;
	m_CPMode = HIM_CUTTING_GEOMETRY_SEGMENT_BODY;
	m_bCuttingPlanesExist = false;
	m_pHImCuttingGeometry=0;
	
	m_bGeometryChanged = true;
	m_LastMouseWheelTime = 0;
	m_MouseWheelSensitivity = 1.0f;
	m_bInvertMouseWheelZoom = false;

	m_pSharedKey = 0;
	m_pAntialiasing = 0;
	m_pMarkupManager = 0;
	m_bPrevBackplaneCullMode = false;
	m_bAxisMode = AxisOff;
	m_bAxisTriadTransparencyAllowed = true;
	m_bTransparentSelectionBox = false;	//turned on in HoopsPartviewer
	m_AxisWindowSize = 125;

	m_bSpritingAllowed = true;

	emit_message_function = 0;
	emit_message_function_user_data = 0;

	m_bHideOverlappedText = false;
	m_bNavCube = false;	
	m_bSmoothTransition = false;
	m_SmoothTransitionDuration= 0.5f;
	m_bLightFollowsCamera = true;
	m_pConstantFrameRate = new HConstantFrameRate(this);
	m_pfEventChecker = 0;

	m_fZoomLimit = 0.000001f;

	m_WindowColorPercentage = 100;

	m_bFirstUpdate = false;
	m_SuppressUpdate = 0;
	m_SuppressUpdateTick = false;
	m_has_initial_view = false;

	m_bUpdateNotFinished = false;

	SimpList = 0;
	simpListLength = 0;

	//RECORD STUFF FOR INIT
	if(!driver_type) 
		driver_type = HDB::GetDriverType();
	if(!instance_name) 
		instance_name = HDB::GetInstanceBase();

	if (driver_type) {
		m_sDriverType = new char[strlen(driver_type)+1];
		strcpy(m_sDriverType, driver_type);
	}
	else {
		m_sDriverType = new char[strlen(HDB::GetDriverType())+1];
		strcpy(m_sDriverType, driver_type);
	}

	if(!driver_path) {
		H_FORMAT_TEXT pathname("/driver/%s/%s+%d", driver_type, instance_name, HDB::GetUniqueID());
		m_sDriverPath = new char[strlen(pathname)+1];
		strcpy(m_sDriverPath, pathname);
		m_bOwnDriver=true;
	} else {
		/* use predefined driver path */
		m_bOwnDriver=false;
		m_sDriverPath = new char[strlen(driver_path)+1];
		strcpy(m_sDriverPath, driver_path);
	}

	HDB::IncrementInstanceCount();

	if(alias && alias[0] != '\0') {
		m_sAlias = new char[strlen(alias)+1];
		strcpy(m_sAlias, alias);
	}else{
		m_sAlias = 0;
	}
	m_NotFinishedCounter = 0;
	m_FrontAxis.Set(1.0f,0.0f,0.0f);
	m_TopAxis.Set(0.0f,0.0f,1.0f);
	m_pWindowHandle=window_handle;
	m_pColormap=colormap;
	m_pClipOverride=clip_override;
	m_pWindowHandle2=window_handle_2;

	m_SignalRequestsList = 0;
	m_bSceneBoundingSet = false;

	m_pUpdateTimerClient = 0;
	m_bUpdateRequested = false;
	m_pEventManager = 0;
	m_pUndoManager = 0;
	m_pModel->GetEventManager()->RegisterHandler((HUpdateListener *)this,
		HUpdateListener::GetType(), HLISTENER_PRIORITY_NORMAL);
	GetEventManager()->RegisterHandler((HMouseListener *)this,
		HMouseListener::GetType(), HLISTENER_PRIORITY_HIGH + 100);
	GetEventManager()->RegisterHandler((HObjectManipulationListener *)this,
		HObjectManipulationListener::GetType(), HLISTENER_PRIORITY_NORMAL);
	m_SceneKey = INVALID_KEY;
	m_OverwriteKey = INVALID_KEY;
	m_pModel->GetBhvBehaviorManager()->SetActiveView(this);

	m_bLButtonDown = false;
	m_bMouseMove = false;
	
	m_bShadowRegeneration = true;
	m_bShadowRegenerationToggle = false;
	m_bCuttingPlaneRegeneration = true;
	
	m_pManipulateOperator = new HOpCameraManipulate(this);
	m_bCameraChanged = true;
	m_pDebugZBuffer = 0;
	m_pObjectManager = 0;
	m_bHasObjects = false;
	m_pHandleOperator = 0;
	m_bAllowInteractiveShadows = true;
	m_bAllowInteractiveCutGeometry = true;
	m_DefaultSelectionProximity = 0.1f;
	m_bInitialCameraSet = false;
	m_pfKeyState = 0;
	m_bRecordCamera = false;
	m_bForceFastHiddenLine = false;

	m_bLineAntialiasing = false;
	m_bTextAntialiasing = false;
	m_IsoDefault = HViewIsoFrontRightTop;
	m_bBeginShellSelection = false;
	m_MaximumUpdateTime = 0.0f;
	m_bFastFitWorld = false;
	m_bDetailedFitWorld = false;
	m_pHighlight = new HSelectionSet(this);
	m_pHighlight->Init();
	m_pHighlight->SetSelectionLevel(HSelectEntity);
	m_bDisplayHandlesOnDblClk = false;
	m_XSize = 0;
	m_YSize = 0;
	m_XSize_override = 0;
	m_YSize_override = 0;
	m_OldCullingThreshold = -1;
	m_FramerateMode = FramerateOff;
	ResetKeyState();
	m_HighlightMode = HighlightDefault;
	m_HardwareAntialiasing = 0;

	m_InUpdateInternal = false;

	m_bSplatRendering = false;
	m_SplatSize = 1; // aka slide at 1%
	m_SplatSymbol = SplatRound;
	m_FakeHLRColor = HPoint(1.0, 1.0, 1.0); //white
	m_pTempClipArray = new float[12];
	m_FocusedOnSelection = false;

	m_UseExplicitShadowLight = false;

	m_FakeHLRStyle=INVALID_KEY;
	HC_Open_Segment(MVO_FAKE_HLR_STYLE_SEGMENT);{
		m_FakeHLRSource = HC_Open_Segment("");{
			HC_Set_Color("faces=white, lines=edges=black");
			HC_Set_Rendering_Options("attribute lock=(color, visibility=lighting), simple reflection=off, ignore subsegment locks = color");
			HC_Set_Visibility("edges=(everything=off,perimeters,hard),faces=on,markers=off,images=on,cutting planes=on,lines=on,text=on,string cursors=off,windows=on,cut geometry=on,lights=off");
			if (!strieq(m_sDriverType,"opengl2") && !strieq(m_sDriverType,"dx9") && !strieq(m_sDriverType,"dx11"))
				HC_Set_Visibility("edges=interior silhouettes");
		}HC_Close_Segment();
	}HC_Close_Segment();

	m_TouchManager = new HTouchManager();
}


void HBaseView::RebindView(
	const char * alias,	
	const char * driver_type,
	const char * instance_name,
	void * window_handle,
	void * colormap,
	void * clip_override,
	void * window_handle_2,
	int debug_flags)
{
	HC_Open_Segment_By_Key(m_ViewKey);{
		HC_Flush_Contents(".", "drivers"); //shuts down the old driver
	}HC_Close_Segment();

	if (driver_type) {
		H_SAFE_DELETE_ARRAY (m_sDriverType);
		m_sDriverType = new char[strlen(driver_type)+1];
		strcpy(m_sDriverType, driver_type);
	}
	if (!m_sDriverType) {
		m_sDriverType = new char[strlen("opengl")+1];
		strcpy(m_sDriverType, "opengl");
	}

	if(!instance_name) 
		instance_name = HDB::GetInstanceBase(); //just grab the mvo default

	H_FORMAT_TEXT pathname("/driver/%s/%s+%d", m_sDriverType, instance_name, HDB::GetUniqueID());
	char * old_driver_path = m_sDriverPath;
	m_sDriverPath = new char[strlen(pathname)+1];
	strcpy(m_sDriverPath, pathname);
	HC_Rename_Segment (old_driver_path, m_sDriverPath);
	H_SAFE_DELETE_ARRAY (old_driver_path);

	if (m_sAlias)
		HC_UnDefine_Alias(m_sAlias);

	if (alias) {
		m_sAlias = new char[strlen(alias)+1];	
		strcpy(m_sAlias, alias);
	}

	if(m_sAlias)
		HC_Define_Alias (m_sAlias, m_sDriverPath);

	m_pWindowHandle = window_handle;
	m_pColormap = colormap;
	m_pClipOverride = clip_override;
	m_pWindowHandle2 = window_handle_2;

	HC_Open_Segment_By_Key(m_ViewKey);{
		if (m_pWindowHandle)
			HC_Set_Driver_Options ("disable input = all");
		HC_Set_Driver_Options (H_FORMAT_TEXT("use window id = (default = %s)", kto0xa(m_pWindowHandle)));
		if (m_pColormap) {
			HC_Set_Driver_Options (H_FORMAT_TEXT("use colormap id = %s", kto0xa(m_pColormap)));
		}
		if (m_pClipOverride) {
			HC_Set_Driver_Options (H_FORMAT_TEXT("use clip override = %s", kto0xa(m_pClipOverride)));
		}
		if (m_pWindowHandle2) {
			HC_Set_Driver_Options (H_FORMAT_TEXT("use window id2 = %s%p", H_EXTRA_POINTER_FORMAT, m_pWindowHandle2));
		}
		HC_Set_Driver_Options (H_FORMAT_TEXT("debug = %u", (unsigned int)debug_flags));
	}HC_Close_Segment();

	// some view Defaults
	HC_Open_Segment_By_Key(m_ViewKey);{
		if (use_hzb(m_sDriverType)) {
			HC_Set_Rendering_Options("hsra = hardware z-buffer, technology = standard");
			if (m_HardwareAntialiasing > 0)
			{
				char text[MVO_BUFFER_SIZE];
				sprintf(text, "anti-alias = %d", m_HardwareAntialiasing);
				HC_Set_Driver_Options(text);
				HC_Set_Rendering_Options("anti-alias = (screen)");
			}
			HC_Set_Driver_Options("double-buffering");
		}
		else {
			// turn on double buffering so the axis window doesn't flicker
			HC_Set_Rendering_Options("hsra = szb, technology = sfb");
			HC_Set_Driver_Options("fixed colors = 216, double-buffering, no gamma correction");
		}
	}HC_Close_Segment();

	Update();
}


HBaseView::~HBaseView()
{
	m_SuppressUpdate++;
	m_SuppressUpdateTick++;
	emit_message_function = 0;

	if (m_AnnotationCookie != 0)
		UnSetSignalNotify(m_AnnotationCookie);

	EndShellSelection();
	SetViewUpdateMode(OnDemand);
	delete m_pUpdateTimerClient;
	
	H_SAFE_DELETE(m_pOperator);
	H_SAFE_DELETE(m_pEvent);
	if (m_pSelection)
		m_pSelection->DeSelectAll();
	if (m_pHighlight)
		m_pHighlight->DeSelectAll();
	H_SAFE_DELETE(m_pSelection);
	H_SAFE_DELETE(m_pHighlight);
	H_SAFE_DELETE(m_pSharedKey);
	H_SAFE_DELETE(m_pHImCuttingGeometry);
	H_SAFE_DELETE(m_pAntialiasing);
	H_SAFE_DELETE(m_pMarkupManager);
	H_SAFE_DELETE(m_pDebugZBuffer);
	H_SAFE_DELETE_ARRAY(m_sDriverPath);
	H_SAFE_DELETE_ARRAY(m_sDriverType);
	H_SAFE_DELETE_ARRAY(m_sAlias);
	H_SAFE_DELETE_ARRAY(m_pTempClipArray);

	//remove signal request list after deleting everything else in case
	// other objects are trying to be responsible and clean up after themselves.
	if(m_SignalRequestsList != 0) {
		struct signal_request *erq;

		Notify(HSignalDelete);
		while((erq = (struct signal_request *)vlist_remove_first(m_SignalRequestsList)) != 0) {
			delete erq;
		}
		delete_vlist(m_SignalRequestsList);
		m_SignalRequestsList = 0;
	}

	delete m_pObjectManager;
	
	HDB::DecrementInstanceCount();
	if (m_pModel)
		m_pModel->GetEventManager()->UnRegisterHandler((HUpdateListener *)this, HUpdateListener::GetType());

	cleanupSimpList();

	HLISTENER_EVENT(HUpdateListener, GetEventManager(), ViewDestroyedEvent(this));
	
	delete m_pManipulateOperator;

	delete m_pEventManager;
	delete m_pUndoManager;
	delete m_TouchManager;

	if (m_pConstantFrameRate)
	{
		m_pConstantFrameRate->Stop();
		m_pConstantFrameRate->Shutdown();
	}
	H_SAFE_DELETE(m_pConstantFrameRate);
	if (m_ViewKey && m_bOwnDriver)
		HC_Delete_By_Key (m_ViewKey);

	HC_Delete_By_Key(m_FakeHLRSource);
}


void HBaseView::Init()
{
	if(m_sAlias)
		HC_Define_Alias (m_sAlias, m_sDriverPath);

	// create scene and windowspace segments as subsegments of the main view segment
	m_ViewKey = HC_KOpen_Segment (m_sDriverPath);

		HC_Set_Selectability("everything = off");
		// Store a pointer to the HBaseView object on the driver segment which we can then query
		// in any IM callback via HIC_Show_User_Index
		HC_Set_User_Index(H_HBASEVIEW_POINTER_MAGIC, this);

		m_WindowspaceKey = HC_KCreate_Segment("windowspace");

		char dopt[MVO_BUFFER_SIZE];

		// client passed a window id handle
		if (m_pWindowHandle) {
			//in (future) threaded HDB mode, we'll use this one
			//sprintf (dopt, "isolated, disable input = all, use window id = %s%p", H_EXTRA_POINTER_FORMAT, m_pWindowHandle);
			sprintf (dopt, "disable input = all, use window id = (default = %s%p)", H_EXTRA_POINTER_FORMAT, m_pWindowHandle);
			HC_Set_Driver_Options (dopt);
		}

		// client passed a colormap
		if (m_pColormap) {
			sprintf (dopt, "use colormap id = %s%p", H_EXTRA_POINTER_FORMAT, m_pColormap);
			HC_Set_Driver_Options (dopt);
		}

		// client passed a colormap
		if (m_pClipOverride) {
			sprintf (dopt, "use clip override = %s%p", H_EXTRA_POINTER_FORMAT, m_pClipOverride);
			HC_Set_Driver_Options (dopt);
		}

		// client passed a window id 2  handle
		if (m_pWindowHandle2) {  
			sprintf (dopt, "use window id2 = %s%p", H_EXTRA_POINTER_FORMAT, m_pWindowHandle2);
			HC_Set_Driver_Options (dopt);
		}

		if (HDB::GetIsolatedDrivers())
			HC_Set_Driver_Options("isolated=on");
		
		// create the lights segment as a subsegment of the scene segment
		m_SceneKey = HC_KOpen_Segment("scene");
			m_AxisTriadKey = HC_KCreate_Segment("axis");
			m_OverwriteKey = HC_KOpen_Segment("overwrite");
				m_ConstructionKey = HC_KCreate_Segment("construction");
				m_LightsKey = HC_KCreate_Segment("lights");
				m_CuttingPlanesKey = HC_KCreate_Segment("cutting planes");
				m_CuttingPlaneGeomKey = HC_KCreate_Segment("cutting plane geometry");
				m_IntersectPolylinesKey = HC_KCreate_Segment("intersection polylines");
				m_BBoxGeometryKey = HC_KCreate_Segment("bbx geometry");
				m_ProgressBarKey = HC_KCreate_Segment("progress bar geometry");
			HC_Close_Segment();
			// include the HOOPS segment associated with this view's model object in the scene segment

			HC_Open_Segment_By_Key(m_OverwriteKey);
			{
				if (m_pModel)
				{
					// Note: "SelectionConditionIncluder" is not meant to be used/exposed/have any attributes.
					// This is purely for selection highlighting. Since this segment is an includer
					// we place our 'conditions' here for highlighting and we don't want them to get 
					// lost when the 'overwrite' is flushed - Rajesh B (03-Jan-05)
					HC_Open_Segment("SelectionConditionIncluder");
							m_lIncludeLinkKey = HC_KInclude_Segment_By_Key(m_pModel->GetModelKey());
					HC_Close_Segment();
				}
			}
			HC_Close_Segment();
		HC_Close_Segment();
	HC_Close_Segment();

	// some view Defaults
	HC_Open_Segment_By_Key(m_ViewKey);
		HC_Set_Heuristics("hidden surfaces, related selection limit = 5");
		HC_Set_Window_Frame ("off");
		SetDefaultSelectionProximity(0.1f);
		HC_Set_Rendering_Options("hlro = (face displacement = 5, visibility = off, pattern = 1, dim factor = 0.6)");
		HC_Set_Rendering_Options ("join cutoff angle=150");
		HC_Set_Driver_Options("marker drawing = fastest");
		
		if (use_hzb(m_sDriverType))
		{

			HC_Set_Rendering_Options ("hsra = hardware z-buffer, technology = standard");
			HC_Set_Driver_Options ("double-buffering");
			if (m_HardwareAntialiasing > 0)
			{
				char text[MVO_BUFFER_SIZE];
				sprintf(text, "anti-alias = %d", m_HardwareAntialiasing);
				HC_Set_Driver_Options(text);
				HC_Set_Rendering_Options("anti-alias = (screen)");
			}

		}
		else
		{
			// turn on double buffering so the axis window doesn't flicker
			HC_Set_Rendering_Options("hsra = szb, technology = sfb");
			HC_Set_Driver_Options("fixed colors = 216,double-buffering, no gamma correction");
		}
	HC_Close_Segment();

	SetWindowColor(HPoint(1,1,1));

	// scene Defaults
	HC_Open_Segment_By_Key(m_SceneKey);
		// set a default camera.
		HC_Set_Visibility ("markers=off, lights=(faces=on, edges=off, markers=off), cut geometry =(faces = on, edges = on)");
		HC_Set_Rendering_Options ("color interpolation=(no faces, no edges, markers), no color index interpolation");
		// we turn off texture interpolation for edges since this is currently not hardware accelerated in 3dGS
		HC_Set_Rendering_Options("texture interpolation = (edges=off)");
		HC_Set_Text_Alignment("v*");
		HC_Set_Text_Font("transforms = off, size = 12 pts, name = sans serif");
		HC_Set_Color_By_Value("lines, text, edges, markers", "RGB", 0.0,0.0,1.0);
		HC_Set_Color("faces = green, cut edges = white");
		HC_Set_Marker_Size(0.3);
		HC_Set_Marker_Symbol("(*)");
		HC_Set_Selectability("everything = off, geometry = on");
		HC_Set_Rendering_Options ("cut geometry options = (level = segment)");
		//HC_Set_User_Index(H_MARKUP_POS_MAGIC,m_pTempClipArray);
	HC_Close_Segment();

	// windowspace defaults
	HC_Open_Segment_By_Key(m_WindowspaceKey);
		HC_Set_Handedness("left");
		HC_Set_Line_Weight(1);
		HC_Set_Edge_Weight(1);
		HC_Set_Camera_By_Volume ("stretched", -1.0, 1.0, -1.0, 1.0);
		HC_Set_Heuristics("quick moves, no hidden surfaces");
		HC_Set_Selectability ("everything = off");
		HC_Set_Visibility("cutting planes = off, faces = off, lights = off");
		HC_Set_Visibility("edges = on, lines = on, text = on, markers = on");
		HC_Set_Visibility("no shadows");
		HC_Set_Color("lines = markers = text = light green");
		HC_Set_Rendering_Options ("no display lists");
		HC_Set_Rendering_Options("no frame buffer effects");
		HC_Set_Heuristics("exclude bounding");
	HC_Close_Segment();	

	// construction defaults
	HC_Open_Segment_By_Key(m_ConstructionKey);
		HC_Set_Heuristics("quick moves, no backplane culling, no hidden surfaces");
		HC_Set_Selectability ("everything = off");
		HC_Set_Line_Weight(1);
		HC_Set_Edge_Weight(1);
		HC_Set_Visibility("lights = off, cutting planes = off, faces = off, edges = on, lines = on, text = on, markers = off");
		HC_Set_Visibility("no shadows");
		HC_Set_Color("lines = markers = text = light green");
		HC_Set_Rendering_Options ("nurbs curve = (budget = 10000, maximum angle = 10)");
		HC_Set_Rendering_Options ("no display lists");
		HC_Set_Rendering_Options("no frame buffer effects");
		HC_Set_Heuristics("exclude bounding");
	HC_Close_Segment();

	// construction defaults
	HC_Open_Segment_By_Key(m_ProgressBarKey);
		HC_Set_Handedness("left");
		HC_Set_Line_Weight(1);
		HC_Set_Edge_Weight(1);
		HC_Set_Rendering_Options("depth range=(0,0.1)");
		HC_Set_Camera_By_Volume ("stretched", -1.0, 1.0, -1.0, 1.0);
		HC_Set_Visibility("no shadows");
		HC_Set_Rendering_Options("no frame buffer effects");
		HC_Set_Heuristics("exclude bounding");
	HC_Close_Segment();

	HC_Open_Segment_By_Key(m_BBoxGeometryKey);
		HC_Set_Edge_Weight(2.0);
		HC_Set_Line_Weight(2.0);
		HC_Set_Color ("geometry = blue");
		HC_Set_Selectability("off");
		HC_Set_Visibility("cutting planes = off, lights = off, markers = off, edges = on, faces = off");
		HC_Set_Visibility("no shadows");
		HC_Set_Rendering_Options("no frame buffer effects");
		HC_Set_Heuristics("exclude bounding");
	HC_Close_Segment();

	HC_Open_Segment_By_Key(m_CuttingPlanesKey);
		HC_Set_Selectability("edges=on");
		HC_Set_Rendering_Options("no frame buffer effects");
		HC_Set_Line_Weight(1);
		HC_Set_Edge_Weight(1);
		HC_Set_Visibility("faces=off, markers=off, edges=on");
		HC_Set_Visibility("no shadows");
	HC_Close_Segment();

	HC_Open_Segment_By_Key(m_CuttingPlaneGeomKey);
		HC_Set_Selectability("everything = off");
		HC_Set_Visibility("cutting planes = off, lines = on");
		HC_Set_Visibility("no shadows");
		HC_Set_Rendering_Options( "face displacement = 2" );
		HC_Set_Rendering_Options( "no lighting interpolation" );
		HC_Set_Rendering_Options( "no display lists" );
		HC_Set_Heuristics("concave polygons");
		HC_Set_Heuristics( "no polygon handedness" );
		HC_Set_Rendering_Options("no frame buffer effects");
		HC_Set_Line_Weight(1);
		HC_Set_Edge_Weight(1);

		#if 0
		/* I wanted to set a face pattern and make the cut lines be bold, but that
		 * interfered with hoops shell-level colors */
		HC_Set_Line_Weight( 3.0 );
		HC_Set_Face_Pattern( "//" );
		HC_Set_Color( "face contrast = light grey" );
		#endif
	HC_Close_Segment();

	HC_Open_Segment_By_Key(m_IntersectPolylinesKey);
		HC_Set_Selectability("everything = off");
		HC_Set_Visibility("lines = on");
		HC_Set_Visibility("no shadows");
		HC_Set_Line_Weight(1);
		HC_Set_Edge_Weight(1);
		HC_Set_Rendering_Options( "no lighting interpolation" );
		HC_Set_Line_Weight( 2.0 );
		HC_Set_Rendering_Options("no frame buffer effects");
	HC_Close_Segment();

	// create a default light 
	SetLightCount(GetLightCount());

	m_ViewMode= HViewXY;

	// init lod
	SetLodMode(false);
	
	// display list
	SetDisplayListMode(true);
//	m_pModel->GetBhvBehaviorManager()->SetActiveSceneKey(GetSceneKey());

// now set in the models file load complete
	m_AnnotationCookie = 0;

	m_AxisResizeCookie = SetSignalNotify( HSignalResize, 
		(HSignalNotifyFunction)HBaseView::AdjustWindowCallback, (void*)this);

	SetViewSelectionLevel(HSelectionLevelEntity);
	
	SetViewUpdateMode(OnDemand);
}

void HBaseView::SetAnnotationResize(bool value)
{

	if (value == true) {
		if (m_AnnotationCookie == 0)
			m_AnnotationCookie = SetSignalNotify( HSignalResize,
				(HSignalNotifyFunction)HUtilityAnnotation::WindowResizeCallback, (void*)this);
	}
	else {	
		if (m_AnnotationCookie != 0)
			UnSetSignalNotify(m_AnnotationCookie);

		m_AnnotationCookie = 0;
	}

}
void HBaseView::AdjustWindowCallback(int signal, void * signal_data, void * user_data)
{
	HBaseView * view = (HBaseView*)user_data;
	AdjustAxisWindowCallback(signal, signal_data, user_data);
	HC_Open_Segment_By_Key(view->GetModel()->GetModelKey());
	if (HUtility::SegmentExists("HAbsoluteWindow"))
	{
		HC_Open_Segment("HAbsoluteWindows");
		HAbsoluteWindow::AdjustAll(view);
		HC_Close_Segment();
	}
	HC_Close_Segment();
	HC_Open_Segment_By_Key(view->GetSceneKey());
	HC_Open_Segment("HAbsoluteWindows");
	HAbsoluteWindow::AdjustAll(view);
	HC_Close_Segment();
	HC_Close_Segment();
	view->Update();

}



void HBaseView::AdjustAxisWindowCallback(int signal, void * signal_data, void * user_data)
{
	UNREFERENCED(signal);
	UNREFERENCED(signal_data);

	HBaseView * view = (HBaseView*)user_data;
	int size;
	int w, h;

	if(view->m_bAxisMode == AxisOff)
		return;
	
	size = view->m_AxisWindowSize;

	HC_Open_Segment_By_Key(view->GetViewKey());
			w = view->m_XSize;
			h = view->m_YSize;		
		if(w > 0 && h > 0) {  // 5145
			HC_Open_Segment_By_Key(view->m_AxisTriadKey);
				char text[4096];
				/* 2.03125 to scale up the range 1/64th.  We scale the axis triad down 1/64th to make sure none of the
				 * letters are clipped. [#9800] */
				HCLOCALE(sprintf(text,"screen range=(%f, %f, %f, %f)",
					-1.0, -1.0 + 2.03125 * (float)size/(float)w, -1.0, -1.0 + 2.03125 * (float)size/(float)h));
				HC_Set_Rendering_Options (text);
			HC_Close_Segment();
		}
	HC_Close_Segment();
}

// 增加轴在窗口上
void HBaseView::AdjustAxisWindow(int axis_window_pixels_in, HIntRectangle *rect_in)
{
	UNREFERENCED(rect_in);

	if(m_bAxisMode == AxisOff)
		return;

	if(axis_window_pixels_in > 0)
		m_AxisWindowSize = axis_window_pixels_in;

	AdjustAxisWindowCallback(0,0,this);
}


void HBaseView::OnSetAxisMode( AxisMode mode )
{
	HC_Open_Segment_By_Key(m_AxisTriadKey);

	HC_Flush_Contents(".","everything");

	HC_Open_Segment("");
	// this forces the axis segment forward in the draw order
	// to get it visited early for timed updates
	HC_Set_Heuristics("quick moves");
	HC_Close_Segment();


	//things under the axis triad segment are too small to benefit from display lists
	HC_Set_Rendering_Options("no display lists"); 
	HC_Set_Rendering_Options("no force gray scale");
	HC_Set_Heuristics("no culling");
	HC_Set_Visibility("text = on");
	HC_Set_Visibility("no shadows");

	if (mode==AxisOn || mode == AxisSimple || mode == AxisCenter)
	{
		//this is just a dummy entry as the window needs to be adjusted outside the MVO with regard to the
		//outer window size
		// 屏幕范围
		HC_Set_Rendering_Options ("screen range=(0.1, 1.0, 0.1, 1.0)");
		// 渲染配置 深度
		HC_Set_Rendering_Options("depth range=(0,0.1)");
		// 渲染配置
		HC_Set_Rendering_Options("no lod, no simple shadow, no simple reflection");
		// 排除包围盒
		HC_Set_Heuristics("exclude bounding");


		//the rendering options need to be set here explicitly so that they overwrite any inherited attributes 
		//from the view segment
		// 设置可见性
		HC_Set_Visibility("markers=off,lines=off, edges=off,faces=on,lights=on, cutting planes = off");
		// 渲染配置
		HC_Set_Rendering_Options("attribute lock=(visibility), hidden line removal options = (render faces = on), frame buffer effects = off");
		// 没有几何坐标
		HC_Set_Heuristics("no polygon handedness");
		// 选择能力 everything关闭
		HC_Set_Selectability("everything=off");	
		// 正投影
		HC_Set_Camera_Projection("orthographic");
		// 隐藏表面
		HC_Set_Heuristics("hidden surfaces");
		if (GetHandedness() == HandednessLeft)
			HC_Set_Handedness("left");
		else
			HC_Set_Handedness("right");

		// by doing this 3dGS will not do font discovery when it 
		// first tries to draw the scene
		HC_Set_Text_Font("name=stroked");

		//instead of using the utility function we could just load in an HMF file with the predefined axis cross				
		//(in case it is undesired to add InsertArrow to the helper functions)
		HPoint p1,p2,p3;

		if (mode == AxisOn || mode == AxisCenter)
		{
			if(mode == AxisCenter)
			{
				HC_Translate_Object(-0.25,-0.25,-0.25);
			}

						/* Scale down 1/64th to keep our letters from being clipped by the screen range. [#9800].  */
						HC_Scale_Object(0.984375, 0.984375, 0.984375);

			HC_Open_Segment("ISOPLANE");
			HC_Set_Visibility("lights = off");
			HC_Set_Rendering_Options("attribute lock = (visibility)");
			HC_Set_Color("geometry = (diffuse = black)");
			if( m_bAxisTriadTransparencyAllowed )
				HC_Set_Color("geometry = (transmission = grey)");

			HC_Open_Segment("");
			p1.Set(1.0f,0.0f,0.05f);
			p2.Set(0.70f,0.70f,0.05f);
			p3.Set(0.0f,1.0f,0.05f);
			HC_Insert_Circular_Wedge(&p1,&p2,&p3);
			HC_Scale_Object(0.2f,0.2f,0.2f);
			HC_Translate_Object(0.05f,0.05f,0.0f);
			HC_Close_Segment();

			HC_Open_Segment("");
			p1.Set(1.0f,0.05f,0.0f);
			p2.Set(0.70f,0.05f,0.7f);
			p3.Set(0.0f,0.05f,1.0f);
			HC_Insert_Circular_Wedge(&p1,&p2,&p3);
			HC_Scale_Object(0.2f,0.2f,0.2f);
			HC_Translate_Object(0.05f,0.0f,0.05f);
			HC_Close_Segment();

			HC_Open_Segment("");
			p1.Set(0.05f,1.0f,0.0f);
			p2.Set(0.05f,0.7f,0.7f);
			p3.Set(0.05f,0.0f,1.0f);
			HC_Insert_Circular_Wedge(&p1,&p2,&p3);
			HC_Scale_Object(0.2f,0.2f,0.2f);
			HC_Translate_Object(0.0f,0.05f,0.05f);
			HC_Close_Segment();
			HC_Close_Segment();
		}

		HC_Open_Segment("XYPLANE");
		HC_Open_Segment("Z");
		HC_Set_Rendering_Options("attribute lock = (color=faces)");
		HUtility::InsertArrow(0.5F, 0.1F, 0.05F, 0.1F,"Z", 0, 0, 10, 0.0F);
		HC_Set_Color("geometry = red");
		HC_Rotate_Object(90.0, 0.0, 0.0);
		HC_Close_Segment();				

		if (mode == AxisOn || mode == AxisCenter)
		{
			HC_Open_Segment("");
			HC_Set_Visibility("lights = off");
			HC_Set_Rendering_Options("attribute lock = (visibility)");			
			p1.Set(1.0f,0.0f,0.0f);
			p2.Set(0.70f,0.70f,0.0f);
			p3.Set(0.0f,1.0f,0.0f);
			HC_Insert_Circular_Wedge(&p1,&p2,&p3);
			HC_Scale_Object(0.4f,0.4f,0.4f);
			HC_Translate_Object(0.05f,0.05f,0.0f);
			HC_Set_Color("geometry = (diffuse = grey)");
			if( m_bAxisTriadTransparencyAllowed )
				HC_Set_Color("geometry = (transmission = grey)");
			HC_Close_Segment();
		}
		HC_Close_Segment();


		HC_Open_Segment("XZPLANE");
		HC_Open_Segment("Y");
		// need to set this for the fast hidden line mode
		HC_Set_Rendering_Options("attribute lock = (color=faces)");
		HUtility::InsertArrow(0.5F, 0.1F, 0.05F, 0.1F,"Y", 0, 0, 10, 0.0F);
		HC_Set_Color("geometry = green");
		HC_Close_Segment();

		if (mode == AxisOn || mode == AxisCenter)
		{
			HC_Open_Segment("");
			HC_Set_Visibility("lights = off");
			HC_Set_Rendering_Options("attribute lock = (visibility)");
			p1.Set(1.0f,0.0f,0.0f);
			p2.Set(0.70f,0.0f,0.7f);
			p3.Set(0.0f,0.0f,1.0f);
			HC_Insert_Circular_Wedge(&p1,&p2,&p3);
			HC_Scale_Object(0.4f,0.4f,0.4f);
			HC_Translate_Object(0.05f,0,0.05f);
			HC_Set_Color("geometry = (diffuse = grey)");
			if( m_bAxisTriadTransparencyAllowed )
				HC_Set_Color("geometry = (transmission = grey)");
			HC_Close_Segment();
		}
		HC_Close_Segment();

		HC_Open_Segment("YZPLANE");
		HC_Open_Segment("X");
		HC_Set_Rendering_Options("attribute lock = (color=faces)");
		HUtility::InsertArrow(0.5F, 0.1F, 0.05F, 0.1F, "X", 0, 0, 10, 0.0F);
		HC_Set_Color("geometry = blue");
		HC_Rotate_Object(0.0, 0.0, -90.0);
		// need to set this for the fast hidden line mode
		HC_Close_Segment();

		if (mode == AxisOn || mode == AxisCenter)
		{
			HC_Open_Segment("");
			HC_Set_Visibility("lights = off");
			HC_Set_Rendering_Options("attribute lock = (visibility)");
			p1.Set(0.0f,1.0f,0.0f);
			p2.Set(0.0f,0.7f,0.7f);
			p3.Set(0.0f,0.0f,1.0f);
			HC_Insert_Circular_Wedge(&p1,&p2,&p3);
			HC_Scale_Object(0.4f,0.4f,0.4f);
			HC_Translate_Object(0.0f,0.05f,0.05f);
			HC_Set_Color("geometry = (diffuse = grey)");
			if( m_bAxisTriadTransparencyAllowed )
				HC_Set_Color("geometry = (transmission = grey)");
			HC_Close_Segment();
		}
		HC_Close_Segment();

		m_bAxisMode=mode;

		AdjustAxisWindow();
		RotateOnUpdate("axis");
	}

	HC_Close_Segment();

	m_bAxisMode=mode;
}

void HBaseView::SetAxisMode(AxisMode mode)
{
	m_bAxisMode = mode;

	OnSetAxisMode(mode);
}
 
void HBaseView::RotateOnUpdate(const char *sname)
{
	HPoint position, target, up_vector;

	HC_Open_Segment_By_Key(GetSceneKey());
		HC_Show_Net_Camera_Target(&target.x, &target.y, &target.z);
		HC_Show_Net_Camera_Up_Vector(&up_vector.x, &up_vector.y, &up_vector.z);
		HC_Show_Net_Camera_Position(&position.x, &position.y, &position.z);
	HC_Close_Segment();

	HC_Open_Segment_By_Key(GetSceneKey());
		HC_Open_Segment(sname);
			HPoint oldposition, difference;
			HPoint old_up_vector, difference2;

			position.Add(-target.x, -target.y, -target.z);
			HC_Compute_Normalized_Vector(&position, &position);
			position.Set(position.x*5.0f, position.y*5.0f, position.z*5.0f);
			HC_Show_Net_Camera_Position(&oldposition.x, &oldposition.y, &oldposition.z);
			difference.Set((float)fabs(oldposition.x - position.x), (float)fabs(oldposition.y - position.y),
				(float)fabs(oldposition.z - position.z));

			HC_Show_Net_Camera_Up_Vector(&old_up_vector.x, &old_up_vector.y, &old_up_vector.z);
			difference2.Set((float)fabs(old_up_vector.x - up_vector.x), (float)fabs(old_up_vector.y - up_vector.y),
				(float)fabs(old_up_vector.z - up_vector.z));

//			 we only modify the axis display if there has been an actual change in the camera settings
//			 we also have to consider the up vector!!!
			if (difference.x + difference.y + difference.z > 0.01 || difference2.x + difference2.y + difference2.z > 0.01)
			{
				HC_Set_Camera_Target(0.0,0.0,0.0);
				HC_Set_Camera_Position(position.x, position.y, position.z);	 
				HC_Set_Camera_Up_Vector(up_vector.x, up_vector.y, up_vector.z);	

			}
		HC_Close_Segment();
	HC_Close_Segment();
	
}




void HBaseView::RemoveTransforms ()
{
	HC_Open_Segment_By_Key (m_SceneKey);{
		HC_UnSet_Modelling_Matrix ();
	}HC_Close_Segment();
}

 
void HBaseView::SmartFilterCircumcuboid(const char *segment, const char *defaults, HPoint *min, HPoint *max)
{
	HLISTENER_EVENT(HFitWorldListener, GetEventManager(), PreFitWorldEvent(this));
	HC_Filter_Circumcuboid(segment,defaults, min, max);
	HLISTENER_EVENT(HFitWorldListener, GetEventManager(), PostFitWorldEvent(this));
	
	if(min->x==0.f && min->y==0.f && min->z==0.f &&
		max->x==0.f && max->y==0.f && max->z==0.f)
	{
		min->Set(-1.f, -1.f, -1.f);
		max->Set(1.f, 1.f, 1.f);
	}
}

void HBaseView::FitWorld(HPoint * center_out)
{
	HPoint min,max;
	HPoint center;

	if (GetModel()->GetBhvBehaviorManager()->IsPlaying() && GetModel()->GetBhvBehaviorManager()->GetCameraUpdated())
		return;

	center.Set(0.0, 0.0, 0.0);

	HC_Open_Segment_By_Key (m_SceneKey);

		bool navcube=false;
		if (HC_QShow_Existence("navcube", "self"))
		{
			navcube=true;
			HC_Rename_Segment("navcube", "/include library/navcube");
		}

		GetSceneExtents(min,max);
		if (navcube)
			HC_Rename_Segment("/include library/navcube", "navcube");


		//order the points
		if (min.x > max.x) {
			float tmp;
			tmp = min.x; min.x = max.x; max.x = tmp;
		}
		if (min.y > max.y) {
			float tmp;
			tmp = min.y; min.y = max.y; max.y = tmp;
		}
		if (min.z > max.z) {
			float tmp;
			tmp = min.z; min.z = max.z; max.z = tmp;
		}

		// compute the length of the bbox diagonal vector
		// this will be used to make the camera field a bit larger
		// to ensure that objects between the camera position and target
		// will still not get clipped (this is for perspective viewing mode)

		HPoint diagonalVector;
		float diagonalLength;

		diagonalVector.Set(
			max.x - min.x,
			max.y - min.y,
			max.z - min.z);

		diagonalLength = (float) sqrt(
			(double)diagonalVector.x * diagonalVector.x +
			(double)diagonalVector.y * diagonalVector.y +
			(double)diagonalVector.z * diagonalVector.z);

		center.x = 0.5F * (min.x + max.x);
		center.y = 0.5F * (min.y + max.y);
		center.z = 0.5F * (min.z + max.z);

		HPoint position, target, up;
		float fieldx, fieldy;
		char proj[MVO_BUFFER_SIZE];

		HC_Show_Net_Camera(&position, &target, &up, &fieldx, &fieldy, proj);

		// now we determine the new camera definition
		HPoint new_position, new_target, new_up;
		float new_fieldx, new_fieldy;

		if( diagonalLength != 0 ) {

			new_target = center;

			HPoint viewingVector;

			viewingVector.Set(
				position.x - target.x,
				position.y - target.y,
				position.z - target.z);

			HC_Compute_Normalized_Vector (&viewingVector, &viewingVector);

			// reset camera position to lie on the viewing vector, at a distance of
			// 2.5 * diagonal length away from the center of the scene centroid
			new_position.Set(
				center.x + 2.5f * viewingVector.x * diagonalLength,
				center.y + 2.5f * viewingVector.y * diagonalLength,
				center.z + 2.5f * viewingVector.z * diagonalLength);

			// set the camera field to be the diagonal length of the scene's bounding box
			new_fieldx = diagonalLength;
			new_fieldy = diagonalLength;
			new_up = up;
		}
		else
		{
			
			HPoint viewingVector;

			viewingVector.Set(
				position.x - target.x,
				position.y - target.y,
				position.z - target.z);

			new_target = center;
			new_position = center + position;

			new_up = up;
			new_fieldx = fieldx;
			new_fieldy = fieldy;
		}

		float maxZoomLevel = GetZoomLimit();
		if(new_fieldx < maxZoomLevel)
			new_fieldx = maxZoomLevel;
		if(new_fieldy < maxZoomLevel)
			new_fieldy = maxZoomLevel;
				
		HC_Set_Camera(&new_position, &new_target, &new_up, new_fieldx, new_fieldy, proj);

	HC_Close_Segment();
	
	if(center_out)
		center_out->Set(center);

	return;
}

void HBaseView::FitWorldExact(HPoint * center_out)
{
	/* FirWorldExact depends on everything in the world being in the view, so let's
	 * call FirWorld first since it doesn't have that dependancy. */
	FitWorld();

	float left, right, bottom, top;

	HC_Compute_Screen_Extent_By_Key(m_SceneKey, "", &left, &right, &bottom, &top);
	HC_Open_Segment_By_Key (m_SceneKey);

		HPoint lefttop, bottomright, center, pos, delta;

		lefttop.Set(left, top,0);
		bottomright.Set(right, bottom,0);

		center.Set((lefttop.x + bottomright.x)/2.0f, (lefttop.y + bottomright.y)/2.0f,0);

		HC_Compute_Coordinates(".", "local window",&center,"viewpoint",&center);

		center.z = 0;

		HC_Show_Net_Camera_Position(&pos.x, &pos.y, &pos.z);
		HC_Compute_Coordinates(".", "world",&pos,"viewpoint",&pos);

		delta.Set(pos.x - center.x, pos.y - center.y, pos.z - center.z);
		HC_Dolly_Camera(-delta.x, -delta.y, 0);

		// Project Rectangle points from window space into viewpoint space
		HC_Compute_Coordinates(".", "local window", &lefttop, "viewpoint", &lefttop);
		HC_Compute_Coordinates(".", "local window", &bottomright, "viewpoint", &bottomright);

		HPoint vector;
		vector.Set(lefttop.x - bottomright.x, lefttop.y - bottomright.y, lefttop.z - bottomright.z);
		//HC_Compute_Vector_Length(&vector);
		vector.x = fabs (vector.x);
		vector.y = fabs (vector.y);

		float maxZoomLevel = GetZoomLimit();
		if (vector.x <= maxZoomLevel) 
			vector.x = maxZoomLevel;
		if (vector.y <= maxZoomLevel) 
			vector.y = maxZoomLevel;

		bottomright.Set (1, -1, 0);
		HC_Compute_Coordinates(".", "local window", &bottomright, "local pixels", &bottomright);

		float	window_aspect = bottomright.y / bottomright.x;
		float	camera_aspect = vector.y / vector.x;

		if (camera_aspect < window_aspect)
			vector.y *= window_aspect / camera_aspect;
		else
			vector.x *= camera_aspect / window_aspect;

		HC_Set_Camera_Field(vector.x, vector.y);

	HC_Close_Segment();

	if(center_out)
		center_out->Set(center);

	return;
}


void HBaseView::ZoomToExtents()
{
	PrepareForCameraChange();

	if (GetSmoothTransition())
	{
		HCamera old_camera, new_camera;
		GetCamera(&old_camera);
		FitWorld();
		GetCamera(&new_camera);
		HC_Open_Segment_By_Key(GetSceneKey());
			HUtility::SmoothTransition(old_camera, new_camera, this);
		HC_Close_Segment();
	}
	else
	{
		FitWorld();
		Update();
	}

	CameraPositionChanged(true, GetSmoothTransition());
}

void HBaseView::ZoomToInitialCamera()
{
	PrepareForCameraChange();

	if (!m_bInitialCameraSet)
	{
		ZoomToExtents();
	}
	else
	{
		if (GetSmoothTransition())
		{
			HCamera old_camera, new_camera;
			GetCamera(&old_camera);
			HC_Open_Segment_By_Key(GetSceneKey());{
				HUtility::SmoothTransition(old_camera, m_InitialCamera, this);
			}HC_Close_Segment();
		}
		else
		{
			SetCamera(m_InitialCamera);
			Update();
		}
		CameraPositionChanged(true, GetSmoothTransition());
	}
}


void HBaseView::FitSegment (HC_KEY seg_key)
{
	HPoint min, max;

	// Compute the oru coordinate bounding box for the segment and its
	// subsegments and then use this box to set the camera on ?Picture
	// to just fit the box.

	HC_Open_Segment_By_Key (seg_key);

	// Find the bouding box of the object 
	if (HC_Compute_Circumcuboid (".", &min, &max)) {
		HC_Compute_Coordinates (".", "object", &min, "world", &min);
		HC_Compute_Coordinates (".", "object", &max, "world", &max);
	}
	else {
		min.x = -1.0F; min.y = -1.0F; min.z = -1.0F;
		max.x =  1.0F; max.y =  1.0F; max.z =  1.0F;
	}

	// the object -> world conversion may have flipped the min and max points. 
	if (min.x > max.x) {
		float tmp;
		tmp = min.x; min.x = max.x; max.x = tmp;
	}
	if (min.y > max.y) {
		float tmp;
		tmp = min.y; min.y = max.y; max.y = tmp;
	}

	//scale min point to fudge for perspective
	if (min.x < 0.0f) 
		min.x *= 1.1f;
	else
		min.x *= 0.9f;

	if (min.y < 0.0f)
		min.y *= 1.1f;
	else
		min.y *= 0.9f;
	
	//scale max point to fudge for perspective
	if (max.x < 0.0f)
		max.x *= 0.9f;
	else
		max.x *= 1.1f;

	if (max.y < 0.0f)
		max.y *= 0.9f;
	else
		max.y *= 1.1f;

	HC_Close_Segment();

	if (min.x != max.x && min.y != max.y){
		HC_Open_Segment_By_Key(GetSceneKey());{
			HC_Set_Camera_By_Volume ("p", min.x, max.x, min.y, max.y);
		}HC_Close_Segment();
	}

	return;
}

void HBaseView::FitItemByPath(HC_KEY key, int incl_count, HC_KEY * incl_path)
{
	PrepareForCameraChange();
	HPoint min, max;
	char type[MVO_BUFFER_SIZE];
	char bounding_info[MVO_BUFFER_SIZE];

	if(key==INVALID_KEY)
		return;

	HC_Show_Key_Type(key, type);

	if(!streq(type, "segment") && !streq(type, "reference")
		&& !streq(type, "shell") && !streq(type, "mesh"))
		return;

	HC_Show_Bounding_Info_By_Key(key, bounding_info);

	// try referee
	if (streq(bounding_info, "") && streq(type, "reference")) {
		key = HC_KShow_Reference_Geometry(key);
		HC_Show_Bounding_Info_By_Key(key, bounding_info);
	}
	
	//figure out whether we have bounding info at all
	if(bounding_info[0]!='\0')
	{
		//get the bounding type and relevant points
		if(strstr(bounding_info, "sphere"))
		{
			float radius;
			HPoint center;

			HC_Show_Bounding_Sphere_By_Key(key, &center, &radius);

			min.x=center.x-radius;
			min.y=center.y-radius;
			min.z=center.z-radius;

			max.x=center.x+radius;
			max.y=center.y+radius;
			max.z=center.z+radius;
		}
		else if(strstr(bounding_info, "cuboid"))
			HC_Show_Bounding_Cuboid_By_Key(key, &min, &max);
	}
	else
		return;

	HC_Compute_Coordinates_By_Path(incl_count, incl_path, "object", &min, "world", &min);
	HC_Compute_Coordinates_By_Path(incl_count, incl_path, "object", &max, "world", &max);

	HC_Open_Segment_By_Key (m_SceneKey);
		HPoint center;
		HPoint camera;
		HPoint target;
		HPoint viewingVector;
		HPoint diagonalVector;
		float diagonalLength;	
		HPoint co,to,uo,cn,tn,un;
		float wo, ho, wn, hn;
		char proj[MVO_BUFFER_SIZE];

		HC_Show_Net_Camera(&co, &to, &uo, &wo, &ho, proj);
		
		// compute the length of the bbox diagonal vector
		// this will be used to make the camera field a bit larger
		// to ensure that objects between the camera position and target
		// will still not get clipped (this is for perspective viewing mode)
		diagonalVector.Set(max.x - min.x, max.y - min.y, max.z - min.z);

		diagonalLength = (float) sqrt(
			diagonalVector.x * diagonalVector.x +
			diagonalVector.y * diagonalVector.y +
			diagonalVector.z * diagonalVector.z);
		if(diagonalLength < 0.001f)
			diagonalLength = 0.001f;

		center.x = 0.5F * (min.x + max.x);
		center.y = 0.5F * (min.y + max.y);
		center.z = 0.5F * (min.z + max.z);

		HC_Show_Net_Camera_Position (&camera.x, &camera.y, &camera.z);
		HC_Show_Net_Camera_Target (&target.x, &target.y, &target.z);

		viewingVector.Set(target.x - camera.x, target.y - camera.y,	target.z - camera.z);
		HC_Compute_Normalized_Vector (&viewingVector, &viewingVector);

		// reset camera position to lie on the viewing vector, at a distance of
		// 2.5 * diagonal length away from the center of the selection centroid
		HC_Set_Camera_Position(center.x - 2.5f * viewingVector.x * diagonalLength,
								center.y - 2.5f * viewingVector.y * diagonalLength,
								center.z - 2.5f * viewingVector.z * diagonalLength);
		// set the camera target to be the centroid of the selection
		HC_Set_Camera_Target (center.x, center.y, center.z);
		// set the camera field to be the diagonal length of the selections's bounding box
		HC_Set_Camera_Field (diagonalLength, diagonalLength);
		HC_Show_Net_Camera (&cn,&tn,&un,&wn,&hn, proj);

		if( GetSmoothTransition() )
			HUtility::SmoothTransition(co,to,uo, wo, ho,  cn,tn, un, wn, hn, this);
	
	HC_Close_Segment();

	CameraPositionChanged(true, GetSmoothTransition());
}


void HBaseView::FitSelection(HC_KEY selectionkey, bool adjustToShellNormal)
{
	PrepareForCameraChange();

	HPoint min, max;
	HPoint newmin, newmax;

	int list_size = GetSelection()->GetSize();

	HPoint normal;	
	normal.Set(0,0,0);
	
	if (selectionkey != INVALID_KEY)
		list_size = 1;

	// if no selection or no key is passed, return
	if (list_size < 1)
		return;

	HSmartSelItem theSelectionItem(
		selectionkey != INVALID_KEY ? selectionkey : HC_Create_Segment("/"),
		GetSelection(),
		0, 0
	);

	for (int i = 0; i < list_size; i++)
	{
		HSelectionItem const * currentItem = selectionkey == INVALID_KEY
			? GetSelection()->GetSelectionItemAt(i)
			: &theSelectionItem
			;
		
		char type[MVO_BUFFER_SIZE];
		HC_Show_Key_Type(currentItem->GetKey(), type);
		if (strcmp(type, "segment") == 0)
		{
			if (i == 0) {
				HC_Filter_Circumcuboid_By_Key(currentItem->GetKey(), "", &min, &max);
			}
			HC_Filter_Circumcuboid_By_Key(currentItem->GetKey(), "", &newmin, &newmax);
		}
		else
		{
			if (strcmp(type, "shell") == 0 && adjustToShellNormal)
				HUtility::ComputeShellNormal(currentItem->GetKey(), normal);

			HC_KEY ownerkey = HC_Show_Owner_Original_Key(currentItem->GetKey());
			HC_Open_Segment_By_Key(ownerkey);{
				// since this is just a single piece of geometry we need to
				// move the object into it's own segment
				HC_KEY temp = HC_Open_Segment("");{
					HC_Move_By_Key(currentItem->GetKey(), ".");
					if (i == 0) {
						HC_Compute_Circumcuboid_By_Key(temp, &min, &max);
					}
					HC_Filter_Circumcuboid_By_Key(temp, "", &newmin, &newmax);
				}HC_Close_Segment();
				HC_Move_By_Key(currentItem->GetKey(), ".");
				HC_Delete_By_Key(temp);
			}HC_Close_Segment();
		}

		// take care of any modelling matrices on the segment
		HC_KEY keys[MVO_BUFFER_SIZE];
		keys[0] = HC_Show_Owner_Original_Key(currentItem->GetKey());
		currentItem->GetIncludeKeys(keys + 1);
		int const kcount = currentItem->GetIncludeCount() + 1;
		if (i == 0)
		{
			HC_Compute_Coordinates_By_Path(kcount, keys, "object", &min, "world", &min);
			HC_Compute_Coordinates_By_Path(kcount, keys, "object", &max, "world", &max);
		}
		HC_Compute_Coordinates_By_Path(kcount, keys, "object", &newmin, "world", &newmin);
		HC_Compute_Coordinates_By_Path(kcount, keys, "object", &newmax, "world", &newmax);

		if (newmin.x < min.x)
			min.x = newmin.x;
		if (newmin.y < min.y)
			min.y = newmin.y;
		if (newmin.z < min.z)
			min.z = newmin.z;
		if (newmax.x > max.x)
			max.x = newmax.x;
		if (newmax.y > max.y)
			max.y = newmax.y;
		if (newmax.z > max.z)
			max.z = newmax.z;
	}

	HPoint center;
	HPoint camera;
	HPoint target;
	HPoint viewingVector;
	HPoint diagonalVector;
	float diagonalLength;
	float tmp;

	if (min.x > max.x) {
		tmp = min.x; min.x = max.x; max.x = tmp;
	}
	if (min.y > max.y) {
		tmp = min.y; min.y = max.y; max.y = tmp;
	}
	if (min.z > max.z) {
		tmp = min.z; min.z = max.z; max.z = tmp;
	}

	HC_Open_Segment_By_Key(m_SceneKey);{
		HPoint co,to,uo,cn,tn,un;
		float wo, ho, wn, hn;
		char proj[MVO_BUFFER_SIZE];

		HC_PShow_Net_Camera(0, 0, &co, &to, &uo, &wo, &ho, proj);
		
		// compute the length of the bbox diagonal vector
		// this will be used to make the camera field a bit larger
		// to ensure that objects between the camera position and target
		// will still not get clipped (this is for perspective viewing mode)
		diagonalVector.Set(max.x - min.x, max.y - min.y, max.z - min.z);

		diagonalLength = (float) sqrt(
			diagonalVector.x * diagonalVector.x +
			diagonalVector.y * diagonalVector.y +
			diagonalVector.z * diagonalVector.z);
		if(diagonalLength < 0.001f)
			diagonalLength = 0.001f;

		center.x = 0.5F * (min.x + max.x);
		center.y = 0.5F * (min.y + max.y);
		center.z = 0.5F * (min.z + max.z);

		HC_PShow_Net_Camera_Position(0, 0, &camera.x, &camera.y, &camera.z);
		HC_PShow_Net_Camera_Target(0, 0, &target.x, &target.y, &target.z);

		viewingVector.Set(target.x - camera.x, target.y - camera.y,	target.z - camera.z);
		HC_Compute_Normalized_Vector (&viewingVector, &viewingVector);

		// reset camera position to lie on the viewing vector, at a distance of
		// 2.5 * diagonal length away from the center of the selection centroid
		HC_Set_Camera_Position(center.x - 2.5f * viewingVector.x * diagonalLength,
							   center.y - 2.5f * viewingVector.y * diagonalLength,
							   center.z - 2.5f * viewingVector.z * diagonalLength);
		HC_Set_Camera_Target (center.x, center.y, center.z);
		HC_Set_Camera_Field (diagonalLength, diagonalLength);
		HC_PShow_Net_Camera (0, 0, &cn,&tn,&un,&wn,&hn, proj);
//		HC_Move_Distant_Light(GetDefaultLightKey(), cn.x, cn.y, cn.z);

		if (normal.x != 0 || normal.y != 0 || normal.z != 0)
		{
			HPoint temp1;
			temp1.Set(cn.x-tn.x, cn.y-tn.y, cn.z-tn.z);
			float length = (float)HC_Compute_Vector_Length(&temp1);

			cn.x = tn.x+normal.x * length;
			cn.y = tn.y+normal.y * length;
			cn.z = tn.z+normal.z * length;
			HC_Set_Camera_Position(cn.x, cn.y, cn.z);

			HPoint p1, p2;
			p1.Set(cn.x-tn.x, cn.y-tn.y, cn.z-tn.z);
			p2.Set(co.x-to.x, co.y-to.y, co.z-to.z);

			float mat[16];
			HUtility::ComputeVectorToVectorRotationMatrix(mat, p1, p2);

			HPoint un2;
			un2.Set(un.x, un.y, un.z);
			HC_Compute_Transformed_Points(1, &uo, mat, &un);
			HC_Set_Camera_Up_Vector(un.x, un.y, un.z);
		}

		if (GetSmoothTransition())
			HUtility::SmoothTransition(co,to,uo, wo, ho,  cn,tn, un, wn, hn, this);

	}HC_Close_Segment();

	CameraPositionChanged(true, GetSmoothTransition());
}


void HBaseView::Update()
{
	if (GetViewActive() && GetModel()->GetFileLoadComplete())
		UpdateInternal();
}

void HBaseView::RefreshViewSize()
{
	char tmp[MVO_BUFFER_SIZE];

	int nx=0;
	int ny=0;

	if (m_XSize_override != 0 || m_YSize_override != 0) {

		nx = m_XSize_override;
		ny = m_YSize_override;
		m_XSize_override = 0;
		m_YSize_override = 0;

	}
	else {

		HC_Show_Device_Info_By_Key(GetViewKey(), "pixels", tmp);

		if (tmp[0] != '\0') {

			sscanf( tmp, "%d,%d", &nx, &ny);
		}
	}


	if (m_XSize != nx || m_YSize != ny) {
		
		m_XSize = nx;
		m_YSize = ny;
		
		if(m_SignalRequestsList != 0)
			Notify( HSignalResize );
	}

}

void HBaseView::UpdateInternal(bool antialias, bool force_update)
{
	m_UpdateInterrupted = false;

	if (m_InUpdateInternal)
		return;

	m_InUpdateInternal = true;

	if (!GetSuppressUpdate())
		RefreshViewSize();

FORCED:

	if (GetSuppressUpdate() || 
		((GetViewUpdateMode() == Continuous || GetViewUpdateMode() == Deferred)
			&& !force_update
		))
	{
		RequestUpdate();

		if (!GetSuppressUpdate() &&
			!GetModel()->GetFileLoadComplete()) {
			force_update = true;
			goto FORCED;
		}
	}
	else {
		if (GetViewUpdateMode() == OnDemand || 
			(GetViewUpdateMode() == Continuous && GetUpdateRequested()) ||
				(GetViewUpdateMode() == Deferred && (GetUpdateRequested() ||
					GetUpdateNotFinished())))
		{
			if (GetViewUpdateMode() == Deferred && !GetUpdateRequested() && GetUpdateNotFinished())
			{
				m_NotFinishedCounter++;
				if (m_NotFinishedCounter<10)
				{
					m_InUpdateInternal = false;
					return;
				}
			}
			if (GetViewUpdateMode() == Deferred && GetUpdateRequested())
				m_NotFinishedCounter = 0;


			char refl[MVO_BUFFER_SIZE];
			bool bReflection = false;
			
			HC_Open_Segment_By_Key(GetSceneKey());
				HC_Show_One_Net_Rendering_Option("simple reflection", refl);
				HC_Parse_String(refl, ",", 0, refl);
				if(streq(refl, "on"))
					bReflection=true;
			HC_Close_Segment();
			

			// in Interactive Shadow Update mode or Reflection Planes,
			// recalculate shadow plane
			// to keep objects from moving through their shadows
			if (GeometryChanged() &&
				(bReflection || 
				(GetShadowMode() != HShadowNone &&
				HC_Show_Existence_By_Key(GetSceneKey(),"rendering options=(simple shadow)"))) ) {
				HPlane shadow_pl;
				HPoint light_pos;

				char plane_buf[MVO_BUFFER_SIZE];
				char light_buf[MVO_BUFFER_SIZE];

				HC_Open_Segment_By_Key( GetSceneKey());
					HC_Show_One_Rendering_Option("simple shadow=(plane)", plane_buf);
					HC_Show_One_Rendering_Option("simple shadow=(light)", light_buf);
				HC_Close_Segment();

				// if there is already an existing shadow then we need to adjust
				// the plane otherwise we need to create one and re-orient it
				if (plane_buf[0] != '\0' && light_buf[0] != '\0')
				{
					int count;

					HCLOCALE (count = sscanf( light_buf, "%f,%f,%f", &light_pos.x, &light_pos.y, &light_pos.z ));
					if (count == 3) {
						AdjustShadowPlane( shadow_pl, light_pos );
						// to only allow shadow to move downward, you could restrict this 
						//		following line to if (shadow_pl.d < old_shadow_pl.d)
						SetShadowParams( shadow_pl, light_pos , 0, 0);
					}
					else
						HDB::ReportError ("internal error: unexpected case in HBaseView::UpdateInternal");
				}
			}
			
			if (m_bAxisMode != AxisOff)
				RotateOnUpdate("axis");
			
			if (m_bNavCube)
				RotateOnUpdate("navcube");

			
			// if Constant Frame Rate is enabled AND it is activated  (e.g. by an operator)
			// then the framerate watch routine is called
			if (m_pConstantFrameRate->GetMode() && m_pConstantFrameRate->GetActive()) {
				m_pConstantFrameRate->Watch();
			}
			else {
				if (antialias) {
					InitAntialiasing();
					m_pAntialiasing->Show();
					InternalUpdate();
					m_pAntialiasing->Hide();
				}
				InternalUpdate();
				HErrorManager::SpoolUpdateErrors();

			}
			if( m_pDebugZBuffer )
				m_pDebugZBuffer->Update();
			ClearGeometryChanged();
			ClearUpdateRequest();  
		}
	}


	// if this is the first update, query the capabilities of the device
	// and update the various render settings accordingly
	if (!m_bFirstUpdate && !GetSuppressUpdate())  
	{
		m_bFirstUpdate = true;
		RefreshViewSize();
		QueryDeviceCapabilities();
	}

	m_InUpdateInternal = false;
}


void HBaseView::ForceUpdate()
{
	RequestUpdate();
	UpdateInternal(false, true);
}


void HBaseView::InternalUpdate()
{
	if (!GetSuppressUpdate()) {

		if (!GetShadowRegenerationMode()) {
			HC_Control_Update_By_Key(GetViewKey(), "unset shadow regeneration");
			HC_Control_Update_By_Key(GetModelKey(), "unset shadow regeneration");
		}

		if (!GetCuttingPlaneRegenerationMode()) {
			HC_Control_Update_By_Key(GetViewKey(), "unset cut geometry regeneration");
			HC_Control_Update_By_Key(GetModelKey(), "unset cut geometry regeneration");
		}

		//in (future) threadsafe version we have to have another case conditional on bool HDB::threaded that handles isolated drivers.  reexamine the other calls to update.
		if (m_MaximumUpdateTime <= 0.0001f)
		{
			if (HDB::GetIsolatedDrivers())
				HC_Update_One_Display(GetDriverPath());
			else
				HC_Update_Display();
			SetUpdateNotFinished(false);
		}
		else 
		{
			if (HDB::GetIsolatedDrivers())
				m_UpdateInterrupted = !HC_Update_One_Display_Timed(GetDriverPath(), m_MaximumUpdateTime);
			else
				m_UpdateInterrupted = !HC_Update_Display_Timed(m_MaximumUpdateTime);
			
			if (GetViewUpdateMode() == Deferred)
				SetUpdateNotFinished(bool2BOOL(m_UpdateInterrupted));
		}

		m_bCameraChanged = false;

		if (m_bShadowRegenerationToggle)
		{
			m_bShadowRegenerationToggle = false;
			m_bShadowRegeneration = true;
		}

	}
}


void HBaseView::RenderAntialiasing()
{
	InitAntialiasing();

	m_pAntialiasing->Show();
	InternalUpdate();
	m_pAntialiasing->Hide();
}

void HBaseView::RenderWireframe()
{
	SetRenderMode(HRenderWireframe, true);
}


void HBaseView::RenderHiddenLineFast()
{
	SetRenderMode(HRenderHiddenLineFast, true);
}


void HBaseView::RenderHiddenLine()
{
	SetRenderMode(HRenderHiddenLine, true);
}

void HBaseView::RenderBRepHiddenLine()
{
	SetRenderMode(HRenderBRepHiddenLine, true);
}

void HBaseView::RenderBRepHiddenLineFast()
{
	SetRenderMode(HRenderBRepHiddenLineFast, true);
}

void HBaseView::RenderBRepWireframe()
{
	SetRenderMode(HRenderBRepWireframe, true);
}

void HBaseView::RenderFakeHiddenLine()
{
	SetRenderMode(HRenderFakeHiddenLine, true);
}

void HBaseView::RenderSilhouette()
{
	SetRenderMode(HRenderSilhouette, true);
}

void HBaseView::RenderGooch()
{
	SetRenderMode(HRenderGoochShaded, true);
}

void HBaseView::RenderFlat()
{
	SetRenderMode(HRenderFlat, true);
}



void HBaseView::RenderGouraud()
{
	SetRenderMode(HRenderGouraud, true);
}



void HBaseView::RenderPhong()
{
	SetRenderMode(HRenderPhong, true);
}



void HBaseView::RenderShaded()
{
	SetRenderMode(HRenderShaded, true);
}

void HBaseView::RenderShadedWithLines()
{
	SetRenderMode(HRenderShadedWithLines, true);
}
void HBaseView::RenderGouraudWithLines()
{
	SetRenderMode(HRenderGouraudWithLines, true);
}

void HBaseView::RenderLOD1()
{
	SetRenderMode(HRenderLOD1, true);
}

void HBaseView::RenderLOD2()
{
	SetRenderMode(HRenderLOD2, true);
}


void HBaseView::RenderWireframeWithSilhouette()
{
	SetRenderMode(HRenderWireframeWithSilhouette, true);
}
void HBaseView::RenderShadedWireframe()
{
	SetRenderMode(HRenderShadedWireframe, true);
}
void HBaseView::RenderVertices()
{
	SetRenderMode(HRenderVertices, true);
}
void HBaseView::RenderShadedVertices()
{
	SetRenderMode(HRenderShadedVertices, true);
}


void HBaseView::SetCPGeomVisibility(bool lines, bool faces)
{
	m_bCPLineVisibility = lines;
	m_bCPFaceVisibility = faces;

	HC_Open_Segment_By_Key(m_SceneKey);
		if (lines)
			HC_Set_Visibility("cut geometry = (edges=on)");
		else 
			HC_Set_Visibility("cut geometry = (edges=off)");

		if (faces)
			HC_Set_Visibility("cut geometry = (faces=on)");
		else 
			HC_Set_Visibility("cut geometry = (faces=off)");
	HC_Close_Segment();

	// If capping faces are being turned off, we should also turn off 
	// backplane culling so that the inside can be seen
	if( !faces )
	{
		if( GetCuttingPlanesExist() && GetBackplaneCulling() )
		{
			m_bPrevBackplaneCullMode = true;	// cache the cull mode
			SetBackplaneCulling(false);
		}
	}
	else if( m_bPrevBackplaneCullMode == true )
	{
		// now that capping faces are back restore the backplane cull mode
		m_bPrevBackplaneCullMode = false;
		SetBackplaneCulling(true);
	}
}

void HBaseView::SetHideOverlappedText(bool hide) 
{
	m_bHideOverlappedText = hide;

	HC_Open_Segment_By_Key(GetSceneKey());{
		if(m_bHideOverlappedText) {
			HImHideOverlappedText::FilterOn();
		}else{
			HImHideOverlappedText::FilterOff();
		}
	}HC_Close_Segment();
}

DisplayListType	HBaseView::GetDisplayListType()
{
	return m_DisplayListType;
}

void HBaseView::SetDisplayListType(DisplayListType type)
{
	m_DisplayListType = type;
	HC_Open_Segment_By_Key(m_ViewKey);

	if (type == DisplayListOff)
		HC_Set_Rendering_Options("no display lists");
	else if (type == DisplayListGeometry)
		HC_Set_Rendering_Options("display lists = geometry");
	else if(type == DisplayListSegment)
		HC_Set_Rendering_Options("display lists = segment");

	HC_Close_Segment();

}

bool HBaseView::GetDisplayListMode()
{
	char cval[MVO_BUFFER_SIZE];

	HC_Open_Segment_By_Key(m_ViewKey);{
		HC_PShow_One_Net_Rendering_Option(0, 0, "display lists", cval);
	}HC_Close_Segment();

	if (m_DisplayListType == DisplayListGeometry)
	{
		if(strstr(cval,"geometry") || strstr(cval,"on"))
			return true;
		else
			return false;
	}
	else if(m_DisplayListType == DisplayListSegment)
	{
		if(strstr(cval,"segment"))
			return true;
		else
			return false;
	}
	else
		return false;
}

void HBaseView::SetDisplayListMode(bool mode)
{
	HC_Open_Segment_By_Key(m_ViewKey);{
		if (mode)
		{
			if (m_DisplayListType == DisplayListGeometry)
				HC_Set_Rendering_Options("display lists = geometry");
			else if(m_DisplayListType == DisplayListSegment)
				HC_Set_Rendering_Options("display lists = segment");
		}
		else
			HC_Set_Rendering_Options("no display lists");
	}HC_Close_Segment();
}

void HBaseView::SetOcclusionCullingMode(bool onoff, bool UseOctree, int threshold)
{
	char heur[MVO_BUFFER_SIZE];

	HC_Open_Segment_By_Key(m_SceneKey);
		if (onoff)
		{
			sprintf(heur, "culling = (view frustum, obscuration = (use octree = %s, pixel threshold = %d))",
				(UseOctree ? "true" : "false"), threshold);
			HC_Set_Heuristics(heur);
		}
		else
			HC_Set_Heuristics("culling = (view frustum, no obscuration)");
	HC_Close_Segment();
}


HandednessMode  HBaseView::GetHandedness()
{
	char cval[MVO_BUFFER_SIZE];

	HC_Open_Segment_By_Key(m_SceneKey);{
		HC_PShow_Net_Handedness(0, 0, cval);
	}HC_Close_Segment();

	if(strstr(cval, "right") != 0)
		return HandednessRight;
	else
		return HandednessLeft;
}


void HBaseView::SetHandedness(HandednessMode mode, bool emit_message)
{
	char const *handedness;

	if (mode == HandednessLeft)
		handedness = "left" ;
	else if (mode == HandednessRight)
		handedness = "right" ;
	else {
		HDB::ReportError("The handedness of the world coordinate system must be set to left or right.");
		return;
	}

	// first set it in the scene
	HC_Open_Segment_By_Key(m_SceneKey);
		HC_Set_Handedness(handedness);	
	HC_Close_Segment();

	// and then in the axis triad
	HC_Open_Segment_By_Key(m_AxisTriadKey);
		HC_Set_Handedness(handedness);	
	HC_Close_Segment();

	if (emit_message){
		if(emit_message_function) {
			char message[MVO_BUFFER_SIZE];

			sprintf(message, "H_SET_HANDEDNESS %s", handedness);
			emit_message_function((const char *)message, 
				0, emit_message_function_user_data);
		}
	}
}


void HBaseView::SetHandednessFromMessage(const char *in_data, unsigned int data_length)
{
	char handedness[MVO_BUFFER_SIZE];
	char data[MVO_MESSAGE_BUFFER_SIZE];

	memcpy(data, in_data, data_length);
	data[data_length] = '\0';

	sscanf(data, "%s", handedness);

	if (streq(handedness, "left"))
		SetHandedness(HandednessLeft);
	else if (streq(handedness, "right"))
		SetHandedness(HandednessRight);
}



HandednessMode  HBaseView::GetPolygonHandednessMode()
{
	char cval[MVO_BUFFER_SIZE];

	HC_Open_Segment_By_Key(m_SceneKey);{
		HC_PShow_Net_Heuristics(0, 0, cval);
	}HC_Close_Segment();

	if(strstr(cval, "polygon handedness=left") != 0)
		return HandednessLeft;

	if(strstr(cval, "polygon handedness=right") != 0)
		return HandednessRight;

	return HandednessNone;
}


void HBaseView::SetPolygonHandednessMode(HandednessMode mode)
{
	HC_Open_Segment_By_Key(m_SceneKey);{
		if(mode == HandednessLeft)
			HC_Set_Heuristics("polygon handedness=left");
		if(mode == HandednessRight)
			HC_Set_Heuristics("polygon handedness=right");
		if(mode == HandednessNone)
			HC_Set_Heuristics("no polygon handedness");
	}HC_Close_Segment();
}

bool HBaseView::GetBackplaneCulling()
{
	char cval[MVO_BUFFER_SIZE];

	HC_Open_Segment_By_Key(m_SceneKey);{
		HC_Show_One_Net_Heuristic("backplane cull", cval);
	}HC_Close_Segment();

	if (strstr(cval,"on"))
		return true;
	else
		return false;
}


void HBaseView::SetBackplaneCulling(bool cull)
{
	HC_Open_Segment_By_Key(m_SceneKey);{
		if (cull)
			HC_Set_Heuristics("backplane cull = on");
		else
			HC_Set_Heuristics("backplane cull = off");
	}HC_Close_Segment();
}



void HBaseView::SetLodMode(bool onoff)
{
	HC_Open_Segment_By_Key(m_SceneKey);{
		if (onoff)
			HC_Set_Rendering_Options("lod, lodo=(preprocess)");
		else
			HC_Set_Rendering_Options("no lod");
	}HC_Close_Segment();
}

bool HBaseView::GetLodMode()
{
	char cval[MVO_BUFFER_SIZE];

	HC_Open_Segment_By_Key(m_SceneKey);{
		HC_PShow_One_Net_Rendering_Option(0, 0, "lod", cval);
	}HC_Close_Segment();

	if(strstr(cval,"on"))
		return true;
	else
		return false;

}

int HBaseView::GetLodThreshold()
{
	char cval[MVO_BUFFER_SIZE];

	HC_Open_Segment_By_Key(m_SceneKey);{
		HC_PShow_One_Net_Rendering_Option(0, 0, "threshold", cval);
	}HC_Close_Segment();

	return atoi(cval);
}

void HBaseView::SetLodThreshold(int val)
{
	char cval[MVO_BUFFER_SIZE];

	sprintf(cval, "lodo =(threshold = %d)", val);

	HC_Open_Segment_By_Key(m_SceneKey);{
		HC_Set_Rendering_Options( cval );
	}HC_Close_Segment();
}


void HBaseView::SetProjMode(ProjMode mode){

	// don't set if we are already in same mode
	if( GetProjMode() == mode )
		return;

	char val[MVO_BUFFER_SIZE];

	HC_Open_Segment_By_Key(m_SceneKey);{
		if      ( mode == ProjPerspective)  strcpy(val, "perspective");
		else if ( mode == ProjOrthographic) strcpy(val, "orthographic");
		else if ( mode == ProjStretched)    strcpy(val, "stretched");
		else if ( mode == ProjUnknown)      strcpy(val, "");

		HC_Set_Camera_Projection(val);
	}HC_Close_Segment();
	CameraPositionChanged(true);
}


ProjMode HBaseView::GetProjMode()
{
	char  val[MVO_BUFFER_SIZE];
	ProjMode mode;

	HC_Open_Segment_By_Key(m_SceneKey);{
		HC_PShow_Net_Camera_Projection(0, 0, val);

		if      ( !strcmp(val, "perspective"))          mode = ProjPerspective;
		else if ( !strcmp(val, "orthographic"))         mode = ProjOrthographic;
		else if ( !strcmp(val, "stretched"))            mode = ProjStretched;
		else                                            mode = ProjUnknown;

	}HC_Close_Segment();

	return mode;
}



void HBaseView::SetViewMode (ViewMode mode, bool invert, bool fitWorld)
{
	HPoint target, camera, view;
	float length;

	HPoint co, to, uo;
	float widtho, heighto;
	char projection[MVO_BUFFER_SIZE];


	if (mode == HViewIso)
		mode = GetIsoDefault();
	HC_Open_Segment_By_Key(GetSceneKey());{
		HC_PShow_Net_Camera(0, 0, &co, &to, &uo, &widtho, &heighto, projection);
	}HC_Close_Segment();

	ViewMode OldViewMode = m_ViewMode;
	m_ViewMode = mode;

	if (mode == HViewUnknown) 
		return;
	PrepareForCameraChange();
	if (fitWorld)
		FitWorld();

	HC_Open_Segment_By_Key(GetSceneKey());
	
	HC_PShow_Net_Camera_Target(0, 0, &target.x, &target.y, &target.z);
	HC_PShow_Net_Camera_Position(0, 0, &camera.x, &camera.y, &camera.z);

	view.Set(camera.x - target.x,
			camera.y - target.y,
			camera.z - target.z);

	length = (float) HC_Compute_Vector_Length( &view);


	if (m_ViewMode == OldViewMode && invert)
	{
		if (m_ViewMode == HViewXY)
			m_ViewMode = HViewYX;
		else if (m_ViewMode == HViewYX)
			m_ViewMode = HViewXY;
		else if (m_ViewMode == HViewXZ)
			m_ViewMode = HViewZX;
		else if (m_ViewMode == HViewZX)
			m_ViewMode = HViewXZ;
		else if (m_ViewMode == HViewYZ)
			m_ViewMode = HViewZY;
		else if (m_ViewMode == HViewZY)
			m_ViewMode = HViewYZ;

		if (m_ViewMode == HViewFront)
			m_ViewMode = HViewBack;
		else if (m_ViewMode == HViewBack)
			m_ViewMode = HViewFront;
		else if (m_ViewMode == HViewLeft)
			m_ViewMode = HViewRight;
		else if (m_ViewMode == HViewRight)
			m_ViewMode = HViewLeft;
		else if (m_ViewMode == HViewTop)
			m_ViewMode = HViewBottom;
		else if (m_ViewMode == HViewBottom)
			m_ViewMode = HViewTop;
	}

	float newLen = length * 0.5774f;


	HPoint rightaxis;
	HC_Compute_Cross_Product(&m_FrontAxis, &m_TopAxis, &rightaxis);

	if (GetHandedness() == HandednessRight)
		rightaxis.Set(-rightaxis.x, -rightaxis.y, -rightaxis.z);

	switch(m_ViewMode){

	case HViewFront:
		camera.Set(target.x + length * m_FrontAxis.x, target.y + length * m_FrontAxis.y,
			target.z + length * m_FrontAxis.z);
		HC_Set_Camera_Position(camera.x, camera.y, camera.z);
		HC_Set_Camera_Up_Vector(m_TopAxis.x, m_TopAxis.y, m_TopAxis.z);

		break;
	case HViewBack:
		camera.Set(target.x - length * m_FrontAxis.x, target.y - length * m_FrontAxis.y,
			target.z - length * m_FrontAxis.z);
		HC_Set_Camera_Position(camera.x, camera.y, camera.z);
		HC_Set_Camera_Up_Vector(m_TopAxis.x, m_TopAxis.y, m_TopAxis.z);

		break;
	case HViewRight:
		camera.Set(target.x + length * rightaxis.x, target.y + length * rightaxis.y,
			target.z + length * rightaxis.z);
		HC_Set_Camera_Position(camera.x, camera.y, camera.z);
		HC_Set_Camera_Up_Vector(m_TopAxis.x, m_TopAxis.y, m_TopAxis.z);

		break;
	case HViewLeft:
		camera.Set(target.x - length * rightaxis.x, target.y - length * rightaxis.y,
			target.z - length * rightaxis.z);
		HC_Set_Camera_Position(camera.x, camera.y, camera.z);
		HC_Set_Camera_Up_Vector(m_TopAxis.x, m_TopAxis.y, m_TopAxis.z);

		break;
	case HViewTop:
		camera.Set(target.x + length * m_TopAxis.x, target.y + length * m_TopAxis.y,
			target.z + length * m_TopAxis.z);
		HC_Set_Camera_Position(camera.x, camera.y, camera.z);
		HC_Set_Camera_Up_Vector(m_FrontAxis.x, m_FrontAxis.y, m_FrontAxis.z);

		break;
	case HViewBottom:
		camera.Set(target.x - length * m_TopAxis.x, target.y - length * m_TopAxis.y,
			target.z - length * m_TopAxis.z);
		HC_Set_Camera_Position(camera.x, camera.y, camera.z);
		HC_Set_Camera_Up_Vector(m_FrontAxis.x, m_FrontAxis.y, m_FrontAxis.z);

		break;

	case HViewXY:

		if (GetHandedness() == HandednessRight)
			camera.Set(target.x, target.y, target.z + length);
		else
			camera.Set(target.x, target.y, target.z - length);

		HC_Set_Camera_Position(camera.x, camera.y, camera.z);
		HC_Set_Camera_Up_Vector(0.0, 1.0, 0.0);
		break;

	case HViewYX:
		
		if (GetHandedness() == HandednessRight)
			camera.Set(target.x, target.y, target.z - length);
		else
			camera.Set(target.x, target.y, target.z + length);

		HC_Set_Camera_Position(camera.x, camera.y, camera.z);
		HC_Set_Camera_Up_Vector(0.0, 1.0, 0.0);
		
		break;
	case HViewXZ:

		if (GetHandedness() == HandednessRight)
			camera.Set(target.x, target.y - length , target.z);
		else
			camera.Set(target.x, target.y + length, target.z);
		
		HC_Set_Camera_Position(camera.x, camera.y, camera.z);
		HC_Set_Camera_Up_Vector(0.0, 0.0, 1.0);

		break;
	case HViewZX:

		if (GetHandedness() == HandednessRight)
			camera.Set(target.x, target.y + length , target.z);
		else
			camera.Set(target.x, target.y - length, target.z);

		HC_Set_Camera_Position(camera.x, camera.y, camera.z);
		HC_Set_Camera_Up_Vector(0.0, 0.0, 1.0);

		break;
	case HViewYZ:

		if (GetHandedness() == HandednessRight)
			camera.Set(target.x + length, target.y, target.z);
		else
			camera.Set(target.x - length, target.y , target.z);

		HC_Set_Camera_Position(camera.x, camera.y, camera.z);
		HC_Set_Camera_Up_Vector(0.0, 1.0, 0.0);
	
		break;
	case HViewZY:

		if (GetHandedness() == HandednessRight)
			camera.Set(target.x - length, target.y, target.z);
		else
			camera.Set(target.x + length, target.y , target.z);

		HC_Set_Camera_Position(camera.x, camera.y, camera.z);
		HC_Set_Camera_Up_Vector(0.0, 1.0, 0.0);
	
		break;
	case HViewIso:
	case HViewIsoFrontRightTop:
	case HViewSEIso:
		{
			camera.Set(target.x + newLen * m_FrontAxis.x +
				newLen * rightaxis.x + newLen * m_TopAxis.x,
			target.y + newLen * m_FrontAxis.y +
				newLen * rightaxis.y + newLen * m_TopAxis.y,
			target.z+ newLen * m_FrontAxis.z +
				newLen * rightaxis.z + newLen * m_TopAxis.z);
			HC_Set_Camera_Position(camera.x, camera.y, camera.z);
			HC_Set_Camera_Up_Vector(m_TopAxis.x, m_TopAxis.y, m_TopAxis.z);
		}
		break;
	case HViewIsoFrontLeftTop:
	case HViewSWIso:
		{
			camera.Set(target.x + newLen * m_FrontAxis.x -
				newLen * rightaxis.x + newLen * m_TopAxis.x,
			target.y + newLen * m_FrontAxis.y -
				newLen * rightaxis.y + newLen * m_TopAxis.y,
			target.z+ newLen * m_FrontAxis.z -
				newLen * rightaxis.z + newLen * m_TopAxis.z);
			HC_Set_Camera_Position(camera.x, camera.y, camera.z);
			HC_Set_Camera_Up_Vector(m_TopAxis.x, m_TopAxis.y, m_TopAxis.z);
		}
		break;
	case HViewIsoFrontLeftBottom:
		{
			camera.Set(target.x + newLen * m_FrontAxis.x -
				newLen * rightaxis.x - newLen * m_TopAxis.x,
			target.y + newLen * m_FrontAxis.y -
				newLen * rightaxis.y - newLen * m_TopAxis.y,
			target.z+ newLen * m_FrontAxis.z -
				newLen * rightaxis.z - newLen * m_TopAxis.z);
			HC_Set_Camera_Position(camera.x, camera.y, camera.z);
			HC_Set_Camera_Up_Vector(m_TopAxis.x, m_TopAxis.y, m_TopAxis.z);
		}
		break;
	case HViewIsoFrontRightBottom:
		{
			camera.Set(target.x + newLen * m_FrontAxis.x +
				newLen * rightaxis.x - newLen * m_TopAxis.x,
			target.y + newLen * m_FrontAxis.y +
				newLen * rightaxis.y - newLen * m_TopAxis.y,
			target.z+ newLen * m_FrontAxis.z +
				newLen * rightaxis.z - newLen * m_TopAxis.z);
			HC_Set_Camera_Position(camera.x, camera.y, camera.z);
			HC_Set_Camera_Up_Vector(m_TopAxis.x, m_TopAxis.y, m_TopAxis.z);
		}
		break;
	case HViewIsoBackRightTop:
	case HViewNEIso:
		{
			camera.Set(target.x - newLen * m_FrontAxis.x +
				newLen * rightaxis.x + newLen * m_TopAxis.x,
			target.y - newLen * m_FrontAxis.y +
				newLen * rightaxis.y + newLen * m_TopAxis.y,
			target.z- newLen * m_FrontAxis.z +
				newLen * rightaxis.z + newLen * m_TopAxis.z);
			HC_Set_Camera_Position(camera.x, camera.y, camera.z);
			HC_Set_Camera_Up_Vector(m_TopAxis.x, m_TopAxis.y, m_TopAxis.z);
		}
		break;
	case HViewIsoBackLeftTop:
	case HViewNWIso:
		{
			camera.Set(target.x - newLen * m_FrontAxis.x -
				newLen * rightaxis.x + newLen * m_TopAxis.x,
			target.y - newLen * m_FrontAxis.y -
				newLen * rightaxis.y + newLen * m_TopAxis.y,
			target.z- newLen * m_FrontAxis.z -
				newLen * rightaxis.z + newLen * m_TopAxis.z);
			HC_Set_Camera_Position(camera.x, camera.y, camera.z);
			HC_Set_Camera_Up_Vector(m_TopAxis.x, m_TopAxis.y, m_TopAxis.z);
		}
		break;
	case HViewIsoBackLeftBottom:
		{
			camera.Set(target.x - newLen * m_FrontAxis.x -
				newLen * rightaxis.x - newLen * m_TopAxis.x,
			target.y - newLen * m_FrontAxis.y -
				newLen * rightaxis.y - newLen * m_TopAxis.y,
			target.z- newLen * m_FrontAxis.z -
				newLen * rightaxis.z - newLen * m_TopAxis.z);
			HC_Set_Camera_Position(camera.x, camera.y, camera.z);
			HC_Set_Camera_Up_Vector(m_TopAxis.x, m_TopAxis.y, m_TopAxis.z);
		}
		break;
	case HViewIsoBackRightBottom:
		{
			camera.Set(target.x - newLen * m_FrontAxis.x +
				newLen * rightaxis.x - newLen * m_TopAxis.x,
			target.y - newLen * m_FrontAxis.y +
				newLen * rightaxis.y - newLen * m_TopAxis.y,
			target.z- newLen * m_FrontAxis.z +
				newLen * rightaxis.z - newLen * m_TopAxis.z);
			HC_Set_Camera_Position(camera.x, camera.y, camera.z);
			HC_Set_Camera_Up_Vector(m_TopAxis.x, m_TopAxis.y, m_TopAxis.z);
		}
		break;

	default:
		assert(0);
		break;
	}

	if (GetSmoothTransition())
	{
		HPoint cn, tn, un;
		float widthn, heightn;
		char projection[MVO_BUFFER_SIZE];

		HC_Show_Net_Camera (&cn, &tn, &un, &widthn, &heightn, projection);

		HUtility::SmoothTransition(co, to, uo, widtho, heighto, cn, tn, un, widthn, heightn, this);	   
	}

	CameraPositionChanged(true, GetSmoothTransition());
	Update();
	HC_Close_Segment();
}

void HBaseView::DeleteSelectionList(bool emit_message)
{
	HSelectionSet	*selection = GetSelection();
	int				sel_count;
	char			type[MVO_BUFFER_SIZE];
	HC_KEY			tmp;

	if( emit_message )
		EmitDeleteSelectionListMessage();

	if(selection)
		sel_count = selection->GetSize();
	else
		return;

	std::vector<HC_KEY> keylist(sel_count);
	selection->GetAll(keylist.data());

	// dis-associate the shared keys
	for(int i = 0; i < sel_count; i++)
	{
		HC_KEY key = keylist[i];

		if (m_pSharedKey)
		{
			HC_Show_Key_Type(key,type);
			if (strcmp(type, "segment") == 0)
			{
				HC_Open_Segment_By_Key(key);
				m_pSharedKey->DisAssociateKeysInSegment();
				HC_Close_Segment();
			}
			else
				m_pSharedKey->DisAssociateKey(key);

		}
	}

	// NOTE: we have already emitted the message
	selection->DeleteSelection(false);
	m_bShadowRegeneration = true;

	HC_Open_Segment_By_Key(m_CuttingPlanesKey);
		HC_Begin_Contents_Search ("...", "cutting planes");
		if (!HC_Find_Contents (type, &tmp)) 
			SetCuttingPlanesExist(false); 
		HC_End_Contents_Search ();
	HC_Close_Segment();

	SetGeometryChanged();


	Update();
}


HQuickMovesMethod HBaseView::GetQuickMovesMethod() const
{
	char cval[MVO_BUFFER_SIZE];
 
	HC_Open_Segment_By_Key(m_ViewKey);
		HC_Show_Device_Info(".", "quick moves method", cval);
	HC_Close_Segment();

	if (streq(cval,"xor"))
		return HQuickMoves_XOR;
	else if (streq(cval,"overlay"))
		return HQuickMoves_Overlay;
	else if (streq(cval,"spriting"))
		return HQuickMoves_Spriting;
	else
		return HQuickMoves_None;
}

void HBaseView::SetWindowMaterial(const char * name, bool emit_message)
{
	char color[MVO_BUFFER_SIZE];

	if (emit_message){
		if(emit_message_function) {
			char message[MVO_BUFFER_SIZE];
			HCLOCALE(sprintf(message, "H_SET_WINDOW_MATERIAL %s", name));
			emit_message_function((const char *)message, 
				0, emit_message_function_user_data);
		}
	}

	HCLOCALE(sprintf(color, "windows=%s", name));
	HC_Open_Segment_By_Key(GetViewKey());
		HC_Set_Color(color);
		HC_Set_Window_Pattern("solid");
		HImUnSetOneCallback("draw window");
	HC_Close_Segment();
}

void HBaseView::GetWindowMaterial(char * out_material, char * out_pattern)
{
	char temp[MVO_BUFFER_SIZE];
	out_material[0] = '\0';
	out_pattern[0] = '\0';

	HC_Open_Segment_By_Key(GetViewKey());
	{
		if (HC_Show_Existence("color = windows"))
		{
			HC_Show_One_Color("windows", temp);
			strcat(out_material, H_FORMAT_TEXT("windows = %s,", temp));
		}
		if (HC_Show_Existence("color = window contrast"))
		{
			HC_Show_One_Color("window contrast", temp);
			strcat(out_material, H_FORMAT_TEXT("window contrast = %s,", temp));
		}
		if (HC_Show_Existence("window pattern"))
		{
			HC_Show_Window_Pattern(out_pattern);
		}
	}
	HC_Close_Segment();

	out_material[strlen(out_material) - 1] = '\0';
}

void HBaseView::GetWindowColor(HPoint &color_out)
{
	char colorSpace[MVO_BUFFER_SIZE];

	HC_Open_Segment_By_Key(GetViewKey());{
		if (HC_Show_Existence("color = windows")) {
			HC_Show_One_Color_By_Value(
				"windows", colorSpace,
				&color_out.x, &color_out.y, &color_out.z
			);
		}
		else {
			HC_PShow_One_Net_Color_By_Value(
				0, 0, "windows", colorSpace,
				&color_out.x, &color_out.y, &color_out.z
			);
		}
		assert(streq(colorSpace, "rgb"));
	}HC_Close_Segment();
}


void HBaseView::GetWindowColor(HPoint &color_top, HPoint &color_bottom)
{
	char colorSpace[MVO_BUFFER_SIZE];

	HC_Open_Segment_By_Key(GetViewKey());{
		if (HC_Show_Existence("color = windows")) {
			HC_Show_One_Color_By_Value(
				"windows", colorSpace,
				&color_top.x, &color_top.y, &color_top.z
			);
		}
		else {
			HC_PShow_One_Net_Color_By_Value(
				0, 0, "windows", colorSpace,
				&color_top.x, &color_top.y, &color_top.z
			);
		}
		assert(streq(colorSpace,"rgb"));

		if (HC_Show_Existence("color = window contrast")) {
			HC_Show_One_Color_By_Value(
				"window contrast", colorSpace,
				&color_bottom.x, &color_bottom.y, &color_bottom.z
			);
		}
		else {
			HC_PShow_One_Net_Color_By_Value(
				0, 0, "window contrast", colorSpace,
				&color_bottom.x, &color_bottom.y, &color_bottom.z
			);
		}
		assert(streq(colorSpace,"rgb"));
	}HC_Close_Segment();
}


void HBaseView::GetWindowColor(HPoint &color_top, HPoint &color_bottom, int &percentage)
{
	GetWindowColor(color_top,color_bottom);
	percentage = m_WindowColorPercentage;
}

void HBaseView::SetWindowColor(HPoint rgb, bool emit_message)
{
	char color[MVO_BUFFER_SIZE];

	if (emit_message){
		if(emit_message_function) {
			char message[MVO_BUFFER_SIZE];
			HCLOCALE(sprintf(message, "H_SET_WINDOW_COLOR %f %f %f %f %f %f %d",
				rgb.x, rgb.y, rgb.z, rgb.x, rgb.y, rgb.z, 100));
			emit_message_function((const char *)message, 
				0, emit_message_function_user_data);
		}
	}
	
	HCLOCALE(sprintf(color, "windows=(r=%f g=%f b=%f)", rgb.x, rgb.y, rgb.z));
	HC_Open_Segment_By_Key(GetViewKey());
		HC_Set_Color(color);
		HC_Set_Window_Pattern("solid");
		HImUnSetOneCallback("draw window");
	HC_Close_Segment();
}

void HBaseView::SetWindowColor(HPoint rgbtop, HPoint rgbbottom, bool emit_message)
{	
	char color[MVO_BUFFER_SIZE];
	
	if (emit_message){
		if(emit_message_function) {
			char message[MVO_BUFFER_SIZE];
			HCLOCALE(sprintf(message, "H_SET_WINDOW_COLOR %f %f %f %f %f %f %d",
				rgbtop.x, rgbtop.y, rgbtop.z, rgbbottom.x, rgbbottom.y, rgbbottom.z, m_WindowColorPercentage));
			emit_message_function((const char *)message, 
				0, emit_message_function_user_data);
		}
	}
	HCLOCALE(sprintf(color, "windows=(r=%f g=%f b=%f), window contrast = (r=%f g=%f b=%f)", rgbtop.x, rgbtop.y, rgbtop.z, rgbbottom.x, rgbbottom.y, rgbbottom.z));
	HC_Open_Segment_By_Key(GetViewKey());
		HC_Set_Color(color);
		HC_Set_Window_Pattern("Down");
		HImUnSetOneCallback("draw window");
	HC_Close_Segment();

	UpdateFakeHLRColor(rgbbottom);
}

void HBaseView::SetWindowColorRange(HPoint rgbtop, HPoint rgbbottom, int percentage, bool emit_message)
{
	m_WindowColorPercentage = percentage;
	char color[MVO_BUFFER_SIZE];
	if (emit_message){
		if(emit_message_function) {
			char message[MVO_BUFFER_SIZE];
			HCLOCALE(sprintf(message, "H_SET_WINDOW_COLOR %f %f %f %f %f %f %d",
				rgbtop.x, rgbtop.y, rgbtop.z, rgbbottom.x, rgbbottom.y, rgbbottom.z, m_WindowColorPercentage));
			emit_message_function((const char *)message, 
				0, emit_message_function_user_data);
		}
	}

	HCLOCALE(sprintf(color, "windows=(r=%f g=%f b=%f),window contrast=(r=%f g=%f b=%f)", rgbtop.x, rgbtop.y, rgbtop.z, rgbbottom.x, rgbbottom.y, rgbbottom.z));
	HC_Open_Segment_By_Key(GetViewKey());
		HC_Set_Color(color);
		HC_Set_Window_Pattern("solid");
		if (!HUtility::PointsEqual(&rgbtop, &rgbbottom))
			HImSetCallback("draw window","HImUtility_draw_gradiated_window_background");
	HC_Close_Segment();
}


void HBaseView::SetWindowColorFromMessage(const char *in_data, unsigned int data_length)
{
	char data[MVO_MESSAGE_BUFFER_SIZE];
  
	memcpy(data, in_data, data_length);
	data[data_length] = '\0';
	float rt,gt,bt, rb, gb, bb;
	int percentage;

	HCLOCALE(sscanf(data, "%f %f %f %f %f %f %d", &rt, &gt, &bt, &rb, &gb, &bb, &percentage));

	HPoint top, bottom;
	top.Set(rt, gt, bt);
	bottom.Set(rb, gb, bb);
	SetWindowColorRange(top, bottom, percentage);
	
	Update();
}

void HBaseView::SetInitialCamera(HPoint pos, HPoint tar, HPoint up, float fieldx, float fieldy, char const * proj)
{
	m_InitialCamera.position = pos;
	m_InitialCamera.target = tar;
	m_InitialCamera.up_vector = up;
	m_InitialCamera.field_width = fieldx;
	m_InitialCamera.field_height = fieldy;
	strcpy(m_InitialCamera.projection,proj);
	m_bInitialCameraSet = true;
  
}


void HBaseView::SetCamera(HPoint pos, HPoint tar, HPoint up, float fieldx, float fieldy, const char *proj)
{
	HC_Open_Segment_By_Key(GetSceneKey());
		HC_Set_Camera (&pos, &tar, &up, fieldx, fieldy, proj);
	HC_Close_Segment();

	Update();
}

void HBaseView::SetCamera(HCamera camera)
{
	HC_Open_Segment_By_Key(GetSceneKey());
		HC_Set_Camera (&camera.position, &camera.target, &camera.up_vector, camera.field_width, camera.field_height, camera.projection);
	HC_Close_Segment();

	Update();
}


void HBaseView::GetCamera(HPoint *pos, HPoint *tar, HPoint *up, float &fieldx, float &fieldy, char *proj)
{
	HC_Open_Segment_By_Key(GetSceneKey());
		HC_Show_Net_Camera (pos, tar, up, &fieldx, &fieldy, proj);
	HC_Close_Segment();
}


void HBaseView::GetCamera(HCamera *camera)
{
	memset(camera, 0, sizeof(HCamera));
	HC_Open_Segment_By_Key(GetSceneKey());
		HC_Show_Net_Camera (&camera->position, &camera->target, &camera->up_vector, &camera->field_width, &camera->field_height, camera->projection);
	HC_Close_Segment();
}

void HBaseView::PrepareForCameraChange()
{
	HUndoManager *hmanager = GetUndoManager();
	HUndoItemCamera *item = new HUndoItemCamera(this);
	HUndoItemCamera *old_item = 0;
	HUndoItem *temp = 0;
	
	// Find the last HUndoItemCamera in the undo list
	hmanager->BeginUndoItemIteration();
	while((temp = hmanager->GetNextUndoItem()) != 0)
	{
		if(temp && streq(temp->GetName(), "HUndoItemCamera"))
			old_item = (HUndoItemCamera *) temp;		
	}
	hmanager->EndUndoItemIteration();

	// We only add the undo item if the last camera item in the undo list is different
	if(!old_item || 
		memcmp(old_item->GetCamera(),item->GetCamera(),sizeof(HCamera)) != 0)
		GetUndoManager()->AddUndoItem(item);
	else
		delete item;
}

void HBaseView::CameraPositionChanged(bool emit_message, bool smooth_camera_transition)
{
	if (m_pMarkupManager && m_pMarkupManager->IsCameraLayer(m_pMarkupManager->GetCurrentLayerKey()) && m_pMarkupManager->GetResetOnCameraChange(m_pMarkupManager->GetCurrentLayerKey()))
		m_pMarkupManager->DisableCurrentLayer();

	if (m_bLightFollowsCamera) {
		HPoint	position, target, up;
		float	width, height;
		char    projection[MVO_BUFFER_SIZE];

		HC_Open_Segment_By_Key(GetSceneKey());
			HC_Show_Net_Camera (&position, &target, &up, &width, &height, projection);
		HC_Close_Segment();

		HPoint vector1, vector2, new_light;
		vector1.Set(position.x - target.x, position.y - target.y, position.z - target.z);
		HC_Compute_Normalized_Vector(&vector1, &vector1);
		HC_Compute_Normalized_Vector(&up, &up);
		HC_Compute_Cross_Product(&vector1, &up, &vector2);

		// offset the light from the camera vector so that flats objects facing the screen
		// do not appear washed out
		new_light.Set(1.5f*vector1.x+vector2.x, 1.5f*vector1.y+vector2.y, 1.5f*vector1.z+vector2.z);
//		HC_Move_Distant_Light (GetDefaultLightKey(), new_light.x, new_light.y, new_light.z);
	}

	SetCameraChanged();
 
	if (emit_message){
	
		if(emit_message_function) {

			char message[MVO_BUFFER_SIZE];
			HPoint position, target, up;
			float width, height;
			char projection[MVO_BUFFER_SIZE];
			int smooth_transition;

			if (smooth_camera_transition)
				smooth_transition = 1;
			else
				smooth_transition = 0;

			HC_Open_Segment_By_Key(GetSceneKey());
				HC_Show_Net_Camera(&position, &target, &up, 
					&width, &height, projection);	
			HC_Close_Segment();


			HCLOCALE(sprintf(message, "H_SET_CAMERA %f %f %f %f %f %f %f %f %f %f %f %s %d",
				position.x, position.y, position.z, 
				target.x, target.y, target.z,
				up.x, up.y, up.z, 
				width, height, projection, smooth_transition));

			emit_message_function((const char *)message, 
				0, emit_message_function_user_data);
		}
	}
	HLISTENER_EVENT(HUpdateListener, GetEventManager(), CameraChangedEvent(this));

	Notify(HSignalCameraPositionChanged);
}

/* 
  Set the camera based on a in incoming message which contains the camera information
  \param in_data A character pointer containing the camera information
  \param data_length The size of the message
*/
void HBaseView::SetCameraFromMessage(const char *in_data, unsigned int data_length)
{
	char data[MVO_MESSAGE_BUFFER_SIZE];
	HPoint position, target, up;
	float width, height;
	char projection[MVO_BUFFER_SIZE];
	int smooth_transition;
	
	memcpy(data, in_data, data_length);
	data[data_length] = '\0';
		
	HCLOCALE(sscanf(data, "%f %f %f %f %f %f %f %f %f %f %f %s %d",
		&position.x, &position.y, &position.z, 
		&target.x, &target.y, &target.z, 
		&up.x, &up.y, &up.z, 
		&width, &height, projection, &smooth_transition));

	HC_Open_Segment_By_Key(GetSceneKey());
	if (smooth_transition)
	{
		HPoint to,co,uo;
		float   widtho, heighto;

		HC_Show_Net_Camera (&co, &to, &uo, &widtho, &heighto, projection);
		HUtility::SmoothTransition(co, to, uo, widtho, heighto, position, target, up, width, height, this);
	}
	else
	{
		HC_Set_Camera(&position, &target, &up, width, height, projection);
	}

	HC_Close_Segment();

	CameraPositionChanged(false);
	Update();
}


void HBaseView::InsertHSFData(const char *segment, const char *in_data, unsigned int data_length, bool emit_message)
{
	if (emit_message){
		 EmitHSFData(segment,in_data,data_length);
	}

	HC_Open_Segment_By_Key(GetModel()->GetModelKey());{
		if (strcmp(segment, "nosegment") != 0)
			HC_Open_Segment(segment);
				GetModel()->GetStreamFileTK()->ParseBuffer(in_data, data_length);
		if (strcmp(segment, "nosegment") != 0)
			HC_Close_Segment();
	}HC_Close_Segment();
}


void HBaseView::EmitHSFData(const char *segment, const char *in_data, unsigned int data_length)
{
 
	if(emit_message_function) {
		
		char encoded_segment_name[MVO_SEGMENT_PATHNAME_BUFFER];
		HUtility::URIencode(segment, (int) strlen(segment), encoded_segment_name);
		
		//			char *encoded_hsf_data = new char [3*data_length+1];
		//			HUtility::URIencode(in_data, data_length, encoded_hsf_data);
		
		//			int length = strlen(encoded_segment_name) + strlen(encoded_hsf_data)+32;
		size_t length = strlen(encoded_segment_name) + 32 + data_length;
		char *message;
		message = new char [length];
		
		sprintf(message, "H_INSERT_HSF_DATA %s ", encoded_segment_name);
		
		int header_length = (int) strlen(message);
		memcpy(&message[header_length], in_data, data_length);
		
		emit_message_function((const char *)message, 
			data_length+header_length, emit_message_function_user_data);
		
		delete []message;
		//delete []encoded_hsf_data;
	}
		
}



void HBaseView::EmitSegment(HC_KEY key, bool emit_contents_only )
{
	if (m_pSharedKey)
	{
		// setup the stream toolkit nicely
		HStreamFileToolkit* tk = GetModel()->GetStreamFileTK();
		tk->Restart();
		m_pSharedKey->SetupStreamToolkit(tk);

		char buffer[MVO_MESSAGE_BUFFER_SIZE];
		int bytes_written;			
		TK_Status status;

		if( emit_contents_only )
		{
			// just emit the hsf data within this segment key
			HC_Open_Segment_By_Key(key);
				do
				{
					status = tk->GenerateBuffer(buffer, MVO_MESSAGE_BUFFER_SIZE, bytes_written);
					EmitHSFData(".",buffer,bytes_written);
				}while (status != TK_Complete);
			HC_Close_Segment();
		}
		else
		{
			// not just the contents of the segment key, but also the segment 
			// should be emitted. So, go to the father of this segment and emit
			// this dude from there - Rajesh B.
			char pathname[MVO_SEGMENT_PATHNAME_BUFFER]; 
			char seg_name[MVO_SEGMENT_PATHNAME_BUFFER];
			char new_seg_name[MVO_SEGMENT_PATHNAME_BUFFER];

			HC_Show_Segment(key, pathname);
			HC_Parse_String(pathname, "/", -1, seg_name);
			strcpy( new_seg_name, seg_name );

			HC_KEY owner_key = HC_KShow_Owner_By_Key(key);

			HC_Open_Segment_By_Key(owner_key);
				do
				{
					status =tk->GenerateBuffer(buffer, MVO_MESSAGE_BUFFER_SIZE, bytes_written, new_seg_name );
					EmitHSFData(".",buffer,bytes_written);
				}while (status != TK_Complete);
			HC_Close_Segment();
		}

	}

	// needed to work around for a bug with HSF data not forcing a redraw
	// at the Daratech conference
	CameraPositionChanged(true);
}


void HBaseView::InsertHSFDataFromMessage(const char *in_data, unsigned int data_length)
{
	// NOTE: TO MAITAIN THE SANITY OF STREAMTOOLKIT, WE SHOULD HAVE CALLED A tk->Restart() 
	// HERE. BUT SINCE THERE IS NOT WAY TO IDENTIFY WHEN THIS FUNCTION IS CALLED 
	// IN THE BEGINING OF FRESH READ, I AM PLACING tk->Restart() IN FlushFromMessage()
	// ENSURE THAT FlushFromMessage() IS CALLED BEFORE YOUR FIRST CALL HERE - RAJESH B

	char *data;
	data = new char [data_length+1];
	char segment[MVO_SEGMENT_PATHNAME_BUFFER];
	char encoded_segment_name[MVO_SEGMENT_PATHNAME_BUFFER];
	char *encoded_hsf_data;
	char *decoded_hsf_data;
	
	encoded_hsf_data = new char [data_length];
	decoded_hsf_data = new char [data_length];

	memcpy(data, in_data, data_length);
	data[data_length] = '\0';
		
	sscanf(data, "%s", encoded_segment_name);


	MVO_POINTER_SIZED_UINT n;
	unsigned long seg_length = (unsigned long) strlen(encoded_segment_name);

	unsigned long hsf_data_length = data_length - seg_length - 1;

	HUtility::URIdecode((const char *)encoded_segment_name, segment, &n);
	segment[n] = '\0';

	//HUtility::URIdecode((const char *)encoded_hsf_data, decoded_hsf_data, &hsf_data_length);
	TK_Status status;

	HC_Open_Segment_By_Key(GetModel()->GetModelKey());

	if (strcmp(segment, "nosegment") != 0)
	{
		if (strcmp(segment,"unnamed") == 0)
			m_pSharedKey->AssociateKey(HC_KOpen_Segment(""));
		else
			HC_KOpen_Segment(segment);
	}

		status = GetModel()->GetStreamFileTK()->ParseBuffer(in_data+seg_length+1, hsf_data_length);

	if (strcmp(segment, "nosegment") != 0)
		HC_Close_Segment();

	HC_Close_Segment();

	if (status == TK_Complete)
	{
		GetModel()->GetStreamFileTK()->Restart();

		// check if we have had any user options of interest set
		MVO_POINTER_SIZED_INT value = 0;
		HC_Open_Segment_By_Key(GetModel()->GetModelKey());
			if (HC_Show_Existence("user value"))
			{
				HC_Show_User_Value(&value);
				if (value == 1)
					GetModel()->SetBRepGeometry(true);
				else if (value == 2)
					GetModel()->SetFEAGeometry(true);
				else if (value == 3)
					GetModel()->SetVectorGeometry(true);
			}
			else
			{
				GetModel()->SetBRepGeometry(false);
				GetModel()->SetFEAGeometry(false);
				GetModel()->SetVectorGeometry(false);
			}
		HC_Close_Segment();

	}

	delete [] encoded_hsf_data;
	delete [] decoded_hsf_data;
	delete [] data;

	Update();
}


void HBaseView::Flush(bool emit_message)
{

	DeleteSelectionList(emit_message);
	
	if (emit_message)
		EmitFlushMessage();

	FlushFromMessage(0,0);
}


void HBaseView::EmitFlushMessage()
{
	if(emit_message_function) 
	{
		emit_message_function("H_FLUSH_BASE_VIEW", 
				0, emit_message_function_user_data);
	}
}

void HBaseView::FlushFromMessage(const char *in_data, unsigned int data_length)
{
	UNREFERENCED(in_data);
	UNREFERENCED(data_length);

	GetModel()->Flush();

	// re-setup the stream toolkit
	HStreamFileToolkit* tk = GetModel()->GetStreamFileTK();
	tk->Restart();

	if (m_pSharedKey)
		m_pSharedKey->Reset();
 
	HC_Open_Segment_By_Key(GetSceneKey());
		HC_Flush_Contents (".", "geometry");	
	HC_Close_Segment();

	H_SAFE_DELETE(m_pMarkupManager);
	
	HC_Open_Segment_By_Key(GetConstructionKey());
		HC_Flush_Contents (".", "geometry");
	HC_Close_Segment();
	
	HC_Open_Segment_By_Key(GetCuttingPlanesKey());
		HC_Flush_Contents (".", "geometry, segment");
		SetCuttingPlanesExist(false);
	HC_Close_Segment();

	HC_Open_Segment_By_Key(GetCuttingPlaneGeomKey());
		HC_Flush_Contents (".", "geometry");
	HC_Close_Segment();

	m_bInitialCameraSet = false;

	Update();
}



void HBaseView::EmitSelectMessage(HC_KEY key, bool isSelect)
{
	long ident;
 
	if(emit_message_function) {

		char message[MVO_BUFFER_SIZE];

		if (m_pSharedKey && (ident = m_pSharedKey->GetIdentFromKey(key)) != 0)
		{
			int temp = isSelect;
			sprintf(message, "H_SELECT_ENTITY %d %ld",temp,ident);
			
			emit_message_function((const char *)message, 
				0, emit_message_function_user_data);
		}
	}
}


void HBaseView::SelectFromMessage(const char *in_data, unsigned int data_length)
{
	char data[MVO_MESSAGE_BUFFER_SIZE];
	int isSelect = false;
	long ident = 0;
	HC_KEY key = 0;


	memcpy(data, in_data, data_length);
	data[data_length] = '\0';
		
	sscanf(data, "%d %ld", &isSelect, &ident);

	if (m_pSharedKey && (key = m_pSharedKey->GetKeyFromIdent(ident)) != 0)
	{
		if (isSelect)
			m_pSelection->Select(key, 0, 0, false);
		else
			m_pSelection->DeSelect(key, 0 , 0, false);

	}
	SetGeometryChanged();

	Update();
}


void HBaseView::EmitDeleteByKeyMessage(HC_KEY key)
{
	long ident;
 
	if(emit_message_function) {

		char message[MVO_BUFFER_SIZE];

		if (m_pSharedKey && (ident = m_pSharedKey->GetIdentFromKey(key)) != 0)
		{
			sprintf(message, "H_DELETE_BY_KEY %ld", ident);
			
			emit_message_function((const char *)message, 
				0, emit_message_function_user_data);

			char type[MVO_BUFFER_SIZE];
			HC_Show_Key_Type(key,type);
			if (strcmp(type, "segment") == 0)
			{
				HC_Open_Segment_By_Key(key);
				m_pSharedKey->DisAssociateKeysInSegment();
				HC_Close_Segment();
			}
			else
				m_pSharedKey->DisAssociateKey(key);

		}
	}
}


void HBaseView::DeleteByKeyFromMessage(const char *in_data, unsigned int data_length)
{
	char data[MVO_MESSAGE_BUFFER_SIZE];
	long ident = 0;
	HC_KEY key = 0;


	memcpy(data, in_data, data_length);
	data[data_length] = '\0';
		
	sscanf(data, "%ld", &ident);

	if (m_pSharedKey && (key = m_pSharedKey->GetKeyFromIdent(ident)) != 0)
	{
		char type[MVO_BUFFER_SIZE];
		HC_Show_Key_Type(key,type);
		if (strcmp(type, "segment") == 0)
		{
			HC_Open_Segment_By_Key(key);
			m_pSharedKey->DisAssociateKeysInSegment();
			HC_Close_Segment();
		}
		else
			m_pSharedKey->DisAssociateKey(key);
		HC_Delete_By_Key(key);
	}

	Update();		
}



void HBaseView::EmitDeSelectAllMessage()
{
 
	if(emit_message_function) { 			
			emit_message_function("H_DESELECT_ALL", 
				0, emit_message_function_user_data);
		}
	
}
 
void HBaseView::EmitDeleteSelectionListMessage()
{
 
	if(emit_message_function) { 			
			emit_message_function("H_DELETE_SELECTION_LIST", 
				0, emit_message_function_user_data);
		}
	
}

void HBaseView::DeSelectAllFromMessage()
{
	GetSelection()->DeSelectAll();
	SetGeometryChanged();
	Update();
}
 
void HBaseView::DeleteSelectionListFromMessage()
{
	DeleteSelectionList();
	Update();
}
 
 

void HBaseView::EmitSetModellingMatrixMessage(HC_KEY key)
{
	long ident;
		
		if(emit_message_function) {
						
			if (m_pSharedKey && (ident = m_pSharedKey->GetIdentFromKey(key)) != 0)
			{
				char message[MVO_BUFFER_SIZE];
				float mmatrix[16];
				HC_Open_Segment_By_Key(key);
				HC_Show_Modelling_Matrix(mmatrix);
				HC_Close_Segment();
				HCLOCALE(sprintf(message, "H_SET_MODELLING_MATRIX %ld %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f",ident, 
					mmatrix[0],mmatrix[1],mmatrix[2],mmatrix[3],
					mmatrix[4],mmatrix[5],mmatrix[6],mmatrix[7],
					mmatrix[8],mmatrix[9],mmatrix[10],mmatrix[11],
					mmatrix[12],mmatrix[13],mmatrix[14],mmatrix[15])); 			
				emit_message_function((const char *)message, 
					0, emit_message_function_user_data);
			}
			
		} 	
}


void HBaseView::EmitSetModellingMatrixMessage()
{
	HC_KEY key;
	char keytype[MVO_BUFFER_SIZE];
	
	if(emit_message_function) {
		
		
		HSelectionSet* selection = GetSelection();
		int size = selection->GetSize();
		
		for (int n = 0; n < size; n++)
		{
			key = selection->GetAt(n);
			HC_Show_Key_Type(key, keytype);
			
			if ((key != 0) && (streq(keytype, "segment")))
			{	 

				char pathname[MVO_SEGMENT_PATHNAME_BUFFER];
				char token[MVO_BUFFER_SIZE];
				HC_Show_Segment(key, pathname);
				
				HC_Parse_String(pathname, "/", -1, token); 
				
				if (streq(token, "cutting grids"))
				{
					char owner[MVO_SEGMENT_PATHNAME_BUFFER];
					HC_Show_Owner(pathname, owner);
					key = HC_KOpen_Segment(owner);
					HC_Close_Segment();
				}
				
			}
			EmitSetModellingMatrixMessage(key);
		}
	}	
	
}


void HBaseView::SetModellingMatrixFromMessage(const char *in_data, unsigned int data_length)
{
	char data[MVO_MESSAGE_BUFFER_SIZE];
	float mmatrix[16];
	long ident;
	HC_KEY key;
	memcpy(data, in_data, data_length);
	data[data_length] = '\0';
	
	
	HCLOCALE(sscanf(data, "%ld %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f", &ident, 
		&mmatrix[0],&mmatrix[1],&mmatrix[2],&mmatrix[3],
		&mmatrix[4],&mmatrix[5],&mmatrix[6],&mmatrix[7],
		&mmatrix[8],&mmatrix[9],&mmatrix[10],&mmatrix[11],
		&mmatrix[12],&mmatrix[13],&mmatrix[14],&mmatrix[15]));
	
	if (m_pSharedKey && (key = m_pSharedKey->GetKeyFromIdent(ident)) != 0)
	{
		HC_Open_Segment_By_Key(key);
		HC_Set_Modelling_Matrix(mmatrix);
		HC_Close_Segment();
	}

	Update();
}

void HBaseView::SetRenderModeFromMessage(const char *in_data, unsigned int data_length)
{
	char rendermode[MVO_BUFFER_SIZE];
	char data[MVO_MESSAGE_BUFFER_SIZE];
	
	memcpy(data, in_data, data_length);
	data[data_length] = '\0';
	
	sscanf(data, "%s", rendermode);

	if (streq(rendermode, "Wireframe"))
	SetRenderMode(HRenderWireframe);

	else if (streq(rendermode, "Silhouette"))
	SetRenderMode(HRenderSilhouette);

	else if (streq(rendermode, "HiddenLine"))
	SetRenderMode(HRenderHiddenLine);

	else if (streq(rendermode, "BRepHiddenLine"))
	SetRenderMode(HRenderBRepHiddenLine);

	else if (streq(rendermode, "BRepHiddenLineFast"))
	SetRenderMode(HRenderBRepHiddenLineFast);

	else if (streq(rendermode, "BRepWireframe"))
	SetRenderMode(HRenderBRepWireframe);

	else if (streq(rendermode, "HiddenLineHOOPS"))
	SetRenderMode(HRenderHiddenLineHOOPS);

	else if (streq(rendermode, "HiddenLineFast"))
	SetRenderMode(HRenderHiddenLineFast);

	else if (streq(rendermode, "Flat"))
	SetRenderMode(HRenderFlat);

	else if (streq(rendermode, "Gouraud"))
	SetRenderMode(HRenderGouraud);

	else if (streq(rendermode, "GouraudWithEdges"))
	SetRenderMode(HRenderGouraudWithEdges);

	else if (streq(rendermode, "Phong"))
	SetRenderMode(HRenderPhong);

	else if (streq(rendermode, "Shaded"))
	SetRenderMode(HRenderShaded);

	else if (streq(rendermode, "ShadedWithLines"))
	SetRenderMode(HRenderShadedWithLines);
	else if (streq(rendermode, "GouraudWithLines"))
	SetRenderMode(HRenderGouraudWithLines);

	else if (streq(rendermode, "Triangulation"))
	SetRenderMode(HRenderTriangulation);

	else if (streq(rendermode, "WireframeWithSilhouette"))
	SetRenderMode(HRenderWireframeWithSilhouette);

	else if (streq(rendermode, "ShadedWireframe"))
	SetRenderMode(HRenderShadedWireframe);

	else if (streq(rendermode, "Vertices"))
	SetRenderMode(HRenderVertices);

	else if (streq(rendermode, "ShadedVertices"))
	SetRenderMode(HRenderShadedVertices);

	else if (streq(rendermode, "LOD1"))
	SetRenderMode(HRenderShadedVertices);

	else if (streq(rendermode, "LOD2"))
	SetRenderMode(HRenderShadedVertices);

	Update();
}

void HBaseView::SetRenderMode (HRenderMode RenderMode, bool emit_message)
{

	HRenderMode NewRenderMode;
	if (m_bForceFastHiddenLine && RenderMode == HRenderHiddenLine)
		NewRenderMode = HRenderHiddenLineFast;
	else if (/*m_bForceFastHiddenLine && */RenderMode==HRenderBRepHiddenLine)
		NewRenderMode = HRenderBRepHiddenLineFast;
	else	
		NewRenderMode = RenderMode;

	 // we use this to package up the rendermode for the HOOPS/Net message
	char rendermode[MVO_BUFFER_SIZE];
	bool update_triad=false;
	SetCPGeomVisibility(GetCPLineVisibility(), true);

	if(GetRenderMode() == HRenderLOD1 || GetRenderMode() == HRenderLOD2){
		HC_Open_Segment_By_Key(m_OverwriteKey);{
			HC_UnSet_One_Rendering_Option("lod");
			HC_UnSet_One_Rendering_Option("lighting");
			HC_UnSet_One_Rendering_Option("lodo");
		}HC_Close_Segment();
	}

	//if we're going into hidden line mode turn off shadows.
	//if we're coming out of hidden line mode, turn them back on.
	if(RenderMode == HRenderHiddenLineFast || RenderMode == HRenderHiddenLine || 
		RenderMode == HRenderBRepHiddenLine || RenderMode == HRenderBRepHiddenLineFast ||
		RenderMode == HRenderFakeHiddenLine)
	{
		HC_Open_Segment_By_Key(m_OverwriteKey);
			HC_Set_Rendering_Options("simple shadow=off");
		HC_Close_Segment();
	}
	else if(GetRenderMode() == HRenderHiddenLineFast || GetRenderMode() == HRenderHiddenLine || 
		GetRenderMode() == HRenderBRepHiddenLine || GetRenderMode() == HRenderBRepHiddenLineFast ||
		GetRenderMode() == HRenderFakeHiddenLine)
	{
		HC_Open_Segment_By_Key(m_OverwriteKey);
			HC_UnSet_One_Rendering_Option("simple shadow");
		HC_Close_Segment();
	}

	//if we're going into wireframe mode turn off frame buffer effects.
	//if we're coming out of wireframe mode, turn them back on.
	if(RenderMode == HRenderWireframe || RenderMode == HRenderBRepWireframe)
	{
		HC_Open_Segment_By_Key(m_OverwriteKey);
			HC_Set_Rendering_Options("no frame buffer effects");
		HC_Close_Segment();
	}
	else if(GetRenderMode() == HRenderWireframe || GetRenderMode() == HRenderBRepWireframe)
	{
		HC_Open_Segment_By_Key(m_OverwriteKey);
			HC_UnSet_One_Rendering_Option("frame buffer effects");
		HC_Close_Segment();
	}

	//Coming out of fake hidden line, we need to do two things:
	// 1) remove the fake hlr style from the model segment
	// 2) restore the driver options if we changed them (for fast silhouette edges)
	if(GetRenderMode() == HRenderFakeHiddenLine)
	{
		if(m_FakeHLRStyle!=INVALID_KEY)
			HC_Delete_By_Key(m_FakeHLRStyle);
		m_FakeHLRStyle=INVALID_KEY;

		HC_Open_Segment_By_Key(GetViewKey());
			/* if we saved a fast silhouette edges, restore it, otherwise unset what we've put in there for fake hlr */
			if(HC_Show_Existence("user option=saved dopt"))
			{
				char dopt[MVO_BUFFER_SIZE];
				sprintf(dopt, "fast silhouette edges=");
				HC_Show_One_User_Option("saved dopt", &dopt[22]);
				HC_Set_Driver_Options(dopt);
				HC_UnSet_One_User_Option("saved dopt");
			}
			else
				HC_UnSet_One_Driver_Option("fast silhouette edges");

			if(HC_Show_Existence("user option=saved wcolor"))
			{
				char wcolor[MVO_BUFFER_SIZE];

				sprintf(wcolor, "windows=");
				HC_Show_One_User_Option("saved wcolor", &wcolor[8]);
				HC_Set_Color(wcolor);
				HC_UnSet_One_User_Option("saved wcolor");
			}
			else
			{
				if(HC_Show_Existence("color=windows"))
					HC_UnSet_One_Color("windows");
			}

			if(HC_Show_Existence("user option=saved contrast wcolor"))
			{
				char wcolor[MVO_BUFFER_SIZE];

				sprintf(wcolor, "window contrast=");
				HC_Show_One_User_Option("saved contrast wcolor", &wcolor[16]);
				HC_Set_Color(wcolor);
				HC_UnSet_One_User_Option("saved contrast wcolor");
			}
			else
			{
				if(HC_Show_Existence("color=window contrast"))
					HC_UnSet_One_Color("window contrast");
			}

			if(HC_Show_Existence("user option=saved ecolor"))
			{
				char ecolor[MVO_BUFFER_SIZE];

				sprintf(ecolor, "edge=");
				HC_Show_One_User_Option("saved ecolor", &ecolor[5]);
				HC_Set_Color(ecolor);
				HC_UnSet_One_User_Option("saved ecolor");
			}
			else
			{
				if(HC_Show_Existence("color=edge"))
					HC_UnSet_One_Color("edge");
			}
		HC_Close_Segment();

		HC_Open_Segment_By_Key(GetSceneKey());
			if(HC_Show_Existence("rendering options=attribute lock"))
				HC_UnSet_One_Rendering_Option("attribute lock");
		HC_Close_Segment();
	}

	
	switch (NewRenderMode)
	{
		case HRenderWireframe:{
			sprintf(rendermode, "Wireframe");

			HC_Open_Segment_By_Key(m_ViewKey);
				HC_Set_Heuristics("hidden surfaces");
				SetDefaultHSRA ();
			HC_Close_Segment();
			
			HC_Open_Segment_By_Key(m_SceneKey);
			if (GetModel()->IsBRepGeometry())
				HC_Set_Visibility("everything=off, cutting planes=on, cut geometry=on, shadows=on, windows=on, text=on, lines=on");
			else
				HC_Set_Visibility("everything=off, cutting planes=on, cut geometry=on, shadows=on, windows=on, text=on, edges=on, lines=on");
			HC_Set_Visibility("lights = (on, edges = off, markers = off), images=on");
			HC_Close_Segment();
			
			SetCPGeomVisibility(GetCPLineVisibility(), false);
			
			if (update_triad){
				HC_Open_Segment_By_Key(m_AxisTriadKey);  
				HC_UnSet_One_Rendering_Option("hidden surface removal algorithm");
				HC_Set_Visibility("faces=off, edges=on, lines=on, lights = off");
				HC_Close_Segment();
			}
			
							  }
			break;

		case HRenderSilhouette:{
			sprintf(rendermode, "Silhouette");
			
			HC_Open_Segment_By_Key(m_SceneKey);
			if (GetModel()->IsBRepGeometry())
				HC_Set_Visibility("everything=off, cutting planes=on, cut geometry=on, shadows=on, windows=on, text=on, lines = on, edges=(interior silhouettes)");
			else
				HC_Set_Visibility("everything=off, cutting planes=on, cut geometry=on, shadows=on, windows=on, text=on, lines = on, edges=(interior silhouettes, perimeters)");
			HC_Set_Visibility("lights = (on, edges = off, markers = off), images=on");
			HC_Close_Segment();
			
			if (update_triad){
				HC_Open_Segment_By_Key(m_AxisTriadKey);
				HC_UnSet_One_Rendering_Option("hidden surface removal algorithm");
				HC_Set_Visibility("faces=off, lights = off, edges=(generics=off,perimeters=on,interior silhouettes=on, hard=on),lines=on");
				HC_Close_Segment();
			}
							   }
			break;
			
		case HRenderHiddenLine:{
			sprintf(rendermode, "HiddenLine");
			HC_Open_Segment_By_Key(m_ViewKey);
			HC_Set_Heuristics("hidden surfaces");
			HC_Set_Visibility("faces=on");
			HC_Set_Rendering_Options("hsra = hidden line");
			HC_Close_Segment();
			
			HC_Open_Segment_By_Key(GetSceneKey());
			if (GetModel()->IsBRepGeometry())
				HC_Set_Visibility("everything=off, cutting planes=on, cut geometry=on, shadows=on, windows=on, text=on, faces=on, lines=on, edges=(interior silhouettes)");
			else
				HC_Set_Visibility("everything=off, cutting planes=on, cut geometry=on, shadows=on, windows=on, text=on, faces=on, lines=on, edges=on");
			HC_Set_Visibility("lights = (on, edges = off, markers = off), images=on");
			HC_Close_Segment();
			
			if (update_triad){
				HC_Open_Segment_By_Key(m_AxisTriadKey);
					HC_UnSet_One_Rendering_Option("hidden surface removal algorithm");
					HC_Set_Visibility("faces = on, lines = on, lights = off, edges=(generics=off,perimeters=on,interior silhouettes=on, hard=on)");
				HC_Close_Segment();
				}
			}

			break;

		case HRenderHiddenLineHOOPS:{
			sprintf(rendermode, "HiddenLineHOOPS");
			}
			break;


		case HRenderHiddenLineFast:{

			sprintf(rendermode, "HiddenLineFast");
			HC_Open_Segment_By_Key(m_ViewKey);
				HC_Set_Heuristics("hidden surfaces");
				HC_Set_Visibility("faces=on");
				HC_Set_Rendering_Options("hsra = fast hidden line");
			HC_Close_Segment();
			
			HC_Open_Segment_By_Key(GetSceneKey());
				if (GetModel()->IsBRepGeometry())
				  HC_Set_Visibility("everything=off, cutting planes=on, cut geometry=on, shadows=on, windows=on, text=on, faces=on, lines=on, edges=(interior silhouettes)");
				else
				  HC_Set_Visibility("everything=off, cutting planes=on, cut geometry=on, shadows=on, windows=on, text=on, faces = on, lines = on, edges = on");
			HC_Set_Visibility("lights = (on, edges = off, markers = off), images=on");
			HC_Close_Segment();
			
			if (update_triad){
				HC_Open_Segment_By_Key(m_AxisTriadKey);
					HC_UnSet_One_Rendering_Option("hsra = fast hidden line");
					HC_Set_Visibility("faces = on, lines = on, lights = off, edges=(generics=off,perimeters=on,interior silhouettes=on, hard=on)");
				HC_Close_Segment();
				}
			}
		break;

   
		case HRenderFlat:{
			sprintf(rendermode, "Flat");
			HC_Open_Segment_By_Key(m_ViewKey);
				HC_Set_Heuristics("hidden surfaces");
				SetDefaultHSRA();
			HC_Close_Segment();

			HC_Open_Segment_By_Key(GetSceneKey());
				HC_Set_Rendering_Options("no lighting interpolation");
				if (GetModel()->IsBRepGeometry())
					HC_Set_Visibility("everything=off, cutting planes=on, cut geometry=on, shadows=on, windows=on, text=on, lights=on, faces=on");
				else
					HC_Set_Visibility("everything=off, cutting planes=on, cut geometry=on, shadows=on, windows=on, text=on, lights=on, faces=on, lines=on");
				HC_Set_Visibility("lights = (on, edges = off, markers = off), images=on");
			HC_Close_Segment();

			if (update_triad){
				HC_Open_Segment_By_Key(m_AxisTriadKey);  
				HC_Set_Rendering_Options("no lighting interpolation");
				HC_UnSet_One_Rendering_Option("hidden surface removal algorithm");
				HC_Set_Visibility("lights=(faces), faces=on, edges=off");
				HC_Close_Segment();
			}
		}
		break;

		case HRenderGouraud:{
			sprintf(rendermode, "Gouraud");
			HC_Open_Segment_By_Key(m_ViewKey);
				HC_Set_Heuristics("hidden surfaces");
				if (use_hzb(m_sDriverType))
					HC_Set_Rendering_Options("hsra = hardware z-buffer");
				else
					HC_Set_Rendering_Options("hsra = szb");
			HC_Close_Segment();

			HC_Open_Segment_By_Key(m_SceneKey);
				HC_Set_Rendering_Options("lighting interpolation=(faces)");
				if (GetModel()->IsBRepGeometry())
					HC_Set_Visibility("everything=off, faces=on, cutting planes=on, cut geometry=on, shadows=on, windows=on, text=on");
				else
					HC_Set_Visibility("everything=off, faces=on, cutting planes=on, cut geometry=on, shadows=on, windows=on, text=on");
				HC_Set_Visibility("lights = (on, edges = off, markers = off), images=on");
			HC_Close_Segment();

			if (update_triad){
				HC_Open_Segment_By_Key(m_AxisTriadKey);  
				HC_Set_Rendering_Options("lighting interpolation=(faces)");
				HC_UnSet_One_Rendering_Option("hidden surface removal algorithm");
				HC_Set_Visibility("lights=(faces), faces=on, edges=off");
				HC_Close_Segment();
			}
		}
		break;

		case HRenderGouraudWithEdges:{
			sprintf(rendermode, "GouraudWithEdges");
			HC_Open_Segment_By_Key(m_ViewKey);
				HC_Set_Heuristics("hidden surfaces");
				SetDefaultHSRA();
			HC_Close_Segment();

			HC_Open_Segment_By_Key(m_SceneKey);
				HC_Set_Rendering_Options("lighting interpolation=(faces)");
				if (GetModel()->IsBRepGeometry())
					HC_Set_Visibility("everything=off, cutting planes=on, cut geometry=on, shadows=on, windows=on, text=on, lights=on, edges=on, faces=on, lines=on");
				else
					HC_Set_Visibility("everything=off, cutting planes=on, cut geometry=on, shadows=on, windows=on, text=on, lights=on, faces=on, edges=on, lines=on");
				HC_Set_Visibility("lights = (on, edges = off, markers = off), images=on");
			HC_Close_Segment();

			if (update_triad){
				HC_Open_Segment_By_Key(m_AxisTriadKey);  
				HC_Set_Rendering_Options("lighting interpolation=(faces)");
				HC_UnSet_One_Rendering_Option("hidden surface removal algorithm");
				HC_Set_Visibility("lights=(faces), faces=on, edges=on");
				HC_Close_Segment();
			}
		}
		break;


		case HRenderPhong:{
			sprintf(rendermode, "Phong");
			HC_Open_Segment_By_Key(m_ViewKey);
				HC_Set_Heuristics("hidden surfaces");
				SetDefaultHSRA();
			HC_Close_Segment();

			HC_Open_Segment_By_Key(m_SceneKey);
				HC_Set_Rendering_Options("lighting interpolation=(faces=phong)");
				if (GetModel()->IsBRepGeometry())
					HC_Set_Visibility("everything=off, cutting planes=on, cut geometry=on, shadows=on, windows=on, text=on, lights=on, faces=on");
				else
					HC_Set_Visibility("everything=off, cutting planes=on, cut geometry=on, shadows=on, windows=on, text=on, lights=on, faces=on, lines=on");
				HC_Set_Visibility("lights = (on, edges = off, markers = off), images=on");
			HC_Close_Segment();

			if (update_triad){
				HC_Open_Segment_By_Key(m_AxisTriadKey);  
				HC_Set_Rendering_Options("lighting interpolation=(faces=phong)");
				HC_UnSet_One_Rendering_Option("hidden surface removal algorithm");
				HC_Set_Visibility("lights=(faces), faces=on, edges=off");
				HC_Close_Segment();
			}
		}
		break;


		case HRenderShaded:{
			sprintf(rendermode, "Shaded");
			
			// only try to set phong lighting if the driver is OGL
			if (strieq(m_sDriverType, "opengl"))	
			{
				char temp[MVO_BUFFER_SIZE];

				// if we've already done an update, we can query the device and then
				// set Phong or Gouraud based on that; otherwise, just set Gouraud and
				// let MVO automaticlaly query the device after the first update
				if (m_bFirstUpdate)
				{
					HC_Show_Device_Info(GetDriverPath(), "phong lighting", temp);

					if (strstr(temp, "yes"))
						SetRenderMode(HRenderPhong);
					else
						SetRenderMode(HRenderGouraud);	
				}
				else
					SetRenderMode(HRenderGouraud);
			}
			else
				SetRenderMode(HRenderGouraud);	
		}
		break;

		case HRenderShadedWithLines:{
			sprintf(rendermode, "ShadedWithLines");
			
			// only try to set phong lighting if the driver is OGL
			if (strieq(m_sDriverType, "opengl"))	
			{
				char temp[MVO_BUFFER_SIZE];

				// if we've already done an update, we can query the device and then
				// set Phong or Gouraud based on that; otherwise, just set Phong and
				// let MVO automaticlaly query the device after the first update
				if (m_bFirstUpdate)
				{
					HC_Show_Device_Info(GetDriverPath(), "phong lighting", temp);

					if (strstr(temp, "yes"))
						SetRenderMode(HRenderPhong);
					else
						SetRenderMode(HRenderGouraud);	
				}
				else
					SetRenderMode(HRenderPhong);
			}
			else
				SetRenderMode(HRenderGouraud);	
			
		}
		HC_Open_Segment_By_Key(m_SceneKey);{
			HC_Set_Visibility("lines = on");
		}HC_Close_Segment();

		break;

		case HRenderGouraudWithLines:{
			sprintf(rendermode, "GouraudWithLines");
			HC_Open_Segment_By_Key(m_ViewKey);
			HC_Set_Heuristics("hidden surfaces");

			if (use_hzb(m_sDriverType))
				HC_Set_Rendering_Options("hsra = hardware z-buffer");
			else
				HC_Set_Rendering_Options("hsra = szb");

			HC_Close_Segment();

			HC_Open_Segment_By_Key(m_SceneKey);
			HC_Set_Rendering_Options("lighting interpolation=(faces)");
			if (GetModel()->IsBRepGeometry())
				HC_Set_Visibility("everything=off, lights=on, faces=on, cutting planes=on, cut geometry=on, shadows=on, windows=on, text=on, lines=on");
			else
				HC_Set_Visibility("everything=off, lights=on, faces=on, cutting planes=on, cut geometry=on, shadows=on, windows=on, text=on, lines=on");
			HC_Set_Visibility("lights = (on, edges = off, markers = off), images=on");
			HC_Close_Segment();

			if (update_triad){
				HC_Open_Segment_By_Key(m_AxisTriadKey);  
				HC_Set_Rendering_Options("lighting interpolation=(faces)");
				HC_UnSet_One_Rendering_Option("hidden surface removal algorithm");
				HC_Set_Visibility("lights=(faces), faces=on, edges=off");
				HC_Close_Segment();
			}

		}
		break;

		case HRenderTriangulation:{
			sprintf(rendermode, "Triangulation");
			HC_Open_Segment_By_Key(m_SceneKey);
				HC_Set_Visibility("edges = on, lines = off, faces =on, lights = (on, edges = off, markers = off), images=on");
				HC_Set_Rendering_Options("no lighting interpolation");
			HC_Close_Segment();

			if (update_triad){
				HC_Open_Segment_By_Key(m_AxisTriadKey);  
				HC_Set_Rendering_Options("no lighting interpolation");
				HC_UnSet_One_Rendering_Option("hidden surface removal algorithm");
				HC_Set_Visibility("edges = on, lines = off, faces =on");
				HC_Close_Segment();
			}
		}
		break;

		case HRenderWireframeWithSilhouette:{
			sprintf(rendermode, "WireframeWithSilhouette");

			HC_Open_Segment_By_Key(m_ViewKey);
				HC_Set_Heuristics("hidden surfaces");
				SetDefaultHSRA ();
			HC_Close_Segment();
			
			HC_Open_Segment_By_Key(m_SceneKey);
			if (GetModel()->IsBRepGeometry())
				HC_Set_Visibility("everything=off, cutting planes=on, cut geometry=on, shadows=on, windows=on, text=on, lights=(on, edges=off), edges=(interior silhouettes), lines=on");
			else
				HC_Set_Visibility("everything=off, cutting planes=on, cut geometry=on, shadows=on, windows=on, text=on, lights=(on, edges=off), edges=on, lines=on");
			HC_Set_Visibility("lights = (on, edges = off, markers = off), images=on");
			HC_Close_Segment();
			
			SetCPGeomVisibility(GetCPLineVisibility(), false);
			
			if (update_triad){
				HC_Open_Segment_By_Key(m_AxisTriadKey);  
				HC_UnSet_One_Rendering_Option("hidden surface removal algorithm");
				HC_Set_Visibility("faces=off, edges=on, lines=on, lights = off");
				HC_Close_Segment();
			}
		}
		break;

		case HRenderShadedWireframe:{
			sprintf(rendermode, "ShadedWireframe");

			HC_Open_Segment_By_Key(m_ViewKey);
				HC_Set_Heuristics("hidden surfaces");
				SetDefaultHSRA();
			HC_Close_Segment();
			
			HC_Open_Segment_By_Key(m_SceneKey);
			HC_Set_Rendering_Options("lighting interpolation= (edges = on)");
			if (GetModel()->IsBRepGeometry())
				HC_Set_Visibility("everything=off, cutting planes=on, cut geometry=on, shadows=on, windows=on, text=on, lights=(edges), lines=on");
			else
				HC_Set_Visibility("everything=off, cutting planes=on, cut geometry=on, shadows=on, windows=on, text=on, lights=(edges), edges=on, lines=on");
			HC_Set_Visibility("lights = on, images=on");
			HC_Close_Segment();
			
			SetCPGeomVisibility(GetCPLineVisibility(), false);
			
			if (update_triad){
				HC_Open_Segment_By_Key(m_AxisTriadKey);  
				HC_UnSet_One_Rendering_Option("hidden surface removal algorithm");
				HC_Set_Visibility("faces=off, edges=on, lines=on, lights = off");
				HC_Close_Segment();
			}
		}
		break;

		case HRenderVertices:{
			sprintf(rendermode, "Vertices");
			
			HC_Open_Segment_By_Key(m_SceneKey);
			HC_Set_Marker_Size(0);
			HC_Set_Marker_Symbol(".");
			HC_Set_Rendering_Options("attribute lock = (marker size, marker symbol)");

			if (GetModel()->IsBRepGeometry())
				HC_Set_Visibility("everything=off, cutting planes=on, cut geometry=on, shadows=on, windows=on, text=on, vertices=on");
			else
				HC_Set_Visibility("everything=off, cutting planes=on, cut geometry=on, shadows=on, windows=on, text=on, vertices=on");
			HC_Set_Visibility("lights = (on, edges = off, markers = off), images=on");
			HC_Close_Segment();
			
			SetCPGeomVisibility(GetCPLineVisibility(), false);
			
			if (update_triad){
				HC_Open_Segment_By_Key(m_AxisTriadKey);  
				HC_UnSet_One_Rendering_Option("hidden surface removal algorithm");
				HC_Set_Visibility("faces=off, edges=on, lines=on, lights = off");
				HC_Close_Segment();
			}
		}
		break;

		case HRenderGoochShaded: {
			sprintf(rendermode, "GoochShaded");
			HC_Open_Segment_By_Key(m_ViewKey);
				HC_Set_Heuristics("hidden surfaces");

				if (use_hzb(m_sDriverType))
					HC_Set_Rendering_Options("hsra = hardware z-buffer");
				else
					HC_Set_Rendering_Options("hsra = szb");
			HC_Close_Segment();

			HC_Open_Segment_By_Key(m_SceneKey);
				HC_Set_Rendering_Options("lighting interpolation = gooch");
				if (GetModel()->IsBRepGeometry())
					HC_Set_Visibility("everything=off, lights=on, faces=on, cutting planes=on, cut geometry=on, shadows=on, windows=on, text=on");
				else
					HC_Set_Visibility("everything=off, lights=on, faces=on, cutting planes=on, cut geometry=on, shadows=on, windows=on, text=on");
				HC_Set_Visibility("lights = (on, edges = off, markers = off), images=on");
			HC_Close_Segment();

			if (update_triad){
				HC_Open_Segment_By_Key(m_AxisTriadKey);  
					HC_Set_Rendering_Options("lighting interpolation=gooch");
					HC_UnSet_One_Rendering_Option("hidden surface removal algorithm");
					HC_Set_Visibility("lights=(faces), faces=on, edges=off");
				HC_Close_Segment();
			}		


		} break;

		case HRenderShadedVertices:{
			sprintf(rendermode, "ShadedVertices");
		
			HC_Open_Segment_By_Key(m_SceneKey);
			HC_Set_Marker_Size(0);
			HC_Set_Marker_Symbol(".");
			HC_Set_Rendering_Options("attribute lock = (marker size, marker symbol)");

			if (GetModel()->IsBRepGeometry())
				HC_Set_Visibility("everything=off, cutting planes=on, cut geometry=on, shadows=on, windows=on, text=on, lights=(vertices=on), vertices=on");
			else				   
				HC_Set_Visibility("everything=off, cutting planes=on, cut geometry=on, shadows=on, windows=on, text=on, lights=(vertices=on), vertices=on");
			HC_Set_Visibility("lights = on, images=on");
			HC_Close_Segment();
			
			SetCPGeomVisibility(GetCPLineVisibility(), false);
			
			if (update_triad){
				HC_Open_Segment_By_Key(m_AxisTriadKey);  
				HC_UnSet_One_Rendering_Option("hidden surface removal algorithm");
				HC_Set_Visibility("faces=off, edges=on, lines=on, lights = off");
				HC_Close_Segment();
			}
		}
		break;

		case HRenderLOD1:{
			sprintf(rendermode, "LOD1");
			HC_Open_Segment_By_Key(m_OverwriteKey);{
				HC_Set_Rendering_Options("lod=on, lodo = (clamp = 1), no lighting interpolation");
			}HC_Close_Segment();
		}break;
		case HRenderLOD2:{
			sprintf(rendermode, "LOD2");
			HC_Open_Segment_By_Key(m_OverwriteKey);{
				HC_Set_Rendering_Options("lod=on, lodo = (clamp = 2), no lighting interpolation");
			}HC_Close_Segment();
		}break;

		case HRenderBRepHiddenLine:{
			sprintf(rendermode, "BRepHiddenLine");
			HC_Open_Segment_By_Key(m_ViewKey);
				HC_Set_Heuristics("hidden surfaces");
				HC_Set_Visibility("faces=on");
				HC_Set_Rendering_Options("hsra = hidden line");
			HC_Close_Segment();
			
			HC_Open_Segment_By_Key(GetSceneKey());
			if (GetModel()->IsBRepGeometry())
				HC_Set_Visibility("everything=off, cutting planes=on, cut geometry=on, shadows=on, windows=on, text=on, faces = on, lines = on, edges=(interior silhouettes=on)");
			else
				HC_Set_Visibility("everything=off, cutting planes=on, cut geometry=on, shadows=on, windows=on, text=on, faces = on, lines = on, edges=(hard=on, perimeters=on, interior silhouettes=on)");
			HC_Set_Visibility("lights = (on, edges = off, markers = off), images=on");
			HC_Close_Segment();
			
			if (update_triad){
				HC_Open_Segment_By_Key(m_AxisTriadKey);
					HC_UnSet_One_Rendering_Option("hidden surface removal algorithm");
					HC_Set_Visibility("everything=off, faces = on, lines = on, lights = off, edges=(generics=off,perimeters=on,interior silhouettes=on, hard=on)");
				HC_Close_Segment();
			}
		}
		break;
		
		case HRenderBRepHiddenLineFast:{
			sprintf(rendermode, "BRepHiddenLineFast");
			HC_Open_Segment_By_Key(m_ViewKey);
				HC_Set_Heuristics("hidden surfaces");
				HC_Set_Visibility("faces=on");
				HC_Set_Rendering_Options("hsra = fast hidden line");
			HC_Close_Segment();
			
			HC_Open_Segment_By_Key(GetSceneKey());
			if (GetModel()->IsBRepGeometry())
				HC_Set_Visibility("everything=off, cutting planes=on, cut geometry=on, shadows=on, windows=on, text=on, faces=on, lines=on, edges=(interior silhouettes=on)");
			else
				HC_Set_Visibility("everything=off, cutting planes=on, cut geometry=on, shadows=on, windows=on, text=on, faces=on, lines=on, edges=(hard=on, perimeters=on, interior silhouettes=on)");
			HC_Set_Visibility("lights = (on, edges = off, markers = off), images=on");
			HC_Close_Segment();
			
			if (update_triad){
				HC_Open_Segment_By_Key(m_AxisTriadKey);
					HC_UnSet_One_Rendering_Option("hidden surface removal algorithm");
					HC_Set_Visibility("everything=off, faces = on, lines = on, edges=(generics=off,perimeters=on,interior silhouettes=on, hard=on)");
				HC_Close_Segment();
			}
		}
		break;

		case HRenderBRepWireframe:
		{
			sprintf(rendermode, "BRepWireframe");
			
			HC_Open_Segment_By_Key(m_ViewKey);
				HC_Set_Heuristics("hidden surfaces");
				SetDefaultHSRA ();
			HC_Close_Segment();
			
			HC_Open_Segment_By_Key(m_SceneKey);
			if (GetModel()->IsBRepGeometry())
				HC_Set_Visibility("everything=off, cutting planes=on, cut geometry=on, shadows=on, windows=on, text=on, edges=(generics=off, perimeters=off, interior silhouettes=off), lines=on");
			else
				HC_Set_Visibility("everything=off, cutting planes=on, cut geometry=on, shadows=on, windows=on, text=on, edges=(everything=off, hard=on, perimeters=on), lines=on");
			HC_Set_Visibility("lights = (on, edges = off, markers = off), images=on");
			HC_Close_Segment();
			
			SetCPGeomVisibility(GetCPLineVisibility(), false);
			
			if (update_triad)
			{
				HC_Open_Segment_By_Key(m_AxisTriadKey);  
					HC_UnSet_One_Rendering_Option("hidden surface removal algorithm");
					HC_Set_Visibility("everything=off, faces=off, edges=on, lines=on, lights = off");
				HC_Close_Segment();
			}
		} break;

		case HRenderFakeHiddenLine: {
			char tmp[MVO_BUFFER_SIZE];
			char dopt[MVO_BUFFER_SIZE];
			char wcolor[MVO_BUFFER_SIZE];

			HC_Open_Segment_By_Key(GetViewKey());
				if (strieq(m_sDriverType,"opengl2") || strieq(m_sDriverType,"dx9") || strieq(m_sDriverType,"dx11"))
				{
					if(HC_Show_Existence("driver options=fast silhouette edges"))
					{
						sprintf(dopt, "saved dopt=(");
						HC_Show_One_Driver_Option("fast silhouette edges", &dopt[12]);
						size_t size=strlen(dopt);
						dopt[size]=')'; 
						dopt[size+1]='\0';
						HC_Set_User_Options(dopt);
					}
					HC_Set_Driver_Options("fast silhouette edges=on");
				}

				if(HC_Show_Existence("color=windows"))
				{
					HC_Show_One_Color("windows", wcolor);
					sprintf(tmp, "saved wcolor=%s", wcolor);
					HC_Set_User_Options(tmp);
				}
				if(HC_Show_Existence("color=window contrast"))
				{
					HC_Show_One_Color("window contrast", wcolor);
					sprintf(tmp, "saved contrast wcolor=%s", wcolor);
					HC_Set_User_Options(tmp);
				}

				HC_Set_Color_By_Value("windows, window contrast", "RGB", m_FakeHLRColor.x, m_FakeHLRColor.y, m_FakeHLRColor.z);
				
				//fast silhouette edges come from view key
				if(HC_Show_Existence("color=edge"))
				{
					HC_Show_One_Color("edge", wcolor);
					sprintf(tmp, "saved ecolor=%s", wcolor);
					HC_Set_User_Options(tmp);
					if((m_FakeHLRColor.x + m_FakeHLRColor.y + m_FakeHLRColor.z)/3.0f < 0.2f)
						HC_Set_Color("edge = white");
				}

				if (use_hzb(m_sDriverType))
					HC_Set_Rendering_Options("hsra = hardware z-buffer");
				else
					HC_Set_Rendering_Options("hsra = szb");

				HC_Open_Segment_By_Key(m_FakeHLRSource);
					HC_Set_Color_By_Value("faces", "RGB", m_FakeHLRColor.x, m_FakeHLRColor.y, m_FakeHLRColor.z);
					if((m_FakeHLRColor.x + m_FakeHLRColor.y + m_FakeHLRColor.z)/3.0f < 0.2f)
						HC_Set_Color("lines=edges=white");
					else
						HC_Set_Color("lines=edges=black");
				HC_Close_Segment();					
			HC_Close_Segment();

			HC_Open_Segment_By_Key(HC_KShow_Owner_Original_Key(GetIncludeLinkKey()));
				m_FakeHLRStyle=HC_KStyle_Segment_By_Key(m_FakeHLRSource);				
			HC_Close_Segment();

			if (update_triad){
				HC_Open_Segment_By_Key(m_AxisTriadKey);
					HC_UnSet_One_Rendering_Option("hidden surface removal algorithm");
					HC_Set_Visibility("everything=off, faces = on, lines = on, lights = off, edges=(generics=off,perimeters=on,interior silhouettes=on, hard=on)");
				HC_Close_Segment();
			}

		} break;

		default:
			break;
	}
	m_RenderMode = RenderMode; 

	HSelectionSet * selection=GetSelection();
	if(selection)
		selection->UpdateHighlighting();

	if ((GetCuttingPlanesExist() && GetOperator() != 0) || GetHasObjects())
	{
//		GetSelection()->DeSelectAll();

		if (NewRenderMode == HRenderHiddenLine || NewRenderMode == HRenderHiddenLineFast 
			|| NewRenderMode == HRenderWireframe || NewRenderMode == HRenderBRepHiddenLine
			|| NewRenderMode == HRenderBRepHiddenLineFast
			|| NewRenderMode == HRenderBRepWireframe
			|| NewRenderMode == HRenderSilhouette)
		{
			HUtilityGeometryCreation::SetCuttingPlaneVisibilityForAll(GetCuttingPlanesKey(), "edges = on, faces = off");
		}
		else
		{
			HUtilityGeometryCreation::SetCuttingPlaneVisibilityForAll(GetCuttingPlanesKey(), "edges = off, faces = on");
		}


	}    // now send the message to the collaboration server
	if (emit_message){

	if(emit_message_function) {

		char message[MVO_BUFFER_SIZE];

		sprintf(message, "H_SET_RENDERMODE %s", rendermode);
		emit_message_function((const char *)message, 
			0, emit_message_function_user_data);

		}
	}
	Notify( HSignalRenderModeChanged, 0 );
}


void HBaseView::StartSharedKey()
{
		m_pSharedKey = new HSharedKey();
}




void HBaseView::Animate (int animation_steps,  HAnimateMode animate_flags, int MaxRotation)
{
	int             i;

	for (i = 0; i < animation_steps; i++)
	{

		if (animate_flags & HAnimateScene) 
		{
			HC_Open_Segment_By_Key (GetSceneKey());
				HC_Orbit_Camera ((float)MaxRotation/animation_steps, 0.0);
				SetCameraChanged();
			HC_Close_Segment ();
		}	                       

		
		ForceUpdate();
	}

	Update ();
}


HFileInputResult HBaseView::FileInput(const wchar_t * FileName, HInputHandler * handler, HInputHandlerOptions * in_options)
{

	if(!handler) return InputNotHandled;
	if(!FileName) return InputBadFileName;

	HFileInputResult result = InputNotHandled;

	HInputHandlerOptions local_options;
	HInputHandlerOptions * options = &local_options;

	if(in_options) options=in_options;

	options->m_pHBaseView=this;

	if(handler->GetInputStyle() == HInputHandlerStyleModel) {

		result = handler->FileInputByKey(FileName, GetModel()->GetModelKey(), options);
	}
	else if(handler->GetInputStyle() == HInputHandlerStyleImage) {

		HC_KEY image_key;

		result = handler->FileInputToImageKey(FileName, &image_key, GetModel()->GetModelKey(), options);
	}else
		result = InputNotHandled;


	return result;
}


HFileOutputResult HBaseView::FileOutput(const wchar_t * FileName, HOutputHandler * handler, HOutputHandlerOptions * options)
{
	HOutputHandlerOptions defaultOptions;

	if(!handler) return OutputNotHandled;
	if(!FileName) return OutputBadFileName;
	if(!options) {
		options = &defaultOptions;
		options->WindowWidth(6.4f);
		options->WindowHeight(4.8f);
		options->ImageDpi(100);
	}

	HFileOutputResult result = OutputNotHandled;

	options->View(this);
	HCamera camera;
	
	if(handler->GetOutputStyle() == HOutputHandlerStyleModel) {

		if (!options->m_pCamera)
		{
			GetCamera(&camera);
			options->m_pCamera = &camera;
		}

		//TODO we should extend this option to other Output styles
		if (options->m_Key == INVALID_KEY)
			options->m_Key = GetModel()->GetModelKey();
	
		result = handler->FileOutputByKey(FileName, options->m_Key, options);

		options->m_pCamera = 0;
	}
	else if(handler->GetOutputStyle() == HOutputHandlerStyleImage) {

		FramerateMode mode = m_FramerateMode;
		int steps = SimpList ? SimpList[1]->GetMaxSimpLevel() : 20;
		bool useLod = SimpList ? ((HFrExtent *)SimpList[1])->GetUseLod() : false;
		float target = GetMaximumUpdateTime();
		int threshold = GetCullingThreshold();

		SetFramerateMode(FramerateOff);
		
		if(options->m_Window_Width == 0 || options->m_Window_Height == 0 || options->m_Image_Dpi == 0) {

			result = OutputBadOptions;
		}
		else{
			char hsra_options[MVO_BUFFER_SIZE];
			char hlr_options[MVO_BUFFER_SIZE];
			char current_hsra[MVO_BUFFER_SIZE];
			char color[MVO_BUFFER_SIZE];
			char anti_aliasing[MVO_BUFFER_SIZE];
			char windowPattern[MVO_BUFFER_SIZE];
			char light_Scaling[MVO_BUFFER_SIZE];

			/* first find out the relevant options associated with the view */
			HC_Open_Segment_By_Key(GetViewKey());
				HC_Show_One_Net_Rendering_Optio("hidden surface removal algorithm", current_hsra);
			
				HC_Show_One_Net_Driver_Option("light scaling", light_Scaling);
				if (HC_Show_Existence("color")) {
					HC_Show_Color(color);
				}
				else {
					HPoint topColor;
					HPoint bottomColor;

					GetWindowColor	(topColor,bottomColor);
					HCLOCALE(sprintf(color, "windows=(r=%f g=%f b=%f), window contrast = (r=%f g=%f b=%f)", topColor.x, topColor.y, topColor.z, bottomColor.x, bottomColor.y, bottomColor.z));
				}

				HC_Show_Net_Window_Pattern(windowPattern);
			
				HRenderMode rndrmode = GetRenderMode();
				if (rndrmode == HRenderHiddenLine ||
					rndrmode == HRenderHiddenLineFast ||
					rndrmode == HRenderBRepHiddenLine ||
					rndrmode == HRenderBRepHiddenLineFast)
				{
					HC_Show_One_Net_Rendering_Optio("hidden line removal options", hlr_options);
					sprintf(hsra_options, "hsra = hidden line, hidden line removal options = (%s)", hlr_options);
				}
				else {

					if (options->HSRAOptions())
						sprintf(hsra_options, "hsra = %s", options->HSRAOptions());
					else if (handler->GetOutputDefaultHSRA())
						sprintf(hsra_options, "hsra = %s", handler->GetOutputDefaultHSRA());
					else if (use_hzb(GetDriverType()))
						sprintf(hsra_options, "hsra = hzb");
					else
						sprintf(hsra_options, "hsra = szb, technology = software frame buffer");
				}
				HC_Show_One_Net_Rendering_Option("anti-aliasing", anti_aliasing);
			HC_Close_Segment();

			char image_segment[MVO_SEGMENT_PATHNAME_BUFFER];
			HBestImageDriver driver_segment(GetDriverType());

			sprintf(image_segment,"?driver/null/hbaseview_%p", (void*)this);


			int x_size=(int)(options->m_Window_Width * options->m_Image_Dpi + 0.5f);
			int y_size=(int)(options->m_Window_Height * options->m_Image_Dpi + 0.5f);

			wchar_t ext[512];
			HUtility::FindFileNameExtension(FileName, ext);
			// prepare image for rendering
			HC_Open_Segment(image_segment);
				HC_KEY image_key;
				if (wcseq(ext, L"bmp"))
					image_key = HC_Insert_Image (0.0, 0.0, 0.0, "rgb", x_size, y_size, 0);
				else
					image_key = HC_Insert_Image (0.0, 0.0, 0.0, "rgba", x_size, y_size, 0);
			HC_Close_Segment ();

			char style_visibility_options[MVO_BUFFER_SIZE];
			style_visibility_options[0] = '\0';
			// prepare data to render
			HC_Open_Segment(driver_segment);
				//HC_Set_Rendering_Options("attribute lock = (color = (window))");
				//HC_Set_Color_By_Value("windows", "RGB", m_WindowColor.x, m_WindowColor.y, m_WindowColor.z);
				HC_Set_Window_Frame("off");
				HC_Set_Rendering_Options(hsra_options);
				//HC_Set_Rendering_Options("hsra = szb");

				HC_Set_Rendering_Options(
					H_FORMAT_TEXT("anti-alias = (%s), %s",
						anti_aliasing,
						options->m_ForceGrayscale ? "force grayscale = on" : "force grayscale = off"));

				H_FORMAT_TEXT drvopt(
					"use window id = (image key = %s%p),"
					"subscreen = (-1, 1, -1, 1),"
					"isolated,no gamma correction,"
					"physical size = (%g, %g),"
					"hardcopy resolution = %d",
					H_EXTRA_POINTER_FORMAT,
					(void*)image_key,
					options->WindowWidth()*2.54f,
					options->WindowHeight()*2.54f,
					options->ImageDpi());

				if (strcmp(light_Scaling, "off") != 0) {
					drvopt.Append(", light scaling = %s", light_Scaling);
				}

				HC_Set_Driver_Options (drvopt);

				HC_Set_Color(color);
								
				HC_Set_Window_Pattern(windowPattern);

				HC_Include_Segment_By_Key (GetSceneKey());

				if (GetRenderMode() == HRenderFakeHiddenLine) {
					HC_Open_Segment_By_Key (m_FakeHLRSource);
						HC_Show_Visibility (style_visibility_options);
						HC_Set_Visibility ("edges=interior silhouettes");
					HC_Close_Segment();
				}
			HC_Close_Segment ();

			bool fgs_scene = false;
			bool fgs_axis = false;
			if(options->m_ForceGrayscale){
				HC_Open_Segment_By_Key(options->m_pHBaseView->GetSceneKey());{
					fgs_scene = HUtility::PushAttribute("rendering options");
					HC_Set_Rendering_Options("force grayscale = on");
					HC_Open_Segment("axis");{
						fgs_axis = HUtility::PushAttribute("rendering options");
						HC_Set_Rendering_Options("force grayscale = on");
					}HC_Close_Segment();	
				}HC_Close_Segment();
			}

			HC_Update_One_Display(driver_segment);

			/* Restore the scene to the way it was before twopassprinting */
			if (style_visibility_options[0]) {
				HC_Open_Segment_By_Key (m_FakeHLRSource);
					HC_Set_Visibility (style_visibility_options);
				HC_Close_Segment();
			}

			if(options->m_ForceGrayscale){
				HC_Open_Segment_By_Key(options->m_pHBaseView->GetSceneKey());{
					if(fgs_scene) HUtility::PopAttribute("rendering options");
					else HC_UnSet_One_Rendering_Option("force grayscale");
					HC_Open_Segment("axis");{
						if(fgs_axis) HUtility::PopAttribute("rendering options");
						else HC_UnSet_One_Rendering_Option("force grayscale");
					}HC_Close_Segment();
				}HC_Close_Segment();
			}

			result = handler->FileOutputFromImageKey(FileName, image_key, options);

			HC_Delete_Segment(driver_segment);
			HC_Delete_Segment(image_segment);
		}

		SetFramerateMode(mode,target,threshold,useLod,steps);
	}
	else if(handler->GetOutputStyle() == HOutputHandlerStyleHardcopy) {

		char hsra_options[MVO_BUFFER_SIZE];
		char hlr_options[MVO_BUFFER_SIZE];
		char current_hsra[MVO_BUFFER_SIZE];

		/* first find out the relevant options associated with the view */
		HC_Open_Segment_By_Key(GetViewKey());
			HC_Show_One_Net_Rendering_Optio("hidden surface removal algorithm", current_hsra);

			HRenderMode rndrmode = GetRenderMode();
			if (rndrmode == HRenderHiddenLine ||
				rndrmode == HRenderHiddenLineFast ||
				rndrmode == HRenderBRepHiddenLine ||
				rndrmode == HRenderBRepHiddenLineFast)
			{
				HC_Show_One_Net_Rendering_Optio("hidden line removal options", hlr_options);
				sprintf(hsra_options, "hsra = hidden line, hidden line removal options = (%s)", hlr_options);
			}
			else {
				
				if(handler->GetOutputDefaultHSRA())
					sprintf(hsra_options, "hsra = %s", handler->GetOutputDefaultHSRA());
				else
					sprintf(hsra_options, "hsra = painters");
			}
		HC_Close_Segment();

		options->HSRAOptions(hsra_options);

		HPoint windowColor;
		GetWindowColor(windowColor);
		options->WindowColor(&windowColor);

		result = handler->FileOutputByKey(FileName, GetSceneKey(), options);
			
	}
	else if(handler->GetOutputStyle() == HOutputHandlerStyleHTML) {

		HOutputHandler * extra_handler = HDB::GetHIOManager()->GetOutputHandler("hsf");

		if(!extra_handler) return OutputNotHandled;

		// html is HOutputHandlerStyleModel
		result = handler->FileOutputByKey(FileName, GetModel()->GetModelKey(), options);

		if(result == OutputOK) {

			// now create a HSF file
			wchar_t hsf_name[MVO_BUFFER_SIZE];
			size_t n = wcslen(FileName);
			while (n && FileName[n] != L'.') n--;
			wcsncpy(hsf_name, FileName, n);
			hsf_name[n] = L'\0';
			wcscat(hsf_name, L".hsf");

			// hsf is HOutputHandlerStyleModel
			result =  extra_handler->FileOutputByKey(hsf_name, GetModel()->GetModelKey(), options);
		}
	}

	return result;
}


void HBaseView::InitAntialiasing()
{
	if (!m_pAntialiasing)
		m_pAntialiasing = new HUtilityAntialiasing(this);
}



unsigned long HBaseView::SetSignalNotify( int signal, HSignalNotifyFunction callback, void *user_data )
{
	struct signal_request *erq = new struct signal_request;
	erq->signal = signal;
	erq->callback = callback;
	erq->user_data = user_data;
	erq->cookie = HDB::GetNextCookie();
	if (m_SignalRequestsList == 0)
		m_SignalRequestsList = new_vlist( malloc, free );
	vlist_add_first( m_SignalRequestsList, erq );
	return erq->cookie;	
}

void HBaseView::UnSetSignalNotify( unsigned long cookie )
{
	struct signal_request *erq;

	assert(m_SignalRequestsList != 0);
	vlist_reset_cursor(m_SignalRequestsList);
	while (1) {
		erq = (struct signal_request *) vlist_peek_cursor(m_SignalRequestsList);
		if (erq == 0) {
			break;
		}
		else if (erq->cookie == cookie) {
			vlist_remove(m_SignalRequestsList, erq);
			delete erq;
			break;
		}
		else
			vlist_advance_cursor(m_SignalRequestsList);
	}
}

void HBaseView::Notify(int signal, void * signal_data)
{
	struct signal_request *erq;

	if (!m_SignalRequestsList)
		return;
	vlist_reset_cursor( m_SignalRequestsList );
	while (1) {
		erq = (struct signal_request *)vlist_peek_cursor(m_SignalRequestsList);
		if (erq == 0)
			break;
		if( erq->signal == signal ) {
			erq->callback( signal, signal_data, erq->user_data );
		}
		vlist_advance_cursor( m_SignalRequestsList );
	}
}

HMarkupManager * HBaseView::GetMarkupManager()
{
	if (!m_pMarkupManager) {
		m_pMarkupManager = new HMarkupManager(this);
	}

	return m_pMarkupManager;
}




void HBaseView::SetShadowMode (HShadowMode shadowMode, bool saveMode) 
{
#ifndef USE_DEPRECATED_SHADOWS
	HPlane shadow_pl;
	HPoint light_pos;

	if (shadowMode!=HShadowNone)
		ComputeShadowParams(shadow_pl, light_pos);
	else {
		shadow_pl.Set(0,0,0,0);
		light_pos.Set(0,0,0);
	}
	switch (shadowMode)
	{
		case HShadowNone: {
			HC_Open_Segment_By_Key(GetSceneKey());{
				HC_Set_Rendering_Options("simple shadow = off");
				HC_Open_Segment("shadows");{
					HC_Flush_Contents(".", "geometry, segments, includes");
				}HC_Close_Segment();
			}HC_Close_Segment();
		}
		break;
		case HShadowHard: {
			HC_KEY shadowkey;
			HC_Open_Segment_By_Key(GetSceneKey());{
				shadowkey = HC_KCreate_Segment("shadows");
			}HC_Close_Segment();
			HHardShadow myShadow(GetModel()->GetModelKey(), shadowkey);
			myShadow.SetShadowGroundPlane(shadow_pl);
			myShadow.SetShadowLightPos(light_pos);
//			SetShadowParams(&myShadow);
			myShadow.Create();
		}
		break;
		case HShadowSoft: {
			HC_Open_Segment_By_Key(GetSceneKey());{
				HC_Set_Rendering_Options("simple shadow = on");
			}HC_Close_Segment();
			HPoint color;
			color.Set(0.5,0.5,0.5);
		}
		break;
	}
	//note: this needs to be unconditional because shadow parameters are 
	// also used to position the reflection plane, if present
	if (shadowMode != HShadowNone)
		SetShadowParams( shadow_pl, light_pos , 0, 0);
#else
	HC_KEY shadowkey;
	switch (shadowMode)
	{
	case HShadowNone:
		if (m_ShadowMode != HShadowNone)
		{
			HC_Open_Segment_By_Key(GetSceneKey());{
				HC_Open_Segment("shadows");{
					HC_Flush_Contents(".", "geometry, segments, includes");
				}HC_Close_Segment();
			}HC_Close_Segment();
		}
		break;
	case HShadowHard:
		{
			HC_Open_Segment_By_Key(GetSceneKey());{
				shadowkey = HC_Create_Segment("shadows");
			}HC_Close_Segment();
			bool UseOglShadow;
			if (strcmp(GetDriverType(), "opengl") == 0)
				UseOglShadow = true;
			else
				UseOglShadow = false;
			HSmoothShadow myShadow(GetModel()->GetModelKey(), shadowkey, UseOglShadow);
			SetShadowParams(&myShadow);
			HPlane shadow_pl;
			HPoint light_pos;
			ComputeShadowParams(shadow_pl, light_pos);
 
			myShadow.Create();
		}
		break;
		
	case HShadowSoft:
		{
			HC_Open_Segment_By_Key(GetSceneKey());{
				shadowkey = HC_Create_Segment("shadows");
			}HC_Close_Segment();
			HHardShadow myShadow(GetModel()->GetModelKey(), shadowkey);
			HPlane shadow_pl;
			HPoint light_pos;
			ComputeShadowParams(shadow_pl, light_pos);
			myShadow.SetShadowGroundPlane(shadow_pl);
			myShadow.SetShadowLightPos(light_pos);

//			SetShadowParams(&myShadow);
			myShadow.Create();
		}
		break;
		
	}

#endif // USE_DEPRECATED_SHADOWS
	if (saveMode)
		m_ShadowMode = shadowMode;
}

void HBaseView::SetReflectionPlane(bool state, float opacity, bool fading, bool attenuate,
			float hither, float yon, bool blur, int blur_level)
{
	char opt [MVO_BUFFER_SIZE];

	if (state)
	{
		HPlane shadow_pl=HPlane(0,0,0,0);
		HPoint light_pos=HPoint(0,0,0);
		char refl_opt[MVO_BUFFER_SIZE];

		ComputeShadowParams( shadow_pl, light_pos );

		HCLOCALE(sprintf(opt, "simple reflection=(on, opacity=%f, fading= %s, ", opacity, (fading?"on":"off")));

		if (attenuate)
			HCLOCALE(sprintf(refl_opt, "attenuation = (hither=%f, yon=%f), ", hither, yon));
		else
			sprintf(refl_opt, "no attenuation, ");
		strcat(opt, refl_opt);

		if (blur)
			sprintf(refl_opt, "blur=%d)", blur_level);
		else
			sprintf(refl_opt, "no blur)");
		strcat(opt, refl_opt);

		SetShadowParams(shadow_pl, light_pos, 0, 0);
	}
	else
		sprintf(opt, "no simple reflection");

	HC_Open_Segment_By_Key (GetSceneKey());{
		HC_Set_Rendering_Options (opt);
	}HC_Close_Segment();
}

// returns the six faces as planes which forms the box from the given diagonal points
static void get_box_faces(const HPoint& min_pt, const HPoint& max_pt, HPlane* ret_pls)
{
	ret_pls[0] = HPlane(-1, 0, 0, min_pt.x);
	ret_pls[1] = HPlane(0, -1, 0, min_pt.y);
	ret_pls[2] = HPlane(0, 0, -1, min_pt.z);
	ret_pls[3] = HPlane(1, 0, 0, -max_pt.x);
	ret_pls[4] = HPlane(0, 1, 0, -max_pt.y);
	ret_pls[5] = HPlane(0, 0, 1, -max_pt.z);
}

// returns the plane which is lowest and farthest from the viewer
// assumes 6 planes, all in viewpoint system
static int get_lowest_and_farthest_plane_index(HPlane* pls)
{
	int pl_idx = 0;
	for (int i = 0; i < 6; i++)
	{
		if(pls[i].b() <= pls[pl_idx].b() && pls[i].c() > -0.1)
			pl_idx = i;
	}
	return pl_idx;
}

#ifndef BIG_FLOAT 
#	define BIG_FLOAT 1e10
#endif

/* given two corners, enumerate all of the points of an xyz-aligned cube. */
static void enumerate_points (
	HPoint const & p1,
	HPoint const & p2,
	HPoint * out)
{
	out[0].x = p1.x;	out[0].y = p1.y;	out[0].z = p1.z;
	out[1].x = p1.x;	out[1].y = p1.y;	out[1].z = p2.z;
	out[2].x = p1.x;	out[2].y = p2.y;	out[2].z = p1.z;
	out[3].x = p1.x;	out[3].y = p2.y;	out[3].z = p2.z;
	out[4].x = p2.x;	out[4].y = p1.y;	out[4].z = p1.z;
	out[5].x = p2.x;	out[5].y = p1.y;	out[5].z = p2.z;
	out[6].x = p2.x;	out[6].y = p2.y;	out[6].z = p1.z;
	out[7].x = p2.x;	out[7].y = p2.y;	out[7].z = p2.z;
}

void HBaseView::AdjustShadowPlane(HPlane & shadow_plane, HPoint const & light_pos)
{
	HPoint min_world_corner, max_world_corner;
	HPlane world_bbox_pls[6];
	shadow_plane.Set(light_pos.x, light_pos.y, light_pos.z, -BIG_FLOAT);

	HC_Open_Segment_By_Key(GetModel()->GetModelKey());{
		//HC_Compute_Circumcuboid(".", &min_world_corner, &max_world_corner);
		HC_Set_Rendering_Options("attribute lock=visibility");
		HC_Filter_Circumcuboid(".", "visibility=(everything=off, faces=on)", &min_world_corner, &max_world_corner);
		HC_UnSet_One_Rendering_Option("attribute lock");
		// expand the box a little
		float temp;
		temp = (max_world_corner.x - min_world_corner.x) / 100.0f;
		min_world_corner.x -= temp;
		max_world_corner.x += temp;
		temp = (max_world_corner.y - min_world_corner.y) / 100.0f;
		min_world_corner.y -= temp;
		max_world_corner.y += temp;
		temp = (max_world_corner.z - min_world_corner.z) / 100.0f;
		min_world_corner.z -= temp;
		max_world_corner.z += temp;
		get_box_faces(min_world_corner, max_world_corner, world_bbox_pls);
	}HC_Close_Segment();

	HPoint corners[8];
	enumerate_points(min_world_corner, max_world_corner, corners);

	if (m_UseExplicitShadowLight)
	{
		HPoint dummy;
		ComputeShadowParams(shadow_plane, dummy);
	}
	else
	{
		for (int i = 0; i < 8; i++) {
			float d = static_cast<float>(HC_Compute_Dot_Product(&corners[i], &light_pos));
			shadow_plane.d(MAX(shadow_plane.d(),d));
		}

		for (int i = 0; i < 6; i++)
		{
			double result = HC_Compute_Dot_Product(&world_bbox_pls[i], &light_pos);
			if (result < -0.9)
			{
				shadow_plane = world_bbox_pls[i];
				break;
			}
		}
	}
}


void HBaseView::ComputeShadowParams(HPlane & shadow_plane, HPoint & light_pos)
{
	HPoint min_world_corner, max_world_corner;
	float world2view[16];
	HPlane world_bbox_pls[6];
	HPlane view_bbox_pls[6];

	HC_Open_Segment_By_Key(m_OverwriteKey);{
		HC_Compute_Circumcuboid("SelectionConditionIncluder", &min_world_corner, &max_world_corner);
		// expand the box a little
		float temp;
		temp = (max_world_corner.x - min_world_corner.x) / 100.0f;
		min_world_corner.x -= temp;
		max_world_corner.x += temp;
		temp = (max_world_corner.y - min_world_corner.y) / 100.0f;
		min_world_corner.y -= temp;
		max_world_corner.y += temp;
		temp = (max_world_corner.z - min_world_corner.z) / 100.0f;
		min_world_corner.z -= temp;
		max_world_corner.z += temp;

		get_box_faces(min_world_corner, max_world_corner, world_bbox_pls);
	}HC_Close_Segment();

	HC_Open_Segment_By_Key(GetSceneKey());{
		HC_Compute_Transform(".", "world", "viewpoint", world2view);
		for (int i = 0; i < 6; i++)
			HC_Compute_Transformed_Plane( &world_bbox_pls[i], world2view, &view_bbox_pls[i]);
	}HC_Close_Segment();

	int pl_i = get_lowest_and_farthest_plane_index(view_bbox_pls);

	shadow_plane = world_bbox_pls[pl_i];

	// set the light so that it is at radius distance along the shadow plane normal
	if (m_UseExplicitShadowLight)
		light_pos.Set(m_ExplicitShadowLightDirection);
	else
		light_pos.Set(-shadow_plane.a(), -shadow_plane.b(), -shadow_plane.c());
}

void HBaseView::SetShadowColor(HPoint color)
{
	char shadow_opts[MVO_BUFFER_SIZE];

	HC_Open_Segment_By_Key(GetSceneKey());{
		HC_Open_Segment("shadows");{
			HC_Set_Color_By_Value("faces", "rgb",color.x, color.y, color.z);
		}HC_Close_Segment();
		HCLOCALE(sprintf(shadow_opts,
			"simple shadow = (color = '( r = %f g = %f b = %f)')",
			color.x, color.y, color.z
		));
		HC_Set_Rendering_Options(shadow_opts);
	}HC_Close_Segment();
}

void HBaseView::SetShadowResolution(int res)
{
	char shadow_opts[MVO_BUFFER_SIZE];

	HC_Open_Segment_By_Key(GetSceneKey());{
		sprintf( shadow_opts, "simple shadow = (resolution = %d)", res );
		HC_Set_Rendering_Options(shadow_opts);
	}HC_Close_Segment();
}
void HBaseView::SetShadowBlurring(int blur)
{
	char shadow_opts[MVO_BUFFER_SIZE];

	HC_Open_Segment_By_Key(GetSceneKey());{
		sprintf( shadow_opts, "simple shadow = (blurring = %d)", blur );
		HC_Set_Rendering_Options(shadow_opts);
	}HC_Close_Segment();
}

void HBaseView::SetShadowParams(
	HPlane const & shadow_plane,
	HPoint const & light_pos,
	HPoint const * color,
	char const * filter)
{
	char shadow_opts[MVO_BUFFER_SIZE];

	HC_Open_Segment_By_Key(GetSceneKey());{

		// set the plane and light pos
		HCLOCALE(sprintf( shadow_opts, "simple shadow = (plane = ( %f, %f, %f, %f), light = (%f, %f, %f) )", 
			shadow_plane.a(), shadow_plane.b(), shadow_plane.c(), shadow_plane.d(),
			light_pos.x, light_pos.y, light_pos.z ));
		HC_Set_Rendering_Options(shadow_opts);

		// set reflection plane at the same place for now, but aimed the other way
		HCLOCALE(sprintf(shadow_opts,
			"simple reflection = plane = (%f, %f, %f, %f)",
			-shadow_plane.a(), -shadow_plane.b(), -shadow_plane.c(), -shadow_plane.d()));
		HC_Set_Rendering_Options(shadow_opts);

		// set shadow color
		if (color)
		{
			HCLOCALE(sprintf(shadow_opts,
				"simple shadow = (color = '( r = %f g = %f b = %f)')",
				color->x, color->y, color->z
			));
			HC_Set_Rendering_Options(shadow_opts);
		}

		// set shadow filter
		if (filter)
		{
			sprintf(shadow_opts, "simple shadow = (blurring = %s)", filter);
			HC_Set_Rendering_Options(shadow_opts);
		}
		//	HC_Set_Rendering_Options("simple shadow = (resolution = 64)");
	}HC_Close_Segment();
}

void HBaseView::SetShadowParams(HShadow * shadowObj)
{
	HPlane shadow_pl;
	HPoint light_pos;

	// get the current viewup vector, bounding sphere and cuboids
	HPoint position, target, up;
	float width, height;
	char projection[MVO_BUFFER_SIZE];
	float radius;
	float center[3];
	HPoint min_world_corner, max_world_corner;
	min_world_corner.Set(0.1f, 0.1f, 0.1f);
	max_world_corner.Set(-0.1f, -0.1f, -0.1f);
	HPoint cuboid_corner[8];

	int lowest_pt_indx = 0;

	HC_Open_Segment_By_Key(GetSceneKey());{
		HC_PShow_Net_Camera(0, 0, &position, &target, &up, &width, &height, projection);
		HC_Compute_Circumsphere(".", center, &radius);
		HC_Compute_Circumcuboid(".", &min_world_corner, &max_world_corner);

		HUtility::GenerateCuboidPoints(&min_world_corner, &max_world_corner, cuboid_corner);

		for (int i = 0; i < 8; i++)
			HC_Compute_Coordinates(".", "object", &cuboid_corner[i], "viewpoint", &cuboid_corner[i]);

		// get the point which is lowest - meaning has least of Y coord 
		float min_y = cuboid_corner[0].y;
		for (int j = 1; j < 8; j++)
		{
			if (cuboid_corner[j].y < min_y)
			{
				min_y = cuboid_corner[j].y;
				lowest_pt_indx = j;
			}
		}
		// get the world coordinate for this lowest point
		HC_Compute_Coordinates(".",
			"viewpoint", &cuboid_corner[lowest_pt_indx],
			"world", &cuboid_corner[lowest_pt_indx]
		);
	}HC_Close_Segment();

	shadow_pl.a(up.x);
	shadow_pl.b(up.y);
	shadow_pl.c(up.z);

	// compute the d coef of shadow plane such that the normal of plane is up vec and
	// it passes through the lowest point which we computed above
	shadow_pl.d(-shadow_pl.a()*cuboid_corner[lowest_pt_indx].x
				-shadow_pl.b()*cuboid_corner[lowest_pt_indx].y
				-shadow_pl.c()*cuboid_corner[lowest_pt_indx].z);

	// set the light so that it is at radius distance along the shadow plane normal
	light_pos.Set(shadow_pl.a()*radius,shadow_pl.b()*radius,shadow_pl.c()*radius);

	// set the ground plane and light pos for the shadow
	shadowObj->SetShadowGroundPlane( shadow_pl );
	shadowObj->SetShadowLightPos( light_pos );
}

void HBaseView::SetColorInterpolation(bool onoff)
{
	HC_Open_Segment_By_Key(GetSceneKey());
		if (onoff)
			HC_Set_Rendering_Options("color interpolation = (faces = on, markers=on, edges = off)");
		else
			HC_Set_Rendering_Options("no color interpolation");
	HC_Close_Segment();	
	Update();
}

void HBaseView::SetColorIndexInterpolation(bool onoff, bool isolines)
{
	HC_Open_Segment_By_Key(GetSceneKey());{
		if (onoff)
		{
			if (isolines)
				HC_Set_Rendering_Options("color index interpolation = isolines");
			else
				HC_Set_Rendering_Options("color index interpolation = (faces = on, edges = off)");
		}
		else
			HC_Set_Rendering_Options("no color index interpolation");
	}HC_Close_Segment();
	Update();
}

bool HBaseView::GetColorIndexInterpolation(char * options)
{
	int retval = 0;
	HC_Open_Segment_By_Key(GetSceneKey());{
		retval = HC_Show_Existence("rendering options = color index interpolation");
		if(retval && options != 0){
			HC_Show_One_Rendering_Option("color index interpolation", options);
		}
	}HC_Close_Segment();
	return retval != 0;
}


bool HBaseView::GetFrameBufferEffectsOptions(char * options, int * border_out)
{
	char buffer[MVO_SMALL_BUFFER_SIZE];
	char temp[MVO_BUFFER_SIZE];
	int border = 0;
	bool active = false;
	if (options == NULL)
		options = buffer;
	char *ptr = options;

	*ptr = '\0';

	HC_PShow_One_Net_Driver_Option(0, 0, "fast silhouette edges", temp);
	if (strncmp (temp, "off", 3) != 0) {
		sprintf (ptr, "fast silhouette edges = (%s),", temp);
		ptr += strlen(ptr);
		active = true;
		if (border < 2)
			border = 2; //estimate 2 pixels radius for fast silhouettes
	}

	HC_PShow_One_Net_Driver_Option(0, 0, "ambient occlusion", temp);
	if (strncmp (temp, "off", 3) != 0) {
		sprintf (ptr, "ambient occlusion = (%s),", temp);
		ptr += strlen(ptr);
		active = true;
		if (border < 8)
			border = 8; //estimate 8 pixels radius for ssao
	}

	HC_PShow_One_Net_Driver_Option(0, 0, "depth of field", temp);
	if (strncmp (temp, "off", 3) != 0) {
		sprintf (ptr, "depth of field = (%s),", temp);
		ptr += strlen(ptr);
		active = true;
		if (border < 8)
			border = 8; //estimate 8 pixels radius for depth of field
	}

	HC_PShow_One_Net_Driver_Option(0, 0, "bloom", temp);
	if (strncmp (temp, "off", 3) != 0) {
		sprintf (ptr, "bloom = (%s),", temp);
		ptr += strlen(ptr);
		active = true;
		if (border < 64)
			border = 64; //estimate 64 pixels radius for bloom
	}

	if ((ptr = strrchr (options, ',')) != NULL)
		*ptr = '\0';
		
	if (border_out)
		*border_out = border;
	return active;
}


void HBaseView::SetCuttingPlanesVisibility(bool visible)
{
	m_bCuttingPlanesVisibility = visible;
	if (GetCuttingPlanesExist())
	{
		if (!visible)
		{
			HC_Open_Segment_By_Key(GetCuttingPlanesKey());{
				HC_Set_Visibility("faces  = off, edges = off, lines = off");
				HC_Set_Rendering_Options("attribute lock = visibility");
			}HC_Close_Segment();
		}
		else
		{
			HC_Open_Segment_By_Key(GetCuttingPlanesKey());{
				if (HC_Show_Existence("visibility"))
				{
					HC_Set_Visibility("faces = off, edges = off, lines = off");
					HC_UnSet_One_Rendering_Option("attribute lock = visibility");
				}
			}HC_Close_Segment();
		}
	}
}



int HBaseView::UpdateEvent(bool antialias, bool forceUpdate)
{
	UpdateInternal(antialias, forceUpdate);
	return HLISTENER_PASS_EVENT;
}


void HBaseView::SetViewUpdateMode(ViewUpdateMode mode,  float rate)
{
	if (mode == OnDemand)
	{
		if (m_pUpdateTimerClient)
			HDB::GetHTManager()->UnRegisterClient( m_pUpdateTimerClient );
	}
	else if (mode == Continuous || mode == Deferred)
	{
		if (m_pUpdateTimerClient)
			HDB::GetHTManager()->UnRegisterClient( m_pUpdateTimerClient );
		delete m_pUpdateTimerClient;

		float StartTime;
		HC_Show_Time(&StartTime);

		m_pUpdateTimerClient = new 	HTClient(
			rate,
			HTCS_PeriodicSkip,
			UpdateTick,
			this
		);
		m_pUpdateTimerClient->SetNextRequest(StartTime + rate);
		HDB::GetHTManager()->RegisterClient(m_pUpdateTimerClient);
	}
	if (mode == Deferred)
	{
		HC_Open_Segment_By_Key(GetSceneKey());{
			HC_Set_Heuristics("culling = maximum extent mode = defer");
		}HC_Close_Segment();
	}
	else
	{
		HC_Open_Segment_By_Key(GetSceneKey());{
			HC_Set_Heuristics("culling = maximum extent mode = none");
		}HC_Close_Segment();
	}

	m_ViewUpdateMode = mode;
}



HEventManager * HBaseView::GetEventManager()
{
	if(!m_pEventManager) {
		m_pEventManager = new HEventManager();
	}
	return m_pEventManager;
}

HUndoManager * HBaseView::GetUndoManager()
{
	if(!m_pUndoManager) {
		m_pUndoManager = new HUndoManager();
	}
	return m_pUndoManager;
}


HObjectManager * HBaseView::GetHObjectManager()
{
	if(!m_pObjectManager) {
		m_pObjectManager = new HObjectManager();
	}
	return m_pObjectManager;
}

bool HBaseView::UpdateTick(float request_time, float actual_time, void * user_data)
{
	UNREFERENCED(request_time);
	UNREFERENCED(actual_time);

	HBaseView * view = (HBaseView *)user_data;
	if (view->m_bRecordCamera)
	{
		float current_time;
		HC_Show_Time(&current_time);
		float timediff = (current_time-view->m_LastRecordTime) * 100.0f;
		HBhvUtility::MakeCameraKeyframe(view,timediff);
		view->m_LastRecordTime = current_time;
	}

	if(!view->GetSuppressUpdateTick() && view->GetModel()->GetFileLoadComplete())
		view->UpdateInternal(false, true);

	return true;
}


void HBaseView::SetOperator(HBaseOperator *NewOperator)
{
	if (m_pOperator)
		GetEventManager()->UnRegisterHandler((HMouseListener *)m_pOperator, HMouseListener::GetType());
	
	if (NewOperator)
		GetEventManager()->RegisterHandler((HMouseListener *)NewOperator, HMouseListener::GetType(), HLISTENER_PRIORITY_HIGH+10);
	
	m_pOperator = NewOperator;
}




void HBaseView::SetCuttingPlanesExist(bool state)
{
	m_bCuttingPlanesExist = state; 

	// adjust the backplane culling setting
	if (state)
	{
		// If capping faces are off, we should also turn off 
		// backplane culling so that the inside can be seen
		if (GetCPFaceVisibility() == false && GetBackplaneCulling())
		{
			m_bPrevBackplaneCullMode = true;
			SetBackplaneCulling(false);
		}
	}
	else
	{
		// restore backplane culling mode if we changed that
		if (m_bPrevBackplaneCullMode == true)
		{
			m_bPrevBackplaneCullMode = false;
			SetBackplaneCulling(true);
		}
	}
}

void HBaseView::SetFontSize(const char *font_size, bool emit_message)
{
	// construct the HOOPS option string
	char hoops_font_size_str[MVO_BUFFER_SIZE];
	sprintf (hoops_font_size_str, "size = %s", font_size);

	// set the font size on the scene key
	HC_Open_Segment_By_Key(GetSceneKey());{
		HC_Set_Text_Font(hoops_font_size_str);
	}HC_Close_Segment();

	// transmit the message to other clients if requested
	if (emit_message)
	{
		if (emit_message_function)
		{
			char message[MVO_BUFFER_SIZE];
			sprintf(message, "H_SET_FONT_SIZE %s", font_size);
			emit_message_function((const char *)message, 0, emit_message_function_user_data);
		}
	}
}


void HBaseView::SetFontSizeFromMessage(const char * in_data, unsigned int data_length)
{
	char data[MVO_BUFFER_SIZE];

	memcpy(data, in_data, data_length);
	data[data_length] = '\0';

	SetFontSize(data, false);
}


int HBaseView::OnLButtonDown(HEventInfo & hevent)
{
	if (GetAxisMode() != AxisOff)
	{
		HPoint new_pos = hevent.GetMouseWindowPos();
		HC_Open_Segment_By_Key(GetViewKey());{
			HC_Compute_Coordinates(".", "outer window", &new_pos, "local pixels", &new_pos);
		}HC_Close_Segment();
		if (new_pos.x >= 0
			&& new_pos.x <= m_AxisWindowSize
			&& new_pos.y >= m_YSize - m_AxisWindowSize
			&& new_pos.y <=m_YSize)
		{
			m_bLButtonDown = true;
			return HLISTENER_CONSUME_EVENT;
		}
	}
	return HLISTENER_PASS_EVENT;
	
}
int HBaseView::OnLButtonDblClk(HEventInfo & event)
{
	if (!m_bDisplayHandlesOnDblClk)
		return HLISTENER_PASS_EVENT;

	if (!GetOperator())
		return HLISTENER_PASS_EVENT;

	if (GetOperator()->OperatorStarted() ||	GetOperator()->OnLButtonDblClk(event) != HOP_NOT_HANDLED)
		return HLISTENER_PASS_EVENT;

	if(!GetSelection())
		return HLISTENER_PASS_EVENT;

	HPoint p = event.GetMouseWindowPos();
	SetDynamicHighlighting(false);
	GetSelection()->DeSelectAll();
	HC_KEY sel = HUtility::SelectUnderCursor(this, p.x, p.y, false);
	if (sel != -1)
	{
		HUtility::UnwindIncludes(sel);
		HC_Open_Segment_By_Key(HUtility::GrabSegment(sel));{
			if (HUtility::SegmentExists("cpbox"))
			{
				HUtilityGeomHandle::ClearAllHandles(0);
				HLISTENER_EVENT(HObjectManipulationListener, GetEventManager(), SetupHandlesEvent(this, -1, false));
			}
			else
			{
				sel = HUtility::GrabSegment(sel);
				HLISTENER_EVENT(HObjectManipulationListener, GetEventManager(), SetupHandlesEvent(this, sel, false));
			}
		}HC_Close_Segment();

		Update();
		return HLISTENER_CONSUME_EVENT;
	}
	return HLISTENER_PASS_EVENT;
}

int HBaseView::OnMouseMove(HEventInfo & hevent)
{
	int ret = HOP_OK;
	if (hevent.LButton() && !hevent.RButton())
		ret = OnLButtonDownAndMove(hevent);
	else if (hevent.RButton() && !hevent.LButton())
		ret = OnRButtonDownAndMove(hevent);
	else if (hevent.MButton() && !hevent.LButton() && !hevent.RButton())
		ret = OnMButtonDownAndMove(hevent);
	else if (hevent.LButton() && hevent.RButton())
		ret = OnLRButtonDownAndMove(hevent);
	else if (!hevent.LButton() && !hevent.RButton())
		ret = OnNoButtonDownAndMove(hevent);

	return ret;
}


int HBaseView::OnLButtonDownAndMove(HEventInfo & event)
{
	HPoint new_pos;
	new_pos = event.GetMouseWindowPos();

	DoDynamicHighlighting(new_pos);

	if (m_bLButtonDown)
	{
		if (m_pManipulateOperator)
		{
			HPoint new_pos = event.GetMousePixelPos();
			new_pos.y = m_YSize - new_pos.y;

			new_pos.x = new_pos.x / m_AxisWindowSize  * m_XSize;
			new_pos.y = m_YSize - (new_pos.y / m_AxisWindowSize  * m_YSize);

			event.SetPoint(event.GetType(), (int)new_pos.x, (int)new_pos.y, event.GetFlags());

			if (!m_bMouseMove)
				m_pManipulateOperator->OnLButtonDown(event);
			m_pManipulateOperator->OnLButtonDownAndMove(event);

			m_bMouseMove = true;
		}
		return HLISTENER_CONSUME_EVENT;
	}
	return HLISTENER_PASS_EVENT;
}


int HBaseView::OnRButtonDownAndMove(HEventInfo & event)
{
	HPoint new_pos;
	new_pos = event.GetMouseWindowPos();

	DoDynamicHighlighting(new_pos);
	return HLISTENER_PASS_EVENT;
}


int HBaseView::OnMButtonDownAndMove(HEventInfo & event)
{
	HPoint new_pos;
	new_pos = event.GetMouseWindowPos();

	DoDynamicHighlighting(new_pos);
	return HLISTENER_PASS_EVENT;
}


int HBaseView::OnLRButtonDownAndMove(HEventInfo & event)
{
	HPoint new_pos;
	new_pos = event.GetMouseWindowPos();

	DoDynamicHighlighting(new_pos);
	return HLISTENER_PASS_EVENT;
}

int HBaseView::OnNoButtonDownAndMove(HEventInfo & event)
{
	HPoint  new_pos;
	new_pos = event.GetMouseWindowPos();

	DoDynamicHighlighting(new_pos);
	return HLISTENER_PASS_EVENT;
}


int HBaseView::OnLButtonUp(HEventInfo & event)
{
	int rv = HLISTENER_PASS_EVENT;
	if (m_bLButtonDown && !m_bMouseMove)
	{
		bool done = false;
		HPoint new_pos = event.GetMouseWindowPos();
		
		HC_Open_Segment_By_Key(GetViewKey());{
			HC_Set_Driver_Options("selection proximity = 0.001");
		}HC_Close_Segment();
		HC_Open_Segment_By_Key(GetAxisTriadKey());{
			HC_Set_Rendering_Options("attribute lock=(selectability)");	
			HC_Compute_Coordinates(".", "outer window", &new_pos, "local window", &new_pos);

			if (new_pos.x >= -1 && new_pos.x <= 1 && new_pos.y >= -1 && new_pos.y <= 1)
			{ 
				HC_Set_Selectability("geometry = on");

				int res = HC_Compute_Selection(GetDriverPath(), ".", "v, selection level = entity, no related selection limit, visual selection = off", new_pos.x, new_pos.y);

				HC_Set_Selectability("everything = off");
				HPoint fa,ra,ta;
				GetViewAxis(&fa, &ta);
				HC_Compute_Cross_Product(&fa, &ta, &ra);
				if (res)
				{
					ViewMode viewmode = HViewIso;
					for (int i = 0; i < res; i++)
					{
						char segpath[MVO_BUFFER_SIZE];
						HC_Show_Selection_Pathname(segpath);
						if (strstr(segpath, "xyplane") && !done)
						{
							if (fa.z != 0)
								viewmode = HViewFront;
							else if (ra.z != 0)
								viewmode = HViewRight;
							else if (ta.z != 0)
								viewmode = HViewTop;
							done = true;
						}
						else if (strstr(segpath, "xzplane") && !done)
						{
							if (fa.y != 0)
								viewmode = HViewFront;
							else if (ra.y != 0)
								viewmode = HViewRight;
							else if (ta.y != 0)
								viewmode = HViewTop;
							done = true;
						}
						else if (strstr(segpath, "yzplane") && !done)
						{
							if (fa.x != 0)
								viewmode = HViewFront;
							else if (ra.x != 0)
								viewmode = HViewRight;
							else if (ta.x != 0)
								viewmode = HViewTop;
							done = true;
						}
						else if (strstr(segpath, "isoplane"))
						{
							viewmode = HViewIso;
							done = true;
						}
						if (done && viewmode == HViewIso)
							break;
						HC_Find_Related_Selection();
					}
					
					if (done)
					{
						SetViewMode(viewmode, true);
						Update();
					}
				}
			}
		}HC_Close_Segment();

		SetDefaultSelectionProximity(GetDefaultSelectionProximity());

		rv = HLISTENER_CONSUME_EVENT;
		
	}
	else
	{
		if (m_pManipulateOperator)
			m_pManipulateOperator->OnLButtonUp(event);
	}
	
	m_bLButtonDown = false;
	m_bMouseMove = false;
	
	return rv;
}

void HBaseView::DoDynamicHighlighting(HPoint mouse_pos)
{
	int res, offset1, offset2, offset3;
	char pathname[MVO_SEGMENT_PATHNAME_BUFFER], type[MVO_BUFFER_SIZE];
	HC_KEY primitive;

	if (!m_bDynamicHighlighting
		|| GetSuppressUpdateTick()
		|| GetSuppressUpdate()
		|| !GetModel()->GetFileLoadComplete())
		return;

	HC_Open_Segment_By_Key(GetViewKey());
		res = HC_Compute_Selection(".", "./scene/overwrite", "v, selection level = entity", mouse_pos.x, mouse_pos.y);
	HC_Close_Segment();

	// compute the selection using the HOOPS window coordinate of the pick location
	bool need_deselect = true;
	bool need_update = true;

	if (res)
	{
		HC_Show_Selection_Element(&primitive, &offset1, &offset2, &offset3);
		HC_Show_Selection_Pathname(pathname);

		int incl_count;
		int skey_count;
		char skey_type[MVO_BUFFER_SIZE];

		HC_Show_Selection_Keys_Count(&skey_count);

		HC_KEY * keys = new HC_KEY[skey_count];
		HC_KEY * incl_keys = new HC_KEY[skey_count];
		HC_Show_Selection_Keys(&skey_count, keys);

		incl_count = 0;
		for (int i = skey_count-1; i >= 0; i--)
		{
			HC_Show_Key_Type(keys[i], skey_type);
			if(strstr(skey_type, "include"))
			{
				incl_keys[incl_count] = keys[i];
				incl_count++;
			}
			else if (streq(skey_type, "reference")) {
				primitive = keys[i];
				break;
			}
		}

		// Get the type of the selected
		HC_Show_Key_Type(primitive, type);

		//if we have a shell with visible faces, we may need to select regions
		if (streq(type, "shell") && offset3 != -1)
		{
			int region;
			int lowest = 0;
			int highest = 0;

			if (m_pHighlight->GetAllowRegionSelection())
				HC_Show_Region_Range(primitive, &lowest, &highest);

			if (lowest != highest || lowest > 0)
			{
				HC_Open_Geometry(primitive);{
					HC_Open_Face(offset3);{
						HC_Show_Region(&region);
					}HC_Close_Face();
				}HC_Close_Geometry();

				need_deselect = false;

				if (!m_pHighlight->IsRegionSelected(primitive, incl_count, incl_keys, region))
				{
					m_pHighlight->DeSelectAll();
					m_pHighlight->SelectRegion(primitive, incl_count, incl_keys, region, false);
				}
				else
					need_update = false;

				goto DONE;
			}

			//NON-REGION SELECT FALLS THROUGH
		}

		need_deselect = false;

		if (!m_pHighlight->IsSelected( primitive, incl_count, incl_keys)) {
			if (m_pHighlight->GetSelectionLevel() != HSelectSegment) // never should fail for dynamic highlighting, but let's be nice and check
			{
				// the key is to a geometric entity.  If we are in segment selection mode,
				// then we need to get the key to its parent segment.

				HC_Show_Key_Type(primitive, type);

				if (!streq("segment", type))
				{
					char segname[MVO_BUFFER_SIZE];
					HC_KEY segkey;

					segkey = HC_KShow_Owner_Original_Key(primitive);
					HC_Show_Owner_By_Key(primitive, segname);

					// climb up one more level if this is the temporary highlight key
					if( m_pHighlight->IsHighlightSegment(segkey) )
					{
						segkey = HC_KShow_Owner_Original_Key(segkey);
						HC_Show_Owner_By_Key(segkey, segname);
					}
				}
			}
			m_pHighlight->DeSelectAll();
			m_pHighlight->Select( primitive, incl_count, incl_keys, false );
		}
		else
			need_update = false;

DONE:
		delete [] keys;
		delete [] incl_keys;
	}

	if (need_deselect) 
		m_pHighlight->DeSelectAll();

	if (need_update) 
		ForceUpdate();
}

void HBaseView::UpdateHighlighting()
{
	if(m_pSelection && m_pSelection->GetSize()>0)
		m_pSelection->UpdateHighlighting();

	if(m_pHighlight && m_pHighlight->GetSize()>0)
		m_pHighlight->UpdateHighlighting();
}


HC_KEY HBaseView::GetModelKey()
{
	return m_pModel->GetModelKey();
}


void HBaseView::SetVisibilitySelectionMode(bool onoff)
{
	HC_Open_Segment_By_Key(GetViewKey());
	if (onoff)
		HC_Set_Heuristics("visual selection = on");
	else
		HC_Set_Heuristics("visual selection = off");
	HC_Close_Segment();
}


bool HBaseView::GetVisibilitySelectionMode()
{
	char cval[MVO_BUFFER_SIZE];

	HC_Open_Segment_By_Key(m_ViewKey);{
		HC_Show_One_Net_Heuristic("visual selection", cval);
	}HC_Close_Segment();

	if (strstr(cval,"on"))
		return true;
	else
		return false;
}


void HBaseView::SetRelatedSelectionLimit(int limit)
{
	HC_Open_Segment_By_Key(GetViewKey());{
		if(limit != NO_RELATED_SELECTION_LIMIT){
			char heuristics[MVO_BUFFER_SIZE];
			sprintf(heuristics, "related selection limit = %d", limit);
			HC_Set_Heuristics(heuristics);
		}
		else {
			HC_Set_Heuristics("no related selection limit");
		}
	}HC_Close_Segment();
}

int HBaseView::GetRelatedSelectionLimit()
{
	int retval = 0;
	HC_Open_Segment_By_Key(GetViewKey());{
		char heuristics[MVO_BUFFER_SIZE] = {""};
		HC_Show_One_Net_Heuristic("related selection limit", heuristics);
		if(strstr(heuristics, "off") || strstr(heuristics, "no")){
			retval = -1;
		}
		else {
			retval = atoi(heuristics);
		}
	}HC_Close_Segment();
	return retval;
}
 
void HBaseView::SetAxisManipulateOperator(HOpCameraManipulate *op)
{
	delete m_pManipulateOperator;
	m_pManipulateOperator = op;
}

 
float HBaseView::GetIdleTime()
{
	float t;
	HC_Show_Time(&t);
	return t - m_IdleTime;
}
 

void HBaseView::ResetIdleTime()
{
	HC_Show_Time(&m_IdleTime);
}

bool HBaseView::GetCuttingPlanesExist(const char * name)
{
	bool exists = false;
	HC_Open_Segment_By_Key(GetCuttingPlanesKey());{
		if (name) 
			HC_Open_Segment(name);

		if (HC_QShow_Existence("...", "first match, geometry"))
			exists = true;

		if (name)
			HC_Close_Segment();
	}HC_Close_Segment();
	return exists;
}


/* If something is under the mouse, project the current target into the same plane as the item under the mouse. */
bool HBaseView::ComputeReasonableTarget(HPoint & new_tar, HPoint const & mouse_win, HPoint const & tar_orig)
{

	char selectability_sav[4096] = {""};

	if (HC_Show_Existence("selectability"))
		HC_Show_Selectability (selectability_sav);

	HC_Set_Selectability ("geometry=on");

	bool const selected = HC_Compute_Selection ("..", ".", 
		"v, selection proximity=0.1, related selection limit = -1, internal selection limit=-1, selection level = entity, visual selection = off",
		mouse_win.x, mouse_win.y) != 0;

	HC_UnSet_Selectability();

	if (selectability_sav[0] != '\0')
		HC_Set_Selectability (selectability_sav);	

	if (!selected)
		return false;

	HPoint selection;
	HC_Show_Selection_Position(0, 0, 0, &selection.x, &selection.y, &selection.z);
	HPoint	new_target = tar_orig;
	HUtility::AdjustPositionToPlane(this, new_target, selection);

	new_tar = new_target;

	return true;
}

/* Adjust the old field to the new target so the picture on the screen won't change. */
void HBaseView::ComputeNewField(
	float & width,
	float & height,
	HPoint const & new_tar,
	HCamera const & old_camera)
{
	HVector old_ptd = old_camera.target - old_camera.position;
	HVector new_ptd = new_tar - old_camera.position;

	float old_len = static_cast<float>(HC_Compute_Vector_Length(&old_ptd));
	float new_len = static_cast<float>(HC_Compute_Vector_Length(&new_ptd));

	float ratio = new_len / old_len;

	width = old_camera.field_width * ratio;
	height = old_camera.field_height * ratio;
}

#ifdef _MSC_VER
# include <float.h>
# define isinf(x) (!_finite(x))
# define isnan(x) _isnan(x)
# else
# define isinf(x) std::isinf(x)
#endif


static bool valid_float(float f)
{
	if(isinf(f) || isnan(f)){
		return false;
	}
	return true;
}

static bool valid_point(HPoint const & p)
{
	if(valid_float(p.x) && valid_float(p.y) && valid_float(p.z)){
		return true;
	}
	return false;
}

/* This code guarantees that whatever is under the mouse pointer will remain under the mouse pointer
 * after the camera has been changed.  This also chooses a reasonable new target based on the item
 * under the mouse pointer.
 */
int HBaseView::OnMouseWheel(HEventInfo & event) 
{
	float zDelta = static_cast<float>(event.GetMouseWheelDelta() / GetMouseWheelSensitivity() / 120.0 / 8.0);

	if (GetInvertMouseWheelZoom())
		zDelta *= -1;

	HC_Open_Segment_By_Key (GetSceneKey());

		/* Save the original camera for smooth transition. */
		HCamera orig;
		HC_Show_Net_Camera (&orig.position, &orig.target, &orig.up_vector, &orig.field_width, &orig.field_height, orig.projection);

		/* We need to fill in these values with the new camera. */
		HCamera adjusted = orig;

		if (event.Shift()) {
			if (zDelta >= 0) {
				adjusted.field_width *= 2.0f;
				adjusted.field_height *= 2.0f;
			}
			else {
				adjusted.field_width *= 0.5f;
				adjusted.field_height *= 0.5f;
			}
		}
		else {

			if(!event.Control()) {
				ComputeReasonableTarget(adjusted.target, event.GetMouseWindowPos(), orig.target);
				if (streq(orig.projection, "perspective"))
					ComputeNewField(adjusted.field_width, adjusted.field_height, adjusted.target, orig);
				adjusted.field_width *= fabs(1 + zDelta);
				adjusted.field_height *= fabs(1 + zDelta);
			}
			else {
				HVector const to_target = orig.target - orig.position;
				adjusted.target = orig.position + to_target * (1 - zDelta);
				if (streq(orig.projection, "orthographic")) {
					adjusted.field_width *= fabs(1 + zDelta);
					adjusted.field_height *= fabs(1 + zDelta);
				}
			}

			double diagonal_len = sqrt(pow(adjusted.field_width, 2) + pow(adjusted.field_height, 2));

			/* If the camera is about to be too small... */
			if (diagonal_len < GetZoomLimit()) {
				if (zDelta < 0) {
					goto BAILOUT;
				}
				adjusted.field_width = orig.field_width * fabs(1 + zDelta);
				adjusted.field_height = orig.field_height * fabs(1 + zDelta);
				diagonal_len = sqrt(pow(adjusted.field_width, 2) + pow(adjusted.field_height, 2));
				adjusted.target = orig.target;
			}
			/* If the camera is about to be too big... */
			if (diagonal_len > fabs(MVO_SQRT_MAX_FLOAT))
				goto BAILOUT;

			/* Shift the target slightly toward the mouse pointer. */
			HVector mwp = event.GetMouseWorldPos();
			HUtility::AdjustPositionToPlane(this, mwp, adjusted.target);
			adjusted.target += (adjusted.target - mwp) * zDelta;

			HVector dir_to_position = orig.position - orig.target;
			HC_Compute_Normalized_Vector(&dir_to_position, &dir_to_position);
			adjusted.position = adjusted.target + dir_to_position * static_cast<float>(2.5 * diagonal_len);
		}

		if (valid_point(adjusted.position) &&
			valid_point(adjusted.target) &&
			valid_point(adjusted.up_vector) &&
			valid_float(adjusted.field_width) &&
			valid_float(adjusted.field_height)){
			if (!adjusted.position.Equal(adjusted.target, GetZoomLimit())){
				PrepareForCameraChange();{
					HC_Set_Camera_Position(adjusted.position.x, adjusted.position.y, adjusted.position.z);
					HC_Set_Camera_Target(adjusted.target.x, adjusted.target.y, adjusted.target.z);
					HC_Set_Camera_Field(adjusted.field_width, adjusted.field_height);
				}CameraPositionChanged(true,false);
				Update();
				
				if(GetHandleOperator())
					GetHandleOperator()->OnNoButtonDownAndMove(event);
			}
		}

BAILOUT:

	HC_Close_Segment();

	return HLISTENER_CONSUME_EVENT;
}

int HBaseView::SetupHandlesEvent(HBaseView *view, HC_KEY key, bool complex) 
{
	char text[MVO_BUFFER_SIZE];
	char objectinfo[MVO_BUFFER_SIZE];

	if (m_pHandleOperator)
	{
		key = HObjectManager::FindHObjectSegment(key, text, objectinfo);
		if (strcmp(objectinfo,"ignore")!=0)
		{
			if (complex)
				HUtilityGeomHandle::SetupManipulator(key, view, 0, true, true, true);
			else
				HUtilityGeomHandle::SetupManipulator(key, view, 0, false, false, false);    
		}
		return HLISTENER_CONSUME_EVENT;
	}
	else
		return HLISTENER_PASS_EVENT;
}


void HBaseView::FlushProgressBar()
{
	HC_Open_Segment_By_Key(m_ProgressBarKey);
	if (HUtility::SegmentExists("progress"))
		HC_Delete_Segment("progress");
	HC_Close_Segment();
	//Update();
}

bool HBaseView::StreamProgressCallback(unsigned long so_far, unsigned long expected, void * user_data)
{ 
	/* Let's not divide by zero... */
	if (expected == 0){
		return false;
	}

	if (so_far > expected)
		so_far = expected;
	HC_Open_Segment_By_Key(((HBaseView *)user_data)->GetProgressBarKey());{
		HC_Open_Segment("progress");{
			HC_Set_Heuristics("culling = maximum extent = 0");

			HC_Open_Segment("outerouterbar");{
				HC_Set_Heuristics("culling = off");
				HC_Set_Color("edges = red, lines = red, faces = grey");
				HUtility::InsertRectangle(".", 0.745f, -0.974f, 0.985f,-0.925f, 1.0f, true);	
				HC_Set_Visibility("lines = on");
			}HC_Close_Segment();
			HC_Open_Segment("outerbar");{
				HC_Set_Color("edges = red, lines = red, faces = white");
				HUtility::InsertRectangle(".", 0.75f, -0.97f, 0.98f,-0.93f, 0.0f, true);	
				HC_Set_Visibility("lines = on");
			}HC_Close_Segment();
			HC_Set_Visibility("lights = off, edges = off, markers = off, lines = off");
			HC_Set_Rendering_Options("color interpolation = on, color index interpolation = on");
			HC_KEY boxkey = HUtility::InsertRectangle(".", 0.75f, -0.97f, 
				((float)so_far/(float)expected) * 0.23f + 0.75f, - 0.93f, -1.0f * (float)so_far/(float)expected, true);
			HC_Open_Geometry(boxkey);{
				HC_Open_Vertex(0);{
					HC_Set_Color("dark blue");
				}HC_Close_Vertex();
				HC_Open_Vertex(3);{
					HC_Set_Color("light blue");
				}HC_Close_Vertex();
				HC_Open_Vertex(2);{
					HC_Set_Color("light blue");
				}HC_Close_Vertex();
				HC_Open_Vertex(1);{
					HC_Set_Color("dark blue");
				}HC_Close_Vertex();
			}HC_Close_Geometry();

		}HC_Close_Segment();
	}HC_Close_Segment();

	return true;
}


void HBaseView::QueryDeviceCapabilities()
{

	char temp[MVO_BUFFER_SIZE];

	// make sure that an update has occurred
	if (m_bFirstUpdate)
	{
		// if we can't do HW phong but the RenderMode is 'Shaded', we must reset it to 'Gouraud'
		HC_Show_Device_Info(GetDriverPath(), "phong lighting", temp);

		if (strstr(temp, "no") && (m_RenderMode == HRenderShaded))
			SetRenderMode(HRenderGouraud);
	}

}

void HBaseView::SetHasObjects(bool hasObjects)
{
	if (hasObjects)
	{
		if (!m_bHasObjects && m_pHandleOperator)
		{
			GetEventManager()->RegisterHandler((HMouseListener *)m_pHandleOperator, HMouseListener::GetType(), HLISTENER_PRIORITY_HIGH + 1000000);
			m_bHasObjects = true;
		}
	}
	else
	{
		if (m_bHasObjects && m_pHandleOperator)
		{
			GetEventManager()->UnRegisterHandler((HMouseListener *)m_pHandleOperator, HMouseListener::GetType());
			m_bHasObjects = false;
		}
	}
}

void HBaseView::SetZoomLimit()
{
	HPoint minBound;
	HPoint maxBound;
	HC_Open_Segment_By_Key(GetModelKey());{
		if (!HC_Compute_Circumcuboid(".", &minBound, &maxBound)) {
			minBound = HPoint();
			maxBound = HPoint();
		}
	}HC_Close_Segment();
	
	HPoint dimensions = maxBound - minBound;

	float minDim = std::min(dimensions.x, std::min(dimensions.y, dimensions.z));
	float maxDim = std::max(dimensions.x, std::max(dimensions.y, dimensions.z));

	float limit = 0.000007f * std::max(minDim, 0.1f * maxDim);
	

	SetZoomLimit(limit);
}

void HBaseView::SetLightFollowsCamera(bool onoroff)
{
	m_bLightFollowsCamera = onoroff;
	SetupLights();
}

void HBaseView::SetDefaultSelectionProximity(float selectionproximity)
{
	char text[MVO_BUFFER_SIZE];
	m_DefaultSelectionProximity = selectionproximity;
	HCLOCALE(sprintf(text, "selection proximity = %f", m_DefaultSelectionProximity));
	HC_Open_Segment_By_Key(GetViewKey());{
		HC_Set_Driver_Options(text);
	}HC_Close_Segment();
}

void HBaseView::SetShadowRenderingMode(HShadowRenderingMode mode)
{
	HC_Open_Segment_By_Key(GetViewKey());{
		switch(mode) {
		case SoftwareShadow:
			HC_Set_Driver_Options("shadow preference = software");
			break;
		case HardwareShadow:
			HC_Set_Driver_Options("shadow preference = hardware");
			break;
		case SoftwareOpenglShadow:
			HC_Set_Driver_Options("shadow preference = software opengl");
			break;
		}
	}HC_Close_Segment();
}


HShadowRenderingMode HBaseView::GetShadowRenderingMode()
{
	char text[MVO_BUFFER_SIZE];
	HC_Open_Segment_By_Key(GetViewKey());{
		HC_Show_One_Driver_Option("shadow preference", text);
	}HC_Close_Segment();
	if (strcmp(text, "software") == 0)
		return SoftwareShadow;
	else if (strcmp(text, "hardware") == 0)
		return HardwareShadow;
	else 
		return SoftwareOpenglShadow;
}

void HBaseView::SetCutGeometryVisibility(HCutGeometryVisibility visibility)
{
	bool faces = false;
	bool edges = false;
	if (visibility & HCutGeometryVisibilityEdges) {
		edges = true;
	}

	if (visibility & HCutGeometryVisibilityFaces) {
		faces = true;
	}

	SetCPGeomVisibility(edges, faces);
}

HCutGeometryVisibility HBaseView::GetCutGeometryVisibility()
{
	int bits = 0;

	if(GetCPLineVisibility()){
		bits |= HCutGeometryVisibilityEdges;
	}

	if(GetCPFaceVisibility()){
		bits |= HCutGeometryVisibilityFaces;
	}

	HCutGeometryVisibility retval = HCutGeometryVisibility(bits);
	return retval;
}


int HBaseView::ObjectMovedEvent(HBaseView *view, HC_KEY key, HPoint *p) 
{
	UNREFERENCED(p);

	view->GetConstantFrameRateObject()->SetActivityType(MovementActivity);
	HUtilityGeometryCreation::AdjustSection(view, key);
	
	return HLISTENER_PASS_EVENT;
}


void HBaseView::SetRecordCamera(bool rc)
{
	HBhvBehaviorManager *pBehaviorManager = GetModel()->GetBhvBehaviorManager();

	if (rc)
	{
		GetModel()->SetBhvBehaviorManager(0);
		pBehaviorManager = GetModel()->GetBhvBehaviorManager();
		pBehaviorManager->SetActiveView(this);
		HC_Show_Time(&m_LastRecordTime);
		pBehaviorManager->SetCurrentTick(0);
		m_bRecordCamera = true;
		pBehaviorManager->SetTicksPerSecond(100);
		if (GetViewUpdateMode() == OnDemand)
			SetViewUpdateMode(Continuous);
	}
	else
		m_bRecordCamera = false;
		
}


void HBaseView::InjectEvent(HEventInfo const & in_hevent)
{
	HEventInfo hevent = in_hevent;
	switch(hevent.GetType())
	{
		//HE_NoEvent,   

	case HE_LButtonDown:
		HLISTENER_EVENT(HMouseListener, GetEventManager(), OnLButtonDown(hevent));
		break;
	case HE_LButtonUp:  
		HLISTENER_EVENT(HMouseListener, GetEventManager(), OnLButtonUp(hevent));
		break;
	case HE_LButtonDblClk:
		HLISTENER_EVENT(HMouseListener, GetEventManager(), OnLButtonDblClk(hevent));
		break;
	

	case HE_MButtonDown:
		HLISTENER_EVENT(HMouseListener, GetEventManager(), OnMButtonDown(hevent));
		break;
	case HE_MButtonUp:
		HLISTENER_EVENT(HMouseListener, GetEventManager(), OnMButtonUp(hevent));
		break;
	case HE_MButtonDblClk:
		HLISTENER_EVENT(HMouseListener, GetEventManager(), OnMButtonDblClk(hevent));
		break;

	case HE_RButtonDown:
		HLISTENER_EVENT(HMouseListener, GetEventManager(), OnRButtonDown(hevent));
		break;
	case HE_RButtonUp:
		HLISTENER_EVENT(HMouseListener, GetEventManager(), OnRButtonUp(hevent));
		break;
	case HE_RButtonDblClk:
		HLISTENER_EVENT(HMouseListener, GetEventManager(), OnRButtonDblClk(hevent));
		break;

	case HE_MouseMove:
		HLISTENER_EVENT(HMouseListener, GetEventManager(), OnMouseMove(hevent));
		break;

	case HE_MouseWheel:
		HLISTENER_EVENT(HMouseListener, GetEventManager(), OnMouseWheel(hevent));
		break;
	case HE_MouseWheelUp:
		HLISTENER_EVENT(HMouseListener, GetEventManager(), OnMouseWheel(hevent));
		break;
	case HE_MouseWheelDown:
		HLISTENER_EVENT(HMouseListener, GetEventManager(), OnMouseWheel(hevent));
		break;

	case HE_TouchesDown:
		HLISTENER_EVENT(HMouseListener, GetEventManager(), OnTouchesDown(hevent));
		break;
	case HE_TouchesUp:  
		HLISTENER_EVENT(HMouseListener, GetEventManager(), OnTouchesUp(hevent));
		break;
	case HE_TouchesMove:
		HLISTENER_EVENT(HMouseListener, GetEventManager(), OnTouchesMove(hevent));
		break;

	//not currently handled by inject_event
	case HE_Timer:
		break;
	case HE_KeyDown:
		HLISTENER_EVENT(HMouseListener, GetEventManager(), OnKeyDown(hevent));
		break;
	case HE_KeyUp:
		HLISTENER_EVENT(HMouseListener, GetEventManager(), OnKeyUp(hevent));
		break;
	case HE_Selection:
	default:
		break;
	}
}


void HBaseView::SetTransparency(char const * text, bool fast_z_sort)
{
	char tr[MVO_BUFFER_SIZE];
	
	if ( text && *text )
	{
		HC_Open_Segment_By_Key(GetViewKey());{
			sprintf(tr, "transparency = (%s, zsort options=%s)",
				text, fast_z_sort ? "fastest" : "nicest");
			HC_Set_Rendering_Options(tr);
		}HC_Close_Segment();
	}

	else
	{
		HDB::ReportError("Null or empty string passed to HBaseView::SetTransparency");
	}
}

void HBaseView::SetLineAntialiasing(bool onoff)
{
	m_bLineAntialiasing = onoff;
	HC_Open_Segment_By_Key(GetViewKey());{
		if (onoff)
			HC_Set_Rendering_Options("anti-alias = (lines)");
		else
			HC_Set_Rendering_Options("anti-alias = (no lines)");
	}HC_Close_Segment();
}

void HBaseView::SetTextAntialiasing(bool onoff)
{
	m_bTextAntialiasing = onoff;
	HC_Open_Segment_By_Key(GetViewKey());{
		if (onoff)
			HC_Set_Rendering_Options("anti-alias = (text)");
		else
			HC_Set_Rendering_Options("anti-alias = (no text)");
	}HC_Close_Segment();
}

void HBaseView::SetDepthOfFieldRange(float near, float far)
{
	char dopt[MVO_BUFFER_SIZE];
	if (near==far && near == 0.0f && HC_Show_Existence_By_Key(GetModelKey(), "bounding"))
	{
		char bounding_info[MVO_BUFFER_SIZE];
		HC_Show_Bounding_Info_By_Key(GetModelKey(), bounding_info);

		if (bounding_info[0] != '\0')
		{
			float radius = 0.0f;

			//cuboid
			if (bounding_info[5] == 'c')
			{
				HPoint pt1, pt2;
				HC_Show_Bounding_Cuboid_By_Key(GetModelKey(), &pt1, &pt2);
				radius = sqrt((pt2.x-pt1.x)*(pt2.x-pt1.x) + (pt2.y-pt1.y)*(pt2.y-pt1.y) + (pt2.z-pt1.z)*(pt2.z-pt1.z));
			}
			//sphere
			else if (bounding_info[5] == 's')
			{
				HPoint pt;
				HC_Show_Bounding_Sphere_By_Key(GetModelKey(), &pt, &radius);
			}
			
			if (radius > 0.0f)
			{
				near = radius * 0.001f;
				far = radius;
			}
		}
	}

	if (near != 0.0f || far != 0.0f)
	{
		HCLOCALE(sprintf(dopt, "depth of field = (near = %f, far = %f)", near, far));
		HC_Open_Segment_By_Key(GetViewKey());{
			HC_Set_Driver_Options(dopt);
		}HC_Close_Segment();
	}
}

void HBaseView::SetDepthOfField(bool onoff, float strength)
{
	char dopt[MVO_BUFFER_SIZE];
	HCLOCALE(sprintf(dopt, "depth of field = (%s, strength = %f)", 
		(onoff ? "on" : "off"), strength));
	HC_Open_Segment_By_Key(GetViewKey());{
		HC_Set_Driver_Options(dopt);
	}HC_Close_Segment();
}

void HBaseView::SetDynamicHighlighting(bool state)
{
	m_bDynamicHighlighting = state;
	if (!m_bDynamicHighlighting)
		m_pHighlight->DeSelectAll();
}

void HBaseView::SetDetailSelection(bool onoff)
{
	HC_Open_Segment_By_Key(GetViewKey());
	if (onoff)
		HC_Set_Heuristics("detail selection = on");
	else
		HC_Set_Heuristics("detail selection = off");
	HC_Close_Segment();
}

bool HBaseView::GetDetailSelectionMode()
{
	char cval[MVO_BUFFER_SIZE];

	HC_Open_Segment_By_Key(m_ViewKey);{
		HC_Show_One_Net_Heuristic("detail selection", cval);
	}HC_Close_Segment();

	return strstr(cval,"on") != 0;
}

void HBaseView::SetRespectSelectionCulling(bool onoff)
{
	HC_Open_Segment_By_Key(GetViewKey());
	if (onoff)
	{
		HC_Set_Heuristics("selection culling=maximum extent");
	}
	else
	{
		HC_Set_Heuristics("selection culling=no maximum extent");
	}
	HC_Close_Segment();
}

bool HBaseView::GetRespectSelectionCulling()
{
	char cval[MVO_BUFFER_SIZE];

	HC_Open_Segment_By_Key(m_ViewKey); {
		HC_Show_One_Net_Heuristic("selection culling=maximum extent", cval);
	}HC_Close_Segment();

	return streq(cval, "on");
}

void HBaseView::SetHighlightMode(HSelectionHighlightMode m)
{
	this->m_HighlightMode = m;
	m_pHighlight->SetHighlightMode(m);
}

void HBaseView::SetReferenceSelection(bool use_ref)
{
	m_pSelection->SetReferenceSelection(use_ref);
	m_pSelection->DeSelectAll();
	m_pHighlight->SetReferenceSelection(use_ref);
	m_pHighlight->DeSelectAll();
}


void HBaseView::SetCullingThreshold(int cs)
{
	char text[MVO_BUFFER_SIZE];
	if (cs > 0)
		sprintf(text, "culling = maximum extent = %d", cs);
	else
		sprintf(text, "culling = (no maximum extent)");

	HC_Open_Segment_By_Key(GetSceneKey());{
		HC_Set_Heuristics(text);
	}HC_Close_Segment();
}

int HBaseView::GetCullingThreshold()
{
	char text[MVO_BUFFER_SIZE];
	HC_Open_Segment_By_Key(GetSceneKey());{
		HC_PShow_One_Net_Heuristic(0, 0, "culling = maximum extent", text);
	}HC_Close_Segment();

	if (streq(text, "on"))
		return 1;
	else if (streq(text, "off"))
		return 0;

	return atoi(text);
}

void HBaseView::SetHardExtent(int cutoff)
{
	char text[MVO_BUFFER_SIZE];
	if (cutoff > 0)
		sprintf(text, "culling = hard extent = %d", cutoff);
	else
		sprintf(text, "culling = (no hard extent)");

	HC_Open_Segment_By_Key(GetSceneKey());{
		HC_Set_Heuristics(text);
	}HC_Close_Segment();
}

int HBaseView::GetHardExtent()
{
	char text[MVO_BUFFER_SIZE];
	HC_Open_Segment_By_Key(GetSceneKey());{
		HC_PShow_One_Net_Heuristic(0, 0, "culling = hard extent", text);
	}HC_Close_Segment();
	return atoi(text);
}


void HBaseView::BeginShellSelection()
{
	if (!m_bBeginShellSelection)
	{
		HC_Begin_Shell_Selection();
		m_bBeginShellSelection = true;
	}
}
void HBaseView::EndShellSelection()
{
	if (m_bBeginShellSelection)
	{
		HC_End_Shell_Selection();
		m_bBeginShellSelection = false;
	}
}


void HBaseView::SetDefaultHSRA()
{
	if (use_hzb(m_sDriverType))
		HC_Set_Rendering_Options("hsra = hardware z-buffer");
	else
		HC_Set_Rendering_Options("hsra = szb");
}

int HBaseView::OnKeyDown(HEventInfo & event)
{
	unsigned int c = event.GetChar();
	if (c > 255)
		return HLISTENER_PASS_EVENT;
	m_KeyMap[c] = true;
	m_KeyFlags = 0;
	if (event.LeftShift())
		m_KeyFlags |= MVO_LEFT_SHIFT;
	if (event.Shift())
		m_KeyFlags |= MVO_SHIFT;
	if (event.RightShift())
		m_KeyFlags |= MVO_RIGHT_SHIFT;
	if (event.Control())
		m_KeyFlags |= MVO_CONTROL;
	
	return HLISTENER_PASS_EVENT;
}

int HBaseView::OnKeyUp(HEventInfo & event)
{
	unsigned int c = event.GetChar();
	if (c > 255)
		return HLISTENER_PASS_EVENT;
	m_KeyMap[c] = false;
	m_KeyFlags = 0;
	if (event.LeftShift())
		m_KeyFlags |= MVO_LEFT_SHIFT;
	if (event.Shift())
		m_KeyFlags |= MVO_SHIFT;
	if (event.RightShift())
		m_KeyFlags |= MVO_RIGHT_SHIFT;
	if (event.Control())
		m_KeyFlags |= MVO_CONTROL;

	return HLISTENER_PASS_EVENT;
}



bool HBaseView::GetKeyState(unsigned int key, int & flags)
{
	if (m_pfKeyState)
	{
		return m_pfKeyState(key, flags);
	}
	key = tolower(key);
	if (key > 255)
		return false;
	flags = m_KeyFlags;

	return m_KeyMap[key];
}


void HBaseView::ResetKeyState()
{
	memset(m_KeyMap, 0,256);
	m_KeyFlags = 0;
}


void HBaseView::SetGeometryChanged() 
{
	m_bGeometryChanged = true;
	InvalidateSceneBounding();
	ResetIdleTime();
}

void HBaseView::SetSceneBounding(HPoint &min, HPoint &max)
{
	m_SceneBoundingMin = min;
	m_SceneBoundingMax = max;
	m_bSceneBoundingSet = true;
}


void HBaseView::GetSceneExtents(HPoint &min, HPoint &max)
{
	HC_Open_Segment_By_Key(GetSceneKey());{
		// do a filter check to make sure we account for visibility settings
		if (GetSceneBoundingSet() && m_bFastFitWorld)
		{
			min = m_SceneBoundingMin;
			max = m_SceneBoundingMax;
		}
		else if (m_bDetailedFitWorld)
		{
			SmartFilterCircumcuboid(".", "detailed, respect locking", &min, &max);
			SetSceneBounding(min, max);
		}
		else
		{
			SmartFilterCircumcuboid(".", "respect locking", &min, &max);
			SetSceneBounding(min,max);
		}
	}HC_Close_Segment();
}


void HBaseView::SetEventCheckerCallback(void (*EventChecker)(HIC_Rendition const * nr))
{
	static int doOnce = true;
	m_pfEventChecker = EventChecker;
	if (doOnce && EventChecker)
	{
		HImRegisterCallback("abort_window_event_checker", EventChecker);
		doOnce = false;
	}
}

HViewSelectionLevel HBaseView::GetViewSelectionLevel()
{
	char heur[MVO_BUFFER_SIZE];

	HC_Open_Segment_By_Key(m_ViewKey);{
		HC_PShow_One_Net_Heuristic(0, 0, "selection level", heur);
	}HC_Close_Segment();

	if(streq(heur, "entity") || streq(heur, "geometry"))
		return HSelectionLevelEntity;
	else if(streq(heur, "segment"))
		return HSelectionLevelSegment;
	else
		return HSelectionLevelSegmentTree;
}

void  HBaseView::SetViewSelectionLevel(HViewSelectionLevel level)
{
	char heur[MVO_BUFFER_SIZE];

	if(level == HSelectionLevelEntity)
		sprintf(heur, "selection level = entity");
	else if(level == HSelectionLevelSegment)
		sprintf(heur, "selection level = segment");
	else
		sprintf(heur, "selection level = segment tree");

	HC_Open_Segment_By_Key(m_ViewKey);{
		HC_Set_Heuristics(heur);
	}HC_Close_Segment();
}
void HBaseView::EnableTargetFrameRate(bool enable, int target, int threshold, int steps)
{
	if(enable)
		SetFramerateMode(FramerateTarget, static_cast<float>(target), threshold, false, steps);
	else if (m_FramerateMode == FramerateTarget)
		SetFramerateMode(FramerateOff, (float)target, threshold, false, steps);
}

namespace {
	class HFrExtent2 : public HFrExtent {
	public:
		HFrExtent2(int min, int max) : HFrExtent(min, max) { return; }
		int cullingThreshold;
	};
};

float HBaseView::GetFramerate()
{
	switch (m_FramerateMode) {
		case FramerateOff:
			return -1.0f;
		case FramerateTarget:
			return 1.0f / GetConstantFrameRateObject()->GetTargetFrameRate();
		case FramerateFixed:
			return GetMaximumUpdateTime();
		default:
			assert(0);
			return -1.0f;
	}
}

FramerateMode HBaseView::GetFramerateMode()
{
	return m_FramerateMode;
}

int HBaseView::GetFramerateCullingThreshold()
{
	switch (m_FramerateMode) {
		case FramerateOff:
			return GetCullingThreshold();
		case FramerateTarget:
			return static_cast<HFrExtent2 *>(SimpList[1])->cullingThreshold;
		case FramerateFixed:
			return GetCullingThreshold();
		default:
			assert(0);
			return -1;
	}
}

int HBaseView::GetFramerateSteps()
{
	switch (m_FramerateMode) {
		case FramerateOff:
			return -1;
		case FramerateTarget:
			return static_cast<HFrExtent2 *>(SimpList[1])->GetMaxSimpLevel();
		case FramerateFixed:
			return -1;
		default:
			assert(0);
			return -1;
	}
}

bool HBaseView::IsFramerateLodEnabled()
{
	switch (m_FramerateMode) {
		case FramerateOff:
			return false;
		case FramerateTarget:
			return static_cast<HFrExtent2 *>(SimpList[1])->GetUseLod();
		case FramerateFixed:
			return false;
		default:
			assert(0);
			return false;
	}
}

void HBaseView::SetFramerateMode(FramerateMode mode, float target, int cullingthreshold, bool enableLods, int steps, int cutoff)
{
	if(mode != FramerateTarget)
	{
		HConstantFrameRate *cframerate = GetConstantFrameRateObject();
		cframerate->Stop();
		cframerate->Shutdown();
		cleanupSimpList();
	}
	if (m_FramerateMode == FramerateOff)
	{
		m_OldCullingThreshold = GetCullingThreshold();
		m_OldViewUpdateMode = GetViewUpdateMode();
	}


	switch (mode)
	{
		case FramerateOff:
		{
			SetHardExtent(0);
			if (m_OldCullingThreshold != -1)
			{
				SetCullingThreshold(m_OldCullingThreshold);
				SetViewUpdateMode(m_OldViewUpdateMode);
				SetMaximumUpdateTime(0.0f);
				m_OldCullingThreshold = -1;
			}
		}
		break;
		case FramerateTarget:
		{
			SetHardExtent(0);
			SetCullingThreshold(1);

			HConstantFrameRate *cframerate = GetConstantFrameRateObject();

			if(steps<=0)
				steps=0;

			cleanupSimpList();
		
			SimpList = new HConstFRSimpType *[2];
			simpListLength = 2;

			HFrExtent2 * frExtent = new HFrExtent2(0, steps);
			SimpList[0] = new HFrNoSimp(0, 2);
			SimpList[1] = frExtent;


			frExtent->SetMultiplier(cullingthreshold / (steps>0 ? steps : 1));
			frExtent->cullingThreshold = cullingthreshold;
			if (enableLods)
				frExtent->SetUseLod(true);
			cframerate->Init(1.0f/target, SimpList, 2);
			if (steps)
			{

				cframerate->SetDetailIncMode((DetailChangeMode)AllowDetailChange);
				cframerate->SetDetailIncOnIdleMode(NoDetailChange);
			}
			else
			{
				cframerate->SetDetailIncMode((DetailChangeMode)NoDetailChange);
				cframerate->SetDetailIncOnIdleMode(NoDetailChange);
			}
			

			cframerate->AdjustDefaultDetailLevelToModel();
			cframerate->Start();
		}
		break;
		case FramerateFixed:
		{
			SetCullingThreshold(cullingthreshold);
			SetHardExtent(cutoff);
			SetViewUpdateMode(Deferred);
			SetMaximumUpdateTime(target);
		}
		break;
		default:
			assert(0);
	}

	m_FramerateMode = mode;
}


void HBaseView::cleanupSimpList()
{
	for (int i = 0; i < simpListLength; i++)
		delete SimpList[i];

	delete [] SimpList;

	simpListLength = 0;
	SimpList = 0;
}

bool HBaseView::IsFrameRateEnabled()
{
	HConstantFrameRate * cframerate = GetConstantFrameRateObject();
	if (cframerate)
	{
		HConstFRSimpType ** sd;
		float fr;
		int len;
		cframerate->GetMode(&sd, &fr, &len);
		if (sd)
			return true;
	}
	return false;
}



void HBaseView::SetLightScaling(float scaling)
{
	float r,g,b;
	char text[MVO_BUFFER_SIZE];
	HC_Open_Segment_By_Key(GetViewKey());{
		HC_Show_One_Net_Color_By_Value("ambient", text, &r, &g, &b);
		if (scaling == 0)
			HC_Set_Driver_Options("no light scaling");
		else
		{
			char text[MVO_BUFFER_SIZE];
			HCLOCALE(sprintf(text, "light scaling = %f", scaling + r));
			HC_Set_Driver_Options(text);
		}
	}HC_Close_Segment();
}




bool HBaseView::GetViewActive()
{
	return GetIncludeLinkKey() != INVALID_KEY;
}

void HBaseView::ExhaustiveUpdate()
{
	int culling_thresh = GetCullingThreshold();
	int hard_extent = GetHardExtent();
	ViewUpdateMode update_mode = GetViewUpdateMode();
	float max_update_time = GetMaximumUpdateTime();

	SetCullingThreshold(0);
	SetHardExtent(0);
	SetViewUpdateMode(OnDemand);
	SetMaximumUpdateTime(0.0f);

	char tmp[MVO_BUFFER_SIZE];
	HC_Show_Device_Info_By_Key(GetViewKey(), "alive", tmp);

	if (GetModel()->GetStaticModelOn()) {
		HC_Control_Update_By_Key(GetModelKey(), "unset redraw");
		HC_Control_Update_By_Key(GetViewKey(), "refresh, compile only");
		ForceUpdate();
	}

	if (GetDisplayListMode() && GetDisplayListType() == DisplayListSegment) {
		HC_Control_Update_By_Key(GetViewKey(), "refresh, compile only");
		ForceUpdate();
	}
	HC_Control_Update_By_Key(GetViewKey(), "set shadow regeneration");

	SetCullingThreshold(culling_thresh);
	SetViewUpdateMode(update_mode);
	SetMaximumUpdateTime(max_update_time);
	SetHardExtent(hard_extent);
}


void HBaseView::SetDoubleBuffering(bool double_buffer)
{
	char driver_opts[4096];

	sprintf(driver_opts, "double-buffering = %s", double_buffer ? "on" : "off");
	HC_Open_Segment_By_Key(GetViewKey());{
		HC_Set_Driver_Options(driver_opts);
	}HC_Close_Segment();
}


void HBaseView::UpdateFakeHLRColor(HPoint paper_color)
{
	char color[MVO_BUFFER_SIZE];

	HCLOCALE(sprintf(color,
		"faces=windows=window contrast=(r=%f g=%f b=%f)",
		paper_color.x, paper_color.y, paper_color.z
	));

	HC_Open_Segment_By_Key(m_FakeHLRSource);{
		HC_Set_Color(color);
	}HC_Close_Segment();
}



//remove ', " or ` from the first and last position of the character string
static void strip_quotes(char *str)
{
	if (str[0] == '\'' ||
		str[0] == '"' ||
		str[0] == '`') {
		int i, len;
		char *ptr;

		len = (int)strlen(str);
		ptr = str;
		for (i = 0 ; i < len-2 ; i++) {
			ptr[i] = ptr[i + 1];
		}
		ptr[i] = '\0';
	}
}

#if 0
#include <stdio.h>
#include <stdarg.h>
#include <Windows.h>

inline void dprintf (char const * format, ...) {
	char buffer[1024];
	va_list arguments;

	va_start (arguments, format);
	vsprintf (buffer, format, arguments);
	va_end (arguments);
#ifdef _DEBUG
#ifdef _MSC_VER
#ifndef _WIN32_WCE
		OutputDebugString (buffer);
#endif
#endif
#endif
		fprintf(stdout, buffer);
		fflush(stdout);
}
#endif

void HBaseView::SetSplatSize(int splat_size)
{
	// Different drivers max out their rendered marker sizes differently with "marker drawing = fastest" enabled.
	// This map is designed to take that into consideration when dealing with splat size slider in the partviewer.
	static std::map<std::string, double> max_pt_map;
	if (max_pt_map.empty()) {
		max_pt_map["dx9"] = 33.0;
		max_pt_map["dx11"] = 70.0;
		max_pt_map["opengl"] = 70.0;
		max_pt_map["opengl2"] = 33.0;
	}

	int const min_splat_size = 1;
	int const max_splat_size = 100;

	splat_size = std::max(min_splat_size, splat_size);
	splat_size = std::min(max_splat_size, splat_size);

	m_SplatSize = splat_size;

	if (!m_bSplatRendering)
		return;

	if (m_SplatSize == min_splat_size) {
		HC_Open_Segment_By_Key(GetSceneKey());{
			HC_Set_Marker_Size(0);
		}HC_Close_Segment();
	}
	else {
		struct {
			double operator() (double x) const {
				double const base = 1.5;
				double const offset = -1;
				double const y = std::pow(base, 8.47 * x + offset) - std::pow(base, offset) + 0.3;
				return y;
		}
		} f;

		double const max_x = 1.0;
		double const max_y = f(max_x);

		double const x = static_cast<double>(splat_size) / static_cast<double>(max_splat_size);
		double const y = f(x);

		double max_pt = max_pt_map[m_sDriverType];
		if (max_pt == 0.0) {
			max_pt = 33.0;
		}

		double const pt = max_pt * (y / max_y);

		HC_Open_Segment_By_Key(GetSceneKey());{
			HC_Set_Variable_Marker_Size(H_FORMAT_TEXT("%f pt", pt));
		}HC_Close_Segment();
	}
	Update();
}


void HBaseView::SetSplatSymbol(SplatSymbol symbol)
{
	m_SplatSymbol = symbol;

	if (!m_bSplatRendering)
		return;

	HC_Open_Segment_By_Key(GetSceneKey());{
		switch (m_SplatSymbol) {
			case SplatSquare: {
			HC_Set_Marker_Symbol("[*]");
			} break;
			case SplatRound: {
			HC_Set_Marker_Symbol("(*)");
			} break;
			default: {
				assert(false);
			}
		}
	}HC_Close_Segment();

	Update();
}

void HBaseView::SetSplatRendering(bool onoff)
{
	char temp[128], orig[128];

	bool was_splat_rendering = m_bSplatRendering;

	m_bSplatRendering = onoff;

	HC_Open_Segment_By_Key(GetSceneKey());{
		if(m_bSplatRendering)
		{
			if (!was_splat_rendering)
			{
				if (HC_Show_Existence("marker size"))
				{
					HC_Show_Variable_Marker_Size(orig);
					sprintf(temp, "saved marker size = %s", orig);
					HC_Set_User_Options(temp);
				}

				if (HC_Show_Existence("marker symbol"))
				{
					HC_Show_Marker_Symbol(orig);
					sprintf(temp, "saved marker symbol = '%s'", orig);
					HC_Set_User_Options(temp);
				}
				
			}

			SetSplatSize(GetSplatSize());
			SetSplatSymbol(GetSplatSymbol());

			HC_Set_Rendering_Options("attribute lock = marker size");
			HC_Set_Rendering_Options("attribute lock = marker symbol");
		}
		else
		{
			if (was_splat_rendering)
			{
				if (HC_Show_Existence("rendering options = attribute lock = marker size"))
					HC_UnSet_One_Rendering_Option("attribute lock = marker size");

				if (HC_Show_Existence("rendering options = attribute lock = marker symbol"))
					HC_UnSet_One_Rendering_Option("attribute lock = marker symbol");

				if (HC_Show_Existence("user option = saved marker size"))
				{
					HC_Show_One_User_Option("saved marker size", orig);
					HC_Set_Variable_Marker_Size(orig);
				}

				if (HC_Show_Existence("user option = saved marker symbol"))
				{
					HC_Show_One_User_Option("saved marker symbol", orig);
					strip_quotes (orig);
					HC_Set_Marker_Symbol(orig);
				}
			}
		}
	}HC_Close_Segment();

	Update();
}

void HBaseView::StaticForceUpdateRequest(void * view_pointer)
{
	HBaseView * view = static_cast<HBaseView *>(view_pointer);
	if (!view->m_UpdateInterrupted) {
		HC_Control_Update_By_Key(view->GetViewKey(), "inspect");
		view->ForceUpdate();
	}
}


void HBaseView::VisualizeBounding(bool draw_something, HC_KEY key, int incl_count, HC_KEY * incl_path)
{
	HC_KEY bounding_seg;
	char bounding_info[MVO_BUFFER_SIZE] = "";
	float matrix[16];
	char * density_info;
	float density;
	const float MAX_COLOR_HUE=240.0f;
	char type[MVO_BUFFER_SIZE];
	
	//open the bounding segment (a child of the overwrite key) and flush it
	HC_Open_Segment_By_Key(GetOverwriteKey());{
		bounding_seg = HC_Open_Segment("bounding");{
			HC_Flush_Contents(".", "everything");
		}HC_Close_Segment();
	}HC_Close_Segment();


	if (draw_something && key != INVALID_KEY)
	{
		HC_Show_Key_Type(key, type);

		//only consider types that could have a bounding
		if (!streq(type, "segment")
			&& !streq(type, "reference")
			&& !streq(type, "shell")
			&& !streq(type, "mesh"))
			return;

		HC_Show_Bounding_Info_By_Key(key, bounding_info);

		// try referee
		while (streq(bounding_info, "") && streq(type, "reference")) {
			int count;
			HC_Show_Reference_Keys_Count (key, &count);
			if (count == 1) {
				key = HC_KShow_Reference_Geometry(key);
				//only consider types that could have a bounding
				HC_Show_Key_Type(key, type);
				if (!streq(type, "segment")
					&& !streq(type, "reference")
					&& !streq(type, "shell")
					&& !streq(type, "mesh"))
					return;
				HC_Show_Bounding_Info_By_Key(key, bounding_info);
			}
			else
				return;
		}
		
		//figure out whether we have bounding info at all
		if (bounding_info[0] != '\0')
		{
			HC_PShow_Net_Modelling_Matrix(incl_count, incl_path, matrix);
			
			//get the bounding type and relevant points
			//insert the geometry and matrix into the bounding segment
			density_info = strstr(bounding_info, "density");
			if (density_info)
			{
				density_info += 8;
				HCLOCALE(sscanf(density_info, "%f", &density));
			}
			else
				density = 0.0f;

			if (strstr(bounding_info, "sphere"))
			{
				float radius;
				HPoint center;

				HC_Show_Bounding_Sphere_By_Key(key, &center, &radius);

				HC_Open_Segment_By_Key(bounding_seg);{
					HC_Set_Modelling_Matrix(matrix);
					HC_Set_Visibility("off, faces=on");
					HC_Set_Color("transmission=grey");
					HC_Set_Selectability("off");
					HC_Set_Color_By_Value("faces", "HSV", MAX_COLOR_HUE*density, 0.5f, 0.5f);
					HC_Set_Rendering_Options("general displacement=-16");
					HC_Set_Heuristics("culling = (no hard extent, no maximum extent)");
					
					HC_Insert_Sphere(&center, radius, 0, 0);
				}HC_Close_Segment();
			}
			else if (strstr(bounding_info, "cuboid"))
			{
				HPoint min, max, bbox[8];
				int flist[] = {
					4,0,1,2,3,
					4,1,5,6,2,
					4,2,6,7,3,
					4,3,7,4,0,
					4,0,4,5,1,
					4,7,6,5,4,
				};
				
				HC_Show_Bounding_Cuboid_By_Key(key, &min, &max);

				bbox[0].x=min.x;	bbox[0].y=min.y;	bbox[0].z=min.z;
				bbox[1].x=min.x;	bbox[1].y=min.y;	bbox[1].z=max.z;
				bbox[2].x=max.x;	bbox[2].y=min.y;	bbox[2].z=max.z;
				bbox[3].x=max.x;	bbox[3].y=min.y;	bbox[3].z=min.z;
				bbox[4].x=min.x;	bbox[4].y=max.y;	bbox[4].z=min.z;
				bbox[5].x=min.x;	bbox[5].y=max.y;	bbox[5].z=max.z;
				bbox[6].x=max.x;	bbox[6].y=max.y;	bbox[6].z=max.z;
				bbox[7].x=max.x;	bbox[7].y=max.y;	bbox[7].z=min.z;

				HC_Open_Segment_By_Key(bounding_seg);{
					HC_Set_Modelling_Matrix(matrix);
					HC_Set_Edge_Weight(1.0);
					HC_Set_Visibility("off, faces=on, edges=on");
					HC_Set_Color("faces=transmission=grey, vertices=edges=transmission=black");
					HC_Set_Selectability("off");
					HC_Set_Color_By_Value("faces,edges,vertices", "HSV", MAX_COLOR_HUE*density, 0.5f, 0.5f);
					HC_Set_Rendering_Options("general displacement=-16");
					HC_Set_Heuristics("culling = (no hard extent, no maximum extent)");

					HC_Insert_Shell(8, bbox, sizeof(flist)/sizeof(flist[0]), flist);

					HC_Open_Segment("");{
						HC_Set_Visibility("off,vertices");
						HC_Set_Marker_Symbol("(*)");
						HC_Set_Variable_Marker_Size("3 pixels");
						HC_Insert_Shell(8, bbox, 30, flist);
						HC_Set_Rendering_Options("depth range=(0.0,0.05)");
					}HC_Close_Segment();

				}HC_Close_Segment();
			}
		}
	}

	Update();
}


inline static void clamp_float(float & number, float low, float high)
{
	if (number > high)
		number = high;
	else if (number < low)
		number = low;
}

HPoint HBaseView::GetFakeHLRColor()
{
	return m_FakeHLRColor;
}
	

void HBaseView::SetFakeHLRColor(HPoint color)
{
	clamp_float(color.x, 0.0f, 1.0f);
	clamp_float(color.y, 0.0f, 1.0f);
	clamp_float(color.z, 0.0f, 1.0f);
	m_FakeHLRColor = color;
	if (m_RenderMode == HRenderFakeHiddenLine)
		SetRenderMode(HRenderFakeHiddenLine);	//refresh if needed
}

void HBaseView::FocusOnSelection(bool onoff)
{
	HPoint min(0,0,0), max(0,0,0), center(0,0,0), diff;
	double distance, near, far, radius;
	HCamera camera;
	char dopt[4096];
	
	if (!onoff)
	{
		HC_Open_Segment_By_Key(GetViewKey());{
			HC_Set_Driver_Options("depth of field = off");
		}HC_Close_Segment();
		m_FocusedOnSelection = false;
		return;
	}
	
	if (GetSelection()->GetSize() == 0)
		return;

	GetSelection()->GetSelectionBounding(&min, &max);
	center.x = (min.x + max.x) / 2;
	center.y = (min.y + max.y) / 2;
	center.z = (min.z + max.z) / 2;
	
	GetCamera(&camera);
	diff = camera.position - center;
	distance = HC_Compute_Vector_Length(&diff);

	diff = max - center;
	radius = HC_Compute_Vector_Length(&diff);
	near = distance - radius;
	far = distance + radius;

	HCLOCALE(sprintf(dopt,
		"depth of field = (on, strength = 0.5, near = %lf, far = %lf)",
		near, far
	));

	HC_Open_Segment_By_Key(GetViewKey());{
		HC_Set_Driver_Options(dopt);
	}HC_Close_Segment();

	m_FocusedOnSelection = true;
	Update();
}

void HBaseView::SetFastMarkerDrawing(bool b_fast)
{
	HC_Open_Segment_By_Key(m_ViewKey);{
		if (b_fast)
			HC_Set_Driver_Options ("marker drawing = fastest");
		else 
			HC_Set_Driver_Options ("marker drawing = nicest");
	}HC_Close_Segment();
}

bool HBaseView::GetFastMarkerDrawing()
{
	char buffer[MVO_BUFFER_SIZE];

	HC_Open_Segment_By_Key(m_ViewKey);{
		HC_Show_One_Driver_Option("marker drawing", buffer);
	}HC_Close_Segment();

	return streq(buffer, "fastest");
}

void HBaseView::SetShadowLightDirection(bool explicit_light, HPoint const * direction)
{
	if (explicit_light && direction)
	{
		m_UseExplicitShadowLight = true;
		m_ExplicitShadowLightDirection = *direction;
	}
	else
		m_UseExplicitShadowLight = false;
	SetShadowMode(GetShadowMode());
}

bool HBaseView::GetShadowLightDirection(HPoint * direction)
{
	if (direction)
		*direction = m_ExplicitShadowLightDirection;
	return m_UseExplicitShadowLight;
}


void HBaseView::SetShadowIgnoresTransparency(bool ignore)
{
	HC_Open_Segment_By_Key(m_SceneKey);{
		if (ignore)
			HC_Set_Rendering_Options("simple shadow = ignore transparencies = on");
		else 
			HC_Set_Rendering_Options("simple shadow = ignore transparencies = off");
	}HC_Close_Segment();
}

bool HBaseView::GetShadowIgnoresTransparency()
{
	char buffer[128];

	HC_Open_Segment_By_Key(m_SceneKey);{
		HC_Show_One_Rendering_Option("simple shadow = ignore transparencies", buffer);
	}HC_Close_Segment();

	if (streq(buffer, "off"))
		return false;
	else 
		return true;
}

void HBaseView::SetLightCount(int count)
{
	m_LightCount = count > MaxLights ? MaxLights : count;
	SetupLights();
}


//utility function used internally to insert and configure distant lights
void HBaseView::SetupLights()
{
	HPoint light_loc[MaxLights];
	assert(m_LightCount <= MaxLights);

	switch (m_LightCount)
	{
		case 0:
			break;

		case 1:
		{
			light_loc[0].Set(1, 0, -1.5);
		} break;

		case 2:
		{
			light_loc[0].Set(1, 0, -1.5);
			light_loc[1].Set(-1, 0, 1.5);
		} break;

		case 3:
		{
			light_loc[0].Set(1, 0, -1.5);
			light_loc[1].Set(-1, 0, 0);
			light_loc[2].Set(1, 0, 1.5);
		} break;

		case 4:
		{
			light_loc[0].Set(1, 0, -1.5);
			light_loc[1].Set(-1, 0, 0);
			light_loc[2].Set(1, 0, 1.5);
			light_loc[3].Set(0, 1, 0);
		} break;

		case 5:
		{
			light_loc[0].Set(1, 0, -1.5);
			light_loc[1].Set(-1, 0, 1.5);
			light_loc[2].Set(1.5, 0, 1);
			light_loc[3].Set(-1.5, 0, -1);
			light_loc[4].Set(0, 1, 0);
		} break;

		case 6:
		{
			light_loc[0].Set(1, 0, -1.5);
			light_loc[1].Set(-1, 0, 1.5);
			light_loc[2].Set(1.5, 0, 1);
			light_loc[3].Set(-1.5, 0, -1);
			light_loc[4].Set(0, 1, 0);
			light_loc[5].Set(0, -1, 0);
		} break;

		default:
			assert(0);
	}

	// 是否允许跟随摄像机
	if (!m_bLightFollowsCamera)
	{
		HPoint position, target, up, vi, vj, vk;
		float width, height;
		char projection[MVO_BUFFER_SIZE];
		
		HC_Open_Segment_By_Key(GetSceneKey());{
			HC_PShow_Net_Camera(0, 0, &position, &target, &up, &width, &height, projection);
		}HC_Close_Segment();
		
		vk.Set(target.x-position.x, target.y-position.y, target.z-position.z);
		HC_Compute_Normalized_Vector(&vk, &vk);
		HC_Compute_Normalized_Vector(&up, &vj);
		HC_Compute_Cross_Product(&vj, &vk, &vi);

		for (int i = 0; i < m_LightCount; ++i)
			light_loc[i] = vi*light_loc[i].x + vj*light_loc[i].y + vk*light_loc[i].z;
	}

	HC_Open_Segment_By_Key(m_LightsKey);{
		HC_Flush_Contents(".", "lights");
		HC_Set_Visibility( "lights=on" );
		for (int i = 0; i < m_LightCount; ++i)
		{
			m_SceneLight[i] = HC_Insert_Distant_Light(light_loc[i].x, light_loc[i].y, light_loc[i].z);
			if (m_bLightFollowsCamera)
				HC_Set_Geometry_Options(m_SceneLight[i], "camera relative");
			else
				HC_Set_Geometry_Options(m_SceneLight[i], "no camera relative");

		}
	}HC_Close_Segment();

	for (int i = m_LightCount; i < MaxLights; ++i)
		m_SceneLight[i] = INVALID_KEY;
}


