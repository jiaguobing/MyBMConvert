// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef HIDDEN_DEFINED

#ifndef DATABASE_DEFINED	
#include "database.h"
#endif

Begin_HOOPS_Namespace

#define PIXEL_ERR				0.2f	/* The basic fudge factor */


typedef short Hidden_Flags;
#define HF_NONE							(Hidden_Flags)0x0000
#define HF_SLABBED						(Hidden_Flags)0x0001
#define HF_CHOPPED_START				(Hidden_Flags)0x0002
#define HF_CHOPPED_END					(Hidden_Flags)0x0004
#define HF_CHOPPED_MIDDLE				(Hidden_Flags)0x0008
#define HF_CHOPPED_ANY					(HF_CHOPPED_START|HF_CHOPPED_MIDDLE|HF_CHOPPED_END)
#define HF_CHOPPED_NEAR_START			(Hidden_Flags)0x0020
#define HF_CHOPPED_NEAR_END				(Hidden_Flags)0x0040
#define HF_ZSORT_BACKFACE				(Hidden_Flags)0x0080
#define HF_ZSORT_FRONTFACE				(Hidden_Flags)0x0100

#define SLABBED_SIZE(s_item, s_count) \
	((((s_item) + (1<<HOOPS_MEMORY_ALIGNMENT) - 1) & ~((1<<HOOPS_MEMORY_ALIGNMENT) - 1))*s_count)

#define SLABBED_ADVANCE(s_data, s_item) \
	((s_data)+=SLABBED_SIZE((s_item), 1))

struct Hidden_Slab {
	unsigned char *			data;
	int						size;	
	Reference_Count			refcount;
};

union Extra_Info;

struct Hidden_Payload {
	Hidden_Payload *		next;
	Net_Rendition			net_rendition;		/* for this geometry */
	Extra_Info *			extra_info;		/* if relevant */
	DC_Point *				points;			/* null for unusual items */	
	Hidden_Slab *			slab;

	Plane					plane;			/* plane equation (screen space) */

	SimpleCuboid			cuboid;

	int						count;			/* number of points */
	short					type;			/* of the geometry */
	Reference_Count			refcount;
};


struct Hidden {
	Hidden *			next;
	Hidden *			prev;
	Hidden *			should_follow;
	Hidden_Payload *	payload;
	int32_t				values; /* used for priority cull pixels and should_follow at different times */
	Hidden_Flags		flags;			
};


struct Hidden_Progress {
	Hidden *			hidden_list;
	HSR_Algorithm		hsr_algorithm;
	bool				transparent;
};


/* given an Tristrip, this structure contains the necessary information
   to address an individual triangle within it */
struct Triangle_Within_Tristrip {
	/* index of the substrip that this triangle comes from.  0 <= which_strip < ts->strips */
	unsigned short	which_strip;		
	/* offset from substrip's first vertex index to triangle's first index.  0 <= strip_offset < ts->lengths[which_strip] */
	unsigned short	strip_offset;		
	/* offset from vertex_indices to substrip's first vertex index.  0 <= strip_start < ts->total */
	int		strip_start;		
};



typedef short Hidden_Tristrip_Flags;
#define HTS_NONE					(Hidden_Tristrip_Flags)0x0000
#define HTS_NEED_WORLD_POINTS		(Hidden_Tristrip_Flags)0x0001
#define HTS_INCOMPATIBLE			(Hidden_Tristrip_Flags)0x0002
#define HTS_ENVIRONMENT_MAP			(Hidden_Tristrip_Flags)0x0004

struct Hidden_Tristrip {
	Net_Rendition				net_rendition;		/* for this geometry */
	Net_Rendition				back_nr;	
	Tristrip const *			tristrips;
	float *						zvals;
	Triangle_Within_Tristrip *	triangles;
	RGBAS32 *					lit_colors;	/* allocated to size 2*max_count */
	Parameter *					params;
	int *						facings;	
	Plane*						wvp;
	Plane*						wfp;
	
	float						next_z;			/* Only for hidden tristrip */	
	int32_t						interpolation_case;
	size_t						max_count;		/* face_count or point count, whichever is greater */
	size_t						triangle_count;
	size_t						next_to_draw;
	Hidden_Tristrip_Flags		flags;	
	DC_Point *					dcp;
};

struct Hidden_Polymarker {
	Net_Rendition			net_rendition;		/* for this geometry */
	Net_Rendition			back_nr;	
	Polymarker const *		polymarkers;
	float *					zvals;

	RGBAS32 *				lit_colors;	/* allocated to size 2*max_count */
	Parameter *				params;
	int *					facings;	
	Plane*					wvp;
	Plane*					wfp;

	float					next_z;			/* Only for hidden tristrip */	
	int32_t					interpolation_case;
	int						max_count;		/* face_count or point count, whichever is greater */
	int						point_count;
	int						next_to_draw;
	
	DC_Point *				dcp;
};


struct Hidden_Tristrip_Heap {
	Hidden_Tristrip **		data;	
	int						allocated;
	int						used;
};


#define HIDDEN_CACHE_BATCH 1024

typedef short Hidden_Cache_Flags;
#define HCF_UNIFORM_RGBA_MULTIPLE	 (Hidden_Cache_Flags)0x0001

struct Hidden_Cache {
	Net_Rendition			nr;

	DC_Point *				points;
	RGBAS32 *				rgbas;
	RGBA *					rgba_colors;
	Plane *					planes;
	DPlane *				dplanes;
	Parameter *				params;

	int32_t					param_flags;
	int32_t					param_width;
	Info_Contents			info_contents; 
	int32_t					interpolation_case;
	int32_t					priority;

	int						used;
	int						allocated_param_width;

	Hidden_Cache_Flags		flags;

};




struct Cut_Polygon_Edge_Info {
	bool *					edge_to_here;
};

struct Interpolated_Info {
	RGBAS32 *				rgbas;
	RGBA *					rgba_colors;
	Plane *					planes;
	DPlane *				dplanes;
	Parameter *				params;
	int32_t					param_width;
	int32_t					param_flags;
	Info_Contents			contents; /* last because size may break alignment */
};

struct Marker_Info {
	RGBAS32 				rgbas;
	float					rotation;
	float					size_fixup;
	Info_Contents			contents; /* last because size may break alignment */
};


struct Hidden_Text_Info {
	Text const *			text;	
	KName					kname;
	float					current_Aheight;
	float					current_Aheight_fixup;
};

struct Image_Info {
	void const * const *	rasters;	
	Driver_Color const *	contrast_color;	
	Image const *			db_image;	
	int						hoffset;
	Image_Format			format;
	int						row_bytes;
	int						pattern;
	int						utility;
};

struct Stencil_Info {
	Stencil_Bitmap *		stencil;
	int						utility;
};


struct Inclusion_Info {
	Hidden *				root;
	Hidden *				previous;
	int						count;
};



union Extra_Info {
	Interpolated_Info		interpolated_info;
	Cut_Polygon_Edge_Info	cut_polygon_edge_info;
	Image_Info				image_info;
	Stencil_Info			stencil_info;
	Hidden_Text_Info		text_info;
	Inclusion_Info			inclusion_info;
};



/*
 * this ordering is important! - Find Next To Draw & All In Back use it
 * to decide who should be in front when everything else is equal.
 */
#define HIDDEN_TRIANGLE					(char)1
#define HIDDEN_FACE						(char)2
#define HIDDEN_POLYGON					(char)3
#define HIDDEN_CUT_POLYGON				(char)4
#define HIDDEN_POLYGONAL_TYPE(type)		((char)(type) <= (char)HIDDEN_CUT_POLYGON)
#define HIDDEN_LINE						(char)5
#define HIDDEN_POLYGLINAL_TYPE(type)	((char)(type) <= (char)HIDDEN_LINE)
#define HIDDEN_IMAGE					(char)6
#define HIDDEN_MARKER					(char)7
#define HIDDEN_DC_QUAD					(char)8
#define HIDDEN_TEXT						(char)9
#define HIDDEN_DOT						(char)10
#define HIDDEN_TRISTRIP					(char)11
#define HIDDEN_POLYMARKER				(char)12

/* only for Priority */
#define HIDDEN_INCLUSION				(char)13

#define FORWARD_FACING(hidden)	(!HIDDEN_POLYGLINAL_TYPE((hidden)->payload->type))

struct Line_Equation {
	float				x0, y0, z0, xd, yd, zd;
};

struct Half_Plane {
	DC_Point *			start;		/* start point of edge */
	DC_Point *			end;
	float				x, y, z;	/* intersection point on plane */
	float				t;			/* of intersection point with respect to planes' intersection line equation */
	//Line_Equation		equation;	/* edge equation */
};

struct Half_Plane_Pair {
	Half_Plane			tlow_hplane;   /* half plane with poly towards +"t" */
	Half_Plane			thigh_hplane;  /* half plane with poly towards -"t" */
	bool				nonsimple;	   /* true if more than 2 crossings found */
};

struct Cutting_Info {
	Half_Plane_Pair		poly0;
	Half_Plane_Pair		poly1;
	//Line_Equation		planei;			/* planes' intersection */
};

End_HOOPS_Namespace

#define HIDDEN_DEFINED
#endif
