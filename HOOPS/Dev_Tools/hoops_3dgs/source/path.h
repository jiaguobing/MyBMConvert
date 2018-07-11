// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef PATH_DEFINED

Begin_HOOPS_Namespace

#define EXPAND			1		/* expand referenced aliases */
#define WILDCARDS		2		/* *,%... are OK in general */
#define QUALIFIERS		4		/* {}<>! are OK */

struct Path_Token {
	Path_Token *	next;
	int				type;
	Name			letters;		/* if type = PATH_LETTERS */
	Path_Token *	expansion;		/* if type = PATH_ALIASREF */
												/* (only valid during parse!) */
	Key				key;			/* if type = PATH_KEYREF */
	int				level;			/* if type == PATH_INCLUDE* or
															  PATH_STYLE* */
	bool			seen_wildcard;
};

struct Path_Token_List {
	Path_Token_List *	prev;
	Path_Token *		tok;
};


#define PATH_DOT				1
#define PATH_SLASH				2
#define PATH_PERCENT			3
#define PATH_UPARROW			4
#define PATH_ALIASREF			5
#define PATH_COMMA				6
#define PATH_LEFTP				7
#define PATH_RIGHTP				8
#define PATH_LETTERS			9
#define PATH_SPLAT				10
#define PATH_STYLES				11
#define PATH_STYLED_BY			12
#define PATH_INCLUDES			13
#define PATH_INCLUDED_BY		14
#define PATH_EXCEPT				15
#define PATH_KEYREF				16
#define PATH_RECURSE			17
#define PATH_RECURSE_INCLUDES	18

End_HOOPS_Namespace

#define PATH_DEFINED
#endif
