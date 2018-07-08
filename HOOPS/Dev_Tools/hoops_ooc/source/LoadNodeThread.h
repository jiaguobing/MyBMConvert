// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#pragma once


#include "hc.h"
#include "Thread.h"


//////////////////////////////////////////////////////////////////////////

# if !defined(__APPLE__) && defined(__GNUC__) && (__GNUC__ < 4 || (__GNUC__ == 4 && __GNUC_MINOR__ < 8))
#	define OLD_GCC
#	define OVERRIDE 
# else
#	define OVERRIDE override
# endif

#ifdef _WIN32
#	if _MSC_VER < 1700
#		pragma warning(disable: 4481)	// Language extension: Using override specifier
#	endif
#endif


//////////////////////////////////////////////////////////////////////////


class LoadNodeThread;
class NodeData;
class PointCloudIndexPopulateData;


//////////////////////////////////////////////////////////////////////////


enum LoadNodeThreadStage {
	TryingToObtainProxyListLock,
	NotTryingToObtainProxyListLock,
	LoadingANode,
	DoneRunning,
};


void LoadNode (HC_KEY proxy_key, NodeData & node_data);


class LoadNodeThread : public Thread {
public:
	LoadNodeThread (PointCloudIndexPopulateData & pcipd);
	void SendStopSignal ();

	LoadNodeThreadStage GetStage () const
	{
		return thread_stage;
	}

private:
	virtual void Run () OVERRIDE;

private:
	PointCloudIndexPopulateData & pcipd;
	LoadNodeThreadStage volatile thread_stage;
	bool volatile should_stop;
};








