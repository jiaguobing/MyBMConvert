// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// HOpSelectPolyline.cpp : implementation of the HOpSelectPolyline class
//

#include <math.h>
#include <string.h>

#include "HTools.h"

#include "HBaseModel.h"
#include "HBaseView.h"
#include "HBaseOperator.h"
#include "HEventInfo.h"
#include "HOpSelectPolyline.h"
#include "HSelectionSet.h"
#include "HEventManager.h"



/////////////////////////////////////////////////////////////////////////////
// HOpSelectPolyline
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
//		Polyline definition is done. 
//		Store the Polyline information; Points are in World Space
//		Flush "?Window Space" segment
//
// HOpSelectPolyline.cpp : implementation of the HOpSelect class
//

HOpSelectPolyline::HOpSelectPolyline(HBaseView* view, int DoRepeat, int DoCapture) : HOpConstructPolyline(view, DoRepeat, DoCapture)
{
}

HOpSelectPolyline::~HOpSelectPolyline()
{
}

HBaseOperator * HOpSelectPolyline::Clone()
{
	return new HOpSelectPolyline(GetView());
}


const char * HOpSelectPolyline::GetName() { return "HOpSelectPolyline"; }

/////////////////////////////////////////////////////////////////////////////
// HOpSelectPolyline message handlers



int HOpSelectPolyline::OnLButtonDblClk(HEventInfo &event)
{
	HOpConstructPolyline::OnLButtonDblClk(event);

	if (!m_pPolyline)
		return HBaseOperator::OnLButtonDblClk(event);

	HSelectionSet* selection = GetView()->GetSelection();

	// Perform standard Windows explorer method of selecting and deselecting
	// using Shift and Control keys

	// If neither the Control key nor Shift key is down, deselect everything
    if (!event.Control() && !event.Shift())
	{
		selection->DeSelectAll();
		GetView()->EmitDeSelectAllMessage();
	}

    HC_Open_Segment_By_Key(GetView()->GetViewKey());

		int count = 0;
		if (m_PolylineCount > 1) 
		{
			count = HC_Compute_Selection_By_Polylin(".", "./scene/overwrite",
				"v, selection level = entity, no related selection limit, no selection sorting, selection proximity = 0.0", m_PolylineCount, m_pPolyline);
		}

	HC_Close_Segment();


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
			char skey_type[MVO_BUFFER_SIZE];

			HC_Show_Selection_Keys_Count(&skey_count);

			HC_KEY *keys		= new HC_KEY[skey_count];
			HC_KEY *incl_keys	= new HC_KEY[skey_count];
			HC_Show_Selection_Keys(&skey_count, keys);

			int i, incl_count = 0;
			for(i = skey_count-1; i >= 0; i--)
			{
				if( keys[i] >= 0 ) {
					/* if it is a locally renumbered key for which we don't have the proper context, bail */
					if( HC_KShow_Original_Key(keys[i]) == INVALID_KEY )
						continue;
				}

				HC_Show_Key_Type(keys[i], skey_type);
				if(strstr(skey_type, "include"))
				{
					incl_keys[incl_count] = keys[i];
					incl_count++;
				}
				else if(streq(skey_type, "reference"))
					key = keys[i];
			}


			if(selection->GetSelectionLevel()==HSelectSubentity)
			{
				HC_KEY sub_key;
				int sub_count;
				int * sub_v1, * sub_v2, * sub_faces;

				HC_Show_Selection_Elements_Coun(&sub_key, &sub_count);

				sub_v1 = new int [sub_count];
				sub_v2 = new int [sub_count];
				sub_faces = new int [sub_count];

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

	GetView()->SetGeometryChanged();
	GetView()->Update();
    return HOP_READY;
}
