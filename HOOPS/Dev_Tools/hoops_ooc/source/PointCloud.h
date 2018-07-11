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

#include "Misc.h"
#include "PointCloudAPI.h"
#include "PointCloudCommon.h"

#include "BStream.h"

#include <memory>
#include <stdlib.h>


//////////////////////////////////////////////////////////////////////////


class PointCloud {
	friend PointCloudWorld & ooc::get_point_cloud_world (ooc::Env);
	friend class ooc::preprocess::Preprocessor;

public:

	// Sets the OOC file which will either be:
	//   -- generated if doing a pre-process
	//   -- rendered if doing a load
	PointCloud (wchar_t const * ooc_file, std::unique_ptr<ooc::delta::in_memory::NodeDeltas> node_deltas);

	~PointCloud ();

	static bool FreePointCloudData (int signal, void * signal_data, void * user_data);
	static bool UpdateCheck (float request_time, float actual_time, void * user_data);
	static ooc::io::IOResult FileInputByKey (wchar_t const * filename, HC_KEY key, HInputHandlerOptions & options);

	// Sets up segment tree for reading in OOC point cloud 
	ooc::LoadStatus Load ();

	void SetLoadType (ooc::LoadType load_type_in);
	ooc::LoadType GetLoadType () const;

	bool PollNodesLoadingOrHaveBeenLoaded ();

	H_UTF8 GenerateDeltaFilename ();
	ooc::io::IOResult CommitDeltasToFile (char * buffer, int buffer_size, H_UTF8 const & file);
	void SetParentDeltaFile (H_UTF8 const & delta_file);
	void AddDeltaFile (H_UTF8 const & delta_file, uint32_t delta_file_id);

private:
	PointCloudWorld	m_pcw;
	time_t			m_delta_filename_time;
	size_t			m_delta_filename_counter;
};





