#ifndef HW2DREADER_H
#define HW2DREADER_H

#include "HW2DReaderCommon.h"

#include "dwf/dwfx/Constants.h"

class HW2DReader : public WT_FileExt
{
public:
	HW2DReader(
		DWFInputStream * rW2DStream, 
		HInputHandlerOptions * pInputHandlerOptions, 
		HC_KEY model_key, 
		bool rebaseModel, 
		HInputHandler * m);

	HW2DReader(
		DWFToolkit::DWFResource * pResource, 
		HInputHandlerOptions * pInputHandlerOptions, 
		HC_KEY model_key, 
		bool rebaseModel, 
		HInputHandler * m);

	virtual ~HW2DReader() { delete m_pReaderCommon; }

	void init();
	void GetExtents(double & x, double & y);
	WT_Result read(double z_separation, double depth_range) { return m_pReaderCommon->read(z_separation, depth_range); }

	WT_Result process_stream_close() { return m_pReaderCommon->process_stream_close(); }
	WT_Result process_stream_end_seek(){ return m_pReaderCommon->process_stream_end_seek(); }
	WT_Result process_stream_open(){ return m_pReaderCommon->process_stream_open(); }
	WT_Result process_stream_read(int desired_bytes, int & bytes_read, void * buffer)
	{ return m_pReaderCommon->process_stream_read(desired_bytes, bytes_read, buffer); }
	WT_Result process_stream_seek(int distance, int & amount_seeked)
	{ return m_pReaderCommon->process_stream_seek (distance, amount_seeked); }
	WT_Result process_stream_tell(unsigned long * current_file_pointer_position)
	{ return m_pReaderCommon->process_stream_tell (current_file_pointer_position); }
	WT_Result process_stream_write(int size, void const * buffer)
	{ return m_pReaderCommon->process_stream_write(size, buffer); }

private:
	HW2DReaderCommon * m_pReaderCommon;
};

#ifndef DWFIO_SUPPORT_ONLY_LEGACY_VERSIONS
#include "Access.h"
#include "dwf/XAML/XamlFile.h"
class HW2DReaderXaml : public WT_XAML_File //WT_FileExt
{
public:
	HW2DReaderXaml(
		DWFToolkit::DWFResource * pResource,
		DWFToolkit::DWFSection * pSection,
		HInputHandlerOptions * pInputHandlerOptions,
		HC_KEY model_key, 
		bool rebaseModel,
		HInputHandler * m,
		bool is_dwfx);

	virtual ~HW2DReaderXaml();

	void init();

	WT_Result read(double z_separation, double depth_range) { return m_pReaderCommon->read(z_separation, depth_range); }
	
	WT_Result process_stream_close() { return m_pReaderCommon->process_stream_close(); }
	WT_Result process_stream_end_seek() { return m_pReaderCommon->process_stream_end_seek(); }
	WT_Result process_stream_open() { return m_pReaderCommon->process_stream_open(); }
	WT_Result process_stream_read(int desired_bytes, int & bytes_read, void * buffer)
	{ return m_pReaderCommon->process_stream_read(desired_bytes, bytes_read, buffer); }
	WT_Result process_stream_seek(int distance, int & amount_seeked)
	{ return m_pReaderCommon->process_stream_seek (distance, amount_seeked); }
	WT_Result process_stream_tell(unsigned long * current_file_pointer_position)
	{ return m_pReaderCommon->process_stream_tell (current_file_pointer_position); }
	WT_Result process_stream_write(int size, void const * buffer)
	{ return m_pReaderCommon->process_stream_write(size, buffer); }

private:
	HW2DReaderCommon * m_pReaderCommon;
	Access * m_pAccess;
};
#endif

#endif //HW2DREADER_H