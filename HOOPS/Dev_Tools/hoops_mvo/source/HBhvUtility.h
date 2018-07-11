// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

//"Parts of HQuat Copyright (C) Jason Shankel, 2000"
//
//

/*! \file HBhvUtility.h
	Defines datatypes associated with the HBhvUtility class  
*/

#ifndef _H_HBhvUtility_H
#define _H_HBhvUtility_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"

#ifndef SWIG
#include "BStream.h"
#endif

class HUtilityXMLTag;
class HUtilityTextParser;
class HBhvTimeline;
class HBaseModel;
class HStreamFileToolkit;
class HBaseView;
class HBaseModel;
class HBhvAnimation;

#define			BHV_MAX_NAME_LENGTH		256		//!< Maximum Length of "Names" in animations
#define			BHV_MAX_VERSION_LENGTH	256			//!< Maximum Length of "Version" string
#define			BHV_MAX_VENDOR_LENGTH	4096			//!< Maximum Length of "Vendor" string
#define			BHV_MAX_TYPE_LENGTH		4096		//!< Maximum Length of Animation Type string


/*! \enum HANIKeyframeType Keyframe type */
enum HANIKeyframeType {
	HANIChannel,			//!< Channel keyframe type. 
	HANIRotation,			//!< Rotational keyframe type.
 	HANIString,				//!< String keyframe type.
 	HANI3String				//!< String keyframe type.
};


/*! \enum HANIChannelType Subtype for channel keyframes */
enum HANIChannelType {
	HANILinear,				//!< Linear channel type.
	HANIHermiteSpline,		//!< Hermite spline channel type.
	HANIBezierSpline,		//!< Bezier spline channel type.
	HANIFollowPath,			//!< Follow path channel type.
	HANIDiscrete			//!< Discrete channel type.
};


/*! \enum HANIRotationType Subtype for rotational keyframes */
enum HANIRotationType {		
	HANIAxisRotation,		//!< Axis rotation type.
	HANIQuatSlerpRotation,	//!< Linear quaternion rotational type.
	HANIQuatSquadRotation,	//!< Non-linear quaternion rotational type.
	HANIEulerRotation		//!< Euler rotation type.
};



					
//! The HQuat class defines the data type of a Quaternion 
/*!
 The HQuat class encapsulates the x,y,x,w values used to define a quaternion as well as various
 functions to perform algebrahic operations on quaternions
*/ 

class MVO_API HQuat
{
public:
	float x;		/*!< Quaternion x Coordinate*/
	float y;		/*!< Quaternion y Coordinate*/
	float z;		/*!< Quaternion z Coordinate*/
	float w;		/*!< Quaternion w Coordinate*/
	const HQuat &Normalize();  /*!< normalizes an HQuat */
 	/*! default constructor */
	HQuat() { x=0.0f;y=0.0f;z=0.0f; w=0;};
	/*! constructor that accepts three floats */
	HQuat(float X,float Y,float Z, float W) { x=X;y=Y;z=Z; w=W;};
	/*! constructor that accepts a pointer to an HQuat object */
	HQuat(HQuat const *p) { x=p->x;y=p->y;z=p->z; w=p->w;};
	/*! constructor that accepts an HQuat reference */
	HQuat(const HQuat &p) { x=p.x;y=p.y;z=p.z; w=p.w;};
	
	/*!  Set packs an HQuat object with coordinate values.  */
	void Set(float X,float Y,float Z, float W) { x=X;y=Y;z=Z; w=W; };

	/*!
	Multiplies two quaternions
	\param q2 Quaternion to multiply with
	\return Result of multiplication
	*/
   	HQuat operator *(const HQuat &q2) const
	{
		
		return HQuat(y*q2.z - z*q2.y + w*q2.x + x*q2.w,
						z*q2.x - x*q2.z + w*q2.y + y*q2.w,
						x*q2.y - y*q2.x + w*q2.z + z*q2.w,
						w*q2.w - x*q2.x - y*q2.y - z*q2.z);
	}

	/*!
	Multiplies a quaternion with a float value
	\param v Value to multiply Quaternion with
	\return Result of multiplication
	*/
 	HQuat operator *(float v) const
	{
		return HQuat(x*v, y*v, z*v, w*v);
	}

	/*!
	Multiplies a quaternion with a float value
	\param v Value to multiply Quaternion with
	\param q Quaternion to multiply with
	\return Result of multiplication
	*/	
	friend HQuat operator* (float v, const HQuat& q)
	{
	 	return HQuat(v*q.x,v*q.y,v*q.z, v*q.w);
	}

	/*!
	Divides a quaternion by a float value
	\param v Value to divide Quaternion by
 	\return Result of multiplication
	*/	
	HQuat operator /(float v) const
	{
		return HQuat(x/v, y/v, z/v, w/v);
	}

	/*!
	Subtracts one quaternion from another
	\param q2 Quaternion to subtract with
  	\return Result of subtraction
	*/		HQuat operator -(const HQuat &q2) const
	{
 		return HQuat(x-q2.x, y-q2.y, z-q2.z, w-q2.w);
	}

	/*!
	Add one quaternion to another
	\param q2 Quaternion to add 
  	\return Result of addition
	*/
	HQuat operator +(const HQuat &q2) const
	{
		return HQuat(x+q2.x, y+q2.y, z+q2.z, w+q2.w);
	}
	
	/*!
	Logarithm of a HQuat, given as:
	Qlog(q) = v*a where q = [cos(a),v*sin(a)]
	\param q Quaternion to perform log on
	\return Result of logarithm
	*/
	static HQuat Qlog(const HQuat &q);
 
	/*!
	Convert Quaternion to matrix form
 	\param matrix Returns result matrix
 	*/
	void ToMatrix(float matrix[16]);
	

	/*!
	Qexp(v*a) = [cos(a),vsin(a)]
	\param q Quaternion to perform exp on
	\return Result of Exp
 	*/
 	static HQuat Qexp(const HQuat &q);	
	/*!
	Linear interpolation between two HQuats
	\param q1 First Quaternion for Interpolation
	\param q2 Second Quaternion for Interpolation
	\param fraction Interpolation Distance (0 - 1)
	\return Result of interplation
	*/	
	static HQuat Qlerp(const HQuat &q1,const HQuat &q2,float fraction);
	/*!
	Spherical interpolation between two HQuats
	\param q1 First Quaternion for Interpolation
	\param q2 Second Quaternion for Interpolation
	\param fraction Interpolation Distance (0 - 1)
	\return Result of interplation
	*/	
	static HQuat Qslerp(const HQuat &q1,const HQuat &q2,float fraction);
	/*!
	Spherical interpolation between two HQuats
	This version of slerp, used by squad, does not check for theta > 90.
	\param q1 First Quaternion for Interpolation
	\param q2 Second Quaternion for Interpolation
	\param fraction Interpolation Distance (0 - 1)
	\return Result of interplation
	*/	
	static HQuat QslerpNoInvert(const HQuat &q1,const HQuat &q2,float fraction);
	/*!
	Spherical interpolation between two HQuats
	This version of slerp, used by squad, does not check for theta > 90.
	\param q1 First Quaternion for Interpolation
	\param q2 Second Quaternion for Interpolation
	\param fraction Interpolation Distance (0 - 1)
	\return Result of interplation
	*/	
	static HQuat QslerpNoInvertExtraSpins(const HQuat &q1,const HQuat &q2,float fraction, int ExtraSpins);
	/*!
	Spherical cubic interpolation between two HQuats
 	\param q1 First Quaternion for Interpolation
	\param q2 Second Quaternion for Interpolation
	\param a First Control Point
	\param b Second Control Point
	\param fraction Interpolation Distance (0 - 1)
	\return Result of interplation
	*/	
	static HQuat Qsquad(const HQuat &q1,const HQuat &q2,const HQuat &a,const HQuat &b,float fraction);
	/*!
	Given 3 HQuats, qn-1,qn and qn+1, calculate a control point to be used in spline interpolation
 	\param qnm1 qn-1 control point
 	\param qn qn control point
 	\param qnp1 qn + 1 control point
 	\return Result of interplation
	*/	
	static HQuat Qspline(const HQuat &qnm1,const HQuat &qn,const HQuat &qnp1);
	/*!
	Convert matrix to quaternion
 	\return converted quaternion
 	*/
	static HQuat MatrixToQuaternion(float matrix[]);

};

 

//! The HKeyframe class is the base class for all keyframe types.
/*!	
	Every interpolator has an array of keyframe values. Based on the given time an interpolation is
	performed between those keyframes which contribute to the overall animation.
	Keyframes can not only hold positional information or rotations but also information like color or segment identifiers.
	For performance reasons, all data members of the various keyframe classes are defined as public.
*/ 
class MVO_API HKeyframe
{
public:
 	/*!
	Constructs an HKeyframe object.
 	*/
	HKeyframe()
	{
		m_bConstant = false;
		m_bEaseInOut = false;
	}
	virtual ~HKeyframe() {};

 	/*!
	This method makes an exact copy of the current HKeyframe and returns a pointer to the newly
	created HKeyframe object.
	\return A pointer to the newly created HKeyframe object.
 	*/
	virtual HKeyframe *Clone() { 
		HKeyframe *p = new HKeyframe;
		*p =  *this;
		return p;
	}

	/*!
	Writes XML data to a buffer.
  	*/
	virtual void Serialize(HUtilityXMLTag *xmlgen) {
		UNREFERENCED(xmlgen);
	}
 
	/*!
	Use this method to keep the keyframe value constant.  
	\param constant Pass true if you want to keep the same value as the previous keyframe or false if you
	want to use a different value.
  	*/
	virtual void SetConstant(bool constant) { m_bConstant = constant; }
	/*!
	Use this method to indicate that you want a smooth transition during the start
	and end of the animation.  
	\param easeinout Pass true to turn on easing in and out or false if you don't want this behavior.
  	*/
	virtual void SetEaseInOut(bool easeinout) { m_bEaseInOut = easeinout; }
	/*!This feature is not yet implemented.*/
	virtual void SetRelative(bool relative) { m_bRelative = relative; }
  
	HANIKeyframeType	m_type;			/*!< Type of Keyframe */
	bool				m_bConstant;	/*!< The value is true if this keyframe's value is the same value as the previous keyframe.*/
	bool				m_bRelative;	/*!< This feature is not yet implemented.*/
	bool				m_bEaseInOut;	/*!< The value is true if smooth transition for start and stop is turned on or false if otherwise.*/
};


//! The HKeyframeChannel class is the base class for all channel related keyframe types.
/*!	
	Channel keyframes are used for positional values as well as colour and scale.
*/ 
class MVO_API HKeyframeChannel : public HKeyframe
{
public:

	/*!
	Constructs an HKeyframeChannel object.
 	*/
 	HKeyframeChannel()
	{
		m_cp.Set(0.0f,0.0f,0.0f);
		m_type =  HANIChannel;
	}
	virtual ~HKeyframeChannel() {};

 	/*!
	This method makes an exact copy of the current HKeyframeChannel and returns a pointer to the newly
	created HKeyframeChannel object.
	\return A pointer to the newly created HKeyframeChannel object.
 	*/
	virtual HKeyframe *Clone () 
	{
		HKeyframeChannel *p= new HKeyframeChannel;
		*p = *this;
		return p;
	}

	/*!
	Calculates the interpolation between this keyframe and the next keyframe.
	\param posarray The array of HKeyframeChannel objects.
	\param keyframe They current keyframe number.
	\param fraction The fraction from this keyframe to next.
	\param length The number of entries in the keyframe array.
	\param res Returns the interpolation result.
  	*/
	virtual void Interpolate(HKeyframeChannel **posarray, int keyframe, float fraction, int length, HPoint &res) {
		UNREFERENCED(posarray);
		UNREFERENCED(keyframe);
		UNREFERENCED(fraction);
		UNREFERENCED(length);
		UNREFERENCED(res);
	}
	
	HPoint			m_cp;					/*!< The channnel coordinates. */
	HANIChannelType m_channeltype;			/*!< The type of channel. */
};



 
//! The HKeyframeChannelLinear class is used for linear interpolation of positional values.
class MVO_API HKeyframeChannelLinear : public HKeyframeChannel
{
public:
 
	/*!
	Constructs a HKeyframeChannelLinear object.
 	*/
	HKeyframeChannelLinear(float x=0, float y=0,  float z=0) : HKeyframeChannel()
	{
		m_channeltype = HANILinear;
		m_cp.Set(x,y,z);
	}
	virtual ~HKeyframeChannelLinear() {};

 	/*!
	This method makes an exact copy of the current HKeyframeChannelLinear and returns a pointer to the newly
	created HKeyframeChannelLinear object.
	\return A pointer to newly created HKeyframeChannelLinear object.
 	*/
	virtual HKeyframe *Clone () 
	{
		HKeyframeChannelLinear *p= new HKeyframeChannelLinear;
		*p = *this;
		return p;
	}
 
	/*!
	Calculates the interpolation between this keyframe and the next keyframe.
	\param posarray The array of HKeyframeChannel objects.
	\param keyframe They current keyframe number.
	\param fraction The fraction from this keyframe to next.
	\param length The number of entries in the keyframe array.
	\param res Returns the interpolation result.
  	*/
	virtual void Interpolate(HKeyframeChannel **posarray, int keyframe, float fraction, int length, HPoint &res);

	/*!
	Writes XML data to a buffer.
  	*/
	virtual void Serialize(HUtilityXMLTag *xmlgen);

};

//! The HKeyframeChannelDiscrete class is used for discrete interpolation of positional values.
/*! 
	No interpolation is performed between this keyframe and the next keyframe.
*/
class MVO_API HKeyframeChannelDiscrete : public HKeyframeChannel
{
public:
 
	/*!
	Constructs a HKeyframeChannelDiscrete Object.
 	*/
	HKeyframeChannelDiscrete(float x=0, float y=0, float z = 0) : HKeyframeChannel()
	{
		m_channeltype = HANIDiscrete;
		m_cp.Set(x,y,z);

	}
	virtual ~HKeyframeChannelDiscrete() {};

 	/*!
	This method makes an exact copy of the current HKeyframeChannelDiscrete and returns a pointer to the newly
	created HKeyframeChannelDiscrete object.
	\return A pointer to the newly created HKeyframeChannelDiscrete object.
 	*/
	virtual HKeyframe *Clone () 
	{
		HKeyframeChannelDiscrete *p= new HKeyframeChannelDiscrete;
		*p = *this;
		return p;
	}

	/*!
	Calculates the interpolation between this keyframe and the next keyframe.  In this case, there is no interpolation.
	The calculated position will be the same position value of the given keyframe if the fraction is less than one.  Otherwise, 
	the caluculated position will be the position value of the next keyframe in the array.
	\param posarray The array of HKeyframeChannel objects.
	\param keyframe They current keyframe number.
	\param fraction The fraction from this keyframe to next.
	\param length The number of entries in the keyframe array.
	\param res Returns the interpolation result.
  	*/
	virtual void Interpolate(HKeyframeChannel **posarray, int keyframe, float fraction, int length, HPoint &res);

	/*!
	Writes XML data to a buffer.
  	*/
	virtual void Serialize(HUtilityXMLTag *xmlgen);
 
};



//! The HKeyframeChannelCurve class is used for spline based interpolation of positional values.
/*! 
*/
class MVO_API HKeyframeChannelCurve : public HKeyframeChannel
{
public:

	/*!
	Constructs a HKeyframeChannelCurve Object.
 	*/
	HKeyframeChannelCurve(float x = 0, float y = 0, float z = 0) : HKeyframeChannel()
	{
		m_channeltype = HANIHermiteSpline;
		m_cp.Set(x,y,z);

	}
 	virtual ~HKeyframeChannelCurve() {};

 	/*!
	This method makes an exact copy of the current HKeyframeChannelCurve and returns a pointer to the newly
	created HKeyframeChannelCurve object.
	\return A pointer to the newly created HKeyframeChannelCurve object.
 	*/
	virtual HKeyframe *Clone () 
	{
		HKeyframeChannelCurve *p= new HKeyframeChannelCurve;
		*p = *this;
		return p;
	}

 	/*!
	This method is a helper function for CalculateHermiteTangents().  Call this method
	to precalculate curve factors before calling CalculateHermiteTangents().
	\param timeline Timeline array necessary to calculate correct curve factors
	\param keyframe Current Keyframe
  	*/
	void CalculateCurveFactor(HBhvTimeline *timeline, int keyframe);

 	/*!
	Use this method to precalculate tangents for the hermite spline.  This method can be usef as a helper function
	for interpolation.
	\param p0 The first point for tangent calculation.
	\param p2 The third point for tangent calculation.
	\param p3 The fourth Point for tangent calculation.
	\param a Constant for hermite spline
   	*/
	void CalculateHermiteTangents(HPoint p0, HPoint p2, HPoint p3, float a = 0.5f); 

 	/*!
	This method uses the given frame and control points to interpolate a Hermite spline. 
	\param fraction Distance between the first and second control point.
	\param p1 The first control point.
	\param p2 The second control point.
	\return The interpolated result.
   	*/
	HPoint InterpolateHermiteSpline(float fraction, HPoint p1, HPoint p2);

//	HPoint InterpolateBezierSpline(float fraction, HPoint p2);


 	/*!
	This method calculates the length of this curve between this keyframe and a given point.
	\param p2 The point to calculate the length with.
    */
	void CalculateCurveLength(HPoint p2);

 	/*!
	This method creates a visualization of the tangents calculated for this curve by drawing them into the
	scene.
	\param p2 The second point to calculate tangents from.
     */
	void DrawTangents(HPoint p2);

	/*!
	Calculates the interpolation between this keyframe and the next keyframe.
	\param posarray The array of HKeyframeChannel objects.
	\param keyframe They current keyframe number.
	\param fraction The fraction from this keyframe to next.
	\param length The number of entries in the keyframe array.
	\param res Returns the interpolation result.
  	*/	
	virtual void Interpolate(HKeyframeChannel **posarray, int keyframe, float fraction, int length, HPoint &res);

	/*!
	Writes XML data to a buffer.
  	*/
	virtual void Serialize(HUtilityXMLTag *xmlgen);
	
	HPoint		m_tangent1;						/*!< The first tangent. */
	HPoint		m_tangent2;						/*!< The second tangent. */
	float		m_factor1;					    /*!< For internal use only.*/
	float		m_factor2;						/*!< For internal use only.*/
	HPoint*		control_polygon;				/*!< For internal use only.*/
	float		ItemType;						/*!< For internal use only.*/
	float		c;								/*!< For internal use only.*/
	float		b;								/*!< For internal use only.*/				
	float		m_curvelength;					/*!< For internal use only.*/

};

//! The HKeyframeChannelFollowPath class is used for interpolators that change orientation in relation to position
/*!
	By specifying a set of positional control points, this interpolator can adjust the orientation of the target 
	according to the path curvature like a car following the road.
*/
class MVO_API HKeyframeChannelFollowPath : public HKeyframeChannelCurve
{

public:
 
	/*!
	Constructs a HKeyframeChannelFollowPath object.
 	*/
	HKeyframeChannelFollowPath() 
	{
		m_channeltype = HANIFollowPath;
		m_bLinear = false;
	}
	virtual ~HKeyframeChannelFollowPath() {};

 	/*!
	This method makes an exact copy of the current HKeyframeChannelFollowPath and returns a pointer to the newly
	created HKeyframeChannelFollowPath object.
	\return A pointer to newly created HKeyframeChannelFollowPath object.
 	*/
	virtual HKeyframe *Clone () 
	{
		HKeyframeChannelFollowPath *p= new HKeyframeChannelFollowPath;
		*p = *this;
		return p;
	}

	/*!
	Calculates the interpolation between this keyframe and the next keyframe.
	\param posarray The array of HKeyframeChannel objects.
	\param keyframe They current keyframe number.
	\param fraction The fraction from this keyframe to next.
	\param length The number of entries in the keyframe array.
	\param res Returns the interpolation result.
  	*/	
	virtual void Interpolate(HKeyframeChannel **posarray, int keyframe, float fraction, int length, HPoint &res);
	virtual void Serialize(HUtilityXMLTag *xmlgen);

//	virtual void Serialize(HUtilityXMLTag *xmlgen);
	float tmatrix[16];				/*!< For internal use only.*/
 	float tmatrix2[16];				/*!< For internal use only.*/
	bool m_bLinear;
	bool m_bNoRotate;
};


 

//! The HKeyframeRotation class is the base class for all rotation related keyframes.
class MVO_API HKeyframeRotation : public HKeyframe
{
public:
 
	/*!
	Constructs a HKeyframeRotation object.
 	*/
	HKeyframeRotation()
	{
		m_type = HANIRotation;
		m_bLinear = false;
	}
	virtual ~HKeyframeRotation() {};

 	/*!
	This method makes an exact copy of the current HKeyframeRotation and returns a pointer to the newly
	created HKeyframeRotation object.
	\return A pointer to the newly created HKeyframeRotation object.
 	*/
	virtual HKeyframe *Clone () 
	{
		HKeyframeRotation *p= new HKeyframeRotation;
		*p = *this;
		return p;
	} 

	HANIRotationType m_rotationtype;			/*!< The type of rotation.*/
	bool m_bLinear;								/*!< The value is true if the rotation is linear or false if the rotation is not non-linear.*/

};



//! The HKeyframeAxisRotation class is used for rotations around an arbitrary axis.
class MVO_API HKeyframeAxisRotation : public HKeyframeRotation
{
public:


	/*!
	Constructs a HKeyframeAxisRotation object.
	\param angle The degrees of rotation for the keyframe.
 	*/
 	HKeyframeAxisRotation(float angle = 0)
	{
		m_rotationtype = HANIAxisRotation;
		m_angle = angle;

	}
	virtual ~HKeyframeAxisRotation() {};

 	/*!
	This method makes an exact copy of the current HKeyframeAxisRotation and returns a pointer to the newly
	created HKeyframeAxisRotation object.
	\return A pointer to newly created HKeyframeAxisRotation object.
 	*/
	virtual HKeyframe *Clone () 
	{
		HKeyframeAxisRotation *p= new HKeyframeAxisRotation;
		*p = *this;
		return p;

	} 

	/*!
	Writes XML data to a buffer.
  	*/
	virtual void Serialize(HUtilityXMLTag *xmlgen);

	float m_angle;				/*!< The degrees of rotation.*/
};


//! The HKeyframeQuatSlerp class is used for linear quaternion based rotation.
class MVO_API HKeyframeQuatSlerp : public HKeyframeRotation
{
public:

	/*!
	Constructs a HKeyframeQuatSlerp object.
 	*/
 	HKeyframeQuatSlerp(float x = 0, float y = 0, float z = 0, float w = 0)
	{
		m_rotationtype = HANIQuatSlerpRotation;
		m_quat.Set(x,y,z,w);
	}

	virtual ~HKeyframeQuatSlerp() {};
 
 	/*!
	This method makes an exact copy of the current HKeyframeQuatSlerp and returns a pointer to the newly
	created HKeyframeQuatSlerp object.
	\return A pointer to the newly created HKeyframeQuatSlerp object.
 	*/
	virtual HKeyframe *Clone () 
	{
		HKeyframeQuatSlerp *p= new HKeyframeQuatSlerp;
		*p = *this;
		return p;
	} 	
	
	HQuat m_quat;			/*!< The quaternion.*/
};




//! The HKeyframeQuatSquad class is used for non-linear quaternion based rotation
class MVO_API HKeyframeQuatSquad : public HKeyframeRotation
{
public:

 
	/*!
	Constructs a HKeyframeQuatSquad object.
 	*/
 	HKeyframeQuatSquad(float x = 0, float y = 0, float z = 0, float w = 0)
	{
		m_rotationtype = HANIQuatSquadRotation;
		m_quat.Set(x,y,z,w);
		m_ExtraSpins = 0;

	}
	virtual ~HKeyframeQuatSquad() {};

 	/*!
	This method makes an exact copy of the current HKeyframeQuatSquad and returns a pointer to the newly
	created HKeyframeQuatSquad object.
	\return A pointer to the newly created HKeyframeQuatSquad object.
 	*/
	virtual HKeyframe *Clone () 
	{
		HKeyframeQuatSquad *p= new HKeyframeQuatSquad;
		*p = *this;
		return p;
	} 

	/*!
	Writes XML data to a buffer.
  	*/	
	virtual void Serialize(HUtilityXMLTag *xmlgen);

	HQuat m_quat;			/*!< The quaternion.*/
	int m_ExtraSpins;	

};


//! The HKeyframeQuatSquad class is used for non-linear quaternion based rotation
class MVO_API HKeyframeMatrix : public HKeyframeRotation
{
public:

 
	/*!
	Constructs a HKeyframeQuatSquad object.
 	*/
 	HKeyframeMatrix(float matrix[] = 0)
	{
		m_rotationtype = HANIQuatSquadRotation;

		if (matrix)
		{		
		for (int i=0;i<16;i++)
			m_matrix[i] = matrix[i];
		}
 
	}
	virtual ~HKeyframeMatrix() {};

 	/*!
	This method makes an exact copy of the current HKeyframeQuatSquad and returns a pointer to the newly
	created HKeyframeQuatSquad object.
	\return A pointer to the newly created HKeyframeQuatSquad object.
 	*/
	virtual HKeyframe *Clone () 
	{
		HKeyframeMatrix *p= new HKeyframeMatrix;
		*p = *this;
		return p;
	} 

	/*!
	Writes XML data to a buffer.
  	*/	
	virtual void Serialize(HUtilityXMLTag *xmlgen);

	float m_matrix[16];			/*!< The quaternion.*/
	int m_ExtraSpins;	

};





//! The HKeyframeString class is used for any string related keyframes
/*!
	String keyframes can contain a segment path in the case of segment switch interpolation or some other
	text based identifier.
*/
class MVO_API HKeyframeString : public HKeyframe
{
public:
	
	/*!
	Constructs a HKeyframeString object.
 	*/
  	HKeyframeString();
	virtual ~HKeyframeString() {};

 	/*!
	This method makes an exact copy of the current HKeyframeString and returns a pointer to the newly
	created HKeyframeString object.
	\return A pointer to the newly created HKeyframeString object.
 	*/
 	virtual HKeyframe *Clone () 
	{
		HKeyframeString *p= new HKeyframeString;
		*p = *this;
		return p;
	}

	/*!
	Writes XML data to a buffer.
  	*/
	virtual void Serialize(HUtilityXMLTag *xmlgen);

	
	/*!
	Sets a new target string.
	\param target The new target for the interpolation.
  	*/
	void SetTarget(const char *target);

	/*! \return The target string.
   	*/
 	char * GetTarget() { return m_target; }

	char m_target[MVO_BUFFER_SIZE];			/*!< Keyframe Target*/
	bool m_bDiscrete;

};


//! The HKeyframeString class is used for any string related keyframes
/*!
	String keyframes can contain a segment path in the case of segment switch interpolation or some other
	text based identifier.
*/
class MVO_API HKeyframe3String : public HKeyframe
{
public:
	
	/*!
	Constructs a HKeyframeString object.
 	*/
  	HKeyframe3String();
	virtual ~HKeyframe3String() {};

 	/*!
	This method makes an exact copy of the current HKeyframeString and returns a pointer to the newly
	created HKeyframeString object.
	\return A pointer to the newly created HKeyframeString object.
 	*/
 	virtual HKeyframe *Clone () 
	{
		HKeyframe3String *p= new HKeyframe3String;
		*p = *this;
		return p;
	}

	/*!
	Writes XML data to a buffer.
  	*/
	virtual void Serialize(HUtilityXMLTag *xmlgen);

	
	/*!
	Sets a new target string.
	\param target The new target for the interpolation.
  	*/
	void SetTarget(const char *t1, const char *t2, const char *t3);

	/*! \return The target string.
   	*/
 	char * GetTarget1() { return m_t1; }
 	char * GetTarget2() { return m_t2; }
 	char * GetTarget3() { return m_t3; }

	char m_t1[255];			/*!< Keyframe Target*/
	char m_t2[255];			/*!< Keyframe Target*/
	char m_t3[255];			/*!< Keyframe Target*/

};





//! The ShellMorphInfo class contains vertex attribute information
/*!
	ShellMorphInfo is used in conjunction with the VertexMorph Interpolator to describe vertex attribute 
	changes
*/
class MVO_API ShellMorphInfo {
public:

	/*!
	Constructs a ShellMorphInfo Object
 	*/
	ShellMorphInfo();
	struct vlist_s*	arrays_list;			/*!< List of Shell Morph Information*/
	HC_KEY original_key;					/*!< For internal use only.*/
	HC_KEY temp_key;						/*!< For internal use only.*/

};



#ifndef SWIG
//! The TK_Bhv_XML subclasses writes and reads behavior related to XML data.
class MVO_API TK_Bhv_XML : public TK_XML
{

 
public:

	/*!
	Constructs a TK_Bhv_XML object.
	\param model A pointer to the HBaseModel object.
 	*/
	TK_Bhv_XML(HBaseModel * model) : TK_XML() 
	{ 
		m_pModel = model;
	}

	/*! This method has been customized to extract information from MVO 'behavior' data structures. */
 	TK_Status	Interpret(BStreamFileToolkit &tk, ID_Key key, int variant);
	/*! This method has been customized to populate MVO 'behavior' data structures with information. */
	TK_Status   Execute (BStreamFileToolkit & tk);

private:
	HBaseModel *	m_pModel;	
};
#endif



//! The HBhvUtility class contains a set of utility functions and data structures used in the behaviour classes.
class MVO_API HBhvUtility
{
public:
	
	/*! 
	Extracts Quaternion from string
	\param tp Pointer to text parser object
	\param quat Returns extracted quaternion
 	*/
 	static void	GetQuatPoint(HUtilityTextParser *tp, HQuat &quat);
	static void GetMatrix(HUtilityTextParser *tp, float matrix[]);


	/*! 
	Extracts Quaternion from string
	\param text Text to extract quaternion from
	\param quat Returns extracted quaternion
 	*/
	static void GetQuatPoint(const char *text, HQuat &quat);

	static void GetMatrix(const char *text, float matrix[]);


	/*! 
	Converts sequence of HSF files to Vertex Morph Interpolator
 	\param filename Name of first file in sequence
	\param model Pointer to HBaseModel object
 	*/
	static void ConvertHSFtoMorph(const char *filename, HBaseModel *model);

	/*! 
	Converts sequence of HSF files to Segment Switch Interpolator
 	\param filename Name of first file in sequence
	\param model Pointer to HBaseModel object
 	*/
	static void ConvertHSFtoSegSwitch(const char *filename, HBaseModel *model);	

	/*! 
	Sets up streaming toolkit for reading of behaviour related XML data
  	\param mytool Pointer to streaming toolkit
 	\param model Pointer to HBaseModel object
 	*/
 	static void SetupAnimationReadFromHSF(HStreamFileToolkit *mytool, HBaseModel *model);
 	
	/*! 
	Sets up streaming toolkit for writing of behaviour related XML data
  	\param mytool Pointer to streaming toolkit
 	\param model Pointer to HBaseModel object
 	*/	
	static void SetupAnimationWriteFromHSF(HStreamFileToolkit *mytool, HBaseModel *model);

	/*! 
	Retrieves modelling matrix in currently open segment 
  	\param segment_modelling_matrix Returns modelling matrix
  	*/
	static void ShowModellingMatrix(float *segment_modelling_matrix);

	static void MakeCameraKeyframe(HBaseView *view, float timediff, bool targetmode = false);

	static HBhvAnimation* AddAnimation(HBaseModel *model, char *name, char *targetpath, HPoint *pivot);
	static void AddPosKeyframe(HBaseModel *model, char *animname, int tick, HPoint pos, bool linear);
	static void AddRotKeyframe(HBaseModel *model, char *animname, int tick, HQuat pos, bool linear);
	static void AddPosRotKeyframe(HBaseModel *model, const char *animname, int tick, HPoint pivot, float matrix[]);
	static void AddMatrixKeyframe(HBaseModel *model, HBhvAnimation *ani, int tick, float matrix[]);

	static void AddPosRotKeyframe(HBaseModel *model, char *animname, int tick, HPoint pos, bool linear, HQuat quat, bool linear2);
 	static void AddInstanceCreateKeyframe(HBaseModel *model, char *animname, int tick, char *path, char *include_path, char *color);
 	static void AddSegmentMoveKeyframe(HBaseModel *model, HBhvAnimation *anim, int tick, char *path);





private:		
	static void Move_Segments_To_Segment(HC_KEY target, HC_KEY destination);
	static void AddShellToMorphList(HC_KEY key, struct vlist_s *morph_data_list);		
	static bool KeyExists(HC_KEY key, struct vlist_s *morp_data_list);
	static void ResetMorphData(struct vlist_s *morp_data_list);
 	static void CollectShellMorphData(HC_KEY key, struct vlist_s *morp_data_list);
	static void ClearAllAttributes(HC_KEY key);
	static int searchcounter;
};

typedef struct
{
	HC_KEY segkey;
	float *matrix;
	HBhvAnimation *anim;
	HBhvAnimation *anim2;
	int tick;
	int tick2;
	char *path;
}segmentanimation;


class MVO_API HBhvSegmentTracker
{
public:
	HBhvSegmentTracker(HBaseModel *model);
	void MakeTreeSnapshotRecursive(HC_KEY segkey, HC_KEY newsegkey, MVO_POINTER_SIZED_INT &uid);
	void MakeTreeSnapshot();

	HC_KEY m_TrackerKey;
	HBaseModel *m_pModel;
	struct vhash_s *		m_NewIncludesHash;
};

class MVO_API HBhvKeyframeGenerator
{
public:
	HBhvKeyframeGenerator(HBaseModel * model);

	void NewFrame();
	void StartFrames();
	
	bool CompareMatrices(float *m1, float *m2);

private:
	HBaseModel *m_pModel;
	struct vlist_s *		m_Anims;
	struct vhash_s *		m_Segments;
	int m_AnimCounter;
	int m_Tick;
	bool m_bFirstFrame;
	HBhvSegmentTracker *m_pSegmentTracker;
};


	


#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif



