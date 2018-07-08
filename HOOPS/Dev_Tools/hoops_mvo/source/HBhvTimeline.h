// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef _H_HBhvTimeline_H
#define _H_HBhvTimeline_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"
#include "varray.h"


class HBhvTimelineInstance;
class HUtilityXMLGenerator;
class HUtilityXMLTag;
class HBhvAnimation;
 

#ifdef WINDOWS_SYSTEM
template class MVO_API VArray< int >;
#endif

//! The HBhvTimeline class encpasulates an animation timeline
/*!	
	Each animation consists of one timeline and multiple interpolators. The timeline is an array of ascending values
	measured in ticks with each value representing one keyframe of the animation.
*/ 

class MVO_API HBhvTimeline 
{
public:
	/*!
	Constructs an HBhvTimeline object.
	\param animation A pointer to the animation that this timeline is associated with.
 	*/
	HBhvTimeline(HBhvAnimation *animation = 0);
 	virtual ~HBhvTimeline() { };

	/*!
	This method replaces a timeline array with array of integer values.
	\param timeline The array of ticks representing the new timeline.
	\param length  The number of entries in the passed timeline.
 	*/
	void SetTimeline(const int *timeline, int length);

	/*!
	Sets the animation that this timeline is associated with.
	\param animation Pointer to Animation
 	*/	
	void  SetAnimation(HBhvAnimation *animation) { m_pAnimation = animation; }

	/*!
	In special cases, you might want to reference this timeline without changing it.  To create a new timeline
	that references this timeline, use this method and pass a pointer to the animation you want associated with
	the new timeline.
 	\param animation Animation that the created timeline instance should reference
 	\return The newly created timeline instance.
 	*/
 	HBhvTimeline * CreateInstance(HBhvAnimation *animation);

	/*! \return The timeline array which contains a list of the keyframe times.  Note this is for read-only purposes.  If you want to modify the timeline,
	use the methods provided by this class.*/
	int * GetTimelineArray() { return &m_pTimelineArray[0]; }

	/*! \return The length of the timeline array.  This is the number of keyframes in the timeline. */
	int GetTimelineArrayLength() { return (int)m_pTimelineArray.Count(); }

	/*!
	Adds a new keyframe to the timeline.
	\param t The keyframe time in ticks.
	\param doesExist Pass by reference. Returns true if a keyframe time already exists in the timeline.
  	\return The number of the newly created keyframe.
 	*/
	int AddKeyframe(int t, bool &doesExist);
	/*!
	Adds a new keyframe to the timeline.
	\param t The keyframe time in ticks.
   	\return  The number of the newly created keyframe.
 	*/
	int AddKeyframe(int t);
	/*!
	This method deletes the keyframe associated with the given time <i>t</i> from the timeline.
	\param t Time in ticks of the keyframe to delete.
   	\return  The position of the deleted keyframe or -1 if no keyframe was found and deleted.
 	*/
	int DeleteKeyframe(int t);

	/*!
	This method searches for keyframe based the given time.
 	\param t Time in ticks of the keyframe to be searched for.
   	\return  The position of the keyframe associated with the given time or -1 if no keyframe could be found.
 	*/
	int CheckKeyframe(int t);

	/*!
	Write XML Data to buffer
  	*/
	void Serialize(HUtilityXMLGenerator *xmlgen);

	/*!
	This is XML read callback which parses XML data and then populates the member variables with the associated values.
  	*/
	static void *XMLCallback(HUtilityXMLTag *xt, bool open, void *m_pExtraData);

	/*! \return The last time value of keyframe time array. */
	int GetLastTick();

	/*!
	This method find the keyframe interval that is closet to the current time.
	\param currentTick The time that we are searching for the closest interval.
	\param interval Returns interval that is closet to the current time.
	\return True if the currentTick is inside the timeline's range and false if the currentTick is 
	outside the timeline's range.
  	*/
 	bool GetTimelineInterval(int currentTick, int &interval);

	/*!
	Set the start time for the timeline. All keyframes are evaluated based on the start time.
 	\param starttick Start Tick
   	*/
	void SetStartTick(float starttick) { m_StartTick = starttick; }

	/*! \return The start time for this time line. */
	float GetStartTick() { return m_StartTick; }

	/*!
	Calculates the keyframe position and fraction value based on the current time.
	\param currentTick Current Time int tps
	\param interval Returns the keyframe interval which was closest to the currentTick.
	\param fraction Returns fraction to next keyframe which will be a value between 0 and 1.
	\return True if a valid keyframe interval was found and false if no valid keyframe was found.
   	*/
	virtual bool Evaluate (float currentTick, int &interval, float &fraction);

	/*! \return A pointer to the associated animation object. */
	HBhvAnimation *GetAnimation() { return m_pAnimation; }

	/*! \return The current tick for this timeline. */
	float GetCurrentTick() { return m_CurrentTick; }

	/*!
	Sets the timeline instance.
	\param timeline Instanced Timeline
 	*/	
	void SetInstancedTimeline(HBhvTimeline *timeline) { m_pInstancedTimeline = timeline; }
	
	/*!
	Adjusts the given keyframe time by the passed delta value. 
	\param time The time of the keyframe to begin the adjustment.
	\param delta The number of ticks to offset the given time.
	\param doall Pass true ripple the effect to all keyframes after the given time.
 	*/	
	void Adjust(int time, int delta, bool doall = true);
	/*!
	Adjust Timeline based on delta value relative to current time. In other words,
	the timeline will be offset by the pass delta value for all the keyframe interval
	at the given time and all the keyframe intervals after that time.
	\param time The time of the keyframe to begin the adjustment.
	\param delta The number of ticks to offset the given time.
  	*/	
	void AdjustRelative(int time, int delta);

	/*! \return Current tick relative to the properties in this timeline.  This value takes into account
	start time and looping. */
	float GetCurrentRelativeTick() { return m_CurrentRelativeTick; }

	/*!
	Sets current relative time position.
  	\param tick Relative Time Position
   	*/
	void SetCurrentRelativeTick(float tick) { m_CurrentRelativeTick = tick; }
	


protected:	
	/*!
	Add keyframe interval to timeline.  This method is for internal use.
	\param t Position where to add interval.
	\param doesExist Returns true if interval already exist or false if interval does not exist.
	\return Position of the newly inserted interval.
 	*/	
	int AddKeyframeInternal(int t, bool *doesExist);	
	/*!
	For the given tick, this method returns a tick that has been adjusted to for the timeline.
	It takes into account factors such as start time and if the animation is looped.  
	\param tick The time in ticks to be adjusted.
	\return The newly adjusted time.
   	*/
	float					AdjustTickToTimeline(float tick);
	VArray< int >			m_pTimelineArray;				//!< This is for internal use only.	

 	HBhvAnimation*			m_pAnimation;					//!< This is for internal use only.			
	HBhvTimeline *			m_pInstancedTimeline;			//!< This is for internal use only.		

	float					m_StartTick;					//!< This is for internal use only.			
 	float					m_CurrentTick;					//!< This is for internal use only.			
	float					m_CurrentRelativeTick;				//!< This is for internal use only.	
 private:
};



#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif



