// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef _HOBJECTS_H
#define _HOBJECTS_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"
#include "HEventListener.h"
#include "HEventManager.h"
#include "HBaseOperator.h"

class HBaseView;
class HTClient;
class HBaseOperator;
class HImagePanel;

enum HButtonState 
{
	ButtonDown,        
	ButtonPressed,        
	ButtonUp      
};

//! The HEdgeVertexSelectionObject class handles interactive "mouse-over" selection of triangles inside a shell.
/*!
    This class allows you to see the backsidge edges of a shell while highlighting the closest vertices during a 
	mouse-over.
    
*/
class MVO_API HEdgeVertexSelectionObject : public HObject, public HMouseListener
{
public:
	/*!
	Constructs an HEdgeVertexSelectionObject object and registers callback for mouse events.
 	  \param view A pointer to the HBaseView object.
 	*/

    HEdgeVertexSelectionObject(HBaseView *view);
    ~HEdgeVertexSelectionObject();

	/*!  
	  This is the mouse move handler.  As the mouse moves, this method identifies if the user has moved 
	  the cursor over a shell edge.  If the mouse cursor is over a selectable edge, it indentifies and marks the edge.
 	  \param hevent An HEventInfo object containing information about the current event
	  \return An #HOperatorReturn indicating the status of the event.
	*/
    virtual int OnMouseMove(HEventInfo &hevent);
	/*!  
	  This is the left button down handler.  This method records if the left mouse button was pressed while the shift and 
	  control keys were down and the mouse move method had found a selectable edge.  
 	  \param hevent An HEventInfo object containing information about the current event.
	  \return An #HOperatorReturn indicating the status of the event.
	*/    
    virtual int OnLButtonDown(HEventInfo &hevent);
	/*!  
	  This is the left button up handler.  If the user had pressed the left mouse button while the control and shift keys 
	  were down and the cursor was over a selectable edge, then geometry manipulator handles are created.
 	  \param hevent An HEventInfo object containing information about the current event.
	  \return An #HOperatorReturn indicating the status of the event.
	*/
    virtual int OnLButtonUp(HEventInfo &hevent);

	/*!  
	  This is a helper function for the mouse move handler. It allows for invisible edge display.
 	  \param view A pointer to the view object.
	  \param movekey The segment key to include.
	  \return An #HOperatorReturn indicating the status of the event.
 	*/
    void IncludeEntity(HBaseView *view, HC_KEY movekey);

	/*!  
  	  \return The name of the object which is 'ev_select'.
	*/
    static const char * GetStaticName();

	/*!  
  	  \return The name of the object which is 'ev_select'.
	*/
    const char * GetName();

     
protected:
    HPoint m_Point1;				/*!< The first edge point. */
    HPoint m_Point2;				/*!< The second edge point. */
    HC_KEY m_SegKey;				/*!< The key of the included segment. */
    HBaseView *m_pView;				/*!< A pointer to the HBaseView object. */
	

private:
    bool m_bFound;			
    bool m_blButtonDown;
 
};


//! The HImageRegion class represents selectable regions inside an image panel
class MVO_API HImageRegion
{
public:

	/*!
	Constructs an HImageRegion object.
  	  \param identifier Numeric identifier of image region
	  \param name Name of image region
 	*/
	HImageRegion(int identifier, char const * const name);
	~HImageRegion();

 	/*!  
	 This method determines if a region is selected given an x/y position and the associated region iage
	 \param x Mouse X Coordinate
	 \param y Mouse Y Coordinate
	 \param region_image_key HOOPS Key to image defining image panel regions
	 \param width Width of Image Panel
	 \param height Height of Image Panel
 	 \return True: Mouse is inside of region
  	*/
	bool IsSelected(float x, float y, HC_KEY region_image_key, int width, int height);

 	/*!  
	 This method returns the name of the image region
 	 \return Name of image region
  	*/
 	char const * GetName() { return m_Name; }
 	/*!  
	 This method returns the identifier of the image region
 	 \return Identifier of image region
  	*/
	int GetID() { return m_Id; }
private:
	bool	m_bHighlighted;
 	char *m_Name;
	int m_Id;
};
//! The HImagePanelManagerObject class manages multipe image panels
class MVO_API HImagePanelManagerObject : public HObject
{
public:
	 

	/*!
	Constructs an HImagePanelManagerObject object.
  	*/
    HImagePanelManagerObject();
    ~HImagePanelManagerObject();

	/*!  
  	  \return The name of the object which is 'imagepanel'.
	*/
    static const char * GetStaticName();

	/*!  
  	  \return The name of the object which is 'imagepanel'.
	*/
    const char * GetName();

	/*!  
		This method adds a new image panel to the manager class
 		\param ip Image Panel Identifier of new image region
   	*/
	void AddImagePanel(HImagePanel *ip);
 	/*!  
	 This method deletes an existing image panel object given its name
	 \param name Name of image panel 
	 \param doDelete True: Call destructor of image panel object
	 \param ip Pointer to Image Panel object
	 \return True: Found
  	*/
	bool DeleteImagePanel(char const * name, bool doDelete = true, HImagePanel **ip = 0);
 	/*!  
	 This method deletes an existing image panel object given a pointer to it
	 \param ip Pointer to Image Panel object
 	 \param doDelete True: Call destructor of image panel object
 	 \return True: Found
  	*/
	bool DeleteImagePanel(HImagePanel *ip, bool doDelete = true);
 	/*!  
	 This method retrieves a pointer to an image panel object given its name
	 \param name Name of image panel 
  	 \return Pointer to Image Panel
  	*/
	HImagePanel * GetImagePanelByName(char const * name);
    
protected:
	

	struct vlist_s*			m_ImagePanelList;		/*!< List of Image panels */
 
};


//! The HImagePanel class displays an image usually representing buttons which a user can highlight with a mouse click and which trigger user defined actions
/*!
	An image panel object consists of 3 HOOPS images which must be stored in  separate HMF files. One of them 
	represents the visual representaton of the unselected panel while the other represents the selected state. The third
	image defines regions inside the panel (based on the rgb values in the image) which trigger execution of a user supplied
	callback function.
	
*/ 
 
class MVO_API HImagePanel : public HBaseOperator
{
public:
	 

	/*!
	Constructs an HImagePanel object.
  	  \param view A pointer to the HBaseView object.
	  \param startkey Key of segment containing the image panel.
	  \param width Width of image panel
	  \param height Height of image panel
	  \param xpos Initial X Position of image panel
	  \param ypos Initial Y Position of image panel
	  \param name Name of image panel
 	*/
    HImagePanel(HBaseView *view, HC_KEY startkey, int width, int height, float xpos, float ypos, char const * const name);
    virtual ~HImagePanel();

 	/*!  
	 This method finds the region associated to a specific x/y position in the image
	 \param xpos Mouse X Position
	 \param ypos Mouse Y Position
	 \return Image Region
  	*/
	HImageRegion * FindSelectedRegion(float xpos, float ypos);

	/*!  
	  Mouse Move Handler
 	  \param hevent An HEventInfo object containing information about the current event
	*/
    virtual int OnLButtonDownAndMove(HEventInfo &hevent);
	/*!  
	  Left Button Down Handler
 	  \param hevent An HEventInfo object containing information about the current event
	*/    
    virtual int OnLButtonDown(HEventInfo &hevent);
	/*!  
	  Left Button Up Handler
 	  \param hevent An HEventInfo object containing information about the current event
	*/
    virtual int OnLButtonUp(HEventInfo &hevent);

 	/*!  
	 This method adds a new image region to the panel
 	 \param identifier Identifier of new image region
	 \param name  Name of new image region
  		*/
	void AddImageRegion(int identifier, char const * const name);
 	/*!  
	 This method returns the name of the image panel
 	 \return Name of image panel
  		*/
	char const * GetPanelName() { return m_Name; }
     
 	/*!  
	 This method associates a callback function to the image panel
	 \param image_action Function to call for image actions
	 \param data generic data passed to image panel callback function
 	*/
	void SetImageAction(int (* image_action)(HImageRegion *ii, void *data, HButtonState button_state), void *data)
	{
		m_ImageAction = image_action;
		m_ImageActionData = data;
	}
 	/*!  
	 Timer callback used to react to button presses
  	*/
	static bool ButtonTimerClbk(float request_time, float actual_time, void * user_data);

 	/*!  
	 This method associates a callback function to the image panel
	 \param image_action Function to call for image actions
	 \param data generic data passed to image panel callback function
 	*/
 	
 	/*!  
	 This method sets the 3 images used for display of the image panel and region selection
	 \param normal_image_path Path to HMF file containing the image which defines the unhighligted "look" of the image panel
	 \param highlighted_image_path Path to HMF file containing the image which defines the highlighted "look" of the image panel
	 \param region_image_path Path to HMF file containing the image which defines the selectable regions of the image panel
  	*/

	void SetImagesFromFile(char const * normal_image_path, char const * highlighted_image_path, char const * region_image_path);
	void SetImages(HC_KEY NormalImageKey, HC_KEY HighlightedImageKey, HC_KEY RegionImageKey);
	
 	void ZoomIn();
	void ZoomOut();

	void SetPosition(float x, float y, float z = 0.0f);

private:
	/*!  
	 This method finds a HOOPS image inside of an HMF file
	 \param path Path to HMF file
	 \param image_key Key of image in HMF file
 	*/
	void SetImageFromFile(char const * path, HC_KEY &image_key);
	/*!  
	 This method creates an image by composing the regular image of the image panel with the highlighted region
	 \param reg Pointer to ImageRegion object
  	*/
	void ComposeWorkerImage(HImageRegion *reg);
	HC_KEY	 m_Key;									/*!< Key to image panel segment. */
    HBaseView *m_pView;								/*!< A pointer to the HBaseView Object. */
	struct vlist_s*			m_ImageRegionList;		/*!< List of image regions */
	int m_width;									/*!< Width of Image Panel. */
	int m_height;									/*!< Height of Image Panel. */		

	int (* m_ImageAction)(HImageRegion *ii,void *data, HButtonState button_state);
	void * m_ImageActionData;
	HTClient*			m_pButtonTimer;
	HImageRegion *m_CurrentImage;
	HC_KEY m_NormalImageKey;
	HC_KEY m_HighlightedImageKey;
	HC_KEY m_RegionImageKey;
	HC_KEY m_WorkerImageKey;
    bool m_bFound;			
    bool m_blButtonDown;
 	HPoint m_ptLast;	
	char *m_Name;		
};
 
//! The HInfoWindowObject class displays is a floating window that can display any text information that you wish to show to the user.
class MVO_API HInfoWindowObject : public HObject
{
public:
	/*!
	Constructs an HInfoWindowObject object.
  	  \param view A pointer to the HBaseView object.
	  \param startposx The initial x cordinate of the text.
	  \param startposy The initial y coordinate of the text.
	  \param maxlines The maximum number of lines that the information window can display.
	\param displaylength The amount of time in seconds to display the text before it is erased if EraseAfterDisplay is true.
	*/
    HInfoWindowObject(HBaseView *view, float startposx = -0.95f, float startposy = 0.3, int maxlines = 20, int displaylength = 7);
    ~HInfoWindowObject();

	/*!  
  	  \return The name of the object which is 'info_window'.
	*/
    static const char *GetStaticName();

	/*!  
  	  \return The name of the object which is 'info_window'.
	*/
    const char *GetName();
	/*!This method adds a line of text to the information window.
	\param text A pointer to the text to be inserted into the information window.
	\param font A pointer to the font for the text.
	\param color A pointer to the color of the text.
	\param EraseAfterDisplay Pass true to erase after a limited amount of time or false to keep the text as it is.
	\param DisplayLength The amount of time in seconds to display the text before it is erased if EraseAfterDisplay is true.
	*/
	void AddLine(char const * text, char const * font = 0, char const * color = 0, bool EraseAfterDisplay = false, int DisplayLength = -1);
	/*!This method flushes the contents of the information window key.  Then it inserts the text in the memory into HOOPS and requests an update in the view.*/
	void UpdateInfoBox();
	/*!This method clears out all the text in the information window.*/
 	void Flush();

	/*!This method handles the window resize event.*/
 	static void WindowResizeCallback(int signal, void * signal_data, void * user_data);

protected:
     HBaseView *m_pView;				/*!< A pointer to the HBaseView Object. */
     HC_KEY m_InfoWindowKey;			/*!< The key to the information window. */
	char *		m_InfoText[25];			/*!< The text that will be displayed in the information window. */
	char *		m_InfoTextFont[25];		/*!< The font of the text in the information window.*/
	char *		m_InfoTextColor[25];	/*!< The color of the text in the information window.*/
	int m_LineNum;						/*!< The number of lines of text.*/
	int m_LineMax;						/*!< The maximum number of lines of text.*/
	float m_StartPosX;					/*!< The initial x position for the text.*/ 
	float m_StartPosY;					/*!< The initial y position for the text.*/
	HTClient*			m_InfoWindowTimer; /*!< The timer client.*/
	/*! The callback for the timer client.
	\param request_time The time the callback requested to be called.
	\param actual_time The actual time the callback was called.
	\param user_data Additional information for time events.
	*/
    static bool InfowWindowTimerClbk(float request_time, float actual_time, void * user_data);
	/*! This method is a helper function for InfowWindowTimerClbk.  It updates the text and captures the 
	time stamp to compare elapsed time.*/
	void DoTimer();	
	/*! <b>This is for internal use only.</b>*/
 	void FlushInternal();

	float m_TimeStamp;			/*!< The time in which the last timer callback was called.*/
	float m_transmission;		
	bool m_bEraseAfterDisplay;	/*!< The value is true if the window will erase the text after an elapsed display time.*/
	int m_TempDisplayLength;	/*!< The temporary display time in seconds for the text.*/
	int m_DefaultDisplayLength; /*!< The default display time in seconds for the text.*/

};


//! The HLightsObject class encapsulates interactive light objects.
/*!
    This class creates 3d objects representing the various HOOPS light types which can be 
    repositioned and rotate interactively using the handle functionality.
    
*/ 
class MVO_API HLightsObject : public HObject, public HObjectManipulationListener
{
public:
	/*!
	Constructs an HLightsObject object.
  	  \param view A pointer to the HBaseView object.
	*/
    HLightsObject(HBaseView *view);
    ~HLightsObject();
	/*!  
  	  \return The name of the object which is 'light'.
	*/
    static const char * GetStaticName();
	
    /*!  
  	  \return The name of the object which is 'light'.
	*/
    const char * GetName();

	/*!  
	 This method creates geometry handles on the lights object.
 	  \param view A pointer to the HBaseView object.
	  \param key The key of the current object.
	  \param complex Pass true to create complex handles or false to create simple handles.
	  \return An #HOperatorReturn indicating the status of the event.
 		*/
    int SetupHandlesEvent(HBaseView *view, HC_KEY key, bool complex);

	/*!  
	 This method insert a local light at the given position in the object segment.  The local light is an infinitesimally 
	 small point that radiates light in all direction as described in #HC_Insert_Local_Light.  Geometry-wise, the local light 
	 is represented by a sphere in the scene.
 	  \param view A pointer to the HBaseView object.
	  \param pos A 3d world position to insert light into.
  	*/
    static void InsertLocalLight(HBaseView *view, HPoint &pos);
	/*!  
	 This method inserts a distant light at the given position in the object segment.  As described in #HC_Insert_Distant_Light, 
	 the distant light represents a directional light source that is so far away that the rays of light are parallel when 
	 it hits the object.  Note that it illuminates all surfaces in a scene with the same intensity. Geometry-wise, the 
	 distant light is represented by an arrow in the scene.
 	  \param view A pointer to the HBaseView object.
	  \param pos A 3d world position to insert light into.
  	*/
    static void InsertDistantLight(HBaseView *view, HPoint &pos);
	/*!  
	 This method inserts a spot light at given position in the object segment.  The spot light is the most common general use 
	 light used in HOOPS.  As described in #HC_Insert_Spot_Light, it can not only behave like a sharp-edged spotlight, but 
	 also like a variable-focus flashlight, or a fuzzy-edged floodlight. These characteristics are determined from the 
	 light's position, target, illumination cone, edge sharpness, and concentration parameters.  Geometry-wise, the spot light 
	 is represented as an arrow and sphere.
 	  \param view A pointer to the HBaseView object.
	  \param pos A 3d world position to insert light into.
  	*/     
    static void InsertSpotLight(HBaseView *view, HPoint &pos);    

	/*!  
	 This method activates/deactivates the light geometry.
 	  \param view A pointer to the HBaseView object.
	  \param onoff Pass true to activate light geometry or false to deactivate it.
  	*/     
    static void TurnLightsOnOff(HBaseView *view,bool onoff);

protected:	
	/*!  
	 This is a helper function the #InsertLocalLight, #InsertDistantLight and #InsertSpotLight methods.  It 
	 sets up the object segment.
 	  \param view A pointer to the HBaseView object.
	  \param pos A 3d world position to insert light into.
	  \param outerkey Returns the object key to insert the light into.
	  \param geometrykey Returns the key to insert the geometry into.
  	*/
    static void SetupLightSegment(HBaseView *view, HPoint &pos, HC_KEY &outerkey, HC_KEY &geometrykey);    
    
	/*!  
	This is a helper function to the #SetupLightSegment method. It create an HLightsObject and adds it to the #HObjectManager 
	as well as setting up default attributes for the segment.
	\param view A pointer to the HBaseView object.
	  \param pos A 3d world position to insert light into.
	  \param outerkey The Object key to insert light into.
	  \param geometrykey Returns the key to insert the geometry into.
  	*/
    static void SetupLightSegmentFromKey(HBaseView *view, HPoint &pos, HC_KEY &outerkey, HC_KEY &geometrykey);
 

    HBaseView *m_pView;			/*!< A pointer to the HBaseView Object. */
 
};


//! The HDraggerObject class encapsulates interactive dragger objects. 
/*!
    These interactive objects allow 3d movement in various dimensions and can be associated to other geometry.
    In the PartViewer, they are used to allow the interactive manipulation of animation paths.
*/ 

class MVO_API HDraggerObject : public HObject, public HObjectManipulationListener
{
public:
	/*!
	Constructs an HDraggerObject object.
  	  \param view A pointer to the HBaseView object.
	*/

    HDraggerObject(HBaseView *view = 0);
    ~HDraggerObject();
	/*!  
  	  \return The name of the object which is 'Dragger'.
	*/
    static const char * GetStaticName();
	/*!  
  	  \return The name of the object which is 'Dragger'.
	*/
    const char * GetName();
	/*!  
	This method is a helper function for #Insert.  It creates a dragger object with the given attributes 
	and adds it to the #HObjectManager.  It also inserts the dragger object into the given HOOPS key.
 	  \param view A pointer to the HBaseView object.
	  \param pos The 3d world position to insert dragger into.
	  \param rotmat The initial rotation of dragger object.
	  \param outerkey The object key to insert light into.
	  \param geometrykey The key to insert geometry into.
  	*/
    void SetupDraggerSegment(HBaseView *view, HPoint &pos, float rotmat[], HC_KEY &outerkey, HC_KEY &geometrykey);
	/*!  
	 This method inserts an HDraggerObject into an object segment.  It also associates it with a given keyframe.  When the 
	 object is moved using the dragger object, the position and orientation are recorded for the given keyframe.
 	  \param view A pointer to the HBaseView object.
	  \param pos The initial position of the dragger object.
	  \param rot The initial rotation of dragger object.
	  \param ani A pointer to the associated animation object.
	  \param rotani A pointer to the associated rotation animation.
	  \param kfr The associated keyframe.
	  \param allowpos Pass true to allow repositioning.
  	*/     

     HC_KEY Insert(HBaseView *view, HPoint &pos, float rotmat[], HBhvAnimation *ani, HBhvAnimation *rotani, int kfr, bool allowpos);

	/*!  
	 This method creates geometry handles on the dragger object.
 	  \param view A pointer to the HBaseView object.
	  \param key The key of the current object.
	  \param complex Pass true to create complex handles or false to create simple handles.
	  \return An #HOperatorReturn indicating the status of the event.
 	*/
    int SetupHandlesEvent(HBaseView *view, HC_KEY key, bool complex);
	/*!  
	  This handles the object moved event.  It calculates the position and rotation of the object and captures this information 
	  for the animation in the associated keyframe and plots the animation.
 	  \param view Pointer to View Object
	  \param key Key of Current Object
	  \param pos New Object Position
	  \return An #HOperatorReturn indicating the status of the event.
 	*/
    int ObjectMovedEvent(HBaseView *view, HC_KEY key, HPoint *pos);

protected:
    HBaseView *m_pView;		    	/*!< A pointer to the HBaseView object. */
 
};
 


#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif



