// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

/*! \file HBhvBehaviorManager.h 
	Interface of the HBhvBehaviorManager class
	*/

#ifndef _H_HBhvBehaviorManager_H
#define _H_HBhvBehaviorManager_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"
#include "HTManager.h"
#include "HBhvUtility.h"
#include "HBhvInterpolator.h"

class HBhvTimeline;
class HBhvInterpolator;
class HBhvAnimation;
class HUtilityXMLGenerator;
class HUtilityXMLTag;
class HBaseModel;


class HBhvBehaviorManager;
class HBhvSensor;
class HBaseView;

/*! \enum HBhvCameraType Camera Type of target object */
enum HBhvCameraType
{
	NoCamera,		 //!< There is no camera animation.
	CameraTarget,	 //!< Animation is target driven.  The position location is derived from the target location.
	CameraPosition,	 //!< Animation is position driven.  The target location is derived from the  position location.
	CameraTargetFree,	 //!< The camera target is independently defined but works in conjunction with CameraPositionFree.
	CameraPositionFree,	 //!< The camera position is independently defined but works in conjunction with CameraTargetFree.
	CameraPositionTarget	 //!< Animation keyframe defines the camera target and position.
};


//! The HBhvTargetObject class encapsulates various target types for animations and sensors.
/*!	
 HBhvTargetObject is mainly used in conjunction with animations and sensors to describe 
 a segment path in the HOOPS database but it can also point to a named animation or sensor. 
 The target string consists of the target type followed by the target descriptor.

 "HKEY: " : A unique key identifying the object<br>
 Example:  Target = "HKEY: 123" 

 "SPATH:" : A segment path corresponding to the HSF segment structure. SCENE and MODEL are reserved keywords 
 (they refer to the model segment  and scene segment as defined in MVO). Specifying SCENE as the segment name 
 can be useful to make global color changes or to do camera movement.  "CAMERAPOS" and "CAMERATARGET" are reserved 
 keywords that refer to the camera of the scene. Translation and quaternion rotation will be applied to the camera 
 position/orientation. Camera manipulation will usually require separate animations  performed in conjunction. One 
 for the camera movement (using the POS interpolator), one for the camera orientation (using QUAT or EULER) and one 
 for zooming in/out (using the scale interpolator). For  "orbit" types of camera animation it is usually preferable 
 to animate the cameratarget, while for walkthroughs the camera position should be used. The camera keywords can 
 appear at the end of a target statement.This is useful if multiple cameras in the scene need to be manipulated. 
 <br>Examples:<br>
 Target = "SPATH: MODEL/car/wheel1" <br>
 Target = "SPATH: SCENE/CAMERATARGET"

 
 "ANIM:" The name of an animation tag<br>
 Example: Target = "ANIM: a1"

 "SSR:" The name of a Sensor tag<br>
 Example: Target = "SSR: s1"

*/ 

class MVO_API HBhvTargetObject
{
public:
	/*!
	Constructs an HBhvTargetObject object.
    \param behaviourmanager A pointer to HBhvBehaviorManager object that is associated with this target object.
    \param name The name of target object.
	\param path The target description.  You can use CreateTargetString to generate the path for this target object.
	\param pivot Sets the center of rotation if the target object is a segment in the HOOPS segment hierachy
  	*/
	HBhvTargetObject(HBhvBehaviorManager *behaviourmanager, const char *name, const char *path, HPoint *pivot = 0);

 
 	/*! \return The name of this target object. */  	
	const char *GetName() { return m_Name; }

 	/*! \return The path associated with this target object. Aliases are not expanded. */  	
	const char *GetPath() { return m_Path; }

 	/*! \return The key of target object if applicable. */  	
	HC_KEY GetTargetKey();

	void SetTargetKey(HC_KEY key) { m_key = key; }

 	/*! \return The path of target object with aliases fully expanded and no type descriptor.*/  	
	const char *GetResolvedPath() { return m_ResolvedPath; }

 	/*! \return Type of target object.  This can be HKEY, ANIM for animation, SPATH for segment and 
	SSR for sensor.*/  	
	const char *GetType() { return m_Type; }

	/*!
	Compares this target object with another one to determine equality.
 	\param target Pass a target descriptor string.  You can generate this string using the CreateTargetString method.
	\return False if they are not equal or true if they are equal.
 	*/
	bool IsEqual(const char *target);

 	/*! \return The camera type for this target object.*/  	
	HBhvCameraType GetCameraType() { return m_CameraType; }

	/*!
	Given an unexpanded target descriptor, this method returns the corresponding HOOPS segment path, target type and 
	camera type for this target object.
 	\param in The target string to evaluate.
 	\param result Returns the expanded target string.
 	\param targettype Returns the type of target.
	\param model Pointer to HBaseModel object.
	\param ctype Returns the camera type.
  	*/
	static void ResolveTarget(const char *in, char *result, char *targettype, HBaseModel *model, HBhvCameraType &ctype);


	/*!
	This utitlity method combines a target type and path to one target into one string.  The generated name can be
	used as an identifier to pass to the method IsEqual() and the methods HBhvBehaviorManager::AddAnimation 
	and HBhvBehaviorManager::FindAnimation to identify a target by a string.
  	\param type The type of the target such as HKEY, SPATH, ANIM or SSR(sensor).
  	\param path If the type is SPATH, path is the path to the segment. If the type is HKEY, then path is the
	unique HKEY identifier.  If type is ANIM or SSR, then pass a the name. 
	\param result Returns a pointer to the combined target string.
  	\param addquotes Pass true if target string should be enclosed by quotation marks or false if you don't want quotes.
  	*/	static void CreateTargetString(const char *type, const char *path, char *result, bool addquotes = false);

 	/*! Sets the center of rotation for this target object.
	\param pv The pivot point.
	*/  	
 	void SetPivot(HPoint pv) { m_Pivot = pv; }

 	/*! 
	This method allows you to indicate if you want this object to be evaluated for collisions. Any collision between this object and the environment will trigger a 
	HAnimationListener::ObjectCollisionEvent event, which can in turn be received by any class deriving from 
	HAnimationListener.  This allows an application to be aware of inter-object collisions during an animation
	and provide visual feedback on the (typically by highlighting the collided objects).
	\param collision Pass true if you want this object to be evaluated for collision.
	*/  	
 	void SetCollision(bool collision) { m_bCollision = collision; }

	/*! \return Returns true if the target should be evaluated for collisions. */  	
 	bool GetCollision() { return m_bCollision; }

 	/*! Sets the center of rotation for the target object. 
	\param x The x coordinate position of the pivot point.
	\param y The y coordinate position of the pivot point.
	\param z The z coordinate position of the pivot point.
	*/  	
	void SetPivot(float x, float y, float z) { m_Pivot.Set(x,y,z); }

 	/*! Returns the center of rotation for the target object.*/  	
	void GetPivot(HPoint &pv) { pv = m_Pivot; }

 	/*! \return Center of rotation for target object. */  	
	HPoint * GetPivot() { return &m_Pivot; }

	/*!
	Writes the XML data for this target object. 
	\param xmlgen A pointer to the xml generator that creates correct xml blocks
  	*/
	void Serialize(HUtilityXMLGenerator *xmlgen);

	/*!
	This is XML read callback which parses XML data and then populates the member variables with the associated values.
  	*/
	static void *XMLCallback(HUtilityXMLTag *xt, bool open, void *m_pExtraData);
	/*
	void CalculateAdjustedPosition(HBhvTargetObject *parent);
	void AdjustChildTargets();
	void AddChildTarget(HBhvTargetObject *child);
	void GetTranslationFromMatrix(HPoint &t);
	void AddRotationToMatrix(float rot[16], float outrot[16]);
	void AddPositionToMatrix(HPoint &trans);
	void SetConstraintPoint(float x, float y, float z) { m_constrainpoint.Set(x,y,z); }
 	void SetLimitAxis(float x, float y, float z) { m_limitaxis.Set(x,y,z); }
	*/

 	/*! Call this method to indicate that this object has moved and should be checked for
	collision with other objects.*/  	
	void FlagForCollision() { m_bHasMoved = true; }
 	/*! Call this method indicates this object has not moved and does not need to be checked
	for collisions with other objects.*/  	
	void ResetForCollision() { m_bHasMoved = false; }
 	/*! \return True if the object has moved during a particular animation slice or false if it has not.*/  	
	bool HasMoved() { return m_bHasMoved; }
	void SetSerializeFromKey( bool sfk ) { m_bSerializeFromKey = sfk; }
	
 
protected:
	/*! This method is for internal use only. */
	static void CollapseTarget(const char *target, char *collapsedtarget);

 	char					m_Name[MVO_SMALL_BUFFER_SIZE];				/*!< The name of this target object. */
	char					m_Path[MVO_SEGMENT_PATHNAME_BUFFER];		/*!<  The unresolved path. */
	char					m_ResolvedPath[MVO_SEGMENT_PATHNAME_BUFFER];/*!<  The resolved path. */ 
	char					m_Type[MVO_SMALL_BUFFER_SIZE];				/*!<  The target type. */
	HC_KEY					m_key;										/*!<  The key of target object. */
	HBhvBehaviorManager *	m_pBehaviorManager;							/*!<  A pointer to BehaviorManager object that this animation belongs to. */
	HBhvCameraType			m_CameraType;								/*!<  The type of camera. */
	HPoint					m_Pivot; 									/*!<  The center of rotation. */
	bool					m_bCollision;								/*!<  A flag indicating if this object should be evaluated for collisions. */
	bool					m_bHasMoved;								/*!<  A flag indicating if object has moved during one animation slice. */
// 	struct vlist_s *		m_ChildTargetList;
//	HPoint					m_limitaxis;
//	HPoint					m_constrainpoint;
//	HBhvTargetObject*		m_ParentTarget;
	bool m_bSerializeFromKey;
};



//! The HBhvBehaviorManager class stores and manages all animation related data.
/*!	
	The HBhvBehaviorManager class stores a list of animations, sensors and target objects and 
	manages them on a frame by frame basis.
*/ 

class MVO_API HBhvBehaviorManager : public HTClient
{
public:
	/*!
	Constructs an HBhvBehaviorManager object.
    \param model A pointer to HBaseModel object that this BehaviorManager object belongs to.
	\param tps The ticks per second value for measuring time.
	\param delay The initial delay for this HBhvBehaviorManager object.
    \param name The name of the behavior manager.
	\param version Additional Version Information.
 	\param vendor Additional Vendor Information.
   	*/

	HBhvBehaviorManager(HBaseModel *model, int tps = 10, int delay = 0, const char *name = 0, const char *version = 0, const char *vendor = 0);
	virtual ~HBhvBehaviorManager();

	/*! 
	This method set the number of ticks that will occur per second. 
	\param tps The number of ticks per second. */
 	void  SetTicksPerSecond(int tps) { m_Tps =  tps; }
 	/*! 
	\return The number of ticks per second. */
	int  GetTicksPerSecond() { return m_Tps; }

	/*!
	This method writes XML data to a buffer.
	\param xmlgen A pointer to an XML generator that creates valid XML blocks.
  	*/
	void Serialize(HUtilityXMLGenerator *xmlgen);
 
	/*!
	This method schedules the given animation for immediate execution.
    \param animation A pointer to the animation you want to schedule for execution
	\param currenttime The start time for the animation in ticks.
   	*/	
	void ScheduleAnimation(HBhvAnimation* animation, float currenttime);

	/*!
	This method creates a new animation based on given name, target, timeline and interpolator.  Then it adds 
	the new animation to the animation list.
    \param name The name of the new animation.
 	\param target A pointer to the target for the new animation.
	\param timeline A pointer to the timeline for the new animation.
	\param interpolator A pointer to the interpolator for the new animation.
	\return A pointer to the newly created animation. 
   	*/	
 	HBhvAnimation * AddAnimation(const char *name, const char *target , HBhvTimeline *timeline, HBhvInterpolator *interpolator);

	/*!
	This method adds a given animation to the animation list.
    \param animation Animation to add to animation list
   	*/	
	void AddAnimation(HBhvAnimation *animation);

	/*!
	The timer method that gets called from the HTManager::Tick() function.  This method updates the current time and 
	executes animations that are scheduled for the current time.
   	*/		
	virtual bool Tick( float request_time, float actual_time );

 	/*! \return A pointer to the current animation list.  Note this information is for read-only purpose.  To modify any information in
	animation list, please use the methods provided by this class.*/  	
	vlist_s * GetAnimationList() { return m_AnimationList; }

 
 	/*! This utility method finds a unique animation name.  This method only works 
	if you are using an incremented naming scheme.
	\return Unused int value indicating free name for a new animation.*/  	
	int GetFreeName();

 	/*! This method deletes all animations and associated objects */
  	void DeleteAllAnimations();

	/*!
	Searches for an animation that matches the given target string and interpolator type.
     \param target The target descriptor to search for in animation instance.  You can use 
	 HBhvTargetObject::CreateTargetSting() to generate this string.
     \param interpolatortype The interpolator type to search for in animation instance.
	 \return A pointer to the matching animation or null if no animation was found.
   	*/	
	HBhvAnimation * FindAnimation(const char *target, const char *interpolatortype);

	/*!
	Searches for an animation by name.
    \param name The name of the animation instance.
	 \return A pointer to the matching animation or null if no animation was found.
   	*/	
	HBhvAnimation * FindAnimationByName(const char *name);

	/*!
	Places all animations in the scheduled animation list.
   	*/	
	void ScheduleAllAnimations(bool reset = false);

	/*!
	 Removes the given animation from the animation list and then deletes it.
     \param animation Pass a pointer to the animation you want to delete.
   	*/	
	void DeleteAnimation(HBhvAnimation *animation);
	
	/*!
	This method takes the given name and target and searches through the target object list.  If a target object 
	corresponding to the name and path exists, it returns that target object. If there are no matches, it creates a new
	one with the given name and path and adds it to the target object list.
	\param name The name of the new target object.
	\param path The path of the new target object.
	\return A newly created target object or an already existing target object.
   	*/		
	HBhvTargetObject * CreateTargetObjectByPath(const char *name, const char *path);

	/*!
	 This method adds the given target object to the target object list.
     \param targetobject The target object to added.
   	*/	
	void AddTargetObject(HBhvTargetObject *targetobject);

	/*!
	This method searches the target object list for target object by name.
     \param name The name of the target object.
     \return A pointer to the target object with the given name or null if no target.
	 object corresponding to the given name was found.
   	*/		
	HBhvTargetObject * FindTargetObjectByName(const char *name);

	/*!
	Searches for a target object given a path.
     \param path The path of the target object. 
     \return A pointer to the target object with the given name or null if no target object was found.
   	*/		
	HBhvTargetObject * FindTargetObjectByPath(const char *path);

	/*!
	 Serializes all animation data to buffer and writes the buffer to a file.
     \param filename The filename you want to create.
   	*/	
	void WriteToFile(const __wchar_t *filename);
#ifdef _MSC_VER
	void WriteToFile(const unsigned short *filename);
#endif
	void WriteToFile(const char *filename);

	/*!
	 Reads XML data containing animation tags from a file and loads it into memory.
	 \param model A pointer to the HBaseModel object.
     \param filename Filename
   	*/	
	static void ReadFromFile(HBaseModel *model, const __wchar_t *filename);
#ifdef _MSC_VER
	static void ReadFromFile(HBaseModel *model, const unsigned short *filename);
#endif
	static void ReadFromFile(HBaseModel *model, const char *filename);

	/*!
	Sets up the XML parser to read animation related xml tags.
     \param model A pointer to the model object.
	 \param buffer A pointer to the memory buffer containing XML data.
   	*/		
	static void ProcessXMLData(HBaseModel *model, const char *buffer);

	/*!
	This is XML read callback which parses XML data and then populates the member variables with the associated values.
  	*/
	static void *XMLCallback(HUtilityXMLTag *xt, bool open, void *m_pExtraData);

 	/*! \return A pointer to the model object*/  	
	HBaseModel * GetModel() { return m_pModel; }

 	/*! 
	Sets the current tick based on a given time.
	\param t The time in seconds. */
 	void  SetCurrentTickByTime(float t) { m_CurrentTick =  (t - m_StartTime) * m_Tps; }

 	/*! Set the current ticks.  If the animation is currently playing, it will make an adjustment
		to m_StartTime based on the difference between old and new tick values.
		\param tick The tick value. 	*/
	void  SetCurrentTick(float tick);

	 /*! Set current tick based on current time. */
	void  SetCurrentTick();

 	/*! \return The current tick value.*/  	
	float GetCurrentTick() { return m_CurrentTick; }	

 	/*! Calculates the current tick based on a given percentage of total length of all animations.
		\param percentage  The percentage value used to calculate the current tick.*/	
	void SetCurrentTickByPercentage(float percentage); 

 	/*! \return The percentage value of the currrent tick in relation to the total length of all animations.*/  	
	float GetCurrentTickByPercentage();

	/*! Stops the execution of all animations and rewinds to the start. */	 	
	void Rewind();

 	/*! \return True if animations are currently playing or false if no animations are playing. */
	bool IsPlaying() { return m_bPlaying; }

	/*! \return The key to active scene.  This key will be used for camera manipulation. */	
	HC_KEY GetActiveSceneKey();

	/*! Use this method to determine which view to apply camera manipulations.
		\param view A pointer to the HBaseView object. */	
 	void SetActiveView(HBaseView *view);

	/*! \return A pointer to the HBaseView object which camera manipulations will be applied to. */	
	HBaseView * GetActiveView() { return m_pView; }

    /*!  \return True if HOOPS will not interpolate between ticks and false if HOOPS will interpolate between ticks. */
  	bool GetRenderEveryTick() { return m_bRenderEveryFrame; }

	/*! Enables/disables interpolation between ticks.
    \param everytick Pass true if you do not want interpolation between ticks and false if you want to interpolate between ticks. */	
	void SetRenderEveryTick(bool everytick) { m_bRenderEveryFrame = everytick; }

    /*!  \return True if HOOPS will update camera during animation and false if HOOPS will not update camera. */
	bool GetUpdateCamera() { return m_bUpdateCamera; }

	/*! \return True if the current tick is greater than or equal to the last tick in the animation timeline.*/
	bool IsAtFinalTick();

	/*! Use this method to indicate if the camera should be updated during animations. 
		\param updatecamera Pass True to update camera or false if otherwise. */	
	void SetUpdateCamera(bool updatecamera) { m_bUpdateCamera = updatecamera; }

	/*! Given a tick number, this method finds the next closest keyframe.	
		\param ticknum The tick to calculate next keyframe from.
		\param forward Pass true to find next closest keyframe or false to find the previous closest keyframe. 
		\return The keyframe location in tick if a keyframe is found or the given ticknum if no keyframe was found.
		*/	
	int GetNextKeyframe(int ticknum, bool forward);

	/*!	This method adds a given sensor to the sensor list.
		\param sensor Sensor to add to sensor list. */	
	void AddSensor(HBhvSensor *sensor);

	/*! Removes the specified animation from the animation list.
		\param animation A pointer to the animation that you want removed. */	
	void RemoveScheduledAnimation(HBhvAnimation *animation);

    /*!  \return True if at least one animation exists or false if no animation exists. */
	bool HasAnimations();

    /*!  \return The last tick of all items in the animation list. */
	int GetLastTick();

	/*!	Searches for a sensor by name.
		 \param name The name of the sensor to search for.
		 \return A pointer to the matching sensor or null if no sensor was found. */	
	HBhvSensor * FindSensorByName(const char *name);

	/*! Executes all the scheduled animations.
		\param currenttick The current time in ticks.
		\param starttick The start time  in ticks.  If you pass -1, <i>currenttick</i> will be the start time. */	
	void ExecuteAnimations(float currenttick, float starttick);

	/*! Enables/disables continuous playing of the animation list.  In other words, if continuous play is on, 
	when the last tick is reached, everything is rewound and starts playing from the beginning again.  Note
	that if infinite play is active, it will override the continuous play behavior.
		\param cont Pass true to enable continuous play or false to play animations once. */		
	void SetContinuousPlay(bool cont) { m_bContinuousPlay = cont; }

	/*! Enables/disables infinite play.  If infinite play is active, then once the animation begins playing,
	it will continue to play pass the last tick of the timeline.  This means that animations that are looped 
	will continue to play while animations that are not looped will stop.  Note that if inifinite play is active,
	it will override continuous play.
		\param inf Pass true to set infinite play to active or false to set infinite play to inactive.*/		
 	void SetInfinitePlay(bool inf) { m_bInfinitePlay = inf; }
	
	/*! \return True if the animation list loops continuously and false if the animation 
	will only play through once and then stop. Note that if infinite play is active, it will override 
	the continuous play behavior.*/		
	bool GetContinuousPlay() { return m_bContinuousPlay; }
	
	/*! Use this method to indicate that camera was updated during animation. */		
	void CameraUpdated() { m_bCameraUpdated = true; }

	/*! \return True if camera has been changed during animation and false if the camera has not been changed. */		
	bool GetCameraUpdated() { return m_bCameraUpdated; }

	/*! Starts the execution of items on the animation list, registers with MVO timer and starts playing at the 
	start time.*/	
	void Play();

	/*! Pauses the execution of the items on the scheduled animation list. */	
  	void Stop();

	/*! Reactivates the execution of items on the schedule animation list.*/	
  	void Continue();
	
	/*! Sets temporary position matrix used by interpolators for camera interpolation.
		\param mat The position matrix.*/
 	void SetPositionMatrix(float *mat) { for (int i=0;i<16;i++) m_positionmatrix[i] = mat[i]; }
	/*!  Returns the position matrix used by interpolators for camera interpolation. */
	void GetPositionMatrix(float *mat) { for (int i=0;i<16;i++) mat[i] = m_positionmatrix[i]; }
	/*! Sets the temporary modelling matrix for the target object.  This matrix is used by interpolators for 
	camera interpolation.
		\param mat The modelling matrix.*/
	void SetTargetMatrix(float *mat) { for (int i=0;i<16;i++) m_targetmatrix[i] = mat[i]; }
	/*!  \return The temporary modelling matrix for the target object.  This matrix is used by interpolators for 
	camera interpolation.*/
	void GetTargetMatrix(float *mat) { for (int i=0;i<16;i++) mat[i] = m_targetmatrix[i]; }
	/*! This method has not been implemented yet.
		\param fov */
	void SetFov(float fov) { m_fov = fov; }
	/*! This method has not been implemented yet. */
	void GetFov(float fov) { UNREFERENCED(fov); }

	/*! Resets all animations. */	
	void Reset();

	/*! This method deactivates all the items in the animation list. If an animation is deactivated, it can only be triggered by
	a sensor.*/	
	void DeactivateAllAnimations();
	/*! This method activates all items on the animation list.  When animations are not active, they can only be 
	triggered by sensors.*/	
	void ActivateAllAnimations();
	/*! This method activates an animation given its partial or full name.  When an 
	animation is deactivated, it can only be triggered by a sensor. 
		\param name The name of the animation.
		\param AllowPartial Pass true if you want to allow a partial name to be used for determining animation. */
	void ActivateAnimationByName(const char *name, bool AllowPartial = true);
	/*! This method deactivates an animation given its full or partial name.  When an animation is deactivated, it can only
	be triggered by a sensor.
		\param name The name of the animation.
		\param AllowPartial Pass true if you want to allow a partial name to be used for determining animation. */
	void DeactivateAnimationByName(const char *name, bool AllowPartial = true);

	
	/*! Sets the animation start time.  The HBhvBehaviorManager will use this time as a referenced start time for all its
	calculations as it iterates through the timeline's array of keyframes.
		\param time The start time of the animation. */
 	void SetStartTime(float time) { m_StartTime = time; }

	/*! Determines if loaded animation data is merged into existing animation.
		\param merge Pass true if you want to merge animation data.
	*/
	void SetMerge(bool merge) { m_bMerge = merge; }
	/*! \return True if new animation data is merged with existing animation. */		
	bool GetMerge() { return m_bMerge; }

	/*! This method creates a new named animation.
 		\param name The name of the new animation
		\param targetpath Segment Path specifying the segment the animation should operate on
		\param pivot Center Point of the target (allows for off-center rotation)
		\return Pointer to newly created Animation Object */
 	HBhvAnimation * AddAnimation(const char *name, const char *targetpath, HPoint *pivot);
 	
	/*! This method creates a new position keyframe for a given animation.  The position keyframe 
	allows you to specify a new position for your target object.  The interpolated path can be 
	a linear or spline movement.
 		\param animname The name of the animation
		\param tick Location of new keyframe in timeline
		\param pos 3d coordinate of keyframe 
		\param linear Specifies if keyframe defines linear movement or spline movement*/
 	void AddPositionKeyframe(const char *animname, int tick, HPoint pos, bool linear);
	/*! This method creates a new quaternion keyframe for a given animation.  The quaternion key frame 
	lets you specify a rotation for your target object.  
 		\param animname The name of the animation
		\param tick Location of new keyframe in timeline
		\param rot Quaternion for keyframe
		\param linear Specifies if keyframe defines linear rotation or non linear rotation*/
	void AddQuatSquadKeyframe(const char *animname, int tick, HQuat rot, bool linear);
	/*! This method creates a new matrix keyframe for a given animation.  Pass a matrix that describe 
	the rotation you want for your target object.
 		\param animname The name of the animation
		\param tick Location of new keyframe in timeline
		\param matrix matrix describing the transformation for the keyframe */
 	void AddMatrixKeyframe(const char *animname, int tick, float matrix[]);
	/*! This method creates a new Segment Move keyframe for a given animation. The current implementation
		only allows one keyframe of this animation type to exist in a timeline.  The segment move keyframe 
		allows you to change the segment path of your target object.
 		\param animname The name of the animation
		\param tick Location of new keyframe in timeline
		\param path Segment path for keyframe that defines where to move segment to. The leaf segment of 
		  the path will be replaced by the name of the target segment*/
 	void AddSegmentMoveKeyframe(const char *animname, int tick, char *path);
	/*! This method creates a new trail keyframe for a given animation.  Adding a trail keyframe tells HOOPS to 
	draw a line from the position of the previous keyframe to the position of the keyframe you are adding.
 		\param animname The name of the animation
		\param tick Location of new keyframe in timeline
		\param pos 3d coordinate of keyframe 
		\param linear Specifies if keyframe defines linear movement or spline movement
		\param trailtype Trailtype for all keyframes
		\param weight Trail weight for all keyframes
		\param style Line style for all keyframes
		\param color Color for all keyframes */		
	void AddTrailKeyframe(const char *animname, int tick, HPoint pos, bool linear, TrailInterpolatorType *trailtype = 0, int *weight = 0, const char *style =0, const char *color = 0);
	/*! This method creates a new Axis Rotate keyframe for a given animation.  The axis rotate keyframe lets you 
	specify the angle of rotation and axis of rotation for you target object.
 		\param animname The name of the animation
		\param tick Location of new keyframe in timeline
		\param rot Angle of rotation of keyframe
		\param axis Axis of rotation for all keyframes*/
	void AddAxisRotateKeyframe(const char *animname, int tick, float rot, HPoint *axis = 0);
	/*! This method creates a new color keyframe for a given animation.
 		\param animname The name of the animation
		\param tick Location of new keyframe in timeline
		\param color rgb color specification for keyframe
		\param colorcomponent Color component (diffuse, specular,...) for all keyframes
		\param geomtype	Geometry Type the color applies to for all keyframes */
	void AddColorKeyframe(const char *animname, int tick, HPoint color, const char *colorcomponent = 0, const char *geomtype = 0);
	/*! This method creates a new scale keyframe for a given animation.
 		\param animname The name of the animation
		\param tick Location of new keyframe in timeline
		\param pos x/y/z/ scale values for keyframe */
 	void AddScaleKeyframe(const char *animname, int tick, HPoint scale);
	/*! This method creates a new Attribute Switch keyframe for a given animation.  They attribute switch keyframes 
	changes they attributes of your target object by giving it the specified style segment.
 		\param animname The name of the animation
		\param tick Location of new keyframe in timeline
		\param att Path to segment containing attributes for keyframe*/
 	void AddAttributeSwitchKeyframe(const char *animname, int tick, const char *att);
	/*! This method creates a new vertex morph keyframe for a given animation.  This interpolator's behavior is 
	described in more detail under the #AddVertexMorphDataToAnimation method.
 		\param animname The name of the animation
		\param tick Location of new keyframe in timeline
		\param mident Vertex Morph Data Index
		\param discrete Specifies either discrete switching between morph data or interpolation for keyframe */
	void AddVertexMorphInterpolator(const char *animname, int tick, const char *mident, bool discrete);
	/*! This method creates a new color morph keyframe for a given animation. A color morph keyframe tell HOOPS to 
	perform an interpolation on the face colors of your target object from the previous keyframe to the new keyframe.  
	This method in conjunction with #AddColorMorphDataToAnimation.
 		\param animname The name of the animation
		\param tick Location of new keyframe in timeline
		\param mident Color Morph Data Index
		\param discrete Specifies either discrete switching between morph data or interpolation for keyframe */
	void AddColorMorphInterpolator(const char *animname, int tick, const char *mident, bool discrete);
	/*! This method creates a new Segment Switch keyframe for a given animation.  This segment switch keyframe 
	unset the visibility of the object specified in the keyframe.
 		\param animname The name of the animation
		\param tick Location of new keyframe in timeline
		\param ss Path to segment for which visibility should be turned on for keyframe*/
	void AddSegmentSwitchKeyframe(const char *animname, int tick, const char *ss);
	/*! This method creates a new Instance Create keyframe for a given animation.  This keyframe creates another 
	instance of the target object in the segment tree specified.
 		\param animname The name of the animation
		\param tick Location of new keyframe in timeline
		\param instance Path to instance which should be created for keyframe*/
	void AddInstanceCreateKeyframe(const char *animname, int tick, const char *instance);
	
	/*! This method adds vertex morph information to a given animation. This function supplies the actual point array and associates 
		an index to it (the first inserted point array has the index 0, the second index 1, etc)
		This method should be called AFTER defining the interpolator keyframe list using the 
		#AddVertexMorphInterpolator method.
 		\param md Pointer to morph data vertex list
		\param pnum Number of points in vertex list
		\param model Point to HBaseModel object
		\param pos Position to add morph data to */
 	void AddVertexMorphDataToAnimation(const char *animname, HPoint *md, int pnum, HBaseModel *model, int pos = -1);
	/*! This method adds color morph information to a given animation.  This function supplies the actual point array and associates 
		an index to it (the first inserted point array has the index 0, the second index 1, etc)
		This method should be called AFTER defining the interpolator keyframe list using the 
		#AddColorMorphInterpolator method.
 		\param md Pointer to morph data rgb color list
		\param pnum Number of points in rgb color list
		\param model Point to HBaseModel object
		\param pos Position to add morph data to */
	void AddColorMorphDataToAnimation(const char *animname, HPoint *md, int pnum, HBaseModel *model, int pos = -1);


protected:

	/*! 
	This method is not yet implemented.
    */	
 	void	ResolveInstances();

	/*! Determine if any collisions have occurred for specified target object.
		\param tob A pointer to the target object. 
	*/
	bool EvaluateCollision(HBhvTargetObject *tob);
	/*! This method creates a new keyframe for a given animation
 		\param animname The name of the animation
		\param IntType Type of Interpolator to create/add to
		\param keyframe Keyframe object to add to interpolator
		\param tick Location of new keyframe in timeline */
 	HBhvInterpolator * AddKeyframe(const char *animname, const char *IntType, HKeyframe *keyframe, int tick);

	char				m_Name[BHV_MAX_NAME_LENGTH];				/*!< The name of the behavior manager object.*/	
	char				m_Version[BHV_MAX_VERSION_LENGTH];				/*!< The version string. */	
	char				m_Vendor[BHV_MAX_VENDOR_LENGTH];				/*!< The additional vendor information.*/
	int					m_Tps;						/*!< The number of ticks per second.*/
	int					m_Delay;					/*!< Initial execution delay for this behaviour block*/
	float				m_StartTime;				/*!< Start of execution.*/
 	float				m_CurrentTick;				/*!< The current time.*/
 	int					m_LastTick;					/*!< Final tick of all animations.*/

 	struct vlist_s *	m_AnimationList;			/*!< The list of all animations.*/
  	struct vlist_s *	m_ScheduledAnimationList;	/*!< The list of animations scheduled for execution.*/
	struct vlist_s *	m_TargetObjectList;			/*!< The list of all target objects.*/
	struct vlist_s *	m_SensorList;				/*!< The list of all sensors.*/

	HBaseModel *		m_pModel;					/*!< A pointer to HBaseModel object that this behavior manager belongs to.*/
	bool				m_bPlaying;					/*!< The value is true if animations are currently playing or false if no animation is playing.*/
	HBaseView *			m_pView;					/*!< A pointer to HBaseView object that is associated to this behavior manager.*/
	bool				m_bRenderEveryFrame;		/*!< If true, HOOPS will display every frame of animations.  If false, HOOPS will display animations with interpolation.*/
	bool				m_bUpdateCamera;			/*!< The value is true if camera manipulations will be performed or false to not execute camera manipulations*/
	bool				m_bContinuousPlay;			/*!< The value is true if the animation list will rewound and played agained whenever the timeline reaches the end or false if the animation list will be played through once and stops at the end.*/
	bool				m_bCameraUpdated;			/*!< The value is true if camera has been changed during animation, false if camera has not been changed.*/	
	float m_positionmatrix[16];						/*!< The temporary position matrix for interpolators.*/
	float m_targetmatrix[16];						/*!< The temporary target matrix for interpolators.*/
	float m_fov;								/*!< This has not been implemented yet.*/
	bool				m_bInfinitePlay;			/*!< The flag that determines if animation should run infinitely.  In otherwords, the ticks continue to increment.*/
	bool				m_bMerge;					/*!< The flag that determines if newly loaded animation should be merged into existing data*/

	bool				m_bShellSelectionActive;
};



#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif
