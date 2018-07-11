// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// HSelectionItem.cpp : implementation of the HSelectionItem class
//

#include <stdio.h>
#include <string.h>

#ifdef _DEBUG
#  include <assert.h>
#  define ASSERT(x) assert(x);
#else
#  define ASSERT(x)
#endif

#include "HSelectionItem.h"
#include "HSelectionSet.h"
#include "HUtility.h"
#include "HUtilityAnnotation.h"
#include "HBaseView.h"
#include "HPShowCache.h"
#include "HEventManager.h"

#include "vlist.h"
#include "vhash.h"

#include <algorithm>



static char const base_entity_proxy_seg_name[] = "mvo_thls";
static char const entity_proxy_tag_seg_name[] = "mvo_tags";



static void validate_include_path(HC_KEY selection_key, int *count, HC_KEY ** keys, bool is_ref_sel, HC_KEY view_key, HC_KEY incl_link_key)
{
	int num_include_keys = *count;
	bool child_of_view = (selection_key == view_key);
	HC_KEY tmp_key = selection_key;

	while((tmp_key = HC_Show_Owner_Original_Key(tmp_key)) != INVALID_KEY)
	{
		if(tmp_key == view_key)
		{
			child_of_view = true;
			break;
		}
	}

	if(is_ref_sel)
	{
		//reference highlighting requires the view and the include link
		if(num_include_keys==0)
		{
			HC_KEY * seg_path = 0;

			if(child_of_view)
			{
				num_include_keys = 1;
				seg_path = new HC_KEY[num_include_keys];

				seg_path[0]=view_key;
			}
			else
			{
				num_include_keys = 2;
				seg_path = new HC_KEY[num_include_keys];

				seg_path[0]=view_key;
				seg_path[1]=incl_link_key;
			}			
			
			delete [] *keys;
			*keys = seg_path;
			*count = num_include_keys;
		}
		else
		{
			bool add_view=false, add_incl_link=false;
			int add_count=0;

			if ((*keys)[0]!=view_key)
			{
				add_view=true;
				++add_count;
			}
			if ((*keys)[0]!=incl_link_key && (num_include_keys==1 || (num_include_keys>1 && (*keys)[1]!=incl_link_key)) && !child_of_view)
			{
				add_incl_link=true;
				++add_count;
			}

			if(add_view || add_incl_link)
			{
				HC_KEY * seg_path = new HC_KEY[num_include_keys+add_count];
				int i;

				seg_path[0]=view_key;
				seg_path[1]=incl_link_key;
				for(i=2; i-add_count<num_include_keys; ++i)
					seg_path[i] = (*keys)[i-add_count];

				num_include_keys+=add_count;
				delete [] *keys;
				*keys = seg_path;
				*count = num_include_keys;
			}
		}
	}
	else
	{
		//default (conditional) highlighting requires the include link and no view
		if(num_include_keys==0)
		{
			if(!child_of_view)
			{
				num_include_keys = 1;
				HC_KEY * seg_path = new HC_KEY[num_include_keys];

				seg_path[0]=incl_link_key;
				
				delete [] *keys;
				*keys = seg_path;
				*count = num_include_keys;
			}
		}
		else
		{
			bool remove_view=false, add_incl_link=false;

			if ((*keys)[0]==view_key)
				remove_view=true;
			if ((*keys)[0]!=incl_link_key && (num_include_keys==1 || (num_include_keys>1 && (*keys)[1]!=incl_link_key)) && !child_of_view)
				add_incl_link=true;

			if(remove_view || add_incl_link)
			{
				int mod = (remove_view && add_incl_link) ? 0 : (remove_view ? -1 : 1);

				int new_number = num_include_keys+mod;

				HC_KEY * seg_path = 0;

				if (new_number > 0)
				{
					seg_path = new HC_KEY[new_number];

					seg_path[0]=incl_link_key;
					for(int i=1; i-mod<num_include_keys; ++i)
						seg_path[i] = (*keys)[i-mod];
				}

				num_include_keys = new_number;

				delete [] *keys;
				*keys = seg_path;
				*count = num_include_keys;
			}
		}
	}
}

static char * get_key_tag(HC_KEY key, char * retTag)
{
	const char hex[] = "0123456789abcdef";
	for(int i = 0; i < (int)(2*sizeof(HC_KEY)); i++) { 
		*retTag++ = hex[(int)(key&0x0F)];
		key>>=4;
	}

	*retTag = '\0';
	return retTag;
}

static void get_base_tag(HC_KEY key, const int incl_count, const HC_KEY * incl_keys, char * retTag)
{
	
	const char * prefix = "mvo_select";
	
	while ((*retTag++ = *prefix++) != 0) {
		// empty
	}
	retTag--;

	for(int i = 0; i <= incl_count; i++)
	{

		*retTag++ = '_';

		retTag = get_key_tag(key, retTag);

		if(i < incl_count)
			key = incl_keys[i];
	}

	*retTag = '\0';
}


/////////////////////////////////////////////////////////////////////////////
// HSelectionItem
//  An interface to a selection item held in a list the HSelectionSet class. This class is meant to be derived 
//  from and have it's virtual function implemented

HSelectionItem::HSelectionItem(HC_KEY key, HSelectionSet * selection_set)
	: m_key(key)
	, m_pSelectionSet(selection_set)
	, m_FullPath(0)
	, m_FullPathCount(0)
{
}


HSelectionItem::~HSelectionItem()
{
}


bool HSelectionItem::Equals(const HSelectionItem * i2) const
{
	// if primary selection key is same
	if( this->GetKey() != i2->GetKey() )
		return false;

	// return false if they are of different type
	if( !streq(this->GetName(), i2->GetName()) )
		return false;

	 /*TODO: may be this should just always return false*/

	return true;
}


HC_KEY HSelectionItem::GetSegmentKey() const
{
	char keytype[MVO_BUFFER_SIZE];
	HC_KEY seg_key;

	HC_Show_Key_Type(m_key, keytype);

	if (streq(keytype, "segment")) 
		seg_key = m_key;
	else
	{
		// this is entity selection, check if highlighted
		seg_key = HC_Show_Owner_Original_Key(m_key);
		if( IsHighlighted() )
			// walk one more level up
			seg_key = HC_Show_Owner_Original_Key(seg_key);
	}
	return seg_key;
}


const HC_KEY * HSelectionItem::GetFullPathKeys(HBaseView * view)
{
	if (!m_FullPath) { 
		int keycount;
		HC_KEY temparray[512], key, limit;

		key = m_key;
		limit = HC_Create_Segment("/"); //doesn't actually create anything-- just gets the key
		keycount = 0;
		while (keycount < 510) {
			temparray[ keycount++ ] = key;
			key = HC_Show_Owner_Original_Key (key);
			if (key == view->GetModelKey() || key == limit)
				break;
		}
		temparray[ keycount++ ] = view->GetModelKey();
		temparray[ keycount++ ] = key = view->GetIncludeLinkKey();
		while (keycount < 512) {
			key = HC_Show_Owner_Original_Key(key);
			temparray[ keycount++ ] = key;
			if (key == view->GetViewKey() || key == limit)
				break;
		}
		m_FullPathCount = keycount;
		m_FullPath = new HC_KEY[ keycount ];
		memcpy(&m_FullPath[0], &temparray[0], keycount*sizeof(HC_KEY));
	}
	return m_FullPath;
}

void HSelectionItem::GetFullPathKeys (HBaseView *view, HC_KEY *path) {
	const HC_KEY *temp = GetFullPathKeys(view);
	int path_size = GetFullPathCount(view);

	memcpy(path, temp, path_size * sizeof(HC_KEY));
}


int HSelectionItem::GetFullPathCount (HBaseView *view)
{
	GetFullPathKeys (view);
	return m_FullPathCount;
}





/////////////////////////////////////////////////////////////////////////////
// HSmartSelItem
/*!
  Implementation of HSelectionItem class which handles instanced segments and entities. 
*/

enum HHighlightObjectType
{
	HHighlightSegment,
	HHighlightGeometry,
	HHighlightRegion,
	HHighlightSubentity
};

struct HSubentityPassthrough
{
	int face_count;
	int * faces;
	int vertex_count;
	int * vertex1;
	int * vertex2;
	bool maintain_maps;
	int vertex_map_count;
	SubentityMap * vertex_map;
	int face_map_count;
	SubentityMap * face_map;
	int highlight_subentity;
};

#define HTYPE_NONE				0x0000
#define HTYPE_TEXT				0x0001
#define	HTYPE_MARKER			0x0002
#define	HTYPE_LINE				0x0004
#define	HTYPE_SHELL_MESH		0x0008
#define	HTYPE_POLYGON			0x0010
#define	HTYPE_IMAGE				0x0020
#define	HTYPE_UNKNOWN			0x0040
#define	HTYPE_EVERYTHING		(HTYPE_TEXT|HTYPE_MARKER|HTYPE_LINE|HTYPE_SHELL_MESH|HTYPE_POLYGON|HTYPE_IMAGE)

#define HSPECIAL_NOTHING		0x0000
#define HSPECIAL_CAMERA			0x0001
#define HSPECIAL_MATRIX			0x0002

#define HGROUP_TRANSMISSION			0x00000001
#define HGROUP_HANDEDNESS			0x00000002
#define HGROUP_ANTIALIAS			0x00000004
#define HGROUP_COLORMAP				0x00000008
#define HGROUP_COLOR				0x00000010
#define HGROUP_SCREEN_RANGE			0x00000020
#define HGROUP_DEPTH_RANGE			0x00000040
#define HGROUP_IMAGE_SCALE			0x00000080
#define HGROUP_GENERAL_DISPLACEMENT	0x00000100
#define HGROUP_VISIBILITY			0x00000200
#define HGROUP_HIDDEN_LINE_HSRA		0x00000400
#define HGROUP_LINE_STUFF			0x00000800
#define HGROUP_MARKER_STUFF			0x00001000
#define HGROUP_EDGE_STUFF			0x00002000
#define HGROUP_TEXT_STUFF			0x00004000
#define HGROUP_MASK_TRANSFORM		0x00008000
#define HGROUP_TESSELLATION			0x00010000
#define HGROUP_CLIP_REGION			0x00020000
#define HGROUP_GENERAL_CURVE		0x00040000
#define HGROUP_NURBS_SURFACE		0x00080000
#define HGROUP_CONDITIONS			0x00100000




//from driver.h
#define RED_GRAYNESS			0.3125f
#define GREEN_GRAYNESS			0.5000f
#define BLUE_GRAYNESS			0.1875f

static bool prepare_and_validate_ppath(
	HC_KEY &				key,
	int	const				in_incl_count,
	const HC_KEY * const	incl_path, 
	HSelectionSet *			selection,
	HHighlightObjectType	hl_type,
	char *					type_string,
	HC_KEY &				seg_key,
	int &					path_count,
	HC_KEY *				path,
	bool					defined=false)
{
	int	incl_count = in_incl_count;

	UNREFERENCED(hl_type);

	if(!defined && hl_type==HHighlightSegment && incl_count != 0)
	{
		char temp_type[128];
		HC_Show_Key_Type(incl_path[incl_count-1], temp_type);
		if(streq(temp_type, "include"))
		{
			HC_KEY	tmp_key = HC_Show_Include_Segment(incl_path[incl_count-1], 0);
			if(tmp_key == key)
			{
				seg_key = HC_Show_Owner_Original_Key(incl_path[incl_count-1]);
				--incl_count;
			}
		}
	}

	HC_Show_Key_Type(key, type_string);

	if(seg_key == INVALID_KEY) {
		if (defined && streq(type_string, "segment"))
			seg_key = key;
		else
			seg_key = HC_Show_Owner_Original_Key(key);
	}

	if(streq(type_string, "reference"))
	{
		HC_KEY reverse_ref_path[MVO_BUFFER_SIZE];
		int reverse_ref_count = 0;
		while(streq(type_string, "reference")) {
			reverse_ref_path[reverse_ref_count++] = key;
			key=HC_Show_Reference_Geometry(key);
			HC_Show_Key_Type(key, type_string);
		}
		do {
			path[path_count++] = reverse_ref_path[--reverse_ref_count];
		} while (reverse_ref_count);
	}

	path[path_count++]=seg_key;
	for(int i=0; i<incl_count; ++i) {

		char temp_type[128];
		int index = incl_count-i-1;
		HC_Show_Key_Type(incl_path[index], temp_type);
		if(streq(temp_type, "include"))
			path[path_count++]=incl_path[index];
	}


	//-- Check if this is an object key (manipulator handle, annotation) --
	char obj_type[512];
	HC_KEY obj_key=HObjectManager::FindHObjectSegment(seg_key, obj_type);
	if(obj_key!=seg_key || strstr(obj_type, "Annotation"))
		return false;
	//-- Done checking for objects --

	//-- Set up the path for PShow --

	path[path_count] = path[path_count-1];

	HC_KEY	view_key = selection->GetView()->GetViewKey();
	HC_KEY	overwrite_key = selection->GetView()->GetOverwriteKey();
	HC_KEY	root_key = HC_Create_Segment("/");

	if (path[path_count] != view_key &&
		path[path_count] != root_key) {

			while (1) {

				HC_KEY owner_key = HC_Show_Owner_Original_Key(path[path_count]);

				if (owner_key == overwrite_key ||
					owner_key == view_key ||
					owner_key == root_key)
					break;

				path[path_count] = owner_key;
			}
	}

	path_count++;
	path[path_count++] = INVALID_KEY;

	return true;
}



static HC_KEY ref_highlight_common(
	HC_KEY const			key,
	HC_KEY const			seg_key,
	int	const				path_count,
	const HC_KEY * const	path, 
	HSelectionSet *			selection,
	HHighlightObjectType	hl_type,
	char const *			type_string,
	int						region = -1,
	void *					data = 0)
{
	//-- set has_type --
	int	has_type_flags = HTYPE_NONE;

	if(hl_type==HHighlightRegion || hl_type==HHighlightSubentity)
		has_type_flags=HTYPE_SHELL_MESH;
	else 
	{

		if (hl_type==HHighlightSegment) {

			if (HC_Show_Existence_By_Key(key, "text, first match"))
				has_type_flags |= HTYPE_TEXT;

			if (HC_Show_Existence_By_Key(key, "marker, first match"))
				has_type_flags |= HTYPE_MARKER;

			if (HC_Show_Existence_By_Key(key, "circular arc, elliptical arc, infinite line, infinite ray, line, nurbs curve, polyline, first match"))
				has_type_flags |= HTYPE_LINE;

			if (HC_Show_Existence_By_Key(key, "shell, mesh, grid, first match"))
				has_type_flags |= HTYPE_SHELL_MESH;

			if (HC_Show_Existence_By_Key(key, "polygon, circle, circular chord, circular wedge, cylinder, ellipse, nurbs surface, polycylinder, sphere, first match"))
				has_type_flags |= HTYPE_POLYGON;

			if (HC_Show_Existence_By_Key(key, "image, first match"))
				has_type_flags |= HTYPE_IMAGE;

		}
		else {

			switch (type_string[0]) {
			case 'c':
				if (streq (type_string, "circle"))
					has_type_flags = HTYPE_POLYGON;
				else if (streq (type_string, "circular arc"))
					has_type_flags = HTYPE_LINE;
				else if (streq (type_string, "circular chord"))
					has_type_flags = HTYPE_POLYGON;
				else if (streq (type_string, "circular wedge"))
					has_type_flags = HTYPE_POLYGON;
				else if (streq (type_string, "cutting plane"))
					has_type_flags = HTYPE_UNKNOWN;
				else if (streq (type_string, "cutting section"))
					has_type_flags = HTYPE_UNKNOWN;
				else if (streq (type_string, "cylinder"))
					has_type_flags = HTYPE_POLYGON;
				break;
			case 'e':
				if (streq (type_string, "ellipse"))
					has_type_flags = HTYPE_POLYGON;
				else if (streq (type_string, "elliptical arc"))
					has_type_flags = HTYPE_LINE;
				break;
			case 'g':
				if (streq (type_string, "grid"))
					has_type_flags = HTYPE_SHELL_MESH;
				break;
			case 'i':
				if (streq (type_string, "image"))
					has_type_flags = HTYPE_IMAGE;
				else if (streq (type_string, "infinite line"))
					has_type_flags = HTYPE_LINE;
				else if (streq (type_string, "infinite ray"))
					has_type_flags = HTYPE_LINE;
				break;
			case 'l':
				if (streq (type_string, "line"))
					has_type_flags = HTYPE_LINE;
				else {
					if (streq (type_string, "area light"))
						has_type_flags = HTYPE_UNKNOWN;
					else if (streq (type_string, "distant light"))
						has_type_flags = HTYPE_UNKNOWN;
					else if (streq (type_string, "local light"))
						has_type_flags = HTYPE_UNKNOWN;
					else if (streq (type_string, "spot light"))
						has_type_flags = HTYPE_UNKNOWN;
				}
				break;
			case 'm':
				if (streq (type_string, "marker"))
					has_type_flags = HTYPE_MARKER;
				else if (streq (type_string, "mesh"))
					has_type_flags = HTYPE_SHELL_MESH;
				break;
			case 'n':
				if (streq (type_string, "nurbs curve"))
					has_type_flags = HTYPE_LINE;
				else if (streq (type_string, "nurbs surface"))
					has_type_flags = HTYPE_POLYGON;
				break;
			case 'p':
				if (streq (type_string, "polygon"))
					has_type_flags = HTYPE_POLYGON;
				else if (streq (type_string, "polyline"))
					has_type_flags = HTYPE_LINE;
				else if (streq (type_string, "polycylinder"))
					has_type_flags = HTYPE_POLYGON;
				break;
			case 'r':
				if (streq (type_string, "reference"))
					has_type_flags = HTYPE_EVERYTHING;
			case 's':
				if (streq (type_string, "shell"))
					has_type_flags = HTYPE_SHELL_MESH;
				else if (streq (type_string, "sphere"))
					has_type_flags = HTYPE_POLYGON;
				break;
			case 't':
				if (streq (type_string, "text"))
					has_type_flags = HTYPE_TEXT;
				break;
			}

			assert(has_type_flags != HTYPE_NONE);
		}
	}


	HC_KEY ref_key=INVALID_KEY;

	//-- Get the cache --
	HPShowCache * cache=0;

	if(selection->GetPShowCache())
	{
		for(int i=0; ; ++i)
		{
			cache=0;
			if (VHASH_STATUS_SUCCESS != 
				selection->GetPShowCache()->LookupNthItem(seg_key, i, &cache))
				break;
			
			if(path_count == cache->path_count)
			{
				for(int j=0; j<cache->path_count; ++j)
				{
					if(cache->path[j]!=path[j])
					{
						cache=0;
						break;
					}
				}
			}

			if(cache)
				break;
		}
		
		if(!cache)
		{
			cache = new HPShowCache;
			cache->path_count=path_count;
			cache->path=new HC_KEY [path_count];
			memcpy(cache->path, path, path_count*sizeof(HC_KEY));
			selection->GetPShowCache()->InsertItem(seg_key, cache);
		}
	}
	else
		cache = new HPShowCache;

	//--Collect basic information --

	char big_buf[MVO_BUFFER_SIZE];
		
	if(!(cache->flags & PSHOW_CACHE_MODELLING_MATRIX)) {
		if (HC_PShow_Net_Modelling_Matrix(path_count, path, cache->modelling_matrix_data)) {
			if (!HUtility::IsIdentityMatrix(cache->modelling_matrix_data))
				cache->modelling_matrix = cache->modelling_matrix_data;
		}
		cache->flags|=PSHOW_CACHE_MODELLING_MATRIX;
	}
	if(!(cache->flags & PSHOW_CACHE_CLIP_REGION)) {
		int	loops;
		int	total;
		if (HC_PShow_Net_Clip_Region_Size(path_count, path, &loops, &total, big_buf)) {
			cache->clip_region_spec=new char[strlen(big_buf)+1];
			cache->clip_region_points=new HPoint[total];
			cache->clip_region_lengths=new int[loops];
			if (!HC_PShow_Net_Clip_Region(path_count, path, &cache->clip_region_loops, cache->clip_region_lengths, cache->clip_region_points, cache->clip_region_spec)) {
				// failed?
				delete cache->clip_region_spec;
				cache->clip_region_spec = 0;
				delete cache->clip_region_points;
				cache->clip_region_points = 0;
				delete cache->clip_region_lengths;
				cache->clip_region_lengths = 0;
			}

		}
		cache->flags|=PSHOW_CACHE_CLIP_REGION;
	}

	if(!(cache->flags & PSHOW_CACHE_VISIBILITY)) {
		HC_PShow_Net_Visibility(path_count, path, big_buf);
		if (big_buf[0]) {
			cache->visibility=new char[strlen(big_buf)+1];
			strcpy(cache->visibility, big_buf);
		}
		cache->flags|=PSHOW_CACHE_VISIBILITY;
	}

	if(!(cache->flags & PSHOW_CACHE_COLORMAP))
	{
		int length = 0;
		HC_PShow_Net_Color_Map_Length(path_count,path,&length);
		if (length > 0) {
			cache->color_map = new char[length+1];
			HC_PShow_Net_Color_Map(path_count,path,cache->color_map);
		}
		cache->flags|=PSHOW_CACHE_COLORMAP;
	}

	if(!(cache->flags & PSHOW_CACHE_RENDERING_OPTIONS))
	{
		HC_PShow_Net_Rendering_Options(path_count, path, big_buf);
		if (big_buf[0]) {
			cache->rendering_options=new char[strlen(big_buf)+1];
			strcpy(cache->rendering_options, big_buf);

			char * cursor=strstr(cache->rendering_options, "screen range");
			if(cursor && !strstr(cursor, "screen range=(-1,1,-1,1)"))
			{
				float sr_1, sr_2, sr_3, sr_4;
				HCLOCALE(sscanf(cursor, "screen range=(%f,%f,%f,%f)", &sr_1, &sr_2, &sr_3, &sr_4));
				HCLOCALE(sprintf(big_buf, "screen range=(%f,%f,%f,%f)", sr_1, sr_2, sr_3, sr_4));
				cache->screen_range=new char[strlen(big_buf)+1];
				strcpy(cache->screen_range, big_buf);
			}
			
			cursor=strstr(cache->rendering_options, "depth range");
			if(cursor && !strstr(cursor, "depth range=(0,1)"))
			{
				float dr_1, dr_2;
				HCLOCALE(sscanf(cursor, "depth range=(%f,%f)", &dr_1, &dr_2));
				HCLOCALE(sprintf(big_buf, "depth range=(%f,%f)", dr_1, dr_2));
				cache->depth_range=new char[strlen(big_buf)+1];
				strcpy(cache->depth_range, big_buf);
			}

			cursor=strstr(cache->rendering_options, "tessellation");
			if(cursor)
			{
				cache->tessellation=new char[strlen(cache->rendering_options)+1];
				int inner = 0;
				const char * cp = cursor;
				int n=0;
				while (*cp != '\0') {
					if (*cp == '(')
						inner++;
					else if (*cp == ')')
						inner--;
					else if (inner == 0 && *cp == ',')
						break;

					cache->tessellation[n++] = *cp++;
				}
				cache->tessellation[n] = '\0';
			}

			cursor=strstr(cache->rendering_options, "general curve");
			if(cursor)
			{
				cache->general_curve=new char[strlen(cache->rendering_options)+1];
				int inner = 0;
				const char * cp = cursor;
				int n=0;
				while (*cp != '\0') {
					if (*cp == '(')
						inner++;
					else if (*cp == ')')
						inner--;
					else if (inner == 0 && *cp == ',')
						break;

					cache->general_curve[n++] = *cp++;
				}
				cache->general_curve[n] = '\0';
			}

			cursor=strstr(cache->rendering_options, "nurbs surface");
			if(cursor)
			{
				cache->nurbs_surface=new char[strlen(cache->rendering_options)+1];
				int inner = 0;
				const char * cp = cursor;
				int n=0;
				while (*cp != '\0') {
					if (*cp == '(')
						inner++;
					else if (*cp == ')')
						inner--;
					else if (inner == 0 && *cp == ',')
						break;

					cache->nurbs_surface[n++] = *cp++;
				}
				cache->nurbs_surface[n] = '\0';
			}
				
			cursor=strstr(cache->rendering_options, "general displacement");
			if(cursor && !strstr(cursor, "general displacement=0"))
			{
				int gd;
				sscanf(cursor, "general displacement=%d", &gd);
				HCLOCALE(sprintf(big_buf, "general displacement=(%d)", gd));
				cache->general_displacement=new char[strlen(big_buf)+1];
				strcpy(cache->general_displacement, big_buf);
			}

			cursor=strstr(cache->rendering_options, "mask transform=");
			if(cursor)
			{		
#if 0
				char *mt = big_buf;
				strcpy(mt, "mask transform=");

				cursor += 15; // strlen("mask transform=")
				mt += 15; // strlen("mask transform=")
				
				unsigned int parens = 0;
				do {
					if(*cursor == '(')
						++parens;
					else if(*cursor == ')')
						--parens;
					*mt++ = *cursor++;				
				} while(*cursor && (*cursor!=',' || parens!=0));
				*mt = 0;		

				cache->mask_transform=new char[strlen(big_buf)+1];
				strcpy(cache->mask_transform, big_buf);
#endif
				//update the modeling matrix
				HC_Compute_Transform_By_Path(path_count-1, path, "object", "world", cache->modelling_matrix_data);
				cache->modelling_matrix = cache->modelling_matrix_data;
			}

			cursor=strstr(cache->rendering_options, "anti-alias=");
			if(cursor)
			{			
				char *aa = big_buf;
				strcpy(aa, "anti-alias=(");
				aa += 12; // strlen("anti-alias=(")
				cursor += 11; // strlen("anti-alias=")
				
				if (*cursor == '(')
				{	
					// multi-item case: anti-alias=(... 
					cursor++;
					while (*cursor != ')')							
						*aa++ = *cursor++;				
				}
				else 
				{
					// single item case: anti-alias=...
					while (*cursor != '\0' && *cursor != ',')							
						*aa++ = *cursor++;				
				}
				*aa++ = ')';
				*aa = 0;	

				cache->anti_alias=new char[strlen(big_buf)+1];
				strcpy(cache->anti_alias, big_buf);
			}
		}
		cache->flags|=PSHOW_CACHE_RENDERING_OPTIONS;
	}

	if(!(cache->flags & PSHOW_CACHE_HANDEDNESS))
	{
		HC_PShow_Net_Handedness(path_count, path, big_buf);
		if (big_buf[0]) {
			cache->handedness=new char[strlen(big_buf)+1];
			strcpy(cache->handedness, big_buf);
		}
		cache->flags|=PSHOW_CACHE_HANDEDNESS;
	}

	if(!(cache->flags & PSHOW_CACHE_HEURISTICS))
	{
		HC_PShow_Net_Heuristics(path_count, path, big_buf);
		if (big_buf[0]) {
			cache->heuristics=new char[strlen(big_buf)+1];
			strcpy(cache->heuristics, big_buf);
		}
		cache->flags|=PSHOW_CACHE_HEURISTICS;
	}		

	if(!(cache->flags & PSHOW_CACHE_CONDITIONS))
	{
		HC_PShow_Net_Conditions(path_count, path, big_buf);
		if (big_buf[0]) {
			cache->conditions=new char[strlen(big_buf)+1];
			strcpy(cache->conditions, big_buf);
		}
		cache->flags|=PSHOW_CACHE_CONDITIONS;
	}		

	if(!(cache->flags & PSHOW_CACHE_CAMERA))
	{

		HC_PShow_Net_Camera(path_count, path, &cache->camera_position, &cache->camera_target, &cache->camera_up, &cache->camera_width, &cache->camera_height, big_buf);

		if(big_buf[0])
		{
			cache->camera_projection=new char[strlen(big_buf)+1];
			strcpy(cache->camera_projection, big_buf);		
		}
		// if cache->camera_projection != null then we have a camera

		cache->flags|=PSHOW_CACHE_CAMERA;
	}	
		
	if(!(cache->flags & PSHOW_CACHE_MARKER_SIZE)) 
	{
		HC_PShow_Net_Var_Marker_Size(path_count, path, big_buf);
		if (big_buf[0]) {
			cache->marker_size=new char [strlen(big_buf)+1];
			strcpy(cache->marker_size, big_buf);
		}
		cache->flags|=PSHOW_CACHE_MARKER_SIZE;
	}

	if(!(cache->flags & PSHOW_CACHE_MARKER_SYMBOL)) 
	{
		HC_PShow_Net_Marker_Symbol(path_count, path, big_buf);
		if (big_buf[0]) {
			cache->marker_symbol=new char[strlen(big_buf)+1];
			strcpy(cache->marker_symbol, big_buf);
		}
		cache->flags|=PSHOW_CACHE_MARKER_SYMBOL;
	}


	if((has_type_flags & HTYPE_TEXT ))
	{
		
		if(!(cache->flags & PSHOW_CACHE_TEXT_FONT))
		{
			HC_PShow_Net_Text_Font(path_count, path, big_buf);
			if (big_buf[0]) {
				cache->text_font=new char[strlen(big_buf)+1];
				strcpy(cache->text_font, big_buf);
			}
			cache->flags|=PSHOW_CACHE_TEXT_FONT;
		}
		
		if(!(cache->flags & PSHOW_CACHE_TEXT_ALIGNMENT))
		{
			HC_PShow_Net_Text_Alignment(path_count, path, big_buf);
			if (big_buf[0]) {
				cache->text_alignment=new char[strlen(big_buf)+1];
				strcpy(cache->text_alignment, big_buf);
			}
			cache->flags|=PSHOW_CACHE_TEXT_ALIGNMENT;
		}

		if(!(cache->flags & PSHOW_CACHE_TEXT_PATH))
		{
			cache->have_text_path = HC_PShow_Net_Text_Path(path_count, path, &cache->text_path_x, &cache->text_path_y, &cache->text_path_z);
			cache->flags|=PSHOW_CACHE_TEXT_PATH;
		}

		if(!(cache->flags & PSHOW_CACHE_TEXT_SPACING))
		{
			cache->have_text_spacing = HC_PShow_Net_Text_Spacing(path_count, path, &cache->text_spacing);
			cache->flags|=PSHOW_CACHE_TEXT_SPACING;
		}
	}
		
	if((has_type_flags & HTYPE_LINE))
	{
		if(!(cache->flags & PSHOW_CACHE_LINE_WEIGHT))
		{
			HC_PShow_Net_Var_Line_Weight(path_count, path, big_buf);
			if (big_buf[0]) {
				cache->line_weight=new char[strlen(big_buf)+1];
				strcpy(cache->line_weight, big_buf);
			}
			cache->flags|=PSHOW_CACHE_LINE_WEIGHT;
		}

		if(!(cache->flags & PSHOW_CACHE_LINE_PATTERN))
		{
			HC_PShow_Net_Line_Pattern(path_count, path, big_buf);
			if (big_buf[0]) {
				cache->line_pattern=new char[strlen(big_buf)+1];
				strcpy(cache->line_pattern, big_buf);
			}
			cache->flags|=PSHOW_CACHE_LINE_PATTERN;
		}
	}	

	
	if((has_type_flags & (HTYPE_SHELL_MESH|HTYPE_POLYGON)))
	{
		if(!(cache->flags & PSHOW_CACHE_EDGE_WEIGHT))
		{
			HC_PShow_Net_Var_Edge_Weight(path_count, path, big_buf);
			if (big_buf[0]) {
				cache->edge_weight=new char[strlen(big_buf)+1];
				strcpy(cache->edge_weight, big_buf);
			}
			cache->flags|=PSHOW_CACHE_EDGE_WEIGHT;
		}

		if(!(cache->flags & PSHOW_CACHE_EDGE_PATTERN))
		{
			HC_PShow_Net_Edge_Pattern (path_count, path, big_buf);
			if (big_buf[0]) {
				cache->edge_pattern=new char[strlen(big_buf)+1];
				strcpy(cache->edge_pattern, big_buf);
			}
			cache->flags|=PSHOW_CACHE_EDGE_PATTERN;
		}
	}

	if((has_type_flags & HTYPE_IMAGE)) {

		if(!(cache->flags & PSHOW_CACHE_IMAGE_SCALE))
		{
			if (cache->rendering_options) 
			{
				char * cursor=strstr(cache->rendering_options, "image scale");
				if(cursor && !strstr(cursor, "image scale=(1,1)"))
				{
					float is_1, is_2;
					HCLOCALE(sscanf(cursor, "image scale=(%f,%f)", &is_1, &is_2));
					HCLOCALE(sprintf(big_buf, "image scale=(%f,%f)", is_1, is_2));
					cache->image_scale=new char[strlen(big_buf)+1];
					strcpy(cache->image_scale, big_buf);
				}
			}
			cache->flags|=PSHOW_CACHE_IMAGE_SCALE;
		}
	}

	if ((has_type_flags & (HTYPE_SHELL_MESH|HTYPE_POLYGON))) {

		if(!(cache->flags & PSHOW_CACHE_POLYGON_HANDEDNESS))
		{
			if (cache->heuristics)
			{
				if (!strstr(cache->heuristics, "no polygon handedness")) 
				{
					const char * left = "polygon handedness=left";
					const char * right = "polygon handedness=right";
					if (strstr(cache->heuristics, left)) 
					{
						cache->polygon_handedness=new char[strlen(left)+1];
						strcpy(cache->polygon_handedness, left);
					}
					else if (strstr(cache->heuristics, right)) 
					{
						cache->polygon_handedness=new char[strlen(right)+1];
						strcpy(cache->polygon_handedness, right);
					}	
				}
			}
			cache->flags|=PSHOW_CACHE_POLYGON_HANDEDNESS;
		}
	}

	if (hl_type!=HHighlightRegion) {

		if(!(cache->flags & PSHOW_CACHE_HIDDEN_LINE_HSRA))
		{
			if(cache->rendering_options)
			{
				char * cursor=strstr(cache->rendering_options, "hidden surface removal algorithm");
				if(cursor)
				{
					cursor+=33; //"hidden surface removal algorithm="
					HC_Parse_String(cursor, ",", 0, big_buf);
					if(strstr(big_buf, "hidden line"))
						cache->hidden_line_hsra=true;
				}
			}
			cache->flags|=PSHOW_CACHE_HIDDEN_LINE_HSRA;
		}
	}




	//-- Done collecting type-specific information --


	//-- Collect *special* information (if any) --
	int special=HSPECIAL_NOTHING;
	int groups = 0;

	bool inv_trans=selection->GetHighlightMode()==InverseTransparency;


	if (inv_trans) {
		
		if(!(cache->flags & PSHOW_CACHE_COLOR))
		{
			HC_PShow_Net_Color(path_count, path, big_buf);
			if (big_buf[0]) {
				cache->color=new char[strlen(big_buf)+1];
				strcpy(cache->color, big_buf);
			}
			cache->flags |= PSHOW_CACHE_COLOR;
		}
	}
			

	if((has_type_flags & (HTYPE_SHELL_MESH|HTYPE_POLYGON)) && !inv_trans)
	{
		if(!(cache->flags & PSHOW_CACHE_TRANS_COLOR))
		{
			HC_PShow_One_Net_Color(path_count, path, "face", big_buf);
			if (big_buf[0]) {

				char * cursor = strstr(big_buf, "transmission");
				
				if (cursor) {

					cursor += strlen("transmission=");
					char * comma = strchr(cursor, ',');

					if (comma)
						*comma = '\0';

					HPoint tc;
					HC_Compute_Color(cursor, "RGB", &tc);
					float intensity = tc.x*RED_GRAYNESS + tc.y*GREEN_GRAYNESS + tc.z*BLUE_GRAYNESS;
					intensity = (float)((int)((intensity+0.005)*100))/100;  //round to hundreths place
					if(intensity>0.f)
					{
						HCLOCALE(sprintf(big_buf, "faces = transmission = (r=%f g=%f b=%f)", intensity, intensity, intensity));
						cache->trans_color=new char[strlen(big_buf)+1];
						strcpy(cache->trans_color, big_buf);	
					}
				}
			}
			cache->flags |= PSHOW_CACHE_TRANS_COLOR;
		}

		if (cache->trans_color)
			groups|=HGROUP_TRANSMISSION;
	}

	if (cache->screen_range)
		groups|=HGROUP_SCREEN_RANGE;

	if (cache->depth_range)
		groups|=HGROUP_DEPTH_RANGE;

	if (cache->tessellation)
		groups|=HGROUP_TESSELLATION;

	if (cache->general_curve)
		groups|=HGROUP_GENERAL_CURVE;

	if (cache->nurbs_surface)
		groups|=HGROUP_NURBS_SURFACE;

	if (cache->clip_region_points)
		groups|=HGROUP_CLIP_REGION;

	if (cache->general_displacement)
		groups|=HGROUP_GENERAL_DISPLACEMENT;

	if (cache->mask_transform)
		groups|=HGROUP_MASK_TRANSFORM;

	if (cache->anti_alias)
		groups|=HGROUP_ANTIALIAS;

	if (cache->handedness)
		groups|=HGROUP_HANDEDNESS;

	if (cache->color_map)
		groups|=HGROUP_COLORMAP;

	if (cache->visibility)
		groups |= HGROUP_VISIBILITY;

	if (cache->conditions)
		groups |= HGROUP_CONDITIONS;

	if (cache->hidden_line_hsra)
		groups |= HGROUP_HIDDEN_LINE_HSRA;

	if ((has_type_flags & HTYPE_IMAGE) && cache->image_scale)
		groups|=HGROUP_IMAGE_SCALE;

	if ((has_type_flags & (HTYPE_SHELL_MESH|HTYPE_POLYGON)) && cache->polygon_handedness)
		groups|=HGROUP_HANDEDNESS;

	if ((has_type_flags & HTYPE_LINE) &&
		(cache->line_weight || cache->line_pattern))
		groups |= HGROUP_LINE_STUFF;

	if ((has_type_flags & (HTYPE_MARKER|HTYPE_SHELL_MESH|HTYPE_LINE|HTYPE_POLYGON)) &&
		(cache->marker_size || cache->marker_symbol))
		groups |= HGROUP_MARKER_STUFF;

	if ((has_type_flags & (HTYPE_SHELL_MESH|HTYPE_POLYGON)) &&
		(cache->edge_weight || cache->edge_pattern))
		groups |= HGROUP_EDGE_STUFF;

	if ((has_type_flags & HTYPE_TEXT) &&
		(cache->text_font || cache->text_alignment || 
		cache->have_text_path || cache->have_text_spacing))
		groups |= HGROUP_TEXT_STUFF;

	if (inv_trans && cache->color)
		groups|=HGROUP_COLOR;




	if (cache->camera_projection)
		special|=HSPECIAL_CAMERA;

	if(hl_type != HHighlightGeometry && cache->modelling_matrix)
		special |= HSPECIAL_MATRIX;

	
	//-- Done collecting special information -- 

	//-- Create the highlight reference --
	HC_Open_Segment_By_Key(selection->GetSelectionSegment());

		if (groups) 
		{
			
			char tmp[MVO_BUFFER_SIZE];
			char seg_name[MVO_BUFFER_SIZE];
			seg_name[0] = '\0';
			char * sn = seg_name;

			if(groups & HGROUP_COLORMAP) 
			{
				sn = strcat(sn, "_CM");
				int	id = selection->GetAttrLookupID(cache->color_map);
				sprintf(tmp, "%d", id);
				sn = strcat(sn, tmp);
			}

			if(groups & HGROUP_COLOR) 
			{
				sn = strcat(sn, "_C");
				int	id = selection->GetAttrLookupID(cache->color);
				sprintf(tmp, "%d", id);
				sn = strcat(sn, tmp);
			}

			if(groups & HGROUP_ANTIALIAS) {

				sn = strcat(sn, "_A");
				int	id = selection->GetAttrLookupID(cache->anti_alias);
				sprintf(tmp, "%d", id);
				sn = strcat(sn, tmp);
			}

			if(groups & HGROUP_HANDEDNESS)
			{
				sn = strcat(sn, "_H");
				if(cache->handedness) {
					int	id = selection->GetAttrLookupID(cache->handedness);
					sprintf(tmp, "W%d", id);
					sn = strcat(sn, tmp);
				}
				if(cache->polygon_handedness) {
					int	id = selection->GetAttrLookupID(cache->polygon_handedness);
					sprintf(tmp, "P%d", id);
					sn = strcat(sn, tmp);
				}
			}

			if(groups & HGROUP_TRANSMISSION)
			{
				int	id = selection->GetAttrLookupID(cache->trans_color);
				sprintf(tmp, "_TC%d", id);
				sn = strcat(sn, tmp);
			}

			if ((groups & HGROUP_VISIBILITY)) {
				int	id = selection->GetAttrLookupID(cache->visibility);
				sprintf(tmp, "%d", id);
				sn = strcat(sn, "-VI");
				sn = strcat(sn, tmp);
			}

			if ((groups & HGROUP_CONDITIONS)) {
				int	id = selection->GetAttrLookupID(cache->conditions);
				sprintf(tmp, "%d", id);
				sn = strcat(sn, "-CN");
				sn = strcat(sn, tmp);
			}

			if((groups & HGROUP_HIDDEN_LINE_HSRA)) 
				sn = strcat(sn, "-NF");
	
			if((groups & HGROUP_LINE_STUFF))
			{
				sn = strcat(sn, "_L");

				if (cache->line_weight) 
				{
					int	id = selection->GetAttrLookupID(cache->line_weight);
					sprintf(tmp, "%d", id);
					sn = strcat(sn, "W");
					sn = strcat(sn, tmp);
				}
				if (cache->line_pattern) 
				{
					int	id = selection->GetAttrLookupID(cache->line_pattern);
					sprintf(tmp, "%d", id);
					sn = strcat(sn, "P");
					sn = strcat(sn, tmp);
				}
			}

			if((groups & HGROUP_MARKER_STUFF))
			{
				sn = strcat(sn, "_M");

				if (cache->marker_size) 
				{
					int	id = selection->GetAttrLookupID(cache->marker_size);
					sprintf(tmp, "%d", id);
					sn = strcat(sn, "S");
					sn = strcat(sn, tmp);
				}
				if (cache->marker_symbol)
				{
					int	id = selection->GetAttrLookupID(cache->marker_symbol);
					sprintf(tmp, "%d", id);
					sn = strcat(sn, "P");
					sn = strcat(sn, tmp);
				}
			}

			if((groups & HGROUP_EDGE_STUFF))
			{
				sn = strcat(sn, "_E");
				if (cache->edge_weight) 
				{
					int	id = selection->GetAttrLookupID(cache->edge_weight);
					sprintf(tmp, "%d", id);
					sn = strcat(sn, "W");
					sn = strcat(sn, tmp);
				}
				if (cache->edge_pattern) 
				{
					int	id = selection->GetAttrLookupID(cache->edge_pattern);
					sprintf(tmp, "%d", id);
					sn = strcat(sn, "P");
					sn = strcat(sn, tmp);
				}
			}

			if((groups & HGROUP_TEXT_STUFF))
			{
				sn = strcat(sn, "_T");

				if (cache->text_font)
				{
					int	id = selection->GetAttrLookupID(cache->text_font);
					sprintf(tmp, "%d", id);
					sn = strcat(sn, "F");
					sn = strcat(sn, tmp);
				}
				if (cache->text_alignment) 
				{
					int	id = selection->GetAttrLookupID(cache->text_alignment);
					sprintf(tmp, "%d", id);
					sn = strcat(sn, "L");
					sn = strcat(sn, tmp);
				}

				if (cache->have_text_path)
				{
					sprintf(tmp, "%f%f%f", cache->text_path_x, cache->text_path_y, cache->text_path_z);
					int	id = selection->GetAttrLookupID(tmp);
					sprintf(tmp, "%d", id);
					sn = strcat(sn, "P");
					sn = strcat(sn, tmp);
				}
				if (cache->have_text_spacing) 
				{
					sprintf(tmp, "%f", cache->text_spacing);
					int	id = selection->GetAttrLookupID(tmp);
					sprintf(tmp, "%d", id);
					sn = strcat(sn, "S");
					sn = strcat(sn, tmp);
				}
			}	

			if(groups & HGROUP_SCREEN_RANGE)
			{
				int	id = selection->GetAttrLookupID(cache->screen_range);
				sprintf(tmp, "%d", id);
				sn = strcat(sn, "-SR");
				sn = strcat(sn, tmp);
			}
	
			if(groups & HGROUP_DEPTH_RANGE)
			{
				int	id = selection->GetAttrLookupID(cache->depth_range);
				sprintf(tmp, "%d", id);
				sn = strcat(sn, "-DR");
				sn = strcat(sn, tmp);
			}

			if(groups & HGROUP_TESSELLATION)
			{
				int	id = selection->GetAttrLookupID(cache->tessellation);
				sprintf(tmp, "%d", id);
				sn = strcat(sn, "-TS");
				sn = strcat(sn, tmp);
			}

			if(groups & HGROUP_GENERAL_CURVE)
			{
				int	id = selection->GetAttrLookupID(cache->general_curve);
				sprintf(tmp, "%d", id);
				sn = strcat(sn, "-GC");
				sn = strcat(sn, tmp);
			}

			if(groups & HGROUP_NURBS_SURFACE)
			{
				int	id = selection->GetAttrLookupID(cache->nurbs_surface);
				sprintf(tmp, "%d", id);
				sn = strcat(sn, "-NS");
				sn = strcat(sn, tmp);
			}

			if(groups & HGROUP_CLIP_REGION)
			{
				int	id = selection->GetAttrLookupID(cache->clip_region_spec);
				sprintf(tmp, "%d", id);
				sn = strcat(sn, "-CR");
				sn = strcat(sn, tmp);

				sprintf(tmp, "%d", cache->clip_region_loops);
				id = selection->GetAttrLookupID(tmp);
				sprintf(tmp, "%d", id);
				sn = strcat(sn, "L");
				sn = strcat(sn, tmp);

				int total = 0;

				//for(int i = 0; i<cache->clip_region_loops; ++i) {
				{
					int i = 0; // this should be multiple but is unsupported now
					sprintf(tmp, "%d", cache->clip_region_lengths[i]);
					total += cache->clip_region_lengths[i];
					id = selection->GetAttrLookupID(tmp);
					sprintf(tmp, "%d", id);
					sn = strcat(sn, "O");
					sn = strcat(sn, tmp);
				}
		
				for(int i = 0; i<total; ++i) {
					sprintf(tmp, "%f%f%f", cache->clip_region_points[i].x, cache->clip_region_points[i].y, cache->clip_region_points[i].z);
					id = selection->GetAttrLookupID(tmp);
					sprintf(tmp, "%d", id);
					sn = strcat(sn, "P");
					sn = strcat(sn, tmp);
				}
			}

			if(groups & HGROUP_IMAGE_SCALE)
			{
				int	id = selection->GetAttrLookupID(cache->image_scale);
				sprintf(tmp, "%d", id);
				sn = strcat(sn, "-IS");
				sn = strcat(sn, tmp);
			}
	
			if(groups & HGROUP_GENERAL_DISPLACEMENT)
			{
				int	id = selection->GetAttrLookupID(cache->general_displacement);
				sprintf(tmp, "%d", id);
				sn = strcat(sn, "-GD");
				sn = strcat(sn, tmp);
			}

			if(groups & HGROUP_MASK_TRANSFORM) {
				int	id = selection->GetAttrLookupID(cache->mask_transform);
				sprintf(tmp, "%d", id);
				sn = strcat(sn, "-MT");
				sn = strcat(sn, tmp);
			}

			HC_Open_Segment(sn);

			if (HC_Show_Existence("attributes") == 0) {

				if(groups & HGROUP_COLORMAP) 
				{
					HC_Set_Color_Map(cache->color_map);
				}

				if(groups & HGROUP_COLOR) 
				{
					HC_Set_Color(cache->color);
				}
				
				if(groups & HGROUP_ANTIALIAS)
				{
					HC_Set_Rendering_Options(cache->anti_alias);
				}

				if(groups & HGROUP_HANDEDNESS)
				{
					if(cache->handedness)
						HC_Set_Handedness(cache->handedness);

					if(cache->polygon_handedness)
						HC_Set_Heuristics(cache->polygon_handedness);
				}

				if(groups & HGROUP_TRANSMISSION)
				{
					HC_Set_Color(cache->trans_color);
				}

				if ((groups & HGROUP_VISIBILITY)) 
					HC_Set_Visibility(cache->visibility);

				if ((groups & HGROUP_CONDITIONS)) 
					HC_Set_Conditions(cache->conditions);

				if ((groups & HGROUP_HIDDEN_LINE_HSRA))
					HC_Set_Visibility("no faces");

				if ((groups & HGROUP_LINE_STUFF))
				{
					if (cache->line_weight)
						HC_Set_Variable_Line_Weight(cache->line_weight);
					if (cache->line_pattern)
						HC_Set_Line_Pattern(cache->line_pattern);
				}

				if((groups & HGROUP_MARKER_STUFF))
				{
					if (cache->marker_size)
						HC_Set_Variable_Marker_Size(cache->marker_size);
					if (cache->marker_symbol)
						HC_Set_Marker_Symbol(cache->marker_symbol);
				}

				if((groups & HGROUP_EDGE_STUFF))
				{
					if (cache->edge_weight)
						HC_Set_Variable_Edge_Weight(cache->edge_weight);
					if (cache->edge_pattern)
						HC_Set_Edge_Pattern(cache->edge_pattern);
				}

				
				if((groups & HGROUP_TEXT_STUFF))
				{
					if (cache->text_font) 
						HC_Set_Text_Font(cache->text_font);
					if (cache->text_alignment) 
						HC_Set_Text_Alignment(cache->text_alignment);
					if (cache->have_text_path)
						HC_Set_Text_Path(cache->text_path_x, cache->text_path_y, cache->text_path_z);
					if (cache->have_text_spacing) 
						HC_Set_Text_Spacing(cache->text_spacing);
				}	

				if(groups & HGROUP_SCREEN_RANGE)
					HC_Set_Rendering_Options(cache->screen_range);
		
				if(groups & HGROUP_DEPTH_RANGE)
					HC_Set_Rendering_Options(cache->depth_range);

				if(groups & HGROUP_TESSELLATION)
					HC_Set_Rendering_Options(cache->tessellation);

				if(groups & HGROUP_GENERAL_CURVE)
					HC_Set_Rendering_Options(cache->general_curve);

				if(groups & HGROUP_NURBS_SURFACE)
					HC_Set_Rendering_Options(cache->nurbs_surface);

				if(groups & HGROUP_IMAGE_SCALE)
					HC_Set_Rendering_Options(cache->image_scale);
		
				if(groups & HGROUP_GENERAL_DISPLACEMENT)
					HC_Set_Rendering_Options(cache->general_displacement);

				if(groups & HGROUP_MASK_TRANSFORM)
					HC_Set_Rendering_Options(cache->mask_transform);

				if(groups & HGROUP_CLIP_REGION) {
					//for(int i=0; i<cache->clip_region_loops; ++i) {
					{
						int i = 0; // this should be multiple but is unsupported now
						HC_Set_Polygonal_Clip_Region (cache->clip_region_lengths[i], cache->clip_region_points, cache->clip_region_spec);
					}
				}
			}

		}

		HC_KEY	special_seg_key = INVALID_KEY;

		//-- If special information exists, create a child segment --
		if(special)
		{	
			special_seg_key = HC_Open_Segment("");
			
			if(special & HSPECIAL_CAMERA)
				HC_Set_Camera(&cache->camera_position, &cache->camera_target, &cache->camera_up, cache->camera_width, cache->camera_height, cache->camera_projection);

			if(special & HSPECIAL_MATRIX) 
				HC_Set_Modelling_Matrix(cache->modelling_matrix);

		}	
		//-- Done handling special information --

		//-- Create the highlight reference --
		if(hl_type==HHighlightRegion)
		{
			if(!special)
				ref_key=HC_Open_Segment("");

			HC_Insert_Shell_From_Region(key, region, "");
			HC_Set_Selectability("off");
			if (cache->visibility) 				
				HC_Set_Visibility(cache->visibility);
			if (cache->marker_size)
				HC_Set_Variable_Marker_Size(cache->marker_size);
			if (cache->marker_symbol)
				HC_Set_Marker_Symbol(cache->marker_symbol);
			if (cache->line_weight)
				HC_Set_Variable_Line_Weight(cache->line_weight);
			if (cache->edge_weight)
				HC_Set_Variable_Edge_Weight(cache->edge_weight);
			if (cache->edge_pattern)
				HC_Set_Edge_Pattern(cache->edge_pattern);
			
			if(!special)
				HC_Close_Segment();
		}
		else if (hl_type==HHighlightSubentity)
		{
			int sub_flist_count=0, sub_pcount=0;

			if(!special)
				ref_key=HC_Open_Segment("");

			HSubentityPassthrough * pt = (HSubentityPassthrough *)data;

			int * v_map = new int[pt->vertex_map_count];
			int * f_map = new int[pt->face_map_count];

			HC_KEY	sub_key;
			if (pt->face_count == 0 && pt->vertex_count == 0) {
				sub_key = HC_Insert_Shell(0, 0, 0, 0);
				std::fill_n(v_map, pt->vertex_map_count, -1);
				std::fill_n(f_map, pt->face_map_count, -1);
			}
			else {
				sub_key = HC_Compute_Subshell(key, pt->face_count, pt->faces, pt->vertex_count, pt->vertex1, pt->vertex2, f_map, v_map);
			}

			for(int i=0; i<pt->vertex_map_count; ++i) {
				if (v_map[i] >= 0)
					pt->vertex_map->InsertItem(i, v_map[i]);
			}

			for(int i=0; i<pt->face_map_count; ++i) {
				if (f_map[i] >= 0)
					pt->face_map->InsertItem(i, f_map[i]);
			}

			delete [] v_map;
			delete [] f_map;

			HC_Show_Shell_Size(sub_key, &sub_pcount, &sub_flist_count);
			
			int max = sub_pcount>pt->vertex_count ? sub_pcount : pt->vertex_count;
			
			//we receive a list of faces, vertices,and edges to KEEP,
			//but we need a list of edges to REMOVE.  
			
			//translate vertex numbers
			if(pt->vertex_count > 0)
			{
				int * new_v1 = new int [max];
				int * new_v2 = 0;
				int remove_vert_count = sub_pcount;
				char * vert_vis = new char [max];
				VHash <int, int> edge_hash;

				memset(vert_vis, 0, max);
				
				if(pt->vertex2)
					new_v2 = new int [max];

				//convert point indices from original shell to sub-shell
				for(int i=0; i<pt->vertex_count; ++i)
				{
					new_v1[i] = pt->vertex_map->Lookup(pt->vertex1[i]);

					//keep track of which vertices should be visible
					if(vert_vis[new_v1[i]]==0)
					{
						vert_vis[new_v1[i]]=(char)1;
						--remove_vert_count;
					}

					//if we have edges to consider too, translate their indices as well
					//do not consider them for purposes of visibility
					if(pt->vertex2)
					{
						if(pt->vertex2[i]>=0)
							new_v2[i] = pt->vertex_map->Lookup(pt->vertex2[i]);
						else
							new_v2[i]=-1;

						if(new_v1[i] < new_v2[i])
							edge_hash.InsertItem(new_v1[i], new_v2[i]);
						else
							edge_hash.InsertItem(new_v2[i], new_v1[i]);
					}
				}

				int actual_count = 0;

				if(pt->highlight_subentity & HSUBENTITY_VERTICES)
				{
					if(remove_vert_count>0)
					{
						//generate list of indices to be removed
						int * modify_verts = new int [max];
						char * modify_vis = new char [max];

						memset(modify_vis, 0, remove_vert_count);					

						for(int i=0; i<sub_pcount; ++i)
						{
							if(vert_vis[i]==0)
								modify_verts[actual_count++]=i;
						}

						HC_MSet_Specific_Vertex_Visibilities(sub_key, actual_count, modify_verts, modify_vis);

						delete [] modify_verts;
						delete [] modify_vis;
					}
				}
				
				//now handle edges...
				if(pt->vertex2 && pt->highlight_subentity & HSUBENTITY_EDGES)
				{
					int fsize, first_index, edge1, edge2, tmp;
					int * remove_v1, * remove_v2;
					int * sub_flist;
					int j, k;

					sub_flist = new int [sub_flist_count];
					HC_Show_Shell(sub_key, 0, 0, 0, sub_flist);

					remove_v1 = new int [sub_flist_count];
					remove_v2 = new int [sub_flist_count];

					int i=0;
					actual_count=0;
					while(i<sub_flist_count)
					{
						fsize=sub_flist[i++];
						first_index=sub_flist[i];
						
						//order the indices, lowest first
						if(first_index<sub_flist[i+fsize-1])
						{
							edge1=first_index;
							edge2=sub_flist[i+fsize-1];
						}
						else
						{
							edge1=sub_flist[i+fsize-1];
							edge2=first_index;							
						}
						
						//check edge between first and last points in the face
						for(k=0; ; ++k)
						{
							//edge not found, add it to the remove list
							//also add it to the hash to prevent duplicate removals
							if (VHASH_STATUS_SUCCESS != edge_hash.LookupNthItem(edge1, k, &tmp))
							{
								remove_v1[actual_count]=edge1;
								remove_v2[actual_count++]=edge2;
								edge_hash.InsertItem(edge1, edge2);
								break;
							}
							
							//edge is in the hash, no need to keep looking
							if (tmp == edge2)
								break;
						}

						//check the edges between the rest of the points in the face
						for(j=0; j< fsize-1; ++j)
						{
							//order the indices, lowest first
							if(sub_flist[i+j]<sub_flist[i+j+1])
							{
								edge1=sub_flist[i+j];
								edge2=sub_flist[i+j+1];
							}
							else
							{
								edge1=sub_flist[i+j+1];
								edge2=sub_flist[i+j];
							}

							for(k=0; ; ++k)
							{
								//edge not found, add it to the remove list.
								//also add it to the hash to prevent duplicate removals
								if (VHASH_STATUS_SUCCESS != edge_hash.LookupNthItem(edge1, k, &tmp))
								{
									remove_v1[actual_count]=edge1;
									remove_v2[actual_count++]=edge2;
									edge_hash.InsertItem(edge1, edge2);
									break;
								}
								
								//edge is in the hash, no need to keep looking
								if (tmp == edge2)
									break;
							}
						}

						i+=fsize;
					}

					if(actual_count>0)
					{
						char * modify_edge_vis = new char[actual_count];
						memset(modify_edge_vis, 0, actual_count);
						
						HC_MSet_Specific_Edge_Visibilities(sub_key, actual_count, remove_v1, remove_v2, modify_edge_vis);
						
						delete [] modify_edge_vis;
					}

					delete [] sub_flist;
					delete [] remove_v1;
					delete [] remove_v2;					
				}

				delete [] new_v1;
				delete [] new_v2; 
				delete [] vert_vis;
			}
			
			if(!(pt->highlight_subentity & HSUBENTITY_VERTICES))
			{
				//remove them all
				char * modify_vis = new char [sub_pcount];

				memset(modify_vis, 0, sub_pcount);

				HC_MSet_Vertex_Visibilities(sub_key, 0, sub_pcount, modify_vis);

				delete [] modify_vis;
			}
	
			if(!(pt->highlight_subentity & HSUBENTITY_EDGES))
			{
				int fsize, first_index, edge1, edge2;
				int * remove_v1, * remove_v2;
				int * sub_flist;
				int actual_count=0;

				sub_flist = new int [sub_flist_count];
				HC_Show_Shell(sub_key, 0, 0, 0, sub_flist);

				remove_v1 = new int [sub_flist_count];
				remove_v2 = new int [sub_flist_count];

				int i=0;
				while(i<sub_flist_count)
				{
					fsize=sub_flist[i++];
					first_index=sub_flist[i];
					
					//order the indices, lowest first
					if(first_index<sub_flist[i+fsize-1])
					{
						edge1=first_index;
						edge2=sub_flist[i+fsize-1];
					}
					else
					{
						edge1=sub_flist[i+fsize-1];
						edge2=first_index;							
					}
					
					remove_v1[actual_count]=edge1;
					remove_v2[actual_count++]=edge2;
					
					//remove the edges between the rest of the points in the face
					for(int j=0; j< fsize-1; ++j)
					{
						//order the indices, lowest first
						if(sub_flist[i+j]<sub_flist[i+j+1])
						{
							edge1=sub_flist[i+j];
							edge2=sub_flist[i+j+1];
						}
						else
						{
							edge1=sub_flist[i+j+1];
							edge2=sub_flist[i+j];
						}

						remove_v1[actual_count]=edge1;
						remove_v2[actual_count++]=edge2;
					}

					i+=fsize;
				}

				if(actual_count>0)
				{
					char * modify_edge_vis = new char[actual_count];
					memset(modify_edge_vis, 0, actual_count);
					
					HC_MSet_Specific_Edge_Visibilities(sub_key, actual_count, remove_v1, remove_v2, modify_edge_vis);
					
					delete [] modify_edge_vis;
				}

				delete [] sub_flist;
				delete [] remove_v1;
				delete [] remove_v2;
			}


			HC_Set_Selectability("off");
			if (cache->visibility) 				
				HC_Set_Visibility(cache->visibility);

			if(pt->highlight_subentity & HSUBENTITY_EDGES)
			{
				char patchup[MVO_BUFFER_SIZE];
				patchup[0] = '\0';
				if (HC_Show_Existence ("visibility=edge"))
					HC_Show_One_Visibility("edge", patchup);
				if(!streq("off", patchup))
					HC_Set_Visibility("edges=on");
			}

			if(cache->hidden_line_hsra)
				HC_Set_Visibility("no faces");

			if (cache->marker_size)
				HC_Set_Variable_Marker_Size(cache->marker_size);
			if (cache->marker_symbol)
				HC_Set_Marker_Symbol(cache->marker_symbol);
			if (cache->line_weight)
				HC_Set_Variable_Line_Weight(cache->line_weight);
			if (cache->edge_weight)
				HC_Set_Variable_Edge_Weight(cache->edge_weight);
			if (cache->edge_pattern)
				HC_Set_Edge_Pattern(cache->edge_pattern);
			
			if(!special)
				HC_Close_Segment();
		}
		else
		{
			if(hl_type == HHighlightSegment)
			{
				
				special_seg_key = HC_Open_Segment("includes");

				ref_key = HC_Include_Segment_By_Key(key);
				if(!inv_trans)
					HC_Set_Rendering_Options("attribute lock = (color), ignore subsegment locks = (color)");
				else
					HC_UnSet_One_Rendering_Option("attribute lock");
				if(cache->hidden_line_hsra)
					HC_Set_Rendering_Options("attribute lock = (visibility), ignore subsegment locks = (visibility)");

				HC_Close_Segment();
			}
			else {

				ref_key = HC_Reference_Geometry_By_Key(key);

				if(cache->modelling_matrix) {
					HC_Open_Geometry(ref_key);
					HC_Set_Modelling_Matrix(cache->modelling_matrix);
					HC_Close_Geometry();
				}
			}
		}

		if(special)
		{
			HC_Close_Segment();
			ref_key=special_seg_key;
		}

		if (groups) 
			HC_Close_Segment();

	HC_Close_Segment();
	//-- Done creating the highlight reference --

	if(!selection->GetPShowCache())
		delete cache;

	return ref_key;
}


HSmartSelItem::HSmartSelItem(HC_KEY key, HSelectionSet * selection_set, int incl_count, HC_KEY * incl_keys) 
	: HSelectionItem(key, selection_set)
	, m_entity_proxy_seg_tag(INVALID_KEY)
{
	m_ConditionStyle = INVALID_KEY;
	m_HighlightMode = HighlightDefault;
	m_IncludePath.count = incl_count;
	m_IncludePath.keys = new HC_KEY[incl_count];
	memcpy(&m_IncludePath.keys[0], incl_keys, incl_count*sizeof(HC_KEY));
	m_pSelKeyList = 0;

	m_is_segment = true;
	char keytype[MVO_BUFFER_SIZE];
	HC_Show_Key_Type(key, keytype);
	if (!streq(keytype, "segment")) 
		m_is_segment = false;

	validate_include_path(m_key, &m_IncludePath.count, &m_IncludePath.keys,
		m_pSelectionSet->GetReferenceSelection(), 
		m_pSelectionSet->GetView()->GetViewKey(), m_pSelectionSet->GetView()->GetIncludeLinkKey());
}

HSmartSelItem::HSmartSelItem(HC_KEY key, HSelectionSet * selection_set, int incl_count, HC_KEY * incl_keys, bool is_segment) 
	: HSelectionItem(key, selection_set)
	, m_entity_proxy_seg_tag(INVALID_KEY)
{
	m_ConditionStyle = INVALID_KEY;
	m_HighlightMode = HighlightDefault;
	m_IncludePath.count = incl_count;
	if (incl_count > 0) 
	{
		m_IncludePath.keys = new HC_KEY[incl_count];
		memcpy(&m_IncludePath.keys[0], incl_keys, incl_count*sizeof(HC_KEY));
	}
	else
		m_IncludePath.keys = 0;

	m_pSelKeyList = 0;
	m_is_segment = is_segment;

	validate_include_path(m_key, &m_IncludePath.count, &m_IncludePath.keys,
		m_pSelectionSet->GetReferenceSelection(), 
		m_pSelectionSet->GetView()->GetViewKey(), m_pSelectionSet->GetView()->GetIncludeLinkKey());
}

HSmartSelItem::HSmartSelItem(bool is_ooc, HC_KEY key, HSelectionSet * selection_set, int incl_count, HC_KEY * incl_keys, bool is_segment) 
	: HSelectionItem(key, selection_set)
	, m_entity_proxy_seg_tag(INVALID_KEY)
{
	m_ConditionStyle = INVALID_KEY;
	m_HighlightMode = HighlightDefault;
	m_IncludePath.count = incl_count;
	if (incl_count > 0) 
	{
		m_IncludePath.keys = new HC_KEY[incl_count];
		memcpy(&m_IncludePath.keys[0], incl_keys, incl_count*sizeof(HC_KEY));
	}
	else
		m_IncludePath.keys = 0;

	m_pSelKeyList = 0;
	m_is_segment = is_segment;

	if (!is_ooc)
	{
		validate_include_path(m_key, &m_IncludePath.count, &m_IncludePath.keys,
			m_pSelectionSet->GetReferenceSelection(), 
			m_pSelectionSet->GetView()->GetViewKey(), m_pSelectionSet->GetView()->GetIncludeLinkKey());
	}
}


HSmartSelItem::~HSmartSelItem()
{
	H_SAFE_DELETE_ARRAY(m_IncludePath.keys);
	H_SAFE_DELETE_ARRAY(m_FullPath);
	if (m_pSelKeyList)
		delete_vlist(m_pSelKeyList);
}


HRegionSelItem::HRegionSelItem(HShellObject const & oShell, HSelectionSet * selection_set, int incl_count, HC_KEY * incl_keys, int region) 
	: HSmartSelItem(oShell.GetKey(), selection_set, incl_count, incl_keys)
	, m_Region(region)
	, m_oShellObj(oShell)
	, m_oRegionObj(&m_oShellObj, region)
{
	FixIncludePath();
}


HSubentitySelItem::HSubentitySelItem(
	HC_KEY key, HSelectionSet * selection_set, int incl_count, HC_KEY * incl_keys, 
	int face_count, int const * faces, int vertex_count, int const * vertex1, int const * vertex2,
		bool highlight_faces, bool highlight_vertices, bool highlight_edges, bool maintain_maps)
	//
	: HSmartSelItem(key, selection_set, incl_count, incl_keys, false)
	, m_face_count(face_count), m_faces(0), m_vertex_count(vertex_count), m_vertex1(0), m_vertex2(0)
	, m_maintain_maps(maintain_maps), m_face_map_count(0), m_face_map(0), m_vertex_map_count(0), m_vertex_map(0)
	, m_subentity_highlight(0)
{
	if (m_face_count && faces)
	{
		m_faces = new int [m_face_count];
		memcpy(m_faces, faces, m_face_count*sizeof(int));
	}
	if (m_vertex_count && vertex1)
	{
		m_vertex1 = new int [m_vertex_count];
		memcpy(m_vertex1, vertex1, m_vertex_count*sizeof(int));
	}
	if (m_vertex_count && vertex2)
	{
		m_vertex2 = new int [m_vertex_count];
		memcpy(m_vertex2, vertex2, m_vertex_count*sizeof(int));
	}

	m_subentity_highlight |= (highlight_faces ? HSUBENTITY_FACES : 0)
	                      |  (highlight_vertices ? HSUBENTITY_VERTICES : 0)
	                      |  (highlight_edges ? HSUBENTITY_EDGES : 0);

	FixIncludePath();
}


HSubentitySelItem::HSubentitySelItem(
	bool is_ooc, HC_KEY key, HSelectionSet * selection_set, int incl_count, HC_KEY * incl_keys, 
	int face_count, int const * faces, int vertex_count, int const * vertex1, int const * vertex2,
	bool highlight_faces, bool highlight_vertices, bool highlight_edges, bool maintain_maps)
	//
	: HSmartSelItem(is_ooc, key, selection_set, incl_count, incl_keys, false)
	, m_face_count(face_count), m_faces(0), m_vertex_count(vertex_count), m_vertex1(0), m_vertex2(0)
	, m_maintain_maps(maintain_maps), m_face_map_count(0), m_face_map(0), m_vertex_map_count(0), m_vertex_map(0)
	, m_subentity_highlight(0)
{
	if (m_face_count && faces)
	{
		m_faces = new int [m_face_count];
		memcpy(m_faces, faces, m_face_count*sizeof(int));
	}
	if (m_vertex_count && vertex1)
	{
		m_vertex1 = new int [m_vertex_count];
		memcpy(m_vertex1, vertex1, m_vertex_count*sizeof(int));
	}
	if (m_vertex_count && vertex2)
	{
		m_vertex2 = new int [m_vertex_count];
		memcpy(m_vertex2, vertex2, m_vertex_count*sizeof(int));
	}
	
	m_subentity_highlight |= (highlight_faces ? HSUBENTITY_FACES : 0)
	                      |  (highlight_vertices ? HSUBENTITY_VERTICES : 0)
	                      |  (highlight_edges ? HSUBENTITY_EDGES : 0);

	FixIncludePath();
}

HSubentitySelItem::~HSubentitySelItem()
{
	if(m_faces)
		delete [] m_faces;

	if(m_vertex1)
		delete [] m_vertex1;

	if(m_vertex2)
		delete [] m_vertex2;

	if(m_vertex_map)
		delete m_vertex_map;

	if(m_face_map)
		delete m_face_map;
}

bool HSubentitySelItem::Equals(const HSelectionItem* i2) const
{
	if(!Similar(i2))
		return false;

	// safe to typecast now
	HSubentitySelItem * sub_sel2 = (HSubentitySelItem*) i2;

	int count1, count2;
	int * array2;
	
	count1=GetFaceCount();
	count2=sub_sel2->GetFaceCount();
	
	if(count1!=count2)
		return false;

	if(count1>0)
	{
		array2 = new int [count2];
		sub_sel2->GetFaces(array2);

		for(int i=0; i<count1; ++i)
		{
			if(m_faces[i]!=array2[i])
			{
				delete [] array2;
				return false;
			}
		}
		delete [] array2;
	}

	count1=GetVertexCount();
	count2=sub_sel2->GetVertexCount();
	
	if(count1!=count2)
		return false;

	if(count1>0)
	{
		array2 = new int [count2];
		int * array2_2 = new int [count2];		
		sub_sel2->GetVertices(array2, array2_2);

		for(int i=0; i<count1; ++i)
		{
			if(m_vertex1[i]!=array2[i])
			{
				delete [] array2;
				delete [] array2_2;
				return false;
			}
		}

		if((m_vertex2 && !array2_2) || (!m_vertex2 && array2_2))
		{
			delete [] array2;
			delete [] array2_2;
			return false;
		}

		if(array2_2)
		{
			for(int i=0; i<count1; ++i)
			{
				if(m_vertex2[i]!=array2_2[i])
				{
					delete [] array2;
					delete [] array2_2;
					return false;
				}
			}
		}
		
		delete [] array2;
		delete [] array2_2;
	}	
	
	return true;
}

bool HSubentitySelItem::Similar(const HSelectionItem * i2) const
{
	// if primary selection key is same
	if( this->GetKey() != i2->GetKey() )
		return false;

	// return false if they are of different type
	if( !streq(this->GetName(), i2->GetName()) )
		return false;

	// safe to typecast now
	HSubentitySelItem * sub_sel2 = (HSubentitySelItem*) i2;

	// if the include keys in the path are same
	int incl_count;
	if( (incl_count = this->GetIncludeCount()) != sub_sel2->GetIncludeCount() )
		return false;

	const HC_KEY * ikeys1 = this->GetIncludeKeys();
	const HC_KEY * ikeys2 = sub_sel2->GetIncludeKeys();
	int i;

	for (i = 0; i < incl_count; i++)
	{
		if(ikeys1[i] != ikeys2[i])
			return false;
	}

	return true;
}

bool HSubentitySelItem::UnHighlight()
{
	CommonUnhighlightRef();
	return true;
}

bool HSubentitySelItem::UnHighlight(HSubentitySelItem const * remove_sel)
{
	//should we make sure the selection items are similar first?
	//currently we are not.

	//typically there will be only ref key in the list and that key will be to a segment containing a shell,
	//but we should still handle the corner cases (relatively) gracefully
	int hl_key_count = GetHighlightedRefListCount();
	HC_KEY * hl_key = new HC_KEY [hl_key_count];
	bool unhighlighted_something = false;
	GetHighlightedRefList(hl_key);

	//we can't do anything without the maps
	if(m_maintain_maps)
	{
		for(int k=0; k<hl_key_count; ++k)
		{
			char type[MVO_BUFFER_SIZE];
			bool error = false;
			
			HC_Show_Key_Type(hl_key[k], type);
			error = !streq(type, "segment");

			//these should all be segments containing a shell, but let's just be sure
			if(!error)
			{
				//we actually need the shell inside
				//there should be exactly 1 shell in each segment
				//we simply replace the segment key in the hl_key array with it
				HC_Open_Segment_By_Key(hl_key[k]);
					HC_Begin_Contents_Search(".", "shell");
					error = !HC_Find_Contents(0, &hl_key[k]);
					HC_End_Contents_Search();
				HC_Close_Segment();
			}
			
			if(error)
			{
				char const * msg = "Unknown sub-entity highlight object encountered.";
				HC_Report_Error(0, 0, 0, 1, &msg, 0, 0);
				delete [] hl_key;
				return false;
			}

			//faces first because they're easy (relatively)
			if((m_subentity_highlight & HSUBENTITY_FACES) && m_faces
				&& (remove_sel->m_subentity_highlight & HSUBENTITY_FACES) && remove_sel->m_faces)
			{
				int * rem_faces = new int [remove_sel->m_face_count];
				int rem_faces_count = 0;

				//walk through remove_sel's faces
				for(int i=0; i<remove_sel->m_face_count; ++i)
				{
					int current_face = remove_sel->m_faces[i];
					int f_lookup;

					//if it occurs in the subshell...
					if(current_face >= 0 && current_face < m_face_map_count && m_face_map->LookupItem(current_face, &f_lookup) != VHASH_STATUS_FAILED)
					{
						//see if it's highlighted
						for(int j=0; j<m_face_count; ++j)
						{
							if(m_faces[j] == current_face)
							{
								//add it to the list of faces to be removed
								rem_faces[rem_faces_count++] = f_lookup;
								m_faces[j] = -1;	//mark for removal
								break;
							}
						}
					}
				}
				
				//if we are removing anything, fix up the local record of what's highlighted
				if(rem_faces_count)
				{
					int new_face_count = 0;
					int * new_faces = 0;
					char * face_vis = new char [rem_faces_count];
					memset(face_vis, 0, rem_faces_count);
					
					if(rem_faces_count < m_face_count)
					{
						new_faces = new int [m_face_count - rem_faces_count];

						//make up a new m_faces list
						for(int i=0; i<m_face_count; ++i)
						{
							if(m_faces[i] != -1)
								new_faces[new_face_count++] = m_faces[i];
						}
					}

					//out with the old, in with the new.
					delete [] m_faces;
					m_faces = new_faces;
					m_face_count = new_face_count;

					//update the subshell
					HC_MSet_Specific_Face_Vis(hl_key[k], rem_faces_count, rem_faces, face_vis);
					delete [] face_vis;
					unhighlighted_something = true;
				}

				delete [] rem_faces;
			}

			//edges and vertices
			bool removing_edges = (m_subentity_highlight & HSUBENTITY_EDGES) && m_vertex1 && m_vertex2
				&& (remove_sel->m_subentity_highlight & HSUBENTITY_EDGES) && remove_sel->m_vertex1 && remove_sel->m_vertex2;
			bool removing_vertices = (m_subentity_highlight & HSUBENTITY_VERTICES) && m_vertex1
				&& (remove_sel->m_subentity_highlight & HSUBENTITY_VERTICES) && remove_sel->m_vertex1;

			if(removing_edges || removing_vertices)
			{
				int max = (remove_sel->m_vertex_count>m_vertex_count ? remove_sel->m_vertex_count : m_vertex_count);
				int * rem_vert1 = new int [max];
				int * rem_vert2 = new int [max];
				int rem_vert_count = 0;
				int rem_edge_count = 0;
				int last_edge_index = 0;

				//walk through remove_sel's edges
				if(remove_sel->m_vertex2)
				{
					for(int i=0; (i < remove_sel->m_vertex_count && remove_sel->m_vertex2[i] != -1); ++i)
					{
						int current_rem_vert1 = remove_sel->m_vertex1[i];
						int current_rem_vert2 = remove_sel->m_vertex2[i];

						int v1_lookup;
						int v2_lookup;

						//if it occurs in the subshell...
						if(current_rem_vert1 < m_vertex_map_count && m_vertex_map->LookupItem(current_rem_vert1, &v1_lookup) != VHASH_STATUS_FAILED
							&& current_rem_vert2 < m_vertex_map_count && m_vertex_map->LookupItem(current_rem_vert2, &v2_lookup) != VHASH_STATUS_FAILED)
						{
							//see if it's highlighted
							for(int j=0; (j < m_vertex_count && m_vertex2[j] != -1); ++j)
							{
								//edges could appear in either order
								if(m_vertex1[j] == current_rem_vert1 && m_vertex2[j] == current_rem_vert2
									|| m_vertex2[j] == current_rem_vert1 && m_vertex1[j] == current_rem_vert2)
								{
									//add it to the list of edges to be removed
									rem_vert1[rem_vert_count] = v1_lookup;
									rem_vert2[rem_vert_count] = v2_lookup;
									++rem_vert_count;
									m_vertex1[j] = -1;	//mark for removal (only mark vertex1)
									//nobreak;
								}
							}
						}
						last_edge_index = i;
					}
				}

				//Note: from here on, rem_edge_count>0 implies removing_edges==true
				if(removing_edges)
					rem_edge_count = rem_vert_count;

				//if we're removing vertices, get them too
				if(removing_vertices)
				{
					//walk through remove_sel's vertices
					for(int i=(remove_sel->m_vertex2 ? last_edge_index+1 : 0); i<remove_sel->m_vertex_count; ++i)
					{
						int current_rem_vert = remove_sel->m_vertex1[i];
						int v_lookup;

						//if it occurs in the subshell...
						if(current_rem_vert >= 0 && current_rem_vert < m_vertex_map_count && m_vertex_map->LookupItem(current_rem_vert, &v_lookup) != VHASH_STATUS_FAILED)
						{
							//see if it's highlighted
							for(int j=0; j<m_vertex_count; ++j)
							{
								if(m_vertex1[j]>=0 && m_vertex1[j] == current_rem_vert)
								{
									//add it to the list of vertices to be removed
									rem_vert1[rem_vert_count++] = v_lookup;
									//only remove the vertex from the highlighted array if it is a pure vertex (not connected to an edge)
									if( ! m_vertex2 || (m_vertex2 && m_vertex2[j] == -1) )
										m_vertex1[j] = -1;	//mark for removal
									//nobreak;
								}
							}
						}
					}
				}
				
				//if we are removing anything, fix up the local record of what's highlighted
				if(rem_edge_count > 0 || (removing_vertices && rem_vert_count > 0))
				{
					int * new_vert1 = 0;
					int * new_vert2 = 0;
					int new_vert_count = 0;
					char * vert_vis = new char [rem_vert_count];
					//we use rem_vert count here because rem_vert_count >= rem_edge_count and we need the larger of the 2
					memset(vert_vis, 0, rem_vert_count);

					//if we're removing everything don't bother making up a new list
					//we look at rem_edge_count here because removing a vertex might not actually affect the new count
					if(rem_edge_count < m_vertex_count)
					{
						new_vert1 = new int [m_vertex_count - rem_edge_count];
						if (m_vertex2)
						{
							new_vert2 = new int [m_vertex_count - rem_edge_count];
							memset(new_vert2, -1, (m_vertex_count - rem_edge_count)*sizeof(int));
						}
						//make up new m_vertex1 m_vertex2 lists
						for(int i=0; i<m_vertex_count; ++i)
						{
							if(m_vertex1[i] != -1)
							{
								new_vert1[new_vert_count] = m_vertex1[i];
								if (m_vertex2)
									new_vert2[new_vert_count] = m_vertex2[i];
								new_vert_count++;
							}
						}
					}

					//out with the old, in with the new.
					delete [] m_vertex1;
					m_vertex1 = new_vert1;
					if (m_vertex2)
					{
						delete [] m_vertex2;
						m_vertex2 = new_vert2;
					}
					m_vertex_count = new_vert_count;

					//update the subshell
					if(rem_edge_count)
						HC_MSet_Specific_Edge_Vis(hl_key[k], rem_edge_count, rem_vert1, rem_vert2, vert_vis);
					if(removing_vertices && rem_vert_count > 0)
						HC_MSet_Specific_Vertex_Vis(hl_key[k], rem_vert_count, rem_vert1, vert_vis);

					delete [] vert_vis;
					unhighlighted_something = true;
				}

				delete [] rem_vert1;
				delete [] rem_vert2;
			}
		}
	}

	delete [] hl_key;
	return unhighlighted_something;
}

static void check_points_in_use(int count, const int * flist, char * appears)
{
	int i=0, j, face_size;

	while(i<count)
	{
		face_size=flist[i++];
		
		for(j=0; j<face_size; ++j)
			appears[flist[i+j]]=(char)0x01;

		i+=face_size;
	}
}

void HSubentitySelItem::MergeMaps(HSubentitySelItem * that)
{

	ASSERT(m_face_map_count == that->m_face_map_count);
	ASSERT(m_face_map != 0);
	ASSERT(that->m_face_map != 0);

	ASSERT(m_vertex_map_count == that->m_vertex_map_count);
	ASSERT(m_vertex_map != 0);
	ASSERT(that->m_vertex_map != 0);

	//walk that face map and merge
	for(int i=0; i<m_face_map_count; ++i)
	{
		if(that->m_face_map->LookupItem(i) != VHASH_STATUS_FAILED &&
			m_face_map->LookupItem(i) == VHASH_STATUS_FAILED)
			m_face_map->InsertItem(i);
	}

	//walk that vertex map and merge
	for(int i=0; i<m_vertex_map_count; ++i)
	{
		if(that->m_vertex_map->LookupItem(i) != VHASH_STATUS_FAILED &&
			m_vertex_map->LookupItem(i) == VHASH_STATUS_FAILED)
			m_vertex_map->InsertItem(i);
	}

}

bool HSubentitySelItem::Delete()
{
	// unhighlight it first
	UnHighlight();
	
	if(HC_Show_Existence_By_Key(m_key, "self"))
	{
		int actual_count, shell_face_cnt, shell_pts_cnt, * shell_flist;
		int	* faces = new int [m_face_map_count];
		int	* verts = new int [m_vertex_map_count];
	
		//walk face map and log faces to be deleted.
		actual_count=0;
		for(int i=0; i<m_face_map_count; ++i)
		{
			if(m_face_map->LookupItem(i) != VHASH_STATUS_FAILED)
				faces[actual_count++]=i;
		}

		if(actual_count>0)
			HC_Delete_Specific_Shell_Faces(m_key, actual_count, faces);

		//walk vertex map and log vertices to be deleted.
		//don't delete vertices that are still being used
		HC_Show_Shell_Size(m_key, &shell_pts_cnt, &shell_face_cnt);
		char *	in_use = new char [shell_pts_cnt];
		memset(in_use, 0x00, shell_pts_cnt);

		if(shell_face_cnt)
		{
			shell_flist = new int [shell_face_cnt];
			HC_Show_Shell(m_key, &shell_pts_cnt, 0, &shell_face_cnt, shell_flist);
		
			check_points_in_use(shell_face_cnt, shell_flist, in_use);

			delete [] shell_flist;
		}

		actual_count=0;
		for(int i=0; i<m_vertex_map_count; ++i)
		{
			if(in_use[i]==0 && m_vertex_map->LookupItem(i) != VHASH_STATUS_FAILED)
				verts[actual_count++]=i;
		}

		if(shell_pts_cnt==actual_count)
		{
			HUtility::DeleteManipulators(m_key);
			HC_Delete_By_Key(m_key);
		}
		else if(actual_count>0)
			HC_Delete_Specific_Shell_Points(m_key, actual_count, verts);	

		delete [] faces;
		delete [] verts;
		delete [] in_use;
	}

	return true;
}

void HSubentitySelItem::GetFaces(int * faces)
{
	if(m_faces)
		memcpy(faces, m_faces, m_face_count*sizeof(int));
	else
		*faces=0; 
}

void HSubentitySelItem::GetVertices(int * vertex1, int * vertex2) 
{ 
	if(m_vertex1)
		memcpy(vertex1, m_vertex1, m_vertex_count*sizeof(int));
	else
		*vertex1=0;

	if(m_vertex2)
		memcpy(vertex2, m_vertex2, m_vertex_count*sizeof(int));
	else
		*vertex2=0;	
}

void HSubentitySelItem::GetMaps(int * face_map, int * vertex_map) 
{
	memset(face_map, 0xFF, m_face_map_count);
	memset(vertex_map, 0xFF, m_vertex_map_count);

	// TODO these need to be populated from the hash
// 	if(m_face_map) {
// 			
// 	}
// 
// 	if(m_vertex_map) {
// 
// 	}
	
}

void HSubentitySelItem::GetMapCounts(int * face_map_count, int * vertex_map_count) 
{
	*face_map_count=m_face_map_count;
	*vertex_map_count=m_vertex_map_count;
}

bool HRegionSelItem::Equals(const HSelectionItem* i2) const
{
	// if primary selection key is same
	if( this->GetKey() != i2->GetKey() )
		return false;

	// return false if they are of different type
	if( !streq(this->GetName(), i2->GetName()) )
		return false;

	// safe to typecast now
	HRegionSelItem * smart_sel2 = (HRegionSelItem*) i2;

	// if the include keys in the path are same
	int incl_count;
	if( (incl_count = this->GetIncludeCount()) != smart_sel2->GetIncludeCount() )
		return false;

	const HC_KEY * ikeys1 = this->GetIncludeKeys();
	const HC_KEY * ikeys2 = smart_sel2->GetIncludeKeys();
	for (int i = 0; i < incl_count; i++)
	{
		if(ikeys1[i] != ikeys2[i])
			return false;
	}

	if (this->m_Region != smart_sel2->GetRegion())
		return false;
	
	return true;
}

void HSmartSelItem::GetIncludeKeys(HC_KEY *includes) const {
	const HC_KEY * const temp = GetIncludeKeys();
	int include_count = GetIncludeCount();

	memcpy(includes, temp, include_count * sizeof(HC_KEY));
}


const HC_KEY *HSmartSelItem::GetFullPathKeys( HBaseView *view )
{
	if (!m_FullPath) { 
		if(m_pSelectionSet->GetReferenceSelection())
		{
			HC_KEY seg_key=m_key;
			char type[MVO_BUFFER_SIZE];
			int i;
			m_FullPathCount = m_IncludePath.count + 1;
			m_FullPath = new HC_KEY[ m_FullPathCount ];

			HC_Show_Key_Type(m_key, type);
			if(!streq(type, "segment"))
				seg_key= HC_Show_Owner_Original_Key(m_key);
				
			//-- Set up the path for PShow --
			m_FullPath[0]=seg_key;
			for(i=0; i<m_IncludePath.count; ++i)
				m_FullPath[i+1]=m_IncludePath.keys[m_IncludePath.count-i-1];
		}
		else
		{
			m_FullPathCount = m_IncludePath.count + 2;
			m_FullPath = new HC_KEY[ m_FullPathCount ];
			m_FullPath[0] = m_key;
			memcpy(&m_FullPath[1], &m_IncludePath.keys[0], m_IncludePath.count*sizeof(HC_KEY));
			m_FullPath[ m_IncludePath.count + 1 ] = view->GetViewKey();
		}
	}
	UNREFERENCED (view);
	return m_FullPath;
}

void HSmartSelItem::GetFullPathKeys (HBaseView *view, HC_KEY *path) {
	const HC_KEY *temp = GetFullPathKeys(view);
	int path_size = GetFullPathCount(view);

	memcpy(path, temp, path_size * sizeof(HC_KEY));
}


bool HSmartSelItem::Equals(const HSelectionItem* i2) const
{
	// if primary selection key is same
	if( this->GetKey() != i2->GetKey() )
		return false;

	// return false if they are of different type
	if( !streq(this->GetName(), i2->GetName()) )
		return false;

	// safe to typecast now
	HSmartSelItem * smart_sel2 = (HSmartSelItem*) i2;

	// if the include keys in the path are same
	int incl_count;
	if( (incl_count = this->GetIncludeCount()) != smart_sel2->GetIncludeCount() )
		return false;

	const HC_KEY * ikeys1 = this->GetIncludeKeys();
	const HC_KEY * ikeys2 = smart_sel2->GetIncludeKeys();
	for (int i = 0; i < incl_count; i++)
	{
		if(ikeys1[i] != ikeys2[i])
			return false;
	}

	return true;
}

#if 0
// finds if the given style segment is applied on the currently opened segment with given conditions
// returns the conditional style key if found otherwise returns INVALID_KEY
static HC_KEY Show_Existence_Conditional_Style(const HC_KEY which_style, const char * what_condition )
{
	HC_KEY return_key = INVALID_KEY;
	// check if same conditional style already exists
	// this is particularly true for non-instanced multiple entities 
	// as they get pushed into a common highlight segment
	HC_Begin_Contents_Search(".", "style");
		HC_KEY foundkey = INVALID_KEY;
		char type[MVO_BUFFER_SIZE];
		char condition[MVO_BUFFER_SIZE];
		while(HC_Find_Contents(type, &foundkey))
		{
			if(HC_Show_Style_Segment(foundkey) == which_style)
			{
				HC_Show_Conditional_Style(foundkey, 0, condition);
				if(strieq(condition, what_condition))
				{
					return_key = foundkey;
					break;
				}
			}
		}
	HC_End_Contents_Search();

	return return_key;
}
#endif

int HSmartSelItem::GetHighlightedRefListCount()
{
	if (!m_pSelKeyList)
		return 0;
	return vlist_count(m_pSelKeyList);
}

	
void HSmartSelItem::GetHighlightedRefList(HC_KEY * keys)
{
	unsigned int i=0;

	if (m_pSelKeyList)
	{
		vlist_reset_cursor(m_pSelKeyList);
		do 
		{
			keys[i++]=(HC_KEY)vlist_peek_cursor(m_pSelKeyList);
			vlist_advance_cursor(m_pSelKeyList);
		} while(i < vlist_count(m_pSelKeyList));
	}
}


HC_KEY HSmartSelItem::GetHighlightedRefKey()
{
	HC_KEY ret = INVALID_KEY;
	if (m_pSelKeyList) {
		if(vlist_peek_first(m_pSelKeyList))
		{
			ret = (HC_KEY)vlist_peek_first(m_pSelKeyList);
			vlist_remove_first(m_pSelKeyList);
		}
	}
	return ret;
}

void HSmartSelItem::FixIncludePath()
{
	int cnt = GetIncludeCount();
	HC_KEY * new_path = new HC_KEY [cnt];
	memcpy(new_path, GetIncludeKeys(), cnt*sizeof(HC_KEY));
	validate_include_path(m_key, &cnt, &new_path, true, 
		m_pSelectionSet->GetView()->GetViewKey(), m_pSelectionSet->GetView()->GetIncludeLinkKey());
	if(cnt!=GetIncludeCount())
		UpdateIncludePath(cnt, new_path);
	else 
		delete [] new_path;
}
	
void HSmartSelItem::AddHighlightedRefKey(HC_KEY key)
{
	if(key!=INVALID_KEY) 
	{
		if (!m_pSelKeyList)
			m_pSelKeyList = new_vlist(malloc, free);
		vlist_add_last(m_pSelKeyList, (void*)key);
	}
}

//#define DEBUG_CSIH
#if defined(DEBUG_CSIH)
#include <stdio.h>
#include <stdarg.h>
#include <Windows.h>

#undef min
#undef max

inline void dprintf (char const * format, ...) {
	char buffer[1024];
	va_list arguments;

	va_start (arguments, format);
	vsprintf (buffer, format, arguments);
	va_end (arguments);
#ifdef _DEBUG
#ifdef _MSC_VER
#ifndef _WIN32_WCE
	OutputDebugString (buffer);
#endif
#endif
#endif
	fprintf(stdout, buffer);
	fflush(stdout);
}

#endif

bool HSmartSelItem::Highlight()
{

	validate_include_path(m_key, &m_IncludePath.count, &m_IncludePath.keys,
		m_pSelectionSet->GetReferenceSelection(), 
		m_pSelectionSet->GetView()->GetViewKey(), m_pSelectionSet->GetView()->GetIncludeLinkKey());

	if (m_pSelectionSet->GetReferenceSelection())
	{
		if(m_pSelectionSet->GetSelectionSegment() == INVALID_KEY)
			m_pSelectionSet->UpdateHighlightStyle();

		if(m_pSelectionSet->GetIsSelection(m_key))
			return true;

		HC_KEY	use_key = m_key;
		int		path_count = 0;
		HC_KEY	path[MVO_BUFFER_SIZE];	
		HC_KEY	seg_key=INVALID_KEY;
		char	type_string[128];

		HHighlightObjectType	hl_type = IsSegment() ? HHighlightSegment : HHighlightGeometry;

		if (prepare_and_validate_ppath(use_key, m_IncludePath.count, m_IncludePath.keys, m_pSelectionSet, hl_type, type_string, seg_key, path_count, path, m_pSelectionSet->GetUseDefinedHighlight())) {
		
			if (m_pSelectionSet->GetUseDefinedHighlight()) {

				const char * opt = 0;
				switch (m_pSelectionSet->GetReferenceSelectionType()) {
				case RefSelSpriting: opt = "quick moves = spriting"; break;
				case RefSelOff: opt = ""; break;
				case RefSelDefault:
				default: opt = "quick moves = on"; break;
				}

				HC_Define_Highlight(path_count, path, m_pSelectionSet->GetHighlightStyleName(), opt);
			}
			else {
				HC_KEY	ref_key = ref_highlight_common(use_key, seg_key, path_count, path, m_pSelectionSet, hl_type, type_string);			
				AddHighlightedRefKey(ref_key);	//add the new key to the list of highlighted geometry
			}
		}
	}
	else
	{
		HC_KEY seg_key = m_key;

		// if it is an entity, move it down under a segment
		if (!IsSegment()) 
		{
			// do not do it if the owner is already a highlight segment
			// this is possible is a different instance of same geometry is selected
			seg_key = HC_Show_Owner_Original_Key(m_key);
			if (!m_pSelectionSet->IsHighlightSegment(seg_key))
			{
				char seg_name[MVO_BUFFER_SIZE];
				HC_Show_Segment_Name(seg_key, seg_name);
				if (strncmp(seg_name, base_entity_proxy_seg_name, sizeof(base_entity_proxy_seg_name) - 1) != 0) {
					char hilite_seg_name[256];
					char * end = hilite_seg_name;
					strcpy(end, base_entity_proxy_seg_name);
					end += sizeof(base_entity_proxy_seg_name) - 1;
					end = get_key_tag(m_key, end);

					HC_Open_Segment_By_Key(seg_key);{
						seg_key = HC_Open_Segment(hilite_seg_name);{
							HC_Move_By_Key(m_key, ".");
						}HC_Close_Segment();
					}HC_Close_Segment();
				}
				
				HC_Open_Segment_By_Key(seg_key);{
					HC_Open_Segment(entity_proxy_tag_seg_name);{
						m_entity_proxy_seg_tag = HC_Create_Segment("");
					}HC_Close_Segment();
				}HC_Close_Segment();

				if (!m_pSelectionSet->IsHighlightSegment(seg_key))
					m_pSelectionSet->RegisterHighlightSeg(seg_key);
			}
		}

		char condition_tag[MVO_BUFFER_SIZE];
		condition_tag[0] = '\0';

		if (m_IncludePath.count > 1) 
		{
			// tag all the includers and prepare our final condition tag
			char base_tag[MVO_BUFFER_SIZE];
			GetBaseTag(base_tag);
			for(int i = 1; i < m_IncludePath.count; i++)
			{
				char sel_tag[MVO_BUFFER_SIZE];
				sprintf(sel_tag, "%s_%d", base_tag, i); //MAKE_MVO_SELECT_TAG

				HC_Define_System_Options("no update control");
				HC_Open_Segment_By_Key(HC_Show_Owner_Original_Key(m_IncludePath.keys[i]));
					HC_Set_Conditions(sel_tag);
				HC_Close_Segment();
				HC_Define_System_Options("update control");

				strcat(condition_tag, sel_tag);
				if(i < m_IncludePath.count-1)
					strcat(condition_tag, " and ");
			}
		}

		HSelectionSegmentConditionInfo * ssci = m_pSelectionSet->GetSelectionSegmentConditionInfo();

		HConditionStyleInfoHash * csih = ssci->Lookup(seg_key);

		if (!csih) {
			csih = new HConditionStyleInfoHash();
			ssci->InsertItem(seg_key, csih);
#if defined(DEBUG_CSIH)
			dprintf("I %p %p\n", (void*)seg_key, (void*)csih);
#endif
		}

		HC_KEY needed_highlight = m_pSelectionSet->GetHighlightStyle();
		HConditionStyleInfo * csi = 0;
		int n = 0;

		if (csih->Count() > 0) {		
			while (1) {
				csih->LookupNthItem(condition_tag, n++, &csi);
				if (!csi || (csi->hightlight_style == needed_highlight))
					break;
			}
		}

		if (!csi) {
			csi = new HConditionStyleInfo(needed_highlight);
			csih->InsertItem(condition_tag, csi);
		}

		if (csi->use_count == 0) {
			// set the conditional style
			HC_Open_Segment_By_Key(seg_key);
			csi->condition_style = HC_Conditional_Style_By_Key(needed_highlight, condition_tag);
			HC_Close_Segment();

			HConditionStyleKeyInfoHash * cski = m_pSelectionSet->GetConditionStyleKeyInfo();
			cski->InsertItem(csi->condition_style, csi);
		}

		m_ConditionStyle = csi->condition_style;
		csi->use_count++;

		m_pSelectionSet->GetView()->SetGeometryChanged();
	}
	
	return true;
}

void HSmartSelItem::UpdateIncludePath(int incl_count, HC_KEY * incl_keys)
{
	m_IncludePath.count=incl_count;
	if(m_IncludePath.keys)
		delete [] m_IncludePath.keys;
	m_IncludePath.keys = incl_keys;
}

bool HSubentitySelItem::Highlight()
{
	HSubentityPassthrough pt;

	FixIncludePath();

	HC_Show_Shell_Size(m_key, &m_vertex_map_count, 0);

	HC_Show_Shell_Face_Count (m_key, &m_face_map_count);
	
	delete m_face_map;
	m_face_map = new SubentityMap();
	delete m_vertex_map;
	m_vertex_map = new SubentityMap();

	pt.face_count=m_face_count;
	pt.faces=m_faces;
	pt.vertex_count=m_vertex_count;
	pt.vertex1=m_vertex1;
	pt.vertex2=m_vertex2;
	pt.maintain_maps=m_maintain_maps;
	pt.vertex_map_count=m_vertex_map_count;
	pt.vertex_map=m_vertex_map;
	pt.face_map_count=m_face_map_count;
	pt.face_map=m_face_map;
	pt.highlight_subentity=m_subentity_highlight;

	//always uses over-draw to create highlighted geometry

	HC_KEY	use_key = m_key;
	int		path_count = 0;
	HC_KEY	path[MVO_BUFFER_SIZE];	
	HC_KEY	seg_key=INVALID_KEY;
	char	type_string[128];

	HHighlightObjectType	hl_type = HHighlightSubentity;

	if (prepare_and_validate_ppath(use_key, GetIncludeCount(), GetIncludeKeys(), m_pSelectionSet, hl_type, type_string, seg_key, path_count, path)) {

		HC_KEY	ref_key = ref_highlight_common(use_key, seg_key, path_count, path, m_pSelectionSet, hl_type, type_string, -1, &pt);

		if(!m_maintain_maps)
		{
			delete m_face_map;
			m_face_map=0;
			m_face_map_count=0;

			delete m_vertex_map;
			m_vertex_map=0;
			m_vertex_map_count=0;		
		}

		//add the new key to the list of highlighted geometry
		AddHighlightedRefKey(ref_key);
	}

	return true;
}

#if 1

int HRegionSelItem::IncrEdgeHighlightCount(HC_KEY key, HShellEdge const & oEdge)
{
	char options[64] = {""};
	int highlight_count = 0;
	void * hash_value = 0;
	
	HC_Open_Geometry(key);{

		struct vhash_s * pEdgeHash;

		if(HC_Show_Existence("user options = edgelookup")){
			HC_Show_One_User_Option("edgelookup", options);
			sscanf(options, "%p", &pEdgeHash);
		} else {
			pEdgeHash = new_vhash(1,malloc,free);
			sprintf(options, "edgelookup = %p", pEdgeHash);
			HC_Set_User_Options(options);
		}
		
		char string_key[128];
		sprintf(string_key, "%x %x", oEdge.GetFirst(), oEdge.GetSecond());
		vhash_status_t tStatus = vhash_lookup_string_key_item(pEdgeHash, string_key, &hash_value);
		highlight_count = static_cast<int>(MVO_V2I(hash_value));

		if(tStatus != VHASH_STATUS_SUCCESS){
			highlight_count = 1;
			hash_value = MVO_I2V(highlight_count);
			vhash_insert_string_key_item(pEdgeHash, string_key, hash_value);
		}
		else {
			++highlight_count;
			void * not_used;
			hash_value = MVO_I2V(highlight_count);
			vhash_replace_string_key_item(pEdgeHash, string_key, hash_value, &not_used);
		}

	}HC_Close_Geometry();

	return highlight_count;
}


int HRegionSelItem::DecrEdgeHighlightCount(HC_KEY key, HShellEdge const & oEdge)
{
	char options[64] = {""};
	int highlight_count = 0;
	void * hash_value = 0;
	
	HC_Open_Geometry(key);{
		HC_Show_One_User_Option("edgelookup", options);
		struct vhash_s * pEdgeHash = 0;
		sscanf(options, "%p", &pEdgeHash);
		char string_key[128];
		sprintf(string_key, "%x %x", oEdge.GetFirst(), oEdge.GetSecond());
		vhash_lookup_string_key_item(pEdgeHash, string_key, &hash_value);
		highlight_count = static_cast<int>(MVO_V2I(hash_value));
		--highlight_count;
		hash_value = MVO_I2V(highlight_count);
		void * not_used;
		vhash_replace_string_key_item(pEdgeHash, string_key, hash_value, &not_used);

		if(highlight_count == 0){
			vhash_remove_string_key_item(pEdgeHash, string_key, &not_used);
			if(vhash_count(pEdgeHash) == 0){
				delete_vhash(pEdgeHash);
				HC_UnSet_One_User_Option("edgelookup");
			}
		}

	}HC_Close_Geometry();

	return highlight_count;
}

#else

#include <map>

typedef std::map<int const, int> EdgeLookup;

int HRegionSelItem::IncrEdgeHighlightCount(HC_KEY key, HShellEdge const & oEdge)
{
	int highlight_count = 0;
	char options[64] = {""};
	
	HC_Open_Geometry(key);{

		EdgeLookup * pEdgeMap = 0;

		if(HC_Show_Existence("user options = edgelookup")){
			HC_Show_One_User_Option("edgelookup", options);
			sscanf(options, "%p", &pEdgeMap);
		} else {
			pEdgeMap = new EdgeLookup();
			sprintf(options, "edgelookup = %p", pEdgeMap);
			HC_Set_User_Options(options);
		}
		
		EdgeLookup::iterator iter = pEdgeMap->find(oEdge.GetIndex());
		if(iter == pEdgeMap->end()){
			highlight_count = 1;
			pEdgeMap->insert(std::make_pair(oEdge.GetIndex(), highlight_count));
		}
		else {
			highlight_count = ++iter->second;
		}

	}HC_Close_Geometry();

	return highlight_count;
}


int HRegionSelItem::DecrEdgeHighlightCount(HC_KEY key, HShellEdge const & oEdge)
{
	int highlight_count = 0;
	char options[64] = {""};
	
	HC_Open_Geometry(key);{
		HC_Show_One_User_Option("edgelookup", options);
		EdgeLookup * pEdgeMap = 0;
		sscanf(options, "%p", &pEdgeMap);
		EdgeLookup::iterator iter = pEdgeMap->find(oEdge.GetIndex());
		highlight_count = --iter->second;

		if(highlight_count == 0){
			pEdgeMap->erase(oEdge.GetIndex());
			if(pEdgeMap->empty()){
				delete pEdgeMap;
				HC_UnSet_One_User_Option("edgelookup");
			}
		}

	}HC_Close_Geometry();

	return highlight_count;
}

#endif

void HRegionSelItem::HighlightFaces()
{
	if(m_pSelectionSet->GetReferenceSelection())
	{
		int i, count = GetHighlightedRefListCount();
		HC_KEY * keys = new HC_KEY[count];
		GetHighlightedRefList(keys);
		
		for(i=0; i<count; ++i)
		{
			HC_Open_Segment_By_Key(keys[i]);
				if(m_pSelectionSet->HighlightRegionFacesAutoVisibility() == FacesForcedOn)
					HC_Set_Visibility("faces = on");
				else if(m_pSelectionSet->HighlightRegionFacesAutoVisibility() == FacesForcedOff)
					HC_Set_Visibility("faces = off");
			HC_Close_Segment();
		}

		delete [] keys;
	}
}

void HRegionSelItem::UnHighlightFaces()
{
	if(m_pSelectionSet->GetReferenceSelection())
	{
		int i, count = GetHighlightedRefListCount();
		HC_KEY * keys = new HC_KEY[count];
		GetHighlightedRefList(keys);
		
		for(i=0; i<count; ++i)
		{
			HC_Open_Segment_By_Key(keys[i]);
				HC_UnSet_One_Visibility("faces");
			HC_Close_Segment();
		}

		delete [] keys;
	}
}

void HRegionSelItem::HighlightEdges()
{
	if(m_pSelectionSet->GetReferenceSelection())
	{
		int i, count = GetHighlightedRefListCount();
		HC_KEY * keys = new HC_KEY[count];
		GetHighlightedRefList(keys);
		
		for(i=0; i<count; ++i)
		{
			HC_Open_Segment_By_Key(keys[i]);
				if(m_pSelectionSet->HighlightRegionEdgesAutoVisibility() == AllEdges)
					HC_Set_Visibility("edges = on");
				else if(m_pSelectionSet->HighlightRegionEdgesAutoVisibility() == PerimeterEdges)
					HC_Set_Visibility("edges = perimeter only");
			HC_Close_Segment();
		}

		delete [] keys;
	}
	else
	{
		float r,g,b;
		char style[32] = {""};
		HC_Open_Segment_By_Key(m_pSelectionSet->GetHighlightStyle());{
			HC_Show_One_Color_By_Value("faces", style, &r, &g, &b);
		}HC_Close_Segment();

		for(int iFaceIndex=0; iFaceIndex<m_oRegionObj.GetFaceCount(); ++iFaceIndex){
			for(int iEdgeIndex=0; iEdgeIndex<m_oRegionObj.GetFace(iFaceIndex).GetEdgeCount(); ++iEdgeIndex){
				m_oRegionObj.GetFace(iFaceIndex).OpenEdge(iEdgeIndex);{
					HShellEdge const oEdge = m_oRegionObj.GetFace(iFaceIndex).GetEdge(iEdgeIndex);
					if(IncrEdgeHighlightCount(m_key, oEdge) == 1){
						if(m_pSelectionSet->HighlightRegionEdgesAutoVisibility() == AllEdges)
							HC_Set_Visibility("edges = on");
						else if(m_pSelectionSet->HighlightRegionEdgesAutoVisibility() == PerimeterEdges)
							HC_Set_Visibility("edges = perimeter only");
						HC_Set_Color_By_Value("edges", "RGB", r, g, b);
					}
				}m_oRegionObj.GetFace(iFaceIndex).CloseEdge();
			}
		}
	}
}

void HRegionSelItem::UnHighlightEdges()
{
	if(!m_pSelectionSet->GetReferenceSelection())
	{
		for(int iFaceIndex=0; iFaceIndex<m_oRegionObj.GetFaceCount(); ++iFaceIndex)
		{
			for(int iEdgeIndex=0; iEdgeIndex<m_oRegionObj.GetFace(iFaceIndex).GetEdgeCount(); ++iEdgeIndex)
			{
				m_oRegionObj.GetFace(iFaceIndex).OpenEdge(iEdgeIndex);{
					HShellEdge const oEdge = m_oRegionObj.GetFace(iFaceIndex).GetEdge(iEdgeIndex);
					if(DecrEdgeHighlightCount(m_key, oEdge) == 0)
					{
						if(m_pSelectionSet->HighlightRegionEdgesAutoVisibility())
							HC_UnSet_Visibility();
						HC_UnSet_Color();
					}
				}
				m_oRegionObj.GetFace(iFaceIndex).CloseEdge();
			}
		}
	}
}


bool HRegionSelItem::Highlight()
{

	FixIncludePath();
	
	if(m_pSelectionSet->GetReferenceSelection())
	{
		HC_KEY	use_key = m_key;
		int		path_count = 0;
		HC_KEY	path[MVO_BUFFER_SIZE];	
		HC_KEY	seg_key=INVALID_KEY;
		char	type_string[128];

		HHighlightObjectType	hl_type = HHighlightRegion;

		if (prepare_and_validate_ppath(use_key, GetIncludeCount(), GetIncludeKeys(), m_pSelectionSet, hl_type, type_string, seg_key, path_count, path)) {

			HC_KEY	ref_key = ref_highlight_common(use_key, seg_key, path_count, path, m_pSelectionSet, hl_type, type_string, m_Region);

			//add the new key to the list of highlighted geometry
			AddHighlightedRefKey(ref_key);
		}
	}
	else
	{
		char style[32];
		char buffer[4096];
		float r,g,b;

		HC_Open_Segment_By_Key(m_pSelectionSet->GetHighlightStyle());{
			HC_Show_One_Color_By_Value("faces", style, &r, &g, &b);
		}HC_Close_Segment();

		HC_Open_Geometry(m_key);{
			HC_Open_Region(m_Region);{
				HC_Set_Color_By_Value("faces", "RGB", r, g, b);
			}HC_Close_Region();
			sprintf(buffer, "hregion%d highlighted", m_Region);
			HC_Set_User_Options(buffer);
		}HC_Close_Geometry();
	}

	if(m_pSelectionSet->HighlightRegionEdgesAutoVisibility())
		HighlightEdges();

	if(m_pSelectionSet->HighlightRegionFacesAutoVisibility())
		HighlightFaces();

	return true;
}

bool HRegionSelItem::UnHighlight()
{
	if(m_pSelectionSet->GetReferenceSelection())
	{
		CommonUnhighlightRef();
	}
	else
	{
		char status[32];
		HC_Show_Key_Status(m_key, status);
		if(!streq(status, "invalid")){
			HC_Open_Geometry(m_key);
			HC_Open_Region(m_Region);
			HC_UnSet_Color();
			HC_Close_Region();
			char buffer[4096];
			sprintf(buffer, "hregion%d highlighted", m_Region);
			HC_UnSet_One_User_Option(buffer);
			HC_Close_Geometry();
		}

		if(m_pSelectionSet->HighlightRegionEdgesAutoVisibility())
			UnHighlightEdges();

		if(m_pSelectionSet->HighlightRegionFacesAutoVisibility())
			UnHighlightFaces();
	}
	
	return true;
}

void HSmartSelItem::CommonUnhighlightRef()
{
	HC_KEY ref_key;
	while ((ref_key = GetHighlightedRefKey()) != INVALID_KEY)
	{
		if (HC_Show_Existence_By_Key(ref_key, "self")) {
			char	type_string[128];
			HC_Show_Key_Type(ref_key, type_string);
			if (strstr(type_string, "segment")) {
				HC_KEY owner = HC_Show_Owner_Original_Key(ref_key);
				HC_Delete_By_Key(ref_key);
				if (HC_Show_Existence_By_Key(owner, "subsegment, geometry, first match") == 0)
					HC_Delete_By_Key(owner);
			}
			else
				HC_Delete_By_Key(ref_key);
		}
	}

}

bool HSmartSelItem::UnHighlight()
{

	if(m_pSelectionSet->GetReferenceSelection())
	{
		if (m_pSelectionSet->GetUseDefinedHighlight()) {
			HC_KEY	use_key = m_key;
			int		path_count = 0;
			HC_KEY	path[MVO_BUFFER_SIZE];	
			HC_KEY	seg_key=INVALID_KEY;
			char	type_string[128];

			HHighlightObjectType	hl_type = IsSegment() ? HHighlightSegment : HHighlightGeometry;

			if (prepare_and_validate_ppath(use_key, m_IncludePath.count, m_IncludePath.keys, m_pSelectionSet, hl_type, type_string, seg_key, path_count, path, true))
				HC_UnDefine_Highlight(path_count, path, m_pSelectionSet->GetHighlightStyleName(), "exact");
		}
		else {		
			CommonUnhighlightRef();
		}
		
		return true;
	}

	else
	{
		// if we are not highlighted currently, return false
		if( m_ConditionStyle == INVALID_KEY )
			return false;

		HC_KEY seg_key = m_key;

		if (!HC_Show_Existence_By_Key(m_key, "self"))
			return true;

		char condition_tag[MVO_BUFFER_SIZE]; 
		condition_tag[0] = '\0';

		// untag all the includers
		if (m_IncludePath.count > 1) 
		{
			char base_tag[MVO_BUFFER_SIZE];
			char sel_tag[MVO_BUFFER_SIZE];
			GetBaseTag(base_tag);
			for(int i = 1; i < m_IncludePath.count; i++)
			{
				sprintf(sel_tag, "%s_%d", base_tag, i); //MAKE_MVO_SELECT_TAG
				strcat(condition_tag, sel_tag);
				if(i < m_IncludePath.count-1)
					strcat(condition_tag, " and ");

				HC_Open_Segment_By_Key(HC_Show_Owner_Original_Key(m_IncludePath.keys[i]));
					HC_UnSet_One_Condition(sel_tag);
				HC_Close_Segment();
			}
		}

		HConditionStyleKeyInfoHash * cski = m_pSelectionSet->GetConditionStyleKeyInfo();

		HConditionStyleInfo * csi = 0;
		cski->LookupItem(m_ConditionStyle, &csi);

		csi->use_count--;

		if (csi->use_count == 0) {

			HC_KEY highlight_seg_orig_key = HC_Show_Owner_Original_Key(m_ConditionStyle);
			HC_KEY highlight_seg_key = HC_Show_Owner_By_Key(m_ConditionStyle, 0);

			HC_Delete_By_Key(m_ConditionStyle);

			cski->Remove(m_ConditionStyle);

			HSelectionSegmentConditionInfo * ssci = m_pSelectionSet->GetSelectionSegmentConditionInfo();

			bool renumbered = false;
			HConditionStyleInfoHash * csih = ssci->Lookup(highlight_seg_orig_key);
			if (!csih) {
				csih = ssci->Lookup(highlight_seg_key);
				renumbered = true;
			}

			csih->Remove(condition_tag);

			if (csih->Count() == 0) {
				if (!renumbered)
					ssci->Remove(highlight_seg_orig_key);
				else
					ssci->Remove(highlight_seg_key);

#if defined(DEBUG_CSIH)
				dprintf("O %p %p\n", (void*)seg_key, (void*)csih);
#endif
				delete csih;
			}

			delete csi;
		}

		// if it is an entity, may be we will have to move it back up under its parent segment
		if (!IsSegment())
		{
			HC_KEY hilight_seg_key = HC_Show_Owner_Original_Key(m_key);

			bool other_proxies;

			HC_Open_Segment_By_Key(hilight_seg_key);{
				// if there is no other style applied, then we know we can safely move
				// the entity back - as there is not other path which wants it to be highlighted
				// else we just get rid of the style segment

				if (m_entity_proxy_seg_tag != INVALID_KEY) {
					HC_Delete_By_Key(m_entity_proxy_seg_tag);
					m_entity_proxy_seg_tag = INVALID_KEY;
				}

				HC_Open_Segment(entity_proxy_tag_seg_name);{
					other_proxies = HC_Show_Existence("segment") > 0;
				}HC_Close_Segment();
			}HC_Close_Segment();

			bool moved = false;

			if (!other_proxies && HC_Show_Existence_By_Key(hilight_seg_key, "style") <= 1) {
				seg_key = HC_Show_Owner_Original_Key(hilight_seg_key);
				HC_Move_Key_By_Key(m_key, seg_key);
				moved = true;
			}

			int seg_count = HC_Show_Existence_By_Key(hilight_seg_key, "segment");
			(void)seg_count;
			assert(seg_count == 1); // just proxy tag segment

			int geom_count = HC_Show_Existence_By_Key(hilight_seg_key, "geometry");
			assert(geom_count <= 1);
			bool unregister = (moved && geom_count == 0) || (!moved && geom_count == 1);
			if (unregister) {
				m_pSelectionSet->UnRegisterHighlightSeg(hilight_seg_key);
			}
			if (!other_proxies) {
				HC_Delete_By_Key(hilight_seg_key);
			}
		}

		m_ConditionStyle = INVALID_KEY;

		return true;
	}
}


void HSmartSelItem::GetBaseTag(char * retTag)
{
	get_base_tag(m_key, m_IncludePath.count, m_IncludePath.keys, retTag);
}


bool HSmartSelItem::IsInstanced() const
{
	// if this is something directly under the model key 
	// then it isn't instanced
	if( m_IncludePath.count <= (m_pSelectionSet->GetReferenceSelection() ? 2 : 1))
		return false;

	return true;
}


bool HSmartSelItem::Delete()
{
	// unhighlight it first
	UnHighlight();
	
	// if this is old style selection or something directly 
	// under the model key - use direct delete
	if( !m_pSelectionSet->GetReferenceSelection() && m_IncludePath.count > 1) {
		m_pSelectionSet->DeInstance(this);
		// unhighlight it first
		UnHighlight();
	}

	if(HC_Show_Existence_By_Key(m_key, "self"))
	{
		HUtility::DeleteManipulators(m_key);
		HC_Delete_By_Key(m_key);
		m_key = INVALID_KEY;
	}

	return true;
}



bool HSmartSelItem::IsHighlighted() const
{
	if(m_pSelectionSet->GetReferenceSelection())
	{
		if((const_cast<HSmartSelItem*>(this))->GetHighlightedRefListCount()>0)
			return true;
		else
			return false;
	}
	else
	{
		if(m_ConditionStyle != INVALID_KEY)
			return true;
		else
			return false;
	}
}


bool HSmartSelItem::IsRegionHighlighted(const HC_KEY key, const HSelectionSet * selection_set, 
								  const int incl_count, const HC_KEY * incl_keys, int region)
{
	if(selection_set->GetReferenceSelection())
	{
		return selection_set->IsRegionSelected(key, incl_count, incl_keys, region);
	}
	else
	{
		bool retval = false;
		HC_Open_Geometry(key);{
			char region_str[MVO_BUFFER_SIZE];
			sprintf(region_str, "user options = hregion%d highlighted", region);
			if(HC_Show_Existence(region_str)){
				retval = true;
			}
		}HC_Close_Geometry();
		return retval;
	}
}

bool HSmartSelItem::IsHighlighted(const HC_KEY key, const HSelectionSet * selection_set, 
								  const int incl_count, const HC_KEY * incl_keys)

{
	return selection_set->IsSelected(key, incl_count, incl_keys);
}


bool HSmartSelItem::QueryIncludeDependency(HC_KEY include_key, HC_KEY& ret_dependent_key)
{
	bool has_dependency = false;
	for(int i = 0; i < m_IncludePath.count; i++)
	{
		if( m_IncludePath.keys[i] == include_key )
		{
			// we are indeed using this include_key - who's the dependent
			if( (i+1) < m_IncludePath.count)
				ret_dependent_key = m_IncludePath.keys[i+1];
			else
				ret_dependent_key = m_key;

			has_dependency = true;
			break;
		}
	}

	return has_dependency;
}

bool HSmartSelItem::UpdateIncludeDependency(HC_KEY include_key, HC_KEY dependent_key, HC_KEY replacement_key)
{
	bool updated=false;

	for(int i = 0; i < m_IncludePath.count; i++)
	{
		if( m_IncludePath.keys[i] == include_key )
		{
			// we are indeed using this include_key - replace the dependent
			if(dependent_key == m_key)
				m_key = replacement_key;
			else if(i+1<m_IncludePath.count && m_IncludePath.keys[i+1]==dependent_key)
			{
				// replace the include key and dependent and adjust the array
				//don't leave invalid keys in the array.
				if(replacement_key!=INVALID_KEY)
					m_IncludePath.keys[i++] = replacement_key;
				for(;(i+1) < m_IncludePath.count; i++)
					m_IncludePath.keys[i] = m_IncludePath.keys[i+1];
			}
			else 
				break;

			// adjust the count
			m_IncludePath.count--;
			updated=true;
			break;
		}
	}

	return updated;
}


/////////////////////////////////////////////////////////////////////////////
// HOldSelItem
//  A wrapper over selection item held in a list the HSelectionSet class. Typically a HOldSelItem 
//  is just a key to a selected segment or shell. In some cases it might have more information viz. sub-elements 
//  associated with it. 

HOldSelItem::HOldSelItem(HC_KEY key, HSelectionSet * selection_set, const char * selection_path, 
						 HC_KEY include_key, HC_KEY includer_key)
			  : HSelectionItem(key, selection_set)
{
	m_pSelectionPath = new char[strlen(selection_path)+1];
	strcpy(m_pSelectionPath, selection_path);
	m_IncludeKey = include_key;
	m_IncluderKey = includer_key;

}


HOldSelItem::~HOldSelItem()
{
	H_SAFE_DELETE_ARRAY(m_pSelectionPath);
}


bool HOldSelItem::Equals(const HSelectionItem* i2) const
{
	return HSelectionItem::Equals(i2);
}


bool HOldSelItem::Highlight()
{  
	char color1[MVO_BUFFER_SIZE], origcolor[MVO_BUFFER_SIZE], keytype[MVO_BUFFER_SIZE];
	char pathname[MVO_SEGMENT_PATHNAME_BUFFER], token[MVO_BUFFER_SIZE];
	HC_KEY key, seg_key, skey;
	float weight;
	bool is_segment_level;

	HPixelRGBA selectedFaceColor, selectedEdgeColor, selectedMarkerColor;
	float selectedEdgeWeight, selectedMarkerSize;
	m_pSelectionSet->GetSelectionFaceColor(selectedFaceColor);
	m_pSelectionSet->GetSelectionEdgeColor(selectedEdgeColor);
	m_pSelectionSet->GetSelectionMarkerColor(selectedMarkerColor);
	selectedEdgeWeight = m_pSelectionSet->GetSelectionEdgeWeight();
	selectedMarkerSize = m_pSelectionSet->GetSelectionMarkerSize();

	key = m_key;

	HC_Show_Key_Type(key, keytype);

	if (streq(keytype, "segment")) 
	{
		HUtilityAnnotation annotation;
		if (annotation.IsNote(key))
			key = HC_Show_Owner_Original_Key(key);
		HC_Show_Segment(key, pathname);
		HC_Parse_String(pathname, "/", -1, token);

		if (streq(token, "cutting grids"))
			seg_key = HC_Show_Owner_Original_Key(key);
		else
			seg_key = key;

		is_segment_level = true;
	}
	else if (streq(keytype, "grid"))
	{
		is_segment_level = true;
		seg_key = HC_Show_Owner_Original_Key(key);
		seg_key = HC_Show_Owner_Original_Key(seg_key);
	}
	else
	{
		is_segment_level = false;
		seg_key = HC_Show_Owner_Original_Key(key);
	}


	skey = seg_key;


	if ( is_segment_level )
	{
		bool include = (strchr (m_pSelectionPath, '<') != 0);


		if ((include) && (m_IncludeKey != INVALID_KEY) && (m_IncluderKey != INVALID_KEY))
		{
			HC_Delete_By_Key(m_IncludeKey);					// remove the original include link
			
			HC_Open_Segment_By_Key(m_IncluderKey);
			 seg_key = HC_Copy_Segment(m_pSelectionPath, "tmp");	// copy the included segment
			HC_Close_Segment();

			HC_Open_Segment_By_Key(seg_key);
			 HC_Set_User_Index(H_HIGHLIGHT_KEY_MAGIC, (void *)skey); // store the orig included segment key in the copied segment
			HC_Close_Segment();
		}
		
		HC_Open_Segment_By_Key(seg_key);

			HC_Set_User_Value(1L);

			if (HC_Show_Existence("color"))
			{
				HC_Show_Color(color1);
				sprintf(origcolor, "origcolor = (%s)", color1);
				HC_Set_User_Options(origcolor);
			}

			if (HC_Show_Existence("line weight"))
			{
				HC_Show_Line_Weight(&weight);
				HCLOCALE(sprintf(origcolor, "origlineweight = (%f)", weight));
				HC_Set_User_Options(origcolor);
			}

			if (HC_Show_Existence("edge weight"))
			{
				HC_Show_Edge_Weight(&weight);
				HCLOCALE(sprintf(origcolor, "origedgeweight = (%f)", weight));
				HC_Set_User_Options(origcolor);
			}

			if (HC_Show_Existence("marker size"))
			{
				HUtility::PushAttribute("marker size");
				HC_Show_Marker_Size(&weight);
				HCLOCALE(sprintf(origcolor, "origmarkersize = (%f)", weight));
				HC_Set_User_Options(origcolor);
			}


			HC_Set_Color_By_Value ("geometry", "rgb", (float)selectedFaceColor.r()/255.0,
				(float)selectedFaceColor.g()/255.0,  (float)selectedFaceColor.b()/255.0); 
			HC_Set_Color_By_Value ("edges,lines", "rgb", (float)selectedEdgeColor.r()/255.0,
				(float)selectedEdgeColor.g()/255.0,  (float)selectedEdgeColor.b()/255.0); 
			HC_Set_Color_By_Value ("markers", "rgb", (float)selectedMarkerColor.r()/255.0,
				(float)selectedMarkerColor.g()/255.0,  (float)selectedMarkerColor.b()/255.0); 

			if (selectedFaceColor.a() != 0 || selectedEdgeColor.a() != 0 || selectedMarkerColor.a() != 0)
			{
				char color[MVO_BUFFER_SIZE];
				HCLOCALE(sprintf(color, "geometry = (transmission=(R=%1.4f G=%1.4f B=%1.4f)), edges = (transmission=(R=%1.4f G=%1.4f B=%1.4f)), lines = (transmission=(R=%1.4f G=%1.4f B=%1.4f)), markers = (transmission=(R=%1.4f G=%1.4f B=%1.4f))", 
					(float)selectedFaceColor.a()/256.0,  (float)selectedFaceColor.a()/256.0,  (float)selectedFaceColor.a()/256.0, 
					(float)selectedEdgeColor.a()/256.0,  (float)selectedEdgeColor.a()/256.0,  (float)selectedEdgeColor.a()/256.0,
					(float)selectedEdgeColor.a()/256.0,  (float)selectedEdgeColor.a()/256.0,  (float)selectedEdgeColor.a()/256.0,
					(float)selectedMarkerColor.a()/256.0,  (float)selectedMarkerColor.a()/256.0,  (float)selectedMarkerColor.a()/256.0));
				HC_Set_Color(color);
			}

			if (selectedEdgeWeight != 1.0)
			{
				HC_Set_Edge_Weight(selectedEdgeWeight);
				HC_Set_Line_Weight(selectedEdgeWeight);
			}

			if(selectedMarkerSize != 1.0)
				HC_Set_Marker_Size(selectedMarkerSize);

			HC_Set_Rendering_Options("attribute lock = color = (on, faces = no transmission)");
			HC_Set_Rendering_Options("general displacement = -8");

		HC_Close_Segment();
	}
	else 
	{
		HC_Open_Segment_By_Key(seg_key);
		 int ready = HC_QShow_Existence ("highlight", "self");
		 HC_Open_Segment("highlight");   
		 if (!ready) {
			HC_Set_User_Index(H_HIGHLIGHT_KEY_MAGIC, (void *)seg_key);
			HC_Set_Color_By_Value ("geometry", "rgb", (float)selectedFaceColor.r()/255.0,
				(float)selectedFaceColor.g()/255.0,  (float)selectedFaceColor.b()/255.0); 
			HC_Set_Color_By_Value ("edges,lines", "rgb", (float)selectedEdgeColor.r()/255.0,
				(float)selectedEdgeColor.g()/255.0,  (float)selectedEdgeColor.b()/255.0); 
			HC_Set_Color_By_Value ("markers", "rgb", (float)selectedMarkerColor.r()/255.0,
				(float)selectedMarkerColor.g()/255.0,  (float)selectedMarkerColor.b()/255.0); 

			if (selectedFaceColor.a() != 0 || selectedEdgeColor.a() != 0 || selectedMarkerColor.a() != 0)
			{
				char color[MVO_BUFFER_SIZE];
				HCLOCALE(sprintf(color, "geometry = (transmission=(R=%1.4f G=%1.4f B=%1.4f)), edges = (transmission=(R=%1.4f G=%1.4f B=%1.4f)), lines = (transmission=(R=%1.4f G=%1.4f B=%1.4f)), markers = (transmission=(R=%1.4f G=%1.4f B=%1.4f))", 
					(float)selectedFaceColor.a()/256.0,  (float)selectedFaceColor.a()/256.0,  (float)selectedFaceColor.a()/256.0, 
					(float)selectedEdgeColor.a()/256.0,  (float)selectedEdgeColor.a()/256.0,  (float)selectedEdgeColor.a()/256.0,
					(float)selectedEdgeColor.a()/256.0,  (float)selectedEdgeColor.a()/256.0,  (float)selectedEdgeColor.a()/256.0,
					(float)selectedMarkerColor.a()/256.0,  (float)selectedMarkerColor.a()/256.0,  (float)selectedMarkerColor.a()/256.0));
				HC_Set_Color(color);
			}

			if (selectedEdgeWeight != 1.0)
			{
				HC_Set_Edge_Weight(selectedEdgeWeight);
				HC_Set_Line_Weight(selectedEdgeWeight);
			}
			if(selectedMarkerSize != 1.0)
				HC_Set_Marker_Size(selectedMarkerSize);
		 }
		 HC_Move_By_Key(key, ".");

		 HC_Close_Segment();
		HC_Close_Segment();	
	}

	return true;
}


bool HOldSelItem::UnHighlight()
{
	HoopsRemove(m_key);
	return true;
}

void HOldSelItem::DeselectSegment(HC_KEY key, char const *keytype)
{
	HC_KEY orig_key = INVALID_KEY;
	HC_KEY owner = INVALID_KEY;
	HC_KEY seg_key = INVALID_KEY;
	HUserOptions orig;
	char origdest[MVO_BUFFER_SIZE];
	char pathname[MVO_SEGMENT_PATHNAME_BUFFER];
	char token[MVO_BUFFER_SIZE];

	if (streq(keytype, "segment")) 
	{
		HC_Show_Segment(key, pathname);
		HC_Parse_String(pathname, "/", -1, token);

		if (streq(token, "cutting grids"))
			seg_key = HC_Show_Owner_Original_Key(key);
		else
			seg_key = key;
	}
	else if (streq(keytype, "grid"))
	{
		seg_key = HC_Show_Owner_Original_Key(key);
		seg_key = HC_Show_Owner_Original_Key(seg_key);
	}
	assert( seg_key != INVALID_KEY );
	if( seg_key == INVALID_KEY )
		//this should never happen, because keytype != [segment,grid] should have 
		//already been screened out by the calling function
		return; 

	HC_Open_Segment_By_Key(seg_key);
		if (!HC_Show_One_Net_User_Index(H_HIGHLIGHT_KEY_MAGIC, &orig_key))
			orig_key = INVALID_KEY;
	HC_Close_Segment();

	if (orig_key != INVALID_KEY)			// segment was previously included
	{			
		owner = HC_Show_Owner_Original_Key(seg_key);

		HC_Open_Segment_By_Key(seg_key);
		float matrix[16];
			if (HC_Show_Existence("modelling matrix"))
			{		    
			HC_Show_Modelling_Matrix(matrix);
			HC_Open_Segment_By_Key(orig_key);
			HC_Set_Modelling_Matrix(matrix);
			HC_Close_Segment();
			}
		HC_Close_Segment();
		HC_Open_Segment_By_Key(owner);
			HC_Include_Segment_By_Key(orig_key);
		HC_Close_Segment();
		HC_Delete_By_Key(seg_key);
	}
	else
	{
		HC_Open_Segment_By_Key(seg_key);{
			
			if(!streq(pathname, "/")){
				HC_UnSet_Color();
				HC_UnSet_Line_Weight();
				HC_UnSet_Edge_Weight();
				HC_UnSet_Marker_Size();
			}
			
			if (HC_Show_Existence("user options = origcolor")){
				
				orig.Show_One_Net_User_Option("origcolor");
				if (strcmp(orig.m_data,"")!=0){
					strncpy(origdest, orig.m_data + 1, strlen(orig.m_data) - 2);	// strip off parens
					strncpy(origdest + (strlen(orig.m_data) - 2), "\0", 2);		// null terminate the string
					HC_Set_Color(origdest);
					HC_UnSet_One_User_Option("origcolor");
				}
			}
			
			if (HC_Show_Existence("user options = origlineweight")){

				orig.Show_One_Net_User_Option("origlineweight");
				if (strcmp(orig.m_data,"")!=0){
					float oldweight;
					strncpy(origdest, orig.m_data + 1, strlen(orig.m_data) - 2);	// strip off parens
					strncpy(origdest + (strlen(orig.m_data) - 2), "\0", 2);		// null terminate the string
					HCLOCALE(oldweight = (float) atof(origdest));
					HC_Set_Line_Weight(oldweight);
					HC_UnSet_One_User_Option("origlineweight");
				}
			}
			
			
			if (HC_Show_Existence("user options = origedgeweight")){

				orig.Show_One_Net_User_Option("origedgeweight");
				if (strcmp(orig.m_data,"")!=0){
					float oldweight;
					strncpy(origdest, orig.m_data + 1, strlen(orig.m_data) - 2);	// strip off parens
					strncpy(origdest + (strlen(orig.m_data) - 2), "\0", 2);		// null terminate the string
					HCLOCALE(oldweight = (float) atof(origdest));
					HC_Set_Edge_Weight(oldweight);
					HC_UnSet_One_User_Option("origedgeweight");
				}
			}
			
			if (HC_Show_Existence("user options = origmarkersize")){

				orig.Show_One_Net_User_Option("origmarkersize");
				if (strcmp(orig.m_data,"")!=0){
					float oldweight;
					strncpy(origdest, orig.m_data + 1, strlen(orig.m_data) - 2);	// strip off parens
					strncpy(origdest + (strlen(orig.m_data) - 2), "\0", 2);		// null terminate the string
					HCLOCALE(oldweight = (float) atof(origdest));
					HC_Set_Marker_Size(oldweight);
					HC_UnSet_One_User_Option("origmarkersize");
				}
			}
			
			if(HC_Show_Existence("rendering options = general displacement"))
				HC_UnSet_One_Rendering_Option("general displacement");
			
			if(streq(pathname, "/"))
				HC_Set_Rendering_Options("no attribute lock");
			else 
				HC_UnSet_One_Rendering_Option("attribute lock");

			HC_Set_User_Value(0L);

		}HC_Close_Segment();		
	}
}


void HOldSelItem::DeselectGeometry(HC_KEY key)
{
	HC_KEY orig_key, seg_key;
	float matrix[16];
	bool has_matrix = false;
	
	seg_key = HC_Show_Owner_Original_Key(key);
	HC_Open_Segment_By_Key(seg_key);
		HC_Show_One_Net_User_Index(H_HIGHLIGHT_KEY_MAGIC, &orig_key);
		if (HC_Show_Existence ("modelling matrix")) 
		{
			HC_Show_Modelling_Matrix (matrix);
			HC_UnSet_Modelling_Matrix ();
			has_matrix = true;
		}
	HC_Close_Segment();
	HC_Open_Segment_By_Key(orig_key);
		if( has_matrix && HC_Show_Existence ("geometry, subsegments, first match") ) {
			HC_Open_Segment( "" );
				HC_Set_Modelling_Matrix( matrix );
				HC_Move_By_Key(key, ".");
			HC_Close_Segment();
		}
		else {
			HC_Move_By_Key(key, ".");
		}
	HC_Close_Segment();
}


/* deselect an item
 - if a piece of geometry, move it back to it's original segment
 - if a segment, restore the original rendering attributes (color, etc...)
*/
void HOldSelItem::HoopsRemove(HC_KEY key)
{
	char key_status[MVO_BUFFER_SIZE];
	char keytype[MVO_BUFFER_SIZE];

	//if the object has already been deleted, there's nothing to do
	HC_Show_Key_Status (key, key_status);
	if( streq( key_status, "invalid" ) )
		return;

	HC_Show_Key_Type(key, keytype);
	if( streq( keytype, "segment" ) ||
		streq( keytype, "grid" ) ) {
		DeselectSegment( key, keytype );
	}
	else {
		DeselectGeometry( key );
	}
	 
}


bool HOldSelItem::Delete()
{
	// unhighlight it first
	UnHighlight();
	
	HC_Delete_By_Key(m_key);
	m_key = INVALID_KEY;
	return true;
}



bool HOldSelItem::IsHighlighted() const
{
	HC_KEY orig_key = INVALID_KEY;
	char keytype[MVO_BUFFER_SIZE];
	MVO_POINTER_SIZED_INT hlite = 0;
	
	HC_Show_Key_Type(m_key, keytype);
	
	if (streq(keytype, "segment"))
	{
		HC_Open_Segment_By_Key(m_key);
	}
	else {
		HC_KEY owner_key = HC_Show_Owner_Original_Key(m_key);
		
		if (owner_key < 0)
			HC_Open_Segment_By_Key(owner_key);
		else {
			char segment[MVO_SEGMENT_PATHNAME_BUFFER];
			HC_Show_Owner_By_Key(m_key, segment);
			HC_Open_Segment(segment);
		}
	}
	
		if (HC_Show_Existence("user value"))
			HC_Show_User_Value(&hlite);

		if (!HC_Show_One_Net_User_Index(H_HIGHLIGHT_KEY_MAGIC, &orig_key))
			orig_key = INVALID_KEY;
	HC_Close_Segment();
	
	
	if((hlite == 1) || (orig_key != INVALID_KEY))
		return true;
	else
		return false;
	
}

bool HOldSelItem::QueryIncludeDependency(HC_KEY include_key, HC_KEY& ret_dependent_key)
{
	UNREFERENCED(include_key);
	UNREFERENCED(ret_dependent_key);

	// I don't handle includes - isn't that why I am called Old?
	return false;		// false is not failure, but says do we have dependency
}


bool HOldSelItem::UpdateIncludeDependency(HC_KEY include_key, HC_KEY dependent_key, HC_KEY replacement_key)
{
	// don't care about instancing

	UNREFERENCED(include_key);
	UNREFERENCED(dependent_key);
	UNREFERENCED(replacement_key);

	return true;
}
