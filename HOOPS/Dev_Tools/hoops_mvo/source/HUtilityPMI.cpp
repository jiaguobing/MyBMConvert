// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#include "HUtilityPMI.h"

#include "HDB.h"

#include <vector>

using namespace PMII;

namespace PMII
{

static int const VIEW_NAME = ('v') | ('i' << 8) | ('e' << 16) | ('w' << 24);
static int const VIEW_CAMERA = ('c') | ('m' << 8) | ('r' << 16) | ('a' << 24);

class PrivateImpl
{
public:
	PrivateImpl() {}

	virtual ~PrivateImpl() {}
};

// ======================= start common class impls =======================

typedef std::vector<PMI::Polyline> PolylineArray;
typedef std::vector<PMI::Point> PointArray;
typedef std::vector<PMI::Polygon> PolygonArray;

// ==============================================

class FramePrivate : public PrivateImpl
{
public:
	FramePrivate()
	{
		m_polylines.resize(0);
	}

	~FramePrivate() {}

	void Copy(FramePrivate * that)
	{
		m_polylines = that->m_polylines;
	}

	PolylineArray	m_polylines;
};

// ==============================================

class DrawingPrivate : public PrivateImpl
{
public:
	DrawingPrivate()
	{
		m_polygons.resize(0);
	}

	~DrawingPrivate() {}

	void Copy(DrawingPrivate * that)
	{
		m_polygons = that->m_polygons;
	}

	PolygonArray	m_polygons;
};

// ==============================================

class OrientationPrivate : public PrivateImpl
{
public:
	OrientationPrivate()
	{
		HC_Compute_Identity_Matrix(m_matrix);
	}

	~OrientationPrivate() {}

	void Copy(OrientationPrivate * that)
	{
		memcpy(m_matrix, that->m_matrix, 16 * sizeof(float));
	}

	float m_matrix[16];
};

// ==============================================

class PolylinePrivate : public PrivateImpl
{
public:
	PolylinePrivate() 
	{
		m_points.resize(0);
		m_color.Set(-1, -1, -1);
		m_pattern[0] = '\0';
	}

	~PolylinePrivate() {}

	void Copy(PolylinePrivate * that)
	{
		m_points = that->m_points;
		m_color = that->m_color;
		strncpy(m_pattern, that->m_pattern, 1024);
	}

	PointArray		m_points;
	PMI::RGBColor	m_color;
	char			m_pattern[1024];
};

// ==============================================

class PolygonPrivate : public PrivateImpl
{
public:
	PolygonPrivate() 
	{
		m_points.resize(0);
		m_color.Set(-1, -1, -1);
	}

	~PolygonPrivate() {}

	void Copy(PolygonPrivate * that)
	{
		m_points = that->m_points;
		m_color = that->m_color;
	}

	PointArray		m_points;
	PMI::RGBColor	m_color;
};

// ==============================================

class TextAttributesPrivate : public PrivateImpl
{
public:
	TextAttributesPrivate()
	{
		m_insertion_point.Set(0, 0, 0);
		m_font_size = 0.0f;
		m_font_name = 0;
		m_color.Set(-1, -1, -1);
		m_format = 0;
		m_width_scale = 1.;
	}

	~TextAttributesPrivate() 
	{
		delete [] m_font_name;
	}

	void Copy(TextAttributesPrivate * that)
	{
		m_insertion_point = that->m_insertion_point;
		m_font_size = that->m_font_size;

		delete [] m_font_name;

		if (!that->m_font_name)
		{
			m_font_name = 0;
		}
		else
		{
			size_t font_name_size = strlen(that->m_font_name);
			m_font_name = new char [font_name_size + 1];
			strcpy(m_font_name, that->m_font_name);
		}

		m_orientation = that->m_orientation;
		m_color = that->m_color;
		m_format = that->m_format;
		m_width_scale = that->m_width_scale;
	}

	PMI::Point			m_insertion_point;
	float				m_font_size;
	char *				m_font_name;
	PMI::Orientation	m_orientation;
	PMI::RGBColor		m_color;
	char				m_format;
	float				m_width_scale;
};

// ==============================================

class StringPrivate : public PrivateImpl
{
public:
	StringPrivate() 
	{
		m_wcs = new H_WCS();
		m_ascii = 0;
	}

	StringPrivate(char const * cstring)
	{
		m_wcs = new H_WCS(cstring);
		m_ascii = 0;
	}

	StringPrivate(wchar_t const * wstring)
	{
		m_wcs = new H_WCS(wstring);
		m_ascii = 0;
	}

	StringPrivate(H_WCS const & wcs)
	{
		m_wcs = new H_WCS(wcs);
		m_ascii = 0;
	}

	StringPrivate(H_UTF8 const & utf8)
	{
		m_wcs = new H_WCS(utf8);
		m_ascii = 0;
	}

	~StringPrivate()
	{
		delete m_wcs;
		delete m_ascii;
	}

	H_WCS *				m_wcs;
	HUtilityAsciiStr *	m_ascii;
};

// ==============================================

class OptionsPrivate : public PrivateImpl
{
public:
	OptionsPrivate()
	{
		m_display_parallel_to_screen = false;
	}
	~OptionsPrivate() 
	{
	}

	void Copy(OptionsPrivate * that)
	{
		m_display_parallel_to_screen = that->m_display_parallel_to_screen;
	}

	bool	m_display_parallel_to_screen;
};

// ======================= end common class impls =======================



// ====================== start actual entity impls =======================

class BasePrivate : public PrivateImpl
{
public:
	BasePrivate()
	{
		m_segment_key = INVALID_KEY;
	}

	~BasePrivate() {}

	void Copy(BasePrivate * that)
	{
		m_segment_key = that->m_segment_key;
	}

	HC_KEY	m_segment_key;
};

// ====================== end actual entity impls =======================

}; // PMII



// ======================= start common classes =======================

PMI::Frame::Frame()
{
	impl_ = new FramePrivate();
}

PMI::Frame::Frame(Frame const & that)
{
	impl_ = new FramePrivate();
	Set(that);
}

PMI::Frame::~Frame()
{
	delete impl_;
}

void PMI::Frame::Set(PMI::Frame const & that)
{
	FramePrivate * my_impl = (FramePrivate *)impl_;
	FramePrivate * that_impl = (FramePrivate *)that.impl_;

	my_impl->Copy(that_impl);
}

PMI::Frame const & PMI::Frame::operator=(PMI::Frame const & that)
{
	Set(that);
	return *this;
}

unsigned int PMI::Frame::GetPolylineCount() const
{
	FramePrivate * my_impl = (FramePrivate *)impl_;

	return static_cast<unsigned int>(my_impl->m_polylines.size());
}

void PMI::Frame::GetPolylines(unsigned int & out_count, PMI::Polyline * out_polylines) const
{
	FramePrivate * my_impl = (FramePrivate *)impl_;

	out_count = (unsigned int)my_impl->m_polylines.size();

	if (out_polylines == 0)
		return;
	
	for (unsigned int i = 0; i < out_count; i++)
		out_polylines[i] = my_impl->m_polylines[i];
}

void PMI::Frame::SetPolylines(unsigned int in_count, PMI::Polyline const * in_polylines)
{
	FramePrivate * my_impl = (FramePrivate *)impl_;

	my_impl->m_polylines.resize(in_count);

	for (unsigned int i = 0; i < in_count; i++)
		my_impl->m_polylines[i] = in_polylines[i];
}

// ==============================================

PMI::Drawing::Drawing()
{
	impl_ = new DrawingPrivate();
}

PMI::Drawing::Drawing(Drawing const & that)
{
	impl_ = new DrawingPrivate();
	Set(that);
}

PMI::Drawing::~Drawing()
{
	delete impl_;
}

void PMI::Drawing::Set(PMI::Drawing const & that)
{
	DrawingPrivate * my_impl = (DrawingPrivate *)impl_;
	DrawingPrivate * that_impl = (DrawingPrivate *)that.impl_;

	my_impl->Copy(that_impl);
}

PMI::Drawing const & PMI::Drawing::operator=(PMI::Drawing const & that)
{
	Set(that);
	return *this;
}

unsigned int PMI::Drawing::GetPolygonCount() const
{
	DrawingPrivate * my_impl = (DrawingPrivate *)impl_;

	return static_cast<unsigned int>(my_impl->m_polygons.size());
}

void PMI::Drawing::GetPolygons(unsigned int & out_count, PMI::Polygon * out_polygons) const
{
	DrawingPrivate * my_impl = (DrawingPrivate *)impl_;

	out_count = (unsigned int)my_impl->m_polygons.size();

	if (out_polygons == 0)
		return;
	
	for (unsigned int i = 0; i < out_count; i++)
		out_polygons[i] = my_impl->m_polygons[i];
}

void PMI::Drawing::SetPolygons(unsigned int in_count, PMI::Polygon const * in_polygons)
{
	DrawingPrivate * my_impl = (DrawingPrivate *)impl_;

	my_impl->m_polygons.resize(in_count);

	for (unsigned int i = 0; i < in_count; i++)
		my_impl->m_polygons[i] = in_polygons[i];
}

// ==============================================

PMI::Orientation::Orientation()
{
	impl_ = new OrientationPrivate();
}

PMI::Orientation::Orientation(PMI::Orientation const & that)
{
	impl_ = new OrientationPrivate();
	Set(that);
}

PMI::Orientation::~Orientation()
{
	delete impl_;
}

void PMI::Orientation::Set(PMI::Orientation const & that)
{
	OrientationPrivate * my_impl = (OrientationPrivate *)impl_;
	OrientationPrivate * that_impl = (OrientationPrivate *)that.impl_;

	my_impl->Copy(that_impl);
}

PMI::Orientation const & PMI::Orientation::operator=(PMI::Orientation const & that)
{
	Set(that);
	return *this;
}

void PMI::Orientation::GetXComponent(Point & out_component) const
{
	OrientationPrivate * my_impl = (OrientationPrivate *)impl_;

	memcpy(&out_component, my_impl->m_matrix, 3 * sizeof(float));
}

void PMI::Orientation::GetXComponent(float & out_x, float & out_y, float & out_z) const
{
	PMI::Point xcomp;
	GetXComponent(xcomp);

	out_x = xcomp.x;
	out_y = xcomp.y;
	out_z = xcomp.z;
}

void PMI::Orientation::SetXComponent(Point const & in_component)
{
	OrientationPrivate * my_impl = (OrientationPrivate *)impl_;

	memcpy(my_impl->m_matrix, &in_component, 3 * sizeof(float));
}

void PMI::Orientation::SetXComponent(float in_x, float in_y, float in_z)
{
	SetXComponent(PMI::Point(in_x, in_y, in_z));
}

void PMI::Orientation::GetYComponent(Point & out_component) const
{
	OrientationPrivate * my_impl = (OrientationPrivate *)impl_;
	
	memcpy(&out_component, my_impl->m_matrix + 4, 3 * sizeof(float));
}

void PMI::Orientation::GetYComponent(float & out_x, float & out_y, float & out_z) const
{
	PMI::Point ycomp;
	GetYComponent(ycomp);

	out_x = ycomp.x;
	out_y = ycomp.y;
	out_z = ycomp.z;
}

void PMI::Orientation::SetYComponent(Point const & in_component)
{
	OrientationPrivate * my_impl = (OrientationPrivate *)impl_;

	memcpy(my_impl->m_matrix + 4, &in_component, 3 * sizeof(float));
}

void PMI::Orientation::SetYComponent(float in_x, float in_y, float in_z)
{
	SetYComponent(PMI::Point(in_x, in_y, in_z));
}

void PMI::Orientation::GetZComponent(Point & out_component) const
{
	OrientationPrivate * my_impl = (OrientationPrivate *)impl_;
	
	memcpy(&out_component, my_impl->m_matrix + 8, 3 * sizeof(float));
}

void PMI::Orientation::GetZComponent(float & out_x, float & out_y, float & out_z) const
{
	PMI::Point zcomp;
	GetZComponent(zcomp);

	out_x = zcomp.x;
	out_y = zcomp.y;
	out_z = zcomp.z;
}

void PMI::Orientation::SetZComponent(Point const & in_component)
{
	OrientationPrivate * my_impl = (OrientationPrivate *)impl_;

	memcpy(my_impl->m_matrix + 8, &in_component, 3 * sizeof(float));
}

void PMI::Orientation::SetZComponent(float in_x, float in_y, float in_z)
{
	SetZComponent(PMI::Point(in_x, in_y, in_z));
}

void PMI::Orientation::GetOriginComponent(Point & out_component) const
{
	OrientationPrivate * my_impl = (OrientationPrivate *)impl_;

	memcpy(&out_component, my_impl->m_matrix + 12, 3 * sizeof(float));
}

void PMI::Orientation::GetOriginComponent(float & out_x, float & out_y, float & out_z) const
{
	PMI::Point ocomp;
	GetOriginComponent(ocomp);

	out_x = ocomp.x;
	out_y = ocomp.y;
	out_z = ocomp.z;
}

void PMI::Orientation::SetOriginComponent(Point const & in_component)
{
	OrientationPrivate * my_impl = (OrientationPrivate *)impl_;
	
	memcpy(my_impl->m_matrix + 12, &in_component, 3 * sizeof(float));
}

void PMI::Orientation::SetOriginComponent(float in_x, float in_y, float in_z)
{
	SetOriginComponent(PMI::Point(in_x, in_y, in_z));
}

void PMI::Orientation::GetMatrix(float out_matrix[16]) const
{
	OrientationPrivate * my_impl = (OrientationPrivate *)impl_;

	memcpy(out_matrix, my_impl->m_matrix, 16 * sizeof(float));
}
	
void PMI::Orientation::SetMatrix(float const in_matrix[16])
{
	OrientationPrivate * my_impl = (OrientationPrivate *)impl_;

	memcpy(my_impl->m_matrix, in_matrix, 16 * sizeof(float));
}

// ==============================================

PMI::Polyline::Polyline()
{
	impl_ = new PolylinePrivate();
}

PMI::Polyline::Polyline(PMI::Polyline const & that)
{
	impl_ = new PolylinePrivate();
	Set(that);
}

PMI::Polyline::~Polyline()
{
	delete impl_;
}

void PMI::Polyline::Set(PMI::Polyline const & that)
{
	PolylinePrivate * my_impl = (PolylinePrivate *)impl_;
	PolylinePrivate * that_impl = (PolylinePrivate *)that.impl_;

	my_impl->Copy(that_impl);
}

PMI::Polyline const & PMI::Polyline::operator=(PMI::Polyline const & that)
{
	Set(that);
	return *this;
}

void PMI::Polyline::GetRGBColor(RGBColor & out_color) const
{
	PolylinePrivate * my_impl = (PolylinePrivate *)impl_;

	out_color = my_impl->m_color;
}

void PMI::Polyline::GetRGBColor(float & out_r, float & out_g, float & out_b) const
{
	PMI::RGBColor color;
	GetRGBColor(color);

	out_r = color.x;
	out_g = color.y;
	out_b = color.z;
}

void PMI::Polyline::SetRGBColor(RGBColor const & in_color)
{
	PolylinePrivate * my_impl = (PolylinePrivate *)impl_;

	my_impl->m_color = in_color;
}

void PMI::Polyline::SetRGBColor(float const in_r, float const in_g, float const in_b)
{
	PolylinePrivate * my_impl = (PolylinePrivate *)impl_;

	my_impl->m_color.Set(in_r, in_g, in_b);
}

void PMI::Polyline::GetLinePattern(char out_pattern[1024]) const
{
	PolylinePrivate * my_impl = (PolylinePrivate *)impl_;

	strncpy(out_pattern, my_impl->m_pattern, 1024);
}

void PMI::Polyline::SetLinePattern(char const in_pattern[1024])
{
	PolylinePrivate * my_impl = (PolylinePrivate *)impl_;

	strncpy(my_impl->m_pattern, in_pattern, 1024);
}

unsigned int PMI::Polyline::GetPointCount() const
{
	PolylinePrivate * my_impl = (PolylinePrivate *)impl_;

	return static_cast<unsigned int>(my_impl->m_points.size());
}

void PMI::Polyline::GetPoints(unsigned int & out_count, PMI::Point * out_points) const
{
	PolylinePrivate * my_impl = (PolylinePrivate *)impl_;

	out_count = GetPointCount();

	if (out_points == 0)
		return;

	for (unsigned int i = 0; i < out_count; i++)
		out_points[i] = my_impl->m_points[i];
}

void PMI::Polyline::GetPoints(unsigned int & out_count, float * out_points) const
{
	PolylinePrivate * my_impl = (PolylinePrivate *)impl_;

	out_count = GetPointCount();

	if (out_points == 0)
		return;

	for (unsigned int i = 0; i < out_count; i++)
		memcpy(out_points + 3 * i, &my_impl->m_points[i], sizeof(PMI::Point));
}

void PMI::Polyline::SetPoints(unsigned int in_count, PMI::Point const * in_points)
{
	PolylinePrivate * my_impl = (PolylinePrivate *)impl_;

	my_impl->m_points.resize(in_count);

	for (size_t i = 0; i < in_count; i++)
		my_impl->m_points[i] = in_points[i];
}

void PMI::Polyline::SetPoints(unsigned int in_count, float const * in_points)
{
	PolylinePrivate * my_impl = (PolylinePrivate *)impl_;

	my_impl->m_points.resize(in_count);

	for (size_t i = 0; i < in_count; i++)
		memcpy(&my_impl->m_points[i], in_points + 3 * i, sizeof(PMI::Point));
}

// ==============================================

PMI::Polygon::Polygon()
{
	impl_ = new PolygonPrivate();
}

PMI::Polygon::Polygon(PMI::Polygon const & that)
{
	impl_ = new PolygonPrivate();
	Set(that);
}

PMI::Polygon::~Polygon()
{
	delete impl_;
}

void PMI::Polygon::Set(PMI::Polygon const & that)
{
	PolygonPrivate * my_impl = (PolygonPrivate *)impl_;
	PolygonPrivate * that_impl = (PolygonPrivate *)that.impl_;

	my_impl->Copy(that_impl);
}

PMI::Polygon const & PMI::Polygon::operator=(PMI::Polygon const & that)
{
	Set(that);
	return *this;
}

void PMI::Polygon::GetRGBColor(RGBColor & out_color) const
{
	PolygonPrivate * my_impl = (PolygonPrivate *)impl_;

	out_color = my_impl->m_color;
}

void PMI::Polygon::GetRGBColor(float & out_r, float & out_g, float & out_b) const
{
	PMI::RGBColor color;
	GetRGBColor(color);

	out_r = color.x;
	out_g = color.y;
	out_b = color.z;
}

void PMI::Polygon::SetRGBColor(RGBColor const & in_color)
{
	PolygonPrivate * my_impl = (PolygonPrivate *)impl_;

	my_impl->m_color = in_color;
}

void PMI::Polygon::SetRGBColor(float const in_r, float const in_g, float const in_b)
{
	PolygonPrivate * my_impl = (PolygonPrivate *)impl_;

	my_impl->m_color.Set(in_r, in_g, in_b);
}

unsigned int PMI::Polygon::GetPointCount() const
{
	PolygonPrivate * my_impl = (PolygonPrivate *)impl_;

	return static_cast<unsigned int>(my_impl->m_points.size());
}

void PMI::Polygon::GetPoints(unsigned int & out_count, PMI::Point * out_points) const
{
	PolygonPrivate * my_impl = (PolygonPrivate *)impl_;

	out_count = GetPointCount();

	if (out_points == 0)
		return;

	for (unsigned int i = 0; i < out_count; i++)
		out_points[i] = my_impl->m_points[i];
}

void PMI::Polygon::GetPoints(unsigned int & out_count, float * out_points) const
{
	PolygonPrivate * my_impl = (PolygonPrivate *)impl_;

	out_count = GetPointCount();

	if (out_points == 0)
		return;

	for (unsigned int i = 0; i < out_count; i++)
		memcpy(out_points + 3 * i, &my_impl->m_points[i], sizeof(PMI::Point));
}

void PMI::Polygon::SetPoints(unsigned int in_count, PMI::Point const * in_points)
{
	PolygonPrivate * my_impl = (PolygonPrivate *)impl_;

	my_impl->m_points.resize(in_count);

	for (size_t i = 0; i < in_count; i++)
		my_impl->m_points[i] = in_points[i];
}

void PMI::Polygon::SetPoints(unsigned int in_count, float const * in_points)
{
	PolygonPrivate * my_impl = (PolygonPrivate *)impl_;

	my_impl->m_points.resize(in_count);

	for (size_t i = 0; i < in_count; i++)
		memcpy(&my_impl->m_points[i], in_points + 3 * i, sizeof(PMI::Point));
}

// ==============================================

PMI::TextAttributes::TextAttributes()
{
	impl_ = new TextAttributesPrivate();
}

PMI::TextAttributes::TextAttributes(PMI::TextAttributes const & that)
{
	impl_ = new TextAttributesPrivate();
	Set(that);
}

PMI::TextAttributes::~TextAttributes()
{
	delete impl_;
}

void PMI::TextAttributes::Set(PMI::TextAttributes const & that)
{
	TextAttributesPrivate * my_impl = (TextAttributesPrivate *)impl_;
	TextAttributesPrivate * that_impl = (TextAttributesPrivate *)that.impl_;

	my_impl->Copy(that_impl);
}

PMI::TextAttributes const & PMI::TextAttributes::operator=(PMI::TextAttributes const & that)
{
	Set(that);
	return *this;
}

void PMI::TextAttributes::GetInsertionPoint(PMI::Point & out_point) const
{
	TextAttributesPrivate * my_impl = (TextAttributesPrivate *)impl_;

	out_point.Set(my_impl->m_insertion_point);
}

void PMI::TextAttributes::GetInsertionPoint(float & out_x, float & out_y, float & out_z) const
{
	PMI::Point point;
	GetInsertionPoint(point);

	out_x = point.x;
	out_y = point.y;
	out_z = point.z;
}

void PMI::TextAttributes::SetInsertionPoint(PMI::Point const & in_point)
{
	TextAttributesPrivate * my_impl = (TextAttributesPrivate *)impl_;

	my_impl->m_insertion_point.Set(in_point);
}

void PMI::TextAttributes::SetInsertionPoint(float in_x, float in_y, float in_z)
{
	SetInsertionPoint(PMI::Point(in_x, in_y, in_z));
}

void PMI::TextAttributes::GetFontSize(float & out_size) const
{
	TextAttributesPrivate * my_impl = (TextAttributesPrivate *)impl_;

	out_size = my_impl->m_font_size;
}

void PMI::TextAttributes::SetFontSize(float const in_size)
{
	TextAttributesPrivate * my_impl = (TextAttributesPrivate *)impl_;
	
	my_impl->m_font_size = in_size;
}

void PMI::TextAttributes::GetFontName(char * out_font_name) const
{
	TextAttributesPrivate * my_impl = (TextAttributesPrivate *)impl_;

	out_font_name[0] = '\0';
	
	if (my_impl->m_font_name)
		strcpy(out_font_name, my_impl->m_font_name);
}

void PMI::TextAttributes::SetFontName(char const * in_font_name)
{
	TextAttributesPrivate * my_impl = (TextAttributesPrivate *)impl_;
	
	delete [] my_impl->m_font_name;

	size_t font_name_size = strlen(in_font_name);

	my_impl->m_font_name = new char [font_name_size + 1];
	strcpy(my_impl->m_font_name, in_font_name);
}

void PMI::TextAttributes::GetOrientation(PMI::Orientation & out_orientation) const
{
	TextAttributesPrivate * my_impl = (TextAttributesPrivate *)impl_;

	out_orientation = my_impl->m_orientation;
}

void PMI::TextAttributes::SetOrientation(PMI::Orientation const & in_orientation)
{
	TextAttributesPrivate * my_impl = (TextAttributesPrivate *)impl_;

	my_impl->m_orientation = in_orientation;
}

void PMI::TextAttributes::GetRGBColor(PMI::RGBColor & out_color) const
{
	TextAttributesPrivate * my_impl = (TextAttributesPrivate *)impl_;

	out_color = my_impl->m_color;
}

void PMI::TextAttributes::GetRGBColor(float & out_r, float & out_g, float & out_b) const
{
	PMI::RGBColor color;
	GetRGBColor(color);

	out_r = color.x;
	out_g = color.y;
	out_b = color.z;
}

void PMI::TextAttributes::SetRGBColor(PMI::RGBColor const & in_color)
{
	TextAttributesPrivate * my_impl = (TextAttributesPrivate *)impl_;

	my_impl->m_color = in_color;
}

void PMI::TextAttributes::SetRGBColor(float in_r, float in_g, float in_b)
{
	SetRGBColor(PMI::RGBColor(in_r, in_g, in_b));
}

bool PMI::TextAttributes::IsBold() const
{
	TextAttributesPrivate * my_impl = (TextAttributesPrivate *)impl_;

	return (my_impl->m_format & 0x02)==0x02;
}

bool PMI::TextAttributes::IsItalic() const
{
	TextAttributesPrivate * my_impl = (TextAttributesPrivate *)impl_;

	return (my_impl->m_format & 0x04)==0x04;
}
bool PMI::TextAttributes::IsUnderlined() const
{
	TextAttributesPrivate * my_impl = (TextAttributesPrivate *)impl_;

	return (my_impl->m_format & 0x08)==0x08;
}
bool PMI::TextAttributes::IsStrikedThrough() const
{
	TextAttributesPrivate * my_impl = (TextAttributesPrivate *)impl_;

	return (my_impl->m_format & 0x10)==0x10;
}
bool PMI::TextAttributes::IsOverlined() const
{
	TextAttributesPrivate * my_impl = (TextAttributesPrivate *)impl_;

	return (my_impl->m_format & 0x20)==0x20;
}
bool PMI::TextAttributes::IsStreched() const
{
	TextAttributesPrivate * my_impl = (TextAttributesPrivate *)impl_;

	return (my_impl->m_format & 0x40)==0x40;
}
bool PMI::TextAttributes::IsWired() const
{
	TextAttributesPrivate * my_impl = (TextAttributesPrivate *)impl_;

	return (my_impl->m_format & 0x80)==0x80;
}
bool PMI::TextAttributes::IsFixedWidth() const
{
	TextAttributesPrivate * my_impl = (TextAttributesPrivate *)impl_;

	return (my_impl->m_format & 0x100)==0x100;
}
void PMI::TextAttributes::SetFormat(char const & in_format)
{
	TextAttributesPrivate * my_impl = (TextAttributesPrivate *)impl_;

	my_impl->m_format = in_format;
}

double PMI::TextAttributes::WidthScale() const
{
	TextAttributesPrivate * my_impl = (TextAttributesPrivate *)impl_;

	return my_impl->m_width_scale;
}

void PMI::TextAttributes::SetWidthScale(const double width_scale)
{
	TextAttributesPrivate * my_impl = (TextAttributesPrivate *)impl_;

	my_impl->m_width_scale = static_cast<float>(width_scale);
}
// ==============================================

PMI::String::String()
{
	impl_ = new StringPrivate();
}

PMI::String::String(char const * cstring)
{
	impl_ = new StringPrivate(cstring);
}

PMI::String::String(wchar_t const * wstring)
{
	impl_ = new StringPrivate(wstring);
}

PMI::String::String(H_WCS const & wcs)
{
	impl_ = new StringPrivate(wcs);
}

PMI::String::String(H_UTF8 const & utf8)
{
	impl_ = new StringPrivate(utf8);
}

PMI::String::String(PMI::String const & that)
{
	impl_ = new StringPrivate();
	Set(that);
}

PMI::String::~String()
{
	delete impl_;
}

void PMI::String::Set(PMI::String const & in_that)
{
	StringPrivate * my_impl = (StringPrivate *)impl_;
	StringPrivate * that_impl = (StringPrivate *)in_that.impl_;

	*my_impl->m_wcs = *that_impl->m_wcs;
}

PMI::String const & PMI::String::operator=(PMI::String const & in_that)
{
	Set(in_that);
	return *this;
}

char const * PMI::String::CStr() const
{
	StringPrivate * my_impl = (StringPrivate *)impl_;
	delete my_impl->m_ascii;
	my_impl->m_ascii = new HUtilityAsciiStr(my_impl->m_wcs->encodedText());
	return (char const *)(*my_impl->m_ascii);
}

wchar_t const * PMI::String::WStr() const
{
	StringPrivate * my_impl = (StringPrivate *)impl_;
	return my_impl->m_wcs->encodedText();
}
size_t PMI::String::WLength() const
{
	StringPrivate * my_impl = (StringPrivate *)impl_;
	return my_impl->m_wcs->length();
}

bool PMI::String::Equals(PMI::String const & in_that) const
{
	StringPrivate * my_impl = (StringPrivate *)impl_;
	StringPrivate * that_impl = (StringPrivate *)in_that.impl_;

	return (*my_impl->m_wcs == *that_impl->m_wcs);
}

bool PMI::String::operator==(String const & in_that) const
{
	return Equals(in_that);
}

bool PMI::String::operator!=(String const & in_that) const
{
	return !Equals(in_that);
}

// ==============================================

PMI::Options::Options()
{
	impl_ = new OptionsPrivate();
}

PMI::Options::Options(PMI::Options const & that)
{
	impl_ = new OptionsPrivate();
	Set(that);
}

PMI::Options::~Options()
{
	delete impl_;
}

void PMI::Options::Set(PMI::Options const & that)
{
	OptionsPrivate * my_impl = (OptionsPrivate *)impl_;
	OptionsPrivate * that_impl = (OptionsPrivate *)that.impl_;

	my_impl->Copy(that_impl);
}

PMI::Options const & PMI::Options::operator=(PMI::Options const & that)
{
	Set(that);
	return *this;
}


bool PMI::Options::IsDisplayParallelToScreen() const
{
	OptionsPrivate * my_impl = (OptionsPrivate *)impl_;

	return my_impl->m_display_parallel_to_screen;
}

void PMI::Options::SetDisplayParallelToScreen(const bool in_parallel)
{
	OptionsPrivate * my_impl = (OptionsPrivate *)impl_;

	my_impl->m_display_parallel_to_screen = in_parallel;
}
// ======================= end common classes =======================


// ====================== start actual entities =======================


static inline void copy_impls(PrivateImpl * this_impl, PrivateImpl * that_impl)
{
	BasePrivate * this_base_impl = (BasePrivate *)this_impl;
	BasePrivate * that_base_impl = (BasePrivate *)that_impl;

	this_base_impl->Copy(that_base_impl);
}

static inline void get_frame(HC_KEY in_segment_key, PMI::Frame & out_frame)
{
	out_frame = PMI::Frame();

	if (in_segment_key == INVALID_KEY)
		return;

	char type[64];
	HC_KEY key;
	int i = 0;
	PolylineArray frame_polylines;

	HC_Open_Segment_By_Key(in_segment_key);
	{
		HC_Open_Segment("frame");
		{
			HC_Begin_Contents_Search("...", "polylines, lines");
			{
				while (HC_Find_Contents(type, &key))
				{
					PMI::Polyline polyline;

					int count;
					if (streq(type, "line"))
						count = 2;
					else
						HC_Show_Polyline_Count(key, &count);

					PMI::Point * points = new PMI::Point [count];
					if (streq(type, "line"))
						HC_Show_Line(key, &points[0].x, &points[0].y, &points[0].z, &points[1].x, &points[1].y, &points[1].z);
					else
						HC_Show_Polyline(key, &count, points);

					polyline.SetPoints(count, points);

					delete [] points;

					HC_Open_Segment_By_Key(HC_KShow_Owner_By_Key(key));
					{
						if (HC_Show_Existence("color = lines"))
						{
							PMI::RGBColor color;
							char color_space[4] = { "" };
							HC_Show_One_Color_By_Value("lines", color_space, &color.x, &color.y, &color.z);
							polyline.SetRGBColor(color);
						}

						if (HC_Show_Existence("line pattern"))
						{
							char pattern[1024] = { "" };
							HC_Show_Line_Pattern(pattern);
							polyline.SetLinePattern(pattern);
						}
					}
					HC_Close_Segment();

					frame_polylines.push_back(polyline);

					i++;
				}
			}
			HC_End_Contents_Search();
		}
		HC_Close_Segment();
	}
	HC_Close_Segment();

	unsigned int count = (unsigned int)frame_polylines.size();
	PMI::Polyline * polylines = new PMI::Polyline [count];
	for (unsigned int i = 0; i < count; i++)
		polylines[i] = frame_polylines[i];

	out_frame.SetPolylines(count, polylines);

	delete [] polylines;
}

static void set_color(const PMI::Polyline& polyline)
{
	PMI::RGBColor color;
	polyline.GetRGBColor(color);

	if (color.x >= 0 && color.y >= 0 && color.z >= 0)
		HC_Set_Color_By_Value("lines", "RGB", color.x, color.y, color.z);

	char pattern[1024] = { "" };
	polyline.GetLinePattern(pattern);

	if (pattern[0] != '\0')
		HC_Set_Line_Pattern(pattern);

}

static void set_color(const PMI::Polygon& polygon)
{
	PMI::RGBColor color;
	polygon.GetRGBColor(color);

	if (color.x >= 0 && color.y >= 0 && color.z >= 0)
		HC_Set_Color_By_Value("faces", "RGB", color.x, color.y, color.z);
}

static inline void set_frame(HC_KEY in_segment_key, PMI::Frame const & in_frame)
{

	PMI::RGBColor color_current;
	PMI::RGBColor color;
	char pattern[1024] = { "" };
	char pattern_current[1024] = { "" };
	if (in_segment_key == INVALID_KEY)
		return;

	unsigned int polyline_count = in_frame.GetPolylineCount();
	PMI::Polyline * polylines = new PMI::Polyline [polyline_count];
	in_frame.GetPolylines(polyline_count, polylines);

	HC_Open_Segment_By_Key(in_segment_key);
	{
		HC_Open_Segment("frame");
		{
			HC_Flush_Contents(".", "everything");

			polylines[0].GetRGBColor(color_current);
			polylines[0].GetLinePattern(pattern_current);

			HC_Open_Segment("");
			set_color(polylines[0]);

			for (unsigned int i = 0; i < polyline_count; i++)
			{
				//new segment if new graphics

				polylines[i].GetRGBColor(color);
				polylines[i].GetLinePattern(pattern);
				if( color!=color_current
					|| strcmp (pattern_current, pattern) !=0)
				{
					HC_Close_Segment();
					HC_Open_Segment("");
					set_color(polylines[i]);
					sprintf(pattern_current, "%s", pattern);
					color_current = color;
				}

				unsigned int point_count;
				polylines[i].GetPoints(point_count, (PMI::Point *)0);
				PMI::Point * points = new PMI::Point [point_count];
				polylines[i].GetPoints(point_count, points);
				HC_Insert_Polyline(point_count, points);
				delete [] points;
			}
			HC_Close_Segment();
		}
		HC_Close_Segment();
	}
	HC_Close_Segment();

	delete [] polylines;
}

static inline void set_polygons(HC_KEY in_segment_key, char const * in_segment_name, unsigned int in_count, PMI::Polygon const * in_polygons)
{
	if (in_segment_key == INVALID_KEY)
		return;

	HC_Open_Segment_By_Key(in_segment_key);
	{
		HC_Open_Segment(in_segment_name);
		{
			PMI::RGBColor color_current;

			HC_Flush_Contents(".", "everything");

			for (unsigned int i = 0; i < in_count; i++)
			{
				//new segment if new graphics

				if (i == 0)
				{
					in_polygons[0].GetRGBColor(color_current);

					HC_Open_Segment("");
					set_color(in_polygons[0]);
				}

				PMI::RGBColor color;
				in_polygons[i].GetRGBColor(color);
				if (color != color_current)
				{
					HC_Close_Segment();
					HC_Open_Segment("");
					set_color(in_polygons[i]);
					color_current = color;
				}

				unsigned int point_count;
				in_polygons[i].GetPoints(point_count, (PMI::Point *)0);
				PMI::Point * points = new PMI::Point [point_count];
				in_polygons[i].GetPoints(point_count, points);
				HC_Insert_Polygon(point_count, points);
				delete [] points;
			}
			HC_Close_Segment();
		}
		HC_Close_Segment();
	}
	HC_Close_Segment();
}


static inline unsigned int get_polygon_count(HC_KEY in_segment_key, char const * in_segment_name)
{
	if (in_segment_key == INVALID_KEY)
		return 0;

	int count = 0;

	HC_Open_Segment_By_Key(in_segment_key);
	{
		HC_Open_Segment(in_segment_name);
		{
			HC_Begin_Contents_Search("...", "polygons");
			{
				HC_Show_Contents_Count(&count);
			}
			HC_End_Contents_Search();
		}
		HC_Close_Segment();
	}
	HC_Close_Segment();

	return (unsigned int)count;
}


static inline void get_polygons(HC_KEY in_segment_key, char const * in_segment_name, PMI::Polygon * out_polygons)
{
	if (in_segment_key == INVALID_KEY)
		return;

	HC_Open_Segment_By_Key(in_segment_key);
	{
		HC_Open_Segment(in_segment_name);
		{
			HC_Begin_Contents_Search("...", "polygons");
			{
				int i = 0;
				HC_KEY key;
				while (HC_Find_Contents(nullptr, &key))
				{
					PMI::Polygon polygon;

					int count;
					HC_Show_Polygon_Count(key, &count);

					PMI::Point * points = new PMI::Point [count];
					HC_Show_Polygon(key, &count, points);

					polygon.SetPoints(count, points);

					delete [] points;

					HC_Open_Segment_By_Key(HC_KShow_Owner_By_Key(key));
					{
						if (HC_Show_Existence("color = faces"))
						{
							PMI::RGBColor color;
							char color_space[4] = { "" };
							HC_Show_One_Color_By_Value("faces", color_space, &color.x, &color.y, &color.z);
							polygon.SetRGBColor(color);
						}
					}
					HC_Close_Segment();

					out_polygons[i] = polygon;

					i++;
				}
			}
			HC_End_Contents_Search();
		}
		HC_Close_Segment();
	}
	HC_Close_Segment();
}


// ==============================================

PMI::Entity::Entity(HC_KEY segment_key)
{
	impl_ = new BasePrivate();

	BasePrivate * my_impl = (BasePrivate *)impl_;
	my_impl->m_segment_key = segment_key;
}

PMI::Entity::Entity(PMI::Entity const & that) 
{
	impl_ = new BasePrivate();

	Set(that);
}

PMI::Entity::~Entity()
{
	delete impl_;
}

void PMI::Entity::Set(PMI::Entity const & that)
{
	copy_impls(impl_, that.impl_);
}

PMI::Entity const & PMI::Entity::operator=(PMI::Entity const & that)
{
	Set(that);
	return *this;
}

HC_KEY PMI::Entity::GetSegmentKey() const
{
	BasePrivate * my_impl = (BasePrivate *)impl_;

	return my_impl->m_segment_key;
}

void PMI::Entity::GetFrame(PMI::Frame & out_frame) const
{
	BasePrivate * my_impl = (BasePrivate *)impl_;

	get_frame(my_impl->m_segment_key, out_frame);
}

void PMI::Entity::SetFrame(PMI::Frame const & in_frame)
{
	BasePrivate * my_impl = (BasePrivate *)impl_;

	set_frame(my_impl->m_segment_key, in_frame);
}


void PMI::Entity::GetDrawing(PMI::Drawing & out_drawing) const
{
	out_drawing = PMI::Drawing();

	BasePrivate * my_impl = (BasePrivate *)impl_;

	unsigned int count = get_polygon_count(my_impl->m_segment_key, "drawing");
	if (count == 0)
		return;

	PolygonArray polygons(count);
	get_polygons(my_impl->m_segment_key, "drawing", polygons.data());

	out_drawing.SetPolygons(count, polygons.data());
}

void PMI::Entity::SetDrawing(PMI::Drawing const & in_drawing)
{
	BasePrivate * my_impl = (BasePrivate *)impl_;

	unsigned int count = in_drawing.GetPolygonCount();
	PolygonArray polygons(count);
	in_drawing.GetPolygons(count, polygons.data());

	set_polygons(my_impl->m_segment_key, "drawing", count, polygons.data());
}

unsigned int PMI::Entity::GetLeaderLineCount() const
{
	BasePrivate * my_impl = (BasePrivate *)impl_;

	if (my_impl->m_segment_key == INVALID_KEY)
		return 0;

	int count = 0;

	HC_Open_Segment_By_Key(my_impl->m_segment_key);
	{
		HC_Open_Segment("leader");
		{
			HC_Begin_Contents_Search("...", "polylines, lines");
			{
				HC_Show_Contents_Count(&count);
			}
			HC_End_Contents_Search();
		}
		HC_Close_Segment();
	}
	HC_Close_Segment();

	return static_cast<unsigned int>(count);
}

void PMI::Entity::GetLeaderLines(unsigned int & out_count, PMI::Polyline * out_leader_lines) const
{
	out_count = 0;

	BasePrivate * my_impl = (BasePrivate *)impl_;

	if (my_impl->m_segment_key == INVALID_KEY)
		return;

	out_count = GetLeaderLineCount();

	if (out_leader_lines == 0)
		return;

	HC_KEY key;
	char type[64];
	unsigned int i = 0;

	HC_Open_Segment_By_Key(my_impl->m_segment_key);
	{
		HC_Open_Segment("leader");
		{
			HC_Begin_Contents_Search("...", "polylines, lines");
			{
				while (HC_Find_Contents(type, &key))
				{
					PMI::Polyline polyline;

					int count;
					if (streq(type, "line"))
						count = 2;
					else
						HC_Show_Polyline_Count(key, &count);

					PMI::Point * points = new PMI::Point [count];
					if (streq(type, "line"))
						HC_Show_Line(key, &points[0].x, &points[0].y, &points[0].z, &points[1].x, &points[1].y, &points[1].z);
					else
						HC_Show_Polyline(key, &count, points);

					polyline.SetPoints(count, points);

					delete [] points;

					HC_Open_Segment_By_Key(HC_KShow_Owner_By_Key(key));
					{
						if (HC_Show_Existence("color = lines"))
						{
							PMI::RGBColor color;
							char color_space[4] = { "" };
							HC_Show_One_Color_By_Value("lines", color_space, &color.x, &color.y, &color.z);
							polyline.SetRGBColor(color);
						}

						if (HC_Show_Existence("line pattern"))
						{
							char pattern[1024] = { "" };
		
							HC_Show_Line_Pattern(pattern);
		
							polyline.SetLinePattern(pattern);
						}
					}
					HC_Close_Segment();

					out_leader_lines[i++] = polyline;
				}
			}
			HC_End_Contents_Search();
		}
		HC_Close_Segment();
	}
	HC_Close_Segment();

	assert(i == out_count);
}

void PMI::Entity::SetLeaderLines(unsigned int in_count, PMI::Polyline const * in_leader_lines)
{
	BasePrivate * my_impl = (BasePrivate *)impl_;

	if (my_impl->m_segment_key == INVALID_KEY)
		return;

	HC_Open_Segment_By_Key(my_impl->m_segment_key);
	{
		HC_Open_Segment("leader");
		{
			HC_Flush_Contents(".", "everything");

			for (unsigned int i = 0; i < in_count; i++)
			{
				HC_Open_Segment("");
				{
					unsigned int count;
					in_leader_lines[i].GetPoints(count, (PMI::Point *)0);
					PMI::Point * points = new PMI::Point [count];
					in_leader_lines[i].GetPoints(count, points);

					HC_Insert_Polyline(count, points);

					delete [] points;

					PMI::RGBColor color;
					in_leader_lines[i].GetRGBColor(color);

					if (color.x >= 0 && color.y >= 0 && color.z >= 0)
						HC_Set_Color_By_Value("lines", "RGB", color.x, color.y, color.z);

					char pattern[1024] = { "" };
					in_leader_lines[i].GetLinePattern(pattern);

					if (pattern[0] != '\0')
						HC_Set_Line_Pattern(pattern);
				}
				HC_Close_Segment();
			}
		}
		HC_Close_Segment();
	}
	HC_Close_Segment();
}

void PMI::Entity::SetLeaderSymbols(unsigned int in_count, PMI::Polygon const * in_leader_symbols)
{
	BasePrivate * my_impl = (BasePrivate *)impl_;

	if (my_impl->m_segment_key == INVALID_KEY)
		return;

	set_polygons(my_impl->m_segment_key, "leader/symbol", in_count, in_leader_symbols);
}

unsigned int PMI::Entity::GetLeaderSymbolCount() const
{
	BasePrivate * my_impl = (BasePrivate *)impl_;

	return get_polygon_count(my_impl->m_segment_key, "leader/symbol");
}

void PMI::Entity::GetLeaderSymbols(unsigned int & out_count, Polygon out_leader_symbols[]) const
{
	out_count = 0;

	BasePrivate * my_impl = (BasePrivate *)impl_;

	if (my_impl->m_segment_key == INVALID_KEY)
		return;

	out_count = GetLeaderSymbolCount();

	if (out_count == 0 || out_leader_symbols == 0)
		return;

	get_polygons(my_impl->m_segment_key, "leader/symbol", out_leader_symbols);
}

void PMI::Entity::SetDisplayParallelToScreen(bool const in_parallel)
{
	BasePrivate * my_impl = (BasePrivate *)impl_;
	if (my_impl->m_segment_key == INVALID_KEY)
		return;
	HC_Open_Segment_By_Key(my_impl->m_segment_key);
	{
		HC_Open_Segment("text");
		{
			if (in_parallel)
			{
				HC_Set_Rendering_Options("depth range = (0.0,0.1)");
				HC_Set_Text_Font("transforms = off");
			}
			else
				HC_Set_Text_Font("transforms = on");
		}
		HC_Close_Segment();
	}
	HC_Close_Segment();
}

bool PMI::Entity::IsDisplayParallelToScreen() const
{
	bool is_parallel_to_screen = false;
	BasePrivate * my_impl = (BasePrivate *)impl_;
	if (my_impl->m_segment_key == INVALID_KEY)
		return is_parallel_to_screen;
	HC_Open_Segment_By_Key(my_impl->m_segment_key);
	{
		int text_segment_count = 0;
		HC_Begin_Segment_Search("text");
		{
			HC_Show_Segment_Count(&text_segment_count);
		}
		HC_End_Segment_Search();
		
		if (text_segment_count > 0)
		{
			char cval[MVO_BUFFER_SIZE];
			HC_Open_Segment("text");
			{
				HC_Show_One_Net_Text_Font ("transforms", cval);
				if (streq(cval, "off"))
					is_parallel_to_screen = true;
			}
			HC_Close_Segment();
		}
	}
	HC_Close_Segment();
	return is_parallel_to_screen;
}

// ==============================================

static inline unsigned int get_strings_and_text_attributes_count(HC_KEY in_segment_key, char const * in_segment_name)
{
	if (in_segment_key == INVALID_KEY)
		return 0;

	int count = 0;

	HC_Open_Segment_By_Key(in_segment_key);
	{
		HC_Open_Segment(H_FORMAT_TEXT("text/%s", in_segment_name));
		{
			HC_Begin_Contents_Search("...", "text");
			{
				HC_Show_Contents_Count(&count);
			}
			HC_End_Contents_Search();
		}
		HC_Close_Segment();
	}
	HC_Close_Segment();

	return static_cast<unsigned int>(count);
}

static inline void get_strings_and_text_attributes(HC_KEY in_segment_key, char const * in_segment_name, PMI::String * out_strings, 
		PMI::TextAttributes * out_text_attributes)
{
	char type[64];
	HC_KEY key;
	int i = 0;

	if (in_segment_key == INVALID_KEY)
		return;

	HC_Open_Segment_By_Key(in_segment_key);
	{
		HC_Open_Segment("text");
		{
			bool is_parallel_to_screen = false;
			if (HC_Show_Existence("text font=transforms"))
			{
				char cval[64];
				HC_Show_One_Text_Font("transforms", cval);
				is_parallel_to_screen = streq(cval, "off");
			}

			HC_Open_Segment(in_segment_name);
			{
				HC_Begin_Contents_Search("...", "text");
				{
					while (HC_Find_Contents(type, &key))
					{
						int length = 0;
						HC_Show_Text_Length(key, &length);

						PMI::Point insertion_point;
						char * text = new char [length + 1];

						HC_Show_Text(key, &insertion_point.x, &insertion_point.y, &insertion_point.z, text);

						if (is_parallel_to_screen)
						{
							char cval[64];
							HC_Show_One_Character_Attribute(key, 0, "horizontal offset", cval);
							sscanf(cval, "%f points", &insertion_point.x);
							HC_Show_One_Character_Attribute(key, 0, "vertical offset", cval);
							sscanf(cval, "%f points", &insertion_point.y);
						}

						if (out_strings)
						{
							H_UTF8 utf8;
							utf8.encodedText(text);
							
							out_strings[i] = PMI::String(utf8);
						}

						delete [] text;

						if (out_text_attributes)
						{
							out_text_attributes[i] = PMI::TextAttributes();

							out_text_attributes[i].SetInsertionPoint(insertion_point);
		
							HC_Open_Segment_By_Key(HC_KShow_Owner_By_Key(key));
							{
								if (HC_Show_Existence("color = text"))
								{
									PMI::RGBColor color;
									char color_space[4] = { "" };
									HC_Show_One_Color_By_Value("text", color_space, &color.x, &color.y, &color.z);
									out_text_attributes[i].SetRGBColor(color);
								}
		
								char options[1024];
		
								float font_size = 0.0f;
								HC_Show_One_Text_Font("size", options);
								HCLOCALE(sscanf(options, "%f world", &font_size));
								out_text_attributes[i].SetFontSize(font_size);
		
								HC_Show_One_Text_Font("name", options);
								out_text_attributes[i].SetFontName(options);
		
								PMI::Orientation orientation;
								float matrix[16];
								HC_Show_Modelling_Matrix(matrix);
								orientation.SetMatrix(matrix);
								out_text_attributes[i].SetOrientation(orientation);

								if (HC_Show_Existence("text font = width scale"))
								{
									HC_Show_One_Text_Font("width scale", options);
									out_text_attributes[i].SetWidthScale(atof(options));
								}
							}
							HC_Close_Segment();
						}

						i++;
					}
				}
				HC_End_Contents_Search();
			}
			HC_Close_Segment();
		}
		HC_Close_Segment();
	}
	HC_Close_Segment();
}

static inline void set_strings_and_text_attributes(HC_KEY in_segment_key, char const * in_segment_name, unsigned int in_count,
		PMI::String const * in_strings, PMI::TextAttributes const * in_text_attributes, bool is_parallel_to_screen = false)
{
	if (in_segment_key == INVALID_KEY)
		return;

	HC_Open_Segment_By_Key(in_segment_key);
	{
		HC_Open_Segment("text");
		{
			HC_Set_Text_Alignment("<v");
			HC_Set_Text_Font(H_FORMAT_TEXT("transforms = %s", (is_parallel_to_screen ? "off" : "on")));
			HC_Set_Heuristics("no culling");

			HC_Open_Segment(in_segment_name);
			{
				HC_Flush_Contents(".", "everything");

				for (size_t i = 0; i < in_count; i++)
				{
					HC_Open_Segment("");
					{
						PMI::Point insertion_point;
						in_text_attributes[i].GetInsertionPoint(insertion_point);
			
						H_UTF8 utf8(in_strings[i].WStr());
			
						if(is_parallel_to_screen)
						{
							HC_KEY text = HC_Insert_Text_With_Encoding(0., 0., 0., "utf8", utf8.encodedText());

							char character_attributes[MVO_BUFFER_SIZE];
							sprintf(character_attributes, "horizontal offset= %d.points", (int)insertion_point.x);
							HC_MSet_Character_Attributes(text, 0, (int)in_strings[i].WLength(), character_attributes);
							sprintf(character_attributes, "vertical offset= %d.points", (int)insertion_point.y);
							HC_MSet_Character_Attributes(text, 0, (int)in_strings[i].WLength(), character_attributes);
						}
						else
							HC_Insert_Text_With_Encoding(insertion_point.x, insertion_point.y, insertion_point.z, "utf8", utf8.encodedText());

						float font_size;
						in_text_attributes[i].GetFontSize(font_size);

						char font_name[1024];
						in_text_attributes[i].GetFontName(font_name);

						if (font_name[0] != '\0')
						{
							if(is_parallel_to_screen)
								HC_Set_Text_Font(H_FORMAT_TEXT("name = %s, size = %f pixels", font_name, font_size));
							else
							HC_Set_Text_Font(H_FORMAT_TEXT("name = %s, size = %f world", font_name, font_size));
						}
						if(in_text_attributes[i].WidthScale()!=1.)
							HC_Set_Text_Font(H_FORMAT_TEXT("width scale=%f", in_text_attributes[i].WidthScale()));


						if(in_text_attributes[i].IsBold())
							HC_Set_Text_Font("bold = on");
						if(in_text_attributes[i].IsItalic())
							HC_Set_Text_Font("italic = on");
						if(in_text_attributes[i].IsOverlined())
							HC_Set_Text_Font("overline = on");
						if(in_text_attributes[i].IsStrikedThrough())
							HC_Set_Text_Font("strikethrough  = on");
						if(in_text_attributes[i].IsUnderlined())
							HC_Set_Text_Font("underline  = on");

						if(in_text_attributes[i].IsWired())
							HC_Set_Text_Font("exterior = on");

						PMI::RGBColor color;
						in_text_attributes[i].GetRGBColor(color);
						
						if (color.x >= 0 && color.y >= 0 && color.z >= 0)
							HC_Set_Color_By_Value("text", "RGB", color.x, color.y, color.z);

						PMI::Orientation orientation;
						in_text_attributes[i].GetOrientation(orientation);
						
						float matrix[16];
						orientation.GetMatrix(matrix);

						HC_Set_Modelling_Matrix(matrix);
					}
					HC_Close_Segment();
				}
			}
			HC_Close_Segment();
		}
		HC_Close_Segment();
	}
	HC_Close_Segment();
}

// ==============================================

PMI::DatumEntity::DatumEntity(HC_KEY segment_key) : PMI::Entity(segment_key)
{
	if (segment_key != INVALID_KEY)
	{
		HC_Open_Segment_By_Key(segment_key);
		{
			HC_Set_User_Options("pmi_type = datum");
		}
		HC_Close_Segment();
	}
}

PMI::DatumEntity::DatumEntity(PMI::DatumEntity const & that) 
{
	Set(that);
}

PMI::DatumEntity::~DatumEntity() {}

void PMI::DatumEntity::Set(PMI::DatumEntity const & that)
{
	copy_impls(impl_, that.impl_);
}

PMI::DatumEntity const & PMI::DatumEntity::operator=(PMI::DatumEntity const & that)
{
	Set(that);
	return *this;
}

PMI::Datum::Type PMI::DatumEntity::GetDatumType() const
{
	BasePrivate * my_impl = (BasePrivate *)impl_;
	PMI::Datum::Type datum_type = PMI::Datum::Unknown;

	if (my_impl->m_segment_key == INVALID_KEY)
		return datum_type;

	HC_Open_Segment_By_Key(my_impl->m_segment_key);
	{
		if (HC_Show_Existence("user options = datum_type"))
		{
			char type[1024];

			HC_Show_One_User_Option("datum_type", type);

			if (streq(type, "identifier"))
				datum_type = PMI::Datum::Identifier;
			else if (streq(type, "target"))
				datum_type = PMI::Datum::Target;
			else
				assert(0);
		}
	}
	HC_Close_Segment();

	return datum_type;
}

void PMI::DatumEntity::SetDatumType(PMI::Datum::Type const in_type)
{
	BasePrivate * my_impl = (BasePrivate *)impl_;

	if (my_impl->m_segment_key == INVALID_KEY)
		return;

	HC_Open_Segment_By_Key(my_impl->m_segment_key);
	{
		switch(in_type)
		{
			case PMI::Datum::Identifier:
				HC_Set_User_Options("datum_type = identifier");
				break;
			case PMI::Datum::Target:
				HC_Set_User_Options("datum_type = target");
				break;
			case PMI::Datum::Unknown:
				HC_UnSet_One_User_Option("datum_type");
				break;
			default:
				assert(0);
		}
	}
	HC_Close_Segment();
}

unsigned int PMI::DatumEntity::GetLabelCount() const
{
	BasePrivate * my_impl = (BasePrivate *)impl_;

	return get_strings_and_text_attributes_count(my_impl->m_segment_key, "labels");
}

void PMI::DatumEntity::GetLabels(unsigned int & out_count, PMI::String * out_labels, PMI::TextAttributes * out_text_attributes) const
{
	BasePrivate * my_impl = (BasePrivate *)impl_;

	out_count = GetLabelCount();

	if (out_count == 0 || (out_labels == 0 && out_text_attributes == 0))
		return;

	get_strings_and_text_attributes(my_impl->m_segment_key, "labels", out_labels, out_text_attributes);
}

void PMI::DatumEntity::SetLabels(unsigned int in_count, PMI::String const * in_labels, PMI::TextAttributes const * in_text_attributes)
{
	BasePrivate * my_impl = (BasePrivate *)impl_;

	set_strings_and_text_attributes(my_impl->m_segment_key, "labels", in_count, in_labels, in_text_attributes, this->IsDisplayParallelToScreen());
}

// ==============================================

PMI::DimensionEntity::DimensionEntity(HC_KEY segment_key) : PMI::Entity(segment_key)
{ 
	if (segment_key != INVALID_KEY)
	{
		HC_Open_Segment_By_Key(segment_key);
		{
			HC_Set_User_Options("pmi_type = dimension");
		}
		HC_Close_Segment();
	}
}

PMI::DimensionEntity::DimensionEntity(PMI::DimensionEntity const & that)
{
	Set(that);
}

PMI::DimensionEntity::~DimensionEntity() {}

void PMI::DimensionEntity::Set(PMI::DimensionEntity const & that)
{
	copy_impls(impl_, that.impl_);
}

PMI::DimensionEntity const & PMI::DimensionEntity::operator=(PMI::DimensionEntity const & that)
{
	Set(that);
	return *this;
}

PMI::Dimension::Type PMI::DimensionEntity::GetDimensionType() const
{
	BasePrivate * my_impl = (BasePrivate *)impl_;
	PMI::Dimension::Type dimension_type = PMI::Dimension::UnknownType;

	if (my_impl->m_segment_key == INVALID_KEY)
		return dimension_type;

	HC_Open_Segment_By_Key(my_impl->m_segment_key);
	{
		if (HC_Show_Existence("user options = dimension_type"))
		{
			char type[1024];

			HC_Show_One_User_Option("dimension_type", type);

			if (streq(type, "basic"))
				dimension_type = PMI::Dimension::BasicType;
			else if (streq(type, "reference"))
				dimension_type = PMI::Dimension::ReferenceType;
			else if (streq(type, "tolerance"))
				dimension_type = PMI::Dimension::ToleranceType;
			else
				assert(0);
		}
	}
	HC_Close_Segment();

	return dimension_type;
}

void PMI::DimensionEntity::SetDimensionType(PMI::Dimension::Type const in_type)
{
	BasePrivate * my_impl = (BasePrivate *)impl_;

	if (my_impl->m_segment_key == INVALID_KEY)
		return;

	HC_Open_Segment_By_Key(my_impl->m_segment_key);
	{
		switch (in_type)
		{
			case PMI::Dimension::BasicType:
				HC_Set_User_Options("dimension_type = basic");
				break;
			case PMI::Dimension::ReferenceType:
				HC_Set_User_Options("dimension_type = reference");
				break;
			case PMI::Dimension::ToleranceType:
				HC_Set_User_Options("dimension_type = tolerance");
				break;
			case PMI::Dimension::UnknownType:
				HC_UnSet_One_User_Option("dimension_type");
				break;
			default:
				assert(0);
		}
	}
	HC_Close_Segment();
}

PMI::Dimension::SubType PMI::DimensionEntity::GetDimensionSubType() const
{
	BasePrivate * my_impl = (BasePrivate *)impl_;
	PMI::Dimension::SubType dimension_subtype = PMI::Dimension::UnknownSubType;

	if (my_impl->m_segment_key == INVALID_KEY)
		return dimension_subtype;

	HC_Open_Segment_By_Key(my_impl->m_segment_key);
	{
		if (HC_Show_Existence("user options = dimension_subtype"))
		{
			char subtype[1024];

			HC_Show_One_User_Option("dimension_subtype", subtype);

			if (streq(subtype, "angle"))
				dimension_subtype = PMI::Dimension::AngleSubType;
			else if (streq(subtype, "chamfer"))
				dimension_subtype = PMI::Dimension::ChamferSubType;
			else if (streq(subtype, "diameter"))
				dimension_subtype = PMI::Dimension::DiameterSubType;
			else if (streq(subtype, "distance"))
				dimension_subtype = PMI::Dimension::DistanceSubType;
			else if (streq(subtype, "length"))
				dimension_subtype = PMI::Dimension::LengthSubType;
			else if (streq(subtype, "radius"))
				dimension_subtype = PMI::Dimension::RadiusSubType;
			else if (streq(subtype, "slope"))
				dimension_subtype = PMI::Dimension::SlopeSubType;
			else
				assert(0);
		}
	}
	HC_Close_Segment();

	return dimension_subtype;
}

void PMI::DimensionEntity::SetDimensionSubType(PMI::Dimension::SubType const in_sub_type)
{
	BasePrivate * my_impl = (BasePrivate *)impl_;

	if (my_impl->m_segment_key == INVALID_KEY)
		return;

	HC_Open_Segment_By_Key(my_impl->m_segment_key);
	{
		switch (in_sub_type)
		{
			case PMI::Dimension::AngleSubType:
				HC_Set_User_Options("dimension_subtype = angle");
				break;
			case PMI::Dimension::ChamferSubType:
				HC_Set_User_Options("dimension_subtype = chamfer");
				break;
			case PMI::Dimension::DiameterSubType:
				HC_Set_User_Options("dimension_subtype = diameter");
				break;
			case PMI::Dimension::DistanceSubType:
				HC_Set_User_Options("dimension_subtype = distance");
				break;
			case PMI::Dimension::LengthSubType:
				HC_Set_User_Options("dimension_subtype = length");
				break;
			case PMI::Dimension::RadiusSubType:
				HC_Set_User_Options("dimension_subtype = radius");
				break;
			case PMI::Dimension::SlopeSubType:
				HC_Set_User_Options("dimension_subtype = slope");
				break;
			case PMI::Dimension::UnknownSubType:
				HC_UnSet_One_User_Option("dimension_subtype");
				break;
			default:
				assert(0);
		}
	}
	HC_Close_Segment();
}

unsigned int PMI::DimensionEntity::GetStringCount() const
{
	BasePrivate * my_impl = (BasePrivate *)impl_;

	return get_strings_and_text_attributes_count(my_impl->m_segment_key, "strings");
}

void PMI::DimensionEntity::GetStrings(unsigned int & out_count, PMI::String * out_strings, PMI::TextAttributes * out_text_attributes) const
{
	BasePrivate * my_impl = (BasePrivate *)impl_;

	out_count = GetStringCount();

	if (out_count == 0 || (out_strings == 0 && out_text_attributes == 0))
		return;

	get_strings_and_text_attributes(my_impl->m_segment_key, "strings", out_strings, out_text_attributes);
}

void PMI::DimensionEntity::SetStrings(unsigned int in_count, PMI::String const * in_strings, PMI::TextAttributes const * in_text_attributes)
{
	BasePrivate * my_impl = (BasePrivate *)impl_;

	set_strings_and_text_attributes(my_impl->m_segment_key, "strings", in_count, in_strings, in_text_attributes, this->IsDisplayParallelToScreen());
}

// ==============================================

PMI::FeatureControlFrameEntity::FeatureControlFrameEntity(HC_KEY segment_key) : PMI::Entity(segment_key)
{
	if (segment_key != INVALID_KEY)
	{
		HC_Open_Segment_By_Key(segment_key);
		{
			HC_Set_User_Options("pmi_type = fcf");
		}
		HC_Close_Segment();
	}
}

PMI::FeatureControlFrameEntity::FeatureControlFrameEntity(PMI::FeatureControlFrameEntity const & that)
{
	Set(that);
}

PMI::FeatureControlFrameEntity::~FeatureControlFrameEntity() {}
 
void PMI::FeatureControlFrameEntity::Set(PMI::FeatureControlFrameEntity const & that)
{
	copy_impls(impl_, that.impl_);
}

PMI::FeatureControlFrameEntity const & PMI::FeatureControlFrameEntity::operator=(PMI::FeatureControlFrameEntity const & that)
{
	Set(that);
	return *this;
}

void PMI::FeatureControlFrameEntity::GetToleranceType(PMI::Tolerance::Type & out_tolerance_type, PMI::TextAttributes & out_text_attributes) const
{
	BasePrivate * my_impl = (BasePrivate *)impl_;

	out_tolerance_type = PMI::Tolerance::Unknown;
	out_text_attributes = PMI::TextAttributes();

	unsigned int count = get_strings_and_text_attributes_count(my_impl->m_segment_key, "tolerance symbol");

	if (count == 0)
		return;

	assert(count == 1);

	PMI::String strings[1];
	PMI::TextAttributes text_attributes[1];

	get_strings_and_text_attributes(my_impl->m_segment_key, "tolerance symbol", strings, text_attributes);

	wchar_t tolerance_symbol[2];
	wcscpy(tolerance_symbol, strings[0].WStr());

	switch (tolerance_symbol[0])
	{
		case 0x2220:	out_tolerance_type = PMI::Tolerance::Angularity;		break;
		case 0x25EF:	out_tolerance_type = PMI::Tolerance::Circularity;		break;
		case 0x2197:	out_tolerance_type = PMI::Tolerance::CircularRunout;	break;
		case 0x25CE:	out_tolerance_type = PMI::Tolerance::Concentricity;		break;
		case 0x232D:	out_tolerance_type = PMI::Tolerance::Cylindricity;		break;
		case 0x23E5:	out_tolerance_type = PMI::Tolerance::Flatness;			break;
		case 0x2312:	out_tolerance_type = PMI::Tolerance::LineProfile;		break;
		case 0x2225:	out_tolerance_type = PMI::Tolerance::Parallelism;		break;
		case 0x27C2:	out_tolerance_type = PMI::Tolerance::Perpendicularity;	break;
		case 0x2316:	out_tolerance_type = PMI::Tolerance::Position;			break;
		case 0x23E4:	out_tolerance_type = PMI::Tolerance::Straightness;		break;
		case 0x2313:	out_tolerance_type = PMI::Tolerance::SurfaceProfile;	break;
		case 0x232F:	out_tolerance_type = PMI::Tolerance::Symmetry;			break;
		case 0x2330:	out_tolerance_type = PMI::Tolerance::TotalRunout;		break;
		default:		assert(0);
	}

	out_text_attributes = text_attributes[0];
}

void PMI::FeatureControlFrameEntity::SetToleranceType(PMI::Tolerance::Type const in_tolerance_type, PMI::TextAttributes const & in_text_attributes)
{
	BasePrivate * my_impl = (BasePrivate *)impl_;

	if (my_impl->m_segment_key == INVALID_KEY)
		return;

	wchar_t tolerance_symbol[2] = { L"" };
	PMI::String strings[1];
	PMI::TextAttributes text_attributes[1];
	unsigned int count = 0;

	if (in_tolerance_type != PMI::Tolerance::Unknown)
	{
		count = 1;

		text_attributes[0] = in_text_attributes;

		switch (in_tolerance_type)
		{
			case PMI::Tolerance::Angularity:		tolerance_symbol[0] = 0x2220;	break;
			case PMI::Tolerance::Circularity:		tolerance_symbol[0] = 0x25EF;	break;
			case PMI::Tolerance::CircularRunout:	tolerance_symbol[0] = 0x2197;	break;
			case PMI::Tolerance::Concentricity:		tolerance_symbol[0] = 0x25CE;	break;
			case PMI::Tolerance::Cylindricity:		tolerance_symbol[0] = 0x232D;	break;
			case PMI::Tolerance::Flatness:			tolerance_symbol[0] = 0x23E5;	break;
			case PMI::Tolerance::LineProfile:		tolerance_symbol[0] = 0x2312;	break;
			case PMI::Tolerance::Parallelism:		tolerance_symbol[0] = 0x2225;	break;
			case PMI::Tolerance::Perpendicularity:	tolerance_symbol[0] = 0x27C2;	break;
			case PMI::Tolerance::Position:			tolerance_symbol[0] = 0x2316;	break;
			case PMI::Tolerance::Straightness:		tolerance_symbol[0] = 0x23E4;	break;
			case PMI::Tolerance::SurfaceProfile:	tolerance_symbol[0] = 0x2313;	break;
			case PMI::Tolerance::Symmetry:			tolerance_symbol[0] = 0x232F;	break;
			case PMI::Tolerance::TotalRunout:		tolerance_symbol[0] = 0x2330;	break;
			default:								assert(0);
		}

		strings[0] = PMI::String(tolerance_symbol);
	}

	set_strings_and_text_attributes(my_impl->m_segment_key, "tolerance symbol", count, strings, text_attributes);
}

void PMI::FeatureControlFrameEntity::GetDiameterModifierType(PMI::DiameterModifier::Type & out_diameter_modifier_type, 
		PMI::TextAttributes & out_text_attributes) const
{
	BasePrivate * my_impl = (BasePrivate *)impl_;

	out_diameter_modifier_type = PMI::DiameterModifier::None;
	out_text_attributes = PMI::TextAttributes();

	unsigned int count = get_strings_and_text_attributes_count(my_impl->m_segment_key, "diameter modifier");

	if (count == 0)
		return;

	assert(count == 1);

	PMI::String strings[1];
	PMI::TextAttributes text_attributes[1];

	get_strings_and_text_attributes(my_impl->m_segment_key, "diameter modifier", strings, text_attributes);

	wchar_t diameter_modifier_symbol[3] = { L"" };
	wcscpy(diameter_modifier_symbol, strings[0].WStr());

	switch (diameter_modifier_symbol[0])
	{
		case 0x2300:
			out_diameter_modifier_type = PMI::DiameterModifier::Diameter;
			break;
		case 0x0053:
			assert(diameter_modifier_symbol[1] == 0x2300);
			out_diameter_modifier_type = PMI::DiameterModifier::SphericalDiameter;
			break;
		default:
			assert(0);
	}

	out_text_attributes = text_attributes[0];
}

void PMI::FeatureControlFrameEntity::SetDiameterModifierType(PMI::DiameterModifier::Type const in_diameter_modifier_type, 
		PMI::TextAttributes const & in_text_attributes)
{
	BasePrivate * my_impl = (BasePrivate *)impl_;

	if (my_impl->m_segment_key == INVALID_KEY)
		return;

	wchar_t diameter_modifier_symbol[3] = { L"" };
	PMI::String strings[1];
	PMI::TextAttributes text_attributes[1];
	unsigned int count = 0;

	if (in_diameter_modifier_type != PMI::DiameterModifier::None)
	{
		count = 1;

		text_attributes[0] = in_text_attributes;

		switch (in_diameter_modifier_type)
		{
			case PMI::DiameterModifier::Diameter:
				diameter_modifier_symbol[0] = 0x2300;
				break;
			case PMI::DiameterModifier::SphericalDiameter:
				diameter_modifier_symbol[0] = 0x0053;	// S
				diameter_modifier_symbol[1] = 0x2300;
				break;
			default:
				assert(0);
		}

		strings[0] = PMI::String(diameter_modifier_symbol);
	}

	set_strings_and_text_attributes(my_impl->m_segment_key, "diameter modifier", count, strings, text_attributes);
}

void PMI::FeatureControlFrameEntity::GetZoneToleranceMagnitude(PMI::String & out_zone_tolerance_magnitude, 
		PMI::TextAttributes & out_text_attributes) const
{
	BasePrivate * my_impl = (BasePrivate *)impl_;

	out_zone_tolerance_magnitude = PMI::String();
	out_text_attributes = PMI::TextAttributes();

	unsigned int count = get_strings_and_text_attributes_count(my_impl->m_segment_key, "zone tolerance");

	if (count == 0)
		return;

	assert(count == 1);

	PMI::String strings[1];
	PMI::TextAttributes text_attributes[1];

	get_strings_and_text_attributes(my_impl->m_segment_key, "zone tolerance", strings, text_attributes);

	out_zone_tolerance_magnitude = strings[0];
	out_text_attributes = text_attributes[0];
}

void PMI::FeatureControlFrameEntity::SetZoneToleranceMagnitude(PMI::String const & in_zone_tolerance_magnitude, 
		PMI::TextAttributes const & in_text_attributes)
{
	BasePrivate * my_impl = (BasePrivate *)impl_;

	if (my_impl->m_segment_key == INVALID_KEY)
		return;

	PMI::String strings[1];
	PMI::TextAttributes text_attributes[1];

	strings[0] = in_zone_tolerance_magnitude;
	text_attributes[0] = in_text_attributes;

	set_strings_and_text_attributes(my_impl->m_segment_key, "zone tolerance", 1, strings, text_attributes);
}

void get_zone_modifier_type(PrivateImpl * in_impl, unsigned int in_count, PMI::ZoneModifier::Type & out_zone_modifier_type,
		PMI::TextAttributes & out_text_attributes)
{
	BasePrivate * my_impl = (BasePrivate *)in_impl;

	out_zone_modifier_type = PMI::ZoneModifier::None;
	out_text_attributes = PMI::TextAttributes();

	if (my_impl->m_segment_key == INVALID_KEY)
		return;

	// have to check for metadata option
	HC_Open_Segment_By_Key(my_impl->m_segment_key);
	{
		HC_Open_Segment("text");
		{
			HC_Open_Segment(H_FORMAT_TEXT("zone modifier %u", in_count));
			{
				if (HC_Show_Existence("user options = zone_modifier"))
				{
					char modifier[1024] = { "" };
					HC_Show_One_User_Option("zone_modifier", modifier);

					assert(streq(modifier, "regardless_of_feature_size"));

					out_zone_modifier_type = PMI::ZoneModifier::RegardlessOfFeatureSize;
				}
			}
			HC_Close_Segment();
		}
		HC_Close_Segment();
	}
	HC_Close_Segment();

	// exit if we found the metadata
	if (out_zone_modifier_type != PMI::ZoneModifier::None)
		return;

	unsigned int count = get_strings_and_text_attributes_count(my_impl->m_segment_key, H_FORMAT_TEXT("zone modifier %u", in_count));

	if (count == 0)
		return;

	assert(count == 1);

	PMI::String strings[1];
	PMI::TextAttributes text_attributes[1];

	get_strings_and_text_attributes(my_impl->m_segment_key, H_FORMAT_TEXT("zone modifier %u", in_count), strings, text_attributes);

	wchar_t zone_modifier_symbol[2] = { L"" };
	wcscpy(zone_modifier_symbol, strings[0].WStr());

	switch (zone_modifier_symbol[0])
	{
	case 0x24C2:
		out_zone_modifier_type = PMI::ZoneModifier::MaximumMaterialCondition;
		break;
	case 0x24C1:
		out_zone_modifier_type = PMI::ZoneModifier::LeastMaterialCondition;
		break;
	case 0x24C9:
		out_zone_modifier_type = PMI::ZoneModifier::TangentPlane;
		break;
	case 0x24C5:
		out_zone_modifier_type = PMI::ZoneModifier::ProjectedToleranceZone;
		break;
	case 0x24BB:
		out_zone_modifier_type = PMI::ZoneModifier::FreeState;
		break;
	case 0xE400:
		out_zone_modifier_type = PMI::ZoneModifier::StatisticalTolerance;
		break;
	default:
		assert(0);
	}

	out_text_attributes = text_attributes[0];
}

void set_zone_modifier_type(PrivateImpl * in_impl, unsigned int in_count, PMI::ZoneModifier::Type const in_zone_modifier_type,
		PMI::TextAttributes const & in_text_attributes)
{
	BasePrivate * my_impl = (BasePrivate *)in_impl;

	if (my_impl->m_segment_key == INVALID_KEY)
		return;

	// this is a special case since there is no symbol to insert, but metadata to be stored
	if (in_zone_modifier_type == PMI::ZoneModifier::RegardlessOfFeatureSize)
	{
		HC_Open_Segment_By_Key(my_impl->m_segment_key);
		{
			HC_Open_Segment("text");
			{
				HC_Open_Segment(H_FORMAT_TEXT("zone modifier %u", in_count));
				{
					HC_Flush_Contents(".", "everything");

					HC_Set_User_Options("zone_modifier = regardless_of_feature_size");
				}
				HC_Close_Segment();
			}
			HC_Close_Segment();
		}
		HC_Close_Segment();

		return;
	}

	wchar_t zone_modifier_symbol[2] = { L"" };
	PMI::String strings[1];
	PMI::TextAttributes text_attributes[1];
	unsigned int count = 0;

	if (in_zone_modifier_type != PMI::ZoneModifier::None)
	{
		count = 1;

		text_attributes[0] = in_text_attributes;

		switch (in_zone_modifier_type)
		{
		case PMI::ZoneModifier::MaximumMaterialCondition:
			zone_modifier_symbol[0] = 0x24C2;
			break;
		case PMI::ZoneModifier::LeastMaterialCondition:
			zone_modifier_symbol[0] = 0x24C1;
			break;
		case PMI::ZoneModifier::TangentPlane:
			zone_modifier_symbol[0] = 0x24C9;
			break;
		case PMI::ZoneModifier::ProjectedToleranceZone:
			zone_modifier_symbol[0] = 0x24C5;
			break;
		case PMI::ZoneModifier::FreeState:
			zone_modifier_symbol[0] = 0x24BB;
			break;
		case PMI::ZoneModifier::StatisticalTolerance:
			zone_modifier_symbol[0] = 0xE400;
			break;
		default:
			assert(0);
		}

		strings[0] = PMI::String(zone_modifier_symbol);
	}

	set_strings_and_text_attributes(my_impl->m_segment_key, H_FORMAT_TEXT("zone modifier %u", in_count), count, strings, text_attributes);
}

void PMI::FeatureControlFrameEntity::GetFirstZoneModifierType(PMI::ZoneModifier::Type & out_zone_modifier_type, 
		PMI::TextAttributes & out_text_attributes) const
{
	get_zone_modifier_type(impl_, 1, out_zone_modifier_type, out_text_attributes);
}

void PMI::FeatureControlFrameEntity::SetFirstZoneModifierType(PMI::ZoneModifier::Type const in_zone_modifier_type, 
		PMI::TextAttributes const & in_text_attributes)
{
	set_zone_modifier_type(impl_, 1, in_zone_modifier_type, in_text_attributes);
}

void PMI::FeatureControlFrameEntity::GetSecondZoneModifierType(PMI::ZoneModifier::Type & out_zone_modifier_type, 
		PMI::TextAttributes & out_text_attributes) const
{
	get_zone_modifier_type(impl_, 2, out_zone_modifier_type, out_text_attributes);
}

void PMI::FeatureControlFrameEntity::SetSecondZoneModifierType(PMI::ZoneModifier::Type const in_zone_modifier_type, 
		PMI::TextAttributes const & in_text_attributes)
{
	set_zone_modifier_type(impl_, 2, in_zone_modifier_type, in_text_attributes);
}

void PMI::FeatureControlFrameEntity::GetProjectedMagnitude(PMI::String & out_projected_magnitude, PMI::TextAttributes & out_text_attributes) const
{
	BasePrivate * my_impl = (BasePrivate *)impl_;

	out_projected_magnitude = PMI::String();
	out_text_attributes = PMI::TextAttributes();

	unsigned int count = get_strings_and_text_attributes_count(my_impl->m_segment_key, "projected magnitude");

	if (count == 0)
		return;

	assert(count == 1);

	PMI::String strings[1];
	PMI::TextAttributes text_attributes[1];

	get_strings_and_text_attributes(my_impl->m_segment_key, "projected magnitude", strings, text_attributes);

	out_projected_magnitude = strings[0];
	out_text_attributes = text_attributes[0];
}

void PMI::FeatureControlFrameEntity::SetProjectedMagnitude(PMI::String const & in_projected_magnitude, PMI::TextAttributes const & in_text_attributes)
{
	BasePrivate * my_impl = (BasePrivate *)impl_;

	if (my_impl->m_segment_key == INVALID_KEY)
		return;

	PMI::String strings[1];
	PMI::TextAttributes text_attributes[1];

	strings[0] = in_projected_magnitude;
	text_attributes[0] = in_text_attributes;

	set_strings_and_text_attributes(my_impl->m_segment_key, "projected magnitude", 1, strings, text_attributes);
}

unsigned int PMI::FeatureControlFrameEntity::GetDatumReferenceCount() const
{
	BasePrivate * my_impl = (BasePrivate *)impl_;

	if (my_impl->m_segment_key == INVALID_KEY)
		return 0;

	unsigned int count = 0;
	char type[64];
	HC_KEY key;
	char seg_name[1024];

	HC_Open_Segment_By_Key(my_impl->m_segment_key);
	{
		HC_Open_Segment("text");
		{
			HC_Begin_Contents_Search(".", "segments");
			{
				while (HC_Find_Contents(type, &key))
				{
					HC_Show_Segment(key, seg_name);

					if (strstr(seg_name, "datum reference"))
						count++;
				}
			}
			HC_End_Contents_Search();
		}
		HC_Close_Segment();
	}
	HC_Close_Segment();

	return count;
}

void PMI::FeatureControlFrameEntity::GetDatumReferences(unsigned int & out_count, PMI::String * out_labels, 
		PMI::TextAttributes * out_label_attributes, PMI::DatumModifier::Type * out_modifiers, PMI::TextAttributes * out_modifier_attributes)
{
	BasePrivate * my_impl = (BasePrivate *)impl_;

	out_count = GetDatumReferenceCount();

	if (out_count == 0 || (out_labels == 0 && out_label_attributes == 0 && out_modifiers == 0 && out_modifier_attributes == 0))
		return;

	for (unsigned int i = 0; i < out_count; i++)
	{
		unsigned int count = 0;
		PMI::String strings[2];
		PMI::TextAttributes text_attributes[2];

		count = get_strings_and_text_attributes_count(my_impl->m_segment_key, H_FORMAT_TEXT("datum reference %u", i + 1));

		assert(count == 1 || count == 2);

		get_strings_and_text_attributes(my_impl->m_segment_key, H_FORMAT_TEXT("datum reference %u", i + 1), strings, text_attributes);

		if (count == 1)
		{
			out_labels[i] = strings[0];
			out_label_attributes[i] = text_attributes[0];
			out_modifiers[i] = PMI::DatumModifier::None;
			out_modifier_attributes[i] = PMI::TextAttributes();

			HC_Open_Segment_By_Key(my_impl->m_segment_key);
			{
				HC_Open_Segment(H_FORMAT_TEXT("text/datum reference %u", i + 1));
				{
					if (HC_Show_Existence("user options = datum_modifier"))
					{
						char modifier[1024] = { "" };
						HC_Show_One_User_Option("datum_modifier", modifier);
		
						assert(streq(modifier, "regardless_of_feature_size"));

						out_modifiers[i] = PMI::DatumModifier::RegardlessOfFeatureSize;
					}
				}
				HC_Close_Segment();
			}
			HC_Close_Segment();
		}
		else if (count == 2)
		{
			size_t j = 0;
			size_t k = 1;

			wchar_t token[1024] = { L"" };
			wcscpy(token, strings[0].WStr());

			if (token[0] == 0x24C1 || token[0] == 0x24C2)
			{
				j = 1;
				k = 0;
			}
			else
			{
				wcscpy(token, strings[1].WStr());
			}

			PMI::DatumModifier::Type datum_modifier_type = PMI::DatumModifier::None;

			switch (token[0])
			{
				case 0x24C1:
					datum_modifier_type = PMI::DatumModifier::LeastMaterialCondition;
					break;
				case 0x24C2:
					datum_modifier_type = PMI::DatumModifier::MaximumMaterialCondition;
					break;
				default:
					assert(0);
			}

			out_labels[i] = strings[j];
			out_label_attributes[i] = text_attributes[j];
			out_modifiers[i] = datum_modifier_type;
			out_modifier_attributes[i] = text_attributes[k];
		}
		else
		{
			assert(0);
		}
	}
	
}

void PMI::FeatureControlFrameEntity::SetDatumReferences(unsigned int in_count, PMI::String const * in_labels, 
		PMI::TextAttributes const * in_label_attributes, PMI::DatumModifier::Type const * in_modifiers, 
		PMI::TextAttributes const * in_modifier_attributes)
{
	BasePrivate * my_impl = (BasePrivate *)impl_;

	if (my_impl->m_segment_key == INVALID_KEY)
		return;

	char type[64];
	HC_KEY key;
	char seg_name[1024];

	HC_Open_Segment_By_Key(my_impl->m_segment_key);
	{
		HC_Open_Segment("text");
		{
			HC_Begin_Contents_Search(".", "segments");
			{
				while (HC_Find_Contents(type, &key))
				{
					HC_Show_Segment(key, seg_name);

					if (strstr(seg_name, "datum reference"))
						HC_Delete_By_Key(key);
				}
			}
			HC_End_Contents_Search();
		}
		HC_Close_Segment();
	}
	HC_Close_Segment();

	for (unsigned int i = 0; i < in_count; i++)
	{
		unsigned int count = 1;
		PMI::String strings[2];
		PMI::TextAttributes text_attributes[2];

		strings[0] = in_labels[i];
		text_attributes[0] = in_label_attributes[i];

		if (in_modifiers[i] != PMI::DatumModifier::None 
			&& in_modifiers[i] != PMI::DatumModifier::RegardlessOfFeatureSize)
		{
			count = 2;

			wchar_t datum_modifier_symbol[2] = { L"" };

			switch (in_modifiers[i])
			{
				case PMI::DatumModifier::MaximumMaterialCondition:
					datum_modifier_symbol[0] = 0x24C2;
					break;
				case PMI::DatumModifier::LeastMaterialCondition:
					datum_modifier_symbol[0] = 0x24C1;
					break;
				default:
					assert(0);
			}

			strings[1] = PMI::String(datum_modifier_symbol);
			text_attributes[1] = in_modifier_attributes[i];
		}

		set_strings_and_text_attributes(my_impl->m_segment_key, H_FORMAT_TEXT("datum reference %u", i + 1), count, strings, text_attributes);

		if (in_modifiers[i] == PMI::DatumModifier::RegardlessOfFeatureSize)
		{
			HC_Open_Segment_By_Key(my_impl->m_segment_key);
			{
				HC_Open_Segment(H_FORMAT_TEXT("text/datum reference %u", i + 1));
				{
					HC_Set_User_Options("datum_modifier = regardless_of_feature_size");
				}
				HC_Close_Segment();
			}
			HC_Close_Segment();
		}
	}
}

// ==============================================

PMI::GenericEntity::GenericEntity(HC_KEY segment_key) : PMI::Entity(segment_key)
{
	if (segment_key != INVALID_KEY)
	{
		HC_Open_Segment_By_Key(segment_key);
		{
			HC_Set_User_Options("pmi_type = generic");
		}
		HC_Close_Segment();
	}
}

PMI::GenericEntity::GenericEntity(PMI::GenericEntity const & that)
{
	Set(that);
}

PMI::GenericEntity::~GenericEntity() {}

void PMI::GenericEntity::Set(PMI::GenericEntity const & that)
{
	copy_impls(impl_, that.impl_);
}

PMI::GenericEntity const & PMI::GenericEntity::operator=(PMI::GenericEntity const & that)
{
	Set(that);
	return *this;
}

unsigned int PMI::GenericEntity::GetStringCount() const
{
	BasePrivate * my_impl = (BasePrivate *)impl_;

	return get_strings_and_text_attributes_count(my_impl->m_segment_key, "strings");
}

void PMI::GenericEntity::GetStrings(unsigned int & out_count, PMI::String * out_strings, PMI::TextAttributes * out_text_attributes) const
{
	BasePrivate * my_impl = (BasePrivate *)impl_;

	out_count = GetStringCount();

	if (out_count == 0 || (out_strings == 0 && out_text_attributes == 0))
		return;

	get_strings_and_text_attributes(my_impl->m_segment_key, "strings", out_strings, out_text_attributes);
}

void PMI::GenericEntity::SetStrings(unsigned int in_count, PMI::String const * in_strings, PMI::TextAttributes const * in_text_attributes)
{
	BasePrivate * my_impl = (BasePrivate *)impl_;

	set_strings_and_text_attributes(my_impl->m_segment_key, "strings", in_count, in_strings, in_text_attributes);
}

void PMI::GenericEntity::SetDisplayParallelToScreen(bool const in_parallel)
{
	PMI::Entity::SetDisplayParallelToScreen(in_parallel);
}

bool PMI::GenericEntity::IsDisplayParallelToScreen() const
{
	return PMI::Entity::IsDisplayParallelToScreen();
}

// ==============================================

PMI::NoteEntity::NoteEntity(HC_KEY segment_key) : PMI::Entity(segment_key)
{
	if (segment_key != INVALID_KEY)
	{
		HC_Open_Segment_By_Key(segment_key);
		{
			HC_Set_User_Options("pmi_type = note");
		}
		HC_Close_Segment();
	}
}

PMI::NoteEntity::NoteEntity(PMI::NoteEntity const & that)
{
	Set(that);
}

PMI::NoteEntity::~NoteEntity() {}

void PMI::NoteEntity::Set(PMI::NoteEntity const & that)
{
	copy_impls(impl_, that.impl_);
}

PMI::NoteEntity const & PMI::NoteEntity::operator=(PMI::NoteEntity const & that)
{
	Set(that);
	return *this;
}

unsigned int PMI::NoteEntity::GetStringCount() const
{
	BasePrivate * my_impl = (BasePrivate *)impl_;

	return get_strings_and_text_attributes_count(my_impl->m_segment_key, "strings");
}

void PMI::NoteEntity::GetStrings(unsigned int & out_count, PMI::String * out_strings, PMI::TextAttributes * out_text_attributes) const
{
	BasePrivate * my_impl = (BasePrivate *)impl_;

	out_count = GetStringCount();

	if (out_count == 0 || (out_strings == 0 && out_text_attributes == 0))
		return;

	get_strings_and_text_attributes(my_impl->m_segment_key, "strings", out_strings, out_text_attributes);
}

void PMI::NoteEntity::SetStrings(unsigned int in_count, PMI::String const * in_strings, PMI::TextAttributes const * in_text_attributes)
{
	BasePrivate * my_impl = (BasePrivate *)impl_;

	set_strings_and_text_attributes(my_impl->m_segment_key, "strings", in_count, in_strings, in_text_attributes, this->IsDisplayParallelToScreen());
}

// ==============================================

PMI::RoughnessEntity::RoughnessEntity(HC_KEY segment_key) : PMI::Entity(segment_key)
{
	if (segment_key != INVALID_KEY)
	{
		HC_Open_Segment_By_Key(segment_key);
		{
			HC_Set_User_Options("pmi_type = roughness");
		}
		HC_Close_Segment();
	}
}

PMI::RoughnessEntity::RoughnessEntity(PMI::RoughnessEntity const & that)
{
	Set(that);
}

PMI::RoughnessEntity::~RoughnessEntity() {}

void PMI::RoughnessEntity::Set(PMI::RoughnessEntity const & that)
{
	copy_impls(impl_, that.impl_);
}

PMI::RoughnessEntity const & PMI::RoughnessEntity::operator=(PMI::RoughnessEntity const & that)
{
	Set(that);
	return *this;
}

PMI::Roughness::Obtention::Type PMI::RoughnessEntity::GetObtentionType() const
{
	BasePrivate * my_impl = (BasePrivate *)impl_;
	PMI::Roughness::Obtention::Type obtention_type = PMI::Roughness::Obtention::Unknown;

	if (my_impl->m_segment_key == INVALID_KEY)
		return obtention_type;

	HC_Open_Segment_By_Key(my_impl->m_segment_key);
	{
		if (HC_Show_Existence("user options = obtention_type"))
		{
			char type[64];

			HC_Show_One_User_Option("obtention_type", type);

			if (streq(type, "not_defined"))
				obtention_type = PMI::Roughness::Obtention::NotDefined;
			else if (streq(type, "machining_mandatory"))
				obtention_type = PMI::Roughness::Obtention::MachiningMandatory;
			else if (streq(type, "machining_forbidden"))
				obtention_type = PMI::Roughness::Obtention::MachiningForbidden;
			else
				assert(0);
		}
	}
	HC_Close_Segment();

	return obtention_type;
}

void PMI::RoughnessEntity::SetObtentionType(PMI::Roughness::Obtention::Type const in_obtention_type)
{
	BasePrivate * my_impl = (BasePrivate *)impl_;

	if (my_impl->m_segment_key == INVALID_KEY)
		return;

	HC_Open_Segment_By_Key(my_impl->m_segment_key);
	{
		switch (in_obtention_type)
		{
			case PMI::Roughness::Obtention::NotDefined:
				HC_Set_User_Options("obtention_type = not_defined");
				break;
			case PMI::Roughness::Obtention::MachiningMandatory:
				HC_Set_User_Options("obtention_type = machining_mandatory");
				break;
			case PMI::Roughness::Obtention::MachiningForbidden:
				HC_Set_User_Options("obtention_type = machining_forbidden");
				break;
			case PMI::Roughness::Obtention::Unknown:
				HC_UnSet_One_User_Option("obtention_type");
				break;
			default:
				assert(0);
		}
	}
	HC_Close_Segment();
}

PMI::Roughness::Applicability::Type PMI::RoughnessEntity::GetApplicabilityType() const
{
	BasePrivate * my_impl = (BasePrivate *)impl_;
	PMI::Roughness::Applicability::Type applicability_type = PMI::Roughness::Applicability::Unknown;

	if (my_impl->m_segment_key == INVALID_KEY)
		return applicability_type;

	HC_Open_Segment_By_Key(my_impl->m_segment_key);
	{
		if (HC_Show_Existence("user options = applicability_type"))
		{
			char type[64];

			HC_Show_One_User_Option("applicability_type", type);

			if (streq(type, "specified_surface"))
				applicability_type = PMI::Roughness::Applicability::SpecifiedSurface;
			else if (streq(type, "surrounding_surfaces"))
				applicability_type = PMI::Roughness::Applicability::SurroundingSurfaces;
			else if (streq(type, "basic_surface"))
				applicability_type = PMI::Roughness::Applicability::BasicSurface;
			else if (streq(type, "all_surfaces"))
				applicability_type = PMI::Roughness::Applicability::AllSurfaces;
			else
				assert(0);
		}
	}
	HC_Close_Segment();

	return applicability_type;
}

void PMI::RoughnessEntity::SetApplicabilityType(PMI::Roughness::Applicability::Type const in_applicability_type)
{
	BasePrivate * my_impl = (BasePrivate *)impl_;

	if (my_impl->m_segment_key == INVALID_KEY)
		return;

	HC_Open_Segment_By_Key(my_impl->m_segment_key);
	{
		switch (in_applicability_type)
		{
			case PMI::Roughness::Applicability::SpecifiedSurface:
				HC_Set_User_Options("applicability_type = specified_surface");
				break;
			case PMI::Roughness::Applicability::SurroundingSurfaces:
				HC_Set_User_Options("applicability_type = surrounding_surfaces");
				break;
			case PMI::Roughness::Applicability::BasicSurface:
				HC_Set_User_Options("applicability_type = basic_surface");
				break;
			case PMI::Roughness::Applicability::AllSurfaces:
				HC_Set_User_Options("applicability_type = all_surfaces");
				break;
			case PMI::Roughness::Applicability::Unknown:
				HC_UnSet_One_User_Option("applicability_type");
				break;
			default:
				assert(0);
		}
	}
	HC_Close_Segment();
}

PMI::Roughness::Mode::Type PMI::RoughnessEntity::GetModeType() const
{
	BasePrivate * my_impl = (BasePrivate *)impl_;
	PMI::Roughness::Mode::Type mode_type = PMI::Roughness::Mode::Unknown;

	if (my_impl->m_segment_key == INVALID_KEY)
		return mode_type;

	HC_Open_Segment_By_Key(my_impl->m_segment_key);
	{
		if (HC_Show_Existence("user options = mode_type"))
		{
			char type[64];

			HC_Show_One_User_Option("mode_type", type);

			if (streq(type, "none"))
				mode_type = PMI::Roughness::Mode::None;
			else if (streq(type, "multidirectional"))
				mode_type = PMI::Roughness::Mode::MultiDirectional;
			else if (streq(type, "circular"))
				mode_type = PMI::Roughness::Mode::Circular;
			else if (streq(type, "radial"))
				mode_type = PMI::Roughness::Mode::Radial;
			else if (streq(type, "particular"))
				mode_type = PMI::Roughness::Mode::Particular;
			else if (streq(type, "with_angles"))
				mode_type = PMI::Roughness::Mode::WithAngles;
			else if (streq(type, "parallel"))
				mode_type = PMI::Roughness::Mode::Parallel;
			else if (streq(type, "perpendicular"))
				mode_type = PMI::Roughness::Mode::Perpendicular;
			else
				assert(0);
		}
	}
	HC_Close_Segment();

	return mode_type;
}

void PMI::RoughnessEntity::SetModeType(PMI::Roughness::Mode::Type const in_mode_type)
{
	BasePrivate * my_impl = (BasePrivate *)impl_;

	if (my_impl->m_segment_key == INVALID_KEY)
		return;

	HC_Open_Segment_By_Key(my_impl->m_segment_key);
	{
		switch (in_mode_type)
		{
			case PMI::Roughness::Mode::None:
				HC_Set_User_Options("mode_type = none");
				break;
			case PMI::Roughness::Mode::MultiDirectional:
				HC_Set_User_Options("mode_type = multidirectional");
				break;
			case PMI::Roughness::Mode::Circular:
				HC_Set_User_Options("mode_type = circular");
				break;
			case PMI::Roughness::Mode::Radial:
				HC_Set_User_Options("mode_type = radial");
				break;
			case PMI::Roughness::Mode::Particular:
				HC_Set_User_Options("mode_type = particular");
				break;
			case PMI::Roughness::Mode::WithAngles:
				HC_Set_User_Options("mode_type = with_angles");
				break;
			case PMI::Roughness::Mode::Parallel:
				HC_Set_User_Options("mode_type = parallel");
				break;
			case PMI::Roughness::Mode::Perpendicular:
				HC_Set_User_Options("mode_type = perpendicular");
				break;
			case PMI::Roughness::Mode::Unknown:
				HC_UnSet_One_User_Option("mode_type");
				break;
			default:
				assert(0);
		}
	}
	HC_Close_Segment();
}

unsigned int PMI::RoughnessEntity::GetFieldCount() const
{
	BasePrivate * my_impl = (BasePrivate *)impl_;

	return get_strings_and_text_attributes_count(my_impl->m_segment_key, "fields");
}

void PMI::RoughnessEntity::GetFields(unsigned int & out_count, PMI::String * out_fields, PMI::TextAttributes * out_text_attributes) const
{
	BasePrivate * my_impl = (BasePrivate *)impl_;

	out_count = GetFieldCount();

	if (out_count == 0 || (out_fields == 0 && out_text_attributes == 0))
		return;

	get_strings_and_text_attributes(my_impl->m_segment_key, "fields", out_fields, out_text_attributes);
}

void PMI::RoughnessEntity::SetFields(unsigned int in_count, PMI::String const * in_fields, PMI::TextAttributes const * in_text_attributes)
{
	BasePrivate * my_impl = (BasePrivate *)impl_;

	set_strings_and_text_attributes(my_impl->m_segment_key, "fields", in_count, in_fields, in_text_attributes);
}

// ==============================================

PMI::ViewEntity::ViewEntity(HC_KEY segment_key)
{
	impl_ = new BasePrivate();

	BasePrivate * my_impl = (BasePrivate *)impl_;
	my_impl->m_segment_key = segment_key;

	if (segment_key != INVALID_KEY)
	{
		HC_Open_Segment_By_Key(segment_key);
		{
			HC_Set_User_Options("pmi_type = view");
		}
		HC_Close_Segment();
	}
}

PMI::ViewEntity::ViewEntity(PMI::ViewEntity const & that)
{
	Set(that);
}

PMI::ViewEntity::~ViewEntity()
{
	delete impl_;
}

void PMI::ViewEntity::Set(PMI::ViewEntity const & that)
{
	copy_impls(impl_, that.impl_);
}

PMI::ViewEntity const & PMI::ViewEntity::operator=(PMI::ViewEntity const & that)
{
	Set(that);
	return *this;
}

void PMI::ViewEntity::GetCamera(PMI::Camera & out_camera) const
{
	BasePrivate * my_impl = (BasePrivate *)impl_;
	out_camera.InvalidateCamera();

	if (my_impl->m_segment_key == INVALID_KEY)
		return;

	HC_Open_Segment_By_Key(my_impl->m_segment_key);
	{
		int const len = -1 * HC_Show_One_User_Data(VIEW_CAMERA, 0, 0);

		if (len > 0)
		{
			assert(len == sizeof(PMI::Camera));
			HC_Show_One_User_Data(VIEW_CAMERA, &out_camera, len);
		}
	}
	HC_Close_Segment();
}

void PMI::ViewEntity::GetCamera(PMI::Point & out_position, PMI::Point & out_target, PMI::Point & out_up_vector, float & out_field_width, 
		float & out_field_height, char out_projection[128])
{
	out_position = PMI::Point(0, 0, 0);
	out_target = PMI::Point(0, 0, 0);
	out_up_vector = PMI::Point(0, 0, 0);
	out_field_width = out_field_height = 0;
	out_projection[0] = '\0';

	PMI::Camera camera;
	GetCamera(camera);

	if (!camera.valid)
		return;

	out_position = camera.position;
	out_target = camera.target;
	out_up_vector = camera.up_vector;
	out_field_width = camera.field_width;
	out_field_height = camera.field_height;
	strncpy(out_projection, camera.projection, 128);
}

void PMI::ViewEntity::GetCamera(float out_position[3], float out_target[3], float out_up_vector[3], float & out_field_width, 
		float & out_field_height, char out_projection[128])
{
	out_position[0] = out_position[1] = out_position[2] = 0;
	out_target[0] = out_target[1] = out_target[2] = 0;
	out_up_vector[0] = out_up_vector[1] = out_up_vector[2] = 0;
	out_field_width = out_field_height = 0;
	out_projection[0] = '\0';

	PMI::Camera camera;
	GetCamera(camera);

	if (!camera.valid)
		return;

	out_position[0] = camera.position.x;
	out_position[1] = camera.position.y;
	out_position[2] = camera.position.z;
	out_target[0] = camera.target.x;
	out_target[1] = camera.target.y;
	out_target[2] = camera.target.z;
	out_up_vector[0] = camera.up_vector.x;
	out_up_vector[1] = camera.up_vector.y;
	out_up_vector[2] = camera.up_vector.z;
	out_field_width = camera.field_width;
	out_field_height = camera.field_height;
	strncpy(out_projection, camera.projection, 128);
}

void PMI::ViewEntity::SetCamera(PMI::Camera const & in_camera)
{
	BasePrivate * my_impl = (BasePrivate *)impl_;

	if (my_impl->m_segment_key == INVALID_KEY)
		return;

	if (!in_camera.valid)
		return;

	HC_Open_Segment_By_Key(my_impl->m_segment_key);
	{
		HC_Set_User_Data(VIEW_CAMERA, &in_camera, sizeof(PMI::Camera));
	}
	HC_Close_Segment();
}

void PMI::ViewEntity::SetCamera(PMI::Point const & in_position, PMI::Point const & in_target, PMI::Point const & in_up_vector, 
		float in_field_width, float in_field_height, char const in_projection[128])
{
	PMI::Camera camera;

	camera.valid = true;
	camera.position = in_position;
	camera.target = in_target;
	camera.up_vector = in_up_vector;
	camera.field_width = in_field_width;
	camera.field_height = in_field_height;
	strncpy(camera.projection, in_projection, 128);

	SetCamera(camera);
}

void PMI::ViewEntity::SetCamera(float const in_position[3], float const in_target[3], float const in_up_vector[3], float in_field_width,
		float in_field_height, char const in_projection[128])
{
	PMI::Camera camera;

	camera.valid = true;
	camera.position = PMI::Point(in_position[0], in_position[1], in_position[2]);
	camera.target = PMI::Point(in_target[0], in_target[1], in_target[2]);
	camera.up_vector = PMI::Point(in_up_vector[0], in_up_vector[1], in_up_vector[2]);
	camera.field_width = in_field_width;
	camera.field_height = in_field_height;
	strncpy(camera.projection, in_projection, 128);

	SetCamera(camera);
}

void PMI::ViewEntity::GetFrame(PMI::Frame & out_frame) const
{
	BasePrivate * my_impl = (BasePrivate *)impl_;

	get_frame(my_impl->m_segment_key, out_frame);
}

void PMI::ViewEntity::SetFrame(PMI::Frame const & in_frame)
{
	BasePrivate * my_impl = (BasePrivate *)impl_;

	set_frame(my_impl->m_segment_key, in_frame);
}

void PMI::ViewEntity::GetName(PMI::String & out_name) const
{
	BasePrivate * my_impl = (BasePrivate *)impl_;
	out_name = PMI::String("");

	if (my_impl->m_segment_key == INVALID_KEY)
		return;

	HC_Open_Segment_By_Key(my_impl->m_segment_key);
	{
		int const len = -1 * HC_Show_One_User_Data(VIEW_NAME, 0, 0);

		if (len > 0)
		{
			H_UTF8 utf8;
			char * buff = new char[len];
			HC_Show_One_User_Data(VIEW_NAME, buff, len);
			utf8.encodedText(buff);
			delete [] buff;
			out_name = PMI::String(utf8);
		}
	}
	HC_Close_Segment();
}

void PMI::ViewEntity::SetName(PMI::String const & in_name)
{
	BasePrivate * my_impl = (BasePrivate *)impl_;

	if (my_impl->m_segment_key == INVALID_KEY)
		return;

	HC_Open_Segment_By_Key(my_impl->m_segment_key);
	{
		H_UTF8 utf8(in_name.WStr());
		HC_Set_User_Data(VIEW_NAME, utf8.encodedText(), (long)utf8.length() + 1);
	}
	HC_Close_Segment();
}

unsigned int PMI::ViewEntity::GetAssociatedEntityCount() const
{
	BasePrivate * my_impl = (BasePrivate *)impl_;

	if (my_impl->m_segment_key == INVALID_KEY)
		return 0;

	int include_count = 0;

	HC_Open_Segment_By_Key(my_impl->m_segment_key);
	{
		HC_Begin_Contents_Search(".", "includes");
		{
			HC_Show_Contents_Count(&include_count);
		}
		HC_End_Contents_Search();
	}
	HC_Close_Segment();

	return static_cast<unsigned int>(include_count);
}

void PMI::ViewEntity::GetAssociatedEntities(unsigned int & out_count, PMI::Entity * out_entities) const
{
	BasePrivate * my_impl = (BasePrivate *)impl_;

	if (my_impl->m_segment_key == INVALID_KEY)
		return;

	out_count = 0;

	char type[64] = { "" };
	HC_KEY key = INVALID_KEY;

	HC_Open_Segment_By_Key(my_impl->m_segment_key);
	{
		HC_Begin_Contents_Search(".", "includes");
		{
			unsigned int i = 0;

			while (HC_Find_Contents(type, &key))
			{
				HC_Open_Segment_By_Key(HC_KShow_Include_Segment(key));
				{
					HC_KEY this_seg = HC_KCreate_Segment(".");
					char pmi_type[64] = { "" };
					assert(HC_Show_Existence("user options = pmi_type"));

					HC_Show_One_User_Option("pmi_type", pmi_type);

					if (streq(pmi_type, "datum"))
						out_entities[i] = PMI::DatumEntity(this_seg);
					else if (streq(pmi_type, "dimension"))
						out_entities[i] = PMI::DimensionEntity(this_seg);
					else if (streq(pmi_type, "fcf"))
						out_entities[i] = PMI::FeatureControlFrameEntity(this_seg);
					else if (streq(pmi_type, "generic"))
						out_entities[i] = PMI::GenericEntity(this_seg);
					else if (streq(pmi_type, "note"))
						out_entities[i] = PMI::NoteEntity(this_seg);
					else if (streq(pmi_type, "roughness"))
						out_entities[i] = PMI::RoughnessEntity(this_seg);
					else
						assert(0);
				}
				HC_Close_Segment();

				i++;
			}

			out_count = i;
		}
		HC_End_Contents_Search();
	}
	HC_Close_Segment();
}

void PMI::ViewEntity::SetAssociatedEntities(unsigned int in_count, PMI::Entity const * in_entities)
{
	BasePrivate * my_impl = (BasePrivate *)impl_;

	if (my_impl->m_segment_key == INVALID_KEY)
		return;

	HC_Open_Segment_By_Key(my_impl->m_segment_key);
	{
		HC_Flush_Contents(".", "includes");

		for (unsigned int i = 0; i < in_count; i++)
		{
			HC_KEY entity_key = in_entities[i].GetSegmentKey();

			if (entity_key != INVALID_KEY)
				HC_Include_Segment_By_Key(entity_key);
		}
	}
	HC_Close_Segment();
}

// ====================== end actual entities =======================

