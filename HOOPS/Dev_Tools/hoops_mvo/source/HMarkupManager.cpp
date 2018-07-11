// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "HMarkupManager.h"
#include "HBaseView.h"
#include "HUtility.h"
#include "HStream.h"
#include "HBaseModel.h"
#include "HSelectionSet.h"





HMarkupManager::HMarkupManager(HBaseView *view) : m_ActiveLayerName(0), m_allNames(0)
{
	ReInit(view);
}

void
HMarkupManager::ReInit(HBaseView *view)
{
	m_pHView = view;
 	m_MarkupColor.Set(1.0,0.0,0.0);
	m_fMarkupWeight = 4.0f;

	HC_Open_Segment_By_Key(m_pHView->GetOverwriteKey()); {
		m_MarkupKey = HC_KOpen_Segment("markup"); {
			HC_Set_Rendering_Options("no frame buffer effects");
			HC_Set_Rendering_Options("simple shadow = off");
			HC_Set_Heuristics("exclude bounding");
			HC_Set_Rendering_Options("hidden line removal options = (render faces), no display lists");
			m_MarkupLayerKey = HC_KOpen_Segment("layers"); {		
//				HC_Set_Selectability ("everything = off");
				HC_Set_Rendering_Options("depth range=(0.0,0.001)");
				HC_Set_Visibility("cutting planes = off, faces = off, edges = on, lines = on, text = on, markers = on");
			} HC_Close_Segment();										
		} HC_Close_Segment();
	} HC_Close_Segment();

	m_ActiveLayerKey = INVALID_KEY;
	m_DefaultLayerKey  = INVALID_KEY;
	m_bGeneratePreviewImage = true;
}

HMarkupManager::~HMarkupManager()
{
	HC_Open_Segment_By_Key(GetMarkupKey());
	    HC_Flush_Contents (".", "everything");
	HC_Close_Segment();
	if(m_ActiveLayerName)
		free(m_ActiveLayerName);

	if(m_allNames)
		free(m_allNames);
}

static void deselect_geometry(HSelectionSet * sel_set)
{
	HC_KEY found_key;

	HC_Begin_Contents_Search(".", "geometry");
	while(HC_Find_Contents(0, &found_key))
	{
		HC_KEY original_key = HC_Show_Original_Key(found_key);
		if(sel_set->IsSelected(original_key))
			sel_set->DeSelect(original_key);
	}
	HC_End_Contents_Search();	
}

static void deselect_markup(HBaseView *view, HC_KEY start_seg)
{
	HSelectionSet* sel_set=view->GetSelection();
	char type[MVO_BUFFER_SIZE];
	HC_KEY found_key;

	if(!sel_set)
		return;

	if(sel_set->IsSelected(start_seg))
		sel_set->DeSelect(start_seg);

	HC_Open_Segment_By_Key(start_seg);
	HC_Begin_Contents_Search("...", "segments");
		while(HC_Find_Contents(type, &found_key))
		{
			//this could be a problem if the segments are renumbered
			HC_Open_Segment_By_Key(found_key);
				deselect_geometry(sel_set);
			HC_Close_Segment();			
		}
	HC_End_Contents_Search();
	HC_Close_Segment();
}


void HMarkupManager::DisableCurrentLayer(bool emit_message)
{
	if (m_ActiveLayerKey != INVALID_KEY)
	{
		UnsetCurrentLayerCondition();
		HC_Open_Segment_By_Key(m_ActiveLayerKey); {
			UpdatePreviewImage();
			HC_Set_Visibility("everything = off");
			HC_Set_Rendering_Options("attribute lock = (visibility)");
		} HC_Close_Segment();
		deselect_markup(m_pHView, m_ActiveLayerKey);

		// Tell other HNet Clients
		if (emit_message)
			EmitDisableCurrentMarkupLayer();

		m_ActiveLayerKey = INVALID_KEY;
	}
}


void HMarkupManager::SetupNewLayer()
{
	HCamera cam;
	m_pHView->GetCamera(&cam);

	HC_Set_User_Options (H_FORMAT_TEXT("H_SET_CAMERA = %f %f %f %f %f %f %f %f %f %f %f %s",
		cam.position.x, cam.position.y, cam.position.z, 
		cam.target.x, cam.target.y, cam.target.z,
		cam.up_vector.x, cam.up_vector.y, cam.up_vector.z, 
		cam.field_width, cam.field_height, cam.projection));
	HC_Set_Line_Weight(m_fMarkupWeight);
	HC_Set_Edge_Weight(m_fMarkupWeight);
	HC_Set_Color_By_Value("everything", "RGB", m_MarkupColor.x, m_MarkupColor.y, m_MarkupColor.z);
	if (m_bGeneratePreviewImage)
		UpdatePreviewImage();
	else
		HC_Set_User_Options ("H_SUPPRESS_PREVIEW_IMAGE = TRUE");

}

bool HMarkupManager::GetPreviewImage(char const * layname,
									 int * width, int * height, int * bbp,
									 unsigned char const ** img)
{
	HC_KEY key = INVALID_KEY;
	HC_KEY lay = FindLayer(layname);
	if(lay != INVALID_KEY){
		HC_Open_Segment_By_Key(lay);{

			HC_Open_Segment("preview image");{
		
				HC_Begin_Contents_Search(".", "images");{
					HC_Find_Contents(0, &key);
				}HC_End_Contents_Search();

				if(key != INVALID_KEY){
					int x = -1, y = -1;
					HC_Show_Image_Size(key, 0, 0, 0, 0, &x, &y);
					if(width)
						*width = x;
					if(height)
						*height = y;
					unsigned char const * tmp = 0;
					if(img){
						HC_Show_Geometry_Pointer(key, "rgba rasters", &tmp);
						*img = tmp;
					}
					if(bbp)
						*bbp = 4;
				}

			}HC_Close_Segment();
		}HC_Close_Segment();
	}

	return key != INVALID_KEY;
}

bool HMarkupManager::IsCameraLayer(HC_KEY layerkey) 
{
	if (layerkey == INVALID_KEY)
		return false;
	HUserOptions useroption;
	HUserOptions cam_exist_option;
	HC_Open_Segment_By_Key(layerkey); 
	/*
	layer corresponding to cad captures, can be defined without any camera definition,
	 current orientation have to be kept
	*/
	cam_exist_option.Show_One_User_Option("camera");
	HC_Close_Segment();
	if( cam_exist_option.m_data && strcmp(cam_exist_option.m_data, "no") == 0 )
		return false;
	return true;
}





bool HMarkupManager::GetCamera(HC_KEY layerkey, HCamera & cam) const
{
	HUserOptions useroption;
	char const * uo = 0;
	HUserOptions cam_exist_option;
	HC_Open_Segment_By_Key(layerkey); 
	/*
	layer corresponding to cad captures, can be defined without any camera definition,
	 current orientation have to be kept
	*/
	cam_exist_option.Show_One_User_Option("camera");
	uo = useroption.Show_One_User_Option("H_SET_CAMERA");
	HC_Close_Segment();
	if( cam_exist_option.m_data && strcmp(cam_exist_option.m_data, "no") == 0 )
		return false;

	if(!uo)
		return false;

	HCLOCALE (sscanf(useroption.m_data, "%f %f %f %f %f %f %f %f %f %f %f %s",
		&cam.position.x, &cam.position.y, &cam.position.z, 
		&cam.target.x, &cam.target.y, &cam.target.z, 
		&cam.up_vector.x, &cam.up_vector.y, &cam.up_vector.z, 
		&cam.field_width, &cam.field_height, cam.projection));
	return true;
}


static
bool changes()
{
	char count_uo[10] = {"-1"};
	if(HC_Show_Existence("user options = contents count")){
		HC_Show_One_User_Option("contents count", count_uo);
	}

	int count = 0;
	HC_Begin_Contents_Search("...", "everything");
		HC_Show_Contents_Count(&count);
	HC_End_Contents_Search();
	if(count != atoi(count_uo)){
		HC_Set_User_Options(H_FORMAT_TEXT("contents count = %d", count));
		return true;
	}
	return false;
}

void HMarkupManager::SetLayerCondition(HC_KEY layerkey)
{ 
	HUserOptions owner_mdl;
	HUserOptions view_condition;
	HUserOptions view_section;
	HUserOptions cutting_planes;

	HC_Open_Segment_By_Key(layerkey);{
		owner_mdl.Show_One_User_Option("owner_mdl");
		view_condition.Show_One_User_Option("condition");
		cutting_planes.Show_One_User_Option("cp");
	}HC_Close_Segment();

	if(!owner_mdl.m_data || owner_mdl.m_data[0]==0 ||!view_condition.m_data || view_condition.m_data[0]==0)
		return;
	HC_Open_Segment(owner_mdl.m_data);{
		HC_Set_Conditions(view_condition.m_data);{
			if( cutting_planes.m_data && strcmp(cutting_planes.m_data, "yes") == 0 )
				HC_Set_Visibility("cutting planes = on");
			else
				HC_Set_Visibility("cutting planes = off");
			HC_Open_Segment(view_condition.m_data);
			HC_Set_Visibility("lines = on, text = on");
		}HC_Close_Segment();
	}HC_Close_Segment();
}


void HMarkupManager::UnsetCurrentLayerCondition()
 {
	UnsetLayerCondition(m_ActiveLayerKey);
}


void HMarkupManager::UnsetLayerCondition(HC_KEY layerkey)
{
	HUserOptions owner_mdl;
	HUserOptions view_condition;

	if (layerkey == INVALID_KEY)
		return;
	
	HC_Open_Segment_By_Key(layerkey);
	owner_mdl.Show_One_User_Option("owner_mdl");
	view_condition.Show_One_User_Option("condition");
	HC_Close_Segment();
	if(!owner_mdl.m_data || owner_mdl.m_data[0]==0 ||!view_condition.m_data || view_condition.m_data[0]==0)
		return;
	HC_Open_Segment(owner_mdl.m_data);
		HC_UnSet_One_Condition(view_condition.m_data);
		HC_Set_Visibility("cutting planes = off");
		HC_Open_Segment(view_condition.m_data);
			HC_Set_Visibility("lines = off, text = off");
		HC_Close_Segment();
	HC_Close_Segment();
}

void HMarkupManager::UpdatePreviewImage(bool forceUpdate)
{
	if(forceUpdate || changes()){
		HUserOptions suppress_preview_image_option;

		const char *sup_preview_image = suppress_preview_image_option.Show_One_User_Option("H_SUPPRESS_PREVIEW_IMAGE");
		if (strcmp(sup_preview_image,"") != 0)
			return;

		HCamera saved, cam;
		if(!GetCamera(m_ActiveLayerKey, cam))
			m_pHView->GetCamera(&cam);	
		m_pHView->GetCamera(&saved);
		m_pHView->SetCamera(cam);

		AxisMode sav_axismode = m_pHView->GetAxisMode();
		m_pHView->SetAxisMode(AxisOff);
		HC_Open_Segment("preview image");{
			HC_Set_Visibility("images = off");
			HC_KEY key = INVALID_KEY;
			HC_Begin_Contents_Search(".", "images");{
				HC_Find_Contents(0, &key);
			}HC_End_Contents_Search();
			if(key == INVALID_KEY)
				key = HC_KInsert_Image(0, 0, 0, "rgba", 255, 255, 0);
			HUtility::MakeViewSnapshot(m_pHView, key);
		}HC_Close_Segment();
		m_pHView->SetAxisMode(sav_axismode);
		m_pHView->SetCamera(saved);
		changes();
	}
}

void HMarkupManager::OpenLayerInternal(HC_KEY layerkey, bool setcamera)
{
	if (setcamera)
	{	
		m_ActiveLayerKey = INVALID_KEY;

		HCamera cam;
		if(GetCamera(layerkey, cam)){
			if (m_pHView->GetSmoothTransition())
			{
				HCamera orig;

				m_pHView->GetCamera(&orig);
				m_pHView->SetCamera(orig);
				HC_Open_Segment_By_Key(m_pHView->GetSceneKey());{
					HUtility::SmoothTransition(orig, cam, m_pHView);
				}HC_Close_Segment();
			}
			else 
				m_pHView->SetCamera(cam);

			m_pHView->CameraPositionChanged(true);
		}
		m_ActiveLayerKey = layerkey;
	}

	HC_Open_Segment_By_Key(layerkey); 
		HC_UnSet_Visibility();
		HC_UnSet_One_Rendering_Option("attribute lock");
	HC_Close_Segment();

	SetLayerCondition( layerkey);

	m_pHView->Update();
}


void HMarkupManager::DeleteLayer(const char *layername)
{
	HC_Open_Segment_By_Key(m_MarkupLayerKey); {
		HC_KEY k = FindLayer(layername);
		if (k != INVALID_KEY){
			DeleteLayer(k);
		}
	} HC_Close_Segment();
}


void HMarkupManager::DeleteLayer(HC_KEY layerkey)
{
	HC_Delete_By_Key(layerkey);
	if (m_ActiveLayerKey == layerkey)
		m_ActiveLayerKey = INVALID_KEY;
	if (m_DefaultLayerKey == layerkey)
		m_DefaultLayerKey  = INVALID_KEY;
}

HC_KEY HMarkupManager::RenameLayer(HC_KEY laykey, char const * new_name)
{
	HC_KEY k = CopyLayer(laykey, new_name);
	HC_Delete_By_Key(laykey);
	return k;
}

HC_KEY HMarkupManager::CopyLayer(HC_KEY laykey, char const * new_name)
{
	UnsetCurrentLayerCondition();
	DisableCurrentLayer();
	HC_KEY key = INVALID_KEY;
	HC_Open_Segment_By_Key(HC_KShow_Owner_By_Key(laykey));{
		char name[MVO_BUFFER_SIZE] = {"trash"};
		HC_Show_Segment(laykey, name);
		char * name_old = strrchr(name, '/');
		++name_old;
		if(HC_QShow_Existence(new_name, "self"))
			HC_Delete_Segment(new_name);
		HC_Copy_Segment(name_old, new_name);
		key = HC_KOpen_Segment(new_name);
		HC_Close_Segment();
	}HC_Close_Segment();
	OpenLayer(key);
	return key;
}

char *HMarkupManager::FindValidLayerName(char *newlayername)
{
	int c = 0;
	HC_Open_Segment_By_Key(m_MarkupLayerKey); 

	do {
		c++;
		sprintf(newlayername, "l%d", c);
	}while (HUtility::SegmentExists(newlayername));
	HC_Close_Segment();
	return newlayername;
}

HC_KEY
HMarkupManager::FindLayer(const char *layername) const
{
	HC_KEY key = INVALID_KEY;
	HC_Open_Segment_By_Key(m_MarkupLayerKey); {
		if(!streq(layername, "") && HC_QShow_Existence(layername, "self")){
			key = HC_KCreate_Segment(layername);
		}
	}HC_Close_Segment();
	return key;
}

char const *
HMarkupManager::GetLayerNames() const
{
	if(m_allNames){
		free(m_allNames);
		m_allNames = 0;
	}
	H_FORMAT_TEXT name_list;

	HC_Open_Segment_By_Key(m_MarkupLayerKey); {
		char name[MVO_BUFFER_SIZE] = {"trash"};
		HC_Show_Segment(m_MarkupLayerKey, name);
		HC_Begin_Contents_Search(".", "segments");{
			HC_KEY key;
			while(HC_Find_Contents(0, &key)){
				HC_Show_Segment(key, name);
				name_list.Append("%s;", strrchr(name, '/') +1);
			}
		}HC_End_Contents_Search();
	}HC_Close_Segment();

	m_allNames = strdup(name_list);
	return m_allNames;
}
HC_KEY HMarkupManager::OpenLayer(const char *layername, bool setcamera, bool emit_message)
{
    char seg[4096];
    HC_Show_Segment(m_MarkupLayerKey, seg);
	HC_Open_Segment_By_Key(m_MarkupLayerKey); {
		if (strcmp("", layername) == 0 || !HUtility::SegmentExists(layername))
		{						
			UnsetCurrentLayerCondition();
			DisableCurrentLayer();
			
			if (strcmp("",layername) == 0 && m_DefaultLayerKey != INVALID_KEY)
			{
				m_ActiveLayerKey = m_DefaultLayerKey;
				HC_Open_Segment_By_Key(m_DefaultLayerKey); 
				HC_Flush_Contents(".", "everything");
			}
			else
			{
				if (strcmp("",layername) == 0)
				{
					char newlayername[MVO_BUFFER_SIZE];
					m_ActiveLayerKey = HC_KOpen_Segment(FindValidLayerName(newlayername)); 
				}
				else
					m_ActiveLayerKey = HC_KOpen_Segment(layername);
			}
			SetupNewLayer();
			HC_Close_Segment();
		}
		else
		{			
			HC_KEY layerkey = HC_KOpen_Segment(layername); {
			OpenLayer(layerkey, setcamera);
			} HC_Close_Segment();
		}
	} HC_Close_Segment();

	// HNet Work
	if (emit_message)
		EmitSetMarkupLayer(layername);

	return m_ActiveLayerKey;
}


HC_KEY HMarkupManager::OpenLayer(HC_KEY layerkey, bool setcamera)
{
	if (layerkey != m_ActiveLayerKey)
	{		
		HC_Open_Segment_By_Key(layerkey); {
				UnsetCurrentLayerCondition();
 				DisableCurrentLayer();
				m_ActiveLayerKey = layerkey;
				HC_Open_Segment_By_Key(m_ActiveLayerKey); {
					OpenLayerInternal(m_ActiveLayerKey, setcamera);
				} HC_Close_Segment();
			
		} HC_Close_Segment();
	}
	return m_ActiveLayerKey;
}
 
  
void HMarkupManager::SetMarkupWeight(float weight)
{
	m_fMarkupWeight = weight; 

	if (m_ActiveLayerKey != INVALID_KEY)
	{
		HC_Open_Segment_By_Key(m_ActiveLayerKey);
			HC_Set_Line_Weight(m_fMarkupWeight);
			HC_Set_Edge_Weight(m_fMarkupWeight);
		HC_Close_Segment();
		m_pHView->Update();
	}
}

void HMarkupManager::SetMarkupColor(HPoint markup_color)
{
	m_MarkupColor.Set(&markup_color);

	if (m_ActiveLayerKey != INVALID_KEY)
	{
		HC_Open_Segment_By_Key(m_ActiveLayerKey);
			HC_Set_Color_By_Value("everything", "RGB", m_MarkupColor.x, m_MarkupColor.y, m_MarkupColor.z);
		HC_Close_Segment();
		m_pHView->Update();
	}
}

void HMarkupManager::InsertMarkupSegmentFromMessage(const char *in_data, unsigned int data_length)
{
	// NOTE: TO MAITAIN THE SANITY OF STREAMTOOLKIT, WE SHOULD HAVE CALLED A tk->Restart() 
	// HERE. BUT SINCE THERE IS NOT WAY TO IDENTIFY WHEN THIS FUNCTION IS CALLED 
	// IN THE BEGINING OF FRESH READ, I AM PLACING tk->Restart() IN FlushFromMessage()
	// ENSURE THAT FlushFromMessage() IS CALLED BEFORE YOUR FIRST CALL HERE - RAJESH B

    char *data;
    data = new char [data_length+1];
    char segment[MVO_SEGMENT_PATHNAME_BUFFER];
    char encoded_segment_name[MVO_SEGMENT_PATHNAME_BUFFER];
    char *encoded_hsf_data;
    char *decoded_hsf_data;
    
    encoded_hsf_data = new char [data_length];
    decoded_hsf_data = new char [data_length];

    memcpy(data, in_data, data_length);
    data[data_length] = '\0';
	    
    sscanf(data, "%s", encoded_segment_name);

    POINTER_SIZED_UINT n;
	unsigned long seg_length = (unsigned long) strlen(encoded_segment_name);
    unsigned long hsf_data_length = data_length - seg_length - 1;

    HUtility::URIdecode((const char *)encoded_segment_name, segment, &n);
    segment[n] = '\0';

	HStreamFileToolkit* tk = m_pHView->GetModel()->GetStreamFileTK();

	TK_Status status;

	// let's create a new layer if one is not active.  this should only
	// happen when a slave moves his camera while a the master is creating
	// markup data
	if (m_ActiveLayerKey == INVALID_KEY)
		return;

    HC_Open_Segment_By_Key(m_ActiveLayerKey);

		int count=0;
		static bool newHSFData = true;
		HC_Begin_Segment_Search(segment);
		HC_Show_Segment_Count(&count);
		HC_End_Segment_Search();

		if (newHSFData)
		{
			newHSFData = false;
			tk->Restart();

			if (count>0) 
				HC_Delete_Segment(segment);
		}

		HC_Open_Segment(segment);
			status = tk->ParseBuffer(in_data+seg_length+1, hsf_data_length);
		HC_Close_Segment();

	HC_Close_Segment();

	if (status == TK_Complete)
	{
		tk->Restart();
		m_pHView->Update();
		newHSFData=true;
	}

	delete [] encoded_hsf_data;
	delete [] decoded_hsf_data;
	delete [] data;
}


void HMarkupManager::EmitMarkupSegment(HC_KEY key)
{
// setup the stream toolkit nicely
	HStreamFileToolkit* tk = m_pHView->GetModel()->GetStreamFileTK();
	tk->Restart();
//	m_pHView->m_pSharedKey->SetupStreamToolkit(tk);

	char buffer[32768];
	int bytes_written;			
	TK_Status status;

	char pathname[MVO_SEGMENT_PATHNAME_BUFFER]; 
	char seg_name[MVO_SEGMENT_PATHNAME_BUFFER];

	HC_Show_Segment(key, pathname);
	HC_Parse_String(pathname, "/", -1, seg_name);

	HC_Open_Segment_By_Key(key);
		do
		{
			status =tk->GenerateBuffer(buffer, 32768, bytes_written, ".");

			HBaseViewEmitMessageFunction emit_message_function = m_pHView->GetEmitMessageFunction();

			if(emit_message_function) {

				char encoded_segment_name[MVO_SEGMENT_PATHNAME_BUFFER];
				HUtility::URIencode(seg_name, (int) strlen(seg_name), encoded_segment_name);
				
				size_t length = strlen(encoded_segment_name) + 32 + bytes_written;
				char *message;
				message = new char [length];
				sprintf(message, "H_INSERT_MARKUP_LAYER_HSF_DATA %s ", encoded_segment_name);
				
				int header_length = (int) strlen(message);
				memcpy(&message[header_length], buffer, bytes_written);
				
				emit_message_function((const char *)message, bytes_written+header_length, m_pHView->GetEmitMessageFunctionUserData());
				
				delete []message;
				}

		}while (status != TK_Complete);
	HC_Close_Segment();

	tk->Restart();
}


void HMarkupManager::SetMarkupLayerFromMessage(const char *data, unsigned int data_length)
{
	if (data_length > 0)
	{
		char in_data[MVO_MESSAGE_BUFFER_SIZE];
		char segment[MVO_SEGMENT_PATHNAME_BUFFER];
		char encoded_segment_name[MVO_SEGMENT_PATHNAME_BUFFER];
    
		memcpy(in_data, data, data_length);
		in_data[data_length] = '\0';

		sscanf(in_data, "%s", encoded_segment_name);

		POINTER_SIZED_UINT n;
		HUtility::URIdecode((const char *)encoded_segment_name, segment, &n);
		segment[n] = '\0';

		OpenLayer(segment, true, false);
	}
	else
		OpenLayer("", true, false);
}


void HMarkupManager::EmitSetMarkupLayer(const char *layer)
{
	HBaseViewEmitMessageFunction emit_message_function = m_pHView->GetEmitMessageFunction();

	if(emit_message_function) { 
		
		char encoded_layer_name[MVO_SEGMENT_PATHNAME_BUFFER];
		HUtility::URIencode(layer, (int) strlen(layer), encoded_layer_name);

		char message[MVO_BUFFER_SIZE];
		sprintf(message, "H_SET_MARKUP_LAYER %s", encoded_layer_name);

		emit_message_function((const char *) message, 0, m_pHView->GetEmitMessageFunctionUserData());
	}
}


void HMarkupManager::DisableCurrentMarkupLayerFromMessage(const char * data, unsigned int data_length)
{
	UNREFERENCED(data);
	UNREFERENCED(data_length);

	DisableCurrentLayer(false);
	return;
}


void HMarkupManager::EmitDisableCurrentMarkupLayer()
{
	HBaseViewEmitMessageFunction emit_message_function = m_pHView->GetEmitMessageFunction();

	if(emit_message_function) { 			
		emit_message_function("H_DISABLE_CURRENT_MARKUP_LAYER", 0, m_pHView->GetEmitMessageFunctionUserData());
	}
}

HC_KEY  HMarkupManager::GetCurrentLayerKey() const
{
	return m_ActiveLayerKey;
}

char const *  HMarkupManager::GetCurrentLayerName() const
{
	if(m_ActiveLayerName)
		free(m_ActiveLayerName);

	char path[MVO_BUFFER_SIZE] = {"trash"};

	if(m_ActiveLayerKey != INVALID_KEY)
		HC_Show_Segment(m_ActiveLayerKey, path);

	char const * s = strrchr(path, '/');
	if(s)
		m_ActiveLayerName = strdup(s +1);
	else 
		m_ActiveLayerName = 0;

	return m_ActiveLayerName;
}


void HMarkupManager::SetNoResetOnCameraChange()
{
	if (m_ActiveLayerKey == INVALID_KEY)
		return;
	HC_Open_Segment_By_Key(m_ActiveLayerKey);
	HC_Set_User_Options ("H_NO_RESET_ON_CAMERA_CHANGE = TRUE");
	HC_Close_Segment();
}


bool HMarkupManager::GetResetOnCameraChange(HC_KEY layerkey) 
{
	if (layerkey == INVALID_KEY)
		return false;
	HUserOptions useroption;
	HUserOptions reset_on_cam_option;
	char const * reset_on_cam_uo = 0;
	HC_Open_Segment_By_Key(layerkey); 
	reset_on_cam_uo = reset_on_cam_option.Show_One_User_Option("H_NO_RESET_ON_CAMERA_CHANGE");
	HC_Close_Segment();
	if( strcmp(reset_on_cam_uo,"") == 0)
		return true;
	return false;
}
