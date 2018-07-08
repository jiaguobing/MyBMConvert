// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef CACHE_MEMORY_OBJECT_H
#define CACHE_MEMORY_OBJECT_H

#include <stdlib.h>

#define FREE_LIST_SIZE 32

class CMObject {
public:
	CMObject ( void * p_in = 0, size_t size_in = 0 ) : p(p_in), size(size_in) {};

	~CMObject () {};

	void *p;
	size_t size;
};

class CacheMemoryObject {
public:
	CacheMemoryObject ();
	virtual ~CacheMemoryObject ();

	static void * operator new ( size_t size );

 	static void operator delete ( void * p, size_t size ) throw ();

private:
	static void * free_list_small[8][FREE_LIST_SIZE];
	static int cursor_small[8];
	static CMObject free_list_large[FREE_LIST_SIZE];
	static int cursor_large;
	static int ref_count;
};


#endif
