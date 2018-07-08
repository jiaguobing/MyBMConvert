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

#include "BStream.h"
#include "BOpcodeShell.h"
#include "NodeDelta.h"


//////////////////////////////////////////////////////////////////////////


class PointCloudIndexNodeData;
class PointCloudStreamFileToolkit;


class TK_Point_Cloud_Node_Randomized : public TK_Shell {
public:
	TK_Point_Cloud_Node_Randomized (PointCloudIndexNodeData * idx_node_data_in = nullptr);
	~TK_Point_Cloud_Node_Randomized ();
	TK_Status Execute (BStreamFileToolkit & tk);
	TK_Status Interpret (BStreamFileToolkit & tk, ID_Key key, int variant = 0);

private:
	TK_Status FilterPoints (PointCloudStreamFileToolkit & ptk, bool & reject_entire_shell);
	void FilterAcceptedPoints (ooc::delta::in_memory::AcceptResult const * results, size_t count);

private:
	PointCloudIndexNodeData *	m_idx_node_data;
};



