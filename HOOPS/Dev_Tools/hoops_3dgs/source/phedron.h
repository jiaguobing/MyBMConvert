// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef PHEDRON_DEFINED

#ifndef DATABASE_DEFINED
#include "database.h"
#endif


Begin_HOOPS_Namespace

struct Polyhedron_No_Data;
struct Mesh_Data;
struct Shell_Data;

template <typename T> struct Polyhedron_Base;


typedef Banked_Array<Highlight, CMO_Allocator<Highlight> >		Entity_Highlights;


typedef unsigned int	Polyhedron_Flags;
#define PHF_CLOSED								(Polyhedron_Flags)0x00000001
#define PHF_CLOSED_VALID						(Polyhedron_Flags)0x00000002
#define PHF_UNIFORM_HANDEDNESS					(Polyhedron_Flags)0x00000004

#define PHF_VERTEX_NORMALS_VALID				(Polyhedron_Flags)0x00000008
#define PHF_FACE_NORMALS_VALID					(Polyhedron_Flags)0x00000010
#define PHF_EDGE_NORMALS_VALID					(Polyhedron_Flags)0x00000020

#define PHF_MANIFOLD							(Polyhedron_Flags)0x00000040
#define PHF_MANIFOLD_VALID						(Polyhedron_Flags)0x00000080

#define PHF_POINTS_BY_REFERENCE					(Polyhedron_Flags)0x00000100
#define PHF_VERTEX_NORMALS_BY_REFERENCE			(Polyhedron_Flags)0x00000200
#define PHF_SHELL_FACE_LIST_BY_REFERENCE		(Polyhedron_Flags)0x00000400

#define PHF_IMPLICIT_NORMALS_REVERSED			(Polyhedron_Flags)0x00000800
#define PHF_FACE_PLANES_VALID					(Polyhedron_Flags)0x00001000

#define PHF_TRISTRIP_RESORT_REQUIRED			(Polyhedron_Flags)0x00002000
#define PHF_NEGATIVE_GAUSSIANS_VALID			(Polyhedron_Flags)0x00004000

#define PHF_ISOLINES_VALID						(Polyhedron_Flags)0x00020000

#define PHF_PLANAR_EDGES_VALID					(Polyhedron_Flags)0x00040000


struct Polyhedron_Counts {
	int							points;
	int							faces;
	int							edges;		/* -1 if not known */

	int							extra_points;

	Polyhedron_Flags			flags;

	Polyhedron_Counts() : points(0), faces(0), edges(-1), extra_points(0), flags(0) {}
};


#define LNA_VERTEX_NORMALS	0x01
#define LNA_FACE_NORMALS	0x02
#define LNA_EDGE_NORMALS	0x04
// small structure to hold normals, which may are usually generated implicitly.
// flags and counts for explicit settings will still be on the appropriate LxA
struct Local_Normal_Attributes {
	Polyhedron_Counts const *	entity_counts;

	Vector *					vertex_normals;
	Plane *						face_planes;
	Vector *					edge_normals;

	bool operator==(Local_Normal_Attributes const &that) const {
		if ((edge_normals==null) != (that.edge_normals==null))
			return false;
		if ((vertex_normals==null) != (that.vertex_normals==null))
			return false;
		if ((face_planes==null) != (that.face_planes==null))
			return false;
		return true;
	}
	bool operator!=(Local_Normal_Attributes const &that) const {return !(*this == that); }

	void	initialize(int which);
	void	clean();
};


/*
 * Polymarkers are assembled a bit differently than polyedges and tristrips,
 * because it's not important to group like-colored markers together.
 * This affects the MASK and the macros below.
 */
typedef unsigned int Local_Vertex_Flags;
#define LVA_EXPLICIT_VISIBLE				(Local_Vertex_Flags)0x00000001
#define LVA_EXPLICIT_INVISIBLE				(Local_Vertex_Flags)0x00000002
#define LVA_ANY_EXPLICIT_VISIBILITY			(LVA_EXPLICIT_VISIBLE | LVA_EXPLICIT_INVISIBLE)

#define LVA_EXPLICIT_SIZE					(Local_Vertex_Flags)0x00000004
#define LVA_EXPLICIT_SYMBOL					(Local_Vertex_Flags)0x00000008




#define LVA_EXPLICIT_MCOLOR_BY_VALUE		(Local_Vertex_Flags)0x00000010
#define LVA_EXPLICIT_MCOLOR_BY_FINDEX		(Local_Vertex_Flags)0x00000020
#define LVA_EXPLICIT_MCOLOR_BY_RGBA			(Local_Vertex_Flags)0x00000040
#define LVA_ANY_EXPLICIT_MCOLOR				(LVA_EXPLICIT_MCOLOR_BY_VALUE | LVA_EXPLICIT_MCOLOR_BY_FINDEX | LVA_EXPLICIT_MCOLOR_BY_RGBA)


#define LVA_POLYMARKER_MASK					(LVA_ANY_EXPLICIT_MCOLOR | LVA_ANY_EXPLICIT_VISIBILITY | \
											 LVA_EXPLICIT_SIZE | LVA_EXPLICIT_SYMBOL)


#define LVA_EXPLICIT_ECOLOR_BY_VALUE		(Local_Vertex_Flags)0x00000100
#define LVA_EXPLICIT_ECOLOR_BY_FINDEX		(Local_Vertex_Flags)0x00000200
#define LVA_ANY_EXPLICIT_ECOLOR				(LVA_EXPLICIT_ECOLOR_BY_VALUE | LVA_EXPLICIT_ECOLOR_BY_FINDEX)

#define LVA_EXPLICIT_FCOLOR_BY_VALUE		(Local_Vertex_Flags)0x00000400
#define LVA_EXPLICIT_FCOLOR_BY_FINDEX		(Local_Vertex_Flags)0x00000800
#define LVA_ANY_EXPLICIT_FCOLOR				(LVA_EXPLICIT_FCOLOR_BY_VALUE | LVA_EXPLICIT_FCOLOR_BY_FINDEX)


#define LVA_ANY_EXPLICIT_COLOR_BY_VALUE		(LVA_EXPLICIT_MCOLOR_BY_VALUE | LVA_EXPLICIT_ECOLOR_BY_VALUE | LVA_EXPLICIT_FCOLOR_BY_VALUE)
#define LVA_ANY_EXPLICIT_COLOR_BY_FINDEX	(LVA_EXPLICIT_MCOLOR_BY_FINDEX | LVA_EXPLICIT_ECOLOR_BY_FINDEX | LVA_EXPLICIT_FCOLOR_BY_FINDEX)
#define LVA_ANY_EXPLICIT_COLOR_BY_RGBA		(LVA_EXPLICIT_MCOLOR_BY_RGBA)
#define LVA_ANY_EXPLICIT_COLOR				(LVA_ANY_EXPLICIT_MCOLOR | LVA_ANY_EXPLICIT_ECOLOR | LVA_ANY_EXPLICIT_FCOLOR)


#define LVA_EXPLICIT_NORMAL					(Local_Vertex_Flags)0x00001000
#define LVA_EXPLICIT_PARAMETER				(Local_Vertex_Flags)0x00002000
#define LVA_IMPLICIT_PARAMETER				(Local_Vertex_Flags)0x00004000

#define LVA_NEGATIVE_GAUSSIAN				(Local_Vertex_Flags)0x00010000
#define LVA_DIRTY							(Local_Vertex_Flags)0x00020000

#define LVA_SPLIT_FOR_FACE_COLORS			(Local_Vertex_Flags)0x00100000
#define LVA_SPLIT_FOR_FACE_NORMALS			(Local_Vertex_Flags)0x00200000
#define LVA_SPLIT_FOR_FACE_PARAMETERS		(Local_Vertex_Flags)0x00400000
#define LVA_SPLIT_FOR_EDGE_COLORS			(Local_Vertex_Flags)0x01000000
#define LVA_SPLIT_FOR_EDGE_NORMALS			(Local_Vertex_Flags)0x02000000
#define LVA_SPLIT_FOR_EDGE_PARAMETERS		(Local_Vertex_Flags)0x04000000

struct One_Vertex_Attributes {
	Highlight				highlight;
	float					size;
	Glyph					glyph;
	unsigned char			glyph_symbol;
	Local_Vertex_Flags		flags;
};

struct LVA_Symbol {
	LVA_Symbol *			next;
	Name					name;
	Reference_Count			refcount;
	unsigned char			id;
};

struct Face_Vertex_Pair {
	int						face;			/* on first entry also used as count */
	int						vertex;			/* on first entry also used as 'unsplit' count */
};


// split into vertex-only and vertex-shared structures?
// normals (vertex, face, and edge) should get separated as they almost always get generated
struct Local_Vertex_Attributes {
	Polyhedron_Counts const *	entity_counts;
	Face_Vertex_Pair **			face_specific_lookup;	/* face-vertex pairings */
//	int *						face_specific_reverse;
	LVA_Symbol *				symbols_list;	// linked list

	Local_Vertex_Flags *		flags;

	RGB *						mcolors;	/* marker colors by value */
	RGB *						ecolors;	/* edge colors by value */
	RGB *						fcolors;	/* face colors by value */
	RGBAS32 *					mrgbas;		/* marker colors by rgba */
	Color_Float_Index *			mfindices; 	/* marker colors by findex */
	Color_Float_Index *			efindices; 	/* edge colors by findex */
	Color_Float_Index *			ffindices; 	/* face colors by findex */
	float *						sizes;
	unsigned char *				symbols;
//	Vector *					normals;

	Parameter *					params;
	Parameter *					natural_params;
	Parameter *					implicit_params;
	int32_t						param_width;
	int32_t						implicit_param_width;
	Parameter_Flags				implicit_param_flags;

	Local_Vertex_Flags			uniform_flag;

	int							explicit_mcolor_by_value_count;
	int							explicit_mcolor_by_findex_count;
	int							explicit_mcolor_by_rgba_count;
	int							explicit_ecolor_by_value_count;
	int							explicit_ecolor_by_findex_count;
	int							explicit_fcolor_by_value_count;
	int							explicit_fcolor_by_findex_count;
	int							explicit_visible_count;
	int							explicit_invisible_count;
	int							explicit_size_count;
	int							explicit_symbol_count;
	int							explicit_parameter_count;
	int							explicit_normal_count;
	int							negative_gaussian_count;

	


	Local_Vertex_Flags &	get_flag (int index) {
		if (flags)
			return flags[index];
		else
			return uniform_flag;
	}

	Local_Vertex_Flags const &	get_flag (int index) const {
		if (flags)
			return flags[index];
		else
			return uniform_flag;
	}

	void ensure_flags () {
		if (flags == null) {
			int		point_count = entity_counts->points + entity_counts->extra_points;
			ZALLOC_ARRAY(flags, point_count, Local_Vertex_Flags);
			for (int i = 0; i < point_count; i++)
				flags[i] = uniform_flag;
		}
	}

	bool operator==(Local_Vertex_Attributes const &that) const {
		if ((explicit_mcolor_by_value_count != that.explicit_mcolor_by_value_count) ||
			(explicit_mcolor_by_findex_count != that.explicit_mcolor_by_findex_count) ||
			(explicit_mcolor_by_rgba_count != that.explicit_mcolor_by_rgba_count) ||
			(explicit_ecolor_by_value_count != that.explicit_ecolor_by_value_count) ||
			(explicit_ecolor_by_findex_count != that.explicit_ecolor_by_findex_count) ||
			(explicit_fcolor_by_value_count != that.explicit_fcolor_by_value_count) ||
			(explicit_fcolor_by_findex_count != that.explicit_fcolor_by_findex_count) ||
			(explicit_visible_count != that.explicit_visible_count) ||
			(explicit_invisible_count != that.explicit_invisible_count) ||
			(explicit_size_count != that.explicit_size_count) ||
			(explicit_symbol_count != that.explicit_symbol_count) ||
			(explicit_parameter_count != that.explicit_parameter_count) ||
			(explicit_normal_count != that.explicit_normal_count) ||
			(negative_gaussian_count != that.negative_gaussian_count))
			return false;

		for (int i = 0; i < entity_counts->points; i++) {
			if (mcolors && mcolors[i] != that.mcolors[i])
				return false;
		}
		return true;
	}
	bool operator!=(Local_Vertex_Attributes const &that) const { return !(*this == that); }

	void	initialize (unsigned int which, int32_t data = 0);
	void	share_colors();		// attempt to share xcolors and/or xfindices arrays
	void	clean();

private:

	friend struct Polyhedron_Base<Shell_Data>;
	friend struct Polyhedron_Base<Mesh_Data>;
	friend struct Polyhedron_Base<Polyhedron_No_Data>;

	bool check_uniform () {
		int		point_count = entity_counts->points + entity_counts->extra_points;
		bool	uniform = true;

		if (explicit_visible_count != 0 && explicit_visible_count != point_count)
			uniform = false;
		if (explicit_invisible_count != 0 && explicit_invisible_count != point_count)
			uniform = false;
		if (explicit_mcolor_by_value_count != 0 && explicit_mcolor_by_value_count != point_count)
			uniform = false;
		if (explicit_mcolor_by_findex_count != 0 && explicit_mcolor_by_findex_count != point_count)
			uniform = false;
		if (explicit_mcolor_by_rgba_count != 0 && explicit_mcolor_by_rgba_count != point_count)
			uniform = false;
		if (explicit_ecolor_by_value_count != 0 && explicit_ecolor_by_value_count != point_count)
			uniform = false;
		if (explicit_ecolor_by_findex_count != 0 && explicit_ecolor_by_findex_count != point_count)
			uniform = false;
		if (explicit_fcolor_by_value_count != 0 && explicit_fcolor_by_value_count != point_count)
			uniform = false;
		if (explicit_fcolor_by_findex_count != 0 && explicit_fcolor_by_findex_count != point_count)
			uniform = false;
		if (explicit_size_count != 0 && explicit_size_count != point_count)
			uniform = false;
		if (explicit_symbol_count != 0 && explicit_symbol_count != point_count)
			uniform = false;
		if (explicit_parameter_count != 0 && explicit_parameter_count != point_count)
			uniform = false;
		if (explicit_normal_count != 0 && explicit_normal_count != point_count)
			uniform = false;
		if (negative_gaussian_count != 0 && negative_gaussian_count != point_count)
			uniform = false;

		if (uniform && flags != null) {
			uniform_flag = flags[0];
			FREE_ARRAY(flags, point_count, Local_Vertex_Flags);
			flags = null;
		}

		return uniform;
	}
};

INLINE void INIT_ONE_VERTEX_ATTRIBUTES(One_Vertex_Attributes & ova) 
{
	ZERO_STRUCT(&ova, One_Vertex_Attributes);
}

INLINE void PACK_ONE_VERTEX_ATTRIBUTES(Local_Vertex_Attributes const * lva, int index, One_Vertex_Attributes & ova) 
{
	ova.flags = lva->get_flag(index) & LVA_POLYMARKER_MASK;
	if (lva->sizes != null)
		ova.size = lva->sizes[index];
	if (lva->symbols != null)
		ova.glyph_symbol = lva->symbols[index];
}

INLINE bool EQUAL_LOCAL_AND_ONE_VERTEX_ATTRIBUTES(Local_Vertex_Attributes const * lva, int index,
												  One_Vertex_Attributes const & ova, Entity_Highlights const * detailed)
{
	if (lva != null) {
		if ((lva->get_flag(index) & LVA_POLYMARKER_MASK) != (ova.flags & LVA_POLYMARKER_MASK) ||
			(lva->sizes != null && lva->sizes[index] != ova.size) ||
			(lva->symbols != null && lva->symbols[index] != ova.glyph_symbol))
			return false;
	}

	if (detailed != null) {
		Highlight				test;
		Highlight const *		potential = detailed->entry_valid (index);
		if (potential != null)
			test = *potential;
		if (test != ova.highlight)
			return false;
	}
	return true;
}

INLINE bool EQUAL_ONE_VERTEX_ATTRIBUTES(One_Vertex_Attributes const & va1, One_Vertex_Attributes const & va2)
{
	return ((va1.flags & LVA_POLYMARKER_MASK) == (va2.flags & LVA_POLYMARKER_MASK) &&
			(!ANYBIT (va1.flags, LVA_EXPLICIT_SIZE) || va1.size == va2.size) &&
			(!ANYBIT (va1.flags, LVA_EXPLICIT_SYMBOL) || va1.glyph_symbol == va2.glyph_symbol) &&
			va1.highlight == va2.highlight);
}

INLINE bool LESS_ONE_VERTEX_ATTRIBUTES(One_Vertex_Attributes const & va1, One_Vertex_Attributes const & va2)
{
	Local_Vertex_Flags f1 = va1.flags & LVA_POLYMARKER_MASK;
	Local_Vertex_Flags f2 = va2.flags & LVA_POLYMARKER_MASK;

	if (f1 < f2)
		return true;
	else if (f1 > f2)
		return false;

	if (va1.size < va2.size)
		return true;
	else if (va1.size > va2.size)
		return false;

	if (va1.glyph_symbol < va2.glyph_symbol)
		return true;
	//else if (va1.glyph_symbol > va2.glyph_symbol)

	if (va1.highlight.style == null) {
		if (va2.highlight.style != null)
			return true;
	}
	else if (va2.highlight.style == null) {
		return false;
	}
	else {
		if (COMPARE_STRINGS (va1.highlight.style->name.text(), va2.highlight.style->name.text()) < 0)
			return true;
	}

	return false;
}

////////////////////////////////////////////////////////////////////////////////

typedef unsigned short Local_Edge_Flags;
#define LEA_HAS_VCOLOR_BY_VALUE			(Local_Edge_Flags)0x0001
#define LEA_HAS_VCOLOR_BY_FINDEX		(Local_Edge_Flags)0x0002
#define LEA_HAS_VCOLOR_BY_DEFAULT		(Local_Edge_Flags)0x0004
#define LEA_HAS_VCOLOR_EXPLICIT			(LEA_HAS_VCOLOR_BY_VALUE | LEA_HAS_VCOLOR_BY_FINDEX)
#define LEA_HAS_VCOLOR_WHICHEVER		(LEA_HAS_VCOLOR_BY_VALUE | LEA_HAS_VCOLOR_BY_FINDEX | LEA_HAS_VCOLOR_BY_DEFAULT)

#define LEA_EXPLICIT_COLOR_BY_VALUE		(Local_Edge_Flags)0x0010
#define LEA_EXPLICIT_COLOR_BY_FINDEX	(Local_Edge_Flags)0x0020
#define LEA_ANY_EXPLICIT_COLOR			(LEA_EXPLICIT_COLOR_BY_VALUE | LEA_EXPLICIT_COLOR_BY_FINDEX)

#define LEA_EXPLICIT_VISIBLE			(Local_Edge_Flags)0x0040
#define LEA_EXPLICIT_INVISIBLE			(Local_Edge_Flags)0x0080
#define LEA_ANY_EXPLICIT_VISIBILITY		(LEA_EXPLICIT_VISIBLE | LEA_EXPLICIT_INVISIBLE)

#define LEA_PERIMETER					(Local_Edge_Flags)0x0100
#define LEA_MESH_QUAD					(Local_Edge_Flags)0x0200
#define LEA_SHELL_PLANAR_FACES			(Local_Edge_Flags)0x0200 //overlaid on top of mesh quad
#define LEA_HARD						(Local_Edge_Flags)0x0400
#define LEA_ADJACENT					(Local_Edge_Flags)0x0800

#define LEA_EXPLICIT_PATTERN			(Local_Edge_Flags)0x1000
#define LEA_EXPLICIT_WEIGHT				(Local_Edge_Flags)0x2000
#define LEA_EXPLICIT_NORMAL				(Local_Edge_Flags)0x4000
#define LEA_EXPLICIT_ISOLINE			(Local_Edge_Flags)0x8000

// special flags (not mixed with those above) to tell polyedge code about face visibility
#define	LEA_FACES_OFF					(Local_Edge_Flags)0x0001
#define LEA_FACES_LOCKED				(Local_Edge_Flags)0x0002
															 

struct One_Edge_Attributes {
	Highlight				highlight;
	Line_Style				line_style;
	RGB						color;			/* edge color, that is */
	Color_Float_Index		findex;			/* edge color, that is */
	float					weight;
	int						isoline_index;
	Local_Edge_Flags		flags;
	unsigned char			pattern;
};

struct LEA_Pattern {
	LEA_Pattern *		next;
	Name				name;
	Reference_Count		refcount;
	unsigned char		id;
};

struct Local_Edge_Attributes {
	Polyhedron_Counts const *	entity_counts;
	LEA_Pattern *				patterns_list;

	Local_Edge_Flags *			flags;

	RGB *						colors;
	Color_Float_Index *			findices;
	unsigned char *				patterns; 
	float *						weights;
//	Vector *					normals;

	int							explicit_color_by_value_count;
	int							explicit_color_by_findex_count;
	int							explicit_visible_count;
	int							explicit_invisible_count;
	int							explicit_pattern_count;
	int							explicit_weight_count;
	int							explicit_normal_count;

	Local_Edge_Flags &	get_flag (int index) {
		return flags[index];
	}

	Local_Edge_Flags const &	get_flag (int index) const {
		return flags[index];
	}

	bool operator==(Local_Edge_Attributes const &that) const {
		if ((explicit_color_by_value_count != that.explicit_color_by_value_count) ||
			(explicit_color_by_findex_count != that.explicit_color_by_findex_count) ||
			(explicit_visible_count != that.explicit_visible_count) ||
			(explicit_invisible_count != that.explicit_invisible_count) ||
			(explicit_pattern_count != that.explicit_pattern_count) ||
			(explicit_weight_count != that.explicit_weight_count) ||
			(explicit_normal_count != that.explicit_normal_count))
			return false;

		for (int i = 0; i < entity_counts->edges; i++) {
			if (get_flag(i) != that.get_flag(i))
				return false;

			if (colors && colors[i] != that.colors[i])
				return false;

			if (findices && findices[i] != that.findices[i])
				return false;

			if (weights && weights[i] != that.weights[i])
				return false;
		}

		return true;
	}
	bool operator!=(Local_Edge_Attributes const &that) const { return !(*this == that); }


	void	initialize (Local_Edge_Flags which);
	void	clean();
};


INLINE void INIT_ONE_EDGE_ATTRIBUTES(One_Edge_Attributes & oea)
{
	ZERO_STRUCT(&oea, One_Edge_Attributes);
}

INLINE void PACK_ONE_EDGE_ATTRIBUTES(Local_Edge_Attributes const * lea, int index, One_Edge_Attributes & oea)
{
	if (lea != null && lea->flags != null) {
		oea.flags = lea->flags[index] & ~LEA_EXPLICIT_NORMAL;
		if (lea->colors != null)
			oea.color = lea->colors[index];
		if (lea->findices != null)
			oea.findex = lea->findices[index];
		if (lea->weights != null)
			oea.weight = lea->weights[index];
		if (lea->patterns != null)
			oea.pattern = lea->patterns[index];
	}
}

INLINE bool EQUAL_LOCAL_AND_ONE_EDGE_ATTRIBUTES(Local_Edge_Attributes const * lea, int index,
												One_Edge_Attributes const & oea, Entity_Highlights const * detailed)
{
#ifndef DISABLE_GEOMETRY_EDGE
	if (lea != null) {
		if ((lea->flags[index] & ~LEA_EXPLICIT_NORMAL) != (oea.flags & ~LEA_EXPLICIT_NORMAL) ||
			(lea->colors != null && lea->colors[index] != oea.color) ||
			(lea->weights != null && lea->weights[index] != oea.weight) ||
			(lea->findices != null && lea->findices[index] != oea.findex) ||
			(lea->patterns != null && lea->patterns[index] != oea.pattern))
			return false;
	}

	if (detailed != null) {
		Highlight				test;
		Highlight const *		potential = detailed->entry_valid (index);
		if (potential != null)
			test = *potential;
		if (test != oea.highlight)
			return false;
	}
#endif
	return true;
}

INLINE bool EQUAL_ONE_EDGE_ATTRIBUTES(One_Edge_Attributes const & ea1, One_Edge_Attributes const & ea2, int mask)
{
	return ((ea1.flags & mask) == (ea2.flags & mask) &&
			(!ANYBIT (ea1.flags&mask, LEA_EXPLICIT_PATTERN) || ea1.pattern == ea2.pattern) &&
			(!ANYBIT (ea1.flags&mask, LEA_EXPLICIT_WEIGHT) || ea1.weight == ea2.weight) &&
			(!ANYBIT (ea1.flags&mask, LEA_EXPLICIT_COLOR_BY_FINDEX) || ea1.findex == ea2.findex) &&
			(!ANYBIT (ea1.flags&mask, LEA_EXPLICIT_COLOR_BY_VALUE) || ea1.color == ea2.color) &&
			ea1.highlight == ea2.highlight);
}

INLINE bool LESS_ONE_EDGE_ATTRIBUTES(One_Edge_Attributes const & ea1, One_Edge_Attributes const & ea2)
{
	if (ea1.flags < ea2.flags)
		return true;
	else if (ea1.flags > ea2.flags)
		return false;

	if (ea1.weight < ea2.weight)
		return true;
	else if (ea1.weight > ea2.weight)
		return false;

	if (ea1.pattern < ea2.pattern)
		return true;
	else if (ea1.pattern > ea2.pattern)
		return false;

	if (ea1.findex < ea2.findex)
		return true;
	else if (ea1.findex > ea2.findex)
		return false;

	if (ea1.color.red < ea2.color.red)
		return true;
	else if (ea1.color.red > ea2.color.red)
		return false;

	if (ea1.color.green < ea2.color.green)
		return true;
	else if (ea1.color.green > ea2.color.green)
		return false;

	if (ea1.color.blue < ea2.color.blue)
		return true;
	else if (ea1.color.blue > ea2.color.blue)
		return false;

	if (ea1.highlight.style == null) {
		if (ea2.highlight.style != null)
			return true;
	}
	else if (ea2.highlight.style == null) {
		return false;
	}
	else {
		if (COMPARE_STRINGS (ea1.highlight.style->name.text(), ea2.highlight.style->name.text()) < 0)
			return true;
	}

	return false;
}

////////////////////////////////////////////////////////////////////////////////

typedef unsigned char Local_Tristrip_Face_Flags;
#define LFA_HAS_VCOLOR_BY_VALUE			(Local_Tristrip_Face_Flags)0x01
#define LFA_HAS_VCOLOR_BY_FINDEX		(Local_Tristrip_Face_Flags)0x02
#define LFA_HAS_VCOLOR_BY_DEFAULT		(Local_Tristrip_Face_Flags)0x04
#define LFA_HAS_VCOLOR_EXPLICIT			(LFA_HAS_VCOLOR_BY_VALUE | LFA_HAS_VCOLOR_BY_FINDEX)
#define LFA_HAS_VCOLOR_WHICHEVER		(LFA_HAS_VCOLOR_BY_VALUE | LFA_HAS_VCOLOR_BY_FINDEX | LFA_HAS_VCOLOR_BY_DEFAULT)


typedef unsigned short Local_Face_Flags;
#define	LFA_RESERVED_FOR_TRISTRIP_MASK		(Local_Face_Flags)0x000F

#define LFA_EXPLICIT_FRONT_COLOR_BY_VALUE	(Local_Face_Flags)0x0010
#define LFA_EXPLICIT_BACK_COLOR_BY_VALUE	(Local_Face_Flags)0x0020
#define LFA_EXPLICIT_FRONT_COLOR_BY_FINDEX	(Local_Face_Flags)0x0040
#define LFA_EXPLICIT_BACK_COLOR_BY_FINDEX	(Local_Face_Flags)0x0080
#define LFA_EXPLICIT_COLOR_BY_VALUE			(LFA_EXPLICIT_FRONT_COLOR_BY_VALUE | LFA_EXPLICIT_BACK_COLOR_BY_VALUE)
#define LFA_EXPLICIT_COLOR_BY_FINDEX		(LFA_EXPLICIT_FRONT_COLOR_BY_FINDEX | LFA_EXPLICIT_BACK_COLOR_BY_FINDEX)
#define LFA_ANY_EXPLICIT_COLOR				(LFA_EXPLICIT_COLOR_BY_VALUE | LFA_EXPLICIT_COLOR_BY_FINDEX)

#define LFA_EXPLICIT_VISIBLE				(Local_Face_Flags)0x0100
#define LFA_EXPLICIT_INVISIBLE				(Local_Face_Flags)0x0200
#define LFA_ANY_EXPLICIT_VISIBILITY			(LFA_EXPLICIT_VISIBLE | LFA_EXPLICIT_INVISIBLE)

#define LFA_EXPLICIT_PATTERN				(Local_Face_Flags)0x0400
#define LFA_EXPLICIT_NORMAL					(Local_Face_Flags)0x0800

#define LFA_ALL_EXPLICIT					(LFA_ANY_EXPLICIT_COLOR | LFA_ANY_EXPLICIT_VISIBILITY | \
											 LFA_EXPLICIT_PATTERN | LFA_EXPLICIT_NORMAL)

#define LFA_DEGENERATE_FACES				(Local_Face_Flags)0x1000
#define LFA_RESORT_REQUIRED					(Local_Face_Flags)0x8000

typedef unsigned short Composite_Face_Flags;
#define	LFA_SUBSTRIP_MASK				(Composite_Face_Flags)(LFA_ALL_EXPLICIT | LFA_HAS_VCOLOR_EXPLICIT)
#define LFA_TRISTRIP_MASK				(Composite_Face_Flags)(LFA_ALL_EXPLICIT | LFA_HAS_VCOLOR_WHICHEVER)


struct One_Face_Attributes {
	Highlight				highlight;
	RGB						color;			/* face color, that is */
	Color_Float_Index		findex;			/* face color, that is */
	int						region;
	Composite_Face_Flags	flags;
	unsigned char			pattern;		/* as in patterns.h */
};


struct Local_Face_Attributes {
	Polyhedron_Counts const *	entity_counts;

	Local_Face_Flags *			flags;
	Local_Tristrip_Face_Flags *	vcoloring;
	
	RGB *						colors;
	Color_Float_Index *			findices;
	unsigned char *				patterns;	// as in patterns.h
//	Plane *						normals;
	int *						regions;

	int							explicit_visible_count;
	int							explicit_invisible_count;
	int							explicit_color_by_value_count;
	int							explicit_color_by_findex_count;
	int							explicit_pattern_count;
	int							explicit_normal_count;
	int							vcoloring_count;
//	int							degenerate_normal_count;

	Local_Face_Flags			uniform_flag;
	Local_Tristrip_Face_Flags 	uniform_vcoloring;

	Local_Face_Flags &	get_flag (int index) {
		if (flags != null)
			return flags[index];
		else
			return uniform_flag;
	}
	Local_Face_Flags const &	get_flag (int index) const {
		if (flags != null)
			return flags[index];
		else
			return uniform_flag;
	}

	void ensure_flags () {
		if (flags == null) {
			int		face_count = entity_counts->faces;
			ZALLOC_ARRAY(flags, face_count, Local_Face_Flags);
			for (int i = 0; i < face_count; i++)
				flags[i] = uniform_flag;
		}
	}

	Local_Tristrip_Face_Flags &	get_vcoloring (int index) {
		if (vcoloring != null)
			return vcoloring[index];
		else
			return uniform_vcoloring;
	}
	Local_Tristrip_Face_Flags const &	get_vcoloring (int index) const {
		if (vcoloring != null)
			return vcoloring[index];
		else
			return uniform_vcoloring;
	}

	bool operator==(Local_Face_Attributes const &that) const {
		if ((explicit_color_by_value_count != that.explicit_color_by_value_count) ||
			(explicit_color_by_findex_count != that.explicit_color_by_findex_count) ||
			(explicit_pattern_count != that.explicit_pattern_count) ||
			(explicit_normal_count != that.explicit_normal_count) ||
			(vcoloring_count != that.vcoloring_count))
			return false;

		for (int i = 0; i < entity_counts->faces; i++) {
			if (get_flag(i) != that.get_flag(i))
				return false;

			if (colors && colors[i] != that.colors[i])
				return false;

			if (findices && findices[i] != that.findices[i])
				return false;
		}

		return true;
	}
	bool operator!=(Local_Face_Attributes const &that) const { return !(*this == that); }

	void	initialize (Local_Face_Flags which);
	void	clean();


private:

	friend struct Polyhedron_Base<Shell_Data>;
	friend struct Polyhedron_Base<Mesh_Data>;
	friend struct Polyhedron_Base<Polyhedron_No_Data>;

	bool check_uniform () {
		int		face_count = entity_counts->faces;
		bool	uniform = true;

		if (explicit_color_by_value_count != 0 && explicit_color_by_value_count != face_count) {
			uniform = false;
		}
		else if (explicit_color_by_value_count == 0 && colors != null) {
			FREE_ARRAY (colors, face_count, RGB);
			colors = null;
		}

		if (explicit_color_by_findex_count != 0 && explicit_color_by_findex_count != face_count) {
			uniform = false;
		}
		else if (explicit_color_by_findex_count == 0 && findices != null) {
			FREE_ARRAY (findices, face_count, Color_Float_Index);
			findices = null;
		}

		if (explicit_pattern_count != 0 && explicit_pattern_count != face_count) {
			uniform = false;
		}
		else if (explicit_pattern_count == 0 && patterns != null) {
			FREE_ARRAY (patterns, face_count, unsigned char);
			patterns = null;
		}

		if (explicit_normal_count != 0 && explicit_normal_count != face_count) {
			uniform = false;
		}

		if (flags == null) {
			ASSERT(uniform == true);
			return true;
		}

		if (explicit_visible_count != 0 && explicit_visible_count != face_count) {									
			uniform = false;
		}

		if (explicit_invisible_count != 0 && explicit_invisible_count != face_count) {
			uniform = false;
		}

		if (uniform && flags != null) {
			uniform_flag = flags[0];
			FREE_ARRAY(flags, face_count, Local_Face_Flags);
			flags = null;
		}

		return uniform;
	}

};

INLINE void INIT_ONE_FACE_ATTRIBUTES(One_Face_Attributes & fa)
{
	ZERO_STRUCT(&fa, One_Face_Attributes);
}

INLINE void PACK_ONE_FACE_ATTRIBUTES(Local_Face_Attributes const * lfa, int index, One_Face_Attributes & fa)
{
	if (lfa != null) {
		fa.flags = lfa->get_flag(index) & LFA_TRISTRIP_MASK;
		fa.flags |= lfa->get_vcoloring(index);
		if (lfa->colors != null)
			fa.color = lfa->colors[index];
		if (lfa->findices != null)
			fa.findex = lfa->findices[index];
		if (lfa->patterns != null)
			fa.pattern = lfa->patterns[index];
		if (lfa->regions != null)
			fa.region = lfa->regions[index];
	}
	else
		fa.flags = LFA_HAS_VCOLOR_BY_DEFAULT;
}

INLINE bool EQUAL_LOCAL_AND_ONE_FACE_ATTRIBUTES(Local_Face_Attributes const * lfa, int index,
												One_Face_Attributes const & fa, Entity_Highlights const * detailed)
{
	if (lfa != null) {
		if (fa.flags != ((lfa->get_flag(index) & LFA_TRISTRIP_MASK) | lfa->get_vcoloring(index)) ||
			(lfa->colors != null && lfa->colors[index] != fa.color) ||
			(lfa->findices != null && lfa->findices[index] != fa.findex) ||
			(lfa->patterns != null && lfa->patterns[index] != fa.pattern) ||
			(lfa->regions != null && lfa->regions[index] != fa.region) ||
			(fa.flags & LFA_SUBSTRIP_MASK) || ANYBIT (HOOPS::WORLD->system_flags, Sysop_TRISTRIP_PER_REGION))
			return false;
	}

	if (detailed != null) {
		Highlight				test;
		Highlight const *		potential = detailed->entry_valid (index);
		if (potential != null)
			test = *potential;
		if (test != fa.highlight)
			return false;
	}
	return true;
}

INLINE bool EQUAL_ONE_FACE_ATTRIBUTES(One_Face_Attributes const & fa1, One_Face_Attributes const &  fa2, int mask)
{
	return ((fa1.flags & mask) == (fa2.flags & mask) &&
		 (fa1.region == fa2.region ||
		  (((fa1.flags & LFA_SUBSTRIP_MASK) == 0) &&
		  !ANYBIT (HOOPS::WORLD->system_flags, Sysop_TRISTRIP_PER_REGION))) &&
		 (!ANYBIT (fa1.flags&mask, LFA_EXPLICIT_PATTERN) || fa1.pattern == fa2.pattern) &&
		 (!ANYBIT (fa1.flags&mask, LFA_EXPLICIT_COLOR_BY_FINDEX) || fa1.findex == fa2.findex) &&
		 (!ANYBIT (fa1.flags&mask, LFA_EXPLICIT_COLOR_BY_VALUE) || fa1.color == fa2.color) &&
		 fa1.highlight == fa2.highlight);
}

INLINE bool LESS_ONE_FACE_ATTRIBUTES(One_Face_Attributes const & fa1, One_Face_Attributes const &  fa2)
{
	if (fa1.flags < fa2.flags)
		return true;
	else if (fa1.flags > fa2.flags)
		return false;

	if (fa1.region < fa2.region)
		return true;
	else if (fa1.region > fa2.region)
		return false;

	if (fa1.pattern < fa2.pattern)
		return true;
	else if (fa1.pattern > fa2.pattern)
		return false;

	if (fa1.findex < fa2.findex)
		return true;
	else if (fa1.findex > fa2.findex)
		return false;

	if (fa1.color.red < fa2.color.red)
		return true;
	else if (fa1.color.red > fa2.color.red)
		return false;

	if (fa1.color.green < fa2.color.green)
		return true;
	else if (fa1.color.green > fa2.color.green)
		return false;

	if (fa1.color.blue < fa2.color.blue)
		return true;
	else if (fa1.color.blue > fa2.color.blue)
		return false;

	if (fa1.highlight.style == null) {
		if (fa2.highlight.style != null)
			return true;
	}
	else if (fa2.highlight.style == null) {
		return false;
	}
	else {
		if (COMPARE_STRINGS (fa1.highlight.style->name.text(), fa2.highlight.style->name.text()) < 0)
			return true;
	}

	return false;
}

////////////////////////////////////////////////////////////////////////////////


template <typename FV, typename FT, typename CT>
bool Set_and_Adjust (FT setting, CT & counter,
					 FV & flag,
					 CT increment = 1) {
	bool	changed = false;
	if (!ANYBIT (flag, setting)) {
		flag |= setting;
		counter += increment;
		changed =  true;
	}
	return changed;
}

template <typename FV, typename FT, typename CT>
bool Set_and_Adjust (FT setting, CT & counter,
					 FT opposing1, CT & opcounter1,
					 FV & flag,
					 CT increment = 1) {
	bool	changed = Set_and_Adjust (setting, counter, flag, increment);
	if (ANYBIT (flag, opposing1)) {
		flag &= ~opposing1;
		opcounter1 -= increment;
		changed =  true;
	}
	return changed;
}

template <typename FV, typename FT, typename CT>
bool Set_and_Adjust (FT setting, CT & counter,
					 FT opposing1, CT & opcounter1,
					 FT opposing2, CT & opcounter2,
					 FV & flag,
					 CT increment = 1) {
	bool	changed = Set_and_Adjust (setting, counter, opposing1, opcounter1, flag, increment);
	if (ANYBIT (flag, opposing2)) {
		flag &= ~opposing2;
		opcounter2 -= increment;
		changed =  true;
	}
	return changed;
}




////////////////////////////////////////////////////////////////////////////////


#define			Part_NONE				0x00
#define			Part_POLYMARKER			0x01
#define			Part_POLYEDGE			0x02
#define			Part_TRISTRIP			0x04

#define			Same_NONE				Part_NONE
#define			Same_VERTEX_COLORS		Part_POLYMARKER
#define			Same_EDGE_COLORS		Part_POLYEDGE
#define			Same_FACE_COLORS		Part_TRISTRIP


#define PF_OWNS_PARAMETER_ARRAY	(Parameter_Flags)0x80000000

struct Texture_Parameters {
	Texture_Parameters *	next;

	Parameter *				vertex_params;
	int						vertex_param_count;
	int						vertex_param_width;
	Parameter_Flags			vertex_param_flags;

	Reference_Count			refcount;

	bool					texturing_requested;
#ifdef TEST_RIG_1_SHADERS
	bool					eftc;
#endif

	local void free_one(Texture_Parameters * tp) {

		if (tp->vertex_params != null && ANYBIT (tp->vertex_param_flags, PF_OWNS_PARAMETER_ARRAY))
			FREE_ARRAY (tp->vertex_params, tp->vertex_param_count * tp->vertex_param_width, Parameter);

		FREE (tp, Texture_Parameters);
	}

	local INLINE void free_list (Texture_Parameters * tp) {

		Texture_Parameters *	next;

		do {
			next = tp->next;

			Texture_Parameters::free_one(tp);

			tp = null;

			if (next != null && DECR_UTILITY (next) == 0)
				tp = next;
		} while (tp != null);
	}

	local INLINE size_t sum_memory_used(Texture_Parameters * curr, MU_Flags flags) {
		size_t		size = 0;
		Texture_Parameters *	next;
		if (curr) do {
			next = curr->next;
			size += curr->memory_used(flags);
		} while ((curr = next) != null);
		return size;
	}

	size_t memory_used(MU_Flags flags) const {

		size_t size = 0;

		if (ANYBIT(flags, MU_DERIVED)) {

			size += sizeof(Texture_Parameters);

			if (vertex_params != null && ANYBIT (vertex_param_flags, PF_OWNS_PARAMETER_ARRAY))
				size += (vertex_param_count * vertex_param_width) * sizeof(Parameter);
		}

		return size;
	}

};



enum Strip_Data_Ownership : uint32_t {
	SDO_NONE					= 0x00000000,
	SDO_INDICES					= 0x00000001,	// lengths & index array(s) -- the usual case
	SDO_POINTS					= 0x00000002,
	SDO_VERTEX_COLORS			= 0x00000004,
	SDO_VERTEX_RGBAS			= 0x00000008,
	SDO_VERTEX_FINDICES			= 0x00000010,
	SDO_VERTEX_NORMALS			= 0x00000020,
	SDO_VERTEX_PARAMETERS		= 0x00000040,
	SDO_VERTEX_PRIORITIES		= 0x00000080,
	SDO_SEGMENT_COLORS			= 0x00000100,
	SDO_SEGMENT_NORMALS			= 0x00000200,
	SDO_SHORT_INDICES			= 0x00000400,
	SDO_EVERYTHING				= 0x000003FF,
	
	SDO_FACE_COLORS				= SDO_SEGMENT_COLORS,
	SDO_EDGE_COLORS				= SDO_SEGMENT_COLORS,
	SDO_FACE_NORMALS			= SDO_SEGMENT_NORMALS,		// planes
	SDO_EDGE_NORMALS			= SDO_SEGMENT_NORMALS,
};

template<> struct _bit_ops_type < Strip_Data_Ownership > {
	typedef Strip_Data_Ownership type;
};


template <typename Base_Type>
struct Strip_Header : public CMO {
public:

	Strip_Header<Base_Type>()
		: next(null), parent_geom(null), display_lists(null), bounding(null), refcount(0), ownership(SDO_NONE), activity(NOTHING), old_activity(0),
		rendo_flags(0), special_flags(DL_NONE), special_flags2(DL2_NONE)
			
	{
		;
	}

	Strip_Header<Base_Type>(Strip_Header<Base_Type> const & that)
		: next(null), parent_geom(null), display_lists(null), bounding(null), refcount(0), ownership(that.ownership),
		activity(that.activity), old_activity(that.dbflags),
		rendo_flags(that.rendo_flags), special_flags(that.special_flags), special_flags2(that.special_flags2)
	{
		ASSERT(0); // figure out if this works
	}

	Base_Type *				next;
	Geometry const *		parent_geom;
	Display_List *			display_lists;
	Bounding				bounding;

	Reference_Count			refcount;

	Strip_Data_Ownership	ownership;
	Activity				activity;
	Activity				old_activity;
	Rendo_Flags				rendo_flags;
	Strip_Flags				special_flags;
	Strip_Flags2			special_flags2;
};


/* Tristrip_Polyedge_Polymarker_Common */
struct Strip_Common : Strip_Header<Strip_Common> {
};


/* Note: face_indices is padded with 2 dummy values at the start of every strip 
 * so that vertex_indices and face_indices stay in synch. */
struct Tristrip : Strip_Header<Tristrip> {

	int						strips;
	int						total;
	int *					lengths;
	int *					vertex_indices;
	unsigned short *		short_vertex_indices;
	int						short_vertex_indices_allocated;
	int						short_vertex_indices_used;
	int *					face_indices;

	int						point_count;
	int						face_count;

	Point *					points;
	DPoint *				dpoints;
	RGB *					vertex_colors;
	float *					vertex_findices;
	Vector *				vertex_normals;
	Texture_Parameters *	texture_params;
	float *					vertex_priorities;

	RGB *					face_colors;
	Plane *					face_normals;

	/*Plane *	world_face_planes;*/
	/*int *	screen_plane_facings;*/
	Tristrip const *		index_source;

#	ifdef CADDS5
	int *					list;
#	endif

	int						lengths_allocated;
	int						vi_allocated;
	int						fi_allocated;

	One_Face_Attributes		face_attributes;

	int						same_colors;

	bool					color_interpolated;
	bool					single_colored;

	local Tristrip *	copy(Tristrip const * in, Geometry const * new_owner) {

		Tristrip *	head = null;
		Tristrip **	linker = &head;

		if (in != null) do {

			Tristrip *	out;
			ZALLOC (out, Tristrip);
			*linker = out;
			linker = &out->next;

			INIT_UTILITY (out);
			out->parent_geom = new_owner;
			out->bounding = in->bounding;

			out->face_attributes = in->face_attributes;

			CLONE_ARRAY(in->lengths, in->strips, int, out->lengths);
			out->lengths_allocated = in->strips;
			CLONE_ARRAY(in->vertex_indices, in->total, int, out->vertex_indices);
			CLONE_ARRAY(in->face_indices, in->total, int, out->face_indices);
			out->vi_allocated = out->fi_allocated = in->total;

			out->ownership |= SDO_INDICES;

			out->total = in->total;
			out->strips = in->strips;
		} while ((in = in->next) != null);

		return head;
	}

	local void free_one (Tristrip * ts)
	{

		if (ANYBIT (ts->ownership, SDO_INDICES)) {
			if (ts->lengths != null) {
				if (!ts->lengths_allocated)
					ts->lengths_allocated = ts->strips;
				FREE_ARRAY (ts->lengths, ts->lengths_allocated, int);
			}
			if (ts->vertex_indices != null) {
				if (!ts->vi_allocated)
					ts->vi_allocated = ts->total;
				FREE_ARRAY (ts->vertex_indices, ts->vi_allocated, int);
			}
			if (ts->face_indices != null) {
				if (!ts->fi_allocated)
					ts->fi_allocated = ts->total;
				FREE_ARRAY (ts->face_indices, ts->fi_allocated, int);
			}
		}

		if (ts->short_vertex_indices != null && ANYBIT (ts->ownership, SDO_SHORT_INDICES)) {			
			FREE_ARRAY (ts->short_vertex_indices, ts->short_vertex_indices_allocated, short);			
		}

		/* if this tristrip is not hanging from the database, but is instead the leftovers from a
		   draw-deferral operation, we need to free more than we ordinarily would */
		if (ts->points != null && ANYBIT (ts->ownership, SDO_POINTS))
			FREE_ARRAY (ts->points, ts->point_count, Point);

#ifndef DISABLE_GEOMETRY_ATTRIBUTES
#  ifndef DISABLE_GEOMETRY_COLORS
		if (ts->vertex_colors != null && ANYBIT (ts->ownership, SDO_VERTEX_COLORS))
			FREE_ARRAY (ts->vertex_colors, ts->point_count, RGB);
		if (ts->vertex_findices != null && ANYBIT (ts->ownership, SDO_VERTEX_FINDICES))
			FREE_ARRAY (ts->vertex_findices, ts->point_count, Color_Float_Index);
		if (ts->face_colors != null && ANYBIT (ts->ownership, SDO_FACE_COLORS))
			FREE_ARRAY (ts->face_colors, ts->face_count, RGB);
#  endif

#  ifndef DISABLE_GEOMETRY_NORMALS
		if (ts->vertex_normals != null && ANYBIT (ts->ownership, SDO_VERTEX_NORMALS))
			FREE_ARRAY (ts->vertex_normals, ts->point_count, Vector);
		if (ts->face_normals != null && ANYBIT (ts->ownership, SDO_FACE_NORMALS))
			FREE_ARRAY (ts->face_normals, ts->face_count, Plane);
#  endif

#  ifndef DISABLE_TEXTURING
		if (ts->texture_params != null && DECR_UTILITY (ts->texture_params) == 0)
			Texture_Parameters::free_list(ts->texture_params);
#  endif
#endif

		if (ts->index_source && DECR_UTILITY (ts->index_source) == 0)
			Tristrip::free_one ((Tristrip *)ts->index_source);

		if (ts->display_lists) {
			Display_List *		dl;

			while ((dl = ts->display_lists) != null) {
				ts->display_lists = dl->next;
				INUTILE(dl);
			}
		}

		ts->bounding.release();
		ts->face_attributes.highlight = null;

		/* this should be the only place where you see a Tristrip passed straight into FREE */
		FREE (ts, Tristrip);
	}

	local INLINE void free_list (Tristrip * curr) {
		Tristrip *	next;
		if (curr) do {
			next = curr->next;
			if (DECR_UTILITY (curr) <= 0)
				Tristrip::free_one (curr);
		} while ((curr = next) != null);
	}

	local INLINE size_t sum_memory_used(Tristrip * curr, MU_Flags flags) {
		size_t		size = 0;
		Tristrip *	next;
		if (curr) do {
			next = curr->next;
			size += curr->memory_used(flags);
		} while ((curr = next) != null);
		return size;
	}

	size_t memory_used(MU_Flags flags) const {

		size_t size = 0;

		if (ANYBIT(flags, MU_DERIVED)) {

			size += sizeof(Tristrip);

			if (ANYBIT (ownership, SDO_INDICES)) {
				if (lengths != null) {
					if (lengths_allocated == 0)
						size += strips * sizeof(int);
					else
						size += lengths_allocated * sizeof(int);
				}
				if (vertex_indices != null) {
					if (vi_allocated == 0)
						size += total * sizeof(int);
					else
						size += vi_allocated * sizeof(int);
				}
				if (face_indices != null) {
					if (!fi_allocated)
						size += total * sizeof(int);
					else
						size += fi_allocated * sizeof(int);
				}

				if (short_vertex_indices != null)
					size += short_vertex_indices_allocated * sizeof(short);
			}

			if (points != null && ANYBIT (ownership, SDO_POINTS))
				size += point_count * sizeof(Point);

#ifndef DISABLE_GEOMETRY_ATTRIBUTES
#  ifndef DISABLE_GEOMETRY_COLORS
			if (vertex_colors != null && ANYBIT (ownership, SDO_VERTEX_COLORS))
				size += point_count * sizeof(RGB);
			if (vertex_findices != null && ANYBIT (ownership, SDO_VERTEX_FINDICES))
				size += point_count * sizeof(Color_Float_Index);
			if (face_colors != null && ANYBIT (ownership, SDO_FACE_COLORS))
				size += face_count * sizeof(RGB);
#  endif

#  ifndef DISABLE_GEOMETRY_NORMALS
			if (vertex_normals != null && ANYBIT (ownership, SDO_VERTEX_NORMALS))
				size += point_count * sizeof(Vector);
			if (face_normals != null && ANYBIT (ownership, SDO_FACE_NORMALS))
				size += face_count * sizeof(Plane);
#  endif

#  ifndef DISABLE_TEXTURING
			if (texture_params != null)
				size += Texture_Parameters::sum_memory_used(texture_params, flags);
#  endif
#endif
		}

// 		if (index_source && DECR_UTILITY (index_source) == 0)
// 			Tristrip::free_one ((Tristrip *)index_source);
// 
// 		if (display_lists) {
// 			Display_List *		dl;
// 
// 			while ((dl = display_lists) != null) {
// 				display_lists = dl->next;
// 				INUTILE(dl);
// 			}
// 		}
// 
// 		bounding.release();
// 		face_attributes.style.release();

		return size;
	}

};


/* edge indices gets 1 dummy pad value per strip */

struct Polyedge : Strip_Header<Polyedge> {
	int						strips;
	int						total;
	int *					lengths;
	int *					vertex_indices;
	int *					edge_indices;

	int						point_count;
	int						edge_count;

	Point *					points;
	DPoint *				dpoints;
	RGB *					vertex_colors;
	float *					vertex_findices;
	Vector *				vertex_normals;
	Parameter *				vertex_params;
	int32_t					vertex_param_width;
	Parameter_Flags			vertex_param_flags;
	float *					vertex_priorities;

	RGB *					edge_colors;
	Vector *				edge_normals;

	Polyedge const *		index_source;

#	ifdef CADDS5
	int *					list;
#	endif

	int						lengths_allocated;
	int						vi_allocated;
	int						ei_allocated;

	One_Edge_Attributes		edge_attributes;

	int						same_colors;
	bool					color_interpolated;
	bool					single_colored;

	static Polyedge *	copy(Polyedge const * in, Geometry const * new_owner) {

		Polyedge *	head = null;
		Polyedge **	linker = &head;

		if (in != null) do {
			Polyedge *		out;
			ZALLOC (out, Polyedge);
			*linker = out;
			linker = &out->next;

			INIT_UTILITY (out);
			out->parent_geom = new_owner;
			out->bounding = in->bounding;

			out->edge_attributes = in->edge_attributes;

			CLONE_ARRAY(in->lengths, in->strips, int, out->lengths);
			out->lengths_allocated = in->strips;
			CLONE_ARRAY(in->vertex_indices, in->total, int, out->vertex_indices);
			CLONE_ARRAY(in->edge_indices, in->total, int, out->edge_indices);
			out->vi_allocated = out->ei_allocated = in->total;

			out->ownership |= SDO_INDICES;

			out->total = in->total;
			out->strips = in->strips;
		} while ((in = in->next) != null);

		return head;
	}

	static void	free_one (Polyedge * pe) {

		if (ANYBIT (pe->ownership, SDO_INDICES)) {
			if (pe->lengths && pe->lengths_allocated > 0)
				FREE_ARRAY (pe->lengths, pe->lengths_allocated, int);
			if (pe->vertex_indices && pe->vi_allocated > 0)
				FREE_ARRAY (pe->vertex_indices, pe->vi_allocated, int);
			if (pe->edge_indices && pe->ei_allocated > 0)
				FREE_ARRAY (pe->edge_indices, pe->ei_allocated, int);
		}

		/* if this polyedge is not hanging from the database, but is instead the leftovers from a
		   draw-deferral operation, we need to free more than we ordinarily would */
		if (pe->points != null && ANYBIT (pe->ownership, SDO_POINTS))
			FREE_ARRAY (pe->points, pe->point_count, Point);

#ifndef DISABLE_GEOMETRY_ATTRIBUTES
#  ifndef DISABLE_GEOMETRY_COLORS
		if (pe->vertex_colors != null && ANYBIT (pe->ownership, SDO_VERTEX_COLORS))
			FREE_ARRAY (pe->vertex_colors, pe->point_count, RGB);
		if (pe->vertex_findices != null && ANYBIT (pe->ownership, SDO_VERTEX_FINDICES))
			FREE_ARRAY (pe->vertex_findices, pe->point_count, Color_Float_Index);
		if (pe->edge_colors != null && ANYBIT (pe->ownership, SDO_EDGE_COLORS))
			FREE_ARRAY (pe->edge_colors, pe->edge_count, RGB);
#  endif

#  ifndef DISABLE_GEOMETRY_NORMALS
		if (pe->vertex_normals != null && ANYBIT (pe->ownership, SDO_VERTEX_NORMALS))
			FREE_ARRAY (pe->vertex_normals, pe->point_count, Vector);
		if (pe->edge_normals != null && ANYBIT (pe->ownership, SDO_EDGE_NORMALS))
			FREE_ARRAY (pe->edge_normals, pe->edge_count, Vector);
#  endif

#  ifndef DISABLE_TEXTURING
		if (pe->vertex_params != null && ANYBIT (pe->ownership, SDO_VERTEX_PARAMETERS))
			FREE_ARRAY (pe->vertex_params, pe->point_count * pe->vertex_param_width, Parameter);
#  endif
#endif

		if (pe->vertex_priorities != null && ANYBIT (pe->ownership, SDO_VERTEX_PRIORITIES))
			FREE_ARRAY (pe->vertex_priorities, pe->point_count, float);

		if (pe->index_source != null && DECR_UTILITY (pe->index_source) == 0)
			Polyedge::free_one ((Polyedge *)pe->index_source);

		if (pe->display_lists) {
			Display_List *      dl;

			while ((dl = pe->display_lists) != null) {
				pe->display_lists = dl->next;
				INUTILE(dl);
			}
		}

		pe->bounding.release();
		pe->edge_attributes.highlight = null;

		/* this should be the only place where you see an Polyedge passed straight into FREE */
		FREE (pe, Polyedge);
	}

	local void free_list (Polyedge * curr) {
		Polyedge *	next;
		if (curr) do {
			next = curr->next;
			if (DECR_UTILITY (curr) <= 0)
				Polyedge::free_one (curr);
		} while ((curr = next) != null);
	}

	local INLINE size_t sum_memory_used(Polyedge * curr, MU_Flags flags) {
		size_t		size = 0;
		Polyedge *	next;
		if (curr) do {
			next = curr->next;
			size += curr->memory_used(flags);
		} while ((curr = next) != null);
		return size;
	}

	size_t memory_used(MU_Flags flags) const {

		size_t size = 0;

		if (ANYBIT(flags, MU_DERIVED)) {

			size += sizeof(Polyedge);

			if (ANYBIT (ownership, SDO_INDICES)) {
				if (lengths && lengths_allocated > 0)
					size += lengths_allocated * sizeof(int);
				if (vertex_indices && vi_allocated > 0)
					size += vi_allocated * sizeof(int);
				if (edge_indices && ei_allocated > 0)
					size += ei_allocated * sizeof(int);
			}

			if (points != null && ANYBIT (ownership, SDO_POINTS))
				size += point_count * sizeof(Point);

#ifndef DISABLE_GEOMETRY_ATTRIBUTES
#  ifndef DISABLE_GEOMETRY_COLORS
			if (vertex_colors != null && ANYBIT (ownership, SDO_VERTEX_COLORS))
				size += point_count * sizeof(RGB);
			if (vertex_findices != null && ANYBIT (ownership, SDO_VERTEX_FINDICES))
				size += point_count * sizeof(Color_Float_Index);
			if (edge_colors != null && ANYBIT (ownership, SDO_EDGE_COLORS))
				size += edge_count * sizeof(RGB);
#  endif

#  ifndef DISABLE_GEOMETRY_NORMALS
			if (vertex_normals != null && ANYBIT (ownership, SDO_VERTEX_NORMALS))
				size += point_count * sizeof(Vector);
			if (edge_normals != null && ANYBIT (ownership, SDO_EDGE_NORMALS))
				size += edge_count * sizeof(Vector);
#  endif

#  ifndef DISABLE_TEXTURING
			if (vertex_params != null && ANYBIT (ownership, SDO_VERTEX_PARAMETERS))
				size += (point_count * vertex_param_width) * sizeof(Parameter);
#  endif
#endif

			if (vertex_priorities != null && ANYBIT (ownership, SDO_VERTEX_PRIORITIES))
				size += point_count * sizeof(float);

// 		if (index_source != null && DECR_UTILITY (index_source) == 0)
// 			Polyedge::free_one ((Polyedge *)index_source);
// 
// 		if (display_lists) {
// 			Display_List *      dl;
// 
// 			while ((dl = display_lists) != null) {
// 				display_lists = dl->next;
// 				INUTILE(dl);
// 			}
// 		}
// 
// 		bounding.release();
// 		edge_attributes.style.release();
		}

		return size;
	}
};




struct Polymarker : public Strip_Header<Polymarker> {

	int *					vertex_indices;
	int						length;
	int						point_count;

	Point *					points;
	DPoint *				dpoints;
	RGB *					vertex_colors;
	RGBAS32 *				vertex_rgbas;
	float *					vertex_findices;
	Vector *				vertex_normals;
	Parameter *				vertex_params;
	int32_t					vertex_param_width;
	Parameter_Flags			vertex_param_flags;
	float *					vertex_priorities;

	Polymarker *			index_source;

#	ifdef CADDS5
	int *					list;
#	endif

	int						vi_allocated;

	One_Vertex_Attributes	vertex_attributes;

	int						same_colors;

	bool					color_interpolated;
	bool					single_colored;

	local Polymarker *	copy (Polymarker const * in, Geometry const * new_owner) {

		Polymarker *	head = null;
		Polymarker **	linker = &head;

		if (in != null) do {
			Polymarker *	out;
			ZALLOC (out, Polymarker);
			*linker = out;
			linker = &out->next;

			INIT_UTILITY (out);
			out->parent_geom = new_owner;
			out->bounding = in->bounding;

			out->vertex_attributes = in->vertex_attributes;

			out->length = in->length;
			if (in->vi_allocated > 0 && in->length > 0) {
				int len;

				if (in->vi_allocated > in->length)
					len = in->length;
				else
					len = in->vi_allocated;
				CLONE_ARRAY(in->vertex_indices, len, int, out->vertex_indices);
				out->vi_allocated = len;
				out->ownership |= SDO_INDICES;
			}
		} while ((in = in->next) != null);

		return head;
	}

	local void free_one (Polymarker * pm) {

		if (pm->vi_allocated && ANYBIT (pm->ownership, SDO_INDICES))
			FREE_ARRAY (pm->vertex_indices, pm->vi_allocated, int);

		if (pm->points != null && ANYBIT (pm->ownership, SDO_POINTS))
			FREE_ARRAY (pm->points, pm->point_count, Point);
		if (pm->dpoints != null && ANYBIT (pm->ownership, SDO_POINTS))
			FREE_ARRAY (pm->dpoints, pm->point_count, DPoint);

#ifndef DISABLE_GEOMETRY_ATTRIBUTES
#  ifndef DISABLE_GEOMETRY_COLORS
		if (pm->vertex_colors != null && ANYBIT (pm->ownership, SDO_VERTEX_COLORS))
			FREE_ARRAY (pm->vertex_colors, pm->point_count, RGB);
		if (pm->vertex_rgbas != null && ANYBIT (pm->ownership, SDO_VERTEX_RGBAS))
			FREE_ARRAY (pm->vertex_rgbas, pm->point_count, RGBAS32);
		if (pm->vertex_findices != null && ANYBIT (pm->ownership, SDO_VERTEX_FINDICES))
			FREE_ARRAY (pm->vertex_findices, pm->point_count, Color_Float_Index);
#  endif

#  ifndef DISABLE_GEOMETRY_NORMALS
		if (pm->vertex_normals != null && ANYBIT (pm->ownership, SDO_VERTEX_NORMALS))
			FREE_ARRAY (pm->vertex_normals, pm->point_count, Vector);
#  endif

#  ifndef DISABLE_TEXTURING
		if (pm->vertex_params != null && ANYBIT (pm->ownership, SDO_VERTEX_PARAMETERS))
			FREE_ARRAY (pm->vertex_params, pm->point_count * pm->vertex_param_width, Parameter);
#  endif
#endif

		if (pm->display_lists) {
			Display_List *	dl;

			while ((dl = pm->display_lists) != null) {
				pm->display_lists = dl->next;
				INUTILE(dl);
			}
		}

		pm->bounding.release();
		pm->vertex_attributes.highlight = null;
		FREE (pm, Polymarker);
	}

	local void free_list (Polymarker * pm) {
		Polymarker *	next;
		if (pm) do {
			next = pm->next;
			if (DECR_UTILITY (pm) <= 0)
				Polymarker::free_one (pm);
		} while ((pm = next) != null);
	}

	local INLINE size_t sum_memory_used(Polymarker * curr, MU_Flags flags) {
		size_t			size = 0;
		Polymarker *	next;
		if (curr) do {
			next = curr->next;
			size += curr->memory_used(flags);
		} while ((curr = next) != null);
		return size;
	}

	size_t memory_used(MU_Flags flags) const {

		size_t size = 0;

		if (ANYBIT(flags, MU_DERIVED)) {

			size += sizeof(Polymarker);

			if (vi_allocated && ANYBIT (ownership, SDO_INDICES))
				size += vi_allocated * sizeof(int);
			if (points != null && ANYBIT (ownership, SDO_POINTS))
				size += point_count * sizeof(Point);
			if (dpoints != null && ANYBIT (ownership, SDO_POINTS))
				size += point_count * sizeof(DPoint);

#ifndef DISABLE_GEOMETRY_ATTRIBUTES
#  ifndef DISABLE_GEOMETRY_COLORS
			if (vertex_colors != null && ANYBIT (ownership, SDO_VERTEX_COLORS))
				size += point_count * sizeof(RGB);
			if (vertex_rgbas != null && ANYBIT (ownership, SDO_VERTEX_RGBAS))
				size += point_count * sizeof(RGBAS32);
			if (vertex_findices != null && ANYBIT (ownership, SDO_VERTEX_FINDICES))
				size += point_count * sizeof(Color_Float_Index);
#  endif

#  ifndef DISABLE_GEOMETRY_NORMALS
			if (vertex_normals != null && ANYBIT (ownership, SDO_VERTEX_NORMALS))
				size += point_count * sizeof(Vector);
#  endif

#  ifndef DISABLE_TEXTURING
			if (vertex_params != null && ANYBIT (ownership, SDO_VERTEX_PARAMETERS))
				size += (point_count * vertex_param_width) * sizeof(Parameter);
#  endif
#endif
		}

// 			if (display_lists) {
// 				Display_List *	dl;
// 
// 				while ((dl = display_lists) != null) {
// 					display_lists = dl->next;
// 					INUTILE(dl);
// 				}
// 			}
// 
// 			bounding.release();
// 			vertex_attributes.style.release(		}

		return size;
	}
};



////////////////////////////////////////////////////////////////////////////////


typedef char			Face_Orientation;
#define FO_FORWARDS		(Face_Orientation)0x1
#define FO_BACKWARDS	(Face_Orientation)0x2
#define FO_WHOKNOWS		(Face_Orientation)0x4
#define FO_CULLED		(Face_Orientation)0x8



struct Tristrip_Data {
	Tristrip *					tristrips;
	Destructible<Entity_Highlights>::unique_ptr	highlight_styles;
	Composite_Face_Flags		valid_tristrip_flags;

	void operator = (Tristrip_Data const & that) {
		tristrips = null;
		highlight_styles = null;
		valid_tristrip_flags = that.valid_tristrip_flags;
	}

	INLINE size_t memory_used(MU_Flags flags) const {
		size_t size = sizeof(Tristrip_Data);
		size += Tristrip::sum_memory_used(tristrips, flags);
		return size;
	}

private:
	Tristrip_Data(Tristrip_Data const &);
};

struct Polyedge_Data {
	Polyedge *					polyedges;
	Destructible<Entity_Highlights>::unique_ptr	highlight_styles;

	Local_Edge_Flags			valid_polyedge_flags;
	float						hard_edge_dihedral;

	void operator = (Polyedge_Data const & that) {
		polyedges = null;
		highlight_styles = null;
		valid_polyedge_flags = that.valid_polyedge_flags;
		hard_edge_dihedral = that.hard_edge_dihedral;
	}

	INLINE size_t memory_used(MU_Flags flags) const {
		size_t size = sizeof(Polyedge_Data);
		size += Polyedge::sum_memory_used(polyedges, flags);
		return size;
	}

private:
	Polyedge_Data(Polyedge_Data const &);
};

struct Polymarker_Data {
	Polymarker *				polymarkers;
	Destructible<Entity_Highlights>::unique_ptr	highlight_styles;

	Local_Vertex_Flags			valid_polymarker_flags;

	void operator = (Polymarker_Data const & that) {
		polymarkers = null;
		highlight_styles = null;
		valid_polymarker_flags = that.valid_polymarker_flags;
	}

	INLINE size_t memory_used(MU_Flags flags) const {
		size_t size = sizeof(Polymarker_Data);
		size += Polymarker::sum_memory_used(polymarkers, flags);
		return size;
	}

private:
	Polymarker_Data(Polymarker_Data const &);
};

struct Isoline_Data {
	Polyedge *					isolines;
	Destructible<Entity_Highlights>::unique_ptr	highlight_styles;

	struct V {
		float		scale;
		float		translate;
		int			position_type;
		int			position_count;
		float  *	positions;
	}		valid;

	int		isoline_index_count;

	void operator = (Isoline_Data const & that) {
		isolines = null;
		highlight_styles = null;
		valid = that.valid;
		isoline_index_count = that.isoline_index_count;
	}

	INLINE size_t memory_used(MU_Flags flags) const {
		size_t size = sizeof(Isoline_Data);
		size += Polyedge::sum_memory_used(isolines, flags);
		return size;
	}

private:
	Isoline_Data(Isoline_Data const &);
};

struct Polyhedron_Extended_Data : public Pointer_Cache {
	HOOPS_API ~Polyhedron_Extended_Data ();

	protected:
		enum {
			Extended_Tristrip_Data,
			Extended_Polyedge_Data,
			Extended_Polymarker_Data,
			Extended_Isoline_Data,
			Extended_LNA,
			Extended_LSA,
			Extended_LVA,
			Extended_LFA,
			Extended_LEA,
			Extended_Highlight_Tristrip_Data,
			Extended_Highlight_Polyedge_Data,
			Extended_Highlight_Polymarker_Data,
			Extended_Highlight_Isoline_Data,
			Extended_Degenerates,	// face normals
			Extended_LOD
		};

		struct LOD_Cache : public CMO {
			LOD_Node *			lodbase;
		};

	public:
		void						set_tristrip_data (Tristrip_Data * tsd) { set (Extended_Tristrip_Data, tsd); }
		INLINE Tristrip_Data *		get_tristrip_data () const { return (Tristrip_Data *)get (Extended_Tristrip_Data); }

		void						set_polyedge_data (Polyedge_Data * ped) { set (Extended_Polyedge_Data, ped); }
		INLINE Polyedge_Data *		get_polyedge_data () const { return (Polyedge_Data *)get (Extended_Polyedge_Data); }

		void						set_polymarker_data (Polymarker_Data * pmd) { set (Extended_Polymarker_Data, pmd); }
		INLINE Polymarker_Data *	get_polymarker_data () const { return (Polymarker_Data *)get (Extended_Polymarker_Data); }

		void						set_isoline_data (Isoline_Data * ild) { set (Extended_Isoline_Data, ild); }
		INLINE Isoline_Data *		get_isoline_data () const { return (Isoline_Data *)get (Extended_Isoline_Data); }

		void						set_highlight_tristrip_data (Tristrip_Data * tsd) { set (Extended_Highlight_Tristrip_Data, tsd); }
		INLINE Tristrip_Data *		get_highlight_tristrip_data () const { return (Tristrip_Data *)get (Extended_Highlight_Tristrip_Data); }

		void						set_highlight_polyedge_data (Polyedge_Data * ped) { set (Extended_Highlight_Polyedge_Data, ped); }
		INLINE Polyedge_Data *		get_highlight_polyedge_data () const { return (Polyedge_Data *)get (Extended_Highlight_Polyedge_Data); }

		void						set_highlight_polymarker_data (Polymarker_Data * pmd) { set (Extended_Highlight_Polymarker_Data, pmd); }
		INLINE Polymarker_Data *	get_highlight_polymarker_data () const { return (Polymarker_Data *)get (Extended_Highlight_Polymarker_Data); }

		void						set_highlight_isoline_data (Isoline_Data * ild) { set (Extended_Highlight_Isoline_Data, ild); }
		INLINE Isoline_Data *		get_highlight_isoline_data () const { return (Isoline_Data *)get (Extended_Highlight_Isoline_Data); }

		void								set_lna (Local_Normal_Attributes * lna) { set (Extended_LNA, lna); }
		INLINE Local_Normal_Attributes *	get_lna () const { return (Local_Normal_Attributes *)get (Extended_LNA); }

//		void								set_lsa (Local_Shared_Attributes * lsa) { set (Extended_LSA, lsa); }
//		INLINE Local_Shared_Attributes *	get_lsa () const { return (Local_Shared_Attributes *)get (Extended_LSA); }

		void								set_lva (Local_Vertex_Attributes * lva) { set (Extended_LVA, lva); }
		INLINE Local_Vertex_Attributes *	get_lva () const { return (Local_Vertex_Attributes *)get (Extended_LVA); }

		void								set_lfa (Local_Face_Attributes * lfa) { set (Extended_LFA, lfa); }
		INLINE Local_Face_Attributes *		get_lfa () const { return (Local_Face_Attributes *)get (Extended_LFA); }

		void								set_lea (Local_Edge_Attributes * lea) { set (Extended_LEA, lea); }
		INLINE Local_Edge_Attributes *		get_lea () const { return (Local_Edge_Attributes *)get (Extended_LEA); }

		void					set_degenerates (int32_t d) { set (Extended_Degenerates, (void *)(intptr_t)d); }
		INLINE int32_t		get_degenerates () const { return (int32_t)(intptr_t)get (Extended_Degenerates); }

		void	set_lod (LOD_Node * lod) {
			LOD_Cache * lc = (LOD_Cache *)get (Extended_LOD);

			if (lod != null) {
				if (lc == null) {
					lc = NEW(LOD_Cache)();
					set (Extended_LOD, lc);
				}
				lc->lodbase = lod;
				lod->backlink = (Anything **)&lc->lodbase;
			}
			else {
				delete lc;
				set (Extended_LOD, null);
			}
		}
		INLINE LOD_Node *	get_lod () const {
			LOD_Cache * lc = (LOD_Cache *)get (Extended_LOD);

			if (lc == null)
				return null;
			return lc->lodbase;
		}
};



#define REGION_RANGE_LOW_OFFSET			0
#define REGION_RANGE_HIGH_OFFSET		1
#define REGION_RANGE_VALIDITY_OFFSET	2
#define REGION_LOW_HIGH_EXTRA			3 /* 1 for the low index, 2 for the high index, 3 for a validity flag. */

struct Polyhedron_Contents_Flags {
	unsigned short lva_any;
	unsigned short lfa_any;
	unsigned short lea_any;
	unsigned short other;
};

#define Shell_Face_List_Valid	DB_TYPE_SPECIFIC(1)

template <typename T>
struct Polyhedron_Base : public Geometry_X<Point_3D> {

private:
	Polyhedron_Base & operator= (Polyhedron_Base const &);

public:

	Bounding					bounding;
	Polyhedron_Extended_Data *	extended;

	T *							data;

	Polyhedron_Counts			counts;
	Polyhedron_Contents_Flags	contents_flags;

	typedef Polyhedron_Base<T> PB;

	Polyhedron_Base(DB_Type in_type) : PB::Geometry_X<Point_3D>(in_type) {
		;
	};

	~Polyhedron_Base();

	Polyhedron_Base (Polyhedron_Base const & that);

	size_t memory_used(MU_Flags flags) const;

#define PH_OTHER_DOWNWIND_ATTRIBUTES 0x01

	INLINE void update_contents_flags(Local_Vertex_Attributes * lva) {update_contents_flags(lva, null, null, false);}
	INLINE void update_contents_flags(Local_Face_Attributes * lfa) {update_contents_flags(null, lfa, null, false);}
	INLINE void update_contents_flags(Local_Edge_Attributes * lea) {update_contents_flags(null, null, lea, false);}
	INLINE void update_contents_flags(bool other) {update_contents_flags(null, null, null, other);}
	INLINE void update_contents_flags() {update_contents_flags(get_lva(), get_lfa(), get_lea(), true);}

	HOOPS_API void update_contents_flags(Local_Vertex_Attributes * lva, Local_Face_Attributes * lfa, Local_Edge_Attributes * lea, bool other);

	protected:
		void					ensure_extended () {
									Prefetch (extended);
									if (extended == null)
										extended = NEW(Polyhedron_Extended_Data)();
								}
		void					clean_empty_extended () {
									if (extended->empty()) {
										delete extended;
										extended = null;
									}
								}

	public:
		void						set_tristrip_data (Tristrip_Data * tsd) {
										if (tsd != null) {
											ensure_extended();
											extended->set_tristrip_data(tsd);
										}
										else {
											extended->set_tristrip_data(null);
											clean_empty_extended();
										}
									}
		INLINE Tristrip_Data *		get_tristrip_data () const {
										if (extended == null)
											return null;
										return extended->get_tristrip_data();
									}

		void						set_polyedge_data (Polyedge_Data * ped) {
										if (ped != null) {
											ensure_extended();
											extended->set_polyedge_data(ped);
										}
										else {
											extended->set_polyedge_data(null);
											clean_empty_extended();
										}
									}
		INLINE Polyedge_Data *		get_polyedge_data () const {
										if (extended == null)
											return null;
										return extended->get_polyedge_data();
									}

		void						set_polymarker_data (Polymarker_Data * pmd) {
										if (pmd != null) {
											ensure_extended();
											extended->set_polymarker_data(pmd);
										}
										else {
											extended->set_polymarker_data(null);
											clean_empty_extended();
										}
									}
		INLINE Polymarker_Data *	get_polymarker_data () const {
										if (extended == null)
											return null;
										return extended->get_polymarker_data();
									}

		void						set_isoline_data (Isoline_Data * ild) {
										if (ild != null) {
											ensure_extended();
											extended->set_isoline_data(ild);
										}
										else {
											extended->set_isoline_data(null);
											clean_empty_extended();
										}
									}
		INLINE Isoline_Data *		get_isoline_data () const {
										if (extended == null)
											return null;
										return extended->get_isoline_data();
									}


		void						set_highlight_tristrip_data (Tristrip_Data * tsd) {
										if (tsd != null) {
											ensure_extended();
											extended->set_highlight_tristrip_data(tsd);
										}
										else {
											extended->set_highlight_tristrip_data(null);
											clean_empty_extended();
										}
									}
		INLINE Tristrip_Data *		get_highlight_tristrip_data () const {
										if (extended == null)
											return null;
										return extended->get_highlight_tristrip_data();
									}

		void						set_highlight_polyedge_data (Polyedge_Data * ped) {
										if (ped != null) {
											ensure_extended();
											extended->set_highlight_polyedge_data(ped);
										}
										else {
											extended->set_highlight_polyedge_data(null);
											clean_empty_extended();
										}
									}
		INLINE Polyedge_Data *		get_highlight_polyedge_data () const {
										if (extended == null)
											return null;
										return extended->get_highlight_polyedge_data();
									}

		void						set_highlight_polymarker_data (Polymarker_Data * pmd) {
										if (pmd != null) {
											ensure_extended();
											extended->set_highlight_polymarker_data(pmd);
										}
										else {
											extended->set_highlight_polymarker_data(null);
											clean_empty_extended();
										}
									}
		INLINE Polymarker_Data *	get_highlight_polymarker_data () const {
										if (extended == null)
											return null;
										return extended->get_highlight_polymarker_data();
									}

		void						set_highlight_isoline_data (Isoline_Data * ild) {
										if (ild != null) {
											ensure_extended();
											extended->set_highlight_isoline_data(ild);
										}
										else {
											extended->set_highlight_isoline_data(null);
											clean_empty_extended();
										}
									}
		INLINE Isoline_Data *		get_highlight_isoline_data () const {
										if (extended == null)
											return null;
										return extended->get_highlight_isoline_data();
									}


		INLINE Tristrip *		get_tristrips () const {
									Tristrip_Data * tsd = get_tristrip_data();
									if (tsd == null)
										return null;
									Prefetch (tsd->tristrips);
									return tsd->tristrips;
								}

		INLINE Polyedge *		get_polyedges () const {
									Polyedge_Data * ped = get_polyedge_data();
									if (ped == null)
										return null;
									Prefetch (ped->polyedges);
									return ped->polyedges;
								}

		INLINE Polymarker *		get_polymarkers () const {
									Polymarker_Data * pmd = get_polymarker_data();
									if (pmd == null)
										return null;
									Prefetch (pmd->polymarkers);
									return pmd->polymarkers;
								}

		INLINE Polyedge *		get_isolines () const {
									Isoline_Data * ild = get_isoline_data();
									if (ild == null)
										return null;
									Prefetch (ild->isolines);
									return ild->isolines;
								}


		INLINE Tristrip *		get_highlight_tristrips () const {
									Tristrip_Data * tsd = get_highlight_tristrip_data();
									if (tsd == null)
										return null;
									Prefetch (tsd->tristrips);
									return tsd->tristrips;
								}

		INLINE Polyedge *		get_highlight_polyedges () const {
									Polyedge_Data * ped = get_highlight_polyedge_data();
									if (ped == null)
										return null;
									Prefetch (ped->polyedges);
									return ped->polyedges;
								}

		INLINE Polymarker *		get_highlight_polymarkers () const {
									Polymarker_Data * pmd = get_highlight_polymarker_data();
									if (pmd == null)
										return null;
									Prefetch (pmd->polymarkers);
									return pmd->polymarkers;
								}

		INLINE Polyedge *		get_highlight_isolines () const {
									Isoline_Data * ild = get_highlight_isoline_data();
									if (ild == null)
										return null;
									Prefetch (ild->isolines);
									return ild->isolines;
								}

#if 0
		void								set_lsa (Local_Shared_Attributes * lsa) {
		INLINE Local_Shared_Attributes *	get_lsa () const {
#endif

		INLINE Local_Normal_Attributes const *	get_lna () const {
														if (extended == null)
															return null;
														return extended->get_lna();
													}
		INLINE Local_Normal_Attributes *	get_lna () {
														if (extended == null)
															return null;
														return extended->get_lna();
													}
		INLINE Local_Normal_Attributes *	ensure_lna () {
													ensure_extended();
													Local_Normal_Attributes *	lna = extended->get_lna();
													if (lna == null) {
														ZALLOC (lna, Local_Normal_Attributes);
														extended->set_lna (lna);

														lna->entity_counts = &counts;
													}
													return lna;
												}

		INLINE Local_Vertex_Attributes const *	get_lva () const {
														if (extended == null)
															return null;
														return extended->get_lva();
													}
		INLINE Local_Vertex_Attributes *	get_lva () {
														if (extended == null)
															return null;
														return extended->get_lva();
													}
		INLINE Local_Vertex_Attributes *	ensure_lva () {
													ensure_extended();
													Local_Vertex_Attributes *	lva = extended->get_lva();
													if (lva == null) {
														ZALLOC (lva, Local_Vertex_Attributes);
														extended->set_lva (lva);

														lva->entity_counts = &counts;
													}
													return lva;
												}

		INLINE Local_Face_Attributes const *	get_lfa () const {
													if (extended == null)
														return null;
													return extended->get_lfa();
												}
		INLINE Local_Face_Attributes *	get_lfa () {
													if (extended == null)
														return null;
													return extended->get_lfa();
												}
		INLINE Local_Face_Attributes *	ensure_lfa () {
													ensure_extended();
													Local_Face_Attributes *	lfa = extended->get_lfa();
													if (lfa == null) {
														ZALLOC (lfa, Local_Face_Attributes);
														extended->set_lfa (lfa);

														lfa->entity_counts = &counts;
													}
													return lfa;
												}

		INLINE Local_Edge_Attributes const *	get_lea () const {
													if (extended == null)
														return null;
													return extended->get_lea();
												}
		INLINE Local_Edge_Attributes *	get_lea () {
													if (extended == null)
														return null;
													return extended->get_lea();
												}
		INLINE Local_Edge_Attributes *	ensure_lea () {
													ensure_extended();
													Local_Edge_Attributes *	lea = extended->get_lea();
													if (lea == null) {
														ZALLOC (lea, Local_Edge_Attributes);
														extended->set_lea (lea);

														lea->entity_counts = &counts;
													}
													return lea;
												}

		void						set_degenerates (int32_t d) {
										if (d != 0) {
											ensure_extended();
											extended->set_degenerates(d);
										}
										else {
											extended->set_degenerates(0);
											clean_empty_extended();
										}
									}
		INLINE int32_t				get_degenerates () const {
										if (extended == null)
											return 0;
										return extended->get_degenerates();
									}



		INLINE LOD_Node const *		get_lod () const {
										if (extended == null)
											return null;
										return extended->get_lod();
									}
		INLINE LOD_Node *			get_lod () {
										if (extended == null)
											return null;
										return extended->get_lod();
									}
		INLINE void					set_lod (LOD_Node * lod) {
										ensure_extended();
										extended->set_lod (lod);
									}



		void discard_tristrips () {
			Tristrip_Data *	tsd = get_tristrip_data();
			if (tsd != null) {
				if (tsd->tristrips != null)
					Tristrip::free_list (tsd->tristrips);

				if (tsd->highlight_styles != null) {
					tsd->highlight_styles->NullAll();
					tsd->highlight_styles = null;
				}
				FREE (tsd, Tristrip_Data);
				set_tristrip_data (null);
			}
		}

		void discard_polyedges () {
			Polyedge_Data *	ped = get_polyedge_data();
			if (ped != null) {
				if (ped->polyedges != null)
					Polyedge::free_list (ped->polyedges);

				if (ped->highlight_styles != null) {
					ped->highlight_styles->NullAll();
					ped->highlight_styles = null;
				}
				FREE (ped, Polyedge_Data);
				set_polyedge_data (null);
			}
		}

		void discard_polymarkers () {
			Polymarker_Data *	pmd = get_polymarker_data();
			if (pmd != null) {
				if (pmd->polymarkers != null)
					Polymarker::free_list (pmd->polymarkers);

				if (pmd->highlight_styles != null) {
					pmd->highlight_styles->NullAll();
					pmd->highlight_styles = null;
				}
				FREE (pmd, Polymarker_Data);
				set_polymarker_data (null);
			}
		}

		void discard_isolines () {
			Isoline_Data *	ild = get_isoline_data();
			if (ild != null) {
				if (ild->isolines != null)
					Polyedge::free_list (ild->isolines);

				if (ild->valid.position_count > 0)
					FREE_ARRAY (ild->valid.positions, ild->valid.position_count, float);

				if (ild->highlight_styles != null) {
					ild->highlight_styles->NullAll();
					ild->highlight_styles = null;
				}
				FREE (ild, Isoline_Data);
				set_isoline_data (null);
			}
			counts.flags &= ~PHF_ISOLINES_VALID;
		}

};



struct Polyhedron_No_Data {};

struct Polyhedron : public Polyhedron_Base<Polyhedron_No_Data> {
public:
	template <typename F>
	INLINE Point_3D<F> * Alloc_Points(int in_count) {}

	void invalidate_display_lists(Actor const * actor = null, bool need_locking = true) const;

private:
	Polyhedron();
	~Polyhedron();
	Polyhedron (Polyhedron const & that);
};

template <>
INLINE Point * Polyhedron::Alloc_Points<float>(int in_count)
{
	counts.points = in_count;
	ALLOC_ARRAY(single_data, counts.points, Point);
	return single_data;
}

template <>
INLINE DPoint * Polyhedron::Alloc_Points<double>(int in_count)
{
	counts.points = in_count;
	ALLOC_ARRAY(double_data, counts.points, DPoint);
	dbflags |= DB_DOUBLE_PRECISION;
	return double_data;
}


struct Mesh_Data {
	/* Other original [user specified, topological] data */
	int		rows;
	int		columns;

	/* Other accumulated and derived data */

	/*
	 * The edge attributes are stored, for each mesh vertex, as the edge
	 * to the right, upwards, and diagonally up and to the left. For
	 * convenience, space is provided for the attributes even when the
	 * edge itself is off the mesh.
	 */
};

struct Mesh : public Polyhedron_Base<Mesh_Data> {
public:

	Mesh() : Polyhedron_Base<Mesh_Data>(MESH) {

		ZALLOC(data, Mesh_Data);

	};

	~Mesh() {

		FREE(data, Mesh_Data);
	}

	Mesh (Mesh const & that) : Polyhedron_Base<Mesh_Data> (that) {

		ASSERT(that.data != null);

		ALLOC(data, Mesh_Data);
		data->rows = that.data->rows;
		data->columns = that.data->columns;
	}

	Geometry * clone() const {return NEW(Mesh)(*this);}

	size_t memory_used(MU_Flags flags) const {

		return Polyhedron_Base<Mesh_Data>::memory_used(flags);
	}

	void invalidate_display_lists(Actor const * actor = null, bool need_locking = true) const;

};



// two faces can define each edge; used by silhouette code.

struct Edge_To_Face {
	int					face1; /* face1 always indicates a real face */
	int					face2;
#	define				ETF_NOT_USED	(-1)
#	define				ETF_OVERUSED	(-2)	/* if more than 2 faces */
};

typedef Banked_Array<Edge_To_Face, CMO_Allocator<Edge_To_Face> > Edge_To_Face_Banked_Array; // XXX cached vs non-cached


struct Shell_Data {
	/* Other original [user specified, topological] data */
	int			face_list_length;
	int *		face_list;

	/* Other accumulated and derived data */

	/*
	 * Edge topology info is saved permanently because, unlike faces and points,
	 * the mapping from the "Open_Edge" info to the correct edge_attribute
	 * block is not simple.
	 *
	 * 'Edges' is a list, sorted by source vertex, of destination vertices.
	 * The source vertex is implied from the info in Vertex_To_Edge.
	 *
	 * 'Vertex_to_edge' maps each source vertex to the beginning of
	 * its collection of destination vertices within the 'edges'
	 * array. The number of such vertices is "vertex_to_edge[I+1] -
	 * vertex_to_edge[I]", so this array is dimensioned and
	 * maintained through "last"+1 for convenience.
	 *
	 * The struct is used in the hopes of adding some clarity.
	 *
	 * The creation of these arrays is deferred until either 
	 *  a) the first Open_Edge
	 *  b) the first time silhouettes are made visible.
	 * 
	 * Note that there is only one record for each connection.  For any given
	 * vertex 'i', you can look up vertices with index > i.  So if j>i and i->j 
	 * is a valid connection, i->j will exist between vertex_to_edge[i] and vertex_to_edge[i+1]
	 * but j->i will not be in the range vertex_to_edge[j] and vertex_to_edge[j+1]
	 */

	Destructible<int_Banked_Array>::unique_ptr	vertex_to_edge;	/* within 'destinations', size=point_count+1 */
	Destructible<int_Banked_Array>::unique_ptr	destinations;	/* size = edge_count */

	// The list of faces associated with each edge are used during silhouette-edge calculation.
	Destructible<Edge_To_Face_Banked_Array>::unique_ptr	edge_to_face;	/* size = edge_count */
};

struct Shell : public Polyhedron_Base<Shell_Data> {
public:
	Shell() : Polyhedron_Base<Shell_Data>(SHELL) {
		ZALLOC(data, Shell_Data);
	};

	~Shell() {

		if (!ANYBIT (counts.flags, PHF_SHELL_FACE_LIST_BY_REFERENCE)) {
			if (data->face_list_length != 0)
				FREE_ARRAY (data->face_list, data->face_list_length, int);
		}

		free_edges();

		FREE(data, Shell_Data);	
	}

	Shell (Shell const & that) : Polyhedron_Base<Shell_Data> (that) {

		ASSERT(that.data != null);

		ZALLOC(data, Shell_Data);

		if (ANYBIT (that.counts.flags, PHF_SHELL_FACE_LIST_BY_REFERENCE))
			data->face_list = that.data->face_list;
		else
			CLONE_ARRAY(that.data->face_list, that.data->face_list_length, int, data->face_list);

		data->face_list_length = that.data->face_list_length;

		if (that.data->vertex_to_edge != null)
			data->vertex_to_edge = Construct<int_Banked_Array>(*that.data->vertex_to_edge);

		if (that.data->destinations != null)
			data->destinations = Construct<int_Banked_Array>(*that.data->destinations);

		if (that.data->edge_to_face != null)
			data->edge_to_face = Construct<Edge_To_Face_Banked_Array>(*that.data->edge_to_face);
	}

	Geometry * clone() const {return NEW(Shell)(*this);}

	size_t memory_used(MU_Flags flags) const {

		size_t size = Polyhedron_Base<Shell_Data>::memory_used(flags);

		if (ANYBIT(flags, MU_DATABASE)) {

			if (!ANYBIT (counts.flags, PHF_SHELL_FACE_LIST_BY_REFERENCE)) {
				if (data->face_list_length != 0)
					size += data->face_list_length * sizeof(int);
			}
		}

		return size;
	}

	void free_edges (
		bool	free_edge_to_face=true,
		bool	free_vertex_to_edge=true) {

		ASSERT(data != null);

		if (free_edge_to_face)
			data->edge_to_face = null;

		if (free_vertex_to_edge) {
			data->vertex_to_edge = null;
			data->destinations = null;
		}
	}


	void invalidate_display_lists(Actor const * actor = null, bool need_locking = true) const;

};

////////////////////////////////////////////////////////////////////////////////


struct Polyhedron_Drawing_Data {
	DC_Point *			dc_points;
	float *				ws;
	Clip_Case *			clip_cases;

	int					point_count;

	int					points_within;

	Reference_Count		refcount;

	static Polyhedron_Drawing_Data * create () {
		Polyhedron_Drawing_Data *	pdd;
		ZALLOC (pdd, Polyhedron_Drawing_Data);
		INIT_UTILITY (pdd);
		return pdd;
	}

	void retain() {
		INCR_UTILITY(this);
	}

	void release() {
		if (DECR_UTILITY(this) == 0) {
			if (dc_points != null) {
				FREE_ARRAY (dc_points, point_count, DC_Point);
				FREE_ARRAY (ws, point_count, float);
				FREE_ARRAY (clip_cases, point_count, Clip_Case);
			}

			FREE (this, Polyhedron_Drawing_Data); //-V704
		}
	}
};


////////////////////////////////////////////////////////////////////////////////

struct UV_Edge {
	UV_Edge *		next;
	UV_Edge *		opposing;

	UV_Region *		region;

	int				index;
	float			priority;

	Plane2D			abc_line;
};


struct UV_Region {
	UV_Region *			next;
	UV_Edge *			edges;

	struct {
		Point2D		min;
		Point2D		max;
	} uv_bounds;			

	bool				cut;
};


struct UV_Region_Set {
	UV_Region *			regions;
	Point2D *			uv_points;		
	int					point_count;
	int					points_allocated;
};

#define UVF_KEEP (char)0x1
struct UV_Trim {
	UV_Trim *			next;

	DB_Type				type;
	char				flags;
	
	static void cleanup (UV_Trim *	node) {
		if (node->next != null)
			cleanup (node->next);
		HI_Delete_Trim (node);
	}

	static UV_Trim * clone (UV_Trim *	in);
};

struct UV_Trim_Poly : public UV_Trim {
	Point2D *			uv_points;		
	int					count;	  
};

struct UV_Trim_Curve : public UV_Trim {

	Point2D *			uv_control_points;
	float *				weights;
	float *				knots;

	int					degree;
	int					control_point_count;
};


struct UV_Trim_Collection : public UV_Trim {
	UV_Trim *			trimlist;
};



#define NSF_HAS_NEGATIVE_WEIGHTS		(NURBS_Surface_Flags)0x01
#define NSF_COPLANAR_CONTROL_POINTS		(NURBS_Surface_Flags)0x02

struct NURBS_Surface : public Geometry_X<Point_3D> {
public:
	Spinlock					spinlock;		// due to cached polyhedron

	// control_point size is (n_u_vertices * n_v_vertices). Varies faster in v
	float *						weights; 		// size is (n_u_vertices * n_v_vertices)
	float *						u_knots; 		// size is (u_degree + n_u_vertices + 1)
	float *						v_knots; 		// size is (v_degree + n_v_vertices + 1)
	UV_Trim *					trimlist;
	Polyhedron *				polyhedron;

	int							n_u_vertices;
	int							n_v_vertices;
	int							u_degree;
	int							v_degree;
	NURBS_Surface_Flags			surface_flags;

	int							checksum;		// determines if any of the settings has changed since polyhedron generation

	NURBS_Surface() : Geometry_X(NURBS_SURFACE) {}

	~NURBS_Surface() {

#ifndef DISABLE_NURBS_SURFACES
		int	u_knot_count = n_u_vertices + u_degree + 1;
		int	v_knot_count = n_v_vertices + v_degree + 1;
		int	control_point_count = n_u_vertices * n_v_vertices;

		if (single_data != null) {
			ASSERT(!ANYBIT(dbflags, DB_DOUBLE_PRECISION));
			FREE_ARRAY (single_data, control_point_count, Point);
		}
		if (double_data != null) {
			ASSERT(ANYBIT(dbflags, DB_DOUBLE_PRECISION));
			FREE_ARRAY (double_data, control_point_count, DPoint);
		}

		if (weights != null)
			FREE_ARRAY (weights, control_point_count, float);
		if (u_knots != null)
			FREE_ARRAY (u_knots, u_knot_count, float);
		if (v_knots != null)
			FREE_ARRAY (v_knots, v_knot_count, float);

		if (trimlist != null)
			UV_Trim::cleanup (trimlist);
		if (polyhedron != null)
			INUTILE  (polyhedron);
#endif
	}

	NURBS_Surface (NURBS_Surface const & that) : Geometry_X (that) {

#ifndef DISABLE_NURBS_SURFACES
		n_u_vertices = that.n_u_vertices;
		n_v_vertices = that.n_v_vertices;
		u_degree = that.u_degree;
		v_degree = that.v_degree;
		surface_flags = that.surface_flags;

		// maybe this could be copied or shared
		polyhedron = null;
		checksum = 0;
		
		int u_knot_count = that.u_degree + that.n_u_vertices + 1;
		int v_knot_count = that.v_degree + that.n_v_vertices + 1;

		CLONE_ARRAY(that.single_data, that.n_u_vertices * that.n_v_vertices, Point, single_data);
		CLONE_ARRAY(that.double_data, that.n_u_vertices * that.n_v_vertices, DPoint, double_data);
		CLONE_ARRAY(that.weights, that.n_u_vertices * that.n_v_vertices, float, weights);
		CLONE_ARRAY(that.u_knots, u_knot_count, float, u_knots);
		CLONE_ARRAY(that.v_knots, v_knot_count, float, v_knots);

		trimlist = UV_Trim::clone(that.trimlist);
#endif
	}

	Geometry * clone() const {return NEW(NURBS_Surface)(*this);}

	size_t memory_used(MU_Flags flags) const {

		size_t size = Geometry_X<Point_3D>::memory_used(flags);

		if (ANYBIT(flags, MU_DATABASE)) {

			size += sizeof(NURBS_Surface);

			int	u_knot_count = n_u_vertices + u_degree + 1;
			int	v_knot_count = n_v_vertices + v_degree + 1;
			int	control_point_count = n_u_vertices * n_v_vertices;

			if (single_data != null)
				size += control_point_count * sizeof(Point);
			if (double_data != null)
				size += control_point_count * sizeof(DPoint);

			if (weights != null)
				size += control_point_count * sizeof(float);
			if (u_knots != null)
				size += u_knot_count * sizeof(float);
			if (v_knots != null)
				size += v_knot_count * sizeof(float);

			// XXX
// 			if (trimlist != null)
// 				UV_Trim::cleanup (trimlist);
		}

		if (ANYBIT(flags, MU_DERIVED)) {

			if (polyhedron != null)
				size +=	polyhedron->memory_used(flags);
		}

		return size;
	}

	private:
		NURBS_Surface & operator= (NURBS_Surface const &);
};


#define MAX_EDGES_PER_POLYEDGE_CHUNK	128 /* the only independent variable here */
#define MAX_POINTS_PER_POLYEDGE_CHUNK	(MAX_EDGES_PER_POLYEDGE_CHUNK + 1)
#define MAX_INTS_PER_POLYEDGE_CHUNK		(MAX_POINTS_PER_POLYEDGE_CHUNK + MAX_EDGES_PER_POLYEDGE_CHUNK)

#define MAX_TRIANGLE_PAIRS_PER_TRISTRIP_CHUNK	64 /* the only independent variable here */
#define MAX_TRIANGLES_PER_TRISTRIP_CHUNK		(2 * MAX_TRIANGLE_PAIRS_PER_TRISTRIP_CHUNK)
#define MAX_POINTS_PER_TRISTRIP_CHUNK			(MAX_TRIANGLES_PER_TRISTRIP_CHUNK + 2)
#define MAX_INTS_PER_TRISTRIP_CHUNK				(MAX_POINTS_PER_TRISTRIP_CHUNK + MAX_TRIANGLES_PER_TRISTRIP_CHUNK)

End_HOOPS_Namespace

#define PHEDRON_DEFINED TRUE
#endif /* PHEDRON_DEFINED */
