// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// Note: this class is still in development

#include <math.h>
#include <string.h>

#include "HTools.h"

#include "HBaseModel.h"
#include "HBaseView.h"
#include "HBaseOperator.h"
#include "HEventInfo.h"
#include "HOpObjectRotate.h"
#include "HSelectionSet.h"
#include "HSelectionItem.h"
#include "HUtilityGeomHandle.h"
#include "HUndoManager.h"

/////////////////////////////////////////////////////////////////////////////
// HOpObjectRotate
//
// Translate items in the selection list in direction of mouse movement
// parallel to camera target plane.
//
// Currently supports rotation if item if it is a segment
//
// This routine implements a virtual trackball.  The trackball
// is a virtual hemisphere of radius 1.0.  The hemisphere is 
// centered on the object being rotated. Each mouse position is
// mapped to the hemisphere and the cross product of sub-sequent
// mapped mouse vectors determines the axis about which the
// object is rotated. (The cross product vector is projected into
// object space and the call Rotate_Object_Offaxis is used)
// Mouse vectors that lie outside the hemisphere are mapped to the 
// edge of the hemisphere.  If two mouse vectors lie out of the 
// hemisphere, the resultant cross product is parallel to the 
// world space z-axis and the object appears to spin.



void HOpObjectRotate::RotateSegmentLocally (
	HC_KEY		key,
	HPoint		*rotate_object,
	float		theta)	
{
	bool		must_append = false;
	float		matrix[4][4];
	bool		is_segment = false;
	char		type[MVO_BUFFER_SIZE];

	HC_Show_Key_Type(key, type);
	if(strcmp(type, "segment") == 0)
		is_segment = true;

	//  Rotate interactive segment locally around line parallel to given axis
	//  that contains the segment's centroid.  This requires preconcatenation
	//  of an off-center/off-axis rotation.  This is acheived in five steps
	//
	//  UnSet the local modelling matrix (if there is one).
	//  Translate the Segment so that its centroid is at the origin.
	//  Apply offaxis rotation.
	//  Translate the centroid back to its original point.
	//  Put the local modelling matrix back (if we Unset it).
	//

	if(is_segment)
		HC_Open_Segment_By_Key(key);
	else 
		HC_Open_Geometry(key);
		// Remove local modelling matrix if there is one. Store it.
		if (HC_Show_Existence ("modelling matrix")) 
		{
			HC_Show_Modelling_Matrix (&matrix[0][0]);

			//save original matrix, and add to undo manager
			if(!m_bInitialMatrix && is_segment)
			{
				m_bInitialMatrix = true;
				HUndoItemMatrix *item = new HUndoItemMatrix(GetView(), 1, &key, &matrix[0][0]);
				GetView()->GetUndoManager()->AddUndoItem(item);
			}

			HC_UnSet_Modelling_Matrix ();
			must_append = true;
		}
		else 
		{
			if(!m_bInitialMatrix && is_segment)
			{
				m_bInitialMatrix = true;
				float matrix[16];
				memset(matrix,0,16*sizeof(float));
				matrix[0] = matrix[5] = matrix[10] = matrix[15] = 1;
				HUndoItemMatrix *item = new HUndoItemMatrix(GetView(), 1, &key, matrix);
				GetView()->GetUndoManager()->AddUndoItem(item);
			}
		}

		//  Translate centroid of object to origin 
		HC_Translate_Object (- m_ptCentroid.x, - m_ptCentroid.y, - m_ptCentroid.z);

		// Apply off-axis rotation. 
		HC_Rotate_Object_Offaxis (rotate_object->x, rotate_object->y, rotate_object->z, theta);

		// Put centroid back where it was 
		HC_Translate_Object (m_ptCentroid.x, m_ptCentroid.y, m_ptCentroid.z);

		//  Put any local transformations back. 
		if (must_append) 
			HC_Append_Modelling_Matrix (&matrix[0][0]);
	if(is_segment)
		HC_Close_Segment ();
	else
		HC_Close_Geometry();

	return;
}

HOpObjectRotate::HOpObjectRotate(HBaseView* view, int DoRepeat,int DoCapture) : HBaseOperator(view, DoRepeat, DoCapture)
{
	m_bTemporarySelection = false;
	m_bInitialMatrix = false;
}

HOpObjectRotate::~HOpObjectRotate()
{
}


HBaseOperator * HOpObjectRotate::Clone()
{
	return new HOpObjectRotate(GetView());
}



const char * HOpObjectRotate::GetName() { return "HOpObjectRotate"; }



/////////////////////////////////////////////////////////////////////////////
// HOrbitHandler message handlers



int HOpObjectRotate::OnLButtonDown(HEventInfo &event)
{
	// Mouse went down 
	HC_KEY		key;
	int			n = 0, i;
	char		keytype[MVO_BUFFER_SIZE], token[MVO_BUFFER_SIZE], tempstr[MVO_BUFFER_SIZE];
	char		pathname[MVO_SEGMENT_PATHNAME_BUFFER], owner[MVO_SEGMENT_PATHNAME_BUFFER];
	float		length;
	HPoint		min, max;

	if (!OperatorStarted()) 
		SetOperatorStarted(true);


	HSelectionSet* selection = GetView()->GetSelection();
	int size = selection->GetSize();
	if (!size)
	{
		HPoint windowpos = event.GetMouseWindowPos();
		HUtility::SmartSelectUnderCursor(GetView(), windowpos.x, windowpos.y);
		GetView()->ForceUpdate();

		size = selection->GetSize();
		m_bTemporarySelection = true;
	}
	if( !GetView()->GetAllowInteractiveShadows() && (m_TempShadowMode = GetView()->GetShadowMode()) != HShadowNone )
		GetView()->SetShadowMode(HShadowNone);
	if( !GetView()->GetAllowInteractiveCutGeometry() && (m_TempCutGeometryVisibility = GetView()->GetCutGeometryVisibility()) != HCutGeometryVisibilityNone)
		GetView()->SetCutGeometryVisibility(HCutGeometryVisibilityNone);

	SetNewPoint(event.GetMouseViewpointPos());

	// DeInstance the selection item because we are going to make 
	// change to its position and don't want it to be across all the includes
	for(i=0; i<size; ++i)
		selection->DeInstance(i);
	
	if (size == 1)
	{
		key = selection->GetAt(n);
		HC_Show_Key_Type(key, keytype);

		if ((key != INVALID_KEY) && (streq(keytype, "segment")))
		{
			float matrix[16];
			

			HC_Show_Segment(key, pathname);
			HC_Parse_String(pathname, "/", -1, token); 

			if (streq(token, "cutting grids"))
			{
				HC_Show_Owner(pathname, owner);
				HC_Open_Segment(owner);
			}
			else {
				HC_Open_Segment_By_Key(key);
					if ((GetView()->GetSpritingMode() || selection->GetHighlightMode()==HighlightQuickmoves) && 
						selection->GetHighlightMode() != ColoredInverseTransparency &&
						selection->GetHighlightMode() != InverseTransparency)
						HC_Set_Heuristics("quick moves = spriting");

					HC_Set_Rendering_Options ("simple shadow=auto");

					HC_Show_System_Options (tempstr);
					if (strstr (tempstr, "no bounding volumes"))
						HC_Compute_Circumcuboid (".", &min, &max);
					else
						HC_Show_Bounding_Cuboid (&min, &max);

					// centroid is in object space units 
					m_ptCentroid.x = 0.5F * (min.x + max.x);
					m_ptCentroid.y = 0.5F * (min.y + max.y);
					m_ptCentroid.z = 0.5F * (min.z + max.z);

					HC_Open_Segment ("bbox");
						HC_Set_Line_Weight(2.0);
						HC_Set_Color("lines=yellow");
						HC_Set_Selectability("off");
						HC_Set_Rendering_Options("attribute lock = (visibility, color)");
						HC_Set_Visibility("faces = off, markers = off, lights = off, cutting planes = off, edges = off, lines = on");
						HUtility::InsertWireframeBox (&max, &min);
					HC_Close_Segment ();			  

					HC_Show_Net_Modelling_Matrix(matrix);

					max.x -= m_ptCentroid.x;
					max.y -= m_ptCentroid.y;
					max.z -= m_ptCentroid.z;
					m_radius = (float)HC_Compute_Vector_Length (&max);

					// open up a temporary segment in which to set the net mod matrix found in the
					// segment containing the selected object.  This is necessary if it happens to be
					// a segment in the include library, so that Compute_Coordinates can have a display
					// driver as context
					HC_Open_Segment_By_Key(GetView()->GetSceneKey());

					  float     width, height;

					  HC_Show_Net_Camera_Field (&width, &height);

					  if (m_radius > 0.5f*width)
						  m_radius = 0.5f*width;
					  else if (m_radius < 0.1f*width)
						  m_radius = 0.1f*width;


					  HC_Open_Segment("dummy");
						HC_Set_Modelling_Matrix(matrix);
						
						// convert the centroid from object space to world space as this may be 
						// needed by derived class 
						HC_Compute_Coordinates (".", "object", &m_ptCentroid, "world", &m_ptCentroidWorld);

						// viewpoint version for trackball calculations
						HC_Compute_Coordinates (".", "object", &m_ptCentroid, "viewpoint", &m_ptCentroidViewpoint);
					  HC_Close_Segment();
					HC_Close_Segment();

				HC_Close_Segment();
			}
		}
	}

	HPoint new_point = GetNewPoint();
	// center the rotation hemisphere on object 
	new_point.x -= m_ptCentroidViewpoint.x;
	new_point.y -= m_ptCentroidViewpoint.y;
	new_point.z = 0.0F;

	new_point.x /= m_radius;
	new_point.y /= m_radius;

	// map the mouse position onto the hemisphere 
	length = (float)(sqrt (new_point.x * new_point.x +
				new_point.y * new_point.y));

	if (length > 1.0)
		HC_Compute_Normalized_Vector (&new_point, &new_point);
	else
		new_point.z = (float)(-sqrt (1.0 - length * length));


	SetNewPoint(new_point);
	SetFirstPoint(new_point);

	GetView()->Update();
	return HOP_OK;	
}



int HOpObjectRotate::OnLButtonDownAndMove(HEventInfo &event)
{
	float		length;
	HPoint		axis_of_rotation;
	float		angle;
	double 		vl;
	HC_KEY		key;
	int			n = 0;
	char		keytype[MVO_BUFFER_SIZE], token[MVO_BUFFER_SIZE], pathname[MVO_SEGMENT_PATHNAME_BUFFER];

	HSelectionSet* selection = GetView()->GetSelection();
	int size = selection->GetSize();

	if (!OperatorStarted()) return HBaseOperator::OnLButtonDownAndMove(event);

	SetNewPoint(event.GetMouseViewpointPos());

	if (size == 1)
	{
		key = selection->GetAt(n);
		HC_Show_Key_Type(key, keytype);

		if ((key != 0) && (streq(keytype, "segment")))
		{
			HC_Show_Segment(key, pathname);
			HC_Parse_String(pathname, "/", -1, token); 

			if (streq(token, "cutting grids"))
			{
				key = HC_KShow_Owner_By_Key(key);
			}

			HC_Open_Segment_By_Key(key);
				// center the rotation hemisphere on object   
				HPoint new_point = GetNewPoint();
				// center the rotation hemisphere on object 
				new_point.x -= m_ptCentroidViewpoint.x;
				new_point.y -= m_ptCentroidViewpoint.y;
				new_point.z = 0.0F;

				new_point.x /= m_radius;
				new_point.y /= m_radius;
				// map the mouse position onto the hemisphere 
				length = (float)(sqrt (new_point.x * new_point.x +
							new_point.y * new_point.y));

				if (length  > 1.0)
					HC_Compute_Normalized_Vector (&new_point, &new_point);
				else
					new_point.z = (float)(-sqrt (1.0 - length * length));

				SetNewPoint(new_point);

				HPoint first_point = GetFirstPoint();
				HC_Compute_Cross_Product (&first_point, &new_point, &axis_of_rotation);

				vl = (float)(sqrt (axis_of_rotation.x * axis_of_rotation.x +
						   axis_of_rotation.y * axis_of_rotation.y +
						   axis_of_rotation.z * axis_of_rotation.z));

				angle = 180.0F/3.1415926F * (float)(H_ASIN (vl));

				if (angle != 0.0)
				{
					HPoint origin, rotation;
					float matrix[16];

					origin.x = 0.0F;
					origin.y = 0.0F;
					origin.z = 0.0F;

					HC_Compute_Normalized_Vector (&axis_of_rotation, &axis_of_rotation);

					HC_Show_Net_Modelling_Matrix(matrix);

					// open up a temporary segment in which to set the net mod matrix found in the
					// segment containing the selected object.  This is necessary if it happens to be
					// a segment in the include library, so that Compute_Coord can have a display
					// driver as context
					HC_Open_Segment_By_Key(GetView()->GetSceneKey());
					  HC_Open_Segment("dummy");
						HC_Set_Modelling_Matrix(matrix);							
						HC_Compute_Coordinates (".", "viewpoint", &axis_of_rotation, "object", &axis_of_rotation);
						HC_Compute_Coordinates (".", "viewpoint", &origin, "object", &origin);
					  HC_Close_Segment();
					HC_Close_Segment();
		
					rotation.x = axis_of_rotation.x - origin.x;
					rotation.y = axis_of_rotation.y - origin.y;
					rotation.z = axis_of_rotation.z - origin.z;
		
					RotateSegmentLocally (key, &rotation, angle);	
				}
			HC_Close_Segment();
		}
	}

	SetFirstPoint(GetNewPoint());

	GetView()->SetGeometryChanged();
	GetView()->Update();
	return HOP_OK;
}



int HOpObjectRotate::OnLButtonUp(HEventInfo &event)
{
	HC_KEY		key;
	int			n = 0;
	char		keytype[MVO_BUFFER_SIZE], token[MVO_BUFFER_SIZE];
	char		pathname[MVO_SEGMENT_PATHNAME_BUFFER], owner[MVO_SEGMENT_PATHNAME_BUFFER];

	HSelectionSet* selection = GetView()->GetSelection();
	int size = selection->GetSize();

	if (!OperatorStarted())
		return HBaseOperator::OnLButtonUp(event);

	if( !GetView()->GetAllowInteractiveCutGeometry() && m_TempCutGeometryVisibility != HCutGeometryVisibilityNone)
		GetView()->SetCutGeometryVisibility(m_TempCutGeometryVisibility);
	/* Drawing the shadows and cut geometry over again takes a long time, lets take them one at a time... */ 
	GetView()->ForceUpdate();

	if( !GetView()->GetAllowInteractiveShadows() && m_TempShadowMode != HShadowNone ) {
		GetView()->SetShadowMode(m_TempShadowMode);
		// HOOPS would not normally regenerate shadows for a spriting segment.  The following
		// line tells it to do so.
		HC_Control_Update_By_Key( GetView()->GetViewKey(), "set derived geometry regeneration" );
	}

	SetNewPoint(event.GetMouseWindowPos());
 
	if (size == 1)
	{
		key = selection->GetAt(n);
		HC_Show_Key_Type(key, keytype);

		if ((key != 0) && (streq(keytype, "segment")))
		{
			HC_Show_Segment(key, pathname);
			HC_Parse_String(pathname, "/", -1, token); 

			if (streq(token, "cutting grids"))
			{
				HC_Show_Owner(pathname, owner);
				HC_Open_Segment(owner);
			}
			else
				HC_Open_Segment_By_Key(key);

					if (HUtility::ShowOneHeuristic("quick moves")) {
						HC_UnSet_One_Heuristic ("quick moves");
					}

					HC_Delete_Segment("bbox");
				HC_Control_Update(".", "compute bounding volume");

				HC_Close_Segment();
		}
	}

	if (m_bTemporarySelection)
	{
		 GetView()->GetSelection()->DeSelectAll();
		 m_bTemporarySelection = false;
	}

	SetOperatorStarted(false);
	m_bInitialMatrix = false;

	GetView()->SetGeometryChanged();
	GetView()->EmitSetModellingMatrixMessage();
	GetView()->Update();
	return HOP_READY;
}


int HOpObjectRotate::OnLButtonDblClk (HEventInfo &hevent)
{
	int n, i;
	char keytype[MVO_BUFFER_SIZE];
	HUndoItem *item;
	HUndoManager *umanager = GetView()->GetUndoManager();
	
	HOpObjectRotate::OnLButtonDown(hevent);

	HSelectionSet* selection = GetView()->GetSelection();
	int size = selection->GetSize();

	int actualkeycount = 0;
	int selkeycount = 0;
	HC_KEY *actualkeys = new HC_KEY[size];
	HC_KEY *selkeys = new HC_KEY[size];
	float *oldmatrices = new float[size * 16];
	float *matrices = new float[size * 16];

	//build a list of all the valid keys
	for (n = 0; n < size; n++)
	{
		HC_KEY sel_key = selection->GetAt(n);

		HC_Show_Key_Type(sel_key, keytype);
		if ((sel_key != INVALID_KEY) && (streq(keytype, "segment"))) {
			while( IsSpecialKey( sel_key ) )
				sel_key = HC_KShow_Owner_By_Key( sel_key );
	
			HC_Open_Segment_By_Key(sel_key);	
				if(HC_Show_Existence("modelling matrix"))
				{
					HC_Show_Modelling_Matrix(&oldmatrices[selkeycount * 16]);
					selkeys[selkeycount] = sel_key;
					selkeycount++;
				}
			HC_Close_Segment();		
		}
	}

	//find all the keys' corresponding original matrices in the undo list
	for (n = 0; n < selkeycount; n++)
	{
		umanager->BeginUndoItemIteration();
		while((item = umanager->GetNextUndoItem()) != 0)
		{
			if(streq(item->GetName(),"HUndoItemMatrix"))
			{
				HUndoItemMatrix *matrixitem = (HUndoItemMatrix *) item;
				int itemcount = matrixitem->GetCount();
				for(i = 0; i < itemcount; i++)
					if(selkeys[n] == matrixitem->GetKey()[i])
					{
						memcpy(&matrices[actualkeycount*16],&matrixitem->GetMatrix()[i*16],16*sizeof(float));
						actualkeys[actualkeycount] = selkeys[n];
						actualkeycount++;
						break;
					}

				//this means we've found an item, so we can break the "while" and search for next key
				if(i < itemcount)
					break;
			}
		}
		umanager->EndUndoItemIteration();
	}

	if(actualkeycount > 0)
	{
		//add the list of keys/matrices as an undo item
		HUndoItem *newitem = new HUndoItemMatrix(GetView(), actualkeycount, actualkeys, oldmatrices);
		umanager->AddUndoItem(newitem);

		//now smoothly transition back to original locations
		HUtility::MultiSmoothTransitions(GetView(), actualkeycount, actualkeys, matrices);
	}

	delete [] selkeys;
	delete [] actualkeys;
	delete [] matrices;
	delete [] oldmatrices;

	GetView()->ForceUpdate();
	return HOpObjectRotate::OnLButtonUp(hevent);
}
