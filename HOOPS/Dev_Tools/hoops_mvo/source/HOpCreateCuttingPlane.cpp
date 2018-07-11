// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#include <math.h>
#include <string.h>

#include "HTools.h"
#include "HBaseView.h"
#include "HEventInfo.h"
#include "HSelectionSet.h"
#include "HOpCreateCuttingPlane.h"



/////////////////////////////////////////////////////////////////////////////
// CreateCuttingPlane
//
 

HOpCreateCuttingPlane::HOpCreateCuttingPlane(HBaseView* view, int DoRepeat, int DoCapture) : HOpObjectTranslate(view, DoRepeat, DoCapture)
{
}


HBaseOperator * HOpCreateCuttingPlane::Clone()
{
	return new HOpCreateCuttingPlane(GetView());
}


const char * HOpCreateCuttingPlane::GetName() { return "HOpCreateCuttingPlane"; }

/////////////////////////////////////////////////////////////////////////////
// CreateCuttingPlane 



int HOpCreateCuttingPlane::OnLButtonDown(HEventInfo &event)
{
	HPoint center;
	float radius;
	char segname[MVO_SEGMENT_PATHNAME_BUFFER];
	HC_KEY newseg;
	HSelectionSet* selection = GetView()->GetSelection();

	if (!OperatorStarted()) 
		SetOperatorStarted(true);

	HC_Open_Segment_By_Key(GetView()->GetSceneKey());
		HC_Compute_Circumsphere(".", &center, &radius); 
		HC_Compute_Coordinates(".", "object", &center, "viewpoint", &center);
	HC_Close_Segment();

	if (radius == 0) radius=1;

	HC_Open_Segment_By_Key(GetView()->GetCuttingPlanesKey());
		newseg = HC_KOpen_Segment("");
			MakeCuttingPlane(center.z, radius);
		HC_Close_Segment();
	HC_Close_Segment();


	GetView()->SetCuttingPlanesExist(true);


	HC_Show_Segment(newseg, segname);
	
	selection->DeSelectAll();
	selection->Select(newseg, 0, 0, false);

	HOpObjectTranslate::OnLButtonDown(event);

	return HOP_OK;
}


int HOpCreateCuttingPlane::OnLButtonUp(HEventInfo &event)
{
	HSelectionSet* selection = GetView()->GetSelection();
	
	selection->DeSelectAll();

	GetView()->SetGeometryChanged();
	GetView()->Update();

	HOpObjectTranslate::OnLButtonUp(event);

	return HOP_READY;
}

// private function not documented in reference manual
void HOpCreateCuttingPlane::MakeCuttingPlane(float z, float rad)
{
	float pl[4];
	HPoint ori, ref1, ref2;
	/*hpux doesn't support "general initalizers"*/
	HPoint p[5];

	p[0].Set(-rad, -rad, z);
	p[1].Set( rad, -rad, z);
	p[2].Set( rad,  rad, z);
	p[3].Set(-rad,  rad, z);
	p[4].Set(-rad, -rad, z);

	for (int i=0; i<4; i++)
		HC_Compute_Coordinates(".", "viewpoint", &p[i], "world", &p[i]);


	HC_Compute_Polygon_Plane(4, p, "right", pl);

	// need to insert the cutting planes in a 
	// separate segment from the constructio
	// grid so that we can ensure the construction 
	// grid is not affected by the cutting plane

	HC_Insert_Cutting_Plane(pl[0],pl[1],pl[2],pl[3]);
	
	ori.Set(p[0].x, p[0].y, p[0].z);

	ref1.Set(	(p[1].x - p[0].x) / 10.0f + p[0].x,	
				(p[1].y - p[0].y) / 10.0f + p[0].y,
				(p[1].z - p[0].z) / 10.0f + p[0].z);

	ref2.Set(	(p[3].x - p[0].x) / 10.0f + p[0].x,
				(p[3].y - p[0].y) / 10.0f + p[0].y,
				(p[3].z - p[0].z) / 10.0f + p[0].z);

	HC_Open_Segment("cutting grids");
		HC_Insert_Grid("quadrilateral", &ori, &ref1, &ref2, 10, 10);
		HC_Set_Visibility("cutting planes = off");
	HC_Close_Segment();
}

