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

#include "HBaseModel.h"
#include "HBaseView.h"
#include "HBaseOperator.h"
#include "HEventInfo.h"
#include "HBhvAnimation.h"
#include "HBhvBehaviorManager.h"
#include "HBhvTimeline.h"
#include "HBhvInterpolator.h"
#include "HSelectionSet.h"
#include "HDB.h"
#include "HUtilityXMLParser.h"
#include "HUtilityGeomHandle.h"
#include "vlist.h"
#include "HObjects.h"



HBhvAnimation::HBhvAnimation(const char *name, HBhvBehaviorManager *BehaviorManager, HBhvAnimation *outerani)
{
	if (name)
		strcpy(m_Name, name);
	else
		strcpy(m_Name, "");

 	strcpy(m_InstanceOf, "");

 	m_Loop = 0;

	m_pParentAnimation = outerani;
	m_pTimeline = 0;
	m_pInstancedAnimation = 0;

	SetBehaviorManager(BehaviorManager);

	m_Delay = 0;
	m_InterpolatorList = new_vlist( malloc, free );
	m_ChildAnimationList = new_vlist( malloc, free );
	m_bCurrentlyRunning = false;
	m_bDefaultActive = true;
 	m_Target = 0;
	m_bExecuteOnce = false;
	HC_Compute_Identity_Matrix(m_TargetScaleMatrix);
}

HBhvAnimation::~HBhvAnimation()
{

	delete m_pTimeline;

	START_LIST_ITERATION(HBhvInterpolator, m_InterpolatorList);{
		delete temp;
	}END_LIST_ITERATION(m_InterpolatorList);
	delete_vlist(m_InterpolatorList);

	START_LIST_ITERATION(HBhvAnimation, m_ChildAnimationList);{
		delete temp;
	}END_LIST_ITERATION(m_ChildAnimationList);
	delete_vlist(m_ChildAnimationList);	
}

void HBhvAnimation::CleanWithoutDelete()
{
    m_pTimeline = 0;
    vlist_flush(m_InterpolatorList);
}
 
void HBhvAnimation::AddInterpolator(HBhvInterpolator *interpolator)
{
 	vlist_add_last(m_InterpolatorList, (void *)interpolator);
	if (interpolator)
		interpolator->SetAnimation(this);
}

void HBhvAnimation::AddChildAnimation(HBhvAnimation *animation)
{
 	vlist_add_last(m_ChildAnimationList, (void *)animation);
}


HBhvAnimation * HBhvAnimation::CreateInstance() 
{ 
	
	HBhvAnimation *ainst = new HBhvAnimation(0, GetBehaviorManager(), 0);
	ainst->SetInstancedAnimation(this);
	return ainst;
}



HBhvInterpolator * HBhvAnimation::GetInterpolator()
{
	return static_cast<HBhvInterpolator *>(vlist_peek_first(m_InterpolatorList));
}


void HBhvAnimation::PlotCurve(bool rotations)
{
	HBhvInterpolatorQuatSquad *quadsquadint = 0;
	HBhvInterpolatorPosition *posint = 0;
	START_LIST_ITERATION(HBhvInterpolator, m_InterpolatorList);{
		if (!temp)
			break;
	    if (strcmp(temp->GetType(),"QuatRot") == 0)
			quadsquadint = (HBhvInterpolatorQuatSquad *)temp;
		else
			if (strcmp(temp->GetType(),"Pos") == 0)	
				posint = ( HBhvInterpolatorPosition *)temp;
	}END_LIST_ITERATION(m_InterpolatorList);
	



	vlist_s *alist = GetBehaviorManager()->GetAnimationList();
 	vlist_reset_cursor(alist);
	HBhvAnimation *rotanimation = 0;
	if (rotations)
	{	
		if (quadsquadint)
		{
			rotanimation = this;
		}
		else
		{		
			while (1)
			{
				HBhvAnimation *temp;

				if ((temp = (HBhvAnimation *)vlist_peek_cursor(alist)) == 0)
					break;
				HBhvInterpolator *tempint = temp->GetInterpolator();
				if (strcmp(tempint->GetType(),"QuatRot") == 0)
				{	    
					rotanimation = temp;
					quadsquadint = (HBhvInterpolatorQuatSquad *)temp->GetInterpolator();
					break;
				}
				vlist_advance_cursor(alist);
			}
		}
	}
	int *ta = m_pTimeline->GetTimelineArray();
	int length = m_pTimeline->GetTimelineArrayLength();
	int oldtick = (int)m_pTimeline->GetStartTick();
	m_pTimeline->SetStartTick(0);
	VArray<HPoint> pos(ta[length-1]);
	HC_Set_Marker_Symbol("+");
	HC_Set_Marker_Size(0.3f);
	HC_Set_Color("markers = blue, lines = blue");
	HC_Set_Visibility("edges = on, lines = on, markers = on");
	HObjectManager::SetHObjectInfo("ignore");

	HC_Open_Segment("temp");
	    HC_Flush_Contents(".", "everything");
	HC_Close_Segment();
	for (int i = 0; i <= ta[length-1]; i++)
	{
		int interval = 0;
		int interval2;
		float fraction = 0;
 		m_pTimeline->Evaluate((float)i, interval, fraction);
		if (fraction == 1.0f)
			interval2 = interval + 1;
		else
			interval2 = interval;

		HBhvTimeline *tl = m_pTimeline;
		int *tlarray = tl->GetTimelineArray();
		//int length = tl->GetTimelineArrayLength();

		HBhvTargetObject *target = GetTarget();
		HPoint pivot;
		target->GetPivot(pivot);
	
	 
		((HBhvInterpolatorPosition *)posint)->CalculatePos(interval, fraction, pos[i]);		
		pos[i].Add(-pivot.x, -pivot.y, - pivot.z);
		HC_Open_Segment("temp");
 		HC_Insert_Marker(pos[i].x, pos[i].y, pos[i].z);
		HC_Close_Segment();
	
	

		

		if (i==tlarray[interval2])
		{
		    HDraggerObject dragger;
		    char text[MVO_BUFFER_SIZE];
		    sprintf(text, "%d", i);
			if (!HUtility::SegmentExists(text))
			{

			    float rotmatrix[16];
			    bool addrotation = false;
			    if (rotanimation && rotations)
			    {
					int interval_tl = 0;
					int interval_tl2;
					float fraction_tl = 0;
					HBhvTimeline *tl = rotanimation->GetTimeline();	 
					int *tlarray = tl->GetTimelineArray();
		 			tl->Evaluate((float)i, interval_tl, fraction_tl);
					if (fraction == 1.0f)
						interval_tl2 = interval_tl+1;
					else
						interval_tl2 = interval_tl;

					if (i == tlarray[interval_tl2])
					{				
						HBhvInterpolatorQuatSquad *qsint = (HBhvInterpolatorQuatSquad *)quadsquadint;
						HQuat resquat;
						qsint->CalculateQuat(interval, fraction, resquat);
						resquat.ToMatrix(rotmatrix); 				
 						addrotation = true;
					}
			    }			    
			    
			    if (!addrotation)
				HC_Compute_Identity_Matrix(rotmatrix);
			    
			    HC_Open_Segment(text);
    			    if (!addrotation)
						dragger.Insert(GetBehaviorManager()->GetActiveView(), pos[i], rotmatrix,this, 0, interval2, true);
    			    else
						dragger.Insert(GetBehaviorManager()->GetActiveView(), pos[i], rotmatrix,this, rotanimation, interval2, true);
			    HC_Close_Segment();
			}

		}
		else
		{
		    if (rotanimation)
		    {
				HBhvTimeline *tl = rotanimation->GetTimeline();	 
				int *tlarray = tl->GetTimelineArray();
	 			tl->Evaluate((float)i, interval, fraction);
				if (i==tlarray[interval])
				{
					HDraggerObject dragger;
					char text[MVO_BUFFER_SIZE];
					sprintf(text, "%d", i);
					float rotmatrix[16];
	 

					HBhvInterpolatorQuatSquad *qsint = (HBhvInterpolatorQuatSquad *)quadsquadint;
					HQuat resquat;
					qsint->CalculateQuat(interval, fraction, resquat);
					resquat.ToMatrix(rotmatrix); 				
    					HC_KEY tkey = HC_KOpen_Segment(text);
					bool containshandles = HUtilityGeomHandle::ContainsGeomHandles(tkey);
					HC_Flush_Contents(".", "everything");

					tkey = dragger.Insert(GetBehaviorManager()->GetActiveView(), pos[i], rotmatrix,this, rotanimation, interval, false);
					if (containshandles)
						HLISTENER_EVENT(HObjectManipulationListener, GetBehaviorManager()->GetActiveView()->GetEventManager(), SetupHandlesEvent(GetBehaviorManager()->GetActiveView(), tkey, false));
					
					HC_Close_Segment();						

				}
		    }
		}





//	HC_Open_Segment("tangents");
//		HC_Set_Color("markers = red, lines = grey");
//		HC_Set_Color("lines  = grey");
// 		HBhvInterpolatorPosition *it = (HBhvInterpolatorPosition *)GetInterpolator();
//		HKeyframeChannelCurve **splinearray = (HKeyframeChannelCurve **)it->GetArray();
//		if (interval<length-1)
//		splinearray[interval]->DrawTangents(splinearray[interval+1]->m_cp);
//		HC_Close_Segment();


	}
	HC_Open_Segment("temp");
	HC_Insert_Polyline(ta[length-1], pos);
	HC_Close_Segment();
	m_pTimeline->SetStartTick((float)oldtick);
}





void HBhvAnimation::DeleteKeyframe(int keyframe)
{
	HBhvTimeline *tline = GetTimeline();
	int pos = tline->DeleteKeyframe(keyframe);
	if (pos!=-1)
	{
		vlist_reset_cursor(m_InterpolatorList);
  		while (1)
		{
			HBhvInterpolator *temp;
			if ((temp = (HBhvInterpolator *)vlist_peek_cursor(m_InterpolatorList)) == 0)
				break;
			temp->Remove(pos);
  			vlist_advance_cursor(m_InterpolatorList);	
		}
	}
 
}

void HBhvAnimation::AdjustKeyframe(int keyframe, int delta, bool doall, bool relative)
{
	HBhvTimeline *tline = GetTimeline();
	if (relative)
    	    tline->AdjustRelative(keyframe,delta); 
	else 
	    tline->Adjust(keyframe,delta, doall); 
}

void HBhvAnimation::DuplicateNextOrPrevious(int keyframe, bool next)
{
	bool replace;
	int pos = GetTimeline()->AddKeyframe(keyframe, replace);
	if (pos!=-1)
	{
		vlist_reset_cursor(m_InterpolatorList);
  		while (1)
		{
			HBhvInterpolator *temp;
			if ((temp = (HBhvInterpolator *)vlist_peek_cursor(m_InterpolatorList)) == 0)
				break;
			temp->Duplicate(pos, replace, next);
  			vlist_advance_cursor(m_InterpolatorList);	
		}
	}
}

void HBhvAnimation::Duplicate(int newkf, int oldkf)
{
	bool replace;
	HBhvTimeline *tl;
	tl = GetTimeline();
 	int pos;
	if ((pos = tl->CheckKeyframe(oldkf))!=-1)
	{
		int adpos = GetTimeline()->AddKeyframe(newkf, replace);
		
		vlist_reset_cursor(m_InterpolatorList);
 		while (1)
		{
			HBhvInterpolator *temp;
			if ((temp = (HBhvInterpolator *)vlist_peek_cursor(m_InterpolatorList)) == 0)
				break;
			temp->Copy(pos, adpos, replace);
  			vlist_advance_cursor(m_InterpolatorList);	
		}
  
	}
}

/*
void HBhvAnimation::AdjustToTick(int totalticks)
{
	int i;
	if (strcmp(m_pInterpolatorInstance->GetInterpolator()->GetType(), "Pos") == 0)
	{
		HBhvInterpolatorPosition *it = (HBhvInterpolatorPosition *)((HBhvInterpolatorInstancePosition *)m_pInterpolatorInstance)->GetInterpolator();
		HKeyframeChannelCurve **splinearray = (HKeyframeChannelCurve **)it->GetArray();
		int length = it->GetArrayLength();
		it->CalculateAllTangents();

		float totallength = 0;
		for (i=0;i<length-1;i++)
		{
	
			splinearray[i]->CalculateCurveLength(splinearray[i+1]->m_cp);
			totallength += splinearray[i]->m_curvelength;
		}

		HBhvTimeline *tl = (HBhvTimeline *)((HBhvTimelineInstance *)m_pTimelineInstance)->GetTimeline();
		int *tlarray = tl->GetTimelineArray();
		for (i=1;i<length;i++)
			tlarray[i] = tlarray[i-1] + (splinearray[i-1]->m_curvelength/totallength * totalticks);
	}
}
*/

void HBhvAnimation::Serialize(HUtilityXMLGenerator *xmlgen, HUtilityXMLTag *xmlt)
{
	xmlt->SetTagname("Animation");
	xmlt->AddProperty("Name", m_Name, true);	 
	xmlt->AddPropertyInt("Loop", (int)m_Loop);
	xmlt->AddPropertyInt("Delay", (int)m_Delay);
	xmlt->SetOpen(true);
	xmlgen->AddTag(xmlt);

	GetTimeline()->Serialize(xmlgen);

	START_LIST_ITERATION(HBhvInterpolator, m_InterpolatorList);{
		temp->Serialize(xmlgen);
	}END_LIST_ITERATION(m_InterpolatorList);
}


void HBhvAnimation::SerializeTarget(HUtilityXMLTag *xmlt)
{
	if (strcmp(m_Target->GetName(), "") != 0)
	{
		char tname[100];
		HBhvTargetObject::CreateTargetString("TOB", m_Target->GetName(), tname);
		xmlt->AddProperty("Target", tname, true);
	}
	else
		xmlt->AddProperty("Target", m_Target->GetPath(), true);		
}
		



void HBhvAnimation::Serialize(HUtilityXMLGenerator *xmlgen)
{
		HUtilityXMLTag xmlt;
		xmlt.SetTagname("Animation");
	
		if (!m_bDefaultActive)
			xmlt.AddPropertyInt("Active", m_bDefaultActive);
		if (strcmp(m_InstanceOf,"") == 0)
		{
 			SerializeTarget(&xmlt);
			Serialize(xmlgen, &xmlt);
  
			START_LIST_ITERATION(HBhvAnimation, m_ChildAnimationList);{
				temp->Serialize(xmlgen);
			}END_LIST_ITERATION(m_ChildAnimationList);
  			xmlgen->CloseTag(&xmlt);

		}
		else
		{
  			xmlt.AddProperty("Name", m_Name, true);
			SerializeTarget(&xmlt);
 			xmlt.AddProperty("InstanceOf", m_InstanceOf, true);		
	   		xmlgen->AddTag(&xmlt);
		}

}




void *HBhvAnimation::XMLCallback(HUtilityXMLTag *xt, bool open, void *m_pExtraData)
{
	char Name[MVO_SMALL_BUFFER_SIZE];
	char InstanceOf[MVO_SMALL_BUFFER_SIZE];
	char Target[MVO_BUFFER_SIZE];
   	int loop;
	int delay;
	int active = 0;
 	HBaseModel *model = (HBaseModel *)m_pExtraData;
	if (!xt->GetProperty("Delay", delay))
				delay = 0;		
	if (!xt->GetProperty("Target", Target))
				strcpy(Target, "");
	if (open)
	{
		if (!xt->GetProperty("Name", Name))
			strcpy(Name, "");
		if (!xt->GetProperty("InstanceOf", InstanceOf))
			strcpy(InstanceOf, "");

	
		if (strcmp(InstanceOf, "") == 0)
		{
 
			if (!xt->GetProperty("Loop", loop))
				loop = 0;

 			HBhvAnimation *outeranimation = (HBhvAnimation *)xt->GetXMLParser()->GetStackData();
  
 			HBhvAnimation * ani = new HBhvAnimation(Name, model->GetBhvBehaviorManager(), outeranimation);
 			ani->m_Loop = loop;
			ani->m_Delay = delay;

			if (strcmp(Target, "") != 0)
				ani->SetTargetByPath(Target);

			if (outeranimation)
 				outeranimation->AddChildAnimation(ani);
			else
 				model->GetBhvBehaviorManager()->AddAnimation(ani);
			return ani;
 		}
		else
		{
			/*
			if (!xt->GetProperty("Active", active))
					active = true;

			char Target[MVO_SMALL_BUFFER_SIZE];
			if (!xt->GetProperty("Target", Target))
				strcpy(Target, "");
			HBhvAnimationInstance *ainst = new HBhvAnimationInstance(model->GetBhvBehaviorManager());
			ainst->SetName(Name);
			ainst->SetInstanceOf(InstanceOf);
			ainst->SetTargetByPath(Target);
			ainst->SetDefaultActive(active != 0);
			ainst->SetDelay(delay);
			model->GetBhvBehaviorManager()->AddAnimationInstance(ainst);
			*/
		}

			
	}
	else
	{
		//			model->GetBhvBehaviorManager->GetCurrentAnimation())
		HBhvAnimation *outeranim = (HBhvAnimation *)xt->GetXMLParser()->GetStackData();
		if (outeranim)
		{
 			if (outeranim->GetParentAnimation() == 0)
			{
				HBhvAnimation *ainst = outeranim;
 
  				if (!xt->GetProperty("Active", active))
						active = true;
				ainst->SetDelay(delay);
				ainst->SetTargetByPath(Target);				
				ainst->SetDefaultActive(active != 0);
			}

 
		}
	}
	return 0;
	
}

bool HBhvAnimation::GetInterpolator(int ctime, int &p, HKeyframe **piece)
{

 	int interval;
 	m_pTimeline->SetStartTick(0);
	if (m_pTimeline->GetTimelineInterval(ctime, interval))
	{
		HBhvTimeline *tl = GetTimeline();
 		int length = tl->GetTimelineArrayLength();
		if (interval >= length)
			return false;

		HKeyframe **kp = GetInterpolator()->GetArray();
		
		p = interval;
		*piece = kp[interval];
		return true;
	}
	return false;
}

 
int HBhvAnimation::GetFirstTick()
{
 	return GetTimeline()->GetTimelineArray()[0];
}

int HBhvAnimation::GetLastTick()
{
 	return GetTimeline()->GetTimelineArray()[GetTimeline()->GetTimelineArrayLength() - 1];
}

void HBhvAnimation::CloneAnimation()
{
	/*
	if (m_pAnimation)
	{

		SetDelay(m_pAnimation->GetDelay());

		if (m_pAnimation->GetTimeline())
			m_pTimelineInstance = m_pAnimation->GetTimeline()->CreateInstance(m_pAnimation);

		HBhvInterpolatorInstance *InterpolatorInstance;
		
		while( (InterpolatorInstance = (HBhvInterpolatorInstance *)vlist_remove_first(m_InterpolatorInstanceList)) != 0 ) 
			delete InterpolatorInstance;
		
 		
 		START_LIST_ITERATION(HBhvInterpolator, m_pAnimation->GetInterpolatorList())
			InterpolatorInstance = temp->CreateInstance(this);
			vlist_add_first(m_InterpolatorInstanceList, (void *)InterpolatorInstance);
		END_LIST_ITERATION(m_pAnimation->GetInterpolatorList())

		HBhvAnimationInstance *AnimationInstance;
		
		while( (AnimationInstance = (HBhvAnimationInstance *)vlist_remove_first(m_AnimationInstanceList)) != 0 ) 
			delete AnimationInstance;

		START_LIST_ITERATION(HBhvAnimation, m_pAnimation->GetAnimationList())
			AnimationInstance = temp->CreateInstance();
			AnimationInstance->SetParentAnimationInstance(this);
			AnimationInstance->SetTargetByPath(temp->GetTargetString());
			vlist_add_first(m_AnimationInstanceList, (void *)AnimationInstance);
		END_LIST_ITERATION(m_pAnimation->GetAnimationList())

 
	}

*/
}


void HBhvAnimation::Reset()
{
	HBhvInterpolator *temp;
	vlist_reset_cursor(m_InterpolatorList);
 	
  	while ((temp = (HBhvInterpolator *)vlist_peek_cursor(m_InterpolatorList)) != 0)
	{
		if (strcmp(temp->GetType(), "QuatRot") == 0)
		{
			HBhvInterpolatorQuatSquad *it = (HBhvInterpolatorQuatSquad *)temp;
			it->AdjustQuaternions();
		}
		temp->Reset();
  		vlist_advance_cursor(m_InterpolatorList);	
	}
	m_bCurrentlyRunning = false;	 
}


bool HBhvAnimation::Animate(float currenttick)
{
	bool result = AnimateInternal(currenttick);
	START_LIST_ITERATION(HBhvAnimation, m_ChildAnimationList);{
		temp->Animate(currenttick, GetTimeline()->GetStartTick());
	}END_LIST_ITERATION(m_ChildAnimationList);
	return result;

}

void HBhvAnimation::Evaluate(float currenttick, bool &hasPos, HPoint &pos, bool &hasQuat, HQuat &quat, bool &hasScale, HPoint &scale)
{
 	int interval;
	float fraction;
 	if (m_pTimeline->Evaluate(currenttick, interval, fraction))
	{
		START_LIST_ITERATION(HBhvInterpolator, m_InterpolatorList);{
			temp->Evaluate(interval, fraction, hasPos, pos, hasQuat, quat, hasScale, scale);
		}END_LIST_ITERATION(m_InterpolatorList);
	}
}


bool HBhvAnimation::AnimateInternal(float currenttick)
{
 	int interval;
	float fraction;
	if (m_pTimeline->Evaluate(currenttick, interval, fraction))
	{
		HBhvTimeline *tl = GetTimeline();
 		int length = tl->GetTimelineArrayLength();
 
		START_LIST_ITERATION(HBhvInterpolator, m_InterpolatorList);{
			temp->Interpolate(interval, fraction);
		}END_LIST_ITERATION(m_InterpolatorList);
		SetCurrentlyRunning(true);
 		if (interval == length)
			return true;
	}
	return false;
}


bool HBhvAnimation::Animate(float currenttick, float startframe)
{
 	m_pTimeline->SetStartTick(startframe);

	bool result = AnimateInternal(currenttick);
	START_LIST_ITERATION(HBhvAnimation, m_ChildAnimationList);{
		temp->Animate(currenttick, startframe);
	}END_LIST_ITERATION(m_ChildAnimationList);
	return result;
}

void HBhvAnimation::SetTargetByPath(const char *name, const char *target)
{
	if (target)
		m_Target = GetBehaviorManager()->CreateTargetObjectByPath(name, target);
	else
		m_Target = 0;
}

void HBhvAnimation::SetTargetByPath(const char *target)
{
	SetTargetByPath(0, target);
}

void HBhvAnimation::SetTarget(HBhvTargetObject *target)
{
	m_Target = target;
}

void HBhvAnimation::SetName(const char *name)
{
	if (name)
		strcpy(m_Name, name);
}

void HBhvAnimation::SetInstanceOf(const char *instanceof)
{
	if (instanceof)
		strcpy(m_InstanceOf, instanceof);
}

void HBhvAnimation::SetTimeline( HBhvTimeline *timeline) 
{
	m_pTimeline = timeline; 
	if (m_pTimeline)
		m_pTimeline->SetAnimation(this); 
}
 
float HBhvAnimation::GetCurrentTick() { 
	return m_pBehaviorManager->GetCurrentTick(); 
}

static HVector ExtractScale(float const * m)
{
	float rx[3];
	float ry[3];
	float rz[3];
	memcpy(rx, m + 0, sizeof(float[3]));
	memcpy(ry, m + 4, sizeof(float[3]));
	memcpy(rz, m + 8, sizeof(float[3]));

	float const sx = static_cast<float>(HC_Compute_Vector_Length(rx));
	float const sy = static_cast<float>(HC_Compute_Vector_Length(ry));
	float const sz = static_cast<float>(HC_Compute_Vector_Length(rz));

	return HVector(sx, sy, sz);
}

static void ExtractScale(float const * in, float * out)
{
	HVector const scale = ExtractScale(in);
	HC_Compute_Scale_Matrix(scale.x, scale.y, scale.z, out);
}

void HBhvAnimation::SetTargetScaleMatrix(float const * target_matrix)
{
	ExtractScale(target_matrix, m_TargetScaleMatrix);
}



