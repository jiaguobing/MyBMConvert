// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#define FPEPS (1.0e-5)							/* for now: should be option */
#define FPZERO(x) ((x < FPEPS) && (x > -FPEPS))

Begin_HOOPS_Namespace

struct HL_Triangle {
	HL_Triangle	*			next;
	Net_Rendition			net_rendition;

	Geometry const *		geometry_in_progress;	/* for callbacks */
	Segment const *			segment_in_progress;
	Include_Path			path;

	DC_Point *				samples;
	DC_Point *				points;

	Plane *					planes;

	int						count;
};



struct HL_Skip {
	HL_Skip *		next;
	HL_Skip *		prev;

	float			start;
	float			end;
};

struct HL_Line {
	HL_Line *			next;

	DC_Point const *	points;
	RGBAS32 const *		colors;

	HL_Skip *			first_skip;
	HL_Skip *			last_skip;

	float				start;
	float				end;
};


struct HL_Polyline {
	HL_Polyline *			next;
	Net_Rendition			net_rendition;
	Geometry const *		geometry_in_progress;	/* for callbacks */
	Segment const *			segment_in_progress;

	HL_Line *				lines;

	DC_Point *				points;
	RGBAS32 *				colors;

	Include_Path			path;

	int						count;

	bool					visible;
};


struct HL_Line_Metadata {
	float				xmin, xmax, ymin, ymax;

	float				epsilon;
};


typedef int				HL_Marker_Type;
#define HL_Marker_POLYMARKER				(HL_Marker_Type)0x00
#define HL_Marker_POLYDOT					(HL_Marker_Type)0x01
#define HL_Marker_COLORIZED_POLYDOT			(HL_Marker_Type)0x02

struct HL_Marker {
	HL_Marker *				next;
	Net_Rendition			net_rendition;
	Geometry const *		geometry_in_progress;	/* for callbacks */
	Segment const *			segment_in_progress;
	Include_Path			path;

	DC_Point				p0;
	float					rotation;
	float					size_fixup;

	bool					visible;
	HL_Marker_Type			type;
	RGBAS32					color;
};


struct HL_String {
	HL_String *				next;
	Net_Rendition			net_rendition;
	Geometry const *		geometry_in_progress;	/* for callbacks */
	Segment const *			segment_in_progress;
	Hidden_Text_Info *		hidden_text_info;
	Text_Karacter_Info *	kinfo;
	Include_Path			path;

	DC_Point				p0;

	bool					visible;
};


struct HL_Image {
	HL_Image *				next;
	Net_Rendition			net_rendition;
	Geometry const *		geometry_in_progress;	/* for callbacks */
	Segment const *			segment_in_progress;
	Image_Info *			hidden_image_info;
	Include_Path			path;

	DC_Point				p0;
	DC_Point				p1;

	bool					visible;	/* ? */
};


struct HL_Triangle_Bounding {
	HL_Triangle	*	triangle;
	int				offset;
	float			xmin, xmax, ymin, ymax;
};


typedef Banked_Array<HL_Triangle_Bounding, POOL_Allocator<HL_Triangle_Bounding> > HL_Triangle_Bounding_BAC;

struct HL_QuadTree_Node {
	HL_QuadTree_Node *		children;
	HL_QuadTree_Node *		sibling;

	Destructible<HL_Triangle_Bounding_BAC>::unique_ptr	boundings;

	HL_Triangle *			triangles;
	HL_Triangle_Bounding *	current;

	HL_QuadTree_Node *		next;

	unsigned long			count;
	unsigned long			level;
	unsigned long			index;

	float					xmin, xmax, ymin, ymax;
};




struct HL_Data {
	HL_QuadTree_Node *			root;
	HL_QuadTree_Node *			nodes;

	HL_Triangle *				triangles;

	HL_Polyline *				polylines;
	HL_Image *					images;
	HL_Marker *					markers;
	HL_String *					strings;

	HL_Polyline	*				silhouettes;

	Display_Context *			dc;

	int							triangle_count;
	int							triangle_batch_count;
	int							polyline_count;
	int							polyline_batch_count;

	float						tolerance_squared;
	float						face_tolerance;
	float						z_scale;
	float						outer_radius;

	int							longest_polyline;
	int							image_count;

	IntRectangle				extent;
	bool						processed;
	bool						hidden_lines;
	bool						silhouette;
	bool						multiple_depth_ranges;
};


End_HOOPS_Namespace

