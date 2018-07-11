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
#include <stdlib.h>
#include <stdio.h>

#include "HBaseModel.h"
#include "HBaseView.h"
#include "HBaseOperator.h"
#include "HEventInfo.h"
#include "HBhvTimeline.h"
#include "HBhvAnimation.h"
#include "HSelectionSet.h"
#include "HDB.h"
#include "HUtilityXMLParser.h"
#include "HBhvBehaviorManager.h"


 

HBhvTimeline::HBhvTimeline(HBhvAnimation *animation)
{
	m_pAnimation = animation;
	m_pTimelineArray.SetCount(0);
	m_pInstancedTimeline = 0; 
 	m_CurrentTick = 0;
	m_StartTick = 0;
	m_CurrentRelativeTick = 0;
}

 
void HBhvTimeline::SetTimeline(const int *timeline, int length)
{
//	const int i = m_pTimelineArray2[2];
 	m_pTimelineArray.SetCount(0);
 
	m_pTimelineArray.AppendArray(timeline, length);
}

int HBhvTimeline::CheckKeyframe(int t)
{
	for (int i=0;i<GetTimelineArrayLength();i++)
	{
		if (m_pTimelineArray[i] == t)
			return i;
	}
	return -1;
}


int HBhvTimeline::AddKeyframeInternal(int t, bool *doesExist)
{
	if (doesExist)
 		*doesExist = false;

	// Need to check that this binary search works in all cases,
	// not just when keyframes are already sorted
	int low;
	int high;
	int mid = 0;
	int index = t;
	int orig_array_len = GetTimelineArrayLength();

	low = 0;
	high = orig_array_len -1;
	if (high>=0)
	{	

		if (index <= m_pTimelineArray[high] && 
				index>= m_pTimelineArray[low]) {
			mid = low + (high - low)/2;

			while (high >= low) {
				int midindex = m_pTimelineArray[mid];

				if (midindex == index)	 {
					if (doesExist)
						*doesExist = true;
					//return mid;
					break;
				}
				else if (midindex > index)
					high = mid - 1;
				else
					low = mid + 1;
				mid = low + (high - low)/2;
			}
		}
		else if (index>= m_pTimelineArray[high])
			mid = high + 1;
		else if (index<= m_pTimelineArray[low])
			mid = low;
	}
	else if (orig_array_len == 0){
		mid = 0;
	}

	if(doesExist && *doesExist)
		m_pTimelineArray.ReplaceAt(t, mid);
	else
		m_pTimelineArray.InsertAt(t, mid);

	return mid;
}

int HBhvTimeline::AddKeyframe(int t, bool &doesExist)
{
	return AddKeyframeInternal(t, &doesExist); 
}
int HBhvTimeline::AddKeyframe(int t)
{
	return AddKeyframeInternal(t, 0); 
}

	

int HBhvTimeline::DeleteKeyframe(int time)
{
	for (int i=0;i<GetTimelineArrayLength();i++)
	{
		if (m_pTimelineArray[i] == time)
		{
 				m_pTimelineArray.RemoveAt(i);
				return i;
		}
	}
	return -1;
}

void HBhvTimeline::Adjust(int time, int delta, bool doall)
{
	for (int i=0;i<GetTimelineArrayLength();i++)
	{
		if (m_pTimelineArray[i] >= time)
		{
		    if (doall)
		    {		    
		    for (int j=i;j<GetTimelineArrayLength();j++)		    		    
			m_pTimelineArray[j]+=delta;
		    }
		    else
			m_pTimelineArray[i]+=delta;
		    break;
		}
		
	}
}

void HBhvTimeline::AdjustRelative(int time, int delta)
{
 	for (int i=1;i<GetTimelineArrayLength();i++)
	{
		if (m_pTimelineArray[i] >= time)
		{
 		     Adjust(m_pTimelineArray[i],delta);

		    for (int j=i+1;j<GetTimelineArrayLength();j++)
		    {		   
			     //int d= m_pTimelineArray[j] - m_pTimelineArray[j-1];
			     //int delta = percent/100.0f * (float)d;
			     Adjust(m_pTimelineArray[j],delta);
		    }
		    break;
		}
		
	}
}


void HBhvTimeline::Serialize(HUtilityXMLGenerator *xmlgen)
{
 		HUtilityXMLTag xmlt;	 
		xmlt.SetTagname("Timeline"); 
		int *tarray = GetTimelineArray();

		for (int i=0; i< GetTimelineArrayLength(); i++)
		{
			char text[MVO_BUFFER_SIZE];
			sprintf(text, "%d", tarray[i]);
			xmlt.AddListItem(text);
		}
   		xmlgen->AddTag(&xmlt);
}


void *HBhvTimeline::XMLCallback(HUtilityXMLTag *xt, bool open, void *m_pExtraData)
{ 	
	UNREFERENCED(m_pExtraData);

	if (open)
	{
 		HBhvAnimation *current_animation = (HBhvAnimation *)xt->GetXMLParser()->GetStackData();
		HBhvTimeline *itp = new HBhvTimeline(current_animation);

		xt->StartListItemsQuery();
 		while (1)
		{
 			char *litem;
  			if (!xt->GetNextListItem(&litem))
				break;
			bool replace;
			itp->AddKeyframe(atoi(litem), replace);
		}

	
   		current_animation->SetTimeline(itp);
	}
	return 0;
}

HBhvTimeline * HBhvTimeline::CreateInstance(HBhvAnimation *ainst) 
{ 
	
	HBhvTimeline *i = new HBhvTimeline(ainst);
	i->SetInstancedTimeline((HBhvTimeline *)this);
	return i;
}

 

int HBhvTimeline::GetLastTick()
{
	if (m_pTimelineArray.Count())
		return m_pTimelineArray[m_pTimelineArray.Count()-1];
	else return -1;
}

 


float  HBhvTimeline::AdjustTickToTimeline(float frame)
{
	int loop = GetAnimation()->GetLoop();

	int *timeline = GetTimelineArray();
	int length = GetTimelineArrayLength();

	frame = frame - m_StartTick;

	if (frame> timeline[length-1] && loop)
	{
		int frac = (int)(frame / timeline[length-1]);
		frame = frame - (frac * timeline[length-1]);
	}
	return frame;
}


bool HBhvTimeline::Evaluate(float currentFrame, int &interval, float &fraction)
{
	int i;
//	int loop = GetAnimation()->GetLoop();
	int *timeline = GetTimelineArray();
	int length = GetTimelineArrayLength();

 	float relativeFrame = AdjustTickToTimeline(currentFrame);
	m_CurrentTick = currentFrame;


	int low;
	int high;
	int mid = 0;

	low = 0;
	high = length-1;

	if (relativeFrame<timeline[0])
		return false;
	// Need to check that this binary search works in all cases,
	// not just when the keyframes are already sorted
	if (relativeFrame <= (float)timeline[high] && 
			relativeFrame >= (float)timeline[low]) {
		mid = low + (high - low)/2;

		while (high >= low) {
			float midindex = (float)timeline[mid];

			if (midindex == relativeFrame)	 {
				i = mid;
				break;
			}
			else if (midindex > relativeFrame)
				high = mid - 1;
			else
				low = mid + 1;
			mid = low + (high - low)/2;
		}
		i = mid;
	}
	else if (relativeFrame < timeline[low]){
		i = low+1;
	}
	else if (relativeFrame > timeline[high]){
		i = length;
	}
	else
		i = 0;
	
	

	if ( i == 0 ){
		if (length >= 2) 
			i = 1;
		else
			return false;
	}
	

	if (i == length)
	{
		interval = length;
		fraction = 1.0f;
		m_CurrentTick = static_cast<float>(timeline[length -1]);
		return true;
	}

	float delta1 = (float)(timeline[i] - timeline[i-1]);

	fraction = 	(relativeFrame-timeline[i-1]) / delta1;
	interval = i - 1;
	m_CurrentRelativeTick = relativeFrame;
	return true;
}

bool HBhvTimeline::GetTimelineInterval(int currentTime, int &interval)
{
	int *timeline = GetTimelineArray();
	int length = GetTimelineArrayLength();

	for (int i=0;i<length;i++)
	{
		if (currentTime>=timeline[i])
		{
			if (i>=length-1)
			{
				if (currentTime == timeline[i])
				{
					interval = i;
					return true;
				}
				else
					return false;
			}
			else
			{
				if (currentTime<timeline[i+1])
				{
					interval = i;
					return true;
				}
			}
		}
	}
	return false;
}

 



