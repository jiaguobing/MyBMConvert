// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
//#include "StdAfx.h"
#include "HTools.h"
#include "HBaseModel.h"
#include "HBaseView.h"
#include "HBaseOperator.h"
#include "HOpCreateBoolean.h"
#include "HEventInfo.h"
#include "HSelectionSet.h"
#include "HMarkupManager.h"
#include "HEventManager.h"
#include "HImManager.h"



/////////////////////////////////////////////////////////////////////////////
// HOpCreateBoolean
//
// Operator for inserting performing boolean operations on selections

HOpCreateBoolean::HOpCreateBoolean (HBaseView* view, int nBooleanType, int DoRepeat, int DoCapture) : HBaseOperator(view, DoRepeat, DoCapture)
{
	if(nBooleanType>=BOOLEAN_TYPE_INTERSECT && nBooleanType<=BOOLEAN_TYPE_UNION)
		m_BooleanType=nBooleanType;
	
	else m_BooleanType=BOOLEAN_TYPE_NONE;

	HSelectionSet* sel_list = GetView()->GetSelection();
	sel_list->DeSelectAll();
	GetView()->EmitDeSelectAllMessage();
}

HOpCreateBoolean::~HOpCreateBoolean()
{
	HC_Open_Segment_By_Key(GetView()->GetConstructionKey());
		HC_Flush_Contents (".", "geometry");
	HC_Close_Segment();
}

HBaseOperator * HOpCreateBoolean::Clone()
{
	return new HOpCreateBoolean(GetView(), m_BooleanType);
}


const char * HOpCreateBoolean::GetName() { return "HOpCreateBoolean"; }

/////////////////////////////////////////////////////////////////////////////
// HOpCreateBoolean message handlers




int HOpCreateBoolean::OnLButtonDown(HEventInfo &event)
{
    // Mouse has come down
	if (m_BooleanType == BOOLEAN_TYPE_NONE)
		return HOP_READY;

	HC_Open_Segment_By_Key(GetView()->GetConstructionKey());
		HC_Flush_Contents (".", "geometry");
	HC_Close_Segment();

	int res = 0;
	HPoint new_pos = event.GetMouseWindowPos();
	HSelectionSet * selection = GetView()->GetSelection();

	//get the selection
	HC_Open_Segment_By_Key(GetView()->GetViewKey());

		if ((res = HC_Compute_Selection(".", "./scene/overwrite", "v, selection level = entity", new_pos.x, new_pos.y)) > 0)
			HandleSelection(event);

	HC_Close_Segment();
	

	//we've picked the second item, compute the boolean
	if(selection->GetSize()==2)
	{
		char options[256], first_seg_name[2048], second_seg_name[2048], temp_seg_name[2048];
		HC_KEY first_key=m_FirstSel;
		HC_KEY second_key=m_SecondSel;
		HC_KEY containing_segment_0, containing_segment_1, bool_key;
		int first_pcount, first_flist_length, *first_face_list, content_count, 
			second_pcount, second_flist_length, *second_face_list,
			bool_pcount, bool_flist_length, *bool_face_list;
		HPoint *first_points, *second_points, *bool_points;
		float first_transform[16], second_transform[16], inv_first_transform[16];
		char booleantype[MVO_BUFFER_SIZE];

		
		//allocate arrays for point and face lists
		HC_Show_Shell_Size(first_key, &first_pcount, &first_flist_length);
		HC_Show_Shell_Size(second_key, &second_pcount, &second_flist_length);
		first_points=new HPoint [first_pcount];
		first_face_list=new int [first_flist_length];
		second_points=new HPoint [second_pcount];
		second_face_list=new int [second_flist_length];

		//get segment names for later use and clear the selection list
		containing_segment_0=selection->GetAt(0);
		containing_segment_1=selection->GetAt(1);
		HC_Show_Segment(containing_segment_0, first_seg_name);
		HC_Show_Segment(containing_segment_1, second_seg_name);
		selection->DeSelectAll();
		GetView()->EmitDeSelectAllMessage();
		

		//get shell data
		HC_Show_Shell(first_key, &first_pcount, first_points, &first_flist_length, first_face_list);
		HC_Show_Shell(second_key, &second_pcount, second_points, &second_flist_length, second_face_list);


		//get the transforms
		HC_QShow_Net_Modelling_Matrix(first_seg_name, first_transform);
		HC_QShow_Net_Modelling_Matrix(second_seg_name , second_transform);

		//get the inverse of the first matrix for later use
		if(HC_Compute_Matrix_Determinant(first_transform)) HC_Compute_Matrix_Inverse(first_transform, inv_first_transform);
		else HC_Compute_Matrix_Adjoint(first_transform, inv_first_transform);

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
		if(m_BooleanType==BOOLEAN_TYPE_INTERSECT)
			strcpy(booleantype, "intersect");
		else if(m_BooleanType==BOOLEAN_TYPE_SUBTRACT)
			strcpy(booleantype, "subtract");
		else
			strcpy(booleantype, "union");
		
		sprintf(options, "boolean type = %s,  tool handedness = %s, target handedness = %s", 
			booleantype, shell1handedness, shell2handedness);

		//do it
		HC_Open_Segment_By_Key(containing_segment_0);
			bool_key=HC_KCompute_Boolean_Shell(second_pcount, second_points, 0, second_flist_length, second_face_list, 
								first_pcount, first_points, 0, first_flist_length, first_face_list, options);
			
			if (bool_key != INVALID_KEY)
			{			
				HC_Show_Shell_Size(bool_key, &bool_pcount, &bool_flist_length);
				bool_points=new HPoint [bool_pcount];
				bool_face_list=new int [bool_flist_length];
				HC_Show_Shell(bool_key, &bool_pcount, bool_points, &bool_flist_length, bool_face_list);
	
				HC_Compute_Transformed_Points(bool_pcount, bool_points, inv_first_transform, bool_points);
				HC_Insert_Shell(bool_pcount, bool_points, bool_flist_length, bool_face_list);
				HC_Delete_By_Key(bool_key);
				
				delete [] bool_points;
				delete [] bool_face_list;
			}
		HC_Close_Segment();

		//delete original shells
		HC_Delete_By_Key(m_FirstSel);
		HC_Delete_By_Key(m_SecondSel);
		
		m_FirstSel=0;
		m_SecondSel=0;

		//delete empty segments, don't delete the model segment under any circumstances
		do
		{
			HC_Begin_Contents_Search(first_seg_name, "geometry, segments, includes");
			HC_Show_Contents_Count(&content_count);
			HC_End_Contents_Search();
			if(!content_count)
			{
				HC_Show_Owner(first_seg_name, temp_seg_name);
				HC_Delete_Segment(first_seg_name);
				strcpy(first_seg_name, temp_seg_name);
			}
		} while(!content_count && GetView()->GetModelKey()!=HC_KCreate_Segment(first_seg_name));
		

		do
		{
			HC_Begin_Contents_Search(second_seg_name, "geometry, segments, includes");
			HC_Show_Contents_Count(&content_count);
			HC_End_Contents_Search();
			if(!content_count)
			{
				HC_Show_Owner(second_seg_name, temp_seg_name);
				HC_Delete_Segment(second_seg_name);
				strcpy(second_seg_name, temp_seg_name);
			}
		} while(!content_count && GetView()->GetModelKey()!=HC_KCreate_Segment(second_seg_name));


		//clean up
		delete [] first_points;
		delete [] first_face_list;
		delete [] second_points;
		delete [] second_face_list;
	}
		
	//update the scene to reflect the new highlight attributes of the selected items
	HC_Control_Update_By_Key(GetView()->GetViewKey(), "set shadow regeneration");
	GetView()->SetGeometryChanged();
	GetView()->Update();

    return HOP_READY;
}

#define	SEL_NONE	0
#define	SEL_SHELL	1
#define	SEL_LINE	2
#define	SEL_MARKER	3

//largely copied from HOpSelectAperature
void HOpCreateBoolean::HandleSelection(HEventInfo &event)
{
	UNREFERENCED(event);
	
	HC_KEY  key, best = -1;
	int offset1, offset2, offset3;
	HC_KEY *keys		= 0;
	HC_KEY *incl_keys	= 0;
	int	previous	= SEL_NONE;
	int	current		= SEL_NONE;
	char	type[64];
	int	incl_count=0;
	
	// Mouse went down; find the selected items

	do 
	{
	    HC_Show_Selection_Element (&key, &offset1, &offset2, &offset3);
		HC_Show_Selection_Original_Key (&key);
	    //this function makes sure that only the top level segment of an "object" is selectable. If a segment/geometry 
	    //is not part of an object definition the key is not changed
	    key = HObjectManager::FindHObjectSegment(key);

	    // if locally renumbered, grab the original HOOPS keys
	    // NOTE: This will only take care of locally renumbered entities (not segments)
	    if(key >= 0)
	    {
		    char status[MVO_BUFFER_SIZE];
		    HC_Show_Key_Status(key, status);
		    if( streq(status, "invalid") )
			    HC_Show_Selection_Original_Key(&key);
	    }

	    HC_Show_Key_Type (key, type);

	    if (!strcmp (type, "line") || !strcmp (type, "polyline") ||	!strcmp (type, "circular arc") 
								   || !strcmp (type, "elliptical arc"))
			current = SEL_LINE;
	    else if (!strcmp (type, "marker"))
			current = SEL_MARKER;
	    else
			current = SEL_SHELL;

	    if (current > previous) 
		{
			// build up an array of include keys to pass with the selection
			int skey_count;
			char skey_type[MVO_BUFFER_SIZE];
			HC_Show_Selection_Keys_Count(&skey_count);

			delete [] keys;
			delete [] incl_keys;
			keys	  = new HC_KEY[skey_count];
			incl_keys = new HC_KEY[skey_count];
			HC_Show_Selection_Original_Keys(&skey_count, keys);

			incl_count = 0;
			int i;
			for(i = skey_count-1; i >= 0; i--)
			{
				HC_Show_Key_Type(keys[i], skey_type);
				if(strstr(skey_type, "include"))
				{
					incl_keys[incl_count] = keys[i];
					incl_count++;
				}
			}
			best = key;
			previous = current;
	    }
	    
		if (!GetView()->GetVisibilitySelectionMode())	break;

	} while (HC_Find_Related_Selection());

	HSelectionSet* selection = GetView()->GetSelection();

	//only deal with shells
	memset(type, 0, 64);
	HC_Show_Key_Type (best, type);
	if (strcmp (type, "shell") && 
		strcmp (type, "cylinder") && 
		strcmp (type, "polycylinder") && 
		strcmp (type, "sphere"))
		return;

	// if selected, deselect
	if (selection->IsSelected(best, incl_count, incl_keys))
	{
		selection->DeSelect(best, incl_count, incl_keys, true);
		m_FirstSel=0;
	}
	//if the selection list is empty, select the first shell
	else if(selection->GetSize()==0)
	{
		selection->Select(best, incl_count, incl_keys, true);
		m_FirstSel=best;
	}
	//otherwise select the second shell
	else
	{
		selection->Select(best, incl_count, incl_keys, true);
		m_SecondSel=best;
	}

	
	delete[] keys;
	delete[] incl_keys;	
}
