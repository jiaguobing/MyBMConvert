// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef _HIC_WRAP_H
#define _HIC_WRAP_H

#include "hic.h"


class Draw_DC_Line_Impl;


/*!
The Draw_DC_Line class is a base class used to define and set 
a callback at the "draw dc line" callback point during the HOOPS update 
cycle.  To use this class, derive your own child class from it and 
implement the method #draw_dc_line. For more information on 
how to define and set callbacks as a C++/C/Java Developer, please see 
the <a href="../prog_guide/08_1_im_overview.html#8142_CallbacksForC++Developers">HOOPS/3dGS Programming Guide</a>.
*/
#ifdef SWIG
%typemap (imtype,directorinattributes="[MarshalAs(UnmanagedType.LPArray,SizeConst=2)]") HIC_DC_Point const *points "HIC_DC_Point[]"
#endif
class HIC_API Draw_DC_Line
{
public:
	/*!
        Constructs a Draw_DC_Line Callback object.
          \param key Key to the segment on which the callback is set.
          \param name The name of the callback.
          \param data Optional pointer to user data that can then be accessed from the
			callback.
    */ 

	Draw_DC_Line(HC_KEY key, const char * name, void * data=0);
	
	/*! Deletes the callback object.  */       
	
	virtual ~Draw_DC_Line();

	 /*!
        The function you implement for your callback.
        \param nr A pointer to a const HIC_Rendition
        \param points A pointer to a const HIC_DC_Point

	*/
	virtual void draw_dc_line(HIC_Rendition const *nr, HIC_DC_Point const *points)
	{
		HIC_Draw_DC_Line(nr, points);
	}

	/*!  Returns the name of the callback.  */       
	
	const char * GetName();

protected:   
	
	/*!   Returns the pointer to the user data that may have been provided when the callback was created.  */

	void * GetData();	
	
private:

	Draw_DC_Line_Impl * _impl;
};
#ifdef SWIG
%clear HIC_DC_Point const *points;
#endif



class Draw_3D_Geometry_Impl;

/*!
The Draw_3D_Geometry class is a base class used to define and set 
a callback at the "draw 3d geometry" callback point during the HOOPS update 
cycle.  To use this class, derive your own child class from it and 
implement the method #draw_3d_geometry. For more information on 
how to define and set callbacks as a C++/C/Java Developer, please see 
the <a href="../prog_guide/08_1_im_overview.html#8142_CallbacksForC++Developers">HOOPS/3dGS Programming Guide</a>.
*/

class HIC_API Draw_3D_Geometry
{
public:
	/*!
        Constructs a Draw_3D_Geometry Callback object.
          \param key Key to the segment on which the callback is set.
          \param name The name of the callback.
          \param data Optional pointer to user data that can then be accessed from the
			callback.
    */ 

	Draw_3D_Geometry(HC_KEY key, const char * name, void * data=0);
	
	/*! Deletes the callback object.  */       
	
	virtual ~Draw_3D_Geometry();

	 /*!
        The function you implement for your callback.
        \param nr A pointer to a const HIC_Rendition
        \param geo A pointer to a const HIC_Geometry
	*/

	virtual void draw_3d_geometry(HIC_Rendition const *nr,HIC_Geometry const *	geo)
	{
		HIC_Draw_3D_Geometry(nr, geo);
	}

	/*!  Returns the name of the callback.  */       
	
	const char * GetName();

protected:  
	
	/*!   Returns the pointer to the user data that may have been provided when the callback was created.  */

	void * GetData();	
	
private:

	Draw_3D_Geometry_Impl * _impl;
};



class Draw_3D_Text_Impl;

/*!
The Draw_3D_Text class is a base class used to define and set 
a callback at the "draw 3d text" callback point during the HOOPS update 
cycle.  To use this class, derive your own child class from it and 
implement the method #draw_3d_text. For more information on 
how to define and set callbacks as a C++/C/Java Developer, please see 
the <a href="../prog_guide/08_1_im_overview.html#8142_CallbacksForC++Developers">HOOPS/3dGS Programming Guide</a>.
*/

class HIC_API Draw_3D_Text
{
public:

	/*!
        Constructs a Draw_3D_Text Callback object.
          \param key Key to the segment on which the callback is set.
          \param name The name of the callback.
          \param data Optional pointer to user data that can then be accessed from the
			callback.
    */ 

	Draw_3D_Text(HC_KEY key, const char * name, void * data=0);
	
	/*! Deletes the callback object.  */       
	
	virtual ~Draw_3D_Text();

	/*!
        The function you implement for your callback.
        \param nr A pointer to a const HIC_Rendition
        \param text A pointer to a const HIC_Text
	*/

	virtual void draw_3d_text(HIC_Rendition const *nr, HIC_Text const *text)
	{
		HIC_Draw_3D_Text(nr, text);
	}

	/*!  Returns the name of the callback.  */       
	
	const char * GetName();

protected:   
	
	/*!   Returns the pointer to the user data that may have been provided when the callback was created.  */

	void * GetData();	
	
private:

	Draw_3D_Text_Impl * _impl;
};


class Draw_3D_Curve_Impl;

/*!
The Draw_3D_Curve class is a base class used to define and set 
a callback at the "draw 3d curve" callback point during the HOOPS update 
cycle.  To use this class, derive your own child class from it and 
implement the method #draw_3d_curve. For more information on 
how to define and set callbacks as a C++/C/Java Developer, please see 
the <a href="../prog_guide/08_1_im_overview.html#8142_CallbacksForC++Developers">HOOPS/3dGS Programming Guide</a>.
*/

class HIC_API Draw_3D_Curve
{
public:

	/*!
        Constructs a Draw_3D_Curve Callback object.
          \param key Key to the segment on which the callback is set.
          \param name The name of the callback.
          \param data Optional pointer to user data that can then be accessed from the
			callback.
    */ 


	Draw_3D_Curve(HC_KEY key, const char * name, void * data=0);
	
	/*! Deletes the callback object.  */       
	
	virtual ~Draw_3D_Curve();

	/*!
        The function you implement for your callback.
        \param nr A pointer to a const HIC_Rendition
        \param curve A pointer to a const HIC_NURBS_Curve
	*/

	virtual void draw_3d_curve(HIC_Rendition const *nr, HIC_NURBS_Curve const *curve)
	{
		HIC_Draw_3D_NURBS_Curve(nr, curve);
	}

	/*!  Returns the name of the callback.  */       
	
	const char * GetName();

protected:   
	
	/*!   Returns the pointer to the user data that may have been provided when the callback was created.  */

	void * GetData();	
	
private:

	Draw_3D_Curve_Impl * _impl;
};


class Draw_3D_Cylinder_Impl;

/*!
The Draw_3D_Cylinder class is a base class used to define and set 
a callback at the "draw 3d cylinder" callback point during the HOOPS update 
cycle.  To use this class, derive your own child class from it and 
implement the method #draw_3d_cylinder. For more information on 
how to define and set callbacks as a C++/C/Java Developer, please see 
the <a href="../prog_guide/08_1_im_overview.html#8142_CallbacksForC++Developers">HOOPS/3dGS Programming Guide</a>.
*/

class HIC_API Draw_3D_Cylinder
{
public:

	/*!
        Constructs a Draw_3D_Cylinder Callback object.
          \param key Key to the segment on which the callback is set.
          \param name The name of the callback.
          \param data Optional pointer to user data that can then be accessed from the
			callback.
    */ 


	Draw_3D_Cylinder(HC_KEY key, const char * name, void * data=0);
	
	/*! Deletes the callback object.  */       
	
	virtual ~Draw_3D_Cylinder();

	/*!
        The function you implement for your callback.
        \param nr A pointer to a const HIC_Rendition
        \param cylinder A pointer to a const HIC_Cylinder
	*/

	virtual void draw_3d_cylinder(HIC_Rendition const *nr, HIC_Cylinder const *cylinder)
	{
		HIC_Draw_3D_Cylinder(nr, cylinder);
	}

	/*!  Returns the name of the callback.  */       
	
	const char * GetName();

protected:   
	
	/*!   Returns the pointer to the user data that may have been provided when the callback was created.  */

	void * GetData();	
	
private:

	Draw_3D_Cylinder_Impl * _impl;
};


class Draw_3D_Ellipse_Impl;

/*!
The Draw_3D_Ellipse class is a base class used to define and set 
a callback at the "draw 3d ellipse" callback point during the HOOPS update 
cycle.  To use this class, derive your own child class from it and 
implement the method #draw_3d_ellipse. For more information on 
how to define and set callbacks as a C++/C/Java Developer, please see 
the <a href="../prog_guide/08_1_im_overview.html#8142_CallbacksForC++Developers">HOOPS/3dGS Programming Guide</a>.
*/

class HIC_API Draw_3D_Ellipse
{
public:

	/*!
        Constructs a Draw_3D_Ellipse Callback object.
          \param key Key to the segment on which the callback is set.
          \param name The name of the callback.
          \param data Optional pointer to user data that can then be accessed from the
			callback.
    */ 


	Draw_3D_Ellipse(HC_KEY key, const char * name, void * data=0);
	
	/*! Deletes the callback object.  */       
	
	virtual ~Draw_3D_Ellipse();

	/*!
        The function you implement for your callback.
        \param nr A pointer to a const HIC_Rendition
        \param ellipse A pointer to a const HIC_Ellipse
	*/

	virtual void draw_3d_ellipse(HIC_Rendition const *nr, HIC_Ellipse const *ellipse)
	{
		HIC_Draw_3D_Ellipse(nr, ellipse);
	}

	/*!  Returns the name of the callback.  */       
	
	const char * GetName();

protected:   
	
	/*!   Returns the pointer to the user data that may have been provided when the callback was created.  */

	void * GetData();	
	
private:

	Draw_3D_Ellipse_Impl * _impl;
};

/* Testing Elliptical Arc */


class Draw_3D_Elliptical_Arc_Impl;

/*!
The Draw_3D_Elliptical_Arc class is a base class used to define and set 
a callback at the "draw 3d elliptical arc" callback point during the HOOPS update 
cycle.  To use this class, derive your own child class from it and 
implement the method #draw_3d_elliptical_arc. For more information on 
how to define and set callbacks as a C++/C/Java Developer, please see 
the <a href="../prog_guide/08_1_im_overview.html#8142_CallbacksForC++Developers">HOOPS/3dGS Programming Guide</a>.
*/

class HIC_API Draw_3D_Elliptical_Arc
{
public:

	/*!
        Constructs a Draw_3D_Elliptical_Arc Callback object.
          \param key Key to the segment on which the callback is set.
          \param name The name of the callback.
          \param data Optional pointer to user data that can then be accessed from the
			callback.
    */ 


	Draw_3D_Elliptical_Arc(HC_KEY key, const char * name, void * data=0);
	
	/*! Deletes the callback object.  */       
	
	virtual ~Draw_3D_Elliptical_Arc();

	/*!
        The function you implement for your callback.
        \param nr A pointer to a const HIC_Rendition
        \param arc A pointer to a const HIC_Elliptical_Arc
	*/

	virtual void draw_3d_elliptical_arc(HIC_Rendition const *nr, HIC_Elliptical_Arc const *arc)
	{
		HIC_Draw_3D_Elliptical_Arc(nr, arc);
	}

	/*!  Returns the name of the callback.  */       
	
	const char * GetName();

protected:   
	
	/*!   Returns the pointer to the user data that may have been provided when the callback was created.  */

	void * GetData();	
	
private:

	Draw_3D_Elliptical_Arc_Impl * _impl;
};


/* Testing 3D_Grid */


class Draw_3D_Grid_Impl;

/*!
The Draw_3D_Grid class is a base class used to define and set 
a callback at the "draw 3d grid" callback point during the HOOPS update 
cycle.  To use this class, derive your own child class from it and 
implement the method #draw_3d_grid. For more information on 
how to define and set callbacks as a C++/C/Java Developer, please see 
the <a href="../prog_guide/08_1_im_overview.html#8142_CallbacksForC++Developers">HOOPS/3dGS Programming Guide</a>.
*/


class HIC_API Draw_3D_Grid
{
public:

	    /*!
        Constructs a Draw_3D_Grid Callback object.
          \param key Key to the segment on which the callback is set.
          \param name The name of the callback.
          \param data Optional pointer to user data that can then be accessed from the
		callback.
			*/ 
	Draw_3D_Grid(HC_KEY key, const char * name, void * data=0);
	
	/*! Deletes the callback object.  */       
	
	virtual ~Draw_3D_Grid();

	/*!
        The function you implement for your callback.
        \param nr A pointer to a const HIC_Rendition
        \param grid A pointer to a const HIC_Geometry
	*/

	virtual void draw_3d_grid(HIC_Rendition const *nr, HIC_Grid const *grid)
	{
		HIC_Draw_3D_Grid(nr, grid);
	}


    
	/*!  Returns the name of the callback.  */       
	
	const char * GetName();

protected:   
	
	/*!   Returns the pointer to the user data that may have been provided when the callback was created.  */

	void * GetData();	
	
private:

	Draw_3D_Grid_Impl * _impl;
};

/* Testing 3D_Image */


class Draw_3D_Image_Impl;

/*!
The Draw_3D_Image class is a base class used to define and set 
a callback at the "draw 3d image" callback point during the HOOPS update 
cycle.  To use this class, derive your own child class from it and 
implement the method #draw_3d_image. For more information on 
how to define and set callbacks as a C++/C/Java Developer, please see 
the <a href="../prog_guide/08_1_im_overview.html#8142_CallbacksForC++Developers">HOOPS/3dGS Programming Guide</a>.
*/

class HIC_API Draw_3D_Image
{
public:

	/*!
        Constructs a Draw_3D_Image Callback object.
          \param key Key to the segment on which the callback is set.
          \param name The name of the callback.
          \param data Optional pointer to user data that can then be accessed from the
			callback.
    */ 


	Draw_3D_Image(HC_KEY key, const char * name, void * data=0);
	
	/*! Deletes the callback object.  */       
	
	virtual ~Draw_3D_Image();

	/*!
        The function you implement for your callback.
        \param nr A pointer to a const HIC_Rendition
        \param image A pointer to a const HIC_Image
	*/

	virtual void draw_3d_image(HIC_Rendition const *nr, HIC_Image const *image)
	{
		HIC_Draw_3D_Image(nr, image);
	}

	/*!  Returns the name of the callback.  */       
	
	const char * GetName();

protected:   
	
	/*!   Returns the pointer to the user data that may have been provided when the callback was created.  */

	void * GetData();	
	
private:

	Draw_3D_Image_Impl * _impl;
};


/* Testing 3D_Marker */


class Draw_3D_Marker_Impl;

/*!
The Draw_3D_Marker class is a base class used to define and set 
a callback at the "draw 3d marker" callback point during the HOOPS update 
cycle.  To use this class, derive your own child class from it and 
implement the method #draw_3d_marker. For more information on 
how to define and set callbacks as a C++/C/Java Developer, please see 
the <a href="../prog_guide/08_1_im_overview.html#8142_CallbacksForC++Developers">HOOPS/3dGS Programming Guide</a>.
*/

class HIC_API Draw_3D_Marker
{
public:

	/*!
        Constructs a Draw_3D_Marker Callback object.
          \param key Key to the segment on which the callback is set.
          \param name The name of the callback.
          \param data Optional pointer to user data that can then be accessed from the
			callback.
    */ 


	Draw_3D_Marker(HC_KEY key, const char * name, void * data=0);
	
	/*! Deletes the callback object.  */       
	
	virtual ~Draw_3D_Marker();

	/*!
        The function you implement for your callback.
        \param nr A pointer to a const HIC_Rendition
        \param marker A pointer to a const HIC_Marker
	*/

	virtual void draw_3d_marker(HIC_Rendition const *nr, HIC_Marker const *marker)
	{
		HIC_Draw_3D_Marker(nr, marker);
	}

	/*!  Returns the name of the callback.  */       
	
	const char * GetName();

protected:   
	
	/*!   Returns the pointer to the user data that may have been provided when the callback was created.  */

	void * GetData();	
	
private:

	Draw_3D_Marker_Impl * _impl;
};



/* Testing 3D_Mesh */


class Draw_3D_Mesh_Impl;

/*!
The Draw_3D_Mesh class is a base class used to define and set 
a callback at the "draw 3d mesh" callback point during the HOOPS update 
cycle.  To use this class, derive your own child class from it and 
implement the method #draw_3d_mesh. For more information on 
how to define and set callbacks as a C++/C/Java Developer, please see 
the <a href="../prog_guide/08_1_im_overview.html#8142_CallbacksForC++Developers">HOOPS/3dGS Programming Guide</a>.
*/

class HIC_API Draw_3D_Mesh
{
public:

	/*!
        Constructs a Draw_3D_Mesh Callback object.
          \param key Key to the segment on which the callback is set.
          \param name The name of the callback.
          \param data Optional pointer to user data that can then be accessed from the
			callback.
    */ 

	Draw_3D_Mesh(HC_KEY key, const char * name, void * data=0);
	
	/*! Deletes the callback object.  */       
	
	virtual ~Draw_3D_Mesh();

	/*!
        The function you implement for your callback.
        \param nr A pointer to a const HIC_Rendition
        \param phon A pointer to a const HIC_Polyhedron
	*/

	virtual void draw_3d_mesh(HIC_Rendition const *nr, HIC_Polyhedron const *phon)
	{
		HIC_Draw_3D_Mesh(nr, phon);
	}

	/*!  Returns the name of the callback.  */       
	
	const char * GetName();

protected:   
	
	/*!   Returns the pointer to the user data that may have been provided when the callback was created.  */

	void * GetData();	
	
private:

	Draw_3D_Mesh_Impl * _impl;
};


/* Testing 3D_Nurbs_Curve */


class Draw_3D_Nurbs_Curve_Impl;

/*!
The Draw_3D_Nurbs_Curve class is a base class used to define and set 
a callback at the "draw 3d nurbs curve" callback point during the HOOPS update 
cycle.  To use this class, derive your own child class from it and 
implement the method #draw_3d_nurbs_curve. For more information on 
how to define and set callbacks as a C++/C/Java Developer, please see 
the <a href="../prog_guide/08_1_im_overview.html#8142_CallbacksForC++Developers">HOOPS/3dGS Programming Guide</a>.
*/

class HIC_API Draw_3D_Nurbs_Curve
{
public:

	/*!
        Constructs a Draw_3D_Nurbs_Curve Callback object.
          \param key Key to the segment on which the callback is set.
          \param name The name of the callback.
          \param data Optional pointer to user data that can then be accessed from the
			callback.
    */ 

	Draw_3D_Nurbs_Curve(HC_KEY key, const char * name, void * data=0);
	
	/*! Deletes the callback object.  */       
	
	virtual ~Draw_3D_Nurbs_Curve();

	/*!
        The function you implement for your callback.
        \param nr A pointer to a const HIC_Rendition
        \param curve A pointer to a const HIC_NURBS_Curve
	*/

	virtual void draw_3d_nurbs_curve(HIC_Rendition const *nr, HIC_NURBS_Curve const *curve)
	{
		HIC_Draw_3D_NURBS_Curve(nr, curve);
	}

	/*!  Returns the name of the callback.  */       
	
	const char * GetName();

protected:   
	
	/*!   Returns the pointer to the user data that may have been provided when the callback was created.  */

	void * GetData();	
	
private:

	Draw_3D_Nurbs_Curve_Impl * _impl;
};

/* Testing 3D_Nurbs_Surface */


class Draw_3D_Nurbs_Surface_Impl;

/*!
The Draw_3D_Nurbs_Surface class is a base class used to define and set 
a callback at the "draw 3d nurbs surface" callback point during the HOOPS update 
cycle.  To use this class, derive your own child class from it and 
implement the method #draw_3d_nurbs_surface. For more information on 
how to define and set callbacks as a C++/C/Java Developer, please see 
the <a href="../prog_guide/08_1_im_overview.html#8142_CallbacksForC++Developers">HOOPS/3dGS Programming Guide</a>.
*/

class HIC_API Draw_3D_Nurbs_Surface
{
public:

	/*!
        Constructs a Draw_3D_Nurbs_Surface Callback object.
          \param key Key to the segment on which the callback is set.
          \param name The name of the callback.
          \param data Optional pointer to user data that can then be accessed from the
			callback.
    */ 

	Draw_3D_Nurbs_Surface(HC_KEY key, const char * name, void * data=0);
	
	/*! Deletes the callback object.  */       
	
	virtual ~Draw_3D_Nurbs_Surface();

	/*!
        The function you implement for your callback.
        \param nr A pointer to a const HIC_Rendition
        \param surface A pointer to a const HIC_NURBS_Surface
	*/

	virtual void draw_3d_nurbs_surface(HIC_Rendition const *nr, HIC_NURBS_Surface const *surface)
	{
		HIC_Draw_3D_NURBS_Surface(nr, surface);
	}

	/*!  Returns the name of the callback.  */       
	
	const char * GetName();

protected:  
	
	/*!   Returns the pointer to the user data that may have been provided when the callback was created.  */

	void * GetData();	
	
private:

	Draw_3D_Nurbs_Surface_Impl * _impl;
};


/* Testing 3D_Polygon */


class Draw_3D_Polygon_Impl;

/*!
The Draw_3D_Polygon class is a base class used to define and set 
a callback at the "draw 3d polygon" callback point during the HOOPS update 
cycle.  To use this class, derive your own child class from it and 
implement the method #draw_3d_polygon. For more information on 
how to define and set callbacks as a C++/C/Java Developer, please see 
the <a href="../prog_guide/08_1_im_overview.html#8142_CallbacksForC++Developers">HOOPS/3dGS Programming Guide</a>.
*/

class HIC_API Draw_3D_Polygon
{
public:

	/*!
        Constructs a Draw_3D_Polygon Callback object.
          \param key Key to the segment on which the callback is set.
          \param name The name of the callback.
          \param data Optional pointer to user data that can then be accessed from the
			callback.
    */ 

	Draw_3D_Polygon(HC_KEY key, const char * name, void * data=0);
	
	/*! Deletes the callback object.  */       
	
	virtual ~Draw_3D_Polygon();

	/*!
        The function you implement for your callback.
        \param nr A pointer to a const HIC_Rendition
        \param polygon A pointer to a const HIC_Polygon
	*/

	virtual void draw_3d_polygon(HIC_Rendition const *nr, HIC_Polygon const *polygon)
	{
		HIC_Draw_3D_Polygon(nr, polygon);
	}

	/*!  Returns the name of the callback.  */       
	
	const char * GetName();

protected:   
	
	/*!   Returns the pointer to the user data that may have been provided when the callback was created.  */

	void * GetData();	
	
private:

	Draw_3D_Polygon_Impl * _impl;
};


/* Testing 3D_Polyline */


class Draw_3D_Polyline_Impl;

/*!
The Draw_3D_Polyline class is a base class used to define and set 
a callback at the "draw 3d polyline" callback point during the HOOPS update 
cycle.  To use this class, derive your own child class from it and 
implement the method #draw_3d_polyline. For more information on 
how to define and set callbacks as a C++/C/Java Developer, please see 
the <a href="../prog_guide/08_1_im_overview.html#8142_CallbacksForC++Developers">HOOPS/3dGS Programming Guide</a>.
*/

class HIC_API Draw_3D_Polyline
{
public:

	/*!
        Constructs a Draw_3D_Polyline Callback object.
          \param key Key to the segment on which the callback is set.
          \param name The name of the callback.
          \param data Optional pointer to user data that can then be accessed from the
			callback.
    */ 

	Draw_3D_Polyline(HC_KEY key, const char * name, void * data=0);
	
	/*! Deletes the callback object.  */       
	
	virtual ~Draw_3D_Polyline();

	/*!
        The function you implement for your callback.
        \param nr A pointer to a const HIC_Rendition
        \param polyline A pointer to a const HIC_Polyline
	*/

	virtual void draw_3d_polyline(HIC_Rendition const *nr, HIC_Polyline const *polyline)
	{
		HIC_Draw_3D_Polyline(nr, polyline);
	}

	/*!  Returns the name of the callback.  */       
	
	const char * GetName();

protected:   
	
	/*!   Returns the pointer to the user data that may have been provided when the callback was created.  */

	void * GetData();	
	
private:

	Draw_3D_Polyline_Impl * _impl;
};


/* Testing 3D_Polymarkers */


class Draw_3D_Polymarkers_Impl;

/*!
The Draw_3D_Polymarkers class is a base class used to define and set 
a callback at the "draw 3d polymarker" callback point during the HOOPS update 
cycle.  To use this class, derive your own child class from it and 
implement the method #draw_3d_polymarkers. For more information on 
how to define and set callbacks as a C++/C/Java Developer, please see 
the <a href="../prog_guide/08_1_im_overview.html#8142_CallbacksForC++Developers">HOOPS/3dGS Programming Guide</a>.
*/


class HIC_API Draw_3D_Polymarkers
{
public:

	/*!
        Constructs a Draw_3D_Polymarkers Callback object.
          \param key Key to the segment on which the callback is set.
          \param name The name of the callback.
          \param data Optional pointer to user data that can then be accessed from the
			callback.
    */ 

	Draw_3D_Polymarkers(HC_KEY key, const char * name, void * data=0);
	
	/*! Deletes the callback object.  */       
	
	virtual ~Draw_3D_Polymarkers();

	/*!
        The function you implement for your callback.
        \param nr A pointer to a const HIC_Rendition
        \param pm A pointer to a const HIC_Polymarker
	*/

	virtual void draw_3d_polymarker(HIC_Rendition const *nr, HIC_Polymarker const *pm)
	{
		HIC_Draw_3D_Polymarker(nr, pm);
	}

	/*!  Returns the name of the callback.  */       
	
	const char * GetName();

protected:   
	
	/*!   Returns the pointer to the user data that may have been provided when the callback was created.  */

	void * GetData();	
	
private:

	Draw_3D_Polymarkers_Impl * _impl;
};


/* Testing 3D_Polycylinder */


class Draw_3D_Polycylinder_Impl;

/*!
The Draw_3D_Polycylinder class is a base class used to define and set 
a callback at the "draw 3d polyclinder" callback point during the HOOPS update 
cycle.  To use this class, derive your own child class from it and 
implement the method #draw_3d_polycylinder. For more information on 
how to define and set callbacks as a C++/C/Java Developer, please see 
the <a href="../prog_guide/08_1_im_overview.html#8142_CallbacksForC++Developers">HOOPS/3dGS Programming Guide</a>.
*/

class HIC_API Draw_3D_Polycylinder
{
public:

	/*!
        Constructs a Draw_3D_Polycylinder Callback object.
          \param key Key to the segment on which the callback is set.
          \param name The name of the callback.
          \param data Optional pointer to user data that can then be accessed from the
			callback.
    */ 

	Draw_3D_Polycylinder(HC_KEY key, const char * name, void * data=0);
	
	/*! Deletes the callback object.  */       
	
	virtual ~Draw_3D_Polycylinder();

	/*!
        The function you implement for your callback.
        \param nr A pointer to a const HIC_Rendition
        \param polycylinder A pointer to a const HIC_Polycylinder
	*/

	virtual void draw_3d_polycylinder(HIC_Rendition const *nr, HIC_Polycylinder const *polycylinder)
	{
		HIC_Draw_3D_PolyCylinder(nr, polycylinder);
	}

	/*!  Returns the name of the callback.  */       
	
	const char * GetName();

protected:   
	
	/*!   Returns the pointer to the user data that may have been provided when the callback was created.  */

	void * GetData();	
	
private:

	Draw_3D_Polycylinder_Impl * _impl;
};

/* Testing 3D_Polyedge */


class Draw_3D_Polyedge_Impl;

/*!
The Draw_3D_Polyedge class is a base class used to define and set 
a callback at the "draw 3d polyedge" callback point during the HOOPS update 
cycle.  To use this class, derive your own child class from it and 
implement the method #draw_3d_polyedge. For more information on 
how to define and set callbacks as a C++/C/Java Developer, please see 
the <a href="../prog_guide/08_1_im_overview.html#8142_CallbacksForC++Developers">HOOPS/3dGS Programming Guide</a>.
*/

class HIC_API Draw_3D_Polyedge
{
public:

	/*!
        Constructs a Draw_3D_Polyedge Callback object.
          \param key Key to the segment on which the callback is set.
          \param name The name of the callback.
          \param data Optional pointer to user data that can then be accessed from the
			callback.
    */ 

	Draw_3D_Polyedge(HC_KEY key, const char * name, void * data=0);
	
	/*! Deletes the callback object.  */       
	
	virtual ~Draw_3D_Polyedge();

	/*!
        The function you implement for your callback.
        \param nr A pointer to a const HIC_Rendition
        \param pe A pointer to a const HIC_Polyedge
	*/

	virtual void draw_3d_polyedge(HIC_Rendition const *nr, HIC_Polyedge const *pe)
	{
		HIC_Draw_3D_Polyedge(nr, pe);
	}

	/*!  Returns the name of the callback.  */       
	
	const char * GetName();

protected:   
	
	/*!   Returns the pointer to the user data that may have been provided when the callback was created.  */

	void * GetData();	
	
private:

	Draw_3D_Polyedge_Impl * _impl;
};

/* Testing 3D_Polyhedron */


class Draw_3D_Polyhedron_Impl;

/*!
The Draw_3D_Polyhedron class is a base class used to define and set 
a callback at the "draw 3d polyhedron" callback point during the HOOPS update 
cycle.  To use this class, derive your own child class from it and 
implement the method #draw_3d_polyhedron. For more information on 
how to define and set callbacks as a C++/C/Java Developer, please see 
the <a href="../prog_guide/08_1_im_overview.html#8142_CallbacksForC++Developers">HOOPS/3dGS Programming Guide</a>.
*/

class HIC_API Draw_3D_Polyhedron
{
public:

	/*!
        Constructs a Draw_3D_Polyhedron Callback object.
          \param key Key to the segment on which the callback is set.
          \param name The name of the callback.
          \param data Optional pointer to user data that can then be accessed from the
			callback.
    */ 

	Draw_3D_Polyhedron(HC_KEY key, const char * name, void * data=0);
	
	/*! Deletes the callback object.  */       
	
	virtual ~Draw_3D_Polyhedron();

	/*!
        The function you implement for your callback.
        \param nr A pointer to a const HIC_Rendition
        \param grid A phon to a const HIC_Polyhedron
	*/

	virtual void draw_3d_polyhedron(HIC_Rendition const *nr, HIC_Polyhedron const *phon)
	{
		HIC_Draw_3D_Polyhedron(nr, phon);
	}

	/*!  Returns the name of the callback.  */       
	
	const char * GetName();

protected:   
	
	/*!   Returns the pointer to the user data that may have been provided when the callback was created.  */

	void * GetData();	
	
private:

	Draw_3D_Polyhedron_Impl * _impl;
};


/* Testing 3D_Shell */


class Draw_3D_Shell_Impl;

/*!
The Draw_3D_Shell class is a base class used to define and set 
a callback at the "draw 3d shell" callback point during the HOOPS update 
cycle.  To use this class, derive your own child class from it and 
implement the method #draw_3d_shell. For more information on 
how to define and set callbacks as a C++/C/Java Developer, please see 
the <a href="../prog_guide/08_1_im_overview.html#8142_CallbacksForC++Developers">HOOPS/3dGS Programming Guide</a>.
*/

class HIC_API Draw_3D_Shell
{
public:

	/*!
        Constructs a Draw_3D_Shell Callback object.
          \param key Key to the segment on which the callback is set.
          \param name The name of the callback.
          \param data Optional pointer to user data that can then be accessed from the
			callback.
    */ 

	Draw_3D_Shell(HC_KEY key, const char * name, void * data=0);
	
	/*! Deletes the callback object.  */       
	
	virtual ~Draw_3D_Shell();


	/*!
        The function you implement for your callback.
        \param nr A pointer to a const HIC_Rendition
        \param phon A pointer to a const HIC_Polyhedron
	*/

	virtual void draw_3d_shell(HIC_Rendition const *nr, HIC_Polyhedron const *phon)
	{
		HIC_Draw_3D_Shell(nr, phon);
	}

	/*!  Returns the name of the callback.  */       
	
	const char * GetName();

protected:   
	
	/*!   Returns the pointer to the user data that may have been provided when the callback was created.  */

	void * GetData();	
	
private:

	Draw_3D_Shell_Impl * _impl;
};

/* Testing 3D_Sphere */


class Draw_3D_Sphere_Impl;

/*!
The Draw_3D_Sphere class is a base class used to define and set 
a callback at the "draw 3d sphere" callback point during the HOOPS update 
cycle.  To use this class, derive your own child class from it and 
implement the method #draw_3d_sphere. For more information on 
how to define and set callbacks as a C++/C/Java Developer, please see 
the <a href="../prog_guide/08_1_im_overview.html#8142_CallbacksForC++Developers">HOOPS/3dGS Programming Guide</a>.
*/

class HIC_API Draw_3D_Sphere
{
public:

	/*!
        Constructs a Draw_3D_Sphere Callback object.
          \param key Key to the segment on which the callback is set.
          \param name The name of the callback.
          \param data Optional pointer to user data that can then be accessed from the
			callback.
    */ 

	Draw_3D_Sphere(HC_KEY key, const char * name, void * data=0);
	
	/*! Deletes the callback object.  */       
	
	virtual ~Draw_3D_Sphere();

	/*!
        The function you implement for your callback.
        \param nr A pointer to a const HIC_Rendition
        \param sphere A pointer to a const HIC_Sphere
	*/

	virtual void draw_3d_sphere(HIC_Rendition const *nr, HIC_Sphere const *sphere)
	{
		HIC_Draw_3D_Sphere(nr, sphere);
	}

	/*!  Returns the name of the callback.  */       
	
	const char * GetName();

protected:   
	
	/*!   Returns the pointer to the user data that may have been provided when the callback was created.  */

	void * GetData();	
	
private:

	Draw_3D_Sphere_Impl * _impl;
};


/* Testing 3D_Surface */


class Draw_3D_Surface_Impl;

/*!
The Draw_3D_Surface class is a base class used to define and set 
a callback at the "draw 3d surface" callback point during the HOOPS update 
cycle.  To use this class, derive your own child class from it and 
implement the method #draw_3d_surface. For more information on 
how to define and set callbacks as a C++/C/Java Developer, please see 
the <a href="../prog_guide/08_1_im_overview.html#8142_CallbacksForC++Developers">HOOPS/3dGS Programming Guide</a>.
*/

class HIC_API Draw_3D_Surface
{
public:

	/*!
        Constructs a Draw_3D_Surface Callback object.
          \param key Key to the segment on which the callback is set.
          \param name The name of the callback.
          \param data Optional pointer to user data that can then be accessed from the
			callback.
    */ 

	Draw_3D_Surface(HC_KEY key, const char * name, void * data=0);
	
	/*! Deletes the callback object.  */       
	
	virtual ~Draw_3D_Surface();

	/*!
        The function you implement for your callback.
        \param nr A pointer to a const HIC_Rendition
        \param surface A pointer to a const HIC_NURBS_Surface
	*/

	virtual void draw_3d_surface(HIC_Rendition const *nr, HIC_NURBS_Surface const *surface)
	{
		HIC_Draw_3D_NURBS_Surface(nr, surface);
	}

	/*!  Returns the name of the callback.  */       
	
	const char * GetName();

protected:   
	
	/*!   Returns the pointer to the user data that may have been provided when the callback was created.  */

	void * GetData();	
	
private:

	Draw_3D_Surface_Impl * _impl;
};


/* Testing 3D_Tristrip */


class Draw_3D_Tristrip_Impl;

/*!
The Draw_3D_Tristrip class is a base class used to define and set 
a callback at the "draw 3d tristrip" callback point during the HOOPS update 
cycle.  To use this class, derive your own child class from it and 
implement the method #draw_3d_tristrip. For more information on 
how to define and set callbacks as a C++/C/Java Developer, please see 
the <a href="../prog_guide/08_1_im_overview.html#8142_CallbacksForC++Developers">HOOPS/3dGS Programming Guide</a>.
*/

class HIC_API Draw_3D_Tristrip
{
public:

	/*!
        Constructs a Draw_3D_Tristrip Callback object.
          \param key Key to the segment on which the callback is set.
          \param name The name of the callback.
          \param data Optional pointer to user data that can then be accessed from the
			callback.
    */ 

	Draw_3D_Tristrip(HC_KEY key, const char * name, void * data=0);
	
	/*! Deletes the callback object.  */       
	
	virtual ~Draw_3D_Tristrip();

	/*!
        The function you implement for your callback.
        \param nr A pointer to a const HIC_Rendition
        \param tristrip A pointer to a const HIC_Tristrip
	*/

	virtual void draw_3d_tristrip(HIC_Rendition const *nr, HIC_Tristrip const *tristrip)
	{
		HIC_Draw_3D_Tristrip(nr, tristrip);
	}

	/*!  Returns the name of the callback.  */       
	
	const char * GetName();

protected:   
	
	/*!   Returns the pointer to the user data that may have been provided when the callback was created.  */

	void * GetData();	
	
private:

	Draw_3D_Tristrip_Impl * _impl;
};

/* Testing DC_Colorized_Face */


class Draw_DC_Colorized_Face_Impl;

/*!
The Draw_DC_Colorized_Face class is a base class used to define and set 
a callback at the "draw dc colorized face" callback point during the HOOPS update 
cycle.  To use this class, derive your own child class from it and 
implement the method #draw_dc_colorized_face. For more information on 
how to define and set callbacks as a C++/C/Java Developer, please see 
the <a href="../prog_guide/08_1_im_overview.html#8142_CallbacksForC++Developers">HOOPS/3dGS Programming Guide</a>.
*/
#ifdef SWIG
%typemap (imtype,directorinattributes="[MarshalAs(UnmanagedType.LPArray,SizeParamIndex=1)]") HIC_DC_Point const *points "HIC_DC_Point[]"
%typemap (imtype,directorinattributes="[MarshalAs(UnmanagedType.LPArray,SizeParamIndex=1)]") HIC_RGBAS32 const *colors "int[]"
#endif
class HIC_API Draw_DC_Colorized_Face
{
public:

	/*!
        Constructs a Draw_DC_Colorized_Face Callback object.
          \param key Key to the segment on which the callback is set.
          \param name The name of the callback.
          \param data Optional pointer to user data that can then be accessed from the
			callback.
    */ 

	Draw_DC_Colorized_Face(HC_KEY key, const char * name, void * data=0);
	
	/*! Deletes the callback object.  */       
	
	virtual ~Draw_DC_Colorized_Face();

	/*!
        The function you implement for your callback.
        \param nr A pointer to a const HIC_Rendition
        \param count A int
		\param points A pointer to a const HIC_DC_Point
		\param colors A pointer to a const HIC_RGBAS32
	*/

	virtual void draw_dc_colorized_face(HIC_Rendition const *nr, int count, HIC_DC_Point const *points, HIC_RGBAS32 const *colors)
	{
		HIC_Draw_DC_Colorized_Face(nr, count, points, colors);
	}

	/*!  Returns the name of the callback.  */       
	
	const char * GetName();

protected:   
	
	/*!   Returns the pointer to the user data that may have been provided when the callback was created.  */

	void * GetData();	
	
private:

	Draw_DC_Colorized_Face_Impl * _impl;
};
#ifdef SWIG
%clear HIC_DC_Point const *points;
%clear HIC_RGBAS32 const *colors;
#endif

/* Testing DC_Colorized_Line */


class Draw_DC_Colorized_Line_Impl;

/*!
The Draw_DC_Colorized_Line class is a base class used to define and set 
a callback at the "draw dc colorized line" callback point during the HOOPS update 
cycle.  To use this class, derive your own child class from it and 
implement the method #draw_dc_colorized_line. For more information on 
how to define and set callbacks as a C++/C/Java Developer, please see 
the <a href="../prog_guide/08_1_im_overview.html#8142_CallbacksForC++Developers">HOOPS/3dGS Programming Guide</a>.
*/
#ifdef SWIG
%typemap (imtype,directorinattributes="[MarshalAs(UnmanagedType.LPArray,SizeConst=2)]") HIC_DC_Point const *points "HIC_DC_Point[]"
%typemap (imtype,directorinattributes="[MarshalAs(UnmanagedType.LPArray,SizeConst=2)]") HIC_RGBAS32 const *colors "int[]"
#endif
class HIC_API Draw_DC_Colorized_Line
{
public:

	/*!
        Constructs a Draw_DC_Colorized_Line Callback object.
          \param key Key to the segment on which the callback is set.
          \param name The name of the callback.
          \param data Optional pointer to user data that can then be accessed from the
			callback.
    */ 

	Draw_DC_Colorized_Line(HC_KEY key, const char * name, void * data=0);
	
	/*! Deletes the callback object.  */       
	
	virtual ~Draw_DC_Colorized_Line();

	/*!
        The function you implement for your callback.
        \param nr A pointer to a const HIC_Rendition
		\param points A pointer to a const HIC_DC_Point
		\param colors A pointer to a const HIC_RGBAS32
	*/

	virtual void draw_dc_colorized_line(HIC_Rendition const *nr, HIC_DC_Point const *points, HIC_RGBAS32 const *colors)
	{
		HIC_Draw_DC_Colorized_Line(nr, points, colors);
	}

	/*!  Returns the name of the callback.  */       
	
	const char * GetName();

protected:   
	
	/*!   Returns the pointer to the user data that may have been provided when the callback was created.  */

	void * GetData();	
	
private:

	Draw_DC_Colorized_Line_Impl * _impl;
};
#ifdef SWIG
%clear HIC_DC_Point const *points;
%clear HIC_RGBAS32 const *colors;
#endif


/* Testing DC_Colorized_Marker */


class Draw_DC_Colorized_Marker_Impl;

/*!
The Draw_DC_Colorized_Marker class is a base class used to define and set 
a callback at the "draw dc colorized marker" callback point during the HOOPS update 
cycle.  To use this class, derive your own child class from it and 
implement the method #draw_dc_colorized_marker. For more information on 
how to define and set callbacks as a C++/C/Java Developer, please see 
the <a href="../prog_guide/08_1_im_overview.html#8142_CallbacksForC++Developers">HOOPS/3dGS Programming Guide</a>.
*/
#ifdef SWIG
%typemap (imtype,directorinattributes="[MarshalAs(UnmanagedType.LPArray,SizeConst=1)]") HIC_DC_Point const *points "HIC_DC_Point[]"
%typemap (imtype,directorinattributes="[MarshalAs(UnmanagedType.LPArray,SizeConst=1)]") HIC_RGBAS32 const *colors "int[]"
#endif
class HIC_API Draw_DC_Colorized_Marker
{
public:

	/*!
        Constructs a Draw_DC_Colorized_Marker Callback object.
          \param key Key to the segment on which the callback is set.
          \param name The name of the callback.
          \param data Optional pointer to user data that can then be accessed from the
			callback.
    */ 

	Draw_DC_Colorized_Marker(HC_KEY key, const char * name, void * data=0);
	
	/*! Deletes the callback object.  */       
	
	virtual ~Draw_DC_Colorized_Marker();

	/*!
        The function you implement for your callback.
        \param nr A pointer to a const HIC_Rendition
		\param points A pointer to a const HIC_DC_Point
		\param color A pointer to a const HIC_RGBAS32
	*/

	virtual void draw_dc_colorized_marker(HIC_Rendition const *nr, HIC_DC_Point const *points, HIC_RGBAS32 const *colors)
	{
		HIC_Draw_DC_Colorized_Marker(nr, points, colors);
	}

	/*!  Returns the name of the callback.  */       
	
	const char * GetName();

protected:   
	
	/*!   Returns the pointer to the user data that may have been provided when the callback was created.  */

	void * GetData();	
	
private:

	Draw_DC_Colorized_Marker_Impl * _impl;
};
#ifdef SWIG
%clear HIC_DC_Point const *points;
%clear HIC_RGBAS32 const *colors;
#endif

/* Testing DC_Colorized_Polyline */


class Draw_DC_Colorized_Polyline_Impl;

/*!
The Draw_DC_Colorized_Polyline class is a base class used to define and set 
a callback at the "draw dc colorized polyline" callback point during the HOOPS update 
cycle.  To use this class, derive your own child class from it and 
implement the method #draw_dc_colorized_polyline. For more information on 
how to define and set callbacks as a C++/C/Java Developer, please see 
the <a href="../prog_guide/08_1_im_overview.html#8142_CallbacksForC++Developers">HOOPS/3dGS Programming Guide</a>.
*/
#ifdef SWIG
%typemap (imtype) HIC_DC_Point const *points "IntPtr"
%typemap (cstype) HIC_DC_Point const *points "IntPtr"
%typemap (imtype) HIC_RGBAS32 const *colors "IntPtr"
%typemap (cstype) HIC_RGBAS32 const *colors "IntPtr"
#endif
class HIC_API Draw_DC_Colorized_Polyline
{
public:

	/*!
        Constructs a Draw_DC_Colorized_Polyline Callback object.
          \param key Key to the segment on which the callback is set.
          \param name The name of the callback.
          \param data Optional pointer to user data that can then be accessed from the
			callback.
    */ 

	Draw_DC_Colorized_Polyline(HC_KEY key, const char * name, void * data=0);
	
	/*! Deletes the callback object.  */       
	
	virtual ~Draw_DC_Colorized_Polyline();

	/*!
        The function you implement for your callback.
        \param nr A pointer to a const HIC_Rendition
		\param points A pointer to a const HIC_DC_Point
		\param colors A pointer to a const HIC_RGBAS32
		\param single A boolean variable
	*/

	virtual void draw_dc_colorized_polyline(HIC_Rendition const *nr, int count, HIC_DC_Point const *points, HIC_RGBAS32 const *colors, bool single)
	{
		HIC_Draw_DC_Colorized_Polyline(nr, count, points, colors, single);
	}

	/*!  Returns the name of the callback.  */       
	
	const char * GetName();

protected:   
	
	/*!   Returns the pointer to the user data that may have been provided when the callback was created.  */

	void * GetData();	
	
private:

	Draw_DC_Colorized_Polyline_Impl * _impl;
};
#ifdef SWIG
%clear HIC_DC_Point const *points;
%clear HIC_RGBAS32 const *colors;
#endif


/* Testing DC_Colorized_Polymarker */


class Draw_DC_Colorized_Polymarker_Impl;

/*!
The Draw_DC_Colorized_Polymarker class is a base class used to define and set 
a callback at the "draw dc colorized polymarker" callback point during the HOOPS update 
cycle.  To use this class, derive your own child class from it and 
implement the method #draw_dc_colorized_polymarker. For more information on 
how to define and set callbacks as a C++/C/Java Developer, please see 
the <a href="../prog_guide/08_1_im_overview.html#8142_CallbacksForC++Developers">HOOPS/3dGS Programming Guide</a>.
*/
#ifdef SWIG
%typemap (imtype,directorinattributes="[MarshalAs(UnmanagedType.LPArray,SizeParamIndex=1)]") HIC_DC_Point const *points "HIC_DC_Point[]"
%typemap (imtype,directorinattributes="[MarshalAs(UnmanagedType.LPArray,SizeParamIndex=1)]") float const *rotations "float[]"
%typemap (imtype,directorinattributes="[MarshalAs(UnmanagedType.LPArray,SizeParamIndex=1)]") float const *size_fixups "float[]"
%typemap (imtype,directorinattributes="[MarshalAs(UnmanagedType.LPArray,SizeParamIndex=1)]") HIC_RGBAS32 const *colors "int[]"
#endif
class HIC_API Draw_DC_Colorized_Polymarker
{
public:

	/*!
        Constructs a Draw_DC_Colorized_Polymarker Callback object.
          \param key Key to the segment on which the callback is set.
          \param name The name of the callback.
          \param data Optional pointer to user data that can then be accessed from the
			callback.
    */ 

	Draw_DC_Colorized_Polymarker(HC_KEY key, const char * name, void * data=0);
	
	/*! Deletes the callback object.  */       
	
	virtual ~Draw_DC_Colorized_Polymarker();

	/*!
        The function you implement for your callback.
        \param nr A pointer to a const HIC_Rendition
        \param count A int
		\param points A pointer to a const HIC_DC_Point
		\param colors A pointer to a const HIC_RGBAS32
		\param rotations A pointer to a const float
		\param size_fixups A pointer to a const float
	*/

	virtual void draw_dc_colorized_polymarker(HIC_Rendition const *nr, int count, HIC_DC_Point const *points, HIC_RGBAS32 const *colors, bool single, float const *rotations, float const *size_fixups)
	{
		HIC_Draw_DC_Colorized_Polymarker(nr, count, points, colors, single, rotations, size_fixups);
	}

	/*!  Returns the name of the callback.  */       
	
	const char * GetName();

protected:   
	
	/*!   Returns the pointer to the user data that may have been provided when the callback was created.  */

	void * GetData();	
	
private:

	Draw_DC_Colorized_Polymarker_Impl * _impl;
};
#ifdef SWIG
%clear HIC_DC_Point const *points;
%clear float const *size_fixups;
%clear float const *rotations;
%clear HIC_RGBAS32 const *colors;
#endif


/* Testing DC_Colorized_Polytriangle */


class Draw_DC_Colorized_Polytriangle_Impl;

/*!
The Draw_DC_Colorized_Polytriangle class is a base class used to define and set 
a callback at the "draw dc colorized polytriangle" callback point during the HOOPS update 
cycle.  To use this class, derive your own child class from it and 
implement the method #draw_dc_colorized_polytriangle. For more information on 
how to define and set callbacks as a C++/C/Java Developer, please see 
the <a href="../prog_guide/08_1_im_overview.html#8142_CallbacksForC++Developers">HOOPS/3dGS Programming Guide</a>.
*/

#ifdef SWIG
%typemap (imtype) HIC_DC_Point const *points "IntPtr"
%typemap (imtype) HIC_RGBAS32 const *colors "IntPtr"
%typemap (cstype) HIC_DC_Point const *points "IntPtr"
%typemap (cstype) HIC_RGBAS32 const *colors "IntPtr"
#endif
class HIC_API Draw_DC_Colorized_Polytriangle
{
public:

	/*!
        Constructs a Draw_DC_Colorized_Polytriangle Callback object.
          \param key Key to the segment on which the callback is set.
          \param name The name of the callback.
          \param data Optional pointer to user data that can then be accessed from the
			callback.
    */ 

	Draw_DC_Colorized_Polytriangle(HC_KEY key, const char * name, void * data=0);
	
	/*! Deletes the callback object.  */       
	
	virtual ~Draw_DC_Colorized_Polytriangle();

	/*!
        The function you implement for your callback.
        \param nr A pointer to a const HIC_Rendition
		\param int A integer variable
		\param points A pointer to a const HIC_DC_Point
		\param colors A pointer to a const HIC_RGBAS32
		\param single A boolean variable
	*/

	virtual void draw_dc_colorized_polytriangle(HIC_Rendition const *nr, int count, HIC_DC_Point const *points, HIC_RGBAS32 const *colors, bool single)
	{
		HIC_Draw_DC_Colorized_Polytriangle(nr, count, points, colors, single);
	}

	/*!  Returns the name of the callback.  */       
	
	const char * GetName();

protected:   
	
	/*!   Returns the pointer to the user data that may have been provided when the callback was created.  */

	void * GetData();	
	
private:

	Draw_DC_Colorized_Polytriangle_Impl * _impl;
};
#ifdef SWIG
%clear HIC_DC_Point const *points;
%clear HIC_RGBAS32 const *colors;
#endif

/* Testing DC_Colorized_Triangle */


class Draw_DC_Colorized_Triangle_Impl;

/*!
The Draw_DC_Colorized_Triangle class is a base class used to define and set 
a callback at the "draw dc colorized triangle" callback point during the HOOPS update 
cycle.  To use this class, derive your own child class from it and 
implement the method #draw_dc_colorized_triangle. For more information on 
how to define and set callbacks as a C++/C/Java Developer, please see 
the <a href="../prog_guide/08_1_im_overview.html#8142_CallbacksForC++Developers">HOOPS/3dGS Programming Guide</a>.
*/
#ifdef SWIG
%typemap (imtype,directorinattributes="[MarshalAs(UnmanagedType.LPArray,SizeConst=3)]") HIC_DC_Point const *points "HIC_DC_Point[]"
%typemap (imtype,directorinattributes="[MarshalAs(UnmanagedType.LPArray,SizeConst=3)]") HIC_RGBAS32 const *colors "int[]"
#endif
class HIC_API Draw_DC_Colorized_Triangle
{
public:

	/*!
        Constructs a Draw_DC_Colorized_Triangle Callback object.
          \param key Key to the segment on which the callback is set.
          \param name The name of the callback.
          \param data Optional pointer to user data that can then be accessed from the
			callback.
    */ 

	Draw_DC_Colorized_Triangle(HC_KEY key, const char * name, void * data=0);
	
	/*! Deletes the callback object.  */       
	
	virtual ~Draw_DC_Colorized_Triangle();

	/*!
        The function you implement for your callback.
        \param nr A pointer to a const HIC_Rendition
		\param points A pointer to a const HIC_DC_Point
		\param colors A pointer to a const HIC_RGBAS32
	*/

	virtual void draw_dc_colorized_triangle(HIC_Rendition const *nr, HIC_DC_Point const *points, HIC_RGBAS32 const *colors)
	{
		HIC_Draw_DC_Colorized_Triangle(nr, points, colors);
	}

	/*!  Returns the name of the callback.  */       
	
	const char * GetName();

protected:   
	
	/*!   Returns the pointer to the user data that may have been provided when the callback was created.  */

	void * GetData();	
	
private:

	Draw_DC_Colorized_Triangle_Impl * _impl;
};
#ifdef SWIG
%clear HIC_DC_Point const *points;
%clear HIC_RGBAS32 const *colors;
#endif

/* Testing DC_Face */


class Draw_DC_Face_Impl;

/*!
The Draw_DC_Face class is a base class used to define and set 
a callback at the "draw dc face" callback point during the HOOPS update 
cycle.  To use this class, derive your own child class from it and 
implement the method #draw_dc_face. For more information on 
how to define and set callbacks as a C++/C/Java Developer, please see 
the <a href="../prog_guide/08_1_im_overview.html#8142_CallbacksForC++Developers">HOOPS/3dGS Programming Guide</a>.
*/
#ifdef SWIG
%typemap (imtype,directorinattributes="[MarshalAs(UnmanagedType.LPArray,SizeParamIndex=1)]") HIC_DC_Point const *points "HIC_DC_Point[]"
#endif
class HIC_API Draw_DC_Face
{
public:

	/*!
        Constructs a Draw_DC_Face Callback object.
          \param key Key to the segment on which the callback is set.
          \param name The name of the callback.
          \param data Optional pointer to user data that can then be accessed from the
			callback.
    */ 

	Draw_DC_Face(HC_KEY key, const char * name, void * data=0);
	
	/*! Deletes the callback object.  */       
	
	virtual ~Draw_DC_Face();

	/*!
        The function you implement for your callback.
        \param nr A pointer to a const HIC_Rendition
		\param points A pointer to a const HIC_DC_Point
	*/

	virtual void draw_dc_face(HIC_Rendition const *nr, int count, HIC_DC_Point const *points)
	{
		HIC_Draw_DC_Face(nr, count, points);
	}

	/*!  Returns the name of the callback.  */       
	
	const char * GetName();

protected:   
	
	/*!   Returns the pointer to the user data that may have been provided when the callback was created.  */

	void * GetData();	
	
private:

	Draw_DC_Face_Impl * _impl;
};
#ifdef SWIG
%clear HIC_DC_Point const *points;
#endif 

/* Testing DC_Gouraud_Line */


class Draw_DC_Gouraud_Line_Impl;

/*!
The Draw_DC_Gouraud_Line class is a base class used to define and set 
a callback at the "draw dc gouraud line" callback point during the HOOPS update 
cycle.  To use this class, derive your own child class from it and 
implement the method #draw_dc_gouraud_line. For more information on 
how to define and set callbacks as a C++/C/Java Developer, please see 
the <a href="../prog_guide/08_1_im_overview.html#8142_CallbacksForC++Developers">HOOPS/3dGS Programming Guide</a>.
*/
#ifdef SWIG
%typemap (imtype,directorinattributes="[MarshalAs(UnmanagedType.LPArray,SizeConst=2)]") HIC_DC_Point const *points "HIC_DC_Point[]"
%typemap (imtype,directorinattributes="[MarshalAs(UnmanagedType.LPArray,SizeConst=2)]") HIC_RGBAS32 const *colors "int[]"
#endif
class HIC_API Draw_DC_Gouraud_Line
{
public:

	/*!
        Constructs a Draw_DC_Gouraud_Line Callback object.
          \param key Key to the segment on which the callback is set.
          \param name The name of the callback.
          \param data Optional pointer to user data that can then be accessed from the
			callback.
    */ 

	Draw_DC_Gouraud_Line(HC_KEY key, const char * name, void * data=0);
	
	/*! Deletes the callback object.  */       
	
	virtual ~Draw_DC_Gouraud_Line();
	/*!
        The function you implement for your callback.
        \param nr A pointer to a const HIC_Rendition
		\param points A pointer to a const HIC_DC_Point
		\param colors A pointer to a const HICRGBAS32
	*/

	virtual void draw_dc_gouraud_line(HIC_Rendition const *nr, HIC_DC_Point const *points, HIC_RGBAS32 const *colors)
	{
		HIC_Draw_DC_Gouraud_Line(nr, points, colors);
	}

	/*!  Returns the name of the callback.  */       
	
	const char * GetName();

protected:   
	
	/*!   Returns the pointer to the user data that may have been provided when the callback was created.  */

	void * GetData();	
	
private:

	Draw_DC_Gouraud_Line_Impl * _impl;
};
#ifdef SWIG
%clear HIC_DC_Point const *points;
%clear HIC_RGBAS32 const *colors; 
#endif

/* Testing DC_Gouraud_Polyline */


class Draw_DC_Gouraud_Polyline_Impl;

/*!
The Draw_DC_Gouraud_Polyline class is a base class used to define and set 
a callback at the "draw dc gouraud polyline" callback point during the HOOPS update 
cycle.  To use this class, derive your own child class from it and 
implement the method #draw_dc_gouraud_polyline. For more information on 
how to define and set callbacks as a C++/C/Java Developer, please see 
the <a href="../prog_guide/08_1_im_overview.html#8142_CallbacksForC++Developers">HOOPS/3dGS Programming Guide</a>.
*/
#ifdef SWIG
%typemap (imtype) HIC_DC_Point const *points "IntPtr"
%typemap (cstype) HIC_DC_Point const *points "IntPtr"
%typemap (imtype) HIC_RGBAS32 const *colors "IntPtr"
%typemap (cstype) HIC_RGBAS32 const *colors "IntPtr"
#endif
class HIC_API Draw_DC_Gouraud_Polyline
{
public:

	/*!
        Constructs a Draw_DC_Gouraud_Polyline Callback object.
          \param key Key to the segment on which the callback is set.
          \param name The name of the callback.
          \param data Optional pointer to user data that can then be accessed from the
			callback.
    */ 

	Draw_DC_Gouraud_Polyline(HC_KEY key, const char * name, void * data=0);
	
	/*! Deletes the callback object.  */       
	
	virtual ~Draw_DC_Gouraud_Polyline();

	/*!
        The function you implement for your callback.
        \param nr A pointer to a const HIC_Rendition
		\param points A pointer to a const HIC_DC_Point
		\param colors A pointer to a const HICRGBAS32
	*/

	virtual void draw_dc_gouraud_polyline(HIC_Rendition const *nr, int count, HIC_DC_Point const *points, HIC_RGBAS32 const *colors)
	{
		HIC_Draw_DC_Gouraud_Polyline(nr, count, points, colors);
	}

	/*!  Returns the name of the callback.  */       
	
	const char * GetName();

protected:   
	
	/*!   Returns the pointer to the user data that may have been provided when the callback was created.  */

	void * GetData();	
	
private:

	Draw_DC_Gouraud_Polyline_Impl * _impl;
};
#ifdef SWIG
%clear HIC_DC_Point const *points;
%clear HIC_RGBAS32 const *colors;
#endif


/* Testing DC_Gouraud_Polytriangle */


class Draw_DC_Gouraud_Polytriangle_Impl;

/*!
The Draw_DC_Gouraud_Polytriangle class is a base class used to define and set 
a callback at the "draw dc gouraud polytriangle" callback point during the HOOPS update 
cycle.  To use this class, derive your own child class from it and 
implement the method #draw_dc_gouraud_polytriangle. For more information on 
how to define and set callbacks as a C++/C/Java Developer, please see 
the <a href="../prog_guide/08_1_im_overview.html#8142_CallbacksForC++Developers">HOOPS/3dGS Programming Guide</a>.
*/
#ifdef SWIG
%typemap (imtype) HIC_DC_Point const *points "IntPtr"
%typemap (imtype) HIC_RGBAS32 const *colors "IntPtr"
%typemap (cstype) HIC_DC_Point const *points "IntPtr"
%typemap (cstype) HIC_RGBAS32 const *colors "IntPtr"
#endif
class HIC_API Draw_DC_Gouraud_Polytriangle
{
public:

	/*!
        Constructs a Draw_DC_Gouraud_Polytriangle Callback object.
          \param key Key to the segment on which the callback is set.
          \param name The name of the callback.
          \param data Optional pointer to user data that can then be accessed from the
			callback.
    */ 

	Draw_DC_Gouraud_Polytriangle(HC_KEY key, const char * name, void * data=0);
	
	/*! Deletes the callback object.  */      
	
	virtual ~Draw_DC_Gouraud_Polytriangle();

	/*!
        The function you implement for your callback.
        \param nr A pointer to a const HIC_Rendition
		\param count A integer variable
		\param points A pointer to a const HIC_DC_Point
		\param colors A pointer to a const HICRGBAS32
	*/

	virtual void draw_dc_gouraud_polytriangle(HIC_Rendition const *nr, int count, HIC_DC_Point const *points, HIC_RGBAS32 const *colors)
	{
		HIC_Draw_DC_Gouraud_Polytriangle(nr, count, points, colors);
	}

	/*!  Returns the name of the callback.  */       
	
	const char * GetName();

protected:   
	
	/*!   Returns the pointer to the user data that may have been provided when the callback was created.  */

	void * GetData();	
	
private:

	Draw_DC_Gouraud_Polytriangle_Impl * _impl;
};
#ifdef SWIG
%clear HIC_DC_Point const *points;
%clear HIC_RGBAS32 const *colors;
#endif

/* Testing DC_Gouraud_Triangle */


class Draw_DC_Gouraud_Triangle_Impl;

/*!
The Draw_DC_Gouraud_Triangle class is a base class used to define and set 
a callback at the "draw dc gouraud triangle" callback point during the HOOPS update 
cycle.  To use this class, derive your own child class from it and 
implement the method #draw_dc_gouraud_triangle. For more information on 
how to define and set callbacks as a C++/C/Java Developer, please see 
the <a href="../prog_guide/08_1_im_overview.html#8142_CallbacksForC++Developers">HOOPS/3dGS Programming Guide</a>.
*/
#ifdef SWIG
%typemap (imtype,directorinattributes="[MarshalAs(UnmanagedType.LPArray,SizeConst=3)]") HIC_DC_Point const *points "HIC_DC_Point[]"
%typemap (imtype,directorinattributes="[MarshalAs(UnmanagedType.LPArray,SizeConst=3)]") HIC_RGBAS32 const *colors "int[]"
#endif
class HIC_API Draw_DC_Gouraud_Triangle
{
public:

	/*!
        Constructs a Draw_DC_Gouraud_Triangle Callback object.
          \param key Key to the segment on which the callback is set.
          \param name The name of the callback.
          \param data Optional pointer to user data that can then be accessed from the
			callback.
    */ 

	Draw_DC_Gouraud_Triangle(HC_KEY key, const char * name, void * data=0);
	
	/*! Deletes the callback object.  */       
	
	virtual ~Draw_DC_Gouraud_Triangle();

	/*!
        The function you implement for your callback.
        \param nr A pointer to a const HIC_Rendition
		\param points A pointer to a const HIC_DC_Point
		\param colors A pointer to a const HICRGBAS32
	*/

	virtual void draw_dc_gouraud_triangle(HIC_Rendition const *nr, HIC_DC_Point const *points, HIC_RGBAS32 const *colors)
	{
		HIC_Draw_DC_Gouraud_Triangle(nr, points, colors);
	}

	/*!  Returns the name of the callback.  */       
	
	const char * GetName();

protected:   
	
	/*!   Returns the pointer to the user data that may have been provided when the callback was created.  */

	void * GetData();	
	
private:

	Draw_DC_Gouraud_Triangle_Impl * _impl;
};
#ifdef SWIG
%clear HIC_DC_Point const *points;
%clear HIC_RGBAS32 const *colors;
#endif

class Draw_DC_Marker_Impl;

/*!
The Draw_DC_Marker class is a base class used to define and set 
a callback at the "draw dc marker" callback point during the HOOPS update 
cycle.  To use this class, derive your own child class from it and 
implement the method #draw_dc_marker. For more information on 
how to define and set callbacks as a C++/C/Java Developer, please see 
the <a href="../prog_guide/08_1_im_overview.html#8142_CallbacksForC++Developers">HOOPS/3dGS Programming Guide</a>.
*/
#ifdef SWIG
%typemap (imtype,directorinattributes="[MarshalAs(UnmanagedType.LPArray,SizeConst=1)]") HIC_DC_Point const *points "HIC_DC_Point[]"
#endif
class HIC_API Draw_DC_Marker
{
public:

	/*!
        Constructs a Draw_DC_Marker Callback object.
          \param key Key to the segment on which the callback is set.
          \param name The name of the callback.
          \param data Optional pointer to user data that can then be accessed from the
			callback.
    */ 

	Draw_DC_Marker(HC_KEY key, const char * name, void * data=0);
	
	/*! Deletes the callback object.  */       
	
	virtual ~Draw_DC_Marker();

	/*!
        The function you implement for your callback.
        \param nr A pointer to a const HIC_Rendition
		\param points A pointer to a const HIC_DC_Point
	*/

	virtual void draw_dc_marker(HIC_Rendition const *nr, HIC_DC_Point const *points)
	{
		HIC_Draw_DC_Marker(nr, points);
	}

	/*!  Returns the name of the callback.  */       
	
	const char * GetName();

protected:   
	
	/*!   Returns the pointer to the user data that may have been provided when the callback was created.  */

	void * GetData();	
	
private:

	Draw_DC_Marker_Impl * _impl;
};
#ifdef SWIG
%clear HIC_DC_Point const *points;
#endif

class Draw_DC_Phong_Polyline_Impl;

/*!
The Draw_DC_Phong_Polyline class is a base class used to define and set 
a callback at the "draw dc phong polyline" callback point during the HOOPS update 
cycle.  To use this class, derive your own child class from it and 
implement the method #draw_dc_phong_polyline. For more information on 
how to define and set callbacks as a C++/C/Java Developer, please see 
the <a href="../prog_guide/08_1_im_overview.html#8142_CallbacksForC++Developers">HOOPS/3dGS Programming Guide</a>.
*/
#ifdef SWIG
%typemap (imtype) HIC_DC_Point const *points "IntPtr"
%typemap (imtype) HIC_RGBA const *colors "IntPtr"
%typemap (imtype) HIC_Plane const *planes "IntPtr"
%typemap (imtype) HIC_DPlane const *dplanes "IntPtr"
%typemap (cstype) HIC_DC_Point const *points "IntPtr"
%typemap (cstype) HIC_RGBA const *colors "IntPtr"
%typemap (cstype) HIC_Plane const *planes "IntPtr"
%typemap (cstype) HIC_DPlane const *dplanes "IntPtr"
#endif
class HIC_API Draw_DC_Phong_Polyline
{
public:

	/*!
        Constructs a Draw_DC_Phong_Polyline Callback object.
          \param key Key to the segment on which the callback is set.
          \param name The name of the callback.
          \param data Optional pointer to user data that can then be accessed from the
			callback.
    */ 

	Draw_DC_Phong_Polyline(HC_KEY key, const char * name, void * data=0);
	
	/*! Deletes the callback object.  */       
	
	virtual ~Draw_DC_Phong_Polyline();

	/*!
        The function you implement for your callback.
        \param nr A pointer to a const HIC_Rendition
		\param count A integer variable
		\param points A pointer to a const HIC_DC_Point
		\param colors A pointer to a const HICRGBA
		\param planes A pointer to a const HIC_Plane
		\param dplanes A pointer to a const HIC_DPlane
	*/

	virtual void draw_dc_phong_polyline(HIC_Rendition const *nr, int count, HIC_DC_Point const *points, HIC_RGBA const *colors, HIC_Plane const *planes, HIC_DPlane const *dplanes)
	{
		HIC_Draw_DC_Phong_Polyline(nr, count, points, colors, planes, dplanes);
	}

	/*!  Returns the name of the callback.  */       
	
	const char * GetName();

protected:  
	
	/*!   Returns the pointer to the user data that may have been provided when the callback was created.  */

	void * GetData();	
	
private:

	Draw_DC_Phong_Polyline_Impl * _impl;
};
#ifdef SWIG
%clear HIC_DC_Point const *points;
%clear HIC_RGBA const *colors;
%clear HIC_Plane const *planes;
%clear HIC_DPlane const *dplanes;
#endif

class Draw_DC_Phong_Polytriangle_Impl;

/*!
The Draw_DC_Phong_Polytriangle class is a base class used to define and set 
a callback at the "draw dc phong polytriangle" callback point during the HOOPS update 
cycle.  To use this class, derive your own child class from it and 
implement the method #draw_dc_phong_polytriangle. For more information on 
how to define and set callbacks as a C++/C/Java Developer, please see 
the <a href="../prog_guide/08_1_im_overview.html#8142_CallbacksForC++Developers">HOOPS/3dGS Programming Guide</a>.
*/
#ifdef SWIG
%typemap (imtype) HIC_DC_Point const *points "IntPtr"
%typemap (imtype) HIC_RGBA const *colors "IntPtr"
%typemap (imtype) HIC_Plane const *planes "IntPtr"
%typemap (imtype) HIC_DPlane const *dplanes "IntPtr"
%typemap (cstype) HIC_DC_Point const *points "IntPtr"
%typemap (cstype) HIC_RGBA const *colors "IntPtr"
%typemap (cstype) HIC_Plane const *planes "IntPtr"
%typemap (cstype) HIC_DPlane const *dplanes "IntPtr"
#endif
class HIC_API Draw_DC_Phong_Polytriangle
{
public:

	/*!
        Constructs a Draw_DC_Phong_Polytriangle Callback object.
          \param key Key to the segment on which the callback is set.
          \param name The name of the callback.
          \param data Optional pointer to user data that can then be accessed from the
			callback.
    */ 

	Draw_DC_Phong_Polytriangle(HC_KEY key, const char * name, void * data=0);
	
	/*! Deletes the callback object.  */       
	
	virtual ~Draw_DC_Phong_Polytriangle();

	/*!
        The function you implement for your callback.
        \param nr A pointer to a const HIC_Rendition
		\param count A integer variable
		\param points A pointer to a const HIC_DC_Point
		\param colors A pointer to a const HICRGBA
		\param planes A pointer to a const HIC_Plane
		\param dplanes A pointer to a const HIC_DPlane
	*/

	virtual void draw_dc_phong_polytriangle(HIC_Rendition const *nr, int count, HIC_DC_Point const *points, HIC_RGBA const *colors, HIC_Plane const *planes, HIC_DPlane const *dplanes)
	{
		HIC_Draw_DC_Phong_Polytriangle(nr, count, points, colors, planes, dplanes);
	}

	/*!  Returns the name of the callback.  */       
	
	const char * GetName();

protected:   
	
	/*!   Returns the pointer to the user data that may have been provided when the callback was created.  */

	void * GetData();	
	
private:

	Draw_DC_Phong_Polytriangle_Impl * _impl;
};
#ifdef SWIG
%clear HIC_DC_Point const *points;
%clear HIC_RGBA const *colors;
%clear HIC_Plane const *planes;
%clear HIC_DPlane const *dplanes;
#endif

class Draw_DC_Polyline_Impl;

/*!
The Draw_DC_Polyline class is a base class used to define and set 
a callback at the "draw dc polyline" callback point during the HOOPS update 
cycle.  To use this class, derive your own child class from it and 
implement the method #draw_dc_polyline. For more information on 
how to define and set callbacks as a C++/C/Java Developer, please see 
the <a href="../prog_guide/08_1_im_overview.html#8142_CallbacksForC++Developers">HOOPS/3dGS Programming Guide</a>.
*/

#ifdef SWIG
%typemap (imtype) HIC_DC_Point const *points "IntPtr"
%typemap (cstype) HIC_DC_Point const *points "IntPtr"
#endif
class HIC_API Draw_DC_Polyline
{
public:

	/*!
        Constructs a Draw_DC_Polyline Callback object.
          \param key Key to the segment on which the callback is set.
          \param name The name of the callback.
          \param data Optional pointer to user data that can then be accessed from the
			callback.
    */ 

	Draw_DC_Polyline(HC_KEY key, const char * name, void * data=0);
	
	/*! Deletes the callback object.  */       
	
	virtual ~Draw_DC_Polyline();

	/*!
        The function you implement for your callback.
        \param nr A pointer to a const HIC_Rendition
		\param count A integer variable
		\param points A pointer to a const HIC_DC_Point
	*/
	virtual void draw_dc_polyline(HIC_Rendition const *nr, int count, HIC_DC_Point const *points)
	{
		HIC_Draw_DC_Polyline(nr, count, points);
	}


	/*!  Returns the name of the callback.  */       
	
	const char * GetName();

protected:   
	
	/*!   Returns the pointer to the user data that may have been provided when the callback was created.  */

	void * GetData();	
	
private:

	Draw_DC_Polyline_Impl * _impl;
};

#ifdef SWIG
%clear HIC_DC_Point const *points;
#endif

class Draw_DC_Cut_Line_Impl;

/*!
The Draw_DC_Cut_Line class is a base class used to define and set 
a callback at the "draw dc cut line" callback point during the HOOPS update 
cycle.  To use this class, derive your own child class from it and 
implement the method #draw_dc_cut_polyline. For more information on 
how to define and set callbacks as a C++/C/Java Developer, please see 
the <a href="../prog_guide/08_1_im_overview.html#8142_CallbacksForC++Developers">HOOPS/3dGS Programming Guide</a>.
*/

#ifdef SWIG
%typemap (imtype) HIC_DC_Point const *points "IntPtr"
%typemap (cstype) HIC_DC_Point const *points "IntPtr"
#endif
class HIC_API Draw_DC_Cut_Line
{
public:

	/*!
        Constructs a Draw_DC_Cut_Line Callback object.
          \param key Key to the segment on which the callback is set.
          \param name The name of the callback.
          \param data Optional pointer to user data that can then be accessed from the
			callback.
    */ 

	Draw_DC_Cut_Line(HC_KEY key, const char * name, void * data=0);
	
	/*! Deletes the callback object.  */       
	
	virtual ~Draw_DC_Cut_Line();

	/*!
        The function you implement for your callback.
        \param nr A pointer to a const HIC_Rendition
		\param count A integer variable
		\param points A pointer to a const HIC_DC_Point
	*/
	virtual void draw_dc_cut_line(HIC_Rendition const *nr, int count, HIC_DC_Point const *points) = 0;


	/*!  Returns the name of the callback.  */       
	
	const char * GetName();

protected:   
	
	/*!   Returns the pointer to the user data that may have been provided when the callback was created.  */

	void * GetData();	
	
private:

	Draw_DC_Cut_Line_Impl * _impl;
};

#ifdef SWIG
%clear HIC_DC_Point const *points;
#endif

class Draw_DC_Polymarker_Impl;

/*!
The Draw_DC_Polymarker class is a base class used to define and set 
a callback at the "draw dc polymarker" callback point during the HOOPS update 
cycle.  To use this class, derive your own child class from it and 
implement the method #draw_dc_polymarker. For more information on 
how to define and set callbacks as a C++/C/Java Developer, please see 
the <a href="../prog_guide/08_1_im_overview.html#8142_CallbacksForC++Developers">HOOPS/3dGS Programming Guide</a>.
*/

#ifdef SWIG
%typemap (imtype,directorinattributes="[MarshalAs(UnmanagedType.LPArray,SizeParamIndex=1)]") HIC_DC_Point const *points "HIC_DC_Point[]"
%typemap (imtype,directorinattributes="[MarshalAs(UnmanagedType.LPArray,SizeParamIndex=1)]") float const *rotations "float[]"
%typemap (imtype,directorinattributes="[MarshalAs(UnmanagedType.LPArray,SizeParamIndex=1)]") float const *size_fixups "float[]"
#endif
class HIC_API Draw_DC_Polymarker
{
public:

	/*!
        Constructs a Draw_DC_Polymarker Callback object.
          \param key Key to the segment on which the callback is set.
          \param name The name of the callback.
          \param data Optional pointer to user data that can then be accessed from the
			callback.
    */ 

	Draw_DC_Polymarker(HC_KEY key, const char * name, void * data=0);
	
	/*! Deletes the callback object.  */       
	
	virtual ~Draw_DC_Polymarker();

	/*!
        The function you implement for your callback.
        \param nr A pointer to a const HIC_Rendition
		\param points A pointer to a const HIC_DC_Point
		\param rotations A pointer to a const float
		\param size_fixups A pointer to a const float
	*/

	virtual void draw_dc_polymarker(HIC_Rendition const *nr, int count, HIC_DC_Point const *points, float const *rotations, float const *size_fixups)
	{
		HIC_Draw_DC_Polymarker(nr, count, points, rotations, size_fixups);
	}

	/*!  Returns the name of the callback.  */       
	
	const char * GetName();

protected:   
	
	/*!   Returns the pointer to the user data that may have been provided when the callback was created.  */

	void * GetData();	
	
private:

	Draw_DC_Polymarker_Impl * _impl;
};
#ifdef SWIG
%clear HIC_DC_Point const *points;
%clear float const *size_fixups;
%clear float const *rotations;
#endif

class Draw_DC_Polytriangle_Impl;

/*!
The Draw_DC_Polytriangle class is a base class used to define and set 
a callback at the "draw dc polytriangle" callback point during the HOOPS update 
cycle.  To use this class, derive your own child class from it and 
implement the method #draw_dc_polytriangle. For more information on 
how to define and set callbacks as a C++/C/Java Developer, please see 
the <a href="../prog_guide/08_1_im_overview.html#8142_CallbacksForC++Developers">HOOPS/3dGS Programming Guide</a>.
*/

class HIC_API Draw_DC_Polytriangle
{
public:

	/*!
        Constructs a Draw_DC_Polytriangle Callback object.
          \param key Key to the segment on which the callback is set.
          \param name The name of the callback.
          \param data Optional pointer to user data that can then be accessed from the
			callback.
    */ 

	Draw_DC_Polytriangle(HC_KEY key, const char * name, void * data=0);
	
	/*! Deletes the callback object.  */       
	
	virtual ~Draw_DC_Polytriangle();

	/*!
        The function you implement for your callback.
        \param nr A pointer to a const HIC_Rendition
		\param count A integer variable
		\param points A pointer to a const HIC_DC_Point
	*/

	virtual void draw_dc_polytriangle(HIC_Rendition const *nr, int count, HIC_DC_Point const *points)
	{
		HIC_Draw_DC_Polytriangle(nr, count, points);
	}

	/*!  Returns the name of the callback.  */       
	
	const char * GetName();

protected:   
	
	/*!   Returns the pointer to the user data that may have been provided when the callback was created.  */

	void * GetData();	
	
private:

	Draw_DC_Polytriangle_Impl * _impl;
};


class Draw_DC_Reshaded_Line_Impl;

/*!
The Draw_DC_Reshaded_Line class is a base class used to define and set 
a callback at the "draw dc reshaded line" callback point during the HOOPS update 
cycle.  To use this class, derive your own child class from it and 
implement the method #draw_dc_reshaded_line. For more information on 
how to define and set callbacks as a C++/C/Java Developer, please see 
the <a href="../prog_guide/08_1_im_overview.html#8142_CallbacksForC++Developers">HOOPS/3dGS Programming Guide</a>.
*/
#ifdef SWIG
%typemap (imtype,directorinattributes="[MarshalAs(UnmanagedType.LPArray,SizeConst=2)]") HIC_DC_Point const *points "HIC_DC_Point[]"
%typemap (imtype,directorinattributes="[MarshalAs(UnmanagedType.LPArray,SizeConst=2)]") HIC_RGBA const *colors "HIC_RGBA[]"
%typemap (imtype,directorinattributes="[MarshalAs(UnmanagedType.LPArray,SizeConst=2)]") HIC_Plane const *planes "HIC_Plane[]"
%typemap (imtype) HIC_Parameter const *parameters "IntPtr"
%typemap (cstype) HIC_Parameter const *parameters "IntPtr"
#endif
class HIC_API Draw_DC_Reshaded_Line
{
public:

	/*!
        Constructs a Draw_DC_Reshaded_Line Callback object.
          \param key Key to the segment on which the callback is set.
          \param name The name of the callback.
          \param data Optional pointer to user data that can then be accessed from the
			callback.
    */ 

	Draw_DC_Reshaded_Line(HC_KEY key, const char * name, void * data=0);
	
	/*! Deletes the callback object.  */       
	
	virtual ~Draw_DC_Reshaded_Line();

	
	/*!
        The function you implement for your callback.
        \param nr A pointer to a const HIC_Rendition
		\param points A pointer to a const HIC_DC_Point
		\param colors A pointer to a const HICRGBA
		\param planes A pointer to a const HIC_Plane
		\param parameters A pointer to a const HIC_Parameter
		\param param_width A integer variable
		\param param_flags A integer variable
	*/

	virtual void draw_dc_reshaded_line(HIC_Rendition const *nr, HIC_DC_Point const *points, HIC_RGBA const *colors, HIC_Plane const *planes, HIC_Parameter const *parameters, int param_width, HIC_Integer32 param_flags)
	{
		HIC_Draw_DC_Reshaded_Line(nr, points, colors, planes, parameters, param_width, param_flags);
	}

	/*!  Returns the name of the callback.  */       
	
	const char * GetName();

protected:   
	
	/*!   Returns the pointer to the user data that may have been provided when the callback was created.  */

	void * GetData();	
	
private:

	Draw_DC_Reshaded_Line_Impl * _impl;
};
#ifdef SWIG
%clear HIC_DC_Point const *points;
%clear HIC_RGBA const *colors;
%clear HIC_Plane const *planes;
%clear HIC_Parameter const *parameters;
#endif


class Draw_DC_Reshaded_Triangle_Impl;

/*!
The Draw_DC_Reshaded_Triangle class is a base class used to define and set 
a callback at the "draw dc reshaded triangle" callback point during the HOOPS update 
cycle.  To use this class, derive your own child class from it and 
implement the method #draw_dc_reshaded_triangle. For more information on 
how to define and set callbacks as a C++/C/Java Developer, please see 
the <a href="../prog_guide/08_1_im_overview.html#8142_CallbacksForC++Developers">HOOPS/3dGS Programming Guide</a>.
*/
#ifdef SWIG
%typemap (imtype,directorinattributes="[MarshalAs(UnmanagedType.LPArray,SizeConst=3)]") HIC_DC_Point const *points "HIC_DC_Point[]"
%typemap (imtype,directorinattributes="[MarshalAs(UnmanagedType.LPArray,SizeConst=3)]") HIC_RGBA const *colors "HIC_RGBA[]"
%typemap (imtype,directorinattributes="[MarshalAs(UnmanagedType.LPArray,SizeConst=3)]") HIC_Plane const *planes "HIC_Plane[]"
%typemap (imtype) HIC_Parameter const *parameters "IntPtr"
%typemap (cstype) HIC_Parameter const *parameters "IntPtr"
#endif
class HIC_API Draw_DC_Reshaded_Triangle
{
public:

	/*!
        Constructs a Draw_DC_Reshaded_Triangle Callback object.
          \param key Key to the segment on which the callback is set.
          \param name The name of the callback.
          \param data Optional pointer to user data that can then be accessed from the
			callback.
    */ 

	Draw_DC_Reshaded_Triangle(HC_KEY key, const char * name, void * data=0);
	
	/*! Deletes the callback object.  */       
	
	virtual ~Draw_DC_Reshaded_Triangle();

	/*!
        The function you implement for your callback.
        \param nr A pointer to a const HIC_Rendition
		\param points A pointer to a const HIC_DC_Point
		\param colors A pointer to a const HICRGBA
		\param planes A pointer to a const HIC_Plane
		\param parameters A pointer to a const HIC_Parameter
		\param param_width A integer variable
		\param param_flags A integer variable
	*/

	virtual void draw_dc_reshaded_triangle(HIC_Rendition const *nr, HIC_DC_Point const *points, HIC_RGBA const *colors, HIC_Plane const *planes, HIC_Parameter const *parameters, int param_width, HIC_Integer32 param_flags)
	{
		HIC_Draw_DC_Reshaded_Triangle(nr, points, colors, planes, parameters, param_width, param_flags);
	}

	/*!  Returns the name of the callback.  */       
	
	const char * GetName();

protected:   
	
	/*!   Returns the pointer to the user data that may have been provided when the callback was created.  */

	void * GetData();	
	
private:

	Draw_DC_Reshaded_Triangle_Impl * _impl;
};
#ifdef SWIG
%clear HIC_DC_Point const *points;
%clear HIC_RGBA const *colors;
%clear HIC_Plane const *planes;
%clear HIC_Parameter const *parameters;
#endif

class Draw_Text_Impl;

/*!
The Draw_Text class is a base class used to define and set 
a callback at the "draw dc text" callback point during the HOOPS update 
cycle.  To use this class, derive your own child class from it and 
implement the method #draw_text. For more information on 
how to define and set callbacks as a C++/C/Java Developer, please see 
the <a href="../prog_guide/08_1_im_overview.html#8142_CallbacksForC++Developers">HOOPS/3dGS Programming Guide</a>.
*/

class HIC_API Draw_Text
{
public:

	/*!
        Constructs a Draw_Text Callback object.
          \param key Key to the segment on which the callback is set.
          \param name The name of the callback.
          \param data Optional pointer to user data that can then be accessed from the
			callback.
    */ 

	Draw_Text(HC_KEY key, const char * name, void * data=0);
	
	/*! Deletes the callback object.  */       
	
	virtual ~Draw_Text();

	/*!
        The function you implement for your callback.
        \param nr A pointer to a const HIC_Rendition
		\param where A pointer to a const HIC_Point
		\param int A integer variable
		\param kstring A pointer to a const HIC_Karacter(unsigned short variable)
	*/

	virtual void draw_text(HIC_Rendition const *nr, HIC_Text_Info const *ti)
	{
		HIC_Draw_Text(nr, ti);
	}

	/*!  Returns the name of the callback.  */       
	
	const char * GetName();

protected:   
	
	/*!   Returns the pointer to the user data that may have been provided when the callback was created.  */

	void * GetData();	
	
private:

	Draw_Text_Impl * _impl;
};


class Draw_DC_Textured_Polyline_Impl;

/*!
The Draw_DC_Textured_Polyline class is a base class used to define and set 
a callback at the "draw dc textured polyline" callback point during the HOOPS update 
cycle.  To use this class, derive your own child class from it and 
implement the method #draw_dc_textured_polyline. For more information on 
how to define and set callbacks as a C++/C/Java Developer, please see 
the <a href="../prog_guide/08_1_im_overview.html#8142_CallbacksForC++Developers">HOOPS/3dGS Programming Guide</a>.
*/
#ifdef SWIG
%typemap (imtype) HIC_DC_Point const *points "IntPtr"
%typemap (imtype) HIC_RGBA const *colors "IntPtr"
%typemap (imtype) float const *parameters "IntPtr"
%typemap (imtype) HIC_Plane const *planes "IntPtr"
%typemap (imtype) HIC_DPlane const *dplanes "IntPtr"
%typemap (cstype) HIC_DC_Point const *points "IntPtr"
%typemap (cstype) HIC_RGBA const *colors "IntPtr"
%typemap (cstype) float const *parameters "IntPtr"
%typemap (cstype) HIC_Plane const *planes "IntPtr"
%typemap (cstype) HIC_DPlane const *dplanes "IntPtr"
#endif
class HIC_API Draw_DC_Textured_Polyline
{
public:

	/*!
        Constructs a Draw_DC_Textured_Polyline Callback object.
          \param key Key to the segment on which the callback is set.
          \param name The name of the callback.
          \param data Optional pointer to user data that can then be accessed from the
			callback.
    */ 

	Draw_DC_Textured_Polyline(HC_KEY key, const char * name, void * data=0);
	
	/*! Deletes the callback object.  */       
	
	virtual ~Draw_DC_Textured_Polyline();

	/*!
        The function you implement for your callback.
        \param nr A pointer to a const HIC_Rendition
		\param points A pointer to a const HIC_DC_Point
		\param count A integer variable
		\param colors A pointer to a const HICRGBA
		\param planes A pointer to a const HIC_Plane
		\param parameters A pointer to a const HIC_Parameter
		\param param_width A integer variable
		\param param_flags A integer variable
	*/

	virtual void draw_dc_textured_polyline(HIC_Rendition const *nr, int count, HIC_DC_Point const *points, HIC_RGBA const *colors, HIC_Plane const *planes, HIC_DPlane const *dplanes, float const *parameters, int param_width, HIC_Integer32 param_flags)
	{
		HIC_Draw_DC_Textured_Polyline(nr, count, points, colors, planes, dplanes, parameters, param_width, param_flags);
	}

	/*!  Returns the name of the callback.  */       
	
	const char * GetName();

protected:   
	
	/*!   Returns the pointer to the user data that may have been provided when the callback was created.  */

	void * GetData();	
	
private:

	Draw_DC_Textured_Polyline_Impl * _impl;
};
#ifdef SWIG
%clear HIC_DC_Point const *points;
%clear HIC_RGBA const *colors;
%clear float const *parameters;
%clear HIC_Plane const *planes;
%clear HIC_DPlane const *dplanes;
#endif

class Draw_DC_Textured_Polytriangle_Impl;

/*!
The Draw_DC_Textured_Polytriangle class is a base class used to define and set 
a callback at the "draw dc textured polytriangle" callback point during the HOOPS update 
cycle.  To use this class, derive your own child class from it and 
implement the method #draw_dc_textured_polytriangle. For more information on 
how to define and set callbacks as a C++/C/Java Developer, please see 
the <a href="../prog_guide/08_1_im_overview.html#8142_CallbacksForC++Developers">HOOPS/3dGS Programming Guide</a>.
*/
#ifdef SWIG
%typemap (imtype) HIC_DC_Point const *points "IntPtr"
%typemap (imtype) HIC_RGBA const *colors "IntPtr"
%typemap (imtype) float const *parameters "IntPtr"
%typemap (imtype) HIC_Plane const *planes "IntPtr"
%typemap (imtype) HIC_DPlane const *dplanes "IntPtr"
%typemap (cstype) HIC_DC_Point const *points "IntPtr"
%typemap (cstype) HIC_RGBA const *colors "IntPtr"
%typemap (cstype) float const *parameters "IntPtr"
%typemap (cstype) HIC_Plane const *planes "IntPtr"
%typemap (cstype) HIC_DPlane const *dplanes "IntPtr"
#endif
class HIC_API Draw_DC_Textured_Polytriangle
{
public:

	/*!
        Constructs a Draw_DC_Textured_Polytriangle Callback object.
          \param key Key to the segment on which the callback is set.
          \param name The name of the callback.
          \param data Optional pointer to user data that can then be accessed from the
			callback.
    */ 

	Draw_DC_Textured_Polytriangle(HC_KEY key, const char * name, void * data=0);
	
	/*! Deletes the callback object.  */       
	
	virtual ~Draw_DC_Textured_Polytriangle();

	/*!
        The function you implement for your callback.
        \param nr A pointer to a const HIC_Rendition
		\param points A pointer to a const HIC_DC_Point
		\param count A integer variable
		\param colors A pointer to a const HICRGBA
		\param planes A pointer to a const HIC_Plane
		\param parameters A pointer to a const HIC_Parameter
		\param param_width A integer variable
		\param param_flags A integer variable
	*/

	virtual void draw_dc_textured_polytriangle(HIC_Rendition const *nr, int count, HIC_DC_Point const *points, HIC_RGBA const *colors, HIC_Plane const *planes, HIC_DPlane const *dplanes, float const *parameters, int param_width, HIC_Integer32 param_flags)
	{
		HIC_Draw_DC_Textured_Polytriangle(nr, count, points, colors, planes, dplanes, parameters, param_width, param_flags);
	}

	/*!  Returns the name of the callback.  */       
	
	const char * GetName();

protected:   
	
	/*!   Returns the pointer to the user data that may have been provided when the callback was created.  */

	void * GetData();	
	
private:

	Draw_DC_Textured_Polytriangle_Impl * _impl;
};
#ifdef SWIG
%clear HIC_DC_Point const *points;
%clear HIC_RGBA const *colors;
%clear float const *parameters;
%clear HIC_Plane const *planes;
%clear HIC_DPlane const *dplanes;
#endif

class Draw_DC_Triangle_Impl;

/*!
The Draw_DC_Triangle class is a base class used to define and set 
a callback at the "draw dc triangle" callback point during the HOOPS update 
cycle.  To use this class, derive your own child class from it and 
implement the method #draw_dc_triangle. For more information on 
how to define and set callbacks as a C++/C/Java Developer, please see 
the <a href="../prog_guide/08_1_im_overview.html#8142_CallbacksForC++Developers">HOOPS/3dGS Programming Guide</a>.
*/
#ifdef SWIG
%typemap (imtype,directorinattributes="[MarshalAs(UnmanagedType.LPArray,SizeConst=3)]") HIC_DC_Point const *points "HIC_DC_Point[]"
#endif
class HIC_API Draw_DC_Triangle
{
public:

	/*!
        Constructs a Draw_DC_Triangle Callback object.
          \param key Key to the segment on which the callback is set.
          \param name The name of the callback.
          \param data Optional pointer to user data that can then be accessed from the
			callback.
    */ 

	Draw_DC_Triangle(HC_KEY key, const char * name, void * data=0);
	
	/*! Deletes the callback object.  */       
	
	virtual ~Draw_DC_Triangle();

	/*!
        The function you implement for your callback.
        \param nr A pointer to a const HIC_Rendition
		\param points A pointer to a const HIC_DC_Point
	*/

	virtual void draw_dc_triangle(HIC_Rendition const *nr, HIC_DC_Point const *points)
	{
		HIC_Draw_DC_Triangle(nr, points);
	}

	/*!  Returns the name of the callback.  */       
	
	const char * GetName();

protected:   
	
	/*!   Returns the pointer to the user data that may have been provided when the callback was created.  */

	void * GetData();	
	
private:

	Draw_DC_Triangle_Impl * _impl;
};
#ifdef SWIG
%clear HIC_DC_Point const *points;
#endif

class Draw_Segment_Impl;

/*!
The Draw_Segment class is a base class used to define and set 
a callback at the "draw segment" callback point during the HOOPS update 
cycle.  To use this class, derive your own child class from it and 
implement the method #draw_segment. For more information on 
how to define and set callbacks as a C++/C/Java Developer, please see 
the <a href="../prog_guide/08_1_im_overview.html#8142_CallbacksForC++Developers">HOOPS/3dGS Programming Guide</a>.
*/

class HIC_API Draw_Segment
{
public:

	/*!
        Constructs a Draw_Segment Callback object.
          \param key Key to the segment on which the callback is set.
          \param name The name of the callback.
          \param data Optional pointer to user data that can then be accessed from the
			callback.
    */ 

	Draw_Segment(HC_KEY key, const char * name, void * data=0);
	
	/*! Deletes the callback object.  */       
	
	virtual ~Draw_Segment();

	/*!
        The function you implement for your callback.
        \param nr A pointer to a const HIC_Rendition
		\param si A pointer to a const HIC_Segment_Info
	*/

	virtual void draw_segment(HIC_Rendition const *nr, HIC_Segment_Info const *si)
	{
		HIC_Draw_Segment(nr, si);
	}

	/*!  Returns the name of the callback.  */       
	
	const char * GetName();

protected:   
	
	/*!   Returns the pointer to the user data that may have been provided when the callback was created.  */

	void * GetData();	
	
private:

	Draw_Segment_Impl * _impl;
};


class Draw_Window_Impl;

/*!
The Draw_Window class is a base class used to define and set 
a callback at the "draw window" callback point during the HOOPS update 
cycle.  To use this class, derive your own child class from it and 
implement the method #draw_window. For more information on 
how to define and set callbacks as a C++/C/Java Developer, please see 
the <a href="../prog_guide/08_1_im_overview.html#8142_CallbacksForC++Developers">HOOPS/3dGS Programming Guide</a>.
*/

class HIC_API Draw_Window
{
public:

	/*!
        Constructs a Draw_Window Callback object.
          \param key Key to the segment on which the callback is set.
          \param name The name of the callback.
          \param data Optional pointer to user data that can then be accessed from the
			callback.
    */ 

	Draw_Window(HC_KEY key, const char * name, void * data=0);
	
	/*! Deletes the callback object.  */       
	
	virtual ~Draw_Window();

	/*!
        The function you implement for your callback.
        \param nr A pointer to a const HIC_Rendition
		\param extent A pointer to a const HIC_Int_Rectangle
	*/

	virtual void draw_window(HIC_Rendition const *nr, HIC_Int_Rectangle const *extent)
	{
		HIC_Draw_Window(nr, extent);
	}

	/*!  Returns the name of the callback.  */       
	
	const char * GetName();

protected:   
	
	/*!   Returns the pointer to the user data that may have been provided when the callback was created.  */

	void * GetData();	
	
private:

	Draw_Window_Impl * _impl;
};


class Draw_Window_Frame_Impl;

/*!
The Draw_Window_Frame class is a base class used to define and set 
a callback at the "draw window frame" callback point during the HOOPS update 
cycle.  To use this class, derive your own child class from it and 
implement the method #draw_window_frame. For more information on 
how to define and set callbacks as a C++/C/Java Developer, please see 
the <a href="../prog_guide/08_1_im_overview.html#8142_CallbacksForC++Developers">HOOPS/3dGS Programming Guide</a>.
*/

class HIC_API Draw_Window_Frame
{
public:

	/*!
        Constructs a Draw_Window_Frame Callback object.
          \param key Key to the segment on which the callback is set.
          \param name The name of the callback.
          \param data Optional pointer to user data that can then be accessed from the
			callback.
    */ 

	Draw_Window_Frame(HC_KEY key, const char * name, void * data=0);
	
	/*! Deletes the callback object.  */      
	
	virtual ~Draw_Window_Frame();

	/*!
        The function you implement for your callback.
        \param nr A pointer to a const HIC_Rendition
		\param extent A pointer to a const HIC_Int_Rectangle
		\param frame A pointer to a const HIC_Int_Rectangle
		\param calculate_only A bool variable
	*/

	virtual void draw_window_frame(HIC_Rendition const *nr, HIC_Int_Rectangle const *extent, HIC_Int_Rectangle *frame, bool calculate_only)
	{
		HIC_Draw_Window_Frame(nr, extent, frame, calculate_only);
	}

	/*!  Returns the name of the callback.  */       
	
	const char * GetName();

protected:   
	
	/*!   Returns the pointer to the user data that may have been provided when the callback was created.  */

	void * GetData();	
	
private:

	Draw_Window_Frame_Impl * _impl;
};


class Finish_Picture_Impl;

/*!
The Finish_Picture class is a base class used to define and set 
a callback at the "finish picture" callback point during the HOOPS update 
cycle.  To use this class, derive your own child class from it and 
implement the method #finish_picture. For more information on 
how to define and set callbacks as a C++/C/Java Developer, please see 
the <a href="../prog_guide/08_1_im_overview.html#8142_CallbacksForC++Developers">HOOPS/3dGS Programming Guide</a>.
*/

class HIC_API Finish_Picture
{
public:

	/*!
        Constructs a Finish_Picture Callback object.
          \param key Key to the segment on which the callback is set.
          \param name The name of the callback.
          \param data Optional pointer to user data that can then be accessed from the
			callback.
    */ 

	Finish_Picture(HC_KEY key, const char * name, void * data=0);
	
	/*! Deletes the callback object.  */      
	
	virtual ~Finish_Picture();

	/*!
        The function you implement for your callback.
        \param nr A pointer to a const HIC_Rendition
		\param swap_buffers A bool variable
	*/


	virtual void finish_picture(HIC_Rendition const *nr, bool swap_buffers)
	{
		HIC_Finish_Picture(nr, swap_buffers);
	}

	/*!  Returns the name of the callback.  */       
	
	const char * GetName();

protected:   
	
	/*!   Returns the pointer to the user data that may have been provided when the callback was created.  */

	void * GetData();	
	
private:

	Finish_Picture_Impl * _impl;
};

class Exit_Update_Impl;

/*!
The Exit_Update class is a base class used to define and set 
a callback at the "finish picture" callback point during the HOOPS update 
cycle.  To use this class, derive your own child class from it and 
implement the method #Exit_Update. For more information on 
how to define and set callbacks as a C++/C/Java Developer, please see 
the <a href="../prog_guide/08_1_im_overview.html#8142_CallbacksForC++Developers">HOOPS/3dGS Programming Guide</a>.
*/

class HIC_API Exit_Update
{
public:

	/*!
        Constructs a Exit_Update Callback object.
          \param key Key to the segment on which the callback is set.
          \param name The name of the callback.
          \param data Optional pointer to user data that can then be accessed from the
			callback.
    */ 

	Exit_Update(HC_KEY key, const char * name, void * data=0);
	
	/*! Deletes the callback object.  */      
	
	virtual ~Exit_Update();

	/*!
        The function you implement for your callback.
        \param nr A pointer to a const HIC_Rendition
		\param swap_buffers A bool variable
	*/


	virtual void exit_update(HIC_Rendition const *nr)
	{
		HIC_Exit_Update(nr);
	}

	/*!  Returns the name of the callback.  */       
	
	const char * GetName();

protected:   
	
	/*!   Returns the pointer to the user data that may have been provided when the callback was created.  */

	void * GetData();	
	
private:

	Exit_Update_Impl * _impl;
};



class Init_Picture_Impl;

/*!
The Init_Picture class is a base class used to define and set 
a callback at the "init picture" callback point during the HOOPS update 
cycle.  To use this class, derive your own child class from it and 
implement the method #init_picture. For more information on 
how to define and set callbacks as a C++/C/Java Developer, please see 
the <a href="../prog_guide/08_1_im_overview.html#8142_CallbacksForC++Developers">HOOPS/3dGS Programming Guide</a>.
*/

class HIC_API Init_Picture
{
public:

	/*!
        Constructs a Init_Picture Callback object.
          \param key Key to the segment on which the callback is set.
          \param name The name of the callback.
          \param data Optional pointer to user data that can then be accessed from the
			callback.
    */ 

	Init_Picture(HC_KEY key, const char * name, void * data=0);
	
	/*! Deletes the callback object.  */      
	
	virtual ~Init_Picture();

	/*!
        The function you implement for your callback.
        \param nr A pointer to a const HIC_Rendition
		\param swap_buffers A bool variable
	*/
	virtual void init_picture(HIC_Rendition const *nr)
	{
		HIC_Init_Picture(nr);
	}

	/*!  Returns the name of the callback.  */       
	
	const char * GetName();

protected:   
	
	/*!   Returns the pointer to the user data that may have been provided when the callback was created.  */

	void * GetData();	
	
private:

	Init_Picture_Impl * _impl;
};


class Select_Geometry_Impl;

/*!
The Select_Geometry class is a base class used to define and set 
a callback at the "select geometry" callback point during the HOOPS update 
cycle.  To use this class, derive your own child class from it and 
implement the method #select_geometry. For more information on 
how to define and set callbacks as a C++/C/Java Developer, please see 
the <a href="../prog_guide/08_1_im_overview.html#8142_CallbacksForC++Developers">HOOPS/3dGS Programming Guide</a>.
*/

class HIC_API Select_Geometry
{
public:

	/*!
        Constructs a Select_Geometry Callback object.
          \param key Key to the segment on which the callback is set.
          \param name The name of the callback.
          \param data Optional pointer to user data that can then be accessed from the
			callback.
    */ 

	Select_Geometry(HC_KEY key, const char * name, void * data=0);
	
	/*! Deletes the callback object.  */       
	
	virtual ~Select_Geometry();

	/*!
        The function you implement for your callback.
        \param nr A pointer to a const HIC_Rendition
		\param geo A pointer to a const HIC_Geometry
	*/


	virtual void select_geometry(HIC_Rendition const *nr, HIC_Geometry const *geo)
	{
		HIC_Select_Geometry(nr, geo);
	}

	/*!  Returns the name of the callback.  */       
	
	const char * GetName();

protected:   
	
	/*!   Returns the pointer to the user data that may have been provided when the callback was created.  */

	void * GetData();	
	
private:

	Select_Geometry_Impl * _impl;
};


class Select_Segment_Impl;

/*!
The Select_Segment class is a base class used to define and set 
a callback at the "select segment" callback point during the HOOPS update 
cycle.  To use this class, derive your own child class from it and 
implement the method #select_segment. For more information on 
how to define and set callbacks as a C++/C/Java Developer, please see 
the <a href="../prog_guide/08_1_im_overview.html#8142_CallbacksForC++Developers">HOOPS/3dGS Programming Guide</a>.
*/

class HIC_API Select_Segment
{
public:

	/*!
        Constructs a Select_Segment Callback object.
          \param key Key to the segment on which the callback is set.
          \param name The name of the callback.
          \param data Optional pointer to user data that can then be accessed from the
			callback.
    */ 

	Select_Segment(HC_KEY key, const char * name, void * data=0);
	
	/*! Deletes the callback object.  */       
	
	virtual ~Select_Segment();

	/*!
        The function you implement for your callback.
        \param nr A pointer to a const HIC_Rendition
		\param si A pointer to a const HIC_Segment_Info
	*/

	virtual void select_segment(HIC_Rendition const *nr, HIC_Segment_Info const *si)
	{
		HIC_Select_Segment(nr, si);
	}

	/*!  Returns the name of the callback.  */       
	
	const char * GetName();

protected:   
	
	/*!   Returns the pointer to the user data that may have been provided when the callback was created.  */

	void * GetData();	
	
private:

	Select_Segment_Impl * _impl;

};

class Select_Window_Impl;

/*!
The Select_Window class is a base class used to define and set 
a callback at the "select window" callback point during the HOOPS update 
cycle.  To use this class, derive your own child class from it and 
implement the method #select_window. For more information on 
how to define and set callbacks as a C++/C/Java Developer, please see 
the <a href="../prog_guide/08_1_im_overview.html#8142_CallbacksForC++Developers">HOOPS/3dGS Programming Guide</a>.
*/

class HIC_API Select_Window
{
public:

	/*!
        Constructs a Select_Window Callback object.
          \param key Key to the segment on which the callback is set.
          \param name The name of the callback.
          \param data Optional pointer to user data that can then be accessed from the
			callback.
    */ 

	Select_Window(HC_KEY key, const char * name, void * data=0);
	
	/*! Deletes the callback object.  */       
	
	virtual ~Select_Window();

	/*!
        The function you implement for your callback.
        \param nr A pointer to a const HIC_Rendition
		\param wi A pointer to a const HIC_Geometry
	*/

	virtual void select_window(HIC_Rendition const *nr, HIC_Geometry *wi)
	{
		HIC_Select_Window(nr, wi);
	}

	/*!  Returns the name of the callback.  */       
	
	const char * GetName();

protected:   
	
	/*!   Returns the pointer to the user data that may have been provided when the callback was created.  */

	void * GetData();	
	
private:

	Select_Window_Impl * _impl;
};

class Draw_3D_Isoline_Impl;

/*!
The Draw_3D_Isoline class is a base class used to define and set 
a callback at the "draw 3d isoline" callback point during the HOOPS update 
cycle.  To use this class, derive your own child class from it and 
implement the method #draw_3d_isoline. For more information on 
how to define and set callbacks as a C++/C/Java Developer, please see 
the <a href="../prog_guide/08_1_im_overview.html#8142_CallbacksForC++Developers">HOOPS/3dGS Programming Guide</a>.
*/

class HIC_API Draw_3D_Isoline
{
public:

	/*!
        Constructs a Draw_3D_Isoline Callback object.
          \param key Key to the segment on which the callback is set.
          \param name The name of the callback.
          \param data Optional pointer to user data that can then be accessed from the
			callback.
    */ 

	Draw_3D_Isoline(HC_KEY key, const char * name, void * data=0);
	
	/*! Deletes the callback object.  */       
	
	virtual ~Draw_3D_Isoline();

	/*!
        The function you implement for your callback.
        \param nr A pointer to a const HIC_Rendition
		\param pe A pointer to a const HIC_Polyedge
	*/

	virtual void draw_3d_isoline(HIC_Rendition const*nr, HIC_Polyedge const*pe)=0;  //DNE

	/*!  Returns the name of the callback.  */       
	
	const char * GetName();

protected:   
	
	/*!   Returns the pointer to the user data that may have been provided when the callback was created.  */

	void * GetData();	
	
private:

	Draw_3D_Isoline_Impl * _impl;
};



class HLR_Hidden_Polyline_Impl;

/*!
The HLR_Hidden_Polyline class is a base class used to define and set 
a callback at the "hlr hidden polyline" callback point during the HOOPS update 
cycle.  To use this class, derive your own child class from it and 
implement the method #hlr_hidden_polyline. For more information on 
how to define and set callbacks as a C++/C/Java Developer, please see 
the <a href="../prog_guide/08_1_im_overview.html#8142_CallbacksForC++Developers">HOOPS/3dGS Programming Guide</a>.
*/

class HIC_API HLR_Hidden_Polyline
{
public:

	/*!
        Constructs a HLR_Hidden_Polyline Callback object.
          \param key Key to the segment on which the callback is set.
          \param name The name of the callback.
          \param data Optional pointer to user data that can then be accessed from the
			callback.
    */ 

	HLR_Hidden_Polyline(HC_KEY key, const char * name, void * data=0);
	
	/*! Deletes the callback object.  */       
	
	virtual ~HLR_Hidden_Polyline();

	/*!
        The function you implement for your callback.
        \param nr A pointer to a const HIC_Rendition
		\param count A integer variable
		\param points A pointer to a const HIC_DC_Point
	*/

	virtual void hlr_hidden_polyline(HIC_Rendition const *nr, int count, HIC_DC_Point const*points)=0; //DNE

	/*!  Returns the name of the callback.  */       
	
	const char * GetName();

protected:   
	
	/*!   Returns the pointer to the user data that may have been provided when the callback was created.  */

	void * GetData();	
	
private:

	HLR_Hidden_Polyline_Impl * _impl;
};

class HLR_Polyline_Impl;

/*!
The HLR_Polyline class is a base class used to define and set 
a callback at the "hlr polyline" callback point during the HOOPS update 
cycle.  To use this class, derive your own child class from it and 
implement the method #hlr_polyline. For more information on 
how to define and set callbacks as a C++/C/Java Developer, please see 
the <a href="../prog_guide/08_1_im_overview.html#8142_CallbacksForC++Developers">HOOPS/3dGS Programming Guide</a>.
*/

class HIC_API HLR_Polyline
{
public:

	/*!
        Constructs a HLR_Polyline Callback object.
          \param key Key to the segment on which the callback is set.
          \param name The name of the callback.
          \param data Optional pointer to user data that can then be accessed from the
			callback.
    */ 

	HLR_Polyline(HC_KEY key, const char * name, void * data=0);
	
	/*! Deletes the callback object.  */       
	
	virtual ~HLR_Polyline();

	/*!
        The function you implement for your callback.
        \param nr A pointer to a const HIC_Rendition
		\param count A integer variable
		\param points A pointer to a const HIC_DC_Point
	*/

	virtual void hlr_polyline(HIC_Rendition const *nr, int count, HIC_DC_Point const*points)=0; //DNE

	/*!  Returns the name of the callback.  */       
	
	const char * GetName();

protected:   
	
	/*!   Returns the pointer to the user data that may have been provided when the callback was created.  */

	void * GetData();	
	
private:

	HLR_Polyline_Impl * _impl;
};

class Draw_Segment_Tree_Impl;

/*!
The Draw_Segment_Tree class is a base class used to define and set 
a callback at the "draw segment tree" callback point during the HOOPS update 
cycle.  To use this class, derive your own child class from it and 
implement the method #draw_segment_tree. For more information on 
how to define and set callbacks as a C++/C/Java Developer, please see 
the <a href="../prog_guide/08_1_im_overview.html#8142_CallbacksForC++Developers">HOOPS/3dGS Programming Guide</a>.
*/

class HIC_API Draw_Segment_Tree
{
public:

	/*!
        Constructs a Draw_Segment_Tree Callback object.
          \param key Key to the segment on which the callback is set.
          \param name The name of the callback.
          \param data Optional pointer to user data that can then be accessed from the
			callback.
    */ 

	Draw_Segment_Tree(HC_KEY key, const char * name, void * data=0);
	
	/*! Deletes the callback object.  */       
	
	virtual ~Draw_Segment_Tree();

	/*!
        The function you implement for your callback.
        \param nr A pointer to a const HIC_Rendition
		\param si A pointer to a const HIC_Segment_Info
	*/

	virtual void draw_segment_tree(HIC_Rendition const*nr, HIC_Segment_Info const *si)=0; //DNE

	/*!  Returns the name of the callback.  */       
	
	const char * GetName();

protected:   
	/*!   Returns the pointer to the user data that may have been provided when the callback was created.  */

	void * GetData();	
	
private:

	Draw_Segment_Tree_Impl * _impl;
};


class Draw_3D_Infinite_Line_Impl;

/*!
The Draw_3D_Infinite_Line class is a base class used to define and set 
a callback at the "draw 3d infinite line" callback point during the HOOPS update 
cycle.  To use this class, derive your own child class from it and 
implement the method #draw_3d_infinite_line. For more information on 
how to define and set callbacks as a C++/C/Java Developer, please see 
the <a href="../prog_guide/08_1_im_overview.html#8142_CallbacksForC++Developers">HOOPS/3dGS Programming Guide</a>.
*/

class HIC_API Draw_3D_Infinite_Line
{
public:

	/*!
        Constructs a Draw_3D_Infinite_Line Callback object.
          \param key Key to the segment on which the callback is set.
          \param name The name of the callback.
          \param data Optional pointer to user data that can then be accessed from the
			callback.
    */ 

	Draw_3D_Infinite_Line(HC_KEY key, const char * name, void * data=0);
	
	/*! Deletes the callback object.  */       
	
	virtual ~Draw_3D_Infinite_Line();

	/*!
        The function you implement for your callback.
        \param nr A pointer to a const HIC_Rendition
		\param il A pointer to a const HIC_Polyline
	*/

	virtual void draw_3d_infinite_line(HIC_Rendition const*nr, HIC_Polyline const *il)
	{
		HIC_Draw_3D_Infinite_Line(nr, il);
	}

	/*!  Returns the name of the callback.  */       
	
	const char * GetName();

protected:   
	/*!   Returns the pointer to the user data that may have been provided when the callback was created.  */

	void * GetData();	
	
private:

	Draw_3D_Infinite_Line_Impl * _impl;
};


class Draw_DC_Edge_Impl;

/*!
The Draw_DC_Edge class is a base class used to define and set 
a callback at the "draw dc edge" callback point during the HOOPS update 
cycle.  To use this class, derive your own child class from it and 
implement the method #draw_dc_edge. For more information on 
how to define and set callbacks as a C++/C/Java Developer, please see 
the <a href="../prog_guide/08_1_im_overview.html#8142_CallbacksForC++Developers">HOOPS/3dGS Programming Guide</a>.
*/

class HIC_API Draw_DC_Edge
{
public:

	/*!
        Constructs a Draw_DC_Edge Callback object.
          \param key Key to the segment on which the callback is set.
          \param name The name of the callback.
          \param data Optional pointer to user data that can then be accessed from the
			callback.
    */ 

	Draw_DC_Edge(HC_KEY key, const char * name, void * data=0);
	
	/*! Deletes the callback object.  */       
	
	virtual ~Draw_DC_Edge();

	/*!
        The function you implement for your callback.
        \param nr A pointer to a const HIC_Rendition
		\param count A integer variable
		\param points A pointer to a const HIC_DC_Point
	*/

	virtual void draw_dc_edge(HIC_Rendition const * nr, int count, HIC_DC_Point const* points)=0; //DNE

	/*!  Returns the name of the callback.  */       
	
	const char * GetName();

protected:   

	/*!   Returns the pointer to the user data that may have been provided when the callback was created.  */

	void * GetData();	
	
private:

	Draw_DC_Edge_Impl * _impl;
};

/* Testing DC_Gouraud_Polytriangle */

#endif







