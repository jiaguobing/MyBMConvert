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
#include <stdio.h>
#include <stdlib.h>

#include "HBaseModel.h"
#include "HBaseView.h"
#include "HBhvInterpolator.h"
#include "HBhvTimeline.h"
#include "HBhvUtility.h"
#include "HBhvBehaviorManager.h"
#include "HBhvAnimation.h"
#include "HUtilityXMLParser.h"
#include "HUtilityTextParser.h"
#include "HUtility.h"
#include "HEventManager.h"



HBhvInterpolator::HBhvInterpolator(HBhvAnimation *animation, const char *name) 
{
	m_pTarget = INVALID_KEY;
	m_pAnimation = animation;
	if (name)
		strcpy(m_Name, name);
	else
		strcpy(m_Name, "");
}


HBhvInterpolator::~HBhvInterpolator()
{
	for (int i=0;i<GetArrayLength();i++)
		delete m_pArray[i];
}

void HBhvInterpolator::SetInstancedInterpolator(HBhvInterpolator *interpolator)
{
	m_pInterpolatorInstance = interpolator;
}


void HBhvInterpolator::Duplicate(int pos, bool rep, bool next)
{
	if ((next && pos>=GetArrayLength()) || (!next && pos<=0))
		return;
	int pos2;
	if (next)
		pos2 = pos;
	else
		pos2 = pos-1;
	HKeyframe * kp = m_pArray[pos2]->Clone();
	if (rep)
		Replace(kp, pos);
	else
		Insert(kp, pos);
}


void HBhvInterpolator::Copy(int pos, int adpos, bool replace)
{
	HKeyframe * kp = m_pArray[pos]->Clone();
	if (replace)
		Replace(kp, adpos);
	else
		Insert(kp, adpos);
}


void HBhvInterpolator::AddRotationToMatrix(float new_rot[16])
{
	HPoint pivot;
	float translation_matrix[16], pivot_translation_matrix[16], pivot_translation_matrix_inverse[16];
	float result[16];

	HC_Open_Segment_By_Key(m_pTarget);{
		HC_Set_Rendering_Options("simple shadow = auto");

		GetAnimation()->GetTarget()->GetPivot(pivot);
		HPoint op;
		GetTranslationFromMatrix(op);
		HC_Compute_Translation_Matrix(op.x, op.y, op.z, translation_matrix);
		HC_Compute_Translation_Matrix(-pivot.x,-pivot.y,-pivot.z, pivot_translation_matrix);
		HC_Compute_Matrix_Inverse(pivot_translation_matrix, pivot_translation_matrix_inverse);

		float const * scale_matrix = m_pAnimation->GetTargetScaleMatrix();

		HC_Compute_Matrix_Product(pivot_translation_matrix_inverse, scale_matrix, result);
		HC_Compute_Matrix_Product(result, new_rot, result);
		HC_Compute_Matrix_Product(result, pivot_translation_matrix, result);
		HC_Compute_Matrix_Product(result, translation_matrix, result);

		HC_Set_Modelling_Matrix(result);
	}HC_Close_Segment();
}


void HBhvInterpolator::AddPositionToMatrix(HPoint & new_pos)
{
	float segment_modelling_matrix[16], translation_matrix[16], result[16];
	float pivot_translation_matrix[16], pivot_translation_matrix_inverse[16];
	HPoint pivot;
	HC_Open_Segment_By_Key(m_pTarget);{
		HBhvUtility::ShowModellingMatrix(segment_modelling_matrix);
		GetAnimation()->GetTarget()->GetPivot(pivot);
		HC_Compute_Translation_Matrix(-pivot.x,-pivot.y,-pivot.z, pivot_translation_matrix);
		HC_Compute_Matrix_Inverse(pivot_translation_matrix, pivot_translation_matrix_inverse);
		segment_modelling_matrix[12] = segment_modelling_matrix[13] = segment_modelling_matrix[14] = 0;
		HC_Compute_Translation_Matrix(new_pos.x, new_pos.y, new_pos.z, translation_matrix);
		HC_Compute_Matrix_Product(pivot_translation_matrix_inverse, segment_modelling_matrix, result);
		HC_Compute_Matrix_Product(result, pivot_translation_matrix, result);
		HC_Compute_Matrix_Product(result, translation_matrix, result);
		HC_Set_Modelling_Matrix(result);
	}HC_Close_Segment();
}


void HBhvInterpolator::AddScaleToMatrix(HPoint & new_scale)
{
	float segment_modelling_matrix[16], translation_matrix[16], result[16];
	float pivot_translation_matrix[16], pivot_translation_matrix_inverse[16], scalematrix[16];
	HPoint pivot;
	HC_Open_Segment_By_Key(m_pTarget);{
		HBhvUtility::ShowModellingMatrix(segment_modelling_matrix);
		GetAnimation()->GetTarget()->GetPivot(pivot);
		HC_Compute_Translation_Matrix(-pivot.x,-pivot.y,-pivot.z, pivot_translation_matrix);
		HC_Compute_Matrix_Inverse(pivot_translation_matrix, pivot_translation_matrix_inverse);
		HC_Compute_Scale_Matrix(new_scale.x, new_scale.y, new_scale.z, scalematrix);
		segment_modelling_matrix[12] = segment_modelling_matrix[13] = segment_modelling_matrix[14] = 0;
		HC_Compute_Normalized_Vector(&segment_modelling_matrix[0], &segment_modelling_matrix[0]);
		HC_Compute_Normalized_Vector(&segment_modelling_matrix[4], &segment_modelling_matrix[4]);
		HC_Compute_Normalized_Vector(&segment_modelling_matrix[8], &segment_modelling_matrix[8]);
		HPoint op;
		GetTranslationFromMatrix(op);
		HC_Compute_Translation_Matrix(op.x, op.y, op.z, translation_matrix);
		HC_Compute_Matrix_Product(pivot_translation_matrix_inverse, segment_modelling_matrix, result);
		HC_Compute_Matrix_Product(result, scalematrix, result);
		HC_Compute_Matrix_Product(result, pivot_translation_matrix, result);
		HC_Compute_Matrix_Product(result, translation_matrix, result);
		HC_Set_Modelling_Matrix(result);
	}HC_Close_Segment();
}


void HBhvInterpolator::GetTranslationFromMatrix(HPoint &t)
{
	HPoint pivot;
	HC_Open_Segment_By_Key(m_pTarget);{
		float segment_modelling_matrix[16];
		GetAnimation()->GetTarget()->GetPivot(pivot);
		HBhvUtility::ShowModellingMatrix(segment_modelling_matrix);
		HPoint op(-pivot.x, -pivot.y, -pivot.z);
		HC_Compute_Transformed_Points(1, &op, segment_modelling_matrix, &op);
		t = op + pivot;
	}HC_Close_Segment();
}


void HBhvInterpolator::GetRotationFromMatrix(HQuat &r)
{
	HC_Open_Segment_By_Key(m_pTarget);{
		float segment_modelling_matrix[16];
		HBhvUtility::ShowModellingMatrix(segment_modelling_matrix);
		r = HQuat::MatrixToQuaternion(segment_modelling_matrix);
	}HC_Close_Segment();
}


void HBhvInterpolator::SetTarget()
{
	m_pTarget = GetAnimation()->GetTarget()->GetTargetKey();
}


HBhvInterpolatorPosition::HBhvInterpolatorPosition(HBhvAnimation *animation, const char *name) : HBhvInterpolator(animation, name)
{
}


HBhvInterpolatorTrail::HBhvInterpolatorTrail(HBhvAnimation *animation, const char *name) : HBhvInterpolatorPosition(animation, name)
{
	m_TrailType = Forward;
	strcpy(m_Style,"----");
	strcpy(m_Color,"lines = white");
 	m_Weight = 1;
	m_TrailSegment = INVALID_KEY;
	m_lastkeyframe = 0;
	m_initialized = false;

 
	HC_Open_Segment_By_Key(GetAnimation()->GetBehaviorManager()->GetActiveView()->GetModelKey());
		HC_Open_Segment("trail");
		
			/* set up for the line */
			m_TrailSegment = HC_KOpen_Segment("");	
				//HC_Set_Color(m_Color);
				//HC_Set_Line_Weight(m_Weight);
				//HC_Set_Line_Pattern(m_Style);
 				HC_Set_Rendering_Options("mask transform=model");
	  			HC_Set_Visibility("lines = on");	
				HC_Set_Heuristics("exclude bounding");
			HC_Close_Segment();

			

		HC_Close_Segment();
	HC_Close_Segment();
	

}

void HBhvInterpolatorTrail::Init(){

	int interval = 0;
	float fraction = 0.0f;

	HC_Open_Segment_By_Key(m_TrailSegment);
	
		/* create the entire trail at the start.  we will then
		use rollback ink (with either positive or negative values)
		to show more or less of the trail */
		HC_Restart_Ink();
		HC_Set_Color(m_Color);
		HC_Set_Line_Weight(m_Weight);
		HC_Set_Line_Pattern(m_Style);
		HC_Set_Rendering_Options("mask transform=model");
  		HC_Set_Visibility("lines = on");	
		HC_Set_Heuristics("exclude bounding");

		for (int i = 0; i < (int)GetAnimation()->GetTimeline()->GetLastTick(); i++){
			if (GetAnimation()->GetTimeline()->Evaluate((float)i, interval, fraction)){					
				HBhvTargetObject *target = GetAnimation()->GetTarget();
				HPoint pivot, pos;

				CalculatePos(interval, fraction, pos);	
				
				if(target){
					target->GetPivot(pivot);
					pos.Add(-pivot.x, -pivot.y, - pivot.z);
				}

				
				m_trailKey = HC_KInsert_Ink(pos.x,pos.y,pos.z);	
			}
		}

		/* now rollback to the very start of the line, and turn on visibility */
		HC_Rollback_Ink( (int)GetAnimation()->GetTimeline()->GetLastTick() );
		
		HC_Set_Visibility("lines = on");	
	HC_Close_Segment();

	m_initialized = true;
}

const char * HBhvInterpolatorPosition::GetType() { return "Pos"; } 
const char * HBhvInterpolatorTrail::GetType() { return "Trail"; } 

void HBhvInterpolatorPosition::Serialize(HUtilityXMLGenerator *xmlgen)
{
 		HUtilityXMLTag xmlt;
		char tagname[MVO_BUFFER_SIZE];
		strcpy(tagname, GetType());
		strcat(tagname,"Interpolator");
		xmlt.SetTagname(tagname);
		xmlt.AddProperty("Name", m_Name, true);

 		for (int i=0; i< GetArrayLength(); i++)
			((HKeyframeChannel *)m_pArray[i])->Serialize(&xmlt);

   		xmlgen->AddTag(&xmlt);

}

void HBhvInterpolatorTrail::Serialize(HUtilityXMLGenerator *xmlgen)
{
 		HUtilityXMLTag xmlt;
		char tagname[MVO_BUFFER_SIZE];
		strcpy(tagname, GetType());
		strcat(tagname,"Interpolator");
		xmlt.SetTagname(tagname);
		xmlt.AddProperty("Name", m_Name, true);

 		xmlt.AddProperty("Color", m_Color, true);
 		xmlt.AddPropertyInt("Weight", m_Weight);
		xmlt.AddProperty("Style", m_Style, true);
		switch (m_TrailType)
		{
		case Forward:
			xmlt.AddProperty("Type", "forward", true);
			break;
		case Backward:
			xmlt.AddProperty("Type", "backward", true);
			break;
		case Full:
			xmlt.AddProperty("Type", "full", true);
		break;

		}

 		for (int i=0; i< GetArrayLength(); i++)
			((HKeyframeChannel *)m_pArray[i])->Serialize(&xmlt);

   		xmlgen->AddTag(&xmlt);

}


static bool FindModifier(const char *text, const char *modifier, char *result, bool skip_bracket = true)
{
	static delimiter_t find_items_td[] = {{"[", true, "]", true, true} ,
											{"\x41-\x7a", false, " |]", true, false}, 
											{"\x2a", false, " |]", true, false}, 
											{"\x30-\x39|-|", false, " |]", true, false},
											{"\"", true, "\"", true, true}};
	static delimiter_t find_end_td[] = {{"]", true, 0, true, true}};


	if (!result)
		return strstr(text, modifier) != 0;

 	HUtilityTextParser tp;
 	tp.InsertBuffer(text, (unsigned int)strlen(text));
	if (text[0] == '[' && skip_bracket)
		tp.SkipAhead(1);
	while (1)
	{
	 	int dlum = -1;
 		bool res = tp.ParseBuffer(result, (int &)dlum, find_items_td, 4);
		if (dlum == 1 || dlum == 2)
		{
			if (strcmp(modifier, result) == 0)
				return true;
		}
		else if (dlum == 0)
		{
			HUtilityTextParser tp;
		 	tp.InsertBuffer(result, (unsigned int)strlen(result));
			char mdf[MVO_SMALL_BUFFER_SIZE];
 	 		tp.ParseBuffer(mdf, (int &)dlum, find_items_td, 4);
			tp.ParseBuffer(result, (int &)dlum, find_end_td, 1);
			if (strcmp(modifier, mdf) == 0)
				return true;
		}
		else
			return false;
		if (!res)
			return false;
	}
}

static void FindData(char *text, char *result)
{
	static delimiter_t find_items_td[] = {{"[", true, "]", true, true},
											{"\x41-\x7a", false, " |]", true, false}, 
											{"\x30-\x39|-|", false, "]", true, false},
											{"\"", true, "\"", true, true}};
  	int dlum;
	HUtilityTextParser tp;
 	tp.InsertBuffer(text, (unsigned int)strlen(text));
	if (text[0] == '[')
		tp.SkipAhead(1);
	while (1)
	{
 		tp.ParseBuffer(result, (int &)dlum, find_items_td, 4);
		if (dlum == 2)
			break;

	}
}

static void FindStringData(char *text, char *result)
{
	static delimiter_t find_items_td[] = {{"\"", true, "\"", true, false}};
	
  	int dlum;
	HUtilityTextParser tp;
 	tp.InsertBuffer(text, (unsigned int)strlen(text));
	if (text[0] == '[')
		tp.SkipAhead(1);
	tp.ParseBuffer(result, (int &)dlum, find_items_td, 1, false);			

}



void *HBhvInterpolatorPosition::XMLCallback(HUtilityXMLTag *xt, bool open, void *m_pExtraData)
{
	UNREFERENCED(m_pExtraData);
	
 	char Name[MVO_SMALL_BUFFER_SIZE];
 	
 	if (open)
	{
		HBhvAnimation *current_animation = (HBhvAnimation *)xt->GetXMLParser()->GetStackData();

		if (!xt->GetProperty("Name", Name))
			strcpy(Name, "");
		HBhvInterpolatorPosition *itp = new HBhvInterpolatorPosition(current_animation, Name);

		xt->StartListItemsQuery();
		int counter = 0;
	
		while (1)
		{
			HUtilityTextParser tp;
			HANIChannelType channelType = HANILinear;
			bool HoldConstant = false;	
			bool easeinout = false;
			char *litem;
			char result[MVO_BUFFER_SIZE];
  			HPoint pos(0,0,0);
			bool followpathlinear = false;
 			if (!xt->GetNextListItem(&litem))
				break;
			if (FindModifier(litem, "l", 0))
					channelType = HANILinear;
			if (FindModifier(litem, "b", 0))
					channelType = HANIHermiteSpline;		 
			if (FindModifier(litem, "d", 0))
					channelType = HANIDiscrete;		 
			if (FindModifier(litem, "ea", 0))
					easeinout = true;		 
			if (FindModifier(litem, "f", 0))
					channelType = HANIFollowPath;
			if (FindModifier(litem, "fl", 0))
			{
					channelType = HANIFollowPath;
					followpathlinear = true;
			}
			if (FindModifier(litem, "*", 0))
			{
				HoldConstant = true;
				channelType = HANIHermiteSpline;		 
			}
 			else
			{
				FindData(litem, result);
				HUtilityXMLParser::GetFloatPoint(result, pos);
			}

 			if (channelType == HANIFollowPath)
			{
				HKeyframeChannelFollowPath * fp = new HKeyframeChannelFollowPath; 
				fp->m_bLinear = followpathlinear;
				fp->m_cp = pos; 
				itp->m_pArray.InsertAt(fp, counter);
 			}
 			else if (channelType == HANILinear)
				itp->InsertLinear(pos, counter);
			else if (channelType == HANIHermiteSpline)
				itp->InsertCurve(pos, counter);
			else if (channelType == HANIDiscrete)
				itp->InsertDiscrete(pos, counter);
			itp->m_pArray[counter]->SetConstant(HoldConstant);
			itp->m_pArray[counter]->SetEaseInOut(easeinout);
			counter++;

		}

   		current_animation->AddInterpolator(itp);

 
	}
	return 0;

}

void *HBhvInterpolatorTrail::XMLCallback(HUtilityXMLTag *xt, bool open, void *m_pExtraData)
{
	UNREFERENCED(m_pExtraData);
	
 	char Name[MVO_SMALL_BUFFER_SIZE];
 	char trailtype[MVO_SMALL_BUFFER_SIZE];
 	char color[MVO_SMALL_BUFFER_SIZE];
 	char weight[MVO_SMALL_BUFFER_SIZE];
 	char style[MVO_SMALL_BUFFER_SIZE];
 	
 	if (open)
	{
		HBhvAnimation *current_animation = (HBhvAnimation *)xt->GetXMLParser()->GetStackData();

		if (!xt->GetProperty("Name", Name))
			strcpy(Name, "");
		if (!xt->GetProperty("Type", trailtype))
			strcpy(trailtype, "");
		if (!xt->GetProperty ("Color", color))
 			strcpy(color, "lines = white");
		if (!xt->GetProperty ("Weight", weight))
 			strcpy(weight, "1");
		if (!xt->GetProperty ("Style", style))
 			strcpy(style, "----");
	
		
		HBhvInterpolatorTrail *itp = new HBhvInterpolatorTrail(current_animation, Name);
		if (strcmp(trailtype,"forward") == 0)
			itp->m_TrailType = Forward;
		else if (strcmp(trailtype,"backward") == 0)
			itp->m_TrailType = Backward;
		else if (strcmp(trailtype,"full") == 0)
			itp->m_TrailType = Full;
 		itp->m_Weight = atoi(weight);
		strcpy(itp->m_Style, style);
		strcpy(itp->m_Color, color);

		xt->StartListItemsQuery();
		int counter = 0;
	
		while (1)
		{
			HUtilityTextParser tp;
			HANIChannelType channelType = HANILinear;
			bool HoldConstant = false;	
			bool easeinout = false;
			char *litem;
			char result[MVO_BUFFER_SIZE];
  			HPoint pos(0,0,0);
 			if (!xt->GetNextListItem(&litem))
				break;
			if (FindModifier(litem, "l", 0))
					channelType = HANILinear;
			if (FindModifier(litem, "f", 0))
					channelType = HANIFollowPath;
			if (FindModifier(litem, "fl", 0))
					channelType = HANIFollowPath;
			if (FindModifier(litem, "b", 0))
					channelType = HANIHermiteSpline;		 
			if (FindModifier(litem, "d", 0))
					channelType = HANIDiscrete;		 
			if (FindModifier(litem, "ea", 0))
					easeinout = true;		 

			if (FindModifier(litem, "*", result))
			{
				HoldConstant = true;
				channelType = HANIHermiteSpline;		 
			}
 			else
			{
				FindData(litem, result);
				HUtilityXMLParser::GetFloatPoint(result, pos);
			}

 			if (channelType == HANILinear)
				itp->InsertLinear(pos, counter);
			else if (channelType == HANIHermiteSpline)
				itp->InsertCurve(pos, counter);
			else if (channelType == HANIDiscrete)
				itp->InsertDiscrete(pos, counter);
			itp->m_pArray[counter]->SetConstant(HoldConstant);
			itp->m_pArray[counter]->SetEaseInOut(easeinout);
			counter++;

		}

   		current_animation->AddInterpolator(itp);

 
	}
	return 0;

} 		

  

HBhvInterpolator * HBhvInterpolatorPosition::CreateInstance(HBhvAnimation *ainst) 
{ 
	
	HBhvInterpolatorPosition *i = new HBhvInterpolatorPosition(ainst);
	i->SetInstancedInterpolator((HBhvInterpolator *)this);
	return i;
}

HBhvInterpolator * HBhvInterpolatorTrail::CreateInstance(HBhvAnimation *ainst) 
{ 
	
	HBhvInterpolatorTrail *i = new HBhvInterpolatorTrail(ainst);
	i->SetInstancedInterpolator((HBhvInterpolator *)this);
	return i;
}




void HBhvInterpolatorPosition::CalculatePos(int keyframe, float fraction,  HPoint &res)
{
	 HKeyframeChannelCurve **splinearray = (HKeyframeChannelCurve **)GetArray();

 	int length = GetArrayLength();

	if (!GetAnimation()->GetCurrentlyRunning() && splinearray[0]->m_bConstant)		
 			GetTranslationFromMatrix(splinearray[0]->m_cp);		
 
 
 	if (keyframe >= length-1)
		res = CURVE_ARRAY(length-1)->m_cp;
	else 
	{
	    if (CURVE_ARRAY(keyframe)->m_bEaseInOut)
		fraction = HUtility::EaseInEaseOut(fraction, 0.49f, 0.02f, 0.49f);	

 		CURVE_ARRAY(keyframe)->Interpolate((HKeyframeChannel **)splinearray, keyframe, fraction, length, res);
	}
}

	

void HBhvInterpolatorPosition::CalculateAllTangents()
{
	
	HKeyframeChannelCurve **splinearray = (HKeyframeChannelCurve **)GetArray();
	int length = GetArrayLength();
	for (int i=0;i<length-1;i++)
	{
		if (((HKeyframeChannel *)splinearray[i])->m_channeltype == HANIFollowPath || 
			((HKeyframeChannel *)splinearray[i])->m_channeltype == HANIBezierSpline || 
			((HKeyframeChannel *)splinearray[i])->m_channeltype == HANIHermiteSpline)
		{
			
			HPoint p0, p1, p2, p3;
			p1 = splinearray[i]->m_cp;
			p2 = splinearray[i+1]->m_cp;
			if (i>0)
			{
			    if (splinearray[i-1]->m_channeltype == HANILinear)
				p0 = p1;
			    else
				p0 = splinearray[i - 1]->m_cp;
			}
			else
				p0 = p1;
			if (i<length-2)
			{
    				if (splinearray[i+1]->m_channeltype == HANILinear)
				    p3 = p2;
				else
				    p3 = splinearray[i + 2]->m_cp;
			}
			else
				p3 = p2;
			splinearray[i]->CalculateCurveFactor(GetAnimation()->GetTimeline(), i);
			splinearray[i]->CalculateHermiteTangents(p0, p2, p3);
		}
	}
}

static void GetPosititonFromMatrix(HPoint &pos, HPoint pivot)
{
    float matrix[16];

 		HUtility::ShowModellingMatrix(matrix);
		HPoint p, p2;
		p.Set(matrix[12], matrix[13], matrix[14]);
		p.Set(0,0,0);
		p = pivot;
		HC_Compute_Transformed_Points(1, &p, matrix, &p2);
		pos.Set(p2.x - p.x, p2.y -  p.y, p2.z - p.z);
}



void HBhvInterpolatorPosition::Evaluate(int keyframe, float fraction, bool &hasPos, HPoint &pos, bool &hasQuat, HQuat &quat, bool &hasScale, HPoint &scale) 
{	
	UNREFERENCED(hasQuat);
	UNREFERENCED(quat);
	UNREFERENCED(hasScale);
	UNREFERENCED(scale);

 	hasPos = true;
 	CalculatePos(keyframe, fraction, pos);


	if (GetAnimation()->GetTarget()->GetCameraType() != NoCamera)
	{
		if ( m_pTarget == INVALID_KEY)
			SetTarget();
			if (GetAnimation()->GetTarget()->GetCameraType() == CameraTargetFree || GetAnimation()->GetTarget()->GetCameraType() == CameraPositionFree || GetAnimation()->GetTarget()->GetCameraType() == CameraPositionTarget)
			    InterpolateCamera2(pos);
			else
			    InterpolateCamera(pos);
	}
}

void HBhvInterpolatorPosition::Interpolate(int keyframe, float fraction)
{
	HPoint res;
	if ( m_pTarget == INVALID_KEY)
		SetTarget();
	
	
	CalculatePos(keyframe, fraction, res);
	
	if (GetAnimation()->GetTarget()->GetCameraType() != NoCamera)
	{
			if (!GetAnimation()->GetBehaviorManager()->GetUpdateCamera())
				return;
			if (GetAnimation()->GetTarget()->GetCameraType() == CameraTargetFree || GetAnimation()->GetTarget()->GetCameraType() == CameraPositionFree || GetAnimation()->GetTarget()->GetCameraType() == CameraPositionTarget)
			    InterpolateCamera2(res);
			else
			    InterpolateCamera(res);
	}
	else
	{
		HBaseView *view = GetAnimation()->GetBehaviorManager()->GetActiveView();

		HC_Open_Segment_By_Key(m_pTarget);
		HPoint befpos,piv;
 		GetAnimation()->GetTarget()->GetPivot(piv);

		GetPosititonFromMatrix(befpos, piv);
		HLISTENER_EVENT(HObjectManipulationListener, view->GetEventManager(), ObjectBeforeMoveEvent(view, m_pTarget, &befpos));

		HC_Set_Rendering_Options("simple shadow = auto");
		HKeyframeChannel **splinearray = (HKeyframeChannel **)GetArray();
		int length = GetArrayLength();
		
		if (keyframe < length-1 && splinearray[keyframe]->m_channeltype == HANIFollowPath)
		{

			if (((HKeyframeChannelFollowPath *)splinearray[keyframe])->m_bNoRotate)
 				AddPositionToMatrix(res);
			else
			{
 				float matrix[16];
				HC_Compute_Identity_Matrix(matrix);			
				matrix[12] = res.x;
				matrix[13] = res.y;
				matrix[14] = res.z;
				HC_Set_Modelling_Matrix(matrix);		
				AddRotationToMatrix(((HKeyframeChannelFollowPath *)splinearray[keyframe])->tmatrix);			
			}
		}
		else
 			AddPositionToMatrix(res);
		
		HLISTENER_EVENT(HObjectManipulationListener, view->GetEventManager(), ObjectMovedEvent(view, m_pTarget, &res));
  		HC_Close_Segment();
 		GetAnimation()->GetTarget()->FlagForCollision();

		 
	}
	GetAnimation()->GetBehaviorManager()->GetActiveView()->SetGeometryChanged();

}
 
void HBhvInterpolatorTrail::Reset()
{
	if (m_TrailType == Backward || m_TrailType == Full)
		;
//		Interpolate(-1,0);
	else
	{
		if (m_TrailSegment != INVALID_KEY)
		{		
			
			HC_Open_Segment_By_Key(m_TrailSegment);
			HC_Open_Segment("temp");
			//HC_Flush_Contents(".", "everything");
			HC_Close_Segment();
			HC_Close_Segment();
		}
	}
}

HBhvInterpolatorTrail::~HBhvInterpolatorTrail()
{
		if (m_TrailSegment != INVALID_KEY)
		{		
			HC_Delete_By_Key(m_TrailSegment);			
		}	

}
void HBhvInterpolatorTrail::Interpolate(int keyframe, float fraction)
{
	UNREFERENCED(keyframe);
	UNREFERENCED(fraction);

	int currenttick = (int)GetAnimation()->GetTimeline()->GetCurrentTick();
	int trail_size = -1;

	if(!m_initialized)
		Init();

	HC_Open_Segment_By_Key(m_TrailSegment);
		HC_Show_Polyline_Count(m_trailKey, &trail_size);
		HC_Rollback_Ink(trail_size - currenttick);
	HC_Close_Segment();

}

void HBhvInterpolatorPosition::InterpolateCamera2(HPoint &trans, bool simulate)
{
    
	float segment_modelling_matrix[16], translation_matrix[16], result[16];
 	HPoint pivot;
 	HC_Open_Segment_By_Key(m_pTarget);
		if (GetAnimation()->GetTarget()->GetCameraType() == CameraPositionFree || GetAnimation()->GetTarget()->GetCameraType() == CameraPositionTarget)				 
 		    GetAnimation()->GetBehaviorManager()->GetPositionMatrix(segment_modelling_matrix);		    		
		else
		    GetAnimation()->GetBehaviorManager()->GetTargetMatrix(segment_modelling_matrix);

   		segment_modelling_matrix[12] = segment_modelling_matrix[13] = segment_modelling_matrix[14] = 0;
  		HC_Compute_Translation_Matrix(trans.x, trans.y, trans.z, translation_matrix);
   		HC_Compute_Matrix_Product(segment_modelling_matrix, translation_matrix, result);
		
		if (GetAnimation()->GetTarget()->GetCameraType() == CameraPositionFree || GetAnimation()->GetTarget()->GetCameraType() == CameraPositionTarget)				 
		{		
		    HPoint pos, up, tar;
		    pos.Set(0,0,0);
		    up.Set(0,1,0);
		    if (GetAnimation()->GetTarget()->GetCameraType() == CameraPositionTarget)
		    {
			HPoint p,t, delta;
			HC_Show_Net_Camera_Position(&p.x, &p.y, &p.z);
			HC_Show_Net_Camera_Target(&t.x, &t.y, &t.z);
			delta.Set(t.x - p.x, t.y - p.y, t.z - p.z);
			tar.Set(0.0f,0.0f,(float)-HC_Compute_Vector_Length(&delta));			
			HC_Compute_Transformed_Points(1, &tar, result, &tar);
			HC_Set_Camera_Target(tar.x, tar.y, tar.z);

		    }
		    
		    HC_Compute_Transformed_Points(1, &pos, result, &pos);
		    HC_Compute_Transformed_Points(1, &up, result, &up);	
		    //up.Add(-pos.x, - pos.y, -pos.z);
		    //HC_Compute_Normalized_Vector(&up,&up);
		    
		    HC_Set_Camera_Position(pos.x, pos.y, pos.z);
		    //HC_Set_Camera_Up_Vector(up.x, up.y, up.z);
		    HC_Compute_Normalized_Vector(&up,&up);
		    GetAnimation()->GetBehaviorManager()->SetPositionMatrix(result);


		}
		else
		{		
		   HPoint pos, up;
		    pos.Set(0,0,0);
		    
		    HC_Compute_Transformed_Points(1, &pos, result, &pos);
		    HC_Compute_Transformed_Points(1, &up, result, &up);	
 		    
		    HC_Set_Camera_Target(pos.x, pos.y, pos.z);
		    GetAnimation()->GetBehaviorManager()->SetTargetMatrix(result);

		}

	HC_Close_Segment();		

	if (!simulate)
	{
		if (GetAnimation()->GetBehaviorManager()->GetActiveView())
			GetAnimation()->GetBehaviorManager()->GetActiveView()->CameraPositionChanged(true, true);
		GetAnimation()->GetBehaviorManager()->CameraUpdated();
	}

}




void HBhvInterpolatorPosition::InterpolateCamera(HPoint &pos, bool simulate)
{
	if (GetAnimation()->GetTarget()->GetCameraType() == CameraPosition)
	{	  

		HPoint op, delta, ot;
		HC_Open_Segment_By_Key(m_pTarget);
		HC_Show_Net_Camera_Position(&op.x, &op.y, &op.z);
		delta.Set(pos.x - op.x, pos.y - op.y, pos.z - op.z);	
		HC_Show_Net_Camera_Target(&ot.x, &ot.y, &ot.z);
		ot.Add(delta.x, delta.y, delta.z);
		HC_Set_Camera_Target(ot.x, ot.y, ot.z);
		HC_Set_Camera_Position(pos.x, pos.y, pos.z);
		HC_Close_Segment();
	}
	else
	{
		HPoint op, delta, ot;
		HC_Open_Segment_By_Key(m_pTarget);
		HC_Show_Net_Camera_Target(&op.x, &op.y, &op.z);
		delta.Set(pos.x - op.x, pos.y - op.y, pos.z - op.z);	
		HC_Show_Net_Camera_Position(&ot.x, &ot.y, &ot.z);
		ot.Add(delta.x, delta.y, delta.z);
		HC_Set_Camera_Target(pos.x, pos.y, pos.z);
		HC_Set_Camera_Position(ot.x, ot.y, ot.z);
		HC_Close_Segment();
	}		
	
	if (!simulate)
	{
		if (GetAnimation()->GetBehaviorManager()->GetActiveView())
			GetAnimation()->GetBehaviorManager()->GetActiveView()->CameraPositionChanged(true, true);
		GetAnimation()->GetBehaviorManager()->CameraUpdated();
	}


}


void HBhvInterpolatorPosition::Reset()
{
	CalculateAllTangents();
}


HBhvInterpolatorAxisRotate::HBhvInterpolatorAxisRotate(HBhvAnimation *animation, const char *name) : HBhvInterpolator(animation, name)
{
	m_axis.Set(0,1,0);
}

const char * HBhvInterpolatorAxisRotate::GetType() { return "AxisRot"; } 


void HBhvInterpolatorAxisRotate::Serialize(HUtilityXMLGenerator *xmlgen)
{
 		HUtilityXMLTag xmlt;
		char tagname[MVO_BUFFER_SIZE];
		strcpy(tagname, GetType());
		strcat(tagname,"Interpolator");
		xmlt.SetTagname(tagname);
		xmlt.AddProperty("Name", m_Name, true); 
		char text[MVO_BUFFER_SIZE];
		HCLOCALE(sprintf(text, "[v %f %f %f]", m_axis.x, m_axis.y, m_axis.z));
		xmlt.AddProperty("Modifiers", text, true); 

		HKeyframeAxisRotation **array = (HKeyframeAxisRotation **)GetArray();
		for (int i=0; i< GetArrayLength(); i++)
			array[i]->Serialize(&xmlt);

   		xmlgen->AddTag(&xmlt);

}



void *HBhvInterpolatorAxisRotate::XMLCallback(HUtilityXMLTag *xt, bool open, void *m_pExtraData)
{
	UNREFERENCED(m_pExtraData);

	char Name[MVO_SMALL_BUFFER_SIZE];
	char Modifiers[MVO_BUFFER_SIZE];
	char result[MVO_BUFFER_SIZE];

 	if (open)
	{

		HBhvAnimation *current_animation = (HBhvAnimation *)xt->GetXMLParser()->GetStackData();
		if (!xt->GetProperty("Name", Name))
			strcpy(Name, "");
		
		HBhvInterpolatorAxisRotate *itp = new HBhvInterpolatorAxisRotate(current_animation, Name);

		if (!xt->GetProperty("Modifiers", Modifiers))
			strcpy(Modifiers, "");

		if (FindModifier(Modifiers, "v", result, false))
		{
			HPoint axis;
			HUtilityXMLParser::GetFloatPoint(result,axis);
			itp->SetAxis(axis.x, axis.y, axis.z);
		}
		else
			itp->SetAxis(0,0,1);

		xt->StartListItemsQuery();
		int counter = 0;
		float angle;
		while (1)
		{
			HUtilityTextParser tp;
			char *litem;
   			if (!xt->GetNextListItem(&litem))
				break;

 			FindData(litem, result);
			HCLOCALE(angle = (float)atof(result));
			itp->Insert(angle, counter++);
		}

	
  		current_animation->AddInterpolator(itp);

	}
	return 0;

}

 



 
HBhvInterpolator * HBhvInterpolatorAxisRotate::CreateInstance(HBhvAnimation *ainst) 
{ 
	
	HBhvInterpolatorAxisRotate *i = new HBhvInterpolatorAxisRotate(ainst);
	i->SetInstancedInterpolator((HBhvInterpolator *)this);
	return i;

}

 void HBhvInterpolatorAxisRotate::Evaluate(int keyframe, float fraction, bool &hasPos, HPoint &pos, bool &hasQuat, HQuat &quat, bool &hasScale, HPoint &scale) 
{
	UNREFERENCED(hasPos);
	UNREFERENCED(pos);
	UNREFERENCED(hasQuat);
	UNREFERENCED(quat);
	UNREFERENCED(hasScale);
	UNREFERENCED(scale);

	hasQuat = true;

	HKeyframeAxisRotation **anglearray  = (HKeyframeAxisRotation **)GetArray();	
 	int length = GetArrayLength();
	float res;
	float delta;

	if (keyframe >= length-1)
		res = anglearray[length-1]->m_angle;
	else 
	{
		delta = anglearray[keyframe+1]->m_angle - anglearray[keyframe]->m_angle;
 		res = anglearray[keyframe]->m_angle + delta * fraction;
	}


 	HPoint rotaxis;
	float matrix[16];
 	GetAxis(rotaxis);
  	HC_Compute_Offaxis_Rotation(rotaxis.x, rotaxis.y, rotaxis.z, res, matrix); 
	quat.MatrixToQuaternion(matrix);
	

}


void HBhvInterpolatorAxisRotate::Interpolate(int keyframe, float fraction)
{

	HKeyframeAxisRotation **anglearray  = (HKeyframeAxisRotation **)GetArray();	
 	int length = GetArrayLength();

	float res;
	float delta;

	if ( m_pTarget == INVALID_KEY)
		SetTarget();

	if (keyframe >= length-1)
		res = anglearray[length-1]->m_angle;
	else 
	{
		delta = anglearray[keyframe+1]->m_angle - anglearray[keyframe]->m_angle;
 		res = anglearray[keyframe]->m_angle + delta * fraction;
	}


 	HPoint rotaxis;
	float matrix[16];
 	GetAxis(rotaxis);
  	HC_Compute_Offaxis_Rotation(rotaxis.x, rotaxis.y, rotaxis.z, res, matrix); 
    
	AddRotationToMatrix(matrix);
 	GetAnimation()->GetTarget()->FlagForCollision();
	GetAnimation()->GetBehaviorManager()->GetActiveView()->SetGeometryChanged();
	
	
}



  
HBhvInterpolatorColor::HBhvInterpolatorColor(HBhvAnimation *animation, const char *name) : HBhvInterpolator(animation, name)
{
 	SetGeomType("everything");
 	SetColorComponent("diffuse");
}

const char * HBhvInterpolatorColor::GetType() { return "Color"; } 


 
void HBhvInterpolatorColor::Serialize(HUtilityXMLGenerator *xmlgen)
{
 		HUtilityXMLTag xmlt;
		char tagname[MVO_BUFFER_SIZE];
		strcpy(tagname, GetType());
		strcat(tagname,"Interpolator");
		xmlt.SetTagname(tagname);
		xmlt.AddProperty("Name", m_Name, true);
		xmlt.AddProperty("Type", m_ColorComponent, true);
		xmlt.AddProperty("GeomType", m_GeomType, true);

		HKeyframeChannel **array = (HKeyframeChannel **)GetArray();
		for (int i=0; i< GetArrayLength(); i++)
			array[i]->Serialize(&xmlt);

   		xmlgen->AddTag(&xmlt);

}

									
	
void * HBhvInterpolatorColor::XMLCallback(HUtilityXMLTag * xt, bool open, void * m_pExtraData)
{
	UNREFERENCED(m_pExtraData);

	char Name[MVO_SMALL_BUFFER_SIZE];	
	static delimiter_t find_items_td[] = {{"l|t|b|d|c|", true, 0, true, false},
											{"\x30-\x39|-|", false, " |]", true, false}};
	if (open)
	{
		HBhvAnimation *current_animation = (HBhvAnimation *)xt->GetXMLParser()->GetStackData();

		if (!xt->GetProperty("Name", Name))
			strcpy(Name, "");
		HBhvInterpolatorColor *itp = new HBhvInterpolatorColor(current_animation, Name);

		if (!xt->GetProperty("GeomType", itp->GetGeomType()))
			itp->SetGeomType("everything");
 		if (!xt->GetProperty("Type", itp->GetColorComponent()))
			itp->SetColorComponent("diffuse");
 
		xt->StartListItemsQuery();
		int counter = 0;
		while (1)
		{
			HUtilityTextParser tp;
			char *litem;
			char result[MVO_BUFFER_SIZE];
 			int dlum;
			HPoint pos;
 			if (!xt->GetNextListItem(&litem))
				break;
			tp.InsertBuffer(litem, (unsigned int)strlen(litem));
			assert(litem[0] == '[');
			tp.SkipAhead(1);
			tp.ParseBuffer(result, dlum, find_items_td, 2);
			if (dlum == 0)
			{
 				tp.ParseBuffer(result, dlum, find_items_td, 2);
			}
			HCLOCALE(pos.x = (float)atof(result));
			tp.ParseBuffer(result, dlum, find_items_td, 2);
			HCLOCALE(pos.y = (float)atof(result));
			tp.ParseBuffer(result, dlum, find_items_td, 2);
			HCLOCALE(pos.z = (float)atof(result));
 			itp->Insert(pos, counter++);
		}
   		current_animation->AddInterpolator(itp);

	}
	return 0;
}

void HBhvInterpolatorColor::SetGeomType (const char *geomtype)
{
	strcpy(m_GeomType, geomtype);
}

void HBhvInterpolatorColor::SetColorComponent (const char *colorcomponent)
{
	strcpy(m_ColorComponent, colorcomponent);
}

 


HBhvInterpolator * HBhvInterpolatorColor::CreateInstance(HBhvAnimation *ainst) 
{ 
	HBhvInterpolatorColor * i = new HBhvInterpolatorColor(ainst);
	i->SetInstancedInterpolator((HBhvInterpolator *)this);
	return i;
}
 

void HBhvInterpolatorColor::Interpolate(int keyframe, float fraction)
{
	static delimiter_t find_items_td[] = {{"\x41-\x7a", false, ",| ", true, false}};

	HKeyframeChannel **colorarray  = (HKeyframeChannel **)GetArray();	
	int length = GetArrayLength();

	HPoint res;
	if ( m_pTarget == INVALID_KEY)
		SetTarget();

	if (keyframe >= length-1)
		res = colorarray[length-1]->m_cp;
	else 
		colorarray[keyframe]->Interpolate((HKeyframeChannel **)colorarray, keyframe, fraction, length, res);

	HC_Open_Segment_By_Key(m_pTarget);{
		char col[2048];
		char geomtype[512];
		char colorcomponent[512];
		strcpy(geomtype, GetGeomType());
		strcpy(colorcomponent, GetColorComponent());

		HUtilityTextParser tp;
		tp.InsertBuffer(geomtype, (unsigned int)strlen(geomtype));
		while (1)
		{
			char result[MVO_BUFFER_SIZE];
			int dlum;
			bool cont = tp.ParseBuffer(result, dlum, find_items_td, 1);

			if (strcmp(colorcomponent, "all") == 0) {
				HCLOCALE(sprintf(col,"%s = (r=%f g=%f b=%f)",result, res.x, res.y, res.z));
			}
			else
				HCLOCALE(sprintf(col,"%s = (%s = (r=%f g=%f b=%f))",result, colorcomponent, res.x, res.y, res.z));
			HC_Set_Color(col);

			if (!cont)
				break;
		}

	}HC_Close_Segment();
}



static void GetCameraFromMatrix(float *matrix, HPoint *target, HPoint *up_vector)
{
//Target remains unchanged
//    HPoint view_vector;
//    view_vector.Set(position.x-target.x, position.y-target.y, position.z-target.z);
	target->x = matrix[8];
	target->y = matrix[9];
	target->z = matrix[10];


    up_vector->x = matrix[4];
    up_vector->y = matrix[5];
    up_vector->z = matrix[6];
}


/*


HBhvInterpolatorQuat::HBhvInterpolatorQuat(const char *name) : HBhvInterpolator(name)
{
 
}

const char * HBhvInterpolatorQuat::GetType() { return "QUATSLERP"; } 


 

HBhvInterpolatorInstance * HBhvInterpolatorQuat::CreateInstance(HBhvAnimationInstance *ainst) { return ((HBhvInterpolatorInstance *)new HBhvInterpolatorInstanceQuat(this, ainst)); }
 



void GetMatrixFromCamera(HPoint position, HPoint target, HPoint up_vector, float *matrix)
{
    HPoint view_vector, view_vector2;
    view_vector.Set(target.x-position.x, target.y-position.y, target.z-position.z);
    view_vector2 = view_vector;
 
    HC_Compute_Normalized_Vector(&view_vector, &view_vector);
    HC_Compute_Normalized_Vector(&up_vector, &up_vector);

    HPoint cross1;

    HC_Compute_Cross_Product(&up_vector, &view_vector, &cross1);

    matrix[0] = cross1.x;
    matrix[1] = cross1.y;
    matrix[2] = cross1.z;
    matrix[3] = 0.0f;

    HPoint cross2;

    HC_Compute_Cross_Product(&view_vector, &cross1, &cross2);

    matrix[4] = cross2.x;
    matrix[5] = cross2.y;
    matrix[6] = cross2.z;
    matrix[7] = 0.0f;

    matrix[8] = view_vector.x;
    matrix[9] = view_vector.y;
    matrix[10] = view_vector.z;
    matrix[11] = 0.0f;
    
    matrix[12] = position.x;
    matrix[13] = position.y;
    matrix[14] = position.z;
    matrix[15] = 1.0f;
}



void HBhvInterpolatorInstanceQuat::Interpolate(int keyframe, float fraction)
{
	HKeyframeQuatSlerp **quatarray  = (HKeyframeQuatSlerp **)((HBhvInterpolatorQuat *)m_pInterpolator)->GetArray();
  	int length = ((HBhvInterpolatorQuat *)m_pInterpolator)->GetArrayLength();

	if ( m_pTarget == INVALID_KEY)
		SetTarget();

	HQuat res;
 
	float q1[4];
	float q2[4];
	float q3[4];
	if (keyframe >= length-1)
	{
		res = quatarray[length-1]->m_quat;
		q3[0] = res.x;
		q3[1] = res.y;
		q3[2] = res.z;
		q3[3] = res.w;
	}
	else 
	{
	q1[0] = quatarray[keyframe]->m_quat.x;
	q1[1] = quatarray[keyframe]->m_quat.y;
	q1[2] = quatarray[keyframe]->m_quat.z;
	q1[3] = quatarray[keyframe]->m_quat.w;

	q2[0] = quatarray[keyframe+1]->m_quat.x;
	q2[1] = quatarray[keyframe+1]->m_quat.y;
	q2[2] = quatarray[keyframe+1]->m_quat.z;
	q2[3] = quatarray[keyframe+1]->m_quat.w;

	float dot1, dot2;

	    dot1 = ((q1[0] - q2[0])*(q1[0] - q2[0])
			+ (q1[1] - q2[1])*(q1[1] - q2[1])
			+ (q1[2] - q2[2])*(q1[2] - q2[2])
			+ (q1[3] - q2[3])*(q1[3] - q2[3])); 

	    dot2 = ((q1[0] + q2[0])*(q1[0] + q2[0])
			+ (q1[1] + q2[1])*(q1[1] + q2[1])
			+ (q1[2] + q2[2])*(q1[2] + q2[2])
			+ (q1[3] + q2[3])*(q1[3] + q2[3])); 

	    if (dot2 < dot1) {
		    q2[0] = -q2[0];
		    q2[1] = -q2[1];
		    q2[2] = -q2[2];
		    q2[3] = -q2[3];
		}
	HUtility::TransitionQuaternion(q1, q2, fraction, q3);
	}

	if (GetAnimation()->GetTarget()->GetCameraType() != NoCamera)
	{
		if (!GetAnimation()->GetAnimation()->GetBehaviorManager()->GetUpdateCamera())
			return;
		InterpolateCamera(q3);
	}
	else
	{

	HC_Open_Segment_By_Key(m_pTarget);	

	float matrix[16], matrix2[16], matrix3[16], matrix5[16];
 	if (HC_Show_Existence("modelling matrix"))
		HC_Show_Modelling_Matrix(matrix5);
	else
		HC_Compute_Identity_Matrix(matrix5);
	
		HC_Compute_Identity_Matrix(matrix2);
		matrix2[12] = matrix5[12];
		matrix2[13] = matrix5[13];
		matrix2[14] = matrix5[14];
	
	HUtility::QuaternionToMatrix(q3, matrix);
	HPoint pivot, pivot2;
	GetAnimation()->GetTarget()->GetPivot(pivot);
 //	pivot.Set(0,0,0);
	HC_Compute_Transformed_Points(1, &pivot, matrix, &pivot2);
 	matrix[12] = pivot2.x;
	matrix[13] = pivot2.y;
	matrix[14] = pivot2.z;

// 	HC_Compute_Matrix_Product(matrix, matrix2, matrix3);

	HC_Compute_Translation_Matrix(-pivot.x, -pivot.y, -pivot.z, matrix5);
 	HC_Compute_Matrix_Product(matrix, matrix5, matrix3);
 	HC_Compute_Matrix_Product(matrix3, matrix2, matrix);
 
 	HC_Set_Modelling_Matrix(matrix);

	HC_Close_Segment();
	}

}

void HBhvInterpolatorInstanceQuat::InterpolateCamera(float *quat)
{
		float matrix[16];
		HUtility::QuaternionToMatrix(quat, matrix);
		HPoint t,u, op, ot;
		GetCameraFromMatrix(matrix, &t,&u);

		HC_Open_Segment_By_Key(m_pTarget);
		HC_Show_Net_Camera_Position(&op.x, &op.y, &op.z);
		HC_Show_Net_Camera_Target(&ot.x, &ot.y, &ot.z);
		HPoint lv;
		lv.Set(ot.x - op.x, ot.y - op.y, ot.z - op.z);
		float length = (float)HC_Compute_Vector_Length(&lv);
		HPoint nt;
		nt.Set(ot.x - length * t.x, ot.y - length * t.y, ot.z - length * t.z);
		HC_Set_Camera_Position(nt.x, nt.y, nt.z);
		HC_Set_Camera_Up_Vector(u.x, u.y, u.z);
		HC_Close_Segment();
		GetAnimation()->GetBehaviorManager()->GetActiveView()->CameraPositionChanged(true, true);

}

*/




void HBhvInterpolatorScale::Reset()
{
	CalculateAllTangents();
}

void HBhvInterpolatorScale::CalculateAllTangents()
{
	
	HKeyframeChannelCurve **splinearray = (HKeyframeChannelCurve **)GetArray();
	int length = GetArrayLength();
	for (int i=0;i<length-1;i++)
	{
		if (((HKeyframeChannel *)splinearray[i])->m_channeltype == HANIHermiteSpline)
		{
			
			HPoint p0, p1, p2, p3;
			p1 = splinearray[i]->m_cp;
			p2 = splinearray[i+1]->m_cp;
			if (i>0)
				p0 = splinearray[i - 1]->m_cp;
			else
				p0 = p1;
			if (i<length-2)
				p3 = splinearray[i + 2]->m_cp;
			else
				p3 = p2;
			splinearray[i]->CalculateCurveFactor(GetAnimation()->GetTimeline(), i);
			splinearray[i]->CalculateHermiteTangents(p0, p2, p3);
		}
	}
}



HBhvInterpolatorScale::HBhvInterpolatorScale(HBhvAnimation *animation, const char *name) : HBhvInterpolator(animation, name)
{
 
}

const char * HBhvInterpolatorScale::GetType() { return "Scale"; } 


void HBhvInterpolatorScale::Serialize(HUtilityXMLGenerator *xmlgen)
{
 		HUtilityXMLTag xmlt;
		char tagname[MVO_BUFFER_SIZE];
		strcpy(tagname, GetType());
		strcat(tagname,"Interpolator");
		xmlt.SetTagname(tagname);
		xmlt.AddProperty("Name", m_Name, true);
  
		HKeyframeChannel **array = (HKeyframeChannel **)GetArray();
		for (int i=0; i< GetArrayLength(); i++)
			array[i]->Serialize(&xmlt);

   		xmlgen->AddTag(&xmlt);

}

void *HBhvInterpolatorScale::XMLCallback(HUtilityXMLTag *xt, bool open, void *m_pExtraData)
{
	UNREFERENCED(m_pExtraData);

	char Name[MVO_SMALL_BUFFER_SIZE];
 	
	static delimiter_t find_items_td[] = {{"l|t|b|d|c|", true, 0, true, false},
											{"\x30-\x39|-|", false, " |]", true, false}};

	if (open)
	{
		HBhvAnimation *current_animation = (HBhvAnimation *)xt->GetXMLParser()->GetStackData();

		if (!xt->GetProperty("Name", Name))
			strcpy(Name, "");
		HBhvInterpolatorScale *itp = new HBhvInterpolatorScale(current_animation, Name);

		xt->StartListItemsQuery();
		int counter = 0;
		while (1)
		{
			HUtilityTextParser tp;
			char *litem;
			char result[MVO_BUFFER_SIZE];
 			int dlum;
			HPoint pos;
 			if (!xt->GetNextListItem(&litem))
				break;
			tp.InsertBuffer(litem, (unsigned int)strlen(litem));
			assert(litem[0] == '[');
			tp.SkipAhead(1);
			tp.ParseBuffer(result, dlum, find_items_td, 2);
			if (dlum == 0)
			{
 				tp.ParseBuffer(result, dlum, find_items_td, 2);
			}
			HCLOCALE(pos.x = (float)atof(result));
			tp.ParseBuffer(result, dlum, find_items_td, 2);
			HCLOCALE(pos.y = (float)atof(result));
			tp.ParseBuffer(result, dlum, find_items_td, 2);
			HCLOCALE(pos.z = (float)atof(result));
 			itp->Insert(pos, counter++);

		}

   		current_animation->AddInterpolator(itp);

	}
	return 0;

}

HBhvInterpolator * HBhvInterpolatorScale::CreateInstance(HBhvAnimation *ainst) 
{ 
	
	HBhvInterpolatorScale *i = new HBhvInterpolatorScale(ainst);
	i->SetInstancedInterpolator((HBhvInterpolator *)this);
	return i;

}
 
void HBhvInterpolatorScale::CalculatePos(int keyframe, float fraction,  HPoint &res)
{
	 HKeyframeChannelCurve **splinearray = (HKeyframeChannelCurve **)GetArray();

 	int length = GetArrayLength();

	if (!GetAnimation()->GetCurrentlyRunning() && splinearray[0]->m_bConstant)		
 			GetTranslationFromMatrix(splinearray[0]->m_cp);		
	
	if (keyframe >= length-1)
		res = CURVE_ARRAY(length-1)->m_cp;
	else 
 		CURVE_ARRAY(keyframe)->Interpolate((HKeyframeChannel **)splinearray, keyframe, fraction, length, res);
}


void HBhvInterpolatorScale::InterpolateCamera2(HPoint &quat, bool simulate)
{
 		
    HPoint pos, tar, delta;
	HC_Open_Segment_By_Key(m_pTarget);

	HC_Show_Net_Camera_Position(&pos.x, &pos.y, &pos.z);
	HC_Show_Net_Camera_Target(&tar.x, &tar.y, &tar.z);

	delta.Set(pos.x - tar.x,pos.y - tar.y,pos.z- tar.z );
	float length = (float)HC_Compute_Vector_Length(&delta);
	HC_Set_Camera_Field(quat.x * length, quat.x * length);
 	 
		
	HC_Close_Segment();			

	if (!simulate)			
	{
		if (GetAnimation()->GetBehaviorManager()->GetActiveView())
			GetAnimation()->GetBehaviorManager()->GetActiveView()->CameraPositionChanged(true, true);
		GetAnimation()->GetBehaviorManager()->CameraUpdated();
	}

}

void HBhvInterpolatorScale::Evaluate(int keyframe, float fraction, bool &hasPos, HPoint &pos, bool &hasQuat, HQuat &quat, bool &hasScale, HPoint &scale) 
{
	UNREFERENCED(hasPos);
	UNREFERENCED(pos);
	UNREFERENCED(hasQuat);
	UNREFERENCED(quat);

	hasScale = true;
 	CalculatePos(keyframe, fraction, scale);
	if (GetAnimation()->GetTarget()->GetCameraType() != NoCamera)
	{
		if ( m_pTarget == INVALID_KEY)
			SetTarget();
			if (GetAnimation()->GetTarget()->GetCameraType() == CameraTargetFree || GetAnimation()->GetTarget()->GetCameraType() == CameraPositionFree || GetAnimation()->GetTarget()->GetCameraType() == CameraPositionTarget)
			    InterpolateCamera2(scale);
			else
			    InterpolateCamera(scale);
	}
}


void HBhvInterpolatorScale::Interpolate(int keyframe, float fraction)
{
 	HPoint res;

	if ( m_pTarget == INVALID_KEY)
		SetTarget();

	CalculatePos(keyframe, fraction, res);
 
	if (GetAnimation()->GetTarget()->GetCameraType() != NoCamera)
	{
		if (!GetAnimation()->GetBehaviorManager()->GetUpdateCamera())
			return;
		if (GetAnimation()->GetTarget()->GetCameraType() == CameraTargetFree || GetAnimation()->GetTarget()->GetCameraType() == CameraPositionFree || GetAnimation()->GetTarget()->GetCameraType() == CameraPositionTarget)
		    InterpolateCamera2(res);
		else
		    InterpolateCamera(res);
	}
	else
	{

 		AddScaleToMatrix(res);
		HBaseView *view = GetAnimation()->GetBehaviorManager()->GetActiveView();
		HLISTENER_EVENT(HObjectManipulationListener, view->GetEventManager(), ObjectScaledEvent(view, m_pTarget, res));

		return;	 
	}
 	GetAnimation()->GetTarget()->FlagForCollision();
	GetAnimation()->GetBehaviorManager()->GetActiveView()->SetGeometryChanged();

}
 

void HBhvInterpolatorScale::InterpolateCamera(HPoint &scale, bool simulate)
{   
		HPoint t, op, ot;

		HC_Open_Segment_By_Key(m_pTarget);
		HC_Show_Net_Camera_Position(&op.x, &op.y, &op.z);
		HC_Show_Net_Camera_Target(&ot.x, &ot.y, &ot.z);
		HPoint lv;
		lv.Set(ot.x - op.x, ot.y - op.y, ot.z - op.z);
		HC_Compute_Normalized_Vector(&lv, &t);
		float length = scale.x;
		HPoint nt;
		if (GetAnimation()->GetTarget()->GetCameraType() == CameraPosition)
		{
			nt.Set(op.x + length * t.x, op.y + length * t.y, op.z + length * t.z);
			HC_Set_Camera_Target(nt.x, nt.y, nt.z);

		}
		else
		{
			nt.Set(ot.x - length * t.x, ot.y - length * t.y, ot.z - length * t.z);
			HC_Set_Camera_Position(nt.x, nt.y, nt.z);
		}
		if (scale.y>0.001)
		    HC_Set_Camera_Field(scale.x, scale.y);

  		HC_Close_Segment();
		if (!simulate)
		{
			if (GetAnimation()->GetBehaviorManager()->GetActiveView())
				GetAnimation()->GetBehaviorManager()->GetActiveView()->CameraPositionChanged(true, true);
			GetAnimation()->GetBehaviorManager()->CameraUpdated();
		}

}

/*
void HBhvInterpolatorInstanceScale::InterpolateCamera(HPoint &scale)
{
		HPoint t, op, ot;

		HC_Open_Segment(m_pTarget);
		HC_Show_Net_Camera_Position(&op.x, &op.y, &op.z);
		HC_Show_Net_Camera_Target(&ot.x, &ot.y, &ot.z);
		HPoint lv;
		lv.Set(ot.x - op.x, ot.y - op.y, ot.z - op.z);
		HC_Compute_Normalized_Vector(&lv, &t);
		float length = scale.x;
		HPoint nt;
		nt.Set(op.x + length * t.x, op.y + length * t.y, op.z + length * t.z);
		HC_Set_Camera_Target(nt.x, nt.y, nt.z);
 		HC_Close_Segment();
}

*/

HBhvInterpolatorQuatSquad::HBhvInterpolatorQuatSquad(HBhvAnimation *animationinstance, const char *name) : HBhvInterpolator(animationinstance, name)
{
 
}



void HBhvInterpolatorQuatSquad::AdjustQuaternions()
{
	HKeyframeQuatSquad **quatarray  = (HKeyframeQuatSquad **)GetArray();
  	int length = GetArrayLength();
	float q1[4];
	float q2[4];
	HQuat qres;
	for (int i=0;i<length-1;i++)
	{

	q1[0] = quatarray[i]->m_quat.x;
	q1[1] = quatarray[i]->m_quat.y;
	q1[2] = quatarray[i]->m_quat.z;
	q1[3] = quatarray[i]->m_quat.w;

	q2[0] = quatarray[i+1]->m_quat.x;
	q2[1] = quatarray[i+1]->m_quat.y;
	q2[2] = quatarray[i+1]->m_quat.z;
	q2[3] = quatarray[i+1]->m_quat.w;

	float dot1, dot2;

	    dot1 = ((q1[0] - q2[0])*(q1[0] - q2[0])
			+ (q1[1] - q2[1])*(q1[1] - q2[1])
			+ (q1[2] - q2[2])*(q1[2] - q2[2])
			+ (q1[3] - q2[3])*(q1[3] - q2[3])); 

	    dot2 = ((q1[0] + q2[0])*(q1[0] + q2[0])
			+ (q1[1] + q2[1])*(q1[1] + q2[1])
			+ (q1[2] + q2[2])*(q1[2] + q2[2])
			+ (q1[3] + q2[3])*(q1[3] + q2[3])); 

	 if (dot2 < dot1) {
		   qres.Set(-q2[0],  -q2[1], -q2[2], -q2[3]);
			Replace(qres, i+1);
		}

 
	}
}

void HBhvInterpolatorQuatSquad::Serialize(HUtilityXMLGenerator *xmlgen)
{
 		HUtilityXMLTag xmlt;
		char tagname[MVO_BUFFER_SIZE];
		strcpy(tagname, GetType());
		strcat(tagname,"Interpolator");
		xmlt.SetTagname(tagname);
		xmlt.AddProperty("Name", m_Name, true);
 
		HKeyframeQuatSquad **array = (HKeyframeQuatSquad **)GetArray();
		for (int i=0; i< GetArrayLength(); i++)
			array[i]->Serialize(&xmlt);

   		xmlgen->AddTag(&xmlt);

}


void *HBhvInterpolatorQuatSquad::XMLCallback(HUtilityXMLTag *xt, bool open, void *m_pExtraData)
{
	UNREFERENCED(m_pExtraData);

	char Name[MVO_SMALL_BUFFER_SIZE];
// 	static delimiter_t find_items_td[] = {{"l|t|b|d|c|", true, 0, true, false},
//											{"\x30-\x39|-|", false, " |]", true, false}};

	if (open)
	{

		HBhvAnimation *current_animation = (HBhvAnimation *)xt->GetXMLParser()->GetStackData();

		if (!xt->GetProperty("Name", Name))
			strcpy(Name, "");

  
		HBhvInterpolatorQuatSquad *itp = new HBhvInterpolatorQuatSquad(current_animation, Name);
 
		xt->StartListItemsQuery();
		int counter = 0;
		while (1)
		{
			bool relative = false;
			bool linear = false;
			bool HoldConstant = false;
			int extraSpins = 0;
			HUtilityTextParser tp;
			char *litem;
			char result[MVO_BUFFER_SIZE];
 			HQuat pos;
 			if (!xt->GetNextListItem(&litem))
				break;

			if (FindModifier(litem, "es", result))
			{
				extraSpins = atoi(result);			 
			}

				relative = true;

			if (FindModifier(litem, "r", 0))
				relative = true;

			if (FindModifier(litem, "l", 0))
					linear = true;

			if (FindModifier(litem, "*", 0))
				HoldConstant = true;
 			else
			{
				FindData(litem, result);
				HBhvUtility::GetQuatPoint(result, pos);
			}

 			itp->Insert(pos, counter);			
			itp->m_pArray[counter]->SetRelative(relative);
			((HKeyframeQuatSquad *)itp->m_pArray[counter])->m_bLinear = linear;
			((HKeyframeQuatSquad *)itp->m_pArray[counter])->m_ExtraSpins  = extraSpins;
			itp->m_pArray[counter]->SetConstant(HoldConstant);
			counter++;

		}

   		current_animation->AddInterpolator(itp);
	}

	return 0;

}
 		



HBhvInterpolator * HBhvInterpolatorQuatSquad::CreateInstance(HBhvAnimation *ainst) 
{ 
	
	HBhvInterpolatorQuatSquad *i = new HBhvInterpolatorQuatSquad(ainst);
	i->SetInstancedInterpolator((HBhvInterpolator *)this);
	return i;

}
 
const char * HBhvInterpolatorQuatSquad::GetType() { return "QuatRot"; } 




void HBhvInterpolatorQuatSquad::CalculateQuat(int keyframe, float fraction,  HQuat &res)
{
	HKeyframeQuatSquad **quatarray  = (HKeyframeQuatSquad **)GetArray();
 	int length = GetArrayLength();
	HQuat q0, q1, q2, q3, a, b;
	

	if (!GetAnimation()->GetCurrentlyRunning() && quatarray[0]->m_bConstant)		
 			GetRotationFromMatrix(quatarray[0]->m_quat);

	if (keyframe >= length-1)
		res = quatarray[length-1]->m_quat;
	else 
	{
		q1  = quatarray[keyframe]->m_quat;
		q2 = quatarray[keyframe+1]->m_quat;
				
		if (keyframe>0)
			q0 = quatarray[keyframe-1]->m_quat;
		else
			q0 = q1;		
		if (keyframe<length-2)
			q3 = quatarray[keyframe+2]->m_quat;
		else
			q3 = q2;		
		
		if (quatarray[keyframe]->m_ExtraSpins)
			res = HQuat::QslerpNoInvertExtraSpins(q1,q2,fraction,quatarray[keyframe]->m_ExtraSpins);
		else
		{		
			
			if (quatarray[keyframe]->m_bLinear)
			{
				float q11[4];
				float q22[4];
				float res2[4];
				q11[0] = q1.x; q11[1] = q1.y; q11[2] = q1.z; q11[3] = q1.w;
				q22[0] = q2.x; q22[1] = q2.y; q22[2] = q2.z; q22[3] = q2.w;			
				HUtility::TransitionQuaternion(q11, q22, fraction, res2);
				res.x = res2[0]; 	res.y = res2[1]; 		res.z = res2[2]; 		res.w = res2[3];
			}
			else
			{						
				a = HQuat::Qspline(q0,q1,q2);
				b = HQuat::Qspline(q1,q2,q3);
				res = HQuat::Qsquad(q1, q2, a,b, fraction);
			}
		}

	}

}




void HBhvInterpolatorQuatSquad::Evaluate(int keyframe, float fraction, bool &hasPos, HPoint &pos, bool &hasQuat, HQuat &quat, bool &hasScale, HPoint &scale) 
{
	UNREFERENCED(hasPos);
	UNREFERENCED(pos);
	UNREFERENCED(hasScale);
	UNREFERENCED(scale);

	hasQuat = true;
 	CalculateQuat(keyframe, fraction, quat);
	if (GetAnimation()->GetTarget()->GetCameraType() != NoCamera)
	{
		if ( m_pTarget == INVALID_KEY)
			SetTarget();

 	      if (GetAnimation()->GetTarget()->GetCameraType() == CameraTargetFree || GetAnimation()->GetTarget()->GetCameraType() == CameraPositionFree || GetAnimation()->GetTarget()->GetCameraType() == CameraPositionTarget)
		    InterpolateCamera2(quat, true);
		else
		    InterpolateCamera(quat, true);
	}

}



void HBhvInterpolatorQuatSquad::Interpolate(int keyframe, float fraction)
{
 
	HQuat res;
	if ( m_pTarget == INVALID_KEY)
		SetTarget();

	CalculateQuat(keyframe, fraction, res);

	if (GetAnimation()->GetTarget()->GetCameraType() != NoCamera)
	{
		if (!GetAnimation()->GetBehaviorManager()->GetUpdateCamera())
			return;
	        if (GetAnimation()->GetTarget()->GetCameraType() == CameraTargetFree || GetAnimation()->GetTarget()->GetCameraType() == CameraPositionFree || GetAnimation()->GetTarget()->GetCameraType() == CameraPositionTarget)
		    InterpolateCamera2(res);
		else
		    InterpolateCamera(res);
	}
	else
	{	 	
		float rotation_matrix[16];
		res.ToMatrix(rotation_matrix); 	
 		AddRotationToMatrix(rotation_matrix);
	}
//	GetAnimation()->GetTarget()->AdjustChildTargets();
 	GetAnimation()->GetTarget()->FlagForCollision();
	GetAnimation()->GetBehaviorManager()->GetActiveView()->SetGeometryChanged();

}




void HBhvInterpolatorQuatSquad::InterpolateCamera2(HQuat &quat, bool simulate)
{

	float rot[16];
    	quat.ToMatrix(rot); 	
  	float translation_matrix[16];
 	float result[16];
	float segment_modelling_matrix[16];	
 		
	HC_Open_Segment_By_Key(m_pTarget);


 		HPoint op;
		op.Set(0,0,0);
		
		{
		    
		    HPoint pivot;
 		  	
		    GetAnimation()->GetTarget()->GetPivot(pivot);

		    if (GetAnimation()->GetTarget()->GetCameraType() == CameraPositionFree || GetAnimation()->GetTarget()->GetCameraType() == CameraPositionTarget)		   		     		   
 			GetAnimation()->GetBehaviorManager()->GetPositionMatrix(segment_modelling_matrix);
		    else
 			GetAnimation()->GetBehaviorManager()->GetTargetMatrix(segment_modelling_matrix);

 		    HC_Compute_Transformed_Points(1, &op, segment_modelling_matrix, &op);
		}
	
  		HC_Compute_Translation_Matrix(op.x, op.y, op.z, translation_matrix);
 
  		HC_Compute_Matrix_Product(rot, translation_matrix, result);
		    	
		if (GetAnimation()->GetTarget()->GetCameraType() == CameraPositionFree || GetAnimation()->GetTarget()->GetCameraType() == CameraPositionTarget)
		{		
		    HPoint pos, up, tar;
		    pos.Set(0,0,0);
		    up.Set(0,1,0);

		    if (GetAnimation()->GetTarget()->GetCameraType() == CameraPositionTarget)
		    {
			HPoint p,t, delta;
			HC_Show_Net_Camera_Position(&p.x, &p.y, &p.z);
			HC_Show_Net_Camera_Target(&t.x, &t.y, &t.z);
			delta.Set(t.x - p.x, t.y - p.y, t.z - p.z);
			tar.Set(0,0,(float)-HC_Compute_Vector_Length(&delta));			
			HC_Compute_Transformed_Points(1, &tar, result, &tar);
			HC_Set_Camera_Target(tar.x, tar.y, tar.z);

		    }
		    HC_Compute_Transformed_Points(1, &pos, result, &pos);
		    HC_Compute_Transformed_Points(1, &up, result, &up);		
		    up.Add(-pos.x, - pos.y, -pos.z);
		    HC_Compute_Normalized_Vector(&up,&up);
		    HC_Set_Camera_Position(pos.x, pos.y, pos.z);
		    HC_Set_Camera_Up_Vector(up.x, up.y, up.z);
 		    GetAnimation()->GetBehaviorManager()->SetPositionMatrix(result);

		}
		else
		{		
		    HPoint pos, up;
		    pos.Set(0,0,0);
 		    HC_Compute_Transformed_Points(1, &pos, result, &pos);
  		    HC_Set_Camera_Target(pos.x, pos.y, pos.z);
 		    GetAnimation()->GetBehaviorManager()->SetPositionMatrix(result);
		}

		
		HC_Close_Segment();			

			
	if (!simulate)
	{
		if (GetAnimation()->GetBehaviorManager()->GetActiveView())
			GetAnimation()->GetBehaviorManager()->GetActiveView()->CameraPositionChanged(true, true);
		GetAnimation()->GetBehaviorManager()->CameraUpdated();
	}

}


void HBhvInterpolatorQuatSquad::InterpolateCamera(HQuat &quat, bool simulate)
{
		float matrix[16];
		quat.ToMatrix(matrix);
		HPoint t,u, op, ot;
		GetCameraFromMatrix(matrix, &t,&u);

		HC_Open_Segment_By_Key(m_pTarget);
		HC_Show_Net_Camera_Position(&op.x, &op.y, &op.z);
		HC_Show_Net_Camera_Target(&ot.x, &ot.y, &ot.z);
		HPoint lv;
		lv.Set(ot.x - op.x, ot.y - op.y, ot.z - op.z);
		float length = (float)HC_Compute_Vector_Length(&lv);
		HPoint nt;
		if (GetAnimation()->GetTarget()->GetCameraType() == CameraPosition)
		{
			nt.Set(op.x + length * t.x, op.y + length * t.y, op.z + length * t.z);
			HC_Set_Camera_Target(nt.x, nt.y, nt.z);
		}
		else
		{
			nt.Set(ot.x - length * t.x, ot.y - length * t.y, ot.z - length * t.z);
			HC_Set_Camera_Position(nt.x, nt.y, nt.z);
		}
 		HC_Set_Camera_Up_Vector(u.x, u.y, u.z);
		HC_Close_Segment();
		if (!simulate)
		{
			if (GetAnimation()->GetBehaviorManager()->GetActiveView())
				GetAnimation()->GetBehaviorManager()->GetActiveView()->CameraPositionChanged(true, true);
			GetAnimation()->GetBehaviorManager()->CameraUpdated();
		}

}

 







HBhvInterpolatorAttSwitch::HBhvInterpolatorAttSwitch(HBhvAnimation *animationinstance, const char *name) 
	: HBhvInterpolator(animationinstance, name)
{
 
}

const char * HBhvInterpolatorAttSwitch::GetType() { return "AttSwitch"; } 


void HBhvInterpolatorAttSwitch::Serialize(HUtilityXMLGenerator *xmlgen)
{
 		HUtilityXMLTag xmlt;
		char tagname[MVO_BUFFER_SIZE];
		strcpy(tagname, GetType());
		strcat(tagname,"Interpolator");
		xmlt.SetTagname(tagname);
		xmlt.AddProperty("Name", m_Name, true);
  
		HKeyframeString **array = (HKeyframeString **)GetArray();
		for (int i=0; i< GetArrayLength(); i++)
			array[i]->Serialize(&xmlt);

   		xmlgen->AddTag(&xmlt);

}

void *HBhvInterpolatorAttSwitch::XMLCallback(HUtilityXMLTag *xt, bool open, void *m_pExtraData)
{
	char Name[MVO_SMALL_BUFFER_SIZE];
  	HBaseModel *model = (HBaseModel *)m_pExtraData;
	
	if (open)
	{
		HBhvAnimation *current_animation = (HBhvAnimation *)xt->GetXMLParser()->GetStackData();

		if (!xt->GetProperty("Name", Name))
			strcpy(Name, "");
		HBhvInterpolatorAttSwitch *itp = new HBhvInterpolatorAttSwitch(current_animation, Name);

		xt->StartListItemsQuery();
		int counter = 0;
		while (1)
		{
 			char *temp;
			char litem[MVO_BUFFER_SIZE];
    			if (!xt->GetNextListItem(&temp))
				break;
				if (temp[0] == '"')
				{
					strcpy(litem, temp + 1);
					litem[strlen(litem) - 1] = 0;
				}
				else
					strcpy(litem, temp);

				char result[MVO_BUFFER_SIZE];	
				HBhvCameraType dummy;
				char dummytype[MVO_SMALL_BUFFER_SIZE];
				 HBhvTargetObject::ResolveTarget(litem, result, dummytype, model, dummy);

  			itp->Insert(result, counter++);
		}

 		current_animation->AddInterpolator(itp);
	}
	return 0;

}

HBhvInterpolator * HBhvInterpolatorAttSwitch::CreateInstance(HBhvAnimation *ainst) 
{ 
	
	HBhvInterpolatorAttSwitch *i = new HBhvInterpolatorAttSwitch(ainst);
	i->SetInstancedInterpolator((HBhvInterpolator *)this);
	return i;
}
 


void HBhvInterpolatorAttSwitch::Interpolate(int keyframe, float fraction)
{
	UNREFERENCED(fraction);

	HKeyframeString **posarray  = (HKeyframeString **)GetArray();
 	int length = GetArrayLength();

	if ( m_pTarget == INVALID_KEY)
		SetTarget();

	char  res[MVO_BUFFER_SIZE];
 	if (keyframe >= length-1)
		strcpy(res, posarray[length-1]->GetTarget());
	else 
		strcpy(res, posarray[keyframe]->GetTarget());

	HC_KEY stylekey;
	bool dontstyle = false;
 	HC_Open_Segment_By_Key(m_pTarget);
	HC_Begin_Contents_Search(".","style");
	if (HUtility::FindContents(stylekey))
	{
		char stylesegment[MVO_BUFFER_SIZE];
		HC_Show_Style_Segment(stylekey, stylesegment);
		if (strcmp(stylesegment,res) == 0)
			dontstyle = true;
	}	
	HC_End_Contents_Search();
	if (!dontstyle)
	{	
	HC_Flush_Contents(".", "style");
	if (strcmp(res, "") != 0)
		HC_Style_Segment(res);
	}
	HC_Close_Segment();		
}
 
 




HBhvInterpolatorVertexMorph::HBhvInterpolatorVertexMorph(HBhvAnimation *animationinstance, const char *name) 
		: HBhvInterpolator(animationinstance, name)
{
	m_MorphDataLength = 0;  
}

HBhvInterpolatorVertexMorph::~HBhvInterpolatorVertexMorph()
{
	for (int i=0;i<m_MorphDataLength;i++)	
			delete [] (HPoint *)m_pMorphData[i];
	
  
}

void * HBhvInterpolatorVertexMorph::GetMorphData(int i)
{
	return m_pMorphData[i];
}


const char * HBhvInterpolatorVertexMorph::GetType() { return "VertexMorph"; } 


void HBhvInterpolatorVertexMorph::Serialize(HUtilityXMLGenerator *xmlgen)
{
 		HUtilityXMLTag xmlt;
		char tagname[MVO_BUFFER_SIZE];
		strcpy(tagname, GetType());
		strcat(tagname,"Interpolator");
		xmlt.SetTagname(tagname);
		xmlt.AddProperty("Name", m_Name, true);
  
		HKeyframeString **array = (HKeyframeString **)GetArray();
		for (int i=0; i< GetArrayLength(); i++)
			array[i]->Serialize(&xmlt);

   		xmlgen->AddTag(&xmlt);

}

void *HBhvInterpolatorVertexMorph::XMLCallback(HUtilityXMLTag *xt, bool open, void *m_pExtraData)
{
	char Name[MVO_SMALL_BUFFER_SIZE];
  	HBaseModel *model = (HBaseModel *)m_pExtraData;
	
	if (open)
	{
		HBhvAnimation *current_animation = (HBhvAnimation *)xt->GetXMLParser()->GetStackData();

		if (!xt->GetProperty("Name", Name))
			strcpy(Name, "");
		HBhvInterpolatorVertexMorph *itp = new HBhvInterpolatorVertexMorph(current_animation, Name);

		xt->StartListItemsQuery();
		int counter = 0;
		while (1)
		{
 			char *temp;
			char result[MVO_BUFFER_SIZE];	
			char litem[MVO_BUFFER_SIZE];
			HANIChannelType channelType = HANILinear;

    			if (!xt->GetNextListItem(&temp))
				break;
				if (temp[0] == '"')
				{
					strcpy(litem, temp + 1);
					litem[strlen(litem) - 1] = 0;
				}
				else
				{
					strcpy(litem, temp);

					if (FindModifier(litem, "d", 0))
						channelType = HANIDiscrete;		 
 					FindStringData(litem, result);
					strcpy(litem, result);
				

				}


  				HBhvCameraType dummy;
				char dummytype[MVO_SMALL_BUFFER_SIZE];
				HBhvTargetObject::ResolveTarget(litem, result, dummytype, model, dummy);				
				int keynum = atoi(result);

				char text[MVO_BUFFER_SIZE];
				HC_Open_Segment_By_Key(model->GetModelKey());
				sprintf(text, "MorphData%s", itp->GetName());
				HC_Open_Segment(text);
 				if (keynum>=itp->m_MorphDataLength)
				{
					for (int j=itp->m_MorphDataLength;j<=keynum;j++)
						itp->m_pMorphData[j] = 0;
					itp->m_MorphDataLength = keynum+1;
				}

				if (!itp->m_pMorphData[keynum])
				{
				int plen, flen;
				HC_Show_Shell_Size((HC_KEY)keynum, &plen, &flen);
				HPoint *plist = new HPoint[plen];
				HC_Show_Shell((HC_KEY)keynum, &plen, plist, &flen, 0);				
 				itp->m_pMorphData[keynum] = (void *)plist;
				}
				HC_Close_Segment();
				HC_Close_Segment();


				if (channelType == HANIDiscrete)
					itp->InsertDiscrete(result,counter++);
				else				
  					itp->Insert(result, counter++);
		}

 		current_animation->AddInterpolator(itp);
	}
	return 0;

}


HBhvInterpolator * HBhvInterpolatorVertexMorph::CreateInstance(HBhvAnimation *ainst) 
{ 
	
	HBhvInterpolatorVertexMorph *i = new HBhvInterpolatorVertexMorph(ainst);
	i->SetInstancedInterpolator((HBhvInterpolator *)this);
	return i;

}
 




void * HBhvInterpolatorVertexMorph::GetUserInfo(char *target)
{
	//void *info;
//	char token[MVO_SMALL_BUFFER_SIZE];
	/*
	char text[1000];
	
	strcpy(text, target);
	HC_Parse_String(text, "/", -1, token);
	text[strlen(text) - strlen(token) - 1] = 0;
	*/
	return GetMorphData(atoi(target));

#if 0
	HC_Open_Segment("/data");
 	HC_Show_One_User_Index(atoi(target), (void **)&info);
	HC_Close_Segment();
	return info;
#endif
}

void HBhvInterpolatorVertexMorph::Interpolate(int keyframe, float fraction)
{
	HKeyframeString **posarray  = (HKeyframeString **)GetArray();
 	int length = GetArrayLength();

	if ( m_pTarget == INVALID_KEY)
	{
		SetTarget();
 	 	HC_Open_Segment_By_Key(m_pTarget);
		HC_Begin_Contents_Search(".", "shells");
		char type[MVO_SMALL_BUFFER_SIZE];
		HC_KEY key;
		if (HC_Find_Contents(type, &key))
			m_shelltarget = key;
 		HC_End_Contents_Search();
		HC_Close_Segment();
	}

 	HPoint delta;
	int pcount, fcount;
	HC_Show_Shell_Size(m_shelltarget, &pcount, &fcount);
	HPoint *res = new HPoint[pcount];

	if (keyframe >= length-1)
	{
		HPoint *ptemp = (HPoint *)GetUserInfo(posarray[length-1]->GetTarget());
		for (int i=0;i<pcount;i++)
			res[i] = ptemp[i];
	}

 	else 
	{
		if (!posarray[keyframe]->m_bDiscrete)
		{
 			HPoint *points0 = (HPoint *)GetUserInfo(posarray[keyframe]->GetTarget());
			HPoint *points1 = (HPoint *)GetUserInfo(posarray[keyframe + 1]->GetTarget());
			HC_Show_Shell_Size(m_shelltarget, &pcount, &fcount);
			HPoint delta;
			for (int i=0;i<pcount;i++)
			{
				delta.Set(points1[i].x - points0[i].x, points1[i].y - points0[i].y,points1[i].z - points0[i].z);
 				res[i].Set(points0[i].x + delta.x * fraction, points0[i].y + delta.y * fraction,
					points0[i].z + delta.z * fraction);
			}
		}
		else
		{
	 			HPoint *points0 = (HPoint *)GetUserInfo(posarray[keyframe]->GetTarget());
				for (int i=0;i<pcount;i++)				
 					res[i].Set(points0[i].x, points0[i].y,points0[i].z);
		}

 
	}
		
 	HC_Edit_Shell_Points(m_shelltarget, 0, pcount, pcount,res);
	delete [] res;


}



void HBhvInterpolatorVertexMorph::AddMorphData(HPoint *md, int pnum, HBaseModel *model, int pos) 
{ 

	int currentpos;
	char text[MVO_BUFFER_SIZE];

	if (pos>=0)
		currentpos = pos;
	else
		currentpos = m_MorphDataLength;
 	char status[MVO_BUFFER_SIZE];
	HC_Open_Segment_By_Key(model->GetModelKey());
	sprintf(text, "MorphData%s", GetName());
 	HC_KEY mdkey = HC_KOpen_Segment(text);
	HC_Show_Key_Status(currentpos, status);
 	if (strcmp(status,"local") == 0 || (strcmp(status,"global") == 0 && HC_KShow_Owner_By_Key(currentpos) == mdkey))
		HC_Delete_By_Key(m_MorphDataLength);
	HC_KEY key = HC_KInsert_Shell(pnum, md, 0,0);
	HC_Renumber_Key(key, currentpos,"local");


	HC_Close_Segment();
	HC_Close_Segment();

	
	HPoint *temp = new HPoint[pnum];
	for (int i=0;i<pnum;i++)
		temp[i] = md[i];

	if (pos>=0) 
		m_pMorphData[pos] = temp; 
	else 
		m_pMorphData[m_MorphDataLength++] = temp;

}






HBhvInterpolatorColorMorph::HBhvInterpolatorColorMorph(HBhvAnimation *animationinstance, const char *name) 
		: HBhvInterpolator(animationinstance, name)
{
	m_MorphDataLength = 0;  
	m_bUseFIndex = false;
}

HBhvInterpolatorColorMorph::~HBhvInterpolatorColorMorph()
{
	for (int i=0;i<m_MorphDataLength;i++)	
			delete (char *)m_pMorphData[i];
	
  
}

void * HBhvInterpolatorColorMorph::GetMorphData(int i)
{
	return m_pMorphData[i];
}


const char * HBhvInterpolatorColorMorph::GetType() { return "ColorMorph"; } 


void HBhvInterpolatorColorMorph::Serialize(HUtilityXMLGenerator *xmlgen)
{
 		HUtilityXMLTag xmlt;
		char tagname[MVO_BUFFER_SIZE];
		strcpy(tagname, GetType());
		strcat(tagname,"Interpolator");
		xmlt.SetTagname(tagname);
		xmlt.AddProperty("Name", m_Name, true);
		xmlt.AddProperty("Name", m_Name, true);
		xmlt.AddPropertyInt("UseFIndex", m_bUseFIndex);
		HKeyframeString **array = (HKeyframeString **)GetArray();
		for (int i=0; i< GetArrayLength(); i++)
			array[i]->Serialize(&xmlt);

   		xmlgen->AddTag(&xmlt);

}

void *HBhvInterpolatorColorMorph::XMLCallback(HUtilityXMLTag *xt, bool open, void *m_pExtraData)
{
	char Name[MVO_SMALL_BUFFER_SIZE];
	bool usefindex;
  	HBaseModel *model = (HBaseModel *)m_pExtraData;
	
	if (open)
	{
		HBhvAnimation *current_animation = (HBhvAnimation *)xt->GetXMLParser()->GetStackData();

		if (!xt->GetProperty("Name", Name))
			strcpy(Name, "");
		if (!xt->GetProperty("UseFIndex", usefindex))
			usefindex = false;
		HBhvInterpolatorColorMorph *itp = new HBhvInterpolatorColorMorph(current_animation, Name);
		itp->m_bUseFIndex = usefindex;
		xt->StartListItemsQuery();
		int counter = 0;
		while (1)
		{
 			char *temp;
			char litem[MVO_BUFFER_SIZE];
			char result[MVO_BUFFER_SIZE];	
    		HANIChannelType channelType = HANILinear;

    			if (!xt->GetNextListItem(&temp))
				break;
				if (temp[0] == '"')
				{
					strcpy(litem, temp + 1);
					litem[strlen(litem) - 1] = 0;
				}
				else
				{
					strcpy(litem, temp);

					if (FindModifier(litem, "d", 0))
						channelType = HANIDiscrete;		 
 					FindStringData(litem, result);
					strcpy(litem, result);
				

				}
		
				HBhvCameraType dummy;
				char dummytype[MVO_SMALL_BUFFER_SIZE];
				HBhvTargetObject::ResolveTarget(litem, result, dummytype, model, dummy);				
				int keynum = atoi(result);
				char text[MVO_BUFFER_SIZE];
				HC_Open_Segment_By_Key(model->GetModelKey());
				sprintf(text, "MorphData%s", itp->GetName());
 				HC_Open_Segment(text);
 				if (keynum>=itp->m_MorphDataLength)
				{
					for (int j=itp->m_MorphDataLength;j<=keynum;j++)
						itp->m_pMorphData[j] = 0;
					itp->m_MorphDataLength = keynum+1;
				}

				if (!itp->m_pMorphData[keynum])
				{
					int plen, flen;
					HC_Show_Shell_Size((HC_KEY)keynum, &plen, &flen);
					HPoint *plist = new HPoint[plen];
					HC_Show_Shell((HC_KEY)keynum, &plen, plist, &flen, 0);				
					itp->m_pMorphData[keynum] = (void *)plist;
				}

				HC_Close_Segment();
				HC_Close_Segment();


  				if (channelType == HANIDiscrete)
					itp->InsertDiscrete(result,counter++);
				else				
  					itp->Insert(result, counter++);

		}

 		current_animation->AddInterpolator(itp);
	}
	return 0;

}


HBhvInterpolator * HBhvInterpolatorColorMorph::CreateInstance(HBhvAnimation *ainst) 
{ 
	
	HBhvInterpolatorColorMorph *i = new HBhvInterpolatorColorMorph(ainst);
	i->SetInstancedInterpolator((HBhvInterpolator *)this);
	return i;

}
 

void HBhvInterpolatorColorMorph::AddMorphData(HPoint *md, int pnum, HBaseModel *model, int pos) 
{ 

	int currentpos;
	if (pos>=0)
		currentpos = pos;
	else
		currentpos = m_MorphDataLength;
 	char status[MVO_BUFFER_SIZE];
	char text[MVO_BUFFER_SIZE];
	HC_Open_Segment_By_Key(model->GetModelKey());
	sprintf(text, "MorphData%s", GetName());
 	HC_KEY mdkey = HC_KOpen_Segment(text);
	HC_Set_Heuristics("exclude bounding");

	HC_Show_Key_Status(currentpos, status);
 	if (strcmp(status,"local") == 0 || (strcmp(status,"global") == 0 && HC_KShow_Owner_By_Key(currentpos) == mdkey))
		HC_Delete_By_Key(m_MorphDataLength);
	HC_KEY key = HC_KInsert_Shell(pnum, md, 0,0);
	HC_Renumber_Key(key, currentpos,"local");


	HC_Close_Segment();
	HC_Close_Segment();

	
	HPoint *temp = new HPoint[pnum];
	for (int i=0;i<pnum;i++)
		temp[i] = md[i];

	if (pos>=0) 
		m_pMorphData[pos] = temp; 
	else 
		m_pMorphData[m_MorphDataLength++] = temp;

}



void HBhvInterpolatorColorMorph::SetUseFIndex(bool tf)
{
	m_bUseFIndex = tf;
}


void * HBhvInterpolatorColorMorph::GetUserInfo(char *target)
{
 
	return GetMorphData(atoi(target));

}

void HBhvInterpolatorColorMorph::Interpolate(int keyframe, float fraction)
{
	HKeyframeString **posarray  = (HKeyframeString **)GetArray();
 	int length = GetArrayLength();

	if ( m_pTarget == INVALID_KEY)
	{
		SetTarget();
 	 	HC_Open_Segment_By_Key(m_pTarget);
		HC_Begin_Contents_Search(".", "shells");
		char type[MVO_SMALL_BUFFER_SIZE];
		HC_KEY key;
		if (HC_Find_Contents(type, &key))
			m_shelltarget = key;
 		HC_End_Contents_Search();
		HC_Close_Segment();
	}

 	HPoint delta;
	int pcount, fcount;
	HC_Show_Shell_Size(m_shelltarget, &pcount, &fcount);
	HPoint *res = 0;
	float *resf = 0;
	
	if (!m_bUseFIndex)
		res = new HPoint[pcount];
	else
		resf = new float[pcount];


 	if (keyframe >= length-1)
	{
		HPoint *ptemp = (HPoint *)GetUserInfo(posarray[length-1]->GetTarget());
		if (!m_bUseFIndex)
		{		
			for (int i=0;i<pcount;i++)
				res[i] = ptemp[i];
		}
		else
		{
			for (int i=0;i<pcount;i++)
				resf[i] = ptemp[i].x;
		}
	}

 	else 
	{
 		if (!posarray[keyframe]->m_bDiscrete)
		{
 			HPoint *points0 = (HPoint *)GetUserInfo(posarray[keyframe]->GetTarget());
			HPoint *points1 = (HPoint *)GetUserInfo(posarray[keyframe + 1]->GetTarget());
			HC_Show_Shell_Size(m_shelltarget, &pcount, &fcount);
			HPoint delta;
			for (int i=0;i<pcount;i++)
			{
				delta.Set(points1[i].x - points0[i].x, points1[i].y - points0[i].y,points1[i].z - points0[i].z);
				if (m_bUseFIndex)
	 				resf[i] = points0[i].x + delta.x * fraction;
				else
 					res[i].Set(points0[i].x + delta.x * fraction, points0[i].y + delta.y * fraction,
						points0[i].z + delta.z * fraction);
			}
		}
		else
		{
	 			HPoint *points0 = (HPoint *)GetUserInfo(posarray[keyframe]->GetTarget());
				for (int i = 0; i < pcount; ++i)				
 					resf[i] = points0[i].x;
		}

		
 
	}
		
	if (!m_bUseFIndex)
		HC_MSet_Vertex_Colors_By_Value(m_shelltarget, "face", 0, "rgb", pcount, res);
	else
		HC_MSet_Vertex_Colors_By_FIndex(m_shelltarget, "face", 0, pcount, resf);

	if (res)
 		delete [] res;
	if (resf)
 		delete [] resf;


}





HBhvInterpolatorSegSwitch::HBhvInterpolatorSegSwitch(HBhvAnimation *animationinstance, const char *name) 
	: HBhvInterpolator(animationinstance, name)
{

  
}

HBhvInterpolatorSegSwitch::~HBhvInterpolatorSegSwitch()
{
  
}



const char * HBhvInterpolatorSegSwitch::GetType() { return "SegSwitch"; } 


void HBhvInterpolatorSegSwitch::Serialize(HUtilityXMLGenerator *xmlgen)
{
 		HUtilityXMLTag xmlt;
		char tagname[MVO_BUFFER_SIZE];
		strcpy(tagname, GetType());
		strcat(tagname,"Interpolator");
		xmlt.SetTagname(tagname);
		xmlt.AddProperty("Name", m_Name, true);
  
		HKeyframeString **array = (HKeyframeString **)GetArray();
		for (int i=0; i< GetArrayLength(); i++)
			array[i]->Serialize(&xmlt);

   		xmlgen->AddTag(&xmlt);

}

void *HBhvInterpolatorSegSwitch::XMLCallback(HUtilityXMLTag *xt, bool open, void *m_pExtraData)
{
	char Name[MVO_SMALL_BUFFER_SIZE];
  	HBaseModel *model = (HBaseModel *)m_pExtraData;
	
	if (open)
	{
		HBhvAnimation *current_animation = (HBhvAnimation *)xt->GetXMLParser()->GetStackData();

//temporary for testing
		HC_Open_Segment("/data");
		for (int i=0;i<10;i++)
			HC_Set_User_Index(i, 0);
		HC_Close_Segment();

		if (!xt->GetProperty("Name", Name))
			strcpy(Name, "");
		HBhvInterpolatorSegSwitch *itp = new HBhvInterpolatorSegSwitch(current_animation, Name);

		xt->StartListItemsQuery();
		int counter = 0;
		while (1)
		{
 			char *temp;
			char litem[MVO_BUFFER_SIZE];
    			if (!xt->GetNextListItem(&temp))
				break;
				if (temp[0] == '"')
				{
					strcpy(litem, temp + 1);
					litem[strlen(litem) - 1] = 0;
				}
				else
					strcpy(litem, temp);

			 
				char result[MVO_BUFFER_SIZE];	
				HBhvCameraType dummy;
				char dummytype[MVO_SMALL_BUFFER_SIZE];
				HBhvTargetObject::ResolveTarget(litem, result, dummytype, model, dummy);							 
  			itp->Insert(result, counter++);
		}

 		current_animation->AddInterpolator(itp);
	}
	return 0;

}


HBhvInterpolator * HBhvInterpolatorSegSwitch::CreateInstance(HBhvAnimation *ainst) 
{ 
	
	HBhvInterpolatorSegSwitch *i = new HBhvInterpolatorSegSwitch(ainst);
	i->SetInstancedInterpolator((HBhvInterpolator *)this);
	return i;

}


void HBhvInterpolatorSegSwitch::Reset()
{
	HKeyframeString **posarray  = (HKeyframeString **)GetArray();
 	int length = GetArrayLength();
	for (int i=0;i<length;i++)
	{
		HC_Open_Segment(posarray[i]->GetTarget());
		HC_Set_Visibility("everything = off");
		HC_Close_Segment();
	}
}


void HBhvInterpolatorSegSwitch::Interpolate(int keyframe, float fraction)
{
	UNREFERENCED(fraction);

	HKeyframeString **posarray  = (HKeyframeString **)GetArray();
 	int length = GetArrayLength();

	char targetsegment[MVO_BUFFER_SIZE];

	if (keyframe >= length-1)
	{
		strcpy(targetsegment, posarray[length-1]->GetTarget());
	}
 	else 
		strcpy(targetsegment, posarray[keyframe]->GetTarget());

	Reset();

	HC_Open_Segment(targetsegment);
	HC_UnSet_Visibility();
	HC_Close_Segment();		

}
 




HBhvInterpolatorInstanceCreate::HBhvInterpolatorInstanceCreate(HBhvAnimation *animationinstance, const char *name) 
	: HBhvInterpolator(animationinstance, name)
{

  
}

HBhvInterpolatorInstanceCreate::~HBhvInterpolatorInstanceCreate()
{
  
}



const char * HBhvInterpolatorInstanceCreate::GetType() { return "InstanceCreate"; } 


void HBhvInterpolatorInstanceCreate::Serialize(HUtilityXMLGenerator *xmlgen)
{
 		HUtilityXMLTag xmlt;
		char tagname[MVO_BUFFER_SIZE];
		strcpy(tagname, GetType());
		strcat(tagname,"Interpolator");
		xmlt.SetTagname(tagname);
		xmlt.AddProperty("Name", m_Name, true);
  
		HKeyframeString **array = (HKeyframeString **)GetArray();
		for (int i=0; i< GetArrayLength(); i++)
			array[i]->Serialize(&xmlt);

   		xmlgen->AddTag(&xmlt);

}



void HBhvInterpolatorInstanceCreate::DeciperString(HBhvInterpolatorInstanceCreate *itp, char *text, HBaseModel *model, int &counter)
{
	char path2[4096];
	char path[4096];
	char include[4096];
	char include2[4096];
	char color[4096];
	char dummytype[MVO_SMALL_BUFFER_SIZE];
	char *pathend = strstr(text,"$$");
	HBhvCameraType dummy;
	if (!pathend)
		return;

 	memcpy(path2, text, pathend - text);
	path2[pathend - text] = 0;
	text = pathend+2;
	pathend = strstr(text,"$$");
	if (!pathend)
		return;
 	memcpy(include2, text, pathend - text);
	include2[pathend - text] = 0;
	text = pathend+2;
	strcpy(color, text);

	HBhvTargetObject::ResolveTarget(path2, path, dummytype, model, dummy);							 
	HBhvTargetObject::ResolveTarget(include2, include, dummytype, model, dummy);							 
	itp->Insert(path, include, color, counter++);


}


void *HBhvInterpolatorInstanceCreate::XMLCallback(HUtilityXMLTag *xt, bool open, void *m_pExtraData)
{
	char Name[MVO_SMALL_BUFFER_SIZE];
  	HBaseModel *model = (HBaseModel *)m_pExtraData;
	
	if (open)
	{
		HBhvAnimation *current_animation = (HBhvAnimation *)xt->GetXMLParser()->GetStackData();

//temporary for testing
		HC_Open_Segment("/data");
		for (int i=0;i<10;i++)
			HC_Set_User_Index(i, 0);
		HC_Close_Segment();

		if (!xt->GetProperty("Name", Name))
			strcpy(Name, "");
		HBhvInterpolatorInstanceCreate *itp = new HBhvInterpolatorInstanceCreate(current_animation, Name);

		xt->StartListItemsQuery();
		int counter = 0;
		while (1)
		{
 			char *temp;
			char litem[MVO_BUFFER_SIZE];
    			if (!xt->GetNextListItem(&temp))
				break;
				if (temp[0] == '"')
				{
					strcpy(litem, temp + 1);
					litem[strlen(litem) - 1] = 0;
				}
				else
					strcpy(litem, temp);

			 
 				DeciperString(itp, litem, model, counter);
//				HBhvTargetObject::ResolveTarget(litem, result, dummytype, model, dummy);							 
//  			itp->Insert(result, counter++);
		}

 		current_animation->AddInterpolator(itp);
	}
	return 0;

}


HBhvInterpolator * HBhvInterpolatorInstanceCreate::CreateInstance(HBhvAnimation *ainst) 
{ 
	
	HBhvInterpolatorInstanceCreate *i = new HBhvInterpolatorInstanceCreate(ainst);
	i->SetInstancedInterpolator((HBhvInterpolator *)this);
	return i;

}


void HBhvInterpolatorInstanceCreate::Reset()
{
	HKeyframe3String **posarray  = (HKeyframe3String **)GetArray();
 	int length = GetArrayLength();
	for (int i=0;i<length;i++)
	{
		HC_Delete_Segment(posarray[i]->GetTarget1());
	}
}


void HBhvInterpolatorInstanceCreate::Interpolate(int keyframe, float fraction)
{
	UNREFERENCED(fraction);

	HKeyframe3String **posarray  = (HKeyframe3String **)GetArray();
 	int length = GetArrayLength();

	char targetsegment[MVO_BUFFER_SIZE];
	char targetinclude[MVO_BUFFER_SIZE];
	char targetcolor[MVO_BUFFER_SIZE];

	if (keyframe >= length-1)
	{
		strcpy(targetsegment, posarray[length-1]->GetTarget1());
		strcpy(targetinclude, posarray[length-1]->GetTarget2());
		strcpy(targetcolor, posarray[length-1]->GetTarget3());
	}
 	else 
	{
		strcpy(targetsegment, posarray[keyframe]->GetTarget1());
		strcpy(targetinclude, posarray[keyframe]->GetTarget2());
		strcpy(targetcolor, posarray[keyframe]->GetTarget3());
	}

	Reset();
	

	HC_Open_Segment(targetsegment);
	HC_Include_Segment(targetinclude);
	HC_Set_Color(targetcolor);
 	HC_Close_Segment();		

}
 






HBhvInterpolatorSegMove::HBhvInterpolatorSegMove(HBhvAnimation *animationinstance, const char *name) 
	: HBhvInterpolator(animationinstance, name)
{

  
}

HBhvInterpolatorSegMove::~HBhvInterpolatorSegMove()
{
  
}



const char * HBhvInterpolatorSegMove::GetType() { return "SegMove"; } 


void HBhvInterpolatorSegMove::Serialize(HUtilityXMLGenerator *xmlgen)
{
 		HUtilityXMLTag xmlt;
		char tagname[MVO_BUFFER_SIZE];
		strcpy(tagname, GetType());
		strcat(tagname,"Interpolator");
		xmlt.SetTagname(tagname);
		xmlt.AddProperty("Name", m_Name, true);
  
		HKeyframeString **array = (HKeyframeString **)GetArray();
		for (int i=0; i< GetArrayLength(); i++)
			array[i]->Serialize(&xmlt);

   		xmlgen->AddTag(&xmlt);

}

void *HBhvInterpolatorSegMove::XMLCallback(HUtilityXMLTag *xt, bool open, void *m_pExtraData)
{
	char Name[MVO_SMALL_BUFFER_SIZE];
  	HBaseModel *model = (HBaseModel *)m_pExtraData;
	
	if (open)
	{
		HBhvAnimation *current_animation = (HBhvAnimation *)xt->GetXMLParser()->GetStackData();

//temporary for testing
		HC_Open_Segment("/data");
		for (int i=0;i<10;i++)
			HC_Set_User_Index(i, 0);
		HC_Close_Segment();

		if (!xt->GetProperty("Name", Name))
			strcpy(Name, "");
		HBhvInterpolatorSegMove *itp = new HBhvInterpolatorSegMove(current_animation, Name);

		xt->StartListItemsQuery();
		int counter = 0;
		while (1)
		{
 			char *temp;
			char litem[MVO_BUFFER_SIZE];
    			if (!xt->GetNextListItem(&temp))
				break;
				if (temp[0] == '"')
				{
					strcpy(litem, temp + 1);
					litem[strlen(litem) - 1] = 0;
				}
				else
					strcpy(litem, temp);

			 
				char result[MVO_BUFFER_SIZE];	
				HBhvCameraType dummy;
				char dummytype[MVO_SMALL_BUFFER_SIZE];
				HBhvTargetObject::ResolveTarget(litem, result, dummytype, model, dummy);							 
  			itp->Insert(result, counter++);
		}

 		current_animation->AddInterpolator(itp);
	}
	return 0;

}


HBhvInterpolator * HBhvInterpolatorSegMove::CreateInstance(HBhvAnimation *ainst) 
{ 
	
	HBhvInterpolatorSegMove *i = new HBhvInterpolatorSegMove(ainst);
	i->SetInstancedInterpolator((HBhvInterpolator *)this);
	return i;

}


void HBhvInterpolatorSegMove::Reset()
{
}

void CutLastText(char *text)
{
	char token[4096];
 	HC_Parse_String(text, "/", -1, token); 
	text[strlen(text)- strlen(token)-1] = 0;

}


void HBhvInterpolatorSegMove::Interpolate(int keyframe, float fraction)
{
	UNREFERENCED(fraction);

	char text[4096];
	if ( m_pTarget == INVALID_KEY)
		SetTarget();

	HKeyframeString **posarray  = (HKeyframeString **)GetArray();
 	int length = GetArrayLength();

	char targetsegment[MVO_BUFFER_SIZE];
	char targetpath[MVO_BUFFER_SIZE];


	if (keyframe >= length-1)
	{
		strcpy(targetsegment, posarray[length-1]->GetTarget());
	}
 	else 
		strcpy(targetsegment, posarray[keyframe]->GetTarget());

	Reset();


	if (m_pTarget != -1 && HC_Show_Existence_By_Key(m_pTarget, "self"))
	{
		char text[4096];
		HC_Show_Segment(m_pTarget, text);
	
		HC_Open_Segment_By_Key(m_pTarget);
		if (!HC_Show_Existence("segments") && !HC_Show_Existence("geometry"))
			m_pTarget = -1;
 		HC_Close_Segment();
	}
 	
	if (m_pTarget == -1 || !HC_Show_Existence_By_Key(m_pTarget, "self"))
	{
	 	int length = GetArrayLength();
		for (int i=0;i<length;i++)
		{
			if (HC_QShow_Existence(posarray[i]->GetTarget(), "self"))
			{
				if (strcmp(posarray[i]->GetTarget(), targetsegment) != 0)
				{

					bool hasStuff = false;
					HC_Open_Segment(posarray[i]->GetTarget());
					if (HC_Show_Existence("segments") || HC_Show_Existence("geometry"))
						hasStuff = true;
					HC_Close_Segment();
					if (hasStuff)
					{
   						m_pTarget = HC_KCreate_Segment(posarray[i]->GetTarget());
						break;
					}
				}
				
			}
							
		}


	}

	strcpy(targetpath, targetsegment);
	CutLastText(targetpath);

	HC_Show_Segment(m_pTarget, text);
	if (m_pTarget != -1 && strcmp(targetsegment, text) != 0)
	{	
		if (HC_QShow_Existence(targetsegment, "self"))
			HC_Delete_Segment(targetsegment);
		HC_Open_Segment(targetpath);
  		HC_Move_By_Key(m_pTarget, ".");
 		HC_Close_Segment();		
		HC_Show_Segment(m_pTarget, text);
	}

}
 





HBhvInterpolatorMatrix::HBhvInterpolatorMatrix(HBhvAnimation *animationinstance, const char *name) : HBhvInterpolator(animationinstance, name)
{
 
}



void HBhvInterpolatorMatrix::Serialize(HUtilityXMLGenerator *xmlgen)
{
 		HUtilityXMLTag xmlt;
		char tagname[MVO_BUFFER_SIZE];
		strcpy(tagname, GetType());
		strcat(tagname,"Interpolator");
		xmlt.SetTagname(tagname);
		xmlt.AddProperty("Name", m_Name, true);
 
		HKeyframeMatrix **array = (HKeyframeMatrix **)GetArray();
		for (int i=0; i< GetArrayLength(); i++)
			array[i]->Serialize(&xmlt);

   		xmlgen->AddTag(&xmlt);

}


void *HBhvInterpolatorMatrix::XMLCallback(HUtilityXMLTag *xt, bool open, void *m_pExtraData)
{
	UNREFERENCED(m_pExtraData);

	char Name[MVO_SMALL_BUFFER_SIZE];
// 	static delimiter_t find_items_td[] = {{"l|t|b|d|c|", true, 0, true, false},
//											{"\x30-\x39|-|", false, " |]", true, false}};

	if (open)
	{

		HBhvAnimation *current_animation = (HBhvAnimation *)xt->GetXMLParser()->GetStackData();

		if (!xt->GetProperty("Name", Name))
			strcpy(Name, "");

  
		HBhvInterpolatorMatrix *itp = new HBhvInterpolatorMatrix(current_animation, Name);
 
		xt->StartListItemsQuery();
		int counter = 0;
		while (1)
		{
  			HUtilityTextParser tp;
			char *litem;
			char result[MVO_BUFFER_SIZE];
 			float pos[16];
 			if (!xt->GetNextListItem(&litem))
				break;

			FindData(litem, result);
			HBhvUtility::GetMatrix(result, pos);
 
 			itp->Insert(pos, counter);			
 			counter++;

		}

   		current_animation->AddInterpolator(itp);
	}

	return 0;

}
 		



HBhvInterpolator * HBhvInterpolatorMatrix::CreateInstance(HBhvAnimation *ainst) 
{ 
	
	HBhvInterpolatorMatrix *i = new HBhvInterpolatorMatrix(ainst);
	i->SetInstancedInterpolator((HBhvInterpolator *)this);
	return i;

}
 
const char * HBhvInterpolatorMatrix::GetType() { return "Matrix"; } 





void HBhvInterpolatorMatrix::Interpolate(int keyframe, float fraction)
{
	UNREFERENCED(fraction);
 
	if ( m_pTarget == INVALID_KEY)
		SetTarget();
	HKeyframeMatrix **posarray  = (HKeyframeMatrix **)GetArray();
 	int length = GetArrayLength();
 
	HC_Open_Segment_By_Key(m_pTarget);

	if (keyframe >= length-1)
		HC_Set_Modelling_Matrix(posarray[length-1]->m_matrix);
	else		
		HC_Set_Modelling_Matrix(posarray[keyframe]->m_matrix);

	HC_Close_Segment();

}

 
 
