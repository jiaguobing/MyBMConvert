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

#include "vbsp.h"
#include "PointCloudCommon.h"


//////////////////////////////////////////////////////////////////////////


class OOCPointList;


class PointBSPItem : public BSPItem<PCPoint *> {
public:
	PointBSPItem (PCPoint * cpt_in = nullptr);
	
	virtual ~PointBSPItem () {}
	
	virtual PCPoint * GetItem ()  OVERRIDE
	{
		return m_cpt;
	}

	PCPoint const * GetItem () const
	{
		return m_cpt;
	}

	virtual float const * GetItemBounds ()  OVERRIDE
	{
		return m_bounds;
	}
	
	void Initialize (PCPoint * cpt_in);
	
private:
	PCPoint * 	m_cpt;
	float		m_bounds[6];
};


class StreamFilePointContainer : public BSPNodeItemContainer<PCPoint *> {
public:
	StreamFilePointContainer (float const bbox_in[6], int depth_in, void * container_data_in);

	virtual ~StreamFilePointContainer ();
	
	virtual bool AddItem (PCPoint * item, float const item_bounds_in[6] = nullptr)  OVERRIDE;

	virtual int Count ()  OVERRIDE
	{
		return m_num_points;
	}
	
	class iterator {
	public:
		iterator (OOCPointList * ooc_point_list_in = nullptr, int index_in = 0);
		
		iterator operator++ ();
		iterator operator++ (int);
		
		bool operator== (iterator const & rhs);
		bool operator!= (iterator const & rhs);
		
		PointBSPItem * operator* ();
		
		iterator operator= (iterator rhs);
		
	private:
		OOCPointList * 	m_ooc_point_list;
		int				m_index;
		PointBSPItem 	m_cpt_bsp_item;
	};
	
	iterator begin ();
	iterator end ();

	void GetPointList (int & count_out, PCPoint *& point_list_out);

	void DeletePointList ();
	
	void GetBBox (float * bbox_out) const
	{
		memcpy(bbox_out, this->m_bbox, 6 * sizeof(float));
	}

private:
	StreamFilePointContainer (StreamFilePointContainer const &); // disable
	void operator= (StreamFilePointContainer const &); // disable
	
private:
	float				m_bbox[6];
	int					m_depth;
	PointCloudWorld & 	m_pcw;
	OOCPointList *		m_ooc_point_list;
	int					m_num_points;
	bool				m_should_compare;
};



