
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "HTools.h"
#include "hc.h"
#include "hic.h"
#include "HImManager.h"
#include "HBaseModel.h"
#include "HBaseView.h"
#include "HBaseOperator.h"
#include "HEventInfo.h"
#include "MyFreehandMarkupOp.h"
#include "HMarkupManager.h"


MyFreehandMarkupOp::MyFreehandMarkupOp(HBaseView* view, int DoRepeat, int DoCapture) : HBaseOperator(view, DoRepeat, DoCapture)
{
	m_bOpStarted = false;
	m_LastKey = -1;
}


HBaseOperator * MyFreehandMarkupOp::Clone()
{
	return (HBaseOperator *)new MyFreehandMarkupOp(m_pView);
}


const char * MyFreehandMarkupOp::GetName() { return "MyFreehandMarkupOp"; }

long MyFreehandMarkupOp::markup_incarnation = 0;

#define DEBUG_PRINTF
void MyFreehandMarkupOp::MarkupSelection(HEventInfo &event)
{
	HC_KEY key;
	int offset1, offset2, offset3, count;
	HPoint window, world;
	char owner[256];
	m_ptNew = event.GetMouseWindowPos();

	HC_Open_Segment_By_Key(m_pView->GetViewKey());{
		count = HC_Compute_Selection(".", ".", "v", m_ptNew.x, m_ptNew.y);
		if(count > 0){
			/* Got something!  Either start and inscription or continue the last one. */
			HC_Show_Selection_Element(&key, &offset1, &offset2, &offset3);
			if(key != m_LastKey){
				++markup_incarnation;
			} else {
			}
			HC_Show_Selection_Position(&window.x, &window.y, &window.z, &world.x, &world.y, &world.z);
			HC_Show_Owner_By_Key(key, owner);
			HC_Open_Segment(owner);{
				char markup_seg[256];
				snprintf(markup_seg, sizeof(markup_seg), "markup %ld", markup_incarnation);
				HC_Open_Segment(markup_seg);{
					HC_Set_Visibility("off, lines=on");
					HC_Set_Selectability("off");
					HC_Insert_Ink(world.x, world.y, world.z);
				}HC_Close_Segment();
			}HC_Close_Segment();
		} else {
			/* Nothing selected. Either do nothing or end the last inscription */
			key = -1;
		}
	}HC_Close_Segment();
	m_LastKey = key;
}

int MyFreehandMarkupOp::OnLButtonDown(HEventInfo &event)
{
	MarkupSelection(event);
	
	m_bOpStarted = true;

	return (HOP_OK);
}



int MyFreehandMarkupOp::OnLButtonDownAndMove(HEventInfo &event)
{
	if (!m_bOpStarted) 
		return HBaseOperator::OnNoButtonDownAndMove(event);

	MarkupSelection(event);

    m_pView->Update();    

	return (HOP_OK);
}



int MyFreehandMarkupOp::OnLButtonUp(HEventInfo &event)
{
	++markup_incarnation;
	return(HOP_READY);
}


void MyFreehandMarkupOp::CreateMarkupFreehand(HBaseView *pView, const char *segment, unsigned int PolyLineLength, HPoint *pPolyline, bool emit_message)
{
#if 0
	HMarkupManager *markupManager;
	HC_KEY freehand=0;
	if ((markupManager = pView->GetMarkupManager()))
	{
		HC_KEY activelayerkey;
		if ((activelayerkey = markupManager->GetCurrentLayerKey()) == -1)
			activelayerkey = markupManager->OpenLayer("");
		
		HC_Open_Segment_By_Key(activelayerkey);
 			freehand = HC_KOpen_Segment (segment);
				// this is now hoops default behaviour but might be useful for "no absolute line weight"
				//HImSetCallback("draw dc polyline","HImUtility_device_independent_polyline_weights");
				HC_Insert_Polyline(PolyLineLength,pPolyline);
				// let's set a round end cap so that we 
				// don't get spikes in the polyline
				HC_Set_Line_Pattern("(----)");
 			HC_Close_Segment();
		HC_Close_Segment();	
	}

	if (freehand && emit_message)
		markupManager->EmitMarkupSegment(freehand);
#endif
}
