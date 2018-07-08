// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef _H_HTCOR_H
#define _H_HTCOR_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif


#include "HTools.h"
#include "HBaseOperator.h"
#include "HTManager.h"

/*! an enumerated type to keep track of what the HTCObjectRotate is currently doing */
enum HTCOR_State {
	HTCOR_Resting,
	HTCOR_Animating
};


/*! HTCObjectRotate is a reference implementation of a timer client.  It is the first of what we hope will be a long list of objects to make use of the centralized
timer service in MVO provided by the HTManager class.  It is not necessarily meant to do anything useful except serve as an example of how it is possible to do animation
in the context of MVO without completely  */ 
class MVO_API HTCObjectRotate : public HBaseOperator, public HTClient
{
public:
	/*! constructor */
	HTCObjectRotate (HBaseView* view, int DoRepeat=0, int DoCapture=1);
	/*! constructor that accepts pointer to a parent HTCObjectRotate object */
	HTCObjectRotate( HTCObjectRotate *mommy );
	~HTCObjectRotate();
	int OnLButtonDown (HEventInfo &hevent);								/*!< Overrides HBaseOperator */
	bool Tick( float request_time, float actual_time );					/*!< Overrides HTClient. */
	void SetDeleteOnUnRegister() { m_delete_on_unregister = true; } 	/*!< Makes HTCObjectRotate clean up after itself, as opposed to having a delete call required after everything is finished */
	void UnRegister ();													/*!< Kills the animation (and possibly deletes *this, depending on the status of m_delete_on_unregister, as described above). */
	bool OnSignal( int signal );										/*!< Handles signals delivered from other objects */

protected:
	int m_state;														/*!< An identifier to describe the current state of the animation. Legal values are HTCOR_Resting and HTCOR_Animating. */
	float m_angle;														/*!< The current rotation angle */ 
	float m_duration;													/*!< The length of the animation */
	float m_start_time;													/*!< The time at which the animation started, as reported by HC_Show_Time */
	HC_KEY m_key;														/*!< The object getting rotated.  Can be geometry or segment. */
	HC_KEY m_segkey;													/*!< The key of the object's owner segment, into which modelling matrices are put (or possibly the same as m_key, if m_key itself refers to a segment) */
	HPoint m_centroid;													/*!< Centroid of segment to be rotated in object space */
	bool m_delete_on_unregister;										/*!< If true, object will delete(this) upon UnRegister */
	bool m_original_matrix_valid;										/*!< If true, m_segkey contained a modelling matrix at the start that needs preserving */
	float m_original_matrix[16];										/*!< The original modelling matrix in the m_segkey segment, if applicable. Left uninitialized if !m_original_matrix_valid */
	int m_cookie;														/*!< The key that needs to be passed to HBaseView::UnSetEventNotify() */

private:
	/*! Adds a modelling matrix to rotate the given segment about the given axis and centroid by an angle of theta
		\param key must refer to a segment 
		\param axis Pointer to an HPoint object denoting the axis about which to rotate
		\param centroid Pointer to an HPoint object denoting the centroid about which to rotate
		\param theta amount to rotate in degrees */
	void RotateSegmentLocally ( HC_KEY key, HPoint *axis, HPoint *centroid, float theta );
	/*! Given the screen location described as part of event, get the most relevant object in the HOOPS scene graph
		\param hevent Reference to an HEventInfo object
		\param key is the most relevant scene graph object.  Returned to user 
		\return true if a relevant selection has been found */
	bool GetSelection( HEventInfo &hevent, HC_KEY *key );
	/*! Initiates the Animation. If the object described is geometry and has siblings in its current 
		segment, it will be moved into a subsegment into which it is safe to append modelling matrices.  
		In all other cases, however, segment structure is not changed. \param key the segment to animate */
	void Animate( HC_KEY key );
	/*! Detects if the given key is currently undergoing an HTCObjectRotate animation.  If it is, the managing operator is UnRegistered (causing the animation to stop).
		\return true if a relevant animation was detected */
	bool SearchAndDestroyHTCOR( HC_KEY key );
	/*! A static function that can be registered as a callback, with a void * suitable for dispatching to a 
		particular object instance.  Simply returns user_data->OnEvent \param hevent the type of the event getting
		passed in.  \param user_data a "this" pointer passed back through the callback so that we may find our way
		to the class instance whose callback has been hit.  */
    static bool signal_callback( int hevent, void * signal_data, void *user_data );
};


#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif




