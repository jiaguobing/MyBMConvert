// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

/*! \file HOpCreateNurbs3D.h
 Interface of the HOpCreateNurbs3D class
 Creates and modifies a Nurbs Surface
*/



#ifndef _OpCreateNurbs3D_H
#define _OpCreateNurbs3D_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"
#include "HBaseOperator.h"


/*! \enum HSurfaceType
	Defines the type of surface that is created by the operator
*/
enum HSurfaceType 
{
	InternalNurbsType,			//!< operator uses HOOPS nurbs function (HC_Insert_Nurbs)
	NurbsType,					//!< operator uses internal routines
	ChakinSubdivisionType		//!< operator creates surface based on chakin subdivision scheme
};


class HNurbsSurface;


//! The HOpCreateNurbs3D class provides support for the interactive construction of various surface types.
/*! 
  HOpCreateNurbs3D implements three of the mouse event handlers defined on the base class to allow the user
  to interactively modify a nurbs surface by dragging it's control points with the mouse.
   
  The operation consists of the following steps:
  <ol>
  <li>Left Button Down:				The operation is initiated and control point for dragging is specified.
  <li>Left Button Down and Drag:	The control point moves with mouse position and the surface is recalculated.
  <li>Left Button Up:				The surface editing is finalized.
  </ol>
  More Detailed Description: see event methods. 
*/
class MVO_API HOpCreateNurbs3D : public HBaseOperator
{
public:
	/*!
	  Constructs an HOpCreateNurbs3D object.
	  \param view A pointer to an HBaseView object.
	  \param DoRepeat An integer denoting whether this is a repeatable operator.  This parameter has been deprecated.
	  \param DoCapture An integer denoting whether the mouse state should be captured, which means that all mouse events should be received after a mousedown, even if it leaves the window.
	  This parameter has been deprecated.
	  */
    HOpCreateNurbs3D (HBaseView* view, int DoRepeat=0, int DoCapture=1);
	virtual ~HOpCreateNurbs3D();

	/*! 
	  Creates a new operator which is a copy of the current operator.  The user is 
	  responsible for deleting the newly created operator.
	  \return A pointer to the newly created HOpCreateNurbs3D object.
	*/
	virtual HBaseOperator * Clone();
    
	/*!
		\return A pointer to a character string denoting the name of the operator which is 'HOpCreateNurbs3D'.
	*/
	virtual const char * GetName();  
	/*!
		OnLButtonDown records the control point that will be used to manipulate the surface.
		\param hevent An HEventInfo object containing information about the current event.
		\return An #HOperatorReturn indicating the status of the event.
	*/
    virtual int OnLButtonDown(HEventInfo &hevent);

	/*!
		OnLButtonUp finalizes nurbs manipulation.
		\param hevent An HEventInfo object containing information about the current event.
		\return An #HOperatorReturn indicating the status of the event.
	*/
    virtual int OnLButtonUp(HEventInfo &hevent);

	/*!
		OnLButtonDownAndMove receives control point manipulation information in plane parallel to viewplane and 
		update the scene.
		\param hevent An HEventInfo object containing information about the current event.
		\return An #HOperatorReturn indicating the status of the event.
	*/
    virtual int OnLButtonDownAndMove(HEventInfo &hevent);
 
	 /*!
		This method insert the given surface into the scene.
		\param pView A pointer to the current view object.
		\param pNurbsSurface A pointer to the nurbs surface object.
	    \param emit_message  Indicates that a message should be sent to the emit_message_function, which (if implemented)
						   would typically use the HOOPS/Net 'Send' function to dispatch the message
						   to clients
	*/
	static HC_KEY InsertNurbs(HBaseView *pView, HNurbsSurface *pNurbsSurface, bool emit_message);


	/*!
		 This method has been deprecated.  This method inserts the surface into the scene based on the HNet message.
		 \param in_data A character pointer containing the surface data.
		 \param data_length The size of the message.
		\param pView A pointer to the current view object.
	*/
  	static void InsertNurbsFromMessage(const char *in_data, unsigned int data_length, HBaseView *m_pView);

	/*!
		This method sends a modify control point message to other clients.
		\param pView A pointer to the current view object.
		\param key  The surface key.
		\param control_point The number identifying the manipulated control point of surface.
 		\param x The new control point x position.
		\param y The new control point y position.
		\param z The new control point z position.
	*/
	static void EmitModifyControlPointMessage(HBaseView *pView, HC_KEY key, int control_point, float x, float y, float z);

	/*!
		This method receives a modify control points based on an HNet message.
		\param in_data A character pointer containing the new control point data.
		\param data_length The size of the message.
 		\param pView  A pointer to the current view object. 
	*/	
	static void ModifyControlPointFromMessage(const char *in_data, unsigned int data_length, HBaseView *pView);

	/*!
    	This method emits the finalized Nurbs editing message to other clients.
		 \param pView  A pointer to the current view object. 
    	 \param key  The surface key.
	     \param emit_message  Indicates that a message should be sent to the emit_message_function, which (if implemented)
 						   would typically use the HOOPS/Net 'Send' function to dispatch the message
						   to clients
	*/
	static void EmitFinishNurbsEditing(HBaseView *pView, HC_KEY key, bool emit_message);
 
	/*!
		This method receives the Finish Nurbs editing message.
		\param in_data A character pointer indicating the end of surface editing.
		\param data_length The size of the message.
 		\param pView  A pointer to the current view object. 
	*/
	static void FinishNurbsEditingFromMessage(const char *in_data, unsigned int data_length, HBaseView *pView);


protected:

private:
	HPoint m_last_point;
	int		m_mnnum;
	float	m_gwidth;
	int		m_gran;
	int		m_degree;
	HC_KEY	m_NurbsKey;
	HNurbsSurface *m_pNurbsSurface;

	
 
};


//! The HNurbsSurface class encapsulates a NURBS surface or Chaking subdivision surface.
/*! 
	The surface can either be created by passing the necessary parameters 
	like degree, control points, tessellation level or implicitly from an 
	existing nurbs segment in the hoops segment tree.  It is internally 
	represented as a HOOPS mesh as described in #HC_Insert_NURBS_Surface and a second invisible mesh representing 
	the control points themselves.
*/
class MVO_API HNurbsSurface
{
public:

	/*!Constructs a HNurbsSurface.*/
	HNurbsSurface()
	{
		m_nurbsKey = 0;
		m_numUVertices = 0;
		m_numVVertices = 0;
		m_pVertices = 0;
		m_pNurbs = 0;
		m_degree = 0;
		m_tesselationLevel = 0;
		m_pNurbs = 0;
		m_pVertices = 0;
		m_vertexDistance = 0;
		m_verticesKey = 0;
		m_surfaceMeshKey = 0;

	}
	~HNurbsSurface()
	{
		H_SAFE_DELETE_ARRAY(m_pVertices);
		H_SAFE_DELETE_ARRAY(m_pNurbs);
	}

	/*!
		This method initializes the Nurbs object based on existing nurbs segment in HOOPS.
		\param key  The key to the nurbs segment in HOOPS.
	*/
	void Init(HC_KEY key);

	/*!
		This method initialize the Nurbs object based on given parameters.
		\param  center  The location of the center of creation.
		\param  axis The initial axis of rotation.
 		\param	num_u_vertices  The number of control points in the u direction.
		\param	num_v_vertices  The number of control points in the v direction.
		\param	degree	The degree of the surface.
		\param	tesselation_level  The tesselation density.
		\param	distance The distance of vertices.  This parameter is used during tesselation.
		\param	stype Pass which type of surface you want to create.  It can either be either NURBS or Chakin.

	*/
 	HC_KEY Init( HPoint * center, HPoint * axis, 
			int num_u_vertices, int num_v_vertices, 
			int degree, int tesselation_level, 
			float distance, HSurfaceType stype);

	/*!
		This method recalculates the surface based on the existing control points.
 	*/	
	void Recalculate();
 
	/*!
 		\return The NURBS surface key.
	*/
	HC_KEY GetKey();

	
	/*!
		This method shows the mesh containing the control points for the surface.
 	*/	
	void DisplayVertices(); 

	/*!
		This method hides the control point mesh.
	*/
	void HideVertices();

	/*!
		This method enables the selectability of the NURBS surface.
	*/
	void EnableSelectability();

	/*!
		This method disables the selectability of NURBS Surface.
	*/
	void DisableSelectability();

	 /*!
		This method allows you to modify a specific control point.
		\param vertexnum  The control point index number in the m_pVertices list.
		\param x The new x coordinate.
		\param y The new y coordinate.
		\param z The new z coordinate.
	*/
	void UpdateVertex(int vertexnum, float x, float y, float z);

	/*!
		This method returns the location of a control point given a specific index identifier.
		\param vertexnum  The control point index number in the m_pVertices list.
		\param v Reference to HPoint structure.  This method returns the control poínt position.
	*/
	void ShowVertex(int vertexnum, HPoint &v);
	
	/*!
		This method creates a set of default control points given a center point and an axis of rotation.
		\param center The center point for creation.
		\param axis An initial axis of rotation.
	*/
	void CreateDefaultVertices(HPoint * center, HPoint * axis);

	/*! \return The key to the tesselated mesh. */
	HC_KEY GetSurfaceMeshKey() { return m_surfaceMeshKey; }
 
private:
  
	HC_KEY m_nurbsKey;
	HC_KEY m_verticesKey;
	HC_KEY m_surfaceMeshKey;
	int m_numUVertices;
	int m_numVVertices;
	int m_degree;
	int m_tesselationLevel;
	float m_vertexDistance;
	HPoint *m_pVertices;
	HPoint *m_pNurbs;
	HSurfaceType m_surfaceType;
	

	void UpdateVertices(); 
	void UpdateParameters();
	float nip(int i,int p, float u, float *uv);
	void InsertNurbsMesh(HPoint *pNurbs);
	void InsertChakinSubdivionSurface(HPoint *pNurbs);
	void CalculateCornerPoint(HPoint &p1, HPoint &p2, HPoint &res, float factor); 
	void SubdivideSurface(HPoint *old_surface, HPoint *ssurf, int xlen, int ylen, int &new_xlen, int &new_ylen, bool xrefine);

	void GetVerticesFromHOOPS();
 	void GetParametersFromHOOPS();

};
 


#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif




