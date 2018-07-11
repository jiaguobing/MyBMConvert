// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef _HDEBUGZBUFFER_H
#define _HDEBUGZBUFFER_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"
#include "hic_types.h"

class HBaseView;


/*! HDebugZBuffer is a intended for debugging purposes only.  When activated, it pops up a 
	standalone HOOPS window in which it displays the contents of the view's z buffer.  It is pretty
	rough, but does the job.  The z values are scanned for min and max, with background z values
	excluded.  The values are then mapped to a linear grey scale ramp with black at the closest value
	and white at the furthest.  The image is also upside down. */
class MVO_API HDebugZBuffer 
{
protected:

    HC_KEY			m_ViewKey;					//!< self-explanatory
	HC_KEY			m_StandaloneKey;			//!< key to the driver relevant to the standalone window
	int				m_width, m_height;			//!< width and height of the view (and thus the amount of scratch space required)
	char			*m_StandaloneSegmentName;	//!< path to the standalone window

	float			*m_FloatBuffer;				//!< scratch space for z values in range [0..1]
	unsigned char	*m_GreyBuffer;				//!< scratch space for output image
	HC_KEY			m_GreyImageKey;				//!< key to the Image_By_Ref that represents the visualization

public:

    /*! Construct an HDebugZBuffer object.
	\param view A pointer the the HBaseView object you want to associate with the object.
	*/
    HDebugZBuffer( HBaseView *view );
    /*! The destructor. */
    virtual ~HDebugZBuffer();
	/*! This method captures the z buffer and converts it to a grey scale image. */
	void OpenglFinishPicture(HIC_Rendition const *nr, bool swap_buffers );
	/*! This method forces a full update in the standalone window. */
	void Update(); 

	/*! A standard routine to associate callback names with their function pointers 
		though setting the callbacks comes later. */
	static void RegisterCallbacks();
	/*! A callback for finish picture to capture the z buffer.  It simply calls through to OpenglFinishPicture/ */
    static void ogl_finish_picture(HIC_Rendition const *nr, bool swap_buffers );
};


#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif



