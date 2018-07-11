// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hc.h"
#include "hic.h"
#include "HUtility.h"
 
#include "HUtilityAnnotationMeasure.h"
#include "HEventManager.h"
#include "HObjects.h"
#include "HBaseView.h"
#include "HImManager.h"



void HUtilityAnnotationMeasure::drawLineCallbackDummy (HIC_Rendition const  *rendition,int count, HIC_DC_Point * p)
{
	UNREFERENCED(count);

	HIC_Draw_DC_Polyline (rendition, 2, p);
}


HC_KEY 
HUtilityAnnotationMeasure::Insert(
		HPoint target,
		HPoint position,
		HC_KEY AnnotateThis,
		HBaseView * pView,
		HC_KEY layer_key,
		int bg_type,
		bool useArrow,
		HC_KEY * key_path,
		int key_path_length)
{
	m_pnt_a1 = m_pnt_a2 = target ;
	m_pnt_b1 = m_pnt_b2 = position ;
	mid_point(target, position, m_position);
	HC_KEY retval = HUtilityAnnotation::Insert(target, position, AnnotateThis, pView, layer_key, bg_type, useArrow, key_path, key_path_length);
	return retval;
}


HC_KEY
HUtilityAnnotationMeasure::Insert(
		char const * text,
		HPoint target,
		HPoint position,
		HC_KEY AnnotateThis,
		HBaseView * pView,
		HC_KEY layer_key,
		int bg_type,
		bool useArrow,
		HC_KEY * key_path,
		int key_path_length)
{
	m_pnt_a1 = m_pnt_a2 = target ;
	m_pnt_b1 = m_pnt_b2 = position ;
	mid_point(target, position, m_position);
	HC_KEY retval = HUtilityAnnotation::Insert(text, target, position, AnnotateThis, pView, layer_key, bg_type, useArrow, key_path, key_path_length);
	return retval;
}


HC_KEY
HUtilityAnnotationMeasure::InsertWithEncoding(
		unsigned short const * text,
		HPoint target,
		HPoint position,
		HC_KEY AnnotateThis,
		HBaseView * pView,
		HC_KEY layer_key,
		int bg_type,
		bool useArrow,
		HC_KEY * key_path,
		int key_path_length)
{
	m_pnt_a1 = m_pnt_a2 = target ;
	m_pnt_b1 = m_pnt_b2 = position ;
	mid_point(target, position, m_position);
	HC_KEY retval = HUtilityAnnotation::InsertWithEncoding(text, target, position, AnnotateThis, pView, layer_key, bg_type, useArrow, key_path, key_path_length);

	HC_Open_Segment_By_Key(m_MyKey);{
		HC_Open_Segment("conditions/xarrow/leader lines");{
			HC_Set_Line_Pattern("- -");
		}HC_Close_Segment();
	}HC_Close_Segment();

	return retval;
}

bool 
HUtilityAnnotationMeasure::IsMeasurement(HC_KEY key){
	char type[512];
	char subtype[512];

	if(key == INVALID_KEY)
		return false;

	HObjectManager::FindHObjectSegment(key, type, subtype);

	if(streq(type, "Annotation") && streq(subtype, "HUtilityAnnotationMeasure"))
		return true;
	return false;
}


void
HUtilityAnnotationMeasure::mid_point(HPoint &A, HPoint &B, HPoint &mid)
{
	mid.Set( (A.x + B.x) / 2.0f,  (A.y + B.y) / 2.0f,  (A.z + B.z) / 2.0f);
}


void 
HUtilityAnnotationMeasure::MoveText(HPoint pos)
{
	HPoint a1b1mid;
	m_position = pos;

	mid_point(m_pnt_a1, m_pnt_b1, a1b1mid);
	HPoint pos_offset = m_position - a1b1mid;
	m_pnt_a2 = m_pnt_a1 + pos_offset;
	m_pnt_b2 = m_pnt_b1 + pos_offset;

	HC_Open_Segment_By_Key(m_MyKey);{
		HC_Open_Segment("conditions/xarrow");{
			HC_Flush_Contents("...","geometry");
			HImSetCallback("draw dc polyline","HIMUtility_drawlineclip");
		
			HC_Insert_Line(m_pnt_a2.x, m_pnt_a2.y, m_pnt_a2.z,
				m_pnt_b2.x, m_pnt_b2.y, m_pnt_b2.z);
		
			HC_Open_Segment("leader lines");{
			HImSetCallback("draw dc polyline","HIMUtility_drawlinedummy");

				HC_Insert_Line(m_pnt_a1.x, m_pnt_a1.y, m_pnt_a1.z,
					m_pnt_a2.x, m_pnt_a2.y, m_pnt_a2.z);
				HC_Insert_Line(m_pnt_b1.x, m_pnt_b1.y, m_pnt_b1.z,
					m_pnt_b2.x, m_pnt_b2.y, m_pnt_b2.z);
			}HC_Close_Segment();

		}HC_Close_Segment();

		HC_Open_Segment("conditions/matrix");{
			HC_UnSet_Modelling_Matrix();
			HC_Translate_Object(m_position.x, m_position.y, m_position.z);
		}HC_Close_Segment();
	}HC_Close_Segment();
}


void
HUtilityAnnotationMeasure::MoveText(HPoint A, HPoint B)
{
	HPoint a1b1mid;
	m_pnt_a1 = A;
	m_pnt_b1 = B;

	mid_point(m_pnt_a1, m_pnt_b1, a1b1mid);
	m_position = a1b1mid;
	m_pnt_a2 = m_pnt_a1;
	m_pnt_b2 = m_pnt_b1;

	HC_Open_Segment_By_Key(m_MyKey);{
		HC_Open_Segment("conditions/xarrow");{
			HC_Flush_Contents("...","geometry");
			HImSetCallback("draw dc polyline","HIMUtility_drawlineclip");

			HC_Insert_Line(m_pnt_a2.x, m_pnt_a2.y, m_pnt_a2.z,
				m_pnt_b2.x, m_pnt_b2.y, m_pnt_b2.z);
		
			HC_Open_Segment("leader lines");{
				HImSetCallback("draw dc polyline","HIMUtility_drawlinedummy");
				HC_Insert_Line(m_pnt_a1.x, m_pnt_a1.y, m_pnt_a1.z,
					m_pnt_a2.x, m_pnt_a2.y, m_pnt_a2.z);
				HC_Insert_Line(m_pnt_b1.x, m_pnt_b1.y, m_pnt_b1.z,
					m_pnt_b2.x, m_pnt_b2.y, m_pnt_b2.z);
			}HC_Close_Segment();

		}HC_Close_Segment();

		HC_Open_Segment("conditions/matrix");{
			HC_UnSet_Modelling_Matrix();
			HC_Translate_Object(m_position.x, m_position.y, m_position.z);
		}HC_Close_Segment();
	}HC_Close_Segment();
}


void
HUtilityAnnotationMeasure::SetNote(HC_KEY Annotation, HBaseView * pView){
	HUtilityAnnotation::SetNote(Annotation, pView->GetViewKey());

	HC_Open_Segment_By_Key(m_MyKey);{
		HC_KEY key;
		
		HC_Open_Segment("conditions/xarrow/leader lines");{
			HC_Begin_Contents_Search(".","lines");{
				char buffer[1024];
				HC_Find_Contents(buffer, &key);
				HC_Show_Line(key, &m_pnt_a1.x, &m_pnt_a1.y, &m_pnt_a1.z,
					&m_pnt_a2.x, &m_pnt_a2.y, &m_pnt_a2.z);
				HC_Find_Contents(buffer, &key);
				HC_Show_Line(key, &m_pnt_b1.x, &m_pnt_b1.y, &m_pnt_b1.z,
					&m_pnt_b2.x, &m_pnt_b2.y, &m_pnt_b2.z);
			}HC_End_Contents_Search();
		}HC_Close_Segment();
	}HC_Close_Segment();

}
