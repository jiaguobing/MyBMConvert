// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#include <stdio.h>
#include <math.h>
#include <string.h>

#include "HTools.h"

#include "HBaseModel.h"
#include "HBaseView.h"
#include "HBaseOperator.h"
#include "HEventInfo.h"
#include "HOpObjectTranslate.h"
#include "HSelectionSet.h"
#include "HSelectionItem.h"
#include "HUtilityGeomHandle.h"
#include "HUndoManager.h"
#include "vlist.h"


class HKeyList: public VList<HC_KEY>
{
};


void HOpObjectTranslate::Init()
{

	HRenderMode rndrmode = GetView()->GetRenderMode();
	if (rndrmode == HRenderHiddenLine)
		GetView()->SetRenderMode(HRenderHiddenLineFast);

	HSelectionSet* selection = GetView()->GetSelection();
	int i, sel_count = selection->GetSize();

	if(m_KeyList->Count())
		Cleanup();

	// we need two things for each selection
	// 1. DeInstance the selection item because we are going to make 
	// change to its position and don't want it to be accross all the includes
	// 2. Set spriting so that it's done quickly
	// REMEMBER: we need to DeInstance first and then apply any other settings (like spriting)
	//			because DeInstance is going to make changes and give us a new copy - Rajesh B (30-Sept-04)
	for(i=0; i<sel_count; ++i)
	{
		// de-instance (remove if shared)
		selection->DeInstance(i);

		// set spriting
		HC_KEY	sel_key = selection->GetAt(i);

		if( HUtility::IsSegmentKey(sel_key) )
		{
			m_KeyList->AddLast(sel_key);
			char token[MVO_BUFFER_SIZE], pathname[MVO_SEGMENT_PATHNAME_BUFFER];

			HC_Show_Segment(sel_key, pathname);
			HC_Parse_String(pathname, "/", -1, token); 
			if (!streq(token, "cutting grids"))
			{
				HC_Open_Segment_By_Key(sel_key);
					if ((GetView()->GetSpritingMode() || selection->GetHighlightMode()==HighlightQuickmoves) && 
						selection->GetHighlightMode() != ColoredInverseTransparency &&
						selection->GetHighlightMode() != InverseTransparency)
						HC_Set_Heuristics("quick moves = spriting");
				HC_Close_Segment();
			}
		}
	}
}


void HOpObjectTranslate::Cleanup()
{
	HRenderMode rndrmode = GetView()->GetRenderMode();
	if (rndrmode == HRenderHiddenLineFast)
		GetView()->SetRenderMode(HRenderHiddenLine);

	int i, count = m_KeyList->Count();

	// UNDO the spriting. We cannot UNDO instancing. 
	for(i=0; i<count; ++i)
	{
		// set spriting
		HC_KEY	sel_key = m_KeyList->RemoveFirst();

		char token[MVO_BUFFER_SIZE], pathname[MVO_SEGMENT_PATHNAME_BUFFER];

		HC_Show_Segment(sel_key, pathname);
		HC_Parse_String(pathname, "/", -1, token); 
		if (!streq(token, "cutting grids"))
		{
			HC_Open_Segment_By_Key(sel_key);
				if (HUtility::ShowOneHeuristic("quick moves"))
				{
					HC_UnSet_One_Heuristic("quick moves");
				}
			HC_Close_Segment();
		}
	}
	m_KeyList->Flush();
}

void HOpObjectTranslate::UpdateKeyList(void * data)
{
	UNREFERENCED(data);

	Cleanup();
	Init();
}

static bool update_key_list(int signal, void *signal_data, void *user_data)
{
	UNREFERENCED(signal);

	((HOpObjectTranslate *)user_data)->UpdateKeyList(signal_data);
	return true;
}
/////////////////////////////////////////////////////////////////////////////
// HOpObjectTranslate
//
// Translate items in the selection list in direction of mouse movement
// parallel to camera target plane.
//
// Currently supports translation if item if it is a segment

HOpObjectTranslate::HOpObjectTranslate(HBaseView* view, int DoRepeat,int DoCapture) : HBaseOperator(view, DoRepeat, DoCapture)
{
	m_bTemporarySelection = false;
	m_bInitialMatrix = false;
	m_KeyList = new HKeyList();

	m_cookie[0]=m_pView->SetSignalNotify(HSignalSelected, update_key_list, this);
	m_cookie[1]=m_pView->SetSignalNotify(HSignalDeSelectedOne, update_key_list, this);

	Init(); 
}




HOpObjectTranslate::~HOpObjectTranslate()
{
	Cleanup();

	//recompute the shadow plane
	m_pView->SetShadowMode(m_pView->GetShadowMode());

	m_pView->UnSetSignalNotify(m_cookie[0]);
	m_pView->UnSetSignalNotify(m_cookie[1]);

	delete m_KeyList;
}


HBaseOperator * HOpObjectTranslate::Clone()
{
	return new HOpObjectTranslate(GetView());
}



const char * HOpObjectTranslate::GetName() { return "HOpObjectTranslate"; }

 
/////////////////////////////////////////////////////////////////////////////
// HOpObjectTranslate message handlers



int HOpObjectTranslate::OnLButtonDown(HEventInfo &event)
{
	if (!OperatorStarted()) 
		SetOperatorStarted(true);
	
	int size=m_KeyList->Count();

	if (!size)
	{
		HPoint windowpos = event.GetMouseWindowPos();
		HUtility::SmartSelectUnderCursor(GetView(), windowpos.x, windowpos.y);
		m_bTemporarySelection = true;
	}
	
	if( !GetView()->GetAllowInteractiveShadows() && (m_TempShadowMode = GetView()->GetShadowMode()) != HShadowNone )
		GetView()->SetShadowMode(HShadowNone);
	if( !GetView()->GetAllowInteractiveCutGeometry() && (m_TempCutGeometryVisibility = GetView()->GetCutGeometryVisibility()) != HCutGeometryVisibilityNone)
		GetView()->SetCutGeometryVisibility(HCutGeometryVisibilityNone);
	SetNewPoint(event.GetMouseWindowPos());

	m_ptRecent.Set(GetNewPoint());
	m_ptRecentVP.Set(event.GetMouseViewpointPos());
	m_ptLast.Set(GetNewPoint());
	SetFirstPoint(event.GetMouseWindowPos());

	GetView()->Update();
	return HOP_OK;
}




int HOpObjectTranslate::GetMatrix(HEventInfo &event, 
								  int key_count,
								  HC_KEY const *full_path_keys, 
								  float *matrix_out)
{
	HPoint	oNewPoint, oLastPoint, translation;
	float identity[16] = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
	
	SetNewPoint(event.GetMouseWindowPos());

	oNewPoint=GetNewPoint();
	oLastPoint=m_ptLast;

	HC_Open_Segment_By_Key(full_path_keys[0]);
		// convert points to object space
	HPoint min,max,middle;
	HC_Compute_Circumcuboid(".",&min,&max);
	middle.Set((min.x + max.x)/2.0f,(min.y + max.y)/2.0f,(min.z + max.z)/2.0f);
	HC_Compute_Coordinates_By_Path (key_count, (HC_KEY *)full_path_keys, "object", &middle, "local window", &middle);
	oNewPoint.z = middle.z;
	oLastPoint.z = middle.z;
	HC_Compute_Coordinates_By_Path (key_count, (HC_KEY *)full_path_keys, "local window", &oNewPoint, "object", &oNewPoint);
	HC_Compute_Coordinates_By_Path (key_count, (HC_KEY *)full_path_keys, "local window", &oLastPoint, "object", &oLastPoint);

	translation = oNewPoint - oLastPoint;
		
		if (event.Shift()) 
		{
			HPoint u, v, proj_u, proj_v, pos, targ;
			double k;

			//convert to world coordinates
			HC_Compute_Coordinates_By_Path (key_count, (HC_KEY *)full_path_keys, "object", &translation, "world", &translation);
			
			//get the pertinent information
			HC_Open_Segment_By_Key(m_pView->GetSceneKey());
				HC_Show_Camera_Up_Vector(&u.x, &u.y, &u.z);
				HC_Show_Camera_Position(&pos.x, &pos.y, &pos.z);
				HC_Show_Camera_Target(&targ.x, &targ.y, &targ.z);
			HC_Close_Segment();
			v=targ-pos;
			
			//normalize the vectors
			k=HC_Compute_Vector_Length(&u);
			u.x=static_cast<float>(u.x/k);		u.y=static_cast<float>(u.y/k);		u.z=static_cast<float>(u.z/k);

			k=HC_Compute_Vector_Length(&v);
			v.x=static_cast<float>(v.x/k);		v.y=static_cast<float>(v.y/k);		v.z=static_cast<float>(v.z/k);

			//compute the projections onto u and v
			k=HC_Compute_Dot_Product(&translation, &u);
			proj_u.x=static_cast<float>(k*u.x);		
			proj_u.y=static_cast<float>(k*u.y);		
			proj_u.z=static_cast<float>(k*u.z);	
			translation=translation-proj_u;
			k=HC_Compute_Vector_Length(&translation);
			proj_v.x=static_cast<float>(k*v.x);		
			proj_v.y=static_cast<float>(k*v.y);		
			proj_v.z=static_cast<float>(k*v.z);
			translation=proj_u+proj_v;

			//convert to object space
			HC_Compute_Coordinates_By_Path (key_count, (HC_KEY *)full_path_keys, "world", &translation, "object", &translation);
		} 
	
	HC_Close_Segment();
	
	memcpy( matrix_out, identity, 16 * sizeof( float ) );
	matrix_out[12] = translation.x;
	matrix_out[13] = translation.y;
	matrix_out[14] = translation.z;
	
	return HOP_OK;
}

	


int HOpObjectTranslate::UpdateMousePos(HEventInfo &event)
{
	m_ptRecent = event.GetMouseWorldPos();
	m_ptRecentVP = event.GetMouseViewpointPos();
	return HOP_OK;
}


int HOpObjectTranslate::OnLButtonDownAndMove(HEventInfo &event)
{
	int n;
	char keytype[MVO_BUFFER_SIZE];
	float seg_matrix[16], tmatrix[16], cmatrix[16];
	HSelectionSet* selection = GetView()->GetSelection();
	int size = m_KeyList->Count();
	
	if (!OperatorStarted()) 
		return HBaseOperator::OnLButtonDownAndMove(event);

	int keycount = 0;
	HC_KEY *keys = new HC_KEY[size];
	float *matrices = new float[size * 16];
	
	for (n = 0; n < size; n++)
	{
		HC_KEY sel_key = m_KeyList->NthItem(n);
		HSelectionItem *sel_item = (HSelectionItem *)selection->GetSelectionItem(sel_key);

		assert(sel_item);

		HC_Show_Key_Type(sel_key, keytype);
		if ((sel_key != INVALID_KEY) && (streq(keytype, "segment")))
		{
			while( IsSpecialKey( sel_key ))
				sel_key = HC_KShow_Owner_By_Key( sel_key );
			GetMatrix( event, sel_item->GetFullPathCount (GetView()), 
					(HC_KEY *)sel_item->GetFullPathKeys (GetView()), tmatrix);

			HC_Open_Segment_By_Key(sel_key);
				if (HC_Show_Existence ("modelling matrix")) 
				{
					HC_Show_Modelling_Matrix (seg_matrix);
					HC_Compute_Matrix_Product (tmatrix, seg_matrix, cmatrix);
					HC_Set_Modelling_Matrix (cmatrix);
				}
				else 
				{
					memset(seg_matrix,0,16*sizeof(float));
					seg_matrix[0] = seg_matrix[5] = seg_matrix[10] = seg_matrix[15] = 1;
					HC_Set_Modelling_Matrix( tmatrix );
				}
			HC_Close_Segment();			

			keys[n] = sel_key;
			memcpy(&matrices[n*16],seg_matrix,16*sizeof(float));
			keycount++;
		}
	}	
	m_ptLast = GetNewPoint();
	UpdateMousePos(event);


	//save original matrix, and add to undo manager
	if(!m_bInitialMatrix)
	{
		m_bInitialMatrix = true;
		HUndoItemMatrix *item = new HUndoItemMatrix(GetView(), keycount, keys, matrices);
		GetView()->GetUndoManager()->AddUndoItem(item);
	}
	delete [] keys;
	delete [] matrices;

	GetView()->Update();
	return HOP_OK;
}


int HOpObjectTranslate::OnLButtonUp(HEventInfo &event)
{
	if(!OperatorStarted()) 
		return HBaseOperator::OnLButtonUp(event);
	SetOperatorStarted(false);

	if( !GetView()->GetAllowInteractiveCutGeometry() && m_TempCutGeometryVisibility != HCutGeometryVisibilityNone)
		GetView()->SetCutGeometryVisibility(m_TempCutGeometryVisibility);
	/* Drawing the shadows and cut geometry over again takes a long time, lets take them one at a time... */ 
	//GetView()->ForceUpdate();

	if( !GetView()->GetAllowInteractiveShadows() && (m_TempShadowMode = GetView()->GetShadowMode()) != HShadowNone )
		GetView()->SetShadowMode(m_TempShadowMode);
		
	HC_Control_Update_By_Key( GetView()->GetViewKey(), "set derived geometry regeneration" );

	if (m_bTemporarySelection)
	{
		Cleanup();
		GetView()->GetSelection()->DeSelectAll();
		m_bTemporarySelection = false;
	}
	m_bInitialMatrix = false;
	GetView()->SetGeometryChanged();
	GetView()->EmitSetModellingMatrixMessage();
	GetView()->Update();

	return HOP_READY;
}



int HOpObjectTranslate::OnLButtonDblClk (HEventInfo &hevent)
{
	int n, i;
	char keytype[MVO_BUFFER_SIZE];
	HUndoItem *item;
	HUndoManager *umanager = GetView()->GetUndoManager();

	HOpObjectTranslate::OnLButtonDown(hevent);
	
	int size = m_KeyList->Count();
	int actualkeycount = 0;
	int selkeycount = 0;
	HC_KEY *actualkeys = new HC_KEY[size];
	HC_KEY *selkeys = new HC_KEY[size];
	float *oldmatrices = new float[size * 16];
	float *matrices = new float[size * 16];

	//build a list of all the valid keys
	for (n = 0; n < size; n++)
	{
		HC_KEY sel_key = m_KeyList->NthItem(n);

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
	return HOpObjectTranslate::OnLButtonUp(hevent);
}
