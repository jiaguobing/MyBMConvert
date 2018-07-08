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

#include "PointCloudAPI.h"

#include "HOpcodeShell.h"

#include <list>
#include <stdlib.h>
#include <unordered_map>


//////////////////////////////////////////////////////////////////////////


class OOCTStreamFileToolkit;


class OOCT_Instance
{
public:
	OOCT_Instance ()
		: m_key(HC_ERROR_KEY)
		, m_id(-1)
		, m_file_location(-1)
	{}

public:
	long m_file_location;
	HC_KEY m_key;
	long m_id;
	long m_last_id;
	ooc::Point m_min;
	ooc::Point m_max;
};


class OOCT_Entity
{
public:
	OOCT_Entity ()
		: m_key(HC_ERROR_KEY)
		, m_instance(nullptr)
	{}

public:
	OOCT_Instance * m_instance;
	HC_KEY m_key;
	float m_diameter;
	ooc::Point m_center;
};


class IdKeyPair
{
public:
	IdKeyPair (HC_KEY key, long id)
		: m_key(key)
		, m_id(id)
		, m_last_id(id)
		, m_is_instance(false)
	{}

public:
	HC_KEY m_key;
	long m_id;
	long m_last_id;
	bool m_is_instance;
};


class OOCT_KDTreeItem
{
public:
	OOCT_KDTreeItem (int partition_type);

	void AppendItem (OOCT_Entity const * item);

	bool HasValidBoundings () const
	{
		return m_min.x <= m_max.x;
	}

public:
	OOCT_KDTreeItem * kids[4];
	int m_partition_type;
	std::list<OOCT_Entity const *> m_item_list;
	ooc::Point m_min;
	ooc::Point m_max;
};


class OOCT_KDTree {
public:
	OOCT_KDTree (int max_depth, ooc::Point min, ooc::Point max);

	void InsertItem (OOCT_Entity const * entity, ooc::Point const & entity_min, ooc::Point const & entity_max, bool test_run);
	void CleanTree ();

private:
	void CleanTreeRecursive (OOCT_KDTreeItem * subtree);
	void CleanTreeRecursive2 (OOCT_KDTreeItem * subtree, OOCT_KDTreeItem * parent, int num);

	void InsertItemRecursive (
		OOCT_KDTreeItem * subtree,
		OOCT_Entity const * entity,
		ooc::Point & min,
		ooc::Point & max,
		ooc::Point const & entity_min,
		ooc::Point const & entity_max,
		int max_depth,
		int remaining_dimensions,
		bool test_run);

private:
	ooc::Point			m_min;
	ooc::Point			m_max;
	int					m_max_depth;

public:
	OOCT_KDTreeItem *	m_root;
};


class OOCT_World {
private:
	~OOCT_World (); // unimplemented

public:
	OOCT_World ();

	// Sets up various variables for pre-processing and/or loading the point cloud
	void Initialize (wchar_t const * ooct_file_in);
	void PreProcess ();

	void AssociateKey (HC_KEY key);
	void SerializeInstances ();

private:
	IdKeyPair * GetIdKeyPair (HC_KEY key);
	void IdentifySegmentsRecursive (HC_KEY key, int level);
	void UpdateLastCountRecursive (HC_KEY key);

	void CreateAtomicInstance (IdKeyPair * id_key, ooc::Point & min, ooc::Point & max);
	void CreateAtomicEntity (IdKeyPair const * id_key, float const * matrix, char const * color);

	IdKeyPair * GetIdKeyPairFromId (long id);
	
	OOCT_Instance * GetInstance (long counter);

	void QuickFindLastRecursive (HC_KEY key, long & last);

	void SerializeKDTree ();
	void SerializeKDTreeRecursive (OOCT_KDTreeItem * subtree, int item_id);

private:
	long							m_id;
	int								m_item_id_count;

	wchar_t *						m_ooct_file_name;
	wchar_t * 						m_ooct_file_dir;
	size_t							m_ooct_file_dir_len;
	wchar_t *						m_node_dir_prefix;
	size_t							m_node_dir_prefix_len;

	OOCTStreamFileToolkit *			m_stream_toolkit;
	HStreamFileToolkit *			m_stream_toolkit_readwrite;

	std::unordered_map<HC_KEY, IdKeyPair *>		m_key_to_id_key_pair_hash;
	std::unordered_map<long, IdKeyPair *>		m_id_to_id_key_pair_hash;
	std::unordered_map<long, OOCT_Instance *>	m_id_to_instance_hash;

	std::list<OOCT_Entity *>		m_atomic_entity_list;
	std::list<OOCT_Instance *>		m_atomic_instances_list;
	OOCT_Instance **				m_sorted_instance_array;

	size_t							m_current_serialization_position;
	FILE *							m_instance_file_handle;
	FILE *							m_kd_tree_file_handle;
	FILE *							m_kd_tree_file_map_handle;
	long							m_current_file_location;
	OOCT_KDTree *					m_kd_tree;
	int								m_number_of_kd_nodes;
	HC_KEY							m_path[256];
};


class TK_OOCT_Shell : public HTK_Shell {
public:
	TK_OOCT_Shell (OOCT_World * ooct_world)
		: HTK_Shell()
		, m_ooct_world(ooct_world)
	{}

	TK_Status Execute (BStreamFileToolkit & tk);

private:
	OOCT_World * m_ooct_world;
};


class TK_OOCT_Shell2 : public HTK_Shell {
public:
	TK_OOCT_Shell2 (OOCT_World * ooct_world)
		: HTK_Shell()
		, m_ooct_world(ooct_world)
	{}

	TK_Status Execute (BStreamFileToolkit & tk);

private:
	OOCT_World * m_ooct_world;
};


class TK_OOCT_Line : public HTK_Line {
public:
	TK_Status Execute (BStreamFileToolkit & tk)
	{
		UNREFERENCED(tk);
		return TK_Normal;
	}
};


class TK_OOCT_Polypoint : public HTK_Polypoint {
public:
	TK_OOCT_Polypoint (unsigned char opcode)
		: HTK_Polypoint(opcode)
	{}

	TK_Status Execute (BStreamFileToolkit & tk)
	{
		UNREFERENCED(tk);
		return TK_Normal;
	}
};


class TK_OOCT_Polygon : public TK_OOCT_Polypoint {
public:
	TK_OOCT_Polygon ()
		: TK_OOCT_Polypoint(TKE_Polygon)
	{}
};


class TK_OOCT_Polyline : public TK_OOCT_Polypoint {
public:
	TK_OOCT_Polyline ()
		: TK_OOCT_Polypoint(TKE_Polyline)
	{}
};


class TK_OOCT_Ellipse_Prim : public HTK_Ellipse {
public:
	TK_OOCT_Ellipse_Prim (unsigned char opcode)
		: HTK_Ellipse(opcode)
	{}

	TK_Status Execute (BStreamFileToolkit & tk)
	{
		UNREFERENCED(tk);
		return TK_Normal;
	}
};


class TK_OOCT_Ellipse : public TK_OOCT_Ellipse_Prim {
public:
	TK_OOCT_Ellipse ()
		: TK_OOCT_Ellipse_Prim(TKE_Ellipse)
	{}
};


class TK_OOCT_Elliptical_Arc : public TK_OOCT_Ellipse_Prim {
public:
	TK_OOCT_Elliptical_Arc ()
		: TK_OOCT_Ellipse_Prim(TKE_Elliptical_Arc)
	{}
};


class TK_OOCT_Text_Prim : public HTK_Text {
public:
	TK_OOCT_Text_Prim (unsigned char opcode)
		: HTK_Text(opcode)
	{}

	TK_Status Execute (BStreamFileToolkit & tk)
	{
		UNREFERENCED(tk);
		return TK_Normal;
	}
};


class TK_OOCT_Text : public TK_OOCT_Text_Prim {
public:
	TK_OOCT_Text ()
		: TK_OOCT_Text_Prim(TKE_Text)
	{}
};


class TK_OOCT_Text_With_Encoding : public TK_OOCT_Text_Prim {
public:
	TK_OOCT_Text_With_Encoding ()
		: TK_OOCT_Text_Prim(TKE_Text_With_Encoding)
	{}
};


class TK_OOCT_Point : public HTK_Point {
public:
	TK_OOCT_Point ()
		: HTK_Point(TKE_Marker)
	{}

	TK_Status   Execute (BStreamFileToolkit & tk)
	{
		UNREFERENCED(tk);
		return TK_Normal;
	}
};


class TK_OOCT_Open_Segment : public HTK_Open_Segment {
public:
	TK_OOCT_Open_Segment (OOCT_World * ooct_world)
		: HTK_Open_Segment()
		, m_ooct_world(ooct_world)
	{}

	TK_Status Execute (BStreamFileToolkit & tk)
	{
		HTK_Open_Segment::Execute(tk);
		m_ooct_world->AssociateKey(last_key(tk));
		return TK_Normal;
	}

private:
	OOCT_World * m_ooct_world;
};


class TK_OOCT_Close_Segment : public HTK_Close_Segment {
public:
	TK_OOCT_Close_Segment (OOCT_World * ooct_world)
		: HTK_Close_Segment()
		, m_ooct_world(ooct_world)
	{}

	TK_Status Execute (BStreamFileToolkit & tk)
	{
		HTK_Close_Segment::Execute(tk);
		m_ooct_world->AssociateKey(last_key(tk));
		return TK_Normal;
	}

private:
	OOCT_World * m_ooct_world;
};


class TK_OOCT_Open_Segment2 : public HTK_Open_Segment {
public:
	TK_OOCT_Open_Segment2 (OOCT_World * ooct_world)
		: HTK_Open_Segment()
		, m_ooct_world(ooct_world)
	{}

	TK_Status Execute (BStreamFileToolkit & tk)
	{
		HTK_Open_Segment::Execute(tk);
		char text[4096];
		HC_Show_Segment(last_key(tk), text);
		m_ooct_world->AssociateKey(last_key(tk));
		m_ooct_world->SerializeInstances();
		return TK_Normal;
	}

private:
	OOCT_World * m_ooct_world;
};


class TK_OOCT_Close_Segment2 : public HTK_Close_Segment {
public:
	TK_OOCT_Close_Segment2 (OOCT_World * ooct_world)
		: HTK_Close_Segment()
		, m_ooct_world(ooct_world)
	{}

	TK_Status   Execute (BStreamFileToolkit & tk)
	{
		HTK_Close_Segment::Execute(tk);
		char text[4096];
		HC_Show_Segment(last_key(tk), text);
		m_ooct_world->AssociateKey(last_key(tk));
		m_ooct_world->SerializeInstances();
		return TK_Normal;
	}

private:
	OOCT_World * m_ooct_world;
};


class OOCTStreamFileToolkit : public HStreamFileToolkit {
public:
	OOCTStreamFileToolkit () {}

	void InitHandlers (OOCT_World * ooct_world)
	{
		SetOpcodeHandler(new TK_OOCT_Shell(ooct_world));
		SetOpcodeHandler(new TK_OOCT_Line());
		SetOpcodeHandler(new TK_OOCT_Polyline());
		SetOpcodeHandler(new TK_OOCT_Ellipse());
		SetOpcodeHandler(new TK_OOCT_Elliptical_Arc());
		SetOpcodeHandler(new TK_OOCT_Text());
		SetOpcodeHandler(new TK_OOCT_Text_With_Encoding());
		SetOpcodeHandler(new TK_OOCT_Point());
		SetOpcodeHandler(new TK_OOCT_Polygon());
		SetOpcodeHandler(new TK_OOCT_Open_Segment(ooct_world));
		SetOpcodeHandler(new TK_OOCT_Close_Segment(ooct_world));
	}

private:
	void SetOpcodeHandler (BBaseOpcodeHandler * handler)
	{
		HStreamFileToolkit::SetOpcodeHandler(handler->Opcode(), handler);
	}
};





