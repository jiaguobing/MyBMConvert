// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#include <math.h>
#include "HSnapGrid.h"
#include "HBaseView.h"


#define	GRID_SEGMENT	"grid geometry"

HSnapGrid::HSnapGrid(HBaseView* pHView)
{
    m_pHView = pHView;
    m_GridSegmentKey = INVALID_KEY;
    m_GridGeomKey = INVALID_KEY;
}


HSnapGrid::~HSnapGrid()
{
    if( m_GridSegmentKey != INVALID_KEY )
    	HC_Delete_By_Key(m_GridSegmentKey);
}


bool HSnapGrid::Create( const HPoint& origin, const HPoint& ref1, const HPoint& ref2, int count1, int count2)
{
    HC_Open_Segment_By_Key( m_pHView->GetSceneKey() );
	    m_GridSegmentKey = HC_KOpen_Segment(GRID_SEGMENT);
	    HC_Close_Segment();
    HC_Close_Segment();

    bool b_ret = insert_grid_geometry( origin, ref1, ref2, count1, count2 );
    set_default_grid_attributes();

    return b_ret;
}


void HSnapGrid::SetOnTop( bool true_false )
{
    HC_Open_Segment_By_Key( m_GridSegmentKey );
	if( true_false )
		HC_Set_Rendering_Options("depth range = (0, 0)");
	else
		HC_UnSet_One_Rendering_Option("depth range");
    HC_Close_Segment();
}


bool HSnapGrid::GetOnTop()
{
    HC_Open_Segment_By_Key( m_GridSegmentKey );
	int on_top = HC_Show_Existence("rendering options=(depth range)");
    HC_Close_Segment();

	return on_top != 0;
}



static void project_point_on_plane(HC_KEY handlekey, HPoint &C, float a, float b, float c, float d)
{
	UNREFERENCED(handlekey);

    float D;
    HPoint vec(a,b,c);
    HC_Compute_Normalized_Vector(&vec, &vec);
    HPoint q(vec.x * (-d), vec.y * (-d), vec.z * (-d));
    HPoint pmq(C.x - q.x, C.y - q.y, C.z - q.z);
    D = (float)HC_Compute_Dot_Product(&vec, &pmq);	
    C.Set(C.x - D * a, C.y - D * b, C.z - D * c);    	
}


static void adjust_position_to_plane(HC_KEY handlekey, HPoint &poi, float A, float B, float C, float D, HC_KEY camerasegment)
{
    HPoint s, d, poi2;
    float a,b, M;

    //we convert the 3d position of the point into viewpoint space, readjust z
    //so that the point lies on the viewplane and convert the point //back to
    //worldspace. this will give us the correct intersection vector
    char projection[MVO_BUFFER_SIZE];
    HC_Open_Segment_By_Key(camerasegment);
	HC_Show_Camera_Projection(projection);
	if (projection[0] != 'p')
	{
		HC_Compute_Coordinates(".","world",&poi,"viewpoint", &poi2);
		poi2.z = 0;
		HC_Compute_Coordinates(".","viewpoint",&poi2,"world", &s);
	}
	else
		HC_Show_Net_Camera_Position(&s.x, &s.y , &s.z);
    HC_Close_Segment();

    //next the delta between the original point and it viewplane projection is
    //calculated which describes the ray which is then used
    //to calculate the intersection with the plane
    HC_Open_Segment_By_Key(handlekey);
  	d.Set(poi.x - s.x, poi.y - s.y, poi.z - s.z);
	HC_Compute_Normalized_Vector(&d,&d);

	a = A * d.x + B * d.y + C * d.z;
	b = A * s.x + B * s.y + C * s.z + D;
	M = -b/a;
	HPoint res;
	res.Set(s.x + d.x * M,s.y + d.y * M, s.z + d.z * M);

	//after some voodoo math the result should be the point on the plane that
	//intersects with the previously calculated ray.
	//now the only thing that can go wrong is the case in which the ray does not
	//actually intersect the plane. In that case we calculate the
	//closest point between the ray and the plane with the function
	HPoint temp;
 	HC_Open_Segment_By_Key(camerasegment);
	    HC_Compute_Coordinates(".","world",&res,"viewpoint", &temp);
	HC_Close_Segment();

   	if (a==0 || b==0 || temp.z < 0)
	    project_point_on_plane(handlekey,poi, A,  B, C, D);
	else
	    poi = res;
    HC_Close_Segment();
}

bool HSnapGrid::snap_to_grid( HPoint& in_out, const HPoint& origin, const HPoint& ref1, const HPoint& ref2 )
{
    // create a grid coordinate system
	float x_intl, y_intl;
    HVector v1( ref1.x - origin.x, ref1.y - origin.y, ref1.z - origin.z );
	x_intl = (float) HC_Compute_Vector_Length(&v1);
    HC_Compute_Normalized_Vector (&v1, &v1);

    HVector v2( ref2.x - origin.x, ref2.y - origin.y, ref2.z - origin.z );
	y_intl = (float) HC_Compute_Vector_Length(&v2);
    HC_Compute_Normalized_Vector (&v2, &v2);
    HVector v3;
    HC_Compute_Cross_Product (&v1, &v2, &v3);

    float grid_mat[16], grid_mat_adj[16];
    grid_mat[0] = v1.x; grid_mat[1] = v1.y; grid_mat[2] = v1.z; grid_mat[3] = 0;
    grid_mat[4] = v2.x; grid_mat[5] = v2.y; grid_mat[6] = v2.z; grid_mat[7] = 0;
    grid_mat[8] = v3.x; grid_mat[9] = v3.y; grid_mat[10] = v3.z; grid_mat[11] = 0;
    grid_mat[12] = origin.x; grid_mat[13] = origin.y; grid_mat[14] = origin.z; grid_mat[15] = 1;

    HC_Compute_Matrix_Adjoint (grid_mat, grid_mat_adj);

    // get our point in grid coordinate system
    HC_Compute_Transformed_Points (1, &in_out, grid_mat_adj, &in_out);

    // snap the point
	float n_x_intls = in_out.x/x_intl;
	float n_y_intls = in_out.y/y_intl;
	n_x_intls = (float)floor(n_x_intls + 0.5f);
	n_y_intls = (float)floor(n_y_intls + 0.5f);
	float dx = (float)(n_x_intls*x_intl);
	float dy = (float)(n_y_intls*y_intl);

    // get the world point back
    in_out.x = origin.x + dx*(v1.x) + dy*(v2.x);
    in_out.y = origin.y + dx*(v1.y) + dy*(v2.y);
    in_out.z = origin.z + dx*(v1.z) + dy*(v2.z);

    return true;
}


bool HSnapGrid::GetSnappedWorldPos( HPoint& in_out )
{
    // project the point onto the grid plane
    HPlane grid_plane;
    HPoint origin, pr1, pr2;
    get_grid_parameters( grid_plane, origin, pr1, pr2 );

    adjust_position_to_plane( m_GridSegmentKey, in_out, grid_plane.a(), grid_plane.b(), grid_plane.c(), grid_plane.d(), m_pHView->GetSceneKey());

    // snap to grid
    snap_to_grid( in_out, origin, pr1, pr2 );

    return true;
}

bool HSnapGrid::GetSnappedViewpointPos( HPoint& in_out )
{
	// get the world point
    HC_Open_Segment_By_Key(m_pHView->GetSceneKey());
		HC_Compute_Coordinates(".","viewpoint", &in_out, "world", &in_out);
    HC_Close_Segment();

    bool b_ret = GetSnappedWorldPos(in_out);

	// get into world space
    HC_Open_Segment_By_Key(m_pHView->GetSceneKey());
		HC_Compute_Coordinates(".","world", &in_out, "viewpoint", &in_out);
    HC_Close_Segment();

    return b_ret;
}


bool HSnapGrid::insert_grid_geometry( const HPoint& origin, const HPoint& ref1, const HPoint& ref2, int count1, int count2)
{
	HC_Open_Segment_By_Key( m_GridSegmentKey );
		// get rid of earlier grid if any
		HC_Flush_Contents(".", "geometry");
		m_GridGeomKey = HC_KInsert_Grid( "quadrilateral", &origin, &ref1, &ref2, count1, count2 );
	HC_Close_Segment();

	return true;
}

bool HSnapGrid::set_default_grid_attributes()
{
    HC_Open_Segment_By_Key( m_GridSegmentKey );
	HC_Set_Visibility("faces = off, edges = on, lines = off, markers = off");
	HC_Set_Color("edges = white");
    HC_Close_Segment();

    return true;
}

bool HSnapGrid::get_grid_parameters( HPlane& retGridPlane, HPoint& retOrigin, HPoint& retRef1, HPoint& retRef2 )
{
    char type[MVO_BUFFER_SIZE];
    HPoint pts[3];
    int count1, count2;
    HC_Show_Grid (m_GridGeomKey, type, &pts[0], &pts[1], &pts[2], &count1, &count2);

    // we need to transform the plane by its corresponding modelling matrix
    float matrix[16];
    HC_Open_Segment_By_Key(m_GridSegmentKey);
	HC_Show_Net_Modelling_Matrix(matrix);
	HC_Compute_Transformed_Points( 3, pts, matrix, pts );
	HC_Compute_Polygon_Plane(3, pts,"right", &retGridPlane);
    HC_Close_Segment();

    retOrigin = pts[0];
    retRef1   = pts[1];
    retRef2   = pts[2];

    return true;
}
