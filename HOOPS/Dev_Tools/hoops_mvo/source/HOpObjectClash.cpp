// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#include <math.h>
#include <string.h>
#include <stdio.h>

#include "HTools.h"

#include "HBaseModel.h"
#include "HBaseView.h"
#include "HBaseOperator.h"
#include "HEventInfo.h"
#include "HOpObjectClash.h"
#include "HSelectionSet.h"
#include "HSelectionItem.h"
#include "vlist.h"

#include "HUndoManager.h"

class HKeyList: public VList<HC_KEY>
{
};



float big_float[10000];
int big_int[1000];



HOpObjectClash::HOpObjectClash(HBaseView * view, int doRepeat, int doCapture)
	: HOpObjectTranslate(view, doRepeat, doCapture)
{
	m_pClashSelection = new HSelectionSet(view, true);
	m_pClashSelection->Init();

	m_TempSegKey = HC_ERROR_KEY;
	m_SelectMode = HOOC_Shell;
	m_IntersectPolylinesVisible = false;

	// Disable spriting
	m_save_spriting_mode = GetView()->GetSpritingMode();
	if (m_save_spriting_mode)
		GetView()->SetSpritingMode(false);
}


HOpObjectClash::~HOpObjectClash()
{
	if (m_save_spriting_mode)
		GetView()->SetSpritingMode(m_save_spriting_mode);

	delete m_pClashSelection;

	if (m_TempSegKey != HC_ERROR_KEY)
	{
		HC_Delete_By_Key(m_TempSegKey);
	}
}


void HOpObjectClash::GrabSelectionSet()
{
	HSelectionSet * selection = GetView()->GetSelection();
	m_pClashSelection->DeSelectAll();
	m_pClashSelection->SetHighlightMode(selection->GetHighlightMode());

	HPixelRGBA rgba;
	selection->GetSelectionFaceColor(rgba);
	m_pClashSelection->SetSelectionFaceColor(rgba);
	selection->GetSelectionEdgeColor(rgba);
	m_pClashSelection->SetSelectionEdgeColor(rgba);
	selection->GetSelectionMarkerColor(rgba);
	m_pClashSelection->SetSelectionMarkerColor(rgba);
	HC_Open_Segment_By_Key(GetView()->GetSceneKey());{
		m_TempSegKey = HC_Open_Segment("HOpObjectClash_temp");{
			HC_Set_Window(-1, 1, -1, 1);
			HC_Set_Camera_By_Volume("stretched", -1, 1, -1, 1);
			HC_Set_Window_Pattern("clear");
			HC_Set_Visibility("everything = off");
		}HC_Close_Segment();
	}HC_Close_Segment();
	SetSelectMode(m_SelectMode);


	// Turn off selectability of the selected objects
	unsigned int size = selection->GetSize();
	for (unsigned int i = 0; i < size; ++i) 
	{
		HC_KEY object = selection->GetAt(i);
		char type[64];
		HC_Show_Key_Type(object, type);
		if (streq(type, "segment")) 
		{
			HC_Open_Segment_By_Key(object);{
				HC_Set_Selectability("everything = off");
			}HC_Close_Segment();
		}
		else 
		{
			selection->DeSelect(object, 0, 0, true);
		}
	}
}

void HOpObjectClash::RestoreSelectionSet()
{
	int i = 0;
	while (HC_KEY object = m_KeyList->NthItem(i++)) // Does this need to handle keys renumbered to 0?
	{
		HC_Open_Segment_By_Key(object);{
			HC_UnSet_Selectability(); // Unset the selectability setting that we set during GrabSelectionSet
		}HC_Close_Segment();
	}

	m_pClashSelection->DeSelectAll();
}

void HOpObjectClash::UpdateKeyList(void * data)
{
	if (data != m_pClashSelection)
	{
		// Order is important
		RestoreSelectionSet();
		Cleanup();
		Init();
		GrabSelectionSet();
	}
}


HBaseOperator * HOpObjectClash::Clone()
{
	return new HOpObjectClash(GetView());
}


const char * HOpObjectClash::GetName() { return "HOpObjectClash"; }


int HOpObjectClash::GetScreenVolume(HC_KEY seg, HPoint & min, HPoint & max) 
{
	HPoint bbx[2];
	HPoint bbox[8];
	float matrix[16];

	// Get min/max and enumerate the 8 corner points around seg's bounding cuboid
	HC_Open_Segment_By_Key(seg);
		HC_Compute_Circumcuboid(".", &bbx[0], &bbx[1]);
		HUtility::GenerateCuboidPoints(&bbx[0], &bbx[1], bbox);
		HC_PShow_Net_Modelling_Matrix(0, 0, matrix);
	HC_Close_Segment();

	// Get screen space min/max 
	HC_Open_Segment_By_Key(GetView()->GetSceneKey());{
		HC_Open_Segment("dummy");{
			HC_Set_Modelling_Matrix (matrix);
			for (int i = 0 ; i < 8 ; i++) {
				HC_Compute_Coordinates(".", "object", &bbox[i], "local window", &bbox[i]);
				HUtility::ClampPointToWindow(&bbox[i]);
			}
		}HC_Close_Segment();
	}HC_Close_Segment();

	min = bbox[0];
	max = bbox[0];
	for (int i = 1; i < 8; i++)
	{
		if (min.x > bbox[i].x) min.x = bbox[i].x;
		if (min.y > bbox[i].y) min.y = bbox[i].y;
		if (min.z > bbox[i].z) min.z = bbox[i].z;
		if (max.x < bbox[i].x) max.x = bbox[i].x;
		if (max.y < bbox[i].y) max.y = bbox[i].y;
		if (max.z < bbox[i].z) max.z = bbox[i].z;
	}
	return HOP_OK;
}


int HOpObjectClash::IntersectPolyline(HC_KEY shell1, float * transformed_points1)
{
	if (shell1 == HC_ERROR_KEY || transformed_points1 == 0)
		return 0;

	// Assumes that the thing that we are clashing against is on HOOPS's selection stack
	HC_KEY shell2;
	HC_Show_Selection_Element(&shell2, 0, 0, 0);
	int point_count2;
	HC_Show_Shell_Size(shell2, &point_count2, 0);
	if (point_count2 == 0)
		return 0;

	float const * points2;
	HC_Show_Geometry_Pointer(shell2, "points", &points2);
	float * transformed_points2 = new float[3 * point_count2];

	int selection_keys_count;
	HC_KEY selection_keys[64];
	HC_Show_Selection_Keys(&selection_keys_count, selection_keys);

	float modelling_matrix[16];
	HC_PShow_Net_Modelling_Matrix(selection_keys_count, selection_keys, modelling_matrix);
	HC_Compute_Transformed_Points(point_count2, points2, modelling_matrix, transformed_points2);

	int point_count1;
	int face_list_length1;
	HC_Show_Shell_Size(shell1, &point_count1, &face_list_length1);
	int const * face_list1;
	HC_Show_Geometry_Pointer(shell1, "face list", &face_list1);

	int face_list_length2;
	HC_Show_Shell_Size(shell2, &point_count2, &face_list_length2);
	int const * face_list2;
	HC_Show_Geometry_Pointer(shell2, "face list", &face_list2);

	int * polyline_lengths = big_int;
	float *polyline_points = big_float;
	int polyline_count;
	int polyline_point_count;
	HC_Compute_Intersect_Polyline(
		point_count1, transformed_points1, face_list_length1, face_list1,
		point_count2, transformed_points2, face_list_length2, face_list2,
		&polyline_count, polyline_lengths, &polyline_point_count, polyline_points);

	if (m_IntersectPolylinesVisible) {
		HC_Open_Segment_By_Key(GetView()->GetIntersectPolylinesKey());{
			float * ptr = polyline_points;
			for (int i = 0 ; i < polyline_count; ++i) {
				HC_Insert_Polyline(polyline_lengths[i], ptr);
				ptr += 3 * polyline_lengths[i];
			}
		}HC_Close_Segment();
	}

	delete [] transformed_points2;

	return polyline_count;
}


int HOpObjectClash::TestEnclosure(HC_KEY shell1, float * transformed_points1)
{
	if (shell1 == HC_ERROR_KEY || transformed_points1 == 0)
		return HOOC_Unknown;

	// Assumes that the thing that we are clashing against is on HOOPS's selection stack
	HC_KEY shell2;
	HC_Show_Selection_Element(&shell2, 0, 0, 0);
	int point_count2;
	HC_Show_Shell_Size(shell2, &point_count2, 0);
	if (point_count2 == 0)
		return HOOC_Unknown;

	float const * points2;
	HC_Show_Geometry_Pointer(shell2, "points", &points2);
	
	int selection_keys_count;
	HC_KEY selection_keys[64];
	HC_Show_Selection_Keys(&selection_keys_count, selection_keys);

	float modelling_matrix[16];
	HC_PShow_Net_Modelling_Matrix(selection_keys_count, selection_keys, modelling_matrix);
	float * transformed_points2 = new float[3 * point_count2];
	HC_Compute_Transformed_Points(point_count2, points2, modelling_matrix, transformed_points2);

	int point_count1;
	int face_list_length1;
	HC_Show_Shell_Size(shell1, &point_count1, &face_list_length1);
	int const * face_list1;
	HC_Show_Geometry_Pointer(shell1, "face list", &face_list1);
	
	int face_list_length2;
	HC_Show_Shell_Size(shell2, &point_count2, &face_list_length2);
	int const * face_list2;
	HC_Show_Geometry_Pointer(shell2, "face list", &face_list2);

	char enclosure_result[128];
	HC_Compute_Shell_Enclosure(
		point_count1, transformed_points1, face_list_length1, face_list1,
		point_count2, transformed_points2, face_list_length2, face_list2,
		enclosure_result);

	HOOC_Result return_val = HOOC_Unknown;
	if (streq(enclosure_result, "clash"))
		return_val = HOOC_Clash;
	else if (streq(enclosure_result, "in"))
		return_val = HOOC_In;
	else if (streq(enclosure_result, "out"))
		return_val = HOOC_Out;

	delete [] transformed_points2;

	return return_val;
}


void HOpObjectClash::ProcessSelectionResults(HC_KEY current_shell_key, float * transformed_points)
{
	H_ASSERT(m_pClashSelection);

	do {
		HC_KEY	hit_object;
		HC_Show_Selection_Original_Key(&hit_object);

		char sel_type[64];
		HC_Show_Key_Type(hit_object, sel_type);

		// Build up an array of include keys to pass with the selection
		int skey_count;
		char skey_type[MVO_BUFFER_SIZE];
		HC_Show_Selection_Keys_Count(&skey_count);

		HC_KEY * keys = new HC_KEY[skey_count];
		HC_KEY * incl_keys = new HC_KEY[skey_count];
		HC_Show_Selection_Keys(&skey_count, keys);

		int incl_count = 0;
		for (int i = skey_count - 1; i >= 0; --i)
		{
			HC_Show_Key_Type(keys[i], skey_type);
			if (strstr(skey_type, "include"))
			{
				incl_keys[incl_count] = keys[i];
				incl_count++;
			}
		}

		if (m_IntersectPolylinesVisible && m_SelectMode == HOOC_Shell && streq(sel_type, "shell"))
			IntersectPolyline(current_shell_key, transformed_points);

		if (m_pClashSelection->IsSelected(hit_object, incl_count, incl_keys))
		{
			delete[] keys;
			delete[] incl_keys;
			continue;
		}

		// Ignore multi-instancing effects for now
		if (m_SelectMode == HOOC_Enclosure) 
		{
			int result = TestEnclosure(current_shell_key, transformed_points);
			if (result == HOOC_In) 
			{
				// Change the highlight style to red and restore
				HPixelRGBA saved_hilite_color;
				m_pClashSelection->GetSelectionFaceColor(saved_hilite_color);{
					HPixelRGBA new_hilite_color(255, 0, 0, 0);
					m_pClashSelection->SetSelectionFaceColor(new_hilite_color);
					m_pClashSelection->Select(hit_object, incl_count, incl_keys, false);
				}m_pClashSelection->SetSelectionFaceColor(saved_hilite_color);
			}
		}
		else {
			m_pClashSelection->Select(hit_object, incl_count, incl_keys, false);
		}

		delete[] keys;
		delete[] incl_keys;

	} while(HC_Find_Related_Selection());
}



int HOpObjectClash::SelectByScreenVolume(HSmartSelItem * sel_item)
{
	HC_KEY seg = sel_item->GetKey();

	while (IsSpecialKey(seg))
		seg = HC_Show_Owner_By_Key(seg, 0);

	HPoint min;
	HPoint max;
	GetScreenVolume(seg, min, max);

	HC_Open_Segment_By_Key(GetView()->GetViewKey());{
		int sel_count = HC_Compute_Selection_By_Volume(
			".", "./scene/overwrite", "v, selection level = entity, no related selection limit",
			min.x, max.x, min.y, max.y, min.z, max.z);
		if (sel_count > 0)
			ProcessSelectionResults();
	}HC_Close_Segment();

	return HOP_OK;
}


int HOpObjectClash::GetWorldVolume(HC_KEY seg, HPoint & min, HPoint & max) 
{
	HPoint bbx[2];
	HPoint bbox[8];
	float matrix[16];

	// Get min/max and enumerate the 8 corner points around seg's bounding cuboid
	HC_Open_Segment_By_Key(seg);{
		HC_Compute_Circumcuboid(".", &bbx[0], &bbx[1]);
		HUtility::GenerateCuboidPoints(&bbx[0], &bbx[1], bbox);
		HC_PShow_Net_Modelling_Matrix(0, 0, matrix);
	}HC_Close_Segment();

	// Get screen space min/max 
	HC_Open_Segment_By_Key(GetView()->GetSceneKey());{
		HC_Open_Segment("dummy");{
			HC_Set_Modelling_Matrix (matrix);
			for (int i = 0; i < 8; i++) {
				HC_Compute_Coordinates(".", "object", &bbox[i], "world", &bbox[i]);
			}
		}HC_Close_Segment();
	}HC_Close_Segment();

	min = bbox[0];
	max = bbox[0];
	for (int i = 1; i < 8; i++)
	{
		if (min.x > bbox[i].x) min.x = bbox[i].x;
		if (min.y > bbox[i].y) min.y = bbox[i].y;
		if (min.z > bbox[i].z) min.z = bbox[i].z;
		if (max.x < bbox[i].x) max.x = bbox[i].x;
		if (max.y < bbox[i].y) max.y = bbox[i].y;
		if (max.z < bbox[i].z) max.z = bbox[i].z;
	}
	return HOP_OK;
}


int HOpObjectClash::SelectByWorldVolume(HSmartSelItem * sel_item)
{
	HC_KEY seg = sel_item->GetKey();

	while (IsSpecialKey(seg))
		seg = HC_Show_Owner_By_Key(seg, 0);

	HPoint min;
	HPoint max;
	GetWorldVolume(seg, min, max);

	HC_Open_Segment_By_Key(GetView()->GetViewKey());{
		int sel_count = HC_Compute_Selection_By_Volume(
			"", "./scene/overwrite", "v, selection level = entity, no related selection limit",
			min.x, max.x, min.y, max.y, min.z, max.z);
		if (sel_count > 0) {
			ProcessSelectionResults();
		} 
	}HC_Close_Segment();

	return HOP_OK;
}


int HOpObjectClash::SelectByEnclosure(HSmartSelItem * sel_item)
{
	HC_KEY seg = sel_item->GetKey();

	while (IsSpecialKey(seg))
		seg = HC_Show_Owner_By_Key(seg, 0);

	char type[64];
	HC_Show_Key_Type(seg, type);
	if (!streq(type, "segment"))
		seg = HC_Show_Owner_By_Key(seg, 0);

	HC_KEY * path = new HC_KEY[sel_item->GetIncludeCount() + 2];
	path[0] = seg;

	int path_count = 0;
	for ( ; path_count < sel_item->GetIncludeCount(); ++path_count)
		path[path_count + 1] = sel_item->GetIncludeKeys()[sel_item->GetIncludeCount() - path_count - 1];

	if (path[path_count] != GetView()->GetViewKey())
		path[++path_count] = GetView()->GetViewKey();

	HC_KEY key = HC_ERROR_KEY;
	float * original_points = 0;
	float * transformed_points = 0;
	int * face_list = 0;
	HC_Open_Segment_By_Key(seg);{
		HC_Begin_Contents_Search(".", "shells");{
			while (HC_Find_Contents(0, &key)) {
				int point_count;
				int face_list_length;
				HC_Show_Shell_Size(key, &point_count, &face_list_length);
				if (point_count == 0)
					continue;

				HC_Show_Geometry_Pointer(key, "points", &original_points);
				HC_Show_Geometry_Pointer(key, "face list", &face_list);

				float modelling_matrix[16];
				HC_PShow_Net_Modelling_Matrix(path_count, path, modelling_matrix);

				transformed_points = new float[3 * point_count];
				HC_Compute_Transformed_Points(point_count, original_points, modelling_matrix, transformed_points); 

				break;
			}
		}HC_End_Contents_Search();
	}HC_Close_Segment();

	if (face_list == 0 || original_points == 0) {
		goto done;
	}

	HC_Open_Segment_By_Key(GetView()->GetViewKey());{
		float * start = transformed_points;
		float up[] = { 0, 1, 0 };
		int hit_count = HC_Compute_Selection_By_Ray(
			"v, selection level = entity, no related selection limit, no selection sorting",
			"./scene/overwrite",
			start, up);
		if (hit_count > 0) {
			ProcessSelectionResults(key, transformed_points);
		}
	}HC_Close_Segment();

done:
	delete [] transformed_points;
	delete [] path;

	return HOP_OK;
}


int HOpObjectClash::SelectByShell(HSmartSelItem * sel_item)
{
	HC_KEY seg = sel_item->GetKey();

	while (IsSpecialKey(seg))
		seg = HC_Show_Owner_By_Key(seg, 0);

	HC_Open_Segment_By_Key(seg);{
		vlist_s * keylist;
		HUtility::ShowContentsWithPath("shells, references", &keylist, true);
	
		START_LIST_ITERATION(HShowContentsWithPathType, keylist);{
			HShowContentsWithPathType *& key_path = temp;

			HC_KEY key = key_path->patharray[key_path->len - 1]; 		
			int point_count;
			int face_list_length;
	 		HC_Show_Shell_Size(key, &point_count, &face_list_length);
			if (point_count == 0)
				continue;

			float * original_points;
			HC_Show_Geometry_Pointer(key, "points", &original_points);

			int * face_list;
			HC_Show_Geometry_Pointer(key, "face list", &face_list);
			if (face_list == 0)
				continue;

			float modelling_matrix[16];
			HC_PShow_Net_Modelling_Matrix(key_path->len - 1, key_path->patharray, modelling_matrix);
			float * transformed_points = new float[3 * point_count];
			HC_Compute_Transformed_Points(point_count, original_points, modelling_matrix, transformed_points);

			HC_Open_Segment_By_Key(GetView()->GetViewKey());{
				//HC_Set_Heuristics("no related selection limit");
				int hit_count = HC_Compute_Selection_By_Shell(
					"v, selection level = entity, no related selection limit", "./scene/overwrite",
					point_count, transformed_points, face_list_length, face_list);
				if (hit_count > 0) {
					ProcessSelectionResults(key, transformed_points);
				} 
			}HC_Close_Segment();

			delete [] transformed_points;
			delete [] key_path->patharray;
			delete key_path;
		}END_LIST_ITERATION(keylist);

		delete_vlist(keylist);
	}HC_Close_Segment();

	return HOP_OK;
}




void HOpObjectClash::ClashDetect(HEventInfo & event)
{
	HC_Open_Segment_By_Key(GetView()->GetCuttingPlaneGeomKey());{
		HC_Flush_Contents(".", "geometry");
	}HC_Close_Segment();

	if (m_IntersectPolylinesVisible) {
		HC_Open_Segment_By_Key(GetView()->GetIntersectPolylinesKey());{
			HC_Flush_Contents(".", "geometry");
		}HC_Close_Segment();
	}

	HSelectionSet * selection = GetView()->GetSelection();
	unsigned int size = selection->GetSize();
	
	int keycount = 0;
	HC_KEY * keys = new HC_KEY[size];
	float * matrices = new float[size * 16];

	m_pClashSelection->DeSelectAll(); // Deselect everything from last time around
	HSmartSelItem * sel_item = 0;
	for (unsigned int i = 0; i < size; ++i) // Calculate and apply modelling matrices to our local selection list
	{
		HC_KEY key = selection->GetAt(i);
		char type[64];
		HC_Show_Key_Type(key, type);
		if (key != HC_ERROR_KEY && streq(type, "segment")) {
			sel_item = (HSmartSelItem *)selection->GetSelectionItemAt(i);

			float tmatrix[16];
			GetMatrix(
				event,
				sel_item->GetFullPathCount (GetView()),
				sel_item->GetFullPathKeys(GetView()), 
				tmatrix);

			while (IsSpecialKey(key))
				key = HC_Show_Owner_By_Key(key, 0);

			float seg_matrix[16];
			HC_Open_Segment_By_Key(key);{
				HC_Set_Rendering_Options ("simple shadow = auto");
				if (GetView()->GetSpritingMode())
					HC_Set_Heuristics("quick moves = spriting");

				if (HC_Show_Existence("modelling matrix")) {
					HC_Show_Modelling_Matrix(seg_matrix);
					HC_UnSet_Modelling_Matrix();
					HC_Set_Modelling_Matrix(tmatrix);
					HC_Append_Modelling_Matrix(seg_matrix);
				}
				else {
					HC_Compute_Identity_Matrix(seg_matrix);
					HC_Set_Modelling_Matrix(tmatrix);
				}
			}HC_Close_Segment();

			keys[i] = key;
			memcpy(&matrices[16 * i], seg_matrix, 16 * sizeof(float));
			keycount++;
		}
		switch (m_SelectMode) {
			case HOOC_Enclosure:
				SelectByEnclosure(sel_item);
				break;
			case HOOC_World:
				SelectByWorldVolume(sel_item);
				break;
			case HOOC_Screen:
				SelectByScreenVolume(sel_item);
				break;
			case HOOC_Shell:
				SelectByShell(sel_item);
				break;
		}	
	}

	m_ptLast = GetNewPoint();
	UpdateMousePos(event);

	if (!m_bInitialMatrix)
	{
		// Save original matrix and add to undo manager
		m_bInitialMatrix = true;
		HUndoItemMatrix * item = new HUndoItemMatrix(GetView(), keycount, keys, matrices);
		GetView()->GetUndoManager()->AddUndoItem(item);
	}

	GetView()->SetGeometryChanged();

	delete[] keys;
	delete[] matrices;
}



int HOpObjectClash::OnLButtonDownAndMove(HEventInfo & event)
{
	if (!OperatorStarted()) 
		return HOpObjectTranslate::OnLButtonDownAndMove(event);

	ClashDetect(event);
	GetView()->Update();
	return HOP_OK;
}



int HOpObjectClash::OnLButtonDown(HEventInfo & event)
{
	SetOperatorStarted(true);
	HOpObjectTranslate::OnLButtonDown(event);

	GrabSelectionSet();

	HC_Begin_Shell_Selection();

	ClashDetect(event);
	GetView()->ForceUpdate();

	return HOP_OK;
}



int HOpObjectClash::OnLButtonUp(HEventInfo & event)
{
	if (!OperatorStarted())
		return HOpObjectTranslate::OnLButtonUp(event); 

	HC_End_Shell_Selection();

	if (m_IntersectPolylinesVisible) {
		HC_Open_Segment_By_Key(GetView()->GetIntersectPolylinesKey());{
			HC_Flush_Contents(".", "geometry");
		}HC_Close_Segment();
	}

	RestoreSelectionSet();
	HOpObjectTranslate::OnLButtonUp(event);

	SetOperatorStarted(false);

	return HOP_READY;
}


void HOpObjectClash::SetSelectMode(int mode)
{
	char const * text;

	switch (mode) {
		case HOOC_Screen:
			text = "select mode = screen";
			break;
		case HOOC_Shell:
			text = "select mode = shell";
			break;
		case HOOC_World:
			text = "select mode = world";
			break;
		default:
			H_ASSERT(false);
			return; 
	}

	HC_Open_Segment_By_Key(m_TempSegKey);{
		HC_Open_Segment("select mode");{
			HC_Flush_Contents(".", "text");
			HC_Insert_Text(0, -0.9, 0, text);
		}HC_Close_Segment();
	}HC_Close_Segment();

	m_SelectMode = mode;
}

int HOpObjectClash::OnKeyDown(HEventInfo & event)
{
	char c = static_cast<char>(event.GetChar());

	switch (c) {
		case 'd': // for "driver"
			m_SelectMode = HOOC_Screen;
			break;
		case 'e': // for "enclosure"
			m_SelectMode = HOOC_Enclosure;
			break;
		case 'p':
			m_IntersectPolylinesVisible = !m_IntersectPolylinesVisible;
			break;
		case 's':
			m_SelectMode = HOOC_Shell;
			break;
		case 'w':
			m_SelectMode = HOOC_World;
			break;
		default:
			return HBaseOperator::OnKeyDown(event);
	}
	return HOP_OK;
}

