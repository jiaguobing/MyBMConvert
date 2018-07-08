// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef _HUtilityAntialiasing_H
#define _HUtilityAntialiasing_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif


#include "HTools.h"
#include "hic_types.h"
 
 
class HBaseView;

//! The HUtilityAntialiasing class contains a set of function for software based image antialiasing
 /*!
	This class provides support for rendering an antialiased scene.  This is achieved by rendering the scene 
	to an image that is large then the extents of the view on the screen.
*/
class MVO_API HUtilityAntialiasing
{
public:

	/*! constructor */
	HUtilityAntialiasing(HBaseView *view);
	~HUtilityAntialiasing();


	/*!  
		Returns Whether antialiased picture is currently displayed
	*/
  	bool GetShowState();

	/*!  
		Creates Antialiased Image in memory buffer
		\return A boolean indicating success or failure
	*/
	bool CreateImage();

	/*!  
		Create and Display antialiased image
		\param factor oversampling factor 
	*/
	void Show(int factor = 4);

	/*!  
		Hide antialiased picture
	*/
	void Hide();

	/*!  
		Register callbacks for class
 	*/

	/*! IM helper function */
	static void RegisterCallbacks();
	/*! IM helper function */	
	static void skip_segment_tree (HIC_Rendition *rendition, HIC_Segment_Info *seg);
 	/*! IM helper function */	
	static void draw_thick_polylines (HIC_Rendition *rendition, int count, HIC_DC_Point poi3[]);
	/*! IM helper function */	
	static void draw_thick_polyedges (HIC_Rendition *rendition, int count, HIC_DC_Point points[]);


 
protected:

	/*!  
		Helper function to retreive current window size
		\param xmax Width of window.  Passed by reference
		\param ymax Width of window.  Passed by reference
	*/ 
	void GetWindowSize(int &xmax, int &ymax);

private:
 	HBaseView	*m_pHView;
	HPixelRGB			*m_pAAImageData; 
	HC_KEY 		m_AntialisedImageSegmentKey;
	int			m_width;
	int			m_height;
 	bool		m_bIsShown;
	int			m_factor;

	char		m_drawsegmenttree_callback_name[MVO_BUFFER_SIZE];
	char		m_drawwindow_callback_name[MVO_BUFFER_SIZE];
	bool		m_bFirst;
	char				largeImageSegment[MVO_BUFFER_SIZE];
	char				imagedriver[MVO_BUFFER_SIZE];

	bool m_bFirstFirst;   	
	HC_KEY					image_key; 

};


#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif





