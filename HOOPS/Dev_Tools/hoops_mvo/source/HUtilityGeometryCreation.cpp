// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#include <algorithm>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "HUtilityGeometryCreation.h"
#include "HUtilityGeomHandle.h"
#include "HBaseView.h"
#include "HBaseModel.h"
#include "hc.h"

#include "vhash.h"

#define H_PI (3.1415927f)
#define Two_H_PI (2.0*3.1415927f)
#define H_D2R(theta) ((theta) * H_PI / 180.0f) // convert to radians

/* a class that is used to keep track of all the faces that reference a particular vertex */
class HFaceLookup
{
  private:
    int *face_references;
    int allocated;
    int used;

  public:
    HFaceLookup() {
		allocated = 6;
		face_references = new int[allocated];
		used = 0;
    }
    ~HFaceLookup() {
		delete [] face_references;
    }
    void append( int newv ) {
		if( used == allocated ) {
			int *temp;
			allocated *= 2;
			temp = new int[allocated];
			memcpy( temp, face_references, used * sizeof( int ) );
			delete [] face_references;
			face_references = temp;
		}
		face_references[used++] = newv;
    }
    void kill( int i ) {
		for( ; i < used - 1 ; i++ )
			face_references[i] = face_references[i+1];
		used--;
    }
    int length() { return used; }
    int elem(int i) { return face_references[i]; }
};




bool HUtilityGeometryCreation::GeneratePointOnCircle(HPoint* point, HPoint const &center, float radius, int axis, float angle, bool convert)
{
	HPoint temp;

	switch(axis)
	{
	case HUtility::XZ:
		temp.Set(center.x + radius * (float)cos (angle),
				center.y,
				center.z + radius * (float)sin (angle));
		break;
	case HUtility::XY:
		temp.Set(center.x + radius * (float)cos (angle),
				center.y + radius * (float)sin (angle),
				center.z);
		break;
	case HUtility::YZ:
		temp.Set(center.x,
				center.y + radius * (float)cos (angle),
				center.z + radius * (float)sin (angle));
		break;
	default:
		return false;
	}

	if (convert)
	    HC_Compute_Coordinates(".", "viewpoint", &temp, "world", &temp);

 	*point = temp;

	return true;
}


bool HUtilityGeometryCreation::GeneratePointsOnCircle (HPoint* points, HPoint const & center,float radius,int axis, int numpoints, bool convert)
{
    int i;
    bool success = false;
    float angle = 2.0f * HUtility::PI / (float)numpoints;

    for (i=0; i<numpoints; i++)
    {
	success = GeneratePointOnCircle(&points[i], center, radius, axis, angle*i, convert);
    }

    return success;
}

bool HUtilityGeometryCreation::GeneratePointsOnCircularArc (HPoint * points, HPoint const & center, float radius, int axis, int numpoints, bool convert, float start_angle, float end_angle)
{
	H_ASSERT( numpoints > 1 );

	bool success = false;

	float real_end_angle = end_angle ;
	while( real_end_angle <= start_angle )
		real_end_angle += 2*H_PI;

	float delta = (real_end_angle - start_angle) / ((float)numpoints-1);
	float current_angle = start_angle;
	for (int i=0; i<numpoints; i++)
	{
		success &= HUtilityGeometryCreation::GeneratePointOnCircle(&points[i], center, radius, axis, current_angle, convert);
		current_angle += delta;
	}
	return success;
}

HC_KEY HUtilityGeometryCreation::CreateWireframeCircleWithEllipse(const HPoint &x1, const HPoint &x2, const HPoint &x3 )
{
	HC_KEY ret_geom_key = INVALID_KEY;

	// Using three points lying on the circumference of a circle we find the center of the circle
	//	and then we compute the major and minor axes such that they are the same length

	double A[2][2], InvA[2][2] ;
	HVector v1(x2.x - x1.x, x2.y - x1.y, x2.z - x1.z), v2(x3.x - x1.x, x3.y - x1.y, x3.z - x1.z) ;

	double v1LenSquared = HC_Compute_Vector_Length(&v1), v2LenSquared = HC_Compute_Vector_Length(&v2) ;
	v1LenSquared *= v1LenSquared ; v2LenSquared *= v2LenSquared ;

	double v1_Dot_v2 = HC_Compute_Dot_Product(&v1, &v2) ;

	A[0][0] = 2.0*v1LenSquared ; A[0][1] = 2.0*v1_Dot_v2 ;
	A[1][0] = 2.0*v1_Dot_v2 ;    A[1][1] = 2.0*v2LenSquared ;

	double invDetA = 1.0 / (A[0][0]*A[1][1]-A[0][1]*A[1][0]) ;

	InvA[0][0] = invDetA*(A[1][1]) ;  InvA[0][1] = invDetA*(-A[0][1]) ;
	InvA[1][0] = invDetA*(-A[1][0]) ; InvA[1][1] = invDetA*(A[0][0]) ;

	double alpha = InvA[0][0]*(v1LenSquared) + InvA[0][1]*(v2LenSquared),
		beta  = InvA[1][0]*(v1LenSquared) + InvA[1][1]*(v2LenSquared) ;

	HPoint center(
		static_cast<float>(x1.x + alpha*v1.x + beta*v2.x),
		static_cast<float>(x1.y + alpha*v1.y + beta*v2.y),
		static_cast<float>(x1.z + alpha*v1.z + beta*v2.z));

	HPoint major, minor ;
	major = HPoint(x1) ;
	HVector v3 = major - center, planeNormal, v4 ;
	double radiusSquared = HC_Compute_Vector_Length(&v3) ;
	radiusSquared *= radiusSquared ;

	HC_Compute_Cross_Product(&v1, &v2, &planeNormal) ;
	HC_Compute_Cross_Product(&planeNormal, &v3, &v4) ;

	double v4LenSquared = HC_Compute_Vector_Length(&v4) ;
	v4LenSquared *= v4LenSquared ;

	double delta = sqrt(radiusSquared / v4LenSquared) ;
	minor = HPoint(
		static_cast<float>(center.x + delta * v4.x),
		static_cast<float>(center.y + delta * v4.y),
		static_cast<float>(center.z + delta * v4.z));

	ret_geom_key = HC_KInsert_Elliptical_Arc(&center, &major, &minor, 0.0, 1.0) ;

	return ret_geom_key;
}


HC_KEY HUtilityGeometryCreation::CreateSphere(HPoint center, float radius, int numsides, HPoint axis, HPoint ref)
{
	HPoint	hpos;
	HC_KEY	ret_geom_key = INVALID_KEY;

 	hpos.Set(0,0,0);
 	bool success = false;
	HPoint *points = 0;
	int *faces_list = 0;
	int i;
	int j;

	hpos.y -= radius + radius/(float)numsides;

	int htile, wtile;
	htile = numsides/2;
	wtile = numsides;

	points= new HPoint[htile*wtile];
	assert(points);
	// faces_list will needs wtile + 1 numbers to specify each sphere end cap
	// and 5*numsides numbers for each strip between the end caps.
	// There will be numsides/2-1 of these strips.
	faces_list= new int[(wtile+1)*2 + 5*(htile-1)*wtile];
	assert(faces_list);

	// generate the points for the sphere
	for (i = 0; i < htile; i++)
	{
		float angle = (.5f+(float)i) * HUtility::PI/(float)htile;
		assert(angle > 0);
		assert(angle < HUtility::PI);
		hpos.y = - radius * (float)cos(angle);	
		success = GeneratePointsOnCircle(points+i*wtile, hpos, radius*(float)sin(angle), HUtility::XZ, wtile, false);
		assert(success);
	}

    //
    // assign texture coordinates to the sphere
    //
    int nV = wtile;
    int nU = htile;

    float * uv = new float [ nU * nV  * 2 ]; 
    float *pUV = uv;
    
    float beta = 2.0f * HUtility::PI / (float)nV;
    

    for ( int iu = 0; iu < nU; iu++ )
    {
        float alpha = (.5f+(float)iu ) * HUtility::PI/(float)nU;
        
        float h = (float)iu/(float)nU;
        
        float u = -1 * (float) cos( alpha );
        u = h;
        
        for ( int jv = 0; jv < nV; jv++ )
        {
            float v = beta * jv;
            v = (float)jv/(float)nV;
            *pUV =u;
            pUV++;
            *pUV=v;
            pUV++;
        }
        
    }


	// and then do the connectivity for the four sided quads which represent the sides of the sphere
	// followed by the connectivity for the end caps which are wtile-sided

	// first the side connectivity
	for (i = 0; i < (htile-1); i++)
	{
		for (j = 0; j < wtile; j++)
		{
			// generate the connectivity list for the side quads of the cylinder
			faces_list[(i*wtile + j)*5] = 4; // number of points in this polygon
			faces_list[(i*wtile + j)*5+1]=i*wtile + j;
			faces_list[(i*wtile + j)*5+2]=i*wtile + (j+1)%wtile; //next point along circle; wrap at numsides
			faces_list[(i*wtile + j)*5+3]=(i+1)*wtile + (j+1)%wtile;  // correcsponding point at top of cylinder
			faces_list[(i*wtile + j)*5+4]=(i+1)*wtile + j; //next point along circle; wrap at numsides
		}
	}


	// now the endcap connectivity
	faces_list[5*(htile-1)*wtile] = wtile;
	faces_list[5*(htile-1)*wtile+wtile+1] = wtile;
	for (j = 0; j < wtile; j++)
	{
		// generate the connectivity list for the ends of the cylinder
		faces_list[(htile-1)*wtile*5 + j + 1]= (wtile - j - 1);
		faces_list[(htile-1)*wtile*5 + j + 1 + wtile + 1]= wtile*(htile-1) + j;
	}


	if (points)
	{
 		// put each entity in a separate segment so that we can easily apply
		// transformations to separately created entities	
			HC_Open_Segment("");
				ret_geom_key = HC_KInsert_Shell(htile*wtile, points, (wtile+1)*2 + 5*(htile-1)*wtile, faces_list);
				
				// setting the polygon handedness improves our ability to light the model
				// as well as improve our rendering speed by being able to do one-sided lighting
				// and backplane culling
				HC_Set_Heuristics("polygon handedness=right");
  				HPoint side;
 	 			HC_Compute_Cross_Product(&axis, &ref, &side);
				SetupModellingMatrix(axis, ref, side, center);
   			HC_Close_Segment();
		delete [] points;
	}
	


    HC_MSet_Vertex_Parameters( ret_geom_key, 0, htile*wtile, 2, uv );
    delete [] uv;
    

	if (faces_list)
		delete [] faces_list;

	return ret_geom_key;
}

void HUtilityGeometryCreation::SetupModellingMatrix(HVector axis, HVector ref, HVector side, HPoint pos)
{
    float mat[16] = {axis.x, axis.y, axis.z, 0, 
			ref.x, ref.y, ref.z, 0,
			side.x, side.y, side.z, 0,
			pos.x, pos.y, pos.z,1};

    HC_Set_Modelling_Matrix(mat);
}



// CreateCone(radius, height, ref_axes1, ref_axes2, num_sides)
HC_KEY HUtilityGeometryCreation::CreateSphere(const char *segment, HPoint center, float radius, int numsides)
{
    HPoint	hpos;
	HC_KEY	ret_geom_key = INVALID_KEY;
   
    hpos = center;
    bool success = false;
    HPoint *points = 0;
    int *faces_list = 0;
    int i;
    int j;
 
    hpos.y -= radius + radius/(float)numsides;

    int htile, wtile;
    htile = numsides/2;
    wtile = numsides;

    points= new HPoint[htile*wtile];
    assert(points);
    // faces_list will needs wtile + 1 numbers to specify each sphere end cap
    // and 5*numsides numbers for each strip between the end caps.
    // There will be numsides/2-1 of these strips.
    faces_list= new int[(wtile+1)*2 + 5*(htile-1)*wtile];
    assert(faces_list);

    // generate the points for the bottom of the cylinder.

    HC_Open_Segment(segment);

	for (i = 0; i < htile; i++)
	{
	    float angle = (.5f+(float)i) * HUtility::PI/(float)htile;
	    assert(angle > 0);
	    assert(angle < HUtility::PI);
	    hpos.y = center.y - radius * (float)cos(angle);	
	    success = GeneratePointsOnCircle(points+i*wtile, hpos, radius*(float)sin(angle), HUtility::XZ, wtile, false);
	    assert(success);
	}

	// set the number of points in the end caps of the sphere in the connectivity list
	faces_list[5*(htile-1)*wtile] = wtile;
	faces_list[5*(htile-1)*wtile+wtile+1] = wtile;

	for (i = 0; i < (htile-1); i++)
	{
	    for (j = 0; j < wtile; j++)
	    {
		// generate the connectivity list for the side quads of the cylinder
		faces_list[(i*wtile + j)*5] = 4; // number of points in this polygon
		faces_list[(i*wtile + j)*5+1]=i*wtile + j;
		faces_list[(i*wtile + j)*5+2]=i*wtile + (j+1)%wtile; //next point along circle; wrap at numsides
		faces_list[(i*wtile + j)*5+3]=(i+1)*wtile + (j+1)%wtile;  // corresponding point at top of cylinder
		faces_list[(i*wtile + j)*5+4]=(i+1)*wtile + j; //next point along circle; wrap at numsides
	    }
	}

	for (j = 0; j < wtile; j++)
	{
	    // generate the connectivity list for the ends of the cylinder
	    faces_list[(htile-1)*wtile*5 + j + 1]= (wtile - j - 1);
	    faces_list[(htile-1)*wtile*5 + j + 1 + wtile + 1]= wtile*(htile-1) + j;
	}

	if (points)
	{
		// setting the polygon handedness improves our ability to light the model
		// as well as improve our rendering speed by being able to do one-sided lighting
		// and backplane culling
		HC_Set_Heuristics("polygon handedness=right");
	    ret_geom_key = HC_KInsert_Shell(htile*wtile, points, (wtile+1)*2 + 5*(htile-1)*wtile, faces_list);
	    delete [] points;
	}

    HC_Close_Segment();

    if (faces_list)
	    delete [] faces_list;

	return ret_geom_key;
}


HC_KEY HUtilityGeometryCreation::CreateCone(HPoint center, float radius, float height, int numsides, HPoint axis, HPoint ref)
{
	HPoint	hpos;
	HC_KEY	ret_geom_key = INVALID_KEY;
	
	hpos.Set(0,0,0);
	bool success = false;
	HPoint *points = 0;
	int *faces_list = 0;
	int i;
	int j;

	success=success;

	hpos.y -= radius + radius/(float)numsides;

	int htile, wtile;
	htile = numsides/4;
	if (htile < 2) htile = 2;
	wtile = numsides;
	if (wtile < 4) wtile = 4;

	points= new HPoint[(htile+1)*wtile];
	assert(points);
	// faces_list will needs wtile + 1 numbers to specify the cone end cap
	// and 5*numsides numbers for each strip above the end cap.  Multiple
	// strips are used to ensure more accurate gouraud shading.
	// There will be numsides/2-1 of these strips.
	faces_list= new int[(wtile+1) + 5*(htile-1)*wtile];
	assert(faces_list);

	// generate the points for the bottom of the cylinder.

	for (i = 0; i < htile; i++)
	{
//		float factor = (float)pow(2^i);
		float scale = (float)(1.0 - 1.0/pow(2.0, (double)i));
		if (i == (htile - 1)) scale = 1.0f;
		hpos.y = height * scale;	
		success = GeneratePointsOnCircle(points+i*wtile, hpos, radius-radius*scale, HUtility::XZ, wtile, false);
		assert(success);
	}
	hpos.y = 0;	
	success = GeneratePointsOnCircle(points+htile*wtile, hpos, radius, HUtility::XZ, wtile,false);


	// set the number of points in the end caps of the sphere in the connectivity list
	faces_list[5*(htile-1)*wtile] = wtile;

	for (i = 0; i < (htile-1); i++)
	{
		for (j = 0; j < wtile; j++)
		{
			// generate the connectivity list for the side quads of the cylinder
			faces_list[(i*wtile + j)*5] = 4; // number of points in this polygon
			if (height < 0)
			{
				faces_list[(i*wtile + j)*5+2]=i*wtile + j;
				faces_list[(i*wtile + j)*5+1]=i*wtile + (j+1)%wtile; //next point along circle; wrap at numsides
				faces_list[(i*wtile + j)*5+4]=(i+1)*wtile + (j+1)%wtile;  // correcsponding point at top of cylinder
				faces_list[(i*wtile + j)*5+3]=(i+1)*wtile + j; //next point along circle; wrap at numsides
			}
			else
			{
				faces_list[(i*wtile + j)*5+3]=i*wtile + j;
				faces_list[(i*wtile + j)*5+4]=i*wtile + (j+1)%wtile; //next point along circle; wrap at numsides
				faces_list[(i*wtile + j)*5+1]=(i+1)*wtile + (j+1)%wtile;  // correcsponding point at top of cylinder
				faces_list[(i*wtile + j)*5+2]=(i+1)*wtile + j; //next point along circle; wrap at numsides
			}
		}
	}
	for (j = 0; j < wtile; j++)
	{
		// generate the connectivity list for the ends of the cylinder
		if (height < 0)
		{
			faces_list[(htile-1)*wtile*5 + j + 1]= htile*wtile + j;
		}
		else
		{
			faces_list[(htile-1)*wtile*5 + ((wtile-1)-j) + 1]= htile*wtile + j;
		}
	}
	if (points)
	{
		// put each entity in a separate segment so that we can easily apply
		// transformations to separately created entities	
			HC_Open_Segment("");
				// setting the polygon handedness improves our ability to light the model
				// as well as improve our rendering speed by being able to do one-sided lighting
				// and backplane culling
				HC_Set_Heuristics("polygon handedness=right");
				ret_geom_key = HC_KInsert_Shell((htile+1)*wtile, points, wtile+1 + 5*(htile-1)*wtile, faces_list);
				HPoint side;
 	 			HC_Compute_Cross_Product(&axis, &ref, &side);
				SetupModellingMatrix(axis,ref,side,center);
			HC_Close_Segment();


		delete [] points;
	}
	if (faces_list)
		delete [] faces_list;

	return ret_geom_key;
}


HC_KEY HUtilityGeometryCreation::CreateCylinder(HPoint center, float radius, float height, int numsides, HPoint axis, HPoint ref)
{
	HPoint	bottom, top;
	HC_KEY	ret_geom_key = INVALID_KEY;
	
	bottom = center;
	bottom.Set(0,0,0);
	bool success = false;
	HPoint *points = 0;
	int *faces_list = 0;
	int i;

	success=success;

	points= new HPoint[numsides*4];
	assert(points);
	// faces_list will needs 5 numbers to specify each side quad
	// and CYLINDER_NUMSIDES+1 numbers to specify each end polygon.
	faces_list= new int[numsides*5 + 2*(numsides+1)];
	assert(faces_list);

	// generate the points for the bottom of the cylinder.

	success = GeneratePointsOnCircle(points, bottom, radius, HUtility::XZ, numsides, false);
	success = GeneratePointsOnCircle(points+numsides*2, bottom, radius, HUtility::XZ, numsides, false);
	assert(success);

	// generate the points for the top of the cylinder
	top.Set(0, height, 0);
	success = GeneratePointsOnCircle(points+numsides, top, radius, HUtility::XZ, numsides, false);
	success = GeneratePointsOnCircle(points+numsides*3, top, radius, HUtility::XZ, numsides, false);
	assert(success);

	// set the number of points in the end caps of the cylinder in the connectivity list
	faces_list[numsides*5] = numsides;
	faces_list[numsides*6+1] = numsides;

	for (i = 0; i < numsides; i++)
	{
		// generate the connectivity list for the side quads of the cylinder
		faces_list[i*5] = 4; // number of points in this polygon
		if (height < 0)
		{
			faces_list[i*5+2]=i;
			faces_list[i*5+1]=(i+1)%numsides; //next point along circle; wrap at numsides
			faces_list[i*5+4]=numsides+((i+1)%numsides);  // correcsponding point at top of cylinder
			faces_list[i*5+3]=numsides+i; //next point along circle; wrap at numsides

			// generate the connectivity list for the ends of the cylinder
			faces_list[numsides*5+i+1]= numsides*2 + i;
			faces_list[numsides*6+i+2]= numsides*3 + (numsides-i-1);
		}
		else
		{
			faces_list[i*5+3]=i;
			faces_list[i*5+4]=(i+1)%numsides; //next point along circle; wrap at numsides
			faces_list[i*5+1]=numsides+((i+1)%numsides);  // correcsponding point at top of cylinder
			faces_list[i*5+2]=numsides+i; //next point along circle; wrap at numsides

			// generate the connectivity list for the ends of the cylinder
			faces_list[numsides*5+i+1]= numsides*2 + (numsides-i-1);
			faces_list[numsides*6+i+2]= numsides*3 + i;
		}
	}

	if (points)
	{
 		// put each entity in a separate segment so that we can easily apply
		// transformations to separately created entities	
			HC_Open_Segment("");
				// setting the polygon handedness improves our ability to light the model
				// as well as improve our rendering speed by being able to do one-sided lighting
				// and backplane culling
				HC_Set_Heuristics("polygon handedness=right");
				ret_geom_key = HC_KInsert_Shell(numsides*4, points, numsides*5 + 2*(numsides+1), faces_list);
				HPoint side;
 	 			HC_Compute_Cross_Product(&axis, &ref, &side);
				SetupModellingMatrix(axis, ref, side, center);
  			HC_Close_Segment();
 		delete [] points;
	}
	if (faces_list)
		delete [] faces_list;

	return ret_geom_key;
}



HC_KEY HUtilityGeometryCreation::CreateCuboid( HPoint const *max, HPoint const *min)
{
    HPoint 	points[24];
    int		face_list[30];

	GenerateCuboidPoints(max, min, points);

	// Face1 in XY
    face_list[0] = 4;
    face_list[1] = 0;
    face_list[2] = 1;
    face_list[3] = 2;
    face_list[4] = 3;

	// Face2 in XY
    face_list[5] = 4;
    face_list[6] = 4;
    face_list[7] = 5;
    face_list[8] = 6;
    face_list[9] = 7;

	// Face1 in YZ
    face_list[10] = 4;
    face_list[11] = 8;
    face_list[12] = 9;
    face_list[13] = 10;
    face_list[14] = 11;

	// Face2 in YZ
    face_list[15] = 4;
    face_list[16] = 12;
    face_list[17] = 13;
    face_list[18] = 14;
    face_list[19] = 15;

	// Face1 in XZ
    face_list[20] = 4;
    face_list[21] = 16;
    face_list[22] = 17;
    face_list[23] = 18;
    face_list[24] = 19;

	// Face2 in XZ
    face_list[25] = 4;
    face_list[26] = 20;
    face_list[27] = 21;
    face_list[28] = 22;
    face_list[29] = 23;

	// setting the polygon handedness improves our ability to light the model
	// as well as improve our rendering speed by being able to do one-sided lighting
	// and backplane culling
	HC_Set_Heuristics("polygon handedness=right");
	return HC_KInsert_Shell (24, points, 30, face_list);
}


void HUtilityGeometryCreation::GenerateCuboidPoints( HPoint const *max, HPoint const *min, HPoint *points)
{
	// points for Face1 in XY
    points[0].x = min->x;
    points[0].y = min->y;
    points[0].z = min->z;

    points[1].x = min->x;
    points[1].y = max->y;
    points[1].z = min->z;

    points[2].x = max->x;
    points[2].y = max->y;
    points[2].z = min->z;

    points[3].x = max->x;
    points[3].y = min->y;
    points[3].z = min->z;

	// points for Face2 in XY
    points[4].x = min->x;
    points[4].y = max->y;
    points[4].z = max->z;

    points[5].x = min->x;
    points[5].y = min->y;
    points[5].z = max->z;

    points[6].x = max->x;
    points[6].y = min->y;
    points[6].z = max->z;

    points[7].x = max->x;
    points[7].y = max->y;
    points[7].z = max->z;


	// points for Face1 in YZ
    points[8].x = min->x;
    points[8].y = min->y;
    points[8].z = max->z;

    points[9].x = min->x;
    points[9].y = max->y;
    points[9].z = max->z;
    
    points[10].x = min->x;
    points[10].y = max->y;
    points[10].z = min->z;

    points[11].x = min->x;
    points[11].y = min->y;
    points[11].z = min->z;

	// points for Face2 in YZ
    points[12].x = max->x;
    points[12].y = min->y;
    points[12].z = min->z;

    points[13].x = max->x;
    points[13].y = max->y;
    points[13].z = min->z;
    
    points[14].x = max->x;
    points[14].y = max->y;
    points[14].z = max->z;

    points[15].x = max->x;
    points[15].y = min->y;
    points[15].z = max->z;

	// points for Face1 in XZ
    points[16].x = min->x;
    points[16].y = min->y;
    points[16].z = min->z;

    points[17].x = max->x;
    points[17].y = min->y;
    points[17].z = min->z;
    
    points[18].x = max->x;
    points[18].y = min->y;
    points[18].z = max->z;

    points[19].x = min->x;
    points[19].y = min->y;
    points[19].z = max->z;

	// points for Face2 in XZ
    points[20].x = min->x;
    points[20].y = max->y;
    points[20].z = min->z;

    points[21].x = min->x;
    points[21].y = max->y;
    points[21].z = max->z;
    
    points[22].x = max->x;
    points[22].y = max->y;
    points[22].z = max->z;

    points[23].x = max->x;
    points[23].y = max->y;
    points[23].z = min->z;


}

void HUtilityGeometryCreation::SetCuttingPlaneVisibilityForAll(HC_KEY startkey, const char *visibility)
{
    HC_KEY segkey;
    char res[MVO_BUFFER_SIZE];
    char segmentname[MVO_BUFFER_SIZE];
    HC_Open_Segment_By_Key(startkey);
    HC_Begin_Contents_Search("...", "segments");
    while (HUtility::FindContents(segkey))
    {
	HC_Open_Segment_By_Key(segkey);
	HC_Show_Segment( segkey, res );			//get the qualified path
	HC_Parse_String(res, "/", -1, segmentname); 	//extract the local segment name

	if (strcmp(segmentname, "cp") == 0)
	{
		char segowner[MVO_BUFFER_SIZE];
		HC_Show_Owner_By_Key(segkey, segowner);
		if (!strstr(segowner,"twosection") && !strstr(segowner,"threesection"))
			HC_Set_Visibility(visibility);
		else
		{
			HC_Open_Segment("temp");
			HC_Set_Visibility(visibility);
			HC_Close_Segment();
		}

	}
	HC_Close_Segment();
	
    }
    HC_End_Contents_Search();
    HC_Close_Segment();

}


HC_KEY HUtilityGeometryCreation::CreateCuttingPlane(HBaseView *view, const char *segmentname, bool createPlane)
{
	HPoint min, max, mid;
 	HPoint points[4];
	bool wireframe;
	if ((view->GetRenderMode() == HRenderBRepHiddenLine || 
		view->GetRenderMode() == HRenderHiddenLine || 
		view->GetRenderMode() == HRenderHiddenLineFast) && createPlane)
		wireframe = true;
	else
		wireframe = false;
 
	HC_Open_Segment_By_Key(view->GetCuttingPlanesKey());	

		HC_KEY key;
		if (!segmentname)
			key = HC_KOpen_Segment("cp");
		else
			key = HC_KOpen_Segment(segmentname);		
			HC_Open_Segment("cp");
				bool firsttime = false;
				if (!HC_Show_Existence("modelling matrix"))
					firsttime = true;
				
				HC_Set_Selectability("everything = off");
				HC_Flush_Contents(".", "geometry");	
				HC_Open_Segment_By_Key(view->GetModel()->GetModelKey());
					view->SmartFilterCircumcuboid(".","",&min, &max);
				HC_Close_Segment();
				
				HPoint diagonalVector;
				diagonalVector.Set(
					max.x - min.x,
					max.y - min.y,
					max.z - min.z);
				float diagonalLength = (float) sqrt(
					diagonalVector.x * diagonalVector.x +
					diagonalVector.y * diagonalVector.y +
					diagonalVector.z * diagonalVector.z);
				
				mid.Set((min.x + max.x) / 2.0f, (min.y + max.y) / 2.0f, (min.z + max.z) / 2.0f);
				min.x = - diagonalLength/2.0f;
				min.z=  - diagonalLength/2.0f;
				max.x = + diagonalLength/2.0f;
				max.z=  + diagonalLength/2.0f;
				
				points[0].Set(min.x, 0, min.z);
				points[1].Set(min.x, 0, max.z);
				points[2].Set(max.x, 0, max.z);
				points[3].Set(max.x, 0, min.z);
				
				float matrix[16];
				float matrix2[16];
				HPoint mid2;
				if (firsttime)
				{
					HC_Show_Net_Modelling_Matrix(matrix);
					HC_Compute_Matrix_Inverse(matrix, matrix2);
					HC_Compute_Transformed_Points(1,&mid, matrix2, &mid2);
					HC_Translate_Object(mid2.x, mid2.y, mid2.z);
				}
				
				HC_Set_Color("edges = white");
				HC_Set_Color("faces = (diffuse = light grey, transmission = light grey)");
				HC_Set_Heuristics("no polygon handedness");

				if (view->GetCuttingPlanesVisibility())
				{
					if (wireframe)
					{
						HC_Set_Visibility("cutting planes = off, faces = off, edges = on");
					}
					else
					{
						HC_Set_Visibility("cutting planes = off, faces = on, edges = off");
 						HC_Set_Rendering_Options("face displacement=20");
					}
				}
				else
					HC_Set_Visibility("faces = off, edges = off");

				HC_Insert_Polygon (4, points);	
				
				HC_Open_Segment("");
					HC_Set_Visibility("cutting planes = on");
					if (createPlane)
						HC_Insert_Cutting_Plane(0,1,0,0);
				HC_Close_Segment();
			
			HC_Close_Segment();
		HC_Close_Segment();
	HC_Close_Segment();	
	
	view->SetCuttingPlanesExist(true);
	view->SetGeometryChanged();
	
	return key;
}

 

void HUtilityGeometryCreation::AdjustCuttingPlaneRendering(HBaseView * view)
{
if (view->GetRenderMode() == HRenderHiddenLine || view->GetRenderMode() == HRenderHiddenLineFast || view->GetRenderMode() == HRenderBRepHiddenLine)
    {
	HUtilityGeomHandle::SetVisibilityForAll(view->GetCuttingPlanesKey(), "edges = on, faces = off");
	HUtilityGeometryCreation::SetCuttingPlaneVisibilityForAll(view->GetCuttingPlanesKey(), "edges = on, faces = off");
    }
    else
    {
	HUtilityGeomHandle::SetVisibilityForAll(view->GetCuttingPlanesKey(), "edges = off, faces = on");
	HUtilityGeometryCreation::SetCuttingPlaneVisibilityForAll(view->GetCuttingPlanesKey(), "edges = off, faces = on");
    }
}




void HUtilityGeometryCreation::AdjustSection(HBaseView *view, HC_KEY key) 
{
	UNREFERENCED(view);

    char segment[4096];
	int numsections;
	int i, j;
	HC_KEY planeseg[3] = {0, 0, 0};
    HC_KEY sectioningkey;
	sectioningkey =  HC_KShow_Original_Key(HC_KShow_Owner_By_Key(key));
 
	if (sectioningkey==INVALID_KEY)
		return;
	HC_Show_Segment(sectioningkey, segment);
	if (strstr(segment, "twosection"))
		numsections = 2;
	else if (strstr(segment, "threesection"))
		numsections = 3;
	else
		return;
	HC_Open_Segment_By_Key(sectioningkey);
	HC_Begin_Contents_Search(".", "segments");
	HC_KEY segkey;
	while (HUtility::FindContents(segkey))
	{
		HC_Show_Segment(segkey, segment);
		if (strstr(segment, "plane1"))
			planeseg[0] = segkey;
		else if (strstr(segment, "plane2"))
			planeseg[1] = segkey;
		else if (strstr(segment, "plane3"))
			planeseg[2] = segkey;
	}
	HC_End_Contents_Search();
	HC_Close_Segment();


	for (i=0;i<numsections;i++)
	{
		HPoint polygon[32];
		HPoint polygonnew[32];
		HPoint polygonnew2[32];
		float polygonmatrix[16];
		float planematrix[16];
		float polygoninversematrix[16];
		int plen = 0;
		int plennew = 0;
		int plennew2 = 0;
		HC_KEY p0,p1,p2;
		p0 = p1 = p2 = INVALID_KEY;
		if (i==0)
		{
			p0 = planeseg[0]; p1 = planeseg[1]; p2=planeseg[2];
		}
		else
		if (i==1)
		{
			p0 = planeseg[1]; p1 = planeseg[0]; p2=planeseg[2];
		}
		else
		if (i==2)
		{
			p0 = planeseg[2]; p1 = planeseg[0]; p2=planeseg[1];
		}

		HC_Open_Segment_By_Key(p0);
		HC_Open_Segment("cp");
		HC_Show_Net_Modelling_Matrix(polygonmatrix);
		HC_Compute_Matrix_Inverse(polygonmatrix, polygoninversematrix);
 		HC_KEY polygonkey = HUtility::FindOneObject("polygon", true);
		HC_Close_Segment();
		HC_KEY polygonsegment = HC_KShow_Owner_By_Key(polygonkey);

		HC_Close_Segment();
		HC_Show_Polygon(polygonkey, &plen, polygon);

 		HC_Open_Segment_By_Key(p1);
		HC_Open_Segment("cp");
		HC_Show_Net_Modelling_Matrix(planematrix);
 		HC_Close_Segment();
		HC_Close_Segment();

		float p[4] = {0,1,0,0};
		float pt[4];
		float pt2[4];
 		HC_Compute_Transformed_Plane(p, planematrix, pt);
		HC_Compute_Transformed_Plane(pt, polygoninversematrix, pt2);
		plennew = HUtility::ClipPolygonAgainstPlane(plen, polygon, pt2, polygonnew);

		if (numsections == 3)
		{
 		HC_Open_Segment_By_Key(p2);
		HC_Open_Segment("cp");
		HC_Show_Net_Modelling_Matrix(planematrix);
 		HC_Close_Segment();
		HC_Close_Segment();

		float p[4] = {0,1,0,0};
		float pt[4];
		float pt2[4];
 		HC_Compute_Transformed_Plane(p, planematrix, pt);
		HC_Compute_Transformed_Plane(pt, polygoninversematrix, pt2);
		plennew2 = HUtility::ClipPolygonAgainstPlane(plen, polygonnew, pt2, polygonnew2);
		for (j=0;j<plennew2;j++)
			polygonnew[j] = polygonnew2[j];
		plennew = plennew2;
		}
	
		HC_Open_Segment_By_Key(polygonsegment);
		HC_Set_Visibility("faces = off");
		HC_Open_Segment("temp");
		HC_Flush_Contents(".", "geometry");
		if (!HC_Show_Existence("visibility"))
			HC_Set_Visibility("faces = on");
		HC_Insert_Polygon(plennew, polygonnew);
		HC_Close_Segment();
		HC_Close_Segment(); 
	}

	float sectionplanes[16];
	float startplane[4] = {0,1,0,0};
	float smatrix[16];
	HC_Open_Segment_By_Key(sectioningkey);
	HC_Show_Net_Modelling_Matrix(smatrix); 
	HC_Compute_Matrix_Inverse(smatrix,smatrix);
	HC_Close_Segment();

	for (i=0;i<numsections;i++)
	{
		float matrix[16];

		HC_Open_Segment_By_Key(planeseg[i]);
		HC_Open_Segment("cp");
		HC_Show_Net_Modelling_Matrix(matrix); 
		HC_Compute_Transformed_Plane(startplane, matrix, &sectionplanes[i*4]);
		HC_Compute_Transformed_Plane(&sectionplanes[i*4], smatrix, &sectionplanes[i*4]);
		HC_Close_Segment();
		HC_Close_Segment();
	}
	HC_Open_Segment_By_Key(sectioningkey);
	HC_Open_Segment("sections");
	HC_Flush_Contents(".", "everything");
	HC_Insert_Cutting_Section(numsections, sectionplanes);
	HC_Close_Segment();
	HC_Close_Segment();
			   
}

void HUtilityGeometryCreation::CreateThreeSidedSection(HBaseView *view)
{
	HC_KEY key = HUtilityGeometryCreation::CreateCuttingPlane(view,"threesection/plane1",false);
	HPoint min,max,delta,mid;
	HUtility::CalculateBoundingInfo(key, min, max, delta,mid);
	mid.z += delta.z/2.0f;
 	mid.x -= delta.x/2.0f;

	HUtilityGeomHandle::SetupCuttingPlaneManipulator(key, view,&mid);
 	HC_Open_Segment_By_Key(view->GetCuttingPlanesKey());
	HC_Open_Segment("threesection/plane2");
	HC_Rotate_Object(90,0,0);
	HC_Close_Segment();
	HC_Close_Segment();

	key = HUtilityGeometryCreation::CreateCuttingPlane(view,"threesection/plane2",false);
	HUtility::CalculateBoundingInfo(key, min, max, delta,mid);
	mid.z -= delta.z/2.0f;
 	mid.x -= delta.x/2.0f;

 	HUtilityGeomHandle::SetupCuttingPlaneManipulator(key, view, &mid);



 	HC_Open_Segment_By_Key(view->GetCuttingPlanesKey());
	HC_Open_Segment("threesection/plane3");
	HC_Rotate_Object(0,0,90);
	HC_Close_Segment();
	HC_Close_Segment();

	key = HUtilityGeometryCreation::CreateCuttingPlane(view,"threesection/plane3",false);
	HUtility::CalculateBoundingInfo(key, min, max, delta,mid);
	mid.z += delta.z/2.0f;
	mid.x += delta.x/2.0f;

 	HUtilityGeomHandle::SetupCuttingPlaneManipulator(key, view, &mid);


	HC_Open_Segment_By_Key(view->GetCuttingPlanesKey());
	HC_KEY skey = HC_KCreate_Segment("threesection");
	HC_Close_Segment();
	HUtilityGeomHandle::SetupManipulator(skey,view,0,false,false,false,false,true,true);
		
	AdjustSection(view, key);
	AdjustCuttingPlaneRendering(view);

}

void HUtilityGeometryCreation::CreateTwoSidedSection(HBaseView *view)
{

 	HC_KEY key = HUtilityGeometryCreation::CreateCuttingPlane(view,"twosection/plane1",false);
	HPoint min,max,delta,mid;
	HUtility::CalculateBoundingInfo(key, min, max, delta,mid);
	mid.z += delta.z/2.0f;
	HUtilityGeomHandle::SetupCuttingPlaneManipulator(key, view,&mid);
 	HC_Open_Segment_By_Key(view->GetCuttingPlanesKey());
	HC_Open_Segment("twosection/plane2");
	HC_Rotate_Object(90,0,0);
	HC_Close_Segment();
	HC_Close_Segment();

	key = HUtilityGeometryCreation::CreateCuttingPlane(view,"twosection/plane2",false);
	HUtility::CalculateBoundingInfo(key, min, max, delta,mid);
	mid.z -= delta.z/2.0f;

 	HUtilityGeomHandle::SetupCuttingPlaneManipulator(key, view, &mid);


	HC_Open_Segment_By_Key(view->GetCuttingPlanesKey());
	HC_KEY skey = HC_KCreate_Segment("twosection");
	HC_Close_Segment();
	HUtilityGeomHandle::SetupManipulator(skey,view,0,false,false,false,false,true,true);
		
	AdjustSection(view, key);
	AdjustCuttingPlaneRendering(view);

}

void HUtilityGeometryCreation::CreateSlice(HBaseView *view, float relative_thickness)
{

	HPoint emin,emax;
	view->GetSceneExtents(emin,emax);
	float d = emax.y-emin.y;
 	HC_KEY key = HUtilityGeometryCreation::CreateCuttingPlane(view,"slice/plane1",true);
	HPoint min,max,delta,mid, mid1;
	HUtility::CalculateBoundingInfo(key, min, max, delta,mid1);
	HPoint loc;
	loc.Set(mid1.x,mid1.y,max.z);
	HUtilityGeomHandle::SetupCuttingPlaneManipulator(key, view,&loc, true);
 	

	key = HUtilityGeometryCreation::CreateCuttingPlane(view,"slice/plane2",true);
	HUtility::CalculateBoundingInfo(key, min, max, delta,mid);
	
	loc.Set(mid.x,mid.y,min.z);
 	HUtilityGeomHandle::SetupCuttingPlaneManipulator(key, view, &loc, true);

 	HC_Open_Segment_By_Key(view->GetCuttingPlanesKey());
	HC_Open_Segment("slice/plane1");
	float matrix[16];
	HC_Compute_Offcenter_Rotation(180,0,0,mid.x, mid.y, mid.z, matrix);
	HC_Set_Modelling_Matrix(matrix);
 	HC_Close_Segment();
	HC_Close_Segment();

	HC_Open_Segment_By_Key(view->GetCuttingPlanesKey());
	HC_Open_Segment("slice/plane2");
	HC_Translate_Object(0,d*relative_thickness,0);
	HC_Close_Segment();
	HC_Close_Segment();


	HC_Open_Segment_By_Key(view->GetCuttingPlanesKey());
	HC_KEY skey = HC_KCreate_Segment("slice");
	HC_Close_Segment();
	HUtilityGeomHandle::SetupManipulator(skey,view,0,false,false,false,true,true,true);
		
	AdjustSection(view, key);
	AdjustCuttingPlaneRendering(view);

}


static void get_face_normal( const HPoint *points, const int *face, HVector *out )
{
    float p[3][3];
    memcpy( p[0], &points[ face[1] ], 3 * sizeof( float ) );
    memcpy( p[1], &points[ face[2] ], 3 * sizeof( float ) );
    memcpy( p[2], &points[ face[3] ], 3 * sizeof( float ) );
    HC_Compute_Polygon_Normal( 3, p, "right", out );
}

HC_KEY HUtilityGeometryCreation::CreateShellWithFaceSpecificVertexAttributes(
	float *points,
	int face_list_length,
	int *face_vertex_indices,
	float *normals,
	int *per_face_vertex_normal_indices,
	float *color_values,
	int *per_face_vertex_color_value_indices,
	float *color_findices,
	int *per_face_vertex_color_findex_indices,
	float *params, 
	int *per_face_vertex_param_indices,
	int param_number,
	float crease_angle)
{
	return CreateShellWithFaceSpecificVertexAttributes(
		(HPoint*) points,
		face_list_length,
		face_vertex_indices,
		(HPoint*) normals,
		per_face_vertex_normal_indices,
		(HPoint*) color_values,
		per_face_vertex_color_value_indices,
		color_findices,
		per_face_vertex_color_findex_indices,
		params,
		per_face_vertex_param_indices,
		param_number,
		crease_angle
		);
}


static int hash_point (HPoint const & p)
{
	int a;
	int b;
	int c;
	memcpy(&a, &p.x, sizeof(float));
	memcpy(&b, &p.y, sizeof(float));
	memcpy(&c, &p.z, sizeof(float));
	b *= 5;
	c *= 11;
	return a | (b ^ c);
}


HC_KEY HUtilityGeometryCreation::CreateShellWithFaceSpecificVertexAttributes(
	HPoint *	points,
	int			face_list_length,
	int *		face_vertex_indices,
	HPoint *	normals,
	int *		per_face_vertex_normal_indices,
	HPoint *	color_values,
	int *		per_face_vertex_color_value_indices,
	float *		color_findices,
	int *		per_face_vertex_color_findex_indices,
	float *		params,
	int *		per_face_vertex_param_indices,
	int			param_number,
	float		crease_angle)
{
	H_ASSERT((params == nullptr) == (param_number == 0));

	// Short circuit if additional functionality isn't needed.
	if (!normals && !color_values && !color_findices && !params && crease_angle == 0.0f)
	{
		int largest_point_index = -1;
		for (int i = 0; i < face_list_length; ++i)
		{
			int const face_point_count = face_vertex_indices[i];
			for (int j = 1; j <= face_point_count; ++j)
			{
				int const point_index = face_vertex_indices[i + j];
				if (point_index > largest_point_index) 
					largest_point_index = point_index;
			}
			i += face_point_count;
		}
		int const point_count = largest_point_index + 1;
		return HC_Insert_Shell(point_count, points, face_list_length, face_vertex_indices);
	}

	Vertex_List_Node * head = nullptr;
	Vertex_List_Node * tail = nullptr;

	int vertex_list_size = 0;
	VArray<int> new_face_list(face_list_length);
	float const mindot = cos(H_D2R(crease_angle));

	std::multimap<int, Vertex_List_Node *, std::less<int>> existing_nodes;

	// Parse through points list. Replicate those with unique attributes.
	for (int i = 0; i < face_list_length; ++i)
	{
		// Get the size of the next face.
		new_face_list[i] = face_vertex_indices[i];
		int current_face_vertex_count = face_vertex_indices[i];

		// Get all vertices in the face.
		for (int j = 1; j <= current_face_vertex_count; ++j)
		{
			int const point_index = i + j;

			Vertex_List_Node * new_node = new Vertex_List_Node();
			new_node->next = nullptr;
			new_node->in_face_current = 0;
			new_node->in_face_array.EnsureSize(8);
			new_node->in_face_array[new_node->in_face_current++] = i;
			new_node->point = points[face_vertex_indices[point_index]];
			new_node->color_value = color_values == nullptr || per_face_vertex_color_value_indices == nullptr
				? HPoint(-1, -1, -1)
				: color_values[per_face_vertex_color_value_indices[point_index]]
				;
			new_node->color_findex = color_findices == nullptr || per_face_vertex_color_findex_indices == nullptr
				? -1.0f
				: color_findices[per_face_vertex_color_findex_indices[point_index]]
				;
			new_node->normal = normals == nullptr || per_face_vertex_normal_indices == nullptr
				? HPoint(0, 0, 0)
				: normals[per_face_vertex_normal_indices[point_index]]
				;

			// In the degenerate case, [param_number == 0] should be sufficient, but let's be safe.
			if (params != nullptr && per_face_vertex_param_indices != nullptr && param_number > 0)
			{
				new_node->params.EnsureSize(param_number);
				for (int k = 0; k < param_number; ++k) 
					new_node->params[k] = params[per_face_vertex_param_indices[point_index] * param_number + k];
			}

			int const new_node_hash_key = hash_point(new_node->point);

			if (head == nullptr)
			{
				head = new_node;
				tail = new_node;
				new_node->ID = vertex_list_size++;
				new_face_list[point_index] = new_node->ID;

				existing_nodes.insert(std::pair<int, Vertex_List_Node *>(new_node_hash_key, new_node));
			}
			else
			{
				Vertex_List_Node * existing_duplicate_node = nullptr;

				// Check list for duplicates of new_node.
				if (existing_nodes.count(new_node_hash_key) > 0)
				{
					std::pair<nodes_iterator, nodes_iterator> matches = existing_nodes.equal_range(new_node_hash_key);
					for (nodes_iterator it = matches.first; it != matches.second; ++it)
					{
						Vertex_List_Node * curr_match = it->second;

						if (curr_match->point == new_node->point
							&& curr_match->color_value == new_node->color_value
							&& curr_match->color_findex == new_node->color_findex
							&& curr_match->normal == new_node->normal)
						{
							bool all_params_match = std::equal(
								curr_match->params + 0,
								curr_match->params + param_number,
								new_node->params + 0);

							if (all_params_match)
							{
								existing_duplicate_node = curr_match;
								break;
							}
						}
					}
				}

				// Check if we need to break this vertex up anyway because of the hard edge tolerance.
				if (existing_duplicate_node != nullptr && crease_angle > 0.0f)
				{
					// Get a normal for the current face
					HPoint v1;
					get_face_normal(points, &face_vertex_indices[i], &v1);
					
					for (int k = 0; k < existing_duplicate_node->in_face_current; ++k)
					{
						// Compare v1 pairwise with the normal of each other face this vertex appears in.
						HPoint v2;
						get_face_normal(points, &face_vertex_indices[existing_duplicate_node->in_face_array[k]], &v2);

						if (HC_Compute_Dot_Product(&v1, &v2) < mindot)
						{
							// We need to break at this vertex. Don't treat it as a duplicate.
							existing_duplicate_node = nullptr;
							break;
						}
					}
				}

				// Did we find a duplicate? If so, delete new_node and use the existing node.
				if (existing_duplicate_node != nullptr)
				{
					delete new_node;

					new_face_list[point_index] = existing_duplicate_node->ID;

					existing_duplicate_node->in_face_array.EnsureSize(existing_duplicate_node->in_face_array.Count() + 1);
					existing_duplicate_node->in_face_array[existing_duplicate_node->in_face_current++] = i;
				}
				else
				{
					new_node->ID = vertex_list_size++;
					tail->next = new_node;
					tail = new_node;
					new_face_list[point_index] = new_node->ID;

					existing_nodes.insert(std::pair<int, Vertex_List_Node *>(new_node_hash_key, new_node));
				}
			} // End uniqueness test
		} // End current face
		
		i += current_face_vertex_count;
	} // End checking all vertices

	// Create a shell using the points list and face list.
	// TODO: Consider degenerate case where [vertex_list_size == 0].
	VArray<HPoint> new_points(vertex_list_size);
	VArray<HPoint> new_color_values;
	VArray<HPoint> new_normals;
	VArray<float> new_color_findices;
	VArray<float> new_params;

	if (color_values)
		new_color_values.EnsureSize(vertex_list_size);
	if (color_findices)
		new_color_findices.EnsureSize(vertex_list_size);
	if (normals)
		new_normals.EnsureSize(vertex_list_size);
	if (params)
		new_params.EnsureSize(vertex_list_size * param_number);

	// Fill new lists.
	{
		Vertex_List_Node * list_temp = head;
		for (int i = 0; i < vertex_list_size; ++i)
		{
			new_points[i] = list_temp->point;
			if (color_values)
				new_color_values[i] = list_temp->color_value;
			if (color_findices)
				new_color_findices[i] = list_temp->color_findex;
			if (normals)
				new_normals[i] = list_temp->normal;

			if (params)
			{
				for (int j = 0; j < param_number; ++j)
					new_params[i * param_number + j] = list_temp->params[j];
			}

			list_temp = list_temp->next;
		}
	}

	HC_KEY shell_key = HC_Insert_Shell(vertex_list_size, new_points, face_list_length, new_face_list);

	// Override vertex properties
	if (color_values)
		HC_MSet_Vertex_Colors_By_Value(shell_key, "vertex, face, edge", 0, "RGB", vertex_list_size, new_color_values);
	if (color_findices)
		HC_MSet_Vertex_Colors_By_FIndex(shell_key, "vertex, face, edge", 0, vertex_list_size, new_color_findices);
	if (normals)
		HC_MSet_Vertex_Normals(shell_key, 0, vertex_list_size, new_normals);
	if (params)
		HC_MSet_Vertex_Parameters(shell_key, 0, vertex_list_size, param_number, new_params);

	{
		Vertex_List_Node * list_temp = head;
		while (list_temp != nullptr)
		{
			head = list_temp->next;
			delete list_temp;
			list_temp = head;
		}
	}

	return shell_key;
}



HC_KEY HUtilityGeometryCreation::GenerateFEAShell( HC_KEY definingShell, bool generateIsolines, const char *algorithm)
{
	UNREFERENCED(algorithm);

	if(definingShell == INVALID_KEY)
		return INVALID_KEY;

	char type[32];
	HC_Show_Key_Type(definingShell, type);
	if(streq(type, "shell") || streq(type, "mesh")){
		HShellWrapper myshell(definingShell);	
		myshell.SubdivideQuadFaces(generateIsolines);
		return myshell.m_Key;
	}
	return INVALID_KEY;
}



static void replace_vertex_in_face( int *face, int oldv, int newv )
{
    int i;
    int end = face[0] + 1;
    for( i = 1 ; i != end; i++ ) {
		if( face[i] == oldv ) 
			face[i] = newv;
    }
}


HC_KEY HUtilityGeometryCreation::CreateSmoothedShell(
		int PointCount, 
		const HPoint *Points, 
		int FaceListLength, 
		const int *FaceList, 
		float creaseAngle)
{
	HPoint *NewPoints;
	int *NewFaceList;
	int *face, *end;
	int a, i, j, k, len;
	int FaceCount;
	int NewPointCount;
	int NewPointsAllocated = PointCount * 3;
	HPoint *FaceNormals;
	HPoint *v1, *v2;
	int **FacePtrs;
	HFaceLookup **flkup;
    float mindot = (float) cos ( H_D2R(creaseAngle) );
	HC_KEY ReturnKey;

	NewFaceList = new int[FaceListLength];
	memcpy (NewFaceList, FaceList, FaceListLength*sizeof(int));

	NewPointCount = PointCount;
	NewPoints = new HPoint[NewPointsAllocated];
	memcpy (NewPoints, Points, NewPointCount*sizeof(HPoint));

	flkup = new HFaceLookup *[NewPointsAllocated];
	memset (flkup, 0, NewPointsAllocated * sizeof (HFaceLookup *));
	for (i = 0 ; i < NewPointCount ; i++)
		flkup[i] = new HFaceLookup;

	//count up the number of faces
	FaceCount = 0;
	face = NewFaceList;
	end = face + FaceListLength;
	while (face < end) {
		len = *face;
		if (len < 0)
			len = -len;
		face += len + 1;
		FaceCount++;
	}

	FaceNormals = new HPoint[FaceCount];
	FacePtrs = new int *[FaceCount];
	i = 0;
	face = NewFaceList;
	while (face < end) {
		len = *face;
		if (len < 0)
			len = -len;
		get_face_normal (Points, face, &FaceNormals[i]);
		FacePtrs[i] = face;
		for (j = 0 ; j < len ; j++)
			// add face i to list of faces referencing that vertex 
			flkup[ face[j+1] ]->append( i ); 
		face += len + 1;
		i++;
	}

    //for each vertex i
    for ( i = 0 ; i < NewPointCount ; i++ ) {
		//compare all face normals with all others that reference the same vertex
		//note that we walk backwards because we are killing vertices as we go
		for( j = flkup[i]->length() - 1 ; j > 0 ; j-- ) {
			v1 = &FaceNormals[ flkup[i]->elem(j) ];
			if (HC_Compute_Vector_Length (v1) > 0.1) {
				for( k = j - 1 ; k >= 0 ; k-- ) {
					v2 = &FaceNormals[ flkup[i]->elem(k) ];
					if( HC_Compute_Dot_Product(v1,v2) < mindot  &&
						HC_Compute_Vector_Length (v2) > 0.1) {
						if (NewPointCount == NewPointsAllocated) {
							NewPointsAllocated = (int)(NewPointsAllocated * 1.2f);
							if (NewPointsAllocated < 12)
								NewPointsAllocated = 12;
							HFaceLookup **newflkup = new HFaceLookup *[NewPointsAllocated];
							memcpy (newflkup, flkup, NewPointCount*sizeof(HFaceLookup *));
							delete [] flkup;
							flkup = newflkup;
							HPoint *temp = new HPoint [NewPointsAllocated];
							memcpy (temp, NewPoints, NewPointCount*sizeof(HPoint));
							delete [] NewPoints;
							NewPoints = temp;
						}
						flkup[NewPointCount] = new HFaceLookup;
						//split vertex i into two so face flkup[i]->elem[j] and face flkup[i]->elem[k] reference different 
						//vertices with different attributes
						memcpy( &NewPoints[NewPointCount], &NewPoints[i], sizeof( HPoint ) );
						replace_vertex_in_face( FacePtrs[flkup[i]->elem(k)], i, NewPointCount );

						//the last vertex is now referenced by one (and only one) face
						flkup[NewPointCount]->append(flkup[i]->elem(k));  
						flkup[i]->kill(k);  //remove element k from the array

						for( a = k - 1 ; a >= 0 ; a-- ) {
							//v1 = &FaceNormals[ flkup[i]->elem(j) ];
							v2 = &FaceNormals[ flkup[i]->elem(a) ];
							if( HC_Compute_Dot_Product(v1,v2) < mindot ) {
								replace_vertex_in_face( FacePtrs[flkup[i]->elem(a)], i, NewPointCount );
								flkup[NewPointCount]->append(flkup[i]->elem(a));  
								flkup[i]->kill(a);  //remove element a from the array
								k = a;
							}
						}
						NewPointCount++;
					}
				}
			}
		}
    }
	ReturnKey = HC_KInsert_Shell (NewPointCount, NewPoints, FaceListLength, NewFaceList);
	for ( i = 0 ; i < NewPointCount ; i++ )
		delete flkup[i];
	delete [] flkup;
	delete [] FacePtrs;
	delete [] FaceNormals;
	delete [] NewFaceList;
	delete [] NewPoints;
	return ReturnKey;
}


bool HUtilityGeometryCreation::ComputeFIndexDemoData (HShellVertexData *pShellVertexData, int data_cycles) 
{
	int             	total_pts = pShellVertexData->GetVertexCount() ;
	float          		*findices = 0;
    float           	x, y, z;

	// We have a valid shell or mesh and know the number of vertices
	// it contains.  Generate floating point data for each of the vertices.
	// The size of the data is the total number of points in the
	// shell or mesh plus extra values equal to the number of animation
	// cycles.  At time step 't' during the animation,  the data that is
	// used starts at index 't' in the array m_svdp...->findices and is
	// total_pts long.

	if ( total_pts > 0 )
	{
		findices = new float[pShellVertexData->GetVertexCount() + data_cycles];

		HC_Open_Geometry(pShellVertexData->GetKey());
			if (streq (pShellVertexData->GetType(), "shell"))
			{ 
				int nn = 0, i=0;
				for (; nn < total_pts + data_cycles; ++nn, ++i) 
				{
					//  Compute floating point number based on vertex normal.
					//  Map the y component of the normal into the interval
					//  [0, 12.0] for shells
					if (nn < total_pts) 
					{
						HC_Open_Vertex (nn);
						HC_Show_Net_Normal (&x, &y, &z);
						findices[i] = 6.0F * (1.0F + x);
						HC_Close_Vertex ();
					}
					else 
						findices[i] = 2.0F;
				}
			}
			else if (streq (pShellVertexData->GetType(), "mesh"))
			{
				int rows, cols;

				HPoint * point_list = new HPoint[total_pts];
				HC_Show_Mesh(pShellVertexData->GetKey(), &rows, &cols, point_list);

				float max = point_list[0].z;
				float min = point_list[0].z;

				int nn = 0;
				for (nn = 0; nn < total_pts; ++nn)
				{
					if (point_list[nn].z > max)
						max = point_list[nn].z;
					else if (point_list[nn].z < min)
						min = point_list[nn].z;
				}
				float mlt = 12.0F /(max-min);

				int i = 0;
				for (nn = 0; nn < total_pts + data_cycles; ++nn, ++i) 
				{
				//  Compute floating point number based on z value of vertex.
				//  Map the range of z values into the interval
				//  [0, 12.0] for meshes

					if (nn < total_pts)
						findices[i] = mlt * (point_list[nn].z - min);
					else 
						findices[i] = 2.0F;
				}
				for (nn = 0; nn < total_pts; ++nn)
				{
					point_list[nn].z = 0.0F;
				}
				delete [] point_list;
			}
		HC_Close_Geometry ();

		pShellVertexData->SetFIndexData(pShellVertexData->GetVertexCount() + data_cycles, findices);
		HC_MSet_Vertex_Colors_By_FIndex (pShellVertexData->GetKey(), "faces, edges, markers", 0, total_pts, findices);
		delete [] findices;
	}
    return true;
}


HC_KEY HUtilityGeometryCreation::CreateBoolean(HC_KEY target, HC_KEY tool, int bool_type)
{
	char options[256], first_seg_name[2048], second_seg_name[2048];
		HC_KEY target_segment, tool_segment, bool_key, ret_key=INVALID_KEY;
		int first_pcount, first_flist_length, *first_face_list,
			second_pcount, second_flist_length, *second_face_list,
			bool_pcount, bool_flist_length, *bool_face_list, i;
		HPoint *first_points, *second_points, *first_normals, *second_normals, *bool_points;
		float first_transform[16], second_transform[16], inv_first_transform[16];
		char booleantype[MVO_BUFFER_SIZE];

		//make sure we're dealing with shells
		HC_Show_Key_Type(target, first_seg_name);
		HC_Show_Key_Type(tool, second_seg_name);
		if(!streq(first_seg_name, "shell") || !streq(second_seg_name, "shell"))
			return INVALID_KEY;

		//allocate arrays for point and face lists
		HC_Show_Shell_Size(target, &first_pcount, &first_flist_length);
		HC_Show_Shell_Size(tool, &second_pcount, &second_flist_length);
		first_points=new HPoint [first_pcount];
		first_face_list=new int [first_flist_length];
		first_normals = new HPoint [first_pcount];
		second_points=new HPoint [second_pcount];
		second_face_list=new int [second_flist_length];
		second_normals = new HPoint [second_pcount];

		//get segment names for later use and clear the selection list
		target_segment=HC_KShow_Owner_Original_Key(target);
		tool_segment=HC_KShow_Owner_Original_Key(tool);
		HC_Show_Segment(target_segment, first_seg_name);
		HC_Show_Segment(tool_segment, second_seg_name);
		

		//get shell data
		HC_Show_Shell(target, &first_pcount, first_points, &first_flist_length, first_face_list);
		HC_Show_Shell(tool, &second_pcount, second_points, &second_flist_length, second_face_list);


		//get the transforms
		HC_QShow_Net_Modelling_Matrix(first_seg_name, first_transform);
		HC_QShow_Net_Modelling_Matrix(second_seg_name , second_transform);

		//get the inverse of the first matrix for later use
		if(HC_Compute_Matrix_Determinant(first_transform)) 
			HC_Compute_Matrix_Inverse(first_transform, inv_first_transform);
		else 
			HC_Compute_Matrix_Adjoint(first_transform, inv_first_transform);

		//get the shell handedness
		char shell1handedness[MVO_BUFFER_SIZE];
		char shell2handedness[MVO_BUFFER_SIZE];
		HC_QShow_One_Net_Heuristic(first_seg_name, "polygon handedness", shell1handedness);
		HC_QShow_One_Net_Heuristic(second_seg_name, "polygon handedness", shell2handedness);

		if (strcmp(shell1handedness, "right")!= 0)
			strcpy(shell1handedness,"left");
		if (strcmp(shell2handedness, "right")!= 0)
			strcpy(shell2handedness,"left");

		//apply the transforms
		HC_Compute_Transformed_Points(first_pcount, first_points, first_transform, first_points);
		HC_Compute_Transformed_Points(second_pcount, second_points, second_transform, second_points);
		
		//apply the proper boolean type
		if(bool_type==BOOLEAN_TYPE_INTERSECT)
			strcpy(booleantype, "intersect");
		else if(bool_type==BOOLEAN_TYPE_SUBTRACT)
			strcpy(booleantype, "subtract");
		else
			strcpy(booleantype, "union");
		
		sprintf(options, "boolean type = %s,  tool handedness = %s, target handedness = %s", 
			booleantype, shell1handedness, shell2handedness);

		//get your normals here
		HC_Open_Geometry(target);
		for(i=0; i<first_pcount; ++i)
		{
			HC_Open_Vertex(i);
				HC_Show_Net_Normal(&first_normals[i].x, &first_normals[i].y, &first_normals[i].z);
			HC_Close_Vertex();
		}
		HC_Close_Geometry();

		HC_Open_Geometry(tool);
		for(i=0; i<second_pcount; ++i)
		{
			HC_Open_Vertex(i);
				HC_Show_Net_Normal(&second_normals[i].x, &second_normals[i].y, &second_normals[i].z);
			HC_Close_Vertex();
		}
		HC_Close_Geometry();

		//do it
		HC_Open_Segment_By_Key(target_segment);
			bool_key=HC_KCompute_Boolean_Shell(first_pcount, first_points, first_normals, 
												first_flist_length, first_face_list,
												second_pcount, second_points, second_normals, 
												second_flist_length, second_face_list, 
												options);
			
			if (bool_key != INVALID_KEY)
			{			
				HC_Show_Shell_Size(bool_key, &bool_pcount, &bool_flist_length);
				bool_points=new HPoint [bool_pcount];
				bool_face_list=new int [bool_flist_length];
				HC_Show_Shell(bool_key, &bool_pcount, bool_points, &bool_flist_length, bool_face_list);
	
				HC_Compute_Transformed_Points(bool_pcount, bool_points, inv_first_transform, bool_points);
				ret_key=HC_KInsert_Shell(bool_pcount, bool_points, bool_flist_length, bool_face_list);
				HC_Delete_By_Key(bool_key);
				HC_Set_Heuristics("polygon handedness=left");
				
				delete [] bool_points;
				delete [] bool_face_list;
			}
		HC_Close_Segment();

		//clean up
		delete [] first_points;
		delete [] first_face_list;
		delete [] first_normals;
		delete [] second_points;
		delete [] second_face_list;
		delete [] second_normals;

		return ret_key;
}

HC_KEY HUtilityGeometryCreation::CreateShellFromRotationalSweep (int nPoints, const HPoint *profile, const float *radii, float start_angle, float end_angle, int axis, const bool *hard, int n_sides)
{
	float angle = end_angle - start_angle;
	int sides = n_sides;
	if(sides <= 3)
		sides = 3;

	// Main Algorithm
	int i,j;
	int numpoints = sides+1;
	HPoint center(0,0,0);

	HPoint *points = 0;
	int *face_list = 0;
	int total_facelist = sides*2*4*nPoints;
	face_list = new int[total_facelist];
	assert(face_list);

	bool *ifHard = 0;
	ifHard = new bool[nPoints];
	H_ASSERT(ifHard);

	// whether hard or smooth edge, copies the input array - default is hard
	if(hard==0)							
	{
		for(i=0;i<nPoints;i++) 
			ifHard[i]=true;
	}
	else
	{
		for(i=0;i<nPoints;i++) 
			ifHard[i]=hard[i];
	}

	// depending upon hard/smooth edge allot space for points.
	// if Hard Edge generates duplicate points on same edge.
	int total_points=0;					
	for(i=0; i<nPoints; i++)
	{
		if(ifHard[i]==true)
			total_points += (numpoints*2);
		else
			total_points += (numpoints);
	}

	points = new HPoint[total_points];
	H_ASSERT(points);


	//These variables are to be used for closing the ends of swept profile (not applicable for 360 degree sweep.
	HPoint *start_end=0;
	HPoint *last_end=0;
	int *face_list_ends=0;
	if(!feq(angle, Two_H_PI, HPOINT_EPSILON))
	{
		start_end = new HPoint[nPoints];
		H_ASSERT(start_end);

		last_end = new HPoint[nPoints];
		H_ASSERT(last_end);

		face_list_ends = new int[nPoints+1];
		H_ASSERT(face_list_ends);
		face_list_ends[0]= nPoints;
		for(i=1; i<nPoints+1; i++) face_list_ends[i] = i-1; 
	}

	/// double pts generation along same edge to take the hard edges into account
	// Generate points on each point at given radius of profile.
	center = profile[0];							
	for(i=0, j=0; i<nPoints; i++)
	{
		center = profile[i];
		GeneratePointsOnCircularArc(points+(j)*numpoints, center, radii[i], axis, numpoints, false, start_angle, end_angle);
		if(!feq(angle, Two_H_PI, HPOINT_EPSILON))		// For less than 360 rotation to close the ends.
		{
			start_end[i] = points[j*numpoints];
			last_end[(nPoints-1)-i]  = points[j*numpoints+sides];	// To separate the points at end to close the sweep
			// This is to maintain the polygon handedness
		}
		j++;

		if(ifHard[i]==true)
		{
			GeneratePointsOnCircularArc((points+(j)*numpoints), center, radii[i], axis, numpoints, false, start_angle, end_angle);
			j++;
		}
	}

	// Create Face_List
	int upper = 0;
	int lower = 0;

	for(i=0, j=0; i<nPoints; i++)
	{
		if(i != nPoints-1)	// if the coming loop is not the last one
		{
			lower = upper+numpoints*(int)ifHard[i];
			upper = lower+numpoints;
		}	
		if(i==nPoints-1)	// if the coming loop is last 
		{
			lower = upper+numpoints*(int)ifHard[i];
			upper = 0;
		}
		for(int k=0; k<sides; k++)
		{
			if(k!=sides-1)
			{
				face_list[j++]  = 3;
				face_list[j++] = lower+k;
				face_list[j++] = lower+k+1;
				face_list[j++] = upper+k;

				face_list[j++] = 3;
				face_list[j++] = lower+k+1;
				face_list[j++] = upper+k+1;
				face_list[j++] = upper+k;		
			}

			if(k==sides-1)
			{
				if(!feq(angle, Two_H_PI, HPOINT_EPSILON))	// Reapeat the same for last  2 triangles of the face if no close loop
				{
					face_list[j++]  = 3;
					face_list[j++] = lower+k;
					face_list[j++] = lower+k+1;
					face_list[j++] = upper+k;

					face_list[j++] = 3;
					face_list[j++] = lower+k+1;
					face_list[j++] = upper+k+1;
					face_list[j++] = upper+k;
				}
				else // Make the last 2 triangles include the start points to complete a loop.
				{
					face_list[j++]  = 3;
					face_list[j++] = lower+k;
					face_list[j++] = lower;
					face_list[j++] = upper+k;

					face_list[j++] = 3;
					face_list[j++] = lower;
					face_list[j++] = upper;
					face_list[j++] = upper+k;	
				}
			}
		}
	}

	HC_KEY spun_geom;
	// put each entity in a separate segment so that we can easily apply
	// transformations to separately created entities	
	spun_geom = HC_KInsert_Shell(total_points, points, total_facelist, face_list);
	if(!feq(angle, Two_H_PI, HPOINT_EPSILON))
	{
		HC_KInsert_Shell(nPoints, start_end, nPoints+1, face_list_ends);
		HC_KInsert_Shell(nPoints, last_end, nPoints+1, face_list_ends);	

		spun_geom = HUtility::Merge_Shells(true, true, false, true);
	}

	delete [] points;
	delete [] face_list;

	if (start_end)
		delete [] start_end;
	if (last_end)
		delete [] last_end;
	if (face_list_ends)
		delete [] face_list_ends;
	if (ifHard)
		delete [] ifHard;

	return spun_geom;
}



/*!
\ Internal utility class to store the profile (being swept) details viz. points and profile normal. 
 */
class HSweepPathProfile
{
public:
	int m_nPointCount;
	HPoint * m_pNormal;
	HPoint * m_pPoints;	

	HSweepPathProfile()
	{
		m_pPoints = 0;
		m_pNormal = 0;
		m_pPoints = 0;
	};

	~HSweepPathProfile()
	{
		H_SAFE_DELETE( m_pNormal );
		H_SAFE_DELETE_ARRAY( m_pPoints );
	};
};

/*!
\ Internal utility class to store the (sweep) path point infomation viz. profile at the point, it's coords, radius etc
*/
class HSweepPathPoint
{
public:
	HPoint m_Center;
	float m_fRadius;
	HPoint m_AxisVector;
	HSweepPathProfile* prof_main;
	HSweepPathProfile* prof_sub; 
	HSweepPathProfile* prof_special;	//???

	//** Used In Liner
	HSweepPathPoint* next;
	HSweepPathPoint* prev;
	//**

	//for Liner(not used 7thJan 08)
	float scale_factor;

	// ??? 30th April 08
	// order of the point in the path.
	int m_nOrder;


	HSweepPathPoint()
	{
		prof_main=0;
		prof_sub=0;
		prof_special=0;
		next=this;
		prev=this;
		m_fRadius = -1; // Useful when circular profiles.
		scale_factor = 1;
	
		// 30thApril 08
		m_nOrder = -1;

	};
	~HSweepPathPoint()
	{
		if(prof_special)
		{
			delete prof_special;
			prof_special = 0;
		}
	};
	void Set()
	{
		m_Center.Set(0.0, 0.0, 0.0);	
	};
	void SetAxisVector(HPoint * first, HPoint * second)
	{
		m_AxisVector = *first - *second;
		SetAxisVector(m_AxisVector);
	};
	void SetAxisVector(HPoint& vector)
	{
		double mag = HC_Compute_Vector_Length(&vector);
		if(mag == 0.0)
			m_AxisVector.Set(0,0,1);
		else
			m_AxisVector = vector;

	};
	void SetAxisVector(float a, float b, float c)
	{
		m_AxisVector.x = a;
		m_AxisVector.y = b;
		m_AxisVector.z = c;

		SetAxisVector(m_AxisVector);
	};
};


// This is preprocessor function provided to understand working of the general sweep
/*
	\nPointsProf-		number of points on profile,
	\m_pPoints-	array of profile points
	\nProfileInnerPts -	number of points on inner profile (if we have annular profile,else NULL)
	\ProfileInnerPts -	array of inner profile points (here if we don't have annular- NULL)
	\nPointsPath-		number of points in path.
	\points_on_path-	array of points on path.
	\bIsConcentric			HPSTATE whether ANNULUS
	\bIsProfileClosed			HPSTATE whether Closed
	*/
static void FillProfile(const int& nPoints, const HPoint * Points, HSweepPathProfile* Prof)
{
    Prof->m_nPointCount = nPoints;

    // calculate profile normal
    // There will be atleast 3 points in profile, hence we can find out the normal of this profile.
    HVector ProfileNormal;
    HVector FirstVect = Points[0] - Points[1];
    HVector SecondVect = Points[2] - Points[1];
    HC_Compute_Cross_Product(&SecondVect, &FirstVect, &ProfileNormal);
    HC_Compute_Normalized_Vector(&ProfileNormal, &ProfileNormal);
    Prof->m_pNormal = new HPoint(ProfileNormal);

    Prof->m_pPoints = new HPoint[Prof->m_nPointCount];
    for(int i=0;i<Prof->m_nPointCount;i++)
    {
        Prof->m_pPoints[i].Set(Points[i]);
    }
}

static float GetDistance(const HPoint& p1, const HPoint& p2)
{
	HPoint diff = p1 - p2;
	return (float)HC_Compute_Vector_Length(&diff);
}

static bool LookForAnAbruptChange(HSweepPathPoint *path)
{
	HPoint vector1(path->prev->m_Center - path->m_Center);
	HPoint vector2(path->next->m_Center - path->m_Center);

	double v1_mag = HC_Compute_Vector_Length(&vector1);
	double v2_mag = HC_Compute_Vector_Length(&vector2);
	if( (v1_mag == 0.0) || (v2_mag == 0.0) )
		return false;

	HC_Compute_Normalized_Vector(&vector1, &vector1);
	HC_Compute_Normalized_Vector(&vector2, &vector2);
	
	float Angle = static_cast<float>(HC_Compute_Vector_Angle( &vector1, &vector2 ));
	if( Angle > 150.0f )
		return false;
	/*
	Create a temperory profile set a special normal vector2-vector1.
	*/	
	HSweepPathProfile TempProfile;
	TempProfile.m_nPointCount = path->prof_main->m_nPointCount;
	TempProfile.m_pNormal = new HPoint(vector2-vector1);//
	TempProfile.m_pPoints = new HPoint[TempProfile.m_nPointCount];
	/*
	Copy the current profile points into it
	*/
	for(int i=0; i<path->prof_main->m_nPointCount; i++)
	{
		TempProfile.m_pPoints[i].Set(path->prof_main->m_pPoints[i]);
	}
	/*
	Set the rotation matrix, and fill it with rotation angle of current profile and temperory profile with
	special normal.
	*/
	float rotate_matrix[4*4] = {	1,0,0,0,
									0,1,0,0,
									0,0,1,0,
									0,0,0,1
								};
	HUtility::ComputeVectorToVectorRotationMatrix(rotate_matrix, path->prof_main->m_pNormal, TempProfile.m_pNormal);
	// Rotate the temperory profile to its normal
	for(int i=0; i<path->prof_main->m_nPointCount; i++)
	{
		HC_Compute_Transformed_Points(1, &TempProfile.m_pPoints[i], rotate_matrix, &TempProfile.m_pPoints[i]);
	}

	// Fill the special profile in HSweepPathProfile, this will hold the projected points of current profile
	//on the temporory profile.
	if(path->prof_special)
		delete path->prof_special;
	path->prof_special = new HSweepPathProfile();
	path->prof_special->m_pNormal = new HPoint(path->prof_main->m_pNormal);
	path->prof_special->m_nPointCount = path->prof_main->m_nPointCount;
	path->prof_special->m_pPoints = new HPoint[path->prof_special->m_nPointCount];
	HPoint temp;
	for(int i=0; i<path->prof_main->m_nPointCount; i++)
	{
		temp.Set(path->prof_main->m_pPoints[i]-TempProfile.m_pPoints[0]);
		//
		float d = -static_cast<float>((HC_Compute_Dot_Product (TempProfile.m_pNormal, &temp))/(HC_Compute_Dot_Product (TempProfile.m_pNormal,&vector1)));
		
		path->prof_special->m_pPoints[i].x = path->prof_main->m_pPoints[i].x + (vector1.x*d);
		path->prof_special->m_pPoints[i].y = path->prof_main->m_pPoints[i].y + (vector1.y*d);
		path->prof_special->m_pPoints[i].z = path->prof_main->m_pPoints[i].z + (vector1.z*d);

	}

	return true;
}

static bool GenerateProfilePointsOnPath (HPoint * points, HSweepPathPoint * path)
{
	bool success = false;
	float rotate_matrix[4*4] = {	1,0,0,0,
									0,1,0,0,
									0,0,1,0,
									0,0,0,1
								};
	float translate_matrix[4*4] ={	1,0,0,0,
									0,1,0,0,
									0,0,1,0,
									path->m_Center.x,path->m_Center.y,path->m_Center.z,1
								};

	HVector PathTangent, ProfileNormal;
	PathTangent.Set(path->m_AxisVector);
	ProfileNormal.Set(path->prof_main->m_pNormal);

	HC_Compute_Normalized_Vector(&PathTangent, &PathTangent);
	HC_Compute_Normalized_Vector(&ProfileNormal, &ProfileNormal);

	float dprod = (float)HC_Compute_Dot_Product(&PathTangent, &ProfileNormal);

	if(fabs(fabs(dprod)- 1.000 )> 0.0001F )  // For parallel and antiparallel vectors keep identity matrix.
		HUtility::ComputeVectorToVectorRotationMatrix(rotate_matrix, path->prof_main->m_pNormal, path->m_AxisVector);

	HC_Compute_Transformed_Points(path->prof_main->m_nPointCount, path->prof_main->m_pPoints, rotate_matrix, path->prof_main->m_pPoints);
	if(path->prof_sub)
		HC_Compute_Transformed_Points(path->prof_main->m_nPointCount, path->prof_sub->m_pPoints, rotate_matrix, path->prof_sub->m_pPoints);

	HC_Compute_Transformed_Points(path->prof_main->m_nPointCount, path->prof_main->m_pPoints, translate_matrix, points);

	
	path->prof_main->m_pNormal->Set(path->m_AxisVector);

	if(path->prof_sub)
		path->prof_sub->m_pNormal->Set(path->m_AxisVector);

	// Now if the corner is sharp, profile needs to be moderated.
	bool AbruptChange = LookForAnAbruptChange(path);
	if(AbruptChange==true)
	{
		HC_Compute_Transformed_Points(path->prof_main->m_nPointCount, path->prof_special->m_pPoints, translate_matrix, points);
	}
	
	return success;
}

static void BringProfileToTheOrigine(HSweepPathPoint * PathPt)
{
	float Bring_It_To_Origin_Matrix[4*4] = {	1,0,0,0, 
												0,1,0,0,
												0,0,1,0, 
												0,0,0,1
											};// Added on 29th April 08 Ashish

	Bring_It_To_Origin_Matrix[12] = -PathPt->m_Center.x;
	Bring_It_To_Origin_Matrix[13] = -PathPt->m_Center.y;
	Bring_It_To_Origin_Matrix[14] = -PathPt->m_Center.z;
	
	HC_Compute_Transformed_Points(PathPt->prof_main->m_nPointCount, PathPt->prof_main->m_pPoints, Bring_It_To_Origin_Matrix, PathPt->prof_main->m_pPoints );
	if(PathPt->prof_sub)
	{
		HC_Compute_Transformed_Points(PathPt->prof_main->m_nPointCount, PathPt->prof_sub->m_pPoints, Bring_It_To_Origin_Matrix, PathPt->prof_sub->m_pPoints);
	}
}


/////Generate Quads
static HC_KEY GeneralProfileSweep(int nPoints, HSweepPathPoint *path, bool *hard, 
								  bool bIsProfileClosed, bool bIsConcentric)
{
	if(nPoints<=1)
		return -1;

	int i,j;
	int sides =path[0].prof_main->m_nPointCount;
	int numpoints = sides;
	
	HPoint *points = 0;
	int *face_list = 0;
	int total_facelist=0;
	if(bIsConcentric)
	{
		if(bIsProfileClosed)
			total_facelist = sides*5*nPoints;
		else
			total_facelist = (sides-1)*5*(nPoints);
	}
	else // i.e if NONANNULUS
	{
		if(bIsProfileClosed)
			total_facelist = sides*5*(nPoints-1);  // since nonannulus it doesnt need to join first and last profile thats why (nPoints-1)
		else
			total_facelist = (sides-1)*5*(nPoints-1);
	}
	face_list = new int[total_facelist];
	assert(face_list);

	bool *ifHard = new bool[nPoints];
	assert(ifHard);

	// whether hard or smooth edge, copies the input array

	if(*hard == false)							
	{
		for(i=0;i<nPoints;i++) 
			ifHard[i]=false;
	}
	else
	{
		for(i=0;i<nPoints;i++) 
			ifHard[i]=hard[i];
	}

	// depending upon hard/smooth edge allot space for points.
	// if Hard Edge generates duplicate points on same edge.
	int total_points=0;					
	for(i=0; i<nPoints; i++)
	{
		if(ifHard[i]==true)
			total_points += (numpoints*2);
			
		else
			total_points += (numpoints);
	}

	points = new HPoint[total_points];
	assert(points);

	// This is important- before sweeping the profile bring it to the origine for further transformations
	// the point of the path is a relative point considered for the profile -Under Construction Beta
	// Added on 30th April 08
	BringProfileToTheOrigine(&path[0]);

	
	//These variables are to be used for closing the ends of swept profile (not applicable for Annulus body).
	HPoint *start_end=0;
	HPoint *last_end=0;
	int *face_list_for_startend=0;
	int *face_list_for_lastend=0;
	if(bIsProfileClosed && !bIsConcentric)
	{
		start_end = new HPoint[sides];
		assert(start_end);

		last_end = new HPoint[sides];
		assert(last_end);

		face_list_for_startend = new int[sides+1];
		assert(face_list_for_startend);
		face_list_for_startend[0]= sides;
		for(i=1; i<sides+1; i++)	face_list_for_startend[i] = sides-i;
		

		face_list_for_lastend = new int[sides+1];
		assert(face_list_for_lastend);
		face_list_for_lastend[0]= sides;
		for(i=1; i<sides+1; i++)	face_list_for_lastend[i] = i-1; 	//Reversed to maintain handedness

		GenerateProfilePointsOnPath(start_end, &path[0]);				// First point on Path
		
	}//

	// To find Texture Coordinates
	HPoint * TextureCoords;
	TextureCoords = new HPoint[total_points];
	float totalLengthOfPath = 0;
	float * len_from_start = new float[nPoints];
	for(i =0; i<nPoints-1; i++)
	{
		len_from_start[i] = totalLengthOfPath;
		totalLengthOfPath = totalLengthOfPath + GetDistance(&path[i].m_Center, &path[i+1].m_Center);
	}
	len_from_start[i] = totalLengthOfPath;
	// double pts generation along same edge to take the hard edges into account
	// Generate points of profile on each point at point of path.
	for(i=0, j=0; i<nPoints; i++)
	{	
		GenerateProfilePointsOnPath(points+(j)*numpoints, &path[i]);
		// TO GET THE TEXTURE COORDINATES
		//SetTextureCoordinates(TextureCoords+(j)*numpoints, points+(j)*numpoints, len_from_start[i], totalLengthOfPath, &path[i]);
		j++;

		if(ifHard[i]==true)
		{
			GenerateProfilePointsOnPath(points+(j)*numpoints, &path[i]);
			// TO GET THE TEXTURE COORDINATES
			//SetTextureCoordinates(TextureCoords+(j)*numpoints, points+(j)*numpoints, len_from_start[i], totalLengthOfPath, &path[i]);
			j++;
		}
	}
	delete [] len_from_start;

 	if( bIsProfileClosed && !bIsConcentric )
	{// Closing face of the swept profile
		GenerateProfilePointsOnPath(last_end, &path[nPoints-1]);// Last Point on Path
	}

	// Create Face_List
	int upper = 0;
	int lower = 0;
 
	for(i=0, j=0; i<nPoints; i++)
	{
		if(i!=nPoints-1)	// if the coming loop is not the last one
		{
			lower = upper+numpoints*( (ifHard[i] == true) ? 1 : 0 );
			upper = lower+numpoints;
		}	
		if(i==nPoints-1)	// if the coming loop is last 
		{
			lower = upper+numpoints*( (ifHard[i] == true) ? 1 : 0 );
			upper = 0;
			
			if(!bIsConcentric)
				break;
		}
		for(int k=0; k<sides; k++)
		{
			if(k!=sides-1)
			{
				face_list[j++]  = 4;
				face_list[j++] = lower+k;
				face_list[j++] = lower+k+1;
				face_list[j++] = upper+k+1;
				face_list[j++] = upper+k;			
			}

			if(k==sides-1)
			{
				if(!bIsProfileClosed)	// Reapeat the same for last  2 triangles of the face if no close loop
				{
					break;
				}
				else // Make the last 2 triangles include the start points to complete a loop.
				{
					face_list[j++]  = 4;
					face_list[j++] = lower+k;
					face_list[j++] = lower;
					face_list[j++] = upper;
					face_list[j++] = upper+k;
					
					/*face_list[j++] = 3;
					face_list[j++] = lower;
					face_list[j++] = upper;
					face_list[j++] = upper+k;*/
				}
			}
			
		}		
	}

	HC_KEY sweep=0;
	// Add each facet as a shell.
	for(int i =0; i< total_facelist;)
	{
		int size_unit_face = face_list[i] + 1;

		int * list_unit_face = new int[size_unit_face];
		list_unit_face[0] = face_list[i];
		for(int k = 0; k<face_list[i]; k++)
			list_unit_face[k+1] = k;

		HPoint * pts_unit_face = new HPoint[size_unit_face - 1];
		for(int k = 0; k < face_list[i]; k++)
			pts_unit_face[k] = points[face_list[i+k+1]];

		HC_Insert_Shell(face_list[i], pts_unit_face, size_unit_face, list_unit_face);

		delete [] list_unit_face;
		delete [] pts_unit_face;

		i = i + size_unit_face;
	}
	if( !bIsConcentric  && bIsProfileClosed )
	{
		HC_KInsert_Shell(sides, start_end, sides+1, face_list_for_startend);
		HC_KInsert_Shell(sides, last_end, sides+1, face_list_for_lastend);   
	}

	sweep = HUtility::Merge_Shells(false, true, false, false);

	//// Merge these separately added facets to form a single shell
	//HC_Optimize_Segment_Tree(".", "merge shells");

	// SetTextureCoordinates
	// OpenShellAndSetTextureCoords(sweep, total_points, TextureCoords);
		
	delete [] points;
	delete [] face_list;

	if (start_end)
		delete [] start_end;

	if (last_end)
		delete [] last_end;

	if (face_list_for_startend)
		delete [] face_list_for_startend;

	if (face_list_for_lastend)
		delete [] face_list_for_lastend;

	if (ifHard)
		delete [] ifHard; 

	delete [] TextureCoords;

	return sweep;
}

////Generate  Traingles
//static HC_KEY GeneralProfileSweep(int nPoints, HSweepPathPoint *path, bool *hard, 
//								  bool bIsProfileClosed, bool bIsConcentric)
//{
//	if(nPoints<=1)
//		return -1;
//
//	int i,j;
//	int sides =path[0].prof_main->m_nPointCount;
//	int numpoints = sides;
//	
//	HPoint *points = 0;
//	int *face_list = 0;
//	int total_facelist=0;
//	if(bIsConcentric)
//	{
//		if(bIsProfileClosed)
//			total_facelist = sides*2*4*nPoints;
//		else
//			total_facelist = (sides-1)*2*4*(nPoints);
//	}
//	else // i.e if NONANNULUS
//	{
//		if(bIsProfileClosed)
//			total_facelist = sides*2*4*(nPoints-1);  // since nonannulus it doesnt need to join first and last profile thats why (nPoints-1)
//		else
//			total_facelist = (sides-1)*2*4*(nPoints-1);
//	}
//	face_list = new int[total_facelist];
//	assert(face_list);
//
//	bool *ifHard = new bool[nPoints];
//	assert(ifHard);
//
//	// whether hard or smooth edge, copies the input array
//
//	if(hard == false)							
//	{
//		for(i=0;i<nPoints;i++) ifHard[i]=false;
//	}
//	else
//	{
//		for(i=0;i<nPoints;i++) ifHard[i]=hard[i];
//	}
//
//	// depending upon hard/smooth edge allot space for points.
//	// if Hard Edge generates duplicate points on same edge.
//	int total_points=0;					
//	for(i=0; i<nPoints; i++)
//	{
//		if(ifHard[i]==true)
//			total_points += (numpoints*2);
//			
//		else
//			total_points += (numpoints);
//	}
//
//	points = new HPoint[total_points];
//	assert(points);
//
//	// This is important- before sweeping the profile bring it to the origine for further transformations
//	// the point of the path is a relative point considered for the profile -Under Construction Beta
//	// Added on 30th April 08
//	BringProfileToTheOrigine(&path[0]);
//
//	
//	//These variables are to be used for closing the ends of swept profile (not applicable for Annulus body).
//	HPoint *start_end=0;
//	HPoint *last_end=0;
//	int *face_list_for_startend=0;
//	int *face_list_for_lastend=0;
//	if(bIsProfileClosed && !bIsConcentric)
//	{
//		start_end = new HPoint[sides];
//		assert(start_end);
//
//		last_end = new HPoint[sides];
//		assert(last_end);
//
//		face_list_for_startend = new int[sides+1];
//		assert(face_list_for_startend);
//		face_list_for_startend[0]= sides;
//		for(i=1; i<sides+1; i++)	face_list_for_startend[i] = sides-i;
//		
//
//		face_list_for_lastend = new int[sides+1];
//		assert(face_list_for_lastend);
//		face_list_for_lastend[0]= sides;
//		for(i=1; i<sides+1; i++)	face_list_for_lastend[i] = i-1; 	//Reversed to maintain handedness
//
//		GenerateProfilePointsOnPath(start_end, &path[0]);				// First point on Path
//		
//	}//
//
//	// To find Texture Coordinates
//	HPoint * TextureCoords;
//	TextureCoords = new HPoint[total_points];
//	float totalLengthOfPath = 0;
//	float * len_from_start = new float[nPoints];
//	for(i =0; i<nPoints-1; i++)
//	{
//		len_from_start[i] = totalLengthOfPath;
//		totalLengthOfPath = totalLengthOfPath + GetDistance(&path[i].m_Center, &path[i+1].m_Center);
//	}
//	len_from_start[i] = totalLengthOfPath;
//	// double pts generation along same edge to take the hard edges into account
//	// Generate points of profile on each point at point of path.
//	for(i=0, j=0; i<nPoints; i++)
//	{	
//		GenerateProfilePointsOnPath(points+(j)*numpoints, &path[i]);
//		// TO GET THE TEXTURE COORDINATES
//		//SetTextureCoordinates(TextureCoords+(j)*numpoints, points+(j)*numpoints, len_from_start[i], totalLengthOfPath, &path[i]);
//		j++;
//
//		if(ifHard[i]==true)
//		{
//			GenerateProfilePointsOnPath(points+(j)*numpoints, &path[i]);
//			// TO GET THE TEXTURE COORDINATES
//			//SetTextureCoordinates(TextureCoords+(j)*numpoints, points+(j)*numpoints, len_from_start[i], totalLengthOfPath, &path[i]);
//			j++;
//		}
//	}
//	delete [] len_from_start;
//
// 	if( bIsProfileClosed && !bIsConcentric )
//	{// Closing face of the swept profile
//		GenerateProfilePointsOnPath(last_end, &path[nPoints-1]);// Last Point on Path
//	}
//
//	// Create Face_List
//	int upper = 0;
//	int lower = 0;
// 
//	for(i=0, j=0; i<nPoints; i++)
//	{
//		if(i!=nPoints-1)	// if the coming loop is not the last one
//		{
//			lower = upper+numpoints*( (ifHard[i] == true) ? 1 : 0 );
//			upper = lower+numpoints;
//		}	
//		if(i==nPoints-1)	// if the coming loop is last 
//		{
//			lower = upper+numpoints*( (ifHard[i] == true) ? 1 : 0 );
//			upper = 0;
//			
//			if(!bIsConcentric)
//				break;
//		}
//		for(int k=0; k<sides; k++)
//		{
//			if(k!=sides-1)
//			{
//				face_list[j++]  = 3;
//				face_list[j++] = lower+k;
//				face_list[j++] = lower+k+1;
//				face_list[j++] = upper+k;
//				
//
//				face_list[j++] = 3;
//				face_list[j++] = lower+k+1;
//				face_list[j++] = upper+k+1;
//				face_list[j++] = upper+k;
//				
//			}
//
//			if(k==sides-1)
//			{
//				if(!bIsProfileClosed)	// Reapeat the same for last  2 triangles of the face if no close loop
//				{
//					break;
//				}
//				else // Make the last 2 triangles include the start points to complete a loop.
//				{
//					face_list[j++]  = 3;
//					face_list[j++] = lower+k;
//					face_list[j++] = lower;
//					face_list[j++] = upper+k;
//					
//					face_list[j++] = 3;
//					face_list[j++] = lower;
//					face_list[j++] = upper;
//					face_list[j++] = upper+k;
//				}
//			}
//			
//		}		
//	}
//
//	HC_KEY sweep=0, end1=0, end2=0;
//	// put each entity in a separate segment so that we can easily apply
//	// transformations to separately created entities	
//	 sweep = HC_KInsert_Shell(total_points, points, total_facelist, face_list);
//
//	 if( !bIsConcentric  && bIsProfileClosed )
//	{
//		end1 = HC_KInsert_Shell(sides, start_end, sides+1, face_list_for_startend);
//		end2 = HC_KInsert_Shell(sides, last_end, sides+1, face_list_for_lastend);	
//
//		if(nPoints>1)
//			sweep = HUtility::Merge_Shells(false, true, false, false);
//	}
//	 // SetTextureCoordinates
//	// OpenShellAndSetTextureCoords(sweep, total_points, TextureCoords);
//		
//	delete [] points;
//	delete [] face_list;
//
//	if (start_end)
//		delete [] start_end;
//
//	if (last_end)
//		delete [] last_end;
//
//	if (face_list_for_startend)
//		delete [] face_list_for_startend;
//
//	if (face_list_for_lastend)
//		delete [] face_list_for_lastend;
//
//	if (ifHard)
//		delete [] ifHard; 
//
//	delete [] TextureCoords;
//
//	return sweep;
//}
//
// Profile should be placed at the start point of path.
HC_KEY HUtilityGeometryCreation::CreateShellFromPathSweep(
		int nProfilePoints, 
		const HPoint * pOuterProfilePoints, 
		const HPoint * pInnerProfilePoints, 
		int nPathPoints, 
		const HPoint * pSweepPathPoints, 
		const bool bIsProfileClosed)
{
	bool is_annular = (pInnerProfilePoints == 0) ? false : true;

	HSweepPathProfile Prof;
	HSweepPathProfile Prof_inner;
	FillProfile(nProfilePoints,pOuterProfilePoints,&Prof);
	if( pInnerProfilePoints != 0 )
	{
		FillProfile( nProfilePoints, pInnerProfilePoints, &Prof_inner);
	}

	int npts = nPathPoints;
	const HPoint * points = pSweepPathPoints;

	int n_path_point = 0, i = 0, j = 0;

	if(is_annular)
		n_path_point = npts*2;
	else
		n_path_point = npts;

	HSweepPathPoint* path_points = new HSweepPathPoint[n_path_point];
	bool* Hard = new bool [n_path_point];

	if(!is_annular)
	{
		path_points[i].Set();
		path_points[i].m_Center.Set(points[i]);
		path_points[i].SetAxisVector(0,0,10);
		path_points[i].prof_main = &Prof;
		path_points[i].m_nOrder = i;
		path_points[i].next = &path_points[i+1];

		Hard[i] = false;
		i++;
		for(;i<npts;i++)
		{
			path_points[i].Set();
			path_points[i].m_Center.Set(points[i]);
			path_points[i].SetAxisVector(&path_points[i].m_Center, &path_points[i-1].m_Center);
			path_points[i].prof_main = &Prof;
			path_points[i].m_nOrder = i;

			path_points[i].prev = &path_points[i-1];
			if(i < npts - 1)
				path_points[i].next = &path_points[i+1];

			Hard[i] = false;
			//HC_Insert_Marker(path_points[i].m_Center.x, path_points[i].m_Center.y, path_points[i].m_Center.z);
		}
		path_points[0].SetAxisVector(path_points[1].m_AxisVector);
	}
	else if(is_annular)
	{

		path_points[i].Set();
		path_points[i].m_Center.Set(points[i]);
		path_points[i].SetAxisVector(0,0,10);
		path_points[i].prof_main = &Prof;
		path_points[i].prof_sub = &Prof_inner;
		path_points[i].m_nOrder = i;

		path_points[i].next = &path_points[i+1];

		Hard[i] = true;	
		//HC_Insert_Marker(path_points[i].m_Center.x, path_points[i].m_Center.y, path_points[i].m_Center.z);	
		i++;
		for(;i<n_path_point/2;i++)
		{
			path_points[i].Set();
			path_points[i].m_Center.Set(points[i]);
			path_points[i].SetAxisVector(&path_points[i].m_Center, &path_points[i-1].m_Center);
			path_points[i].prof_main= &Prof;
			path_points[i].prof_sub = &Prof_inner;
			path_points[i].m_nOrder = i;

			path_points[i].prev = &path_points[i-1];
			if(i < n_path_point - 1)
				path_points[i].next = &path_points[i+1];

			Hard[i] = false;
			//HC_Insert_Marker(path_points[i].m_Center.x, path_points[i].m_Center.y, path_points[i].m_Center.z);
		}
		Hard[i-1] = true;
		path_points[0].SetAxisVector(path_points[1].m_AxisVector); // first point axis vector adjusted.

		j = npts-1; // now we want to move back along the curve
		path_points[i].Set();
		path_points[i].m_Center.Set(points[j]);
		path_points[i].SetAxisVector(0,0,10);
		path_points[i].prof_main = &Prof_inner;
		path_points[i].m_nOrder = i;

		path_points[i].prev = &path_points[i-1];
		if(i < n_path_point - 1)
			path_points[i].next = &path_points[i+1]; 

		Hard[i] = true;	
		//HC_Insert_Marker(path_points[i].m_Center.x, path_points[i].m_Center.y, path_points[i].m_Center.z);	
		i++;j--;
		for(;i<n_path_point;i++,j--)
		{
			path_points[i].Set();
			path_points[i].m_Center.Set(points[j]);	// Note j  ****
			path_points[i].SetAxisVector(&path_points[i].m_Center, &path_points[i-1].m_Center);
			path_points[i].prof_main = &Prof_inner;
			path_points[i].m_nOrder = i;

			path_points[i].prev = &path_points[i-1];
			if(i < n_path_point - 1)
				path_points[i].next = &path_points[i+1];

			Hard[i] = false;
			//HC_Insert_Marker(path_points[i].m_Center.x, path_points[i].m_Center.y, path_points[i].m_Center.z);
		}
		Hard[i-1] = true;
		path_points[n_path_point/2].SetAxisVector(path_points[n_path_point/2+1].m_AxisVector); // return first point axis vector adjusted.

	}

	HC_KEY swept_geom = GeneralProfileSweep(n_path_point, path_points, Hard, bIsProfileClosed, is_annular);

	delete []path_points;
	delete []Hard;
	return swept_geom;
}







