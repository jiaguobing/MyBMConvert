// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef DRIVER_DEFINED
#define DRIVER_DEFINED 1

#ifndef DATABASE_DEFINED
#	include "database.h"
#endif

Begin_HOOPS_Namespace

#define TOTAL_FACE_BIT_PATTERNS 17	 /* Should match patterns.h */
#define DITHER_BIT_PATTERNS		65	 /* intensities 0.0 thru 1.0, inclusive */
#define TOTAL_BIT_PATTERNS		(TOTAL_FACE_BIT_PATTERNS+DITHER_BIT_PATTERNS)

#define DITHER_BIT_RESOLUTION	64	 /* ie, 1 part in 64 */

#define MAX_NOMINAL_COLOR_RESOLUTION	256


#define FONT_HASH_SIZE			128


/* There is a clipping precision problem if we try to cut off almost all
 * of a line upon entering - ie, if the entering 't' value is very nearly
 * 1.0. If floats have 6 digits of precision, then "0.99" will allow
 * 4 digits to remain. Only a very few lines will be within but less
 * than 1% (1 - .99) within.
 */
#define CLIP_T_TOLERANCE		0.01f
#define MAX_T_ENTERING			(1.0f - CLIP_T_TOLERANCE)

/* allow for very tiny round-off problems when clipping */
#define PIXEL_FUDGE		0.001f

/*
 * specifies the depth of the ortho space in z,
 * expressed relative to the camera-to-target distance
 * this value is applied to both sides of the target
 */
#define ORTHO_RANGE	   4.0f

#define NOMINAL_RASTER_LENGTH	2048

#define DEFAULT_MARKER_SIZE		0.015f


//an offset on transform_rendition incarnation to use when toggling z writes
#define DISABLE_ZWRITE_MAGIC  (0x71649572)



typedef float			New_Character_Width;
typedef short			Character_Width;
typedef int32_t			Incarnation;


typedef int32_t			Gray_Level;

struct Driver_Color {
	public:
		union {
			DirectRGB			direct_rgb;		// used with DIRECT_RGB color system
			Color_Index			map_index;		// used with MAPPED_RGB color system
			Gray_Level			gray_level;		// used with GRAY_SCALE color system
		};

		Driver_Color () {}
		Driver_Color (Driver_Color const & c) { *this = c; }
		Driver_Color (RGBAS32 const & c) { *this = c; }

		Driver_Color const &	operator= (Driver_Color const & c)	{ whole() = c.whole(); return *this; }
		Driver_Color const &	operator= (RGBAS32 const & c)		{ whole() = (int32_t const &)c; return *this; }

		bool			operator== (Driver_Color const & c) const	{ return  whole() == c.whole(); }
		bool			operator!= (Driver_Color const & c) const	{ return  !(*this == c); }

		operator RGBAS32 const & () const	{ return *(RGBAS32 const *)this; }

		void clear() {whole() = 0;}
		bool invalid() const {return (whole() == 0);}
		bool valid() const {return (whole() != 0);}

	private:
		// allow us to internally do what the old "whole_thing" part (as a union) let us do explicitly
		int32_t const &	whole() const	{ return *(int32_t const *)this; }
		int32_t &	whole()	{ return *(int32_t *)this; }
};
static_assert(sizeof (Driver_Color) == sizeof (RGBAS32), "Size of Driver_Color is wrong");



typedef short Info_Contents;
#define Info_UNIFORM_RGBA				(Info_Contents)0x0001
#define Info_VERTEX_RGBAS				(Info_Contents)0x0002
#define Info_VERTEX_COLORS				(Info_Contents)0x0004
#define Info_VERTEX_PARAMETERS			(Info_Contents)0x0008
#define Info_VERTEX_NORMALS				(Info_Contents)0x0010
#define Info_VERTEX_PLANES				(Info_Contents)0x0020
#define Info_VERTEX_DPLANES				(Info_Contents)0x0040

#define PF_NONE					(Parameter_Flags)0x00000000
#define PF_IGNORE_TRANSFORM		(Parameter_Flags)0x00000001
#define PF_VALID				(Parameter_Flags)0x00000002
#define PF_DEFERRED				(Parameter_Flags)0x00000004
#define PF_SOURCE_EXPLICIT		(Parameter_Flags)0x00000010
#define PF_SOURCE_IMPLICIT		(Parameter_Flags)0x00000020
#define PF_SOURCE_OTHER			(Parameter_Flags)0x00000040
#define PF_SOURCE_WORLD			(Parameter_Flags)0x00000080
#define PF_SOURCE_NORMAL		(Parameter_Flags)0x00000100
#define PF_SOURCE_SPHERE		(Parameter_Flags)0x00000200
#define PF_SOURCE_CYLINDER		(Parameter_Flags)0x00000400
#define PF_SOURCE_OBJECT		(Parameter_Flags)0x00000800
#define PF_NEED_FIXUPS			(Parameter_Flags)0x00001000
#define PF_SOURCE_NATURAL		(Parameter_Flags)0x00002000
#define PF_IMAGE				(Parameter_Flags)0x00004000

//#define PF_OWNS_PARAMETER_ARRAY	(Parameter_Flags)0x80000000

#define PF_PARAM_SOURCE_ANYBITS (PF_SOURCE_WORLD|PF_SOURCE_NORMAL|PF_SOURCE_SPHERE|PF_SOURCE_CYLINDER|PF_SOURCE_OBJECT)


#define INTERPOLATE_PARAMETER(nr, p0, p1, fraction, out, ratio, param_width) do { \
			if (ANYBIT(nr->misc_rendition->simple_options, Rendo_PERSPECTIVE_CORRECTION)) { \
				float inv_denom; \
				inv_denom = (fraction) / ((ratio) + (1.0f - (ratio)) * (fraction)); \
				for (int iii=0; iii<(param_width); iii++) { \
					(out)[iii] = ((p0)[iii] + ((p1)[iii] - (p0)[iii]) * inv_denom); \
				} \
			} \
			else { \
				for (int iii=0; iii<(param_width); iii++) { \
					(out)[iii] = ((p0)[iii] + ((p1)[iii] - (p0)[iii]) * (fraction)); \
				} \
			} \
		} while (0)


struct Cutting_Plane_Instance {
	Cutting_Plane_Instance *	next;
	Plane						world_plane;
	DPlane						world_dplane;
	Plane						dc_plane;
	Cutting_Plane const *		cutter;
	int							offset;
	int							id;

	bool						internal;	/* not from user, so not for cut geometry */
	bool						double_precision;

	INLINE void	REVERSE_CUTTING_PLANE_INSTANCE()
	{
		world_plane = -world_plane;
		world_dplane = -world_dplane;
		dc_plane = -dc_plane;
	}
};

struct Cutting_Plane_Set {
	Cutting_Plane_Set *				next;

	Cutting_Plane_Instance const *	cutting_plane_instances;

	Cutting_Plane_Options			options;

	int								cutting_plane_count;

	bool							drawn;
};


struct Internal_Cutting_Plane_Rendition : public Rendition_Base {
	friend class Rendition_Pointer<Internal_Cutting_Plane_Rendition>;

	public:
		Cutting_Plane_Set const	*	original_sets;			// grouped as in database
		Cutting_Plane_Set const	*	cutting_plane_sets;		// grouped for drawing in multiple passes
		Cutting_Plane_Set const	*	transparent_sets;		// "" but with no overlapping subvolumes
		int							max_cutting_plane_count;
		int							max_transparent_count;
		int							cutting_plane_ids;
		bool						all_drawn;
		bool						contains_dplanes;

	public:
		HOOPS_API Internal_Cutting_Plane_Rendition (Internal_Cutting_Plane_Rendition const & that);
		HOOPS_API ~Internal_Cutting_Plane_Rendition ();

	private:
		Internal_Cutting_Plane_Rendition (Display_Context const * dc);
		Internal_Cutting_Plane_Rendition const & operator= (Internal_Cutting_Plane_Rendition const & that);
};
typedef Rendition_Pointer<Internal_Cutting_Plane_Rendition>	Cutting_Plane_Rendition;




struct Convex_Clip_Region {
	Convex_Clip_Region *		next;

	DC_Point *					points;
	Cutting_Plane_Instance *	edges;
	int							count;

	Reference_Count				refcount;
};

struct Complex_Clip_Region {
	Complex_Clip_Region *		next;
	Convex_Clip_Region *		parts;

	DC_Point *					points;
	int *						lengths;
	int							loops;
	int							point_count;
	int							flags;
	Rectangle					bounds;

	Reference_Count				refcount;
};


typedef std::vector<Matrix, POOL_Allocator<Matrix> > Clip_Modelling_Vector_Pooled;

struct Internal_Clipping_Rendition : public Rendition_Base {
	friend class Rendition_Pointer<Internal_Clipping_Rendition>;

	public:
		Complex_Clip_Region const *		original_clips;		// clips/masks as in database
		Convex_Clip_Region const *		clipping_sets;		// reduced to convex regions
		Clip_Region_Vector_Pooled		clip_regions;		// oldest first
		Clip_Modelling_Vector_Pooled	clip_modelling;		// at the time each region is added
		int								max_cuts;			// maximum from any clipping_set
		bool							any_masks;			// in original_clips
		bool							any_non_window;		// cases that need patching on camera chang

	public:
		HOOPS_API Internal_Clipping_Rendition (Internal_Clipping_Rendition const & that);
		HOOPS_API ~Internal_Clipping_Rendition ();

	private:
		Internal_Clipping_Rendition (Display_Context const * dc);
		Internal_Clipping_Rendition const & operator= (Internal_Clipping_Rendition const & that);
};
typedef Rendition_Pointer<Internal_Clipping_Rendition>	Clipping_Rendition;











typedef int Clip_DC_Flags;

#define DCCF_NONE			(Clip_DC_Flags)0x00000000
#define DCCF_HARD_CLIP		(Clip_DC_Flags)0x00000001
#define DCCF_CLIP_REGION	(Clip_DC_Flags)0x00000002
#define DCCF_CUTTING_PLANES	(Clip_DC_Flags)0x00000004
#define DCCF_CRUDE_CLIP		(Clip_DC_Flags)0x00000008
#define DCCF_COLOR_VERTEX	(Clip_DC_Flags)0x00000010
#define DCCF_COLOR_EACH		(Clip_DC_Flags)0x00000020
#define DCCF_COLOR_SINGLE	(Clip_DC_Flags)0x00000040

#define DCCF_ALL			(DCCF_HARD_CLIP|DCCF_CLIP_REGION|DCCF_CUTTING_PLANES)
#define	DCCF_COLOR_MASK		(DCCF_COLOR_VERTEX|DCCF_COLOR_EACH|DCCF_COLOR_SINGLE)

#define CRUDE_CLIP_MARGIN	1024

struct User_Face_Pattern {
	unsigned char const *	pattern;
	int						height;
	int						width;
	bool					top_to_bottom;
	int						see_through;

	int						stipple;

	Reference_Count			refcount;
	Incarnation				incarnation;
};


/* This rendition only tracks what was actually drawn in the window above the segment
   being processed.  Window attributes are wound up and down into the ht_display_rendition->window */
struct Internal_Window_Rendition : public Rendition_Base {
	friend class Rendition_Pointer<Internal_Window_Rendition>;

	public:
		Segment_Cache *				segment_cache;
		Texture const *				window_texture;
		User_Face_Pattern const	*	user_pattern;

		Driver_Color				window_color;
		Driver_Color				contrast_color;
		RGB							window_diffuse; /* ungamma'd version */
		RGB							contrast_diffuse; /* ungamma'd version */

		int							window_pattern;
		bool						window_blending;

		Rectangle					extent;
		Rectangle					soft_clip;

		IntRectangle				net_extent;
		IntRectangle				transform_extent;
		IntRectangle				full_hard_clip;
		IntRectangle				frame_widths;
		IntRectangle				blend_extent;
		Camera_Handle				blend_skycam;

		float						aspect;

		Window_Frame_Flags			frame_flags;

		float						shadow_resolution;

	public:
		HOOPS_API Internal_Window_Rendition (Internal_Window_Rendition const & that);
		HOOPS_API ~Internal_Window_Rendition ();

	private:
		Internal_Window_Rendition (Display_Context const * dc);
		Internal_Window_Rendition const & operator= (Internal_Window_Rendition const & that);
};
typedef Rendition_Pointer<Internal_Window_Rendition>	Window_Rendition;



struct Internal_Material_Rendition : public Rendition_Base {
	friend class Rendition_Pointer<Internal_Material_Rendition>;

	public:
		Texture_Control_List *		diffuse_textures;
		Texture *					texture[Named_Material::TEXTURE_COUNT];
		RGB							diffuse_color;
		RGB							color[Named_Material::TEXTURE_COUNT];
		RGB							base_diffuse_color;
		RGB							base_color[Named_Material::TEXTURE_COUNT];

		float						opacity; /* 0.0=transparent, 1.0=opaque */
		float						gloss;
		float						index_of_refraction;

		Tinting						highlight;

		bool						has_textures;
		int							texture_rendition_incarnation;

	public:
		HOOPS_API Internal_Material_Rendition (Internal_Material_Rendition const & that);
		HOOPS_API ~Internal_Material_Rendition ();

	private:
		Internal_Material_Rendition (Display_Context const * dc);
		Internal_Material_Rendition const & operator= (Internal_Material_Rendition const & that);
};
typedef Rendition_Pointer<Internal_Material_Rendition>	Material_Rendition;



struct Internal_Color_Map_Rendition : public Rendition_Base {
	friend class Rendition_Pointer<Internal_Color_Map_Rendition>;

	public:
		Color_Map const *			gooch_color_map;		/* pointer to database */
		Driver_Color const *		gooch_driver_colors;	/* for non-MAPPED mode */
		Color_Index					gooch_color_map_length;

		Color_Map const *			color_map;		/* pointer to database */
		Driver_Color const *		driver_colors;	 /* for non-MAPPED mode */

		Color_Index					physical_map_offset; /* for MAPPED mode */
		Color_Index					length;

	public:
		HOOPS_API Internal_Color_Map_Rendition (Internal_Color_Map_Rendition const & that);
		HOOPS_API ~Internal_Color_Map_Rendition ();

	private:
		Internal_Color_Map_Rendition (Display_Context const * dc);
		Internal_Color_Map_Rendition const & operator= (Internal_Color_Map_Rendition const & that);
};
typedef Rendition_Pointer<Internal_Color_Map_Rendition>	Color_Map_Rendition;



struct Internal_Texture_Rendition : public Rendition_Base {
	friend class Rendition_Pointer<Internal_Texture_Rendition>;

	public:

		Texture_Defs_Vector			tdv;

	public:
		HOOPS_API Internal_Texture_Rendition (Internal_Texture_Rendition const & that);
		HOOPS_API ~Internal_Texture_Rendition ();

	private:
		Internal_Texture_Rendition (Display_Context const * dc);
};
typedef Rendition_Pointer<Internal_Texture_Rendition>	Texture_Rendition;



struct Internal_Line_Style_Rendition : public Rendition_Base {
	friend class Rendition_Pointer<Internal_Line_Style_Rendition>;

	public:
		Line_Style *				line_styles;
		Line_Style					solid_line_style;
		int							count;

	public:
		HOOPS_API Internal_Line_Style_Rendition (Internal_Line_Style_Rendition const & that);
		HOOPS_API ~Internal_Line_Style_Rendition ();

	private:
		Internal_Line_Style_Rendition (Display_Context const * dc);
		Internal_Line_Style_Rendition const & operator= (Internal_Line_Style_Rendition const & that);
};
typedef Rendition_Pointer<Internal_Line_Style_Rendition>	Line_Style_Rendition;



struct Internal_Glyph_Rendition : public Rendition_Base {
	friend class Rendition_Pointer<Internal_Glyph_Rendition>;

	public:
		Glyph *						glyphs;
		int							count;

	public:
		HOOPS_API Internal_Glyph_Rendition (Internal_Glyph_Rendition const & that);
		HOOPS_API ~Internal_Glyph_Rendition ();

	private:
		Internal_Glyph_Rendition (Display_Context const * dc);
		Internal_Glyph_Rendition const & operator= (Internal_Glyph_Rendition const & that);
};
typedef Rendition_Pointer<Internal_Glyph_Rendition>	Glyph_Rendition;


struct Internal_Named_Style_Rendition : public Rendition_Base {
	friend class Rendition_Pointer<Internal_Named_Style_Rendition>;

public:

	Named_Style_Vector_Pooled			named_styles_vector;

private:
	Internal_Named_Style_Rendition (Display_Context const * dc);
};
typedef Rendition_Pointer<Internal_Named_Style_Rendition>	Named_Style_Rendition;



struct Internal_Misc_Rendition : public Rendition_Base {
	friend class Rendition_Pointer<Internal_Misc_Rendition>;

	public:
		Color_Map_Rendition		color_map_rendition;

		Texture_Matrix const *	texture_matrix;

		Type_Flags				base_visibility;
		Type_Flags				shadow_visibility;
		Type_Flags				cut_geometry_visibility;

		Rendo_Flags				complex_options;
		Rendo_Flags				simple_options;
		Rendo_Flags				interpolation_options;
		Rendo_Debug				rendering_debug; /* see database.h for definitions */
		Quantization_Flags		quantization;
		Technology_Flags		technology;
		HSR_Algorithm			hsr_algorithm;


		Attribute_Lock			locks;			// active locks
		Attribute_Lock			parent_locks;	// forced locks (child cannot change)

		struct Transparency {
			int						options;
			int						style;
			HSR_Algorithm			hsr_algorithm;
			int						peeling_layers;
			float					peeling_min_area;
			bool					blending_active;
			bool					zsort_fast;
			bool					use_buffers;
		}						transparency;

		Vector					ambient_up_vector;

		RGB						ambient_up;
		RGB						ambient_up_base;
		RGB						ambient_down;
		RGB						ambient_down_base;

		RGB						lighting;

		/* only used during Standard_Select... */
		Type_Flags				down, still_down, up, still_up;
		Type_Flags				selectability;
		Type_Flags				restricted_selectability;
		Type_Flags				even_invisible;

		int						related_selection_limit;

		struct ISL {
			int					shell;
			int					polyline;
		}		internal_selection_limit;

		int						selection_level;

		int						pixel_threshold;
		bool					use_octree;
		int						maximum_extent;
		int						maximum_extent_mode;
		int						maximum_extent_level;
		int						hard_extent;
		int						vertex_threshold;

		float					max_distance;

		Point					volume_min;
		Point					volume_max;

		Vector					vector;
		float					vector_tolerance;

		int						force_defer_batch;
		float					force_defer_bias;

		int						model_type;

		int						display_lists;
//		bool					display_list_tree_here;

		bool					mask_color_image;

		short					antialias_options;

		INLINE void Override_Ambient_Up_Color(RGB const & rgb, bool base_too);
		INLINE void Override_Ambient_Down_Color(RGB const & rgb, bool base_too);

	public:
		//HOOPS_API Internal_Misc_Rendition (Internal_Misc_Rendition const & that);
		//HOOPS_API ~Internal_Misc_Rendition ();

	private:
		Internal_Misc_Rendition (Display_Context const * dc);
};
typedef Rendition_Pointer<Internal_Misc_Rendition>	Misc_Rendition;
typedef	Internal_Misc_Rendition::Transparency	MR_Transparency;

struct Internal_Display_Rendition : public Rendition_Base {
	friend class Rendition_Pointer<Internal_Display_Rendition>;

	public:
		
		struct Buffer {
			Buffer_Flags				options;
			int							size_limit;
			Color_Depth					color_depth;
		}	buffer;

		struct HL {
			Hidden_Line_Flags			options;
			RGB							color;
			Line_Style					line_style;
			float						dim_factor;
			float						face_displacement;
			float						transparency_cutoff;
			Generic_Size				weight;
			HSR_Algorithm				hsr_algorithm;
		}	hidden_line;

		struct Window {					/* effective window values, held here until we actually get a window */
			Driver_Color				color;
			Driver_Color				contrast;
			Driver_Color				base_color;
			Driver_Color				base_contrast;
			RGB							diffuse; /* ungamma'd version */
			RGB							contrast_diffuse; /* ungamma'd version */

			Texture const *				texture;
			User_Face_Pattern const *	user_pattern;
			int							pattern;
			bool						blending;
			Window_Frame_Flags			frame_flags;
		}	window;

		Conditional_Action *			conditional_actions;
		Condition_Bits					condition_bits;

		INLINE void Override_Window_Color(RGB const & rgb, bool base_too=true);
		INLINE void Override_Window_Contrast_Color(RGB const & rgb, bool base_too=true);

	public:
		HOOPS_API Internal_Display_Rendition (Internal_Display_Rendition const & that);
		HOOPS_API ~Internal_Display_Rendition ();

	private:
		Internal_Display_Rendition (Display_Context const * dc);
		Internal_Display_Rendition const & operator= (Internal_Display_Rendition const & that);
};
typedef Rendition_Pointer<Internal_Display_Rendition>	Display_Rendition;
typedef	Internal_Display_Rendition::HL					DR_HL;
typedef	Internal_Display_Rendition::Window				DR_Window;
typedef	Internal_Display_Rendition::Buffer				DR_Buffer;


struct Internal_Geometry_Rendition : public Rendition_Base {
	friend class Rendition_Pointer<Internal_Geometry_Rendition>;

	public:
		struct LOD {
			Bounding				bounding;
			LOD_Flags				options;
			float					ratio[LOD_MAX_LEVELS]; /* compression ratio */
			float					threshold[LOD_MAX_LEVELS]; /* switch threshold */
			int						min_triangle_count;
			int						clamp;			/* clamp to level.	-1 if disabled */
			int						num_levels;
			int						max_degree;
			float					tolerance;	
			float					cutoff[LOD_MAX_LEVELS]; /* calculation cutoff */
			int						heuristic; /* usefulness heuristic (view independent) */
			int						fallback; /* fallback in case we request a non-existent lod */
			int						algorithm;
		} lod;

		NURBS_Curve_Control			nurbs_curve;

		NURBS_Surface_Config		nurbs_surface_config;

		struct T {
			int						cylinder[LOD_MAX_LEVELS];
			int						sphere[LOD_MAX_LEVELS];
		} tessellation;

		struct I {
			float					width_scale;
			float					height_scale;
			RGB						tint_color;
			RGB						diffuse_texture_tint_color;
		} image;

		Tinting						diffuse_tint;

		int							geometry_options;
		float						hard_edge_dihedral;

		struct Contour {
			Type_Flags				visibility;
			int						adjustment;
			float					scale;
			float					translate;
		} contour;

		struct ISO {
			float *					positions;
			RGB *					colors;
			Name *					pattern_names;
			Generic_Size *			weights;

			int						position_type;

			int						position_count;
			int						color_count;
			int						pattern_count;
			int						weight_count;
		
			bool					lighting;
		} isoline;

	public:
		HOOPS_API Internal_Geometry_Rendition (Internal_Geometry_Rendition const & that);
		HOOPS_API ~Internal_Geometry_Rendition ();

	private:
		Internal_Geometry_Rendition (Display_Context const * dc);
};
typedef Rendition_Pointer<Internal_Geometry_Rendition>	Geometry_Rendition;
typedef	Internal_Geometry_Rendition::ISO				GR_ISO;
typedef	Internal_Geometry_Rendition::LOD				GR_LOD;
typedef	Internal_Geometry_Rendition::Contour			GR_Contour;


struct Internal_Visualization_Rendition : public Rendition_Base {
	friend class Rendition_Pointer<Internal_Visualization_Rendition>;

	public:
		struct SS {
			Plane					plane;
			Vector					light;
			RGBAS32					color;
			float					opacity;
			int						resolution;
			int						blurring;
			bool					ignore_transparency;
			bool					fast_bounding;
		}						simple_shadow;

		struct SM {
			int						resolution;
			int						samples;
			bool					jitter;
			bool					view_dependent;
		}						shadow_map;

		struct SR {
			Plane					plane;
			Type_Flags				visibility;
			float					opacity;
			float					hither, yon;
			int						blurring;
			bool					attenuation; /* by distance */
			bool					fading; /* by angle */
		}						simple_reflection;

		struct CG {
			int						level;
			float					tolerance;
			float					base_tolerance;
			bool					tolerance_is_percent;
			int						match_color;
		}							cut_geometry;

		struct {
			float					hither;
			float					yon;
		}						fog;

		struct Gooch {
			float					color_range[2];
			float					diffuse_weight;
		}						gooch;

		float					ordered_weights[Heur_Order_COUNT];
		float					defer_bias;
		int						defer_range_min;
		int						defer_range_max;

	private:
		Internal_Visualization_Rendition (Display_Context const * dc);
};
typedef Rendition_Pointer<Internal_Visualization_Rendition>	Visualization_Rendition;
typedef	Internal_Visualization_Rendition::SR			VR_SR;
typedef	Internal_Visualization_Rendition::Gooch			VR_Gooch;
typedef	Internal_Visualization_Rendition::CG			VR_CG;
typedef	Internal_Visualization_Rendition::SS			VR_SS;
typedef	Internal_Visualization_Rendition::SM			VR_SM;


template <typename F>
struct Light_Instance_Data {
	Point_3D<F>				position;
	Point_3D<F>				target;
	Point_3D<F> *			points;
};
typedef Light_Instance_Data<float> Light_Instance_Single;
typedef Light_Instance_Data<double> Light_Instance_Double;

class Light_Instance : public CMO_Pooled {

private:
	Light_Instance const & operator= (Light_Instance const & that);

public:
	Light_Instance *		next;
	Memory_Pool *			memory_pool;
	Light_Instance_Single *	single_data;
	Light_Instance_Double *	double_data;

	Light_Instance (Memory_Pool *p)
		: next (null)
		, memory_pool(p)
		, single_data (null)
		, double_data (null)
		, shadow_map (null)
		, lightkey (0)
		, color (0,0,0)
		, ambient_up (0,0,0)
		, direction (0,0,0)
		, halfway (0,0,0)
		, count (0)
		, bright_side (0)
		, inner (0)
		, outer (0)
		, inner_cos (0)
		, outer_cos (0)
		, concentration (0)
		, area (0)
		, generation (0)
		, visibility (T_NONE)
		, shadow_visibility (T_NONE)
		, camera_relative (false)
		, light_type (BAD_TYPE)
	{}

	Light_Instance(Light_Instance const & that) :
		next(null),
		memory_pool(that.memory_pool),
		single_data(null),
		double_data(null),
		shadow_map(null),
		lightkey(that.lightkey),
		color(that.color),
		ambient_up(that.ambient_up),
		direction(that.direction),
		halfway(that.halfway),
		count(that.count),
		bright_side(that.bright_side),
		inner(that.inner),
		outer(that.outer),
		inner_cos(that.inner_cos),
		outer_cos(that.outer_cos),
		concentration(that.concentration),
		area(that.area),
		generation(that.generation),
		visibility(that.visibility),
		shadow_visibility(that.shadow_visibility),
		camera_relative(that.camera_relative),
		light_type(that.light_type)
	{
		if (that.single_data != null) {
			POOL_CLONE_ARRAY(that.single_data, 1, Light_Instance_Single, single_data, memory_pool);
			POOL_CLONE_ARRAY(that.single_data->points, that.count, Point, single_data->points, memory_pool);
		}
		if (that.double_data != null) {
			POOL_CLONE_ARRAY(that.double_data, 1, Light_Instance_Double, double_data, memory_pool);
			POOL_CLONE_ARRAY(that.double_data->points, that.count, DPoint, double_data->points, memory_pool);
		}
	}

	~Light_Instance() {
		if (single_data != null) {
			if (count > 0)
				FREE_ARRAY(single_data->points, count, Point);
			FREE(single_data, Light_Instance_Single);
		}
		if (double_data != null) {
			if (count > 0)
				FREE_ARRAY(double_data->points, count, DPoint);
			FREE(double_data, Light_Instance_Double);
		}
	}

	template <typename F>
	INLINE Light_Instance_Data<F> * Create_Light_Instance_Data() {}

	template <typename F>
	INLINE Light_Instance_Data<F> * Get_Light_Instance_Data() {}

	Shadow_Map const *		shadow_map;
	Key						lightkey;
	RGB						color;
	RGB						ambient_up;
	RGB						ambient_down;
	Vector					direction;
	Vector					halfway;
	int						count;
	int						bright_side;
	float					inner;
	float					outer;
	float					inner_cos;
	float					outer_cos;
	float					concentration;
	float					area;
	uint32_t				generation;
	Type_Flags				visibility;
	Type_Flags				shadow_visibility;
	bool					camera_relative;
	DB_Type					light_type;
};

template <>
INLINE Light_Instance_Single * Light_Instance::Create_Light_Instance_Data<float>() {
	POOL_ZALLOC(single_data, Light_Instance_Single, memory_pool);
	return single_data;
}
template <>
INLINE Light_Instance_Double * Light_Instance::Create_Light_Instance_Data<double>() {
	POOL_ZALLOC(double_data, Light_Instance_Double, memory_pool);
	return double_data;
}

template <> INLINE Light_Instance_Single * Light_Instance::Get_Light_Instance_Data<float>() { return single_data; }
template <> INLINE Light_Instance_Double * Light_Instance::Get_Light_Instance_Data<double>() { return double_data; }

struct Internal_Light_Rendition : public Rendition_Base {
	friend class Rendition_Pointer<Internal_Light_Rendition>;

	public:
		Light_Instance *		lights;
		float					scale;
		int						count;
		bool					compute_light_requires_xyz;

	public:
		HOOPS_API ~Internal_Light_Rendition ();

	private:
		Internal_Light_Rendition (Internal_Light_Rendition const & that);
		Internal_Light_Rendition (Display_Context const * dc);
		Internal_Light_Rendition const & operator= (Internal_Light_Rendition const & that);
};
typedef Rendition_Pointer<Internal_Light_Rendition>	Light_Rendition;


enum TR_Flags : int32_t	{

	TR_NONE							= 0x00000000,

	TR_HARDWARE_LIGHTING			= 0x00000001,
	TR_LIGHTING						= 0x00000002,
	TR_MODELLING_IS_3D				= 0x00000004,
	TR_RIGHT_HANDED_WORLD			= 0x00000008,

	TR_MATRIX_INCLUDES_SCREEN		= 0x00000010,
	TR_OVERSIZED_WINDOW				= 0x00000020,
	TR_CRUDE_CLIP_DONE				= 0x00000040,
	TR_PLEASE_CULL					= 0x00000080,

	TR_HARDWARE_CUTTING_PLANES		= 0x00000100,
	TR_CUTTING_PLANES				= 0x00000200,
	TR_PLEASE_SET_DC_CUT_PLANES		= 0x00000400,
	TR_CLIPPING_NOT_NEEDED			= 0x00000800,

	TR_SORT_HIDDEN_BY_YMIN			= 0x00001000,
	TR_SORT_HIDDEN_BY_PRIORITY		= 0x00002000,
	TR_SORT_HIDDEN_MASK = (TR_SORT_HIDDEN_BY_YMIN|TR_SORT_HIDDEN_BY_PRIORITY),
	TR_CLIP_IS_NONNULL				= 0x00004000,
	TR_CLIP_IS_FULL					= 0x00008000,

	TR_SCALED_DISPLACEMENT			= 0x00010000,
	TR_ZSORT_BACKFACE				= 0x00020000,

	TR_SEG_CULLING_ALREADY_DONE		= 0x00100000,
	TR_REF_CULLING_ALREADY_DONE		= 0x00200000,
	TR_GEO_CULLING_ALREADY_DONE		= 0x00400000,

	TR_Z_BUFFERING					= 0x01000000,
	TR_DISABLE_Z_WRITES				= 0x02000000,
	TR_REVERSE_DEPTH_TEST			= 0x04000000,
	TR_ENABLE_ALPHA_TEST			= 0x08000000,
};

template<> struct _bit_ops_type < TR_Flags > {
	typedef TR_Flags type;
};


#	define		STEREO_OFF		0
#	define		STEREO_LEFT		1
#	define		STEREO_RIGHT	2


/* Transform-and-clip data structures and information */

typedef unsigned char	Clip_Case;
#define CC_WITHIN				(Clip_Case)0x00
#define CC_OFF_TO_LEFT			(Clip_Case)0x01		/* must be in order! */
#define CC_OFF_TO_TOP			(Clip_Case)0x02		/* (corner-walking	 */
#define CC_OFF_TO_RIGHT			(Clip_Case)0x04		/*	depends on it.)	 */
#define CC_OFF_TO_BOTTOM		(Clip_Case)0x08
#define CC_BEHIND_THE_EYE		(Clip_Case)0x10
#define	CC_CUTTING_PLANE		(Clip_Case)0x20


#	define		TType_NOT_AT_ALL		1		/* No transform */
#	define		TType_XY_To_XY			2		/* XY in --> XY out */
#	define		TType_XYZ_To_XY			3		/* XYZ in --> XY out */
#	define		TType_XYZ_To_XYZ		0		/* XYZ in --> XYZ out */
#	define		TType_XYZ_To_XY_W		5		/* XYZ in --> XY/W out */
#	define		TType_XYZ_To_XYZ_W		4		/* XYZ in --> XYZ/W out */

#	define		Z_TRANSFORM(x)	(!ANYBIT(x, 3))
#	define		W_TRANSFORM(x)	((x) >= 4)


struct Modelling_History {
	Modelling_History(Matrix_Overrides overrides) : matrix(null), overrides(overrides) {}
	Modelling_History(Matrix matrix) : matrix(matrix), overrides(Matrix_Contents_NONE) {}

	Matrix				matrix;
	Matrix_Overrides	overrides;
};
typedef std::vector<Modelling_History, POOL_Allocator<Modelling_History> > Modelling_History_Vector;



struct Internal_Transform_Rendition  : public Rendition_Base {
	friend class Rendition_Pointer<Internal_Transform_Rendition>;

	public:
		Camera_Handle				camera;
		Camera_Handle				db_camera;
		Matrix						matrix;
		Matrix						net_modelling_matrix;

		Matrix						world_to_screen_matrix;
		Matrix						object_to_view_matrix;
		Matrix						object_to_projection_matrix;

		Modelling_History_Vector	modelling_history;

		Cutting_Plane_Rendition		cutting_plane_rendition;
		Clipping_Rendition			clipping_rendition;

		int							transform_type;

		TR_Flags					flags;

		IntRectangle				hard_clip;
		IntRectangle				unframed_hard_clip;

		float						wlimit;
		float						wlimit_inverse;
		float						wlimit_software_offset_scale;
		float						z_max;
		float						z_range[2];
		Rectangle					screen_range;

		float						effective_window_aspect;
		float						effective_field_width_scale;
		float						effective_field_height_scale;
		float						effective_min_secant;

		float						face_displacement;
		float						db_face_displacement;
		float						vertex_displacement;
		float						db_vertex_displacement;
		float						general_displacement;
		float						db_general_displacement;
		DPoint2D					pixel_scale;
		DPoint2D					inverse_pixel_scale;

		Heuristics_Flags			heuristics;

		short						culling;
		short						selection_culling;

		float						stereo_separation;
		float						stereo_distance;

		char						stereo_state;
		char						stereo_pending_state;

		bool	Is_Perspective() const {
			return W_TRANSFORM (transform_type);
			//ASSERT(W_TRANSFORM (transform_type) == matrix->Is_Perspective());
			//return matrix->Is_Perspective();
		}

		bool TransformClamped(Point const & in_point, Point & out_point) const {
			float	w;

			out_point = matrix->Transform(in_point, w);

			if (w <= wlimit)
				return false;

			if (Z_TRANSFORM (transform_type)) {
				if (ANYBIT(flags, TR_Z_BUFFERING)) {
					if (out_point.z < 0.0f)
						out_point.z = 0.0f;
					else if (out_point.z > z_max) 
						out_point.z = z_max;
				}
			}
			else
				out_point.z = 0.0f;

			return true;
		}

		bool TransformClamped(DPoint const & in_point, Point & out_point) const {
#ifndef DISABLE_DOUBLE_PRECISION
			float	w;

			out_point = Point(matrix->Transform(in_point, w));

			if (w <= wlimit)
				return false;

			if (Z_TRANSFORM (transform_type)) {
				if (ANYBIT(flags, TR_Z_BUFFERING)) {
					if (out_point.z < 0.0)
						out_point.z = 0.0;
					else if (out_point.z > z_max) 
						out_point.z = z_max;
				}
			}
			else
				out_point.z = 0.0;

			return true;
#else
			return false;
#endif
		}

	public:
		HOOPS_API Internal_Transform_Rendition (Internal_Transform_Rendition const & that);
		HOOPS_API ~Internal_Transform_Rendition ();

	private:
		Internal_Transform_Rendition (Display_Context const * dc);
		Internal_Transform_Rendition const & operator= (Internal_Transform_Rendition const & that);
};
typedef Rendition_Pointer<Internal_Transform_Rendition>	Transform_Rendition;


struct Internal_Face_Rendition  : public Rendition_Base {
	friend class Rendition_Pointer<Internal_Face_Rendition>;

	public:
		Material_Rendition			material_rendition;
		Material_Rendition			back_material_rendition;
		Light_Rendition				light_rendition;
		User_Face_Pattern const	*	user_pattern;

		Driver_Color				color;
		Driver_Color				contrast_color;
		Driver_Color				back_color;
		Driver_Color				back_contrast_color;
		Driver_Color				base_color;
		Driver_Color				base_contrast_color;
		Driver_Color				base_back_color;
		Driver_Color				base_back_contrast_color;

		int							pattern;
		bool						drawing_wide_dc_lines;

		INLINE void Override_Color(RGB const & rgb, bool base_too=true);
		INLINE void Override_Contrast_Color(RGB const & rgb, bool base_too=true);

		INLINE void Override_Back_Color(RGB const & rgb, bool base_too=true);
		INLINE void Override_Back_Contrast_Color(RGB const & rgb, bool base_too=true);


	public:
		HOOPS_API Internal_Face_Rendition (Internal_Face_Rendition const & that);
		HOOPS_API ~Internal_Face_Rendition ();

	private:
		Internal_Face_Rendition (Display_Context const * dc);
		Internal_Face_Rendition const & operator= (Internal_Face_Rendition const & that);
};
typedef Rendition_Pointer<Internal_Face_Rendition>	Face_Rendition;



#define MAX_LINE_WEIGHT	(MAX_INT>>4)

typedef int Line_Rendition_Flags;

#define LRF_NONE						(Line_Rendition_Flags) 0x00000000
#define LRF_SUPPRESS_START_GLYPH		(Line_Rendition_Flags) 0x00000001
#define LRF_SUPPRESS_END_GLYPH			(Line_Rendition_Flags) 0x00000002
#define LRF_SUPPRESS_MIDDLE_GLYPH		(Line_Rendition_Flags) 0x00000004
#define LRF_FORCE_ABSOLUTE_WEIGHT		(Line_Rendition_Flags) 0x00000010
#define LRF_END_GLYPH_IS_ARROW			(Line_Rendition_Flags) 0x00000020
#define LRF_ANY_GLYPHS					(Line_Rendition_Flags) 0x00000040

#define LRF_SUPPRESS_CAP_GLYPHS (LRF_SUPPRESS_START_GLYPH|LRF_SUPPRESS_END_GLYPH)
#define LRF_SUPPRESS_GLYPHS		(LRF_SUPPRESS_CAP_GLYPHS|LRF_SUPPRESS_MIDDLE_GLYPH)


struct Internal_Line_Rendition;
typedef Rendition_Pointer<Internal_Line_Rendition>	Line_Rendition;

struct Internal_Line_Rendition  : public Rendition_Base {
	friend class Rendition_Pointer<Internal_Line_Rendition>;

	public:
		Material_Rendition			material_rendition;
		Light_Rendition				light_rendition;
		Line_Style					line_style;

		Glyph						start_glyph;
		Glyph						middle_glyph;
		Glyph						end_glyph;

		Face_Rendition				face_rendition;
		Line_Rendition				solid_rendition;
		Line_Rendition				contrast_rendition;

		Driver_Color				color;
		Driver_Color				contrast_color;
		Driver_Color				base_color;
		Driver_Color				base_contrast_color;

		Line_Rendition_Flags		flags;
		int							style;
		float						weight;
		float						join_cutoff_angle;
		float						dbsize;
		Generic_Size_Units			units;

		INLINE void Override_Color(RGB const & rgb, bool base_too=true);
		INLINE void Override_Contrast_Color(RGB const & rgb, bool base_too=true);

	public:
		//HOOPS_API Internal_Line_Rendition (Internal_Line_Rendition const & that);
		//HOOPS_API ~Internal_Line_Rendition ();

		void apply_weight (Generic_Size const & size);


	private:
		Internal_Line_Rendition (Display_Context const * dc);
};



typedef unsigned char Marker_Rendition_Flags;
#define MRF_NONE			(Marker_Rendition_Flags) 0x00000000
#define MRF_MIRROR			(Marker_Rendition_Flags) 0x00000001
#define MRF_TRANSFORMED		(Marker_Rendition_Flags) 0x00000002
#define MRF_WEIGHTED		(Marker_Rendition_Flags) 0x00000004


struct Internal_Marker_Rendition  : public Rendition_Base {
	friend class Rendition_Pointer<Internal_Marker_Rendition>;

	public:
		Material_Rendition			material_rendition;
		Light_Rendition				light_rendition;
		Line_Rendition				solid_rendition;
		Line_Rendition				contrast_rendition;

		Glyph						glyph;

		Driver_Color				color;
		Driver_Color				contrast_color;
		Driver_Color				base_color;
		Driver_Color				base_contrast_color;

		float						xsize;
		float						ysize;
		float						dbsize;
		float						vertex_decimation;
		Generic_Size_Units			units;
		Generic_Size				stroke_weight;
		Marker_Rendition_Flags		flags;

		INLINE void Override_Color(RGB const & rgb, bool base_too=true);
		INLINE void Override_Contrast_Color(RGB const & rgb, bool base_too=true);

	public:
		//HOOPS_API Internal_Marker_Rendition (Internal_Marker_Rendition const & that);
		//HOOPS_API ~Internal_Marker_Rendition ();

	private:
		Internal_Marker_Rendition (Display_Context const * dc);
};
typedef Rendition_Pointer<Internal_Marker_Rendition>	Marker_Rendition;



struct Internal_Text_Rendition : public Rendition_Base {
	friend class Rendition_Pointer<Internal_Text_Rendition>;

	public:
		mutable	Font_Instance const *	font;			/* driver land font */
		Material_Rendition				material_rendition;
		Line_Rendition					stroke_rendition; /* null if not needed */
		Font_Name_List 					names;

		Driver_Color					color;
		Driver_Color					contrast_color;
		Driver_Color					base_color;
		Driver_Color					base_contrast_color;

		int								alignment;
		int								justification;
		float							spacing;
		Vector							path;

		mutable	int						fallback_index;
		bool							font_stroked_fallback;

		/* the following are raw, copied from the database... */
		float							dbsize;/* (db = database = prevent confusion) */
		float							size_tolerance;
		float							extra_space;
		float							greeking_limit;
		float							rotation;
		float							width_scale;
		float							line_spacing;
		float							slant;
		float							renderer_cutoff;
		float							preference_cutoff;
		float							effective_size_tolerance;
		bool							rotation_follow_path;
		bool							even_spacing;
		bool							uniform_spacing;
		bool							outline;
		bool							underline;
		bool							strikethrough;
		bool							fill_edges;
		bool							overline;
		bool							bold;
		bool							italic;
		bool							skip_trim;	/* special handling for things like gather_screen_extent */

		int								font_index; /* used for CGM font table */

		Font_Transform					transform;
		Font_Renderer					renderers[2];
		Font_Preference					preferences[2];
		Font_Greeking_Mode				greeking_mode;

		Font_Layout						layout;

		Font_Size_Units					size_units;
		Font_Size_Units					size_tolerance_units;
		Font_Size_Units					extra_space_units;
		Font_Size_Units					greeking_limit_units;
		Font_Size_Units					renderer_cutoff_units;
		Font_Size_Units					preference_cutoff_units;

		INLINE void Override_Color(RGB const & rgb, bool base_too=true);
		INLINE void Override_Contrast_Color(RGB const & rgb, bool base_too=true);

	public:
		HOOPS_API Internal_Text_Rendition (Internal_Text_Rendition const & that);
		HOOPS_API ~Internal_Text_Rendition ();

		void Invalidate () const { font = null; fallback_index = 0; }

	private:
		Internal_Text_Rendition (Display_Context const * dc);
		Internal_Text_Rendition const & operator= (Internal_Text_Rendition const & that);
};
typedef Rendition_Pointer<Internal_Text_Rendition>	Text_Rendition;


struct Internal_User_Rendition : public Rendition_Base {
	friend class Rendition_Pointer<Internal_User_Rendition>;

	public:
		User_Options_Vector_Pooled	user_options; // oldest first

		void * Lookup(uint32_t index) const;

	public:
		HOOPS_API Internal_User_Rendition (Internal_User_Rendition const & that);
		HOOPS_API ~Internal_User_Rendition ();

	private:
		Internal_User_Rendition (Display_Context const * dc);
};
typedef Rendition_Pointer<Internal_User_Rendition>	User_Rendition;


struct Segment_Info {
	Segment_Info *		next;
	Segment const *		segment;
	Include_Path		path;
	int					save_depth;
	bool				force_erase;
	bool				is_copy;
};


typedef int Text_Info_Flags;
#define TIF_NONE			(Text_Info_Flags) 0x00000000
#define TIF_ONE_AT_A_TIME	(Text_Info_Flags) 0x00000001
#define TIF_MISSING_CHARS	(Text_Info_Flags) 0x00000002
#define TIF_USING_BUILTIN	(Text_Info_Flags) 0x00000004


#define TKIF_NONE				(Text_Info_Flags) 0x00000000
#define TKIF_OMITTED			(Text_Info_Flags) 0x00000001
#define TKIF_INVISIBLE			(Text_Info_Flags) 0x00000002
#define TKIF_MISSING			(Text_Info_Flags) 0x00000004
#define TKIF_VERTICAL_OFFSET	(Text_Info_Flags) 0x00000010
#define TKIF_HORIZONTAL_OFFSET	(Text_Info_Flags) 0x00000020
#define TKIF_COLOR				(Text_Info_Flags) 0x00000040

struct Text_Karacter_Info {
	Net_Rendition			nr;

	Point					position;
	Point					point;
	float					width;
	float					height;

	struct {
		float					width;
		float					height;
		float					x;
		float					y;
	}						string,	/* relative to basic path (before region) */
							screen;	/* relative to screen (before region) */

	RGB						color;
	float					slant_extra;
	float					vertical_offset;
	float					horizontal_offset;
	float					distance;

	Text_Info_Flags			flags;
	int						offset;
};

struct Text_Info {
	Text const *			text;
	Text_Karacter_Info	*	kinfo;
	int *					kindex;
	int						kinfo_count;
	bool *					left_justifications;

	KName *					local_kname;
	Text_Karacter_Info	**	local_kinfo;

	KName				kname;

	Karacter *			trimmed_kp;
	int					trimmed_kp_count;

	int					local_lines;

	Point				position;
	Point				reference;
	Vector				ref_adjust;
	Point				db_point;
	Vector				position_offset; /* used with I.M. */
	Vector				path;			/* final path of overall text */
	Vector				cpath;			/* baseline path of each character */
	Vector				contrib;
	Vector				scale;
	Vector				next_line;		/* jump to successive lines */

	/* these few only needed by I.M. but placed before the rest for alignment */
	Vector				baseline;
	Vector				longest_baseline;
	Vector				vertical;
	Point				special;
	int					which;
	int					line_index;

	float				total_distance;
	float				extra_space;
	float				greeking_limit;

	/* largest from all characters */
	float				max_vspace;
	float				max_size;

	int					omitted_count;

	Text_Info_Flags		flags;
};

enum Action_Block_ID {
	Action_Block_ID_Segment,		Action_Block_ID_FIRST = Action_Block_ID_Segment,
	Action_Block_ID_DC,
	Action_Block_ID_3D,
	Action_Block_ID_Driver,
	Action_Block_ID_Select,
	Action_Block_ID_HLR,			Action_Block_ID_LAST = Action_Block_ID_HLR,
};

#define ACTION_CALL HC_CDECL
template <Callback_Type index> struct Action_Call_Info {};


typedef void (ACTION_CALL * IM_Populate_Segment_Action)(Net_Rendition const * nr, Key key);
template <> struct Action_Call_Info<CBT_POPULATE_SEGMENT> {
	typedef IM_Populate_Segment_Action im_type;
	static const Action_Block_ID		action_block_id = Action_Block_ID_Segment;
	static const size_t					action_offset = CBT_POPULATE_SEGMENT - CBT_Block_Start_SEGMENT;
};

typedef void (ACTION_CALL * IM_Process_Segment_Action)(Net_Rendition const * nr);
template <> struct Action_Call_Info<CBT_PROCESS_SEGMENT> {
	typedef IM_Process_Segment_Action im_type;
	static const Action_Block_ID		action_block_id = Action_Block_ID_Segment;
	static const size_t					action_offset = CBT_PROCESS_SEGMENT - CBT_Block_Start_SEGMENT;
};

typedef void (ACTION_CALL * IM_Draw_Segment_Action)(Net_Rendition const * nr, Segment_Info const *info);
template <> struct Action_Call_Info<CBT_DRAW_SEGMENT> {
	typedef IM_Draw_Segment_Action im_type;
	static const Action_Block_ID		action_block_id = Action_Block_ID_Segment;
	static const size_t					action_offset = CBT_DRAW_SEGMENT - CBT_Block_Start_SEGMENT;
};
template <> struct Action_Call_Info<CBT_SELECT_SEGMENT> {
	typedef IM_Draw_Segment_Action im_type;
	static const Action_Block_ID		action_block_id = Action_Block_ID_Select;
	static const size_t					action_offset = CBT_SELECT_SEGMENT - CBT_Block_Start_Select;
};

typedef void (ACTION_CALL * IM_Draw_Segment_Tree_Action)(Net_Rendition const * nr, Segment_Info const *info);
template <> struct Action_Call_Info<CBT_DRAW_SEGMENT_TREE> {
	typedef IM_Draw_Segment_Tree_Action im_type;
	static const Action_Block_ID		action_block_id = Action_Block_ID_Segment;
	static const size_t					action_offset = CBT_DRAW_SEGMENT_TREE - CBT_Block_Start_SEGMENT;
};
template <> struct Action_Call_Info<CBT_SELECT_SEGMENT_TREE> {
	typedef IM_Draw_Segment_Tree_Action im_type;
	static const Action_Block_ID		action_block_id = Action_Block_ID_Select;
	static const size_t					action_offset = CBT_SELECT_SEGMENT_TREE - CBT_Block_Start_Select;
};



typedef void (ACTION_CALL * IM_Draw_DC_Polyline_Action)(Net_Rendition const * nr, int count, DC_Point const *points);
template <> struct Action_Call_Info<CBT_DRAW_DC_EDGE> {
	typedef IM_Draw_DC_Polyline_Action im_type;
	static const Action_Block_ID		action_block_id = Action_Block_ID_DC;
	static const size_t					action_offset = CBT_DRAW_DC_EDGE - CBT_Block_Start_DC;
};
template <> struct Action_Call_Info<CBT_DRAW_DC_POLYLINE> {
	typedef IM_Draw_DC_Polyline_Action im_type;
	static const Action_Block_ID		action_block_id = Action_Block_ID_DC;
	static const size_t					action_offset = CBT_DRAW_DC_POLYLINE - CBT_Block_Start_DC;
};
template <> struct Action_Call_Info<CBT_DRAW_DC_CUT_LINE> {
	typedef IM_Draw_DC_Polyline_Action im_type;
	static const Action_Block_ID		action_block_id = Action_Block_ID_DC;
	static const size_t					action_offset = CBT_DRAW_DC_CUT_LINE - CBT_Block_Start_DC;
};
template <> struct Action_Call_Info<CBT_HLR_POLYLINE> {
	typedef IM_Draw_DC_Polyline_Action im_type;
	static const Action_Block_ID		action_block_id = Action_Block_ID_HLR;
	static const size_t					action_offset = CBT_HLR_POLYLINE - CBT_Block_Start_HLR;
};
template <> struct Action_Call_Info<CBT_HLR_HIDDEN_POLYLINE> {
	typedef IM_Draw_DC_Polyline_Action im_type;
	static const Action_Block_ID		action_block_id = Action_Block_ID_HLR;
	static const size_t					action_offset = CBT_HLR_HIDDEN_POLYLINE - CBT_Block_Start_HLR;
};

template <> struct Action_Call_Info<CBT_DRAW_DC_TEXT_STROKES> {
	typedef IM_Draw_DC_Polyline_Action im_type;
	static const Action_Block_ID		action_block_id = Action_Block_ID_DC;
	static const size_t					action_offset = CBT_DRAW_DC_TEXT_STROKES - CBT_Block_Start_DC;
};




typedef void (ACTION_CALL * IM_Draw_DC_Polymarker_Action)(Net_Rendition const * nr, int count, DC_Point const *points, float const *rotations, float const *size_fixups);
template <> struct Action_Call_Info<CBT_DRAW_DC_POLYMARKER> {
	typedef IM_Draw_DC_Polymarker_Action im_type;
	static const Action_Block_ID		action_block_id = Action_Block_ID_DC;
	static const size_t					action_offset = CBT_DRAW_DC_POLYMARKER - CBT_Block_Start_DC;
};

typedef void (ACTION_CALL * IM_Draw_DC_Polytriangle_Action)(Net_Rendition const * nr, int count, DC_Point const *points);
template <> struct Action_Call_Info<CBT_DRAW_DC_POLYTRIANGLE> {
	typedef IM_Draw_DC_Polytriangle_Action im_type;
	static const Action_Block_ID		action_block_id = Action_Block_ID_DC;
	static const size_t					action_offset = CBT_DRAW_DC_POLYTRIANGLE - CBT_Block_Start_DC;
};

typedef void (ACTION_CALL * IM_Draw_DC_Colorized_Polytriangle_Action)(Net_Rendition const * nr, int count, DC_Point const *points, RGBAS32 const *color, bool single);
template <> struct Action_Call_Info<CBT_DRAW_DC_COLORIZED_POLYTRIANGLE> {
	typedef IM_Draw_DC_Colorized_Polytriangle_Action im_type;
	static const Action_Block_ID		action_block_id = Action_Block_ID_DC;
	static const size_t					action_offset = CBT_DRAW_DC_COLORIZED_POLYTRIANGLE - CBT_Block_Start_DC;
};

typedef void (ACTION_CALL * IM_Draw_DC_Gouraud_Polytriangle_Action)(Net_Rendition const * nr, int count, DC_Point const *points, RGBAS32 const *colors);
template <> struct Action_Call_Info<CBT_DRAW_DC_GOURAUD_POLYTRIANGLE> {
	typedef IM_Draw_DC_Gouraud_Polytriangle_Action	im_type;
	static const Action_Block_ID		action_block_id = Action_Block_ID_DC;
	static const size_t					action_offset = CBT_DRAW_DC_GOURAUD_POLYTRIANGLE - CBT_Block_Start_DC;
};

typedef void (ACTION_CALL * IM_Draw_DC_Phong_Polytriangle_Action)(Net_Rendition const * nr, int count, DC_Point const *points, RGBA const *colors, Plane const *planes, DPlane const *dplanes);
template <> struct Action_Call_Info<CBT_DRAW_DC_PHONG_POLYTRIANGLE> {
	typedef IM_Draw_DC_Phong_Polytriangle_Action im_type;
	static const Action_Block_ID		action_block_id = Action_Block_ID_DC;
	static const size_t					action_offset = CBT_DRAW_DC_PHONG_POLYTRIANGLE - CBT_Block_Start_DC;
};

typedef void (ACTION_CALL * IM_Draw_DC_Textured_Polytriangle_Action)(Net_Rendition const * nr, int count, DC_Point const *points, RGBA const *colors, Plane const *planes, DPlane const *dplanes, float const *params, int param_width, int32_t param_flags);
template <> struct Action_Call_Info<CBT_DRAW_DC_TEXTURED_POLYTRIANGLE> {
	typedef IM_Draw_DC_Textured_Polytriangle_Action im_type;
	static const Action_Block_ID		action_block_id = Action_Block_ID_DC;
	static const size_t					action_offset = CBT_DRAW_DC_TEXTURED_POLYTRIANGLE - CBT_Block_Start_DC;
};

typedef void (ACTION_CALL * IM_Draw_DC_Colorized_Polyline_Action)(Net_Rendition const * nr, int count, DC_Point const *points, RGBAS32 const *color, bool single_color);
template <> struct Action_Call_Info<CBT_DRAW_DC_COLORIZED_POLYLINE> {
	typedef IM_Draw_DC_Colorized_Polyline_Action im_type;
	static const Action_Block_ID		action_block_id = Action_Block_ID_DC;
	static const size_t					action_offset = CBT_DRAW_DC_COLORIZED_POLYLINE - CBT_Block_Start_DC;
};

typedef void (ACTION_CALL * IM_Draw_DC_Gouraud_Polyline_Action)(Net_Rendition const * nr, int count, DC_Point const *points, RGBAS32 const *colors);
template <> struct Action_Call_Info<CBT_DRAW_DC_GOURAUD_POLYLINE> {
	typedef IM_Draw_DC_Gouraud_Polyline_Action im_type;
	static const Action_Block_ID		action_block_id = Action_Block_ID_DC;
	static const size_t					action_offset = CBT_DRAW_DC_GOURAUD_POLYLINE - CBT_Block_Start_DC;
};

typedef void (ACTION_CALL * IM_Draw_DC_Phong_Polyline_Action)(Net_Rendition const * nr, int count, DC_Point const *points, RGBA const *colors, Plane const *planes, DPlane const *dplanes);
template <> struct Action_Call_Info<CBT_DRAW_DC_PHONG_POLYLINE> {
	typedef IM_Draw_DC_Phong_Polyline_Action im_type;
	static const Action_Block_ID		action_block_id = Action_Block_ID_DC;
	static const size_t					action_offset = CBT_DRAW_DC_PHONG_POLYLINE - CBT_Block_Start_DC;
};

typedef void (ACTION_CALL * IM_Draw_DC_Textured_Polyline_Action)(Net_Rendition const * nr, int count, DC_Point const *points, RGBA const *colors, Plane const *planes, DPlane const *dplanes, float const *params, int param_width, int32_t param_flags);
template <> struct Action_Call_Info<CBT_DRAW_DC_TEXTURED_POLYLINE> {
	typedef IM_Draw_DC_Textured_Polyline_Action im_type;
	static const Action_Block_ID		action_block_id = Action_Block_ID_DC;
	static const size_t					action_offset = CBT_DRAW_DC_TEXTURED_POLYLINE - CBT_Block_Start_DC;
};

typedef void (ACTION_CALL * IM_Draw_DC_Colorized_Polymarker_Action)(Net_Rendition const * nr, int count, DC_Point const *points,RGBAS32 const *color, bool single_color, float const *rotations, float const *size_fixups);
template <> struct Action_Call_Info<CBT_DRAW_DC_COLORIZED_POLYMARKER> {
	typedef IM_Draw_DC_Colorized_Polymarker_Action im_type;
	static const Action_Block_ID		action_block_id = Action_Block_ID_DC;
	static const size_t					action_offset = CBT_DRAW_DC_COLORIZED_POLYMARKER - CBT_Block_Start_DC;
};

typedef void (ACTION_CALL * IM_Draw_DC_Polydot_Action)(Net_Rendition const * nr, int count, DC_Point const *points);
template <> struct Action_Call_Info<CBT_HLR_POLYDOT> {
	typedef IM_Draw_DC_Polydot_Action im_type;
	static const Action_Block_ID		action_block_id = Action_Block_ID_HLR;
	static const size_t					action_offset = CBT_HLR_POLYDOT - CBT_Block_Start_HLR;
};
template <> struct Action_Call_Info<CBT_HLR_HIDDEN_POLYDOT> {
	typedef IM_Draw_DC_Polydot_Action im_type;
	static const Action_Block_ID		action_block_id = Action_Block_ID_HLR;
	static const size_t					action_offset = CBT_HLR_HIDDEN_POLYDOT - CBT_Block_Start_HLR;
};

typedef void (ACTION_CALL * IM_Draw_DC_Colorized_Polydot_Action)(Net_Rendition const * nr, int count, DC_Point const *points, RGBAS32 const *color, bool single);
template <> struct Action_Call_Info<CBT_HLR_COLORIZED_POLYDOT> {
	typedef IM_Draw_DC_Colorized_Polydot_Action im_type;
	static const Action_Block_ID		action_block_id = Action_Block_ID_HLR;
	static const size_t					action_offset = CBT_HLR_COLORIZED_POLYDOT - CBT_Block_Start_HLR;
};
template <> struct Action_Call_Info<CBT_HLR_HIDDEN_COLORIZED_POLYDOT> {
	typedef IM_Draw_DC_Colorized_Polydot_Action im_type;
	static const Action_Block_ID		action_block_id = Action_Block_ID_HLR;
	static const size_t					action_offset = CBT_HLR_HIDDEN_COLORIZED_POLYDOT - CBT_Block_Start_HLR;
};

typedef void (ACTION_CALL * IM_Draw_DC_Triangle_Action)(Net_Rendition const * nr, DC_Point const *points);
template <> struct Action_Call_Info<CBT_DRAW_DC_TRIANGLE> {
	typedef IM_Draw_DC_Triangle_Action im_type;
	static const Action_Block_ID		action_block_id = Action_Block_ID_DC;
	static const size_t					action_offset = CBT_DRAW_DC_TRIANGLE - CBT_Block_Start_DC;
};

typedef void (ACTION_CALL * IM_Draw_DC_Colorized_Triangle_Action)(Net_Rendition const * nr, DC_Point const *points, RGBAS32 const *color);
template <> struct Action_Call_Info<CBT_DRAW_DC_COLORIZED_TRIANGLE> {
	typedef IM_Draw_DC_Colorized_Triangle_Action im_type;
	static const Action_Block_ID		action_block_id = Action_Block_ID_DC;
	static const size_t					action_offset = CBT_DRAW_DC_COLORIZED_TRIANGLE - CBT_Block_Start_DC;
};

typedef void (ACTION_CALL * IM_Draw_DC_Gouraud_Triangle_Action)(Net_Rendition const * nr, DC_Point const *points, RGBAS32 const *colors);
template <> struct Action_Call_Info<CBT_DRAW_DC_GOURAUD_TRIANGLE> {
	typedef IM_Draw_DC_Gouraud_Triangle_Action im_type;
	static const Action_Block_ID		action_block_id = Action_Block_ID_DC;
	static const size_t					action_offset = CBT_DRAW_DC_GOURAUD_TRIANGLE - CBT_Block_Start_DC;
};

typedef void (ACTION_CALL * IM_Draw_DC_Reshaded_Triangle_Action)(Net_Rendition const * nr, DC_Point const *points, RGBA const *colors, Plane const *planes, float const *params, int param_width, int32_t param_flags);
template <> struct Action_Call_Info<CBT_DRAW_DC_RESHADED_TRIANGLE> {
	typedef IM_Draw_DC_Reshaded_Triangle_Action im_type;
	static const Action_Block_ID		action_block_id = Action_Block_ID_DC;
	static const size_t					action_offset = CBT_DRAW_DC_RESHADED_TRIANGLE - CBT_Block_Start_DC;
};

typedef void (ACTION_CALL * IM_Draw_DC_Line_Action)(Net_Rendition const * nr, DC_Point const *points);
template <> struct Action_Call_Info<CBT_DRAW_DC_LINE> {
	typedef IM_Draw_DC_Line_Action im_type;
	static const Action_Block_ID		action_block_id = Action_Block_ID_DC;
	static const size_t					action_offset = CBT_DRAW_DC_LINE - CBT_Block_Start_DC;
};

typedef void (ACTION_CALL * IM_Draw_DC_Colorized_Line_Action)(Net_Rendition const * nr, DC_Point const *points, RGBAS32 const *color);
template <> struct Action_Call_Info<CBT_DRAW_DC_COLORIZED_LINE> {
	typedef IM_Draw_DC_Colorized_Line_Action im_type;
	static const Action_Block_ID		action_block_id = Action_Block_ID_DC;
	static const size_t					action_offset = CBT_DRAW_DC_COLORIZED_LINE - CBT_Block_Start_DC;
};

typedef void (ACTION_CALL * IM_Draw_DC_Gouraud_Line_Action)(Net_Rendition const * nr, DC_Point const *points, RGBAS32 const *colors);
template <> struct Action_Call_Info<CBT_DRAW_DC_GOURAUD_LINE> {
	typedef IM_Draw_DC_Gouraud_Line_Action im_type;
	static const Action_Block_ID		action_block_id = Action_Block_ID_DC;
	static const size_t					action_offset = CBT_DRAW_DC_GOURAUD_LINE - CBT_Block_Start_DC;
};

typedef void (ACTION_CALL * IM_Draw_DC_Reshaded_Line_Action)(Net_Rendition const * nr, DC_Point const *points, RGBA const *colors, Plane const *planes, float const *params, int param_width, int32_t param_flags);
template <> struct Action_Call_Info<CBT_DRAW_DC_RESHADED_LINE> {
	typedef IM_Draw_DC_Reshaded_Line_Action im_type;
	static const Action_Block_ID		action_block_id = Action_Block_ID_DC;
	static const size_t					action_offset = CBT_DRAW_DC_RESHADED_LINE - CBT_Block_Start_DC;
};

typedef void (ACTION_CALL * IM_Draw_DC_Marker_Action)(Net_Rendition const * nr, DC_Point const *where);
template <> struct Action_Call_Info<CBT_HLR_MARKER> {
	typedef IM_Draw_DC_Marker_Action im_type;
	static const Action_Block_ID		action_block_id = Action_Block_ID_HLR;
	static const size_t					action_offset = CBT_HLR_MARKER - CBT_Block_Start_HLR;
};
template <> struct Action_Call_Info<CBT_HLR_HIDDEN_MARKER> {
	typedef IM_Draw_DC_Marker_Action im_type;
	static const Action_Block_ID		action_block_id = Action_Block_ID_HLR;
	static const size_t					action_offset = CBT_HLR_HIDDEN_MARKER - CBT_Block_Start_HLR;
};
template <> struct Action_Call_Info<CBT_DRAW_DC_MARKER> {
	typedef IM_Draw_DC_Marker_Action im_type;
	static const Action_Block_ID		action_block_id = Action_Block_ID_DC;
	static const size_t					action_offset = CBT_DRAW_DC_MARKER - CBT_Block_Start_DC;
};

typedef void (ACTION_CALL * IM_Draw_DC_Colorized_Marker_Action)(Net_Rendition const * nr, DC_Point const *where, RGBAS32 const *color);
template <> struct Action_Call_Info<CBT_DRAW_DC_COLORIZED_MARKER> {
	typedef IM_Draw_DC_Colorized_Marker_Action im_type;
	static const Action_Block_ID		action_block_id = Action_Block_ID_DC;
	static const size_t					action_offset = CBT_DRAW_DC_COLORIZED_MARKER - CBT_Block_Start_DC;
};

typedef void (ACTION_CALL * IM_Draw_DC_Face_Action)(Net_Rendition const * nr, int count, DC_Point const *points);
template <> struct Action_Call_Info<CBT_DRAW_DC_FACE> {
	typedef IM_Draw_DC_Face_Action im_type;
	static const Action_Block_ID		action_block_id = Action_Block_ID_DC;
	static const size_t					action_offset = CBT_DRAW_DC_FACE - CBT_Block_Start_DC;
};

template <> struct Action_Call_Info<CBT_DRAW_DC_TEXT_AREAS> {
	typedef IM_Draw_DC_Face_Action im_type;
	static const Action_Block_ID		action_block_id = Action_Block_ID_DC;
	static const size_t					action_offset = CBT_DRAW_DC_TEXT_AREAS - CBT_Block_Start_DC;
};


typedef void (ACTION_CALL * IM_Draw_DC_Colorized_Face_Action)(Net_Rendition const * nr, int count, DC_Point const *points, RGBAS32 const *color);
template <> struct Action_Call_Info<CBT_DRAW_DC_COLORIZED_FACE> {
	typedef IM_Draw_DC_Colorized_Face_Action im_type;
	static const Action_Block_ID		action_block_id = Action_Block_ID_DC;
	static const size_t					action_offset = CBT_DRAW_DC_COLORIZED_FACE - CBT_Block_Start_DC;
};


typedef void (ACTION_CALL * IM_Draw_Text_Action)(Net_Rendition const * nr, Text_Info const *info);
template <> struct Action_Call_Info<CBT_DRAW_TEXT> {
	typedef IM_Draw_Text_Action im_type;
	static const Action_Block_ID		action_block_id = Action_Block_ID_DC;
	static const size_t					action_offset = CBT_DRAW_TEXT - CBT_Block_Start_DC;
};
template <> struct Action_Call_Info<CBT_SELECT_TEXT> {
	typedef IM_Draw_Text_Action im_type;
	static const Action_Block_ID		action_block_id = Action_Block_ID_Select;
	static const size_t					action_offset = CBT_SELECT_TEXT - CBT_Block_Start_Select;
};


typedef void (ACTION_CALL * IM_Draw_Window_Action)(Net_Rendition const * nr, IntRectangle const *extent);
template <> struct Action_Call_Info<CBT_DRAW_WINDOW> {
	typedef IM_Draw_Window_Action im_type;
	static const Action_Block_ID		action_block_id = Action_Block_ID_DC;
	static const size_t					action_offset = CBT_DRAW_WINDOW - CBT_Block_Start_DC;
};
template <> struct Action_Call_Info<CBT_SELECT_WINDOW> {
	typedef IM_Draw_Window_Action im_type;
	static const Action_Block_ID		action_block_id = Action_Block_ID_Select;
	static const size_t					action_offset = CBT_SELECT_WINDOW - CBT_Block_Start_Select;
};

typedef void (ACTION_CALL * IM_Draw_Window_Frame_Action)(Net_Rendition const * nr, IntRectangle const *extent, IntRectangle *frame, bool calculate_only);
template <> struct Action_Call_Info<CBT_DRAW_WINDOW_FRAME> {
	typedef IM_Draw_Window_Frame_Action im_type;
	static const Action_Block_ID		action_block_id = Action_Block_ID_DC;
	static const size_t					action_offset = CBT_DRAW_WINDOW_FRAME - CBT_Block_Start_DC;
};


typedef void (ACTION_CALL * IM_Draw_3D_Cylinder_Action)(Net_Rendition const * nr, Cylinder const *cyl);
template <> struct Action_Call_Info<CBT_DRAW_3D_CYLINDER> {
	typedef IM_Draw_3D_Cylinder_Action im_type;
	static const Action_Block_ID		action_block_id = Action_Block_ID_3D;
	static const size_t					action_offset = CBT_DRAW_3D_CYLINDER - CBT_Block_Start_3D;
};

typedef void (ACTION_CALL * IM_Draw_3D_Ellipse_Action)(Net_Rendition const * nr, Ellipse const *el);
template <> struct Action_Call_Info<CBT_DRAW_3D_ELLIPSE> {
	typedef IM_Draw_3D_Ellipse_Action im_type;
	static const Action_Block_ID		action_block_id = Action_Block_ID_3D;
	static const size_t					action_offset = CBT_DRAW_3D_ELLIPSE - CBT_Block_Start_3D;
};

typedef void (ACTION_CALL * IM_Draw_3D_Elliptical_Arc_Action)(Net_Rendition const * nr, Elliptical_Arc const *ela);
template <> struct Action_Call_Info<CBT_DRAW_3D_ELLIPTICAL_ARC> {
	typedef IM_Draw_3D_Elliptical_Arc_Action im_type;
	static const Action_Block_ID		action_block_id = Action_Block_ID_3D;
	static const size_t					action_offset = CBT_DRAW_3D_ELLIPTICAL_ARC - CBT_Block_Start_3D;
};

typedef void (ACTION_CALL * IM_Draw_3D_Grid_Action)(Net_Rendition const * nr, Grid const *gr);
template <> struct Action_Call_Info<CBT_DRAW_3D_GRID> {
	typedef IM_Draw_3D_Grid_Action im_type;
	static const Action_Block_ID		action_block_id = Action_Block_ID_3D;
	static const size_t					action_offset = CBT_DRAW_3D_GRID - CBT_Block_Start_3D;
};

typedef void (ACTION_CALL * IM_Draw_3D_Image_Action)(Net_Rendition const * nr, Image const *img);
template <> struct Action_Call_Info<CBT_DRAW_3D_IMAGE> {
	typedef IM_Draw_3D_Image_Action im_type;
	static const Action_Block_ID		action_block_id = Action_Block_ID_3D;
	static const size_t					action_offset = CBT_DRAW_3D_IMAGE - CBT_Block_Start_3D;
};

typedef void (ACTION_CALL * IM_Draw_3D_Marker_Action)(Net_Rendition const * nr, Marker const *pl);
template <> struct Action_Call_Info<CBT_DRAW_3D_MARKER> {
	typedef IM_Draw_3D_Marker_Action im_type;
	static const Action_Block_ID		action_block_id = Action_Block_ID_3D;
	static const size_t					action_offset = CBT_DRAW_3D_MARKER - CBT_Block_Start_3D;
};
template <> struct Action_Call_Info<CBT_SELECT_3D_MARKER> {
	typedef IM_Draw_3D_Marker_Action im_type;
	static const Action_Block_ID		action_block_id = Action_Block_ID_Select;
	static const size_t					action_offset = CBT_SELECT_3D_MARKER - CBT_Block_Start_Select;
};

typedef void (ACTION_CALL * IM_Draw_3D_Multimarker_Action)(Net_Rendition const * nr, Multimarker const *mm);
template <> struct Action_Call_Info<CBT_DRAW_3D_MULTIMARKER> {
	typedef IM_Draw_3D_Multimarker_Action im_type;
	static const Action_Block_ID		action_block_id = Action_Block_ID_3D;
	static const size_t					action_offset = CBT_DRAW_3D_MULTIMARKER - CBT_Block_Start_3D;
};

typedef void (ACTION_CALL * IM_Draw_3D_NURBS_Curve_Action)(Net_Rendition const * nr, NURBS_Curve const *crv);
template <> struct Action_Call_Info<CBT_DRAW_3D_NURBS_CURVE> {
	typedef IM_Draw_3D_NURBS_Curve_Action im_type;
	static const Action_Block_ID		action_block_id = Action_Block_ID_3D;
	static const size_t					action_offset = CBT_DRAW_3D_NURBS_CURVE - CBT_Block_Start_3D;
};

typedef void (ACTION_CALL * IM_Draw_3D_NURBS_Surface_Action)(Net_Rendition const * nr, NURBS_Surface const *srf);
template <> struct Action_Call_Info<CBT_DRAW_3D_NURBS_SURFACE> {
	typedef IM_Draw_3D_NURBS_Surface_Action im_type;
	static const Action_Block_ID		action_block_id = Action_Block_ID_3D;
	static const size_t					action_offset = CBT_DRAW_3D_NURBS_SURFACE - CBT_Block_Start_3D;
};

typedef void (ACTION_CALL * IM_Draw_3D_PolyCylinder_Action)(Net_Rendition const * nr, PolyCylinder const *pcyl);
template <> struct Action_Call_Info<CBT_DRAW_3D_POLYCYLINDER> {
	typedef IM_Draw_3D_PolyCylinder_Action im_type;
	static const Action_Block_ID		action_block_id = Action_Block_ID_3D;
	static const size_t					action_offset = CBT_DRAW_3D_POLYCYLINDER - CBT_Block_Start_3D;
};

typedef void (ACTION_CALL * IM_Draw_3D_Polygon_Action)(Net_Rendition const * nr, Polygon const *pg);
template <> struct Action_Call_Info<CBT_DRAW_3D_POLYGON> {
	typedef IM_Draw_3D_Polygon_Action im_type;
	static const Action_Block_ID		action_block_id = Action_Block_ID_3D;
	static const size_t					action_offset = CBT_DRAW_3D_POLYGON - CBT_Block_Start_3D;
};
template <> struct Action_Call_Info<CBT_SELECT_3D_POLYGON> {
	typedef IM_Draw_3D_Polygon_Action im_type;
	static const Action_Block_ID		action_block_id = Action_Block_ID_Select;
	static const size_t					action_offset = CBT_SELECT_3D_POLYGON - CBT_Block_Start_Select;
};
template <> struct Action_Call_Info<CBT_DRAW_3D_TEXT_AREAS> {
	typedef IM_Draw_3D_Polygon_Action im_type;
	static const Action_Block_ID		action_block_id = Action_Block_ID_3D;
	static const size_t					action_offset = CBT_DRAW_3D_TEXT_AREAS - CBT_Block_Start_3D;
};


typedef void (ACTION_CALL * IM_Draw_3D_Polyhedron_Action)(Net_Rendition const * nr, Polyhedron const *ph);
template <> struct Action_Call_Info<CBT_DRAW_3D_POLYHEDRON> {
	typedef IM_Draw_3D_Polyhedron_Action im_type;
	static const Action_Block_ID		action_block_id = Action_Block_ID_3D;
	static const size_t					action_offset = CBT_DRAW_3D_POLYHEDRON - CBT_Block_Start_3D;
};

typedef void (ACTION_CALL * IM_Draw_3D_Polyline_Action)(Net_Rendition const * nr, Polyline const *pl);
template <> struct Action_Call_Info<CBT_DRAW_3D_POLYLINE> {
	typedef IM_Draw_3D_Polyline_Action im_type;
	static const Action_Block_ID		action_block_id = Action_Block_ID_3D;
	static const size_t					action_offset = CBT_DRAW_3D_POLYLINE - CBT_Block_Start_3D;
};
template <> struct Action_Call_Info<CBT_SELECT_3D_POLYLINE> {
	typedef IM_Draw_3D_Polyline_Action im_type;
	static const Action_Block_ID		action_block_id = Action_Block_ID_Select;
	static const size_t					action_offset = CBT_SELECT_3D_POLYLINE - CBT_Block_Start_Select;
};
template <> struct Action_Call_Info<CBT_DRAW_3D_TEXT_STROKES> {
	typedef IM_Draw_3D_Polyline_Action im_type;
	static const Action_Block_ID		action_block_id = Action_Block_ID_3D;
	static const size_t					action_offset = CBT_DRAW_3D_TEXT_STROKES - CBT_Block_Start_3D;
};
template <> struct Action_Call_Info<CBT_DRAW_3D_INFINITE_LINE> {
	typedef IM_Draw_3D_Polyline_Action im_type;
	static const Action_Block_ID		action_block_id = Action_Block_ID_3D;
	static const size_t					action_offset = CBT_DRAW_3D_INFINITE_LINE - CBT_Block_Start_3D;
};


typedef void (ACTION_CALL * IM_Draw_3D_Sphere_Action)(Net_Rendition const * nr, Sphere const *s);
template <> struct Action_Call_Info<CBT_DRAW_3D_SPHERE> {
	typedef IM_Draw_3D_Sphere_Action im_type;
	static const Action_Block_ID		action_block_id = Action_Block_ID_3D;
	static const size_t					action_offset = CBT_DRAW_3D_SPHERE - CBT_Block_Start_3D;
};

typedef void (ACTION_CALL * IM_Draw_3D_Text_Action)(Net_Rendition const * nr, Text const *tx);
template <> struct Action_Call_Info<CBT_DRAW_3D_TEXT> {
	typedef IM_Draw_3D_Text_Action im_type;
	static const Action_Block_ID		action_block_id = Action_Block_ID_3D;
	static const size_t					action_offset = CBT_DRAW_3D_TEXT - CBT_Block_Start_3D;
};
template <> struct Action_Call_Info<CBT_SELECT_3D_TEXT> {
	typedef IM_Draw_3D_Text_Action im_type;
	static const Action_Block_ID		action_block_id = Action_Block_ID_Select;
	static const size_t					action_offset = CBT_SELECT_3D_TEXT - CBT_Block_Start_Select;
};


typedef void (ACTION_CALL * IM_Draw_3D_Geometry_Action)(Net_Rendition const * nr, Geometry const *g);
template <> struct Action_Call_Info<CBT_DRAW_3D_GEOMETRY> {
	typedef IM_Draw_3D_Geometry_Action im_type;
	static const Action_Block_ID		action_block_id = Action_Block_ID_3D;
	static const size_t					action_offset = CBT_DRAW_3D_GEOMETRY - CBT_Block_Start_3D;
};
template <> struct Action_Call_Info<CBT_SELECT_3D_GEOMETRY> {
	typedef IM_Draw_3D_Geometry_Action im_type;
	static const Action_Block_ID		action_block_id = Action_Block_ID_Select;
	static const size_t					action_offset = CBT_SELECT_3D_GEOMETRY - CBT_Block_Start_Select;
};

typedef void (ACTION_CALL * IM_Draw_3D_Tristrip_Action)(Net_Rendition const * nr, Tristrip const *ts);
template <> struct Action_Call_Info<CBT_DRAW_3D_TRISTRIP> {
	typedef IM_Draw_3D_Tristrip_Action im_type;
	static const Action_Block_ID		action_block_id = Action_Block_ID_3D;
	static const size_t					action_offset = CBT_DRAW_3D_TRISTRIP - CBT_Block_Start_3D;
};

typedef void (ACTION_CALL * IM_Draw_3D_Polyedge_Action)(Net_Rendition const * nr, Polyedge const *pe);
template <> struct Action_Call_Info<CBT_DRAW_3D_POLYEDGE> {
	typedef IM_Draw_3D_Polyedge_Action im_type;
	static const Action_Block_ID		action_block_id = Action_Block_ID_3D;
	static const size_t					action_offset = CBT_DRAW_3D_POLYEDGE - CBT_Block_Start_3D;
};
template <> struct Action_Call_Info<CBT_DRAW_3D_ISOLINE> {
	typedef IM_Draw_3D_Polyedge_Action im_type;
	static const Action_Block_ID		action_block_id = Action_Block_ID_3D;
	static const size_t					action_offset = CBT_DRAW_3D_ISOLINE - CBT_Block_Start_3D;
};

typedef void (ACTION_CALL * IM_Draw_3D_Polymarker_Action)(Net_Rendition const * nr, Polymarker const *pm);
template <> struct Action_Call_Info<CBT_DRAW_3D_POLYMARKER> {
	typedef IM_Draw_3D_Polymarker_Action im_type;
	static const Action_Block_ID		action_block_id = Action_Block_ID_3D;
	static const size_t					action_offset = CBT_DRAW_3D_POLYMARKER - CBT_Block_Start_3D;
};





typedef void (ACTION_CALL * IM_Driver_Startup_Action)(Net_Rendition const * nr);
template <> struct Action_Call_Info<CBT_DRIVER_STARTUP> {
	typedef IM_Driver_Startup_Action im_type;
	static const Action_Block_ID		action_block_id = Action_Block_ID_Driver;
	static const size_t					action_offset = CBT_DRIVER_STARTUP - CBT_Block_Start_Driver;
};

typedef void (ACTION_CALL * IM_Driver_Shutdown_Action)(Net_Rendition const * nr);
template <> struct Action_Call_Info<CBT_DRIVER_SHUTDOWN> {
	typedef IM_Driver_Shutdown_Action im_type;
	static const Action_Block_ID		action_block_id = Action_Block_ID_Driver;
	static const size_t					action_offset = CBT_DRIVER_SHUTDOWN - CBT_Block_Start_Driver;
};

typedef void (ACTION_CALL * IM_Init_Picture_Action)(Net_Rendition const * nr);
template <> struct Action_Call_Info<CBT_INIT_PICTURE> {
	typedef IM_Init_Picture_Action im_type;
	static const Action_Block_ID		action_block_id = Action_Block_ID_Driver;
	static const size_t					action_offset = CBT_INIT_PICTURE - CBT_Block_Start_Driver;
};

typedef void (ACTION_CALL * IM_Finish_Picture_Action)(Net_Rendition const * nr, bool swap_buffers);
template <> struct Action_Call_Info<CBT_FINISH_PICTURE> {
	typedef IM_Finish_Picture_Action im_type;
	static const Action_Block_ID		action_block_id = Action_Block_ID_Driver;
	static const size_t					action_offset = CBT_FINISH_PICTURE - CBT_Block_Start_Driver;
};

typedef void (ACTION_CALL * IM_Create_Region_Action)(Net_Rendition const * nr, IntRectangle const *area, void * *image_buffer, void * *z_buffer);
template <> struct Action_Call_Info<CBT_CREATE_REGION> {
	typedef IM_Create_Region_Action im_type;
	static const Action_Block_ID		action_block_id = Action_Block_ID_Driver;
	static const size_t					action_offset = CBT_CREATE_REGION - CBT_Block_Start_Driver;
};

typedef void (ACTION_CALL * IM_Save_Region_Action)(Net_Rendition const * nr, IntRectangle const *area, void *image_buffer, void *z_buffer);
template <> struct Action_Call_Info<CBT_SAVE_REGION> {
	typedef IM_Save_Region_Action im_type;
	static const Action_Block_ID		action_block_id = Action_Block_ID_Driver;
	static const size_t					action_offset = CBT_SAVE_REGION - CBT_Block_Start_Driver;
};

typedef void (ACTION_CALL * IM_Restore_Region_Action)(Net_Rendition const * nr, IntRectangle const *area, void *image_buffer, void *z_buffer);
template <> struct Action_Call_Info<CBT_RESTORE_REGION> {
	typedef IM_Restore_Region_Action im_type;
	static const Action_Block_ID		action_block_id = Action_Block_ID_Driver;
	static const size_t					action_offset = CBT_RESTORE_REGION - CBT_Block_Start_Driver;
};

typedef void (ACTION_CALL * IM_Destroy_Region_Action)(Net_Rendition const * nr, void *image_buffer, void *z_buffer);
template <> struct Action_Call_Info<CBT_DESTROY_REGION> {
	typedef IM_Destroy_Region_Action im_type;
	static const Action_Block_ID		action_block_id = Action_Block_ID_Driver;
	static const size_t					action_offset = CBT_DESTROY_REGION - CBT_Block_Start_Driver;
};

typedef void (ACTION_CALL * IM_Set_Shader_Action)(Net_Rendition const * nr, int driver_type, int geom_type, int geom_bits, void *thing);
template <> struct Action_Call_Info<CBT_SET_SHADER> {
	typedef IM_Set_Shader_Action im_type;
	static const Action_Block_ID		action_block_id = Action_Block_ID_Driver;
	static const size_t					action_offset = CBT_SET_SHADER - CBT_Block_Start_Driver;
};


typedef uint32_t	Drawing_Mode;

enum Drawing_Mode_Flags : Drawing_Mode {

	Draw_NONE						= 0x00000000,
	Draw_ALL_BITS					= 0xFFFFFFFF,

	Draw_2D							= 0x00000001,
	Draw_QUICKMOVES					= 0x00000002,
	Draw_UNUSED_0					= 0x00000004,
	Draw_UNUSED_1					= 0x00000008,
	Draw_HIDDEN						= 0x00000010,
	Draw_WINDOW						= 0x00000020,
	Draw_LIGHTED					= 0x00000040,
	Draw_Z_BUFFERED					= 0x00000080,
	Draw_OUT_OF_ORDER				= 0x00000100,
	Draw_SOFTWARE_FRAME_BUFFERED	= 0x00000200,
	Draw_SOFTWARE_Z_BUFFERED		= 0x00000400,
	Draw_VANILLA_3D_FACES			= 0x00000800,
	Draw_VANILLA_3D_LINES			= 0x00001000,
	Draw_CUTTING_PLANED				= 0x00002000,
	Draw_VANILLA_3D_MARKERS			= 0x00004000,
	Draw_ACTUAL_WINDOW				= 0x00008000,
	Draw_VANILLA_3D_MISC			= 0x00010000,
	Draw_TRANSPARENT				= 0x00020000,
	Draw_UNUSED_2					= 0x00040000,
	Draw_DEFERRED_LINES				= 0x00080000,
	Draw_UNUSED_3					= 0x00100000,
	Draw_GENERATE_CUT_GEOMETRY		= 0x00200000, // not really a state but a flag for a special case
	Draw_DEFERRED_TEXT				= 0x00400000,
	Draw_ZHLR						= 0x00800000,
	Draw_SUPRESS_2D					= 0x80000000,
	
	Draw_ANY_HIDDEN	= (Draw_HIDDEN|Draw_SOFTWARE_Z_BUFFERED|Draw_Z_BUFFERED|Draw_ZHLR),
	Draw_VANILLA_3D	= (Draw_VANILLA_3D_FACES|Draw_VANILLA_3D_LINES| Draw_VANILLA_3D_MARKERS|Draw_VANILLA_3D_MISC),
};


struct AR_Action {
	Void_Routine	address;
	Segment const *	segment;
	void const *	data;
};

struct Internal_AR_Action_Block : public Counted_Object_Pooled {
	friend class Counted_Pointer_Pooled<Internal_AR_Action_Block>;
	friend class Internal_Action_Rendition;

private:

	Internal_AR_Action_Block (Memory_Pool * p)
		: Counted_Object_Pooled(p), actions (null), count (0) {}

	Internal_AR_Action_Block (Internal_AR_Action_Block const & that) : Counted_Object_Pooled(that) {
		count = that.count;
		POOL_ALLOC_ARRAY (actions, count, AR_Action, memory_pool);
		COPY_ARRAY (that.actions, count, AR_Action, actions);
	}

	Internal_AR_Action_Block const & operator= (Internal_AR_Action_Block const & that);

public:

	AR_Action *		actions;
	size_t			count;
	
	~Internal_AR_Action_Block () {
		if (count > 0)
			FREE_ARRAY (actions, count, AR_Action);
	}

private:

	void init(int id) {
		count = block_size(id);
		POOL_ZALLOC_ARRAY (actions, count, AR_Action, memory_pool);
	}

	static size_t block_size(int id) {
		switch(id) {
		case Action_Block_ID_Segment: return (CBT_Block_End_SEGMENT - CBT_Block_Start_SEGMENT) + 1;
		case Action_Block_ID_DC: return (CBT_Block_End_DC - CBT_Block_Start_DC) + 1;
		case Action_Block_ID_3D: return (CBT_Block_End_3D - CBT_Block_Start_3D) + 1;
		case Action_Block_ID_Driver: return (CBT_Block_End_Driver - CBT_Block_Start_Driver) + 1;
		case Action_Block_ID_Select: return (CBT_Block_End_Select - CBT_Block_Start_Select) + 1;
		case Action_Block_ID_HLR: return (CBT_Block_End_HLR - CBT_Block_Start_HLR) + 1;
		default: ASSERT(0); break;
		}
		return 0;
	}
};


class AR_Action_Block : public Counted_Pointer_Pooled<Internal_AR_Action_Block> {
	typedef		Counted_Pointer_Pooled<Internal_AR_Action_Block>		CP;

public:
	AR_Action_Block () : CP () {};
	AR_Action_Block (Null_Object const &) : CP () {}

	INLINE AR_Action_Block const &	operator= (Null_Object const &) {
		CP::release();
		CP::counted_item = 0;
		return *this;
	}

	bool operator!= (Null_Object const &) const	{ return  !(*this == AR_Action_Block (null)); }
	bool operator== (Null_Object const &) const	{ return  (*this == AR_Action_Block (null)); }

	AR_Action_Block (Counted_Pointer_Pooled<Internal_AR_Action_Block> const & that) : CP (that) {};

	bool operator== (AR_Action_Block const & that) const {
		if (CP::counted_item == that.counted_item)	// same structure or both null
			return true;
		if (CP::counted_item == null || that.counted_item == null)
			return false;
		ASSERT(0); // XXX
		return true;
	}
	bool operator!= (AR_Action_Block const & that) const	{ return  !(*this == that); }

	static AR_Action_Block Create (Memory_Pool * pool) {
		return CP::Create(pool);
	}
};


class Internal_Action_Rendition : public Rendition_Base {
	friend class Rendition_Pointer<Internal_Action_Rendition>;

	public:

		AR_Action_Block		action_block[Action_Block_ID_LAST+1];

		Drawing_Mode		drawmode;
		Action_Mask			action_mask;

		int					active_count;

		AR_Action const &	callback_type_to_action(Callback_Type type)
		{
			ASSERT(type > CBT_ZERO);

			if (type <= CBT_Block_End_SEGMENT)
				return action_block[Action_Block_ID_Segment]->actions[type - CBT_Block_Start_SEGMENT];
			else if (type <= CBT_Block_End_DC)
				return action_block[Action_Block_ID_DC]->actions[type - CBT_Block_Start_DC];
			else if (type <= CBT_Block_End_3D)
				return action_block[Action_Block_ID_3D]->actions[type - CBT_Block_Start_3D];
			else if (type <= CBT_Block_End_Driver)
				return action_block[Action_Block_ID_Driver]->actions[type - CBT_Block_Start_Driver];
			else if (type <= CBT_Block_End_Select)
				return action_block[Action_Block_ID_Select]->actions[type - CBT_Block_Start_Select];
			else if (type <= CBT_Block_End_HLR)
				return action_block[Action_Block_ID_HLR]->actions[type - CBT_Block_Start_HLR];
		
			ASSERT(0);
			return action_block[0]->actions[0]; // fail
		}

		AR_Action &	callback_type_to_action_modify(Callback_Type type)
		{
			ASSERT(type > CBT_ZERO);

			if (type <= CBT_Block_End_SEGMENT)
				return action_block[Action_Block_ID_Segment].Modify()->actions[type - CBT_Block_Start_SEGMENT];
			else if (type <= CBT_Block_End_DC)
				return action_block[Action_Block_ID_DC].Modify()->actions[type - CBT_Block_Start_DC];
			else if (type <= CBT_Block_End_3D)
				return action_block[Action_Block_ID_3D].Modify()->actions[type - CBT_Block_Start_3D];
			else if (type <= CBT_Block_End_Driver)
				return action_block[Action_Block_ID_Driver].Modify()->actions[type - CBT_Block_Start_Driver];
			else if (type <= CBT_Block_End_Select)
				return action_block[Action_Block_ID_Select].Modify()->actions[type - CBT_Block_Start_Select];
			else if (type <= CBT_Block_End_HLR)
				return action_block[Action_Block_ID_HLR].Modify()->actions[type - CBT_Block_Start_HLR];

			ASSERT(0);
			return action_block[0].Modify()->actions[0]; // fail
		}


	public:
		HOOPS_API Internal_Action_Rendition (Internal_Action_Rendition const & that);
		HOOPS_API ~Internal_Action_Rendition ();

	private:
		Internal_Action_Rendition (Display_Context const * dc);
		Internal_Action_Rendition const & operator= (Internal_Action_Rendition const & that);
};
typedef Rendition_Pointer<Internal_Action_Rendition>	Action_Rendition;


enum NRF_Flags : uint32_t {

	NRF_NONE									= 0x00000000,

	NRF_FIXUP_HIDDEN_REVERSED_COLORS			= 0x00000001,
	NRF_FIXUP_HIDDEN_REVERSED_PATTERN			= 0x00000002,
	NRF_FIXUP_HIDDEN_REVERSED_DISPLACEMENT		= 0x00000004,
	NRF_FIXUP_HIDDEN_LINE_VISIBILITIES			= 0x00000010,
	NRF_FIXUP_TINT								= 0x00000020,
	NRF_FIXUP_REFLECTION_VISIBILITIES			= 0x00000040,
	NRF_FIXUP_SELECTION_VISIBILITIES			= 0x00000080,

	NRF_FIXUP_HIDDEN_REVERSED = (NRF_FIXUP_HIDDEN_REVERSED_COLORS | NRF_FIXUP_HIDDEN_REVERSED_PATTERN |
									NRF_FIXUP_HIDDEN_REVERSED_DISPLACEMENT | NRF_FIXUP_HIDDEN_LINE_VISIBILITIES),
	
	NRF_Depth_Peeling_Collecting				= 0x00000100,
	NRF_Defer_Antialiased_Collecting			= 0x00000200,

	NRF_Pruned									= 0x00001000,

	NRF_Simple_Reflection_Visible				= 0x00010000,
	NRF_Simple_Reflection_Here					= 0x00020000,
	NRF_Simple_Shadow_Visible					= 0x00040000,
	NRF_Simple_Shadow_Here						= 0x00080000,
	NRF_Shadow_Map_Here							= 0x00100000,
	NRF_Cut_Level_Segment_Tree					= 0x00200000,
	NRF_Cut_Level_Skip							= 0x00400000,
	NRF_Buffer_Size_Limit						= 0x00800000,
	NRF_Force_Defer_Batch						= 0x01000000,
	NRF_Active_Callbacks						= 0x02000000,
	NRF_Conditional_Actions						= 0x04000000,
	NRF_Supress_Static							= 0x08000000,

	NRF_QUICKMOVES_OVERLAY						= 0x10000000,
	NRF_QUICKMOVES_SPRITING						= 0x20000000,
	NRF_QUICKMOVES_INPLACE						= 0x40000000,
	
	NRF_ANY_QUICKMOVES = (NRF_QUICKMOVES_OVERLAY|NRF_QUICKMOVES_SPRITING|NRF_QUICKMOVES_INPLACE),
};


template<> struct _bit_ops_type < NRF_Flags > {
	typedef NRF_Flags type;
};


struct Internal_Net_Rendition : public Rendition_Base {
	friend class Rendition_Pointer<Internal_Net_Rendition>;

	public:

		NRF_Flags					flags;
		Drawing_Mode				current;
		Drawing_Mode				pending;
		int							geometry_mask;
		Type_Flags					visibility;
		Type_Flags					maybe_transparent;

		Action_Rendition			action_rendition;
		Display_Rendition			display_rendition;
		Geometry_Rendition			geometry_rendition;
		Glyph_Rendition				glyph_rendition;
		Named_Style_Rendition		named_style_rendition;
		Line_Style_Rendition		line_style_rendition;	
		Misc_Rendition				misc_rendition;
		Texture_Rendition			texture_rendition;
		Transform_Rendition			transform_rendition;
		User_Rendition				user_rendition;
		Visualization_Rendition		visualization_rendition;

		Line_Rendition				edge_rendition;
		Face_Rendition				face_rendition;
		Line_Rendition				line_rendition;
		Marker_Rendition			marker_rendition;
		Text_Rendition				text_rendition;
		Marker_Rendition			vertex_rendition;
		Window_Rendition			window_rendition;

		Line_Rendition				cut_edge_rendition;
		Face_Rendition				cut_face_rendition;

	public:
		HOOPS_API Internal_Net_Rendition(Internal_Net_Rendition const & that);
		HOOPS_API ~Internal_Net_Rendition();

	private:
		Internal_Net_Rendition (Display_Context const * dc);
};






#define SOFTWARE_Z_BUFFER_DEPTH			24				/* sizeof(Depth)*8 */
#define SOFTWARE_Z_BUFFER_MAX			((1<<SOFTWARE_Z_BUFFER_DEPTH)-1)
#define SOFTWARE_Z_BUFFER_BACKGROUND	0x7F7F7F7F
#define	SOFTWARE_Z_BUFFER_OFFSET_SCALE(_tr_)((_tr_)->wlimit_software_offset_scale)

struct FBI_Vertex {
		DC_Point const *	point;
		RGBAS32 const *		color;

		Plane const *		plane;
		float const *		parameter;		/* variable size */

		/* RGB const			rgb; */
};

struct FBI_Input {
	/* per-vertex values (line=2, triangle=3, rectangle=4 (ll,lr,ul,ur) */
	FBI_Vertex			vertex[4];

	/* per-element values */
	Key					key;
	Driver_Color		color;
	int					parameter_width;

/* flags for items which may be Tested, Updated, and Interpolated */
#define FBI_NONE		0x0000	/*	   */
#define FBI_Z_VALUE		0x0001	/* TUI */
#define FBI_STENCIL		0x0002	/* TU  */
#define FBI_IMAGE		0x0004	/*	U  */
#define FBI_OCCLUSION	0x0008	/*	U  */
#define FBI_RGB32		0x0002	/*	 I */
#define FBI_NORMAL		0x0004	/*	 I */
#define FBI_TEXTURE		0x0008	/*	 I */

	struct {
		int				interpolate;
		int				test;
		int				update;
	}					requested,		/* what a function normally does */
						required,		/* special drawing modes need these */
						blocked;		/*	 but not these */


#define TEST_NEVER		0x00
#define TEST_LESS		0x01
#define TEST_EQUAL		0x02
#define TEST_GREATER	0x04
#define TEST_LEQUAL		0x03
#define TEST_GEQUAL		0x06
#define TEST_NOTEQUAL	0x05
#define TEST_ALWAYS		0x07
	int					z_test;
	int					stencil_test;

	int					stencil_reference;

#define UPDATE_KEEP		0
#define UPDATE_REPLACE	1
#define UPDATE_INCR		2
#define UPDATE_DECR		3
#define UPDATE_INVERT	4
#define UPDATE_ZERO		5
	int					stencil_fail;
	int					stencil_zfail;
	int					stencil_zpass;

	int					pixel_limit;
	int					pixels_drawn;	/* really more output :) */
};


struct Cap_Mapping {
	Geometry const *		cap_piece;
	Geometry const *		source_geometry;
};

struct Cut_Geometry {
	Cut_Geometry *			next;
	Geometry *				geometry;
	Cutting_Plane const *	cutter;
	int						offset;
	int						plane_id;
	Plane					plane;
	Cap_Mapping *			mapping;
	int						mapping_size;
	Driver_Color			face_color;
	Driver_Color			line_color;
	bool					use_local_color;
};

struct Cut_Geometry_Set {
	Cutting_Plane_Rendition	cpr;
	Cut_Geometry *			geometries;
	uint32_t				generation;

	void Clean() {
		Cut_Geometry *		cut_geometry;

		while ((cut_geometry = geometries) != null) {
			Geometry *		victim;

			while ((victim = cut_geometry->geometry) != null) {
				cut_geometry->geometry = victim->next;
				UNLINK (victim);
				INUTILE (victim);
			}

			if (cut_geometry->mapping != null)
				FREE_ARRAY (cut_geometry->mapping, cut_geometry->mapping_size, Cap_Mapping);

			geometries = cut_geometry->next;
			FREE (cut_geometry, Cut_Geometry);
		}

		generation = 0;
	}
};



struct Base_Shadow {
	Net_Rendition	master_rendition;
	Spinlock		spinlock;
	uint32_t		generation;
	bool			valid;
	bool			valid_pending;
	bool			useful;
	bool			used;
	bool			in_place;
};

struct Simple_Shadow : public Base_Shadow {
	Image *						image;
	Texture *					texture;
	DPoint						dpoints[5];
	Point						points[5];
	float						u_scale, v_scale;
	float						world_resolution;
	bool						double_precision;
};


struct Shadow_Map  : public Base_Shadow {
	Shadow_Map const *			next;

	Display_List *				depth_image;

	Key							lightkey;
	float						light_projection[16];
	float						light_modelview[16];
	float						light_eye_distance;

	/* Is this a view-dependent shadowmap? If so, the following information will be valid. */
	bool						view_dependent;

	/* How many cascade slices do we support? */
	enum { NUM_SLICES = 4 };

	/* Description of a single frustum slice. */
	struct Slice
	{
		Vector size;
		Point centre;
		float zmin, zmax;

		/* Is this slice active? If not, do not render or use it. */
		bool active;
	};

	/* Slices for the current frustum */
	Slice slices[NUM_SLICES];

	/* SIL: Temporary values for backward communication from driver. May get moved. */
	float scene_zmin, scene_zmax;
};


struct Selection_Map  : public Base_Shadow {
	//Selection_Map const *			next;

	Display_List *				map_image;

	int							height;
	int							width;
};


#define NO_INDEX				((Color_Index)-1)

struct Singleton_Color {
	Singleton_Color *			next;
	Color_Index					index;
	Driver_Color				driver_color; // always .rgb
};

struct Soft_Color_Map {
	Soft_Color_Map *			next;
	Color_Map const	*			database_origin;
	int							length;
	bool						force_load;
	Color_Index					base_index;
};


/**********************************************************************

	The drawing sequence for each primitive, except tristrips, polymarkers,
	and polyedges, is:

		3D				- does transform-and-clip
		Trimmed			- does cutting planes
		Big				- does oversize faces/polylines
		Lighted			- turns light into a local color
			Colorized	- triangulates to colorized triangles
						   (or gets captured by hidden)
		DC Callback	- user does what s/he wants
		Hide_xxx		- software hidden-surfaces happen
		DC				- draws

	*In that order.*   Each layer is responsible for making sure that
	the next "necessary" layer is called, in sequence. For efficiency,
	"necessary" usually gets determined by the caller rather than the
	callee.

	For tristrips, polymarkers, polyedges, the sequence is:

		3D_Polyhedron	- master of ceremonies
		Indexicated		- does contours
		3D_xxx			- does transform, clip/cut-testing, lighting & planes
		Clipped			- does clipping, cutting planes
		BASE				- unpacks the data, cuts "big" ones, generates DC calls
		(DC Callback - user does what s/he wants - Not implemented)
		Hide_xxx		- software hidden-surfaces happen
		DC				- draws

**********************************************************************/

struct Internal_Action_Table;
typedef Counted_Pointer_Pooled<Internal_Action_Table> Action_Table;


typedef void (ACTION_CALL * Draw_DC_Polytriangle_Action)(Net_Rendition const & nr, int count, DC_Point const *points);
typedef void (ACTION_CALL * Draw_DC_Colorized_Polytriangle_Action)(Net_Rendition const & nr, int count, DC_Point const *points, RGBAS32 const *color, bool single);
typedef void (ACTION_CALL * Draw_DC_Gouraud_Polytriangle_Action)(Net_Rendition const & nr, int count, DC_Point const *points, RGBAS32 const *colors);
typedef void (ACTION_CALL * Draw_DC_Phong_Polytriangle_Action)(Net_Rendition const & nr, int count, DC_Point const *points, RGBA const *colors, Plane const *planes, DPlane const *dplanes);
typedef void (ACTION_CALL * Draw_DC_Textured_Polytriangle_Action)(Net_Rendition const & nr, int count, DC_Point const *points, RGBA const *colors,Plane const *planes, DPlane const *dplanes, float const *params, int param_width, int32_t param_flags);

typedef void (ACTION_CALL * Draw_DC_Polyline_Action)(Net_Rendition const & nr, int count, DC_Point const *points);
typedef void (ACTION_CALL * Draw_DC_Colorized_Polyline_Action)(Net_Rendition const & nr, int count, DC_Point const *points, RGBAS32 const *color, bool single_color);
typedef void (ACTION_CALL * Draw_DC_Gouraud_Polyline_Action)(Net_Rendition const & nr, int count, DC_Point const *points, RGBAS32 const *colors);
typedef void (ACTION_CALL * Draw_DC_Phong_Polyline_Action)(Net_Rendition const & nr, int count, DC_Point const *points, RGBA const *colors, Plane const *planes, DPlane const *dplanes);
typedef void (ACTION_CALL * Draw_DC_Textured_Polyline_Action)(Net_Rendition const & nr, int count, DC_Point const *points, RGBA const *colors, Plane const *planes, DPlane const *dplanes, float const *params, int param_width, int32_t param_flags);

typedef void (ACTION_CALL * Draw_DC_Polymarker_Action)(Net_Rendition const & nr, int count, DC_Point const *points, float const *rotations, float const *size_fixups);
typedef void (ACTION_CALL * Draw_DC_Colorized_Polymarker_Action)(Net_Rendition const & nr, int count, DC_Point const *points, RGBAS32 const *color, bool single_color, float const *rotations, float const *size_fixups);

typedef void (ACTION_CALL * Draw_DC_Polydot_Action)(Net_Rendition const & nr, int count, DC_Point const *points);
typedef void (ACTION_CALL * Draw_DC_Colorized_Polydot_Action)(Net_Rendition const & nr, int count, DC_Point const *points, RGBAS32 const *color, bool single);


typedef void (ACTION_CALL * Draw_DC_Marker_Action)(Net_Rendition const & nr, DC_Point const *where);
typedef void (ACTION_CALL * Draw_DC_Colorized_Marker_Action)(Net_Rendition const & nr, DC_Point const *where, RGBAS32 const *color);

typedef void (ACTION_CALL * Draw_DC_Face_Action)(Net_Rendition const & nr, int count, DC_Point const *points);
typedef void (ACTION_CALL * Draw_DC_Colorized_Face_Action)(Net_Rendition const & nr, int count, DC_Point const *points, RGBAS32 const *color);

typedef void (ACTION_CALL * Draw_DC_Edge_Action)(Net_Rendition const & nr, int count, DC_Point const *points);

typedef void (ACTION_CALL * Draw_DC_Stenciled_Quad_Action)(Net_Rendition const & nr, DC_Point const *pts, Stencil_Bitmap const *sb);

typedef void (ACTION_CALL * Draw_DC_Text_Action)(Net_Rendition const & nr, DC_Point const *where, int count, unsigned short const *kstring);
typedef void (ACTION_CALL * Draw_DC_Text_Stroke_Action)(Net_Rendition const & nr, int count, DC_Point const *points);
typedef void (ACTION_CALL * Draw_DC_Text_Area_Action)(Net_Rendition const & nr, int count, DC_Point const *points);
typedef void (ACTION_CALL * Draw_Text_Action)(Net_Rendition const & nr, Text_Info const *info);

typedef void (ACTION_CALL * Draw_DC_Polygon_Action)(Net_Rendition const & nr, int count, DC_Point const *points);
typedef void (ACTION_CALL * Draw_DC_Rectangle_Action)(Net_Rendition const & nr, int left, int right, int bottom, int top);

typedef void (ACTION_CALL * Draw_DC_Ellipse_Action)(Net_Rendition const & nr, DC_Point const *where, DC_Point const *radius);
typedef void (ACTION_CALL * Draw_DC_Outline_Ellipse_Action)(Net_Rendition const & nr, DC_Point const *where, DC_Point const *radius);
typedef void (ACTION_CALL * Draw_DC_Elliptical_Arc_Action)(Net_Rendition const & nr, DC_Point const *where, DC_Point const *radius, float start, float end);

typedef void (ACTION_CALL * Draw_DC_Dot_Action)(Net_Rendition const & nr, DC_Point const *point);

typedef void (ACTION_CALL * Draw_DC_Image_Action)(Net_Rendition const & nr, DC_Point const *start, DC_Point const *end, int hoffset, Image_Format format, int row_bytes, void const *const *rasters, int pattern, Driver_Color const *contrast_color, Image const *db_image);

typedef void (ACTION_CALL * Draw_DC_Bit_Rasters_Action)(Net_Rendition const & nr, DC_Point const *start, DC_Point const *end, int row_bytes, unsigned char const *rasters);
typedef void (ACTION_CALL * Draw_DC_Gray8_Rasters_Action)(Net_Rendition const & nr, DC_Point const *start, DC_Point const *end, int row_bytes, unsigned char const *rasters);
typedef void (ACTION_CALL * Draw_DC_Mapped8_Rasters_Action)(Net_Rendition const & nr, DC_Point const *start, DC_Point const *end,int row_bytes, unsigned char const *rasters);
typedef void (ACTION_CALL * Draw_DC_Mapped16_Rasters_Action)(Net_Rendition const & nr, DC_Point const *start, DC_Point const *end,int row_bytes, unsigned short const *rasters);
typedef void (ACTION_CALL * Draw_DC_RGB32_Rasters_Action)(Net_Rendition const & nr, DC_Point const *start, DC_Point const *end,int row_bytes, RGBAS32 const *rasters);

typedef void (ACTION_CALL * Draw_DC_Spans_Action)(Net_Rendition const & nr, Span_Appearance const & sr, Span const *span, int num_spans, int top);
typedef void (ACTION_CALL * Draw_DC_Colorized_Spans_Action)(Net_Rendition const & nr, Span_Appearance const & sr, Span const *span, int num_spans, int top);
typedef void (ACTION_CALL * Draw_DC_Gouraud_Spans_Action)(Net_Rendition const & nr, Span_Appearance const & sr, Span const *span, int num_spans, int top);
typedef void (ACTION_CALL * Draw_DC_Reshaded_Spans_Action)(Net_Rendition const & nr, Span_Appearance const & sr, Span const *span, int num_spans, int top, int32_t param_flags);

typedef void (ACTION_CALL * Draw_Window_Action)(Net_Rendition const & nr, IntRectangle const *extent);
typedef void (ACTION_CALL * Draw_Window_Frame_Action)(Net_Rendition const & nr, IntRectangle const *extent, IntRectangle *frame, bool calculate_only);

typedef void (ACTION_CALL * Draw_3D_Cylinder_Action)(Net_Rendition const & nr, Cylinder const *cyl);
typedef void (ACTION_CALL * Draw_3D_Ellipse_Action)(Net_Rendition const & nr, Ellipse const *el);
typedef void (ACTION_CALL * Draw_3D_Elliptical_Arc_Action)(Net_Rendition const & nr, Elliptical_Arc const *ela);
typedef void (ACTION_CALL * Draw_3D_Grid_Action)(Net_Rendition const & nr, Grid const *gr);
typedef void (ACTION_CALL * Draw_3D_Image_Action)(Net_Rendition const & nr, Image const *img);
typedef void (ACTION_CALL * Draw_3D_Light_Action)(Net_Rendition const & nr, Light const *el);
typedef void (ACTION_CALL * Draw_3D_Marker_Action)(Net_Rendition const & nr, Marker const *m);
typedef void (ACTION_CALL * Draw_3D_Multimarker_Action)(Net_Rendition const & nr, Multimarker const *mm);
typedef void (ACTION_CALL * Draw_3D_NURBS_Curve_Action)(Net_Rendition const & nr, NURBS_Curve const *crv);
typedef void (ACTION_CALL * Draw_3D_NURBS_Surface_Action)(Net_Rendition const & nr, NURBS_Surface const *srf);
typedef void (ACTION_CALL * Draw_3D_PolyCylinder_Action)(Net_Rendition const & nr, PolyCylinder const *pcyl);
typedef void (ACTION_CALL * Draw_3D_Polygon_Action)(Net_Rendition const & nr, Polygon const *pg);
typedef void (ACTION_CALL * Draw_3D_Polyhedron_Action)(Net_Rendition const & nr, Polyhedron const *ph);
typedef void (ACTION_CALL * Draw_3D_Polyline_Action)(Net_Rendition const & nr, Polyline const *pl);
typedef void (ACTION_CALL * Draw_3D_Sphere_Action)(Net_Rendition const & nr, Sphere const *s);
typedef void (ACTION_CALL * Draw_3D_Text_Action)(Net_Rendition const & nr, Text const *tx);

typedef void (ACTION_CALL * Draw_3D_Geometry_Action)(Net_Rendition const & nr, Geometry const *g);

typedef void (ACTION_CALL * Draw_3D_Tristrip_Action)(Net_Rendition const & nr, Tristrip const *ts);
typedef void (ACTION_CALL * Draw_3D_Polyedge_Action)(Net_Rendition const & nr, Polyedge const *pe);
typedef void (ACTION_CALL * Draw_3D_Polymarker_Action)(Net_Rendition const & nr, Polymarker const *pm);
typedef void (ACTION_CALL * Draw_3D_Polyedge_From_Tristrip_Action)(Net_Rendition const & nr, Polyedge const *pe, Tristrip const *ts);

typedef void (ACTION_CALL * Draw_3D_Contour_Action)(Net_Rendition const & nr, Contour const *con);
typedef void (ACTION_CALL * Draw_3D_Stenciled_Quad_Action)(Net_Rendition const & nr, Point const *pts, Stencil_Bitmap const *sb);
typedef void (ACTION_CALL * Draw_3D_Text_Strokes_Action)(Net_Rendition const & nr, Polyline const *pl);
typedef void (ACTION_CALL * Draw_3D_Text_Areas_Action)(Net_Rendition const & nr, Polygon const *pg);

typedef void (ACTION_CALL * Draw_Segment_Action)(Net_Rendition const & nr, Segment_Info const *info);
typedef void (ACTION_CALL * Draw_Segment_Tree_Action)(Net_Rendition const & nr, Segment_Info const *info);

typedef void (ACTION_CALL * Get_Actions_Action)(Action_Table & at);

typedef void (ACTION_CALL * Driver_Startup_Action)(Net_Rendition const & nr);
typedef void (ACTION_CALL * Driver_Shutdown_Action)(Net_Rendition const & nr);

typedef void (ACTION_CALL * Init_Picture_Action)(Net_Rendition const & nr);
typedef void (ACTION_CALL * Postprocess_Picture_Action)(Net_Rendition const & nr);
typedef void (ACTION_CALL * Finish_Picture_Action)(Net_Rendition const & nr, bool swap_buffers);

typedef void (ACTION_CALL * Create_Region_Action)(Net_Rendition const & nr, IntRectangle const *area,void * *image_buffer, void * *z_buffer);
typedef void (ACTION_CALL * Save_Region_Action)(Net_Rendition const & nr, IntRectangle const *area, void *image_buffer, void *z_buffer);
typedef void (ACTION_CALL * Restore_Region_Action)(Net_Rendition const & nr, IntRectangle const *area, void *image_buffer, void *z_buffer);
typedef void (ACTION_CALL * Destroy_Region_Action)(Net_Rendition const & nr, void *image_buffer, void *z_buffer);

typedef bool (ACTION_CALL * Update_Action)(Display_Context * dc, Activity update_activity);

typedef void (ACTION_CALL * Draw_Tree_Action)(Net_Rendition const & nr, Segment const *seg, Include_Path const & path);
typedef Geometry const * (ACTION_CALL * Render_Action)(Net_Rendition const & nr, Geometry const *geom, uint32_t mask, bool single);

typedef bool (ACTION_CALL * Select_Action)(Display_Context * dc, Selection_Event *se);

typedef bool (ACTION_CALL * Start_Device_Action)(Display_Context * dc);
typedef void (ACTION_CALL * Stop_Device_Action)(Display_Context * dc);

typedef void (ACTION_CALL * Get_Physical_Info_Action)(Display_Context * dc);
typedef void (ACTION_CALL * Get_Current_Info_Action)(Display_Context * dc);

typedef void (ACTION_CALL * Init_Update_Action)(Display_Context const * dc);
typedef void (ACTION_CALL * Finish_Update_Action)(Display_Context const * dc);

typedef void (ACTION_CALL * HWVS_Init_Render_Action)(Net_Rendition const & nr);
typedef void (ACTION_CALL * HWVS_Finish_Render_Action)(Net_Rendition const & nr);

typedef intptr_t (ACTION_CALL * Get_Outer_Window_Action)(Display_Context const * dc);
typedef void (ACTION_CALL * Free_Outer_Window_Action)(Display_Context const * dc);
typedef void (ACTION_CALL * Resize_Outer_Window_Action)(Display_Context const * dc);
typedef void (ACTION_CALL * Retitle_Outer_Window_Action)(Display_Context const * dc);

typedef bool (ACTION_CALL * Measure_Char_Action)(Net_Rendition const & nr, Font_Instance const *font_instance, unsigned short karacter,float *size_out, bool *missing, void ** per_char_data);

typedef void (ACTION_CALL * Draw_Font_Action)(Net_Rendition const & nr, Font_Instance const *font_instance, Point const *position, int count, unsigned short const *karacter, Vector const *scale);
typedef bool (ACTION_CALL * Name_Font_Action)(Net_Rendition const & nr, Font_Name const *font_name_in, Font_Name *font_name_out);
typedef void (ACTION_CALL * Size_Font_Action)(Net_Rendition const & nr, Font_Instance *font_instance);
typedef bool (ACTION_CALL * Will_Use_Stencil_Font_Action)(Net_Rendition const & nr, Font_Instance const *font_instance);
typedef unsigned short (ACTION_CALL * Map_Font_Char_Index_Action)(Font_Instance const *font_instance, unsigned short count);
typedef Stencil_Bitmap * (ACTION_CALL * Make_Font_Stencil_Action)(Net_Rendition const & nr, Font_Instance const *font_instance, unsigned short mapped_index,int bitmap_height, int bolding_level, float slant, float rotation, float width_scale, bool antialias);

typedef void (ACTION_CALL * Draw_Inter_Text_Action)(Net_Rendition const & nr, Text_Info const *text_info);
typedef void (ACTION_CALL * Draw_Plain_Text_Action)(Net_Rendition const & nr, Text const * text, Point const * position,int count, unsigned short const * kp, Text_Karacter_Info const * kinfo);

typedef void (ACTION_CALL * Toggle_XOR_Action)(Net_Rendition const & nr, bool undrawing);

typedef bool (ACTION_CALL * Request_Location_Action)(Display_Context const * dc, int *button, int *x, int *y);
typedef bool (ACTION_CALL * Request_Keyboard_Action)(Display_Context const * dc, int *button, int *status);
typedef void (ACTION_CALL * Set_Device_Color_Map_Action)(Display_Context const * dc, unsigned int r, unsigned int g, unsigned int b, int index);

typedef bool (ACTION_CALL * Find_One_Font_Action)(Display_Context * dc, char const *name);
typedef void (ACTION_CALL * Find_Several_Fonts_Action)(Display_Context * dc);
typedef void * (ACTION_CALL * Load_Font_Action)(Display_Context const * dc, char const *name, Font_Instance *instance);
typedef void (ACTION_CALL * Unload_Font_Action)(Display_Context const * dc, Font_Instance *instance);

typedef void (ACTION_CALL * Unload_All_Fonts_Action)(Net_Rendition const & nr);

typedef void (ACTION_CALL * Clear_Z_Buffer_Action)(Net_Rendition const & nr, IntRectangle const & area);

typedef void (ACTION_CALL * Start_Inclusion_Action)(Net_Rendition const & nr, int32_t priority);
typedef void (ACTION_CALL * Finish_Inclusion_Action)(Net_Rendition const & nr);

typedef void (ACTION_CALL * Flush_Display_Lists_Action)(Display_Context const * dc);

typedef void (ACTION_CALL * Show_Image_Data_Action)(Display_Context const * dc, void * id);

typedef void * (ACTION_CALL * Begin_Occlusion_Query_Action)(Net_Rendition const & nr, bool disable_write);
typedef void (ACTION_CALL * End_Occlusion_Query_Action)(Net_Rendition const & nr, void * id);
typedef int (ACTION_CALL * Get_Occlusion_Query_Action)(Net_Rendition const & nr, void * id);
typedef void (ACTION_CALL * Delete_Occlusion_Query_Action)(Net_Rendition const & nr, void * id);

typedef void (ACTION_CALL * Create_Frame_Buffer_Action)(Net_Rendition const & nr, int width, int height, Image_Format format, Image * *image, void * *driver_specific);
typedef void (ACTION_CALL * Display_Frame_Buffer_Action)(Net_Rendition const & nr, DC_Point const *start, DC_Point const *end, Image const *image, int hoffset, int voffset, void const *driver_specific);
typedef void (ACTION_CALL * Destroy_Frame_Buffer_Action)(Net_Rendition const & nr, Image *buffer, void *driver_specific);

typedef bool (ACTION_CALL * Exit_Update_Function) (Net_Rendition const & nr, void const *data);

typedef bool (ACTION_CALL * Snapshot_Action)(Net_Rendition const & nr, int width, int height, unsigned char *data);

typedef void (ACTION_CALL * Depth_Peeling_Action)(Net_Rendition const & nr);
typedef void (ACTION_CALL * Simple_Shadow_Draw_Tree_Action)(Net_Rendition const & nr, Segment const *seg, Include_Path const & path);
typedef void (ACTION_CALL * Shadow_Map_Draw_Tree_Action)(Net_Rendition const & nr, Segment const *seg, Include_Path const & path);
typedef void (ACTION_CALL * Reflection_Plane_Draw_Tree_Action)(Net_Rendition const & nr, Segment const *seg, Include_Path const & path, float opacity, float radius);

typedef bool (ACTION_CALL * Begin_Display_List_Action)(Net_Rendition const & nr, Display_List **dll, uint32_t *mask, bool compile_only);
typedef void (ACTION_CALL * Check_Display_List_Action)(Net_Rendition const & nr, Display_List **dll, uint32_t *mask, uint32_t *viz_added);
typedef void (ACTION_CALL * End_Display_List_Action)(Net_Rendition const & nr, Display_List **dll, bool compile_only);
typedef void (ACTION_CALL * Execute_Display_List_Action)(Net_Rendition const & nr, Display_List const *dll);

typedef void (ACTION_CALL * Downgrade_Segdl_Action)(Display_Context const * dc, Display_List * dl);

typedef void * (ACTION_CALL * IM_Utility_Action)(Net_Rendition const & nr, long request, void *pointer,intptr_t v1,  intptr_t v2,  intptr_t v3, intptr_t v4, float f1, float f2, float f3, float f4);


struct Highlight_Map {
	enum { SIZE = 1024 };
	enum { MAX_BITS = SIZE * SIZE * 32 };

	Display_List *					display_list;			// Texture with one bit per subgeometry ID
};

#define Highlight_Map_FRESH		0	// clear any old highlight then ADD
#define Highlight_Map_ADD		1	// set bits for specified items
#define Highlight_Map_REMOVE	2	// clear bits for specified items


enum Action_Mask_Flags : Action_Mask {

	// segdl-able
	Action_Mask_DRAW_3D_TRISTRIP					= 0x00000001,
	Action_Mask_DRAW_INDEXED_TRISTRIP				= 0x00000002,
	Action_Mask_DRAW_3D_POLYEDGE					= 0x00000004,
	Action_Mask_DRAW_INDEXED_POLYEDGE				= 0x00000008,
	Action_Mask_DRAW_3D_POLYLINE					= 0x00000010,
	Action_Mask_DRAW_3D_ELLIPTICAL_ARC				= 0x00000020,
	Action_Mask_DRAW_3D_NURBS_CURVE					= 0x00000040,
	Action_Mask_DRAW_3D_POLYMARKER					= 0x00000080,
	Action_Mask_DRAW_3D_POLYGON						= 0x00000100,
	Action_Mask_DRAW_3D_ELLIPSE						= 0x00000200,
	Action_Mask_DRAW_3D_MARKER						= 0x00000400,
	Action_Mask_DRAW_3D_ISOLINE						= 0x00000800,

	// non-segdl-able
	Action_Mask_DRAW_3D_STENCILED_QUAD				= 0x00001000,
	Action_Mask_DRAW_3D_GRID						= 0x00002000,
	Action_Mask_DRAW_3D_INFINITE_LINE				= 0x00004000,
	Action_Mask_DRAW_3D_CONTOUR						= 0x00008000,
	Action_Mask_DRAW_3D_IMAGE						= 0x00010000,
	Action_Mask_DRAW_3D_TEXT						= 0x00020000,
	Action_Mask_DRAW_3D_LIGHT						= 0x00040000,
	Action_Mask_DRAW_SILHOUETTE_POLYEDGE			= 0x00080000,
	Action_Mask_DRAW_NONCULLED_POLYEDGE				= 0x00100000,
	Action_Mask_DRAW_3D_POLYEDGE_FROM_TRISTRIP		= 0x00200000,
// unused											= 0x00400000,
// unused											= 0x00800000,
	Action_Mask_DRAW_TREE							= 0x01000000,
	Action_Mask_RENDER								= 0x02000000,
// unused											= 0x04000000,
// unused											= 0x08000000,
	Action_Mask_DRAW_3D_CYLINDER					= 0x10000000, // also polycylinders and spheres
	Action_Mask_DRAW_3D_POLYCYLINDER				= Action_Mask_DRAW_3D_CYLINDER,
	Action_Mask_DRAW_3D_SPHERE						= Action_Mask_DRAW_3D_CYLINDER,
	Action_Mask_DRAW_3D_NURBS_SURFACE				= 0x20000000,
// unused											= 0x40000000,
	Action_Mask_DRAW_3D_POLYHEDRON					= 0x80000000,

	Action_Mask_ALL									= 0xFFFFFFFF,


	Action_Mask_ANY_POLYLINE = (Action_Mask_DRAW_3D_POLYLINE | Action_Mask_DRAW_3D_ELLIPTICAL_ARC | Action_Mask_DRAW_3D_NURBS_CURVE),

	Action_Mask_ANY_TRISTRIP = (Action_Mask_DRAW_3D_TRISTRIP | Action_Mask_DRAW_INDEXED_TRISTRIP),

	Action_Mask_ANY_POLYEDGE = (
			Action_Mask_DRAW_3D_POLYEDGE |
			Action_Mask_DRAW_INDEXED_POLYEDGE |
			Action_Mask_DRAW_SILHOUETTE_POLYEDGE |
			Action_Mask_DRAW_NONCULLED_POLYEDGE |
			Action_Mask_DRAW_3D_POLYEDGE_FROM_TRISTRIP),

	Action_Mask_ANY_POLYHEDRON_DERIVATIVE = (
			Action_Mask_ANY_TRISTRIP |
			Action_Mask_ANY_POLYEDGE |
			Action_Mask_DRAW_3D_POLYMARKER |
			Action_Mask_DRAW_3D_ISOLINE),

	Action_Mask_ANY_HIGHER_ORDER = (Action_Mask_DRAW_3D_CYLINDER | Action_Mask_DRAW_3D_NURBS_SURFACE | Action_Mask_DRAW_3D_POLYHEDRON),

	Action_Mask_ANY_POLYGON = (Action_Mask_DRAW_3D_GRID | Action_Mask_DRAW_3D_POLYGON | Action_Mask_DRAW_3D_ELLIPSE),

	Action_Mask_DISPLAY_LIST_ALL = (
			Action_Mask_DRAW_3D_POLYMARKER |
			Action_Mask_DRAW_3D_MARKER |
			Action_Mask_DRAW_3D_POLYEDGE |
			Action_Mask_DRAW_INDEXED_POLYEDGE |
			Action_Mask_ANY_TRISTRIP |
			Action_Mask_DRAW_3D_POLYGON |
			Action_Mask_DRAW_3D_ELLIPSE |
			Action_Mask_ANY_POLYLINE),

	Action_Mask_ANY_GEOMETRY = (Action_Mask_ALL & ~(Action_Mask_DRAW_TREE|Action_Mask_RENDER)),
};



/* be sure to add to get actions */
struct Internal_Action_Table : public Counted_Object_Pooled {
	friend class Counted_Pointer_Pooled<Internal_Action_Table>;

private:
	HOOPS_API Internal_Action_Table (Memory_Pool *p);

public:

	Action_Table						prev;
	char const *						id;
	Drawing_Mode						mode;
	Action_Mask							mask;

	Start_Device_Action						start_device;
	Stop_Device_Action						stop_device;
	Get_Physical_Info_Action				get_physical_info;
	Get_Current_Info_Action					get_current_info;

	Update_Action							update;
	Draw_Tree_Action						draw_tree;
	Render_Action							render;
	Select_Action							select;

	Init_Update_Action						init_update;
	Finish_Update_Action					finish_update;

	HWVS_Init_Render_Action					hwvs_init_render;
	HWVS_Finish_Render_Action				hwvs_finish_render;

	Init_Picture_Action						init_picture;
	Postprocess_Picture_Action				postprocess_picture;
	Finish_Picture_Action					finish_picture;

	Get_Outer_Window_Action					get_outer_window;
	Free_Outer_Window_Action				free_outer_window;
	Resize_Outer_Window_Action				resize_outer_window;
	Retitle_Outer_Window_Action				retitle_outer_window;

	Draw_3D_Ellipse_Action					draw_3d_ellipse;
	Draw_3D_Elliptical_Arc_Action			draw_3d_elliptical_arc;
	Draw_3D_Grid_Action						draw_3d_grid;
	Draw_3D_Marker_Action					draw_3d_marker;
	Draw_3D_Multimarker_Action				draw_3d_multimarker;
	Draw_3D_Polyline_Action					draw_3d_polyline;
	Draw_3D_Polyline_Action					draw_3d_infinite_line;
	Draw_3D_NURBS_Curve_Action				draw_3d_nurbs_curve;
	Draw_3D_NURBS_Surface_Action			draw_3d_nurbs_surface;
	Draw_3D_Cylinder_Action					draw_3d_cylinder;
	Draw_3D_PolyCylinder_Action				draw_3d_polycylinder;
	Draw_3D_Sphere_Action					draw_3d_sphere;
	Draw_3D_Polygon_Action					draw_3d_polygon;
	Draw_3D_Contour_Action					draw_3d_contour;
	Draw_3D_Image_Action					draw_3d_image;
	Draw_3D_Polyhedron_Action				draw_3d_polyhedron;
	Draw_3D_Text_Action						draw_3d_text;
	Draw_3D_Light_Action					draw_3d_light;

	Draw_3D_Stenciled_Quad_Action			draw_3d_stenciled_quad;

	Draw_3D_Polyedge_Action					draw_3d_polyedge;
	Draw_3D_Tristrip_Action					draw_3d_tristrip;
	Draw_3D_Polymarker_Action				draw_3d_polymarker;

	Draw_3D_Polyedge_Action					draw_silhouette_polyedge;
	Draw_3D_Polyedge_Action					draw_nonculled_polyedge;
	Draw_3D_Polyedge_From_Tristrip_Action	draw_3d_polyedge_from_tristrip;

	Draw_3D_Polyedge_Action					draw_indexed_polyedge;
	Draw_3D_Tristrip_Action					draw_indexed_tristrip;

	Draw_Window_Action						draw_window;
	Draw_Window_Frame_Action				draw_window_frame;

	Draw_DC_Rectangle_Action				draw_dc_rectangle;

	Draw_DC_Polytriangle_Action				draw_dc_polytriangle;
	Draw_DC_Colorized_Polytriangle_Action	draw_dc_colorized_polytriangle;
	Draw_DC_Gouraud_Polytriangle_Action		draw_dc_gouraud_polytriangle;
	Draw_DC_Phong_Polytriangle_Action		draw_dc_phong_polytriangle;
	Draw_DC_Textured_Polytriangle_Action	draw_dc_textured_polytriangle;
	Draw_DC_Polyline_Action					draw_dc_polyline;
	Draw_DC_Colorized_Polyline_Action		draw_dc_colorized_polyline;
	Draw_DC_Gouraud_Polyline_Action			draw_dc_gouraud_polyline;
	Draw_DC_Phong_Polyline_Action			draw_dc_phong_polyline;
	Draw_DC_Textured_Polyline_Action		draw_dc_textured_polyline;
	Draw_DC_Polymarker_Action				draw_dc_polymarker;
	Draw_DC_Colorized_Polymarker_Action		draw_dc_colorized_polymarker;
	Draw_DC_Polydot_Action					draw_dc_polydot;
	Draw_DC_Colorized_Polydot_Action		draw_dc_colorized_polydot;

	Draw_DC_Polygon_Action					draw_dc_polygon;
	Draw_DC_Face_Action						draw_dc_face;
	Draw_DC_Colorized_Face_Action			draw_dc_colorized_face;

	Draw_DC_Ellipse_Action					draw_dc_ellipse;
	Draw_DC_Elliptical_Arc_Action			draw_dc_elliptical_arc;
	Draw_DC_Outline_Ellipse_Action			draw_dc_outline_ellipse;

	Draw_DC_Image_Action					draw_dc_image;
	Draw_DC_Text_Action						draw_dc_text;
	Draw_DC_Stenciled_Quad_Action			draw_dc_stenciled_quad;

	Draw_DC_Spans_Action					draw_dc_spans;
	Draw_DC_Colorized_Spans_Action			draw_dc_colorized_spans;
	Draw_DC_Gouraud_Spans_Action			draw_dc_gouraud_spans;
	Draw_DC_Reshaded_Spans_Action			draw_dc_reshaded_spans;

	Draw_DC_Bit_Rasters_Action				draw_dc_bit_rasters;
	Draw_DC_Gray8_Rasters_Action			draw_dc_gray8_rasters;
	Draw_DC_Mapped8_Rasters_Action			draw_dc_mapped8_rasters;
	Draw_DC_Mapped16_Rasters_Action			draw_dc_mapped16_rasters;
	Draw_DC_RGB32_Rasters_Action			draw_dc_rgb32_rasters;

	Draw_Inter_Text_Action					draw_inter_text;
	Draw_Plain_Text_Action					draw_plain_text;

	Toggle_XOR_Action						toggle_xor;

	Request_Location_Action					request_location;
	Request_Keyboard_Action					request_keyboard;

	Set_Device_Color_Map_Action				set_device_color_map;

	Find_Several_Fonts_Action				find_basic_fonts;
	Find_Several_Fonts_Action				find_all_fonts;
	Find_One_Font_Action					find_one_font;
	Load_Font_Action						load_font;
	Unload_Font_Action						unload_font;
	Unload_All_Fonts_Action					unload_all_fonts;
	Measure_Char_Action						measure_char;
	Draw_Font_Action						draw_font;
	Name_Font_Action						name_font;
	Map_Font_Char_Index_Action				map_font_char_index;
	Size_Font_Action						size_font;
	Will_Use_Stencil_Font_Action			will_use_stencil;
	Make_Font_Stencil_Action				make_font_stencil;

	Clear_Z_Buffer_Action					clear_z_buffer;

	Start_Inclusion_Action					start_inclusion;
	Finish_Inclusion_Action					finish_inclusion;

	Create_Frame_Buffer_Action				create_frame_buffer;
	Display_Frame_Buffer_Action				display_frame_buffer;
	Destroy_Frame_Buffer_Action				destroy_frame_buffer;

	Flush_Display_Lists_Action				flush_display_lists;

	Show_Image_Data_Action					show_image_data;

	Begin_Occlusion_Query_Action			begin_occlusion_query;
	End_Occlusion_Query_Action				end_occlusion_query;
	Get_Occlusion_Query_Action				get_occlusion_query;
	Delete_Occlusion_Query_Action			delete_occlusion_query;

	Create_Region_Action					create_region;
	Save_Region_Action						save_region;
	Restore_Region_Action					restore_region;
	Destroy_Region_Action					destroy_region;

	Depth_Peeling_Action					depth_peeling;

	Begin_Display_List_Action				begin_display_list;
	Check_Display_List_Action				check_display_list;
	End_Display_List_Action					end_display_list;
	Execute_Display_List_Action				execute_display_list;

	Downgrade_Segdl_Action					downgrade_segdl;

	Snapshot_Action							snapshot;
	IM_Utility_Action						im_utility;

	Shadow_Map_Draw_Tree_Action				shadow_map_draw_tree;
	Simple_Shadow_Draw_Tree_Action			simple_shadow_draw_tree;
	Reflection_Plane_Draw_Tree_Action		reflection_plane_draw_tree;

};
/* be sure to add to get actions */



struct Net_Driver_Options {

	Driver_Options::Drivo_Bits	bits;

	bool					backing_store;
	bool					border;
	bool					control_area;

	bool					disable_input;
	bool					disable_all_input;
	bool					double_buffering;

	bool					force_black_and_white;
	bool					force_grayscale;
	bool					gamma_correction;

	bool					special_events;
	bool					subscreen_moving;
	bool					subscreen_resizing;
	bool					subscreen_stretching;

	bool					subscreen_by_program;
	bool					text_prefers_outline;
	bool					stereo_requested;
	bool					anti_alias_requested;
	bool					absolute_line_weight;
	bool					hidden_window;

	bool					marker_drawing_fastest;
	bool					fast_silhouette_edges;
	bool					fast_silhouette_heavy_exterior;
	bool					fast_ambient_occlusion;
	bool					fast_ambient_occlusion_nice;
	bool					bloom;
	bool					depth_of_field;
	bool					eye_dome_lighting;

	int						debug;

	int						first_color;
	int						fixed_colors;
	int						fixed_colors_parts[3];

	int						landscape_orientation;
#ifndef Drivo_Landscape_OFF
#	define	  Drivo_Landscape_OFF				0
#	define	  Drivo_Landscape_ON				1
#	define	  Drivo_Landscape_ENVIRONMENT		2
#endif

	float					light_scaling;
	float					window_opacity;
	float					color_consolidation;
	float					locater_transform[3][2];
	int32_t					number_of_colors;
	float					pen_speed;
	Vector2D				physical_size;
	float					selection_proximity;

	Rectangle				subscreen;

	Name					title;
	Name					display_stats_file;

	Font_Name_List			fallback_font_names;
	Font_Name_List			font_table;
	int						update_interrupts;

	intptr_t				use_window_ID;
	intptr_t				use_window_ID2;
	intptr_t				file_descriptor;
	intptr_t				use_colormap_ID;
	intptr_t				use_clip_override;
	int						window_id_type;

	int						write_mask;
	Name_List *				output_format2;

	int						hardcopy_resolution;
	int						qmoves_preferences[LAST_QMOVES];
	int						shadow_preference;

	int						anti_alias_samples;

	int						draw_degenerate_lines;

	float					fast_silhouette_tolerance;

	float					fast_ambient_occlusion_strength;

	int						display_interrupted_update_mode;

	int						bloom_blur_passes;
	int						bloom_shape;
	float					bloom_strength;

	float					eye_dome_lighting_exponent;
	float					eye_dome_lighting_tolerance;
	float					eye_dome_lighting_radius;
	float					eye_dome_lighting_strength;

	float					depth_of_field_near;
	float					depth_of_field_far;
	float					depth_of_field_strength;
	Exit_Update_Function	exit_update;
	void *					exit_update_data;
};

struct Event_State {
	int32_t const *	which_buttons;
	bool			mouse_up;
	bool			mouse_down;
	bool			mouse_moved;
	bool			buttons;
};

enum Deferred_Tree_Progress {
	Deferred_Progress_Setup,
	Deferred_Progress_Draw_Tree
};

struct Deferred_Draw_Tree {
	Deferred_Draw_Tree *	next;
	Segment const *			segment;
	Include_Path			path;
	Geometry const *		geometry;
	Deferred_Draw_Tree *	active;
	Frame_Buffer_Info *		fbi;
	Net_Rendition			net_rendition;
	Activity				activity;
	int						stage;
	Drawing_Mode			save_pending;
	float					value;
	bool					quickmoves_checked;
};


struct Deferred_Tree_List {
	Deferred_Draw_Tree *	list;
	Deferred_Draw_Tree **	linker;
	int						batch;
};

typedef std::vector<Deferred_Tree_List *, POOL_Allocator<Deferred_Tree_List *> > Deferred_Tree_List_Heap;

struct Deferred_Tree_List_Heap_Compare
{
	bool operator()(const Deferred_Tree_List * a, const Deferred_Tree_List * b)
	{
		return a->batch > b->batch;
	}
};

typedef VHash<int, Deferred_Tree_List *> Deferred_Tree_List_Int_Hash;


#define Deferred_WINDOW			0
#define Deferred_GEOMETRY		1
#define Deferred_2D				2
#define Deferred_COUNT			3
#define Deferred_Batches_MAX	10


struct Font_Hash {
	Font_Hash *		hash_next;
	Name			name;
	short			type;

#	define		FH_FAMILY		0
#	define		FH_INSTANCE		1
};

/* identifiers for built in fonts */
#define Builtin_MONO			1L
#define Builtin_SIMPLEX			2L
#define Builtin_DUPLEX			3L

/* font types */
#define Font_Type_DRIVER				0
#define Font_Type_FREETYPE				1
#define Font_Type_DEFINED				2

#define FONT_TYPE_COUNT					3

struct Font_Family : public Font_Hash {	/* name = nickname w/trailing \0 */
	Font_Family *			next;
	Font const *			font;
	Font_Instance const *	members;
	int						family_type;
	bool					scaleable;
	bool					unnamed_members_exist;
};

#define FONT_INSTANCE_WIDTH_COUNT 32

struct Font_Instance_Char_Data {
	/*data is a non-owning reference*/
	void *					data;
	Font_Instance const *	fall_font;

	int valid;

	int bold;

	float measured_width; /* basic char Aheight */
	float measured_width_current_Aheight; /* Aheight of measured_width */

	Karacter karacter;
	Karacter fall_karacter;

};

struct Font_Instance : public Font_Hash {		/* name = full explicit name w/ \0 */
	Font_Instance *			next_in_family;		/* sorted by height */
	Font_Instance *			next_in_generic;	/* sorted by height */
	Font_Instance const *	next;				/* MRU order */

	Font_Family const *		family;
	Font_Instance const *	base_instance;
	void *					identifier;		/* valid when loaded */

#define FONT_INSTANCE_CHAR_DATA_COUNT 256
#define KARACTER_2_CHAR_DATA_INDEX(k) (k&0xFF)
	Font_Instance_Char_Data *		char_data;


	Load_Font_Action				load_font;
	Unload_Font_Action				unload_font;
	Measure_Char_Action				measure_char;
	Draw_Font_Action				draw_font;
	Map_Font_Char_Index_Action		map_font_char_index;
	Size_Font_Action				size_font;
	Make_Font_Stencil_Action		make_font_stencil;
	Will_Use_Stencil_Font_Action	will_use_stencil;

	float							Aheight;	/* height of letter A */
	float							vspace;		/* nominal vertical spacing */


	float							correction;	/* post load size correction */
	float							current_Aheight; /* updated in draw 3d text */
	float							current_Aheight_fixup;
	bool							current_using_stencil;

	bool							i_am_in_make_font_stencil_via_measure_char;

	bool							loaded;			/* ready for drawing */
	bool							dead;			/* don't try to load this */
	bool							draw_stenciled; /* decided by load font */
	Generic_Font_Name				generic_name;

#define TT_NEVER				0x00

#define TT_90_DEGREE			0x01	/* basic forms */
#define TT_ANY_ANGLE			0x02
#define TT_WIDTH				0x04
#define TT_SLANT				0x08
#define TT_3D					0x10
#define TT_PER_CHAR				0x20	/* do any transforms 1 char at a time */

#define TT_ANY					0x0E	/* standard combinations */
#define TT_ANY_3D				0x1E
	int								transformable;

	Name							alias_name;
	long							face_index;
	Text_Encoding					encoding;		/* encoding of this font */
};

struct Freetype_Font {
	void *						v_face;
	outline_cache_t *			outline_cache;
	int							load_flag;
	int							offset;	
	int							face_last_size;
	Name						full_name;
	Name						postscript_name;
	Reference_Count				refcount;
	unsigned short				test_index;
};

struct Freetype_Globals {
	void *			library;
	void *			name_cache;
};


struct Polygon_Collector {
	int		count;
	int *	points;
};


struct Cut_Geometry_Collector {
	Cut_Geometry_Collector *next;
	Cut_Geometry_Pieces *	cut_geometry_pieces;
	Segment const *			segment;
	Include_Path			path;

	bool					seen_cut_geometry_colors;
	bool					use_cut_geometry_color;
	Driver_Color			cut_geometry_face_color;
	Driver_Color			cut_geometry_line_color;
};

typedef VXMap<Segment const *, Cut_Geometry_Collector *, Hasher<Segment const *>,
	std::equal_to<Segment const *>, POOL_Allocator<std::pair<Segment const * const, Cut_Geometry_Collector *> > > Cut_Collector_Map;



enum Deferred_Item_Flags : uint32_t {

	DI_NONE					= 0x00000000,
	DI_SINGLE				= 0x00000001,
	DI_GEOMETRY				= 0x01000000,
	DI_TRISTRIP				= 0x02000000,
	DI_POLYEDGE				= 0x04000000,
	DI_POLYMARKER			= 0x08000000,
	DI_DISPLAY_LIST			= 0x10000000,

	DI_STRIP_COMMON_MASK = (DI_TRISTRIP|DI_POLYEDGE|DI_POLYMARKER),
	DI_ANYTHING_MASK = (DI_GEOMETRY|DI_DISPLAY_LIST),
};

template<> struct _bit_ops_type < Deferred_Item_Flags > {
	typedef Deferred_Item_Flags type;
};

#if 0
if (ANYBIT(item.flags, DI_STRIP_COMMON_MASK)) {
	if (ANYBIT(item.flags, DI_TRISTRIP)) {

	}
	else if (ANYBIT(item.flags, DI_POLYEDGE)) {

	}
	else {
		ASSERT(ANYBIT(item.flags, DI_POLYMARKER);

	}

}
else if (ANYBIT(item.flags, DI_DISPLAY_LIST)) {

}
else {
	ASSERT(ANYBIT(item.flags, DI_GEOMETRY));
}
#endif

class Deferred_Item {

public:
	Net_Rendition				nr;
	Include_Path				path;
	Segment const *				segment;
	Geometry const *			geometry_in_progress;

	Modelling_Matrix const *	mm;
	Color const *				color;

private:
	union {
		Anything const *		anything;
		Geometry const *		geometry;
		Display_List const *	display_list;
		Strip_Common const *	common;
		Tristrip const *		tristrip;
		Polyedge const *		polyedge;
		Polymarker const *		polymarker;
	};

	HOOPS_API void own();
	HOOPS_API void disown();
	HOOPS_API void record(bool skip_mm_and_color = false);

public:
	Deferred_Item_Flags			flags;

	INLINE Deferred_Item(
		Net_Rendition const &	nr,
		Geometry const *		geometry,
		Deferred_Item_Flags		flags=DI_SINGLE) : nr(nr), geometry(geometry), flags(flags | DI_GEOMETRY)
	{
		record();
		own();
	}

	INLINE Deferred_Item(
		Net_Rendition const &	nr,
		Display_List const *	display_list) : nr(nr), display_list(display_list), flags(DI_DISPLAY_LIST | DI_SINGLE)
	{
		record();
		own();
	}

	INLINE	Deferred_Item(
		Net_Rendition const &	nr,
		Tristrip const *		tristrip) : nr(nr), tristrip(tristrip), flags(DI_TRISTRIP)
	{
		record();
		own();
	}

	INLINE Deferred_Item(
		Net_Rendition const &	nr,
		Polyedge const *		polyedge) : nr(nr), polyedge(polyedge), flags(DI_POLYEDGE)
	{
		record();
		own();
	}

	INLINE Deferred_Item(
		Net_Rendition const &	nr,
		Polymarker const *		polymarker) : nr(nr), polymarker(polymarker), flags(DI_POLYMARKER)
	{
		record();
		own();
	}

	INLINE	Deferred_Item(
		Net_Rendition const &		nr,
		Tristrip const *			tristrip,
		Modelling_Matrix const *	mm,
		Color const *				color) : nr(nr), tristrip(tristrip), mm(mm), color(color), flags(DI_TRISTRIP)
	{
		record(true);
		own();
	}

	INLINE Deferred_Item(
		Net_Rendition const &		nr,
		Polyedge const *			polyedge,
		Modelling_Matrix const *	mm,
		Color const *				color) : nr(nr), polyedge(polyedge), mm(mm), color(color), flags(DI_POLYEDGE)
	{
		record(true);
		own();
	}


	INLINE Deferred_Item(
		Net_Rendition const &		nr,
		Polymarker const *			polymarker,
		Modelling_Matrix const *	mm,
		Color const *				color) : nr(nr), polymarker(polymarker), mm(mm), color(color), flags(DI_POLYMARKER)
	{
		record(true);
		own();
	}

	INLINE Deferred_Item(Deferred_Item const & that) :
		nr(that.nr),
		path(that.path),
		segment(that.segment),
		geometry_in_progress(that.geometry_in_progress),
		mm(that.mm),
		color(that.color),
		geometry(that.geometry), // could be a strip common
		flags(that.flags)
	{
		//	ASSERT(0); this happens if move behaviour is not working
		own();
	}

	INLINE Deferred_Item(Deferred_Item const &	that, Geometry const * geometry) :
		nr(that.nr),
		path(that.path),
		segment(that.segment),
		geometry_in_progress(that.geometry_in_progress),
		mm(that.mm),
		color(that.color),
		geometry(geometry), // could be a strip common
		flags(that.flags)
	{
	//	ASSERT(0);
		//	ASSERT(0); this happens if move behaviour is not working
		if (geometry != null)
			flags |= DI_SINGLE;

		own();
	}

	INLINE Deferred_Item const & operator= (Deferred_Item const & that)
	{
		disown();

		nr = that.nr;
		path = that.path;
		segment = that.segment;
		geometry_in_progress = that.geometry_in_progress;
		mm = that.mm;
		color = that.color;
		geometry = that.geometry;
		flags = that.flags;

		own();

		return *this;
	}

	INLINE Deferred_Item(Deferred_Item && that) :
		nr(std::move(that.nr)),
		path(std::move(that.path)),
		segment(that.segment),
		geometry_in_progress(that.geometry_in_progress),
		mm(that.mm),
		color(that.color),
		geometry(that.geometry),
		flags(that.flags)
	{
		that.nr = null;
		that.path = null;
		that.segment = null;
		that.geometry_in_progress = null;
		that.mm = null;
		that.color = null;
		that.geometry = null;
		that.flags = DI_NONE;
	}

	INLINE ~Deferred_Item()
	{
		disown();
	}


	INLINE Anything const *		get_anything() const {
		ASSERT(ANYBIT(flags, DI_ANYTHING_MASK));
		return anything;
	}
	INLINE Geometry const *		get_geometry() const {
		ASSERT(ANYBIT(flags, DI_GEOMETRY));
		return geometry;
	}
	INLINE Display_List const *	get_display_list() const {
		ASSERT(ANYBIT(flags, DI_DISPLAY_LIST));
		return display_list;
	}
	INLINE Tristrip const *		get_tristrip() const {
		ASSERT(ANYBIT(flags, DI_TRISTRIP));
		return tristrip;
	}
	INLINE Polyedge const *		get_polyedge() const {
		ASSERT(ANYBIT(flags, DI_POLYEDGE));
		return polyedge;
	}
	INLINE Polymarker const *	get_polymarker() const {
		ASSERT(ANYBIT(flags, DI_POLYMARKER));
		return polymarker;
	}
	INLINE Strip_Common const *	get_strip_common() const {
		ASSERT(ANYBIT(flags, DI_TRISTRIP|DI_POLYEDGE|DI_POLYMARKER));
		return common;
	}

	INLINE bool colors_compatible(Deferred_Item const & that) const {
		return ((color == null) && (that.color == null)) || (color && that.color && (color->objects == that.color->objects));
	}
};

typedef std::vector<Deferred_Item, POOL_Allocator<Deferred_Item> > Deferred_Item_Vector;
typedef std::vector<Deferred_Item const *, POOL_Allocator<Deferred_Item const *> > Deferred_Item_Ptr_Vector;

INLINE Destructible<Deferred_Item>::unique_ptr Construct(Memory_Pool *	memory_pool, Deferred_Item && prototype)
{
	Deferred_Item * item;
	POOL_ALLOC(item, Deferred_Item, memory_pool);
	new(item)Deferred_Item(prototype);
	return Destructible<Deferred_Item>::unique_ptr(item);
}


struct Text_Selection {
	Text const *			current_text;
	String_Cursor const *	current_cursor;
	int						current_offset;
	float					best_prox,
							best_zw;
	int						best_offset;
	Type_Flags				selectables;
};


struct Net_GRef_Attributes_List {
	Net_GRef_Attributes_List *	next;
	Modelling_Matrix const *	modelling_matrix;
	Color const *				color;
};



// structure to hold actions/data that need to be done in the preset or cleanup phases
struct Tree_Process : public CMO_Pooled {
	Tree_Process *			prev;

	Tree_Process () : prev (null) {}
	virtual bool Process (Net_Rendition & nr, bool just_cleanup=false) = 0;
};

// similar, but filters also need to act on the current segment
struct Tree_Filter: public CMO_Pooled {
	Tree_Filter *			prev;

	Tree_Filter () : prev (null) {}
	virtual bool Filter (Net_Rendition & nr, Segment const * seg, Include_Path const & path) = 0;
};


enum Tree_Progress {
	Tree_Progress_Actions,
	Tree_Progress_Filter,
	Tree_Progress_Preset,
	Tree_Progress_Setup,
	Tree_Progress_Preprocess,
	Tree_Progress_Render_Setup,
	Tree_Progress_Render,
	Tree_Progress_Render_Cleanup,
	Tree_Progress_Subsegments,
	Tree_Progress_Postprocess,
	Tree_Progress_Cut_Geometry,
	Tree_Progress_Shadow,
	Tree_Progress_Cleanup,
	Tree_Progress_Done
};

enum Tree_State_Flags {
	Process_Geometry		= 0x0001,
	Segment_Display_List	= 0x0002,
	Recheck_Culling			= 0x0004,
	Cut_Tree_Level			= 0x0008,
//	Shadow_Map_Here			= 0x0010,
	Simple_Reflection_Here	= 0x0020,
	Simple_Shadow_Here		= 0x0040,
	Draw_Simple_Shadow		= 0x0080,
	Simple_Shadow_Setting	= 0x0100,
	Breadth_First			= 0x0200,
	Populator				= 0x0400,
	Static_Root				= 0x0800,

	Have_Priority			= 0x1000
};

struct Reference_Node : public CMO_Pooled {
	public:
		Reference_Node *			next;

		Net_Rendition				nr;
		Geometry_Reference const *	reference;
		Geometry const *			geometry;
		Geometry const *			start;		// saved copy of first 'geometry' value

		Modelling_Matrix const *	modelling_matrix;
		Color const *				color;
#ifdef HOOPS_DEBUG_BUILD
		Modelling_Matrix const *	correct_modelling_matrix;
		Color const *				correct_color;
#endif
		Cut_Geometry_Collector	*	saved_collector;

		Activity					activity;
		int							index;

		bool						local_transparent;
		Tree_Process *				local_quickmoves_cleanup;
		bool						single;

		bool						added_highlights;

		~Reference_Node();
};



#define Highlight_Set_Level_OFF			0x00
#define Highlight_Set_Level_SEGMENT		0x01
#define Highlight_Set_Level_TREE		0x03	// implies segment
#define Highlight_Set_Level_GEOMETRY	0x04

struct Tree_Node : public CMO_Pooled {
	public:
		Tree_Node *					next;

		int							stage;		// Tree_Progress
		int							flags;		// Tree_State_Flags

		Net_Rendition				nr;
		Segment const *				segment;
		Segment_Cache *				cache;
		Bounding					parent_bounding;
		Action_Table				actions;
		Activity					activity;

		Include_Path				path;
		Include_Path				original_path;

		Action_Mask					save_action_mask;
		Action_Mask					desired_mask;

		Net_Rendition				gnr;
		Subsegment const *			subsegment;
		Geometry const *			geometry;
		Geometry const *			current_geometry;
		Geometry const *			effective_geometry;

		Cut_Geometry_Collector	*	saved_collector;

		Segment const *				static_root;

		Tree_Process *				preset;
		Tree_Process *				cleanup;

		Highlight_Vector			tree_highlights;
		Highlight_Vector			seg_highlights;

		float						actual_extent;

		Destructible<Deferred_Item_Vector>::unique_ptr	saved_depth_peeling_list;
		Destructible<Deferred_Item_Vector>::unique_ptr	saved_deferred_3d_list;
		Destructible<Deferred_Item_Vector>::unique_ptr	saved_priority_deferred_3d_list;


		Hidden const *				save_hidden_root;
		int							save_hidden_count;

		bool						single;
		bool						was_processing_priority_segment;
		bool						local_transparent;
		Tree_Process *				local_quickmoves_cleanup;
		bool						maybe_highlights;
		bool						was_blocked;

		void *						selection_segment_level;

		Tree_Node (Net_Rendition const & n, Segment const * s, Include_Path const & p);
		~Tree_Node ();

		void Add_Preset (Tree_Process * ps) {
			stage = Tree_Progress_Preset;	// start over when we visit this node
			ps->prev = preset;
			preset = ps;
		}
		void Add_Cleanup (Tree_Process * cu) {
			cu->prev = cleanup;
			cleanup = cu;
		}
};



struct DC_Stats {
	Time						update_time;
	Time						finish_time;
	Time						static_time;
	Time						shadow_time;
	Time						assemble_time;
	Time						region_time;
	intptr_t					ffdebug;
	intptr_t					lffdebug;
	long						dots_dc;
	long						dots_3d;
	long						vertices_3d;
	long						dl_vertices_3d;
	long						lines_dc;
	long						lines_3d;
	long						dl_lines_3d;
	long						polygons_dc;
	long						polygons_3d;
	long						triangles_dc;
	long						triangles_3d;
	long						dl_triangles_3d;
	long						mem_dl;
	long						rasters;
	long						analytics;
	long						dl_analytics;
	long						occlusion_queries;
	long						tristrips;
	long						dl_ts_culled;
	long						dl_pe_culled;
	long						dl_executes;
	long						dl_strips;
	long						ndl_strips;
	long						dl_deletes;
	long						punted_grefs;
	long						segments;
	long						segments_extent_culled;
	long						segments_frustum_culled;
	long						segments_distance_culled;
	long						segments_vector_culled;
	long						segments_occlusion_culled;
	long						polyhedrons;
	long						polyhedron_extent_culled;
	long						polyhedron_frustum_culled;
	long						polyhedron_distance_culled;
	long						text_3d;
	long						text_extent_culled;
	long						text_frustum_culled;
	long						refs;
	long						refs_extent_culled;
	long						refs_frustum_culled;

	long						defer_inplace_items;
	long						defer_overlay_items;
	long						defer_sprite_items;
	long						defer_sprite_items_trans;
	long						defer_qm_dev_null;

	long						draw_inplace_items;
	long						draw_sprite_items;
	long						draw_overlay_items;
	long						undraw_qm_items;
	long						save_undraw_qm_items;
	long						delete_qm_items;

	long						defer_3d_items;
	long						delete_defer_3d_items;

	long						seg_forced_defer;
	long						dls_extent_defer;
	long						ref_extent_defer;
	long						seg_extent_defer;
	long						geo_extent_defer;

	long						lookup_texture;
	long						process_texture;
	long						activate_texture;
	long						load_texture;
	long						bind_texture;
	long						failed_texture;

	long						highlight_lookup;
	long						highlight_apply;

	long						dd_3d_geo;
	long						dp_3d_geo;
	long						deferred_geometry;
	long						deferred_segments;
	long						deferred_refs;
	long						deferred_seg_dls;
	long						oversized_allocs;
	long						raw_free;
	long						memory_acquired;
	long						memory_allocate;
	bool						event_interrupted;
	bool						resumed_update;
	long						hw_flush;
	long						hw_flush_last_count;
	long						seg_bounding_count;
	long						geo_bounding_count;

	long						proxy_segments;
	long						populated_segments;
	long						depopulated_segments;

	long						total_triangles_3d;
	long						total_dl_triangles_3d;

	float						last_height;
	float						last_width;
};




//#define UPDATE_LOGGING

#ifdef UPDATE_LOGGING
struct DC_Update_Log {

	const char * message;
	const char * file;
	intptr_t val;

	Geometry_Reference const *	ref;
	int							ref_index;
	Geometry const *			ref_geo;

	Time time;
	Time end_time;
};
#endif

class Rand {
public:

	Rand (unsigned int seed = 12345) {
		this->seed(seed);
	}

	INLINE void seed (unsigned int seed = 12345) {
		state = static_cast<unsigned long>(seed);
	}

	INLINE	int next () const {
		return static_cast<int>(
			((state = state * 214013L + 2531011L) >> 16) & 0x7fff
			);
	}

	INLINE	int nextInt (int hi) const {
		return nextInt(0, hi);
	}

	INLINE int nextInt (int lo, int hi) const {
		return (next() % (hi - lo)) + lo;
	}

private:

	mutable unsigned long state;
};




typedef std::vector<Hidden *, POOL_Allocator<Hidden *> >	Hidden_List_Vector;

struct Alternate {
	enum State : uint32_t {
		NONE = 0x00,
		OVERLAY = 0x01,
		INPLACE = 0x02,
		SPRITED = 0x03,
		REFLECTION = 0x04,
		SHADOW = 0x05,
		SELECTION = 0x06,
		ENUM_MASK = 0x07,
		CAPPING = 0x08,	// next bit up
		STEREO = 0x10,	// next bit up
		COUNT = 0x20	// next bit up
	};

	Destructible<Deferred_Item_Vector>::unique_ptr	dp_deferred_items;
	Destructible<Deferred_Item_Vector>::unique_ptr	deferred_items;

	Hidden const *		hidden_roots;
	Hidden_Progress *	hidden_progress;
	int					hidden_counts;

	Destructible<Hidden_List_Vector>::unique_ptr	capping_lists;
};


class Camera_ID_TR {

public:
	uint32_t			id;
	Transform_Rendition	transform_rendition;

	Camera_ID_TR(Transform_Rendition const & tr) : id(tr->db_camera->id), transform_rendition(tr) {}
	~Camera_ID_TR() {}
};

typedef std::vector<Camera_ID_TR, POOL_Allocator<Camera_ID_TR> > Camera_ID_TR_Vector;

struct Frame_Buffer_Info {
	Segment_Cache *			owning_cache;

	/* Items for the software z-buffer */
	Image *					color_buffer;
	Image *					depth_buffer;
	void *					color_buffer_driver_specific;
	void *					depth_buffer_driver_specific;

	Deferred_Text *			deferred_text;

	static void free_deferred_text(Frame_Buffer_Info * fbi);

	Net_Rendition			pending_window;
	Window_Rendition		window_rendition;

	Image_Format			required_format;
	bool					need_alpha;

	Int_XY					offset;					/* copy of tr->extent */

	bool					could_draw_as_block;
	bool					please_clear_depth_buffer;
	bool					force_window;
	bool					wanted_window;

	bool					fbi_touched;
	bool					fbi_3d;

	/* items used by spriting */
	void *					color_region;
	void *					depth_region;

	IntRectangle			region_area;
	int						unused_region_counter;
	bool					saved_z_regions_valid;
	bool					saved_color_regions_valid;
	bool					drawn_spriting;

	bool					patch_spriting;
	bool					patch_inplace;

	Destructible<Deferred_Item_Vector>::unique_ptr	sprite_defered;
	Destructible<Deferred_Item_Vector>::unique_ptr	inplace_defered;

	Destructible<Camera_ID_TR_Vector>::unique_ptr		camera_id_to_tr;

	void Record_Camera(Transform_Rendition const & tr);
	Transform_Rendition const & Find_Camera(Transform_Rendition const & tr);

	// used by visual selection
	FBI_Input				input;
	uint32_t				generation;
	IntRectangle			known_region;
	Image_Format			stencil_format;
	Image *					stencil_buffer;
	void *					stencil_buffer_driver_specific;

	// cut geometry
	uint32_t				cap_generation;
};


typedef std::vector<Frame_Buffer_Info *, POOL_Allocator<Frame_Buffer_Info *> > Frame_Buffer_Info_Vector_Pooled;


typedef std::vector<Polyhedron *, POOL_Allocator<Polyhedron *> > Polyhedron_Vector_Pooled;


INLINE void Flush_Geometry_Highlights2(Geometry_Highlights & geometry_highlights)
{
	for (auto it : geometry_highlights) {

		Geometry_Highlight * gh = it.second;

		gh->highlight = null;

		while (gh->details != null) {
			Geometry_Details *	gd = gh->details;
			gh->details = gd->next;
			FREE(gd, Geometry_Details);
		}

		FREE(gh, Geometry_Highlight);
	}
}


enum DCFF_Flags : uint32_t {
	DCFF_None						= 0x00000000,
	DCFF_one_font_located			= 0x00000001,
	DCFF_basic_fonts_located		= 0x00000002,
	DCFF_all_fonts_located			= 0x00000004,
	DCFF_all_freetype_fonts_located	= 0x00000008,
};

template<> struct _bit_ops_type < DCFF_Flags > {
	typedef DCFF_Flags type;
};


struct Display_Context {
private:

		Memory_Pool *		internal_memory_pool;

public:
		Actor const *		actor;
		Segment const *		actor_segment;
		Include_Path		actor_path;
		Name				actor_name;

		void *				data;
		void *				data2;				// for hybrid drivers

		Mutex				mutex;
		HThreadID			owning_thread;

		Thread_Data *		user_thread_data;
		Thread_Data *		updater_thread_data;

		INLINE Memory_Pool *	memory_pool() const {
			return (Memory_Pool *)internal_memory_pool;
		}
		INLINE void set_internal_memory_pool(Memory_Pool * memory_pool) {
			internal_memory_pool = memory_pool;
		}

		Action_Table			actions;
		Action_Table			current_actions;
		Action_Table			suspended_actions;

		uint32_t				highlight_generation;

		Tree_Filter *			tree_filter;

		Reference_Node *		reference_list_base;
		Reference_Node *		reference_list;

		Segment_Highlight_Map *	active_segment_highlights; // non-owning ref


		Geometry const *		geometry_in_progress;	/* for callbacks */
		Segment const *			segment_in_progress;
		Include_Path			path;

		Tree_Node *				tree_walk;
		Tree_Node *				breadth_tree_walk;

		enum DCF_Index {
			index_DCF_update_interrupted,
			index_DCF_block_interrupts,
			index_DCF_simple_shadow_processing,
			index_DCF_shadow_map_processing,
			index_DCF_shadow_map_first,
			index_DCF_mirroring,
			index_DCF_im_callthrough,
			index_DCF_processing_cut_geometry,
			index_DCF_compile_only,
			index_DCF_may_defer,
			index_DCF_depth_peeling_collecting_segdl,
			index_DCF_upwinding,
			index_DCF_processing_priority_segment,
			index_DCF_update_in_progress,
			index_DCF_scene_changed,
			index_DCF_doing_depth_peeling_now,
			index_DCF_damaged_z,
			index_DCF_doing_qm_render_highlights_now,
			index_DCF_preserve_depth_texture,
			index_DCF_doing_render_highlight_now,
			index_DCF_outer_window_exists,
			index_DCF_population_changed,
			index_DCF_NORMAL_DRAWING,
			index_DCF_NOT_DRAWING,
			index_DCF_SHADOW_ONLY_DRAWING,
			index_DCF_collecting_seg_dl,
			index_DCF_collecting_overlay,
			index_DCF_collecting_spriting,
			index_DCF_collecting_inplace,
			index_DCF_drawing_overlay,
			index_DCF_drawing_spriting,
			index_DCF_drawing_inplace,
			index_DCF_regenerate_3d_text_bounds,
			index_DCF_consider_update_complete,
			index_DCF_processing_capping_faces,
			index_DCF_drawing_window,
			index_DCF_stroked_text_in_progress,
			index_DCF_stereo_seen,
			index_DCF_forced_background_white,
			index_DCF_drawing_hidden,
			index_DCF_drawing_interrupted,
			index_DCF_bit_patterns_reversed,
			index_DCF_fixed_colors_allocated,
			index_DCF_timer_interrupts_enabled,
			index_DCF_interrupt_and_recycle,
			index_DCF_first_update_done,
			index_DCF_delete_display_list_immediately,
			index_DCF_color_warning_needed,
			index_DCF_is_shadow_driver,
			index_DCF_find_lights,
			index_DCF_gathering,
			index_DCF_doing_deferred_3d_now,
			index_DCF_overlay_collecting_segdl,

			///////////////////////////////
			index_DCF_LAST,
		};
		Define_Bitset(index_DCF_LAST) DCF_Bits;
		DCF_Bits				flags;

		enum DCPF_Index {
			index_DCPF_has_transparency,
			index_DCPF_file_output,
			index_DCPF_can_shadow_map,
			index_DCPF_incremental_z_updates,
			index_DCPF_z_buffer_depth_is_zero,
			index_DCPF_can_save_and_restore_regions,
			index_DCPF_cannot_overlay_text,
			index_DCPF_cannot_overlay_at_all,
			index_DCPF_can_prioritize,
			index_DCPF_can_reverse_depth_test,
			index_DCPF_can_mask_color_image,
			index_DCPF_hardware_dc_curves,
			index_DCPF_dither_rgb32_to_mapped8,
			index_DCPF_has_display,
			index_DCPF_has_locater,
			index_DCPF_has_keyboard,
			index_DCPF_windowing_system,
			index_DCPF_driver_controls_stretching,
			index_DCPF_has_gouraud_shading,
			index_DCPF_has_gouraud_lighting,
			index_DCPF_has_phong_lighting,
			index_DCPF_needs_gamma_correction,
			index_DCPF_load_font_requires_window,
			index_DCPF_can_do_landscape,
			index_DCPF_driver_does_truetype,
			index_DCPF_text_prefers_outline,
			index_DCPF_wants_wide_lines,
			index_DCPF_wants_contours,
			index_DCPF_can_display_stereo,
			index_DCPF_can_antialias,
			index_DCPF_can_antialias_lines,
			index_DCPF_can_widen_poly_cylinders,
			index_DCPF_window_id_change_ok,
			index_DCPF_has_hardware,
			index_DCPF_can_load_rotated_fonts,
			index_DCPF_absolute_coordinates,
			index_DCPF_supports_double_precision,
			index_DCPF_has_shaders,

			///////////////////////////////
			index_DCPF_LAST,
		};
		Define_Bitset(index_DCPF_LAST) DCPF_Bits;
		DCPF_Bits				physical_flags;

		DCFF_Flags				font_flags;


		Activity				activity;
		Activity				double_swap_mask;

		Activity				interesting_quickmove_activity;

		int32_t					window_update_generation;
		int32_t					update_slice;
		int32_t					fresh_slice_count;

		Segment const *			update_interrupted_by_stop_segment;
		intptr_t				update_stop_segment_path_key;
		Segment const *			update_stop_segment;
		intptr_t				update_last_segment_path_key;
		Segment const *			update_last_segment;

		size_t					starting_dl_memory;

		IntRectangle			outer_extent;
		IntRectangle			outer_actual_extent;	// after resize_subscreen

		Selection_Context *		selection_context;
		Frame_Buffer_Info *		visual_frame_buffer;
		Segment_Info const *	im_segments;

		bool					doing_selection_szb;
		bool					have_probed_overlays;

		//int						depth_peeling_layers;
		//float					depth_peeling_min_area;

		Action_Mask				deferred_3d_action_mask;
		Action_Mask				std_render_action_mask;
		Geometry const *		deferred_3d_most_recently_processed; /* dumb pointer -- do not refcount */

		Frame_Buffer_Info *		last_new_window_fbi;

		INLINE Alternate *		alternate() const {
			return alternate_ptr;
		}

		void Toggle_Alternate(Alternate::State state) const {

			static_assert(index_DCF_LAST < 64, "keep this to 64 bits please");
			ASSERT(alternate_ptr == &alternates[alternate_state]);

			if (ANYBIT(state, ~Alternate::ENUM_MASK)) {
				Alter(alternate_state) ^= state;
			}
			else {
				uint32_t values = alternate_state & Alternate::ENUM_MASK;
				Alter(alternate_state) &= ~Alternate::ENUM_MASK;
				if (values == 0)
					Alter(alternate_state) |= state;
				else
					ASSERT(values == state);
			}
			Alter(alternate_ptr) = &Alter(alternates)[alternate_state];
		}

		uint32_t Alternate_State(uint32_t mask = ~0) const {
			return alternate_state & mask;
		}

		void Reset_Alternate() const {
			Alter(alternate_state) = Alternate::NONE;
			Alter(alternate_ptr) = &Alter(alternates)[alternate_state];
		}

		INLINE Deferred_Item_Vector & get_deferred_items(size_t reserve=1024) const {
			if (alternate_ptr->deferred_items == null) {
				alternate_ptr->deferred_items = Construct<Deferred_Item_Vector>(memory_pool());
				alternate_ptr->deferred_items->reserve(reserve);
			}
			return *alternate_ptr->deferred_items;
		}

		INLINE Deferred_Item_Vector & get_dp_deferred_items(size_t reserve=1024) const {
			if (alternate_ptr->dp_deferred_items == null) {
				alternate_ptr->dp_deferred_items = Construct<Deferred_Item_Vector>(memory_pool());
				alternate_ptr->dp_deferred_items->reserve(reserve);
			}
			return *alternate_ptr->dp_deferred_items;
		}


		unsigned int 			hidden_action_mask;
		Geometry const *		hidden_most_recently_processed; /* dumb pointer -- do not refcount */

		Hidden const *			current_inclusion;
		Hidden_Cache *			hidden_cache;
		Hidden_Tristrip_Heap *	hidden_tristrip_heap;
		int32_t					hidden_priority;


		Net_Driver_Options		options;

		Rand					random;

	private:
		mutable	Incarnation		last_incarnation;
		int						drawing_pass;

		Alternate				alternates[Alternate::COUNT];
		uint32_t				alternate_state;
		Alternate *				alternate_ptr;


	public:
		int						cutting_id;

		void	Clean_Breadcrumbs();

		int		New_Incarnation() const { return ++last_incarnation; }


#define	NORMAL_DRAWING					0
#define	NOT_DRAWING						1
#define	SHADOW_ONLY_DRAWING				2

#define	ZHLR_PASS_WINDOW				2
#define	ZHLR_PASS_RENDERED_FACES		3
#define	ZHLR_PASS_POPULATE_Z_BUFFER		4
#define	ZHLR_PASS_HIDDEN_GEOMETRIES		5
#define	ZHLR_PASS_VISIBLE_GEOMETRIES	6

		INLINE int	Get_Draw_Pass() const { return drawing_pass; }

		INLINE void	Set_Draw_Pass(int new_draw_pass);

		int						soft_map_colors;
		int						first_soft_map_color;
		int						singleton_colors;
		int						first_singleton_color;
		int						fixed_colors;
		int						first_fixed_color;

		Int_RGB					fixed_color_resolution;
		Int_RGB					fixed_color_multiplier;
		Int_RGB					fixed_color_range;

		// Actual_color_res is used internally as the "real" number of distinguishable shades of colors. It's controlled
		// by the "color consolidation" driver option and the physical value "nominal color resolution".
		int						actual_color_resolution;

		Singleton_Color *		last_singletons;
		Singleton_Color *		current_singletons;

		Soft_Color_Map *		last_soft_maps;
		Soft_Color_Map *		current_soft_maps;

		Driver_Color const *	color_map;	/* 1 for each - rgb only */
		RGB const *				rgb_map;	/* and the matching float RGB */
		
		Driver_Color			force_foreground;
		Driver_Color			force_background;

		struct {
			/*		color info */
			int					first_color;
			int32_t				number_of_colors;
			int					fixed_colors;

			/*		size info */
			Int_XY				number_of_pixels;
			Int_XY				offset_in_pixels;
			Rectangle			extent;					/* min/max*/

			/* the rest include the effects of the 'physical size' Option */
			Vector2D			resolution;				/* pixels/cm */
			float				pixel_aspect;	/* height/pix / width/pix */

			/* 'size' also includes the subscreen, etc */
			Vector2D			size;					/* in centimeters */

			intptr_t			window_ID;		/* window systems only */
			intptr_t			window_ID2;		/* window systems only */

			IntRectangle		border;

			long				colormap_ID;	/* window systems only */
		}						current;


		struct {
			/*		color info */
			int						first_color;

			int32_t					number_of_colors;
			int32_t					max_number_of_colors;

			int						fixed_colors;

			/*		size info */
			Int_XY					number_of_pixels;
			Int_XY					offset_in_pixels;

			float					override_scale;

			Vector2D				size;					/* in centimeters */
			Rectangle				extent;					/* min/max*/

			/* more color info */


#define		DIRECT_RGB		0		/* 3-channel (24-bit) */
#define		MAPPED_RGB		1		/* 1-channel (2/4/8-bit) */
#define		GRAY_SCALE		2		/* something else */
			int						color_system;

			/* nominal_color_res is used as the scale factor for DIRECT and
			 * GRAY stored Driver_Color values, and as the scale factor for
			 * MAPPED calls to "set_device_color_map". It is usually a power
			 * of 2. For DIRECT colors, it must be <= 256, since that's all that
			 * will fit in direct_rgb.r/g/b.
			 */
			int32_t					nominal_color_resolution;

			/* more size info */

			float					default_line_weight; /* weight of "1.0" line;
															should be ODD number */
#define 	Double_Buffer_NONE		0
#define 	Double_Buffer_COPY		1
#define 	Double_Buffer_SWAP		2
			int						double_buffer_method;

			int						qmoves_methods[LAST_QMOVES+1];

			Int_RGB					dither_resolution;
			int						dither_offset;


			int						number_of_locater_buttons;
			char const *			driver_type;
			char const *			driver_version;		 /* RCS id */
			char const *			display_type;
			char const *			locater_type;


			int						z_buffer_depth;
			float					default_z_max; /* computed from z_buffer_depth*/

			int						hardware_lights;
			int						hardware_cutting_planes;

			int						hardware_max_texture_size;
			int						max_view_dependent_shadow_maps; //older cards can't handle more than one


#			if MAX_NOMINAL_COLOR_RESOLUTION > 256
				short				gamma[MAX_NOMINAL_COLOR_RESOLUTION];
#			else
				unsigned char		gamma[MAX_NOMINAL_COLOR_RESOLUTION];
#			endif

			/* scale factor for patterned polylines */
			float					pattern_scale;

			/* scale factor for weighted polylines */
			float					weight_scale;

			int						depth_peeling_layers;

			/*XBIT STUFF */

			/* what we looked up */
			Name					driver_config_id;
			Name					driver_config_version;

			/* non-owning reference owned by the driver data */
			Driver_Config *			card_entry;
		}	physical;

		float default_line_weight() const	{ return physical.default_line_weight; }

		unsigned short const *		bit_pattern[TOTAL_BIT_PATTERNS];
		int							bit_pattern_size; /* which are loaded? 0/8/16 */

		Drawing_Mode				outer_pending_drawing_state;
		Activity					pending_forced_activity;

		Deferred_Tree_List			deferred_trees[Deferred_COUNT];

		Deferred_Tree_List			also_draw_list;

		void Record_Camera(Net_Rendition const & nr) const;

		Destructible<Deferred_Tree_List_Heap>::unique_ptr	defer_tree_heap;
		Deferred_Tree_List_Int_Hash *	defer_tree_hash;

		Destructible<Intersecting_PSeg_List>::unique_ptr	populators;

		Bounding					global_bounds;

		Plane *						world_vertex_planes;
		DPlane *					world_vertex_dplanes;
		int							wvp_count;
		int							wvdp_count;
		Plane *						world_face_planes;
		DPlane *					world_face_dplanes;
		int							wfp_count;
		int							wfdp_count;
		int *						screen_plane_facings;
		int							screen_plane_facing_count;

typedef char			Face_Orientation;
#define FO_FORWARDS		(Face_Orientation)0x1
#define FO_BACKWARDS	(Face_Orientation)0x2
#define FO_WHOKNOWS		(Face_Orientation)0x4
#define FO_CULLED		(Face_Orientation)0x8

		Face_Orientation *		face_orientations;
		int						face_orientation_count;

		RGB *					polyhedron_vertex_colors;
		int						vertex_colors_count;
		RGB *					polyhedron_face_colors;
		int						face_colors_count;


		Gather_Font_Info *		gather_font_info;		/* pass-through */

		Text_Selection *		text_in_progress;		/* for selection */

		Net_Rendition			outer_rendition;
		Net_Rendition			top_rendition;

		/* FONT STUFF */
		Font_Family const *		font_families;
		Font_Hash const *		font_hash[FONT_HASH_SIZE];
		Font_Instance const *	generic_instances[GFN_LAST_GFN+1];

		unsigned int			fonts_active_n[FONT_TYPE_COUNT];
		Font_Instance const *	fonts_active[FONT_TYPE_COUNT];
		Name_List *				find_one_failures;

		HL_Data *				hidden_line_data;

		Activity				quick_moves_activity;


		Deferred_Item_Vector *	defered;
		Destructible<Deferred_Item_Vector>::unique_ptr	overlay_defered;
		Destructible<Deferred_Item_Vector>::unique_ptr	overlay_undraw_defered;

		Destructible<Frame_Buffer_Info_Vector_Pooled>::unique_ptr	fbi_associates;

		Destructible<Polyhedron_Vector_Pooled>::unique_ptr	polyhedrons_for_flushing;
		Destructible<Polyhedron_Vector_Pooled>::unique_ptr	polyhedrons_for_flushing_last;


		Destructible<Segment_Vector_Pooled>::unique_ptr	segment_scratch;
		Destructible<Include_Vector_Pooled>::unique_ptr	include_scratch;

		

		bool					overlay_buffer_dirty;
		bool					xor_on;

		// qmoves_method is the marriage between what the driver supports and the user's driver options requests
		int						qmoves_method;

		int						soft_clip_allowance;

		int						facings_allocated;
		int *					facings_buffer;


		float					(*deferred_texture_matrix)[4];

		Cut_Geometry_Collector *cut_geometry_collector;
		Destructible<Cut_Collector_Map>::unique_ptr		cut_collector_map;

		Isoline_Pieces *		isoline_pieces;

		struct {
			Image *						image;
			Actor *						actor;
			Start_Device_Action			start_device;
			Get_Current_Info_Action		get_current_info;
			int							debug_bits;
			Simple_Shadow *				current;
		}							simple_shadow;

		struct {
			Shadow_Map *				current;
		}							shadow_map;

		struct {
			Selection_Map *				current;
			int							path_index;
		}							selection_map;

		SimpleCuboid *				gather_cuboid;
		Texture *					antialias_line_texture;

		DC_Stats *					stats;
		DC_Stats *					prev_stats;

		Segment	const *				associated_segment;
		void const *				associated_data;


		Destructible<DC_Point_Vector_Pooled>::unique_ptr	polydot_batch_points;
		Destructible<RGBAS32_Vector_Pooled>::unique_ptr		polydot_batch_colors;


		int							hw_flush_last_count;
		int							hw_flush_limit;

		Matrix						line_pattern_transform;

		int32_t						sizeof_raster_buffer;
		void *						raster_buffer;

#ifdef UPDATE_LOGGING
#define DC_UPDATE_LOG_LENGTH 1024*8
		DC_Update_Log				update_log[DC_UPDATE_LOG_LENGTH];
		int							update_log_index;
#endif

		Path_Table					path_table;

		Destructible<Geometry_Highlights>::unique_ptr	geometry_highlights;

		Geometry_Details *								geometry_highlights_details;

		Polyhedron_Drawing_Data *	polyhedron_data;

	public:

		INLINE void Flush_Geometry_Highlights()
		{
			ASSERT(geometry_highlights != null);
			Flush_Geometry_Highlights2(*geometry_highlights.get());
			geometry_highlights = null;
		}

		Tree_Node *	_recycle_tree_node;

		Tree_Node *	Push_Tree (Net_Rendition const & n, Segment const * s, Include_Path const & p, bool breadth = false) {

				Tree_Node *	node = _recycle_tree_node;

				if (node == null)
					POOL_ALLOC(node, Tree_Node, memory_pool());
				else
					_recycle_tree_node = null;

				new (node) Tree_Node(n, s, p);

				if (breadth) {
					node->next = breadth_tree_walk;
					breadth_tree_walk = node;
				}
				else {
					node->next = tree_walk;
					tree_walk = node;
				}

				return node;
			}
		void Pop_Tree (bool preserve_current = false) {
				Tree_Node *	node = tree_walk;

				tree_walk = node->next;
				Prefetch(tree_walk);

				node->~Tree_Node();

				if(_recycle_tree_node == null)
					_recycle_tree_node = node;
				else
					FREE(node, Tree_Node);

				if (tree_walk != null)
					current_actions = tree_walk->actions;
				else if (!preserve_current)
					current_actions = actions;
			}
		bool
			Activate_Breadth_Tree() {
				Tree_Node *		node;

				if (breadth_tree_walk == null)
					return false;

				while ((node = breadth_tree_walk) != null) {
					breadth_tree_walk = node->next;
					node->next = tree_walk;
					tree_walk = node;
				}

				return true;
			}

		Action_Table &
			Push_Actions (char const * id, Net_Rendition const & nr) {
				Action_Table	at = current_actions.Copy();

				at->prev = current_actions;
				current_actions = at;
				at->id = id;
				at->mode = nr->current;
				if (tree_walk != null)
					tree_walk->actions = at;

				return current_actions;
			}
		Action_Table &
			Pop_Actions (char const * id) {
				Action_Table	at = current_actions;
				UNREFERENCED(id);
				if (at->prev == null
#ifdef HOOPS_DEBUG_BUILD
					|| id != null && !EQUAL_STRINGS(id, at->id)
#endif
					)
					HE_PROCEDURAL_ERROR("Push/Pop _Actions mismatch");
				else
					current_actions = at->prev;
				if (tree_walk != null)
					tree_walk->actions = current_actions;

				return current_actions;
			}

		Action_Table &
			Use_Previous_Actions () {
				Action_Table	at = current_actions;

				if (at->prev == null)
					HE_PROCEDURAL_ERROR("Push/Pop _Actions mismatch");
				else {
					current_actions = at->prev;
					at->prev = suspended_actions;
					suspended_actions = at;
				}
				if (tree_walk != null)
					tree_walk->actions = current_actions;

				return current_actions;
			}

		Action_Table &
			Resume_Current_Actions () {
				Action_Table	at = suspended_actions;

				if (at == null)
					HE_PROCEDURAL_ERROR("Previous/Resume _Actions mismatch");
				else {
					suspended_actions = at->prev;
					at->prev = current_actions;
					current_actions = at;
				}
				if (tree_walk != null)
					tree_walk->actions = current_actions;

				return current_actions;
			}


		void
			Add_Filter (Tree_Filter * f) {
				f->prev = tree_filter;
				tree_filter = f;
			}
		void
			Remove_Filter () {
				Tree_Filter *		f = tree_filter;

				if (f == null)
					HE_PROCEDURAL_ERROR("Remove_Filter mismatch");
				else {
					tree_filter = f->prev;
					delete f;
				}
			}
};

#define	PUSH_ACTIONS(_dc_, _id_, _nr_)			((Display_Context *)_dc_)->Push_Actions (_id_, _nr_)
#define	POP_ACTIONS(_dc_, _id_)					((Display_Context *)_dc_)->Pop_Actions (_id_)
#define	USE_PREVIOUS_ACTIONS(_dc_)				((Display_Context *)_dc_)->Use_Previous_Actions ()
#define	RESUME_CURRENT_ACTIONS(_dc_)			((Display_Context *)_dc_)->Resume_Current_Actions ()


#define DC_STATS(x) ((Display_Context*)dc)->stats->x

#ifdef HOOPS_DEBUG_BUILD
// two versions on for debugging
static const Display_Context::DCF_Bits	DCF_update_interrupted				(Display_Context::index_DCF_update_interrupted)				  ;
static const Display_Context::DCF_Bits	DCF_block_interrupts				(Display_Context::index_DCF_block_interrupts)				  ;
static const Display_Context::DCF_Bits	DCF_simple_shadow_processing		(Display_Context::index_DCF_simple_shadow_processing)		  ;
static const Display_Context::DCF_Bits	DCF_shadow_map_processing			(Display_Context::index_DCF_shadow_map_processing)			  ;
static const Display_Context::DCF_Bits	DCF_shadow_map_first				(Display_Context::index_DCF_shadow_map_first)				  ;
static const Display_Context::DCF_Bits	DCF_mirroring						(Display_Context::index_DCF_mirroring)						  ;
static const Display_Context::DCF_Bits	DCF_im_callthrough					(Display_Context::index_DCF_im_callthrough)					  ;
static const Display_Context::DCF_Bits	DCF_processing_cut_geometry			(Display_Context::index_DCF_processing_cut_geometry)		  ;
static const Display_Context::DCF_Bits	DCF_compile_only					(Display_Context::index_DCF_compile_only)					  ;
static const Display_Context::DCF_Bits	DCF_may_defer						(Display_Context::index_DCF_may_defer)						  ;
static const Display_Context::DCF_Bits	DCF_depth_peeling_collecting_segdl	(Display_Context::index_DCF_depth_peeling_collecting_segdl)	  ;
static const Display_Context::DCF_Bits	DCF_upwinding						(Display_Context::index_DCF_upwinding)						  ;
static const Display_Context::DCF_Bits	DCF_processing_priority_segment		(Display_Context::index_DCF_processing_priority_segment)	  ;
static const Display_Context::DCF_Bits	DCF_update_in_progress				(Display_Context::index_DCF_update_in_progress)				  ;
static const Display_Context::DCF_Bits	DCF_scene_changed					(Display_Context::index_DCF_scene_changed)					  ;
static const Display_Context::DCF_Bits	DCF_doing_depth_peeling_now			(Display_Context::index_DCF_doing_depth_peeling_now)		  ;
static const Display_Context::DCF_Bits	DCF_damaged_z						(Display_Context::index_DCF_damaged_z)						  ;
static const Display_Context::DCF_Bits	DCF_doing_qm_render_highlights_now	(Display_Context::index_DCF_doing_qm_render_highlights_now)	  ;
static const Display_Context::DCF_Bits	DCF_preserve_depth_texture			(Display_Context::index_DCF_preserve_depth_texture)			  ;
static const Display_Context::DCF_Bits	DCF_doing_render_highlight_now		(Display_Context::index_DCF_doing_render_highlight_now)		  ;
static const Display_Context::DCF_Bits	DCF_outer_window_exists				(Display_Context::index_DCF_outer_window_exists)			  ;
static const Display_Context::DCF_Bits	DCF_population_changed				(Display_Context::index_DCF_population_changed)				  ;
static const Display_Context::DCF_Bits	DCF_NORMAL_DRAWING					(Display_Context::index_DCF_NORMAL_DRAWING)					  ;
static const Display_Context::DCF_Bits	DCF_NOT_DRAWING						(Display_Context::index_DCF_NOT_DRAWING)					  ;
static const Display_Context::DCF_Bits	DCF_SHADOW_ONLY_DRAWING				(Display_Context::index_DCF_SHADOW_ONLY_DRAWING)			  ;
static const Display_Context::DCF_Bits	DCF_collecting_seg_dl				(Display_Context::index_DCF_collecting_seg_dl)				  ;
static const Display_Context::DCF_Bits	DCF_collecting_overlay				(Display_Context::index_DCF_collecting_overlay)				  ;
static const Display_Context::DCF_Bits	DCF_collecting_spriting				(Display_Context::index_DCF_collecting_spriting)			  ;
static const Display_Context::DCF_Bits	DCF_collecting_inplace				(Display_Context::index_DCF_collecting_inplace)				  ;
static const Display_Context::DCF_Bits	DCF_drawing_overlay					(Display_Context::index_DCF_drawing_overlay)				  ;
static const Display_Context::DCF_Bits	DCF_drawing_spriting				(Display_Context::index_DCF_drawing_spriting)				  ;
static const Display_Context::DCF_Bits	DCF_drawing_inplace					(Display_Context::index_DCF_drawing_inplace)				  ;
static const Display_Context::DCF_Bits	DCF_regenerate_3d_text_bounds		(Display_Context::index_DCF_regenerate_3d_text_bounds)		  ;
static const Display_Context::DCF_Bits	DCF_consider_update_complete		(Display_Context::index_DCF_consider_update_complete)		  ;
static const Display_Context::DCF_Bits	DCF_processing_capping_faces		(Display_Context::index_DCF_processing_capping_faces)		  ;
static const Display_Context::DCF_Bits	DCF_drawing_window					(Display_Context::index_DCF_drawing_window)					  ;
static const Display_Context::DCF_Bits	DCF_stroked_text_in_progress		(Display_Context::index_DCF_stroked_text_in_progress)		  ;
static const Display_Context::DCF_Bits	DCF_stereo_seen						(Display_Context::index_DCF_stereo_seen)					  ;
static const Display_Context::DCF_Bits	DCF_forced_background_white			(Display_Context::index_DCF_forced_background_white)		  ;
static const Display_Context::DCF_Bits	DCF_drawing_hidden					(Display_Context::index_DCF_drawing_hidden)					  ;
static const Display_Context::DCF_Bits	DCF_drawing_interrupted				(Display_Context::index_DCF_drawing_interrupted)			  ;
static const Display_Context::DCF_Bits	DCF_bit_patterns_reversed			(Display_Context::index_DCF_bit_patterns_reversed)			  ;
static const Display_Context::DCF_Bits	DCF_fixed_colors_allocated			(Display_Context::index_DCF_fixed_colors_allocated)			  ;
static const Display_Context::DCF_Bits	DCF_timer_interrupts_enabled		(Display_Context::index_DCF_timer_interrupts_enabled)		  ;
static const Display_Context::DCF_Bits	DCF_interrupt_and_recycle			(Display_Context::index_DCF_interrupt_and_recycle)			  ;
static const Display_Context::DCF_Bits	DCF_first_update_done				(Display_Context::index_DCF_first_update_done)				  ;
static const Display_Context::DCF_Bits	DCF_delete_display_list_immediately	(Display_Context::index_DCF_delete_display_list_immediately)  ;
static const Display_Context::DCF_Bits	DCF_color_warning_needed			(Display_Context::index_DCF_color_warning_needed)			  ;
static const Display_Context::DCF_Bits	DCF_is_shadow_driver				(Display_Context::index_DCF_is_shadow_driver)				  ;
static const Display_Context::DCF_Bits	DCF_find_lights						(Display_Context::index_DCF_find_lights)					  ;
static const Display_Context::DCF_Bits	DCF_gathering						(Display_Context::index_DCF_gathering)						  ;
static const Display_Context::DCF_Bits	DCF_doing_deferred_3d_now			(Display_Context::index_DCF_doing_deferred_3d_now)			  ;
static const Display_Context::DCF_Bits	DCF_overlay_collecting_segdl		(Display_Context::index_DCF_overlay_collecting_segdl)		  ;
																																
static const Display_Context::DCF_Bits	DCF_ANY_COLLECTING_QUICKMOVES		(DCF_collecting_overlay | DCF_collecting_spriting | DCF_collecting_inplace)	;
static const Display_Context::DCF_Bits	DCF_ANY_DRAWING_QUICKMOVES			(DCF_drawing_overlay | DCF_drawing_spriting | DCF_drawing_inplace)			;


static const Display_Context::DCPF_Bits DCPF_has_transparency				(Display_Context::index_DCPF_has_transparency)				;
static const Display_Context::DCPF_Bits DCPF_file_output					(Display_Context::index_DCPF_file_output)					;
static const Display_Context::DCPF_Bits DCPF_can_shadow_map					(Display_Context::index_DCPF_can_shadow_map)				;
static const Display_Context::DCPF_Bits DCPF_incremental_z_updates			(Display_Context::index_DCPF_incremental_z_updates)			;	// clear_Z_buffer observed AND geometry equal in depth gets painted
static const Display_Context::DCPF_Bits DCPF_z_buffer_depth_is_zero			(Display_Context::index_DCPF_z_buffer_depth_is_zero)		;	// physical.z_buffer_depth == 0
static const Display_Context::DCPF_Bits DCPF_can_save_and_restore_regions	(Display_Context::index_DCPF_can_save_and_restore_regions)	;
static const Display_Context::DCPF_Bits DCPF_cannot_overlay_text			(Display_Context::index_DCPF_cannot_overlay_text)			;
static const Display_Context::DCPF_Bits DCPF_cannot_overlay_at_all			(Display_Context::index_DCPF_cannot_overlay_at_all)			;
static const Display_Context::DCPF_Bits DCPF_can_prioritize					(Display_Context::index_DCPF_can_prioritize)				;
static const Display_Context::DCPF_Bits DCPF_can_reverse_depth_test			(Display_Context::index_DCPF_can_reverse_depth_test)		;
static const Display_Context::DCPF_Bits DCPF_can_mask_color_image			(Display_Context::index_DCPF_can_mask_color_image)			;
static const Display_Context::DCPF_Bits DCPF_hardware_dc_curves				(Display_Context::index_DCPF_hardware_dc_curves)			;
static const Display_Context::DCPF_Bits DCPF_dither_rgb32_to_mapped8		(Display_Context::index_DCPF_dither_rgb32_to_mapped8)		;
static const Display_Context::DCPF_Bits DCPF_has_display					(Display_Context::index_DCPF_has_display)					;
static const Display_Context::DCPF_Bits DCPF_has_locater					(Display_Context::index_DCPF_has_locater)					;
static const Display_Context::DCPF_Bits DCPF_has_keyboard					(Display_Context::index_DCPF_has_keyboard)					;
static const Display_Context::DCPF_Bits DCPF_windowing_system				(Display_Context::index_DCPF_windowing_system)				;
static const Display_Context::DCPF_Bits DCPF_driver_controls_stretching		(Display_Context::index_DCPF_driver_controls_stretching)	;
static const Display_Context::DCPF_Bits DCPF_has_gouraud_shading			(Display_Context::index_DCPF_has_gouraud_shading)			;
static const Display_Context::DCPF_Bits DCPF_has_gouraud_lighting			(Display_Context::index_DCPF_has_gouraud_lighting)			;
static const Display_Context::DCPF_Bits DCPF_has_phong_lighting				(Display_Context::index_DCPF_has_phong_lighting)			;
static const Display_Context::DCPF_Bits DCPF_needs_gamma_correction			(Display_Context::index_DCPF_needs_gamma_correction)		;
static const Display_Context::DCPF_Bits DCPF_load_font_requires_window		(Display_Context::index_DCPF_load_font_requires_window)		;
static const Display_Context::DCPF_Bits DCPF_can_do_landscape				(Display_Context::index_DCPF_can_do_landscape)				;
static const Display_Context::DCPF_Bits DCPF_driver_does_truetype			(Display_Context::index_DCPF_driver_does_truetype)			;
static const Display_Context::DCPF_Bits DCPF_text_prefers_outline			(Display_Context::index_DCPF_text_prefers_outline)			;
static const Display_Context::DCPF_Bits DCPF_wants_wide_lines				(Display_Context::index_DCPF_wants_wide_lines)				;
static const Display_Context::DCPF_Bits DCPF_wants_contours					(Display_Context::index_DCPF_wants_contours)				;
static const Display_Context::DCPF_Bits DCPF_can_display_stereo				(Display_Context::index_DCPF_can_display_stereo)			;
static const Display_Context::DCPF_Bits DCPF_can_antialias					(Display_Context::index_DCPF_can_antialias)					;
static const Display_Context::DCPF_Bits DCPF_can_antialias_lines			(Display_Context::index_DCPF_can_antialias_lines)			;
static const Display_Context::DCPF_Bits DCPF_can_widen_poly_cylinders		(Display_Context::index_DCPF_can_widen_poly_cylinders)		;
static const Display_Context::DCPF_Bits DCPF_window_id_change_ok			(Display_Context::index_DCPF_window_id_change_ok)			;
static const Display_Context::DCPF_Bits DCPF_has_hardware					(Display_Context::index_DCPF_has_hardware)					;
static const Display_Context::DCPF_Bits DCPF_can_load_rotated_fonts			(Display_Context::index_DCPF_can_load_rotated_fonts)		;
static const Display_Context::DCPF_Bits DCPF_absolute_coordinates			(Display_Context::index_DCPF_absolute_coordinates)			;
static const Display_Context::DCPF_Bits DCPF_supports_double_precision		(Display_Context::index_DCPF_supports_double_precision)		;
static const Display_Context::DCPF_Bits DCPF_has_shaders					(Display_Context::index_DCPF_has_shaders)					;

#else
// two versions on for debugging
#define DCF_update_interrupted				Display_Context::DCF_Bits(Display_Context::index_DCF_update_interrupted)
#define DCF_block_interrupts				Display_Context::DCF_Bits(Display_Context::index_DCF_block_interrupts)
#define DCF_simple_shadow_processing		Display_Context::DCF_Bits(Display_Context::index_DCF_simple_shadow_processing)
#define DCF_shadow_map_processing			Display_Context::DCF_Bits(Display_Context::index_DCF_shadow_map_processing)
#define DCF_shadow_map_first				Display_Context::DCF_Bits(Display_Context::index_DCF_shadow_map_first)
#define DCF_mirroring						Display_Context::DCF_Bits(Display_Context::index_DCF_mirroring)
#define DCF_im_callthrough					Display_Context::DCF_Bits(Display_Context::index_DCF_im_callthrough)
#define DCF_processing_cut_geometry			Display_Context::DCF_Bits(Display_Context::index_DCF_processing_cut_geometry)
#define DCF_compile_only					Display_Context::DCF_Bits(Display_Context::index_DCF_compile_only)
#define DCF_may_defer						Display_Context::DCF_Bits(Display_Context::index_DCF_may_defer)
#define DCF_depth_peeling_collecting_segdl	Display_Context::DCF_Bits(Display_Context::index_DCF_depth_peeling_collecting_segdl)
#define DCF_upwinding						Display_Context::DCF_Bits(Display_Context::index_DCF_upwinding)
#define DCF_processing_priority_segment		Display_Context::DCF_Bits(Display_Context::index_DCF_processing_priority_segment)
#define DCF_update_in_progress				Display_Context::DCF_Bits(Display_Context::index_DCF_update_in_progress)
#define DCF_scene_changed					Display_Context::DCF_Bits(Display_Context::index_DCF_scene_changed)
#define DCF_doing_depth_peeling_now			Display_Context::DCF_Bits(Display_Context::index_DCF_doing_depth_peeling_now)
#define DCF_damaged_z						Display_Context::DCF_Bits(Display_Context::index_DCF_damaged_z)
#define DCF_doing_qm_render_highlights_now	Display_Context::DCF_Bits(Display_Context::index_DCF_doing_qm_render_highlights_now)
#define DCF_preserve_depth_texture			Display_Context::DCF_Bits(Display_Context::index_DCF_preserve_depth_texture)
#define DCF_doing_render_highlight_now		Display_Context::DCF_Bits(Display_Context::index_DCF_doing_render_highlight_now)
#define DCF_outer_window_exists				Display_Context::DCF_Bits(Display_Context::index_DCF_outer_window_exists)
#define DCF_population_changed				Display_Context::DCF_Bits(Display_Context::index_DCF_population_changed)
#define DCF_NORMAL_DRAWING					Display_Context::DCF_Bits(Display_Context::index_DCF_NORMAL_DRAWING)
#define DCF_NOT_DRAWING						Display_Context::DCF_Bits(Display_Context::index_DCF_NOT_DRAWING)
#define DCF_SHADOW_ONLY_DRAWING				Display_Context::DCF_Bits(Display_Context::index_DCF_SHADOW_ONLY_DRAWING)
#define DCF_collecting_seg_dl				Display_Context::DCF_Bits(Display_Context::index_DCF_collecting_seg_dl)
#define DCF_collecting_overlay				Display_Context::DCF_Bits(Display_Context::index_DCF_collecting_overlay)
#define DCF_collecting_spriting				Display_Context::DCF_Bits(Display_Context::index_DCF_collecting_spriting)
#define DCF_collecting_inplace				Display_Context::DCF_Bits(Display_Context::index_DCF_collecting_inplace)
#define DCF_drawing_overlay					Display_Context::DCF_Bits(Display_Context::index_DCF_drawing_overlay)
#define DCF_drawing_spriting				Display_Context::DCF_Bits(Display_Context::index_DCF_drawing_spriting)
#define DCF_drawing_inplace					Display_Context::DCF_Bits(Display_Context::index_DCF_drawing_inplace)
#define DCF_regenerate_3d_text_bounds		Display_Context::DCF_Bits(Display_Context::index_DCF_regenerate_3d_text_bounds)
#define DCF_consider_update_complete		Display_Context::DCF_Bits(Display_Context::index_DCF_consider_update_complete)
#define DCF_processing_capping_faces		Display_Context::DCF_Bits(Display_Context::index_DCF_processing_capping_faces)
#define DCF_drawing_window					Display_Context::DCF_Bits(Display_Context::index_DCF_drawing_window)
#define DCF_stroked_text_in_progress		Display_Context::DCF_Bits(Display_Context::index_DCF_stroked_text_in_progress)
#define DCF_stereo_seen						Display_Context::DCF_Bits(Display_Context::index_DCF_stereo_seen)
#define DCF_forced_background_white			Display_Context::DCF_Bits(Display_Context::index_DCF_forced_background_white)
#define DCF_drawing_hidden					Display_Context::DCF_Bits(Display_Context::index_DCF_drawing_hidden)
#define DCF_drawing_interrupted				Display_Context::DCF_Bits(Display_Context::index_DCF_drawing_interrupted)
#define DCF_bit_patterns_reversed			Display_Context::DCF_Bits(Display_Context::index_DCF_bit_patterns_reversed)
#define DCF_fixed_colors_allocated			Display_Context::DCF_Bits(Display_Context::index_DCF_fixed_colors_allocated)
#define DCF_timer_interrupts_enabled		Display_Context::DCF_Bits(Display_Context::index_DCF_timer_interrupts_enabled)
#define DCF_interrupt_and_recycle			Display_Context::DCF_Bits(Display_Context::index_DCF_interrupt_and_recycle)
#define DCF_first_update_done				Display_Context::DCF_Bits(Display_Context::index_DCF_first_update_done)
#define DCF_delete_display_list_immediately	Display_Context::DCF_Bits(Display_Context::index_DCF_delete_display_list_immediately)
#define DCF_color_warning_needed			Display_Context::DCF_Bits(Display_Context::index_DCF_color_warning_needed)
#define DCF_is_shadow_driver				Display_Context::DCF_Bits(Display_Context::index_DCF_is_shadow_driver)
#define DCF_find_lights						Display_Context::DCF_Bits(Display_Context::index_DCF_find_lights)
#define DCF_gathering						Display_Context::DCF_Bits(Display_Context::index_DCF_gathering)
#define DCF_doing_deferred_3d_now			Display_Context::DCF_Bits(Display_Context::index_DCF_doing_deferred_3d_now)
#define DCF_overlay_collecting_segdl		Display_Context::DCF_Bits(Display_Context::index_DCF_overlay_collecting_segdl)


#define DCF_ANY_COLLECTING_QUICKMOVES		(DCF_collecting_overlay|DCF_collecting_spriting|DCF_collecting_inplace)
#define DCF_ANY_DRAWING_QUICKMOVES			(DCF_drawing_overlay|DCF_drawing_spriting|DCF_drawing_inplace)


#define DCPF_has_transparency				Display_Context::DCPF_Bits(Display_Context::index_DCPF_has_transparency)
#define DCPF_file_output					Display_Context::DCPF_Bits(Display_Context::index_DCPF_file_output)
#define DCPF_can_shadow_map					Display_Context::DCPF_Bits(Display_Context::index_DCPF_can_shadow_map)
#define DCPF_incremental_z_updates			Display_Context::DCPF_Bits(Display_Context::index_DCPF_incremental_z_updates) // clear_Z_buffer observed AND geometry equal in depth gets painted
#define DCPF_z_buffer_depth_is_zero			Display_Context::DCPF_Bits(Display_Context::index_DCPF_z_buffer_depth_is_zero) // physical.z_buffer_depth == 0
#define DCPF_can_save_and_restore_regions	Display_Context::DCPF_Bits(Display_Context::index_DCPF_can_save_and_restore_regions)
#define DCPF_cannot_overlay_text			Display_Context::DCPF_Bits(Display_Context::index_DCPF_cannot_overlay_text)
#define DCPF_cannot_overlay_at_all			Display_Context::DCPF_Bits(Display_Context::index_DCPF_cannot_overlay_at_all)
#define DCPF_can_prioritize					Display_Context::DCPF_Bits(Display_Context::index_DCPF_can_prioritize)
#define DCPF_can_reverse_depth_test			Display_Context::DCPF_Bits(Display_Context::index_DCPF_can_reverse_depth_test)
#define DCPF_can_mask_color_image			Display_Context::DCPF_Bits(Display_Context::index_DCPF_can_mask_color_image)
#define DCPF_hardware_dc_curves				Display_Context::DCPF_Bits(Display_Context::index_DCPF_hardware_dc_curves)
#define DCPF_dither_rgb32_to_mapped8		Display_Context::DCPF_Bits(Display_Context::index_DCPF_dither_rgb32_to_mapped8)
#define DCPF_has_display					Display_Context::DCPF_Bits(Display_Context::index_DCPF_has_display)
#define DCPF_has_locater					Display_Context::DCPF_Bits(Display_Context::index_DCPF_has_locater)
#define DCPF_has_keyboard					Display_Context::DCPF_Bits(Display_Context::index_DCPF_has_keyboard)
#define DCPF_windowing_system				Display_Context::DCPF_Bits(Display_Context::index_DCPF_windowing_system)
#define DCPF_driver_controls_stretching		Display_Context::DCPF_Bits(Display_Context::index_DCPF_driver_controls_stretching)
#define DCPF_has_gouraud_shading			Display_Context::DCPF_Bits(Display_Context::index_DCPF_has_gouraud_shading)
#define DCPF_has_gouraud_lighting			Display_Context::DCPF_Bits(Display_Context::index_DCPF_has_gouraud_lighting)
#define DCPF_has_phong_lighting				Display_Context::DCPF_Bits(Display_Context::index_DCPF_has_phong_lighting)
#define DCPF_needs_gamma_correction			Display_Context::DCPF_Bits(Display_Context::index_DCPF_needs_gamma_correction)
#define DCPF_load_font_requires_window		Display_Context::DCPF_Bits(Display_Context::index_DCPF_load_font_requires_window)
#define DCPF_can_do_landscape				Display_Context::DCPF_Bits(Display_Context::index_DCPF_can_do_landscape)
#define DCPF_driver_does_truetype			Display_Context::DCPF_Bits(Display_Context::index_DCPF_driver_does_truetype)
#define DCPF_text_prefers_outline			Display_Context::DCPF_Bits(Display_Context::index_DCPF_text_prefers_outline)
#define DCPF_wants_wide_lines				Display_Context::DCPF_Bits(Display_Context::index_DCPF_wants_wide_lines)
#define DCPF_wants_contours					Display_Context::DCPF_Bits(Display_Context::index_DCPF_wants_contours)
#define DCPF_can_display_stereo				Display_Context::DCPF_Bits(Display_Context::index_DCPF_can_display_stereo)
#define DCPF_can_antialias					Display_Context::DCPF_Bits(Display_Context::index_DCPF_can_antialias)
#define DCPF_can_antialias_lines			Display_Context::DCPF_Bits(Display_Context::index_DCPF_can_antialias_lines)
#define DCPF_can_widen_poly_cylinders		Display_Context::DCPF_Bits(Display_Context::index_DCPF_can_widen_poly_cylinders)
#define DCPF_window_id_change_ok			Display_Context::DCPF_Bits(Display_Context::index_DCPF_window_id_change_ok)
#define	DCPF_has_hardware					Display_Context::DCPF_Bits(Display_Context::index_DCPF_has_hardware)
#define	DCPF_can_load_rotated_fonts			Display_Context::DCPF_Bits(Display_Context::index_DCPF_can_load_rotated_fonts)
#define DCPF_absolute_coordinates			Display_Context::DCPF_Bits(Display_Context::index_DCPF_absolute_coordinates)
#define DCPF_supports_double_precision		Display_Context::DCPF_Bits(Display_Context::index_DCPF_supports_double_precision)
#define	DCPF_has_shaders					Display_Context::DCPF_Bits(Display_Context::index_DCPF_has_shaders)


#endif



INLINE void	Display_Context::Set_Draw_Pass(int new_draw_pass) {
	drawing_pass = new_draw_pass;
	switch (drawing_pass) {

	case NOT_DRAWING:
		flags = DCF_NOT_DRAWING | (~DCF_NORMAL_DRAWING&~DCF_SHADOW_ONLY_DRAWING&flags);
		break;
	case NORMAL_DRAWING:
		flags = DCF_NORMAL_DRAWING | (~DCF_NOT_DRAWING&~DCF_SHADOW_ONLY_DRAWING&flags);
		break;
	case SHADOW_ONLY_DRAWING:
		/*case ZHLR_PASS_WINDOW:*/
		flags = DCF_SHADOW_ONLY_DRAWING | (~DCF_NORMAL_DRAWING&~DCF_NOT_DRAWING&flags);
		break;
	case ZHLR_PASS_RENDERED_FACES:
	case ZHLR_PASS_POPULATE_Z_BUFFER:
	case ZHLR_PASS_HIDDEN_GEOMETRIES:
	case ZHLR_PASS_VISIBLE_GEOMETRIES:
		flags &= ~(DCF_SHADOW_ONLY_DRAWING | DCF_NORMAL_DRAWING | DCF_NOT_DRAWING);
		break;
	}
}


struct Intersecting_PSeg : public CMO_Pooled {

	Intersecting_PSeg(Net_Rendition	const & _nr, Segment const * _seg, float _extent, bool _populated) {
		nr = _nr;
		seg = _seg;
		extent = _extent;
		populated = _populated;
	}

	Net_Rendition	nr;
	Segment const *	seg;
	float			extent;
	bool			populated;
};



class Block_Interrupts {
private:
	Display_Context *			display_context;
	Display_Context::DCF_Bits	save_block_interrupts;
	
public:
	INLINE Block_Interrupts (Display_Context * dc) :
		display_context (dc),
		save_block_interrupts (dc->flags & DCF_block_interrupts) {
		display_context->flags |= DCF_block_interrupts;
	}
	INLINE ~Block_Interrupts () { restore(); }

	INLINE void	restore () {
		display_context->flags &= ~DCF_block_interrupts;
		display_context->flags |= save_block_interrupts;
	}

private:
	Block_Interrupts ();
	Block_Interrupts (Block_Interrupts const &);
	Block_Interrupts & operator= (Block_Interrupts const &);
};


INLINE Tree_Node::Tree_Node (Net_Rendition const & n, Segment const * s, Include_Path const & p)
	: nr (n)
	, segment (s)
	, path (p)
	, original_path (p)
	, tree_highlights(nr->display_context->memory_pool())
	, seg_highlights(nr->display_context->memory_pool())
{
	Display_Context const *		dc = nr->display_context;

	activity = dc->activity;
	actions = dc->current_actions;

	was_processing_priority_segment = ANYBIT(dc->flags, DCF_processing_priority_segment);
}


template <typename T>
Rendition_Pointer<T>	Rendition_Pointer<T>::Create (Display_Context const * dc) {
	Rendition_Pointer<T>		temp;
	typedef T Rendition_Pointer_T;
	temp.counted_item = POOL_NEW(dc->memory_pool(), Rendition_Pointer_T)(dc);
	temp.initialize();
	return temp;
}

template <typename T>
INLINE	Rendition_Pointer<T> & Rendition_Pointer<T>::Modify (int offset) {
	if (CP::counted_item->shared()) {
		typedef T Rendition_Pointer_T;
		T *		temp = POOL_NEW(CP::counted_item->memory_pool, Rendition_Pointer_T)(*CP::counted_item);

		if (offset != 0)
			temp->incarnation = CP::counted_item->incarnation + offset;

		CP::counted_item->release();	// call directly instead of using CP::Retain() as we know it's not null
		CP::counted_item = temp;
		CP::counted_item->retain();		// likewise
	}
	else
		Renew();
	return *this;
}
template <typename T>
INLINE	Rendition_Pointer<T> & Rendition_Pointer<T>::Renew () {
	CP::counted_item->incarnation = CP::counted_item->display_context->New_Incarnation();
	return *this;
}


#define INTERCHAR_SPACE			0.20f /* of char width: a guess */
#define INTERLINE_SPACE			0.44f /* of char height: a guess */
#define DESCENDER_SIZE			0.40f /* of char height: a guess */

struct HWVisSel_Item {
	Net_Rendition			nr;
	Geometry const *		geometry;
	Segment const *			segment;
	Include_Chain *			inc_chain;

	bool					selected;
	bool					needs_inutile;
};
typedef std::vector<HWVisSel_Item, POOL_Allocator<HWVisSel_Item> > HWVisSel_Vector;

#ifdef HOOPS_WINDOWS_HDI_DRIVER
#	define EXPORT_DLL_ENTRY_POINT(get_actions) \
		extern "C" __declspec(dllexport) bool HD_Driver (\
			Thread_Data *	thread_data, \
			Actor *			actor, \
			int						request) { \
			return HD_Connect_Standard_Driver (thread_data, actor, request, get_actions); \
		}
#else
#	define EXPORT_DLL_ENTRY_POINT(get_actions)
#endif


INLINE void SET_TIMER_EXPIRED(bool flag)
{
	HI_What_Time();
#ifdef HPS_CORE_BUILD
	HI_HPS_Timer_Hook();
#endif
	Thread_Data *	thread_data = HOOPS::WORLD->main_thread;
	do {
		thread_data->timer_expired = flag;
	} while ((thread_data = thread_data->next) != null);
} 

// #define UPDATE_LOGGING 1
// #define UPDATE_LOGGING_DPRINTF 1

#ifdef UPDATE_LOGGING
#define UPDATE_LOG(dc, message, file, line) HD_Update_Log(dc, message, file, line)
#define CHECK_EXIT_UPDATE(_nr_, _uid_) HD_Check_Exit_Update((_nr_), __FILE__, __LINE__, _uid_)
#else
#define UPDATE_LOG(dc, message, file, line) UNREFERENCED(dc)
#define CHECK_EXIT_UPDATE(_nr_, _uid_) HD_Check_Exit_Update((_nr_), 0, 0, _uid_)
#endif
	

#define CHECK_INTERRUPED(dc) (!ANYBIT((dc)->flags, DCF_block_interrupts)&&ANYBIT((dc)->flags, DCF_update_interrupted))

/* HIA macros */

#define DRIVER_ACTION(_current_actions_, routine) ((_current_actions_)->routine)


#define AR_ACTION(_ar_, _type_) (_ar_)->action_block[Action_Call_Info<_type_>::action_block_id]->actions[Action_Call_Info<_type_>::action_offset]

#define AR_ACTION_EQUAL(_ar_, _type_1_, _type_2_) (AR_ACTION(_ar_, _type_1_).address == AR_ACTION(_ar_, _type_2_).address)

#define AR_ACTION_NULL(_ar_, _type_) (AR_ACTION(_ar_, _type_).address == nullroutine)

#define AR_ACTION_ASSIGN(_ar_, _target_type_, _source_type_) \
	((_ar_)->action_block[Action_Call_Info<_target_type_>::action_block_id].Modify()->actions[Action_Call_Info<_target_type_>::action_offset] = AR_ACTION(_ar_, _source_type_))

#define AR_ACTION_ASSIGN_NULL(_ar_, _target_type_) \
	(_ar_)->action_block[Action_Call_Info<_target_type_>::action_block_id].Modify()->actions[Action_Call_Info<_target_type_>::action_offset].address = nullroutine;


#define CALLBACK_ACTION(_ar_action_, _type_) ((Action_Call_Info<_type_>::im_type)(_ar_action_).address)

#define ASSOCIATE_CALLBACK_DATA(_dc_, _ar_action_) do { \
	Alter((_dc_))->associated_data = (_ar_action_).data; \
	Alter((_dc_))->associated_segment = (_ar_action_).segment; \
} while (0)

#define UNASSOCIATE_CALLBACK_DATA(_dc_) do { \
	Alter((_dc_))->associated_data = 0; \
	Alter((_dc_))->associated_segment = 0; \
	Alter((_dc_))->user_thread_data->flags &= ~Thread_Data::ALLOW_UNSAFE_THREADING; \
} while (0)

#define PRIMARY_IF(_nr_, _cbt_, ...) \
	Display_Context const *		_dc_ = (_nr_)->display_context; \
	Action_Rendition const &	_ar_ = (_nr_)->action_rendition; \
	AR_Action const &			_ar_action_ = AR_ACTION(_ar_, (_cbt_)); \
	if (_ar_action_.address != nullroutine) { \
		ASSOCIATE_CALLBACK_DATA(_dc_, _ar_action_); \
		CALLBACK_ACTION(_ar_action_, (_cbt_)) (&(_nr_), __VA_ARGS__); \
		UNASSOCIATE_CALLBACK_DATA(_dc_); \
	}

#define PRIMARY_IF_NA(_nr_, _cbt_) \
	Display_Context const *		_dc_ = (_nr_)->display_context; \
	Action_Rendition const &	_ar_ = (_nr_)->action_rendition; \
	AR_Action const &			_ar_action_ = AR_ACTION(_ar_, (_cbt_)); \
	if (_ar_action_.address != nullroutine) { \
		ASSOCIATE_CALLBACK_DATA(_dc_, _ar_action_); \
		CALLBACK_ACTION(_ar_action_, (_cbt_)) (&(_nr_)); \
		UNASSOCIATE_CALLBACK_DATA(_dc_); \
	}

#define ELSE_DRIVER(_nr_, _driver_action_, ...) \
	else { \
		Action_Table const & _current_actions_ = _dc_->current_actions; \
		DRIVER_ACTION(_current_actions_, _driver_action_)((_nr_), __VA_ARGS__); \
	}

#define ELSE_DRIVER_NA(_nr_, _driver_action_) \
	else { \
	Action_Table const & _current_actions_ = _dc_->current_actions; \
	DRIVER_ACTION(_current_actions_, _driver_action_)((_nr_)); \
}

#define SECONDARY_IF(_nr_, _cbt_, ...) \
	AR_Action const &	_ar_action_ = AR_ACTION(_ar_, (_cbt_)); \
	if (_ar_action_.address != nullroutine) { \
		ASSOCIATE_CALLBACK_DATA(_dc_, _ar_action_); \
		CALLBACK_ACTION(_ar_action_, (_cbt_)) (&(_nr_), __VA_ARGS__); \
		UNASSOCIATE_CALLBACK_DATA(_dc_); \
	}

#define CALLBACK_IF(_nr_, _cbt_, ...) do { \
	PRIMARY_IF(_nr_, _cbt_, __VA_ARGS__) \
} while(0)

#define CALLBACK_IF_NA(_nr_, _cbt_) do { \
	PRIMARY_IF_NA(_nr_, _cbt_) \
} while(0)

#define CALLBACK_IF_ELSE_DRIVER(_nr_, _cbt_, _driver_action_, ...) do { \
	PRIMARY_IF(_nr_, _cbt_, __VA_ARGS__) \
	ELSE_DRIVER(_nr_, _driver_action_, __VA_ARGS__) \
} while(0)

#define CALLBACK_IF_ELSE_DRIVER_NA(_nr_, _cbt_, _driver_action_) do { \
	PRIMARY_IF_NA(_nr_, _cbt_) \
	ELSE_DRIVER_NA(_nr_, _driver_action_) \
} while(0)

#define CALLBACK_IF_ELSE_GEOMETRY_ELSE_DRIVER(_nr_, _cbt_, _driver_action_, ...) do { \
	PRIMARY_IF(_nr_, _cbt_, __VA_ARGS__) \
	else { \
		SECONDARY_IF(_nr_, CBT_DRAW_3D_GEOMETRY, __VA_ARGS__) \
		ELSE_DRIVER(_nr_, _driver_action_, __VA_ARGS__) \
	} \
} while(0)

#define CALLBACK_MASKED_IF_ELSE_DRIVER(_nr_, _mask_, _cbt_, _driver_action_, ...) do { \
	if (ANYBIT ((nr)->display_context->current_actions->mask, _mask_)) { \
		PRIMARY_IF(_nr_, _cbt_, __VA_ARGS__) \
		ELSE_DRIVER(_nr_, _driver_action_, __VA_ARGS__) \
	} \
} while(0)

#define CALLBACK_MASKED_IF_ELSE_GEOMETRY_ELSE_DRIVER(_nr_, _mask_, _cbt_, _driver_action_, ...) do { \
	if (ANYBIT ((nr)->display_context->current_actions->mask, _mask_)) { \
		PRIMARY_IF(_nr_, _cbt_, __VA_ARGS__) \
		else { \
			SECONDARY_IF(_nr_, CBT_DRAW_3D_GEOMETRY, __VA_ARGS__) \
			ELSE_DRIVER(_nr_, _driver_action_, __VA_ARGS__) \
		} \
	} \
} while(0)

#define CALLBACK_IF_ELSE_UNROLL_ELSE_DRIVER(_nr_, _cbt_, _unroll_cbt_, _unroll_action_, _driver_action_, ...) do { \
	PRIMARY_IF(_nr_, _cbt_, __VA_ARGS__) \
	else { \
		AR_Action const &	_ar_action_ = AR_ACTION(_ar_, (_unroll_cbt_)); \
		if (_ar_action_.address != nullroutine) { \
			ASSOCIATE_CALLBACK_DATA(_dc_, _ar_action_); \
			_unroll_action_(_nr_, __VA_ARGS__); \
			UNASSOCIATE_CALLBACK_DATA(_dc_); \
		} \
		ELSE_DRIVER(_nr_, _driver_action_, __VA_ARGS__) \
	} \
} while(0)

#define CALLBACK_UNROLL_ELSE_DRIVER(_nr_, _cbt_, _unroll_action_, _driver_action_, ...) do { \
	Display_Context const *		_dc_ = (_nr_)->display_context; \
	Action_Rendition const &	_ar_ = (_nr_)->action_rendition; \
	AR_Action const &	_ar_action_ = AR_ACTION(_ar_, (_cbt_)); \
	if (_ar_action_.address != nullroutine) { \
		ASSOCIATE_CALLBACK_DATA(_dc_, _ar_action_); \
		_unroll_action_(_nr_, __VA_ARGS__); \
		UNASSOCIATE_CALLBACK_DATA(_dc_); \
	} \
	ELSE_DRIVER(_nr_, _driver_action_, __VA_ARGS__) \
} while(0)




#define HIA_Populate_Segment(nr, key) CALLBACK_IF(nr, CBT_POPULATE_SEGMENT, key)

#define HIA_Set_Shader(nr, driver_type, geom_type, geom_bits, thing) CALLBACK_IF(nr, CBT_SET_SHADER, driver_type, geom_type, geom_bits, thing)

#define HIA_Driver_Startup(nr) CALLBACK_IF_NA(nr, CBT_DRIVER_STARTUP)
#define HIA_Driver_Shutdown(nr) CALLBACK_IF_NA(nr, CBT_DRIVER_SHUTDOWN)

#define HIA_Draw_DC_Face(nr, count, points) CALLBACK_IF_ELSE_DRIVER(nr, CBT_DRAW_DC_FACE, draw_dc_face, count, points)
#define HIA_Draw_DC_Colorized_Face(nr, count, points, rgb) CALLBACK_IF_ELSE_DRIVER(nr, CBT_DRAW_DC_COLORIZED_FACE, draw_dc_colorized_face, count, points, rgb)
#define HIA_Draw_DC_Cut_Line(nr, count, points) CALLBACK_IF_ELSE_DRIVER(nr, CBT_DRAW_DC_CUT_LINE, draw_dc_polyline, count, points)
#define HIA_Draw_DC_Cut_Line(nr, count, points) CALLBACK_IF_ELSE_DRIVER(nr, CBT_DRAW_DC_CUT_LINE, draw_dc_polyline, count, points)
#define HIA_Draw_Text(nr, text_info) CALLBACK_IF_ELSE_DRIVER(nr, CBT_DRAW_TEXT, draw_inter_text, text_info)

#define HIA_Draw_Window(nr) CALLBACK_IF_ELSE_DRIVER(nr, CBT_DRAW_WINDOW, draw_window, &(nr)->window_rendition->net_extent)
#define HIA_Draw_Window_Frame(nr, extent, frame, calc) CALLBACK_IF_ELSE_DRIVER(nr, CBT_DRAW_WINDOW_FRAME, draw_window_frame, extent, frame, calc)

#define HIA_Init_Picture(nr) CALLBACK_IF_ELSE_DRIVER_NA(nr, CBT_INIT_PICTURE, init_picture)
#define HIA_Finish_Picture(nr, swap_buffers) CALLBACK_IF_ELSE_DRIVER(nr, CBT_FINISH_PICTURE, finish_picture, swap_buffers)

#define HIA_Create_Region(nr, area, image_buffer, z_buffer) CALLBACK_IF_ELSE_DRIVER(nr, CBT_CREATE_REGION, create_region, area, image_buffer, z_buffer)
#define HIA_Destroy_Region(nr, image_buffer, z_buffer) CALLBACK_IF_ELSE_DRIVER(nr, CBT_DESTROY_REGION, destroy_region, image_buffer, z_buffer)
#define HIA_Save_Region(nr, area, image_buffer, z_buffer) CALLBACK_IF_ELSE_DRIVER(nr, CBT_SAVE_REGION, save_region, area, image_buffer, z_buffer)
#define HIA_Restore_Region(nr, area, image_buffer, z_buffer) CALLBACK_IF_ELSE_DRIVER(nr, CBT_RESTORE_REGION, restore_region, area, image_buffer, z_buffer)

#define HIA_Draw_HLR_Polyline(nr, count, points) CALLBACK_IF_ELSE_DRIVER(nr, CBT_HLR_POLYLINE, draw_dc_polyline, count, points)
#define HIA_Draw_HLR_Polydot(nr, point) CALLBACK_IF_ELSE_DRIVER(nr, CBT_HLR_POLYDOT, draw_dc_polydot, 1, point)
#define HIA_Draw_HLR_Colorized_Polydot(nr, point, color) CALLBACK_IF_ELSE_DRIVER(nr, CBT_HLR_COLORIZED_POLYDOT, draw_dc_colorized_polydot, 1, point, color, true)
#define HIA_Draw_HLR_Hidden_Polyline(nr, count, points) CALLBACK_IF_ELSE_DRIVER(nr, CBT_HLR_HIDDEN_POLYLINE, draw_dc_polyline, count, points)
#define HIA_Draw_HLR_Hidden_Polydot(nr, point) CALLBACK_IF_ELSE_DRIVER(nr, CBT_HLR_HIDDEN_POLYDOT, draw_dc_polydot, 1, point)
#define HIA_Draw_HLR_Hidden_Colorized_Polydot(nr, point, color) CALLBACK_IF_ELSE_DRIVER(nr, CBT_HLR_HIDDEN_COLORIZED_POLYDOT, draw_dc_colorized_polydot, 1, point, color, true)

#define HIA_Draw_HLR_Marker(nr, count, points, rotations, size_fixups) \
	CALLBACK_UNROLL_ELSE_DRIVER(nr, CBT_HLR_MARKER, \
	HD_Unwrap_HLR_Polymarker, draw_dc_polymarker, count, points, rotations, size_fixups)

#define HIA_Draw_HLR_Hidden_Marker(nr, count, points, rotations, size_fixups) \
	CALLBACK_UNROLL_ELSE_DRIVER(nr, CBT_HLR_HIDDEN_MARKER, \
	HD_Unwrap_HLR_Hidden_Polymarker, draw_dc_polymarker, count, points, rotations, size_fixups)


#define HIA_Draw_DC_Polytriangle(nr, count, points) \
	CALLBACK_IF_ELSE_UNROLL_ELSE_DRIVER(nr, CBT_DRAW_DC_POLYTRIANGLE, CBT_DRAW_DC_TRIANGLE, \
	HD_Unwrap_Polytriangle, draw_dc_polytriangle, count, points)

#define HIA_Draw_DC_Colorized_Polytriangle(nr, count, points, color, single) \
	CALLBACK_IF_ELSE_UNROLL_ELSE_DRIVER(nr, CBT_DRAW_DC_COLORIZED_POLYTRIANGLE, CBT_DRAW_DC_COLORIZED_TRIANGLE, \
	HD_Unwrap_Colorized_Polytriangle, draw_dc_colorized_polytriangle, count, points, color, single)

#define HIA_Draw_DC_Gouraud_Polytriangle(nr, count, points, colors) \
	CALLBACK_IF_ELSE_UNROLL_ELSE_DRIVER(nr, CBT_DRAW_DC_GOURAUD_POLYTRIANGLE, CBT_DRAW_DC_GOURAUD_TRIANGLE, \
	HD_Unwrap_Gouraud_Polytriangle, draw_dc_gouraud_polytriangle, count, points, colors)

#define HIA_Draw_DC_Phong_Polytriangle(nr, count, points, colors, planes, dplanes) \
	CALLBACK_IF_ELSE_UNROLL_ELSE_DRIVER(nr, CBT_DRAW_DC_PHONG_POLYTRIANGLE, CBT_DRAW_DC_RESHADED_TRIANGLE, \
	HD_Unwrap_Phong_Polytriangle, draw_dc_phong_polytriangle, count, points, colors, planes, dplanes)

#define HIA_Draw_DC_Textured_Polytriangle(nr, count, points, colors, planes, dplanes, params, param_width, param_flags) \
	CALLBACK_IF_ELSE_UNROLL_ELSE_DRIVER(nr, CBT_DRAW_DC_TEXTURED_POLYTRIANGLE, CBT_DRAW_DC_RESHADED_TRIANGLE, \
	HD_Unwrap_Reshaded_Polytriangle, draw_dc_textured_polytriangle, count, points, colors, planes, dplanes, params, param_width, param_flags)

#define HIA_Draw_DC_Polyline(nr, count, points) \
	CALLBACK_IF_ELSE_UNROLL_ELSE_DRIVER(nr, CBT_DRAW_DC_POLYLINE, CBT_DRAW_DC_LINE, \
	HD_Unwrap_Polyline, draw_dc_polyline, count, points)

#define HIA_Draw_DC_Colorized_Polyline(nr, count, points, color, single) \
	CALLBACK_IF_ELSE_UNROLL_ELSE_DRIVER(nr, CBT_DRAW_DC_COLORIZED_POLYLINE, CBT_DRAW_DC_COLORIZED_LINE, \
	HD_Unwrap_Colorized_Polyline, draw_dc_colorized_polyline, count, points, color, single)

#define HIA_Draw_DC_Gouraud_Polyline(nr, count, points, colors) \
	CALLBACK_IF_ELSE_UNROLL_ELSE_DRIVER(nr, CBT_DRAW_DC_GOURAUD_POLYLINE, CBT_DRAW_DC_GOURAUD_LINE, \
	HD_Unwrap_Gouraud_Polyline, draw_dc_gouraud_polyline, count, points, colors)

#define HIA_Draw_DC_Phong_Polyline(nr, count, points, colors, planes, dplanes) \
	CALLBACK_IF_ELSE_UNROLL_ELSE_DRIVER(nr, CBT_DRAW_DC_PHONG_POLYLINE, CBT_DRAW_DC_RESHADED_LINE, \
	HD_Unwrap_Phong_Polyline, draw_dc_phong_polyline, count, points, colors, planes, dplanes)

#define HIA_Draw_DC_Textured_Polyline(nr, count, points, colors, planes, dplanes, params, param_width, param_flags) \
	CALLBACK_IF_ELSE_UNROLL_ELSE_DRIVER(nr, CBT_DRAW_DC_TEXTURED_POLYLINE, CBT_DRAW_DC_RESHADED_LINE, \
	HD_Unwrap_Reshaded_Polyline, draw_dc_textured_polyline, count, points, colors, planes, dplanes, params, param_width, param_flags)

#define HIA_Draw_DC_Polymarker(nr, count, points, rotations, size_fixups) \
	CALLBACK_IF_ELSE_UNROLL_ELSE_DRIVER(nr, CBT_DRAW_DC_POLYMARKER, CBT_DRAW_DC_MARKER, \
	HD_Unwrap_Polymarker, draw_dc_polymarker, count, points, rotations, size_fixups)

#define HIA_Draw_DC_Colorized_Polymarker(nr, count, points, color, single_color, rotations, size_fixups) \
	CALLBACK_IF_ELSE_UNROLL_ELSE_DRIVER(nr, CBT_DRAW_DC_COLORIZED_POLYMARKER, CBT_DRAW_DC_COLORIZED_MARKER, \
	HD_Unwrap_Colorized_Polymarker, draw_dc_colorized_polymarker, count, points, color, single_color, rotations, size_fixups)


#define HIA_Draw_3D_Marker(nr, marker) \
	CALLBACK_MASKED_IF_ELSE_GEOMETRY_ELSE_DRIVER(nr, Action_Mask_DRAW_3D_MARKER, CBT_DRAW_3D_MARKER, draw_3d_marker, marker)
#define HIA_Draw_3D_Multimarker(nr, multimarker) \
	CALLBACK_MASKED_IF_ELSE_GEOMETRY_ELSE_DRIVER(nr, Action_Mask_DRAW_3D_MARKER, CBT_DRAW_3D_MULTIMARKER, draw_3d_multimarker, multimarker)
#define HIA_Draw_3D_Polyline(nr, polyline) \
	CALLBACK_MASKED_IF_ELSE_GEOMETRY_ELSE_DRIVER(nr, Action_Mask_DRAW_3D_POLYLINE, CBT_DRAW_3D_POLYLINE, draw_3d_polyline, polyline)
#define HIA_Draw_3D_NURBS_Curve(nr, curve) \
	CALLBACK_MASKED_IF_ELSE_GEOMETRY_ELSE_DRIVER(nr, Action_Mask_DRAW_3D_NURBS_CURVE, CBT_DRAW_3D_NURBS_CURVE, draw_3d_nurbs_curve, curve)
#define HIA_Draw_3D_NURBS_Surface(nr, surface) \
	CALLBACK_MASKED_IF_ELSE_GEOMETRY_ELSE_DRIVER(nr, Action_Mask_DRAW_3D_NURBS_SURFACE, CBT_DRAW_3D_NURBS_SURFACE, draw_3d_nurbs_surface, surface)
#define HIA_Draw_3D_Cylinder(nr, cylinder) \
	CALLBACK_MASKED_IF_ELSE_GEOMETRY_ELSE_DRIVER(nr, Action_Mask_DRAW_3D_CYLINDER, CBT_DRAW_3D_CYLINDER, draw_3d_cylinder, cylinder)
#define HIA_Draw_3D_PolyCylinder(nr, polycylinder) \
	CALLBACK_MASKED_IF_ELSE_GEOMETRY_ELSE_DRIVER(nr, Action_Mask_DRAW_3D_POLYCYLINDER, CBT_DRAW_3D_POLYCYLINDER, draw_3d_polycylinder, polycylinder)
#define HIA_Draw_3D_Sphere(nr, sphere) \
	CALLBACK_MASKED_IF_ELSE_GEOMETRY_ELSE_DRIVER(nr, Action_Mask_DRAW_3D_SPHERE, CBT_DRAW_3D_SPHERE, draw_3d_sphere, sphere)
#define HIA_Draw_3D_Polygon(nr, polygon) \
	CALLBACK_MASKED_IF_ELSE_GEOMETRY_ELSE_DRIVER(nr, Action_Mask_DRAW_3D_POLYGON, CBT_DRAW_3D_POLYGON, draw_3d_polygon, polygon)
#define HIA_Draw_3D_Text(nr, text) \
	CALLBACK_MASKED_IF_ELSE_GEOMETRY_ELSE_DRIVER(nr, Action_Mask_DRAW_3D_TEXT, CBT_DRAW_3D_TEXT, draw_3d_text, text)
#define HIA_Draw_3D_Grid(nr, grid) \
	CALLBACK_MASKED_IF_ELSE_GEOMETRY_ELSE_DRIVER(nr, Action_Mask_DRAW_3D_GRID, CBT_DRAW_3D_GRID, draw_3d_grid, grid)
#define HIA_Draw_3D_Polyhedron(nr, polyhedron) \
	CALLBACK_MASKED_IF_ELSE_GEOMETRY_ELSE_DRIVER(nr, Action_Mask_DRAW_3D_POLYHEDRON, CBT_DRAW_3D_POLYHEDRON, draw_3d_polyhedron, polyhedron)
#define HIA_Draw_3D_Ellipse(nr, ellipse) \
	CALLBACK_MASKED_IF_ELSE_GEOMETRY_ELSE_DRIVER(nr, Action_Mask_DRAW_3D_ELLIPSE, CBT_DRAW_3D_ELLIPSE, draw_3d_ellipse, ellipse)
#define HIA_Draw_3D_Elliptical_Arc(nr, elliptical_arc) \
	CALLBACK_MASKED_IF_ELSE_GEOMETRY_ELSE_DRIVER(nr, Action_Mask_DRAW_3D_ELLIPTICAL_ARC, CBT_DRAW_3D_ELLIPTICAL_ARC, draw_3d_elliptical_arc, elliptical_arc)

#define HIA_Draw_3D_Tristrip(nr, ts) \
	CALLBACK_MASKED_IF_ELSE_DRIVER(nr, Action_Mask_DRAW_3D_TRISTRIP, CBT_DRAW_3D_TRISTRIP, draw_3d_tristrip, ts)
#define HIA_Draw_Indexicated_3D_Tristrip(nr, ts) \
	CALLBACK_MASKED_IF_ELSE_DRIVER(nr, Action_Mask_DRAW_INDEXED_TRISTRIP, CBT_DRAW_3D_TRISTRIP, draw_indexed_tristrip, ts)
#define HIA_Draw_3D_Polyedge(nr, pe) \
	CALLBACK_MASKED_IF_ELSE_DRIVER(nr, Action_Mask_DRAW_3D_POLYEDGE, CBT_DRAW_3D_POLYEDGE, draw_3d_polyedge, pe)
#define HIA_Draw_3D_Isoline(nr, pe) \
	CALLBACK_MASKED_IF_ELSE_DRIVER(nr, Action_Mask_DRAW_3D_ISOLINE, CBT_DRAW_3D_ISOLINE, draw_3d_polyedge, pe)
#define HIA_Draw_Indexicated_3D_Polyedge(nr, pe) \
	CALLBACK_MASKED_IF_ELSE_DRIVER(nr, Action_Mask_DRAW_INDEXED_POLYEDGE, CBT_DRAW_3D_POLYEDGE, draw_indexed_polyedge, pe)
#define HIA_Draw_3D_Polymarker(nr, pm) \
	CALLBACK_MASKED_IF_ELSE_DRIVER(nr, Action_Mask_DRAW_3D_POLYMARKER, CBT_DRAW_3D_POLYMARKER, draw_3d_polymarker, pm)


#define	 HIA_Draw_Segment_Tree(nr, si) \
do { \
	Display_Context const *		_dc_ = (nr)->display_context; \
	Action_Rendition const &	_ar_ = (nr)->action_rendition; \
	AR_Action const &			_ar_action_ = AR_ACTION(_ar_, CBT_DRAW_SEGMENT_TREE); \
	if (_ar_action_.address != nullroutine) { \
		Net_Rendition		subnr = nr; \
		Action_Rendition &	subar = subnr.Modify()->action_rendition.Modify(); \
		if (!AR_ACTION_NULL(subar, CBT_DRAW_SEGMENT_TREE)) { \
			--subar->active_count; \
			if (subar->active_count == 0) \
				subnr->flags &= ~NRF_Active_Callbacks; \
		} \
		AR_ACTION_ASSIGN_NULL(subar, CBT_DRAW_SEGMENT_TREE); \
		ASSOCIATE_CALLBACK_DATA(_dc_, _ar_action_); \
		CALLBACK_ACTION(_ar_action_, CBT_DRAW_SEGMENT_TREE) (&subnr, si); \
		UNASSOCIATE_CALLBACK_DATA(_dc_); \
	} else {\
		AR_Action const &	_ar_action_ = AR_ACTION(_ar_, CBT_DRAW_SEGMENT); \
		ASSOCIATE_CALLBACK_DATA(_dc_, _ar_action_); \
		CALLBACK_ACTION(_ar_action_, CBT_DRAW_SEGMENT) (&nr, si); \
		UNASSOCIATE_CALLBACK_DATA(_dc_); \
	}\
} while (0)

#define	 HIA_Select_Segment_Tree(nr, si) \
do { \
	Display_Context const *		_dc_ = (nr)->display_context; \
	Action_Rendition const &	_ar_ = (nr)->action_rendition; \
	AR_Action const &			_ar_action_ = AR_ACTION(_ar_, CBT_SELECT_SEGMENT_TREE); \
	if (_ar_action_.address != nullroutine) { \
		Net_Rendition		subnr = nr; \
		Action_Rendition &	subar = subnr.Modify()->action_rendition.Modify(); \
		AR_ACTION_ASSIGN_NULL(subar, CBT_SELECT_SEGMENT_TREE); \
		ASSOCIATE_CALLBACK_DATA(_dc_, _ar_action_); \
		CALLBACK_ACTION(_ar_action_, CBT_SELECT_SEGMENT_TREE) (&subnr, si); \
		UNASSOCIATE_CALLBACK_DATA(_dc_); \
	} else {\
		AR_Action const &	_ar_action_ = AR_ACTION(_ar_, CBT_SELECT_SEGMENT); \
		ASSOCIATE_CALLBACK_DATA(_dc_, _ar_action_); \
		CALLBACK_ACTION(_ar_action_, CBT_SELECT_SEGMENT) (&nr, si); \
		UNASSOCIATE_CALLBACK_DATA(_dc_); \
	} \
} while (0)




//Internal HII macros

#define HII_Draw_Silhouette_Polyedge(nr, pe) \
do { \
	Display_Context const *		_dc_ = (nr)->display_context; \
	Action_Table const &		_current_actions_ = _dc_->current_actions; \
	if (ANYBIT (_current_actions_->mask, Action_Mask_DRAW_SILHOUETTE_POLYEDGE)) \
		DRIVER_ACTION(_current_actions_, draw_silhouette_polyedge)(nr, pe); \
} while (0)

#define HII_Draw_NonCulled_Polyedge(nr, pe) \
do { \
	Display_Context const *		_dc_ = (nr)->display_context; \
	Action_Table const &		_current_actions_ = _dc_->current_actions; \
	if (ANYBIT (_current_actions_->mask, Action_Mask_DRAW_NONCULLED_POLYEDGE)) \
		DRIVER_ACTION(_current_actions_, draw_nonculled_polyedge)(nr, pe); \
} while (0)

#define HII_Draw_Tree(nr, tree, ip) \
do { \
	Display_Context const *		_dc_ = (nr)->display_context; \
	Action_Table const &		_current_actions_ = _dc_->current_actions; \
	if (ANYBIT (_current_actions_->mask, Action_Mask_DRAW_TREE)) { \
		DRIVER_ACTION(_current_actions_, draw_tree)(nr, tree, ip); \
	} \
} while (0)

#define HII_Render(nr, gg, mm, ss) \
	(ANYBIT ((nr)->display_context->current_actions->mask, Action_Mask_RENDER) ? \
		DRIVER_ACTION((nr)->display_context->current_actions, render)(nr, gg, mm, ss) : null)


/* internal dc drawing */

#define HII_Draw_Plain_Text(nr, text, position, count, kp, ki) \
	DRIVER_ACTION((nr)->display_context->current_actions, draw_plain_text)(nr, text, position, count, kp, ki)

#define HII_Draw_DC_Stenciled_Quad(nr, points, stencil) \
	DRIVER_ACTION((nr)->display_context->current_actions, draw_dc_stenciled_quad)(nr, points, stencil)

#define HII_Draw_DC_Dot(nr, point) \
	DRIVER_ACTION((nr)->display_context->current_actions, draw_dc_polydot)(nr, 1, point)

#define HII_Draw_DC_Polydot(nr, count, point) \
	DRIVER_ACTION((nr)->display_context->current_actions, draw_dc_polydot)(nr, count, point)

#define HII_Draw_DC_Colorized_Polydot(nr, count, point, color, single) \
	DRIVER_ACTION((nr)->display_context->current_actions, draw_dc_colorized_polydot)(nr, count, point, color, single)

#define HII_Draw_DC_Polygon(nr, count, points) \
	DRIVER_ACTION((nr)->display_context->current_actions, draw_dc_polygon)(nr, count, points)

#define HII_Draw_DC_Face(nr, count, points) \
	DRIVER_ACTION((nr)->display_context->current_actions, draw_dc_face)(nr, count, points)

#define HII_Draw_DC_Ellipse(nr, where, radius) \
	DRIVER_ACTION((nr)->display_context->current_actions, draw_dc_ellipse)(nr, where, radius)

#define HII_Draw_DC_Elliptical_Arc(nr, where, radius, start, end) \
	DRIVER_ACTION((nr)->display_context->current_actions, draw_dc_elliptical_arc)(nr, where, radius, start, end)

#define HII_Draw_DC_Outline_Ellipse(nr, where, radius) \
	DRIVER_ACTION((nr)->display_context->current_actions, draw_dc_outline_ellipse)(nr, where, radius)

#define HII_Draw_DC_Colorized_Face(nr, count, points, color) \
	DRIVER_ACTION((nr)->display_context->current_actions, draw_dc_colorized_face)(nr, count, points, color)

#define HII_Draw_DC_Rectangle(nr, left, right, bottom, top) \
	DRIVER_ACTION((nr)->display_context->current_actions, draw_dc_rectangle)(nr, left, right, bottom, top)

#define HII_Draw_DC_Image(nr, start, end, hoffset, format, rowbytes, rasters, pattern, contrast_color, dbi) \
	DRIVER_ACTION((nr)->display_context->current_actions, draw_dc_image)(nr, start, end, hoffset, format, rowbytes, rasters, pattern, contrast_color, dbi)

#define HII_Draw_DC_Text(nr, where, count, string) \
	DRIVER_ACTION((nr)->display_context->current_actions, draw_dc_text)(nr, where, count, string)

#define HII_Draw_DC_Triangle(nr, points) \
	DRIVER_ACTION((nr)->display_context->current_actions, draw_dc_polytriangle)(nr, 3, points)

#define HII_Draw_DC_Colorized_Triangle(nr, points, color) \
	DRIVER_ACTION((nr)->display_context->current_actions, draw_dc_colorized_polytriangle)(nr, 3, points, color, true)

#define HII_Draw_DC_Gouraud_Triangle(nr, points, colors) \
	DRIVER_ACTION((nr)->display_context->current_actions, draw_dc_gouraud_polytriangle)(nr, 3, points, colors)

#define HII_Draw_DC_Reshaded_Triangle(nr, points, colors, planes, dplanes, params, param_width, param_flags) \
	DRIVER_ACTION((nr)->display_context->current_actions, draw_dc_textured_polytriangle)(nr, 3, points, colors, planes, dplanes, params, param_width, param_flags)

#define HII_Draw_DC_Polytriangle(nr, count, points) \
	DRIVER_ACTION((nr)->display_context->current_actions, draw_dc_polytriangle)(nr, count, points)

#define HII_Draw_DC_Colorized_Polytriangle(nr, count, points, color, single) \
	DRIVER_ACTION((nr)->display_context->current_actions, draw_dc_colorized_polytriangle)(nr, count, points, color, single)

#define HII_Draw_DC_Gouraud_Polytriangle(nr, count, points, colors) \
	DRIVER_ACTION((nr)->display_context->current_actions, draw_dc_gouraud_polytriangle)(nr, count, points, colors)

#define HII_Draw_DC_Phong_Polytriangle(nr, count, points, colors, planes, dplanes) \
	DRIVER_ACTION((nr)->display_context->current_actions, draw_dc_phong_polytriangle)(nr, count, points, colors, planes, dplanes)

#define HII_Draw_DC_Textured_Polytriangle(nr, count, points, colors, planes, dplanes, params, param_width, param_flags) \
	DRIVER_ACTION((nr)->display_context->current_actions, draw_dc_textured_polytriangle)(nr, count, points, colors, planes, dplanes, params, param_width, param_flags)

#define HII_Draw_DC_Marker(nr, point) \
	DRIVER_ACTION((nr)->display_context->current_actions, draw_dc_polymarker)(nr, 1, point, null, null)

#define HII_Draw_DC_Polymarker(nr, count, points, rotations, size_fixups) \
	DRIVER_ACTION((nr)->display_context->current_actions, draw_dc_polymarker)(nr, count, points, rotations, size_fixups)

#define HII_Draw_DC_Colorized_Polymarker(nr, count, points, colors, single, rotations, size_fixups) \
	DRIVER_ACTION((nr)->display_context->current_actions, draw_dc_colorized_polymarker)(nr, count, points, colors, single, rotations, size_fixups)

#define HII_Draw_DC_Line(nr, points) \
	DRIVER_ACTION((nr)->display_context->current_actions, draw_dc_polyline)(nr, 2, points)

#define HII_Draw_DC_Colorized_Line(nr, points, color) \
	DRIVER_ACTION((nr)->display_context->current_actions, draw_dc_colorized_polyline)(nr, 2, points, color, true)

#define HII_Draw_DC_Gouraud_Line(nr, points, colors) \
	DRIVER_ACTION((nr)->display_context->current_actions, draw_dc_gouraud_polyline)(nr, 2, points, colors)

#define HII_Draw_DC_Reshaded_Line(nr, points, colors, planes, dplanes, params, param_width, param_flags) \
	DRIVER_ACTION((nr)->display_context->current_actions, draw_dc_textured_polyline)(nr, 2, points, colors, planes, dplanes, params, param_width, param_flags)

#define HII_Draw_DC_Polyline(nr, count, points) \
	DRIVER_ACTION((nr)->display_context->current_actions, draw_dc_polyline)(nr, count, points)

#define HII_Draw_DC_Colorized_Polyline(nr, count, points, color, single) \
	DRIVER_ACTION((nr)->display_context->current_actions, draw_dc_colorized_polyline)(nr, count, points, color, single)

#define HII_Draw_DC_Gouraud_Polyline(nr, count, points, colors) \
	DRIVER_ACTION((nr)->display_context->current_actions, draw_dc_gouraud_polyline)(nr, count, points, colors)

#define HII_Draw_DC_Phong_Polyline(nr, count, points, colors, planes, dplanes) \
	DRIVER_ACTION((nr)->display_context->current_actions, draw_dc_phong_polyline)(nr, count, points, colors, planes, dplanes)

#define HII_Draw_DC_Textured_Polyline(nr, count, points, colors, planes, dplanes, params, param_width, param_flags) \
	DRIVER_ACTION((nr)->display_context->current_actions, draw_dc_textured_polyline)(nr, count, points, colors, planes, dplanes, params, param_width, param_flags)


/* internal 3d drawing */

#define HII_Draw_3D_Ellipse(nr, ellipse) \
	DRIVER_ACTION((nr)->display_context->current_actions, draw_3d_ellipse)(nr, ellipse)

#define HII_Draw_3D_Elliptical_Arc(nr, arc) \
	DRIVER_ACTION((nr)->display_context->current_actions, draw_3d_elliptical_arc)(nr, arc)

#define HII_Draw_3D_NURBS_Curve(nr, curve) \
	DRIVER_ACTION((nr)->display_context->current_actions, draw_3d_nurbs_curve)(nr, curve)

#define HII_Draw_3D_NURBS_Surface(nr, surface) \
	DRIVER_ACTION((nr)->display_context->current_actions, draw_3d_nurbs_surface)(nr, surface)

#define HII_Draw_3D_Cylinder(nr, cylinder) \
	DRIVER_ACTION((nr)->display_context->current_actions, draw_3d_cylinder)(nr, cylinder)

#define HII_Draw_3D_PolyCylinder(nr, polycylinder) \
	DRIVER_ACTION((nr)->display_context->current_actions, draw_3d_polycylinder)(nr, polycylinder)

#define HII_Draw_3D_Sphere(nr, sphere) \
	DRIVER_ACTION((nr)->display_context->current_actions, draw_3d_sphere)(nr, sphere)

#define HII_Draw_3D_Grid(nr, grid) \
	DRIVER_ACTION((nr)->display_context->current_actions, draw_3d_grid)(nr, grid)

#define HII_Draw_3D_Image(nr, image) \
	DRIVER_ACTION((nr)->display_context->current_actions, draw_3d_image)(nr, image)

#define HII_Draw_3D_Light(nr, light) \
	DRIVER_ACTION((nr)->display_context->current_actions, draw_3d_light)(nr, light)

#define HII_Draw_3D_Polymarker(nr, pm) \
	DRIVER_ACTION((nr)->display_context->current_actions, draw_3d_polymarker)(nr, pm)

#define HII_Draw_3D_Marker(nr, marker) \
	DRIVER_ACTION((nr)->display_context->current_actions, draw_3d_marker)(nr, marker)

#define HII_Draw_3D_Multimarker(nr, multimarker) \
	DRIVER_ACTION((nr)->display_context->current_actions, draw_3d_multimarker)(nr, multimarker)

#define HII_Draw_3D_Polyline(nr, polyline) \
	DRIVER_ACTION((nr)->display_context->current_actions, draw_3d_polyline)(nr, polyline)

#define HII_Draw_3D_Infinite_Line(nr, polyline) \
	DRIVER_ACTION((nr)->display_context->current_actions, draw_3d_infinite_line)(nr, polyline)

#define HII_Draw_3D_Polygon(nr, polygon) \
	DRIVER_ACTION((nr)->display_context->current_actions, draw_3d_polygon)(nr, polygon)

#define HII_Draw_3D_Text(nr, text) \
	DRIVER_ACTION((nr)->display_context->current_actions, draw_3d_text)(nr, text)

#define HII_Draw_3D_Stenciled_Quad(nr, points, stencil) \
	DRIVER_ACTION((nr)->display_context->current_actions, draw_3d_stenciled_quad)(nr, points, stencil)

#define HII_Draw_3D_Tristrip(nr, ts) \
do { \
	Display_Context const *		_dc_ = (nr)->display_context; \
	Action_Table const &		_current_actions_ = _dc_->current_actions; \
	if ((ts)->vertex_findices != null) \
		DRIVER_ACTION(_current_actions_, draw_indexed_tristrip)(nr, ts); \
	else \
		DRIVER_ACTION(_current_actions_, draw_3d_tristrip)(nr, ts); \
} while(0)

#define HII_Draw_3D_Polyhedron(nr, phon) \
		DRIVER_ACTION((nr)->display_context->current_actions, draw_3d_polyhedron)(nr, phon);


#define HII_Draw_3D_Polyedge(nr, pe) \
do { \
	Display_Context const *		_dc_ = (nr)->display_context; \
	Action_Table const &		_current_actions_ = _dc_->current_actions; \
	if ((pe)->vertex_findices != null) \
		DRIVER_ACTION(_current_actions_, draw_indexed_polyedge)(nr, pe); \
	else \
		DRIVER_ACTION(_current_actions_, draw_3d_polyedge)(nr, pe); \
} while(0)

#define HII_Draw_3D_Polyedge_From_Tristrip(nr, pe, ts) \
	DRIVER_ACTION((nr)->display_context->current_actions, draw_3d_polyedge_from_tristrip)(nr, pe, ts)


/* internal raster drawing */

#define HII_Draw_DC_Bit_Rasters(nr, start, end, rowbytes, rasters) \
	DRIVER_ACTION((nr)->display_context->current_actions, draw_dc_bit_rasters)(nr, start, end, rowbytes, rasters)

#define HII_Draw_DC_Gray8_Rasters(nr, start, end, rowbytes, rasters) \
	DRIVER_ACTION((nr)->display_context->current_actions, draw_dc_gray8_rasters)(nr, start, end, rowbytes, rasters)

#define HII_Draw_DC_Mapped8_Rasters(nr, start, end, rowbytes, rasters) \
	DRIVER_ACTION((nr)->display_context->current_actions, draw_dc_mapped8_rasters)(nr, start, end, rowbytes, rasters)

#define HII_Draw_DC_Mapped16_Rasters(nr, start, end, rowbytes, rasters) \
	DRIVER_ACTION((nr)->display_context->current_actions, draw_dc_mapped16_rasters)(nr, start, end, rowbytes, rasters)

#define HII_Draw_DC_RGB32_Rasters(nr, start, end, rowbytes, rasters) \
	DRIVER_ACTION((nr)->display_context->current_actions, draw_dc_rgb32_rasters)(nr, start, end, rowbytes, rasters)

/* internal span routines */

#define HII_Draw_DC_Spans(nr, sr, spans, num_spans, top) \
	DRIVER_ACTION((nr)->display_context->current_actions, draw_dc_spans)(nr, sr, spans, num_spans, top)

#define HII_Draw_DC_Colorized_Spans(nr, sr, spans, num_spans, top) \
	DRIVER_ACTION((nr)->display_context->current_actions, draw_dc_colorized_spans)(nr, sr, spans, num_spans, top)

#define HII_Draw_DC_Gouraud_Spans(nr, sr, spans, num_spans, top) \
	DRIVER_ACTION((nr)->display_context->current_actions, draw_dc_gouraud_spans)(nr, sr, spans, num_spans, top)

#define HII_Draw_DC_Reshaded_Spans(nr, sr, spans, num_spans, top, param_flags) \
	DRIVER_ACTION((nr)->display_context->current_actions, draw_dc_reshaded_spans)(nr, sr, spans, num_spans, top, param_flags)


/* buffer activity */

#define HII_Create_Frame_Buffer(nr, width, height, format, image, driver_specific) \
	DRIVER_ACTION((nr)->display_context->current_actions, create_frame_buffer) (nr, width, height, format, image, driver_specific)

#define HII_Display_Frame_Buffer(nr, start, end, image, hoffset, voffset, driver_specific) \
	DRIVER_ACTION((nr)->display_context->current_actions, display_frame_buffer) (nr, start, end, image, hoffset, voffset, driver_specific)

#define HII_Destroy_Frame_Buffer(nr, image, driver_specific) \
	DRIVER_ACTION((nr)->display_context->current_actions, destroy_frame_buffer) (nr, image, driver_specific)

/* occlusion query */

INLINE void * HII_Begin_Occlusion_Query(Net_Rendition const & nr, bool disable_write)
{
	Display_Context const *		dc = nr->display_context;
	Action_Table const &		current_actions = dc->current_actions;
	if (dc->actions->begin_occlusion_query)
		return DRIVER_ACTION(current_actions, begin_occlusion_query)(nr, disable_write);
	return (void *)null;
}

INLINE void HII_End_Occlusion_Query(Net_Rendition const & nr, void * query_id)
{
	Display_Context const *		dc = nr->display_context;
	Action_Table const &		current_actions = dc->current_actions;
	if (dc->actions->end_occlusion_query)
		DRIVER_ACTION(current_actions, end_occlusion_query)(nr, query_id);
}

/*
 #define HII_Begin_Occlusion_Query(nr, disable_write) \
	((nr)->display_context->actions->begin_occlusion_query) ? \
		DRIVER_ACTION((nr)->display_context->current_actions, begin_occlusion_query)(nr, disable_write) : (void *)null
 
 #define HII_End_Occlusion_Query(nr, vid)  \
	((nr)->display_context->actions->end_occlusion_query) ? \
		DRIVER_ACTION((nr)->display_context->current_actions, end_occlusion_query)(nr, (vid)) : (void)(nr)
*/

#define HII_Get_Occlusion_Query(nr, vid)  \
	(((nr)->display_context->actions->get_occlusion_query) ? \
		DRIVER_ACTION((nr)->display_context->current_actions, get_occlusion_query)(nr, vid) : 0)

#define HII_Delete_Occlusion_Query(nr, vid)  \
	((nr)->display_context->actions->delete_occlusion_query) ? \
		DRIVER_ACTION((nr)->display_context->current_actions, delete_occlusion_query)(nr, vid) : (void)(nr)

#define HII_Occlusion_Query_Available(nr)  \
	((nr)->display_context->actions->begin_occlusion_query) ? true : false


/* depth peeling */

#define HII_Depth_Peeling(nr)  \
	(*((nr)->display_context)->actions->depth_peeling)(nr)

#define HII_Clear_Z_Buffer(nr, area)  \
	(*((nr)->display_context)->actions->clear_z_buffer)(nr, area)

#define HII_Toggle_XOR(nr, undraw)  \
	(*((nr)->display_context)->actions->toggle_xor)(nr, undraw)

/* segment [tree] display lists */

#define HII_Begin_Display_List(nr, dl, mm, compile_only)  \
	(((nr)->display_context->current_actions->begin_display_list) ? \
	DRIVER_ACTION((nr)->display_context->current_actions, begin_display_list)(nr, dl, mm, compile_only) : false)

#define HII_Check_Display_List(nr, dl, mm, vv)  \
	(((nr)->display_context->current_actions->check_display_list) ? \
		DRIVER_ACTION((nr)->display_context->current_actions, check_display_list)(nr, dl, mm, vv) : (void)0)

#define HII_End_Display_List(nr, dl, compile_only) do { \
	Display_Context const *		_dc_ = (nr)->display_context; \
	Action_Table const &		_current_actions_ = _dc_->current_actions; \
	if (_current_actions_->end_display_list) \
		DRIVER_ACTION(_current_actions_, end_display_list)(nr, dl, compile_only); \
} while(0)

// except for deferred geometry (e.g. depth peeling), execute is normally implicit as part of begin/end 
#define HII_Execute_Display_List(nr, dl)  \
	(((nr)->display_context->current_actions->execute_display_list) ? \
		DRIVER_ACTION((nr)->display_context->current_actions, execute_display_list)(nr, dl) : (void)0)



#define P_NVC_NFC_VN_NFN			0x10010010
#define P_NVC_NFC_VN_NFN_FIXUP		0x10F10010
#define P_NVC_NFC_VN_NFN_BUMPMAP	0x10B10010
#define P_NVC_NFC_NVN_FN			0x10010001 /*face*/
#define P_NVC_NFC_NVN_NFN			0x10010000
#define NP_VC_NFC_VN_NFN			0x10001010
#define NP_VC_NFC_NVN_FN			0x10001001 /*face*/
#define NP_VC_NFC_NVN_NFN			0x10001000
#define NP_NVC_FC_VN_NFN			0x10000110 /*face*/
#define NP_NVC_FC_NVN_FN			0x10000101 /*face*/
#define NP_NVC_FC_NVN_NFN			0x10000100 /*face*/
#define NP_NVC_NFC_VN_NFN			0x10000010
#define NP_NVC_NFC_NVN_FN			0x10000001 /*face*/
#define NP_NVC_NFC_NVN_NFN			0x10000000



End_HOOPS_Namespace

#ifndef HD_PROTO_DEFINED
#include "hd_proto.h"
#endif

Begin_HOOPS_Namespace

/* the clipping test for an elementary character box... */
INLINE bool UNCLIPPED(Transform_Rendition const & tr, Rectangle const & clip, Point const & point, int width, int height)
{
	return (
		point.x + (width-1) >= clip.left &&
		point.x <= clip.right &&
		point.y + (height-1) >= clip.bottom &&
		point.y <= clip.top &&
		// ZCLIP is broke!
		(!ANYBIT (tr->flags, TR_CUTTING_PLANES) ||
		HD_Box_Within_Cutting_Planes (tr, point.x, point.y+height-1, point.z, width, height)));
}

INLINE bool HAS_TRANSPARENCY(Material_Rendition const &	matr)
{
	return (matr->opacity != 1.0f || 
			matr->texture[MAT_TRANSMISSION] || 
		   (matr->diffuse_textures && 
			ANYBIT (matr->diffuse_textures->texture->flags, TF_TRANSPARENCY) && 
			!ANYBIT (matr->diffuse_textures->texture->bool_flags, TBF_DECAL)));
}


// This retrieves a driver color, which has already been gamma corrected.
GLOBAL_FUNCTION RGBAS32 DRIVER_COLOR_TO_RGBAS32(Display_Context const * dc, Driver_Color const & specific, int color_system);

INLINE RGBAS32 DRIVER_COLOR_TO_RGBAS32(Display_Context const * dc, Driver_Color const & specific)
{
	if (dc->physical.color_system == DIRECT_RGB)
		return specific;

	return DRIVER_COLOR_TO_RGBAS32(dc, specific, dc->physical.color_system);
}

// This retrieves a driver color, which has already been gamma corrected.
GLOBAL_FUNCTION RGB DRIVER_COLOR_TO_RGB(Display_Context const * dc, Driver_Color const & specific, int color_system);

INLINE RGB DRIVER_COLOR_TO_RGB(Display_Context const * dc, Driver_Color const & specific)
{
	if (dc->physical.color_system == DIRECT_RGB)
		return RGB(specific);

	return DRIVER_COLOR_TO_RGB(dc, specific, dc->physical.color_system);
}


// converts from floating point [0..1] rgb vs to whatever color system is active on the driver
GLOBAL_FUNCTION Driver_Color RGB_TO_DRIVER_COLOR (Display_Context const * dc, RGB const & rgb, int color_system);

INLINE Driver_Color RGB_TO_DRIVER_COLOR (Display_Context const * dc, RGB const & rgb)
{
	if (dc->physical.color_system == DIRECT_RGB)
		return RGBAS32(rgb);

	return RGB_TO_DRIVER_COLOR(dc, rgb, dc->physical.color_system);
}

void Internal_Text_Rendition::Override_Color(RGB const & rgb, bool base_too) {
	material_rendition.Modify();
	stroke_rendition = null;
	color = RGB_TO_DRIVER_COLOR (display_context, rgb);
	if (base_too)
		base_color = color;
	material_rendition->diffuse_color = rgb;
}

void Internal_Text_Rendition::Override_Contrast_Color(RGB const & rgb, bool base_too) {
	contrast_color = RGB_TO_DRIVER_COLOR (display_context, rgb);
	if (base_too)
		base_contrast_color = contrast_color;
}

void Internal_Line_Rendition::Override_Color(RGB const & rgb, bool base_too) {
	material_rendition.Modify();
	solid_rendition = null;
	face_rendition = null;
	color = RGB_TO_DRIVER_COLOR (display_context, rgb);
	if (base_too)
		base_color = color;
	material_rendition->diffuse_color = rgb;
}

void Internal_Line_Rendition::Override_Contrast_Color(RGB const & rgb, bool base_too) {
	contrast_rendition = null;
	contrast_color = RGB_TO_DRIVER_COLOR (display_context, rgb);
	if (base_too)
		base_contrast_color = contrast_color;
}

void Internal_Marker_Rendition::Override_Color(RGB const & rgb, bool base_too) {
	material_rendition.Modify();
	solid_rendition = null;
	color = RGB_TO_DRIVER_COLOR (display_context, rgb);
	if (base_too)
		base_color = color;
	material_rendition->diffuse_color = rgb;
}

void Internal_Marker_Rendition::Override_Contrast_Color(RGB const & rgb, bool base_too) {
	solid_rendition = null;
	contrast_color = RGB_TO_DRIVER_COLOR (display_context, rgb);
	if (base_too)
		base_contrast_color = contrast_color;
}

void Internal_Face_Rendition::Override_Color(RGB const & rgb, bool base_too) {
	material_rendition.Modify();
	color = RGB_TO_DRIVER_COLOR (display_context, rgb);
	if (base_too)
		base_color = color;
	material_rendition->diffuse_color = rgb;
}

void Internal_Face_Rendition::Override_Contrast_Color(RGB const & rgb, bool base_too) {
	contrast_color = RGB_TO_DRIVER_COLOR (display_context, rgb);
	if (base_too)
		base_contrast_color = contrast_color;
}

void Internal_Face_Rendition::Override_Back_Color(RGB const & rgb, bool base_too) {
	back_material_rendition.Modify();
	back_color = RGB_TO_DRIVER_COLOR (display_context, rgb);
	if (base_too)
		base_back_color = back_color;
	back_material_rendition->diffuse_color = rgb;
}

void Internal_Face_Rendition::Override_Back_Contrast_Color(RGB const & rgb, bool base_too) {
	back_contrast_color = RGB_TO_DRIVER_COLOR (display_context, rgb);
	if (base_too)
		base_back_contrast_color = back_contrast_color;
}

void Internal_Display_Rendition::Override_Window_Color(RGB const & rgb, bool base_too) {
	window.color = RGB_TO_DRIVER_COLOR (display_context, rgb);
	if (base_too)
		window.base_color = window.color;
}

void Internal_Display_Rendition::Override_Window_Contrast_Color(RGB const & rgb, bool base_too) {
	window.contrast = RGB_TO_DRIVER_COLOR (display_context, rgb);
	if (base_too)
		window.base_contrast = window.contrast;
}

void Internal_Misc_Rendition::Override_Ambient_Up_Color(RGB const & rgb, bool base_too) {
	ambient_up = rgb;
	if (base_too)
		ambient_up_base = rgb;
}

void Internal_Misc_Rendition::Override_Ambient_Down_Color(RGB const & rgb, bool base_too) {
	ambient_down = rgb;
	if (base_too)
		ambient_down_base = rgb;
}

/* Various good things useful during polyhedron drawing. */

INLINE RGB const & INDEX_TO_RGB(Color_Map const * map, int index)
{
	return map->materials[index % map->length].diffuse_color;
}


INLINE RGB FINDEX_TO_RGB(Color_Map const * map, float findex, bool interpolate)
{
	if (findex == (int)findex || !interpolate) {
		return INDEX_TO_RGB(map, (int)(findex + 0.5f));
	}
	else {
		float	frac = findex + (float)map->length; /* make positive */
		int		index = (int)frac;
		frac -= (float)index;
		return Interpolate(INDEX_TO_RGB(map, index), INDEX_TO_RGB(map, index + 1), frac);
	}
}

INLINE RGB FINDEX_TO_RGB(Color_Map const * map, float findex, bool interpolate, Tinting const & eff)
{
	RGB net = FINDEX_TO_RGB(map, findex, interpolate);

	if (ANYBIT (eff.options, Tint_ACTIVE))
		net = HI_Apply_Color_Effects (net, eff);

	return net;
}


INLINE void GAMMA_CORRECT_RGBAS32(Display_Context const * dc, RGBAS32 & clr)
{
	if (dc->options.gamma_correction) {
		clr.r = dc->physical.gamma[UNSIGNED_CHARTOINT(clr.r)];
		clr.g = dc->physical.gamma[UNSIGNED_CHARTOINT(clr.g)];
		clr.b = dc->physical.gamma[UNSIGNED_CHARTOINT(clr.b)];
	}
}

INLINE void TINT_RGBAS32(Tinting const & tinting, RGBAS32 & cl)
{
	if (ANYBIT(tinting.options, Tint_ACTIVE)) {
		RGB	tmp = HI_Apply_Color_Effects (RGB(cl), tinting);
		cl = RGBAS32 (tmp, cl.a);
	}
}

INLINE void TINT_RGBA(Tinting const & tinting, RGBA & cl)
{
	if (ANYBIT(tinting.options, Tint_ACTIVE)) {
		RGB	tmp = HI_Apply_Color_Effects (RGB(cl), tinting);
		cl = RGBA (tmp, cl.alpha);
	}
}

INLINE void COMPUTE_SHADING(
	Net_Rendition const &		nr,
	Material_Rendition const &	matr,
	bool						need_world,
	Point const *				point,
	RGB const *					color,
	Plane const *				plane,
	Vector const *				aligner,
	RGBA *						outrgb,
	RGBA *						outrgb_specular=null,
	Parameter const *			params = null,
	int32_t						param_width = 0,
	Parameter_Flags				param_flags = 0)
{
	if (need_world &&
		!nr->transform_rendition->net_modelling_matrix->Is_Identity()) {
			Point	ws = nr->transform_rendition->net_modelling_matrix->Transform(*point);
			*outrgb = HD_Standard_Compute_Shading (nr, nr->face_rendition->light_rendition, matr, &ws, color, plane, aligner,
												   params, param_width, param_flags, outrgb_specular);
	}
	else
		*outrgb = HD_Standard_Compute_Shading (nr, nr->face_rendition->light_rendition, matr, point, color, plane, aligner,
											   params, param_width, param_flags, outrgb_specular);
}


#ifndef DISABLE_DOUBLE_PRECISION
INLINE void COMPUTE_SHADING(
	Net_Rendition const &		nr,
	Material_Rendition const &	matr,
	bool						need_world,
	DPoint const *				point,
	RGB const *					color,
	DPlane const *				plane,
	Vector const *				aligner,
	RGBA *						outrgb,
	RGBA *						outrgb_specular=null,
	Parameter const *			params = null,
	int32_t						param_width = 0,
	Parameter_Flags				param_flags = 0)
{
	if (need_world &&
		!nr->transform_rendition->net_modelling_matrix->Is_Identity()) {
			DPoint	ws = nr->transform_rendition->net_modelling_matrix->Transform(*point);
			*outrgb = HD_Standard_Compute_Shading (nr, nr->face_rendition->light_rendition, matr, &ws, color, plane, aligner,
												   params, param_width, param_flags, outrgb_specular);
	}
	else
		*outrgb = HD_Standard_Compute_Shading (nr, nr->face_rendition->light_rendition, matr, point, color, plane, aligner,
											   params, param_width, param_flags, outrgb_specular);
}
#endif


End_HOOPS_Namespace

#endif
