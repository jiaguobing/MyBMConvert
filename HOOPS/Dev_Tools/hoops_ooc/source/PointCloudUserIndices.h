// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#pragma once

#ifndef OOC_LIB_BUILD
#	error "Do not include this file."
#endif


//////////////////////////////////////////////////////////////////////////


namespace ooc
{
	enum UserIndices : long {
		UserIndices_Base_Index_Helper = -0x720185E7, // Engineered to be negative, fit in 4 bytes, and "random".

		UserIndices_Seg_Key_To_Node_Data_Ptr,
		UserIndices_Callbacks_Defined,
		UserIndices_Point_Cloud_Instance_Ptr,
	};
}




