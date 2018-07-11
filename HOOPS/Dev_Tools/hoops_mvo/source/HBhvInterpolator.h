// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef _H_HBhvInterpolator_H
#define _H_HBhvInterpolator_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"
#include "HBhvUtility.h"
#include "varray.h"


class HUtilityXMLGenerator;
class HBhvAnimation;
  
#ifdef WINDOWS_SYSTEM
template class MVO_API VArray< HKeyframe *>;
template class MVO_API VArray< void *>;
#endif


/*! \enum TrailInterpolatorType
	Various Modes for Trail Interpolator
*/
enum TrailInterpolatorType
{
	Forward,		  //!< line is drawn from beginning of animation to current position
	Backward,			//!< line is drawn from end of animation to current position
	Full				//!< line is drawn from beginning of animation to end of animation
};


#define REPLACE_VARRAY(c, pos) {	if ((int)m_pArray.Count() > pos)     \
									{								\
									HKeyframe *temp = m_pArray[pos];\
									delete temp;\
									}								\
									m_pArray.ReplaceAt(c, pos);    }								

#define REMOVE_VARRAY(pos)			{  HKeyframe *temp = m_pArray[pos];\
										delete temp;\
										m_pArray.RemoveAt(pos);    }								

#define CURVE_ARRAY(pos)		((HKeyframeChannelCurve *)GetAt(pos))
							


//! The HBhvInterpolator class is the abstract base class for all interpolator types.
/*!	
	HBhvInterpolator keeps an array of keyframes as well as methods to access members of 
	that array. The various interpolator types then implement the required
	functions that actually perform the interpolation. 
*/ 
class MVO_API HBhvInterpolator
{
public:
	/*!
	Constructs an HBhvInterpolator object.
	\param animation The animation that this interpolator belongs to.
	\param name The name of this interpolator.
	*/
	HBhvInterpolator(HBhvAnimation *animation = 0, const char *name = 0);
	virtual ~HBhvInterpolator();

	/*! \return Type of this interpolator. */
	virtual const char * GetType() = 0;

	/*!
	In special cases, if you want to reference another interpolator without changing it, use this method to indicate
	the interpolator that you want this interpolator to refer to.
	\param interpolator Interpolator to reference.
	*/	
	void SetInstancedInterpolator(HBhvInterpolator *interpolator);

	/*!
	In special cases, you may want an interpolator that references this interpolator without changing it. Use 
	this method to create the new interpolator passing a pointer to the animation that you want it to be associated with.
	\param animationinst The animation instance that this interpolator is associated with.
	\return The newly created interpolator that references this interpolator.
	*/
	virtual HBhvInterpolator *CreateInstance(HBhvAnimation *animationinst) = 0;

	/*!
	This method replaces an array item at a given position.
	\param c The replacement keyframe.
	\param pos The array position for the replacement keyframe. 
	*/
	void  Replace(HKeyframe * c, int pos) { REPLACE_VARRAY(c, pos); }

	/*!
	Inserts a keyframe at the given position.
	\param piece The keyframe to insert
	\param pos The array position to insert item after.
	*/
	void  Insert(HKeyframe * piece, int pos = 0) { m_pArray.InsertAt(piece, pos); }

	/*!
	This method appends the given keyframe to the end of array.
	\param piece The keyframe you want to append.
	*/
	void  Append(HKeyframe * piece) { m_pArray.InsertAt(piece, m_pArray.Count()); }

	/*!
	This method removes a keyframe at the given position.
	\param pos The array position indicating where to remove the keyframe.
	*/
	void  Remove(int pos) { REMOVE_VARRAY(pos); }

	/*! \return The name of the interpolator. */
	const char * GetName() { return m_Name; }

	/*!
	Returns the keyframe that exists at the given position.
	\param pos The array position to retrieve.
	\return The keyframe at the given position.
	*/
	virtual HKeyframe *GetAt(int pos) { return m_pArray[pos]; }

	/*! \return The keyframe array.  Note this is for read-only purposes.  If you want to modify the keyframe
	array, use the methods provided in this class to do so.*/
	HKeyframe **GetArray() { return &m_pArray[0]; }

	/*! \return The number of entries in the keyframe array. */
	virtual int   GetArrayLength() { return (int)m_pArray.Count(); }
   
	/*!
	Duplicates a keyframe and inserts it at the position indicated the replace and next parameters.
	\param pos The position of the keyframe to duplicate.
	\param replace Pass true if you want to replace the item at pos with the duplicate and false to insert the duplicate 
	item after the pos item.
	\param next Pass true duplicate the item at given pos and false to duplicate the item before the given pos.
	*/	
	void Duplicate(int pos, bool replace, bool next);
	
	/*!
	This method copies a given keyframe and inserts the copy at a new location. 
	\param pos The position of the item to copy.
	\param adpos The target position.
	\param replace Pass true to replace item at the target position and false to insert after the target position.
	*/	
	void Copy(int pos, int adpos, bool replace);
 
	/*!
	Writes XML data to a buffer.
	*/
	virtual void Serialize(HUtilityXMLGenerator *xmlgen) {
		UNREFERENCED(xmlgen);
	}

	/*!
	Performs interpolation on the animation target given a keyframe and a fraction.
	\param keyframe Reference keyframe for this interpolation.
	\param fraction Fraction value from the specified keyframe to the next keyframe.  This value is from 0 to 1.
	*/
	virtual void Interpolate(int keyframe, float fraction) {
		UNREFERENCED(keyframe);
		UNREFERENCED(fraction);
	}

	virtual void Evaluate(int keyframe, float fraction, bool &hasPos, HPoint &pos, bool &hasQuat, HQuat &quat, bool &hasScale, HPoint &scale) {
		UNREFERENCED(keyframe);
		UNREFERENCED(fraction);
		UNREFERENCED(hasPos);
		UNREFERENCED(pos);
		UNREFERENCED(hasQuat);
		UNREFERENCED(quat);
		UNREFERENCED(hasScale);
		UNREFERENCED(scale);
	}
 
	/*! \return A pointer to the animation associated with this interpolator. */
	HBhvAnimation * GetAnimation() { return m_pAnimation; }

	/*!
	Associates an animation with this interpolater.
	*/
	void  SetAnimation(HBhvAnimation *animation) { m_pAnimation = animation; }

	/*!
	Calculates the translation components from modelling matrix of the target segment.
	\param translation Returns the x/y/z translation.
	*/
	void GetTranslationFromMatrix(HPoint &translation);
	/*!
	Calculates the rotation components from modelling matrix of the target segment.
	\param rotation Returns the x/y/z rotation.
	*/
	void GetRotationFromMatrix(HQuat &rotation);

	/*!
	Resets the interpolator instance. 
	*/	
	virtual void Reset() { };

protected:	

	/*!
	Sets the target as the one associated with this interpolator's animation instance.
	*/	
	void SetTarget();

	/*!
	Adds a translational component to the modelling matrix.
	\param trans The translational component.
	*/	
	void AddPositionToMatrix(HPoint &trans);

	/*!
	Adds a rotational component to the modelling matrix.
	\param rot The rotation matrix.
	*/	
	void AddRotationToMatrix(float rot[16]);

	/*!
	Adds a scaling component to modelling matrix.
	\param scale The scale component.
	*/	
	void AddScaleToMatrix(HPoint &scale);
	
	
	char						m_Name[BHV_MAX_NAME_LENGTH];		/*!< The name of the interpolator object.*/	 
	VArray< HKeyframe *>		m_pArray;							/*!< The keyframe array.*/	
	HBhvAnimation*				m_pAnimation;						/*!< A pointer to the animation that this interpolator belongs to.*/	
	HBhvInterpolator*			m_pInterpolatorInstance;			/*!< A pointer to instanced interpolator.*/	
	HC_KEY						m_pTarget;  						/*!< The key of interpolator target.*/	

};


//! The HBhvInterpolatorPosition class controls positional interpolation
/*!	
	HBhvInterpolatorPosition allows for either linear or spline based movement. This class can be used for camera interpolation as well other targets.
*/ 

class MVO_API HBhvInterpolatorPosition : public HBhvInterpolator
{
public:
	/*!
	Constructs an HBhvInterpolatorPosition object.
	\param animation The animation that this interpolator belongs to.
	\param name The name of the interpolator.
	*/
	HBhvInterpolatorPosition(HBhvAnimation *animation = 0, const char *name = 0);


	/*! \return Type of the interpolator which is 'Pos'. */
	const char * GetType();

	/*!
	In special cases, you may want an interpolator that references this interpolator without changing it. Use 
	this method to create the new interpolator passing a pointer to the animation that you want it to be associated with.
	\param ainst The animation instance that this interpolator is associated with.
	\return The newly created interpolator that references this interpolator.
	*/
	virtual HBhvInterpolator * CreateInstance(HBhvAnimation *ainst);

	/*!
	This method creates a new linear positional keyframe with the passed <i>pos</i> value and then inserts it
	after the passed position <i>l</i> into the keyframe array.
	\param pos The position value for the keyframe to be inserted.
	\param l The position in the array to insert the item after.
	*/
	void  InsertLinear(HPoint pos, int l = 0) { HKeyframeChannelLinear * linear = new HKeyframeChannelLinear; 
				linear->m_cp = pos; m_pArray.InsertAt(linear, l); }
	/*!
	Replaces the item at the given position in the keyframe array with a new linear positional keyframe that
	was created with the passed <i>pos</i> value.
	\param pos The position value for the new replacement keyframe.
	\param l The array position to replace.
	*/
	void  ReplaceLinear(HPoint pos, int l) { HKeyframeChannelLinear * c = new HKeyframeChannelLinear; 
				c->m_cp = pos; REPLACE_VARRAY(c, l) }
	/*!
	This method creates a new non-linear positional keyframe with the passed <i>pos</i> value and then inserts it
	after the passed position <i>l</i> into the keyframe array.
	\param pos The position value for the keyframe to be insert.
	\param l The array position to insert item after.
	*/
	void  InsertCurve(HPoint pos, int l = 0) { HKeyframeChannelCurve * linear = new HKeyframeChannelCurve; 
				linear->m_cp = pos; m_pArray.InsertAt(linear, l); }
	/*!
	Replaces the item at the given position in the keyframe array with a new non-linear positional keyframe that
	was created with the passed <i>pos</i> value.
	\param pos The position value for the new replacement keyframe.
	\param l The array position to replace.
	*/
	void  ReplaceCurve(HPoint pos, int l) { HKeyframeChannelCurve * c = new HKeyframeChannelCurve; 
				c->m_cp = pos; REPLACE_VARRAY(c, l) }

	/*!
	This method creates a new discrete positional keyframe with the passed <i>pos</i> value and then inserts it
	after the passed position <i>l</i> into the keyframe array.
	\param pos The position value for the new keyframe to be inserted.
	\param l The array position to insert item after.
	*/
	void  InsertDiscrete(HPoint pos, int l = 0) { HKeyframeChannelDiscrete * d = new HKeyframeChannelDiscrete; 
				d->m_cp = pos; m_pArray.InsertAt(d, l); }

	/*!
	Writes XML data to a buffer using the passed XML generator.
	*/
	virtual void Serialize(HUtilityXMLGenerator *xmlgen);
	/*!
	This is XML read callback which parses XML data and then populates the member variables with the associated values.
	*/
	static	void *XMLCallback(HUtilityXMLTag *xt, bool open, void *m_pExtraData);

	/*!
	Performs an interpolation on this interpolator's animation target.
	\param keyframe The reference keyframe for this interpolation.
	\param fraction The fraction value from the specified keyframe to the next keyframe.  The value is from 0 to 1.
	*/
	virtual void Interpolate(int keyframe, float fraction);

	virtual void Evaluate(int keyframe, float fraction, bool &hasPos, HPoint &pos, bool &hasQuat, HQuat &quat, bool &hasScale, HPoint &scale);

	/*!
	Resets the interpolator instance. 
	*/	
	virtual void Reset();
	
	/*!
	Calculates the interpolated positional value given a keyframe and fraction to the next keyframe.
	\param keyframe The reference keyframe for this interpolation.
	\param fraction The fractional value from the specified keyframe to the next keyframe.  This value is from 0 to 1.
	\param res The result of the interpolation.
	*/
	virtual void CalculatePos(int keyframe, float fraction, HPoint &res);

protected:
	/*!
	Calculates the interpolated camera orientation and position for camera types that are position driven.
	\param pos New Camera Position Reference  
	*/
	virtual void InterpolateCamera(HPoint &pos, bool simulate = false);

	/*!
	Calculates the interpolated camera orientation for camera types that are target driven.
	\param pos New Camera Position Reference  
	*/
	virtual void InterpolateCamera2(HPoint &pos, bool simulate = false);

	/*!
	This is a helper method that calculates the tangents for all the entries in the keyframe array. You can call
	this method before doing an interpolation.
	*/
	virtual void CalculateAllTangents(); 
};

//! The HBhvInterpolatorTrail class draws a line trail from one keyframe position to the next.
/*!	
	HBhvInterpolatorTrail honors all modifiers from HBhvInterpolatorPosition.
*/ 
class MVO_API HBhvInterpolatorTrail : public HBhvInterpolatorPosition
{
public:
	/*!
	Constructs an HBhvInterpolatorTrail object.
	\param animation The animation that this interpolator belongs to.
	\param name The name of the interpolator.
	*/
	HBhvInterpolatorTrail(HBhvAnimation *animation = 0, const char *name = 0);
	~HBhvInterpolatorTrail();


	/*! \return Type of the interpolator is 'Trail'.*/
	const char * GetType();

	/*!
	In special cases, you may want an interpolator that references this interpolator without changing it. Use 
	this method to create the new interpolator passing a pointer to the animation that you want it to be associated with.
	\param ainst The animation instance that this interpolator is associated with.
	\return The newly created interpolator that references this interpolator.
	*/
	virtual HBhvInterpolator * CreateInstance(HBhvAnimation *ainst);
 
	/*!
	Writes XML data to a buffer using the passed XML generator.
	*/
	virtual void Serialize(HUtilityXMLGenerator *xmlgen);
	/*!
	This is XML read callback which parses XML data and then populates the member variables with the associated values.
	*/
	static	void *XMLCallback(HUtilityXMLTag *xt, bool open, void *m_pExtraData);

	/*!
	Performs an interpolation on the animation target.
	\param keyframe The reference keyframe for this interpolation.
	\param fraction The fraction value from the specified keyframe to the next keyframe.  The value is from 0 to 1.
	*/
	virtual void Interpolate(int keyframe, float fraction);
	/*!
	Resets interpolator instance. 
	*/	
	virtual void Reset();

	/*!
	Sets up the Trail
	*/
	void Init();
	void SetTrailType(TrailInterpolatorType tt) { m_TrailType = tt; }
	void SetTrailColor(const char *color) { strncpy_s(m_Color, color, sizeof(m_Color)); }
	void SetTrailWeight(int weight) { m_Weight = weight; }
	void SetTrailStyle(const char *style) { strncpy_s(m_Style, style, sizeof(m_Style)); }
	
	
protected:
	TrailInterpolatorType m_TrailType; /*!< For internal use only.*/
	char 				m_Color[MVO_SMALL_BUFFER_SIZE]; /*!< For internal use only.*/
	int					m_Weight; /*!< For internal use only.*/
	char				m_Style[MVO_SMALL_BUFFER_SIZE]; /*!< For internal use only.*/
	HC_KEY				m_TrailSegment, m_trailKey;			/*!< For internal use only.*/
	int					m_lastkeyframe;
	bool				m_initialized;
 
};

//! The HBhvInterpolatorAxisRotate class controls rotation around an arbitrary axis
/*!	
	
 */ 
class MVO_API HBhvInterpolatorAxisRotate : public HBhvInterpolator
{
public:
	/*!
	Constructs an HBhvInterpolatorAxisRotate object.
	\param animation The animation that this interpolator belongs to.
	\param name The name of of the interpolator.
	*/
	HBhvInterpolatorAxisRotate(HBhvAnimation *animation = 0, const char *name = 0);

	/*!
	Sets the axis of rotation.
	*/
	void SetAxis(float x, float y, float z) {m_axis.x = x; m_axis.y = y; m_axis.z = z; }

	/*!
	Returns the axis of rotation.
	*/
	void GetAxis (HPoint &axis) { axis = m_axis; }

	/*! \return The type of the interpolator which is 'AxisRot'.*/
	const char * GetType();

	/*!
	In special cases, you may want an interpolator that references this interpolator without changing it. Use 
	this method to create the new interpolator passing a pointer to the animation that you want it to be associated with.
	\param ainst The animation instance that this interpolator is associated with.
	\return The newly created interpolator that references this interpolator.
	*/
	virtual	HBhvInterpolator * CreateInstance(HBhvAnimation *ainst);
  
	/*!
	This method creates a new axis rotation keyframe with the passed <i>angle</i> value and then inserts it
	after the passed position <i>l</i> into the keyframe array.
	\param angle The angle for the new keyframe.
	\param l The array position to insert new keyframe after.
	*/
	void  Insert(float angle, int l = 0) { HKeyframeAxisRotation* linear = new HKeyframeAxisRotation; 
				linear->m_angle = angle; m_pArray.InsertAt(linear, l); }
	/*!
	Replaces the item at the given position in the keyframe array with a new axis rotation keyframe that
	was created with the passed <i>angle</i> value.
	\param angle The angle for the replacement keyframe.
	\param l The array position to replace.
	*/
	void  Replace(float angle, int l) { HKeyframeAxisRotation * c = new HKeyframeAxisRotation; 
				c->m_angle = angle; REPLACE_VARRAY(c, l); }

	/*!
	This is XML read callback which parses XML data and then populates the member variables with the associated values.
	*/
	static	void *XMLCallback(HUtilityXMLTag *xt, bool open, void *m_pExtraData);

	/*!
	Writes XML data to a buffer using the passed XML generator.
	*/
	virtual void Serialize(HUtilityXMLGenerator *xmlgen);


	/*!
	Performs interpolation on the animation target.
	\param keyframe The reference keyframe for this interpolation.
	\param fraction The fraction value from the specified keyframe to the next keyframe.  The value is from 0 to 1.
	*/
	virtual void Interpolate(int keyframe, float fraction);
	virtual void Evaluate(int keyframe, float fraction, bool &hasPos, HPoint &pos, bool &hasQuat, HQuat &quat, bool &hasScale, HPoint &scale);

protected:
	HPoint			m_axis;				/*!<For internal use only.*/
 
};


//! The HBhvInterpolatorColor class controls color attribute changes.
	
class MVO_API HBhvInterpolatorColor : public HBhvInterpolator
{
public:
	/*!
	Constructs an HBhvInterpolatorColor object.
	\param animation The animation that this interpolator belongs to.
	\param name The name of the interpolator.
	*/

	HBhvInterpolatorColor(HBhvAnimation *animation = 0, const char *name = 0);
	/*! \return The type of the interpolator which is 'Color'.*/
	const char * GetType();

	/*!
	In special cases, you may want an interpolator that references this interpolator without changing it. Use 
	this method to create the new interpolator passing a pointer to the animation that you want it to be associated with.
	\param ainst The animation instance that this interpolator is associated with.
	\return The newly created interpolator that references this interpolator.
	*/
	virtual HBhvInterpolator * CreateInstance(HBhvAnimation *ainst);

	/*!
	This method creates a new keyframe with the passed color value and then inserts it after the given
	position <i>l</i> in the keyframe array.
	\param pos The color value you want to set for the new keyframe.  Pass r,g, and b
	values in the x,y and z parameters respectively in HPoint.
	\param l array position to insert item after
	*/
	void  Insert(HPoint pos, int l = 0) { HKeyframeChannelLinear* color = new HKeyframeChannelLinear; 
				color->m_cp = pos; m_pArray.InsertAt(color, l); }
	/*!
	Replaces the item at the given position in the keyframe array with a new keyframe that
	was created with the passed color value.
	\param pos The color value for the replacement keyframe. Pass r,g, and b
	values in the x,y and z parameters respectively in HPoint.  
	\param l array position to replace
	*/
	void  Replace(HPoint pos, int l) { HKeyframeChannelLinear * c = new HKeyframeChannelLinear; 
				c->m_cp = pos; REPLACE_VARRAY(c, l); }

	/*!
	Writes XML data to a buffer using the passed XML generator.
	*/
	virtual void Serialize(HUtilityXMLGenerator *xmlgen);

	/*!
	This is XML read callback which parses XML data and then populates the member variables with the associated values.
	*/
	static void *XMLCallback(HUtilityXMLTag *xt, bool open, void *m_pExtraData);
	
	/*!
	Sets the geometry type that color changes applies to like faces, lines, etc...
	*/
	void SetGeomType (const char *geomtype);

	/*! \return The geometry type that color changes apply to like faces, lines, etc... */
	char * GetGeomType () { return m_GeomType; }

	/*!
	Sets color components attributes like diffuse, specular and gloss that color changes will be applied to.
	\param ColorComponent The \ref Set_Color_Face_Color_Options "color options" that can be set in HC_Set_Color for faces.
	*/
	void SetColorComponent (const char *ColorComponent);

	/*! \return The \ref Set_Color_Face_Color_Options "color components" that color changes are applied. */
	char * GetColorComponent () { return m_ColorComponent; }

	/*!
	Performs an interpolation on the animation target.
	\param keyframe The reference keyframe for this interpolation.
	\param fraction The fraction value from the specified keyframe to the next keyframe.  The value is from 0 to 1.
	*/
	virtual void Interpolate(int keyframe, float fraction);

protected:
	char				m_ColorComponent[MVO_BUFFER_SIZE];  /*!< For internal use only.*/
	char				m_GeomType[MVO_BUFFER_SIZE];		/*!< For internal use only.*/
 
};


/*
class MVO_API HBhvInterpolatorQuat : public HBhvInterpolator
{
public:
	HBhvInterpolatorQuat(const char *name = 0);
	const char * GetType();

	virtual HBhvInterpolatorInstance * CreateInstance(HBhvanimation *ainst);
	void  Insert(HQuat pos, int l) { HKeyframeQuatSlerp * rot = new HKeyframeQuatSlerp; 
				rot->m_quat = pos; m_pArray.InsertAt(rot, l); }
	void  Replace(HQuat pos, int l) { HKeyframeQuatSlerp * c = new HKeyframeQuatSlerp; 
				c->m_quat = pos; REPLACE_VARRAY(c, l); }

};



class MVO_API HBhvInterpolatorInstanceQuat : public HBhvInterpolatorInstance
{
public:
	HBhvInterpolatorInstanceQuat(HBhvInterpolatorQuat *ip, HBhvanimation *ainst) : HBhvInterpolatorInstance((HBhvInterpolator *)ip, ainst) {}
	virtual void Interpolate(int keyframe, float fraction);
 
protected:

	virtual void InterpolateCamera(float *quat);

};

*/



//! The HBhvInterpolatorScale class controls scaling.  This class can be used for camera interpolation as well other targets.

class MVO_API HBhvInterpolatorScale : public HBhvInterpolator
{
public:

	/*!
	Calculates the interpolated positional value given a keyframe and fraction to the next keyframe.
	\param keyframe The reference keyframe for this interpolation/
	\param fraction The fractional value from the specified keyframe to the next keyframe.  This value is from 0 to 1.
	\param res The result of the interpolation.
	*/
	virtual void CalculatePos(int keyframe, float fraction,  HPoint &res);


	/*!
	This is a helper method that calculates the tangents for all the entries in the keyframe array. You can call
	this method before doing an interpolation.
	*/
	virtual void CalculateAllTangents(); 

	/*!
	Resets this interpolator instance. 
	*/	
	virtual void Reset();

	/*!
	Constructs an HBhvInterpolatorScale object.
	\param animation The animation that this interpolator belongs to.
	\param name The name of the interpolator.
	*/
	HBhvInterpolatorScale(HBhvAnimation *animation = 0, const char *name = 0);

	/*! \return The type of this interpolator which is 'Scale'. */
	const char * GetType();

	/*!
	In special cases, you may want an interpolator that references this interpolator without changing it. Use 
	this method to create the new interpolator passing a pointer to the animation that you want it to be associated with.
	\param ainst The animation instance that this interpolator is associated with.
	\return The newly created interpolator that references this interpolator.
	*/
	virtual HBhvInterpolator * CreateInstance(HBhvAnimation *ainst);

	/*!
	This method creates a new keyframe with the passed scaling factor and then inserts it after the given
	position <i>l</i> in the keyframe array.
	\param pos The scale factors for the new keyframe.
	\param l The array position to insert item after.
	*/
	void  Insert(HPoint pos, int l = 0) { HKeyframeChannelLinear * scale = new HKeyframeChannelLinear; 
				scale->m_cp = pos; m_pArray.InsertAt(scale, l); }
	/*!
	Replaces the item at the given position in the keyframe array with a new keyframe that
	was created with the passed scaling factors.
	\param pos The scale factors for the replacement keyframe.
	\param l The array position to replace.
	*/
	void  Replace(HPoint pos, int l) { HKeyframeChannelLinear * c = new HKeyframeChannelLinear; 
				c->m_cp = pos; REPLACE_VARRAY(c, l) }

	/*!
	Writes XML data to a buffer using the passed XML generator.
	*/
	virtual void Serialize(HUtilityXMLGenerator *xmlgen);

	/*!
	This is XML read callback which parses XML data and then populates the member variables with the associated values.
	*/
	static void *XMLCallback(HUtilityXMLTag *xt, bool open, void *m_pExtraData);

	/*!
	Performs an interpolation on the animation target.
	\param keyframe The reference keyframe for this interpolation.
	\param fraction The fraction value from the specified keyframe to the next keyframe.  The value is from 0 to 1.
	*/
	virtual void Interpolate(int keyframe, float fraction);
	void Evaluate(int keyframe, float fraction, bool &hasPos, HPoint &pos, bool &hasQuat, HQuat &quat, bool &hasScale, HPoint &scale);

 
protected:
	/*!
	Calculates the interpolated camera orientation and position for camera types that are position driven.
	\param pos New Camera Position Reference  
	*/
	virtual void InterpolateCamera(HPoint &Scale, bool simulate = false);
	/*!
	Calculates the interpolated camera orientation for camera types that are target driven.
	\param pos New Camera Position Reference  
	*/	
	virtual void InterpolateCamera2(HPoint &Scale, bool simulate = false);

};

 

//! The HBhvInterpolatorQuatSquad class controls quaternion based rotation. This class can be used for camera interpolation as well other targets.
class MVO_API HBhvInterpolatorQuatSquad : public HBhvInterpolator
{
public:

	/*!
	Constructs an HBhvInterpolatorQuatSquad object.
	\param animation Animation that this interpolator belongs to
	\param name Name of Interpolator
	*/
	HBhvInterpolatorQuatSquad(HBhvAnimation *animation = 0, const char *name = 0);

	/*! \return The type of the interpolator which is 'QuatRot'. */
	const char * GetType();

	/*!
	In special cases, you may want an interpolator that references this interpolator without changing it. Use 
	this method to create the new interpolator passing a pointer to the animation that you want it to be associated with.
	\param ainst The animation instance that this interpolator is associated with.
	\return The newly created interpolator that references this interpolator.
	*/
	virtual HBhvInterpolator * CreateInstance(HBhvAnimation *ainst);

	/*!
	This method creates a new quaternion rotation keyframe with the passed <i>q</i> value and then inserts it
	after the passed position <i>l</i> into the keyframe array.
	\param q The quaternion to use for new keyframe.
	\param l The array position to insert item after.
	*/
	void  Insert(HQuat q, int l = 0) { HKeyframeQuatSquad * rot = new HKeyframeQuatSquad; 
				rot->m_quat = q; m_pArray.InsertAt(rot, l); }
	
	/*!
	This method creates a new linear quaternion rotation keyframe with the passed <i>q</i> value and then inserts it
	after the passed position <i>l</i> into the keyframe array.
	\param q The quaternion to use for new keyframe.
	\param l The array position to insert item after.
	*/
	void  InsertLinear(HQuat q, int l = 0) { HKeyframeQuatSquad * rot = new HKeyframeQuatSquad; 
				rot->m_quat = q; rot->m_bLinear = true; m_pArray.InsertAt(rot, l); }

	/*!
	Replaces the item at the given position in the keyframe array with a new quaternion rotation keyframe that
	was created with the passed <i>q</i> value.
	\param q The quaternion to use for the replace keyframe
	\param l The array position to replace.
	*/
	void  Replace(HQuat q, int l) { HKeyframeQuatSquad * c = new HKeyframeQuatSquad; 
				c->m_quat = q; REPLACE_VARRAY(c, l); }
	/*!
	Replaces the item at the given position in the keyframe array with a new linear quaternion rotation keyframe that
	was created with the passed <i>q</i> value.
	\param q The quaternion to use for the replace keyframe
	\param l The array position to replace.
	*/
	void  ReplaceLinear(HQuat q, int l) { HKeyframeQuatSquad * c = new HKeyframeQuatSquad; 
				c->m_quat = q; c->m_bLinear = true; REPLACE_VARRAY(c, l); }

	/*!
	Recalculates the quaternion array to ensure shortest rotation path.
	*/ 	
	void AdjustQuaternions();

	/*!
	Writes XML data to a buffer using the passed XML generator.
	*/
	virtual void Serialize(HUtilityXMLGenerator *xmlgen);

	/*!
	This is XML read callback which parses XML data and then populates the member variables with the associated values.
	*/
	static void *XMLCallback(HUtilityXMLTag *xt, bool open, void *m_pExtraData);

	/*!
	Performs an interpolation on the animation target.
	\param keyframe The reference keyframe for this interpolation.
	\param fraction The fraction value from the specified keyframe to the next keyframe.  The value is from 0 to 1.
	*/
	virtual void Interpolate(int keyframe, float fraction);
	virtual void Evaluate(int keyframe, float fraction, bool &hasPos, HPoint &pos, bool &hasQuat, HQuat &quat, bool &hasScale, HPoint &scale);

	/*!
	Calculates the quaternion interpolation given a reference keyframe and a fraction.
	\param keyframe The reference keyframe for this interpolation.
	\param fraction The fraction value from the specified keyframe to the next keyframe.  The value is from 0 to 1.
	\param res Returns the result of the calculation.  	
	*/
	virtual void CalculateQuat(int keyframe, float fraction,  HQuat &res);



protected:
	/*!
	Calculates the interpolated camera orientation and position for camera types that are position driven.
	\param pos New Camera Position Reference  
	*/
	virtual void InterpolateCamera(HQuat &quat, bool simulate = false);

	/*!
	Calculates the interpolated camera orientation for camera types that are target driven.
	\param pos New Camera Position Reference  
	*/	
	virtual void InterpolateCamera2(HQuat &quat, bool simulate = false);

};

 


//! The HBhvInterpolatorAttSwitch class switches attributes on segments
/*! HBhvInterpolatorAttSwitch uses HC_Style_Segment() to change the attribute on the segments 
	specified in the keyframe list.
*/
class MVO_API HBhvInterpolatorAttSwitch : public HBhvInterpolator
{
public:
	/*!
	Constructs an HBhvInterpolatorAttSwitch object.
	\param animation The animation that this interpolator belongs to.
	\param name The name of the interpolator.
	*/
	HBhvInterpolatorAttSwitch(HBhvAnimation *animation = 0, const char *name = 0);

	/*! \return The type of the interpolator which is 'AttSwitch'. */
	const char * GetType();

	/*!
	In special cases, you may want an interpolator that references this interpolator without changing it. Use 
	this method to create the new interpolator passing a pointer to the animation that you want it to be associated with.
	\param ainst The animation instance that this interpolator is associated with.
	\return The newly created interpolator that references this interpolator.
	*/
	virtual HBhvInterpolator * CreateInstance(HBhvAnimation *ainst);

	/*!
	This method creates a new keyframe with the passed style segment path <i>t</i>  and then inserts it
	after the passed position <i>l</i> into the keyframe array.
	\param t The style segment path for the new keyframe.
	\param l The array position to insert item after.
	*/
	void  Insert(const char * t, int l = 0) { HKeyframeString* AttSwitch = new HKeyframeString; 
				AttSwitch->SetTarget(t); m_pArray.InsertAt(AttSwitch, l); }

	/*!
	Replaces the item at the given position in the keyframe array with a new keyframe that
	was created with the passed style segment path <i>spath</i>.
	\param spath The style segment path for the new replacement keyframe.
	\param l The array position to replace.
	*/
	void  Replace(const char *spath, int l) { HKeyframeString * c = new HKeyframeString; 
				c->SetTarget(spath); REPLACE_VARRAY(c, l) }

	/*!
	Writes XML data to a buffer using the passed XML generator.
	*/
	virtual void Serialize(HUtilityXMLGenerator *xmlgen);

	/*!
	This is XML read callback which parses XML data and then populates the member variables with the associated values.
	*/
	static void *XMLCallback(HUtilityXMLTag *xt, bool open, void *m_pExtraData);

	/*!
	Performs interpolation on the animation target.
	\param keyframe The reference keyframe for this interpolation.
	\param fraction The fraction value from the specified keyframe to the next keyframe.  The value is from 0 to 1.
	*/
	virtual void Interpolate(int keyframe, float fraction);
 
};


 




//! The HBhvInterpolatorVertexMorph class performs interpolation by interpolating the individual vertex parameters of an object.  The result of the animation is that object changes shape but retains the same set of vertices.
class MVO_API HBhvInterpolatorVertexMorph : public HBhvInterpolator
{
public:

	/*!
	Constructs an HBhvInterpolatorVertexMorph object.
	\param animation The animation that this interpolator belongs to.
	\param name The name of the interpolator.
	*/
	HBhvInterpolatorVertexMorph(HBhvAnimation *animation = 0, const char *name = 0);
	~HBhvInterpolatorVertexMorph();

	/*! \return The type of the interpolator 'VertexMorph'.*/
	const char * GetType();

	/*!
	In special cases, you may want an interpolator that references this interpolator without changing it. Use 
	this method to create the new interpolator passing a pointer to the animation that you want it to be associated with.
	\param ainst The animation instance that this interpolator is associated with.
	\return The newly created interpolator that references this interpolator.
	*/
	virtual HBhvInterpolator * CreateInstance(HBhvAnimation *ainst);

	/*!
	This method creates a new keyframe with the passed Morph Data position index and then inserts it
	after the passed position <i>l</i> into the keyframe array.  To use this method, you must first add your vertex position
	information to the Morph Data using AddMorphData().  Once you have done that, note the position index that 
	your vertex information was added to the Morph Data array.  This will be the value of the parameter <i>mident</i>.
	\param mident The Morph Data position index to your vertex array to use for the new keyframe.  Note that although
	the index is an int, you can use itoa() to convert the index position.
	\param l The array position to insert item after.
	*/
	void  Insert(char * mident, int l = 0) { HKeyframeString* VertexMorph = new HKeyframeString; 
				VertexMorph->SetTarget(mident); m_pArray.InsertAt(VertexMorph, l); }
	void  InsertDiscrete(char * mident, int l = 0) { HKeyframeString* VertexMorph = new HKeyframeString; 
				VertexMorph->SetTarget(mident); VertexMorph->m_bDiscrete = true; m_pArray.InsertAt(VertexMorph, l); }

	/*!
	Replaces the item at the given position in the keyframe array with a new keyframe that
	was created with the passed morph identifier.  To use this method, you must first add your vertex position
	information to the Morph Data using AddMorphData().  Once you have done that, note the position index that 
	your vertex information was added to the Morph Data array.  This will be the value of the parameter <i>t</i>.
	\param t The Morph Data position index to your vertex array to use for the replace keyframe.  Note that although
	the index is an int, you can use itoa() to convert the index position.
	\param l The array position to replace.
	*/
	void  Replace(char *t, int l) { HKeyframeString * c = new HKeyframeString; 
				c->SetTarget(t); REPLACE_VARRAY(c, l) }

	/*!
	Writes XML data to a buffer using the passed XML generator.
	*/
	virtual void Serialize(HUtilityXMLGenerator *xmlgen);

	/*!
	This is XML read callback which parses XML data and then populates the member variables with the associated values.
	*/
	static void *XMLCallback(HUtilityXMLTag *xt, bool open, void *m_pExtraData);
	
	/*!
	Performs an interpolation on the animation target.
	\param keyframe The reference keyframe for this interpolation.
	\param fraction The fraction value from the specified keyframe to the next keyframe.  The value is from 0 to 1.
	*/
	virtual void Interpolate(int keyframe, float fraction);

	/*!
	This method adds a point array to morph data array.  Use this method to morph your data so that it can be added
	to the keyframe array for this interpolator.  Note Insert() and Replace() take the morph data in the form of 
	its indexed position in the Morph Data array.  
	\param md  Array of Points
	\param pnum	 Number of points in array
	\param model A pointer to the base model. The Point array is stored as a shell in a special MorphData segment underneath the model segment
	\param pos Optional index into the Morph Data array which indicates where to put the point array
	*/
	virtual void AddMorphData(HPoint *md, int pnum, HBaseModel *model, int pos = -1);
 
	/*! \return A pointer to the morph data array. */
	VArray < void *> GetMorphData() { return m_pMorphData; }

	/*! \return The number of entries in the morph data array. */
	int GetMorphDataLength() { return m_MorphDataLength; }
private:	
 
	void *GetUserInfo(char *target); /*!< For internal use only.*/
	void * GetMorphData(int i); /*!< For internal use only.*/

	VArray < void *>		m_pMorphData;		/*!< For internal use only.*/
	HC_KEY					m_shelltarget;		/*!< For internal use only.*/
	int						m_MorphDataLength; /*!< For internal use only.*/
};



//! The HBhvInterpolatorColorMorph class performs an interpolation on the face colors of your target object.
class MVO_API HBhvInterpolatorColorMorph : public HBhvInterpolator
{
public:

	/*!
	Constructs an HBhvInterpolatorColorMorph object.
	\param animation The animation that this interpolator belongs to.
	\param name The name of the interpolator.
	*/
	HBhvInterpolatorColorMorph(HBhvAnimation *animation = 0, const char *name = 0);
	~HBhvInterpolatorColorMorph();

	/*! \return The type of the interpolator which is 'ColorMorph'. */
	const char * GetType();

	/*!
	In special cases, you may want an interpolator that references this interpolator without changing it. Use 
	this method to create the new interpolator passing a pointer to the animation that you want it to be associated with.
	\param ainst The animation instance that this interpolator is associated with.
	\return The newly created interpolator that references this interpolator.
	*/
	virtual HBhvInterpolator * CreateInstance(HBhvAnimation *ainst);

	/*!
	This method creates a new keyframe with the passed Morph Data position index and then inserts it
	after the passed position <i>l</i> into the keyframe array.  To use this method, you must first add your 
	face color information to the Morph Data using AddMorphData().  Once you have done that, note the position index that 
	your vertex information was added to the Morph Data array.  This will be the value of the parameter <i>mident</i>.
	\param mident The Morph Data position index to your vertex array to use for the new keyframe.  Note that although
	the index is an int, you can use itoa() to convert the index position.
	\param l The array position to insert item after.
		*/
	void  Insert(char * mident, int l = 0) { HKeyframeString* ColorMorph = new HKeyframeString; 
				ColorMorph->SetTarget(mident); m_pArray.InsertAt(ColorMorph, l); }
	void  InsertDiscrete(char * mident, int l = 0) { HKeyframeString* ColorMorph = new HKeyframeString; 
				ColorMorph->SetTarget(mident); ColorMorph->m_bDiscrete = true; m_pArray.InsertAt(ColorMorph, l); }
	/*!
	Replaces the item at the given position in the keyframe array with a new keyframe that
	was created with the passed morph identifier.  To use this method, you must first add your face color 
	information to the Morph Data using AddMorphData().  Once you have done that, note the position index that 
	your vertex information was added to the Morph Data array.  This will be the value of the parameter <i>t</i>.
	\param t The Morph Data position index to your vertex array to use for the replace keyframe.  Note that although
	the index is an int, you can use itoa() to convert the index position.
	\param l The array position to replace.
	*/
	void  Replace(char *t, int l) { HKeyframeString * c = new HKeyframeString; 
				c->SetTarget(t); REPLACE_VARRAY(c, l) }

	/*!
	Writes XML data to a buffer using the passed XML generator.
	*/
	virtual void Serialize(HUtilityXMLGenerator *xmlgen);

	/*!
	This is XML read callback which parses XML data and then populates the member variables with the associated values.
	*/
	static void *XMLCallback(HUtilityXMLTag *xt, bool open, void *m_pExtraData);
	
	/*!
	Performs an interpolation on the animation target.
	\param keyframe The reference keyframe for this interpolation.
	\param fraction The fraction value from the specified keyframe to the next keyframe.  The value is from 0 to 1.
	*/
	virtual void Interpolate(int keyframe, float fraction);
 
	/*! \return A pointer to the morph data array. */
	VArray < void *> GetMorphData() { return m_pMorphData; }

	/*! \return The number of entries in the morph data array. */
	int GetMorphDataLength() { return m_MorphDataLength; }
	/*!
	This method adds an array of colors for the vertices in the target object to morph data array.  Use this method 
	to morph your data so that it can be added to the keyframe array for this interpolator.  Note Insert() and 
	Replace() take the morph data in the form of its indexed position in the morph data array.  
	\param md  The array of colors. For an HPoint, pass the r, g and b values in the x, y and z parameters if you have set
	the UseFIndex flag to false.  If UseFIndex is true, then pass an index to the color lookup table for the x parameter of
	each HPoint in the array.
	\param pnum	 Number of items in array.
	\param model A pointer to the base model. The point array is stored as a shell in a special MorphData segment underneath the model segment.
	\param pos Optional index into the Morph Data array which indicates where to put the point array.
	*/
	virtual void AddMorphData(HPoint *md, int pnum, HBaseModel *model, int pos = -1);

	/*! Use this method to indicate that HOOPS should use a lookup table for colors in this interpolation.  When creating 
	morph data in AddMorphData, instead of passing RGB values in the x,y,z parameters of #HPoint, pass the color lookup index 
	in x parameter.
	\param tf Pass true to use a lookup table or false to use RGB values. 
	*/
	virtual void SetUseFIndex(bool tf);

private:	
 
	void *GetUserInfo(char *target);
	void * GetMorphData(int i);
	VArray < void *>		m_pMorphData;		/*!< For internal use only.*/
	 HC_KEY					m_shelltarget;		/*!< For internal use only.*/
	int						m_MorphDataLength;
	bool					m_bUseFIndex;
};

 


//! The HBhvInterpolatorSegSwitch performs animation by switching on and off the visibility of segments.
class MVO_API HBhvInterpolatorSegSwitch : public HBhvInterpolator
{
public:

	/*!
	Constructs an HBhvInterpolatorSegSwitch object.
	\param animation The animation that this interpolator belongs to.
	\param name The name of the interpolator.
	*/
	HBhvInterpolatorSegSwitch(HBhvAnimation *animation = 0, const char *name = 0);
	~HBhvInterpolatorSegSwitch();

	/*! \return The type of the interpolator which is 'SegSwitch'. */
	const char * GetType();

	/*!
	In special cases, you may want an interpolator that references this interpolator without changing it. Use 
	this method to create the new interpolator passing a pointer to the animation that you want it to be associated with.
	\param ainst The animation instance that this interpolator is associated with.
	\return The newly created interpolator that references this interpolator.
	*/
	virtual HBhvInterpolator * CreateInstance(HBhvAnimation *ainst);

	/*!
	This method creates a new keyframe with the passed segment path <i>t</i> and then inserts it
	after the passed position <i>l</i> into the keyframe array.
	\param t Segment path to insert
	\param l array position to insert item after
	*/
	void  Insert(char * t, int l = 0) { HKeyframeString* SegSwitch = new HKeyframeString; 
				SegSwitch->SetTarget(t); m_pArray.InsertAt(SegSwitch, l); }

	/*!
	Replaces the item at the given position in the keyframe array with a new keyframe that
	was created with the passed segment path <i>t</i>.
	\param spath Segment path to replace with
	\param l array position to replace
	*/
	void  Replace(char *spath, int l) { HKeyframeString * c = new HKeyframeString; 
				c->SetTarget(spath); REPLACE_VARRAY(c, l) }

	/*!
	Writes XML data to a buffer using the passed XML generator.
	*/
	virtual void Serialize(HUtilityXMLGenerator *xmlgen);

	/*!
	This is XML read callback which parses XML data and then populates the member variables with the associated values.
	*/
	static void *XMLCallback(HUtilityXMLTag *xt, bool open, void *m_pExtraData);

	/*!
	Performs an interpolation on the animation target.
	\param keyframe The reference keyframe for this interpolation.
	\param fraction The fraction value from the specified keyframe to the next keyframe.  The value is from 0 to 1/
	*/
	virtual void Interpolate(int keyframe, float fraction);

	/*!
	Resets the interpolator instance by turning off the visibility of all segments in the keyframe list.
	*/	
	virtual void Reset();
		
};




//! The HBhvInterpolatorInstanceCreate creates a new instance of the target object at the location in the segment hierachy specified in the keyframe
class MVO_API HBhvInterpolatorInstanceCreate : public HBhvInterpolator
{
public:

	/*!
	Constructs an HBhvInterpolatorInstanceCreate object.
	\param animation The animation that this interpolator belongs to.
	\param name The name of the interpolator.
	*/
	HBhvInterpolatorInstanceCreate(HBhvAnimation *animation = 0, const char *name = 0);
	~HBhvInterpolatorInstanceCreate();

	/*! \return The type of the interpolator which is 'InstanceCreate'. */
	const char * GetType();

	/*!
	In special cases, you may want an interpolator that references this interpolator without changing it. Use 
	this method to create the new interpolator passing a pointer to the animation that you want it to be associated with.
	\param ainst The animation instance that this interpolator is associated with.
	\return The newly created interpolator that references this interpolator.
	*/
	virtual HBhvInterpolator * CreateInstance(HBhvAnimation *ainst);

	/*!
	This method creates a new keyframe with the passed segment path <i>t</i> and then inserts it
	after the passed position <i>l</i> into the keyframe array.
	\param t Segment path to insert
	\param l array position to insert item after
	*/
	void  Insert(char * t, char *t2, char *t3, int l = 0) { HKeyframe3String* InstanceCreate = new HKeyframe3String; 
				InstanceCreate->SetTarget(t,t2,t3); m_pArray.InsertAt(InstanceCreate, l); }

	/*!
	Replaces the item at the given position in the keyframe array with a new keyframe that
	was created with the passed segment path <i>t</i>.
	\param spath Segment path to replace with
	\param l array position to replace
	*/
	void  Replace(char *t, char *t2, char *t3, int l) { HKeyframe3String * c = new HKeyframe3String; 
				c->SetTarget(t,t2,t3); REPLACE_VARRAY(c, l) }

	/*!
	Writes XML data to a buffer using the passed XML generator.
	*/
	virtual void Serialize(HUtilityXMLGenerator *xmlgen);

	/*!
	This is XML read callback which parses XML data and then populates the member variables with the associated values.
	*/
	static void *XMLCallback(HUtilityXMLTag *xt, bool open, void *m_pExtraData);

	/*!
	Performs an interpolation on the animation target.
	\param keyframe The reference keyframe for this interpolation.
	\param fraction The fraction value from the specified keyframe to the next keyframe.  The value is from 0 to 1/
	*/
	virtual void Interpolate(int keyframe, float fraction);

	/*!
	Resets the interpolator instance by turning off the visibility of all segments in the keyframe list.
	*/	
	virtual void Reset();

	static void DeciperString(HBhvInterpolatorInstanceCreate *itp, char *text, HBaseModel *model, int &counter);

		
};



//! The HBhvInterpolatorSegMove performs animation by switching on and off the visibility of segments.
class MVO_API HBhvInterpolatorSegMove : public HBhvInterpolator
{
public:

	/*!
	Constructs an HBhvInterpolatorSegMove object.
	\param animation The animation that this interpolator belongs to.
	\param name The name of the interpolator.
	*/
	HBhvInterpolatorSegMove(HBhvAnimation *animation = 0, const char *name = 0);
	~HBhvInterpolatorSegMove();

	/*! \return The type of the interpolator which is 'SegMove'. */
	const char * GetType();

	/*!
	In special cases, you may want an interpolator that references this interpolator without changing it. Use 
	this method to create the new interpolator passing a pointer to the animation that you want it to be associated with.
	\param ainst The animation instance that this interpolator is associated with.
	\return The newly created interpolator that references this interpolator.
	*/
	virtual HBhvInterpolator * CreateInstance(HBhvAnimation *ainst);

	/*!
	This method creates a new keyframe with the passed segment path <i>t</i> and then inserts it
	after the passed position <i>l</i> into the keyframe array.
	\param t Segment path to insert
	\param l array position to insert item after
	*/
	void  Insert(char * t, int l = 0) { HKeyframeString* SegMove = new HKeyframeString; 
				SegMove->SetTarget(t); m_pArray.InsertAt(SegMove, l); }

	/*!
	Replaces the item at the given position in the keyframe array with a new keyframe that
	was created with the passed segment path <i>t</i>.
	\param spath Segment path to replace with
	\param l array position to replace
	*/
	void  Replace(char *spath, int l) { HKeyframeString * c = new HKeyframeString; 
				c->SetTarget(spath); REPLACE_VARRAY(c, l) }

	/*!
	Writes XML data to a buffer using the passed XML generator.
	*/
	virtual void Serialize(HUtilityXMLGenerator *xmlgen);

	/*!
	This is XML read callback which parses XML data and then populates the member variables with the associated values.
	*/
	static void *XMLCallback(HUtilityXMLTag *xt, bool open, void *m_pExtraData);

	/*!
	Performs an interpolation on the animation target.
	\param keyframe The reference keyframe for this interpolation.
	\param fraction The fraction value from the specified keyframe to the next keyframe.  The value is from 0 to 1/
	*/
	virtual void Interpolate(int keyframe, float fraction);

	/*!
	Resets the interpolator instance by turning off the visibility of all segments in the keyframe list.
	*/	
	virtual void Reset();
		
};




//! The HBhvInterpolatorMatrix class controls quaternion based rotation. This class can be used for camera interpolation as well other targets.
class MVO_API HBhvInterpolatorMatrix : public HBhvInterpolator
{
public:

	/*!
	Constructs an HBhvInterpolatorMatrix object.
	\param animation Animation that this interpolator belongs to
	\param name Name of Interpolator
	*/
	HBhvInterpolatorMatrix(HBhvAnimation *animation = 0, const char *name = 0);

	/*! \return The type of the interpolator which is 'QuatRot'. */
	const char * GetType();

	/*!
	In special cases, you may want an interpolator that references this interpolator without changing it. Use 
	this method to create the new interpolator passing a pointer to the animation that you want it to be associated with.
	\param ainst The animation instance that this interpolator is associated with.
	\return The newly created interpolator that references this interpolator.
	*/
	virtual HBhvInterpolator * CreateInstance(HBhvAnimation *ainst);

	/*!
	This method creates a new quaternion rotation keyframe with the passed <i>q</i> value and then inserts it
	after the passed position <i>l</i> into the keyframe array.
	\param q The quaternion to use for new keyframe.
	\param l The array position to insert item after.
	*/
	void  Insert(float *mat, int l = 0) { HKeyframeMatrix * rot = new HKeyframeMatrix; 
				for (int i=0;i<16;i++) rot->m_matrix[i] = mat[i]; m_pArray.InsertAt(rot, l); }
	
 
 
	/*!
	Replaces the item at the given position in the keyframe array with a new quaternion rotation keyframe that
	was created with the passed <i>q</i> value.
	\param q The quaternion to use for the replace keyframe
	\param l The array position to replace.
	*/
	void  Replace(float * q, int l) { HKeyframeMatrix * c = new HKeyframeMatrix; 
					for (int i=0;i<16;i++) c->m_matrix[i] = q[i]; REPLACE_VARRAY(c, l); }


	/*!
	Writes XML data to a buffer using the passed XML generator.
	*/
	virtual void Serialize(HUtilityXMLGenerator *xmlgen);

	/*!
	This is XML read callback which parses XML data and then populates the member variables with the associated values.
	*/
	static void *XMLCallback(HUtilityXMLTag *xt, bool open, void *m_pExtraData);

	/*!
	Performs an interpolation on the animation target.
	\param keyframe The reference keyframe for this interpolation.
	\param fraction The fraction value from the specified keyframe to the next keyframe.  The value is from 0 to 1.
	*/
	virtual void Interpolate(int keyframe, float fraction);

 
protected:

};

 

#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif



