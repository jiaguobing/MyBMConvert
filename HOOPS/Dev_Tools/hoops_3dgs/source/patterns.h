// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

/*** C++ STYLE COMMENTS PROHIBITED IN THIS FILE ***/



#ifndef PATTERNS_DEFINED

/* DO NOT ADD ANY INCLUDES TO THIS FILE */
/* DO NOT USE ANY  TYPES IN THIS FILE */

/* IM user face patterns */
#define UPAT_NORMAL				0
#define UPAT_NO_FACE			1
#define UPAT_NO_CONTRAST		2


/* Legacy Edge and line patterns */
#define LP_SOLID				0
#define LP_DASHDOT				1
#define LP_DASHED				2
#define LP_DOTTED				3
#define LP_DASH_2DOT			4
#define LP_DASH_3DOT			5
#define LP_LONG_DASH			6
#define LP_CENTER				7
#define LP_PHANTOM				8
#define LP_FINEDOT				9


#define LCAP_BUTT				0x0000
#define LCAP_ROUND				0x0001
#define LCAP_SQUARE				0x0002
#define LCAP_TRIANGLE			0x0004
#define LCAP_END_ROUND			0x0010
#define LCAP_END_SQUARE			0x0020
#define LCAP_END_TRIANGLE		0x0040
#define LCAP_END_BUTT			0x0080
#define LCAP_START_MASK			0x000F
#define LCAP_END_MASK			0x00F0
#define LCAP_MASK				0x00FF
#define LCAP_SHIFT				0
#define LCAP_END_SHIFT			4
#define LCAP_COMPLEX_BITS		(LCAP_TRIANGLE|LCAP_END_ROUND|LCAP_END_SQUARE|LCAP_END_TRIANGLE|LCAP_END_BUTT)

#define LJOIN_MITER				0x0000
#define LJOIN_ROUND				0x0100
#define LJOIN_BEVEL				0x0200
#define LJOIN_MASK				0x0F00
#define LJOIN_SHIFT				8

#define LPAT_BUTT				0x0000
#define LPAT_ROUND				0x1000
#define LPAT_SQUARE				0x2000
#define LPAT_TRIANGLE			0x4000
#define LPAT_MASK				0xF000
#define LPAT_SHIFT				12

/* Polygon fill patterns */
#ifndef FP_SOLID
#	define		FP_SOLID		0		/* must match driver.h */
#endif
#define FP_CHECKERBOARD					1
#define FP_CROSSHATCH					2
#define FP_DIAMONDS						3
#define FP_HORIZONTAL_BARS				4
#define FP_SLANT_LEFT					5
#define FP_SLANT_RIGHT					6
#define FP_SQUARE_DOTS					7
#define FP_VERTICAL_BARS				8
#define FP_WINDOW_CHECKERBOARD			9
#define FP_WINDOW_CROSSHATCH			10
#define FP_WINDOW_DIAMONDS				11
#define FP_WINDOW_HORIZONTAL_BARS		12
#define FP_WINDOW_SLANT_LEFT			13
#define FP_WINDOW_SLANT_RIGHT			14
#define FP_WINDOW_SQUARE_DOTS			15
#define FP_WINDOW_VERTICAL_BARS			16

#define FP_WINDOW_TRANSPARENT			17		/* not really a pattern - has no bits */

#define FP_WINDOW_FIRST_GRADIENT		18
#define FP_WINDOW_GRADIENT_N			18
#define FP_WINDOW_GRADIENT_NE			19
#define FP_WINDOW_GRADIENT_E			20
#define FP_WINDOW_GRADIENT_SE			21
#define FP_WINDOW_GRADIENT_S			22
#define FP_WINDOW_GRADIENT_SW			23
#define FP_WINDOW_GRADIENT_W			24
#define FP_WINDOW_GRADIENT_NW			25
#define FP_WINDOW_LAST_GRADIENT			25

#define FP_BLEND						26

#define FP_WINDOW_TRANS_NO_ZCLEAR		27

#define FP_USER_DEFINED					127


/* Legacy Marker symbols */
#define MK_CIRCLE						0
#define MK_CIRCLE_WITH_DOT				1
#define MK_CIRCLE_WITH_CROSS			2
#define MK_CIRCLE_WITH_TIMES			3
#define MK_CIRCLE_WITH_CIRCLE			4
#define MK_SOLID_CIRCLE					5
#define MK_DOT							6
#define MK_CROSS						7
#define MK_SPLAT						8
#define MK_TIMES						9
#define MK_BOX							10
#define MK_BOX_WITH_DOT					11
#define MK_BOX_WITH_TIMES				12
#define MK_SOLID_BOX					13
#define MK_DIAMOND						14
#define MK_DIAMOND_WITH_DOT				15
#define MK_DIAMOND_WITH_CROSS			16
#define MK_SOLID_DIAMOND				17
#define MK_TRIANGLE_UP					18
#define MK_TRIANGLE_UP_V				19
#define MK_TRIANGLE_UP_WITH_DOT			20
#define MK_SOLID_TRIANGLE_UP			21
#define MK_SOLID_TRIANGLE_UP_V			22
#define MK_TRIANGLE_DOWN				23
#define MK_TRIANGLE_DOWN_V				24
#define MK_TRIANGLE_DOWN_WITH_DOT		25
#define MK_SOLID_TRIANGLE_DOWN			26
#define MK_SOLID_TRIANGLE_DOWN_V		27
#define MK_TRIANGLE_RIGHT				28
#define MK_TRIANGLE_RIGHT_V				29
#define MK_TRIANGLE_RIGHT_WITH_DOT		30
#define MK_SOLID_TRIANGLE_RIGHT			31
#define MK_SOLID_TRIANGLE_RIGHT_V		32
#define MK_TRIANGLE_LEFT				33
#define MK_TRIANGLE_LEFT_V				34
#define MK_TRIANGLE_LEFT_WITH_DOT		35
#define MK_SOLID_TRIANGLE_LEFT			36
#define MK_SOLID_TRIANGLE_LEFT_V		37
#define MK_POUND						38
#define MK_WIDE_PLUS					39
#define MK_OPEN_ARROW					40
#define MK_CLOSED_ARROW					41
#define MK_VERTICAL_BAR					42
#define MK_HALF_ARROW_LEFT				43
#define MK_HALF_ARROW_RIGHT				44
#define MK_WIDE_ARROW					45
#define MK_DOUBLE_ARROW					46
#define MK_LETTER_Y						47
#define MK_LETTER_Z						48


/* Text [block] alignment */
#define TALIGN_CENTER			0x00
#define TALIGN_LEFT				0x01
#define TALIGN_RIGHT			0x02
#define TALIGN_LOWER			0x04
#define TALIGN_UPPER			0x08
#define TALIGN_INSERTION_POINT	0x10
#define TALIGN_HORIZONTAL_MASK	0x03
#define TALIGN_VERTICAL_MASK	0x0C
#define TALIGN_UNDEFINED		0x7F

#define TA_CENTERCENTER			0x00
#define TA_CENTERLEFT			0x01
#define TA_CENTERRIGHT			0x02
#define TA_LOWERCENTER			0x04
#define TA_LOWERLEFT			0x05
#define TA_LOWERRIGHT			0x06
#define TA_UPPERCENTER			0x08
#define TA_UPPERLEFT			0x09
#define TA_UPPERRIGHT			0x0A
#define TA_INSERTION_POINT		0x10

/* Text justification (string alignment) goes in "extra" */
#define TJUST_LEFT				0
#define TJUST_RIGHT				1
#define TJUST_CENTER			2
#define TJUST_UNDEFINED			0x0F



/* Fonts */
#define TIMES_ROMAN		0
#define MAX_FONT		0

#	define		FT_NOT_VALID			0x00
#	define		FT_NONE					0x01
#	define		FT_POSITION_ONLY		0x02
#	define		FT_POSITION_ADJUSTED	0x03
#	define		FT_FULL					0x04

#define PATTERNS_DEFINED
#endif
