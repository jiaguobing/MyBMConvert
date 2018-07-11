#ifndef HW2DBOUNDREADER_H
#define HW2DBOUNDREADER_H

#include "dwf/whiptk/fileext.h"
#include "dwf/whiptk/whiperrs.h"
#include "dwfcore/InputStream.h"
#include "dwf/package/Resource.h"
#include "dwf/package/Section.h"
#include "dwf/dwfx/Constants.h"
#include "dwf/package/EPlotSection.h"
#include "dwf/package/Paper.h"

#include "Access.h"

// A class which reads through 2D DWF and only computes the bounding box
class HW2DBoundReader : public WT_FileExt
{
public:
	HW2DBoundReader(DWFInputStream & rW2DStream);
	virtual ~HW2DBoundReader() {}

	WT_Result read();
	WT_Result process_stream_close() { return WT_Result::Success; }
	WT_Result process_stream_end_seek() { return WT_Result::Success; }
	WT_Result process_stream_open();
	WT_Result process_stream_read(int desired_bytes, int & bytes_read, void * buffer);
	WT_Result process_stream_seek(int distance, int & amount_seeked);
	WT_Result process_stream_tell(unsigned long * current_file_pointer_position);
	WT_Result process_stream_write(int /*size*/, void const * /*buffer*/) { return WT_Result::Toolkit_Usage_Error; }

	static WT_Result _processPolyMarker(WT_Polymarker & /*polymarker*/, WT_File & /*file*/) { return WT_Result::Success; }
	static WT_Result _processPolytriangle(WT_Polytriangle & polytriangle, WT_File & file);
	static WT_Result _processText(WT_Text & text, WT_File & file);
	static WT_Result _processEllipse(WT_Outline_Ellipse & wt_oellipse, WT_File & file);
	static WT_Result _processPolyline(WT_Polyline & polyline, WT_File & file);
	static WT_Result _processPolygon(WT_Polygon & polygon,	WT_File & file);
	static WT_Result _processImage(WT_Image & image, WT_File & file);
	static WT_Result _processPNG_Group4_Image(WT_PNG_Group4_Image & image, WT_File & file);
	static WT_Result _processVisibility(WT_Visibility & visibility,  WT_File & file);

	WT_Logical_Box m_wtModelBoundingBox;

private:
	size_t				m_nBytesAvailable;
	DWFString			m_zContent;
	DWFInputStream &	m_rW2DStream;
};

#ifndef DWFIO_SUPPORT_ONLY_LEGACY_VERSIONS
#include "dwf/XAML/XamlFile.h"

class HW2DBoundReaderXaml : public WT_XAML_File //WT_FileExt
{
public:
	HW2DBoundReaderXaml(DWFToolkit::DWFResource * pResource, DWFToolkit::DWFSection * pSection);
	virtual ~HW2DBoundReaderXaml();

	WT_Result read();
	WT_Result process_stream_close() { return WT_Result::Success; }
	WT_Result process_stream_end_seek() { return WT_Result::Success; }
	WT_Result process_stream_open();
	WT_Result process_stream_read(int desired_bytes, int & bytes_read, void * buffer);
	WT_Result process_stream_seek(int distance, int & amount_seeked);
	WT_Result process_stream_tell(unsigned long * current_file_pointer_position);
	WT_Result process_stream_write(int /*size*/, void const * /*buffer*/) { return WT_Result::Toolkit_Usage_Error; }

	static WT_Result _processPolyMarker(WT_Polymarker & /*polymarker*/, WT_File & /*file*/) { return WT_Result::Success; }
	static WT_Result _processPolytriangle(WT_Polytriangle & polytriangle, WT_File & file);
	static WT_Result _processText(WT_Text & text, WT_File & file);
	static WT_Result _processEllipse(WT_Outline_Ellipse & wt_oellipse, WT_File & file);
	static WT_Result _processPolyline(WT_Polyline & polyline, WT_File & file);
	static WT_Result _processPolygon(WT_Polygon & polygon,	WT_File & file);
	static WT_Result _processImage(WT_Image & image, WT_File & file);
	static WT_Result _processPNG_Group4_Image(WT_PNG_Group4_Image & image, WT_File & file);
	static WT_Result _processVisibility(WT_Visibility & visibility,  WT_File & file);

	WT_Logical_Box m_wtModelBoundingBox;

private:
	size_t				m_nBytesAvailable;
	DWFString			m_zContent;
	DWFInputStream *	m_rW2DStream;
	Access *			m_pAccess;
};
#endif

#endif //HW2DBOUNDREADER_H