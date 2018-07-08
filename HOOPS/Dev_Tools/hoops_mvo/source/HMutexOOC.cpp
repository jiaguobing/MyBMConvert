// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#include "HMutexOOC.h"


//////////////////////////////////////////////////////////////////////////


#ifdef _WIN32
#	pragma warning(disable: 4127)
#endif


#ifndef HMFC_STATIC_LIB
#	define HMUTEX_IMPLEMENT_MUTEX_FUNCTIONS
#endif
#include "hmutex.h"


//////////////////////////////////////////////////////////////////////////


namespace
{
	template<class T, class U>
	struct is_same {
		enum { value = 0 };
	};

	template<class T>
	struct is_same<T, T> {
		enum { value = 1 };
	};
}


HMutexOOC::HMutexOOC ()
{
	static_assert(is_same<void *, HMutex>::value, "Code relies on (HMutex) being a typedef of (void *).");

	CREATE_MUTEX(opaque_mutex);
}


HMutexOOC::~HMutexOOC ()
{
	DESTROY_MUTEX(opaque_mutex);
}


void HMutexOOC::Lock ()
{
	LOCK_MUTEX(opaque_mutex);
}


void HMutexOOC::Unlock ()
{
	UNLOCK_MUTEX(opaque_mutex);
}



