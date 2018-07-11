#pragma once
#include "hc.h"
#include "BStream.h"
#include "HStream.h"


#ifdef _DEBUG  
#ifndef _WIN64  // win32
#pragma comment(lib,"../HOOPS/Dev_Tools/hoops_3dgs/lib/nt_i386_vc12/hoopsd.lib") 
#pragma comment(lib,"../HOOPS/Dev_Tools/base_stream/lib/nt_i386_vc12/base_streamd.lib") 
#pragma comment(lib,"../HOOPS/Dev_Tools/hoops_stream/lib/nt_i386_vc12/hoops_streamd.lib")
#pragma comment(lib,"../HOOPS/Dev_Tools/3rdParty/ImageMagick/lib/nt_i386_vc12/hoops_mgkstatd_md.lib") 
#pragma comment(lib,"../openssl/static/lib/libeay32.lib") 
#pragma comment(lib,"../openssl/static/lib/ssleay32.lib")

#else  //x64
#pragma comment(lib,"../HOOPS/Dev_Tools/hoops_3dgs/lib/nt_x64_vc12/hoopsd.lib")  
#pragma comment(lib,"../HOOPS/Dev_Tools/base_stream/lib/nt_x64_vc12/base_streamd.lib") 
#pragma comment(lib,"../HOOPS/Dev_Tools/hoops_stream/lib/nt_x64_vc12/hoops_streamd.lib") 
#pragma comment(lib,"../HOOPS/Dev_Tools/3rdParty/ImageMagick/lib/nt_x64_vc12/hoops_mgkstatd_md.lib") 
#pragma comment(lib,"../openssl/static/libx64/libeay32.lib") 
#pragma comment(lib,"../openssl/static/libx64/ssleay32.lib") 
#pragma comment(lib,"../GDAL201/lib/gdal_i.lib") 
#pragma comment(lib,"../SQLite/x64/SimuDb_static_x64.lib")
#endif  

#else  
#ifndef _WIN64  //win32
#pragma comment(lib,"../HOOPS/Dev_Tools/hoops_3dgs/lib/nt_i386_vc12/hoops.lib") 
#pragma comment(lib,"../HOOPS/Dev_Tools/base_stream/lib/nt_i386_vc12/base_stream.lib") 
#pragma comment(lib,"../HOOPS/Dev_Tools/hoops_stream/lib/nt_i386_vc12/hoops_stream.lib") 
#pragma comment(lib,"../HOOPS/Dev_Tools/3rdParty/ImageMagick/lib/nt_i386_vc12/hoops_mgkstat_md.lib") 
#pragma comment(lib,"../openssl/static/lib/libeay32.lib") 
#pragma comment(lib,"../openssl/static/lib/ssleay32.lib")
#else  //x64 
#pragma comment(lib,"../HOOPS/Dev_Tools/hoops_3dgs/lib/nt_x64_vc12/hoops.lib")  
#pragma comment(lib,"../HOOPS/Dev_Tools/base_stream/lib/nt_x64_vc12/base_stream.lib") 
#pragma comment(lib,"../HOOPS/Dev_Tools/hoops_stream/lib/nt_x64_vc12/hoops_stream.lib") 
#pragma comment(lib,"../HOOPS/Dev_Tools/3rdParty/ImageMagick/lib/nt_x64_vc12/hoops_mgkstat_md.lib") 
#pragma comment(lib,"../openssl/static/libx64/libeay32.lib") 
#pragma comment(lib,"../openssl/static/libx64/ssleay32.lib")
#pragma comment(lib,"../GDAL201/lib/gdal_i.lib") 
#pragma comment(lib,"../SQLite/x64/SimuDb_static_x64.lib")
#endif  
#endif

#define H_VIEW_POINTER_INDEX			4155
#define SA_BUFFER_SIZE					4096

#define max(a,b)    (((a) > (b)) ? (a) : (b))
#define min(a,b)    (((a) < (b)) ? (a) : (b))

#ifndef INVALID_KEY
#define INVALID_KEY (-1L)
#endif

class HPoint
{
public:
	float		x;	//!< The x-coordinate of a 3-dimensional point.
	float		y;	//!< The y-coordinate of a 3-dimensional point.
	float		z;	//!< The z-coordinate of a 3-dimensional point.

	/*! The default constructor. */
	HPoint() : x(0), y(0), z(0) {};

	/*! This constructor accepts floats for the x, y and z coordinates.*/
	HPoint(float X, float Y, float Z = 0.0f) : x(X), y(Y), z(Z) {};

	/*! This constructor accepts a pointer to an HPoint object. */
	HPoint(HPoint const * p) : x(p->x), y(p->y), z(p->z) {};

	/*! This constructor accepts an HPoint reference. */
	HPoint(HPoint const & p) : x(p.x), y(p.y), z(p.z) {};

	/*!  This method packs an HPoint object with coordinate values.  */
	void Set(float X, float Y, float Z = 0.0f) { x = X; y = Y; z = Z; };

	/*!  This method packs an HPoint object with coordinate values from a given HPoint.  */
	void Set(HPoint *p) { x = p->x; y = p->y; z = p->z; };

	/*!  This method packs an HPoint object with coordinate values from a given HPoint.  */
	void Set(HPoint const *p) { x = p->x; y = p->y; z = p->z; };

	/*!  This method packs an HPoint object with coordinate values from a given HPoint that is passed by reference.  */
	void Set(HPoint &p) { x = p.x; y = p.y; z = p.z; };

	/*!  This method packs an HPoint object with coordinate values from a given HPoint that is passed by reference.  */
	void Set(const HPoint &p) { x = p.x; y = p.y; z = p.z; };

	/*!  This method increases/decreases the coordinate values of an existing HPoint object.
	\param X The amount in which you want to increment or decrement the x coordinate.
	\param Y The amount in which you want to increment or decrement the y coordinate.
	\param Z The amount in which you want to increment or decrement the z coordinate.
	*/
	void Add(float X, float Y, float Z = 0.0){ x += X; y += Y; z += Z; };

	HPoint const operator -(const HPoint &p2) const
	{
		return HPoint(x - p2.x, y - p2.y, z - p2.z);
	}

	HPoint const operator +(const HPoint &p2) const
	{
		return HPoint(x + p2.x, y + p2.y, z + p2.z);
	}

	HPoint const operator *(float const rhs) const
	{
		return HPoint(x*rhs, y*rhs, z*rhs);
	}

	HPoint const operator /(float const rhs) const
	{
		return HPoint(x / rhs, y / rhs, z / rhs);
	}

	HPoint const & operator += (HPoint const & rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}

	HPoint const & operator -= (HPoint const & rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		return *this;
	}

	HPoint const & operator *= (float const rhs)
	{
		x *= rhs;
		y *= rhs;
		z *= rhs;
		return *this;
	}

	HPoint const & operator /= (float const rhs)
	{
		x /= rhs;
		y /= rhs;
		z /= rhs;
		return *this;
	}

	bool operator ==(HPoint const & rhs) const
	{
		return Equal(&rhs);
	}

	bool operator !=(HPoint const & rhs) const
	{
		return !Equal(&rhs);
	}


	/*!\def HPOINT_EPSILON */
#define HPOINT_EPSILON (1e-5f)  //!<  HPOINT_EPSILON


	/*! This method checks if the xyz components of the point are equal.  */
	bool Equal(HPoint const *p, float epsi = HPOINT_EPSILON) const {
		return (x - p->x < epsi) && (x - p->x > -epsi)
			&& (y - p->y < epsi) && (y - p->y > -epsi)
			&& (z - p->z < epsi) && (z - p->z > -epsi);
	};

	/*!  This method checks if the xyz components of the point are equal.  */
	bool Equal(HPoint const &p, float epsi = HPOINT_EPSILON) const {
		return Equal(&p, epsi);
	};
};