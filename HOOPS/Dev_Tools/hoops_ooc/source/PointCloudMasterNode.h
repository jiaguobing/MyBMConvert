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

#include "PointCloudCommon.h"


//////////////////////////////////////////////////////////////////////////


class PointCloudMasterNode {
public:
	PointCloudMasterNode (PointCloudWorld & pcw, unsigned int master_node_size = 0);
	~PointCloudMasterNode ();

	void ConcatenateNode (
		std::vector<char> & in_node_buffer,
		unsigned int & out_file_id,
		unsigned int & out_offset,
		unsigned int & out_num_bytes);

private:
	PointCloudMasterNode (PointCloudMasterNode const &); // disable
	void operator= (PointCloudMasterNode const &); // disable

private:
	PointCloudWorld & 	m_pcw;
	char 				m_buffer[OOC_BUFFER_CHUNK_SIZE];
	unsigned int 		m_bpos;
	unsigned int 		m_master_node_size;
	unsigned int 		m_fpos;
	unsigned int 		m_fid;
	FILE *				m_file;
};



