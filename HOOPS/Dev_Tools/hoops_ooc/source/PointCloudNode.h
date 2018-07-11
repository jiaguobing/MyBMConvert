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

#ifndef _WIN32
#	include <unistd.h>
#endif

#include "BStream.h"
#include "BOpcodeHandler.h"
#include "PointCloudCommon.h"


//////////////////////////////////////////////////////////////////////////


class TK_Point_Cloud_Node;


namespace Point_Cloud_Node
{
	class RW_Point {
	public:
		RW_Point ()
			: stage(0)
		{}

		TK_Status Read (BStreamFileToolkit & tk, PCPoint & point);
		TK_Status Write (BStreamFileToolkit & tk, PCPoint const & point);

	private:
		int stage;
	};


	class RW_Points {
	public:
		RW_Points ()
			: curr_pos(0)
		{}

		TK_Status Read (BStreamFileToolkit & tk, TK_Point_Cloud_Node & handler);
		TK_Status Write (BStreamFileToolkit & tk, TK_Point_Cloud_Node const & handler);

	private:
		int curr_pos;
		RW_Point rw_point;
	};
}


class TK_Point_Cloud_Node: public TK_User_Data {
private:
	friend class Point_Cloud_Node::RW_Points;

public:
	TK_Point_Cloud_Node () {}
	~TK_Point_Cloud_Node () {}
	
	TK_Status Read (BStreamFileToolkit & tk);
	TK_Status Execute (BStreamFileToolkit & tk);
	
	TK_Status Write (BStreamFileToolkit & tk);
	TK_Status Interpret (BStreamFileToolkit & tk, ID_Key key, int variant = 0);
	
	void Reset ();

	void SetPointList (int num_points_in, PCPoint * point_list_in)
	{
		m_num_points = num_points_in;
		m_point_list = point_list_in;
	}

private:
	PCPoint *					m_point_list;
	int							m_num_points;
	Point_Cloud_Node::RW_Points	rw_points;
};



