#include "stdafx.h"
#include "TessellationCylinder.h"
//
bool TessellationCylinder::Tessellation(vector<HDPoint>& vCenterPoints, vector<float>& vRadius, int tesselationNum/* = 6*/)
{
	size_t numPt = vCenterPoints.size();
	size_t numRa = vRadius.size();
	if (numPt < 2 || numRa == 0) return false;
	//确保vRadius 中半径个数与 vCenterPoints中中心点个数 一一对应
	if (numRa < numPt)
	{
		float lastRadius = vRadius[numRa - 1];
		for (size_t i = 0; i < (numPt -numRa); ++i)
		{
			vRadius.push_back(lastRadius);
		}
	}
	//最少4分
	tesselationNum = max(4, tesselationNum);
	//
	std::map<int,vector<HDPoint>> vMapCrossProfilePoints;
	vector<HDPoint> vTempCrossProfile;
	vTempCrossProfile.reserve(tesselationNum);
	if (numPt == 2)
	{ 
		HDPoint _lineNormal = vCenterPoints[0] - vCenterPoints[1];
		HC_DCompute_Normalized_Vector(&_lineNormal, &_lineNormal);
		//第一个点
		ComputeCrossProfilePoints(vCenterPoints[0], _lineNormal, tesselationNum, vRadius[0], vTempCrossProfile);
		vMapCrossProfilePoints[0] = vTempCrossProfile;
		//第二个点
		vTempCrossProfile.clear();
		ComputeCrossProfilePoints(vCenterPoints[1], _lineNormal, tesselationNum, vRadius[1], vTempCrossProfile);
		vMapCrossProfilePoints[1] = vTempCrossProfile;
	}
	else
	{
		//先计算中间点
		for (size_t i = 1; i < numPt - 1; ++i)
		{
			vTempCrossProfile.clear();
			ComputeCrossProfilePoints(vCenterPoints[i - 1], vCenterPoints[i], vCenterPoints[i + 1], tesselationNum, vRadius[i], vTempCrossProfile);
			vMapCrossProfilePoints[i] = vTempCrossProfile;
		}
		//计算第一个点
		vTempCrossProfile.clear();
		HDPoint _lineNormal = vCenterPoints[0] - vCenterPoints[1];
		HDPoint _assistPoint = vMapCrossProfilePoints[1][0];
		HC_DCompute_Normalized_Vector(&_lineNormal, &_lineNormal);
		ComputeCrossProfilePointsEX(vCenterPoints[0], _lineNormal, _assistPoint, tesselationNum, vRadius[0], vTempCrossProfile);
		vMapCrossProfilePoints[0] = vTempCrossProfile;
		//计算最后一个点
		vTempCrossProfile.clear();
		_lineNormal = vCenterPoints[numPt - 1] - vCenterPoints[numPt - 2];
	    _assistPoint = vMapCrossProfilePoints[numPt - 2][0];
		HC_DCompute_Normalized_Vector(&_lineNormal, &_lineNormal);
		ComputeCrossProfilePointsEX(vCenterPoints[numPt - 1], _lineNormal, _assistPoint, tesselationNum, vRadius[numPt - 1], vTempCrossProfile);
		vMapCrossProfilePoints[numPt - 1] = vTempCrossProfile;
		//处理点顺序
		for (size_t i = 1; i < numPt; ++i)
		{
			vector<HDPoint>* pPoints = &vMapCrossProfilePoints[i];
			_lineNormal = vCenterPoints[i] - vCenterPoints[i - 1];
			HC_DCompute_Normalized_Vector(&_lineNormal, &_lineNormal);
			//
			HDPoint& _basePt = vMapCrossProfilePoints[i - 1][0];
			size_t _maxDotIndex = 0;
			HDPoint _tempNormal = pPoints->at(0) - _basePt;
			HC_DCompute_Normalized_Vector(&_tempNormal, &_tempNormal);
			double _maxDot = abs(HC_DCompute_Dot_Product(&_lineNormal,&_tempNormal));
			for (size_t j = 1; j < tesselationNum;++j)
			{
				_tempNormal = pPoints->at(j) - _basePt;
				HC_DCompute_Normalized_Vector(&_tempNormal, &_tempNormal);
				double tempDot = abs(HC_DCompute_Dot_Product(&_lineNormal, &_tempNormal));
				if (tempDot > _maxDot)
				{
					_maxDot = tempDot;
					_maxDotIndex = j;
				}
			}
			//
			_tempNormal = pPoints->at((_maxDotIndex + 1) % tesselationNum) - vMapCrossProfilePoints[i - 1][1];
			HC_DCompute_Normalized_Vector(&_tempNormal, &_tempNormal);
			if (abs(HC_DCompute_Dot_Product(&_lineNormal, &_tempNormal) > 0.99999))
			{
				vector<HDPoint>_tempPoints;
				_tempPoints.reserve(tesselationNum);
				for (size_t k = 0; k < tesselationNum;++k)
				{
					_tempPoints.push_back(pPoints->at((_maxDotIndex + k)%tesselationNum));
				}
				pPoints->swap(_tempPoints);
			}
			else
			{
				vector<HDPoint>_tempPoints;
				_tempPoints.reserve(tesselationNum);
				for (size_t k = 0; k < tesselationNum; ++k)
				{
					_tempPoints.push_back(pPoints->at((_maxDotIndex - k + tesselationNum) % tesselationNum));
				}
				pPoints->swap(_tempPoints);
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	//构建 shell
	m_vShellPoints.reserve(numPt*tesselationNum);
	for (size_t i = 0; i < numPt;++i)
	{
		vector<HDPoint>& tempPoints = vMapCrossProfilePoints[i];
		m_vShellPoints.insert(m_vShellPoints.end(), tempPoints.begin(), tempPoints.end());
	}
	m_vFaceLists.reserve((numPt-1)*tesselationNum*2*4);
	for (size_t i = 1; i < numPt;++i)
	{
		vector<HDPoint>& vPrePoints = vMapCrossProfilePoints[i-1];
		vector<HDPoint>& vCurpPoints = vMapCrossProfilePoints[i];
		//
		HDPoint normal0 = ComputeTriangleNormal(vPrePoints[0], vCurpPoints[0], vPrePoints[1]);
		HDPoint normal1 = ComputeTriangleNormal(vPrePoints[1], vCurpPoints[1], vCurpPoints[2]);
		HDPoint center0 = ComputeCenterPt(vPrePoints[0], vCurpPoints[0], vPrePoints[1]);
		HDPoint center1 = ComputeCenterPt(vPrePoints[1], vCurpPoints[1], vCurpPoints[2]);
		HDPoint newPt0 = ComputeNewPtOnNormalByDis(center0, normal0, vRadius[i] / 2);
		HDPoint newPt1 = ComputeNewPtOnNormalByDis(center1, normal1, vRadius[i] / 2);
		HDPoint temp0 = center1 - center0;
		HDPoint temp1 = newPt1 - newPt0;
		double dis0 = HC_DCompute_Vector_Length(&temp0);
		double dis1 = HC_DCompute_Vector_Length(&temp1);
		if (dis1 > dis0)
		{
			for (int j = 0; j < tesselationNum;++j)
			{
				int preIndex0 = (i - 1)*tesselationNum + j;
				int preIndex1 = (i - 1)*tesselationNum + (j + 1) % tesselationNum;
				int curIndex0 = preIndex0 + tesselationNum;
				int curIndex1 = preIndex1 + tesselationNum;
				m_vFaceLists.push_back(3); m_vFaceLists.push_back(preIndex0); m_vFaceLists.push_back(curIndex0); m_vFaceLists.push_back(preIndex1);
				m_vFaceLists.push_back(3); m_vFaceLists.push_back(curIndex0); m_vFaceLists.push_back(curIndex1); m_vFaceLists.push_back(preIndex1);
			}
		}
		else
		{
			for (int j = 0; j < tesselationNum; ++j)
			{
				int preIndex0 = (i - 1)*tesselationNum + j;
				int preIndex1 = (i - 1)*tesselationNum + (j + 1) % tesselationNum;
				int curIndex0 = preIndex0 + tesselationNum;
				int curIndex1 = preIndex1 + tesselationNum;
				m_vFaceLists.push_back(3); m_vFaceLists.push_back(preIndex0); m_vFaceLists.push_back(preIndex1); m_vFaceLists.push_back(curIndex0);
				m_vFaceLists.push_back(3); m_vFaceLists.push_back(preIndex1); m_vFaceLists.push_back(curIndex1); m_vFaceLists.push_back(curIndex0);
			}
		}
	}
	//
	return true;
}
//////////////////////////////////////////////////////////////////////////
void TessellationCylinder::ComputeCrossProfilePointsEX(HDPoint& point, HDPoint& lineNormal, HDPoint& assistPoint, int tesselationNum, float radius, vector<HDPoint>& returnPoints)
{
	HDPoint tempXNormal = assistPoint;
	HDPoint worldZ = lineNormal;
	HDPoint worldY;
	HC_DCompute_Cross_Product(&worldZ, &tempXNormal, &worldY);
	HC_DCompute_Normalized_Vector(&worldY, &worldY);
	//
	HDPoint worldX;
	HC_DCompute_Cross_Product(&worldY, &worldZ, &worldX);
	HC_DCompute_Normalized_Vector(&worldX, &worldX);
	//
	ComputePointOnEllipse(radius, radius, tesselationNum, returnPoints);
	LocalToWorldCoord(point,worldX, worldY, worldZ, returnPoints);
}
//计算 直线point----lineNormal 在point处截面圆上点
void TessellationCylinder::ComputeCrossProfilePoints(HDPoint& point, HDPoint& lineNormal, int tesselationNum, float radius, vector<HDPoint>& returnPoints)
{
	HDPoint tempXNormal = HDPoint(1.0,0.0,0.0);
	if (lineNormal == HDPoint(1.0, 0.0, 0.0) || lineNormal == HDPoint(-1.0, 0.0, 0.0))
		tempXNormal = HDPoint(0.0, 1.0, 0.0);
	//
	HDPoint worldZ = lineNormal;
	HDPoint worldY;
	HC_DCompute_Cross_Product(&worldZ, &tempXNormal, &worldY);
	HC_DCompute_Normalized_Vector(&worldY, &worldY);
	//
	HDPoint worldX;
	HC_DCompute_Cross_Product(&worldY, &worldZ, &worldX);
	HC_DCompute_Normalized_Vector(&worldX, &worldX);
	//
	ComputePointOnEllipse(radius, radius, tesselationNum, returnPoints);
	LocalToWorldCoord(point,worldX, worldY, worldZ, returnPoints);
}
//计算 折线assistPoint0-point-assistPoint1 在point处截面椭圆上点
void TessellationCylinder::ComputeCrossProfilePoints(HDPoint& assistPoint0, HDPoint& point, HDPoint& assistPoint1, int tesselationNum, float radius, vector<HDPoint>& returnPoints)
{
	HDPoint n0 = assistPoint0 - point;
	HC_DCompute_Normalized_Vector(&n0, &n0);
	HDPoint n1 = assistPoint1 - point;
	HC_DCompute_Normalized_Vector(&n1, &n1);
	//
	double angle = acos(HC_DCompute_Dot_Product(&n0, &n1));
	float majorRadius = float(radius / sin(angle / 2));
	//
	HDPoint worldX = n1 + n0;
	HC_DCompute_Normalized_Vector(&worldX, &worldX);
	HDPoint worldY;
	HC_DCompute_Cross_Product(&n0, &n1, &worldY);
	HC_DCompute_Normalized_Vector(&worldY, &worldY);
	//
	HDPoint worldZ;
	HC_DCompute_Cross_Product(&worldX, &worldY, &worldZ);
	HC_DCompute_Normalized_Vector(&worldZ, &worldZ);
	//
	ComputePointOnEllipse(majorRadius, radius, tesselationNum, returnPoints);
	LocalToWorldCoord(point,worldX, worldY, worldZ, returnPoints);
}
HDPoint TessellationCylinder::ComputeTriangleNormal(HDPoint& p0, HDPoint& p1, HDPoint&p2)
{
	HDPoint n0 = p1 - p0;
	HDPoint n1 = p2 - p0;
	HDPoint resultNormal;
	HC_DCompute_Cross_Product(&n0, &n1, &resultNormal);
	HC_DCompute_Normalized_Vector(&resultNormal, &resultNormal);
	return resultNormal;
}
HDPoint TessellationCylinder::ComputeCenterPt(HDPoint& p0, HDPoint& p1, HDPoint&p2)
{
	return (p0 + p1 + p2) / 3.0f;
}
HDPoint TessellationCylinder::ComputeNewPtOnNormalByDis(HDPoint& originPt, HDPoint& normal, float dis)
{
	return originPt + normal * dis;
}
//计算椭圆细分点majorRadius长半径 semiMajorRadius短半径
void TessellationCylinder::ComputePointOnEllipse(float majorRadius, float semiMajorRadius, int tesselationNum, vector<HDPoint>& returnPoints)
{
	double perAngle = (3.14159265 * 2) / tesselationNum;
	HDPoint tempPt;
	for (int i = 0; i < tesselationNum;++i)
	{
		tempPt.x = majorRadius * cos(perAngle * i);
		tempPt.y = semiMajorRadius * sin(perAngle * i);
		returnPoints.push_back(tempPt);
	}
}
void TessellationCylinder::LocalToWorldCoord(HDPoint& originPt,HDPoint& xx, HDPoint& yy, HDPoint& zz, vector<HDPoint>& returnPoints)
{
	size_t num = returnPoints.size();
	for (size_t i = 0; i < num;++i)
	{
		HDPoint& tempPoint = returnPoints[i];
		//
		double x = originPt.x + tempPoint.x*xx.x + tempPoint.y*yy.x + tempPoint.z*zz.x;
		double y = originPt.y + tempPoint.x*xx.y + tempPoint.y*yy.y + tempPoint.z*zz.y;
		double z = originPt.z + tempPoint.x*xx.z + tempPoint.y*yy.z + tempPoint.z*zz.z;
		tempPoint.x = x;
		tempPoint.y = y;
		tempPoint.z = z;
	}
}