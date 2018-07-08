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

#include "hmutex.h"

#include "BStream.h"
#include "BOpcodeHandler.h"

#include "LoadNodeThread.h"
#include "PointCloudCommon.h"
#include "PointCloudStreamFileToolkit.h"

#include "vbsp.h"

#include "hc.h"
#include "hic.h"


//////////////////////////////////////////////////////////////////////////


int const LOD_POINT_COUNT_CUTOFF = 500;


class PointCloudMasterNode;
class PointCloudStreamFileToolkit;
class StreamFilePointContainer;


typedef BSPNodeMetadata<PCPoint *, StreamFilePointContainer> BSPMetadata;
typedef std::vector<float> BBoxList;
typedef std::vector<PCPoint> PointList;
typedef std::vector<TreeTraversal> SegmentList;
typedef std::vector<unsigned int> MasterNodeDataList;
typedef std::list<int> PropagationList;
typedef std::list<PointList *> PointListList;


class PointCloudIndexNodeData {
public:
	PointCloudIndexNodeData (PointCloudWorld & pcw, float min_intensity_in, float max_intensity_in);
	~PointCloudIndexNodeData ();

	void SetContainer (StreamFilePointContainer * container_in)
	{
		m_container = container_in;
	}
	
	void SetBBox (float const bbox_in[6])
	{
		memcpy(m_bbox, bbox_in, 6 * sizeof(float));
	}

	void GetBBox (float bbox_out[6]) const
	{
		memcpy(bbox_out, m_bbox, 6 * sizeof(float));
	}

	void GetMasterNodeData (unsigned int & fid_out, unsigned int & offset_out, unsigned int & num_bytes_out) const
	{
		fid_out = m_fid;
		offset_out = m_offset;
		num_bytes_out = m_num_bytes;
	}
	
	bool GetIsEnteringNode () const
	{
		return (m_traversal_state % 2) == 0;
	}
	
	TreeTraversal GetTreeTraversalState () const
	{
		return m_traversal_state;
	}

	void SetTreeTraversalState (TreeTraversal traversal_state_in)
	{
		m_traversal_state = traversal_state_in;
	}

	void SetBSPNodeMetadata (BSPMetadata * node_metadata_in)
	{
		m_node_metadata = node_metadata_in;
	}

	void Update ();
	void ExportPointCloudIndex ();
	
	PointList const & GetNodePointList () const
	{
		return m_node_point_list;
	}

	float GetMinIntensity () const
	{
		return m_min_intensity;
	}

	float GetMaxIntensity () const
	{
		return m_max_intensity;
	}

private:
	inline uint_fast32_t Unirand ()
	{
		return s_uniform_rand[++m_rand48_seed & 0x3FF];
	}

	inline uint_fast32_t UnirandOneHalf () const
	{
		return 500000;
	}

	inline uint_fast32_t UnirandOne () const
	{
		return 1000000;
	}

private:
	PointCloudIndexNodeData (PointCloudIndexNodeData const &); // disable
	void operator= (PointCloudIndexNodeData const &); // disable

private:
	PointCloudWorld &				m_pcw;
	wchar_t *						m_index_file_name;
	StreamFilePointContainer * 		m_container;				// the container we are currently working with (possibly null)
	float							m_bbox[6];					// the bounding box we are currently working with
	unsigned int					m_fid;						// file id of current node
	unsigned int					m_offset;					// file offset for current node
	unsigned int					m_num_bytes;				// size of file for current node
	PointCloudStreamFileToolkit * 	m_index_tk;					// toolkit for writing out the index
	PointCloudStreamFileToolkit * 	m_node_tk;					// toolkit for writing out the nodes
	TreeTraversal					m_traversal_state;			// traversal state when walking BSP
	BSPMetadata *					m_node_metadata;			// BSP node metadata
	int								m_node_shell_size;			// number of points to (try to) have at each node
	int								m_node_shell_cutoff;		// minimum number of points to have in a node, otherwise, collapse into parent
	PointListList					m_lod_point_list_list;		// list of lod point lists (one per parent node)
	PointList 						m_node_point_list;			// list of points that are living in a particular node
	PropagationList					m_propagation_amount;		// number of points to propagate upward
	SegmentList						m_segment_list;				// list of segments in OOC file
	BBoxList						m_bbox_list;				// list of bboxes for external references
	MasterNodeDataList				m_mnd_list;					// list of file id's, file offsets and file sizes for each node
	std::vector<char>				m_node_buffer;
	float							m_min_intensity;
	float							m_max_intensity;

	int								m_rand48_seed;
	static uint_fast32_t			s_uniform_rand[1024];

	uint64_t						m_nodes_processed;
	uint64_t						m_num_files_written;

	PointCloudMasterNode *			m_pcmn;
};


class NodeData {
public:
	NodeData (PointCloudWorld & pcw) 
		: m_pcw(pcw)
		, m_fid(0)
		, m_offset(0)
		, m_num_bytes(0)
		, m_original_shell_size(-1)
	{}

	~NodeData () {}

	PointCloudWorld & GetWorld () const
	{
		return m_pcw;
	}

	void SetFileID (uint32_t fid_in)
	{
		m_fid = fid_in;
	}

	uint32_t GetFileID () const
	{
		return m_fid;
	}

	void SetOffset (uint32_t offset_in)
	{
		m_offset = offset_in;
	}

	uint32_t GetOffset () const
	{
		return m_offset;
	}

	void SetByteCount (uint32_t num_bytes_in)
	{
		m_num_bytes = num_bytes_in;
	}

	uint32_t GetByteCount () const
	{
		return m_num_bytes;
	}

	void SetOriginalShellSize (int size)
	{
		m_original_shell_size = size;
	}

	int GetOriginalShellSize () const
	{
		return m_original_shell_size;
	}

private:
	NodeData (NodeData const &);
	void operator= (NodeData const &);

private:
	PointCloudWorld &	m_pcw;
	uint32_t			m_fid;
	uint32_t			m_offset;
	uint32_t	 		m_num_bytes;
	int					m_original_shell_size;
};


typedef std::list<NodeData *> NodeDataList;

typedef std::list<std::pair<HC_KEY, NodeData *>> KeyDataList;

class PCIPD_ThreadData;


class PointCloudIndexPopulateData {
public:
	PointCloudIndexPopulateData (PointCloudWorld & pcw);
	~PointCloudIndexPopulateData ();

	PCIPD_ThreadData & GetConsumerThreadData (HThreadID thread_id);
	
	KeyDataList & GetKeyDataList ()
	{
		return m_key_data_list;
	}

	NodeDataList & GetNodeDataList ()
	{
		return m_node_data_list;
	}

	PointCloudWorld & GetWorld ()
	{
		return m_pcw;
	}

	void FlushProxies ();

	size_t LoadNodeThreadCount () const;

	LoadNodeThread const * LoadNodeThreads () const
	{
		return m_load_node_threads;
	}

private:
	void StopLoadNodeThreads ();

private:
	PointCloudIndexPopulateData (PointCloudIndexPopulateData const &); // disable
	void operator= (PointCloudIndexPopulateData const &);

private:
	PointCloudWorld &		m_pcw;
	KeyDataList				m_key_data_list;
	LoadNodeThread *		m_load_node_threads;
	NodeDataList			m_node_data_list;
	HMutex					m_thread_datas_mutex;

	std::unordered_map<HThreadID, PCIPD_ThreadData *>	m_consumer_thread_datas;

public:
	HMutex					m_key_data_list_mutex;
};


class PCIPD_ThreadData {
public:
	PCIPD_ThreadData (PointCloudWorld & pcw);
	~PCIPD_ThreadData ();

private:
	PCIPD_ThreadData (PCIPD_ThreadData const &); // disable
	void operator= (PCIPD_ThreadData const &); // disable

public:
	PointCloudStreamFileToolkit & GetNodeToolkit ();
	char * GetWorkBuffer (unsigned int num_bytes);
	FILE * GetFilePointer (unsigned int i);
	ooc::delta::in_memory::NodeDelta::ThreadData & NodeDeltaThreadData ();

private:
	PointCloudWorld &			m_pcw;
	PointCloudStreamFileToolkit	m_toolkit;
	std::vector<char>			m_work_buffer;
	FILE **						m_file_handles;
	ooc::delta::in_memory::NodeDelta::ThreadData node_delta_thread_data;
};


// This signals a new segment/node in the index and outputs the corresponding bounding box for the segment/node.
class TK_Point_Cloud_Index_Open_Segment : public TK_Open_Segment {
public:	
	TK_Point_Cloud_Index_Open_Segment (PointCloudWorld & pcw, PointCloudIndexNodeData * node_data_in = nullptr);
	~TK_Point_Cloud_Index_Open_Segment ();
	
	TK_Status Read (BStreamFileToolkit & tk);
	TK_Status Execute (BStreamFileToolkit & tk);
	
	TK_Status Write (BStreamFileToolkit & tk);
	TK_Status Interpret (BStreamFileToolkit & tk, ID_Key key, int variant = 0);
	
	void Reset ();

private:
	TK_Point_Cloud_Index_Open_Segment (TK_Point_Cloud_Index_Open_Segment const &); // disable
	void operator= (TK_Point_Cloud_Index_Open_Segment const &); // disable

private:
	PointCloudWorld &			m_pcw;
	int							m_meta_stage;
	int							m_meta_sub_stage;
	PointCloudIndexNodeData * 	m_node_data;
	bool						m_skip_writing;
	float						m_bbox[6];
	int							m_node_dir_prefix_len;
	char *						m_node_dir_prefix;
};


// This signals the end of a segment/node in the index.
class TK_Point_Cloud_Index_Close_Segment : public TK_Close_Segment {
public:
	TK_Point_Cloud_Index_Close_Segment (PointCloudWorld & pcw, PointCloudIndexNodeData * node_data = nullptr);
	~TK_Point_Cloud_Index_Close_Segment ();
	
	TK_Status Read (BStreamFileToolkit & tk);
	TK_Status Execute (BStreamFileToolkit & tk);
	
	TK_Status Write (BStreamFileToolkit & tk);
	TK_Status Interpret (BStreamFileToolkit & tk, ID_Key key, int variant = 0);
	
	void Reset ();

private:
	TK_Point_Cloud_Index_Close_Segment (TK_Point_Cloud_Index_Close_Segment const &); // disable
	void operator= (TK_Point_Cloud_Index_Close_Segment const &); // disable

private:
	PointCloudWorld &			m_pcw;
	int							m_meta_stage;
	int							m_meta_sub_stage;
	PointCloudIndexNodeData * 	m_node_data;
	bool						m_skip_writing;
	unsigned int				m_fid;
	unsigned int				m_offset;
	unsigned int 				m_num_bytes;
};



