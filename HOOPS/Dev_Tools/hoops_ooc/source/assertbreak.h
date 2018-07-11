// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#pragma once


//////////////////////////////////////////////////////////////////////////


#ifdef _MSC_VER
#	pragma warning(disable: 4127) // conditional expression is constant
#endif


//////////////////////////////////////////////////////////////////////////


#ifndef ASSERT
#	ifdef NDEBUG
#		define ASSERT(x)		do {} while (false)
#	else
#if _MSC_VER
#		define ASSERT(x)		((!(x)) ? __debugbreak() : 1)
#else
#		include <assert.h>
#		define ASSERT			assert
#endif
#	endif
#endif


#if _MSC_VER
#	define INLINE		__forceinline
#else
#	define INLINE		inline
#endif



