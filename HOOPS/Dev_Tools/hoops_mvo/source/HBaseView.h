// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

/*! \file HBaseView.h
	Defines datatypes associated with the HBaseView class  
*/
#ifndef _HBASEVIEW_H
#define _HBASEVIEW_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"
#include "HTManager.h"
#include "HEventListener.h"

class HDB;
class HBaseModel;
class HCamera;
class HEventInfo;
class HSelectionSet;
class HBaseOperator;
class HConstantFrameRate;
class HSharedKey;
class HImCuttingGeometry;
class HOutputHandler;
class HOutputHandlerOptions;
class HInputHandler;
class HInputHandlerOptions;
class HUtilityAntialiasing;
class HMarkupManager;
class HShadow;
class HUndoManager;
class HEventManager;
class HObjectManager;
class HOpCameraManipulate;
class HDebugZBuffer;
class HConstFRSimpType;


/*! Functional prototype for the signalling notify system in HBaseView. 
	\param signal identifier of the signal
	\param signal_data the data associated with this specific signal
	\param user_data any user data that may be associated with this specific signal
*/
typedef bool(*HSignalNotifyFunction)(int signal, void *signal_data, void *user_data);



/*! \enum HRenderMode
	Indicates the rendering mode
*/
enum HRenderMode 
{
	HRenderWireframe = 1,       //!< edges/lines only
	HRenderTriangulation = 2,   //!< edges and non-lit faces
	HRenderSilhouette = 3,      //!< shell/mesh silhouette edges
	HRenderHiddenLine = 4,      //!< hidden line removal
	HRenderHiddenLineHOOPS = 5, //!< currently does nothing
	HRenderHiddenLineFast = 6,  //!< simulate hidden line (regular hardware z-buffer, with faces set to window background color)
	HRenderFlat = 7,            //!< flat shading
	HRenderGouraud = 8,         //!< gouraud (smooth) shading
	HRenderGouraudWithEdges = 9,//!< gouraud (smooth) shading with edges turned on
	HRenderPhong = 10,          //!< phong shading
	HRenderShaded = 11,         //!< indicates shaded rendering; underlying code will use phong if hardware accel is avialable, otherwise gouraud will be used
	HRenderShadedWithLines = 12,//!< same as HReanderShaded but with lines visible
	HRenderWireframeWithSilhouette = 13, //!< wireframe + silhouette
	HRenderShadedWireframe = 14,//!< wireframe, with shaded lines
	HRenderVertices = 15,		//!< vertices only
	HRenderShadedVertices = 16,	//!< vertices only (shaded)
	HRenderGouraudWithLines = 17,//!< same as HReanderShaded but with lines visible
	HRenderLOD1 = 18,//!< clamps to LOD1
	HRenderLOD2 = 19,//!< clamps to LOD2
	HRenderBRepHiddenLine = 20, //!<hidden line-ish removal
	HRenderBRepHiddenLineFast = 21, //!<hidden line-ish removal
	HRenderBRepWireframe = 22,  //!<brep wireframe
	HRenderGoochShaded	= 23,	//!<Gooch Shaded
	HRenderFakeHiddenLine = 24, //!<fake hidden line
	HRenderUnknown = 0
};

/*! \enum ViewMode 
	Indicates the camera target plane (or iso view) 
*/
enum ViewMode 
{
	HViewNone,     //!< none view mode
	HViewXY,  //!< self-explanatory
	HViewYX,  //!< self-explanatory
	HViewXZ,  //!< self-explanatory
	HViewZX,  //!< self-explanatory
	HViewYZ,  //!< self-explanatory
	HViewZY,  //!< self-explanatory
	HViewIso,  //!< self-explanatory
	HViewIsoFrontRightTop,  //!< positive X, positive Y, positive Z (same as HViewISO)
	HViewIsoBackRightTop,  //!< negative X, positive Y, positive Z 
	HViewIsoBackLeftTop,  //!< negative X, positive Y, positive Z 
	HViewIsoBackLeftBottom,  //!< negative X, positive Y, positive Z 
	HViewIsoBackRightBottom,  //!< negative X, positive Y, positive Z 
	HViewIsoFrontLeftTop,  //!< negative X, positive Y, positive Z 
	HViewIsoFrontLeftBottom,  //!< positive X, positive Y, positive Z (same as HViewISO)
	HViewIsoFrontRightBottom,  //!< positive X, positive Y, positive Z (same as HViewISO)

	HViewSEIso, //!< same as HViewIsoXYZ and HViewISO
	HViewSWIso, //!< same as HViewIsoxYZ
	HViewNEIso, //!< same as HViewIsoXYz
	HViewNWIso, //!< same as HViewIsoxYz

	HViewFront,
	HViewBack,
	HViewRight,
	HViewLeft,
	HViewTop,
	HViewBottom,

	HViewUnknown  //!< unknown view mode
};


/*!  \enum LightingIntMode 
	Indicates the lighting interpolation mode
*/
enum LightingIntMode 
{
	LightingIntUnknown,  //!< unknown mode
	LightingIntGouraud,  //!< self-explanatory
	LightingIntPhong     //!< self-explanatory
};


/*!  \enum FramerateMode 
	Indicates the framerate mode
*/
/*!
  \addtogroup Framerate_Mode
  @{
*/

enum FramerateMode 
{
	FramerateOff,  //!< unknown mode
	FramerateFixed,  //!< self-explanatory
	FramerateTarget     //!< self-explanatory
};
/*!  @}  */


/*!  \enum ProjMode 
	Indicates the camera projection mode; refer to HC_Set_Camera for details
*/
enum ProjMode 
{
	ProjUnknown,       //!< unknown mode
	ProjPerspective,   //!< Objects which are close to the eye appear large, and objects which are far away appear small.
	ProjOrthographic,  //!< The represented size of an object doesn't depend on how close or far it is---this can be handy in architecture and engineering.
	ProjStretched      //!< In a "stretched" view, an orthographic projection is made, and then the view is "stretched" to fit exactly into the available screen window. 
};

 

/*! \enum HandednessMode 
	Indicates the handedness mode.  Used for polygon handedness and world handedness 
*/  
enum HandednessMode 
{
	HandednessLeft,   //!< self-explanatory
	HandednessRight,  //!< self-explanatory
	HandednessNone    //!< no handedness; only applies to polygon-handedness
};


/*!	\enum DisplayListType
	Indicates the display list type.
*/
enum DisplayListType
{
	DisplayListOff,			//!< No display lists
	DisplayListGeometry,	//!< Geometry-level display lists
	DisplayListSegment		//!< Segment-level display lists
};

/*!	\enum MultiThreadingType
	Indicates the multi-threading type.
*/
enum MultiThreadingType
{
	MultiThreadingOff,
	MultiThreadingBasic,
	MultiThreadingFull
};

/*!	\enum HLRMode
	Indicates the default hidden line mode
*/
enum HLRMode
{
	AnalyticHiddenLine,		//!< Analytic hidden line
	FastHiddenLine,			//!< Fast hidden line
	FakeHiddenLine			//!< Fake hidden line
};


/*! \enum AxisMode */
enum AxisMode 
{
	AxisOff,    //!< self-explanatory
	AxisSimple, //!< Like AxisOn, but without the planes
	AxisCenter, //!< Like AxisOn, but centered differently
	AxisOn      //!< self-explanatory 
};

/*! \enum UpDirection */
enum UpDirection
{
	UpDirectionX = 0xFADE,
	UpDirectionY,
	UpDirectionZ
};


/*! \enum ViewUpdateMode 
	Indicates how the scene gets updated.
*/
enum ViewUpdateMode
{
	OnDemand,    //!< This tells the MVO framework not to implicitly perform updates. They will be explicity made by calls to Update().
	Continuous,   //!< This tells the MVO framework to implicitly perform continuous updates by polling for posted update events at a regular interval. 
	Deferred	//!< This tells the MVO framework to implicitly perform continuous culled, timed updates drawing culled items as time permits. 
};


/*! \enum HShadowRenderingMode */
enum HShadowRenderingMode 
{
	SoftwareShadow,             //!< image driver shadow generation
	HardwareShadow,		    //!< opengl p-buffer shadow generation
	SoftwareOpenglShadow        //!< opengl software shadow generation
};

/*! \enum HBloomShape */
enum HBloomShape
{
	RadialBloom,			//!< radial bloom
	StarBloom				//!< star bloom
};

/*! \enum HQuickMovesMethod */
enum HQuickMovesMethod
{
	HQuickMoves_None,		//!< the driver is unable to quick move at all
	HQuickMoves_XOR,		//!< quick-moved geometry was last drawn as XOR (exclusive OR), meaning that colors are often inverted or completely wrong
	HQuickMoves_Overlay,    //!< quick-moved geometry was last drawn as overlay, meaning that colors are correct, but that the quick-moved item is always on top
	HQuickMoves_Spriting	//!< quick-moved geometry was last drawn as spriting, color-correct and depth-mixed with the rest of the scene
};


/*! \enum HViewSelectionLevel */
/*!
  \addtogroup Selection_Level
  @{
*/

enum HViewSelectionLevel
{
	HSelectionLevelEntity,		//!< The driver returns geometry objects from selections
	HSelectionLevelSegment,		//!< The driver returns segment objects from selections
	HSelectionLevelSegmentTree	//!< The driver returns segment trees from selections
};
/*!  @}  */

/*!	\enum SplatSymbol
	Indicates the splat symbol type.
*/
enum SplatSymbol
{
	SplatRound,			//!< Round
	SplatSquare			//!< Square
};



/*! This has been deprecated. Functional prototype for the function which emits an MVO message.  This is typically used in 
	conjunciton with a HNet session.
	\param message this is the character string which uniquely identifies the message 
	\param length this is the byte length of the user_data
	\param user_data the user_data which contains the message
*/
typedef void(*HBaseViewEmitMessageFunction)(const char* message, unsigned int length, void * user_data);


//! The HBaseView class defines and manages a view of model information
/*!
 The HBaseView class stores view-specific information and provides a high level interface for managing viewing
 attributes.   A view typically references an HBaseModel object to define a view of a model.
 
 This class encapsulates a HOOPS segment associated with an instance of a HOOPS driver.  This type of segment defines a
 connection to a device (i.e. OpenGL, X11, GDI, etc.).  Its Init method creates a default segment structure under the driver
 instance segment, which is used to manage the inclusion of the HOOPS Include Library segment associated with HBaseModel,
 lights for the scene, and segments containing camera and window attributes used for temporary 'rubberband' graphics.

 The HBaseView object has associations with the following objects: 

	- HBaseModel
	- HEventInfo
	- HSelectionSet
	- HBaseOperator

 It also implements the following viewing manipulations by setting the appropriate HOOPS camera attributes: 

	- Fit the view's camera to the model's extents.
	- Fit the view's camera to the extents of the current selection set.
	- Fit the view's camera to the extents of the entire world.
	- Set the view to default angles (XY, YZ, XZ target planes).

 It includes interfaces for managing: 

	- Rendering methods (antialiasing, shadows, capping lines/faces, etc...)
	- Hidden Surface Removal Algorithms
	- Lighting Interpolation
	- Camera Projection
	- Constant Framerate Logic

*/ 
class MVO_API HBaseView : public HUpdateListener , public HMouseListener, public HObjectManipulationListener
{

public:

	
	/*!
	Constructs an HBaseView object.

	\param model A pointer to the HBaseModel object, which defines the model to viewed by this HBaseView object.  
	If model is null, then the view will not reference any external part of the HOOPS segment tree, and can only view
	geometry that is created directly underneath the HBaseView object's 'scene' segment

	\param alias A character pointer denoting the HOOPS alias representing the driver instance segment encapsulated
	by this HBaseView object

	\param driver_type A character pointer denoting the type of HOOPS driver instance that should be created to correspond
	with this HBaseView object. See the <a href="../../Hoops3DGS/PlatformDeviceGuide_C.html">Device Guide</a> for a list of available drivers.

	\param instance_name A character pointer denoting the name of the HOOPS driver instance associated with this HBaseView
	object.  For example, this might be 'window' if driver_type is 'msw', or 'unix:0.0' if driver_type is 'x11'
	If either driver_type or instance_name are null, HOOPS will use default values obtained from the unique hDB HOOPS MVO object.

	\param window_handle This identifies the handle of the precreated native window system window (X11 or GDI) which the HOOPS 
	driver instance segment should draw into.  HOOPS will not handle any events for this window if it is passed in.  If 
	window_handle is null, HOOPS will automatically create a native window, and manage its updating as well as all events.

	\param colormap This identifies a native window system colormap which HOOPS will utilize when allocating and managing colors.
	If null, HOOPS will creates its own system colormap as necessary.

	\param clip_override This identifies a pointer to a clip override array. 

	\param window_handle_2 This identifies a window handle to be used for use window id2. 
	\param driver path If provided this driver path will be used rather than an internally generated one.
	*/
	HBaseView(HBaseModel *model,
		const char * alias = 0,	
		const char * driver_type = 0,
		const char * instance_name = 0,
		void * window_handle = 0,
		void * colormap = 0,
		void * clip_override = 0,
		void * window_handle_2 = 0,
		const char * driver_path = 0);

	/*!
	This method rebinds the view to a new window.

	\param alias A character pointer denoting the HOOPS alias representing the driver instance segment encapsulated
	by this HBaseView object

	\param driver_type A character pointer denoting the type of HOOPS driver instance that should be created to correspond
	with this HBaseView object

	\param instance_name A character pointer denoting the name of the HOOPS driver instance associated with this HBaseView
	object.  For example, this might be 'window' if driver_type is 'msw', or 'unix:0.0' if driver_type is 'x11'
	If either driver_type or instance_name are null, HOOPS will use default values obtained from the unique hDB HOOPS MVO object.

	\param window_handle This identifies the handle of the precreated native window system window (X11 or GDI) which the HOOPS 
	driver instance segment should draw into.  HOOPS will not handle any events for this window if it is passed in.  If 
	window_handle is null, HOOPS will automatically create a native window, and manage its updating as well as all events.

	\param colormap This identifies a native window system colormap which HOOPS will utilize when allocating and managing colors.
	If null, HOOPS will creates its own system colormap as necessary.

	\param clip_override This identifies a pointer to a clip override array.

	\param window_handle_2 This identifies a window handle to be used for use window id2.

	\param debug_flags The optional debug flags passed to #HC_Set_Driver_Options().

	*/
	void RebindView(
		const char * alias = 0,	
		const char * driver_type = 0,
		const char * instance_name = 0,
		void * window_handle = 0,
		void * colormap = 0,
		void * clip_override = 0,
		void * window_handle_2 = 0, 
		int debug_flags = 0
		);

	/*! 
		Deletes the HOOPS segment tree and operator/event/selection/framerate-support objects associated 
		with this view
	*/
	virtual ~HBaseView();

	/*!
		This method initializes the view.  The startup default segment hierarchy associated with the HBaseView 
		object is created and initialized.  
	*/
	virtual void Init();

	/*!
	  This resets the camera so that it views the extents of the scene.  It does not call Update().
	  \param center_out  A pointer to a 3d point.  If a non-null pointer is passed, then the new camera 
	  target position is returned.
	*/
	virtual void FitWorld(HPoint * center_out = 0);

	/*!
	  This method moves the camera along the exising view vector so that it wholly encloses all geometry, 
	  including text, in the scene.  The camera is set so that it tightly encloses the geometry and so 
	  the geometry in the scene will extend to the bounds of the window in either the x or y direction. 
	  It uses #HC_Compute_Screen_Extent to get the tight 2d box around the model. It behaves similarly to FitWorld.
	  \param center_out Pass a pointer to an #HPoint if you want this method to return the center of the scene viewed by the camera.
	*/
	virtual void FitWorldExact(HPoint * center_out = 0);
	
	/*!
	  This method uses #FitWorld to reset the camera and then calls #Update in various ways depending on how the smooth transition 
	  parameters are set.
	*/
	virtual void ZoomToExtents();

	/*!
	  This functions zooms to the original camera settings which have been specified when the model was initially loaded.
	*/
	virtual void ZoomToInitialCamera();


	/*!
	This method fits the camera to the extents of the HOOPS segment tree corresponding with the specified segment.
	\param seg_key  The key of the HOOPS segment denoting the top of the desired segment tree.
	*/
	void FitSegment(HC_KEY seg_key);

	/*!
		This method fits the camera to view the extents of the geometric objects in the views HSelectionSet list;
		function is still under development.
	*/
	void FitSelection(HC_KEY key = INVALID_KEY, bool adjustToShellNormal = false);

	/*!
		This method fits the camera to view the extents of the key passed in.
	*/
	void FitItemByPath(HC_KEY key, int incl_count, HC_KEY * incl_path); 
	
	/*!This method redraws the scene with antialiasing enabled. */
	void RenderAntialiasing(); 

	/*!This method initializes the antialiasing logic. */
	void InitAntialiasing();   
	
	/*! \return A pointer to an HUtilityAntialiasing object. */
	HUtilityAntialiasing * GetAntialiasing() { return m_pAntialiasing; }

	/*! \return A pointer to the HMarkupManager object. */
	HMarkupManager * GetMarkupManager();

	/*! \return A true if we a HMarkupManager object. */
	bool HaveMarkupManager() {return (m_pMarkupManager != 0);}

	/*! This method sets the HMarkupManager object for the view.
		\param  markup A pointer to an HMarkupManager object.
	*/
	void	SetMarkupManager( HMarkupManager* markup ) { m_pMarkupManager = markup; }

	/*! This method sets the render mode to wireframe so that only lines and edges appear in the scene. */
	void RenderWireframe(); 

	/*! This method sets the render mode to silhouette which renders edges with two attached non-invisible faces, 
	one of which is facing forward and one back on the screen to be. */
	void RenderSilhouette();    

	/*! This method sets the render mode to \ref Set_Rendering_Options_hidden_surface_removal_algorithm "hidden line". */
	void RenderHiddenLine(); 

	/*! This method sets the render mode to \ref Set_Rendering_Options_hidden_surface_removal_algorithm "fast hidden line". */
	void RenderHiddenLineFast();  

	/*! This method sets the render mode to flat shading. */
	void RenderFlat();  

	/*! This method sets the render mode to Gooch shading. */
	void RenderGooch();

	/*! This method sets the render mode to \ref Set_Rendering_Options_lighting_interpolation "gouraud shading". */
	void RenderGouraud();    

	/*! This method sets the render mode to \ref Set_Rendering_Options_lighting_interpolation "phong shading". */
	void RenderPhong();     

	/*! This method sets the render mode to wireframe with silhouette. */
	void RenderWireframeWithSilhouette();

	/*! This method sets the render mode to shaded wireframe. */
	void RenderShadedWireframe(); 

	/*! This method sets the render mode to vertices. */
	void RenderVertices();		  

	/*! This method sets the render mode to shaded vertices. */
	void RenderShadedVertices();  

	/*! This method sets the render mode to fake hidden line.  Fake hidden line mode sets the top window
	color and the face color to be the same as the bottom window color.  If shaders are available,
	fake hidden line will use fast silhouette edges to achieve excellent performance.*/
	void RenderFakeHiddenLine();

	/*! This method sets the render mode to \ref Set_Rendering_Options_hidden_surface_removal_algorithm "hidden line" 
	for BRep geometry. */
	void RenderBRepHiddenLine();  

	/*! This method sets the render mode to \ref Set_Rendering_Options_hidden_surface_removal_algorithm "fast hidden line" 
	for BRep geometry. */
	void RenderBRepHiddenLineFast();  
	
	/*! This method sets the render mode to brep wireframe. */
	void RenderBRepWireframe();   
 
	/*! 
		A higher level function to enable smooth shading.  If phong lighting is hardware accelerated, 
		then phong shading will be enabled.  Otherwise gouraud shading will be enabled. 
	*/
	void RenderShaded();

	/*! Sets render mode with smooth shading and lines on */
	void RenderShadedWithLines();          

	/*! Sets render mode to Gouraud with lines on */
	void RenderGouraudWithLines();          

	/*! Clamps rendering to LOD1. */
	void RenderLOD1();          
	/*! Clamps rendering to LOD2. */
	void RenderLOD2();          

	
	/*! This method removes the modelling transformations from the scene segment. */
	void RemoveTransforms(); 

	/*!
		This method updates the scene.  The HOOPS segment tree is traversed and any changes in geometry or attributes are updated via
		a redraw.  Cutting plane geometry and the orientation of the axis display are recalculated if necessary.  If constant
		frame rate is enabled and activated by an operator, the detail level of the geometry is adjusted if necessary in order
		to maintain the target frame rate.
	*/
	virtual void Update();

	/*!
		If the view update mode is set to #ViewUpdateMode.Continuous, you can use this method to request an immediate update. 
		If it is set to #ViewUpdateMode.OnDemand, this method will simply call #Update.  To find the current setting for the view 
		update mode, you can call look at call #GetViewUpdateMode.
	*/
	virtual void ForceUpdate();

	/*!
	  This method uses a HInputHandler to input image or model data into the HBaseView current HBaseModel.
	  If no HInputHandlerOptions is provided a default, one with the correct HBaseView pointer filled in
	  is created and used for the operation.  The key provided by GetModel()->GetModelKey() is used for
	  the input target segment. 

	  \param FileName A null terminated string of the complete file path for the operation.
	  \param handler A pointer to an instance of a subclassed HInputHandler implementing the input operation need.
	  \param options An optional pointer to an instance of a HInputHandlerOptions contain extra data for input operation.
	  \return An #HFileInputResult value depending on the outcome of the input operation.
	*/
	virtual HFileInputResult FileInput(const __wchar_t * FileName, HInputHandler * handler, HInputHandlerOptions * options = 0);
	
	/*!
	  This method uses a HInputHandler to input image or model data into the HBaseView current HBaseModel.
	  If no HInputHandlerOptions is provided a default, one with the correct HBaseView pointer filled in
	  is created and used for the operation.  The key provided by GetModel()->GetModelKey() is used for
	  the input target segment. 

	  \param FileName A null terminated string of the complete file path for the operation.
	  \param handler A pointer to an instance of a subclassed HInputHandler implementing the input operation need.
	  \param options An optional pointer to an instance of a HInputHandlerOptions contain extra data for input operation.
	  \return An #HFileInputResult value depending on the outcome of the input operation.
	*/
	virtual HFileInputResult FileInput(const char * FileName, HInputHandler * handler, HInputHandlerOptions * options = 0){
		return FileInput((__wchar_t*)H_WCS(FileName).encodedText(), handler, options);
	}

	/*!
	  This method uses a HInputHandler to input image or model data into the HBaseView current HBaseModel.
	  If no HInputHandlerOptions is provided a default, one with the correct HBaseView pointer filled in
	  is created and used for the operation.  The key provided by GetModel()->GetModelKey() is used for
	  the input target segment. 

	  \param FileName A null terminated string of the complete UNICODE file path for the operation.
	  \param handler A pointer to an instance of a subclassed HInputHandler implementing the input operation need.
	  \param options An optional pointer to an instance of a HInputHandlerOptions contain extra data for input operation.
	  \return A #HFileInputResult value depending on the outcome of the input operation.
	*/
#ifdef H_USHORT_OVERLOAD
	virtual HFileInputResult FileInput(const unsigned short * FileName, HInputHandler * handler, HInputHandlerOptions * options = 0){
		H_UTF16 utf16;
		utf16.encodedText((utf16_char*)FileName);
		return FileInput((__wchar_t*)H_WCS(utf16).encodedText(), handler, options);
	}
#endif

	/*!
	
	  This method uses a HOutputHandler to create image, model, hardcopy and html output. 
	  If no HOutputHandlerOptions is provided a default, one with the correct HBaseView pointer filled in
	  is created and used for the operation.  In many cases the default options will not be enough to get 
	  the desired output.  You should read the documentation for the specific HOutputHandler used and provide
	  the options it needs. HOutputHandler implementers should setup up sane defaults for a minimal options
	  setup.  In some cases OutputBadOptions will be returned if a required option was not filled in.

	  Depending on the style of the HOutputHandler a HOOPS driver may be created or a HOOPS image rendered
	  and its key passed to the handle.  In the case of model output the HBaseView, HCamera and model key
	  will be provided by default.  If output style is image, then a image will be created by the view
	  and passed to the handle.  Height and Width are required for image style output.
	  In the case of html, a page embedding an hsf file will be created.  An hsf file will also be
	  created using the current handler for "hsf" registered with the HIOManager.   

	  Note that geometry in the view segment but not in the model segment, though it may be drawn to the screen, 
	  will not be written to the file.

	  \param FileName A null terminated string of the complete file path for the operation.
	  \param handler A pointer to an instance of a subclassed HOutputHandler implementing the output operation need.
	  \param options An optional pointer to an instance of a HOutputHandlerOptions contain extra data for output operation.
	  \return A HFileOutputResult value depending on the outcome of the output operation

	*/
	virtual HFileOutputResult FileOutput(const __wchar_t * FileName, HOutputHandler * handler, HOutputHandlerOptions * options = 0);
	
	/*!
	
	  This method uses a HOutputHandler to create image, model, hardcopy and html output. 
	  If no HOutputHandlerOptions is provided a default, one with the correct HBaseView pointer filled in
	  is created and used for the operation.  In many cases the default options will not be enough to get 
	  the desired output.  You should read the documentation for the specific HOutputHandler used and provide
	  the options it needs. HOutputHandler implementers should setup up sane defaults for a minimal options
	  setup.  In some cases OutputBadOptions will be returned if a required option was not filled in.

	  Depending on the style of the HOutputHandler a HOOPS driver may be created or a HOOPS image rendered
	  and its key passed to the handle.  In the case of model output the HBaseView, HCamera and model key
	  will be provided by default.  If output style is image, then a image will be created by the view
	  and passed to the handle.  Height and Width are required for image style output.
	  In the case of html, a page embedding an hsf file will be created.  An hsf file will also be
	  created using the current handler for "hsf" registered with the HIOManager.   

	  Note that geometry in the view segment but not in the model segment, though it may be drawn to the screen, 
	  will not be written to the file.

	  \param FileName A null terminated string of the complete file path for the operation.
	  \param handler A pointer to an instance of a subclassed HOutputHandler implementing the output operation need.
	  \param options An optional pointer to an instance of a HOutputHandlerOptions contain extra data for output operation.
	  \return A HFileOutputResult value depending on the outcome of the output operation

	*/
	virtual HFileOutputResult FileOutput(const char * FileName, HOutputHandler * handler, HOutputHandlerOptions * options = 0){
		return FileOutput((__wchar_t*)H_WCS(FileName).encodedText(), handler, options);
	}

	// accessors

	/*!  \return  A string denoting the fully rooted pathname of the driver instance segment. */
	const char*		GetDriverPath() { return m_sDriverPath; }

	/*!  \return  A string denoting the driver type; e.g. msw, opengl, x11. */
	const char*		GetDriverType() { return m_sDriverType; }

	/*!	This method updates the highlighting of selected objects in all HSelectionSets owned by this view. */
	virtual void UpdateHighlighting();

	/*! \return A pointer to the HSelectionSet object for this view. */
	HSelectionSet*	GetSelection() { return m_pSelection; }
	
	/*! This method sets the selection object for this view. 
	\param selection A pointer the HSelectionSet object you want associated with this view.
	*/
	void			SetSelection(HSelectionSet* selection) { m_pSelection = selection; }

	/*! \return A pointer to the HBaseModel object referenced by this view. */
	virtual HBaseModel * GetModel() { return m_pModel; }

	/*! A helper function that allows developers to access the root of the model segment tree. 
		\return The key to the root of the segment hierarchy in the include library.  */
	HC_KEY			GetModelKey();

	/*! \return An enumerated type denoting the render mode for this view.  The default is HRenderGouraud. */
	HRenderMode		GetRenderMode () { return m_RenderMode; }

	/*! \return  An enumerated type denoting the 2d plane which this view's camera is looking at.  The default is HViewXY. */
	ViewMode		GetViewMode () { return m_ViewMode; }

	/*!
	  This method sets the view mode for this view.  The default is HViewXY.
	  \param mode Pass the new view mode.
	  \param invert Pass true if you want to set the inverted view of the passed mode.
	  \param fitworld Pass true if you want to reset the camera so that it views the extents of the scene.
	*/
	virtual void	SetViewMode (ViewMode mode, bool invert = false, bool fitWorld = true);

	/*! \return  A boolean denoting the visibility of cutting-plane intersection lines. */
	bool			GetCPLineVisibility() {return m_bCPLineVisibility; }

	/*! \return  A boolean denoting the visibility of cutting-plane intersection faces. */
	bool			GetCPFaceVisibility() {return m_bCPFaceVisibility; }

	/*! This method enables the visibility of cutting-plane intersection geometry.  If cutting 
	planes are present, the lines and faces of intersection between the cutting planes and 
	geometry can be computed and drawn.  Faces of intersection will only be present when closed 
	objects are cut by the cutting planes.  Both intersection lines and faces will be drawn by default.
	  \param lines  Pass true if the intersection lines should be drawn or false if they shouldn't be drawn.
	  \param faces  Pass true if the intersection faces should be drawn or false if they shouldn't be drawn. 
	  */
	void			SetCPGeomVisibility(bool lines, bool faces=false);


	/*! This method determines if the cutting planes exist in the given segment. 
	\param plane The name of the segment in which you wish to know if cutting planes exist.  If plane is 0, the entire 
	scene will be checked for cutting planes.
	\return  A boolean denoting whether cutting planes exist in the scene.
	*/
	bool			GetCuttingPlanesExist(const char *plane = 0);
 
	/*! Use this method to tell the system whether cutting planes exist.
	\param state Pass true if cutting planes exist or false if they don't.
	*/
	void			SetCuttingPlanesExist(bool state);

	/*! \return  A boolean denoting the visibility of the cutting plane. */
	bool			GetCuttingPlanesVisibility() {return m_bCuttingPlanesVisibility; }

	/*! This method enables visibility of cutting-plane faces.
		\param visible  Pass true to indicate that cutting planes are visible.
	*/
	void			SetCuttingPlanesVisibility(bool visible);

	/*! \return A pointer to the HConstantFrameRate object for this view. */
	HConstantFrameRate*	GetConstantFrameRateObject() { return m_pConstantFrameRate; }

	/*! This method sets the HConstantFrameRate object for this view. 
	\param cf A pointer to the HConstantFrameRate object.
	*/
	void			SetConstantFrameRateObject(HConstantFrameRate* cf) { m_pConstantFrameRate = cf; }

	/*! This method sets the event checker callback. */
	void			SetEventCheckerCallback(void (* EventChecker)(HIC_Rendition const *nr));
 
	/*! Sets the key state callback for low level key access.*/
	void SetKeyStateCallback(bool (* KeyState)(unsigned int, int &)) { m_pfKeyState = KeyState; }	

	/*! \return A pointer to a keyboard query callback function. */
	bool  (*GetKeyStateCallback())(unsigned int, int &) { return m_pfKeyState; }

	/*! \return A pointer to an event checker callback function.*/
	void  (*GetEventCheckerCallback())(HIC_Rendition const *nr) { return m_pfEventChecker; }


	/*! This method sets the occlusion culling mode.  If true, then HOOPS will do a test to 
	determine if an object in the scene will modify any pixels in the scene i.e. is this object fully 
	occluded by other objects in the scene be before deciding to draw it.   
		\param mode If true, occlusion culling will be used.  If false, it will not.
		\param UseOctree If true, HOOPS will use an Octree in calculating occlusion logic.  
		If false, it will not.  This parameter is only applied if mode is True.
	*/
	void			SetOcclusionCullingMode(bool mode,  bool UseOctree = true, int threshold = 50);

 
	/*! 
		\return An enumerated type indicating the current handedness of the world coordinate system.*/
	HandednessMode  GetHandedness();

	/*! This method sets the handedness of the world coordinate system.  Note that this is very different from polygon handedness.
		A "left-handed" Cartesian coordinate system is one in which, if you curl the fingers of your left hand 
		from the X axis towards the Y axis, your thumb will point in the direction of the Z axis. This means 
		your viewing camera is set up so that your X axis points to the right on the screen and the Y axis 
		points straight up, then the Z axis will go into the screen.
		A "right-handed" coordinate system is just the same, except that Z axis will be in the direction of 
		your right hand's thumb. If the camera is set up with X and Y as above, the Z axis will point out of 
		the screen.
		\param mode An enumerated type indicating the world handedness.
		\param emit_message Indicates that a message should be sent to the emit_message_function, which (if implemented)
								would typically use the HOOPS/Net 'Send' function to dispatch the message to clients */
	virtual void SetHandedness(HandednessMode mode, bool emit_message=false);

	/*! This method sets the handedness of the world coordinate system based on the information in the message.
		\param in_data A character pointer containing the handedness information.
		\param data_length The size of the message.
		*/
	virtual void SetHandednessFromMessage(const char *in_data, unsigned int data_length);

	/*!
	   This method sets the font size on the scene key.
	  \param font_size A character pointer containing the font size information (e.g. "10 oru" or "0.1 sru").
	  \param emit_message Pass true to indicates that a message should be sent to the emit_message_function, which if implemented
							would typically use the HOOPS/Net 'Send' function to dispatch the message to clients.
	*/
	virtual void SetFontSize(const char *font_size, bool emit_message=false);

	/*! This method sets the font size on the scene key based on the information in the message.
		\param in_data A character pointer containing the font size information (e.g. "10 oru" or "0.1 sru").
		\param data_length The size of the message.
	*/
	virtual void SetFontSizeFromMessage(const char *in_data, unsigned int data_length);
	
	/*! \return  True if back face culling is currently being done or false if it is not. */
	bool			GetBackplaneCulling(); 
	
	/*! This method enables or disables \ref Set_Heuristics_backplane_cull "back face culling" which is a 3-D update 
	optimization that tells the system it's OK to  remove any polygons, circles, or ellipses, or shell or mesh faces 
	that land "facing away" from the camera during drawing.  The initial value is true.  This option has no effect 
	unless polygon handedness is set using the method #SetPolygonHandednessMode(). 
	\param cull If the value is true, backplane will be enabled.  If the value is false, it will be disabled.
	*/
	void			SetBackplaneCulling(bool cull); 

	/*!
	This method gets the projection mode used to convert a 3-D scene into 2-D for viewing.
	\return The projection mode which determines how a 3-D will be converted in 2-D for viewing.*/
	ProjMode		GetProjMode(); 
	
	/*! This method sets the projection mode for how the camera converts a 3-D scene into 2-D for viewing.
	\param mode The mode in which you want the camera to project.  
	*/
	void			SetProjMode(ProjMode mode); 

	/*! \return The current display list type. */
	DisplayListType	GetDisplayListType(); 

	/*!This method sets the display list type.  Note that this only applies when the HOOPS/3dGS OpenGL or DX9 driver is being used. 
	\param type Pass the type of display list you would like to use.
	*/
	void			SetDisplayListType(DisplayListType type); 

	/*!< \return True if the currently active display list type is on and false if it is off.  This only applies when HOOPS/3dGS Open GL driver is being used.*/
	bool			GetDisplayListMode(); 

	/*! This method enables display lists of the currently active display list type (see m_DisplayListType). 
	\param mode Pass true to activate the display list type or false to turn it off.
	*/
	void			SetDisplayListMode(bool mode); 

	/*!< \return The polygon handedness which must be set to do backplane culling. */
	HandednessMode	GetPolygonHandednessMode(); 

	/*! This method sets the polygon handedness.  In order to do a backplane cull, there must be some 
	definition of a polygon's "front" and "back" (or equivalently "inside face" and "outside 
	face"). This orientation is determined by the order in which the polygon's vertices were 
	originally defined, and the polygon handedness setting (and not the coordinate system 
	handedness). If you wrap the fingers of the chosen hand along the vertices of the polygon, 
	with your wrist at the first vertex specified and your fingertips at the last, then extend 
	your thumb perpendicular to your fingers, by definition, your thumb is extending out of the 
	front face of the polygon.  The hand you use, left or right, determines the front and back 
	of the polygon.
	\param HandednessMode This mode will help HOOPS determine which the inside and outside of a given face.
	*/
	void			SetPolygonHandednessMode(HandednessMode); 
	
	/*! \return True if the \ref Set_Rendering_Options_level_of_detail "Level-Of-Detail" mode is enabled 
	or false if it is disabled.*/
	bool			GetLodMode(); 

	/*! This method enables or disables the \ref Set_Rendering_Options_level_of_detail "Level-Of-Detail".  LOD is an optimization that involves simplifying objects that have facetted 
		representations so that they will be drawn using a smaller number of triangles. As a result, LOD 
		can improve performance by an order of magnitude while maintaining the visual integrity of 
		the model.    
		\param onoff If true, LOD is enabled.  If false, it is disabled.
	*/
	void			SetLodMode(bool onoff);
	
	/*! \return True if spriting is enabled and false if otherwise. */
	bool			GetSpritingMode() { return m_bSpritingAllowed; } 

	/*! This method allows you to enables or disables \ref Set_Heuristics_quick_moves "spriting".  If the spriting mode is turned on,
	it allows other classes in MVO to set the \ref Set_Heuristics_quick_moves "quick moves" heuristic option to spriting for a segment 
	that will be changing rapidly over a certain period time.  This lets HOOPS render a scene more quickly because it updates only 
	the changing geometry leaving the rest of the scene unchanged.
		\param onoff Pass true if you wish to turn on spriting.  Pass false, if you wish to turn it off.*/
	void			SetSpritingMode(bool onoff) { m_bSpritingAllowed = onoff; } 

	/*! \returns The \ref Set_Rendering_Options_level_of_detail_options "LOD threshold" which is the distance in world 
	space from the camera position. The LOD threshold tells HOOPS when to dynamically switch between levels of detail.
	*/
	int				GetLodThreshold(); 

	/*! This method sets the \ref Set_Rendering_Options_level_of_detail_options "LOD threshold" for which HOOPS will dynamically switch between different levels of detail.
		\param val The threshold distance in world space from the camera position in which HOOPS will 
		dynamically switch between levels of detail.
	*/
	void			SetLodThreshold(int val);

	/*!< \return The key of the HOOPS driver instance segment. */
	HC_KEY			GetViewKey() { return m_ViewKey; }	
	/*!< This method sets the key of the HOOPS driver instance segment. */
	void			SetViewKey( HC_KEY view_key ) { m_ViewKey = view_key; }    

	/*!< \return The key to the link which includes the HBaseModel segment into the scene. */
	HC_KEY			GetIncludeLinkKey() { return m_lIncludeLinkKey; }	
	/*!< This method sets the key of the link which includes the HBaseModel segment into the scene. */
	void			SetIncludeLinkKey( HC_KEY link ) { m_lIncludeLinkKey = link; }    

	/*!< \return The key of the segment used to represent the top of the scene's segment hierarchy.  */ 
	HC_KEY			GetSceneKey() { return m_SceneKey; }    
	
	/*! This method sets the key of the segment used to represent the top of the scene's segment hierarchy. 
	\param scene_key The key of the segment to use at the top-level scene segment.
	*/ 
	void			SetSceneKey( HC_KEY scene_key ) { m_SceneKey = scene_key; } 

	/*! \return The key of the segment below the scene key into which temporary attributes can be set (and safely flushed) that are intended to affect the entire model.  */ 
	HC_KEY			GetOverwriteKey() { return m_OverwriteKey; }    
 
	/*! \return The key of the segment at which shadow maps are set.  Currently defined as the owner of the model include link. */ 
	HC_KEY			GetShadowMapSegmentKey() { return HC_KShow_Owner_By_Key (m_lIncludeLinkKey); }
					
	/*!< \return The key of the segment containing the lights for the scene. */
	HC_KEY			GetLightsKey() { return m_LightsKey; }   

	/*!This method sets the key of the segment containing the lights for the scene. 
	\param lights_key Pass the key of the segment that you want HOOPS to create light for the scene in.
	*/
	void			SetLightsKey( HC_KEY lights_key ) { m_LightsKey = lights_key; }   

	/*!< \return The maximum zoom level for a camera. The value is in object space units.*/
	float			GetZoomLimit() { return m_fZoomLimit; }   

	/*! This method sets the maximum zoom level for a camera. It guarantees that the window always views more than this number of units.
	\param zoomlimit Pass a value in object space units.  
	*/
	void			SetZoomLimit( float zoomLimit ) { m_fZoomLimit = zoomLimit; }   
	
	/*!< This method performs an automatic calculation of the zoom limit based off of the bounding box of the scene.*/
	virtual void	SetZoomLimit();   

	/*!< \return The key of the segment used to hold windowspace attributes. */
	HC_KEY			GetWindowspaceKey() { return m_WindowspaceKey; }   

	/*! This method sets the key of the segment used to hold window space attributes. 
	\param winspace_key The key of the segment you want HOOPS to hold window space attributes.
	*/
	void			SetWindowspaceKey( HC_KEY winspace_key ) { m_WindowspaceKey = winspace_key; }   

	/*!< \return The key of the segment used to store temporary construction geometry. */
	HC_KEY			GetConstructionKey() { return m_ConstructionKey; }   

	/*! This method sets the key of the segment used to store temporary construction geometry. 
	\param construct_key The key to build construction geometry under.
	*/
	void			SetConstructionKey( HC_KEY construct_key ) { m_ConstructionKey = construct_key; }   

	/*!< \return The key of the segment used to store temporary bounding boxes for the scene. */
	HC_KEY			GetBBoxGeometryKey() { return m_BBoxGeometryKey; }   
	/*!< This method sets the key of the segment used to store temporary bounding boxes for the scene. */
	void			SetBBoxGeometryKey( HC_KEY bbox_key ) { m_BBoxGeometryKey = bbox_key; }   

	/*!< \return The key of the segment used to store the view's cutting planes. */
	HC_KEY			GetCuttingPlanesKey() { return m_CuttingPlanesKey; }   

	/*! This method sets the the key of the segment used to store the view's cutting planes. 
	\param cuttingplanes_key Pass the key in which you want to the view's cutting planes.
	*/
	void			SetCuttingPlanesKey( HC_KEY cuttingplanes_key ) { m_CuttingPlanesKey = cuttingplanes_key; }   
	
	/*!< \return The key of the  segment used to store geometry representing cutting plane intersections. */
	HC_KEY			GetCuttingPlaneGeomKey() { return m_CuttingPlaneGeomKey; }   

	/*!This method sets the key of the segment used to store geometry representing cutting plane intersections. 
	\param cuttingplanegeom_key Pass the segment that you want HOOPS to store cutting plane geometry under.
	*/
	void			SetCuttingPlaneGeomKey( HC_KEY cuttingplanegeom_key ) { m_CuttingPlaneGeomKey = cuttingplanegeom_key; }   
	/*!< \return The key of the  segment used to store geometry representing polylines of intersection. */
	HC_KEY			GetIntersectPolylinesKey() { return m_IntersectPolylinesKey; }   

	/*! This method sets the key of the segment used to store intersection polylines. 
	\param interesetpolylines_key Pass the segment key you want HOOPS to store intersection polylines under.
	*/
	void			SetIntersectPolylinesKey( HC_KEY intersectpolylines_key ) { m_IntersectPolylinesKey = intersectpolylines_key; }   
	
	/*! \return The key of the segment that contains the progress bar for loading files. */
	HC_KEY			GetProgressBarKey() { return m_ProgressBarKey; }	/*!< \return The key of the progress bar segment. */

	/*! This method sets the key of the segment that HBaseView uses to create the progress bar for loading files under. 
	\param view_key The key of the segment to use for creating the progress bar.
	*/
	void			SetProgressBarKey( HC_KEY key ) { m_ProgressBarKey = key; } 

	
	/*!< \return True if the last update was interrupted. */
	int				GetUpdateInterrupted() { return m_UpdateInterrupted; }  

	/*!Use this method to tell HOOPS if an update was interrupted.
	\param val Pass 1 to indicate that an update was interrupted or 0 to indicate that it wasn't.
	*/
	void			SetUpdateInterrupted( int val ) { m_UpdateInterrupted = val; } 

	/*!< \return The time since the last event, as reported by #HC_Show_Time. */
	float			GetIdleTime(); 

	/*! This method reports that an event has occurred, so the idle time should be returned to zero. */
	void			ResetIdleTime(); 
	
	/*! \return The method for quick moves last used by HOOPS.  Applications can use this information to make decisions regarding the UI.  
	For example, XOR may look best if drawn wireframe only. */	
	HQuickMovesMethod GetQuickMovesMethod () const;  

	/*!< \return A pointer to the debug zbuffer object (associated with any active zbuffer visualization winow), or null if there is none. */
	class HDebugZBuffer *GetDebugZBuffer() { return m_pDebugZBuffer; } 

	/*! This method sets the pointer to the debug zbuffer object.  The HBaseView will automatically be deleted on destruction,
		but not on reassignment. */
	void SetDebugZBuffer(HDebugZBuffer *dzb) { m_pDebugZBuffer = dzb; }; 
	
	/*!< \return The current axis mode. */
	AxisMode		GetAxisMode() {return m_bAxisMode; }   
	
	/*! This method creates an axis triad which is a separate HOOPS window in the current view at bottom left 
		that contains an axis display for directional reference. 
		\param mode The mode in which you wish the axis triad to appear.
	*/
	virtual void			SetAxisMode(AxisMode mode);

	/*! This method does the actual work of settig the axis mode.  Users should use SetAxisMode().
		\param mode The mode in which you wish the axis triad to appear.
	*/
	virtual void			OnSetAxisMode( AxisMode mode );
	
	/*! This method recreates the axis triad to account for any changes in behavior that might have been requested. */
	void			UpdateAxisTriad() { SetAxisMode(GetAxisMode()); }
	
	/*! This method adjusts the size of the axis triad to keep it as a constant size.
		\param axis_window_pixels_in The width and height of the axis triad, given as a number 
				of pixels.  The value is defaulted to negative -1 which leaves the size unchanged 
				but adjusts it to keep it constant in screen space.
		\param rect_in The screen space rectangle of the drawing context.  This parameter is optional.*/
	virtual void			AdjustAxisWindow( int axis_window_pixels_in = -1, HIntRectangle *rect_in=0 );
	
	/*! This function is used as a callback to dynamically adjust the axis window.	*/	
	static	void			AdjustAxisWindowCallback(int signal, void * signal_data, void * user_data);

	/*! This function is used as a callback to dynamically adjust the HAbsoluteWindow's.	*/
	static	void			AdjustWindowCallback(int signal, void * signal_data, void * user_data);

	/*! \return The current shadow mode. */
	HShadowMode	GetShadowMode() {return m_ShadowMode; }   

	/*! This method updates the current shadow of model in the current view. 
	\param sm The new shadow mode that you want the view to use.
	\param saveMode Pass true to save the mode internally independent of the HOOPS database.
	*/
	virtual void SetShadowMode (HShadowMode sm, bool saveMode = true);	

	/*!
		This method sets up the reflection plane in the current view.
		\param state whether the reflection plane should be on or off.
		\param opacity the opacity of the reflection.
		\param fading if true, the reflection will fade as it moves away from the camera.
		\param attenuate determines whether the reflection should fade as the model moves away from it.
		\param hither if attenuate is true, this is the near plane where attenuation begins.
		\param yon if attenuate is true, this is the far plane.  attenuation occurs linearly between hither and yon.
		\param blur indicates whether the reflection will be blurred.
		\param blur_level the amount of blurring applied.
	*/
	virtual void SetReflectionPlane(bool state, float opacity=0.5f, bool fading=true, bool attenuate=false,
			float hither=0.0f, float yon=1.0f, bool blur=false, int blur_level=1);

	/*! \return The key of the default light for the scene. */
	HC_KEY			GetDefaultLightKey(int which = 0) { return m_SceneLight[which]; }   

 
	/*! \return True if the selection area for the HOpSelectArea operator should be drawn transparent. */
	bool 			GetTransparentSelectionBoxMode() {return m_bTransparentSelectionBox; }   
	
	/*! Determines whether the HOpSelectArea operator should draw its area with a transparent rectangle as opposed to just an outline. 
	\param mode If the value is true, the selection area will be draw with a transparent rectangle.  If the value is false, it will just have an outline.
	*/
	void			SetTransparentSelectionBoxMode(bool mode) { m_bTransparentSelectionBox = mode; }

	/*! This method deletes the items in the HSelectionSet object associated with this view.
	  \param emit_message  Pass to indicate that a message should be sent to the emit_message_function, which if 
							implemented would typically use the HOOPS/Net 'Send' function to dispatch the message 
							to clients.
	*/
	virtual	void	DeleteSelectionList(bool emit_message=false);

	/*!
		This method adjusts the axis display with regard to the camera setting of the view window.  The display indicates 
		the absolute position of the camera related to the three major view axes (x,y,z).  The arrows always point in the 
		direction of increasing value.
		\param sname The segment name on the view that contains the camera settings for the axis display.
	*/
	void			RotateOnUpdate(const char *sname);

	/*!
	  This method sets the window color.
	  \param rgb An HPoint containing the rgb color at the top of the window where x is the r value, y is the g value and z is the b value.
	  \param emit_message  Pass true to indicate that a message should be sent to the emit_message_function, which if implemented
						   would typically use the HOOPS/Net 'Send' function to dispatch the message
						   to clients.
	*/
	virtual void SetWindowColor(HPoint rgb, bool emit_message=false);
	
	/*!
	  This method sets the window color, and does an interpolation between color values if top and bottom are different.  This is done in the 
			IM Callback HImUtility::draw_gradiated_window_background.
	  \param rgbtop An HPoint containing the rgb color at the top of the window where x is the r value, y is the g value and z is the b value.
	  \param rgbbottom An HPoint containing the rgb color at the bottom of the window,where x is the r value, y is the g value and z is the b 
	  value. If the color is different, then rgbtop we will smoothly interpolate between the two color values.
	  \param emit_message  Pass true to indicate that a message should be sent to the emit_message_function, which if implemented
						   would typically use the HOOPS/Net 'Send' function to dispatch the message
						   to clients.
	*/
	virtual void SetWindowColor(HPoint rgbtop, HPoint rgbbottom, bool emit_message=false); 

	/*!
	  This method sets the window color, and does an interpolation between color values if top and bottom are different. Additionally
	  a percentage value can be specified which determines how far into the vertical direction the gradient will go.
	  \param rgbtop An HPoint containing the rgb color at the top of the window where x is the r value, y is the g value and z is the b value.
	  \param rgbbottom An HPoint containing the rgb color at the bottom of the window,where x is the r value, y is the g value and z is the b 
	  value. If the color is different, then rgbtop we will smoothly interpolate between the two color values.
	  \param percentage The percentage to extend gradient in the vertical direction.
	  \param emit_message  Pass true to indicate that a message should be sent to the emit_message_function, which if implemented
						   would typically use the HOOPS/Net 'Send' function to dispatch the message
						   to clients.
	*/
	virtual void SetWindowColorRange(HPoint rgbtop, HPoint rgbbottom, int percentage, bool emit_message=false); 


	/*!
	  This method give the window a texture.
	  \param name The name of a texture material 
	  \param emit_message  Pass true to indicate that a message should be sent to the emit_message_function, which if implemented
						   would typically use the HOOPS/Net 'Send' function to dispatch the message
						   to clients.
	*/
	virtual void SetWindowMaterial(const char * name, bool emit_message=false);

	/*!
	  This method sets the window color based on the information in the message.
	  \param in_data A character pointer containing the window color information.
	  \param data_length The size of <i>in_data</i>.
	*/
	virtual void SetWindowColorFromMessage(const char *in_data, unsigned int data_length);

	/*! Use this method to get the RGB value of the current window's background in HPoint passed.
	\param color_out Returns the window's background color in the form of an HPoint where x is the red value, y is green value and z is the blue value.
	*/
	void GetWindowColor(HPoint &color_out);
	
	/*! Use this method to get the top and bottom RGB values of the current window's background. 
	\param color_top Returns the window's background top color in the form of an HPoint where x is the red value, y is green value and z is the blue value.
	\param color_bottom Returns the window's background bottom color in the form of an HPoint where x is the red value, y is green value and z is the blue value.
	*/
	void GetWindowColor(HPoint &color_top, HPoint &color_bottom);
	
	/*! Use this method to get top and bottom RGB values of the current window's background as well as the percentage the gradient extends in the vertical direction.
	\param color_top Returns the window's background top color in the form of an HPoint where x is the red value, y is green value and z is the blue value.
	\param color_bottom Returns the window's background bottom color in the form of an HPoint where x is the red value, y is green value and z is the blue value.
	\param percentage Returns the percentage the gradient extends in the vertical direction.
	*/
	void GetWindowColor(HPoint &color_top, HPoint &color_bottom, int &percentage);

	/*! Use this method to get the raw color and pattern string for the current window.
	\param out_material Returns the raw color string for the window background
	\param out_pattern Returns the raw pattern string for the window pattern
	*/
	void GetWindowMaterial(char * out_material, char * out_pattern);

	/*! This method sets the current operator to NewOperator. 
		\param NewOperator A pointer to the new operator. 
	*/
	void			SetOperator(HBaseOperator *NewOperator);


	/*! <b> Deprecated.</b>  Please use #SetOperator(). */
	void			SetCurrentOperator(HBaseOperator *NewOperator) { SetOperator(NewOperator); }

	/*!
		\return  A pointer to the current operator.
	*/
	HBaseOperator*	GetOperator() { return m_pOperator; }

	/*!
		<b>Obsolete function.<b> Please use #GetOperator().
		\return  A pointer to the current operator
	*/
	HBaseOperator*	GetCurrentOperator() { return m_pOperator; }  	


	/*! \return The key of the segment containing the axis triad. */
	HC_KEY	GetAxisTriadKey() {return m_AxisTriadKey;}  

	/*! \return The value of the flag indicating whether overlapped text should be hidden. */
	bool	GetHideOverlappedText(){ return m_bHideOverlappedText;}  

	/*!< This method determines the visibility of overlapped text.
	\param hide Pass true to hide overlapped text or false to show it.
	*/
	void	SetHideOverlappedText(bool hide);  

	/*! This method informs the view of the visibility of the navcube. 
	\param onoff Pass true to indicate that the navcube is visible or false to indicate that it is not.
		*/
	void	SetNavCube(bool onoff) { m_bNavCube = onoff;}	

	/*! \return True if the camera will smoothly from one position to the next.*/
	bool	GetSmoothTransition() { return m_bSmoothTransition;}	

	/*!< This method determines whether the camera should smoothly transition from the previous position to the new one. 
	\param onoff Pass true to turn on smooth transition.  Pass false to turn it off.
	*/	
	void	SetSmoothTransition(bool onoff) { m_bSmoothTransition = onoff;} 

	/*! This method determines the length of the smooth transition in seconds.  
		\param duration The duration of the transiation in seconds. 
	*/
	void	SetSmoothTransitionDuration(float duration) { m_SmoothTransitionDuration = duration;} 
	
	/*! \return The length of the smooth transition in seconds. */
	float	GetSmoothTransitionDuration() { return m_SmoothTransitionDuration;}	

	/*! \return True if the light follows the camera or false if it doesn't. */
	bool	IsLightFollowCamera(){return m_bLightFollowsCamera;}  

	/*! Use this method to indicate if you want the light to follow the camera.
	\param onoroff Pass true for the light to follow the camera or false if it should not.
	*/
	void	SetLightFollowsCamera(bool onoroff); 	

	static const int MaxLights = 6;

	/*! Use this method to control the number of lights in the scene.
	\param count the number of lights.
	*/
	void	SetLightCount(int count);

	/*! This method returns the number of lights currently set in the scene.
	\return The number of lights.
	*/
	int	GetLightCount() { return m_LightCount; }
	
	/*!
	  This method tells the view that the camera position has been modified.  Camera-dependent information
	  such as lights which will follow the camera by default are updated here.  
	  \param emit_message  Indicates that a message should be sent to the emit_message_function, which if implemented
								would typically use the HOOPS/Net 'Send' function to dispatch the message to clients
	  \param smooth_camera_transition Indicates that the camera should smoothly transition from the last position to 
	  the new position.
	*/
	virtual void CameraPositionChanged(bool emit_message=false, bool smooth_camera_transition=false);


	/*!
		This method is called before a camera change.
	*/
	virtual void PrepareForCameraChange();


	/*!
	  This method sets the camera for the scene.
	  \param pos A HPoint defining the camera position.
	  \param tar A HPoint defining the camera target.
	  \param up A HPoint defining the camera up vector.
	  \param fieldx The minimum amount of the scene that should be viewable in the horizontal direction about the camera target in object space units.
	  \param fieldy The minimum amount of the scene that should be viewable in the vertical direction about the camera target in object space units.
	  \param *proj projection mode of camera, 'orthographic, 'perspective' and 'stretched' are all valid options.
	*/
	virtual void SetCamera(HPoint pos, HPoint tar, HPoint up, float fieldx, float fieldy, const char *proj);

	/*!
	  This method sets the intial camera for the scene usually after loading.
	  \param pos A HPoint defining the camera position.
	  \param tar A HPoint defining the camera target.
	  \param up A HPoint defining the camera up vector.
	  \param fieldx The minimum amount of the scene that should be viewable in the horizontal direction about the camera target in object space units.
	  \param fieldy The minimum amount of the scene that should be viewable in the vertical direction about the camera target in object space units.
	  \param *proj projection mode of camera, 'orthographic, 'perspective' and 'stretched' are all valid options.
	*/
	virtual void SetInitialCamera(HPoint pos, HPoint tar, HPoint up, float fieldx, float fieldy, char const * proj);


	/*!
	  This method populates the parameters passed by the the caller with the definition of the camera currently in use.
	  \param pos Returns the HPoint defining the camera position. 
	  \param tar Returns the HPoint defining the camera target.
	  \param up Returns the HPoint defining the camera up vector.
	  \param fieldx Returns the float value indicating in object space units the minimum amount of the scene that should be viewable in the horizontal direction about the camera target. 
	  \param fieldy Returns the float value indicating in object space units the minimum amount of the scene that should be viewable in the vertical direction about the camera target.
	  \param *proj Returns the projection mode of camera, 'orthographic, 'perspective' and 'stretched' are all valid options.
	*/
	virtual void GetCamera(HPoint *pos, HPoint *tar, HPoint *up, float &fieldx, float &fieldy, char *proj);

	/*!
	 This method sets the camera on the scene.
	  \param camera HCamera object defining the camera.	
	*/
	virtual void SetCamera(HCamera camera);

	/*!
	  This method populates the HCamera object passed by the the caller with the definition of the camera currently in use.
	  \param camera Returns the HCamera object defining the currently used camera,
	*/
	virtual void GetCamera(HCamera *camera);

	/*!
	  This method has been deprecated.  This is the method that HNet calls to set the camera on the scene.
	  \param *data A character string containing the camera definition.
	  \param data_length The length of data string.
	*/
	virtual void SetCameraFromMessage(const char *data, unsigned int data_length);

	/*!
	  This method has been deprecated. InsertHSFData inserts HSF data into a named segment in the model tree.
	  \param segment The segment in the base of the model tree that the HSF data should be inserted.
	  \param in_data The binary HSF data.
	  \param data_length The length of the binary HSF data.
	  \param emit_message Pass true to indicate that this data should be shared with other members of a HNet session.
	*/
	virtual void InsertHSFData(const char *segment, const char *in_data, unsigned int data_length, bool emit_message=false);

	/*!
	  InsertHSFDataFromMessage is a function HOOPS/Net calls to insert HSF data in a scene.
	  \param in_data The binary HSF Data.
	  \param data_length The length of the in_data array.
	*/
	virtual void InsertHSFDataFromMessage(const char *in_data, unsigned int data_length);

	/*!
	  This method has been deprecated. EmitHSFData takes the HSF data and sends it to HNet which will then send it to InsertHSFDataFromMessage of all the clients involved in the HNet session.
	  \param segment The segment within the model which the user wants the HSF data to be inserted into.
	  \param in_data The binary HSF data.
	  \param data_length The length of the in_data array.
	*/
	virtual void EmitHSFData(const char *segment, const char *in_data, unsigned int data_length);

	/*!
	  This method has been deprecated. This method sets the render mode for this view to NewRenderMode.  The default is HRenderGouraud.
	  \param NewRenderMode  The new render mode.
	  \param emit_message  Pass true to indicates that an HNet message should be generated/sent.
	*/
	virtual void SetRenderMode (HRenderMode NewRenderMode, bool emit_message=false);

	/*!
	  This method has been deprecated. This method is called by HNet to change the rendermode of the client involved in a HNet session.
	  \param in_data the data containing the requested rendermode.
	  \param data_length The length of the in_data array.
	*/
	virtual void SetRenderModeFromMessage(const char *in_data, unsigned int data_length);

	/*!
	 This method has been deprecated. EmitFlushMessage sends a message to all clients of HNet session to flush geometry from the Scene, CuttingPlane, CuttingPlaneGeometry and Construction segments as well as reset the HSharedKey object and restart the HStreamToolkit.
	*/
	virtual void EmitFlushMessage();

	/*!
	This method has been deprecated. This is the message that a client receives after the controlling client emits a flush message.  
	The parameters in_data and data_length are not referenced in this method.  They are present so
	that this method conforms to the signature of the "FromMessage" methods.
	  \param in_data Not referenced in this method.
	  \param data_length Not referenced in this method.
	*/
	virtual void FlushFromMessage(const char *in_data, unsigned int data_length);


	/*!
	This method has been deprecated. This method flushes geometry from the Scene, CuttingPlane, CuttingPlaneGeometry and Construction segments as 
	well as resets the HSharedKey object and restarts the HStreamToolkit.
	  \param emit_message Pass true if  you want a message emitted telling other clients to 'Flush' as well or false if no message should be emitted.
	*/
	virtual void Flush(bool emit_message=false);

	/*!
		This method has been deprecated. SetEmitMessageFunction is what you use to register the function which will send messages to the HNet server during any HNet sessions.
		\param new_emit_message_function A pointer to the emit message function.
		\param user_data Users set this when registering the emit_function. When the emit_message_function is subsequently 
		called users can query this value to determine the origins of the callback function.
	*/
	void SetEmitMessageFunction(
		HBaseViewEmitMessageFunction new_emit_message_function,
		void * user_data=0) {
		emit_message_function=new_emit_message_function;
		emit_message_function_user_data=user_data;};

	/*!
	This method has been deprecated. This method indicates whether or not the HBaseView is emitting messages.
	\return True if the HBaseView is emitting messages, false if otherwise.
	*/
	bool EmittingMessages() {return emit_message_function != 0;};
	
	/*! This method has been deprecated. 
	  \return 	An HBaseViewEmitMessageFunction pointer to the emit_message_function.
	*/
	HBaseViewEmitMessageFunction GetEmitMessageFunction() {return emit_message_function;};

	/*! This method has been deprecated. 
	  \return A pointer to emit_message_function_user_data.
	*/
	void * GetEmitMessageFunctionUserData() {return emit_message_function_user_data;};

	/*! This method has been deprecated. 
	  EmitSelectMessage emits a message to the HNet server telling all other clients to select an object.
	  \param key key to the HOOPS/3dGS object to be selected
	  \param isSelect boolean value indicating whether or not the object is already selected
	*/
	virtual void EmitSelectMessage(HC_KEY key, bool isSelect);

	/*!
	  This method has been deprecated. SelectFromMessage is the function the HNet server calls to tell the slave clients to select an object.
	  \param *in_data data containing the selection information
	  \param data_length length of the in_data array
	*/
	virtual void SelectFromMessage(const char *in_data, unsigned int data_length);

	/*!
	  This method has been deprecated. EmitDeSelectAllMessage emits a message to the HNet server to tell all clients involved in the view's HNet session to deselect everything.
	*/
	virtual void EmitDeSelectAllMessage();


	/*!
	  This method has been deprecated. EmitDeleteSelectionListMessage emits a message to the HNet server telling all HNet clients to delete thei selection list.
	*/
	virtual void EmitDeleteSelectionListMessage();


	/*!
	  This method has been deprecated. This is the function the HNet server calls on each HNet client when it receives a DeleteSelectionList message from a controlling client.
	*/
	virtual void DeleteSelectionListFromMessage();


	/*!
	  This method has been deprecated. This is the function the HNet server calls on each HNet client when it receives a DeSelectAll message from a controlling client.
	*/
	virtual void DeSelectAllFromMessage();


	/*!
	  This method has been deprecated. EmitSetModellingMatrixMessage sends a message to the HNet server with a modelling matrix which is to be set on a specific segment.
	  \param key key to the segment that the modelling matrix should be applied to.
	*/
	virtual void EmitSetModellingMatrixMessage(HC_KEY key);


	/*!
	  This method has been deprecated. EmitSetModellingMatrixMessage should be used if you want all the modelling matrices on each element in the selection list to be applied to the same objects in the other 
	  HNet clients.  This is used in both the HOpObjectTranslate and HOpObjectRotate operators. 
	*/
	virtual void EmitSetModellingMatrixMessage();

	/*!
		This method has been deprecated. Emits the hsf data of the segment to the peers in the collabrative session if any.

		\param key A HOOPS key denoting the segment to be emitted.
		\param emit_contents_only If true, only contents of the segments will be emitted. If false, the segment itself will be emitted along
									with the contents. Default is false.
	*/
	virtual void EmitSegment(HC_KEY key, bool emit_contents_only = false);

	/*!
	  This method has been deprecated. EmitDeleteByKeyMessage sends a message with the key to an object to be deleted.
	  \param key A HOOPS key denoting segment to be deleted.
	*/
	virtual void EmitDeleteByKeyMessage(HC_KEY key);

	/*!
	  This method has been deprecated.  DeleteByKeyFromMessage is the function that will receive any messages which originate in a EmitDeleteByKey message.
	  \param in_data contains the key to the object to be selected
	  \param data_length length of the in_data array
	*/
	virtual void DeleteByKeyFromMessage(const char *in_data, unsigned int data_length);

	/*!
	  This method receives all SetModellingMatrix messages.
	  \param in_data this contains the modelling matrix information
	  \param data_length length of the in_data array
	*/
	virtual void SetModellingMatrixFromMessage(const char *in_data, unsigned int data_length);
 
	/*!
	  \return A pointer to m_pSharedKey which is used to map the keys of HOOPS/3dGS objects contained in different HBaseView objects.
	*/
	HSharedKey * GetSharedKey() {return m_pSharedKey;};


	/*!
	  This method should be called when you want to associate keys of objects from different HBaseView objects.  This is mostly used for HNet sessions.
	*/
	virtual void StartSharedKey();


	/*! \enum  HAnimateMode Indicates how the scene should be animated; obselete; animation should be performed using MVO's behavior/animation support. */
	enum HAnimateMode {
		HAnimateScene	 =			0x00000001 
	};


	/*! 
	  This method rotates a camera around the scene based on the degrees of rotation and the number of animation steps passed.
	  This method is used in the PartViewer's "Test Performance" functionality.
	  \param animation_steps Number of times that the scene should be updated
	  \param animate_flags Specifies what to animate.
	  \param MaxRotation Degrees Of Rotation
	*/
	virtual void Animate (int animation_steps = 30, HAnimateMode animate_flags = HAnimateScene, int MaxRotation = 360);
	
	/*! Use this method to indicate when geometry in a view changes. During #Update(), this information is used to 
	determine if recalculations are needed like if cutting lines need to be updated. */
	virtual void SetGeometryChanged();

	/*! This function tells you whether or not a scene bounding is set.
		\return true if a scene bounding is set, false otherwise.
	*/
	bool GetSceneBoundingSet() { return m_bSceneBoundingSet; }
	
	/*! This function sets a scene bounding.
		\param min the minimum values of the bounding
		\param max the maximum values of the bounding
	*/
	void SetSceneBounding(HPoint &min, HPoint &max);

	/*! This function invalidates (unsets) any current scene bounding. */
	void InvalidateSceneBounding() { m_bSceneBoundingSet = false; }

	/*! This method indicates whether marker drawing will bias toward performance or quality.
		\return true if fast marker drawing will be used, false otherwise.
	*/
	void SetFastMarkerDrawing(bool b_fast);

	/*! This method controls whether marker drawing will bias toward performance or quality.
		\param b_fast if true, fast marker drawing will be used, otherwise nice marker drawing will be used.
	*/
	bool GetFastMarkerDrawing();
	
	/*!
		This method controls whether the HOOPS view will be single- or double-buffered.  
		It must be called before the first update to have any effect.
		\param double_buffer If true, the view will be double-buffered, otherwise it will be single-buffered.
	*/
	void SetDoubleBuffering(bool double_buffer);
	
	/*! Call this method to clear that flag that changes have occurred in the geometry. #Update() calls this method.*/
	virtual void ClearGeometryChanged() {m_bGeometryChanged = false; };

	/*! \return True if geometry has changed since the last update. */
	virtual bool GeometryChanged() {return m_bGeometryChanged;};
	
	/*! Request notification when a particular event occurs. 
		\param signal Event for which we are requesting notification (see enum HSignal in HGlobals.h).
		\param callback The function to be called when the event happens.
		\param user_data Data that should be passed along to the callback function. 
		\return A cookie (i.e. a unique id) that we will need to use when we want to clean up this notification request. */
	unsigned long SetSignalNotify( int signal, HSignalNotifyFunction callback, void *user_data ); 
	
	/*! Rescind a signal request notification. 
		\param cookie the unique identifier of the request to be cancelled. */
	void UnSetSignalNotify( unsigned long cookie );											

	/*!
	 This method sets the \ref Set_Rendering_Options_color_interpolation "color interpolation" rendering option on the current scene.
	  \param onoff Pass true to turn on color interpolation or false to turn it off.
	*/
	void SetColorInterpolation(bool onoff);

	/*!
	  This method sets the \ref Set_Rendering_Options_color_index_interpolation "color index interpolation" rendering option on the current scene.
	  \param onoff Pass true to turn on color index interpolation or false to turn it off.
	  \param isolines Pass true to render color interpolation data in the form of polylines instead of color band or false to not render polylines.  Note this parameter only applies if you pass true for the <i>onoff</i> parameter.
	  */
	void SetColorIndexInterpolation(bool onoff, bool isolines = false);

	/*!
		Call this method to find out if color index interpolation is set on the scene.
	  \param option Returns either "on" or "isolines" if the color index interpolation is on.
	  \return True if color index interpolation is on or false if it is not on.
	*/
	bool GetColorIndexInterpolation(char * option = 0);

	/*!
		Call this method to find out if frame buffer effects are active (e.g. bloom, depth of field, ambient occlusion
		or fast silhouettes).
	  \param option Returns all driver options relevant to frame buffer effects.
	  \param border Returns the search radius of the current active effects (and, hence, the recommended number of 
					extra pixels to be added around the edge of an image in order to avoid artifacts).
	  \return True if active. Always false on drivers other than dx9 or opengl2.
	*/
	bool GetFrameBufferEffectsOptions(char * options = 0, int *border = 0);

	/*!
	  Notifies the appropriate subscribers about the event.
	  \param signal event which is to be notified
	  \param signal_data data associated with this event to be passed to the subscribers.
	*/
	void Notify(int signal, void * signal_data = 0);

	/*!
	  This event method receives update events.
	  \param antialias  If true, then an anti-aliased version of the scene will be drawn.
	  \param forceUpdate  If true, then an update will be forced.
	  \return A standard event code indicating if the event was consumed or passed.
	*/	
	int UpdateEvent(bool antialias = false, bool forceUpdate = false);	
	/*!
	  \return True if an update was requested and false if no update was requested.
	*/	
	bool	GetUpdateRequested() { return m_bUpdateRequested; }
	/*!
	  Call this method to indicate to the view that you would like it to performat an update.
	  */	
	void	RequestUpdate() { m_bUpdateRequested = true; }
	/*!
	  This method sets the update request flag to false indicating that you do not need the view to perform an update.
	*/	
	void	ClearUpdateRequest() { m_bUpdateRequested = false; }

	/*!
	  Use this method to prevent an update from occurring. This can be useful if you are in the middle of an operation and 
	  do not want a timer event to cause an update to occur.
	  \param suppress If this value is set to true, then no updates will be made.
	*/	
	void	SetSuppressUpdate(bool suppress) {
		suppress?m_SuppressUpdate++:m_SuppressUpdate--;
		if (m_SuppressUpdate<0)
			m_SuppressUpdate = 0;
	}

	/*!
	  \return The value of the suppress update flag.  If the value is true, then no updates will 
	  be made.
	*/	
	bool	GetSuppressUpdate() { return m_SuppressUpdate > 0; }

		/*!
	  Use this method to prevent an timer update from occurring. This can be useful if you are in the middle of an operation and 
	  do not want a timer event to cause an update to occur.
	  \param suppress If this value is set to true, then no updates will be made.
	*/	
	void	SetSuppressUpdateTick(bool suppress) { 
		suppress?m_SuppressUpdateTick++:m_SuppressUpdateTick--;
		if (m_SuppressUpdateTick<0)
			m_SuppressUpdateTick = 0;
	}

	/*!
	  \return The value of the suppress update flag.  If the value is true, then no updates will 
	  be made.
	*/	
	bool	GetSuppressUpdateTick() { return m_SuppressUpdateTick > 0; }

	/*!
	  Use this method to determine how the view is updated by setting one of the 
	  three ViewUpdateModes. ViewUpdateMode::Continuous is the recommended mode for most 
	  applications.  In this mode, HOOPS will poll at regular intervals for the posted 
	  update events.  If you choose this mode, please provide a polling interval value is 
	  ticks. On the opposite end of the spectrum, ViewUpdateMode::OnDemand tell HOOPS not 
	  to perform any update automatically but instead, it expects the application to 
	  explicitly call Update when it is required.  The final update mode is 
	  ViewUpdateMode::Deferred which makes continuous culled, timed updates drawing culled 
	  items if time permits. When HBaseView is initialized, the mode is set to 
	  ViewUpdateMode::OnDemand.

	  \param mode The new view update mode ViewUpdateMode::Continuous, ViewUpdateMode::OnDemand or ViewUpdateMode::Deferred.
	  \param rate Rate, in ticks, in which updates are generated.
	*/		
	void	SetViewUpdateMode(ViewUpdateMode mode, float rate = 0.01f);

	/*!
	  \return The current view update mode.
	*/	
	ViewUpdateMode	GetViewUpdateMode() { return m_ViewUpdateMode; }

	/*!
	  The Tick function called from the timer manager.
	  \param request_time The time in which the function requested a call.
	  \param actual_time The actual time the function was called.
	  \param user_data The user data.
	  \return True if the event was processed.
	*/		
	static bool UpdateTick(float request_time, float actual_time, void * user_data);

	/*!
	  The ProgressBarCallback usually called from streaming toolkit.
	  \param so_far The amount of data received.
	  \param expected The amount Of data expected.
	  \param user_data The user data.
	  \return True if the event was processed.
	*/		
	static bool StreamProgressCallback(unsigned long so_far, unsigned long expected, void * user_data);

	/*! This method clears outs the progress bar window. */		
	void FlushProgressBar();

	/*! \return The undo manager associated with this view. */		
	HUndoManager *GetUndoManager();

	/*! \return The event manager associated with this view. */		
	HEventManager *GetEventManager();

	/*! \return The object manager associated with this view. */		
	HObjectManager *GetHObjectManager();

	/*! \return True to indicate that dynamic highlighting is turned on or false if it is turned off. */
	bool	GetDynamicHighlighting() { return m_bDynamicHighlighting; }

	/*! This enables/disables dynamic highlighting for the geometry in the scene.  If dynamic highlighting is set to true,
	HOOPS will highlight different segments in the scene as the user moves the mouse cursor over the segment.
	  \param state  Pass true to turn on dynamic highlighting or false to turn it off.*/
	void	SetDynamicHighlighting(bool state);

	/*! This handles no button down and move event for dynamic highlighting of geometry.  If dynamic highlight is on, 
	then as the mouse cursor passes over a given segment, HOOOPS will highlight that segment.  
		\param hevent An HEventInfo object containing information about the current event.
	  \return An #HOperatorReturn indicating the status of the event.
	  */
	virtual int OnNoButtonDownAndMove(HEventInfo &hevent);

	/*! This method handles the left button down and move event for dynamic highlighting of geometry.
		\param hevent An HEventInfo object containing information about the current event.
	  \return An #HOperatorReturn indicating the status of the event.
	  */
	virtual int OnLButtonDownAndMove(HEventInfo &hevent);

	/*! This method handles the left button double click event for object selection and setting up handles 
	for object manipulation.
		\param hevent An HEventInfo object containing information about the current event.
	  \return An #HOperatorReturn indicating the status of the event.
	  */
	virtual int OnLButtonDblClk(HEventInfo &hevent);

	/*! Currently not handled.
		\param hevent An HEventInfo object containing information about the current event.
	  \return An #HOperatorReturn indicating the status of the event.
	  */
	virtual int OnRButtonDownAndMove(HEventInfo &hevent);
	/*! Currently not handled.
		\param hevent An HEventInfo object containing information about the current event.
	  \return An #HOperatorReturn indicating the status of the event.
	  */
	virtual int OnMButtonDownAndMove(HEventInfo &hevent);
	/*! Currently not handled.
		\param hevent An HEventInfo object containing information about the current event.
	  \return An #HOperatorReturn indicating the status of the event.
	  */
	virtual int OnLRButtonDownAndMove(HEventInfo &hevent);

	/*! This method handles the left button down event for axis based camera manipulation.
		\param hevent An HEventInfo object containing information about the current event.
	  \return An #HOperatorReturn indicating the status of the event.
		*/		
	virtual int OnLButtonDown(HEventInfo &hevent);	

	/*! This method handles mouse move events.
		\param hevent An HEventInfo object containing information about the current event.
	  \return An #HOperatorReturn indicating the status of the event.
	  */
	virtual int OnMouseMove(HEventInfo &hevent);

	/*! This method handles left button up events finalizing any object selection activity and 
	camera manipulation.
		\param hevent An HEventInfo object containing information about the current event.
	  \return An #HOperatorReturn indicating the status of the event.
	  */
	virtual int OnLButtonUp(HEventInfo &hevent);

#if 0
	/*! This method handles mouse wheel events for camera manipulation.
		\param hevent An HEventInfo object containing information about the current event.
	  \return An #HOperatorReturn indicating the status of the event.
	  */
	virtual int OnMouseWheel(HEventInfo &hevent);
#else
	/*! This method handles mouse wheel events for camera manipulation.
		\param hevent An HEventInfo object containing information about the current event.
	  \return An #HOperatorReturn indicating the status of the event.
	  */
	virtual int OnMouseWheel(HEventInfo &hevent);
	
	/*!	This function attempts to come up with a reasonable camera target.
	If something is under the mouse, project the current target into the same plane as the item under the mouse. */
	virtual bool ComputeReasonableTarget(HPoint & new_tar, HPoint const & mouse_win, HPoint const & tar_orig);

	/* This method adjusts the old field to the new target so the picture on the screen won't change. */
	void ComputeNewField(float & width, float & height, HPoint const & new_tar,
								 HCamera const & old_camera);
#endif

	/*! This function is called when a key is pressed. */
	virtual int OnKeyDown(HEventInfo &hevent);

	/*! This function is called when a key is released. */
	virtual int OnKeyUp(HEventInfo &hevent);

	/*! This method sets the shadow color.
		\param color The new shadow color passed as an HPoint where x is the red value, y is the green value and g is the blue value. */		
	virtual void SetShadowColor(HPoint color);

	/*! This method allows you to set a custom light direction for simple shadows.
		\param explicit_light Whether or not to use an explicit light.  If false, a light direction will be automatically generated.
		\param direction A pointer to a HPoint object that determines the direction of the light.  Passing null is equivalent to passing false for explicit_light.
	*/
	virtual void SetShadowLightDirection(bool explicit_light, HPoint const * direction = 0);

	/*! This method allows you to set a custom light direction for simple shadows.
		\param direction A pointer to a HPoint object that determines the direction of the light, will be null if unset.  Returned to user.
		\return true if an explicit light direction is set, false otherwise.
	*/
	virtual bool GetShadowLightDirection(HPoint * direction = 0);

	/*!
		Toggles whether or not transparency is considered when casting simple shadow.
	*/
	virtual void SetShadowIgnoresTransparency(bool ignore);

	/*!
		Shows whether or not transparency is considered when casting simple shadow.
	*/
	virtual bool GetShadowIgnoresTransparency();

	/*!	Handles the dynamic highlighting logic.
		\param mouse_pos the cursor position in window space. */
	virtual void DoDynamicHighlighting(HPoint mouse_pos);

	/*! This method sets the shadow resolution.
		\param res The new shadow resolution which is number from 32 to 1024.*/		
	virtual void SetShadowResolution(int res);

	/*! This method sets how blurring will be done on shadows.
		\param blur A number between 1 to 31 indicating the level of blurring (softening) that is applied to the shadow*/		
	virtual void SetShadowBlurring(int blur);

	/*! This method checks the flags related to updating the scene and then calls #HC_Update_Display. */		
	virtual void InternalUpdate();

	/*! Use this method to enable or disable updates of shadows in the scene.  If the mode is on, shadows will be redrawn if 
	there are any changes to the geometry.  
		\param onoff Pass true to turn shadow regeneration on or false to turn it off. */		
	virtual void SetShadowRegenerationMode(bool onoff) { m_bShadowRegeneration = onoff; }

	/*!This method suppresses shadow regeneration for the next update. After the update is performed, 
	the m_bShadowRegeneration flag will be set to true again and the toggle value will be set to false.*/
	virtual void SetShadowRegenerationToggle() { m_bShadowRegenerationToggle  = true; m_bShadowRegeneration = false; }

	/*! 
		\return True if cutting planes will be redrawn when there are changes to the geometry or false if they will not be updated. */		
	bool GetCuttingPlaneRegenerationMode() { return m_bCuttingPlaneRegeneration; }
 
	/*! Use this method to enable or disable updates to the cutting planes in the scene.  If the mode is on, cutting planes 
	will be redrawn if there are any changes to the geometry.
	\param onoff Pass true to turn on cutting plane regeneration or false to turn it off. 
	*/		
	virtual void SetCuttingPlaneRegenerationMode(bool onoff) { m_bCuttingPlaneRegeneration = onoff; }

	/*! 
		\return True if the shadows will be generated in the scene or false if they will not be redrawn after an update to the
		geometry.*/		
	bool GetShadowRegenerationMode() { return m_bShadowRegeneration; }

	/*! Use this method to indicate if the camera has changed.
	\param val Pass true to indicate the camera has changed or false if the camera has not changed.
	*/		
	virtual void SetCameraChanged( bool val=true ) { m_bCameraChanged = val; if(val) ResetIdleTime(); }
	
	/*! \return True if camera changed between updates. */	
	virtual bool GetCameraChanged() { return m_bCameraChanged; }

	/*! Use this method to control the direction of a zoom resulting from a mouse wheel event
	\param val Pass true if you want to invert the zoom direction resulting from a camera wheel event.
	*/		
	virtual void SetInvertMouseWheelZoom( bool val=true ) { m_bInvertMouseWheelZoom = val;}

	/*! \return True if we are inverting the default direction of the camera zoom which results from a mouse wheel event. */	
	virtual bool GetInvertMouseWheelZoom() { return m_bInvertMouseWheelZoom; }


	/*! The method sets operator for axis based camera manipulation.
		\param op A pointer the operator to replace existing operator with.
	*/	
	virtual void SetAxisManipulateOperator(HOpCameraManipulate *op);

	/*! This method enables or disables \ref Set_Heuristics_visual_selection "visibility selection" mode.
		\param onoff  Pass true for object visibility is taken into account or false for object visibility 
		not taken into account. 
		*/		
	virtual void SetVisibilitySelectionMode(bool onoff);
 
	/*! \return True if the visibility selection is enabled. */	
	virtual bool GetVisibilitySelectionMode();

	/*! This method sets the max number of items that each selection will hit.
		\param limit  Pass in NO_RELATED_SELECTION_LIMIT for no related selection limit.*/		
	virtual void SetRelatedSelectionLimit(int limit);
 
	/*! \return The current related selection limit on the view. */	
	virtual int GetRelatedSelectionLimit();

	/*! Use this method to sets the \ref Set_Heuristics_detail_selection "detail selection" mode.  If it is on, selection events honor the exact shape of 
	the glyphs of a curve, edge or polyline. This means that both the pattern and the 
	weight of a line/edge will affect its selectability.
		\param onoff  Pass true if edge and line weight/pattern should be taken into account or false if they shouldn't be.*/	
	virtual void SetDetailSelection(bool onoff);

	/*! \return True if the \ref Set_Heuristics_detail_selection "detail selection" heuristic is on or false if it is off.*/	
	virtual bool GetDetailSelectionMode();

	/*! Use this method to set the \ref Set_Heuristics_selection_culling "selection culling" mode to "maximum extents".  If it is on, 
	selected geometry will be culled in the same manner as unselected geometry.
	\param onoff  Pass true to cull selected geometry in the same manner as unselected geometry, or false for selected geometry not to be culled.*/
	virtual void SetRespectSelectionCulling(bool onoff);
	/*! \return True if the \ref Set_Heuristics_selection_culling "selection culling" mode is set to "maximum extents" or false otherwise.*/
	virtual bool GetRespectSelectionCulling();

	/*! This is a wrapper around #HC_Filter_Circumcuboid that sends a pre and post filter event to allow the application to
		manipulate filter results.
		\param segment The segment to start the bounding calculation.
		\param defaults The default visibility settings.
		\param bottomfrontleft Returns a point describing one end of the bounding circumcuboid.
		\param topbackright Returns a point describing the other end of the bounding circumcuboid.
	*/		
	virtual void SmartFilterCircumcuboid(const char *segment, const char *defaults, HPoint *bottomfrontleft, HPoint *topbackright);
	
	/*! This method creates handles for the specified object.
	  \param view A pointer to the HBaseView object.
	  \param key The key of the object to set up the handles for.
	  \param complex Pass true to create complex handles or false to create simple ones.
	  \return An event code that states whether the event was process.
	*/
	int SetupHandlesEvent(HBaseView *view, HC_KEY key, bool complex);

	/*! \return   True if the scene has segments grouped as object and/or has handles in it. */	
	virtual bool GetHasObjects() { return m_bHasObjects; }
	
	/*! This method tells the view if there are objects in the scene.
	\param hasobjects  Pass true to indicate that the scene has segments that are grouped as objects and/or has handles. 
	*/	
	virtual void SetHasObjects(bool hasobjects);
	
	
	/*! This method sets the handle operator for this view.
	  \param op A pointer to handle operator you want associated with this view.
	*/
	virtual void SetHandleOperator(HBaseOperator *op) { m_pHandleOperator = op; }

	/*! 
	  \return A pointer to the handle operator for this view.
	*/
	virtual HBaseOperator *GetHandleOperator() { return m_pHandleOperator; }	

	/*! \return The distance in centimeters in which HOOPS will look to find an object that is eligible 
	for selection.  The nearest object within proximity will be selected.
	*/	
	virtual float GetDefaultSelectionProximity() { return m_DefaultSelectionProximity; }
	/*! Use this method to control how far away your cursor can be from an object on the screen 
	and still have it select the object. Within that radius, the nearest object is selected. 
	\param selectionproximity the range in centimeters for selection proximity */	
	virtual void SetDefaultSelectionProximity(float selectionproximity);
	
	/*!
		\return True if shadows will be drawn during object manipuation or false if they won't be. */		
	bool GetAllowInteractiveShadows(){
		return m_bAllowInteractiveShadows;
	}

	/*! Use this method to enable or disable shadows during the object manipulation.  If you pass true, then 
	shadows will not be drawn while an object is translated, rotated, etc...
		\param onOff Pass true to allow shadows to be drawn while objects are manipulated or false for no shadows 
		to be rendered.
	*/
	void SetAllowInteractiveShadows(bool onoff) {
		m_bAllowInteractiveShadows = onoff;
	}

	/*!
		\return The current shadow rendering mode. */		
	HShadowRenderingMode GetShadowRenderingMode();

	/*! This method sets the shadow rendering mode. The shadow render mode determines if shadows are rendered either by 
	hardware or software.
		\param mode The shadow rendering mode you want HOOPS to use.
	*/
	void SetShadowRenderingMode(HShadowRenderingMode mode);

	/*! 
		\return True if cut geometry will be rendered during object manipulation or false if it will not be. */		
	bool GetAllowInteractiveCutGeometry() { return m_bAllowInteractiveCutGeometry; }

	/*! Use this method to enable or disable the rendering of cut geometry during object manipulation.  If you pass 
	false, cut geometry will not be drawn when an object is translated, rotated, etc...
	\param onOff Pass true to allow the rendering of cut geometry during object manipulation or false to not draw it.
	*/
	void SetAllowInteractiveCutGeometry(bool onoff){
		m_bAllowInteractiveCutGeometry = onoff;
	}

	/*! 
		\return The value of the Cut Geometry Visibility flag.*/		
	HCutGeometryVisibility GetCutGeometryVisibility();

	/*! Use this method to set the cut geometry visibility.
		\param visibility The cut geometry visibility you want to set for the scene.
	*/
	void SetCutGeometryVisibility(HCutGeometryVisibility visibility);
	

	/*! 
		\return A pointer to the window handle for this view. */		
	void *			GetWindowHandle() { return m_pWindowHandle; }

	/*!  
		\return True if the initial camera has been set or false if it hasn't been set.
		*/		
	bool			GetInitialCameraSet() { return m_bInitialCameraSet; }
	


	/*! The event handler that gets triggered when an object is moved usually as a result of an animation.
		\param view A pointer to HBaseView object.
		\param key The key of the moved object.
		\param pos The position of moved object which is not always available.
		\return Whether the event passed or consumed.
	*/
	int ObjectMovedEvent(HBaseView *view, HC_KEY key, HPoint *pos);

	/*! This method enables or disables the automatic camera movement recording.
		\param rc Pass true to enable camera recording or false to disable it.
	*/
	void SetRecordCamera(bool rc);

	/*! \return True if the camera is currently recording or false if it isn't recording.*/	
	bool GetRecordMode() { return m_bRecordCamera; }
	
	/*! This allows the injection of a synthetic event. 
		\param hevent The event to inject
		*/
	void InjectEvent(HEventInfo const & hevent);


	/*! This method enables the forced fast hidden line mode. If this mode is on, all hidden line rendering will be done 
		using the "fast hidden line" mode even if analytic hidden line is requested.
		\param force On/Off Pass true to turn on force all hidden line rendering to fast hidden line.
		*/
	void SetForceFastHiddenLine(bool force) { m_bForceFastHiddenLine = force; }

	/*! \return True if force fast hidden line mode is turned on or false if it is off. */	
	bool	GetForceFastHiddenLine() { return m_bForceFastHiddenLine; }

	/*!  This method sets how \ref Set_Rendering_Options_transparency "transparency" should be rendered in the view.
		\param text A text string that describes how transparency should be rendered as describe in 
		\ref Set_Rendering_Options_transparency "HC_Set_Rendering_Options".
		\param fast_z_sort If the sorting algorithm is z-sort only, this controls whether to use the fastest or nicest option.
	*/
	void SetTransparency(char const * text, bool fast_z_sort=false);

	/*!  This method enables or disables \ref Set_Rendering_Options_anti_alias "anti-aliasing" for lines by setting the appropriate 
	rendering option in the view segment.
		\param onoff Line Anti-aliasing on/off
	*/
	void SetLineAntialiasing( bool onoff );

	/*! \return True if \ref Set_Rendering_Options_anti_alias "anti-aliasing" for lines is turned on for the rendering options.*/	
	bool GetLineAntialiasing() { return m_bLineAntialiasing; }

	/*!  This method enables or disables \ref Set_Rendering_Options_anti_alias "anti-aliasing" for text by setting the appropriate rendering option in the view segment.
		\param onoff text Anti-aliasing on/off
	*/
	void SetTextAntialiasing( bool onoff );

	/*! This method enables of disables Depth Of Field
		\param onoff whether or not to use depth of field
		\param strength multiplier of the default strength
	*/
	void SetDepthOfField(bool onoff, float strength = 1.0f);

	/*! This method sets the focal range for the Depth Of Field option
		\param near the near plane distance from the camera
		\param far the far plane distance from the camera
	*/
	void SetDepthOfFieldRange(float near=0.0f, float far=0.0f);

	/*! \return True if \ref Set_Rendering_Options_anti_alias "anti-aliasing" for text is enabled and false if otherwise. */	
	bool GetTextAntialiasing() { return m_bTextAntialiasing; }

	/*!  This method sets the default viewpoint or quadrant for ISO View.
		\param vm The default ISO View Setting.
	*/
	void SetIsoDefault(ViewMode vm) { m_IsoDefault = vm; }\
	
	/*! \return The current ISO View setting. */
	ViewMode GetIsoDefault() { return m_IsoDefault; }

	/*! This method sets the culling threshold.  When objects are smaller than 
	the pixel diameter given by the threshold, they will be culled from the scene as
	described in the \ref Set_Heuristics_culling "maximum extent culling" option in 
	#HC_Set_Heuristics.
		\param cullingthreshold The culling threshold value given in pixels.
	*/
	void SetCullingThreshold(int cullingthreshold);

	/*! \return The current culling threshold value which is the pixel diameter used to determine
	if objects will be culled from a scene.  If the object is smaller than the pixel diameter,
	it will be culled.*/	
	int GetCullingThreshold();

	/*! This method sets the hard extent cutoff.  When objects are smaller than 
	the pixel diameter given by the threshold, they will be culled from the scene as
	described in the \ref Set_Heuristics_culling "hard extent" option in 
	#HC_Set_Heuristics.
		\param cutoff The hard extent value given in pixels.
	*/
	void SetHardExtent(int cutoff);

	/*! \return The current hard extent cutoff value which is the pixel diameter used to determine
	if objects will be culled from a scene.  If the object is smaller than the pixel diameter,
	it will be culled.*/	
	int GetHardExtent();

	/*!  This method activates the shell selection mode. If this mode is active, data structures 
	related to shell selection are cached which can greatly improve performance for subsequent 
	shell selection calls.
	*/
	void BeginShellSelection();
	
	/*!  This method ends the shell selection mode.  
	*/
	void EndShellSelection();

	/*! \return Highlight selection set which is used for mouse-over highlighting.*/	
	HSelectionSet * GetHighlightSelection() { return m_pHighlight; }

	/*!  This method defines the three vectors which represent the front-back, right-left and top-bottom camera axis for the current model. 
		 These vectors are used in calls to SetViewMode to determine the correct viewing position for the standard- as well 
		 as the isometric views. 
		 \param front_axis A pointer to the front - back axis.		 
		 \param top_axis A pointer to the top - bottom axis.
	*/
	void SetViewAxis(HVector *front_axis, HVector *top_axis) { m_FrontAxis = front_axis; m_TopAxis = top_axis; }
	/*! This method returns the view axis vectors. 
		 \param front_axis Returns the front - back axis.	
		 \param top_axis Returns the top - bottom axis.
	*/
	void GetViewAxis(HVector *front_axis, HVector *top_axis) { *front_axis = m_FrontAxis; *top_axis = m_TopAxis; }
	
	/*!  This method sets the \ref Set_Rendering_Options_hidden_surface_removal_algorithm "hidden surface removal algorithm" rendering option to the 
		 value that is preferred by the current driver type. 
	*/
	void SetDefaultHSRA();


	/*!This method lets you set the ceiling on the time it takes for an update to be performed.
		\param mut The upper limit on the duration of a update in seconds.
	*/
	void SetMaximumUpdateTime(float mut) { m_MaximumUpdateTime = mut; }
	
	/*! This method shows the ceiling on the time it takes for an update to be performed.
		\return The upper limit on the duration of a update in seconds.
	*/
	float GetMaximumUpdateTime() { return m_MaximumUpdateTime; }

	/*! 
		\return The selection highlight mode
	*/
	virtual const HSelectionHighlightMode GetHighlightMode() { return m_HighlightMode; }

	/*!
		This method sets the selection highlight mode.
		\param m The new highlight mode as described by #HSelectionHighlightMode.
	*/
	virtual void SetHighlightMode(HSelectionHighlightMode m);

	/*! This method controls whether or not quick moves (reference) highlighting will be used in both of the selection sets maintained by the HBaseView.
		\param use_ref if true, quickmoves highlighting will be used, otherwise conditional highlighting will be.
	*/
	virtual void SetReferenceSelection(bool use_ref);
	
	/*!
		This function indicates whether a key is currently pressed.
		\param key the character in question.
		\param flags additional flags.
		\return true if key is depressed, false otherwise.
	*/
	bool GetKeyState(unsigned int key, int &flags);

	/*! This function resets the current key state. */
	void ResetKeyState();

	/*!  This method controls whether fast fit world will be used.  Note: a fast fit world may be looser.
		\param fw if true fast fit world will be used.
	*/
	void SetFastFitWorld(bool fw) { m_bFastFitWorld = fw;	 }

	/*!  This method controls whether fit world will acknowledge sub-entity visibilities. This will only work if fast fit world is disabled.
		\param fw if true detailed fit world will be used.
	*/
	void SetDetailedFitWorld(bool fw) { m_bDetailedFitWorld = fw; }

	/*!  This method controls whether fast fit world will be used.  Note: a fast fit world may be looser.
	\param fw if true fast fit world will be used.
	*/
	bool GetFastFitWorld() { return m_bFastFitWorld; }

	/*!  This method controls whether fit world will acknowledge sub-entity visibilities. This will only work if fast fit world is disabled.
	\param fw if true detailed fit world will be used.
	*/
	bool GetDetailedFitWorld() { return m_bDetailedFitWorld; }

	/*!
		This function retrieves the scene extents.
		\param min the minimum values for the scene extents.  Passed by reference.  Returned to user.
		\param max the maximum values for the scene extents.  Passed by reference.  Returned to user.
	*/
	void GetSceneExtents(HPoint &min, HPoint &max);	

	/*!

	*/
	void SetLightScaling(float sv);

	void SetXYSizeOverride(int x,int y) { m_XSize_override = x, m_YSize_override = y; }


	/*! \return The window width in pixels.  If zero is returned, an update needs to be done.*/
	int GetXSize() {return m_XSize;}

	/*! \return The window height in pixels.  If zero is returned, an update needs to be done.*/
	int GetYSize() {return m_YSize;}

	/*! \return The selection level currently set on the driver.*/
	HViewSelectionLevel GetViewSelectionLevel();

	/*! 
		Controls the behavior of Compute_Selection calls performed on this driver.
		If level is set to HSelectionLevelEntity, the key of each selected piece geometry will 
		be returned.  If it is set to HSelectionLevelSegment, only the containing segment keys
		will be returned once, even if several geometries are selected in a given segment.  
		HSelectionLevelSegmentTree is not currently supported.
		\param level The new selection level to be set on the driver.
	*/
	void  SetViewSelectionLevel(HViewSelectionLevel level);

	/*!
		Enables or disables the target framerate.  This function is deprecated.
		Use SetFramerateMode() instead.
		\param enable Whether to enable or disable the target framerate.
		\param target The desired target framerate.
		\param threshold The minimum size of geometry to be drawn.
		\param steps The number of steps to reduce the threshold.
	*/
	void EnableTargetFrameRate(bool enable=true, int target=100, int threshold=10, int steps=5);

	/*!
		If a framerate is used, this returns the framerate out parameter.
		Otherwise this returns -1.
	*/
	float GetFramerate();

	/*! Returns the framerate mode */
	FramerateMode GetFramerateMode();

	/*! Returns the framerate culling threshold */
	int GetFramerateCullingThreshold();

	/*! 
		Returns the number of steps to reduce the culling threshold if applicable.
		Otherwise this returns -1.
	*/
	int GetFramerateSteps();

	/*! Returns true if and only if LODs are allowed in the framerate mode */
	bool IsFramerateLodEnabled();

	/*!
		This methods lets you set the ::FramerateMode which can be off, target or fixed.  The default is 
		off.  The fixed framerate mode guarantees that the specified framerate will be met while 
		the target framerate mode uses constant framerate logic to ensure that the actual framerate 
		is always within range of the specified framerate while still ensuring a smooth non-flickering 
		rendered scene.
		\param mode Framerate mode (off, target, fixed)
		\param framerate The desired framerate (in seconds).
		\param cullingthreshold The minimum size of geometry to be drawn.
		\param enablelods Allow lods in framerate (only applies if FramerateMode is FramerateTarget)
		\param steps The number of steps to reduce the threshold (only applies if FramerateMode is FramerateTarget)
		\param cutoff The pixel size to trigger culling
	*/
	void SetFramerateMode(FramerateMode mode, float framerate = 0.1f, int cullingthreshold = 150, 
		bool enableLods = false, int steps = 20, int cutoff = 1);
 
	/*! return The current state of the constant framerate setting */
	bool IsFrameRateEnabled();
	
	/*! \return true if this is the active view, false otherwise. */
	bool GetViewActive();

	/*! This method frees the SimpList and all members. */
	void cleanupSimpList();

	/*!  This method enables or disables annotation resizing. It must be on for annotations to work.
		\param value annotation resizing on/off
	*/	
	void SetAnnotationResize(bool value);

	/*! \return True if \ref annotation resizing is enabled and false if otherwise. */	
	bool GetAnnotationResize() { return m_AnnotationCookie==0?false:true; }

	/*! \return true if the first update has occurred, false otherwise.  */
	bool GetFirstUpdate() { return m_bFirstUpdate; }

	/*! This function controls whether manipulator handles will be created when geometry is double clicked. */
	void SetDisplayHandlesOnDblClk(bool dh) { m_bDisplayHandlesOnDblClk = dh; }	

	/*! \return true if there is an initial view on the scene. */
	bool HasInitialView()	{ return m_has_initial_view; }

	/*! Is there an initial view on the scene?  Used primarily for hsf streaming. */
	void HasInitialView(bool does_it)	{ m_has_initial_view=does_it; }

	/*! Perform a thorough and full update. */
	void ExhaustiveUpdate();

	/*! Used to indicate whether an update is currently in progress. */
	bool GetUpdateNotFinished() { return m_bUpdateNotFinished; }

	/*! Used to indicate whether an update is currently in progress. */
	void SetUpdateNotFinished(bool NotFinished) { m_bUpdateNotFinished  = NotFinished; }

	/* This method sets the mousewheel sensitivity which is used to control the apparent sensitivity of the mousewheel for MVO operators.
	   \param sensitivity value is strictly positive float. */
	void SetMouseWheelSensitivity(float sensitivity) {
		if (sensitivity > 0)
			m_MouseWheelSensitivity = sensitivity;
	}
	/* Use this in an operator to modify the return value of HEventInfo::GetMouseWheelDelta()
	   \return value of the operator mousewheel sensitivity, a strictly positive value. */
	float GetMouseWheelSensitivity() { return m_MouseWheelSensitivity; }

	/*! Controls the state of splat rendering of markers. */
	void SetSplatRendering(bool onoff);

	/*! Queries the state of splat rendering of markers. */
	bool GetSplatRendering() { return m_bSplatRendering; };

	/*! This method sets the size of marker splats in wsu. */
	void SetSplatSize(int splat_size);

	/*! \return The size of marker splats in wsu. */
	int GetSplatSize() { return m_SplatSize; };

	/*! This method sets the symbol used for marker splats. */
	void SetSplatSymbol(SplatSymbol symbol);

	/*! \return The symbol used for marker splats. */
	SplatSymbol GetSplatSymbol() {return m_SplatSymbol;};
	
	
	/*! This method is used for setting the background color used for fake hidden-line drawing.
		\param paper_color An RGB triplet where each value is in [0.0, 1.0].
	*/
	void UpdateFakeHLRColor(HPoint paper_color);

	/*! Controls whether hardware acceleration will be used for anti-aliasing. */
	void SetHardwareAntialiasing(int antialiasinglevel) { m_HardwareAntialiasing = antialiasinglevel; }

	/*! Queries whether hardware acceleration will be used for anti-aliasing. */
	int GetHardwareAntialiasing() { return m_HardwareAntialiasing; }

	/*! This function can be used to force an update on a given view.
		\param view_pointer a pointer to an HBaseView object.
	*/	
	static void StaticForceUpdateRequest(void * view_pointer);

	/*
		This method can be used to visualize the internal bounding stored on segments 
		and some types of geometry.   The bounding is drawn outside of the model so it does not
		interfere with static trees.  It is semi-transparent with a color that indicates the 
		bounding density: blue is very dense, red is sparse, green is somewhere in between.
		\param draw_something If this value is false, any existing bounding geometry will simply 
			be discarded.  If it is true, a new bounding geometry will also be generated.
		\param key The key of the segment or geometry whose bounding should be visualized.
		\param incl_count The number of keys in the incl_path array.
		\param incl_path This array of keys should be structured for a PShow.  That is, the leaf 
			as the first entry followed by a series of include keys mapping a path to the view.
	*/
	void VisualizeBounding(bool draw_something, HC_KEY key=INVALID_KEY, int incl_count=0, HC_KEY * incl_path=0);

	
	/*
		Retrieves the color set by an earlier call to SetFakeHLRColor.
		\return the color as an rgb triplet with values from 0.0 to 1.0.
	*/
	HPoint GetFakeHLRColor();
	
	/*
		Sets the color to be used in fake hidden-line render mode.
		\param color An rgb triplet with values from 0.0 to 1.0.
	*/
	void SetFakeHLRColor(HPoint color);

	/*
		Orients the camera to point at selected geometry and sets depth-of-field to keep it in focus.
		\param onoff passing true will focus on the selected geometry, passing false will remove an earlier depth-of-field setting.
	*/
	void FocusOnSelection(bool onoff = true);

	/*
		\return true if the view is currently focused on a selection via a previous call to FocusOnSelection(), false otherwise.
	*/
	bool GetFocusOnSelection() { return m_FocusedOnSelection; }
	
	/*! \return A pointer to the HTouchManager object which handles multi-touch events. */
	HTouchManager *GetTouchManager() { return m_TouchManager; }

protected:
	HC_KEY			m_FakeHLRStyle;				/*!< The key of the fake hidden line style, if it is currently applied (should be invalid otherwise). */
	HC_KEY			m_FakeHLRSource;			/*!< The key of the segment containing fake hidden line settings. */
	bool			m_bCPLineVisibility;		/*!< This member indicates the visibility setting of cutting plane intersection lines . */
	bool			m_bCPFaceVisibility;		/*!< This member indicates the visibility setting of cutting plane intersection faces.  */
	int				m_CPMode;					/*!< Indicates where, for purposes of generating cutting plane intersection faces, solid bodies begin and end.  See HIMCG_Body_Type in HImCuttingGeometry.h. */ 
	bool			m_bCuttingPlanesExist;		/*!< Existence of cutting plane geometry. */
	bool			m_bCuttingPlanesVisibility;	/*!< Visibility of transparent face representing the cutting plane. */
	bool			m_bSpritingAllowed;			/*!< Allow/disallow spriting in operators. */
	bool			m_bAxisTriadTransparencyAllowed; /*!< Allow/disallow portions of the axis triad to be alpha blended (regardless, window is still clear). */
	int				m_AxisWindowSize;			/*!< The width and height of the axis triad, given as a number of pixels. */

	bool			m_bForceFastHiddenLine;		/*!< If this member is set to true, it forces fast hidden line mode overriding other settings.  */
	bool			m_bLineAntialiasing;		/*!< Enables/Disables line anti-aliasing.  */
	bool			m_bTextAntialiasing;		/*!< Enables/Disables text anti-aliasing.  */
	bool			m_bBeginShellSelection;		/*!< Indicates if Shell Selection Mode is currently active.  */
	bool			m_bDisplayHandlesOnDblClk;
	bool			m_has_initial_view;
	
	HTouchManager   *m_TouchManager;			/*!< A pointer to the touch manager class. */

	int				m_HardwareAntialiasing;
	ViewMode		m_IsoDefault;				/*!< The default ISO View.  */
	HSelectionHighlightMode m_HighlightMode;		/*!< This member stores the current highlighting mode. */
	DisplayListType	m_DisplayListType;			/*!< They type of display lists used. */
	
	/*!	If true, cutting plane intersection geometry will be recomputed during an update; if false, such 
		geometry will not be recomputed. */
	bool			m_bGeometryChanged;
	HPoint			m_SceneBoundingMin;
	HPoint			m_SceneBoundingMax;
	bool			m_bSceneBoundingSet;
	bool			m_bFastFitWorld;
	bool			m_bDetailedFitWorld;
	HImCuttingGeometry * m_pHImCuttingGeometry; /*!< A pointer to the object containing the cutting geometry .*/
	HUtilityAntialiasing * m_pAntialiasing;		/*!< A pointer to the anti-aliasing utility class.*/
	HMarkupManager * m_pMarkupManager;			/*!< A pointer to the markup manager class. */
	AxisMode		m_bAxisMode;				/*!< Current axis display mode. */
	bool			m_bHideOverlappedText;		/*!< Informs the view if overlapped text should be filtered.  This causes a performance hit.*/	 
	bool			m_bNavCube;					/*!< Informs the view if the navcube is on and needs to be updated. */
	bool			m_bSmoothTransition;		/*!< Enables/Disables smooth transition for non-interactive view changes. */
	float			m_SmoothTransitionDuration;	/*!< Duration of Smooth Transition. */
	bool			m_bLightFollowsCamera;      /*!< Indicates whether the light should follow the camera around; default is true. */	
	int				m_WindowColorPercentage;	/*!< This value determines how far the gradiated background extends into the vertical direction. */
	HVector			m_FrontAxis;				/*!< Front-Back Axis Vector used in SetViewMode. */
	HVector			m_TopAxis;					/*!< Top-Bottom Axis Vector used in SetViewMode. */

	HBaseModel*		m_pModel;					/*!< Pointer to the HBaseModel object to be viewed. */
	HC_KEY			m_ViewKey;					/*!< Key of the HOOPS driver instance segment. */
	/*! Key of the segment associated with this view's scene.  This is a subsegment of the View segment. */
	HC_KEY			m_SceneKey;				
	/*! Key of the segment associated with this view's scene.  This is a subsegment of the View segment. */
	HC_KEY			m_OverwriteKey;				
	/*! Key of the segment which is used to draw geometry representing the bounding box of geometry being manipulated. */
	HC_KEY			m_BBoxGeometryKey;		
	/*! Key of the segment with window (screen) space attributes typically used for temporary geometry. */
	HC_KEY			m_WindowspaceKey;		
	/*! Key of the segment with quickmoves attributes typically used for temporary geometry. */
	HC_KEY			m_ConstructionKey;		
	/*! Key of the node of the segment tree which contains the the cutting planes and the quad grids used to visually represent them. */
	HC_KEY			m_CuttingPlanesKey;		
	/*! Key of the node of the segment which contains the cutting plane intersection geometry. */
	HC_KEY			m_CuttingPlaneGeomKey;	
	/*! Key of the node of the segment which contains polylines of intersection (see HOpObjectClash operator). */
	HC_KEY			m_IntersectPolylinesKey;	
	/*! Key of the segment which contains the axis triad. */
	HC_KEY			m_AxisTriadKey;	
	/*! Key of the segment which contains the file load progress bar. */
	HC_KEY			m_ProgressBarKey;	

 
	HC_KEY			m_InfoTextKey;	

	/*! This is the key which defines the link from the view segment to the HBaseModel segment. */
	HC_KEY			m_lIncludeLinkKey;
	HC_KEY			m_LightsKey;			/*!< The key of the HOOPS segment containing the lights for this view. */
	HC_KEY			m_SceneLight[MaxLights];		/*!< The key of the default distant light. */
	HRenderMode	    m_RenderMode;			/*!< The current render mode. */
	ViewMode	    m_ViewMode;				/*!< The current view mode. */
	HBaseOperator*	m_pOperator;			/*!< A pointer to the current operator of this view.*/
	HEventInfo*		m_pEvent;				/*!< A pointer to the current event of this view. */
	HSelectionSet*	m_pSelection;			/*!< A pointer to the selection object of this view. */
	HSelectionSet*	m_pHighlight;			/*!< A pointer to the highlight object of this view. */
	HConstantFrameRate *m_pConstantFrameRate;/*!< A pointer to the constant framerate object of this view. */
	HShadowMode		m_ShadowMode;			/*!< The current shadow mode. */
	HSharedKey *	m_pSharedKey;			/*!< A pointer to Shared Key class. */
	int				m_UpdateInterrupted;   /*!< The value is true if update has been interrupted by GUI event, and false if update was not interrupted. */
	int				m_InUpdateInternal;   /*!< The value is true if we are in UpdateInternal*/
	void			(* m_pfEventChecker)(HIC_Rendition const *nr); /*!< A pointer to an event checker function. */
	bool			(* m_pfKeyState)(unsigned int, int &); /*!< A pointer to a keyboard query  function .*/
	ViewUpdateMode	m_ViewUpdateMode;		/*!< The value of the update mode which can be continuous or on demand. */
	bool			m_bUpdateRequested;		/*!< If the value is true, an update was requested.  If the value is false, then no update was requested. */
	HEventManager *	m_pEventManager;		/*!< A pointer to the event manager. */
	HUndoManager *	m_pUndoManager;			/*!< A pointer to the undo manager. */
	HObjectManager *m_pObjectManager;		/*!< A pointer to the object manager. */
	class HDebugZBuffer *m_pDebugZBuffer;   /*!< A pointer to the object associated with the active z buffer visualization window (if any). */
	bool			m_bLButtonDown;			/*!< For internal use. */
	bool			m_bMouseMove;			/*!< For internal use. */
	HOpCameraManipulate *m_pManipulateOperator;	/*!< Operator Instance used in axis based camera manipulation. */
	bool			m_bShadowRegeneration;	/*!< Enables/Disables shadow regeneration. */
	bool			m_bShadowRegenerationToggle;	/*!< If enabled, shadow regeneration will be toggled back on during next update. */
	bool			m_bCuttingPlaneRegeneration; /*!< Enables/Disables Cutting Plane Regeneration. */
	bool			m_bCameraChanged;		/*!< The value is true if the camera changed between updates. */
	float			m_IdleTime;				/*!< Time since the last event. */
	bool			m_bFirstUpdate;         /*!< A boolean indicating if a call to Update() has occurred. */
	HBaseOperator * m_pHandleOperator;		/*!< A pointer to the handle operator. */
	bool			m_bAllowInteractiveShadows;	/*!< The flag to allow interactive shadow mode. */
	bool			m_bAllowInteractiveCutGeometry;	/*!< The flag to allow interactive cut geometry mode. */
	float			m_DefaultSelectionProximity; /*!< Selection Proximity for View. */
	float			m_MaximumUpdateTime;	/*!< Time beyond which updates should be cut off. */
	HCamera			m_InitialCamera;		/*!< The camera settings for the initial camera. */	
	bool			m_bInitialCameraSet;	/*!< A flag indicating if initial camera is set. */	
	bool			m_bRecordCamera;		/*!< Indicates if camera recording mode is active. */
	bool			m_bTransparentSelectionBox; /*!< Indicates whether selection by area should draw a transparent rectangle or just its outline. */
	bool			m_bInvertMouseWheelZoom;  /*!< A flag which indicate whether we should invert the zoom which results from a mouse wheel event. */
	unsigned long	m_AnnotationCookie;
	unsigned long	m_AxisResizeCookie;
	FramerateMode	m_FramerateMode;
	bool			m_bSplatRendering;
	int				m_SplatSize;
	SplatSymbol		m_SplatSymbol;
	bool			m_FocusedOnSelection;
	
	/*! This method computes the plane and light position for the given shadow. 
		The shadow plane has a normal along the view-up vector and just kisses 
		the objects in the scene at the bottom. The shadow light is placed a distance 
		"r", radius of circum cuboid of scene, from the shadow plane.
		\param shadow_plane Returns the HPlane (a,b,c,d) value for the plane computed. 
		\param light_pos    Returns the HPoint (x,y,z) value representing light position for shadow. */
	void ComputeShadowParams(HPlane& shadow_plane, HPoint& light_pos);

	/*! This method adjusts the shadow plane so that it is consistent with the current plane orientation.  It also
	positions it so that it remains outside the bounding box of the model.
	\param shadow_plane Returns the HPlane (a,b,c,d) value for the plane computed. 
	\param light_pos    Returns the HPoint (x,y,z) value representing light position for shadow. 
	*/
	void AdjustShadowPlane(HPlane& shadow_plane, HPoint const & light_pos);

	/*! This method sets the plane, light position, color and filter for the shadow. 
		\param shadow_plane An HPlane (a,b,c,d) value representing the shadow plane.
		\param light_pos    An HPoint (x,y,z) value representing light position for shadow.
		\param color		An HPoint representing the rgb color value for the shadow where x is the r value, y is the g value and z is the b value.
		\param filter		A string value the amount of \ref Set_Rendering_Options_simple_shadow "blurring" for the shadow. */
	void SetShadowParams(const HPlane& shadow_plane, const HPoint& light_pos, 
						 const HPoint* color = 0, const char * filter = 0);

	/*! <b> Deprecated.</b> Please use #ComputeShadowParams() instead.
		Computes the plane and light position for the given shadow. 
		The shadow plane is one which has normal along view-up vector and just kisses 
		the objects in the scene at the bottom. The shadow light is placed a distance 
		"r" (radius of circumcuboid of scene) farther from the shadow plane
		\param shadowObj  The HShadow object representing the shadow */
	virtual void SetShadowParams(HShadow* shadowObj);

	/*! Special helper function to check the size of the view. 
		This function is intended primarily for internal use. */
	void RefreshViewSize();

	/*! This is a helper function that is called during the first call to Update().  It will check 
		the rendering capabilities of the device, and modify the rendering settings as necessary.
		For example, if the user had set HRenderShaded, but HW phong isn't available, then the mode
		will be reset to HRenderGouraud.
	*/
	void QueryDeviceCapabilities();

	/*!
		This method updates the scene.  The HOOPS segment tree is traversed and any changes in geometry or attributes are updated via
		a redraw.  Cutting plane geometry and the orientation of the axis display are recalculated if necessary.  If constant
		frame rate is enabled and activated by an operator, the detail level of the geometry is adjusted if necessary in order
		to maintain the target frame rate.
		\param antialias  If true, then an anti-aliased version of the scene will be drawn.
		\param force_update  If true, then an update will be forced.
	*/
	virtual void UpdateInternal(bool antialias = false, bool force_update = false);


	/*! 
		This method is used internally to place distant lights.  It must consider factors such as how many lights are expected 
		in the scene and whether those lights should follow the camera.
	*/
	virtual void SetupLights();
	
private:
	float	m_LastRecordTime;	/*!< Last recorded time  */

	char * m_sDriverPath;		// fully rooted pathname of driver instance
	char * m_sDriverType;		// driver type; e.g. msw, opengl, x11
	char * m_sAlias;	
	void * m_pWindowHandle;
	void * m_pColormap;
	void * m_pClipOverride;
	void * m_pWindowHandle2;
	struct vlist_s *m_SignalRequestsList;	/*!< a list of requests for notification when specific events occur.  This list stores "struct signal_request"'s, defined in HBaseView.cpp */
	HBaseViewEmitMessageFunction emit_message_function;
	void * emit_message_function_user_data;
	HTClient *	m_pUpdateTimerClient;
	bool	m_bPrevBackplaneCullMode;	// this caches backplane cull mode (which is switched off when capping planes are turned off with cutting planes on )
	bool	m_bHasObjects;
	float	m_fZoomLimit;
	int		m_SuppressUpdate;
	int		m_SuppressUpdateTick;
	bool	m_bDynamicHighlighting;
	int		m_XSize;
	int		m_YSize;
	int		m_XSize_override;
	int		m_YSize_override;
	float	m_LastMouseWheelTime;
	float	m_MouseWheelSensitivity;
	bool	m_KeyMap[256];
	int		m_KeyFlags;
	bool	m_bOwnDriver;
	bool	m_bUpdateNotFinished;
	int		m_NotFinishedCounter;
	HPoint	m_FakeHLRColor;
	HConstFRSimpType			**SimpList;
	int							simpListLength;
	int		m_OldCullingThreshold;
	float *m_pTempClipArray;
	ViewUpdateMode		m_OldViewUpdateMode;
	bool m_UseExplicitShadowLight;
	HPoint m_ExplicitShadowLightDirection;
	int m_LightCount;

	//no default constructor
	HBaseView() {;};

	//obsolete
	HBaseView(HBaseModel *model, char const * alias, char const * driver_type,char const * instance_name, long window_handle, long colormap) {
		UNREFERENCED(model);
		UNREFERENCED(alias);
		UNREFERENCED(driver_type);
		UNREFERENCED(instance_name);
		UNREFERENCED(window_handle);
		UNREFERENCED(colormap);
	}

	//obsolete
	void RebindView(HBaseModel *model, char const * alias, char const * driver_type,char const * instance_name, long window_handle, long colormap) {
		UNREFERENCED(model);
		UNREFERENCED(alias);
		UNREFERENCED(driver_type);
		UNREFERENCED(instance_name);
		UNREFERENCED(window_handle);
		UNREFERENCED(colormap);
	}

};


#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif
