#pragma once
#include "HoopsFile/Viewer.h"
#include "BMNode.h"
using namespace Simu;
class HDPoint
{
public:
	double		x;	//!< The x-coordinate of a 3-dimensional point.
	double		y;	//!< The y-coordinate of a 3-dimensional point.
	double		z;	//!< The z-coordinate of a 3-dimensional point.

	/*! The default constructor. */
	HDPoint() : x(0), y(0), z(0) {};

	/*! This constructor accepts floats for the x, y and z coordinates.*/
	HDPoint(double X, double Y, double Z = 0.0) : x(X), y(Y), z(Z) {};

	/*! This constructor accepts a pointer to an HDPoint object. */
	HDPoint(HDPoint const * p) : x(p->x), y(p->y), z(p->z) {};

	/*! This constructor accepts an HDPoint reference. */
	HDPoint(HDPoint const & p) : x(p.x), y(p.y), z(p.z) {};

	/*!  This method packs an HDPoint object with coordinate values.  */
	void Set(double X, double Y, double Z = 0.0) { x = X; y = Y; z = Z; };

	/*!  This method packs an HDPoint object with coordinate values from a given HDPoint.  */
	void Set(HDPoint *p) { x = p->x; y = p->y; z = p->z; };

	/*!  This method packs an HDPoint object with coordinate values from a given HDPoint.  */
	void Set(HDPoint const *p) { x = p->x; y = p->y; z = p->z; };

	/*!  This method packs an HDPoint object with coordinate values from a given HDPoint that is passed by reference.  */
	void Set(HDPoint &p) { x = p.x; y = p.y; z = p.z; };

	/*!  This method packs an HDPoint object with coordinate values from a given HDPoint that is passed by reference.  */
	void Set(const HDPoint &p) { x = p.x; y = p.y; z = p.z; };

	/*!  This method increases/decreases the coordinate values of an existing HDPoint object.
	\param X The amount in which you want to increment or decrement the x coordinate.
	\param Y The amount in which you want to increment or decrement the y coordinate.
	\param Z The amount in which you want to increment or decrement the z coordinate.
	*/
	void Add(double X, double Y, double Z = 0.0){ x += X; y += Y; z += Z; };

	HDPoint const operator -(const HDPoint &p2) const
	{
		return HDPoint(x - p2.x, y - p2.y, z - p2.z);
	}

	HDPoint const operator +(const HDPoint &p2) const
	{
		return HDPoint(x + p2.x, y + p2.y, z + p2.z);
	}

	HDPoint const operator *(double const rhs) const
	{
		return HDPoint(x*rhs, y*rhs, z*rhs);
	}

	HDPoint const operator /(double const rhs) const
	{
		return HDPoint(x / rhs, y / rhs, z / rhs);
	}

	HDPoint const & operator += (HDPoint const & rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}

	HDPoint const & operator -= (HDPoint const & rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		return *this;
	}

	HDPoint const & operator *= (double const rhs)
	{
		x *= rhs;
		y *= rhs;
		z *= rhs;
		return *this;
	}

	HDPoint const & operator /= (double const rhs)
	{
		x /= rhs;
		y /= rhs;
		z /= rhs;
		return *this;
	}

	bool operator ==(HDPoint const & rhs) const
	{
		return Equal(&rhs);
	}

	bool operator !=(HDPoint const & rhs) const
	{
		return !Equal(&rhs);
	}


	/*!\def HDPoint_EPSILON */
#define HDDPoint_EPSILON (1e-5)  //!<  HDPoint_EPSILON


	/*! This method checks if the xyz components of the point are equal.  */
	bool Equal(HDPoint const *p, double epsi = HDDPoint_EPSILON) const {
		return (x - p->x < epsi) && (x - p->x > -epsi)
			&& (y - p->y < epsi) && (y - p->y > -epsi)
			&& (z - p->z < epsi) && (z - p->z > -epsi);
	};

	/*!  This method checks if the xyz components of the point are equal.  */
	bool Equal(HDPoint const &p, double epsi = HDDPoint_EPSILON) const {
		return Equal(&p, epsi);
	};
};
class TessellationCylinder
{
public:
	vector<HDPoint> m_vShellPoints;
	vector<int> m_vFaceLists;
	bool Tessellation(vector<HDPoint>& vCenterPoints,vector<float>& vRadius,int tesselationNum = 6);
private:
	void ComputeCrossProfilePointsEX(HDPoint& point, HDPoint& lineNormal, HDPoint& assistPoint,int tesselationNum,float radius, vector<HDPoint>& returnPoints);
	//计算 直线point----lineNormal 在point处截面圆上点
	void ComputeCrossProfilePoints(HDPoint& point, HDPoint& lineNormal, int tesselationNum, float radius, vector<HDPoint>& returnPoints);
	//计算 折线assistPoint0-point-assistPoint1 在point处截面椭圆上点
	void ComputeCrossProfilePoints(HDPoint& assistPoint0, HDPoint& point, HDPoint& assistPoint1, int tesselationNum, float radius, vector<HDPoint>& returnPoints);
	HDPoint ComputeTriangleNormal(HDPoint& p0, HDPoint& p1, HDPoint&p2);
	inline HDPoint ComputeCenterPt(HDPoint& p0, HDPoint& p1, HDPoint&p2);
	inline HDPoint ComputeNewPtOnNormalByDis(HDPoint& originPt, HDPoint& normal, float dis);
	//计算椭圆细分点majorRadius长半径 semiMajorRadius短半径
	void ComputePointOnEllipse(float majorRadius, float semiMajorRadius, int tesselationNum, vector<HDPoint>& returnPoints);
	//
	void LocalToWorldCoord(HDPoint& originPt, HDPoint& xx, HDPoint& yy, HDPoint& zz, vector<HDPoint>& returnPoints);
};

