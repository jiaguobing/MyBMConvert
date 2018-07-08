// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

/*** C++ STYLE COMMENTS PROHIBITED IN THIS FILE ***/


#ifndef HIC_TYPES_DEFINED

/* for C compatibility */
#ifndef __cplusplus
#ifndef bool
#define bool int
#endif
#endif


/* opaque types */
typedef struct hic_rendition_s {
	void * opaque;
} HIC_Rendition;

typedef struct HIC_Segment_Info_s {
	void * opaque;
}  HIC_Segment_Info;

typedef struct HIC_Shell_s {
	void * opaque;
}  HIC_Shell;

typedef struct HIC_Convex_Clip_Region_s {
	void * opaque;
}  HIC_Convex_Clip_Region, HIC_Polygonal_Clip_Region, HIC_Complex_Clip_Region;

typedef struct HIC_User_Face_Pattern_s {
	void * opaque;
}  HIC_User_Face_Pattern;

typedef struct HIC_Line_Style_s {
	void * opaque;
}  HIC_Line_Style;

typedef struct HIC_Driver_Color_s {
	void * opaque;
}  HIC_Driver_Color;

typedef struct HIC_Segment_s {
	void * opaque;
}  HIC_Segment;

typedef struct HIC_Text_Karacter_Info_s {
	void * opaque;
}  HIC_Text_Karacter_Info;

typedef struct HIC_Text_Info_s {
	void * opaque;
}  HIC_Text_Info;


/* opaque 3d geometry types */

typedef struct HIC_Geometry_s {
	void * opaque;
}  HIC_Geometry;


typedef HIC_Geometry	HIC_Marker;
typedef HIC_Geometry	HIC_Polyline;
typedef HIC_Geometry	HIC_Polygon;
typedef HIC_Geometry	HIC_Text;
typedef HIC_Geometry	HIC_Stencil_Bitmap;
typedef HIC_Geometry	HIC_Tristrip;
typedef HIC_Geometry	HIC_Polyhedron;
typedef HIC_Geometry	HIC_Polyedge;
typedef HIC_Geometry	HIC_Polymarker;
typedef HIC_Geometry	HIC_Image;
typedef HIC_Geometry	HIC_Ellipse;
typedef HIC_Geometry	HIC_Elliptical_Arc;
typedef HIC_Geometry	HIC_NURBS_Curve;
typedef HIC_Geometry	HIC_NURBS_Surface;
typedef HIC_Geometry	HIC_Cylinder;
typedef HIC_Geometry	HIC_Polycylinder;
typedef HIC_Geometry	HIC_Sphere;
typedef HIC_Geometry	HIC_Grid;

/*back compatibility*/
#define HIC_User_Line_Pattern HIC_Line_Style



/* real types */
typedef int				HIC_Integer32;
typedef unsigned short	HIC_Karacter;
typedef float			HIC_Parameter;
typedef HIC_Integer32	HIC_Parameter_Flags;
typedef HIC_Integer32	HIC_RGBAS32;

typedef struct hic_int_size {
	int width, height;
} HIC_Int_Size;

typedef struct hic_int_rectangle {
	int					left,	right, bottom, top;
}	HIC_Int_Rectangle;

typedef struct hic_volume {
	float				left, right, bottom, top, hither, yon;
}	HIC_Volume;

typedef struct hic_point {
	float				x, y, z;
}	HIC_Point;

typedef struct hic_vector {
	float				x, y, z;
}	HIC_Vector;

typedef struct hic_dc_point {
	float				x, y, z;
}	HIC_DC_Point;

typedef struct hic_plane {
	float				a, b, c, d;
}	HIC_Plane;

typedef struct hic_dplane {
	double				a, b, c, d;
}	HIC_DPlane;

typedef struct hic_rgba {
	float				red, green, blue, alpha;
} HIC_RGBA;

typedef struct hic_rgb {
	float				red, green, blue;
} HIC_RGB;

typedef struct hic_segment_path {
	HC_KEY const *		keys;
	int					count;
} HIC_Segment_Path;	

typedef struct hic_bounding_cuboid {
	HIC_Point			min;
	HIC_Point			max;
	bool				valid;
} HIC_Bounding_Cuboid;



/* locks */
typedef int				HIC_Material_Color_Locks;
typedef int				HIC_Color_Locks;
typedef int				HIC_Visibility_Locks;
typedef int				HIC_Attribute_Locks;

/*!
  \addtogroup HIC_Enumerations
  @{
*/
typedef enum HIC_Attributes_e {
	HIC_Attribute_STYLE					= 0x00000001,
	HIC_Attribute_RENDERING_OPTIONS		= 0x00000002,
	HIC_Attribute_COLOR_MAP				= 0x00000004,
	HIC_Attribute_SELECTABILITY			= 0x00000008,
	HIC_Attribute_VISIBILITY			= 0x00000010,
	HIC_Attribute_COLOR					= 0x00000020,
	HIC_Attribute_WINDOW_FRAME			= 0x00000040,
	HIC_Attribute_WINDOW_PATTERN		= 0x00000080,
	HIC_Attribute_WINDOW				= 0x00000100,
	HIC_Attribute_CAMERA				= 0x00000200,
	HIC_Attribute_HEURISTICS			= 0x00000400,
	HIC_Attribute_MODELLING_MATRIX		= 0x00000800,
	HIC_Attribute_CALLBACK				= 0x00001000,
	HIC_Attribute_CLIP_REGION			= 0x00002000,
	HIC_Attribute_DRIVER				= 0x00004000,
	HIC_Attribute_DRIVER_OPTIONS		= 0x00008000,
	HIC_Attribute_EDGE_PATTERN			= 0x00010000,
	HIC_Attribute_EDGE_WEIGHT			= 0x00020000,
	HIC_Attribute_FACE_PATTERN			= 0x00040000,
	HIC_Attribute_HANDEDNESS			= 0x00080000,
	HIC_Attribute_LINE_PATTERN			= 0x00100000,
	HIC_Attribute_LINE_WEIGHT			= 0x00200000,
	HIC_Attribute_MARKER_SIZE			= 0x00400000,
	HIC_Attribute_MARKER_SYMBOL			= 0x00800000,
	HIC_Attribute_TEXT_ALIGNMENT		= 0x01000000,
	HIC_Attribute_TEXT_FONT				= 0x02000000,
	HIC_Attribute_TEXT_PATH				= 0x04000000,
	HIC_Attribute_TEXT_SPACING			= 0x08000000,
	HIC_Attribute_USER_OPTIONS			= 0x10000000,
	HIC_Attribute_USER_VALUE			= 0x20000000,
	HIC_Attribute_TEXTURE_MATRIX		= 0x40000000
} HIC_Attributes;


/* if you change any of these flags, you must change their shadow flags in database.h */
typedef enum HIC_Visibility_e {
	HIC_Visibility_FACES				= 0x00000001,
	HIC_Visibility_EDGES				= 0x00000002,
	HIC_Visibility_LINES				= 0x00000004,
	HIC_Visibility_MARKERS				= 0x00000008,
	HIC_Visibility_VERTICES				= 0x00000010,
	HIC_Visibility_IMAGES				= 0x00000020,
	HIC_Visibility_TEXT					= 0x00000040,
	HIC_Visibility_STRING_CURSORS		= 0x00000080,
	HIC_Visibility_WINDOWS				= 0x00000100,

	HIC_Visibility_CUTTING_PLANES		= 0x00000200,

	HIC_Visibility_SILHOUETTE_EDGES		= 0x00010000,
	HIC_Visibility_PERIMETER_EDGES		= 0x00020000,
	HIC_Visibility_ADJACENT_EDGES		= 0x00040000,
	HIC_Visibility_MESH_QUAD_EDGES		= 0x00080000,
	HIC_Visibility_HARD_EDGES			= 0x00100000,
	HIC_Visibility_NONCULLED_EDGES		= 0x00200000,

	HIC_Visibility_LIGHTS_FOR_FACES		= 0x00400000,
	HIC_Visibility_LIGHTS_FOR_EDGES		= 0x00800000,
	HIC_Visibility_LIGHTS_FOR_MARKERS	= 0x01000000,

	HIC_Visibility_ISOLINES				= 0x02000000,

	HIC_Visibility_SHADOW_RECEIVING		= 0x04000000,
	HIC_Visibility_SHADOW_CASTING		= 0x08000000,
	HIC_Visibility_SHADOW_EMITTING		= 0x10000000,
	HIC_Visibility_ANY_SHADOW			=(HIC_Visibility_SHADOW_RECEIVING | HIC_Visibility_SHADOW_CASTING | HIC_Visibility_SHADOW_EMITTING),

	HIC_Visibility_CUT_FACES			= 0x20000000,
	HIC_Visibility_CUT_EDGES			= 0x40000000,
	HIC_Visibility_CUT_GEOMETRY			= HIC_Visibility_CUT_FACES | HIC_Visibility_CUT_EDGES,

	HIC_Visibility_GENERIC_EDGES		= HIC_Visibility_EDGES,
	HIC_Visibility_POLYGON_EDGES		= HIC_Visibility_GENERIC_EDGES|HIC_Visibility_PERIMETER_EDGES|
											HIC_Visibility_ADJACENT_EDGES,
	HIC_Visibility_GRID_EDGES			= HIC_Visibility_POLYGON_EDGES,
	HIC_Visibility_SHELL_EDGES			= HIC_Visibility_POLYGON_EDGES|HIC_Visibility_HARD_EDGES|
											HIC_Visibility_SILHOUETTE_EDGES|HIC_Visibility_NONCULLED_EDGES,
	HIC_Visibility_MESH_EDGES			= HIC_Visibility_SHELL_EDGES|HIC_Visibility_MESH_QUAD_EDGES,
	HIC_Visibility_SPHERE_EDGES			= HIC_Visibility_GENERIC_EDGES|HIC_Visibility_SILHOUETTE_EDGES|
											HIC_Visibility_MESH_QUAD_EDGES|HIC_Visibility_NONCULLED_EDGES,

	HIC_Visibility_ANY_EDGE				= HIC_Visibility_MESH_EDGES,

	HIC_Visibility_LIGHTS				= HIC_Visibility_LIGHTS_FOR_FACES|HIC_Visibility_LIGHTS_FOR_EDGES|HIC_Visibility_LIGHTS_FOR_MARKERS,

	HIC_Visibility_GEOMETRY				= HIC_Visibility_FACES|HIC_Visibility_ANY_EDGE|HIC_Visibility_LINES|HIC_Visibility_MARKERS|
											HIC_Visibility_VERTICES|HIC_Visibility_IMAGES|HIC_Visibility_TEXT|HIC_Visibility_STRING_CURSORS|
											HIC_Visibility_LIGHTS|HIC_Visibility_CUTTING_PLANES
} HIC_Visibility;


typedef enum HIC_Color_Lock_Objects_e {	
	HIC_Color_Lock_Objects_NONE				=0x00000000,
	HIC_Color_Lock_Objects_UNKNOWN			=0x80000000,

	HIC_Color_Lock_Objects_FRONT			=0x00000001,
	HIC_Color_Lock_Objects_BACK				=0x00000002,
	HIC_Color_Lock_Objects_EDGE				=0x00000004,
	HIC_Color_Lock_Objects_LINE				=0x00000008,
	HIC_Color_Lock_Objects_VERTEX			=0x00000010,
	HIC_Color_Lock_Objects_MARKER			=0x00000020,
	HIC_Color_Lock_Objects_TEXT				=0x00000040,
	HIC_Color_Lock_Objects_CUT_FACE			=0x00000080,
	HIC_Color_Lock_Objects_CUT_EDGE			=0x00000100,
	HIC_Color_Lock_Objects_WINDOW			=0x00000200,

	HIC_Color_Lock_Objects_FRONT_CONTRAST	=0x00000400,
	HIC_Color_Lock_Objects_BACK_CONTRAST	=0x00000800,
	HIC_Color_Lock_Objects_EDGE_CONTRAST	=0x00001000,
	HIC_Color_Lock_Objects_LINE_CONTRAST	=0x00002000,
	HIC_Color_Lock_Objects_VERTEX_CONTRAST	=0x00004000,
	HIC_Color_Lock_Objects_MARKER_CONTRAST	=0x00008000,
	HIC_Color_Lock_Objects_TEXT_CONTRAST	=0x00010000,
	HIC_Color_Lock_Objects_CUT_FACE_CONTRAST=0x00020000,
	HIC_Color_Lock_Objects_CUT_EDGE_CONTRAST=0x00040000,
	HIC_Color_Lock_Objects_WINDOW_CONTRAST	=0x00080000,

	HIC_Color_Lock_Objects_LIGHTING			=0x00100000,
	HIC_Color_Lock_Objects_AMBIENT_UP		=0x00200000,
	HIC_Color_Lock_Objects_AMBIENT_DOWN		=0x00400000,

	HIC_Color_Lock_Objects_SIMPLE_REFLECTION = 0x00800000,

	HIC_Color_Lock_Objects_AMBIENT			=(HIC_Color_Lock_Objects_AMBIENT_UP|HIC_Color_Lock_Objects_AMBIENT_DOWN),
	HIC_Color_Lock_Objects_FACE				=(HIC_Color_Lock_Objects_FRONT|HIC_Color_Lock_Objects_BACK),
	HIC_Color_Lock_Objects_FACE_CONTRAST	=(HIC_Color_Lock_Objects_FRONT_CONTRAST|HIC_Color_Lock_Objects_BACK_CONTRAST),
	HIC_Color_Lock_Objects_POLYGON			=(HIC_Color_Lock_Objects_FACE|HIC_Color_Lock_Objects_EDGE),
	HIC_Color_Lock_Objects_GEOMETRY			=(HIC_Color_Lock_Objects_POLYGON|HIC_Color_Lock_Objects_LINE|
												HIC_Color_Lock_Objects_MARKER|HIC_Color_Lock_Objects_VERTEX|
												HIC_Color_Lock_Objects_TEXT),
	HIC_Color_Lock_Objects_CUT_GEOMETRY		=(HIC_Color_Lock_Objects_CUT_FACE|HIC_Color_Lock_Objects_CUT_EDGE),
	HIC_Color_Lock_Objects_CUT_GEOMETRY_CONTRAST
											=(HIC_Color_Lock_Objects_CUT_FACE_CONTRAST|HIC_Color_Lock_Objects_CUT_EDGE_CONTRAST),
	HIC_Color_Lock_Objects_CONTRASTS		=(HIC_Color_Lock_Objects_FACE_CONTRAST|HIC_Color_Lock_Objects_EDGE_CONTRAST|
												HIC_Color_Lock_Objects_LINE_CONTRAST|HIC_Color_Lock_Objects_MARKER_CONTRAST|
												HIC_Color_Lock_Objects_VERTEX_CONTRAST|HIC_Color_Lock_Objects_TEXT_CONTRAST|
												HIC_Color_Lock_Objects_WINDOW_CONTRAST|HIC_Color_Lock_Objects_CUT_FACE_CONTRAST),
	HIC_Color_Lock_Objects_EVERYTHING		=(HIC_Color_Lock_Objects_WINDOW|HIC_Color_Lock_Objects_GEOMETRY|
												HIC_Color_Lock_Objects_CUT_GEOMETRY|HIC_Color_Lock_Objects_CONTRASTS|
												HIC_Color_Lock_Objects_LIGHTING|HIC_Color_Lock_Objects_AMBIENT)
} HIC_Color_Lock_Objects;	


typedef enum HIC_Material_Components_e {
	HIC_Color_Channel_NONE					= 0x0000,
	HIC_Color_Channel_DIFFUSE				= 0x0001,
	HIC_Color_Channel_SPECULAR				= 0x0002,
	HIC_Color_Channel_MIRROR				= 0x0004,
	HIC_Color_Channel_TRANSMISSION			= 0x0008,
	HIC_Color_Channel_EMISSION				= 0x0010,

	HIC_Color_Channel_GLOSS					= 0x0020,
	HIC_Color_Channel_INDEX					= 0x0040,

	HIC_Color_Channel_DIFFUSE_TEXTURE		= 0x0100,
	HIC_Color_Channel_SPECULAR_TEXTURE		= 0x0200,
	HIC_Color_Channel_MIRROR_TEXTURE		= 0x0400,
	HIC_Color_Channel_TRANSMISSION_TEXTURE	= 0x0800,
	HIC_Color_Channel_EMISSION_TEXTURE		= 0x1000,

	HIC_Color_Channel_BUMP_TEXTURE			= 0x2000,
	HIC_Color_Channel_ENVIRONMENT_TEXTURE	= 0x4000,

	HIC_Color_Channel_HAS_TEXTURES			= 0x8000,
	HIC_Color_Channel_ANY_TEXTURE			= 0x7F00,
	HIC_Color_Channel_ANY_COLOR				= 0x001F,
	HIC_Color_Channel_LOCKABLE				= 0x7F7F
} HIC_Material_Components;

typedef enum HIC_Image_Format_e {
	HIC_Image_Format_NONE,
	HIC_Image_Format_MAPPED_8,
	HIC_Image_Format_MAPPED_16,
	HIC_Image_Format_RGB24,
	HIC_Image_Format_ARGB32,
	HIC_Image_Format_RGBA32,
	HIC_Image_Format_DEPTH_FLOAT32,
	HIC_Image_Format_RGBAS32,
	HIC_Image_Format_DEPTH16,
	HIC_Image_Format_STENCIL,
	HIC_Image_Format_DEPTH32,
	HIC_Image_Format_DXT1,
	HIC_Image_Format_DXT3,
	HIC_Image_Format_DXT5,
	HIC_Image_Format_PNG,
	HIC_Image_Format_JPEG,
	HIC_Image_Format_TARGA,
	HIC_Image_Format_LUMINANCE8
} HIC_Image_Format;

typedef enum HIC_Size_Units_e {
	HIC_GSU_UNQUALIFIED,
	HIC_GSU_ORU,
	HIC_GSU_SRU,
	HIC_GSU_WRU,
	HIC_GSU_PIXELS,
	HIC_GSU_POINTS,
	HIC_GSU_PERCENT,
	HIC_GSU_SCALE,
	HIC_GSU_WORLD
} HIC_Size_Units;


typedef enum HIC_Geometry_Type_e {
	HIC_Geometry_Type_CUTTING_PLANE	= 38,
	HIC_Geometry_Type_LIGHT,
	HIC_Geometry_Type_SHELL,
	HIC_Geometry_Type_MESH,
	HIC_Geometry_Type_GRID,
	HIC_Geometry_Type_NURBS_SURFACE,
	HIC_Geometry_Type_CYLINDER,
	HIC_Geometry_Type_POLYCYLINDER,
	HIC_Geometry_Type_SPHERE,
	HIC_Geometry_Type_POLYGON,
	HIC_Geometry_Type_CIRCLE,
	HIC_Geometry_Type_CIRCULAR_CHORD,
	HIC_Geometry_Type_CIRCULAR_WEDGE,
	HIC_Geometry_Type_ELLIPSE,
	HIC_Geometry_Type_LINE,
	HIC_Geometry_Type_MULTILINE,
	HIC_Geometry_Type_POLYLINE,
	HIC_Geometry_Type_NURBS_CURVE,
	HIC_Geometry_Type_CIRCULAR_ARC,
	HIC_Geometry_Type_ELLIPTICAL_ARC,
	HIC_Geometry_Type_INFINITE_LINE,
	HIC_Geometry_Type_IMAGE,
	HIC_Geometry_Type_MARKER,
	HIC_Geometry_Type_MULTIMARKER,
	HIC_Geometry_Type_TEXT,
	HIC_Geometry_Type_STRING_CURSOR,
	HIC_Geometry_Type_GEOMETRY_REFERENCE,
	HIC_Geometry_Type_MISSING_GEOMETRY,
	HIC_Geometry_Type_INCLUDE, // HIC_Show_Geometry_Type will not show this deleted key callback will
	HIC_Geometry_Type_SEGMENT, // HIC_Show_Geometry_Type will not show this deleted key callback will
	HIC_Geometry_Type_BAD_TYPE = -1
} HIC_Geometry_Type;

	
typedef enum HIC_KInfo_Flags_e {
	HIC_KInfo_Flags_NONE				= 0x00000000,
	HIC_KInfo_Flags_OMITTED				= 0x00000001,
	HIC_KInfo_Flags_INVISIBLE			= 0x00000002,
	HIC_KInfo_Flags_MISSING				= 0x00000004,
	HIC_KInfo_Flags_VERTICAL_OFFSET		= 0x00000010,
	HIC_KInfo_Flags_HORIZONTAL_OFFSET	= 0x00000020,
	HIC_KInfo_Flags_COLOR				= 0x00000040
} HIC_KInfo_Flags;
/** @} */ /* end of HIC_Enumerations */

#define HIC_TYPES_DEFINED
#endif

