// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef SPAN_DEFINED
#define SPAN_DEFINED


Begin_HOOPS_Namespace

#	 define MULU(a, b) ((a) * (b))

/*
 * A fixed point number is used to represent a 32 bit number, with
 * 16 bits of integral and 16 bit fractional precision.
 * Truncation is accomplished by merely accessing the integer part.
 * Arithmetic operations can be accomplished by accessing the entire
 * structure as a single unit (the "number" field).
 */
struct Fixed_Point_Bytes {
#	 if HOOPS_BIGENDIAN
		unsigned char	b3;
		unsigned char	b2;
		unsigned char	b1;
		unsigned char	b0;
#	 else
		unsigned char	b0;
		unsigned char	b1;
		unsigned char	b2;
		unsigned char	b3;
#	 endif
};

struct Fixed_Point_Parts {
#	if HOOPS_BIGENDIAN
		short				integer;		/* integral part, 16b.0b   */
		unsigned short		fraction;		/* fractional part, 0b.16b */
#	else
		unsigned short		fraction;		/* fractional part, 0b.16b */
		short				integer;		/* integral part, 16b.0b   */
#	endif
};


union Fixed_Point {
	int32_t						number;			/* entire number, 16b.16b  */
	struct Fixed_Point_Bytes bytes;
	struct Fixed_Point_Parts parts;
};


struct Fixed_Color{
	Fixed_Point		r;
	Fixed_Point		g;
	Fixed_Point		b;
};


#define LOAD_ARGUMENT(arg)	(void)(arg)
#define TRUNCATE_POINT(src, sindex, dst, dindex, field)		\
		dst[dindex].field = int(src[sindex].field)


#define TRUNCATE_3_POINTS(src, dst, n1, n2, n3, field)	\
		TRUNCATE_POINT (src, n1, dst, 0, field);				\
		TRUNCATE_POINT (src, n2, dst, 1, field);				\
		TRUNCATE_POINT (src, n3, dst, 2, field); 

#define TRUNCATE_2_POINTS(src, dst, n1, n2, field)				\
		TRUNCATE_POINT (src, n1, dst, 0, field);				\
		TRUNCATE_POINT (src, n2, dst, 1, field); 

#define PRIORITIZE_PTRS(src, dst, n1, n2, n3)	\
		dst[0] = &src[n1]; dst[1] = &src[n2]; dst[2] = &src[n3];

#define PRIORITIZE_RGBAS32(src, dst, n1, n2, n3) \
		dst[0].r = UNSIGNED_CHARTOINT(src[n1].r);	\
		dst[0].g = UNSIGNED_CHARTOINT(src[n1].g);	\
		dst[0].b = UNSIGNED_CHARTOINT(src[n1].b);	\
		dst[1].r = UNSIGNED_CHARTOINT(src[n2].r);	\
		dst[1].g = UNSIGNED_CHARTOINT(src[n2].g);	\
		dst[1].b = UNSIGNED_CHARTOINT(src[n2].b);	\
		dst[2].r = UNSIGNED_CHARTOINT(src[n3].r);	\
		dst[2].g = UNSIGNED_CHARTOINT(src[n3].g);	\
		dst[2].b = UNSIGNED_CHARTOINT(src[n3].b);

	
#define COLOR_TOLERANCE 4
#define RGBSAME(a,b,which) \
				((diff = a.which - b.which) < COLOR_TOLERANCE && diff > -COLOR_TOLERANCE)


#define SWAP_INT(a, b) { int _swap_temp_ = (a); (a) = (b); (b) = _swap_temp_; }

#define DITHER(thresh, c0, d1, c1, d2, c2, d3, c3) \
	(((d1) <= (thresh)) ? ((d2) <= (thresh)) ? ((d3) <= (thresh)) ? (c3) : (c2) : (c1) : (c0))

#define DIVIDE(numer, denom, quotient, remainder) \
	quotient = numer / denom; \
	remainder = numer % denom;


struct Span_Appearance {
	Light_Rendition				light_rendition;		// for reshading only
	Material_Rendition			material_rendition;		// for reshading only
	Driver_Color				color;
	int							color_system;	// defines type of "color" field
	int							pattern;
	Driver_Color				contrast_color; // matches dc->physical.color_system
	int32_t					z;
};



/*
 * Interpolant types
 */
 
struct UFixed_Point_Bytes {
#	if HOOPS_BIGENDIAN
		unsigned char	b3;
		unsigned char	b2;
		unsigned char	b1;
		unsigned char	b0;
#	else
		unsigned char	b0;
		unsigned char	b1;
		unsigned char	b2;
		unsigned char	b3;
#	endif
};

struct UFixed_Point_Parts {
#	if HOOPS_BIGENDIAN
		unsigned short		integer;		/* integral part, 16b.0b   */
		unsigned short		fraction;		/* fractional part, 0b.16b */
#	else
		unsigned short		fraction;		/* fractional part, 0b.16b */
		unsigned short		integer;		/* integral part, 16b.0b   */
#	endif
};

union UFixed_Point{
	uint32_t				 number;		 /* entire number, 16b.16b	*/
	UFixed_Point_Bytes bytes;
	UFixed_Point_Parts parts;
};


struct Fixed_Interpolant {
	Fixed_Point		val;
	Fixed_Point		slope;
};

struct UFixed_Interpolant {
	UFixed_Point		val;
	Fixed_Point		slope;
};

INLINE void SETUP_FIXED(Fixed_Interpolant &f, int t0, int t1, int dy)
{
	f.val.number = 0;							
	f.val.parts.fraction = 0x8000;				
	f.val.parts.integer = (short)t0;					
	f.slope.number = 0;							
	f.slope.parts.fraction = 0;					
	f.slope.parts.integer = (short)(t1 - t0);		
	switch (dy) {								
		case 0: break;							
		case 1: break;							
		case 2: f.slope.number >>= 1; break;	
		case 3: f.slope.number /=  3; break;	
		case 4: f.slope.number >>= 2; break;	
		case 5: f.slope.number /=  5; break;	
		case 6: f.slope.number /=  6; break;	
		case 7: f.slope.number /=  7; break;	
		case 8: f.slope.number >>= 3; break;	
		default: f.slope.number /= (int)dy;
	}
} 

INLINE void SETUP_FIXED(UFixed_Interpolant &f, int t0, int t1, int dy)
{
	f.val.number = 0;							
	f.val.parts.fraction = 0x8000;				
	f.val.parts.integer = (short)t0;					
	f.slope.number = 0;							
	f.slope.parts.fraction = 0;					
	f.slope.parts.integer = (short)(t1 - t0);		
	switch (dy) {								
		case 0: break;							
		case 1: break;							
		case 2: f.slope.number >>= 1; break;	
		case 3: f.slope.number /=  3; break;	
		case 4: f.slope.number >>= 2; break;	
		case 5: f.slope.number /=  5; break;	
		case 6: f.slope.number /=  6; break;	
		case 7: f.slope.number /=  7; break;	
		case 8: f.slope.number >>= 3; break;	
		default: f.slope.number /= (int)dy;
	}
} 

#define COPY_FIXED(s, d)						\
	(d).val.number		   = (s).val.number;	\
	(d).slope.number	   = (s).slope.number;

#define UPDATE_FIXED(f)							\
	f.val.number += f.slope.number

#define SKIP_FIXED(f,n)							\
	f.val.number += f.slope.number * (n)

#define HALF_STEP_FIXED(f)				\
	f.val.number += f.slope.number / 2




struct Floating_Interpolant {
	float				val;
	float				slope;
};

#define SETUP_FLOAT(f, t0, t1, dy)				\
	f.val = t0;									\
	f.slope = (t1) - (t0);						\
	if (dy != 0 && dy != 1)						\
		f.slope /= (float)dy;

#define COPY_FLOAT(s, d)						\
	(d).val				   = (s).val;			\
	(d).slope			   = (s).slope;

#define UPDATE_FLOAT(f)							\
	f.val += f.slope

#define SKIP_FLOAT(f,n)							\
	f.val += f.slope * (n)

#define HALF_STEP_FLOAT(f)						\
	f.val += 0.5f * f.slope


struct Double_Interpolant {
	double				val;
	double				slope;
};

#define SETUP_DOUBLE(f, t0, t1, dy)				\
	f.val = t0;									\
	f.slope = (t1) - (t0);						\
	if (dy != 0 && dy != 1)						\
		f.slope /= (float)dy;

#define COPY_DOUBLE(s, d)						\
	(d).val				   = (s).val;			\
	(d).slope			   = (s).slope;

#define UPDATE_DOUBLE(f)						\
	f.val += f.slope

#define SKIP_DOUBLE(f,n)						\
	f.val += f.slope * (n)

#define HALF_STEP_DOUBLE(f)						\
	f.val += 0.5f * f.slope




struct Bresenham_Interpolant {
	struct {
		int32_t	integer;
		int32_t	fraction;
		float			simple;
	} val;
	struct {
		struct {
			int32_t		integer;
			int32_t		fraction;
		} acc, cor; /* accumulation, correction */
		float			simple;
	} slope;
};

INLINE void SETUP_BRESENHAM(Bresenham_Interpolant &b, int t0, int t1, int dy)									
{	
	int dt = t1 - t0;
	int	delta = dy ? dy : 1;					
	b.val.simple = t0 + 0.5f;											
	b.slope.simple = (float)dt / (float)delta;						
	//DIVIDE (dt, delta, b.slope.acc.integer, b.slope.acc.fraction);
	b.slope.acc.integer = dt / delta;
	b.slope.acc.fraction = dt % delta;
	if (dt < 0) {														
		b.slope.acc.fraction = -(b.slope.acc.fraction << 1);			
		b.slope.cor.integer	 = b.slope.acc.integer - 1;					
	}																	
	else {																
		b.slope.acc.fraction = b.slope.acc.fraction << 1;				
		b.slope.cor.integer	 = b.slope.acc.integer + 1;					
	}																	
	b.slope.cor.fraction = b.slope.acc.fraction - (delta << 1);		
	b.val.integer = t0;													
	b.val.fraction = b.slope.acc.fraction - delta; 
}


#define COPY_BRESENHAM(s, d)							\
	(d).val.integer		   	= (s).val.integer;			\
	(d).val.fraction	   	= (s).val.fraction;			\
	(d).val.simple		   	= (s).val.simple;			\
	(d).slope.acc.integer  	= (s).slope.acc.integer;	\
	(d).slope.acc.fraction 	= (s).slope.acc.fraction;	\
	(d).slope.cor.integer  	= (s).slope.cor.integer;	\
	(d).slope.cor.fraction 	= (s).slope.cor.fraction;	\
	(d).slope.simple		= (s).slope.simple;

#define UPDATE_BRESENHAM(b)	 do {										\
	b.val.simple += b.slope.simple;										\
	if (b.val.fraction >= 0) {b.val.integer	 += b.slope.cor.integer;	\
							  b.val.fraction += b.slope.cor.fraction;}	\
	else					 {b.val.integer	 += b.slope.acc.integer;	\
							  b.val.fraction += b.slope.acc.fraction;}	\
	} while (0)

#define SKIP_BRESENHAM(b,n) do {	\
		int _n_ = (n);		\
		while (_n_-- > 0) {			\
			UPDATE_BRESENHAM (b);	\
		}							\
	} while (0)

#define HALF_STEP_BRESENHAM(b) do {											\
	b.val.simple += 0.5f * b.slope.simple;							\
	if (b.val.fraction >= 0) {b.val.integer	 += b.slope.cor.integer/2;	\
							  b.val.fraction += b.slope.cor.fraction/2;}\
	else					 {b.val.integer	 += b.slope.acc.integer/2;	\
							  b.val.fraction += b.slope.acc.fraction/2;}	\
	} while (0)



/*
 * a span anchor is an edge of sorts,
 * and therefore contains all associated interpolants
 */
#define SPAN_PARAM_COUNT 64

struct Span_Anchor {
	int							dy;				/* height of the side, in pixels, minus 1 */

	Fixed_Interpolant		x;				/* horizontal position */
	Fixed_Interpolant		y;				/* vertical position */

	Bresenham_Interpolant	bx;				/* can deal with unclipped coordinates */
	Bresenham_Interpolant	bz;				/* depth (32 bits or less) */

	Fixed_Interpolant		r;				/* color red */
	Fixed_Interpolant		g;				/* color green */
	Fixed_Interpolant		b;				/* color blue */

	/* everything below for phong and/or textures, individual parts may be present or not */
	Floating_Interpolant		fr;				/* floating point red */
	Floating_Interpolant		fg;				/* floating point green */
	Floating_Interpolant		fb;				/* floating point blue */

	Floating_Interpolant		pa;				/* plane a */
	Floating_Interpolant		pb;				/* plane b */
	Floating_Interpolant		pc;				/* plane c */
	Floating_Interpolant		pd;				/* plane d */
	Double_Interpolant			dpa;			/* dplane a */
	Double_Interpolant			dpb;			/* dplane b */
	Double_Interpolant			dpc;			/* dplane c */
	Double_Interpolant			dpd;			/* dplane d */


	Fixed_Interpolant		uu;
	Fixed_Interpolant		vv;


	Floating_Interpolant		wx;				/* world x */
	Floating_Interpolant		wy;				/* world y */
	Floating_Interpolant		wz;				/* world z */

	Floating_Interpolant		numwx;			/* perspective correct numerator x */
	Floating_Interpolant		numwy;			/* perspective correct numerator y */
	Floating_Interpolant		numwz;			/* perspective correct numerator z */
	float						startwx, startwy, startwz;	/* perspective correct start values */

	int							param_width;
	Floating_Interpolant		param[SPAN_PARAM_COUNT];			/* parameter */
	Floating_Interpolant		num_param[SPAN_PARAM_COUNT];		/* perspective correct numerators */
	float						start_param[SPAN_PARAM_COUNT];		/* perspective correct start values */

	Floating_Interpolant		homog;	/* Homogeneous coord */
	Floating_Interpolant		den;	/* perspective correct denominator */
};


struct Anchor_X {
	int							dy;
	Fixed_Interpolant		x;				/* horizontal position */
};

struct Anchor_XZ {
	int							dy;
	Fixed_Interpolant		x;				/* horizontal position */
	Bresenham_Interpolant	bz;				/* depth (32 bits or less) */
};


/*
 * SPAN_BUFFER_SIZE *must* be at least 2
 */

#define SPAN_BUFFER_SIZE 64


struct Span {
	Span_Anchor						left, right;

#define	Span_RGB		0x01
#define	Span_PLANE		0x02
#define	Span_PARAMETERS	0x04	/* and all associated stuff like homog & num/den */
#define	Span_WORLD		0x08	/* and numw if appropriate */
#define	Span_DPLANE		0x10
	int							valid;
};


#define C08PP	unsigned char * *
#define C32PP	RGBAS32 * *
#define DepthPP	Depth * *

End_HOOPS_Namespace

#endif /* SPAN_DEFINED */
