// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#include "HUndoManager.h"
#include "HBaseView.h"
#include <math.h>
#include "string.h"
#include "stdio.h"

#include "vhash.h"
#include "vdlist.h"
#include "HEventInfo.h"

HUndoManager::HUndoManager()
{
	m_pUndoItemList = new_vdlist(malloc, free);
	m_CurrentItem = vdlist_new_unique_cursor(m_pUndoItemList);
}

HUndoManager::~HUndoManager()
{
	Flush();
	vdlist_delete_unique_cursor(m_pUndoItemList, m_CurrentItem);
	delete_vdlist(m_pUndoItemList);
}

void HUndoManager::AddUndoItem(HUndoItem *item)
{
	HUndoItem * victim = 0;
	while ((victim = (HUndoItem *)vdlist_remove_unique_cursor_next(m_pUndoItemList, m_CurrentItem)) != 0)
		delete victim;

	vdlist_add_last(m_pUndoItemList, item);
	vdlist_reset_unique_cursor_back(m_pUndoItemList, m_CurrentItem);
}

int HUndoManager::Undo(int steps, bool testOnly, bool setupOnly)
{
	/* fast paths for test-only */
	if (m_CurrentItem->state == VDLIST_BEGIN)
		return 0;
	else if (testOnly) {
		if (steps <= m_CurrentItem->index + 1)
			return steps;
		else
			return static_cast<int>(m_CurrentItem->index + 1);
	}

	int undosperformed = 0; 
	HUndoItem * temp;

	while ((temp = (HUndoItem *)vdlist_peek_unique_cursor(m_pUndoItemList, m_CurrentItem)) != 0)
	{
		temp->Undo(setupOnly);
		vdlist_rewind_unique_cursor(m_pUndoItemList, m_CurrentItem);
		if(++undosperformed == steps) break;
	}

	return undosperformed;
}

int HUndoManager::Redo(int steps, bool testOnly, bool setupOnly)
{
	/* fast paths */
	if (vdlist_peek_unique_cursor(m_pUndoItemList, m_CurrentItem) == vdlist_peek_last(m_pUndoItemList) || 
		vdlist_count(m_pUndoItemList) == 0)
		return 0;
	else if (testOnly) {
		if (steps <= vdlist_count(m_pUndoItemList) - m_CurrentItem->index)
			return steps;
		else
			return static_cast<int>(vdlist_count(m_pUndoItemList) - (m_CurrentItem->index + 1));
	}

	int redosperformed = 0; 
	HUndoItem * temp;

	while ((temp = (HUndoItem *)vdlist_peek_unique_cursor_next(m_pUndoItemList, m_CurrentItem)) != 0)
	{
		temp->Redo(setupOnly);
		vdlist_advance_unique_cursor(m_pUndoItemList, m_CurrentItem);
		if(++redosperformed == steps) break;
	}

	return redosperformed;
}

void HUndoManager::Flush()
{
	HUndoItem * temp;
	while ((temp = (HUndoItem *)vdlist_remove_first(m_pUndoItemList)) != 0)
		delete temp;
	vdlist_flush(m_pUndoItemList);
	vdlist_reset_unique_cursor(m_pUndoItemList, m_CurrentItem);
}

HUndoItem * HUndoManager::Peek()
{
	return static_cast<HUndoItem *>(vdlist_peek_unique_cursor(m_pUndoItemList, m_CurrentItem));
}

void HUndoManager::Reset(HBaseView *view)
{
	HUndoItem *item, *saved_curr_item;
	HCamera *camera = 0;
	int i = 0, curr_index = 0, keycount = 0;
	float *matrices, *oldmatrices;

	HC_KEY *keys;
	vhash_t *table;

	//nothing to do
	if (m_CurrentItem->state == VDLIST_BEGIN)
		return;

	//set continuous mode to prevent camera changes from updating the scene at a bad time
	ViewUpdateMode old_mode = view->GetViewUpdateMode();
	if(old_mode == OnDemand)
		view->SetViewUpdateMode(Continuous);

	//get current location of undo sequence
	saved_curr_item = Peek();

	//get maximum number of possible keys, and allocate memory for list of keys
	BeginUndoItemIteration();
	while((item = GetNextUndoItem()) != 0)
	{
		//matrices might have multiple keys
		if(streq(item->GetName(),"HUndoItemMatrix"))
			keycount += ((HUndoItemMatrix *)item)->GetCount();
		else
			keycount++;
		if(item == saved_curr_item)
			break;
	}
	EndUndoItemIteration();

	keys = new HC_KEY[keycount];
	matrices = new float[keycount * 16];
	oldmatrices = new float[keycount * 16];
	table = new_vhash(keycount, malloc, free);
	
	//1st pass:
	//----------------------------------------------------//
	//iterate through entire undo list, get list of unique geometry keys and first camera setting
	//build up list of pointers to each undo object
	BeginUndoItemIteration();
	while((item = GetNextUndoItem()) != 0)
	{
		if(streq(item->GetName(), "HUndoItemMatrix"))
		{
			HC_KEY *key = ((HUndoItemMatrix *)item)->GetKey();
			int count = ((HUndoItemMatrix *)item)->GetCount();

			for(i = 0; i < count; i++)
			{
				//see if key already exists in hash table; if not, then add item to table
				if(vhash_lookup_item(table, (void *)key[i], 0) != VHASH_STATUS_SUCCESS)
				{
					vhash_insert_item(table, (void *) key[i], 0);
					
					//now add to list
					keys[curr_index] = key[i];
					memcpy(&matrices[curr_index * 16], ((HUndoItemMatrix *)item)->GetMatrix()+16*i, 16*sizeof(float));
					curr_index++;
				}
			}
		}
		else if(camera == 0 && streq(item->GetName(), "HUndoItemCamera"))
		{
			camera = new HCamera();
			memcpy(camera, ((HUndoItemCamera *)item)->GetCamera(), sizeof(HCamera));
		}

		if(item == saved_curr_item)
			break;
	}
	EndUndoItemIteration();

	//apply matrices
	//----------------------------------------------------//
	//first, save the current keys/matrices, so we can restore them for the smooth transition
	for(i = 0; i < curr_index; i++)
	{
		HC_Open_Segment_By_Key(keys[i]);
			HC_Show_Modelling_Matrix(&oldmatrices[i*16]);
		HC_Close_Segment();
	}
	//save the old camera, so we can restore for smooth transition
	HCamera oldcamera;
    view->GetCamera(&oldcamera);
	
	//move undo pointer back to beginning, properly modifying the undo values along the way
	//this will bash current settings, so we'll need to restore aftewards for smooth transitions
	while(Undo(1,false,true));	


	//if smooth transitions, restore camera so can render the smooth transitions
	if(view->GetSmoothTransition())
		view->SetCamera(oldcamera);

	//pass list of geometry keys and their corresponding matrices to utility function to 
	//smoothly transition all to original positions
	if(view->GetSmoothTransition())
	{
		//if doing smooth transitions, need to restore the matrices (from before we bashed them)
		for(i = 0; i < curr_index; i++)
		{
			HC_Open_Segment_By_Key(keys[i]);
				HC_Set_Modelling_Matrix(&oldmatrices[i*16]);
			HC_Close_Segment();
		}

		HC_Open_Segment_By_Key(view->GetSceneKey());
			HShadowMode oldmode = view->GetShadowMode();
			view->SetShadowMode(HShadowNone);
			HUtility::MultiSmoothTransitions(view, curr_index, keys, matrices, camera);
			view->SetShadowMode(oldmode);
		HC_Close_Segment();

		//set final modelling matrix; this will force a shadow update
		for(i = 0; i < curr_index; i++)
		{
			HC_Open_Segment_By_Key(keys[i]);
				HC_Set_Modelling_Matrix(&matrices[i*16]);
			HC_Close_Segment();
		}
	}

	view->SetGeometryChanged();
	view->ForceUpdate();

	//now restore the original mode
	if(old_mode != view->GetViewUpdateMode())
		view->SetViewUpdateMode(OnDemand);

	if(camera)
		delete camera;
	delete [] keys;
	delete [] matrices;
	delete [] oldmatrices;
	delete_vhash(table);
}

 
/* the primary cursor of the vdlist is only be used for this iteration */
void HUndoManager::BeginUndoItemIteration()
{
	vdlist_reset_cursor(m_pUndoItemList);
}

HUndoItem * HUndoManager::GetNextUndoItem()
{
	HUndoItem *item = (HUndoItem *)vdlist_peek_cursor(m_pUndoItemList);
	if (item != 0)
		vdlist_advance_cursor(m_pUndoItemList);
	return item;
}

void HUndoManager::EndUndoItemIteration()
{
}

  
 

//HUndoItemCamera
HUndoItemCamera::HUndoItemCamera(HBaseView *view)
{
    m_pView = view;
    view->GetCamera(&m_Camera);
 
}


void HUndoItemCamera::Undo(bool setupOnly)
{
	HCamera oldcamera;
    m_pView->GetCamera(&oldcamera);

	if(!setupOnly)
	{
		if (m_pView->GetSmoothTransition())
		{
			HC_Open_Segment_By_Key(m_pView->GetSceneKey());
			HUtility::SmoothTransition(oldcamera, m_Camera, m_pView);
			HC_Close_Segment();
		}
	}

	m_pView->SetCamera(m_Camera);
    m_Camera = oldcamera;
}

void HUndoItemCamera::Redo(bool setupOnly)
{
	HCamera oldcamera;
    m_pView->GetCamera(&oldcamera);

	if(!setupOnly)
	{
		if (m_pView->GetSmoothTransition())
		{
			HC_Open_Segment_By_Key(m_pView->GetSceneKey());
			HUtility::SmoothTransition(oldcamera, m_Camera, m_pView);
			HC_Close_Segment();
		}
	}

	m_pView->SetCamera(m_Camera);
    m_Camera = oldcamera;
}


// HUndoItemMatrix
HUndoItemMatrix::HUndoItemMatrix(HBaseView *view, int num_items, HC_KEY *keys, float *matrices)
{
	m_pView = view;
	m_Count = num_items;

	m_SegmentKey = new HC_KEY[m_Count];
	memcpy(m_SegmentKey, keys, m_Count*sizeof(HC_KEY));
	
	m_Matrix = new float[m_Count * 16];
	memcpy(m_Matrix, matrices, m_Count * 16 * sizeof(float));
}

HUndoItemMatrix::~HUndoItemMatrix()
{ 
	if(m_Matrix) 
		delete [] m_Matrix; 

	if(m_SegmentKey)
		delete [] m_SegmentKey;
};

void HUndoItemMatrix::GetMatrix(float *matrices)
{
	memcpy(matrices, m_Matrix, m_Count * 16 * sizeof(float));
}

void HUndoItemMatrix::GetKey(HC_KEY *keys)
{
	memcpy(keys,m_SegmentKey,m_Count*sizeof(HC_KEY));
}

void HUndoItemMatrix::Undo(bool setupOnly)
{
	int i = 0;
	float *oldmatrix = new float[m_Count * 16];
	
	HShadowMode oldmode;
	oldmode = m_pView->GetShadowMode();
	m_pView->SetShadowMode(HShadowNone);

	for(i = 0; i < m_Count; i++)
	{
		HC_Open_Segment_By_Key(m_SegmentKey[i]);
			HC_Show_Modelling_Matrix(&oldmatrix[i*16]);
		HC_Close_Segment();
	}

	if(!setupOnly)
	{
		if (m_pView->GetSmoothTransition())
			HUtility::MultiSmoothTransitions(m_pView, m_Count, m_SegmentKey, m_Matrix);
	}

	for(i = 0; i < m_Count; i++)
	{
		HC_Open_Segment_By_Key(m_SegmentKey[i]);
		HC_Set_Modelling_Matrix(&m_Matrix[i*16]);
		HC_Close_Segment();
	}

	delete [] m_Matrix;
	m_Matrix = oldmatrix;
	m_pView->SetShadowMode(oldmode);

	if(!setupOnly)
		m_pView->ForceUpdate();
}

void HUndoItemMatrix::Redo(bool setupOnly)
{
	int i = 0;
	float *oldmatrix = new float[m_Count * 16];
	
	HShadowMode oldmode;
	oldmode = m_pView->GetShadowMode();
	m_pView->SetShadowMode(HShadowNone);

	for(i = 0; i < m_Count; i++)
	{
		HC_Open_Segment_By_Key(m_SegmentKey[i]);
			HC_Show_Modelling_Matrix(&oldmatrix[i*16]);
		HC_Close_Segment();
	}

	if(!setupOnly)
	{
		if (m_pView->GetSmoothTransition())
			HUtility::MultiSmoothTransitions(m_pView, m_Count, m_SegmentKey, m_Matrix);
	}

	for(i = 0; i < m_Count; i++)
	{
		HC_Open_Segment_By_Key(m_SegmentKey[i]);
		HC_Set_Modelling_Matrix(&m_Matrix[i*16]);
		HC_Close_Segment();
	}

	delete [] m_Matrix;
	m_Matrix = oldmatrix;
	m_pView->SetShadowMode(oldmode);

	if(!setupOnly)
		m_pView->ForceUpdate();
}
