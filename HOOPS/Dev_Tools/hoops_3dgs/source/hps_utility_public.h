// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef HPS_UTILITY_PUBLIC_H
#define HPS_UTILITY_PUBLIC_H

#include <stdint.h>

#include	<limits>
#include	<cmath>
#include	<cfloat>
#include	<cstring>

#ifndef HPS_API
#	define HPS_API HOOPS_API
#endif
#ifndef HPS_TEMPLATE_API
#	define HPS_TEMPLATE_API HOOPS_TEMPLATE_API
#endif


namespace HPS
{

#ifdef _MSC_VER
#	ifndef HPS_INLINE
#		define	HPS_INLINE		__forceinline
#	endif
#else
#	define HPS_INLINE inline
#endif

#ifndef HPS_UNREFERENCED
#define HPS_UNREFERENCED(param) ((void)(param))
#endif


const double PI = 3.141592653589793238462643383279502884197169399375105820974944592308;

template<typename T>
HPS_INLINE T Degrees_To_Radians(T const & degrees)
{
	return degrees * (T)(PI / 180.0);
}

template<typename T>
HPS_INLINE T Radians_To_Degrees(T const & radians)
{
	return radians * (T)(180.0 / PI);
}

template<typename T>
HPS_INLINE void SinCos(T const & angle, T & sine, T & cosine) {
	T a = Degrees_To_Radians(angle);
	cosine = cos(a);
	sine = sin(a);
}

template<typename T>
HPS_INLINE T Cos(T const & angle) {
	return cos (Degrees_To_Radians(angle));
}

template<typename T>
HPS_INLINE T Sin(T const & angle) {
	return sin (Degrees_To_Radians(angle));
}

template<typename T>
HPS_INLINE T ACos(T const & x) {
	if (x > 1)
		return 0;
	if (x < -1)
		return 180;
	return Radians_To_Degrees(acos(x));
}

template<typename T>
HPS_INLINE T ATan2(T const & y, T const & x) {
	if (x == 0 && y == 0)
		return 0;
	return Radians_To_Degrees(atan2(y, x));
}


template <typename F>	struct Float_Traits {};
template <> struct Float_Traits<float>	{
	typedef double	Alternative;
	static const int Type = 1;
	static float Epsilon () { return 1.0e-30f; }
};
template <> struct Float_Traits<double>	{
	typedef float	Alternative;
	static const int Type = 2;
	static double Epsilon () { return 1.0e-300; }
};



/*! The Float class is a concept class that exposes a number of useful utilities for working with floating point numbers.	*/
class HPS_API Float {
private:
	enum Parts {
#		if HOOPS_BIGENDIAN
			High, Low
#		else
			Low, High
#		endif
	};

	// & functions for a float represented in an int, * version for a double in an array of 2 ints
	static HPS_INLINE bool is_infinite(int32_t const & v)	{ return (v & 0x7FFFFFFF) == 0x7F800000; }
	static HPS_INLINE bool is_infinite(uint32_t const & v)	{ return (v & 0x7FFFFFFF) == 0x7F800000; }
	static HPS_INLINE bool is_infinite(int32_t const * v)	{ return (v[High] & 0x7FFFFFFF) == 0x7FF00000 && v[Low] == 0; }
	static HPS_INLINE bool is_infinite(uint32_t const * v)	{ return (v[High] & 0x7FFFFFFF) == 0x7FF00000 && v[Low] == 0; }

	static HPS_INLINE bool is_nan(int32_t const & v) {
		uint32_t	exp = v & 0x7F800000, mantissa = v & 0x007FFFFF;
		return exp == 0x7F800000 && mantissa != 0;
	}
	static HPS_INLINE bool is_nan(uint32_t const & v) {
		uint32_t	exp = v & 0x7F800000, mantissa = v & 0x007FFFFF;
		return exp == 0x7F800000 && mantissa != 0;
	}
	static HPS_INLINE bool is_nan(int32_t const * v) {
		uint32_t	exp = v[High] & 0x7FF00000, mantissa_high = v[High] & 0x000FFFFF;
		return exp == 0x7FF00000 && (mantissa_high | v[Low]) != 0;
	}
	static HPS_INLINE bool is_nan(uint32_t const * v) {
		uint32_t	exp = v[High] & 0x7FF00000, mantissa_high = v[High] & 0x000FFFFF;
		return exp == 0x7FF00000 && (mantissa_high | v[Low]) != 0;
	}

	static HPS_INLINE bool is_special(int32_t const & v)	{ return (v & 0x7F800000) == 0x7F800000; }
	static HPS_INLINE bool is_special(uint32_t const & v)	{ return (v & 0x7F800000) == 0x7F800000; }
	static HPS_INLINE bool is_special(int32_t const * v)	{ return (v[High] & 0x7FFFFFFF) == 0x7FF00000; }
	static HPS_INLINE bool is_special(uint32_t const * v)	{ return (v[High] & 0x7FFFFFFF) == 0x7FF00000; }
public:

	/*! The 32-bit float representation of infinity.  */
	static const float Infinity;
	/*! The 32-bit float representation of negative infinity.  */
	static const float NegativeInfinity;

	/*! See if the value is either infinity */
	static HPS_INLINE bool IsInfinite(float const & a) { return is_infinite (extract_uint32_t(a)); }
	static HPS_INLINE bool IsInfinite(double const & a) {
		uint32_t	v[2];
		memcpy (v, &a, sizeof(double));
		return is_infinite (v);
	}

	/*! See if the value is Not-A-Number */
	static HPS_INLINE bool IsNAN(float const & a) { return is_nan (extract_uint32_t(a)); }
	static HPS_INLINE bool IsNAN(double const & a) {
		uint32_t	v[2];
		memcpy (v, &a, sizeof(double));
		return is_nan (v);
	}

	/*! See if the value is not "normal" (infinite or NaN) */
	static HPS_INLINE bool IsAbnormal(float const & a) { return is_special (extract_uint32_t(a)); }
	static HPS_INLINE bool IsAbnormal(double const & a) {
		uint32_t	v[2];
		memcpy (v, &a, sizeof(double));
		return is_special (v);
	}

	/*! Checks two floats for equality within a specified tolerance.
	The tolerance is specified in float increments that scale with the floats themselves. */
	static HPS_INLINE bool Equals(float const & a, float const & b, int tolerance = 32);
	static HPS_INLINE bool Equals(double const & a, double const & b, int tolerance = 32);

#ifndef HOOPS_DEFINED
	template <typename Alloc>
	static HPS_INLINE bool Equals(std::vector<float, Alloc> const & a, std::vector<float, Alloc> const & b, int tolerance = 32)
	{
		if (a.size() != b.size())
			return false; 

		auto it1 = a.begin(); 
		auto it2 = b.begin();
		auto const end = a.end(); 
		for ( ; it1 != end; ++it1, ++it2)
		{
			if (!Equals(*it1, *it2, tolerance))
				return false; 
		}
		return true; 
	}
#endif

	static HPS_INLINE uint32_t extract_sign_bit(float const & a) {
		return extract_uint32_t(a) & 0x80000000;
	}
	static HPS_INLINE uint32_t extract_sign_bit(double const & a) {
		uint32_t	v[2];
		memcpy (v, &a, sizeof(double));
		return v[High] & 0x80000000;
	}

	static HPS_INLINE void apply_sign_bit(float & a, uint32_t const & sign_bit) {
		uint32_t	v = extract_uint32_t(a);
		v &= 0x7FFFFFFF;
		v |= sign_bit;
		inject_uint32_t(a, v);
	}
	static HPS_INLINE void apply_sign_bit(double & a, uint32_t const & sign_bit) {
		uint32_t	v[2];
		memcpy (v, &a, sizeof(double));
		v[High] &= 0x7FFFFFFF;
		v[High] |= sign_bit;
		memcpy (&a, v, sizeof(double));
	}


	static HPS_INLINE unsigned char unit_to_byte(float const & a) {
		uint32_t	v = extract_uint32_t(a);

		v &= 0x7FFFFFFF;
		if (v < 0x3B800000)
			return 0;

		v--;

		uint32_t	exp = v >> 23;
		uint32_t	man = (v & 0x007FFFFF) | 0x00800000;

		return (unsigned char) (man >> (16 + 126 - exp));
	}

	static HPS_INLINE unsigned char unit_to_byte_scaled(float const & a, unsigned char mix) {
		uint32_t	v = extract_uint32_t(a);

		v &= 0x7FFFFFFF;
		if (v < 0x3B800000)
			return 0;

		v--;

		uint32_t	exp = v >> 23;
		uint32_t	man = (v & 0x007FFFFF) | 0x00800000;

		uint32_t	x = (man >> (16 + 126 - exp));

		return (unsigned char) ((x * (mix+1)) >> 8);
	}


	static HPS_INLINE bool match(float const & a, float const & b) {
		uint32_t va = extract_uint32_t(a);
		uint32_t vb = extract_uint32_t(b);

		if (((va | vb) & 0x7FFFFFFF) == 0)
			return true;

		return va == vb;
	}
	static HPS_INLINE bool match(double const & a, double const & b) {
		return a == b;
	}


	static HPS_INLINE void replace_if_smaller(float & a, float const & b) {
		uint32_t	va = extract_uint32_t(a);
		uint32_t	vb = extract_uint32_t(b);

		uint32_t	a_sign = va & 0x80000000;
		uint32_t	b_sign = vb & 0x80000000;

		(int32_t&)a_sign >>= 31;
		(int32_t&)b_sign >>= 31;

		uint32_t mash = (a_sign&b_sign);

		va ^= mash;
		vb ^= mash;

		if ((int32_t&)vb < (int32_t&)va)
			memcpy(&a, &b, sizeof(float));
	}
	static HPS_INLINE void replace_if_smaller(double & a, double const & b) {
		if (b < a)
			a = b;
	}

	static HPS_INLINE void replace_if_larger(float & a, float const & b) {
		uint32_t	va = extract_uint32_t(a);
		uint32_t	vb = extract_uint32_t(b);

		uint32_t	a_sign = va & 0x80000000;
		uint32_t	b_sign = vb & 0x80000000;

		(int32_t&)a_sign >>= 31;
		(int32_t&)b_sign >>= 31;

		uint32_t mash = (a_sign&b_sign);

		va ^= mash;
		vb ^= mash;

		if ((int32_t&)vb > (int32_t&)va)
			memcpy(&a, &b, sizeof(float));
	}
	static HPS_INLINE void replace_if_larger(double & a, double const & b) {
		if (b > a)
			a = b;
	}


	static HPS_INLINE uint32_t extract_uint32_t(float const & a) {
		uint32_t i;
		memcpy(&i, &a, sizeof(float));
		return i;
	}

	static HPS_INLINE void inject_uint32_t(float & a, uint32_t const & i) {
		memcpy(&a, &i, sizeof(float));
	}

	static HPS_INLINE float C2F(unsigned char x) {
		//return (float)x * (1.0f/255.0f);
		return char_to_float[x];
	}


	// SSE convenience functions
	static HPS_INLINE void pack_4(float const & f, float * m) {
		memcpy(&m[0], &f, sizeof(float));
		memcpy(&m[1], &f, sizeof(float));
		memcpy(&m[2], &f, sizeof(float));
		memcpy(&m[3], &f, sizeof(float));
	}

	static HPS_INLINE void pack_4(float const & f0, float const & f1, float const & f2, float const & f3, float * m) {
		memcpy(&m[0], &f0, sizeof(float));
		memcpy(&m[1], &f1, sizeof(float));
		memcpy(&m[2], &f2, sizeof(float));
		memcpy(&m[3], &f3, sizeof(float));
	}

	static HPS_INLINE void unpack_4(float * f0, float const * const m) {
		memcpy(f0, m, sizeof(float)*4);
	}

	static HPS_INLINE void unpack_4(float & f0, float & f1, float & f2, float & f3, float const * const m) {
		memcpy(&f0, &m[0], sizeof(float));
		memcpy(&f1, &m[1], sizeof(float));
		memcpy(&f2, &m[2], sizeof(float));
		memcpy(&f3, &m[3], sizeof(float));
	}

private:
	static const float char_to_float[256];

	Float();
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


template <typename T>	HPS_INLINE	T *	Alter (T const * a)	{ return const_cast<T *>(a); }
template <typename T>	HPS_INLINE	T &	Alter (T const & a)	{ return const_cast<T &>(a); }

template <typename T>	HPS_INLINE	T		Abs (T const & a)					{ return  a < 0 ? -a : a; }
template <typename T>	HPS_INLINE	int		Compare (T const & a, T const & b)	{ return a == b ? 0 : a < b ? -1 : 1;  }
template <typename T>	HPS_INLINE	int		Sign (T const & a)					{ return Compare(a,(T)0); }
template <typename T>	HPS_INLINE	void	Swap (T & a, T & b)	{ T temp = a; a = b; b = temp; }
template <typename T>	HPS_INLINE	int		Floor(T const & a) { return ((a > 0 || (T)(int)a == a) ? (int)a : ((int)a - 1)); }
template <typename T>	HPS_INLINE	int		Ceiling(T const & a) { return ((a < 0 || (T)(int)a == a) ? (int)a : ((int)a + 1)); }

template <typename T>	HPS_INLINE	T const &	Min (T const & a, T const & b)	{ return  a < b ? a : b; }
template <typename T>	HPS_INLINE	T const &	Min (T const & a, T const & b, T const & c)	{ return  Min(Min(a,b),c); }
template <typename T>	HPS_INLINE	T const &	Min (T const & a, T const & b, T const & c, T const & d) { return Min(Min(a, b, c),d); }
template <typename T>	HPS_INLINE	T const &	Min (T const & a, T const & b, T const & c, T const & d, T const & e) { return Min(Min(a,b,c,d),e); }
template <typename T>	HPS_INLINE	T const &	Min (T const & a, T const & b, T const & c, T const & d, T const & e, T const & f) { return Min(Min(a,b,c,d,e),f); }

template <typename T>	HPS_INLINE	T const &	Max (T const & a, T const & b)	{ return  a > b ? a : b; }
template <typename T>	HPS_INLINE	T const &	Max (T const & a, T const & b, T const & c)	{ return  Max(Max(a,b),c); }
template <typename T>	HPS_INLINE	T const &	Max (T const & a, T const & b, T const & c, T const & d) { return Max(Max(a, b, c),d); }
template <typename T>	HPS_INLINE	T const &	Max (T const & a, T const & b, T const & c, T const & d, T const & e) { return Max(Max(a,b,c,d),e); }
template <typename T>	HPS_INLINE	T const &	Max (T const & a, T const & b, T const & c, T const & d, T const & e, T const & f) { return Max(Max(a,b,c,d,e),f); }

template <typename T>	HPS_INLINE	T const &	Clamp(T const & x, T const & min, T const & max) { return x < min ? min : x > max ? max : x; }


template <> HPS_INLINE float	Abs<float> (float const & a) {
	uint32_t	v = Float::extract_uint32_t(a);
	v &= 0x7FFFFFFF;
	float f;
	Float::inject_uint32_t(f,v);
	return f;
}

template <> HPS_INLINE int	Sign<float> (float const & a) {
	uint32_t	v = Float::extract_uint32_t(a);

	if ((v & 0x7FFFFFFF) == 0)
		return 0;

	return ((int32_t)(v & 0x80000000)>>31) | 0x01;
}

template <> HPS_INLINE int	Compare<float> (float const & a, float const & b) {
	int		a_sign = Sign(a);
	int		b_sign = Sign(b);

	if (a_sign != b_sign)
		return Compare(a_sign, b_sign);

	uint32_t	v = Float::extract_uint32_t(a);
	uint32_t	u = Float::extract_uint32_t(b);

	return Compare((v & 0x7FFFFFFF), (u & 0x7FFFFFFF)) * a_sign;
}

template <> HPS_INLINE float const &	Min<float> (float const & a, float const & b) {
	if (Compare(a,b)>0)
		return b;
	return a;
}

template <> HPS_INLINE float const &	Max<float> (float const & a, float const & b) {
	if (Compare(a,b)>0)
		return a;
	return b;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

HPS_INLINE bool Float::Equals(float const & a, float const & b, int tolerance) {
	int32_t		va = Float::extract_uint32_t(a);
	int32_t		vb = Float::extract_uint32_t(b);

	if (is_special(va) || is_special(vb)) {
		if (is_infinite(va) || is_infinite(vb))
			return va == vb;				// final check is for sign bits same
		if (is_nan(va) || is_nan(vb))
			return false;
	}

	int const	close_to_zero = 0x36A00000;	// (approx) 5.0e-6f;
	if ((va & 0x7FFFFFFF) == 0)
		return (vb & 0x7FFFFFFF) < close_to_zero;
	else if ((vb & 0x7FFFFFFF) == 0)
		return (va & 0x7FFFFFFF) < close_to_zero;

	uint32_t	sign_mask = va ^ vb;
	(int32_t &)sign_mask >>= 31;

	int32_t		diff = ((va + sign_mask) ^ (sign_mask & 0x7FFFFFFF)) -vb; 
	int32_t		v1 = tolerance + diff;
	int32_t		v2 = tolerance - diff;
	return (v1|v2) >= 0;
}

HPS_INLINE bool Float::Equals(double const & a, double const & b, int tolerance) {
	int32_t		va[2], vb[2];
	memcpy (va, &a, sizeof(double));
	memcpy (vb, &b, sizeof(double));

	if (is_special(va) || is_special(vb)) {
		if (is_infinite(va) || is_infinite(vb))
			return va[High] == vb[High] && va[Low] == vb[Low];	// final check is for sign bits same
		if (is_nan(va) || is_nan(vb))
			return false;
	}

	if ((va[High] == 0 && va[Low] == 0) || (vb[High] == 0 && vb[Low] == 0))
		return Abs(a - b) < 0.000000000000005;

	if (extract_sign_bit(a) != extract_sign_bit(b))
		return a == b;

	if (va[High] != vb[High])
		return false;

	return Abs(va[Low] - vb[Low]) <= tolerance;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


HPS_INLINE bool Is_Abnormal (float const & f)	{ return Float::IsAbnormal (f); }
HPS_INLINE bool Is_Abnormal (double const & d)	{ return Float::IsAbnormal (d); }

template <typename T>
HPS_INLINE bool Is_Abnormal (size_t count, T const * t) {
	while (count-- > 0)
		if (Is_Abnormal (*t++))
			return true;
	return false;
}

template <typename T>
HPS_INLINE bool Is_Abnormal (int count, T const * t) {
	return count >= 0 && Is_Abnormal((size_t)count, t);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


template <typename F> class HPS_TEMPLATE_API Vector_3D;
template <typename F> class HPS_TEMPLATE_API Plane_3D;
template <typename F> class HPS_TEMPLATE_API Vector_2D;
template <typename F> class HPS_TEMPLATE_API Point_2D;


template <typename F>
class HPS_TEMPLATE_API Point_3D {
	public:
		F	x;
		F	y;
		F	z;

		Point_3D () {}
		Point_3D (F v1, F v2, F v3) : x (v1), y (v2), z (v3) {}

		template <typename D>
		explicit Point_3D (Point_3D<D> const & that) : x ((F)that.x), y ((F)that.y), z ((F)that.z) {}

		explicit Point_3D (Vector_3D<F> const & v);
		explicit Point_3D (Vector_2D<F> const & v);
		explicit Point_3D (Point_2D<F> const & that);

		Point_3D const	operator- () const	{ return Point_3D (-x, -y, -z); }

		bool				operator== (Point_3D const & p) const	{ return  x == p.x && y == p.y && z == p.z; }
		bool				operator!= (Point_3D const & p) const	{ return  !(*this == p); }

		bool				Equals(Point_3D const & p, int in_tolerance = 32) const {
								return	Float::Equals(x, p.x, in_tolerance) &&
										Float::Equals(y, p.y, in_tolerance) &&
										Float::Equals(z, p.z, in_tolerance);
							}


		Point_3D &			operator*= (F s)		{ x *= s; y *= s; z *= s;  return *this; }
		Point_3D &			operator/= (F s)		{ return operator*= ((F)1 / s); }
		Point_3D const		operator* (F s) const	{ return Point_3D (x * s, y * s, z * s); }
		Point_3D const		operator/ (F s) const	{ return operator* ((F)1 / s); }

		F &					operator[] (size_t i)		{ return (&x)[i]; }
		F const &			operator[] (size_t i) const	{ return (&x)[i]; }

		Point_3D &			operator+= (Vector_3D<F> const & v);
		Point_3D &			operator-= (Vector_3D<F> const & v);
		Point_3D &			operator*= (Vector_3D<F> const & v);
		Point_3D &			operator/= (Vector_3D<F> const & v);
		Point_3D const		operator* (Vector_3D<F> const & v) const;
		Point_3D const		operator/ (Vector_3D<F> const & v) const;

		Point_3D &			operator+= (Vector_2D<F> const & v);
		Point_3D &			operator-= (Vector_2D<F> const & v);
		Point_3D &			operator*= (Vector_2D<F> const & v);
		Point_3D &			operator/= (Vector_2D<F> const & v);
		Point_3D const		operator* (Vector_2D<F> const & v) const;
		Point_3D const		operator/ (Vector_2D<F> const & v) const;

		Vector_3D<F> const	operator- (Point_3D const & p) const;

		Point_3D const		operator+ (Vector_3D<F> const & v) const;
		Point_3D const		operator- (Vector_3D<F> const & v) const;

		Point_3D const		operator+ (Vector_2D<F> const & v) const;
		Point_3D const		operator- (Vector_2D<F> const & v) const;

		static HPS_INLINE Point_3D	Origin() {return Point_3D (0, 0, 0);};
		static HPS_INLINE Point_3D	Zero() {return Point_3D (0, 0, 0);}; //-V524
};

typedef	Point_3D<float>		Point;
typedef	Point_3D<double>	DPoint;



template <typename F, typename S>
HPS_INLINE Point_3D<F>	operator* (S s, Point_3D<F> const & a) { return Point_3D<F> (F(s * a.x), F(s * a.y), F(s * a.z)); }

template <typename F>
HPS_INLINE Point_3D<F> Midpoint (Point_3D<F> const & a, Point_3D<F> const & b) {
	return Point_3D<F> (a.x + b.x, a.y + b.y, a.z + b.z) * 0.5f;
}

template <typename F>
HPS_INLINE Point_3D<F> Midpoint (Point_3D<F> const & a, Point_3D<F> const & b, Point_3D<F> const & c) {
	return Point_3D<F> (a.x + b.x + c.x, a.y + b.y + c.y, a.z + b.z + c.z) * (F)(1.0/3.0);
}

template <typename F>
HPS_INLINE bool Is_Abnormal (Point_3D<F> const & p) {
	return Is_Abnormal (p.x) || Is_Abnormal (p.y) || Is_Abnormal (p.z);
}


template <typename F>
class HPS_TEMPLATE_API Point_2D {
public:
	F	x;
	F	y;

	Point_2D () {}
	Point_2D (F v1, F v2) : x (v1), y (v2) {}

	template <typename D>
	explicit Point_2D (Point_2D<D> const & that) : x ((F)that.x), y ((F)that.y) {}

	explicit Point_2D (Point_3D<F> const & that) : x ((F)that.x), y ((F)that.y) {}
	explicit Point_2D (Vector_2D<F> const & v);

	Point_2D const	operator- () const	{ return Point_2D (-x, -y); }

	bool				operator== (Point_2D const & p) const	{ return  x == p.x && y == p.y; }
	bool				operator!= (Point_2D const & p) const	{ return  !(*this == p); }

	bool				Equals(Point_2D const & p, int in_tolerance = 32) const {
		return Float::Equals(x, p.x, in_tolerance) && Float::Equals(y, p.y, in_tolerance);
	}


	Point_2D &			operator*= (F s)		{ x *= s; y *= s; return *this; }
	Point_2D &			operator/= (F s)		{ return operator*= ((F)1 / s); }
	Point_2D const		operator* (F s) const	{ return Point_2D (x * s, y * s); }
	Point_2D const		operator/ (F s) const	{ return operator* ((F)1 / s); }

	F &					operator[] (size_t i)		{ return (&x)[i]; }
	F const &			operator[] (size_t i) const	{ return (&x)[i]; }

	Point_2D &			operator+= (Vector_2D<F> const & v);
	Point_2D &			operator-= (Vector_2D<F> const & v);
	Point_2D &			operator*= (Vector_2D<F> const & v);
	Point_2D &			operator/= (Vector_2D<F> const & v);
	Point_2D const		operator* (Vector_2D<F> const & v) const;
	Point_2D const		operator/ (Vector_2D<F> const & v) const;

	Vector_2D<F> const	operator- (Point_2D const & p) const;

	Point_2D const		operator+ (Vector_2D<F> const & v) const;
	Point_2D const		operator- (Vector_2D<F> const & v) const;

	static HPS_INLINE Point_2D	Origin() {return Point_2D (0, 0);};
	static HPS_INLINE Point_2D	Zero() {return Point_2D (0, 0);}; //-V524
};

typedef	Point_2D<float>		Point2D;
typedef	Point_2D<double>	DPoint2D;

template <typename F>
HPS_INLINE Point_3D<F>::Point_3D (Point_2D<F> const & that) : x (that.x), y (that.y), z (0) {}

template <typename F, typename S>
HPS_INLINE Point_2D<F>	operator* (S s, Point_2D<F> const & a) { return Point_2D<F> (F(s * a.x), F(s * a.y)); }

template <typename F>
HPS_INLINE Point_2D<F> Midpoint (Point_2D<F> const & a, Point_2D<F> const & b) {
	return Point_2D<F> (a.x + b.x, a.y + b.y) * 0.5f;
}

template <typename F>
HPS_INLINE Point_2D<F> Midpoint (Point_2D<F> const & a, Point_2D<F> const & b, Point_2D<F> const & c) {
	return Point_2D<F> (a.x + b.x + c.x, a.y + b.y + c.y, a.z + b.z + c.z) * (F)(1.0/3.0);
}

template <typename F>
HPS_INLINE bool Is_Abnormal (Point_2D<F> const & p) {
	return Is_Abnormal (p.x) || Is_Abnormal (p.y);
}


template <typename F>
class HPS_TEMPLATE_API Vector_3D {
	public:
		F	x;
		F	y;
		F	z;

		Vector_3D () {}
		Vector_3D (F v1, F v2, F v3) : x (v1), y (v2), z (v3) {}
		template <typename D>
			explicit Vector_3D (Vector_3D<D> const & that) : x ((F)that.x), y ((F)that.y), z ((F)that.z) {}
		explicit Vector_3D(Point_3D<F> const & p) : x(p.x), y(p.y), z(p.z) {}
		explicit Vector_3D(Plane_3D<F> const & p);

		explicit Vector_3D (Vector_2D<F> const & that);

		Vector_3D const	operator- () const	{ return Vector_3D (-x, -y, -z); }

		bool				operator== (Vector_3D const & v) const		{
								return  Float::match(x, v.x) && Float::match(y, v.y) && Float::match(z, v.z);
							}
		bool				operator!= (Vector_3D const & v) const		{ return  !(*this == v); }

		bool				Equals(Vector_3D const & v, int in_tolerance = 32) const {
								return	Float::Equals(x, v.x, in_tolerance) &&
										Float::Equals(y, v.y, in_tolerance) &&
										Float::Equals(z, v.z, in_tolerance);
							}

		Vector_3D &			operator+= (Vector_3D const & v)		{ x += v.x; y += v.y; z += v.z;  return *this; }
		Vector_3D &			operator-= (Vector_3D const & v)		{ x -= v.x; y -= v.y; z -= v.z;  return *this; }
		Vector_3D const		operator+ (Vector_3D const & v) const	{ return Vector_3D (x + v.x, y + v.y, z + v.z); }
		Vector_3D const		operator- (Vector_3D const & v) const	{ return Vector_3D (x - v.x, y - v.y, z - v.z); }

		Vector_3D &			operator*= (F s)				{ x *= s; y *= s; z *= s;  return *this; }
		Vector_3D &			operator/= (F s)				{ return operator*= (1.0f / s); }
		Vector_3D const		operator* (F s) const			{ return Vector_3D (x * s, y * s, z * s); }
		Vector_3D const		operator/ (F s) const			{ return operator* (1.0f / s); }

		F &					operator[] (size_t i)		{ return (&x)[i]; }
		F const &			operator[] (size_t i) const	{ return (&x)[i]; }

		HPS_INLINE double	Length () const { return sqrt (LengthSquared()); }

		HPS_INLINE double	LengthSquared () const { return (double)x*(double)x + (double)y*(double)y + (double)z*(double)z; }

		HPS_INLINE double	Length2D () const { return sqrt (LengthSquared2D()); }

		HPS_INLINE double	LengthSquared2D () const { return (double)x*(double)x + (double)y*(double)y;}

		HPS_INLINE Vector_3D &	Normalize (bool check_range = false, F epsilon = Float_Traits<F>::Epsilon()) {// not const &; allow V.normalize() *= S;
			if (check_range) {
				F	range = Max (Abs (x), Abs (y), Abs (z));
				if (range > F(1.0e10))
					operator/= (range);
			}

			F	len = (F)Length();
			if (len > epsilon)
				operator/= (len);
			else
				*this = Zero();
			return *this;
		}
		HPS_INLINE Vector_3D &	Normalize (F epsilon) { return Normalize (false, epsilon); }

		HPS_INLINE F		Magnitude () const { return Max (Abs(x), Abs(y), Abs(z)); }
		HPS_INLINE F		Manhattan () const { return Abs(x)+Abs(y)+Abs(z); }

		HPS_INLINE F		Dot (Vector_3D const & v) const { return x * v.x  +  y * v.y  +  z * v.z; }

		HPS_INLINE Vector_3D	Cross (Vector_3D const & v) const {
									return Vector_3D (y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
								}


		HPS_INLINE Vector_3D	Scale (Vector_3D const & v) const {
									return Vector_3D (x * v.x, y * v.y, z * v.z);
								}

		static HPS_INLINE Vector_3D	XAxis()	{return Vector_3D (1, 0, 0);};
		static HPS_INLINE Vector_3D	YAxis()	{return Vector_3D (0, 1, 0);};
		static HPS_INLINE Vector_3D	ZAxis()	{return Vector_3D (0, 0, 1);};
		static HPS_INLINE Vector_3D	Zero()	{return Vector_3D (0, 0, 0);};
		static HPS_INLINE Vector_3D	Unit()	{return Vector_3D (1, 1, 1);};
};

typedef Vector_3D<float>	Vector;
typedef Vector_3D<double>	DVector;


template <typename F, typename S>
HPS_INLINE	Vector_3D<F>	operator* (S s, Vector_3D<F> const & v) {return Vector_3D<F> (F(s * v.x), F(s * v.y), F(s * v.z)); }

template <typename F>
HPS_INLINE bool Is_Abnormal (Vector_3D<F> const & v) {
	return Is_Abnormal (v.x) || Is_Abnormal (v.y) || Is_Abnormal (v.z);
}


template <typename F>
HPS_INLINE	Point_3D<F>::Point_3D(Vector_3D<F> const & v) : x(v.x), y(v.y), z(v.z) {}

template <typename F>
HPS_INLINE	Point_3D<F> &		Point_3D<F>::operator+= (Vector_3D<F> const & v)	{ x += v.x; y += v.y; z += v.z;  return *this; }
template <typename F>
HPS_INLINE	Point_3D<F> &		Point_3D<F>::operator-= (Vector_3D<F> const & v)	{ x -= v.x; y -= v.y; z -= v.z;  return *this; }

template <typename F>
HPS_INLINE	Vector_3D<F> const	Point_3D<F>::operator- (Point_3D<F> const & p) const	{ return Vector_3D<F> (x - p.x, y - p.y, z - p.z); }

template <typename F>
HPS_INLINE	Point_3D<F> const	Point_3D<F>::operator+ (Vector_3D<F> const & v) const	{ return Point_3D<F>  (x + v.x, y + v.y, z + v.z); }
template <typename F>
HPS_INLINE	Point_3D<F> const	Point_3D<F>::operator- (Vector_3D<F> const & v) const	{ return Point_3D<F>  (x - v.x, y - v.y, z - v.z); }

template <typename F>
HPS_INLINE	Point_3D<F> &		Point_3D<F>::operator*= (Vector_3D<F> const & v) { x *= v.x; y *= v.y; z *= v.z;  return *this; }
template <typename F>
HPS_INLINE	Point_3D<F> &		Point_3D<F>::operator/= (Vector_3D<F> const & v) { x /= v.x; y /= v.y; z /= v.z;  return *this; }
template <typename F>
HPS_INLINE	Point_3D<F> const	Point_3D<F>::operator* (Vector_3D<F> const & v) const { return Point_3D<F> (x * v.x, y * v.y, z * v.z); }
template <typename F>
HPS_INLINE	Point_3D<F> const	Point_3D<F>::operator/ (Vector_3D<F> const & v) const { return Point_3D<F> (x / v.x, y / v.y, z / v.z); }



template <typename F>
HPS_INLINE Point_3D<F> Interpolate(Point_3D<F> const & a, Point_3D<F> const & b, float t) {
	return a + (b - a) * t;
}

template <typename F>
HPS_INLINE Vector_3D<F> Interpolate(Vector_3D<F> const & a, Vector_3D<F> const & b, float t) {
	return Vector_3D<F>(a + (b - a) * t).Normalize();
}



template <typename F>
HPS_INLINE	double PointToPointDistance(Point_3D<F> const & p1, Point_3D<F> const & p2) {
	return (p2 - p1).Length();
}

template <typename F>
HPS_INLINE	double PointToPointDistanceSquared(Point_3D<F> const & p1, Point_3D<F> const & p2) {
	return (p2 - p1).LengthSquared();
}

template <typename F>
HPS_INLINE Point_3D<F> Circumcenter(Point_3D<F> const & a, Point_3D<F> const & b, Point_3D<F> const & c) {
	F	p = static_cast<F>((c - b).LengthSquared());
	F	q = static_cast<F>((c - a).LengthSquared());
	F	r = static_cast<F>((b - a).LengthSquared());

	return Point_3D<F>(
		(a * (p*(q+r-p)) + (Vector_3D<F>)b * (q*(r+p-q)) + (Vector_3D<F>)c * (r*(p+q-r)))
			/ (2 * (p*q + p*r + q*r) - (p*p + q*q + r*r)) );
}



template <typename F>
HPS_INLINE bool Normalize(size_t count, Vector_3D<F> * vectors) {
	bool success = true;
	for (size_t i = 0; i < count; ++i) {
		if (vectors->Normalize() == Vector_3D<F>::Zero())
			success = false;
		vectors++;
	}
	return success;
}


template <typename F> class HPS_TEMPLATE_API Plane_2D;

template <typename F>
class HPS_TEMPLATE_API Vector_2D {
public:
	F	x;
	F	y;

	Vector_2D () {}
	Vector_2D (F v1, F v2) : x (v1), y (v2) {}
	template <typename D>
	explicit Vector_2D (Vector_2D<D> const & that) : x ((F)that.x), y ((F)that.y) {}

	explicit Vector_2D (Vector_3D<F> const & that) : x (that.x), y (that.y) {}
	explicit Vector_2D (Point_2D<F> const & p) : x(p.x), y(p.y) {}
	explicit Vector_2D (Plane_2D<F> const & p);

	Vector_2D const	operator- () const	{ return Vector_2D (-x, -y); }

	bool				operator== (Vector_2D const & v) const		{
		return  Float::match(x, v.x) && Float::match(y, v.y);
	}
	bool				operator!= (Vector_2D const & v) const		{ return  !(*this == v); }

	bool				Equals(Vector_2D const & v, int in_tolerance = 32) const {
		return	Float::Equals(x, v.x, in_tolerance) && Float::Equals(y, v.y, in_tolerance);
	}

	Vector_2D &			operator+= (Vector_2D const & v)		{ x += v.x; y += v.y; return *this; }
	Vector_2D &			operator-= (Vector_2D const & v)		{ x -= v.x; y -= v.y; return *this; }
	Vector_2D const		operator+ (Vector_2D const & v) const	{ return Vector_2D (x + v.x, y + v.y); }
	Vector_2D const		operator- (Vector_2D const & v) const	{ return Vector_2D (x - v.x, y - v.y); }

	Vector_2D &			operator*= (F s)				{ x *= s; y *= s; return *this; }
	Vector_2D &			operator/= (F s)				{ return operator*= (1.0f / s); }
	Vector_2D const		operator* (F s) const			{ return Vector_2D (x * s, y * s); }
	Vector_2D const		operator/ (F s) const			{ return operator* (1.0f / s); }

	F &					operator[] (size_t i)		{ return (&x)[i]; }
	F const &			operator[] (size_t i) const	{ return (&x)[i]; }

	HPS_INLINE double	Length () const { return sqrt (LengthSquared()); }

	HPS_INLINE double	LengthSquared () const { return (double)x*(double)x + (double)y*(double)y; }

	HPS_INLINE Vector_2D &	Normalize (bool check_range = false, F epsilon = Float_Traits<F>::Epsilon()) {// not const &; allow V.normalize() *= S;
		if (check_range) {
			F	range = Max (Abs (x), Abs (y));
			if (range > F(1.0e10))
				operator/= (range);
		}

		F	len = (F)Length();
		if (len > epsilon)
			operator/= (len);
		else
			*this = Zero();
		return *this;
	}
	HPS_INLINE Vector_2D &	Normalize (F epsilon) { return Normalize (false, epsilon); }

	HPS_INLINE F		Magnitude () const { return Max (Abs(x), Abs(y)); }
	HPS_INLINE F		Manhattan () const { return Abs(x)+Abs(y); }

	HPS_INLINE F		Dot (Vector_2D const & v) const { return x * v.x  +  y * v.y; }


	HPS_INLINE float	Cross (Vector_2D const & v) const {
		return x * v.y - y * v.x;
	}

	HPS_INLINE Vector_2D	Scale (Vector_2D const & v) const {
		return Vector_2D (x * v.x, y * v.y);
	}

	static HPS_INLINE Vector_2D	XAxis()	{return Vector_2D (1, 0);};
	static HPS_INLINE Vector_2D	YAxis()	{return Vector_2D (0, 1);};

	static HPS_INLINE Vector_2D	Zero()	{return Vector_2D (0, 0);};
	static HPS_INLINE Vector_2D	Unit()	{return Vector_2D (1, 1);};
};

typedef Vector_2D<float>	Vector2D;
typedef Vector_2D<double>	DVector2D;

template <typename F, typename S>
HPS_INLINE	Vector_2D<F>	operator* (S s, Vector_2D<F> const & v) {return Vector_2D<F> (F(s * v.x), F(s * v.y)); }

template <typename F>
HPS_INLINE bool Is_Abnormal (Vector_2D<F> const & v) {		
	return Is_Abnormal (v.x) || Is_Abnormal (v.y);
}


template <typename F>
HPS_INLINE	Vector_3D<F>::Vector_3D (Vector_2D<F> const & that) : x (that.x), y (that.y), z(0) {}

template <typename F>
HPS_INLINE	Point_3D<F>::Point_3D(Vector_2D<F> const & v) : x(v.x), y(v.y), z(0) {}

template <typename F>
HPS_INLINE	Point_3D<F> &		Point_3D<F>::operator+= (Vector_2D<F> const & v)	{ x += v.x; y += v.y; return *this; }
template <typename F>
HPS_INLINE	Point_3D<F> &		Point_3D<F>::operator-= (Vector_2D<F> const & v)	{ x -= v.x; y -= v.y; return *this; }

template <typename F>
HPS_INLINE	Point_3D<F> const	Point_3D<F>::operator+ (Vector_2D<F> const & v) const	{ return Point_3D<F>  (x + v.x, y + v.y, z); }
template <typename F>
HPS_INLINE	Point_3D<F> const	Point_3D<F>::operator- (Vector_2D<F> const & v) const	{ return Point_3D<F>  (x - v.x, y - v.y, z); }

template <typename F>
HPS_INLINE	Point_3D<F> &		Point_3D<F>::operator*= (Vector_2D<F> const & v) { x *= v.x; y *= v.y; return *this; }
template <typename F>
HPS_INLINE	Point_3D<F> &		Point_3D<F>::operator/= (Vector_2D<F> const & v) { x /= v.x; y /= v.y; return *this; }

template <typename F>
HPS_INLINE	Point_3D<F> const	Point_3D<F>::operator* (Vector_2D<F> const & v) const { return Point_3D<F> (x * v.x, y * v.y, z); }
template <typename F>
HPS_INLINE	Point_3D<F> const	Point_3D<F>::operator/ (Vector_2D<F> const & v) const { return Point_3D<F> (x / v.x, y / v.y, z); }


template <typename F>
HPS_INLINE	Point_2D<F> &		Point_2D<F>::operator+= (Vector_2D<F> const & v)	{ x += v.x; y += v.y; return *this; }
template <typename F>
HPS_INLINE	Point_2D<F> &		Point_2D<F>::operator-= (Vector_2D<F> const & v)	{ x -= v.x; y -= v.y; return *this; }

template <typename F>
HPS_INLINE	Vector_2D<F> const	Point_2D<F>::operator- (Point_2D<F> const & p) const	{ return Vector_2D<F> (x - p.x, y - p.y); }

template <typename F>
HPS_INLINE	Point_2D<F> const	Point_2D<F>::operator+ (Vector_2D<F> const & v) const	{ return Point_2D<F>  (x + v.x, y + v.y); }
template <typename F>
HPS_INLINE	Point_2D<F> const	Point_2D<F>::operator- (Vector_2D<F> const & v) const	{ return Point_2D<F>  (x - v.x, y - v.y); }

template <typename F>
HPS_INLINE	Point_2D<F> &		Point_2D<F>::operator*= (Vector_2D<F> const & v) { x *= v.x; y *= v.y; return *this; }
template <typename F>
HPS_INLINE	Point_2D<F> &		Point_2D<F>::operator/= (Vector_2D<F> const & v) { x /= v.x; y /= v.y; return *this; }
template <typename F>
HPS_INLINE	Point_2D<F> const	Point_2D<F>::operator* (Vector_2D<F> const & v) const { return Point_2D<F> (x * v.x, y * v.y); }
template <typename F>
HPS_INLINE	Point_2D<F> const	Point_2D<F>::operator/ (Vector_2D<F> const & v) const { return Point_2D<F> (x / v.x, y / v.y); }





template <typename F>
class HPS_TEMPLATE_API Plane_3D {
	public:
		F	a;
		F	b;
		F	c;
		F	d;

		Plane_3D () {}
		Plane_3D (F v1, F v2, F v3, F v4) : a (v1), b (v2), c (v3), d (v4) {}
		Plane_3D (Vector_3D<F> const & v, F f = 0) : a (v.x), b (v.y), c (v.z), d (f) {}
		Plane_3D (Vector_3D<F> const & v, Point_3D<F> const & p) : a (v.x), b (v.y), c (v.z), d (-(p.x * v.x + p.y * v.y + p.z * v.z)) {}
		Plane_3D (Point_3D<F> const & p, Vector_3D<F> const & v) : a (v.x), b (v.y), c (v.z), d (-(p.x * v.x + p.y * v.y + p.z * v.z)) {}
		template <typename D>
			explicit Plane_3D (Plane_3D<D> const & that) : a ((F)that.a), b ((F)that.b), c ((F)that.c), d ((F)that.d) {}

		Plane_3D (size_t count, Point_3D<F> const * points) {
			if (count >= 3) {
				// The 3 coefficients A, B, and C are proportional to the areas of the
				// projections of the polygon onto the yz, zx, and xy planes, respectively.

				// run around the polygon, collecting trapezoidal areas
				// a "center" point is also collected, to make the plane 'd' slightly more "valid" when the polygon is non-planar.

				// take care of the swing point first
				Point_3D<F> const * p0 = &points[count-1];

				Point_3D<double>	ctr = Point_3D<double>::Origin();
				Vector_3D<F>		normal = Vector_3D<F>::Zero();

				for (size_t i=0; i<count; ++i) {
					Point_3D<F> const * p1 = &points[i];

					normal.x += (p0->y + p1->y) * (p1->z - p0->z);
					normal.y += (p0->z + p1->z) * (p1->x - p0->x);
					normal.z += (p0->x + p1->x) * (p1->y - p0->y);

					ctr += Vector_3D<double>(Vector_3D<F>(points[i]));

					p0 = p1;
				}

				// ("should" always be != 0)
				if (normal.Normalize() != Vector_3D<F>::Zero()) {
					/* finish finding the average */
					double	inv_count = 1.0 / (double)count;
					ctr *= inv_count;

					*this = Plane_3D(normal, Point_3D<F>(ctr));
					return;
				}
			}

			*this = Plane_3D::Zero();
		}


		Plane_3D const		operator- () const							{ return Plane_3D (-a, -b, -c, -d); }

		bool				operator== (Plane_3D const & p) const		{ return  a == p.a && b == p.b && c == p.c && d == p.d; }
		bool				operator!= (Plane_3D const & p) const		{ return  !(*this == p); }

		F &					operator[] (size_t i)		{ return (&a)[i]; }
		F const &			operator[] (size_t i) const	{ return (&a)[i]; }

		HPS_INLINE bool	Equals(Plane_3D const & p, int in_tolerance = 32) const { 
			return  Float::Equals(a, p.a, in_tolerance) && Float::Equals(b, p.b, in_tolerance) && 
					Float::Equals(c, p.c, in_tolerance) && Float::Equals(d, p.d, in_tolerance);
		}

		Plane_3D &		Normalize (F epsilon = Float_Traits<F>::Epsilon()) {	// not const &; allow V.normalize() *= S;
			F len = (F)Vector_3D<F>(*this).Length();
			if (len > epsilon)
				operator/= (len);
			else
				*this = Zero();
			return *this;
		}

		Point_3D<F>		IntersectLineSegment(Point_3D<F> const & p1, Point_3D<F> const & p2, float eps = 1e-5f) const {
			F	val1 = Abs (a * p1.x + b * p1.y + c * p1.z + d);
			F	val2 = Abs (a * p2.x + b * p2.y + c * p2.z + d);

			if (val1 >= eps) {
				return Point_3D<F> (((val1 * p2.x) + (val2 * p1.x)) / (val1 + val2),
									((val1 * p2.y) + (val2 * p1.y)) / (val1 + val2),
									((val1 * p2.z) + (val2 * p1.z)) / (val1 + val2));
			}
			else
				return p1;
		}

		Point_3D<F>		IntersectLineSegment2(Point_3D<F> const & p1, Point_3D<F> const & p2) const {
			F		 u = (a * p1.x + b * p1.y + c * p1.z + d) /
						(a * (p1.x - p2.x) + b * (p1.y - p2.y) + c * (p1.z - p2.z));

			return Point_3D<F>(p1.x + u * (p2.x - p1.x), p1.y + u * (p2.y - p1.y), p1.z + u * (p2.z - p1.z));
		}

		static HPS_INLINE Plane_3D Zero() {return Plane_3D (0.0f, 0.0f, 0.0f, 0.0f);};


	private:
		Plane_3D &		operator*= (F s)				{ a *= s; b *= s; c *= s; d *= s; return *this; }
		Plane_3D &		operator/= (F s)				{ return operator*= ((F)1.0 / s); }
		Plane_3D const	operator* (F s) const			{ return Plane_3D (a * s, b * s, c * s, d * s); }
		Plane_3D const	operator/ (F s) const			{ return operator* ((F)1.0 / s); }
};

typedef Plane_3D<float>		Plane;
typedef Plane_3D<double>	DPlane;


template <typename F>
HPS_INLINE bool Is_Abnormal (Plane_3D<F> const & p) {
	return Is_Abnormal (p.a) || Is_Abnormal (p.b) || Is_Abnormal (p.c) || Is_Abnormal (p.d);
}


template <typename F>
HPS_INLINE	F operator* (Plane_3D<F> const & plane, Point_3D<F> const & point) {
	return plane.a * point.x  +  plane.b * point.y  +  plane.c * point.z  +  plane.d;
}
template <typename F>
HPS_INLINE	F operator* (Point_3D<F> const & point, Plane_3D<F> const & plane) {
	return plane * point;
}

template <typename F>
HPS_INLINE Plane_3D<F> Interpolate(Plane_3D<F> const & a, Plane_3D<F> const & b, float t) {
	return Plane_3D<F>(a.a + (b.a - a.a) * t, a.b + (b.b - a.b) * t, a.c + (b.c - a.c) * t, a.d + (b.d - a.d) * t);
}

template <typename F>
Vector_3D<F>::Vector_3D(Plane_3D<F> const & p) : x(p.a), y(p.b), z(p.c) {}






template <typename F>
class HPS_TEMPLATE_API Plane_2D {
public:
	F	a;
	F	b;
	F	c;

	Plane_2D () {}
	Plane_2D (F v1, F v2, F v3) : a (v1), b (v2), c (v3) {}
	Plane_2D (Vector_2D<F> const & v, F f = 0) : a (v.x), b (v.y), c (f) {}
	Plane_2D (Vector_2D<F> const & v, Point_2D<F> const & p) : a (v.x), b (v.y), c (-(p.x * v.x + p.y * v.y)) {}
	Plane_2D (Point_2D<F> const & p, Vector_2D<F> const & v) : a (v.x), b (v.y), c (-(p.x * v.x + p.y * v.y)) {}
	template <typename D>
	explicit Plane_2D (Plane_2D<D> const & that) : a ((F)that.a), b ((F)that.b), c ((F)that.c) {}

	Plane_2D const		operator- () const							{ return Plane_2D (-a, -b, -c); }

	bool				operator== (Plane_2D const & p) const		{ return  a == p.a && b == p.b && c == p.c; }
	bool				operator!= (Plane_2D const & p) const		{ return  !(*this == p); }

	F &					operator[] (size_t i)		{ return (&a)[i]; }
	F const &			operator[] (size_t i) const	{ return (&a)[i]; }

	HPS_INLINE bool	Equals(Plane_2D const & p, int in_tolerance = 32) const { 
		return  Float::Equals(a, p.a, in_tolerance) && Float::Equals(b, p.b, in_tolerance) && Float::Equals(c, p.c, in_tolerance); 
	}

	Plane_2D &		Normalize (F epsilon = Float_Traits<F>::Epsilon()) {	// not const &; allow V.Normalize() *= S;
		F len = (F)Vector_2D<F>(*this).Length();
		if (len > epsilon)
			operator/= (len);
		else
			*this = Zero();
		return *this;
	}

	static HPS_INLINE Plane_2D Zero() {return Plane_2D (0.0f, 0.0f, 0.0f);};


private:
	Plane_2D &		operator*= (F s)				{ a *= s; b *= s; c *= s; return *this; }
	Plane_2D &		operator/= (F s)				{ return operator*= ((F)1.0 / s); }
	Plane_2D const	operator* (F s) const			{ return Plane_2D (a * s, b * s, c * s); }
	Plane_2D const	operator/ (F s) const			{ return operator* ((F)1.0 / s); }
};

typedef Plane_2D<float>		Plane2D;
typedef Plane_2D<double>	DPlane2D;


template <typename F>
HPS_INLINE bool Is_Abnormal (Plane_2D<F> const & p) {
	return Is_Abnormal (p.a) || Is_Abnormal (p.b) || Is_Abnormal (p.c);
}


template <typename F>
HPS_INLINE	F operator* (Plane_2D<F> const & plane, Point_2D<F> const & point) { 
	return plane.a * point.x + plane.b * point.y + plane.c;
}
template <typename F>
HPS_INLINE	F operator* (Point_3D<F> const & point, Plane_2D<F> const & plane) {
	return plane * point;
}

template <typename F>
HPS_INLINE Plane_2D<F> Interpolate(Plane_2D<F> const & a, Plane_2D<F> const & b, float t) {
	return Plane_2D<F>(a.a + (b.a - a.a) * t, a.b + (b.b - a.b) * t, a.c + (b.c - a.c) * t);
}

template <typename F>
Vector_2D<F>::Vector_2D(Plane_2D<F> const & p) : x(p.a), y(p.b) {}




struct Rectangle;

struct HPS_API IntRectangle {
	int	left;
	int	right;
	int	bottom;
	int	top;

	IntRectangle()
		: left(std::numeric_limits<int>::max()), right(std::numeric_limits<int>::min()),
		  bottom(std::numeric_limits<int>::max()), top(std::numeric_limits<int>::min()) {}

	IntRectangle(int in_left, int in_right, int in_bottom, int in_top)
		: left(in_left), right(in_right), bottom(in_bottom), top(in_top) {}

	IntRectangle(IntRectangle const & that)
		: left(that.left), right(that.right), bottom(that.bottom), top(that.top) {}

	explicit IntRectangle(Rectangle const & that);

	bool operator==(IntRectangle const & rect) const {
		return  (left == rect.left && right == rect.right && bottom == rect.bottom && top == rect.top);
	}

	bool operator!=(IntRectangle const & rect) const {
		return  !(*this == rect);
	}

	HPS_INLINE int PixelWidth() const {
		return right - left + 1;
	}

	HPS_INLINE int PixelHeight() const {
		return top - bottom + 1;
	}

	HPS_INLINE int Width() const {
		return right - left;
	}

	HPS_INLINE int Height() const {
		return top - bottom;
	}

	HPS_INLINE int Area() const {
		return Width() * Height();
	}

	HPS_INLINE Point2D Center() const {
		return Point2D((float)(left + right) * 0.5f, (float)(bottom + top) * 0.5f);
	}

	HPS_INLINE bool Intersecting(IntRectangle const & rect) const {
		return	right >= rect.left && left <= rect.right && top >= rect.bottom && bottom <= rect.top;
	}

	HPS_INLINE bool Contains(IntRectangle const & rect) const {
		return (left <= rect.left && right >= rect.right && bottom <= rect.bottom && top >= rect.top);
	}

	HPS_INLINE IntRectangle & Expand(int border) {
		left -= border;
		right += border;
		bottom -= border;
		top += border;
		return *this;
	}

	HPS_INLINE IntRectangle & Expand(IntRectangle const & rect) {
		left -= rect.left;
		right += rect.right;
		bottom -= rect.bottom;
		top += rect.top;
		return *this;
	}

	HPS_INLINE IntRectangle & Contract(int border) {
		left += border;
		right -= border;
		bottom += border;
		top -= border;
		return *this;
	}

	HPS_INLINE IntRectangle & Contract(IntRectangle const & rect) {
		left += rect.left;
		right -= rect.right;
		bottom += rect.bottom;
		top -= rect.top;
		return *this;
	}

	HPS_INLINE IntRectangle & Intersect(IntRectangle const & rect) {
		left = Max(left, rect.left);
		right = Min(right, rect.right);
		bottom = Max(bottom, rect.bottom);
		top = Min(top, rect.top);
		return *this;
	}

	HPS_INLINE IntRectangle & Union(IntRectangle const & rect) {
		left = Min(left, rect.left);
		right = Max(right, rect.right);
		bottom = Min(bottom, rect.bottom);
		top = Max(top, rect.top);
		return *this;
	}


	static HPS_INLINE IntRectangle Invalid() {
		return IntRectangle();
	}

	static HPS_INLINE IntRectangle Zero() {
		return IntRectangle(0, 0, 0, 0);
	}

};

HPS_INLINE IntRectangle Expand(IntRectangle const & a, IntRectangle const & border) {
	IntRectangle temp = a;
	return temp.Expand(border);
}

HPS_INLINE IntRectangle Expand(IntRectangle const & a, int border) {
	IntRectangle temp = a;
	return temp.Expand(border);
}

HPS_INLINE IntRectangle Contract(IntRectangle const & a, int border) {
	IntRectangle temp = a;
	return temp.Contract(border);
}

HPS_INLINE IntRectangle Contract(IntRectangle const & a, IntRectangle const & border) {
	IntRectangle temp = a;
	return temp.Contract(border);
}

HPS_INLINE IntRectangle Intersect(IntRectangle const & a, IntRectangle const & b) {
	IntRectangle temp = a;
	return temp.Intersect(b);
}

HPS_INLINE IntRectangle Union(IntRectangle const & a, IntRectangle const & b) {
	IntRectangle temp = a;
	return temp.Union(b);
}

struct HPS_API Rectangle {
	float	left;
	float	right;
	float	bottom;
	float	top;

	Rectangle()
		: left(std::numeric_limits<float>::max()), right(std::numeric_limits<float>::min()),
		bottom(std::numeric_limits<float>::max()), top(std::numeric_limits<float>::min()) {}

	Rectangle(float in_left, float in_right, float in_bottom, float in_top)
		: left(in_left), right(in_right), bottom(in_bottom), top(in_top) {}

	Rectangle(Rectangle const & that) 
		: left(that.left), right(that.right), bottom(that.bottom), top(that.top) {}

	explicit Rectangle(IntRectangle const & that)
		: left((float)that.left), right((float)that.right), bottom((float)that.bottom), top((float)that.top) {}

	HPS_INLINE	Rectangle(size_t count, Point const * points) {
		left = points->x;
		right = points->x;
		top = points->y;
		bottom = points->y;
		--count;
		if (count > 0) {
			++points;
			Merge(count, points);
		}
	}

	HPS_INLINE	Rectangle(size_t count, Point2D const * points) {
		left = points->x;
		right = points->x;
		top = points->y;
		bottom = points->y;
		--count;
		if (count > 0) {
			++points;
			Merge(count, points);
		}
	}

	HPS_INLINE float Width() const {
		return right - left;
	}

	HPS_INLINE float Height() const {
		return top - bottom;
	}

	HPS_INLINE float Area() const {
		return Width() * Height();
	}

	HPS_INLINE Point2D Center() const {
		return Point2D((left + right) * 0.5f, (bottom + top) * 0.5f);
	}

	HPS_INLINE void Merge(size_t count, Point const * points) {

		while (count > 1) {
			if (Compare(points[0].x, points[1].x)>0) {
				Float::replace_if_smaller(left, points[1].x);
				Float::replace_if_larger(right, points[0].x);
			}
			else {
				Float::replace_if_smaller(left, points[0].x);
				Float::replace_if_larger(right, points[1].x);
			}

			if (Compare(points[0].y, points[1].y)>0) {
				Float::replace_if_smaller(bottom, points[1].y);
				Float::replace_if_larger(top, points[0].y);
			}
			else {
				Float::replace_if_smaller(bottom, points[0].y);
				Float::replace_if_larger(top, points[1].y);
			}

			points += 2;
			count -= 2;
		}

		if (count > 0)
			Merge(*points);
	}

	HPS_INLINE void Merge(Point const & point) 	{
		Float::replace_if_smaller(left, point.x);
		Float::replace_if_smaller(bottom, point.y);
		Float::replace_if_larger(right, point.x);
		Float::replace_if_larger(top, point.y);
	}

	HPS_INLINE void Merge(size_t count, Point2D const * points) {

		while (count > 1) {
			if (Compare(points[0].x, points[1].x)>0) {
				Float::replace_if_smaller(left, points[1].x);
				Float::replace_if_larger(right, points[0].x);
			}
			else {
				Float::replace_if_smaller(left, points[0].x);
				Float::replace_if_larger(right, points[1].x);
			}

			if (Compare(points[0].y, points[1].y)>0) {
				Float::replace_if_smaller(bottom, points[1].y);
				Float::replace_if_larger(top, points[0].y);
			}
			else {
				Float::replace_if_smaller(bottom, points[0].y);
				Float::replace_if_larger(top, points[1].y);
			}

			points += 2;
			count -= 2;
		}

		if (count > 0)
			Merge(*points);
	}

	HPS_INLINE void Merge(Point2D const & point) 	{
		Float::replace_if_smaller(left, point.x);
		Float::replace_if_smaller(bottom, point.y);
		Float::replace_if_larger(right, point.x);
		Float::replace_if_larger(top, point.y);
	}

	bool operator==(Rectangle const & rect) const {
		return (left == rect.left && right == rect.right && bottom == rect.bottom && top == rect.top);
	}

	bool operator!=(Rectangle const & rect) const {
		return  !(*this == rect);
	}

	HPS_INLINE bool Intersecting(Rectangle const & rect) const {
		return	right >= rect.left && left <= rect.right && top >= rect.bottom && bottom <= rect.top;
	}

	HPS_INLINE bool Contains(Point const & contained) const {
		return !(contained.x < left || contained.x > right || contained.y < bottom || contained.y > top);
	}

	HPS_INLINE bool Contains(Point2D const & contained) const {
		return !(contained.x < left || contained.x > right || contained.y < bottom || contained.y > top);
	}

	HPS_INLINE bool Contains(Rectangle const & rect) {
		return (left <= rect.left && right >= rect.right && bottom <= rect.bottom && top >= rect.top);
	}

	HPS_INLINE bool Contains(Rectangle const & rect, float epsilon) {
		return (left <= rect.left + epsilon && right >= rect.right - epsilon && 
				bottom <= rect.bottom + epsilon && top >= rect.top	- epsilon);
	}

	HPS_INLINE Rectangle & Expand(float border) {
		left -= border;
		right += border;
		bottom -= border;
		top += border;
		return *this;
	}

	HPS_INLINE Rectangle & Expand(int border) {
		Expand((float)border);
		return *this;
	}

	HPS_INLINE Rectangle & Expand(Rectangle const & rect) {
		left -= rect.left;
		right += rect.right;
		bottom -= rect.bottom;
		top += rect.top;
		return *this;
	}

	HPS_INLINE Rectangle & Contract(int border) {
		left += border;
		right -= border;
		bottom += border;
		top -= border;
		return *this;
	}

	HPS_INLINE Rectangle & Contract(Rectangle const & rect) {
		left += rect.left;
		right -= rect.right;
		bottom += rect.bottom;
		top -= rect.top;
		return *this;
	}

	HPS_INLINE Rectangle & Intersect(Rectangle const & rect) {
		left = Max(left, rect.left);
		right = Min(right, rect.right);
		bottom = Max(bottom, rect.bottom);
		top = Min(top, rect.top);
		return *this;
	}

	HPS_INLINE Rectangle & Union(Rectangle const & rect) {
		left = Min(left, rect.left);
		right = Max(right, rect.right);
		bottom = Min(bottom, rect.bottom);
		top = Max(top, rect.top);
		return *this;
	}

	HPS_INLINE Rectangle & Inscribe_Scope(Rectangle const & scope) {
		float scale = (scope.right - scope.left) * 0.5f;
		float trans = (scope.right + scope.left) * 0.5f;

		left = left * scale + trans;
		right = right * scale + trans;

		scale = (scope.top - scope.bottom) * 0.5f;
		trans = (scope.top + scope.bottom) * 0.5f;

		bottom = bottom * scale + trans;
		top = top * scale + trans;
		return *this;
	}

	HPS_INLINE Rectangle & Circumscribe_Scope(Rectangle const & scope) {
		float tmp = 2.0f/(scope.right - scope.left);
		right = (right - scope.left) * tmp - 1.0f;
		left = (left - scope.left) * tmp - 1.0f;

		tmp = 2.0f/(scope.top - scope.bottom);
		top = (top - scope.bottom) * tmp - 1.0f;
		bottom = (bottom - scope.bottom) * tmp - 1.0f;
		return *this;
	}

	static HPS_INLINE Rectangle FullScope() {
		return Rectangle(-1.0f, 1.0f, -1.0f, 1.0f);
	}

	static HPS_INLINE Rectangle InvalidScope() {
		return Rectangle(1.0f, -1.0f, 1.0f, -1.0f);
	}

	static HPS_INLINE Rectangle Zero() {
		return Rectangle(0, 0, 0, 0);
	}

};


HPS_INLINE IntRectangle Floor(Rectangle const & a) {
	IntRectangle temp;
	temp.left = Floor (a.left);
	temp.right = Floor (a.right);
	temp.bottom = Floor (a.bottom);
	temp.top = Floor (a.top);
	return temp;
}

HPS_INLINE Rectangle Expand(Rectangle const & a, Rectangle const & border) {
	Rectangle temp = a;
	return temp.Expand(border);
}

HPS_INLINE Rectangle Expand(Rectangle const & a, float border) {
	Rectangle temp = a;
	return temp.Expand(border);
}

HPS_INLINE Rectangle Contract(Rectangle const & a, int border) {
	Rectangle temp = a;
	return temp.Contract(border);
}

HPS_INLINE Rectangle Contract(Rectangle const & a, Rectangle const & border) {
	Rectangle temp = a;
	return temp.Contract(border);
}

HPS_INLINE Rectangle Intersect(Rectangle const & a, Rectangle const & b) {
	Rectangle temp = a;
	return temp.Intersect(b);
}

HPS_INLINE Rectangle Union(Rectangle const & a, Rectangle const & b) {
	Rectangle temp = a;
	return temp.Union(b);
}

HPS_INLINE Rectangle Inscribe_Scope(Rectangle const & a, Rectangle const & scope) {
	Rectangle temp = a;
	return temp.Inscribe_Scope(scope);
}

HPS_INLINE Rectangle Circumscribe_Scope(Rectangle const & a, Rectangle const & scope) {
	Rectangle temp = a;
	return temp.Circumscribe_Scope(scope);
}

HPS_INLINE	IntRectangle::IntRectangle(Rectangle const & that)
	: left ((int)that.left), right ((int)that.right), bottom ((int)that.bottom), top ((int)that.top) {}




template <typename F>
struct Sphere_3D;

template <typename F>
struct HPS_TEMPLATE_API Cuboid_3D {
	/*! The minimum point of this cuboid */
	Point_3D<F>		min;
	/*! The maximum point of this cuboid */
	Point_3D<F>		max;


	/*!
		Creates an invalid cuboid. 
	*/
	Cuboid_3D () : min (Limit_Point()), max (-Limit_Point()) {}

	template <typename D>
	
	/*!
		Creates a cuboid equal to another cuboid. 
		\param that The other cuboid
	*/
	explicit Cuboid_3D (Cuboid_3D<D> const & that) : min (Point_3D<F>(that.min)), max (Point_3D<F>(that.max)) {}


	/*!
		Creates a cuboid that will fit tightly around a sphere. 
		\param that The sphere to fit
	*/
	Cuboid_3D (Sphere_3D<F> const & that);

	/*!
		Creates a cuboid based on two points, which become opposite corners of the cuboid. 
		\param in_min The first point of the cuboid
		\param in_max The second point of the cuboid
	*/
	Cuboid_3D (Point_3D<F> const & in_min, Point_3D<F> const & in_max) : min (in_min), max (in_max) {}

	/*!
		Creates a cuboid based on an array of points. Only the minimum and maximum points are used, which become opposite corners of the cuboid. If count is 0, then an invalid cuboid is returned.
		\param count The size of points
		\param points An array of points
	*/
	Cuboid_3D (size_t count, Point_3D<F> const * points) {
		if (count == 0) {
			min = Limit_Point();
			max = -Limit_Point();
			return;
		}
		min = max = *points++;
		--count;
		if (count>0)
			Merge(count, points);
	}

	/*!
		Creates a cuboid with the same dimensions as a rectangle.
		\param that The rectangle to create the cuboid from
	*/
	Cuboid_3D (Rectangle const & that) : min (Point_3D<F>(that.left, that.bottom, 0)), max (Point_3D<F>(that.right, that.top, 0)) {}

	/*!
		Tests whether this cuboid is valid. The cuboid is considered valid if the minimum point is less than or equal to the maximum point.
	*/
	HPS_INLINE bool		IsValid() const {
		return min.x <= max.x && min.y <= max.y && min.z <= max.z;
	}

	/*!
		Creates an invalid cuboid.
	*/
	static HPS_INLINE Cuboid_3D Invalid() {return Cuboid_3D ();};

	/*!
		Invalidates this cuboid.
	*/
	void Invalidate() {min = Limit_Point(); max = -Limit_Point();}

	/*!
		Determines if the maximum and minimum points of this cuboid are equal to the maximum and minimum points of another cuboid.
		\param cuboid The other cuboid
	*/
	HPS_INLINE bool		operator== (Cuboid_3D const & cuboid) const	{ return  (min == cuboid.min && max == cuboid.max); }
	
	/*!
		Determines if the maximum and minimum points of this cuboid are not equal to the maximum and minimum points of another cuboid.
		\param cuboid The other cuboid
	*/
	HPS_INLINE bool		operator!= (Cuboid_3D const & cuboid) const	{ return  !(*this == cuboid); }

	/*!
		Assigns the eight corners of the cuboid based on the points in the parameter array. The array must contain at least eight points.
		\param points The array which contains points
	*/
	HPS_INLINE void Generate_Cuboid_Points (Point_3D<F> * points) const {
		points[0] = Point_3D<F> (min.x, min.y, min.z);
		points[1] = Point_3D<F> (min.x, min.y, max.z);
		points[2] = Point_3D<F> (min.x, max.y, min.z);
		points[3] = Point_3D<F> (min.x, max.y, max.z);
		points[4] = Point_3D<F> (max.x, min.y, min.z);
		points[5] = Point_3D<F> (max.x, min.y, max.z);
		points[6] = Point_3D<F> (max.x, max.y, min.z);
		points[7] = Point_3D<F> (max.x, max.y, max.z);
	}

	/*!
		Returns the diagonal vector of the cuboid.
	*/
	HPS_INLINE Vector_3D<F> Diagonal() const { return max - min; }

	/*!
		Returns the volume of this cuboid.
	*/
	HPS_INLINE F Volume () const { return (max.x - min.x) * (max.y - min.y) * (max.z - min.z); }

	/*! 
		Determines whether this cuboid intersects with another cuboid.
	
		\param cuboid The other cuboid
	*/
	HPS_INLINE bool Intersecting (Cuboid_3D const & cuboid) const {
		return	max.x >= cuboid.min.x && min.x <= cuboid.max.x &&
				max.y >= cuboid.min.y && min.y <= cuboid.max.y &&
				max.z >= cuboid.min.z && min.z <= cuboid.max.z;
	}

	/*! 
		Determines whether this cuboid intersects with another cuboid. Allows for a tolerance value to be considered.
	
		\param cuboid The other cuboid
	*/
	HPS_INLINE bool Intersecting (Cuboid_3D const & cuboid, F const allowance) const {
		return	max.x + allowance >= cuboid.min.x && min.x - allowance <= cuboid.max.x &&
				max.y + allowance >= cuboid.min.y && min.y - allowance <= cuboid.max.y &&
				max.z + allowance >= cuboid.min.z && min.z - allowance <= cuboid.max.z;
	}

	/*!
		Returns true if this cuboid intersects with the parameter cuboid along a certain axis.
	
		\param dimension Must be either 0, 1, or 2. Corresponds to the X, Y, and Z axes.
		\param cuboid The cuboid to test against
	*/
	HPS_INLINE bool Intersecting (int dimension, Cuboid_3D const & cuboid) const {
		//ASSERT (0 <= dimension && dimension <= 2);
		return	max[dimension] >= cuboid.min[dimension] && min[dimension] <= cuboid.max[dimension];
	}

	/*!
		Returns true if this cuboid intersects with the parameter cuboid along a certain axis, and considers a tolerance value.
	
		\param dimension Must be 0, 1, or 2. Corresponds to the X, Y, and Z axes.
		\param cuboid The cuboid to test against
		\param allowance The tolerance value
	*/
	HPS_INLINE bool Intersecting (int dimension, Cuboid_3D const & cuboid, F const allowance) const {
		//ASSERT (0 <= dimension && dimension <= 2);
		return	max[dimension] + allowance >= cuboid.min[dimension] && min[dimension] - allowance <= cuboid.max[dimension];
	}

	/*! 
		Determines whether this cuboid intersects with a vector.
	
		\param start The start point of the vector
		\param direction The vector
	*/
	bool Intersecting (Point_3D<F> const & start, Vector_3D<F> const & direction) const;

	/*!
		Merges this cuboid with another cuboid. Only the smallest and largest values of the cuboids are retained.
	
		\param cuboid The other cuboid
	*/
	HPS_INLINE void Merge(Cuboid_3D const & cuboid) 	{
		Float::replace_if_smaller(min.x, cuboid.min.x);
		Float::replace_if_smaller(min.y, cuboid.min.y);
		Float::replace_if_smaller(min.z, cuboid.min.z);
		Float::replace_if_larger(max.x, cuboid.max.x);
		Float::replace_if_larger(max.y, cuboid.max.y);
		Float::replace_if_larger(max.z, cuboid.max.z);
	}

	/*!
		Merges this cuboid with another cuboid created from a sphere. Only the smallest and largest values of the cuboids are retained.
	
		\param cuboid The other cuboid
	*/
	HPS_INLINE void Merge(Sphere_3D<F> const & sphere) { Merge (Cuboid_3D (sphere)); }

	/*!
		Expands the cuboid to include a point.
	
		\param point The point to include
	*/
	HPS_INLINE void Merge(Point_3D<F> const & point) 	{
		Float::replace_if_smaller(min.x, point.x);
		Float::replace_if_smaller(min.y, point.y);
		Float::replace_if_smaller(min.z, point.z);
		Float::replace_if_larger(max.x, point.x);
		Float::replace_if_larger(max.y, point.y);
		Float::replace_if_larger(max.z, point.z);
	}

	/*!
		Expands this cuboid to include a set of points.
	
		\param count The number of points in points
		\param points The array of points
	*/
	void Merge(size_t count, Point_3D<F> const * points) 	{
		while (count > 1) {
			if (Compare(points[0].x, points[1].x)>0) {
				Float::replace_if_smaller(min.x, points[1].x);
				Float::replace_if_larger(max.x, points[0].x);
			}
			else {
				Float::replace_if_smaller(min.x, points[0].x);
				Float::replace_if_larger(max.x, points[1].x);
			}

			if (Compare(points[0].y, points[1].y)>0) {
				Float::replace_if_smaller(min.y, points[1].y);
				Float::replace_if_larger(max.y, points[0].y);
			}
			else {
				Float::replace_if_smaller(min.y, points[0].y);
				Float::replace_if_larger(max.y, points[1].y);
			}

			if (Compare(points[0].z, points[1].z)>0) {
				Float::replace_if_smaller(min.z, points[1].z);
				Float::replace_if_larger(max.z, points[0].z);
			}
			else {
				Float::replace_if_smaller(min.z, points[0].z);
				Float::replace_if_larger(max.z, points[1].z);
			}

			points += 2;
			count -= 2;
		}

		if (count > 0)
			Merge(*points);
	}

	/*!
		Returns true if the cuboid completely contains the parameter cuboid.
	
		\param contained The other cuboid
	*/
	HPS_INLINE bool Contains(Cuboid_3D const & contained) const {
		return (contained.min.x >= min.x &&
				contained.min.y >= min.y &&
				contained.min.z >= min.z &&
				contained.max.x <= max.x &&
				contained.max.y <= max.y &&
				contained.max.z <= max.z);
	}

	/*!
		Returns true if the cuboid completely contains a cuboid based on the  parameter points.
	
		\param contained The points to compare against
	*/
	HPS_INLINE bool Contains(Point_3D<F> const & contained) const {
		return (contained.x >= min.x &&
				contained.y >= min.y &&
				contained.z >= min.z &&
				contained.x <= max.x &&
				contained.y <= max.y &&
				contained.z <= max.z);
	}

	/*!
		Returns true if the cuboid completely contains a cuboid based on the  parameter points, with a tolerance value.
	
		\param contained The points to compare against
		\param epsilon The tolerance value
	*/
	HPS_INLINE bool Contains(Point_3D<F> const & contained, F epsilon) const {
		return (contained.x >= min.x - epsilon &&
				contained.y >= min.y - epsilon &&
				contained.z >= min.z - epsilon &&
				contained.x <= max.x + epsilon &&
				contained.y <= max.y + epsilon &&
				contained.z <= max.z + epsilon);
	}

	/*!
		Replaces this cuboid with the intersection of this cuboid and the parameter cuboid.
	
		\param cuboid The cuboid to compute intersection against
	*/
	HPS_INLINE Cuboid_3D & Intersect(Cuboid_3D const & cuboid)  {
		Float::replace_if_larger(min.x, cuboid.min.x);
		Float::replace_if_larger(min.y, cuboid.min.y);
		Float::replace_if_larger(min.z, cuboid.min.z);
		Float::replace_if_smaller(max.x, cuboid.max.x);
		Float::replace_if_smaller(max.y, cuboid.max.y);
		Float::replace_if_smaller(max.z, cuboid.max.z);
		return *this;
	}

	/*!
		Replaces this cuboid with the union of this cuboid and the parameter cuboid.
	
		\param cuboid The cuboid to compute union against
	*/
	HPS_INLINE Cuboid_3D & Union(Cuboid_3D const & cuboid)  {
		Float::replace_if_smaller(min.x, cuboid.min.x);
		Float::replace_if_smaller(min.y, cuboid.min.y);
		Float::replace_if_smaller(min.z, cuboid.min.z);
		Float::replace_if_larger(max.x, cuboid.max.x);
		Float::replace_if_larger(max.y, cuboid.max.y);
		Float::replace_if_larger(max.z, cuboid.max.z);
		return *this;
	}

	/*!
		Expands both corners of this cuboid.
	
		\param border Amount to expand
	*/
	HPS_INLINE Cuboid_3D & Expand(F border)  {
		Vector_3D<F>		delta (border, border, border);
		min -= delta;
		max += delta;
		return *this;
	}

	/*!
		Contracts both corners of this cuboid.
	
		\param border Amount to contract
	*/
	HPS_INLINE Cuboid_3D & Contract(F border)  {
		Vector_3D<F>		delta (border, border, border);
		min += delta;
		max -= delta;
		return *this;
	}

	private:
		HPS_INLINE static Point_3D<F> Limit_Point () {
			F const		limit = std::numeric_limits<F>::max();
			return Point_3D<F> (limit, limit, limit);
		}
};

typedef	Cuboid_3D<float>	SimpleCuboid;
typedef	Cuboid_3D<double>	DSimpleCuboid;


template <typename F>
HPS_INLINE Cuboid_3D<F> Intersect(Cuboid_3D<F> const & a, Cuboid_3D<F> const & b) {
	Cuboid_3D<F> temp = a;
	return temp.Intersect(b);
}

template <typename F>
HPS_INLINE Cuboid_3D<F> Union(Cuboid_3D<F> const & a, Cuboid_3D<F> const & b) {
	Cuboid_3D<F> temp = a;
	return temp.Union(b);
}

template <typename F>
HPS_INLINE Cuboid_3D<F> Expand(Cuboid_3D<F> const & a, F border) {
	Cuboid_3D<F> temp = a;
	return temp.Expand(border);
}

template <typename F>
HPS_INLINE Cuboid_3D<F> Contract(Cuboid_3D<F> const & a, F border) {
	Cuboid_3D<F> temp = a;
	return temp.Contract(border);
}



template <typename F>
struct HPS_TEMPLATE_API Sphere_3D {
	Point_3D<F>			center;
	F					radius;

	Sphere_3D () : center(Point_3D<F>(0, 0, 0)), radius(0) {}

	template <typename D>
		explicit Sphere_3D (Sphere_3D<D> const & that) : center(Point_3D<F>(that.center)), radius(F(that.radius)) {}

	Sphere_3D (Cuboid_3D<F> const & cuboid) :
		center (Midpoint(cuboid.min, cuboid.max)), radius (F(0.5 * cuboid.Diagonal().Length())) {}

	Sphere_3D (Point_3D<F> const & starting_center, F in_radius = 0) : center(starting_center), radius(in_radius) {}

	Sphere_3D (size_t count, Point_3D<F> const * points) : radius(0.0f) {
		Cuboid_3D<F>	cuboid(count, points);
		center = Midpoint(cuboid.min, cuboid.max);
		Engulf (count, points);
	}

	Sphere_3D (size_t count, Point_3D<F> const * points, Point_3D<F> const & starting_center) : center(starting_center), radius(0) {
		Engulf (count, points);
	}

	HPS_INLINE bool		IsValid() const {
		return radius >= 0;
	}

	static HPS_INLINE Sphere_3D Invalid() {return Sphere_3D(Point_3D<F>(0,0,0), -1);};

	void Invalidate() {radius = -1;}

	HPS_INLINE bool	operator== (Sphere_3D const & sphere) const	{ return  (center == sphere.center && radius == sphere.radius); }
	HPS_INLINE bool	operator!= (Sphere_3D const & sphere) const	{ return  !(*this == sphere); }

	HPS_INLINE F	Volume () const { return F((4.0 / 3.0 * PI) * radius * radius * radius); }

	HPS_INLINE void Merge(Point_3D<F> const & point) {
		Vector_3D<F>	dir = point - center;
		F				distance = (F)dir.Length();

		if (distance > radius) {
			F	t = F(0.5) * (distance - radius);
			center += t * dir.Normalize();
			radius += t;
		}
	}

	HPS_INLINE void Merge(size_t count, Point_3D<F> const * points) {

		for (size_t i = 0; i < count; ++i) {
			Vector_3D<F>	dir = *points - center;
			F				distance = (F)dir.Length();

			if (distance > radius) {
				F			t = F(0.5) * (distance - radius);
				center += t * dir.Normalize();
				radius += t;
			}

			++points;
		}
	}

	HPS_INLINE void	Merge (Sphere_3D const & sphere) {
		Vector_3D<F>	dir = sphere.center - center;
		F				distance = (F)dir.Length();

		if (distance + sphere.radius > radius) {
			if (distance + radius > sphere.radius) {
				F		t = F(0.5 * (sphere.radius + distance - radius));
				center += t * dir.Normalize();
				radius += t;
			}
			else {
				center = sphere.center;
				radius = sphere.radius;
			}
		}
	}

	HPS_INLINE void	Merge (Cuboid_3D<F> const & cuboid)	{ Merge (Sphere_3D (cuboid)); }

private:
	// Engulf expands the sphere to include the points, but does not change the center as Merge does
	HPS_INLINE void Engulf (size_t count, Point_3D<F> const * points) {
		for (size_t i = 0; i < count; ++i) {
			double	dsq = (*points++ - center).LengthSquared();
			if ((F)dsq > radius * radius)
				radius = (F)sqrt(dsq);
		}
	}
};

typedef	Sphere_3D<float>	SimpleSphere;
typedef	Sphere_3D<double>	DSimpleSphere;


template <typename F>
HPS_INLINE	Cuboid_3D<F>::Cuboid_3D(Sphere_3D<F> const & sphere) {
	min = Point_3D<F>(sphere.center.x - sphere.radius, sphere.center.y - sphere.radius, sphere.center.z - sphere.radius);
	max = Point_3D<F>(sphere.center.x + sphere.radius, sphere.center.y + sphere.radius, sphere.center.z + sphere.radius);
}




class RGB24Color;
class RGBA32Color;
class RGBAS32Color;
class RGBAColor;

class HPS_API RGBColor {
public:
	float	red;
	float	green;
	float	blue;

	HPS_INLINE RGBColor () {}
	HPS_INLINE RGBColor (float r, float g, float b) : red (r), green (g), blue (b) {}
	explicit HPS_INLINE RGBColor (RGB24Color const & c24);
	explicit HPS_INLINE RGBColor (float gray) : red (gray), green (gray), blue (gray) {}
	explicit HPS_INLINE RGBColor (RGBAS32Color const & c32);
	explicit HPS_INLINE RGBColor (RGBA32Color const & c32);
	explicit HPS_INLINE RGBColor (RGBAColor const & c);

	HPS_INLINE bool		IsGray() const {return (red == green && green == blue);}
	HPS_INLINE float	Gray() const { return  0.3125f * red  +  0.5000f * green  +  0.1875f * blue; }
	float Distance(RGBColor const & other_color) const;
	HPS_INLINE bool		IsValid() const {
		return (Float::extract_sign_bit(red) | Float::extract_sign_bit(green) | Float::extract_sign_bit(blue)) == 0;
	}

	HPS_INLINE bool	operator== (RGBColor const & c) const	{ return  red == c.red && green == c.green && blue == c.blue; }
	HPS_INLINE bool	operator!= (RGBColor const & c) const	{ return  !(*this == c); }

	HPS_INLINE bool	Equals(RGBColor const & c, int in_tolerance = 32) const
		{ return  Float::Equals(red, c.red, in_tolerance) && Float::Equals(green, c.green, in_tolerance) && Float::Equals(blue, c.blue, in_tolerance); }

	HPS_INLINE RGBColor &		operator*= (RGBColor const & c)	{ red *= c.red; green *= c.green; blue *= c.blue;  return *this; }
	HPS_INLINE RGBColor &		operator+= (RGBColor const & c)	{ red += c.red; green += c.green; blue += c.blue;  return *this; }
	HPS_INLINE RGBColor &		operator-= (RGBColor const & c)	{ red -= c.red; green -= c.green; blue -= c.blue;  return *this; }
	HPS_INLINE RGBColor const	operator* (RGBColor const & c) const { return RGBColor (red * c.red, green * c.green, blue * c.blue); }
	HPS_INLINE RGBColor const	operator+ (RGBColor const & c) const { return RGBColor (red + c.red, green + c.green, blue + c.blue); }
	HPS_INLINE RGBColor const	operator- (RGBColor const & c) const { return RGBColor (red - c.red, green - c.green, blue - c.blue); }

	HPS_INLINE RGBColor &		operator*= (float s) { red *= s; green *= s; blue *= s;  return *this; }
	HPS_INLINE RGBColor &		operator/= (float s) { return operator*= (1.0f / s); }
	HPS_INLINE RGBColor &		operator+= (float s) { red += s; green += s; blue += s;  return *this; }
	HPS_INLINE RGBColor &		operator-= (float s) { red -= s; green -= s; blue -= s;  return *this; }
	HPS_INLINE RGBColor const	operator* (float s) const { return RGBColor (red * s, green * s, blue * s); }
	HPS_INLINE RGBColor const	operator/ (float s) const { return operator* (1.0f / s); }
	HPS_INLINE RGBColor const	operator+ (float s) const { return RGBColor (red + s, green + s, blue + s); }
	HPS_INLINE RGBColor const	operator- (float s) const { return RGBColor (red - s, green - s, blue - s); }

	static HPS_INLINE RGBColor Black() {return RGBColor (0, 0, 0);};
	static HPS_INLINE RGBColor White() {return RGBColor (1, 1, 1);};
	static HPS_INLINE RGBColor Invalid() {return RGBColor (-1, -1, -1);};

	void ShowHLS(float & out_hue, float & out_lightness, float & out_saturation) const;
	void ShowHSV(float & out_hue, float & out_saturation, float & out_value) const;
	void ShowHIC(float & out_hue, float & out_intensity, float & out_chromaticity) const;

	static RGBColor HLS(float in_hue, float in_lightness, float in_saturation);
	static RGBColor HSV(float in_hue, float in_saturation, float in_value);
	static RGBColor HIC(float in_hue, float in_intensity, float in_chromaticity);
};

HPS_INLINE	RGBColor const	operator* (float s, RGBColor const & v) { return RGBColor (s * v.red, s * v.green, s * v.blue); }
HPS_INLINE	RGBColor const	operator+ (float s, RGBColor const & v) { return RGBColor (s + v.red, s + v.green, s + v.blue); }
HPS_INLINE	RGBColor const	operator- (float s, RGBColor const & v) { return RGBColor (s - v.red, s - v.green, s - v.blue); }


class HPS_API RGBAColor {
public:
	
	float	red; /*!< Red component of this color object, from 0.0 to 1.0 */
	float	green; /*!< Green component of this color object, from 0.0 to 1.0 */
	float	blue; /*!< Blue component of this color object, from 0.0 to 1.0 */
	float	alpha; /*!< Alpha component of this color object, from 0.0 to 1.0 */

	HPS_INLINE RGBAColor () {}
	explicit HPS_INLINE RGBAColor (float gray, float a = 1) : red (gray), green (gray), blue (gray), alpha (a) {}
	HPS_INLINE RGBAColor (float r, float g, float b, float a = 1) : red (r), green (g), blue (b), alpha (a) {}

	HPS_INLINE RGBAColor (RGBColor const & c) {
		memcpy(this, &c, sizeof(RGBColor)); //-V512
		alpha = 1.0f;
		Float::apply_sign_bit(alpha, Float::extract_sign_bit(c.red) | Float::extract_sign_bit(c.green) | Float::extract_sign_bit(c.blue));
	}
	HPS_INLINE RGBAColor (RGBColor const & c, float a) {
		memcpy(this, &c, sizeof(RGBColor));
		memcpy(&alpha, &a, sizeof(float));
		Float::apply_sign_bit(alpha, Float::extract_sign_bit(c.red) | Float::extract_sign_bit(c.green) | Float::extract_sign_bit(c.blue));
	}
	explicit HPS_INLINE RGBAColor (RGBA32Color const & c32);
	explicit HPS_INLINE RGBAColor (RGBAS32Color const & c32);

	HPS_INLINE bool		IsGray() const {return (red == green && green == blue);}
	HPS_INLINE float	Gray() const { return  0.3125f * red  +  0.5000f * green  +  0.1875f * blue; }
	HPS_INLINE bool		IsValid() const {
		return (Float::extract_sign_bit(red) | Float::extract_sign_bit(green) | Float::extract_sign_bit(blue) | Float::extract_sign_bit(alpha)) == 0;
	}

	/*!
		Determines RGBA equivalency between this object and argument.

		\param c The color object to test against
		\return true if the objects are equivalent, false otherwise
	*/
	HPS_INLINE bool	operator== (RGBAColor const & c) const		{ return  red == c.red && green == c.green && blue == c.blue && alpha == c.alpha; }

	/*!
	Determines RGBA non-equivalency between this object and argument.

	\param c The color object to test against
	\return true if the objects are not equivalent, false otherwise
	*/
	HPS_INLINE bool	operator!= (RGBAColor const & c) const		{ return  !(*this == c); }

	HPS_INLINE bool	Equals(RGBAColor const & c, int in_tolerance = 32) const {
		return Float::Equals(red, c.red, in_tolerance) && Float::Equals(green, c.green, in_tolerance) &&
				Float::Equals(blue, c.blue, in_tolerance) && Float::Equals(alpha, c.alpha, in_tolerance);
	}

	HPS_INLINE RGBAColor &		operator*= (RGBAColor const & c)	{ red *= c.red; green *= c.green; blue *= c.blue; alpha *= c.alpha; return *this; }
	HPS_INLINE RGBAColor &		operator+= (RGBAColor const & c)	{ red += c.red; green += c.green; blue += c.blue; alpha += c.alpha; return *this; }
	HPS_INLINE RGBAColor &		operator-= (RGBAColor const & c)	{ red -= c.red; green -= c.green; blue -= c.blue; alpha -= c.alpha; return *this; }
	HPS_INLINE RGBAColor const	operator* (RGBAColor const & c) const { return RGBAColor (red * c.red, green * c.green, blue * c.blue, alpha * c.alpha); }
	HPS_INLINE RGBAColor const	operator+ (RGBAColor const & c) const { return RGBAColor (red + c.red, green + c.green, blue + c.blue, alpha + c.alpha); }
	HPS_INLINE RGBAColor const	operator- (RGBAColor const & c) const { return RGBAColor (red - c.red, green - c.green, blue - c.blue, alpha - c.alpha); }

	HPS_INLINE RGBAColor &		operator*= (float s) { red *= s; green *= s; blue *= s; alpha *= s; return *this; }
	HPS_INLINE RGBAColor &		operator/= (float s) { return operator*= (1.0f / s); }
	HPS_INLINE RGBAColor &		operator+= (float s) { red += s; green += s; blue += s; alpha += s; return *this; }
	HPS_INLINE RGBAColor &		operator-= (float s) { red -= s; green -= s; blue -= s; alpha -= s; return *this; }
	HPS_INLINE RGBAColor const	operator* (float s) const { return RGBAColor (red * s, green * s, blue * s, alpha * s); }
	HPS_INLINE RGBAColor const	operator/ (float s) const { return operator* (1.0f / s); }
	HPS_INLINE RGBAColor const	operator+ (float s) const { return RGBAColor (red + s, green + s, blue + s, alpha + s); }
	HPS_INLINE RGBAColor const	operator- (float s) const { return RGBAColor (red - s, green - s, blue - s, alpha - s); }

	HPS_INLINE RGBAColor &		operator*= (RGBColor const & c)	{ red *= c.red; green *= c.green; blue *= c.blue; return *this; }
	HPS_INLINE RGBAColor &		operator+= (RGBColor const & c)	{ red += c.red; green += c.green; blue += c.blue; return *this; }
	HPS_INLINE RGBAColor &		operator-= (RGBColor const & c)	{ red -= c.red; green -= c.green; blue -= c.blue; return *this; }
	HPS_INLINE RGBAColor const	operator* (RGBColor const & c) const { return RGBAColor (red * c.red, green * c.green, blue * c.blue, alpha); }
	HPS_INLINE RGBAColor const	operator+ (RGBColor const & c) const { return RGBAColor (red + c.red, green + c.green, blue + c.blue, alpha); }
	HPS_INLINE RGBAColor const	operator- (RGBColor const & c) const { return RGBAColor (red - c.red, green - c.green, blue - c.blue, alpha); }

	static HPS_INLINE RGBAColor Black() {return RGBAColor (0, 0, 0, 1);};
	static HPS_INLINE RGBAColor White() {return RGBAColor (1, 1, 1, 1);};
	static HPS_INLINE RGBAColor Nothing() {return RGBAColor (0, 0, 0, 0);};
	static HPS_INLINE RGBAColor Invalid() {return RGBAColor (-1, -1, -1, -1);};
};



struct DirectRGBColor {
public:
	enum Order {
		Order_ABGR,
		Order_RGBA,
		Order_BGRA
	};

#ifdef _MSC_VER
	HPS_INLINE static Order Preferred_Order () {return Order_BGRA;}
	unsigned char	b, g, r, a;
#endif

#ifdef __linux__
	HPS_INLINE static Order Preferred_Order () {return Order_RGBA;}
	unsigned char	r, g, b, a;
#endif

#ifdef __APPLE_CC__
	HPS_INLINE static Order Preferred_Order () {return Order_RGBA;}
	unsigned char	r, g, b, a;
#endif
};



class RGBAS32Color : public DirectRGBColor {
public:

	HPS_INLINE RGBAS32Color () /* : DirectRGBColor() */ {}
	explicit HPS_INLINE RGBAS32Color (unsigned char gray, unsigned char aa = 255) {
		r = gray;
		g = gray;
		b = gray;
		a = aa;
	}
	HPS_INLINE RGBAS32Color (unsigned char rr, unsigned char gg, unsigned char bb, unsigned char aa = 255) {
		r = rr;
		g = gg;
		b = bb;
		a = aa;
	}
	HPS_INLINE RGBAS32Color (DirectRGBColor const & c) : DirectRGBColor(c) {};

	explicit HPS_INLINE RGBAS32Color (RGBColor const & c)
	{
		r = Float::unit_to_byte(c.red);
		g = Float::unit_to_byte(c.green);
		b = Float::unit_to_byte(c.blue);
		a = 255;
	}

	HPS_INLINE RGBAS32Color (RGB24Color const & c);

	HPS_INLINE RGBAS32Color (RGBA32Color const & c);

	HPS_INLINE RGBAS32Color (RGBColor const & c, float alpha)
	{
		r = Float::unit_to_byte(c.red);
		g = Float::unit_to_byte(c.green);
		b = Float::unit_to_byte(c.blue);
		a = Float::unit_to_byte(alpha);
	}
	HPS_INLINE RGBAS32Color (RGBColor const & c, unsigned char aa)
	{
		r = Float::unit_to_byte(c.red);
		g = Float::unit_to_byte(c.green);
		b = Float::unit_to_byte(c.blue);
		a = aa;
	}
	explicit HPS_INLINE RGBAS32Color (RGBAColor const & c)
	{
		r = Float::unit_to_byte(c.red);
		g = Float::unit_to_byte(c.green);
		b = Float::unit_to_byte(c.blue);
		a = Float::unit_to_byte(c.alpha);
	}
	HPS_INLINE RGBAS32Color (RGBAColor const & c, unsigned char mix)
	{
		r = Float::unit_to_byte(c.red);
		g = Float::unit_to_byte(c.green);
		b = Float::unit_to_byte(c.blue);
		a = Float::unit_to_byte_scaled(c.alpha, mix);
	}

	HPS_INLINE bool				IsGray() const {return (r == g && g == b);}
	HPS_INLINE unsigned char	Gray() const { return (unsigned char)(0.3125f * (float)r  +  0.5000f * (float)g  +  0.1875f * (float)b); }
	HPS_INLINE bool				IsValid() const { return ((r | g | b | a) != 0); }

	HPS_INLINE bool	operator== (RGBAS32Color const & c) const	{ return (r == c.r && g == c.g && b == c.b && a == c.a); }
	HPS_INLINE bool	operator!= (RGBAS32Color const & c) const	{ return !(*this == c); }

	static HPS_INLINE RGBAS32Color Black() {return RGBAS32Color (0, 0, 0, 255);};
	static HPS_INLINE RGBAS32Color White() {return RGBAS32Color (255, 255, 255, 255);};
	static HPS_INLINE RGBAS32Color Invalid() {return RGBAS32Color (0, 0, 0, 0);};

	static	HPS_INLINE unsigned char Opaque_Alpha () {return 0xFF;}
};

class RGBA32Color {
public:
	unsigned char	r;
	unsigned char	g;
	unsigned char	b;
	unsigned char	a;

	HPS_INLINE RGBA32Color () {}
	explicit HPS_INLINE RGBA32Color (unsigned char gray, unsigned char aa = 255)
		: r (gray), g (gray), b (gray), a (aa) {}
	HPS_INLINE RGBA32Color (unsigned char rr, unsigned char gg, unsigned char bb, unsigned char aa = 255)
		: r (rr), g (gg), b (bb), a (aa) {}
	HPS_INLINE RGBA32Color (RGBAS32Color const & c32)
		: r (c32.r), g (c32.g), b (c32.b), a (c32.a) {}
	explicit HPS_INLINE	RGBA32Color (RGBColor const & c) {
		r = Float::unit_to_byte(c.red);
		g = Float::unit_to_byte(c.green);
		b = Float::unit_to_byte(c.blue);
		a = 255;
	}
	HPS_INLINE RGBA32Color (RGBColor const & c, float alpha) {
		r = Float::unit_to_byte(c.red);
		g = Float::unit_to_byte(c.green);
		b = Float::unit_to_byte(c.blue);
		a = Float::unit_to_byte(alpha);
	}
	HPS_INLINE RGBA32Color (RGBColor const & c, unsigned char aa) {
		r = Float::unit_to_byte(c.red);
		g = Float::unit_to_byte(c.green);
		b = Float::unit_to_byte(c.blue);
		a = aa;
	}
	explicit HPS_INLINE RGBA32Color (RGBAColor const & c) {
		r = Float::unit_to_byte(c.red);
		g = Float::unit_to_byte(c.green);
		b = Float::unit_to_byte(c.blue);
		a = Float::unit_to_byte(c.alpha);
	}
	HPS_INLINE RGBA32Color (RGBAColor const & c, unsigned char mix) {
		r = Float::unit_to_byte(c.red);
		g = Float::unit_to_byte(c.green);
		b = Float::unit_to_byte(c.blue);
		a = Float::unit_to_byte_scaled(c.alpha, mix);
	}

	HPS_INLINE bool				IsGray() const {return (r == g && g == b);}
	HPS_INLINE unsigned char	Gray() const { return (unsigned char)(0.3125f * (float)r  +  0.5000f * (float)g  +  0.1875f * (float)b); }

	HPS_INLINE bool	operator== (RGBA32Color const & c) const { return (r == c.r && g == c.g && b == c.b && a == c.a); }
	HPS_INLINE bool	operator!= (RGBA32Color const & c) const { return  !(*this == c); }

	static HPS_INLINE RGBA32Color Black() {return RGBA32Color (0, 0, 0, 255);};
	static HPS_INLINE RGBA32Color White() {return RGBA32Color (255, 255, 255, 255);};

	static	HPS_INLINE unsigned char Opaque_Alpha () {return 0xFF;}
};


class RGB24Color
{
public:
	unsigned char r;
	unsigned char g;
	unsigned char b;

	HPS_INLINE RGB24Color () {}
	explicit HPS_INLINE RGB24Color (unsigned char gray)
		: r (gray), g (gray), b (gray) {}
	HPS_INLINE RGB24Color (unsigned char rr, unsigned char gg, unsigned char bb)
		: r (rr), g (gg), b (bb) {}
	explicit HPS_INLINE RGB24Color (RGBColor const & c) {
		r = Float::unit_to_byte(c.red);
		g = Float::unit_to_byte(c.green);
		b = Float::unit_to_byte(c.blue);
	}

	HPS_INLINE bool				IsGray() const {return (r == g && g == b);}
	HPS_INLINE unsigned char	Gray() const { return (unsigned char)(0.3125f * (float)r  +  0.5000f * (float)g  +  0.1875f * (float)b); }

	HPS_INLINE bool	operator== (RGB24Color const & c) const { return (r == c.r && g == c.g && b == c.b); }
	HPS_INLINE bool	operator!= (RGB24Color const & c) const { return  !(*this == c); }
};

HPS_INLINE RGBColor::RGBColor (RGBAS32Color const & c32) {
	red = Float::C2F(c32.r);
	green = Float::C2F(c32.g);
	blue = Float::C2F(c32.b);
}

HPS_INLINE RGBColor::RGBColor (RGBA32Color const & c32) {
	red = Float::C2F(c32.r);
	green = Float::C2F(c32.g);
	blue = Float::C2F(c32.b);
}

HPS_INLINE RGBColor::RGBColor (RGBAColor const & c) {
	red = c.red;
	green = c.green;
	blue = c.blue;
}

HPS_INLINE RGBColor::RGBColor (RGB24Color const & c24) {
	red = Float::C2F(c24.r);
	green = Float::C2F(c24.g);
	blue = Float::C2F(c24.b);
}

HPS_INLINE RGBAS32Color::RGBAS32Color (RGB24Color const & c)
{
	r = c.r;
	g = c.g;
	b = c.b;
	a = 255;
}

HPS_INLINE RGBAS32Color::RGBAS32Color (RGBA32Color const & c)
{
	r = c.r;
	g = c.g;
	b = c.b;
	a = c.a;
}

HPS_INLINE RGBAColor::RGBAColor (RGBAS32Color const & c32) {
	red = Float::C2F(c32.r);
	green = Float::C2F(c32.g);
	blue = Float::C2F(c32.b);
	alpha = Float::C2F(c32.a);
}

HPS_INLINE RGBAColor::RGBAColor (RGBA32Color const & c32) {
	red = Float::C2F(c32.r);
	green = Float::C2F(c32.g);
	blue = Float::C2F(c32.b);
	alpha = Float::C2F(c32.a);
}

HPS_INLINE RGBColor Modulate(RGBColor const & a, RGBColor const & b) {
	return RGBColor(a.red * b.red, a.green * b.green, a.blue * b.blue);
}



HPS_INLINE RGBColor Interpolate(RGBColor const & a, RGBColor const & b, float t) {
	return RGBColor(a.red + (b.red - a.red) * t, a.green + (b.green - a.green) * t, a.blue + (b.blue - a.blue) * t);
}

HPS_INLINE RGBAColor Interpolate(RGBAColor const & a, RGBAColor const & b, float t) {
	return RGBAColor(a.red + (b.red - a.red) * t, a.green + (b.green - a.green) * t, a.blue + (b.blue - a.blue) * t, a.alpha + (b.alpha - a.alpha) * t);
}

HPS_INLINE RGBAS32Color Interpolate(RGBAS32Color const & a, RGBAS32Color const & b, float t) {
	return RGBAS32Color(
		(unsigned char)(a.r + ((float)b.r - (float)a.r) * t),
		(unsigned char)(a.g + ((float)b.g - (float)a.g) * t),
		(unsigned char)(a.b + ((float)b.b - (float)a.b) * t),
		(unsigned char)(a.a + ((float)b.a - (float)a.a) * t));
}

HPS_INLINE RGBA32Color Interpolate(RGBA32Color const & a, RGBA32Color const & b, float t) {
	return RGBA32Color(
		(unsigned char)(a.r + ((float)b.r - (float)a.r) * t),
		(unsigned char)(a.g + ((float)b.g - (float)a.g) * t),
		(unsigned char)(a.b + ((float)b.b - (float)a.b) * t),
		(unsigned char)(a.a + ((float)b.a - (float)a.a) * t));
}

HPS_INLINE RGB24Color Interpolate(RGB24Color const & a, RGB24Color const & b, float t) {
	return RGB24Color(
		(unsigned char)(a.r + ((float)b.r - (float)a.r) * t),
		(unsigned char)(a.g + ((float)b.g - (float)a.g) * t),
		(unsigned char)(a.b + ((float)b.b - (float)a.b) * t));
}



class HPS_API Quaternion {
public:
	float		w;
	float		x;
	float		y;
	float		z;

	Quaternion() : w(0.0f), x(0.0f), y(0.0f), z(0.0f) { }

	Quaternion(float in_w, float in_x, float in_y, float in_z) : w(in_w), x(in_x), y(in_y), z(in_z) { }

	Quaternion(Quaternion const & that) : w(that.w), x(that.x), y(that.y), z(that.z) { }

	Quaternion const & Normalize() {
		float mag = Norm();

		if (mag > 0) {
			w /= mag;
			x /= mag;
			y /= mag;
			z /= mag;
		}
		return *this;
	}


	Quaternion operator* (Quaternion const & in_right) const {
		return Quaternion(w*in_right.w - x*in_right.x - y*in_right.y - z*in_right.z,
						y*in_right.z - z*in_right.y + w*in_right.x + x*in_right.w,
						z*in_right.x - x*in_right.z + w*in_right.y + y*in_right.w,
						x*in_right.y - y*in_right.x + w*in_right.z + z*in_right.w);
	}

	Quaternion operator* (float in_right) const {
		return Quaternion(w*in_right, x*in_right, y*in_right, z*in_right);
	}

	friend Quaternion operator* (float in_left, Quaternion const & in_right) {
		return Quaternion(in_left*in_right.w, in_left*in_right.x, in_left*in_right.y, in_left*in_right.z);
	}

	Quaternion operator/ (float in_right) const {
		return Quaternion(w/in_right, x/in_right, y/in_right, z/in_right);
	}

	Quaternion operator- (Quaternion const & in_right) const {
		return Quaternion(w-in_right.w, x-in_right.x, y-in_right.y, z-in_right.z);
	}

	Quaternion operator+ (Quaternion const & in_right) const {
		return Quaternion(w+in_right.w, x+in_right.x, y+in_right.y, z+in_right.z);
	}

	inline float Norm() const {
		return static_cast<float>(sqrt(w*w + x*x + y*y + z*z));
	}


	Quaternion Log() const {
		Quaternion	ret;
		float		mag_q = Norm();
		float		mag_V = static_cast<float>(sqrt(x*x + y*y + z*z));

		ret.w = static_cast<float>(log(mag_q));

		if (mag_V > 0) {
			float scale = static_cast<float>(acos(w / mag_q) / mag_V);

			ret.x = x * scale;
			ret.y = y * scale;
			ret.z = z * scale;
		}
		else
			ret.x = ret.y = ret.z = 0;

		return ret;
	}

	Quaternion Exp() const {
		Quaternion	ret;
		float		ea = static_cast<float>(exp(w));
		float		mag_V = static_cast<float>(sqrt(x*x + y*y + z*z));
		float		scale = ea * sin(mag_V) / mag_V;

		ret.w = ea * cos(mag_V);
		ret.x = scale * x;
		ret.y = scale * y;
		ret.z = scale * z;

		return ret;
	}

	Quaternion Lerp(Quaternion const & in_right, float in_fraction) const {
		Quaternion	ret = *this + in_fraction * (in_right - *this);
		return ret.Normalize();
	}


	Quaternion Slerp(Quaternion const & in_right, float in_fraction, bool in_shortest_path_only = true) const {
		Quaternion	q3;
		float		dot = x*in_right.x + y*in_right.y + z*in_right.z + w*in_right.w;

		if (in_shortest_path_only && dot < 0) {
			dot = -dot;
			q3 = -1 * in_right;
		}
		else
			q3 = in_right;

		if (dot > -0.95f && dot < 0.95f) {
			float	 angle = static_cast<float>(acos(dot));
			float	 sina = static_cast<float>(sin(angle));
			float	 sinat = static_cast<float>(sin(angle*in_fraction));
			float	 sinaomt = static_cast<float>(sin(angle*(1-in_fraction)));

			return (*this * sinaomt + q3 * sinat) / sina;
		}
		else	//if the angle is small, we use linear interpolation
			return Lerp(q3, in_fraction);
	}


	/*
	Spherical cubic interpolation between two Quaternions
	\param in_right The Quaternion for interpolation.
	\param in_control1 The first control point.
	\param in_control2 The second control point.
	\param in_fraction Interpolation distance (0 - 1).
	\return Result of interpolation.
	*/
	Quaternion Squad(Quaternion const & in_right, Quaternion const & in_control1, Quaternion const & in_control2, float in_fraction) const {
		Quaternion	q1 = Slerp(in_right , in_fraction, false);
		Quaternion	q2 = in_control1.Slerp(in_control2, in_fraction, false);

		return q1.Slerp(q2, 2*in_fraction*(1-in_fraction), false);
	}

	/*!
	Calculate a control point to be used in spline interpolation
	\param in_previous The control point before this
	\param in_next The control point after this
	\return Resultant spline control point
	*/
	Quaternion Spline(Quaternion const & in_previous, Quaternion const & in_next) const {
		Quaternion qni(-x, -y, -z, w);

		return *this * Quaternion(((qni*in_previous).Log() + (qni*in_next).Log()) / -4).Exp();
	}

};

} // HPS namespace

#endif

