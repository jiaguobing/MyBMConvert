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

#include "HTools.h"

#include "hc.h"
#include "hic.h"

#include "HDB.h"
#include "HBaseView.h"
#include "HBaseOperator.h"
 
#include "HImManager.h"
#include "HUtility.h"
#include "HConstantFrameRate.h"
#include "HUtilityAntialiasing.h"
#include "HBhvBehaviorManager.h"

#include "HModelInfo.h"

#define COUNTER_THRESHOLD 10



void HFrSolidBBox::RegisterCallbacks()
{
	HImRegisterCallback("HFrSolidBBox_draw_solid_bbx", draw_solid_bbx);
	HImRegisterCallback("HFrSolidBBox_draw_solid_bbx_tree", draw_solid_bbx_tree);
}

void HFrWireframeBBox::RegisterCallbacks()
{
	HImRegisterCallback("HFrWireframeBBox_draw_wireframe_bbx", draw_wireframe_bbx);
}



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

HConstantFrameRate::HConstantFrameRate(HBaseView *view)
{
	m_pHView = view;
	m_pSimpList = 0;
  	m_bConstFRActive = false;
  	m_TargetFrameRate = 10.0;
	m_SimpListLength = 0;
	SetDetailIncMode(NoDetailChange);
	SetDetailDecMode(AllowDetailChange);
	SetDetailIncOnIdleMode(AllowDetailChange);
  	m_ActivityType  = NoActivity;
	m_bFullDetailTemp = true;
  	m_pUpdateTimerClient = 0;
	m_bDisableIncreaseTemp = false;
	m_OneDelayStart = 0;
	m_bOneDelay = false;
	m_bAdjustNextTime = false;
	m_bIncrease = false;
	m_CounterThreshold = COUNTER_THRESHOLD;
	m_bFirstIncrease = true;
	m_lastupdown = 0;
	for (int i=0;i<11;i++)
		m_oldPos[i] = 0;
}

HConstantFrameRate::~HConstantFrameRate()
{
	Shutdown();
	delete m_pUpdateTimerClient;
}




bool HConstantFrameRate::GetMode(HConstFRSimpType ***sd, float *target_framerate, int *length)
{				
	if (sd)
		*sd = m_pSimpList; 
	if (target_framerate)
		*target_framerate = m_TargetFrameRate;
	if (length)
		*length = m_SimpListLength;
	return m_pSimpList ? true : false;
}


void HConstantFrameRate::Init(float target_framerate, HConstFRSimpType **list,int length, float deadzone)
{
 
	Shutdown();
 
	if (!list)
	{
		// if we have an opengl card then don't do the wireframe case since OpenGL cards
		// do not typically provide good wireframe acceleration
		if (streq(m_pHView->GetDriverType(), "opengl") || streq(m_pHView->GetDriverType(), "direct3d"))				
		{
 			m_pOwnsList = true;
		    m_pSimpList = new HConstFRSimpType *[3];
			m_pSimpList[0] = new HFrNoSimp;
			m_pSimpList[1] = new HFrLodClamp(0,4);
			m_pSimpList[2] = new HFrSolidBBox;
 			length = 3;
			SetDetailLevel(1,1);
		}
		else
		{
			m_pOwnsList = true;
 		    m_pSimpList = new HConstFRSimpType *[4];
			m_pSimpList[0] = new HFrNoSimp;
			m_pSimpList[1] = new HFrLodClamp(0,4);
			m_pSimpList[2] = new HFrWireframe(1,1);
			m_pSimpList[3] = new HFrSolidBBox;
  			length = 4;
			SetDetailLevel(1,1);

		}
	} 
	else
	{
		m_pOwnsList=false;
		m_pSimpList=list;	
		m_SimpListPos = 0;
		m_SimpLevel = 0;	
		SetDetailLevel(0,0);	 
	}
	
	
	m_SimpListLength = length;

	m_TargetFrameRate = target_framerate;
	m_DeadZone = deadzone;
	m_bEmergencyDecrease = true;
	m_bWatchActivity = true;
}


void HConstantFrameRate::Shutdown()
{
 
	if (m_pSimpList && m_pOwnsList)
	{
		for (int i=0;i<m_SimpListLength;i++)
			delete m_pSimpList[i];

		delete [] m_pSimpList;
		
	}

	m_pSimpList = 0;
  	m_SimpListLength = 0;
}




void HConstantFrameRate::AdjustLodOptions()
{
	char cval[128];
	int pos = m_SimpListPos;
	while (pos>0 && m_pSimpList[pos]->GetType() != ConstFRLodClampType && m_pSimpList[pos]->GetType() != ConstFRLodThresholdType)
		pos--;

	if (m_pSimpList[pos]->GetType() == ConstFRLodClampType)
	{
		sprintf(cval, "lod, lodo =(clamp=%d, fallback = coarsest)", m_pSimpList[pos]->m_SimpLevelMax);
		HC_Set_Rendering_Options(cval);	 	
		HC_Set_Rendering_Options ("tessellation=(cylinder=(24,12,6,4,2))");
	}
	else if (m_pSimpList[pos]->GetType() == ConstFRLodThresholdType)
	{
 		float value=(float)301.0f/((float)(m_pSimpList[pos]->m_SimpLevelMax+1.0f)*5.0f);

		HCLOCALE(sprintf(cval, "lod, lodo = (threshold = %f, no clamp, fallback = coarsest)",			
			value));
 		HC_Set_Rendering_Options(cval);	 	
		HC_Set_Rendering_Options ("tessellation=(cylinder=(24,12,6,4,2))");
	}
}



void HConstantFrameRate::SetHoopsSimplificationOptions(int SimpLevel, int SimpListPos)
{

 
	HC_Open_Segment_By_Key(m_pHView->GetOverwriteKey());	 
	//this defines all the settings for the different simplification types
 	HUtility::UnsetAttributes();

	if (m_pHView->GetShadowMode() == HShadowSoft)
	{
//		HC_Open_Segment_By_Key(m_pHView->GetSceneKey());	 
//			HC_Set_Rendering_Options("simple shadow = on");
//		HC_Close_Segment();
	}

   	for (int i=0;i<SimpListPos;i++)
 		m_pSimpList[i]->Set(this, m_pSimpList[i]->m_SimpLevelMax);

	m_pSimpList[SimpListPos]->Set(this, SimpLevel);
   	for (int i=SimpListPos+1;i<this->m_SimpListLength;i++)
 		m_pSimpList[i]->Reset(this);

 	HC_Close_Segment();
}

void HConstantFrameRate::UpdateLevel(int SimpLevel, int SimpListPos)
{

  		m_pSimpList[SimpListPos]->Update(this, SimpLevel);
}


void HConstantFrameRate::AdjustDetail(float ActualFrameRate)
{
	//	char cval[128];
	 int updown=0;   	
	 bool bIncreaseSet = false;
	 float deadzone;
	 if (m_TargetFrameRate<=15.0f)
		 deadzone = 3;
	 else if (m_TargetFrameRate<=17.0f)
		 deadzone = 4;
	 else if (m_TargetFrameRate<=25.0f)
		 deadzone = 5;
	 else
		 deadzone = 7;
 		
	if (ActualFrameRate>99)
		ActualFrameRate=99;
	
	int counterdiv;
	if (m_TargetFrameRate <= 10)
		counterdiv = 2;
	else
		counterdiv = 1;

	


 	if (m_TargetFrameRate > ActualFrameRate + deadzone) 
		updown = 1;
	else if (m_TargetFrameRate < ActualFrameRate - deadzone/2) 
		updown=-1;

	if (updown == 0 && m_lastupdown != 0)
	{
 		if (m_lastupdown == -1)
		{
			if (m_TargetFrameRate < ActualFrameRate) 						
				updown = -1;
			else
				updown = 0;
		}
 		if (m_lastupdown == 1)
		{
			if (m_TargetFrameRate > ActualFrameRate) 						
				updown = 1;
			else
				updown = 0;
		}
	}
	m_lastupdown = updown;

	//if the framerate is lower than the target framerate (inside a certain range)
	//a lower detail level is selected
 	if (updown == 1)
	{
		m_FrameRateCounter = 0;
		
		if ((m_TargetFrameRate > ActualFrameRate) && m_SimplificationDecrease != NoDetailChange)
		{
			m_SimpLevel++;
			if (m_SimplificationIncrease == AllowDetailChangeOnce)
				m_SimplificationIncrease = NoDetailChange;
			if (m_bIncrease)
				m_CounterThreshold+= (10/counterdiv);
		}
		else
			updown = 0;	

	}	
	//if the framerate is higher than the target framerate (inside a certain range)
	//a higher detail level is used again but only if the simplification was not reduced before
	//this avoids the annoying switch between two simplification settings
	else if (updown == -1)
	{
		m_FrameRateCounter++;
		if (m_FrameRateCounter > m_CounterThreshold)
		{		
			m_FrameRateCounter = 0;
 			if (m_SimplificationIncrease != NoDetailChange)
			{
				m_SimpLevel--;
				m_bIncrease = true;
				bIncreaseSet = true;
				m_CounterThreshold -= (8/counterdiv);
				if (m_CounterThreshold < (COUNTER_THRESHOLD/counterdiv))
					m_CounterThreshold = (COUNTER_THRESHOLD/counterdiv);
			}
			else
				updown = 0;
		}
		else
			updown = 0;
	}
	else
		m_FrameRateCounter = 0;
	
 
	
	//this code tries to react immediatly to a very low framerate by switching to the highest available
	//simplification level of the active type
 	if (ActualFrameRate < 3.0f && m_bEmergencyDecrease)
	{
		ChangeDetail(5);
		if (m_SimplificationIncrease == AllowDetailChangeOnce)
					m_SimplificationIncrease = NoDetailChange;		
	}

	 AdjustListPointers(m_SimpLevel, m_SimpListPos);
	 if (!bIncreaseSet)
		 m_bIncrease = false;


}



void HConstantFrameRate::Start()
{
 	if (m_pSimpList && m_pHView->GetRenderMode()!=HRenderHiddenLine && !m_bConstFRActive)
	{	
		//this callback is always set to retrieve the number of shells in the scene
//		HC_Open_Segment_By_Key(m_pHView->GetSceneKey());
 //			HC_Set_Rendering_Options("lodo = (fallback = coarsest)");
//		HC_Close_Segment();

		m_SimplificationIncrease=m_DetailIncMode;
		m_SimplificationDecrease=m_DetailDecMode;
		m_SimplificationIncreaseOnIdle=m_DetailIncOnIdleMode;

		m_FrameRateCounter = 0;
  		m_bConstFRActive = true;
		m_bFullDetailTemp = true;

		//we call this function only so that the correct Rendering Options are initialized
		//(and we don't need to duplicate the code here or invent another function)
		AdjustDetail(m_TargetFrameRate);
		SetHoopsSimplificationOptions(m_SimpLevel, m_SimpListPos);
//		m_pHView->SetShadowRegenerationMode(false);
 	//	strcpy(m_OldRenderingOptions, OldRenderingOptions);
		StartTimer(); 
	}
}


void HConstantFrameRate::StartTimer()
{
    
    if (m_pUpdateTimerClient)
	HDB::GetHTManager()->UnRegisterClient( m_pUpdateTimerClient ); 
    delete m_pUpdateTimerClient;
    
    float StartTime;
    HC_Show_Time(&StartTime);
    
    m_pUpdateTimerClient = new 	HTClient(0.01f, 
	HTCS_PeriodicSkip, Tick, this); 
    m_pUpdateTimerClient->SetNextRequest(StartTime + 0.01f);
     HDB::GetHTManager()->RegisterClient( m_pUpdateTimerClient );

}
void HConstantFrameRate::StopTimer()
{
    if (m_pUpdateTimerClient)
    {    
		HDB::GetHTManager()->UnRegisterClient( m_pUpdateTimerClient ); 
		delete m_pUpdateTimerClient;
		m_pUpdateTimerClient = 0;
    }
}

bool HConstantFrameRate::Tick(float request_time, float actual_time, void * user_data)
{
	UNREFERENCED(request_time);
	UNREFERENCED(actual_time);

  	HConstantFrameRate *cframerate = (HConstantFrameRate *)user_data;
 	if (cframerate->GetActive())
	{	
		if (cframerate->m_OneDelayStart)
		{
			float dend;
			HC_Show_Time(&dend);
			if (dend - cframerate->m_OneDelayStart < 1.0f)
				return true;
			else
				cframerate->m_OneDelayStart = 0;
		}
		HBaseView * view = cframerate->GetView(); 		
		if (view->GetIdleTime() > 0.2f &&
			(!view->GetOperator() || !view->GetOperator()->OperatorStarted())  &&
			!cframerate->GetActivityType() && 
			!view->GetModel()->GetBhvBehaviorManager()->IsPlaying())
		{
 			cframerate->IncreaseDetailTemp();					 
		}				
		if (cframerate->GetActivityType() == GeneralActivity)
			cframerate->m_bFirstIncrease = true;

		if (cframerate->m_bOneDelay)
		{
			HC_Show_Time(&cframerate->m_OneDelayStart);
			cframerate->m_bOneDelay = false;
		}
 
	}					
	cframerate->SetActivityType(NoActivity);

	return true;
}

void HConstantFrameRate::Stop()
{	
	if (m_pSimpList && m_bConstFRActive)
	{	
		HC_Open_Segment_By_Key(m_pHView->GetViewKey());
			HC_Open_Segment("ttext");	
				HC_Flush_Contents(".","everything");
 		HC_Close_Segment();
		HC_Close_Segment();
		m_bConstFRActive=false;

 
		HC_Open_Segment_By_Key(m_pHView->GetOverwriteKey());
 		HUtility::UnsetAttributes();		
		HC_Close_Segment();

		HC_Control_Update_By_Key(m_pHView->GetViewKey(),"redraw everything");	
		m_pHView->SetShadowRegenerationMode(true);		
 
		StopTimer();
		m_pHView->Update();
		
	}
	
}

void HConstantFrameRate::AdjustDefaultDetailLevelToModel()
{
	char buf[MVO_BUFFER_SIZE];
	int total=0, current=0;

	HC_Show_Device_Info_By_Key(m_pHView->GetViewKey(), "dc triangles", buf);
	sscanf(buf, "%d", &current);
	total+=current;
	HC_Show_Device_Info_By_Key(m_pHView->GetViewKey(), "dl 3d triangles", buf);
	sscanf(buf, "%d", &current);
	total+=current;
	HC_Show_Device_Info_By_Key(m_pHView->GetViewKey(), "3d triangles", buf);
	sscanf(buf, "%d", &current);
	total+=current;
	
	if (m_SimpListLength > 1 && m_pSimpList[1]->GetType() == ConstFRExtentType)
	{
		m_SimpListPos = 1;
		m_SimpLevel = 1;
//		(m_pSimpList[1]->GetMaxSimpLevel() - m_pSimpList[1]->GetMinSimpLevel()) / 2.0f;
	    AdjustListPointers(m_SimpLevel, m_SimpListPos);
		m_bFullDetailTemp = false;
	}

#if 0
 	if (total > 100000)
	{
		m_SimpLevel = 1000;
		m_SimpListPos = 1000;
	    AdjustListPointers(m_SimpLevel, m_SimpListPos);
		m_bFullDetailTemp = true;
	}
#endif
}

bool HConstantFrameRate::IsLowestDetail()
{
	AdjustListPointers(m_SimpLevel, m_SimpListPos);
	if (m_SimpListPos == m_SimpListLength-1 && m_pSimpList[m_SimpListPos]->m_SimpLevelMax == m_SimpLevel)
		return true;
	else
		return false;
}


int HConstantFrameRate::GetDetailLevelNum()
{
	int sum = 0;
	for (int i=0;i<m_SimpListPos;i++)
 		sum+=(m_pSimpList[i]->m_SimpLevelMax - m_pSimpList[i]->m_SimpLevelMin + 1);
 
 	sum+=(m_SimpLevel - m_pSimpList[m_SimpListPos]->m_SimpLevelMin + 1);

	return sum;
}

int HConstantFrameRate::GetTotalDetailLevelNum()
{
	int sum = 0;
	for (int i=0;i<m_SimpListLength;i++)
 		sum+=(m_pSimpList[i]->m_SimpLevelMax - m_pSimpList[i]->m_SimpLevelMin + 1);
 
	return sum;
}

void HConstantFrameRate::MajorDetailDecrease()
{

//	int total = (m_pSimpList[m_SimpListPos]->m_SimpLevelMax - m_pSimpList[m_SimpListPos]->m_SimpLevelMin);
	int total = GetTotalDetailLevelNum();
	int decrease;
	if (GetTotalDetailLevelNum() > 10)
		decrease = total/4;
	else decrease = 1;

	ChangeDetail(decrease);


#if 0
	int lp = m_SimpListPos;
	int level = m_SimpLevel;



	ChangeDetail(decrease, &level, &lp);
	if (m_SimpListPos != lp)
	{
		lp = m_SimpListPos + 1;
		level = m_pSimpList[lp]->m_SimpLevelMin;
	}
	
	m_SimpLevel = level;
	m_SimpListPos = lp;
#endif
}


void HConstantFrameRate::MajorDetailIncrease()
{
	int total = (m_pSimpList[m_SimpListPos]->m_SimpLevelMax - m_pSimpList[m_SimpListPos]->m_SimpLevelMin);
	int increase;
	if (total > 10)
		increase = -3;
	else increase = -1;

	int lp = m_SimpListPos;
	int level = m_SimpLevel;

	ChangeDetail(increase, &level, &lp);
	if (m_SimpListPos != lp)
	{
		lp = m_SimpListPos - 1;
		level = m_pSimpList[lp]->m_SimpLevelMax;
	}
	
	m_SimpLevel = level;
	m_SimpListPos = lp;
}



	

void HConstantFrameRate::ChangeDetail(int change, int *SimpLevel, int *SimpListPos)
{
	int csimplevel;
	int clistpos;
	if (!SimpLevel)
 		csimplevel = m_SimpLevel;
	else
		csimplevel = *SimpLevel;
	if (!SimpListPos)
		clistpos = m_SimpListPos;
	else
		clistpos = *SimpListPos;
	int cchange = change;
	if (change < 0)
		cchange=-change;
	if (change>0)
	{	
		while (1)
		{
			if (csimplevel+cchange <= m_pSimpList[clistpos]->m_SimpLevelMax)
			{
				csimplevel+=cchange;
				break;
			}
			cchange -= (m_pSimpList[clistpos]->m_SimpLevelMax - csimplevel);
			csimplevel = m_pSimpList[clistpos]->m_SimpLevelMax;
		
			if (clistpos == m_SimpListLength -1)
				break;
			clistpos++;
			csimplevel = m_pSimpList[clistpos]->m_SimpLevelMin;
		}

	}
	else if (change < 0)
	{	
		while (1)
		{
			if (csimplevel-cchange >= m_pSimpList[clistpos]->m_SimpLevelMin)
			{
				csimplevel-=cchange;
				break;
			}
			cchange -= (csimplevel - m_pSimpList[clistpos]->m_SimpLevelMin);
			csimplevel = m_pSimpList[clistpos]->m_SimpLevelMin;
		
			if (clistpos == 0)
				break;
			clistpos--;
			csimplevel = m_pSimpList[clistpos]->m_SimpLevelMax;
		}

	}

	if (!SimpLevel)
		m_SimpLevel = csimplevel;
	else
		*SimpLevel = csimplevel;
	if (!SimpListPos)
		m_SimpListPos = clistpos;
	else
		*SimpListPos = clistpos;

}


void HConstantFrameRate::AdjustListPointers(int &SimpLevel, int &SimpListPos)
{
	if (SimpListPos >= m_SimpListLength)
		SimpListPos = m_SimpListLength - 1;
 	if (SimpLevel < m_pSimpList[SimpListPos]->m_SimpLevelMin)
		{
			if (SimpListPos > 0)
			{
				SimpListPos--;
				SimpLevel = m_pSimpList[SimpListPos]->m_SimpLevelMax;
			}
			else
				SimpLevel = m_pSimpList[SimpListPos]->m_SimpLevelMin;			
			
		}
		else if (SimpLevel > m_pSimpList[SimpListPos]->m_SimpLevelMax) 
		{
			if (SimpListPos < m_SimpListLength-1)
			{
				SimpListPos++;
				SimpLevel=m_pSimpList[SimpListPos]->m_SimpLevelMin;
			}
			else
				SimpLevel=m_pSimpList[SimpListPos]->m_SimpLevelMax;
		}		
}



void HConstantFrameRate::IncreaseDetailTemp()
{

  	if (!m_bDisableIncreaseTemp && (!m_bFullDetailTemp || m_bFirstIncrease))
	{
	
 
		if (m_bFirstIncrease)
		{
		 	
			SetHoopsSimplificationOptions(m_pSimpList[0]->m_SimpLevelMin,0);
			HC_Open_Segment_By_Key(m_pHView->GetOverwriteKey());
				HUtility::UnsetAttributes();
				HC_Set_Heuristics("culling = (maximum extent = 100, maximum extent mode = defer)");				
			HC_Close_Segment();
			m_bFirstIncrease = false;
			m_bFullDetailTemp = false;
			m_pHView->SetMaximumUpdateTime(0.35f);
		}
		else
			m_pHView->SetMaximumUpdateTime(0.15f);
 
		
		m_pHView->InternalUpdate();
		m_pHView->SetMaximumUpdateTime(0.0f);

		if (!m_pHView->GetUpdateInterrupted())
		{
			HC_Open_Segment_By_Key(m_pHView->GetOverwriteKey());
				if (HC_Show_Existence("heuristics"))
 					HC_UnSet_Heuristics();			
			HC_Control_Update_By_Key (m_pHView->GetViewKey(), "unset redraw");
 			HC_Close_Segment();
			m_bFullDetailTemp = true;
 			
		}

	}
}



#if 0


   	if (!m_bFullDetailTemp)
	{
		static int ovr_SimpListPos = -1;
		static int ovr_SimpLevel  = -1;		

		if (ovr_SimpListPos == -1)
		{		
			ovr_SimpListPos = m_SimpListPos;
			ovr_SimpLevel = m_SimpLevel;
			if (GetDetailLevelNum() > 10)
				m_TempDetailIncrement = GetDetailLevelNum() / 3;
			else
				m_TempDetailIncrement = 1;
		}
	
		if (HC_Update_Display_Timed(0.1f))

		while (1)
		{
			HC_Open_Segment_By_Key(m_pHView->GetViewKey());
 			HC_Set_Driver_Options("exit update = abort_window_event_checker");
			HC_Close_Segment();
 					
			SetHoopsSimplificationOptions(ovr_SimpLevel,ovr_SimpListPos);
 
			m_pHView->SetUpdateInterrupted(false);
 			HC_Show_Time(&starttime);					 
			float tupdatetime = 0.0f ;
 			if (ovr_SimpListPos == 0 && ovr_SimpLevel == m_pSimpList[0]->m_SimpLevelMin)
			{
				tupdatetime = m_pHView->GetMaximumUpdateTime();
				m_pHView->SetMaximumUpdateTime(0);

			}

 
			UpdateLevel(ovr_SimpLevel, ovr_SimpListPos);
 			
			if (ovr_SimpListPos == 0 && ovr_SimpLevel == m_pSimpList[0]->m_SimpLevelMin)
			{
 				m_pHView->SetMaximumUpdateTime(tupdatetime);

			}

 			HC_Show_Time(&endtime);
			HC_Open_Segment_By_Key(m_pHView->GetViewKey());
			HC_Set_Driver_Options ("no exit update");	
			HC_Close_Segment();

			if (m_pHView->GetEventCheckerCallback())
			{
 			    if (m_pHView->GetUpdateInterrupted() == 1)
				{				
					ovr_SimpListPos = -1;
					ovr_SimpLevel  = -1;
  			    	break;
				}
			}		
		
			if (endtime - starttime == 0.0f)
				fr = 1.0;
			else
				fr =(1.0f / (endtime - starttime));
			if (fr+m_DeadZone > m_TargetFrameRate && m_SimplificationIncreaseOnIdle != NoDetailChange)
			{
				m_SimpLevel   = ovr_SimpLevel;
				m_SimpListPos = ovr_SimpListPos;
 			
			}
  			if (ovr_SimpListPos == 0 && ovr_SimpLevel == m_pSimpList[0]->m_SimpLevelMin)
			{	
				m_bFullDetailTemp = true;
				float tupdatetime = m_pHView->GetMaximumUpdateTime();
				m_pHView->SetMaximumUpdateTime(0);
				SetHoopsSimplificationOptions(ovr_SimpLevel,ovr_SimpListPos);
 				UpdateLevel(ovr_SimpLevel, ovr_SimpListPos);
				m_pHView->SetMaximumUpdateTime(tupdatetime);
				m_bIncreaseDetailTemp = false;
				ovr_SimpListPos = -1;
				ovr_SimpLevel  = -1;		
				return;
			}

			ChangeDetail(-m_TempDetailIncrement, &ovr_SimpLevel, &ovr_SimpListPos);
 			break;
		}
		
	}
	else
	{
		m_pHView->SetShadowRegenerationMode(true);	    
	}


//	m_bIncreaseDetailTemp = false;
	}
}
#endif

void HConstantFrameRate::Watch()
{
	if (ViewpointHasChanged(true) || ((m_ActivityType == GeneralActivity) && m_bWatchActivity) || m_pHView->GetModel()->GetBhvBehaviorManager()->IsPlaying())
	{
 		bool bWasFullDetail = m_bFullDetailTemp;
 		if (m_ActivityType == MovementActivity)
		{
			UpdateLevel(m_SimpLevel, m_SimpListPos);
			m_ActivityType = NoActivity;
			return;
		}
		m_bFirstIncrease = true;
		float lowtarget = 1.0f / (m_TargetFrameRate * 0.5f);
		if (m_bAdjustNextTime)
		{
			AdjustDetail(m_AdjustTime);	
			m_bAdjustNextTime  = false;
		}

		m_ActivityType = NoActivity;
		if (m_SimpLevel != 0  || m_SimpListPos!=0)
		    m_bFullDetailTemp = false;
	
		SetHoopsSimplificationOptions(m_SimpLevel, m_SimpListPos);
	
		HC_Open_Segment_By_Key(m_pHView->GetViewKey());
		HC_Set_Driver_Options("display interrupted update = off");
		HC_Close_Segment();
		HC_Control_Update_By_Key(m_pHView->GetViewKey(), "clear stats");
		if (!IsLowestDetail())
			m_pHView->SetMaximumUpdateTime(lowtarget);

		float starttime,endtime;
		HC_Show_Time(&starttime);
		UpdateLevel(m_SimpLevel, m_SimpListPos);
		HC_Show_Time(&endtime);
		m_pHView->SetMaximumUpdateTime(0.0f);
		HC_Open_Segment_By_Key(m_pHView->GetViewKey());
		HC_Set_Driver_Options("display interrupted update = on");
		HC_Close_Segment();

		int segmentsdrawn = 0;
		if (m_pHView->GetUpdateInterrupted())
		    m_bFullDetailTemp = false;
	
  		if (endtime - starttime < 0.0001f)
		{		
			char buf[4096];
			HC_Open_Segment_By_Key(m_pHView->GetViewKey());
			HC_Show_Device_Info(".", "segments", buf);
			HC_Close_Segment();
			segmentsdrawn = atoi(buf);
		}
 		
 		if (endtime - starttime  > 0.0001f || segmentsdrawn > 0)
		{		

			if ((!bWasFullDetail || endtime-starttime >=0.4) && m_pHView->GetUpdateInterrupted())
			{			
				MajorDetailDecrease();
 				SetHoopsSimplificationOptions(m_SimpLevel, m_SimpListPos);
				UpdateLevel(m_SimpLevel, m_SimpListPos);
			}
			else
			{
				if (endtime - starttime < 0.02 && GetDetailIncMode() == AllowDetailChange)
	 				ChangeDetail(-3);
				else
				{				
					m_bAdjustNextTime = true;
					if (!bWasFullDetail || endtime-starttime >=0.4)
						m_AdjustTime = (1.0f / (endtime-starttime +0.0001f));
					else
						m_AdjustTime = m_TargetFrameRate;

				}
			}		
		}
	}
	else
	{	
#if 0
		if (m_bFullDetailTemp)
		{
			HC_Open_Segment_By_Key(m_pHView->GetOverwriteKey());
			if (HC_Show_Existence("heuristics"))
				HC_UnSet_Heuristics();
			HC_Close_Segment();
			UpdateLevel(m_SimpLevel, m_SimpListPos);
		}
#endif
		HC_Open_Segment_By_Key(m_pHView->GetOverwriteKey());
		char text[4096];
		if (!m_bFirstIncrease && HC_Show_Existence("heuristics"))
		{
			HC_Show_Heuristics(text);
			HC_UnSet_Heuristics();
		}
		HC_Close_Segment();
		UpdateLevel(m_SimpLevel, m_SimpListPos);
		m_bAdjustNextTime = false;
	}
}




bool HConstantFrameRate::ViewpointHasChanged(bool update)
{
  	float pos[11]; 
	HC_Open_Segment_By_Key(m_pHView->GetSceneKey());
		HC_Show_Net_Camera_Target (&pos[0], &pos[1], &pos[2]);
		HC_Show_Net_Camera_Position (&pos[3], &pos[4], &pos[5]);
		HC_Show_Net_Camera_Up_Vector (&pos[6], &pos[7], &pos[8]);
		HC_Show_Net_Camera_Field (&pos[9], &pos[10]);
	HC_Close_Segment();
	
 	for (int i=0;i<11;i++)
	{
		if (m_oldPos[i]!=pos[i])
		{
			if (update)
			{
			for (int j=0;j<11;j++)
				m_oldPos[j]=pos[j];	 
			}
			return true;
		}
	} 	
	return false;
}

void HFrNoSimpAA::Set(HConstantFrameRate *pCfr, int SimpLevel)
{
    switch (SimpLevel)
    {
		case 1:
			if (strcmp(pCfr->GetView()->GetDriverType(), "direct3d") != 0)
				HC_Set_Rendering_Options("transparency = style=screen door");
 		break;
		case 0:
 		break;
    }
	
}

void HFrNoSimpAA::Update(HConstantFrameRate *pCfr, int SimpLevel)
{
	if (SimpLevel == 1)
		pCfr->GetView()->InternalUpdate();
	else
	{
		pCfr->GetView()->InitAntialiasing();
		pCfr->GetView()->GetAntialiasing()->Show();
		pCfr->GetView()->InternalUpdate();
		pCfr->GetView()->GetAntialiasing()->Hide();
	}
}


void HConstFRSimpType::Update(HConstantFrameRate *pCfr, int SimpLevel)
{
	UNREFERENCED(SimpLevel);
	// 调用BaseView更新函数
    pCfr->GetView()->InternalUpdate();
}


void HFrNoSimp::Set(HConstantFrameRate *pCfr, int SimpLevel)
{

	switch (SimpLevel)
	{
	case 2:
 		HC_Set_Visibility("shadows = off");	
	//	HC_Open_Segment_By_Key(pCfr->GetView()->GetSceneKey());
	//	HC_Set_Rendering_Options("simple shadow = off");
	//	HC_Close_Segment();
// 		HC_Set_Rendering_Options("transparency = style=screen door");

 	case 1:
//		HC_Set_Visibility("cut geometry = off");	
 		HC_Set_Rendering_Options("no simple reflection");			
 		pCfr->GetView()->SetShadowRegenerationMode(false);	

 	case 0:
		if (SimpLevel == 0)
		{
			if (pCfr->GetView()->GetAllowInteractiveShadows())
 				pCfr->GetView()->SetShadowRegenerationMode(true);	
		} 		
		break;
	}

}

void HFrLodClamp::Reset(HConstantFrameRate *pCfr)
{
	Set(pCfr, 0);
}

void HFrLodClamp::Set(HConstantFrameRate *pCfr, int SimpLevel)
{	 
	char cval[MVO_BUFFER_SIZE];

	if (m_bSegmentSwitch)
	{
		if (SimpLevel != 0)
			SimpLevel = SimpLevel;
		HC_KEY segkey;
		HC_Open_Segment_By_Key(pCfr->GetView()->GetModelKey());
		HC_Open_Segment("fr_lods");
		HC_Begin_Contents_Search(".", "segments");
		while (HUtility::FindContents(segkey))
		{
			char name[MVO_BUFFER_SIZE];
			char temp[MVO_BUFFER_SIZE];
		    HC_Show_Segment(segkey, name);
		    HC_Parse_String(name, "/", -1, temp);
			HC_Open_Segment_By_Key(segkey);
			if (atoi(temp) == SimpLevel)
			{
 					if (HC_Show_Existence("visibility"))
						HC_UnSet_Visibility();
			}
			else
				HC_Set_Visibility("off");
			HC_Close_Segment();
		}
		HC_End_Contents_Search();
		HC_Close_Segment();
		HC_Close_Segment();
	}
	else
	{

	 //	sprintf(cval, "lod, lodo =(clamp=%d)", SimpLevel);
	//	HC_Set_Rendering_Options(cval);	 	
	 
		sprintf(cval, "lod, lodo =(clamp=%d, fallback = coarsest)", SimpLevel);
		HC_Set_Rendering_Options(cval);	 		 
			
		if (strcmp(pCfr->GetView()->GetDriverType(), "direct3d") != 0)
			HC_Set_Rendering_Options("transparency = style=screen door");
 		HC_Set_Rendering_Options ("tessellation=(cylinder=(24,12,6,4,2))");
	}
}
 

static void SetThreshold (int level)
{
	switch (level)
	{
	case 0:
		HC_Set_Rendering_Options("lod, lodo =(no clamp, threshold = (0.4%, 0.1%, 0.005%, 0.004%, 0.003%, 0.002%, 0.001%, 0%), fallback = bounding none)");	 		 
		break;
	case 1:
		HC_Set_Rendering_Options("lod, lodo =(no clamp, threshold = (1%, 0.5%, 0.25%, 0.1%, 0.05%, 0.01%, 0.005%, 0%), fallback = bounding none)");	 		 
		break;
	case 2:
		HC_Set_Rendering_Options("lod, lodo =(no clamp, threshold = (3%, 1%, 0.5%, 0.2%, 0.1%, 0.05%, 0.01%, 0%), fallback = bounding none)");	 		 
		break;
	case 3:
		HC_Set_Rendering_Options("lod, lodo =(no clamp, threshold = (7%, 3%, 1%, 0.5%, 0.2%, 0.1%, 0.05%, 0%), fallback = bounding none)");	 		 
		break;
	case 4:
		HC_Set_Rendering_Options("lod, lodo =(no clamp, threshold = (10%, 8%, 5%,1%, 0.7%, 0.2%, 0.1%, 0%),fallback = bounding none)");	 		 
		break;
	case 5:
		HC_Set_Rendering_Options("lod, lodo =(no clamp, threshold = (10%, 8%, 5%,2%, 1%, 0.5%, 0.3%, 0%),fallback = bounding none)");	 		 
		break;
	case 6:
		HC_Set_Rendering_Options("lod, lodo =(no clamp, threshold = (10%, 8%, 5%,1%, 0.7%, 0.2%, 0.1%, 0%),fallback = bounding none)");	 		 
		break;		 		
	}
}

void HFrLodThreshold::Set(HConstantFrameRate *pCfr, int SimpLevel)
{	 
	
	if (strcmp(pCfr->GetView()->GetDriverType(), "direct3d") != 0)
		HC_Set_Rendering_Options("transparency = style=screen door");
 	HC_Set_Rendering_Options ("tessellation=(cylinder=(24,12,6,4,2))");

	
	SetThreshold(SimpLevel);
//	if (SimpLevel>0)
//	    HC_Set_Heuristics("culling");

	
	
}

void HFrSolidBBox::Set(HConstantFrameRate *pCfr, int SimpLevel)
{	 
 	if (SimpLevel == 0)
	{
	    HC_Set_Rendering_Options("lod, lodo = (clamp = 7, fallback = bounding)");
	}
	else
	{
		HImSetCallback("draw segment tree","HFrSolidBBox_draw_solid_bbx_tree");	
		HImSetCallback("draw 3d geometry","HImUtility_suppress_3d_geometry");
		HC_Set_Visibility("lines = on, edges = off, faces = on, markers = off");				  
	}	
  				
 	if (strcmp(pCfr->GetView()->GetDriverType(), "direct3d") != 0)
		HC_Set_Rendering_Options("transparency = style=screen door");
 
}



// retrieves the bounding value for all segments and display them as a polyline
void HFrSolidBBox::draw_solid_bbx_tree(HIC_Rendition *rendition, HIC_Segment_Info * geo3)
{

    HPoint min,max;
    float points[16][3];
//  char segment[128];
    char type[128];
//  long keytype;

    HC_KEY key=HIC_Show_Segment_Key(geo3);

    //this code checks if a segment contains shell or mesh geometry to avoid drawing
    //of unnecessary bounding boxes
//  HC_Show_Segment(key,segment);
//  HC_Begin_Contents_Search(segment,"shells,mesh");
//  if (HC_Find_Contents(type,&keytype))

    HC_Show_Bounding_Info_By_Key(key, type);
    if (strcmp("", type) != 0)
    {

    HC_Show_Bounding_Cuboid_By_Key(key,&min,&max);

    points[0][0] = min.x; points[0][1] = min.y; points[0][2] = min.z;
    points[1][0] = max.x; points[1][1] = min.y; points[1][2] = min.z;
    points[2][0] = max.x; points[2][1] = max.y; points[2][2] = min.z;
    points[3][0] = min.x; points[3][1] = max.y; points[3][2] = min.z;
    points[4][0] = min.x; points[4][1] = max.y; points[4][2] = max.z;
    points[5][0] = max.x; points[5][1] = max.y; points[5][2] = max.z;
    points[6][0] = max.x; points[6][1] = min.y; points[6][2] = max.z;
    points[7][0] = min.x; points[7][1] = min.y; points[7][2] = max.z;
    points[8][0] = min.x; points[8][1] = max.y; points[8][2] = max.z;
    points[9][0] = min.x; points[9][1] = max.y; points[9][2] = min.z;
    points[10][0] = min.x; points[10][1] = min.y; points[10][2] = min.z;
    points[11][0] = min.x; points[11][1] = min.y; points[11][2] = max.z;
    points[12][0] = max.x; points[12][1] = min.y; points[12][2] = max.z;
    points[13][0] = max.x; points[13][1] = min.y; points[13][2] = min.z;
    points[14][0] = max.x; points[14][1] = max.y; points[14][2] = min.z;
    points[15][0] = max.x; points[15][1] = max.y; points[15][2] = max.z;

    HIC_Polyline const *pl = HIC_New_Polyline(16 , (HIC_Point *)points);
    HIC_Draw_3D_Polyline(rendition , pl);
    HIC_Free_Geometry(pl);
    }
    HIC_Draw_Segment(rendition,geo3);

}




//retrieves the bounding value for all segments and display them as shell
void HFrSolidBBox::draw_solid_bbx (HIC_Rendition *rendition, HIC_Segment_Info * geo3)
{
    HPoint min, max;
    float points[4][3];
    HC_KEY key = HIC_Show_Segment_Key(geo3);
    HIC_Polygon const *pgon;

    if (HC_Show_Existence_By_Key(key, "shells, meshes, lines, polygons, cylinders, polycylinders, first match"))
    {
        HC_Show_Bounding_Cuboid_By_Key(key, &min, &max);

        //front
        points[0][0] = min.x; points[0][1] = min.y; points[0][2] = min.z;
        points[1][0] = max.x; points[1][1] = min.y; points[1][2] = min.z;
        points[2][0] = max.x; points[2][1] = max.y; points[2][2] = min.z;
        points[3][0] = min.x; points[3][1] = max.y; points[3][2] = min.z;
        pgon = HIC_New_Polygon(4, (HIC_Point *)points);
        HIC_Draw_3D_Polygon(rendition, pgon);
        HIC_Free_Geometry(pgon);

        //back
        points[0][2] = max.z;
        points[1][2] = max.z;
        points[2][2] = max.z;
        points[3][2] = max.z;
        pgon = HIC_New_Polygon(4, (HIC_Point *)points);
        HIC_Draw_3D_Polygon(rendition, pgon);
        HIC_Free_Geometry(pgon);

        points[0][0] = min.x; points[0][1] = min.y; points[0][2] = min.z;
        points[1][0] = min.x; points[1][1] = min.y; points[1][2] = max.z;
        points[2][0] = min.x; points[2][1] = max.y; points[2][2] = max.z;
        points[3][0] = min.x; points[3][1] = max.y; points[3][2] = min.z;
        pgon = HIC_New_Polygon(4, (HIC_Point *)points);
        HIC_Draw_3D_Polygon(rendition, pgon);
        HIC_Free_Geometry(pgon);

        points[0][0] = max.x;
        points[1][0] = max.x;
        points[2][0] = max.x;
        points[3][0] = max.x;
        pgon = HIC_New_Polygon(4, (HIC_Point *)points);
        HIC_Draw_3D_Polygon(rendition, pgon);
        HIC_Free_Geometry(pgon);

        points[0][0] = min.x; points[0][1] = min.y; points[0][2] = min.z;
        points[1][0] = min.x; points[1][1] = min.y; points[1][2] = max.z;
        points[2][0] = max.x; points[2][1] = min.y; points[2][2] = max.z;
        points[3][0] = max.x; points[3][1] = min.y; points[3][2] = min.z;
        pgon = HIC_New_Polygon(4, (HIC_Point *)points);
        HIC_Draw_3D_Polygon(rendition, pgon);
        HIC_Free_Geometry(pgon);

        points[0][1] = max.y;
        points[1][1] = max.y;
        points[2][1] = max.y;
        points[3][1] = max.y;
        pgon = HIC_New_Polygon(4, (HIC_Point *)points);
        HIC_Draw_3D_Polygon(rendition, pgon);
        HIC_Free_Geometry(pgon);
    }
    HIC_Draw_Segment(rendition, geo3);
}



void HFrWireframeBBox::Set(HConstantFrameRate *pCfr,int SimpLevel)
{
	
 	if (SimpLevel == 0)
	{
		HImUnSetOneCallback ("draw segment tree");
		HImSetCallback("draw segment","HFrWireframeBBox_draw_wireframe_bbx");
	}
	else
	{
		HImSetCallback("draw segment tree","HFrWireframeBBox_draw_wireframe_bbx");
		HImUnSetOneCallback ("draw segment");
	}
	
	HImSetCallback("draw 3d geometry","HImUtility_suppress_3d_geometry");
	HC_Set_Visibility("lines = on, edges = off, faces = off, markers = off");
		 	
//	HC_Open_Segment_By_Key(pCfr->GetView()->GetViewKey());
	HC_UnSet_One_Rendering_Option("lod");
	HC_Set_Heuristics("no hidden surfaces");
	if (strcmp(pCfr->GetView()->GetDriverType(), "direct3d") != 0)
		HC_Set_Rendering_Options("transparency = style=screen door");
//	HC_Close_Segment();	
}


// retrieves the bounding value for all segments and display them as a polyline
void HFrWireframeBBox::draw_wireframe_bbx(HIC_Rendition   *rendition,HIC_Segment_Info * geo3)
{
    HPoint min,max;
    float points[16][3];
//  char segment[128];
    char type[128];
//  long keytype;

    HC_KEY key=HIC_Show_Segment_Key(geo3);

    //this code checks if a segment contains shell or mesh geometry to avoid drawing
    //of unnecessary bounding boxes
//  HC_Show_Segment(key,segment);
//  HC_Begin_Contents_Search(segment,"shells,mesh");
//  if (HC_Find_Contents(type,&keytype))

    HC_Show_Bounding_Info_By_Key(key, type);

    if (strcmp("", type) != 0)
    {
        HC_Show_Bounding_Cuboid_By_Key(key,&min,&max);

        points[0][0] = min.x; points[0][1] = min.y; points[0][2] = min.z;
        points[1][0] = max.x; points[1][1] = min.y; points[1][2] = min.z;
        points[2][0] = max.x; points[2][1] = max.y; points[2][2] = min.z;
        points[3][0] = min.x; points[3][1] = max.y; points[3][2] = min.z;
        points[4][0] = min.x; points[4][1] = max.y; points[4][2] = max.z;
        points[5][0] = max.x; points[5][1] = max.y; points[5][2] = max.z;
        points[6][0] = max.x; points[6][1] = min.y; points[6][2] = max.z;
        points[7][0] = min.x; points[7][1] = min.y; points[7][2] = max.z;
        points[8][0] = min.x; points[8][1] = max.y; points[8][2] = max.z;
        points[9][0] = min.x; points[9][1] = max.y; points[9][2] = min.z;
        points[10][0] = min.x; points[10][1] = min.y; points[10][2] = min.z;
        points[11][0] = min.x; points[11][1] = min.y; points[11][2] = max.z;
        points[12][0] = max.x; points[12][1] = min.y; points[12][2] = max.z;
        points[13][0] = max.x; points[13][1] = min.y; points[13][2] = min.z;
        points[14][0] = max.x; points[14][1] = max.y; points[14][2] = min.z;
        points[15][0] = max.x; points[15][1] = max.y; points[15][2] = max.z;

        HIC_Polyline const *pl = HIC_New_Polyline(16 , (HIC_Point *)points);
        HIC_Draw_3D_Polyline(rendition , pl);
        HIC_Free_Geometry(pl);
    }

    HIC_Draw_Segment(rendition,geo3);
}





void HFrWireframe::Set(HConstantFrameRate *pCfr, int SimpLevel)
{
	
	HC_Set_Visibility("lines = off, edges = on, faces = off, markers = off");
	HImUnSetOneCallback ("draw segment");
	HImUnSetOneCallback ("draw 3d geometry");
	
	HC_Set_Heuristics("no hidden surfaces");
	if (strcmp(pCfr->GetView()->GetDriverType(), "direct3d") != 0)
		HC_Set_Rendering_Options("transparency = style=screen door");
	
	if (SimpLevel == 1)
		pCfr->AdjustLodOptions();
	else
		HC_UnSet_One_Rendering_Option("lod");
	
}


void HFrExtent::Set(HConstantFrameRate *pCfr, int SimpLevel)
{	 
	UNREFERENCED(pCfr);

	char text[4096];
	int ss = (SimpLevel) * m_Multiplier;
	if (SimpLevel == 0)
		ss = m_Multiplier/2;
	sprintf(text, "culling = (maximum extent = %d)", ss);
	HC_Set_Heuristics(text);	
	if (m_bUseLOD)
	{
		int delta = (m_SimpLevelMax - this->m_SimpLevelMin) / 7;
		SetThreshold(delta);
	}
}

HConstFRSimpType * HConstantFrameRate::GetSimpType(ConstFRType stype)
{
	for (int i=0;i<m_SimpListLength;i++)
	{
		if (m_pSimpList[i]->GetType() == stype)
			return m_pSimpList[i];
	}
	return 0;
}
