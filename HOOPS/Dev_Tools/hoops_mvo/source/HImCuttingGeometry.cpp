// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// HCuttingPlaneLinedata.cpp : implementation of the HCuttingPlaneLinedata class
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "HDB.h"
#include "HImCuttingGeometry.h"
#include "HImManager.h"
#include "HUtility.h"
#include "vlist.h"

#include "hc.h"
#include "hic.h"


int HImCuttingGeometry::m_cutting_type = -1;
float HImCuttingGeometry::m_smallest_feature = 0.0001f;
bool HImCuttingGeometry::m_body_is_open = false;
float HImCuttingGeometry::m_tolerance = 0.1f; /* as a proportion of camera width and height */
struct vlist_s * HImCuttingGeometry::m_lines = 0;
struct vlist_s * HImCuttingGeometry::m_polylines = 0;
struct vlist_s * HImCuttingGeometry::m_faces = 0;
HC_KEY HImCuttingGeometry::m_target_segment=0;
bool HImCuttingGeometry::m_honor_matrices=false;

typedef struct line_s {
	HIC_DC_Point pts[2];
} line_t;

typedef struct polyline_s {
	HIC_DC_Point *pts;
	int n_pts;
} polyline_t;

typedef struct face_s {
	int point_count;
	HIC_DC_Point *points;
	int face_list_length;	
	int *face_list;
	float r, g, b;
} face_t;


HImCuttingGeometry::HImCuttingGeometry()
{
}


HImCuttingGeometry::~HImCuttingGeometry()
{
	if(m_lines)
		delete_vlist(m_lines);
	m_lines = 0;
	if(m_polylines)
		delete_vlist(m_polylines);
	m_polylines = 0;
	if(m_faces)
		delete_vlist(m_faces);
	m_faces = 0;
}


void HImCuttingGeometry::RegisterCallbacks()
{
	HImRegisterCallback("HImCuttingGeometry_StoreCuttingPlaneLines", StoreCuttingPlaneLines);
	HImRegisterCallback("HImCuttingGeometry_ShellBody", ShellBody);
	HImRegisterCallback("HImCuttingGeometry_SegmentBody", SegmentBody);
	HImRegisterCallback("HImCuttingGeometry_GetCaps", GetCaps);

}


void HImCuttingGeometry::StoreCuttingPlaneLines (HIC_Rendition const *nr, int count, HIC_DC_Point const * pts)
{
	line_t * line;
	float adjoint[16];
	int i;

	HC_Compute_Matrix_Adjoint ((float *)HIC_Show_Transform_Matrix (nr), adjoint);
	for( i = 0 ; i < count - 1 ; i++ ) {
		line = (line_t *) malloc ( sizeof(line_t) );
		HC_Compute_Transformed_Points (2, pts, adjoint, line->pts);
		HC_Compute_Transformed_Points (2, line->pts, (float *)HIC_Show_Modelling_Matrix (nr), line->pts);
		vlist_add_first(m_lines, (void*) line);
		pts++;
	}
}



static float xyzdiff( HIC_DC_Point *p1, HIC_DC_Point *p2 ) 
{
	return (p1->x - p2->x) * (p1->x - p2->x)
		 + (p1->y - p2->y) * (p1->y - p2->y)
		 + (p1->z - p2->z) * (p1->z - p2->z);
}


static void reverse_line( line_t *line )
{
	HIC_Point temp;

	memcpy( &temp, &line->pts[0], sizeof( HIC_Point ) );
	memcpy( &line->pts[0], &line->pts[1], sizeof( HIC_Point ) );
	memcpy( &line->pts[1], &temp, sizeof( HIC_Point ) );
}


#define MATCHMODE_INVALID 0
#define START_START 1
#define END_END	    2
#define END_START   3
#define START_END   4
static bool find_mate (struct vlist_s *line_list, struct vlist_s *bucket_list, float tolerance )
{
	struct vlist_s *bucket;
	line_t *polyline_start, *polyline_end, *bucket_start, *bucket_end, *line;
	float min_diff = tolerance;
	float diff;
	int best_fit = -1;
	int num_buckets, i;
	int match_mode = MATCHMODE_INVALID;

	polyline_start = (line_t *)vlist_peek_first( line_list );
	polyline_end = (line_t *)vlist_peek_last( line_list );
	num_buckets = vlist_count( bucket_list );
	vlist_reset_cursor( bucket_list ); 
	for( i = 0 ; i < num_buckets ; i++ ) {
		/* get a bucket */
		bucket = (struct vlist_s *)vlist_peek_cursor( bucket_list );

		bucket_start = (line_t *)vlist_peek_first( bucket );
		bucket_end = (line_t *)vlist_peek_last( bucket );
		diff = (float)fabs( xyzdiff ( &polyline_start->pts[0], &bucket_start->pts[0] ) );	/* start start */
		if( diff < min_diff ) {
			best_fit = i;
			min_diff = diff;
			match_mode = START_START;
		}
		diff = (float)fabs( xyzdiff ( &polyline_end->pts[1],   &bucket_end->pts[1] ) ); 	/* end end */
		if( diff < min_diff ) {
			best_fit = i;
			min_diff = diff;
			match_mode = END_END;
		}
		diff = (float)fabs( xyzdiff ( &polyline_end->pts[1],   &bucket_start->pts[0] ) );	/* end start */
		if( diff < min_diff ) {
			best_fit = i;
			min_diff = diff;
			match_mode = END_START;
		}
		diff = (float)fabs( xyzdiff ( &polyline_start->pts[0], &bucket_end->pts[1] ) ); 	/* start end */
		if( diff < min_diff ) {
			best_fit = i;
			min_diff = diff;
			match_mode = START_END;
		}
		/* advance to the next bucket */
		vlist_advance_cursor( bucket_list );
	}
	/* bail early if nothing fit */
	if( best_fit == -1 )
		return false;

	/* if there are more than just a few line segments in the line list, see if 
	 * it closes itself to a higher precision than the match we found.	*/
	if( line_list->count > 3 ) {
		if( xyzdiff( &polyline_start->pts[0], &polyline_end->pts[1] ) < min_diff) 
			return false;
	}

	/* re-fetch the best fit from the bucket list */
	bucket = (struct vlist_s *) vlist_nth_item( bucket_list, best_fit );
	/* if there are more than just a few line segments in the bucket, see if 
	 * it closes itself to a higher precision than the match we found.	*/
	if( bucket->count > 3 ) {
		bucket_start = (line_t *)vlist_peek_first( bucket );
		bucket_end = (line_t *)vlist_peek_last( bucket );
		if( xyzdiff( &bucket_start->pts[0], &bucket_end->pts[1]) < min_diff )
			return false; /* the bucket was already closed */
	}

		
	switch( match_mode ) {
		case START_START:
			while((line = (line_t *)vlist_remove_first(line_list)) != 0) {
				reverse_line( line );
				vlist_add_first( bucket, line );
			}
		break;
		case END_END:
			vlist_reverse( line_list );
			while((line = (line_t *)vlist_remove_first(line_list)) != 0) {
				reverse_line( line );
				vlist_add_last( bucket, line );
			}
		break;
		case END_START:
			vlist_reverse( line_list );
			while((line = (line_t *)vlist_remove_first(line_list)) != 0) {
				vlist_add_first( bucket, line );
			}
		break;
		case START_END:
			while((line = (line_t *)vlist_remove_first(line_list)) != 0) {
				vlist_add_last( bucket, line );
			}
		break;
		default:
			/* this should never happen -- if nothing had fit, we should have
			 * already bailed */
			return false;
	}
	delete_vlist( line_list );
	line_list = 0;
	return true;
}


polyline_t *clone_polyline( const polyline_t *in )
{
	polyline_t *out = (polyline_t *) malloc ( sizeof(polyline_t) );
	int bufsize = in->n_pts * sizeof(HIC_DC_Point);

	out->n_pts = in->n_pts;
	out->pts = (HIC_DC_Point *) malloc ( bufsize  );
	memcpy( out->pts, in->pts, bufsize );
	return out;
}


void HImCuttingGeometry::StopBody (HIC_Rendition const *nr)
{		
	struct vlist_s *bucket_list, *bucket_list2, *bucket, *line_list, *intermediate_polylines;
	line_t *line;
	polyline_t *polyline;
	face_t *face;
	int i, count, next_point, point_count, face_list_length;
	int *face_out;
	HIC_DC_Point *point_out;
	bool is_first_face;
	float tolerance;
	HIC_RGB const *face_color;

	if( m_cutting_type == HIM_CUTTING_GEOMETRY_TYPE_LINES )
		return;
	tolerance = MAX( HIC_Show_Camera_Field_Width(nr), HIC_Show_Camera_Field_Height(nr) ) * m_tolerance;

	/* allocate some temporary lists.  These are all cleaned up at the end of the function, so beware of
	 * any early escapes that bypass that cleanup. */
	bucket_list = new_vlist( malloc, free );
	bucket_list2 = new_vlist( malloc, free );
	intermediate_polylines = new_vlist( malloc, free );

	/* cook from line to polyline */
	while((line = (line_t *)vlist_remove_first(m_lines)) != 0) {
		if( xyzdiff( &line->pts[0], &line->pts[1] ) < 1e-10 ) {
			free( line );
		}
		else {
			bucket = new_vlist( malloc, free );
			vlist_add_first( bucket, line );
			vlist_add_last( bucket_list, bucket );
		}
	}
	while((line_list = (struct vlist_s *)vlist_remove_first(bucket_list)) != 0) {
		if( find_mate( line_list, bucket_list, tolerance ) )
			continue;
		vlist_add_first( bucket_list2, line_list );
	}

	while((line_list = (struct vlist_s *)vlist_remove_first(bucket_list2)) != 0) {
		/* we've popped the first bucket off the front of the list.  Before we start
		 * processing it, however, see if it can be merged with any of the others. */
		if( find_mate( line_list, bucket_list2, tolerance ) )
			continue;
		/* collate from a linked list representation into an array.  We go from a vlist of "line_s"'s
		 * to a single "polyline_s" */
		if( vlist_count( line_list ) != 0 ) {
		    polyline = (polyline_t *) malloc ( sizeof(polyline_t) );
		    polyline->n_pts = 1 + vlist_count( line_list );
		    polyline->pts = (HIC_DC_Point *) malloc ( polyline->n_pts * sizeof(HIC_DC_Point) );
		    count = vlist_count( line_list );
		    for( i = 0 ; i < count ; i++ ) {
			    line = (line_t *) vlist_remove_first( line_list );
			    memcpy( &polyline->pts[i], &line->pts[0], sizeof(HIC_DC_Point) );
			    if( i == count - 1 )
				    memcpy( &polyline->pts[i+1], &line->pts[1], sizeof(HIC_DC_Point) );
			    free( line );
		    }
		    vlist_add_first( intermediate_polylines, polyline );
		}
		delete_vlist( line_list );
		line_list = 0;
	}

	if( m_cutting_type == HIM_CUTTING_GEOMETRY_TYPE_POLYLINES ) {
		/* only interested in polylines -- move to the global m_polylines list */
		while( (polyline = (polyline_t *) vlist_remove_first( intermediate_polylines )) != 0 ) {
			vlist_add_first( m_polylines, polyline );
		}
		goto cleanup;
	}
	else if( m_cutting_type & HIM_CUTTING_GEOMETRY_TYPE_POLYLINES ) {
		/* polylines as well as faces -- copy to global m_polylines list */
		vlist_reset_cursor( intermediate_polylines );
		while( (polyline = (polyline_t *) vlist_peek_cursor( intermediate_polylines )) != 0 ) {
			vlist_add_first( m_polylines, clone_polyline( polyline ) );
			vlist_advance_cursor( intermediate_polylines );
		}
	}
	
	/* cook from polyline to face */
	point_count = 0;
	face_list_length = 0;
	next_point = 0;
	vlist_reset_cursor( intermediate_polylines );
	  /* compute an upper bound on point count and face list length */
	count = vlist_count( intermediate_polylines );
	for( i = 0 ; i < count ; i++ ) {
		polyline = (polyline_t *)vlist_peek_cursor(intermediate_polylines);
		point_count += polyline->n_pts;
		face_list_length += polyline->n_pts + 1;
		vlist_advance_cursor( intermediate_polylines );
	}
	if( point_count == 0 )
		goto cleanup;
	face = (face_t *) malloc( sizeof( face_t ) );
	face->points = (HIC_DC_Point *) malloc( point_count * sizeof( HIC_DC_Point ) );
	point_out = face->points;
	face->face_list = (int *) malloc( face_list_length * sizeof( int ) );
	face_out = face->face_list;
	is_first_face = true;
	while((polyline = (polyline_t *)vlist_remove_first( intermediate_polylines )) != 0) {
		/* if the polyline is more than a single line and is a closed region (first and last points 
		 * are equal), we can add it to the face list */
		if( polyline->n_pts > 3 && 
		    xyzdiff( &polyline->pts[0], &polyline->pts[polyline->n_pts - 1]) < tolerance ) {
			if( is_first_face )
				*face_out = polyline->n_pts - 1;
			else 
				/* pretend all faces after the first are holes.  Even if they're disjoint, the
				 * HOOPS triangulator will know what to do */
				*face_out = -(polyline->n_pts - 1);
			face_out++;
			for( i = 0 ; i < polyline->n_pts - 1 ; i++ ) {
				*face_out++ = next_point++;
				memcpy( point_out, &polyline->pts[ i ], sizeof( HIC_DC_Point ) );
				point_out++;
			}
			is_first_face = false;
		}
		free( polyline->pts );
		free( polyline );
	}
	face->point_count = static_cast<int>(point_out - face->points);
	face->face_list_length = static_cast<int>(face_out - face->face_list);
	face_color = HIC_Show_Face_Material_Diffuse_Color(nr);


	face->r = face_color->red;
	face->g = face_color->green;
	face->b = face_color->blue;
	if( face->point_count != 0 ) {
		/* keep it if it's good */
		vlist_add_first( m_faces, face );		
	}
	else {
		/* toss it if it's bad (e.g. all faces degenerate) */
		free( face->points );
		free( face->face_list );
		free( face );	
	}				

    cleanup:
	delete_vlist( bucket_list );
	bucket_list = 0;
	delete_vlist( bucket_list2 );
	bucket_list2 = 0;
	delete_vlist( intermediate_polylines );
	intermediate_polylines = 0;

} // end function HImCuttingGeometry::StopBody

	
void HImCuttingGeometry::ShellBody (HIC_Rendition const *nr, HIC_Polyhedron const *phon)
{
	HIC_Draw_3D_Polyhedron( nr, phon );
	HImCuttingGeometry::StopBody(nr);
}

void HImCuttingGeometry::SegmentBody (HIC_Rendition const *nr, HIC_Segment_Info const *seg)
{
	MVO_POINTER_SIZED_INT value = 0;

	value = (MVO_POINTER_SIZED_INT) HIC_Show_User_Index( nr, H_SOLID_BODY_MAGIC );
	if( !m_body_is_open && value ) {
		m_body_is_open = true;
		HIC_Draw_Segment( nr, seg );
		m_body_is_open = false;
	}
	else
		HIC_Draw_Segment( nr, seg );
	if( !m_body_is_open )
		HImCuttingGeometry::StopBody(nr);
}


static void fit_source_key( HC_KEY source_key )
{
	HIC_Point min, max, center, camera_pos;
	float up[3] = { 0, 1, 0 };
	float longest_dimension;

	if( !HC_Show_Bounding_Cuboid_By_Key( source_key, &min, &max ) ) {
		if( !HC_Compute_Circumcuboid_By_Key( source_key, &min, &max ) )
			return;
	}
	center.x = (min.x + max.x) / 2;
	center.y = (min.y + max.y) / 2;
	center.z = (min.z + max.z) / 2;
	longest_dimension = max.x - min.x;
	if( longest_dimension < max.y - min.y ) 
		longest_dimension = max.y - min.y;
	if( longest_dimension < max.z - min.z ) 
		longest_dimension = max.z - min.z;
	camera_pos.x = center.x;
	camera_pos.y = center.y;
	camera_pos.z = center.z - (2 * longest_dimension);
	HC_Set_Camera( &camera_pos, &center, up, longest_dimension, longest_dimension, "stretched" );
	HC_Set_Rendering_Options("attribute lock = (camera)");
}


void HImCuttingGeometry::CreateCuttingGeometry(HC_KEY source_key, HC_KEY target_key, int cutting_type, int body_level)
{
	char buf[MVO_BUFFER_SIZE], sysop[MVO_BUFFER_SIZE];
	int image_width, image_height;
	line_t *line;
	polyline_t *polyline;
	face_t *face;
	HC_KEY key;

	m_cutting_type = cutting_type;
	//make the image just big enough that HOOPS doesn't think it can get away with doing nothing
	image_width = 32;
	image_height = 32;

	H_FORMAT_TEXT tmpseg("/driver/null/%d",HDB::GetUniqueID());
	HC_Open_Segment(tmpseg);
		HC_KEY image_key = HC_KInsert_Image(0.0, 0.0, 0.0, "rgb", image_width,	image_height, 0);
	HC_Close_Segment();
	m_lines = new_vlist( malloc, free );
	m_polylines = new_vlist( malloc, free );
	m_faces = new_vlist( malloc, free );
	H_FORMAT_TEXT dvrseg("/driver/image/%s+%d", HDB::GetInstanceBase(), HDB::GetUniqueID());
	HC_Open_Segment(dvrseg);
		sprintf(buf, "use window id = (image key = %s%p)", H_EXTRA_POINTER_FORMAT, (void*)image_key);
		HC_Set_Driver_Options(buf);   
		HImSetCallback ("draw dc cut line","HImCuttingGeometry_StoreCuttingPlaneLines");
		if( body_level == HIM_CUTTING_GEOMETRY_SEGMENT_BODY )
			HImSetCallback ("draw segment", "HImCuttingGeometry_SegmentBody")
		else
			HImSetCallback ("draw 3d polyhedron", "HImCuttingGeometry_ShellBody")

		HC_Set_Heuristics ("no hidden surfaces, quick moves = off");
		HC_Show_One_System_Option( "bounding volumes", sysop );
		HC_Define_System_Options( "bounding volumes");
		HC_Set_Visibility("lines = off, lights = off" );
		HC_Set_Visibility("cut edges = on, faces = on");
		HC_Set_Rendering_Options("attribute lock = (visibility = (lines))");
		HC_Set_Rendering_Options("attribute lock = (visibility = (cut geometry))");
		HC_Set_Rendering_Options("attribute lock = (heuristics)");
		// include source segment into the image driver segment
		HC_Include_Segment_By_Key(source_key);
		fit_source_key( source_key );
		HC_Set_Driver_Options("isolated");	 
	HC_Close_Segment();
	// call the update that will trigger our callback so that we can collect our cut lines

	HC_Update_One_Display(dvrseg);
	// shut down the image driver 
	HC_Delete_Segment(dvrseg); 
	HC_Delete_Segment(tmpseg); 
	//restore the state of the bounding volume sys op
	sprintf( buf, "bounding volumes = %s", sysop );
	HC_Define_System_Options( buf );

	HC_Open_Segment_By_Key(target_key);
		HC_Set_Visibility( "edges = off" );
		while((line = (line_t *)vlist_remove_first(m_lines)) != 0) {
			HC_Insert_Polyline(2, line->pts);
			free( line );
		}
		while((polyline = (polyline_t *)vlist_remove_first(m_polylines)) != 0) {
			HC_Insert_Polyline(polyline->n_pts, polyline->pts);
			free(polyline->pts);
			free(polyline);
		}
		while((face = (face_t *)vlist_remove_first(m_faces)) != 0) {
			key = HC_KInsert_Shell(face->point_count, face->points, face->face_list_length, face->face_list);
			HC_Open_Geometry( key );
				HC_Set_Color_By_Value( "face", "rgb", face->r, face->g, face->b );
			HC_Close_Geometry();
			free(face->points);
			free(face->face_list);
			free(face);
		}
	HC_Close_Segment();
	delete_vlist( m_lines );
	delete_vlist( m_polylines );
	delete_vlist( m_faces );
	m_lines = m_polylines = m_faces = 0;

} // HImCuttingGeometry::CreateCuttingGeometry


//new version of CreateCuttingGeometry that uses more modern techniques
void HImCuttingGeometry::CreateCuttingGeometry(HC_KEY source_key, HC_KEY target_key, bool honor_modelling_matrices)
{
	m_target_segment=target_key;
	m_honor_matrices=honor_modelling_matrices;

	HC_Open_Segment_By_Key(source_key);
		HImSetCallback ("draw 3d polyhedron","HImCuttingGeometry_GetCaps");
//		HC_Set_Callback ("draw 3d polyhedron = get_cap");

		//do some sort of update here
		//probably shouldn't trigger a full update 
		HC_Control_Update(".", "redraw everything");
		HC_Update_Display();

//		HC_UnSet_One_Callback("get_cap");
		HImUnSetOneCallback("draw 3d polyhedron");
	HC_Close_Segment();
}


void HImCuttingGeometry::GetCaps(HIC_Rendition const *r, HIC_Polyhedron const *p)
{
	HC_KEY		key = HIC_Show_Geometry_Key(r);  

	if (HIC_Get_Geometry_Owner(p) == 0 )				// capping geometry (or maybe level of detail)
	{
			int			point_count, face_list_length;
			HIC_Point	*points;
			int			*face_list;

			HC_Show_Shell_Size (key, &point_count, &face_list_length);
			points = new HIC_Point[point_count];
			face_list = new int[face_list_length];
			HC_Show_Shell (key, &point_count, points, &face_list_length, face_list);
		
			if(m_honor_matrices)
			{
				const float	**	mm= (const float **)HIC_Show_Modelling_Matrix(r);
				HC_Compute_Transformed_Points(point_count, points, (float*)mm, points);
			}

			//output the shell geometry to the target_key from CreateCuttingGeometry, above
			HC_Open_Segment_By_Key(m_target_segment);
				HC_Insert_Shell(point_count, points, face_list_length, face_list);
			HC_Close_Segment();

			// done with the shell data
			delete [] points;
			delete [] face_list;
	}

}










