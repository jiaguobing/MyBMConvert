// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef BSTREAM_MEMORY
#define BSTREAM_MEMORY

#ifdef HPS_CORE_BUILD
#	include "hoops.h"
#	define BSTREAM_NEW(type)					NEW(type)
#	define BSTREAM_ALLOC(p, type)				ALLOC(p, type)
#	define BSTREAM_ALLOC_ARRAY(p, count, type)	ALLOC_ARRAY(p, count, type)
#	define BSTREAM_FREE(p, type) do { \
			if (p != null) \
				FREE(p, type); \
			} while (0)
#	define BSTREAM_FREE_ARRAY(p, count, type) do { \
			if (p != null) \
				FREE_ARRAY(p, count, type); \
			} while (0)
#	define BSTREAM_NEW_VLIST(p)					p = new_vlist(DEFAULT_MEMORY_POOL())
#	define BSTREAM_NEW_VHASH(p, size)			p = new_vhash(size, DEFAULT_MEMORY_POOL())
#else
#	define BSTREAM_NEW(type)					new type
#	define BSTREAM_ALLOC(p, type)				p = new type
#	define BSTREAM_ALLOC_ARRAY(p, count, type)	p = new type [count]
#	define BSTREAM_FREE(p, type)				delete p
#	define BSTREAM_FREE_ARRAY(p, count, type)	delete [] p
#	define BSTREAM_NEW_VLIST(p)					p = new_vlist(malloc, free)
#	define BSTREAM_NEW_VHASH(p, size)			p = new_vhash(size, malloc, free)
#endif

#	define BSTREAM_DELETE(p)					delete p

#endif // BSTREAM_MEMORY
