// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

/*! \file hic.
 *
 * Description of hic.h TODO
 */


/*** C++ STYLE COMMENTS PROHIBITED IN THIS FILE ***/
/*** MULTI-LINE FUNCTION SIGNATURES PROHIBITED IN THIS FILE ***/



#ifndef HIC_DEFINED

#include "hc.h"
#include "hversion.h"
#include "patterns.h"
#include "hic_types.h"

#ifdef _MSC_VER
#	ifndef HOOPS_STATIC_LIB
#		if defined (BUILDING_HOOPS_INTERNALS)
#			define HIC_API __declspec(dllexport)
#		else
#			define HIC_API __declspec(dllimport)
#		endif
#	else
#		define HIC_API
#	endif
# elif defined(LINUX_SYSTEM) && !defined(HOOPS_STATIC_LIB)
#   define HIC_API __attribute__ ((visibility ("default")))
#else
#		define HIC_API
#endif

/*!
  \addtogroup Abort
  @{
*/

/*!
 * \param nr A pointer to a const HIC_Rendition.
 *
 * This function interrupts tree traversal and ends the update.
 */
HC_EXTERNAL HIC_API void HIC_Abort_Update(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 *
 * This function interrupts tree traversal and ends the selection.
 */
HC_EXTERNAL HIC_API void HIC_Abort_Selection(HIC_Rendition const *nr);
/*!
 * \param nr A pointer to a const HIC_Rendition.
 *
 * This function checks for conditions that would cause the update to exit.
 */
HC_EXTERNAL HIC_API void HIC_Exit_Update(HIC_Rendition const *nr);


/** @} */ /* end of Abort */


/*!
  \addtogroup Occlusion_Query
  @{
*/

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param disable_write A boolean.
 * \return A void pointer.
 *
 * This function begins an occlusion query.
 */
HC_EXTERNAL HIC_API void * HIC_Begin_Occlusion_Query(HIC_Rendition const *nr, bool disable_write);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param id A void pointer.
 *
 * This function ends an occlusion query.
 */
HC_EXTERNAL HIC_API void HIC_End_Occlusion_Query(HIC_Rendition const *nr, void * id);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param id A void pointer.
 * \return An int.
 *
 * This function returns the occlusion query.
 */
HC_EXTERNAL HIC_API int HIC_Get_Occlusion_Query(HIC_Rendition const *nr, void * id);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param id A void pointer.
 *
 * This function deletes the occlusion query.
 */
HC_EXTERNAL HIC_API void HIC_Delete_Occlusion_Query(HIC_Rendition const *nr, void * id);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A boolean.
 *
 * This function returns a boolean which is true if the occlusion query is available and false if the occlusion query is not available.
 */
HC_EXTERNAL HIC_API bool HIC_Occlusion_Query_Available(HIC_Rendition const *nr);
/** @} */ /* end of Occlusion_Query */


/*!
  \addtogroup Cutting_Plane
  @{
*/
/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param pln A pointer to a HIC_Plane.
 *
 * This function adds a cutting plane.
 */
HC_EXTERNAL HIC_API void  HIC_Add_Cutting_Plane(HIC_Rendition const *nr, HIC_Plane const *pln);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 *
 * This function removes all cutting planes.
 */
HC_EXTERNAL HIC_API void  HIC_Remove_Cutting_Planes(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 *
 * This function clears all cutting planes.
 */
HC_EXTERNAL HIC_API void HIC_Clear_Cutting_Planes(HIC_Rendition const *nr);

/** @} */ /* end of Cutting_Plane */



/*!
  \addtogroup Regions
  @{
*/

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A boolean.
 *
 * This function returns a boolean which is true if you can save and restore regions and false if you cannot save and restore regions.
 */
HC_EXTERNAL HIC_API bool HIC_Can_Save_And_Restore_Regions(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param area A pointer to a const HIC_Int_Rectangle.
 * \param image_buffer A pointer to a void pointer.
 * \param z_buffer A pointer to a void pointer.
 *
 * This function creates a region.
 */
HC_EXTERNAL HIC_API void HIC_Create_Region(HIC_Rendition const *nr, HIC_Int_Rectangle const *area, void **image_buffer, void **z_buffer);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param area A pointer to a const HIC_Int_Rectangle.
 * \param image_buffer A void pointer.
 * \param z_buffer A void pointer.
 *
 * This function saves a region.
 */
HC_EXTERNAL HIC_API void HIC_Save_Region(HIC_Rendition const *nr, HIC_Int_Rectangle const *area, void *image_buffer, void *z_buffer);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param area A pointer to a const HIC_Int_Rectangle.
 * \param image_buffer A void pointer.
 * \param z_buffer A void pointer.
 *
 * This function restores a region.
 */
HC_EXTERNAL HIC_API void HIC_Restore_Region(HIC_Rendition const *nr, HIC_Int_Rectangle const *area, void *image_buffer, void *z_buffer);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param image_buffer A void pointer.
 * \param z_buffer A void pointer.
 *
 * This function destroys a region.
 */
HC_EXTERNAL HIC_API void HIC_Destroy_Region(HIC_Rendition const *nr, void *image_buffer, void *z_buffer);
/** @} */ /* end of region */


/*!
  \addtogroup Snapshot
  @{
*/

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param width An int.
 * \param height An int.
 * \param data A pointer to an unsigned char.
 * \return A boolean, true if snapshot was taken, otherwise false.
 *
 * This function takes a snapshot whose dimensions are the passed in width and height parameters.
 */
HC_EXTERNAL HIC_API bool HIC_Snapshot(HIC_Rendition const *nr, int width, int height, unsigned char *data);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A boolean.
 *
 * This function returns a boolean which is true if the driver supports snapshotting and false otherwise.
 */
HC_EXTERNAL HIC_API bool HIC_Can_Snapshot(HIC_Rendition const *nr);


HC_EXTERNAL HIC_API HC_POINTER_SIZED_INT HIC_Driver_Snapshot_Texture(HIC_Rendition const *nr, HC_POINTER_SIZED_INT id);
HC_EXTERNAL HIC_API HC_POINTER_SIZED_INT HIC_Driver_Snapshot_Depth_Texture(HIC_Rendition const *nr, HC_POINTER_SIZED_INT id);

/*! @} */ /* end of snapshot */


/*!
  \addtogroup Geometry_Interesting
  @{
*/
/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param geo A pointer to a const HIC_Marker.
 *
 * This function sets a geometry to interesting.
 */
HC_EXTERNAL HIC_API void HIC_Set_Geometry_Interesting(HIC_Rendition const *nr, HIC_Marker const *geo);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param geo A pointer to a const HIC_Marker.
 *
 * This function sets geometry to not interesting.
 */
HC_EXTERNAL HIC_API void HIC_UnSet_Geometry_Interesting(HIC_Rendition const *nr, HIC_Marker const *geo);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param geo A pointer to a const HIC_Marker.
 * \return A boolean, true if geometery is interesting, otherwise false.
 *
 * This function returns a boolean, true if geometry is interesting, false if geometry is not interesting.
 */
HC_EXTERNAL HIC_API bool HIC_Check_Geometry_Interesting(HIC_Rendition const *nr, HIC_Marker const *geo);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 *
 * This function sets current geometry to interesting.
 */
HC_EXTERNAL HIC_API void HIC_Set_Current_Geometry_Interesting(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 *
 * This function sets current geometry to not interesting.
 */
HC_EXTERNAL HIC_API void HIC_UnSet_Current_Geometry_Interesting(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A boolean, true if current geometry is interesting, otherwise false.
 *
 * This function returns a boolean, true if current geometry is interesting, false if current geometry is not interesting.
 */
HC_EXTERNAL HIC_API bool HIC_Check_Current_Geometry_Interesting(HIC_Rendition const *nr);

/** @} */ /* end of geo_interesting */


/*!
  \addtogroup Segment_Interesting
  @{
*/

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param si A pointer to a const HIC_Segment_Info.
 *
 * This function sets a segment to interesting.	
 */
HC_EXTERNAL HIC_API void HIC_Set_Segment_Interesting(HIC_Rendition const *nr, HIC_Segment_Info const *si);
/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param si A pointer to a const HIC_Segment_Info.
 *
 * This function sets a segment to not interesting.	
 */
HC_EXTERNAL HIC_API void HIC_UnSet_Segment_Interesting(HIC_Rendition const *nr, HIC_Segment_Info const *si);
/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param si A pointer to a const HIC_Segment_Info.
 * \return A boolean, true if segment is interesting, otherwise false.
 *
 * This function returns a boolean, true if segment is interesting, false if segment is not interesting.
 */
HC_EXTERNAL HIC_API bool HIC_Check_Segment_Interesting(HIC_Rendition const *nr, HIC_Segment_Info const *si);

/** @} */ /* end of interesting */


/*!
  \addtogroup Z_Buffer
  @{
*/

/*!
 * \param nr A pointer to a const HIC_Rendition.
 *
 * This function clears the Z-buffer.
 */
HC_EXTERNAL HIC_API void HIC_Clear_Z_Buffer(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A boolean, true if Z-buffering is in effect, otherwise false.
 *
 * This function returns a boolean, true if Z-buffering is in effect, false if Z-buffering is not in effect.
 */
HC_EXTERNAL HIC_API bool HIC_Show_Z_Buffering(HIC_Rendition const *nr);

/** @} */ /* end of buffering */

/*!
  \addtogroup Compute_Transformed_Text_Position
  @{
*/

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param ti A pointer to a const HIC_Text_Info.
 * \param pt A pointer to a HIC_DC_Point.
 * \return A pointer to a const HIC_Point.
 *
 * This function computes transformed text position.
 */
HC_EXTERNAL HIC_API HIC_Point const * HIC_Compute_Transformed_Text_Position(HIC_Rendition const *nr, HIC_Text_Info const *ti, HIC_DC_Point *pt);

/** @} */ /* end of compute_text */

/* segment info */

/*!
  \addtogroup Segment_Info
  @{
*/
/*!
 * \param si A pointer to a const HIC_Segment_Info.
 * \return A const pointer to a HIC_Segment_Info.
 *
 * This function copies segment information.
 */
HC_EXTERNAL HIC_API HIC_Segment_Info const * HIC_Copy_Segment_Info(HIC_Segment_Info const *si);

/*!
 * \param si A pointer to a const HIC_Segment_Info.
 *
 * This function frees memory associated with segment information.
 */
HC_EXTERNAL HIC_API void HIC_Free_Segment_Info(HIC_Segment_Info const *si);

/** @} */ /* end of seg_info */

/*!
  \addtogroup Shadows
  @{
*/
/*!
 * \param nr A pointer to a const HIC_Rendition.
 *
 * This function disables shadows.
 */
HC_EXTERNAL HIC_API void  HIC_Disable_Shadows(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 *
 * This function enables shadows.
 */
HC_EXTERNAL HIC_API void  HIC_Enable_Shadows(HIC_Rendition const *nr);

/** @} */ /* end of shadow */


/* 3D drawing */

/*!
  \addtogroup Draw_3d_Geometry
  @{
*/

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param geo A pointer to a const HIC_Geometry.
 *
 * This function draws the geometry.
 */
HC_EXTERNAL HIC_API void HIC_Draw_3D_Geometry(HIC_Rendition const *nr, HIC_Geometry const *	geo);
	
/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param marker A pointer to a const HIC_Marker.
 *
 * This function draws the marker.
 */
HC_EXTERNAL HIC_API void HIC_Draw_3D_Marker(HIC_Rendition const *nr, HIC_Marker const *marker);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param polyline A pointer to a const HIC_Polyline.
 *
 * This function draws the polyline.
 */
HC_EXTERNAL HIC_API void HIC_Draw_3D_Polyline(HIC_Rendition const *nr, HIC_Polyline const *polyline);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param polyline A pointer to a const HIC_Polyline.
 *
 * This function draws the infinite line.
 */
HC_EXTERNAL HIC_API void HIC_Draw_3D_Infinite_Line(HIC_Rendition const *nr, HIC_Polyline const *polyline);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param polygon A pointer to a const HIC_Polygon.
 *
 * This function draws the polygon.
 */
HC_EXTERNAL HIC_API void HIC_Draw_3D_Polygon(HIC_Rendition const *nr, HIC_Polygon const *polygon);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param text A pointer to a const HIC_Text.
 *
 * This function draws text.
 */
HC_EXTERNAL HIC_API void HIC_Draw_3D_Text(HIC_Rendition const *nr, HIC_Text const *text);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param points A pointer to a const HIC_Point.
 * \param stencil A pointer to a const HIC_Stencil_Bitmap.
 *
 * This function draws a stenciled quad.
 */
HC_EXTERNAL HIC_API void HIC_Draw_3D_Stenciled_Quad(HIC_Rendition const *nr, HIC_Point const points[], HIC_Stencil_Bitmap const *stencil);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param tristrip A pointer to a const HIC_Tristrip.
 *
 * This function draws a tristrip.
 */
HC_EXTERNAL HIC_API void HIC_Draw_3D_Tristrip(HIC_Rendition const *nr, HIC_Tristrip const *tristrip);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param phon A pointer to a const HIC_Polyhedron.
 *
 * This function draws a polyhedron.
 */
HC_EXTERNAL HIC_API void HIC_Draw_3D_Polyhedron(HIC_Rendition const *nr, HIC_Polyhedron const *phon);	

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param phon A pointer to a const HIC_Polyhedron.
 *
 * This function draws a shell.
 */
HC_EXTERNAL HIC_API void HIC_Draw_3D_Shell(HIC_Rendition const *nr, HIC_Polyhedron const *phon);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param phon A pointer to a const HIC_Polyhedron.
 *
 * This function draws a mesh.
 */
HC_EXTERNAL HIC_API void HIC_Draw_3D_Mesh(HIC_Rendition const *nr, HIC_Polyhedron const *phon);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param pe A pointer to a const HIC_Polyedge.
 *
 * This function draws polyedge.
 */
HC_EXTERNAL HIC_API void HIC_Draw_3D_Polyedge(HIC_Rendition const *nr, HIC_Polyedge const *pe);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param pm A pointer to a const HIC_Polymarker.
 *
 * This function draws polymarker.
 */
HC_EXTERNAL HIC_API void HIC_Draw_3D_Polymarker(HIC_Rendition const *nr, HIC_Polymarker const *pm);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param pe A pointer to a const HIC_Polyedge.
 * \param ts A pointer to a const HIC_Tristrip.
 *
 * This function draws a 3D polyedge from a tristrip.
 */
HC_EXTERNAL HIC_API void HIC_Draw_3D_Polyedge_From_Tristrip(HIC_Rendition const *nr, HIC_Polyedge const *pe, HIC_Tristrip const *ts);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param ellipse A pointer to a const HIC_Ellipse.
 *
 * This function draws the ellipse.
 */
HC_EXTERNAL HIC_API void HIC_Draw_3D_Ellipse(HIC_Rendition const *nr, HIC_Ellipse const *ellipse);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param arc A pointer to a const HIC_Elliptical_Arc.
 *
 * This function draws the elliptical arc.
 */
HC_EXTERNAL HIC_API void HIC_Draw_3D_Elliptical_Arc(HIC_Rendition const *nr, HIC_Elliptical_Arc const *arc);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param curve A pointer to a const HIC_NURBS_Curve.
 *
 * This function draws the NURBS curve.
 */
HC_EXTERNAL HIC_API void HIC_Draw_3D_NURBS_Curve(HIC_Rendition const *nr, HIC_NURBS_Curve const *curve);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param surface A pointer to a const HIC_NURBS_Surface.
 *
 * This function draws the NURBS surface.
 */
HC_EXTERNAL HIC_API void HIC_Draw_3D_NURBS_Surface(HIC_Rendition const *nr, HIC_NURBS_Surface const *surface);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param cylinder A pointer to a const HIC_Cylinder.
 *
 * This function draws the cylinder.
 */
HC_EXTERNAL HIC_API void HIC_Draw_3D_Cylinder(HIC_Rendition const *nr, HIC_Cylinder const *cylinder);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param polycylinder A pointer to a const HIC_Polycylinder.
 *
 * This function draws the polycylinder.
 */
HC_EXTERNAL HIC_API void HIC_Draw_3D_PolyCylinder(HIC_Rendition const *nr, HIC_Polycylinder const *polycylinder);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param sphere A pointer to a const HIC_Sphere.
 *
 * This function draws sphere.
 */
HC_EXTERNAL HIC_API void HIC_Draw_3D_Sphere(HIC_Rendition const *nr, HIC_Sphere const *sphere);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param grid A pointer to a const HIC_Grid.
 *
 * This function draws 3D grid.
 */
HC_EXTERNAL HIC_API void HIC_Draw_3D_Grid(HIC_Rendition const *nr, HIC_Grid const *grid);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param image A pointer to a const HIC_Image.
 *
 * This function draws the image.
 */
HC_EXTERNAL HIC_API void HIC_Draw_3D_Image(HIC_Rendition const *nr, HIC_Image const *image);

/** @} */ /* end of 3D_draw */


/*!
  \addtogroup Draw_DC_XXX_Rasters
  @{
*/


/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param start A pointer to a const HIC_DC_Point.
 * \param end A pointer to a const HIC_DC_Point.
 * \param rowbytes An int.
 * \param rasters A pointer to a const unsigned char.
 *
 * This function draws a single scan line determined by the bits in the data array. 
 */
HC_EXTERNAL HIC_API void HIC_Draw_DC_Bit_Rasters(HIC_Rendition const *nr, HIC_DC_Point const *start, HIC_DC_Point const *end, int rowbytes, unsigned char const rasters[]);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param start A pointer to a const HIC_DC_Point.
 * \param end A pointer to a const HIC_DC_Point.
 * \param rowbytes An int.
 * \param rasters A pointer to a const unsigned char.
 *
 * This function draws an image specified by an array of eight-bit gray scale values.
 */
HC_EXTERNAL HIC_API void HIC_Draw_DC_Gray8_Rasters(HIC_Rendition const *nr, HIC_DC_Point const *start, HIC_DC_Point const *end, int rowbytes, unsigned char const rasters[]);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param start A pointer to a const HIC_DC_Point.
 * \param end A pointer to a const HIC_DC_Point.
 * \param rowbytes An int.
 * \param rasters A pointer to a const unsigned char.
 *
 * This function draws an image specified by an array of eight-bit color indices into the current color map.
 */
HC_EXTERNAL HIC_API void HIC_Draw_DC_Mapped8_Rasters(HIC_Rendition const *nr, HIC_DC_Point const *start, HIC_DC_Point const *end, int rowbytes, unsigned char const rasters[]);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param start A pointer to a const HIC_DC_Point.
 * \param end A pointer to a const HIC_DC_Point.
 * \param rowbytes An int.
 * \param rasters A pointer to a const unsigned char.
 *
 * This function draws an image specified by an array of 16-bit color indices into the current color map.
 */
HC_EXTERNAL HIC_API void HIC_Draw_DC_Mapped16_Rasters(HIC_Rendition const *nr, HIC_DC_Point const *start, HIC_DC_Point const *end, int rowbytes, unsigned short const rasters[]);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param start A pointer to a const HIC_DC_Point.
 * \param end A pointer to a const HIC_DC_Point.
 * \param rowbytes An int.
 * \param rasters A pointer to a const unsigned char.
 *
 * This function draws an image specified by an array of 32-bit words, each giving a pixel color in the platform-dependent four-byte color format.
 */
HC_EXTERNAL HIC_API void HIC_Draw_DC_RGB32_Rasters(HIC_Rendition const *nr, HIC_DC_Point const *start, HIC_DC_Point const *end, int rowbytes, HIC_RGBAS32 const rasters[]);

/** @} */ /* end of rasters */

/* dc lines */
/*!
  \addtogroup Draw_DC_XXX_Line
  @{
*/

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param points A pointer to a const HIC_DC_Point.
 *
 * This function draws a line.
 */
HC_EXTERNAL HIC_API void HIC_Draw_DC_Line(HIC_Rendition const *nr, HIC_DC_Point const points[]);			

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param points A pointer to a const HIC_DC_Point.
 * \param color A pointer to a const HIC_RGBAS32.
 *
 * This function draws a line using passed in color values instead of the rendition colors.
 */
HC_EXTERNAL HIC_API void HIC_Draw_DC_Colorized_Line(HIC_Rendition const *nr, HIC_DC_Point const points[], HIC_RGBAS32 const color[]);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param points A pointer to a const HIC_DC_Point.
 * \param colors A pointer to a const HIC_RGBAS32.
 *
 * This function draws a line segment with color interpolated from the argument endpoint colors.
 */
HC_EXTERNAL HIC_API void HIC_Draw_DC_Gouraud_Line(HIC_Rendition const *nr, HIC_DC_Point const points[], HIC_RGBAS32 const colors[]);			

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param points A pointer to a const HIC_DC_Point.
 * \param colors A pointer to a const HIC_RGBA.
 * \param planes A pointer to a const HIC_Plane, defined for each vertex V as the camera-space plane that passes through V with V's surface normal, after modelling and camera positioning and before camera projection.
 * \param params A pointer to a const HIC_Parameter.
 * \param param_width An int.
 * \param param_flags A HIC_Integer32.
 *
 * This function draws a line segment with color interpolated at each pixel, using the line pattern and line weight attributes.
 */
HC_EXTERNAL HIC_API void HIC_Draw_DC_Reshaded_Line(HIC_Rendition const *nr, HIC_DC_Point const points[], HIC_RGBA const colors[], HIC_Plane const planes[], HIC_Parameter const params[], int param_width, HIC_Integer32 param_flags);

/** @} */ /* end of dc_line */

/*!
  \addtogroup Draw_DC_XXX_Marker
  @{
*/

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param point A pointer to a const HIC_DC_Point.
 *
 * This function draws a marker, with position specified in device coordinates.
 */
HC_EXTERNAL HIC_API void HIC_Draw_DC_Marker(HIC_Rendition const *nr, HIC_DC_Point const *point);
	
/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param point A pointer to a const HIC_DC_Point.
 * \param color A pointer to a const HIC_RGBAS32.
 *
 * This function draws a marker using passed in color values instead of the rendition colors.
 */
HC_EXTERNAL HIC_API void HIC_Draw_DC_Colorized_Marker(HIC_Rendition const *nr, HIC_DC_Point const *point, HIC_RGBAS32 const *color);
	
/** @} */ /* end of dc_mark */

/* dc polymarkers */
/*!
  \addtogroup Draw_DC_XXX_Polymarker
  @{
*/

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param count An int.
 * \param point An array of const HIC_DC_Point.
 * \param rotations A pointer to a const float.
 * \param size_fixups A pointer to a const float.
 *
 * This function draws a polymarker.
 */
HC_EXTERNAL HIC_API void HIC_Draw_DC_Polymarker(HIC_Rendition const *nr, int count, HIC_DC_Point const point[], float const *rotations, float const *size_fixups);
	
/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param count An int representing the number of points in the HIC_DC_Point array.
 * \param point An array of const HIC_DC_Point.
 * \param colors A pointer to a const HIC_RGBAS32.
 * \param single A boolean.
 * \param rotations A pointer to a const float.
 * \param size_fixups A pointer to a const float.
 *
 * This function draws a polymarker using passed in color values instead of rendition colors.
 */
HC_EXTERNAL HIC_API void HIC_Draw_DC_Colorized_Polymarker(HIC_Rendition const *nr, int count, HIC_DC_Point const point[], HIC_RGBAS32 const *colors, bool single, float const *rotations, float const *size_fixups);

/** @} */ /* end of dc_polymark */

/*!
  \addtogroup Draw_DC_XXX_Polydot
  @{
*/

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param count An int.
 * \param points An array of const HIC_DC_Point.
 *
 * This function draws a polydot.
 */
HC_EXTERNAL HIC_API void HIC_Draw_DC_Polydot(HIC_Rendition const *nr, int count, HIC_DC_Point const points[]);
	
/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param count An int.
 * \param points An array of const HIC_DC_Point.
 * \param colors A pointer to a const HIC_RGBAS32.
 * \param single A boolean.
 *
 * This function draws a polydot using passed in color values instead of rendition colors.
 */
HC_EXTERNAL HIC_API void HIC_Draw_DC_Colorized_Polydot(HIC_Rendition const *nr, int count, HIC_DC_Point const points[], HIC_RGBAS32 const *colors, bool single);
	
/** @} */ /* end of dc_pdot */

/* dc polylines */
/*!
  \addtogroup Draw_DC_XXX_Polylines
  @{
*/
/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param count An int.
 * \param points An array of const HIC_DC_Point.
 *
 * This function draws a polyline.
 */
HC_EXTERNAL HIC_API void HIC_Draw_DC_Polyline(HIC_Rendition const *nr, int count, HIC_DC_Point const points[]);			

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param count An int.
 * \param points An array of const HIC_DC_Point.
 * \param color A pointer to a const HIC_RGBAS32.
 * \param single A boolean.
 *
 * This function draws a polyline using passed in color values instead of the rendition colors.
 *
 * \sa HIC_Draw_DC_Polyline, HIC_Draw_DC_Edges
 */
HC_EXTERNAL HIC_API void HIC_Draw_DC_Colorized_Polyline(HIC_Rendition const *nr, int count, HIC_DC_Point const points[], HIC_RGBAS32 const color[], bool single);			

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param count An int.
 * \param points An array of const HIC_DC_Point.
 * \param colors A pointer to a const HIC_RGBAS32.
 *
 * This function draws a gouraud polyline.
 */
HC_EXTERNAL HIC_API void HIC_Draw_DC_Gouraud_Polyline(HIC_Rendition const *nr, int count, HIC_DC_Point const points[], HIC_RGBAS32 const colors[]);			

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param count An int.
 * \param points An array of const HIC_DC_Point.
 * \param colors A pointer to a const HIC_RGBA.
 * \param planes A pointer to a const HIC_Plane.
 *
 * This function draws a phong polyline.
 */
HC_EXTERNAL HIC_API void HIC_Draw_DC_Phong_Polyline(HIC_Rendition const *nr, int count, HIC_DC_Point const points[], HIC_RGBA const colors[], HIC_Plane const planes[], HIC_DPlane const dplanes[]);			

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param count An int.
 * \param points An array of const HIC_DC_Point.
 * \param colors A pointer to a const HIC_RGBA.
 * \param planes A pointer to a const HIC_Plane.
 * \param params A pointer to a const float.
 * \param param_width An int.
 * \param param_flags A HIC_Integer32.
 *
 * This function draws a textured polyline.
 */
HC_EXTERNAL HIC_API void HIC_Draw_DC_Textured_Polyline(HIC_Rendition const *nr, int count, HIC_DC_Point const points[], HIC_RGBA const colors[], HIC_Plane const planes[], HIC_DPlane const dplanes[], float const params[], int param_width, HIC_Integer32 param_flags);			

/** @} */ /* end of dc_polyline */

/* dc triangles */
/*!
  \addtogroup Draw_DC_XXX_Triangle
  @{
*/

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param points A pointer to a const HIC_DC_Point.
 *
 * This function draws a triangle using passed in color values instead of the rendition colors.
 */
HC_EXTERNAL HIC_API void HIC_Draw_DC_Triangle(HIC_Rendition const *nr, HIC_DC_Point const points[]);			

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param points A pointer to a const HIC_DC_Point.
 * \param color A pointer to a const HIC_RGBAS32.
 *
 * This function draws a triangle using passed in color values instead of the rendition colors.
 */
HC_EXTERNAL HIC_API void HIC_Draw_DC_Colorized_Triangle(HIC_Rendition const *nr, HIC_DC_Point const points[], HIC_RGBAS32 const color[]);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param points A pointer to a const HIC_DC_Point.
 * \param colors A pointer to a const HIC_RGBAS32.
 *
 * This function draws a filled triangle using the face pattern and face contrast color attributes with color interpolated from the vertex colors.
 */
HC_EXTERNAL HIC_API void HIC_Draw_DC_Gouraud_Triangle(HIC_Rendition const *nr, HIC_DC_Point const points[], HIC_RGBAS32 const colors[]);


/* XXX need to abstract param_flags */

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param points A pointer to a const HIC_DC_Point.
 * \param colors A pointer to a const HIC_RGBA.
 * \param planes A pointer to a const HIC_Plane.
 * \param params A pointer to a const HIC_Parameter.
 * \param param_width An int.
 * \param param_flags A HIC_Integer32.
 *
 * This function draws a filled triangle with color interpolated at each pixel using the face pattern and face contrast color attributes.
 */
HC_EXTERNAL HIC_API void HIC_Draw_DC_Reshaded_Triangle(HIC_Rendition const *nr, HIC_DC_Point const points[], HIC_RGBA const colors[], HIC_Plane const planes[], HIC_Parameter const params[], int param_width, HIC_Integer32 param_flags);						

/** @} */ /* end of dc_tri */

/* dc dots */
/*!
  \addtogroup Draw_DC_Dot
  @{
*/

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param point A pointer to a const HIC_DC_Point.
 *
 * This function draws one pixel whose color is the line color.
 */
HC_EXTERNAL HIC_API void HIC_Draw_DC_Dot(HIC_Rendition const *nr, HIC_DC_Point const *point);
/** @} */ /* end of dc_dot */


/*!
  \addtogroup Draw_DC_Ellipse
  @{
*/
/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param where A pointer to a const HIC_DC_Point.
 * \param radius A pointer to a const HIC_DC_Point.
 *
 * This function draws a filled ellipse. 
 */
HC_EXTERNAL HIC_API void HIC_Draw_DC_Ellipse(HIC_Rendition const *nr, HIC_DC_Point const *where, HIC_DC_Point const *radius);
	
/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param where A pointer to a const HIC_DC_Point.
 * \param radius A pointer to a const HIC_DC_Point.
 * \param start A float.
 * \param end A float.
 *
 * This function draws an elliptical arc.
 */
HC_EXTERNAL HIC_API void HIC_Draw_DC_Elliptical_Arc(HIC_Rendition const *nr, HIC_DC_Point const *where, HIC_DC_Point const *radius, float start, float end);
/** @} */ /* end of dc_ellipse */


/*!
  \addtogroup Draw_DC_Face
  @{
*/
/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param count An int, the number of points in the array of vertices which define a polygon.
 * \param points A pointer to a const HIC_DC_Point.
 *
 * This function draws a face.
 */
HC_EXTERNAL HIC_API void HIC_Draw_DC_Face(HIC_Rendition const *nr, int count, HIC_DC_Point const points[]);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param count An int.
 * \param points A pointer to a const HIC_DC_Point.
 * \param color A pointer to a const HIC_RGBAS32.
 *
 * This function draws a face using passed in color values instead of rendition colors.
 */
HC_EXTERNAL HIC_API void HIC_Draw_DC_Colorized_Face(HIC_Rendition const *nr, int count, HIC_DC_Point const points[], HIC_RGBAS32 const color[]);

/** @} */ /* end of dc_face */


/*!
  \addtogroup Draw_DC_Image
  @{
*/
/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param start A pointer to a const HIC_DC_Point, may be off-screen.
 * \param end A pointer to a const HIC_DC_Point.
 * \param hoffset An int.
 * \param format A HIC_Image_Format.
 * \param row_bytes An int.
 * \param rasters A const void pointer that depends on the format of the image.
 * \param pattern An int.
 * \param contrast_color A pointer to a const HIC_Driver_Color.
 * \param db_image A pointer to a const HIC_Image.
 *
 * This device-independent function draws the entire image stored in the data block and does not provide for Z-buffering tilted images.
 */
HC_EXTERNAL HIC_API void HIC_Draw_DC_Image(HIC_Rendition const *nr, HIC_DC_Point const *start, HIC_DC_Point const *end, int hoffset, HIC_Image_Format format, int row_bytes, void const *const *rasters, int pattern, HIC_Driver_Color const *contrast_color, HIC_Image const *db_image);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param strt A HIC_DC_Point pointer which may be off-screen.
 * \param wdth An int.
 * \param hite An int.
 * \param frmt A HIC_Image_Format.
 * \param rowbytes An int.
 * \param image_block A const void pointer.
 *
 * This device-independent function draws the entire image stored in the data block and does not provide for Z-buffering tilted images.
 * limited to RGB24 RGBA32 RGBAS32 image blocks
 */
HC_EXTERNAL HIC_API void HIC_Draw_DC_Image_Block(HIC_Rendition const *nr, HIC_DC_Point *strt, int wdth, int hite, HIC_Image_Format frmt, int rowbytes, void const *image_block);
/** @} */ /* end of dc_image */

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param strt A HIC_DC_Point pointer which may be off-screen.
 * \param wdth An int.
 * \param hite An int.
 * \param frmt A HIC_Image_Format.
 * \param rowbytes An int.
 * \param image_block A const void pointer to the image data.
 * \param z_format A HIC_Image_Format.
 * \param z_rowbytes An int.
 * \param z_image_block A const void pointer to the z image data.
 *
 * This device-independent function draws the entire image stored in the data block and z image block.
 * limited to RGB24 RGBA32 RGBAS32 image blocks and DEPTH16 DEPTH32 DEPTH_FLOAT32 z image blocks.
 */
HC_EXTERNAL HIC_API void HIC_Draw_DC_Image_Block_Z(HIC_Rendition const *nr, HIC_DC_Point *strt, int wdth, int hite, HIC_Image_Format frmt, int rowbytes, void const *image_block, HIC_Image_Format z_format, int z_rowbytes, void const *z_image_block);
/** @} */ /* end of dc_image */


/*!
  \addtogroup Draw_DC_Polygon
  @{
*/

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param count An int, one greater than the number of polygon vertices.
 * \param points A pointer to a const HIC_DC_Point.
 *
 * This function draws a filled polygon with edge.
 *
 * \sa HIC_Insert_Polygon, HIC_Draw_3D_Polygon
 */
HC_EXTERNAL HIC_API void HIC_Draw_DC_Polygon(HIC_Rendition const *nr, int count, HIC_DC_Point const points[]);
/** @} */ /* end of Draw_DC_Polygon */
	
/*!
  \addtogroup Draw_DC_Outline_Ellipse
  @{
*/	
/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param where A pointer to a const HIC_DC_Point.
 * \param radius A pointer to a const HIC_DC_Point.
 *
 * This function draws an ellipse outline (closed curve). 
 */
HC_EXTERNAL HIC_API void HIC_Draw_DC_Outline_Ellipse(HIC_Rendition const *nr, HIC_DC_Point const *where, HIC_DC_Point const *radius);
/** @} */ /* end of Draw_DC_Outline_Ellipse */
	
/*!
  \addtogroup Draw_DC_Rectangle
  @{
*/
/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param left An int.
 * \param right An int.
 * \param bottom An int.
 * \param top An int.
 *
 * This function draws a filled rectangle using face color, pattern, and contrast color attributes.
 * The rectangle is aligned with the device coordinate axes.
 * HOOPS uses this function to fill windows after replacing face with window attributes.
 */
HC_EXTERNAL HIC_API void HIC_Draw_DC_Rectangle(HIC_Rendition const *nr, int left, int right, int bottom, int top);

/** @} */ /* end of Draw_DC_Rectangle */

/*!
  \addtogroup Draw_DC_Stenciled_Quad
  @{
*/

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param points A pointer to a const HIC_DC_Point.
 * \param stencil A pointer to a const HIC_Stencil_Bitmap.
 *
 * This function draws a stenciled quad.
 */
HC_EXTERNAL HIC_API void HIC_Draw_DC_Stenciled_Quad(HIC_Rendition const *nr, HIC_DC_Point const points[], HIC_Stencil_Bitmap const *stencil);
/** @} */ /* end of quad */

/* text drawing */

/*!
  \addtogroup Draw_XXX_Text
  @{
*/

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param ti A pointer to a const HIC_Text_Info.
 *
 * Reconciles the text attributes in the rendition with the capabilities of the requested font and completes the computations to draw the text.
 * Applies the per-character transformations and the atmospheric attenuation when called for in the rendition. 
 * Specified font may be replaced by a stroked font if it doesn't permit called for transformations.
 * If text is drawn in a hardware font, the window system, or the driver, a driver-level method is called otherwise the text is stroked out.
 */
HC_EXTERNAL HIC_API void HIC_Draw_Text(HIC_Rendition const *nr, HIC_Text_Info const *ti);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param where An array of const HIC_DC_Point.
 * \param count An int.
 * \param kstring A pointer to a const HIC_Karacter.
 *
 * This function draws text.
 *
 * \sa HIC_Draw_Text
 */
HC_EXTERNAL HIC_API void HIC_Draw_DC_Text(HIC_Rendition const *nr, HIC_DC_Point const *where, int count, HIC_Karacter const kstring[]);

/*!
* \param nr A pointer to a const HIC_Rendition.
* \param where A pointer to a const HIC_DC_Point.
* \param text A null-terminated string containing the text to be drawn.
*
* This function draws simple text for purposes such as debugging.
*
* \sa HIC_Draw_Simple_Text
*/
HC_EXTERNAL HIC_API void HIC_Draw_Simple_Text(HIC_Rendition const *nr, HIC_DC_Point const *where, const char *text);

/** @} */ /* end of text_draw */

/*!
  \addtogroup Draw_Segment
  @{
*/

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param si A pointer to a const HIC_Segment_Info.
 *
 * This function draws geometry defined in the segment and its subsegments.
 */
HC_EXTERNAL HIC_API void HIC_Draw_Segment(HIC_Rendition const *nr, HIC_Segment_Info const *si);

/** @} */ /* end of draw_segment */


/* special window actions */

/*!
  \addtogroup Draw_Window
  @{
*/

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param extent A pointer to a const HIC_Int_Rectangle.
 *
 * This function fills the intersection of the rectangle specified by extent and the current clipping rectangle
 * with the pattern determined by window color, pattern and contrast color.
 */
HC_EXTERNAL HIC_API void HIC_Draw_Window(HIC_Rendition const *nr, HIC_Int_Rectangle const *extent);
	
/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param extent A pointer to a const HIC_Int_Rectangle.  Specifies extents of frame around window.
 * \param frame A pointer to a HIC_Int_Rectangle.
 * \param calculate_only A boolean.  If true the function draws nothing, otherwise draws specified frame.
 *
 * This function draws a frame of concentric rectangles using two solid polylines 
 * where the weight of the lines and the space between them is the device's default line weight.
 */
HC_EXTERNAL HIC_API void HIC_Draw_Window_Frame(HIC_Rendition const *nr, HIC_Int_Rectangle const *extent, HIC_Int_Rectangle *frame, bool calculate_only);

/** @} */ /* end of draw_window */

/* driver IM */

/*!
  \addtogroup Driver_XXX
  @{
*/

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return An int.
 *
 * The behavior of this function is driver and platform specific.
 * This function returns the window width as an int.
 */
HC_EXTERNAL HIC_API int HIC_Driver_Get_Window_Width(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return An int.
 *
 * The behavior of this function is driver and platform specific.
 * This function returns the window height as an int.
 */
HC_EXTERNAL HIC_API int HIC_Driver_Get_Window_Height(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A boolean, true if double buffering is in effect, otherwise false.
 *
 * The behavior of this function is driver and platform specific.
 * This function returns a boolean which is true if double buffering is in effect, false if double buffering is not in effect.
 */
HC_EXTERNAL HIC_API bool HIC_Driver_Is_Double_Buffering(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A void pointer.
 *
 * This function gets the HDC associated with the current driver.
 * The behavior of this function is driver and platform specific.  
 */
HC_EXTERNAL HIC_API void * HIC_Driver_Get_HDC(HIC_Rendition const *nr);
			
/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param ts A pointer to a const HIC_Tristrip.
 * \return A boolean, true if tristrip has valid display list, otherwise false.
 *
 * The behavior of this function is driver and platform specific.
 * This function returns a boolean which is true if driver tristrip has a valid display list, false if driver tristrip has an invalid display list.
 */
HC_EXTERNAL HIC_API bool HIC_Driver_Tristrip_Has_Valid_DL(HIC_Rendition const *nr, HIC_Tristrip const *ts);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param number An int.
 *
 * The behavior of this function is driver and platform specific. 
 * This is only valid with opengl driver and corresponds to the gl call ActiveTextureARB.
 */
HC_EXTERNAL HIC_API void HIC_Driver_glActiveTextureARB(HIC_Rendition const *nr, int number);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param number An int.
 *
 * The behavior of this function is driver and platform specific. 
 */
HC_EXTERNAL HIC_API void  HIC_Driver_ENSURE_ALPHA_TEST_ON(HIC_Rendition const *nr, int number);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param number An int.
 *
 * The behavior of this function is driver and platform specific. 
 */
HC_EXTERNAL HIC_API void  HIC_Driver_ENSURE_ALPHA_TEST_OFF(HIC_Rendition const *nr, int number);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param number An int.
 *
 * The behavior of this function is driver and platform specific. 
 */
HC_EXTERNAL HIC_API void  HIC_Driver_ENSURE_TEXTURES_ON(HIC_Rendition const *nr, int number);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param number An int.
 *
 * The behavior of this function is driver and platform specific. 
 */
HC_EXTERNAL HIC_API void HIC_Driver_ENSURE_TEXTURES_OFF(HIC_Rendition const *nr, int number);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param number An int.
 *
 * The behavior of this function is driver and platform specific. 
 */
HC_EXTERNAL HIC_API void HIC_Driver_FORCE_TEXTURE_GEN_OFF(HIC_Rendition const *nr, int number);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param image A pointer to const HIC_Image.
 *
 * The behavior of this function is driver and platform specific. 
 */
HC_EXTERNAL HIC_API void HIC_Driver_Bind_Image_As_Texture(HIC_Rendition const *nr, HIC_Image const *image);

 HC_EXTERNAL HIC_API void HIC_Driver_Show_Projection_Matrix(HIC_Rendition const *nr, float matrix[]);
 HC_EXTERNAL HIC_API void HIC_Driver_Draw_Clear_Window(HIC_Rendition const *nr, HIC_Int_Rectangle const *extent);
 
/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A HC_POINTER_SIZED_INT.
 *
 * This function returns the surface pointer for the driver.
 * Currently only valid with the DX9 driver.
 */
HC_EXTERNAL HIC_API HC_POINTER_SIZED_INT HIC_Driver_Get_Surface(HIC_Rendition const *nr);

/** @} */ /* end of driv_im */


/*!
  \addtogroup Finish_Picture
  @{
*/

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * This function can be used to do whatever driver-specific pre-update maintenance might be required
 */
HC_EXTERNAL HIC_API void HIC_Init_Picture(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param swap_buffers A boolean.  If true, function called once, otherwise called multiple times per frame if the hidden surface removal algorithm is "fast hlr".
 *
 * This function can be used to make direct opengl or gdi calls to capture the results of a render.
 */
HC_EXTERNAL HIC_API void HIC_Finish_Picture(HIC_Rendition const *nr, bool swap_buffers);
/** @} */ /* end of Finish_Picture */


/*!
  \addtogroup Convex_Clip_Region
  @{
*/

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param ncnt An int.
 * \param npts A pointer to a const HIC_DC_Point.
 * \return A pointer to a const HIC_Convex_Clip_Region.
 *
 * This function creates a convex clip region.
 */
HC_EXTERNAL HIC_API HIC_Convex_Clip_Region const* HIC_New_Convex_Clip_Region(HIC_Rendition const *nr, long ncnt, HIC_DC_Point const npts[]);

/*!
 * \param ccr A pointer to a const Convex_Clip_Region.
 *
 * This function frees the memory associated with a convex clip region.
 */
HC_EXTERNAL HIC_API void  HIC_Free_Convex_Clip_Region(HIC_Convex_Clip_Region const *ccr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param ccr A pointer to a const HIC_Convex_Clip_Region.
 *
 * This function sets the convex clip region.
 *
 * \sa HIC_New_Convex_Clip_Region
 */
HC_EXTERNAL HIC_API void  HIC_Set_Convex_Clip_Region(HIC_Rendition const *nr, HIC_Convex_Clip_Region const *ccr);

/** @} */ /* end of Convex_Clip_Region */


/*!
  \addtogroup Polygonal_Clip_Region
  @{
*/

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param ncnt An int.
 * \param npts A pointer to a const HIC_DC_Point.
 * \return A pointer to a const HIC_Polygonal_Clip_Region.
 *
 * This function creates a polygonal clip region.
 */
HC_EXTERNAL HIC_API HIC_Polygonal_Clip_Region const* HIC_New_Polygonal_Clip_Region(HIC_Rendition const *nr, long ncnt, HIC_DC_Point const npts[]);

/*!
 * \param pcr A pointer to a const Polygonal_Clip_Region.
 *
 * This function frees the memory associated with a polygonal clip region.
 */
HC_EXTERNAL HIC_API void  HIC_Free_Polygonal_Clip_Region(HIC_Polygonal_Clip_Region const *pcr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param ccr A pointer to a const HIC_Polygonal_Clip_Region.
 *
 * This function sets the polygonal clip region.
 *
 * \sa HIC_New_Polygonal_Clip_Region
 */
HC_EXTERNAL HIC_API void  HIC_Set_Polygonal_Clip_Region(HIC_Rendition const *nr, HIC_Polygonal_Clip_Region const *pcr);

/** @} */ /* end of Polygonal_Clip_Region */


/*!
  \addtogroup Complex_Clip_Region
  @{
*/

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param ncnt An int.
 * \param npts A pointer to a const HIC_DC_Point.
 * \return A pointer to a const HIC_Complex_Clip_Region.
 *
 * This function creates a complex clip region.
 */
HC_EXTERNAL HIC_API HIC_Complex_Clip_Region const* HIC_New_Complex_Clip_Region(HIC_Rendition const *nr, long nloops, int const nlengths[], HIC_DC_Point const npts[]);

/*!
 * \param ccr A pointer to a const Complex_Clip_Region.
 *
 * This function frees the memory associated with a complex clip region.
 */
HC_EXTERNAL HIC_API void  HIC_Free_Complex_Clip_Region(HIC_Complex_Clip_Region const *ccr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param ccr A pointer to a const HIC_Complex_Clip_Region.
 *
 * This function sets the complex clip region.
 *
 * \sa HIC_New_Complex_Clip_Region
 */
HC_EXTERNAL HIC_API void  HIC_Set_Complex_Clip_Region(HIC_Rendition const *nr, HIC_Complex_Clip_Region const *ccr);

/** @} */ /* end of Complex_Clip_Region */


/*!
  \addtogroup New_Geometry
  @{
*/

/*!
 * \param pointer A pointer to a HIC_Geometry.
 *
 * This function frees the memory associated with a geometry data structure.
 */
HC_EXTERNAL HIC_API void HIC_Free_Geometry(HIC_Geometry const *pointer);

/*!
 * \param ts A pointer to a HIC_Tristrip.
 * \return A pointer to a const HIC_Tristrip.
 *
 * This function creates a tristrip data structure.
 *
 * \sa HIC_Draw_3D_Tristrip
 */
HC_EXTERNAL HIC_API HIC_Tristrip const * HIC_New_Tristrip(HIC_Tristrip const *ts);

/*!
 * \param ts A pointer to a HIC_Tristrip.
 *
 * This function frees the memory associated with a tristrip data structure.
 */
HC_EXTERNAL HIC_API void HIC_Free_Tristrip(HIC_Tristrip const *ts);

/*!
 * \param pe A pointer to a HIC_Polyedge.
 * \return A pointer to a const HIC_Polyedge.
 *
 * This function creates a polyedge data structure.
 */
HC_EXTERNAL HIC_API HIC_Polyedge const * HIC_New_Polyedge(HIC_Polyedge const *pe);

/*!
 * \param pe A pointer to a HIC_Polyedge.
 *
 * This function frees the memory associated with a polyedge data structure.
 */
HC_EXTERNAL HIC_API void HIC_Free_Polyedge(HIC_Polyedge const *pe);

/*!
 * \param pm A pointer to a HIC_Polymarker.
 * \return A pointer to a const HIC_Polymarker.
 *
 * This function creates a polymarker data structure.
 */
HC_EXTERNAL HIC_API HIC_Polymarker const * HIC_New_Polymarker(HIC_Polymarker const *pm);

/*!
 * \param pm A pointer to a HIC_Polymarker.
 *
 * This function frees the memory associated with a polymarker data structure.
 */
HC_EXTERNAL HIC_API void HIC_Free_Polymarker(HIC_Polymarker const *pm);

/*!
 * \param point A pointer to a const HIC_Point.
 * \return A pointer to a const HIC_Marker.
 *
 * This function allocates a marker descriptor structure.
 */
HC_EXTERNAL HIC_API HIC_Marker const * HIC_New_Marker(HIC_Point const *point);


/*!
 * \param count An int.
 * \param points A pointer to a HIC_Point.
 * \return A pointer to a const HIC_Polyline.
 *
 * This function allocates a polyline descriptor structure.
 */
HC_EXTERNAL HIC_API HIC_Polyline const * HIC_New_Polyline(int count, HIC_Point points[]);


/*!
 * \param count An int.
 * \param points A pointer to a HIC_Point.
 * \return A pointer to a const HIC_Polygon.
 *
 * This function allocates a polygon descriptor structure.
 */
HC_EXTERNAL HIC_API HIC_Polygon const * HIC_New_Polygon(int count, HIC_Point points[]);
/** @} */ /* end of New_Geometry */


/* routines to allow user to change rendition */
/*!
  \addtogroup New_Rendition
  @{
*/

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A pointer to a const HIC_Rendition.
 *
 * This function creates a new rendition.
 */
HC_EXTERNAL HIC_API HIC_Rendition const * HIC_New_Rendition(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 *
 * This function destroys the rendition and frees the memory allocated for it.
 */
HC_EXTERNAL HIC_API void HIC_Free_Rendition(HIC_Rendition const *nr);
/** @} */ /* end of change_rend */


/*!
  \addtogroup Segment_Path
  @{
*/

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A pointer to a const HIC_Segment_Path.
 *
 * This function returns an array of keys which specifies the complete draw path.
 */
HC_EXTERNAL HIC_API HIC_Segment_Path const *  HIC_Show_Segment_Path(HIC_Rendition const *nr);

/*!
 * \param sp A pointer to a const HIC_Segment_Path.
 *
 * This function frees memory associated with a segment path.
 */
HC_EXTERNAL HIC_API void HIC_Free_Segment_Path(HIC_Segment_Path const *sp);

/** @} */ /* end of seg_info */


/*!
  \addtogroup Stencil_Bitmap
  @{
*/

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param nw An int.
 * \param nh An int.
 * \param bpr An int.
 * \param bpp An int.
 * \param bm A pointer to an unsigned char.
 * \return A pointer to a const HIC_Stencil_Bitmap.
 *
 * This function creates a stencil bitmap.
 */
HC_EXTERNAL HIC_API HIC_Stencil_Bitmap const * HIC_New_Stencil_Bitmap(HIC_Rendition const *nr, int nw, int nh, int  bpr, int bpp, unsigned char *bm);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param stencil A pointer to a const HIC_Stencil_Bitmap.
 *
 * This function frees memory associated with a stencil bitmap.
 */
HC_EXTERNAL HIC_API void HIC_Free_Stencil_Bitmap(HIC_Rendition const *nr, HIC_Stencil_Bitmap const *stencil);

/** @} */ /* end of stencil_bm */


/*!
  \addtogroup Text_Info
  @{
*/
/*!
 * \param text_info A pointer to a const HIC_Text_Info.
 * \return A pointer to a const HIC_Text_Info.
 *
 * This function creates new text info.
 */
HC_EXTERNAL HIC_API HIC_Text_Info const * HIC_New_Text_Info(HIC_Text_Info const *text_info);

/*!
 * \param text_info A pointer to a const HIC_Text_Info.
 *
 * This function frees the memory associated with a text info structure.
 */
HC_EXTERNAL HIC_API void HIC_Free_Text_Info(HIC_Text_Info const *text_info);

/** @} */ /* end of text_info */


/* dc poly triangles */

/*!
  \addtogroup Draw_DC_XXX_Polytriangles
  @{
*/

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param count An int.
 * \param points An array of const HIC_DC_Point.
 *
 * This function draws a polytriangle.
 */
HC_EXTERNAL HIC_API void HIC_Draw_DC_Polytriangle(HIC_Rendition const *nr, int count, HIC_DC_Point const points[]);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param count An int.
 * \param points An array of const HIC_DC_Point.
 * \param color A pointer to a const HIC_RGBAS32.
 * \param single A boolean.
 *
 * This function draws a polytriangle using passed in color values instead of the rendition colors.
 */
HC_EXTERNAL HIC_API void HIC_Draw_DC_Colorized_Polytriangle(HIC_Rendition const *nr, int count, HIC_DC_Point const points[], HIC_RGBAS32 const color[], bool single);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param count An int.
 * \param points An array of const HIC_DC_Point.
 * \param colors A pointer to a const HIC_RGBAS32.
 *
 * This function draws a gouraud polytriangle.
 */
HC_EXTERNAL HIC_API void HIC_Draw_DC_Gouraud_Polytriangle(HIC_Rendition const *nr, int count, HIC_DC_Point const points[], HIC_RGBAS32 const colors[]);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param count An int.
 * \param points An array of const HIC_DC_Point.
 * \param colors A pointer to a const HIC_RGBA.
 * \param planes An pointer to a const HIC_Plane.
 *
 * This function draws a phong polytriangle.
 */
HC_EXTERNAL HIC_API void HIC_Draw_DC_Phong_Polytriangle(HIC_Rendition const *nr, int count, HIC_DC_Point const points[], HIC_RGBA const colors[], HIC_Plane const planes[], HIC_DPlane const dplanes[]);


/* XXX need to abstract param_flags */

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param count An int.
 * \param points An array of const HIC_DC_Point.
 * \param colors A pointer to a const HIC_RGBA.
 * \param planes A pointer to a const HIC_Plane.
 * \param params A pointer to a const float.
 * \param param_width An int.
 * \param param_flags A HIC_Integer32.
 *
 * This function draws a textured polytriangle.
 */
HC_EXTERNAL HIC_API void HIC_Draw_DC_Textured_Polytriangle(HIC_Rendition const *nr, int count, HIC_DC_Point const points[], HIC_RGBA const colors[], HIC_Plane const planes[], HIC_DPlane const dplanes[], float const params[], int param_width, HIC_Integer32 param_flags);

/** @} */ /* end of dc_poly_tri */

/*!
  \addtogroup User_Face_Pattern
  @{
*/

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param nrows An int. If negative, the pattern will be applied with the row order reversed from that of the stored character array.
 * \param ncols An int.
 * \param npat A static unsigned char.
 * \param nsee A const int.
 * \return A pointer to a const HIC_User_Face_Pattern.
 *
 * This function defines a custom face pattern.
 */
HC_EXTERNAL HIC_API HIC_User_Face_Pattern const * HIC_New_User_Face_Pattern(HIC_Rendition const *nr, int nrows, int ncols,  unsigned char npat[], int const nsee);

/*!
 * \param upat A pointer to a const HIC_User_Face_Pattern.
 *
 * This function releases the memory allocated for a user pattern.
 */
HC_EXTERNAL HIC_API void HIC_Free_User_Face_Pattern(HIC_User_Face_Pattern const *upat);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param upat A pointer to a const HIC_User_Face_Pattern.
 *
 * This function sets a user face pattern.
 */
HC_EXTERNAL HIC_API void HIC_Set_User_Face_Pattern(HIC_Rendition const *nr, HIC_User_Face_Pattern const *upat);
/** @} */ /* end of user_face_pattern */


/* line and edges */
/*!
  \addtogroup User_Line_Pattern
  @{
*/

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param nlen A long.
 * \param npat A void pointer.
 * \return A pointer to a HIC_Line_Style.
 *
 * This function creates a new user line pattern.
 */
HC_EXTERNAL HIC_API HIC_Line_Style *  HIC_New_User_Line_Pattern(HIC_Rendition const *nr, long nlen, void *npat);
/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param nlen A long.
 * \param npat A void pointer.
 * \param abs A long.
 * \return A pointer to a HIC_Line_Style.
 *
 * This function creates a new user line pattern, using integers to describe the pattern.
 *
 * \sa HIC_New_User_Line_Pattern
 */
HC_EXTERNAL HIC_API HIC_Line_Style * HIC_New_User_Line_Pattern2(HIC_Rendition const *nr, long nlen, void *npat, long abs);

/*!
 * \param upat A pointer to a const HIC_Line_Style.
 *
 * This function frees memory associated with a user line pattern.
 *
 * \sa HIC_New_User_Line_Pattern
 */
HC_EXTERNAL HIC_API void HIC_Free_User_Line_Pattern(HIC_Line_Style const *upat);
/** @} */ /* end of user_line_pattern */

/* user shells */

/*!
  \addtogroup User_Shell
  @{
*/

/*!
 * \param point_count An int representing the number of points in the points array.
 * \param points A pointer to a const HIC_Point.
 * \param normals A pointer to a const HIC_Vector.
 * \param face_list_length An int.
 * \param face_list A pointer to an int.
 * \return A pointer to a const HIC_Polyhedron.
 *
 * This function allocates memory associated with a polyhedron data structure.
 */
HC_EXTERNAL HIC_API HIC_Polyhedron const * HIC_New_User_Shell(int point_count, HIC_Point const points[], HIC_Vector const normals[], int face_list_length, int face_list[]);

/*!
 * \param user_shell A pointer to a const HIC_Polyhedron.
 *
 * This function frees memory associated with a polyhedron data structure.
 */
HC_EXTERNAL HIC_API void HIC_Free_User_Shell(HIC_Polyhedron const *user_shell);

/** @} */ /* end of u_shells */

/*!
  \addtogroup Get_Bounding_Cuboid
  @{
*/

/*!
 * \param si A pointer to a const HIC_Segment_Info.
 * \param bounding A pointer to HIC_Bounding_Cuboid.
 *
 * This function returns the bounding cuboid.
 */
HC_EXTERNAL HIC_API void HIC_Get_Bounding_Cuboid(HIC_Segment_Info const *si, HIC_Bounding_Cuboid *bounding);

/** @} */ /* end of get_bounding_cuboid */

/*!
  \addtogroup Get_Geometry_Key
  @{
*/

/*!
 * \param geo A pointer to a const HIC_Polyhedron.
 * \return A HC_KEY.
 *
 * This function returns the geometry key.
 */
HC_EXTERNAL HIC_API HC_KEY HIC_Get_Geometry_Key(HIC_Geometry const *geo);
/** @} */ /* end of Get_Geometry_Key */

/*!
  \addtogroup Get_Image_Block
  @{
*/

/*!
 * \param image A pointer to a const HIC_Image.
 *
 * This function gets the image block data pointer.
 */
HC_EXTERNAL HIC_API void * HIC_Get_Image_Block(HIC_Image const *image);
/** @} */ /* end of Get_Image_Block */

/*!
  \addtogroup Key_To_Geometry_Pointer
  @{
*/

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param key A HC_KEY.
 * \return A pointer to a const HIC_Geometry.
 *
 * This function returns a pointer to geometry referred to by the passed in key parameter.
 */
HC_EXTERNAL HIC_API HIC_Geometry const * HIC_Key_To_Geometry_Pointer(HIC_Rendition const *nr, HC_KEY key);
/** @} */ /* end of Key_To_Geometry_Pointer */

/*!
  \addtogroup Partial_Erasing
  @{
*/

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A boolean.
 *
 * If the incremental update heuristic is enabled partial erasing is used to draw out objects deleted from the database.
 * When partial erasing is in progress geometry items are drawn in the window color.
 */
HC_EXTERNAL HIC_API bool HIC_Partial_Erasing(HIC_Rendition const *nr);
/** @} */ /* end of Partial_Erasing */

/*!
  \addtogroup Polyhedron_XXX_Face_Normals
  @{
*/
/*!
 * \param phon A pointer to a const HIC_Polyhedron.
 * \return A boolean, true if polyhedron face normals are valid and false if polyhedron face normals are invalid.
 *
 * This function returns a boolean which is true if polyhedron face normals are valid and 
 * false if polyhedron face normals are invalid.
 */
HC_EXTERNAL HIC_API bool HIC_Polyhedron_Face_Normals_Valid(HIC_Polyhedron const *phon);
/*!
 * \param phon A pointer to a const HIC_Polyhedron.
 *
 * This function assembles polyhedron face normals.
 */
HC_EXTERNAL HIC_API void HIC_Polyhedron_Assemble_Face_Normals(HIC_Polyhedron const *phon);
/** @} */ /* end of Polyhedron_XXX_Face_Normals*/

/*!
  \addtogroup Polyhedron_Generate_Face_List
  @{
*//*!
 * \param phon A pointer to a const HIC_Polyhedron.
 *
 * This function generates a polyhedron face list.
 */
HC_EXTERNAL HIC_API void HIC_Polyhedron_Generate_Face_List(HIC_Polyhedron const *phon);
/** @} */ /* end of Polyhedron_Generate_Face_List*/

/*!
  \addtogroup Record_Selection
  @{
*/

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param xyoff A float, representing the distance from the item to the locator position.
 * \param bz A float, representing the z coordinate.
 * \param off1 A int, representing the offset of the item from the locator position.
 * \param off2 A int, representing the offset of the item from the locator position.
 * \param off3 A int, representing the offset of the item from the locator position.
 *
 * This function enters an item into the current selection list.
 */
HC_EXTERNAL HIC_API void HIC_Record_Selection(HIC_Rendition const *nr, float xyoff, float bz, int off1, int off2, int off3);
/** @} */ /* end of Record_Selection*/

/*!
  \addtogroup Restrict_Clipping
  @{
*//*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param nleft An int.
 * \param nright An int.
 * \param nbottom An int.
 * \param ntop An int.
 *
 * This function restricts the clipping rectangle using the passed in clipping limits.
 */
HC_EXTERNAL HIC_API void  HIC_Restrict_Clipping(HIC_Rendition const *nr, long nleft, long nright, long nbottom, long ntop);
/** @} */ /* end of Restrict_Clipping*/

/*!
  \addtogroup Select_Geometry
  @{
*/

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param si A pointer to a const HIC_Segment_Info.
 *
 * This function selects a segment.
 */
HC_EXTERNAL HIC_API void HIC_Select_Segment(HIC_Rendition const *nr, HIC_Segment_Info const *si);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param wi A pointer to a const HIC_Geometry.
 *
 * This function selects a window.
 */
HC_EXTERNAL HIC_API void HIC_Select_Window(HIC_Rendition const *nr, HIC_Geometry *wi);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param geo A pointer to a const HIC_Geometry.
 *
 * This function checks the referenced database element against the current selection event.
 */
HC_EXTERNAL HIC_API void HIC_Select_Geometry(HIC_Rendition const *nr, HIC_Geometry const *geo);

/** @} */ /* end of sel_util */

/*!
  \addtogroup Show_Selection_XXX
  @{
*/
/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A pointer to a const HIC_Point.
 *
 * This function returns the location the user was pointing at when the current selection traversal was triggered.
 */
HC_EXTERNAL HIC_API HIC_Point const * HIC_Show_Selection_Point(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A pointer to a const HIC_Volume.
 *
 * This function returns selection volume.
 */
HC_EXTERNAL HIC_API HIC_Volume const * HIC_Show_Selection_Volume(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A float.
 *
 * This function returns selection proximity square as a float value.
 */
HC_EXTERNAL HIC_API float HIC_Show_Selection_Proximity_Sq(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A HIC_Integer32.
 *
 * This function returns selection status.
 */
HC_EXTERNAL HIC_API HIC_Integer32 HIC_Show_Selection_Status(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return An int.
 *
 * This function returns the selection action.
 */
HC_EXTERNAL HIC_API int HIC_Show_Selection_Action(HIC_Rendition const *nr);
/** @} */ /* end of sel_util */

/*!
  \addtogroup Show_Face_XXX
  @{
*/


/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A float.
 *
 * This function returns face opacity as a float.
 */
HC_EXTERNAL HIC_API float HIC_Show_Face_Opacity(HIC_Rendition const *inr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return An unsigned char.
 *
 * This function returns face direct rgb red as an unsigned char.
 */
HC_EXTERNAL HIC_API unsigned char HIC_Show_Face_DirectRGB_Red(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return An unsigned char.
 *
 * This function returns face direct rgb green as an unsigned char.
 */
HC_EXTERNAL HIC_API unsigned char HIC_Show_Face_DirectRGB_Green(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return An unsigned char.
 *
 * This function returns face direct rgb blue as an unsigned char.
 */
HC_EXTERNAL HIC_API unsigned char HIC_Show_Face_DirectRGB_Blue(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return An unsigned char.
 *
 * This function returns face direct rgb alpha as an unsigned char.
 */
HC_EXTERNAL HIC_API unsigned char HIC_Show_Face_DirectRGB_Alpha(HIC_Rendition const *nr);

/** @} */ /* end of Show_Face_XXX */


/*!
  \addtogroup Face_Material_XXX
  @{
*/
/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A bool.
 *
 * This function returns true if the face has a material.
 */
HC_EXTERNAL HIC_API bool HIC_Has_Face_Material(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A bool.
 *
 * This function returns true if the face material has an environment texture.
 */
HC_EXTERNAL HIC_API bool HIC_Face_Mat_Has_Enviro_Txr(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition and an integer index into the texture list.
 * \return A bool.
 *
 * This function returns true if the face material has an diffuse texture on the specified layer.
 */
HC_EXTERNAL HIC_API bool HIC_Face_Mat_Has_Diffuse_Txr(HIC_Rendition const *nr, int layer);

/*!
 * \param nr A pointer to a const HIC_Rendition 
 * \param rgb A pointer to a HIC_RGB, a structure of three floats, each between zero and one.
 *
 * This sets the tint on a face material diffuse texture.
 */
HC_EXTERNAL HIC_API void HIC_Face_Mat_Set_Diffuse_Txr_Tint(HIC_Rendition const *nr, HIC_RGB const *rgb);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A float.
 *
 * This function returns the face material mirror red component.
 */
HC_EXTERNAL HIC_API float HIC_Show_Face_Mat_Mirror_Txr_R(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A float.
 *
 * This function returns the face material mirror green component.
 */
HC_EXTERNAL HIC_API float HIC_Show_Face_Mat_Mirror_Txr_G(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A float.
 *
 * This function returns the face material mirror green component.
 */
HC_EXTERNAL HIC_API float HIC_Show_Face_Mat_Mirror_Txr_B(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A float.
 *
 * This function returns the face material opacity.
 */
HC_EXTERNAL HIC_API float HIC_Show_Face_Mat_Opacity(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param r  A float
 *
 * This function sets the face material transmission red component.
 */
HC_EXTERNAL HIC_API float HIC_Show_Face_Mat_Trans_R(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param r  A float
 *
 * This function sets the face material transmission green component.
 */
HC_EXTERNAL HIC_API float HIC_Show_Face_Mat_Trans_G(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param r  A float
 *
 * This function sets the face material transmission blue component.
 */
HC_EXTERNAL HIC_API float HIC_Show_Face_Mat_Trans_B(HIC_Rendition const *nr);
/** @} */ /* end of face_material_xxx */

/*!
  \addtogroup Current_Arc_XXX
  @{
*/
/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A float.
 *
 * This function returns the current arc start.
 */
HC_EXTERNAL HIC_API float HIC_Show_Current_Arc_Start(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A float.
 *
 * This function returns the current arc end.
 */
HC_EXTERNAL HIC_API float HIC_Show_Current_Arc_End(HIC_Rendition const *nr);
/** @} */ /* end of current_arc_xxx */

/*!
  \addtogroup Show_Diffuse_XXX
  @{
*/

/*!
 * \param nr A pointer to a const HIC_Rendition and an integer index into the texture list.
 * \return An unsigned char.
 *
 * This function returns face material diffuse texture name length as an int.
 */
HC_EXTERNAL HIC_API int HIC_Show_Diffuse_Textures_Name_Length(HIC_Rendition const *nr, const int layer);

/*!
 * \param nr A pointer to a const HIC_Rendition and an integer index into the texture list.
 * \return An unsigned char.
 *
 * This function returns face material diffuse texture name text as a pointer to a const char.
 */
HC_EXTERNAL HIC_API const char * HIC_Show_Diffuse_Textures_Name_Text(HIC_Rendition const *nr, const int layer);

/*!
 * \param nr A pointer to a const HIC_Rendition and an integer index into the texture list.
 * \return An unsigned char.
 *
 * This function returns face material diffuse texture texture component name length as an int.
 */
HC_EXTERNAL HIC_API int HIC_Show_Diffuse_Texture_Source_Name_Length(HIC_Rendition const *nr, const int layer);

/*!
 * \param nr A pointer to a const HIC_Rendition and an integer index into the texture list.
 * \return An unsigned char.
 *
 * This function returns face material diffuse texture texture component name text as a pointer to a const char.
 */
HC_EXTERNAL HIC_API const char * HIC_Show_Diffuse_Texture_Source_Name_Text(HIC_Rendition const *nr, const int layer);

/** @} */ /* end of Show_Diffuse_XXX */

/*!
  \addtogroup Show_Mirroring
  @{
*/

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A float.
 *
 * This function returns mirroring as a boolean.
 */
HC_EXTERNAL HIC_API bool HIC_Show_Mirroring(HIC_Rendition const *inr);

/** @} */ /* end of Show_Mirroring */


/*!
  \addtogroup Get_Geometry_Owner
  @{
*/

/*!
 * \param geo A pointer to a const HIC_Geometry.
 * \return A pointer to a const HIC_Segment.
 *
 * This function returns the geometry owner.
 */
HC_EXTERNAL HIC_API HIC_Segment const * HIC_Get_Geometry_Owner(HIC_Geometry const *geo);

/** @} */ /* end of get_geo */

/*!
  \addtogroup Set_Attribute_Locks
  @{
*/
/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param attribute_locks A HIC_Attributes, one or more of which work together.
 *
 * This function locks the attribute(s).
 */
HC_EXTERNAL HIC_API void HIC_Set_Attribute_Locks(HIC_Rendition const *nr, HIC_Attributes attribute_locks);


/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A HIC_Attribute_Locks.
 *
 * This function returns the attribute locks.
 */
HC_EXTERNAL HIC_API HIC_Attribute_Locks HIC_Show_Attribute_Locks(HIC_Rendition const *nr);
/** @} */ /* end of Set_Attribute_Locks */



/*!
  \addtogroup Set_Clipping
  @{
*/

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param nleft An int.
 * \param nright An int.
 * \param nbottom An int.
 * \param ntop An int.
 *
 * This function sets the hard clip limit attributes.
 */
HC_EXTERNAL HIC_API void  HIC_Set_Clipping(HIC_Rendition const *nr, long nleft, long nright, long nbottom, long ntop);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A pointer to a HIC_Int_Rectangle.
 *
 * This function returns the clipping rectangle.		
 */
HC_EXTERNAL HIC_API HIC_Int_Rectangle const * HIC_Show_Clipping(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.  This should be a new rendition created by HIC_New_Rendition.
 *
 * This function clears the 'clipping not needed' flag.
 */
HC_EXTERNAL HIC_API void  HIC_UnSet_Clipping_Not_Needed(HIC_Rendition const *nr);

/** @} */ /* end of Set_Clipping */


/* locks */

/*!
  \addtogroup Set_Color_Locks
 *  It is possible to override local attributes temporarily, using attribute lock. 
 *  When a specific attribute is locked in a segment, that attribute's value applies to all sub-segments, 
 *  regardless of whether those sub-segments have local attribute values set for that same attribute. 
  @{
*/

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param material_color_locks A HIC_Color_Lock_Objects.
 *
 * This function locks the color front attribute.
 */
HC_EXTERNAL HIC_API void  HIC_Set_Color_Front_Locks(HIC_Rendition const *nr, HIC_Color_Lock_Objects material_color_locks);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param material_color_locks A HIC_Color_Lock_Objects.
 *
 * This function locks the color back attribute.
 */
HC_EXTERNAL HIC_API void  HIC_Set_Color_Back_Locks(HIC_Rendition const *nr, HIC_Color_Lock_Objects material_color_locks);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param material_color_locks A HIC_Color_Lock_Objects.
 *
 * This function locks the color face contrast attribute.
 */
HC_EXTERNAL HIC_API void  HIC_Set_Color_Face_Contrast_Locks(HIC_Rendition const *nr, HIC_Color_Lock_Objects material_color_locks);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param material_color_locks A HIC_Color_Lock_Objects.
 *
 * This function locks the color edge attribute.
 */
HC_EXTERNAL HIC_API void  HIC_Set_Color_Edge_Locks(HIC_Rendition const *nr, HIC_Color_Lock_Objects material_color_locks);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param material_color_locks A HIC_Color_Lock_Objects.
 *
 * This function locks the color edge contrast attribute.
 */
HC_EXTERNAL HIC_API void  HIC_Set_Color_Edge_Contrast_Locks(HIC_Rendition const *nr, HIC_Color_Lock_Objects material_color_locks);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param material_color_locks A HIC_Color_Lock_Objects.
 *
 * This function locks the color line attribute.
 */
HC_EXTERNAL HIC_API void  HIC_Set_Color_Line_Locks(HIC_Rendition const *nr, HIC_Color_Lock_Objects material_color_locks);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param material_color_locks A HIC_Color_Lock_Objects.
 *
 * This function locks the color line contrast attribute.
 */
HC_EXTERNAL HIC_API void  HIC_Set_Color_Line_Contrast_Locks(HIC_Rendition const *nr, HIC_Color_Lock_Objects material_color_locks);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param material_color_locks A HIC_Color_Lock_Objects.
 *
 * This function locks the color marker attribute.
 */
HC_EXTERNAL HIC_API void  HIC_Set_Color_Marker_Locks(HIC_Rendition const *nr, HIC_Color_Lock_Objects material_color_locks);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param material_color_locks A HIC_Color_Lock_Objects.
 *
 * This function locks the color marker contrast attribute.
 */
HC_EXTERNAL HIC_API void  HIC_Set_Color_Marker_Contrast_Locks(HIC_Rendition const *nr, HIC_Color_Lock_Objects material_color_locks);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param material_color_locks A HIC_Color_Lock_Objects.
 *
 * This function locks the color vertex attribute.
 */
HC_EXTERNAL HIC_API void  HIC_Set_Color_Vertex_Locks(HIC_Rendition const *nr, HIC_Color_Lock_Objects material_color_locks);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param material_color_locks A HIC_Color_Lock_Objects.
 *
 * This function locks the color vertex contrast attribute.
 */
HC_EXTERNAL HIC_API void  HIC_Set_Color_Vertex_Contrast_Locks(HIC_Rendition const *nr, HIC_Color_Lock_Objects material_color_locks);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param material_color_locks A HIC_Color_Lock_Objects.
 *
 * This function locks the color text attribute.
 */
HC_EXTERNAL HIC_API void  HIC_Set_Color_Text_Locks(HIC_Rendition const *nr, HIC_Color_Lock_Objects material_color_locks);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param material_color_locks A HIC_Color_Lock_Objects.
 *
 * This function locks the color text contrast attribute.
 */
HC_EXTERNAL HIC_API void  HIC_Set_Color_Text_Contrast_Locks(HIC_Rendition const *nr, HIC_Color_Lock_Objects material_color_locks);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param material_color_locks A HIC_Color_Lock_Objects.
 *
 * This function locks the color window attribute.
 */
HC_EXTERNAL HIC_API void  HIC_Set_Color_Window_Locks(HIC_Rendition const *nr, HIC_Color_Lock_Objects material_color_locks);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param material_color_locks A HIC_Color_Lock_Objects.
 *
 * This function locks the color window contrast attribute.
 */
HC_EXTERNAL HIC_API void  HIC_Set_Color_Window_Contrast_Locks(HIC_Rendition const *nr, HIC_Color_Lock_Objects material_color_locks);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param material_color_locks A HIC_Color_Lock_Objects.
 *
 * This function locks the color cut face attribute.
 */
HC_EXTERNAL HIC_API void  HIC_Set_Color_Cut_Face_Locks(HIC_Rendition const *nr, HIC_Color_Lock_Objects material_color_locks);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param material_color_locks A HIC_Color_Lock_Objects.
 *
 * This function locks the color cut edge attribute.
 */
HC_EXTERNAL HIC_API void  HIC_Set_Color_Cut_Edge_Locks(HIC_Rendition const *nr, HIC_Color_Lock_Objects material_color_locks);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param color_locks A HIC_Color_Lock_Objects.
 *
 * This function locks the color attribute.
 */
HC_EXTERNAL HIC_API void HIC_Set_Color_Locks(HIC_Rendition const *nr, HIC_Color_Lock_Objects color_locks);

/** @} */ /* end of color locks */

/*!
  \addtogroup Show_Color_Locks_XXX
  @{
*/
/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A HIC_Material_Color_Locks.
 *
 * This function returns the color front locks.
 */
HC_EXTERNAL HIC_API  HIC_Material_Color_Locks  HIC_Show_Color_Front_Locks(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A HIC_Material_Color_Locks.
 *
 * This function returns the color back locks.
 */
HC_EXTERNAL HIC_API  HIC_Material_Color_Locks  HIC_Show_Color_Back_Locks(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A HIC_Material_Color_Locks.
 *
 * This function returns the color face contrast locks.
 *
 * \sa HIC_Set_Color_Face_Contrast_Locks
 */
HC_EXTERNAL HIC_API  HIC_Material_Color_Locks  HIC_Show_Color_Face_Contrast_Locks(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A HIC_Material_Color_Locks.
 *
 * This function returns the color edge locks.
 *
 * \sa HIC_Set_Color_Edge_Locks
 */
HC_EXTERNAL HIC_API  HIC_Material_Color_Locks  HIC_Show_Color_Edge_Locks(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A HIC_Material_Color_Locks.
 *
 * This function returns the color edge contrast locks.
 *
 * \sa HIC_Set_Color_Edge_Contrast_Locks
 */
HC_EXTERNAL HIC_API  HIC_Material_Color_Locks  HIC_Show_Color_Edge_Contrast_Locks(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A HIC_Material_Color_Locks.
 *
 * This function returns the color line locks.
 *
 * \sa HIC_Set_Color_Line_Locks
 */
HC_EXTERNAL HIC_API  HIC_Material_Color_Locks  HIC_Show_Color_Line_Locks(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A HIC_Material_Color_Locks.
 *
 * This function returns the color line contrast locks.
 *
 * \sa HIC_Set_Color_Line_Contrast_Locks
 */
HC_EXTERNAL HIC_API  HIC_Material_Color_Locks  HIC_Show_Color_Line_Contrast_Locks(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A HIC_Material_Color_Locks.
 *
 * This function returns the color marker locks.
 *
 * \sa HIC_Set_Color_Marker_Locks
 */
HC_EXTERNAL HIC_API  HIC_Material_Color_Locks  HIC_Show_Color_Marker_Locks(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A HIC_Material_Color_Locks.
 *
 * This function returns the color marker contrast locks.
 *
 * \sa HIC_Set_Color_Marker_Contrast_Locks
 */
HC_EXTERNAL HIC_API  HIC_Material_Color_Locks  HIC_Show_Color_Marker_Contrast_Locks(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A HIC_Material_Color_Locks.
 *
 * This function returns the color vertex locks.
 *
 * \sa HIC_Set_Color_Vertex_Locks
 */
HC_EXTERNAL HIC_API  HIC_Material_Color_Locks  HIC_Show_Color_Vertex_Locks(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A HIC_Material_Color_Locks.
 *
 * This function returns the color vertex contrast locks.
 *
 * \sa HIC_Set_Color_Vertex_Contrast_Locks
 */
HC_EXTERNAL HIC_API  HIC_Material_Color_Locks  HIC_Show_Color_Vertex_Contrast_Locks(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A HIC_Material_Color_Locks.
 *
 * This function returns the color text locks.
 *
 * \sa HIC_Set_Color_Text_Locks
 */
HC_EXTERNAL HIC_API  HIC_Material_Color_Locks  HIC_Show_Color_Text_Locks(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A HIC_Material_Color_Locks.
 *
 * This function returns the color text contrast locks.
 *
 * \sa HIC_Set_Color_Text_Contrast_Locks
 */
HC_EXTERNAL HIC_API  HIC_Material_Color_Locks  HIC_Show_Color_Text_Contrast_Locks(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A HIC_Material_Color_Locks.
 *
 * This function returns the color window locks.
 *
 * \sa HIC_Set_Color_Window_Locks
 */
HC_EXTERNAL HIC_API  HIC_Material_Color_Locks  HIC_Show_Color_Window_Locks(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A HIC_Material_Color_Locks.
 *
 * This function returns the color window contrast locks.
 *
 * \sa HIC_Set_Color_Window_Contrast_Locks
 */
HC_EXTERNAL HIC_API  HIC_Material_Color_Locks  HIC_Show_Color_Window_Contrast_Locks(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A HIC_Material_Color_Locks.
 *
 * This function returns the color cut face locks.
 *
 * \sa HIC_Set_Color_Cut_Face_Locks
 */
HC_EXTERNAL HIC_API  HIC_Material_Color_Locks  HIC_Show_Color_Cut_Face_Locks(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A HIC_Material_Color_Locks.
 *
 * This function returns the color cut edge locks.
 *
 * \sa HIC_Set_Color_Cut_Edge_Locks
 */
HC_EXTERNAL HIC_API  HIC_Material_Color_Locks  HIC_Show_Color_Cut_Edge_Locks(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A HIC_Color_Locks.
 *
 * This function returns the color locks.
 *
 * \sa HIC_Set_Color_Locks
 */
HC_EXTERNAL HIC_API HIC_Color_Locks HIC_Show_Color_Locks(HIC_Rendition const *nr);
/** @} */ /* end of color locks */


/*!
  \addtogroup Show_Visibility_Locks
  @{
*/

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A HIC_Visibility_Locks.
 *
 * This function returns the visibility locks.
 *
 * \sa HIC_Set_Visibility_Locks
 */
HC_EXTERNAL HIC_API HIC_Visibility_Locks HIC_Show_Visibility_Locks(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param visibility_locks A HIC_Visibility_e, one or more of which work together.
 *
 * This function locks the visibility.
 */
HC_EXTERNAL HIC_API void HIC_Set_Visibility_Locks(HIC_Rendition const *nr, HIC_Visibility visibility_locks);

/** @} */ /* end of visibility locks */

/*!
  \addtogroup Set_Color_Map_Start
  @{
*/
/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param strt A HIC_Color_Index. XXX -missing type-
 *
 * This function sets the color map starting index.
 */
HC_EXTERNAL HIC_API void HIC_Set_Color_Map_Start(HIC_Rendition const *nr, long strt);
/** @} */ /* end of Set_Color_Map_Start */

/*!
  \addtogroup Set_Edge_XXX
  @{
*/

/*!
 * \param nr A pointer to a const HIC_Rendition.
 *
 * This function sets the line rendition equal to the edge rendition.
 */
HC_EXTERNAL HIC_API void HIC_Set_Line_Equal_Edge_Rendition(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param nweight An int.
 *
 * This function changes the edge weight.
 */
HC_EXTERNAL HIC_API void  HIC_Set_Edge_Weight(HIC_Rendition const *nr, int nweight);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param nunit An HIC_Size_Units.
 * \param nweight An float.
 *
 * This function changes the edge weight.
 */
HC_EXTERNAL HIC_API void  HIC_Set_Variable_Edge_Weight(HIC_Rendition const *nr, HIC_Size_Units nunits, float nweight);


/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param npattern A char.
 *
 * This function changes the edge pattern.
 */
HC_EXTERNAL HIC_API void  HIC_Set_Edge_Pattern(HIC_Rendition const *nr, char npattern);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param nname A const char.
 *
 * This function changes the edge style.
 */
HC_EXTERNAL HIC_API void HIC_Set_Edge_Style(HIC_Rendition const *nr, char const *nname);
/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param upat A pointer to a const HIC_User_Line_Pattern.
 *
 * This function sets the user edge pattern.
 */
HC_EXTERNAL HIC_API void HIC_Set_User_Edge_Pattern(HIC_Rendition const *nr, HIC_User_Line_Pattern const *upat);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param rgb A pointer to a HIC_RGB, a structure of three floats, each between zero and one.
 *
 * This function changes the value of edge color.
 */
HC_EXTERNAL HIC_API void HIC_Set_Edge_Color(HIC_Rendition const *nr, HIC_RGB const *rgb);
/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param rgb A pointer to a HIC_RGB, a structure of three floats, each between zero and one.
 *
 * This function changes the edge contrast color.
 */
HC_EXTERNAL HIC_API void HIC_Set_Edge_Contrast_Color(HIC_Rendition const *nr, HIC_RGB const *rgb);
/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param rgb A pointer to a HIC_RGB, a structure of three floats, each between zero and one.
 *
 * This function changes the cut edge color.
 */
HC_EXTERNAL HIC_API void HIC_Set_Cut_Edge_Color(HIC_Rendition const *nr, HIC_RGB const *rgb);
/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param internal A HIC_RGBAS32.
 *
 * This function changes the internal edge color.
 */
HC_EXTERNAL HIC_API void HIC_Set_Internal_Edge_Color(HIC_Rendition const *nr, HIC_RGBAS32 internal) ;
/** @} */ /* end of edge */


/*!
  \addtogroup Set_Face_XXX
  @{
*/

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param rgb A pointer to a const HIC_RGB, a structure of three floats, each between 0 and 1.
 *
 * This function sets the cut face color.
 */
HC_EXTERNAL HIC_API void HIC_Set_Cut_Face_Color(HIC_Rendition const *nr, HIC_RGB const *rgb);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param npattern An int.
 *
 * This function changes the face pattern to the passed in int value.
 *
 * \sa HIC_New_User_Pattern
 */
HC_EXTERNAL HIC_API void HIC_Set_Face_Pattern(HIC_Rendition const *nr, int npattern);


/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param rgb A pointer to a const HIC_RGB, a structure of three floats, each between zero and one.
 *
 * This function changes the face color.
 */
HC_EXTERNAL HIC_API void HIC_Set_Face_Color(HIC_Rendition const *nr, HIC_RGB const *rgb);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param rgb A pointer to a const HIC_RGB, a structure of three floats, each between 0 and 1.
 *
 * This function changes the face mirror color.
 */
HC_EXTERNAL HIC_API void HIC_Set_Face_Mirror_Color(HIC_Rendition const *nr, HIC_RGB const *rgb);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param internal A HIC_RGBAS32 int.
 *
 * This function changes the internal face color.
 */
HC_EXTERNAL HIC_API void HIC_Set_Internal_Face_Color(HIC_Rendition const *nr, HIC_RGBAS32 internal);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param rgb A pointer to a HIC_RGB, a structure of three floats, each between 0 and 1.
 *
 * This function changes the face back color.
 */
HC_EXTERNAL HIC_API void HIC_Set_Face_Back_Color(HIC_Rendition const *nr, HIC_RGB const *rgb);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param internal A HIC_RGBAS32 int.
 *
 * This function changes the internal face back color.
 */
HC_EXTERNAL HIC_API void HIC_Set_Internal_Face_Back_Color(HIC_Rendition const *nr, HIC_RGBAS32 internal);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param rgb A pointer to a HIC_RGB,  a structure of three floats, each between 0 and 1.
 *
 * This function changes the face contrast color.
 */
HC_EXTERNAL HIC_API void HIC_Set_Face_Contrast_Color(HIC_Rendition const *nr, HIC_RGB const *rgb);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param internal A HIC_RGBAS32 int.
 *
 * This function changes the internal face contrast color.
 */
HC_EXTERNAL HIC_API void HIC_Set_Internal_Face_Contrast_Color(HIC_Rendition const *nr, HIC_RGBAS32 internal);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param rgb A pointer to a HIC_RGB, a structure of three floats, each between zero and one.
 *
 * This function changes the face transmission color.
 */
HC_EXTERNAL HIC_API void HIC_Set_Face_Transmission_Color(HIC_Rendition const *nr, HIC_RGB const *rgb);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param displacement A float.
 *
 * This function changes the face displacement using the passed in float value.
 */
HC_EXTERNAL HIC_API void HIC_Set_Face_Displacement(HIC_Rendition const *nr, float displacement);

/** @} */ /* end of face */

/*!
  \addtogroup General_Displacement
  @{
*/

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param displacement A float.
 *
 * This function sets the general displacement using the passed in float value.
 */
HC_EXTERNAL HIC_API void HIC_Set_General_Displacement(HIC_Rendition const *nr, float displacement);
/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A float.
 *
 * This function returns the general displacement as a float value.
 */
HC_EXTERNAL HIC_API float HIC_Show_General_Displacement(HIC_Rendition const *nr);
/** @} */ /* end of general displacement */

/*!
  \addtogroup Set_Marker_XXX
  @{
*/

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param nsymbol A HIC_RGBAS32 int.
 *
 * This function changes the marker symbol.
 */
HC_EXTERNAL HIC_API void HIC_Set_Marker_Symbol(HIC_Rendition const *nr, HIC_RGBAS32 nsymbol);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param nname A const char pointer, a user-defined glyph or marker symbol such as "<>".
 *
 * This function sets the marker symbol using the passed in string value.
 */
HC_EXTERNAL HIC_API void HIC_Set_Marker_Name(HIC_Rendition const *nr, const char *nname);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param nsize A float.
 *
 * This function changes the marker size to the passed in float value.
 */
HC_EXTERNAL HIC_API void HIC_Set_Marker_Size(HIC_Rendition const *nr, float nsize);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param nunits A HIC_Size_Units.
 * \param nsize A float.
 *
 * This function changes the marker size.
 */
HC_EXTERNAL HIC_API void HIC_Set_Variable_Marker_Size(HIC_Rendition const *nr, HIC_Size_Units nunits, float nsize);


/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param rgb A pointer to a HIC_RGB, a structure of three floats, each between zero and one.
 *
 * This function changes the marker color.
 */
HC_EXTERNAL HIC_API void HIC_Set_Marker_Color(HIC_Rendition const *nr, HIC_RGB const *rgb);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param rgb A pointer to a HIC_RGB, a structure of three floats, each between zero and one.
 *
 * This function changes the marker contrast color.
 */
HC_EXTERNAL HIC_API void HIC_Set_Marker_Contrast_Color(HIC_Rendition const *nr, HIC_RGB const *rgb);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param internal A HIC_RGBAS32 int.
 *
 * This function changes the internal marker color.
 */
HC_EXTERNAL HIC_API void HIC_Set_Internal_Marker_Color(HIC_Rendition const *nr, HIC_RGBAS32 internal);

/** @} */ /* end of set marker */


/*!
  \addtogroup Set_Text_XXX
  @{
*/

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param xfrm A HIC_Integer32 int.
 *
 * This function changes the text transform.
 */
HC_EXTERNAL HIC_API void HIC_Set_Text_Transform(HIC_Rendition const *nr, HIC_Integer32 xfrm);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param sz A float, the float form of the corresponding string parameter in HC_Set_Text_Font().
 *
 * This function changes the text size.
 */
HC_EXTERNAL HIC_API void HIC_Set_Text_Size(HIC_Rendition const *nr, HIC_Parameter sz);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param unts A HIC_Integer32 int.
 *
 * This function changes the text size units.
 */
HC_EXTERNAL HIC_API void HIC_Set_Text_Size_Units(HIC_Rendition const *nr, HIC_Integer32 unts);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param sz A float.
 *
 * This function changes the text size tolerance to the passed in float value.
 */
HC_EXTERNAL HIC_API void HIC_Set_Text_Size_Tolerance(HIC_Rendition const *nr, HIC_Parameter sz);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param unts An int.
 *
 * This function changes the text size tolerance units to the passed in int value.
 */
HC_EXTERNAL HIC_API void HIC_Set_Text_Size_Tolerance_Units(HIC_Rendition const *nr, HIC_Integer32 unts);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param spc A float, the float form of the corresponding string parameter of HC_Set_Text_Font().
 *
 * This function changes the text extra space attribute.
 */
HC_EXTERNAL HIC_API void HIC_Set_Text_Extra_Space(HIC_Rendition const *nr, HIC_Parameter  spc);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param unts An int.
 *
 * This function changes the text extra space untis attribute.
 */
HC_EXTERNAL HIC_API void HIC_Set_Text_Extra_Space_Units(HIC_Rendition const *nr, HIC_Integer32 unts);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param ali An int.
 *
 * This function sets the text alignment to the passed in int value.
 */
HC_EXTERNAL HIC_API void HIC_Set_Text_Alignment(HIC_Rendition const *nr, HIC_Integer32 ali);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param spc A float which has the same type and meaning as the corresponding argument to HC_Set_Text_Spacing().
 *
 * This function changes the text spacing attributes.
 */
HC_EXTERNAL HIC_API void HIC_Set_Text_Spacing(HIC_Rendition const *nr, HIC_Parameter spc);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param px A float.
 * \param py A float.
 * \param pz A float.
 *
 * This function changes text path attributes.
 */
HC_EXTERNAL HIC_API void HIC_Set_Text_Path(HIC_Rendition const *nr, float px, float py, float pz);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param rot A float, the float form of the corresponding string parameters in HC_Set_Text_Font().
 *
 * This function sets the text rotation.
 */
HC_EXTERNAL HIC_API void HIC_Set_Text_Rotation(HIC_Rendition const *nr, HIC_Parameter rot);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 *
 * This function sets the text rotation follow path.
 */
HC_EXTERNAL HIC_API void HIC_Set_Text_Rotation_Follow_Path(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param ws A float.
 *
 * This function sets the text width scale to the passed in float value.
 */
HC_EXTERNAL HIC_API void HIC_Set_Text_Width_Scale(HIC_Rendition const *nr, HIC_Parameter ws);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param sl A float.
 *
 * This function sets the text slant to the passed in float value.
 */
HC_EXTERNAL HIC_API void HIC_Set_Text_Slant(HIC_Rendition const *nr, HIC_Parameter sl);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param rgb A pointer to a HIC_RGB, a structure of three floats, each between zero and one.
 *
 * This function sets the text color.
 */
HC_EXTERNAL HIC_API void HIC_Set_Text_Color(HIC_Rendition const *nr, HIC_RGB const *rgb);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param rgb A pointer to a HIC_RGB, a structure of three floats, each between zero and one.
 *
 * This function sets the text contrast color.
 */
HC_EXTERNAL HIC_API void HIC_Set_Text_Contrast_Color(HIC_Rendition const *nr, HIC_RGB const *rgb);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param internal A HIC_RGBAS32 type.
 *
 * This function sets the internal text color.
 */
HC_EXTERNAL HIC_API void HIC_Set_Internal_Text_Color(HIC_Rendition const *nr, long internal);

/*!
 * \param iti A pointer to a const HIC_Text_Info.
 * \param p A pointer to a HIC_Point.
 *
 * This function allows you to change the device-coordinate position of a text string at the intermediate level of text rendering. 
 */
HC_EXTERNAL HIC_API void HIC_Set_Text_Position(HIC_Text_Info const *iti, HIC_Point const *p);

/*!
 * \param iti A pointer to a const HIC_Text_Info.
 * \param p A pointer to a HIC_Point.
 *
 * This function allows you to change the reference position of a text string at the intermediate level of text rendering. 
 */
HC_EXTERNAL HIC_API void HIC_Set_Text_Reference(HIC_Text_Info const *iti, HIC_Point const *p);

/*!
 * \param it A pointer to a const HIC_Text.
 * \param p A pointer to a HIC_Point.
 *
 * This function allows you to change the 3d coordinate position of the text string in the graphics database.  If this value is changed in a callback, it should be set back to the original value before the callback returns. 
 */
HC_EXTERNAL HIC_API void HIC_Set_3D_Text_Position(HIC_Text const *it, HIC_Point const *p);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param index An int.
 *
 * This function sets the index into the font table for the CGM3 driver.
 */
HC_EXTERNAL HIC_API void HIC_Set_Text_Font_Index(HIC_Rendition const *nr, int index);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param iti A pointer to a const HIC_Text_Info.
 * \param info A pointer to a const int.
 */
HC_EXTERNAL HIC_API void HIC_Set_Text_KIndex(HIC_Rendition const *nr, HIC_Text_Info const *iti, int const *index);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param iti A pointer to a const HIC_Text_Info.
 * \param info A pointer to a const HIC_Text_Karacter_Info.
 */
HC_EXTERNAL HIC_API void HIC_Set_Text_KInfo(HIC_Rendition const *nr, HIC_Text_Info const *iti, HIC_Text_Karacter_Info const *info);

/*!
 * \param iti A pointer to a const HIC_Text_Info.
 * \param p A int.
 *
 * This function allows you to set the text line index.
 */
HC_EXTERNAL HIC_API void HIC_Set_Text_Line_Index(HIC_Text_Info const *iti, int index);

/*!
* \param iti A pointer to a const HIC_Text_Info.
* \param txt A HIC_Karacter string.
*
* This function allows you to set the text string.
*/
HC_EXTERNAL HIC_API void HIC_Set_Text_String(HIC_Text_Info const *iti, HIC_Karacter const txt[]);

/*!
* \param iti A pointer to a const HIC_Text_Info.
* \param len An int.
*
* This function allows you to set the text string length.
*/
HC_EXTERNAL HIC_API void HIC_Set_Text_Length(HIC_Text_Info const *iti, int len);

/*!
 * \param info A pointer to a const HIC_Text_Karacter_Info.
 * \param p A pointer to a HIC_Point.
 *
 * This function allows you to change the Karacter_Info position of a text string at the intermediate level of text rendering. 
 */
HC_EXTERNAL HIC_API void HIC_Set_KInfo_Position(HIC_Text_Karacter_Info const *info, HIC_Point const *p);

/*!
 * \param info A pointer to a const HIC_Text_Karacter_Info.
 * \param p A pointer to a HIC_Point.
 *
 * This function allows you to change the Karacter_Info point of a text string at the intermediate level of text rendering. 
 */
HC_EXTERNAL HIC_API void HIC_Set_KInfo_Point(HIC_Text_Karacter_Info const *info, HIC_Point const *p);

/*!
 * \param info A pointer to a const HIC_Text_Karacter_Info.
 * \param width A float
 *
 * This function allows you to change the Karacter_Info string width.
 */
HC_EXTERNAL HIC_API void HIC_Set_KInfo_String_Width(HIC_Text_Karacter_Info const *info, float width);

/*!
 * \param info A pointer to a const HIC_Text_Karacter_Info.
 * \param height A float
 *
 * This function allows you to change the Karacter_Info string height.
 */
HC_EXTERNAL HIC_API void HIC_Set_KInfo_String_Height(HIC_Text_Karacter_Info const *info, float height);

/*!
 * \param info A pointer to a const HIC_Text_Karacter_Info.
 * \param x A float
 *
 * This function allows you to change the Karacter_Info string x.
 */
HC_EXTERNAL HIC_API void HIC_Set_KInfo_String_X(HIC_Text_Karacter_Info const *info, float x);

/*!
 * \param info A pointer to a const HIC_Text_Karacter_Info.
 * \param y A float
 *
 * This function allows you to change the Karacter_Info string y.
 */
HC_EXTERNAL HIC_API void HIC_Set_KInfo_String_Y(HIC_Text_Karacter_Info const *info, float y);

/*!
 * \param info A pointer to a const HIC_Text_Karacter_Info.
 * \param width A float
 *
 * This function allows you to change the Karacter_Info screen width.
 */
HC_EXTERNAL HIC_API void HIC_Set_KInfo_Screen_Width(HIC_Text_Karacter_Info const *info, float width);

/*!
 * \param info A pointer to a const HIC_Text_Karacter_Info.
 * \param height A float
 *
 * This function allows you to change the Karacter_Info screen height.
 */
HC_EXTERNAL HIC_API void HIC_Set_KInfo_Screen_Height(HIC_Text_Karacter_Info const *info, float height);

/*!
 * \param info A pointer to a const HIC_Text_Karacter_Info.
 * \param x A float
 *
 * This function allows you to change the Karacter_Info screen x.
 */
HC_EXTERNAL HIC_API void HIC_Set_KInfo_Screen_X(HIC_Text_Karacter_Info const *info, float x);

/*!
 * \param info A pointer to a const HIC_Text_Karacter_Info.
 * \param y A float
 *
 * This function allows you to change the Karacter_Info screen y.
 */
HC_EXTERNAL HIC_API void HIC_Set_KInfo_Screen_Y(HIC_Text_Karacter_Info const *info, float y);

/*!
 * \param info A pointer to a const HIC_Text_Karacter_Info.
 * \param flags A HIC_KInfo_Flags
 *
 * This function allows you to change the Karacter_Info flags.
 */
HC_EXTERNAL HIC_API void HIC_Set_KInfo_Flags(HIC_Text_Karacter_Info const *info, HIC_KInfo_Flags flags);

/** @} */ /* end of set text */


/*!
  \addtogroup Set_Vertex_XXX
  @{
*/


/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param nsymbol A HIC_RGBAS32 int.
 *
 * This function changes the vertex symbol.
 */
HC_EXTERNAL HIC_API void HIC_Set_Vertex_Symbol(HIC_Rendition const *nr, HIC_RGBAS32 nsymbol);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param nname A const char pointer, a user-defined glyph or marker symbol such as "<>".
 *
 * This function sets the vertex symbol using the passed in string value.
 */
HC_EXTERNAL HIC_API void HIC_Set_Vertex_Name(HIC_Rendition const *nr, const char *nname);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param nsize A float.
 *
 * This function changes the vertex size to the passed in float value.
 */
HC_EXTERNAL HIC_API void HIC_Set_Vertex_Size(HIC_Rendition const *nr, float nsize);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param nunits A HIC_Size_Units.
 * \param nsize A float.
 *
 * This function changes the vertex size.
 */
HC_EXTERNAL HIC_API void HIC_Set_Variable_Vertex_Size(HIC_Rendition const *nr, HIC_Size_Units nunits, float nsize);



/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param rgb A pointer to a HIC_RGB, a structure of three floats, each between zero and one.
 *
 * This function sets the vertex color.
 */
HC_EXTERNAL HIC_API void HIC_Set_Vertex_Color(HIC_Rendition const *nr, HIC_RGB const *rgb);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param rgb A pointer to a HIC_RGB, a structure of three floats, each between zero and one.
 *
 * This function sets the vertex contrast color.
 */
HC_EXTERNAL HIC_API void HIC_Set_Vertex_Contrast_Color(HIC_Rendition const *nr, HIC_RGB const *rgb);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param internal A HIC_RGBAS32 int.
 *
 * This function sets the internal vertex color.
 */
HC_EXTERNAL HIC_API void HIC_Set_Internal_Vertex_Color(HIC_Rendition const *nr, HIC_RGBAS32 internal);
/** @} */ /* end of set vertex */

/*!
  \addtogroup Set_Window_XXX
  @{
*/

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param npattern An int.
 *
 * This function sets the window pattern attribute.
 */
HC_EXTERNAL HIC_API void HIC_Set_Window_Pattern(HIC_Rendition const *nr, int npattern);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param upat A pointer to a const HIC_User_Face_Pattern.
 *
 * This function applies a user pattern to the window background.
 */
HC_EXTERNAL HIC_API void HIC_Set_User_Window_Pattern(HIC_Rendition const *nr, HIC_User_Face_Pattern const *upat);


/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param rgb A pointer to a HIC_RGB, a structure of three floats, each between zero and one.
 *
 * This function sets the window color attributes.
 */
HC_EXTERNAL HIC_API void HIC_Set_Window_Color(HIC_Rendition const *nr, HIC_RGB const *rgb);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param internal A list of RGBA values.
 *
 * This function sets the internal window color attribute.
 */
HC_EXTERNAL HIC_API void HIC_Set_Internal_Window_Color(HIC_Rendition const *nr, long internal);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param rgb A pointer to a const HIC_RGB structure of three floats, each between zero and one.
 *
 * This function sets the window contrast color.
 */
HC_EXTERNAL HIC_API void HIC_Set_Window_Contrast_Color(HIC_Rendition const *nr, HIC_RGB const *rgb);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param internal A list of RGBA values.
 *
 * This function sets the internal window contrast color.
 */
HC_EXTERNAL HIC_API void HIC_Set_Internal_Window_Contrast_Color(HIC_Rendition const *nr, long internal);
/** @} */ /* end of set window */

/*!
  \addtogroup Set_Line_XXX
  @{
*/

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param nweight An int.
 *
 * This function sets the line weight to the width in pixels using the passed in int value.
 */
HC_EXTERNAL HIC_API void HIC_Set_Line_Weight(HIC_Rendition const *nr, int nweight);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param nunit An HIC_GSU_UNITS.
 * \param nweight An float.
 *
 * This function sets the line weight to the width in pixels using the passed in int value.
 */
HC_EXTERNAL HIC_API void HIC_Set_Variable_Line_Weight(HIC_Rendition const *nr, HIC_Size_Units nunit, float nweight);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param npattern An int.
 *
 * This function changes the line pattern to the passed in int value.
 */
HC_EXTERNAL HIC_API void HIC_Set_Line_Pattern(HIC_Rendition const *nr, int npattern);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param nname A pointer to a const char, a user-defined line style or existing line pattern such as "dashdot".
 *
 * This function sets the line style using the passed in string value.
 */
HC_EXTERNAL HIC_API void HIC_Set_Line_Style(HIC_Rendition const *nr, const char *nname);


/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param upat A pointer to a HIC_Line_Style.
 *
 * This function sets the user line pattern.
 *
 * \sa HIC_New_User_Line_Pattern
 */
HC_EXTERNAL HIC_API void HIC_Set_User_Line_Pattern(HIC_Rendition const *nr, HIC_Line_Style const *upat);
/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param rgb A pointer to a HIC_RGB, a structure of three floats, each between zero and one.
 *
 * This function changes the line color.
 */
HC_EXTERNAL HIC_API void HIC_Set_Line_Color(HIC_Rendition const *nr, HIC_RGB const *rgb);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param rgb A pointer to a HIC_RGB, a structure of three floats, each between zero and one.
 *
 * This function changes the line contrast color.
 */
HC_EXTERNAL HIC_API void HIC_Set_Line_Contrast_Color(HIC_Rendition const *nr, HIC_RGB const *rgb);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param internal A HIC_RGBAS32 type.
 *
 * This function changes the internal line color.
 */
HC_EXTERNAL HIC_API void  HIC_Set_Internal_Line_Color(HIC_Rendition const *nr, HIC_RGBAS32 internal);
/** @} */ /* end of set_line */

/*!
  \addtogroup Set_LOD
  @{
*/

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param clampval An int.
 *
 * This function sets a level of detail (LOD) clamp value.
 */
HC_EXTERNAL HIC_API void HIC_Set_LOD_Clamp(HIC_Rendition const *nr, int clampval);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 *
 * This function returns the level of detail (LOD) clamp value.
 */
HC_EXTERNAL HIC_API int HIC_Show_LOD_Clamp(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 *
 * This function enables the level of detail module for rendering.
 */
HC_EXTERNAL HIC_API void HIC_LOD_Enable(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 *
 * This function disables the level of detail module for rendering.
 */
HC_EXTERNAL HIC_API void HIC_LOD_Disable(HIC_Rendition const *nr);

/** @} */ /* end of set_lod */


/*!
  \addtogroup Set_Modelling_Matrix_XXX
  @{
*/

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param fp A pointer to a const float.
 *
 * This function provides animation.
 */
HC_EXTERNAL HIC_API void  HIC_Set_Modelling_Matrix(HIC_Rendition const *nr, float const *fp);
/** @} */ /* end of set_modelling_matrix_XXX */


/*! \addtogroup Set_Polyedge_XXX
*  @{
*/
/*!
 * \param pe A pointer to a const HIC_Polyedge.
 * \param vnormals A pointer to a HIC_Vector.
 *
 * This function sets vertex normals of a HOOPS polyedge data structure.
 */
HC_EXTERNAL HIC_API void HIC_Set_Polyedge_Vertex_Normals(HIC_Polyedge const *pe, HIC_Vector const *vnormals);

/*!
 * \param pe A pointer to a const HIC_Polyedge.
 * \param vparameters A pointer to a HIC_Parameter.
 *
 * This function sets vertex parameters of a HOOPS polyedge data structure.
 */
HC_EXTERNAL HIC_API void HIC_Set_Polyedge_Vertex_Parameters(HIC_Polyedge const *pe, HIC_Parameter const *vparameters);
/*!
 * \param pe A pointer to a const HIC_Polyedge.
 * \param flag A boolean.
 *
 * This function sets findex color interpolation of a HOOPS polyedge data structure.
 */
HC_EXTERNAL HIC_API void HIC_Set_Polyedge_Findex_Color_Interpolation(HIC_Polyedge const *pe, bool flag);

/*!
 * \param pe A pointer to a const HIC_Polyedge.
 * \param width An HIC_Integer32.
 *
 * This function sets vertex parameter width of a HOOPS polyedge data structure.
 */
HC_EXTERNAL HIC_API void HIC_Set_Polyedge_Vertex_Parameter_Width(HIC_Polyedge const *pe, HIC_Integer32 width);

/*!
 * \param pe A pointer to a const HIC_Polyedge.
 * \param pts A pointer to a HIC_Point.
 *
 * This function sets points of a HOOPS polyedge data structure.
 */
HC_EXTERNAL HIC_API void HIC_Set_Polyedge_Points(HIC_Polyedge const *pe, HIC_Point const *pts);

/*!
 * \param pe A pointer to a const HIC_Polyedge.
 * \param str An int.
 *
 * This function sets strips of a HOOPS polyedge data structure.
 */
HC_EXTERNAL HIC_API void HIC_Set_Polyedge_Strips(HIC_Polyedge const *pe, int str);

/*!
 * \param pe A pointer to a const HIC_Polyedge.
 * \param lns An int pointer.
 *
 * This function sets lengths of a HOOPS polyedge data structure.
 */
HC_EXTERNAL HIC_API void HIC_Set_Polyedge_Lengths(HIC_Polyedge const *pe, int const *lns);


/*!
 * \param pe A pointer to a const HIC_Polyedge.
 * \param vi An int pointer.
 *
 * This function sets vertex indices of a HOOPS polyedge data structure.
 */
HC_EXTERNAL HIC_API void HIC_Set_Polyedge_Vertex_Indices(HIC_Polyedge const *pe, int const *vi);
/*!
 * \param pe A pointer to a const HIC_Polyedge.
 * \param enormals A pointer to a HIC_Vector.
 *
 * This function sets edge normals of a HOOPS polyedge data structure.
 */
HC_EXTERNAL HIC_API void HIC_Set_Polyedge_Edge_Normals(HIC_Polyedge const *pe, HIC_Vector const *enormals);

/*!
 * \param pe A pointer to a const HIC_Polyedge.
 * \param vfindices A float pointer.
 *
 * This function sets vertex findices of a HOOPS polyedge data structure.
 */
HC_EXTERNAL HIC_API void HIC_Set_Polyedge_Vertex_Findices(HIC_Polyedge const *pe, float const *vfindices);
/*!
 * \param pe A pointer to a const HIC_Polyedge.
 * \param ei An int pointer.
 *
 * This function sets edge indices of a HOOPS polyedge data structure.
 */
HC_EXTERNAL HIC_API void HIC_Set_Polyedge_Edge_Indices(HIC_Polyedge const *pe, int const *ei);
/*!
 * \param pe A pointer to a const HIC_Polyedge.
 * \param ecolors A pointer to a HIC_RGB, a structure of three floats, each between zero and one.
 *
 * This function sets edge colors of a HOOPS polyedge data structure.
 */
HC_EXTERNAL HIC_API void HIC_Set_Polyedge_Edge_Colors(HIC_Polyedge const *pe, HIC_RGB const *ecolors);

/*!
 * \param pe A pointer to a const HIC_Polyedge.
 * \param vcolors A pointer to a HIC_RGB, a structure of three floats, each between zero and one.
 *
 * This function sets vertex colors of a HOOPS polyedge data structure.
 */
HC_EXTERNAL HIC_API void HIC_Set_Polyedge_Vertex_Colors(HIC_Polyedge const *pe, HIC_RGB const *vcolors);
/*! @} */ /* end of set_poly */

/*!
  \addtogroup Show_PolyEdge_XXX
  @{
*/

/*!
 * \param pe A pointer to a const HIC_Polyedge.
 * \return A pointer to a const HIC_Point.
 *
 * This function returns polyedge points.		
 */
HC_EXTERNAL HIC_API HIC_Point const * HIC_Show_Polyedge_Points(HIC_Polyedge const *pe);

/*!
 * \param pe A pointer to a const HIC_Polyedge.
 * \return A pointer to a const HIC_Vector.
 *
 * This function returns polyedge vertex normals.		
 */
HC_EXTERNAL HIC_API HIC_Vector const * HIC_Show_Polyedge_Vertex_Normals(HIC_Polyedge const *pe);
/*!
 * \param pe A pointer to a const HIC_Polyedge.
 * \return A const int pointer.
 *
 * This function returns polyedge lengths.
 */
HC_EXTERNAL HIC_API int const * HIC_Show_Polyedge_Lengths(HIC_Polyedge const *pe);

/*!
 * \param pe A pointer to a const HIC_Polyedge.
 * \return A pointer to a const HIC_Parameter.
 *
 * This function returns polyedge vertex parameters.		
 */
HC_EXTERNAL HIC_API HIC_Parameter const * HIC_Show_Polyedge_Vertex_Parameters(HIC_Polyedge const *pe);

/*!
 * \param pe A pointer to a const HIC_Polyedge.
 * \return A HIC_Integer32.
 *
 * This function returns the polyedge vertex parameter width.
 */
HC_EXTERNAL HIC_API HIC_Integer32 HIC_Show_Polyedge_Vertex_Parameter_Width(HIC_Polyedge const *pe);

/*!
 * \param pe A pointer to a const HIC_Polyedge.
 * \return A boolean.
 *
 * This function returns polyedge findex color interpolation as a boolean.		
 */
HC_EXTERNAL HIC_API bool HIC_Show_Polyedge_Findex_Color_Interpolation(HIC_Polyedge const *pe);

/*!
 * \param pe A pointer to a const HIC_Polyedge.
 * \return A const int pointer.
 *
 * This function returns polyedge vertex indices.
 */
HC_EXTERNAL HIC_API int const * HIC_Show_Polyedge_Vertex_Indices(HIC_Polyedge const *pe);
/*!
 * \param pe A pointer to a const HIC_Polyedge.
 * \return An int.
 *
 * This function returns the number of points in a polyedge as an int.		
 */
HC_EXTERNAL HIC_API int HIC_Show_Polyedge_Point_Count(HIC_Polyedge const *pe);

/*!
 * \param pe A pointer to a const HIC_Polyedge.
 * \return An int.
 *
 * This function returns the number of edges in a polyedge as an int.		
 */

HC_EXTERNAL HIC_API int HIC_Show_Polyedge_Edge_Count(HIC_Polyedge const *pe);

/*!
 * \param pe A pointer to a const HIC_Polyedge.
 * \return An int.
 *
 * This function returns polyedge strips as an int.
 */
HC_EXTERNAL HIC_API int HIC_Show_Polyedge_Strips(HIC_Polyedge const *pe);

/*!
 * \param pe A pointer to a const HIC_Polyedge.
 * \return A const int pointer.
 *
 * This function returns polyedge edge indices.
 */
HC_EXTERNAL HIC_API int const * HIC_Show_Polyedge_Edge_Indices(HIC_Polyedge const *pe);

/*!
 * \param pe A pointer to a const HIC_Polyedge.
 * \return A pointer to a const HIC_RGB, a structure of three floats, each between zero and one.
 *
 * This function returns edge colors of a HOOPS polyedge data structure.
 */
HC_EXTERNAL HIC_API HIC_RGB const * HIC_Show_Polyedge_Edge_Colors(HIC_Polyedge const *pe);


/*!
 * \param pe A pointer to a const HIC_Polyedge.
 * \return A pointer to a const HIC_Vector.
 *
 * This function returns polyedge edge normals.
 */
HC_EXTERNAL HIC_API HIC_Vector const * HIC_Show_Polyedge_Edge_Normals(HIC_Polyedge const *pe);

/*!
 * \param pe A pointer to a const HIC_Polyedge.
 * \return A pointer to a const HIC_RGB, a structure of three floats, each between zero and one.
 *
 * This function returns polyedge vertex colors.		
 */
HC_EXTERNAL HIC_API HIC_RGB const * HIC_Show_Polyedge_Vertex_Colors(HIC_Polyedge const *pe);


/*!
 * \param pe A pointer to a const HIC_Polyedge.
 * \return A pointer to a const float.	
 *
 * This function returns polyedge vertex findices.	
 */
HC_EXTERNAL HIC_API float const * HIC_Show_Polyedge_Vertex_Findices(HIC_Polyedge const *pe);
/*! @} */ /* end of show_poly */

/*!
  \addtogroup Show_Polymarker_XXX
  @{
*/
/*!
 * \param pm A pointer to a const HIC_Polymarker.
 * \return A pointer to a const HIC_Point.
 *
 * This function returns polymarker points.			
 */
HC_EXTERNAL HIC_API HIC_Point const * HIC_Show_Polymarker_Points(HIC_Polymarker const *pm);
/*!
 * \param pm A pointer to a const HIC_Polymarker.
 * \return An int.
 *
 * This function returns the number of points in a polymarker as an int.			
 */
HC_EXTERNAL HIC_API int HIC_Show_Polymarker_Point_Count(HIC_Polymarker const *pm);

/*!
 * \param pm A pointer to a const HIC_Polymarker.
 * \return An int.
 *
 * This function returns the polymarker length as an int.
 */
HC_EXTERNAL HIC_API int HIC_Show_Polymarker_Length(HIC_Polymarker const *pm);
/*!
 * \param pm A pointer to a const HIC_Polymarker.
 * \return A const int pointer.
 *
 * This function returns the polymarker vertex indices.
 */
HC_EXTERNAL HIC_API int const * HIC_Show_Polymarker_Vertex_Indices(HIC_Polymarker const *pm);
/*!
 * \param pm A pointer to a const HIC_Polymarker.
 * \return A pointer to a const HIC_RGB, a structure of three floats, each between zero and one.
 *
 * This function returns the polymarker vertex colors as a structure of three floats, all between zero and one.			
 */
HC_EXTERNAL HIC_API HIC_RGB const * HIC_Show_Polymarker_Vertex_Colors(HIC_Polymarker const *pm);
/*!
 * \param pm A pointer to a const HIC_Polymarker.
 * \return A pointer to a const float.
 *
 * This function returns the polymarker vertex findices.			
 */
HC_EXTERNAL HIC_API float const * HIC_Show_Polymarker_Vertex_Findices(HIC_Polymarker const *pm);

/*!
 * \param pm A pointer to a const HIC_Polymarker.
 * \return A pointer to a const HIC_Vector.
 *
 * This function returns the polymarker vertex normals.			
 */
HC_EXTERNAL HIC_API HIC_Vector const * HIC_Show_Polymarker_Vertex_Normals(HIC_Polymarker const *pm);
/*!
 * \param pm A pointer to a const HIC_Polymarker.
 * \return A pointer to a const HIC_Parameter.
 *
 * This function returns the polymarker vertex parameters.
 */
HC_EXTERNAL HIC_API HIC_Parameter const * HIC_Show_Polymarker_Vertex_Parameters(HIC_Polymarker const *pm);

/*!
 * \param pm A pointer to a const HIC_Polymarker.
 * \return A HIC_Integer32.
 *
 * This function returns polymarker vertex parameter width.
 */
HC_EXTERNAL HIC_API HIC_Integer32 HIC_Show_Polymarker_Vertex_Parameter_Width(HIC_Polymarker const *pm);

/*!
 * \param pm A pointer to a const HIC_Polymarker.
 * \return A boolean.
 *
 * This function returns polymarker findex color interpolation as a boolean.
 *
 * \sa HIC_Set_Polymarker_Findex_Color_Interpolation
 */
HC_EXTERNAL HIC_API bool HIC_Show_Polymarker_Findex_Color_Interpolation(HIC_Polymarker const *pm);
/*! @} */ /* end of show_poly */

/*!
  \addtogroup Set_Polymarker_XXX
  @{
*/

/*!
 * \param pm A pointer to a const HIC_Polymarker.
 * \param pts A pointer to a HIC_Point, the first in an array of 3D points that contains the vertices of all the polymarkers in the list.
 *
 * This function sets a HOOPS polymarker data structure.
 */
HC_EXTERNAL HIC_API void HIC_Set_Polymarker_Points(HIC_Polymarker const *pm, HIC_Point const pts[]);

/*!
 * \param pm A pointer to a const HIC_Polymarker.
 * \param len An int.
 *
 * This function sets the polymarker length.
 */
HC_EXTERNAL HIC_API void HIC_Set_Polymarker_Length(HIC_Polymarker const *pm, int len);

/*!
 * \param pm A pointer to a const HIC_Polymarker.
 * \param vi An int pointer.
 *
 * This function sets the polymarker vertex indices.
 */
HC_EXTERNAL HIC_API void HIC_Set_Polymarker_Vertex_Indices(HIC_Polymarker const *pm, int const *vi);
/*!
 * \param pm A pointer to a const HIC_Polymarker.
 * \param vcolors A HIC_RGB, a structure of three floats, each between zero and one.
 *
 * This function sets vertex colors of a HOOPS polymarker data structure.
 */
HC_EXTERNAL HIC_API void HIC_Set_Polymarker_Vertex_Colors(HIC_Polymarker const *pm, HIC_RGB const *vcolors);
/*!
 * \param pm A pointer to a const HIC_Polymarker.
 * \param vfindices A pointer to a float.
 *
 * This funciton sets vertex findices of a HOOPS polymarker data structure.
 */
HC_EXTERNAL HIC_API void HIC_Set_Polymarker_Vertex_Findices(HIC_Polymarker const *pm, float const *vfindices);

/*!
 * \param pm A pointer to a const HIC_Polymarker.
 * \param vnormals A pointer to a HIC_Vector.
 *
 * This function sets vertex normals of a HOOPS polymarker data structure.
 */
HC_EXTERNAL HIC_API void HIC_Set_Polymarker_Vertex_Normals(HIC_Polymarker const *pm, HIC_Vector const *vnormals);

/*!
 * \param pm A pointer to a const HIC_Polymarker.
 * \param vparameters A pointer to a HIC_Parameter.
 *
 * This function sets vertex parameters of a HOOPS polymarker data structure.
 */
HC_EXTERNAL HIC_API void HIC_Set_Polymarker_Vertex_Parameters(HIC_Polymarker const *pm, HIC_Parameter const *vparameters);


/*!
 * \param pm A pointer to a const HIC_Polymarker.
 * \param width A HIC_Integer32.
 *
 * This function sets vertex parameter width of a HOOPS polymarker data structure.
 */
HC_EXTERNAL HIC_API void HIC_Set_Polymarker_Vertex_Parameter_Width(HIC_Polymarker const *pm, HIC_Integer32 width);

/*!
 * \param pm A pointer to a const HIC_Polymarker.
 * \param flag A boolean.
 *
 * This function sets findex color interpolation of a polymarker data structure.
 */
HC_EXTERNAL HIC_API void HIC_Set_Polymarker_Findex_Color_Interpolation(HIC_Polymarker const *pm, bool flag);

/*! @} */ /* end of set_poly */

/*!
  \addtogroup Set_Tristrip_XXX
  @{
*/

/*!
 * \param ts A pointer to a const HIC_Tristrip.
 * \param pts A pointer to a HIC_Point.
 *
 * This function sets points of a HOOPS tristrip data structure.
 */
HC_EXTERNAL HIC_API void HIC_Set_Tristrip_Points(HIC_Tristrip const *ts, HIC_Point const *pts);
/*!
 * \param ts A pointer to a const HIC_Tristrip.
 * \param str An int.
 *
 * This function sets the strips of a HOOPS tristrip data structure.
 */
HC_EXTERNAL HIC_API void HIC_Set_Tristrip_Strips(HIC_Tristrip const *ts, int str);
/*!
 * \param ts A pointer to a const HIC_Tristrip.
 * \param lns An int pointer.
 *
 * This function sets lengths on a HOOPS tristrip data structure.
 */
HC_EXTERNAL HIC_API void HIC_Set_Tristrip_Lengths(HIC_Tristrip const *ts, int const *lns);
/*!
 * \param ts A pointer to a const HIC_Tristrip.
 * \param vi An array of int.
 *
 * This function sets vertex indices of a HOOPS tristrip data structure.
 */
HC_EXTERNAL HIC_API void HIC_Set_Tristrip_Vertex_Indices(HIC_Tristrip const *ts, int const *vi);
/*!
 * \param ts A pointer to a const HIC_Tristrip.
 * \param fi An int pointer.
 *
 * This function sets face indices of a HOOPS tristrip data structure.
 */
HC_EXTERNAL HIC_API void HIC_Set_Tristrip_Face_Indices(HIC_Tristrip const *ts, int const *fi);


/*!
 * \param ts A pointer to a const HIC_Tristrip.
 * \param fcolors A pointer to a HIC_RGB, a structure of three floats, each between zero and one.
 *
 * This function sets face colors of a HOOPS tristrip data structure.
 */
HC_EXTERNAL HIC_API void HIC_Set_Tristrip_Face_Colors(HIC_Tristrip const *ts, HIC_RGB const *fcolors);
/*!
 * \param ts A pointer to a const HIC_Tristrip.
 * \param fnormals A pointer to a HIC_Plane.
 *
 * This function sets face normals of a HOOPS tristrip data structure.
 */
HC_EXTERNAL HIC_API void HIC_Set_Tristrip_Face_Normals(HIC_Tristrip const *ts, HIC_Plane const *fnormals);


/*!
 * \param ts A pointer to a const HIC_Tristrip.
 * \param vcolors A pointer to a HIC_RGB, a structure of three floats, each between zero and one.
 *
 * This functiony sets vertex colors of a HOOPS tristrip data structure.
 */
HC_EXTERNAL HIC_API void HIC_Set_Tristrip_Vertex_Colors(HIC_Tristrip const *ts, HIC_RGB const *vcolors);
/*!
 * \param ts A pointer to a const HIC_Tristrip.
 * \param vfindices A float pointer.
 *
 * This function sets vertex findices of a HOOPS tristrip data structure.
 */
HC_EXTERNAL HIC_API void HIC_Set_Tristrip_Vertex_Findices(HIC_Tristrip const *ts, float const *vfindices);

/*!
 * \param ts A pointer to a const HIC_Tristrip.
 * \param vnormals A pointer to a HIC_Vector.
 *
 * This function sets vertex normals of a HOOPS tristrip data structure.
 */
HC_EXTERNAL HIC_API void HIC_Set_Tristrip_Vertex_Normals(HIC_Tristrip const *ts, HIC_Vector const *vnormals);

/*!
 * \param ts A pointer to a const HIC_Tristrip.
 * \param vparameters A pointer to a const HIC_Parameter.
 *
 * This function sets vertex parameters of a HOOPS tristrip data structure.
 */
HC_EXTERNAL HIC_API void HIC_Set_Tristrip_Vertex_Parameters(HIC_Tristrip const *ts, HIC_Parameter const *vparameters);
/*!
 * \param ts A pointer to a const HIC_Tristrip.
 * \param flag A boolean.
 *
 * This function sets findex color interpolation of a HOOPS tristrip data structure.
 */
HC_EXTERNAL HIC_API void HIC_Set_Tristrip_Findex_Color_Interpolation(HIC_Tristrip const *ts, bool flag);

/*!
 * \param ts A pointer to a const HIC_Tristrip.
 * \param width An int.
 *
 * This function sets vertex parameter width of a HOOPS tristrip data structure.
 */
HC_EXTERNAL HIC_API void HIC_Set_Tristrip_Vertex_Parameter_Width(HIC_Tristrip const *ts, int width);
/** @} */ /* end of set_tri */

/* user options */

/*!
  \addtogroup User_Options
  @{
*/

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param count An int.
 * \param keys An HC_KEY array.
 * \param uoname The desired option string.
 * \param options The output buffer.
 *
 * This function fills in the option for the given path, if it exists and return true.
 */
HC_EXTERNAL HIC_API bool HIC_PShow_One_Net_User_Option(HIC_Rendition const *nr, int count, HC_KEY const keys[], char const * uoname, char * options);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param key An HC_KEY.
 * \param uoname The desired option string.
 * \param options The output buffer.
 *
 * This function fills in the option for the given path, if it exists and return true.
 */
HC_EXTERNAL HIC_API bool HIC_Show_One_User_Option_By_Key(HIC_Rendition const *nr, HC_KEY key, char const * uoname, char * options);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param count An int.
 * \param keys An HC_KEY array.
 * \param index The desired data index.
 * \param data The output buffer.
 * \param data_length The output buffer length.
 * \param data_length_out The length of the data returned.
 *
 * This function fills in the data for the given path, if it doesn't exists data_length_out will be zero.
 */
HC_EXTERNAL HIC_API void HIC_PShow_One_Net_User_Data(HIC_Rendition const *nr, int count, HC_KEY const keys[], HC_POINTER_SIZED_INT index, void * data, long data_length, long * data_length_out);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param key An HC_KEY.
 * \param index The desired data index.
 * \param data The output buffer.
 * \param data_length The output buffer length.
 * \param data_length_out The length of the data returned.
 *
 * This function fills in the data for the given path,  if it doesn't exists data_length_out will be zero.
 */
HC_EXTERNAL HIC_API void HIC_Show_One_User_Data_By_Key(HIC_Rendition const *nr, HC_KEY key, HC_POINTER_SIZED_INT index, void * data, long data_length, long * data_length_out);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param index An int.
 *
 * This function returns the option value for the given index, if it exists, otherwise returns null.
 */
HC_EXTERNAL HIC_API void * HIC_Show_User_Index(HIC_Rendition const *nr, int index);
/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param index An int.
 * \param value A pointer to a const char.
 * \return A void pointer.
 *
 * This function sets the value of the user option corresponding to the given index, if it exists, 
 * otherwise it is created and inserted into the user option-by-index list.
 */
HC_EXTERNAL HIC_API void * HIC_Set_User_Index(HIC_Rendition const *nr, int index, const void *value);
/* for compatibility */

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param index An int.
 * \return A void pointer.
 *
 * This function gets the user option by index.
 */
HC_EXTERNAL HIC_API void * HIC_Show_User_Option_By_Index(HIC_Rendition const *nr, int index);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param index An int.
 * \param value A pointer to a const char.
 * \return A void pointer.
 *
 * This function sets user option by index.
 */
HC_EXTERNAL HIC_API void * HIC_Set_User_Option_By_Index(HIC_Rendition const *nr, int index, const char *value);

/** @} */ /* end of user_opt */


/*!
  \addtogroup Visibility
  @{
*/

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param vis A HIC_Type_Flags. XXX -missing type-
 *
 * This function provides view-dependent geometry visibilities.
 */
HC_EXTERNAL HIC_API void  HIC_Set_Visibility(HIC_Rendition const *nr, long vis);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return An int.
 * 
 * This routine is used to obtain the existing bit values for visibility.
 */
HC_EXTERNAL HIC_API int HIC_Show_Visibility(HIC_Rendition const *nr);
/** @} */ /* end of visibility */


/*!
  \addtogroup Selectability
  @{
*/

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param sel A HIC_Type_Flags. XXX -missing type-
 *
 * This function provides view-dependent geometry selectabilities.
 */
HC_EXTERNAL HIC_API void  HIC_Set_Selectability(HIC_Rendition const *nr, long sel);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return An int.
 * 
 * This routine is used to obtain the existing bit values for selectability.
 */
HC_EXTERNAL HIC_API int HIC_Show_Selectability(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param sel A HIC_Type_Flags. XXX -missing type-
 *
 * This function provides view-dependent geometry selectabilities for invisible items.
 */
HC_EXTERNAL HIC_API void  HIC_Set_Selectability_Even_Invisible(HIC_Rendition const *nr, long sel);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return An int.
 * 
 * This routine is used to obtain the existing bit values for invisible item selectability.
 */
HC_EXTERNAL HIC_API int HIC_Show_Selectability_Even_Invisible(HIC_Rendition const *nr);
/** @} */ /* end of visibility */


/*!
  \addtogroup Set_Z_Range
  @{
*/
/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param _near_ A float such that 0 <= _near_ <= _far_.
 * \param _far_ A float such that _near_ <= _far_ <= 1.
 *
 * This function sets the Z range.
 */
HC_EXTERNAL HIC_API void HIC_Set_Z_Range(HIC_Rendition const *nr, float _near_, float _far_);
/** @} */ /* end of z range */

/* callback information */

/*!
  \addtogroup Show_Callback_Data
  @{
*/
/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A pointer to a void const.
 *
 * This function returns callback data.
 *
 * \sa HIC_Set_Callback_With_Data
 */
HC_EXTERNAL HIC_API void const * HIC_Show_Callback_Data(HIC_Rendition const *nr);
/** @} */ /* end of Show_Callback_Data */

/*!
  \addtogroup Set_Cylinder_XXX
  @{
*/
/*!
 * \param geo A pointer to a const HIC_Geometry.
 * \param radius A float, value to set the cylinder radius to.
 *
 * This function sets the cylinder radius to the float value of the passed in parameter.
 */
HC_EXTERNAL HIC_API void HIC_Set_Cylinder_Radius(HIC_Geometry const *geo, float radius);

/*!
 * \param geo A pointer to a const HIC_Geometry.
 * \param v A HIC_Vector *, value to set the cylinder basis axis 0 to.
 *
 * This function sets the cylinder basis axis 0 to the HIC_Vector value of the passed in parameter.
 */
HC_EXTERNAL HIC_API void HIC_Set_Cylinder_Basis_Axis0(HIC_Geometry const *geo, HIC_Vector const *v);

/*!
 * \param geo A pointer to a const HIC_Geometry.
 * \param v A HIC_Vector *, value to set the cylinder basis axis 1 to.
 *
 * This function sets the cylinder basis axis 1 to the HIC_Vector value of the passed in parameter.
 */
HC_EXTERNAL HIC_API void HIC_Set_Cylinder_Basis_Axis1(HIC_Geometry const *geo, HIC_Vector const *v);

/*!
 * \param geo A pointer to a const HIC_Geometry.
 * \param p A HIC_Point *, value to set the cylinder axis 0 to.
 *
 * This function sets the cylinder axis 0 to the HIC_Point value of the passed in parameter.
 */
HC_EXTERNAL HIC_API void HIC_Set_Cylinder_Axis0(HIC_Geometry const *geo, HIC_Point const *p);

/*!
 * \param geo A pointer to a const HIC_Geometry.
 * \param p A HIC_Point *, value to set the cylinder axis 1 to.
 *
 * This function sets the cylinder axis 1 to the HIC_Point value of the passed in parameter.
 */
HC_EXTERNAL HIC_API void HIC_Set_Cylinder_Axis1(HIC_Geometry const *geo, HIC_Point const *p);

/** @} */ /* end of Set_Cylinder_XXX */

/*!
  \addtogroup Show_Cylinder_XXX
  @{
*/
/*!
 * \param geo A pointer to a const HIC_Geometry.
 * \return A float.
 *
 * This function returns the cylinder radius as a float.
 */
HC_EXTERNAL HIC_API float HIC_Show_Cylinder_Radius(HIC_Geometry const *geo);

/*!
 * \param geo A pointer to a const HIC_Geometry.
 * \return A HIC_Vector const *.
 *
 * This function returns the cylinder basis axis 0 as a HIC_Vector const *.
 */
HC_EXTERNAL HIC_API HIC_Vector const * HIC_Show_Cylinder_Basis_Axis0(HIC_Geometry const *geo);

/*!
 * \param geo A pointer to a const HIC_Geometry.
 * \return A HIC_Vector const *.
 *
 * This function returns the cylinder basis axis 1 as a HIC_Vector const *.
 */
HC_EXTERNAL HIC_API HIC_Vector const * HIC_Show_Cylinder_Basis_Axis1(HIC_Geometry const *geo);

/*!
 * \param geo A pointer to a const HIC_Geometry.
 * \return A HIC_Point const *.
 *
 * This function returns the cylinder axis 0 as a HIC_Point const *.
 */
HC_EXTERNAL HIC_API HIC_Point const * HIC_Show_Cylinder_Axis0(HIC_Geometry const *geo);

/*!
 * \param geo A pointer to a const HIC_Geometry.
 * \return A HIC_Point const *.
 *
 * This function returns the cylinder axis 1 as a HIC_Point const *.
 */
HC_EXTERNAL HIC_API HIC_Point const * HIC_Show_Cylinder_Axis1(HIC_Geometry const *geo);

/** @} */ /* end of Show_Cylinder_XXX */


/*!
  \addtogroup Set_PolyCylinder_XXX
  @{
*/
/*!
 * \param geo A pointer to a const HIC_Geometry.
 * \param basis_index An int, value of the basis index to set.
 * \param v A HIC_Vector *, value to set the polycylinder basis axis 0 to.
 *
 * This function sets the polycylinder basis (at basis_index) axis 0 to the HIC_Vector value of the passed in parameter.
 */
HC_EXTERNAL HIC_API void HIC_Set_PolyCylinder_Basis_Axis0(HIC_Geometry const *geo, int basis_index, HIC_Vector const *v);

/*!
 * \param geo A pointer to a const HIC_Geometry.
 * \param basis_index An int, index value of the basis to set.
 * \param v A HIC_Vector *, value to set the polycylinder basis axis 1 to.
 *
 * This function sets the polycylinder basis (at basis_index) axis 1 to the HIC_Vector value of the passed in parameter.
 */
HC_EXTERNAL HIC_API void HIC_Set_PolyCylinder_Basis_Axis1(HIC_Geometry const *geo, int basis_index, HIC_Vector const *v);

/*!
 * \param geo A pointer to a const HIC_Geometry.
 * \param index An int, index value of the point to set.
 * \param p A HIC_Point *, value to set the polycylinder point at index to.
 *
 * This function sets the polycylinder point (at index) to the HIC_Point value of the passed in parameter.
 */
HC_EXTERNAL HIC_API void HIC_Set_PolyCyliner_Points(HIC_Geometry const *geo, int index, HIC_Point const *p);

/*!
 * \param geo A pointer to a const HIC_Geometry.
 * \param index An int, index value of the radii to set.
 * \param r A float, value to set the polycylinder radius at index to.
 *
 * This function sets the polycylinder radius (at index) to the float value of the passed in parameter.
 */
HC_EXTERNAL HIC_API void HIC_Set_PolyCylinder_Radii(HIC_Geometry const *geo, int index, float r);


/** @} */ /* end of Set_PolyCylinder_XXX */

/*!
  \addtogroup Show_PolyCylinder_XXX
  @{
*/
/*!
 * \param geo A pointer to a const HIC_Geometry.
 * \param basis_index An int, value at the basis index to return.
 * \return A HIC_Vector const *.
 *
 * This function returns the polycylinder basis axis 0 at the specified basis index as a HIC_Vector const *.
 */
HC_EXTERNAL HIC_API HIC_Vector const * HIC_Show_PolyCylinder_Basis_Axis0(HIC_Geometry const *geo, int basis_index);

/*!
 * \param geo A pointer to a const HIC_Geometry.
 * \param basis_index An int, value at the basis index to return.
 * \return A HIC_Vector const *.
 *
 * This function returns the polycylinder basis axis 1 at the specified basis index as a HIC_Vector const *.
 */
HC_EXTERNAL HIC_API HIC_Vector const * HIC_Show_PolyCylinder_Basis_Axis1(HIC_Geometry const *geo, int basis_index);

/*!
 * \param geo A pointer to a const HIC_Geometry.
 * \param basis_index An int, point of the polycylinder to return.
 * \return A HIC_Point const *.
 *
 * This function returns the polycylinder point at the specified index as a HIC_Point const *.
 */
HC_EXTERNAL HIC_API HIC_Point const * HIC_Show_PolyCylinder_Points(HIC_Geometry const *geo, int index);

/*!
 * \param geo A pointer to a const HIC_Geometry.
 * \param basis_index An int, radius of the polycylinder to return.
 * \return A float.
 *
 * This function returns the polycylinder radius at the specified index as a float.
 */
HC_EXTERNAL HIC_API float HIC_Show_PolyCylinder_Radii(HIC_Geometry const *geo, int index);

/*!
 * \param geo A pointer to a const HIC_Geometry.
 * \return An int.
 *
 * This function returns the polycylinder point count.
 */
HC_EXTERNAL HIC_API int HIC_Show_PolyCylinder_Point_Count(HIC_Geometry const *geo);

/** @} */ /* end of Show_PolyCylinder_XXX */

/*!
  \addtogroup Set_Sphere_XXX
  @{
*/
/*!
 * \param geo A pointer to a const HIC_Geometry.
 * \param radius A float, value to set the sphere radius to.
 *
 * This function sets the sphere radius to the float value of the passed in parameter.
 */
HC_EXTERNAL HIC_API void HIC_Set_Sphere_Radius(HIC_Geometry const *geo, float radius);

/** @} */ /* end of Set_Sphere_XXX */

/*!
  \addtogroup Show_Sphere_XXX
  @{
*/
/*!
 * \param geo A pointer to a const HIC_Geometry.
 * \return A float.
 *
 * This function returns the sphere radius as a float.
 */
HC_EXTERNAL HIC_API float HIC_Show_Sphere_Radius(HIC_Geometry const *geo);

/** @} */ /* end of Show_Sphere_XXX */




/* general transform attributes */
/*!
  \addtogroup Show_Camera_XXX
  @{
*/
/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A float.
 *
 * This function returns camera field width as a float.
 */
HC_EXTERNAL HIC_API float HIC_Show_Camera_Field_Width(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A float.
 *
 * This function returns camera field height as a float.
 */
HC_EXTERNAL HIC_API float HIC_Show_Camera_Field_Height(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A float.
 *
 * This function returns camera oblique x as a float.
 */
HC_EXTERNAL HIC_API float HIC_Show_Camera_Oblique_X(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A float.
 *
 * This function returns camera oblique y as a float.
 */
HC_EXTERNAL HIC_API float HIC_Show_Camera_Oblique_Y(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A float.
 *
 * This function returns camera eye distance as a float.
 */
HC_EXTERNAL HIC_API float HIC_Show_Camera_Eye_Distance(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A pointer to a const HIC_Point.
 *
 * This function returns camera position information.
 */
HC_EXTERNAL HIC_API HIC_Point const * HIC_Show_Camera_Position(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A pointer to a const HIC_Point.
 *
 * This function returns camera target information.
 */
HC_EXTERNAL HIC_API HIC_Point const * HIC_Show_Camera_Target(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A pointer to a const HIC_Vector.
 *
 * This function returns camera up vector information.
 */
HC_EXTERNAL HIC_API HIC_Vector const * HIC_Show_Camera_Up_Vector(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return An int.
 *
 * This function returns camera projection as an int.
 */
HC_EXTERNAL HIC_API int HIC_Show_Camera_Projection(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A pointer to a 4x4 array of floats.
 *
 * This function returns the camera positioning matrix.
 */
HC_EXTERNAL HIC_API float const * HIC_Show_Camera_Positioning_Matrix(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A pointer to a 4x4 array of floats.
 *
 * This function returns the camera projection matrix.
 */
HC_EXTERNAL HIC_API float const * HIC_Show_Camera_Projection_Matrix(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A float.
 *
 * This function returns camera aspect ratio width as a float.
 */
HC_EXTERNAL HIC_API float HIC_Show_Camera_Aspect(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A float.
 *
 * This function returns the current database camera near limit setting
 * (or the system setting if the camera doesn't specify its own).
 */
HC_EXTERNAL HIC_API float HIC_Show_Camera_Near_Limit(HIC_Rendition const *nr);
/** @} */ /* end of show_cam */

/*!
  \addtogroup Show_Current_Z_Max
  @{
*/
/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A float.
 *
 * This function returns the current Z max as a float value.
 */
HC_EXTERNAL HIC_API float HIC_Show_Current_Z_Max(HIC_Rendition const *nr);
/** @} */ /* end of show_cam */

/* device information */
/*!
  \addtogroup Show_Device_XXX
  @{
*/
/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A pointer to a const char.
 *
 * This function returns the device type.
 */
HC_EXTERNAL HIC_API char const * HIC_Show_Device_Type(HIC_Rendition const *nr);

/*!
 * \param inr A pointer to a const HIC_Rendition.
 * \return A pointer to a const char.
 *
 * This function returns the actor name.
 */
HC_EXTERNAL HIC_API char const * HIC_Show_Actor_Name(HIC_Rendition const *inr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A pointer to a const char.
 *
 * This function returns the device driver.
 */
HC_EXTERNAL HIC_API char const * HIC_Show_Device_Driver(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A boolean.
 *
 * This function returns the device windowing system as a boolean.
 */
HC_EXTERNAL HIC_API bool HIC_Show_Device_Windowing_System(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A boolean.
 *
 * This function returns the device locater as a boolean.
 */
HC_EXTERNAL HIC_API bool HIC_Show_Device_Locater(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return An int.
 *
 * This function returns the number of locater buttons as an int.
 */
HC_EXTERNAL HIC_API int HIC_Show_Device_Locater_Button_Count(HIC_Rendition const *nr);
/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A pointer to a const char.
 *
 * This function returns the device locater type.
 */
HC_EXTERNAL HIC_API char const * HIC_Show_Device_Locater_Type(HIC_Rendition const *nr);
/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return An int.
 *
 * This function returns the number of planes as an int.
 */
HC_EXTERNAL HIC_API int HIC_Show_Device_Plane_Count(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return An int.
 *
 * This function returns the number of colors as an int.
 */
HC_EXTERNAL HIC_API int HIC_Show_Device_Colors(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A HIC_Integer32.
 *
 * This function returns device max colors.
 */
HC_EXTERNAL HIC_API HIC_Integer32  HIC_Show_Device_Max_Colors(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return An int.
 *
 * This function returns horizontal pixels as an int.
 */
HC_EXTERNAL HIC_API int  HIC_Show_Device_Pixels_X(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return An int.
 *
 * This function returns vertical pixels as an int.
 */
HC_EXTERNAL HIC_API int HIC_Show_Device_Pixels_Y(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return An int.
 *
 * This function returns maximum horizontal pixels as an int.
 */
HC_EXTERNAL HIC_API int  HIC_Show_Device_Max_Pixels_X(HIC_Rendition const *nr);	
/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return An int.
 *
 * This function returns maximum vertical pixels as an int.
 */
HC_EXTERNAL HIC_API int HIC_Show_Device_Max_Pixels_Y(HIC_Rendition const *nr);


/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A float.
 *
 * This function returns horizontal size as a float.
 */
HC_EXTERNAL HIC_API float HIC_Show_Device_Size_X(HIC_Rendition const *nr);	

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A float.
 *
 * This function returns vertical size as a float.
 */
HC_EXTERNAL HIC_API float HIC_Show_Device_Size_Y(HIC_Rendition const *nr);	

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A float.
 *
 * This function returns maximum horizontal size as a float.
 */
HC_EXTERNAL HIC_API float HIC_Show_Device_Max_Size_X(HIC_Rendition const *nr);	

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A float.
 *
 * This function returns maximum vertical size as a float.
 */
HC_EXTERNAL HIC_API float HIC_Show_Device_Max_Size_Y(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A float.
 *
 * This function returns horizontal resolution as a float.
 */
HC_EXTERNAL HIC_API float HIC_Show_Device_Resolution_X(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A float.
 *
 * This function returns vertical resolution as a float.
 */
HC_EXTERNAL HIC_API float HIC_Show_Device_Resolution_Y(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A float.
 *
 * This function returns the device pixel aspect ratio as a float.
 */
HC_EXTERNAL HIC_API float HIC_Show_Device_Pixel_Aspect_Ratio(HIC_Rendition const *nr);	

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A float.
 *
 * This function returns the device window aspect ratio as a float.
 */
HC_EXTERNAL HIC_API float HIC_Show_Device_Window_Aspect_Ratio(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A HC_POINTER_SIZED_INT.
 *
 * This function returns the device window ID.
 */
HC_EXTERNAL HIC_API HC_POINTER_SIZED_INT HIC_Show_Device_Window_ID(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A HC_POINTER_SIZED_INT.
 *
 * This function returns the device window ID2.
 */
HC_EXTERNAL HIC_API HC_POINTER_SIZED_INT HIC_Show_Device_Window_ID2(HIC_Rendition const *nr);


/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A long.
 *
 * This function returns the device colormap ID as a long.
 */
HC_EXTERNAL HIC_API long HIC_Show_Device_Colormap_ID(HIC_Rendition const *nr);
/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return An int.
 *
 * This function returns the z-buffer depth as an int.
 */
HC_EXTERNAL HIC_API int HIC_Show_Device_Z_Buffer_Depth(HIC_Rendition const *nr);
/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A boolean.
 *
 * This function returns the device hardware lights as a boolean.
 */
HC_EXTERNAL HIC_API bool HIC_Show_Device_Hardware_Lights(HIC_Rendition const *nr);
/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A boolean.
 *
 * This function returns the device gouraud shading as a boolean.
 */
HC_EXTERNAL HIC_API bool HIC_Show_Device_Gouraud_Shading(HIC_Rendition const *nr);
/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A boolean.
 *
 * This function returns the device phong shading as a boolean.
 */
HC_EXTERNAL HIC_API bool HIC_Show_Device_Phong_Shading(HIC_Rendition const *nr);
/** @} */ /* end of dev_info */

/*!
  \addtogroup Show_Edge_XXX
  @{
*/
/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A char.
 *
 * This function returns the edge line patterns as a symbolic constants.
 */
HC_EXTERNAL HIC_API  char HIC_Show_Edge_Pattern(HIC_Rendition const *nr);
/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A constant char pointer.
 *
 * This function returns the edge style.
 */
HC_EXTERNAL HIC_API const char * HIC_Show_Edge_Style(HIC_Rendition const *nr);
/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return An int.
 *
 * This function returns the edge weight (width in pixels) as an int.
 */
HC_EXTERNAL HIC_API int HIC_Show_Edge_Weight(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A float.
 *
 * This function returns the red component of the edge color as a float value with a range 0 to 1.
 */
HC_EXTERNAL HIC_API float HIC_Show_Edge_Color_Red(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A float.
 *
 * This function returns the green component of the edge color as a float value with a range 0 to 1.
 */
HC_EXTERNAL HIC_API float HIC_Show_Edge_Color_Green(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A float.
 *
 * This function returns the blue component of the edge color as a float value with a range 0 to 1.
 */
HC_EXTERNAL HIC_API float HIC_Show_Edge_Color_Blue(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A HIC_Integer32.
 *
 * This function returns the internal representation of the edge color.
 */
HC_EXTERNAL HIC_API HIC_Integer32 HIC_Show_Internal_Edge_Color(HIC_Rendition const *nr);
/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A float.
 *
 * This function returns the red component of the edge contrast color as a float value with a range 0 to 1.
 */
HC_EXTERNAL HIC_API float HIC_Show_Edge_Contrast_Color_Red(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A float.
 *
 * This function returns the green component of the edge contrast color as a float value with a range 0 to 1.
 */
HC_EXTERNAL HIC_API float HIC_Show_Edge_Contrast_Color_Green(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A float.
 *
 * This function returns the blue component of the edge contrast color as a float value with a range 0 to 1.
 */
HC_EXTERNAL HIC_API float HIC_Show_Edge_Contrast_Color_Blue(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return HIC_Integer32.
 *
 * This function returns the internal representation of the edge contrast color.
 */
HC_EXTERNAL HIC_API HIC_Integer32 HIC_Show_Internal_Edge_Contrast_Color(HIC_Rendition const *nr);
/** @} */ /* end of Show_Edge */

/*!
  \addtogroup Show_Face_XXX
  @{
*/
/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A float.
 *
 * This function returns the face displacement as a float value.			
 */
HC_EXTERNAL HIC_API float HIC_Show_Face_Displacement(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A char.
 *
 * This function returns the face pattern as a char.
 */
HC_EXTERNAL HIC_API char  HIC_Show_Face_Pattern(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A pointer to a const HIC_RGB, a structure of three floats, each between zero and one.
 *
 * This function returns the internal representation of the face material diffuse color.
 */
HC_EXTERNAL HIC_API HIC_RGB const * HIC_Show_Face_Material_Diffuse_Color(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A float.
 *
 * This function returns the red component of the face color as a float value with a range 0 to 1.
 */
HC_EXTERNAL HIC_API float HIC_Show_Face_Color_Red(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A float.
 *
 * This function returns the green component of the face color as a float value with a range 0 to 1.
 */
HC_EXTERNAL HIC_API float HIC_Show_Face_Color_Green(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A float.
 *
 * This function returns the blue component of the face color as a float value with a range 0 to 1.
 */
HC_EXTERNAL HIC_API float HIC_Show_Face_Color_Blue(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A HIC_Integer32.
 *
 * This function returns the internal representation of the face color.
 */
HC_EXTERNAL HIC_API HIC_Integer32 HIC_Show_Internal_Face_Color(HIC_Rendition const *nr);
/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A float.
 *
 * This function returns the red component of the face back color as a float value with a range 0 to 1.
 */
HC_EXTERNAL HIC_API float HIC_Show_Face_Back_Color_Red(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A float.
 *
 * This function returns the green component of the face back color as a float value with a range 0 to 1.
 */
HC_EXTERNAL HIC_API float HIC_Show_Face_Back_Color_Green(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A float.
 *
 * This function returns the blue component of the face back color as a float value with a range 0 to 1.
 */
HC_EXTERNAL HIC_API float HIC_Show_Face_Back_Color_Blue(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A HIC_Integer32.
 *
 * This function returns the internal representation of the face back color.
 */
HC_EXTERNAL HIC_API HIC_Integer32 HIC_Show_Internal_Face_Back_Color(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A float.
 *
 * This function returns the red component of the face contrast color as a float value with a range 0 to 1.
 */
HC_EXTERNAL HIC_API float HIC_Show_Face_Contrast_Color_Red(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A float.
 *
 * This function returns the green component of the face contrast color as a float value with a range 0 to 1.
 */
HC_EXTERNAL HIC_API float HIC_Show_Face_Contrast_Color_Green(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A float.
 *
 * This function returns the blue component of the face contrast color as a float value with a range 0 to 1.
 */
HC_EXTERNAL HIC_API float HIC_Show_Face_Contrast_Color_Blue(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A HIC_Integer32.
 *
 * This function returns the internal representation of the face contrast color.
 */
HC_EXTERNAL HIC_API HIC_Integer32 HIC_Show_Internal_Face_Contrast_Color(HIC_Rendition const *nr);
/** @} */ /* end of Show_Face */

/*!
  \addtogroup Show_Full_Hard_Clip
  @{
*/
/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A pointer to a HIC_Int_Rectangle.
 *
 * This function returns the full hard clip rectangle.
 */
HC_EXTERNAL HIC_API HIC_Int_Rectangle const * HIC_Show_Full_Hard_Clip(HIC_Rendition const *nr);
/** @} */ /* end of Show_Full_Hard_Clip */

/*!
  \addtogroup Show_Buffer_Size
  @{
*/
/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return An HIC_Int_Size
 *
 * This function returns the driver back buffer size
 */
HC_EXTERNAL HIC_API HIC_Int_Size HIC_Show_Buffer_Size(HIC_Rendition const *nr);
/** @} */ /* end of Show_Buffer_Size */

/*!
  \addtogroup Show_Geometry_XXX
  @{
*/
/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A pointer to a const HIC_Geometry.
 *
 * This function returns information about the geometry element in progress or null if no geometry is in progress.
 */
HC_EXTERNAL HIC_API HIC_Geometry const * HIC_Show_Geometry(HIC_Rendition const *nr);
/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A HC_KEY.
 *
 * This function returns the geometry key.		
 */
HC_EXTERNAL HIC_API HC_KEY HIC_Show_Geometry_Key(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A HC_KEY.
 *
 * This function returns the original geometry key.		
 */
HC_EXTERNAL HIC_API HC_KEY HIC_Show_Original_Geometry_Key(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A HIC_Geometry_Type.
 *
 * This function returns the geometry type.		
 */
HC_EXTERNAL HIC_API HIC_Geometry_Type HIC_Show_Geometry_Type(HIC_Rendition const *nr);
/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A HC_KEY.
 *
 * This function returns the geometry owner.
 */
HC_EXTERNAL HIC_API HC_KEY HIC_Show_Geometry_Owner(HIC_Rendition const *nr);
/** @} */ /* end of show_geo */

/*!
  \addtogroup Show_Hidden_Surfaces
  @{
*/
/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A boolean.
 *
 * This function returns a boolean, true if hidden surfaces are shown.
 */
HC_EXTERNAL HIC_API bool HIC_Show_Hidden_Surfaces(HIC_Rendition const *nr);
/** @} */ /* end of show_hidden_surfaces */

/*!
  \addtogroup Using_XXX_HSRA
  @{
*/
/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A boolean.
 *
 * This function returns true if the hsra is hidden line.
 */
HC_EXTERNAL HIC_API bool HIC_Using_Hidden_Line_HSRA(HIC_Rendition const *nr);
/** @} */ /* end of using_xxx_hsra */


/* line attributes */
/*!
  \addtogroup Show_Line_XXX
  @{
*/
/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A char.
 *
 * This function returns the line pattern as a string.
 */
HC_EXTERNAL HIC_API char HIC_Show_Line_Pattern(HIC_Rendition const *nr);
/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A pointer to a const char.
 *
 * This function returns the line style name as a string which is either a user-defined line style or line pattern such as "dashdot".
 */
HC_EXTERNAL HIC_API const char * HIC_Show_Line_Style(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return An int.
 *
 * This function returns line weight as an int.
 */
HC_EXTERNAL HIC_API int HIC_Show_Line_Weight(HIC_Rendition const *nr);
/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A float.
 *
 * This function returns the red component of the line color as a float value with a range 0 to 1.
 */
HC_EXTERNAL HIC_API float HIC_Show_Line_Color_Red(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A float.
 *
 * This function returns the green component of the line color as a float value with a range 0 to 1.
 */
HC_EXTERNAL HIC_API float HIC_Show_Line_Color_Green(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A float.
 *
 * This function returns the blue component of the line color as a float value with a range 0 to 1.
 */
HC_EXTERNAL HIC_API float HIC_Show_Line_Color_Blue(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A HIC_Integer32.
 *
 * This function returns the internal representation of the line color.
 */
HC_EXTERNAL HIC_API HIC_Integer32 HIC_Show_Internal_Line_Color(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A float.
 *
 * This function returns the red component of the line contrast color as a float value with a range 0 to 1.
 */
HC_EXTERNAL HIC_API float HIC_Show_Line_Contrast_Color_Red(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A float.
 *
 * This function returns the green component of the line contrast color as a float value with a range 0 to 1.
 */
HC_EXTERNAL HIC_API float HIC_Show_Line_Contrast_Color_Green(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A float.
 *
 * This function returns the blue component of the line contrast color as a float value with a range 0 to 1.
 */
HC_EXTERNAL HIC_API float HIC_Show_Line_Contrast_Color_Blue(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return An HIC_Integer32.
 *
 * This function returns the internal represantation of the line contrast color.
 */
HC_EXTERNAL HIC_API HIC_Integer32 HIC_Show_Internal_Line_Contrast_Color(HIC_Rendition const *nr);

/** @} */ /* end of show_line */


/* marker attributes */
/*!
  \addtogroup Show_Marker_XXX
  @{
*/
/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A char.
 *
 * This function returns the marker symbol as a char.
 */
HC_EXTERNAL HIC_API char HIC_Show_Marker_Symbol(HIC_Rendition const *nr);
/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A pointer to a const char.
 *
 * This function returns the marker glyph name or symbol as a string.
 *
 * \sa HIC_Show_Marker_Symbol
 */
HC_EXTERNAL HIC_API const char * HIC_Show_Marker_Name(HIC_Rendition const *nr);
/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A float.
 *
 * This function returns the marker size as a float value.			
 */
HC_EXTERNAL HIC_API float HIC_Show_Marker_Size(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A float.
 *
 * This function returns the red component of the marker color as a float value with a range 0 to 1.
 */
HC_EXTERNAL HIC_API float HIC_Show_Marker_Color_Red(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A float.
 *
 * This function returns the green component of the marker color as a float value with a range 0 to 1.
 */
HC_EXTERNAL HIC_API float HIC_Show_Marker_Color_Green(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A float.
 *
 * This function returns the blue component of the marker color as a float value with a range 0 to 1.
 */
HC_EXTERNAL HIC_API float HIC_Show_Marker_Color_Blue(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A HIC_Integer32.
 */
HC_EXTERNAL HIC_API HIC_Integer32  HIC_Show_Internal_Marker_Color(HIC_Rendition const *nr);
/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A float.
 *
 * This function returns the red component of the marker contrast color as a float value with a range 0 to 1.
 */
HC_EXTERNAL HIC_API float HIC_Show_Marker_Contrast_Color_Red(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A float.
 *
 * This function returns the green component of the marker contrast color as a float value with a range 0 to 1.
 */
HC_EXTERNAL HIC_API float HIC_Show_Marker_Contrast_Color_Green(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A float.
 *
 * This function returns the blue component of the marker contrast color as a float value with a range 0 to 1.
 */
HC_EXTERNAL HIC_API float HIC_Show_Marker_Contrast_Color_Blue(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A HIC_Integer32.
 *
 * This function returns the internal representation of the marker contrast color.
 */
HC_EXTERNAL HIC_API HIC_Integer32 HIC_Show_Internal_Marker_Contrast_Color(HIC_Rendition const *nr);

/*!
 * \param marker A pointer to a const HIC_Marker.
 * \return A pointer to a const HIC_Point, given with respect to object coordinates.
 *
 * This function gets the position of a given marker.
 */
HC_EXTERNAL HIC_API HIC_Point const * HIC_Show_Marker_Position(HIC_Marker const *marker);

/*!
 * \param marker A pointer to a const HIC_Rendition.
 * \return An int.
 *
 * This function gets the marker x size;
 */
HC_EXTERNAL HIC_API int HIC_Show_Marker_X_Size(HIC_Rendition const *nr);

/*!
 * \param marker A pointer to a const HIC_Rendition.
 * \return An int.
 *
 * This function gets the marker y size;
 */
HC_EXTERNAL HIC_API int HIC_Show_Marker_Y_Size(HIC_Rendition const *nr);


/** @} */ /* end of marker_attr */

/*!
  \addtogroup Show_Modelling_Matrix_XXX
  @{
*/
/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A pointer to a 4x4 array of floats.
 *
 * This function returns the modelling matrix.
 */
HC_EXTERNAL HIC_API float const * HIC_Show_Modelling_Matrix(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A pointer to a 4x4 array of floats.
 *
 * This function returns the adjoint modelling matrix.
 */
HC_EXTERNAL HIC_API float const * HIC_Show_Modelling_Matrix_Adjoint(HIC_Rendition const *nr);
/** @} */ /* end of show_modelling_matrix */

/*!
  \addtogroup Get_Polyhedron_XXX
  @{
*/
/*!
 * \param phon A pointer to a const HIC_Polyhedron.
 * \return An int, the number of points in a polyhedron.
 *
 * This function returns the number of points in a polyhedron as an int.
 */
HC_EXTERNAL HIC_API int HIC_Get_Polyhedron_Point_Count(HIC_Polyhedron const *phon);
/*!
 * \param phon A pointer to a const HIC_Polyhedron.
 * \return A const pointer to a HIC_Point.
 *
 * This function returns polyhedron points.
 */
HC_EXTERNAL HIC_API HIC_Point const * HIC_Get_Polyhedron_Points(HIC_Polyhedron const *phon);
/*!
 * \param phon A pointer to a const HIC_Polyhedron.
 * \return A pointer to an int.
 *
 * This function returns a polyhedron shell face list.
 */
HC_EXTERNAL HIC_API int * HIC_Get_Polyhedron_Shell_Face_List(HIC_Polyhedron const *phon);
/*!
 * \param phon A pointer to a const HIC_Polyhedron.
 * \return An int.
 *
 * This function returns the length of the polyhedron shell face list as an int.
 */
HC_EXTERNAL HIC_API int HIC_Get_Polyhedron_Shell_Face_List_Length(HIC_Polyhedron const *phon);
/*!
 * \param phon A pointer to a const HIC_Polyhedron.
 * \return An int.
 *
 * This function returns the number of faces in a polygedron as an int.
 */
HC_EXTERNAL HIC_API int HIC_Get_Polyhedron_Face_Count(HIC_Polyhedron const *phon);
/** @} */ /* end of get_poly */

/*!
  \addtogroup Show_Object_To_Screen_XXX
  @{
*/
/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A pointer to a 4x4 array of floats.
 *
 * This function returns the object to screen matrix.
 */
HC_EXTERNAL HIC_API float const * HIC_Show_Object_To_Screen_Matrix(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A pointer to a 4x4 array of floats.
 *
 * This function returns the object to screen adjoint.
 */
HC_EXTERNAL HIC_API float const * HIC_Show_Object_To_Screen_Adjoint(HIC_Rendition const *nr);
/** @} */ /* end of get_poly */


/*!
  \addtogroup Set_Object_To_Screen_Matrix_XXX
  @{
*/

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param fp A pointer to a const float.
 *
 * This function overrides the current transform matrix.  Note that its usefulness is limited to special
 * cases as drawing code may refer directly to the original components (modelling, camera, etc.) or trigger
 * regeneration of the matrix (which will overwrite anything done here).
 */
HC_EXTERNAL HIC_API void  HIC_Set_Object_To_Screen_Matrix(HIC_Rendition const *nr, float const *fp);
/** @} */ /* end of set_object_to_screen_matrix_XXX */



/*!
  \addtogroup Show_Cuurrent_Near_Limit
  @{
*/

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A float.
 *
 * This function returns the current effective camera near limit.
 */
HC_EXTERNAL HIC_API float HIC_Show_Current_Near_Limit(HIC_Rendition const *nr);
/** @} */ /* end of show_current_near_limit */



/*!
  \addtogroup Show_Driver_Segment_Key
  @{
*/
/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A HC_KEY.
 *
 * This function returns the driver segment key.
 */
HC_EXTERNAL HIC_API HC_KEY HIC_Show_Driver_Segment_Key(HIC_Rendition const *nr);
/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A HC_KEY.
 *
 * This function returns the original driver segment key.
 */
HC_EXTERNAL HIC_API HC_KEY HIC_Show_Original_Driver_Segment_Key(HIC_Rendition const *nr);

/** @} */ /* end of driver_segment */

/*!
  \addtogroup Show_Segment_Key
  @{
*/
/*!
 * \param si A pointer to a const HIC_Segment_Info.
 * \return A HC_KEY.
 *
 * This function returns the segment key.
 */
HC_EXTERNAL HIC_API HC_KEY HIC_Show_Segment_Key(HIC_Segment_Info const *si);

/*!
 * \param si A pointer to a const HIC_Segment_Info.
 * \return A HC_KEY.
 *
 * This function returns the original segment key.		
 */
HC_EXTERNAL HIC_API HC_KEY HIC_Show_Original_Segment_Key(HIC_Segment_Info const *si);
/** @} */ /* end of Show_Segment_Key */

/*!
  \addtogroup Show_Polygon_XXX
  @{
*/
/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return An int.
 *
 * \sa HIC_Set_Visibility, HIC_Show_Visibility
 */
HC_EXTERNAL HIC_API int HIC_Show_Polygon_Visibility(HIC_Rendition const *nr);

/*!
 * \param polygon A pointer to a const HIC_Polygon.
 * \return An int, representing vertices with respect to object coordinates.
 *
 * This function returns an int which represents vertices with respect to coordinates of a polygon.
 */
HC_EXTERNAL HIC_API int HIC_Show_Polygon_Count(HIC_Polygon const *polygon);

/*!
 * \param polygon A pointer to a const HIC_Polygon.
 * \return A pointer to a const HIC_Point.
 *
 * This function returns polygon points.			
 */
HC_EXTERNAL HIC_API HIC_Point const * HIC_Show_Polygon_Points(HIC_Polygon const *polygon);
/** @} */ /* end of Show_Polygon_XXX */

/*!
  \addtogroup Show_Polyline_XXX
  @{
*/
/*!
 * \param polyline A pointer to a const HIC_Polyline.
 * \return An int.  The returned vertices are given with respect to object coordinates.
 *
 * This function lets you determine the defining parameters of a given polyline.
 */
HC_EXTERNAL HIC_API int HIC_Show_Polyline_Count(HIC_Polyline const *polyline);

/*!
 * \param polyline A pointer to a const HIC_Polyline.
 * \return A pointer to a const HIC_Point.
 *
 * This function returns the points of a HOOPS polyline data structure.			
 */
HC_EXTERNAL HIC_API HIC_Point const * HIC_Show_Polyline_Points(HIC_Polyline const *polyline);
/** @} */ /* end of Show_Polygon_XXX */

/*!
  \addtogroup Show_Transform_Matrix
  @{
*/

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A pointer to a 4x4 array of floats.
 *
 * This function returns the transform matrix as a 4x4 array of floats.
 */
HC_EXTERNAL HIC_API float const * HIC_Show_Transform_Matrix(HIC_Rendition const *nr);

/** @} */ /* end of Show_Transform_Matrix */




/*!
  \addtogroup Using_Analytic_HSRA
  @{
*/
/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A boolean, true if using analytic HSRA, otherwise false.
 *
 * This function returns a boolean which is true if using analytic hidden surface removal algorithm (HSRA) and 
 * false if not using analytic HSRA.
 */
HC_EXTERNAL HIC_API bool HIC_Using_Analytic_HSRA(HIC_Rendition const *nr);
/** @} */ /* end of using */

/*!
  \addtogroup Tristrip_XXX
  @{
*/
/*!
 * \param ts A pointer to a const HIC_Tristrip.
 * \return A boolean, true if the tristrip parameter has a display list and false if it does not.
 *
 * This function returns a boolean which is true if the tristrip parameter has a display list and 
 * false if the tristrip parameter does not have a display list.
 */
HC_EXTERNAL HIC_API bool HIC_Tristrip_Has_DL(HIC_Tristrip const *ts);
/*!
 * \param ts A pointer to a const HIC_Tristrip.
 *
 * This function marks a texture as prebound on a passed in tristrip data structure.
 */
HC_EXTERNAL HIC_API void HIC_Tristrip_Mark_Texture_Prebound(HIC_Tristrip const *ts);
/** @} */ /* end of tristrip */

/*! \addtogroup Using_Double_Buffer_Swap
*  @{
*/
/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A boolean, true if using double buffer swap and false if not using double buffer swap.
 *
 * This function returns a boolean which is true if using double buffer swap and 
 * false if not using double buffer swap.
 */
HC_EXTERNAL HIC_API bool HIC_Using_Double_Buffer_Swap(HIC_Rendition const *nr);
/*! @} */ /* end of using */


/*! \addtogroup RGBAS32
*  @{
*/
/*!
 * \param r An int.
 * \param g An int.
 * \param b An int.
 * \param a An int.
 * \return A HIC_RGBAS32.
 *
 * This function takes 4 color channels to create an HIC_RGBAS32 structure.
 */
HC_EXTERNAL HIC_API HIC_RGBAS32 HIC_Pack_RGBAS32(int r, int g, int b, int a);
/*!
 * \param rgbas32 An HIC_RGBAS32.
 * \return An unsigned char.
 *
 * This function shows the red component of an HIC_RGBAS32 structure.
 */
HC_EXTERNAL HIC_API unsigned char HIC_Unpack_RGBAS32_Red(HIC_RGBAS32 rgbas32);

/*!
 * \param rgbas32 An HIC_RGBAS32.
 * \return An unsigned char.
 *
 * This function shows the green component of an HIC_RGBAS32 structure.
 */
HC_EXTERNAL HIC_API unsigned char HIC_Unpack_RGBAS32_Green(HIC_RGBAS32 rgbas32);

/*!
 * \param rgbas32 An HIC_RGBAS32.
 * \return An unsigned char.
 *
 * This function shows the blue component of an HIC_RGBAS32 structure.
 */
HC_EXTERNAL HIC_API unsigned char HIC_Unpack_RGBAS32_Blue(HIC_RGBAS32 rgbas32);

/*!
 * \param rgbas32 An HIC_RGBAS32.
 * \return An unsigned char.
 *
 * This function shows the alpha component of an HIC_RGBAS32 structure.
 */
HC_EXTERNAL HIC_API unsigned char HIC_Unpack_RGBAS32_Alpha(HIC_RGBAS32 rgbas32);

/** @} */ /* end of RGBAS32 */


/*!
  \addtogroup Show_Vertex_XXX
  @{
*/
/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A float.
 *
 * This function returns the red component of the vertex color as a float value with a range 0 to 1.
 */
HC_EXTERNAL HIC_API float HIC_Show_Vertex_Color_Red(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A float.
 *
 * This function returns the green component of the vertex color as a float value with a range 0 to 1.
 */
HC_EXTERNAL HIC_API float HIC_Show_Vertex_Color_Green(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A float.
 *
 * This function returns the blue component of the vertex color as a float value with a range 0 to 1.
 */
HC_EXTERNAL HIC_API float HIC_Show_Vertex_Color_Blue(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A HIC_Integer32.
 *
 * This function returns the internal representation of color used to draw shell vertices.
 */
HC_EXTERNAL HIC_API HIC_Integer32 HIC_Show_Internal_Vertex_Color(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A float.
 *
 * This function returns the red component of the vertex contrast color as a float value with a range 0 to 1.
 */
HC_EXTERNAL HIC_API float HIC_Show_Vertex_Contrast_Color_Red(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A float.
 *
 * This function returns the green component of the vertex contrast color as a float value with a range 0 to 1.
 */
HC_EXTERNAL HIC_API float HIC_Show_Vertex_Contrast_Color_Green(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A float.
 *
 * This function returns the blue component of the vertex contrast color as a float value with a range 0 to 1.
 */
HC_EXTERNAL HIC_API float HIC_Show_Vertex_Contrast_Color_Blue(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A HIC_Integer32.
 *
 * This function returns the internal representation of the marker contrast color.
 */
HC_EXTERNAL HIC_API HIC_Integer32 HIC_Show_Internal_Vertex_Contrast_Color(HIC_Rendition const *nr);
/** @} */ /* end of vert_attr */

/*!
  \addtogroup Show_Text_XXX
  @{
*/
/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return An int.
 *
 * This function returns the text alignment as in int.
 *
 * \sa HIC_Set_Text_Alignment
 */
HC_EXTERNAL HIC_API int HIC_Show_Text_Alignment(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A pointer to a const HIC_Vector.
 *
 * This function returns the text path.
 */
HC_EXTERNAL HIC_API HIC_Vector const * HIC_Show_Text_Path(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A HIC_Size_Units.
 *
 * This function returns the text font size units.
 */
HC_EXTERNAL HIC_API HIC_Size_Units HIC_Show_Text_Font_Size_Units(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A HIC_Size_Units.
 *
 * This function returns the text font size tolerance units.
 */
HC_EXTERNAL HIC_API HIC_Size_Units HIC_Show_Text_Font_Size_Tol_Units(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A HIC_Size_Units.
 *
 * This function returns the text font extra space units.	
 */
HC_EXTERNAL HIC_API HIC_Size_Units HIC_Show_Text_Font_Extra_Space_Units(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return void pointer
 *
 * This function returns the text font ID.
 */
HC_EXTERNAL HIC_API void * HIC_Show_Text_Font_ID(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A pointer to a const char.
 *
 * This function returns the text font name as a string.
 */
HC_EXTERNAL HIC_API const char * HIC_Show_Text_Font_Name(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return An int.
 *
 * This function returns the text font transform as an int.
 */
HC_EXTERNAL HIC_API int HIC_Show_Text_Font_Transform(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return An int.
 *
 * This function returns the text font index as an int.
 */
HC_EXTERNAL HIC_API int HIC_Show_Text_Font_Index(HIC_Rendition const *nr);


/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A float.
 *
 * This function returns the red component of the text color as a float value with a range 0 to 1.
 */
HC_EXTERNAL HIC_API float HIC_Show_Text_Color_Red(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A float.
 *
 * This function returns the green component of the text color as a float value with a range 0 to 1.
 */
HC_EXTERNAL HIC_API float HIC_Show_Text_Color_Green(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A float.
 *
 * This function returns the blue component of the text color as a float value with a range 0 to 1.
 */
HC_EXTERNAL HIC_API float HIC_Show_Text_Color_Blue(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A HIC_Integer32.
 *
 * This function returns the internal representation of the text color.
 */
HC_EXTERNAL HIC_API HIC_Integer32 HIC_Show_Internal_Text_Color(HIC_Rendition const *nr);

/*!
 * \param ti A pointer to a const HIC_Text_Info.
 * \return A pointer to a const HIC_Karacter.
 *
 * This function returns the text string.
 */
HC_EXTERNAL HIC_API HIC_Karacter const * HIC_Show_Text_String(HIC_Text_Info const *ti);

/*!
 * \param ti A pointer to a const HIC_Text_Info.
 * \return An int.
 *
 * This function returns the text length as an int representing the number of characters in the text string.
 */
HC_EXTERNAL HIC_API int HIC_Show_Text_Length(HIC_Text_Info const *ti);

/*!
 * \param ti A pointer to a const HIC_Text_Info.
 * \return An int.
 *
 * This function returns the text char encoding as an int.
 */
HC_EXTERNAL HIC_API int HIC_Show_Text_Char_Encoding(HIC_Text_Info const *ti);

/*!
 * \param ti A pointer to a const HIC_Text_Info.
 * \return An int.
 *
 * This function returns the text line count as an int.
 */
HC_EXTERNAL HIC_API int HIC_Show_Text_Line_Count(HIC_Text_Info const *ti);

/*!
 * \param ti A pointer to a const HIC_Text_Info.
 * \return An int.
 *
 * This function returns the text line index as an int.
 */
HC_EXTERNAL HIC_API int HIC_Show_Text_Line_Index(HIC_Text_Info const *ti);

/*!
 * \param ti A pointer to a const HIC_Text_Info.
 * \return A pointer to a const int.
 *
 * This function returns the text counts array.
 */
HC_EXTERNAL HIC_API int const *HIC_Show_Text_Counts(HIC_Text_Info const *ti);

/*!
 * \param ti A pointer to a const HIC_Text_Info.
 * \return A pointer to a const HIC_Point.
 *
 * This function returns the text position.
 */
HC_EXTERNAL HIC_API HIC_Point const * HIC_Show_Text_Position(HIC_Text_Info const *ti);

/*!
 * \param ti A pointer to a const HIC_Text_Info.
 * \return A pointer to a const HIC_Point.
 *
 * This function returns the text reference.
 */
HC_EXTERNAL HIC_API HIC_Point const * HIC_Show_Text_Reference(HIC_Text_Info const *ti);

/*!
 * \param t A pointer to a const HIC_Text.
 * \return A pointer to a const HIC_Point.
 *
 * This function returns the text position that is stored in the graphics database.
 */
HC_EXTERNAL HIC_API HIC_Point const * HIC_Show_3D_Text_Position(HIC_Text const *t);

/*!
 * \param ti A pointer to a const HIC_Text_Info.
 * \return A pointer to a const HIC_Vector.
 *
 * This function returns the text baseline.
 */
HC_EXTERNAL HIC_API HIC_Vector const * HIC_Show_Text_Baseline(HIC_Text_Info const *ti);

/*!
 * \param ti A pointer to a const HIC_Text_Info.
 * \return A pointer to a const HIC_Vector.
 *
 * This function returns the text longest baseline.
 */
HC_EXTERNAL HIC_API HIC_Vector const * HIC_Show_Text_Longest_Baseline(HIC_Text_Info const *ti);

/*!
 * \param ti A pointer to a const HIC_Text_Info.
 * \return A pointer to a const HIC_Vector.
 *
 * This function returns the text char horizontal.
 */
HC_EXTERNAL HIC_API HIC_Vector const * HIC_Show_Text_Char_Horizontal(HIC_Text_Info const *ti);

/*!
 * \param ti A pointer to a const HIC_Text_Info.
 * \return A pointer to a const HIC_Vector.
 *
 * This function returns the text char vertical.
 */
HC_EXTERNAL HIC_API HIC_Vector const * HIC_Show_Text_Char_Vertical(HIC_Text_Info const *ti);

/*!
 * \param ti A pointer to a const HIC_Text_Info.
 * \return A pointer to a const HIC_Vector.
 *
 * This function returns the text next line vector.
 */
HC_EXTERNAL HIC_API HIC_Vector const * HIC_Show_Text_Next_Line(HIC_Text_Info const *ti);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param ti A pointer to a const HIC_Text_Info.
 * \param index An int.
 * \return A pointer to a const HIC_Point.
 *
 * This function returns the text char position.
 */
HC_EXTERNAL HIC_API HIC_Point const * HIC_Show_Text_Char_Position(HIC_Rendition const *nr, HIC_Text_Info const *ti, int index);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param ti A pointer to a const HIC_Text_Info.
 * \param index An int.
 * \return A float.
 *
 * This function returns the text char width as a float value.
 */
HC_EXTERNAL HIC_API float HIC_Show_Text_Char_Width(HIC_Rendition const *nr, HIC_Text_Info const *ti, int index);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param ti A pointer to a const HIC_Text_Info.
 * \return A pointer to a const int.
 *
 * This function returns the int pointer owned by the HIC_Text_Info.
 */
HC_EXTERNAL HIC_API int const * HIC_Show_Text_KIndex(HIC_Rendition const *nr, HIC_Text_Info const *ti);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param ti A pointer to a const HIC_Text_Info.
 * \param index Index into Karacter_Info array
 * \return A pointer to a const Text_Karacter_Info.
 *
 * This function returns the Text_Karacter_Info pointer owned by the HIC_Text_Info.
 */
HC_EXTERNAL HIC_API HIC_Text_Karacter_Info const * HIC_Show_Text_KInfo_At_Index(HIC_Rendition const *nr, HIC_Text_Info const *ti, int index);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \param ti A pointer to a const HIC_Text_Info.
 * \return A pointer to a const Text_Karacter_Info.
 *
 * This function returns the Text_Karacter_Info pointer owned by the HIC_Text_Info.
 */
HC_EXTERNAL HIC_API HIC_Text_Karacter_Info const * HIC_Show_Text_KInfo(HIC_Rendition const *nr, HIC_Text_Info const *ti);

/*!
 * \param iki A pointer to a const HIC_Text_Info.
 * \return A pointer to a const HIC_Rendition.
 *
 * This function returns the Text_Karacter_Info net rendition.
 */
HC_EXTERNAL HIC_API HIC_Rendition const * HIC_Show_KInfo_Net_Rendition(HIC_Text_Karacter_Info const *iki);

/*!
 * \param ki A pointer to a const HIC_Text_Info.
 * \return A pointer to a const HIC_Point.
 *
 * This function returns the Text_Karacter_Info position.
 */
HC_EXTERNAL HIC_API HIC_Point const * HIC_Show_KInfo_Position(HIC_Text_Karacter_Info const *ki);

/*!
 * \param ki A pointer to a const HIC_Text_Info.
 * \return A pointer to a const HIC_Point.
 *
 * This function returns the Text_Karacter_Info point.
 */
HC_EXTERNAL HIC_API HIC_Point const * HIC_Show_KInfo_Point(HIC_Text_Karacter_Info const *ki);

/*!
 * \param iki A pointer to a const HIC_Text_Info.
 * \return A pointer to a const HIC_Point.
 *
 * This function returns the Text_Karacter_Info width.
 */
HC_EXTERNAL HIC_API float HIC_Show_KInfo_Width(HIC_Text_Karacter_Info const *iki);

/*!
 * \param iki A pointer to a const HIC_Text_Info.
 * \return A pointer to a const HIC_Point.
 *
 * This function returns the Text_Karacter_Info height.
 */
HC_EXTERNAL HIC_API float HIC_Show_KInfo_Height(HIC_Text_Karacter_Info const *iki);

/*!
 * \param inr A pointer to a const HIC_Rendition.
 * \return A float.
 *
 * This function returns the text spacing as a float.
 */
HC_EXTERNAL HIC_API float HIC_Show_Text_Spacing(HIC_Rendition const *inr);

/*!
 * \param inr A pointer to a const HIC_Rendition.
 * \return A float.
 *
 * This function returns the text font size as a float.
 */
HC_EXTERNAL HIC_API float HIC_Show_Text_Font_Size(HIC_Rendition const *inr);

/*!
 * \param inr A pointer to a const HIC_Rendition.
 * \return A float.
 *
 * This function returns the text font size tolerance as a float.
 */
HC_EXTERNAL HIC_API float HIC_Show_Text_Font_Size_Tolerance(HIC_Rendition const *inr);

/*!
 * \param inr A pointer to a const HIC_Rendition.
 * \return A float.
 *
 * This function returns the final text font size as a float.
 */
HC_EXTERNAL HIC_API float HIC_Show_Text_Final_Font_Size(HIC_Rendition const *inr);

/*!
 * \param inr A pointer to a const HIC_Rendition.
 * \return A float.
 *
 * This function returns the text font extra space as a float.
 */
HC_EXTERNAL HIC_API float HIC_Show_Text_Font_Extra_Space(HIC_Rendition const *inr);

/*!
 * \param inr A pointer to a const HIC_Rendition.
 * \return A float.
 *
 * This function returns the text font rotation as a float.
 */
HC_EXTERNAL HIC_API float HIC_Show_Text_Font_Rotation(HIC_Rendition const *inr);

/*!
 * \param inr A pointer to a const HIC_Rendition.
 * \return A float.
 *
 * This function returns the text font slant as a float.
 */
HC_EXTERNAL HIC_API float HIC_Show_Text_Font_Slant(HIC_Rendition const *inr);

/*!
 * \param inr A pointer to a const HIC_Rendition.
 * \return A bool.
 *
 * This function returns true if the text is outlined.
 */
HC_EXTERNAL HIC_API bool HIC_Show_Text_Is_Outlined(HIC_Rendition const *nr);

/*!
 * \param inr A pointer to a const HIC_Rendition.
 * \return A double.
 *
 * This function returns the text width scale.
 */
HC_EXTERNAL HIC_API double HIC_Show_Text_Width_Scale(HIC_Rendition const *nr);

/*!
 * \param inr A pointer to a const HIC_Rendition.
 * \return A const char *.
 *
 * This function returns the text specified text.
 */
HC_EXTERNAL HIC_API const char * HIC_Show_Text_Specified_Text(HIC_Rendition const *nr);

/*! @} */ /* end of show_text */


/*!
  \addtogroup Has_Text_XXX
  @{
*/
/*!
 * \param inr A pointer to a const HIC_Rendition.
 * \return A bool.
 *
 * This function returns true if the text has a names_ref.
 */
HC_EXTERNAL HIC_API bool HIC_Has_Text_Names_Ref(HIC_Rendition const *nr);

/*!
 * \param inr A pointer to a const HIC_Rendition.
 * \return A bool.
 *
 * This function returns true if the text has a names_ref names.
 */
HC_EXTERNAL HIC_API bool HIC_Has_Text_Names_Ref_Names(HIC_Rendition const *nr);
/*! @} */ /* end of has_text_xxx */

/*!
  \addtogroup Is_Text_XXX
  @{
*/
/*!
 * \param inr A pointer to a const HIC_Rendition.
 * \return A bool.
 *
 * This function returns true if the text is GFN_SPECIFIED.
 */
HC_EXTERNAL HIC_API bool HIC_Is_Text_GFN_Specified(HIC_Rendition const *nr);
/*! @} */ /* end of is_text_xxx */

/*!
  \addtogroup Show_Image_XXX
  @{
*/
/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A pointer to a const HIC_Point.
 *
 * This function returns the image position.
 */
HC_EXTERNAL HIC_API HIC_Point const * HIC_Show_Image_Position(HIC_Image const *image);

/*! @} */ /* end of show_image_xxx */


/*!
  \addtogroup Show_World_To_Screen_XXX
  @{
*/
/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A pointer to a 4x4 array of floats.
 *
 * This function returns the world to screen matrix.
 */
HC_EXTERNAL HIC_API float const * HIC_Show_World_To_Screen_Matrix(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A pointer to a 4x4 array of floats.
 *
 * This function returns the world to screen adjoint.
 */
HC_EXTERNAL HIC_API float const * HIC_Show_World_To_Screen_Adjoint(HIC_Rendition const *nr);
/*! @} */ /* end of show_text */


/*!
  \addtogroup Show_Window_XXX
  @{
*/
/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A pointer to a HIC_Int_Rectangle.
 *
 * This function returns the window extent.			
 */
HC_EXTERNAL HIC_API HIC_Int_Rectangle const * HIC_Show_Window_Extent(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A float.
 *
 * This function returns the red component of the window color as a float value with a range 0 to 1.			
 */
HC_EXTERNAL HIC_API float HIC_Show_Window_Color_Red(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A float.
 *
 * This function returns the green component of the window color as a float value with a range 0 to 1.			
 */
HC_EXTERNAL HIC_API float HIC_Show_Window_Color_Green(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A float.
 *
 * This function returns the blue component of the window color as a float value with a range 0 to 1.
 */
HC_EXTERNAL HIC_API float HIC_Show_Window_Color_Blue(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A HIC_Integer32.
 *
 * This function returns the internal representation of the window color.
 */
HC_EXTERNAL HIC_API HIC_Integer32 HIC_Show_Internal_Window_Color(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A float.
 *
 * This function returns the red component of the window contrast color as a float value with a range 0 to 1.			
 */
HC_EXTERNAL HIC_API float HIC_Show_Window_Contrast_Color_Red(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A float.
 *
 * This function returns the green component of the window contrast color as a float value with a range 0 to 1.			
 */
HC_EXTERNAL HIC_API float HIC_Show_Window_Contrast_Color_Green(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A float.
 *
 * This function returns the blue component of the window contrast color as a float value with a range 0 to 1.			
 */
HC_EXTERNAL HIC_API float HIC_Show_Window_Contrast_Color_Blue(HIC_Rendition const *nr);

/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A HIC_Integer32.
 *
 * This function returns the internal representation of the internal window contrast color.
 */
HC_EXTERNAL HIC_API HIC_Integer32 HIC_Show_Internal_Window_Contrast_Color(HIC_Rendition const *nr);
/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A char.
 *
 * This function returns the window pattern.
 */
HC_EXTERNAL char HIC_API HIC_Show_Window_Pattern(HIC_Rendition const *nr);
/** @} */ /* end of show_win */


/* Note: lengths and *_indices are owned by the strips, not just references */

/* tristrip attributes */
/*!
  \addtogroup Show_Tristrip_XXX
  *  Note: lengths and *_indices are owned by the strips, not just references.
 @{
*/
/*!
 * \param ts A pointer to a const HIC_Tristrip.
 * \return A pointer to a a const HIC_Point.
 *
 * This function returns the points of a HOOPS tristrip data structure.		
 */

HC_EXTERNAL HIC_API HIC_Point const * HIC_Show_Tristrip_Points(HIC_Tristrip const *ts);


/*!
 * \param ts A pointer to a const HIC_Tristrip.
 * \return An int.
 *
 * This function returns the number of points in a HOOPS tristrip data structure.		
 */
HC_EXTERNAL HIC_API int HIC_Show_Tristrip_Point_Count(HIC_Tristrip const *ts);

/*!
 * \param ts A pointer to a const HIC_Tristrip.
 * \return An int.
 *
 * This function returns the number of faces in a HOOPS tristrip data structure.			
 */
HC_EXTERNAL HIC_API int HIC_Show_Tristrip_Face_Count(HIC_Tristrip const *ts);

/*!
 * \param ts A pointer to a const HIC_Tristrip.
 * \return An int.
 *
 * This function returns the strips of a HOOPS tristrip data structure.
 */
HC_EXTERNAL HIC_API int HIC_Show_Tristrip_Strips(HIC_Tristrip const *ts);


/*!
 * \param ts A pointer to a const HIC_Tristrip.
 * \return A const int pointer.
 *
 * This function returns the lengths of a HOOPS tristrip data structure.
 */
HC_EXTERNAL HIC_API int const * HIC_Show_Tristrip_Lengths(HIC_Tristrip const *ts);


/*!
 * \param ts A pointer to a const HIC_Tristrip.
 * \return A const int pointer.
 *
 * This function returns the vertex indices of a HOOPS tristrip data structure.
 */
HC_EXTERNAL HIC_API int const * HIC_Show_Tristrip_Vertex_Indices(HIC_Tristrip const *ts);


/*!
 * \param ts A pointer to a const HIC_Tristrip.
 * \return A const int pointer.
 *
 * This function returns the face indices of a HOOPS tristrip data sructure.
 */
HC_EXTERNAL HIC_API int const * HIC_Show_Tristrip_Face_Indices(HIC_Tristrip const *ts);



/*!
 * \param ts A pointer to a const HIC_Tristrip.
 * \return A pointer to a const HIC_RGB, a structure of three floats, each between zero and one.
 *
 * This function returns the face colors of a a HOOPS tristrip data structure.
 */
HC_EXTERNAL HIC_API HIC_RGB const * HIC_Show_Tristrip_Face_Colors(HIC_Tristrip const *ts);


/*!
 * \param ts A pointer to a const HIC_Tristrip.
 * \return A pointer to a const HIC_Plane.
 *
 * This function returns the face normals of a HOOPS tristrip data structure.		
 */
HC_EXTERNAL HIC_API HIC_Plane const * HIC_Show_Tristrip_Face_Normals(HIC_Tristrip const *ts);

/*!
 * \param ts A pointer to a const HIC_Tristrip.
 * \return A pointer to a const HIC_RGB, a structure of three floats, each between zero and one.
 *
 * This function returns the vertex colors of a HOOPS tristrip data structure.
 */
HC_EXTERNAL HIC_API HIC_RGB const * HIC_Show_Tristrip_Vertex_Colors(HIC_Tristrip const *ts);

/*!
 * \param ts A pointer to a const HIC_Tristrip.
 * \return A pointer to a const float.
 *
 * This function returns the vertex findices of a HOOPS tristrip data structure.		
 */
HC_EXTERNAL HIC_API float const * HIC_Show_Tristrip_Vertex_Findices(HIC_Tristrip const *ts);

/*!
 * \param ts A pointer to a const HIC_Tristrip.
 * \return A pointer to a const HIC_Vector.
 *
 * This function returns the vertex normals of a HOOPS tristrip data structure.
 */
HC_EXTERNAL HIC_API HIC_Vector const * HIC_Show_Tristrip_Vertex_Normals(HIC_Tristrip const *ts);

/*!
 * \param ts A pointer to a const HIC_Tristrip.
 * \return A pointer to a const HIC_Parameter.
 *
 * This function returns the vertex parameters of a HOOPS tristrip data structure.
 */
HC_EXTERNAL HIC_API HIC_Parameter const * HIC_Show_Tristrip_Vertex_Parameters(HIC_Tristrip const *ts);
/*!
 * \param ts A pointer to a const HIC_Tristrip.
 * \return A HIC_Integer.
 *
 * This function returns the vertex parameter width of a HOOPS tristrip data structure.
 */
HC_EXTERNAL HIC_API HIC_Integer32 HIC_Show_Tristrip_Vertex_Parameter_Width(HIC_Tristrip const *ts);
/*!
 * \param ts A pointer to a const HIC_Tristrip.
 * \return A boolean.
 *
 * This function returns the findex color interpolation of a HOOPS tristrip data structure.
 */
HC_EXTERNAL HIC_API bool HIC_Show_Tristrip_Findex_Color_Interpolation(HIC_Tristrip const *ts);
/** @} */ /* end of show_tri */

/*!
  \addtogroup Get_Update_Stat
  @{
*/
/*!
 * \param inr A pointer to a const HIC_Rendition.
 * \param stat_name A pointer to a null terminated string specifying stat to query.
 * \param stat_value A pointer to double returning the value of the stat query.
 * \return A double.
 *
 * This function returns the value for the stat queried.
 */
HC_EXTERNAL HIC_API void HIC_Get_Update_Stat(HIC_Rendition const *inr, const char *stat_name, double *stat_value);
/** @} */ /* end of Get_Update_Stat */

/*!
  \addtogroup Is_Selection
  @{
*/
/*!
 * \param nr A pointer to a const HIC_Rendition.
 * \return A bool.
 *
 * This function returns true if we are currently in a selection callback.
 */
HC_EXTERNAL HIC_API bool HIC_Is_Selection(HIC_Rendition const *nr);
/*! @} */ /* end of is_selection */


HC_EXTERNAL HIC_API void HIC_Show_Time(double *current_time);



HC_EXTERNAL HIC_API void HIC_Compute_Transformed_Points (HIC_Rendition const *nr, int count, HIC_Point const points[], float const matp[], HIC_Point result[]);

HC_EXTERNAL HIC_API HC_KEY HIC_Find_Relative_Segment(HIC_Rendition const *nr, HC_KEY start, const char *path);

HC_EXTERNAL HIC_API HC_KEY HIC_Show_Owner_Key_By_Key(HIC_Rendition const *nr, HC_KEY key);

HC_EXTERNAL HIC_API HC_KEY HIC_Show_Owner_Original_Key_By_Key(HIC_Rendition const *nr, HC_KEY key);

HC_EXTERNAL HIC_API bool HIC_Compute_Transform_By_Path(HIC_Rendition const *nr, int count, HC_KEY const keys[], char const * in_system, char const * out_system, float matrix[]);

HC_EXTERNAL HIC_API void HIC_Show_Shell_Size(HIC_Rendition const *nr, HC_KEY key, int * point_countp, int * face_list_lengthp); 

HC_EXTERNAL HIC_API void HIC_Show_Shell (HIC_Rendition const *nr, HC_KEY key, int *point_countp, HIC_Point points[], int *face_list_lengthp, int face_list[]); 

HC_EXTERNAL HIC_API void HIC_Compute_Identity_Matrix (HIC_Rendition const *nr, float matp[]);

HC_EXTERNAL HIC_API void HIC_Compute_Scale_Matrix (HIC_Rendition const *nr, double x, double y, double z, float matp[]);

HC_EXTERNAL HIC_API void HIC_Compute_Matrix_Product (HIC_Rendition const *nr, float const leftp[], float const rightp[], float resultp[]);

HC_EXTERNAL HIC_API bool HIC_Compute_Matrix_Inverse (HIC_Rendition const *nr, float const matp[], float invp[]);

HC_EXTERNAL HIC_API void HIC_Unsafe_Threading (HIC_Rendition const *nr, bool allow);

HC_EXTERNAL HIC_API HC_POINTER_SIZED_INT HIC_Show_Effective_Thread_ID();

HC_EXTERNAL HIC_API bool HIC_Show_Bounding_Sphere_By_Key(HIC_Rendition const *in_nr, HC_KEY key, HIC_Point *center, float *radius);

HC_EXTERNAL HIC_API bool HIC_Show_Bounding_Cuboid_By_Key (HIC_Rendition const *in_nr, HC_KEY key, HIC_Point * min, HIC_Point * max);

HC_EXTERNAL HIC_API bool HIC_Show_Camera_By_Key(HIC_Rendition const *in_nr, HC_KEY key, HIC_Point *position, HIC_Point *target, HIC_Vector *up_vector, float *width, float *height, int *projection);

HC_EXTERNAL HIC_API float HIC_Get_Current_Segment_Extent(HIC_Rendition const *in_nr);

HC_EXTERNAL HIC_API void HIC_Prune_Segment(HIC_Rendition const *in_nr);

HC_EXTERNAL HIC_API void HIC_Driver_Draw_Textured_Background(HIC_Rendition const *in_nr, void * texture, float const * matrix, HIC_Int_Rectangle const * extent);

HC_EXTERNAL HIC_API HC_POINTER_SIZED_INT HIC_Driver_Get_Graphics_Context(HIC_Rendition const *in_nr);



#define HIC_DEFINED
#endif




