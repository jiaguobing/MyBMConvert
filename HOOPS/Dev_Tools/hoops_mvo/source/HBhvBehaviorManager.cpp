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
#include <wchar.h>

#include "HBaseModel.h"
#include "HBaseView.h"
#include "HBhvBehaviorManager.h"
#include "HBhvAnimation.h"
#include "HBhvTimeline.h"
#include "HBhvInterpolator.h"
#include "HBhvSensor.h"
#include "HUtilityXMLParser.h"
#include "HUtilityTextParser.h"
#include "HEventManager.h"
#include "HStream.h"
#include "HDB.h"
#include "vlist.h"
#include "vhash.h"


void HBhvTargetObject::Serialize(HUtilityXMLGenerator *xmlgen)
{
	if (strcmp(m_Type, "SPATH") == 0)
	{
		HUtilityXMLTag xmlt;
		xmlt.SetTagname("TargetObject");
		if (strcmp(m_Name, "") != 0)
			xmlt.AddProperty("Name", m_Name, true);

 		if (m_bSerializeFromKey && m_CameraType==NoCamera)
		{
			char full_path[4096];
 			HC_Show_Segment(m_key, full_path);
			char modelsegment[4096];
			HC_Show_Segment(m_pBehaviorManager->GetActiveView()->GetModel()->GetModelKey(), modelsegment);
			if (strstr(full_path, modelsegment) == full_path)
			{				
 				char partial_path[4096];
 				strcpy(partial_path, full_path + strlen(modelsegment));
				sprintf(m_Path,"SPATH:MODEL%s", partial_path);
			}
			else				
			{
				sprintf(m_Path,"SPATH:%s", full_path);
			}
	 		xmlt.AddProperty("Path", GetPath(), true);
		}
		else
	 		xmlt.AddProperty("Path", GetPath(), true);	
		char buffer[MVO_BUFFER_SIZE];
		HCLOCALE(sprintf(buffer, "[%f %f %f]", m_Pivot.x, m_Pivot.y, m_Pivot.z));
		xmlt.AddProperty("Pivot", buffer, true);
		xmlt.AddPropertyInt("Collision",GetCollision());
		xmlt.SetOpen(false);
		xmlgen->AddTag(&xmlt);
	}
}



 
void *HBhvTargetObject::XMLCallback(HUtilityXMLTag *xt, bool open, void *m_pExtraData)
{
	char Name[MVO_BUFFER_SIZE];
	char Path[MVO_SEGMENT_PATHNAME_BUFFER];
	char Pivot[MVO_BUFFER_SIZE];
	HPoint pv;
	bool collision = false;
  	HBaseModel *model = (HBaseModel *)m_pExtraData;
	if (open)
	{
		if (!xt->GetProperty("Name", Name))
			strcpy(Name, "");
		if (!xt->GetProperty("Path", Path))
			strcpy(Path, "");
		if (!xt->GetProperty("Pivot", Pivot))
			pv.Set(0,0,0);
		else
			HUtilityXMLParser::GetFloatPoint(Pivot, pv);

		if (!xt->GetProperty("Collision", collision))
				 collision = false;



		HBhvTargetObject *to = model->GetBhvBehaviorManager()->CreateTargetObjectByPath(Name, Path);
		to->SetPivot(pv);
		to->SetCollision(collision);
 
	}
	return 0;

}


HBhvTargetObject::HBhvTargetObject(HBhvBehaviorManager *BehaviorManager, const char *name, const char *path, HPoint *pivot)
{
	m_pBehaviorManager = BehaviorManager;
	m_bSerializeFromKey = true;	
	if (name)
		strcpy(m_Name, name);
	else 
		strcpy(m_Name, "");

	if (path)
		strcpy(m_Path, path);
	else
		strcpy(m_Path, "");

	if (pivot)
		m_Pivot = pivot;
	else 
		m_Pivot.Set(0,0,0);

	m_CameraType = NoCamera;

	strcpy(m_Path, path);
 	ResolveTarget(m_Path, m_ResolvedPath, m_Type, BehaviorManager->GetModel(), m_CameraType);
	if (strcmp(m_Type, "SPATH") == 0 && m_CameraType == NoCamera)
	{
		m_key = HC_KOpen_Segment(m_ResolvedPath);
		HC_Close_Segment();
	}
	else if (strcmp(m_Type, "TID") == 0)
	{
		int identifier = atoi(m_ResolvedPath);
		m_pBehaviorManager->GetActiveView()->GetModel()->GetStreamFileTK()->IndexToKey(identifier, m_key);
	}
	else
		m_key = INVALID_KEY;

	m_bHasMoved = false;
	m_bCollision = false;

/*
 	m_ChildTargetList = new_vlist( malloc, free );
	m_ParentTarget = 0;
	m_constrainpoint.Set(2.75, 0.5, 1.5);
	m_limitaxis.Set(0,0,1);
	*/
}

/*
static float ComputeAngleBetweenVector(HPoint p1, HPoint p2)
{
	HPoint p3;
    HC_Compute_Normalized_Vector(&p1,&p1);
    HC_Compute_Normalized_Vector(&p2,&p2);
    HC_Compute_Cross_Product(&p1,&p2,&p3);

    float dprod = (float)HC_Compute_Dot_Product(&p1,&p2);
    float l=(float)HC_Compute_Vector_Length(&p3);
   float   ang = (float)atan2(l,dprod);
    ang*=(180.0f/(float)3.1415927f);
	return ang;

}


void HBhvTargetObject::AddRotationToMatrix(float rot[16], float outrot[16])
{
	HPoint pivot;
 	float translation_matrix[16], pivot_translation_matrix[16], pivot_translation_matrix_inverse[16];
 	float result[16], result2[16];
 		
	HC_Open_Segment_By_Key(GetTargetKey());
		GetPivot(pivot);				
		HPoint op;
		GetTranslationFromMatrix(op);
  		HC_Compute_Translation_Matrix(op.x, op.y, op.z, translation_matrix);
		HC_Compute_Translation_Matrix(-pivot.x,-pivot.y,-pivot.z, pivot_translation_matrix);
		HC_Compute_Matrix_Inverse(pivot_translation_matrix, pivot_translation_matrix_inverse);	

  		HC_Compute_Matrix_Product(pivot_translation_matrix_inverse, rot, result);			
		HC_Compute_Matrix_Product(result, pivot_translation_matrix, result2);
 		HC_Compute_Matrix_Product(result2, translation_matrix, outrot);
		    	
//		HC_Set_Modelling_Matrix(result);
		HC_Close_Segment();			
}

void HBhvTargetObject::GetTranslationFromMatrix(HPoint &t)
{

	HPoint pivot;
	HC_Open_Segment_By_Key(GetTargetKey());
		float segment_modelling_matrix[16];		
		GetPivot(pivot);
 		if (HC_Show_Existence("modelling matrix"))
				HC_Show_Modelling_Matrix(segment_modelling_matrix);
		else
				HC_Compute_Identity_Matrix(segment_modelling_matrix); 
		HPoint op;
 		op.Set(-pivot.x, -pivot.y, -pivot.z);
 		HC_Compute_Transformed_Points(1, &op, segment_modelling_matrix, &op);
		t.Set(op.x + pivot.x, op.y + pivot.y, op.z + pivot.z);
	HC_Close_Segment();
}


void HBhvTargetObject::AddPositionToMatrix(HPoint &trans)
{
	float segment_modelling_matrix[16], translation_matrix[16], result[16], result2[16];
	float pivot_translation_matrix[16], pivot_translation_matrix_inverse[16];
	HPoint pivot;
 	HC_Open_Segment_By_Key(GetTargetKey());
  		HBhvUtility::ShowModellingMatrix(segment_modelling_matrix);	
		GetPivot(pivot);
		HC_Compute_Translation_Matrix(-pivot.x,-pivot.y,-pivot.z, pivot_translation_matrix);
		HC_Compute_Matrix_Inverse(pivot_translation_matrix, pivot_translation_matrix_inverse);	
  		segment_modelling_matrix[12] = segment_modelling_matrix[13] = segment_modelling_matrix[14] = 0;
  		HC_Compute_Translation_Matrix(trans.x, trans.y, trans.z, translation_matrix);
  		HC_Compute_Matrix_Product(pivot_translation_matrix_inverse, segment_modelling_matrix, result);		    	
  		HC_Compute_Matrix_Product(result, pivot_translation_matrix, result2);		    	
  		HC_Compute_Matrix_Product(result2, translation_matrix, result);		    	
		HC_Set_Modelling_Matrix(result);
	HC_Close_Segment();			
}



void HBhvTargetObject::CalculateAdjustedPosition(HBhvTargetObject *parent)
{
	float segment_modelling_matrix[16];
	HC_Open_Segment_By_Key(parent->GetTargetKey());
 	if (HC_Show_Existence("modelling matrix"))
		HC_Show_Modelling_Matrix(segment_modelling_matrix);
	else
		HC_Compute_Identity_Matrix(segment_modelling_matrix); 
	HC_Close_Segment();
	HPoint c;
	HC_Compute_Transformed_Points(1, &m_constrainpoint, segment_modelling_matrix, &c);
	HPoint p1, p2;
	HPoint piv2;
	piv2.Set(-m_Pivot.x, -m_Pivot.y, - m_Pivot.z);
	p1.Set(m_constrainpoint.x - piv2.x, m_constrainpoint.y - piv2.y, 
			m_constrainpoint.z - piv2.z);

	p2.Set(c.x - piv2.x, c.y- piv2.y, c.z - piv2.z);

	float angle = ComputeAngleBetweenVector(p1, p2);
	float matrix[16];
	float outrot1[16], outrot2[16];
   	HC_Compute_Offaxis_Rotation(m_limitaxis.x, m_limitaxis.y, m_limitaxis.z, -angle, matrix); 
	AddRotationToMatrix(matrix, outrot1);
  	HC_Compute_Offaxis_Rotation(m_limitaxis.x, m_limitaxis.y, m_limitaxis.z, angle, matrix); 
	AddRotationToMatrix(matrix, outrot2);

	HC_Open_Segment_By_Key(GetTargetKey());
  

 	HPoint c2;
	HC_Compute_Transformed_Points(1, &m_constrainpoint, outrot1, &c2);
	HPoint delta1, delta2;
	delta1.Set(c.x - c2.x, c.y - c2.y, c.z - c2.z);
	HC_Compute_Transformed_Points(1, &m_constrainpoint, outrot2, &c2);
	delta2.Set(c.x - c2.x, c.y - c2.y, c.z - c2.z);
	if (HC_Compute_Vector_Length(&delta1) < HC_Compute_Vector_Length(&delta2))
	{
		HC_Set_Modelling_Matrix(outrot1);
		AddPositionToMatrix(delta1);
	}
	else
	{
		HC_Set_Modelling_Matrix(outrot2);
		AddPositionToMatrix(delta2);
	}

//	AddPositionToMatrix(delta);
	
	HC_Close_Segment();
}


	


void HBhvTargetObject::AddChildTarget(HBhvTargetObject *child)
{
 	vlist_add_last(m_ChildTargetList, (void *)child);
}


void HBhvTargetObject::AdjustChildTargets()
{
	START_LIST_ITERATION(HBhvTargetObject, m_ChildTargetList)
		temp->CalculateAdjustedPosition(this);
		temp->AdjustChildTargets();
 	END_LIST_ITERATION(m_ChildTargetList)

}
*/
void HBhvTargetObject::CreateTargetString(const char *type, const char *path, char *result, bool addquotes)
{
	char temppath[MVO_SEGMENT_PATHNAME_BUFFER];
	strcpy(temppath, path);
	strcpy(result, type);
	strcat(result, ":");

	if (strcmp(type, "SPATH") == 0)
	{
		CollapseTarget(temppath, temppath);
		strcat(result, temppath);
	}
	else
		strcat (result, temppath);
 	if (addquotes)
	{
		char result2[MVO_SEGMENT_PATHNAME_BUFFER];
		sprintf(result2, "\"%s\"", result);
		strcpy(result, result2);
	}

}
	

bool HBhvTargetObject::IsEqual(const char *target)
{
	char result[MVO_SEGMENT_PATHNAME_BUFFER];
	char type[MVO_BUFFER_SIZE];
	HBhvCameraType ctype;
	ResolveTarget(target, result, type, m_pBehaviorManager->GetModel(), ctype);
	if (((strcmp(result, m_ResolvedPath) == 0) && ctype == m_CameraType) ||
		(strcmp(type, "TOB") == 0 && strcmp(result, m_Name) == 0) || 
		(strcmp(type, "TID") == 0 && strcmp(result, m_Name) == 0) ||
		(ctype == m_CameraType && ctype != NoCamera))
		return true;
	else
		return false;
}

void HBhvTargetObject::ResolveTarget(const char *in, char *result, char *TargetType, HBaseModel *model, HBhvCameraType &ctype)
{
	char target[MVO_SEGMENT_PATHNAME_BUFFER];
	char temp[MVO_SEGMENT_PATHNAME_BUFFER];
	strcpy(target, in);
	ctype = NoCamera;
	static delimiter_t find_end_td[] = {{"\x21-\x7a", false, ":", true, false}};
	static delimiter_t find_end_td2[] = {{"\x21-\x7a", false, "\x01", true, false}};

  	HUtilityTextParser tp;
 	tp.InsertBuffer(target, (unsigned int)strlen(target));
	int dlum;
	tp.ParseBuffer(TargetType, (int &)dlum, find_end_td, 1);
	tp.ParseBuffer(target, (int &)dlum, find_end_td2, 1);

	if (strcmp(TargetType, "TOB") == 0 || strcmp(TargetType, "TID") == 0 || strcmp(TargetType, "ANIM") == 0 || strcmp(TargetType, "SSR") == 0)
	{
		strcpy(result, target);
		return;
	}
 	else if (strstr(target, "MODEL") == target)
	{
		strncpy(temp, target + strlen("MODEL"), strlen(target) - strlen("MODEL") + 1);
		if (model)
			HC_Show_Segment(model->GetModelKey(), target);
		else
			strcpy(target, "/include library/model_0");
		strcat(target,temp);
	}
	else if (strstr(target, "SCENE") == target)
	{
 		if (model && model->GetBhvBehaviorManager()->GetActiveSceneKey() != INVALID_KEY)
		{
			strncpy(temp, target + strlen("SCENE"), strlen(target) - strlen("SCENE") + 1);
			HC_Show_Segment(model->GetBhvBehaviorManager()->GetActiveSceneKey(), target);
			strcat(target,temp);
		}
		else
		{
			strncpy(temp, target + strlen("SCENE"), strlen(target) - strlen("SCENE") + 1);
			strcpy(target, "driver/opengl/window0+0/scene");
			strcat(target,temp);

		}
	}
	char extraname[MVO_SEGMENT_PATHNAME_BUFFER];

	HC_Parse_String(target, "/", -1, extraname);
 	int len2 = 0;
	if (strcmp(extraname, "CAMERA") == 0 || strcmp(extraname, "TARGET") == 0 || strcmp(extraname, "POSITION") == 0 || strcmp(extraname, "TARGETFREE") == 0 || strcmp(extraname, "POSITIONFREE") == 0 || strcmp(extraname, "POSITIONTARGET") == 0)
	{
		if (strcmp(extraname, "POSITION") == 0)
			ctype = CameraPosition;
		else if (strcmp(extraname, "TARGET") == 0)
			ctype = CameraTarget;
		else if (strcmp(extraname, "TARGETFREE") == 0)
			ctype = CameraTargetFree;
		else if (strcmp(extraname, "POSITIONFREE") == 0)
			ctype = CameraPositionFree;
		else if (strcmp(extraname, "POSITIONTARGET") == 0)
			ctype = CameraPositionTarget;

		len2 = (int)(strlen(target) - strlen(extraname) -1);
	}
	else
		ctype = NoCamera;

	strcpy(result, target);
	if (len2)
		result[len2] = 0;

	return;
}


void HBhvTargetObject::CollapseTarget(const char *target, char *collapsedtarget)
{
	char temptarget[MVO_SEGMENT_PATHNAME_BUFFER];
	strcpy(temptarget, target);
	if (strstr(temptarget, "/include library/model") == temptarget)
	{
		char * pstart = strstr(temptarget + strlen("/include library/model"), "/");
		strcpy(collapsedtarget, "MODEL");
		if (pstart)
			strcat(collapsedtarget, pstart);
	}
	else if (strstr(temptarget, "/driver") == temptarget)
	{
		int skip = 1;
		int skiplen =0;
		for (int i=0;i<3;i++)
		{
			HC_Parse_String(target, "/", skip, temptarget);
			skiplen+=(int)(strlen(temptarget)+1);
			skip++;
		}

		const char * pstart = &target[skiplen+strlen("scene")+1];
		strcpy(collapsedtarget, "SCENE");
		strcat(collapsedtarget, pstart);
	}
	else 
 		strcpy(collapsedtarget, temptarget);
}

HC_KEY HBhvTargetObject::GetTargetKey() { 
	if (m_key == INVALID_KEY && m_CameraType != NoCamera)
		m_key = m_pBehaviorManager->GetActiveSceneKey();
	return m_key; 
}

HBhvBehaviorManager::HBhvBehaviorManager( HBaseModel *model , int tps, int delay,const char *name, const char *version, const char *vendor) : HTClient()
{
	m_pModel = model;

	if (name)

		strcpy(m_Name, name);
	else
		strcpy(m_Name, "");

 	if (version)
		strcpy(m_Version, version);
	else
		strcpy(m_Version, "");

	if (vendor)
		strcpy(m_Vendor, vendor);
	else
		strcpy(m_Vendor, "");

	m_Tps = tps;
	m_Delay = delay;
	m_StartTime = 0.0f;
  	m_AnimationList = new_vlist( malloc, free );
  	m_ScheduledAnimationList = new_vlist( malloc, free );
	m_TargetObjectList = new_vlist(malloc, free);
	m_SensorList = new_vlist(malloc, free);
	m_CurrentTick = 0;
 	m_bPlaying = false;
	m_pView = 0;
	m_bRenderEveryFrame = false;
	m_bUpdateCamera = true;
	m_bContinuousPlay = false;
	m_bCameraUpdated = false;
	m_bInfinitePlay = false;
	m_bMerge = false;
	m_bShellSelectionActive = false;
 }

void HBhvBehaviorManager::SetActiveView(HBaseView *view) {
	m_pView = view;
}

void HBhvBehaviorManager::DeleteAllAnimations()
{

	if (IsPlaying())
		Stop();	

	START_LIST_ITERATION(HBhvAnimation, m_AnimationList);{
		delete temp;
	}END_LIST_ITERATION(m_AnimationList);

	vlist_flush(m_AnimationList);


	vlist_flush(m_ScheduledAnimationList);


	START_LIST_ITERATION(HBhvTargetObject, m_TargetObjectList);{
		delete temp;
	}END_LIST_ITERATION(m_TargetObjectList);
	vlist_flush(m_TargetObjectList);
	
	START_LIST_ITERATION(HBhvSensor, m_SensorList);{
		delete temp;
	}END_LIST_ITERATION(m_SensorList);
	vlist_flush(m_SensorList);

}

HBhvBehaviorManager::~HBhvBehaviorManager()
{
	DeleteAllAnimations();

	delete_vlist(m_AnimationList);
 	delete_vlist(m_ScheduledAnimationList);
	delete_vlist(m_TargetObjectList);
	delete_vlist(m_SensorList);
}


void HBhvBehaviorManager::Rewind()
{
	if (IsPlaying())
		Stop();
	ScheduleAllAnimations(true);		
	SetCurrentTick(0);
 	ExecuteAnimations(GetCurrentTick(), -1.0f);		
	GetModel()->Update();
}


void HBhvBehaviorManager::Play()
{	
	if (!IsPlaying())
	{
		ScheduleAllAnimations(true);	
		HC_Show_Time( &m_StartTime );  
		SetInterval( 0.01f );
		SetStyle( HTCS_PeriodicSkip );
		SetNextRequest( m_StartTime + 0.01f );
		HDB::GetHTManager()->RegisterClient( this );
		m_bPlaying = true;
		m_LastTick = GetLastTick();
	}
	HC_Open_Segment_By_Key(m_pView->GetModelKey());{
		/* If it doesn't exist, create it. It's easier than
		 * searching for it first. */
		HC_Create_Segment("CMMDisplay");
		HC_Delete_Segment("CMMDisplay");
	}HC_Close_Segment();
}

bool HBhvBehaviorManager::IsAtFinalTick()
{
	if (GetLastTick()<=GetCurrentTick() && GetCurrentTick()>0)
		return true;
	else
		return false;
}

void HBhvBehaviorManager::Continue()
{	
	if (IsAtFinalTick())
		Rewind();
 	Play();	
	m_StartTime -= GetCurrentTick()/m_Tps; 
}


void HBhvBehaviorManager::Stop()
{
	if (IsPlaying())
	{
		HDB::GetHTManager()->UnRegisterClient( this ); 
		
		START_LIST_ITERATION(HBhvSensor, m_SensorList);{
			temp->DeActivate();
		}END_LIST_ITERATION(m_SensorList);
		START_LIST_ITERATION(HBhvAnimation, m_AnimationList);{
			temp->SetCurrentlyRunning(false);
		}END_LIST_ITERATION(m_AnimationList);
			m_bPlaying = false;
		m_bCameraUpdated = false;
	}
	if (m_bShellSelectionActive)
	{
		HC_End_Shell_Selection();
		m_bShellSelectionActive = false;
	}

}
 

HBhvTargetObject * HBhvBehaviorManager::CreateTargetObjectByPath(const char *name, const char *path)
{

	HBhvTargetObject *tobject;
	if ((tobject = FindTargetObjectByPath(path)) == 0)
	{
 		tobject = new HBhvTargetObject(this, name, path);
		AddTargetObject(tobject);
	}
	return tobject;
}
 

void HBhvBehaviorManager::AddTargetObject(HBhvTargetObject *tinfo)
{
 	
	vlist_reset_cursor(m_TargetObjectList);
	vlist_add_last(m_TargetObjectList, (void *)tinfo);
}

HBhvTargetObject * HBhvBehaviorManager::FindTargetObjectByName(const char *name)
{
 	
	vlist_reset_cursor(m_TargetObjectList);
 	while (1)
	{
		HBhvTargetObject *temp;
		if ((temp = (HBhvTargetObject *)vlist_peek_cursor(m_TargetObjectList)) == 0)
				break;
		if (strcmp(temp->GetName(), name) == 0)
			return temp;
   		vlist_advance_cursor(m_TargetObjectList);	
	}
	return 0;
}



HBhvTargetObject * HBhvBehaviorManager::FindTargetObjectByPath(const char *path)
{
  	vlist_reset_cursor(m_TargetObjectList);
 	while (1)
	{
		HBhvTargetObject *temp;
		if ((temp = (HBhvTargetObject *)vlist_peek_cursor(m_TargetObjectList)) == 0)
				break;
		if (temp->IsEqual(path))
			return temp;
   		vlist_advance_cursor(m_TargetObjectList);	
	}
	return 0;
}


void HBhvBehaviorManager::SetCurrentTick()
{
	float ctime;
	HC_Show_Time( &ctime);
	SetCurrentTickByTime(ctime);
}


void HBhvBehaviorManager::SetCurrentTick(float tick)
{
	/* if we are currently playing, adjust the start time to match 
	 * what we just set the tick to be */
	if (IsPlaying()) {
		m_StartTime += (m_CurrentTick - tick)/m_Tps;
	}
	m_CurrentTick =  tick; 
}



void HBhvBehaviorManager::ScheduleAnimation(HBhvAnimation *animation, float currentframe)
{
	HBhvTimeline *ti = animation->GetTimeline();
	if (animation->GetLoop())
	    ti->SetStartTick( currentframe + animation->GetDelay() - ti->GetCurrentRelativeTick());
	else
	    ti->SetStartTick( currentframe + animation->GetDelay());

	animation->Reset();
	animation->SetDefaultActive(true);
	animation->SetExecuteOnce(false);
	vlist_remove(m_ScheduledAnimationList, animation);
 	vlist_add_last(m_ScheduledAnimationList, (void *)animation);
 
}

void HBhvBehaviorManager::RemoveScheduledAnimation(HBhvAnimation *animation)
{
 
	vlist_remove(m_ScheduledAnimationList, animation);
	 
}

 

void HBhvBehaviorManager::ScheduleAllAnimations(bool reset)
{
	delete_vlist(m_ScheduledAnimationList);
	m_ScheduledAnimationList = new_vlist( malloc, free );
	vlist_reset_cursor(m_AnimationList);
	while (1)
	{
		HBhvAnimation *temp;
		if ((temp = (HBhvAnimation *)vlist_peek_cursor(m_AnimationList)) == 0)
				break;
		if (reset)
			temp->GetTimeline()->SetCurrentRelativeTick(0);
		if (temp->GetDefaultActive())
			ScheduleAnimation(temp, 0);
		vlist_advance_cursor(m_AnimationList);	
	}

	START_LIST_ITERATION(HBhvSensor, m_SensorList);{
		if (temp->GetDefaultActive())
			temp->Activate();
	}END_LIST_ITERATION(m_SensorList);
}





HBhvAnimation * HBhvBehaviorManager::AddAnimation(const char *name, const char *target , HBhvTimeline *tl, HBhvInterpolator *interpolator)
{
	HBhvAnimation *ani = new HBhvAnimation(name, this, 0);
	ani->SetTimeline(tl);
	ani->AddInterpolator(interpolator);
 	if (target)
	{
		ani->SetTargetByPath(target);

		HC_Open_Segment_By_Key(ani->GetTarget()->GetTargetKey());
		{
			if (HC_Show_Existence("modelling matrix"))
			{
				float current_matrix[16];
				HC_Show_Modelling_Matrix(current_matrix);
				ani->SetTargetScaleMatrix(current_matrix);
			}
		}
		HC_Close_Segment();
	}
	tl->SetAnimation(ani);
	interpolator->SetAnimation(ani);

//	HBhvAnimation *aninst = ani->CreateInstance();
 	vlist_add_last(m_AnimationList, ani);
//	vlist_add_last(m_AnimationList, aninst);
	return ani;
}


void HBhvBehaviorManager::AddAnimation(HBhvAnimation *animation)
{
	if (animation->GetTarget())
	{
		HC_Open_Segment_By_Key(animation->GetTarget()->GetTargetKey());
		{
			if (HC_Show_Existence("modelling matrix"))
			{
				float current_matrix[16];
				HC_Show_Modelling_Matrix(current_matrix);
				animation->SetTargetScaleMatrix(current_matrix);
			}
		}
		HC_Close_Segment();
	}

  	vlist_add_last(m_AnimationList, (void *)animation);
}

 

int HBhvBehaviorManager::GetFreeName()
{
		vlist_reset_cursor(m_AnimationList);
 		int number = 0;
		while (1)
		{
			HBhvAnimation *temp;
			if ((temp = (HBhvAnimation *)vlist_peek_cursor(m_AnimationList)) == 0)
				break;
			int nnum = atoi(temp->GetName());
			if (nnum>=number)
				number = nnum + 1;
 			vlist_advance_cursor(m_AnimationList);	
		}
		return number;
}



 
void HBhvBehaviorManager::ResolveInstances()
{
/*
	START_LIST_ITERATION(HBhvAnimation, m_AnimationList)
	{
		HBhvAnimation *animation = temp;
		START_LIST_ITERATION(HBhvAnimation, m_AnimationList)
		if (!temp->GetAnimation() && strcmp(temp->GetInstanceOf(), animation->GetName()) == 0)
			temp->SetAnimation(animation);
		END_LIST_ITERATION(m_AnimationList)
	}
	END_LIST_ITERATION(m_AnimationList)
*/

}
			
bool HBhvBehaviorManager::Tick( float request_time, float actual_time )
{
	UNREFERENCED(request_time);

	if (m_CurrentTick >= m_LastTick && !vlist_peek_first(m_SensorList) && !m_bInfinitePlay)
	{
		if (m_bContinuousPlay)
		{
			ScheduleAllAnimations();	
			HC_Show_Time( &m_StartTime );  
			m_CurrentTick = 0;
		}
		else
		{
			Stop();
			return true;
		}
	}
	else
	{
 		if (m_bRenderEveryFrame)
			m_CurrentTick++;
		else
			SetCurrentTickByTime(actual_time);
	}
	
 
 	ExecuteAnimations(GetCurrentTick(),-1);	
	
	GetModel()->Update();
//	GetModel()->Update(false, true);
  	return true;

} //end function Tick

void HBhvBehaviorManager::Reset()
{
    HC_Open_Segment_By_Key(GetActiveView()->GetSceneKey());

   HPoint p,t,u;
		    HC_Show_Net_Camera_Position(&p.x, &p.y, &p.z);
		    HC_Show_Net_Camera_Target(&t.x, &t.y, &t.z);
		    HC_Show_Net_Camera_Up_Vector(&u.x, &u.y, &u.z);

		    HUtility::GetMatrixFromCamera(t,p,u,p,m_positionmatrix);
		    SetTargetMatrix(m_positionmatrix);
		    HC_Close_Segment();

//    HC_Compute_Identity_Matrix(m_positionmatrix);
  //  HC_Compute_Identity_Matrix(m_targetmatrix);
    START_LIST_ITERATION(HBhvTargetObject, m_TargetObjectList);
		temp->ResetForCollision();
	END_LIST_ITERATION(m_TargetObjectList);

}


void HBhvBehaviorManager::ExecuteAnimations(float currentframe, float startframe)
{
	struct vlist_s *	deletionlist;		
	deletionlist = new_vlist(malloc, free);
	Reset();
	m_bCameraUpdated = false;
	START_LIST_ITERATION(HBhvAnimation, m_ScheduledAnimationList);{
		bool animfinished = false;		
		if (!temp->GetParentAnimation())
		{
			if (startframe>-1)
				temp->Animate(currentframe, startframe);
			else
				animfinished = temp->Animate(currentframe);

			if ((animfinished && !temp->GetLoop()) || temp->ExecuteOnce())
			{
				temp->SetExecuteOnce(false);
				vlist_add_last(deletionlist,temp);
				//					vlist_remove(m_ScheduledAnimationList, temp);
				temp->SetCurrentlyRunning(false);
				HLISTENER_EVENT(HAnimationListener, m_pModel->GetEventManager(), AnimationFinishedEvent(temp));
			}
		}
	}END_LIST_ITERATION(m_ScheduledAnimationList);
	START_LIST_ITERATION(HBhvAnimation, deletionlist);{
		vlist_remove(m_ScheduledAnimationList,temp);
	}END_LIST_ITERATION(deletionlist);
	delete_vlist(deletionlist);


	START_LIST_ITERATION(HBhvSensor, m_SensorList);{
		if (temp->GetDefaultActive())
			temp->Tick();
	}END_LIST_ITERATION(m_SensorList);

	START_LIST_ITERATION(HBhvTargetObject, m_TargetObjectList);{
		if (temp->GetCollision() && temp->HasMoved())
			EvaluateCollision(temp);
	}END_LIST_ITERATION(m_TargetObjectList);
}


HBhvAnimation * HBhvBehaviorManager::FindAnimation(const char *target, const char *interpolatortype)
{

	START_LIST_ITERATION(HBhvAnimation, m_AnimationList);{
		if (strcmp(temp->GetInterpolator()->GetType(), interpolatortype) == 0
			&& temp->GetTarget()->IsEqual(target))
			return temp;							
	}END_LIST_ITERATION(m_AnimationList);
	return 0;
}

HBhvAnimation * HBhvBehaviorManager::FindAnimationByName(const char *name)
{

	START_LIST_ITERATION(HBhvAnimation, m_AnimationList);{
		if (temp->GetName() && strcmp(temp->GetName(), name) == 0)
			return temp;							
	}END_LIST_ITERATION(m_AnimationList);
	return 0;
}

HBhvSensor * HBhvBehaviorManager::FindSensorByName(const char *name)
{

	START_LIST_ITERATION(HBhvSensor, m_SensorList);{
		if (temp->GetName() && strcmp(temp->GetName(), name) == 0)
			return temp;							
	}END_LIST_ITERATION(m_SensorList);
	return 0;
}


 

void HBhvBehaviorManager::DeleteAnimation(HBhvAnimation *animation)
{
	
	vlist_remove(m_AnimationList, animation);
	vlist_remove(m_ScheduledAnimationList, animation);
	delete animation;
	
}


void HBhvBehaviorManager::WriteToFile(const char *filename){
  WriteToFile((wchar_t*)H_WCS(filename).encodedText());
}

#ifdef _MSC_VER
void HBhvBehaviorManager::WriteToFile(const unsigned short *filename)
{
	WriteToFile((wchar_t const *) filename);
}
#endif

void HBhvBehaviorManager::WriteToFile(const wchar_t *filename)
{
	HUtilityXMLGenerator xmlgen;
	xmlgen.Reset();
	Serialize(&xmlgen);
	xmlgen.Finalize();
	FILE *fp;
 	fp = wfopen(filename, L"w");
	fwrite(xmlgen.GetBuffer(), 1, xmlgen.GetBufferSize(), fp);
	fclose(fp);
}

 
void *HBhvBehaviorManager::XMLCallback(HUtilityXMLTag *xt, bool open, void *m_pExtraData)
{
	char Name[BHV_MAX_NAME_LENGTH];
	char Version[BHV_MAX_VERSION_LENGTH];
	char Vendor[BHV_MAX_VENDOR_LENGTH];
 	int fps;
	int delay;
	HBaseModel *model = (HBaseModel *)m_pExtraData;
	if (open)
	{
		if (!xt->GetProperty("Name", Name))
			strcpy(Name, "");
		if (!xt->GetProperty("Version", Version))
			strcpy(Version, "");
		if (!xt->GetProperty("Vendor", Vendor))
			strcpy(Vendor, "");
		if (!xt->GetProperty("TicksPerSecond", fps))
			fps = 10;
		if (!xt->GetProperty("Delay", delay))
			delay = 10;

		HBhvBehaviorManager *oldbhvmanager = model->GetBhvBehaviorManager();
		HBaseView *oldview = 0;
		if (oldbhvmanager)	
			oldview = oldbhvmanager->GetActiveView();			
		if (!oldbhvmanager->GetMerge())
			model->SetBhvBehaviorManager(new HBhvBehaviorManager(model, fps, delay, Name, Version, Vendor));	
		model->GetBhvBehaviorManager()->SetActiveView(oldview);

	}
	else
	{
		HBhvBehaviorManager * ad = model->GetBhvBehaviorManager(); 	 
		ad->ResolveInstances();
	}
	return 0;
 	 
}


void HBhvBehaviorManager::ReadFromFile(HBaseModel *model, const char *filename){
  ReadFromFile(model, (wchar_t*)H_WCS(filename).encodedText());
}

#ifdef _MSC_VER
void HBhvBehaviorManager::ReadFromFile(HBaseModel *model, const unsigned short *filename)
{
	ReadFromFile(model, (wchar_t const *) filename);
}
#endif

void HBhvBehaviorManager::ReadFromFile(HBaseModel *model, const wchar_t *filename)
{
	static bool first = true;
	static wchar_t oldfilename[MVO_BUFFER_SIZE];
	wchar_t loadfilename[MVO_BUFFER_SIZE];
	if (!filename)
	{
		if (first)
			return;
		wcscpy(loadfilename, oldfilename);
	}		
	else
		wcscpy(loadfilename, filename);

	int size = 10000;
	first = false;

	char *buffer = new char[size];
 	int c = 0;
    FILE * fp = wfopen(loadfilename, L"r");
	while (1)
	{
		int rr = (int)fread( &buffer[c], 1, 1, fp);
		if (!rr)
			break;
 		c++;
		if (c >=size)
		{
			size+=1000;
			char *temp = new char[size];
			memcpy(temp, buffer, c);
			delete [] buffer;
			buffer = temp;
		}
 
	}
	fclose(fp);
	buffer[c + 1] = 0;
	
	ProcessXMLData(model, buffer);
	delete [] buffer;
	wcscpy(oldfilename, loadfilename);

}

void HBhvBehaviorManager::ProcessXMLData(HBaseModel *model, const char *buffer)
{
 	HUtilityXMLParser xp;
	HDB::GetHDB();


	char doctype[MVO_BUFFER_SIZE];
	xp.FindDocType(doctype, buffer);
	xp.SetupParseBuffer(buffer);
 	xp.SetTagCallback("BehaviorManager", HBhvBehaviorManager::XMLCallback, model);
 	xp.SetTagCallback("Animdef", HBhvBehaviorManager::XMLCallback, model);
	xp.SetTagCallback("Animation", HBhvAnimation::XMLCallback, model);
	xp.SetTagCallback("PosInterpolator", HBhvInterpolatorPosition::XMLCallback, model);
	xp.SetTagCallback("QuatRotInterpolator", HBhvInterpolatorQuatSquad::XMLCallback, model);
	xp.SetTagCallback("MatrixInterpolator", HBhvInterpolatorMatrix::XMLCallback, model);
	xp.SetTagCallback("ColorInterpolator", HBhvInterpolatorColor::XMLCallback, model);
	xp.SetTagCallback("AxisRotInterpolator", HBhvInterpolatorAxisRotate::XMLCallback, model);
	xp.SetTagCallback("ScaleInterpolator", HBhvInterpolatorScale::XMLCallback, model);
	xp.SetTagCallback("AttSwitchInterpolator", HBhvInterpolatorAttSwitch::XMLCallback, model);
	xp.SetTagCallback("VertexMorphInterpolator", HBhvInterpolatorVertexMorph::XMLCallback, model);
	xp.SetTagCallback("ColorMorphInterpolator", HBhvInterpolatorColorMorph::XMLCallback, model);
	xp.SetTagCallback("SegSwitchInterpolator", HBhvInterpolatorSegSwitch::XMLCallback, model);
	xp.SetTagCallback("SegMoveInterpolator", HBhvInterpolatorSegMove::XMLCallback, model);
	xp.SetTagCallback("InstanceCreateInterpolator", HBhvInterpolatorInstanceCreate::XMLCallback, model);
	xp.SetTagCallback("TrailInterpolator", HBhvInterpolatorTrail::XMLCallback, model);
	xp.SetTagCallback("Timeline", HBhvTimeline::XMLCallback, model);
	xp.SetTagCallback("TargetObject", HBhvTargetObject::XMLCallback, model);
	xp.SetTagCallback("Sensor", HBhvSensor::XMLCallback, model);
	xp.SetTagCallback("Condition", HBhvCondition::XMLCallback, model);
	xp.SetTagCallback("Action", HBhvAction::XMLCallback, model);

	xp.ProcessXMLData();

}
void HBhvBehaviorManager::Serialize(HUtilityXMLGenerator *xmlgen)
{
	
 	HUtilityXMLTag xmlt;
	xmlgen->WriteToBuffer("<?xml version=\"1.0\" encoding = \"UTF-8\"?>\n");
	xmlt.SetTagname("Animdef");

	xmlt.AddProperty("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
	xmlt.AddProperty("xsi:noNamespaceSchemaLocation", "http://www.openhsf.org/specification/extensions/published/hsfanimation.xsd");
	xmlt.AddProperty("Name", m_Name, true);
	xmlt.AddProperty("Version", m_Version, true);
	xmlt.AddProperty("Vendor", m_Vendor, true);
	xmlt.AddPropertyInt("TicksPerSecond", (int)m_Tps);
	xmlt.AddPropertyInt("Delay", (int)m_Delay);
	xmlt.SetOpen(true);
   	xmlgen->AddTag(&xmlt);

	vlist_reset_cursor(m_TargetObjectList);
	while (1)
	{
		HBhvTargetObject *temp;
		if ((temp = (HBhvTargetObject *)vlist_peek_cursor(m_TargetObjectList)) == 0)
			break;
		temp->Serialize(xmlgen);
		vlist_advance_cursor(m_TargetObjectList);	
	}


	vlist_reset_cursor(m_AnimationList);
 	while (1)
	{
		HBhvAnimation *temp;
		if ((temp = (HBhvAnimation *)vlist_peek_cursor(m_AnimationList)) == 0)
			break;
		temp->Serialize(xmlgen);
 		vlist_advance_cursor(m_AnimationList);	
	}

	
	START_LIST_ITERATION(HBhvSensor, m_SensorList);{
		temp->Serialize(xmlgen);
	}END_LIST_ITERATION(m_SensorList);


  	xmlgen->CloseTag(&xmlt);
}


int HBhvBehaviorManager::GetNextKeyframe(int ticknum, bool forward)
{
	bool found = false;
	int ftick = 1000000;
	if (!forward)
		ftick = -ftick;
	START_LIST_ITERATION(HBhvAnimation, m_ScheduledAnimationList);{
		if (!temp->GetParentAnimation())
		{
			HBhvTimeline *ti = temp->GetTimeline();
			int *ta = ti->GetTimelineArray();
			int talength = ti->GetTimelineArrayLength();
			for (int i=0;i<talength; i++)
			{
				if (forward)
				{
					if (ta[i] > ticknum)
					{
						found = true;
						if ((ta[i] - ticknum) < (ftick - ticknum))
							ftick = ta[i];
					}
				}
				else
				{
					if (ta[i] < ticknum)
					{
						found = true;
						if ((ticknum - ta[i]) < (ticknum - ftick))
							ftick = ta[i];
					}
				}
			}

		}
	}END_LIST_ITERATION(m_ScheduledAnimationList);
	if (found)
		return ftick;
	else
		return ticknum;
}
 

 
void HBhvBehaviorManager::SetCurrentTickByPercentage(float percentage)
{
 
	int dist = GetLastTick();
	if (dist == 0)
		dist = 1;
	m_CurrentTick = (percentage / 100.0f) * dist;		

}
 
 
float HBhvBehaviorManager::GetCurrentTickByPercentage()
{
 		int dist = GetLastTick();
		if( dist == 0 )
			return 0;
		else
			return (m_CurrentTick / dist) * 100.0f;

}

int HBhvBehaviorManager::GetLastTick()
{
	int dist = 0;
	START_LIST_ITERATION(HBhvAnimation, m_ScheduledAnimationList);{
		if (!temp->GetParentAnimation())
		{
			HBhvTimeline *ti = temp->GetTimeline();
			int lasttick = ti->GetLastTick();
			if (lasttick != -1)
			{
				if (lasttick > dist)
					dist = lasttick;
			}
		}
	}END_LIST_ITERATION(m_ScheduledAnimationList);
	return dist;
}


 
void HBhvBehaviorManager::AddSensor(HBhvSensor *sensor)
{
  	vlist_add_last(m_SensorList, (void *)sensor);
}


HC_KEY HBhvBehaviorManager::GetActiveSceneKey()
{
	 if (m_pView) 
		 return m_pView->GetSceneKey();
	 else
		 return INVALID_KEY; 
}


bool HBhvBehaviorManager::HasAnimations()
{ 
	return vlist_peek_first(m_AnimationList) != 0;
}



void HBhvBehaviorManager::DeactivateAllAnimations()
{
	START_LIST_ITERATION(HBhvAnimation, m_AnimationList);{
	    temp->SetDefaultActive(false);
	}END_LIST_ITERATION(m_AnimationList);
}

void HBhvBehaviorManager::ActivateAllAnimations()
{
	START_LIST_ITERATION(HBhvAnimation, m_AnimationList);{
	    temp->SetDefaultActive(true);
	}END_LIST_ITERATION(m_AnimationList);
}

void HBhvBehaviorManager::ActivateAnimationByName(const char *name, bool AllowPartial)
{
	START_LIST_ITERATION(HBhvAnimation, m_AnimationList);{
		if (AllowPartial)
		{	    
			if (strstr(temp->GetName(),name) == temp->GetName())
				temp->SetDefaultActive(true);
		}
		else
		{	    
			if (strcmp(temp->GetName(),name) == 0)
				temp->SetDefaultActive(true);
		}
	}END_LIST_ITERATION(m_AnimationList);
}

void HBhvBehaviorManager::DeactivateAnimationByName(const char *name, bool AllowPartial)
{
	START_LIST_ITERATION(HBhvAnimation, m_AnimationList);{
		if (AllowPartial)
		{	    
			if (strstr(temp->GetName(),name) == temp->GetName())
				temp->SetDefaultActive(false);
		}
		else
		{	    
			if (strcmp(temp->GetName(),name) == 0)
				temp->SetDefaultActive(false);
		}
	}END_LIST_ITERATION(m_AnimationList);
}

bool HBhvBehaviorManager::EvaluateCollision(HBhvTargetObject* pTargetObject)
{
 
	
	HC_KEY hTarget = pTargetObject->GetTargetKey();
	if(!HUtility::GrabSegment(hTarget))
		return false;


	if (!m_bShellSelectionActive)
	{
		HC_Begin_Shell_Selection();
		m_bShellSelectionActive = true;
	}
 	
	// The target may consist of 0...n shells, any of which may clash with the environment
	// This means we have to move the entire target segement including all its shells to a 
	// temporary segment so it is not included in the HC_Compute_Selection_By_Shell function.

	// If we are moving the target segment then we should remember the parent segement
	// so we can move it back later
	HC_KEY hTargetOwner = HC_KShow_Owner_By_Key(hTarget);

	HC_Create_Segment("/tempsegment");
	HC_Move_By_Key(hTarget, "/tempsegment");

	// Search for every shell under the target object's segment
	// and perform a selection by shell for each
	// If any collision is found then stop iterating
	bool bCollisionFound(false);
	//int nShellCount = 0;

	HC_Open_Segment_By_Key(hTarget);
 		HC_KEY hShellKey(INVALID_KEY);
		float	modelling_matrix[16];
		HC_Begin_Contents_Search("...", "shells");
			while(HUtility::FindContents(hShellKey) && !bCollisionFound)
			{
				// The HC_Compute_Selection_By_Shell works in object space
				// and our shell may have been subjected to a transformation maxtrix.
				// We need to transform this shell's points into object space so we will ask
				// the owning segment of this shell for its net effect modelling matrix.
				HC_KEY hShellOwnerSegment = HC_KShow_Owner_By_Key(hShellKey);
				HC_Open_Segment_By_Key(hShellOwnerSegment);
					HC_Show_Net_Modelling_Matrix(modelling_matrix);
				HC_Close_Segment();

				// Get the shell data so we can pass it to HC_Compute_Selection_By_Shell
				int nPointCount(0), nFaceListLength(0);
				float* pOriginalPoints = 0;
				int* pFaceList = 0;
				HC_Show_Shell_Size( hShellKey, &nPointCount, &nFaceListLength );
  				HC_Show_Geometry_Pointer( hShellKey, "points", &pOriginalPoints);
				HC_Show_Geometry_Pointer( hShellKey, "face list", &pFaceList );

				// Create a data block large enough to hold the transformed
				// shell data points
				int* pTransformedPoints = new int[nPointCount * 3];

				HC_Compute_Transformed_Points(	nPointCount,
												pOriginalPoints, 
												modelling_matrix, 
												pTransformedPoints);

				HC_Open_Segment_By_Key(GetActiveView()->GetOverwriteKey());

					if(HC_Compute_Selection_By_Shell( "v, selection level = entity, no related selection limit",
													".", nPointCount, pTransformedPoints, nFaceListLength, pFaceList))
					{
						bCollisionFound = true;
					}

				HC_Close_Segment();

				delete [] pTransformedPoints;
			}
		HC_End_Contents_Search();
	HC_Close_Segment(); // hTarget

	// Move the target object's segment structure back to where it started
	HC_Open_Segment_By_Key(hTargetOwner);
		HC_Move_By_Key(hTarget,".");
	HC_Close_Segment();

	// Send a collision/no collision event to any object registered with the events manager
	if(bCollisionFound)
	{
		HLISTENER_EVENT(HAnimationListener, m_pModel->GetEventManager(), ObjectCollisionEvent(pTargetObject));
	}
	else
	{
		HLISTENER_EVENT(HAnimationListener, m_pModel->GetEventManager(), ObjectNoCollisionEvent(pTargetObject));
	}

	return false;
}

HBhvAnimation * HBhvBehaviorManager::AddAnimation(const char *name, const char *targetpath, HPoint *pivot)
{
 	HBhvAnimation *ani = new HBhvAnimation(name, this, 0);
 	if (targetpath)
	{
		ani->SetTargetByPath(targetpath);
		if (pivot)
			ani->GetTarget()->SetPivot(pivot->x, pivot->y, pivot->z);

		HC_Open_Segment_By_Key(ani->GetTarget()->GetTargetKey());
		{
			if (HC_Show_Existence("modelling matrix"))
			{
				float current_matrix[16];
				HC_Show_Modelling_Matrix(current_matrix);
				ani->SetTargetScaleMatrix(current_matrix);
			}
		}
		HC_Close_Segment();
	}
	AddAnimation(ani);
	return ani;
}



void HBhvBehaviorManager::AddPositionKeyframe(const char *animname, int tick, HPoint pos, bool linear)
{
	HKeyframe * c;
	if (linear)
		c = new HKeyframeChannelLinear; 
	else
		c =  new HKeyframeChannelCurve; 
	((HKeyframeChannel *)c)->m_cp = pos;
	AddKeyframe(animname, "Pos", c, tick);


}
void HBhvBehaviorManager::AddTrailKeyframe(const char *animname, int tick, HPoint pos, bool linear, TrailInterpolatorType *trailtype, int *weight, const char *style, const char *color)
{
	HKeyframe * c;
	if (linear)
		c = new HKeyframeChannelLinear; 
	else
		c =  new HKeyframeChannelCurve; 
	((HKeyframeChannel *)c)->m_cp = pos;
	HBhvInterpolatorTrail *interpol = (HBhvInterpolatorTrail *)AddKeyframe(animname, "Trail", c, tick);
	if (weight)
		interpol->SetTrailWeight(*weight);
	if (trailtype)
		interpol->SetTrailType(*trailtype);
	if (color)
		interpol->SetTrailColor(color);
	if (style)
		interpol->SetTrailStyle(style);
}


void HBhvBehaviorManager::AddQuatSquadKeyframe(const char *animname, int tick, HQuat rot, bool linear)
{
	HKeyframe * c = new HKeyframeQuatSquad; 
	
	((HKeyframeQuatSquad *)c)->m_quat = rot;
	((HKeyframeQuatSquad *)c)->m_bLinear = linear;
	
	AddKeyframe(animname, "QuatRot", c, tick);

}
void HBhvBehaviorManager::AddAxisRotateKeyframe(const char *animname, int tick, float rot, HPoint *axis)
{
	HKeyframe * c = new HKeyframeAxisRotation; 	
	((HKeyframeAxisRotation *)c)->m_angle = rot; 	
	HBhvInterpolatorAxisRotate *interpol = (HBhvInterpolatorAxisRotate *)AddKeyframe(animname, "AxisRot", c, tick);
	if (axis)
	interpol->SetAxis((*axis).x, (*axis).y, (*axis).z);

}


void HBhvBehaviorManager::AddColorKeyframe(const char *animname, int tick, HPoint color, const char *colorcomponent, const char *geomtype)
{
	HKeyframe * c = new HKeyframeChannelLinear; 
 	((HKeyframeChannel *)c)->m_cp = color;

	HBhvInterpolatorColor *interpol = (HBhvInterpolatorColor *)AddKeyframe(animname, "Color", c, tick);
	if (colorcomponent)
		interpol->SetColorComponent(colorcomponent);
	if (geomtype)
		interpol->SetGeomType(geomtype);
}

void HBhvBehaviorManager::AddScaleKeyframe(const char *animname, int tick, HPoint scale)
{
	HKeyframe * c = new HKeyframeChannelLinear; 
 	((HKeyframeChannel *)c)->m_cp = scale;

	AddKeyframe(animname, "Scale", c, tick);

}


void HBhvBehaviorManager::AddSegmentMoveKeyframe(const char *animname, int tick, char *path)
{
	HKeyframe * c = new HKeyframeString; 
	
	
 	

	HBhvCameraType dummy;
	char result[MVO_BUFFER_SIZE];
 	HBhvAnimation *ani = FindAnimationByName(animname);
	char dummytype[MVO_SMALL_BUFFER_SIZE];
	HBhvTargetObject::ResolveTarget(path, result, dummytype, ani->GetBehaviorManager()->GetModel(), dummy);							 
	((HKeyframeString *)c)->SetTarget(result);
	AddKeyframe(animname, "SegMove", c, tick);
}

void HBhvBehaviorManager::AddMatrixKeyframe(const char *animname, int tick, float *matrix)
{
	HKeyframe * c = new HKeyframeMatrix; 
	
	for (int i=0;i<16;i++)
		((HKeyframeMatrix *)c)->m_matrix[i] = matrix[i];
 	
	AddKeyframe(animname, "Matrix", c, tick);

}

void HBhvBehaviorManager::AddAttributeSwitchKeyframe(const char *animname, int tick, const char *att)
{
	HKeyframe * c = new HKeyframeString; 
	
	
	HBhvCameraType dummy;
	char result[MVO_BUFFER_SIZE];
 	HBhvAnimation *ani = FindAnimationByName(animname);
	char dummytype[MVO_SMALL_BUFFER_SIZE];
	HBhvTargetObject::ResolveTarget(att, result, dummytype, ani->GetBehaviorManager()->GetModel(), dummy);							 
 	((HKeyframeString *)c)->SetTarget(result);
 	
	AddKeyframe(animname, "AttSwitch", c, tick);
}

void HBhvBehaviorManager::AddSegmentSwitchKeyframe(const char *animname, int tick, const char *ss)
{
	HKeyframe * c = new HKeyframeString; 
	
	HBhvCameraType dummy;
	char result[MVO_BUFFER_SIZE];
 	HBhvAnimation *ani = FindAnimationByName(animname);
	char dummytype[MVO_SMALL_BUFFER_SIZE];
	HBhvTargetObject::ResolveTarget(ss, result, dummytype, ani->GetBehaviorManager()->GetModel(), dummy);							 
	((HKeyframeString *)c)->SetTarget(result);
  	
	AddKeyframe(animname, "SegSwitch", c, tick);
}

void HBhvBehaviorManager::AddInstanceCreateKeyframe(const char *animname, int tick, const char *instance)
{
	HKeyframe * c = new HKeyframeString; 
	
	HBhvCameraType dummy;
	char result[MVO_BUFFER_SIZE];
 	HBhvAnimation *ani = FindAnimationByName(animname);
	char dummytype[MVO_SMALL_BUFFER_SIZE];
	HBhvTargetObject::ResolveTarget(instance, result, dummytype, ani->GetBehaviorManager()->GetModel(), dummy);							 	
 	((HKeyframeString *)c)->SetTarget(result);
 	
	AddKeyframe(animname, "InstanceCreate", c, tick);
}


void HBhvBehaviorManager::AddVertexMorphInterpolator(const char *animname, int tick, const char *mident, bool discrete)
{
	HKeyframe * c = new HKeyframeString; 
		
 	((HKeyframeString *)c)->SetTarget(mident);
 	((HKeyframeString *)c)->m_bDiscrete = discrete;
 	
	AddKeyframe(animname, "VertexMorph", c, tick);
}

void HBhvBehaviorManager::AddColorMorphInterpolator(const char *animname, int tick, const char *mident, bool discrete)
{
	HKeyframe * c = new HKeyframeString; 
		
 	((HKeyframeString *)c)->SetTarget(mident);
 	((HKeyframeString *)c)->m_bDiscrete = discrete;
 	
	AddKeyframe(animname, "ColorMorph", c, tick);
}



void HBhvBehaviorManager::AddVertexMorphDataToAnimation(const char *animname, HPoint *md, int pnum, HBaseModel *model, int pos)
{
 	HBhvAnimation *ani = FindAnimationByName(animname);
	HBhvInterpolatorVertexMorph * interpol = 0;

	START_LIST_ITERATION(HBhvInterpolator, ani->GetInterpolatorList());{
		if (!temp)
			break;
			if (strcmp(temp->GetType(),"VertexMorph") == 0)	
				interpol = (HBhvInterpolatorVertexMorph *)temp;
	}END_LIST_ITERATION(ani->GetInterpolatorList());
	
	if (interpol)
		interpol->AddMorphData(md, pnum, model, pos);
}
void HBhvBehaviorManager::AddColorMorphDataToAnimation(const char *animname, HPoint *md, int pnum, HBaseModel *model, int pos)
{
 	HBhvAnimation *ani = FindAnimationByName(animname);
	HBhvInterpolatorColorMorph * interpol = 0;

	START_LIST_ITERATION(HBhvInterpolator, ani->GetInterpolatorList());{
		if (!temp)
			break;
			if (strcmp(temp->GetType(),"ColorMorph") == 0)	
				interpol = (HBhvInterpolatorColorMorph *)temp;
	}END_LIST_ITERATION(ani->GetInterpolatorList());
	
	if (interpol)
		interpol->AddMorphData(md, pnum, model, pos);
}
	
 

HBhvInterpolator * HBhvBehaviorManager::AddKeyframe(const char *animname, const char *anitype, HKeyframe *keyframe, int tick)
{
 	HBhvAnimation *ani = FindAnimationByName(animname);
	HBhvTimeline *t = ani->GetTimeline();
	HBhvInterpolator * interpol = 0;

	START_LIST_ITERATION(HBhvInterpolator, ani->GetInterpolatorList());{
		if (!temp)
			break;
			if (strcmp(temp->GetType(),anitype) == 0)	
				interpol = temp;
	}END_LIST_ITERATION(ani->GetInterpolatorList());
	

	if (!interpol)
	{
	
		if (!t)
		{
			HBhvTimeline *t = new HBhvTimeline();
 			ani->SetTimeline(t);
		}
		if (strcmp(anitype, "Pos")== 0)
			interpol = new HBhvInterpolatorPosition();
		else if (strcmp(anitype, "QuatRot")== 0)
			interpol = new HBhvInterpolatorQuatSquad();
		else if (strcmp(anitype, "SegMove")== 0)
			interpol = new HBhvInterpolatorSegMove();
		else if (strcmp(anitype, "Matrix")== 0)
			interpol = new HBhvInterpolatorMatrix();
		else if (strcmp(anitype, "Trail")== 0)
			interpol = new HBhvInterpolatorTrail(ani);
		else if (strcmp(anitype, "AxisRot")== 0)
			interpol = new HBhvInterpolatorAxisRotate();
		else if (strcmp(anitype, "Color")== 0)
			interpol = new HBhvInterpolatorColor();
		else if (strcmp(anitype, "Scale")== 0)
			interpol = new HBhvInterpolatorScale();
		else if (strcmp(anitype, "AttSwitch")== 0)
			interpol = new HBhvInterpolatorAttSwitch();
		else if (strcmp(anitype, "VertexMorph")== 0)
			interpol = new HBhvInterpolatorVertexMorph();
		else if (strcmp(anitype, "ColorMorph")== 0)
			interpol = new HBhvInterpolatorColorMorph();
		else if (strcmp(anitype, "SegSwitch")== 0)
			interpol = new HBhvInterpolatorSegSwitch();
		else if (strcmp(anitype, "InstanceCreate")== 0)
			interpol = new HBhvInterpolatorInstanceCreate();

		ani->AddInterpolator(interpol);
	}

	bool replace;
	int l = ani->GetTimeline()->AddKeyframe(tick, replace);
	
	if (replace)
		interpol->Replace(keyframe, l);
 	else
 		interpol->Insert(keyframe, l);
 
	return interpol;
}
