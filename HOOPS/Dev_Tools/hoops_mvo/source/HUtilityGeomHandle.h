// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

/*! \file HUtilityGeomHandle.h
 Interface of the HUtilityGeomHandle class
*/
#ifndef _HUTILITYGEOMHANDLE_H
#define _HUTILITYGEOMHANDLE_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"
#include "HUtilityAnnotation.h"
#include "hic_types.h"

class HPoint;
class HBaseView;


/*! \enum HConstrainType
	Defines the type of constraint that is applied to each handle.
*/
enum HConstrainType
{
	HConstrainNone,					//!< No constraint.
	HConstrainToRay,				//!< Constrain to ray given by two points.
	HConstrainToRotationVector,		//!< Constrain to specific rotation vector. 
	HConstrainToPlane,				//!< Constrain to plane.
	HConstrainToViewplane,			//!< Constrain to viewplane.
	HConstrainToScale,				//!< Constrain to Scale factor.
	HConstrainToOnOff				//!< Constrain to on/off by click.
};



//! The HUtilityGeomHandle class contains a set of functions which controls the creation and manipulation of geometry handles
/*! 
  "Handles" can be attached to any geometry in the HOOPS segment tree.  They can be used for dragging, rotating or scaling 
  of objects. By specifying constraints, it is possible to limit their movement to a given ray or plane.

  "Handles" are created as subsegments of any segment that contains geometry that needs to be manipulated. Multiple 
  handles can be created fot the same geometry.
 
*/
class MVO_API HUtilityGeomHandle 
{
public:


	virtual ~HUtilityGeomHandle() { };

    //!  Registers various callbacks needed for this class.
	static  void RegisterCallbacks();

	/*! Creates a new handle underneath the currently open segment.
		\param position The coordinates where the handle should be created.
		\param edges Pass true to display edges for handle arrows or false not to show them.
		\param level Modelling Matrix level for handles.
		\return The key of the newly created handle segment.
 	*/
	virtual HC_KEY Insert(HPoint position, bool edges = false, int level = 4);

	/*! This method sets default attributes for the handle segment which includes turning LOD off and backplane culling.
 		\param edges Pass true to display edges for handle arrows.
 	*/
  	virtual void SetupGeomHandleSegment(bool edges);

	/*! This method constrains handle movement to the given plane.
		\param handlekey The key to the handle segment.
		\param p1 The first point which defines the plane.
		\param p2 The second point which defines the plane.
		\param p3 The third point which defines the plane.
 
 	*/
	virtual void ConstrainToPlane(HC_KEY handlekey, HPoint p1, HPoint p2, HPoint p3,HBaseView *view = 0);

	/*! This method contrains the given handle's movement to the view plane.
		\param handlekey The key to the handle segment.
 	*/
 	virtual void ConstrainToViewplane(HC_KEY handlekey,HBaseView *view = 0);

	/*! This method constrains the given handle's movement to the given ray.
		\param handlekey The key to the handle segment.
		\param p1 The first point which defines the ray.
		\param p2 The second point which defines the ray.
 	*/
	virtual void ConstrainToRay(HC_KEY handlekey, HPoint p1, HPoint p2, HBaseView *view = 0);

	/*! This method constrains the given handle's rotation to the passed rotation vector.
		\param handlekey The key to the handle segment.
		\param p1 The first point which defines the rotation vector.
		\param p2 The second point which defines the rotation vector.
 	*/
	virtual void ConstrainToRotationVector(HC_KEY handlekey, HPoint p1, HPoint p2,HBaseView *view = 0);

	/*! The method contrains the given handle to the given scale.
		\param handlekey The key to the handle segment.
		\param p1 The center for scaling. 
		\param p2 The initial scaling value along the three axis.  Pass 1.0 to indicate no scaling.
 	*/
	virtual void ConstrainToScale(HC_KEY handlekey, HPoint p1, HPoint p2,HBaseView *view = 0);
	
	/*! This method constrains the given handle to delete associated geometry
		\param handlekey The key to the handle segment.  
 	*/
	virtual void ConstrainToOnOff(HC_KEY handlekey,HBaseView *view = 0);

	/*! This method moves the handle position.  Note this affects the associated geometry.
		\param handlekey The key to the handle segment.
		\param newposition The coordinates of the new handle position.
 		\param camerakey The key to the camera segment.
 		\param view A pointer to the HBaseView object.
 	*/
	virtual void Move(HC_KEY handlekey, HPoint newposition, HC_KEY camerakey, HBaseView *view = 0);


	/*! This method shows the initial position of the given handle relative to segment attached to the handle. 
		\param handlekey The key to the handle segment.
		\param bp Returns the coordinate of the initial handle position.   
 	*/
 	virtual void ShowBasePosition(HC_KEY handlekey, HPoint &bp);

	/*! This method restores the constraint parameters for a given handle from user options to the class members so that 
	they can be used for calculations like rotate.
		\param handlekey key to handle segment
 	*/
	virtual void GetConstraintParameters(HC_KEY handlekey);

	/*! This method applies a given rotation angle to the associated geometry of the handle.
		\param handlekey The key to the handle segment.
		\param rotangle The angle of rotation in degrees.    
 	*/
	void Rotate(HC_KEY handlekey, float rotangle);

	/*! This method calculates the correct z value corresponding to the handle plane for a given point.  
	Note that you must pass a segment with camera attributes to be open.
		\param handlekey The key to the handle segment.
		\param point The point for which z value needs to be calculated
		\return The z value on the handle plane.
 	*/
	virtual float CalculatePlaneZ(HC_KEY handlekey, HPoint point);

 
	/*! This method adjusts the given point <i>C</i> to lie on the ray defined by points <i>A</i> and <i>B</i>.
		\param handlekey The key to the handle segment.
		\param C The point to be adjusted.
		\param A The first point that define the ray.
		\param B The second point that defines the ray.
 	*/
 	virtual void AdjustPositionToRay(HC_KEY handlekey, HPoint &C, HPoint A, HPoint B);

	/*! This method projects the given point <i>C</i> to a given plane described via the parameters of a planar 
	equation ax + by + cz + d = 0.
		\param handlekey The key to the handle segment.
		\param p The point to be adjusted.
		\param a The first parameter that defines the plane.
		\param b The second parameter that defines the plane.
		\param c The third parameter that defines the plane.
		\param d The fourth parameter that defines the plane.
  	*/
	virtual void CalculatePositionOnPlane(HC_KEY handlekey, HPoint &p, float a, float b, float c, float d);
	

		/*! This method adjusts the given point <i>p</i> to lie to the ray that constrains the given handles.
  		\param geomhandlekey The key to the handle segment.
 		\param p The point to be adjusted. When the method returned, p will be populated with the newly adjusted values.
		\param camerasegment The key to the associated camera segment.
		\param view A pointer to the HBaseView object.
 		*/	
	void AdjustPointToRay(HC_KEY geomhandlekey, HPoint &p, HC_KEY camerasegment, HBaseView *view);
	
	/*! This method adjusts point <i>p</i> to lie on a plane(described by the planar equation ax + by + cz + d = 0) by 
	constructing a ray defined by the camera position and the point itself.
		\param handlekey The key to the handle segment.
		\param p The point to be adjusted.
		\param a The first parameter that defines the plane.
		\param b The second parameter that defines the plane.
		\param c The third parameter that defines the plane.
		\param d The fourth parameter that defines the plane.
  		\param camerakey key to segment with desired reference camera
	*/
	virtual void AdjustPositionToPlane(HC_KEY handlekey, HPoint &p, float a, float b, float c, float d, HC_KEY camerakey);
	
	/*! This method identifies if the given segment has handles. 
		\param key The key to the segment.
		\return The original key that was passed if there are handles or INVALID_KEY if there are no handles.
	*/
	static  HC_KEY IsGeomHandle(HC_KEY key);


	/*! This method identifies if the given segment or any of it's subsegments contains handles.
		\param key The key to the segment.
		\return True if there is handle in segment or subsegments or false if there are no handles.
	*/
	static bool ContainsGeomHandles(HC_KEY key);


	/*! This method calculates the handle position after being combined with the world modelling transformation.
		\param handlekey The key to the handle segment.
		\param t The handle position to be transformed.
		\param view A pointer to HBaseView object.
 	*/
	virtual void CalculateTransformedPosition(HC_KEY handlekey, HPoint &t, HBaseView *view);

	/*! This method gets the constraint type for the given handle.
		\param handlekey The key to handle segment.
		\return The constraint type of the given handle.  
 	*/
	HConstrainType GetConstraintType(HC_KEY handlekey ) { GetConstraintParameters(handlekey); return m_ConstraintType; }

	/*! This method shows the given handle with the passed scale factor and with a symbol based on constraint type.
		\param handlekey The key to handle segment.
 		\param sf The scale factor for the handle.
 	*/
	virtual void ShowHandle(HC_KEY handlekey, float sf = 0.7f,HBaseView *view = 0);

	/*! This method shows the given handle with default values.
		\param handlekey The key to the handle segment.
		\param scale The scale factor for the handle.
 	*/
	virtual void ResetHandle(HC_KEY handlekey, float scale = 0.5f, HBaseView *view = 0);

	/*! This method applies a scale matrix to the geometry associated with the given handle.
		\param handlekey The key to the handle segment.
		\param scalex The scale factor in x direction.
		\param scaley The scale factor in y direction.
		\param scalez The scale factor in z direction.
 	*/
	virtual void Scale(HC_KEY handlekey, float scalex, float scaley, float scalez);

	/*! This method delete the geometry associated with the handle.
		\param handlekey The key to the handle segment.
 	*/
	virtual void TurnOff(HC_KEY handlekey);


	/*! This method gets the rotation axis for a handle with a rotational constraint.
		\param handlekey The key to the handle segment.
		\return A pointer to array defining the ray.
 	*/
	virtual HPoint * GetRotationAxis(HC_KEY handlekey) {	GetConstraintParameters(handlekey);  return m_rpoint; }

	/*! This method delete all the handles from the current HBaseView.
		\param view A pointer to the view object.
  	*/
	static void ClearAllHandles(HBaseView *view);

	/*! This method finds the handle that is closest to the given screen position.
		\param view A pointer to the view object.
		\param pos The screen position.
		\return The key of the closest handle or INVALID_KEY if no handle was found.
  	*/
	static HC_KEY SelectHandle(HBaseView *view, HPoint pos);
	
	/*! This method finds an object to attach handles to based on the given screen position.
		\param view A pointer to view object.
		\param pos The screen position.
		\param oldkey The key to the currently active object.
		\param exclude Pass true to exclude currently active object from the search or false if you want to include it in the search.
		\param proximity The selection proximity to be used.  Pass a negative number for "no selection proximity"
		\return The key of the object that was found or INVALID_KEY if no object was found.
  	*/
	static HC_KEY SelectObject(HBaseView *view, HPoint pos, HC_KEY oldkey, bool exclude, float proximity=0.3);

	/*! This method attaches default handles to a given object.
		\param key The key to the segment to attach handles to.
		\param view A pointer to the view object.
		\param boxzoom The default scale factor for the object's bounding box.
   	*/
	static void SetupObject(HC_KEY key, HBaseView *view, float boxzoom = 1.0f);
	
	/*! This method attaches default handles to a given text object.
		\param key The key to the segment to attach handles to.
		\param view A pointer to the view object.
		\param boxzoom The default scale factor for the object's bounding box.
		\param scale  The text scale factor.
   	*/	
	static void SetupTextObject(HC_KEY key, HBaseView *view, float boxzoom = 1.0f, float scale = 1.0f);

	/*! This method has been deprecated.  Please use #SetupCuttingPlaneManipulator instead.
		\param key The cutting plane segment to attach handles to.
		\param view A pointer to the view object.
   	*/
	static void SetupCuttingPlane(HC_KEY key, HBaseView *view);

	/*! This method attaches handles with the specified attributes to the given object.
		\param startkey The segment key to attach handles to.
		\param view A pointer to the view object.
		\param mp The midpoint for the handles.
		\param AddNonUniformScale Pass true to create a handle of this type.
		\param AddScale Pass true to create a handle of this type.
		\param AddPlane Pass true to create a handle of this type.
		\param AddAxismove Pass true to create a handle of this type.
		\param AddAxisrotate Pass true to create a handle of this type.
		\param AddViewplaneMove Pass true to create a handle of this type.
		\return The top level key that contains all the newly created handles.
   	*/	
 	static HC_KEY SetupManipulator(HC_KEY startkey, HBaseView *view, HPoint *mp, bool AddNonUniformScale = true, bool AddScale = true, bool AddPlane = true, bool AddAxismove = true, bool AddAxisrotate = true, bool AddViewplaneMove = true);
	
	/*! This method attaches default handles to a cutting plane object.
		\param startkey The cutting plane segment to attach handles to.
		\param view A pointer to the view object.
   	*/
	static void SetupCuttingPlaneManipulator(HC_KEY startkey, HBaseView *view, HPoint *midpoint = 0, bool omit_rotate = false);

	/*! This method finds a key to attached a modelling matrix segment for a given handle.
		\param handlekey A key to the handle.
		\return The segment key containing modelling matrix.
   	*/
	static HC_KEY GetMMSegment(HC_KEY handlekey);

	/*! This method finds the center point of the object (in its local space) attached to the given handle.
		\param objectkey The object attached to the handle.
		\param centerpoint Returns a center point of the object.
   	*/
	static void GetCenterPoint(HC_KEY objectkey, HPoint &centerpoint);
	/*!
		This method sets the visibility for all handles. 
 		\param startkey The start key to search for handles.
 		\param visibility The visibility setting as described in #HC_Set_Visibility.
 	*/	
	static void SetVisibilityForAll(HC_KEY startkey, const char *visibility);

private:
	static void drawTriangleCallback (HIC_Rendition *rendition, HIC_DC_Point *p);
  	 
	static void FindTranslation(HC_KEY handlekey, HPoint &trans);
	static int GetMMLevel(HC_KEY handlekey);
 
 	HC_KEY m_GeomHandleKey;
	HConstrainType m_ConstraintType;
	HPoint m_rpoint[3];	

};

#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif



