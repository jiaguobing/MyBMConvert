// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// HUtilityShadow.h : interface of the HUtilityShadow classes
// Set of utility methods for shadow creations
 
/*! \file HUtilityShadow.h
	Defines datatypes for the HShadow classes
*/

#ifndef _HUTILITYSHADOW_H
#define _HUTILITYSHADOW_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"
 

class HBaseView;
class HPixelRGB;
class HPixelRGBA;


/*! \enum ShadowPlaneType
	Defines the plane that the shadow will fall on
*/
enum ShadowPlaneType
{
	XZ_PLANE,   //!< self-explanatory
	XY_PLANE,   //!< self-explanatory
	YZ_PLANE    //!< self-explanatory
};
 
 
//! <b>This class has been deprecated.</b> 
/*!  
<b>This class has been deprecated.</b> The HShadow class aids in creating soft and hard shadows of a given scene

Here is an example of how to use it:
										
	HC_KEY shadowkey;
	HC_Open_Segment_By_Key(m_pView->GetSceneKey()); //we first need to create a separate segment for the shadow geometry...
		shadowkey = HC_KCreate_Segment("shadows");
	HC_Close_Segment();
	HSmoothShadow myShadow(m_pView->GetModel()->GetModelKey(), shadowkey);
  	myShadow.Create(XY_PLANE);

Smooth shadows are created by projecting the geometry to the desired plane (from a given light direction)
and converting the result to a texture map on which a softening filter is applied
 	myshadow = new HSmoothShadow(m_pView->GetModel()->GetModelKey(), shadowkey);

Apart from the model key (geometry that should be shadowed) and the key for the shadow segment
we can also specify the size of the texture map (can effectively only be set to 64, 128 or 256) to avoid distortion
as well as a smoothing filter that is applied at each pixel of the shadow texture (see below for the default filter matrix)
Those values can either be set in the Create call or the default parameters can be used with the only requirement being
to identify a ground plane (Most HSF files have an XY ground plane) as follows:

  	myshadow.Create(XY_PLANE);


This function also calculates the world space extents of the model to automatically find a suitable
light/plane position. If you want to supply your own arbitrary light/plane position simply call the overloaded create function
without any parameter and use SetShadowLightPos and SetShadowGroundPlane respectively.
After this call has completed the shadow should be visible. If the scene changes a new call to
Create() is required in the case of the smooth shadow to recreate the texture.
To adjust the brightness of the shadow the function SetShadowBrightness() can be used (from 0 to 255).

Hard Shadows are creating with the same process (of course no texture/filter parameters). The difference is that no further 
calls to Create() are necessary unless a change of the light/plane position is desired.

Here are a few considerations when using that functionality:
As soft shadows use a texture map of limited size the complexity/size of the scene can effect the quality/detail of the shadow.
It might make sense to create a separate shadow for every object in the scene. However there is currently a blending problem
when multiple shadows are overlapping. This should be fixed in the next release.
The "smoothness" of the shadow is greatly affected by the size of the texturemap and the extends of the smoothness filter. 
On the other hand a larger texture/filter also increases the texture creation time.
In the current implementation the shadow texture is scaled so that it goes a bit beyond the model extents. This might be not the perfect solution
for certain models though...

Hard shadows are created by projecting the given scene with a "shadow matrix". This in effect doubles the complexity of the scene
so it might make sense to use an LOD technique on the model. This could also be selectively applied on the shadows segment only (not tested yet).

*/
class MVO_API HShadow
{
public:

	/*!  Constructor
		\param ModelSegmentKey Key of top-level segment that shadows should be applied to 
		\param ShadowBaseSegmentKey Key of segment that contains the shadow geometry
		\param ObjectModellingMatrix NetModellingMatrix of the shadow segment; zero by default
	*/
	HShadow(HC_KEY ModelSegmentKey, HC_KEY ShadowBaseSegmentKey, float ObjectModellingMatrix[] = 0);

	/*!  
		Calculates the total extents of the model in world space
	*/
	void CalculateTotalModelExtents();

	/*! Returns the extents of the model */
 	void GetModelExtents(HPoint &ObjectBoundingMin, HPoint &ObjectBoundingMax)
	{
		ObjectBoundingMin = m_ObjectBoundingMin;
		ObjectBoundingMax = m_ObjectBoundingMax;
	}

	/*! Returns the light position */
	void GetShadowLightPos(HPoint &lightpos) { lightpos = m_ShadowLightPos; }
	/*! Returns the shadow ground plane */
	void GetShadowGroundPlane(HPlane &plane) { plane = m_ShadowGroundPlane; }
	/*! Returns the key of the shadow segment */
	HC_KEY GetShadowKey() { return m_ShadowSegmentKey; }

	/*! Set the light position */
	void SetShadowLightPos(HPoint lightpos) { m_ShadowLightPos = lightpos; }
	/*! Set the shadow ground plane */
	void SetShadowGroundPlane(HPlane plane) { m_ShadowGroundPlane = plane; }

	/*! Set the shadow brightness*/
 	void SetShadowBrightness(int ShadowBrightness) { m_ShadowBrightness = ShadowBrightness; }
	/*! Returns the shadow brightness*/
	int GetShadowBrightness() { return m_ShadowBrightness; }
  
protected:
	bool  m_bFirstCoordinate;				/*!< Internal Use*/

	HC_KEY m_ModelSegmentKey;				/*!< Internal Use*/
	HC_KEY m_ShadowSegmentKey;				/*!< Internal Use*/
	int m_ShadowBrightness;					/*!< Internal Use*/
 	
	HPoint m_ShadowLightPos;				/*!< Internal Use*/
	HPlane m_ShadowGroundPlane;				/*!< Internal Use*/
	
	HPoint m_ObjectBoundingMin;				/*!< Internal Use*/
	HPoint m_ObjectBoundingMax;				/*!< Internal Use*/

	float m_ObjectModellingMatrix[16];		/*!< Internal Use*/

	 /*!  
		Returns combined modelling matrix
		\param segkey Key of the segment for which the modelling matrix needs to be retrieved 
		\param oldmatrix base matrix for multiplication.  Passed by reference.
		\param newmatrix combined matrix.  Passed by reference.  Returned to user.  
	*/
	void ShowModellingMatrix(HC_KEY segkey, float oldmatrix[], float newmatrix[]);

	/*!  
		Computes axis alligned bounding box of pointcloud
		\param min  minimum extents of bounding box
		\param max  maximum extents of bounding box
		\param points	point cloud data
		\param numpoints
	*/
	void ComputeObjectExtents(HPoint &min, HPoint &max, HPoint points[], int numpoints);

	/*!  
		Rotates Plane to a given Vector
		\param plane to rotate
		\param matrix rotation matrix
	*/
 	void RotateToGroundPlane(HPlane plane, float matrix[]);

	/*!  
		Computes shadow matrix based on shadow plane and light position
		\param matrix Modelling Matrix
		\param plane shadow plane
		\param d light position
	*/
 	void ShadowMatrix2(float matrix[], HPlane plane, HPoint d);

	/*!  
		Moves shell out of view for "fitworld" calculation 
	*/
	HC_KEY IsolateShell();

	/*!  
		Restores shadow shell position
	*/
	void RestoreShell();


	/*!  
	  \param segKey   Segment key to look in.
	  \return Whether any shells exist in a specified segment.
	*/
	bool HasShell(HC_KEY segKey);

private:
	void CalculateModelExtents(HC_KEY SegKey, float totalmatrix[]);
	void SetIdentityMatrix(float matrix[]);
	void ShadowMatrix(float mm[], HPlane plane, HPoint light);


};

//! <b>This class has been deprecated.</b> 
/*!  
	<b>This class has been deprecated.</b> 
*/
class  MVO_API HSmoothShadow : public HShadow
{

public:
	/*!  
		\param ModelSegmentKey Top-Level Segment that shadows should be applied to 
		\param ShadowBaseSegmentKey Segment that contains the shadow geometry
		\param UseOglShadow Enables Hardware Support for Shadow Generation	*/ 
	HSmoothShadow(HC_KEY ModelSegmentKey, HC_KEY ShadowBaseSegmentKey, bool UseOglShadow = false);
	virtual ~HSmoothShadow();
 	/*!  
	  Objetct Cleanup
	*/ 
	void CleanUp();
  
	/*!  Creates a smooth shadow at the given plane
		\param ptype  can be XZ_PLANE, XY_PLANE or YZ_PLANE. 
		\param TextureSize	width/height of the shadow texture. For full hardware support must be 64,128, or 256
		\param FilterSize	width/height of the filter used for texture smoothing (only uneven filter sizes make sense!)
		\param filter		Pointer to texture smoothing 
	*/ 
	void Create(ShadowPlaneType ptype, int TextureSize = 0, int FilterSize = -1, int filter[] = 0); 

	/*!  
	  Creates a smooth shadow with the current parameters
	*/ 
 	void Create();

 	/*! Returns the shadow filter dimension */
	int GetFilterSize() { return m_FilterSize; }

 	/*!  Returns the texture size of the shadow */
 	int GetTextureSize() { return m_TextureSize; }

 	/*! Sets the texture size of the shadow 
		\param TextureSize dimensions of texture in x/y
 	
	*/
 	void SetTextureSize(int TextureSize) { m_TextureSize = TextureSize; }

 	/*! Specifies the smoothing filter
		\param filter Pointer to smoothing filter
		\param FilterSize SIze of smoothing filter
 	
	*/
 	void SetFilter(int filter[], int FilterSize) { m_filter = filter; m_FilterSize = FilterSize; }

 	/*! Undefines the Shadow Texture
	*/
	void UnDefineShadowTexture();
 	/*! Undefines all Shadow Textures
	*/
	static void UnDefineAllShadowTextures(HC_KEY modelsegmentkey);

 	/*! Allow Opengl offscreen buffer for shadow generation
		\param AllowOpenglShadow	true = opengl offscreen buffer, false = image driver
  	
	*/
	void SetAllowOpenglShadow(bool AllowOpenglShadow) { m_bAllowOpenglShadow = AllowOpenglShadow; }
protected:
	int m_TextureSize;										//!< size of texture extents
	int m_FilterSize;										//!< size of filter matrix
	int *m_filter;											//!< filter matrix
	HC_KEY m_ImageKey;										//!< key to shadow image
 	HPoint m_ShadowBoundingMin;								//!< min value of shadow bounding volume 
	HPoint m_ShadowBoundingMax;								//!< max value of shadow bounding volume 
	char m_texname[MVO_SMALL_BUFFER_SIZE];									//!< name of shadow texture
	bool m_bAllowOpenglShadow;								//!< use opengl offscreen buffer
	/*!  
		Calculates the total shadow size
	*/ 
	void CalculateTotalShadowExtents();
  
	/*!  
		Recursively calculates the total shadow size 
		\param segkey  startsegment for the shadow calculation
		\param totalmatrix combined modelling matrix 
	*/ 
 	void CalculateShadowExtents(HC_KEY segkey,float *totalmatrix);

	/*!  
		Image driver setup for shadow texture creation
	*/ 
	void CreateSoftShadow();

	/*! 
		Creates shadow texture and associates texture to simple mesh
	*/ 
	void CreateShadowTexture();

	/*! Image Filter function
		\param origImage  input image
		\param smoothImage output image
		\param width image width
		\param height image heigh
		\param filterSize size of image filter
		\param filter filter matrix
	*/ 
	void SmoothImage(HPixelRGB *origImage, HPixelRGBA **smoothImage, int width, int height, int filterSize, int filter[]);

private:
	void CalculateOneShadowExtent(HC_KEY shellkey, float *modellingMatrix);

};


//! <b>This class has been deprecated.</b> 
class  MVO_API HHardShadow : public HShadow
{

public:
	/*! custom constructor */
	HHardShadow(HC_KEY ModelSegmentKey, HC_KEY ShadowBaseSegmentKey) : HShadow(ModelSegmentKey, ShadowBaseSegmentKey)  {}

	virtual ~HHardShadow() {};

	
	/*!  
		Creates a hard shadow at the given plane
		\param ptype  can be XZ_PLANE XY_PLANE or YZ_PLANE. 
	*/ 
	void Create(ShadowPlaneType ptype); 

	/*!  
		Creates a hard shadow with the current parameters
	*/ 
 	virtual void Create();

protected:

	/*!  
	   Setup for shadow texture creation
	*/ 
	void CreateHardShadow();
  
};



#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif



