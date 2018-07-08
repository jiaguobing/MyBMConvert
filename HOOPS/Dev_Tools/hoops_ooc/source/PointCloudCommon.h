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

#include "NodeDelta.h"
#include "Misc.h"

#include <memory>
#include <stdint.h>
#include <wchar.h>


//////////////////////////////////////////////////////////////////////////


//#define POINT_CLOUD_VERTEX_NORMALS	// TODO: Trash all POINT_CLOUD_VERTEX_NORMALS conditional code
//#define POINT_CLOUD_RGBA
//#define ENABLE_COMPRESSION
#define THREADED_PREPROCESS


//////////////////////////////////////////////////////////////////////////


unsigned int const		OOC_FORMAT_VERSION				= 1;

size_t const			OOC_BUFFER_CHUNK_SIZE			= 16777216;		// 16 MB
int const				OOC_TOOLKIT_BUFFER_SIZE			= 262144;		// 256 KB
size_t const			NUM_BYTES_IN_MEGABYTE			= 1048576;		// 1 MB
unsigned int const		OOC_MAX_FILE_SIZE				= 2147483648U;	// 2 GB
int const				DEFAULT_MAX_SHELL_SIZE			= 10000;
size_t const			DEFAULT_MAX_MEMORY_USAGE		= 512;			// 512 MB
int const				THREAD_POINT_BUFFER_SIZE		= 200000;
unsigned int const		MAX_NUMBER_OPEN_FILES_PER_THREAD= 128;

double const 			FEPSILON 						= 1e-7f; 


class PointCloudWorld;


class OOCMemoryObject {
protected:
	OOCMemoryObject () {}

public:
	static void * operator new (size_t size, PointCloudWorld & pcw) throw();
	static void * operator new [] (size_t size, PointCloudWorld & pcw) throw();
	static void operator delete (void * p, PointCloudWorld & pcw);
	static void operator delete [] (void * p, PointCloudWorld & pcw);

public:
	static void * operator new (size_t size)
	{
		return ::operator new(size);
	}

	static void * operator new [] (size_t size)
	{
		return ::operator new(size);
	}

	static void operator delete (void * p)
	{
		return ::operator delete(p);
	}

	static void operator delete [] (void * p)
	{
		return ::operator delete[](p);
	}
};


class RGBColor24 {
public:
	RGBColor24 (unsigned char red_in = 0, unsigned char green_in = 0, unsigned char blue_in = 0);
	RGBColor24 (float red_in, float green_in, float blue_in);
	RGBColor24 (RGBColor24 const & rgb_in);

public:
	unsigned char red;
	unsigned char green;
	unsigned char blue;
};


class Coordinates {
public:
	Coordinates ()
		: x(0.0f)
		, y(0.0f)
		, z(0.0f)
	{}

	Coordinates (float x, float y, float z)
		: x(x)
		, y(y)
		, z(z)
	{}

	Coordinates & operator= (Coordinates const & other)
	{
		x = other.x;
		y = other.y;
		z = other.z;
		return *this;
	}

public:
	float x;
	float y;
	float z;
};


unsigned char const PCPOINT_VALIDITY_INVALID			= 0x00;
unsigned char const PCPOINT_VALIDITY_XYZ				= 0x01;
unsigned char const PCPOINT_VALIDITY_INTENSITY			= 0x02;
unsigned char const PCPOINT_VALIDITY_RGB				= 0x04;
unsigned char const PCPOINT_VALIDITY_GRAY_SCALE_POINT	= PCPOINT_VALIDITY_XYZ | PCPOINT_VALIDITY_INTENSITY;
unsigned char const PCPOINT_VALIDITY_RGB_POINT			= PCPOINT_VALIDITY_XYZ | PCPOINT_VALIDITY_RGB;
unsigned char const PCPOINT_VALIDITY_ALL_VALID			= PCPOINT_VALIDITY_XYZ | PCPOINT_VALIDITY_RGB | PCPOINT_VALIDITY_INTENSITY;


class PCPoint : public OOCMemoryObject {
public:
	PCPoint ()
		: coords(0.0f, 0.0f, 0.0f)
		, intensity(0.0f)
		, rgb_color(0.0f, 0.0f, 0.0f)
#ifdef POINT_CLOUD_VERTEX_NORMALS
		, normal(0.0f, 0.0f, 0.0f)
#endif
	{}

	PCPoint (
		Coordinates const & pt,
		float intensity,
		RGBColor24 const & rgb
#ifdef POINT_CLOUD_VERTEX_NORMALS
		, Coordinates const & normal
#endif
		);
	
	PCPoint & operator= (PCPoint const & other);

public:
	Coordinates		coords;
	float			intensity;
	RGBColor24		rgb_color;
	unsigned char	vfield;
#ifdef POINT_CLOUD_VERTEX_NORMALS
	Coordinates			normal;
#endif
};


void FindFileNameWithoutDirectory (wchar_t const * file_name_in, wchar_t * directory_path = nullptr, wchar_t * file_name_with_extension = nullptr);

void FindFileNameWithoutExtension (wchar_t const * file_name_in, wchar_t * file_name_no_extension = nullptr, wchar_t * extension = nullptr);

void FindFileNameWithoutExtensionOrDirectory (
	wchar_t const * file_name_in, wchar_t * directory_path = nullptr, 
	wchar_t * file_name_no_extension = nullptr, wchar_t * extension = nullptr);


class PrettifyInteger {
public:
	PrettifyInteger (uint64_t n);

	char const * c_str () const
	{
		return pretty;
	}

	operator char const * () const
	{
		return pretty;
	}
private:
	char buff[1024];
	char * pretty;
};


class PointCloudIndexPopulateData;


typedef std::list<wchar_t *> PointCloudFileList;


class PointCloudWorld {
	friend class OOCMemoryObject;

public:
	typedef std::unordered_map<ooc::NodeHandle, HC_KEY, ooc::HashKeyAdapter<ooc::NodeHandle>> NodeHandleToKey;

public:
	PointCloudWorld (ooc::LoadType load_type, std::unique_ptr<ooc::delta::in_memory::NodeDeltas> node_deltas);
	~PointCloudWorld ();
	
	// sets up various variables for pre-processing and/or loading the point cloud
	void Initialize (wchar_t const * ooc_file);

	void Fail (wchar_t const * error_message);
	bool Failed () const;

	wchar_t const * GetOOCFileName () const
	{
		return m_ooc_file_name;
	}
	
	/********************************************************************************/
	/*                  Pre-processing functions/settings                           */
	/********************************************************************************/

	ooc::preprocess::PreprocessStatus Preprocess ();

	size_t GetNodeFileNameSize () const;
	void GetFullNodeFileName (float const bbox_in[6], wchar_t * node_file_name_out) const;
	void GetNodeDirectoryPrefix (wchar_t * node_dir_prefix_out) const;
	wchar_t const * GetNodeDirectoryPrefix () const;
	void GetNodeFileName (float const bbox_in[6], wchar_t * node_file_name_out) const;
	void GetTempFileName (unsigned int fid_in, wchar_t * temp_file_name_out) const;
	void GetMasterNodeFileName (unsigned int fid_in, wchar_t * master_node_file_name_out) const;

	size_t GetStreamFileNameSize () const;
	void GetStreamFileName (wchar_t * stream_file_name_out) const;

	size_t GetStreamMetadataFileNameSize () const;
	void GetStreamMetadataFileName (wchar_t * stream_metadata_file_name_out) const;

	size_t GetIndexFileNameSize () const;
	void GetIndexFileName (wchar_t * index_file_name_out) const;

	void SetOOCFormatVersion (unsigned int in_version)
	{
		m_ooc_format_version = in_version;
	}

	PointCloudFileList & GetPointCloudFileList ();

	void AddPointCloudFile (wchar_t const * point_cloud_file_in);

	void SetLoggingFile (wchar_t const * log_file_name = nullptr);
	void LogMessage (wchar_t const * log_string, bool should_timestamp = false);
	void LogMessage (char const * log_string, bool should_timestamp = false);

	void SetMaxShellSize (int max_shell_size_in)
	{
		m_max_shell_size = max_shell_size_in;
	}

	int GetMaxShellSize () const
	{
		return m_max_shell_size;
	}

	void SetSubSamplePercentage (double subsample_in)
	{
		m_subsample = subsample_in;
	}

	double GetSubSamplePercentage () const
	{
		return m_subsample;
	}

	void SetCullingBoundingBox (float const culling_bbox_in[6])
	{
		m_has_culling_bbox = true;
		memcpy(m_culling_bbox, culling_bbox_in, 6 * sizeof(float));
	}

	bool GetHasCullingBoundingBox () const
	{
		return m_has_culling_bbox;
	}

	void GetCullingBoundingBox (float culling_bbox_out[6]) const
	{
		memcpy(culling_bbox_out, m_culling_bbox, 6 * sizeof(float));
	}

	void SetOverwriteExistingFiles (bool overwrite_in)
	{
		m_overwrite = overwrite_in;
	}

	bool GetOverwriteExistingFiles () const
	{
		return m_overwrite;
	}

	void SetRandomizePoints (bool randomize_in)
	{
		m_randomize = randomize_in;
	}

	bool GetRandomizePoints () const
	{
		return m_randomize;
	}

	// expects value in MB
	void SetMaxMemoryUsage (size_t max_memory_usage_in);

	// returns value in MB
	size_t GetMaxMemoryUsage () const
	{
		return m_max_memory_usage / NUM_BYTES_IN_MEGABYTE;
	}

	double GetCurrentMemoryUsage () const
	{
		return static_cast<double>(m_raw_memory_usage) / static_cast<double>(NUM_BYTES_IN_MEGABYTE);
	}

	void SetReadMemoryUsage (size_t rm)
	{
		m_read_memory_usage = rm;
	}

	/********************************************************************************/
	/*                    OOC loading functions/settings                            */
	/********************************************************************************/

	ooc::LoadStatus Load ();
	void SetNodeDirectoryPrefix (wchar_t const * node_dir_prefix_in);
	void GetNodeFileName (wchar_t const * relative_node_file_name, wchar_t * full_node_file_name_out) const;
	PointCloudIndexPopulateData & GetPopulateData ();
	void SetLoadingLastNodeInList (bool value);
	ooc::io::IOResult CommitDeltasToFile (char * buffer, int buffer_size, H_UTF8 file);
	void SetParentDeltaFile (H_UTF8 const & parent_file);
	void AddDeltaFile (H_UTF8 const & delta_file, uint32_t delta_file_id);
	H_WCS const * LookupDeltaFile (uint32_t delta_file_id) const;
	NodeHandleToKey & GetNodeHandleToKey ();
	void IncrementLoadingLastNodeInList ();
	void DecrementLoadingLastNodeInList ();
	void SetNodeLoaded ();
	bool PollNodesLoadingOrHaveBeenLoaded ();

	H_UTF8 const & GetOOCFileDirectory () const
	{
		return m_utf8_ooc_file_dir;
	}

	ooc::delta::in_memory::NodeDeltas & GetNodeDeltas ()
	{
		return *m_node_deltas;
	}

	void SetLoadType (ooc::LoadType load_type_in)
	{
		m_load_type = load_type_in;
	}

	ooc::LoadType GetLoadType () const
	{
		return m_load_type;
	}

private:
	wchar_t *						m_ooc_file_name;
	wchar_t * 						m_ooc_file_dir;
	size_t							m_ooc_file_dir_len;
	wchar_t *						m_node_dir_prefix;
	size_t							m_node_dir_prefix_len;
	unsigned int					m_ooc_format_version;
	PointCloudFileList				m_point_cloud_file_list;
	FILE *							m_log_fp;
	int								m_max_shell_size;
	double							m_subsample;
	bool							m_has_culling_bbox;
	float							m_culling_bbox[6];
	bool							m_overwrite;
	bool							m_randomize;
	size_t							m_max_memory_usage;
	size_t							m_raw_memory_usage;

	std::unordered_map<void *, size_t>	m_pointer_size_hash;

	ooc::LoadType					m_load_type;
	PointCloudIndexPopulateData *	m_populate_data;
	bool volatile					m_new_nodes_loaded;
	int volatile					m_threads_loading_last_node_in_list;
	size_t							m_read_memory_usage;
	bool							m_failed;
	H_UTF8							m_utf8_ooc_file_dir;

	std::unique_ptr<ooc::delta::in_memory::NodeDeltas>	m_node_deltas;

	H_UTF8								m_parent_delta_file;
	std::unordered_map<uint32_t, H_WCS>	m_delta_file_id_to_delta_file;
	NodeHandleToKey						m_handle_to_key;
};



