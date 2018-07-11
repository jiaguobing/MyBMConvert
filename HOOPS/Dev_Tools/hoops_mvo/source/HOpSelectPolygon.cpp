// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// HOpSelectPolygon.cpp : implementation of the HOpSelectPolygon class
//

#include <math.h>
#include <string.h>

#include "HTools.h"

#include "HBaseModel.h"
#include "HBaseView.h"
#include "HBaseOperator.h"
#include "HEventInfo.h"
#include "HOpSelectPolygon.h"
#include "HSelectionSet.h"
#include "HEventManager.h"


/////////////////////////////////////////////////////////////////////////////
// HOpSelectPolygon
//
// Operator for rubberbanding a Polyline in the viewplane.
//
// Left button down:
//		Insert_Ink with position in window space in "?window space" segment
//
// NoButtonDownAndMove:
//		Rubber band line from last point to current point 
//
// Left Button Double Click: 
//		Polygon definition is complete. 
//		Insert Polygon into scene with it's face visibility off
//		Flush "?Window Space" segment
//
// HOpSelectPolygon.cpp : implementation of the HOpSelectPolygon class
//

HOpSelectPolygon::HOpSelectPolygon(HBaseView* view, int DoRepeat, int DoCapture) : HOpConstructPolyline(view, DoRepeat, DoCapture)
{
	m_bFirstTimeThrough = true;
}

HOpSelectPolygon::~HOpSelectPolygon()
{
	HC_Open_Segment_By_Key(GetView()->GetWindowspaceKey());
		HC_Flush_Geometry(".");
	HC_Close_Segment();
	GetView()->Update();
}



HBaseOperator * HOpSelectPolygon::Clone()
{
	return new HOpSelectPolygon(GetView());
}


const char * HOpSelectPolygon::GetName() { return "HOpSelectPolygon"; }

/////////////////////////////////////////////////////////////////////////////
// HOpSelectPolygon message handlers



int HOpSelectPolygon::OnLButtonDown(HEventInfo &event)
{
	if (m_bFirstTimeThrough)
	{
		HC_Open_Segment_By_Key(GetView()->GetWindowspaceKey());
			HC_Flush_Geometry(".");
		HC_Close_Segment();

		GetView()->Update();
		m_bFirstTimeThrough = false;
	}

	HOpConstructPolyline::OnLButtonDown(event);
    return HOP_READY;
}




int HOpSelectPolygon::OnLButtonDblClk(HEventInfo & event)
{
	HOpConstructPolyline::OnLButtonDblClk(event);

	if (!m_pPolyline)
		return HBaseOperator::OnLButtonDblClk(event);

	HC_Open_Segment_By_Key(GetView()->GetWindowspaceKey());{
		HC_Insert_Polygon(m_PolylineCount, m_pPolyline);
	}HC_Close_Segment();

	GetView()->Update();

	HSelectionSet * selection = GetView()->GetSelection();

	// Perform standard Windows explorer method of selecting and deselecting
	// using Shift and Control keys

	// If neither the Control key nor Shift key is down, deselect everything
	if (!event.Control() && !event.Shift())
	{
		selection->DeSelectAll();
		GetView()->EmitDeSelectAllMessage();
	}

	HSelectLevel const sel_level = selection->GetSelectionLevel();

	int count;

	HC_Open_Segment_By_Key(GetView()->GetViewKey());{
		bool need_internal_selection = sel_level == HSelectSubentity;

		char action[512];
		int len = sprintf(action, "%s", "v, selection level = entity, no related selection limit, no selection sorting, selection proximity = 0.0");
		if (need_internal_selection)
			sprintf(action + len, "%s", ", no internal selection limit");

		count = HC_Compute_Selection_By_Polygon(".", "./scene/overwrite", action, m_PolylineCount, m_pPolyline);
	}HC_Close_Segment();


	if (count)
	{
		selection->SetSelectWillNotify(false);

		do	// process each item in the selection queue
		{
			HC_KEY key;

			// Use this function instead of HC_Show_Selection_Element so we get the internal key 3dGS
			// uses to refer to the piece of geometry.  key will hold this value
			HC_Show_Selection_Original_Key(&key);

			//this function makes sure that only the top level segment of an "object" is selectable. If a segment/geometry 
			//is not part of an object definition the key is not changed
			key = HObjectManager::FindHObjectSegment(key);
			
			// build up an array of include keys to pass with the selection
			int skey_count;
			HC_Show_Selection_Keys_Count(&skey_count);

			HC_KEY * keys = new HC_KEY[skey_count];
			HC_KEY * incl_keys = new HC_KEY[skey_count];
			HC_Show_Selection_Keys(&skey_count, keys);

			int incl_count = 0;
			for (int i = skey_count - 1; i >= 0; i--)
			{
				if (keys[i] >= 0)
				{
					// if it is a locally renumbered key for which we don't have the proper context, bail
					if (HC_KShow_Original_Key(keys[i]) == HC_ERROR_KEY)
						continue;
				}

				char skey_type[64];
				HC_Show_Key_Type(keys[i], skey_type);
				if(strstr(skey_type, "include"))
				{
					incl_keys[incl_count] = keys[i];
					incl_count++;
				}
				else if (streq(skey_type, "reference"))
					key = keys[i];
			}

			if (sel_level == HSelectSubentity)
			{
				HC_KEY sub_key;
				int sub_count;
				HC_Show_Selection_Elements_Coun(&sub_key, &sub_count);

				int * sub_v1 = new int [sub_count];
				int * sub_v2 = new int [sub_count];
				int * sub_faces = new int [sub_count];

				HC_Show_Selection_Elements(&sub_key, &sub_count, sub_v1, sub_v2, sub_faces);

				selection->SelectSubentity(key, incl_count, incl_keys, sub_count, sub_faces, sub_v1, sub_v2, true);

				delete [] sub_v1;
				delete [] sub_v2;
				delete [] sub_faces;
			}
			else if (!selection->IsSelected(key, incl_count, incl_keys))	
				selection->Select(key, incl_count, incl_keys, true);

			delete[] keys;
			delete[] incl_keys;

		} while (HC_Find_Related_Selection());

		selection->SetSelectWillNotify(true);

		// notify the event to view
		GetView()->Notify(HSignalSelected);
	}


	m_bFirstTimeThrough = true;

	GetView()->SetGeometryChanged();
	GetView()->Update();

	return HOP_READY;
}


