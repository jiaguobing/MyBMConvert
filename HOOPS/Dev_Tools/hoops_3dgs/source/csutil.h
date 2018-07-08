// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef __CSBSHELL_H__
#define __CSBSHELL_H__

#include "hoops.h"
#include "database.h"

/*------------data structure definitions------------------*/

#define HK_FACE_NODE					1
#define HK_POLYCYLINDER_SECTION_NODE	2
#define HK_PHON_INFO_NODE				3 /* see csbshell.c */
#define HK_POLYCYLINDER_INFO_NODE		4 /* see csbshell.c */
#define HK_CYLINDER_INFO_NODE			5 /* see csbshell.c */
#define HK_POLYGON_INFO_NODE			6 /* see csbshell.c */
#define HK_GRID_INFO_NODE				7 /* see csbshell.c */
#define HK_SPHERE_INFO_NODE				8 /* see csbshell.c */
#define HK_VERTEX_NODE					9 /* see csbshell.c */


template <typename F>
struct csnode_header {
	Cuboid_3D<F>	bbox;				/* pretransformed bounding box around the object */
	char			type;
	char			flags;				/* type specific */
};
typedef csnode_header<float>	csnode_header_t;


#define FN_OWNS_FACE_PTR	 0x1	/* memory for face_ptr is locally owned (and thus must be deleted at some point) */

template <typename F>
struct face_node_tmpl : public csnode_header<F> {
	short			len;
	int const *		face_ptr;			/* pointer into the face list (or tristrips' vertex index array) -- does not include length. */
	Plane_3D<F>		plane;				/* pretransformed plane of the face */
	int				index;
};
typedef face_node_tmpl<float>	face_node_t;


template <typename F>
struct vertex_node : public csnode_header<F> {
	int				pnum;
	Point_3D<F>		vertex;
};

#define PCSN_LOOSE_BOUNDS	0x1
template <typename F>
struct pcsnode : public csnode_header<F> {
	int				start;						
	int				end;
};
typedef pcsnode<float>	pcsnode_t;


template <typename F>
struct face_face_node {
	face_node_tmpl<F> *	f1;
	face_node_tmpl<F> *	f2;
	void *			env;
	F				min;
	F				max;
};
typedef face_face_node<float>	face_face_node_t;

template <typename F>
face_node_tmpl<F> * cs_new_face_node(
	char				flags,
	short				len,
	int const *			indices, 
	Point_3D<F> const *	points,
	Matrix const &		matrix,
	Plane_3D<F> const &	transformed_plane,
	int					index);


template <typename F>
vertex_node<F> * cs_new_vertex_node( 
	Point_3D<F> const *	point, 
	F					tolerance, 
	int					pnum);

template <typename F>
pcsnode<F> * cs_new_polycylinder_section_node( 
	int					start,
	int					end,
	PolyCylinder *		pc,
	Matrix const &		matrix);

/* signature matches HI_BSP_Map_Function.  
 * Looks at type in header to determine which destructor to call */
template <typename F>
vbsp_status cs_delete_node(void *ptr, void *ptr2);

template <typename F>
void cs_compute_intersection(Point_3D<F> const * pt1, Point_3D<F> const * pt2,
							 Plane_3D<F> const * halfspace, Point_3D<F> * out);

#define CS_OUT		0  /* must be zero */
#define CS_IN		1
#define CS_MAYBE	2

template <typename F>
int cs_point_inside_all_edges( 
	Point_3D<F> const *	pt,
	Point_3D<F> const *	points1,
	int					len1,
	int const *			face1,
	Plane_3D<F> const *	plane1,
	F					tolerance = 0);

template <typename F>
void cs_clip_line_to_edges( 
	Point_3D<F> *		pt,
	Point_3D<F> const *	points1,
	int					len1,
	int const *			face1,
	Plane_3D<F> const *	plane1,
	F					tolerance);

template <typename F>
bool cs_final_accurate_test_polygon_polygon(
	Point_3D<F> const *	points1, 
	int					len1,
	int const *			face1,
	Plane_3D<F> const *	plane1,
	Point_3D<F> const *	points2,
	int					len2,
	int const *			face2,
	Plane_3D<F> const *	plane2,
	Point_3D<F> *		intersection = null);

template <typename F>
bool cs_final_accurate_test_polygon_polygon2(
	Thread_Data *	thread_data,
	Point_3D<F> const *		points1, 
	int						len1,
	int const *				face1,
	Plane_3D<F> const *		plane1,
	Point_3D<F> const *		points2,
	int						len2,
	int const *				face2,
	Plane_3D<F> const *		plane2,
	Point_3D<F> *			intersection,
	Cuboid_3D<F> *			collision_bbox,
	bool					coincident_clash,
	bool					world_handedness_left = true);

template <typename F>
bool cs_final_accurate_test_polygon_polygon_soft(
	Thread_Data *	thread_data,
	Point_3D<F> const *		points1, 
	Vector_3D<F> const *	normals1, 
	Plane_3D<F> const *		plane1,
	int						len1,
	int const *				face1,
	Point_3D<F> const *		points2,
	int						len2,
	int const *				face2,
	Plane_3D<F> const *		plane2,
	Point_3D<F> *			intersecton,
	F						fudge = 0);

template <typename F>
bool cs_test_face_plane(
	Point_3D<F> const *	points, 
	int					len,
	int const *			iptr,
	Plane_3D<F> const &	plane,
	F const soft_clash_tolerance = F(0));

template <typename F>
void cs_build_face_BSP(
	VBSPT<F> *			tree, 
	int					point_count, 
	Point_3D<F> const *	points, 
	int					face_list_length, 
	int const *			face_list, 
	bool				reverse_plane_direction);

template <typename F>
void cs_build_vertex_BSP(
	VBSPT<F> *			tree, 
	int					point_count, 
	Point_3D<F> const *	points,
	F					tolerance);

template <typename F>
void cs_calc_face_bbox(
	Point_3D<F> const *		points, 
	Matrix const &			matrix,
	int						len,
	int const *				iptr, 
	Cuboid_3D<F> &			bbox_out);

template <typename F>
bool cs_compute_one_face_plane(
	Memory_Pool *			memory_pool,
	Point_3D<F> const *		points_in, 
	int						len,
	int const *				iptr, 
	bool					reverse_plane_direction,
	Plane_3D<F> &			plane_out);

template <typename F>
int cs_test_face_ray(
	Point_3D<F> const *		points, 
	face_node_tmpl<F> const *	face,
	Point_3D<F> const *		start, 
	Vector_3D<F> const *	direction, 
	F *						t_out,
	Point_3D<F> *			intersect_out = null);

template <typename F>
int cs_test_face_line(
	Point_3D<F> const *		points, 
	face_node_tmpl<F> const *	face,
	Point_3D<F> const *		p1, 
	Point_3D<F> const *		p2);


#endif
