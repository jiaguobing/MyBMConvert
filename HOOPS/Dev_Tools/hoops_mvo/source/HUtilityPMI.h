// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef _HUTILITYPMI_H
#define _HUTILITYPMI_H

#include "hc.h"

#include "HTools.h"
#include "HGlobals.h"
#include "HUtility.h"

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

namespace PMII
{
	class PrivateImpl;
};

/*! \namespace PMI
 * 	Top-level namespace for all PMI functions
 */
namespace PMI 
{

// ======================= start enums =======================

/*! \enum Type
 * 	PMI types
 */
enum Type
{
	UnknownType,
	DatumType,
	DimensionType,
	FeatureControlFrameType,
	GenericType,
	NoteType,
	RoughnessType
};

/*! \namespace Datum
 * 	Namespace for Datum types
 */
namespace Datum
{
	/*! \enum Type
	 * 	Datum types
	*/
	enum Type
	{
		Unknown,
		Identifier,
		Target
	};
};

/*! \namespace Tolerance
 * 	Namespace for Tolerance types
 */
namespace Tolerance
{
	/*! \enum Type
	 * 	Tolerance types
	 */
	enum Type
	{
		Unknown,
		Angularity,
		Circularity,
		CircularRunout,
		Concentricity,
		Cylindricity,
		Flatness,
		LineProfile,
		Parallelism,
		Perpendicularity,
		Position,
		Straightness,
		SurfaceProfile,
		Symmetry,
		TotalRunout
	};
};

/*! \namespace ZoneModifier
 * 	Namespace for ZoneModifier types
 */
namespace ZoneModifier
{
	/*! \enum Type
	 * 	ZoneModifier types
	 */
	enum Type
	{
		None,
		MaximumMaterialCondition,
		LeastMaterialCondition,
		RegardlessOfFeatureSize,
		TangentPlane,
		ProjectedToleranceZone,
		FreeState,
		StatisticalTolerance
	};
};

/*! \namespace DatumModifier
 * 	Namespace for DatumModifier types
 */
namespace DatumModifier
{
	/*! \enum Type
	 * 	DatumModifier types
	 */
	enum Type
	{
		None,
		MaximumMaterialCondition,
		LeastMaterialCondition,
		RegardlessOfFeatureSize
	};
};

/*! \namespace DiameterModifier
 * 	Namespace for DiameterModifier types
 */
namespace DiameterModifier
{
	/*!	\enum Type
	 * 	DiameterModifier types
	 */
	enum Type
	{
		None,
		Diameter,
		SphericalDiameter
	};
};

/*! \namespace Dimension
 * 	Namespace for Dimension types and subtypes
 */
namespace Dimension
{
	/*! \enum Type
	 * 	Dimension types
	 */
	enum Type
	{
		UnknownType,
		BasicType,
		ReferenceType,
		ToleranceType
	};

	/*! \enum SubType
	 * 	Dimension subtypes
	 */
	enum SubType
	{
		UnknownSubType,
		AngleSubType,
		ChamferSubType,
		DiameterSubType,
		DistanceSubType,
		LengthSubType,
		RadiusSubType,
		SlopeSubType
	};
};

/*! \namespace Roughness
 * 	Namespace for Roughness obtention, applicability and mode types
 */
namespace Roughness
{
	/*! \namespace Obtention
	 * 	Namespace for Obtention types
	 */
	namespace Obtention
	{
		/*! \enum Type
		 * 	Obtention types
		 */
		enum Type
		{
			Unknown,
			NotDefined,
			MachiningMandatory,
			MachiningForbidden
		};
	};

	/*! \namespace Applicability
	 * 	Namespace for Applicability types
	 */
	namespace Applicability
	{
		/*! \enum Type
		 * 	Applicability types
		 */
		enum Type
		{
			Unknown,
			SpecifiedSurface,
			SurroundingSurfaces,
			BasicSurface,
			AllSurfaces
		};
	};

	/*! \namespace Mode
	 * 	Namespace for Mode types
	 */
	namespace Mode
	{
		/*! \enum Type
		 * 	Mode types
		 */
		enum Type
		{
			Unknown,
			None,
			MultiDirectional,
			Circular,
			Radial,
			Particular,
			WithAngles,
			Parallel,
			Perpendicular
		};
	};
};

// ======================= end enums =======================


// ======================= start common classes =======================

class Frame;
class LeaderLine;
class Orientation;
class Polyline;
class TextAttributes;
class Polygon;

typedef HPoint Point;
typedef HPoint RGBColor;
typedef HCamera Camera;


/*! \class Frame
 * 	This class represents a collection of polylines which constitute the frame of an Entity or ViewEntity.
 */
class MVO_API Frame
{
public:
	Frame();
	Frame(Frame const & that);
	~Frame();

	void Set(Frame const & that);
	Frame const & operator=(Frame const & that);

	/*! \return the number of Polylines in this Frame */
	unsigned int GetPolylineCount() const;
	/*! Gets the polylines for this Frame.
	 * 	\param out_count the number of polylines in this Frame. Returned to the user.
	 * 	\param out_polylines the array of polylines for this Frame. Returned to the user.
	 * 		Necessary space should be allocated prior to calling. If pointer is NULL
	 * 		no values will be put into the array.
	 */
	void GetPolylines(unsigned int & out_count, Polyline out_polylines[]) const;
	/*! Sets the polylines for this Frame. These will replace any existing polylines.
	 * 	\param in_count the number of polylines in the array 
	 * 	\param in_polylines the array of polylines for this Frame
	 */
	void SetPolylines(unsigned int in_count, Polyline const in_polylines[]);

private:
	PMII::PrivateImpl * impl_;
};

/*! \class Drawing
 * 	This class represents a collection of polygons, which can be associated to an Entity.
 */
class MVO_API Drawing
{
public:
	Drawing();
	Drawing(Drawing const & that);
	~Drawing();

	void Set(Drawing const & that);
	Drawing const & operator=(Drawing const & that);

	/*! \return the number of Polygons in this Drawing */
	unsigned int GetPolygonCount() const;
	/*! Gets the polygons for this Frame.
	 * 	\param out_count the number of polygons in this Drawing. Returned to the user.
	 * 	\param polygons the array of polygons for this Drawing. Returned to the user.
	 * 		Necessary space should be allocated prior to calling. If pointer is NULL
	 * 		no values will be put into the array.
	 */
	void GetPolygons(unsigned int & out_count, Polygon out_polygons[]) const;
	/*! Sets the polygons for this Drawing. These will replace any existing polygons.
	 * 	\param in_count the number of polygons in the array 
	 * 	\param in_polygons the array of polygons for this Drawing
	 */
	void SetPolygons(unsigned int in_count, Polygon const in_polygons[]);

private:
	PMII::PrivateImpl * impl_;
};


/*! \class Orientation
 * 	This class represents the positioning information necessary to display a piece of text associated with an Entity.
 * 	It is essentially a modelling matrix. The X, Y, Z and Origin components describe the plane in which the text lies.
 * 	The X component is the direction in which the text will be written, the Y component describes the vertical 
 * 	orientation along X, the Z component describes the normal of the plane defined by X and Y, 
 * 	and the origin component specifies the exact plane in which the text should be inserted.
 */
class MVO_API Orientation
{
public:
	Orientation();
	Orientation(Orientation const & that);
	~Orientation();

	void Set(Orientation const & that);
	Orientation const & operator=(Orientation const & that);

	/*! Gets the X component. Returned to the user. */
	void GetXComponent(Point & out_component) const;
	/*! Gets the X component. Returned to the user. */
	void GetXComponent(float & out_x, float & out_y, float & out_z) const;
	/*! Sets the X component. This will replace an existing X component. */
	void SetXComponent(Point const & in_component);
	/*! Sets the X component. This will replace an existing X component. */
	void SetXComponent(float in_x, float in_y, float in_z);

	/*! Gets the Y component. Returned to the user. */
	void GetYComponent(Point & out_component) const;
	/*! Gets the Y component. Returned to the user. */
	void GetYComponent(float & out_x, float & out_y, float & out_z) const;
	/*! Sets the Y component. This will replace an existing Y component. */
	void SetYComponent(Point const & in_component);
	/*! Sets the Y component. This will replace an existing Y component. */
	void SetYComponent(float in_x, float in_y, float in_z);

	/*! Gets the Z component. Returned to the user. */
	void GetZComponent(Point & out_component) const;
	/*! Gets the Z component. Returned to the user. */
	void GetZComponent(float & out_x, float & out_y, float & out_z) const;
	/*! Sets the Z component. This will replace an existing Z component. */
	void SetZComponent(Point const & in_component);
	/*! Sets the Z component. This will replace an existing Z component. */
	void SetZComponent(float in_x, float in_y, float in_z);

	/*! Gets the Origin component. Returned to the user. */
	void GetOriginComponent(Point & out_component) const;
	/*! Gets the Origin component. Returned to the user. */
	void GetOriginComponent(float & out_x, float & out_y, float & out_z) const;
	/*! Sets the Origin component. This will replace an existing Origin component. */
	void SetOriginComponent(Point const & in_component);
	/*! Sets the Origin component. This will replace an existing Origin component. */
	void SetOriginComponent(float in_x, float in_y, float in_z);

	/*! Gets the matrix representing the Orientation. Returned to the user. */
	void GetMatrix(float out_matrix[16]) const;
	/*! Sets the matrix representing the Orientation. This will replace all existing components. */
	void SetMatrix(float const in_matrix[16]);

private:
	PMII::PrivateImpl * impl_;
};


/*! \class Polyline
 * 	This class represents a polyline used for Frames and leader lines.
 */
class MVO_API Polyline
{
public:
	Polyline();
	Polyline(Polyline const & that);
	~Polyline();

	void Set(Polyline const & that);
	Polyline const & operator=(Polyline const & that);

	/*! Gets the color for this Polyline. Returned to the user. */
	void GetRGBColor(RGBColor & out_color) const;
	/*! Gets the color for this Polyline. Returned to the user. */
	void GetRGBColor(float & out_r, float & out_g, float & out_b) const;
	/*! Sets the color for this Polyline. This will replace an existing color. */
	void SetRGBColor(RGBColor const & in_color);
	/*! Sets the color for this Polyline. This will replace an existing color. */
	void SetRGBColor(float const in_r, float const in_g, float const in_b);

	/*! Gets the line pattern for this Polyline. Returned to the user. */
	void GetLinePattern(char out_pattern[1024]) const;
	/*! Sets the line pattern for this Polyline. This will replace an existing line pattern. */
	void SetLinePattern(char const in_pattern[1024]);

	/*! \return the number of points in this polyline */
	unsigned int GetPointCount() const;
	/*! Gets the points for this Polyline.
	 * 	\param out_count the number of points in this Polyline. Returned to the user.
	 * 	\param out_points the array of points for this Polyline. Returned to the user.
	 * 		Necessary space should be allocated prior to calling. If pointer is NULL
	 * 		no values will be put into the array.
	 */
	void GetPoints(unsigned int & out_count, Point out_points[]) const;
	/*! Gets the points for this Polyline.
	 * 	\param out_count the number of points in this Polyline. Returned to the user.
	 * 	\param out_points the array of points for this Polyline. Returned to the user.
	 * 		Necessary space should be allocated prior to calling. If pointer is NULL
	 * 		no values will be put into the array.
	 */
	void GetPoints(unsigned int & out_count, float out_points[]) const;
	/*! Sets the points for this Polyline. These will replace any existing points.
	 * 	\param in_count the number of points in the array 
	 * 	\param in_points the array of points for this Polyline
	 */
	void SetPoints(unsigned int in_count, Point const in_points[]);
	/*! Sets the points for this Polyline. These will replace any existing points.
	 * 	\param in_count the number of points in the array 
	 * 	\param in_points the array of points for this Polyline
	 */
	void SetPoints(unsigned int in_count, float const in_points[]);

private:
	PMII::PrivateImpl * impl_;
};

/*! \class Polygon
 * 	This class represents a polygon used for leader symbols (arrow head).
 */
class MVO_API Polygon
{
public:
	Polygon();
	Polygon(Polygon const & that);
	~Polygon();

	void Set(Polygon const & that);
	Polygon const & operator=(Polygon const & that);

	/*! Gets the color for this Polygon. Returned to the user. */
	void GetRGBColor(RGBColor & out_color) const;
	/*! Gets the color for this Polygon. Returned to the user. */
	void GetRGBColor(float & out_r, float & out_g, float & out_b) const;
	/*! Sets the color for this Polygon. This will replace an existing color. */
	void SetRGBColor(RGBColor const & in_color);
	/*! Sets the color for this Polygon. This will replace an existing color. */
	void SetRGBColor(float const in_r, float const in_g, float const in_b);

	/*! \return the number of points in this Polygon */
	unsigned int GetPointCount() const;
	/*! Gets the points for this Polygon.
	 * 	\param out_count the number of points in this Polygon. Returned to the user.
	 * 	\param out_points the array of points for this Polygon. Returned to the user.
	 * 		Necessary space should be allocated prior to calling. If pointer is NULL
	 * 		no values will be put into the array.
	 */
	void GetPoints(unsigned int & out_count, Point out_points[]) const;
	/*! Gets the points for this Polygon.
	 * 	\param out_count the number of points in this Polygon. Returned to the user.
	 * 	\param out_points the array of points for this Polygon. Returned to the user.
	 * 		Necessary space should be allocated prior to calling. If pointer is NULL
	 * 		no values will be put into the array.
	 */
	void GetPoints(unsigned int & out_count, float out_points[]) const;
	/*! Sets the points for this Polygon. These will replace any existing points.
	 * 	\param in_count the number of points in the array 
	 * 	\param in_points the array of points for this Polygon
	 */
	void SetPoints(unsigned int in_count, Point const in_points[]);
	/*! Sets the points for this Polygon. These will replace any existing points.
	 * 	\param in_count the number of points in the array 
	 * 	\param in_points the array of points for this Polygon
	 */
	void SetPoints(unsigned int in_count, float const in_points[]);

private:
	PMII::PrivateImpl * impl_;
};

/*! \class TextAttributes
 * 	This class represents all the information related to displaying a piece of text associated with an Entity.
 */
class MVO_API TextAttributes
{
public:
	TextAttributes();
	TextAttributes(TextAttributes const & that);
	~TextAttributes();

	void Set(TextAttributes const & that);
	TextAttributes const & operator=(TextAttributes const & that);

	/*! Gets the lower left corner insertion point for the text. Returned to the user. */
	void GetInsertionPoint(Point & out_point) const;
	/*! Gets the lower left corner insertion point for the text. Returned to the user. */
	void GetInsertionPoint(float & out_x, float & out_y, float & out_z) const;
	/*! Sets the lower left corner insertion point for the text. This will replace an existing insertion point. */
	void SetInsertionPoint(Point const & in_point);
	/*! Sets the lower left corner insertion point for the text. This will replace an existing insertion point. */
	void SetInsertionPoint(float in_x, float in_y, float in_z);

	/*! Gets the font size (in object relative units or oru) for the text. Returned to the user. */
	void GetFontSize(float & out_size) const;
	/*! Sets the font size (in object relative units or oru) of the text. This will replace an existing font size. */
	void SetFontSize(float const in_size);

	/*! Gets the font name for the text. Returned to the user. */
	void GetFontName(char * out_font_name) const;
	/*! Sets the font name for the text. This will replace an existing font name. */
	void SetFontName(char const * in_font_name);

	/*! Gets the Orientation for the text. Returned to the user. */
	void GetOrientation(Orientation & out_orientation) const;
	/*! Sets the Orientation for the text. This will replace an existing Orientation. */
	void SetOrientation(Orientation const & in_orientation);

	/*! Gets the color information for the text. Returned to the user. */
	void GetRGBColor(RGBColor & out_color) const;
	/*! Gets the color information for the text. Returned to the user. */
	void GetRGBColor(float & out_r, float & out_g, float & out_b) const;
	/*! Sets the color information for the text. This will replace an existing color. */
	void SetRGBColor(RGBColor const & in_color);
	/*! Sets the color information for the text. This will replace an existing color. */
	void SetRGBColor(float in_r, float in_g, float in_b);

	
	/*! Return the option text bold to screen value. */
	bool IsBold() const;
	/*! Return the option text italic to screen value. */
	bool IsItalic() const;
	/*! Return the option text underlined to screen value. */
	bool IsUnderlined() const;
	/*! Return the option text striked out to screen value. */
	bool IsStrikedThrough() const;
	/*! Return the option text overlined to screen value. */
	bool IsOverlined() const;
	/*! Return the option text streched to screen value. */
	bool IsStreched() const;
	/*! Return true if the original font is a wirefame font. */
	bool IsWired() const;
	/*! Return true if the original font is not proportional (each glyph has the same width). */
	bool IsFixedWidth() const;
	/*! Sets format for the text. This will replace an existing format. */
	void SetFormat(char const & in_format);
	
	/*! Return width scale value. */
	double WidthScale() const;
	/*! Set width scale value. */
	void SetWidthScale(const double width_scale);

private:
	PMII::PrivateImpl * impl_;
};


/*! \class String
 * 	This class is used to specify any string which may be associated with an Entity or ViewEntity.
 * 	It takes standard character and wide character strings as well as making use of the H_UTF and H_WCS utility classes
 * 	to allow specifying stings with the various UTF encodings.
 */
class MVO_API String
{
public:
	String();
	String(char const * cstring);
	String(wchar_t const * wstring);
	String(H_WCS const & wcs);
	String(H_UTF8 const & utf8);
	String(String const & that);
	~String();

	void Set(String const & in_that);
	String const & operator=(String const & in_that);

	/*! \return the standard C-string representation of the String */
	char const * CStr() const;
	/*! \return the wide character representation of the String */
	wchar_t const * WStr() const;
	/*! \return the number of characters in the String */
	size_t  WLength() const;

	bool Equals(String const & in_that) const;
	bool operator==(String const & in_that) const;
	bool operator!=(String const & in_that) const;

private:
	PMII::PrivateImpl * impl_;
};

/*! \class Options
 * 	This class represents all the information related to display options associated with an Entity.
 */
class MVO_API Options
{
public:
	Options();
	Options(Options const & that);
	~Options();

	void Set(Options const & that);
	Options const & operator=(Options const & that);

	/*! Set the option parallel to screen. The pmi is display parallel to screen and is not impacted by any rotation */
	void SetDisplayParallelToScreen(bool const in_parallel  = true);
	/*! Return the option parallel to screen value. */
	bool IsDisplayParallelToScreen() const;

private:
	PMII::PrivateImpl * impl_;
};
// ======================= end common classes =======================


// ====================== start actual entities =======================

class Entity;
class DatumEntity;
class DimensionEntity;
class FeatureControlFrameEntity;
class GenericEntity;
class GeometricToleranceEntity;
class NoteEntity;
class RoughnessEntity;
class ViewEntity;


/*! \class Entity
 * 	This class is the base class for all the various PMI entity types. 
 * 	It contains functions common to all the PMI entities.
 */
class MVO_API Entity
{
public:
	/*! Constructs an Entity object. It requires specifying the segment key where the Entity will 
	 * 	insert associated geometry. */
	Entity(HC_KEY segment_key = INVALID_KEY);
	Entity(Entity const & that);
	virtual ~Entity(); 

	void Set(Entity const & that);
	Entity const & operator=(Entity const & that);

	/*! \return the PMI::Type for this entity. Overridden by subclasses. */
	virtual Type GetType() const { return PMI::UnknownType; };

	/*! \return the segment key where the Entity inserts associated geometry */
	HC_KEY GetSegmentKey() const;

	/*! Gets the Frame associated with this Entity. Returned to the user. */
	void GetFrame(Frame & out_frame) const;
	/*! Sets the Frame associated with this Entity. This will replace an existing Frame. */
	void SetFrame(Frame const & in_frame);

	/*! \return the number of leaders lines for this Entity */
	unsigned int GetLeaderLineCount() const;
	/*! Gets the leader lines for this Entity.
	 * 	\param out_count the number of leader lines for this Entity. Returned to the user.
	 * 	\param out_points the array of leader lines for this Entity. Returned to the user.
	 * 		Necessary space should be allocated prior to calling. If pointer is NULL
	 * 		no values will be put into the array.
	 */
	void GetLeaderLines(unsigned int & out_count, Polyline out_leader_lines[]) const;
	/*! Sets the leader lines for this Entity. These will replace any existing leader lines.
	 * 	\param in_count the number of leader lines in the array 
	 * 	\param in_polylines the array of leader lines for this Entity
	 */
	void SetLeaderLines(unsigned int in_count, Polyline const in_leader_lines[]);

	/*! \return the number of leaders symbols for this Entity */
	unsigned int GetLeaderSymbolCount() const;
	/*! Gets the leader symbols for this Entity.
	 * 	\param out_count the number of leader symbols for this Entity. Returned to the user.
	 * 	\param out_points the array of leader symbols for this Entity. Returned to the user.
	 * 		Necessary space should be allocated prior to calling. If pointer is NULL
	 * 		no values will be put into the array.
	 */
	void GetLeaderSymbols(unsigned int & out_count, Polygon out_leader_symbols[]) const;
	/*! Sets the leader symbols for this Entity. These will replace any existing leader symbols.
	 * 	\param in_count the number of leader symbols in the array 
	 * 	\param in_polygons the array of leader symbols for this Entity
	 */
	void SetLeaderSymbols(unsigned int in_count, Polygon const in_leader_symbols[]);


	/*! Set the option parallel to screen. The pmi is display parallel to screen and is not impacted by any rotation */
	void SetDisplayParallelToScreen(bool const in_parallel = true);
	/*! Get the option parallel to screen value. */
	bool IsDisplayParallelToScreen() const;


	/*! Gets the Drawing associated with this Entity. Returned to the user. */
	void GetDrawing(Drawing & out_drawing) const;
	/*! Sets the Drawing associated with this Entity. This will replace an existing Drawing. */
	void SetDrawing(Drawing const & in_drawing);


protected:
	PMII::PrivateImpl * impl_;
};


/*! \class DatumEntity
 * 	This class represents datum identifiers and datum targets.
 */
class MVO_API DatumEntity : public Entity
{
public:
	/*! Constructs a DatumEntity object. It requires specifying the segment key where the DatumEntity will 
	 * 	insert associated geometry. */
	DatumEntity(HC_KEY segment_key = INVALID_KEY);
	DatumEntity(DatumEntity const & that);
	virtual ~DatumEntity(); 

	void Set(DatumEntity const & that);
	DatumEntity const & operator=(DatumEntity const & that);

	virtual Type GetType() const { return PMI::DatumType; };
	
	/*! \return the datum type of this DatumEntity */
	Datum::Type GetDatumType() const;
	/*! Sets the datum type of this DatumEntity. This will replace an existing datum type. */
	void SetDatumType(Datum::Type const in_type);

	/* \return the number of labels for this DatumEntity */
	unsigned int GetLabelCount() const;
	/*! Gets the labels for this DatumEntity.
	 * 	\param out_count the number of labels for this DatumEntity. Returned to the user.
	 * 	\param out_labels the array of labels for this DatumEntity. Returned to the user.
	 * 		Necessary space should be allocated prior to calling. If pointer is NULL
	 * 		no values will be put into the array.
	 * 	\param out_text_attributes the array of TextAttributes for the labels for this DatumEntity. 
	 * 		Returned to the user. Necessary space should be allocated prior to calling. If pointer is NULL
	 * 		no values will be put into the array.
	 */
	void GetLabels(unsigned int & out_count, String out_labels[], TextAttributes out_text_attributes[]) const;
	/*! Sets the labels for this DatumEntity. These will replace any existing labels.
	 * 	\param in_count the number of labels in the array 
	 * 	\param in_labels the array of labels for this DatumEntity
	 * 	\param in_text_attributes the array of TextAttributes for the labels for this DatumEntity
	 */
	void SetLabels(unsigned int in_count, String const in_labels[], TextAttributes const in_text_attributes[]);
};


/*! \class DimensionEntity
 * 	This class represents basic, reference or tolerance dimension.
 */
class MVO_API DimensionEntity : public Entity
{
public:
	/*! Constructs a DimensionEntity object. It requires specifying the segment key where the DimensionEntity will 
	 * 	insert associated geometry. */
	DimensionEntity(HC_KEY segment_key = INVALID_KEY);
	DimensionEntity(DimensionEntity const & that);
	virtual ~DimensionEntity(); 

	void Set(DimensionEntity const & that);
	DimensionEntity const & operator=(DimensionEntity const & that);
	
	virtual Type GetType() const { return PMI::DimensionType; };

	/*! \return the dimension type of this DimensionEntity */
	Dimension::Type GetDimensionType() const;
	/*! Sets the dimension type of this DimensionEntity. This will replace an existing dimension type. */
	void SetDimensionType(Dimension::Type const in_type);

	/*! \return the dimension subtype of this DimensionEntity */
	Dimension::SubType GetDimensionSubType() const;
	/*! Sets the dimension subtype of this DimensionEntity. This will replace an existing dimension subtype. */
	void SetDimensionSubType(Dimension::SubType const in_sub_type);

	/*! \return the number of strings for this DimensionEntity */
	unsigned int GetStringCount() const;
	/*! Gets the strings for this DimensionEntity.
	 * 	\param out_count the number of strings for this DimensionEntity. Returned to the user.
	 * 	\param out_strings the array of strings for this DimensionEntity. Returned to the user.
	 * 		Necessary space should be allocated prior to calling. If pointer is NULL
	 * 		no values will be put into the array.
	 * 	\param out_text_attributes the array of TextAttributes for the strings for this DimensionEntity. 
	 * 		Returned to the user. Necessary space should be allocated prior to calling. If pointer is NULL
	 * 		no values will be put into the array.
	 */
	void GetStrings(unsigned int & out_count, String out_strings[], TextAttributes out_text_attributes[]) const;
	/*! Sets the strings for this DimensionEntity. These will replace any existing strings.
	 * 	\param in_count the number of strings in the array 
	 * 	\param in_strings the array of strings for this DimensionEntity
	 * 	\param in_text_attributes the array of TextAttributes for the strings for this DimensionEntity
	 */
	void SetStrings(unsigned int in_count, String const in_strings[], TextAttributes const in_text_attributes[]);
};


/*! \class FeatureControlFrameEntity
 * 	This class represents a feature control frame.
 */
class MVO_API FeatureControlFrameEntity : public Entity
{
public:
	/*! Constructs a FeatureControlFrameEntity object. It requires specifying the segment key where the 
	 * 	FeatureControlFrameEntity will insert associated geometry. */
	FeatureControlFrameEntity(HC_KEY segment_key = INVALID_KEY);
	FeatureControlFrameEntity(FeatureControlFrameEntity const & that);
	virtual ~FeatureControlFrameEntity();

	void Set(FeatureControlFrameEntity const & that);
	FeatureControlFrameEntity const & operator=(FeatureControlFrameEntity const & that);

	virtual Type GetType() const { return PMI::FeatureControlFrameType; };

	/*!	Gets the tolerance type information for this FeatureControlFrameEntity.
	 * 	\param out_tolerance_type the tolerance type. Returned to the user.
	 * 	\param out_text_attributes the TextAttributes indicating where the symbol for this tolerance type is placed.
	 * 		Returned to the user.
	 */
	void GetToleranceType(Tolerance::Type & out_tolerance_type, TextAttributes & out_text_attributes) const;
	/*!	Sets the tolerance type information for this FeatureControlFrameEntity. This will replace an existing tolerance type.
	 * 	\param in_tolerance_type the tolerance type
	 * 	\param in_text_attributes the TextAttributes indicating where the symbol for this tolerance type is placed
	 */
	void SetToleranceType(Tolerance::Type const in_tolerance_type, TextAttributes const & in_text_attributes);

	/*!	Gets the diameter modifier type information for this FeatureControlFrameEntity.
	 * 	\param out_diameter_modifier_type the diameter modifier type. Returned to the user.
	 * 	\param out_text_attributes the TextAttributes indicating where the symbol for this diameter modifier type is placed.
	 * 		Returned to the user.
	 */
	void GetDiameterModifierType(DiameterModifier::Type & out_diameter_modifier_type, TextAttributes & out_text_attributes) const;
	/*!	Sets the diameter modifier type information for this FeatureControlFrameEntity. This will replace an existing diameter modifier.
	 * 	\param in_diameter_modifier_type the diameter modifier type
	 * 	\param in_text_attributes the TextAttributes indicating where the symbol for this diameter modifier type is placed
	 */
	void SetDiameterModifierType(DiameterModifier::Type const in_diameter_modifier_type, TextAttributes const & in_text_attributes);

	/*!	Gets the zone tolerance magnitude information for this FeatureControlFrameEntity.
	 * 	\param out_zone_tolerance_magnitude the zone tolerance magnitude. Returned to the user.
	 * 	\param out_text_attributes the TextAttributes indicating where the text for this zone tolerance magnitude is placed.
	 * 		Returned to the user.
	 */
	void GetZoneToleranceMagnitude(String & out_zone_tolerance_magnitude, TextAttributes & out_text_attributes) const;
	/*!	Sets the zone tolerance magnitude information for this FeatureControlFrameEntity. This will replace an existing
	 * 	zone tolerance magnitude.
	 * 	\param in_diameter_modifier_type the diameter modifier type
	 * 	\param in_text_attributes the TextAttributes indicating where the text for this zone tolerance magnitude is placed
	 */
	void SetZoneToleranceMagnitude(String const & in_zone_tolerance_magnitude, TextAttributes const & in_text_attributes);

	/*!	Gets the first zone modifier type information for this FeatureControlFrameEntity.
	 * 	\param out_zone_modifier_type the zone modifier type. Returned to the user.
	 * 	\param out_text_attributes the TextAttributes indicating where the symbol for this zone modifier type is placed.
	 * 		Returned to the user.
	 */
	void GetFirstZoneModifierType(ZoneModifier::Type & out_zone_modifier_type, TextAttributes & out_text_attributes) const;
	/*!	Sets the first zone modifier type information for this FeatureControlFrameEntity. This will replace an existing
	 * 	first zone modifier type.
	 * 	\param in_zone_modifier_type the zone modifier type
	 * 	\param in_text_attributes the TextAttributes indicating where the symbol for this zone modifier type is placed
	 */
	void SetFirstZoneModifierType(ZoneModifier::Type const in_zone_modifier_type, TextAttributes const & in_text_attributes);

	/*!	Gets the second zone modifier type information for this FeatureControlFrameEntity.
	 * 	\param out_zone_modifier_type the zone modifier type. Returned to the user.
	 * 	\param out_text_attributes the TextAttributes indicating where the symbol for this zone modifier type is placed.
	 * 		Returned to the user.
	 */
	void GetSecondZoneModifierType(ZoneModifier::Type & out_zone_modifier_type, TextAttributes & out_text_attributes) const;
	/*!	Sets the second zone modifier type information for this FeatureControlFrameEntity. This will replace an existing
	 * 	second zone modifier type.
	 * 	\param in_zone_modifier_type the zone modifier type
	 * 	\param in_text_attributes the TextAttributes indicating where the symbol for this zone modifier type is placed
	 */
	void SetSecondZoneModifierType(ZoneModifier::Type const in_zone_modifier_type, TextAttributes const & in_text_attributes);

	/*!	Gets the projected magnitude information for this FeatureControlFrameEntity.
	 * 	\param out_projected_magnitude the projected magnitude. Returned to the user.
	 * 	\param out_text_attributes the TextAttributes indicating where the text for this projected magnitude is placed.
	 * 		Returned to the user.
	 */
	void GetProjectedMagnitude(String & out_projected_magnitude, TextAttributes & out_text_attributes) const;
	/*!	Sets the projected magnitude information for this FeatureControlFrameEntity. This will replace an existing 
	 * 	projected magnitude.
	 * 	\param in_projected_magnitude the projected magnitude
	 * 	\param in_text_attributes the TextAttributes indicating where the text for this projected magnitude is placed
	 */
	void SetProjectedMagnitude(String const & in_projected_magnitude, TextAttributes const & in_text_attributes);

	/*! \return the number of datum references for this FeatureControlFrameEntity */
	unsigned int GetDatumReferenceCount() const;
	/*! Gets the datum reference information for this FeatureControlFrameEntity.
	 * 	\param out_count the number of datum references for this FeatureControlFrameEntity. Returned to the user.
	 * 	\param out_labels the array of datum reference labels for this FeatureControlFrameEntity. Returned to the user. 
	 * 		Necessary space should be allocated prior to calling. If pointer is NULL no values will be put into the array.
	 * 	\param out_label_attributes the array of TextAttributes for the datum reference labels for this FeatureControlFrameEntity. 
	 * 		Returned to the user. Necessary space should be allocated prior to calling. If pointer is NULL
	 * 		no values will be put into the array.
	 * 	\param out_modifiers the array of datum modifiers for this FeatureControlFrameEntity. Returned to the user. 
	 * 		Necessary space should be allocated prior to calling. If pointer is NULL no values will be put into the array.
	 * 	\param out_modifier_attributes the array of TextAttributes for the datum modifiers for this FeatureControlFrameEntity. 
	 * 		Returned to the user. Necessary space should be allocated prior to calling. If pointer is NULL
	 * 		no values will be put into the array.
	 */
	void GetDatumReferences(unsigned int & out_count, String out_labels[], TextAttributes out_label_attributes[],
							DatumModifier::Type out_modifiers[], TextAttributes out_modifier_attributes[]);
	/*! Sets the datum reference information for this FeatureControlFrameEntity. These will replace any existing datum references.
	 * 	\param in_count the number of datum references in the arrays
	 * 	\param in_labels the array of datum reference labels for this FeatureControlFrameEntity
	 * 	\param in_label_attributes the array of TextAttributes for the datum reference labels for this FeatureControlFrameEntity 
	 * 	\param in_modifiers the array of datum modifiers for this FeatureControlFrameEntity
	 * 	\param in_modifier_attributes the array of TextAttributes for the datum modifiers for this FeatureControlFrameEntity 
	 */
	void SetDatumReferences(unsigned int in_count, String const in_labels[], TextAttributes const in_label_attributes[],
							DatumModifier::Type const in_modifiers[], TextAttributes const in_modifier_attributes[]);
};


/*! \class GenericEntity
 * 	This class represents a generic PMI entity, i.e., any entity not represented by any other PMI entity.
 */
class MVO_API GenericEntity : public Entity
{
public:
	/*! Constructs a GenericEntity object. It requires specifying the segment key where the GenericEntity will insert
	 * 	associated geometry. */
	GenericEntity(HC_KEY segment_key = INVALID_KEY);
	GenericEntity(GenericEntity const & that);
	virtual ~GenericEntity();

	void Set(GenericEntity const & that);
	GenericEntity const & operator=(GenericEntity const & that);

	virtual Type GetType() const { return PMI::GenericType; };

	/*! \return the number of strings for this GenericEntity */
	unsigned int GetStringCount() const;
	/*! Gets the strings for this GenericEntity.
	 * 	\param out_count the number of strings for this GenericEntity. Returned to the user.
	 * 	\param out_strings the array of strings for this GenericEntity. Returned to the user.
	 * 		Necessary space should be allocated prior to calling. If pointer is NULL
	 * 		no values will be put into the array.
	 * 	\param out_text_attributes the array of TextAttributes for the strings for this GenericEntity. 
	 * 		Returned to the user. Necessary space should be allocated prior to calling. If pointer is NULL
	 * 		no values will be put into the array.
	 */
	void GetStrings(unsigned int & out_count, String out_strings[], TextAttributes out_text_attributes[]) const;
	/*! Sets the strings for this GenericEntity. These will replace any existing strings.
	 * 	\param in_count the number of strings in the array 
	 * 	\param in_strings the array of strings for this GenericEntity
	 * 	\param in_text_attributes the array of TextAttributes for the strings for this GenericEntity
	 */
	void SetStrings(unsigned int in_count, String const in_strings[], TextAttributes const in_text_attributes[]);


	/*! Sets the display behavior for this GenericEntity.
	 * 	\param in_parallel option value 
	 */
	void SetDisplayParallelToScreen(bool const in_parallel = true);
	/*! Gets the display behavior for this GenericEntity.
	 * 	\return option value 
	 */
	bool IsDisplayParallelToScreen() const;

};


/*! \class NoteEntity
 * 	This class represents a single or multi-line block of text.
 */
class MVO_API NoteEntity : public Entity
{
public:
	/*! Constructs a NoteEntity object. It requires specifying the segment key where the NoteEntity will insert
	 * 	associated geometry. */
	NoteEntity(HC_KEY segment_key = INVALID_KEY);
	NoteEntity(NoteEntity const & that);
	virtual ~NoteEntity();

	void Set(NoteEntity const & that);
	NoteEntity const & operator=(NoteEntity const & that);

	virtual Type GetType() const { return PMI::NoteType; };

	/*! \return the number of strings for this NoteEntity */
	unsigned int GetStringCount() const;
	/*! Gets the strings for this NoteEntity.
	 * 	\param out_count the number of strings for this NoteEntity. Returned to the user.
	 * 	\param out_strings the array of strings for this NoteEntity. Returned to the user.
	 * 		Necessary space should be allocated prior to calling. If pointer is NULL
	 * 		no values will be put into the array.
	 * 	\param out_text_attributes the array of TextAttributes for the strings for this NoteEntity. 
	 * 		Returned to the user. Necessary space should be allocated prior to calling. If pointer is NULL
	 * 		no values will be put into the array.
	 */
	void GetStrings(unsigned int & out_count, String out_strings[], TextAttributes out_text_attributes[]) const;
	/*! Sets the strings for this NoteEntity. These will replace any existing strings.
	 * 	\param in_count the number of strings in the array 
	 * 	\param in_strings the array of strings for this NoteEntity
	 * 	\param in_text_attributes the array of TextAttributes for the strings for this NoteEntity
	 */
	void SetStrings(unsigned int in_count, String const in_strings[], TextAttributes const in_text_attributes[]);
};


/*! \class RoughnessEntity
 * 	This class represents a roughness feature.
 */
class MVO_API RoughnessEntity : public Entity
{
public:
	/*! Constructs a RoughnessEntity object. It requires specifying the segment key where the RoughnessEntity will insert
	 * 	associated geometry. */
	RoughnessEntity(HC_KEY segment_key = INVALID_KEY);
	RoughnessEntity(RoughnessEntity const & that);
	virtual ~RoughnessEntity();

	void Set(RoughnessEntity const & that);
	RoughnessEntity const & operator=(RoughnessEntity const & that);
	
	virtual Type GetType() const { return PMI::RoughnessType; };

	/*! \return the obtention type of this RoughnessEntity */
	Roughness::Obtention::Type GetObtentionType() const;
	/*! Sets the obtention type of this RoughnessEntity. This will replace an existing obtention type. */
	void SetObtentionType(Roughness::Obtention::Type const in_obtention_type);

	/*! \return the applicability type of this RoughnessEntity */
	Roughness::Applicability::Type GetApplicabilityType() const;
	/*! Sets the applicability type of this RoughnessEntity. This will replace an existing applicability type. */
	void SetApplicabilityType(Roughness::Applicability::Type const in_applicability_type);

	/*! \return the mode type of this RoughnessEntity */
	Roughness::Mode::Type GetModeType() const;
	/*! Sets the mode type of this RoughnessEntity. This will replace an existing mode type. */
	void SetModeType(Roughness::Mode::Type const in_mode_type);

	/*! \return the number of fields for this RoughnessEntity */
	unsigned int GetFieldCount() const;
	/*! Gets the fields for this RoughnessEntity.
	 * 	\param out_count the number of fields for this RoughnessEntity. Returned to the user.
	 * 	\param out_fields the array of fields for this RoughnessEntity. Returned to the user.
	 * 		Necessary space should be allocated prior to calling. If pointer is NULL
	 * 		no values will be put into the array.
	 * 	\param out_text_attributes the array of TextAttributes for the fields for this RoughnessEntity. 
	 * 		Returned to the user. Necessary space should be allocated prior to calling. If pointer is NULL
	 * 		no values will be put into the array.
	 */
	void GetFields(unsigned int & out_count, String out_fields[], TextAttributes out_text_attributes[]) const;
	/*! Sets the fields for this RoughnessEntity. These will replace any existing fields.
	 * 	\param in_count the number of fields in the array 
	 * 	\param in_fields the array of fields for this RoughnessEntity
	 * 	\param in_text_attributes the array of TextAttributes for the fields for this RoughnessEntity
	 */
	void SetFields(unsigned int in_count, String const in_fields[], TextAttributes const in_text_attributes[]);
};


/*! \class ViewEntity
 * 	This class represents a view, or a collection of PMI entities.
 */
class MVO_API ViewEntity
{
public:
	/*! Constructs a ViewEntity object. It requires specifying the segment key where the ViewEntity will insert
	 * 	associated geometry. */
	ViewEntity(HC_KEY segment_key = INVALID_KEY);
	ViewEntity(ViewEntity const & that);
	virtual ~ViewEntity();

	void Set(ViewEntity const & that);
	ViewEntity const & operator=(ViewEntity const & that);

	/*! Gets the camera associated with this ViewEntity. Returned to the user. */
	void GetCamera(Camera & out_camera) const;
	/*! Gets the camera associated with this ViewEntity. Returned to the user. */
	void GetCamera(Point & out_position, Point & out_target, Point & out_up_vector, float & out_field_width, float & out_field_height,
			char out_projection[128]);
	/*! Gets the camera associated with this ViewEntity. Returned to the user. */
	void GetCamera(float out_position[3], float out_target[3], float out_up_vector[3], float & out_field_width, float & out_field_height,
			char out_projection[128]);
	/*! Sets the camera associated with this ViewEntity. This will replace an existing camera. */
	void SetCamera(Camera const & in_camera);
	/*! Sets the camera associated with this ViewEntity. This will replace an existing camera. */
	void SetCamera(Point const & in_position, Point const & in_target, Point const & in_up_vector, float in_field_width,
			float in_field_height, char const in_projection[128]);
	/*! Sets the camera associated with this ViewEntity. This will replace an existing camera. */
	void SetCamera(float const in_position[3], float const in_target[3], float const in_up_vector[3], float in_field_width,
			float in_field_height, char const in_projection[128]);

	/*! Gets the Frame associated with this ViewEntity. Returned to the user. */
	void GetFrame(Frame & out_frame) const;
	/*! Sets the Frame associated with this ViewEntity. This will replace an existing Frame. */
	void SetFrame(Frame const & in_frame);

	/*! Gets the name associated with this ViewEntity. Returned to the user. */
	void GetName(String & out_name) const;
	/*! Sets the name associated with this ViewEntity. This will replace an existing name. */
	void SetName(String const & in_name);

	/*! \return the number of associated entities for this ViewEntity */
	unsigned int GetAssociatedEntityCount() const;
	/*! Gets the entities associated with this ViewEntity
	 * 	\param out_count the number of associated entities. Returned to the user.
	 * 	\param out_entities the array of entities associated with this ViewEntity. Returned to the user.
	 * 		Necessary space should be allocated prior to calling. If pointer is NULL
	 * 		no values will be put into the array.
	 */
	void GetAssociatedEntities(unsigned int & out_count, Entity out_entities[]) const;
	/*! Sets the entities associated with this ViewEntity. These will replace any existing entity associations.
	 * 	\param in_count the number of associated entities
	 * 	\param in_entities the array of entities to associate with this ViewEntity
	 */
	void SetAssociatedEntities(unsigned int in_count, Entity const in_entities[]);

protected:
	PMII::PrivateImpl * impl_;
};

// ====================== end actual entities =======================


}; // PMI

#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif // _HUTILITYPMI_H
