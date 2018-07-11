// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// HConstantFrameRate.h: interface for the HConstantFrameRate class.
//
//////////////////////////////////////////////////////////////////////

/*! \file HConstantFrameRate.h
	Defines datatypes associated with the HConstantFrameRate class  
*/

#ifndef HCONSTANTFRAMERATE_H
#define HCONSTANTFRAMERATE_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"
#include "hic_types.h"

class HTClient;

/*! \enum ConstFRType
	Defines the various simplification types
*/
enum ConstFRType
{
	ConstFRNoSimpType,			//!< No simplification.
	ConstFRNoSimpAAType,		//!< No simplification.  The scene is antialiased.
	ConstFRWireframeBBoxType,	//!< Wireframe bounding box similfication type.
	ConstFRSolidBBoxType,		//!< Solid bounding box simplification type.
	ConstFRWireframeType,		//!< Wireframe simplification type.
 	ConstFRLodClampType,		//!< LOD clamp type.  All objects share the same detail level.
	ConstFRLodThresholdType,	//!< LOD threshold type.  LOD level for each individual object based on size on screen.
 	ConstFRUserType,			//!< Placeholder type	
	ConstFRExtentType			//!< Maximum extent simplification type.  
};

/*! \enum HActivityType
	Defines the type of user interaction in the scene.  This affects the constant frame rate logic.
*/
enum HActivityType
{
	NoActivity,				//!< Indicates no activity that would affect the constant framerate logic.
	MovementActivity,		//!< Indicates movement activity in the scene.
	CuttingPlaneActivity,	//!< Indicates that a cutting plane is being manipulated.
	GeneralActivity			//!< Indicates that some activity is occuring that isn't movement or cutting plane related.
};


/*! \enum DetailChangeMode
	Detail change types which specify how changes in framerate are handled.
*/
enum DetailChangeMode
{
	NoDetailChange,				//!< No detail level change.
	AllowDetailChangeOnce,		//!< Only one detail change is allowed.
	AllowDetailChange			//!< Detail change is allowed based on target framerate.
};


class HConstantFrameRate;

class HBaseView;

/*! 
  This abstract base class is used to represent an element of the framerate simplication list.
  When the constant framerate functionality is enabled, the system uses a list of object simplification types to determine
  which steps to take in simplifying the scene to increase rendering speed.  
*/
class MVO_API HConstFRSimpType
{
public:
	int				m_SimpLevelMin;  /*!< Lowest simplification level which is specific to the simplification type. */
	int				m_SimpLevelMax;  /*!< Highest simplification level which is specific to the simplification type. */
	
 	/*! Default constructor that sets the minimum and maximum simplication levels to 0. */
	HConstFRSimpType()
	{
		m_SimpLevelMin=0;
		m_SimpLevelMax=0;
	}

	/*! The constructor that allows you the set the minimum and maximum simplification levels.
	\param min The minimum simplification level.
	\param max The maximum simplification level.
	*/
	HConstFRSimpType(int min, int max)
	{
		m_SimpLevelMin=min;
		m_SimpLevelMax=max;
	}
	virtual ~HConstFRSimpType() {}

	virtual void Reset(HConstantFrameRate *pCfr) {
		UNREFERENCED(pCfr);
	} 


	/*!
		Set rendering options based on simplification type.
		\param pCfr A Pointer to the ConstantFrameRate object.
		\param SimpLevel The simplification level for the current type.

	*/
 	virtual void Set(HConstantFrameRate *pCfr, int SimpLevel) = 0;
	/*!
		Update Display and additional framerate related settings.
		\param pCfr A Pointer to the ConstantFrameRate object.
		\param SimpLevel The simplification level for the current type.

	*/
 	virtual void Update(HConstantFrameRate *pCfr, int SimpLevel);

	/*! \return The simplification type.  */
	virtual ConstFRType GetType() = 0;		
	/*! \return The lowest simplification level.  */
	int GetMinSimpLevel() { return m_SimpLevelMin; };
	/*! \return The highest simplification level.  */
	int GetMaxSimpLevel() { return m_SimpLevelMax; };

	/*! \return A pointer to an array of HConstFRSimpType objects. */
	static HConstFRSimpType **CreateList(int size) { return new HConstFRSimpType *[size]; };
 
};


/*!
HFrExtent is a simplification type that tweaks the maximum extent suboption in ::Set_Heuristics.  The maximum extent
value is calculated based on the current simplification level and a multiplier.
*/

class  MVO_API HFrExtent : public HConstFRSimpType
{
public:
	/*! The default constructor.*/
	 HFrExtent() : HConstFRSimpType()  { m_Multiplier = 5; m_bUseLOD = false; }

	/*! The constructor that allows you the set the minimum and maximum simplification levels.
	\param min The minimum simplification level.
	\param max The maximum simplification level.
	*/
	 HFrExtent(int min, int max) : HConstFRSimpType(min,max) { m_Multiplier = 5; m_bUseLOD = false; }

	virtual ~HFrExtent() {}
	/*! This method sets the multiplier.  The multiplier is use to set the maximum extent for the culling option of ::Set_Heuristics.
	When the Set method is called, the maximum extent option will be set with a value calculated from the
	simplication level and the multiplier.
	\param mp The multiplier used in the calculation to set the maximum extent.
	*/
	void SetMultiplier(int mp) { m_Multiplier = mp; }
	/*!
		Set rendering options based on simplification type.
		\param pCfr A pointer to a HConstantFrameRate Object.
		\param SimpLevel Simplification level for the current type

	*/	
	void Set(HConstantFrameRate *pCfr, int SimpLevel);
	/*!
	This methods enables/disables the use of LOD options for rendering.
	\param ul Pass true if you want to set the LOD options to be set to %15 and 0% for threshold and
	the fallback to be set to coarsest.  Pass false if you don't want to use set any LOD options.
	*/
	void SetUseLod(bool ul) { m_bUseLOD = ul; }
	bool GetUseLod() const { return m_bUseLOD;}
	/*! \return The simplification type associated with this class.  */
	ConstFRType GetType() { return ConstFRExtentType; }
	/*!
		Create a new simplification type object. Avoids object ownership problems.
		\param min lowest simplification level
		\param max highest simplification level

	*/
	static HConstFRSimpType * Create(int min, int max) {return new HFrExtent(min, max); };   

protected:
	int m_Multiplier;
	bool m_bUseLOD;
};

/*! 
	HFrNoSimpAA is a no simplification type with antialiasing in the scene.
*/
class MVO_API HFrNoSimpAA : public HConstFRSimpType
{
public:
	/*! The default constructor. */
	HFrNoSimpAA() : HConstFRSimpType(0, 1) {} 

	/*! The constructor that allows you the set the minimum and maximum simplification levels.
	\param min The minimum simplification level.
	\param max The maximum simplification level.
	*/
	HFrNoSimpAA(int min, int max) : HConstFRSimpType(min,max) {};

	virtual ~HFrNoSimpAA() {}

	/*!
		When SimpLevel is 1, this method sets the rendering option for 
		\ref Set_Rendering_Options_transparency "transparency style" to screen door if the driver is not Direct3D.
		\param pCfr A pointer to a HConstantFrameRate Object.
		\param SimpLevel Simplification level for the current type

	*/
	void Set(HConstantFrameRate *pCfr, int SimpLevel);
	/*! \return The simplification type associated with this class.  */
	ConstFRType GetType() { return ConstFRNoSimpAAType; }
	/*!
		Update Display and additional framerate related settings including antialiasing.
		\param pCfr A pointer to a HConstantFrameRate Object.
		\param SimpLevel Simplification level for the current type

	*/
	void Update(HConstantFrameRate *pCfr, int SimpLevel);

	/*!
		Create a new simplification type object. Use this method to avoid object ownership problems.
		\param min lowest simplification level
		\param max highest simplification level

	*/
	static HConstFRSimpType * Create(int min, int max) {return new HFrNoSimpAA(min, max); };                                  

};

/*! 
	HFrNoSimp is a no simplification type.  Use this type as your base simplification type list 
	if there is no antialiasing in the scene.
*/
class MVO_API HFrNoSimp : public HConstFRSimpType
{
public:
	/*! The default construtor. */
	HFrNoSimp() : HConstFRSimpType(0, 1) {} 
	/*! The constructor that allows you the set the minimum and maximum simplification levels.
	\param min The minimum simplification level.
	\param max The maximum simplification level.
	*/
	HFrNoSimp(int min, int max) : HConstFRSimpType(min,max) {};

	virtual ~HFrNoSimp() {}

	/*!
		If SimpLevel is 1 or 0, this method turns off shadow and cutting plane regeneration.
		\param pCfr A pointer to a HConstantFrameRate Object.
		\param SimpLevel Simplification level for the current type

	*/
	void Set(HConstantFrameRate *pCfr, int SimpLevel);
	/*! \return The simplification type associated with this class.  */
	ConstFRType GetType() { return ConstFRNoSimpType; }


	/*!
		Create a new simplification type object. Use this method to avoid object ownership problems.
		\param min lowest simplification level
		\param max highest simplification level

	*/
	static HConstFRSimpType * Create(int min, int max) {return new HFrNoSimp(min, max); };   

 
};


/*! 
	LOD Simplification type using the CLAMP option.  The simplification level corresponds directly to the 
	clamp value rendering option.
*/
class MVO_API HFrLodClamp : public HConstFRSimpType
{
public:
	/*! The default constructor. */
	HFrLodClamp()
		: HConstFRSimpType() 
	{ m_bSegmentSwitch = false; }

	/*! The constructor that allows you the set the minimum and maximum simplification levels.
	\param min The minimum simplification level.
	\param max The maximum simplification level.
	*/
	HFrLodClamp(int min, int max)
		: HConstFRSimpType(min, max)
	{ m_bSegmentSwitch = false; }

	void Reset(HConstantFrameRate *pCfr);
	virtual ~HFrLodClamp() {}

	/*!
		This methods set the LOD clamp value to the SimpLevel.  It also sets the 
		\ref Set_Rendering_Options_tessellation "tessellation" option 
		for cylinders.
		\param pCfr A pointer to a HConstantFrameRate Object.
		\param SimpLevel Simplification level for the current type

	*/
	void Set(HConstantFrameRate *pCfr, int SimpLevel);
	/*! \return The simplification type associated with this class.  */
	ConstFRType GetType() { return ConstFRLodClampType; }
	/*!
		Create a new simplification type object. Use this method to avoid object ownership problems.
		\param min lowest simplification level
		\param max highest simplification level

	*/
	static HConstFRSimpType * Create(int min, int max) {return new HFrLodClamp(min, max); };    
	void SetSegmentSwitch(bool ssl) { m_bSegmentSwitch = ssl; }
	bool m_bSegmentSwitch;


};

/*! 
	LOD Simplification type using the THRESHOLD option. The simplification level describes a range from 1 to 200 for
	the threshold rendering option (in reverse order).
*/
class MVO_API HFrLodThreshold : public HConstFRSimpType
{
public:
	/*! The default constructor. */
	HFrLodThreshold() : HConstFRSimpType()  {}

	/*! The constructor that allows you the set the minimum and maximum simplification levels.
	\param min The minimum simplification level.
	\param max The maximum simplification level.
	*/
	HFrLodThreshold(int min, int max) : HConstFRSimpType(min,max) {}

	virtual ~HFrLodThreshold() {}

	/*!
		Set rendering options based on simplification type
		\param pCfr A pointer to a HConstantFrameRate Object.
		\param SimpLevel Simplification level for the current type

	*/
	void Set(HConstantFrameRate *pCfr, int SimpLevel);
	/*! \return The simplification type associated with this class.  */
	ConstFRType GetType() { return ConstFRLodThresholdType; }
	/*!
		Create a new simplification type object. Use this method to avoid object ownership problems.
		\param min lowest simplification level
		\param max highest simplification level

	*/
	static HConstFRSimpType * Create(int min, int max) {return new HFrLodThreshold(min, max); };   

};


/*! 
	This simplification type replaces all shells in the scene with a bounding box. A simplification level of 
	one allows only the display of the outer bounding box that surrounds the complete geometry.
*/
class MVO_API HFrSolidBBox : public HConstFRSimpType
{
public:
	/*! The default constructor. */
	HFrSolidBBox() : HConstFRSimpType()  {}
	/*! The constructor that allows you the set the minimum and maximum simplification levels.
	\param min The minimum simplification level.
	\param max The maximum simplification level.
	*/
	HFrSolidBBox(int min, int max) : HConstFRSimpType(min,max) {}

	virtual ~HFrSolidBBox() {}

	/*!
		If SimpLevel is not 0, then this method sets callbacks to suppress 3D geometry and draw a bounding box instead.
		\param pCfr A pointer to a HConstantFrameRate Object.
		\param SimpLevel Simplification level for the current type

	*/
	void Set(HConstantFrameRate *pCfr, int SimpLevel);
	/*! \return The simplification type associated with this class.  */
 	ConstFRType GetType() { return ConstFRSolidBBoxType; }

     //!  registers various callbacks needed for this class
	static void RegisterCallbacks();
	/*!
		Create a new simplification type object. Use this method to avoid object ownership problems.
		\param min lowest simplification level
		\param max highest simplification level

	*/
	static HConstFRSimpType * Create(int min, int max) {return new HFrSolidBBox(min, max); };   

private:
	static void draw_solid_bbx_tree(HIC_Rendition *rendition, HIC_Segment_Info * geo3);
	static void draw_solid_bbx(HIC_Rendition *rendition, HIC_Segment_Info * geo3);

};
 
/*! 
	This simplification type replaces all shells in the scene with wireframe box. A simplification level of one allows only display
	of the outer bounding box that surrounds the complete geometry.
*/
class MVO_API HFrWireframeBBox : public HConstFRSimpType
{
public:
	/*! The default constructor. */
	HFrWireframeBBox() : HConstFRSimpType()  {}

	/*! The constructor that allows you the set the minimum and maximum simplification levels.
	\param min The minimum simplification level.
	\param max The maximum simplification level.
	*/
	HFrWireframeBBox(int min, int max) : HConstFRSimpType(min,max) {}

	virtual ~HFrWireframeBBox() {}

	/*!
		This method sets callbacks to suppress 3D geometry and draws a wireframe bounding box instead.
		\param pCfr A pointer to a HConstantFrameRate Object.
		\param SimpLevel Simplification level for the current type

	*/
	void Set(HConstantFrameRate *pCfr, int SimpLevel);
	/*! \return The simplification type associated with this class.  */
	ConstFRType GetType() { return ConstFRWireframeBBoxType; }
 
    //!  registers various callbacks needed for this class
	static void RegisterCallbacks();


	/*!
		Create a new simplification type object. Use this method to avoid object ownership problems.
		\param min lowest simplification level
		\param max highest simplification level

	*/
	static HConstFRSimpType * Create(int min, int max) {return new HFrWireframeBBox(min, max); };   
private:
	static void draw_wireframe_bbx(HIC_Rendition   *rendition,HIC_Segment_Info * geo3);

};

/*! 
	This simplification type activates wireframe drawing and sets the Heuristic  'no hidden surfaces'. 	
	A simplification level of 0 displays a no lod wireframe drawing.
	A simplification level of 1 takes the lod settings in the list into account
*/
class MVO_API HFrWireframe : public HConstFRSimpType
{
public:

	/*! The default constructor. */
	HFrWireframe() : HConstFRSimpType()  {}

	/*! The constructor that allows you the set the minimum and maximum simplification levels.
	\param min The minimum simplification level.
	\param max The maximum simplification level.
	*/
	HFrWireframe(int min, int max) : HConstFRSimpType(min,max) {}

	virtual ~HFrWireframe() {}

	/*!
		If SimpLevel is 0, the methods tells HOOPS to render a wirefram drawing.  If SimpLevel is 1, then
		it takes into account other lod settings in the simplification level list.
		\param pCfr A pointer to a HConstantFrameRate Object.
		\param SimpLevel Simplification level for the current type

	*/
	void Set(HConstantFrameRate *pCfr, int SimpLevel);

	/*! \return The simplification type associated with this class.  */
 	ConstFRType GetType() { return ConstFRWireframeType; }

	/*!
		Create a new simplification type object. Use this method to avoid object ownership problems.
		\param min lowest simplification level
		\param max highest simplification level

	*/
	static HConstFRSimpType * Create(int min, int max) {return new HFrWireframe(min, max); };   

};


//! The HConstantFrameRate class provides support for maintaining a constant framerate when rendering
/*!
  HConstantFrameRate stores and manages 'simplification-level' objects which are used as fallbacks
  during rendering, in order to maintain a constant framerate.  It also includes logic to determine
  the current framerate and if fallback is necessary and how much.
*/
class MVO_API HConstantFrameRate  
{
	friend class HConstFRSimpType;
	friend class HFrNoSimp;
public:
	/*! 
	   Constructs a new HConstantFrameRatge Object.
 	   \param view Pointer to HBaseView object
 	*/
	HConstantFrameRate(HBaseView *view);
	virtual ~HConstantFrameRate();

	HBaseView* GetView() { return m_pHView; }	/*!< \return A pointer to the HBaseView object. */

	/*! 
		This method calls update on the simplication type associated with SimpListPos with the given SimpLevel.
		\param SimpLevel Simplification Level
		\param SimpListPos Position in current Simplification Level
 	*/
	void UpdateLevel(int SimpLevel, int SimpListPos);
	
protected:
	HBaseView		*m_pHView;				/*!< A pointer to the the associated view object. */
	bool			m_bConstFRActive;		/*!< This value indicates whether the constant frame rate mode is currently active. */
 	/*! 
	   Target frame rate in frames per second.  The system will try to achieve this frame rate if
	   the constant frame rate mode has been activated.
	*/
	float			m_TargetFrameRate;	  

	/*! 
	   List of object simplification types used by the constant frame rate logic to simplify the scene and increase rendering speed.  
	   The list should be in order of increasing simplification.
	*/
	HConstFRSimpType		**m_pSimpList;	

	bool			m_pOwnsList;
										

	int				m_SimpLevel;		/*!< Current object simplification level used by the constant frame rate logic. */
	int				m_SimpListPos;		/*!< Current object simplification type used by the constant frame rate logic. */
 
	int				m_SimpListLength;	/*!< Length of the object simplification list used by the constant frame rate logic. */
	
 	
	   	
 	
	DetailChangeMode	m_SimplificationDecrease; /*!< State variable that reflects if a detail decrease is still allowed. */
	DetailChangeMode	m_SimplificationIncrease; /*!< State variable that reflects if a detail increase is still allowed. */
	DetailChangeMode	m_SimplificationIncreaseOnIdle; /*!< State variable that reflects if a detail increase is still allowed. */

	int				m_FrameRateCounter;			/*!< Counter for number of frames passed before framerate is calculated. */
 
	/*! Determines if a detail increase is allowed after the 
		constant framerate logic has been activated.
	*/
	DetailChangeMode	m_DetailIncMode;	
	DetailChangeMode	m_DetailIncOnIdleMode;	
	/*! Determines if a detail decrease is allowed after the 
		constant framerate logic has been activated.
	*/
	DetailChangeMode	m_DetailDecMode;

	float			m_DeadZone;					/*!< Tolerance around target framerate. */
	int			m_lastupdown;
	/*! Flag that indicates if detail level should be significantly reduced if framerate has fallen 
		below a critical threshold. 
	*/
	bool			m_bEmergencyDecrease;		
 
	/*! Flag that indicates whether an event like a user interaction has occurred that would effect the framerate.  If it false,
	then no event occurred.
	*/
	HActivityType			m_ActivityType;

	/*! 
		Sets HOOPS rendering and driver options for the given simplification type and level.  
		\param SimpLevel  Simplification level inside the current simplification type
		\param SimpListPos Current position in the Simplification list - determines current simplification type.
	*/
	void			SetHoopsSimplificationOptions(int SimpLevel, int SimpListPos);

	/*! 
		This method adjusts simplification level and simplification list pointers to valid values inside the simplification ranges.
 		\param SimpLevel Pass by reference.  This method returns with the simplification level adjusted to be within simplification range.
 		\param SimpListPos Pass by reference.  This method returns with the simplification list position adjusted to be within simplification range.

	*/
	void			AdjustListPointers(int &SimpLevel, int &SimpListPos); 


	/*! 
		This method increases or lowers the simplification level as necessary in order to achieve the target framerate. 
		\param FrameRate The rendering time for the last frame in frames per second.
	*/
	void			AdjustDetail(float FrameRate);

 
	/*! 
		This method creates a new detail increase timer and starts it so that the Tick() method will be called periodically.
	*/
	void			StartTimer();
	
	/*! 
		This method stops the detail increase timer.  The Tick() method will no longer be called by the timer.
	*/
	void			StopTimer();

	bool	IsLowestDetail();
 	void	ChangeDetail(int change, int *SimpLevel = 0, int *SimpListPos = 0);



public:
 
	/*!
		This method lets you retrieve the simplification type list, its length and the target frame rate.  Please
		do NOT use the pointers to the objects returned to change the their values.  If you want to modify
		the target frame rate, use the SetTargetFrameRate() method.  If you want to update the simplification type list,
		please use the Init() method.

		\param sd If sd is not 0, the method returns a pointer to the simplification type list.  
		This is a read- only array. If you want to update the list, use the Init() method.
		\param target_framerate If target_framerate is not 0, the method returns the desired update rate for the scene, in frames per second.
		\param length If length is not 0, the method returns the length of the simplification array.
		\return  True if there is a simplification type list and false if none has been set up.  

	*/
	bool			GetMode(HConstFRSimpType ***sd = 0L ,float *target_framerate = 0L, int *length = 0L);

	/*!
		Initializes constant frame rate mode, sets the target framerate, and supplies a list of geometry simplification levels.  
		The levels are used by the internal framereate logic to degrade the visual quality of the geometry in the scene in 
		order to maintain the target framerate.  If no list of simplification levels is provided, a default list is used.
 		\param target_framerate The desired update rate for the scene, in frames per second.
 		\param sd A pointer to an array of simplification types. Passed by reference.
		\param length The length of the simplification list.
		\param deadzone Allowed variance from target framerate
	*/
 	void			Init(float target_framerate=20.0, HConstFRSimpType **sd = 0, int length = 0, float deadzone =3.0);

	/*! This method clears the simplification types list. 
	*/
	void			Shutdown(); 


	bool			GetActive() {return m_bConstFRActive;}	/*!< \return True if constant framerate watch is activated and false if otherwise. */

	/*!
		This method checks the update time for every frame and calls AdjustDetail() with a frames per second value.
		This method updates the scene if the view position has changed since the last update OR different activity
		that is relevant for the framerate has occured.
	*/
	void			Watch();			


	/*!
		If the simplification type list is set along with the deadzone and target framerate, this method activates
		the constant framerate functionality.  It starts the timer. The watch function will also begin making adjustments
		to the scene according to the state variables that have been set. This function is normally called when an 
		operator is started (e.g. mouse down event on rotate).
	*/
	void			Start();

	/*!
		Deactivates the constant frame rate functionality.  This function is normally called if an operator is stopped 
		(e.g. mouse up event on rotate).
	*/
	void			Stop();

	/*!
		Temporarily bumps up the detail level if e.g. no mouse movement has occured
		Also adjust the base detail level based on the draw time of the frames
		This function is usually called from a timer event in the main application.
	*/
	void			IncreaseDetailTemp();

	/*! 
		Determines if the viewpoint has changed for the associated scene.
		\param update  Pass true if you want the new viewpoint to be recorded if it has changed.  Default is true.
		\return True if the viewpoint has changed and false if otherwise.
	*/
	bool			ViewpointHasChanged(bool update = true);
 
	/*! This method sets the state of detail increase mode which tells the framerate logic whether to try 
	a lower simplification level when the actual framerate exceeds the target frame rate.
		\param mode New Detail change mode
	*/
	void			SetDetailIncMode (DetailChangeMode mode) {m_DetailIncMode=mode; m_SimplificationIncrease = mode;}
	void			SetDetailIncOnIdleMode (DetailChangeMode mode) {m_DetailIncOnIdleMode=mode; m_SimplificationIncreaseOnIdle = mode;}
	
	/*! \return The current state of detail increase mode which determines if the framerate logic should be allowed
		to try a lower simplification level if the actual framerate exceeds the target framerate.*/
	DetailChangeMode	GetDetailIncMode () {return m_DetailIncMode; }	

	/*! Sets the state of the detail decrease mode which determines if the framerate logic should be allowed to try 
	increase the simplification level if the actual framerate is less than the target framerate.
		\param mode New Detail decrease mode
	*/
	void			SetDetailDecMode (DetailChangeMode mode) {m_DetailDecMode=mode; m_SimplificationDecrease = mode;}
	/*! \return The state of detail decrease mode which determines if the framerate logic should be allowed to try 
	increase the simplification level if the actual framerate is less than the target framerate.*/
	DetailChangeMode	GetDetailDecMode () {return m_DetailDecMode; } 
	/*! Resets the simplification list and level pionters.
	*/ 	
	void			ResetListPointers() { m_SimpLevel=0; 	m_SimpListPos = 0; }	 

	/*! \return Target framerate which we want the scene to maintain.*/
	float			GetTargetFrameRate() { return m_TargetFrameRate;	}	
	/*! Sets the new target framerate which we want the scene to maintain during rendering. 
		\param framerate New target framerate
	*/
	void			SetTargetFrameRate( float framerate ) { m_TargetFrameRate = framerate;} 
	/*! Sets the new dead zone around the target framerate. Framerate changes in that area will not result in
		detail level changes
		\param deadzone new dead zone
	*/
	void			SetDeadZone( float deadzone ) { m_DeadZone = deadzone;} 	
	/*! Sets the emergency decrease feature which allows sharp drops in detail level if the framerate is very low.
		\param detaildecrease Turn detaildecrease on/off
	*/
 	void			SetEmergencyDetailDecrease( bool detaildecrease ) { m_bEmergencyDecrease = detaildecrease;}
	/*! This method sets the status of the activity flag. The information is used by this class	to determine how 
	to maintain the framerate.  This method supersedes SetNoActivity().
		\param at Pass the current activity type which can be NoActivity, MovementActivity, CuttingPlaneActivity or GeneralActivity.
	*/
	void			SetActivityType(HActivityType at) { m_ActivityType = at; }

  	 /*! \return The state of the activity flag which can be NoActivity, MovementActivity, CuttingPlaneActivity or 
	 GeneralActivity.  This flag affects the constant framerate logic. */
    HActivityType	GetActivityType() { return m_ActivityType;}
	int				GetSimpLevel()	{ return m_SimpLevel;}	 /*!< \return Current simplification level. */

	/*! This method adjusts the LOD rendering settings. If there is a #ConstFRLodClampType in the simplification list, 
	this method sets the LOD clamp value to the simple level maximum.  If there is a #ConstFRLodThresholdType
	in the simplification list, then this method performs a calculation based on the simple level maximum to 
	set the LOD threshold.*/
	void			AdjustLodOptions();	
	/*!< \return State of detail increase flag. */
	bool			GetDetailIncreaseState() { return m_bIncreaseDetailTemp; } 	 
	/*! Sets the default detail level before the framerate logic is started.
		\param listpos simplification type
		\param simplevel simplification level for given type
	*/
	void			SetDetailLevel ( int listpos, int simplevel) { m_SimpListPos = listpos; m_SimpLevel = simplevel; }
	
	/*! This method returns a pointer to the object in the simplification list that correctsponds to given ConstFRType.
	\param stype The ConstFRType type you are looking for in the simplification list.
	\return The HConstFRSimpType object in the simplification list whose type is the given stype.
	*/
	HConstFRSimpType			* GetSimpType(ConstFRType stype);
	/*! Sets the current simplification level.
		\param simplevel simplification level for the scene
	*/	
	void			SetSimpLevel(int simplevel) { m_SimpLevel = simplevel; }
 
	/*! This method adjusts the default detail level by calculating the complexity of the model of the associated view.
 	*/	
	void AdjustDefaultDetailLevelToModel();

 	/*!
	  Tick function called from timer manager.
	  \param request_time Timer Parameter
	  \param actual_time Timer Parameter
 	  \param user_data User Data
	  \return True if the event was processed.
 	*/		
	static bool Tick(float request_time, float actual_time, void * user_data);

	void SetDisableIncreaseTemp(bool it) { m_bDisableIncreaseTemp = it; }
	void InitiateDelay() { m_bOneDelay = true; }
	int GetDetailLevelNum();
	int GetTotalDetailLevelNum();

	void SetWatchActivity(bool wa) { m_bWatchActivity = wa; }
	void MajorDetailDecrease();
	void MajorDetailIncrease();
	
	
private:
	bool			m_bFullDetailTemp;
	bool			m_bWatchActivity;
	bool			m_bIncreaseDetailTemp;
 	float			m_oldPos[11]; 
	HTClient *		m_pUpdateTimerClient;
	bool			m_bDisableIncreaseTemp;
	bool			m_bOneDelay;
	float			m_OneDelayStart;
	bool			m_bAdjustNextTime;
	float			m_AdjustTime;
	bool			m_bIncrease;
	int				m_CounterThreshold;
	int				m_TempDetailIncrement;
	bool			m_bFirstIncrease;


 };


#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif








