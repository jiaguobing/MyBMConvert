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

#include <stdio.h>
#include <wchar.h>
#include <string.h>
#ifndef _WIN32
#	include <unistd.h>
#endif

#include "BStream.h"
#include "BOpcodeHandler.h"

#include "PointCloudCommon.h"
#include "Rand.h"

#include "vbsp.h"

#ifdef THREADED_PREPROCESS
#	include "hmutex.h"
#endif


//////////////////////////////////////////////////////////////////////////


class PointCloudStreamFileToolkit;
class StreamFilePointContainer;
class ExtractionData;


typedef VBSP<PCPoint *, StreamFilePointContainer> PointCloudBSP;


//////////////////////////////////////////////////////////////////////////


#ifdef THREADED_PREPROCESS

struct PointBuffer {
	PCPoint *	m_buffer;
	int			m_count;
};

typedef std::list<PointBuffer *> PointBufferList;

#endif


//////////////////////////////////////////////////////////////////////////


class TK_Point_Cloud_Data;


namespace Point_Cloud_Data
{
	class RW_Point {
	public:
		RW_Point ()
			: stage(0)
		{}

		template <typename ProccessPointCallback>
		TK_Status Read (BStreamFileToolkit & tk, ProccessPointCallback & callback);

		TK_Status Write (BStreamFileToolkit & tk, PCPoint const & point);

	private:
		int stage;
		PCPoint point;
	};


	class RW_Points {
	public:
		RW_Points ()
			: stage(0)
		{}

		TK_Status Read (BStreamFileToolkit & tk, TK_Point_Cloud_Data & handler);

	private:
		int stage;
		RW_Point rw_point;
	};

	class RW_BufferChunk {
	public:
		RW_BufferChunk ()
			: stage(0)
		{}

		TK_Status Write (BStreamFileToolkit & tk, TK_Point_Cloud_Data & handler, wchar_t const * current_file_name);

	private:
		int stage;
		RW_Point rw_point;
	};

	class RW_PointCloudFileList {
	public:
		RW_PointCloudFileList ()
			: stage(0)
		{}

		TK_Status Write (BStreamFileToolkit & tk, TK_Point_Cloud_Data & handler);

	private:
		int stage;
		Point_Cloud_Data::RW_BufferChunk rw_buffer_chunk;
	};
}


// Opcode for parsing the point cloud file, saving out the point data as a binary stream
// file and reading that stream file back in for purposes of spatial subdivision
class TK_Point_Cloud_Data : public TK_User_Data {
private:
	friend class Point_Cloud_Data::RW_Point;
	friend class Point_Cloud_Data::RW_Points;
	friend class Point_Cloud_Data::RW_PointCloudFileList;
	friend class Point_Cloud_Data::RW_BufferChunk;

public:
	TK_Point_Cloud_Data (PointCloudWorld & pcw);
	~TK_Point_Cloud_Data ();
	
	virtual TK_Status Read (BStreamFileToolkit & tk)  OVERRIDE;

	virtual TK_Status Execute (BStreamFileToolkit & tk)  OVERRIDE;
	
	virtual TK_Status Write (BStreamFileToolkit & tk)  OVERRIDE;
	virtual TK_Status Interpret (BStreamFileToolkit & tk, ID_Key key, int variant = 0)  OVERRIDE;
	
	virtual void Reset ()  OVERRIDE;
	
	void GetBoundingBox (float bbox_out[6]) const
	{
		memcpy(bbox_out, m_std_bbox, 6 * sizeof(float));
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
	TK_Status ReadMetafileAndInit (BStreamFileToolkit & tk);
	TK_Status ProcessReadPoint (PCPoint const & point);
	TK_Status ExportPointCloudIndex ();

	TK_Status InitWriteData ();
	TK_Status WritePointCloudFileList (BStreamFileToolkit & tk);
	TK_Status WriteMetafileData (BStreamFileToolkit & tk);
	TK_Status ParseLineForPointWriting (BStreamFileToolkit & tk, wchar_t const * current_file_name);

	static VBSPStatus ExportPointCloudIndexNodes (
		TreeTraversal state,
		StreamFilePointContainer * container,
		BSPNodeMetadata<PCPoint *,
		StreamFilePointContainer> * node_metadata,
		void * user_data);

private:
	PointCloudWorld &		m_pcw;							// "global" PointCloudWorld instance
	FILE *					m_current_point_cloud_file;		// file pointer to current point cloud to be parsed
	char * 					m_read_buffer;					// buffer used for reading in pieces of original point cloud
	wchar_t *				m_index_file_name;				// file name of top-level point cloud index file
	ooc::FileType			m_point_cloud_file_type;		// original point cloud file type
	float		 			m_std_bbox[6];					// bounding box based on the entire point cloud
	float 					m_min_intensity;				// minimum intensity found in point cloud
	float 					m_max_intensity;				// maximum intensity found in point cloud
	float 					m_matrix[16];					// transformation matrix which may not get used depending on file type
	size_t 					m_read_position;				// current position in our internal buffer;
	size_t 					m_amount_read;					// how much data is in our internal buffer;
	bool 					m_success;						// flag indicating results of parsing text
	bool 					m_initial_pass;					// flag indicating initial pass
	bool					m_eof;							// flag indicating there is no more data to examine (end-of-file)o

	bool					m_no_more_reads;				// flag indicating we've read the entire file (though it probably hasn't all been processed yet)

	PCPoint					m_cpt;							// a point extracted from the original point cloud
	uint64_t				m_num_points;					// number of points discovered in file
	uint64_t				m_read_point_count;				// number of read points
	uint64_t				m_line_number;					// current line number
	uint64_t 				m_valid_point_count;			// number of valid points encountered in the file
	uint64_t 				m_num_points_remaining;			// number of points left to process
	bool					m_need_to_subsample;			// whether or not we need to sub-sample

	// Number of points actually sorted.
	// May be less than the total number of points in the point cloud if sub-sampling or bounding box culling is used.
	uint64_t				m_num_points_sorted;


#ifdef THREADED_PREPROCESS
public:	// we need the BSP to be public if we are doing threaded loading
#endif
	PointCloudBSP *			m_point_bsp;					// BSP for storing spatially sorted points

private:
	off_t 					m_metadata_offset;				// file offset for metadata
	off_t 					m_current_file_position;		// current file position

	PointCloudFileList::const_iterator		file_list_iter;
	Point_Cloud_Data::RW_Point				rw_point;
	Point_Cloud_Data::RW_Points				rw_points;
	Point_Cloud_Data::RW_PointCloudFileList	rw_point_cloud_file_list;

	// Flag to signify we need to flush the buffer (necessary for 
	// situations where we want to reposition the file pointer,
	// but potentially have data that hasn't been written out yet
	// because the read/write buffer is not full).
	bool 					m_flush_buffer;

#ifdef THREADED_PREPROCESS
public:
	PointBuffer *			m_point_buffer;
	PointBufferList *		m_point_buffer_list;
	HMutex					m_point_buffer_list_mutex;
	HThreadHandle			m_point_process_thread;
	size_t					m_bytes_used;
	int						m_current_point_buffer_size;
	bool					m_no_more_points;
#endif 

private:
	ooc::Rand				m_rand;
};




