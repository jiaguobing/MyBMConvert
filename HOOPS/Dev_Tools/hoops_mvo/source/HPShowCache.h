// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef _HPSHOWCACHE_H
#define _HPSHOWCACHE_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"
#include "HGlobals.h"
#include "vhash.h"


#define PSHOW_CACHE_MODELLING_MATRIX	0x00000001
#define PSHOW_CACHE_VISIBILITY			0x00000002
#define PSHOW_CACHE_RENDERING_OPTIONS	0x00000004
#define PSHOW_CACHE_HANDEDNESS			0x00000008
#define PSHOW_CACHE_HEURISTICS			0x00000010
#define PSHOW_CACHE_TEXT_FONT			0x00000020
#define PSHOW_CACHE_TEXT_ALIGNMENT		0x00000040
#define PSHOW_CACHE_TEXT_PATH			0x00000080
#define PSHOW_CACHE_TEXT_SPACING		0x00000100
#define PSHOW_CACHE_MARKER_SIZE			0x00000200
#define PSHOW_CACHE_MARKER_SYMBOL		0x00000400
#define PSHOW_CACHE_LINE_WEIGHT			0x00000800
#define PSHOW_CACHE_LINE_PATTERN		0x00001000
#define PSHOW_CACHE_COLOR				0x00002000
#define PSHOW_CACHE_EDGE_WEIGHT			0x00004000
#define PSHOW_CACHE_EDGE_PATTERN		0x00008000
#define PSHOW_CACHE_CAMERA				0x00010000
#define PSHOW_CACHE_IMAGE_SCALE			0x00020000
#define PSHOW_CACHE_HIDDEN_LINE_HSRA	0x00040000
#define PSHOW_CACHE_POLYGON_HANDEDNESS	0x00080000
#define PSHOW_CACHE_TRANS_COLOR			0x00100000
#define PSHOW_CACHE_COLORMAP			0x00200000
#define PSHOW_CACHE_CLIP_REGION			0x00400000
#define PSHOW_CACHE_CONDITIONS			0x00800000



class HPShowCache
{
public:
	HPShowCache():flags(0), path(0), modelling_matrix(0), 
		clip_region_loops(0), clip_region_spec(0), clip_region_lengths(0), clip_region_points(0),
		visibility(0), conditions(0), rendering_options(0), handedness(0), heuristics(0),
		text_font(0), text_alignment(0), text_spacing(1.f), marker_size(0), marker_symbol(0),
		line_weight(0), line_pattern(0), color(0), color_map(0), edge_weight(0), 
		edge_pattern(0), camera_position(0,0,0), camera_target(0,0,0), camera_up(0,0,0), 
		camera_width(0.f), camera_height(0.f), camera_projection(0),
		text_path_x(1.f), text_path_y(0), text_path_z(0),
		screen_range(0), depth_range(0), tessellation(0), general_curve(0), nurbs_surface(0), general_displacement(0), mask_transform(0),
		anti_alias(0), image_scale(0), polygon_handedness(0), trans_color(0),
		hidden_line_hsra(false), have_text_path(false), have_text_spacing(false)
	{
	}

	~HPShowCache()
	{
		delete [] path;
		delete [] visibility;
		delete [] conditions;
		delete [] clip_region_spec;
		delete [] clip_region_points;
		delete [] clip_region_lengths;
		delete [] rendering_options;
		delete [] handedness;
		delete [] heuristics;
		delete [] text_font;
		delete [] text_alignment;
		delete [] marker_size;
		delete [] marker_symbol;
		delete [] line_weight;
		delete [] line_pattern;
		delete [] color;
		delete [] color_map;
		delete [] edge_weight;
		delete [] edge_pattern;	
		delete [] camera_projection;
		delete [] screen_range;
		delete [] depth_range;
		delete [] tessellation;
		delete [] general_curve;
		delete [] nurbs_surface;
		delete [] general_displacement;	
		delete [] mask_transform;	
		delete [] anti_alias;
		delete [] image_scale;
		delete [] polygon_handedness;	
		delete [] trans_color;	
	}

	unsigned int flags;

	HC_KEY*	path;
	int		path_count;

	float	modelling_matrix_data[16];
	float * modelling_matrix;

	int		clip_region_loops;
	char *	clip_region_spec;
	int	*	clip_region_lengths;
	HPoint *clip_region_points;

	char *	visibility;
	char *	conditions;
	char *	rendering_options;
	char *	handedness;
	char *	heuristics;
	char *	text_font;
	char *	text_alignment;
	
	float	text_spacing;
	char *	marker_size;
	char *	marker_symbol;
	char *	line_weight;
	char *	line_pattern;
	char *	color;
	char *	color_map;
	char *	edge_weight;
	char *	edge_pattern;
	HPoint	camera_position;
	HPoint	camera_target;
	HPoint	camera_up;
	float	camera_width;
	float	camera_height;
	char *	camera_projection;

	float	text_path_x, text_path_y, text_path_z;
	char *	screen_range;
	char *	depth_range;
	char *	tessellation;
	char *	general_curve;
	char *	nurbs_surface;
	char *	general_displacement;
	char *	mask_transform;
	char *	anti_alias;
	char *	image_scale;
	char *	polygon_handedness;
	char *	trans_color;

	bool	hidden_line_hsra;
	bool	have_text_path;
	bool	have_text_spacing;
};

//typedef VHash<HC_KEY, HPShowCache *> HVHashPShowCache;

class HVHashPShowCache: public VHash<HC_KEY, HPShowCache *>
{

};


class HConditionStyleInfo {
public:
	HC_KEY	hightlight_style;
	HC_KEY	condition_style;
	HC_POINTER_SIZED_INT use_count;

	HConditionStyleInfo(HC_KEY	HightlightStyle): hightlight_style(HightlightStyle), use_count(0) {};
	~HConditionStyleInfo() {};
};

class HConditionStyleInfoHash: public VStringHash<HConditionStyleInfo *> {
public:
	HConditionStyleInfoHash():VStringHash<HConditionStyleInfo *>(2) {};
};

class HConditionStyleKeyInfoHash: public VHash<HC_KEY, HConditionStyleInfo *> {};

class HSelectionSegmentConditionInfo: public VHash<HC_KEY, HConditionStyleInfoHash *> {};


#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif



















