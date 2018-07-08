#pragma once
#include "HoopsFile/Viewer.h"
#include "BMNode.h"
using namespace Simu;
class TessellationCylinder
{
public:
	vector<HPoint> m_vShellPoints;
	vector<int> m_vFaceLists;
	bool Tessellation(vector<HPoint>& vCenterPoints,vector<float>& vRadius,int tesselationNum = 6);
private:
	void ComputeCrossProfilePointsEX(HPoint& point, HPoint& lineNormal, HPoint& assistPoint,int tesselationNum,float radius, vector<HPoint>& returnPoints);
	//计算 直线point----lineNormal 在point处截面圆上点
	void ComputeCrossProfilePoints(HPoint& point, HPoint& lineNormal, int tesselationNum, float radius, vector<HPoint>& returnPoints);
	//计算 折线assistPoint0-point-assistPoint1 在point处截面椭圆上点
	void ComputeCrossProfilePoints(HPoint& assistPoint0, HPoint& point, HPoint& assistPoint1, int tesselationNum, float radius, vector<HPoint>& returnPoints);
	HPoint ComputeTriangleNormal(HPoint& p0, HPoint& p1, HPoint&p2);
	inline HPoint ComputeCenterPt(HPoint& p0, HPoint& p1, HPoint&p2);
	inline HPoint ComputeNewPtOnNormalByDis(HPoint& originPt, HPoint& normal, float dis);
	//计算椭圆细分点majorRadius长半径 semiMajorRadius短半径
	void ComputePointOnEllipse(float majorRadius, float semiMajorRadius, int tesselationNum, vector<HPoint>& returnPoints);
	//
	void LocalToWorldCoord(HPoint& originPt, HPoint& xx, HPoint& yy, HPoint& zz, vector<HPoint>& returnPoints);
};

