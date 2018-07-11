// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef _H_SNAPGRID__H__INCLUDED
#define _H_SNAPGRID__H__INCLUDED

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"

class HBaseView;

//! The HSnapGrid class is used to display the grid and implements snapping to the grid.
/*! 
  HSnapGrid implements the grid display and snapping to grid functionality.
  
  The class encapsulates a HOOPS snap-grid segment. The grid is a plane in 3D. 
  Once the 'snap' is set, all the mouse events will returned the positions constrained 
  to the grid plane and snapped at the set interval.  Be sure to call #Create before using the 
  other methods in this class.  If you do not, the other methods may fail.
*/
class MVO_API HSnapGrid
{

public:

	/*!Construct an HSnapGrid object.
	\param pHView A pointer the HBaseView object you want to associate with this object.
	*/
    HSnapGrid(HBaseView* pHView);
    virtual ~HSnapGrid();

    // create one - should be the first function you call
    // return true if succeeds
    /*!
      This method creates a grid. The other functions of this class may fail if a grid was not created.
      A quadrilateral unit is formed as (origin, ref1, ref1 + ref2 - origin, ref2).  The system will make 
	  the appropriate optimizations when the grid is seen in perspective. 
      \param origin An x-y-z triplet for the coordinates of the "start point" of      
                         the grid. (A simple array of three elements may also be           
                         used in most languages.) Passed by reference always.     
      \param ref1   An x-y-z triplet for the coordinates of the first point "to the   
                         right of" origin. See below for details. Passed by reference      
                         always.
      \param ref2   An x-y-z triplet for the coordinates of the first point           
                         "above" origin. See below for details. Passed by reference        
                         always
      \param count1 The number of grid units along the ref1 dimension.  If count1 is positive, the number of units is laid 
	  down along the ref1 axis starting at the origin.  If it is negative, the number of units laid down along the ref1 axis
	  and an additional count1 number of units is laid down on the negative side of the ref1 axis. If count1 is zero, the grid 
	  extends infinitely along the ref1 axis.
      \param count2 The number of grid units along the ref2 dimension. If count2 is positive, the number of units is laid 
	  down along the ref2 axis starting at the origin.  If it is negative, the number of units laid down along the ref2 axis
	  and an additional count2 number of units is laid down on the negative side of the ref1 axis. If count2 is zero, the grid 
	  extends infinitely along the ref2 axis.
      \return True if function succeeded.
    */
    bool Create( const HPoint& origin, const HPoint& ref1, const HPoint& ref2, int count1 = 0, int count2 = 0);

    /*!
      This method displays the grid so that it is always on top. Note that this will just change 
      how the grid is displayed. The grid plane for all computation purposes will remain the same 
      as specified during creation.
      \param true_false Pass true to display grid on top or false to display it at its actual location.
    */
    void SetOnTop( bool true_false );

    /*!
      \return True if grid is set to be displayed on top or false if it is displayed at its actual location.
    */
    bool GetOnTop();

    /*!
     This method snaps a given point to the grid.
      \param in_out  The input point in viewpoint space. The snapped point will be returned in same variable and 
	  will be in viewpoint space.
      \return True if the operation succeeded.
    */
    bool GetSnappedViewpointPos( HPoint& in_out );

    /*!
     This method snaps a given point to the grid.
      \param in_out  The input point in world space. The snapped point will be returned in same variable and will be 
	  in world space.
      \return True if the operation succeeded.
    */
    bool GetSnappedWorldPos( HPoint& in_out );


    /*!
      \return The grid segment key or INVALID_KEY if the grid was not created.
    */
    HC_KEY GetGridSegment(){ return m_GridSegmentKey; };

    /*!
      \return  The grid geometry key or INVALID_KEY if the grid was not created.
    */
    HC_KEY GetGridGeometry(){ return m_GridGeomKey; };

    //--- There are more possible interfaces that could be added ---///
    //bool SetOrientation( const HPlane& grid_plane, const HPoint& ref_point );
    //void SetPlane( const float a, const float b, const float c, const float d );
    //void SetPlane( const HPlane& grid_plane );
    //void GetPlane( float& a, float& b, float& c, float& d);
    //void GetPlane( HPlane& grid_plane );

    // indicates if the grid is locked with respect to the camera position changes.
    //void SetPositionLock( bool on_off );
    //bool GetPositionLock();

    // switches grid display on/off.
    //void SetDisplay( bool on_off );
    //bool GetDisplay();

    // sets grid display color
    //void SetColor(const HPixelRGBA &rgba);
    //void GetColor(HPixelRGBA &rgba);

    // minimum distances in x & y for which grid (lines/dots) will be displayed.
    //void SetInterval( const float& x, const float& y);
    //void GetInterval( float& x, float& y);


    // switches snap to grid on/off. If turned all, 
    // all the mouse events will be constrained to the grid plane and also snapped to the intervals
    //void SetSnap( bool on_off );
    //bool GetSnap();

    // snap and display intervals for the grid. The diplay interval takes precedance if it is coarser.
    //void SetSnapInterval( const float& x, const float& y);
    //void GetSnapInterval( float& x, float& y);


protected:

// functions
	
    /*!
      This method inserts the grid geometry.  It is a helper function to #Create.
      \param origin An x-y-z triplet for the coordinates of the "start point" of      
                         the grid. (A simple array of three elements may also be           
                         used in most languages.) Passed by reference always.     
      \param ref1   An x-y-z triplet for the coordinates of the first point "to the   
                         right of" origin. See below for details. Passed by reference      
                         always.
      \param ref2   An x-y-z triplet for the coordinates of the first point           
                         "above" origin. See below for details. Passed by reference        
                         always
      \param count1 The number of grid units along the ref1 dimension.  If count1 is positive, the number of units is laid 
	  down along the ref1 axis starting at the origin.  If it is negative, the number of units laid down along the ref1 axis
	  and an additional count1 number of units is laid down on the negative side of the ref1 axis. If count1 is zero, the grid 
	  extends infinitely along the ref1 axis.
      \param count2 The number of grid units along the ref2 dimension. If count2 is positive, the number of units is laid 
	  down along the ref2 axis starting at the origin.  If it is negative, the number of units laid down along the ref2 axis
	  and an additional count2 number of units is laid down on the negative side of the ref1 axis. If count2 is zero, the grid 
	  extends infinitely along the ref2 axis.
      \return True if function succeeded.
    */
    bool insert_grid_geometry( const HPoint& origin, const HPoint& ref1, const HPoint& ref2, int count1, int count2);
    /*!
    This method is an internal function used to set the defualt attributes on grid segment.
    \return 	True if the operation succeeded.
    */
    bool set_default_grid_attributes();

    /*!
      This method returns the grid parameters.
      \param retGridPlane  Returns a plane describing the grid.
      \param retOrigin Returns the center location of the grid.
      \param retRef1 Returns the first reference axis.
      \param retRef2 Retruns the second reference axis.
    \return 	True if the operation succeeded.
    */
    bool get_grid_parameters( HPlane& retGridPlane, HPoint& retOrigin, HPoint& retRef1, HPoint& retRef2 );
	
	/*!
	  Given a point and grid geometry, this internal function returns the snapped point.
	  \param in_out		The point to be snapped. The snapped point is returned in this variable.
	  \param origin		The location of the grid's origin.
	  \param ref1		The first grid reference axis.
	  \param ref2		The second grid reference axis.
    \return 	True if the operation succeeded.
	*/
	bool snap_to_grid( HPoint& in_out, const HPoint& origin, const HPoint& ref1, const HPoint& ref2 );

// members

    /*! A pointer to the HBaseView object. */
    HBaseView * m_pHView;

    /*! The segment where the grid is inserted. */
    HC_KEY m_GridSegmentKey;

    /*! The grid geometry key. */
    HC_KEY m_GridGeomKey;

};


#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif



