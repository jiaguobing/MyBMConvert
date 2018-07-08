// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// HOpObjectAnnotate.cpp : implementation of the HOpObjectAnnotate class
//

#include <math.h>
#include <string.h>
#include <stdio.h>

#include "HTools.h"

#include "hc.h"
#include "hic.h"

#include "HImManager.h"


#include "HBaseModel.h"
#include "HBaseView.h"
#include "HBaseOperator.h"
#include "HEventInfo.h"
#include "HOpObjectAnnotate.h"
#include "HUtility.h"
#include "HSharedKey.h"




/////////////////////////////////////////////////////////////////////////////
// HOpObjectAnnotate
//
// Operator for annotating geometry in the scene.
//
// Left button down:
//		Check if the previous text input was finished with a carriage return.  If 
//		not terminate the previous text input.
//		Determine if we have starting or finishing the annotation line.
//		if starting
//			Check if any geometry is under the mouse and if so find use the intersection
//			as a starting point for the anotation line.  
//		if ending
//			draw line in the text subsegment of the segment in which the geometry resides
//
// NoButtonDownAndMove:
//		Rubber band line from last point to current point 
//
// OnKeyDown
//		Look at the key and determine what to do with the key depending on whether
//		its an erase, a carriage return or a printable characted 

// HOpObjectAnnotate.cpp : implementation of the HOpObjectAnnotate
//

HC_KEY *  HOpObjectAnnotate::m_pSelectionPath = 0;
int HOpObjectAnnotate::m_iPathKeysCount = 0;


HOpObjectAnnotate::HOpObjectAnnotate(HBaseView* view, int DoRepeat, int DoCapture) : HBaseOperator(view, DoRepeat, DoCapture)
{

	SetOperatorStarted(false);
	m_pString = 0;
	m_bObject = false;

	/*
	// initialise to the identity matrix
	for (int i=0; i< 16; i++)
	{
		if (i%5 == 0)
			m_ModellingMatrix[i] = 1.0f;
		else
			m_ModellingMatrix[i] = 0.0f;
	}
	*/

	HC_Open_Segment_By_Key(GetView()->GetConstructionKey());	
		HC_Set_Text_Font("name=stroked");
	HC_Close_Segment();
	


}

HOpObjectAnnotate::~HOpObjectAnnotate()
{	
	// just in case the user did not press return
	if (m_pString)
		finalize_text();
	
	HC_Open_Segment_By_Key(GetView()->GetConstructionKey());	
		HC_Flush_Geometry(".");
		HC_UnSet_Text_Font();
	HC_Close_Segment();
	
	GetView()->Update();
}	


HBaseOperator * HOpObjectAnnotate::Clone()
{
	return new HOpObjectAnnotate(GetView());
}



const char * HOpObjectAnnotate::GetName() { return "HOpObjectAnnotate"; }

/////////////////////////////////////////////////////////////////////////////
// HOpObjectAnnotate message handlers



int HOpObjectAnnotate::OnLButtonDown(HEventInfo &event)
{
	if (m_pString) 
		finalize_text();

	if (!OperatorStarted()) {
		HPoint WindowSpace = event.GetMouseWindowPos();
		HC_Open_Segment_By_Key(GetView()->GetViewKey());

			int count = HC_Compute_Selection (".", "./scene/overwrite", "v, selection level = entity", WindowSpace.x, WindowSpace.y);

		HC_Close_Segment ();
		// is something pointed to for annotation
		if (count > 0) {
		
			HPoint	window, world;
			HC_Show_Selection_Position (&window.x, &window.y, &window.z, &world.x, &world.y, &world.z);

			HC_Show_Selection_Keys_Count(&m_iPathKeysCount);
			m_pSelectionPath = new HC_KEY[m_iPathKeysCount];
			HC_Show_Selection_Keys(&m_iPathKeysCount, m_pSelectionPath);

			// we're going to attach the string to the object.  Depending on how
			// you want includes to be dealt with you may need to extra work here
			HC_Open_Segment_By_Key(m_pSelectionPath[1]);
				HC_Show_Pathname_Expansion(".", m_sSegment);
			HC_Close_Segment();

			m_ptLast.Set(world.x, world.y, world.z);
		
			SetOperatorStarted(true);
			m_bObject = true;
		}
	}else
		SetOperatorStarted(false);

	GetView()->Update();
	return HOP_OK;
}



int HOpObjectAnnotate::OnNoButtonDownAndMove(HEventInfo &event)
{
	if (!OperatorStarted()) 
		return HBaseOperator::OnNoButtonDownAndMove(event);

    SetNewPoint(event.GetMouseWorldPos());

	// insert line from first click to where the mouse is moving
    HC_Open_Segment_By_Key(GetView()->GetConstructionKey());
	    
		HC_Flush_Geometry(".");
		HC_Insert_Line(m_ptLast.x, m_ptLast.y, m_ptLast.z, 
					   GetNewPoint().x,  GetNewPoint().y,  GetNewPoint().z);
			
	HC_Close_Segment();

    GetView()->Update();
    return HOP_OK;
}



int HOpObjectAnnotate::OnKeyDown(HEventInfo &event)
{
	if (OperatorStarted() || !m_bObject) 
		return HBaseOperator::OnKeyDown(event);

	char the_char = static_cast<char>(event.GetChar());

	switch((int)the_char){

	case '\r':		
		// is this a return
		finalize_text();
		return HOP_OK;
		break;
	
	case '\b':
		// erase
		if (m_pString){
			size_t len = strlen(m_pString);	
			
			// terminate the string one char shorter
			if(len)
				m_pString[len-1] = '\0'; 
		}else
			return HOP_OK;
		break;

	default:
		//range of printable characters
		if	((the_char >= ' ') && (the_char <= '~')) {
			
			if(m_pString){
			
				char *new_string = new char[strlen(m_pString)+2];

				sprintf(new_string, "%s%c", m_pString, the_char);
		
				delete[] m_pString;
				m_pString = new_string;
			}else{

				m_pString = new char[2];
				sprintf(m_pString, "%c", the_char);
			}
				
		}else
			// not a printable character
			return HOP_OK;
		break;
	}//switch

		
	HC_Open_Segment_By_Key(GetView()->GetConstructionKey());
		
		HC_Flush_Geometry(".");
		HC_Insert_Line(m_ptLast.x, m_ptLast.y, m_ptLast.z, 
			GetNewPoint().x,  GetNewPoint().y,  GetNewPoint().z);
		HC_Flush_Contents(".", "text");
		HC_Insert_Text(GetNewPoint().x,  GetNewPoint().y,  GetNewPoint().z, m_pString);

	HC_Close_Segment();
		
	GetView()->Update();
	return HOP_OK;

}

//	finalize_text() is called if the annotated geometry exists within and include segment.  In this case, the function
//	moves annotation text and lines from the construction segment into the include segment and cleans up.
//	carriage return event.  Erase is backspace.

void HOpObjectAnnotate::finalize_text()
{
	HC_Open_Segment_By_Key(GetView()->GetConstructionKey());
		HC_Flush_Geometry(".");
	HC_Close_Segment();
	
	if (m_pString) {

		if(strlen(m_pString)) {

				char	seg[MVO_BUFFER_SIZE];
				sprintf(seg, "%s", m_sSegment);

				AnnotateObject(GetView(),m_sSegment, m_pString, m_ptLast,GetNewPoint(), true);
			
/*				
		GB.  beginnings of extra work needed for multiple includes each with their 
		own modelling matrix ...

					float l[4],n[4];

					l[0] = m_ptLast.x;
					l[1] = m_ptLast.y;
					l[2] = m_ptLast.z;
					l[3] = 0.0f;

					n[0] = GetNewPoint().x;
					n[1] = GetNewPoint().y;
					n[2] = GetNewPoint().z;
					n[3] = 0.0f;

					HC_Compute_Matrix_Inverse(m_ModellingMatrix, m_ModellingMatrix);
					HC_Compute_Matrix_Product(m_ModellingMatrix, l, l);
					HC_Compute_Matrix_Product(m_ModellingMatrix, n, n);
					HC_Insert_Text(n[0], n[1], n[2], m_pString);
					HC_Insert_Line(l[0], l[1], l[2], n[0], n[1], n[2]);
*/	
	
 
		}

		delete[] m_pString;
		// re-initialise the string for next time around
		m_pString = 0;
	}

	m_bObject=false;
	SetOperatorStarted(false);
	GetView()->Update();
}


void HOpObjectAnnotate::AnnotateObject(HBaseView *pView,const char *segment, char *AnnotationText, HPoint lineStart, 
									   HPoint lineEnd, bool emit_message)
{
	HPoint l, n;

	HC_Open_Segment(segment);
		HC_Open_Segment("text");	
//			HC_Compute_Coordinates( segment, "world", &lineStart, "object", &l);
//			HC_Compute_Coordinates( segment, "world", &lineEnd, "object", &n);
			HC_Compute_Coordinates_By_Path(m_iPathKeysCount, m_pSelectionPath, "world", &lineStart, "viewpoint", &l);
			HC_Compute_Coordinates_By_Path(m_iPathKeysCount, m_pSelectionPath, "world", &lineEnd, "viewpoint", &n);
			n.z = l.z;
			HC_Compute_Coordinates_By_Path(m_iPathKeysCount, m_pSelectionPath, "viewpoint", &l, "object", &l);
			HC_Compute_Coordinates_By_Path(m_iPathKeysCount, m_pSelectionPath, "viewpoint", &n, "object", &n);
			HC_Set_Visibility("edges = on, lines = on");
			HC_Set_Rendering_Options("attribute lock = visibility");
			HC_Insert_Text(n.x, n.y, n.z, AnnotationText);
			HC_Insert_Line(l.x, l.y, l.z, n.x, n.y, n.z);					
			HImSetCallback("draw 3d polyline","HImUtility_draw_annotation_lines");
			if (pView->GetSharedKey())
			    pView->GetSharedKey()->AssociateKeysInSegment();
//			HC_Set_Color("lines=white");
		HC_Close_Segment();
	HC_Close_Segment();

    if (emit_message){
		HBaseViewEmitMessageFunction emit_message_function = pView->GetEmitMessageFunction();
		if (emit_message_function)
		{
			long ident;
			HC_KEY key;
			char message[MVO_MESSAGE_BUFFER_SIZE];

			key = HC_KOpen_Segment(segment);
			HC_Close_Segment();
			ident = pView->GetSharedKey()->GetIdentFromKey(key);

		    char encodedAnnotationText[MVO_BUFFER_SIZE];
			HUtility::URIencode(AnnotationText, (int) strlen(AnnotationText), encodedAnnotationText);

			HCLOCALE(sprintf(message, "H_ANNOTATE_OBJECT %ld %s %f %f %f %f %f %f",ident,encodedAnnotationText, 
				l.x, l.y, l.z, n.x, n.y, n.z));
	 	    emit_message_function((const char *)message, 
			    0, pView->GetEmitMessageFunctionUserData());
		}
    }

	H_SAFE_DELETE_ARRAY(m_pSelectionPath);
	m_iPathKeysCount = 0;
}

 
void HOpObjectAnnotate::AnnotateObjectFromMessage(HBaseView *pView, const char *in_data, unsigned int data_length)
{
	long ident;
	HC_KEY key;
    char data[MVO_MESSAGE_BUFFER_SIZE];
    char encodedAnnotationText[MVO_BUFFER_SIZE];
    char AnnotationText[MVO_BUFFER_SIZE];
    HPoint lineStart, lineEnd;
    memcpy(data, in_data, data_length);
    data[data_length] = '\0';
     HCLOCALE(sscanf(data, "%ld %s %f %f %f %f %f %f", &ident, encodedAnnotationText, &lineStart.x,&lineStart.y,&lineStart.z,
		 &lineEnd.x,&lineEnd.y,&lineEnd.z));
  
    MVO_POINTER_SIZED_UINT n;
    HUtility::URIdecode((const char *)encodedAnnotationText, AnnotationText, &n);
    AnnotationText[n] = '\0';

	key = pView->GetSharedKey()->GetKeyFromIdent(ident);
	if (key)
	{
		char path[MVO_SEGMENT_PATHNAME_BUFFER];
		HC_Show_Segment(key, path);

		HC_Open_Segment(path);{
			HC_Open_Segment("text");{	
				HC_Set_Visibility("edges = on, lines = on");
				HC_Set_Rendering_Options("attribute lock = visibility");
				HC_Insert_Text(lineEnd.x, lineEnd.y, lineEnd.z, AnnotationText);
				HC_Insert_Line(lineStart.x, lineStart.y, lineStart.z, lineEnd.x, lineEnd.y, lineEnd.z);					
				HImSetCallback("draw 3d polyline","HImUtility_draw_annotation_lines");
			}HC_Close_Segment();
		}HC_Close_Segment();
	}

	pView->Update();
}





