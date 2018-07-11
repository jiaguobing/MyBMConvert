// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#include <math.h>
#include <string.h>

#include "HTools.h"

#include "HBaseModel.h"
#include "HBaseView.h"
#include "HBaseOperator.h"
#include "HEventInfo.h"
#include "HTCObjectRotate.h"
#include "HSelectionSet.h"
#include "HDB.h"



HTCObjectRotate::HTCObjectRotate(HBaseView* view, int DoRepeat,int DoCapture )
			: HBaseOperator(view, DoRepeat, DoCapture), HTClient()
{
	m_state = HTCOR_Resting;
	m_angle = 0;
	m_duration = 10;
	m_start_time = -1;
	m_segkey = -1;
	m_key = -1;
	m_original_matrix_valid = false;
}

HTCObjectRotate::~HTCObjectRotate()
{
	if( m_state != HTCOR_Resting ) {
		UnRegister();
	}
}


HTCObjectRotate::HTCObjectRotate( HTCObjectRotate *mommy )
: HBaseOperator(mommy->GetView(), mommy->Repeat(), mommy->Capture())
{
	m_state = mommy->m_state;
	m_angle = mommy->m_angle;
	m_duration = mommy->m_duration;
	m_start_time =mommy->m_start_time;
	m_segkey = mommy->m_segkey;
	m_key = mommy->m_key;
	m_original_matrix_valid = mommy->m_original_matrix_valid;
}


bool HTCObjectRotate::GetSelection( HEventInfo &event, HC_KEY *key ) 
{
	HPoint  new_pos;

	// Mouse went down; find the selected item
	HC_Open_Segment_By_Key(GetView()->GetConstructionKey());
		HC_Flush_Contents (".", "geometry");
	HC_Close_Segment();
	new_pos = event.GetMouseWindowPos();

	HC_Open_Segment_By_Key(GetView()->GetViewKey());
		// compute the selection using the HOOPS window coordinate ofthe the pick location
		if (HC_Compute_Selection(".","./scene/overwrite","v, selection level = entity", new_pos.x, new_pos.y) > 0)
			HC_Show_Selection_Element (key, 0, 0, 0);
		else
			*key = -1;
    HC_Close_Segment();
	return *key != -1;
}





bool HTCObjectRotate::SearchAndDestroyHTCOR( HC_KEY key ) 
{
	void *op = 0;

	char keytype[MVO_BUFFER_SIZE];

	HC_Show_Key_Type( key, keytype );
	if (!streq(keytype, "segment")) {
		key = HC_KShow_Owner_By_Key( key );
	}
	HC_Open_Segment_By_Key( key );
		HC_Show_One_Net_User_Index(HTC_OBJECT_ROTATE_MAGIC, &op);
	HC_Close_Segment();
	if( op ) {
		/* the parent already has an htcor_temp segment, meaning that it is currently
		 * in the process of an HTCObjectRotate.  Instead of kicking off a new animation,
		 * stop the other one.  The pointer to the object handling that other animation is
		 * tucked away in a user index */
		((HTCObjectRotate *)op)->UnRegister();
		return true;
	}
	return false;
}



void HTCObjectRotate::Animate( HC_KEY key )
{
	int count;
	HPoint min, max;
	char keytype[MVO_BUFFER_SIZE];

    m_state = HTCOR_Animating;
	m_angle = 0;
	if( key != -1 )
		m_key = key;

	while( IsSpecialKey( m_key )) {
		m_key = HC_KShow_Owner_By_Key( m_key );
	}
	HC_Show_Key_Type(m_key, keytype);
	if( !streq(keytype, "segment") ) {
		m_segkey = HC_KShow_Owner_By_Key( m_key );
		HC_Open_Segment_By_Key( m_segkey );
			//find out if it's safe to put modelling matrices in the parent
			HC_Begin_Contents_Search( ".", "geometry, subsegments" );
				HC_Show_Contents_Count( &count );
			HC_End_Contents_Search();
			if( count > 1 ) {
				m_segkey = HC_KOpen_Segment( "" );
					HC_Move_By_Key( m_key, "." );
				HC_Close_Segment();
			}
		HC_Close_Segment();
	}
	else {
		m_segkey = m_key;
	}
	HC_Open_Segment_By_Key( m_segkey );
		if( HC_Show_Existence ("modelling matrix") ) {
			m_original_matrix_valid = true;
			HC_Show_Modelling_Matrix( m_original_matrix );
		}
	HC_Close_Segment();

	// get centroid of the object (in object space units) 
	HC_Compute_Circumcuboid_By_Key (m_key, &min, &max);
	m_centroid.x = 0.5F * (min.x + max.x);
	m_centroid.y = 0.5F * (min.y + max.y);
	m_centroid.z = 0.5F * (min.z + max.z);

	HC_Open_Segment_By_Key(m_segkey);
		HC_Set_User_Index( HTC_OBJECT_ROTATE_MAGIC, this );
		HC_Open_Segment ("htcor_temp");
			HC_Set_Edge_Weight(2.0);
			HC_Set_Line_Weight(2.0);
			HC_Set_Color_By_Index ("edges", 4);
			HC_Set_Selectability("everything = off");
			HC_Set_Visibility("faces = off, markers = off, lights = off, cutting planes = off, edges = on"); 
			HUtility::InsertBox (&max, &min);
		HC_Close_Segment ();			  
	HC_Close_Segment();
	HC_Show_Time( &m_start_time );  

	SetInterval( 0.01f );
	SetStyle( HTCS_PeriodicSkip );
	SetNextRequest( m_start_time + 0.01f );
	m_cookie = GetView()->SetSignalNotify( HSignalDelete, HTCObjectRotate::signal_callback, this );
	HDB::GetHTManager()->RegisterClient( this );
	GetView()->Update();
}


int HTCObjectRotate::OnLButtonDown(HEventInfo &event)
{
	HC_KEY key;
	HTCObjectRotate *op;

	if( m_state == HTCOR_Animating ) {
		UnRegister();
		return HOP_OK;
	}

	if( !GetSelection( event, &key ) )
		return HOP_OK;

	if( SearchAndDestroyHTCOR( key ) ) 
		return HOP_OK;

	while( IsSpecialKey( key ) )
		key = HC_KShow_Owner_By_Key( key );

	op = new HTCObjectRotate(this);
	op->SetDeleteOnUnRegister();
	op->Animate( key );
    return HOP_OK;
}


/* return value indicates whether or not to schedule new ticks after this one is finished */
bool HTCObjectRotate::Tick( float request_time, float actual_time )
{
	HPoint axis;
	float halfway, elapsed, angle;

	if( request_time - m_start_time > m_duration ) {
	        HC_Open_Segment_By_Key(m_segkey);
		    HC_UnSet_Modelling_Matrix();
		    if( m_original_matrix_valid ) {
			    HC_Append_Modelling_Matrix( m_original_matrix );
		    }
	        HC_Close_Segment();

		UnRegister();
		return false;
	}

	// calculate the angle by which we would like to rotate
	elapsed = actual_time - m_start_time;
	halfway = m_start_time + m_duration/2.0f;

	angle = elapsed / m_duration;
        if( actual_time < halfway )
                angle *= angle * 2.0f;
        else {
		angle = 1.0f - angle;
                angle *= angle;
                angle = 1.0f - 2.0f * angle;
        }
	m_angle = angle * 3.0f * 360.0f;

	HC_Open_Segment_By_Key(m_segkey);
		axis.x = 1;
		axis.y = 1;
		axis.z = 1;
		HC_UnSet_Modelling_Matrix();
		RotateSegmentLocally ( m_segkey, &axis, &m_centroid, m_angle );
		if( m_original_matrix_valid ) {
			HC_Append_Modelling_Matrix( m_original_matrix );
		}
	HC_Close_Segment();
	GetView()->Update();
	return true;

} //end function Tick


//assumes that given key is a segment that has no local modelling matrix
void HTCObjectRotate::RotateSegmentLocally (
    HC_KEY		key,
    HPoint		*axis,
    HPoint		*centroid,
    float		theta)	
{
	char	handedness[16];

	HC_Open_Segment_By_Key(key);
		HC_Show_Net_Handedness(handedness);
		if (GetView()->GetHandedness() == HandednessRight || streq(handedness, "right")) {
			axis->x *= -1;
			axis->z *= -1;
		}
		//  Translate centroid of object to origin 
		HC_Translate_Object (-centroid->x, -centroid->y, -centroid->z);
		// Apply off-axis rotation. 
		HC_Rotate_Object_Offaxis (axis->x, axis->y, axis->z, theta);
		// Put centroid back where it was 
		HC_Translate_Object (centroid->x, centroid->y, centroid->z);
    HC_Close_Segment ();

    return;
}

void HTCObjectRotate::UnRegister()
{
	HDB::GetHTManager()->UnRegisterClient( this );
	m_state = HTCOR_Resting;
	if( GetView() != 0 ) {
		HC_Open_Segment_By_Key( m_segkey );
			HC_UnSet_One_User_Index( HTC_OBJECT_ROTATE_MAGIC );
			HC_Delete_Segment( "htcor_temp" );
		HC_Close_Segment();
		GetView()->UnSetSignalNotify( m_cookie );
		GetView()->Update();
	}
	m_segkey = -1;
	m_key = -1;
	if( m_delete_on_unregister )
		delete this;
}

bool HTCObjectRotate::OnSignal(int signal)
{
	switch( signal )	{
		case HSignalDelete:
			SetView(0);
			UnRegister();
			return true;
		default:
			return false;
	}
}

bool HTCObjectRotate::signal_callback(int signal, void *signal_data, void *user_data)
{
	UNREFERENCED(signal_data);

	return static_cast<HTCObjectRotate *>(user_data)->OnSignal(signal);
}

