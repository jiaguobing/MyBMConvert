// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef DATABASE_DEFINED
#define DATABASE_DEFINED

#include "hi_proto.h"

Begin_HOOPS_Namespace

typedef uint32_t	Attribute_Mask;

template <DB_Type type>
struct Lock_Mask {
	static const Attribute_Mask value = (1UL << (type - FIRST_LOCKABLE + 1));
private:
	void check_type() {
		static_assert(type >= FIRST_LOCKABLE && type <= LAST_LOCKABLE, "something is wrong here");
		static_assert(1+(LAST_LOCKABLE-FIRST_LOCKABLE+1) <= 32, "too many attributes in lockable range");
	}
};

template <>
struct Lock_Mask<STYLE> {
	static const Attribute_Mask value = 1;
};

template <>
struct Lock_Mask<CONDITIONS> {
	static const Attribute_Mask value = 0;
};

template <>
struct Lock_Mask<TEXTURE_DEFS> {
	static const Attribute_Mask value = 0;
};

template <>
struct Lock_Mask<GLYPH_DEFS> {
	static const Attribute_Mask value = 0;
};

template <>
struct Lock_Mask<LINE_STYLE_DEFS> {
	static const Attribute_Mask value = 0;
};

template <>
struct Lock_Mask<NAMED_STYLE_DEFS> {
	static const Attribute_Mask value = 0;
};

template <>
struct Lock_Mask<STREAMING_MODE> {
	static const Attribute_Mask value = 0;
};


INLINE Attribute_Mask 	Lock_Type_Mask (DB_Type type) {
	if (type == STYLE)
		return (Attribute_Mask)1UL;
	if (type < FIRST_LOCKABLE)
		return (Attribute_Mask)0UL;
	return (Attribute_Mask) (1UL << (type - FIRST_LOCKABLE + 1));
}

#define Lock_EVERYTHING	 ~(Lock_Mask<WINDOW>::value | Lock_Mask<ACTOR>::value | Lock_Mask<DRIVER_OPTIONS>::value)

///////////////////////////////////////////////////////////////////////////////////////

#define M_NONE					(Material_Components)0x0000
#define M_DIFFUSE_COLOR			(Material_Components)0x0001
#define M_SPECULAR_COLOR		(Material_Components)0x0002
#define M_MIRROR_COLOR			(Material_Components)0x0004
#define M_TRANSMISSION_COLOR	(Material_Components)0x0008
#define M_EMISSION_COLOR		(Material_Components)0x0010

#define M_GLOSS					(Material_Components)0x0020
#define M_INDEX					(Material_Components)0x0040

#define M_DIFFUSE_TEXTURE		(Material_Components)0x0100
#define M_SPECULAR_TEXTURE		(Material_Components)0x0200
#define M_MIRROR_TEXTURE		(Material_Components)0x0400
#define M_TRANSMISSION_TEXTURE	(Material_Components)0x0800
#define M_EMISSION_TEXTURE		(Material_Components)0x1000

#define M_BUMP_TEXTURE			(Material_Components)0x2000
#define M_ENVIRONMENT_TEXTURE	(Material_Components)0x4000

#define M_HAS_TEXTURES			(Material_Components)0x8000

#define M_DIFFUSE				(M_DIFFUSE_COLOR|M_DIFFUSE_TEXTURE)
#define M_SPECULAR				(M_SPECULAR_COLOR|M_SPECULAR_TEXTURE)
#define M_MIRROR				(M_MIRROR_COLOR|M_MIRROR_TEXTURE)
#define M_TRANSMISSION			(M_TRANSMISSION_COLOR|M_TRANSMISSION_TEXTURE)
#define M_EMISSION				(M_EMISSION_COLOR|M_EMISSION_TEXTURE)

#define M_ANY_TEXTURE	(M_DIFFUSE_TEXTURE|M_SPECULAR_TEXTURE|M_MIRROR_TEXTURE|M_TRANSMISSION_TEXTURE|M_EMISSION_TEXTURE|M_BUMP_TEXTURE|M_ENVIRONMENT_TEXTURE)

#define M_ANY_COLOR		(M_DIFFUSE_COLOR|M_SPECULAR_COLOR|M_MIRROR_COLOR|M_TRANSMISSION_COLOR|M_EMISSION_COLOR)

#define M_LOCKABLE		(M_ANY_TEXTURE|M_ANY_COLOR|M_GLOSS|M_INDEX)

#define Color_Type_FRONT				0
#define Color_Type_BACK					1
#define Color_Type_EDGE					2
#define Color_Type_LINE					3
#define Color_Type_VERTEX				4
#define Color_Type_MARKER				5
#define Color_Type_TEXT					6
#define Color_Type_CUT_FACE				7
#define Color_Type_CUT_EDGE				8
#define Color_Type_WINDOW				9
#define Color_Type_MATERIAL_COUNT		(Color_Type_WINDOW + 1)
// lights and contrast colors are not full materials
#define Color_Type_FRONT_CONTRAST		10
#define Color_Type_BACK_CONTRAST		11
#define Color_Type_EDGE_CONTRAST		12
#define Color_Type_LINE_CONTRAST		13
#define Color_Type_VERTEX_CONTRAST		14
#define Color_Type_MARKER_CONTRAST		15
#define Color_Type_TEXT_CONTRAST		16
#define Color_Type_CUT_FACE_CONTRAST	17
#define Color_Type_CUT_EDGE_CONTRAST	18
#define Color_Type_WINDOW_CONTRAST		19
//
#define Color_Type_LIGHTING				20
#define Color_Type_AMBIENT_UP			21
#define Color_Type_AMBIENT_DOWN			22
//
#define Color_Type_SIMPLE_REFLECTION	23 // unused but still in the parse
#define Color_Type_TOTAL_COUNT			(Color_Type_SIMPLE_REFLECTION + 1)
// keep last
#define Color_Type_UNKNOWN				31

/* need to match HIC_Color_Lock_Objects */

#define Color_NONE				(Color_Object)0x00000000
#define Color_UNKNOWN			(Color_Object)(1U<<Color_Type_UNKNOWN)

#define Color_FRONT				(Color_Object)(1U<<Color_Type_FRONT)
#define Color_BACK				(Color_Object)(1U<<Color_Type_BACK)
#define Color_EDGE				(Color_Object)(1U<<Color_Type_EDGE)
#define Color_LINE				(Color_Object)(1U<<Color_Type_LINE)
#define Color_VERTEX			(Color_Object)(1U<<Color_Type_VERTEX)
#define Color_MARKER			(Color_Object)(1U<<Color_Type_MARKER)
#define Color_TEXT				(Color_Object)(1U<<Color_Type_TEXT)
#define Color_CUT_FACE			(Color_Object)(1U<<Color_Type_CUT_FACE)
#define Color_CUT_EDGE			(Color_Object)(1U<<Color_Type_CUT_EDGE)
#define Color_WINDOW			(Color_Object)(1U<<Color_Type_WINDOW)
#define Color_FRONT_CONTRAST	(Color_Object)(1U<<Color_Type_FRONT_CONTRAST)
#define Color_BACK_CONTRAST		(Color_Object)(1U<<Color_Type_BACK_CONTRAST)
#define Color_EDGE_CONTRAST		(Color_Object)(1U<<Color_Type_EDGE_CONTRAST)
#define Color_LINE_CONTRAST		(Color_Object)(1U<<Color_Type_LINE_CONTRAST)
#define Color_VERTEX_CONTRAST	(Color_Object)(1U<<Color_Type_VERTEX_CONTRAST)
#define Color_MARKER_CONTRAST	(Color_Object)(1U<<Color_Type_MARKER_CONTRAST)
#define Color_TEXT_CONTRAST		(Color_Object)(1U<<Color_Type_TEXT_CONTRAST)
#define Color_CUT_FACE_CONTRAST	(Color_Object)(1U<<Color_Type_CUT_FACE_CONTRAST)
#define Color_CUT_EDGE_CONTRAST	(Color_Object)(1U<<Color_Type_CUT_EDGE_CONTRAST)
#define Color_WINDOW_CONTRAST	(Color_Object)(1U<<Color_Type_WINDOW_CONTRAST)
#define Color_LIGHTING			(Color_Object)(1U<<Color_Type_LIGHTING)
#define Color_AMBIENT_UP		(Color_Object)(1U<<Color_Type_AMBIENT_UP)
#define Color_AMBIENT_DOWN		(Color_Object)(1U<<Color_Type_AMBIENT_DOWN)
#define Color_SIMPLE_REFLECTION	(Color_Object)(1U<<Color_Type_SIMPLE_REFLECTION) // unused but still in the parse


#define Color_ANY_AMBIENT			(Color_AMBIENT_UP|Color_AMBIENT_DOWN)
#define Color_FACE					(Color_FRONT|Color_BACK)
#define Color_FACE_CONTRAST			(Color_FRONT_CONTRAST|Color_BACK_CONTRAST)
#define Color_POLYGON				(Color_FACE|Color_EDGE)
#define Color_GEOMETRY				(Color_POLYGON|Color_LINE|Color_MARKER|Color_VERTEX|Color_TEXT)
#define Color_CUT_GEOMETRY			(Color_CUT_FACE|Color_CUT_EDGE)
#define Color_CUT_GEOMETRY_CONTRAST	(Color_CUT_FACE_CONTRAST|Color_CUT_EDGE_CONTRAST)

#define Color_CONTRASTS		(Color_FACE_CONTRAST|Color_EDGE_CONTRAST|Color_LINE_CONTRAST|Color_MARKER_CONTRAST| \
								Color_VERTEX_CONTRAST|Color_TEXT_CONTRAST|Color_WINDOW_CONTRAST|Color_CUT_FACE_CONTRAST)

#define Color_EVERYTHING	(Color_WINDOW|Color_GEOMETRY|Color_CUT_GEOMETRY|Color_CONTRASTS|Color_LIGHTING|Color_ANY_AMBIENT)



#define BLOCK_OP(_target_, _source_, _insize_, _op_) {				\
	size_t					_size_ = _insize_;						\
	unsigned char *	_ap_ = (unsigned char *)_target_;				\
	unsigned char const *	_bp_ = (unsigned char const *)_source_;	\
	while (_size_ >= 4) {											\
	*(unsigned int *)_ap_ _op_ *(unsigned int const *)_bp_;			\
	_ap_ += 4;	_bp_ += 4;	_size_ -= 4;							\
	}																\
	while (_size_-- > 0) *_ap_++ _op_ *_bp_++;						\
}

/* equivalent of 'a |= b' for structures */
#define BLOCK_SET(_target_, _source_, _insize_) BLOCK_OP((_target_), (_source_), (_insize_), |=)

/* equivalent of 'a ^= b' for structures */
#define BLOCK_XOR(_target_, _source_, _insize_) BLOCK_OP((_target_), (_source_), (_insize_), ^=)

/* equivalent of 'a &= b' for structures */
#define BLOCK_MASK(_target_, _source_, _insize_) BLOCK_OP((_target_), (_source_), (_insize_), &=)

/* equivalent of 'a &= ~b' for structures */
#define BLOCK_CLEAR(_target_, _source_, _insize_) BLOCK_OP((_target_), (_source_), (_insize_), &= ~)



struct Attribute_Lock {
	Attribute_Mask			attributes;
	Color_Object			color;
	Material_Components		materials[Color_Type_MATERIAL_COUNT];
	Type_Flags				visibility;

	INLINE Material_Components pick_color_flags (Color_Object	object) const
	{
		ASSERT(LOWBIT(object) == object); // single only
		if (object < 1<<Color_Type_MATERIAL_COUNT) {
			for (uint32_t i=0; i<Color_Type_MATERIAL_COUNT; i++)
				if (object == 1U<<i)
					return materials[i];
		}
		else if (object < 1U<<Color_Type_TOTAL_COUNT)
			return M_LOCKABLE;		/* pretend lighting types are just 'on' */

		ASSERT(0);
		HE_DATA_ERROR("Can't choose color name");
		return 0;
	}

	

	INLINE bool locks_match (Color_Object	objects) const {

		Color_Object	first = LOWBIT (objects);
		uint32_t		i = 0;
		int				first_index = 0;

		for (i=0; i<Color_Type_MATERIAL_COUNT; i++) {
			if (first == 1U<<i) {
				first_index = i++;
				break;
			}
		}

		for (; i<Color_Type_MATERIAL_COUNT; i++) {
			if (ANYBIT (objects, 1U<<i)) {
				if (materials[i] != materials[first_index])
					return false;
			}
		}

		return true;
	}

};

struct Attribute_Lock_Set {
	Attribute_Lock		mask;
	Attribute_Lock		value;

	INLINE bool locks_match (Color_Object	objects) const {

		Color_Object	first = LOWBIT (objects);
		uint32_t		i = 0;
		int				first_index = 0;

		for (i=0; i<Color_Type_MATERIAL_COUNT; i++) {
			if (first == 1U<<i) {
				first_index = i++;
				break;
			}
		}

		for (; i<Color_Type_MATERIAL_COUNT; i++) {
			if (ANYBIT (objects, 1U<<i)) {
				if (mask.materials[i]  != mask.materials[first_index]  ||
					value.materials[i] != value.materials[first_index]) {
						return false;
				}
			}
		}

		return true;
	}
};

///////////////////////////////////////////////////////////////////////////////////////

static_assert(sizeof(Utility) == sizeof(uint32_t), "utility must be an uint32_t sized");

template <typename T>
local INLINE void UTILE (T const * ref) {
	Utility *	utility = (Utility *) &ref->utility;

	if (utility_valid (utility)) {
		if (ANYBIT (WORLD->system_flags, Sysop_THREADING))
			locked_increment (utility);
		else
			++*utility;
	}
}

template <typename T>
local INLINE void INUTILE (T const * ref) {
	Utility *	utility = (Utility *) &ref->utility;

	if (utility_valid (utility)) {
		if (ANYBIT (WORLD->system_flags, Sysop_THREADING)) {
			if (locked_decrement (utility) == 0)
				HI_Au_Revoir ((Anything *)ref);
		}
		else {
			if (--*utility == 0)
				HI_Au_Revoir ((Anything *)ref);
		}
	}
}


template<typename T>
struct INUTILE_Deleter
{
	void operator()(T * item) const
	{
		ASSERT(item != null);
		INUTILE (item);
	}
};

///////////////////////////////////////////////////

template <DB_Type DB_TYPE> struct DB_Type_Info {};

template <> struct DB_Type_Info<STYLE> {
	typedef Style attribute_type;
	typedef std::unique_ptr<attribute_type, INUTILE_Deleter<attribute_type>> unique_ptr_type;
};
template <> struct DB_Type_Info<CONDITIONS> {
	typedef Conditions attribute_type;
	typedef std::unique_ptr<attribute_type, INUTILE_Deleter<attribute_type>> unique_ptr_type;
};

template <> struct DB_Type_Info<TEXTURE_DEFS> {
	typedef Texture_Defs attribute_type;
	typedef std::unique_ptr<attribute_type, INUTILE_Deleter<attribute_type>> unique_ptr_type;
};

template <> struct DB_Type_Info<GLYPH_DEFS> {
	typedef Glyph_Defs attribute_type;
	typedef std::unique_ptr<attribute_type, INUTILE_Deleter<attribute_type>> unique_ptr_type;
};

template <> struct DB_Type_Info<LINE_STYLE_DEFS> {
	typedef Line_Style_Defs attribute_type;
	typedef std::unique_ptr<attribute_type, INUTILE_Deleter<attribute_type>> unique_ptr_type;
};

template <> struct DB_Type_Info<NAMED_STYLE_DEFS> {
	typedef Named_Style_Defs attribute_type;
	typedef std::unique_ptr<attribute_type, INUTILE_Deleter<attribute_type>> unique_ptr_type;
};

template <> struct DB_Type_Info<RENDERING_OPTIONS> {
	typedef Rendering_Options attribute_type;
	typedef std::unique_ptr<attribute_type, INUTILE_Deleter<attribute_type>> unique_ptr_type;
};

template <> struct DB_Type_Info<COLOR_MAP> {
	typedef Color_Map attribute_type;
	typedef std::unique_ptr<attribute_type, INUTILE_Deleter<attribute_type>> unique_ptr_type;
};

template <> struct DB_Type_Info<SELECTABILITY> {
	typedef Selectability attribute_type;
	typedef std::unique_ptr<attribute_type, INUTILE_Deleter<attribute_type>> unique_ptr_type;
};

template <> struct DB_Type_Info<VISIBILITY> {
	typedef Visibility attribute_type;
	typedef std::unique_ptr<attribute_type, INUTILE_Deleter<attribute_type>> unique_ptr_type;
};

template <> struct DB_Type_Info<COLOR> {
	typedef Color attribute_type;
	typedef std::unique_ptr<attribute_type, INUTILE_Deleter<attribute_type>> unique_ptr_type;
};

template <> struct DB_Type_Info<WINDOW_FRAME> {
	typedef Window_Frame attribute_type;
	typedef std::unique_ptr<attribute_type, INUTILE_Deleter<attribute_type>> unique_ptr_type;
};

template <> struct DB_Type_Info<WINDOW_PATTERN> {
	typedef Window_Pattern attribute_type;
	typedef std::unique_ptr<attribute_type, INUTILE_Deleter<attribute_type>> unique_ptr_type;
};

template <> struct DB_Type_Info<WINDOW> {
	typedef Window attribute_type;
	typedef std::unique_ptr<attribute_type, INUTILE_Deleter<attribute_type>> unique_ptr_type;
};

template <> struct DB_Type_Info<CAMERA> {
	typedef Camera attribute_type;
	typedef std::unique_ptr<Camera, INUTILE_Deleter<Camera>> unique_ptr_type;
};

template <> struct DB_Type_Info<HEURISTICS> {
	typedef Heuristics attribute_type;
	typedef std::unique_ptr<attribute_type, INUTILE_Deleter<attribute_type>> unique_ptr_type;
};

template <> struct DB_Type_Info<MODELLING_MATRIX> {
	typedef Modelling_Matrix attribute_type;
	typedef std::unique_ptr<attribute_type, INUTILE_Deleter<attribute_type>> unique_ptr_type;
};

template <> struct DB_Type_Info<IM_CALLBACK> {
	typedef Callback attribute_type;
	typedef std::unique_ptr<attribute_type, INUTILE_Deleter<attribute_type>> unique_ptr_type;
};

template <> struct DB_Type_Info<CLIP_REGION> {
	typedef Clip_Region attribute_type;
	typedef std::unique_ptr<attribute_type, INUTILE_Deleter<attribute_type>> unique_ptr_type;
};

template <> struct DB_Type_Info<ACTOR> {
	typedef Actor attribute_type;
	typedef std::unique_ptr<attribute_type, INUTILE_Deleter<attribute_type>> unique_ptr_type;
};

template <> struct DB_Type_Info<DRIVER_OPTIONS> {
	typedef Driver_Options attribute_type;
	typedef std::unique_ptr<attribute_type, INUTILE_Deleter<attribute_type>> unique_ptr_type;
};

template <> struct DB_Type_Info<EDGE_PATTERN> {
	typedef Edge_Pattern attribute_type;
	typedef std::unique_ptr<attribute_type, INUTILE_Deleter<attribute_type>> unique_ptr_type;
};

template <> struct DB_Type_Info<EDGE_WEIGHT> {
	typedef Edge_Weight attribute_type;
	typedef std::unique_ptr<attribute_type, INUTILE_Deleter<attribute_type>> unique_ptr_type;
};

template <> struct DB_Type_Info<FACE_PATTERN> {
	typedef Face_Pattern attribute_type;
	typedef std::unique_ptr<attribute_type, INUTILE_Deleter<attribute_type>> unique_ptr_type;
};

template <> struct DB_Type_Info<HANDEDNESS> {
	typedef Handedness attribute_type;
	typedef std::unique_ptr<attribute_type, INUTILE_Deleter<attribute_type>> unique_ptr_type;
};

template <> struct DB_Type_Info<LINE_PATTERN> {
	typedef Line_Pattern attribute_type;
	typedef std::unique_ptr<attribute_type, INUTILE_Deleter<attribute_type>> unique_ptr_type;
};

template <> struct DB_Type_Info<LINE_WEIGHT> {
	typedef Line_Weight attribute_type;
	typedef std::unique_ptr<attribute_type, INUTILE_Deleter<attribute_type>> unique_ptr_type;
};

template <> struct DB_Type_Info<MARKER_SIZE> {
	typedef Marker_Size attribute_type;
	typedef std::unique_ptr<attribute_type, INUTILE_Deleter<attribute_type>> unique_ptr_type;
};

template <> struct DB_Type_Info<MARKER_SYMBOL> {
	typedef Marker_Symbol attribute_type;
	typedef std::unique_ptr<attribute_type, INUTILE_Deleter<attribute_type>> unique_ptr_type;
};

template <> struct DB_Type_Info<TEXT_ALIGNMENT> {
	typedef Text_Alignment attribute_type;
	typedef std::unique_ptr<attribute_type, INUTILE_Deleter<attribute_type>> unique_ptr_type;
};

template <> struct DB_Type_Info<TEXT_FONT> {
	typedef Text_Font attribute_type;
	typedef std::unique_ptr<attribute_type, INUTILE_Deleter<attribute_type>> unique_ptr_type;
};

template <> struct DB_Type_Info<TEXT_PATH> {
	typedef Text_Path attribute_type;
	typedef std::unique_ptr<attribute_type, INUTILE_Deleter<attribute_type>> unique_ptr_type;
};

template <> struct DB_Type_Info<TEXT_SPACING> {
	typedef Text_Spacing attribute_type;
	typedef std::unique_ptr<attribute_type, INUTILE_Deleter<attribute_type>> unique_ptr_type;
};

template <> struct DB_Type_Info<USER_OPTIONS> {
	typedef User_Options attribute_type;
	typedef std::unique_ptr<attribute_type, INUTILE_Deleter<attribute_type>> unique_ptr_type;
};

template <> struct DB_Type_Info<USER_VALUE> {
	typedef User_Value attribute_type;
	typedef std::unique_ptr<attribute_type, INUTILE_Deleter<attribute_type>> unique_ptr_type;
};

template <> struct DB_Type_Info<TEXTURE_MATRIX> {
	typedef Texture_Matrix attribute_type;
	typedef std::unique_ptr<attribute_type, INUTILE_Deleter<attribute_type>> unique_ptr_type;
};

template <> struct DB_Type_Info<STREAMING_MODE> {
	typedef Streaming_Mode attribute_type;
	typedef std::unique_ptr<attribute_type, INUTILE_Deleter<attribute_type>> unique_ptr_type;
};


class Segments_Const {
public:

	INLINE Segments_Const() : _segments(null), _size(0) {}
	INLINE Segments_Const(Segment const * const * segments, size_t size) : _segments(segments), _size(size) {}
	INLINE Segments_Const(Null_Object const &) : _segments(null), _size(0) {}
	INLINE Segments_Const(Segments_Const const & that) : _segments(that._segments), _size(that._size) {}
	INLINE Segments_Const(Segment_Vector_Pooled const &	segments) : _segments(&segments[0]), _size(segments.size()) {}

	INLINE ~Segments_Const() {}

	INLINE Segments_Const const &	operator= (Segments_Const const & that) {
		_segments = that._segments;
		_size = that._size;
		return *this;
	}

	INLINE size_t size() const {return _size;}

	INLINE bool empty() const {return (_size==0);}

	INLINE Segment const * const & operator [] (size_t index) const {
		ASSERT(index<_size);
		return _segments[index];
	}

private:
	Segment const * const *	_segments;
	size_t					_size;

	bool operator == (Segments_Const const & that) const;
	bool operator != (Segments_Const const & that) const {return !(that == *this);}
};

template <DB_Type DB_TYPE>
inline typename DB_Type_Info<DB_TYPE>::unique_ptr_type Find_Net(
	Thread_Data *					thread_data,
	Segments_Const const &			segments,
	int								faf_flags=0,
	Find_Attribute_Control const *	control=null)
{
	ASSERT(!segments.empty());
	return typename DB_Type_Info<DB_TYPE>::unique_ptr_type((typename DB_Type_Info<DB_TYPE>::attribute_type *)
			HI_Find_Net_Attribute_By_Path (thread_data, (int)segments.size(), &segments[0], DB_TYPE, faf_flags, control));
}

template <DB_Type DB_TYPE, typename TARGET_TYPE>
inline typename DB_Type_Info<DB_TYPE>::unique_ptr_type Find(
	Thread_Data *			thread_data,
	TARGET_TYPE const *		target,
	bool					complain=false)
{
	return typename DB_Type_Info<DB_TYPE>::unique_ptr_type((typename DB_Type_Info<DB_TYPE>::attribute_type *)
		HI_Find_Attribute (thread_data, (Anything const *)target, DB_TYPE, complain));
}




/////////////////////////////////////////////////////////

struct HOOPS_API Volume : public Rectangle {
	float	hither;
	float	yon;

	HPS_INLINE bool Contains(Point const & contained) const
	{
		return Rectangle::Contains(contained) && (contained.z >= hither && contained.z <= yon);
	}
};


class Point_Interceptor {

public:
	Point_Interceptor() {}

	virtual ~Point_Interceptor() {}

	virtual	void	Merge (int c, Point const * p) {
		UNREFERENCED(c);
		UNREFERENCED(p);
		ASSERT(0);
	}

	virtual	void	Merge (int c, DPoint const * p) {
		UNREFERENCED(c);
		UNREFERENCED(p);
		ASSERT(0);
	}
};

#define Bounding_EMPTY					(Bounding_Flags)0x0001
#define Bounding_LOCKED					(Bounding_Flags)0x0002
#define Bounding_EXCLUDED				(Bounding_Flags)0x0004
#define Bounding_ANTIQUATED				(Bounding_Flags)0x0008		// may be worth recalculating
#define Bounding_TEXT					(Bounding_Flags)0x0010
#define Bounding_IMAGE					(Bounding_Flags)0x0020
#define Bounding_TRANSFORM_DEPENDENT	(Bounding_Flags)0x0040		// other than text/image
#define Bounding_OVERSIZED				(Bounding_Flags)0x0080		// double-precision version kept for range or other reason
#define Bounding_USER_CUBOID			(Bounding_Flags)0x0100
#define Bounding_BEST_VOLUME_SPHERE		(Bounding_Flags)0x0200
#define Bounding_BEST_VOLUME_CUBOID		(Bounding_Flags)0x0400
#define Bounding_SAW_DOUBLE_DATA		(Bounding_Flags)0x0800		// double-precision data was seen (not necessarily retained)
#define Bounding_LOCAL_NOTE				(Bounding_Flags)0x8000		// special ogl driver usage

struct Double_Bounding {
		DSimpleCuboid		cuboid;
		DSimpleSphere		sphere;
};

class Internal_Bounding : public Counted_Object_Interlocked {
	friend class Bounding;
	public:
		Double_Bounding *		oversized;

		SimpleCuboid			cuboid;
		SimpleSphere			sphere;

		float					density;
		mutable Bounding_Flags	flags;


		Internal_Bounding *	Clone () const {
			return NEW(Internal_Bounding)(*this);
		}

		bool				Merge (Bounding const & bounding);
		bool				Merge (int count, Point const * points);
		bool				Merge (SimpleCuboid const & cuboid);
		bool				Merge (SimpleSphere const & sphere);
		bool				Merge (int count, DPoint const * points, bool keep_double);
		bool				Merge (DSimpleCuboid const & cuboid, bool keep_double);
		bool				Merge (DSimpleSphere const & sphere, bool keep_double);

		bool				Try_Single ();

		Internal_Bounding *	Transform (Matrix const & matrix) const;

		float				Volume () const;

		bool	operator== (Internal_Bounding const & that) const {
			if (this->flags != that.flags)
				return false;

			if (this->density != that.density)
				return false;

			if (this->oversized != null && that.oversized != null) {
				if (this->oversized->cuboid != that.oversized->cuboid ||
					this->oversized->sphere != that.oversized->sphere)
					return false;
			}

			return (this->cuboid == that.cuboid && this->sphere == that.sphere);
		}

		bool	operator!= (Internal_Bounding const & that) const { return  !(*this == that); }

	protected:
		Internal_Bounding () : oversized (null), density (1.0f), flags (Bounding_EMPTY) {}
		Internal_Bounding (Internal_Bounding const & that)
			: oversized (null), cuboid (that.cuboid), sphere (that.sphere), density (that.density), flags (that.flags) {
			if (that.oversized) {
				ALLOC (oversized, Double_Bounding);
				*oversized = *that.oversized;
			}
		}

		~Internal_Bounding() {
			if (oversized != null)
				FREE (oversized, Double_Bounding);
		}

	private:
		Internal_Bounding const & operator= (Internal_Bounding const & that);
};

class Bounding : public Counted_Pointer<Internal_Bounding> {
	typedef		Counted_Pointer<Internal_Bounding>		CP;
	public:
		Bounding () : CP () {};	// needed because the next one is needed...
		// these don't inherit as needed due to the effective type of the result
		Bounding (Null_Object const &) : CP () {}
		INLINE Bounding const &	operator= (Null_Object const &) {
			CP::release();
			CP::counted_item = 0;
			return *this;
		}

		INLINE Bounding Copy () const {
			Bounding	temp;
			temp.counted_item = CP::counted_item->Clone();
			temp->retain();
			return temp;
		}
		INLINE Bounding & Modify () {
			if (CP::counted_item == null) {
				CP::counted_item = NEW(Internal_Bounding)();
				retain();
			}
			else if (CP::counted_item->shared()) {
				Internal_Bounding *	temp = CP::counted_item->Clone();
				release();
				CP::counted_item = temp;
				retain();
			}
			return *this;
		}

		bool	operator== (Bounding const & that) const {
			if (this->Empty() && that.Empty())
				return true;

			if (this->Empty() || that.Empty())
				return false;

			if (CP::counted_item == that.CP::counted_item)
				return true;

			return (*CP::counted_item == *that.CP::counted_item);
		}
		bool	operator!= (Bounding const & that) const { return  !(*this == that); }

		INLINE bool operator== (Null_Object const &) const { return counted_item == 0; }
		INLINE bool operator!= (Null_Object const &) const { return counted_item != 0; }


		INLINE bool Merge (Bounding const & bounding) {
			if (CP::counted_item == bounding.CP::counted_item)
				return true; // do nothing
			else if (CP::counted_item == null) {
				*this = bounding;
				return true;
			}
			else
				return Modify()->Merge (bounding);
		}

		INLINE bool Merge (int count, Point const * points) {
			return Modify()->Merge (count, points);
		}
		INLINE bool Merge (SimpleSphere const & sphere) {
			return Modify()->Merge (sphere);
		}
		INLINE bool Merge (SimpleCuboid const & cuboid) {
			return Modify()->Merge (cuboid);
		}

		INLINE bool Merge (int count, DPoint const * points, bool keep_double = false) {
			return Modify()->Merge (count, points, keep_double);
		}
		INLINE bool Merge (DSimpleSphere const & sphere, bool keep_double = false) {
			return Modify()->Merge (sphere, keep_double);
		}
		INLINE bool Merge (DSimpleCuboid const & cuboid, bool keep_double = false) {
			return Modify()->Merge (cuboid, keep_double);
		}

		INLINE Bounding Transform (Matrix const & matrix) const {
			ASSERT(CP::counted_item != null);
			Bounding	temp;
			temp.counted_item = CP::counted_item->Transform (matrix);
			temp->initialize(); // assuming Transform always return a new copy
			return temp;
		}

		INLINE bool Empty () const {
			return CP::counted_item == null || ANYBIT (CP::counted_item->flags, Bounding_EMPTY);
		}

		INLINE float Density () const {
			if (Empty())
				return 0.0f;
			else
				return CP::counted_item->density;
		}

		INLINE float Volume () const {
			if (Empty())
				return 0.0f;
			else
				return CP::counted_item->Volume();
		}

		static Bounding Create () {
			Bounding	temp;
			temp.counted_item = NEW(Internal_Bounding)();
			temp.initialize();
			return temp;
		}

		static Bounding Create (int count, Point const * points) {
			Bounding	temp;
			temp.counted_item = NEW(Internal_Bounding)();
			temp.initialize();
			if (count != 0)
				temp.Merge(count, points);
			return temp;
		}
		static Bounding Create (SimpleCuboid const & cuboid) {
			Bounding	temp;
			temp.counted_item = NEW(Internal_Bounding)();
			temp.initialize();
			temp.Merge(cuboid);
			return temp;
		}
		static Bounding Create (SimpleSphere const & sphere) {
			Bounding	temp;
			temp.counted_item = NEW(Internal_Bounding)();
			temp.initialize();
			temp.Merge(sphere);
			return temp;
		}

		static Bounding Create (int count, DPoint const * points, bool keep_double = false) {
			Bounding	temp;
			temp.counted_item = NEW(Internal_Bounding)();
			temp.initialize();
			if (count != 0)
				temp.Merge(count, points, keep_double);
			return temp;
		}
		static Bounding Create (DSimpleCuboid const & cuboid, bool keep_double = false) {
			Bounding	temp;
			temp.counted_item = NEW(Internal_Bounding)();
			temp.initialize();
			temp.Merge(cuboid, keep_double);
			return temp;
		}
		static Bounding Create (DSimpleSphere const & sphere, bool keep_double = false) {
			Bounding	temp;
			temp.counted_item = NEW(Internal_Bounding)();
			temp.initialize();
			temp.Merge(sphere, keep_double);
			return temp;
		}


		bool Show (Point * center, float * radius) {
			if (Empty()) {
				center->x = center->y = center->z = 0;
				*radius = -1;
				return false;
			}
			if (CP::counted_item->oversized != null) {
				*center = Point (CP::counted_item->oversized->sphere.center);
				*radius = (float)CP::counted_item->oversized->sphere.radius;
			}
			else {
				*center = CP::counted_item->sphere.center;
				*radius = CP::counted_item->sphere.radius;
			}
			return true;
		}

		bool Show (Point * min, Point * max) {
			if (Empty()) {
				min->x = min->y = min->z = 1;
				max->x = max->y = max->z = -1;
				return false;
			}
			if (CP::counted_item->oversized != null) {
				*min = Point (CP::counted_item->oversized->cuboid.min);
				*max = Point (CP::counted_item->oversized->cuboid.max);
			}
			else {
				*min = CP::counted_item->cuboid.min;
				*max = CP::counted_item->cuboid.max;
			}
			return true;
		}


		bool Show (DPoint * center, double * radius) {
			if (Empty()) {
				center->x = center->y = center->z = 0;
				*radius = -1;
				return false;
			}
			if (CP::counted_item->oversized != null) {
				*center = CP::counted_item->oversized->sphere.center;
				*radius = CP::counted_item->oversized->sphere.radius;
			}
			else {
				*center = DPoint (CP::counted_item->sphere.center);
				*radius = CP::counted_item->sphere.radius;
			}
			return true;
		}

		bool Show (DPoint * min, DPoint * max) {
			if (Empty()) {
				min->x = min->y = min->z = 1;
				max->x = max->y = max->z = -1;
				return false;
			}
			if (CP::counted_item->oversized != null) {
				*min = CP::counted_item->oversized->cuboid.min;
				*max = CP::counted_item->oversized->cuboid.max;
			}
			else {
				*min = DPoint (CP::counted_item->cuboid.min);
				*max = DPoint (CP::counted_item->cuboid.max);
			}
			return true;
		}


		INLINE Internal_Bounding const *	operator-> () const	{ return (Internal_Bounding const *)CP::counted_item; }
		INLINE Internal_Bounding *			operator-> ()		{ return (Internal_Bounding *)CP::counted_item; }
};

enum Activity_Bits : Activity {
	Act_NONE						= 0x00000000,
	Act_REDRAW_WINDOWS				= 0x00000001,
	Act_REDRAW_GEOMETRY				= 0x00000002,
	Act_REDRAW_WINDOW_FRAMES		= 0x00000004,
	Act_GEOMETRY_REDRAW_WITHIN		= 0x00000008,
	Act_INNER_REDRAW				= 0x00000010,
	Act_SCOPE_IS_VALID				= 0x00000020,
	Act_FULL_SCOPE_REDRAW			= 0x00000040,
	Act_ERASE						= 0x00000080,
	Act_REDRAW_ANYTHING				= 0x000000FF,

	Act_CAMERA_CHANGE				= 0x00000100,
	Act_HIGHLIGHTED					= 0x00000200,
	Act_MODIFIED					= 0x00000400,
	Act_CREATED						= 0x00000800,

	Act_INNER_CHANGES				= 0x00001000,
	Act_VALIDATE_REGION				= 0x00002000,
	Act_REFRESH						= 0x00004000,
	Act_REGENERATE_STATIC			= 0x00008000,

	Act_TEXT_BOUNDS_REGENERATED		= 0x00010000,	// only used during update
	Act_MUST_VISIT					= 0x00020000,	// only used during update
	Act_REFRESH_SUBWINDOWS			= 0x00040000,	// only used during update
	Act_HIGHLIGHT_CHANGES			= 0x00080000,	// only used during update

	Act_REGENERATE_CUT_GEOMETRY		= 0x00100000,
	Act_REGENERATE_ISOLINES			= 0x00200000,
	Act_REGENERATE_3D_TEXT_BOUNDS	= 0x00400000,
	Act_REGENERATE_HIDDEN_LINE		= 0x00800000,
	Act_REGENERATE_SIMPLE_SHADOW	= 0x01000000,
	Act_REPOSITION_SIMPLE_SHADOW	= 0x02000000,
	Act_SHADOW_POINT_CHANGE			= 0x04000000,
	Act_REGENERATE_SHADOW_MAP		= 0x08000000,

	Act_SPRITING					= 0x10000000,	// only used during update
	Act_OVERLAY						= 0x20000000,	// only used during update
	Act_INPLACE						= 0x40000000,	// only used during update
	Act_TREE_HIGHLIGHT				= 0x80000000,	// only used during update well in a static tree
	
	Act_REGENERATE_DERIVED			= (Act_REGENERATE_CUT_GEOMETRY|Act_REGENERATE_SIMPLE_SHADOW|Act_REGENERATE_SHADOW_MAP),
	
	Act_ANY_QUICKMOVED				= (Act_SPRITING|Act_OVERLAY|Act_INPLACE|Act_HIGHLIGHTED),
	
	Act_SET_INNER_CHANGES			= Act_INNER_CHANGES,
	Act_SET_INNER_REDRAW			= (Act_SET_INNER_CHANGES|Act_INNER_REDRAW),
	Act_SET_SCOPED_INNER_REDRAW		= (Act_SET_INNER_REDRAW|Act_SCOPE_IS_VALID|Act_FULL_SCOPE_REDRAW),
	Act_SET_GEOMETRY_REDRAW_WITHIN	= (Act_SET_SCOPED_INNER_REDRAW|Act_GEOMETRY_REDRAW_WITHIN|Act_VALIDATE_REGION),
	Act_SET_REDRAW_GEOMETRY			= (Act_SET_GEOMETRY_REDRAW_WITHIN|Act_REDRAW_GEOMETRY),
	Act_SET_REDRAW_WINDOWS			= (Act_SET_SCOPED_INNER_REDRAW|Act_REDRAW_WINDOWS),
	Act_SET_REDRAW_WINDOW_FRAMES	= (Act_SET_SCOPED_INNER_REDRAW|Act_REDRAW_WINDOW_FRAMES),
	Act_SET_REDRAW_FRAMED_WINDOWS	= (Act_SET_SCOPED_INNER_REDRAW|Act_REDRAW_WINDOW_FRAMES|Act_REDRAW_WINDOWS),
	Act_SET_REDRAW_EVERYTHING		= (Act_SET_REDRAW_GEOMETRY|Act_REDRAW_WINDOW_FRAMES|Act_REDRAW_WINDOWS),
	Act_SET_ERASE_GEOMETRY			= (Act_SET_REDRAW_GEOMETRY|Act_REDRAW_WINDOWS|Act_ERASE),
	Act_SET_ERASE_EVERYTHING		= (Act_SET_REDRAW_EVERYTHING|Act_ERASE),
	Act_SET_MODIFIED				= (Act_SET_INNER_CHANGES|Act_MODIFIED),
	Act_SET_CREATED					= (Act_SET_INNER_CHANGES|Act_CREATED),
	Act_SET_GEOMETRY_MODIFIED		= (Act_SET_REDRAW_GEOMETRY|Act_MODIFIED),
	Act_SET_GEOMETRY_CREATED		= (Act_SET_REDRAW_GEOMETRY|Act_CREATED),
	Act_SET_REFRESH					= (Act_SET_REDRAW_WINDOW_FRAMES|Act_REFRESH),
	Act_SET_REGENERATE_CUT_GEOMETRY = (Act_SET_INNER_REDRAW|Act_REGENERATE_CUT_GEOMETRY),
	Act_SET_REGENERATE_HIDDEN_LINE	= (Act_SET_INNER_REDRAW|Act_REGENERATE_HIDDEN_LINE),
	Act_SET_REGENERATE_SHADOW		= (Act_SET_INNER_REDRAW|Act_REGENERATE_SIMPLE_SHADOW),
	Act_SET_REGENERATE_DERIVED		= (Act_SET_INNER_REDRAW|Act_REGENERATE_DERIVED),
	Act_SET_SHADOW_POINT_CHANGE		= (Act_SET_INNER_REDRAW|Act_SHADOW_POINT_CHANGE),

	Act_SET_ERASE_LINE				= (Act_SET_ERASE_GEOMETRY|Act_REGENERATE_HIDDEN_LINE),
	Act_SET_ERASE_FACE				= (Act_SET_ERASE_GEOMETRY|Act_REGENERATE_HIDDEN_LINE|Act_REGENERATE_DERIVED),
	
	Act_SET_REDRAW_LINE				= (Act_SET_REDRAW_GEOMETRY|Act_REGENERATE_HIDDEN_LINE),
	Act_SET_REDRAW_FACE				= (Act_SET_REDRAW_GEOMETRY|Act_REGENERATE_HIDDEN_LINE|Act_REGENERATE_DERIVED),

};


#define PROPAGATE_ACTIVITY(_td_, _who_, _what_) HI_Propagate_Activity (_td_, (Segstuff *)(_who_), (_what_))

#define ERASE_G_ACTIVITY(_seg_) ((ANYBIT ((_seg_)->activity, Act_CREATED)) ? Act_SET_REDRAW_GEOMETRY : Act_SET_ERASE_GEOMETRY)
#define ERASE_G(_td_,_seg_,_x_) PROPAGATE_ACTIVITY ((_td_),(_seg_), ERASE_G_ACTIVITY((_seg_)) | (_x_))

#define ERASE_ALL_ACTIVITY(_seg_) ((ANYBIT ((_seg_)->activity, Act_CREATED)) ? Act_SET_REDRAW_EVERYTHING : Act_SET_ERASE_EVERYTHING)
#define ERASE_ALL(_td_,_seg_,_x_) PROPAGATE_ACTIVITY ((_td_),(_seg_), ERASE_ALL_ACTIVITY((_seg_)) | (_x_))


typedef int32_t	MU_Flags;

#define MU_NONE			(MU_Flags)0x00000000
#define MU_DATABASE		(MU_Flags)0x00000001
#define MU_DERIVED		(MU_Flags)0x00000002
#define MU_ATTRIBUTES	(MU_Flags)0x00000004
#define MU_GEOMETRY		(MU_Flags)0x00000010
#define MU_AMORTIZE		(MU_Flags)0x00000020


template <typename Base_Type, typename Owner_Type>
struct Database_Header : public Minimum_Header<Base_Type, Owner_Type> {
	public:
		Activity				activity;
		Activity				old_activity;
		Key						key;

		Database_Header<Base_Type, Owner_Type>()
			: Minimum_Header<Base_Type, Owner_Type>(), activity(0), old_activity(0), key(0) {
			;
		}

		Database_Header<Base_Type, Owner_Type>(Database_Header<Base_Type, Owner_Type> const & that)
			: Minimum_Header<Base_Type, Owner_Type>(that), activity(Act_CREATED), old_activity(0), key(ERROR_KEY)
		{
			;
		}

		virtual size_t memory_used(MU_Flags flags) const {UNREFERENCED(flags); ASSERT(0); return 0;}

		INLINE size_t sum_memory_used(MU_Flags flags) const {

			size_t	size = 0;

			Database_Header<Base_Type, Owner_Type> const * item = this;

			while (item != null) {
				size += item->memory_used(flags);
				item = item->next;
			}

			return size;
		}
};



struct Anything : public Database_Header<Anything, Anything> {};

struct HOOPS_API Attribute : public Database_Header<Attribute, Segment> {
public:

	Attribute() {};

	Attribute(DB_Type in_type) {
		type = in_type;
	};

	virtual int hash_key() const {ASSERT(0); return 0;};
	virtual Attribute * clone() const {ASSERT(0); return 0;}
	virtual bool equal(Attribute const * attr) const {UNREFERENCED(attr); ASSERT(0); return 0;};
	virtual Attribute * acquire (Thread_Data *	thread_data, Anything * item, Attribute const * old_attribute)
		{UNREFERENCED(thread_data); UNREFERENCED(item); UNREFERENCED(old_attribute); ASSERT(0); return 0;};
	virtual bool unset_acquire (Thread_Data *	thread_data, Anything * item, Attribute const * unset_proto)
		{UNREFERENCED(thread_data); UNREFERENCED(item); UNREFERENCED(unset_proto); ASSERT(0); return 0;};
	virtual Attribute * merge (Thread_Data * thread_data, Attribute const * attr, void * control=0)
		{UNREFERENCED(thread_data); UNREFERENCED(attr); UNREFERENCED(control); ASSERT(0); return 0;};

	static Attribute *generic_acquire (
		Thread_Data *	thread_data,
		Anything *				item,
		Attribute const *		old_attribute,
		Attribute *				new_attribute)
	{
		return new_attribute->acquire(thread_data, item, old_attribute);
	}

	static bool generic_unset_acquire (
		Thread_Data *	thread_data,
		Anything *				item,
		Attribute *				old_attribute,
		Attribute const *		unset_proto)
	{
		return old_attribute->unset_acquire(thread_data, item, unset_proto);
	}
};

struct HOOPS_API Geometry : public Database_Header<Geometry, Segment> {
public:
	Attribute *				attributes;
	int32_t					priority;
	int32_t					index;

	Geometry(DB_Type in_type) {
		type = in_type;
	};

	virtual Geometry * clone() const {ASSERT(0); return 0;}

	virtual void fixup() {}

	size_t memory_used(MU_Flags flags) const {
		size_t	size = 0;

		if (ANYBIT(flags, MU_ATTRIBUTES)) {
			if (attributes != null)
				attributes->sum_memory_used(flags);
		}

		return size;
	}

	virtual void invalidate_display_lists(Actor const * actor = null, bool need_locking = true) const {
		UNREFERENCED(actor);
		UNREFERENCED(need_locking);
	}

	void	Prefetch_Data () const;

	template <typename T>
	static INLINE T * copy (Thread_Data * thread_data, T const * old_geom, Segment * new_owner = null) {
#ifndef DISABLE_Copy_Geometry
		ASSERT(!(old_geom->type < FIRST_GEOMETRY || old_geom->type > LAST_GEOMETRY));

		T *	new_geom = (T*)old_geom->clone();

		new_geom->utility = 1;
		new_geom->key = POINTER_TO_KEY (new_geom);
		new_geom->owner = new_owner;
		new_geom->activity = Act_SET_CREATED;
		new_geom->old_activity = 0;
		new_geom->next = null;
		new_geom->backlink = &new_geom->next;

		if ((new_geom->attributes = HI_Clone_Attributes (thread_data, old_geom->attributes, (Segment const *)new_geom, true, null)) != null)
			new_geom->attributes->backlink = &new_geom->attributes;

		return new_geom;
#else
		static_assert(false, "someone should not have called this at all!");
		return null;
#endif
	}

protected:
	Geometry();
};


template <typename F> struct No_Data {};

template <template <typename> class FD>
struct Geometry_X : public Geometry {
public:
	Geometry_X() : Geometry() {};
	Geometry_X(DB_Type in_type) : Geometry(in_type) {};
	Geometry_X(Geometry_X const & that) : Geometry (that) {};

	size_t memory_used(MU_Flags flags) const {return Geometry::memory_used(flags);}

	FD<float> *				single_data;
	FD<double> *			double_data;

private:
	Geometry_X const & operator= (Geometry_X const & that);
};

INLINE void Geometry::Prefetch_Data () const {
	Geometry_X<No_Data> const *	g = (Geometry_X<No_Data> const *)this;
	Prefetch (g->single_data);
	Prefetch (g->double_data);
}



template <typename F, template <typename> class FD>
struct Precision_Helpers {
	static INLINE FD<F> *& precision_data (Geometry_X<FD> & geom);
	static INLINE FD<F> * const & precision_data (Geometry_X<FD> const & geom);
	static INLINE void update_precision_flag (Geometry_X<FD> & geom);
	static INLINE bool has_precision (Geometry_X<FD> const & geom);
};

template <template <typename> class FD>
struct Precision_Helpers<float, FD> {
	static INLINE FD<float> *& precision_data (Geometry_X<FD> & geom) {
		return geom.single_data;
	}
	static INLINE FD<float> * const & precision_data (Geometry_X<FD> const & geom) {
		return geom.single_data;
	}
	static INLINE void update_precision_flag (Geometry_X<FD> & geom) {
		geom.dbflags &= ~DB_DOUBLE_PRECISION;
	}
	static INLINE bool has_precision (Geometry_X<FD> const & geom) {
		return !ANYBIT (geom.dbflags, DB_DOUBLE_PRECISION);
	}
};

template <template <typename> class FD>
struct Precision_Helpers<double, FD> {
	static INLINE FD<double> *& precision_data (Geometry_X<FD> & geom) {
		return geom.double_data;
	}
	static INLINE FD<double> * const & precision_data (Geometry_X<FD> const & geom) {
		return geom.double_data;
	}
	static INLINE void update_precision_flag (Geometry_X<FD> & geom) {
		geom.dbflags |= DB_DOUBLE_PRECISION;
	}
	static INLINE bool has_precision (Geometry_X<FD> const & geom) {
		return ANYBIT (geom.dbflags, DB_DOUBLE_PRECISION);
	}
};

template <typename F, template <typename> class FD>
INLINE FD<F> *& precision_data (Geometry_X<FD> & geom) {
	return Precision_Helpers<F, FD>::precision_data(geom);
}

template <typename F, template <typename> class FD>
INLINE FD<F> *& precision_data (Geometry_X<FD> * geom) {
	return precision_data<F, FD>(*geom);
}

template <typename F, template <typename> class FD>
INLINE FD<F> * const & precision_data (Geometry_X<FD> const & geom) {
	return Precision_Helpers<F, FD>::precision_data(geom);
}

template <typename F, template <typename> class FD>
INLINE FD<F> * const & precision_data (Geometry_X<FD> const * geom) {
	return precision_data<F, FD>(*geom);
}

template <typename F, template <typename> class FD>
INLINE void update_precision_flag (Geometry_X<FD> & geom) {
	Precision_Helpers<F, FD>::update_precision_flag(geom);
}

template <typename F, template <typename> class FD>
INLINE void update_precision_flag (Geometry_X<FD> * geom) {
	update_precision_flag<F, FD>(*geom);
}

template <typename F, template <typename> class FD>
INLINE bool has_precision (Geometry_X<FD> const & geom) {
	return Precision_Helpers<F, FD>::has_precision(geom);
}

template <typename F, template <typename> class FD>
INLINE bool has_precision (Geometry_X<FD> const * geom) {
	return has_precision<F, FD>(*geom);
}




struct Segstuff : public Database_Header<Segstuff, Segment> {};


struct HOOPS_API Event : public Minimum_Header<Event, World> {
	public:
		Time		timestamp;
};


/* The following are used in segflags */

enum Seg_Flags_Bits : Seg_Flags {
	Seg_STREAMING					= 0x00000001,
	Seg_GEOMETRY_DISORDERED			= 0x00000002,
	Seg_SUBSEGMENTS_DISORDERED		= 0x00000004,
	Seg_HAS_WINDOW					= 0x00000008,
	Seg_HAS_ACTOR					= 0x00000010,
	Seg_HAS_DISPLAY_DRIVER			= 0x00000020,
	Seg_HAS_DRIVER_GENERATOR		= 0x00000040,
	Seg_BOUNDING_ANTIQUATED			= 0x00000080,
	Seg_ACTIVITY_CHANGE				= 0x00000100,
	Seg_HAS_ISOLATED_DRIVER			= 0x00000200,
	Seg_VISIBLE_WINDOWS				= 0x00000400,
	Seg_INVISIBLE_WINDOWS			= 0x00000800,
	Seg_TRANSPARENT_WINDOW			= 0x00001000,
	Seg_unused_1					= 0x00002000,
	Seg_unused_2					= 0x00004000,
	Seg_unused_3					= 0x00008000,
	Seg_SHADOW_POINT				= 0x00010000,
	Seg_HAS_QUICKMOVES				= 0x00020000,
	Seg_unused_9					= 0x00040000,
	Seg_HAS_CAMERA					= 0x00080000,
	Seg_DATABASE_CALLBACK			= 0x00100000,
	Seg_unused_5					= 0x00200000,
	Seg_SUBSEGMENT_PRIORITY			= 0x00400000,
	Seg_unused_6					= 0x00800000,
	Seg_HAS_LOCAL_EXPLICIT_VISIBLE	= 0x01000000,
	Seg_QUICKMOVES_AT_WINDOW		= 0x02000000,
	Seg_DO_NOT_CONDENSE				= 0x04000000, // used in static tree compile
	Seg_HPS_NO_OWNER				= 0x08000000, // HPS portfolio window or root
	Seg_Cached_NR					= 0x10000000,
	Seg_POPULATE					= 0x20000000,
	Seg_POPULATED					= 0x40000000,
	Seg_PROXY						= 0x80000000,
	Seg_DEAD_PROXY					= Seg_POPULATED, // reused
};


typedef char			Redraw_Flags;
enum Redraw_Flags_Bits : Redraw_Flags {
	R_WINDOWS		= 0x01,
	R_POLYGONS		= 0x02,
	R_LINES			= 0x04,
	R_IMAGES		= 0x08,
	R_MARKERS		= 0x10,
	R_TEXT			= 0x20,
	R_INSIDE		= 0x40,
	
	R_GEOMETRY		= (R_POLYGONS|R_LINES|R_IMAGES|R_MARKERS|R_TEXT),
	R_EVERYTHING	= (R_GEOMETRY|R_WINDOWS),
};




struct Conditional_Action;


// XXX refactor actions also the hash need to be invalidated

class Conditions : public Attribute {

public:
	enum : uint32_t {
		VALUE_BIT = 0x80000000,
		MASKS_MASK = ~VALUE_BIT,
	};

	enum Tristate {
		OFF = 0,
		ON,
		UNKNOWN
	};

	Conditional_Action *	actions;

	uint32_t_Vector			conditions;

	mutable int				_hash_key; // used by optimize

	Conditions() : Attribute(CONDITIONS), actions(null), _hash_key(0)
	{
		;
	}

	Conditions(Conditions const & that) : _hash_key(0)
	{
		conditions = that.conditions;
		actions = HI_Copy_Conditional_Actions(actions);
	}

	~Conditions() {
		conditions.clear();
		HI_Free_Conditional_Actions(actions);
	}

	bool operator == (Conditions const & that) const;

	Attribute * clone() const {
		Attribute * the_clone = NEW(Conditions)(*this);
		return the_clone;
	};

	bool equal(Attribute const * attr) const {
		if (attr->type != CONDITIONS)
			return false;
		return *this == *(Conditions*)attr;
	};

	// 	Attribute * merge(Thread_Data * thread_data, Attribute const * attr, void * control = 0);
	// 
	// 	Attribute * acquire(Thread_Data *	thread_data, Anything * item, Attribute const * old_attribute);

	int hash_key() const;

	INLINE Tristate State(uint32_t code) const {

		auto it = std::lower_bound(conditions.cbegin(), conditions.cend(), code, LessThan());

		if (it == conditions.cend() || (Conditions::MASKS_MASK & *it) != code)
			return UNKNOWN;
		if (ANYBIT(*it, VALUE_BIT))
			return ON;
		return OFF;
	}

	INLINE void Unset(uint32_t code) {

		auto it = std::lower_bound(conditions.begin(), conditions.end(), code, LessThan());

		if (it == conditions.end() || (Conditions::MASKS_MASK & *it) != code)
			return;

		conditions.erase(it);
	}

	INLINE bool Empty() const {
		return (conditions.empty() && actions == null);
	}

	static Conditions * Default(Thread_Data * thread_data);

	struct LessThan {
		INLINE bool operator()(const uint32_t & a, const uint32_t & b) const {
			return (Conditions::MASKS_MASK & a) < (Conditions::MASKS_MASK & b);
		}
	};

private:
	Conditions const & operator= (Conditions const & that);
};



typedef VXMap<uint32_t, uint32_t, Hasher<uint32_t>, std::equal_to<uint32_t>, POOL_Allocator<std::pair<const uint32_t, uint32_t>> > Condition_Map;

class Condition_Bits : public CMO_Pooled_Copyable {

public:

	Condition_Bits(Memory_Pool * pool) : CMO_Pooled_Copyable(pool), conditions(pool), conditions_refs(pool)
	{
		;
	}

	~Condition_Bits()
	{
		Clear();
	}

	Condition_Bits(Condition_Bits const & that) : CMO_Pooled_Copyable(that), conditions(memory_pool), conditions_refs(that.conditions_refs)
	{
		if (!conditions_refs.empty()) {
			for (auto cref : conditions_refs) {
				UTILE(cref);
			}
		}
	}

	INLINE Condition_Bits const &	operator= (Null_Object const &) {
		Clear();
		return *this;
	}

	INLINE Condition_Bits const &	operator= (Condition_Bits const & that) {
		Clear();
		if (!that.conditions_refs.empty()) {
			conditions_refs = that.conditions_refs;
			for (auto cref : conditions_refs) {
				UTILE(cref);
			}
		}
		return *this;
	}

	INLINE bool Empty() const {
		return (conditions.empty() && conditions_refs.empty());
	}

	INLINE void Clear() {

		if (!conditions_refs.empty()) {
			for (auto cref : conditions_refs) {
				INUTILE(cref);
			}
		}
		conditions.clear();
		conditions_refs.clear();
	}

	Conditions::Tristate State(uint32_t code) const;

	INLINE void Push_Conditions(Conditions const * cond)
	{
		UTILE(cond);
		conditions_refs.push_back(cond);
		conditions.clear();
	}

private:

	mutable Condition_Map	conditions;
	Conditions_List			conditions_refs;

};


// operators need to start at -1 and be contiguous for name tables
enum Condition_Codes {
	NothingPure = 0,
	// numerical comparisons
	Condition_LT = -1, Condition_FIRST_ARITHMETIC = Condition_LT,
	Condition_LE = -2,
	Condition_GE = -3,
	Condition_GT = -4, Condition_LAST_ARITHMETIC = Condition_GT,
	// numeric or logical
	Condition_EQ = -5,
	Condition_NEQ = -6,
	// logical only
	Condition_AND = -7,
	Condition_OR = -8,
	Condition_XOR = -9,
	Condition_NOT = -10,

	// explicit value
	Condition_VALUE = -11,
	// internal calculations
	Condition_FIRST_BUILTIN = -12
};

class Conditional : public CMO {
public:

	Conditional() : codes(0), values(0), count(0), value_count(0), builtin_count(0), purity(NothingPure) {};

	~Conditional() {
		Clear();
	}

	Conditional(Conditional const & that)
		: codes(0), values(0), count(that.count), value_count(that.value_count),
		builtin_count(that.builtin_count), purity(that.purity) {
		CLONE_ARRAY(that.codes, count, int, codes);
		CLONE_ARRAY(that.values, value_count, float, values);
	}

	Conditional(Conditional && that)
		: codes(that.codes), values(that.values), count(that.count), value_count(that.value_count),
		builtin_count(that.builtin_count), purity(that.purity) {
		that.count = 0;
		that.value_count = 0;
		that.builtin_count = 0;
		that.codes = null;
		that.values = null;
		that.purity = NothingPure;
	}

	void Clear() {
		if (count > 0)
			FREE_ARRAY(codes, count, int);
		if (value_count > 0)
			FREE_ARRAY(values, value_count, float);
		count = 0;
		value_count = 0;
		builtin_count = 0;
		purity = NothingPure;
	}

	INLINE bool Empty() const {
		return (count == 0);
	}

	INLINE bool HasBuiltIn() const {
		return (builtin_count > 0);
	}

	INLINE Condition_Codes Purity() const {
		return purity;
	}

	inline Conditional & operator= (Conditional const & that) {
		Clear();
		count = that.count;
		value_count = that.value_count;
		builtin_count = that.builtin_count;
		purity = that.purity;
		CLONE_ARRAY(that.codes, count, int, codes);
		CLONE_ARRAY(that.values, value_count, float, values);
		return *this;
	}

	inline Conditional & operator= (Null_Object const &) {
		Clear();
		return *this;
	}

	INLINE bool operator == (Conditional const & that) const {
		if (count != that.count || value_count != that.value_count || builtin_count != that.builtin_count ||
			!EQUAL_MEMORY(codes, count * sizeof(int), that.codes) ||
			(value_count > 0 && !EQUAL_MEMORY(values, value_count * sizeof(float), that.values)))
			return false;
		return true;
	};

	INLINE bool operator != (Conditional const & that) const {
		return !(*this == that);
	};

	void Merge(Conditional const & that, Condition_Codes join_code) {

		ASSERT(!Empty());

		if (!that.Empty()) {

			if (count + that.count == 2 && (join_code == Condition_AND || join_code == Condition_OR))
				purity = join_code;
			else if ((join_code != purity && count > 1) || (join_code != that.purity && that.count > 1))
				purity = NothingPure;

			int			old_count = count;
			int const *	old_codes = codes;

			count += that.count + 1;
			ALLOC_ARRAY(codes, count, int);
			COPY_ARRAY(old_codes, old_count, int, codes);
			COPY_ARRAY(that.codes, that.count, int, codes + old_count);
			FREE_ARRAY(old_codes, old_count, int);

			codes[count - 1] = join_code;
		}

	}

	Conditions::Tristate resolve(Condition_Bits const & condition_bits) const;

	int hash_key() const {

		int hash_key = count + (value_count << 8) + (builtin_count << 16);

		for (int i = 0; i < count; ++i)
			hash_key += codes[i];

		for (int i = 0; i < value_count; ++i)
			hash_key += (int)(values[i] * 10000.0);

		return hash_key;
	}

	INLINE void ValidatePurity() {

		purity = NothingPure;

		if (value_count + builtin_count > 0)
			return;

		for (int i = 0; i < count; ++i) {
			if (codes[i] < 0) {
				if (codes[i] == Condition_AND || codes[i] == Condition_OR) {
					if (purity == NothingPure)
						purity = (Condition_Codes)codes[i];
					else if (purity != codes[i]) {
						purity = NothingPure;
						return;
					}
				}
				else {
					purity = NothingPure;
					return;
				}
			}
		}
	}

public:
	int *			codes;
	float *			values;
	int				count;
	int				value_count;
	int				builtin_count;
	Condition_Codes	purity;
};


#define CAF_SEGMENT_TREE	0x0001
#define CAF_MERGE			0x8000

#define CAF_TYPE_PRUNE_SEGMENT	1

struct Conditional_Action {
	Conditional_Action *	next;
	Conditional				conditional;
	short					type;
	short					flags;
};




INLINE int32_t	Count_Bits (int32_t	v) {
	int32_t c;
#if 0
	v = v - ((v >> 1) & 0x55555555);
	v = (v & 0x33333333) + ((v >>2) & 0x33333333);
	c = ((v + (v >> 4) & 0x0F0F0F0F) * 0x1010101) >> 24;
#else
	c = 0;
	while (v != 0) {
		++c;
		v &= v - 1;
	}
#endif
	return c;
}

struct Pointer_Cache : public CMO {
	protected:
		void **		pointers;
		int32_t		size;
		int32_t		present;

		void			expand (int32_t index) {
							void **		oldp = pointers;
							ALLOC_ARRAY (pointers, size+1, void *);
							if (index > 0)
								COPY_ARRAY (oldp, index, void *, pointers);
							if (index < size)
								COPY_ARRAY (&oldp[index], size-index, void *, &pointers[index+1]);
							if (size > 0)
								FREE_ARRAY (oldp, size, void *);
							++size;
						}

		void			contract (int32_t index) {
							void **		oldp = pointers;
							if (--size > 0) {
								ALLOC_ARRAY (pointers, size, void *);
								if (index > 0)
									COPY_ARRAY (oldp, index, void *, pointers);
								if (index < size)
									COPY_ARRAY (&oldp[index+1], size-index, void *, &pointers[index]);
							}
							else
								pointers = null;
							FREE_ARRAY (oldp, size+1, void *);
						}

	public:
		Pointer_Cache () : pointers (null), size (0), present (0) {}
		~Pointer_Cache () {
			if (size > 0)
				FREE_ARRAY (pointers, size, void *);
		}

		INLINE bool		has (int32_t id) const {
							int32_t			bit = 1 << id;
							if (ANYBIT (present, bit))
								return true;
							return false;
						}

		INLINE void *	get (int32_t id) const {
							int32_t			bit = 1 << id;
							if (!ANYBIT (present, bit))
								return null;
							int32_t			index = Count_Bits (present & (bit-1));
							void *			result = pointers[index];
							Prefetch (result);
							return result;
						}

		void			set (int32_t id, void * pointer) {
							int32_t			bit = 1 << id;
							int32_t			index = Count_Bits (present & (bit-1));
							if (pointer != null) {
								if (!ANYBIT (present, bit)) {
									expand (index);
									present |= bit;
								}
								pointers[index] = pointer;
							}
							else {
								if (ANYBIT (present, bit)) {
									contract (index);
									present &= ~bit;
								}
							}
						}

		INLINE bool		empty () const { return present == 0; }
};



#define GT_INDEX(x)		(int32_t)((x)-FIRST_GEOMETRY)		// zero-based index from a geometry Type
#define GT_MASK(x)		(1 << GT_INDEX(x))					// single mask bit from a Type
#define GT_OO			(LAST_GEOMETRY-FIRST_GEOMETRY+1)	// Out of Order geometry table index

static_assert(GT_OO < 32, "too many geometries for geo table bitfield");


struct Geometry_Table : public Pointer_Cache {
	public:
		// for filtering, return the mask
		int					get_types () const { return present; }

		// simplify type usage
		Geometry *			get (int id) const { return (Geometry *)Pointer_Cache::get (id); }

		// functions for looking at "list"
		Geometry *			get_first () const {
								if (pointers == null)
									return null;
								return (Geometry *)pointers[0];
							}
		Geometry *			get_last () const {
								if (pointers == null)
									return null;
								return (Geometry *)pointers[size-1];
							}
		INLINE Geometry *	get_next (int id) const {
								int			bit = 1 << id;
								if (!ANYBIT (present, bit))
									return null;
								int			index = Count_Bits (present & (bit-1));
								if (++index < size)
									return null;
								return (Geometry *)pointers[index];
							}
		INLINE Geometry *	get_prev (int id) const {
								int			bit = 1 << id;
								if (!ANYBIT (present, bit))
									return null;
								int			index = Count_Bits (present & (bit-1));
								if (--index < 0)
									return null;
								return (Geometry *)pointers[index];
							}
};


#define LOD_MAX_LEVELS 8
struct LOD_Node : public Anything {
	Geometry *	data[LOD_MAX_LEVELS];

#ifndef DISABLE_Copy_Geometry
	local LOD_Node * copy (
		Thread_Data *		thread_data,
		LOD_Node const *	in) {

		LOD_Node *	out;
		ZALLOC (out, LOD_Node);

		out->type = LOD_NODE;
		out->utility = 1;

		for (int i = 0; i < LOD_MAX_LEVELS; i++) {
			if (in->data[i] != null)
				out->data[i] = Geometry::copy(thread_data, in->data[i]);
		}

		return out;
	}
#endif
};


struct Subsegment : public Database_Header<Subsegment, Segment> {
	int32_t						priority;
};


enum IP_Flags : uint32_t {
	None		= 0x00000000,
	Complex		= 0x00000001,
};

template<> struct _bit_ops_type < IP_Flags > {
	typedef IP_Flags type;
};

struct Internal_Include_Path : public Counted_Object_Pooled {
	friend class Counted_Pointer_Pooled<Internal_Include_Path>;

	private:
		HOOPS_API Internal_Include_Path (Memory_Pool *p);

	public:

		Include const **	jumps;
		static const size_t _jb_byte_size = 128 - sizeof(Counted_Object_Pooled) -
			sizeof(Include const **) - sizeof(intptr_t) - sizeof(int) - sizeof(IP_Flags);
		static const size_t _jb_size = _jb_byte_size / sizeof(Include const *);
		Include const *		_jb[_jb_size];

		intptr_t			hash_key;
		int					count;
		IP_Flags			flags;

		~Internal_Include_Path();
};

class Include_Path : public Counted_Pointer_Pooled<Internal_Include_Path> {
	typedef		Counted_Pointer_Pooled<Internal_Include_Path>		CP;
	public:
		Include_Path () : CP () {};	// needed because the next one is needed...
		// these don't inherit as needed due to the effective type of the result
		Include_Path (Null_Object const &) : CP () {}

		Include_Path(size_t count, Include const * const * includes, Display_Context const * dc = null);

		INLINE Include_Path const &	operator= (Null_Object const &) {
			CP::release();
			CP::counted_item = 0;
			return *this;
		}

		bool operator!= (Null_Object const &) const	{ return  !(*this == Include_Path (null)); }
		bool operator== (Null_Object const &) const	{ return  (*this == Include_Path (null)); }

		Include_Path (Counted_Pointer_Pooled<Internal_Include_Path> const & that) : CP (that) {};

		bool operator== (Include_Path const & that) const {
			if (CP::counted_item == that.counted_item)	// same structure or both null
				return true;
			if (CP::counted_item == null || that.counted_item == null)
				return false;
			Internal_Include_Path const &	a = *CP::counted_item;
			Internal_Include_Path const &	b = *that.counted_item;
			if (a.count != b.count || a.hash_key != b.hash_key)
				return false;
			for (int i = 0; i < a.count; ++i)
				if (a.jumps[i] != b.jumps[i])
					return false;
			return true;
		}
		bool operator!= (Include_Path const & that) const	{ return  !(*this == that); }

		void Expand (Include const * jump, Display_Context const * dc = null);
		void Expand (Include_Path const & path, Display_Context const * dc = null);

		intptr_t hash_key() const {
			if (CP::counted_item == null)
				return 0;
			return CP::counted_item->hash_key;
		}
};




struct Segment_Cache;


struct Geo_Index_Table : public CMO {
	Geo_Index_Table (int size) {
		table_size = size;
		ZALLOC_ARRAY (table, table_size, Geometry const *);
		update_generation = HOOPS::WORLD->update_generation;
	}
	~Geo_Index_Table () {
		FREE_ARRAY (table, table_size, Geometry const *);
	}

	uint32_t			update_generation;
	int					table_size;
	Geometry const **	table;
};


struct Associated_Segs;

struct Seg_Geo : public CMO {
		Geometry *			geometry;
		Geometry_Table *	geometry_table;
		Bounding			geometry_bounding;	// local geometry only
		int32_t				max_geometry_index;
	
		Seg_Geo() {}

		~Seg_Geo() {
			if (geometry != null)
				HI_Bon_Voyage ((Anything *)geometry);
			delete geometry_table;
		}

	private:
		void	Remove(Geometry const * g);
		bool	Insert(Geometry const * g);
		void	clear(); // caller must delete this


	friend struct Segment;
};


// blow past specified geometry types in geometry list,
INLINE bool SKIP(Geometry const *& g, DB_Type type_to_skip, Seg_Geo const * seg_geo)
{
	if (g != null && g->type == type_to_skip) {
		int32_t table_type = GT_INDEX(type_to_skip);
		if (g->next == null) {
			g = null;
		}
		else if (seg_geo == null || seg_geo->geometry_table == null || ANYBIT (g->dbflags, DB_OUT_OF_ORDER)) {
			do {
				if ((g = g->next) == null)
					break;
			} while (g->type == type_to_skip);
		}
		else {
			for (;;) {
				table_type++;
				if (table_type > GT_OO) {
					g = null;
					break;
				}
				if (seg_geo->geometry_table->get (table_type) != null) {
					g = seg_geo->geometry_table->get (table_type);
					break;
				}
			}
		}
		return true;
	}
	return false;
}

// Variant that breaks (Note: can't use do {} while (0) as that would block the required breaks
#define SKIP_ALL_BREAK(geo_list, type_to_skip) { \
	if (geo_list != null) { \
		Seg_Geo const * seg_geo = null; \
		if (geo_list->owner != null) \
			seg_geo = geo_list->owner->get_geo(); \
		if (SKIP(geo_list, type_to_skip, seg_geo)) \
			break; \
	} \
}

#define SKIP_ALL_NO_BREAK(geo_list, type_to_skip) do { \
	SKIP_ALL_BREAK(geo_list, type_to_skip); \
} while (0)


struct Extended_Segment_Data : public Pointer_Cache {
	void cleanup (Segment const * owner);
	HOOPS_API ~Extended_Segment_Data ();

	protected:
		enum {
			Extended_Seg_Geo,
			Extended_Fast_Lookup,
			Extended_References,
			Extended_Generation,
			Extended_Named_Style_Count,
			Extended_LOD,
			Extended_Geo_Index_Table,
			Extended_Associated_Segs,
			Extended_Max_Godl_Index
		};

		struct Reference_Cache : public CMO {
			Xref *				references;
		};

		struct LOD_Cache : public CMO {
			LOD_Node *			lodbase;
		};


	public:
		INLINE void			set_seg_geo (Seg_Geo * seg_geo) {
								set (Extended_Seg_Geo, seg_geo);
							}
		INLINE Seg_Geo *	get_seg_geo () const {
								return (Seg_Geo *)get (Extended_Seg_Geo);
							}

		INLINE	bool		has_seg_geo () const {
								return has(Extended_Seg_Geo);
							}

		void					set_fast_lookup (Fast_Lookup * fl) {
									set (Extended_Fast_Lookup, fl);
								}
		INLINE	Fast_Lookup *	get_fast_lookup () const {
									return (Fast_Lookup *)get (Extended_Fast_Lookup);
								}

		void				set_references (Xref * x);	// after Xref definition
		Xref *				get_references () const {
								Reference_Cache *	rc = (Reference_Cache *)get (Extended_References);
								if (rc == null)
									return null;
								return rc->references;
							}

		void				set_lodbase (LOD_Node * ln) {
								LOD_Cache *			lc = (LOD_Cache *)get (Extended_LOD);
								if (ln != null) {
									if (lc == null) {
										lc = NEW(LOD_Cache)();
										set (Extended_LOD, lc);
									}
									lc->lodbase = ln;
									ln->backlink = (Anything **)&lc->lodbase;
								}
								else {
									delete lc;
									set (Extended_LOD, null);
								}
							}
		LOD_Node *			get_lodbase () const {
								LOD_Cache *		lc = (LOD_Cache *)get (Extended_LOD);
								if (lc == null)
									return null;
								return lc->lodbase;
							}

		void				set_geometry_index_table (Geo_Index_Table * git) {
								set (Extended_Geo_Index_Table, git);
							}
		Geo_Index_Table *	get_geometry_index_table () const {
								return (Geo_Index_Table *)get (Extended_Geo_Index_Table);
							}

		void				set_update_generation (uint32_t value) {
								set (Extended_Generation, (void *)(intptr_t)value);
							}
		INLINE uint32_t		get_update_generation () const {
								return (uint32_t)(intptr_t)get (Extended_Generation);
							}

		void						set_associated_segments (Associated_Segs * as) {
										set (Extended_Associated_Segs, as);
									}
		INLINE Associated_Segs *	get_associated_segments () const {
										return (Associated_Segs *)get (Extended_Associated_Segs);
									}

		void				set_max_godl_index (uint32_t i) {
								set (Extended_Max_Godl_Index, (void *)(intptr_t)i);
							}
		INLINE uint32_t		get_max_godl_index () const {
								return (uint32_t)(intptr_t)get (Extended_Max_Godl_Index);
							}

		void				set_named_style_count (int cc) {
								set (Extended_Named_Style_Count, (void *)(intptr_t)cc);
							}
		INLINE int			get_named_style_count () const {
								return (int)(intptr_t)get (Extended_Named_Style_Count);
							}
		INLINE	bool		has_named_style_count () const {
								return has(Extended_Named_Style_Count);
							}
};


#define Segment_STATIC_TREE		DB_TYPE_SPECIFIC(1)
#define Segment_HIGHLIGHT		DB_TYPE_SPECIFIC(2)

struct Segment : public Subsegment {
	Subsegment *				subsegments;
	Attribute *					attributes;
	Segment_Cache *				cache;
	Extended_Segment_Data		extended;
	Seg_Flags					segflags;
	Seen_Flags					maybes;
	Bounding					bounding;			// includes subsegments
	Spinlock					spinlock;			// due to cache
	Name						name;

#ifdef HOOPS_DEBUG_BUILD
	Seg_Geo *					sg;
	uint32_t					id;
#endif

	// access to segment geometry
	void				clear_geometry() {
							Seg_Geo * seg_geo = get_geo();
							ASSERT(seg_geo != null);
							seg_geo->clear();
							delete seg_geo;
							set_geo (null);
						}

	void				remove(Geometry const * g) {
							ASSERT(g->owner == this);
							Seg_Geo * seg_geo = get_geo();
							ASSERT(seg_geo != null);
							seg_geo->Remove(g);
							if (seg_geo->geometry == null &&
								!ANYBIT (dbflags, Segment_STATIC_TREE)) {
								delete seg_geo;
								set_geo (null);
							}
						}

	bool				insert(Geometry const * g) {
							ASSERT(g->owner == this);
							Seg_Geo * seg_geo = ensure_geo();
							return seg_geo->Insert(g);
						}

	void				set_geo (Seg_Geo *	seg_geo) {
							extended.set_seg_geo(seg_geo);
#ifdef HOOPS_DEBUG_BUILD
							sg = seg_geo;
#endif
						}

	Seg_Geo *			ensure_geo () {
							Seg_Geo *	seg_geo = get_geo();
							if (seg_geo == null) {
								seg_geo = NEW(Seg_Geo)();
								set_geo(seg_geo);
							}
							return seg_geo;
						}

	INLINE	Seg_Geo *	get_geo () const {
							return extended.get_seg_geo();
						}
	INLINE	bool		has_geo() const {
							return extended.has_seg_geo();
						}

	// access to references
	void				set_references (Xref * x) {
							extended.set_references(x);
						}
	INLINE	Xref *		get_references () const {
							return extended.get_references();
						}

	// access to fast lookup table
	void					set_fast_lookup (Fast_Lookup * fl) {
								extended.set_fast_lookup(fl);
							}
	INLINE	Fast_Lookup *	get_fast_lookup () const {
								return extended.get_fast_lookup ();
							}

	// access to geometry index table
	void					set_geometry_index_table (Geo_Index_Table *	git) {
								extended.set_geometry_index_table (git);
							}
	Geo_Index_Table *		get_geometry_index_table () const {
								Geo_Index_Table *	git = extended.get_geometry_index_table();

								if (git != null &&
									git->update_generation < get_update_generation()) {
									delete git;
									git = null;
								}

								if (git == null) {
									Seg_Geo *	seg_geo = get_geo();
									if (seg_geo != null) {
										git = NEW(Geo_Index_Table) (seg_geo->max_geometry_index + 1);
										Geometry const *	g = seg_geo->geometry;
										while (g) {
											git->table[g->index] = g;
											g = g->next;
										}
										Alter(extended).set_geometry_index_table (git);
									}
								}
								return git;
							}

	// access to lod storage
	void					set_lodbase (LOD_Node * ln) {
								extended.set_lodbase (ln);
							}
	INLINE	LOD_Node *		get_lodbase () const {
								return extended.get_lodbase();
							}

	// access to update generation (driver segments)
	void					set_update_generation (uint32_t gen) {
								extended.set_update_generation (gen);
							}
	INLINE	uint32_t		get_update_generation () const {
								return extended.get_update_generation();
							}

	// access to fast lookup table
	void						set_associated_segments (Associated_Segs * as) {
									extended.set_associated_segments(as);
								}
	INLINE	Associated_Segs *	get_associated_segments () const {
									return extended.get_associated_segments ();
								}

	void				set_named_style_count (int cc) {
							extended.set_named_style_count(cc);
						}
	INLINE	int			get_named_style_count () const {
							return extended.get_named_style_count ();
						}
	INLINE	bool		has_named_style_count () const {
							return extended.has_named_style_count();
						}

	INLINE	bool		has_functional_window(Attribute_Lock const * filter = null) const {
							if (filter == null)
								return ANYBIT (segflags, Seg_HAS_WINDOW | Seg_HAS_ACTOR);
							if ((ANYBIT (segflags, Seg_HAS_WINDOW) && !ANYBIT (filter->attributes, Lock_Mask<WINDOW>::value)) ||
								(ANYBIT (segflags, Seg_HAS_ACTOR) && !ANYBIT (filter->attributes, Lock_Mask<ACTOR>::value)))
								return true;
							return false;
						}

	void				set_max_godl_index (uint32_t i) {
							extended.set_max_godl_index(i);
						}
	INLINE uint32_t		get_max_godl_index () const {
							return extended.get_max_godl_index ();
						}
};



struct Associated_Segs : CMO {

private:

		Segment_Vector_Pooled segs;

public:
		Associated_Segs(size_t	count) : segs(DEFAULT_MEMORY_POOL()) {
			segs.reserve(count);
		}

		~Associated_Segs() {
			;
		}

		INLINE Segment_Vector_Pooled const & get_segs() const {
			return segs;
		}

		INLINE void hook_one(Segment const * target_seg, Segment const * source_seg) {

			segs.push_back(target_seg);

			Associated_Segs *	as = target_seg->get_associated_segments();

			if (as == null) {
				as = NEW(Associated_Segs)(1);
				Alter(target_seg)->set_associated_segments (as);
			}

			as->segs.push_back(source_seg);
		}

		INLINE void unhook_all(Segment const * source_seg) {

			for (auto target_seg: segs) {

				Associated_Segs *	as = target_seg->get_associated_segments();
				ASSERT(as != null);

				auto segment_to_erase = std::find(as->segs.begin(), as->segs.end(), source_seg);
				ASSERT(segment_to_erase != as->segs.end());
				as->segs.erase(segment_to_erase);

				if (as->segs.empty()) {
					Alter(target_seg)->set_associated_segments(null);
					delete as;
				}
			}
		}

		void mark_highlight() {

			for (auto seg: segs) {
				Alter(seg)->dbflags |= Segment_HIGHLIGHT;
			}
		}

		void unmark_all_highlights() {

			for (auto target_seg: segs) {
				Associated_Segs *	as = target_seg->get_associated_segments();
				ASSERT(as != null);

				Alter(target_seg)->dbflags &= ~Segment_HIGHLIGHT;

				for (auto seg : as->segs) {
					Alter(target_seg)->dbflags |= seg->dbflags & Segment_HIGHLIGHT;
				}
			}
		}
};

// these can reused as they are mutually exclusive
#define Include_IMPLICIT_PRIORITY			DB_TYPE_SPECIFIC(1)
#define Include_STATIC_MAY_HAVE_CALLBACKS	DB_TYPE_SPECIFIC(2)
#define	Include_INVISIBLE					DB_TYPE_SPECIFIC(3)
#define	Include_STATIC_TREE					DB_TYPE_SPECIFIC(4)

struct Include : public Subsegment {
								// start xref common section
	Segment const *				referee;
	Conditional					conditional;
	Xref *						referee_next;
	Xref **						referee_backlink;
	Attribute_Lock *			filter;
								// end xref common section
	Include_Path				path;
};

typedef std::vector<Include const *, POOL_Allocator<Include const *> > Include_Vector_Pooled;

#define Style_PORTFOLIO						DB_TYPE_SPECIFIC(1)

struct Style : public Attribute {
	int32_t						pad_priority;		// keeps structures aligned due to shorter Attribute
								// start xref common section
	Segment const *				referee;
	Conditional					conditional;
	Xref *						referee_next;
	Xref **						referee_backlink;
	Attribute_Lock *			filter;
								// end xref common section
	Name						named_style;
};


struct Xref : public Include {};



GLOBAL_FUNCTION void LINK_GEO_REFERENCE (Thread_Data * thread_data, Geometry_Reference * gr);
GLOBAL_FUNCTION void UNLINK_GEO_REFERENCE (Thread_Data * thread_data, Geometry_Reference * gr, Segstuff const * ss, bool allow_missing=false);

#define Geometry_Reference_UNLINKED	DB_TYPE_SPECIFIC(1)

struct Geometry_Reference : public Geometry {

public:
		Conditional			conditional;
		Include_Path		path;
		Bounding			bounding;
private:
		Segstuff const **	_referees;
		Segstuff const *	_referees_builtin;
		int					_referee_count;
public:
		int					geometry_mask;
		Seen_Flags			seen;

		enum NHG : int {
			NHG_Overlay = 0,
			NHG_Spriting = 1,
			NHG_Inplace = 2,
			NHG_Count = 3,
			NHG_Invalid = NHG_Count,
		};
		uint32_t			not_highlighted_generation[NHG_Count];
		intptr_t			not_highlighted_hash_key;

		Geometry_Reference() : Geometry(GEOMETRY_REFERENCE) {}

		~Geometry_Reference() {

			UNLINK();

			for (int i=0; i<_referee_count; i++)
				INUTILE (_referees[i]);

			reset();
		}

		Geometry_Reference (Geometry_Reference const & that) : Geometry (that) {
			path = that.path;
			bounding = that.bounding;
			geometry_mask = that.geometry_mask;
			seen = that.seen;

			not_highlighted_generation[NHG_Overlay] = 0;
			not_highlighted_generation[NHG_Spriting] = 0;
			not_highlighted_generation[NHG_Inplace] = 0;
			not_highlighted_hash_key = 0;

			reserve(that._referee_count);

			COPY_ARRAY (that._referees, that._referee_count, Segstuff const *, _referees);

			for (int i = 0; i<_referee_count; i++)
				UTILE (_referees[i]);

			if (!ANYBIT(dbflags, Geometry_Reference_UNLINKED)) {
				Thread_Data *	thread_data;
				FIND_INTERNAL_THREAD_DATA (thread_data);
				LINK_GEO_REFERENCE(thread_data, this);
			}

			conditional = that.conditional;
		}

		Geometry * clone() const {return NEW(Geometry_Reference)(*this);}

		INLINE size_t size() const { return (size_t)_referee_count; }
		INLINE bool empty() const { return (_referee_count==0); }

		INLINE void	reserve(size_t size) {
			ASSERT(_referee_count == 0);
			_referee_count = (int)size;
			if (_referee_count > 1)
				ALLOC_ARRAY(_referees, _referee_count, Segstuff const *);
			else
				_referees = &_referees_builtin;
		}

		INLINE void	reset() {
			if (_referee_count > 1)
				FREE_ARRAY(_referees, _referee_count, Segstuff const *);
			_referee_count = 0;
			_referees = null;
		}

		// refs passes to the gr who will use it or free it
		INLINE void set(Segstuff const ** refs, size_t refs_size) {
			reset();
			if (refs_size == 1) {
				reserve(1);
				_referees[0] = refs[0];
				FREE_ARRAY(refs, refs_size, Segstuff const *);
			}
			else {
				_referee_count = (int)refs_size;
				_referees = refs;
			}
		}

		size_t memory_used(MU_Flags flags) const {
			size_t	size = Geometry::memory_used(flags);

			if (ANYBIT(flags, MU_DATABASE)) {
				size += sizeof(Geometry_Reference);
				size += _referee_count * sizeof(Segstuff *);
			}

			return size;
		}

		INLINE void UNLINK () {
			if (!ANYBIT(dbflags, Geometry_Reference_UNLINKED)) {
				Thread_Data *	thread_data;
				FIND_INTERNAL_THREAD_DATA (thread_data);
				for (int i=0; i< _referee_count; ++i)
					UNLINK_GEO_REFERENCE (thread_data, this, _referees[i], true);
			}
		}

		class const_iterator : public std::iterator < std::bidirectional_iterator_tag, Segstuff const *, size_t, Segstuff const * const *, Segstuff const * const & >
		{
			friend struct Geometry_Reference;
		public:
			typedef std::bidirectional_iterator_tag iterator_category;
			typedef Segstuff const *			value_type;
			typedef std::ptrdiff_t				difference_type;
			typedef Segstuff const * const *	pointer;
			typedef Segstuff const * const &	reference;

			INLINE const_iterator(Segstuff const ** referees, size_t index = 0) : _ptr(&referees[index]) {}

			INLINE reference operator*() {
				return *_ptr;
			}

			INLINE pointer operator->() {
				return &(**this);
			}

			INLINE const_iterator & operator++() {
				_ptr++;
				return *this;
			}

			INLINE const_iterator & operator--() {
				_ptr--;
				return *this;
			}

			INLINE const_iterator operator++(int) {
				const_iterator old_it = *this;
				++(*this);
				return old_it;
			}

			INLINE const_iterator operator--(int) {
				const_iterator old_it = *this;
				--(*this);
				return old_it;
			}

			INLINE bool operator ==(const_iterator const & that) {
				return _ptr == that._ptr;
			}

			INLINE bool operator !=(const_iterator const & that) {
				return !(*this == that);
			}
		protected:
			Segstuff const **	_ptr;
		};

		class iterator : public const_iterator
		{
			friend struct Geometry_Reference;
		public:
			typedef std::bidirectional_iterator_tag iterator_category;
			typedef Segstuff const *	value_type;
			typedef size_t				difference_type;
			typedef Segstuff const **	pointer;
			typedef Segstuff const *&	reference;

			using const_iterator::_ptr;

			INLINE iterator(Segstuff const ** referees, size_t iter_index = 0) : const_iterator(referees, iter_index) {}

			INLINE reference operator*() {
				return *_ptr;
			}

			INLINE pointer operator ->() {
				return &(**this);
			}

			INLINE iterator & operator++() {
				const_iterator::operator++();
				return *this;
			}

			INLINE iterator & operator--() {
				const_iterator::operator--();
				return *this;
			}

			INLINE iterator operator++(int) {
				iterator old_it = *this;
				++(*this);
				return old_it;
			}

			INLINE iterator operator--(int) {
				iterator old_it = *this;
				--(*this);
				return old_it;
			}
		};

		friend class const_iterator;
		friend class iterator;

		INLINE Segstuff const * & front() {
			return *begin();
		}

		INLINE Segstuff const * & back() {
			return *--end();
		}

		INLINE Segstuff const * const & front() const {
			return *begin();
		}

		INLINE Segstuff const * const & back() const {
			return *--end();
		}

		INLINE iterator begin() {
			return iterator(_referees, 0);
		}

		INLINE iterator end() {
			return iterator(_referees, _referee_count);
		}

		INLINE const_iterator begin() const {
			return const_iterator(_referees, 0);
		}

		INLINE const_iterator end() const {
			return const_iterator(_referees, _referee_count);
		}

		INLINE const_iterator cbegin() const {
			return const_iterator(_referees, 0);
		}

		INLINE const_iterator cend() const {
			return const_iterator(_referees, _referee_count);
		}

		INLINE iterator index_iterator(size_t in_index) {
			return iterator(_referees, in_index);
		}

		INLINE const_iterator cindex_iterator(size_t in_index) const {
			return const_iterator(_referees, in_index);
		}

		INLINE iterator find(Segstuff const * g) {
			for (size_t i=0; i<(size_t)_referee_count; ++i) {
				if (_referees[i] == g)
					return index_iterator(i);
			}
			return end();
		}

		INLINE const_iterator find(Segstuff const * g) const {
			for (size_t i=0; i<(size_t)_referee_count; ++i) {
				if (_referees[i] == g)
					return cindex_iterator(i);
			}
			return cend();
		}

		INLINE void remove(iterator it) {
			if (_referee_count > 1) {
				ASSERT(it._ptr >= _referees);
				size_t index_to_remove = it._ptr - _referees;
				if (_referee_count > 2) {
					size_t				new_size = _referee_count - 1;
					Segstuff const **	new_refs;
					ALLOC_ARRAY(new_refs, new_size, Segstuff const *);
					Segstuff const **	r = new_refs;
					for (size_t i = 0; i < index_to_remove; ++i) {
						*r = _referees[i];
						++r;
					}
					for (size_t i = index_to_remove+1; i < (size_t)_referee_count; ++i) {
						*r = _referees[i];
						++r;
					}
					set(new_refs, new_size);
				}
				else {
					Segstuff const * g = _referees[index_to_remove ^ 0x1];
					FREE_ARRAY(_referees, _referee_count, Segstuff const *);
					_referee_count = 1;
					_referees = &_referees_builtin;
					_referees[0] = g;
				}
			}
			else {
				reset();
			}
		}

		INLINE bool remove(Segstuff const * g) {
			auto it = find(g);
			if (it != end()) {
				remove(it);
				return true;
			}
			return false;
		}

		INLINE void push_back(Segstuff const * g) {
			if (_referee_count > 0) {
				size_t				new_size = _referee_count + 1;
				Segstuff const **	new_refs;
				ALLOC_ARRAY(new_refs, new_size, Segstuff const *);
				COPY_ARRAY(_referees, _referee_count, Segstuff const *, new_refs);
				new_refs[_referee_count] = g;
				set(new_refs, new_size);
			}
			else {
				reserve(1);
				_referees[0] = g;
			}
		}

	private:
		Geometry_Reference const & operator= (Geometry_Reference const & that);
};


INLINE	void	Extended_Segment_Data::set_references (Xref * x) {
	Reference_Cache *		rc = (Reference_Cache *)get (Extended_References);
	if (x != null) {
		if (rc == null) {
			rc = NEW(Reference_Cache)();
			set (Extended_References, rc);
		}
		rc->references = x;
		x->referee_backlink = (Xref **)&rc->references;
	}
	else {
		delete rc;
		set (Extended_References, null);
	}
}





INLINE Segment const * Segment_From_Subsegment (Subsegment const * subseg) {
	if (subseg != null && subseg->type == INCLUDE) {
		Include const *		include = (Include const *)subseg;

		return (Segment const *)include->referee;
	}
	return (Segment const *)subseg;
}
INLINE Segment * Segment_From_Subsegment (Subsegment * subseg) {
	if (subseg != null && subseg->type == INCLUDE) {
		Include *		include = (Include *)subseg;

		return (Segment *)include->referee;
	}
	return (Segment *)subseg;
}


enum Strip_Flags : uint32_t {

	DL_NONE							= 0x00000000,

	DL_LIGHTING						= 0x00000001,
	DL_FLAT_LIGHTING				= 0x00000002,
	DL_TEXTURING					= 0x00000004,
	DL_PATTERN						= 0x00000008,
	DL_COLOR_INTERPOLATION			= 0x00000010,
	DL_INDEX_INTERPOLATION			= 0x00000020,
	DL_FORCE_SMOOTH_SHADE_MODEL		= 0x00000040, /* use smooth shade model when drawing */
	DL_COLORS_LOCKED				= 0x00000080,
	DL_NATURAL_PARAMETERS			= 0x00000100,
	DL_LOCAL_GRAYSCALE				= 0x00000200, /* grayscale colors baked into display list */
	DL_TEMPORARY_STRIP				= 0x00000400, /* not attached to database */
	DL_TEMPORARY_DATA				= 0x00000800, /* isolate needed before deferral */
	DL_DO_NOT_DEFER					= 0x00001000,
	DL_3D_TRANSPARENCY				= 0x00002000,
	DL_DO_NOT_RECOMPILE				= 0x00004000, /* struct has no real data.  it is a display list container only.  do not punt, defer or recompile */
	DL_POLYEDGE_FROM_TRISTRIP		= 0x00008000,
	DL_LIGHTWEIGHT_DISPLAY_LIST		= 0x00010000, /* display list is a GLuint (or equivalent), not an Display_List pointer */
	DL_INSUFFICIENT_VRAM			= 0x00020000, /* display lists were suppressed because of vram limits */
	DL_TEXTURE_PREBOUND				= 0x00040000, /* skip texture setup */
	DL_NEGATIVE_3X3					= 0x00080000, /* tristrips only: faces were transformed through a negative scale matrix at display list creation time (i.e. handedness reversed) */
	DL_WIDE_LINE					= 0x00100000,
	DL_VERTEX_NORMALS				= 0x00200000,	/* the fake/temp polymarker has vertex normals */
	DL_HAS_TRANSPARENCY				= 0x00400000,
	DL_RESOLVED_FINDICES			= 0x00800000, /* findices are resolved into face colors.  SDO_FACE/EDGE_COLORS should be synchronized with this */
	DL_unused1						= 0x01000000,
	DL_SHADOW_TRISTRIP				= 0x02000000, /* trigger special handling for the simple shadow's tristrip */
	DL_SUPPRESS_GEO_DLS				= 0x04000000, /* do not attempt to create geometry-level display lists (segment-level still ok, though, unless otherwise specified)*/
	DL_SUPPRESS_SEG_DLS				= 0x08000000, /* do not attempt to create segment-level display lists (geometry-level still ok, though, unless otherwise specified)*/
	DL_COLORS_BY_FINDEX				= 0x10000000, /* the tristrip or polyedge has colors by findex attached */
	DL_INDEXICATED					= 0x20000000, /* has colors by findex and was compiled with cii (color index interpolation) enabled */
	DL_VCOLORED_TRISTRIP			= 0x40000000, /* has vertex colors by findex or value and was compiled with ci (color interpolation) enabled */

	DL_FLAG_INVALID					= 0xFFFFFFFF,

	DL_SUPPRESS_ALL_DISPLAY_LISTS = (DL_SUPPRESS_GEO_DLS | DL_SUPPRESS_SEG_DLS),


	/* utility mask to get rid of some bits that should not trigger display list recompiles */
	DL_SPECIALS_MASK = (uint32_t)(~(DL_FORCE_SMOOTH_SHADE_MODEL | DL_DO_NOT_RECOMPILE | DL_TEMPORARY_STRIP | DL_TEMPORARY_DATA |
						DL_INSUFFICIENT_VRAM | DL_LIGHTWEIGHT_DISPLAY_LIST | DL_SUPPRESS_ALL_DISPLAY_LISTS | DL_INDEXICATED)),
};

template<> struct _bit_ops_type < Strip_Flags > {
	typedef Strip_Flags type;
};

enum Strip_Flags2 : uint32_t {

	DL2_NONE						= 0x00000000,

	DL2_LOCAL_IBOS					= 0x00000001,
	DL2_POLY_CYLINDER				= 0x00000002, /* polycylinders: current polyedge is a polycylinder axis */
	DL2_PC_AXIS						= 0x00000004, /* polycylinders: current polyedge is a polycylinder axis */
	DL2_PC_RADII_REVERSED			= 0x00000008, /* polycylinders: reverse radii */
	DL2_PC_COLORS_REVERSED			= 0x00000010, /* polycylinders: reverse colors */
	DL2_PC_LEFT_HANDED				= 0x00000020, /* polycylinders: polycylinder was generated as left handed */
	DL2_NEEDS_RECOLLECTION			= 0x00000040, /* if compiled with "compile only", we need to revisit all non-dl geometry in the segment */
	DL2_POLYGON						= 0x00000080, /* the object started life as a polygon, so we don't need to worry about interpolation options or texturing */
	DL2_CONTAINS_CURVES				= 0x00000100,
	DL2_VIEW_DEPENDENT_CURVES		= 0x00000200,
	DL2_FAKE_ELLIPSE				= 0x00000400, /* the object started life as an ellipse, should be drawn as a quad and rasterized. */
	DL2_EFTC						= 0x00000800, /* force texture coordinates */
	DL2_DRAW_LAST_PIXEL				= 0x00001000, /* polyedges: draw a dot at the end of each strip */
	DL2_ISOLINE						= 0x00002000, /* polyedges: current polyedge is an isoline */
	DL2_SSAA_ACTIVE					= 0x00004000, /* polyedges: compiled when SSAA was active */
	DL2_PRIORITY_HSRA				= 0x00008000, /* display list compiled with priority values on the vertices (meaning that if priorities change, the display list needs to be invalidated) */
	DL2_MARKER_POINT_SPRITE			= 0x00010000, /* */
	DL2_MARKER_CIRCLE				= 0x00020000, /* */
	DL2_GS_ELLIPTICAL_ARCS			= 0x00040000, /* */
	DL2_RANDOMIZED_VERTICES			= 0x00080000, /* display list compiled with randomized vertices*/
	DL2_PEELING_DISPLAY_LIST		= 0x08000000, /* used to identify dpdl (which are in most, but not all, aspects just like segdl's */


	/* utility mask to get rid of some bits that should not trigger display list recompiles */
	DL2_SPECIALS_MASK = (uint32_t)~DL2_PC_LEFT_HANDED,
};

template<> struct _bit_ops_type < Strip_Flags2 > {
	typedef Strip_Flags2 type;
};

#define DL_TYPE_GEOMETRY		0
#define DL_TYPE_TEXTURE			1

class Driver_DL {
public:
	Display_List *		peer;
	Driver_DL *			next;
	Driver_DL **		prev;
	Actor const *		actor;
	Base_Shadow *		notify_shadow;
#ifdef TEST_RIG_1_SHADERS
	void *				extra;
#endif
	uint32_t			type;
	uint32_t			generation;			//most recent edit
};


typedef std::vector<Driver_DL *, POOL_Allocator<Driver_DL *> > Driver_DL_List;

typedef uint32_t	Action_Mask;

struct Display_List : public Minimum_Header<Display_List, Anything> {

private:
	mutable Actor const *	actor;		/* owner */
public:
	Driver_DL *			list;			/* whatever */
	Display_List *		mate;			/* segdl and dpdl only: dpdl points at segdl, and vice versa */
	Action_Mask			action_mask;
	Rendo_Flags			rendo_flags;	/* options at creation */
	Strip_Flags			special;		/* extra info */
	Strip_Flags2		special2;		/* extra info */
	int					heap_location;	/* location, if any, in the heap (so that we can pop it out if it gets freed) */
	float				priority;
	int					vram;

	INLINE Actor const *	get_actor() const {
		ASSERT(actor != null);
		return actor;
	}

	INLINE void set_actor(Actor const * in_actor) {
		actor = in_actor;
	}

	static bool Get(Actor const * driver, Display_List ** linker, Display_List *& out);

#ifdef HOOPS_DEBUG_BUILD
	static void validate(Display_List *	dl);
#else
	static INLINE void validate(Display_List *	dl) { UNREFERENCED(dl); };
#endif
};


struct Display_List_Cache_Internal :  public Counted_Object_Pooled {
	friend class Counted_Pointer_Pooled<Display_List_Cache_Internal>;

	Display_List_Cache_Internal(Memory_Pool * memory_pool) : Counted_Object_Pooled (memory_pool) {}

	~Display_List_Cache_Internal() {
		Display_List *				dl;

		while ((dl = display_lists) != null) {
			display_lists = dl->next;
			INUTILE(dl);
		}

		while ((dl = depth_peeling_display_lists) != null) {
			depth_peeling_display_lists = dl->next;
			INUTILE(dl);
		}

		while ((dl = polyline_display_lists) != null) {
			polyline_display_lists = dl->next;
			INUTILE(dl);
		}
	}
	Display_List *			display_lists;
	Display_List *			depth_peeling_display_lists;
	Display_List *			polyline_display_lists;
};

typedef Counted_Pointer_Pooled<Display_List_Cache_Internal> Display_List_Cache;


struct Proxy_Cache : public CMO {
	Segment *		proxy;
	int32_t			depopulate_generation;
};

typedef Unordered<intptr_t, Segment_Cache *, Hasher<intptr_t>,
	std::equal_to<intptr_t>, POOL_Allocator<std::pair<const intptr_t, Segment_Cache *> > > Pathed_Caches;


typedef Unordered<Name, Segment *, NameHasher, std::equal_to<const Name>, POOL_Allocator<std::pair<const Name, Segment *> > > Segment_Lookup_Hash;


struct Segment_Cache : public Pointer_Cache {
	private:
		enum {
			Cache_Item_NMM,
			Cache_Item_PROXY,
			Cache_Item_STATIC,
			Cache_Item_FRAME_BUFFER_INFO,
			Cache_Item_SIMPLE_SHADOW,
			Cache_Item_SHADOW_MAP,
			Cache_Item_CUT_GEOMETRY,
			Cache_Item_HIDDEN_LINE,
			Cache_Item_SEGFLAGS,
			Cache_Item_LOOKUP_HASH,
			Cache_Item_PATH_INDEX
		};

		struct Static_Cache : public CMO {
			Include *				attribute_tree;
		};

		struct Matrix_Cache : public CMO {
			Matrix	matrix;
		};


	public:
		Segment_Cache *			next;
		Segment const *			segment;
		Include_Path			path;
		Display_List_Cache		dlc_handle;

		Pathed_Caches *			pathed_caches;
		Include_Path			last_path;
		Segment_Cache *			last_segment_cache;

		Net_Rendition			net_rendition;		// not a trivial pointer

		//HOOPS_API Segment_Cache ();
		HOOPS_API Segment_Cache (Segment const * s, Include_Path const & p);
		HOOPS_API ~Segment_Cache ();

		bool						empty () const { return Pointer_Cache::empty() && net_rendition == null; }

		void						set_fbi (Frame_Buffer_Info * fbi) {
										Alter(segment)->segflags |= Seg_Cached_NR;
										set (Cache_Item_FRAME_BUFFER_INFO, fbi);
									}
		INLINE Frame_Buffer_Info *	get_fbi () const { return (Frame_Buffer_Info *)get (Cache_Item_FRAME_BUFFER_INFO); }

		void						set_proxy_cache (Proxy_Cache * pc) { set (Cache_Item_PROXY, pc); }
		INLINE Proxy_Cache *		get_proxy_cache () const { return (Proxy_Cache *)get (Cache_Item_PROXY); }

		void						set_simple_shadow (Simple_Shadow * ss) { set (Cache_Item_SIMPLE_SHADOW, ss); }
		INLINE Simple_Shadow *		get_simple_shadow () const { return (Simple_Shadow *)get (Cache_Item_SIMPLE_SHADOW); }

		void						set_shadow_map (Shadow_Map * sm) { set (Cache_Item_SHADOW_MAP, sm); }
		INLINE Shadow_Map *			get_shadow_map () const { return (Shadow_Map *)get (Cache_Item_SHADOW_MAP); }

		void						set_cut_geometry (Cut_Geometry_Set * pc) { set (Cache_Item_CUT_GEOMETRY, pc); }
		INLINE Cut_Geometry_Set *	get_cut_geometry () const { return (Cut_Geometry_Set *)get (Cache_Item_CUT_GEOMETRY); }

		void						set_hidden_line (HL_Data * hl) { set (Cache_Item_HIDDEN_LINE, hl); }
		INLINE HL_Data *			get_hidden_line () const { return (HL_Data *)get (Cache_Item_HIDDEN_LINE); }

		void						set_net_modelling_matrix (Matrix const & matrix) {
										Matrix_Cache *	mc = (Matrix_Cache *)get (Cache_Item_NMM);
										if (matrix != null) {
											if (mc == null) {
												mc = NEW(Matrix_Cache)();
												set (Cache_Item_NMM, mc);
											}
											mc->matrix = matrix;
										}
										else {
											delete mc;
											set (Cache_Item_NMM, null);
										}
									}
		Matrix						get_net_modelling_matrix () const {
										Matrix_Cache *	mc = (Matrix_Cache *)get (Cache_Item_NMM);
										if (mc == null)
											return null;
										return mc->matrix;
									}

		void						set_pathed_caches (Pathed_Caches * pch) {pathed_caches = pch;}

		INLINE Pathed_Caches *		get_pathed_caches () const {return pathed_caches;}

		void						set_last_path (Include_Path const & lp) {last_path = lp;}
		INLINE Include_Path const &	get_last_path() const {return last_path;}

		void						set_last_segment_cache (Segment_Cache * sc) {last_segment_cache = sc;}
		INLINE Segment_Cache *		get_last_segment_cache() const {return last_segment_cache;}

		void						set_segflags (Seg_Flags f) { set (Cache_Item_SEGFLAGS, (void *)(intptr_t)f); }
		INLINE Seg_Flags			get_segflags () const { return (Seg_Flags)(intptr_t)get (Cache_Item_SEGFLAGS); }

		void						set_lookup_hash (Segment_Lookup_Hash * slh) { set (Cache_Item_LOOKUP_HASH, slh); }
		INLINE Segment_Lookup_Hash *get_lookup_hash () const { return (Segment_Lookup_Hash *)get (Cache_Item_LOOKUP_HASH); }


		void						set_static (Include * at) {
										Static_Cache * sc = (Static_Cache *)get (Cache_Item_STATIC);

										if (at != null) {
											if (sc == null) {
												sc = NEW(Static_Cache)();
												set (Cache_Item_STATIC, sc);
											}
											sc->attribute_tree = at;
											at->backlink = (Subsegment **)&sc->attribute_tree;
										}
										else {
											delete sc;
											set (Cache_Item_STATIC, null);
										}
									}
		INLINE Include *			get_static () const {
										Static_Cache * sc = (Static_Cache *)get (Cache_Item_STATIC);

										if (sc == null)
											return null;
										return sc->attribute_tree;
									}

		void								set_dl_cache (Display_List_Cache const & in_dlc) {dlc_handle = in_dlc;}
		INLINE Display_List_Cache const &	get_dl_cache () const {return dlc_handle;}

		void					set_path_index (uint32_t i) { set (Cache_Item_PATH_INDEX, (void *)(intptr_t)i); }
		INLINE uint32_t			get_path_index () const { return (uint32_t)(intptr_t)get (Cache_Item_PATH_INDEX); }
};




enum Precision_Options {
	PO_Single				= 0x0,	// force, no checking
	PO_Single_If_Range		= 0x1,	// if range allows
	PO_Single_If_Precision	= 0x2,	// if no [significant?] precision loss
	PO_Single_If_Safe		= 0x3,	// both of previous
	PO_Double				= 0x4	// always safe
};



#define Polyline_INKING			DB_TYPE_SPECIFIC(1)
#define Polyline_BY_REFERENCE	DB_TYPE_SPECIFIC(2)
#define Polyline_BASIS_PAIR		DB_TYPE_SPECIFIC(3)
#define Polyline_INFINITE_RAY	DB_TYPE_SPECIFIC(4)

struct Polyline : public Geometry_X<Point_3D> {
public:
	int			count;
	int			allocated;
	Vector *	orientation;

	Polyline(DB_Type in_type=POLYLINE) : Geometry_X(in_type) {}

	~Polyline() {
		if (!ANYBIT(dbflags, Polyline_BY_REFERENCE)) {
			if (allocated > 0) {
				if (ANYBIT(dbflags, DB_DOUBLE_PRECISION))
					FREE_ARRAY (double_data, allocated, DPoint);
				else
					FREE_ARRAY (single_data, allocated, Point);
			}
			if (orientation != null) {
				if (ANYBIT(dbflags, Polyline_BASIS_PAIR))
					FREE_ARRAY (orientation, 2, Vector);
				else
					FREE_ARRAY (orientation, 1, Vector);
			}
		}
	}

	Polyline (Polyline const & that) : Geometry_X (that) {
		allocated = Abs(that.count);
		count = that.count;

		CLONE_ARRAY(that.single_data, allocated, Point, single_data);
		CLONE_ARRAY(that.double_data, allocated, DPoint, double_data);

		if (that.orientation != null) {
			if (ANYBIT(that.dbflags, Polyline_BASIS_PAIR))
				CLONE_ARRAY(that.orientation, 2, Vector, orientation);
			else
				CLONE_ARRAY(that.orientation, 1, Vector, orientation);
		}
	}

	Geometry * clone() const {return NEW(Polyline)(*this);}

	size_t memory_used(MU_Flags flags) const {
		size_t	size = Geometry_X<Point_3D>::memory_used(flags);

		if (ANYBIT(flags, MU_DATABASE)) {
			size += sizeof(Polyline);

			if (!ANYBIT(dbflags, Polyline_BY_REFERENCE)) {
				if (allocated > 0) {
					if (ANYBIT(dbflags, DB_DOUBLE_PRECISION))
						size += allocated * sizeof(DPoint);
					else
						size += allocated * sizeof(Point);
				}

				if (orientation != null) {
					if (ANYBIT(dbflags, Polyline_BASIS_PAIR))
						size += 2 * sizeof(Vector);
					else
						size += sizeof(Vector);
				}
			}
		}

		return size;
	}

	template <typename F>
	INLINE Point_3D<F> * Alloc_Points(int in_count) {
		count = in_count;
		allocated = Abs(count);
		ALLOC_ARRAY(precision_data<F>(this), allocated, Point_3D<F>);
		update_precision_flag<F>(this);
		return precision_data<F>(this);
	}

private:
	Polyline const & operator= (Polyline const & that);
};

struct Multiline : public Geometry_X<Point_3D> {
public:
	int				count;
	int				allocated;

	intptr_t *		mapping;

	Multiline() : Geometry_X(MULTILINE) {}
	Multiline (Multiline const & that);
	~Multiline();

	Geometry * clone() const {return NEW(Multiline)(*this);}
	void fixup();

	size_t memory_used(MU_Flags flags) const {
		size_t	size = Geometry_X<Point_3D>::memory_used(flags);

		if (ANYBIT(flags, MU_DATABASE)) {
			size += sizeof(Multiline);

			if (allocated > 0) {
				if (single_data != null)
					size += allocated * sizeof(Point);
				if (double_data != null)
					size += allocated * sizeof(DPoint);
			}

			if (mapping != null)
				size += allocated/2 * sizeof(intptr_t);
		}

		if (ANYBIT(flags, MU_AMORTIZE))
			size = (size_t)((float)size / Abs(count/2));

		return size;
	}

private:
	Multiline const & operator= (Multiline const & that);
};

#define Polygon_PLANE_TESTED	DB_TYPE_SPECIFIC(1)
#define Polygon_PLANE_VALID		DB_TYPE_SPECIFIC(2)
#define Polygon_NON_CONVEX		DB_TYPE_SPECIFIC(3)

struct Polygon : public Geometry_X<Point_3D> {
public:
	int		count;
	int		allocated;

	Plane	plane;

	Polygon() : Geometry_X(POLYGON) {}

	~Polygon() {
		if (allocated > 0) {
			if (single_data != null)
				FREE_ARRAY (single_data, allocated, Point);
			if (double_data != null)
				FREE_ARRAY (double_data, allocated, DPoint);
		}
	}

	Polygon (Polygon const & that) : Geometry_X (that) {
		CLONE_ARRAY(that.single_data, that.count, Point, single_data);
		CLONE_ARRAY(that.double_data, that.count, DPoint, double_data);

		allocated = that.count;
		count = that.count;
		plane = that.plane;

		dbflags |= that.dbflags & (Polygon_PLANE_TESTED|Polygon_PLANE_VALID|Polygon_NON_CONVEX);
	}

	Geometry * clone() const {return NEW(Polygon)(*this);}

	size_t memory_used(MU_Flags flags) const {
		size_t	size = Geometry_X<Point_3D>::memory_used(flags);

		if (ANYBIT(flags, MU_DATABASE)) {
			size += sizeof(Polygon);

			if (allocated > 0) {
				if (single_data != null)
					size += allocated * sizeof(Point);
				if (double_data != null)
					size += allocated * sizeof(DPoint);
			}
		}

		return size;
	}
};

struct Contour : public Geometry_X<Point_3D> {
public:
	int *	lengths;
	int		count;
	int		allocated;
	int		length_count;
	int		length_count_allocated;
	Plane	plane;

	Contour() : Geometry_X(CONTOUR) {}

	~Contour() {
		if (allocated > 0) {
			if (single_data != null)
				FREE_ARRAY (single_data, allocated, Point);
			if (double_data != null)
				FREE_ARRAY (double_data, allocated, DPoint);
		}

		if (length_count_allocated > 0)
			FREE_ARRAY (lengths, length_count_allocated, int);
	}

	Contour (Contour const & that) : Geometry_X (that) {
		ASSERT(0);
	}

	Geometry * clone() const {return NEW(Contour)(*this);}

	size_t memory_used(MU_Flags flags) const {
		size_t	size = Geometry_X<Point_3D>::memory_used(flags);

		if (ANYBIT(flags, MU_DATABASE)) {
			size += sizeof(Contour);

			if (allocated > 0) {
				if (single_data != null)
					size += allocated * sizeof(Point);
				if (double_data != null)
					size += allocated * sizeof(DPoint);
			}

			if (length_count_allocated > 0)
				size += length_count_allocated * sizeof(int);
		}

		return size;
	}

private:
	Contour const & operator= (Contour const & that);
};


/* careful: matching array in show_onefloats */
#	define		GSU_UNQUALIFIED (Generic_Size_Units)0
#	define		GSU_ORU			(Generic_Size_Units)1
#	define		GSU_SRU			(Generic_Size_Units)2
#	define		GSU_WRU			(Generic_Size_Units)3
#	define		GSU_PIXELS		(Generic_Size_Units)4
#	define		GSU_POINTS		(Generic_Size_Units)5
#	define		GSU_PERCENT		(Generic_Size_Units)6
#	define		GSU_SCALE		(Generic_Size_Units)7
#	define		GSU_WORLD		(Generic_Size_Units)8
/* if more are added to this list, show_onefloats will need updating */

class Generic_Size {
public:
	float					value;
	Generic_Size_Units		units;

	Generic_Size(float in_value, Generic_Size_Units in_units): value(in_value), units(in_units) {};

	Generic_Size(): value(0), units(GSU_UNQUALIFIED) {};

	INLINE bool operator == (Generic_Size const & that) const {
		return (value == that.value && units == that.units);
	};

	INLINE bool operator != (Generic_Size const & that) const {
		return (value != that.value || units != that.units);
	};
};





class CPV_Mode {
public:
	enum Mode {
		None,
		Round,
		Square,
		Plane
	};
};

struct Cutting_Plane_Options {
	Bounding		bounding;
	struct {
		Name			name;	// corresponds to rgb portion; g & b negative if r stores index
		RGBA			rgba;	// 
	}				face, edge;
	float			scale;
	Generic_Size	edge_weight;
	CPV_Mode::Mode	mode;
};

struct Cutting_Plane : public Geometry_X<Plane_3D> {
public:
	int							count;
	Cutting_Plane_Options *		options;

	Cutting_Plane() : Geometry_X(CUTTING_PLANE) {}

	~Cutting_Plane() {
		if (options != null) {
			options->face.name = null;
			options->edge.name = null;
			FREE(options, Cutting_Plane_Options);
		}

		if (count > 0) {
			if (ANYBIT (dbflags, DB_DOUBLE_PRECISION))
				FREE_ARRAY (double_data, count, DPlane);
			else
				FREE_ARRAY (single_data, count, Plane);
		}
	}

	Cutting_Plane (Cutting_Plane const & that) : Geometry_X (that) {
		options = 0;
		count = that.count;

		if (ANYBIT (that.dbflags, DB_DOUBLE_PRECISION))
			CLONE_ARRAY(that.double_data, that.count, DPlane, double_data);
		else
			CLONE_ARRAY(that.single_data, that.count, Plane, single_data);

		if (that.options != null) {
			ZALLOC(options, Cutting_Plane_Options);
			*options = *that.options;
		}
	}

	Geometry * clone() const {return NEW(Cutting_Plane)(*this);}

	size_t memory_used(MU_Flags flags) const {
		size_t	size = Geometry_X<Plane_3D>::memory_used(flags);

		if (ANYBIT(flags, MU_DATABASE)) {
			size += sizeof(Cutting_Plane);

			if (options != null) {
				size += options->face.name.length();
				size += options->edge.name.length();
				size += sizeof(Cutting_Plane_Options);
			}

			if (count > 0) {
				if (ANYBIT (dbflags, DB_DOUBLE_PRECISION))
					size += count * sizeof(DPoint);
				else
					size += count * sizeof(Point);
			}
		}

		return size;
	}

private:
	Cutting_Plane const & operator= (Cutting_Plane const & that);
};

template <typename F>
INLINE Plane_3D<F> * Create_Cutting_Planes(Cutting_Plane * cp, int count) {
	ALLOC_ARRAY(precision_data<F>(cp), count, Plane_3D<F>);
	cp->count = count;
	update_precision_flag<F>(cp);
	return precision_data<F>(cp);
}

// NOTE: all circle & ellipse structures are arranged to share all common ellipse values; in addition, all common arc
// parameters then match each other. circle and ellipse specific data vary in size so they come last.
// circular_wedge & circular_chord are just special cases of circular_arc
// which define an extra point or two, and are filled instead of stroked

// careful: Circle won't fit into the pattern of Generic_Arc if ANYBIT (circle->dbflags, Circle_LIGHTWEIGHT|Circle_LINEAR_ARC)
#define Circle_USER_CENTER		DB_TYPE_SPECIFIC(1)
#define Circle_LIGHTWEIGHT		DB_TYPE_SPECIFIC(2)
#define Circle_LINEAR_ARC		DB_TYPE_SPECIFIC(3)

// first, a lot of different data templates and synonym typedefs
template <typename F>
struct Generic_Ellipse_Data {
	Point_3D<F>					center;
	Vector_3D<F>				basis1;
	Vector_3D<F>				basis2;
	Plane_3D<F>					plane;
};
typedef Generic_Ellipse_Data<float>		Generic_Ellipse_Single;
typedef Generic_Ellipse_Data<double>	Generic_Ellipse_Double;

struct Arc_Limits {
	float						arc_start;
	float						arc_end;
};

template <typename F>	struct Generic_Arc_Data : public Generic_Ellipse_Data<F>, Arc_Limits {};
typedef Generic_Arc_Data<float>			Generic_Arc_Single;
typedef Generic_Arc_Data<double>		Generic_Arc_Double;


template <typename F>
struct Ellipse_Specific_Data {
	Point_3D<F>					major;
	Point_3D<F>					minor;
};

template <typename F>
struct Circle_Specific_Data {
	F							radius;
	Point_3D<F>					start;
	Point_3D<F>					middle;
	Point_3D<F>					end;
};


template <typename F>	struct Ellipse_Data			: public Generic_Ellipse_Data<F>,	Ellipse_Specific_Data<F> {};
template <typename F>	struct Elliptical_Arc_Data	: public Generic_Arc_Data<F>,		Ellipse_Specific_Data<F> {};
template <typename F>	struct Circle_Data			: public Generic_Ellipse_Data<F>,	Circle_Specific_Data<F> {};
template <typename F>	struct Circular_Arc_Data	: public Generic_Arc_Data<F>,		Circle_Specific_Data<F> {};

typedef Ellipse_Data<float>				Ellipse_Single;
typedef Ellipse_Data<double>			Ellipse_Double;

typedef Elliptical_Arc_Data<float>		Elliptical_Arc_Single;
typedef Elliptical_Arc_Data<double>		Elliptical_Arc_Double;

typedef Circle_Data<float>				Circle_Single;
typedef Circle_Data<double>				Circle_Double;

typedef Circular_Arc_Data<float>		Circular_Arc_Single;
typedef Circular_Arc_Data<double>		Circular_Arc_Double;

// these are used to save memory, but require extra handling to store/retrieve them in place of the basic structures above
template <typename F>
struct Circle_Lite_Data {
	struct {	F	x, y;	}	center;
	F							radius;
};
typedef Circle_Lite_Data<float>		Circle_Lite_Single;
typedef Circle_Lite_Data<double>	Circle_Lite_Double;

template <typename F>	struct Circular_Arc_Lite_Data : public Circle_Lite_Data<F>, Arc_Limits {};
typedef Circular_Arc_Lite_Data<float>	Circular_Arc_Lite_Single;
typedef Circular_Arc_Lite_Data<double>	Circular_Arc_Lite_Double;

template <typename F>
struct Circular_Arc_Linear_Data {
	Point_3D<F>			points[3];
};
typedef Circular_Arc_Linear_Data<float>		Circular_Arc_Linear_Single;
typedef Circular_Arc_Linear_Data<double>	Circular_Arc_Linear_Double;

// a couple pseudo base class types
struct Generic_Ellipse : public Geometry_X<Generic_Ellipse_Data> {
private:
	Generic_Ellipse();
	~Generic_Ellipse();
	Generic_Ellipse (Generic_Ellipse const & that);
};

struct Generic_Arc : public Geometry_X<Generic_Arc_Data> {
private:
	Generic_Arc();
	~Generic_Arc();
	Generic_Arc (Generic_Arc const & that);
};


// and finally the geometries themselves
struct Elliptical_Arc : public Geometry_X<Elliptical_Arc_Data> {
public:

	Elliptical_Arc() : Geometry_X(ELLIPTICAL_ARC) {}

	~Elliptical_Arc() {
		if (ANYBIT (dbflags, DB_DOUBLE_PRECISION))
			FREE(double_data, Elliptical_Arc_Double);
		else
			FREE(single_data, Elliptical_Arc_Single);
	}

	Elliptical_Arc (Elliptical_Arc const & that) : Geometry_X(that) {
		if (ANYBIT (that.dbflags, DB_DOUBLE_PRECISION)) {
			ALLOC (double_data, Elliptical_Arc_Double);
			*double_data = *that.double_data;
		}
		else {
			ALLOC (single_data, Elliptical_Arc_Single);
			*single_data = *that.single_data;
		}
	}

	Geometry * clone() const {return NEW(Elliptical_Arc)(*this);}

	size_t memory_used(MU_Flags flags) const {
		size_t	size = Geometry_X<Elliptical_Arc_Data>::memory_used(flags);

		if (ANYBIT(flags, MU_DATABASE)) {
			size += sizeof(Elliptical_Arc);

			if (ANYBIT (dbflags, DB_DOUBLE_PRECISION))
				size += sizeof(Elliptical_Arc_Double);
			else
				size += sizeof(Elliptical_Arc_Single);
		}

		return size;
	}
};

struct Ellipse : public Geometry_X<Ellipse_Data> {
public:

	Ellipse() : Geometry_X(ELLIPSE) {}

	~Ellipse() {
		if (ANYBIT (dbflags, DB_DOUBLE_PRECISION))
			FREE(double_data, Ellipse_Double);
		else
			FREE(single_data, Ellipse_Single);
	}

	Ellipse (Ellipse const & that) : Geometry_X(that) {
		if (ANYBIT (that.dbflags, DB_DOUBLE_PRECISION)) {
			ALLOC (double_data, Ellipse_Double);
			*double_data = *that.double_data;
		}
		else {
			ALLOC (single_data, Ellipse_Single);
			*single_data = *that.single_data;
		}
	}

	Geometry * clone() const {return NEW(Ellipse)(*this);}

	size_t memory_used(MU_Flags flags) const {
		size_t	size = Geometry_X<Ellipse_Data>::memory_used(flags);

		if (ANYBIT(flags, MU_DATABASE)) {
			size += sizeof(Ellipse);

			if (ANYBIT (dbflags, DB_DOUBLE_PRECISION))
				size += sizeof(Ellipse_Double);
			else
				size += sizeof(Ellipse_Single);
		}

		return size;
	}
};

struct Circular_Arc : public Geometry_X<Circular_Arc_Data> {
public:

	Circular_Arc(DB_Type in_type) : Geometry_X(in_type) {}

	~Circular_Arc() {
		if (ANYBIT (dbflags, DB_DOUBLE_PRECISION)) {
			if (ANYBIT (dbflags, Circle_LINEAR_ARC))
				FREE(double_data, Circular_Arc_Linear_Double);
			else if (ANYBIT (dbflags, Circle_LIGHTWEIGHT))
				FREE(double_data, Circular_Arc_Lite_Double);
			else
				FREE(double_data, Circular_Arc_Double); //-V523
		}
		else {
			if (ANYBIT (dbflags, Circle_LINEAR_ARC))
				FREE(single_data, Circular_Arc_Linear_Single);
			else if (ANYBIT (dbflags, Circle_LIGHTWEIGHT))
				FREE(single_data, Circular_Arc_Lite_Single);
			else
				FREE(single_data, Circular_Arc_Single); //-V523
		}
	}

	Circular_Arc (Circular_Arc const & that) : Geometry_X(that) {

		if (ANYBIT (that.dbflags, DB_DOUBLE_PRECISION)) {
			if (ANYBIT (that.dbflags, Circle_LINEAR_ARC)) {
				Circular_Arc_Linear_Double *	data;
				ALLOC (data, Circular_Arc_Linear_Double);
				*data = *(Circular_Arc_Linear_Double *)that.double_data;
				double_data = (Circular_Arc_Double*)data;
			}
			else if (ANYBIT (that.dbflags, Circle_LIGHTWEIGHT)) {
				Circular_Arc_Lite_Double *	data;
				ALLOC (data, Circular_Arc_Lite_Double);
				*data = *(Circular_Arc_Lite_Double *)that.double_data;
				double_data = (Circular_Arc_Double*)data;
			}
			else {
				Circular_Arc_Double *	data;
				ALLOC (data, Circular_Arc_Double);
				*data = *(Circular_Arc_Double *)that.double_data;
				double_data = (Circular_Arc_Double*)data;
			}		
		}
		else {
			if (ANYBIT (that.dbflags, Circle_LINEAR_ARC)) {
				Circular_Arc_Linear_Single *	data;
				ALLOC (data, Circular_Arc_Linear_Single);
				*data = *(Circular_Arc_Linear_Single *)that.single_data;
				single_data = (Circular_Arc_Single*)data;
			}
			else if (ANYBIT (that.dbflags, Circle_LIGHTWEIGHT)) {
				Circular_Arc_Lite_Single *	data;
				ALLOC (data, Circular_Arc_Lite_Single);
				*data = *(Circular_Arc_Lite_Single *)that.single_data;
				single_data = (Circular_Arc_Single*)data;
			}
			else {
				Circular_Arc_Single *	data;
				ALLOC (data, Circular_Arc_Single);
				*data = *(Circular_Arc_Single *)that.single_data;
				single_data = (Circular_Arc_Single*)data;
			}	
		}
	}

	Geometry * clone() const {return NEW(Circular_Arc)(*this);}

	size_t memory_used(MU_Flags flags) const {
		size_t	size = Geometry_X<Circular_Arc_Data>::memory_used(flags);

		if (ANYBIT(flags, MU_DATABASE)) {
			size += sizeof(Circular_Arc);

			if (ANYBIT (dbflags, DB_DOUBLE_PRECISION)) {
				if (ANYBIT (dbflags, Circle_LINEAR_ARC))
					size += sizeof(Circular_Arc_Linear_Double);
				else if (ANYBIT (dbflags, Circle_LIGHTWEIGHT))
					size += sizeof(Circular_Arc_Lite_Double);
				else
					size += sizeof(Circular_Arc_Double);
			}
			else {
				if (ANYBIT (dbflags, Circle_LINEAR_ARC))
					size += sizeof(Circular_Arc_Linear_Single);
				else if (ANYBIT (dbflags, Circle_LIGHTWEIGHT))
					size += sizeof(Circular_Arc_Lite_Single);
				else
					size += sizeof(Circular_Arc_Single);
			}
		}

		return size;
	}
};

struct Circle : public Geometry_X<Circle_Data> {
public:

	Circle() : Geometry_X(CIRCLE) {}

	~Circle() {
		clean();
	}

	Circle (Circle const & that) : Geometry_X(that) {

		if (ANYBIT (that.dbflags, DB_DOUBLE_PRECISION)) {
			if (ANYBIT (that.dbflags, Circle_LIGHTWEIGHT)) {
				Circle_Lite_Double *	data;
				ALLOC (data, Circle_Lite_Double);
				*data = *(Circle_Lite_Double*)that.double_data;
				double_data = (Circle_Double*)data;
			}
			else {
				Circle_Double *	data;
				ALLOC (data, Circle_Double);
				*data = *(Circle_Double*)that.double_data;
				double_data = (Circle_Double*)data;
			}		
		}
		else {
			if (ANYBIT (that.dbflags, Circle_LIGHTWEIGHT)) {
				Circle_Lite_Single *	data;
				ALLOC (data, Circle_Lite_Single);
				*data = *(Circle_Lite_Single*)that.single_data;
				single_data = (Circle_Single*)data;
			}
			else {
				Circle_Single *	data;
				ALLOC (data, Circle_Single);
				*data = *(Circle_Single*)that.single_data;
				single_data = (Circle_Single*)data;
			}	
		}
	}

	Geometry * clone() const {return NEW(Circle)(*this);}

	size_t memory_used(MU_Flags flags) const {
		size_t	size = Geometry_X<Circle_Data>::memory_used(flags);

		if (ANYBIT(flags, MU_DATABASE)) {
			size += sizeof(Circle);

			if (ANYBIT (dbflags, DB_DOUBLE_PRECISION)) {
				if (ANYBIT (dbflags, Circle_LIGHTWEIGHT))
					size += sizeof(Circle_Lite_Double);
				else
					size += sizeof(Circle_Double);
			}
			else {
				if (ANYBIT (dbflags, Circle_LIGHTWEIGHT))
					size += sizeof(Circle_Lite_Single);
				else
					size += sizeof(Circle_Single);
			}
		}

		return size;
	}

	INLINE void clean () {
		if (ANYBIT (dbflags, DB_DOUBLE_PRECISION)) {
			if (ANYBIT (dbflags, Circle_LIGHTWEIGHT))
				FREE(double_data, Circle_Lite_Double);
			else
				FREE(double_data, Circle_Double); //-V523
			double_data = null;
		}
		else {
			if (ANYBIT (dbflags, Circle_LIGHTWEIGHT))
				FREE(single_data, Circle_Lite_Single);
			else
				FREE(single_data, Circle_Single); //-V523
			single_data = null;
		}

		dbflags &= ~(DB_DOUBLE_PRECISION | Circle_LIGHTWEIGHT);
	}

private:
	Circle const & operator= (Circle const & that);
};


////////////////////////////////////////////////////////////////////////////////


#define Curve_HAS_NEGATIVE_WEIGHTS	DB_TYPE_SPECIFIC(1)


struct NURBS_Curve : public Geometry_X<Point_3D> {
public:
	Spinlock		spinlock;

	float *			weights;	// length is common.control_point_count
	float *			knots;		// length is common.control_point_count + common.degree + 1

	Polyline *		polyline;

	int				degree;
	int				control_point_count;

	float			start_u;
	float			end_u;

	int				checksum;	// determines if any of the settings has changed since polyhedron generation

	NURBS_Curve() : Geometry_X(NURBS_CURVE) {};

	~NURBS_Curve() {
#ifndef DISABLE_NURBS_CURVES
		int	knot_count = control_point_count + degree + 1;

		if (single_data != null) {
			ASSERT(!ANYBIT(dbflags, DB_DOUBLE_PRECISION));
			FREE_ARRAY	(single_data, control_point_count, Point);
		}
		if (double_data != null) {
			ASSERT(ANYBIT(dbflags, DB_DOUBLE_PRECISION));
			FREE_ARRAY	(double_data, control_point_count, DPoint);
		}

		if (knots != null)
			FREE_ARRAY	(knots, knot_count, float);
		if (weights != null)
			FREE_ARRAY	(weights, control_point_count, float);

		if (polyline != null) {
			INUTILE(polyline);
			polyline = null;
		}
#endif
	}

	NURBS_Curve (NURBS_Curve const & that) : Geometry_X (that) {
#ifndef DISABLE_NURBS_CURVES

		degree = that.degree;
		control_point_count = that.control_point_count;
		start_u = that.start_u;
		end_u = that.end_u;

		int knot_count = that.control_point_count + that.degree + 1;

		CLONE_ARRAY(that.single_data, that.control_point_count, Point, single_data);
		CLONE_ARRAY(that.double_data, that.control_point_count, DPoint, double_data);
		CLONE_ARRAY(that.knots, knot_count, float, knots);
		CLONE_ARRAY(that.weights, that.control_point_count, float, weights);

		// maybe this could be copied or shared
		polyline = null;
		checksum = 0;
#endif
	}

	Geometry * clone() const {return NEW(NURBS_Curve)(*this);}

	size_t memory_used(MU_Flags flags) const {
		size_t	size = Geometry_X<Point_3D>::memory_used(flags);

		if (ANYBIT(flags, MU_DATABASE)) {
			size += sizeof(NURBS_Curve);

			int	knot_count = control_point_count + degree + 1;

			if (single_data != null)
				size += control_point_count * sizeof(Point);

			if (double_data != null)
				size += control_point_count * sizeof(DPoint);

			if (knots != null)
				size += knot_count * sizeof(float);

			if (weights != null)
				size += control_point_count * sizeof(float);
		}

		if (ANYBIT(flags, MU_DERIVED)) {
			if (polyline != null)
				size +=	polyline->memory_used(flags);
		}

		return size;
	}

private:
	NURBS_Curve const & operator= (NURBS_Curve const & that);
};

#define Cylinder_Cap_NONE		0x00
#define Cylinder_Cap_FIRST		0x01
#define Cylinder_Cap_SECOND		0x02
#define Cylinder_Cap_BOTH		0x03
#define Cylinder_Cap_MASK		0x03
#define Cylinder_CLOSED			0x04
/* Normals for end alignment on polycylinders */
#define Cylinder_Normal_FIRST	0x10
#define Cylinder_Normal_SECOND	0x20
#define Cylinder_Normal_MASK	0x30


template <typename F>
class Basis_Pair_3D {
	public:
		Vector_3D<F>			axis[2];

		Basis_Pair_3D () {}

		template <typename D>
			explicit Basis_Pair_3D (Basis_Pair_3D<D> const & that) {
				axis[0] = Vector_3D<F>(that.axis[0]);
				axis[1] = Vector_3D<F>(that.axis[1]);
			}
};
typedef Basis_Pair_3D<float>	Basis_Pair;
typedef Basis_Pair_3D<double>	DBasis_Pair;


template <typename F>
struct Cylinder_Data {
	Point_3D<F>					points[2];
	Basis_Pair_3D<F>			basis;
	F							radius;
};
typedef Cylinder_Data<float>	Cylinder_Single;
typedef Cylinder_Data<double>	Cylinder_Double;


struct Cylinder : public Geometry_X<Cylinder_Data> {
public:
	int			flags;

	Cylinder() : Geometry_X(CYLINDER) {}

	~Cylinder();

	Cylinder (Cylinder const & that);

	Geometry * clone() const {return NEW(Cylinder)(*this);}

	size_t memory_used(MU_Flags mu_flags) const {
		size_t	size = Geometry_X<Cylinder_Data>::memory_used(mu_flags);

		if (ANYBIT(mu_flags, MU_DATABASE)) {
			size += sizeof(Cylinder);

			if (ANYBIT (dbflags, DB_DOUBLE_PRECISION))
				size += sizeof(Cylinder_Double);
			else
				size += sizeof(Cylinder_Single);
		}

		return size;
	}
};

typedef unsigned char PCA_Vertex_Flags;
#define PCA_EXPLICIT_ECOLOR_BY_VALUE	 (PCA_Vertex_Flags)0x01
#define PCA_EXPLICIT_ECOLOR_BY_FINDEX	 (PCA_Vertex_Flags)0x02
#define PCA_ANY_EXPLICIT_ECOLOR			 (PCA_Vertex_Flags)0x03
#define PCA_EXPLICIT_FCOLOR_BY_VALUE	 (PCA_Vertex_Flags)0x04
#define PCA_EXPLICIT_FCOLOR_BY_FINDEX	 (PCA_Vertex_Flags)0x08
#define PCA_ANY_EXPLICIT_FCOLOR			 (PCA_Vertex_Flags)0x0C
#define PCA_ANY_EXPLICIT_COLOR_BY_VALUE	 (PCA_Vertex_Flags)0x05
#define PCA_ANY_EXPLICIT_COLOR_BY_FINDEX (PCA_Vertex_Flags)0x0A
#define PCA_ANY_EXPLICIT_COLOR			 (PCA_Vertex_Flags)0x0F

struct PolyCylinder_Attributes {
	/* All arrays of size == point_count */
	PCA_Vertex_Flags *		flags;
	RGB *					fcolors;
	RGB *					ecolors;
	Color_Float_Index *		ffindices;
	Color_Float_Index *		efindices;
	int						explicit_fcolor_by_value_count;
	int						explicit_fcolor_by_findex_count;
	int						explicit_ecolor_by_value_count;
	int						explicit_ecolor_by_findex_count;
};


template <typename F>
struct PolyCylinder_Data {
	Point_3D<F> *		points;
	F *					radii;
	Basis_Pair_3D<F> *	basis;
	float *				twists;
	Vector				end_normals[2];
	F					maximum_displacement;
};
typedef PolyCylinder_Data<float>	PolyCylinder_Single;
typedef PolyCylinder_Data<double>	PolyCylinder_Double;


struct PolyCylinder : public Geometry_X<PolyCylinder_Data> {
public:
	Spinlock					spinlock;	// due to display lists

	PolyCylinder_Attributes	*	local_attributes;
	Display_List *				tristrip_display_list;
	Display_List *				polyedge_display_list;
	Bounding					bounding;

	int							point_count;
	int							radius_count;
	int							flags;
	int							display_list_resolution;

	PolyCylinder() : Geometry_X(POLYCYLINDER) {}

	~PolyCylinder();

	PolyCylinder (PolyCylinder const & that);

	Geometry * clone() const {return NEW(PolyCylinder)(*this);}

	size_t memory_used(MU_Flags mu_flags) const {
		size_t	size = Geometry_X<PolyCylinder_Data>::memory_used(mu_flags);

		if (ANYBIT(mu_flags, MU_DATABASE)) {
			size += sizeof(PolyCylinder);

			if (ANYBIT (dbflags, DB_DOUBLE_PRECISION)) {
				if (point_count > 0) {
					size += point_count * sizeof(DPoint);
					size += point_count * sizeof(DBasis_Pair);
					size += point_count * sizeof(float);
				}
				if (radius_count > 0)
					size += radius_count * sizeof(double);
			}
			else {
				if (point_count > 0) {
					size += point_count * sizeof(Point);
					size += point_count * sizeof(Basis_Pair);
					size += point_count * sizeof(float);
				}
				if (radius_count > 0)
					size += radius_count * sizeof(float);
			}

			if (local_attributes != null) {
				PolyCylinder_Attributes *	pca = local_attributes;

				if (pca->ecolors != null && pca->ecolors != pca->fcolors)
					size += point_count * sizeof(RGB);
				if (pca->fcolors != null)
					size += point_count * sizeof(RGB);
				if (pca->efindices != null && pca->efindices != pca->ffindices)
					size += point_count * sizeof(Color_Float_Index);
				if (pca->ffindices != null)
					size += point_count * sizeof(Color_Float_Index);
				if (pca->flags != null)
					size += point_count * sizeof(PCA_Vertex_Flags);

				size += sizeof(PolyCylinder_Attributes);
			}
		}

		return size;
	}

	void invalidate_display_lists(Actor const * actor = null, bool need_locking = true) const;

private:
	PolyCylinder const & operator= (PolyCylinder const & that);
};



template <typename F>
struct Sphere_Data {
	Sphere_3D<F>				data;
	Vector_3D<F> *				basis;	 // if non-null, allocated as array of 3 vectors
};
typedef Sphere_Data<float>	Sphere_Single;
typedef Sphere_Data<double>	Sphere_Double;

struct Sphere : public Geometry_X<Sphere_Data> {
	Sphere() : Geometry_X(SPHERE) {}

	~Sphere() {
#ifndef DISABLE_SPHERE
		if (ANYBIT (dbflags, DB_DOUBLE_PRECISION)) {
			if (double_data->basis != null)
				FREE_ARRAY (double_data->basis, 3, DVector);
			FREE (double_data, Sphere_Double);
		}
		else {
			if (single_data->basis != null)
				FREE_ARRAY (single_data->basis, 3, Vector);
			FREE (single_data, Sphere_Single);
		}
#endif
	}

	Sphere (Sphere const & that) : Geometry_X (that) {
#ifndef DISABLE_SPHERE
		if (ANYBIT (that.dbflags, DB_DOUBLE_PRECISION)) {
			ALLOC (double_data, Sphere_Double);
			*double_data = *that.double_data;
			if (that.double_data->basis != null) {
				ALLOC_ARRAY (double_data->basis, 3, DVector);
				COPY_ARRAY (that.double_data->basis, 3, DVector, double_data->basis);
			}
		}
		else {
			ALLOC (single_data, Sphere_Single);
			*single_data = *that.single_data;
			if (that.single_data->basis != null) {
				ALLOC_ARRAY (single_data->basis, 3, Vector);
				COPY_ARRAY (that.single_data->basis, 3, Vector, single_data->basis);
			}
		}
#endif
	}

	Geometry * clone() const {return NEW(Sphere)(*this);}

	size_t memory_used(MU_Flags flags) const {
		size_t	size = Geometry_X<Sphere_Data>::memory_used(flags);

		if (ANYBIT(flags, MU_DATABASE)) {
			size += sizeof(Sphere);

			if (ANYBIT (dbflags, DB_DOUBLE_PRECISION)) {
				if (double_data->basis != null)
					size +=  3 * sizeof(DVector);
			}
			else {
				if (single_data->basis != null)
					size +=  3 * sizeof(Vector);
			}
		}

		return size;
	}

private:
	Sphere const & operator= (Sphere const & that);
};



#define Marker_BASIS_PAIR		DB_TYPE_SPECIFIC(1)

struct Marker : public Geometry_X<Point_3D> {
public:
	int			count;
	int			allocated;

	Vector *	orientation;

	Marker() : Geometry_X(MARKER) {}

	~Marker();

	Marker (Marker const & that);

	Geometry * clone() const {return NEW(Marker)(*this);}

	size_t memory_used(MU_Flags flags) const {
		size_t	size = Geometry_X<Point_3D>::memory_used(flags);

		if (ANYBIT(flags, MU_DATABASE)) {
			size += sizeof(Marker);

			if (single_data != null)
				size += allocated * sizeof(Point);
			if (double_data != null)
				size += allocated * sizeof(DPoint);
			if (orientation != null) {
				if (ANYBIT(dbflags, Marker_BASIS_PAIR))
					size += 2 * sizeof(Vector);
				else
					size += sizeof(Vector);
			}
		}

		return size;
	}

	INLINE void Set_Whereabouts(Point const & point)
	{
		dbflags &= ~DB_DOUBLE_PRECISION;
		if (double_data != null) {
			FREE(double_data, DPoint);
			double_data = null;
		}
		if (single_data == null)
			ALLOC(single_data, Point);
		*single_data = point;
	}

	INLINE void Set_Whereabouts(DPoint const & point)
	{
		dbflags |= DB_DOUBLE_PRECISION;
		if (single_data != null) {
			FREE(single_data, Point);
			single_data = null;
		}
		if (double_data == null)
			ALLOC(double_data, DPoint);
		*double_data = point;
	}

private:
	Marker const & operator= (Marker const & that);
};


struct Multimarker : public Geometry_X<Point_3D> {
public:
	int			count;
	int			allocated;

	intptr_t *	mapping;

	Multimarker() : Geometry_X(MULTIMARKER) {}
	Multimarker (Multimarker const & that);
	~Multimarker();

	Geometry * clone() const {return NEW(Multimarker)(*this);}
	void fixup();

	size_t memory_used(MU_Flags flags) const {
		size_t	size = Geometry_X<Point_3D>::memory_used(flags);

		if (ANYBIT(flags, MU_DATABASE)) {
			size += sizeof(Polyline);

			if (allocated > 0) {
				if (single_data != null)
					size += allocated * sizeof(Point);
				if (double_data != null)
					size += allocated * sizeof(DPoint);
			}
			if (mapping != null)
				size += allocated * sizeof(intptr_t);
		}

		if (ANYBIT(flags, MU_AMORTIZE))
			size = (size_t)((float)size / count);

		return size;
	}

private:
	Multimarker const & operator= (Multimarker const & that);
};

typedef short	Light_Flags;
#	define		LF_CAMERA_RELATIVE		(Light_Flags)0x0001
#	define		LF_INNER				(Light_Flags)0x0002 /* relevant to spot lights only */
#	define		LF_OUTER				(Light_Flags)0x0004 /* relevant to spot lights only */
#	define		LF_CONCENTRATION		(Light_Flags)0x0008 /* relevant to spot lights only */
#	define		LF_BRIGHT_SIDE			(Light_Flags)0x0010 /* relevant to area lights only */
#	define		LF_FRONT				(Light_Flags)0x0020 /* relevant to area lights only */
#	define		LF_BACK					(Light_Flags)0x0040 /* relevant to area lights only */
#	define		LF_BOTH					(Light_Flags)0x0080 /* relevant to area lights only */

typedef char	Spot_Light_Size_Units;
#	define		LSU_NOT_VALID	(Spot_Light_Size_Units)0
#	define		LSU_FRU			(Spot_Light_Size_Units)1
#	define		LSU_DEGREES		(Spot_Light_Size_Units)2
#	define		LSU_PERCENT		(Spot_Light_Size_Units)3


template <template <typename> class FD>
struct Light_Base : public Geometry_X<FD> {
public:
	typedef Light_Base<FD> LB;

	uint32_t		generation;
	Light_Flags		flags;

	Light_Base(DB_Type in_xtype) : LB::Geometry_X(LIGHT) {
		LB::xtype = in_xtype;
	}

	~Light_Base() {}

	Light_Base (Light_Base const & that) : LB::Geometry_X(that) {
		LB::xtype = that.xtype;
		generation = 0;
		flags = that.flags;
	}

private:
	Light_Base();
};

struct Light : public Light_Base<No_Data> {
private:
	Light();
	~Light();
	Light (Light const & that);
};

struct Distant_Light : public Light_Base<No_Data> {
public:
	Vector			original;
	Vector			direction;

	Distant_Light() : Light_Base(Light_DISTANT) {}

	~Distant_Light() {}

	Distant_Light (Distant_Light const & that) : Light_Base<No_Data> (that) {
		original = that.original;
		direction = that.original;
	}

	Geometry * clone() const {return NEW(Distant_Light)(*this);}

	size_t memory_used(MU_Flags mu_flags) const {
		size_t	size = Light_Base<No_Data>::memory_used(mu_flags);

		if (ANYBIT(mu_flags, MU_DATABASE)) {
			size += sizeof(Distant_Light);
		}

		return size;
	}
};

struct Point_Light : public Light_Base<Point_3D> {
public:
	Point_Light() : Light_Base(Light_POINT) {}

	~Point_Light() {
		if (ANYBIT (dbflags, DB_DOUBLE_PRECISION))
			FREE (double_data, DPoint);
		else
			FREE (single_data, Point);
	}

	Point_Light (Point_Light const & that) : Light_Base<Point_3D> (that) {
		if (ANYBIT (that.dbflags, DB_DOUBLE_PRECISION)) {
			ALLOC (double_data, DPoint);
			*double_data = *that.double_data;
		}
		else {
			ALLOC (single_data, Point);
			*single_data = *that.single_data;
		}
	}

	Geometry * clone() const {return NEW(Point_Light)(*this);}

	size_t memory_used(MU_Flags mu_flags) const {
		size_t	size = Light_Base<Point_3D>::memory_used(mu_flags);

		if (ANYBIT(mu_flags, MU_DATABASE)) {
			size += sizeof(Point_Light);

			if (ANYBIT (dbflags, DB_DOUBLE_PRECISION))
				size += sizeof(DPoint);
			else
				size += sizeof(Point);
		}

		return size;
	}

	INLINE void Set_Position(Point const & point) {
		dbflags &= ~DB_DOUBLE_PRECISION;
		if (double_data != null) {
			FREE(double_data, DPoint);
			double_data = null;
		}
		if (single_data == null)
			ALLOC(single_data, Point);
		*single_data = point;
	}

	INLINE void Set_Position(DPoint const & point) {
		dbflags |= DB_DOUBLE_PRECISION;
		if (single_data != null) {
			FREE(single_data, Point);
			single_data = null;
		}
		if (double_data == null)
			ALLOC(double_data, DPoint);
		*double_data = point;
	}
};

struct Area_Light : public Light_Base<Point_3D> {
public:
	int				count;
	Light_Flags		bright_side;

	Area_Light() : Light_Base(Light_AREA) {}

	~Area_Light() {
		if (count != 0) {
			if (ANYBIT (dbflags, DB_DOUBLE_PRECISION))
				FREE_ARRAY (double_data, count, DPoint);
			else
				FREE_ARRAY (single_data, count, Point);
		}
	}

	Area_Light (Area_Light const & that) : Light_Base<Point_3D> (that) {
		count = that.count;
		bright_side = that.bright_side;

		if (ANYBIT (that.dbflags, DB_DOUBLE_PRECISION))
			CLONE_ARRAY (that.double_data, that.count, DPoint, double_data);
		else
			CLONE_ARRAY (that.single_data, that.count, Point, single_data);
	}

	Geometry * clone() const {return NEW(Area_Light)(*this);}

	size_t memory_used(MU_Flags mu_flags) const {
		size_t	size = Light_Base<Point_3D>::memory_used(mu_flags);

		if (ANYBIT(mu_flags, MU_DATABASE)) {
			size += sizeof(Area_Light);

			if (count != 0) {
				if (ANYBIT (dbflags, DB_DOUBLE_PRECISION))
					size += count * sizeof(DPoint);
				else
					size += count * sizeof(Point);
			}
		}

		return size;
	}

	INLINE bool Set_Points(int in_count, Point const * in_points) {
		dbflags &= ~DB_DOUBLE_PRECISION;
		if (double_data != null) {
			FREE_ARRAY(double_data, count, DPoint);
			double_data = null;
		}
		if (single_data != null) {
			FREE_ARRAY(single_data, count, Point);
			single_data = null;
		}

		count = in_count;

		bool z_is_zero = true;

		if (count > 0) {
			ALLOC_ARRAY(single_data, count, Point);

			Point *	points = single_data;
			do {
				if (in_points->z != 0)
					z_is_zero = false;
				*points++ = *in_points++;
			} while (--in_count > 0);
		}

		return z_is_zero;
	}

	INLINE bool Set_Points(int in_count, DPoint const *	in_points) {
		dbflags |= DB_DOUBLE_PRECISION;
		if (double_data != null) {
			FREE_ARRAY(double_data, count, DPoint);
			double_data = null;
		}
		if (single_data != null) {
			FREE_ARRAY(single_data, count, Point);
			single_data = null;
		}

		count = in_count;

		bool z_is_zero = true;

		if (count > 0) {
			ALLOC_ARRAY(double_data, count, DPoint);

			DPoint *	points = double_data;
			do {
				if (in_points->z != 0)
					z_is_zero = false;
				*points++ = *in_points++;
			} while (--in_count > 0);
		}

		return z_is_zero;
	}
};


template <typename F>
struct Spot_Light_Data {
	Point_3D<F>				position;
	Point_3D<F>				target;
	float					outer;
	float					inner;
	float					concentration;
	float					final_inner;
	float					final_outer;
	Spot_Light_Size_Units	outer_units;
	Spot_Light_Size_Units	inner_units;
};
typedef Spot_Light_Data<float> Spot_Light_Single;
typedef Spot_Light_Data<double> Spot_Light_Double;

struct Spot_Light : public Light_Base<Spot_Light_Data> {
public:

	Spot_Light() : Light_Base(Light_SPOT) {}

	~Spot_Light() {
		if (ANYBIT (dbflags, DB_DOUBLE_PRECISION))
			FREE (double_data, Spot_Light_Double);
		else
			FREE (single_data, Spot_Light_Single);
	}

	Spot_Light (Spot_Light const & that) : Light_Base<Spot_Light_Data> (that) {
		if (ANYBIT (that.dbflags, DB_DOUBLE_PRECISION)) {
			ALLOC (double_data, Spot_Light_Double);
			*double_data = *that.double_data;
		}
		else {
			ALLOC (single_data, Spot_Light_Single);
			*single_data = *that.single_data;
		}
	}

	Geometry * clone() const {return NEW(Spot_Light)(*this);}

	size_t memory_used(MU_Flags mu_flags) const {
		size_t	size = Light_Base<Spot_Light_Data>::memory_used(mu_flags);

		if (ANYBIT(mu_flags, MU_DATABASE)) {
			size += sizeof(Spot_Light);

			if (ANYBIT (dbflags, DB_DOUBLE_PRECISION))
				size += sizeof(Spot_Light_Double);
			else
				size += sizeof(Spot_Light_Single);
		}

		return size;
	}

	INLINE void Set_Data(
		Point const &			position,
		Point const &			target,
		float					inner,
		float					outer,
		float					concentration,
		Spot_Light_Size_Units	inner_units,
		Spot_Light_Size_Units	outer_units,
		float					final_outer,
		float					final_inner) {

		dbflags &= ~DB_DOUBLE_PRECISION;

		if (double_data != null) {
			FREE(double_data, Spot_Light_Double);
			double_data = null;
		}

		if (single_data == null)
			ALLOC(single_data, Spot_Light_Single);

		single_data->position = position;
		single_data->target = target;

		single_data->outer = outer;
		single_data->inner = inner;
		single_data->outer_units = outer_units;
		single_data->inner_units = inner_units;
		single_data->concentration = concentration;
		single_data->final_outer = final_outer;
		single_data->final_inner = final_inner;
	}

	INLINE void Set_Data(
		DPoint const &			position,
		DPoint const &			target,
		float					inner,
		float					outer,
		float					concentration,
		Spot_Light_Size_Units	inner_units,
		Spot_Light_Size_Units	outer_units,
		float					final_outer,
		float					final_inner) {

		dbflags |= DB_DOUBLE_PRECISION;

		if (single_data != null) {
			FREE(single_data, Spot_Light_Single);
			single_data = null;
		}

		if (double_data == null)
			ALLOC(double_data, Spot_Light_Double);

		double_data->position = position;
		double_data->target = target;

		double_data->outer = outer;
		double_data->inner = inner;
		double_data->outer_units = outer_units;
		double_data->inner_units = inner_units;
		double_data->concentration = concentration;
		double_data->final_outer = final_outer;
		double_data->final_inner = final_inner;
	}
};


struct String_Cursor : public Geometry {	// should never occur in a general geometry list, so don't need generic
public:
	int			row;
	int			column;

	String_Cursor() : Geometry(STRING_CURSOR) {}

	~String_Cursor() {}

	String_Cursor (String_Cursor const & that) : Geometry(that) {
		row = that.row;
		column = that.column;
	}

	Geometry * clone() const {return NEW(String_Cursor)(*this);}

	size_t memory_used(MU_Flags flags) const {
		size_t	size = Geometry::memory_used(flags);

		if (ANYBIT(flags, MU_DATABASE)) {
			size += sizeof(String_Cursor);
		}

		return size;
	}
};

class Font_Name : public CMO {
public:
	Font_Name() : next(null), generic(0), specified(null) {}

	Font_Name(Font_Name const & that) {
		if (that.next != null)
			next = NEW(Font_Name)(*that.next);

		generic = that.generic;
		specified = that.specified;
	};

	~Font_Name() {
		delete next;
	}

	Font_Name & operator= (Font_Name const & that) {
		if (that.next != null)
			next = NEW(Font_Name)(*that.next);

		generic = that.generic;
		specified = that.specified;
		return *this;
	};

	Font_Name *	next;

	// (generic font name values (GFN_XXX) are defined in Hoops.h, for driver convenience)
	Generic_Font_Name	generic;
	Name				specified;
};

struct Font_Names_Data : public Counted_Object_Interlocked {
	Font_Name *			list;

	Font_Names_Data () : Counted_Object_Interlocked () {}
	~Font_Names_Data () {
		delete list;
	}

private:
	Font_Names_Data (Font_Names_Data const &);
	Font_Names_Data const & operator= (Font_Names_Data const & that);
};
typedef Counted_Pointer<Font_Names_Data>	Font_Name_List;



struct Text_Region {
	int						count;
	Point					points[4];		/* 2, 3, or 4 */
#define Text_Region_WINDOW		0x0001	/* default object */
#define Text_Region_RELATIVE	0x0002	/* default absolute */
#define Text_Region_ADJUST		0x0004	/* default maintain direction */

#define Text_Region_BOTTOM		0x0000
#define Text_Region_CENTER		0x0010
#define Text_Region_TOP			0x0020
#define Text_Region_ALIGNMENT	0x0030	/* id & mask */

#define	Text_Region_HFIT		0x0040
#define	Text_Region_VFIT		0x0080
#define	Text_Region_FITTING		0x00C0
	int						options;

#define Text_Region_Fit_NONE		0
#define Text_Region_Fit_SPACING		1
#define Text_Region_Fit_SIZE		2
	unsigned char			hfit;
	unsigned char			vfit;
};


typedef int32_t KAttribute_Flags;

#define KA_NAME					(KAttribute_Flags)0x00000001
#define KA_SIZE					(KAttribute_Flags)0x00000002
#define KA_VERTICAL_OFFSET		(KAttribute_Flags)0x00000004
#define KA_OMITTED				(KAttribute_Flags)0x00000008
#define KA_INVISIBLE			(KAttribute_Flags)0x00000010
#define KA_SLANT				(KAttribute_Flags)0x00000020
#define KA_WIDTH_SCALE			(KAttribute_Flags)0x00000040
#define KA_ROTATION				(KAttribute_Flags)0x00000080
#define KA_ROTATION_FIXED		(KAttribute_Flags)0x00000100
#define KA_COLOR				(KAttribute_Flags)0x00000200
#define KA_HORIZONTAL_OFFSET	(KAttribute_Flags)0x00000400

struct KAttribute {
	Font_Name_List			names;

	RGB						color;
	float					size;
	float					horizontal_offset;
	float					vertical_offset;
	float					slant;
	float					rotation;
	float					width_scale;

	KAttribute_Flags		flags_mask;
	KAttribute_Flags		flags_value;

	Font_Size_Units			size_units;
	Font_Size_Units			vertical_offset_units;
	Font_Size_Units			horizontal_offset_units;
};

struct Text : public Geometry_X<Point_3D> {
public:
	Karacter *					karacters;
	KAttribute *				kattribute;
	int *						counts;
	Karacter **					starts;
	String_Cursor *				cursors;
	Text_Region *				region;

	Bounding					bounding;
	uint32_t					bounding_generation;

	int							count;
	int							lines;
	int							kattribute_count;

	Text_Encoding				encoding;

	Text() : Geometry_X(TEXT) {}

	~Text();

	Text (Text const & that);

	Geometry * clone() const {return NEW(Text)(*this);}

	size_t memory_used(MU_Flags flags) const {
		size_t	size = Geometry_X<Point_3D>::memory_used(flags);

		if (ANYBIT(flags, MU_DATABASE)) {
			size += sizeof(Text);

			if (single_data != null)
				size += sizeof(Point);
			if (double_data != null)
				size += sizeof(DPoint);

			if (karacters != null)
				size += (count+1) * sizeof(Karacter);

			if (lines != 0) {
				if (counts != null)
					size += lines * sizeof(int);
				if (starts != null)
					size += lines * sizeof(Karacter *);
			}

			if (region != null)
				size += sizeof(Text_Region);

			if (kattribute != null) {
				// XXX too much work
				//for (int i=0; i<count; i++)
				//	size += kattribute[i].names;

				size += count * sizeof(KAttribute);
			}
		}

		return size;
	}

	INLINE void Set_Whereabouts(Point const & point) {
		dbflags &= ~DB_DOUBLE_PRECISION;
		if (double_data != null) {
			FREE(double_data, DPoint);
			double_data = null;
		}
		if (single_data == null)
			ALLOC(single_data, Point);
		*single_data = point;
	}

	INLINE void Set_Whereabouts(DPoint const & point) {
		dbflags |= DB_DOUBLE_PRECISION;
		if (single_data != null) {
			FREE(single_data, Point);
			single_data = null;
		}
		if (double_data == null)
			ALLOC(double_data, DPoint);
		*double_data = point;
	}

private:
	Text const & operator= (Text const & that);
};






#define Tint_ACTIVE			0x0001			// apply any specified options
#define Tint_ADJUST			0x0002			// scale & translate values
#define Tint_COLOR			0x0004			// modulation color
#define Tint_EFFECT			0x0008			// use of modulation color
#define	Tint_TRANSPARENCY	0x0010			// override alpha

// note that gray bit is used in several of these "enum" values
#define Tint_Effect_GRAYSCALE				0x0001
#define Tint_Effect_MODULATE				0x0002
#define Tint_Effect_MODULATE_GRAYSCALE		0x0003
#define Tint_Effect_TONE					0x0005
#define Tint_Effect_REPLACE					0x0008

struct Tinting {
	int				options;
	int				effect;
	float			scale;
	float			translate;
	RGB				color;
	float			transparency;

	Tinting () : options(0), effect(0), scale(0), translate(0), color(), transparency(0) {}

	bool	operator== (Tinting const & that) const {
		return	options		==	that.options	&&
				effect		==	that.effect		&&
				scale		==	that.scale		&&
				translate	==	that.translate	&&
				color		==	that.color		&&
				transparency==	that.transparency;
	}
	bool	operator!= (Tinting const & that) const { return  !(*this == that); }
};



struct Named_Style_Tracker_Node : public CMO {
	Named_Style_Tracker_Node() : style_segments(), highlight_segments() {}
	~Named_Style_Tracker_Node() {}

	INLINE bool remove_style_segment(Name const & name, Segment const * segment) {
		auto segment_to_erase = std::find(style_segments.begin(), style_segments.end(), segment);
		ASSERT(segment_to_erase != style_segments.cend());
		style_segments.erase(segment_to_erase);

		HOOPS::WORLD->flags |= World::NAMED_STYLES_DIRTY;

		ASSERT(!ANYBIT (HOOPS::WORLD->system_flags, Sysop_THREADING) ||
			HOOPS::WORLD->hash_mutex->Owner() == THREAD_ID()); // caller must have

		if (!ANYBIT (segment->dbflags, DB_DELETED)) {
			UTILE(segment);
			HOOPS::WORLD->removed_named_style_segments->push_back(segment);
		}

		if (style_segments.size() == 0 && highlight_segments.size() == 0) {
			HOOPS::WORLD->named_style_tracker->erase(name);
			delete this;
			return true;
		}

		return false;
	}

	INLINE bool remove_highlight_segment(Name const & name, Segment const * segment, bool was_quickmoved) {
		auto segment_to_erase = std::find(highlight_segments.begin(), highlight_segments.end(), segment);
		ASSERT(segment_to_erase != highlight_segments.cend());
		highlight_segments.erase(segment_to_erase);

		ASSERT(!ANYBIT (HOOPS::WORLD->system_flags, Sysop_THREADING) ||
			HOOPS::WORLD->hash_mutex->Owner() == THREAD_ID()); // caller must have

		if (!was_quickmoved) {
			HOOPS::WORLD->flags |= World::NAMED_STYLES_DIRTY;
			if (!ANYBIT (segment->dbflags, DB_DELETED)) {
				UTILE(segment);
				HOOPS::WORLD->removed_named_style_segments->push_back(segment);
			}
		}

		if (style_segments.size() == 0 && highlight_segments.size() == 0) {
			HOOPS::WORLD->named_style_tracker->erase(name);
			delete this;
			return true;
		}

		return false;
	}

	Segment_List	style_segments;
	Segment_List	highlight_segments;
};

struct Index_Array {
	int *				indices;
	int					size;
	int					count;

	Index_Array (int initial=0) : indices (0), size (initial), count (0) {
		if (size > 0)
			ALLOC_ARRAY (indices, size, int);
	}

	~Index_Array() {
		if (size > 0)
			FREE_ARRAY (indices, size, int);
	}

	int operator [] (int index) const {
		if (index < count)
			return indices[index];
		return -1;
	}

	void resize (int new_size) {
		if (new_size > size) {
			int *		old_indices = indices;
			ALLOC_ARRAY (indices, new_size, int);
			if (old_indices != null) {
				COPY_ARRAY (old_indices, size, int, indices);
				FREE_ARRAY (old_indices, size, int);
			}
			size = new_size;
		}
	}

	bool insert (int value) {
		for (int i=0; i<count; ++i) {	// should do binary search
			if (indices[i] == value)
				return false;
			if (indices[i] > value) {
				if (count >= size)
					resize (size+4);
				for (int j=count; j>i; --j)
					indices[j] = indices[j-1];

				indices[i] = value;
				++count;
				return true;
			}
		}
		if (count >= size)
			resize (size+4);
		indices[count++] = value;
		return true;
	}

	bool remove (int value) {
		for (int i=0; i<count; ++i) {	// should do binary search
			if (indices[i] == value) {
				--count;
				while (i < count) {
					indices[i] = indices[i+1];
					++i;
				}
				return true;
			}
			if (indices[i] > value)
				break;
		}
		return false;
	}
};


struct Highlight_Subentity : public CMO {
	Highlight_Subentity *	next;

	Index_Array				faces;
	Index_Array				edges;
	Index_Array				vertices;
	Index_Array				isolines;

	Name					style;
};

struct Highlight_Geometry : public CMO {
	Highlight_Geometry *	next;
	Name					style_name;			// if a geometry-level highlight
	Highlight_Subentity *	subentities;
	int						flags;

	~Highlight_Geometry() {
		Highlight_Subentity *	hse;
		while ((hse = subentities) != null) {
			subentities = hse->next;
			delete hse;
		}
	}
};


INLINE static Key Get_Highlight_Key(Thread_Data * thread_data, Key key, Geometry const * geo)
{
	Key key_to_use = key;

	if (key_to_use >= 0) {
		ASSERT(geo->owner != null);
		int			offset;
		Anything *	thing = (Anything *)Key_To_Pointer (thread_data, key_to_use, &offset, geo->owner);
		key_to_use = POINTER_TO_KEY (thing);
	}

	return key_to_use;
}



struct Quickmoved_Segment : public CMO {
	Segment const *			segment;
	size_t					overlay_count;
	size_t					spriting_count;
	size_t					inplace_count;

	INLINE Quickmoved_Segment(Segment const * segment) : segment(segment), overlay_count(0), spriting_count(0), inplace_count(0) {};
	INLINE ~Quickmoved_Segment() {};
};


#define HF_UNDEF_EXACT					0x00000001
#define HF_UNDEF_ALL_PATHS				0x00000002
#define HF_UNDEF_SUB_SPECIFIC			0x00000004
#define HF_UNDEF_QUICKMOVED				0x00000008
#define HF_UNDEF_GEOMETRY				0x00000010
#define HF_UNDEF_SEGMENT				0x00000020
#define HF_UNDEF_SUBENTITY				0x00000040
#define HF_UNDEF_EVERYTHING				0x00000070
#define HF_UNDEF_ONLY_QUICKMOVED		0x10000000
#define HF_UNDEF_ONLY_NON_QUICKMOVED	0x20000000

#define	HF_Segment_Only		0x00000001
#define	HF_Wildcard_Path	0x00000002
#define	HF_OVERLAY			0x00000010
#define	HF_SPRITING			0x00000020

#define HF_QUICK_MOVE_MASK	(HF_OVERLAY|HF_SPRITING)

struct Highlight_Path : public CMO {
	Include_Path				path;
	Name						style_name;	// if a segment-level highlight
	Destructible<Geometry_Highlight_Map>::unique_ptr	geometries;
	int							flags;

	~Highlight_Path() {
		if (geometries != null) {
			for (auto const & it : *geometries) {
				delete it.second;
			}
		}
		geometries = null;
	}

};


typedef std::vector<Highlight_Path *, CMO_Allocator<Highlight_Path *> > Highlight_Path_Vector;
typedef std::vector<Highlight_Path *, POOL_Allocator<Highlight_Path *> > Highlight_Path_Vector_Pooled;

struct Highlight_Segment : public CMO {

	Highlight_Path_Vector	overlay_paths;
	Highlight_Path_Vector	spriting_paths;
	Highlight_Path_Vector	inplace_paths;

	Segment const *			segment;

	Counted_Name_List *		style_name_list;

	INLINE Highlight_Segment(Segment const * segment) : segment(segment), style_name_list(null) {

		ASSERT(!ANYBIT(segment->dbflags, Segment_HIGHLIGHT));
		ASSERT(!ANYBIT(segment->dbflags, Segment_STATIC_TREE));

		if (!ANYBIT(segment->dbflags, Segment_HIGHLIGHT)) {

			Alter(segment)->dbflags |= Segment_HIGHLIGHT;

			Associated_Segs *	as = segment->get_associated_segments();

			if (as != null)
				as->mark_highlight();
		}

	}

	INLINE ~Highlight_Segment() {

		ASSERT(ANYBIT(segment->dbflags, Segment_HIGHLIGHT));

		ASSERT(!ANYBIT(HOOPS::WORLD->system_flags, Sysop_THREADING) ||
			HOOPS::WORLD->hash_mutex->Owner() == THREAD_ID()); // caller must have

		bool was_quickmoved = false;

		if (!overlay_paths.empty()) {
			auto & qh = HOOPS::WORLD->overlay_segment_highlights;
			auto it = qh->find(segment);
			ASSERT(it != qh->end());
			if (it != qh->end()) {
				qh->erase(it);
			}
			was_quickmoved = true;
		}
		if (!spriting_paths.empty()) {
			auto & qh = HOOPS::WORLD->spriting_segment_highlights;
			auto it = qh->find(segment);
			ASSERT(it != qh->end());
			if (it != qh->end()) {
				qh->erase(it);
			}
			was_quickmoved = true;
		}
		if (!inplace_paths.empty()) {
			auto & qh = HOOPS::WORLD->inplace_segment_highlights;
			auto it = qh->find(segment);
			ASSERT(it != qh->end());
			if (it != qh->end()) {
				qh->erase(it);
			}

		}

		auto nst = HOOPS::WORLD->named_style_tracker.get();
		Counted_Name_List *	walker = style_name_list;

		while (walker != null) {

			auto	it = nst->find(walker->name);
			ASSERT(it != nst->end());
			Named_Style_Tracker_Node *	nstn = it->second;
			ASSERT(nstn != null);
			nstn->remove_highlight_segment(walker->name, segment, was_quickmoved);
			walker = walker->next;
		}

		delete style_name_list;

		for (auto hp : overlay_paths)
			delete hp;
		overlay_paths.clear();

		for (auto hp : spriting_paths)
			delete hp;
		spriting_paths.clear();

		for (auto hp : inplace_paths)
			delete hp;
		inplace_paths.clear();
	
		Alter(segment)->dbflags &= ~Segment_HIGHLIGHT;

		Associated_Segs *	as = segment->get_associated_segments();

		if (as != null)
			as->unmark_all_highlights();

	}

	INLINE void record_style_name(Name_Const const & style_name) {
		Counted_Name_List *	walker = style_name_list;

		while (walker != null) {
			if (walker->name == style_name)
				break;
			walker = walker->next;
		}

		if (walker != null) {
			walker->count++;
		}
		else {
			// new named style for this segment -- record it
			Counted_Name_List *	new_name = NEW(Counted_Name_List)();
			new_name->name = style_name;
			new_name->count = 1;
			new_name->next = style_name_list;
			style_name_list = new_name;

			ASSERT(!ANYBIT (HOOPS::WORLD->system_flags, Sysop_THREADING) ||
				HOOPS::WORLD->hash_mutex->Owner() == THREAD_ID()); // caller must have

			auto	it = HOOPS::WORLD->named_style_tracker->find(new_name->name); //-V807
			ASSERT(it != HOOPS::WORLD->named_style_tracker->end());
			Named_Style_Tracker_Node *	nstn = it->second;

			if (nstn == null) {
				nstn = NEW(Named_Style_Tracker_Node)();
				HOOPS::WORLD->named_style_tracker->insert(std::make_pair(Name(style_name), nstn));
			}

			nstn->highlight_segments.push_back(segment);
			HOOPS::WORLD->flags |= World::NAMED_STYLES_DIRTY;
		}
	}
};


enum {
	Highlight_Options_Match_Style		= 0x01,
	Highlight_Options_Match_Quickmoves	= 0x02,
};
struct Highlight_Options {
	Name	style_name;
	int		mask;
	bool	quickmoves;
};

enum {
	Highlight_Result_None			= 0x00,
	Highlight_Result_Highlighted	= 0x01,
	Highlight_Result_Tree_Highlight	= 0x02,
	Highlight_Result_On_Path		= 0x04,
	Highlight_Result_Overlay		= 0x08,
	Highlight_Result_Subentity		= 0x10,
};



///////////////////////////////////////////////////////////////


struct Highlight {
	Highlight() : style(null), flags(0) {}
	Named_Style		style;
	int				flags;

	INLINE bool operator == (Highlight const & that) const {
		return (style == that.style && flags == that.flags);
	};

	INLINE bool operator != (Highlight const & that) const {
		return !(*this == that);
	};

	INLINE bool operator == (Null_Object const &) const {
		return (*this == Highlight());
	};

	INLINE bool operator != (Null_Object const &) const {
		return !(*this == Highlight());
	};

	INLINE Highlight const & operator= (Null_Object const &) {
		*this = Highlight();
		return *this;
	}
};

typedef std::vector<Highlight, POOL_Allocator<Highlight> > Highlight_Vector;

struct Geometry_Details {
	Geometry_Details *		next;
	Highlight_Subentity *	entry;
};

struct Geometry_Highlight {
	Highlight				highlight;
	Geometry_Details *		details;
};






class Near_Limit {
public:
	Near_Limit() {
		local_limit = 0;
		current = &HOOPS::WORLD->camera_near_limit;
	};

	Near_Limit(Near_Limit const & that) {
		*this = that;
	};

	Near_Limit const &	operator= (Near_Limit const & that) {
		local_limit = that.local_limit;
		current = that.current;
		if (that.current != &HOOPS::WORLD->camera_near_limit)
			current = &local_limit;
		return *this;
	};

	bool operator == (Near_Limit const & that) const {

		if (current == that.current)
			return true;

		if (current == &HOOPS::WORLD->camera_near_limit)
			return false;

		if (that.current == &HOOPS::WORLD->camera_near_limit)
			return false;

		if (that.local_limit != that.local_limit)
			return false;

		return true;
	};

	bool operator != (Near_Limit const & that) const {
		return !operator ==(that);
	};

	float				local_limit;
	float const *		current;
};



class Internal_Camera;

// precision-specific camera data
template <typename F>
struct Camera_Data : public CMO {
	// user supplied
	Point_3D<F>			position;
	Point_3D<F>			target;			// may be == position
	F					field_width;
	F					field_height;	// (may be zero)
	// derived
	F					eye_distance;	// may be zero
	F					eye_distance_inverse;	// may be zero

	Camera_Data() : position(0, 0, -5), target(0, 0, 0), field_width(2), field_height(2), eye_distance(5), eye_distance_inverse((F)1/(F)5) {}

	bool operator== (Camera_Data<F> const & that) const {
		return position == that.position && target == that.target &&
				field_width == that.field_width && field_height == that.field_height;
	}
	bool operator!= (Camera_Data<F> const & that) const { return !operator== (that); }

	Matrix_Pointer<F> generate_positioning (Internal_Camera const & common) const;
	Matrix_Pointer<F> generate_projection (Internal_Camera const & common, float stereo_skew = 0.0f) const;
};
typedef	Camera_Data<float>		Camera_Single;
typedef	Camera_Data<double>		Camera_Double;


class Internal_Camera : public Counted_Object_Interlocked {
	public:
		// precision-specific, contains user-supplied and derived data
		Camera_Single *		single_data;
		Camera_Double *		double_data;

		// user supplied
		Vector				up_vector;				// normalized
		int					projection;
		float				obliquity_around_y;
		float				obliquity_around_x;
		Near_Limit			near_limit;

		// derived
		Vector				eye;					// target - position, normalized
		Vector				up_cross_eye;			// normalized
		float				field_width_tangent;	// field / eye_distance; always good
		float				field_height_tangent;
		float				aspect;

		Spinlock			spinlock;				// for validation
		mutable Matrix		positioning_matrix;		// filled in by validate_transform
		mutable Matrix		projection_matrix;		// filled in by validate_transform
		mutable Matrix		complete_matrix;		// filled in by validate_transform
		uint32_t			id;

		Internal_Camera (bool double_precision);
		Internal_Camera (Internal_Camera const & that);
		~Internal_Camera ();

		bool	operator== (Internal_Camera const & that) const {
			if (single_data != null) {
				if (that.single_data == null)
					return false;
				if (*single_data != *that.single_data)
					return false;
			}
			else {
				if (that.double_data == null)
					return false;
				if (*double_data != *that.double_data)
					return false;
			}

			if (up_vector != that.up_vector)
				return false;
			if (projection != that.projection)
				return false;
			if (obliquity_around_y != that.obliquity_around_y)
				return false;
			if (obliquity_around_x != that.obliquity_around_x)
				return false;
			if (near_limit != that.near_limit)
				return false;

			return true;
		}

		Internal_Camera *	Clone () const {
			return NEW(Internal_Camera)(*this);
		}

		void invalidate_transform (Thread_Data const * thread_data) const {
			Spinner			validate_lock (spinlock, thread_data->thread_id);
			positioning_matrix = null;
			projection_matrix = null;
			complete_matrix = null;
		}
		bool validate_transform (Thread_Data const * thread_data, float stereo_skew = 0.0f) const;

	private:
		Internal_Camera const & operator= (Internal_Camera const & that);
};


class Camera_Handle : public Counted_Pointer<Internal_Camera> {
	typedef		Counted_Pointer<Internal_Camera>		CP;
	public:
		Camera_Handle () : CP () {};	// needed because the next one is needed...
		// these don't inherit as needed due to the effective type of the result
		Camera_Handle (Null_Object const &) : CP () {}
		INLINE Camera_Handle const &	operator= (Null_Object const &) {
			CP::release();
			CP::counted_item = 0;
			return *this;
		}

		static Camera_Handle Create (bool double_precision = false) {
			Camera_Handle	temp;
			temp.counted_item = NEW(Internal_Camera)(double_precision);
			temp.initialize();
			return temp;
		}

		INLINE Camera_Handle & Modify () {
			if (CP::counted_item->shared()) {
				Internal_Camera *	temp = CP::counted_item->Clone();
				release();
				CP::counted_item = temp;
				retain();
			}
			return *this;
		}

		INLINE Camera_Handle Copy () const {
			Camera_Handle	temp;
			temp.counted_item = CP::counted_item->Clone();
			temp->retain();
			return temp;
		}

		bool	operator== (Camera_Handle const & that) const {
			if (*this == null && that == null)
				return true;

			if (*this == null || that == null)
				return false;

			if (CP::counted_item == that.CP::counted_item)
				return true;

			return (*CP::counted_item == *that.CP::counted_item);
		}
		bool	operator!= (Camera_Handle const & that) const { return  !(*this == that); }

		INLINE bool operator== (Null_Object const &) const { return counted_item == 0; }
		INLINE bool operator!= (Null_Object const &) const { return counted_item != 0; }
};




class Camera : public Attribute {
public:
	enum Camera_Projection {
		Projection_PERSPECTIVE,
		Projection_ORTHOGRAPHIC,
		Projection_STRETCHED
	};

	Camera_Handle		data;


	Camera (bool double_precision = false);
	Camera (Camera const & that);
	Camera const & operator= (Camera const & that);

	INLINE bool operator == (Camera const & that) const {
		return data == that.data;
	};

	Attribute * clone() const {return NEW(Camera)(*this);};

	bool equal(Attribute const * attr) const {
		if (attr->type != CAMERA)
			return false;
		return *this == *(Camera*)attr;
	}

	HOOPS_API Attribute * merge (Thread_Data * thread_data, Attribute const * attr, void * control=0);

	HOOPS_API Attribute * acquire (Thread_Data * thread_data, Anything * item, Attribute const * old_attribute);

	int hash_key() const {return CAMERA;};

	void invalidate_transform (Thread_Data const * thread_data) const {
		data->invalidate_transform (thread_data);
	}

	bool validate_transform (Thread_Data const * thread_data, float stereo_skew = 0.0f) const {
		return data->validate_transform (thread_data, stereo_skew);
	}

	static Camera * Default(Thread_Data * thread_data);
};







typedef char Param_Source;
#define HK_PS_UNSPECIFIED			(Param_Source)0
#define HK_PS_U						(Param_Source)1
#define HK_PS_UV					(Param_Source)2
#define HK_PS_UVW					(Param_Source)3
#define HK_PS_OBJECT				(Param_Source)4
#define HK_PS_WORLD					(Param_Source)5
#define HK_PS_SURFACE_NORMAL		(Param_Source)6
#define HK_PS_REFLECTION_VECTOR 	(Param_Source)7
#define HK_PS_NATURAL_UV			(Param_Source)8
#define HK_PS_LOCAL_PIXELS			(Param_Source)9
#define HK_PS_OUTER_PIXELS			(Param_Source)10
#define HK_PS_LOCAL_WINDOW			(Param_Source)11
#define HK_PS_OUTER_WINDOW			(Param_Source)12
#define HK_PS_TRANSMISSION_VECTOR	(Param_Source)13
#define HK_PS_SPHERE				(Param_Source)14
#define HK_PS_CYLINDER				(Param_Source)15
#define HK_PS_PHYSICAL_REFLECTION	(Param_Source)16

typedef char Texture_Layout;
#define HK_TL_UNSPECIFIED		(Param_Source)0
#define HK_TL_RECTILINEAR		(Param_Source)1
#define HK_TL_SPHERICAL			(Param_Source)2
#define HK_TL_HEMISPHERICAL		(Param_Source)3
#define HK_TL_CUBIC_FACES		(Param_Source)4

typedef char Tiling_Style;
#define HK_TS_REPEAT			(Tiling_Style)0
#define HK_TS_CLAMP				(Tiling_Style)1
#define HK_TS_MIRROR			(Tiling_Style)2
#define HK_TS_DROP				(Tiling_Style)3

typedef char Filter;
#define HK_FILTER_NONE			(Filter)0
#define HK_FILTER_BILINEAR		(Filter)1
#define HK_FILTER_TRILINEAR		(Filter)2
#define HK_FILTER_MIPMAP		(Filter)3
#define HK_FILTER_SUMED_AREAS	(Filter)4
#define HK_FILTER_GAUSSIAN		(Filter)5
#define HK_FILTER_STOCHASTIC	(Filter)6
#define HK_FILTER_ANISOTROPIC	(Filter)7

typedef char Channel;
#define HK_CHAN_NONE			(Channel)0
#define HK_CHAN_RED				(Channel)1
#define HK_CHAN_GREEN			(Channel)2
#define HK_CHAN_BLUE			(Channel)3
#define HK_CHAN_ALPHA			(Channel)4
#define HK_CHAN_ZERO			(Channel)5
#define HK_CHAN_ONE				(Channel)6
#define HK_CHAN_LUMINANCE		(Channel)7

typedef int32_t Texture_Flags;
#define TF_NONE					(Texture_Flags)0x00000000
#define TF_PARAM_SOURCE			(Texture_Flags)0x00000001
#define TF_PARAM_OFFSET			(Texture_Flags)0x00000002
#define TF_TRANSFORM			(Texture_Flags)0x00000004
#define TF_TILING				(Texture_Flags)0x00000008
#define TF_SOURCE				(Texture_Flags)0x00000010
#define TF_INTERPOLATION_FILTER (Texture_Flags)0x00000020
#define TF_DECIMATION_FILTER	(Texture_Flags)0x00000040
#define TF_RED_CHAN_MAPPING		(Texture_Flags)0x00000080
#define TF_GREEN_CHAN_MAPPING	(Texture_Flags)0x00000100
#define TF_BLUE_CHAN_MAPPING	(Texture_Flags)0x00000200
#define TF_ALPHA_CHAN_MAPPING	(Texture_Flags)0x00000400
#define TF_ANY_CHANNEL_REMAPPED (Texture_Flags)0x00000780
#define TF_VALUE_SCALE			(Texture_Flags)0x00000800
#define TF_CACHING				(Texture_Flags)0x00001000
#define TF_IMPLICIT_DEFINITION	(Texture_Flags)0x00002000
#define TF_TRANSPARENCY			(Texture_Flags)0x00004000
#define TF_TEXTURE_LAYOUT		(Texture_Flags)0x00008000
#define TF_UNEVALUATED			(Texture_Flags)0x00010000
#define TF_MODULATE				(Texture_Flags)0x00020000
#define TF_DOWNSAMPLE			(Texture_Flags)0x00040000
#define TF_DECAL				(Texture_Flags)0x00080000
#define TF_SOURCE_CALLBACK		(Texture_Flags)0x00100000
#define TF_CHECKERBOARD			(Texture_Flags)0x00200000
#define TF_ACTIVATED			(Texture_Flags)0x00400000
#define TF_CUBEMAP				(Texture_Flags)0x00800000
#define TF_TRANSFORM_OVERRIDE	(Texture_Flags)0x01000000
#define TF_CAMERA				(Texture_Flags)0x02000000
#define TF_SHADER_MULTITEXTURE	(Texture_Flags)0x04000000
#define TF_SOURCE_DIMENSIONS	(Texture_Flags)0x08000000
#define TF_TEXTURE3D			(Texture_Flags)0x10000000
#define TF_SOURCE_CALLBACK_DATA	(Texture_Flags)0x20000000

#define TBF_NONE				(Texture_Flags)0x00000000
#define TBF_MODULATE			(Texture_Flags)0x00000001
#define TBF_NO_DOWNSAMPLE		(Texture_Flags)0x00000002
#define TBF_DECAL				(Texture_Flags)0x00000004
#define TBF_NO_CACHING			(Texture_Flags)0x00000008
#define TBF_MODULATE_COLOR		(Texture_Flags)0x00000010
#define TBF_SOURCE_CALLBACK		(Texture_Flags)0x00000020
#define TBF_PIXEL_SHADER		(Texture_Flags)0x00000040
#define TBF_GEOMETRY_SHADER		(Texture_Flags)0x00000080

#define TBF_ANY_SHADER			(Texture_Flags)(TBF_PIXEL_SHADER|TBF_GEOMETRY_SHADER)

/* used in HI_Determine_Image_Format */
struct Image_Options {
	Name				name; /*remember to free this*/
	Image_Format		format;
	bool 				use_bytes;
	bool 				localized;
	bool 				texture;
	Generic_Size 		width_size;
	Generic_Size 		height_size;
	bool				down_sample;
};

#define Image_BY_REFERENCE		DB_TYPE_SPECIFIC(1)

typedef unsigned int Image_Flags;
#define IF_NONE				(Image_Flags)0x00000000
#define IF_TRANSPARENCY		(Image_Flags)0x00000001
#define IF_POWER_OF_TWO		(Image_Flags)0x00000002
#define IF_BORDERED			(Image_Flags)0x00000004
#define IF_MIRRORED			(Image_Flags)0x00000008
#define IF_FORCED_ALPHA		(Image_Flags)0x00000010
#define IF_MIPMAPS			(Image_Flags)0x00000020
#define IF_REMAPPED			(Image_Flags)0x00000040
#define IF_BLOCK			(Image_Flags)0x00000080
#define IF_BEST_QUALITY		(Image_Flags)0x00000100
#define IF_NO_DATA			(Image_Flags)0x00000200
#define IF_WAS_TARGET		(Image_Flags)0x00000400
#define IF_FORCED_BLOCK		(Image_Flags)0x00000800
#define IF_FORCED_GRAY		(Image_Flags)0x00001000
#define IF_INSET			(Image_Flags)0x00002000
#define IF_ANISOTROPIC		(Image_Flags)0x00004000
#define IF_NON_MIPMAPS		(Image_Flags)0x00008000
#define IF_DL_ONLY			(Image_Flags)0x00010000
#define IF_NO_DOWNSAMPLE	(Image_Flags)0x00020000
#define IF_COMPRESSED		(Image_Flags)0x00040000
#define IF_DISCARD			(Image_Flags)0x00080000
#define IF_COMPRESS_ON_LOAD	(Image_Flags)0x00100000
#define IF_NEED_RGBA32		(Image_Flags)0x00200000
#define IF_DECIMATION		(Image_Flags)0x00400000
#define IF_CACHED_MEMORY	(Image_Flags)0x00800000


// determines which bits from image flags a (uncompressed) new image should inherit
// from the (compressed) source image
#define IF_COPY_MASK (		\
	IF_COMPRESS_ON_LOAD |	\
	IF_NO_DOWNSAMPLE |		\
	IF_ANISOTROPIC |		\
	IF_FORCED_GRAY |		\
	IF_MIPMAPS |			\
	IF_MIRRORED |			\
	IF_POWER_OF_TWO)


struct Image_Remapping {
public:
	Channel						red_channel_source;
	Channel						green_channel_source;
	Channel						blue_channel_source;
	Channel						alpha_channel_source;
	float						scale_min;
	float						scale_max;
	float						attenuation;
	RGB							attenuation_color;

	Image_Remapping():
		red_channel_source(0),
		green_channel_source(0),
		blue_channel_source(0),
		alpha_channel_source(0),
		scale_min(0),
		scale_max(0),
		attenuation(0),
		attenuation_color() {;}

	bool	operator== (Image_Remapping const & that) const {
		return	red_channel_source		== 	that.red_channel_source		&&
				green_channel_source	== 	that.green_channel_source	&&
				blue_channel_source		== 	that.blue_channel_source	&&
				alpha_channel_source	== 	that.alpha_channel_source	&&
				scale_min				== 	that.scale_min				&&
				scale_max				== 	that.scale_max				&&
				attenuation				== 	that.attenuation			&&
				attenuation_color		== 	that.attenuation_color;
	}
	bool	operator!= (Image_Remapping const & that) const { return  !(*this == that); }
};

struct Image : public Geometry_X<Point_3D> {
public:

	void const * const *	rasters;
	void const * const *	inverted_rasters;
	void const *			block;

	// texture related stuff
	Display_List *			display_lists;
	Texture	*				texture;
	Image *					next_image;
	Image *					prev_image;

	Name					name;

	Image_Flags				flags;

	int32_t					width;
	int32_t					height;

	int32_t					row_bytes;
	int32_t					compressed_block_size;  //only used for compressed images
	float					compression_quality;    //if less than 1.0, have driver compress on load

	int32_t					transparent_pixels;
	int32_t					forced_alpha;

	Generic_Size			width_size;
	Generic_Size			height_size;

	Image_Remapping			remap;
	Tinting					tint;

	Color_Index				color_max;

	Image_Format			format;
	Image_Format			source_format;
	char					bytes_per_pixel;
	char					source_bytes_per_pixel;

	Image();
	~Image();
	Image (Image const & that);

	Geometry * clone() const {return NEW(Image)(*this);}

	size_t memory_used(MU_Flags mu_flags) const {

		size_t	size = Geometry_X<Point_3D>::memory_used(mu_flags);

		if (ANYBIT(mu_flags, MU_DATABASE)) {
			size += sizeof(Image);

			if (ANYBIT (dbflags, DB_DOUBLE_PRECISION))
				size += sizeof(DPoint);
			else
				size += sizeof(Point);

			if (ANYBIT (dbflags, Image_BY_REFERENCE)) {
;
			}
			else if (block != null) {
				if (ANYBIT (flags, IF_COMPRESSED))
					size += compressed_block_size;
				else if (row_bytes * height > 0)
					size += row_bytes * height;
			}
			else if (!ANYBIT (flags,IF_NO_DATA|IF_DL_ONLY)) {
				size += width * bytes_per_pixel * height;
			}

			if (rasters != null)
				size += height * sizeof(void *);

			if (inverted_rasters != null)
				size += height * sizeof(void *);

			size += name.length();
		}

		if (ANYBIT(flags, MU_DERIVED)) {
			if (next_image != null)
				size += next_image->memory_used(flags);
		}

		return size;
	}

	void Flush_Cache();

	void invalidate_display_lists(Actor const * actor = null, bool need_locking = true) const;

private:
	Image const & operator= (Image const & that);
};


//always owned by Texture, never passed on its own
class Shader : public CMO {
public:
	//enum Shader_Type    shader_type; //always Material_Shader for now
	Name			shader_source;
	int32_t			id;

	Shader() {
		id = locked_increment(&HOOPS::WORLD->global_id);
	};
};

struct Texture : public Minimum_Header<Texture, Anything> {
	Name				name;
	Name_List *			source_names;
	Callback_Name		lookup;
	void  *				lookup_data;
	Image *				image;
	Image *				active_image;
	Texture_Matrix *	direct_transform;
	Segment *			transform_style_segment;
	Texture_Matrix *	transform_cache;
	Texture	**			texture_panels;  //from source_dimensions>1 or cubemap
	Segment *			camera_style_segment;
	Camera_Handle		camera_cache;
	Shader *			shader;
	int					texture_rendition_incarnation; // active only
	Texture_Flags		flags;
	Texture_Flags		bool_flags;
	int					param_offset;
	Param_Source		param_source;
	Texture_Layout		layout;
	Tiling_Style		tiling_style;
	Filter				interpolation_filter;
	Filter				decimation_filter;
	Image_Remapping		remap;
	int					source_dimensions[3];

	INLINE bool operator == (Texture const & that) const {
		return Equal(*this, that);
	}

	static bool Equal(Texture const & con, Texture const & can);
};

class Texture_Defs : public Attribute {

public:
	Texture_Vector	textures;

	Texture_Defs(Texture * texture) : Attribute(TEXTURE_DEFS) {
		textures.push_back(texture);
		texture->owner = (Anything*)this;
		dbflags |= DB_CONTAINER; // we own this texture
	};


	Texture_Defs(Texture_Defs const & that) : Attribute(TEXTURE_DEFS) {
		textures.reserve(that.textures.size());
		std::copy(that.textures.cbegin(), that.textures.cend(), std::back_inserter(textures));
		dbflags &= ~DB_CONTAINER; // we don't own these
	};

	~Texture_Defs() {
		if (!textures.empty() && ANYBIT(dbflags, DB_CONTAINER)) {
			// we own these textures
			for (auto texture: textures) {
				if (texture->image != null && texture->utility == 2) {
					if (texture->image->texture == texture)
						texture->image->texture = null;
					Alter(texture)->owner = null;
					INUTILE(texture->image);
					INUTILE(texture);
					Alter(texture)->image = null;
				}
				HI_Bon_Voyage((Anything *)texture);
			}
		}
	};

	INLINE bool operator == (Texture_Defs const & that) const {
		if (textures.size() != that.textures.size())
			return false;

		// unordered & painful
		for (auto texture : textures) {
			bool	matched = true;
			for (auto it = that.textures.cbegin(), end = that.textures.cend(); it != end; ++it) {
				auto that_texture = *it;
				if (*texture == *that_texture) {
					matched = true;
					break;
				}
			}
			if (!matched)
				return false;
		}
		return true;
	};

	Attribute * clone() const {
		Attribute * the_clone = NEW(Texture_Defs)(*this);
		the_clone->dbflags &= ~DB_CONTAINER; // the clone doesn't own the textures
		return the_clone;
	};

	bool equal(Attribute const * attr) const {
		if (attr->type != TEXTURE_DEFS)
			return false;
		return *this == *(Texture_Defs*)attr;
	};

	Attribute * merge(Thread_Data * thread_data, Attribute const * attr, void * control = 0);

	Attribute * acquire(Thread_Data *	thread_data, Anything * item, Attribute const * old_attribute);

	int hash_key() const {
		return static_cast<int>(textures.size());
	};

	static void Flush(
		Thread_Data *	thread_data,
		Texture_Defs *	defs,
		bool			want_shaders);


	static Texture_Defs * Default(Thread_Data * thread_data);

private:
	Texture_Defs();
};


struct Stencil_Bitmap {
	Stencil_Bitmap *	next;
	unsigned char *		bitmap;
	Texture *			texture;
	unsigned char *		rle;
	unsigned int		rle_n;
	int					width;
	int					height;
	int					advance_x;
	int					advance_y;
	int					x_orig;
	int					y_orig;
	int					x_correct;
	int					y_correct;
	int					bytes_per_row;
	int					bits_per_pixel;
	int					id;
	/* these positioning offsets are used to move the character inward,
	   away from the stencil edges, so nothing is chopped off */
	int					vertical_positioning_offset;
	int					horizontal_positioning_offset;
	float				rotation;
	float				width_scale;
};


template <typename F>
struct Grid_Data {
	int					count1;
	int					count2;
	Point_3D<F>			origin;
	Point_3D<F>			ref1;
	Point_3D<F>			ref2;
	Vector_3D<F>		basis1;
	Vector_3D<F>		basis2;
	Plane_3D<F>			plane;
};
typedef Grid_Data<float>	Grid_Single;
typedef Grid_Data<double>	Grid_Double;


#define Grid_QUADRILATERAL		1
#define Grid_RADIAL				2

struct Grid : public Geometry_X<Grid_Data> {
public:
	int			grid_type;
	int			divisions;	// radial only

	Grid() : Geometry_X(GRID) {}

	~Grid() {

		if (ANYBIT (dbflags, DB_DOUBLE_PRECISION))
			FREE(double_data, Grid_Double);
		else
			FREE(single_data, Grid_Single);
	}

	Grid (Grid const & that) : Geometry_X (that) {
		grid_type = that.grid_type;
		divisions = that.divisions;

		if (ANYBIT (that.dbflags, DB_DOUBLE_PRECISION)) {
			ALLOC (double_data, Grid_Double);
			*double_data = *that.double_data;
		}
		else {
			ALLOC (single_data, Grid_Single);
			*single_data = *that.single_data;
		}
	}

	Geometry * clone() const {return NEW(Grid)(*this);}

	size_t memory_used(MU_Flags flags) const {

		size_t	size = Geometry_X<Grid_Data>::memory_used(flags);

		if (ANYBIT(flags, MU_DATABASE)) {
			size += sizeof(Grid);

			if (ANYBIT (dbflags, DB_DOUBLE_PRECISION))
				size += sizeof(Grid_Double);
			else
				size += sizeof(Grid_Single);
		}

		return size;
	}
};




class Modelling_Matrix : public Attribute {
#define DB_MM_SET_ACQUIRE		0
#define DB_MM_APPEND_ACQUIRE	1

public:
	Matrix			matrix;
	mutable int		_hash_key; // used by optimize

	Modelling_Matrix() :Attribute(MODELLING_MATRIX), _hash_key(0) {};

	INLINE bool operator == (Modelling_Matrix const & that) const {
		return (matrix == that.matrix);
	};

	Attribute * clone() const {return NEW(Modelling_Matrix)(*this);};

	bool equal(Attribute const * attr) const {
		if (attr->type != MODELLING_MATRIX)
			return false;
		return *this == *(Modelling_Matrix*)attr;
	}

	Attribute * merge (Thread_Data * thread_data, Attribute const * attr, void * control=0);

	Attribute * acquire (Thread_Data *	thread_data, Anything * item, Attribute const * old_attribute);

	int hash_key() const {
		if (_hash_key == 0) 
			_hash_key = matrix->hash_key();
		return _hash_key;
	};


	static Modelling_Matrix * Default(Thread_Data * thread_data);
};



class Texture_Matrix : public Attribute {
#define DB_TM_SET_ACQUIRE		0
#define DB_TM_APPEND_ACQUIRE	1

public:
	Matrix	matrix;

	Texture_Matrix():Attribute(TEXTURE_MATRIX) {};

	INLINE bool operator == (Texture_Matrix const & that) const {
		return (matrix == that.matrix);
	};

	Attribute * clone() const {return NEW(Texture_Matrix)(*this);};

	bool equal(Attribute const * attr) const {
		if (attr->type != TEXTURE_MATRIX)
			return false;
		return *this == *(Texture_Matrix*)attr;
	}

	Attribute * merge (Thread_Data * thread_data, Attribute const * attr, void * control=0);

	Attribute * acquire (Thread_Data *	thread_data, Anything * item, Attribute const * old_attribute);

	int hash_key() const {return matrix->hash_key();};


	static Texture_Matrix * Default(Thread_Data * thread_data);
};

class Window : public Attribute {
public:
	Rectangle	bounds;
	Rectangle	redraw;
	Rectangle	old_redraw;

	Window():Attribute(WINDOW) {};

	INLINE bool operator == (Window const & that) const {
		return (bounds == that.bounds);
	};

	Attribute * clone() const {return NEW(Window)(*this);};

	bool equal(Attribute const * attr) const {
		if (attr->type != WINDOW)
			return false;
		return *this == *(Window*)attr;
	}

	Attribute * merge (Thread_Data * thread_data, Attribute const * attr, void * control=0);

	Attribute * acquire (Thread_Data *	thread_data, Anything * item, Attribute const * old_attribute);

	int hash_key() const {
		return Float::extract_uint32_t(bounds.left) + Float::extract_uint32_t(bounds.right) + Float::extract_uint32_t(bounds.bottom) + Float::extract_uint32_t(bounds.top);
	};


	static Window * Default(Thread_Data * thread_data);
};

#define	Clip_Region_WORLD_SPACE		0x01
#define	Clip_Region_OBJECT_SPACE	0x02
#define	Clip_Region_MASK			0x04
#define	Clip_Region_COMPLEX			0x10

class Clip_Region : public Attribute {
public:
	Point_Vector		points;
	DPoint_Vector		dpoints;
	int_Vector			lengths;
	int					flags;

	Clip_Region():Attribute(CLIP_REGION), points(), dpoints(), lengths(), flags(0) {};

	INLINE bool operator == (Clip_Region const & that) const {
		if (flags != that.flags)
			return false;
		if (points != that.points)
			return false;
		if (dpoints != that.dpoints)
			return false;
		return true;
	};

	Attribute * clone() const {return NEW(Clip_Region)(*this);};

	bool equal(Attribute const * attr) const {
		if (attr->type != CLIP_REGION)
			return false;
		return *this == *(Clip_Region*)attr;
	}

	Attribute * merge (Thread_Data * thread_data, Attribute const * attr, void * control=0);

	Attribute * acquire (Thread_Data *	thread_data, Anything * item, Attribute const * old_attribute);

	int hash_key() const { return flags + static_cast<int>(points.size()) + static_cast<int>(dpoints.size()) + static_cast<int>(lengths.size()); };

	static Clip_Region * Default(Thread_Data * thread_data);
};


class Visibility : public Attribute {
public:
	Type_Flags			mask;
	Type_Flags			value;

	Visibility():Attribute(VISIBILITY), mask(T_NONE), value(T_NONE) {};

	INLINE bool operator == (Visibility const & that) const {
		if (mask != that.mask || value != that.value)
			return false;
		return true;
	};

	Attribute * clone() const {return NEW(Visibility)(*this);};

	bool equal(Attribute const * attr) const {
		if (attr->type != VISIBILITY)
			return false;
		return *this == *(Visibility*)attr;
	}

	Attribute * merge (Thread_Data * thread_data, Attribute const * attr, void * control=0);

	bool unset_acquire (Thread_Data * thread_data, Anything * item, Attribute const * unset_proto);

	Attribute * acquire (Thread_Data *	thread_data, Anything * item, Attribute const * old_attribute);

	int hash_key() const {return mask+value;};

	Seen_Flags maybe_visible() const {
		Seen_Flags	seen = S_NONE;

		if (ANYBIT (value, T_ANY_GEOMETRY))
			seen |= S_GENERIC_VISIBLE;
		if (ANYBIT (value, T_LINES|T_ANY_EDGE))
			seen |= S_LINES_VISIBLE;
		if (ANYBIT (value, T_MARKERS|T_VERTICES))
			seen |= S_GLYPHS_VISIBLE;
		if (ANYBIT(value, T_CUT_FACES))
			seen |= S_GENERIC_VISIBLE;
		if (ANYBIT(value, T_CUT_EDGES))
			seen |= S_LINES_VISIBLE;
		if (ANYBIT (value, T_WINDOWS))
			seen |= S_WINDOWS_VISIBLE;

		return seen;
	}

	static Visibility * Default(Thread_Data * thread_data);
};

class Selectability : public Attribute {
public:
	Type_Flags		mask;
	Type_Flags		up;
	Type_Flags		down;
	Type_Flags		still_up;
	Type_Flags		still_down;
	Type_Flags		even_invisible;
	Type_Flags		any_kind;

	Selectability():Attribute(SELECTABILITY),
		mask(T_NONE), up(T_NONE), down(T_NONE),
		still_up(T_NONE), still_down(T_NONE),
		even_invisible(T_NONE), any_kind(T_NONE)
		{};

	INLINE bool operator == (Selectability const & that) const {

		if (mask != that.mask ||
			up != that.up ||
			down != that.down ||
			still_up != that.still_up ||
			still_down != that.still_down ||
			even_invisible != that.even_invisible ||
			any_kind != that.any_kind)
			return false;

		return true;
	};

	Attribute * clone() const {return NEW(Selectability)(*this);};

	bool equal(Attribute const * attr) const {
		if (attr->type != SELECTABILITY)
			return false;
		return *this == *(Selectability*)attr;
	}

	Attribute * merge (Thread_Data * thread_data, Attribute const * attr, void * control=0);

	bool unset_acquire (Thread_Data * thread_data, Anything * item, Attribute const * unset_proto);

	Attribute * acquire (Thread_Data *	thread_data, Anything * item, Attribute const * old_attribute);

	int hash_key() const {
		return mask + up + down + still_up + still_down + even_invisible + any_kind;
	};

	static Selectability * Default(Thread_Data * thread_data);
};


//typedef int32_t Window_Frame_Flags; // move to hoops.h for proto

#define WFF_OFF							(Window_Frame_Flags)0x00000001
#define	WFF_ON							(Window_Frame_Flags)0x00000002
#define WFF_SINGLE						(Window_Frame_Flags)0x00000004
#define WFF_INSET						(Window_Frame_Flags)0x00000008
#define WFF_IMPLICIT_INSET				(Window_Frame_Flags)0x00000010
#define WFF_DECORATIVE					(Window_Frame_Flags)0x00000020
#define WFF_ATTRIBUTE_MASK				(Window_Frame_Flags)0x0000FFFF

/* these are used by the window rendition */
#define	WFF_WINDOW_WAS_REDRAWN			(Window_Frame_Flags)0x00010000
#define	WFF_UPPER_WINDOW_WAS_REDRAWN	(Window_Frame_Flags)0x00020000
#define	WFF_BYPASS_Z_CLEAR				(Window_Frame_Flags)0x80000000


class Window_Frame : public Attribute {
public:
	Window_Frame_Flags	flags;

	Window_Frame():Attribute(WINDOW_FRAME), flags(0) {};

	INLINE bool operator == (Window_Frame const & that) const {
		return (flags == that.flags);
	};

	Attribute * clone() const {return NEW(Window_Frame)(*this);};

	bool equal(Attribute const * attr) const {
		if (attr->type != WINDOW_FRAME)
			return false;
		return *this == *(Window_Frame*)attr;
	}

	//Attribute * merge (Thread_Data * thread_data, Attribute const * attr, void * control=0);

	Attribute * acquire (Thread_Data *	thread_data, Anything * item, Attribute const * old_attribute);

	int hash_key() const {return flags;};

	static Window_Frame * Default(Thread_Data * thread_data);
};

class Handedness : public Attribute {
public:
	int					which;
#	define		RIGHT_HANDED	0
#	define		LEFT_HANDED		1

	Handedness():Attribute(HANDEDNESS), which(0) {};

	INLINE bool operator == (Handedness const & that) const {
		return (which == that.which);
	};

	Attribute * clone() const {return NEW(Handedness)(*this);};

	bool equal(Attribute const * attr) const {
		if (attr->type != HANDEDNESS)
			return false;
		return *this == *(Handedness*)attr;
	}

	Attribute * merge (Thread_Data * thread_data, Attribute const * attr, void * control=0);

	Attribute * acquire (Thread_Data *	thread_data, Anything * item, Attribute const * old_attribute);

	int hash_key() const {return which;};

	static Handedness * Default(Thread_Data * thread_data);
};

class Streaming_Mode : public Attribute {
public:
	Segment *			end_of_stream;
	bool				flag;

	Streaming_Mode():Attribute(STREAMING_MODE), end_of_stream(null), flag(false) {};

	INLINE bool operator == (Streaming_Mode const & that) const {
		return (end_of_stream == that.end_of_stream && flag == that.flag);
	};

	Attribute * clone() const {return NEW(Streaming_Mode)(*this);};

	bool equal(Attribute const * attr) const {
		if (attr->type != STREAMING_MODE)
			return false;
		return *this == *(Streaming_Mode*)attr;
	}

	Attribute * acquire (Thread_Data *	thread_data, Anything * item, Attribute const * old_attribute);

	int hash_key() const {return (int)((intptr_t)end_of_stream+(flag?1:0));};

	static Streaming_Mode * Default(Thread_Data * thread_data);
};

#define Drivo_DEBUG							Driver_Options::Drivo_Bits(Driver_Options::index_Drivo_DEBUG)
#define Drivo_DISABLE_INPUT					Driver_Options::Drivo_Bits(Driver_Options::index_Drivo_DISABLE_INPUT)
#define Drivo_FIRST_COLOR					Driver_Options::Drivo_Bits(Driver_Options::index_Drivo_FIRST_COLOR)
#define Drivo_FIXED_COLORS					Driver_Options::Drivo_Bits(Driver_Options::index_Drivo_FIXED_COLORS)
#define Drivo_LANDSCAPE_ORIENTATION			Driver_Options::Drivo_Bits(Driver_Options::index_Drivo_LANDSCAPE_ORIENTATION)
#define Drivo_LIGHT_SCALING					Driver_Options::Drivo_Bits(Driver_Options::index_Drivo_LIGHT_SCALING)
#define Drivo_COLOR_CONSOLIDATION			Driver_Options::Drivo_Bits(Driver_Options::index_Drivo_COLOR_CONSOLIDATION)
#define Drivo_LOCATER_TRANSFORM				Driver_Options::Drivo_Bits(Driver_Options::index_Drivo_LOCATER_TRANSFORM)
#define Drivo_NUMBER_OF_COLORS				Driver_Options::Drivo_Bits(Driver_Options::index_Drivo_NUMBER_OF_COLORS)
#define Drivo_PEN_SPEED						Driver_Options::Drivo_Bits(Driver_Options::index_Drivo_PEN_SPEED)
#define Drivo_PHYSICAL_SIZE					Driver_Options::Drivo_Bits(Driver_Options::index_Drivo_PHYSICAL_SIZE)
#define Drivo_SELECTION_PROXIMITY			Driver_Options::Drivo_Bits(Driver_Options::index_Drivo_SELECTION_PROXIMITY)
#define Drivo_SUBSCREEN						Driver_Options::Drivo_Bits(Driver_Options::index_Drivo_SUBSCREEN)
#define Drivo_TITLE							Driver_Options::Drivo_Bits(Driver_Options::index_Drivo_TITLE)
#define Drivo_UPDATE_INTERRUPTS				Driver_Options::Drivo_Bits(Driver_Options::index_Drivo_UPDATE_INTERRUPTS)
#define Drivo_USE_COLORMAP_ID				Driver_Options::Drivo_Bits(Driver_Options::index_Drivo_USE_COLORMAP_ID)
#define Drivo_USE_WINDOW_ID					Driver_Options::Drivo_Bits(Driver_Options::index_Drivo_USE_WINDOW_ID)
#define Drivo_USE_WINDOW_ID2				Driver_Options::Drivo_Bits(Driver_Options::index_Drivo_USE_WINDOW_ID2)
#define Drivo_WRITE_MASK					Driver_Options::Drivo_Bits(Driver_Options::index_Drivo_WRITE_MASK)
#define Drivo_OUTPUT_FORMAT					Driver_Options::Drivo_Bits(Driver_Options::index_Drivo_OUTPUT_FORMAT)
#define Drivo_HARDCOPY_RESOLUTION			Driver_Options::Drivo_Bits(Driver_Options::index_Drivo_HARDCOPY_RESOLUTION)
#define Drivo_TEXT_PREFERENCE				Driver_Options::Drivo_Bits(Driver_Options::index_Drivo_TEXT_PREFERENCE)
#define Drivo_USE_CLIP_OVERRIDE				Driver_Options::Drivo_Bits(Driver_Options::index_Drivo_USE_CLIP_OVERRIDE)
#define Drivo_QMOVES_PREFERENCE				Driver_Options::Drivo_Bits(Driver_Options::index_Drivo_QMOVES_PREFERENCE)
#define Drivo_SHADOW_PREFERENCE				Driver_Options::Drivo_Bits(Driver_Options::index_Drivo_SHADOW_PREFERENCE)
#define Drivo_FALLBACK_FONT					Driver_Options::Drivo_Bits(Driver_Options::index_Drivo_FALLBACK_FONT)
#define Drivo_ANTI_ALIAS_REQUESTED			Driver_Options::Drivo_Bits(Driver_Options::index_Drivo_ANTI_ALIAS_REQUESTED)
#define Drivo_EXIT_UPDATE					Driver_Options::Drivo_Bits(Driver_Options::index_Drivo_EXIT_UPDATE)
#define Drivo_DISPLAY_INTERRUPTED_UPDATE	Driver_Options::Drivo_Bits(Driver_Options::index_Drivo_DISPLAY_INTERRUPTED_UPDATE)
#define Drivo_FONT_TABLE					Driver_Options::Drivo_Bits(Driver_Options::index_Drivo_FONT_TABLE)
#define Drivo_EXIT_UPDATE_DATA				Driver_Options::Drivo_Bits(Driver_Options::index_Drivo_EXIT_UPDATE_DATA)
#define Drivo_FILE_DESCRIPTOR				Driver_Options::Drivo_Bits(Driver_Options::index_Drivo_FILE_DESCRIPTOR)
#define Drivo_BACKING_STORE					Driver_Options::Drivo_Bits(Driver_Options::index_Drivo_BACKING_STORE)
#define Drivo_BORDER						Driver_Options::Drivo_Bits(Driver_Options::index_Drivo_BORDER)
#define Drivo_CONTROL_AREA					Driver_Options::Drivo_Bits(Driver_Options::index_Drivo_CONTROL_AREA)
#define Drivo_DOUBLE_BUFFERING				Driver_Options::Drivo_Bits(Driver_Options::index_Drivo_DOUBLE_BUFFERING)
#define Drivo_FORCE_BLACK_AND_WHITE			Driver_Options::Drivo_Bits(Driver_Options::index_Drivo_FORCE_BLACK_AND_WHITE)
#define Drivo_GAMMA_CORRECTION				Driver_Options::Drivo_Bits(Driver_Options::index_Drivo_GAMMA_CORRECTION)
#define Drivo_SUBSCREEN_MOVING				Driver_Options::Drivo_Bits(Driver_Options::index_Drivo_SUBSCREEN_MOVING)
#define Drivo_SUBSCREEN_RESIZING			Driver_Options::Drivo_Bits(Driver_Options::index_Drivo_SUBSCREEN_RESIZING)
#define Drivo_SUBSCREEN_STRETCHING			Driver_Options::Drivo_Bits(Driver_Options::index_Drivo_SUBSCREEN_STRETCHING)
#define Drivo_SPECIAL_EVENTS				Driver_Options::Drivo_Bits(Driver_Options::index_Drivo_SPECIAL_EVENTS)
#define Drivo_STEREO_REQUESTED				Driver_Options::Drivo_Bits(Driver_Options::index_Drivo_STEREO_REQUESTED)
#define Drivo_STENCIL_REQUESTED				Driver_Options::Drivo_Bits(Driver_Options::index_Drivo_STENCIL_REQUESTED)
#define Drivo_ABSOLUTE_LINE_WEIGHT			Driver_Options::Drivo_Bits(Driver_Options::index_Drivo_ABSOLUTE_LINE_WEIGHT)
#define Drivo_VERTEX_SPOTLIGHT				Driver_Options::Drivo_Bits(Driver_Options::index_Drivo_VERTEX_SPOTLIGHT)
#define Drivo_FORCE_GRAYSCALE				Driver_Options::Drivo_Bits(Driver_Options::index_Drivo_FORCE_GRAYSCALE)
#define Drivo_DISPLAY_STATS					Driver_Options::Drivo_Bits(Driver_Options::index_Drivo_DISPLAY_STATS)
#define Drivo_DISPLAY_TIME_STATS			Driver_Options::Drivo_Bits(Driver_Options::index_Drivo_DISPLAY_TIME_STATS)
#define Drivo_DISPLAY_MEMORY_STATS			Driver_Options::Drivo_Bits(Driver_Options::index_Drivo_DISPLAY_MEMORY_STATS)
#define Drivo_FULLSCREEN					Driver_Options::Drivo_Bits(Driver_Options::index_Drivo_FULLSCREEN)
#define Drivo_MARKER_DRAWING_FASTEST		Driver_Options::Drivo_Bits(Driver_Options::index_Drivo_MARKER_DRAWING_FASTEST)
#define Drivo_FAST_SILHOUETTE_EDGES			Driver_Options::Drivo_Bits(Driver_Options::index_Drivo_FAST_SILHOUETTE_EDGES)
#define Drivo_FAST_AMBIENT_OCCLUSION		Driver_Options::Drivo_Bits(Driver_Options::index_Drivo_FAST_AMBIENT_OCCLUSION)
#define Drivo_BLOOM							Driver_Options::Drivo_Bits(Driver_Options::index_Drivo_BLOOM)
#define Drivo_EYE_DOME_LIGHTING				Driver_Options::Drivo_Bits(Driver_Options::index_Drivo_EYE_DOME_LIGHTING)
#define Drivo_DEPTH_OF_FIELD				Driver_Options::Drivo_Bits(Driver_Options::index_Drivo_DEPTH_OF_FIELD)
#define Drivo_GPU_RESIDENT					Driver_Options::Drivo_Bits(Driver_Options::index_Drivo_GPU_RESIDENT)
#define Drivo_DRAW_DEGENERATE_LINES			Driver_Options::Drivo_Bits(Driver_Options::index_Drivo_DRAW_DEGENERATE_LINES)
#define Drivo_OVERRIDE_HARDCOPY_RES			Driver_Options::Drivo_Bits(Driver_Options::index_Drivo_OVERRIDE_HARDCOPY_RES)
#define Drivo_ISOLATED						Driver_Options::Drivo_Bits(Driver_Options::index_Drivo_ISOLATED)
#define Drivo_STATS_FILE					Driver_Options::Drivo_Bits(Driver_Options::index_Drivo_STATS_FILE)
#define Drivo_DISCARD_FRAMEBUFFER			Driver_Options::Drivo_Bits(Driver_Options::index_Drivo_DISCARD_FRAMEBUFFER)
#define Drivo_WINDOW_OPACITY				Driver_Options::Drivo_Bits(Driver_Options::index_Drivo_WINDOW_OPACITY)

class Driver_Options : public Attribute {
public:

	enum Drivo_Index {
		index_Drivo_DEBUG = 0,
		index_Drivo_DISABLE_INPUT,
		index_Drivo_FIRST_COLOR,
		index_Drivo_FIXED_COLORS,
		index_Drivo_LANDSCAPE_ORIENTATION,
		index_Drivo_LIGHT_SCALING,
		index_Drivo_COLOR_CONSOLIDATION,
		index_Drivo_LOCATER_TRANSFORM,
		index_Drivo_NUMBER_OF_COLORS,
		index_Drivo_PEN_SPEED,
		index_Drivo_PHYSICAL_SIZE,
		index_Drivo_SELECTION_PROXIMITY,
		index_Drivo_SUBSCREEN,
		index_Drivo_TITLE,
		index_Drivo_UPDATE_INTERRUPTS,
		index_Drivo_USE_COLORMAP_ID,
		index_Drivo_USE_WINDOW_ID,
		index_Drivo_USE_WINDOW_ID2,
		index_Drivo_WRITE_MASK,
		index_Drivo_OUTPUT_FORMAT,
		index_Drivo_HARDCOPY_RESOLUTION,
		index_Drivo_TEXT_PREFERENCE,
		index_Drivo_USE_CLIP_OVERRIDE,
		index_Drivo_QMOVES_PREFERENCE,
		index_Drivo_SHADOW_PREFERENCE,
		index_Drivo_FALLBACK_FONT,
		index_Drivo_ANTI_ALIAS_REQUESTED,
		index_Drivo_EXIT_UPDATE,
		index_Drivo_DISPLAY_INTERRUPTED_UPDATE,
		index_Drivo_FONT_TABLE,
		index_Drivo_EXIT_UPDATE_DATA,
		index_Drivo_FILE_DESCRIPTOR,
		index_Drivo_BACKING_STORE,
		index_Drivo_BORDER,
		index_Drivo_CONTROL_AREA,
		index_Drivo_DOUBLE_BUFFERING,
		index_Drivo_FORCE_BLACK_AND_WHITE,
		index_Drivo_GAMMA_CORRECTION,
		index_Drivo_SUBSCREEN_MOVING,
		index_Drivo_SUBSCREEN_RESIZING,
		index_Drivo_SUBSCREEN_STRETCHING,
		index_Drivo_SPECIAL_EVENTS,
		index_Drivo_STEREO_REQUESTED,
		index_Drivo_STENCIL_REQUESTED,
		index_Drivo_ABSOLUTE_LINE_WEIGHT,
		index_Drivo_VERTEX_SPOTLIGHT,
		index_Drivo_FORCE_GRAYSCALE,
		index_Drivo_DISPLAY_STATS,
		index_Drivo_DISPLAY_TIME_STATS,
		index_Drivo_DISPLAY_MEMORY_STATS,
		index_Drivo_FULLSCREEN,
		index_Drivo_MARKER_DRAWING_FASTEST,
		index_Drivo_FAST_SILHOUETTE_EDGES,
		index_Drivo_FAST_AMBIENT_OCCLUSION,
		index_Drivo_BLOOM,
		index_Drivo_DEPTH_OF_FIELD,
		index_Drivo_EYE_DOME_LIGHTING,
		index_Drivo_GPU_RESIDENT,
		index_Drivo_DRAW_DEGENERATE_LINES,
		index_Drivo_OVERRIDE_HARDCOPY_RES,
		index_Drivo_ISOLATED,
		index_Drivo_STATS_FILE,
		index_Drivo_DISCARD_FRAMEBUFFER,
		index_Drivo_WINDOW_OPACITY,

		index_Drivo_LAST, // must be after last real option

		// these are for parse tables only
		index_Drivo_CLEAR_DEBUG,
		index_Drivo_SET_DEBUG,
		index_Drivo_ZERO,
	};


	Define_Bitset(index_Drivo_LAST)	Drivo_Bits;

	Drivo_Bits	bits_mask;
	Drivo_Bits	bits_set;

	int					debug;
	int					set_debug; // aquire time only
	int					clear_debug; // aquire time only

	int					first_color;
	int					fixed_colors[3];
	float				light_scaling;
	float				color_consolidation;
	float				locater_transform[3][2];
	int					number_of_colors;
	float				pen_speed;
	Vector2D			physical_size;
	float				selection_proximity;
	Rectangle			subscreen;
	Name				title;
	Name				display_stats_file;
	int					anti_alias_samples;
	float				window_opacity;

	Font_Name_List		fallback_font_names;
	Font_Name_List		font_table2;

#	define		Interrupt_NONE			0
#	define		Interrupt_NORMAL		1
#	define		Interrupt_UNRESTRICTED	3
	int					update_interrupts;

#	define		Display_Interrupted_Update_NONE		0
#	define		Display_Interrupted_Update_OFF		1
#	define		Display_Interrupted_Update_ON		2
	int					display_interrupted_update_mode;


	intptr_t			use_window_ID;
	intptr_t			use_window_ID2;
	intptr_t			file_descriptor;
	intptr_t			use_colormap_ID;
	intptr_t			use_clip_override;

#	define		Window_ID_DEFAULT			0
#	define		Window_ID_IMAGE_KEY			1
	int					window_id_type;

	int					write_mask;
	Name_List *			output_format;

#	define	  Drivo_Landscape_OFF				0
#	define	  Drivo_Landscape_ON				1
#	define	  Drivo_Landscape_ENVIRONMENT		2
	int					landscape_option;

#	define	  Drivo_Draw_Degenerate_Lines_DEFAULT	0
#	define	  Drivo_Draw_Degenerate_Lines_NICE		1
#	define	  Drivo_Draw_Degenerate_Lines_FAST		2
	int					draw_degenerate_lines;

#	define	  Drivo_Text_Bitmap					0
#	define	  Drivo_Text_Outline				1
#	define	  Drivo_Text_Default				2
#	define	  Drivo_Text_Exterior_Outline		3
	int					text_preference;

#	define	  Drivo_Input_ENABLED		0
#	define	  Drivo_Input_DISABLED		1
#	define	  Drivo_Input_ALL_DISABLED	2
	int					disable_input_option;

#	define	  Drivo_QMoves_Default		0
#	define	  Drivo_QMoves_XOR			1
#	define	  Drivo_QMoves_Overlay		2
#	define	  Drivo_QMoves_Spriting		3
#   define	  LAST_QMOVES				Drivo_QMoves_Spriting
	int					qmoves_preferences[LAST_QMOVES];

#	define	  Drivo_Shadow_Software		0
#	define	  Drivo_Shadow_Hardware		1
#	define	  Drivo_Shadow_Image		2
#	define	  Drivo_Shadow_OGL			3
#	define	  Drivo_Shadow_SoftOGL		4
	int					shadow_preference;

	bool				subscreen_by_program;

	int					hardcopy_resolution;

#	define    Drivo_Silhouette_On				0x0001
#	define    Drivo_Silhouette_Tolerance		0x0002
#	define    Drivo_Silhouette_Heavy_Exterior	0x0004
	struct FSO {
		FSO():mask(0),value(0),fast_silhouette_tolerance(0) {}
		INLINE bool operator == (FSO const & that) const {
			return (mask == that.mask && value == that.value && fast_silhouette_tolerance == that.fast_silhouette_tolerance);
		}
		Fast_Silhouette_Flags   mask;
		Fast_Silhouette_Flags	value;
		float					fast_silhouette_tolerance;
	} fso;

#	define    Drivo_Ambient_Occlusion_On			0x0001
#	define    Drivo_Ambient_Occlusion_Strength		0x0002
#	define    Drivo_Ambient_Occlusion_Nice			0x0004
#	define    Drivo_Ambient_Occlusion_Fast			0x0008
#	define    Drivo_Ambient_Occlusion_Any_Quality	(Drivo_Ambient_Occlusion_Nice|Drivo_Ambient_Occlusion_Fast)
	struct FAOO {
		FAOO():mask(0),value(0),fast_ambient_occlusion_strength(0) {}
		INLINE bool operator == (FAOO const & that) const {
			return (mask == that.mask && value == that.value && fast_ambient_occlusion_strength == that.fast_ambient_occlusion_strength);
		}
		Fast_Ambient_Occlusion_Flags	mask;
		Fast_Ambient_Occlusion_Flags	value;
		float							fast_ambient_occlusion_strength;
	} faoo;

#	define    Drivo_Bloom_On				0x0001
#	define    Drivo_Bloom_Strength			0x0002
#	define    Drivo_Bloom_Blur_Passes		0x0004
#	define    Drivo_Bloom_Star				0x0008
#	define    Drivo_Bloom_Radial			0x0010
#	define    Drivo_Bloom_Any_Shape			(Drivo_Bloom_Star|Drivo_Bloom_Radial)
	struct BLOOM {
		BLOOM():mask(0),value(0),strength(0),blur_passes(0) {}
		INLINE bool operator == (BLOOM const & that) const {
			return (mask == that.mask && value == that.value && strength == that.strength && blur_passes == that.blur_passes);
		}
		Bloom_Flags	mask;
		Bloom_Flags	value;
		float		strength;
		int			blur_passes;
	} bloom;

#	define    Drivo_Eye_Dome_Lighting_On		0x0001
#	define    Drivo_Eye_Dome_Lighting_Exponent	0x0002
#	define    Drivo_Eye_Dome_Lighting_Tolerance	0x0004
#	define    Drivo_Eye_Dome_Lighting_Radius	0x0008
#	define    Drivo_Eye_Dome_Lighting_Strength	0x0010

	struct EDL {
		EDL() :mask(0), value(0), exponent(0) {}
		INLINE bool operator == (EDL const & that) const {
			return (mask == that.mask && value == that.value && exponent == that.exponent && tolerance == that.tolerance && radius == that.radius && strength == that.strength);
		}
		Eye_Dome_Lighting_Flags	mask;
		Eye_Dome_Lighting_Flags	value;
		float		exponent;
		float		tolerance;
		float		radius;
		float		strength;
	} edl;

#	define    Drivo_Depth_Of_Field_On		0x0001
#	define    Drivo_Depth_Of_Field_Near		0x0002
#	define    Drivo_Depth_Of_Field_Far		0x0004
#	define    Drivo_Depth_Of_Field_Strength	0x0008
	struct DOF {
		DOF():mask(0),value(0),near_depth(0),far_depth(0),strength(0) {}
		INLINE bool operator == (DOF const & that) const {
			return (mask == that.mask && value == that.value && near_depth == that.near_depth && far_depth == that.far_depth && strength == that.strength);
		}
		Depth_Of_Field_Flags	mask;
		Depth_Of_Field_Flags	value;
		float					near_depth;
		float					far_depth;
		float					strength;
	} dof;

	Callback_Name		exit_update_name;
	void *				exit_update_data;

	Driver_Options();

	bool operator == (Driver_Options const & that) const;

	Attribute * clone() const {return NEW(Driver_Options)(*this);};

	bool equal(Attribute const * attr) const {
		if (attr->type != DRIVER_OPTIONS)
			return false;
		return *this == *(Driver_Options*)attr;
	}


	Attribute * merge (Thread_Data * thread_data, Attribute const * attr, void * control=0);

	Attribute * acquire (Thread_Data *	thread_data, Anything * item, Attribute const * old_attribute);

	//int hash_key() const {ASSERT(0); return 0;};

	static Driver_Options * Default(Thread_Data * thread_data);
};

struct NURBS_Curve_Control {
	int			checksum;
	int			budget;
	int			continued_budget;
	float		max_deviation;
	float		max_angle;
	float		max_length;
	bool		view_dependent;

	INLINE void GENERATE_CHECKSUM()
	{
		// just XOR a bunch of stuff together
		checksum = budget ^  (budget << 16);

		int addin;
		memcpy(&addin, &continued_budget, sizeof(int));
		checksum ^= addin;

		memcpy(&addin, &max_deviation, sizeof(float));
		checksum ^= addin>>2;

		memcpy(&addin, &max_angle, sizeof(float));
		checksum ^= addin>>4;

		memcpy(&addin, &max_length, sizeof(float));
		checksum ^= addin>>6;

		if (view_dependent == true) {
			addin = RANDOM(); // these should not match anything
			checksum ^= addin>>8;
		}
	}

	/* for situations where we are not sure where we should be pulling the budget information from,
	 * so just make something up, and hope it's reasonable. Start out with
	 * constant initializers to trigger a compile error if the length
	 * of the struct changes (so that we know to come back here and add stuff) */
	INLINE void Set_Defaults()
	{
		// set up default values.
		budget = 512;
		continued_budget = 0;
		max_deviation = 0;
		max_angle = 20;
		max_length = 1;
		view_dependent = false;

		GENERATE_CHECKSUM();
	}
};


struct NURBS_Surface_Config {
	int			checksum;
	float		max_trim_curve_deviation;	/* normalized [0..1] parametric space */
	int			trim_curve_budget;			/* point_count */
	float		max_facet_deviation;		/* object space */
	float		max_facet_angle;			/* degrees */
	float		max_facet_width;			/* normalized [0..1] parametric space */
	int			surface_budget;				/* point count */


	INLINE void GENERATE_CHECKSUM()
	{
		// just XOR a bunch of stuff together
		checksum = trim_curve_budget ^  (trim_curve_budget << 16);

		int addin;
		memcpy(&addin, &max_trim_curve_deviation, sizeof(int));
		checksum ^= addin;

		memcpy(&addin, &max_facet_deviation, sizeof(int));
		checksum ^= addin>>2;

		memcpy(&addin, &max_facet_angle, sizeof(int));
		checksum ^= addin>>4;

		memcpy(&addin, &max_facet_width, sizeof(int));
		checksum ^= addin>>6;
	}

	/* for situations where we are not sure where we should be pulling the budget information from,
	 * so just make something up, and hope it's reasonable. Start out with
	 * constant initializers to trigger a compile error if the length
	 * of the struct changes (so that we know to come back here and add stuff) */
	INLINE void Set_Defaults()
	{
		max_trim_curve_deviation = 0.005f;
		trim_curve_budget = 500;
		max_facet_deviation = 10;
		max_facet_angle = 20;
		max_facet_width = 1.42f;
		surface_budget = 10000;
		GENERATE_CHECKSUM();
	}
};




class Heuristics : public Attribute {
public:
	Heuristics_Flags	mask;
	Heuristics_Flags	value;
#	define		Heur_NONE						(Heuristics_Flags)0x00000000
#	define		Heur_CLIPPING					(Heuristics_Flags)0x00000001
#	define		Heur_TRANSFORMATIONS			(Heuristics_Flags)0x00000002
#	define		Heur_UNUSED_0					(Heuristics_Flags)0x00000004
#	define		Heur_BACKPLANE_CULL				(Heuristics_Flags)0x00000008
#	define		Heur_CONCAVE_POLYGONS			(Heuristics_Flags)0x00000010
#	define		Heur_UNUSED_1					(Heuristics_Flags)0x00000020
#	define		Heur_RIGHT_HANDED_POLYGONS_ONLY (Heuristics_Flags)0x00000040
#	define		Heur_LEFT_HANDED_POLYGONS_ONLY	(Heuristics_Flags)0x00000080
#	define		Heur_INCREMENTAL_UPDATES		(Heuristics_Flags)0x00000100
#	define		Heur_HIDDEN_SURFACES			(Heuristics_Flags)0x00000200
#	define		Heur_QMOVES_INPLACE				(Heuristics_Flags)0x00000400
#	define		Heur_SELECTION_CULLING			(Heuristics_Flags)0x00000800
#	define		Heur_QMOVES_OVERLAY				(Heuristics_Flags)0x00001000
#	define		Heur_QMOVES_SPRITING			(Heuristics_Flags)0x00002000
#	define		Heur_QMOVES_XOR					(Heuristics_Flags)0x00004000
#	define		Heur_RELATED_SELECTION_LIMIT	(Heuristics_Flags)0x00008000
#	define		Heur_INTERNAL_SELECTION_LIMIT	(Heuristics_Flags)0x00010000
#	define		Heur_SELECTION_SORTING			(Heuristics_Flags)0x00020000
#	define		Heur_CULLING					(Heuristics_Flags)0x00040000
#	define		Heur_VISUAL_SELECTION			(Heuristics_Flags)0x00080000
#	define		Heur_EXCLUDE_BOUNDING			(Heuristics_Flags)0x00100000
#	define		Heur_DETAIL_SELECTION			(Heuristics_Flags)0x00200000
#	define		Heur_ORDERED_DRAWING			(Heuristics_Flags)0x00400000
#	define		Heur_ORDERED_UNIT				(Heuristics_Flags)0x00800000
#	define		Heur_ORDERED_OPTIONS			(Heuristics_Flags)0x01000000
#	define		Heur_ORDERED_RECURSIVE			(Heuristics_Flags)0x02000000
#	define		Heur_STATIC_SCENE				(Heuristics_Flags)0x04000000
#	define		Heur_SELECTION_LEVEL			(Heuristics_Flags)0x08000000
#	define		Heur_FORCE_DEFER_BATCH			(Heuristics_Flags)0x10000000
#	define		Heur_MODEL_TYPE					(Heuristics_Flags)0x20000000


#	define		Heur_ANY_POLYGON_HANDEDNESS	(Heuristics_Flags)(Heur_RIGHT_HANDED_POLYGONS_ONLY|Heur_LEFT_HANDED_POLYGONS_ONLY)
#	define		Heur_ANY_QUICK_MOVES		(Heuristics_Flags)(Heur_QMOVES_OVERLAY|Heur_QMOVES_SPRITING|Heur_QMOVES_XOR|Heur_QMOVES_INPLACE)
#	define		Heur_ANY_ORDERED_MODE		(Heuristics_Flags)(Heur_ORDERED_DRAWING|Heur_ORDERED_UNIT)


#	define		Heur_Culling_NONE				0x0000
#	define		Heur_Culling_VIEW_FRUSTUM		0x0001
#	define		Heur_Culling_EXTENT				0x0002
#	define		Heur_Culling_HARD_EXTENT		0x0004
# 	define		Heur_Culling_MAX_DISTANCE		0x0008
# 	define		Heur_Culling_VIEW_VOLUME		0x0010
#   define		Heur_Culling_VECTOR				0x0020
#	define		Heur_Culling_VOLUMETRIC			(Heur_Culling_EXTENT | Heur_Culling_HARD_EXTENT | \
												 Heur_Culling_MAX_DISTANCE | Heur_Culling_VIEW_VOLUME)
#	define		Heur_Culling_BASIC				(Heur_Culling_VIEW_FRUSTUM | Heur_Culling_VOLUMETRIC | Heur_Culling_VECTOR)
#	define		Heur_Culling_ANY_SELECTION		Heur_Culling_BASIC

#	define		Heur_Culling_OBSCURATION		0x0040

#	define		Heur_Culling_EXTENT_DETAIL		0x0100
#	define		Heur_Culling_EXTENT_MODE		0x0200
#	define		Heur_Culling_VERTEX_THRESHOLD	0x0400
#	define		Heur_Culling_EXTENT_LEVEL		0x0800
#	define		Heur_Culling_VECTOR_TOLERANCE	0x1000

#	define		Heur_Culling_ANY				(Heur_Culling_BASIC | Heur_Culling_OBSCURATION | \
												 Heur_Culling_EXTENT_DETAIL | Heur_Culling_EXTENT_MODE | \
												 Heur_Culling_VERTEX_THRESHOLD | Heur_Culling_EXTENT_LEVEL | \
												 Heur_Culling_VECTOR_TOLERANCE)

#	define		Heur_MAX_EXTENT_MODE_NONE			0
#	define		Heur_MAX_EXTENT_MODE_DOT			1
#	define		Heur_MAX_EXTENT_MODE_BOUNDING		2
#	define		Heur_MAX_EXTENT_MODE_DEFER			3

#	define		Heur_MAX_EXTENT_LEVEL_NONE			0x00
#	define		Heur_MAX_EXTENT_LEVEL_SEGMENT		0x01
#	define		Heur_MAX_EXTENT_LEVEL_GEOMETRY		0x02
#	define		Heur_MAX_EXTENT_LEVEL_PRIMITIVE		0x04
#	define		Heur_MAX_EXTENT_LEVEL_ALL			0x07

	struct Culling {
		Culling() :
			mask(0),
			value(0),
			vector(0,0,0),
			vector_tolerance(0),
			max_distance(0),
			maximum_extent(0),
			vertex_threshold(0),
			maximum_extent_mode(0),
			maximum_extent_level(0),
			hard_extent(0),
			volume_min(0,0,0),
			volume_max(0,0,0),
			pixel_threshold(0),
			use_octree(0) {};


		short				mask;
		short				value;
		/* segment-level culling vector */
		Vector				vector;
		float				vector_tolerance;

		/* maximum extent culling */
		float				max_distance;
		int					maximum_extent;
		int					vertex_threshold;
		int					maximum_extent_mode;
		int					maximum_extent_level;
		int					hard_extent;
		Point				volume_min;
		Point				volume_max;

#	define		Heur_OBSCURATION_CULLING_PIXEL_THRESHOLD    0
#	define		Heur_OBSCURATION_CULLING_USE_OCTREE			1
		/* obscuration culling */
		int					pixel_threshold;
		bool				use_octree;
	}					culling;
	struct Selection_Culling {
		Selection_Culling() :
			mask(0),
			value(0) {};

		short				mask;
		short				value;
	}					selection_culling;

	int					related_selection_limit;
	struct Internal_Selection_Limit {
		Internal_Selection_Limit() :
			shell(0),
			polyline(0),
			mask(0),
			value(0) {};

		int					shell;
		int					polyline;
#		define	Heur_Internal_SHELL			0x01
#		define	Heur_Internal_POLYLINE		0x02
		short				mask;
		short				value;
	}					internal_selection_limit;

#	define		Heur_Order_WORLD_VOLUME			0
#	define		Heur_Order_SCREEN_EXTENT		1
#	define		Heur_Order_DISTANCE				2
#	define		Heur_Order_DIVERGENCE			3
#	define		Heur_Order_DENSITY				4
#	define		Heur_Order_PRIORITY				5
#	define		Heur_Order_COUNT				6

	int					ordered_weight_mask;
	float				ordered_weights[Heur_Order_COUNT];
	float				defer_bias;
	int					defer_range_min;
	int					defer_range_max;

#	define 		Heur_Selection_Level_ENTITY			0
#	define 		Heur_Selection_Level_SEGMENT		1
#	define 		Heur_Selection_Level_SEGMENT_TREE	2
#	define 		Heur_Selection_Level_COLLECTING		3	/* internal */
	int					selection_level;
	
	HPS::Selection::Level	hps_selection_level;

	int					force_defer_batch;
	float				force_defer_bias;

#	define		Heur_MODEL_TYPE_DEFAULT			0
#	define		Heur_MODEL_TYPE_LMV				1

	int					model_type;

	int					selection_action;
	float				selection_proximity;


public:

	Heuristics():Attribute(HEURISTICS) ,
		mask(0),
		value(0),
		culling(),
		selection_culling(),
		related_selection_limit(0),
		internal_selection_limit(),

		ordered_weight_mask(0),
		defer_bias(0),
		defer_range_min(0),
		defer_range_max(0),

		selection_level(0),
		hps_selection_level(HPS::Selection::Level::Entity),

		force_defer_batch(0),
		force_defer_bias(0),

		model_type(0),

		selection_action(0),
		selection_proximity(-1.0f)
	{
		for (int i=0; i< Heur_Order_COUNT; i++)
			ordered_weights[i] = 0;
	};

	INLINE bool operator == (Heuristics const & that) const {

		if (mask != that.mask ||
			value != that.value)
			return false;

		if (ANYBIT (mask, Heur_CULLING)) {
			if (culling.mask != that.culling.mask ||
				culling.value != that.culling.value)
				return false;

			if (ANYBIT (culling.value, Heur_Culling_OBSCURATION)) {
				if (culling.pixel_threshold != that.culling.pixel_threshold ||
					culling.use_octree != that.culling.use_octree)
					return false;
			}
			if (ANYBIT (culling.value, Heur_Culling_EXTENT)) {
				if (culling.maximum_extent != that.culling.maximum_extent)
					return false;
			}
			if (ANYBIT (culling.value, Heur_Culling_HARD_EXTENT)) {
				if (culling.hard_extent != that.culling.hard_extent)
					return false;
			}
			if (ANYBIT (culling.value, Heur_Culling_VERTEX_THRESHOLD)) {
				if (culling.vertex_threshold != that.culling.vertex_threshold)
					return false;
			}
			if (ANYBIT (culling.value, Heur_Culling_EXTENT_MODE)) {
				if (culling.maximum_extent_mode != that.culling.maximum_extent_mode)
					return false;
			}
			if (ANYBIT (culling.value, Heur_Culling_EXTENT_LEVEL)) {
				if (culling.maximum_extent_level != that.culling.maximum_extent_level)
					return false;
			}
			if (ANYBIT (culling.value, Heur_Culling_VECTOR)) {
				if (culling.vector != that.culling.vector)
					return false;
			}
			if (ANYBIT (culling.value, Heur_Culling_VECTOR_TOLERANCE)) {
				if (culling.vector_tolerance != that.culling.vector_tolerance)
					return false;
			}
			if (ANYBIT (culling.value, Heur_Culling_MAX_DISTANCE)) {
				if (culling.max_distance != that.culling.max_distance)
					return false;
			}
			if (ANYBIT (culling.value, Heur_Culling_VIEW_VOLUME)) {
				if (culling.volume_min != that.culling.volume_min ||
					culling.volume_max != that.culling.volume_max)
					return false;
			}

		}

		if (ANYBIT (mask, Heur_SELECTION_CULLING)) {
			if (selection_culling.mask != that.selection_culling.mask ||
				selection_culling.value != that.selection_culling.value)
				return false;
		}


		if (ANYBIT (value, Heur_MODEL_TYPE)) {
			if (model_type != that.model_type)
				return false;
		}
		if (ANYBIT (value, Heur_SELECTION_LEVEL)) {
			if (selection_level != that.selection_level)
				return false;
			if (hps_selection_level != that.hps_selection_level)
				return false;
		}

		if (ANYBIT (value, Heur_RELATED_SELECTION_LIMIT)) {
			if (related_selection_limit != that.related_selection_limit)
				return false;
		}
		if (ANYBIT (value, Heur_INTERNAL_SELECTION_LIMIT)) {
			if (internal_selection_limit.mask != that.internal_selection_limit.mask ||
				internal_selection_limit.value != that.internal_selection_limit.value)
				return false;

			if (ANYBIT (internal_selection_limit.value, Heur_Internal_SHELL)) {
				if (internal_selection_limit.shell != that.internal_selection_limit.shell)
					return false;
			}
			if (ANYBIT (internal_selection_limit.value, Heur_Internal_POLYLINE)) {
				if (internal_selection_limit.polyline != that.internal_selection_limit.polyline)
					return false;
			}
		}
		if (ANYBIT (value, Heur_ORDERED_OPTIONS)) {
			if (ordered_weight_mask != that.ordered_weight_mask)
				return false;

			for (int i=0; i< Heur_Order_COUNT; i++) {
				if (ANYBIT (ordered_weight_mask, 1<<i)) {
					if (ordered_weights[i] != that.ordered_weights[i])
						return false;
				}
			}
		}
		if (ANYBIT (value, Heur_FORCE_DEFER_BATCH)) {
			if (force_defer_batch != that.force_defer_batch ||
				force_defer_bias != that.force_defer_bias)
				return false;
		}

		return true;
	};

	Attribute * clone() const {return NEW(Heuristics)(*this);};

	bool equal(Attribute const * attr) const {
		if (attr->type != HEURISTICS)
			return false;
		return *this == *(Heuristics*)attr;
	}

	Attribute * merge (Thread_Data * thread_data, Attribute const * attr, void * control=0);

	Attribute * acquire (Thread_Data *	thread_data, Anything * item, Attribute const * old_attribute);

	bool unset_acquire (Thread_Data * thread_data, Anything * item, Attribute const * unset_proto);

	int hash_key() const {
		return mask + value;
	};


	static Heuristics * Default(Thread_Data * thread_data);
};

struct Rendering_Options_Locks {
	Attribute_Lock_Set	normal;
	Attribute_Lock_Set	forced;
};

struct Rendering_Options_HLO {
	Hidden_Line_Flags		mask;
	Hidden_Line_Flags		value;
#	define		Hidden_LINE_VISIBILITY			(Hidden_Line_Flags)0x0001
#	define		Hidden_LINE_PATTERN				(Hidden_Line_Flags)0x0002
#	define		Hidden_LINE_DIM_FACTOR			(Hidden_Line_Flags)0x0004
#	define		Hidden_LINE_FACE_DISPLACEMENT	(Hidden_Line_Flags)0x0008
#	define		Hidden_LINE_RENDER_FACES		(Hidden_Line_Flags)0x0010
#	define		Hidden_LINE_RENDER_TEXT			(Hidden_Line_Flags)0x0020
#	define		Hidden_LINE_SILHOUETTE_CLEANUP	(Hidden_Line_Flags)0x0040
#	define		Hidden_LINE_COLOR				(Hidden_Line_Flags)0x0080
#	define		Hidden_LINE_WEIGHT				(Hidden_Line_Flags)0x0100
#	define		Hidden_LINE_IMAGE_OUTLINE		(Hidden_Line_Flags)0x0200
#	define		Hidden_LINE_HSR_ALGORITHM		(Hidden_Line_Flags)0x0400
#	define		Hidden_LINE_TRANSPARENCY_CUTOFF	(Hidden_Line_Flags)0x0800
#	define		Hidden_LINE_REMOVE_DUPLICATES	(Hidden_Line_Flags)0x1000

	RGB						color;
	Name					line_pattern_name;
	float					dim_factor;
	float					face_displacement;
	float					transparency_cutoff;
	Generic_Size			weight;
	HSR_Algorithm			hsr_algorithm;	/* as normal hsr above */
};

struct Rendering_Options_Isoline_Contour {

	Isoline_Contour_Flags	mask;
	Isoline_Contour_Flags	value;
#define	Contour_VISIBILITY	(Isoline_Contour_Flags)0x00000001
#define Contour_ADJUSTMENT	(Isoline_Contour_Flags)0x00000002
#define Contour_MASK		(Isoline_Contour_Flags)0x000000FF
#define	Isoline_VISIBILITY	(Isoline_Contour_Flags)0x00000100
#define	Isoline_POSITION	(Isoline_Contour_Flags)0x00000200
#define	Isoline_COLOR		(Isoline_Contour_Flags)0x00000400
#define	Isoline_PATTERN		(Isoline_Contour_Flags)0x00000800
#define	Isoline_WEIGHT		(Isoline_Contour_Flags)0x00001000
#define	Isoline_LIGHTING	(Isoline_Contour_Flags)0x00002000
#define Isoline_MASK		(Isoline_Contour_Flags)0x0000FF00

	struct {
		Type_Flags				mask;
		Type_Flags				value;
	}	visibility;

	float *					positions;
	RGB *					colors;
	Name *					pattern_names;
	Generic_Size *			weights;

#define Adjustment_NONE			0
#define Adjustment_NORMALIZED	1
#define Adjustment_EXPLICIT		2
	int						adjustment;
	float					scale;
	float					translate;

#define	Position_REPEATING	0
#define Position_EXPLICIT	1
	int						position_type;

	int						position_count;
	int						color_count;
	int						pattern_count;
	int						weight_count;
};

struct Rendering_Options_LOD {
#define LOD_MAX_LEVELS 8
/* generation-time options */
#	define LOD_FLAGS_ON					(LOD_Flags)0x00000001
#	define LOD_FLAGS_CONSERVE_MEMORY	(LOD_Flags)0x00000002
#	define LOD_FLAGS_TOLERANCE_FRU		(LOD_Flags)0x00000004
#	define LOD_FLAGS_TOLERANCE_ORU		(~(LOD_FLAGS_TOLERANCE_FRU))
#	define LOD_RATIO					(LOD_Flags)0x00000008
#	define LOD_MIN_TRIANGLE_COUNT		(LOD_Flags)0x00000010
#	define LOD_NUM_LEVELS				(LOD_Flags)0x00000020
#	define LOD_MAX_DEGREE				(LOD_Flags)0x00000040
#	define LOD_COLLAPSE_VERTICES		(LOD_Flags)0x00000080
#	define LOD_TOLERANCE				(LOD_Flags)0x00000100  /* relates to LOD_COLLAPSE_VERTICES */
#	define LOD_ALGORITHM				(LOD_Flags)0x00000200
#	define LOD_SEGMENT_MODE				(LOD_Flags)0x00000400
#	define LOD_FLAGS_PREPROCESS			(LOD_Flags)0x00000800  /* old */
#	define LOD_USEFULNESS_HEURISTIC		(LOD_Flags)0x00001000  /* old */
#	define LOD_BOUNDING					(LOD_Flags)0x00002000  /* old */
#	define LOD_FLAGS_BOUNDING_CURRENT	(LOD_Flags)0x00004000  /* old */
#	define LOD_FLAGS_BOUNDING_EXPLICIT	(LOD_Flags)0x00008000  /* old */
#	define LOD_FLAGS_BOUNDING_ANY		(LOD_FLAGS_BOUNDING_CURRENT|LOD_FLAGS_BOUNDING_EXPLICIT)
#	define LOD_CALCULATION_CUTOFF		(LOD_Flags)0x00010000  /* old */
/* draw-time options */
#	define LOD_FLAGS_PHYSICAL			(LOD_Flags)0x00020000
#	define LOD_FLAGS_SCREEN_SPACE		(LOD_Flags)0x00040000
#	define LOD_FLAGS_UNSPECIFIED		(LOD_Flags)0
#	define LOD_FLAGS_AREA_PERCENTAGE	(LOD_Flags)0x00080000
#	define LOD_FLAGS_DISTANCE			(LOD_Flags)0x00100000
#	define LOD_FALLBACK					(LOD_Flags)0x00200000
#	define LOD_CLAMP					(LOD_Flags)0x00400000
#	define LOD_THRESHOLD				(LOD_Flags)0x00800000

#	define LOD_HEUR_TRI_DIAG_RATIO		1
#	define LOD_HEUR_DIAG_RATIO			2
#	define LOD_HEUR_TRI_DIAG			3
#	define LOD_HEUR_DIAG				4
#	define LOD_HEUR_TRI_VOL_RATIO		5
#	define LOD_HEUR_VOL_RATIO			6
#	define LOD_HEUR_TRI_VOL				7
#	define LOD_HEUR_VOL					8
#	define LOD_HEUR_TSIZE				9

#	define LOD_FALLBACK_NONE			  0x01	 /* draw nothing */
#	define LOD_FALLBACK_BOUNDING		  0x02	 /* draw the bounding cuboid */
#	define LOD_FALLBACK_COARSEST		  0x04	 /* draw the last lod in the list */
#	define LOD_FALLBACK_COARSEST_NONE	  0x08	 /* coarsest if inexpensive, nothing otherwise */
#	define LOD_FALLBACK_COARSEST_BOUNDING 0x10	 /* coarsest if inexpensive, bounding otherwise */
#	define LOD_FALLBACK_BOUNDING_NONE	  0x20	 /* bounding if off by only one level, nothing otherwise*/

#	define LOD_ALGORITHM_FAST 0
#	define LOD_ALGORITHM_NICE 1
		Bounding				bounding;
		LOD_Flags				mask;
		LOD_Flags				value;
		float					ratio[LOD_MAX_LEVELS]; /* compression ratio */
		float					threshold[LOD_MAX_LEVELS]; /* switch threshold */
		int						min_triangle_count;
		int						clamp;			/* clamp to level.	-1 if disabled */
		int						num_levels;
		int						max_degree;
		float					tolerance;
		float					cutoff[LOD_MAX_LEVELS]; /* calculation usefulness cutoff */
		int						heuristic; /* usefulness heuristic (view independent) */
		int						fallback; /* fallback in case we request a non-existent lod */
		int						algorithm;
};




struct Rendering_Options_Effects {
	struct {
		float	hither;
		float	yon;
	}	fog;

	float					stereo_separation;
	float					stereo_distance;

	Vector					ambient_up_vector;

	bool					mask_framebuffer_effects;

	struct {
#	define GOOCH_COLOR_RANGE			(GOOCH_Flags)0x0001
#	define GOOCH_DIFFUSE_WEIGHT			(GOOCH_Flags)0x0002
#	define GOOCH_COLOR_MAP				(GOOCH_Flags)0x0004
		GOOCH_Flags				mask;
		GOOCH_Flags				value;
		Style *					color_map_segment;
		Name					color_map_segment_name;
		float					color_range[2];
		float					diffuse_weight;
		bool					warning_given;
	}	gooch_options;

#define Cut_Geometry_LEVEL				(Cut_Geometry_Flags)0x00000001
#define Cut_Geometry_TOLERANCE			(Cut_Geometry_Flags)0x00000002
#define Cut_Geometry_MATCH_COLOR		(Cut_Geometry_Flags)0x00000004
#define Cut_Geometry_TOL_PERCENT		(Cut_Geometry_Flags)0x00000010
#define Cut_Geometry_Level_SKIP			0		/* internal */
#define Cut_Geometry_Level_ENTITY		1
#define Cut_Geometry_Level_SEGMENT		2
#define Cut_Geometry_Level_SEGMENT_TREE 3
#define Cut_Geometry_Match_EXPLICIT		0
#define Cut_Geometry_Match_CURRENT		1
#define Cut_Geometry_Match_FIRST		2
#define Cut_Geometry_Match_LAST			3
	struct {
		Cut_Geometry_Flags		mask;
		Cut_Geometry_Flags		value;
		int						level;
		int						match_color;
		float					tolerance;
	}	cut_geometry;

#define Simple_Shadow_VISIBLE				(Simple_Shadow_Flags)0x00000001
#define Simple_Shadow_PLANE					(Simple_Shadow_Flags)0x00000002
#define Simple_Shadow_LIGHT					(Simple_Shadow_Flags)0x00000004
#define Simple_Shadow_COLOR					(Simple_Shadow_Flags)0x00000008
#define Simple_Shadow_OPACITY				(Simple_Shadow_Flags)0x00000010
#define Simple_Shadow_RESOLUTION			(Simple_Shadow_Flags)0x00000020
#define Simple_Shadow_SOFTNESS				(Simple_Shadow_Flags)0x00000040
#define Simple_Shadow_IGNORE_TRANSPARENCY	(Simple_Shadow_Flags)0x00000080
#define Simple_Shadow_FAST_BOUNDING			(Simple_Shadow_Flags)0x00000100

#define Simple_Shadow_OFF				0
#define Simple_Shadow_ON				1
#define Simple_Shadow_AUTO				2
	struct SS {
		Simple_Shadow_Flags		mask;
		Simple_Shadow_Flags		value;
		Plane					plane;
		Vector					light;
		struct {
			Name				name;
			RGB					rgb;	/* g & b negative if r stores index */
		}	color;
		float					opacity;

		int						visibility;
		int						resolution;
		int						blurring;
	}	simple_shadow;

#define Shadow_Map_VISIBLE			(Shadow_Map_Flags)0x00000001
#define Shadow_Map_JITTER			(Shadow_Map_Flags)0x00000002
#define Shadow_Map_RESOLUTION		(Shadow_Map_Flags)0x00000004
#define Shadow_Map_SAMPLES			(Shadow_Map_Flags)0x00000008
#define Shadow_Map_VIEW_DEPENDENT	(Shadow_Map_Flags)0x00000010

	struct {
		Shadow_Map_Flags		mask;
		Shadow_Map_Flags		value;
		int						resolution;
		int						samples;
	}	shadow_map;

#define Simple_Reflection_VISIBLE		(Simple_Reflection_Flags)0x00000001
#define Simple_Reflection_PLANE			(Simple_Reflection_Flags)0x00000002
#define Simple_Reflection_OPACITY		(Simple_Reflection_Flags)0x00000004
#define Simple_Reflection_FADING		(Simple_Reflection_Flags)0x00000008
#define Simple_Reflection_ATTENUATION	(Simple_Reflection_Flags)0x00000010
#define Simple_Reflection_SOFTNESS		(Simple_Reflection_Flags)0x00000020
#define Simple_Reflection_VISIBILITY	(Simple_Reflection_Flags)0x00000040

	struct SR {
		Simple_Reflection_Flags	mask;
		Simple_Reflection_Flags	value;
		Plane					plane;
		float					opacity;
		float					hither;
		float					yon;
		int						blurring;
		struct SRV {
			Type_Flags			mask;
			Type_Flags			value;
		}	visibility;
	}	simple_reflection;
};


struct Rendering_Options_Geometry{
#define	Geometry_Options_DIHEDRAL					0x01
#define	Geometry_Options_FEA_NODES					0x02
#define	Geometry_Options_REVERSE_CYLINDER_RADII		0x04
#define	Geometry_Options_REVERSE_CYLINDER_COLORS	0x08
#define	Geometry_Options_REVERSE_CYLINDERS			0x0C

	int						mask;
	int						value;

	float					hard_edge_dihedral;

	struct {
#define NURBS_Curve_VIEW_DEPENDENT		(NURBS_Curve_Flags)0x00000001
#define NURBS_Curve_BUDGET				(NURBS_Curve_Flags)0x00000002
#define NURBS_Curve_CONTINUED_BUDGET	(NURBS_Curve_Flags)0x00000004
#define NURBS_Curve_MAX_DEVIATION		(NURBS_Curve_Flags)0x00000008
#define NURBS_Curve_MAX_ANGLE			(NURBS_Curve_Flags)0x00000010
#define NURBS_Curve_MAX_LENGTH			(NURBS_Curve_Flags)0x00000020

		NURBS_Curve_Flags		mask;
		NURBS_Curve_Flags		value;
		int						budget;
		int						continued_budget;
		float					max_deviation;		/* object space */
		float					max_angle;			/* degrees */
		float					max_length;			/* normalized [0..1] parametric space */
	}						nurbs_curve;


#define NURBS_Surface_MAX_TRIM_CURVE_DEVIATION	(NURBS_Surface_Flags)0x00000001
#define NURBS_Surface_TRIM_CURVE_BUDGET			(NURBS_Surface_Flags)0x00000002
#define NURBS_Surface_MAX_FACET_DEVIATION		(NURBS_Surface_Flags)0x00000004
#define NURBS_Surface_MAX_FACET_ANGLE			(NURBS_Surface_Flags)0x00000008
#define NURBS_Surface_MAX_FACET_WIDTH			(NURBS_Surface_Flags)0x00000010
#define NURBS_Surface_BUDGET					(NURBS_Surface_Flags)0x00000020
	struct {
		NURBS_Surface_Flags		mask;
		NURBS_Surface_Flags		value;
		NURBS_Surface_Config	configs;
	} nurbs_surface;

#define Tessellation_CYLINDER			0x00000001
#define Tessellation_SPHERE				0x00000002
	struct {
		Tessellation_Flags		mask;
		int						cylinder[LOD_MAX_LEVELS];
		int						sphere[LOD_MAX_LEVELS];
	} tessellation;
};


struct Set_Clear_Rendo_Debug {
	Rendo_Debug		clear, set;
};


class Rendering_Options : public Attribute {

public:
#	define Rendo_SUBTYPE_MASK					(Rendo_Flags)0xC0000000
#	define Rendo_SUBTYPE_COMPLEX				(Rendo_Flags)0x00000000
#	define Rendo_SUBTYPE_SIMPLE					(Rendo_Flags)0x40000000
#	define Rendo_SUBTYPE_INTERPOLATION			(Rendo_Flags)0x80000000
#	define Rendo_SUBTYPE_UNUSED					(Rendo_Flags)0xC0000000

	Rendo_Flags				complex_mask;
	Rendo_Flags				complex_value;
#	define Rendo_ATTRIBUTE_LOCK					(Rendo_Flags)0x00000001
#	define Rendo_BUFFER_OPTIONS					(Rendo_Flags)0x00000002
#	define Rendo_CUT_GEOMETRY_OPTIONS			(Rendo_Flags)0x00000004
#	define Rendo_GEOMETRY_OPTIONS				(Rendo_Flags)0x00000008
#	define Rendo_HIDDEN_LINE_OPTIONS			(Rendo_Flags)0x00000010
#	define Rendo_LOD_OPTIONS					(Rendo_Flags)0x00000020
#	define Rendo_MATRIX_OVERRIDE				(Rendo_Flags)0x00000040
#	define Rendo_NURBS_CURVE_OPTIONS			(Rendo_Flags)0x00000080
#	define Rendo_NURBS_SURFACE_OPTIONS			(Rendo_Flags)0x00000100
#	define Rendo_SIMPLE_SHADOW_OPTIONS			(Rendo_Flags)0x00000200
#	define Rendo_TESSELLATION_OPTIONS			(Rendo_Flags)0x00000400
#	define Rendo_TRANSPARENCY_OPTIONS			(Rendo_Flags)0x00000800
#	define Rendo_ANTIALIAS_OPTIONS				(Rendo_Flags)0x00001000
#	define Rendo_SHADOW_MAP_OPTIONS				(Rendo_Flags)0x00002000
#	define Rendo_SIMPLE_REFLECTION_OPTIONS		(Rendo_Flags)0x00004000
#   define Rendo_GOOCH_OPTIONS					(Rendo_Flags)0x00010000
#	define Rendo_FORCED_LOCK					(Rendo_Flags)0x00020000
#	define Rendo_CONTOUR_OPTIONS				(Rendo_Flags)0x00040000
#	define Rendo_ISOLINE_OPTIONS				(Rendo_Flags)0x00080000
#	define Rendo_DIFFUSE_COLOR_TINT				(Rendo_Flags)0x00100000
#	define Rendo_RADIOSITY_OPTIONS				(Rendo_Flags)0x10000000
#	define Rendo_RAY_TRACE_OPTIONS				(Rendo_Flags)0x20000000
#	define Rendo_BITS_RESERVED_FOR_TYPE_MASK	(Rendo_Flags)0xC0000000

/* 'LOD' looks like a high-level option, but is stored under lod_options, so any unique value here */
#	define Rendo_LOD							(Rendo_Flags)0x00000021

	Rendo_Flags				simple_mask;
	Rendo_Flags				simple_value;
#	define Rendo_RANDOMIZE_VERTICES				(Rendo_Flags)0x40000001
#	define Rendo_ATMOSPHERIC_ATTENUATION		(Rendo_Flags)0x40000002
#	define Rendo_DEBUG							(Rendo_Flags)0x40000004
#	define Rendo_DEPTH_RANGE					(Rendo_Flags)0x40000008
#	define Rendo_DISPLAY_LISTS					(Rendo_Flags)0x40000010
#	define Rendo_FACE_DISPLACEMENT				(Rendo_Flags)0x40000020
#	define Rendo_FORCE_GRAYSCALE				(Rendo_Flags)0x40000040
#	define Rendo_HSR_ALGORITHM					(Rendo_Flags)0x40000080
#	define Rendo_IMAGE_SCALE					(Rendo_Flags)0x40000100
#	define Rendo_IMAGE_TINT						(Rendo_Flags)0x40000200
#	define Rendo_LOCAL_VIEWER					(Rendo_Flags)0x40000400
#	define Rendo_LOCAL_CUTTING_PLANES			(Rendo_Flags)0x40000800
#	define Rendo_PERSPECTIVE_CORRECTION			(Rendo_Flags)0x40001000
#	define Rendo_QUANTIZATION					(Rendo_Flags)0x40002000
#	define Rendo_STEREO							(Rendo_Flags)0x40004000
#	define Rendo_STEREO_SEPARATION				(Rendo_Flags)0x40008000
#	define Rendo_TECHNOLOGY						(Rendo_Flags)0x40010000
#	define Rendo_GENERAL_DISPLACEMENT			(Rendo_Flags)0x40020000
#	define Rendo_JOIN_CUTOFF_ANGLE				(Rendo_Flags)0x40040000
#	define Rendo_STEREO_DISTANCE				(Rendo_Flags)0x40080000
#	define Rendo_SCREEN_RANGE					(Rendo_Flags)0x40100000
#	define Rendo_AMBIENT_UP_VECTOR				(Rendo_Flags)0x40200000
#	define Rendo_VERTEX_DECIMATION				(Rendo_Flags)0x40400000
#	define Rendo_VERTEX_DISPLACEMENT			(Rendo_Flags)0x40800000
#	define Rendo_FRAME_BUFFER_EFFECTS			(Rendo_Flags)0x41000000
#	define Rendo_DIFFUSE_TEXTURE_TINT			(Rendo_Flags)0x42000000
#	define Rendo_SCALED_DISPLACEMENT			(Rendo_Flags)0x44000000

	Rendo_Flags				interpolation_mask;
	Rendo_Flags				interpolation_value;
#	define Rendo_COLOR_INTERPOLATION_MARKERS		(Rendo_Flags)0x80000001
#	define Rendo_COLOR_INTERPOLATION_EDGES			(Rendo_Flags)0x80000002
#	define Rendo_COLOR_INTERPOLATION_FACES			(Rendo_Flags)0x80000004
#	define Rendo_COLOR_INTERPOLATION				(Rendo_Flags)0x80000007

#	define Rendo_COLOR_INDEX_INTERPOLATION_MARKERS	(Rendo_Flags)0x80000010
#	define Rendo_COLOR_INDEX_INTERPOLATION_EDGES	(Rendo_Flags)0x80000020
#	define Rendo_COLOR_INDEX_INTERPOLATION_FACES	(Rendo_Flags)0x80000040
#	define Rendo_COLOR_INDEX_INTERPOLATION			(Rendo_Flags)0x80000070
#	define Rendo_COLOR_INDEX_FACE_ISOLINES			(Rendo_Flags)0x80000080

#	define Rendo_TEXTURE_INTERPOLATION_MARKERS		(Rendo_Flags)0x80000100
#	define Rendo_TEXTURE_INTERPOLATION_EDGES		(Rendo_Flags)0x80000200
#	define Rendo_TEXTURE_INTERPOLATION_FACES		(Rendo_Flags)0x80000400
#	define Rendo_TEXTURE_INTERPOLATION				(Rendo_Flags)0x80000700

#	define Rendo_MARKER_INTERPOLATIONS				(Rendo_Flags)0x80000111
#	define Rendo_EDGE_INTERPOLATIONS				(Rendo_Flags)0x80000222
#	define Rendo_FACE_INTERPOLATIONS				(Rendo_Flags)0x80000444

#	define Rendo_COMMON_INTERPOLATIONS				(Rendo_Flags)0x80000777


#	define Rendo_PHONG_EDGES						(Rendo_Flags)0x80001000
#	define Rendo_GOURAUD_EDGES						(Rendo_Flags)0x80002000
#	define Rendo_GOOCH_EDGES						(Rendo_Flags)0x80004000
#	define Rendo_PHONG_FACES						(Rendo_Flags)0x80010000
#	define Rendo_GOURAUD_FACES						(Rendo_Flags)0x80020000
#	define Rendo_GOOCH_FACES						(Rendo_Flags)0x80040000
#	define Rendo_LIGHTING_INTERPOLATION				(Rendo_Flags)0x80077000

#	define Rendo_BUMP_PARALLAX						(Rendo_Flags)0x80100000


	Quantization_Flags		quantization;
#	define Quantization_THRESHOLD				(Quantization_Flags)0x01
#	define Quantization_DITHER					(Quantization_Flags)0x02
#	define Quantization_ERROR_DIFFUSION			(Quantization_Flags)0x04

	Technology_Flags		technology;
#	define		Technology_STANDARD						(Technology_Flags)0x01
#	define		Technology_RADIOSITY					(Technology_Flags)0x02
#	define		Technology_RAY_TRACE					(Technology_Flags)0x04
#	define		Technology_SOFTWARE_FRAME_BUFFER		(Technology_Flags)0x08

	Rendo_Debug				debug;
#	define		Rendo_Debug_NO_WRITE_FRAME_BUFFER		(Rendo_Debug)0x00000002
#	define		Rendo_Debug_SHOW_DEPTH_BUFFER			(Rendo_Debug)0x00000008
#	define		Rendo_Debug_DISABLE_HLR_RENDER			(Rendo_Debug)0x00000010



	/* some flags for display list invalidation */
#	define Rendo_TRISTRIP_MASK (Rendo_GOOCH_FACES|Rendo_PHONG_FACES|Rendo_GOURAUD_FACES|Rendo_TEXTURE_INTERPOLATION_FACES)
#	define Rendo_POLYEDGE_MASK (Rendo_GOOCH_EDGES|Rendo_PHONG_EDGES|Rendo_GOURAUD_EDGES|Rendo_TEXTURE_INTERPOLATION_EDGES)
#	define Rendo_POLYMARKER_MASK (Rendo_COLOR_INTERPOLATION_MARKERS)
#   define Rendo_ISOLINES_MASK ((Rendo_COLOR_INDEX_INTERPOLATION_FACES|Rendo_COLOR_INDEX_FACE_ISOLINES) & ~Rendo_SUBTYPE_MASK)

	int32_t				face_displacement;
	int32_t				vertex_displacement;
	int32_t				general_displacement;


#	define		HSR_NONE						(HSR_Algorithm)0x0000
#	define		HSR_HARDWARE_Z_BUFFER			(HSR_Algorithm)0x0001
#	define		HSR_PAINTERS					(HSR_Algorithm)0x0002
#	define		HSR_SOFTWARE_Z_BUFFER			(HSR_Algorithm)0x0004
#	define		HSR_UNUSED_1					(HSR_Algorithm)0x0008
#	define		HSR_Z_SORT_ONLY					(HSR_Algorithm)0x0010
#	define		HSR_PRIORITY					(HSR_Algorithm)0x0020
#	define		HSR_HIDDEN_LINE					(HSR_Algorithm)0x0040
#	define		HSR_Z_HIDDEN_LINE				(HSR_Algorithm)0x0080
#	define		HSR_DEPTH_PEELING				(HSR_Algorithm)0x0100	/* only for transparency */
#	define		HSR_TRANSPARENT					(HSR_Algorithm)0x8000
	HSR_Algorithm			hsr_algorithm;

	struct AO {
		AO():mask(0),value(0) {}
#	define		Antialias_OFF				0x0000
#	define		Antialias_SCREEN			0x0001
#	define		Antialias_TEXT				0x0002
#	define		Antialias_LINES				0x0004
#	define		Antialias_ON				(Antialias_SCREEN|Antialias_TEXT|Antialias_LINES)
#	define		Antialias_EVERYTHING		(Antialias_SCREEN|Antialias_TEXT|Antialias_LINES)

		short					mask;
		short					value;
	}	antialias_options;

	struct TO {
		TO():mask(0),value(0),style(0),hsr_algorithm(), peeling_layers(0), peeling_min_area(0) {}
		short				mask;
		short				value;
#	define		Transparency_STYLE				0x0001
#	define		Transparency_HSRA				0x0002
#	define		Transparency_UNUSED_0			0x0004
#	define		Transparency_PEELING_LAYERS		0x0008
#	define		Transparency_PEELING_MIN_AREA	0x0010
#	define		Transparency_PEELING_OPTIONS	0x0098
#	define		Transparency_ZSORT_FAST			0x0020
#	define		Transparency_DEPTH_WRITING		0x0040
#	define		Transparency_PEELING_BUFFER		0x0080

		char				style;
#	define		Transparency_STYLE_NONE				0
#	define		Transparency_STYLE_BLENDING			1
#	define		Transparency_STYLE_SCREEN_DOOR		2

		HSR_Algorithm		hsr_algorithm;	/* selected types per normal hsr above */

		int					peeling_layers;
		float				peeling_min_area;
	}	transparency_options;

	struct BO {
		BO():mask(0),value(0),size_limit(0),color_depth(0) {}
		Buffer_Flags		mask;
		Buffer_Flags		value;
#	define		Buffer_SIZE_LIMIT				(Buffer_Flags)0x01
#	define		Buffer_RETENTION				(Buffer_Flags)0x02
#	define		Buffer_COLOR_DEPTH				(Buffer_Flags)0x04
		int					size_limit;

		int					color_depth;
#	define		Color_Depth_MATCH_DEVICE		(Color_Depth)0x01
#	define		Color_Depth_FULL_COLOR			(Color_Depth)0x02
	}	buffer_options;

	Rendering_Options_LOD *				lod;
	Rendering_Options_HLO *				hlo;
	Rendering_Options_Effects *			effects;
	Rendering_Options_Geometry *		geometry;
	Rendering_Options_Locks *			locks;
	Rendering_Options_Isoline_Contour *	isoline_contour;

	float					vertex_decimation;

	float					depth_range[2];
	Rectangle				screen_range;
	float					image_scale[2];
	RGB						image_tint_color;
	RGB						diffuse_texture_tint_color;

	short					diffuse_tint_mask;
	short					diffuse_tint_value;
	Tinting					diffuse_tint;

#define Matrix_Override_CAMERA_SCALE			(Matrix_Overrides)0x0001
#define Matrix_Override_CAMERA_ROTATION			(Matrix_Overrides)0x0002
#define Matrix_Override_CAMERA_TRANSLATION		(Matrix_Overrides)0x0004
#define Matrix_Override_CAMERA_PERSPECTIVE		(Matrix_Overrides)0x0008
#define Matrix_Override_MODEL_SCALE				(Matrix_Overrides)0x0010
#define Matrix_Override_MODEL_ROTATION			(Matrix_Overrides)0x0020
#define Matrix_Override_MODEL_TRANSLATION		(Matrix_Overrides)0x0040
#define Matrix_Override_CAMERA_OFFSET			(Matrix_Overrides)0x0100
#define Matrix_Override_MODEL_OFFSET			(Matrix_Overrides)0x0200
#define Matrix_Override_CAMERA_NEAR_LIMIT		(Matrix_Overrides)0x0400

#define Matrix_Override_CAMERA_PROJECTION		(Matrix_Overrides)0x0800
#define Matrix_Override_CAMERA_PROJECTION_MASK	(Matrix_Overrides)0x3000
#define Matrix_Override_CAMERA_PROJECTION_SHIFT	12

#define Matrix_Override_NONE					(Matrix_Overrides)0x0000
#define Matrix_Override_CAMERA					(Matrix_Overrides)0x000F
#define Matrix_Override_MODEL					(Matrix_Overrides)0x0070
#define Matrix_Override_SCALE					(Matrix_Overrides)0x0011
#define Matrix_Override_ROTATION				(Matrix_Overrides)0x0022
#define Matrix_Override_TRANSLATION				(Matrix_Overrides)0x0044
#define Matrix_Override_PERSPECTIVE				(Matrix_Overrides)0x0008
#define Matrix_Override_ALL						(Matrix_Overrides)0x007F

	struct MO {
		MO():mask(0),value(0) {}
		Matrix_Overrides		mask;
		Matrix_Overrides		value;
	}						matrix_overrides;

	int						line_join_cutoff_angle;
	int						edge_join_cutoff_angle;

#	define Display_List_NONE					0
#	define Display_List_GEOMETRY				1
#	define Display_List_SEGMENT					2
#	define Display_List_SEGMENT_TREE			3
	int						display_list_mode;


	Set_Clear_Rendo_Debug *	set_clear_debug;


public:

	Rendering_Options();
	Rendering_Options(Rendering_Options const & that);

	~Rendering_Options();

	bool operator == (Rendering_Options const & that) const;

	Attribute * clone() const {return NEW(Rendering_Options)(*this);};

	bool equal(Attribute const * attr) const {
		if (attr->type != RENDERING_OPTIONS)
			return false;
		return *this == *(Rendering_Options*)attr;
	}

	Attribute * merge (Thread_Data * thread_data, Attribute const * attr, void * control=0);

	Attribute * acquire (Thread_Data *	thread_data, Anything * item, Attribute const * old_attribute);

	bool unset_acquire (Thread_Data * thread_data, Anything * item, Attribute const * unset_proto);

	int hash_key() const {

		Rendering_Options const * ro = this;
		int hash = ro->complex_mask + ro->complex_value + ro->simple_mask + ro->simple_value + ro->interpolation_mask + ro->interpolation_value;

		if (ANYBIT (ro->simple_value, Rendo_DEPTH_RANGE)) {
			uint32_t add_in = Float::extract_uint32_t(ro->depth_range[0]);
			hash += add_in;
			add_in = Float::extract_uint32_t(ro->depth_range[1]);
			hash += add_in;
		}
	
		return hash;
	};


	static Rendering_Options * Default(Thread_Data * thread_data);

private:
	Rendering_Options const & operator= (Rendering_Options const & that);
};

typedef int32_t Font_Flags;

#define FF_NAME					(Font_Flags)0x00000001
#define FF_SIZE					(Font_Flags)0x00000002
#define FF_SIZE_TOLERANCE		(Font_Flags)0x00000004
#define FF_TRANSFORM			(Font_Flags)0x00000008
#define FF_ROTATION				(Font_Flags)0x00000010
#define FF_WIDTH_SCALE			(Font_Flags)0x00000020
#define FF_EXTRA_SPACE			(Font_Flags)0x00000040
#define FF_SLANT				(Font_Flags)0x00000080
#define FF_GREEKING_LIMIT		(Font_Flags)0x00000100
#define FF_LINE_SPACING			(Font_Flags)0x00000200
#define FF_EVEN_SPACING			(Font_Flags)0x00000400
#define FF_OUTLINE				(Font_Flags)0x00000800
#define FF_UNDERLINE			(Font_Flags)0x00001000
#define FF_STRIKETHROUGH		(Font_Flags)0x00002000
#define FF_OVERLINE				(Font_Flags)0x00004000
#define FF_FOLLOW_PATH			(Font_Flags)0x00008000
#define FF_UNIFORM_SPACING		(Font_Flags)0x00010000
#define FF_FILL_EDGES			(Font_Flags)0x00020000
#define FF_BOLD					(Font_Flags)0x00040000
#define FF_ITALIC				(Font_Flags)0x00080000
#define FF_RENDERER				(Font_Flags)0x00100000
#define FF_GREEKING_MODE		(Font_Flags)0x00200000
#define FF_PREFERENCE			(Font_Flags)0x00400000
#define FF_LAYOUT		  		(Font_Flags)0x00800000

#define FL_LEGACY		(Font_Layout)0
#define FL_UNICODE	 	(Font_Layout)1

#define	FGM_NONE		(Font_Greeking_Mode)0
#define	FGM_LINES		(Font_Greeking_Mode)1
#define	FGM_BOX			(Font_Greeking_Mode)2

#define FR_DEFAULT		(Font_Renderer)0
#define FR_DRIVER		(Font_Renderer)1
#define FR_TRUETYPE		(Font_Renderer)2
#define FR_DEFINED		(Font_Renderer)3

typedef	char	Font_Renderer, Font_Layout, Font_Greeking_Mode, Font_Preference;

class Text_Font : public Attribute {
public:
	Font_Name_List		names;

	Font_Flags			flags_mask;
	Font_Flags			flags_value;

	float				size;
	float				size_tolerance;
	float				extra_space;
	float				greeking_limit;
	float				rotation;
	float				width_scale;
	float				line_spacing;
	float				slant;
	float				renderer_cutoff;
	float				preference_cutoff;

	/* (put all the "char" items at the end of the structure) */

	Font_Transform		transform;		/* choose one */

	Font_Renderer		renderers[2];

	Font_Preference		preferences[2];

	Font_Size_Units		size_units;
	Font_Size_Units		size_tolerance_units;
	Font_Size_Units		extra_space_units;
	Font_Size_Units		greeking_limit_units;
	Font_Size_Units		renderer_cutoff_units;
	Font_Size_Units		preference_cutoff_units;

	Font_Greeking_Mode	greeking_mode;

	Font_Layout			layout;

	Text_Font();

	bool operator == (Text_Font const & that) const;

	Attribute * clone() const {return NEW(Text_Font)(*this);};

	bool equal(Attribute const * attr) const {
		if (attr->type != TEXT_FONT)
			return false;
		return *this == *(Text_Font*)attr;
	}

	Attribute * merge (Thread_Data * thread_data, Attribute const * attr, void * control=0);

	Attribute * acquire (Thread_Data *	thread_data, Anything * item, Attribute const * old_attribute);

	bool unset_acquire (Thread_Data * thread_data, Anything * item, Attribute const * unset_proto);

	int hash_key() const {
		return flags_mask + flags_value + transform + size_units;
	};

	static const Name_Const system_default;
	static const Name_Const sans_serif;
	static const Name_Const typewriter;
	static const Name_Const roman;
	static const Name_Const stroked;

	static const char * SYSTEM_DEFAULT() {return system_default.text();}
	static const char * SANS_SERIF() {return sans_serif.text();}
	static const char * TYPEWRITER() {return typewriter.text();}
	static const char * ROMAN() {return roman.text();}
	static const char * STROKED() {return stroked.text();}

	static const char * generic_name_lookup(int index);

	void set_size(float in_size, HPS::Text::SizeUnits units);
	bool show_size(float & out_size, HPS::Text::SizeUnits & out_units) const;

	void set_bold (bool state);
	bool show_bold(bool & out_state) const;

	void set_italic(bool state);
	bool show_italic(bool & out_state) const;

	void set_overline(bool state);
	bool show_overline(bool & out_state) const;

	void set_strikethrough(bool state);
	bool show_strikethrough(bool & out_state) const;

	void set_underline(bool state);
	bool show_underline(bool & out_state) const;

	void set_slant(float angle);
	bool show_slant(float & out_angle) const;

	void set_line_spacing(float multiplier);
	bool show_line_spacing(float & out_multiplier) const;

	void set_width_scale(float scale);
	bool show_width_scale(float & out_scale) const;

	void set_rotation(HPS::Text::Rotation in_rotation, float angle);
	bool show_rotation(HPS::Text::Rotation & out_rot, float & out_angle) const;

	void set_extra_space(bool state, float size, HPS::Text::SizeUnits units);
	bool show_extra_space(bool & out_state, float & out_size, HPS::Text::SizeUnits & out_units) const;

	void set_greeking(bool state, float size, HPS::Text::GreekingUnits units, HPS::Text::GreekingMode mode);
	bool show_greeking(bool & out_state, float & out_size, HPS::Text::GreekingUnits & out_units, HPS::Text::GreekingMode & out_mode) const;

	void set_size_tolerance(bool state, float size, HPS::Text::SizeToleranceUnits units);
	bool show_size_tolerance(bool & out_state, float & out_size, HPS::Text::SizeToleranceUnits & out_units) const;

	void set_font(const char * in_cstr_name);
	const char * show_font() const;

	void set_transform(HPS::Text::Transform trans);
	bool show_transform(HPS::Text::Transform & out_trans) const;

	void set_renderer(HPS::Text::Renderer rend);
	bool show_renderer(HPS::Text::Renderer & out_rend) const;

	void set_preference(HPS::Text::Preference pref);
	bool show_preference(float & cutoff, HPS::Text::SizeUnits & units, HPS::Text::Preference & smaller, HPS::Text::Preference & larger) const;
	void set_preference_detailed(float cutoff, HPS::Text::SizeUnits units, HPS::Text::Preference smaller, HPS::Text::Preference larger);


	static Text_Font * Default(Thread_Data * thread_data);
};



struct ARGB32 {
	unsigned char		a, r, g, b;
};


struct Int_RGB {
	int					r, g, b;
};

struct HLS {
	float				hue, lightness, saturation;
};


// the following two items don't include diffuse, which is in a separate linked list

#define MAT_SPECULAR		0
#define MAT_MIRROR			1
#define MAT_TRANSMISSION	2
#define MAT_EMISSION		3
#define MAT_BUMP			4
#define MAT_ENVIRONMENT		5

typedef uint32_t Material_Channel_Flags;
#define MCF_None					(Material_Channel_Flags)0x0000
#define MCF_Modulate_Color			(Material_Channel_Flags)0x0001  // modulate the texture by its attenuation_color
#define MCF_Explicit				(Material_Channel_Flags)0x0002  // textures must exactly match definitions
#define MCF_Tail_Extend				(Material_Channel_Flags)0x0004  // propagate to all subsequent channels
#define MCF_Modulate_From_Diffuse	(Material_Channel_Flags)0x0008  // if modulating the texture use the diffuse_color

class Texture_Control_List : public CMO_Pooled {

public:

	Texture_Control_List *		next;
	Texture *					texture;
	Name						name;
	float						opacity;
	int							order;
	Material_Channel_Flags		flags;

	Texture_Control_List() : next(0), texture(null) {
		;
	}

	Texture_Control_List(Texture * texture) : next(0), texture(texture){
		UTILE(texture);
	}

	Texture_Control_List(Texture_Control_List const & that) 
		: next(null), texture(that.texture), name(that.name), opacity(that.opacity), order(that.order), flags(that.flags) {
		if (texture != null)
			UTILE(texture);
	}

	~Texture_Control_List() {
		if (texture != null)
			INUTILE(texture);
		delete next;
	}


	static Texture_Control_List * Copy(Texture_Control_List const * tcl, Memory_Pool * pool = null);

	static Texture_Control_List * Mix(Texture_Control_List const * oldlist, Texture_Control_List const * newlist, Memory_Pool * pool=null);

private:
	Texture_Control_List const & operator= (Texture_Control_List const & that);
};

class Named_Material : public CMO {

public:

	static const int TEXTURE_COUNT = 6;
	static const int COLOR_COUNT = 4;

	Texture *					texture[TEXTURE_COUNT];	// non-diffuse channels only
	Texture_Control_List *		diffuse_textures;
	RGB							diffuse_color;
	Name						diffuse_name;
	RGB							color[COLOR_COUNT];
	Name						name[COLOR_COUNT];
	Material_Channel_Flags		flags[TEXTURE_COUNT];

	Material_Components			combined_color_flags;

	float						gloss;
	float						index_of_refraction;
	float						opacity;
	Material_Components			components;

	Named_Material() {

	}

	Named_Material(Named_Material const & that) {
		*this = that;
	}

	~Named_Material();

	void operator = (Named_Material const & that);

	bool operator == (Named_Material const & that) const;

	INLINE bool operator != (Named_Material const & that) const {
		return !(*this == that);
	};

	INLINE bool IsSimple() const {
		return !(ANYBIT(components, ~(M_DIFFUSE)) || diffuse_textures != null);
	}

	INLINE bool IsTransparent() const {
		if ((ANYBIT(components, M_TRANSMISSION_TEXTURE) && texture[MAT_TRANSMISSION] != null) ||
			(ANYBIT(components, M_TRANSMISSION_COLOR) && color[MAT_TRANSMISSION] != RGB::Black()))
			return true;
		return false;
	}

	static INLINE Material_Components Color_Components(size_t i)
	{
		ASSERT(i<COLOR_COUNT);
		switch(i) {
		case 0: return M_SPECULAR_COLOR;
		case 1: return M_MIRROR_COLOR;
		case 2: return M_TRANSMISSION_COLOR;
		case 3: return M_EMISSION_COLOR;
		default: break;
		}
		return 0;
	}

	static INLINE Material_Components Texture_Components(size_t i)
	{
		ASSERT(i<TEXTURE_COUNT);
		switch(i) {
		case 0: return M_SPECULAR_TEXTURE;
		case 1: return M_MIRROR_TEXTURE;
		case 2: return M_TRANSMISSION_TEXTURE;
		case 3: return M_EMISSION_TEXTURE;
		case 4: return M_BUMP_TEXTURE;
		case 5: return M_ENVIRONMENT_TEXTURE;
		default: break;
		}
		return 0;
	}
};


class Named_Material_Handle {

public:

	INLINE Named_Material const *	operator-> () const	{ return named_material; }

	INLINE bool operator== (Null_Object const &) const { return named_material == null; }
	INLINE bool operator!= (Null_Object const &) const { return named_material != null; }

	INLINE Named_Material const & operator* () const { return *named_material; }

	INLINE Named_Material_Handle() : named_material(null), owns(false){};
	INLINE Named_Material_Handle(Named_Material const * named_material, bool owns = false) : named_material(named_material), owns(owns) {};
	INLINE ~Named_Material_Handle() {
		if (owns)
			delete named_material;
	};

private:
	Named_Material const * named_material;
	bool owns;
};


class Color_List : public CMO {
public:

	enum Types {
		FINDEX=0,
		NORMAL,
		GENERAL
	};
	
	Color_List(Color_List::Types type) : next(null), driver_data(null), objects(0), type(type) {};

	~Color_List() {
		delete next;
	}

	virtual bool equal(Color_List const * cb) const {UNREFERENCED(cb); ASSERT(0); return 0;};

	virtual bool IsTransparent() const { ASSERT(type != GENERAL); return false; }

	// copy a list whole with optional transformations
	static Color_List * Copy(Color_List const * old_list, Color_Map const * map = 0, Attribute_Lock const * locks = 0);

	Color_List *		next;
	void *				driver_data;
	Color_Object		objects;
	Types				type;

private:
	Color_List();
};

class Color_Findex : public Color_List {
public:
	Color_Findex() : Color_List(Color_List::FINDEX) {};

	bool equal(Color_List const * cb) const {
		if (cb->type != type)
			return false;
		return *this == *(Color_Findex*)cb;
	}

	INLINE bool operator == (Color_Findex const & that) const {
		ASSERT(type == that.type);

		if (value != that.value)
			return false;

		return true;
	}

	Color_Float_Index	value;
};

class Color_Normal : public Color_List {
public:
	Color_Normal() : Color_List(NORMAL) {};

	bool equal(Color_List const * cb) const {
		if (cb->type != type)
			return false;
		return *this == *(Color_Normal*)cb;
	}

	INLINE bool operator == (Color_Normal const & that) const {
		ASSERT(type == that.type);

		if (rgb != that.rgb)
			return false;

		return true;
	}

	RGB					rgb;
	Name				name;
};

class Color_General : public Color_List {
public:

	Color_General() : Color_List(GENERAL) {};

	bool equal(Color_List const * cb) const {
		if (cb->type != type)
			return false;
		return *this == *(Color_General*)cb;
	}

	bool IsTransparent() const {
		return material.IsTransparent(); 
	}

	INLINE bool operator == (Color_General const & that) const {
		ASSERT(type == that.type);

		if (material != that.material)
			return false;

		return true;
	}

	Named_Material		material;
	Name				name;
};


#define HASH_RGB(_c_) (int)((37.0f * (_c_).red) + (86.0f * (_c_).green) + (144.0f * (_c_).blue))

#define Color_TRANSPARENT			DB_TYPE_SPECIFIC(1)
#define Color_TEXTURES_RECORDED		DB_TYPE_SPECIFIC(2)

class Color : public Attribute {
public:
	Color_List *		colors;
	Color_Object		objects;
	Color_Object		changed;

	Color():Attribute(COLOR), colors(null), objects(0), changed(0) {}

	Color(Color const & that) {
	
		objects = that.objects;
		changed = Color_NONE;
		colors = Color_List::Copy (that.colors);
	}

	~Color() {

		if (ANYBIT(dbflags, Color_TEXTURES_RECORDED)) {
			ASSERT(owner != null);
			scan_textures(owner, false);
		}

		delete colors;
	}

	INLINE bool operator == (Color const & that) const {

		Color const *		con_color = this;
		Color const *		can_color = &that;

		Color_List const *	first = con_color->colors;

		if (con_color->objects != can_color->objects)
			return false;

		do {
			Color_List const *	second = can_color->colors;

			do if (first->objects == second->objects) {

				if (first->type != second->type)
					return false;

				switch (first->type) {
					case Color_List::FINDEX: {
						Color_Findex const *	ffi= (Color_Findex const *)first;
						Color_Findex const *	sfi = (Color_Findex const *)second;

						if (ffi->value != sfi->value)
							return false;
					} break;

					case Color_List::NORMAL: {
						Color_Normal const *	fn = (Color_Normal const *)first;
						Color_Normal const *	sn = (Color_Normal const *)second;

						if (fn->rgb != sn->rgb)
							return false;
					} break;

					case Color_List::GENERAL: {
						Color_General const *	fg = (Color_General const *)first;
						Color_General const *	sg = (Color_General const *)second;

						if (fg->material != sg->material)
							return false;
					} break;
				}
				break;	// this one was ok, continue with outer loop
			} while ((second = second->next) != null);

			if (second == null)	// if no list entry matches the objects
				return false;

		} while ((first = first->next) != null);

		return true;
	}

	Attribute * clone() const {return NEW(Color)(*this);}

	bool equal(Attribute const * attr) const {
		if (attr->type != COLOR)
			return false;
		return *this == *(Color*)attr;
	}

	//Attribute * merge (Thread_Data * thread_data, Attribute const * attr, void * control=0);

	Attribute * acquire (Thread_Data *	thread_data, Anything * item, Attribute const * old_attribute);

	bool unset_acquire (Thread_Data * thread_data, Anything * item, Attribute const * unset_proto);

	int hash_key() const {

		Color_List const *	list = colors;

		int	value = (int)objects;

		do {
			switch (list->type) {
				case Color_List::FINDEX: {
					Color_Findex const *	findex = (Color_Findex const *)list;
					value += (int)findex->value;
				}	break;

				case Color_List::NORMAL: {
					Color_Normal const *	normal = (Color_Normal const *)list;
					value += HASH_RGB (normal->rgb);
				}	break;

				case Color_List::GENERAL: {
					Color_General const *			general = (Color_General const *)list;
					Named_Material const *			mat = &general->material;
					Material_Components				components = mat->components;
					Texture_Control_List const *	tcl = mat->diffuse_textures;

					value += mat->components;
					value += mat->combined_color_flags;

					while (tcl != null) {
						value += texture_hash (tcl->texture);
						value += (int)(tcl->order * tcl->opacity);
						value += tcl->flags;

						tcl = tcl->next;
					}

					value += HASH_RGB (mat->diffuse_color);

					for (int i=0; i<Named_Material::COLOR_COUNT; i++)
						if (ANYBIT (components, Named_Material::Color_Components(i)))
							value += HASH_RGB (mat->color[i]);

					for (int i=0; i<Named_Material::TEXTURE_COUNT; i++)
						if (ANYBIT (components, Named_Material::Texture_Components(i)))
							value += texture_hash (mat->texture[i]);

					if (ANYBIT (components, M_TRANSMISSION_COLOR))
						value += (int)(64.0f * mat->opacity);

					if (ANYBIT (components, M_GLOSS))
						value += (int)(32.0f * mat->gloss);

					if (ANYBIT (components, M_INDEX))
						value += (int)(128.0f * mat->index_of_refraction);
				}	break;
			}
		} while ((list = list->next) != null);

		return value;
	}

	// merge color list into color with possible transformations
	void Merge(Color_List const * new_list, Attribute_Lock const * locks = 0, Color_Map const * map = 0);


	static Color * Default(Thread_Data * thread_data);


	void scan_textures(Segment * seg, bool hook) const;

	// sets or unsets whole tree return if did anything
	static bool Track_All_Textures(bool hook);

private:
	local int texture_hash (Texture const *	texture) {
		Name_List const *	nl;

		if (texture == null)
			return 0;

		int	value = texture->flags;

		if ((nl = texture->source_names) != null) do {
			value += nl->name.hash_key();
			nl = nl->next;
		} while (nl != null);

		return value;
	}

	static bool update_texture_hooks(Segment * seg, Name_List const * nl, bool hook);

	Color const & operator= (Color const & that);
};


#define Color_Map_TRANSPARENT	DB_TYPE_SPECIFIC(1)

class Color_Map : public Attribute {
public:
	Named_Material *	materials;
	void *				driver_data;

	Material_Components	component_summary;
	int32_t				length;
	uint32_t			generation;

	Color_Map():Attribute(COLOR_MAP), materials(null), driver_data(null), component_summary(0), length(0), generation(0) {}

	Color_Map(Color_Map const & that);

	~Color_Map();

	INLINE bool operator == (Color_Map const & that) const {
	
		Color_Map const *	concm = (Color_Map const *)this;
		Color_Map const *	cancm = (Color_Map const *)&that;

		if (concm->length != cancm->length)
			return false;

		if (component_summary != that.component_summary)
			return false;

		for (int i=0; i<concm->length; i++) {
			if (concm->materials[i] != cancm->materials[i])
				return false;
		}

		return true;
	}

	Attribute * clone() const {return NEW(Color_Map)(*this);}

	bool equal(Attribute const * attr) const {
		if (attr->type != COLOR_MAP)
			return false;
		return *this == *(Color_Map*)attr;
	}

	//Attribute * merge (Thread_Data * thread_data, Attribute const * attr, void * control=0);

	Attribute * acquire (Thread_Data *	thread_data, Anything * item, Attribute const * old_attribute);

	int hash_key() const {

		int hash = 0;

		hash += length;
		hash += (int)component_summary;

		return hash;
	}


	static Color_Map * Default(Thread_Data * thread_data);

private:
	Color_Map const & operator= (Color_Map const & that);
};


class Text_Path : public Attribute {
public:
	Vector				value;

	Text_Path():Attribute(TEXT_PATH), value(Vector::Zero()) {}

	INLINE bool operator == (Text_Path const & that) const {
		return (value == that.value);
	}

	Attribute * clone() const {return NEW(Text_Path)(*this);};

	bool equal(Attribute const * attr) const {
		if (attr->type != TEXT_PATH)
			return false;
		return *this == *(Text_Path*)attr;
	}

	Attribute * merge (Thread_Data * thread_data, Attribute const * attr, void * control=0);

	Attribute * acquire (Thread_Data *	thread_data, Anything * item, Attribute const * old_attribute);

	int hash_key() const {return (int)(value.x+value.y+value.z);};

	void set(Vector const & in_path)
	{
		value = in_path;
		value.Normalize();
	}

	bool show(Vector & out_path) const
	{
		out_path = value;
		return true;
	}

	static Text_Path * Default(Thread_Data * thread_data);
};


class Callback : public Attribute {
public:
	Callback_List *		callback_list;

	Callback():Attribute(IM_CALLBACK), callback_list(null) {}

	Callback(Callback const & that);

	~Callback();

	bool operator == (Callback const & that) const;

	Attribute * clone() const {return NEW(Callback)(*this);}

	bool equal(Attribute const * attr) const {
		if (attr->type != IM_CALLBACK)
			return false;
		return *this == *(Callback*)attr;
	}

	Attribute * merge (Thread_Data * thread_data, Attribute const * attr, void * control=0);

	Attribute * acquire (Thread_Data *	thread_data, Anything * item, Attribute const * old_attribute);

	bool unset_acquire (Thread_Data *	thread_data, Anything * item, Attribute const * old_attribute);

	int hash_key() const;

	static Callback * Default(Thread_Data * thread_data);

private:
	Callback const & operator= (Callback const & that);
};




enum Callback_Type {
	CBT_ZERO = 0,

	// segment block	
	CBT_POPULATE_SEGMENT, 
		CBT_Block_Start_SEGMENT = CBT_POPULATE_SEGMENT,
		CBT_FIRST_ACTION = CBT_POPULATE_SEGMENT,
	CBT_PROCESS_SEGMENT,
	CBT_DRAW_SEGMENT,
	CBT_DRAW_SEGMENT_TREE,
		CBT_Block_End_SEGMENT = CBT_DRAW_SEGMENT_TREE,

	// draw dc block
	CBT_DRAW_DC_MARKER, 
		CBT_Block_Start_DC = CBT_DRAW_DC_MARKER,
	CBT_DRAW_DC_EDGE,
	CBT_DRAW_DC_FACE,
	CBT_DRAW_DC_COLORIZED_MARKER,
	CBT_DRAW_DC_COLORIZED_FACE,
	CBT_DRAW_DC_LINE,
	CBT_DRAW_DC_COLORIZED_LINE,
	CBT_DRAW_DC_GOURAUD_LINE,
	CBT_DRAW_DC_RESHADED_LINE,
	CBT_DRAW_DC_TRIANGLE,
	CBT_DRAW_DC_COLORIZED_TRIANGLE,
	CBT_DRAW_DC_GOURAUD_TRIANGLE,
	CBT_DRAW_DC_RESHADED_TRIANGLE,
	CBT_DRAW_DC_POLYTRIANGLE,
	CBT_DRAW_DC_COLORIZED_POLYTRIANGLE,
	CBT_DRAW_DC_GOURAUD_POLYTRIANGLE,
	CBT_DRAW_DC_PHONG_POLYTRIANGLE,
	CBT_DRAW_DC_TEXTURED_POLYTRIANGLE,
	CBT_DRAW_DC_POLYLINE,
	CBT_DRAW_DC_COLORIZED_POLYLINE,
	CBT_DRAW_DC_GOURAUD_POLYLINE,
	CBT_DRAW_DC_PHONG_POLYLINE,
	CBT_DRAW_DC_TEXTURED_POLYLINE,
	CBT_DRAW_DC_POLYMARKER,
	CBT_DRAW_DC_COLORIZED_POLYMARKER,
	CBT_DRAW_DC_POLYDOT,
	CBT_DRAW_DC_TEXT_STROKES,
	CBT_DRAW_DC_TEXT_AREAS,
	CBT_DRAW_DC_CUT_LINE,
	CBT_DRAW_WINDOW,
	CBT_DRAW_WINDOW_FRAME,
	CBT_DRAW_TEXT,
		CBT_Block_End_DC = CBT_DRAW_TEXT,

	// draw 3d block
	CBT_DRAW_3D_MARKER,
		CBT_Block_Start_3D = CBT_DRAW_3D_MARKER,
	CBT_DRAW_3D_POLYLINE,
	CBT_DRAW_3D_POLYGON,
	CBT_DRAW_3D_TEXT,
	CBT_DRAW_3D_POLYEDGE,
	CBT_DRAW_3D_POLYMARKER,
	CBT_DRAW_3D_TRISTRIP,
	CBT_DRAW_3D_GEOMETRY,
	CBT_DRAW_3D_TEXT_STROKES,
	CBT_DRAW_3D_TEXT_AREAS,
	CBT_DRAW_3D_SPHERE,
	CBT_DRAW_3D_CYLINDER,
	CBT_DRAW_3D_POLYCYLINDER,
	CBT_DRAW_3D_NURBS_CURVE,
	CBT_DRAW_3D_NURBS_SURFACE,
	CBT_DRAW_3D_MULTIMARKER,
	CBT_DRAW_3D_POLYHEDRON,
	CBT_DRAW_3D_CONTOUR,
	CBT_DRAW_3D_IMAGE,
	CBT_DRAW_3D_GRID,
	CBT_DRAW_3D_ELLIPSE,
	CBT_DRAW_3D_ELLIPTICAL_ARC,
	CBT_DRAW_3D_ISOLINE,
	CBT_DRAW_3D_INFINITE_LINE,
		CBT_Block_End_3D = CBT_DRAW_3D_INFINITE_LINE,

	// driver block
	CBT_DRIVER_STARTUP,
		CBT_Block_Start_Driver = CBT_DRIVER_STARTUP,
	CBT_DRIVER_SHUTDOWN,
	CBT_INIT_PICTURE,
	CBT_FINISH_PICTURE,
	CBT_CREATE_REGION,
	CBT_SAVE_REGION,
	CBT_RESTORE_REGION,
	CBT_DESTROY_REGION,
	CBT_SET_SHADER,
		CBT_Block_End_Driver = CBT_SET_SHADER,

	// select block
	CBT_SELECT_3D_MARKER,
		CBT_Block_Start_Select = CBT_SELECT_3D_MARKER,
	CBT_SELECT_3D_POLYLINE,
	CBT_SELECT_3D_POLYGON,
	CBT_SELECT_3D_TEXT,
	CBT_SELECT_3D_GEOMETRY,
	CBT_SELECT_TEXT,
	CBT_SELECT_WINDOW,
	CBT_SELECT_SEGMENT,
	CBT_SELECT_SEGMENT_TREE,
		CBT_Block_End_Select = CBT_SELECT_SEGMENT_TREE,
	
	// hlr block
	CBT_HLR_POLYLINE,
		CBT_Block_Start_HLR = CBT_HLR_POLYLINE,
	CBT_HLR_MARKER,
	CBT_HLR_HIDDEN_POLYLINE,
	CBT_HLR_HIDDEN_MARKER,
	CBT_HLR_HIDDEN_POLYDOT,
	CBT_HLR_HIDDEN_COLORIZED_POLYDOT,
	CBT_HLR_POLYDOT,
	CBT_HLR_COLORIZED_POLYDOT,
		CBT_Block_End_HLR = CBT_HLR_COLORIZED_POLYDOT,

	// not draw time rather database side
	CBT_SEGMENT_ACTIVITY_CHANGE,

	// keep this correct!
	CBT_LAST_ACTION = CBT_SEGMENT_ACTIVITY_CHANGE,

	// parse only
	CBT_NO_REDRAW,
};

struct Callback_List {
	Callback_List *		next;
	Callback_Name		name;
	void const *		associated_data;

	Callback_Type		type;
	bool				stealth;


	local void free_list (Callback_List * item);

	local Callback_List * copy (Callback_List const * in);

	local Callback_List * merge (Callback_List const * older, Callback_List const * newer);
};



#define Font_BY_REFERENCE		DB_TYPE_SPECIFIC(1)

struct Font : public Minimum_Header<Font, World> {
	int					data_length;
	char const *		data;
	Callback_Name		lookup;
	Name				name;
	Text_Encoding		encoding;
	char				pad;

	char const * const *stroke_defs;
	int					num_stroke_defs;
	bool				system_font;
	float				aspect;
};

typedef void (HC_CDECL * Font_Lookup_Routine)
				(char const * name, char const * data, Karacter kar, char const * * defs);

typedef void (HC_CDECL * Texture_Lookup_Routine)
				(int widht, int height, int current_row,
				 int * rwidth, int * totalheight, int * rheight, char ** defs,
				 int counter, void * data);


#define CHAR_INDEX1(p)	UNSIGNED_CHARTOINT(*(p))
#define CHAR_INDEX2(p)	((UNSIGNED_CHARTOINT(*(p)) << 8) + \
						 UNSIGNED_CHARTOINT(*((p)+1)))


class User_Value : public Attribute {
public:
	intptr_t	data;

	User_Value():Attribute(USER_VALUE), data(0) {};

	INLINE bool operator == (User_Value const & that) const {
		return (data == that.data);
	};

	Attribute * clone() const {return NEW(User_Value)(*this);};

	bool equal(Attribute const * attr) const {
		if (attr->type != USER_VALUE)
			return false;
		return *this == *(User_Value*)attr;
	}

	Attribute * merge (Thread_Data * thread_data, Attribute const * attr, void * control=0);

	Attribute * acquire (Thread_Data *	thread_data, Anything * item, Attribute const * old_attribute);

	int hash_key() const {return (int)data;};

	static User_Value * Default(Thread_Data * thread_data);
};

struct Metafile {
#	define		META_READ						(int)0x00000001
#	define		META_WRITE						(int)0x00000002
#	define		META_NAME						(int)0x00000004
#	define		META_DIRECTORY					(int)0x00000008
#	define		META_COMMENT					(int)0x00000010
#	define		META_ORIGINAL_NAME				(int)0x00000020
#	define		META_FOLLOW_CROSS_REFERENCES	(int)0x00000040
#	define		META_OVERWRITE					(int)0x00000080
#	define		META_SAVE_STATE					(int)0x00000100
#	define		META_USE_COLOR_NAMES			(int)0x00000200
#	define		META_RESTORE_STATE				(int)0x00000400
#	define		META_EXTENSION_MODE				(int)0x00000800
#	define		META_TRISTRIPS_MODE				(int)0x00001000
#	define		META_SUPPRESS_LOD				(int)0x00002000
#	define		META_SUPPRESS_CAMERA			(int)0x00004000
#	define		META_RELATIVE_LIMIT				(int)0x00008000
#	define		META_PREPEND_ROOT				(int)0x00010000
	int					mask,
						value;
#	define		META_STATE_ALIASES				(int)0x0001
#	define		META_STATE_COLOR_NAMES			(int)0x0002
#	define		META_STATE_OPEN_ITEMS			(int)0x0004
#	define		META_STATE_FONTS				(int)0x0008
#	define		META_STATE_TEXTURES				(int)0x0010
#	define		META_STATE_EVERYTHING			(int)0x001F
	int					save_mask,
						save_value,
						restore_mask,
						restore_value;
	unsigned int		relative_limit;

	Name				name;
	Name				directory;
	Name				comment;
	Name				original_name;
	Name				metafile_root;

#	define		META_EXTMODE_AUTO				0
#	define		META_EXTMODE_DISABLE			1
#	define		META_EXTMODE_FORCE				2
	int					filename_extension_mode;
#	define		META_TRISTRIPS_AUTO				0
#	define		META_TRISTRIPS_SUPPRESS			1
#	define		META_TRISTRIPS_FORCE			2
	int					tristrips_mode;
	bool				use_fresh_output_file;
};

typedef int32_t		Driver_Flags;		 /* defined further in "please.h" */

struct String_List {
	String_List *	next;
	String_List *	prev;
	KName			string;
};


enum {
	Display_List_UNSHAREABLE = 0,
	Display_List_OPENGL = 1,
	Display_List_DX9 = 2,
	Display_List_OPENGL2 = 3,
	Display_List_DX11 = 4
};


#define ACTOR_PLEASE(_thread_data_,_actor_, _request_, _request_info_) \
	((_actor_)->connected_action)((_thread_data_),(Actor*)(_actor_), _request_, _request_info_)

struct Actor : public Attribute {

private:

	Actor const & operator= (Actor const & that);

public:

	mutable Task_Queue * _task_queue; // for updates and display list cleanup
	mutable Task_List *	 _delayed_task_list; // for delayed updates

	Actor() : Attribute(ACTOR) {
		;
	}

	Actor(Actor const & that) : Attribute(ACTOR) {

		ASSERT(0);
		action = that.action;
		connected_action = that.action;

		extent.generation = 0;
		extent.segcount = 0;
		extent.segments = null;
		dl_cleanup_list = null;
		_delayed_task_list = null;
		_task_queue = null;
	}

	~Actor();

	Actor * clone() const { return NEW(Actor)(*this); }

	Attribute * acquire(Thread_Data *	thread_data, Anything * item, Attribute const * old_attribute);

	void Init(Thread_Data * thread_data, bool dependent_driver);

	void Add_Task(Task * task) const {
#ifdef HOOPS_DEBUG_BUILD
		Task_Type task_type = task->Type();
		ASSERT(task_type == Task_Type_DL_Delete_Task || task_type == Task_Type_Standard_Driver_Task || task_type == Task_Type_Actor_Image_Message_Pump_Task);
#endif
		if (_task_queue != null)
			_task_queue->Add_Task(task);
		else
			task->Do_Task(false);
	}

	void Add_Delayed_Task(Task * task) const {
		if (_delayed_task_list != null)
			_delayed_task_list->Add_Task(task);
		else
			task->Do_Task(false);
	}

	// process all delayed tasks
	void Process_Delayed_Tasks() const {
		while (_delayed_task_list != null) {
			Task *	task = _delayed_task_list->Get_Task();
			if (task == null)
				break;
			task->Do_Task(false);
		}
	}

	// process tasks until the requested one is done
	void Process_Delayed_Tasks(Task const * the_task) const {
		while (_delayed_task_list != null) {
			Task *	task = _delayed_task_list->Get_Task();
			if (task == null)
				break;
			task->Do_Task(false);
			if (the_task == task)
				break;
		}
	}


	Time				last_update_time;
	Actor *				actor_list;
	Actor **			actor_list_backlink;

	Driver_Action		action;
	Driver_Action		connected_action;
	Driver_Action		secondary_action; // for two-level drivers GLX

	void *				driver_data;
	void *				driver_data2; // used only by Dynamic_Driver

	Driver_DL *			dl_cleanup_list;

	uint32_t			update_generation;
	uint32_t			update_level;
	uint32_t			please_generation;


	Driver_Flags		dflags;

	int					dl_share_type;
	uint32_t			hps_driver_type;

#define ALUI_COMPLETE		0x00000000	// normal non-interrupted update
#define ALUI_INTERRUPTED	0x00000001	// update was interrupted by something
#define ALUI_TIMED_OUT		0x00000002	// timed update expired
#define ALUI_STOP_SEGMENT	0x00000004	// limited from stop seg special timed out
#define ALUI_EVENT			0x00000010	// aborted from user event or IM_Abort
#define ALUI_KILLED			0x00000020	// device was was killed
#define ALUI_DRIVER			0x00000040	// device was not ready or was iconic

	int					updates_info;

	struct Extent_Cache {	
		Vector2D			resolution;
		Int_XY				screen_pixels;
		IntRectangle		window_pixels;
		Segment **			segments;
		int					segcount;
		uint32_t			generation;
	}	extent;

	Int_XY				buffer_size;

	struct Driver_Enables {
		Actor *				target;

		int					locater_up_enables;
		int					locater_down_enables;
		int					locater_still_up_enables;
		int					locater_still_down_enables;

		int					target_selection_enables;
		int					cursor_target_enables;

		int *				button_enables;

		int32_t				total_button_enables;

		int					string_enables;
		String_Cursor *		string_cursor;
		int					string_cursor_enables;
	}	pending, active;

	size_t				string_offset;
	KName				current_string;
	String_List *		string_list;
	String_List *		last_arrowed_string;
	String_List *		string_in_progress;
	int					string_list_length;

	bool				keyboard_has_been_enabled;

	Locater_Actions		locatable; // net of the above

	Locater_Actions		locater_has_been_enabled;

	struct {
		Locater_Actions		action;
		float				x;
		float				y;
		int					status;
	}					previous_location;

	Activity			double_swap_mask;

	Window				fake_window; // to hold redraw scope flags etc

	Karacter			partial_karacter;
	char				karacter_parts;
};

struct Show_Window_Info {

	unsigned int	max_texture_size;

	Int_XY			physical_pixels;
	Vector2D		physical_size;
	Int_XY			current_pixels;
	Vector2D		current_size;

	float			current_pixel_aspect;


};

#define UOF_Options			0x01
#define UOF_Unicode			0x02
#define UOF_Indices			0x04
#define UOF_Data			0x08
#define UOF_All				(UOF_Options|UOF_Unicode|UOF_Indices|UOF_Data)
#define UOF_Distill_Inplace	0x10

class User_Options : public Attribute {

public:
	Option_Value *			option_list;
	Option_Value *			koption_list;
	Destructible<By_Index_Hash>::unique_ptr			value_hash;
	Destructible<By_Index_Data_Hash>::unique_ptr	data_hash;

	User_Options_Vector_Pooled	user_options; // oldest first

	mutable int				_hash_key; // also used to sneak unset_flags

	User_Options() : Attribute(USER_OPTIONS),
		option_list(null), koption_list(null), value_hash(null), data_hash(null), user_options(DEFAULT_MEMORY_POOL()), _hash_key(0)
	{
		;
	}

	User_Options(User_Options const & that);

	~User_Options();

	bool operator == (User_Options const & that) const;

	Attribute * clone() const {
		Attribute * the_clone = NEW(User_Options)(*this);
		return the_clone;
	};

	bool equal(Attribute const * attr) const {
		if (attr->type != USER_OPTIONS)
			return false;
		return *this == *(User_Options*)attr;
	};

	Attribute * merge(Thread_Data * thread_data, Attribute const * attr, void * control = 0);

	Attribute * acquire(Thread_Data *	thread_data, Anything * item, Attribute const * old_attribute);

	int hash_key() const;

	int find_contents() const
	{
		int	contents = 0;

		if (!user_options.empty()) {
			for (auto it : user_options)
				contents |= it->find_contents();
		}

		if (option_list != null)
			contents |= UOF_Options;

		if (koption_list != null)
			contents |= UOF_Unicode;

		if (value_hash != null && !value_hash->empty())
			contents |= UOF_Indices;

		if (data_hash != null && !data_hash->empty())
			contents |= UOF_Data;

		return contents;
	}

	void merge_options(User_Options const * uo);
	void merge_unicode(User_Options const * uo);
	void merge_indices(User_Options const * uo);
	void merge_data(User_Options const * uo);


	static User_Options * Default(Thread_Data * thread_data);

private:
	User_Options const & operator= (User_Options const & that);
};



typedef std::vector<Line_Style, CMO_Allocator<Line_Style> > Line_Style_Vector;
typedef std::vector<Glyph, CMO_Allocator<Glyph> > Glyph_Vector;
typedef std::vector<Named_Style, CMO_Allocator<Named_Style> > Named_Style_Vector;

typedef int Glyph_Flags;
#define GF_NONE				(Glyph_Flags)0x00000000
#define GF_FILL				(Glyph_Flags)0x00000001
#define GF_CONTRAST			(Glyph_Flags)0x00000002
#define GF_RGB				(Glyph_Flags)0x00000004
#define GF_INDEX			(Glyph_Flags)0x00000008
#define GF_DOT				(Glyph_Flags)0x00000010
#define GF_ORDERED			(Glyph_Flags)0x00000020
#define	GF_INFINITE			(Glyph_Flags)0x00000040
#define	GF_BUILT_IN_DOT		(Glyph_Flags)0x00000080
#define	GF_BUILT_IN_CIRCLE	(Glyph_Flags)0x00000100
#define	GF_BUILT_IN_SQUARE	(Glyph_Flags)0x00000200
#define	GF_BUILT_IN_X		(Glyph_Flags)0x00000400
#define	GF_BUILT_IN_PLUS	(Glyph_Flags)0x00000800
#define	GF_BUILT_IN_SPHERE	(Glyph_Flags)0x00001000

#define GF_COMPLEX_ANYBITS (GF_CONTRAST|GF_RGB|GF_INDEX)

#define G_OP_DOT				 1
#define G_OP_TERMINATION		 0
#define G_OP_START_FILL			-1
#define G_OP_RESTART_FILL		-2
#define G_OP_END_FILL			-3
#define G_OP_DRAW_ELLIPSE		-4
#define G_OP_DRAW_ARC			-5
#define G_OP_DRAW_INF_LINE		-6
#define G_OP_DRAW_INF_RAY		-7
#define G_OP_START_CONTRAST		-8
#define G_OP_START_NORMAL		-9
#define G_OP_START_RGB			-10
#define G_OP_START_INDEX		-11
#define G_OP_ORDERED			-12
#define G_OP_NEW_FILL_LOOP		-13


#define GLYPH_RADIUS(glyph) (int)(glyph)->data[0]
#define GLYPH_HOTSPOT_X(glyph) (int)(glyph)->data[1]
#define GLYPH_HOTSPOT_Y(glyph) (int)(glyph)->data[2]
#define GLYPH_OPS(glyph) (&((glyph)->data[3]))

struct Glyph_Data : public Counted_Object_Interlocked {
	Name				name;
	Glyph_Flags			flags;
	char *				data;
	unsigned int		length;
	float				scale;

	Glyph_Data () : Counted_Object_Interlocked () {}
	~Glyph_Data () {
		if (length > 0)
			FREE_ARRAY (data, length, char);
	}

	INLINE bool operator == (Glyph_Data const & that) const {

		if (name != that.name)
			return false;
		if (flags != that.flags)
			return false;
		if (length != that.length)
			return false;
		if (scale != that.scale)
			return false;
		if (!EQUAL_MEMORY (data, length, that.data))
			return false;

		return true;
	}

	INLINE bool operator!= (Glyph_Data const & that) const	{ return  !(*this == that); }

private:
	Glyph_Data (Glyph_Data const &);
	Glyph_Data const & operator= (Glyph_Data const & that);
};
typedef Counted_Pointer<Glyph_Data>	Glyph;


class Glyph_Defs : public Attribute {
public:

	Glyph_Vector		glyphs;
	intptr_t_Vector		mapping;

	Glyph_Defs():Attribute(GLYPH_DEFS) {};

	~Glyph_Defs() {
		if (ANYBIT(dbflags, DB_CONTAINER)) {
			for (size_t i=0; i<glyphs.size(); ++i) {
				HI_Remove_Lightweight_Key (mapping[i], GLYPH_DEFS);
			}
		}
	};

	INLINE bool operator == (Glyph_Defs const & that) const {
	
		if (glyphs.size() != that.glyphs.size())
			return false;

		for (size_t i=0; i<glyphs.size(); ++i) {
			if (!glyphs[i].items_equal(that.glyphs[i]))
				return false;
		}

		return true;
	};

	Attribute * clone() const {
		Attribute * the_clone = NEW(Glyph_Defs)(*this);
		the_clone->dbflags &= ~DB_CONTAINER;
		return the_clone;
	};

	bool equal(Attribute const * attr) const {
		if (attr->type != GLYPH_DEFS)
			return false;
		return *this == *(Glyph_Defs*)attr;
	};

	Attribute * merge(Thread_Data * thread_data, Attribute const * attr, void * control=0);

	Attribute * acquire (Thread_Data *	thread_data, Anything * item, Attribute const * old_attribute);

	int hash_key() const {
		return static_cast<int>(glyphs.size());
	};

	bool undef_at(size_t i); // true if it was the last def


	static Glyph_Defs * Default(Thread_Data * thread_data);
};


struct Named_Style_Data : public Counted_Object_Interlocked {
	Name				name;
	Segment *			referee;

	Named_Style_Data () : Counted_Object_Interlocked () {}
	~Named_Style_Data () {}

	INLINE bool operator == (Named_Style_Data const & that) const {

		if (name != that.name)
			return false;
		if (referee != that.referee)
			return false;

		return true;
	}

	INLINE bool operator!= (Named_Style_Data const & that) const	{ return  !(*this == that); }

	void disentangle();

private:
	Named_Style_Data (Named_Style_Data const &);
	Named_Style_Data const & operator= (Named_Style_Data const & that);
};
typedef Counted_Pointer<Named_Style_Data>	Named_Style;


struct Geometry_Highlight_Info {
	Geometry_Details const *			details; // non-owning
	void *								data;
	Named_Style_Vector_Pooled const *	named_styles_vector;
};



class Named_Style_Defs : public Attribute {
public:

	Named_Style_Vector		named_styles;
	intptr_t_Vector			mapping;

	Named_Style_Defs():Attribute(NAMED_STYLE_DEFS) {};

	~Named_Style_Defs() {
		if (ANYBIT(dbflags, DB_CONTAINER)) {
			for (size_t i = 0; i < named_styles.size(); ++i) {
				HI_Remove_Lightweight_Key(mapping[i], NAMED_STYLE_DEFS);
				named_styles[i]->disentangle();
			}
		}
	};

	INLINE bool operator == (Named_Style_Defs const & that) const {

		if (named_styles.size() != that.named_styles.size())
			return false;

		for (size_t i=0; i<named_styles.size(); ++i) {
			if (!named_styles[i].items_equal(that.named_styles[i]))
				return false;
		}

		return true;
	};

	Attribute * clone() const {
		Attribute * the_clone = NEW(Named_Style_Defs)(*this);
		the_clone->dbflags &= ~DB_CONTAINER;
		return the_clone;
	};

	bool equal(Attribute const * attr) const {
		if (attr->type != NAMED_STYLE_DEFS)
			return false;
		return *this == *(Named_Style_Defs*)attr;
	};

	Attribute * merge(Thread_Data * thread_data, Attribute const * attr, void * control=0);

	Attribute * acquire (Thread_Data *	thread_data, Anything * item, Attribute const * old_attribute);

	int hash_key() const {
		return static_cast<int>(named_styles.size());
	};

	bool undef_at(size_t i); // true if it was the last def


	static Named_Style_Defs * Default(Thread_Data * thread_data);
};




enum Line_Style_Flags : uint32_t {
	LSF_NONE				= 0x00000000,
	LSF_SOLID				= 0x00000001,
	LSF_BITS16				= 0x00000002,
	LSF_BITS32				= 0x00000004,
	LSF_ABSOLUTE_LENGTH		= 0x00000008,
	LSF_ABSOLUTE_WEIGHT		= 0x00000010,
	LSF_PATTERN_ITEMS		= 0x00000020,
	LSF_LONG_LENGTHS		= 0x00000040,
	LSF_CONTRAST			= 0x00000080,
	LSF_GLYPHS				= 0x00000100,
	LSF_PARALLELS			= 0x00000200,
	LSF_OFFSET				= 0x00000400,
	LSF_MIDDLE_GLYPH		= 0x00000800,
	LSF_START_GLYPH			= 0x00001000,
	LSF_END_GLYPH			= 0x00002000,
	LSF_FIXED_OFFSET		= 0x00004000,
	LSF_WEIGHT				= 0x00008000,
	LSF_FIXED_WEIGHT		= 0x00010000,
	LSF_SEGMENTED			= 0x00020000,
	LSF_PERCENT				= 0x00040000,
	LSF_JOIN				= 0x00080000,
	LSF_CAP					= 0x00100000,
	LSF_FIT_CENTER			= 0x00200000,
	LSF_FIT_STRETCH			= 0x00400000,
	LSF_BEVELED_OFFSET		= 0x00800000,
	LSF_TRANSFORMED			= 0x01000000,
	LSF_INDEX_COLOR			= 0x02000000,
	LSF_RGB_COLOR			= 0x04000000,
	LSF_INVISIBLE			= 0x08000000,

	LSF_GLYPHD_ANYBITS = (LSF_START_GLYPH|LSF_END_GLYPH|LSF_MIDDLE_GLYPH),

	LSF_COMPLEX_ANYBITS = (LSF_ABSOLUTE_LENGTH|LSF_ABSOLUTE_WEIGHT|LSF_GLYPHD_ANYBITS|
							LSF_PATTERN_ITEMS | LSF_LONG_LENGTHS | LSF_CONTRAST | LSF_GLYPHS | LSF_WEIGHT |
							LSF_PARALLELS | LSF_OFFSET | LSF_SEGMENTED | LSF_FIT_CENTER | LSF_FIT_STRETCH |
							LSF_BEVELED_OFFSET | LSF_TRANSFORMED | LSF_INDEX_COLOR | LSF_RGB_COLOR | LSF_INVISIBLE),
};


template<> struct _bit_ops_type < Line_Style_Flags > {
	typedef Line_Style_Flags type;
};



#define LSIF_NONE				(Line_Style_Flags)0x00000000
#define LSIF_GLYPH_INSET		(Line_Style_Flags)0x00000001
#define LSIF_GLYPH_FIXED		(Line_Style_Flags)0x00000002
#define LSIF_GLYPH_INLINE		(Line_Style_Flags)0x00000004
#define LSIF_GLYPH_MIRROR		(Line_Style_Flags)0x00000010
#define LSIF_GLYPH_ROTATION		(Line_Style_Flags)0x00000020
#define LSIF_GLYPH_WEIGHTED		(Line_Style_Flags)0x00000040
#define LSIF_STRETCH_FIXUPS		(Line_Style_Flags)0x00000100
#define LSIF_PERCENT_FIXUPS		(Line_Style_Flags)0x00000200


struct Line_Style_Item {
	Name				name;
	Generic_Size		size;
	Line_Style_Flags	flags;
	float				rotation;
	Generic_Size		weight;
	RGB					rgb;
	int					index;

	INLINE bool operator == (Line_Style_Item const & that) const {

		if (name != that.name)
			return false;
		if (size != that.size)
			return false;
		if (flags != that.flags)
			return false;
		if (rotation != that.rotation)
			return false;
		if (weight != that.weight)
			return false;
		if (rgb != that.rgb)
			return false;
		if (index != that.index)
			return false;

		return true;
	}

	INLINE bool operator!= (Line_Style_Item const & that) const	{ return  !(*this == that); }
};


struct Line_Style_Parallel {
	Line_Style_Parallel *		next;

	Line_Style_Flags			flags;

	Generic_Size				offset;
	Generic_Size				weight;

	Line_Style_Item *			start_glyph;
	Line_Style_Item *			middle_glyph;
	Line_Style_Item *			end_glyph;

	Line_Style_Item *			pattern_items;
	int *						pattern;

	unsigned int				pattern_items_n;
	unsigned int				pattern_n;


	INLINE bool operator == (Line_Style_Parallel const & that) const {

		if (flags != that.flags)
			return false;
		if (offset != that.offset)
			return false;
		if (weight != that.weight)
			return false;
		if (pattern_items_n != that.pattern_items_n)
			return false;
		if (pattern_n != that.pattern_n)
			return false;

		if (start_glyph == that.start_glyph)
;
		else if (start_glyph == null || that.start_glyph == null || *start_glyph != *that.start_glyph)
			return false;
		if (middle_glyph == that.middle_glyph)
;
		else if (middle_glyph == null || that.middle_glyph == null || *middle_glyph != *that.middle_glyph)
			return false;
		if (end_glyph == that.end_glyph)
;
		else if (end_glyph == null || that.end_glyph == null || *end_glyph != *that.end_glyph)
			return false;

		for (unsigned int i = 0; i<pattern_n; i++) {
			if (pattern[i] != that.pattern[i])
				return false;
		}
		for (unsigned int i = 0; i<pattern_items_n; i++) {
			if (pattern_items[i] != that.pattern_items[i])
				return false;
		}

		return true;
	}

	INLINE bool operator!= (Line_Style_Parallel const & that) const	{ return  !(*this == that); }
};

#define LS_OP_DASH		(int)-1
#define LS_OP_BLANK		(int)-2
#define LS_OP_CONTRAST	(int)-3
#define LS_OP_GLYPH		(int)-4

struct Line_Style_Data : public Counted_Object_Interlocked {
	Name						name;
	int32_t						id;
	Line_Style_Flags			flags;
	Name						definition;

	Line_Style_Parallel *		parallels;
	unsigned int				parallels_n;

	int							scale32;
	int							scale16;
	uint32_t					bits32;
	unsigned short				bits16;

	Line_Style_Data () : Counted_Object_Interlocked () {}
	~Line_Style_Data () {
		Line_Style_Parallel *	p;

		while ((p = parallels) != null) {
			parallels = p->next;

			if (p->start_glyph) {
				p->start_glyph->name = null;
				FREE(p->start_glyph, Line_Style_Item);
			}
			if (p->middle_glyph) {
				p->middle_glyph->name = null;
				FREE(p->middle_glyph, Line_Style_Item);
			}
			if (p->end_glyph) {
				p->end_glyph->name = null;
				FREE(p->end_glyph, Line_Style_Item);
			}
			if (p->pattern_items) {
				for (unsigned int i=0; i<p->pattern_items_n; ++i)
					p->pattern_items[i].name = null;
				FREE_ARRAY(p->pattern_items, p->pattern_items_n, Line_Style_Item);
			}
			if (p->pattern)
				FREE_ARRAY(p->pattern, p->pattern_n, int);

			FREE(p, Line_Style_Parallel);
		}
	};

	INLINE bool operator == (Line_Style_Data const & that) const {

		if (name != that.name)
			return false;
		if (id != that.id)
			return false;
		if (flags != that.flags)
			return false;
		if (definition != that.definition)
			return false;
		if (scale32 != that.scale32)
			return false;
		if (scale16 != that.scale16)
			return false;
		if (bits32 != that.bits32)
			return false;
		if (bits16 != that.bits16)
			return false;

		Line_Style_Parallel * this_parallels = parallels;
		Line_Style_Parallel * that_parallels = that.parallels;

		while (this_parallels != null) {
			if (*this_parallels != *that_parallels)
				return false;

			this_parallels = this_parallels->next;
			that_parallels = that_parallels->next;
		}

		return true;
	}

	INLINE bool operator!= (Line_Style_Data const & that) const	{ return  !(*this == that); }

private:
	Line_Style_Data (Line_Style_Data const &);
	Line_Style_Data const & operator= (Line_Style_Data const & that);
};
typedef Counted_Pointer<Line_Style_Data>	Line_Style;

INLINE void Canonize_Line_Pattern(char const *	in_string, Name * name, int32_t * in_extra)
{
#ifdef HPS_CORE_BUILD
	UNREFERENCED(in_extra);
	*name = in_string;
#else
	HI_Really_Canonize_Line_Pattern(in_string, name, in_extra);
#endif
}

class Line_Style_Defs : public Attribute {
public:

	Line_Style_Vector	line_styles;
	intptr_t_Vector		mapping;

	Line_Style_Defs():Attribute(LINE_STYLE_DEFS) {};

	~Line_Style_Defs() {
		if (ANYBIT(dbflags, DB_CONTAINER)) {
			for (size_t i=0; i<line_styles.size(); ++i) {
				HI_Remove_Lightweight_Key (mapping[i], LINE_STYLE_DEFS);
			}
		}
	};

	INLINE bool operator == (Line_Style_Defs const & that) const {

		if (line_styles.size() != that.line_styles.size())
			return false;

		for (size_t i=0; i<line_styles.size(); ++i) {
			if (!line_styles[i].items_equal(that.line_styles[i]))
				return false;
		}

		return true;
	};

	Attribute * clone() const {
		Attribute * the_clone = NEW(Line_Style_Defs)(*this);
		the_clone->dbflags &= ~DB_CONTAINER;
		return the_clone;
	};

	bool equal(Attribute const * attr) const {
		if (attr->type != LINE_STYLE_DEFS)
			return false;
		return *this == *(Line_Style_Defs*)attr;
	};

	Attribute * merge(Thread_Data * thread_data, Attribute const * attr, void * control=0);

	Attribute * acquire (Thread_Data *	thread_data, Anything * item, Attribute const * old_attribute);

	int hash_key() const {
		return static_cast<int>(line_styles.size());
	};

	bool undef_at(size_t i); // true if it was the last def


	static Line_Style_Defs * Default(Thread_Data * thread_data);
};



class Line_Weight : public Attribute {
public:
	Generic_Size	data;

	Line_Weight():Attribute(LINE_WEIGHT) {};

	INLINE bool operator == (Line_Weight const & that) const {
		return (data == that.data);
	};

	Attribute * clone() const {return NEW(Line_Weight)(*this);};

	bool equal(Attribute const * attr) const {
		if (attr->type != LINE_WEIGHT)
			return false;
		return *this == *(Line_Weight*)attr;
	}

	Attribute * merge(Thread_Data * thread_data, Attribute const * attr, void * control=0);

	Attribute * acquire (Thread_Data *	thread_data, Anything * item, Attribute const * old_attribute);

	int hash_key() const {return Float::extract_uint32_t(data.value) + data.units;};

	void set(float in_weight, HPS::Line::SizeUnits in_units)
	{
		switch (in_units) {
		case HPS::Line::SizeUnits::ScaleFactor:
			data.units = GSU_UNQUALIFIED;
			break;
		case HPS::Line::SizeUnits::ObjectSpace:
			data.units = GSU_ORU;
			break;
		case HPS::Line::SizeUnits::SubscreenRelative:
			data.units = GSU_SRU;
			break;
		case HPS::Line::SizeUnits::WindowRelative:
			data.units = GSU_WRU;
			break;
		case HPS::Line::SizeUnits::WorldSpace:
			data.units = GSU_WORLD;
			break;
		case HPS::Line::SizeUnits::Points:
			data.units = GSU_POINTS;
			break;
		case HPS::Line::SizeUnits::Pixels:
			data.units = GSU_PIXELS;
			break;
		default: ASSERT(0); break;
		}

		data.value = in_weight;
	}

	bool show(float & out_weight, HPS::Line::SizeUnits & out_units) const
	{
		bool success = true;

		out_weight = data.value;

		switch (data.units) {
		case GSU_UNQUALIFIED:
			out_units = HPS::Line::SizeUnits::ScaleFactor;
			break;
		case GSU_ORU:
			out_units = HPS::Line::SizeUnits::ObjectSpace;
			break;
		case GSU_SRU:
			out_units = HPS::Line::SizeUnits::SubscreenRelative;
			break;
		case GSU_WRU:
			out_units = HPS::Line::SizeUnits::WindowRelative;
			break;
		case GSU_WORLD:
			out_units = HPS::Line::SizeUnits::WorldSpace;
			break;
		case GSU_POINTS:
			out_units = HPS::Line::SizeUnits::Points;
			break;
		case GSU_PIXELS:
			out_units = HPS::Line::SizeUnits::Pixels;
			break;
		default: success=false; ASSERT(0); break;
		}

		return success;
	}



	static Line_Weight * Default(Thread_Data * thread_data);
};

class Edge_Weight : public Attribute {
public:
	Generic_Size	data;

	Edge_Weight():Attribute(EDGE_WEIGHT) {};

	INLINE bool operator == (Edge_Weight const & that) const {
		return (data == that.data);
	};

	Attribute * clone() const {return NEW(Edge_Weight)(*this);};

	bool equal(Attribute const * attr) const {
		if (attr->type != EDGE_WEIGHT)
			return false;
		return *this == *(Edge_Weight*)attr;
	}

	Attribute * merge(Thread_Data * thread_data, Attribute const * attr, void * control=0);

	Attribute * acquire (Thread_Data *	thread_data, Anything * item, Attribute const * old_attribute);

	int hash_key() const {return Float::extract_uint32_t(data.value) + data.units;};

	void set(float in_weight, HPS::Edge::SizeUnits in_units)
	{
		switch (in_units) {
		case HPS::Edge::SizeUnits::ScaleFactor:
			data.units = GSU_UNQUALIFIED;
			break;
		case HPS::Edge::SizeUnits::ObjectSpace:
			data.units = GSU_ORU;
			break;
		case HPS::Edge::SizeUnits::SubscreenRelative:
			data.units = GSU_SRU;
			break;
		case HPS::Edge::SizeUnits::WindowRelative:
			data.units = GSU_WRU;
			break;
		case HPS::Edge::SizeUnits::WorldSpace:
			data.units = GSU_WORLD;
			break;
		case HPS::Edge::SizeUnits::Points:
			data.units = GSU_POINTS;
			break;
		case HPS::Edge::SizeUnits::Pixels:
			data.units = GSU_PIXELS;
			break;
		default: ASSERT(0); break;
		}

		data.value = in_weight;
	}

	bool show(float & out_weight, HPS::Edge::SizeUnits & out_units) const
	{
		bool success = true;

		out_weight = data.value;

		switch (data.units) {
			case GSU_UNQUALIFIED:
				out_units = HPS::Edge::SizeUnits::ScaleFactor;
				break;
			case GSU_ORU:
				out_units = HPS::Edge::SizeUnits::ObjectSpace;
				break;
			case GSU_SRU:
				out_units = HPS::Edge::SizeUnits::SubscreenRelative;
				break;
			case GSU_WRU:
				out_units = HPS::Edge::SizeUnits::WindowRelative;
				break;
			case GSU_WORLD:
				out_units = HPS::Edge::SizeUnits::WorldSpace;
				break;
			case GSU_POINTS:
				out_units = HPS::Edge::SizeUnits::Points;
				break;
			case GSU_PIXELS:
				out_units = HPS::Edge::SizeUnits::Pixels;
				break;
			default: success=false; ASSERT(0); break;
		}

		return success;
	}




	static Edge_Weight * Default(Thread_Data * thread_data);

};

class Marker_Size : public Attribute {
public:
	Generic_Size	data;

	Marker_Size():Attribute(MARKER_SIZE) {};

	INLINE bool operator == (Marker_Size const & that) const {
		return (data == that.data);
	};

	Attribute * clone() const {return NEW(Marker_Size)(*this);};

	bool equal(Attribute const * attr) const {
		if (attr->type != MARKER_SIZE)
			return false;
		return *this == *(Marker_Size*)attr;
	}

	Attribute * merge(Thread_Data * thread_data, Attribute const * attr, void * control=0);

	Attribute * acquire (Thread_Data *	thread_data, Anything * item, Attribute const * old_attribute);

	int hash_key() const {return Float::extract_uint32_t(data.value) + data.units;};

	void set(float in_size, HPS::Marker::SizeUnits in_units)
	{
		switch (in_units) {
			case HPS::Marker::SizeUnits::ScaleFactor:
				data.units = GSU_UNQUALIFIED;
			break;
			case HPS::Marker::SizeUnits::ObjectSpace:
				data.units = GSU_ORU;
			break;
			case HPS::Marker::SizeUnits::SubscreenRelative:
				data.units = GSU_SRU;
			break;
			case HPS::Marker::SizeUnits::WindowRelative:
				data.units = GSU_WRU;
			break;
			case HPS::Marker::SizeUnits::WorldSpace:
				data.units = GSU_WORLD;
			break;
			case HPS::Marker::SizeUnits::Points:
				data.units = GSU_POINTS;
			break;
			case HPS::Marker::SizeUnits::Pixels:
				data.units = GSU_PIXELS;
			break;
			default: ASSERT(0); break;
		}

		data.value = in_size;
	}

	bool show(float & out_size, HPS::Marker::SizeUnits & out_units) const
	{
		bool success = true;

		out_size = data.value;

		switch (data.units) {
			case GSU_UNQUALIFIED:
				out_units = HPS::Marker::SizeUnits::ScaleFactor;
				break;
			case GSU_ORU:
				out_units = HPS::Marker::SizeUnits::ObjectSpace;
				break;
			case GSU_SRU:
				out_units = HPS::Marker::SizeUnits::SubscreenRelative;
				break;
			case GSU_WRU:
				out_units = HPS::Marker::SizeUnits::WindowRelative;
				break;
			case GSU_WORLD:
				out_units = HPS::Marker::SizeUnits::WorldSpace;
				break;
			case GSU_POINTS:
				out_units = HPS::Marker::SizeUnits::Points;
				break;
			case GSU_PIXELS:
				out_units = HPS::Marker::SizeUnits::Pixels;
				break;
			default: success=false; ASSERT(0); break;
		}

		return success;
	}

	static Marker_Size * Default(Thread_Data * thread_data);
};


class Text_Spacing : public Attribute {
public:
	Generic_Size	data;

	Text_Spacing():Attribute(TEXT_SPACING) {};

	INLINE bool operator == (Text_Spacing const & that) const {
		return (data == that.data);
	};

	Attribute * clone() const {return NEW(Text_Spacing)(*this);};

	bool equal(Attribute const * attr) const {
		if (attr->type != TEXT_SPACING)
			return false;
		return *this == *(Text_Spacing*)attr;
	}

	Attribute * merge(Thread_Data * thread_data, Attribute const * attr, void * control=0);

	Attribute * acquire (Thread_Data *	thread_data, Anything * item, Attribute const * old_attribute);

	int hash_key() const {return Float::extract_uint32_t(data.value) + data.units;};

	void set(float in_multiplier)
	{
		data.value = in_multiplier;
		data.units = GSU_UNQUALIFIED;
	}

	bool show(float & out_multiplier) const
	{
		out_multiplier = data.value;
		return true;
	}

	static Text_Spacing * Default(Thread_Data * thread_data);
};


class Face_Pattern : public Attribute {
public:
	unsigned char	value;
	unsigned char	extra;

	Face_Pattern():Attribute(FACE_PATTERN),
		value(0),
		extra(0) {};

	INLINE bool operator == (Face_Pattern const & that) const {
		return (value == that.value && extra == that.extra);
	};

	Attribute * clone() const {return NEW(Face_Pattern)(*this);};

	bool equal(Attribute const * attr) const {
		if (attr->type != FACE_PATTERN)
			return false;
		return *this == *(Face_Pattern*)attr;
	}

	Attribute * merge(Thread_Data * thread_data, Attribute const * attr, void * control=0);

	Attribute * acquire (Thread_Data *	thread_data, Anything * item, Attribute const * old_attribute);

	int hash_key() const {return value + extra;};

	static Face_Pattern * Default(Thread_Data * thread_data);
};

typedef int32_t Window_Pattern_Flags;

#define WPF_BACKGROUND_TYPE		(Window_Pattern_Flags)0x00000001
#define WPF_MATERIAL_TYPE		(Window_Pattern_Flags)0x00000002
#define WPF_RGB					(Window_Pattern_Flags)0x00000004
#define WPF_FINDEX				(Window_Pattern_Flags)0x00000008
#define WPF_CONTRAST_TYPE		(Window_Pattern_Flags)0x00000010
#define WPF_CONTRAST_RGB		(Window_Pattern_Flags)0x00000020
#define WPF_CONTRAST_FINDEX		(Window_Pattern_Flags)0x00000040

#define Window_Pattern_TRANSPARENT	DB_TYPE_SPECIFIC(1)

class Window_Pattern : public Attribute {
public:
	unsigned char			value;
	unsigned char			extra;
	Window_Pattern_Flags	flags_mask;

#ifdef HPS_CORE_BUILD
	// informational
	HPS::Subwindow::Background	bg_type;
	HPS::Material::Type			material_type;
	RGB							rgb;
	Color_Float_Index			findex;
	HPS::Material::Type			contrast_material_type;
	RGB							contrast_rgb;
	Color_Float_Index			contrast_findex;
#endif

	Window_Pattern():Attribute(WINDOW_PATTERN),
		value(0),
		extra(0)
	{
#ifdef HPS_CORE_BUILD
		bg_type = HPS::Subwindow::Background::SolidColor;
		material_type = HPS::Material::Type::Nothing;
		contrast_material_type = HPS::Material::Type::Nothing;
#endif
	};

#ifdef HPS_CORE_BUILD
	INLINE bool operator == (Window_Pattern const & that) const {
		return (value == that.value && 
				extra == that.extra &&
				bg_type == that.bg_type &&
				material_type == that.material_type &&
				rgb == that.rgb &&
				findex == that.findex &&
				contrast_material_type == that.contrast_material_type &&
				contrast_rgb == that.contrast_rgb &&
				contrast_findex == that.contrast_findex);
	};
#else
	INLINE bool operator == (Window_Pattern const & that) const {
		return (value == that.value && extra == that.extra);
	};
#endif

	Attribute * clone() const {return NEW(Window_Pattern)(*this);};

	bool equal(Attribute const * attr) const {
		if (attr->type != WINDOW_PATTERN)
			return false;
		return *this == *(Window_Pattern*)attr;
	}

	Attribute * merge(Thread_Data * thread_data, Attribute const * attr, void * control=0);

	Attribute * acquire (Thread_Data *	thread_data, Anything * item, Attribute const * old_attribute);

	int hash_key() const {return value + extra;};


	static Window_Pattern * Default(Thread_Data * thread_data);
};


class Text_Alignment : public Attribute {
public:
	unsigned char	value;
	unsigned char	extra;

	Text_Alignment():Attribute(TEXT_ALIGNMENT),
		value(0),
		extra(0) {};

	INLINE bool operator == (Text_Alignment const & that) const {
		return (value == that.value && extra == that.extra);
	};

	Attribute * clone() const {return NEW(Text_Alignment)(*this);};

	bool equal(Attribute const * attr) const {
		if (attr->type != TEXT_ALIGNMENT)
			return false;
		return *this == *(Text_Alignment*)attr;
	}


	Attribute * merge (Thread_Data * thread_data, Attribute const * attr, void * control=0);

	Attribute * acquire (Thread_Data *	thread_data, Anything * item, Attribute const * old_attribute);

	int hash_key() const {return value + extra;};

	void set(HPS::Text::Alignment in_align, HPS::Text::ReferenceFrame in_ref, HPS::Text::Justification in_justify)
	{
		switch (in_align) {
		case HPS::Text::Alignment::TopLeft:
			value = TALIGN_UPPER | TALIGN_LEFT;
			break;
		case HPS::Text::Alignment::CenterLeft:
			value = TALIGN_CENTER | TALIGN_LEFT;
			break;
		case HPS::Text::Alignment::BottomLeft:
			value = TALIGN_LOWER | TALIGN_LEFT;
			break;
		case HPS::Text::Alignment::TopCenter:
			value = TALIGN_UPPER | TALIGN_CENTER;
			break;
		case HPS::Text::Alignment::Center:
			value = TALIGN_CENTER | TALIGN_CENTER;
			break;
		case HPS::Text::Alignment::BottomCenter:
			value = TALIGN_LOWER | TALIGN_CENTER;
			break;
		case HPS::Text::Alignment::TopRight:
			value = TALIGN_UPPER | TALIGN_RIGHT;
			break;
		case HPS::Text::Alignment::CenterRight:
			value = TALIGN_CENTER | TALIGN_RIGHT;
			break;
		case HPS::Text::Alignment::BottomRight:
			value = TALIGN_LOWER | TALIGN_RIGHT;
			break;
		}

		if (in_ref == HPS::Text::ReferenceFrame::PathAligned)
			value |= TALIGN_INSERTION_POINT;

		switch (in_justify) {
		case HPS::Text::Justification::LeftJustified:
			extra = TJUST_LEFT;
			break;
		case HPS::Text::Justification::RightJustified:
			extra = TJUST_RIGHT;
			break;
		case HPS::Text::Justification::CenterJustified:
			extra = TJUST_CENTER;
			break;
		}
	}

	bool show(HPS::Text::Alignment & out_align, HPS::Text::ReferenceFrame & out_ref, HPS::Text::Justification & out_justify) const
	{

		out_ref = (ANYBIT(value, TALIGN_INSERTION_POINT) ? HPS::Text::ReferenceFrame::PathAligned : HPS::Text::ReferenceFrame::WorldAligned);

		if (extra == TJUST_LEFT)
			out_justify = HPS::Text::Justification::LeftJustified;
		else if (extra == TJUST_RIGHT)
			out_justify = HPS::Text::Justification::RightJustified;
		else
			out_justify = HPS::Text::Justification::CenterJustified;

		if (ANYBIT(value, TALIGN_LEFT)) {
			switch (value & TALIGN_VERTICAL_MASK) {
			case TALIGN_LOWER:		out_align = HPS::Text::Alignment::BottomLeft; break;
			case TALIGN_CENTER:		out_align = HPS::Text::Alignment::CenterLeft; break;
			case TALIGN_UPPER:		out_align = HPS::Text::Alignment::TopLeft; break;
			}
		}
		else if (ANYBIT(value, TALIGN_RIGHT)) {
			switch (value & TALIGN_VERTICAL_MASK) {
			case TALIGN_LOWER:		out_align = HPS::Text::Alignment::BottomRight; break;
			case TALIGN_CENTER:		out_align = HPS::Text::Alignment::CenterRight; break;
			case TALIGN_UPPER:		out_align = HPS::Text::Alignment::TopRight; break;
			}
		}
		else {
			switch (value & TALIGN_VERTICAL_MASK) {
			case TALIGN_LOWER:		out_align = HPS::Text::Alignment::BottomCenter; break;
			case TALIGN_CENTER:		out_align = HPS::Text::Alignment::Center; break;
			case TALIGN_UPPER:		out_align = HPS::Text::Alignment::TopCenter; break;
			}
		}
		return true;
	}


	static Text_Alignment * Default(Thread_Data * thread_data);
};

class Marker_Symbol : public Attribute {
public:
	Name	name;

	Marker_Symbol():Attribute(MARKER_SYMBOL) {};

	INLINE bool operator == (Marker_Symbol const & that) const {
		return (/*extra == that.extra &&*/ name == that.name);
	};

	int hash_key() const {
		return name.hash_key();
	}

	Attribute * clone() const {return NEW(Marker_Symbol)(*this);};
	
	bool equal(Attribute const * attr) const {
		if (attr->type != MARKER_SYMBOL)
			return false;
		return *this == *(Marker_Symbol*)attr;
	}

	Attribute * merge(Thread_Data * thread_data, Attribute const * attr, void * control=0);

	Attribute * acquire (Thread_Data *	thread_data, Anything * item, Attribute const * old_attribute);


	static Marker_Symbol * Default(Thread_Data * thread_data);
};



#define LP_Explicit_NONE					0x00000000
#define LP_Explicit_LINE_STYLE				0x00000001
#define LP_Explicit_CAP						0x00000002
#define LP_Explicit_END_CAP					0x00000004
#define LP_Explicit_INNER_CAP				0x00000008
#define LP_Explicit_JOIN					0x00000010

#define LP_Explicit_START_GLYPH				0x00000020
#define LP_Explicit_END_GLYPH				0x00000040
#define LP_Explicit_MIDDLE_GLYPH			0x00000080
#define LP_Explicit_GLYPHS					(LP_Explicit_START_GLYPH|LP_Explicit_MIDDLE_GLYPH|LP_Explicit_END_GLYPH)
#define LP_Explicit_END_GLYPH_ARROW			0x00000100
#define LP_Explicit_ARROWS					(LP_Explicit_START_GLYPH|LP_Explicit_END_GLYPH_ARROW)
#define LP_Explicit_UNSET_GLYPHS			0x00000200
#define LP_Explicit_NEGATED_LINE_STYLE		0x00000400

class Line_Pattern : public Attribute {
public:

	Name		line_style;
	Name		start_glyph;
	Name		middle_glyph;
	Name		end_glyph;
	int32_t	mask;
	int32_t	value;

	Line_Pattern():Attribute(LINE_PATTERN),
		mask(0), value(0)
	{};

	INLINE bool operator == (Line_Pattern const & that) const {
		return (value == that.value &&
			mask == that.mask &&
			line_style == that.line_style &&
			start_glyph == that.start_glyph &&
			middle_glyph == that.middle_glyph &&
			end_glyph == that.end_glyph);
	};

	Attribute * clone() const {return NEW(Line_Pattern)(*this);};

	bool equal(Attribute const * attr) const {
		if (attr->type != LINE_PATTERN)
			return false;
		return *this == *(Line_Pattern*)attr;
	}


	Attribute * merge(Thread_Data * thread_data, Attribute const * attr, void * control=0);

	Attribute * acquire (Thread_Data *	thread_data, Anything * item, Attribute const * old_attribute);

	int hash_key() const {
		return value + mask + end_glyph.hash_key() + middle_glyph.hash_key() + start_glyph.hash_key() + line_style.hash_key();
	}


	static Line_Pattern * Default(Thread_Data * thread_data);
};

class Edge_Pattern : public Attribute {
public:

	Name		line_style;
	int32_t	mask;
	int32_t	value;

	Edge_Pattern():Attribute(EDGE_PATTERN),
		mask(0), value(0) {};

	INLINE bool operator == (Edge_Pattern const & that) const {
		return (value == that.value &&
			mask == that.mask &&
			line_style == that.line_style);
	};

	Attribute * clone() const {return NEW(Edge_Pattern)(*this);};

	bool equal(Attribute const * attr) const {
		if (attr->type != EDGE_PATTERN)
			return false;
		return *this == *(Edge_Pattern*)attr;
	}

	Attribute * merge(Thread_Data * thread_data, Attribute const * attr, void * control=0);

	Attribute * acquire (Thread_Data *	thread_data, Anything * item, Attribute const * old_attribute);

	int hash_key() const {return value + mask + line_style.hash_key();};

	static Edge_Pattern * Default(Thread_Data * thread_data);
};


// convert keys options
#define CK_KEEP_INCLUDES			0x01
#define CK_ALLOW_WILDCARDS			0x02
#define CK_DISCARD_EXTRA_SEGMENTS	0x04

#define FAF_NONE					0x00
#define FAF_MASK_TRANSFORM			0x01
#define FAF_PARTIAL_ACCUMULATION	0x02
#define	FAF_TRACK_COLOR_MAP			0x04
#define	FAF_NOT_REQUIRED			0x08
#define	FAF_NET						0x10
#define	FAF_TRACK_MODELLING_MATRIX	0x20
#define FAF_PRESERVE_CLIP_REGION	0x40

struct Find_Attribute_Control {
	Camera const *			camera;
	Color_Map const *		color_map;
	Matrix					matrix;
	Attribute_Lock *		locks;
	Attribute_Lock *		forced;
	Condition_Bits *		condition_bits;
	float					window_aspect;
};


struct Color_Name {
	Color_Name *			next;
	Color_Def *				def;
	Color_Name *			hash_next;
	Color_Name **			hash_backlink;
	Name					name;
	int						degree;
};

struct Color_Def : public Minimum_Header<Color_Def, World> {
	Color_Name *	cnames;
	Named_Material	material;
	Name			definition;
};

struct Alias : public Minimum_Header<Alias, World> {
	Path_Token *	def;
	Name			name;
};
#define Alias_PARENS_ADDED		DB_TYPE_SPECIFIC(1)

/* the "units" below are derived from the "suboption" table for the option */

struct Float_With_Units {
	float		value;
	long		units;
};

struct Option_Value {
	Option_Value *	next;
	Option_Type *	type;
	union {
		bool *	_boolean;
		float *	_float;
		int *	_int;
		Name *	_name;
		KName *	_kname;
		long *	_long;
		void *	_void;
		intptr_t *	_ptr;
		Option_Value *	option_list; /* for recursive parsing */
		Float_With_Units *	_float_with_units;
	}	value;
	bool			negated;
	int				actual_value_count;
	int				actual_type;
};

/* Parse_Options types */
#define OValue_NONE						 0
#define OValue_BOOLEAN					 1
#define OValue_FLOAT					 2
#define OValue_INT						 3
#define OValue_NAME						 4
#define OValue_STRING					 5	/* same as Name, except not canonical */
#define OValue_LONG						 6
#define OValue_USER_OPTION				 7
#define OValue_FLOAT_WITH_UNITS 		 8
#define OValue_PTR						 9
#define OValue_USER_KOPTION				10
#define OValue_KNAME					11
#define OValue_KSTRING					12
#define OValue_CONDITION				13
// only "suboption" types from her on
#define OValue_SUBOPTIONS				14	/* recursive parse */
#define OValue_INT_OR_SUBOPTIONS		15	/* single int or recursive suboptions */
#define OValue_PTR_OR_SUBOPTIONS		16	/* single pointer or recursive suboptions */
#define OValue_FLOAT_OR_SUBOPTIONS		17	/* single float or recursive suboptions */
#define OValue_INT_AS_OPTION			18	/* single int or maybe not */


#define OT_DONT_FREE_SUBOPTIONS	DB_TYPE_SPECIFIC(1)

struct Option_Type : public Minimum_Header<Option_Type, Anything> {
	Option_Hash_Table *			suboptions;
	Name_Const					name;
	int32_t						id;
	int32_t						value_type;
	int32_t						value_count;	// -1 if variable
	bool						negatable;
	Name						name_data;		// If this->name needs a persistent back buffer, point to this->name_data
};

struct Constant_Option_Type {

	template<typename T>
	Constant_Option_Type(
		T									id,
		Name_Const							name,
		int32_t								value_type = OValue_NONE,
		int32_t								value_count = 0,
		bool								negatable = false,
		Constant_Option_Table_Info const *	suboption_table = 0);

	Constant_Option_Table_Info const *	suboption_table;
	Name_Const							name;
	int32_t								id;
	int32_t								value_type;
	int32_t								value_count;	/* -1 if variable */
	bool								negatable;	
};

template<typename T>
Constant_Option_Type::Constant_Option_Type(
	T									id,
	Name_Const							name,
	int32_t								value_type,
	int32_t								value_count,
	bool								negatable,
	Constant_Option_Table_Info const *	suboption_table) 
	: suboption_table(suboption_table), name(name), id(id), value_type(value_type), value_count(value_count), negatable(negatable) {}

typedef Constant_Option_Type COT;



/* (a c_o_t_i exists just to save having to put 'count' into the above) */
struct Constant_Option_Table_Info {
	int								count;
	Constant_Option_Type const *	options;
};


struct KOption_Type : public Minimum_Header<KOption_Type, Anything> {
	Option_Hash_Table *			suboptions;
	KName						kname;
	int32_t						id;
	int32_t						value_type;
	int32_t						value_count;	// -1 if variable
	bool						negatable;
	Name						placeholder;	// Just to keep the same shape as Option_Type
};


#define	INITIALIZE_OPTION_TABLE(_index_, _choices_)	\
	do {											\
		Mutexer mutexer(HOOPS::WORLD->hash_mutex);	\
		if (OPTIONS_HASH (_index_)->table == null)	\
			HI_Define_Constant_Options (_choices_, countof (_choices_),	OPTIONS_HASH (_index_));	\
	} while (0)

#define	Option_Parse_NORMAL			0x00
#define	Option_Parse_OPTIONS_ONLY	0x01
#define	Option_Parse_SINGLE			0x02
#define	Option_Parse_RECURSING		0x04
#define	Option_Parse_SUPPRESS_ERROR	0x08

#ifndef HPS_CORE_BUILD
struct Exit_Handler : public Minimum_Header<Exit_Handler, World> {
	Exit_Handler_Routine		routine;
};

struct Error_Handler : public Minimum_Header<Error_Handler, World> {
	Error_Handler_Routine		routine;
};
#endif

struct Wakeup : public Minimum_Header<Wakeup, World> {
	Time						ticks;
	Time						next_expiration;
	int							enables;
	float						interval;
	int							events_pending;
};

#define EF_ALREADY_PAUSED		DB_TYPE_SPECIFIC(1)


struct Wakeup_Event : public Event {
	Wakeup *				wakeup;
};

struct Button_Event : public Event {
	Actor *					source;
	Button					button;
	int32_t					status;
		/* in case a string event generated by the same button press */
	String_Event *			string_event;
};

struct String_Event : public Event {
	Actor *					source;
	KName					string;
	Text_Encoding			encoding;
};

struct Special_Event : public Event {
	Name					name;
	Name					value;
};

struct Location_Event : public Event {
	Actor *					source;
	float					x, y;
	int32_t					status;
	Locater_Actions			action;
		/* and if a selection is generated by the same mouse action */
	Selection_Event	*		selection_event;
};


struct Include_Chain {
	Include_Chain *			prev;
	Include const *			include;
};

struct Selection_Extras {
	Selection_Extras *		next;
	int						offsets[3];
	float					prox;
	float					tiebreaker;
	float					t;
};

struct Selection_Item {
	Selection_Item *		next;
	Anything const *		item;
	Key						item_key;
	Key						original_key;
	Include_Chain *			include_chain;
	Locater_Actions			item_action;
	int						offset1, offset2, offset3;

	Point					window;
	Point					camera;
	DPoint					dcamera;
	int						precision;

	Selection_Extras *		extras;

	/* these are for driver convenience */
	float					screen_z;
	float					sorting_z;	// includes any appropriate displacements
	float					proximity;
	Net_Rendition			net_rendition;
};


typedef int SIH_Flags;
#define SIH_SORT_DEFERRED (SIH_Flags)0x0001

struct Selection_Item_Heap {

	Selection_Item **		si_array;
	Selection_Item *		zsort_list;
	bool					(*compare)(Selection_Item *, Selection_Item*);
	SIH_Flags				flags;

	int 					allocated;
	int 					used;

	int						zsort_list_length;
};


struct Selection_Path {
	Include_Path		path;
	Segment const *		segment;
};



struct Selection_ID {
	uint32_t			path;

	// meed the "low value" deciphered into this:
	Geometry_Reference const *	reference;	// may be null
	Geometry const *			geometry;
	uint32_t					subentity;
	Type_Flags					type;		// T_FACES, T_EDGES, T_VERTICES, T_ISOLINES
};


struct Path_Table {
	Selection_Path *			paths;
	int							size;
	int							index;

		int
			record_path (Include_Path const & p, Segment const * s) {
				if (++index >= size) {
					Selection_Path *	old_paths = paths;
					size += 1024;
					ZALLOC_ARRAY (paths, size, Selection_Path);
					if (old_paths != null) {
						COPY_ARRAY (old_paths, index, Selection_Path, paths);
						FREE_ARRAY (old_paths, index, Selection_Path);
					}
				}
				paths[index].path = p;
				paths[index].segment = s;
				return index;
			}

		bool
			resolve_path (int i, Include_Path & p, Segment const * & s) const {
				/* Path index 0 is reserved to indicate an empty path - see preincrement in record_path above. */
				if (i <= 0 || i > index)
					return false;
				p = paths[i].path;
				s = paths[i].segment;
				return true;
			}

		bool
			resolve_path (int i, Segment const * & s) const {
				/* Path index 0 is reserved to indicate an empty path - see preincrement in record_path above. */
				if (i <= 0 || i > index)
					return false;
				s = paths[i].segment;
				return true;
			}

		void
			clean () {
				if (paths != null) {
					for (int i = 0; i <= index; ++i)
						paths[i].path = null;
					FREE_ARRAY (paths, size, Selection_Path);
				}
			}
};




typedef int		SE_Flags;
#define SE_COMPUTE				(SE_Flags)0x0001
#define SE_COMPUTE_BY_VOLUME	(SE_Flags)0x0002
#define SE_COMPUTE_WORLD_SPACE	(SE_Flags)0x0004
#define SE_COMPUTE_BY_POLYGON	(SE_Flags)0x0008
#define SE_COMPUTE_VISUAL		(SE_Flags)0x0010
#define SE_PENETRATE_SUBWINDOWS	(SE_Flags)0x0040
#define SE_BIAS_LINES			(SE_Flags)0x0080
#define SE_BIAS_MARKERS			(SE_Flags)0x0100
#define SE_BIAS_Z_SORT			(SE_Flags)0x0200

struct Selection_Map;

class Selection_Event : public Event {
public:
	Selection_Event(Locater_Actions in_action, Actor * in_target, Segment const * in_start_segment = null, Actor * in_source = null);
	Selection_Event(Selection_Event const & that);
	~Selection_Event();

	Actor *					source;
	Actor *					target;
	Locater_Actions			locater_action;
	int32_t					status;
	Volume					locater_volume;
	Segment const *			start_segment;
	Include_Path			start_path;
	Selection_Item_Heap *	item_heap;
	Selection_Item *		item_list;
	Point *					poly;
	int						count;
	SE_Flags				flags;

	Heuristics const *		heuristics;

	Selection_Sample *		selection_sample;
	Selection_Map *			selection_map;
	Path_Table *			path_table;
	Highlight_Map *			highlight_map;
};



struct Key_List {
	Key_List *				next;

	Anything *				owner;
	Key						renumbered;
	Key						original;
};

struct Pending_Special {
	Pending_Special	*		next;
	Name					type;
	Name					value;
};


struct Copy_Segment_Passthru {
	Geometry const *	geometry;	// original geometry if any
	Key					key;		// the output segment we make
	Key const *			old_keys;	// keys to watch for
	Key *				new_keys;	// equivalent keys if seen
	int					count;		// number of keys to watch
	int					offset;		// for copy from containers

#define		Copy_Segment_SKIP_RENUMBER			0x0001
#define		Copy_Segment_REFERENCE_GEOMETRY		0x0002
#define		Copy_Segment_IGNORE_DRIVER			0x0004
	int					flags;		// mostly special options for Optimize_Segment
};



#define BASE_COLOR				0
#define FIRST_DEGREE_COLOR		1
#define SECOND_DEGREE_COLOR		2
#define BLEND_COLOR				3


struct Set_And_Clear_Flags_Control {
	struct {
		Activity				clear,
								set;
	}						activities;
	struct {
		Database_Flags			clear,
								set;
	}						database;
	Rectangle		area;
	bool					compute_bounding;
	bool					clear_stats;
	bool					compile_only;
	bool					reset_device;
};


struct Pending_Activity {


	Pending_Activity() : who(), what() {
		;
	};

	Pending_Activity(Segstuff const * who, Activity what) : who(who), what(what) {
		UTILE(who);
	};

	~Pending_Activity() {
		if (who != null)
			INUTILE(who);
	};

	Pending_Activity(Pending_Activity const & that) {
		who = that.who;
		what = that.what;
		UTILE(who);
	}

	Pending_Activity(Pending_Activity && that) {
		who = that.who;
		what = that.what;
		that.who = null;
	}

	Pending_Activity const & operator= (Pending_Activity const & that) {
		who = that.who;
		what = that.what;
		UTILE(who);
		return *this;
	}

	static INLINE void Process(Thread_Data * thread_data) {
		Mutexer mutexer(HOOPS::WORLD->hash_mutex);
		auto & av = *HOOPS::WORLD->pending_activity;
		if (!av.empty()) {
			HOOPS::WORLD->flags |= World::UPDATE_NEEDED;
			for (auto & x : av) {
				if (!ANYBIT(x.who->dbflags, DB_DELETED))
					PROPAGATE_ACTIVITY(thread_data, x.who, x.what);
				INUTILE(x.who);
				x.who = null;
			}
			av.clear();
		}
	}

	template <typename T>
	static INLINE void Add_Activity(T const * who, Activity what) {
		Mutexer mutexer(HOOPS::WORLD->hash_mutex);
		HOOPS::WORLD->pending_activity->push_back(Pending_Activity((Segstuff const *)who, what));
	}

	Segstuff const *	who;
	Activity			what;
};



#define RFG_Flags_None				0x00000000
#define RFG_Flags_Depopulate		0x00000001
#define RFG_Flags_Retain_Includes	0x00000002
#define RFG_Flags_Recurse			0x00000004





struct Open_Item : public Minimum_Header<Open_Item, World> {
	Open_Type				open_type;

#define		Open_NONE				(Open_Type)0x00000000
#define		Open_SEGMENT			(Open_Type)0x00000001
#define		Open_GEOMETRY			(Open_Type)0x00000002
#define		Open_EDGE				(Open_Type)0x00000004
#define		Open_VERTEX				(Open_Type)0x00000010
#define		Open_FACE				(Open_Type)0x00000020
#define		Open_LOD				(Open_Type)0x00000040
#define		Open_TRIM				(Open_Type)0x00000100
#define		Open_REGION				(Open_Type)0x00000200


#define		Open_ATTRIBUTE			(Open_Type)0x00001000
#define		Open_NOT_WHEN_STREAMING	(Open_Type)0x00002000
#define		Open_GEO_TYPE_MASK		(Open_Type)0x00004000
#define		Open_SHOW				(Open_Type)0x00008000
//#define	Open_THESE_ARE_USED		(Open_Type)0x00FF0000
#define		Open_ASSUME_LOCKED		(Open_Type)0x01000000
#define		Open_NOT_ON_ROOT		(Open_Type)0x02000000


#define		OPEN_ATTRIBUTE(_type_)	((((_type_)<<16)&0x00FF0000)|Open_ATTRIBUTE)
#define		EXTRACT_OPEN_ATTRIBUTE(_flags_)	(DB_Type)(char)(((_flags_)&0x00FF0000)>>16)

#define		OPEN_GEO_TYPES(_type_)	((((_type_)<<16)&0x00FF0000)|Open_GEO_TYPE_MASK)
#define		EXTRACT_OPEN_GEO_MASK(_flags_, _lookup_)	((_lookup_)[(((_flags_)&0x00FF0000)>>16)])


// local int const	geo_type_masks[] is in open_segment.cpp

#define		OPEN_GEO_NOTHING					0
#define		OPEN_GEO_PHEDRON_AND_POLYCYLINDER	1
#define		OPEN_GEO_PHEDRON					2
#define		OPEN_GEO_BOUNDED					3
#define		OPEN_GEO_CIRCLES					4
#define		OPEN_GEO_CURVE_POLYLINE				5
#define		OPEN_GEO_TEXT						6
#define		OPEN_GEO_POLYCYLINDER				7
#define		OPEN_GEO_CIRCLE						8
#define		OPEN_GEO_SHELL						9
#define		OPEN_GEO_MESH						10
#define		OPEN_GEO_CIRCULAR_ARC				11
#define		OPEN_GEO_CIRCULAR_CHORD				12
#define		OPEN_GEO_CIRCULAR_WEDGE				13
#define		OPEN_GEO_CUTTING_SECTION			14
#define		OPEN_GEO_LIGHT						15
#define		OPEN_GEO_ELLIPSE					16
#define		OPEN_GEO_ELLIPTICAL_ARC				17
#define		OPEN_GEO_INFINITE_LINE				18
#define		OPEN_GEO_LINE						19
#define		OPEN_GEO_MARKER						20
#define		OPEN_GEO_NURBS_CURVE				21
#define		OPEN_GEO_NURBS_SURFACE				22
#define		OPEN_GEO_POLYGON					23
#define		OPEN_GEO_SPHERE						24
#define		OPEN_GEO_POLYLINE					25
#define		OPEN_GEO_IMAGE						26
#define		OPEN_GEO_GRID						27
#define		OPEN_GEO_CYLINDER					28


#define		Open_MSET_VERTEX_COLORS		(Open_GEOMETRY|OPEN_GEO_TYPES(OPEN_GEO_PHEDRON_AND_POLYCYLINDER))
#define		Open_MSET_FACE_COLORS		(Open_GEOMETRY|OPEN_GEO_TYPES(OPEN_GEO_PHEDRON))
#define		Open_MSET_VERTEX_NORMALS	(Open_GEOMETRY|OPEN_GEO_TYPES(OPEN_GEO_PHEDRON))
#define		Open_MSET_VERTEX_PARAMS		(Open_GEOMETRY|OPEN_GEO_TYPES(OPEN_GEO_PHEDRON))
#define		Open_MSET_VERTEX_VISIBILITY	(Open_GEOMETRY|OPEN_GEO_TYPES(OPEN_GEO_PHEDRON))

#define		Open_EDIT_TEXT				(Open_GEOMETRY|OPEN_GEO_TYPES(OPEN_GEO_TEXT))
#define		Open_EDIT_CIRCLE			(Open_GEOMETRY|OPEN_GEO_TYPES(OPEN_GEO_CIRCLE))
#define		Open_EDIT_POLYCYLINDER		(Open_GEOMETRY|OPEN_GEO_TYPES(OPEN_GEO_POLYCYLINDER))
#define		Open_EDIT_SHELL				(Open_GEOMETRY|OPEN_GEO_TYPES(OPEN_GEO_SHELL))
#define		Open_EDIT_MESH				(Open_GEOMETRY|OPEN_GEO_TYPES(OPEN_GEO_MESH))
#define		Open_EDIT_CIRCULAR_ARC		(Open_GEOMETRY|OPEN_GEO_TYPES(OPEN_GEO_CIRCULAR_ARC))
#define		Open_EDIT_CIRCULAR_CHORD	(Open_GEOMETRY|OPEN_GEO_TYPES(OPEN_GEO_CIRCULAR_CHORD))
#define		Open_EDIT_CIRCULAR_WEDGE	(Open_GEOMETRY|OPEN_GEO_TYPES(OPEN_GEO_CIRCULAR_WEDGE))
#define		Open_EDIT_CUTTING_SECTION	(Open_GEOMETRY|OPEN_GEO_TYPES(OPEN_GEO_CUTTING_SECTION))
#define		Open_EDIT_LIGHT				(Open_GEOMETRY|OPEN_GEO_TYPES(OPEN_GEO_LIGHT))
#define		Open_EDIT_ELLIPSE			(Open_GEOMETRY|OPEN_GEO_TYPES(OPEN_GEO_ELLIPSE))
#define		Open_EDIT_ELLIPTICAL_ARC	(Open_GEOMETRY|OPEN_GEO_TYPES(OPEN_GEO_ELLIPTICAL_ARC))
#define		Open_EDIT_INFINITE_LINE		(Open_GEOMETRY|OPEN_GEO_TYPES(OPEN_GEO_INFINITE_LINE))
#define		Open_EDIT_LINE				(Open_GEOMETRY|OPEN_GEO_TYPES(OPEN_GEO_LINE))
#define		Open_EDIT_MARKER			(Open_GEOMETRY|OPEN_GEO_TYPES(OPEN_GEO_MARKER))
#define		Open_EDIT_NURBS_CURVE		(Open_GEOMETRY|OPEN_GEO_TYPES(OPEN_GEO_NURBS_CURVE))
#define		Open_EDIT_NURBS_SURFACE		(Open_GEOMETRY|OPEN_GEO_TYPES(OPEN_GEO_NURBS_SURFACE))
#define		Open_EDIT_POLYGON			(Open_GEOMETRY|OPEN_GEO_TYPES(OPEN_GEO_POLYGON))
#define		Open_EDIT_SPHERE			(Open_GEOMETRY|OPEN_GEO_TYPES(OPEN_GEO_SPHERE))
#define		Open_EDIT_POLYLINE			(Open_GEOMETRY|OPEN_GEO_TYPES(OPEN_GEO_POLYLINE))
#define		Open_EDIT_GRID				(Open_GEOMETRY|OPEN_GEO_TYPES(OPEN_GEO_GRID))
#define		Open_EDIT_CYLINDER			(Open_GEOMETRY|OPEN_GEO_TYPES(OPEN_GEO_CYLINDER))

#define		Open_INSERT_GEOMETRY		(Open_SEGMENT|Open_LOD)

#define		Open_SET_BOUNDING			(Open_SEGMENT)
#define		Open_SHOW_BOUNDING			(Open_SHOW|Open_SEGMENT|Open_GEOMETRY|OPEN_GEO_TYPES(OPEN_GEO_BOUNDED)|Open_REGION|Open_VERTEX|Open_EDGE|Open_FACE)

#define		Open_SET_TEXT_REGION		(Open_GEOMETRY|OPEN_GEO_TYPES(OPEN_GEO_TEXT))
#define		Open_SHOW_TEXT_REGION		(Open_SHOW|Open_GEOMETRY|OPEN_GEO_TYPES(OPEN_GEO_TEXT))
#define		Open_EDIT_TEXT				(Open_GEOMETRY|OPEN_GEO_TYPES(OPEN_GEO_TEXT))

#define		Open_SET_CIRCULAR_CENTER	(Open_GEOMETRY|OPEN_GEO_TYPES(OPEN_GEO_CIRCLES))
#define		Open_SHOW_CIRCULAR_CENTER	(Open_SHOW|Open_GEOMETRY|OPEN_GEO_TYPES(OPEN_GEO_CIRCLES))

#define		Open_GENERATE_CURVE_POLYLINE	(Open_SHOW|Open_GEOMETRY|OPEN_GEO_TYPES(OPEN_GEO_CURVE_POLYLINE))

#define		Open_SET_COLOR					(Open_SEGMENT|Open_GEOMETRY|Open_EDGE|Open_FACE|Open_VERTEX|Open_REGION|OPEN_ATTRIBUTE(COLOR))
#define		Open_SET_LINE_PATTERN			(Open_SEGMENT|Open_GEOMETRY|OPEN_ATTRIBUTE(LINE_PATTERN))
#define		Open_SET_EDGE_PATTERN			(Open_SEGMENT|Open_GEOMETRY|Open_EDGE|OPEN_ATTRIBUTE(EDGE_PATTERN))
#define		Open_SET_MARKER_SYMBOL			(Open_SEGMENT|Open_GEOMETRY|Open_VERTEX|OPEN_ATTRIBUTE(MARKER_SYMBOL))
#define		Open_SET_CALLBACK				(Open_SEGMENT|OPEN_ATTRIBUTE(IM_CALLBACK))
#define		Open_SET_CAMERA					(Open_SEGMENT|OPEN_ATTRIBUTE(CAMERA))
#define		Open_SET_CLIP_REGION			(Open_SEGMENT|OPEN_ATTRIBUTE(CLIP_REGION))
#define		Open_SET_COLOR_MAP				(Open_SEGMENT|Open_GEOMETRY|OPEN_ATTRIBUTE(COLOR_MAP))
#define		Open_SET_CONDITIONS				(Open_SEGMENT|Open_GEOMETRY|OPEN_ATTRIBUTE(CONDITIONS))
#define		Open_SET_LINE_WEIGHT			(Open_SEGMENT|Open_GEOMETRY|OPEN_ATTRIBUTE(LINE_WEIGHT))
#define		Open_SET_EDGE_WEIGHT			(Open_SEGMENT|Open_GEOMETRY|Open_EDGE|OPEN_ATTRIBUTE(EDGE_WEIGHT))
#define		Open_SET_MARKER_SIZE			(Open_SEGMENT|Open_GEOMETRY|Open_VERTEX|OPEN_ATTRIBUTE(MARKER_SIZE))
#define		Open_SET_TEXT_SPACING			(Open_SEGMENT|Open_GEOMETRY|OPEN_ATTRIBUTE(TEXT_SPACING))
#define		Open_SET_FACE_PATTERN			(Open_SEGMENT|Open_GEOMETRY|Open_FACE|Open_REGION|OPEN_ATTRIBUTE(FACE_PATTERN))
#define		Open_SET_WINDOW_PATTERN			(Open_SEGMENT|OPEN_ATTRIBUTE(WINDOW_PATTERN))
#define		Open_SET_TEXT_ALIGNMENT			(Open_SEGMENT|Open_GEOMETRY|OPEN_ATTRIBUTE(TEXT_ALIGNMENT))
#define		Open_SET_TEXT_PATH				(Open_SEGMENT|Open_GEOMETRY|OPEN_ATTRIBUTE(TEXT_PATH))
#define		Open_SET_GLYPH_DEFS				(Open_SEGMENT|OPEN_ATTRIBUTE(GLYPH_DEFS))
#define		Open_SET_LINE_STYLE_DEFS		(Open_SEGMENT|OPEN_ATTRIBUTE(LINE_STYLE_DEFS))
#define		Open_SET_TEXT_FONT				(Open_SEGMENT|Open_GEOMETRY|OPEN_ATTRIBUTE(TEXT_FONT))
#define		Open_SET_DRIVER_OPTIONS			(Open_SEGMENT|OPEN_ATTRIBUTE(DRIVER_OPTIONS))
#define		Open_SET_HANDEDNESS				(Open_SEGMENT|Open_GEOMETRY|OPEN_ATTRIBUTE(HANDEDNESS))
#define		Open_SET_HEURISTICS				(Open_SEGMENT|Open_GEOMETRY|OPEN_ATTRIBUTE(HEURISTICS))
#define		Open_SET_MODELLING_MATRIX		(Open_SEGMENT|Open_GEOMETRY|OPEN_ATTRIBUTE(MODELLING_MATRIX))
#define		Open_SET_RENDERING_OPTIONS		(Open_SEGMENT|Open_GEOMETRY|OPEN_ATTRIBUTE(RENDERING_OPTIONS))
#define		Open_SET_SELECTABILITY			(Open_SEGMENT|Open_GEOMETRY|OPEN_ATTRIBUTE(SELECTABILITY))
#define		Open_SET_STREAMING_MODE			(Open_SEGMENT|OPEN_ATTRIBUTE(STREAMING_MODE))
#define		Open_SET_TEXTURE_MATRIX			(Open_SEGMENT|Open_GEOMETRY|OPEN_ATTRIBUTE(TEXTURE_MATRIX))
#define		Open_SET_TEXTURE_DEFS			(Open_SEGMENT|OPEN_ATTRIBUTE(TEXTURE_DEFS))
#define		Open_SET_VISIBILITY				(Open_SEGMENT|Open_GEOMETRY|Open_VERTEX|Open_EDGE|Open_FACE|Open_REGION|OPEN_ATTRIBUTE(VISIBILITY))
#define		Open_SET_WINDOW					(Open_SEGMENT|OPEN_ATTRIBUTE(WINDOW))
#define		Open_SET_WINDOW_FRAME			(Open_SEGMENT|OPEN_ATTRIBUTE(WINDOW_FRAME))
#define		Open_SET_USER_OPTIONS			(Open_SEGMENT|Open_GEOMETRY|OPEN_ATTRIBUTE(USER_OPTIONS))
#define		Open_SET_USER_VALUE				(Open_SEGMENT|Open_GEOMETRY|OPEN_ATTRIBUTE(USER_VALUE))
#define		Open_SET_NAMED_STYLE_DEFS		(Open_SEGMENT|OPEN_ATTRIBUTE(NAMED_STYLE_DEFS))

#define		Open_SHOW_COLOR					(Open_SHOW|Open_SEGMENT|Open_GEOMETRY|Open_EDGE|Open_FACE|Open_VERTEX|Open_REGION|OPEN_ATTRIBUTE(COLOR))
#define		Open_SHOW_LINE_PATTERN			(Open_SHOW|Open_SEGMENT|Open_GEOMETRY|OPEN_ATTRIBUTE(LINE_PATTERN))
#define		Open_SHOW_EDGE_PATTERN			(Open_SHOW|Open_SEGMENT|Open_GEOMETRY|Open_EDGE|OPEN_ATTRIBUTE(EDGE_PATTERN))
#define		Open_SHOW_MARKER_SYMBOL			(Open_SHOW|Open_SEGMENT|Open_GEOMETRY|Open_VERTEX|OPEN_ATTRIBUTE(MARKER_SYMBOL))
#define		Open_SHOW_CALLBACK				(Open_SHOW|Open_SEGMENT|OPEN_ATTRIBUTE(IM_CALLBACK))
#define		Open_SHOW_CAMERA				(Open_SHOW|Open_SEGMENT|OPEN_ATTRIBUTE(CAMERA))
#define		Open_SHOW_CLIP_REGION			(Open_SHOW|Open_SEGMENT|OPEN_ATTRIBUTE(CLIP_REGION))
#define		Open_SHOW_COLOR_MAP				(Open_SHOW|Open_SEGMENT|Open_GEOMETRY|OPEN_ATTRIBUTE(COLOR_MAP))
#define		Open_SHOW_CONDITIONS			(Open_SHOW|Open_SEGMENT|Open_GEOMETRY|OPEN_ATTRIBUTE(CONDITIONS))
#define		Open_SHOW_LINE_WEIGHT			(Open_SHOW|Open_SEGMENT|Open_GEOMETRY|OPEN_ATTRIBUTE(LINE_WEIGHT))
#define		Open_SHOW_EDGE_WEIGHT			(Open_SHOW|Open_SEGMENT|Open_GEOMETRY|Open_EDGE|OPEN_ATTRIBUTE(EDGE_WEIGHT))
#define		Open_SHOW_MARKER_SIZE			(Open_SHOW|Open_SEGMENT|Open_GEOMETRY|Open_VERTEX|OPEN_ATTRIBUTE(MARKER_SIZE))
#define		Open_SHOW_TEXT_SPACING			(Open_SHOW|Open_SEGMENT|Open_GEOMETRY|OPEN_ATTRIBUTE(TEXT_SPACING))
#define		Open_SHOW_FACE_PATTERN			(Open_SHOW|Open_SEGMENT|Open_GEOMETRY|Open_FACE|Open_REGION|OPEN_ATTRIBUTE(FACE_PATTERN))
#define		Open_SHOW_WINDOW_PATTERN		(Open_SHOW|Open_SEGMENT|OPEN_ATTRIBUTE(WINDOW_PATTERN))
#define		Open_SHOW_TEXT_ALIGNMENT		(Open_SHOW|Open_SEGMENT|Open_GEOMETRY|OPEN_ATTRIBUTE(TEXT_ALIGNMENT))
#define		Open_SHOW_TEXT_PATH				(Open_SHOW|Open_SEGMENT|Open_GEOMETRY|OPEN_ATTRIBUTE(TEXT_PATH))
#define		Open_SHOW_GLYPH_DEFS			(Open_SHOW|Open_SEGMENT|OPEN_ATTRIBUTE(GLYPH_DEFS))
#define		Open_SHOW_LINE_STYLE_DEFS		(Open_SHOW|Open_SEGMENT|OPEN_ATTRIBUTE(LINE_STYLE_DEFS))
#define		Open_SHOW_TEXT_FONT				(Open_SHOW|Open_SEGMENT|Open_GEOMETRY|OPEN_ATTRIBUTE(TEXT_FONT))
#define		Open_SHOW_DRIVER_OPTIONS		(Open_SHOW|Open_SEGMENT|OPEN_ATTRIBUTE(DRIVER_OPTIONS))
#define		Open_SHOW_HANDEDNESS			(Open_SHOW|Open_SEGMENT|Open_GEOMETRY|OPEN_ATTRIBUTE(HANDEDNESS))
#define		Open_SHOW_HEURISTICS			(Open_SHOW|Open_SEGMENT|Open_GEOMETRY|OPEN_ATTRIBUTE(HEURISTICS))
#define		Open_SHOW_MODELLING_MATRIX		(Open_SHOW|Open_SEGMENT|Open_GEOMETRY|OPEN_ATTRIBUTE(MODELLING_MATRIX))
#define		Open_SHOW_RENDERING_OPTIONS		(Open_SHOW|Open_SEGMENT|Open_GEOMETRY|OPEN_ATTRIBUTE(RENDERING_OPTIONS))
#define		Open_SHOW_SELECTABILITY			(Open_SHOW|Open_SEGMENT|Open_GEOMETRY|OPEN_ATTRIBUTE(SELECTABILITY))
#define		Open_SHOW_STREAMING_MODE		(Open_SHOW|Open_SEGMENT|OPEN_ATTRIBUTE(STREAMING_MODE))
#define		Open_SHOW_TEXTURE_MATRIX		(Open_SHOW|Open_SEGMENT|Open_GEOMETRY|OPEN_ATTRIBUTE(TEXTURE_MATRIX))
#define		Open_SHOW_TEXTURE_DEFS			(Open_SHOW|Open_SEGMENT|OPEN_ATTRIBUTE(TEXTURE_DEFS))
#define		Open_SHOW_VISIBILITY			(Open_SHOW|Open_SEGMENT|Open_GEOMETRY|Open_VERTEX|Open_EDGE|Open_FACE|Open_REGION|OPEN_ATTRIBUTE(VISIBILITY))
#define		Open_SHOW_WINDOW				(Open_SHOW|Open_SEGMENT|OPEN_ATTRIBUTE(WINDOW))
#define		Open_SHOW_WINDOW_FRAME			(Open_SHOW|Open_SEGMENT|OPEN_ATTRIBUTE(WINDOW_FRAME))
#define		Open_SHOW_USER_OPTIONS			(Open_SHOW|Open_SEGMENT|Open_GEOMETRY|OPEN_ATTRIBUTE(USER_OPTIONS))
#define		Open_SHOW_USER_VALUE			(Open_SHOW|Open_SEGMENT|Open_GEOMETRY|OPEN_ATTRIBUTE(USER_VALUE))
#define		Open_SHOW_NAMED_STYLE_DEFS		(Open_SHOW|Open_SEGMENT|OPEN_ATTRIBUTE(NAMED_STYLE_DEFS))

#define		Open_SHOW_POLYCYLINDER			(Open_SHOW|Open_GEOMETRY|OPEN_GEO_TYPES(OPEN_GEO_POLYCYLINDER))
#define		Open_SHOW_TEXT					(Open_SHOW|Open_GEOMETRY|OPEN_GEO_TYPES(OPEN_GEO_TEXT))
#define		Open_SHOW_CIRCLE				(Open_SHOW|Open_GEOMETRY|OPEN_GEO_TYPES(OPEN_GEO_CIRCLE))
#define		Open_SHOW_SHELL					(Open_SHOW|Open_GEOMETRY|OPEN_GEO_TYPES(OPEN_GEO_SHELL))
#define		Open_SHOW_MESH					(Open_SHOW|Open_GEOMETRY|OPEN_GEO_TYPES(OPEN_GEO_MESH))
#define		Open_SHOW_CIRCULAR_ARC			(Open_SHOW|Open_GEOMETRY|OPEN_GEO_TYPES(OPEN_GEO_CIRCULAR_ARC))
#define		Open_SHOW_CIRCULAR_CHORD		(Open_SHOW|Open_GEOMETRY|OPEN_GEO_TYPES(OPEN_GEO_CIRCULAR_CHORD))
#define		Open_SHOW_CIRCULAR_WEDGE		(Open_SHOW|Open_GEOMETRY|OPEN_GEO_TYPES(OPEN_GEO_CIRCULAR_WEDGE))
#define		Open_SHOW_CUTTING_SECTION		(Open_SHOW|Open_GEOMETRY|OPEN_GEO_TYPES(OPEN_GEO_CUTTING_SECTION))
#define		Open_SHOW_LIGHT					(Open_SHOW|Open_GEOMETRY|OPEN_GEO_TYPES(OPEN_GEO_LIGHT))
#define		Open_SHOW_ELLIPSE				(Open_SHOW|Open_GEOMETRY|OPEN_GEO_TYPES(OPEN_GEO_ELLIPSE))
#define		Open_SHOW_ELLIPTICAL_ARC		(Open_SHOW|Open_GEOMETRY|OPEN_GEO_TYPES(OPEN_GEO_ELLIPTICAL_ARC))
#define		Open_SHOW_INFINITE_LINE			(Open_SHOW|Open_GEOMETRY|OPEN_GEO_TYPES(OPEN_GEO_INFINITE_LINE))
#define		Open_SHOW_LINE					(Open_SHOW|Open_GEOMETRY|OPEN_GEO_TYPES(OPEN_GEO_LINE))
#define		Open_SHOW_MARKER				(Open_SHOW|Open_GEOMETRY|OPEN_GEO_TYPES(OPEN_GEO_MARKER))
#define		Open_SHOW_NURBS_CURVE			(Open_SHOW|Open_GEOMETRY|OPEN_GEO_TYPES(OPEN_GEO_NURBS_CURVE))
#define		Open_SHOW_NURBS_SURFACE			(Open_SHOW|Open_GEOMETRY|OPEN_GEO_TYPES(OPEN_GEO_NURBS_SURFACE))
#define		Open_SHOW_POLYGON				(Open_SHOW|Open_GEOMETRY|OPEN_GEO_TYPES(OPEN_GEO_POLYGON))
#define		Open_SHOW_SPHERE				(Open_SHOW|Open_GEOMETRY|OPEN_GEO_TYPES(OPEN_GEO_SPHERE))
#define		Open_SHOW_POLYLINE				(Open_SHOW|Open_GEOMETRY|OPEN_GEO_TYPES(OPEN_GEO_POLYLINE))
#define		Open_SHOW_IMAGE					(Open_SHOW|Open_GEOMETRY|OPEN_GEO_TYPES(OPEN_GEO_IMAGE))
#define		Open_SHOW_GRID					(Open_SHOW|Open_GEOMETRY|OPEN_GEO_TYPES(OPEN_GEO_GRID))



	union {
		struct {
#			define			Open_Item_SEGMENT_NO_CLOSE		DB_TYPE_SPECIFIC(1)
			Segment *		referee;
			Segment *		original_referee;
			Polyline *		inked_line;
		}				segment;

		struct {
			Geometry *		referee;
			int				offset;
		}				geometry;

		struct {
			Polyhedron *	referee;
			int				offset;
		}				polyhedron;

		struct {
			Anything *		referee;
			int				offset;
		}				lod;

		struct {
			NURBS_Surface *	referee;
			int				offset;
		}				nurbs_surface;
	}				info;
};




template <typename T>
local INLINE void UNLINK (T const * citem)
{
	T * item = (T*)citem;
	if ((*item->backlink = item->next) != null)
		item->next->backlink = item->backlink;
	item->backlink = &item->next;
	item->next = null;
}

local INLINE void UNLINK (Open_Item * item)
{
	Mutexer mutexer(HOOPS::WORLD->thread_mutex);
	if ((*item->backlink = item->next) != null)
		item->next->backlink = item->backlink;
	item->backlink = &item->next;
	item->next = null;
}



local INLINE void LINK_OPEN_ITEM (Thread_Data * thread_data, Open_Item * item)
{
	Mutexer mutexer(HOOPS::WORLD->thread_mutex);
	if ((item->next = thread_data->open_list) != null)
		thread_data->open_list->backlink = &item->next;
	item->backlink = &thread_data->open_list;
	thread_data->open_list = item;
}



local INLINE void LINK_SUBSEGMENT (Subsegment * ss, Segment * const seg)
{
	if ((ss->next = seg->subsegments) != null)
		seg->subsegments->backlink = &ss->next;
	Alter(seg)->subsegments = ss;
	ss->backlink = &Alter(seg)->subsegments;
}


local INLINE void LINK_INCLUDE_XREF (Include * include, Segment const * referee)
{
	if ((include->referee_next = referee->get_references()) != null)
		include->referee_next->referee_backlink = &include->referee_next;
	Alter(referee)->set_references ((Xref *)include);
}

local INLINE void LINK_STYLE_XREF (Style * style, Segment const * referee)
{
	if ((style->referee_next = referee->get_references()) != null)
		style->referee_next->referee_backlink = &style->referee_next;
	Alter(referee)->set_references ((Xref *)style);
}




enum Loop_Plane {
	XY_PLANE,
	XZ_PLANE,
	YZ_PLANE
};

#define LOOP_GET_X(_pt_, _pl_) (_pl_ == YZ_PLANE ? _pt_.y : _pt_.x)
#define LOOP_GET_Y(_pt_, _pl_) (_pl_ == XY_PLANE ? _pt_.y : _pt_.z)

template <typename F>
struct Loop_Info_3D {	/* contains some properties of each loop */
	int				loop_index;
	signed char		orientation;
	Point_3D<F>		min, max;
};
typedef Loop_Info_3D<float>		Loop_Info;
typedef Loop_Info_3D<double>	DLoop_Info;

template <typename F>
struct Loop_Containment_Tree_3D {	/* represents the loop containment hierarchy */
	Loop_Info_3D<F> *				stats;
	Loop_Containment_Tree_3D<F> *	next;
	Loop_Containment_Tree_3D<F> *	child;
};
typedef Loop_Containment_Tree_3D<float>		Loop_Containment_Tree;
typedef Loop_Containment_Tree_3D<double>	DLoop_Containment_Tree;


class Shell_Optimization_Config {
public:
	enum : long {
		Normal_Tolerance_DEG = 1,
		Normal_Tolerance_RAD = 2,
	};
	enum : long {
		Tolerance_ORU = 1,
		Tolerance_FRU = 2,
	};

	Shell_Optimization_Config ();

public:
	Callback_Name		collapsible_func_name;		 /* undocumented feature */

	long				normal_tolerance_units;
	long				tolerance_units;

	float				normal_tolerance;
	float				tolerance;
	float				lod_ratio;
	float				lod_max_degree;

	Decimate_Algorithm	lod_algorithm;

	bool				orphan_elimination;
	bool				lod;	
	bool				skin;
	bool				fix;
	bool				reverse_handedness;
};


template <typename F>
class Collapse_Predicate {
public:
	struct Input {
		Point_3D<F> const *	points;
		Vector const *		normals;
		double				max_point_distance_squared;
		double				max_normal_distance_squared;
		int32_t				point_index_1;
		int32_t				point_index_2;
	};

public:
	typedef	bool (HC_CDECL * Modern)	(Input const & input, void * user_data);

	typedef	int  (HC_CDECL * Vintage)	(void const * points,
										 void const * normals,
										 int32_t point_index_1,
										 int32_t point_index_2,
										 double max_point_distance_squared,
										 double max_normal_distance_squared);

public:
	Collapse_Predicate ()
		: func(default_predicate)
		, user_data(null)
		, vintage_box(nullroutine)
	{}

	Collapse_Predicate (Vintage func)
		: func(vintage_to_modern)
		, user_data(&vintage_box)
		, vintage_box(func)
	{}

	Collapse_Predicate (Modern func, void * user_data)
		: func(func)
		, user_data(user_data)
		, vintage_box(nullroutine)
	{}

	bool operator() (Input const & input) const {
		return func(input, user_data);
	}

private:
	static bool HC_CDECL default_predicate (Input const & input, void * user_data) {
		UNREFERENCED(user_data);

		int32_t const idx1 = input.point_index_1;
		int32_t const idx2 = input.point_index_2;

		Point_3D<F> const *	points = input.points;
		if ((points[idx1] - points[idx2]).LengthSquared() > input.max_point_distance_squared)
			return false;

		Vector const * normals = input.normals;
		if (normals != null) {
			if ((normals[idx1] - normals[idx2]).LengthSquared() > input.max_normal_distance_squared)
				return false;
		}

		return true;
	}

	static bool HC_CDECL vintage_to_modern (Input const & in, void * user_data) {
		Vintage f = static_cast<Vintage_Box *>(user_data)->vintage_func;
		return f(in.points, in.normals, in.point_index_1, in.point_index_2, in.max_point_distance_squared, in.max_normal_distance_squared) != 0;
	}

private:
	class Vintage_Box {
	public:
		Vintage_Box (Vintage func)
			: vintage_func(func)
		{}

	public:
		Vintage		vintage_func;
	};

private:
	Modern			func;
	void *			user_data;
	Vintage_Box		vintage_box;
};


class Begin_Decimate_Config {
public:
	Begin_Decimate_Config ();

public:
	int * out_old_to_new_vertex_mapping;
	bool collapse_vertices;
	bool abort_if_extra_points_are_added;
	bool abort_if_face_hole_is_present;
	bool shell_is_part_of_database;
};


class Decimate_Config {
public:
	Decimate_Config (int percent_quality);
	~Decimate_Config ();

private:
	Decimate_Config (Decimate_Config const &); // disable
	void operator= (Decimate_Config); // disable

public:
	void * const opaque;
	Decimate_Algorithm algorithm;
	float max_vertices_to_introduce_factor;
	size_t min_face_count;
	int * out_old_to_new_face_mapping;
};


struct HPS_Search_Type_List {
	int						type;
	HPS_Search_Type_List *	next;
};

struct Search_List {
	Search_List *			next;
	Anything *				item;
	Key						key;
	DB_Type					type;
	bool					special;
	HPS_Search_Type_List *	hps_types;
};

struct Search_Header {
	Search_Header *			prev;
	Search_List *			list;
	int						count;

	Destructible<Callback_Name_Vector_Pooled>::unique_ptr	callback_name_vector;

	// used by font searches...
	Name_Const *			names;
	unsigned				name_count;
	unsigned				next;
	Actor *					original_driver;
};


/* used by Find_Contents, Show_Existence, and related routines... */
#define FC_LAST_SEARCH_TYPE				LAST_TYPE
#define FC_INTERESTING_ARRAY_SIZE		((int)FC_LAST_SEARCH_TYPE + 1)

#define FC_NORMAL						(BAD_TYPE+0)
#define FC_PARAMETER					(BAD_TYPE+1)
#define FC_FACE							(BAD_TYPE+2)
#define FC_EDGE							(BAD_TYPE+3)
#define FC_VERTEX						(BAD_TYPE+4)

#define FC_ATTRIBUTES					(BAD_TYPE+5)
#define FC_GEOMETRY						(BAD_TYPE+6)
#define FC_SUBSEGMENTS					(BAD_TYPE+7)
#define FC_ORDERED						(BAD_TYPE+8)
#define FC_INKED_LINE					(BAD_TYPE+9)
#define FC_SELF							(BAD_TYPE+10)
#define FC_INTERESTING					(BAD_TYPE+11)
#define FC_FIRST_MATCH					(BAD_TYPE+12)
#define FC_EVERYTHING					(BAD_TYPE+13)

#define FC_DISTANT_LIGHTS				(BAD_TYPE+14)
#define FC_POINT_LIGHTS					(BAD_TYPE+15)
#define FC_SPOT_LIGHTS					(BAD_TYPE+16)
#define FC_AREA_LIGHTS					(BAD_TYPE+17)

#define FC_INCLUDERS					(BAD_TYPE+18)
#define FC_STYLERS						(BAD_TYPE+19)
#define FC_REFERRERS					(BAD_TYPE+20)

#define FC_FACE_ATTRIBUTES				(BAD_TYPE+21)
#define FC_FACE_COLORS					(BAD_TYPE+22)
#define FC_FACE_NORMALS					(BAD_TYPE+23)
#define FC_FACE_PATTERNS				(BAD_TYPE+24)
#define FC_FACE_VISIBILITIES			(BAD_TYPE+25)
#define FC_EDGE_ATTRIBUTES				(BAD_TYPE+26)
#define FC_EDGE_COLORS					(BAD_TYPE+27)
#define FC_EDGE_NORMALS					(BAD_TYPE+28)
#define FC_EDGE_PATTERNS				(BAD_TYPE+29)
#define FC_EDGE_WEIGHTS					(BAD_TYPE+30)
#define FC_EDGE_VISIBILITIES			(BAD_TYPE+31)
#define FC_VERTEX_ATTRIBUTES			(BAD_TYPE+32)
#define FC_VERTEX_COLORS				(BAD_TYPE+33)
#define FC_VERTEX_FACE_COLORS			(BAD_TYPE+34)
#define FC_VERTEX_EDGE_COLORS			(BAD_TYPE+35)
#define FC_VERTEX_MARKER_COLORS			(BAD_TYPE+36)
#define FC_VERTEX_MARKER_RGBAS			(BAD_TYPE+37)
#define FC_VERTEX_NORMALS				(BAD_TYPE+38)
#define FC_VERTEX_PARAMETERS			(BAD_TYPE+39)
#define FC_VERTEX_SYMBOLS				(BAD_TYPE+40)
#define FC_VERTEX_SIZES					(BAD_TYPE+41)
#define FC_VERTEX_VISIBILITIES			(BAD_TYPE+42)

#define	FC_STATIC_MODEL					(BAD_TYPE+43)
#define	FC_TRANSPARENCY_PRESENT			(BAD_TYPE+44)

#define FC_INFINITE_RAY					(BAD_TYPE+45)
#define FC_GEOMETRY_OPTIONS				(BAD_TYPE+46)

#define FC_BOUNDING						(BAD_TYPE+47)
#define FC_EXPLICIT_BOUNDING			(BAD_TYPE+48)

#define FC_SHADERS						(BAD_TYPE+49)

#define FC_CONDITIONAL_ACTIONS			(BAD_TYPE+50)
#define FC_NAMED_STYLE					(BAD_TYPE+51)

#define FC_FOLLOW_REFERENCES			(BAD_TYPE+52)


struct Find_Contents_Data {
	union {
		Search_List **			linker;
		int						count;
	}							item;

	bool						interesting[FC_INTERESTING_ARRAY_SIZE];
	bool						everything;
	bool						explicitly_everything;
	bool						any_attributes;
	bool						any_geometries;
	bool						include_self;
	bool						includers;
	bool						stylers;
	bool						referrers;
	bool						static_model;
	bool						transparency_present;
	bool						naughty_bit;
	bool						first_match;
	bool						any_local_attributes;
	bool						any_face_attributes;
	bool						any_edge_attributes;
	bool						any_vertex_attributes;
	bool						any_vertex_colors;
	bool						infinite_ray;
	bool						geometry_options;
	bool						bounding;
	bool						explicit_bounding;
	bool						follow_references;
	bool						priority; /* priority is HPS-only and experimental for now */

#	define		FC_INCLUDE		(-1)
#	define		FC_DONT_CARE	0
#	define		FC_EXCLUDE		1
	int							ordered;
	int							inked;

	struct {
		bool					distant;
		bool					point;
		bool					spot;
		bool					area;
	}							lights;
	struct {
		bool					face_colors;
		bool					face_normals;
		bool					face_patterns;
		bool					face_visibilities;
		bool					edge_colors;
		bool					edge_normals;
		bool					edge_patterns;
		bool					edge_weights;
		bool					edge_visibilities;
		bool					vertex_face_colors;
		bool					vertex_edge_colors;
		bool					vertex_marker_colors;
		bool					vertex_marker_rgbas;
		bool					vertex_normals;
		bool					vertex_parameters;
		bool					vertex_symbols;
		bool					vertex_sizes;
		bool					vertex_visibilities;
	}							local_attributes;

	struct {
		Type_Flags				selectability;
		Type_Flags				visibility;

#	define		FC_TEXTURE_DEFS_TEXTURES	0x1
#	define		FC_TEXTURE_DEFS_SHADERS		0x2
#	define		FC_TEXTURE_DEFS_ANY			0x3
		int						texture_definition_subtypes;
#	define		FC_CONDITION_CONDITIONS		0x1
#	define		FC_CONDITION_ACTIONS		0x2
#	define		FC_CONDITION_ANY			0x3
		int						condition_subtypes;
#	define		FC_STYLE_STYLES			0x1
#	define		FC_STYLE_NAMED			0x2
#	define		FC_STYLE_PORTFOLIO		0x4
#	define		FC_STYLE_PALETTE		0x8
#	define		FC_STYLE_ANY			0xF
		int						style_subtypes;
		struct {
			Driver_Options::Drivo_Bits	bits_mask;
		}						driver_options;
		struct {
			Heuristics_Flags		mask;
			short					culling;
			short					selection_culling;
		}						heuristics;
		Font_Flags					text_font;
		struct {
			Color_Object			mask;
			Material_Components		materials[Color_Type_MATERIAL_COUNT];
			int						diffuse_layer[Color_Type_MATERIAL_COUNT];
		}						color;
		struct {
			Rendo_Flags					complex;
			Rendo_Flags					simple;
			Rendo_Flags					interpolation;
			Attribute_Lock				locks;
			short						transparency_options;
			Buffer_Flags				buffer_options;
			Hidden_Line_Flags			hidden_line_options;
			LOD_Flags					lod_options;
			int							geometry;
			Isoline_Contour_Flags		isoline_contour;
			NURBS_Curve_Flags			nurbs_curve;
			NURBS_Surface_Flags			nurbs_surface;
			Tessellation_Flags			tessellation;
			Cut_Geometry_Flags			cut_geometry;
			Simple_Shadow_Flags			simple_shadow;
			Simple_Reflection_Flags		simple_reflection;
			Matrix_Overrides			matrix_overrides;
		}							rendering_options;
		struct {
			int							count;
			int *						ids;
		}							user_options;
		struct {
			int							count;
			int *						ids;
		}							conditions;
		struct {
			int							count;
			int *						ids;
		}							conditional_actions;
		struct {
			int							count;
			int *						ids;
		}							callbacks;
	}							specifics;

	bool						flushing;

	bool						using_hps;
};



End_HOOPS_Namespace

#endif
