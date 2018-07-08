// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef _H_HBhvAnimation_H
#define _H_HBhvAnimation_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"
#include "HBhvUtility.h"
#include "HEventListener.h"

class HBhvBehaviorManager;
class HBhvTimeline;
class HBhvTimelineInstance;
class HBhvInterpolator;
class HBhvInterpolatorInstance;
class HUtilityXMLGenerator;
class HUtilityXMLTag;
class HKeyframe;
class HBhvTargetObject;
class HBhvAnimationInstance;


//! The HBhvAnimation class defines an animation 
/*!
 By combining a timeline with one or more interpolators and a target object, the animation class defines a single animation.
 Animations must be added to the behaviour manager for execution. They can either be active by default or be
 activated by a sensor action.
 The animation class also defines various helper functions to manage interpolator keyframes and to set and retrieve various
 properties of the animation.
*/ 

class MVO_API HBhvAnimation 
{
public:
	/*!	Constructs an HBhvAnimation object.
		\param name Name of the animation. This name should be unique.
		\param BehaviorManager A pointer to HBhvBehaviorManager object that this animation belongs to.
		\param parentanimation A pointer to parent animation if it exists.  
		This value can be null if there is not parent animation. */
 	HBhvAnimation(const char *name, HBhvBehaviorManager * BehaviorManager, HBhvAnimation *parentanimation = 0);

	/*! Destructor of the HBhvAnimation object. */  
 	virtual ~HBhvAnimation();   

	/*! \return The type of animation which is 'HBhvAnimation'. */
	virtual const char * GetType() { return "HBhvAnimation"; } 

	
	/*! \return Name of the animation. */  	
	const char * GetName() { return m_Name; }

 	/*! Sets the name of the animation. */
	void SetName(const char *name);

	/*! Performs animation based on the given time.  
		\param currenttime Time (in ticks) relative to start time of overall animation. */
  	virtual bool Animate(float currenttime);

	/*! Performs animation based on the given time in relation to given start time.  
		\param currenttime Time (in ticks) relative to given start time (second parameter)
		\param starttime Start time (in ticks) to evaluate animation from */
	virtual bool Animate(float currenttime, float starttime);

	/*! This method deletes a given keyframe in the timeline and interpolators for this animation.
		\param keyframe Number of keyframe
 	*/
	void DeleteKeyframe(int keyframe);

	/*! Copies the next or previous keyframe to the given target keyframe.
 		\param keyframe The keyframe to copy to.
		\param next Pass false for the previous keyframe and true for the next keyframe. */
	void DuplicateNextOrPrevious(int keyframe, bool next);

	/*! This method makes a copy of <i>newkeyframe</i> and then replaces <i>oldkeyframe</i> with the copy.
 		\param newkeyframe Keyframe to copy to
		\param oldkeyframe Keyframe to copy from */
	void Duplicate(int newkeyframe, int oldkeyframe);

	/*! This method plots a curve on the scene representing the path of this animation. 
	\param rotations Pass true to include rotation in the curve or false to not include it.
	*/	
	void PlotCurve(bool rotations);

	/*! This method has been <i>deprecated</i>. */
 	void AdjustToTick(int totalticks);

	/*!	This method writes XML data to a buffer. */
	virtual void Serialize(HUtilityXMLGenerator *xmlgen, HUtilityXMLTag *xmlt);
	/*!	This method writes XML data to a buffer. */
	virtual void Serialize(HUtilityXMLGenerator *xmlgen);

	/*!
	This is XML read callback which parses XML data and then populates the member variables with the associated values.
  	*/
	static void *XMLCallback(HUtilityXMLTag *xt, bool open, void *m_pExtraData);

 	/*! \return A pointer to the HBhvBehaviorManager object associated with this animation. */  	
	HBhvBehaviorManager * GetBehaviorManager() { return m_pBehaviorManager; }

 	/*! Sets the HBhvBehaviorManager Object associated to this animation. */  	
	void SetBehaviorManager(HBhvBehaviorManager *behaviourmanager) { m_pBehaviorManager = behaviourmanager; }

 	/*! \return The value of first entry in the timeline. */
  	int GetFirstTick();

 	/*! \return The value of last entry in the timeline. */
	int GetLastTick();

 	/*! \return A value of 1 if the animation will be looped or 0 if the animation stops when it reaches the
	end of the timeline.*/
	int GetLoop() { return m_Loop; }

	/*! Use this method to indicate if animation is looped or plays once.
	\param loop Pass 1 if the animation should loop or 0 if the animation should stop when it reaches the end of
	the timeline.*/
	void SetLoop(int loop) {  m_Loop = loop; }

	/*! \return A pointer to the timeline associated to this animation. */
	HBhvTimeline *GetTimeline() { return m_pTimeline; }

	/*! This method associates the passed timeline with this animation. 
	\param timeline The timeline to be associated with this animation.*/
	void SetTimeline( HBhvTimeline *timeline);
 
	/*! In special cases, you may want an animation that references this animation without changing it. Use 
	this method to create such an animation.
		\return A pointer to newly created animation that refers to this animation. */
	HBhvAnimation * CreateInstance();

	/*! \return A pointer to the list of interpolators for this animation.*/
 	struct vlist_s * GetInterpolatorList() { return m_InterpolatorList; }

	/*! \return A pointer to the list of child animations associated to this animation.*/
	struct vlist_s * GetChildAnimationList() { return m_ChildAnimationList; }

	/*! \return The first interpolator in the interpolator list.*/
	HBhvInterpolator *GetInterpolator();

	/*! Adds an interpolator to the interpolator list.
	\param interpolator The interpolator to be added.*/
	void AddInterpolator(HBhvInterpolator *interpolator);

	/*! Adds an animation to the child animation list.
	\param childanimation The child animation to be added.*/
	void AddChildAnimation(HBhvAnimation *childanimation);

	/*! \return Parent animation for this animation. */
 	HBhvAnimation *GetParentAnimation() { return m_pParentAnimation; }
 
  	/*! Sets initial delay time for this animation. A delay can be used to prevent this animation
	from occuring at exactly the same time as other items in the HBhvBehaviorManager's animation list.*/
	void SetDelay(int delay) { m_Delay = delay; }

  	/*! \return The initial delay time for this animation. */
	int	 GetDelay() { return m_Delay; }

	/*! \return A pointer to target object of this animation. */
	HBhvTargetObject * GetTarget() { return m_Target; } 

 	/*! \return A pointer to the animation that this animation references. */
	HBhvAnimation * GetInstancedAnimation() { return m_pInstancedAnimation; }

	/*! Sets pointer to animation that you want this animation to reference. 
	\param animation A pointer to the animation to reference.*/
	void SetInstancedAnimation(HBhvAnimation *animation) { m_pInstancedAnimation = animation; CloneAnimation(); }

	/*! Stop the animation if it is currently running and resets all the interpolators associated with this animation.  */
 	void Reset();

	/*! This method creates a target object using the given target path and then attaches it to this
	animation.
		\param targetpath Segment path to target */  	
 	void SetTargetByPath(const char *targetpath);

	/*! This method creates a target object using the given target path and name. Then it attaches the
	newly created target to this animation.
		\param name Name of newly created target object
		\param targetpath Segment path to target */  	
 	void SetTargetByPath(const char *name, const char *targetpath);

 	/*! Associates a given target object with this animation.*/  	
 	void SetTarget(HBhvTargetObject *target);
 
	/*! For a given time, this method finds the closest keyframe and interval in the interpolator.
		\param time Time in which you are looking for the closest keyframe. 
		\param interval Returns the interval in timeline which the given time falls between.
		\param  keyframe Returns an Hkeyframe object for given time.
		\return True if a keyframe and interval were found and false if none was found.	*/
 	bool GetInterpolator(int time, int &interval, HKeyframe **keyframe);

 	/*! Sets default active parameter for this animation. If you pass false, then the animation is
	inactive by default which means that it can only be triggered by a sensor action.
		\param active Pass true if you want this animation to be active by default or false if otherwise. */	
 	void SetDefaultActive(bool active) { m_bDefaultActive = active; }

 	/*! \return True if the animation is active by default and false if otherwise. If the animation is not active by default
	then it can only become activated by a sensor or by calling SetDefaultActive and passing true.*/
	bool GetDefaultActive() { return m_bDefaultActive; }

 	/*! Sets the currently running flag which indicates whether an animation is currently executing.
		\param running Pass true to indicate that this animation is currently running and false if it is not running. */
 	void SetCurrentlyRunning(bool running) { m_bCurrentlyRunning = running; }

 	/*! \return True if the animation is running and false if it is not running */
	bool GetCurrentlyRunning() { return m_bCurrentlyRunning; }

 	/*! \return The current "tick" (i.e. frame) as reported by the behavior manager. */
	float GetCurrentTick(); 
	
 	/*! Adjusts keyframe intervals by the given number of ticks.
		\param keyframe Start keyframe to adjust.
		\param delta Amount of ticks to offset the keyframe time.
		\param doall Pass true to adjust all keyframes after the start keyframe. 
		\param relative Adjust keyframes all keyframes after the given keyframe so that they
		will also have an additional delta offset between the next keyframe. 
	*/
 	void AdjustKeyframe(int keyframe, int delta, bool doall = true, bool relative = false);
	
 	/*! This method set disassociates the current timeline from this animation and clears the interpolator list without
	deleting any of the previous associated timeline or interpolator objects.
 	*/
	void CleanWithoutDelete();


 	/*! \return True if the animation is flagged to only run for one update.*/
	bool ExecuteOnce() { return m_bExecuteOnce; }
 	/*! Use this method to indicate if this animation should only run for one update.
		\param once Pass true if you want the animation to be disabled after the next update 
		or false if you want to the animation to run normally.
 	*/
	void SetExecuteOnce(bool once) {m_bExecuteOnce = once; }

 	void Evaluate(float currenttick, bool &hasPos, HPoint &pos, bool &hasQuat, HQuat &quat, bool &hasScale, HPoint &scale);

	/*! Given the target modelling matrix, it extracts its scaling elements. */
	void SetTargetScaleMatrix(float const * target_matrix);

	float * GetTargetScaleMatrix() { return m_TargetScaleMatrix; }
	
protected:

	/*! Sets name of the animation that this animation references.
	\param name The name of the referenced animation.
	*/
 	void SetInstanceOf(const char *name);

 	/*! This is a helper function for Animate().  It checks that the current
	tick is valid for the associated timeline and then animates the interpolators.
		\param currenttick Current Time in ticks of animation */
	virtual bool AnimateInternal(float currenttick);

 	/*! Creates an animation instance. */
	void CloneAnimation();

	/*!	This method writes XML Data for this animation's target to a buffer. */
 	virtual void SerializeTarget(HUtilityXMLTag *xmlt);

	HBhvAnimation*			m_pInstancedAnimation; 				/*!< A pointer to the referenced animation (if any). */
  	HBhvTimelineInstance*	m_pTimelineInstance;				/*!< A pointer to the referenced timeline (if any). */	
  	char					m_InstanceOf[BHV_MAX_NAME_LENGTH];	/*!< The name of the referenced animation.*/	
	HBhvTargetObject*		m_Target;							/*!< A pointer to the target object of this animation.*/
	bool					m_bCurrentlyRunning;				/*!< The value is true if the animation is currently running and false if otherwise.*/
	bool					m_bDefaultActive;					/*!< The value is true if the animation is active by default and false if otherwise. */
	HBhvBehaviorManager*	m_pBehaviorManager;					/*!< A pointer to the BehaviorManager object that this animation belongs to. */	
 	HBhvAnimation*			m_pParentAnimation;					/*!< A pointer to the parent animation. */	
	HBhvTimeline*			m_pTimeline;						/*!< A pointer to the timeline object. */
	struct vlist_s*			m_InterpolatorList;					/*!< A pointer to the interpolator list. */
	struct vlist_s*			m_ChildAnimationList;				/*!< A pointer to the list of child animations.*/
 	char					m_Name[BHV_MAX_NAME_LENGTH];		/*!< Name of this animation. */
 	int						m_Loop;								/*!< A value of 1 means that this animation should be looped while a value of 0 means the animation should stop when it reaches the end of the timeline. */
	int						m_Delay;							/*!< Initial delay for this animation */
	bool					m_bExecuteOnce;						/*!< A flag that determines if animation should only run for one update. */
	float					m_TargetScaleMatrix[16];			/*!< The target scale matrix. */
};
 
 

#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif



