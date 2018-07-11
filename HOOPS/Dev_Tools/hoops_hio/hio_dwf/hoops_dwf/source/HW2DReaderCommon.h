#ifndef HW2DREADERCOMMON_H
#define HW2DREADERCOMMON_H

#include "hc.h"
#include "HBaseView.h"
#include "HIOManager.h"
#include "HUtility.h"
#include "tempfile_utils.h"

#include <string>

#include "dwf/whiptk/fileext.h"
#include "dwf/package/GlobalSection.h"
#include "dwf/package/EPlotSection.h"

#if !defined(TWO_PI)
#define TWO_PI                  6.283185307179586476925286766559
#endif

#define HK_Two_Pi TWO_PI

#define DWFIO_EPSILON 1e-05		// Define your own tolerance
#define DWFIO_FLOAT_EQ(x,v) (((v - DWFIO_EPSILON) < x) && (x <( v + DWFIO_EPSILON)))

struct HColor
{
	float red;
	float green;
	float blue;
};

// Common class which handles both reading of regular DWF file and DWFx files
class HW2DReaderCommon
{
public:

	std::map<int, H_FORMAT_TEXT> lay_num_map;

	enum CoordinateMode
	{
		logical_coords = 0,
		application_coords,
		recentered_coords
	};

	// typically used when reading legacy DWF files
	HW2DReaderCommon(WT_File * whip_file,
					 DWFInputStream * rW2DStream,
					 HInputHandlerOptions * pInputHandlerOptions, 
					 HC_KEY model_key,
					 bool rebaseModel,
					 HInputHandler * m,
					 CoordinateMode coord_mode = recentered_coords);

	// used when reading w2d files embedded in DWF files
	HW2DReaderCommon(WT_File * whip_file,
					DWFToolkit::DWFResource * pResource,
					HInputHandlerOptions * pInputHandlerOptions, 
		            HC_KEY model_key,
					bool rebaseModel,
					HInputHandler * m,
					CoordinateMode coord_mode = recentered_coords);

	// used when reading xml w2d files embedded in DWFx files
	HW2DReaderCommon(WT_File * whip_file,
					DWFToolkit::DWFResource * pResource,
					DWFToolkit::DWFSection * pSection, 
					HInputHandlerOptions * pInputHandlerOptions,
					HC_KEY model_key,
					bool rebaseModel, 
					bool is_dwfx,
					HInputHandler * m,
					CoordinateMode coord_mode = recentered_coords);

	~HW2DReaderCommon();

	void init();

	//Process Geometry
	static WT_Result _processPolyMarker(WT_Polymarker & polymarker, WT_File & file);
	static WT_Result _processPolytriangle(WT_Polytriangle & polytriangle, WT_File & file);
	static WT_Result _processText(WT_Text & text, WT_File & file);
	static WT_Result _processContourNode(WT_Contour_Set & contour_set, WT_File & file);
	static WT_Result _processEllipse(WT_Outline_Ellipse & e, WT_File & file);
	static WT_Result _processPolyline(WT_Polyline & polyline, WT_File & file );
	static WT_Result _processPolygon(WT_Polygon & polygon, WT_File & file);
	static WT_Result _processImage(WT_Image & image, WT_File & file);
	static WT_Result _processPNG_Group4_Image(WT_PNG_Group4_Image & image, WT_File & file);

	//Process Attributes
	static WT_Result _processColorMap(WT_Color_Map & color_map, WT_File & file);
	static WT_Result _processURL(WT_URL & named_url, WT_File & file);
	static WT_Result _processLineWeight(WT_Line_Weight & lw, WT_File & file);
	static WT_Result _processFill(WT_Fill  & rFill, WT_File & file);
	static WT_Result _processColor(WT_Color & rColor, WT_File & file);
	static WT_Result _processViewport(WT_Viewport & viewport, WT_File & file);
	static WT_Result _processView(WT_View & view,  WT_File & file);
	static WT_Result _processFont(WT_Font & font, WT_File & file);
	static WT_Result _processVisibility(WT_Visibility & visibility,  WT_File & file);
	static WT_Result _processBackground(WT_Background & background,  WT_File & file);
	static WT_Result _processHAlignment(WT_Text_HAlign & hAln,  WT_File & file);
	static WT_Result _processObjectNode(WT_Object_Node & object_node, WT_File & file);
	static WT_Result _processPlotInfo(WT_Plot_Info & plot_info, WT_File & file);
	static WT_Result _processInkedArea(WT_Inked_Area & inked_area, WT_File & file);
	static WT_Result _processNamedView(WT_Named_View & named_view, WT_File & file);
	static WT_Result _processLayer(WT_Layer & layer, WT_File & file); 
	static WT_Result _processUnits(WT_Units & units, WT_File & file);

	bool compute_extents(DWFInputStream & input_stream);
	WT_Result read(double z_separation, double depth_range_separation);
	int incrementImageCount();
	void setFillMode(bool val);
	bool fillMode() { return m_bFill; }

	WT_Result process_stream_close() { return WT_Result::Success; }
	WT_Result process_stream_end_seek(){ return WT_Result::Success; }
	WT_Result process_stream_open();
	WT_Result process_stream_read(int desired_bytes, int & bytes_read, void * buffer);
	WT_Result process_stream_seek(int distance, int & amount_seeked);
	WT_Result process_stream_tell(unsigned long * current_file_pointer_position);
	WT_Result process_stream_write(int size, void const * buffer) { return WT_Result::Toolkit_Usage_Error; }

	//Import Geometry
	WT_Result _importPolytriangle (WT_Polytriangle & polytriangle);
	WT_Result _importText(WT_Text & text);
	bool is_this_face_really_a_hole(int point_count, HPoint const * points);
	WT_Result _importContourNode(WT_Contour_Set & contour_set);
	WT_Result _importEllipse(WT_Outline_Ellipse & e);
	WT_Result _importPolyline(WT_Polyline & polyline);
	WT_Result _importPolygon(WT_Polygon & polygon);
	WT_Result _importImage(WT_Image & image);
	WT_Result _importPNG_Group4_Image(WT_PNG_Group4_Image & image);

	//Import Attributes
	WT_Result import_line_weight (WT_Line_Weight & lw);
	WT_Result _importLineWeight (WT_Line_Weight & lw) { return import_line_weight(lw); }
	WT_Result _importFill (WT_Fill  & rFill);
	void apply_object_attrs(HC_KEY key);
	WT_Result _importColor(WT_Color & rColor);
	WT_Result _importViewport(WT_Viewport & viewport);
	WT_Result _importLayer(WT_Layer & layer); 
	WT_Result _importColorMap(WT_Color_Map & color_map);
	WT_Result _importObjectNode(WT_Object_Node & object_node);
	WT_Result _importPlotInfo(WT_Plot_Info & plot_info, WT_File & file);
	WT_Result _importInkedArea(WT_Inked_Area & inked_area, WT_File & file) { return WT_Result::Success; }
	WT_Result _importNamedView(WT_Named_View & named_view);
	WT_Result _importFont(WT_Font & font);
	WT_Result _importVisibility(WT_Visibility & visibility);
	WT_Result _importHAlignment(WT_Text_HAlign & hAln);
	WT_Result _importUnits(WT_Units & units);
	WT_Result _importURL(WT_URL & named_url);
	WT_Result _importView(WT_View & view);
	WT_Result _importBackground(WT_Background & background);

	//Scene graph utility functions
	void resort_start();
	void resort_finish();
	void resort_close();
	void resort_open();
	void resort_segment();

private:
	HInputHandler * m_console;

	struct ViewportInfo
	{
		ViewportInfo(WT_String vName, WT_String vUnits, double scale)
			: measurementScalingFactor(scale)
		{
			if (vName.ascii())
				strcpy(viewportName, vName.ascii());
			else
				strcpy(viewportName, H_FORMAT_TEXT("viewport_%d", anonymousViewportCount++));

			if (vUnits.ascii())
				strcpy(viewportMeasuringUnits, vUnits.ascii());
			else
				strcpy(viewportMeasuringUnits, "no_units");
		}

		char	viewportName[MVO_BUFFER_SIZE];				//name of the viewport
		char	viewportMeasuringUnits[MVO_BUFFER_SIZE];	//viewport units (i.e. 'millimeters', 'inches'...)
		double	measurementScalingFactor;					//scale to apply to world coordinates to obtain the correct measurement
	};

	ViewportInfo * m_currentViewport;
	bool m_bisViewportOpen;
	static int anonymousViewportCount;

	std::set<HC_KEY> m_viewportSegments;

	size_t				m_nBytesAvailable;
	DWFString			m_zContent;
	DWFInputStream *	m_rW2DStream;
	bool				m_bFreeInputStream;
	bool				m_bColorMapExist;
	bool				m_bIsDWFx;
	WT_File *			m_pWT_File;
	DWFToolkit::DWFResource * m_pResource;
	DWFToolkit::DWFSection * m_pSection;

	WT_Result import_image( int size, const WT_Byte * image_data, const char * type,
							WT_Logical_Point const & min_corner, WT_Logical_Point const & max_corner);
	WT_Result import_Other_Formats(WT_Image & image);
	WT_Result insert_Image_Mesh(char * image_name, WT_Logical_Point const & min_corner, WT_Logical_Point const & max_corner);


	// removes all the 2*Pi surpluses from the angle and returns the remainder.
	// if the angle is exactly 2*Pi or multiples of it, then it makes it 2*Pi
	// NOTE: Should the start_angle be set to 0 when it is exactly 2*Pi (or multiples of it)
	// instead of making it 2*Pi?
	static inline float unwind_loops(float arc_angle)
	{
		if( arc_angle <= HK_Two_Pi )
			return arc_angle;

		float fmod_arc_angle = fmod(arc_angle, (float) HK_Two_Pi);
		if( (arc_angle >= HK_Two_Pi) && DWFIO_FLOAT_EQ(0.0f, fmod_arc_angle) )
			// rare case where it is exactly divisible by 2PI
			return (float) HK_Two_Pi;
		else
			return fmod_arc_angle;
	}

public:
	H_FORMAT_TEXT m_layerName;
	bool m_layer_visibility;
	double m_z_plane;
	double m_z_separation;
	double m_use_depth_range;
	double m_depth_range;

	CoordinateMode m_CoordMode;
	WT_Units* m_pWhipUnits;
	double m_x_extents;
	double m_y_extents;
	int m_layer_stack;
	bool m_unmark_detector;
	H_UTF8 m_current_url;

	/* HOOPS Attributes affecting geometry*/
	HColor m_Color;
	HPoint m_text_path;
	char m_font[MVO_BUFFER_SIZE];
	char m_fontName[100];
	char m_backupFont[100];
	char m_fontStyle[100];
	char m_fontHeight[100];
	char m_fontScale[100];
	char m_fontOblique[100];
	double m_line_edge_weight;
	bool m_visible;
	char m_hAlignment[4];
	bool m_resort;

	char m_object_id[MVO_BUFFER_SIZE];
	bool m_bDisableDWFDrawOrder;
	double m_priority;
	bool  m_bImportThumbnail;

	/* background stuff */
	HColor m_backgroundColor;
	HPoint m_plotInfoMin;
	HPoint m_plotInfoMax;
	int m_plotInfoUnits;
	int m_pageWidth;
	int m_pageHeight;

protected:
	bool segment_open;
	bool m_bFill;
	static int m_image_count;
	HC_KEY m_modelkey;
	//Please refer issue no :13552 ( Internal Note 1 ) for more details.
	HC_KEY m_stylekey;

	HInputHandlerOptions * m_pInputHandlerOptions;

};

#endif //HW2DREADERCOMMON_H