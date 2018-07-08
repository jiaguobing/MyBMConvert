// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// HOpSelectArea.cpp : implementation of the HOpSelectArea class
//

#include <math.h>
#include <string.h>

#include "HTools.h"
#include "HBaseModel.h"
#include "HBaseView.h"
#include "HBaseOperator.h"
#include "HOpConstructRectangle.h"
#include "HOpSelectArea.h"
#include "HEventInfo.h"
#include "HSelectionSet.h"
#include "HUtility.h"
#include "HEventManager.h"
#include "HConstantFrameRate.h"
#include "HPShowCache.h"



/////////////////////////////////////////////////////////////////////////////
// HOpSelectArea
//
// Operator for performing a hit test on the scene using a screen-space window
// and placing hit objects in the view's selection list
// Mouse down - computes the first point of the selection window
// Mouse down and move - rubberbands a box to denote current selection window
// Mouse up - computes the area selection and adds items to the selection list
// No mouse down and move - performs an "O-snap" quickmoves highlight of closest marker edge or line segment

 
HOpSelectArea::HOpSelectArea(HBaseView* view, int DoRepeat, int DoCapture) 
		: HOpConstructRectangle(view, DoRepeat, DoCapture, false)
{
}

HOpSelectArea::~HOpSelectArea()
{
}

HBaseOperator * HOpSelectArea::Clone()
{
	return new HOpSelectArea(GetView());
}



const char * HOpSelectArea::GetName() { return "HOpSelectArea"; }


int HOpSelectArea::OnLButtonDown(HEventInfo &event)
{	
	int status = HOpConstructRectangle::OnLButtonDown(event);
	HRenderMode mode = GetView()->GetRenderMode();

	HC_Open_Segment_By_Key(GetView()->GetWindowspaceKey());
		//HOpConstructRectangle handles the save and restore for visibility and color
		if (mode != HRenderHiddenLine && 
			mode != HRenderHiddenLineFast && 
			mode != HRenderHiddenLineHOOPS &&
			mode != HRenderBRepHiddenLine &&
			mode != HRenderBRepHiddenLineFast &&
			GetView()->GetTransparentSelectionBoxMode() &&
			GetView()->GetQuickMovesMethod() != HQuickMoves_XOR) {
			HC_Set_Visibility ("faces=on");
			HC_Set_Color ("faces = transmission = light grey");
		}
		HC_Set_Color ("geometry = blue");
	HC_Close_Segment();
	GetView()->GetConstantFrameRateObject()->SetDisableIncreaseTemp(true);
	return status;
}


int HOpSelectArea::OnLButtonUp(HEventInfo &event)
{
	if (!OperatorStarted()) 
		return HBaseOperator::OnLButtonDownAndMove(event);

	HOpConstructRectangle::OnLButtonUp(event);

	HSelectionSet* selection = GetView()->GetSelection();

	// Perform standard Windows explorer method of selecting and deselecting
	// using Shift and Control keys

	// If neither the Control key nor Shift key is down, deselect everything
	if (!event.Control() && !event.Shift())
	{
		selection->DeSelectAll();
		GetView()->EmitDeSelectAllMessage();
	}


	HC_Open_Segment_By_Key(GetView()->GetViewKey());
		// compute the selection using the HOOPS window coordinate of the the pick location
		int selection_count = 0;

		bool need_internal_selection = (selection->GetSelectionLevel()==HSelectSubentity ||
										selection->GetSelectionLevel()==HSelectRegion);


		if (m_bRectangleExists) 
		{
			const char * opt = need_internal_selection?
				"v, no related selection limit, no selection sorting, no internal selection limit":
				"v, no related selection limit, no selection sorting, internal selection limit=0";

			//HC_QSet_Heuristics ("./scene/overwrite", "no related selection limit, no selection sorting, internal selection limit=0");
			HUtility::Order(&m_ptRectangle[0], &m_ptRectangle[1]);
			selection_count = HC_Compute_Selection_By_Area(".",
				(selection->GetSubwindowPenetration() ? "" : "./scene/overwrite"), opt, 
					m_ptRectangle[0].x, m_ptRectangle[1].x,	m_ptRectangle[0].y, m_ptRectangle[1].y);
		}
		else
		{
			const char * opt = need_internal_selection?
				"v, related selection limit = 0, selection sorting, no internal selection limit":
				"v, related selection limit = 0, selection sorting, internal selection limit=0";

			//HC_QSet_Heuristics ("./scene/overwrite", "related selection limit = 0, selection sorting, internal selection limit=0");
			HPoint new_pos = event.GetMouseWindowPos();
			selection_count = HC_Compute_Selection(".",
				(selection->GetSubwindowPenetration() ? "" : "./scene/overwrite"), opt, 
					new_pos.x, new_pos.y);
		}
		//HC_QUnSet_Heuristics ("./scene/overwrite");
	HC_Close_Segment();


	// BIG NOTE AND A BIG TODO:
	//	I am disabling this optimization of not having to check for selected for now
	// The problem is when you have segment level selection and you selection multiple 
	// entities owned by same segment. With this optimization you end up in duplicate selection
	// which causes problems when deselection etc. (primitive.hmf will show the problem)
	// With this optimization - selection is about 26% faster for roughly 500,000 entity selection.
	// flooplan.hmf.
	// So - we need to revisit this - Rajesh B (22-Mar-05)
	bool need_to_check_selected = true || (selection->GetSize()>0);

	if (selection_count)
	{

		int count = 0;
		int * vertex = 0;
		int * faces = 0;
		int array_size = 0;
		bool * regions = 0;
		int region_array_size = 0;

		selection->SetPShowCache(new HVHashPShowCache);

		selection->SetSelectWillNotify(false);
		
		do	// process each item in the selection queue
		{
			bool only_select_regions = false;
			HC_KEY key, okey;

			// Use this function instead of HC_Show_Selection_Element so we get the internal key 3dGS
			// uses to refer to the piece of geometry.  key will hold this value
			HC_Show_Selection_Original_Key(&key);

			//this function makes sure that only the top level segment of an "object" is selectable. If a segment/geometry 
			//is not part of an object definition the key is not changed
			okey = HObjectManager::FindHObjectSegment(key);

			if(okey != key) {
				need_to_check_selected=true;
				key = okey;
			}
			if (key == INVALID_KEY)
				continue;

			// build up an array of include keys to pass with the selection
			int skey_count;
			char skey_type[MVO_BUFFER_SIZE];
			HC_Show_Selection_Keys_Count(&skey_count);

#define KEY_DATA_NOMINAL_LENGTH	128

			HC_KEY keys_data[KEY_DATA_NOMINAL_LENGTH];
			HC_KEY incl_keys_data[KEY_DATA_NOMINAL_LENGTH];

			HC_KEY *	keys = keys_data;
			HC_KEY *	incl_keys = incl_keys_data;

			if (skey_count > KEY_DATA_NOMINAL_LENGTH) {
				keys = new HC_KEY[skey_count];
				incl_keys = new HC_KEY[skey_count];
			}

			HC_Show_Selection_Original_Keys(&skey_count, keys);

			int i, incl_count = 0;
			for(i = skey_count-1; i >= 0; i--)
			{
				HC_Show_Key_Type(keys[i], skey_type);
				if(streq(skey_type, "include"))
				{
					incl_keys[incl_count] = keys[i];
					incl_count++;
				}
				else if(streq(skey_type, "reference"))
					key = keys[i];
				else if(streq(skey_type, "shell")) 
				{
					int low = 0, high = 0;
					HC_Show_Region_Range(keys[i], &low, &high);
					if(selection->GetSelectionLevel()==HSelectRegion && (low != high || low > 0))
					{
						/* Select regions in the shell. */
						HC_Show_Selection_Elements_Coun(0, &count);

						if(count > 0){
							/* Get the selected faces. */
							if (count > array_size){
								delete [] vertex;
								delete [] faces;
								array_size = count;
								vertex = new int[array_size];
								faces = new int[array_size];
							}
							HC_KEY shell_key = keys[i];
							HC_Show_Selection_Elements(0, &count, vertex, vertex, faces);

							/* build an array that represents selected regions. */
							if (high+1 > region_array_size){
								delete [] regions;
								regions = new bool[high +1];
								region_array_size = high +1;
							}
							memset(regions, 0, sizeof(bool) * (high +1));
							/* TODO: find a faster way to do this. */
							HC_Open_Geometry(shell_key);{
								int region;
								for(int i=0; i<count; ++i){
									if(faces[i] == INVALID_KEY){
										continue;
									}
									HC_Open_Face(faces[i]);{
										HC_Show_Region(&region);
										regions[region] = true;
									}HC_Close_Face();
								}
							}HC_Close_Geometry();
							HShellObject oShellObj(shell_key);
							for(int i = 0; i <= high; ++i){
								if(regions[i] && !selection->IsRegionSelected(shell_key, incl_count, incl_keys, i)){
									selection->SelectRegion(oShellObj, incl_count, incl_keys, i, true);
								}
							}
							only_select_regions = true;
						}

					}
				}
			}

			if(selection->GetSelectionLevel()==HSelectSubentity)
			{
				HC_KEY sub_key;
				int sub_count;
				int * sub_v1, * sub_v2, * sub_faces;

				HC_Show_Selection_Elements_Coun(&sub_key, &sub_count);

				sub_v1 = new int [sub_count];
				sub_v2 = new int [sub_count];
				sub_faces = new int [sub_count];

				HC_Show_Selection_Elements(&sub_key, &sub_count, sub_v1, sub_v2, sub_faces);

				if(event.Shift())
					selection->DeSelectSubentity(key, incl_count, incl_keys, sub_count, sub_faces, sub_v1, sub_v2, true);
				else
					selection->SelectSubentity(key, incl_count, incl_keys, sub_count, sub_faces, sub_v1, sub_v2, true);

				delete [] sub_v1;
				delete [] sub_v2;
				delete [] sub_faces;
			}

			// handle control, shift and no_key cases
			else if (!only_select_regions &&
				(!need_to_check_selected ||	!(selection->IsSelected(key, incl_count, incl_keys)))){
				selection->Select(key, incl_count, incl_keys, true);
			}
			
			if (skey_count > KEY_DATA_NOMINAL_LENGTH) {
				delete[] keys;
				delete[] incl_keys;
			}

			only_select_regions = false;

		} while (HC_Find_Related_Selection());

		selection->SetSelectWillNotify(true);

		// notify the event to view
		GetView()->Notify(HSignalSelected, selection);

		selection->FlushPShowCache();

		delete [] regions;
		delete [] vertex;
		delete [] faces;
	}

	if(GetView()->GetFocusOnSelection())
		GetView()->FocusOnSelection();
	GetView()->Update();
	GetView()->GetConstantFrameRateObject()->SetDisableIncreaseTemp(false);
	return HOP_READY;
}




