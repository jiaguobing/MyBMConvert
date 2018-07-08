// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// HBhvSensor.cpp: implementation of the HBhvSensor class.
//
//////////////////////////////////////////////////////////////////////
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "HBhvTimeline.h"
#include "HBhvAnimation.h"
#include "HUtilityXMLParser.h"
#include "HUtilityTextParser.h"
#include "HBhvBehaviorManager.h"
#include "HBaseModel.h"
#include "HBaseView.h"
#include "HEventManager.h"
#include "HEventListener.h"
#include "HStream.h"

#include "vlist.h"


#include "HBhvSensor.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

HBhvSensor::HBhvSensor(const char *name, bool active, HBhvBehaviorManager *BehaviorManager)
{
	if (name)
		strcpy(m_Name, name);
	else
		strcpy(m_Name, "");

	m_pBehaviorManager = BehaviorManager;
	m_ConditionList = new_vlist( malloc, free );
	m_ActionList = new_vlist( malloc, free );
	m_bDefaultActive = active;
	m_bCurrentlyActive = false;
	m_Delay = 0;

}


HBhvSensor::~HBhvSensor()
{
	DeActivate();
	START_LIST_ITERATION(HBhvCondition, m_ConditionList);{
		delete temp;
	}END_LIST_ITERATION(m_ConditionList);
	delete_vlist(m_ConditionList);

	START_LIST_ITERATION(HBhvAction, m_ActionList);{
		delete temp;
	}END_LIST_ITERATION(m_ActionList);
	delete_vlist(m_ActionList);

}



void *HBhvSensor::XMLCallback(HUtilityXMLTag *xt, bool open, void *m_pExtraData)
{
	char Name[MVO_SMALL_BUFFER_SIZE];
	int active = true;
 	HBaseModel *model = (HBaseModel *)m_pExtraData;
	if (open)
	{
		if (!xt->GetProperty("Name", Name))
			strcpy(Name, "");
		xt->GetProperty("Active", active);
 		 
		HBhvSensor * sensor = new HBhvSensor(Name, active != 0, model->GetBhvBehaviorManager());		
 		model->GetBhvBehaviorManager()->AddSensor(sensor);
		return sensor;				
	}
	return 0;
}

void HBhvSensor::Serialize(HUtilityXMLGenerator *xmlgen)
{
	HUtilityXMLTag xmlt;
	xmlt.SetTagname("Sensor");
	xmlt.AddProperty("Name", m_Name, true);	 
  	xmlt.AddPropertyInt("Active", (int)m_bDefaultActive);
	xmlt.SetOpen(true);
	xmlgen->AddTag(&xmlt);
		
	START_LIST_ITERATION(HBhvCondition, m_ConditionList);{
		temp->Serialize(xmlgen);
	}END_LIST_ITERATION(m_ConditionList);
	START_LIST_ITERATION(HBhvAction, m_ActionList);{
		temp->Serialize(xmlgen);
	}END_LIST_ITERATION(m_ActionList);
	xmlgen->CloseTag(&xmlt);

}

void HBhvSensor::AddCondition(HBhvCondition *condition)
{
  	vlist_add_last(m_ConditionList, (void *)condition);
}

void HBhvSensor::AddAction(HBhvAction *action)
{
  	vlist_add_last(m_ActionList, (void *)action);
}


void HBhvSensor::Activate(int delay)
{

	if (delay)
		m_Delay = (int)m_pBehaviorManager->GetCurrentTick() + delay;
	else
	{
		START_LIST_ITERATION(HBhvCondition, m_ConditionList);{
			temp->Register();
		}END_LIST_ITERATION(m_ConditionList);
		m_bCurrentlyActive = true;

		HLISTENER_EVENT(HSensorListener, m_pBehaviorManager->GetModel()->GetEventManager(), SensorActivatedEvent(this));		
		m_Delay = 0;
	}
}

void HBhvSensor::DeActivate()
{
	START_LIST_ITERATION(HBhvCondition, m_ConditionList);{
		temp->UnRegister();
	}END_LIST_ITERATION(m_ConditionList);
	m_bCurrentlyActive = false;
	m_Delay = 0;
}
 


bool HBhvSensor::Evaluate()
{
	if (m_bCurrentlyActive)
	{
		int total_counter = 0;
		int and_counter = 0;
		bool and_flag = false, or_flag = false;
		START_LIST_ITERATION(HBhvCondition, m_ConditionList);{
			total_counter++;
 			temp->Evaluate(and_flag, or_flag);
			if (and_flag)
				and_counter++;
			if (or_flag)
				break;
		}END_LIST_ITERATION(m_ConditionList);
		
		if (and_counter == total_counter || or_flag)
		{
			START_LIST_ITERATION(HBhvAction, m_ActionList);{
				temp->DoAction();
			}END_LIST_ITERATION(m_ActionList);
			return true;
		}
	}
	return false;
}

void HBhvSensor::Tick()
{
	if (m_Delay)
	{
		if (m_pBehaviorManager->GetCurrentTick()>=m_Delay)			
			Activate(0);
		
	}
}





HBhvCondition::HBhvCondition(const char *target, const char *event, bool invert, HBhvSensor *sensor)
{
	m_pSensor = sensor;

 	if (target)
		SetTarget(target);
	else
		m_pTarget = 0;
 
 	if (event)
		strcpy(m_Event, event);
	else
		strcpy(m_Event, "");

	m_bInvert = invert;

	m_bConditionMet = false;

}


HBhvCondition::~HBhvCondition()
{



}



void *HBhvCondition::XMLCallback(HUtilityXMLTag *xt, bool open, void *m_pExtraData)
{
	UNREFERENCED(m_pExtraData);

	char Target[MVO_SMALL_BUFFER_SIZE];
	char Event[MVO_SMALL_BUFFER_SIZE];
	char Value[MVO_SMALL_BUFFER_SIZE];
	int invert = false;
// 	HBaseModel *model = (HBaseModel *)m_pExtraData;
	if (open)
	{
		if (!xt->GetProperty("Target", Target))
			strcpy(Target, "");
		if (!xt->GetProperty("Event", Event))
			strcpy(Event, "");
		if (!xt->GetProperty("Value", Value))
			strcpy(Value, "");

		xt->GetProperty("Invert", invert);
			
		 
		HBhvSensor * sensor =  (HBhvSensor *)xt->GetXMLParser()->GetStackData();

		HBhvCondition * condition = 0;
		if (strcmp(Event, "ONLCLICK") == 0)
			condition = (HBhvCondition *)new HBhvConditionONLCLICK(Target, Event, invert != 0, sensor);
		else if (strcmp(Event, "ONMOUSEENTER") == 0)
			condition = (HBhvCondition *)new HBhvConditionONMOUSEENTER(Target, Event, invert != 0, sensor);
		else if (strcmp(Event, "ONMOUSELEAVE") == 0)
			condition = (HBhvCondition *)new HBhvConditionONMOUSELEAVE(Target, Event, invert != 0, sensor);
 		else if (strcmp(Event, "ONANIMRUNNING") == 0)
			condition = (HBhvCondition *)new HBhvConditionAnimationRunning(Target, Event, invert != 0, sensor);
  		else if (strcmp(Event, "ONSENSORACTIVE") == 0)
			condition = (HBhvCondition *)new HBhvConditionSensorActive(Target, Event, invert != 0, sensor);
 		else if (strcmp(Event, "ONANIMFINISHED") == 0)
			condition = (HBhvCondition *)new HBhvConditionAnimationFinished(Target, Event, invert != 0, sensor);
 		else if (strcmp(Event, "ONSENSORACTIVATED") == 0)
			condition = (HBhvCondition *)new HBhvConditionSensorActivated(Target, Event, invert != 0, sensor);
 		else if (strcmp(Event, "ONSENSORACTION") == 0)
			condition = (HBhvCondition *)new HBhvConditionSensorAction(Target, Event, invert != 0, sensor);
		if (condition)
		{
			strcpy(condition->m_Value, Value);
			sensor->AddCondition(condition);		
		}

	}
	return 0;
}


void HBhvCondition::SetTarget(const char *target)
{
	if (target)
		m_pTarget = m_pSensor->GetBehaviorManager()->CreateTargetObjectByPath(0, target);
	else
		m_pTarget = 0;
}

void HBhvCondition::Evaluate(bool &and_flag, bool &or_flag)
{
	UNREFERENCED(or_flag);

	and_flag = m_bConditionMet;
	m_bConditionMet = false;
}
 
void HBhvCondition::Serialize(HUtilityXMLGenerator *xmlgen, HUtilityXMLTag *xmlt)
{
	UNREFERENCED(xmlgen);

	xmlt->SetTagname("Condition");
	xmlt->AddProperty("Target", m_pTarget->GetPath());
	xmlt->AddPropertyInt("Invert", m_bInvert);
}

void HBhvCondition::Serialize(HUtilityXMLGenerator *xmlgen)
{
	UNREFERENCED(xmlgen);
}


HBhvConditionMouse::HBhvConditionMouse(const char *target, const char *event, bool invert, HBhvSensor *sensor)
	: HBhvCondition(target, event, invert, sensor) 
{
}

bool HBhvConditionMouse::IsClicked(HC_KEY target, float x, float y) 
{
	int res;	
	bool found = false;
	int skey_count;
	HC_KEY reskey;

	HBhvBehaviorManager *pBehaviorManager = GetSensor()->GetBehaviorManager();
	HC_Open_Segment_By_Key(target);
		HC_Set_Selectability("faces = on, text = on");
	HC_Close_Segment();
	
	HC_Open_Segment_By_Key(pBehaviorManager->GetActiveView()->GetViewKey());
		HC_QSet_Selectability( "./scene/overwrite", "everything = off" );
		res = HC_Compute_Selection(".","./scene/overwrite","v, selection level = entity", x, y);
		HC_QUnSet_Selectability( "./scene/overwrite" );
	HC_Close_Segment();
	if (res)
	{
		HC_Show_Selection_Element(&reskey, 0, 0, 0);
		char type[MVO_BUFFER_SIZE];
		HC_Show_Key_Type(reskey, type);
		if ( strcmp(type, "segment") != 0)
			reskey  = HC_KShow_Owner_By_Key(reskey);
		if (reskey == target)
			found = true;
		else
		{
			HC_Show_Selection_Keys_Count(&skey_count);
			HC_KEY keys[MVO_SMALL_BUFFER_SIZE];
			
			HC_Show_Selection_Keys(&skey_count, keys);
			
			for (int i=0;i<skey_count; i++)
			{
				if (target == keys[i])
				{
					found = true;
					break;
				}
			}
		}
	}
	HC_Open_Segment_By_Key(target);
		HC_UnSet_Selectability();
	HC_Close_Segment();
	
	return found;
}

 

void HBhvConditionMouse::Register()
{
	HBaseView *view = m_pSensor->GetBehaviorManager()->GetActiveView();
	if (view)
	{
		view->GetEventManager()->RegisterHandler(this, HMouseListener::GetType(), HLISTENER_PRIORITY_NORMAL);
	}
}

void HBhvConditionMouse::UnRegister()
{
	HBaseView *view = m_pSensor->GetBehaviorManager()->GetActiveView();
	if (view)
	{
		view->GetEventManager()->UnRegisterHandler(this, HMouseListener::GetType());
	}
}

HBhvConditionONLCLICK::HBhvConditionONLCLICK(const char *target, const char *event, bool invert, HBhvSensor *sensor) : 
	HBhvConditionMouse(target, event, invert, sensor) 
{

}


int HBhvConditionONLCLICK::OnLButtonDown(HEventInfo &event)
{
	 HPoint windowpos = event.GetMouseWindowPos();
	if (IsClicked(m_pTarget->GetTargetKey(), windowpos.x, windowpos.y))
	{
		m_bConditionMet = true;
		if (m_pSensor->Evaluate())
			return HLISTENER_CONSUME_EVENT;
	
	}
	return HLISTENER_PASS_EVENT;

}

void HBhvConditionONLCLICK::Serialize(HUtilityXMLGenerator *xmlgen)
{
	HUtilityXMLTag xmlt;
	HBhvCondition::Serialize(xmlgen, &xmlt);
   	xmlt.AddProperty("Event", "ONLCLICK");
  	xmlgen->AddTag(&xmlt);
}


HBhvConditionONMOUSEENTER::HBhvConditionONMOUSEENTER(const char *target, const char *event, bool invert, HBhvSensor *sensor) : 
	HBhvConditionMouse(target, event, invert, sensor) 
{
	m_OldKey = INVALID_KEY;
}


int HBhvConditionONMOUSEENTER::OnMouseMove(HEventInfo &event)
{
	 HPoint windowpos = event.GetMouseWindowPos();
	if (IsClicked(m_pTarget->GetTargetKey(), windowpos.x, windowpos.y))
	{

		if (m_OldKey != m_pTarget->GetTargetKey())
		{
			m_bConditionMet = true;
			m_pSensor->Evaluate();
			m_OldKey = m_pTarget->GetTargetKey();
	
		}
	}
	else
		m_OldKey = INVALID_KEY;
 	return HLISTENER_PASS_EVENT;

}

void HBhvConditionONMOUSEENTER::UnRegister()
{
	m_OldKey = INVALID_KEY;
	HBhvConditionMouse::UnRegister();
}

void HBhvConditionONMOUSEENTER::Serialize(HUtilityXMLGenerator *xmlgen)
{
	HUtilityXMLTag xmlt;
	HBhvCondition::Serialize(xmlgen, &xmlt);
   	xmlt.AddProperty("Event", "ONMOUSEENTER");
  	xmlgen->AddTag(&xmlt);
}


HBhvConditionONMOUSELEAVE::HBhvConditionONMOUSELEAVE(const char *target, const char *event, bool invert, HBhvSensor *sensor) : 
	HBhvConditionMouse(target, event, invert, sensor) 
{
	m_OldKey = INVALID_KEY;
}


int HBhvConditionONMOUSELEAVE::OnMouseMove(HEventInfo &event)
{
	 HPoint windowpos = event.GetMouseWindowPos();
	if (IsClicked(m_pTarget->GetTargetKey(), windowpos.x, windowpos.y))
	{
		if (m_OldKey != m_pTarget->GetTargetKey())
			m_OldKey = m_pTarget->GetTargetKey();
	
	}
	else
	{
		if (m_OldKey == m_pTarget->GetTargetKey())
		{
			m_bConditionMet = true;
			m_pSensor->Evaluate();
		}
		m_OldKey = INVALID_KEY;
	}
 	return HLISTENER_PASS_EVENT;

}

void HBhvConditionONMOUSELEAVE::UnRegister()
{
	m_OldKey = INVALID_KEY;
	HBhvConditionMouse::UnRegister();
}

void HBhvConditionONMOUSELEAVE::Serialize(HUtilityXMLGenerator *xmlgen)
{
	HUtilityXMLTag xmlt;
	HBhvCondition::Serialize(xmlgen, &xmlt);
   	xmlt.AddProperty("Event", "ONMOUSELEAVE");
  	xmlgen->AddTag(&xmlt);
}


HBhvConditionAnimationRunning::HBhvConditionAnimationRunning(const char *target, const char *event, bool invert, HBhvSensor *sensor) : 
	HBhvCondition(target, event, invert, sensor) 
{
	
}


void HBhvConditionAnimationRunning::Evaluate(bool &and_flag, bool &or_flag)
{
	UNREFERENCED(or_flag);

	HBhvAnimation *ainst = GetSensor()->GetBehaviorManager()->FindAnimationByName(m_pTarget->GetResolvedPath());
	if (ainst->GetCurrentlyRunning())
		and_flag = true;
	else
		and_flag = false;
	if (m_bInvert)
		and_flag = !and_flag;
}

void HBhvConditionAnimationRunning::Serialize(HUtilityXMLGenerator *xmlgen)
{
	HUtilityXMLTag xmlt;
	HBhvCondition::Serialize(xmlgen, &xmlt);
   	xmlt.AddProperty("Event", "ONANINIMRUNNING");
  	xmlgen->AddTag(&xmlt);
}


HBhvConditionSensorActive::HBhvConditionSensorActive(const char *target, const char *event, bool invert, HBhvSensor *sensor) : 
	HBhvCondition(target, event, invert, sensor) 
{
	
}


void HBhvConditionSensorActive::Evaluate(bool &and_flag, bool &or_flag)
{
	UNREFERENCED(or_flag);

	HBhvSensor *sensor = GetSensor()->GetBehaviorManager()->FindSensorByName(m_pTarget->GetResolvedPath());
	if (sensor->GetCurrentlyActive())
		and_flag = true;
	else
		and_flag = false;
	if (m_bInvert)
		and_flag = !and_flag;
}


void HBhvConditionSensorActive::Serialize(HUtilityXMLGenerator *xmlgen)
{
	HUtilityXMLTag xmlt;
	HBhvCondition::Serialize(xmlgen, &xmlt);
   	xmlt.AddProperty("Event", "ONSENSORACTIVE");
  	xmlgen->AddTag(&xmlt);
}




HBhvConditionSensor::HBhvConditionSensor(const char *target, const char *event, bool invert, HBhvSensor *sensor) : 
	HBhvCondition(target, event, invert, sensor) 
{

}

  
 

void HBhvConditionSensor::Register()
{
 		GetSensor()->GetBehaviorManager()->GetModel()->GetEventManager()->RegisterHandler((HSensorListener *)this, HSensorListener::GetType(), HLISTENER_PRIORITY_NORMAL);
}

void HBhvConditionSensor::UnRegister()
{
 		GetSensor()->GetBehaviorManager()->GetModel()->GetEventManager()->UnRegisterHandler((HSensorListener *)this, HSensorListener::GetType());
}






HBhvConditionSensorActivated::HBhvConditionSensorActivated(const char *target, const char *event, bool invert, HBhvSensor *sensor) : 
	HBhvConditionSensor(target, event, invert, sensor) 
{
}



int HBhvConditionSensorActivated::SensorActivatedEvent(HBhvSensor * sensor)
{
	if (strcmp(sensor->GetName(), m_pTarget->GetResolvedPath()) == 0)
	{
		m_bConditionMet = true;
		m_pSensor->Evaluate();
	}
 	return HLISTENER_PASS_EVENT;
}


void HBhvConditionSensorActivated::Serialize(HUtilityXMLGenerator *xmlgen)
{
	HUtilityXMLTag xmlt;
	HBhvCondition::Serialize(xmlgen, &xmlt);
   	xmlt.AddProperty("Event", "ONSENSORACTIVATED");
  	xmlgen->AddTag(&xmlt);
}




HBhvConditionSensorAction::HBhvConditionSensorAction(const char *target, const char *event, bool invert, HBhvSensor *sensor) : 
	HBhvConditionSensor(target, event, invert, sensor) 
{
}



int HBhvConditionSensorAction::SensorActionEvent(const char *eventname)
{
	if (strcmp(m_Value, eventname) == 0)
	{
		m_bConditionMet = true;
		m_pSensor->Evaluate();
	}
 	return HLISTENER_PASS_EVENT;
}


void HBhvConditionSensorAction::Serialize(HUtilityXMLGenerator *xmlgen)
{
	HUtilityXMLTag xmlt;
	HBhvCondition::Serialize(xmlgen, &xmlt);
   	xmlt.AddProperty("Event", "ONSENSORACTION");
   	xmlt.AddProperty("Value", m_Value);
  	xmlgen->AddTag(&xmlt);
}







HBhvConditionAnimation::HBhvConditionAnimation(const char *target, const char *event, bool invert, HBhvSensor *sensor) : 
	HBhvCondition(target, event, invert, sensor) 
{

}

  
 

void HBhvConditionAnimation::Register()
{
	HBaseView *view = m_pSensor->GetBehaviorManager()->GetActiveView();
	if (view)
	{
		view->GetModel()->GetEventManager()->RegisterHandler((HAnimationListener *)this, HAnimationListener::GetType(), HLISTENER_PRIORITY_NORMAL);
	}
}

void HBhvConditionAnimation::UnRegister()
{
	HBaseView *view = m_pSensor->GetBehaviorManager()->GetActiveView();
	if (view)
	{
		view->GetModel()->GetEventManager()->UnRegisterHandler((HAnimationListener *)this, HAnimationListener::GetType());
	}
}


HBhvConditionAnimationFinished::HBhvConditionAnimationFinished(const char *target, const char *event, bool invert, HBhvSensor *sensor) : 
	HBhvConditionAnimation(target, event, invert, sensor) 
{
}



int HBhvConditionAnimationFinished::AnimationFinishedEvent(HBhvAnimation * ainst)
{
	if (strcmp(ainst->GetName(), m_pTarget->GetResolvedPath()) == 0)
	{
		m_bConditionMet = true;
		m_pSensor->Evaluate();
	}
 	return HLISTENER_PASS_EVENT;
}


void HBhvConditionAnimationFinished::Serialize(HUtilityXMLGenerator *xmlgen)
{
	HUtilityXMLTag xmlt;
	HBhvCondition::Serialize(xmlgen, &xmlt);
   	xmlt.AddProperty("Event", "ONANIMFINISHED");
  	xmlgen->AddTag(&xmlt);
}

HBhvAction::HBhvAction(const char *target, const char *type, const char *value, HBhvSensor *sensor)
{	
	m_pSensor = sensor;
 	
	if (target)
		SetTargetByPath(target);
	else
		m_pTarget = 0;
 
 	if (type)
		strcpy(m_Type, type);
	else
		strcpy(m_Type, "");
 	if (value)
		strcpy(m_Value, value);
	else
		strcpy(m_Value, "");
}


HBhvAction::~HBhvAction()
{
}



void *HBhvAction::XMLCallback(HUtilityXMLTag *xt, bool open, void *m_pExtraData)
{
	UNREFERENCED(m_pExtraData);

	char Target[MVO_SMALL_BUFFER_SIZE];
	char Type[MVO_SMALL_BUFFER_SIZE];
	char Value[MVO_SMALL_BUFFER_SIZE];
// 	HBaseModel *model = (HBaseModel *)m_pExtraData;
	if (open)
	{
		if (!xt->GetProperty("Target", Target))
			strcpy(Target, "");
		if (!xt->GetProperty("Type", Type))
			strcpy(Type, "");
		if (!xt->GetProperty("Value", Value))
			strcpy(Value, "");
		 
		HBhvSensor * sensor =  (HBhvSensor *)xt->GetXMLParser()->GetStackData();

		HBhvAction * action = new HBhvAction(Target, Type, Value, sensor);
		sensor->AddAction(action);		
	}
	return 0;
}


void HBhvAction::SetTargetByPath(const char *targetpath)
{
	if (targetpath)
		m_pTarget = m_pSensor->GetBehaviorManager()->CreateTargetObjectByPath(0, targetpath);
	else
		m_pTarget = 0;
}

void HBhvAction::DoAction()
{
	HBhvBehaviorManager *pBehaviorManager = m_pSensor->GetBehaviorManager();
	if (strcmp(m_Type, "DOACTION") == 0)
	{
		HLISTENER_EVENT(HSensorListener, pBehaviorManager->GetModel()->GetEventManager(), SensorActionEvent(m_Value));		
	}
	else if (strcmp(m_Type, "LOADFILE") == 0)
	{
		char dummy[MVO_SMALL_BUFFER_SIZE];
		char filename[MVO_SMALL_BUFFER_SIZE];
		char directory[MVO_SMALL_BUFFER_SIZE];
		int dlum;

		static delimiter_t find_items_td[] = {	{"\x41-\x7a", false, ":", true, false} };
		static delimiter_t find_items_td2[] = {	{"\x41-\x7a", false, ",", true, false} };
		static delimiter_t find_items_td3[] = {	{"\x41-\x7a", false, "", true, false} };
 		HUtilityTextParser tp;
		tp.InsertBuffer(m_Value, (unsigned int)strlen(m_Value));
		tp.ParseBuffer(dummy, (int &)dlum, find_items_td, 1);
		tp.ParseBuffer(filename, (int &)dlum, find_items_td2, 1);
		tp.ParseBuffer(dummy, (int &)dlum, find_items_td, 1);
		tp.ParseBuffer(directory, (int &)dlum, find_items_td3, 1);

		HStreamFileToolkit *tk = m_pSensor->GetBehaviorManager()->GetActiveView()->GetModel()->GetStreamFileTK();
		HC_Open_Segment_By_Key(m_pSensor->GetBehaviorManager()->GetActiveView()->GetModelKey());
		HC_Open_Segment(directory);
		HTK_Read_Stream_File(filename,0,tk);
		HC_Close_Segment();
		HC_Close_Segment();
 		
		
	}
	else if (strcmp(m_pTarget->GetType(), "ANIM") == 0)
	{
		HBhvAnimation *ainst = pBehaviorManager->FindAnimationByName(m_pTarget->GetResolvedPath());
		if (strcmp(m_Type, "ACTIVATE") == 0)
		{
 			pBehaviorManager->RemoveScheduledAnimation(ainst);
			int delay;
			if (strcmp(m_Value, "") != 0)
				delay = atoi(m_Value);
			else
				delay = 0;
			ainst->SetDelay(delay);
			pBehaviorManager->ScheduleAnimation(ainst, pBehaviorManager->GetCurrentTick());
		}
		else if (strcmp(m_Type, "CONTINUE") == 0)
		{
 			pBehaviorManager->RemoveScheduledAnimation(ainst);
			HBhvTimeline *ti = ainst->GetTimeline();
			if (!ainst->GetDefaultActive())
 				pBehaviorManager->ScheduleAnimation(ainst,pBehaviorManager->GetCurrentTick() - (ti->GetCurrentTick() - ti->GetStartTick()));
			else
				pBehaviorManager->ScheduleAnimation(ainst, pBehaviorManager->GetCurrentTick());
		}
		else if (strcmp(m_Type, "DEACTIVATE") == 0)
		{
 			pBehaviorManager->RemoveScheduledAnimation(ainst);
			ainst->SetCurrentlyRunning(false);
		}
	}
	else if (strcmp(m_pTarget->GetType(), "SSR") == 0)
	{
		HBhvSensor *sensor = pBehaviorManager->FindSensorByName(m_pTarget->GetResolvedPath());
		if (strcmp(m_Type, "ACTIVATE") == 0 && !sensor->GetCurrentlyActive())
		{
			if (strcmp(m_Value, "") != 0)
				sensor->Activate(atoi(m_Value));
			else
				sensor->Activate(0);

		}
 		else if (strcmp(m_Type, "DEACTIVATE") == 0)
		{
			sensor->DeActivate();

		}
	}

}

void HBhvAction::Serialize(HUtilityXMLGenerator *xmlgen)
{
	HUtilityXMLTag xmlt;
	xmlt.SetTagname("Action");
	xmlt.AddProperty("Target",m_pTarget->GetPath());	 
	xmlt.AddProperty("Type",m_Type);
	if (strcmp(m_Value, "") != 0)
		xmlt.AddProperty("Value",m_Value);	 
	xmlgen->AddTag(&xmlt);
	
}
