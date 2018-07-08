// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef _HMODELINFO_H
#define _HMODELINFO_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"

//! The HModelInfo class computes and stores model information.
/*! 
  HModelInfo encapusulates information associated with a model. It provides routines that search and compute the 
  contents of a given HOOPS sub-tree.  Classes that implement custom model information routines should be derived from 
  HModelInfo.
*/
class MVO_API HModelInfo 
{
public:
	/*! Constructs an HModelInfo object. */
	HModelInfo (HC_KEY segment_key = INVALID_KEY) {m_SegmentKey=segment_key;ResetCounters();};
	virtual ~HModelInfo () {;};

	/*!
		ComputeModelInfo performs a contents search on a Hoops sub-tree.  The search begins at the segment passed with the 
		argument list and continues into and all of the direct and included sub-segments, counting each type of geometric 
		primitive in these segments.  Geometry totals can be accessed through the HModelInfo Get routines, or manipulated 
		by derived classes. 
		\param segment_key   The key of segment where the search should start.
	*/
 	virtual void ComputeModelInfo (HC_KEY segment_key=INVALID_KEY, bool recurse=true);

	/*! \return The sum of all faces in meshes and shells plus the polygon total.  */
	unsigned long			GetTotalFaces() { return m_TotalFaces; }	
	/*! \return The sum of all vertices in meshes, each shell face and all polygons. */
	unsigned long			GetTotalVertices() { return m_TotalVertices; }
	/*! \return The result of (total_vertices/total_faces - 2) * total_faces. */
	unsigned long			GetTotalTriangles() { return m_TotalTriangles; }
	/*! \return The total tristrip count. */
	unsigned long			GetTotalTristrips() { return m_TotalTristrips; }
	/*! \return The sum of all segments in the model. */
	unsigned long			GetTotalSegments() { return m_TotalSegments; }
	/*! \return The sum of all circles in the model. */
	unsigned long			GetTotalCircle() { return m_TotalCircle; }
	/*! \return The sum of all circular arcs in the model. */
	unsigned long			GetTotalCircularArc() { return m_TotalCircularArc; }
	/*! \return The sum of all circular chords in the model. */
	unsigned long			GetTotalCircularChord() { return m_TotalCircularChord; }
	/*! \return The sum of all circular wedges in the model. */
	unsigned long			GetTotalCircularWedge() { return m_TotalCircularWedge; }
	/*! \return The sum of all ellipses in the model. */
	unsigned long			GetTotalEllipse() { return m_TotalEllipse; }
	/*! \return The sum of all elliptical arcs in the model. */
	unsigned long			GetTotalEllipticalArc() { return m_TotalEllipticalArc; }
	/*! \return The sum of all grids in the model. */
	unsigned long			GetTotalGrid() { return m_TotalGrid; }
	/*! \return The sum of all images in the model. */
	unsigned long			GetTotalImage() { return m_TotalImage; }
	/*! \return The sum of all includes in the model. */
	unsigned long			GetTotalIncludes() { return m_TotalIncludes; }
	/*! \return The sum of all includer of the model. */
	unsigned long			GetTotalIncluders() { return m_TotalIncluders; }
	/*! \return The sum of all lines in the model. */
	unsigned long			GetTotalLine() { return m_TotalLine; }
	/*! \return The sum of all markers in the model. */
	unsigned long			GetTotalMarker() { return m_TotalMarker; }
	/*! \return The sum of all meshes in the model. */
	unsigned long			GetTotalMesh() { return m_TotalMesh; }
	/*! \return The sum of all polygons in the model. */
	unsigned long			GetTotalPolygon() { return m_TotalPolygon; }
	/*! \return The sum of all polylines in the model. */
	unsigned long			GetTotalPolyline() { return m_TotalPolyline; }
	/*! \return The sum of all shells in the model. */
	unsigned long			GetTotalShell() { return m_TotalShell; }
	/*! \return The sum of all string cursors in the model. */
	unsigned long			GetTotalStringCursor() { return m_TotalStringCursor; }
	/*! \return The sum of all text strings in the model. */
	unsigned long			GetTotalText() { return m_TotalText; }
	/*! \return The sum of all modelling matrices in the model. */
	unsigned long			GetTotalMatrices() { return m_TotalMatrices; }
	/*! \return The sum of all spot lights in the model. */
	unsigned long			GetTotalSpotLight() { return m_TotalSpotLight; }
	/*! \return The sum of all distant lights in the model. */
	unsigned long			GetTotalDistantLight() { return m_TotalDistantLight; }
	/*! \return The sum of all local lights in the model. */
	unsigned long			GetTotalLocalLight() { return m_TotalLocalLight; }
	/*! \return The sum of all cutting planes in the model. */
	unsigned long			GetTotalCuttingPlane() { return m_TotalCuttingPlane; }
	/*! \return The sum of all cylinders in the model. */
	unsigned long			GetTotalCylinders() { return m_TotalCylinders; }
	/*! \return The sum of all polycylinders in the model. */
	unsigned long			GetTotalPolyCylinders() { return m_TotalPolyCylinders; }
	/*! \return The sum of all spheres in the model. */
	unsigned long			GetTotalSpheres() { return m_TotalSpheres; }
	/*! \return The sum of all NURBS curves in the model. */
	unsigned long			GetTotalNURBSCurves() { return m_TotalNURBSCurves; }
	/*! \return The sum of all NURBS surfaces in the model. */
	unsigned long			GetTotalNURBSSurfaces() { return m_TotalNURBSSurfaces; }
	/*! \return The sum of all references in the model. */
	unsigned long			GetTotalReferences() { return m_TotalReferences; }
	/*! \return The sum of all infinite lines in the model. */
	unsigned long			GetTotalInfiniteLines() {return m_TotalInfiniteLines; }
	/*! \return The sum of all infinite rays in the model. */
	unsigned long			GetTotalInfiniteRays() {return m_TotalInfiniteRays; }
	/*! \return The sum of all other items in the model (SHOULD ALWAYS BE ZERO). */
	unsigned long			GetTotalOthers() { return m_TotalOthers; }	

protected:

	/*! <b>For internal use only.</b>

		Initializes the number of primitives in a the count to zero.  Used internally by 
		ComputeModelInfo(), but can be useful in derived classes.
	*/ 
	virtual void ResetCounters();


	/*! <b>For internal use only.</b>
		Used internally by ComputeModelInfo(), but can be used in derived classes.  Increments the counters 
		for geometry of various types based on the return values of Show_Existence
	*/
	 virtual void IncrementCounters (); 

	/*!	no longer used internally, but can be used in derived classes.  
		\param item_type Type of the primitive to increment in the count (see HC_Begin_Contents_Search for types)
		\param item_key HOOPS key to any geometry.  In this case, "key" is used only for polygons, shells, and 
		meshes in order to calculate the sub-primitives and increment their numbers.
	*/
  	virtual void IncrementCounter (char * item_type, HC_KEY item_key); 
	/*! <b>For internal use only.</b>*/
	virtual void ComputeSegmentContents(HC_KEY the_key, bool recurse=true); 

	HC_KEY		m_SegmentKey;		 //!< <b>For internal use only</b>

	unsigned long			m_TotalFaces;		 //!< <b>For internal use only.</b>		
	unsigned long			m_TotalVertices;	 //!< <b>For internal use only.</b>		
	unsigned long			m_TotalTriangles;	 //!< <b>For internal use only.</b>
	unsigned long			m_TotalTristrips;	 //!< <b>For internal use only.</b>
	unsigned long			m_TotalSegments;	 //!< <b>For internal use only.</b>

	unsigned long			m_TotalCircle;		 //!< <b>For internal use only.</b>
	unsigned long			m_TotalCircularArc;	 //!< <b>For internal use only.</b>
	unsigned long			m_TotalCircularChord;	 //!< <b>For internal use only.</b>

	unsigned long			m_TotalCircularWedge;	//!< <b>For internal use only.</b>
	unsigned long			m_TotalEllipse;			//!< <b>For internal use only.</b>
	unsigned long			m_TotalEllipticalArc;	//!< <b>For internal use only.</b>
	unsigned long			m_TotalGrid;			//!< <b>For internal use only.</b>
	unsigned long			m_TotalImage;			//!< <b>For internal use only.</b>
	unsigned long			m_TotalIncludes;		//!< <b>For internal use only.</b>
	unsigned long			m_TotalIncluders;		//!< <b>For internal use only.</b>
	unsigned long			m_TotalLine;			//!< <b>For internal use only.</b>		
	unsigned long			m_TotalMarker;			//!< <b>For internal use only.</b>
	unsigned long			m_TotalMesh;			//!< <b>For internal use only.</b>
	unsigned long			m_TotalPolygon;			//!< <b>For internal use only.</b>
	unsigned long			m_TotalPolyline;		//!< <b>For internal use only.</b>
	unsigned long			m_TotalShell;			//!< <b>For internal use only.</b>
	unsigned long			m_TotalStringCursor;	//!< <b>For internal use only.</b>
	unsigned long			m_TotalText;			//!< <b>For internal use only.</b>
	unsigned long			m_TotalMatrices;		//!< <b>For internal use only.</b>
   	
   	unsigned long			m_TotalSpotLight;		//!< <b>For internal use only.</b>
   	unsigned long			m_TotalDistantLight;	//!< <b>For internal use only.</b>
	unsigned long			m_TotalLocalLight;		//!< <b>For internal use only.</b>

	unsigned long			m_TotalCuttingPlane;	//!< <b>For internal use only.</b>

	unsigned long			m_TotalPolyCylinders;	//!< <b>For internal use only.</b>
	unsigned long			m_TotalCylinders;		//!< <b>For internal use only.</b>
	unsigned long			m_TotalSpheres;			//!< <b>For internal use only.</b>
	unsigned long			m_TotalNURBSCurves;		//!< <b>For internal use only.</b>
	unsigned long			m_TotalNURBSSurfaces;	//!< <b>For internal use only.</b>
	unsigned long			m_TotalReferences;		//!< <b>For internal use only.</b>
	unsigned long			m_TotalInfiniteLines;	//!< <b>For internal use only.</b>
	unsigned long			m_TotalInfiniteRays;	//!< <b>For internal use only.</b>

	unsigned long			m_TotalOthers;			//!< <b>For internal use only.</b>

};


#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif




