
#ifndef HC_DEFINED

#ifdef __cplusplus
#   define		HC_EXTERNAL	extern "C"
#   define		HC_ANY_ARGS	...
#else
#   define		HC_EXTERNAL	extern
#   define		HC_ANY_ARGS
#   ifndef __STDC__
#	ifndef _MSC_VER
#	    define	const
#	endif
#   endif
#   ifdef sun
#	ifndef __STDC__
#	    define	NO_PROTOTYPES
#	endif
#   endif
#endif

#ifdef NO_PROTOTYPES
#   define HC_PROTO(a) ()
#else
#   define HC_PROTO(a) a
#endif


#ifndef HC_CDECL
#   ifdef _MSC_VER
#       define HC_CDECL __cdecl
#   else
#       define HC_CDECL
#   endif
#endif


#ifndef HC_POINT
#	ifdef SWIG
typedef float HC_POINT;
#	else
#   	define	HC_POINT	void
    /*
     * NOTE: if you want a real definition for "HC_POINT", put
     *		typedef	struct {float x, y, z;}	Point;
     *	        #define	HC_POINT	Point
     *	     in your program before including <hc.h>.
     */
#	endif
#endif


#ifndef HC_DPOINT
#	ifdef SWIG
typedef double HC_DPOINT;
#	else
#   	define	HC_DPOINT	void
    /*
     * NOTE: if you want a real definition for "HC_DPOINT", put
     *		typedef	struct {double x, y, z;}	DPoint;
     *	        #define	HC_DPOINT	DPoint
     *	     in your program before including <hc.h>.
     */
#	endif
#endif


#ifndef HC_VECTOR
#	ifdef SWIG
typedef float HC_VECTOR;
#	else
#   	define	HC_VECTOR	void
    /*
     * NOTE: if you want a real definition for "HC_VECTOR", put
     *		typedef	struct {float x, y, z;}	Vector;
     *	        #define	HC_VECTOR	Vector
     *	     in your program before including <hc.h>.
     */
#	endif
#endif

#ifndef HC_DVECTOR
#	ifdef SWIG
typedef double HC_DVECTOR;
#	else
#   	define	HC_DVECTOR	void
    /*
     * NOTE: if you want a real definition for "HC_DVECTOR", put
     *		typedef	struct {double x, y, z;}	DVector;
     *	        #define	HC_DVECTOR	DVector
     *	     in your program before including <hc.h>.
     */
#	endif
#endif

#ifndef HC_PLANE
#	ifdef SWIG
typedef float HC_PLANE;
#	else
#   define	HC_PLANE	void
    /*
     * NOTE: if you want a real definition for "HC_PLANE", put
     *		typedef	struct {float a, b, c, d;} Plane;
     *	        #define	HC_PLANE	Plane
     *	     in your program before including <hc.h>.
     */
#	endif
#endif

#ifndef HC_DPLANE
#	ifdef SWIG
typedef double HC_DPLANE;
#	else
#   define	HC_DPLANE	void
    /*
     * NOTE: if you want a real definition for "HC_DPLANE", put
     *		typedef	struct {double a, b, c, d;} DPlane;
     *	        #define	HC_DPLANE	DPlane
     *	     in your program before including <hc.h>.
     */
#	endif
#endif

#ifndef HC_RGB
#	ifdef SWIG
typedef float HC_RGB;
#	else
#   	define	HC_RGB	void
    /*
     * NOTE: if you want a real definition for "HC_RGB", put
     *		typedef	struct {float red, green, blue;}	RGB;
     *	        #define	HC_RGB	RGB
     *	     in your program before including <hc.h>.
     */
#	endif
#endif

#ifndef HC_RGBA
#	ifdef SWIG
typedef float HC_RGBA;
#	else
#   	define	HC_RGBA	void
    /*
     * NOTE: if you want a real definition for "HC_RGB", put
     *		typedef	struct {float red, green, blue, alpha;}	RGBA;
     *	        #define	HC_RGBA	RGBA
     *	     in your program before including <hc.h>.
     */
#	endif
#endif


#ifndef HC_BOOLEAN
#	ifdef __cplusplus
#	define	HC_BOOLEAN	bool
#	else
#	define	HC_BOOLEAN	char
#endif
#endif


#ifndef HC_POINTER_SIZED_INT
#if defined(_M_IA64) || defined(_M_AMD64) || defined(WIN64) || defined(_WIN64) || defined(_M_X64)
#   define  HC_POINTER_SIZED_INT    __int64
#else
#   define  HC_POINTER_SIZED_INT    long
#endif
#endif


#ifndef HC_KEY
#   define  HC_KEY      HC_POINTER_SIZED_INT
#endif

#ifndef HC_ERROR_KEY
#   define  HC_ERROR_KEY      (HC_KEY)-1
#endif

#ifndef HC_PIXEL
    /* (This definition probably shouldn't be changed) */
#   define	HC_PIXEL	void
#endif

HC_EXTERNAL void HC_CDECL HC_Abort_Program (
		const char * message);
HC_EXTERNAL void HC_CDECL HC_Append_Modelling_Matrix (
		const float * array);
HC_EXTERNAL void HC_CDECL HC_Append_Texture_Matrix (
		const float * array);
HC_EXTERNAL void HC_CDECL HC_Apply_Attribute_Filter (
		HC_KEY key, const char * filter);
HC_EXTERNAL void HC_CDECL HC_Begin_Alias_Search (
		void);
HC_EXTERNAL int HC_CDECL HC_Begin_Buffered_Selection (
		const char * start_name, double left, double right, double bottom, double top, const char * options);
HC_EXTERNAL void HC_CDECL HC_Begin_Callback_Name_Search (
		void);
HC_EXTERNAL void HC_CDECL HC_Begin_Color_Name_Search (
		void);
HC_EXTERNAL void HC_CDECL HC_Begin_Contents_Search (
		const char * segment, const char * filter);
HC_EXTERNAL void HC_CDECL HC_Begin_Decimate_Shell (
		HC_KEY key);
HC_EXTERNAL void HC_CDECL HC_Begin_Error_Handler_Search (
		void);
HC_EXTERNAL void HC_CDECL HC_Begin_Exit_Handler_Search (
		void);
HC_EXTERNAL void HC_CDECL HC_Begin_Font_Search (
		const char * display, const char * filter);
HC_EXTERNAL void HC_CDECL HC_Begin_Glyph_Search (
		void);
HC_EXTERNAL void HC_CDECL HC_Begin_Line_Style_Search (
		void);
HC_EXTERNAL void HC_CDECL HC_Begin_Local_Texture_Search (
		void);
HC_EXTERNAL void HC_CDECL HC_Begin_Named_Style_Search (
		void);
HC_EXTERNAL void HC_CDECL HC_Begin_Open_Item_Search (
		void);
HC_EXTERNAL void HC_CDECL HC_Begin_Open_Segment_Search (
		void);
HC_EXTERNAL void HC_CDECL HC_Begin_Segment_Search (
		const char * segspec);
HC_EXTERNAL void HC_CDECL HC_Begin_Shader_Search (
		void);
HC_EXTERNAL void HC_CDECL HC_Begin_Shell_Selection (
		void);
HC_EXTERNAL void HC_CDECL HC_Begin_Texture_Search (
		void);
HC_EXTERNAL void HC_CDECL HC_Bring_To_Front (
		const char * segment);
HC_EXTERNAL void HC_CDECL HC_Bring_To_Front_By_Key (
		HC_KEY key);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_Check_Double_Precision (
		HC_KEY key, const char * options);
HC_EXTERNAL void HC_CDECL HC_Close_Edge (
		void);
HC_EXTERNAL void HC_CDECL HC_Close_Face (
		void);
HC_EXTERNAL void HC_CDECL HC_Close_Geometry (
		void);
HC_EXTERNAL void HC_CDECL HC_Close_LOD (
		void);
HC_EXTERNAL void HC_CDECL HC_Close_Region (
		void);
HC_EXTERNAL void HC_CDECL HC_Close_Segment (
		void);
HC_EXTERNAL void HC_CDECL HC_Close_Trim (
		void);
HC_EXTERNAL void HC_CDECL HC_Close_Vertex (
		void);
HC_EXTERNAL int HC_CDECL HC_Collect_Area_Samples (
		double left, double right, double bottom, double top);
HC_EXTERNAL void HC_CDECL HC_Commit_Proxy (
		HC_KEY key, const char * options);
HC_EXTERNAL HC_KEY HC_CDECL HC_Compute_Boolean_Shell (
		int target_pcount, const HC_POINT * target_points, const HC_VECTOR * target_normals, int target_flist_length, const int * target_face_list, int tool_pcount, const HC_POINT * tool_points, const HC_VECTOR * tool_normals, int tool_flist_length, const int * tool_face_list, const char * options);
HC_EXTERNAL HC_KEY HC_CDECL HC_Compute_Boolean_Shell2 (
		int in_pt_count_target, const HC_POINT * in_pts_target, const HC_VECTOR * in_normals_target, int in_flist_len_target, const int * in_flist_target, int in_pt_count_tool, const HC_POINT * in_pts_tool, const HC_VECTOR * in_normals_tool, int in_flist_len_tool, const int * in_flist_tool, const char * option_string, const HC_RGB * in_color1, const HC_RGB * in_color2, const unsigned char * in_vis1, const unsigned char * in_vis2);
HC_EXTERNAL HC_KEY HC_CDECL HC_Compute_Capping_Shell (
		int shell_count, const HC_KEY * shell_list, const float * plane);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_Compute_Circumcuboid (
		const char * segment, HC_POINT * min, HC_POINT * max);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_Compute_Circumcuboid_By_Key (
		HC_KEY key, HC_POINT * min, HC_POINT * max);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_Compute_Circumsphere (
		const char * segment, HC_POINT * center, float * radius);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_Compute_Circumsphere_By_Key (
		HC_KEY key, HC_POINT * center, float * radius);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_Compute_Color (
		const char * name, const char * out_type, HC_RGB * out_value);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_Compute_Color_By_Value (
		const char * in_type, const HC_RGB * in_value, const char * out_type, HC_RGB * out_value);
HC_EXTERNAL void HC_CDECL HC_Compute_Convex_Hull (
		int pcount, const HC_POINT * points, int * fcount, int * face_list);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_Compute_Coordinates (
		const char * segment, const char * in_system, const HC_POINT * in_position, const char * out_system, HC_POINT * out_position);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_Compute_Coordinates_By_Path (
		int count, const HC_KEY * keys, const char * in_system, const HC_POINT * in_position, const char * out_system, HC_POINT * out_position);
HC_EXTERNAL void HC_CDECL HC_Compute_Cross_Product (
		const HC_VECTOR * vector1, const HC_VECTOR * vector2, HC_VECTOR * out_vector);
HC_EXTERNAL double HC_CDECL HC_Compute_Dot_Product (
		const HC_VECTOR * vector1, const HC_VECTOR * vector2);
HC_EXTERNAL void HC_CDECL HC_Compute_Face_Neighborhood (
		HC_KEY shell_key, int face_index, int * count, int * neighbors);
HC_EXTERNAL void HC_CDECL HC_Compute_Face_Neighborhood_Si (
		HC_KEY shell_key, int face_index, int * count);
#define HC_Compute_Face_Neighborhood_Size(shell_key, face_index, count) \
		HC_Compute_Face_Neighborhood_Si(shell_key, face_index, count)
HC_EXTERNAL void HC_CDECL HC_Compute_Identity_Matrix (
		float * out_matrix);
HC_EXTERNAL int HC_CDECL HC_Compute_Intersect_Polyline (
		int pcount1, const HC_POINT * points1, int flist_length1, const int * face_list1, int pcount2, const HC_POINT * points2, int flist_length2, const int * face_list2, int * polyline_count, int * polyline_lengths, int * polyline_points_count, HC_POINT * polyline_points);
HC_EXTERNAL void HC_CDECL HC_Compute_Matrix_Adjoint (
		const float * matrix, float * out_matrix);
HC_EXTERNAL double HC_CDECL HC_Compute_Matrix_Determinant (
		const float * matrix);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_Compute_Matrix_Inverse (
		const float * matrix, float * out_matrix);
HC_EXTERNAL void HC_CDECL HC_Compute_Matrix_Product (
		const float * matrix1, const float * matrix2, float * out_matrix);
HC_EXTERNAL void HC_CDECL HC_Compute_Minimized_Shell (
		int in_point_count, const HC_POINT * in_points, int in_face_list_length, const int * in_face_list, const char * option_string, int * out_point_count, HC_POINT * out_points, int * out_face_list_length, int * out_face_list, int * vertex_mapping, int * face_mapping);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_Compute_Normalized_Matrix (
		const float * matrix, float * out_matrix);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_Compute_Normalized_Vector (
		const HC_VECTOR * vector, HC_VECTOR * out_vector);
HC_EXTERNAL void HC_CDECL HC_Compute_Offaxis_Rotation (
		double x, double y, double z, double theta, float * out_matrix);
HC_EXTERNAL void HC_CDECL HC_Compute_Offcenter_Rotation (
		double alpha, double beta, double theta, double x, double y, double z, float * out_matrix);
HC_EXTERNAL void HC_CDECL HC_Compute_Optimized_Shell (
		int pcount, const HC_POINT * points, const HC_VECTOR * normals, int flist_length, const int * face_list, const char * options, int * new_pcount, HC_POINT * new_points, int * new_flist_length, int * new_face_list, int * point_mapping_list, int * face_mapping_list);
HC_EXTERNAL void HC_CDECL HC_Compute_Pline_From_Crv_Size (
		HC_KEY key, const char * options, int * pcount);
#define HC_Compute_Polyline_From_Curve_Size(key, options, pcount) \
		HC_Compute_Pline_From_Crv_Size(key, options, pcount)
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_Compute_Point_Inside_Shell (
		const HC_POINT * test_point, int point_count, const HC_POINT * points, int face_list_length, const int * face_list);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_Compute_Point_On_Shell (
		const HC_POINT * test_point, double tolerance, int point_count, const HC_POINT * points, int face_list_length, const int * face_list);
HC_EXTERNAL int HC_CDECL HC_Compute_Points_Distance_F_S2 (
		int test_pcount, const HC_POINT * test_pt, double tolerance, int point_count1, const HC_POINT * points1, int face_list_length1, const int * face_list1, const char * options, float * results, int * face_results);
#define HC_Compute_Points_Distance_From_Shell2(test_pcount, test_pt, tolerance, point_count1, points1, face_list_length1, face_list1, options, results, face_results) \
		HC_Compute_Points_Distance_F_S2(test_pcount, test_pt, tolerance, point_count1, points1, face_list_length1, face_list1, options, results, face_results)
HC_EXTERNAL int HC_CDECL HC_Compute_Points_Distance_F_Sh (
		int test_point_count, const HC_POINT * test_points, double tolerance, int point_count, const HC_POINT * points, int face_list_length, const int * face_list, const char * options, float * results);
#define HC_Compute_Points_Distance_From_Shell(test_point_count, test_points, tolerance, point_count, points, face_list_length, face_list, options, results) \
		HC_Compute_Points_Distance_F_Sh(test_point_count, test_points, tolerance, point_count, points, face_list_length, face_list, options, results)
HC_EXTERNAL void HC_CDECL HC_Compute_Points_Inside_Shell (
		int test_point_count, const HC_POINT * test_points, const HC_VECTOR * test_points_vectors, int point_count, const HC_POINT * points, int face_list_length, const int * face_list, const char * options, int * results);
HC_EXTERNAL int HC_CDECL HC_Compute_Points_On_Shell (
		int test_point_count, const HC_POINT * test_points, double tolerance, int point_count, const HC_POINT * points, int face_list_length, const int * face_list, int * results);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_Compute_Polygon_Normal (
		int count, const HC_POINT * points, const char * hand, HC_VECTOR * out_vector);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_Compute_Polygon_Plane (
		int count, const HC_POINT * points, const char * hand, HC_PLANE * out_plane);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_Compute_Polyline_From_Curve (
		HC_KEY key, const char * options, int * pcount, HC_POINT * points);
HC_EXTERNAL void HC_CDECL HC_Compute_Rotation_Matrix (
		double alpha, double beta, double theta, float * out_matrix);
HC_EXTERNAL void HC_CDECL HC_Compute_Scale_Matrix (
		double x, double y, double z, float * out_matrix);
HC_EXTERNAL void HC_CDECL HC_Compute_Screen_Extent (
		const char * segment, const char * options, float * left, float * right, float * bottom, float * top);
HC_EXTERNAL void HC_CDECL HC_Compute_Screen_Extent_By_Key (
		HC_KEY segKey, const char * options, float * left, float * right, float * bottom, float * top);
HC_EXTERNAL void HC_CDECL HC_Compute_Screen_Extent_By_Pat (
		int count, const HC_KEY * keys, const char * options, float * left, float * right, float * bottom, float * top);
#define HC_Compute_Screen_Extent_By_Path(count, keys, options, left, right, bottom, top) \
		HC_Compute_Screen_Extent_By_Pat(count, keys, options, left, right, bottom, top)
HC_EXTERNAL int HC_CDECL HC_Compute_Selection (
		const char * display, const char * start_seg, const char * action, double x, double y);
HC_EXTERNAL int HC_CDECL HC_Compute_Selection_By_Area (
		const char * display, const char * start_seg, const char * action, double left, double right, double bottom, double top);
HC_EXTERNAL int HC_CDECL HC_Compute_Selection_By_Key (
		const char * action, const char * start_seg, HC_KEY key, const float * matrix);
HC_EXTERNAL int HC_CDECL HC_Compute_Selection_By_Polygon (
		const char * display, const char * start_seg, const char * action, int pcount, const HC_POINT * points);
HC_EXTERNAL int HC_CDECL HC_Compute_Selection_By_Polylin (
		const char * display, const char * start_seg, const char * action, int pcount, const HC_POINT * points);
#define HC_Compute_Selection_By_Polyline(display, start_seg, action, pcount, points) \
		HC_Compute_Selection_By_Polylin(display, start_seg, action, pcount, points)
HC_EXTERNAL int HC_CDECL HC_Compute_Selection_By_Ray (
		const char * action, const char * start_seg, const HC_POINT * start_point, const HC_VECTOR * direction);
HC_EXTERNAL int HC_CDECL HC_Compute_Selection_By_Sh_W_Op (
		const char * action, const char * start_seg, int point_count, const HC_POINT * points, int face_list_length, const int * face_list, const char * options);
#define HC_Compute_Selection_By_Shell_With_Options(action, start_seg, point_count, points, face_list_length, face_list, options) \
		HC_Compute_Selection_By_Sh_W_Op(action, start_seg, point_count, points, face_list_length, face_list, options)
HC_EXTERNAL int HC_CDECL HC_Compute_Selection_By_Shell (
		const char * action, const char * start_seg, int point_count, const HC_POINT * points, int face_list_length, const int * face_list);
HC_EXTERNAL int HC_CDECL HC_Compute_Selection_By_Swept_2 (
		const char * action, const char * start_seg, HC_KEY shellkey, int sweepcount, const float * matrices, const char * options, float * bounding_box);
#define HC_Compute_Selection_By_Swept_Shell2(action, start_seg, shellkey, sweepcount, matrices, options, bounding_box) \
		HC_Compute_Selection_By_Swept_2(action, start_seg, shellkey, sweepcount, matrices, options, bounding_box)
HC_EXTERNAL int HC_CDECL HC_Compute_Selection_By_Swept_S (
		const char * action, const char * start_seg, HC_KEY shellkey, int number_of_sweeps, const float * matrices, const char * options);
#define HC_Compute_Selection_By_Swept_Shell(action, start_seg, shellkey, number_of_sweeps, matrices, options) \
		HC_Compute_Selection_By_Swept_S(action, start_seg, shellkey, number_of_sweeps, matrices, options)
HC_EXTERNAL int HC_CDECL HC_Compute_Selection_By_Volume (
		const char * display, const char * start_seg, const char * action, double left, double right, double bottom, double top, double hither, double yon);
HC_EXTERNAL float HC_CDECL HC_Compute_Selection_Dist_By_Sh (
		const char * action, const char * start_segment, int pcount, const HC_POINT * points, int flist_length, const int * faces, const char * options);
#define HC_Compute_Selection_Distance_By_Shell(action, start_segment, pcount, points, flist_length, faces, options) \
		HC_Compute_Selection_Dist_By_Sh(action, start_segment, pcount, points, flist_length, faces, options)
HC_EXTERNAL void HC_CDECL HC_Compute_Shell_Enclosure (
		int point_count1, const HC_POINT * points1, int face_list_length1, const int * face_list1, int point_count2, const HC_POINT * points2, int face_list_length2, const int * face_list2, char * result);
HC_EXTERNAL HC_KEY HC_CDECL HC_Compute_Subshell (
		HC_KEY source, int keep_fcount, const int * keep_faces, int keep_pcount, const int * keep_pts1, const int * keep_pts2, int * face_map, int * pts_map);
HC_EXTERNAL HC_KEY HC_CDECL HC_Compute_Swept_Shell (
		HC_KEY shellkey, const HC_VECTOR * direction_in, HC_KEY * front_key, HC_KEY * back_key, HC_KEY * side_key, const char * options);
HC_EXTERNAL void HC_CDECL HC_Compute_Swept_Shell_Along_P (
		HC_KEY shellkey, int number_of_sweeps, const float * matrices, const char * options);
#define HC_Compute_Swept_Shell_Along_Path(shellkey, number_of_sweeps, matrices, options) \
		HC_Compute_Swept_Shell_Along_P(shellkey, number_of_sweeps, matrices, options)
HC_EXTERNAL void HC_CDECL HC_Compute_Text16_From_Text (
		const char * intext, const char * encoding, int * outcount, unsigned short * outtext16);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_Compute_Text_Extent (
		const char * segment, const char * text, float * xsize, float * ysize);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_Compute_Text_Extent_By_Key (
		HC_KEY key, float * xfrac, float * yfrac);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_Compute_Text_Extent_By_Path (
		int count, const HC_KEY * keys, const char * text, float * xfrac, float * yfrac);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_Compute_Text_Extent_Enc_Path (
		int count, const HC_KEY * keys, const char * encoding, const void * text, float * xfrac, float * yfrac);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_Compute_Text_Extent_With_Enc (
		const char * segment, const char * encoding, const void * text, float * xfrac, float * yfrac);
#define HC_Compute_Text_Extent_With_Encoding(segment, encoding, text, xfrac, yfrac) \
		HC_Compute_Text_Extent_With_Enc(segment, encoding, text, xfrac, yfrac)
HC_EXTERNAL void HC_CDECL HC_Compute_Text_From_Text16 (
		int incount, const unsigned short * intext16, const char * encoding, char * outtext);
HC_EXTERNAL void HC_CDECL HC_Compute_Text_Outline (
		const char * segment, const char * text, int * pcount, HC_POINT * points, int * flist_length, int * face_list);
HC_EXTERNAL void HC_CDECL HC_Compute_Text_Outline_By_Key (
		HC_KEY key, int * pcount, HC_POINT * points, int * flist_length, int * face_list);
HC_EXTERNAL void HC_CDECL HC_Compute_Text_Outline_By_Path (
		int count, const HC_KEY * keys, const char * text, int * pcount, HC_POINT * points, int * flist_length, int * face_list);
HC_EXTERNAL void HC_CDECL HC_Compute_Text_Outline_Size (
		const char * segment, const char * text, int * pcount, int * flist_length);
HC_EXTERNAL void HC_CDECL HC_Compute_Text_Outline_Size_By (
		HC_KEY key, int * pcount, int * flist_length);
#define HC_Compute_Text_Outline_Size_By_Key(key, pcount, flist_length) \
		HC_Compute_Text_Outline_Size_By(key, pcount, flist_length)
HC_EXTERNAL void HC_CDECL HC_Compute_Text_Outline_Size_Pa (
		int count, const HC_KEY * keys, const char * text, int * pcount, int * flist_length);
#define HC_Compute_Text_Outline_Size_By_Path(count, keys, text, pcount, flist_length) \
		HC_Compute_Text_Outline_Size_Pa(count, keys, text, pcount, flist_length)
HC_EXTERNAL void HC_CDECL HC_Compute_Text_Outline_Size_Wi (
		const char * segment, const char * encoding, const char * etext, int * pcount, int * flist_length);
#define HC_Compute_Text_Outline_Size_With_Encoding(segment, encoding, etext, pcount, flist_length) \
		HC_Compute_Text_Outline_Size_Wi(segment, encoding, etext, pcount, flist_length)
HC_EXTERNAL void HC_CDECL HC_Compute_Text_Outline_With_En (
		const char * segment, const char * encoding, const char * etext, int * pcount, HC_POINT * points, int * flist_length, int * face_list);
#define HC_Compute_Text_Outline_With_Encoding(segment, encoding, etext, pcount, points, flist_length, face_list) \
		HC_Compute_Text_Outline_With_En(segment, encoding, etext, pcount, points, flist_length, face_list)
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_Compute_Transform (
		const char * segment, const char * in_system, const char * out_system, float * matrix);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_Compute_Transform_By_Path (
		int count, const HC_KEY * keys, const char * in_system, const char * out_system, float * matrix);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_Compute_Transformed_Plane (
		const HC_PLANE * plane, const float * matrix, HC_PLANE * out_plane);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_Compute_Transformed_Planes (
		int count, const HC_PLANE * planes, const float * matrix, HC_PLANE * out_planes);
HC_EXTERNAL void HC_CDECL HC_Compute_Transformed_Points (
		int count, const HC_POINT * points, const float * matrix, HC_POINT * out_points);
HC_EXTERNAL void HC_CDECL HC_Compute_Translation_Matrix (
		double x, double y, double z, float * out_matrix);
HC_EXTERNAL void HC_CDECL HC_Compute_Triangulation (
		const HC_POINT * points, int flist_length, const int * face_list, int * tlist_length, int * triangle_list);
HC_EXTERNAL void HC_CDECL HC_Compute_Triangulation_Size (
		const HC_POINT * points, int flist_length, const int * face_list, int * tlist_length);
HC_EXTERNAL double HC_CDECL HC_Compute_Vector_Angle (
		const HC_VECTOR * vector, const HC_VECTOR * vector2);
HC_EXTERNAL double HC_CDECL HC_Compute_Vector_Length (
		const HC_VECTOR * vector);
HC_EXTERNAL HC_KEY HC_CDECL HC_Conditional_Include (
		const char * include, const char * condition);
HC_EXTERNAL HC_KEY HC_CDECL HC_Conditional_Include_By_Key (
		HC_KEY seg_key, const char * condition);
HC_EXTERNAL HC_KEY HC_CDECL HC_Conditional_Include_Key_By_Ke (
		HC_KEY key, const char * condition, HC_KEY includer_key);
#define HC_Conditional_Include_Key_By_Key(key, condition, includer_key) \
		HC_Conditional_Include_Key_By_Ke(key, condition, includer_key)
HC_EXTERNAL HC_KEY HC_CDECL HC_Conditional_Named_Style (
		const char * style_name, const char * condition);
HC_EXTERNAL HC_KEY HC_CDECL HC_Conditional_Named_Style_By_Key (
		HC_KEY target_segment, const char * style_name, const char * condition);
HC_EXTERNAL HC_KEY HC_CDECL HC_Conditional_Reference (
		const char * segname, const char * condition);
HC_EXTERNAL HC_KEY HC_CDECL HC_Conditional_Reference_By_Key (
		HC_KEY geom_key, const char * condition);
HC_EXTERNAL HC_KEY HC_CDECL HC_Conditional_Style (
		const char * style, const char * conditions);
HC_EXTERNAL HC_KEY HC_CDECL HC_Conditional_Style_By_Key (
		HC_KEY seg_key, const char * conditions);
HC_EXTERNAL void HC_CDECL HC_Control_Update (
		const char * segment, const char * options);
HC_EXTERNAL void HC_CDECL HC_Control_Update_Area (
		const char * segment, double left, double right, double bottom, double top, const char * options);
HC_EXTERNAL void HC_CDECL HC_Control_Update_Area_By_Key (
		HC_KEY key, double left, double right, double bottom, double top, const char * options);
HC_EXTERNAL void HC_CDECL HC_Control_Update_By_Key (
		HC_KEY key, const char * options);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_Convert_Precision (
		HC_KEY key, const char * options);
HC_EXTERNAL HC_KEY HC_CDECL HC_Copy_Geometry (
		HC_KEY key, const char * segment);
HC_EXTERNAL HC_KEY HC_CDECL HC_Copy_Segment (
		const char * old_seg_name, const char * new_seg_name);
HC_EXTERNAL HC_KEY HC_CDECL HC_Copy_Segment_With_Mapping (
		const char * source_seg, const char * destination, int count, const HC_KEY * interesting_keys, HC_KEY * copied_keys);
HC_EXTERNAL HC_KEY HC_CDECL HC_Create_Proxy (
		HC_KEY key);
HC_EXTERNAL HC_KEY HC_CDECL HC_Create_Segment (
		const char * segment);
HC_EXTERNAL HC_KEY HC_CDECL HC_Create_Segment_Key_By_Key (
		HC_KEY parent_segment, const char * child_segment_name);
HC_EXTERNAL void HC_CDECL HC_Create_Trim_Collection (
		void);
HC_EXTERNAL void HC_CDECL HC_DAppend_Modelling_Matrix (
		const double * array);
HC_EXTERNAL HC_KEY HC_CDECL HC_DCompute_Boolean_Shell (
		int in_pt_count_target, const HC_DPOINT * in_pts_target, const HC_VECTOR * in_normals_target, int in_flist_len_target, const int * in_flist_target, int in_pt_count_tool, const HC_DPOINT * in_pts_tool, const HC_VECTOR * in_normals_tool, int in_flist_len_tool, const int * in_flist_tool, const char * option_string, const HC_RGB * in_color1, const HC_RGB * in_color2, const unsigned char * in_vis1, const unsigned char * in_vis2);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_DCompute_Circumcuboid (
		const char * segment, HC_DPOINT * min, HC_DPOINT * max);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_DCompute_Circumcuboid_By_Key (
		HC_KEY key, HC_DPOINT * min, HC_DPOINT * max);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_DCompute_Circumsphere (
		const char * segment, HC_DPOINT * center, double * radius);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_DCompute_Circumsphere_By_Key (
		HC_KEY key, HC_DPOINT * center, double * radius);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_DCompute_Coordinates (
		const char * segment, const char * in_system, const HC_DPOINT * in_position, const char * out_system, HC_DPOINT * out_position);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_DCompute_Coordinates_By_Path (
		int count, const HC_KEY * keys, const char * in_system, const HC_DPOINT * in_position, const char * out_system, HC_DPOINT * out_position);
HC_EXTERNAL void HC_CDECL HC_DCompute_Cross_Product (
		const HC_DVECTOR * vector1, const HC_DVECTOR * vector2, HC_DVECTOR * out_vector);
HC_EXTERNAL double HC_CDECL HC_DCompute_Dot_Product (
		const HC_DVECTOR * vector1, const HC_DVECTOR * vector2);
HC_EXTERNAL void HC_CDECL HC_DCompute_Identity_Matrix (
		double * out_matrix);
HC_EXTERNAL void HC_CDECL HC_DCompute_Matrix_Adjoint (
		const double * matrix, double * out_matrix);
HC_EXTERNAL double HC_CDECL HC_DCompute_Matrix_Determinant (
		const double * matrix);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_DCompute_Matrix_Inverse (
		const double * matrix, double * out_matrix);
HC_EXTERNAL void HC_CDECL HC_DCompute_Matrix_Product (
		const double * matrix1, const double * matrix2, double * out_matrix);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_DCompute_Normalized_Matrix (
		const double * matrix, double * out_matrix);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_DCompute_Normalized_Vector (
		const HC_DVECTOR * vector, HC_DVECTOR * out_vector);
HC_EXTERNAL void HC_CDECL HC_DCompute_Offaxis_Rotation (
		double x, double y, double z, double theta, double * out_matrix);
HC_EXTERNAL void HC_CDECL HC_DCompute_Offcenter_Rotation (
		double alpha, double beta, double theta, double x, double y, double z, double * out_matrix);
HC_EXTERNAL void HC_CDECL HC_DCompute_Optimized_Shell (
		int pcount, const HC_DPOINT * points, const HC_VECTOR * normals, int flist_length, const int * face_list, const char * options, int * new_pcount, HC_DPOINT * new_points, int * new_flist_length, int * new_face_list, int * point_mapping_list, int * face_mapping_list);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_DCompute_Point_Inside_Shell (
		const HC_DPOINT * test_point, int point_count, const HC_DPOINT * points, int face_list_length, const int * face_list);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_DCompute_Point_On_Shell (
		const HC_DPOINT * test_point, double tolerance, int point_count, const HC_DPOINT * points, int face_list_length, const int * face_list);
HC_EXTERNAL int HC_CDECL HC_DCompute_Points_Distance_FS2 (
		int test_pcount, const HC_DPOINT * test_pt, double tolerance, int point_count1, const HC_DPOINT * points1, int face_list_length1, const int * face_list1, const char * options, double * results, int * face_results);
#define HC_DCompute_Points_Distance_From_Shell2(test_pcount, test_pt, tolerance, point_count1, points1, face_list_length1, face_list1, options, results, face_results) \
		HC_DCompute_Points_Distance_FS2(test_pcount, test_pt, tolerance, point_count1, points1, face_list_length1, face_list1, options, results, face_results)
HC_EXTERNAL int HC_CDECL HC_DCompute_Points_Distance_F_S (
		int test_point_count, const HC_DPOINT * test_points, double tolerance, int point_count, const HC_DPOINT * points, int face_list_length, const int * face_list, const char * options, double * results);
#define HC_DCompute_Points_Distance_From_Shell(test_point_count, test_points, tolerance, point_count, points, face_list_length, face_list, options, results) \
		HC_DCompute_Points_Distance_F_S(test_point_count, test_points, tolerance, point_count, points, face_list_length, face_list, options, results)
HC_EXTERNAL void HC_CDECL HC_DCompute_Points_Inside_Shell (
		int test_point_count, const HC_DPOINT * test_points, const HC_DVECTOR * test_points_vectors, int point_count, const HC_DPOINT * points, int face_list_length, const int * face_list, const char * options, int * results);
HC_EXTERNAL int HC_CDECL HC_DCompute_Points_On_Shell (
		int test_point_count, const HC_DPOINT * test_points, double tolerance, int point_count, const HC_DPOINT * points, int face_list_length, const int * face_list, int * results);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_DCompute_Polygon_Normal (
		int count, const HC_DPOINT * points, const char * hand, HC_DVECTOR * out_vector);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_DCompute_Polygon_Plane (
		int count, const HC_DPOINT * points, const char * hand, HC_DPLANE * out_plane);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_DCompute_Polyline_From_Curve (
		HC_KEY key, const char * options, int * pcount, HC_DPOINT * points);
HC_EXTERNAL void HC_CDECL HC_DCompute_Rotation_Matrix (
		double alpha, double beta, double theta, double * out_matrix);
HC_EXTERNAL void HC_CDECL HC_DCompute_Scale_Matrix (
		double x, double y, double z, double * out_matrix);
HC_EXTERNAL int HC_CDECL HC_DCompute_Selection_By_Key (
		const char * action, const char * start_seg, HC_KEY key, const double * matrix);
HC_EXTERNAL int HC_CDECL HC_DCompute_Selection_By_Ray (
		const char * action, const char * start_seg, const HC_DPOINT * start_point, const HC_DVECTOR * direction);
HC_EXTERNAL int HC_CDECL HC_DCompute_Selection_By_Sh_W_O (
		const char * action, const char * start_seg, int point_count, const HC_DPOINT * points, int face_list_length, const int * face_list, const char * options);
#define HC_DCompute_Selection_By_Shell_With_Options(action, start_seg, point_count, points, face_list_length, face_list, options) \
		HC_DCompute_Selection_By_Sh_W_O(action, start_seg, point_count, points, face_list_length, face_list, options)
HC_EXTERNAL int HC_CDECL HC_DCompute_Selection_By_Shell (
		const char * action, const char * start_seg, int point_count, const HC_DPOINT * points, int face_list_length, const int * face_list);
HC_EXTERNAL int HC_CDECL HC_DCompute_Selection_By_Swpt_2 (
		const char * action, const char * start_seg, HC_KEY shellkey, int sweepcount, const double * matrices, const char * options, double * bounding_box);
#define HC_DCompute_Selection_By_Swept_Shell2(action, start_seg, shellkey, sweepcount, matrices, options, bounding_box) \
		HC_DCompute_Selection_By_Swpt_2(action, start_seg, shellkey, sweepcount, matrices, options, bounding_box)
HC_EXTERNAL int HC_CDECL HC_DCompute_Selection_By_Swpt_S (
		const char * action, const char * start_seg, HC_KEY shellkey, int number_of_sweeps, const double * matrices, const char * options);
#define HC_DCompute_Selection_By_Swept_Shell(action, start_seg, shellkey, number_of_sweeps, matrices, options) \
		HC_DCompute_Selection_By_Swpt_S(action, start_seg, shellkey, number_of_sweeps, matrices, options)
HC_EXTERNAL double HC_CDECL HC_DCompute_Selection_Dist_By_S (
		const char * action, const char * start_segment, int pcount, const HC_DPOINT * points, int flist_length, const int * faces, const char * options);
#define HC_DCompute_Selection_Distance_By_Shell(action, start_segment, pcount, points, flist_length, faces, options) \
		HC_DCompute_Selection_Dist_By_S(action, start_segment, pcount, points, flist_length, faces, options)
HC_EXTERNAL void HC_CDECL HC_DCompute_Shell_Enclosure (
		int point_count1, const HC_DPOINT * points1, int face_list_length1, const int * face_list1, int point_count2, const HC_DPOINT * points2, int face_list_length2, const int * face_list2, char * result);
HC_EXTERNAL HC_KEY HC_CDECL HC_DCompute_Swept_Shell (
		HC_KEY shellkey, const HC_DVECTOR * direction_in, HC_KEY * front_key, HC_KEY * back_key, HC_KEY * side_key, const char * options);
HC_EXTERNAL void HC_CDECL HC_DCompute_Swept_Shell_Along_P (
		HC_KEY shellkey, int number_of_sweeps, const double * matrices, const char * options);
#define HC_DCompute_Swept_Shell_Along_Path(shellkey, number_of_sweeps, matrices, options) \
		HC_DCompute_Swept_Shell_Along_P(shellkey, number_of_sweeps, matrices, options)
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_DCompute_Transform (
		const char * segment, const char * in_system, const char * out_system, double * matrix);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_DCompute_Transform_By_Path (
		int count, const HC_KEY * keys, const char * in_system, const char * out_system, double * matrix);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_DCompute_Transformed_Plane (
		const HC_DPLANE * plane, const double * matrix, HC_DPLANE * out_plane);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_DCompute_Transformed_Planes (
		int count, const HC_DPLANE * planes, const double * matrix, HC_DPLANE * out_planes);
HC_EXTERNAL void HC_CDECL HC_DCompute_Transformed_Points (
		int count, const HC_DPOINT * points, const double * matrix, HC_DPOINT * out_points);
HC_EXTERNAL void HC_CDECL HC_DCompute_Translation_Matrix (
		double x, double y, double z, double * out_matrix);
HC_EXTERNAL double HC_CDECL HC_DCompute_Vector_Angle (
		const HC_DVECTOR * vector, const HC_DVECTOR * vector2);
HC_EXTERNAL double HC_CDECL HC_DCompute_Vector_Length (
		const HC_DVECTOR * vector);
HC_EXTERNAL void HC_CDECL HC_DDolly_Camera (
		double x, double y, double z);
HC_EXTERNAL void HC_CDECL HC_DEdit_Circle (
		HC_KEY key, const HC_DPOINT * point1, const HC_DPOINT * point2, const HC_DPOINT * point3);
HC_EXTERNAL void HC_CDECL HC_DEdit_Circle_By_Radius (
		HC_KEY key, const HC_DPOINT * center, double radius, const HC_DVECTOR * vector);
HC_EXTERNAL void HC_CDECL HC_DEdit_Circular_Arc (
		HC_KEY key, const HC_DPOINT * point1, const HC_DPOINT * point2, const HC_DPOINT * point3);
HC_EXTERNAL void HC_CDECL HC_DEdit_Circular_Chord (
		HC_KEY key, const HC_DPOINT * point1, const HC_DPOINT * point2, const HC_DPOINT * point3);
HC_EXTERNAL void HC_CDECL HC_DEdit_Circular_Wedge (
		HC_KEY key, const HC_DPOINT * point1, const HC_DPOINT * point2, const HC_DPOINT * point3);
HC_EXTERNAL void HC_CDECL HC_DEdit_Cutting_Plane (
		HC_KEY key, double a, double b, double c, double d);
HC_EXTERNAL void HC_CDECL HC_DEdit_Cutting_Section (
		HC_KEY key, int ioffset, int ndelete, int insert, const HC_DPLANE * iplanes);
HC_EXTERNAL void HC_CDECL HC_DEdit_Cylinder (
		HC_KEY key, const HC_DPOINT * p1, const HC_DPOINT * p2, double radius, const char * cap);
HC_EXTERNAL void HC_CDECL HC_DEdit_Ellipse (
		HC_KEY key, const HC_DPOINT * center, const HC_DPOINT * major, const HC_DPOINT * minor);
HC_EXTERNAL void HC_CDECL HC_DEdit_Elliptical_Arc (
		HC_KEY key, const HC_DPOINT * center, const HC_DPOINT * major, const HC_DPOINT * minor, double start, double end);
HC_EXTERNAL void HC_CDECL HC_DEdit_Grid (
		HC_KEY key, const char * type, const HC_DPOINT * origin, const HC_DPOINT * ref1, const HC_DPOINT * ref2, int count1, int count2);
HC_EXTERNAL void HC_CDECL HC_DEdit_Infinite_Line (
		HC_KEY key, double xa, double ya, double za, double xb, double yb, double zb);
HC_EXTERNAL void HC_CDECL HC_DEdit_Infinite_Ray (
		HC_KEY key, double xa, double ya, double za, double xb, double yb, double zb);
HC_EXTERNAL void HC_CDECL HC_DEdit_Line (
		HC_KEY key, double xa, double ya, double za, double xb, double yb, double zb);
HC_EXTERNAL void HC_CDECL HC_DEdit_Marker (
		HC_KEY key, double x, double y, double z);
HC_EXTERNAL void HC_CDECL HC_DEdit_Mesh (
		HC_KEY key, int row_offset, int column_offset, int row_count, int column_count, const HC_DPOINT * points);
HC_EXTERNAL void HC_CDECL HC_DEdit_NURBS_Curve (
		HC_KEY key, int cp_offset, int knot_offset, int cp_count, int knot_count, const HC_DPOINT * control_points, const float * weights, const float * knots);
HC_EXTERNAL void HC_CDECL HC_DEdit_NURBS_Surface (
		HC_KEY key, int cp_offset, int weights_offset, int u_knot_offset, int v_knot_offset, int cp_replace_count, int weights_replace_count, int u_knot_replace_count, int v_knot_replace_count, const HC_DPOINT * points, const float * weights, const float * u_knots, const float * v_knots);
HC_EXTERNAL void HC_CDECL HC_DEdit_NURBS_Surface_Points (
		HC_KEY key, int cp_offset, int cp_replace_count, const HC_DPOINT * points);
HC_EXTERNAL void HC_CDECL HC_DEdit_PolyCylinder (
		HC_KEY key, int p_count, const HC_DPOINT * pts, int r_count, const double * radii, const char * capping);
HC_EXTERNAL void HC_CDECL HC_DEdit_PolyCylinder_Points (
		HC_KEY key, int offset, int delete_count, int insert_count, const HC_DPOINT * points);
HC_EXTERNAL void HC_CDECL HC_DEdit_PolyCylinder_Radii (
		HC_KEY key, int offset, int delete_count, int insert_count, const double * radii);
HC_EXTERNAL void HC_CDECL HC_DEdit_PolyCylinder_With_Basis (
		HC_KEY key, int p_count, const HC_DPOINT * pts, int r_count, const double * radii, const char * capping, const HC_DVECTOR * basis);
HC_EXTERNAL void HC_CDECL HC_DEdit_Polygon (
		HC_KEY key, int offset, int ndelete, int insert, const HC_DPOINT * points);
HC_EXTERNAL void HC_CDECL HC_DEdit_Polyline (
		HC_KEY key, int offset, int ndelete, int insert, const HC_DPOINT * points);
HC_EXTERNAL void HC_CDECL HC_DEdit_Shell_Points (
		HC_KEY key, int ioffset, int ndelete, int insert, const HC_DPOINT * points);
HC_EXTERNAL void HC_CDECL HC_DEdit_Sphere (
		HC_KEY key, const HC_DPOINT * center, double radius, const HC_DVECTOR * axis, const HC_DVECTOR * ortho);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_DFilter_Circumcuboid (
		const char * segment, const char * options, HC_DPOINT * min, HC_DPOINT * max);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_DFilter_Circumcuboid_By_Key (
		HC_KEY segKey, const char * options, HC_DPOINT * min, HC_DPOINT * max);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_DFilter_Circumsphere (
		const char * segment, const char * options, HC_DPOINT * center, double * radius);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_DFilter_Circumsphere_By_Key (
		HC_KEY segKey, const char * options, HC_DPOINT * center, double * radius);
HC_EXTERNAL HC_KEY HC_CDECL HC_DInsert_Area_Light (
		int count, const HC_DPOINT * points, const char * listptr);
HC_EXTERNAL HC_KEY HC_CDECL HC_DInsert_Circle (
		const HC_DPOINT * point1, const HC_DPOINT * point2, const HC_DPOINT * point3);
HC_EXTERNAL HC_KEY HC_CDECL HC_DInsert_Circle_By_Radius (
		const HC_DPOINT * center, double radius, const HC_DVECTOR * normal);
HC_EXTERNAL HC_KEY HC_CDECL HC_DInsert_Circular_Arc (
		const HC_DPOINT * point1, const HC_DPOINT * point2, const HC_DPOINT * point3);
HC_EXTERNAL HC_KEY HC_CDECL HC_DInsert_Circular_Chord (
		const HC_DPOINT * point1, const HC_DPOINT * point2, const HC_DPOINT * point3);
HC_EXTERNAL HC_KEY HC_CDECL HC_DInsert_Circular_Wedge (
		const HC_DPOINT * point1, const HC_DPOINT * point2, const HC_DPOINT * point3);
HC_EXTERNAL HC_KEY HC_CDECL HC_DInsert_Compressed_Image (
		double x, double y, double z, const char * format, int width, int height, int size, const void * data);
HC_EXTERNAL HC_KEY HC_CDECL HC_DInsert_Cutting_Plane (
		double a, double b, double c, double d);
HC_EXTERNAL HC_KEY HC_CDECL HC_DInsert_Cutting_Section (
		int count, const HC_DPLANE * planes);
HC_EXTERNAL HC_KEY HC_CDECL HC_DInsert_Cylinder (
		const HC_DPOINT * center_point_1, const HC_DPOINT * center_point_2, double radius, const char * cap);
HC_EXTERNAL HC_KEY HC_CDECL HC_DInsert_Ellipse (
		const HC_DPOINT * center, const HC_DPOINT * major, const HC_DPOINT * minor);
HC_EXTERNAL HC_KEY HC_CDECL HC_DInsert_Elliptical_Arc (
		const HC_DPOINT * center, const HC_DPOINT * major, const HC_DPOINT * minor, double start, double end);
HC_EXTERNAL HC_KEY HC_CDECL HC_DInsert_Grid (
		const char * type, const HC_DPOINT * origin, const HC_DPOINT * ref1, const HC_DPOINT * ref2, int count1, int count2);
HC_EXTERNAL HC_KEY HC_CDECL HC_DInsert_Image (
		double x, double y, double z, const char * format, int width, int height, const void * data);
HC_EXTERNAL HC_KEY HC_CDECL HC_DInsert_Image_By_Ref (
		double x, double y, double z, const char * format, int width, int height, const void * data);
HC_EXTERNAL HC_KEY HC_CDECL HC_DInsert_Infinite_Line (
		double xa, double ya, double za, double xb, double yb, double zb);
HC_EXTERNAL HC_KEY HC_CDECL HC_DInsert_Infinite_Ray (
		double xa, double ya, double za, double xb, double yb, double zb);
HC_EXTERNAL HC_KEY HC_CDECL HC_DInsert_Ink (
		double x, double y, double z);
HC_EXTERNAL HC_KEY HC_CDECL HC_DInsert_Line (
		double xa, double ya, double za, double xb, double yb, double zb);
HC_EXTERNAL HC_KEY HC_CDECL HC_DInsert_Local_Light (
		double x, double y, double z);
HC_EXTERNAL HC_KEY HC_CDECL HC_DInsert_Marker (
		double x, double y, double z);
HC_EXTERNAL HC_KEY HC_CDECL HC_DInsert_Mesh (
		int rows, int columns, const HC_DPOINT * points);
HC_EXTERNAL HC_KEY HC_CDECL HC_DInsert_Mesh_By_Ref (
		int rows, int columns, const HC_DPOINT * points);
HC_EXTERNAL HC_KEY HC_CDECL HC_DInsert_NURBS_Curve (
		int degree, int cpcount, const HC_DPOINT * control_points, const float * weights, const float * knots, double start_u, double end_u);
HC_EXTERNAL HC_KEY HC_CDECL HC_DInsert_NURBS_Surface (
		int u_degree, int v_degree, int u_count, int v_count, const HC_DPOINT * points, const float * weights, const float * u_knots, const float * v_knots);
HC_EXTERNAL HC_KEY HC_CDECL HC_DInsert_PolyCylinder (
		int p_count, const HC_DPOINT * pts, int r_count, const double * radii, const char * capping);
HC_EXTERNAL HC_KEY HC_CDECL HC_DInsert_Polygon (
		int count, const HC_DPOINT * points);
HC_EXTERNAL HC_KEY HC_CDECL HC_DInsert_Polyline (
		int count, const HC_DPOINT * points);
HC_EXTERNAL HC_KEY HC_CDECL HC_DInsert_Shell (
		int point_count, const HC_DPOINT * points, int face_list_length, const int * face_list);
HC_EXTERNAL HC_KEY HC_CDECL HC_DInsert_Shell_By_Ref (
		int pcount, const HC_DPOINT * points, int flist_length, const int * face_list);
HC_EXTERNAL HC_KEY HC_CDECL HC_DInsert_Shell_By_Tristrips (
		int pcount, const HC_DPOINT * points, int tristrips_length, const int * tristrips, int face_indices_length, const int * face_indices);
HC_EXTERNAL HC_KEY HC_CDECL HC_DInsert_Sphere (
		const HC_DPOINT * center, double radius, const HC_DVECTOR * axis, const HC_DVECTOR * ortho);
HC_EXTERNAL HC_KEY HC_CDECL HC_DInsert_Spot_Light (
		const HC_DPOINT * position, const HC_DPOINT * target, const char * list);
HC_EXTERNAL HC_KEY HC_CDECL HC_DInsert_Text (
		double x, double y, double z, const char * text);
HC_EXTERNAL HC_KEY HC_CDECL HC_DInsert_Text_With_Encoding (
		double x, double y, double z, const char * encoding, const void * text);
HC_EXTERNAL void HC_CDECL HC_DMove_Image (
		HC_KEY key, double x, double y, double z);
HC_EXTERNAL void HC_CDECL HC_DOrbit_Camera (
		double horiz, double vert);
HC_EXTERNAL int HC_CDECL HC_DPShow_Net_Camera (
		int key_count, const HC_KEY * path_keys, HC_DPOINT * position, HC_DPOINT * target, HC_DVECTOR * up, double * width, double * height, char * projection);
HC_EXTERNAL int HC_CDECL HC_DPShow_Net_Camera_By_Volume (
		int count, const HC_KEY * keys, char * proj, double * xmin, double * xmax, double * ymin, double * ymax);
HC_EXTERNAL int HC_CDECL HC_DPShow_Net_Camera_Field (
		int key_count, const HC_KEY * path_keys, double * width, double * height);
HC_EXTERNAL int HC_CDECL HC_DPShow_Net_Camera_Position (
		int key_count, const HC_KEY * path_keys, double * x, double * y, double * z);
HC_EXTERNAL int HC_CDECL HC_DPShow_Net_Camera_Target (
		int key_count, const HC_KEY * path_keys, double * x, double * y, double * z);
HC_EXTERNAL int HC_CDECL HC_DPShow_Net_Camera_Up_Vector (
		int key_count, const HC_KEY * path_keys, double * x, double * y, double * z);
HC_EXTERNAL int HC_CDECL HC_DPShow_Net_Clip_Region (
		int count, const HC_KEY * keys, int * loopCount, int * loopSizeArray, HC_DPOINT * points, char * spec);
HC_EXTERNAL int HC_CDECL HC_DPShow_Net_Clip_Region_Size (
		int count, const HC_KEY * keys, int * loopCount, int * totalPointCount, char * spec);
HC_EXTERNAL int HC_CDECL HC_DPShow_Net_Modelling_Matrix (
		int key_count, const HC_KEY * path_keys, double * matrix);
HC_EXTERNAL void HC_CDECL HC_DPan_Camera (
		double horiz, double vert);
HC_EXTERNAL void HC_CDECL HC_DRoll_Camera (
		double angle);
HC_EXTERNAL void HC_CDECL HC_DRotate_Object (
		double theta, double phi, double psi);
HC_EXTERNAL void HC_CDECL HC_DRotate_Object_Offaxis (
		double x, double y, double z, double theta);
HC_EXTERNAL void HC_CDECL HC_DScale_Object (
		double x, double y, double z);
HC_EXTERNAL void HC_CDECL HC_DSet_Bounding_Cuboid (
		const HC_DPOINT * min, const HC_DPOINT * max);
HC_EXTERNAL void HC_CDECL HC_DSet_Bounding_Sphere (
		const HC_DPOINT * center, double radius);
HC_EXTERNAL void HC_CDECL HC_DSet_Camera (
		const HC_DPOINT * position, const HC_DPOINT * target, const HC_DVECTOR * up, double width, double height, const char * projection);
HC_EXTERNAL void HC_CDECL HC_DSet_Camera_By_Volume (
		const char * proj, double xmin, double xmax, double ymin, double ymax);
HC_EXTERNAL void HC_CDECL HC_DSet_Camera_Field (
		double width, double height);
HC_EXTERNAL void HC_CDECL HC_DSet_Camera_Position (
		double x, double y, double z);
HC_EXTERNAL void HC_CDECL HC_DSet_Camera_Target (
		double x, double y, double z);
HC_EXTERNAL void HC_CDECL HC_DSet_Camera_Up_Vector (
		double x, double y, double z);
HC_EXTERNAL void HC_CDECL HC_DSet_Circular_Center (
		double x, double y, double z);
HC_EXTERNAL void HC_CDECL HC_DSet_Circular_Center_By_Key (
		HC_KEY key, double x, double y, double z);
HC_EXTERNAL void HC_CDECL HC_DSet_Complex_Clip_Region (
		int loops, const int * lengths, const HC_DPOINT * points, const char * options);
HC_EXTERNAL void HC_CDECL HC_DSet_Modelling_Matrix (
		const double * matrix);
HC_EXTERNAL void HC_CDECL HC_DSet_Polygonal_Clip_Region (
		int count, const HC_DPOINT * points, const char * options);
HC_EXTERNAL void HC_CDECL HC_DShow_Area_Light (
		HC_KEY key, int * ucount, HC_DPOINT * upoints, char * listptr);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_DShow_Bounding_Cuboid (
		HC_DPOINT * min, HC_DPOINT * max);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_DShow_Bounding_Cuboid_By_Key (
		HC_KEY key, HC_DPOINT * min, HC_DPOINT * max);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_DShow_Bounding_Sphere (
		HC_DPOINT * center, double * radius);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_DShow_Bounding_Sphere_By_Key (
		HC_KEY key, HC_DPOINT * center, double * radius);
HC_EXTERNAL void HC_CDECL HC_DShow_Camera (
		HC_DPOINT * position, HC_DPOINT * target, HC_DVECTOR * up, double * width, double * height, char * projection);
HC_EXTERNAL void HC_CDECL HC_DShow_Camera_By_Volume (
		char * proj, double * xmin, double * xmax, double * ymin, double * ymax);
HC_EXTERNAL void HC_CDECL HC_DShow_Camera_Field (
		double * width, double * height);
HC_EXTERNAL void HC_CDECL HC_DShow_Camera_Position (
		double * x, double * y, double * z);
HC_EXTERNAL void HC_CDECL HC_DShow_Camera_Target (
		double * x, double * y, double * z);
HC_EXTERNAL void HC_CDECL HC_DShow_Camera_Up_Vector (
		double * x, double * y, double * z);
HC_EXTERNAL void HC_CDECL HC_DShow_Circle (
		HC_KEY key, HC_DPOINT * point1, HC_DPOINT * point2, HC_DPOINT * point3);
HC_EXTERNAL void HC_CDECL HC_DShow_Circle_By_Radius (
		HC_KEY key, HC_DPOINT * center, double * radius, HC_DVECTOR * normal);
HC_EXTERNAL void HC_CDECL HC_DShow_Circular_Arc (
		HC_KEY key, HC_DPOINT * point1, HC_DPOINT * point2, HC_DPOINT * point3);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_DShow_Circular_Center (
		HC_KEY key, double * x, double * y, double * z);
HC_EXTERNAL void HC_CDECL HC_DShow_Circular_Chord (
		HC_KEY key, HC_DPOINT * point1, HC_DPOINT * point2, HC_DPOINT * point3);
HC_EXTERNAL void HC_CDECL HC_DShow_Circular_Wedge (
		HC_KEY key, HC_DPOINT * point1, HC_DPOINT * point2, HC_DPOINT * point3);
HC_EXTERNAL void HC_CDECL HC_DShow_Clip_Region (
		int * loopCount, int * loopSizeArray, HC_DPOINT * points, char * options);
HC_EXTERNAL void HC_CDECL HC_DShow_Clip_Region_Size (
		int * loopCount, int * totalPointCount, char * options);
HC_EXTERNAL void HC_CDECL HC_DShow_Compressed_Image (
		HC_KEY key, double * x, double * y, double * z, char * format, int * width, int * height, int * size, void * data);
HC_EXTERNAL void HC_CDECL HC_DShow_Cutting_Plane (
		HC_KEY key, double * a, double * b, double * c, double * d);
HC_EXTERNAL void HC_CDECL HC_DShow_Cutting_Section (
		HC_KEY key, int * count, HC_DPLANE * planes);
HC_EXTERNAL void HC_CDECL HC_DShow_Cylinder (
		HC_KEY key, HC_DPOINT * p1, HC_DPOINT * p2, double * radius, char * cap);
HC_EXTERNAL void HC_CDECL HC_DShow_Ellipse (
		HC_KEY key, HC_DPOINT * center, HC_DPOINT * major, HC_DPOINT * minor);
HC_EXTERNAL void HC_CDECL HC_DShow_Elliptical_Arc (
		HC_KEY key, HC_DPOINT * center, HC_DPOINT * major, HC_DPOINT * minor, float * start, float * end);
HC_EXTERNAL void HC_CDECL HC_DShow_Grid (
		HC_KEY key, char * type, HC_DPOINT * origin, HC_DPOINT * ref1, HC_DPOINT * ref2, int * count1, int * count2);
HC_EXTERNAL void HC_CDECL HC_DShow_Image (
		HC_KEY key, double * x, double * y, double * z, char * format, int * width, int * height, void * data);
HC_EXTERNAL void HC_CDECL HC_DShow_Image_Size (
		HC_KEY key, double * x, double * y, double * z, char * format, int * width, int * height);
HC_EXTERNAL void HC_CDECL HC_DShow_Infinite_Line (
		HC_KEY key, double * xa, double * ya, double * za, double * xb, double * yb, double * zb);
HC_EXTERNAL void HC_CDECL HC_DShow_Infinite_Ray (
		HC_KEY key, double * xa, double * ya, double * za, double * xb, double * yb, double * zb);
HC_EXTERNAL void HC_CDECL HC_DShow_Line (
		HC_KEY key, double * xa, double * ya, double * za, double * xb, double * yb, double * zb);
HC_EXTERNAL void HC_CDECL HC_DShow_Local_Light (
		HC_KEY key, double * x, double * y, double * z);
HC_EXTERNAL void HC_CDECL HC_DShow_Marker (
		HC_KEY key, double * x, double * y, double * z);
HC_EXTERNAL void HC_CDECL HC_DShow_Mesh (
		HC_KEY key, int * rows, int * columns, HC_DPOINT * points);
HC_EXTERNAL void HC_CDECL HC_DShow_Modelling_Matrix (
		double * matrix);
HC_EXTERNAL void HC_CDECL HC_DShow_NURBS_Curve (
		HC_KEY key, int * degree, int * cp_count, HC_DPOINT * points, float * weights, float * knots, float * start_u, float * end_u);
HC_EXTERNAL void HC_CDECL HC_DShow_NURBS_Surface (
		HC_KEY key, int * u_degree, int * v_degree, int * u_count, int * v_count, HC_DPOINT * points, float * weights, float * u_knots, float * v_knots);
HC_EXTERNAL void HC_CDECL HC_DShow_PolyCylinder (
		HC_KEY key, int * p_count, HC_DPOINT * pts, int * r_count, double * radii, char * capping);
HC_EXTERNAL void HC_CDECL HC_DShow_Polygon (
		HC_KEY key, int * count, HC_DPOINT * points);
HC_EXTERNAL void HC_CDECL HC_DShow_Polyline (
		HC_KEY key, int * count, HC_DPOINT * points);
HC_EXTERNAL int HC_CDECL HC_DShow_Selection_Position (
		float * window_x, float * window_y, float * window_z, double * camera_x, double * camera_y, double * camera_z);
HC_EXTERNAL void HC_CDECL HC_DShow_Selection_Source_Elem (
		int * vertex, int * edge, int * face, HC_DPOINT * hit_location);
#define HC_DShow_Selection_Source_Element(vertex, edge, face, hit_location) \
		HC_DShow_Selection_Source_Elem(vertex, edge, face, hit_location)
HC_EXTERNAL void HC_CDECL HC_DShow_Shell (
		HC_KEY key, int * point_countp, HC_DPOINT * points, int * face_list_lengthp, int * face_list);
HC_EXTERNAL void HC_CDECL HC_DShow_Shell_By_Tristrips (
		HC_KEY key, int * point_countp, HC_DPOINT * points, int * tristrip_list_lengthp, int * tristrip_list, int * face_list_lengthp, int * face_list);
HC_EXTERNAL void HC_CDECL HC_DShow_Sphere (
		HC_KEY key, HC_DPOINT * center, double * radius, HC_DVECTOR * axis, HC_DVECTOR * ortho);
HC_EXTERNAL void HC_CDECL HC_DShow_Spot_Light (
		HC_KEY key, HC_DPOINT * position, HC_DPOINT * target, char * list);
HC_EXTERNAL void HC_CDECL HC_DShow_Text (
		HC_KEY key, double * x, double * y, double * z, char * text);
HC_EXTERNAL void HC_CDECL HC_DShow_Text_With_Encoding (
		HC_KEY key, double * x, double * y, double * z, char * encoding, void * text);
HC_EXTERNAL void HC_CDECL HC_DTranslate_Object (
		double x, double y, double z);
HC_EXTERNAL void HC_CDECL HC_DZoom_Camera (
		double zoom);
HC_EXTERNAL HC_KEY HC_CDECL HC_Decimate_Shell (
		int quality_percent, const char * options);
HC_EXTERNAL void HC_CDECL HC_Define_Alias (
		const char * name, const char * expansion);
HC_EXTERNAL void HC_CDECL HC_Define_Callback_Name (
		const char * name, void (HC_CDECL * callback)(HC_ANY_ARGS));
HC_EXTERNAL void HC_CDECL HC_Define_Color_Name (
		const char * name, const char * er_name, const char * ish_name, const char * definition);
HC_EXTERNAL void HC_CDECL HC_Define_Driver_Config (
		const char * driver, const char * id, const char * version, const char * config);
HC_EXTERNAL void HC_CDECL HC_Define_Error_Handler (
		void (HC_CDECL * handler)(HC_ANY_ARGS));
HC_EXTERNAL void HC_CDECL HC_Define_Exit_Handler (
		void (HC_CDECL * handler)(HC_ANY_ARGS));
HC_EXTERNAL void HC_CDECL HC_Define_Font (
		const char * name, const char * options, int data_length, const char * data);
HC_EXTERNAL void HC_CDECL HC_Define_Font_By_Reference (
		const char * name, const char * options, int data_length, const char * data);
#define HC_Define_Font_By_Ref(name, options, data_length, data) \
		HC_Define_Font_By_Reference(name, options, data_length, data)
HC_EXTERNAL void HC_CDECL HC_Define_Geometry_Highlight (
		int count, const HC_KEY * keys, const char * style, const char * options, int elements_count, const int * offset1, const int * offset2, const int * offset3);
HC_EXTERNAL void HC_CDECL HC_Define_Glyph (
		const char * name, int data_size, const char * data);
HC_EXTERNAL void HC_CDECL HC_Define_Highlight (
		int count, const HC_KEY * keys, const char * style, const char * options);
HC_EXTERNAL void HC_CDECL HC_Define_Line_Style (
		const char * name, const char * definition);
HC_EXTERNAL void HC_CDECL HC_Define_Local_Texture (
		const char * texture_name, const char * definition);
HC_EXTERNAL void HC_CDECL HC_Define_Named_Style (
		const char * style_name, const char * source_segment);
HC_EXTERNAL void HC_CDECL HC_Define_Named_Style_By_Key (
		const char * style_name, HC_KEY style_segment_key);
HC_EXTERNAL void HC_CDECL HC_Define_Shader (
		const char * name, const char * options, const char * shader_source);
HC_EXTERNAL void HC_CDECL HC_Define_System_Options (
		const char * list);
HC_EXTERNAL void HC_CDECL HC_Define_Texture (
		const char * texture_name, const char * definition);
HC_EXTERNAL void HC_CDECL HC_Delete_By_Key (
		HC_KEY key);
HC_EXTERNAL void HC_CDECL HC_Delete_Segment (
		const char * segment);
HC_EXTERNAL void HC_CDECL HC_Delete_Specific_Shell_Faces (
		HC_KEY key, int count, const int * indices);
HC_EXTERNAL void HC_CDECL HC_Delete_Specific_Shell_Points (
		HC_KEY key, int count, const int * indices);
HC_EXTERNAL void HC_CDECL HC_Delete_Trim (
		int index);
HC_EXTERNAL void HC_CDECL HC_Dolly_Camera (
		double right, double up, double in);
HC_EXTERNAL HC_POINTER_SIZED_INT HC_CDECL HC_Dump_Memory_Profile (
		const char * which);
HC_EXTERNAL void HC_CDECL HC_Edit_Circle (
		HC_KEY key, const HC_POINT * point1, const HC_POINT * point2, const HC_POINT * point3);
HC_EXTERNAL void HC_CDECL HC_Edit_Circle_By_Radius (
		HC_KEY key, const HC_POINT * center, double radius, const HC_VECTOR * vector);
HC_EXTERNAL void HC_CDECL HC_Edit_Circular_Arc (
		HC_KEY key, const HC_POINT * point1, const HC_POINT * point2, const HC_POINT * point3);
HC_EXTERNAL void HC_CDECL HC_Edit_Circular_Chord (
		HC_KEY key, const HC_POINT * point1, const HC_POINT * point2, const HC_POINT * point3);
HC_EXTERNAL void HC_CDECL HC_Edit_Circular_Wedge (
		HC_KEY key, const HC_POINT * point1, const HC_POINT * point2, const HC_POINT * point3);
HC_EXTERNAL void HC_CDECL HC_Edit_Cutting_Plane (
		HC_KEY key, double a, double b, double c, double d);
HC_EXTERNAL void HC_CDECL HC_Edit_Cutting_Section (
		HC_KEY key, int ioffset, int ndelete, int insert, const HC_PLANE * iplanes);
HC_EXTERNAL void HC_CDECL HC_Edit_Cylinder (
		HC_KEY key, const HC_POINT * p1, const HC_POINT * p2, double radius, const char * cap);
HC_EXTERNAL void HC_CDECL HC_Edit_Ellipse (
		HC_KEY key, const HC_POINT * center, const HC_POINT * major, const HC_POINT * minor);
HC_EXTERNAL void HC_CDECL HC_Edit_Elliptical_Arc (
		HC_KEY key, const HC_POINT * center, const HC_POINT * major, const HC_POINT * minor, double start, double end);
HC_EXTERNAL void HC_CDECL HC_Edit_Grid (
		HC_KEY key, const char * type, const HC_POINT * origin, const HC_POINT * ref1, const HC_POINT * ref2, int count1, int count2);
HC_EXTERNAL void HC_CDECL HC_Edit_Image (
		HC_KEY key, int xoffset, int yoffset, int xcount, int ycount, const void * data);
HC_EXTERNAL void HC_CDECL HC_Edit_Image_Options (
		HC_KEY image_key, const char * options);
HC_EXTERNAL void HC_CDECL HC_Edit_Infinite_Line (
		HC_KEY key, double xa, double ya, double za, double xb, double yb, double zb);
HC_EXTERNAL void HC_CDECL HC_Edit_Infinite_Ray (
		HC_KEY key, double xa, double ya, double za, double xb, double yb, double zb);
HC_EXTERNAL void HC_CDECL HC_Edit_Line (
		HC_KEY key, double xa, double ya, double za, double xb, double yb, double zb);
HC_EXTERNAL void HC_CDECL HC_Edit_Marker (
		HC_KEY key, double x, double y, double z);
HC_EXTERNAL void HC_CDECL HC_Edit_Mesh (
		HC_KEY key, int row_offset, int column_offset, int row_count, int column_count, const HC_POINT * points);
HC_EXTERNAL void HC_CDECL HC_Edit_NURBS_Curve (
		HC_KEY key, int cp_offset, int knot_offset, int cp_count, int knot_count, const HC_POINT * control_points, const float * weights, const float * knots);
HC_EXTERNAL void HC_CDECL HC_Edit_NURBS_Surface (
		HC_KEY key, int cp_offset, int weights_offset, int u_knot_offset, int v_knot_offset, int cp_replace_count, int weights_replace_count, int u_knot_replace_count, int v_knot_replace_count, const HC_POINT * points, const float * weights, const float * u_knots, const float * v_knots);
HC_EXTERNAL void HC_CDECL HC_Edit_NURBS_Surface_Knots (
		HC_KEY key, int u_knot_offset, int v_knot_offset, int u_knot_replace_count, int v_knot_replace_count, const float * u_knots, const float * v_knots);
HC_EXTERNAL void HC_CDECL HC_Edit_NURBS_Surface_Points (
		HC_KEY key, int cp_offset, int cp_replace_count, const HC_POINT * points);
HC_EXTERNAL void HC_CDECL HC_Edit_NURBS_Surface_Weights (
		HC_KEY key, int weights_offset, int weights_replace_count, const float * weights);
HC_EXTERNAL void HC_CDECL HC_Edit_PolyCylinder (
		HC_KEY key, int p_count, const HC_POINT * pts, int r_count, const float * radii, const char * capping);
HC_EXTERNAL void HC_CDECL HC_Edit_PolyCylinder_Points (
		HC_KEY key, int offset, int delete_count, int insert_count, const HC_POINT * points);
HC_EXTERNAL void HC_CDECL HC_Edit_PolyCylinder_Radii (
		HC_KEY key, int offset, int delete_count, int insert_count, const float * radii);
HC_EXTERNAL void HC_CDECL HC_Edit_PolyCylinder_With_Basis (
		HC_KEY key, int p_count, const HC_POINT * pts, int r_count, const float * radii, const char * capping, const HC_VECTOR * basis);
HC_EXTERNAL void HC_CDECL HC_Edit_Polygon (
		HC_KEY key, int offset, int ndelete, int insert, const HC_POINT * points);
HC_EXTERNAL void HC_CDECL HC_Edit_Polyline (
		HC_KEY key, int offset, int ndelete, int insert, const HC_POINT * points);
HC_EXTERNAL void HC_CDECL HC_Edit_Shell_Faces (
		HC_KEY key, int offset, int ndelete, int ilist_length, const int * insert_list);
HC_EXTERNAL void HC_CDECL HC_Edit_Shell_Points (
		HC_KEY key, int offset, int ndelete, int insert, const HC_POINT * points);
HC_EXTERNAL void HC_CDECL HC_Edit_Sphere (
		HC_KEY key, const HC_POINT * center, double radius, const HC_VECTOR * axis, const HC_VECTOR * ortho);
HC_EXTERNAL void HC_CDECL HC_Edit_Text (
		HC_KEY key, int row, int column, int erase, int ndelete, const char * new_text);
HC_EXTERNAL void HC_CDECL HC_End_Alias_Search (
		void);
HC_EXTERNAL void HC_CDECL HC_End_Buffered_Selection (
		void);
HC_EXTERNAL void HC_CDECL HC_End_Callback_Name_Search (
		void);
HC_EXTERNAL void HC_CDECL HC_End_Color_Name_Search (
		void);
HC_EXTERNAL void HC_CDECL HC_End_Contents_Search (
		void);
HC_EXTERNAL void HC_CDECL HC_End_Decimate_Shell (
		void);
HC_EXTERNAL void HC_CDECL HC_End_Error_Handler_Search (
		void);
HC_EXTERNAL void HC_CDECL HC_End_Exit_Handler_Search (
		void);
HC_EXTERNAL void HC_CDECL HC_End_Font_Search (
		void);
HC_EXTERNAL void HC_CDECL HC_End_Glyph_Search (
		void);
HC_EXTERNAL void HC_CDECL HC_End_Line_Style_Search (
		void);
HC_EXTERNAL void HC_CDECL HC_End_Named_Style_Search (
		void);
HC_EXTERNAL void HC_CDECL HC_End_Open_Item_Search (
		void);
HC_EXTERNAL void HC_CDECL HC_End_Open_Segment_Search (
		void);
HC_EXTERNAL void HC_CDECL HC_End_Segment_Search (
		void);
HC_EXTERNAL void HC_CDECL HC_End_Shader_Search (
		void);
HC_EXTERNAL void HC_CDECL HC_End_Shell_Selection (
		void);
HC_EXTERNAL void HC_CDECL HC_End_Texture_Search (
		void);
HC_EXTERNAL void HC_CDECL HC_Exit_Program (
		void);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_Filter_Circumcuboid (
		const char * segment, const char * options, HC_POINT * min, HC_POINT * max);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_Filter_Circumcuboid_By_Key (
		HC_KEY segKey, const char * options, HC_POINT * min, HC_POINT * max);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_Filter_Circumsphere (
		const char * segment, const char * options, HC_POINT * center, float * radius);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_Filter_Circumsphere_By_Key (
		HC_KEY segKey, const char * options, HC_POINT * center, float * radius);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_Find_Alias (
		char * alias);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_Find_Callback_Name (
		char * name);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_Find_Color_Name (
		char * name, char * er_name, char * ish_name);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_Find_Contents (
		char * type, HC_KEY * key);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_Find_Error_Handler (
		void (HC_CDECL ** handler)(HC_ANY_ARGS));
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_Find_Exit_Handler (
		void (HC_CDECL ** handler)(HC_ANY_ARGS));
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_Find_Font (
		char * name);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_Find_Glyph (
		char * name);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_Find_Line_Style (
		char * name);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_Find_Named_Style (
		char * style_name);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_Find_Next_Sample (
		int * key_count, HC_KEY * keys, int * offset1, int * offset2, int * offset3);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_Find_Open_Item (
		HC_KEY * key, char * type, int * offset1, int * offset2);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_Find_Open_Segment (
		char * segment);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_Find_Related_Selection (
		void);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_Find_Segment (
		char * segment);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_Find_Shader (
		char * sname);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_Find_Texture (
		char * name);
HC_EXTERNAL void HC_CDECL HC_Flush_Attributes (
		const char * segment);
HC_EXTERNAL void HC_CDECL HC_Flush_By_Key (
		HC_KEY key);
HC_EXTERNAL void HC_CDECL HC_Flush_Contents (
		const char * segment, const char * filter);
HC_EXTERNAL void HC_CDECL HC_Flush_Geometry (
		const char * segment);
HC_EXTERNAL int HC_CDECL HC_Gather_Capping_Geometry (
		int key_count, const HC_KEY * path_keys, HC_KEY cut_key, const char * options);
HC_EXTERNAL HC_KEY HC_CDECL HC_Generate_Edges (
		HC_KEY geom, const char * options);
HC_EXTERNAL void HC_CDECL HC_Generate_Error (
		int level, int category, int specific, const char * buf1, const char * buf2, const char * buf3);
HC_EXTERNAL HC_KEY HC_CDECL HC_Generate_Poly_From_Geometry (
		HC_KEY geom, const char * options);
#define HC_Generate_Polyline_From_Geometry(geom, options) \
		HC_Generate_Poly_From_Geometry(geom, options)
HC_EXTERNAL HC_KEY HC_CDECL HC_Generate_Shell_From_Geometry (
		HC_KEY geom, const char * options);
HC_EXTERNAL HC_KEY HC_CDECL HC_Include_Segment (
		const char * include);
HC_EXTERNAL HC_KEY HC_CDECL HC_Include_Segment_By_Key (
		HC_KEY seg_key);
HC_EXTERNAL HC_KEY HC_CDECL HC_Include_Segment_Key_By_Key (
		HC_KEY key, HC_KEY includer_key);
HC_EXTERNAL HC_KEY HC_CDECL HC_Insert_Area_Light (
		int count, const HC_POINT * points, const char * listptr);
HC_EXTERNAL HC_KEY HC_CDECL HC_Insert_Circle (
		const HC_POINT * point1, const HC_POINT * point2, const HC_POINT * point3);
HC_EXTERNAL HC_KEY HC_CDECL HC_Insert_Circle_By_Radius (
		const HC_POINT * center, double radius, const HC_VECTOR * normal);
HC_EXTERNAL HC_KEY HC_CDECL HC_Insert_Circular_Arc (
		const HC_POINT * point1, const HC_POINT * point2, const HC_POINT * point3);
HC_EXTERNAL HC_KEY HC_CDECL HC_Insert_Circular_Chord (
		const HC_POINT * point1, const HC_POINT * point2, const HC_POINT * point3);
HC_EXTERNAL HC_KEY HC_CDECL HC_Insert_Circular_Wedge (
		const HC_POINT * point1, const HC_POINT * point2, const HC_POINT * point3);
HC_EXTERNAL HC_KEY HC_CDECL HC_Insert_Compressed_Image (
		double x, double y, double z, const char * format, int width, int height, int size, const void * data);
HC_EXTERNAL HC_KEY HC_CDECL HC_Insert_Cutting_Plane (
		double a, double b, double c, double d);
HC_EXTERNAL HC_KEY HC_CDECL HC_Insert_Cutting_Section (
		int count, const HC_PLANE * planes);
HC_EXTERNAL HC_KEY HC_CDECL HC_Insert_Cylinder (
		const HC_POINT * center_point_1, const HC_POINT * center_point_2, double radius, const char * cap);
HC_EXTERNAL HC_KEY HC_CDECL HC_Insert_Distant_Light (
		double di, double dj, double dk);
HC_EXTERNAL HC_KEY HC_CDECL HC_Insert_Ellipse (
		const HC_POINT * center, const HC_POINT * major, const HC_POINT * minor);
HC_EXTERNAL HC_KEY HC_CDECL HC_Insert_Elliptical_Arc (
		const HC_POINT * center, const HC_POINT * major, const HC_POINT * minor, double start, double end);
HC_EXTERNAL HC_KEY HC_CDECL HC_Insert_Grid (
		const char * type, const HC_POINT * origin, const HC_POINT * ref1, const HC_POINT * ref2, int count1, int count2);
HC_EXTERNAL HC_KEY HC_CDECL HC_Insert_Image (
		double x, double y, double z, const char * format, int width, int height, const void * data);
HC_EXTERNAL HC_KEY HC_CDECL HC_Insert_Image_By_Ref (
		double x, double y, double z, const char * format, int width, int height, const void * data);
HC_EXTERNAL HC_KEY HC_CDECL HC_Insert_Infinite_Line (
		double xa, double ya, double za, double xb, double yb, double zb);
HC_EXTERNAL HC_KEY HC_CDECL HC_Insert_Infinite_Ray (
		double xa, double ya, double za, double xb, double yb, double zb);
HC_EXTERNAL HC_KEY HC_CDECL HC_Insert_Ink (
		double x, double y, double z);
HC_EXTERNAL HC_KEY HC_CDECL HC_Insert_Line (
		double xa, double ya, double za, double xb, double yb, double zb);
HC_EXTERNAL HC_KEY HC_CDECL HC_Insert_Local_Light (
		double x, double y, double z);
HC_EXTERNAL HC_KEY HC_CDECL HC_Insert_Marker (
		double x, double y, double z);
HC_EXTERNAL HC_KEY HC_CDECL HC_Insert_Mesh (
		int rows, int columns, const HC_POINT * points);
HC_EXTERNAL HC_KEY HC_CDECL HC_Insert_Mesh_By_Ref (
		int rows, int columns, const HC_POINT * points);
HC_EXTERNAL HC_KEY HC_CDECL HC_Insert_NURBS_Curve (
		int degree, int cpcount, const HC_POINT * control_points, const float * weights, const float * knots, double start_u, double end_u);
HC_EXTERNAL HC_KEY HC_CDECL HC_Insert_NURBS_Surface (
		int u_degree, int v_degree, int u_count, int v_count, const HC_POINT * points, const float * weights, const float * u_knots, const float * v_knots);
HC_EXTERNAL HC_KEY HC_CDECL HC_Insert_PolyCylinder (
		int p_count, const HC_POINT * pts, int r_count, const float * radii, const char * capping);
HC_EXTERNAL HC_KEY HC_CDECL HC_Insert_Polygon (
		int count, const HC_POINT * points);
HC_EXTERNAL HC_KEY HC_CDECL HC_Insert_Polyline (
		int count, const HC_POINT * points);
HC_EXTERNAL HC_KEY HC_CDECL HC_Insert_Shell (
		int pcount, const HC_POINT * points, int flist_length, const int * face_list);
HC_EXTERNAL HC_KEY HC_CDECL HC_Insert_Shell_By_Ref (
		int pcount, const HC_POINT * points, int flist_length, const int * face_list);
HC_EXTERNAL HC_KEY HC_CDECL HC_Insert_Shell_By_Tristrips (
		int pcount, const HC_POINT * points, int tristrips_length, const int * tristrips, int face_indices_length, const int * face_indices);
HC_EXTERNAL HC_KEY HC_CDECL HC_Insert_Shell_From_Region (
		HC_KEY key1, int region, const char * options);
HC_EXTERNAL HC_KEY HC_CDECL HC_Insert_Sphere (
		const HC_POINT * center, double radius, const HC_VECTOR * axis, const HC_VECTOR * ortho);
HC_EXTERNAL HC_KEY HC_CDECL HC_Insert_Spot_Light (
		const HC_POINT * position, const HC_POINT * target, const char * list);
HC_EXTERNAL HC_KEY HC_CDECL HC_Insert_String_Cursor (
		HC_KEY text_key, int row, int column);
HC_EXTERNAL HC_KEY HC_CDECL HC_Insert_Text (
		double x, double y, double z, const char * text);
HC_EXTERNAL HC_KEY HC_CDECL HC_Insert_Text_With_Encoding (
		double x, double y, double z, const char * encoding, const void * text);
HC_EXTERNAL HC_KEY HC_CDECL HC_Insert_Unicode_Text (
		double x, double y, double z, const unsigned short * text);
HC_EXTERNAL void HC_CDECL HC_MSet_Character_Attributes (
		HC_KEY key, int offset, int count, const char * options);
HC_EXTERNAL void HC_CDECL HC_MSet_Face_Colors_By_FIndex (
		HC_KEY key, const char * type, int offset, int count, const float * findices);
HC_EXTERNAL void HC_CDECL HC_MSet_Face_Colors_By_Value (
		HC_KEY key, const char * type, int offset, const char * color_space, int count, const HC_RGB * values);
HC_EXTERNAL void HC_CDECL HC_MSet_Face_Normals (
		HC_KEY key, int offset, int count, const HC_VECTOR * normals);
HC_EXTERNAL void HC_CDECL HC_MSet_Face_Regions (
		HC_KEY key, int first_face, int face_count, const int * regions);
HC_EXTERNAL void HC_CDECL HC_MSet_Face_Visibilities (
		HC_KEY key, int offset, int count, const char * settings);
HC_EXTERNAL void HC_CDECL HC_MSet_Region_Faces (
		HC_KEY key, int region, int first_face, int face_count);
HC_EXTERNAL void HC_CDECL HC_MSet_Spec_Face_Col_By_FIndex (
		HC_KEY key, int count, const int * indices, const float * findices);
#define HC_MSet_Specific_Face_Colors_By_FIndex(key, count, indices, findices) \
		HC_MSet_Spec_Face_Col_By_FIndex(key, count, indices, findices)
HC_EXTERNAL void HC_CDECL HC_MSet_Spec_Face_Col_By_Value (
		HC_KEY key, int count, const int * indices, const char * color_space, const HC_RGB * values);
#define HC_MSet_Specific_Face_Colors_By_Value(key, count, indices, color_space, values) \
		HC_MSet_Spec_Face_Col_By_Value(key, count, indices, color_space, values)
HC_EXTERNAL void HC_CDECL HC_MSet_Spec_Vert_Col_By_FIndex (
		HC_KEY key, const char * types, int count, const int * offsets, const float * indices);
#define HC_MSet_Specific_Vertex_Colors_By_FIndex(key, types, count, offsets, indices) \
		HC_MSet_Spec_Vert_Col_By_FIndex(key, types, count, offsets, indices)
HC_EXTERNAL void HC_CDECL HC_MSet_Spec_Vert_Col_By_Value (
		HC_KEY key, const char * types, int count, const int * offsets, const char * color_space, const HC_RGB * values);
#define HC_MSet_Specific_Vertex_Colors_By_Value(key, types, count, offsets, color_space, values) \
		HC_MSet_Spec_Vert_Col_By_Value(key, types, count, offsets, color_space, values)
HC_EXTERNAL void HC_CDECL HC_MSet_Spec_Vertex_Parameters (
		HC_KEY key, int count, const int * indices, int number_parameters, const float * user_parameters);
#define HC_MSet_Specific_Vertex_Parameters(key, count, indices, number_parameters, user_parameters) \
		HC_MSet_Spec_Vertex_Parameters(key, count, indices, number_parameters, user_parameters)
HC_EXTERNAL void HC_CDECL HC_MSet_Specific_Edge_Vis (
		HC_KEY key, int count, const int * indices1, const int * indices2, const char * settings);
#define HC_MSet_Specific_Edge_Visibilities(key, count, indices1, indices2, settings) \
		HC_MSet_Specific_Edge_Vis(key, count, indices1, indices2, settings)
HC_EXTERNAL void HC_CDECL HC_MSet_Specific_Face_Normals (
		HC_KEY key, int count, const int * indices, const HC_VECTOR * normals);
HC_EXTERNAL void HC_CDECL HC_MSet_Specific_Face_Vis (
		HC_KEY key, int count, const int * indices, const char * settings);
#define HC_MSet_Specific_Face_Visibilities(key, count, indices, settings) \
		HC_MSet_Specific_Face_Vis(key, count, indices, settings)
#define HC_MSet_Face_Visibilities_By_Index(key, count, indices, settings) \
		HC_MSet_Specific_Face_Vis(key, count, indices, settings)
HC_EXTERNAL void HC_CDECL HC_MSet_Specific_Vertex_Normals (
		HC_KEY key, int count, const int * indices, const HC_VECTOR * normals);
HC_EXTERNAL void HC_CDECL HC_MSet_Specific_Vertex_Vis (
		HC_KEY key, int count, const int * indices, const char * settings);
#define HC_MSet_Specific_Vertex_Visibilities(key, count, indices, settings) \
		HC_MSet_Specific_Vertex_Vis(key, count, indices, settings)
HC_EXTERNAL void HC_CDECL HC_MSet_Vertex_Colors_By_FIndex (
		HC_KEY key, const char * types, int offset, int count, const float * findices);
HC_EXTERNAL void HC_CDECL HC_MSet_Vertex_Colors_By_Value (
		HC_KEY key, const char * types, int offset, const char * color_space, int count, const HC_RGB * values);
HC_EXTERNAL void HC_CDECL HC_MSet_Vertex_Colors_By_Value4 (
		HC_KEY key, const char * geometry, int offset, const char * color_space, int count, const HC_RGBA * values);
HC_EXTERNAL void HC_CDECL HC_MSet_Vertex_Normals (
		HC_KEY key, int offset, int count, const HC_VECTOR * normals);
HC_EXTERNAL void HC_CDECL HC_MSet_Vertex_Parameters (
		HC_KEY key, int offset, int pcount, int number, const float * parameters);
HC_EXTERNAL void HC_CDECL HC_MSet_Vertex_Visibilities (
		HC_KEY key, int offset, int count, const char * settings);
HC_EXTERNAL void HC_CDECL HC_MShow_Character_Attributes (
		HC_KEY key, int offset, int count, char * options);
HC_EXTERNAL void HC_CDECL HC_MShow_Face_Colors_By_FIndex (
		HC_KEY key, int offset, int count, float * findices);
HC_EXTERNAL void HC_CDECL HC_MShow_Face_Colors_By_Value (
		HC_KEY key, int offset, int count, HC_RGB * rgb);
HC_EXTERNAL int HC_CDECL HC_MShow_Face_Colors_With_Ex (
		HC_KEY key, const char * geometry, int offset, int count, char * result_type, float * index_colors, HC_RGB * rgb_colors);
#define HC_MShow_Face_Colors_With_Existence(key, geometry, offset, count, result_type, index_colors, rgb_colors) \
		HC_MShow_Face_Colors_With_Ex(key, geometry, offset, count, result_type, index_colors, rgb_colors)
HC_EXTERNAL void HC_CDECL HC_MShow_Face_Normals (
		HC_KEY key, int offset, int count, HC_VECTOR * normals);
HC_EXTERNAL int HC_CDECL HC_MShow_Face_Normals_W_Ex (
		HC_KEY key, int offset, int count, char * existence, HC_VECTOR * normals);
#define HC_MShow_Face_Normals_With_Existence(key, offset, count, existence, normals) \
		HC_MShow_Face_Normals_W_Ex(key, offset, count, existence, normals)
HC_EXTERNAL void HC_CDECL HC_MShow_Face_Regions (
		HC_KEY key, int * face_count, int * regions);
HC_EXTERNAL void HC_CDECL HC_MShow_Face_Regions_Count (
		HC_KEY key, int * face_count);
HC_EXTERNAL int HC_CDECL HC_MShow_Face_Vis_W_Ex (
		HC_KEY key, int offset, int count, char * existence, char * visibilities);
#define HC_MShow_Face_Visibilities_With_Existence(key, offset, count, existence, visibilities) \
		HC_MShow_Face_Vis_W_Ex(key, offset, count, existence, visibilities)
HC_EXTERNAL void HC_CDECL HC_MShow_Face_Visibilities (
		HC_KEY key, int offset, int count, char * settings);
HC_EXTERNAL void HC_CDECL HC_MShow_Net_Face_Normals (
		HC_KEY key, int offset, int count, HC_VECTOR * normals);
HC_EXTERNAL void HC_CDECL HC_MShow_Net_Vertex_Normals (
		HC_KEY key, int offset, int count, HC_VECTOR * normals);
HC_EXTERNAL void HC_CDECL HC_MShow_Partial_Face_Regions (
		HC_KEY key, int first_face, int face_count, int * regions);
HC_EXTERNAL void HC_CDECL HC_MShow_Region_Faces (
		HC_KEY key, int region, int * face_count, int * faces);
HC_EXTERNAL void HC_CDECL HC_MShow_Region_Faces_Count (
		HC_KEY key, int region, int * face_count);
HC_EXTERNAL void HC_CDECL HC_MShow_Spec_Face_Col_By_FInd (
		HC_KEY key, int count, const int * indices, float * findices);
#define HC_MShow_Specific_Face_Colors_By_FIndex(key, count, indices, findices) \
		HC_MShow_Spec_Face_Col_By_FInd(key, count, indices, findices)
HC_EXTERNAL void HC_CDECL HC_MShow_Spec_Face_Col_By_Value (
		HC_KEY key, int count, const int * indices, HC_RGB * rgb);
#define HC_MShow_Specific_Face_Colors_By_Value(key, count, indices, rgb) \
		HC_MShow_Spec_Face_Col_By_Value(key, count, indices, rgb)
HC_EXTERNAL int HC_CDECL HC_MShow_Spec_Face_Colors_W_Ex (
		HC_KEY key, const char * geometry, int count, const int * faces, char * result_type, float * index_colors, HC_RGB * rgb_colors);
#define HC_MShow_Specific_Face_Colors_With_Existence(key, geometry, count, faces, result_type, index_colors, rgb_colors) \
		HC_MShow_Spec_Face_Colors_W_Ex(key, geometry, count, faces, result_type, index_colors, rgb_colors)
HC_EXTERNAL int HC_CDECL HC_MShow_Spec_Face_Normals_W_Ex (
		HC_KEY key, int count, const int * indices, char * existence, HC_VECTOR * normals);
#define HC_MShow_Specific_Face_Normals_With_Existence(key, count, indices, existence, normals) \
		HC_MShow_Spec_Face_Normals_W_Ex(key, count, indices, existence, normals)
HC_EXTERNAL int HC_CDECL HC_MShow_Spec_Face_Vis_W_Ex (
		HC_KEY key, int count, const int * indices, char * existence, char * visibilities);
#define HC_MShow_Specific_Face_Visibilities_With_Existence(key, count, indices, existence, visibilities) \
		HC_MShow_Spec_Face_Vis_W_Ex(key, count, indices, existence, visibilities)
HC_EXTERNAL void HC_CDECL HC_MShow_Spec_Vert_Col_By_FInd (
		HC_KEY key, const char * type, int pcount, const int * offsets, float * indices);
#define HC_MShow_Specific_Vertex_Colors_By_FIndex(key, type, pcount, offsets, indices) \
		HC_MShow_Spec_Vert_Col_By_FInd(key, type, pcount, offsets, indices)
HC_EXTERNAL void HC_CDECL HC_MShow_Spec_Vert_Col_By_Value (
		HC_KEY key, const char * type, int count, const int * offsets, HC_RGB * rgb);
#define HC_MShow_Specific_Vertex_Colors_By_Value(key, type, count, offsets, rgb) \
		HC_MShow_Spec_Vert_Col_By_Value(key, type, count, offsets, rgb)
HC_EXTERNAL int HC_CDECL HC_MShow_Spec_Vert_Colors_W_Ex (
		HC_KEY key, const char * geometry, int count, const int * vertices, char * result_type, float * index_colors, HC_RGB * rgb_colors, HC_RGBA * rgba_colors);
#define HC_MShow_Specific_Vertex_Colors_With_Existence(key, geometry, count, vertices, result_type, index_colors, rgb_colors, rgba_colors) \
		HC_MShow_Spec_Vert_Colors_W_Ex(key, geometry, count, vertices, result_type, index_colors, rgb_colors, rgba_colors)
HC_EXTERNAL int HC_CDECL HC_MShow_Spec_Vert_Normals_W_Ex (
		HC_KEY key, int count, const int * indices, char * existence, HC_VECTOR * normals);
#define HC_MShow_Specific_Vertex_Normals_With_Existence(key, count, indices, existence, normals) \
		HC_MShow_Spec_Vert_Normals_W_Ex(key, count, indices, existence, normals)
HC_EXTERNAL void HC_CDECL HC_MShow_Spec_Vertex_Parameters (
		HC_KEY key, int count, const int * indices, int * number_parameters, float * user_parameters);
HC_EXTERNAL int HC_CDECL HC_MShow_Spec_Vertex_Params_W_E (
		HC_KEY key, int count, const int * specific_vertices, char * existence, int * param_width, float * user_parameters);
#define HC_MShow_Specific_Vertex_Parameters_With_Existence(key, count, specific_vertices, existence, param_width, user_parameters) \
		HC_MShow_Spec_Vertex_Params_W_E(key, count, specific_vertices, existence, param_width, user_parameters)
HC_EXTERNAL int HC_CDECL HC_MShow_Spec_Vertex_Vis_W_Ex (
		HC_KEY key, int count, const int * indices, char * existence, char * visibilities);
#define HC_MShow_Specific_Vertex_Visibilities_With_Existence(key, count, indices, existence, visibilities) \
		HC_MShow_Spec_Vertex_Vis_W_Ex(key, count, indices, existence, visibilities)
HC_EXTERNAL void HC_CDECL HC_MShow_Specific_Edge_Vis (
		HC_KEY key, int count, const int * indices1, const int * indices2, char * settings);
#define HC_MShow_Specific_Edge_Visibilities(key, count, indices1, indices2, settings) \
		HC_MShow_Specific_Edge_Vis(key, count, indices1, indices2, settings)
HC_EXTERNAL void HC_CDECL HC_MShow_Specific_Face_Normals (
		HC_KEY key, int count, const int * indices, HC_VECTOR * normals);
HC_EXTERNAL void HC_CDECL HC_MShow_Specific_Face_Vis (
		HC_KEY key, int count, const int * indices, char * settings);
#define HC_MShow_Specific_Face_Visibilities(key, count, indices, settings) \
		HC_MShow_Specific_Face_Vis(key, count, indices, settings)
#define HC_MShow_Face_Visibilities_By_Index(key, count, indices, settings) \
		HC_MShow_Specific_Face_Vis(key, count, indices, settings)
HC_EXTERNAL void HC_CDECL HC_MShow_Specific_Vert_Normals (
		HC_KEY key, int count, const int * indices, HC_VECTOR * normals);
#define HC_MShow_Specific_Vertex_Normals(key, count, indices, normals) \
		HC_MShow_Specific_Vert_Normals(key, count, indices, normals)
HC_EXTERNAL void HC_CDECL HC_MShow_Specific_Vertex_Vis (
		HC_KEY key, int count, const int * indices, char * settings);
#define HC_MShow_Specific_Vertex_Visibilities(key, count, indices, settings) \
		HC_MShow_Specific_Vertex_Vis(key, count, indices, settings)
HC_EXTERNAL void HC_CDECL HC_MShow_Vertex_Colors_By_FInd (
		HC_KEY key, const char * type, int offset, int pcount, float * indices);
#define HC_MShow_Vertex_Colors_By_FIndex(key, type, offset, pcount, indices) \
		HC_MShow_Vertex_Colors_By_FInd(key, type, offset, pcount, indices)
HC_EXTERNAL void HC_CDECL HC_MShow_Vertex_Colors_By_Value (
		HC_KEY key, const char * type, int offset, int count, HC_RGB * rgb);
HC_EXTERNAL int HC_CDECL HC_MShow_Vertex_Colors_With_Ex (
		HC_KEY key, const char * geometry, int offset, int count, char * result_type, float * index_colors, HC_RGB * rgb_colors, HC_RGBA * rgba_colors);
#define HC_MShow_Vertex_Colors_With_Existence(key, geometry, offset, count, result_type, index_colors, rgb_colors, rgba_colors) \
		HC_MShow_Vertex_Colors_With_Ex(key, geometry, offset, count, result_type, index_colors, rgb_colors, rgba_colors)
HC_EXTERNAL void HC_CDECL HC_MShow_Vertex_Normals (
		HC_KEY key, int offset, int count, HC_VECTOR * normals);
HC_EXTERNAL int HC_CDECL HC_MShow_Vertex_Normals_W_Ex (
		HC_KEY key, int offset, int count, char * existence, HC_VECTOR * normals);
#define HC_MShow_Vertex_Normals_With_Existence(key, offset, count, existence, normals) \
		HC_MShow_Vertex_Normals_W_Ex(key, offset, count, existence, normals)
HC_EXTERNAL void HC_CDECL HC_MShow_Vertex_Parameter_Size (
		HC_KEY key, int * number);
HC_EXTERNAL void HC_CDECL HC_MShow_Vertex_Parameters (
		HC_KEY key, int offset, int pcount, int * number, float * params);
HC_EXTERNAL int HC_CDECL HC_MShow_Vertex_Parameters_W_Ex (
		HC_KEY key, int offset, int count, char * existence, int * param_width, float * user_parameters);
#define HC_MShow_Vertex_Parameters_With_Existence(key, offset, count, existence, param_width, user_parameters) \
		HC_MShow_Vertex_Parameters_W_Ex(key, offset, count, existence, param_width, user_parameters)
HC_EXTERNAL int HC_CDECL HC_MShow_Vertex_Vis_W_Ex (
		HC_KEY key, int offset, int count, char * existence, char * visibilities);
#define HC_MShow_Vertex_Visibilities_With_Existence(key, offset, count, existence, visibilities) \
		HC_MShow_Vertex_Vis_W_Ex(key, offset, count, existence, visibilities)
HC_EXTERNAL void HC_CDECL HC_MShow_Vertex_Visibilities (
		HC_KEY key, int offset, int count, char * settings);
HC_EXTERNAL void HC_CDECL HC_MUnSet_Character_Attributes (
		HC_KEY key, int offset, int count, const char * options);
HC_EXTERNAL void HC_CDECL HC_MUnSet_Face_Colors (
		HC_KEY key, int offset, int count);
HC_EXTERNAL void HC_CDECL HC_MUnSet_Face_Normals (
		HC_KEY key, int offset, int count);
HC_EXTERNAL void HC_CDECL HC_MUnSet_Face_Visibilities (
		HC_KEY key, int offset, int count);
HC_EXTERNAL void HC_CDECL HC_MUnSet_Spec_Face_Colors (
		HC_KEY key, int count, const int * faces);
#define HC_MUnSet_Specific_Face_Colors(key, count, faces) \
		HC_MUnSet_Spec_Face_Colors(key, count, faces)
HC_EXTERNAL void HC_CDECL HC_MUnSet_Spec_Vert_Parameters (
		HC_KEY key, int count, const int * indices);
#define HC_MUnSet_Specific_Vertex_Parameters(key, count, indices) \
		HC_MUnSet_Spec_Vert_Parameters(key, count, indices)
HC_EXTERNAL void HC_CDECL HC_MUnSet_Spec_Vertex_Colors (
		HC_KEY key, int count, const int * offsets);
#define HC_MUnSet_Specific_Vertex_Colors(key, count, offsets) \
		HC_MUnSet_Spec_Vertex_Colors(key, count, offsets)
HC_EXTERNAL void HC_CDECL HC_MUnSet_Spec_Vertex_Colors2 (
		HC_KEY key, const char * type, int count, const int * indices);
#define HC_MUnSet_Specific_Vertex_Colors2(key, type, count, indices) \
		HC_MUnSet_Spec_Vertex_Colors2(key, type, count, indices)
HC_EXTERNAL void HC_CDECL HC_MUnSet_Specific_Edge_Vis (
		HC_KEY key, int count, const int * indices1, const int * indices2);
#define HC_MUnSet_Specific_Edge_Visibilities(key, count, indices1, indices2) \
		HC_MUnSet_Specific_Edge_Vis(key, count, indices1, indices2)
HC_EXTERNAL void HC_CDECL HC_MUnSet_Specific_Face_Normals (
		HC_KEY key, int count, const int * indices);
HC_EXTERNAL void HC_CDECL HC_MUnSet_Specific_Face_Vis (
		HC_KEY key, int count, const int * indices);
#define HC_MUnSet_Specific_Face_Visibilities(key, count, indices) \
		HC_MUnSet_Specific_Face_Vis(key, count, indices)
#define HC_MUnSet_Face_Visibilities_By_Index(key, count, indices) \
		HC_MUnSet_Specific_Face_Vis(key, count, indices)
HC_EXTERNAL void HC_CDECL HC_MUnSet_Specific_Vert_Normals (
		HC_KEY key, int count, const int * indices);
#define HC_MUnSet_Specific_Vertex_Normals(key, count, indices) \
		HC_MUnSet_Specific_Vert_Normals(key, count, indices)
HC_EXTERNAL void HC_CDECL HC_MUnSet_Specific_Vertex_Vis (
		HC_KEY key, int count, const int * indices);
#define HC_MUnSet_Specific_Vertex_Visibilities(key, count, indices) \
		HC_MUnSet_Specific_Vertex_Vis(key, count, indices)
HC_EXTERNAL void HC_CDECL HC_MUnSet_Vertex_Colors (
		HC_KEY key, int offset, int count);
HC_EXTERNAL void HC_CDECL HC_MUnSet_Vertex_Colors2 (
		HC_KEY key, const char * type, int offset, int count);
HC_EXTERNAL void HC_CDECL HC_MUnSet_Vertex_Normals (
		HC_KEY key, int offset, int count);
HC_EXTERNAL void HC_CDECL HC_MUnSet_Vertex_Parameters (
		HC_KEY key, int offset, int count);
HC_EXTERNAL void HC_CDECL HC_MUnSet_Vertex_Visibilities (
		HC_KEY key, int offset, int count);
HC_EXTERNAL HC_POINTER_SIZED_INT HC_CDECL HC_Memory_Usage_By_Key (
		HC_KEY key, const char * which);
HC_EXTERNAL void HC_CDECL HC_Modify_Color_Map (
		int offset, const char * colors);
HC_EXTERNAL void HC_CDECL HC_Modify_Color_Map_By_Value (
		int offset, const char * color_space, int count, const HC_RGB * values);
HC_EXTERNAL void HC_CDECL HC_Move_By_Key (
		HC_KEY key, const char * newowner);
HC_EXTERNAL void HC_CDECL HC_Move_Distant_Light (
		HC_KEY key, double di, double dj, double dk);
HC_EXTERNAL void HC_CDECL HC_Move_Image (
		HC_KEY key, double x, double y, double z);
HC_EXTERNAL void HC_CDECL HC_Move_Key_By_Key (
		HC_KEY key, HC_KEY newowner);
#define HC_Move_By_Key_By_Key(key, newowner) \
		HC_Move_Key_By_Key(key, newowner)
HC_EXTERNAL void HC_CDECL HC_Move_Light_Position (
		HC_KEY key, double x, double y, double z);
HC_EXTERNAL void HC_CDECL HC_Move_Light_Target (
		HC_KEY key, double x, double y, double z);
HC_EXTERNAL void HC_CDECL HC_Move_Segment (
		const char * old_seg_name, const char * new_seg_name);
HC_EXTERNAL void HC_CDECL HC_Move_String_Cursor (
		HC_KEY key, int row, int column);
HC_EXTERNAL void HC_CDECL HC_Move_Text (
		HC_KEY key, double x, double y, double z);
HC_EXTERNAL HC_KEY HC_CDECL HC_Named_Style_Segment (
		const char * style_name);
HC_EXTERNAL HC_KEY HC_CDECL HC_Named_Style_Segment_By_Key (
		HC_KEY target_segment, const char * style_name);
HC_EXTERNAL void HC_CDECL HC_Open_Edge (
		int offset1, int offset2);
HC_EXTERNAL void HC_CDECL HC_Open_Face (
		int id);
HC_EXTERNAL void HC_CDECL HC_Open_Geometry (
		HC_KEY key);
HC_EXTERNAL void HC_CDECL HC_Open_LOD (
		int level);
HC_EXTERNAL void HC_CDECL HC_Open_Region (
		int region);
HC_EXTERNAL HC_KEY HC_CDECL HC_Open_Segment (
		const char * segment);
HC_EXTERNAL void HC_CDECL HC_Open_Segment_By_Key (
		HC_KEY key);
HC_EXTERNAL HC_KEY HC_CDECL HC_Open_Segment_Key_By_Key (
		HC_KEY parent_segment, const char * child_segment_name);
HC_EXTERNAL void HC_CDECL HC_Open_Trim (
		int offset);
HC_EXTERNAL void HC_CDECL HC_Open_Vertex (
		int offset);
HC_EXTERNAL void HC_CDECL HC_Optimize_Segment_Tree (
		const char * segname, const char * option_string);
HC_EXTERNAL void HC_CDECL HC_Optimize_Segment_Tree_By_Key (
		HC_KEY segkey, const char * option_string);
HC_EXTERNAL void HC_CDECL HC_Orbit_Camera (
		double theta, double phi);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_PShow_Net_Callback (
		int key_count, const HC_KEY * path_keys, char * callbacks);
HC_EXTERNAL int HC_CDECL HC_PShow_Net_Camera (
		int key_count, const HC_KEY * path_keys, HC_POINT * position, HC_POINT * target, HC_VECTOR * up, float * width, float * height, char * projection);
HC_EXTERNAL int HC_CDECL HC_PShow_Net_Camera_By_Volume (
		int count, const HC_KEY * keys, char * proj, float * xmin, float * xmax, float * ymin, float * ymax);
HC_EXTERNAL int HC_CDECL HC_PShow_Net_Camera_Field (
		int key_count, const HC_KEY * path_keys, float * width, float * height);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_PShow_Net_Camera_Near_Limit (
		int count, const HC_KEY * keys, float * limit);
HC_EXTERNAL int HC_CDECL HC_PShow_Net_Camera_Position (
		int key_count, const HC_KEY * path_keys, float * x, float * y, float * z);
HC_EXTERNAL int HC_CDECL HC_PShow_Net_Camera_Projection (
		int key_count, const HC_KEY * path_keys, char * projection);
HC_EXTERNAL int HC_CDECL HC_PShow_Net_Camera_Target (
		int key_count, const HC_KEY * path_keys, float * x, float * y, float * z);
HC_EXTERNAL int HC_CDECL HC_PShow_Net_Camera_Up_Vector (
		int key_count, const HC_KEY * path_keys, float * x, float * y, float * z);
HC_EXTERNAL int HC_CDECL HC_PShow_Net_Clip_Region (
		int count, const HC_KEY * keys, int * loopCount, int * loopSizeArray, HC_POINT * points, char * spec);
HC_EXTERNAL int HC_CDECL HC_PShow_Net_Clip_Region_Size (
		int count, const HC_KEY * keys, int * loopCount, int * totalPointCount, char * spec);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_PShow_Net_Color (
		int count, const HC_KEY * keys, char * color_spec);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_PShow_Net_Color_Map (
		int key_count, const HC_KEY * path_keys, char * colors);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_PShow_Net_Color_Map_By_Value (
		int key_count, const HC_KEY * path_keys, char * color_space, int * count, HC_RGB * values);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_PShow_Net_Color_Map_Count (
		int key_count, const HC_KEY * path_keys, int * count);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_PShow_Net_Color_Map_Length (
		int count, const HC_KEY * keys, int * value);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_PShow_Net_Conditions (
		int count, const HC_KEY * keys, char * options);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_PShow_Net_Conditions_Length (
		int count, const HC_KEY * keys, int * length);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_PShow_Net_Driver_Options (
		int key_count, const HC_KEY * path_keys, char * list);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_PShow_Net_Edge_Pattern (
		int key_count, const HC_KEY * path_keys, char * pattern);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_PShow_Net_Edge_Pattern_Exp (
		int count, const HC_KEY * keys, char * pattern);
#define HC_PShow_Net_Edge_Pattern_Explicit(count, keys, pattern) \
		HC_PShow_Net_Edge_Pattern_Exp(count, keys, pattern)
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_PShow_Net_Edge_Weight (
		int key_count, const HC_KEY * path_keys, float * weight);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_PShow_Net_Explicit_Color (
		int keycount, const HC_KEY * pathkeys, const char * type, const char * channel, float * rgb, char * texture, char * options);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_PShow_Net_Face_Pattern (
		int key_count, const HC_KEY * path_keys, char * pattern);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_PShow_Net_Glyph (
		int count, const HC_KEY * keys, const char * name, char * definition);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_PShow_Net_Glyph_Size (
		int count, const HC_KEY * keys, const char * name, int * size);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_PShow_Net_Handedness (
		int key_count, const HC_KEY * path_keys, char * value);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_PShow_Net_Heuristics (
		int key_count, const HC_KEY * path_keys, char * list);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_PShow_Net_Line_Pattern (
		int key_count, const HC_KEY * path_keys, char * pattern);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_PShow_Net_Line_Pattern_Exp (
		int count, const HC_KEY * keys, char * pattern);
#define HC_PShow_Net_Line_Pattern_Explicit(count, keys, pattern) \
		HC_PShow_Net_Line_Pattern_Exp(count, keys, pattern)
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_PShow_Net_Line_Style (
		int count, const HC_KEY * keys, const char * name, char * definition);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_PShow_Net_Line_Style_Size (
		int count, const HC_KEY * keys, const char * name, int * size);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_PShow_Net_Line_Weight (
		int key_count, const HC_KEY * path_keys, float * weight);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_PShow_Net_Marker_Size (
		int key_count, const HC_KEY * path_keys, float * size);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_PShow_Net_Marker_Symbol (
		int key_count, const HC_KEY * path_keys, char * symbol);
HC_EXTERNAL int HC_CDECL HC_PShow_Net_Modelling_Matrix (
		int key_count, const HC_KEY * path_keys, float * matrix);
HC_EXTERNAL HC_KEY HC_CDECL HC_PShow_Net_Named_Style (
		int count, const HC_KEY * path_keys, const char * style_name, char * style_segment_name);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_PShow_Net_Rendering_Options (
		int key_count, const HC_KEY * path_keys, char * list);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_PShow_Net_Selectability (
		int key_count, const HC_KEY * path_keys, char * list);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_PShow_Net_Shader (
		int count, const HC_KEY * keys, const char * name, char * output, char * shader_source);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_PShow_Net_Text_Alignment (
		int key_count, const HC_KEY * path_keys, char * locater);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_PShow_Net_Text_Font (
		int key_count, const HC_KEY * path_keys, char * options);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_PShow_Net_Text_Path (
		int key_count, const HC_KEY * path_keys, float * x, float * y, float * z);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_PShow_Net_Text_Spacing (
		int key_count, const HC_KEY * path_keys, float * spacing);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_PShow_Net_Texture (
		int count, const HC_KEY * keys, const char * name, char * output, HC_KEY * out_image);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_PShow_Net_Texture_Matrix (
		int key_count, const HC_KEY * path_keys, float * matrix);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_PShow_Net_Unicode_Opt_Length (
		int key_count, const HC_KEY * path_keys, int * length);
#define HC_PShow_Net_Unicode_Options_Length(key_count, path_keys, length) \
		HC_PShow_Net_Unicode_Opt_Length(key_count, path_keys, length)
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_PShow_Net_Unicode_Options (
		int key_count, const HC_KEY * path_keys, unsigned short * options);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_PShow_Net_User_Index_Count (
		int key_count, const HC_KEY * keys, int * count);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_PShow_Net_User_Indices (
		int key_count, const HC_KEY * path_keys, int * count, long * indices, void * values);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_PShow_Net_User_Opt_Length (
		int key_count, const HC_KEY * path_keys, int * length);
#define HC_PShow_Net_User_Options_Length(key_count, path_keys, length) \
		HC_PShow_Net_User_Opt_Length(key_count, path_keys, length)
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_PShow_Net_User_Options (
		int key_count, const HC_KEY * path_keys, char * list);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_PShow_Net_User_Value (
		int key_count, const HC_KEY * path_keys, HC_POINTER_SIZED_INT * data);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_PShow_Net_Var_Edge_Weight (
		int count, const HC_KEY * keys, char * weight);
#define HC_PShow_Net_Variable_Edge_Weight(count, keys, weight) \
		HC_PShow_Net_Var_Edge_Weight(count, keys, weight)
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_PShow_Net_Var_Line_Weight (
		int count, const HC_KEY * keys, char * weight);
#define HC_PShow_Net_Variable_Line_Weight(count, keys, weight) \
		HC_PShow_Net_Var_Line_Weight(count, keys, weight)
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_PShow_Net_Var_Marker_Size (
		int count, const HC_KEY * keys, char * size);
#define HC_PShow_Net_Variable_Marker_Size(count, keys, size) \
		HC_PShow_Net_Var_Marker_Size(count, keys, size)
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_PShow_Net_Visibility (
		int key_count, const HC_KEY * path_keys, char * list);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_PShow_Net_Window (
		int key_count, const HC_KEY * path_keys, float * left, float * right, float * bottom, float * top);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_PShow_Net_Window_Frame (
		int key_count, const HC_KEY * path_keys, char * flag);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_PShow_Net_Window_Pattern (
		int key_count, const HC_KEY * path_keys, char * pattern);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_PShow_One_Net_Callback (
		int count, const HC_KEY * keys, const char * callback_point, char * callback_name);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_PShow_One_Net_Callback_WD (
		int count, const HC_KEY * keys, const char * callback_point, char * callback, void * data);
#define HC_PShow_One_Net_Callback_With_Data(count, keys, callback_point, callback, data) \
		HC_PShow_One_Net_Callback_WD(count, keys, callback_point, callback, data)
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_PShow_One_Net_Color (
		int count, const HC_KEY * keys, const char * type, char * color);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_PShow_One_Net_Color_By_Index (
		int count, const HC_KEY * keys, const char * type, int * index);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_PShow_One_Net_Color_By_Value (
		int count, const HC_KEY * keys, const char * type, char * color_space, float * a, float * b, float * c);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_PShow_One_Net_Color_Map (
		int count, const HC_KEY * keys, int offset, char * color);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_PShow_One_Net_Color_Map_By_V (
		int count, const HC_KEY * keys, int offset, char * color_space, float * x, float * y, float * z);
#define HC_PShow_One_Net_Color_Map_By_Value(count, keys, offset, color_space, x, y, z) \
		HC_PShow_One_Net_Color_Map_By_V(count, keys, offset, color_space, x, y, z)
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_PShow_One_Net_Condition (
		int count, const HC_KEY * keys, const char * condition, char * options);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_PShow_One_Net_Driver_Option (
		int count, const HC_KEY * keys, const char * type, char * value);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_PShow_One_Net_Heuristic (
		int count, const HC_KEY * keys, const char * type, char * value);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_PShow_One_Net_Rendering_Opti (
		int count, const HC_KEY * keys, const char * type, char * value);
#define HC_PShow_One_Net_Rendering_Option(count, keys, type, value) \
		HC_PShow_One_Net_Rendering_Opti(count, keys, type, value)
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_PShow_One_Net_Selectability (
		int count, const HC_KEY * keys, const char * type, char * value);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_PShow_One_Net_Shader_Option (
		int count, const HC_KEY * keys, const char * name, const char * opt, char * output);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_PShow_One_Net_Text_Font (
		int count, const HC_KEY * keys, const char * type, char * value);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_PShow_One_Net_Texture (
		int count, const HC_KEY * keys, const char * name, const char * opt, char * output);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_PShow_One_Net_Uni_Opt_Len (
		int count, const HC_KEY * keys, const unsigned short * which, int * length);
#define HC_PShow_One_Net_Unicode_Option_Length(count, keys, which, length) \
		HC_PShow_One_Net_Uni_Opt_Len(count, keys, which, length)
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_PShow_One_Net_Unicode_Opt (
		int key_count, const HC_KEY * keys, const unsigned short * requested_option, unsigned short * options);
#define HC_PShow_One_Net_Unicode_Option(key_count, keys, requested_option, options) \
		HC_PShow_One_Net_Unicode_Opt(key_count, keys, requested_option, options)
HC_EXTERNAL long HC_CDECL HC_PShow_One_Net_User_Data (
		int count, const HC_KEY * keys, HC_POINTER_SIZED_INT index, void * data, long data_length);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_PShow_One_Net_User_Index (
		int count, const HC_KEY * keys, long index, void * value);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_PShow_One_Net_User_Opt_Len (
		int count, const HC_KEY * keys, const char * which, int * length);
#define HC_PShow_One_Net_User_Option_Length(count, keys, which, length) \
		HC_PShow_One_Net_User_Opt_Len(count, keys, which, length)
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_PShow_One_Net_User_Option (
		int count, const HC_KEY * keys, const char * type, char * value);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_PShow_One_Net_Visibility (
		int count, const HC_KEY * keys, const char * type, char * value);
HC_EXTERNAL void HC_CDECL HC_Pan_Camera (
		double theta, double phi);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_Parse_String (
		const char * string, const char * delimiter, int offset, char * token);
HC_EXTERNAL void HC_CDECL HC_Pause (
		void);
HC_EXTERNAL void HC_CDECL HC_Print_Version (
		void);
HC_EXTERNAL int HC_CDECL HC_QShow_Existence (
		const char * segment, const char * filter);
HC_EXTERNAL int HC_CDECL HC_QShow_Existence_Unicode (
		const char * segment, const unsigned short * filter);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_Read_Metafile (
		const char * file, const char * segment, const char * options);
HC_EXTERNAL void HC_CDECL HC_Record_Instance_Handle (
		const void * handle);
HC_EXTERNAL void HC_CDECL HC_Record_Profile_Source (
		const char * section, const char * filename);
HC_EXTERNAL HC_KEY HC_CDECL HC_Reference_Geometry (
		const char * segname);
HC_EXTERNAL HC_KEY HC_CDECL HC_Reference_Geometry_By_Key (
		HC_KEY geom_key);
HC_EXTERNAL HC_KEY HC_CDECL HC_Reference_Geometry_Key_By_Ke (
		HC_KEY target_segment, HC_KEY reference_key);
#define HC_Reference_Geometry_Key_By_Key(target_segment, reference_key) \
		HC_Reference_Geometry_Key_By_Ke(target_segment, reference_key)
HC_EXTERNAL void HC_CDECL HC_Regenerate_LOD (
		const char * segment, const char * options);
HC_EXTERNAL void HC_CDECL HC_Regenerate_LOD_By_Key (
		HC_KEY segKey, const char * options);
HC_EXTERNAL void HC_CDECL HC_Relinquish_Memory (
		void);
HC_EXTERNAL void HC_CDECL HC_Rename_Segment (
		const char * old_seg_name, const char * new_seg_name);
HC_EXTERNAL HC_KEY HC_CDECL HC_Renumber_Key (
		HC_KEY oldkey, HC_KEY newkey, const char * scope);
HC_EXTERNAL void HC_CDECL HC_Report_Error (
		int category, int specific, int severity, int msgc, const char * const * msgv, int stack_c, const char * const * stack_v);
HC_EXTERNAL void HC_CDECL HC_Reset_System (
		void);
HC_EXTERNAL void HC_CDECL HC_Restart_Ink (
		void);
HC_EXTERNAL void HC_CDECL HC_Reverse_Contents_Search (
		void);
HC_EXTERNAL void HC_CDECL HC_Roll_Camera (
		double theta);
HC_EXTERNAL void HC_CDECL HC_Rollback_Ink (
		int count);
HC_EXTERNAL void HC_CDECL HC_Rotate_Object (
		double theta, double phi, double psi);
HC_EXTERNAL void HC_CDECL HC_Rotate_Object_Offaxis (
		double x, double y, double z, double theta);
HC_EXTERNAL void HC_CDECL HC_Rotate_Texture (
		double theta, double phi, double psi);
HC_EXTERNAL void HC_CDECL HC_Rotate_Texture_Offaxis (
		double x, double y, double z, double theta);
HC_EXTERNAL void HC_CDECL HC_Scale_Object (
		double x, double y, double z);
HC_EXTERNAL void HC_CDECL HC_Scale_Texture (
		double u, double v, double w);
HC_EXTERNAL void HC_CDECL HC_Scroll_Text (
		HC_KEY key, int left_scroll, int up_scroll);
HC_EXTERNAL void HC_CDECL HC_Set_Bounding_Cuboid (
		const HC_POINT * min, const HC_POINT * max);
HC_EXTERNAL void HC_CDECL HC_Set_Bounding_Sphere (
		const HC_POINT * center, double radius);
HC_EXTERNAL void HC_CDECL HC_Set_Callback (
		const char * callbacks);
HC_EXTERNAL void HC_CDECL HC_Set_Callback_With_Data (
		const char * callback, const void * data);
HC_EXTERNAL void HC_CDECL HC_Set_Camera (
		const HC_POINT * position, const HC_POINT * target, const HC_VECTOR * up, double width, double height, const char * projection);
HC_EXTERNAL void HC_CDECL HC_Set_Camera_By_Volume (
		const char * projection, double xmin, double xmax, double ymin, double ymax);
HC_EXTERNAL void HC_CDECL HC_Set_Camera_Field (
		double width, double height);
HC_EXTERNAL void HC_CDECL HC_Set_Camera_Near_Limit (
		double wlimit);
HC_EXTERNAL void HC_CDECL HC_Set_Camera_Position (
		double x, double y, double z);
HC_EXTERNAL void HC_CDECL HC_Set_Camera_Projection (
		const char * type);
HC_EXTERNAL void HC_CDECL HC_Set_Camera_Target (
		double x, double y, double z);
HC_EXTERNAL void HC_CDECL HC_Set_Camera_Up_Vector (
		double x, double y, double z);
HC_EXTERNAL void HC_CDECL HC_Set_Circular_Center (
		double x, double y, double z);
HC_EXTERNAL void HC_CDECL HC_Set_Circular_Center_By_Key (
		HC_KEY key, double x, double y, double z);
HC_EXTERNAL void HC_CDECL HC_Set_Color (
		const char * color_spec);
HC_EXTERNAL void HC_CDECL HC_Set_Color_By_FIndex (
		const char * types, double findex);
HC_EXTERNAL void HC_CDECL HC_Set_Color_By_Index (
		const char * types, int index);
HC_EXTERNAL void HC_CDECL HC_Set_Color_By_Value (
		const char * types, const char * colorspace, double a, double b, double c);
HC_EXTERNAL void HC_CDECL HC_Set_Color_Map (
		const char * colors);
HC_EXTERNAL void HC_CDECL HC_Set_Color_Map_By_Value (
		const char * color_space, int count, const HC_RGB * values);
HC_EXTERNAL void HC_CDECL HC_Set_Complex_Clip_Region (
		int loops, const int * lengths, const HC_POINT * points, const char * options);
HC_EXTERNAL void HC_CDECL HC_Set_Conditional_Action (
		const char * options, const char * condition);
HC_EXTERNAL void HC_CDECL HC_Set_Conditions (
		const char * list);
HC_EXTERNAL void HC_CDECL HC_Set_Default_Glyphs (
		void);
HC_EXTERNAL void HC_CDECL HC_Set_Default_Glyphs_By_Key (
		HC_KEY target_segment);
HC_EXTERNAL void HC_CDECL HC_Set_Default_Line_Styles (
		void);
HC_EXTERNAL void HC_CDECL HC_Set_Default_Line_Styles_By_K (
		HC_KEY target_segment);
#define HC_Set_Default_Line_Styles_By_Key(target_segment) \
		HC_Set_Default_Line_Styles_By_K(target_segment)
HC_EXTERNAL void HC_CDECL HC_Set_Driver_Options (
		const char * list);
HC_EXTERNAL void HC_CDECL HC_Set_Edge_Pattern (
		const char * pattern);
HC_EXTERNAL void HC_CDECL HC_Set_Edge_Pattern_Explicit (
		const char * pattern);
HC_EXTERNAL void HC_CDECL HC_Set_Edge_Weight (
		double weight);
HC_EXTERNAL void HC_CDECL HC_Set_Explicit_Color (
		const char * type, const char * channel, const HC_POINT * rgb, const char * texture, const char * options);
HC_EXTERNAL void HC_CDECL HC_Set_Face_Pattern (
		const char * pattern);
HC_EXTERNAL void HC_CDECL HC_Set_Faces (
		int first_face, int face_count);
HC_EXTERNAL void HC_CDECL HC_Set_Geometry_Options (
		HC_KEY key, const char * list);
#define HC_Edit_Geometry_Options(key, list) \
		HC_Set_Geometry_Options(key, list)
HC_EXTERNAL void HC_CDECL HC_Set_Handedness (
		const char * value);
HC_EXTERNAL void HC_CDECL HC_Set_Heuristics (
		const char * list);
HC_EXTERNAL void HC_CDECL HC_Set_Line_Pattern (
		const char * pattern);
HC_EXTERNAL void HC_CDECL HC_Set_Line_Pattern_Explicit (
		const char * pattern);
HC_EXTERNAL void HC_CDECL HC_Set_Line_Weight (
		double weight);
HC_EXTERNAL void HC_CDECL HC_Set_Marker_Size (
		double weight);
HC_EXTERNAL void HC_CDECL HC_Set_Marker_Symbol (
		const char * symbol);
HC_EXTERNAL void HC_CDECL HC_Set_Modelling_Matrix (
		const float * matrix);
HC_EXTERNAL void HC_CDECL HC_Set_Normal (
		double x, double y, double z);
HC_EXTERNAL void HC_CDECL HC_Set_Parameter (
		int number, const float * parameters);
HC_EXTERNAL void HC_CDECL HC_Set_Polygonal_Clip_Region (
		int count, const HC_POINT * points, const char * options);
HC_EXTERNAL void HC_CDECL HC_Set_Priority (
		HC_KEY key, int priority);
HC_EXTERNAL void HC_CDECL HC_Set_Region (
		int region);
HC_EXTERNAL void HC_CDECL HC_Set_Rendering_Options (
		const char * list);
HC_EXTERNAL void HC_CDECL HC_Set_Selectability (
		const char * list);
HC_EXTERNAL void HC_CDECL HC_Set_Streaming_Mode (
		const char * flag);
HC_EXTERNAL void HC_CDECL HC_Set_Text_Alignment (
		const char * locater);
HC_EXTERNAL void HC_CDECL HC_Set_Text_Font (
		const char * list);
HC_EXTERNAL void HC_CDECL HC_Set_Text_Path (
		double x, double y, double z);
HC_EXTERNAL void HC_CDECL HC_Set_Text_Region (
		int pCount, const HC_POINT * points, const char * options);
HC_EXTERNAL void HC_CDECL HC_Set_Text_Size (
		double ivalue);
HC_EXTERNAL void HC_CDECL HC_Set_Text_Spacing (
		double spacing);
HC_EXTERNAL void HC_CDECL HC_Set_Texture_Matrix (
		const float * matrix);
HC_EXTERNAL void HC_CDECL HC_Set_Trim_Operation (
		int index, const char * operation);
HC_EXTERNAL void HC_CDECL HC_Set_Unicode_Options (
		const unsigned short * options);
HC_EXTERNAL void HC_CDECL HC_Set_User_Data (
		HC_POINTER_SIZED_INT index, const void * data, long data_length);
HC_EXTERNAL void HC_CDECL HC_Set_User_Index (
		long index, const void * data);
HC_EXTERNAL void HC_CDECL HC_Set_User_Options (
		const char * list);
HC_EXTERNAL void HC_CDECL HC_Set_User_Value (
		HC_POINTER_SIZED_INT data);
HC_EXTERNAL void HC_CDECL HC_Set_Variable_Edge_Weight (
		const char * weight);
HC_EXTERNAL void HC_CDECL HC_Set_Variable_Line_Weight (
		const char * weight);
HC_EXTERNAL void HC_CDECL HC_Set_Variable_Marker_Size (
		const char * size);
HC_EXTERNAL void HC_CDECL HC_Set_Visibility (
		const char * list);
HC_EXTERNAL void HC_CDECL HC_Set_Window (
		double left, double right, double bottom, double top);
HC_EXTERNAL void HC_CDECL HC_Set_Window_Frame (
		const char * flag);
HC_EXTERNAL void HC_CDECL HC_Set_Window_Pattern (
		const char * pattern);
HC_EXTERNAL void HC_CDECL HC_Show_Alias (
		const char * alias, char * expansion);
HC_EXTERNAL void HC_CDECL HC_Show_Alias_Count (
		int * count);
HC_EXTERNAL void HC_CDECL HC_Show_Area_Light (
		HC_KEY key, int * ucount, HC_POINT * upoints, char * listptr);
HC_EXTERNAL void HC_CDECL HC_Show_Area_Light_Count (
		HC_KEY key, int * count, char * listptr);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_Show_Attribute_Filter (
		HC_KEY key, char * filter);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_Show_Attribute_Filter_Length (
		HC_KEY key, int * filter_length);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_Show_Bounding_Cuboid (
		HC_POINT * min, HC_POINT * max);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_Show_Bounding_Cuboid_By_Key (
		HC_KEY key, HC_POINT * min, HC_POINT * max);
HC_EXTERNAL void HC_CDECL HC_Show_Bounding_Info (
		char * list);
HC_EXTERNAL void HC_CDECL HC_Show_Bounding_Info_By_Key (
		HC_KEY key, char * list);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_Show_Bounding_Sphere (
		HC_POINT * center, float * radius);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_Show_Bounding_Sphere_By_Key (
		HC_KEY key, HC_POINT * center, float * radius);
HC_EXTERNAL void HC_CDECL HC_Show_Button (
		char * button);
HC_EXTERNAL void HC_CDECL HC_Show_Button_Source (
		char * keyboard, char * button, int * status);
HC_EXTERNAL void HC_CDECL HC_Show_Callback (
		char * callbacks);
HC_EXTERNAL void HC_CDECL HC_Show_Callback_Name (
		const char * name, void (HC_CDECL ** callback)(HC_ANY_ARGS));
HC_EXTERNAL void HC_CDECL HC_Show_Callback_Name_Count (
		int * count);
HC_EXTERNAL void HC_CDECL HC_Show_Camera (
		HC_POINT * position, HC_POINT * target, HC_VECTOR * up, float * width, float * height, char * projection);
HC_EXTERNAL void HC_CDECL HC_Show_Camera_By_Volume (
		char * projection, float * xmin, float * xmax, float * ymin, float * ymax);
HC_EXTERNAL void HC_CDECL HC_Show_Camera_Field (
		float * width, float * height);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_Show_Camera_Near_Limit (
		float * limit);
HC_EXTERNAL void HC_CDECL HC_Show_Camera_Position (
		float * x, float * y, float * z);
HC_EXTERNAL void HC_CDECL HC_Show_Camera_Projection (
		char * projection);
HC_EXTERNAL void HC_CDECL HC_Show_Camera_Target (
		float * x, float * y, float * z);
HC_EXTERNAL void HC_CDECL HC_Show_Camera_Up_Vector (
		float * x, float * y, float * z);
HC_EXTERNAL void HC_CDECL HC_Show_Character_Attribute_Cou (
		HC_KEY key, int * count);
#define HC_Show_Character_Attribute_Count(key, count) \
		HC_Show_Character_Attribute_Cou(key, count)
HC_EXTERNAL void HC_CDECL HC_Show_Circle (
		HC_KEY key, HC_POINT * point1, HC_POINT * point2, HC_POINT * point3);
HC_EXTERNAL void HC_CDECL HC_Show_Circle_By_Radius (
		HC_KEY key, HC_POINT * center, float * radius, HC_VECTOR * normal);
HC_EXTERNAL void HC_CDECL HC_Show_Circular_Arc (
		HC_KEY key, HC_POINT * point1, HC_POINT * point2, HC_POINT * point3);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_Show_Circular_Center (
		HC_KEY key, float * x, float * y, float * z);
HC_EXTERNAL void HC_CDECL HC_Show_Circular_Chord (
		HC_KEY key, HC_POINT * point1, HC_POINT * point2, HC_POINT * point3);
HC_EXTERNAL void HC_CDECL HC_Show_Circular_Wedge (
		HC_KEY key, HC_POINT * point1, HC_POINT * point2, HC_POINT * point3);
HC_EXTERNAL void HC_CDECL HC_Show_Clip_Region (
		int * loopCount, int * loopSizeArray, HC_POINT * points, char * options);
HC_EXTERNAL void HC_CDECL HC_Show_Clip_Region_Size (
		int * loopCount, int * totalPointCount, char * options);
HC_EXTERNAL void HC_CDECL HC_Show_Color (
		char * color_spec);
HC_EXTERNAL void HC_CDECL HC_Show_Color_By_Index (
		char * types, int * index);
HC_EXTERNAL void HC_CDECL HC_Show_Color_By_Value (
		char * types, char * colorspace, float * a, float * b, float * c);
HC_EXTERNAL void HC_CDECL HC_Show_Color_Map (
		char * colors);
HC_EXTERNAL void HC_CDECL HC_Show_Color_Map_By_Value (
		char * color_space, int * count, HC_RGB * values);
HC_EXTERNAL void HC_CDECL HC_Show_Color_Map_Count (
		int * count);
HC_EXTERNAL void HC_CDECL HC_Show_Color_Map_Length (
		int * value);
HC_EXTERNAL void HC_CDECL HC_Show_Color_Name (
		const char * color, char * definition);
HC_EXTERNAL void HC_CDECL HC_Show_Color_Name_Count (
		int * count);
HC_EXTERNAL void HC_CDECL HC_Show_Compressed_Image (
		HC_KEY key, float * x, float * y, float * z, char * format, int * width, int * height, int * size, void * data);
HC_EXTERNAL void HC_CDECL HC_Show_Compressed_Image_Size (
		HC_KEY key, char * format, int * width, int * height, int * size);
HC_EXTERNAL void HC_CDECL HC_Show_Cond_Action_Types (
		char * action_types);
#define HC_Show_Conditional_Action_Types(action_types) \
		HC_Show_Cond_Action_Types(action_types)
HC_EXTERNAL void HC_CDECL HC_Show_Conditional_Include (
		HC_KEY key, char * pathname, char * conditions);
HC_EXTERNAL void HC_CDECL HC_Show_Conditional_Named_Style (
		HC_KEY style_key, char * style_name, char * condition);
HC_EXTERNAL HC_KEY HC_CDECL HC_Show_Conditional_Reference (
		HC_KEY reference_key, char * conditions);
HC_EXTERNAL void HC_CDECL HC_Show_Conditional_Style (
		HC_KEY key, char * pathname, char * conditions);
HC_EXTERNAL void HC_CDECL HC_Show_Conditions (
		char * list);
HC_EXTERNAL void HC_CDECL HC_Show_Conditions_Length (
		int * length);
HC_EXTERNAL void HC_CDECL HC_Show_Contents_Count (
		int * count);
HC_EXTERNAL void HC_CDECL HC_Show_Cutting_Plane (
		HC_KEY key, float * a, float * b, float * c, float * d);
HC_EXTERNAL void HC_CDECL HC_Show_Cutting_Section (
		HC_KEY key, int * count, HC_PLANE * planes);
HC_EXTERNAL void HC_CDECL HC_Show_Cutting_Section_Count (
		HC_KEY key, int * count);
HC_EXTERNAL void HC_CDECL HC_Show_Cylinder (
		HC_KEY key, HC_POINT * p1, HC_POINT * p2, float * radius, char * cap);
HC_EXTERNAL void HC_CDECL HC_Show_Device_Info (
		const char * driver, const char * item, char * data);
HC_EXTERNAL void HC_CDECL HC_Show_Device_Info_By_Key (
		HC_KEY key, const char * item, char * data);
HC_EXTERNAL void HC_CDECL HC_Show_Distant_Light (
		HC_KEY key, float * di, float * dj, float * dk);
HC_EXTERNAL void HC_CDECL HC_Show_Driver_Options (
		char * list);
HC_EXTERNAL void HC_CDECL HC_Show_Edge_Pattern (
		char * pattern);
HC_EXTERNAL void HC_CDECL HC_Show_Edge_Pattern_Explicit (
		char * pattern);
HC_EXTERNAL void HC_CDECL HC_Show_Edge_Weight (
		float * weight);
HC_EXTERNAL void HC_CDECL HC_Show_Ellipse (
		HC_KEY key, HC_POINT * center, HC_POINT * major, HC_POINT * minor);
HC_EXTERNAL void HC_CDECL HC_Show_Elliptical_Arc (
		HC_KEY key, HC_POINT * center, HC_POINT * major, HC_POINT * minor, float * start, float * end);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_Show_Environment (
		const char * variable, char * value);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_Show_Error (
		char * severity, int * category, int * severity_level);
HC_EXTERNAL void HC_CDECL HC_Show_Error_Handler_Count (
		int * count);
HC_EXTERNAL int HC_CDECL HC_Show_Existence (
		const char * filter);
HC_EXTERNAL int HC_CDECL HC_Show_Existence_By_Key (
		HC_KEY key, const char * filter);
HC_EXTERNAL int HC_CDECL HC_Show_Existence_Unicode (
		const unsigned short * filter);
HC_EXTERNAL int HC_CDECL HC_Show_Existence_Unicode_By_K (
		HC_KEY key, const unsigned short * filter);
#define HC_Show_Existence_Unicode_By_Key(key, filter) \
		HC_Show_Existence_Unicode_By_K(key, filter)
HC_EXTERNAL void HC_CDECL HC_Show_Exit_Handler_Count (
		int * count);
HC_EXTERNAL void HC_CDECL HC_Show_Explicit_Color (
		const char * type, const char * channel, float * rgb, char * texture, char * options);
HC_EXTERNAL void HC_CDECL HC_Show_Face_Pattern (
		char * pattern);
HC_EXTERNAL void HC_CDECL HC_Show_Faces (
		int * face_count, int * faces);
HC_EXTERNAL void HC_CDECL HC_Show_Faces_Count (
		int * face_count);
HC_EXTERNAL void HC_CDECL HC_Show_Font (
		const char * name, char * options, int * data_length, char * data);
HC_EXTERNAL void HC_CDECL HC_Show_Font_Count (
		int * count);
HC_EXTERNAL void HC_CDECL HC_Show_Font_Info (
		const char * driver, const char * name, const char * item, char * data);
HC_EXTERNAL void HC_CDECL HC_Show_Font_Info_By_Path (
		int count, const HC_KEY * keys, const char * font, const char * item, char * data);
HC_EXTERNAL void HC_CDECL HC_Show_Font_Size (
		const char * name, char * options, int * data_length);
HC_EXTERNAL void HC_CDECL HC_Show_Geometry_Options (
		HC_KEY key, char * list);
HC_EXTERNAL void HC_CDECL HC_Show_Geometry_Pointer (
		HC_KEY key, const char * data_type, void * value);
HC_EXTERNAL void HC_CDECL HC_Show_Glyph (
		const char * name, char * data);
HC_EXTERNAL void HC_CDECL HC_Show_Glyph_Count (
		int * count);
HC_EXTERNAL void HC_CDECL HC_Show_Glyph_Size (
		const char * name, int * data_size);
HC_EXTERNAL void HC_CDECL HC_Show_Grid (
		HC_KEY key, char * type, HC_POINT * origin, HC_POINT * ref1, HC_POINT * ref2, int * count1, int * count2);
HC_EXTERNAL void HC_CDECL HC_Show_Handedness (
		char * value);
HC_EXTERNAL void HC_CDECL HC_Show_Heuristics (
		char * list);
HC_EXTERNAL void HC_CDECL HC_Show_Image (
		HC_KEY key, float * x, float * y, float * z, char * format, int * width, int * height, void * data);
HC_EXTERNAL int HC_CDECL HC_Show_Image_Bytes_Per_Pixel (
		HC_KEY key);
HC_EXTERNAL void HC_CDECL HC_Show_Image_Format (
		HC_KEY key, char * format);
HC_EXTERNAL void HC_CDECL HC_Show_Image_Name (
		HC_KEY key, char * name);
HC_EXTERNAL void HC_CDECL HC_Show_Image_Size (
		HC_KEY key, float * x, float * y, float * z, char * format, int * width, int * height);
HC_EXTERNAL HC_KEY HC_CDECL HC_Show_Include_Segment (
		HC_KEY key, char * pathname);
HC_EXTERNAL void HC_CDECL HC_Show_Infinite_Line (
		HC_KEY key, float * xa, float * ya, float * za, float * xb, float * yb, float * zb);
HC_EXTERNAL void HC_CDECL HC_Show_Infinite_Ray (
		HC_KEY key, float * xa, float * ya, float * za, float * xb, float * yb, float * zb);
HC_EXTERNAL int HC_CDECL HC_Show_Intersect_Polyline_Size (
		int pcount1, const HC_POINT * points1, int flist_length1, const int * face_list1, int pcount2, const HC_POINT * points2, int flist_length2, const int * face_list2, int * polyline_count, int * polyline_points_count);
HC_EXTERNAL void HC_CDECL HC_Show_Key_Status (
		HC_KEY key, char * status);
HC_EXTERNAL void HC_CDECL HC_Show_Key_Type (
		HC_KEY key, char * type);
HC_EXTERNAL void HC_CDECL HC_Show_LOD_Type (
		HC_KEY key, int level, char * type);
HC_EXTERNAL void HC_CDECL HC_Show_Line (
		HC_KEY key, float * xa, float * ya, float * za, float * xb, float * yb, float * zb);
HC_EXTERNAL void HC_CDECL HC_Show_Line_Pattern (
		char * pattern);
HC_EXTERNAL void HC_CDECL HC_Show_Line_Pattern_Explicit (
		char * pattern);
HC_EXTERNAL void HC_CDECL HC_Show_Line_Style (
		const char * style, char * definition);
HC_EXTERNAL void HC_CDECL HC_Show_Line_Style_Count (
		int * count);
HC_EXTERNAL void HC_CDECL HC_Show_Line_Style_Size (
		const char * style, int * size);
HC_EXTERNAL void HC_CDECL HC_Show_Line_Weight (
		float * weight);
HC_EXTERNAL void HC_CDECL HC_Show_Local_Light (
		HC_KEY key, float * x, float * y, float * z);
HC_EXTERNAL void HC_CDECL HC_Show_Local_Texture (
		const char * texture_name, char * definition);
HC_EXTERNAL void HC_CDECL HC_Show_Location (
		float * x, float * y);
HC_EXTERNAL void HC_CDECL HC_Show_Location_Source (
		char * locater, char * display, char * action, int * status);
HC_EXTERNAL void HC_CDECL HC_Show_Marker (
		HC_KEY key, float * x, float * y, float * z);
HC_EXTERNAL void HC_CDECL HC_Show_Marker_Size (
		float * size);
HC_EXTERNAL void HC_CDECL HC_Show_Marker_Symbol (
		char * symbol);
HC_EXTERNAL void HC_CDECL HC_Show_Memory_Usage (
		long * allocated, long * in_use);
HC_EXTERNAL void HC_CDECL HC_Show_Mesh (
		HC_KEY key, int * rows, int * columns, HC_POINT * points);
HC_EXTERNAL void HC_CDECL HC_Show_Mesh_Size (
		HC_KEY key, int * rows, int * columns);
HC_EXTERNAL void HC_CDECL HC_Show_Modelling_Matrix (
		float * matrix);
HC_EXTERNAL void HC_CDECL HC_Show_NURBS_Curve (
		HC_KEY key, int * degree, int * cp_count, HC_POINT * points, float * weights, float * knots, float * start_u, float * end_u);
HC_EXTERNAL void HC_CDECL HC_Show_NURBS_Curve_Size (
		HC_KEY key, int * degree, int * cp_count, int * weight_count, int * knot_count);
HC_EXTERNAL void HC_CDECL HC_Show_NURBS_Surface (
		HC_KEY key, int * u_degree, int * v_degree, int * u_count, int * v_count, HC_POINT * points, float * weights, float * u_knots, float * v_knots);
HC_EXTERNAL void HC_CDECL HC_Show_NURBS_Surface_Size (
		HC_KEY key, int * u_degree, int * v_degree, int * u_count, int * v_count, int * weights_count, int * u_knots_count, int * v_knots_count);
HC_EXTERNAL HC_KEY HC_CDECL HC_Show_Named_Style (
		const char * style_name, char * style_source);
HC_EXTERNAL void HC_CDECL HC_Show_Named_Style_Count (
		int * count);
HC_EXTERNAL void HC_CDECL HC_Show_Named_Style_Segment (
		HC_KEY style_key, char * style_name);
HC_EXTERNAL void HC_CDECL HC_Show_Net_Normal (
		float * x, float * y, float * z);
HC_EXTERNAL void HC_CDECL HC_Show_Normal (
		float * x, float * y, float * z);
HC_EXTERNAL void HC_CDECL HC_Show_One_Callback (
		const char * callback_point, char * callback_name);
HC_EXTERNAL void HC_CDECL HC_Show_One_Callback_With_Data (
		const char * callback_point, char * callback, void * data);
HC_EXTERNAL void HC_CDECL HC_Show_One_Character_Attribute (
		HC_KEY key, int offset, const char * which, char * options);
HC_EXTERNAL void HC_CDECL HC_Show_One_Color (
		const char * type, char * color);
HC_EXTERNAL void HC_CDECL HC_Show_One_Color_By_Index (
		const char * type, int * index);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_Show_One_Color_By_Value (
		const char * type, char * color_space, float * a, float * b, float * c);
HC_EXTERNAL void HC_CDECL HC_Show_One_Color_Map (
		int offset, char * color);
HC_EXTERNAL void HC_CDECL HC_Show_One_Color_Map_By_Value (
		int offset, char * color_space, float * a, float * b, float * c);
HC_EXTERNAL void HC_CDECL HC_Show_One_Condition (
		const char * condition, char * value);
HC_EXTERNAL void HC_CDECL HC_Show_One_Conditional_Action (
		const char * action_type, char * options, char * condition);
HC_EXTERNAL void HC_CDECL HC_Show_One_Driver_Option (
		const char * type, char * value);
HC_EXTERNAL void HC_CDECL HC_Show_One_Heuristic (
		const char * type, char * value);
HC_EXTERNAL void HC_CDECL HC_Show_One_Rendering_Option (
		const char * type, char * value);
HC_EXTERNAL void HC_CDECL HC_Show_One_Selectability (
		const char * type, char * value);
HC_EXTERNAL void HC_CDECL HC_Show_One_System_Option (
		const char * type, char * value);
HC_EXTERNAL void HC_CDECL HC_Show_One_Text_Font (
		const char * type, char * value);
HC_EXTERNAL void HC_CDECL HC_Show_One_Uni_Option_Length (
		const unsigned short * text, int * length);
#define HC_Show_One_Unicode_Option_Length(text, length) \
		HC_Show_One_Uni_Option_Length(text, length)
HC_EXTERNAL void HC_CDECL HC_Show_One_Unicode_Option (
		const unsigned short * requestedOption, unsigned short * options);
HC_EXTERNAL long HC_CDECL HC_Show_One_User_Data (
		HC_POINTER_SIZED_INT index, void * data, long data_length);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_Show_One_User_Index (
		long index, void * value);
HC_EXTERNAL void HC_CDECL HC_Show_One_User_Option (
		const char * type, char * value);
HC_EXTERNAL void HC_CDECL HC_Show_One_User_Option_Length (
		const char * which, int * length);
HC_EXTERNAL void HC_CDECL HC_Show_One_Visibility (
		const char * type, char * value);
HC_EXTERNAL void HC_CDECL HC_Show_Open_Item_Count (
		int * count);
HC_EXTERNAL void HC_CDECL HC_Show_Open_Segment_Count (
		int * count);
HC_EXTERNAL void HC_CDECL HC_Show_Optimized_Mapping (
		const char * segname, const char * direction, HC_KEY in_owner, HC_KEY in_key, int in_region, HC_KEY * out_owner, HC_KEY * out_key, int * out_region);
HC_EXTERNAL HC_KEY HC_CDECL HC_Show_Original_Key (
		HC_KEY key);
HC_EXTERNAL HC_KEY HC_CDECL HC_Show_Owner (
		const char * segment, char * owner);
HC_EXTERNAL HC_KEY HC_CDECL HC_Show_Owner_By_Key (
		HC_KEY key, char * owner);
HC_EXTERNAL HC_KEY HC_CDECL HC_Show_Owner_Original_Key (
		HC_KEY key);
HC_EXTERNAL void HC_CDECL HC_Show_Parameter (
		int * size, float * list);
HC_EXTERNAL void HC_CDECL HC_Show_Parameter_Size (
		int * size);
HC_EXTERNAL void HC_CDECL HC_Show_Partial_Image (
		HC_KEY key, int xpos, int ypos, int xlen, int ylen, void * data);
HC_EXTERNAL void HC_CDECL HC_Show_Partial_Mesh (
		HC_KEY key, int row_offset, int col_offset, int row_count, int col_count, HC_POINT * points);
HC_EXTERNAL void HC_CDECL HC_Show_Partial_Polygon (
		HC_KEY key, int offset, int request, HC_POINT * points);
HC_EXTERNAL void HC_CDECL HC_Show_Partial_Polyline (
		HC_KEY key, int offset, int request, HC_POINT * points);
HC_EXTERNAL void HC_CDECL HC_Show_Partial_Shell (
		HC_KEY key, int voffset, int vcnt, HC_POINT * points, int foffset, int fcnt, int * flist_length, int * face_list);
HC_EXTERNAL void HC_CDECL HC_Show_Partial_Shell_Size (
		HC_KEY key, int foffset, int fcnt, int * flist_length);
HC_EXTERNAL void HC_CDECL HC_Show_Pathname_Expansion (
		const char * pathname, char * expansion);
HC_EXTERNAL void HC_CDECL HC_Show_PolyCylinder (
		HC_KEY key, int * p_count, HC_POINT * pts, int * r_count, float * radii, char * capping);
HC_EXTERNAL void HC_CDECL HC_Show_PolyCylinder_Counts (
		HC_KEY key, int * p_count, int * r_count, char * capping);
HC_EXTERNAL void HC_CDECL HC_Show_Polygon (
		HC_KEY key, int * count, HC_POINT * points);
HC_EXTERNAL void HC_CDECL HC_Show_Polygon_Count (
		HC_KEY key, int * count);
HC_EXTERNAL void HC_CDECL HC_Show_Polyline (
		HC_KEY key, int * count, HC_POINT * points);
HC_EXTERNAL void HC_CDECL HC_Show_Polyline_Count (
		HC_KEY key, int * count);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_Show_Priority (
		HC_KEY key, int * priority);
HC_EXTERNAL void HC_CDECL HC_Show_Priority_Range (
		HC_KEY key, int * low, int * high);
HC_EXTERNAL HC_KEY HC_CDECL HC_Show_Reference_Geometry (
		HC_KEY reference_key);
HC_EXTERNAL void HC_CDECL HC_Show_Reference_Geometry_Filt (
		HC_KEY key, char * filter);
#define HC_Show_Reference_Geometry_Filter(key, filter) \
		HC_Show_Reference_Geometry_Filt(key, filter)
HC_EXTERNAL void HC_CDECL HC_Show_Reference_Keys (
		HC_KEY key, int * count, HC_KEY * keys);
HC_EXTERNAL void HC_CDECL HC_Show_Reference_Keys_Count (
		HC_KEY key, int * count);
HC_EXTERNAL void HC_CDECL HC_Show_Region (
		int * region);
HC_EXTERNAL void HC_CDECL HC_Show_Region_Range (
		HC_KEY key, int * lowest, int * highest);
HC_EXTERNAL void HC_CDECL HC_Show_Rendering_Options (
		char * list);
HC_EXTERNAL HC_KEY HC_CDECL HC_Show_Renumbered_Key (
		HC_KEY key, char * status);
HC_EXTERNAL int HC_CDECL HC_Show_Sample_Max_Length (
		void);
HC_EXTERNAL HC_KEY HC_CDECL HC_Show_Segment (
		HC_KEY key, char * pathname);
HC_EXTERNAL void HC_CDECL HC_Show_Segment_Count (
		int * count);
HC_EXTERNAL HC_KEY HC_CDECL HC_Show_Segment_Name (
		HC_KEY key, char * name);
HC_EXTERNAL int HC_CDECL HC_Show_Segment_Name_Length (
		HC_KEY key);
HC_EXTERNAL void HC_CDECL HC_Show_Selectability (
		char * list);
HC_EXTERNAL void HC_CDECL HC_Show_Selection (
		char * segment);
HC_EXTERNAL void HC_CDECL HC_Show_Selection_Element (
		HC_KEY * key, int * offset1, int * offset2, int * offset3);
HC_EXTERNAL void HC_CDECL HC_Show_Selection_Elements (
		HC_KEY * key, int * count, int * vertex1, int * vertex2, int * faces);
HC_EXTERNAL void HC_CDECL HC_Show_Selection_Elements_Coun (
		HC_KEY * key, int * count);
#define HC_Show_Selection_Elements_Count(key, count) \
		HC_Show_Selection_Elements_Coun(key, count)
HC_EXTERNAL void HC_CDECL HC_Show_Selection_Item (
		HC_KEY * key, int * offset1, int * offset2);
HC_EXTERNAL void HC_CDECL HC_Show_Selection_Keys (
		int * count, HC_KEY * keys);
HC_EXTERNAL void HC_CDECL HC_Show_Selection_Keys_Count (
		int * count);
HC_EXTERNAL void HC_CDECL HC_Show_Selection_Location (
		float * xw, float * yw, float * xc, float * yc, float * zc);
HC_EXTERNAL void HC_CDECL HC_Show_Selection_Original_Key (
		HC_KEY * key);
HC_EXTERNAL void HC_CDECL HC_Show_Selection_Original_Keys (
		int * count, HC_KEY * key);
HC_EXTERNAL void HC_CDECL HC_Show_Selection_Pathname (
		char * segment);
HC_EXTERNAL int HC_CDECL HC_Show_Selection_Position (
		float * window_x, float * window_y, float * window_z, float * camera_x, float * camera_y, float * camera_z);
HC_EXTERNAL void HC_CDECL HC_Show_Selection_Source (
		char * locater, char * picture, char * action, int * status);
HC_EXTERNAL void HC_CDECL HC_Show_Selection_Source_Elem (
		int * vertex, int * edge, int * face, HC_POINT * hit_location);
#define HC_Show_Selection_Source_Element(vertex, edge, face, hit_location) \
		HC_Show_Selection_Source_Elem(vertex, edge, face, hit_location)
HC_EXTERNAL void HC_CDECL HC_Show_Shader (
		const char * name, char * definition, char * shader_source);
HC_EXTERNAL void HC_CDECL HC_Show_Shell (
		HC_KEY key, int * pcount, HC_POINT * points, int * flist_length, int * face_list);
HC_EXTERNAL void HC_CDECL HC_Show_Shell_By_Tristrips (
		HC_KEY key, int * pcount, HC_POINT * points, int * tristrips_length, int * tristrips, int * face_indices_length, int * face_indices);
HC_EXTERNAL void HC_CDECL HC_Show_Shell_By_Tristrips_Size (
		HC_KEY key, int * pcount, int * tristrips_length, int * face_indices_length);
HC_EXTERNAL void HC_CDECL HC_Show_Shell_Face_Count (
		HC_KEY key, int * face_count);
HC_EXTERNAL void HC_CDECL HC_Show_Shell_Size (
		HC_KEY key, int * pcount, int * flist_length);
HC_EXTERNAL void HC_CDECL HC_Show_Shell_Tristrip_Count (
		HC_KEY key, int * tristrip_count);
HC_EXTERNAL void HC_CDECL HC_Show_Sphere (
		HC_KEY key, HC_POINT * center, float * radius, HC_VECTOR * axis, HC_VECTOR * ortho);
HC_EXTERNAL void HC_CDECL HC_Show_Spot_Light (
		HC_KEY key, HC_POINT * position, HC_POINT * target, char * list);
HC_EXTERNAL void HC_CDECL HC_Show_Streaming_Mode (
		char * flag);
HC_EXTERNAL void HC_CDECL HC_Show_String (
		char * text);
HC_EXTERNAL void HC_CDECL HC_Show_String_Count (
		int * count);
HC_EXTERNAL void HC_CDECL HC_Show_String_Cursor (
		HC_KEY key, HC_KEY * text_key, int * row, int * col);
HC_EXTERNAL void HC_CDECL HC_Show_String_Length (
		int * length);
HC_EXTERNAL void HC_CDECL HC_Show_String_Source (
		char * keyboard, HC_KEY * cursor_key);
HC_EXTERNAL void HC_CDECL HC_Show_String_With_Encoding (
		char * encoding, void * text);
HC_EXTERNAL HC_KEY HC_CDECL HC_Show_Style_Segment (
		HC_KEY key, char * pathname);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_Show_System_Info (
		const char * type, char * value);
HC_EXTERNAL void HC_CDECL HC_Show_System_Options (
		char * list);
HC_EXTERNAL void HC_CDECL HC_Show_Text (
		HC_KEY key, float * x, float * y, float * z, char * text);
HC_EXTERNAL void HC_CDECL HC_Show_Text_Alignment (
		char * locater);
HC_EXTERNAL void HC_CDECL HC_Show_Text_Count (
		HC_KEY key, int * count);
HC_EXTERNAL void HC_CDECL HC_Show_Text_Encoding (
		HC_KEY key, char * encoding);
HC_EXTERNAL void HC_CDECL HC_Show_Text_Font (
		char * options);
HC_EXTERNAL void HC_CDECL HC_Show_Text_Length (
		HC_KEY key, int * length);
HC_EXTERNAL void HC_CDECL HC_Show_Text_Path (
		float * x, float * y, float * z);
HC_EXTERNAL void HC_CDECL HC_Show_Text_Region (
		int * count, HC_POINT * points, char * options);
HC_EXTERNAL void HC_CDECL HC_Show_Text_Region_Count (
		int * count);
HC_EXTERNAL void HC_CDECL HC_Show_Text_Size (
		float * value);
HC_EXTERNAL void HC_CDECL HC_Show_Text_Spacing (
		float * spacing);
HC_EXTERNAL void HC_CDECL HC_Show_Text_With_Encoding (
		HC_KEY key, float * x, float * y, float * z, char * encoding, void * text);
HC_EXTERNAL void HC_CDECL HC_Show_Texture (
		const char * texture, char * definition);
HC_EXTERNAL void HC_CDECL HC_Show_Texture_Count (
		int * count);
HC_EXTERNAL void HC_CDECL HC_Show_Texture_Matrix (
		float * matrix);
HC_EXTERNAL void HC_CDECL HC_Show_Time (
		float * time);
HC_EXTERNAL void HC_CDECL HC_Show_Trim_Count (
		int * count);
HC_EXTERNAL void HC_CDECL HC_Show_Trim_Curve (
		int index, int * degree, int * cp_count, float * points, float * weights, float * knots, float * start_u, float * end_u);
HC_EXTERNAL void HC_CDECL HC_Show_Trim_Curve_Count (
		int index, int * degree, int * cp_count, int * w_count, int * knot_count);
HC_EXTERNAL void HC_CDECL HC_Show_Trim_Operation (
		int index, char * operation);
HC_EXTERNAL void HC_CDECL HC_Show_Trim_Poly (
		int index, int * count, float * points);
HC_EXTERNAL void HC_CDECL HC_Show_Trim_Poly_Count (
		int index, int * count);
HC_EXTERNAL void HC_CDECL HC_Show_Trim_Type (
		int index, char * trim_type);
HC_EXTERNAL void HC_CDECL HC_Show_Unicode_Options (
		unsigned short * options);
HC_EXTERNAL void HC_CDECL HC_Show_Unicode_Options_Length (
		int * length);
HC_EXTERNAL void HC_CDECL HC_Show_Unicode_Text (
		HC_KEY key, float * x, float * y, float * z, unsigned short * text);
HC_EXTERNAL long HC_CDECL HC_Show_User_Data_Indices (
		HC_POINTER_SIZED_INT * data_indices, long data_indices_count);
HC_EXTERNAL void HC_CDECL HC_Show_User_Index_Count (
		int * count);
HC_EXTERNAL void HC_CDECL HC_Show_User_Indices (
		int * count, long * indices, void * values);
HC_EXTERNAL void HC_CDECL HC_Show_User_Options (
		char * list);
HC_EXTERNAL void HC_CDECL HC_Show_User_Options_Length (
		int * length);
HC_EXTERNAL void HC_CDECL HC_Show_User_Value (
		HC_POINTER_SIZED_INT * data);
HC_EXTERNAL void HC_CDECL HC_Show_Variable_Edge_Weight (
		char * weight);
HC_EXTERNAL void HC_CDECL HC_Show_Variable_Line_Weight (
		char * weight);
HC_EXTERNAL void HC_CDECL HC_Show_Variable_Marker_Size (
		char * size);
HC_EXTERNAL void HC_CDECL HC_Show_Visibility (
		char * list);
HC_EXTERNAL void HC_CDECL HC_Show_Wakeup (
		float * time);
HC_EXTERNAL void HC_CDECL HC_Show_Window (
		float * left, float * right, float * bottom, float * top);
HC_EXTERNAL void HC_CDECL HC_Show_Window_Frame (
		char * flag);
HC_EXTERNAL void HC_CDECL HC_Show_Window_Pattern (
		char * pattern);
HC_EXTERNAL HC_KEY HC_CDECL HC_Style_Segment (
		const char * style);
HC_EXTERNAL HC_KEY HC_CDECL HC_Style_Segment_By_Key (
		HC_KEY seg_key);
HC_EXTERNAL HC_KEY HC_CDECL HC_Style_Segment_Key_By_Key (
		HC_KEY target_segment, HC_KEY style_segment);
HC_EXTERNAL void HC_CDECL HC_Translate_Object (
		double x, double y, double z);
HC_EXTERNAL void HC_CDECL HC_Translate_Texture (
		double u, double v, double w);
HC_EXTERNAL void HC_CDECL HC_Trim_NURBS_Surface_By_Curve (
		int degree, int cpcount, const float * control_points, const float * weights, const float * knots, double start_u, double end_u);
HC_EXTERNAL void HC_CDECL HC_Trim_NURBS_Surface_By_Poly (
		int cpcount, const float * points);
HC_EXTERNAL void HC_CDECL HC_UnDefine_Alias (
		const char * name);
HC_EXTERNAL void HC_CDECL HC_UnDefine_Callback_Name (
		const char * name);
HC_EXTERNAL void HC_CDECL HC_UnDefine_Color_Name (
		const char * name);
HC_EXTERNAL void HC_CDECL HC_UnDefine_Error_Handler (
		void (HC_CDECL * handler)(HC_ANY_ARGS));
HC_EXTERNAL void HC_CDECL HC_UnDefine_Exit_Handler (
		void (HC_CDECL * handler)(HC_ANY_ARGS));
HC_EXTERNAL void HC_CDECL HC_UnDefine_Font (
		const char * name);
HC_EXTERNAL int HC_CDECL HC_UnDefine_Geometry_Highlight (
		int count, const HC_KEY * keys, const char * style, const char * options, int elements_count, const int * offset1, const int * offset2, const int * offset3);
HC_EXTERNAL void HC_CDECL HC_UnDefine_Glyph (
		const char * name);
HC_EXTERNAL int HC_CDECL HC_UnDefine_Highlight (
		int count, const HC_KEY * keys, const char * style, const char * options);
HC_EXTERNAL void HC_CDECL HC_UnDefine_Line_Style (
		const char * name);
HC_EXTERNAL void HC_CDECL HC_UnDefine_Local_Texture (
		const char * texture_name);
HC_EXTERNAL void HC_CDECL HC_UnDefine_Named_Style (
		const char * style_name);
HC_EXTERNAL void HC_CDECL HC_UnDefine_Shader (
		const char * iname);
HC_EXTERNAL void HC_CDECL HC_UnDefine_Texture (
		const char * texture_name);
HC_EXTERNAL void HC_CDECL HC_UnSet_Bounding_Volume (
		void);
HC_EXTERNAL void HC_CDECL HC_UnSet_Callback (
		void);
HC_EXTERNAL void HC_CDECL HC_UnSet_Camera (
		void);
HC_EXTERNAL void HC_CDECL HC_UnSet_Camera_Near_Limit (
		void);
HC_EXTERNAL void HC_CDECL HC_UnSet_Clip_Region (
		void);
HC_EXTERNAL void HC_CDECL HC_UnSet_Color (
		void);
HC_EXTERNAL void HC_CDECL HC_UnSet_Color_Map (
		void);
HC_EXTERNAL void HC_CDECL HC_UnSet_Conditional_Actions (
		void);
HC_EXTERNAL void HC_CDECL HC_UnSet_Conditions (
		void);
HC_EXTERNAL void HC_CDECL HC_UnSet_Driver_Options (
		void);
HC_EXTERNAL void HC_CDECL HC_UnSet_Edge_Pattern (
		void);
HC_EXTERNAL void HC_CDECL HC_UnSet_Edge_Weight (
		void);
HC_EXTERNAL void HC_CDECL HC_UnSet_Face_Pattern (
		void);
HC_EXTERNAL void HC_CDECL HC_UnSet_Handedness (
		void);
HC_EXTERNAL void HC_CDECL HC_UnSet_Heuristics (
		void);
HC_EXTERNAL void HC_CDECL HC_UnSet_Line_Pattern (
		void);
HC_EXTERNAL void HC_CDECL HC_UnSet_Line_Weight (
		void);
HC_EXTERNAL void HC_CDECL HC_UnSet_Marker_Size (
		void);
HC_EXTERNAL void HC_CDECL HC_UnSet_Marker_Symbol (
		void);
HC_EXTERNAL void HC_CDECL HC_UnSet_Modelling_Matrix (
		void);
HC_EXTERNAL void HC_CDECL HC_UnSet_Normal (
		void);
HC_EXTERNAL void HC_CDECL HC_UnSet_One_Callback (
		const char * callback);
HC_EXTERNAL void HC_CDECL HC_UnSet_One_Color (
		const char * which);
HC_EXTERNAL void HC_CDECL HC_UnSet_One_Condition (
		const char * condition);
HC_EXTERNAL void HC_CDECL HC_UnSet_One_Conditional_Action (
		const char * which);
HC_EXTERNAL void HC_CDECL HC_UnSet_One_Driver_Option (
		const char * which);
HC_EXTERNAL void HC_CDECL HC_UnSet_One_Heuristic (
		const char * which);
HC_EXTERNAL void HC_CDECL HC_UnSet_One_Rendering_Option (
		const char * which);
HC_EXTERNAL void HC_CDECL HC_UnSet_One_Selectability (
		const char * which);
HC_EXTERNAL void HC_CDECL HC_UnSet_One_Text_Font (
		const char * which);
HC_EXTERNAL void HC_CDECL HC_UnSet_One_Unicode_Option (
		const unsigned short * which);
HC_EXTERNAL void HC_CDECL HC_UnSet_One_User_Data (
		HC_POINTER_SIZED_INT index);
HC_EXTERNAL void HC_CDECL HC_UnSet_One_User_Index (
		long index);
HC_EXTERNAL void HC_CDECL HC_UnSet_One_User_Option (
		const char * which);
HC_EXTERNAL void HC_CDECL HC_UnSet_One_Visibility (
		const char * which);
HC_EXTERNAL void HC_CDECL HC_UnSet_Parameter (
		void);
HC_EXTERNAL void HC_CDECL HC_UnSet_Priority (
		HC_KEY key);
HC_EXTERNAL void HC_CDECL HC_UnSet_Rendering_Options (
		void);
HC_EXTERNAL void HC_CDECL HC_UnSet_Selectability (
		void);
HC_EXTERNAL void HC_CDECL HC_UnSet_Streaming_Mode (
		void);
HC_EXTERNAL void HC_CDECL HC_UnSet_Text_Alignment (
		void);
HC_EXTERNAL void HC_CDECL HC_UnSet_Text_Font (
		void);
HC_EXTERNAL void HC_CDECL HC_UnSet_Text_Path (
		void);
HC_EXTERNAL void HC_CDECL HC_UnSet_Text_Region (
		void);
HC_EXTERNAL void HC_CDECL HC_UnSet_Text_Spacing (
		void);
HC_EXTERNAL void HC_CDECL HC_UnSet_Texture_Matrix (
		void);
HC_EXTERNAL void HC_CDECL HC_UnSet_Unicode_Options (
		void);
HC_EXTERNAL void HC_CDECL HC_UnSet_User_Data (
		void);
HC_EXTERNAL void HC_CDECL HC_UnSet_User_Indices (
		void);
HC_EXTERNAL void HC_CDECL HC_UnSet_User_Options (
		void);
HC_EXTERNAL void HC_CDECL HC_UnSet_User_Value (
		void);
HC_EXTERNAL void HC_CDECL HC_UnSet_Visibility (
		void);
HC_EXTERNAL void HC_CDECL HC_UnSet_Window (
		void);
HC_EXTERNAL void HC_CDECL HC_UnSet_Window_Frame (
		void);
HC_EXTERNAL void HC_CDECL HC_UnSet_Window_Pattern (
		void);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_Update_Display (
		void);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_Update_Display_Timed (
		double time);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_Update_One_Display (
		const char * segment);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_Update_One_Display_Timed (
		const char * segment, double time);
HC_EXTERNAL HC_BOOLEAN HC_CDECL HC_Write_Metafile (
		const char * segment, const char * file, const char * options);
HC_EXTERNAL void HC_CDECL HC_Zoom_Camera (
		double zoom);

#define HC_DEFINED
#endif /* HC_DEFINED */

#ifndef HOOPS_19_EXCLUDE_HC_LEGACY_H
#include "hc_legacy.h"
#endif

#ifdef INCLUDE_HOOPS_STANDALONE
#include "hc_standalone.h"
#endif
