// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#pragma once


#include "assertbreak.h"
#include "hmutex.h"


//////////////////////////////////////////////////////////////////////////
#ifndef _WIN32
#	define override
#endif


class Thread {
public:
	Thread ();
	virtual ~Thread ();

	void Start ();

	bool IsRunning () const
	{
		return running;
	}

private:
	virtual void Run () = 0;
	static void StaticRun (Thread & thread_obj);

private:
	Thread (Thread &&); // disable
	Thread (Thread const &); // disable
	void operator= (Thread &&); // disable
	void operator= (Thread const &); // disable

private:
	HThreadHandle handle;
	bool volatile running;
};











