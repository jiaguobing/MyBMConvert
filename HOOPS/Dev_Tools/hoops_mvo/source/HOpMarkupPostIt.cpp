// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// HOpMarkupPostIt.cpp : implementation of the HOpMarkupPostIt class
//
#include "HBaseModel.h"
#include "HBaseView.h"
#include "HBaseOperator.h"
#include "HEventInfo.h"
#include "HTools.h"
#include "HOpMarkupPostIt.h"
#include "HUtilityAnnotation.h"
#include "HUtilityAnnotationMeasure.h"
#include "HMarkupManager.h"
 
#include "stdio.h"
#include <math.h>
#include <string.h>
#include <stdlib.h>

const char * HOpMarkupPostIt::GetName() { return "HOpMarkupPostIt"; }

HOpMarkupPostIt::HOpMarkupPostIt(HBaseView* view, int DoRepeat,int DoCapture) : HOpMarkupAnnotate(view, true, DoRepeat, DoCapture)
{
}


/////////////////////////////////////////////////////////////////////////////
// HOpMarkupAnnotate message handlers
int HOpMarkupPostIt::OnLButtonDown(HEventInfo &event)
{
	HPoint pos = event.GetMouseWindowPos();
	int count, dummy;
	float fdummy;

	if(OperatorStarted() && m_Annotation.GetTextLength() == 0){
		EndOp();
		HC_Delete_By_Key(m_Annotation.GetKey());
		GetView()->Update();
	}

	HC_Open_Segment_By_Key(GetView()->GetViewKey());{
		count = HC_Compute_Selection(".", ".", "v, selection level = entity", pos.x, pos.y);
	}HC_Close_Segment();


	HC_KEY key = INVALID_KEY;
	if(count > 0){
		HC_Show_Selection_Element(&key, &dummy, &dummy, &dummy);
		HC_Show_Selection_Position(&fdummy, &fdummy, &fdummy,
			&pos.x, &pos.y, &pos.z);
	} else {
		pos = event.GetMouseWorldPos();
	}

	if(HUtilityAnnotationMeasure::IsMeasurement(key)){
		EndOp();
		return HOP_OK;
	}

	EndOp();
	if(HUtilityAnnotation::IsNote(key)){
		/* This create an annotation class for an existing annotation in the model */
		m_Annotation.SetNote(key, GetView()->GetViewKey());
	} else {
		/* Put these notes in a markup layer. */
		HC_KEY markup_layer = GetView()->GetMarkupManager()->GetCurrentLayerKey();
		if(markup_layer == INVALID_KEY)
			markup_layer = GetView()->GetMarkupManager()->OpenLayer("");

		/* This creates a new annotation */
		m_Annotation.Insert(pos, pos, markup_layer, GetView(), INVALID_KEY, 1, false);
		m_Annotation.SetOnTop(true);
	}
	m_Annotation.Resize(0, 0, GetView()->GetViewKey()); 

	StartOp();
	GetView()->Update();

	return HOP_OK;
}

