// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <wchar.h>

#include "HBaseModel.h"

#include "HDB.h"
#include "HBaseView.h"
#include "HModelInfo.h"
#include "HUtility.h"
#include "HUtilityShadow.h"
#include "HIOManager.h"
#include "HBhvBehaviorManager.h"
#include "HStream.h"
#include "HEventManager.h"
#include "HDispatchXML.h"
#include "HUtilityAnnotation.h"
#include "vlist.h"


HBaseModel::HBaseModel(char const * model)
{

	m_ModelKey = 0;
	m_ModelIncludeKey = -1;
	m_pModelInfo = 0;

	m_pHFile = 0;

	m_bFileLoadComplete = true;
	m_bFirstFitComplete = false;
	m_bObjectCheck = false;

	m_bBRepGeometry = false;
	m_bFEAGeometry = false;
	m_bVectorGeometry = false;
	m_bNeedsStencilBuffer = false;

	m_pBhvBehaviorManager = 0;
	m_pEventManager = 0;
	m_pKeyframeGenerator = 0;
	m_pSegmentTracker = 0;
	
	if (model)
		 m_ModelKey = HC_KCreate_Segment(model);
	else {
		char model_name[MVO_BUFFER_SIZE];
		sprintf (model_name, "?Include Library/model_%d", HDB::GetUniqueID());
		m_ModelKey = HC_KCreate_Segment(model_name);
	}
	

    m_pDispatchXML = 0;
}






HBaseModel::~HBaseModel()
{
	if (m_pBhvBehaviorManager)
	{
		m_pBhvBehaviorManager->SetActiveView(0);
		delete m_pBhvBehaviorManager;
	}

	if (m_ModelKey)
	{
		// delete all the included stuff
		DeleteIncludedSegments();
	
		// then delete the segment
		HC_Delete_By_Key(m_ModelKey);
	}
	
	if (m_pModelInfo)
		delete m_pModelInfo;

	if (m_pHFile)
		delete m_pHFile;

 
	if (m_pEventManager)
		delete m_pEventManager;

	HSmoothShadow::UnDefineAllShadowTextures(m_ModelKey);

	delete m_pDispatchXML;
}



void HBaseModel::Init()
{
	m_pModelInfo = new HModelInfo(m_ModelKey);

	m_pHFile = new HStreamFileToolkit;
}

HModelInfo * HBaseModel::GetHModelInfo() const
{

	if(!m_pModelInfo)
		m_pModelInfo = new HModelInfo();

	m_pModelInfo->ComputeModelInfo(m_ModelKey);

	return m_pModelInfo;
}



void HBaseModel::Flush()
{
	bool have_cube_map=false;

	DeleteIncludedSegments();

	HC_Open_Segment_By_Key(m_ModelKey);
		if(HC_QShow_Existence("cubemap_images", "self")) {
			have_cube_map=true;
			HC_Move_Segment("cubemap_images", "/tmp_cubemap_images");
		}
	HC_Close_Segment();

	HC_Open_Segment_By_Key(m_ModelKey);
		HC_Flush_Contents(".", "everything");	
	HC_Close_Segment();

	CreateIncludeSegment();


	if (have_cube_map) {
		HC_Open_Segment_By_Key(m_ModelKey);
			HC_Move_Segment("/tmp_cubemap_images", "cubemap_images");
		HC_Close_Segment();
	}

	m_bFirstFitComplete = false;
	m_bFileLoadComplete = true;

}

void HBaseModel::SetFileLoadComplete(bool value, HBaseView * pHView) 
{
	m_bFileLoadComplete=value;

	if(value == true) {
		
		if (GetObjectCheck() == true)
			HObjectManager::FileLoadHObjectCheck(m_ModelKey);

		
		if (pHView) {

			if (GetObjectCheck() == true) {
				pHView->SetAnnotationResize(true);
				HUtilityAnnotation::WindowResizeCallback(0, 0, pHView);
			}
		
			HC_Open_Segment_By_Key(m_ModelKey);
				if(HC_QShow_Existence("cubemap_images", "self")) {

					HC_KEY image_seg = HC_KCreate_Segment("cubemap_images");

					// this needs to be improved to check the correct style
					if (HC_Show_Existence_By_Key(pHView->GetViewKey(), "style") == 0) {
						HC_Open_Segment_By_Key(pHView->GetViewKey());
							HC_Style_Segment_By_Key(image_seg);
							HC_Style_Segment_By_Key(pHView->GetSceneKey());
						HC_Close_Segment();		
					}
					
					pHView->SetWindowMaterial("cubemap");

				}
			HC_Close_Segment();
		}
	}
}

void HBaseModel::SetStaticModel(bool value)
{
	HC_Open_Segment_By_Key(m_ModelKey);
	if(value)
		HC_Set_Heuristics("static model");
	else
		if(GetStaticModel())
			HC_UnSet_One_Heuristic("static model");
	HC_Close_Segment();
}

bool HBaseModel::GetStaticModel() const
{
	int found;
	
	HC_Open_Segment_By_Key(m_ModelKey);
		found=HC_Show_Existence("static model");
	HC_Close_Segment();

	return found!=0;
}

bool HBaseModel::GetStaticModelOn() const
{	
	bool status = false;
	HC_Open_Segment_By_Key(m_ModelKey);
		if (HC_Show_Existence("heuristics=static model")) {
			char setting[32];
			HC_Show_One_Heuristic("static model", setting);
			if (setting[0] == 'o' && setting[1] == 'n')
				status = true;
		}
	HC_Close_Segment();

	return status;
}

void HBaseModel::SetLMVModel(bool value)
{
	HC_Open_Segment_By_Key(m_ModelKey);
	if(value)
		HC_Set_Heuristics("model type=lmv");
	else
		if(HC_Show_Existence("heuristics=model type"))
			HC_UnSet_One_Heuristic("model type");
	HC_Close_Segment();
}

bool HBaseModel::GetLMVModel() const
{
	bool status=false;
	
	HC_Open_Segment_By_Key(m_ModelKey);
		if (HC_Show_Existence("heuristics=model type")) 
		{
			char setting[MVO_BUFFER_SIZE];
			HC_Show_One_Heuristic("model type", setting);
			if (strstr(setting, "lmv"))
				status = true;
		}
	HC_Close_Segment();

	return status;
}

void HBaseModel::DeleteIncludedSegments()
{
	char type[MVO_BUFFER_SIZE], scope[MVO_BUFFER_SIZE], status[MVO_BUFFER_SIZE];
	char segname[MVO_SEGMENT_PATHNAME_BUFFER];
	char *cp;
	HC_KEY key, new_key, renumbered;

	// first let's clean up any segments it includes
	HC_Open_Segment_By_Key(m_ModelKey);
		HC_Begin_Contents_Search("...", "includes");

		while (HC_Find_Contents(type, &key))
		{
			HC_Show_Key_Status (key, status);
			if( streq( status, "invalid" ))
				continue;
			if( key >= 0 ) {
				renumbered = key;
				HC_Show_Key_Status (key, scope);
				key = HC_KRenumber_Key (key, INVALID_KEY, "irrelevant");
				HC_Renumber_Key (key, renumbered, scope);
			}
			new_key = HC_KShow_Include_Segment (key);

			// we may need some special handstanding for includes that refer to
			// things that have been locally renumbered
			if (new_key >= 0) {

				//get the full pathname of the referee
				HC_Show_Include_Segment (key, segname);
				//strip the last part get the full pathname of the parent
				cp = strrchr (segname, '/');
				if (cp != segname)
					*cp = '\0';
				else
					segname[1] = '\0';
				//open the parent
				HC_Open_Segment (segname); 
					//get the internal key used by HOOPS (always unique) 
					renumbered = new_key;
					HC_Show_Key_Status (new_key, scope);
					new_key = HC_KRenumber_Key (new_key, INVALID_KEY, "irrelevant");
					HC_Renumber_Key (new_key, renumbered, scope);
				HC_Close_Segment ();
			}

			HC_Delete_By_Key(new_key);
		}

		HC_End_Contents_Search();
	HC_Close_Segment();

}



void HBaseModel::CleanIncludes()
{
	char child_segment_name[MVO_SEGMENT_PATHNAME_BUFFER];
	char child_segment_name_simple[MVO_SEGMENT_PATHNAME_BUFFER];

	CreateIncludeSegment();
	HC_Open_Segment("?Include Library");

		HC_Begin_Segment_Search("*");
		while (HC_Find_Segment(child_segment_name))
		{
			HC_Parse_String(child_segment_name,"/",-1,child_segment_name_simple);
			if ((!memcmp(child_segment_name_simple, "model_", strlen("model_"))==0 &&
				!memcmp(child_segment_name_simple, "references", strlen("references"))==0 ) ||
				strstr(child_segment_name_simple, "include"))
			{

 				HC_Open_Segment_By_Key(m_ModelIncludeKey);
					
					if (HC_QShow_Existence(child_segment_name_simple, "self")) 
					{	// wrapper
						HC_Open_Segment("");
						HC_Rename_Segment(child_segment_name,"./*");
						HC_Close_Segment();
					}
					else
						HC_Rename_Segment(child_segment_name,"./*");

				HC_Close_Segment();
			}
		}
		HC_End_Segment_Search();

	HC_Close_Segment();
}




HFileInputResult HBaseModel::Read(const wchar_t * FileName, HBaseView * pHView, bool clean_include, HInputHandlerOptions * options) 
{   
 #define NOMINAL_MAX_FILENAME_LENGTH (4096+1)
	wchar_t ext[NOMINAL_MAX_FILENAME_LENGTH];

	if(!FileName || wcslen(FileName) > NOMINAL_MAX_FILENAME_LENGTH)
		return InputBadFileName;

	HUtility::FindFileNameExtension(FileName, ext);

	// hmf is the default handler
	if(ext[0] == 0)
		wcscpy(ext, L"hmf");

	//treat psf files as hsf
	if(wcseq(ext, L"psf"))
		wcscpy(ext, L"hsf");

	// find handler for type
	HInputHandler * handler = HDB::GetHIOManager()->GetInputHandler(ext);

	// if none found the quit
	if(!handler) return InputNotHandled;

	m_bFileLoadComplete = false;
	m_bFirstFitComplete = false;

	HFileInputResult result;

	if(pHView) {

		result = pHView->FileInput(FileName, handler, options);
	}else{

		if(handler->GetInputStyle() == HInputHandlerStyleModel) {

			result = handler->FileInputByKey(FileName, m_ModelKey, options);
		}
		else if(handler->GetInputStyle() == HInputHandlerStyleImage) {

			HC_KEY image_key;

			result = handler->FileInputToImageKey(FileName, &image_key, GetModelKey(), options);
		}else
			result = InputNotHandled;
	}

	if(result == InputOK) {

		HC_Open_Segment_By_Key(m_ModelKey);
			
		MVO_POINTER_SIZED_INT    value;

		if (clean_include)
			CleanIncludes();

		if (HC_Show_Existence("user value"))
		{
			HC_Show_User_Value(&value);

			if (value == 1)
				m_bBRepGeometry = true;
			else if (value == 2)
				m_bFEAGeometry = true;
			else if (value == 3)
				m_bVectorGeometry = true;
		}
		else
			m_bBRepGeometry = m_bFEAGeometry = m_bVectorGeometry = false;

		HC_Close_Segment();
	}

	SetFileLoadComplete(true, pHView);

    return result;
}

HFileOutputResult HBaseModel::WriteWithOptions(const wchar_t * FileName, HBaseView * pHView, HOutputHandlerOptions * options) 
{
#define NOMINAL_MAX_FILENAME_LENGTH (4096+1)
	wchar_t ext[NOMINAL_MAX_FILENAME_LENGTH];

	if(!FileName || wcslen(FileName) > NOMINAL_MAX_FILENAME_LENGTH) return OutputBadFileName;

	HUtility::FindFileNameExtension(FileName, ext);

	// hmf is the default handler
	if(ext[0] == 0)
		wcscpy(ext, L"hmf");

	// find handler for type
	HOutputHandler * handler = HDB::GetHIOManager()->GetOutputHandler(ext);

	// if none found the quit
	if(!handler) return OutputNotHandled;

    HC_Open_Segment_By_Key(m_ModelKey);

		if (m_bBRepGeometry)
			HC_Set_User_Value(1);
		else if (m_bFEAGeometry)
			HC_Set_User_Value(2);
		else if (m_bVectorGeometry)
			HC_Set_User_Value(3);

	HC_Close_Segment();
	
	HFileOutputResult result;

	if(pHView) {

		result = pHView->FileOutput(FileName, handler, options);
	}else{

		if(handler->GetOutputStyle() == HOutputHandlerStyleModel) {
			result = handler->FileOutputByKey(FileName, m_ModelKey, options);
		}else{
			//TODO what to do for non-view base output
			return OutputNotHandled;
		}
	}

	return result;
}




HFileOutputResult HBaseModel::Write(const wchar_t * FileName, HBaseView * pHView, float width, float height) 
{
	HOutputHandlerOptions options;
	options.m_Window_Width=width;
	options.m_Window_Height=height;
	options.m_Paper_Width=width;
	options.m_Paper_Height=height;
	return WriteWithOptions( FileName, pHView, &options);
}

HFileOutputResult HBaseModel::WriteWithAnimation(const wchar_t * FileName, HBaseView * pHView, float width, float height) 
{
	HOutputHandlerOptions options;
	options.m_Window_Width=width;
	options.m_Window_Height=height;
	options.m_Paper_Width=width;
	options.m_Paper_Height=height;
	options.m_pHBaseModel = this;
	options.m_bWriteAnimations = true;
	return WriteWithOptions( FileName, pHView, &options);
}

HBhvBehaviorManager * HBaseModel::GetBhvBehaviorManager()
{
	if(!m_pBhvBehaviorManager) {
		m_pBhvBehaviorManager = new HBhvBehaviorManager(this);
	}

	return m_pBhvBehaviorManager;
}


void  HBaseModel::SetBhvBehaviorManager( HBhvBehaviorManager* BehaviorManager )
{
	if (m_pBhvBehaviorManager)
		delete m_pBhvBehaviorManager;
	 m_pBhvBehaviorManager = BehaviorManager; 
}

HEventManager * HBaseModel::GetEventManager()
{
	if(!m_pEventManager) {
		m_pEventManager = new HEventManager();
	}

	return m_pEventManager;
}

void HBaseModel::Update(bool antialias, bool forceUpdate)
{
	HLISTENER_EVENT(HUpdateListener, GetEventManager(), UpdateEvent(antialias, forceUpdate));
}


void HBaseModel::MakeAnimationSnapshot(int tick)
{
	UNREFERENCED(tick);

	if (!m_pKeyframeGenerator)
	{
		m_pKeyframeGenerator = new HBhvKeyframeGenerator(this);
		m_pKeyframeGenerator->StartFrames();
	}
	m_pKeyframeGenerator->NewFrame();
	/*
	if (!m_pSegmentTracker)
	{
		m_pSegmentTracker = new HBhvSegmentTracker(this);
//		m_pKeyframeGenerator->StartFrames();
	}
	m_pSegmentTracker->MakeTreeSnapshot();
*/
}


void HBaseModel::CreateIncludeSegment()
{
	HC_Open_Segment_By_Key(m_ModelKey);
    m_ModelIncludeKey = HC_KOpen_Segment("model_include");
	HC_Set_Visibility("off");
	HC_Set_Rendering_Options("attribute lock = visibility");
	HC_Set_Heuristics("exclude bounding");
	HC_Close_Segment();
	HC_Close_Segment();
}






