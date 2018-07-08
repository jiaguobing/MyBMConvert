// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// HOpNavCube.cpp : implementation of the HOpNavCube class
//

#include <math.h>
#include <string.h>

#include "HTools.h"

#include "HBaseModel.h"
#include "HBaseView.h"
#include "HBaseOperator.h"
#include "HOpNavCube.h"
#include "HEventInfo.h"
#include "HSelectionSet.h"

/////////////////////////////////////////////////////////////////////////////
// HOpNavCube
//
// Operator that displays a transparent cube with arrows on all sides for quick viewpoint changes
// 

//cube is displayed when constructor is executed
HOpNavCube::HOpNavCube(HBaseView* view, int DoRepeat, int DoCapture) : HBaseOperator(view, DoRepeat, DoCapture)
{
	GetView()->SetNavCube(true);
	GetView()->FitWorld();

	HC_Open_Segment_By_Key(view->GetSceneKey());

 		m_NavCubeSegKey = HC_KOpen_Segment("navcube");

			HC_Set_Selectability("everything = off, faces = on!");
 			HC_Set_Heuristics("no polygon handedness");
			HC_Set_Rendering_Options("simple shadow = off");

/*
			HC_Set_Visibility("everything=on, edges=off, lines = off, markers=off, lights=on");
			HC_Set_Rendering_Options("lighting interpolation=on");

			HC_Set_Heuristics("hidden surfaces");

			char	*driver = (char *)view->GetDriverType();

			if (streq(driver,"opengl") || streq(driver,"xgl") || streq(driver,"sbx"))
				HC_Set_Rendering_Options("hsra = (hardware z-buffer, transparency=painters), technology = standard");
			else
				HC_Set_Rendering_Options("hsra = (szb, transparency=painters), technology = sfb");

 			HC_Set_Rendering_Options("attribute lock=(visibility, heuristics, rendering options)");
*/
			HPoint min,max;

			if (!HC_Compute_Circumcuboid_By_Key(view->GetSceneKey(), &min, &max))
			{
				// nothing in scene so let's set default values
				min.Set(-1,-1,-1);
				max.Set(1,1,1);
			}

			HC_Open_Segment("cube");
 				HC_Set_Color("faces = (diffuse = light blue, transmission = light grey)");
				HC_Set_Selectability("everything = off");		
				HUtility::InsertBox(&min, &max);
	 		HC_Close_Segment();

			HPoint diagonal;
			diagonal.Set( min.x - max.x, min.y - max.y, min.z - max.z ); 

			double diagonal_length = HC_Compute_Vector_Length(&diagonal);

			HPoint center;
			center.Set( (min.x+max.x)/2,(min.y+max.y)/2,(min.z+max.z)/2 );

			float vlen = (float)diagonal_length/5;
			float tl=vlen/2.0f;
			float sr=vlen/5.0f;
			float tr=vlen/3.3f;
			vlen-=tl;

			HC_Open_Segment("right");
				HC_Rotate_Object(0, 0, 90);
				HC_Translate_Object(vlen+tl+max.x, center.y, center.z);
				HUtility::InsertArrow(vlen, tl, sr, tr, 0,"r=0.0 g=0.0 b=1.0", "r=1.0 g=1.0 b=0.0",10,0.0);
			HC_Close_Segment();

			HC_Open_Segment("left");
				HC_Rotate_Object(0, 0, 270);
				HC_Translate_Object(-vlen-tl+min.x, center.y, center.z);
				HUtility::InsertArrow(vlen, tl, sr, tr, 0, "r=0.0 g=0.0 b=1.0", "r=1.0 g=1.0 b=0.0", 10, 0.0);
			HC_Close_Segment();

			HC_Open_Segment("top");
				HC_Rotate_Object(180, 0, 0);
				HC_Translate_Object(center.x, vlen+tl+max.y, center.z);
				HUtility::InsertArrow(vlen, tl, sr, tr, 0, "r=0.0 g=0.0 b=1.0", "r=1.0 g=1.0 b=0.0", 10, 0.0);
			HC_Close_Segment();

			HC_Open_Segment("bottom");
				HC_Translate_Object(center.x, -vlen-tl+min.y, center.z);
				HUtility::InsertArrow(vlen, tl, sr, tr, 0, "r=0.0 g=0.0 b=1.0", "r=1.0 g=1.0 b=0.0", 10, 0.0);
			HC_Close_Segment();

			HC_Open_Segment("front");
				HC_Rotate_Object(270, 0, 0);
				HC_Translate_Object(center.x, center.y, vlen+tl+max.z);
				HUtility::InsertArrow(vlen, tl, sr, tr, 0, "r=0.0 g=0.0 b=1.0", "r=1.0 g=1.0 b=0.0", 10, 0.0);
			HC_Close_Segment();

			HC_Open_Segment("back");
				HC_Rotate_Object(90, 0, 0);
				HC_Translate_Object(center.x, center.y, -vlen-tl+min.z);
				HUtility::InsertArrow(vlen, tl, sr, tr, 0, "r=0.0 g=0.0 b=1.0", "r=1.0 g=1.0 b=0.0", 10, 0.0);
			HC_Close_Segment();

		HC_Close_Segment();
	HC_Close_Segment();

 	GetView()->Update();
}


//desctructor deletes the cube from the segment hierachy
HOpNavCube::~HOpNavCube()
{
	GetView()->SetNavCube(false);
	HC_Delete_By_Key(m_NavCubeSegKey);
	GetView()->Update();
}



HBaseOperator * HOpNavCube::Clone()
{
	return new HOpNavCube(GetView());
}
 


const char * HOpNavCube::GetName() { return "HOpNavCube"; }



int HOpNavCube::OnLButtonDown(HEventInfo &event)
{
	HPoint  new_pos;
    int		res, count;
    char	type[MVO_BUFFER_SIZE];
    char	owner[MVO_SEGMENT_PATHNAME_BUFFER];
	HC_KEY  *keys = 0;
	HC_KEY  key;
	bool valid = false;
	
	// Mouse went down; find the selected items
	new_pos = event.GetMouseWindowPos();
	
	HC_Open_Segment_By_Key(m_NavCubeSegKey);
		res = HC_Compute_Selection(GetView()->GetDriverPath(),".","v, selection level = entity, related selection limit = 4", new_pos.x, new_pos.y);
	HC_Close_Segment();
	
 	  // compute the selection using the HOOPS window coordinate ofthe the pick location
	if (res > 0)
	{
		do	// process each item in the selection queue
		{
			
			HC_Show_Selection_Element (&key, 0, 0, 0);
			HC_Show_Key_Type(key,type);
			if (strcmp(type,"shell")==0)
			{
				valid = true;
				HC_Show_Selection_Keys_Count(&count);
				keys = new HC_KEY[count];				
				HC_Show_Selection_Keys(&count, keys);
				HC_Show_Owner_By_Key(keys[1],owner);
			}
			delete keys;
		} while (HC_Find_Related_Selection());
		
		//this is a little tricky. We need to find the last item in the selection list. This is usefull for the 
		//special case when one arrow obscures another
		if (valid)
		{
			char token[MVO_BUFFER_SIZE];
			HC_Parse_String(owner, "/", -2, token); 
			if (strcmp(token,"right")==0)
				GetView()->SetViewMode(HViewYZ);
			if (strcmp(token,"left")==0)
				GetView()->SetViewMode(HViewZY);
			if (strcmp(token,"back")==0)
				GetView()->SetViewMode(HViewXY);
			if (strcmp(token,"front")==0)
				GetView()->SetViewMode(HViewYX);
			if (strcmp(token,"top")==0)
				GetView()->SetViewMode(HViewXZ);
			if (strcmp(token,"bottom")==0)
				GetView()->SetViewMode(HViewZX);
		}
	}
	
    GetView()->Update();	// update the scene to reflect the new highlight attributes
	// of the selected items
    return HOP_READY;
}

 







