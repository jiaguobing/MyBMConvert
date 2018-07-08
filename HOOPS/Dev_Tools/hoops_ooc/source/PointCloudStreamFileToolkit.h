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

#include <wchar.h>

#ifndef _WIN32
#	include <unistd.h>
#endif

#include "BStream.h"
#include "BStreamFileToolkit.h"

#include "PointCloudCommon.h"
#include "Misc.h"
#include "NodeDelta.h"


//////////////////////////////////////////////////////////////////////////


enum PointCloudStreamFileWriteMode {
	// Write_Stream_File will return TK_Complete, TK_Error or TK_Pause 
	//   - TK_Pause indicates that the writing is only partially complete, and should be resumed later by subsequent calls to the 
	//  	 function with Append mode specified
	//   - an existing file of the same name will be overwritten if it exists
	WriteMode_Standard,

	// Write_Stream_File will return TK_Complete, TK_Error or TK_Pause
	//   - TK_Pause has same meaning as above
	//   - will append to an existing file of the same name if it exists
	WriteMode_Append,
};


// This class handles setting up the opcode handlers and initiating the reading/writing of data
class PointCloudStreamFileToolkit : public BStreamFileToolkit {
public:
	PointCloudStreamFileToolkit (PointCloudWorld & pcw, unsigned int max_file_size_in = 0);
	~PointCloudStreamFileToolkit ();


	void SetBufferLimit (int buffer_limit_in);

	void ResetOpcodes ();

	void SetOpcodeHandler (int which, BBaseOpcodeHandler * handler);


	ooc::preprocess::PreprocessStatus Generate_OOC_File ();
	ooc::LoadStatus Read_OOC_File ();
	
	// we  OVERRIDE the BStreamFileToolkit::Read_Stream_File function because we need to handle split files
	TK_Status Read_Stream_File (wchar_t const * stream_file_in);
	
	// this is a general purpose function for writing out stream files
	// - it expects any opcode handlers to already be set up
	// - the write mode flag affects how the writing occurs (see descriptions of PointCloudStreamFileWriteMode enum above)
	TK_Status Write_Stream_File (wchar_t const * in_file_name, PointCloudStreamFileWriteMode in_write_mode = WriteMode_Standard);

	TK_Status Write_Stream_Buffer (std::vector<char> & out_buffer);
	
	// this function should only get called when a paused stream file is already getting written out and the writing needs to terminate or complete
	TK_Status Finalize_Stream_File (wchar_t const * stream_file_out);
	
	// this sets up anything that needs to get done for iterating though a particular node
	void Stream_File_Iteration (wchar_t const * stream_file_out);
	
	// custom OpenFile function which handles split files
	virtual TK_Status OpenFile (wchar_t const * name, bool write_flag = false)  OVERRIDE;
	
	// function which opens a file which we can append to (default BStreamFileToolkit::OpenFile blows away existing files)
	TK_Status OpenFileForAppend (wchar_t const * name);
	
	bool GetCanPause () const
	{
		return m_can_pause;
	}

	void SoftRestart ();
	void Restart ();

	void SetDataSink (ooc::Buffer<ooc::Point> * point_sink, ooc::Buffer<ooc::RGBColor> * color_sink)
	{
		ASSERT((point_sink == nullptr) == (color_sink == nullptr));
		m_data_sink = std::make_pair(point_sink, color_sink);
	}

	std::pair<ooc::Buffer<ooc::Point> *, ooc::Buffer<ooc::RGBColor> *> GetDataSink() const
	{
		return m_data_sink;
	}

	NodeData & GetCurrentNodeData () const
	{
		ASSERT(m_current_node_data != nullptr);
		return *m_current_node_data;
	}

	ooc::delta::in_memory::NodeDelta::ThreadData & GetCurrentThreadData () const
	{
		ASSERT(m_current_thread_data != nullptr);
		return *m_current_thread_data;
	}

	void SetCurrentNodeData (NodeData * node_data)
	{
		if (node_data != nullptr) {
			ASSERT(m_current_node_data == nullptr);
		}
		m_current_node_data = node_data;
	}

	void SetCurrentThreadData (ooc::delta::in_memory::NodeDelta::ThreadData * thread_data)
	{
		if (thread_data != nullptr) {
			ASSERT(m_current_thread_data == nullptr);
		}
		m_current_thread_data = thread_data;
	}

	std::vector<ooc::delta::in_memory::AcceptResult> & GetAcceptResultBuffer ()
	{
		return m_accept_results;
	}

private:
	// generate binary version of file, compute bounding information
	TK_Status Initial_Pass ();

	// read in binary version of data
	//	- generate skeleton structure file
	//	- generate node files referenced by skeleton
	TK_Status Second_Pass ();

	// function which populates the write buffer with data 
	TK_Status GenerateBuffer (char * buffer, int size, int & written);

	// returns size of split file name
	//   NOTE: does not include trailing null terminator
	int GetSplitFileNameSize (wchar_t const * file_name_in);

	// populates file_name_out with the split file name
	//   NOTE: file_name_out should have adequate storage
	void GetSplitFileName (wchar_t const * file_name_in, wchar_t * file_name_out);

	TK_Status SplitFile (wchar_t const * name, bool write_flag = false);

	TK_Status Write_Stream_Data ();

private:
	PointCloudStreamFileToolkit (PointCloudStreamFileToolkit const &); // disable
	void operator= (PointCloudStreamFileToolkit const &);

private:
	PointCloudWorld &				m_pcw;						// "global" PointCloudWorld instance
	std::vector<char> *				m_stream_buffer;
	wchar_t *						m_write_file_name;
	wchar_t *						m_point_cloud_stream_file;	// the binary version of the point cloud
	int								m_limit;					// size of the read/write buffer
	std::vector<char>				m_split_buffer;				// buffer for storing contents being read due to file split

	char * 							m_block;				// block for buffered reading/writing
	PointCloudStreamFileWriteMode	m_write_mode;
	std::vector<int>				m_opcodes;				// array of opcodes actually handled by this toolkit
	size_t							m_current_opcode_index;	// current index in the opcode array we are working with
	int								m_amount_pending;		// amount of data (bytes) written to buffer, but not yet written to file
	unsigned int					m_amount_written;		// amount of data (bytes) that has (currently) been written to file
	unsigned int					m_amount_read;			// amount of data (bytes) that has (currently) been read from the file
	unsigned int					m_max_file_size;		// maximum size of stream files generated by the toolkit
	unsigned int					m_split_file_count;		// current count of number of splits
	bool							m_can_pause;			// flag indicating whether opcodes are allowed to "pause" writing of stream files

	bool							m_can_iterate;			// flag indicating that the toolkit is in iterate_mode - this amounts to:
															//   - toolkit has written out any pending data to file and closed it
															//   - toolkit subsequently read file in

	NodeData *															m_current_node_data;
	ooc::delta::in_memory::NodeDelta::ThreadData *						m_current_thread_data;
	std::vector<ooc::delta::in_memory::AcceptResult>					m_accept_results;
	std::pair<ooc::Buffer<ooc::Point> *, ooc::Buffer<ooc::RGBColor> *>	m_data_sink;
};



