// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#include "HBaseModel.h"
#include "HBaseView.h"
#include "HBaseOperator.h"
#include "HEventInfo.h"
#include "HTools.h"
#include "HOpMarkupAnnotate.h"
#include "HUtilityAnnotation.h"
#include "HUtilityAnnotationMeasure.h"
#include "HImHideOverlappedText.h"
#include "HEventManager.h"
#include "HSelectionSet.h"

 
#include "stdio.h"
#include <math.h>
#include <string.h>
#include <stdlib.h>



HOpMarkupAnnotate::HOpMarkupAnnotate(HBaseView* view, bool AddToLayer, int DoRepeat,int DoCapture, bool create_new_notes) : HBaseOperator(view, DoRepeat, DoCapture)
{
	m_iBackgroundType = BG_QUAD;
 	m_pNoteKey = 0;
	m_bAddToLayer = AddToLayer;
	m_bCreateNewNotes = create_new_notes;
	view->SetAnnotationResize(true);
}

HOpMarkupAnnotate::~HOpMarkupAnnotate()
{
	EndOp();
}


HBaseOperator * HOpMarkupAnnotate::Clone()
{
	return new HOpMarkupAnnotate(GetView(), m_bAddToLayer);
}

 
const char * HOpMarkupAnnotate::GetName() { return "HOpMarkupAnnotate"; }

 
 
/////////////////////////////////////////////////////////////////////////////
// HOpMarkupAnnotate message handlers



int HOpMarkupAnnotate::OnLButtonDown(HEventInfo &event)
{
	HPoint pos = event.GetMouseWindowPos();
	int count, dummy;
	float fdummy;

	if(OperatorStarted() && m_Annotation.GetTextLength() == 0){
		EndOp();
		HC_Delete_By_Key(m_Annotation.GetKey());
		GetView()->Update();
	}

	/* we always want to use entity level selection */
	HC_Open_Segment_By_Key(GetView()->GetViewKey());

		count = HC_Compute_Selection(".", ".", "v, selection level = entity", pos.x, pos.y);

	HC_Close_Segment();
	
	if(count == 0){
		EndOp();
		return HOP_CANCEL;
	}

	HC_KEY keys[128];
	int key_path_length;
	HC_Show_Selection_Original_Keys(&key_path_length, keys);

	HC_KEY key;
	HC_Show_Selection_Element(&key, &dummy, &dummy, &dummy);
	HC_Show_Selection_Position(&fdummy, &fdummy, &fdummy, &pos.x, &pos.y, &pos.z);

	char type[1024];
	HC_Show_Key_Type(key, type);
	
	if(streq(type, "segment")){
		EndOp();
		return HOP_CANCEL;
	}

	if(HUtilityAnnotationMeasure::IsMeasurement(key)){
		EndOp();
		return HOP_CANCEL;
	}

	EndOp();
	if(HUtilityAnnotation::IsNote(key)){
		/* This creates an annotation class for an existing annotation in the model */
		m_Annotation.SetNote(key, GetView()->GetViewKey());
	} else if (m_bCreateNewNotes){
		/* If the selected item is a geometry reference, insert annotation in reference segment */
		bool reference = false;
		for (int i = 0; i < key_path_length; i++) {
			HC_Show_Key_Type(keys[i], type);
			if (streq(type, "reference")) {
				key = keys[i];
				reference = true;
				break;
			}
		}

		/* This creates a new annotation */
		char segment[1024*4];
		HC_Show_Owner_By_Key(key, segment);
		HC_KEY seg_key = HC_KOpen_Segment(segment);{
		}HC_Close_Segment();
		m_Annotation.Insert(pos, pos, seg_key, GetView(), INVALID_KEY, BG_QUAD, true, keys, key_path_length);

		if (reference) {
			bool ref_matrix = false;
			float matrix[16];
			HC_Open_Geometry(key);
			if (HC_Show_Existence("modelling matrix")) {
				HC_Show_Modelling_Matrix(matrix);
				ref_matrix = true;
			}
			HC_Close_Geometry();

			if (ref_matrix) {
				HC_Open_Segment_By_Key(m_Annotation.GetKey());
				HC_Set_Modelling_Matrix(matrix);
				HC_Close_Segment();
			}
		}
	} else {
		return HOP_OK;
	}
	StartOp();
	m_Annotation.Resize(GetView()->GetIncludeLinkKey(), GetView()->GetViewKey()); 
	GetView()->Update();

	return HOP_OK;
}


int HOpMarkupAnnotate::OnLButtonDownAndMove(HEventInfo &event)
{
	if(OperatorStarted()){
		HPoint pos = event.GetMouseWorldPos();
		//AdjustPositionToTextPlane(pos);
		HPoint plane_point;
		m_Annotation.ShowPosition(plane_point);
		HUtility::AdjustPositionToPlane(GetView(), pos, plane_point);
		HCamera cam;
		m_pView->GetCamera(&cam);
		m_Annotation.MoveText(pos, &cam);
		m_Annotation.Resize(GetView()->GetIncludeLinkKey(), GetView()->GetViewKey());
		GetView()->Update();
	}
	return HOP_OK;
}

bool HOpMarkupAnnotate::EndOp()
{
	/* This assumes the annotation is still valid. */
	bool retval = OperatorStarted();
	if(OperatorStarted()){

		SetOperatorStarted(false);

		HC_Open_Segment_By_Key(m_Annotation.GetKey());{
			HC_UnSet_One_Heuristic("quick moves");
			HC_UnSet_One_Rendering_Option("depth range");
		}HC_Close_Segment();

		m_Annotation.SetColor("lines = red, edges = red");
		GetView()->Update();

		if(m_bHideOverlappedText){
			HC_Open_Segment_By_Key(GetView()->GetSceneKey());{
				HImHideOverlappedText::FilterOn();
			}HC_Close_Segment();
		}	
	}
	return retval;
}

void HOpMarkupAnnotate::StartOp()
{
	SetOperatorStarted(true);
	m_bTouchedText = false;
	m_Annotation.SetColor("lines = yellow, edges = yellow");
	HC_Open_Segment_By_Key(m_Annotation.GetKey());{
		HC_Set_Heuristics("quick moves");
	  	HC_Set_Rendering_Options("depth range=(0,0.1)");
 	}HC_Close_Segment();

	if((m_bHideOverlappedText = HImHideOverlappedText::IsFilterOn()) != 0){
		HC_Open_Segment_By_Key(GetView()->GetSceneKey());{
			HImHideOverlappedText::FilterOff();
		}HC_Close_Segment();
	}
}

int HOpMarkupAnnotate::OnRButtonDown(HEventInfo &event)
{
	UNREFERENCED(event);

	EndOp();
	return HOP_OK;
}

int HOpMarkupAnnotate::OnLButtonUp(HEventInfo &event)
{
	UNREFERENCED(event);

	return HOP_OK;
}


int
HOpMarkupAnnotate::OnKeyDown(HEventInfo &event)
{
	bool please_update = false;

	if (OperatorStarted()){

		char the_char = static_cast<char>(event.GetChar());
		
		switch((int)the_char){

		case '\r':		
			// is this a return
			if(!m_bTouchedText){
				m_bTouchedText = true;
				m_Annotation.SetText("");
			}
			m_Annotation.AddChar('\n');		
			please_update = true;
			break;
		
		case '\b':
			// erase
			if(m_Annotation.GetTextLength() > 0){
				// terminate the string one char shorter
				m_Annotation.AddChar('\b');
				m_bTouchedText = true;
			} else {
				EndOp();
				HC_Delete_By_Key(m_Annotation.GetKey());
			}
			please_update = true;
			break;

		default:{
				if(event.Control()){
					if(the_char == ' '){
						m_iBackgroundType = m_iBackgroundType == BG_NONE ? 0 : m_iBackgroundType +1;
						m_Annotation.SetBackgroundType(m_iBackgroundType);
						please_update = true;
					}
				}
				else if	((the_char >= ' ') && (the_char <= '~')) {
					if(!m_bTouchedText){
						m_bTouchedText = true;
						m_Annotation.SetText("");
					}
					m_Annotation.AddChar(the_char);		
					please_update = true;
				}

				}break;
		}//switch
	}

	if(please_update){
		if(OperatorStarted())
			m_Annotation.Resize(GetView()->GetIncludeLinkKey(), GetView()->GetViewKey());
		GetView()->Update();
	}
	
	return HOP_OK;
}


void
HOpMarkupAnnotate::TransmitMarkupSegment()
{
}

