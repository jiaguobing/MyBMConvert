// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// HOpSelectAperture.cpp : implementation of the HOpSelectAperture class
//

#include <math.h>
#include <string.h>

#include "HTools.h"
#include "HBaseModel.h"
#include "HBaseView.h"
#include "HBaseOperator.h"
#include "HOpSelectAperture.h"
#include "HEventInfo.h"
#include "HSelectionSet.h"
#include "HMarkupManager.h"
#include "HEventManager.h"
#include "HImManager.h"
#include "HConstantFrameRate.h"

/////////////////////////////////////////////////////////////////////////////
// HOpSelectAperture
//
// Operator for performing a hit test on the scene and placing hit objects in the view's
// selection list
// Mouse down - computes the selection and adds items to the selection list


HOpSelectAperture::HOpSelectAperture(HBaseView* view, int DoRepeat, int DoCapture) : HBaseOperator(view, DoRepeat, DoCapture)
{
}

HOpSelectAperture::~HOpSelectAperture()
{

	HC_Open_Segment_By_Key(GetView()->GetConstructionKey());
		HC_Flush_Contents (".", "geometry");
	HC_Close_Segment();
}


HBaseOperator * HOpSelectAperture::Clone()
{
	return new HOpSelectAperture(GetView());
}
 


const char * HOpSelectAperture::GetName() { return "HOpSelectAperture"; }

 
#define	SEL_NONE	0
#define	SEL_SHELL	1
#define SEL_REGION  2
#define	SEL_MARKER	4
#define	SEL_LINE	8

void HOpSelectAperture::HandleSelection(HEventInfo &event)
{
	HC_KEY  key = INVALID_KEY;
	int offset1, offset2, offset3;
	HC_KEY *keys		= 0;
	HC_KEY *incl_keys	= 0;
	int	selected_type	= SEL_NONE;
	int region			= 0;
	char type[64]		= {""};
	int	incl_count		= 0;
	HSelectionSet* selection = GetView()->GetSelection();

	//don't notify in the selection set, we'll do that at the end
	selection->SetSelectWillNotify(false);
	
	// Mouse went down; find the selected items
	do {
	    HC_Show_Selection_Element (&key, &offset1, &offset2, &offset3);
		HC_Show_Selection_Original_Key (&key);

	    //this function makes sure that only the top level segment of an "object" is selectable. If a segment/geometry 
	    //is not part of an object definition the key is not changed
	    key = HObjectManager::FindHObjectSegment(key);

		if (key == INVALID_KEY)
			continue;
	    // if locally renumbered, grab the original HOOPS keys
	    // NOTE: This will only take care of locally renumbered entities (not segments)
	    if (key >= 0)
	    {
		    char status[MVO_BUFFER_SIZE];
		    HC_Show_Key_Status(key, status);
		    if ( streq(status, "invalid") )
			    HC_Show_Selection_Original_Key(&key);
	    }

	    HC_Show_Key_Type (key, type);

	    if (streq(type, "line") || streq(type, "polyline") ||
			streq(type, "circular arc") || streq(type, "elliptical arc"))
			selected_type = SEL_LINE;
	    else if (streq(type, "marker"))
			selected_type = SEL_MARKER;
		else {
			/* This may be shell, mesh, cyliner, etc... */
			selected_type = SEL_SHELL;
			/* But if it really is a shell, check for regions. */
			if (streq(type, "shell") && offset3 != -1){
				int low, high;
				HC_Show_Region_Range(key, &low, &high);
				if ((low != high || low > 0) && selection->GetAllowRegionSelection()){
					selected_type |= SEL_REGION;
					HC_Open_Geometry(key);{
						HC_Open_Face(offset3);{
							HC_Show_Region(&region);
						}HC_Close_Face();
					}HC_Close_Geometry();
				}
			}
		}

		// build up an array of include keys to pass with the selection
		int skey_count;
		char skey_type[MVO_BUFFER_SIZE];
		HC_Show_Selection_Keys_Count(&skey_count);

		if (skey_count > 0) {
			delete [] keys;
			delete [] incl_keys;
			keys	  = new HC_KEY[skey_count];
			incl_keys = new HC_KEY[skey_count];
			HC_Show_Selection_Original_Keys(&skey_count, keys);

			int i;
			
			incl_count = 0;
			for(i = skey_count-1; i >= 0; i--)
			{
				HC_Show_Key_Type(keys[i], skey_type);
				if (strstr(skey_type, "include"))
				{
					incl_keys[incl_count] = keys[i];
					incl_count++;
				}
				else if (streq(skey_type, "reference"))
					key = keys[i];
			}
			

			if (selection->GetSelectionLevel()==HSelectSubentity && (selected_type & SEL_SHELL))
			{
				if(event.Shift())
					selection->DeSelectSubentity(key, incl_count, incl_keys, 1, &offset3, &offset1, &offset2, true);
				else
					selection->SelectSubentity(key, incl_count, incl_keys, 1, &offset3, &offset1, &offset2, true);
			}
			// handle control, shift and no_key cases
			else if (event.Control())
			{
				// if selected, deselect, else select
				if (selection->IsSelected(key, incl_count, incl_keys) ||
					(selected_type & SEL_REGION && selection->IsRegionSelected(key, incl_count, incl_keys, region)))
				{
					if (selected_type & SEL_REGION)
						selection->DeSelectRegion(key, incl_count, incl_keys, region, true);
					else 
						selection->DeSelect(key, incl_count, incl_keys, true);
				} 
				else 
				{
					if (selected_type & SEL_REGION)
						selection->SelectRegion(key, incl_count, incl_keys, region, true);
					else
						selection->Select(key, incl_count, incl_keys, true);
				}
			}
			else if (!selection->IsSelected(key, incl_count, incl_keys))
			{
				if (selected_type & SEL_REGION) {
					if (!selection->IsRegionSelected(key, incl_count, incl_keys, region))
						selection->SelectRegion(key, incl_count, incl_keys, region, true);
				}
				else 
					selection->Select(key, incl_count, incl_keys, true);
			}
		}

	} while (HC_Find_Related_Selection());

	selection->SetSelectWillNotify(true);

	// notify the event to view
	GetView()->Notify(HSignalSelected);

	delete[] keys;
	delete[] incl_keys;
	
}
int HOpSelectAperture::OnLButtonDown(HEventInfo &event)
{
	HPoint  new_pos;
    int		res = 0;
	bool	need_update = false;
	
	// Mouse went down; find the selected items
	
	HC_Open_Segment_By_Key(GetView()->GetConstructionKey());
	HC_Flush_Contents (".", "geometry");
	HC_Close_Segment();
	
    HSelectionSet* selection = GetView()->GetSelection();
	
	new_pos = event.GetMouseWindowPos();
	
	// perform standard Windows explorer method of selecting and deselecting
	// using Shift and Control keys
	
	// if neither the Control key nor Shift key is down, deselect everything
    if (selection->GetSize() > 0 &&
		!event.Control() && !event.Shift())
	{
		selection->DeSelectAll();
		GetView()->EmitDeSelectAllMessage();
		need_update = true;
	}

	HMarkupManager *markupManager;
	if ((markupManager = GetView()->GetMarkupManager()) != 0)	
	{
		HC_Open_Segment_By_Key(markupManager->GetMarkupKey());
			// compute the selection using the HOOPS window coordinate of the the pick location

			res = HC_Compute_Selection(GetView()->GetDriverPath(),
				(selection->GetSubwindowPenetration() ? "" : "."), 
				"v, selection level = entity, selection sorting, internal selection limit = 0", new_pos.x, new_pos.y);
	
		HC_Close_Segment();

	}
	if (res == 0)
	{
		HC_Open_Segment_By_Key(GetView()->GetViewKey());

			res = HC_Compute_Selection(".",
				(selection->GetSubwindowPenetration() ? "" : "./scene/overwrite"), 
				"v, selection level = entity, related selection limit = 0, selection sorting, internal selection limit = 0", new_pos.x, new_pos.y);

		HC_Close_Segment();
	}

	if (res > 0) {
		HandleSelection(event);
		need_update = true;
	}
	GetView()->GetConstantFrameRateObject()->SetDisableIncreaseTemp(true);	
	
    
	//GetView()->SetGeometryChanged();
	if (need_update)
		GetView()->Update();	// update the scene to reflect the new highlight attributes
	GetView()->GetConstantFrameRateObject()->SetDisableIncreaseTemp(false);

	if(GetView()->GetFocusOnSelection())
		GetView()->FocusOnSelection();

	// of the selected items
    return HOP_READY;
}

#if 0
int HOpSelectAperture::OnNoButtonDownAndMove(HEventInfo &event)
{
	HPoint  new_pos;
    int		res, offset1, offset2, offset3;
    char	pathname[MVO_SEGMENT_PATHNAME_BUFFER], type[MVO_BUFFER_SIZE];
    HC_KEY	primitive;

	new_pos = event.GetMouseWindowPos();

	HC_Open_Segment_By_Key(GetView()->GetViewKey());
	
		res = HC_Compute_Selection(".", "./scene/overwrite", "v, selection level = entity", new_pos.x, new_pos.y);

	HC_Close_Segment();

	// compute the selection using the HOOPS window coordinate ofthe the pick location
	if (res)
	{	
		HC_Show_Selection_Element (&primitive, 0, 0, 0);
		HC_Show_Selection_Pathname (pathname);

		HC_Open_Segment_By_Key(GetView()->GetConstructionKey());
			HC_Flush_Contents (".", "geometry");
			HC_Set_Color_By_Index ("geometry", 1);
			HC_Set_Line_Weight (3.0);
			HC_Set_Selectability ("everything = off");
			HC_Set_Visibility ("everything = on");

			// Display the name of the selected segment 
			HC_Show_Key_Type (primitive, type);

			if (streq (type, "marker")) 
			{
				char	symbol[4];
				HPoint	point;
				float	size;

				HC_Show_Marker (primitive, &point.x, &point.y, &point.z);
				HC_QShow_Net_Marker_Symbol (pathname, symbol);
				HC_QShow_Net_Marker_Size (pathname, &size);
				HC_Set_Marker_Size (1.5 * size);
				HC_Set_Marker_Symbol (symbol);
				HC_Compute_Coordinates (pathname, "object", &point, "world", &point);
				HC_Insert_Marker (point.x, point.y, point.z);
			}
			else if (streq (type, "text")) 
			{
				HPoint	point;
				HPoint	box[5];
				int		length, i;
				float	x_fraction, y_fraction, matrix[16];
				char	hoops_string[MVO_BUFFER_SIZE];


				// put a box around the text string for highlighting.
				// As the information for the text extent is returned in window coordinates,
				// the box is drawn in the windowspace segment.
				 

				HC_Show_Text_Length (primitive, &length);
				HC_Show_Text (primitive, &point.x, &point.y, &point.z, hoops_string);
			
				HC_QShow_Net_Modelling_Matrix(pathname, matrix);


				// open up a temporary segment in which to set the net mod matrix found in the
				// segment containing the selected text.  This is necessary if it happens to be
				// a segment in the include library, so that Compute_Text_Extent can have a display
				// driver as context
				HC_Open_Segment("dummy");
					HC_Set_Modelling_Matrix(matrix);
					HC_Compute_Text_Extent (".", hoops_string, &x_fraction, &y_fraction);

					// Convert the text insertion point from object space to world space
					HC_Compute_Coordinates (".", "object", &point, "local window", &point);
				HC_Close_Segment();


				box[0].x = 	point.x + x_fraction;  box[0].y = point.y - y_fraction;  box[0].z = point.z;
				box[1].x = 	point.x + x_fraction;  box[1].y = point.y + y_fraction;  box[1].z = point.z;
				box[2].x = 	point.x - x_fraction;  box[2].y = point.y + y_fraction;  box[2].z = point.z;
				box[3].x = 	point.x - x_fraction;  box[3].y = point.y - y_fraction;  box[3].z = point.z;
				box[4].x = 	point.x + x_fraction;  box[4].y = point.y - y_fraction;  box[4].z = point.z;


				for (i = 0; i < 5; i++)
				{
					HC_Compute_Coordinates (".", "local window", &box[i], "world", &box[i]);
				}


				HC_Open_Segment_By_Key (GetView()->GetConstructionKey()); 
					HC_Flush_Contents (".", "geometry");
					HC_Insert_Polyline (5, box);
				HC_Close_Segment ();

			}
			else if (streq (type, "polyline")) 
			{
				HPoint	points[2];

				/*
				 * For the polyline offset1 is the closest vertex to mouse and offset2 is the connecting
				 * vertex that defines the closest edge.
				 */
				HC_Show_Partial_Polyline (primitive, offset1 < offset2 ? offset1 : offset2, 2, points);
    
				HC_Compute_Coordinates (pathname, "object", &points[0], "world", &points[0]);
				HC_Compute_Coordinates (pathname, "object", &points[1], "world", &points[1]);

				HC_Insert_Line (points[0].x, points[0].y, points[0].z, points[1].x, points[1].y, points[1].z);

			}
			else if (streq (type, "polygon")) 
			{
				HPoint	points[2];

				/*
				 * For Polygons, offset1 is the closest vertex to the mouse and teh offset2 is the connecting
				 * vertex defining the closest edge.
				 */

				HC_Show_Partial_Polygon (primitive, offset1 < offset2 ? offset1 : offset2, 2, points);

				HC_Compute_Coordinates (pathname, "object", &points[0], "world", &points[0]);
				HC_Compute_Coordinates (pathname, "object", &points[1], "world", &points[1]);

				HC_Insert_Line (points[0].x, points[0].y, points[0].z, points[1].x, points[1].y, points[1].z);

			}
			else if (streq (type, "shell")) 
			{
				HPoint	points[2];

				/* For the shell offset1 is the nearest vertex, offset 2 is the vertex that defines the best
				 * edge, and offset3 is the best face. If there is no visible face attached to the edge from
				 * offset1 to offset2, offset3 has value of -1.
				 */

				HC_Show_Partial_Shell (primitive, offset1, 1, &points[0], 0, 0, 0, 0);
				HC_Show_Partial_Shell (primitive, offset2, 1, &points[1], 0, 0, 0, 0);

				HC_Compute_Coordinates (pathname, "object", &points[0], "world", &points[0]);
				HC_Compute_Coordinates (pathname, "object", &points[1], "world", &points[1]);

				HC_Insert_Line (points[0].x, points[0].y, points[0].z, points[1].x, points[1].y, points[1].z);

			}
			else if (streq (type, "mesh")) 
			{
				
				int		rows, columns;
				int		row_offset, col_offset;
				HPoint	point1, point2;

					 /* 
				  * Offsets for meshes are the vertex numbers from the array passed into Insert_Mesh.
				  * For a given point in the mesh P(r,c),  its vertex number is r * columns + c.
					  */

					HC_Show_Mesh_Size (primitive, &rows, &columns);

				row_offset = offset1 / columns;
				col_offset = offset1 % columns;
				HC_Show_Partial_Mesh (primitive, row_offset, col_offset, 1, 1, &point1);

				row_offset = offset2 / columns;
				col_offset = offset2 % columns;
				HC_Show_Partial_Mesh (primitive, row_offset, col_offset, 1, 1, &point2);

				HC_Compute_Coordinates (pathname, "object", &point1, "world", &point1);
				HC_Compute_Coordinates (pathname, "object", &point2, "world", &point2);

				HC_Insert_Line (point1.x, point1.y, point1.z, point2.x, point2.y, point2.z);

			}
			else if (streq (type, "image")) 
			{
				char	format[MVO_BUFFER_SIZE];
				HPoint	center;
				HPoint	extent;
				HPoint	origin;
				int 	width, height;
				HPoint	box[5];

				HC_Show_Image_Size (primitive, &center.x, &center.y, &center.z, format, &width, &height);
				extent.x = (float) width;
				extent.y = (float) height;
				extent.z = 0.0F;

				origin.x =  0.0F;
				origin.y =  0.0F;
				origin.z =  0.0F;
	
				HC_Compute_Coordinates (pathname, "object", &center, "world", &center);
				HC_Compute_Coordinates (pathname, "local pixels", &extent, "world", &extent);
				HC_Compute_Coordinates (pathname, "local pixels", &origin, "world", &origin);

				extent.x -= origin.x;
				extent.y -= origin.y;
				extent.z -= origin.z;


				box[0].x = 	center.x + extent.x / 2.0F;  box[0].y = center.y - extent.y / 2.0F;  box[0].z = center.z;
				box[1].x = 	center.x + extent.x / 2.0F;  box[1].y = center.y + extent.y / 2.0F;  box[1].z = center.z;
				box[2].x = 	center.x - extent.x / 2.0F;  box[2].y = center.y + extent.y / 2.0F;  box[2].z = center.z;
				box[3].x = 	center.x - extent.x / 2.0F;  box[3].y = center.y - extent.y / 2.0F;  box[3].z = center.z;
				box[4].x = 	center.x + extent.x / 2.0F;  box[4].y = center.y - extent.y / 2.0F;  box[4].z = center.z;

				HC_Insert_Polyline (5, box);
			}   
			else if (streq (type, "line")) 
			{
				HPoint	point1, point2;

				HC_Show_Line (primitive, &point1.x, &point1.y, &point1.z, &point2.x, &point2.y, &point2.z);
				HC_Compute_Coordinates (pathname, "object", &point1, "world", &point1);
				HC_Compute_Coordinates (pathname, "object", &point2, "world", &point2);

				HC_Insert_Line (point1.x, point1.y, point1.z, point2.x, point2.y, point2.z);
			}   
			else if (streq (type, "circular chord")) 
			{
				 HPoint	points[3];

				 HC_Show_Circular_Chord (primitive, &points[0], &points[1], &points[2]);
				 HC_Compute_Coordinates (pathname, "object", &points[0], "world", &points[0]);
				 HC_Compute_Coordinates (pathname, "object", &points[1], "world", &points[1]);
				 HC_Compute_Coordinates (pathname, "object", &points[2], "world", &points[2]);

				 HC_Insert_Circular_Chord (&points[0], &points[1], &points[2]);

			}
			else if (streq (type, "circular wedge")) 
			{
				 HPoint	points[3];

				 HC_Show_Circular_Wedge (primitive, &points[0], &points[1], &points[2]);
				 HC_Compute_Coordinates (pathname, "object", &points[0], "world", &points[0]);
				 HC_Compute_Coordinates (pathname, "object", &points[1], "world", &points[1]);
				 HC_Compute_Coordinates (pathname, "object", &points[2], "world", &points[2]);

				 HC_Insert_Circular_Wedge (&points[0], &points[1], &points[2]);

			}
			else if (streq (type, "circle")) 
			{
				 HPoint	points[3];

				 HC_Show_Circle (primitive, &points[0], &points[1], &points[2]);
				 HC_Compute_Coordinates (pathname, "object", &points[0], "world", &points[0]);
				 HC_Compute_Coordinates (pathname, "object", &points[1], "world", &points[1]);
				 HC_Compute_Coordinates (pathname, "object", &points[2], "world", &points[2]);

				 HC_Insert_Circle (&points[0], &points[1], &points[2]);
			}
			else if (streq (type, "circular arc")) 
			{
				 HPoint	points[3];

				 HC_Show_Circular_Arc (primitive, &points[0], &points[1], &points[2]);
				 HC_Compute_Coordinates (pathname, "object", &points[0], "world", &points[0]);
				 HC_Compute_Coordinates (pathname, "object", &points[1], "world", &points[1]);
				 HC_Compute_Coordinates (pathname, "object", &points[2], "world", &points[2]);

				 HC_Insert_Circular_Arc (&points[0], &points[1], &points[2]);
			}
			else if (streq (type, "ellipse")) 
			{
				 HPoint	center, major_axis, minor_axis;

				 HC_Show_Ellipse (primitive, &center, &major_axis, &minor_axis);
				 HC_Compute_Coordinates (pathname, "object", &center, "world", &center);
				 HC_Compute_Coordinates (pathname, "object", &major_axis,  "world", &major_axis);
				 HC_Compute_Coordinates (pathname, "object", &minor_axis,  "world", &minor_axis);

				 HC_Insert_Ellipse (&center, &major_axis, &minor_axis);

			}   
			else if (streq (type, "elliptical arc")) 
			{
				 HPoint	center, major_axis, minor_axis;
				 float	start, end;

				 HC_Show_Elliptical_Arc (primitive, &center, &major_axis, &minor_axis, &start, &end);
				 HC_Compute_Coordinates (pathname, "object", &center, "world", &center);
				 HC_Compute_Coordinates (pathname, "object", &major_axis,  "world", &major_axis);
				 HC_Compute_Coordinates (pathname, "object", &minor_axis,  "world", &minor_axis);

				 HC_Insert_Elliptical_Arc (&center, &major_axis, &minor_axis, start, end);
			}  
			else if (streq (type, "grid")) 
			{
				/*
				 *  For the grids, offsets 1 and 2 orient the selection in coordinate system based
				 *  on the basis vectors used to define the grid.
				 *
				 *  Highlight by inserting markers at the closest two vertices. This shows how to snap to grid.
				 */
				HPoint	origin, ref1, ref2, marker;
				int		count1, count2;
				char	grid_type[MVO_BUFFER_SIZE];


				HC_Show_Grid (primitive, grid_type, &origin, &ref1, &ref2, &count1, &count2);


				if (streq (grid_type, "quadrilateral"))  
				{

					HPoint	basis1, basis2;

					/* find basis vectors for quad grid */
					basis1.x = ref1.x - origin.x;    basis1.y = ref1.y - origin.y;    basis1.z = ref1.z - origin.z;
					basis2.x = ref2.x - origin.x;    basis2.y = ref2.y - origin.y;    basis2.z = ref2.z - origin.z;

					/* find the grid point in object space */
					marker.x = origin.x + basis1.x * offset1 + basis2.x * offset2;
					marker.y = origin.y + basis1.y * offset1 + basis2.y * offset2;
					marker.z = origin.z + basis1.z * offset1 + basis2.z * offset2;

				}
				else if (streq (grid_type, "radial")) 
				{
					HPoint	radius;
					HPoint	basis2;
					float	wedge_angle;
					float	dot_product;
					float	radius_angle;
					float	length_radius;
					float	length_basis2;


					/* find radius vector of pie wedge */
					radius.x = ref1.x - origin.x;    radius.y = ref1.y - origin.y;    radius.z = ref1.z - origin.z;

					/*
					 * Find the angle of the pie wedge.
					 *
					 *  First find the second basis vector (radius is the first)
					 *  Then:
					 *  Given two vectors A and B, the angle between is:
					 *    angle = H_ACOS ( A dot B / |A| * |B|)
					 *
					 */
					basis2.x = ref2.x - origin.x;    basis2.y = ref2.y - origin.y;    basis2.z = ref2.z - origin.z;
					length_basis2 = (float)(sqrt (basis2.x * basis2.x + basis2.y * basis2.y + basis2.z * basis2.z));
					length_radius = (float)(sqrt (radius.x * radius.x + radius.y * radius.y + radius.z * radius.z));
					dot_product = radius.x * basis2.x + radius.y * basis2.y + radius.z * basis2.z;

					wedge_angle = (float) H_ACOS (dot_product / (length_radius * length_basis2));

					/*
					 * Find the angle between radius vector and x axis
					 * use formula from above where vector A = radius vector and B = unit vector (1, 0, 0)
					 * note that A dot B is just x-component of A and that |B| = 1.0;
					 */
					radius_angle = (float) H_ACOS (radius.x / length_radius);

					/* Compute the marker location in object space */
					marker.x = origin.x + (offset1 * radius.x) * (float)(cos (double(offset2 * wedge_angle + radius_angle)));
					marker.y = origin.y + (offset1 * radius.x) * (float)(sin (double(offset2 * wedge_angle + radius_angle)));
					marker.z = origin.z + (offset1 * radius.z);

				}

				HC_Compute_Coordinates (pathname, "object", &marker, "world", &marker);
				HC_Insert_Marker (marker.x, marker.y, marker.z);

			}

		HC_Close_Segment ();

	}


	GetView()->Update();
    return HOP_READY;
}

#endif








