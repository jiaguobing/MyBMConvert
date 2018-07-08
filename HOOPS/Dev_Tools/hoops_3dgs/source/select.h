// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef SELECT_DEFINED

Begin_HOOPS_Namespace

struct Window_Tree : CMO_Pooled_Copyable
{
	Window_Tree(Memory_Pool * pool)
		: CMO_Pooled_Copyable(pool), condition_bits(pool), named_styles_vector(pool) {
	}

	~Window_Tree() {
		named_styles_vector.clear();
	}

	Selection_Context *		selection_context;
	Window_Tree *			underlying_sibling;
	Window_Tree *			children;
	Window_Tree *			parent;
	Segment const *			seg;	
	Subsegment const *		subseg;				// for Include Chains

	Rectangle				window;
	Rectangle				visible_window;		// after oversize removed
	Type_Flags				selectability;
	Type_Flags				restricted_selectability;
	Type_Flags				even_invisible;
	Type_Flags				visibility;
	Attribute_Lock			locks;			// active locks
	Attribute_Lock			parent_locks;	// forced locks (child cannot change)
	
	Condition_Bits			condition_bits;

	Named_Style_Vector_Pooled	named_styles_vector;

	bool					treat_as_opaque;
};

typedef int		Selection_Options;

#define Select_BY_AREA			(Selection_Options)0x0001
#define Select_BY_VOLUME		(Selection_Options)0x0002
#define Select_WORLD_SPACE		(Selection_Options)0x0004
#define Select_POLYGON			(Selection_Options)0x0008
#define Select_IGNORE_MODELLING (Selection_Options)0x0010
#define Select_CAMERA_WARNING	(Selection_Options)0x0020
#define Select_VISUAL			(Selection_Options)0x0040
#define Select_VISUAL_RENDERING	(Selection_Options)0x0080
#define Select_BIAS_LINES		(Selection_Options)0x0100
#define Select_BIAS_MARKERS		(Selection_Options)0x0200
#define Select_BIAS_Z_SORT		(Selection_Options)0x0400


struct Selection_Segment_Level {
	Selection_Segment_Level	 *	next;
	Segment const *				segment;
	float						zw;
	float						proximity;
	Type_Flags					typeflags;
	bool						valid;
};



#define Owner_APERTURE	1
#define Owner_VICTIM	2

struct Selection_Context {
	Display_Context *	display_context;
	Selection_Event *	event;

	Point2D				window;

	float				squared_radial_proximity;	// in cm? (squared!)
	Vector2D			pixel_proximity;

	Volume				volume;
	Point				screen_crux;

	int					count;

	Point *				screen_poly;
	Vector2D *			screen_vector;
	float *				screen_scale;
	Point2D				centroid;
	Wedge_Tree *		wedge_tree;
	bool				counter_clockwise;
	int					wing_count;
	Wing **				wing_array;

	Selection_Options			options;
	Include_Chain *				current_include_chain;
	Selection_Segment_Level *	segment_level;
	int							item_list_length;
	int							related_selection_limit;
	Vector2D					current_resolution;
	Vector2D					inverse_resolution;
	bool						selection_aborted;
	bool						brought_to_front;

	bool						hardware_visual_selection;
	bool						in_hw_vis_sel_render;
	HWVisSel_Vector				hw_vis_sel_items;

	void *						local_info;				// pass-through of extra data

	struct {
		Geometry const *		geometry_in_progress;	// for detailed selection

		Selection_Extras *		extras;
		Selection_Extras **		extras_linker;
		int						extras_count;

		float					line_allowance;
		float					edge_allowance;

		int						current_offset;
		float					best_prox, best_zw;
		int						best_offset;
		int						internal_limit;
		float					t;
		bool					hit;
	}							detail;

	struct {
		int						offset;
		int						offset1;
		int						offset2;
		float					prox;
	}							parts;
};



struct Best_Hit {
	DC_Point const *		vertex1;
	DC_Point const *		vertex2;
	float					prox;
	int						face;
	float					zw;
};

struct PH_Selection_Info {
	Selection_Context *		sc;
	Polyhedron const *		polyhedron;

	DC_Point *				dc_points;
	float *					ws;
	Clip_Case *				clip_case;

	Selection_Extras *		extras;
	Selection_Extras **		extras_linker;
	int						extras_count;

	Best_Hit				best_face_hit;
	Best_Hit				best_isoline_hit;
	Best_Hit				best_edge_hit;
	Best_Hit				best_vertex_hit;

	bool					face_hit_made;
	bool					isoline_hit_made;
	bool					edge_hit_made;
	bool					vertex_hit_made;
	bool					multiple_selections;
	bool					skip_details;
};




struct Region_Edge {

	Region_Edge *		next;
	Region_Edge *		prev;

#	define BASE					0
#	define NEXT					1
	Point				pt[2];

	int					owner;
	int					region;
	bool				solid;
};



/*
 * a "wing" is essentially a wide vertex, covering three cases:
 * 1. duplicate points together form a single wing
 *	  in this case, left and right may have different values, but
 *	  the points to which they refer must be equivalent
 * 2. all points connected on a horizontal form a single wing
 *	  here, left points to the leftmost point on the horizontal,
 *	  right points to the rightmost.
 * 3. of course, a lone vertex forms a wing
 *	  in this case, left and right refer to the same point
 */
struct Wing {

	Wing *				next;			/* auto link */

	Region_Edge			edges[2];

#	define LEFT_IS_RISING		0x02
#	define RIGHT_IS_RISING		0x01
#	define VALLEY				0x03	/* both are rising */
#	define PEAK					0x00	/* neither is rising */
	int					type;			/* classification of this wing */
	int					owner;
	int					region;
	float				gap[2];
	bool				solid;
};




struct Wedge_Tree {
	Wedge_Tree *	left;
	Wedge_Tree *	right;
	Vector2D		partition_ray;
	int				partition_index;
};




End_HOOPS_Namespace

#define SELECT_DEFINED
#endif
