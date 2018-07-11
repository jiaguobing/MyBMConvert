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

#include <stdlib.h>


//////////////////////////////////////////////////////////////////////////


#ifdef _WIN32
#	define OOC_API __declspec(dllexport)
#else
#	define OOC_API
#endif


//////////////////////////////////////////////////////////////////////////


class OOCT_World;


class OOC_API OOCT {
public:
	OOCT (char const * ooc_file_in);
	void PreProcess ();

private:
	OOCT_World * m_OOCT_World;
};



