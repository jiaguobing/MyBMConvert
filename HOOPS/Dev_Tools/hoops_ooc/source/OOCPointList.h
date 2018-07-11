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

#include <list>


//////////////////////////////////////////////////////////////////////////


class BackingFileData;
class LRUList;
class LRUListNode;
class PCPoint;
class PointCloudWorld;
class StreamFilePointContainer;
class TempFileToolkit;


template <typename T> class BSPItem;

typedef std::list<BackingFileData *> BFDList;


class OOCPointList {
public:
	OOCPointList (PointCloudWorld & pcw, StreamFilePointContainer * container_in);
	~OOCPointList ();

	bool Append (PCPoint * new_point, bool should_compare = false);
	
	void SetLRUNode (LRUListNode * lru_node_in)
	{
		m_lru_node = lru_node_in;
	}

	LRUListNode * GetLRUNode ()
	{
		return m_lru_node;
	}
	
	PCPoint * operator [] (int const location);

	void GetPointList (int & count_out, PCPoint *& point_list_out);

private:
	void EnsureSize ();
	void EnsurePointsInMemory (bool allow_appending = false);
	void CacheLRULists ();

private:
	OOCPointList (OOCPointList const &); // disable
	void operator= (OOCPointList const &); // disable

private:
	PointCloudWorld &			m_pcw;
	StreamFilePointContainer *	m_container;
	PCPoint *					m_point_list;
	int							m_total_count;
	int							m_current_count;
	int							m_size;
	bool						m_can_cache;
	LRUListNode *				m_lru_node;

	BFDList						m_bfd_list;

	static LRUList *			s_lru_point_list_list;
	static unsigned long		s_ref_count;
	static TempFileToolkit *	s_temp_file_tk;
};



