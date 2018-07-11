

#include "HBaseModel.h"
#include "HBaseView.h"
#include "HBaseOperator.h"
#include "HEventInfo.h"
#include "HTools.h"
#include "HOpFramedAnnotate.h"
#include "HUtilityAnnotationNoHIC.h"
#include "HUtilityAnnotationMeasureNoHIC.h"
#include "HMarkupManager.h"
#include "vlist.h"
 
#include "stdio.h"
#include <math.h>
#include <string.h>
#include <stdlib.h>



static HC_KEY m_scene_key; 
//static HC_KEY m_pNoteKey;

HOpFramedAnnotate::HOpFramedAnnotate(HBaseView* view, bool AddToLayer, int DoRepeat,int DoCapture) : HBaseOperator(view, DoRepeat, DoCapture)
{
 	m_pString = 0;
 	m_pNoteKey = 0;
	m_bAddToLayer = AddToLayer;
	m_scene_key = view->GetSceneKey();
}

HOpFramedAnnotate::~HOpFramedAnnotate()
{
 	if (m_pString)
		finalize_text(false);
}


HBaseOperator * HOpFramedAnnotate::Clone()
{
	return (HBaseOperator *)new HOpFramedAnnotate(m_pView, m_bAddToLayer);
}

 
const char * HOpFramedAnnotate::GetName() { return "HOpFramedAnnotate"; }

 
 
/////////////////////////////////////////////////////////////////////////////
// HOpFramedAnnotate message handlers



int HOpFramedAnnotate::OnLButtonDown(HEventInfo &event)
{
	HPoint new_pos;
    int	res;
	HC_KEY key;
	HUtilityAnnotationNoHIC note(m_pView);
	char type[MVO_BUFFER_SIZE];
	HPoint WindowSpace = event.GetMouseWindowPos();
	int selections_found;
	HPoint	window, world;
	bool DisregardNote = false;
	HC_KEY keys[1000];
	int keycount;
	
	if (m_pString)
	{
		bool emitting_messages=false;
		
		if (streq(GetName(), "HOpMarkupPostIt"))
			emitting_messages=true;
		
		finalize_text(emitting_messages);
	}
	
	if (!m_bOpStarted) 
		m_bOpStarted = true;
	else return true;
	
	new_pos = event.GetMouseWindowPos();
	
	m_pNoteKey = -1;
	HC_Open_Segment_By_Key(m_pView->GetViewKey());
		HC_QSet_Heuristics( "./scene/overwrite", "no related selection limit");
		res = HC_Compute_Selection(".","./scene/overwrite","v", new_pos.x, new_pos.y);
		HC_QUnSet_Heuristics("./scene/overwrite");
    HC_Close_Segment();

	// compute the selection using the HOOPS window coordinate ofthe the pick location
	if (res > 0)
	{
		do	// process each item in the selection queue
		{
			HC_Show_Selection_Element (&key, NULL, NULL, NULL);
			if( key >= 0 ) {
				// if renumbered, grab the original unrenumbered version
				HC_Show_Selection_Keys( &keycount, keys );
				HC_Open_Segment_By_Key( keys[1] );
					key = HC_KShow_Original_Key( key );
				HC_Close_Segment();
			}
			HC_Show_Key_Type(key,type);
			HC_KEY temp = note.IsNote(key);
			HUtilityAnnotationMeasureNoHIC note2;
			HC_KEY temp2 = note2.IsMeasurement(key);
			if (temp != -1 && temp2 == -1)
			{
				m_pNoteKey = temp;
				break;
			}
			if (temp2 != -1)
				DisregardNote = true;
		} while (HC_Find_Related_Selection());
	}

	if (m_pNoteKey == -1 && res > 0 && !DisregardNote)
	{	
		WindowSpace = event.GetMouseWindowPos();
		HC_Open_Segment_By_Key(m_pView->GetViewKey());
			selections_found = HC_Compute_Selection (".", "./scene/overwrite", "v", WindowSpace.x, WindowSpace.y);
		HC_Close_Segment ();

		// is something pointed to for annotation
		if (selections_found)
		{
			HC_Show_Selection_Element( &key, NULL, NULL, NULL );
			if( key >= 0 ) {
				// if renumbered, grab the original unrenumbered version
				HC_Show_Selection_Keys( &keycount, keys );
				HC_Open_Segment_By_Key( keys[1] );
					key = HC_KShow_Original_Key( key );
				HC_Close_Segment();
			}
			HC_Show_Selection_Position (&window.x, &window.y, &window.z, &world.x, &world.y, &world.z);							 
			m_ptNew.Set(world.x, world.y, world.z);  			
			HC_Show_Key_Type(key, type);			
			if (strcmp(type,"segment") != 0)
				key = HC_KShow_Owner_By_Key(key); 
			
			HMarkupManager *markup = m_pView->GetMarkupManager();
			HC_KEY activelayerkey = -1;
			if (m_bAddToLayer && (activelayerkey = markup->GetCurrentLayerKey()) != -1)
			{
				HC_Open_Segment_By_Key(activelayerkey); 
				HC_Open_Segment("notes"); 
			}
			else
			{	
				HC_Open_Segment_By_Key(markup->GetMarkupKey()); 
				HC_Open_Segment("notes"); 
				HC_Set_Visibility("everything = off, text = on, lines = on, faces = on");
			}
			unsigned short *text = HUtility::uconvert("Note");
			m_pNoteKey = note.InsertWithEncoding(text,m_ptNew, m_ptNew);
			delete [] text;
			if (activelayerkey == -1)
				note.SetOnTop(m_pNoteKey, false);
			HC_Close_Segment();
			HC_Close_Segment();
		}
		
	}
	if (m_pNoteKey == -1)
		m_bOpStarted = false;
	else
	{
		HC_Open_Segment_By_Key(m_pNoteKey);
		HC_Set_Heuristics("quick moves = off");
		HC_Close_Segment();
	}
	
	
    m_ptNew = event.GetMouseWorldPos();	
	
	m_ptFirst.x = m_ptNew.x;
	m_ptFirst.y = m_ptNew.y;
	m_ptFirst.z = m_ptNew.z;
	
	m_pView->Update();
	
    return (HOP_OK);
}




int HOpFramedAnnotate::OnLButtonDownAndMove(HEventInfo &event)
{
	HPoint	tmp;
	HUtilityAnnotationNoHIC note(m_pView);
	if (!m_bOpStarted) return HBaseOperator::OnLButtonDownAndMove(event);

    m_ptNew = event.GetMouseWorldPos();
	tmp = event.GetMouseWindowPos();
	
//	m_pNoteKey->m_NotePos=m_ptNew;
//	m_pNoteKey->SetNotePos(&m_ptNew,m_pView);
	note.AdjustPositionToTextPlane(m_pNoteKey,m_pView->GetSceneKey(),m_ptNew);
 	note.MoveText(m_pNoteKey,m_ptNew);

  	m_pView->Update();
	return (HOP_OK);
}



int HOpFramedAnnotate::OnLButtonUp(HEventInfo &event)
{
	if(!m_bOpStarted) return HBaseOperator::OnLButtonUp(event);

	m_bOpStarted = false;
    HC_Open_Segment_By_Key(m_pNoteKey);
	HC_UnSet_One_Heuristic("quick moves");
    HC_Close_Segment();
	if (streq(GetName(), "HOpMarkupPostIt"))
		TransmitMarkupSegment();

 	m_pView->Update();
 
//	HUtilityAnnotation::FixText(m_pNoteKey,m_pView->GetSceneKey());
    return(HOP_READY);
}

 


int HOpFramedAnnotate::OnKeyDown(HEventInfo &event)
{
	HUtilityAnnotationNoHIC note(m_pView);

	if (m_bOpStarted || m_pNoteKey == -1) 
		return HBaseOperator::OnKeyDown(event);

	unsigned short the_char = event.GetChar();

	switch((int)the_char){

	case '\r':		
		// is this a return
		{
			bool emitting_messages=false;

			if (streq(GetName(), "HOpMarkupPostIt"))
				emitting_messages=true;

			finalize_text(emitting_messages);
		}
		return (HOP_OK);
		break;
	
	case '\b':
		// erase
		if (m_pString){
		    int len = HUtility::ustrlen(m_pString);	
			
			// terminate the string one char shorter
			if(len)
				m_pString[len-1] = '\0'; 
		}else
			return (HOP_OK);
		break;

	default:
 			
	    if(m_pString){
		
		int slen = HUtility::ustrlen(m_pString);
		unsigned short *new_string = new unsigned short[slen+2];
		for (int i=0;i<slen;i++)
		    new_string[i] = m_pString[i];
		new_string[slen] = the_char;
		new_string[slen+1] = 0;				
 		
		delete[] m_pString;
		m_pString = new_string;
	    }else{
		

		m_pString = new unsigned short[2];
		m_pString[0] = the_char;
		m_pString[1] = 0;
	    }
	    
	    break;
	}//switch
 	
 	note.SetTextWithEncoding(m_pNoteKey, m_pString);
 
 

 	m_pView->Update();
	return (HOP_OK);

}

 
void HOpFramedAnnotate::finalize_text(bool emit_message)
{
	delete[] m_pString;
	m_pString = 0;

	// Now let's do the Net work and send this info over the 
	// wire

	if (emit_message)
		TransmitMarkupSegment();
 
	m_bOpStarted=false;
	m_pView->Update();
}


void HOpFramedAnnotate::TransmitMarkupSegment()
{
	if (m_pView->EmittingMessages())
	{
		HMarkupManager *markupManager =  m_pView->GetMarkupManager();

		HC_KEY activelayerkey;
		if ((activelayerkey = markupManager->GetCurrentLayerKey()) != -1)
		{
			HC_Open_Segment_By_Key(activelayerkey);
 			markupManager->EmitMarkupSegment(HC_KOpen_Segment("notes"));
			HC_Close_Segment();
			HC_Close_Segment();
//		else 
//			m_pView->EmitSegment(m_pNotesSegKey);
		}
	}
}

