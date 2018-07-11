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
#include "HBhvUtility.h"
#include "HBhvBehaviorManager.h"
#include "HBhvTimeline.h"
#include "HBhvAnimation.h"
#include "HBhvInterpolator.h"
#include "HUtility.h"
#include "HBaseModel.h"
#include "HBaseView.h"

#include "HUtilityXMLParser.h"
#include "HUtilityTextParser.h"
#include "HDispatchXML.h"
#include "HStream.h"

#include "vlist.h"
#include "vhash.h"
 


const HQuat & HQuat::Normalize()
{
	float mag = sqrt(x*x + y*y + z*z + w*w);
	if (mag > 0)
	{
		x /= mag;
		y /= mag;
		z /= mag;
		w /= mag;
	}
	return *this;
}



/*
e^HQuat given as:
Qexp(v*a) = [cos(a),vsin(a)]
*/
HQuat HQuat::Qexp(const HQuat & q)
{
	float a = sqrt(q.x*q.x + q.y*q.y + q.z*q.z);
	float sina = sin(a);
	float cosa = cos(a);
	HQuat ret;

	ret.w = cosa;
	if (a > 0)
	{
		ret.x = sina * q.x / a;
		ret.y = sina * q.y / a;
		ret.z = sina * q.z / a;
	}
	else
	{
		ret.x = ret.y = ret.z = 0;
	}

	return ret;
}

/*
Linear interpolation between two HQuats
*/
HQuat HQuat::Qlerp(const HQuat & q1, const HQuat & q2, float t)
{
	HQuat ret = q1 + t * (q2 - q1);
	// return value must be normalized
	return ret.Normalize();
}

/*
Spherical linear interpolation between two HQuats
*/
HQuat HQuat::Qslerp(const HQuat & q1, const HQuat & q2, float t)
{
	HQuat q3;
	float dot;
	dot = q1.x*q2.x + q1.y*q2.y + q1.z*q2.z + q1.w*q2.w;

	/*
	dot = cos(theta)
	if (dot < 0), q1 and q2 are more than 90 degrees apart,
	so we can invert one to reduce spinning
	*/
	if (dot < 0)
	{
		dot = -dot;
		q3 = -1 * q2;
	}
	else
	{
		q3 = q2;
	}

	if (dot < 0.95f)
	{
		float angle = static_cast<float>(H_ACOS(dot));
		float sina = sin(angle);
		float sinat = sin(angle * t);
		float sinaomt = sin(angle * (1 - t));
		return (q1 * sinaomt + q3 * sinat) / sina;
	}
	else //if the angle is small, use linear interpolation
	{
		return Qlerp(q1, q3, t);
	}
}

 


/*
This version of slerp, used by squad, does not check for theta > 90.
*/

HQuat HQuat::QslerpNoInvert(const HQuat & q1, const HQuat & q2, float t)
{
	float dot = q1.x*q2.x + q1.y*q2.y + q1.z*q2.z + q1.w*q2.w;

	if (dot > -0.95f && dot < 0.95f)
	{
		float angle = static_cast<float>(H_ACOS(dot));
		float sina = sin(angle);
		float sinat = sin(angle * t);
		float sinaomt = sin(angle * (1 - t));
		return (q1 * sinaomt + q2 * sinat) / sina;
	}
	else //if the angle is small, use linear interpolation
	{
		return Qlerp(q1, q2, t);
	}
}
HQuat HQuat::QslerpNoInvertExtraSpins(const HQuat & rkP, const HQuat & rkQ, float fT, int iExtraSpins)
{
	float fCos = rkP.x*rkQ.x + rkP.y*rkQ.y + rkP.z*rkQ.z + rkP.w*rkQ.w;
	float fAngle = static_cast<float>(H_ACOS(fCos));

	if (fabs(fAngle) < 0.005f)
		return rkP;
	
	float fSin = sin(fAngle);
	float fInvSin = 1.0f / fSin;
	float fPhase = 3.1415927f * iExtraSpins * fT;
	float fCoeff0 = sin((1.0f - fT) * fAngle - fPhase) * fInvSin;
	float fCoeff1 = sin(fT * fAngle + fPhase) * fInvSin;
	/*
        if (fCos < 0.0f)
        {
            fCoeff0 = -fCoeff0;
            // taking the complement requires renormalisation
            HQuat t(fCoeff0*rkP + fCoeff1*rkQ);
            t.Normalize();
            return t;
        }
		*/
		
	return fCoeff0 * rkP + fCoeff1 * rkQ;
}	




HQuat HQuat::MatrixToQuaternion(float * matrix)
{
	float quaternion[4];

	double tr = matrix[0] + matrix[5] + matrix[10];
	if (tr > 0.0)
	{
		double s = sqrt(tr + 1.0);
		quaternion[3] = (float)s * 0.5f;
		s = 0.5f/(float)s;

		quaternion[0] = (matrix[6] - matrix[9]) * static_cast<float>(s);
		quaternion[1] = (matrix[8] - matrix[2]) * static_cast<float>(s);
		quaternion[2] = (matrix[1] - matrix[4]) * static_cast<float>(s);
	}
	else
	{
		int i, j, k;
		i = 0;
		if (matrix[(1*4)+1] > matrix[(0*4)+0])
			i = 1;
		if (matrix[(2*4)+2] > matrix[(i*4)+i])
			i = 2;

		int nxt[3] = {1,2,0};

		j = nxt[i];
		k = nxt[j];

		double s = sqrt(matrix[(i*4)+i] - (matrix[(j*4)+j] + matrix[(k*4)+k]) + 1.0f);

		quaternion[i] = (float)s * 0.5f;
		s = 0.5f / s;

		quaternion[3] = (matrix[(j*4) + k] - matrix[(k*4) + j]) * static_cast<float>(s);
		quaternion[j] = (matrix[(i*4) + j] + matrix[(j*4) + i]) * static_cast<float>(s);
		quaternion[k] = (matrix[(i*4) + k] + matrix[(k*4) + i]) * static_cast<float>(s);
	}
	return HQuat(quaternion[0], quaternion[1], quaternion[2], quaternion[3]);
}





/*
Spherical cubic interpolation
*/
HQuat HQuat::Qsquad(const HQuat & q1, const HQuat & q2, const HQuat & a, const HQuat & b, float t)
{
	HQuat c = QslerpNoInvert(q1, q2, t);
	HQuat d = QslerpNoInvert(a, b, t);
	return QslerpNoInvert(c, d, 2 * t * (1 - t));
}


/*
Given 3 HQuats, qn-1,qn and qn+1, calculate a control point to be used in spline interpolation
*/
HQuat HQuat::Qspline(const HQuat & qnm1, const HQuat & qn, const HQuat & qnp1)
{
	HQuat qni(-qn.x, -qn.y, -qn.z, qn.w);
	return qn * Qexp((Qlog(qni * qnm1) + Qlog(qni * qnp1)) / -4);
}




void HQuat::ToMatrix(float * matrix)
{	
	float s, xs, ys, zs, wx, wy, wz, xx, xy, xz, yy, yz, zz;

	s = 2.0f / (x*x + y*y + z*z + w*w);

	xs = x*s;	ys = y*s;		zs = z*s;
	wx = w*xs;	wy = w*ys;		wz = w*zs;
	xx = x*xs;	xy = x*ys;		xz = x*zs;
	yy = y*ys;	yz = y*zs;		zz = z*zs;

	matrix[0] = 1.0f - (yy + zz);
	matrix[1] = xy + wz;
	matrix[2] = xz - wy;
	matrix[3] = 0.0f;

	matrix[4] = xy - wz;
	matrix[5] = 1.0f - (xx + zz);
	matrix[6] = yz + wx;
	matrix[7] = 0.0f;

	matrix[8] = xz + wy;
	matrix[9] = yz - wx;
	matrix[10] = 1.0f - (xx + yy);
	matrix[11] = 0.0f;

	matrix[12] = 0.0f;
	matrix[13] = 0.0f;
	matrix[14] = 0.0f;
	matrix[15] = 1.0f;
}

/*
Logarithm of a HQuat, given as:
Qlog(q) = v*a where q = [cos(a),v*sin(a)]
*/

HQuat HQuat::Qlog(const HQuat & q)
{
	float a = static_cast<float>(H_ACOS(q.w));
	float sina = sin(a);
	HQuat ret;
	ret.w = 0;
	if (sina > 0)
	{
		ret.x = a*q.x / sina;
		ret.y = a*q.y / sina;
		ret.z = a*q.z / sina;
	}
	else
	{
		ret.x = ret.y = ret.z = 0;
	}
	return ret;
}

 
void HKeyframeChannelLinear::Interpolate(HKeyframeChannel ** posarray, int keyframe, float fraction, int length, HPoint & res)
{
	UNREFERENCED(length);

	HPoint delta(
		posarray[keyframe+1]->m_cp.x - posarray[keyframe]->m_cp.x,
		posarray[keyframe+1]->m_cp.y - posarray[keyframe]->m_cp.y,
		posarray[keyframe+1]->m_cp.z - posarray[keyframe]->m_cp.z
	);
	res.Set(
		posarray[keyframe]->m_cp.x + delta.x * fraction,
		posarray[keyframe]->m_cp.y + delta.y * fraction,
		posarray[keyframe]->m_cp.z + delta.z * fraction
	);
}

void HKeyframeChannelLinear::Serialize(HUtilityXMLTag * xmlgen)
{
	char text[MVO_BUFFER_SIZE];
	if (m_bEaseInOut)
        HCLOCALE(sprintf(text, "[[ea]%f %f %f]", m_cp.x, m_cp.y, m_cp.z));
	else
	    HCLOCALE(sprintf(text, "[%f %f %f]", m_cp.x, m_cp.y, m_cp.z));
	xmlgen->AddListItem(text);
}

void HKeyframeChannelCurve::Serialize(HUtilityXMLTag * xmlgen)
{
	char text[MVO_BUFFER_SIZE];
	HCLOCALE(sprintf(text, "[b %f %f %f]", m_cp.x, m_cp.y, m_cp.z));
	xmlgen->AddListItem(text);
}


void HKeyframeChannelFollowPath::Serialize(HUtilityXMLTag * xmlgen)
{
	char text[MVO_BUFFER_SIZE];
	if (!m_bLinear)
		HCLOCALE(sprintf(text, "[f %f %f %f]", m_cp.x, m_cp.y, m_cp.z));
	else
		HCLOCALE(sprintf(text, "[fl %f %f %f]", m_cp.x, m_cp.y, m_cp.z));
	xmlgen->AddListItem(text);
}

void HKeyframeChannelFollowPath::Interpolate(HKeyframeChannel ** posarray, int keyframe, float fraction, int length, HPoint & res)
{
	HPoint res2;
	HPoint p0, p1, p2, p3;
	HPoint reslinear;

	m_bNoRotate = false;
	float oldfraction = fraction;
	p1 = posarray[keyframe]->m_cp;
	p2 = posarray[keyframe+1]->m_cp;
	if (p1 == p2)
		m_bNoRotate = true;

	if (m_bLinear)
	{
		HPoint delta;
		delta.Set(
			posarray[keyframe+1]->m_cp.x - posarray[keyframe]->m_cp.x,
			posarray[keyframe+1]->m_cp.y - posarray[keyframe]->m_cp.y,
			posarray[keyframe+1]->m_cp.z - posarray[keyframe]->m_cp.z
		);
		reslinear.Set(
			posarray[keyframe]->m_cp.x + delta.x * fraction,
			posarray[keyframe]->m_cp.y + delta.y * fraction,
			posarray[keyframe]->m_cp.z + delta.z * fraction
		);
	}

	if (keyframe > 0)
		p0 = posarray[keyframe - 1]->m_cp;
	else
		p0 = p1;
	if (keyframe < length - 2)
		p3 = posarray[keyframe + 2]->m_cp;
	else
		p3 = p2;

	HPoint res_sav;
	m_factor1 = 1.0f;
	m_factor2 = 1.0f;
	CalculateHermiteTangents(p0, p2, p3);
	res = InterpolateHermiteSpline(fraction, p1, p2);

	fraction += 0.01f;
	if (fraction > 1.0f)
	{
		fraction = fraction - 1.0f;
		++keyframe;
		if (keyframe >= length - 1)
			keyframe = 0;
	}

	p1 = posarray[keyframe]->m_cp;
	p2 = posarray[keyframe+1]->m_cp;
	if (keyframe > 0)
		p0 = posarray[keyframe - 1]->m_cp;
	else
		p0 = p1;
	if (keyframe < length - 2)
		p3 = posarray[keyframe + 2]->m_cp;
	else
		p3 = p2;

	CalculateHermiteTangents(p0, p2, p3);
	res2 = InterpolateHermiteSpline(fraction, p1, p2);	

	HPoint vec(res2.x - res.x, res2.y - res.y, res2.z - res.z);
	HPoint px(0,1,0);
	float result1[16];
	float result2[16];
	HC_Compute_Normalized_Vector(&vec, &vec);
	HUtility::ComputeVectorToVectorRotationMatrix(result1, vec, px);


	float ang;
	//HC_Compute_Offaxis_Rotation(vec.x, vec.y , vec.z, 45, matrix);
	{
		HPoint p1, p2, p3;
		p1.Set(result1[8], result1[9], result1[10]);
		p1.Set(0, 0, 1);

		HC_Compute_Transformed_Points(1, &p1, result1, &p1);
		p1.y = 0;
		HC_Compute_Normalized_Vector(&p1, &p1);
		p2.Set(0, 0, 1);

		HC_Compute_Normalized_Vector(&p1, &p1);
		HC_Compute_Normalized_Vector(&p2, &p2);
		HC_Compute_Cross_Product(&p1, &p2, &p3);

		float dprod = (float)HC_Compute_Dot_Product(&p1, &p2);
		float l = static_cast<float>(HC_Compute_Vector_Length(&p3));
		//float ang2 = (float)H_ASIN(l);
		ang = (float)atan2(l, dprod);
		ang *= 180.0f / 3.1415927f;
	}

	HPoint rr(result1[4], result1[5], result1[6]);
	HC_Compute_Normalized_Vector(&rr, &rr);
	HC_Compute_Offaxis_Rotation(rr.x, rr.y, rr.z, -ang, result2);

	//HUtility::ComputeVectorToVectorRotationMatrix(rmatrix2,vec2,px);
	HC_Compute_Matrix_Product(result1, result2, tmatrix);
	HPoint ttx(1,0,0);
	HPoint ttx2(1,0,0);
	HC_Compute_Transformed_Points(1, &ttx, tmatrix, &ttx);
	ang = -ang;
	HC_Compute_Offaxis_Rotation(rr.x, rr.y, rr.z, -ang, result2);
	HC_Compute_Matrix_Product(result1, result2, tmatrix);
	HC_Compute_Transformed_Points(1, &ttx2, tmatrix, &ttx2);
	if (ttx.z < 0)
		ttx.z = -ttx.z;
	if (ttx2.z < 0)
		ttx2.z = -ttx2.z;
	if (ttx2.z >= ttx.z)
	{
		ang = -ang;
		HC_Compute_Offaxis_Rotation(result1[4], result1[5], result1[6], -ang, result2);
		HC_Compute_Matrix_Product(result1, result2, tmatrix);
	}

	fraction = oldfraction;
	if (m_bLinear)
	{
		res = reslinear;
	}

	/*
	HC_Open_Segment("/include library/model_0/lines");
	HC_Set_Visibility("lines = on");
	HC_Insert_Line(0,0,0,vec.x, vec.y, vec.z);
	HC_Close_Segment();
	*/
}




void HKeyframeChannelCurve::Interpolate(HKeyframeChannel ** posarray, int keyframe, float fraction, int length, HPoint & res)
{	
	HPoint p0;
	HPoint p3;
	HPoint p1 = posarray[keyframe]->m_cp;
	HPoint p2 = posarray[keyframe+1]->m_cp;
	if (keyframe > 0)
		p0 = posarray[keyframe - 1]->m_cp;
	else
		p0 = p1;
	if (keyframe < length - 2)
		p3 = posarray[keyframe + 2]->m_cp;
	else
		p3 = p2;

	//CalculateHermiteTangents(p0, p2, p3);
	res = InterpolateHermiteSpline(fraction, p1, p2);
}


void HKeyframeChannelCurve::DrawTangents(HPoint p2)
{
	HC_Insert_Line(m_cp.x, m_cp.y, m_cp.z, m_cp.x + m_tangent1.x, m_cp.y + m_tangent1.y, m_cp.z + m_tangent1.z);
	HC_Insert_Marker(m_cp.x + m_tangent1.x, m_cp.y + m_tangent1.y, m_cp.z + m_tangent1.z);
 
	HC_Open_Segment("m2");{
		HC_Set_Color("lines = yellow, markers = blue");
		HC_Set_Marker_Symbol("*");
		HC_Insert_Line(
			p2.x, p2.y, p2.z,
			p2.x + m_tangent2.x, p2.y + m_tangent2.y, p2.z + m_tangent2.z
		);
		HC_Insert_Marker(m_tangent2.x + p2.x , m_tangent2.y + p2.y , m_tangent2.z + p2.z);
	}HC_Close_Segment();
}

void HKeyframeChannelCurve::CalculateCurveFactor(HBhvTimeline * tl, int keyframe)
{
	int nm1, nm0, np1;

	int * tlarray = tl->GetTimelineArray();
	int length = tl->GetTimelineArrayLength();
	if (keyframe > 0 && keyframe < length)
		nm1 = tlarray[keyframe] - tlarray[keyframe - 1];
	else
		nm1 = 0;

	if (keyframe < length - 1)
		nm0 = tlarray[keyframe + 1] - tlarray[keyframe];
	else 
		nm0 = 0;

	if (keyframe < length - 2)
		np1 = tlarray[keyframe + 2] - tlarray[keyframe + 1];
	else 
		np1 = 0;
	m_factor1 = (2.0f * nm0)/ (nm1 + nm0);	
	m_factor2 = (2.0f * nm0) / (nm0 + np1);
}

void HKeyframeChannelCurve::CalculateHermiteTangents(HPoint p0, HPoint p2, HPoint p3, float a)
{
	HPoint p1 = m_cp;
	a = 0.5f;
	m_tangent1.Set(a * (p2.x - p0.x),a * (p2.y - p0.y), a * (p2.z - p0.z));
	m_tangent2.Set(a * (p3.x - p1.x),a * (p3.y - p1.y), a * (p3.z - p1.z));

	//float factor = (2.0f * nm0)/ (nm1 + nm0);
	m_tangent1.Set(m_factor1 * m_tangent1.x, m_factor1 * m_tangent1.y, m_factor1 * m_tangent1.z);
	//factor = (2.0f * nm0) / (nm0 + np1);
	m_tangent2.Set(m_factor2 * m_tangent2.x, m_factor2 * m_tangent2.y, m_factor2 * m_tangent2.z);
}



HPoint HKeyframeChannelCurve::InterpolateHermiteSpline(float fraction, HPoint p1, HPoint p2)
{
	float s = fraction;
	float s2 = fraction * fraction;
	float s3 = fraction * fraction * fraction;

	float h1 = 2 * s3 - 3 *s2 + 1;
	float h2 = -2 * s3 + 3 * s2;
	float h3 = s3 - 2 * s2 + s;
	float h4 = s3 - s2;
	return HPoint(
		h1 * p1.x + h2 * p2.x + h3 * m_tangent1.x + h4 * m_tangent2.x,
		h1 * p1.y + h2 * p2.y + h3 * m_tangent1.y + h4 * m_tangent2.y,
		h1 * p1.z + h2 * p2.z + h3 * m_tangent1.z + h4 * m_tangent2.z
	);
}
/*

HPoint HKeyframeChannelCurve::InterpolateBezierSpline(float fraction, HPoint p2)
{

	HPoint p1 = m_cp, res;
	float s = fraction;
	float s2 = fraction * fraction;
	float s3 = fraction * fraction * fraction;
 
	float h1 = -1 * s3  + 3 *s2 -3 * s + 1;
	float h2 = 3 * s3   - 6 * s2 + 3 *s;
	float h3 = -3 * s3  + 3 * s2;
	float h4 = s3;

	res.Set(h1 * p1.x + h2 * m_tangent1.x + h3 * m_tangent2.x + h4 * p2.x,
		h1 * p1.y + h2 * m_tangent1.y + h3 * m_tangent2.y + h4 * p2.y,
		h1 * p1.z + h2 * m_tangent1.z + h3 * m_tangent2.z + h4 * p2.z);
	return res;
}
*/

void HKeyframeChannelCurve::CalculateCurveLength(HPoint p2)
{
	float adder = 1.0f / 100.0f;
	float fraction = 0.0f;
	HPoint oldpoint = m_cp;
	m_curvelength = 0.0f;
	for (int i = 0; i < 100; fraction += adder, ++i)
	{
		HPoint res = InterpolateHermiteSpline(fraction,m_cp,  p2);
		HPoint delta(res.x - oldpoint.x, res.y - oldpoint.y, res.z - oldpoint.z);
		m_curvelength += static_cast<float>(HC_Compute_Vector_Length(&delta));
		oldpoint = res;
	}
}

 
void HKeyframeMatrix::Serialize(HUtilityXMLTag * xmlgen)
{
	char text[MVO_BUFFER_SIZE];

 	HCLOCALE(sprintf(text, "[%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f]",
		m_matrix[ 0], m_matrix[ 1], m_matrix[ 2], m_matrix[ 3],
		m_matrix[ 4], m_matrix[ 5], m_matrix[ 6], m_matrix[ 7],
		m_matrix[ 8], m_matrix[ 9], m_matrix[10], m_matrix[11],
		m_matrix[12], m_matrix[13], m_matrix[14], m_matrix[15]
	));
 	xmlgen->AddListItem(text);
}
 
 
 
void HKeyframeQuatSquad::Serialize(HUtilityXMLTag * xmlgen)
{
	char text[MVO_BUFFER_SIZE];

	char esptext[MVO_BUFFER_SIZE];
	if (m_ExtraSpins)
		HCLOCALE(sprintf(esptext, "[es %d]", m_ExtraSpins));
	else
		HCLOCALE(sprintf(esptext, "%s", ""));


	if (m_bLinear)
		HCLOCALE(sprintf(text, "[%s l %f %f %f %f]", esptext, m_quat.x, m_quat.y, m_quat.z, m_quat.w));
	else
		HCLOCALE(sprintf(text, "[%s %f %f %f %f]", esptext, m_quat.x, m_quat.y, m_quat.z, m_quat.w));
	xmlgen->AddListItem(text);
}


void HKeyframeAxisRotation::Serialize(HUtilityXMLTag * xmlgen)
{
	char text[MVO_BUFFER_SIZE];
	HCLOCALE(sprintf(text, "%f", m_angle));
	xmlgen->AddListItem(text);
}


void HKeyframeChannelDiscrete::Interpolate(HKeyframeChannel ** posarray, int keyframe, float fraction, int length, HPoint & res)
{
	UNREFERENCED(length);

 	if (fraction < 1.0f)
		res.Set(posarray[keyframe]->m_cp);
	else
		res.Set(posarray[keyframe+1]->m_cp);

}


void HKeyframeChannelDiscrete::Serialize(HUtilityXMLTag * xmlgen)
{
	char text[MVO_BUFFER_SIZE];
	HCLOCALE(sprintf(text, "[d %f %f %f]", m_cp.x, m_cp.y, m_cp.z));
	xmlgen->AddListItem(text);
}



HKeyframeString::HKeyframeString()
{
	strcpy(m_target, "");
	m_type =  HANIString;
	m_bDiscrete = false;
}

/*
HKeyframe& HKeyframeString::operator=(const HKeyframe &kfp) 
{
	if (this != &kfp)
	{
		if (kfp.m_type == HANIString)
			strcpy(m_target, ((HKeyframeString &)kfp).m_target);
	}
	return *this;
}
*/

void HKeyframeString::SetTarget(const char * target) 
{ 
	strcpy(m_target, target); 
}

void HKeyframeString::Serialize(HUtilityXMLTag * xmlgen)
{
	char result[MVO_BUFFER_SIZE];
	HBhvTargetObject::CreateTargetString("SPATH", m_target, result, true);

	if (!m_bDiscrete)
 		xmlgen->AddListItem(result);
	else
	{
		char res2[4096];
		sprintf(res2, "[ d %s]",result);
 		xmlgen->AddListItem(res2);
	}
}







HKeyframe3String::HKeyframe3String()
{
	strcpy(m_t1, "");
	strcpy(m_t2, "");
	strcpy(m_t3, "");
	m_type =  HANI3String;
}

 

void HKeyframe3String::SetTarget(const char * t1,const char * t2,const char * t3) 
{ 
	strcpy(m_t1, t1); 
	strcpy(m_t2, t2); 
	strcpy(m_t3, t3); 
}

void HKeyframe3String::Serialize(HUtilityXMLTag * xmlgen)
{
	char result[MVO_BUFFER_SIZE];
	char temp[MVO_BUFFER_SIZE];
	char temp2[MVO_BUFFER_SIZE];
	HBhvTargetObject::CreateTargetString("SPATH", m_t1, temp, false);
	HBhvTargetObject::CreateTargetString("SPATH", m_t2, temp2, false);
	sprintf(result, "\"%s$$%s$$%s\"", temp, temp2, m_t3);
 	xmlgen->AddListItem(result);
}










void HBhvUtility::GetQuatPoint(HUtilityTextParser * tp, HQuat & quat)
{
	static delimiter_t number_td[] = {{"\x30-\x39|-|", false, " |]", true, false}};
 
	char result[MVO_BUFFER_SIZE];
	int dummy;
	tp->ParseBuffer(result, dummy, number_td, 1);
	HCLOCALE(quat.x = (float)atof(result));
	tp->ParseBuffer(result, dummy, number_td, 1);
	HCLOCALE(quat.y = (float)atof(result));
	tp->ParseBuffer(result, dummy, number_td, 1);
	HCLOCALE(quat.z = (float)atof(result));
	tp->ParseBuffer(result, dummy, number_td, 1);
	HCLOCALE(quat.w = (float)atof(result));
}

void HBhvUtility::GetMatrix(HUtilityTextParser * tp, float * matrix)
{
	static delimiter_t number_td[] = {{"\x30-\x39|-|", false, " |]", true, false}};
 
	char result[MVO_BUFFER_SIZE];
	for (int i = 0; i < 16; i++)
	{	
		int dummy;
		tp->ParseBuffer(result, dummy, number_td, 1);
		HCLOCALE(matrix[i] = (float)atof(result));
	}
}

void HBhvUtility::GetMatrix(const char * text, float * matrix)
{
	HUtilityTextParser tp;
 	tp.InsertBuffer(text, (unsigned int)strlen(text));
	if (text[0] == '[')
 		tp.SkipAhead(1); 
	GetMatrix(&tp, matrix);
}


void HBhvUtility::GetQuatPoint(const char *text, HQuat &quat)
{
	HUtilityTextParser tp;
 	tp.InsertBuffer(text, (unsigned int)strlen(text));
	if (text[0] == '[')
 		tp.SkipAhead(1); 
	GetQuatPoint(&tp, quat);
}

void UnSetAttributes(HC_KEY key)
{
	HC_Open_Segment_By_Key(key);{
		char segment[MVO_SEGMENT_PATHNAME_BUFFER];
		char segment2[MVO_SEGMENT_PATHNAME_BUFFER];

		HC_Show_Segment(key, segment);
		HC_Parse_String(segment, "/", -1, segment2);

		if (strcmp(segment2, "triad") != 0 && strcmp(segment2, "legend") != 0 && strcmp(segment2, "title") != 0)
		{
			if (HC_Show_Existence("camera"))
				HC_UnSet_Camera();
			if (HC_Show_Existence("visibility"))
				HC_UnSet_Visibility();
			if (HC_Show_Existence("window"))
				HC_UnSet_Window();
			if (HC_Show_Existence("rendering options"))
				HC_UnSet_Rendering_Options();
		}
	}HC_Close_Segment();
}

int HBhvUtility::searchcounter = 0;

ShellMorphInfo::ShellMorphInfo()
{
	arrays_list = new_vlist(malloc, free);
	original_key = INVALID_KEY;
	temp_key = INVALID_KEY;
};


void HBhvUtility::AddShellToMorphList(HC_KEY key, struct vlist_s * morph_data_list)
{
	vlist_reset_cursor(morph_data_list);
	for (int i = 0; i < searchcounter; i++)
   		vlist_advance_cursor(morph_data_list);	
	ShellMorphInfo * temp;
	if ((temp = static_cast<ShellMorphInfo *>(vlist_peek_cursor(morph_data_list))) == 0)
	{
 		temp = new ShellMorphInfo();
		temp->original_key = key;
		vlist_add_last(morph_data_list, temp);
	}
	int plength, flength;
	HPoint * points;
	HC_Show_Shell_Size(key, &plength, &flength);
	points = new HPoint[plength];
	HC_Show_Shell(key, &plength, points, &flength, 0);
	vlist_add_last(temp->arrays_list, points);
	temp->temp_key = key;
}
 

			

void HBhvUtility::CollectShellMorphData(HC_KEY key, struct vlist_s * morp_data_list)
{
	char pathname[MVO_SEGMENT_PATHNAME_BUFFER];
	HC_Open_Segment_By_Key(key);{
		HC_Show_Segment(key, pathname);
		HC_Begin_Contents_Search("...", "includes, shells, segments");{
			HC_KEY foundkey;
			char type[MVO_BUFFER_SIZE];
			while (HC_Find_Contents(type, &foundkey))
			{
				if (strcmp(type, "shell") == 0)
				{
					if (!KeyExists(foundkey, morp_data_list))
					{
						AddShellToMorphList(foundkey, morp_data_list);
						searchcounter++;
					}
				}
				else if (strcmp(type, "include") == 0)
					CollectShellMorphData(HC_KShow_Include_Segment(foundkey), morp_data_list);
				else
				{
					UnSetAttributes(foundkey);
				}
			}
		}HC_End_Contents_Search();
	}HC_Close_Segment();
}


bool HBhvUtility::KeyExists(HC_KEY key, struct vlist_s * morp_data_list)
{

	START_LIST_ITERATION(ShellMorphInfo, morp_data_list);{
		if (temp->temp_key == key)
			return true;
	}END_LIST_ITERATION(morp_data_list);
	return false;
}

void HBhvUtility::ResetMorphData(struct vlist_s *morp_data_list)
{

	searchcounter = 0;
	START_LIST_ITERATION(ShellMorphInfo, morp_data_list);{
		temp->temp_key = INVALID_KEY;
	}END_LIST_ITERATION(morp_data_list);
}
void CalculateNumberedFilename(const char *pathname, char *buffer, int num)
{
		char currentfilename[MVO_BUFFER_SIZE];
		strcpy(currentfilename, pathname);
		char ext[MVO_SMALL_BUFFER_SIZE];
		HUtility::FindFileNameExtension(pathname, ext);
		currentfilename[strlen(currentfilename) - 4] = 0;
 		sprintf(buffer, "%s%d.%s", currentfilename, num, ext);
}


void HBhvUtility::ConvertHSFtoMorph(const char *filename, HBaseModel *model)
{
	struct vlist_s*		ShellMorphInfoList;
	ShellMorphInfoList = new_vlist(malloc, free);

	for (int i=0;i<10;i++)	
	{

		model->Flush();
		char currentfilename[MVO_BUFFER_SIZE];
		CalculateNumberedFilename(filename, currentfilename, i+1);
  		model->Read(currentfilename);
		ResetMorphData(ShellMorphInfoList);
 		CollectShellMorphData(model->GetModelKey(), ShellMorphInfoList);
		UnSetAttributes(model->GetModelKey());

	}
}

 
void HBhvUtility::ClearAllAttributes(HC_KEY key)
{
 	HC_Open_Segment_By_Key(key);
 	HC_Begin_Contents_Search("...", "includes, segments");
	HC_KEY foundkey;
	char type[MVO_BUFFER_SIZE];
	while (HC_Find_Contents(type, &foundkey))
	{
		if (strcmp(type, "include") == 0)
			ClearAllAttributes(HC_KShow_Include_Segment(foundkey));
		else
			UnSetAttributes(foundkey);
	}
	HC_Close_Segment();
	HC_End_Contents_Search();
}



void HBhvUtility::Move_Segments_To_Segment(HC_KEY target, HC_KEY destination)
{
	char child_segment_name[MVO_SEGMENT_PATHNAME_BUFFER];
	char child_segment_name_simple[MVO_SEGMENT_PATHNAME_BUFFER];
	HC_Open_Segment_By_Key(target);

		HC_Begin_Segment_Search("*");
		while (HC_Find_Segment(child_segment_name))
		{
			HC_Parse_String(child_segment_name,"/",-1,child_segment_name_simple);
 			if (!memcmp(child_segment_name_simple, "ao", strlen("ao"))==0)
			{
				HC_Open_Segment_By_Key(destination);
					HC_Rename_Segment(child_segment_name,"./*");
				HC_Close_Segment();
			}
		}
		HC_End_Segment_Search();
	HC_Close_Segment();
}


void HBhvUtility::ConvertHSFtoSegSwitch(const char *filename, HBaseModel *model)
{
	model->Flush();
	int i;

	for (i=1;i<53;i++)	
	{

 		char currentfilename[MVO_BUFFER_SIZE];
		CalculateNumberedFilename(filename, currentfilename, i);
  		model->Read(currentfilename);
		char framename[MVO_SMALL_BUFFER_SIZE];
		sprintf(framename, "frame%3d", i);
		ClearAllAttributes(model->GetModelKey());
		UnSetAttributes(model->GetModelKey());
		HC_Open_Segment_By_Key(model->GetModelKey());
		HC_Open_Segment("ao");
		HC_KEY destination = HC_KCreate_Segment(framename);
		HC_Close_Segment();
		HC_Close_Segment();
		Move_Segments_To_Segment(model->GetModelKey(), destination);

		ClearAllAttributes(model->GetModelIncludeKey());
		UnSetAttributes(model->GetModelIncludeKey());
		HC_Open_Segment_By_Key(model->GetModelIncludeKey());
		HC_Open_Segment("ao");
		destination = HC_KOpen_Segment(framename);
		HC_Set_Visibility("everything = off");
		HC_Close_Segment();
		HC_Close_Segment();
		HC_Close_Segment();
		Move_Segments_To_Segment(model->GetModelIncludeKey(), destination);
	 

	}
		HBhvBehaviorManager *pBehaviorManager = model->GetBhvBehaviorManager();
		char targetname[MVO_BUFFER_SIZE];
		char objectname[MVO_BUFFER_SIZE];
 		HBhvTargetObject::CreateTargetString("SPATH", "MODEL", targetname);
 		sprintf(objectname, "%d", pBehaviorManager->GetFreeName());

 		HBhvInterpolatorSegSwitch *iss = new HBhvInterpolatorSegSwitch(0, objectname);
		HBhvTimeline *t = new HBhvTimeline(0);
		HBhvAnimation *animation = pBehaviorManager->AddAnimation(objectname, targetname, t, iss);			
	
 
	for (i=1;i<53;i++)	
	{

 		char currentfilename[MVO_BUFFER_SIZE];
		CalculateNumberedFilename(filename, currentfilename, i);
 		char framename[MVO_SMALL_BUFFER_SIZE];
		sprintf(framename, "frame%3d", i);
 		HC_Open_Segment_By_Key(model->GetModelKey());
		HC_Open_Segment("ao");
		HC_KEY k = HC_KOpen_Segment(framename);
 		HC_Set_Visibility("everything = off");
		HC_Set_Rendering_Options("attribute lock = (visibility)");
		HC_Close_Segment();
		HC_Close_Segment();
		HC_Close_Segment();
		HC_Show_Segment(k, targetname);
   		animation->GetTimeline()->AddKeyframe(i-1);		
		((HBhvInterpolatorSegSwitch *)(animation->GetInterpolator()))->Insert(targetname, i-1);

 

	}
}



void HBhvUtility::SetupAnimationWriteFromHSF(HStreamFileToolkit *mytool, HBaseModel *model)
{
 	mytool->SetPostwalkHandler (new TK_Bhv_XML (model));
}

void HBhvUtility::SetupAnimationReadFromHSF(HStreamFileToolkit *mytool, HBaseModel *model)
{
	UNREFERENCED(mytool);

 	model->GetDispatchXML()->AddHandler(new TK_Bhv_XML (model));
}

void HBhvUtility::ShowModellingMatrix(float *segment_modelling_matrix)
{
 		
		if (HC_Show_Existence("modelling matrix"))
				HC_Show_Modelling_Matrix(segment_modelling_matrix);
		else
				HC_Compute_Identity_Matrix(segment_modelling_matrix); 
}


 

TK_Status	TK_Bhv_XML::Interpret(BStreamFileToolkit &tk, ID_Key key, int variant) 
{
	UNREFERENCED(tk);
	UNREFERENCED(key);
	UNREFERENCED(variant);

	HUtilityXMLGenerator xmlgen;
	xmlgen.Reset();
	m_pModel->GetBhvBehaviorManager()->Serialize(&xmlgen);
	char *temp = new char [xmlgen.GetBufferSize()];
	memcpy(temp, xmlgen.GetBuffer(), xmlgen.GetBufferSize());
	SetXML (xmlgen.GetBufferSize(), temp);
	delete [] temp;
    return TK_Normal;
}
 
TK_Status   TK_Bhv_XML::Execute (BStreamFileToolkit & tk) 
{
	UNREFERENCED(tk);

	char *tempbuf = new char[GetSize() + 1];
	memcpy(tempbuf, GetXML(), GetSize());
	tempbuf[GetSize()] = 0;
 	if (strstr(tempbuf,"<Animdef") != 0)
	{
	    m_pModel->GetDispatchXML()->StopIteration();
	    HBhvBehaviorManager::ProcessXMLData(m_pModel, tempbuf);
	}
 	delete [] tempbuf;
    return TK_Normal;
}

static void GetMatrixFromCamera(HPoint position, HPoint target, HPoint up_vector, float *matrix)
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
    
    matrix[12] = 0;
    matrix[13] = 0;
    matrix[14] = 0;
    matrix[15] = 1.0f;
}

HBhvAnimation * HBhvUtility::AddAnimation(HBaseModel *model, char *name, char *targetpath, HPoint *pivot)
{
 	HBhvBehaviorManager *pBehaviorManager = model->GetBhvBehaviorManager();
	HBhvAnimation *ani = new HBhvAnimation(name, pBehaviorManager, 0);
 	if (targetpath)
	{
		ani->SetTargetByPath(targetpath);
		if (pivot)
			ani->GetTarget()->SetPivot(pivot->x, pivot->y, pivot->z);
	}
	pBehaviorManager->AddAnimation(ani);
	return ani;
}


void HBhvUtility::AddPosRotKeyframe(HBaseModel *model, const char *animname, int tick, HPoint midpoint, float *matrix)
{
		midpoint.Set(-midpoint.x, -midpoint.y, -midpoint.z);
		HPoint p(midpoint), p2(0,0,0);
		HC_Compute_Transformed_Points(1, &p, matrix, &p2);
		p = p2 - p;

		float fquat[4];
		HUtility::MatrixToQuaternion(matrix, fquat);
		HQuat quat(fquat[0], fquat[1], fquat[2], fquat[3]);
		AddPosRotKeyframe(model, (char *)animname, tick, p, true, quat, true);
		//AddPosKeyframe(view, (char *)animname, tick, p, true);
	//	AddRotKeyframe(view, (char *)animname, tick, quat, true);
		
	
}

void HBhvUtility::AddPosKeyframe(HBaseModel *model, char *animname, int tick, HPoint pos, bool linear)
{
 	HBhvBehaviorManager *pBehaviorManager = model->GetBhvBehaviorManager();
	HBhvAnimation *ani = pBehaviorManager->FindAnimationByName(animname);

	HBhvInterpolatorPosition * interpol= (HBhvInterpolatorPosition *)ani->GetInterpolator();

	if (!interpol)
	{
		HBhvTimeline *t = new HBhvTimeline();
 		ani->SetTimeline(t);
		interpol = new HBhvInterpolatorPosition();
		ani->AddInterpolator(interpol);
	}
	bool replace;
	int l = ani->GetTimeline()->AddKeyframe(tick, replace) - 1;
	if (l < 0)
		l = 0;
	if (!linear)
		((HBhvInterpolatorPosition *)(ani->GetInterpolator()))->InsertCurve(pos, l);
	else
		((HBhvInterpolatorPosition *)(ani->GetInterpolator()))->InsertLinear(pos, l);

}


void HBhvUtility::AddInstanceCreateKeyframe(HBaseModel *model, char *animname, int tick, char *path, char *include_path, char *color)
{
 	HBhvBehaviorManager *pBehaviorManager = model->GetBhvBehaviorManager();
	HBhvAnimation *ani = pBehaviorManager->FindAnimationByName(animname);

	HBhvInterpolatorInstanceCreate * interpol= (HBhvInterpolatorInstanceCreate *)ani->GetInterpolator();

	if (!interpol)
	{
		HBhvTimeline *t = new HBhvTimeline();
 		ani->SetTimeline(t);
		interpol = new HBhvInterpolatorInstanceCreate();
		ani->AddInterpolator(interpol);
	}
	bool replace;
	int l = ani->GetTimeline()->AddKeyframe(tick, replace);

	((HBhvInterpolatorInstanceCreate *)(ani->GetInterpolator()))->Insert(path, include_path, color, l-1);
 
}

void HBhvUtility::AddSegmentMoveKeyframe(HBaseModel *model, HBhvAnimation *ani, int tick, char *path)
{
	UNREFERENCED(model);
  
	HBhvInterpolatorSegMove * interpol= (HBhvInterpolatorSegMove *)ani->GetInterpolator();

	if (!interpol)
	{
		HBhvTimeline *t = new HBhvTimeline();
 		ani->SetTimeline(t);
		interpol = new HBhvInterpolatorSegMove();
		ani->AddInterpolator(interpol);
	}
	bool replace;
	int l = ani->GetTimeline()->AddKeyframe(tick, replace);

	((HBhvInterpolatorSegMove *)(ani->GetInterpolator()))->Insert(path, l);
 
}
void HBhvUtility::AddMatrixKeyframe(HBaseModel *model, HBhvAnimation *ani, int tick, float *matrix)
{
	UNREFERENCED(model);
  
	HBhvInterpolatorMatrix * interpol= (HBhvInterpolatorMatrix *)ani->GetInterpolator();

	if (!interpol)
	{
		HBhvTimeline *t = new HBhvTimeline();
 		ani->SetTimeline(t);
		interpol = new HBhvInterpolatorMatrix();
		ani->AddInterpolator(interpol);
	}
	bool replace;
	int l = ani->GetTimeline()->AddKeyframe(tick, replace);

	((HBhvInterpolatorMatrix *)(ani->GetInterpolator()))->Insert(matrix, l);
 
}



void HBhvUtility::AddPosRotKeyframe(HBaseModel *model, char *animname, int tick, HPoint pos, bool linear, HQuat quat, bool linear2)
{
 	HBhvBehaviorManager *pBehaviorManager = model->GetBhvBehaviorManager();
	HBhvAnimation *ani = pBehaviorManager->FindAnimationByName(animname);

	HBhvInterpolatorPosition * interpol = 0;
	HBhvInterpolatorQuatSquad * interpolqs = 0;
 
	vlist_s *il = ani->GetInterpolatorList();
	START_LIST_ITERATION(HBhvInterpolator, il);
		if (strcmp(temp->GetType(), "Pos") == 0)
		{
			interpol = (HBhvInterpolatorPosition *)temp;
		}
		if (strcmp(temp->GetType(), "QuatRot") == 0)
		{
			interpolqs = (HBhvInterpolatorQuatSquad *)temp;
		}
	END_LIST_ITERATION(il);

	if (!interpol)
	{
		HBhvTimeline *t = new HBhvTimeline();
 		ani->SetTimeline(t);
		interpol = new HBhvInterpolatorPosition();
 		ani->AddInterpolator(interpol);
	}
	if (!interpolqs)
	{	 
		interpolqs = new HBhvInterpolatorQuatSquad();
 		ani->AddInterpolator(interpolqs);
	}
 	bool replace;
	int l = ani->GetTimeline()->AddKeyframe(tick, replace);
	if (l<=0)
		l=0;
	if (!linear)
		((HBhvInterpolatorPosition *)(interpol))->InsertCurve(pos, l);
	else
		((HBhvInterpolatorPosition *)(interpol))->InsertLinear(pos, l);
 	if (!linear2)
		((HBhvInterpolatorQuatSquad *)(interpolqs))->Insert(quat, l);
	else
		((HBhvInterpolatorQuatSquad *)(interpolqs))->InsertLinear(quat, l);

 }


void HBhvUtility::AddRotKeyframe(HBaseModel *model, char *animname, int tick, HQuat pos, bool linear)
{
 	HBhvBehaviorManager *pBehaviorManager = model->GetBhvBehaviorManager();
	HBhvAnimation *ani = pBehaviorManager->FindAnimationByName(animname);

	HBhvInterpolatorQuatSquad * interpol= (HBhvInterpolatorQuatSquad *)ani->GetInterpolator();

	if (!interpol)
	{
		HBhvTimeline *t = new HBhvTimeline();
 		ani->SetTimeline(t);
		interpol = new HBhvInterpolatorQuatSquad();
		ani->AddInterpolator(interpol);
	}
	bool replace;
	int l = ani->GetTimeline()->AddKeyframe(tick, replace) - 1;
	if (l < 0)
		l = 0;
	if (!linear)
		((HBhvInterpolatorQuatSquad *)(ani->GetInterpolator()))->Insert(pos, l);
	else
		((HBhvInterpolatorQuatSquad *)(ani->GetInterpolator()))->InsertLinear(pos, l);

}


void HBhvUtility::MakeCameraKeyframe(HBaseView *view, float timediff, bool targetmode) 
{
	HPoint p,t,u;
	float fx, fy;
	char proj[4096];
  	HC_Open_Segment_By_Key(view->GetSceneKey());		
		HC_Show_Net_Camera_Position(&p.x, &p.y, &p.z);
 		HC_Show_Net_Camera_Target(&t.x, &t.y, &t.z);
		HC_Show_Net_Camera_Up_Vector(&u.x, &u.y, &u.z);
		HC_Show_Net_Camera_Field(&fx,&fy);	
		HC_Show_Net_Camera_Projection(proj);
		HC_Close_Segment();
	HPoint p2, t2, u2;
 
	char targetname[MVO_BUFFER_SIZE];	
	char ipc[256];
 	HBhvBehaviorManager *pBehaviorManager = view->GetModel()->GetBhvBehaviorManager();
	sprintf(ipc, "%d", pBehaviorManager->GetFreeName());
	pBehaviorManager->SetTicksPerSecond(100);

	HC_Show_Segment(view->GetSceneKey(), targetname);
	HBhvTargetObject::CreateTargetString("SPATH", targetname, targetname);
	if (targetmode)
		strcat(targetname,"/TARGET");
	else
		strcat(targetname,"/POSITION");
 	
 	HBhvAnimation *animation = pBehaviorManager->FindAnimation(targetname, "QuatRot");
	float matrix[16];
	float fquat[4];
	HQuat quat;
	if (animation == 0)
	{
		HBhvInterpolatorQuatSquad *i = new HBhvInterpolatorQuatSquad(0, ipc);
		HQuat parray(0,0,0,1);
 		int tline = 0;
 	 	i->Insert(parray, 0);
  		HBhvTimeline *t = new HBhvTimeline(0);
		t->SetTimeline(&tline , 1);
		animation = pBehaviorManager->AddAnimation(ipc, targetname, t, i);
 		animation->GetTarget()->SetPivot(0,0,0);
	}

	bool replace;
	int pos;
	
	pos = animation->GetTimeline()->AddKeyframe((int)pBehaviorManager->GetCurrentTick(), replace);
 
	HPoint translation;
	translation.Set(0,0,0);
 	GetMatrixFromCamera(p, t, u, matrix);
	HUtility::MatrixToQuaternion(matrix, fquat);
	quat.Set(fquat[0], fquat[1], fquat[2], fquat[3]);

 	if (replace)
		((HBhvInterpolatorQuatSquad *)(animation->GetInterpolator()))->ReplaceLinear(quat, pos);
	else
		((HBhvInterpolatorQuatSquad *)(animation->GetInterpolator()))->InsertLinear(quat, pos);


	animation = pBehaviorManager->FindAnimation(targetname, "Pos");
	sprintf(ipc, "%d", pBehaviorManager->GetFreeName());
	if (animation == 0)
	{
		HBhvInterpolatorPosition *i = new HBhvInterpolatorPosition(0, ipc);
		HPoint parray(0,0,0);
 		int tline = 0;
 	 	i->InsertCurve(parray);
//		(parray, 1);
 		HBhvTimeline *t = new HBhvTimeline(0);
		t->SetTimeline(&tline , 1);
		animation = pBehaviorManager->AddAnimation(ipc, targetname, t, i);
	}

 	pos = animation->GetTimeline()->AddKeyframe((int)pBehaviorManager->GetCurrentTick(), replace);
 

	if (targetmode)
	{
 		if (replace)
			((HBhvInterpolatorPosition *)(animation->GetInterpolator()))->ReplaceLinear(t, pos);
		else
			((HBhvInterpolatorPosition *)(animation->GetInterpolator()))->InsertLinear(t, pos);
	}
	else
	{

 		if (replace)
			((HBhvInterpolatorPosition *)(animation->GetInterpolator()))->ReplaceLinear(p, pos);
		else
			((HBhvInterpolatorPosition *)(animation->GetInterpolator()))->InsertLinear(p, pos);
	}
	


	animation = pBehaviorManager->FindAnimation(targetname, "Scale");
	sprintf(ipc, "%d", pBehaviorManager->GetFreeName());
	if (animation == 0)
	{
		HBhvInterpolatorScale *i = new HBhvInterpolatorScale(0, ipc);
		HPoint parray(0,0,0);
 		int tline = 0;
 	 	i->Insert(parray, 0);
 		HBhvTimeline *t = new HBhvTimeline(0);
		t->SetTimeline(&tline , 1);
		animation = pBehaviorManager->AddAnimation(ipc, targetname, t, i);
	}

 	pos = animation->GetTimeline()->AddKeyframe((int)pBehaviorManager->GetCurrentTick(), replace);
	 
	HPoint scale;
	
	HPoint lv(t.x - p.x, t.y - p.y, t.z - p.z);
	if (proj[0] == 'p')
	{
	    scale.x =	(float)HC_Compute_Vector_Length(&lv);
	    scale.y = 0;
	}
	else
	    scale.Set(fx,fy);	
 	if (replace)
		((HBhvInterpolatorScale *)(animation->GetInterpolator()))->Replace(scale, pos);
	else
		((HBhvInterpolatorScale*)(animation->GetInterpolator()))->Insert(scale, pos);
 
	{		
//		float current_time;
//		HC_Show_Time(&current_time);
//		int timediff = (current_time-m_last_time) * 100.0f;
//
 //		m_last_time = current_time;
//		if (timediff<400)
		{
			HBhvBehaviorManager *pBehaviorManager = view->GetModel()->GetBhvBehaviorManager();
			pBehaviorManager->SetCurrentTick(pBehaviorManager->GetCurrentTick()+timediff);
		}
	}

	
}
	// TODO: Add your control notification handler code here
	
HBhvKeyframeGenerator::HBhvKeyframeGenerator(HBaseModel *model)
{
	m_pModel = model;
	m_Anims = new_vlist(malloc, free);
	m_Segments = new_vhash(10, malloc, free);
	m_AnimCounter = 0;
	m_Tick = 0;

}

bool HBhvKeyframeGenerator::CompareMatrices(float *m1, float *m2)
{
	if ((!m1 && m2) || (!m2 && m1))
		return false;
	for (int i=0;i<16;i++)
	{
		if (m1[i]!=m2[i])
			return false;
	}

	return true;
}

void HBhvKeyframeGenerator::StartFrames()
{
	m_bFirstFrame = true; 
//	m_pSegmentTracker = new HBhvSegmentTracker(m_pModel);
}

void HBhvKeyframeGenerator::NewFrame()
{
	HC_KEY segkey;
	int i;
	char text[4096];
	char text2[4096];
	char text3[4096];
	char text4[4096];
	char path[4096];
	char include_path[4096];
	char color[4096];
//	m_pSegmentTracker->MakeTreeSnapshot();
//	return;

	HC_Open_Segment_By_Key(m_pModel->GetModelKey());
	HC_Begin_Contents_Search("...", "segments");
	while (HUtility::FindContents(segkey))
	{
 		float matrix[16];
		bool hasMatrix = false;

		segkey = HC_KShow_Original_Key(segkey);
		HC_Open_Segment_By_Key(segkey);
		char balt1[4096];
		HC_Show_Segment(segkey,balt1);
//		if (HC_Show_Existence("modelling matrix") && strstr(balt1, "balt1"))
		if (HC_Show_Existence("modelling matrix"))
		{
			HC_Show_Modelling_Matrix(matrix);
			hasMatrix = true;		
		 
		segmentanimation *seganim; 	
 		if ((vhash_lookup_item(m_Segments, (void *)segkey ,  (void **)&seganim) == VHASH_STATUS_SUCCESS))
		{

			HC_Show_Segment(seganim->segkey, text);
			if (strcmp(text, seganim->path) != 0 && seganim->tick)
			{
				if (!seganim->anim2)
				{					
					sprintf(text,"%d",m_AnimCounter++);
 					HC_Show_Segment(m_pModel->GetModelKey(), text3);
					strcpy(text4, seganim->path + strlen(text3));
					sprintf(path,"SPATH:MODEL%s", text4);
					HBhvAnimation *anim2 = 	HBhvUtility::AddAnimation(m_pModel, text, path, 0);
 					HBhvUtility::AddSegmentMoveKeyframe(m_pModel,anim2, seganim->tick2, seganim->path);
					seganim->anim2 = anim2;
				}
				HC_Show_Segment(segkey, path);
				HBhvUtility::AddSegmentMoveKeyframe(m_pModel,seganim->anim2, m_Tick, path);
				seganim->tick2 = m_Tick;
				delete [] seganim->path;
				seganim->path = new char[strlen(path) + 1];
				strcpy(seganim->path, path);

			}
			if (hasMatrix && !CompareMatrices(seganim->matrix, matrix))	
			{
				if (!seganim->matrix)
				{
					seganim->matrix = new float[16];
					HC_Compute_Identity_Matrix(seganim->matrix);
				}
				if (!seganim->anim)
				{					
					sprintf(text,"%d",m_AnimCounter++);
					HC_Show_Segment(segkey, text2);
					HC_Show_Segment(m_pModel->GetModelKey(), text3);
					strcpy(text4, text2 + strlen(text3));
					sprintf(path,"SPATH:MODEL%s", text4);
					HPoint min, max, pivot;
					HC_Compute_Circumcuboid(".", &min, &max);
					pivot.x = -(0.5F * (min.x + max.x));
					pivot.y = -(0.5F * (min.y + max.y));
					pivot.z = -(0.5F * (min.z + max.z));				
					HBhvAnimation *anim = 	HBhvUtility::AddAnimation(m_pModel, text, path, &pivot);
//					HBhvUtility::AddPosRotKeyframe(m_pModel, anim->GetName(), seganim->tick, pivot, seganim->matrix);
					HBhvUtility::AddMatrixKeyframe(m_pModel, anim, seganim->tick, seganim->matrix);

					seganim->anim = anim;					
				}
//				if (m_Tick>seganim->tick+1)
//					HBhvUtility::AddPosRotKeyframe(m_pModel, seganim->anim->GetName(), m_Tick, seganim->anim->GetTarget()->GetPivot(), seganim->matrix);

			
// 				HBhvUtility::AddPosRotKeyframe(m_pModel, seganim->anim->GetName(), m_Tick, seganim->anim->GetTarget()->GetPivot(), matrix);
 				HBhvUtility::AddMatrixKeyframe(m_pModel, seganim->anim, m_Tick, matrix);
 				if (!seganim->matrix)
					seganim->matrix = new float[16];
				for (int i=0;i<16;i++)
					seganim->matrix[i] = matrix[i];
				seganim->tick = m_Tick;

			}
			else
			{		
				;
			}		

		}
		else
		{
			if (!m_bFirstFrame && 0)
			{
				if (HC_Show_Existence("includes"))
				{

					HC_KEY includekey;
					HC_Begin_Contents_Search(".", "includes");
					HUtility::FindContents(includekey);
					HC_End_Contents_Search();
					sprintf(text,"%d",m_AnimCounter++);
					HC_Show_Segment(segkey, text2);
					HC_Show_Segment(m_pModel->GetModelKey(), text3);
					strcpy(text4, text2 + strlen(text3));
					sprintf(path,"SPATH:MODEL%s", text4);

					HC_Show_Segment(HC_KShow_Include_Segment(includekey), include_path);
 					if (HC_Show_Existence("color"))
						HC_Show_Color(color);
					else
						strcpy(color,"");
					HPoint dummy;
					HBhvUtility::AddAnimation(m_pModel, text,path,&dummy);
 					HC_Show_Segment(segkey, path);
					HBhvUtility::AddInstanceCreateKeyframe(m_pModel,text, m_Tick, path, include_path, color);
				}
			}

 			seganim = new segmentanimation();
			if (hasMatrix)
			{			
				seganim->matrix = new float[16];
				for (i=0;i<16;i++)
					seganim->matrix[i] = matrix[i];
			}
			else
				seganim->matrix = 0;

			seganim->segkey = segkey;

			seganim->anim = 0;
			seganim->anim2 = 0;
			seganim->tick = m_Tick;
			seganim->tick2 = m_Tick;

			HC_Show_Segment(segkey, text);
			seganim->path = new char[strlen(text)+1];
			strcpy(seganim->path, text);
		  	vhash_insert_item(m_Segments, (void *)segkey , (void *)seganim);	
			vlist_add_last(m_Anims, seganim);
		}
		}			
		HC_Close_Segment();

	}
	HC_End_Contents_Search();
	HC_Close_Segment();
	m_Tick+=1;
	m_bFirstFrame = false;
}
 
 
HBhvSegmentTracker::HBhvSegmentTracker(HBaseModel *model)
{
	m_pModel = model;
	m_NewIncludesHash = new_vhash(10, malloc, free);

	HC_Open_Segment_By_Key(m_pModel->GetModelKey());
	m_TrackerKey = HC_KOpen_Segment("stracker");
	HC_Set_Rendering_Options("attribute lock = visibility");
	HC_Set_Visibility("off");
	HC_Create_Segment("trees");
	HC_Open_Segment("includes");
	HC_Set_Visibility("off");
	HC_Close_Segment();
	HC_Close_Segment();
	HC_Close_Segment();
}



void HBhvSegmentTracker::MakeTreeSnapshotRecursive(HC_KEY segkey, HC_KEY newsegkey, MVO_POINTER_SIZED_INT &uid)
{
	char visibility[4096];
	char color[4096];
 	HC_KEY shellkey, includekey;
	bool hasMatrix = false;
	float matrix[16];
	HC_Open_Segment_By_Key(segkey);

	if (HC_Show_Existence("modelling matrix"))
	{
		char text[4096];
		float tt = 0.0f;
		int i;
		HC_Show_Modelling_Matrix(matrix);
		for (i=0;i<16;i++)
			tt+=matrix[i];
		HCLOCALE(sprintf(text, "%f",tt));
		for (unsigned int ii=0;ii<strlen(text);ii++)
			uid+=text[ii];
		hasMatrix = true;
	}
 
	if (HC_Show_Existence("color"))
	{
		HC_Show_Color(color);
		for (unsigned int i=0;i<strlen(color);i++)
			uid+=color[i];
	}
	else
		strcpy(color,"");
	if (HC_Show_Existence("visibility"))
		HC_Show_Visibility(visibility);
	else
		strcpy(visibility,"");

	if (HC_Show_Existence("shells"))
	{
		HC_KEY newincludekey;
 		if (!(vhash_lookup_item(m_NewIncludesHash, (void *)segkey ,  (void **)&newincludekey) == VHASH_STATUS_SUCCESS))
		{
			HC_Open_Segment_By_Key(m_TrackerKey);
			HC_Open_Segment("includes");
			newincludekey = HC_KCreate_Segment("");
			HC_Close_Segment();
			HC_Close_Segment();
			HC_Begin_Contents_Search(".", "shells");
			while (HUtility::FindContents(shellkey))
			{			

				HC_Open_Segment_By_Key(newincludekey);
				HC_Copy_Geometry(shellkey, ".");
				HC_Close_Segment();
			}
			HC_End_Contents_Search();
		  	vhash_insert_item(m_NewIncludesHash, (void *)segkey , (void *)newincludekey);	
		}
		HC_Open_Segment_By_Key(newsegkey);
		HC_Include_Segment_By_Key(newincludekey);
		uid++;
//		HC_Set_User_Index(0,(void *)newincludekey);
 		HC_Close_Segment();
	}
	HC_Open_Segment_By_Key(newsegkey);
	if (strcmp(color,"") != 0)
		HC_Set_Color(color);
	if (strcmp(visibility,"") != 0)
		HC_Set_Visibility(visibility);
	if (hasMatrix)
		HC_Set_Modelling_Matrix(matrix);
	HC_Close_Segment();

 	HC_Begin_Contents_Search(".", "includes");
	while (HUtility::FindContents(includekey))
	{
		HC_Open_Segment_By_Key(newsegkey);
		HC_Include_Segment_By_Key(HC_KShow_Include_Segment(includekey));
		uid++;
//		HC_Set_User_Index(cc++,(void *)HC_KShow_Include_Segment(includekey));
		HC_Close_Segment();
	}
	HC_End_Contents_Search();

	HC_Begin_Contents_Search(".", "segments");
	while (HUtility::FindContents(segkey))
	{
 		char what[4096];
		HC_Show_Segment(segkey,what);
		if (segkey!=m_TrackerKey)
		{		
			uid+=123;
			char temp[4096];
			HC_Open_Segment_By_Key(segkey);
			if (HC_Show_Existence("visibility"))
				HC_Show_Visibility(temp);
			else
				strcpy(temp,"");
			if (strcmp(visibility,"off") != 0)
			{
  				HC_Open_Segment_By_Key(newsegkey);
				HC_KEY newk = HC_KCreate_Segment("");
				HC_Close_Segment();
				MakeTreeSnapshotRecursive(segkey, newk,uid);
			}
		}
	}
	HC_End_Contents_Search();
	HC_Close_Segment();

}

void HBhvSegmentTracker::MakeTreeSnapshot()
{
	HC_KEY segkey;
	HC_Open_Segment_By_Key(m_TrackerKey);
	HC_Open_Segment("trees");
	HC_KEY newkey = HC_KCreate_Segment("");
	HC_Close_Segment();
	HC_Close_Segment();

	HC_Open_Segment_By_Key(m_pModel->GetModelKey());
	HC_Begin_Contents_Search(".", "segments");
	while (HUtility::FindContents(segkey))
	{
			MVO_POINTER_SIZED_INT    v = 0, v2 = 0;
			bool firsttime = false;
			HC_Open_Segment_By_Key(segkey);
			if (HC_Show_Existence("user value"))
				HC_Show_User_Value(&v);
			else
				firsttime = true;
			HC_Close_Segment();

			HC_Open_Segment_By_Key(newkey);
			HC_KEY unewkey =  HC_KCreate_Segment("");
			HC_Close_Segment();
			HC_Close_Segment();
			
 			MakeTreeSnapshotRecursive(segkey, unewkey, v2);
			if (v2 == 0 || v2 == v)
			{
				HC_Delete_By_Key(unewkey);
				HC_Open_Segment_By_Key(newkey);
				HC_Include_Segment_By_Key(segkey);
				HC_Close_Segment();
			}
			if (firsttime)
			{
				HC_Open_Segment_By_Key(segkey);
				HC_Set_User_Value(v2);
				HC_Close_Segment();
			}	
	}
	HC_End_Contents_Search();
	HC_Close_Segment();

}


 		
  
