// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

/*! \file HGlobals.h
	Defines several file I/O and point/vector/pixel datatypes */

#ifndef _H_GLOBALS_H
#define _H_GLOBALS_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

/*! \enum HFileIOResult
* Indicates the result of an attempt to read a file
*/
enum HFileIOResult
{
	HIO_OK					= 1,    //!< self-explanatory
	HIO_VersionMismatch	= 2,    //!< self-explanatory
	HIO_Fail				= 3,    //!< self-explanatory
	HIO_NotHandled			= 4,    //!< self-explanatory
	HIO_BadFileName		= 5,    //!< self-explanatory
	HIO_BadOptions			= 6,    //!< self-explanatory
	HIO_BadLicense			= 7,    //!< self-explanatory
	HIO_LibraryNotFound	= 8		//!< self-explanatory
};

#define HFileInputResult		HFileIOResult
#define InputOK					HIO_OK
#define InputVersionMismatch	HIO_VersionMismatch
#define InputFail				HIO_Fail
#define InputNotHandled			HIO_NotHandled
#define InputBadFileName		HIO_BadFileName
#define InputBadOptions			HIO_BadOptions
#define InputBadLicense			HIO_BadLicense
#define InputLibraryNotFound	HIO_LibraryNotFound


#define HFileOutputResult		HFileIOResult
#define OutputOK				HIO_OK
#define OutputVersionMismatch	HIO_VersionMismatch
#define OutputFail				HIO_Fail
#define OutputNotHandled		HIO_NotHandled
#define OutputBadFileName		HIO_BadFileName
#define OutputBadOptions		HIO_BadOptions
#define OutputBadLicense		HIO_BadLicense
#define OutputLibraryNotFound	HIO_LibraryNotFound

/*! \def LAST_HFileInputResult
	\brief Specifies last file output type
*/
#define LAST_HFileInputResult InputBadOptions
/*! \def LAST_HFileOutputResult
	\brief Specifies last file output type
*/
#define LAST_HFileOutputResult OutputBadOptions

/*! \enum HSignal
	Used for requesting notification from objects when given actions happen.
	This is for intra-object signalling, not for messages passed to/from UI or OS
*/
enum HSignal
{
	HSignalDelete					= 1, //!< self-explantory
	HSignalCameraPositionChanged	= 2, //!< self-explantory
	HSignalSelected					= 3, //!< self-explantory
	HSignalDeSelectedOne			= 4, //!< self-explantory
	HSignalDeSelectedAll			= 5, //!< self-explantory
	HSignalPaint					= 6, //!< a paint (screen update) has occurred; signal_data is pointer to HRectangle containing window size
	HSignalClash					= 7, //!< objects have clashed; signal_data is pointer to StencilProbe object that detected a clash; StencilProbe is deprecated, should this be removed?
	HSignalRenderModeChanged		= 8, //!< self-explantory
	HSignalResize					= 9  //!< self-explantory
};

/*! \def LAST_HSignal
	\brief Specifies last object signal
*/
#define LAST_HSignal HSignalClash		 


/*! \enum HShadowMode
* Shadow Mode 
*/
enum HShadowMode
{
	HShadowNone = 1,  //!< no shadow
	HShadowSoft = 2,  //!< soft shadow
	HShadowHard = 3   //!< hard shadow
};

/*! \enum HCutGeometryVisibility
* cut geometry visibility settings
*/
enum HCutGeometryVisibility
{
	HCutGeometryVisibilityNone = 0x0, //!< No cut geometry will be visible
	HCutGeometryVisibilityFaces = 0x1, //!< Faces will be visible for cut geometry
	HCutGeometryVisibilityEdges = 0x2, //!< Edges will be visible for cut geometry
	HCutGeometryVisibilityAll = HCutGeometryVisibilityFaces | HCutGeometryVisibilityEdges //!< Both Faces and Edges will be visible
};

/*! \def LAST_HShadowMode
	\brief Specifies last shadow mode type
*/
#define LAST_HShadowMode HShadowHard		

//! The HPoint class is the data type of a three-dimensional point.
/*! 
  The HPoint class encapsulates the x, y, and z coordinates of a 3-dimensional point. HPoint simplifies the passing of 
  3-dimensional point data to all of the MVO and derived classes.  It is widely used by public and internal functions as 
  well as by native HOOPS routines.
*/
class MVO_API HPoint 
{ 
public:
	float		x;	//!< The x-coordinate of a 3-dimensional point.
	float		y;	//!< The y-coordinate of a 3-dimensional point.
	float		z;	//!< The z-coordinate of a 3-dimensional point.
	
	/*! The default constructor. */
	HPoint() : x(0), y(0), z(0) {};

	/*! This constructor accepts floats for the x, y and z coordinates.*/
	HPoint(float X, float Y, float Z=0.0f) : x(X), y(Y), z(Z) {};
	
	/*! This constructor accepts a pointer to an HPoint object. */
	HPoint(HPoint const * p) : x(p->x), y(p->y), z(p->z) {};
	
	/*! This constructor accepts an HPoint reference. */
	HPoint(HPoint const & p) : x(p.x), y(p.y), z(p.z) {};
	
	/*!  This method packs an HPoint object with coordinate values.  */
	void Set(float X, float Y, float Z=0.0f) { x=X; y=Y; z=Z; };
	
	/*!  This method packs an HPoint object with coordinate values from a given HPoint.  */
	void Set(HPoint *p) { x=p->x; y=p->y; z=p->z; };
	
	/*!  This method packs an HPoint object with coordinate values from a given HPoint.  */
	void Set(HPoint const *p) { x=p->x; y=p->y; z=p->z; };
	
	/*!  This method packs an HPoint object with coordinate values from a given HPoint that is passed by reference.  */
	void Set(HPoint &p) { x=p.x; y=p.y; z=p.z; };
	
	/*!  This method packs an HPoint object with coordinate values from a given HPoint that is passed by reference.  */
	void Set(const HPoint &p) { x=p.x; y=p.y; z=p.z; };

	/*!  This method increases/decreases the coordinate values of an existing HPoint object.  
	\param X The amount in which you want to increment or decrement the x coordinate.
	\param Y The amount in which you want to increment or decrement the y coordinate.
	\param Z The amount in which you want to increment or decrement the z coordinate.
	*/
	void Add(float X, float Y, float Z=0.0){ x+=X; y+=Y; z+=Z; };

	HPoint const operator -(const HPoint &p2) const
	{
		return HPoint(x-p2.x, y-p2.y, z-p2.z);
	}

	HPoint const operator +(const HPoint &p2) const
	{
		return HPoint(x+p2.x, y+p2.y, z+p2.z);
	}

	HPoint const operator *(float const rhs) const
	{
		return HPoint(x*rhs, y*rhs, z*rhs);
	}

	HPoint const operator /(float const rhs) const
	{
		return HPoint(x/rhs, y/rhs, z/rhs);
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
 
/*! The HPointKey class is the data type of a three-dimensional point with compairison operators defined so 
	the point can be used as a key in a container class (e.g. the STL).*/
/*! 
  The HPointKey class encapsulates HPoint. HPointKey can be used as a key in STL containers. 
*/

class MVO_API HPointKey : public HPoint
{
public:
	/*! The default constructor. */
	HPointKey() : HPoint(0, 0, 0) {};

	/*! This constructor accepts floats for the x, y and z coordinates.*/
	HPointKey(float X, float Y, float Z=0.0f) : HPoint(X, Y, Z) {};
	
	/*! This constructor accepts a pointer to an HPoint object. */
	HPointKey(HPoint const * p) : HPoint(p->x, p->y, p->z) {};
	
	/*! This constructor accepts an HPoint reference. */
	HPointKey(HPoint const & p) : HPoint(p.x, p.y, p.z) {};

	/* Define comparison operators so points can be used as keys in container classes. */
	bool operator < (HPointKey const & rhs) const
	{
		if(x < rhs.x)
			return true;
		else if(x == rhs.x){
			if(y < rhs.y)
				return true;
			else if(y == rhs.y){
				if(z < rhs.z)
					return true;
			}
		}
		return false;
	}

	bool operator > (HPointKey const & rhs) const
	{
		return !(*this < rhs);
	}

	bool operator <= (HPointKey const & rhs) const
	{
		if( (*this) < rhs ){
			return true;
		}
		else if( (*this) == rhs ){
			return true;
		}
		return false;
	}

	bool operator >= (HPointKey const & rhs) const
	{
		if( (*this) > rhs ){
			return true;
		}
		else if( (*this) == rhs ){
			return true;
		}
		return false;
	}
};


/*! This class is used for describing rectangles like window sizes or screen space areas.
*/
class MVO_API HIntRectangle
{ 
public:
	int	left;	//!< The left position.
	int	right;	//!< The right position.
	int	bottom; //!< The bottom position
	int	top;	//!< The top position.
};


//! The HVector class is the data type of a three-dimensional vector.
/*! 
  The HVector class encapsulates the x, y, and z coordinates of a 3-dimensional vector.  HVector simplifies the passing 
  of 3-dimensional vector data to all of the MVO and derived classes.  It is widely used by public and internal functions
  as well as by native HOOPS routines.
*/
typedef class HPoint HVector;


//! The HPlane class is the data type of a plane.
/*! 
  The  HPlane class encapsulates the a, b, c and d parameters of the plane equation.  
*/
class MVO_API HPlane
{
private:
	float		m_a;	//!< The a-parameter of the plane equation
	float		m_b;	//!< The b-parameter of the plane equation
	float		m_c;	//!< The c-parameter of the plane equation
	float		m_d;	//!< The d-parameter of the plane equation

public:
	HPlane(float const A = 0, float const B = 1, float const C = 0, float const D = 0) : 
	m_a(A), m_b(B), m_c(C), m_d(D){
	};
	
	HPlane(HVector const & normal, float const D) : 
	m_a(normal.x), m_b(normal.y), m_c(normal.z), m_d(D){
	};

	HPlane(HPlane const & p) : 
	m_a(p.a()), m_b(p.b()), m_c(p.c()), m_d(p.d()){
	};

	/*! This method sets an HPlane object with parameter values.*/
	inline void Set(float A, float B, float C, float D) { a(A); b(B); c(C); d(D); }

	/*! This method determines if given point is infront of or behind the plane.
	\return A positive value if the point is in front of the plane or a negative value if the point is behind the plane.  
	If the value is 0, the point is on the plane.*/
	inline double ClassifyPoint(HPoint const & p) const { return a() * p.x + b() * p.y + c() * p.z + d(); };

	/*! This method returns a perpendicular plane vector. 
	\param normal Pass by reference.  The method returns the value for a normalized vector that is
	perpendicular to the plane.*/
	inline void CalculateNormal(HPoint &normal) const
	{
		normal.Set(a(), b(), c());
		HC_Compute_Normalized_Vector(&normal, &normal);
	};

	/*! This method checks if the xyz components of the point are equal.  */
	bool Equal(HPlane const *p, float epsi1 = HPOINT_EPSILON, float epsi2 = HPOINT_EPSILON) const { 
		return (m_a - p->m_a < epsi1) && (m_a - p->m_a > -epsi1)
			&& (m_b - p->m_b < epsi1) && (m_b - p->m_b > -epsi1)
			&& (m_c - p->m_c < epsi1) && (m_c - p->m_c > -epsi1)
			&& (m_d - p->m_d < epsi2) && (m_d - p->m_d > -epsi2);
	};


	inline float a() const {return m_a;};
	inline float b() const {return m_b;};
	inline float c() const {return m_c;};
	inline float d() const {return m_d;};

	inline void a(float A) {m_a = A;};
	inline void b(float B) {m_b = B;};
	inline void c(float C) {m_c = C;};
	inline void d(float D) {m_d = D;};
}; 

//! The HPlaneKey class is the data type of a plane that can be used as a key in STL containers.
/*! 
  The  HPlane class encapsulates the a, b, c and d parameters of the plane equation.  
*/
class MVO_API HPlaneKey : public HPlane
{
public:
	HPlaneKey(float const A = 0, float const B = 1, float const C = 0, float const D = 0) : 
	HPlane(A, B, C, D) {
	};
	
	HPlaneKey(HVector const & normal, float const D) : HPlane(normal, D) {
	};

	HPlaneKey(HPlane const & p) : HPlane(p) {
	};

	bool operator < (HPlane const & rhs) const {
		HVector n;

		CalculateNormal(n);
		HPlane p1(n, d());

		rhs.CalculateNormal(n);
		HPlane p2(n, rhs.d());

		if(p1.a() < p2.a())
			return true;
		else if(p1.a() == p2.a()){
			if(p1.b() < p2.b())
				return true;
			else if(p1.b() == p2.b()){
				if(p1.c() < p2.c())
					return true;
				else if(p1.c() == p2.c()){
					if(p1.d() < p2.d())
						return true;
				}
			}
		}
		return false;	
	};

	bool operator == (HPlane const & rhs) const {
		HVector n;

		CalculateNormal(n);
		HPlane p1(n, d());

		rhs.CalculateNormal(n);
		HPlane p2(n, rhs.d());

		if( p1.a() == p2.a() &&
			p1.b() == p2.b() &&
			p1.c() == p2.c() &&
			p1.d() == p2.d())
			return true;

		return false;
	};

	bool operator >= (HPlane const & rhs) const {
		return !(*this < rhs);
	};

	bool operator > (HPlane const & rhs) const {
		return !(*this < rhs) && !(*this == rhs);
	};

	bool operator <= (HPlane const & rhs) const {
		return *this < rhs || *this == rhs;
	};

	bool operator != (HPlane const & rhs) const {
		return !(*this == rhs);
	};
}; 


//! The HPixelRGB class is the data type of a rgb pixel.
class MVO_API HPixelRGB
{
private:
	unsigned char m_r; //!< The red channel.
	unsigned char m_g; //!< The green channel.
	unsigned char m_b; //!< The blue channel.

public:
	HPixelRGB(unsigned char R = 0,unsigned char G = 0, unsigned char B = 0) : m_r(R), m_g(G), m_b(B) {
	};

	HPixelRGB(HPixelRGB const & p) : m_r(p.r()), m_g(p.g()), m_b(p.b()) {
	};

	inline unsigned char r() const {return m_r;};
	inline unsigned char g() const {return m_g;};
	inline unsigned char b() const {return m_b;};

	inline void r(unsigned char const R) {m_r = R;};
	inline void g(unsigned char const G) {m_g = G;};
	inline void b(unsigned char const B) {m_b = B;};

	/*! Sets the red, green and blue values. */
	inline void Set(unsigned char R, unsigned char G, unsigned char B){
		r(R);
		g(G);
		b(B);
	};

	/*! Sets a pixel value taking floats (between 0.0 - 1.0) for rgb.*/
	inline void Setf(float R, float G, float B){
		r((unsigned char)(R*255.99f));
		g((unsigned char)(G*255.99f));
		b((unsigned char)(B*255.99f));
	};
};

//! The HPixelRGBA class is the data type of a rgba pixel.
class MVO_API HPixelRGBA : public HPixelRGB
{
private:
	unsigned char m_a; //!< The alpha channel.

public:
	HPixelRGBA(
		unsigned char const R = 0,
		unsigned char const G = 0,
		unsigned char const B = 0,
		unsigned char const A = 0) 
		:HPixelRGB(R, G, B), m_a(A){
	};

	HPixelRGBA(HPixelRGBA const & p):HPixelRGB(p.r(), p.g(), p.b()),m_a(p.a()){
	};
	/*! Sets the pixel value. */
	inline void Set(unsigned char R, unsigned char G, unsigned char B, unsigned char A=0){
		HPixelRGB::Set(R, G, B);
		a(A);
	};

	/*! Sets a pixel value taking floats (between 0.0 - 1.0) for rgba.*/
	inline void Setf(float R, float G, float B, float A=0.0f){
		HPixelRGB::Setf(R, G, B);
		a((unsigned char)(A*255.99f));
	};

	inline unsigned char a() const {return m_a;};
	inline void a(unsigned char const A) {m_a = A;};
};

/*! \enum HSelectionHighlightMode */
/*!
  \addtogroup Selection_Level
  @{
*/

enum HSelectionHighlightMode {
	HighlightDefault,	//!< use default conditional highlighting
	HighlightQuickmoves, //!< use quick move reference highlighting 
	InverseTransparency,  //!< do nothing to the selected item, but make the rest of the model transparent.
	ColoredInverseTransparency  //!< color the selected item and make the rest of the model transparent.
};
/*!  @}  */


/*! \enum HRegionEdgeHighlightMode */
/*!
  \addtogroup Selection_Level
  @{
*/
enum HRegionEdgeHighlightMode
{
	NoEdges = 0,
	AllEdges,
	PerimeterEdges
};

enum HRegionFaceHighlightMode
{
	FacesUnmodified = 0,
	FacesForcedOn,
	FacesForcedOff
};
/*!  @}  */

/*! \enum HRefSelType */
enum HRefSelType
{
	RefSelSpriting,	//!< use spriting for reference selection
	RefSelDefault,	//!< use whatever the current quick moves preference is
	RefSelOff		//!< do not use quick moves for reference selection
};



#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif







