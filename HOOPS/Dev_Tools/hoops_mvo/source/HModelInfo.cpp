// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "HTools.h"
#include "HModelInfo.h"
#include "HUtility.h"





void HModelInfo::ComputeModelInfo(HC_KEY segment_key, bool recurse) 
{
	if(segment_key==INVALID_KEY)
		segment_key=m_SegmentKey;
	
	ResetCounters();

	ComputeSegmentContents(segment_key, recurse);
}




void HModelInfo::ResetCounters()
{
    m_TotalCircle = 0;
    m_TotalCircularArc = 0;
    m_TotalCircularChord = 0;
    m_TotalCircularWedge = 0;
    m_TotalEllipse = 0;
    m_TotalEllipticalArc = 0;
    m_TotalGrid = 0;
    m_TotalImage = 0;
	m_TotalIncludes = 0;
	m_TotalIncluders = 0;
    m_TotalLine = 0;
    m_TotalMarker = 0;
    m_TotalMesh = 0;
    m_TotalPolygon = 0;
    m_TotalPolyline = 0;
    m_TotalShell = 0;
    m_TotalStringCursor = 0;
    m_TotalText = 0;
    m_TotalSegments = 0;
	m_TotalMatrices = 0;

    m_TotalDistantLight = 0;
    m_TotalLocalLight = 0;
    m_TotalSpotLight = 0;

    m_TotalFaces = 0;
    m_TotalTriangles = 0;
	m_TotalTristrips = 0;
    m_TotalVertices = 0;

    m_TotalCuttingPlane = 0;

	m_TotalPolyCylinders = 0;
	m_TotalCylinders = 0;
	m_TotalSpheres = 0;
	m_TotalNURBSCurves = 0;
	m_TotalNURBSSurfaces = 0;
	m_TotalReferences = 0;

	m_TotalInfiniteLines = 0;
	m_TotalInfiniteRays = 0;

	m_TotalOthers = 0;

}


void HModelInfo::IncrementCounter(char *type, HC_KEY key) 
{

    if (streq (type, "circle")) {
        m_TotalCircle++;
    }

    else if (streq (type, "circular arc")) {
        m_TotalCircularArc++;
    }

    else if (streq (type, "circular chord")) {
        m_TotalCircularChord++;
    }

    else if (streq (type, "circular wedge")) {
        m_TotalCircularWedge++;
    }

    else if (streq (type, "cutting plane")) {
        m_TotalCuttingPlane++;
    }

    else if (streq (type, "ellipse")) {
        m_TotalEllipse++;
    }

    else if (streq (type, "elliptical arc")) {
        m_TotalEllipticalArc++;
    }

    else if (streq (type, "grid")) {
        m_TotalGrid++;
    }

    else if (streq (type, "image")) {
        m_TotalImage++; 
    } 

	else if (streq (type, "include")) {
		m_TotalIncludes++;
	}

    else if (streq (type, "line")) {
        m_TotalLine++; 
    } 

    else if (streq (type, "marker")) { 
        m_TotalMarker++; 
    } 

     else if (streq (type, "mesh")) { 
         int        rows, columns;

        HC_Show_Mesh_Size (key, &rows, &columns);
        m_TotalFaces += (rows - 1) * (columns -1);
        m_TotalMesh++;
        m_TotalVertices += rows * columns;
        m_TotalTriangles += 2 * (rows - 1) * (columns -1);

    }

    else if (streq (type, "polygon")) {
        int count;

        m_TotalPolygon++;
        m_TotalFaces++;

        HC_Show_Polygon_Count (key, &count);
        m_TotalVertices += count;

        m_TotalTriangles += count - 2;
    }

    else if (streq (type, "polyline")) {
        m_TotalPolyline++;
    }

    else if (streq (type, "shell")) {
         
         m_TotalShell++;

        // obtain totals for shell components
        int face_count = 0;
        int vertex_count = 0;
        int triangle_count = 0;
		int tristrip_count = 0;

        HUtility::CountShellData (key, &face_count, &vertex_count, &triangle_count, &tristrip_count);        
        
        m_TotalFaces        += face_count;  
        m_TotalVertices     += vertex_count;
        m_TotalTriangles    += triangle_count;
		m_TotalTristrips    += tristrip_count;
    }

    else if (streq (type, "segment")) {
        m_TotalSegments++;
    }

    else if (streq (type, "string cursor")) {
        m_TotalStringCursor++;
    }

    else if (streq (type, "text")) {
        m_TotalText++;
    }

	else if (streq (type, "modelling matrix")) {
        m_TotalMatrices++;
    }

	else if( streq (type, "cylinder") ) {
		m_TotalCylinders++;
	}
	else if( streq (type, "polycylinder") ) {
		m_TotalPolyCylinders++;
	}
	else if( streq (type, "sphere") ) {
		m_TotalSpheres++;
	}
	else if( streq (type, "nurbs curve") ) {
		m_TotalNURBSCurves++;
	}
	else if( streq (type, "nurbs surface") ) {
		m_TotalNURBSSurfaces++;
	}
	else if( streq (type, "reference") ) {
		m_TotalReferences++;
	}
	else if (streq (type, "light")) 
	{ 
		char light_type[MVO_BUFFER_SIZE];
		
		HC_Show_Key_Type(key, light_type);

		if (streq (light_type, "distant light"))
			m_TotalDistantLight++;

		else if (streq (light_type, "spot light")) 
			m_TotalSpotLight++;
		
		else if (streq (light_type, "local light")) 
			m_TotalLocalLight++;
		
		else 		
			m_TotalOthers++;
	}
	else if(streq(type, "infinite line"))
	{
		++m_TotalInfiniteLines;
	}
	else if(streq(type, "infinite ray"))
	{
		++m_TotalInfiniteRays;
	}
	else {
		/* This should always be zero.  If you find this happening, implement the unhandled type. */
		m_TotalOthers++;

	}
}



void HModelInfo::IncrementCounters() 
{
	HC_KEY key;
	char type[64];

	if (HC_Show_Existence (
			"circle, circular chord, circular wedge, cutting plane, ellipse, "
			"grid, image, string cursor, nurbs surface, sphere, reference, infinite line, infinite ray, "
			"lights")) {
		m_TotalCircle += HC_Show_Existence ("circle");
		m_TotalCircularChord += HC_Show_Existence ("circular chord");
		m_TotalCircularWedge += HC_Show_Existence ("circular wedge");
		m_TotalCuttingPlane += HC_Show_Existence ("cutting plane");
		m_TotalEllipse += HC_Show_Existence ("ellipse");
		m_TotalGrid += HC_Show_Existence ("grid");
		m_TotalImage += HC_Show_Existence ("image");
		m_TotalStringCursor += HC_Show_Existence ("string cursor");
		m_TotalNURBSSurfaces += HC_Show_Existence ("nurbs surface");
		m_TotalSpheres += HC_Show_Existence ("sphere");
		m_TotalReferences += HC_Show_Existence ("reference");
		m_TotalInfiniteLines += HC_Show_Existence ("infinite line");
		m_TotalInfiniteRays += HC_Show_Existence ("infinite ray");
		if (HC_Show_Existence ("lights")) {
			char light_type[64];
			HC_Begin_Contents_Search (".", "lights");
			while (HC_Find_Contents (type, &key)) {
				HC_Show_Key_Type(key, light_type);
				if (streq (light_type, "distant light"))
					m_TotalDistantLight++;
				else if (streq (light_type, "spot light")) 
					m_TotalSpotLight++;
				else if (streq (light_type, "local light")) 
					m_TotalLocalLight++;
			}
			HC_End_Contents_Search ();
		}
	}
	if (HC_Show_Existence (
			"line, marker, "
			"circular arc, elliptical arc, nurbs curve")) {
		m_TotalLine += HC_Show_Existence ("line");
		m_TotalMarker += HC_Show_Existence ("marker");
		m_TotalCircularArc += HC_Show_Existence ("circular arc");
		m_TotalEllipticalArc += HC_Show_Existence ("elliptical arc");
		m_TotalNURBSCurves += HC_Show_Existence ("nurbs curve");
	}
	if (HC_Show_Existence (
			"cylinder, polycylinder, polygon, polyline, text, modelling matrix")) {
		m_TotalCylinders += HC_Show_Existence ("cylinder");
		m_TotalPolyCylinders += HC_Show_Existence ("polycylinder");
		m_TotalPolygon += HC_Show_Existence ("polygon");
		m_TotalPolyline += HC_Show_Existence ("polyline");
		m_TotalText += HC_Show_Existence ("text");
		m_TotalMatrices += HC_Show_Existence ("modelling matrix");
	}		
	if (HC_Show_Existence ("shells, meshes")) {
		HC_Begin_Contents_Search (".", "shells");
		while (HC_Find_Contents (type, &key)) {
			m_TotalShell++;
			// obtain totals for shell components
			int face_count = 0;
			int vertex_count = 0;
			int triangle_count = 0;
			int tristrip_count = 0;
			HUtility::CountShellData (key, &face_count, &vertex_count, &triangle_count, &tristrip_count);        
			m_TotalFaces        += face_count;  
			m_TotalVertices     += vertex_count;
			m_TotalTriangles    += triangle_count;
			m_TotalTristrips    += tristrip_count;
		}
		HC_End_Contents_Search ();

		HC_Begin_Contents_Search (".", "meshes");
		while (HC_Find_Contents (type, &key)) {
			int        rows, columns;
			HC_Show_Mesh_Size (key, &rows, &columns);
			m_TotalFaces += (rows - 1) * (columns -1);
			m_TotalMesh++;
			m_TotalVertices += rows * columns;
			m_TotalTriangles += 2 * (rows - 1) * (columns -1);
		}
		HC_End_Contents_Search ();
	}
}


// Perform a contents search on "start segment" and all of its 
// sub-segments, following includes. Search for all geometry and 
// compute totals for all HOOPS primitives
// and totals derived from the primitive totals. Derived totals include:
// total faces, vertices and triangles in the model.

void HModelInfo::ComputeSegmentContents(HC_KEY the_key, bool recurse)
{
    HC_KEY	key;
	char	*type = new char [MVO_BUFFER_SIZE];
	char	*pathname = 0;

	m_TotalSegments++;
	if(the_key != INVALID_KEY){
		HC_Open_Segment_By_Key (the_key);
			
			m_TotalIncluders = HC_Show_Existence("includers");
			HC_Begin_Contents_Search (".", "includes, segment"); 
			while (HC_Find_Contents (type, &key)) {
				if (streq(type, "include") && recurse) {
					if (!pathname)
						pathname = new char [MVO_SEGMENT_PATHNAME_BUFFER];
					HC_Show_Include_Segment (key, pathname);
					HC_KEY new_key = HC_KCreate_Segment(pathname);
					new_key = HC_KShow_Original_Key (new_key);		
					ComputeSegmentContents(new_key);
				}
				else if (streq (type, "segment") && recurse) {
					ComputeSegmentContents(key);
				}
			}
			IncrementCounters ();

			HC_End_Contents_Search ();

		HC_Close_Segment ();
	}

	H_SAFE_DELETE_ARRAY(pathname);
	H_SAFE_DELETE_ARRAY(type);
}




