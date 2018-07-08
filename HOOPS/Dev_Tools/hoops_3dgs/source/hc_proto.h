// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef HC_PROTO_DEFINED

#ifndef HPS_CORE_BUILD

HC_INTERFACE void HC_CDECL HC_Abort_Program (char const *message);
HC_INTERFACE void HC_CDECL HC_Append_Modelling_Matrix (float const array[]);
HC_INTERFACE void HC_CDECL HC_DAppend_Modelling_Matrix (double const array[]);
HC_INTERFACE void HC_CDECL HC_Append_Texture_Matrix (float const array[]);
HC_INTERFACE void HC_CDECL HC_Begin_Alias_Search (void);
HC_INTERFACE void HC_CDECL HC_Begin_Callback_Name_Search (void);
HC_INTERFACE void HC_CDECL HC_Begin_Color_Name_Search (void);
HC_INTERFACE void HC_CDECL HC_Begin_Contents_Search (char const *segment, char const *filter);
HC_INTERFACE void HC_CDECL HC_Begin_Font_Search (char const *display, char const *filter);
HC_INTERFACE void HC_CDECL HC_Begin_Glyph_Search (void);
HC_INTERFACE void HC_CDECL HC_Begin_Line_Style_Search (void);
HC_INTERFACE void HC_CDECL HC_Begin_Open_Segment_Search (void);
HC_INTERFACE void HC_CDECL HC_Begin_Segment_Search (char const *segspec);
HC_INTERFACE void HC_CDECL HC_Begin_Shader_Search (void);
HC_INTERFACE void HC_CDECL HC_Begin_Texture_Search (void);
HC_INTERFACE void HC_CDECL HC_Begin_Local_Texture_Search (void);
HC_INTERFACE void HC_CDECL HC_Bring_To_Front (char const *segment);
HC_INTERFACE void HC_CDECL HC_Bring_To_Front_By_Key (Key key);
HC_INTERFACE void HC_CDECL HC_Close_Edge (void);
HC_INTERFACE void HC_CDECL HC_Close_Face (void);
HC_INTERFACE void HC_CDECL HC_Close_Geometry (void);
HC_INTERFACE void HC_CDECL HC_Close_LOD (void);
HC_INTERFACE void HC_CDECL HC_Close_Region (void);
HC_INTERFACE void HC_CDECL HC_Close_Segment (void);
HC_INTERFACE void HC_CDECL HC_Close_Vertex (void);
HC_INTERFACE bool HC_CDECL HC_Compute_Circumcuboid (char const *segment, Point *min, Point *max);
HC_INTERFACE bool HC_CDECL HC_Compute_Circumcuboid_By_Key (Key key, Point *min, Point *max);
HC_INTERFACE bool HC_CDECL HC_Compute_Circumsphere (char const *segment, Point *center, float *radius);
HC_INTERFACE bool HC_CDECL HC_Compute_Circumsphere_By_Key (Key key, Point *center, float *radius);
HC_INTERFACE bool HC_CDECL HC_DCompute_Circumcuboid (char const *segment, DPoint *min, DPoint *max);
HC_INTERFACE bool HC_CDECL HC_DCompute_Circumcuboid_By_Key (Key key, DPoint *min, DPoint *max);
HC_INTERFACE bool HC_CDECL HC_DCompute_Circumsphere (char const *segment, DPoint *center, double *radius);
HC_INTERFACE bool HC_CDECL HC_DCompute_Circumsphere_By_Key (Key key, DPoint *center, double *radius);
HC_INTERFACE bool HC_CDECL HC_Compute_Color (char const *name, char const *out_type, RGB *out_value);
HC_INTERFACE bool HC_CDECL HC_Compute_Color_By_Value (char const *in_type, RGB const *in_value, char const *out_type, RGB *out_value);
HC_INTERFACE void HC_CDECL HC_Compute_Convex_Hull (int pcount, Point const points[], int *fcount, int face_list[]);
HC_INTERFACE bool HC_CDECL HC_Compute_Coordinates (char const *segment,
		 char const *in_system,
		 Point const *in_position,
		 char const *out_system,
		 Point *out_position);
HC_INTERFACE bool HC_CDECL HC_Compute_Coordinates_By_Path (int count,
		 Key const keys[],
		 char const *in_system,
		 Point const *in_position,
		 char const *out_system,
		 Point *out_position);
HC_INTERFACE void HC_CDECL HC_Compute_Cross_Product (Vector const *vector1, Vector const *vector2, Vector *out_vector);
HC_INTERFACE void HC_CDECL HC_DCompute_Cross_Product (DVector const *vector1, DVector const *vector2, DVector *out_vector);
HC_INTERFACE double HC_CDECL HC_Compute_Dot_Product (Vector const *vector1, Vector const *vector2);
HC_INTERFACE double HC_CDECL HC_DCompute_Dot_Product (DVector const *vector1, DVector const *vector2);
HC_INTERFACE void HC_CDECL HC_Compute_Identity_Matrix (float out_matrix[]);
HC_INTERFACE void HC_CDECL HC_DCompute_Identity_Matrix (double out_matrix[]);
HC_INTERFACE void HC_CDECL HC_Compute_Matrix_Adjoint (float const matrix[], float out_matrix[]);
HC_INTERFACE void HC_CDECL HC_DCompute_Matrix_Adjoint (double const matrix[], double out_matrix[]);
HC_INTERFACE double HC_CDECL HC_Compute_Matrix_Determinant (float const matrix[]);
HC_INTERFACE double HC_CDECL HC_DCompute_Matrix_Determinant (double const matrix[]);
HC_INTERFACE bool HC_CDECL HC_Compute_Matrix_Inverse (float const matrix[], float out_matrix[]);
HC_INTERFACE bool HC_CDECL HC_DCompute_Matrix_Inverse (double const matrix[], double out_matrix[]);
HC_INTERFACE void HC_CDECL HC_Compute_Matrix_Product (float const matrix1[], float const matrix2[], float out_matrix[]);
HC_INTERFACE void HC_CDECL HC_DCompute_Matrix_Product (double const matrix1[], double const matrix2[], double out_matrix[]);
HC_INTERFACE void HC_CDECL HC_Compute_Minimized_Shell (int in_point_count,
		 Point const in_points[],
		 int in_face_list_length,
		 int const in_face_list[],
		 char const *option_string,
		 int *out_point_count,
		 Point out_points[],
		 int *out_face_list_length,
		 int out_face_list[],
		 int vertex_mapping[],
		 int face_mapping[]);
HC_INTERFACE bool HC_CDECL HC_Compute_Normalized_Matrix (float const matrix[], float out_matrix[]);
HC_INTERFACE bool HC_CDECL HC_DCompute_Normalized_Matrix (double const matrix[], double out_matrix[]);
HC_INTERFACE bool HC_CDECL HC_Compute_Normalized_Vector (Vector const *vector, Vector *out_vector);
HC_INTERFACE bool HC_CDECL HC_DCompute_Normalized_Vector (DVector const *vector, DVector *out_vector);
HC_INTERFACE void HC_CDECL HC_Compute_Offaxis_Rotation (double x, double y, double z, double theta, float out_matrix[]);
HC_INTERFACE void HC_CDECL HC_DCompute_Offaxis_Rotation (double x, double y, double z, double theta, double out_matrix[]);
HC_INTERFACE void HC_CDECL HC_Compute_Offcenter_Rotation (double alpha, double beta, double theta, double x, double y, double z,
														  float out_matrix[]);
HC_INTERFACE void HC_CDECL HC_DCompute_Offcenter_Rotation (double alpha, double beta, double theta, double x, double y, double z,
														   double out_matrix[]);
HC_INTERFACE void HC_CDECL HC_Compute_Optimized_Shell (int pcount,
	Point const 	points[],
	Vector const 	normals[],
	int				flist_length,
	int const 		face_list[],
	char const *	options,
	int *			new_pcount,
	Point 			new_points[],
	int *			new_flist_length,
	int 			new_face_list[],
	int 			point_mapping_list[],
	int 			face_mapping_list[]);
HC_INTERFACE Key HC_CDECL HC_Compute_Capping_Shell  (
	int				shell_count,
	Key const		shell_list[],
	float const		plane[]);
HC_INTERFACE Key HC_CDECL HC_Compute_Boolean_Shell (
	int				target_pcount,
	Point const 	target_points[],
	Vector const 	target_normals[],
	int				target_flist_length,
	int const 		target_face_list[],
	int				tool_pcount,
	Point const 	tool_points[],
	Vector const 	tool_normals[],
	int				tool_flist_length,
	int const 		tool_face_list[],
	char const *	options);
HC_INTERFACE bool HC_CDECL HC_Compute_Polygon_Normal (int count, Point const points[], char const *hand, Vector *out_vector);
HC_INTERFACE bool HC_CDECL HC_DCompute_Polygon_Normal (int count, DPoint const points[], char const *hand, DVector *out_vector);
HC_INTERFACE bool HC_CDECL HC_Compute_Polygon_Plane (int count, Point const points[], char const *hand, Plane *out_plane);
HC_INTERFACE bool HC_CDECL HC_DCompute_Polygon_Plane (int count, DPoint const points[], char const *hand, DPlane *out_plane);
HC_INTERFACE void HC_CDECL HC_Compute_Rotation_Matrix (double alpha, double beta, double theta, float out_matrix[]);
HC_INTERFACE void HC_CDECL HC_DCompute_Rotation_Matrix (double alpha, double beta, double theta, double out_matrix[]);
HC_INTERFACE void HC_CDECL HC_Compute_Scale_Matrix (double x, double y, double z, float out_matrix[]);
HC_INTERFACE void HC_CDECL HC_DCompute_Scale_Matrix (double x, double y, double z, double out_matrix[]);
HC_INTERFACE int HC_CDECL HC_Compute_Selection (char const *display,
		 char const *start_seg,
		 char const *action,
		 double x,
		 double y);
HC_INTERFACE int HC_CDECL HC_Compute_Selection_By_Area (char const *display,
		 char const *start_seg,
		 char const *action,
		 double left,
		 double right,
		 double bottom,
		 double top);
HC_INTERFACE int HC_CDECL HC_Compute_Selection_By_Polygon (char const *display,
		 char const *start_seg,
		 char const *action,
		 int pcount,
		 Point const points[]);
HC_INTERFACE int HC_CDECL HC_Compute_Selection_By_Polylin (char const *display,
		 char const *start_seg,
		 char const *action,
		 int pcount,
		 Point const points[]); // HC_Compute_Selection_By_Polyline
HC_INTERFACE int HC_CDECL HC_Compute_Selection_By_Volume (char const *display,
		 char const *start_seg,
		 char const *action,
		 double left,
		 double right,
		 double bottom,
		 double top,
		 double hither,
		 double yon);
HC_INTERFACE void HC_CDECL HC_Compute_Text16_From_Text (char const *intext, char const *encoding, int *outcount, unsigned short outtext16[]);
HC_INTERFACE bool HC_CDECL HC_Compute_Text_Extent (char const *segment, char const *text, float *xsize, float *ysize);
HC_INTERFACE bool HC_CDECL HC_Compute_Text_Extent_By_Key (Key key, float *xfrac, float *yfrac);
HC_INTERFACE bool HC_CDECL HC_Compute_Text_Extent_By_Path (int count, Key const keys[], char const *text, float *xfrac, float *yfrac);
HC_INTERFACE bool HC_CDECL HC_Compute_Text_Extent_Enc_Path (int count,
		 Key const keys[],
		 char const *encoding,
		 void const *text,
		 float *xfrac,
		 float *yfrac);
HC_INTERFACE bool HC_CDECL HC_Compute_Text_Extent_With_Enc (char const *segment,
		 char const *encoding,
		 void const *text,
		 float *xfrac,
		 float *yfrac); // HC_Compute_Text_Extent_With_Encoding
HC_INTERFACE void HC_CDECL HC_Compute_Text_From_Text16 (int incount, unsigned short const intext16[], char const *encoding, char *outtext);
HC_INTERFACE void HC_CDECL HC_Compute_Text_Outline (char const *segment,
		 char const *text,
		 int *pcount,
		 Point points[],
		 int *flist_length,
		 int face_list[]);
HC_INTERFACE void HC_CDECL HC_Compute_Text_Outline_By_Key (Key key, int *pcount, Point points[], int *flist_length, int face_list[]);
HC_INTERFACE void HC_CDECL HC_Compute_Text_Outline_Size (char const *segment, char const *text, int *pcount, int *flist_length);
HC_INTERFACE void HC_CDECL HC_Compute_Text_Outline_Size_By (Key key, int *pcount, int *flist_length); // HC_Compute_Text_Outline_Size_By_Key
HC_INTERFACE void HC_CDECL HC_Compute_Text_Outline_Size_Wi (
	char const *	segment,
	char const *	encoding,
	char const *	etext,
	int *			pcount,
	int *			flist_length); // HC_Compute_Text_Outline_Size_With_Encoding
HC_INTERFACE void HC_CDECL HC_Compute_Text_Outline_With_En (char const *segment,
	char const *	encoding,
	char const *	etext,
	int *			pcount,
	Point 			points[],
	int *			flist_length,
	int 			face_list[]); // HC_Compute_Text_Outline_With_Encoding
HC_INTERFACE void HC_CDECL HC_Compute_Text_Outline_By_Path (
		int						count,
		Key const 				keys[],
		char const *			text,
		int *					pcount,
		Point 					points[],
		int *					flist_length,
		int 					face_list[]);
HC_INTERFACE void HC_CDECL HC_Compute_Text_Outline_Size_Pa (
		int						count,
		Key const 				keys[],
		char const *			text,
		int *					pcount,
		int *					flist_length); // HC_Compute_Text_Outline_Size_By_Path
HC_INTERFACE bool HC_CDECL HC_Compute_Transform (char const *segment,
		 char const *in_system,
		 char const *out_system,
		 float matrix[]);
HC_INTERFACE bool HC_CDECL HC_DCompute_Transform (char const *segment,
		 char const *in_system,
		 char const *out_system,
		 double matrix[]);
HC_INTERFACE bool HC_CDECL HC_Compute_Transform_By_Path (int count,
		 Key const keys[],
		 char const *in_system,
		 char const *out_system,
		 float matrix[]);
HC_INTERFACE bool HC_CDECL HC_DCompute_Transform_By_Path (int count,
		 Key const keys[],
		 char const *in_system,
		 char const *out_system,
		 double matrix[]);
HC_INTERFACE bool HC_CDECL HC_Compute_Transformed_Plane (Plane const *plane, float const matrix[], Plane *out_plane);
HC_INTERFACE bool HC_CDECL HC_DCompute_Transformed_Plane (DPlane const *plane, double const matrix[], DPlane *out_plane);
HC_INTERFACE bool HC_CDECL HC_Compute_Transformed_Planes (int count, Plane const planes[], float const matrix[], Plane out_planes[]);
HC_INTERFACE bool HC_CDECL HC_DCompute_Transformed_Planes (int count, DPlane const planes[], double const matrix[], DPlane out_planes[]);
HC_INTERFACE void HC_CDECL HC_Compute_Transformed_Points (int count, Point const points[], float const matrix[], Point out_points[]);
HC_INTERFACE void HC_CDECL HC_Compute_Translation_Matrix (double x, double y, double z, float out_matrix[]);
HC_INTERFACE void HC_CDECL HC_DCompute_Translation_Matrix (double x, double y, double z, double out_matrix[]);
HC_INTERFACE double HC_CDECL HC_Compute_Vector_Length (Vector const *vector);
HC_INTERFACE double HC_CDECL HC_DCompute_Vector_Length (DVector const *vector);
HC_INTERFACE void HC_CDECL HC_Control_Update (char const *segment, char const *options);
HC_INTERFACE void HC_CDECL HC_Control_Update_Area (char const *segment, double left, double right, double bottom, double top, char const *options);
HC_INTERFACE void HC_CDECL HC_Control_Update_Area_By_Key (Key key, double left, double right, double bottom, double top, char const *options);
HC_INTERFACE void HC_CDECL HC_Control_Update_By_Key (Key key, char const *options);
HC_INTERFACE bool HC_CDECL HC_Convert_Precision(Key key, char const * options);
HC_INTERFACE void HC_CDECL HC_Define_Alias (char const *name, char const *expansion);
HC_INTERFACE void HC_CDECL HC_Define_Callback_Name (char const *name, Void_Routine callback);
HC_INTERFACE void HC_CDECL HC_Define_Color_Name (char const *name, char const *er_name, char const *ish_name, char const *definition);
HC_INTERFACE void HC_CDECL HC_Define_Driver_Config (char const *driver, char const *id, char const *version, char const *config);
HC_INTERFACE void HC_CDECL HC_Define_Error_Handler (Void_Routine handler);
HC_INTERFACE void HC_CDECL HC_Define_Exit_Handler (Void_Routine handler);
HC_INTERFACE void HC_CDECL HC_Define_Font (char const *name, char const *options, int data_length, char const data[]);
HC_INTERFACE void HC_CDECL HC_Define_Font_By_Reference (char const *name, char const *options, int data_length, char const data[]); // HC_Define_Font_By_Ref
HC_INTERFACE void HC_CDECL HC_Define_Glyph (char const *name, int data_size, char const data[]);
HC_INTERFACE void HC_CDECL HC_Define_Line_Style (char const *name, char const *definition);
HC_INTERFACE void HC_CDECL HC_Define_System_Options (char const *list);
HC_INTERFACE void HC_CDECL HC_Define_Texture (char const *texture_name, char const *definition);
HC_INTERFACE void HC_CDECL HC_Define_Local_Texture (char const *texture_name, char const *definition);
HC_INTERFACE void HC_CDECL HC_Define_Shader (char const *name, char const *options, char const *shader_source);
HC_INTERFACE void HC_CDECL HC_Delete_By_Key (Key key);
HC_INTERFACE void HC_CDECL HC_Delete_Segment (char const *segment);
HC_INTERFACE void HC_CDECL HC_Dolly_Camera (double right, double up, double in);
HC_INTERFACE intptr_t HC_CDECL HC_Dump_Memory_Profile (char const *which);
HC_INTERFACE void HC_CDECL HC_Edit_Image (Key key, int xoffset, int yoffset, int xcount, int ycount, const void *data);
HC_INTERFACE void HC_CDECL HC_Edit_Image_Options (Key image_key, char const *options);
HC_INTERFACE void HC_CDECL HC_Edit_Mesh (Key key, int row_offset, int column_offset, int row_count, int column_count, Point const points[]);
HC_INTERFACE void HC_CDECL HC_Edit_NURBS_Curve (Key key,
		 int cp_offset,
		 int knot_offset,
		 int cp_count,
		 int knot_count,
		 Point const control_points[],
		 float const weights[],
		 float const knots[]);
HC_INTERFACE void HC_CDECL HC_Edit_Polygon (Key key, int offset, int ndelete, int insert, Point const points[]);
HC_INTERFACE void HC_CDECL HC_Edit_Polyline (Key key, int offset, int ndelete, int insert, Point const points[]);
HC_INTERFACE void HC_CDECL HC_Edit_Shell_Faces (Key key, int offset, int ndelete, int ilist_length, int const insert_list[]);
HC_INTERFACE void HC_CDECL HC_Edit_Shell_Points (Key key, int offset, int ndelete, int insert, Point const points[]);
HC_INTERFACE void HC_CDECL HC_Edit_Text (Key key, int row, int column, int erase, int ndelete, char const *new_text);
HC_INTERFACE void HC_CDECL HC_End_Alias_Search (void);
HC_INTERFACE void HC_CDECL HC_End_Callback_Name_Search (void);
HC_INTERFACE void HC_CDECL HC_End_Color_Name_Search (void);
HC_INTERFACE void HC_CDECL HC_End_Contents_Search (void);
HC_INTERFACE void HC_CDECL HC_End_Font_Search (void);
HC_INTERFACE void HC_CDECL HC_End_Open_Segment_Search (void);
HC_INTERFACE void HC_CDECL HC_End_Segment_Search (void);
HC_INTERFACE void HC_CDECL HC_End_Texture_Search (void);
HC_INTERFACE void HC_CDECL HC_End_Shader_Search (void);
HC_INTERFACE void HC_CDECL HC_End_Glyph_Search (void);
HC_INTERFACE void HC_CDECL HC_End_Line_Style_Search (void);
HC_INTERFACE void HC_CDECL HC_Exit_Program (void);
HC_INTERFACE bool HC_CDECL HC_Find_Alias (char * alias);
HC_INTERFACE bool HC_CDECL HC_Find_Callback_Name (char *name);
HC_INTERFACE bool HC_CDECL HC_Find_Color_Name (char *name, char *er_name, char *ish_name);
HC_INTERFACE bool HC_CDECL HC_Find_Contents (char *type, Key *key);
HC_INTERFACE bool HC_CDECL HC_Find_Font (char *name);
HC_INTERFACE bool HC_CDECL HC_Find_Open_Segment (char *segment);
HC_INTERFACE bool HC_CDECL HC_Find_Related_Selection (void);
HC_INTERFACE bool HC_CDECL HC_Find_Segment (char *segment);
HC_INTERFACE bool HC_CDECL HC_Find_Texture (char *name);
HC_INTERFACE bool HC_CDECL HC_Find_Shader (char *sname);
HC_INTERFACE bool HC_CDECL HC_Find_Glyph (char *name);
HC_INTERFACE bool HC_CDECL HC_Find_Line_Style (char *name);
HC_INTERFACE void HC_CDECL HC_Flush_Attributes (char const *segment);
HC_INTERFACE void HC_CDECL HC_Flush_By_Key (Key key);
HC_INTERFACE void HC_CDECL HC_Flush_Contents (char const *segment, char const *filter);
HC_INTERFACE void HC_CDECL HC_Flush_Geometry (char const *segment);
HC_INTERFACE void HC_CDECL HC_Edit_Sphere (Key key, Point const *center, double radius, Vector const *axis, Vector const *ortho);
HC_INTERFACE void HC_CDECL HC_Edit_Circle (Key key, Point const *point1, Point const *point2, Point const *point3);
HC_INTERFACE void HC_CDECL HC_Edit_Circle_By_Radius (Key key, Point const *center, double radius, Vector const *vector);
HC_INTERFACE void HC_CDECL HC_Edit_Circular_Arc (Key key, Point const *point1, Point const *point2, Point const *point3);
HC_INTERFACE void HC_CDECL HC_Edit_Circular_Chord (Key key, Point const *point1, Point const *point2, Point const *point3);
HC_INTERFACE void HC_CDECL HC_Edit_Circular_Wedge (Key key, Point const *point1, Point const *point2, Point const *point3);
HC_INTERFACE void HC_CDECL HC_Edit_Cutting_Plane (Key key, double a, double b, double c, double d);
HC_INTERFACE void HC_CDECL HC_Edit_Ellipse (Key key, Point const *center, Point const *major, Point const *minor);
HC_INTERFACE void HC_CDECL HC_Edit_Elliptical_Arc (
		 Key key, 
		 Point const *center,
		 Point const *major,
		 Point const *minor,
		 double start,
		 double end);
HC_INTERFACE void HC_CDECL HC_Edit_Grid (
		 Key key, 
		 char const *type,
		 Point const *origin,
		 Point const *ref1,
		 Point const *ref2,
		 int count1,
		 int count2);
HC_INTERFACE void HC_CDECL HC_Edit_Line (Key key, double xa, double ya, double za, double xb, double yb, double zb);
HC_INTERFACE void HC_CDECL HC_Edit_Infinite_Line (Key key, double xa, double ya, double za, double xb, double yb, double zb);
HC_INTERFACE void HC_CDECL HC_Edit_Infinite_Ray (Key key, double xa, double ya, double za, double xb, double yb, double zb);
HC_INTERFACE void HC_CDECL HC_DEdit_Infinite_Line (Key key, double xa, double ya, double za, double xb, double yb, double zb);
HC_INTERFACE void HC_CDECL HC_DEdit_Infinite_Ray (Key key, double xa, double ya, double za, double xb, double yb, double zb);
HC_INTERFACE void HC_CDECL HC_Edit_Marker (Key key, double x, double y, double z);
HC_INTERFACE void HC_CDECL HC_Edit_Cylinder (Key key, Point const *p1, Point const *p2, double radius, char const *cap);
HC_INTERFACE void HC_CDECL HC_Edit_PolyCylinder_Points (Key key, int offset, int delete_count, int insert_count, Point const points[]);
HC_INTERFACE void HC_CDECL HC_Edit_PolyCylinder_Radii (Key key, int offset, int delete_count, int insert_count, float const radii[]);
HC_INTERFACE void HC_CDECL HC_DEdit_PolyCylinder_Points (Key key, int offset, int delete_count, int insert_count, DPoint const points[]);
HC_INTERFACE void HC_CDECL HC_DEdit_PolyCylinder_Radii (Key key, int offset, int delete_count, int insert_count, double const radii[]);
HC_INTERFACE void HC_CDECL HC_Edit_PolyCylinder (Key key, int p_count, Point const pts[], int r_count, float const radii[], char const *capping);
HC_INTERFACE void HC_CDECL HC_Edit_PolyCylinder_With_Basis (Key key, int p_count, Point const pts[], int r_count, float const radii[], char const *capping, Vector const basis[]);
HC_INTERFACE Key HC_CDECL HC_Copy_Geometry (Key key, char const *segment);
HC_INTERFACE Key HC_CDECL HC_Copy_Segment (char const *old_seg_name, char const *new_seg_name);
HC_INTERFACE Key HC_CDECL HC_Create_Segment (char const *segment);
HC_INTERFACE Key HC_CDECL HC_Include_Segment (char const *include);
HC_INTERFACE Key HC_CDECL HC_Include_Segment_By_Key (Key seg_key);
HC_INTERFACE Key HC_CDECL HC_Insert_Area_Light (int count, Point const points[], char const *listptr);
HC_INTERFACE Key HC_CDECL HC_Insert_Circle (Point const *point1, Point const *point2, Point const *point3);
HC_INTERFACE Key HC_CDECL HC_Insert_Circular_Arc (Point const *point1, Point const *point2, Point const *point3);
HC_INTERFACE Key HC_CDECL HC_Insert_Circular_Chord (Point const *point1, Point const *point2, Point const *point3);
HC_INTERFACE Key HC_CDECL HC_Insert_Circular_Wedge (Point const *point1, Point const *point2, Point const *point3);
HC_INTERFACE Key HC_CDECL HC_Insert_Cutting_Plane (double a, double b, double c, double d);
HC_INTERFACE Key HC_CDECL HC_Insert_Cylinder (Point const *center_point_1, Point const *center_point_2, double radius, char const *cap);
HC_INTERFACE Key HC_CDECL HC_Insert_Sphere (Point const *center, double radius, Vector const *axis, Vector const *ortho);
HC_INTERFACE Key HC_CDECL HC_Insert_Circle_By_Radius (Point const *center, double radius, Vector const *normal);
HC_INTERFACE Key HC_CDECL HC_Insert_Distant_Light (double di, double dj, double dk);
HC_INTERFACE Key HC_CDECL HC_Insert_Ellipse (Point const *center, Point const *major, Point const *minor);
HC_INTERFACE Key HC_CDECL HC_Insert_Elliptical_Arc (
		Point const *center,
		Point const *major,
		Point const *minor,
		double start,
		double end);
HC_INTERFACE Key HC_CDECL HC_Insert_Grid (
		char const *type,
		Point const *origin,
		Point const *ref1,
		Point const *ref2,
		int count1,
		int count2);
HC_INTERFACE Key HC_CDECL HC_Insert_Image (double x, double y, double z, char const *format, int width, int height, const void *data);
HC_INTERFACE Key HC_CDECL HC_Insert_Compressed_Image (double x, double y, double z, char const *format, int width, int height, int size, void const *data);
HC_INTERFACE Key HC_CDECL HC_Insert_Image_By_Ref (double x, double y, double z, char const *format, int width, int height, const void *data);
HC_INTERFACE Key HC_CDECL HC_DInsert_Image_By_Ref (double x, double y, double z, char const *format, int width, int height, const void *data);
HC_INTERFACE Key HC_CDECL HC_Insert_Ink (double x, double y, double z);
HC_INTERFACE Key HC_CDECL HC_Insert_Line (double xa, double ya, double za, double xb, double yb, double zb);
HC_INTERFACE Key HC_CDECL HC_Insert_Infinite_Line (double xa, double ya, double za, double xb, double yb, double zb);
HC_INTERFACE Key HC_CDECL HC_Insert_Infinite_Ray (double xa, double ya, double za, double xb, double yb, double zb);
HC_INTERFACE Key HC_CDECL HC_DInsert_Infinite_Line (double xa, double ya, double za, double xb, double yb, double zb);
HC_INTERFACE Key HC_CDECL HC_DInsert_Infinite_Ray (double xa, double ya, double za, double xb, double yb, double zb);
HC_INTERFACE Key HC_CDECL HC_Insert_Local_Light (double x, double y, double z);
HC_INTERFACE Key HC_CDECL HC_Insert_Marker (double x, double y, double z);
HC_INTERFACE Key HC_CDECL HC_Insert_Mesh (int rows, int columns, Point const points[]);
HC_INTERFACE Key HC_CDECL HC_Insert_Mesh_By_Ref (int rows, int columns, Point const points[]);
HC_INTERFACE Key HC_CDECL HC_DInsert_Mesh_By_Ref (int rows, int columns, DPoint const points[]);
HC_INTERFACE Key HC_CDECL HC_Insert_NURBS_Curve (int degree,
		 int cpcount,
		 Point const control_points[],
		 float const weights[],
		 float const knots[],
		 double start_u,
		 double end_u);
HC_INTERFACE Key HC_CDECL HC_Insert_Polygon (int count, Point const points[]);
HC_INTERFACE Key HC_CDECL HC_Insert_Polyline (int count, Point const points[]);
HC_INTERFACE Key HC_CDECL HC_Insert_Shell (int pcount, Point const points[], int flist_length, int const face_list[]);
HC_INTERFACE Key HC_CDECL HC_Insert_Shell_By_Ref (int pcount, Point const points[], int flist_length, int const face_list[]);
HC_INTERFACE Key HC_CDECL HC_DInsert_Shell_By_Ref (int pcount, DPoint const points[], int flist_length, int const face_list[]);
HC_INTERFACE Key HC_CDECL HC_Insert_Shell_By_Tristrips (
	int				pcount,
	Point const 	points[],
	int				tristrips_length,
	int const 		tristrips[],
	int				face_indices_length,
	int const 		face_indices[]);
HC_INTERFACE Key HC_CDECL HC_DInsert_Shell_By_Tristrips (
	int				pcount,
	DPoint const 	points[],
	int				tristrips_length,
	int const 		tristrips[],
	int				face_indices_length,
	int const 		face_indices[]);
HC_INTERFACE Key HC_CDECL HC_Insert_Spot_Light (Point const *position, Point const *target, char const *list);
HC_INTERFACE Key HC_CDECL HC_Insert_String_Cursor (Key text_key, int row, int column);
HC_INTERFACE Key HC_CDECL HC_Insert_Text (double x, double y, double z, char const *text);
HC_INTERFACE Key HC_CDECL HC_Insert_Text_With_Encoding (double x, double y, double z, char const *encoding, void const *text);
HC_INTERFACE Key HC_CDECL HC_Open_Segment (char const *segment);
HC_INTERFACE Key HC_CDECL HC_Renumber_Key (Key oldkey, Key newkey, char const *scope);
HC_INTERFACE Key HC_CDECL HC_Show_Include_Segment (Key key, char *pathname);
HC_INTERFACE Key HC_CDECL HC_Show_Owner_Original_Key (Key key);
HC_INTERFACE Key HC_CDECL HC_Style_Segment (char const *style);
HC_INTERFACE Key HC_CDECL HC_Style_Segment_By_Key (Key seg_key);
HC_INTERFACE void HC_CDECL HC_MSet_Character_Attributes (Key key, int offset, int count, char const *options);
HC_INTERFACE void HC_CDECL HC_MUnSet_Character_Attributes (Key key, int offset, int count, char const *options);
HC_INTERFACE void HC_CDECL HC_MSet_Face_Normals (Key key, int offset, int count, Vector const *normals);
HC_INTERFACE void HC_CDECL HC_MSet_Face_Colors_By_FIndex (Key key, char const *type, int offset, int count, float const findices[]);
HC_INTERFACE void HC_CDECL HC_MSet_Face_Colors_By_Value (
	Key				key,
	char const *	type,
	int				offset,
	char const *	color_space,
	int				count,
	RGB const 	values[]);
HC_INTERFACE void HC_CDECL HC_MSet_Vertex_Colors_By_FIndex (Key key, char const *types, int offset, int count, float const findices[]);
HC_INTERFACE void HC_CDECL HC_MSet_Vertex_Colors_By_Value (
	Key				key,
	char const *	types,
	int				offset,
	char const *	color_space,
	int				count,
	RGB const 		values[]);
HC_INTERFACE void HC_CDECL HC_MSet_Vertex_Colors_By_Value4 (
	Key				key,
	char const *	geometry,
	int				offset,
	char const *	color_space,
	int				count,
	RGBA const 	values[]);
HC_INTERFACE void HC_CDECL HC_MSet_Vertex_Normals (Key key, int offset, int count, Vector const normals[]);
HC_INTERFACE void HC_CDECL HC_MSet_Vertex_Parameters (Key key, int offset, int pcount, int number, float const parameters[]);
HC_INTERFACE void HC_CDECL HC_Modify_Color_Map (int offset, char const *colors);
HC_INTERFACE void HC_CDECL HC_Modify_Color_Map_By_Value (int offset, char const *color_space, int count, RGB const values[]);
HC_INTERFACE void HC_CDECL HC_Move_By_Key (Key key, char const *newowner);
HC_INTERFACE void HC_CDECL HC_Move_Distant_Light (Key key, double di, double dj, double dk);
HC_INTERFACE void HC_CDECL HC_Move_Image (Key key, double x, double y, double z);
HC_INTERFACE void HC_CDECL HC_Move_Light_Position (Key key, double x, double y, double z);
HC_INTERFACE void HC_CDECL HC_Move_Light_Target (Key key, double x, double y, double z);
HC_INTERFACE void HC_CDECL HC_Move_Segment (char const *old_seg_name, char const *new_seg_name);
HC_INTERFACE void HC_CDECL HC_Move_String_Cursor (Key key, int row, int column);
HC_INTERFACE void HC_CDECL HC_Move_Text (Key key, double x, double y, double z);
HC_INTERFACE void HC_CDECL HC_Open_Edge (int offset1, int offset2);
HC_INTERFACE void HC_CDECL HC_Open_Face (int id);
HC_INTERFACE void HC_CDECL HC_Open_Geometry (Key key);
HC_INTERFACE void HC_CDECL HC_Open_LOD (int level);
HC_INTERFACE void HC_CDECL HC_Open_Region (int region);
HC_INTERFACE void HC_CDECL HC_Open_Segment_By_Key (Key key);
HC_INTERFACE void HC_CDECL HC_Open_Vertex (int offset);
HC_INTERFACE void HC_CDECL HC_Orbit_Camera (double theta, double phi);
HC_INTERFACE bool HC_CDECL HC_PShow_Net_Callback (int key_count, Key const path_keys[], char *callbacks);
HC_INTERFACE int HC_CDECL HC_PShow_Net_Camera (int key_count,
		 Key const path_keys[],
		 Point *position,
		 Point *target,
		 Vector *up,
		 float *width,
		 float *height,
		 char *projection);
HC_INTERFACE int HC_CDECL HC_PShow_Net_Camera_By_Volume (int count,
		 Key const keys[],
		 char *proj,
		 float *xmin,
		 float *xmax,
		 float *ymin,
		 float *ymax);
HC_INTERFACE int HC_CDECL HC_PShow_Net_Camera_Position (int key_count, Key const path_keys[], float *x, float *y, float *z);
HC_INTERFACE int HC_CDECL HC_PShow_Net_Camera_Target (int key_count, Key const path_keys[], float *x, float *y, float *z);
HC_INTERFACE int HC_CDECL HC_PShow_Net_Camera_Up_Vector (int key_count, Key const path_keys[], float *x, float *y, float *z);
HC_INTERFACE int HC_CDECL HC_PShow_Net_Camera_Field (int key_count, Key const path_keys[], float *width, float *height);
HC_INTERFACE int HC_CDECL HC_PShow_Net_Camera_Projection (int key_count, Key const path_keys[], char *projection);

HC_INTERFACE int HC_CDECL HC_DPShow_Net_Camera (int key_count,
		 Key const path_keys[],
		 DPoint *position,
		 DPoint *target,
		 DVector *up,
		 double *width,
		 double *height,
		 char *projection);
HC_INTERFACE int HC_CDECL HC_DPShow_Net_Camera_By_Volume (int count,
		 Key const keys[],
		 char *proj,
		 double *xmin,
		 double *xmax,
		 double *ymin,
		 double *ymax);
HC_INTERFACE int HC_CDECL HC_DPShow_Net_Camera_Position (int key_count, Key const path_keys[], double *x, double *y, double *z);
HC_INTERFACE int HC_CDECL HC_DPShow_Net_Camera_Target (int key_count, Key const path_keys[], double *x, double *y, double *z);
HC_INTERFACE int HC_CDECL HC_DPShow_Net_Camera_Up_Vector (int key_count, Key const path_keys[], double *x, double *y, double *z);
HC_INTERFACE int HC_CDECL HC_DPShow_Net_Camera_Field (int key_count, Key const path_keys[], double *width, double *height);

HC_INTERFACE bool HC_CDECL HC_PShow_Net_Color (int count, Key const keys[], char *color_spec);
HC_INTERFACE bool HC_CDECL HC_PShow_Net_Color_Map (int key_count, Key const path_keys[], char *colors);
HC_INTERFACE bool HC_CDECL HC_PShow_Net_Color_Map_By_Value (int key_count, Key const path_keys[], char *color_space, int *count, RGB values[]);
HC_INTERFACE bool HC_CDECL HC_PShow_Net_Color_Map_Count (int key_count, Key const path_keys[], int *count);
HC_INTERFACE bool HC_CDECL HC_PShow_Net_Driver_Options (int key_count, Key const path_keys[], char *list);
HC_INTERFACE bool HC_CDECL HC_PShow_Net_Edge_Pattern (int key_count, Key const path_keys[], char *pattern);
HC_INTERFACE bool HC_CDECL HC_PShow_Net_Edge_Weight (int key_count, Key const path_keys[], float *weight);
HC_INTERFACE bool HC_CDECL HC_PShow_Net_Face_Pattern (int key_count, Key const path_keys[], char *pattern);
HC_INTERFACE bool HC_CDECL HC_PShow_Net_Handedness (int key_count, Key const path_keys[], char *value);
HC_INTERFACE bool HC_CDECL HC_PShow_Net_Heuristics (int key_count, Key const path_keys[], char *list);
HC_INTERFACE bool HC_CDECL HC_PShow_Net_Line_Pattern (int key_count, Key const path_keys[], char *pattern);
HC_INTERFACE bool HC_CDECL HC_PShow_Net_Line_Weight (int key_count, Key const path_keys[], float *weight);
HC_INTERFACE bool HC_CDECL HC_PShow_Net_Marker_Size (int key_count, Key const path_keys[], float *size);
HC_INTERFACE bool HC_CDECL HC_PShow_Net_Marker_Symbol (int key_count, Key const path_keys[], char *symbol);
HC_INTERFACE int HC_CDECL HC_PShow_Net_Modelling_Matrix (int key_count, Key const path_keys[], float matrix[]);
HC_INTERFACE int HC_CDECL HC_DPShow_Net_Modelling_Matrix (int key_count, Key const path_keys[], double matrix[]);
HC_INTERFACE bool HC_CDECL HC_PShow_Net_Rendering_Options (int key_count, Key const path_keys[], char *list);
HC_INTERFACE bool HC_CDECL HC_PShow_Net_Selectability (int key_count, Key const path_keys[], char *list);
HC_INTERFACE bool HC_CDECL HC_PShow_Net_Text_Alignment (int key_count, Key const path_keys[], char *locater);
HC_INTERFACE bool HC_CDECL HC_PShow_Net_Text_Font (int key_count, Key const path_keys[], char *options);
HC_INTERFACE bool HC_CDECL HC_PShow_Net_Text_Path (int key_count, Key const path_keys[], float *x, float *y, float *z);
HC_INTERFACE bool HC_CDECL HC_PShow_Net_Text_Spacing (int key_count, Key const path_keys[], float *spacing);
HC_INTERFACE bool HC_CDECL HC_PShow_Net_Texture_Matrix (int key_count, Key const path_keys[], float matrix[]);
HC_INTERFACE bool HC_CDECL HC_PShow_Net_User_Index_Count (int key_count, Key const keys[], int *count);
HC_INTERFACE bool HC_CDECL HC_PShow_Net_User_Indices (int key_count, Key const path_keys[], int *count, long *indices, void **values);
HC_INTERFACE bool HC_CDECL HC_PShow_Net_User_Options (int key_count, Key const path_keys[], char *list);
HC_INTERFACE bool HC_CDECL HC_PShow_Net_Unicode_Options (int key_count, Key const path_keys[], unsigned short *options);
HC_INTERFACE bool HC_CDECL HC_PShow_Net_User_Opt_Length (int key_count, Key const path_keys[], int *length); // HC_PShow_Net_User_Options_Length
HC_INTERFACE bool HC_CDECL HC_PShow_Net_Unicode_Opt_Length (int key_count, Key const path_keys[], int *length); // HC_PShow_Net_Unicode_Options_Length
HC_INTERFACE bool HC_CDECL HC_PShow_Net_User_Value (int key_count, Key const path_keys[], intptr_t *data);
HC_INTERFACE bool HC_CDECL HC_PShow_Net_Visibility (int key_count, Key const path_keys[], char *list);
HC_INTERFACE bool HC_CDECL HC_PShow_Net_Window (int key_count, Key const path_keys[], float *left, float *right, float *bottom, float *top);
HC_INTERFACE bool HC_CDECL HC_PShow_Net_Window_Frame (int key_count, Key const path_keys[], char *flag);
HC_INTERFACE bool HC_CDECL HC_PShow_Net_Window_Pattern (int key_count, Key const path_keys[], char *pattern);
HC_INTERFACE bool HC_CDECL HC_PShow_One_Net_Callback (int count, Key const keys[], char const *callback_point, char *callback_name);
HC_INTERFACE bool HC_CDECL HC_PShow_One_Net_Callback_WD (int count, Key const keys[], char const *callback_point, char *callback, void **data); // HC_PShow_One_Net_Callback_With_Data
HC_INTERFACE bool HC_CDECL HC_PShow_One_Net_Color (int count, Key const keys[], char const *type, char *color);
HC_INTERFACE bool HC_CDECL HC_PShow_One_Net_Color_By_Index (int count, Key const keys[], char const *type, int *index);
HC_INTERFACE bool HC_CDECL HC_PShow_One_Net_Color_By_Value (
	int				count,
	Key const 		keys[],
	char const *	type,
	char *			color_space,
	float *			a,
	float *			b,
	float *			c);
HC_INTERFACE bool HC_CDECL HC_PShow_One_Net_Color_Map (int count, Key const keys[], int offset, char *color);
HC_INTERFACE bool HC_CDECL HC_PShow_One_Net_Color_Map_By_V (
	int				count,
	Key const 		keys[],
	int				offset,
	char *			color_space,
	float *			x,
	float *			y,
	float *			z); // HC_PShow_One_Net_Color_Map_By_Value
HC_INTERFACE bool HC_CDECL HC_PShow_One_Net_Driver_Option (int count, Key const keys[], char const *type, char *value);
HC_INTERFACE bool HC_CDECL HC_PShow_One_Net_Heuristic (int count, Key const keys[], char const *type, char *value);
HC_INTERFACE bool HC_CDECL HC_PShow_One_Net_Rendering_Opti (int count, Key const keys[], char const *type, char *value); // HC_PShow_One_Net_Rendering_Option
HC_INTERFACE bool HC_CDECL HC_PShow_One_Net_Selectability (int count, Key const keys[], char const *type, char *value);
HC_INTERFACE bool HC_CDECL HC_PShow_One_Net_Text_Font (int count, Key const keys[], char const *type, char *value);
HC_INTERFACE bool HC_CDECL HC_PShow_One_Net_User_Index (int count, Key const keys[], long index, void **value);
HC_INTERFACE bool HC_CDECL HC_PShow_One_Net_User_Option (int count, Key const keys[], char const *type, char *value);
HC_INTERFACE bool HC_CDECL HC_PShow_One_Net_Unicode_Opt (int key_count, Key const keys[], unsigned short const *requested_option, unsigned short *options); // HC_PShow_One_Net_Unicode_Option
HC_INTERFACE bool HC_CDECL HC_PShow_One_Net_Visibility (int count, Key const keys[], char const *type, char *value);
HC_INTERFACE void HC_CDECL HC_Pan_Camera (double theta, double phi);
HC_INTERFACE bool HC_CDECL HC_Parse_String (char const *string, char const *delimiter, int offset, char *token);
HC_INTERFACE void HC_CDECL HC_Pause (void);
HC_INTERFACE void HC_CDECL HC_Print_Version (void);
HC_INTERFACE int HC_CDECL HC_QShow_Existence (char const *segment, char const *filter);
HC_INTERFACE int HC_CDECL HC_Show_Existence (char const *filter);
HC_INTERFACE int HC_CDECL HC_Show_Existence_By_Key (Key key, char const *filter);
HC_INTERFACE int HC_CDECL HC_QShow_Existence_Unicode (char const *segment, unsigned short const *filter);
HC_INTERFACE int HC_CDECL HC_Show_Existence_Unicode (unsigned short const *filter);
HC_INTERFACE int HC_CDECL HC_Show_Existence_Unicode_By_K (Key key, unsigned short const *filter); // HC_Show_Existence_Unicode_By_Key
HC_INTERFACE bool HC_CDECL HC_Read_Metafile (char const *file, char const *segment, char const *options);
HC_INTERFACE void HC_CDECL HC_Record_Instance_Handle (void const *handle);
HC_INTERFACE void HC_CDECL HC_Record_Profile_Source (char const *section, char const *filename);
HC_INTERFACE void HC_CDECL HC_Relinquish_Memory (void);
HC_INTERFACE void HC_CDECL HC_Rename_Segment (char const *old_seg_name, char const *new_seg_name);
HC_INTERFACE void HC_CDECL HC_Report_Error (int category,
		 int specific,
		 int severity,
		 int msgc,
		 char const * const *msgv,
		 int stack_c,
		 char const * const *stack_v);
HC_INTERFACE void HC_CDECL HC_Reset_System (void);
HC_INTERFACE void HC_CDECL HC_Restart_Ink (void);
HC_INTERFACE void HC_CDECL HC_Roll_Camera (double theta);
HC_INTERFACE void HC_CDECL HC_Rotate_Object (double theta, double phi, double psi);
HC_INTERFACE void HC_CDECL HC_Rotate_Object_Offaxis (double x, double y, double z, double theta);
HC_INTERFACE void HC_CDECL HC_DRotate_Object (double theta, double phi, double psi);
HC_INTERFACE void HC_CDECL HC_DRotate_Object_Offaxis (double x, double y, double z, double theta);
HC_INTERFACE void HC_CDECL HC_Rotate_Texture (double theta, double phi, double psi);
HC_INTERFACE void HC_CDECL HC_Rotate_Texture_Offaxis (double x, double y, double z, double theta);
HC_INTERFACE void HC_CDECL HC_Scale_Object (double x, double y, double z);
HC_INTERFACE void HC_CDECL HC_DScale_Object (double x, double y, double z);
HC_INTERFACE void HC_CDECL HC_Scale_Texture (double u, double v, double w);
HC_INTERFACE void HC_CDECL HC_Scroll_Text (Key key, int left_scroll, int up_scroll);
HC_INTERFACE void HC_CDECL HC_Set_Bounding_Cuboid (Point const *min, Point const *max);
HC_INTERFACE void HC_CDECL HC_Set_Bounding_Sphere (Point const *center, double radius);
HC_INTERFACE void HC_CDECL HC_DSet_Bounding_Cuboid (DPoint const *min, DPoint const *max);
HC_INTERFACE void HC_CDECL HC_DSet_Bounding_Sphere (DPoint const *center, double radius);
HC_INTERFACE void HC_CDECL HC_Set_Callback (char const *callbacks);
HC_INTERFACE void HC_CDECL HC_Set_Callback_With_Data (char const *callback, void const *data);
HC_INTERFACE void HC_CDECL HC_Set_Camera (Point const *position,
		 Point const *target,
		 Vector const *up,
		 double width,
		 double height,
		 char const *projection);
HC_INTERFACE void HC_CDECL HC_Set_Camera_By_Volume (char const *projection, double xmin, double xmax, double ymin, double ymax);
HC_INTERFACE void HC_CDECL HC_Set_Camera_Field (double width, double height);
HC_INTERFACE void HC_CDECL HC_Set_Camera_Position (double x, double y, double z);
HC_INTERFACE void HC_CDECL HC_Set_Camera_Projection (char const *type);
HC_INTERFACE void HC_CDECL HC_Set_Camera_Target (double x, double y, double z);
HC_INTERFACE void HC_CDECL HC_Set_Camera_Up_Vector (double x, double y, double z);
HC_INTERFACE void HC_CDECL HC_Set_Color (char const *color_spec);
HC_INTERFACE void HC_CDECL HC_Set_Explicit_Color (
	char const *type, 
	char const *channel, 
	Point const *rgb, 
	char const *texture, 
	char const *options);
HC_INTERFACE void HC_CDECL HC_Set_Color_By_FIndex (char const *types, double findex);
HC_INTERFACE void HC_CDECL HC_Set_Color_By_Index (char const *types, int index);
HC_INTERFACE void HC_CDECL HC_Set_Color_By_Value (char const *types, char const *colorspace, double a, double b, double c);
HC_INTERFACE void HC_CDECL HC_Set_Color_Map (char const *colors);
HC_INTERFACE void HC_CDECL HC_Set_Color_Map_By_Value (char const *color_space, int count, RGB const values[]);
HC_INTERFACE void HC_CDECL HC_Set_Driver_Options (char const *list);
HC_INTERFACE void HC_CDECL HC_Set_Edge_Pattern (char const *pattern);
HC_INTERFACE void HC_CDECL HC_Set_Edge_Pattern_Explicit (char const *pattern);
HC_INTERFACE void HC_CDECL HC_Set_Edge_Weight (double weight);
HC_INTERFACE void HC_CDECL HC_Set_Face_Pattern (char const *pattern);
HC_INTERFACE void HC_CDECL HC_Set_Handedness (char const *value);
HC_INTERFACE void HC_CDECL HC_Set_Heuristics (char const *list);
HC_INTERFACE void HC_CDECL HC_Set_Line_Pattern (char const *pattern);
HC_INTERFACE void HC_CDECL HC_Set_Line_Pattern_Explicit (char const *pattern);
HC_INTERFACE void HC_CDECL HC_Set_Line_Weight (double weight);
HC_INTERFACE void HC_CDECL HC_Set_Marker_Size (double weight);
HC_INTERFACE void HC_CDECL HC_Set_Marker_Symbol (char const *symbol);
HC_INTERFACE void HC_CDECL HC_Set_Modelling_Matrix (float const matrix[]);
HC_INTERFACE void HC_CDECL HC_DSet_Modelling_Matrix (double const matrix[]);
HC_INTERFACE void HC_CDECL HC_Set_Normal (double x, double y, double z);
HC_INTERFACE void HC_CDECL HC_Set_Parameter (int number, float const parameters[]);
HC_INTERFACE void HC_CDECL HC_Set_Priority (Key key, int priority);
HC_INTERFACE void HC_CDECL HC_Set_Rendering_Options (char const *list);
HC_INTERFACE void HC_CDECL HC_Set_Selectability (char const *list);
HC_INTERFACE void HC_CDECL HC_Set_Streaming_Mode (char const *flag);
HC_INTERFACE void HC_CDECL HC_Set_Text_Alignment (char const *locater);
HC_INTERFACE void HC_CDECL HC_Set_Text_Font (char const *list);
HC_INTERFACE void HC_CDECL HC_Set_Text_Path (double x, double y, double z);
HC_INTERFACE void HC_CDECL HC_Set_Text_Size (double ivalue);
HC_INTERFACE void HC_CDECL HC_Set_Text_Spacing (double spacing);
HC_INTERFACE void HC_CDECL HC_Set_Texture_Matrix (float const matrix[]);
HC_INTERFACE void HC_CDECL HC_Set_User_Index (long index, void const *data);
HC_INTERFACE void HC_CDECL HC_Set_User_Options (char const *list);
HC_INTERFACE void HC_CDECL HC_Set_Unicode_Options (unsigned short const *options);
HC_INTERFACE void HC_CDECL HC_Set_User_Value (intptr_t data);
HC_INTERFACE void HC_CDECL HC_Set_Visibility (char const *list);
HC_INTERFACE void HC_CDECL HC_Set_Window (double left, double right, double bottom, double top);
HC_INTERFACE void HC_CDECL HC_Set_Window_Frame (char const *flag);
HC_INTERFACE void HC_CDECL HC_Set_Window_Pattern (char const *pattern);
HC_INTERFACE void HC_CDECL HC_Show_Alias (char const *alias, char *expansion);
HC_INTERFACE void HC_CDECL HC_Show_Alias_Count (int *count);
HC_INTERFACE void HC_CDECL HC_Show_Area_Light (Key key, int *ucount, Point upoints[], char *listptr);
HC_INTERFACE void HC_CDECL HC_Show_Area_Light_Count (Key key, int *count, char *listptr);
HC_INTERFACE bool HC_CDECL HC_Show_Bounding_Cuboid (Point *min, Point *max);
HC_INTERFACE bool HC_CDECL HC_Show_Bounding_Cuboid_By_Key (Key key, Point *min, Point *max);
HC_INTERFACE bool HC_CDECL HC_DShow_Bounding_Cuboid (DPoint *min, DPoint *max);
HC_INTERFACE bool HC_CDECL HC_DShow_Bounding_Cuboid_By_Key (Key key, DPoint *min, DPoint *max);
HC_INTERFACE void HC_CDECL HC_Show_Bounding_Info (char *list);
HC_INTERFACE void HC_CDECL HC_Show_Bounding_Info_By_Key (Key key, char *list);
HC_INTERFACE bool HC_CDECL HC_Show_Bounding_Sphere (Point *center, float *radius);
HC_INTERFACE bool HC_CDECL HC_Show_Bounding_Sphere_By_Key (Key key, Point *center, float *radius);
HC_INTERFACE bool HC_CDECL HC_DShow_Bounding_Sphere (DPoint *center, double *radius);
HC_INTERFACE bool HC_CDECL HC_DShow_Bounding_Sphere_By_Key (Key key, DPoint *center, double *radius);
HC_INTERFACE void HC_CDECL HC_Show_Button (char *button);
HC_INTERFACE void HC_CDECL HC_Show_Button_Source (char *keyboard, char *button, int *status);
HC_INTERFACE void HC_CDECL HC_Show_Callback (char *callbacks);
HC_INTERFACE void HC_CDECL HC_Show_Callback_Name (char const *name, Void_Routine *callback);
HC_INTERFACE void HC_CDECL HC_Show_Callback_Name_Count (int *count);
HC_INTERFACE void HC_CDECL HC_Show_Camera (Point *position,
		 Point *	target,
		 Vector *	up,
		 float *	width,
		 float *	height,
		 char *		projection);
HC_INTERFACE void HC_CDECL HC_Show_Camera_By_Volume (char *projection, float *xmin, float *xmax, float *ymin, float *ymax);
HC_INTERFACE void HC_CDECL HC_Show_Camera_Field (float *width, float *height);
HC_INTERFACE void HC_CDECL HC_Show_Camera_Position (float *x, float *y, float *z);
HC_INTERFACE void HC_CDECL HC_Show_Camera_Projection (char *projection);
HC_INTERFACE void HC_CDECL HC_Show_Camera_Target (float *x, float *y, float *z);
HC_INTERFACE void HC_CDECL HC_Show_Camera_Up_Vector (float *x, float *y, float *z);
HC_INTERFACE void HC_CDECL HC_Show_Circle (Key key, Point *point1, Point *point2, Point *point3);
HC_INTERFACE void HC_CDECL HC_Show_Circular_Arc (Key key, Point *point1, Point *point2, Point *point3);
HC_INTERFACE void HC_CDECL HC_Show_Circular_Chord (Key key, Point *point1, Point *point2, Point *point3);
HC_INTERFACE void HC_CDECL HC_Show_Circular_Wedge (Key key, Point *point1, Point *point2, Point *point3);
HC_INTERFACE void HC_CDECL HC_Show_Color (char *color_spec);
HC_INTERFACE void HC_CDECL HC_Show_Color_By_Index (char *types, int *index);
HC_INTERFACE void HC_CDECL HC_Show_Color_By_Value (char *types, char *colorspace, float *a, float *b, float *c);
HC_INTERFACE void HC_CDECL HC_Show_Color_Map (char *colors);
HC_INTERFACE void HC_CDECL HC_Show_Color_Map_By_Value (char *color_space, int *count, RGB values[]);
HC_INTERFACE void HC_CDECL HC_Show_Color_Map_Count (int *count);
HC_INTERFACE void HC_CDECL HC_Show_Color_Name (char const *color, char *definition);
HC_INTERFACE void HC_CDECL HC_Show_Color_Name_Count (int *count);
HC_INTERFACE void HC_CDECL HC_Show_Explicit_Color (
	char const *type,
	char const *channel,
	float rgb[],
	char *texture,
	char *options);
HC_INTERFACE bool HC_CDECL HC_PShow_Net_Explicit_Color (
	int keycount,
	Key const pathkeys[],
	char const *type,
	char const *channel,
	float rgb[],
	char *texture,
	char *options);
HC_INTERFACE void HC_CDECL HC_Show_Contents_Count (int *count);
HC_INTERFACE void HC_CDECL HC_Show_Cutting_Plane (Key key, float *a, float *b, float *c, float *d);
HC_INTERFACE void HC_CDECL HC_Show_Cylinder (Key key, Point *p1, Point *p2, float *radius, char *cap);
HC_INTERFACE void HC_CDECL HC_Show_Circle_By_Radius (Key key, Point *center, float *radius, Vector *normal);
HC_INTERFACE void HC_CDECL HC_Show_Sphere (Key key, Point *center, float *radius, Vector *axis, Vector *ortho);
HC_INTERFACE void HC_CDECL HC_Show_Device_Info (char const *driver, char const *item, char *data);
HC_INTERFACE void HC_CDECL HC_Show_Device_Info_By_Key (Key key, char const *item, char *data);
HC_INTERFACE void HC_CDECL HC_Show_Distant_Light (Key key, float *di, float *dj, float *dk);
HC_INTERFACE void HC_CDECL HC_Show_Driver_Options (char *list);
HC_INTERFACE void HC_CDECL HC_Show_Edge_Pattern (char *pattern);
HC_INTERFACE void HC_CDECL HC_Show_Edge_Pattern_Explicit (char *pattern);
HC_INTERFACE void HC_CDECL HC_Show_Edge_Weight (float *weight);
HC_INTERFACE void HC_CDECL HC_Show_Ellipse (Key key, Point *center, Point *major, Point *minor);
HC_INTERFACE void HC_CDECL HC_Show_Elliptical_Arc (Key key,
		 Point *center,
		 Point *major,
		 Point *minor,
		 float *start,
		 float *end);
HC_INTERFACE bool HC_CDECL HC_Show_Environment (char const *variable, char *value);
HC_INTERFACE void HC_CDECL HC_Show_Face_Pattern (char *pattern);
HC_INTERFACE void HC_CDECL HC_Show_Font (char const *name, char *options, int *data_length, char *data);
HC_INTERFACE void HC_CDECL HC_Show_Font_Count (int *count);
HC_INTERFACE void HC_CDECL HC_Show_Font_Info (char const *driver, char const *name, char const *item, char *data);
HC_INTERFACE void HC_CDECL HC_Show_Font_Size (char const *name, char *options, int *data_length);
HC_INTERFACE void HC_CDECL HC_Show_Grid (Key key,
		 char *type,
		 Point *origin,
		 Point *ref1,
		 Point *ref2,
		 int *count1,
		 int *count2);
HC_INTERFACE void HC_CDECL HC_Show_Handedness (char *value);
HC_INTERFACE void HC_CDECL HC_Show_Heuristics (char *list);
HC_INTERFACE void HC_CDECL HC_Show_Image (Key key, float *x, float *y, float *z, char *format, int *width, int *height, void *data);
HC_INTERFACE void HC_CDECL HC_Show_Image_Size (Key key, float *x, float *y, float *z, char *format, int *width, int *height);
HC_INTERFACE void HC_CDECL HC_Show_Compressed_Image (Key key, float *x, float *y, float *z, char *format, int *width, int *height, int *size, void *data);
HC_INTERFACE void HC_CDECL HC_Show_Compressed_Image_Size (Key key, char *format, int *width, int *height, int *size);
HC_INTERFACE int HC_CDECL HC_Show_Image_Bytes_Per_Pixel (Key key);
HC_INTERFACE void HC_CDECL HC_Show_Image_Format (Key key, char *format);
HC_INTERFACE void HC_CDECL HC_Show_Image_Name (Key key, char *name);
HC_INTERFACE void HC_CDECL HC_Show_Glyph (char const *name, char *data);
HC_INTERFACE void HC_CDECL HC_Show_Glyph_Size (char const *name, int *data_size);
HC_INTERFACE void HC_CDECL HC_Show_Line_Style (char const *style, char *definition);
HC_INTERFACE void HC_CDECL HC_Show_Line_Style_Size (char const *style, int *size);
HC_INTERFACE void HC_CDECL HC_Show_Line_Pattern (char *pattern);
HC_INTERFACE void HC_CDECL HC_Show_Line_Pattern_Explicit (char *pattern);
HC_INTERFACE void HC_CDECL HC_Show_Key_Status (Key key, char *status);
HC_INTERFACE void HC_CDECL HC_Show_Key_Type (Key key, char *type);
HC_INTERFACE void HC_CDECL HC_Show_LOD_Type (Key key, int level, char *type);
HC_INTERFACE void HC_CDECL HC_Show_Line (Key key, float *xa, float *ya, float *za, float *xb, float *yb, float *zb);
HC_INTERFACE void HC_CDECL HC_Show_Infinite_Line (Key key, float *xa, float *ya, float *za, float *xb, float *yb, float *zb);
HC_INTERFACE void HC_CDECL HC_Show_Infinite_Ray (Key key, float *xa, float *ya, float *za, float *xb, float *yb, float *zb);
HC_INTERFACE void HC_CDECL HC_DShow_Infinite_Line (Key key, double *xa, double *ya, double *za, double *xb, double *yb, double *zb);
HC_INTERFACE void HC_CDECL HC_DShow_Infinite_Ray (Key key, double *xa, double *ya, double *za, double *xb, double *yb, double *zb);
HC_INTERFACE void HC_CDECL HC_Show_Line_Weight (float *weight);
HC_INTERFACE void HC_CDECL HC_Show_Local_Light (Key key, float *x, float *y, float *z);
HC_INTERFACE void HC_CDECL HC_Show_Location (float *x, float *y);
HC_INTERFACE void HC_CDECL HC_Show_Location_Source (char *locater, char *display, char *action, int *status);
HC_INTERFACE void HC_CDECL HC_Show_Marker (Key key, float *x, float *y, float *z);
HC_INTERFACE void HC_CDECL HC_Show_Marker_Size (float *size);
HC_INTERFACE void HC_CDECL HC_Show_Marker_Symbol (char *symbol);
HC_INTERFACE void HC_CDECL HC_Show_Memory_Usage (long *allocated, long *in_use);
HC_INTERFACE void HC_CDECL HC_Show_Mesh (Key key, int *rows, int *columns, Point points[]);
HC_INTERFACE void HC_CDECL HC_Show_Mesh_Size (Key key, int *rows, int *columns);
HC_INTERFACE void HC_CDECL HC_Show_Modelling_Matrix (float matrix[]);
HC_INTERFACE void HC_CDECL HC_DShow_Modelling_Matrix (double matrix[]);
HC_INTERFACE void HC_CDECL HC_Show_NURBS_Curve (Key key,
		 int *degree,
		 int *cp_count,
		 Point points[],
		 float weights[],
		 float knots[],
		 float *start_u,
		 float *end_u);
HC_INTERFACE void HC_CDECL HC_Show_NURBS_Curve_Size (Key key, int *degree, int *cp_count, int *weight_count, int *knot_count);
HC_INTERFACE void HC_CDECL HC_Show_Normal (float *x, float *y, float *z);
HC_INTERFACE void HC_CDECL HC_Show_One_Callback (char const *callback_point, char *callback_name);
HC_INTERFACE void HC_CDECL HC_Show_One_Callback_With_Data (char const *callback_point, char *callback, void **data);
HC_INTERFACE void HC_CDECL HC_Show_One_Color (char const *type, char *color);
HC_INTERFACE void HC_CDECL HC_Show_One_Color_By_Index (char const *type, int *index);
HC_INTERFACE bool HC_CDECL HC_Show_One_Color_By_Value (char const *type, char *color_space, float *a, float *b, float *c);
HC_INTERFACE void HC_CDECL HC_Show_One_Color_Map (int offset, char *color);
HC_INTERFACE void HC_CDECL HC_Show_One_Color_Map_By_Value (int offset, char *color_space, float *a, float *b, float *c);
HC_INTERFACE void HC_CDECL HC_Show_One_Driver_Option (char const *type, char *value);
HC_INTERFACE void HC_CDECL HC_Show_One_Heuristic (char const *type, char *value);
HC_INTERFACE void HC_CDECL HC_Show_One_Rendering_Option (char const *type, char *value);
HC_INTERFACE void HC_CDECL HC_Show_One_Selectability (char const *type, char *value);
HC_INTERFACE void HC_CDECL HC_Show_One_System_Option (char const *type, char *value);
HC_INTERFACE void HC_CDECL HC_Show_One_Text_Font (char const *type, char *value);
HC_INTERFACE bool HC_CDECL HC_Show_One_User_Index (long index, void **value);
HC_INTERFACE void HC_CDECL HC_Show_One_User_Option (char const *type, char *value);
HC_INTERFACE void HC_CDECL HC_Show_One_Unicode_Option (unsigned short const *requestedOption, unsigned short *options);
HC_INTERFACE void HC_CDECL HC_Show_One_Visibility (char const *type, char *value);
HC_INTERFACE void HC_CDECL HC_Show_Open_Segment_Count (int *count);
HC_INTERFACE Key HC_CDECL HC_Show_Owner (char const *segment, char *owner);
HC_INTERFACE Key HC_CDECL HC_Show_Owner_By_Key (Key key, char *owner);
HC_INTERFACE void HC_CDECL HC_Show_Parameter (int *size, float list[]);
HC_INTERFACE void HC_CDECL HC_Show_Parameter_Size (int *size);
HC_INTERFACE void HC_CDECL HC_Show_Partial_Image (Key key, int xpos, int ypos, int xlen, int ylen, void *data);
HC_INTERFACE void HC_CDECL HC_Show_Partial_Mesh (Key key,
		 int row_offset,
		 int col_offset,
		 int row_count,
		 int col_count,
		 Point points[]);
HC_INTERFACE void HC_CDECL HC_Show_Partial_Polygon (Key key, int offset, int request, Point points[]);
HC_INTERFACE void HC_CDECL HC_Show_Partial_Polyline (Key key, int offset, int request, Point points[]);
HC_INTERFACE void HC_CDECL HC_Show_Partial_Shell (Key key,
		 int voffset,
		 int vcnt,
		 Point points[],
		 int foffset,
		 int fcnt,
		 int *flist_length,
		 int face_list[]);
HC_INTERFACE void HC_CDECL HC_Show_Partial_Shell_Size (Key key, int foffset, int fcnt, int *flist_length);
HC_INTERFACE void HC_CDECL HC_Show_Pathname_Expansion (char const *pathname, char *expansion);
HC_INTERFACE void HC_CDECL HC_Show_Polygon (Key key, int *count, Point points[]);
HC_INTERFACE void HC_CDECL HC_Show_Polygon_Count (Key key, int *count);
HC_INTERFACE void HC_CDECL HC_Show_Polyline (Key key, int *count, Point points[]);
HC_INTERFACE void HC_CDECL HC_Show_Polyline_Count (Key key, int *count);
HC_INTERFACE bool HC_CDECL HC_Show_Priority (Key key, int *priority);
HC_INTERFACE void HC_CDECL HC_Show_Rendering_Options (char *list);
HC_INTERFACE int HC_CDECL HC_Show_Segment_Name_Length (Key key);
HC_INTERFACE Key HC_CDECL HC_Show_Segment_Name (Key key, char *name);
HC_INTERFACE Key HC_CDECL HC_Show_Segment (Key key, char *pathname);
HC_INTERFACE void HC_CDECL HC_Show_Segment_Count (int *count);
HC_INTERFACE void HC_CDECL HC_Show_Selectability (char *list);
HC_INTERFACE void HC_CDECL HC_Show_Selection (char *segment);
HC_INTERFACE void HC_CDECL HC_Show_Selection_Element (Key *key, int *offset1, int *offset2, int *offset3);
HC_INTERFACE void HC_CDECL HC_Show_Selection_Source_Elem (int *vertex, int *edge, int *face, Point *hit_location); // HC_Show_Selection_Source_Element
HC_INTERFACE void HC_CDECL HC_DShow_Selection_Source_Elem (int *vertex, int *edge, int *face, DPoint *hit_location); // HC_DShow_Selection_Source_Element
HC_INTERFACE void HC_CDECL HC_Show_Selection_Original_Key (Key *key);
HC_INTERFACE void HC_CDECL HC_Show_Selection_Elements (Key *key, int *count, int vertex1[], int vertex2[], int faces[]);
HC_INTERFACE void HC_CDECL HC_Show_Selection_Elements_Coun (Key *key, int *count); // HC_Show_Selection_Elements_Count
HC_INTERFACE void HC_CDECL HC_Show_Selection_Item (Key *key, int *offset1, int *offset2);
HC_INTERFACE void HC_CDECL HC_Show_Selection_Keys (int *count, Key keys[]);
HC_INTERFACE void HC_CDECL HC_Show_Selection_Original_Keys (int *count, Key key[]);
HC_INTERFACE void HC_CDECL HC_Show_Selection_Keys_Count (int *count);
HC_INTERFACE void HC_CDECL HC_Show_Selection_Location (float *xw, float *yw, float *xc, float *yc, float *zc);
HC_INTERFACE void HC_CDECL HC_Show_Selection_Pathname (char *segment);
HC_INTERFACE int HC_CDECL HC_Show_Selection_Position (float *window_x, float *window_y, float *window_z, float *camera_x, float *camera_y, float *camera_z);
HC_INTERFACE int HC_CDECL HC_DShow_Selection_Position (float *window_x, float *window_y, float *window_z, double *camera_x, double *camera_y, double *camera_z);
HC_INTERFACE void HC_CDECL HC_Show_Selection_Source (char *locater, char *picture, char *action, int *status);
HC_INTERFACE void HC_CDECL HC_Show_Shell (Key key, int *pcount, Point points[], int *flist_length, int face_list[]);
HC_INTERFACE void HC_CDECL HC_Show_Shell_By_Tristrips (Key key,
		 int *pcount,
		 Point points[],
		 int *tristrips_length,
		 int tristrips[],
		 int *face_indices_length,
		 int face_indices[]);
HC_INTERFACE void HC_CDECL HC_Show_Shell_By_Tristrips_Size (Key key, int *pcount, int *tristrips_length, int *face_indices_length);
HC_INTERFACE void HC_CDECL HC_Show_Shell_Size (Key key, int *pcount, int *flist_length);
HC_INTERFACE void HC_CDECL HC_Show_Shell_Face_Count (Key key, int *face_count);
HC_INTERFACE void HC_CDECL HC_Show_Shell_Tristrip_Count (Key key, int *tristrip_count);
HC_INTERFACE void HC_CDECL HC_Show_Spot_Light (Key key, Point *position, Point *target, char *list);
HC_INTERFACE void HC_CDECL HC_Show_Streaming_Mode (char *flag);
HC_INTERFACE void HC_CDECL HC_Show_String (char *text);
HC_INTERFACE void HC_CDECL HC_Show_String_Count (int *count);
HC_INTERFACE void HC_CDECL HC_Show_String_Cursor (Key key, Key *text_key, int *row, int *col);
HC_INTERFACE void HC_CDECL HC_Show_String_Length (int *length);
HC_INTERFACE void HC_CDECL HC_Show_String_Source (char *keyboard, Key *cursor_key);
HC_INTERFACE void HC_CDECL HC_Show_String_With_Encoding (char *encoding, void *text);
HC_INTERFACE Key HC_CDECL HC_Show_Style_Segment (Key key, char *pathname);
HC_INTERFACE bool HC_CDECL HC_Show_System_Info (char const *type, char *value);
HC_INTERFACE void HC_CDECL HC_Show_System_Options (char *list);
HC_INTERFACE void HC_CDECL HC_Show_Text (Key key, float *x, float *y, float *z, char *text);
HC_INTERFACE void HC_CDECL HC_Show_Text_With_Encoding (Key key, float *x, float *y, float *z, char *encoding, void *text);
HC_INTERFACE void HC_CDECL HC_Show_Text_Alignment (char *locater);
HC_INTERFACE void HC_CDECL HC_Show_Text_Count (Key key, int *count);
HC_INTERFACE void HC_CDECL HC_Show_Text_Encoding (Key key, char *encoding);
HC_INTERFACE void HC_CDECL HC_Show_Text_Font (char *options);
HC_INTERFACE void HC_CDECL HC_Show_Text_Length (Key key, int *length);
HC_INTERFACE void HC_CDECL HC_Show_Text_Path (float *x, float *y, float *z);
HC_INTERFACE void HC_CDECL HC_Show_Text_Size (float *value);
HC_INTERFACE void HC_CDECL HC_Show_Text_Spacing (float *spacing);
HC_INTERFACE void HC_CDECL HC_Show_Texture (char const *texture, char *definition);
HC_INTERFACE void HC_CDECL HC_Show_Shader (char const *name, char *definition, char *shader_source);
HC_INTERFACE void HC_CDECL HC_Show_Local_Texture (char const *texture_name, char *definition);
HC_INTERFACE void HC_CDECL HC_Show_Texture_Count (int *count);
HC_INTERFACE void HC_CDECL HC_Show_Glyph_Count (int *count);
HC_INTERFACE void HC_CDECL HC_Show_Line_Style_Count (int *count);
HC_INTERFACE void HC_CDECL HC_Show_Texture_Matrix (float matrix[]);
HC_INTERFACE void HC_CDECL HC_Show_Time (float *time);
HC_INTERFACE void HC_CDECL HC_Show_User_Index_Count (int *count);
HC_INTERFACE void HC_CDECL HC_Show_User_Indices (int count[], long indices[], void **values);
HC_INTERFACE void HC_CDECL HC_Show_User_Options (char *list);
HC_INTERFACE void HC_CDECL HC_Show_Unicode_Options (unsigned short *options);
HC_INTERFACE void HC_CDECL HC_Show_User_Options_Length (int *length);
HC_INTERFACE void HC_CDECL HC_Show_Unicode_Options_Length (int *length);
HC_INTERFACE void HC_CDECL HC_Show_User_Value (intptr_t *data);
HC_INTERFACE void HC_CDECL HC_Show_Visibility (char *list);
HC_INTERFACE void HC_CDECL HC_Show_Wakeup (float *time);
HC_INTERFACE void HC_CDECL HC_Show_Window (float *left, float *right, float *bottom, float *top);
HC_INTERFACE void HC_CDECL HC_Show_Window_Frame (char *flag);
HC_INTERFACE void HC_CDECL HC_Show_Window_Pattern (char *pattern);
HC_INTERFACE void HC_CDECL HC_Translate_Object (double x, double y, double z);
HC_INTERFACE void HC_CDECL HC_DTranslate_Object (double x, double y, double z);
HC_INTERFACE void HC_CDECL HC_Translate_Texture (double u, double v, double w);
HC_INTERFACE void HC_CDECL HC_UnDefine_Alias (char const *name);
HC_INTERFACE void HC_CDECL HC_UnDefine_Callback_Name (char const *name);
HC_INTERFACE void HC_CDECL HC_UnDefine_Color_Name (char const *name);
HC_INTERFACE void HC_CDECL HC_UnDefine_Error_Handler (Void_Routine handler);
HC_INTERFACE void HC_CDECL HC_UnDefine_Exit_Handler (Void_Routine handler);
HC_INTERFACE void HC_CDECL HC_UnDefine_Font (char const *name);
HC_INTERFACE void HC_CDECL HC_UnDefine_Glyph (char const *name);
HC_INTERFACE void HC_CDECL HC_UnDefine_Line_Style (char const *name);
HC_INTERFACE void HC_CDECL HC_UnDefine_Texture (char const *texture_name);
HC_INTERFACE void HC_CDECL HC_UnDefine_Local_Texture (char const *texture_name);
HC_INTERFACE void HC_CDECL HC_UnDefine_Shader (char const *iname);
HC_INTERFACE void HC_CDECL HC_UnSet_Bounding_Volume (void);
HC_INTERFACE void HC_CDECL HC_UnSet_Callback (void);
HC_INTERFACE void HC_CDECL HC_UnSet_Camera (void);
HC_INTERFACE void HC_CDECL HC_UnSet_Color (void);
HC_INTERFACE void HC_CDECL HC_UnSet_Color_Map (void);
HC_INTERFACE void HC_CDECL HC_UnSet_Driver_Options (void);
HC_INTERFACE void HC_CDECL HC_UnSet_Edge_Pattern (void);
HC_INTERFACE void HC_CDECL HC_UnSet_Edge_Weight (void);
HC_INTERFACE void HC_CDECL HC_UnSet_Face_Pattern (void);
HC_INTERFACE void HC_CDECL HC_UnSet_Handedness (void);
HC_INTERFACE void HC_CDECL HC_UnSet_Heuristics (void);
HC_INTERFACE void HC_CDECL HC_UnSet_Line_Pattern (void);
HC_INTERFACE void HC_CDECL HC_UnSet_Line_Weight (void);
HC_INTERFACE void HC_CDECL HC_UnSet_Marker_Size (void);
HC_INTERFACE void HC_CDECL HC_UnSet_Marker_Symbol (void);
HC_INTERFACE void HC_CDECL HC_UnSet_Modelling_Matrix (void);
HC_INTERFACE void HC_CDECL HC_UnSet_Normal (void);
HC_INTERFACE void HC_CDECL HC_UnSet_One_Callback (char const *callback);
HC_INTERFACE void HC_CDECL HC_UnSet_One_Color (char const *which);
HC_INTERFACE void HC_CDECL HC_UnSet_One_Driver_Option (char const *which);
HC_INTERFACE void HC_CDECL HC_UnSet_One_Heuristic (char const *which);
HC_INTERFACE void HC_CDECL HC_UnSet_One_Rendering_Option (char const *which);
HC_INTERFACE void HC_CDECL HC_UnSet_One_Selectability (char const *which);
HC_INTERFACE void HC_CDECL HC_UnSet_One_Text_Font (char const *which);
HC_INTERFACE void HC_CDECL HC_UnSet_One_User_Index (long index);
HC_INTERFACE void HC_CDECL HC_UnSet_One_User_Option (char const *which);
HC_INTERFACE void HC_CDECL HC_UnSet_One_Unicode_Option (unsigned short const *which);
HC_INTERFACE void HC_CDECL HC_UnSet_One_Visibility (char const *which);
HC_INTERFACE void HC_CDECL HC_UnSet_Parameter (void);
HC_INTERFACE void HC_CDECL HC_UnSet_Rendering_Options (void);
HC_INTERFACE void HC_CDECL HC_UnSet_Selectability (void);
HC_INTERFACE void HC_CDECL HC_UnSet_Streaming_Mode (void);
HC_INTERFACE void HC_CDECL HC_UnSet_Text_Alignment (void);
HC_INTERFACE void HC_CDECL HC_UnSet_Text_Font (void);
HC_INTERFACE void HC_CDECL HC_UnSet_Text_Path (void);
HC_INTERFACE void HC_CDECL HC_UnSet_Text_Spacing (void);
HC_INTERFACE void HC_CDECL HC_UnSet_Texture_Matrix (void);
HC_INTERFACE void HC_CDECL HC_UnSet_User_Options (void);
HC_INTERFACE void HC_CDECL HC_UnSet_Unicode_Options (void);
HC_INTERFACE void HC_CDECL HC_UnSet_User_Indices (void);
HC_INTERFACE void HC_CDECL HC_UnSet_User_Data (void);
HC_INTERFACE void HC_CDECL HC_UnSet_User_Value (void);
HC_INTERFACE void HC_CDECL HC_UnSet_Visibility (void);
HC_INTERFACE void HC_CDECL HC_UnSet_Window (void);
HC_INTERFACE void HC_CDECL HC_UnSet_Window_Frame (void);
HC_INTERFACE void HC_CDECL HC_UnSet_Window_Pattern (void);
HC_INTERFACE bool HC_CDECL HC_Update_Display (void);
HC_INTERFACE bool HC_CDECL HC_Update_Display_Timed (double time);
HC_INTERFACE bool HC_CDECL HC_Update_One_Display (char const *segment);
HC_INTERFACE bool HC_CDECL HC_Update_One_Display_Timed (char const *segment, double time);
HC_INTERFACE bool HC_CDECL HC_Write_Metafile (char const *segment, char const *file, char const *options);
HC_INTERFACE void HC_CDECL HC_Zoom_Camera (double zoom);
HC_INTERFACE void HC_CDECL HC_Show_Geometry_Pointer (Key key, const char * data_type, void * value);
HC_INTERFACE void HC_CDECL HC_Show_Geometry_Options (Key key, char *list);
HC_INTERFACE void HC_CDECL HC_Set_Geometry_Options (Key key, const char *list); // HC_Edit_Geometry_Options
HC_INTERFACE Key HC_CDECL HC_Insert_NURBS_Surface (
		 int u_degree,
		 int v_degree,
		 int u_count,
		 int v_count,
		 Point const points[], 
		 float const weights[],
		 float const u_knots[],
		 float const v_knots[]);
HC_INTERFACE void HC_CDECL HC_Show_NURBS_Surface_Size (
		 Key key, 
		 int *u_degree,
		 int *v_degree,
		 int *u_count,
		 int *v_count,
		 int *weights_count,
		 int *u_knots_count,
		 int *v_knots_count);
HC_INTERFACE void HC_CDECL HC_Show_NURBS_Surface (
		 Key key,
		 int *u_degree,
		 int *v_degree,
		 int *u_count,
		 int *v_count,
		 Point points[], 
		 float weights[],
		 float u_knots[],
		 float v_knots[]);
HC_INTERFACE void HC_CDECL HC_Trim_NURBS_Surface_By_Poly (
		 int cpcount,
		 float const points[]);
HC_INTERFACE void HC_CDECL HC_Trim_NURBS_Surface_By_Curve (
		 int			degree,
		 int			cpcount,
		 float const 	control_points[], 
		 float const 	weights[],
		 float const 	knots[],
		 double start_u,
		 double end_u);
HC_INTERFACE void HC_CDECL HC_Edit_NURBS_Surface (
		 Key key,
		 int cp_offset,
		 int weights_offset,
		 int u_knot_offset,
		 int v_knot_offset,
		 int cp_replace_count,
		 int weights_replace_count,
		 int u_knot_replace_count,
		 int v_knot_replace_count,
		 Point const points[],
		 float const weights[],
		 float const u_knots[],
		 float const v_knots[]);
HC_INTERFACE void HC_CDECL HC_Edit_NURBS_Surface_Points (
		 Key key,
		 int cp_offset,
		 int cp_replace_count,
		 Point const points[]);
HC_INTERFACE void HC_CDECL HC_Edit_NURBS_Surface_Weights (
		 Key key,
		 int weights_offset,
		 int weights_replace_count,
		 float const weights[]);
HC_INTERFACE void HC_CDECL HC_Edit_NURBS_Surface_Knots (
		 Key key,
		 int u_knot_offset,
		 int v_knot_offset,
		 int u_knot_replace_count,
		 int v_knot_replace_count,
		 float const u_knots[],
		 float const v_knots[]);
HC_INTERFACE void HC_CDECL HC_Show_Trim_Count (
		int *	count);
HC_INTERFACE void HC_CDECL HC_Show_Trim_Type (
		int						index,
		char *	trim_type);
HC_INTERFACE void HC_CDECL HC_Show_Trim_Poly (
		int						index,	   
		int *	count,
		float					points[]);
HC_INTERFACE void HC_CDECL HC_Show_Trim_Poly_Count (
		int						index,	   
		int *	count);
HC_INTERFACE void HC_CDECL HC_Show_Trim_Curve (
		int						index,	   
		int *	degree,
		int *	cp_count,
		float					points[],
		float					weights[],
		float					knots[],
		float *	start_u,
		float *	end_u);
HC_INTERFACE void HC_CDECL HC_Show_Trim_Curve_Count (
		int						index,	   
		int *	degree,
		int *	cp_count,
		int *	w_count,
		int *	knot_count);
HC_INTERFACE void HC_CDECL HC_Delete_Trim (
		int						index);
HC_INTERFACE void HC_CDECL HC_Open_Trim (
		int						offset);
HC_INTERFACE void HC_CDECL HC_Close_Trim (void);
HC_INTERFACE void HC_CDECL HC_Set_Trim_Operation (
		int						index,
		char const *	operation);
HC_INTERFACE void HC_CDECL HC_Show_Trim_Operation (
		int						index,
		char *	operation);
HC_INTERFACE void HC_CDECL HC_Create_Trim_Collection (void);
HC_INTERFACE void HC_CDECL HC_Set_Variable_Line_Weight (
		char const *			weight);
HC_INTERFACE void HC_CDECL HC_Set_Variable_Edge_Weight (
		char const *			weight);
HC_INTERFACE void HC_CDECL HC_Set_Variable_Marker_Size (
		char const *	size);
HC_INTERFACE void HC_CDECL HC_Show_Variable_Edge_Weight (
		char *	weight);
HC_INTERFACE void HC_CDECL HC_Show_Variable_Line_Weight (
		char *	weight);
HC_INTERFACE void HC_CDECL HC_Show_Variable_Marker_Size (
		char *	size);
HC_INTERFACE bool HC_CDECL HC_PShow_Net_Var_Edge_Weight (
		int					count,
		Key const			keys[],
		char			* weight); // HC_PShow_Net_Variable_Edge_Weight
HC_INTERFACE bool HC_CDECL HC_PShow_Net_Var_Line_Weight (
		int						count,
		Key const			keys[],
		char				* weight); // HC_PShow_Net_Variable_Line_Weight
HC_INTERFACE bool HC_CDECL HC_PShow_Net_Var_Marker_Size (
		int						count,
		Key const			keys[],
		char				* size); // HC_PShow_Net_Variable_Marker_Size
HC_INTERFACE void HC_CDECL HC_MShow_Character_Attributes(
		Key	key, 
		int		offset, 
		int		count, 
		char			* options);
HC_INTERFACE void HC_CDECL HC_Show_Character_Attribute_Cou(
		Key	key,  
		int *	count); // HC_Show_Character_Attribute_Count
HC_INTERFACE void HC_CDECL HC_Show_One_Character_Attribute(
		Key	key, 
		int		offset, 
		char	const *which, 
		char			* options);
HC_INTERFACE void HC_CDECL HC_Set_Faces (
		int				first_face,
		int				face_count);
HC_INTERFACE void HC_CDECL HC_Set_Region (
		int				region);
HC_INTERFACE void HC_CDECL HC_MSet_Region_Faces (
		Key			key,
		int				region,
		int				first_face,
		int				face_count);
HC_INTERFACE void HC_CDECL HC_MSet_Face_Regions (
		Key			key,
		int				first_face,
		int				face_count,
		int const		regions[]);
HC_INTERFACE void HC_CDECL HC_Show_Faces (
		int *		face_count,
		int 		faces[]);
HC_INTERFACE void HC_CDECL HC_Show_Faces_Count (
		int *		face_count);
HC_INTERFACE void HC_CDECL HC_Show_Region (
		int 		region[]);
HC_INTERFACE void HC_CDECL HC_MShow_Region_Faces (
		Key			key,
		int				region,
		int *	face_count,
		int		faces[]);
HC_INTERFACE void HC_CDECL HC_MShow_Region_Faces_Count (
		Key			key,
		int				region,
		int *	face_count);
HC_INTERFACE void HC_CDECL HC_MShow_Face_Regions (
		Key			key,
		int *	face_count,
		int		regions[]);
HC_INTERFACE void HC_CDECL HC_MShow_Face_Regions_Count (
		Key			key,
		int *	face_count);
HC_INTERFACE void HC_CDECL HC_MShow_Partial_Face_Regions (
		Key			key,
		int				first_face,
		int				face_count,
		int		regions[]);
HC_INTERFACE void HC_CDECL HC_Show_Region_Range (
		Key			key,
		int *		lowest,
		int *		highest);
HC_INTERFACE Key HC_CDECL HC_Insert_PolyCylinder (int p_count, Point const pts[], int r_count, float const radii[], char const *capping);
HC_INTERFACE void HC_CDECL HC_Show_PolyCylinder (Key key, int *p_count, Point pts[], int *r_count, float radii[], char *capping);
HC_INTERFACE void HC_CDECL HC_Show_PolyCylinder_Counts (Key key, int *p_count, int *r_count, char *capping);
HC_INTERFACE Key HC_CDECL HC_Insert_Shell_From_Region (Key key1, int region, char const *options);
HC_INTERFACE Key HC_CDECL HC_Insert_Unicode_Text (
	double x,
	double y,
	double z,
	unsigned short const text[]);
HC_INTERFACE void HC_CDECL HC_Show_Unicode_Text (
	Key key, 
	float *x, 
	float *y, 
	float *z, 
	unsigned short text[]);
HC_INTERFACE int HC_CDECL HC_Compute_Selection_By_Shell ( 
		char const *action, 
		char const *start_seg, 
		int point_count, 
		Point const points[], 
		int face_list_length, 
		int const face_list[]);
HC_INTERFACE int HC_CDECL HC_DCompute_Selection_By_Shell ( 
		char const *action, 
		char const *start_seg, 
		int point_count, 
		DPoint const points[], 
		int face_list_length, 
		int const face_list[]);
HC_INTERFACE int HC_CDECL HC_Compute_Selection_By_Sh_W_Op ( 
		char const *action, 
		char const *start_seg, 
		int point_count, 
		Point const points[], 
		int face_list_length, 
		int const face_list[],
		char const *options); // HC_Compute_Selection_By_Shell_With_Options
HC_INTERFACE int HC_CDECL HC_DCompute_Selection_By_Sh_W_O ( 
		char const *action, 
		char const *start_seg, 
		int point_count, 
		DPoint const points[], 
		int face_list_length, 
		int const face_list[],
		char const *options); // HC_DCompute_Selection_By_Shell_With_Options
HC_INTERFACE int HC_CDECL HC_Compute_Selection_By_Swept_S ( 
		char const *action, 
		char const *start_seg, 
		Key			shellkey,
		int			number_of_sweeps,
		float		const matrices[],		
		char const *options); // HC_Compute_Selection_By_Swept_Shell
HC_INTERFACE int HC_CDECL HC_DCompute_Selection_By_Swpt_S ( 
		char const *action, 
		char const *start_seg, 
		Key			shellkey,
		int			number_of_sweeps,
		double		const matrices[],		
		char const *options); // HC_DCompute_Selection_By_Swept_Shell
HC_INTERFACE int HC_CDECL HC_Compute_Selection_By_Swept_2 ( 
		char const *action, 
		char const *start_seg, 
		Key			shellkey,
		int			sweepcount,
		float		const matrices[],		
		char const *options,
		float		bounding_box[]); // HC_Compute_Selection_By_Swept_Shell2
HC_INTERFACE int HC_CDECL HC_DCompute_Selection_By_Swpt_2 ( 
		char const *action, 
		char const *start_seg, 
		Key			shellkey,
		int			sweepcount,
		double		const matrices[],		
		char const *options,
		double		bounding_box[]); // HC_DCompute_Selection_By_Swept_Shell2
HC_INTERFACE int HC_CDECL HC_Compute_Selection_By_Key ( 
		char const *action, 
		char const *start_seg, 
		Key key, float const matrix[]);
HC_INTERFACE int HC_CDECL HC_DCompute_Selection_By_Key ( 
		char const *action, 
		char const *start_seg, 
		Key key, double const matrix[]);
HC_INTERFACE Key HC_CDECL HC_Compute_Swept_Shell(
	Key				shellkey,					
	Vector const *	direction_in,
	Key *			front_key,					
	Key *			back_key,		
	Key *			side_key,					
	char const *	options);
HC_INTERFACE Key HC_CDECL HC_DCompute_Swept_Shell(
	Key				shellkey,					
	DVector const *	direction_in,
	Key *			front_key,					
	Key *			back_key,		
	Key *			side_key,					
	char const *	options);
HC_INTERFACE void HC_CDECL HC_Compute_Swept_Shell_Along_P (
	Key				shellkey,
	int				number_of_sweeps,
	float const 	matrices[],
	char const *	options); // HC_Compute_Swept_Shell_Along_Path
HC_INTERFACE void HC_CDECL HC_DCompute_Swept_Shell_Along_P (
	Key				shellkey,
	int				number_of_sweeps,
	double const 	matrices[],
	char const *	options); // HC_DCompute_Swept_Shell_Along_Path
HC_INTERFACE float HC_CDECL HC_Compute_Selection_Dist_By_Sh ( 
	char const *	action, 
	char const *	start_segment, 
	int				pcount, 
	Point const 	points[], 
	int				flist_length, 
	int const 		faces[] ,
	const char *	options); // HC_Compute_Selection_Distance_By_Shell
HC_INTERFACE double HC_CDECL HC_DCompute_Selection_Dist_By_S ( 
	char const *	action, 
	char const *	start_segment, 
	int				pcount, 
	DPoint const 	points[], 
	int				flist_length, 
	int const 		faces[] ,
	const char *	options); // HC_DCompute_Selection_Distance_By_Shell
HC_INTERFACE void HC_CDECL HC_Begin_Shell_Selection (void);
HC_INTERFACE void HC_CDECL HC_End_Shell_Selection (void);
HC_INTERFACE int HC_CDECL HC_Compute_Intersect_Polyline (
		int pcount1, 
		Point const points1[], 
		int flist_length1, 
		int const face_list1[],
		int pcount2, 
		Point const points2[], 
		int flist_length2, 
		int const face_list2[],
		int *polyline_count, 
		int polyline_lengths[], 
		int *polyline_points_count, 
		Point polyline_points[]);
HC_INTERFACE int HC_CDECL HC_Show_Intersect_Polyline_Size (
		int pcount1, 
		Point const points1[], 
		int flist_length1, 
		int const face_list1[],
		int pcount2, 
		Point const points2[], 
		int flist_length2, 
		int const face_list2[],
		int *polyline_count, 
		int *polyline_points_count);
HC_INTERFACE bool HC_CDECL HC_Compute_Point_Inside_Shell (
		Point const *test_point,
		int point_count,
		Point const points[],
		int face_list_length,
		int const face_list[]);
HC_INTERFACE bool HC_CDECL HC_DCompute_Point_Inside_Shell (
		DPoint const *test_point,
		int point_count,
		DPoint const points[],
		int face_list_length,
		int const face_list[]);
HC_INTERFACE bool HC_CDECL HC_Compute_Point_On_Shell (
		Point const *test_point,
		double tolerance,
		int point_count,
		Point const points[],
		int face_list_length,
		int const face_list[]);
HC_INTERFACE bool HC_CDECL HC_DCompute_Point_On_Shell (
		DPoint const *test_point,
		double tolerance,
		int point_count,
		DPoint const points[],
		int face_list_length,
		int const face_list[]);
HC_INTERFACE void HC_CDECL HC_Compute_Points_Inside_Shell (
		int test_point_count,
		Point const test_points[],
		Vector const test_points_vectors[],
		int point_count,
		Point const points[],
		int face_list_length,
		int const face_list[],
		char const *options,
		int results[]);
HC_INTERFACE void HC_CDECL HC_DCompute_Points_Inside_Shell (
		int test_point_count,
		DPoint const test_points[],
		DVector const test_points_vectors[],
		int point_count,
		DPoint const points[],
		int face_list_length,
		int const face_list[],
		char const *options,
		int results[]);
HC_INTERFACE int HC_CDECL HC_Compute_Points_On_Shell (
		int test_point_count,
		Point const test_points[],
		double tolerance,
		int point_count,
		Point const points[],
		int face_list_length,
		int const face_list[],
		int results[]);
HC_INTERFACE int HC_CDECL HC_DCompute_Points_On_Shell (
		int test_point_count,
		DPoint const test_points[],
		double tolerance,
		int point_count,
		DPoint const points[],
		int face_list_length,
		int const face_list[],
		int results[]);
HC_INTERFACE int HC_CDECL HC_Compute_Points_Distance_F_Sh (
		int test_point_count,
		Point const test_points[],
		double tolerance,
		int point_count,
		Point const points[],
		int face_list_length,
		int const face_list[],
		const char *options,
		float results[]); // HC_Compute_Points_Distance_From_Shell
HC_INTERFACE int HC_CDECL HC_DCompute_Points_Distance_F_S (
		int test_point_count,
		DPoint const test_points[],
		double tolerance,
		int point_count,
		DPoint const points[],
		int face_list_length,
		int const face_list[],
		const char *options,
		double results[]); // HC_DCompute_Points_Distance_From_Shell
HC_INTERFACE int HC_CDECL HC_Compute_Points_Distance_F_S2 (
		int test_pcount,
		Point const test_pt[],
		double tolerance,
		int point_count1,
		Point const points1[],
		int face_list_length1,
		int const face_list1[],
		const char *options,
		float results[],
		int face_results[]); // HC_Compute_Points_Distance_From_Shell2
HC_INTERFACE int HC_CDECL HC_DCompute_Points_Distance_FS2 (
		int test_pcount,
		DPoint const test_pt[],
		double tolerance,
		int point_count1,
		DPoint const points1[],
		int face_list_length1,
		int const face_list1[],
		const char *options,
		double results[],
		int face_results[]); // HC_DCompute_Points_Distance_From_Shell2
HC_INTERFACE void HC_CDECL HC_Compute_Shell_Enclosure (
		int point_count1,
		Point const points1[],
		int face_list_length1,
		int const face_list1[],
		int point_count2,
		Point const points2[],
		int face_list_length2,
		int const face_list2[],
		char *result);
HC_INTERFACE void HC_CDECL HC_DCompute_Shell_Enclosure (
		int point_count1,
		DPoint const points1[],
		int face_list_length1,
		int const face_list1[],
		int point_count2,
		DPoint const points2[],
		int face_list_length2,
		int const face_list2[],
		char *result);
HC_INTERFACE int HC_CDECL HC_Compute_Selection_By_Ray ( 
		char const *action, 
		char const *start_seg, 
		Point const *start_point,
		Vector const *direction);
HC_INTERFACE int HC_CDECL HC_DCompute_Selection_By_Ray ( 
		char const *action, 
		char const *start_seg, 
		DPoint const *start_point,
		DVector const *direction);
HC_INTERFACE bool HC_CDECL HC_Filter_Circumcuboid (char const *segment, char const *options, Point *min, Point *max);
HC_INTERFACE bool HC_CDECL HC_Filter_Circumcuboid_By_Key (Key segKey, char const *options, Point *min, Point *max);
HC_INTERFACE bool HC_CDECL HC_Filter_Circumsphere (char const *segment, char const *options, Point *center, float *radius);
HC_INTERFACE bool HC_CDECL HC_Filter_Circumsphere_By_Key (Key segKey, char const *options, Point *center, float *radius);
HC_INTERFACE bool HC_CDECL HC_DFilter_Circumcuboid (char const *segment, char const *options, DPoint *min, DPoint *max);
HC_INTERFACE bool HC_CDECL HC_DFilter_Circumcuboid_By_Key (Key segKey, char const *options, DPoint *min, DPoint *max);
HC_INTERFACE bool HC_CDECL HC_DFilter_Circumsphere (char const *segment, char const *options, DPoint *center, double *radius);
HC_INTERFACE bool HC_CDECL HC_DFilter_Circumsphere_By_Key (Key segKey, char const *options, DPoint *center, double *radius);
HC_INTERFACE void HC_CDECL HC_Set_Polygonal_Clip_Region (int count, Point const points[], char const * options);
HC_INTERFACE void HC_CDECL HC_DSet_Polygonal_Clip_Region (int count, DPoint const points[], char const * options);
HC_INTERFACE void HC_CDECL HC_Set_Complex_Clip_Region (int loops, int const lengths[], Point const points[], char const * options);
HC_INTERFACE void HC_CDECL HC_DSet_Complex_Clip_Region (int loops, int const lengths[], DPoint const points[], char const * options);
HC_INTERFACE void HC_CDECL HC_Show_Clip_Region (
 int *loopCount, int loopSizeArray[], Point points[], char *options);
HC_INTERFACE void HC_CDECL HC_DShow_Clip_Region (
 int *loopCount, int loopSizeArray[], DPoint points[], char *options);
HC_INTERFACE void HC_CDECL HC_Show_Clip_Region_Size (
 int *loopCount, int *totalPointCount, char *options);
HC_INTERFACE void HC_CDECL HC_DShow_Clip_Region_Size (
 int *loopCount, int *totalPointCount, char *options);
HC_INTERFACE int HC_CDECL HC_PShow_Net_Clip_Region (
 int count, Key const keys[], int *loopCount, int loopSizeArray[], Point points[], char *spec);
HC_INTERFACE int HC_CDECL HC_PShow_Net_Clip_Region_Size (
 int count, Key const keys[], int *loopCount, int *totalPointCount, char *spec);
HC_INTERFACE int HC_CDECL HC_DPShow_Net_Clip_Region (
 int count, Key const keys[], int *loopCount, int loopSizeArray[], DPoint points[], char *spec);
HC_INTERFACE int HC_CDECL HC_DPShow_Net_Clip_Region_Size (
 int count, Key const keys[], int *loopCount, int *totalPointCount, char *spec);
HC_INTERFACE void HC_CDECL HC_UnSet_Clip_Region (void);
HC_INTERFACE void HC_CDECL HC_Regenerate_LOD (char const *segment, char const *options);
HC_INTERFACE void HC_CDECL HC_Regenerate_LOD_By_Key (Key segKey, char const *options);
HC_INTERFACE void HC_CDECL HC_Set_Text_Region (int pCount, Point const points[], char const *options);
HC_INTERFACE void HC_CDECL HC_Show_Text_Region (int *count, Point points[], char *options);
HC_INTERFACE void HC_CDECL HC_Show_Text_Region_Count (int *count);
HC_INTERFACE void HC_CDECL HC_UnSet_Text_Region (void);
HC_INTERFACE void HC_CDECL HC_Compute_Screen_Extent (
		char const *segment, char const *options, float *left, float *right, float *bottom, float *top);
HC_INTERFACE void HC_CDECL HC_Compute_Screen_Extent_By_Pat (
		int count, Key const keys[], char const *options, float *left, float *right, float *bottom, float *top); // HC_Compute_Screen_Extent_By_Path
HC_INTERFACE void HC_CDECL HC_Compute_Screen_Extent_By_Key (
		Key segKey, char const *options, float *left, float *right, float *bottom, float *top);
HC_INTERFACE void HC_CDECL HC_Set_Camera_Near_Limit (double wlimit);
HC_INTERFACE bool HC_CDECL HC_Show_Camera_Near_Limit (float *limit);
HC_INTERFACE bool HC_CDECL HC_PShow_Net_Camera_Near_Limit (int count, Key const keys[], float *limit);
HC_INTERFACE void HC_CDECL HC_UnSet_Camera_Near_Limit (void);
HC_INTERFACE bool HC_CDECL HC_PShow_One_Net_User_Opt_Len (int count, Key const keys[], char const * which, int *length); // HC_PShow_One_Net_User_Option_Length
HC_INTERFACE void HC_CDECL HC_Show_One_User_Option_Length (char const *which, int *length);
HC_INTERFACE bool HC_CDECL HC_PShow_One_Net_Uni_Opt_Len (int count, Key const keys[], unsigned short const * which, int * length); // HC_PShow_One_Net_Unicode_Option_Length
HC_INTERFACE void HC_CDECL HC_Show_One_Uni_Option_Length (unsigned short const *text, int *length); // HC_Show_One_Unicode_Option_Length
HC_INTERFACE Key HC_CDECL HC_Show_Original_Key (Key key);
HC_INTERFACE Key HC_CDECL HC_Show_Renumbered_Key (Key key, char *status);
HC_INTERFACE double HC_CDECL HC_Compute_Vector_Angle (Vector const *vector, Vector const *vector2);
HC_INTERFACE double HC_CDECL HC_DCompute_Vector_Angle (DVector const *vector, DVector const *vector2);
HC_INTERFACE void HC_CDECL HC_Set_Conditions (char const *list);
HC_INTERFACE void HC_CDECL HC_UnSet_Conditions (void);
HC_INTERFACE void HC_CDECL HC_UnSet_One_Condition (char const *condition);
HC_INTERFACE void HC_CDECL HC_Set_Conditional_Action (char const * options, char const * condition);
HC_INTERFACE void HC_CDECL HC_Show_Cond_Action_Types (char * action_types); // HC_Show_Conditional_Action_Types
HC_INTERFACE void HC_CDECL HC_Show_One_Conditional_Action (char const * action_type, char * options, char * condition);
HC_INTERFACE void HC_CDECL HC_UnSet_Conditional_Actions (void);
HC_INTERFACE void HC_CDECL HC_UnSet_One_Conditional_Action (char const *which);
HC_INTERFACE Key HC_CDECL HC_Conditional_Style (char const *style, char const *conditions);
HC_INTERFACE Key HC_CDECL HC_Conditional_Style_By_Key (Key seg_key, char const *conditions);
HC_INTERFACE Key HC_CDECL HC_Conditional_Include (char const *include, char const *condition);
HC_INTERFACE Key HC_CDECL HC_Conditional_Include_By_Key (Key seg_key, char const *condition);
HC_INTERFACE void HC_CDECL HC_Show_Conditions (char *list);
HC_INTERFACE bool HC_CDECL HC_PShow_Net_Conditions (int count, Key const keys[], char *options);
HC_INTERFACE void HC_CDECL HC_Show_Conditions_Length (int *length);
HC_INTERFACE bool HC_CDECL HC_PShow_Net_Conditions_Length (int count, Key const keys[], int *length);
HC_INTERFACE void HC_CDECL HC_Show_One_Condition (char const *condition, char *value);
HC_INTERFACE bool HC_CDECL HC_PShow_One_Net_Condition (int count, Key const keys[], char const * condition, char * options);
HC_INTERFACE void HC_CDECL HC_Show_Conditional_Include (Key key, char *pathname, char *conditions);
HC_INTERFACE void HC_CDECL HC_Show_Conditional_Style (Key key, char *pathname, char *conditions);
HC_INTERFACE Key HC_CDECL HC_Copy_Segment_With_Mapping (char const *source_seg, char const *destination, int count, Key const interesting_keys[], Key copied_keys[]);
HC_INTERFACE void HC_CDECL HC_Reverse_Contents_Search (void);
HC_INTERFACE void HC_CDECL HC_MShow_Vertex_Colors_By_Value (Key key, char const *type, int offset, int count, RGB rgb[]);
HC_INTERFACE void HC_CDECL HC_MShow_Vertex_Colors_By_FInd (Key key, char const *type, int offset, int pcount, float indices[]); // HC_MShow_Vertex_Colors_By_FIndex
HC_INTERFACE void HC_CDECL HC_MShow_Face_Colors_By_Value (Key key, int offset, int count, RGB rgb[]);
HC_INTERFACE void HC_CDECL HC_MShow_Face_Colors_By_FIndex (Key key, int offset, int count, float findices[]);
HC_INTERFACE void HC_CDECL HC_MShow_Vertex_Normals (Key key, int offset, int count, Vector normals[]);
HC_INTERFACE void HC_CDECL HC_MShow_Vertex_Parameters (Key key, int offset, int pcount, int *number, float params[]);
HC_INTERFACE void HC_CDECL HC_MShow_Vertex_Parameter_Size (Key key, int *number);
HC_INTERFACE void HC_CDECL HC_Set_Circular_Center (double x, double y, double z);
HC_INTERFACE void HC_CDECL HC_Set_Circular_Center_By_Key (Key key, double x, double y, double z);
HC_INTERFACE void HC_CDECL HC_DSet_Circular_Center (double x, double y, double z);
HC_INTERFACE void HC_CDECL HC_DSet_Circular_Center_By_Key (Key key, double x, double y, double z);
HC_INTERFACE bool HC_CDECL HC_Show_Circular_Center (Key key, float * x, float * y, float * z);
HC_INTERFACE bool HC_CDECL HC_DShow_Circular_Center (Key key, double * x, double * y, double * z);
HC_INTERFACE void HC_CDECL HC_MShow_Net_Vertex_Normals (Key key, int offset, int count, Vector normals[]);
HC_INTERFACE bool HC_CDECL HC_Show_Error (char * severity, int * category, int * severity_level);
HC_INTERFACE void HC_CDECL HC_MSet_Vertex_Visibilities (Key key, int offset, int count, char const settings[]);
HC_INTERFACE void HC_CDECL HC_MUnSet_Vertex_Visibilities (Key key, int offset, int count);
HC_INTERFACE void HC_CDECL HC_MShow_Vertex_Visibilities (Key key, int offset, int count, char settings[]);
HC_INTERFACE void HC_CDECL HC_MSet_Face_Visibilities (Key key, int offset, int count, char const settings[]);
HC_INTERFACE void HC_CDECL HC_MUnSet_Face_Visibilities (Key key, int offset, int count);
HC_INTERFACE void HC_CDECL HC_MShow_Face_Visibilities (Key key, int offset, int count, char settings[]);
HC_INTERFACE void HC_CDECL HC_MSet_Specific_Vertex_Vis (Key key, int count, int const indices[], char const settings[]); // HC_MSet_Specific_Vertex_Visibilities
HC_INTERFACE void HC_CDECL HC_MUnSet_Specific_Vertex_Vis (Key key, int count, int const  indices[]); // HC_MUnSet_Specific_Vertex_Visibilities
HC_INTERFACE void HC_CDECL HC_MShow_Specific_Vertex_Vis (Key key, int count, int const  indices[], char settings[]); // HC_MShow_Specific_Vertex_Visibilities
HC_INTERFACE void HC_CDECL HC_MSet_Specific_Face_Vis (Key key, int count, int const indices[], char const settings[]); // HC_MSet_Specific_Face_Visibilities,HC_MSet_Face_Visibilities_By_Index
HC_INTERFACE void HC_CDECL HC_MUnSet_Specific_Face_Vis (Key key, int count, int const indices[]); // HC_MUnSet_Specific_Face_Visibilities,HC_MUnSet_Face_Visibilities_By_Index
HC_INTERFACE void HC_CDECL HC_MShow_Specific_Face_Vis (Key key, int count, int const indices[], char settings[]); // HC_MShow_Specific_Face_Visibilities,HC_MShow_Face_Visibilities_By_Index
HC_INTERFACE void HC_CDECL HC_MUnSet_Face_Colors (Key key, int offset, int count);
HC_INTERFACE void HC_CDECL HC_MUnSet_Vertex_Colors (Key key, int offset, int count);
HC_INTERFACE Key HC_CDECL HC_Insert_Cutting_Section (int count, Plane const planes[]);
HC_INTERFACE Key HC_CDECL HC_DInsert_Cutting_Section (int count, DPlane const planes[]);
HC_INTERFACE void HC_CDECL HC_Show_Cutting_Section (Key key, int *count, Plane planes[]);
HC_INTERFACE void HC_CDECL HC_Show_Cutting_Section_Count (Key key, int *count);
HC_INTERFACE void HC_CDECL HC_DShow_Cutting_Section (Key key, int *count, DPlane planes[]);
HC_INTERFACE void HC_CDECL HC_Edit_Cutting_Section (Key key, int ioffset, int ndelete, int insert, Plane const iplanes[]);
HC_INTERFACE void HC_CDECL HC_DEdit_Cutting_Section (Key key, int ioffset, int ndelete, int insert, DPlane const iplanes[]);
HC_INTERFACE void HC_CDECL HC_Compute_Triangulation(Point const points[], int flist_length, int const face_list[], int *tlist_length, int triangle_list[]);
HC_INTERFACE void HC_CDECL HC_Compute_Triangulation_Size(Point const points[], int flist_length, int const face_list[], int *tlist_length);
HC_INTERFACE bool HC_CDECL HC_Compute_Polyline_From_Curve(Key key, char const *options, int *pcount, Point points[]);
HC_INTERFACE bool HC_CDECL HC_DCompute_Polyline_From_Curve(Key key, char const *options, int *pcount, DPoint points[]);
HC_INTERFACE void HC_CDECL HC_Compute_Pline_From_Crv_Size(Key key, char const *options, int *pcount); // HC_Compute_Polyline_From_Curve_Size
HC_INTERFACE void HC_CDECL HC_Optimize_Segment_Tree (char const * segname, char const * option_string);
HC_INTERFACE void HC_CDECL HC_Optimize_Segment_Tree_By_Key (Key segkey, char const * option_string);
HC_INTERFACE void HC_CDECL HC_Show_Optimized_Mapping (char const * segname, char const * direction,
													Key in_owner, Key in_key, int in_region,
													Key * out_owner, Key * out_key, int * out_region);
HC_INTERFACE bool HC_CDECL HC_PShow_Net_Texture(int count, Key const keys[],char const * name, char *output, Key * out_image);
HC_INTERFACE bool HC_CDECL HC_PShow_One_Net_Texture(int count, Key const keys[], char const * name, char const * opt, char *output);
HC_INTERFACE bool HC_CDECL HC_PShow_Net_Shader(int count, Key const keys[],char const * name, char *output, char *shader_source);
HC_INTERFACE bool HC_CDECL HC_PShow_One_Net_Shader_Option(int count, Key const keys[], char const * name, char const * opt, char *output);
HC_INTERFACE void HC_CDECL HC_Show_Priority_Range (Key key, int *low, int *high);
HC_INTERFACE void HC_CDECL HC_Delete_Specific_Shell_Points (Key key, int count, int const  indices[]);
HC_INTERFACE void HC_CDECL HC_Delete_Specific_Shell_Faces (Key key, int count, int const  indices[]);
HC_INTERFACE void HC_CDECL HC_MSet_Spec_Vert_Col_By_FIndex (
	Key key, char const *types, int count, int const  offsets[], float const indices[]); // HC_MSet_Specific_Vertex_Colors_By_FIndex
HC_INTERFACE void HC_CDECL HC_MSet_Spec_Vert_Col_By_Value (
	Key key, char const *types, int count, int const  offsets[], char const *color_space, RGB const values[]); // HC_MSet_Specific_Vertex_Colors_By_Value
HC_INTERFACE void HC_CDECL HC_MSet_Spec_Face_Col_By_FIndex (
	Key key, int count, int const  indices[], float const findices[]); // HC_MSet_Specific_Face_Colors_By_FIndex
HC_INTERFACE void HC_CDECL HC_MSet_Spec_Face_Col_By_Value (
	Key key, int count, int const  indices[], char const *color_space, RGB const values[]); // HC_MSet_Specific_Face_Colors_By_Value
HC_INTERFACE void HC_CDECL HC_MShow_Spec_Vert_Col_By_FInd (
	Key key, char const *type, int pcount, int const  offsets[], float indices[]); // HC_MShow_Specific_Vertex_Colors_By_FIndex
HC_INTERFACE void HC_CDECL HC_MShow_Spec_Vert_Col_By_Value (
	Key key, char const *type, int count, int const  offsets[], RGB rgb[]); // HC_MShow_Specific_Vertex_Colors_By_Value
HC_INTERFACE void HC_CDECL HC_MShow_Spec_Face_Col_By_FInd (
	Key key, int count, int const  indices[], float findices[]); // HC_MShow_Specific_Face_Colors_By_FIndex
HC_INTERFACE void HC_CDECL HC_MShow_Spec_Face_Col_By_Value (
	Key key, int count, int const  indices[], RGB rgb[]); // HC_MShow_Specific_Face_Colors_By_Value
HC_INTERFACE void HC_CDECL HC_MUnSet_Spec_Vertex_Colors (Key key, int count, int const  offsets[]); // HC_MUnSet_Specific_Vertex_Colors
HC_INTERFACE void HC_CDECL HC_MUnSet_Spec_Face_Colors (Key key, int count, int const  faces[]); // HC_MUnSet_Specific_Face_Colors
HC_INTERFACE int HC_CDECL HC_Gather_Capping_Geometry (int key_count, Key const path_keys[], Key cut_key, char const * options);

HC_INTERFACE Key HC_CDECL HC_Generate_Shell_From_Geometry(Key geom, char const *options);
HC_INTERFACE Key HC_CDECL HC_Generate_Edges(Key geom, char const *options);
HC_INTERFACE Key HC_CDECL HC_Generate_Poly_From_Geometry(Key geom, char const *options); // HC_Generate_Polyline_From_Geometry
HC_INTERFACE void HC_CDECL HC_Compute_Face_Neighborhood_Si (
	Key shell_key, 
	int face_index, 
	int *count); // HC_Compute_Face_Neighborhood_Size
HC_INTERFACE void HC_CDECL HC_Compute_Face_Neighborhood (
	Key shell_key, 
	int face_index, 
	int *count, 
	int neighbors[]);
HC_INTERFACE void HC_CDECL HC_Show_Net_Normal (float *x, float *y, float *z);
HC_INTERFACE void HC_CDECL HC_Rollback_Ink(int count);
HC_INTERFACE Key HC_CDECL HC_Reference_Geometry (char const *segname);
HC_INTERFACE Key HC_CDECL HC_Reference_Geometry_By_Key (Key geom_key);
HC_INTERFACE Key HC_CDECL HC_Conditional_Reference (char const *segname, char const *condition);
HC_INTERFACE Key HC_CDECL HC_Conditional_Reference_By_Key (Key geom_key, char const *condition);
HC_INTERFACE Key HC_CDECL HC_Show_Reference_Geometry (Key reference_key);
HC_INTERFACE Key HC_CDECL HC_Show_Conditional_Reference (Key reference_key, char *conditions);
HC_INTERFACE void HC_CDECL HC_Show_Reference_Geometry_Filt (Key key, char *filter); // HC_Show_Reference_Geometry_Filter
HC_INTERFACE void HC_CDECL HC_Begin_Open_Item_Search (void);
HC_INTERFACE bool HC_CDECL HC_Find_Open_Item (Key	*key, char *type, int *offset1, int	*offset2);
HC_INTERFACE void HC_CDECL HC_Show_Open_Item_Count (int	*count);
HC_INTERFACE void HC_CDECL HC_End_Open_Item_Search (void);
HC_INTERFACE bool HC_CDECL HC_PShow_Net_Color_Map_Length (int count, Key const keys[], int *value);
HC_INTERFACE void HC_CDECL HC_Show_Color_Map_Length (int *value);
HC_INTERFACE void HC_CDECL HC_Move_Key_By_Key (Key key, Key newowner); // HC_Move_By_Key_By_Key
HC_INTERFACE void HC_CDECL HC_Set_User_Data (intptr_t index, void const *data, long data_length);
HC_INTERFACE long HC_CDECL HC_Show_One_User_Data (intptr_t index, void *data, long data_length);
HC_INTERFACE long HC_CDECL HC_Show_User_Data_Indices (intptr_t data_indices[], long data_indices_count);
HC_INTERFACE void HC_CDECL HC_UnSet_One_User_Data (intptr_t index);
HC_INTERFACE long HC_CDECL HC_PShow_One_Net_User_Data (
	int					count,
	Key const 			keys[],	
	intptr_t index, 
	void *data, 
	long data_length);
HC_INTERFACE int HC_CDECL HC_MShow_Vertex_Colors_With_Ex (Key key, char const *	geometry, int offset, int count, char result_type[], float index_colors[], RGB rgb_colors[], RGBA rgba_colors[]); // HC_MShow_Vertex_Colors_With_Existence
HC_INTERFACE void HC_CDECL HC_MUnSet_Vertex_Normals (Key key, int offset, int count);
HC_INTERFACE void HC_CDECL HC_MUnSet_Face_Normals (Key key, int offset, int count);
HC_INTERFACE void HC_CDECL HC_MUnSet_Vertex_Parameters (Key key, int offset, int count);
HC_INTERFACE void HC_CDECL HC_MSet_Specific_Vertex_Normals (Key key, int count, int const  indices[], Vector const normals[]);
HC_INTERFACE void HC_CDECL HC_MUnSet_Specific_Vert_Normals (Key key, int count, int const  indices[]); // HC_MUnSet_Specific_Vertex_Normals
HC_INTERFACE void HC_CDECL HC_MShow_Specific_Vert_Normals (Key key, int count, int const  indices[], Vector normals[]); // HC_MShow_Specific_Vertex_Normals
HC_INTERFACE void HC_CDECL HC_MSet_Specific_Face_Normals (Key key, int count, int const  indices[], Vector const normals[]);
HC_INTERFACE void HC_CDECL HC_MUnSet_Specific_Face_Normals (Key key, int count, int const  indices[]);
HC_INTERFACE void HC_CDECL HC_MShow_Specific_Face_Normals (Key key, int count, int const  indices[], Vector normals[]);
HC_INTERFACE void HC_CDECL HC_MSet_Spec_Vertex_Parameters (Key key, int count, int const  indices[], int number_parameters, float const user_parameters[]); // HC_MSet_Specific_Vertex_Parameters
HC_INTERFACE void HC_CDECL HC_MShow_Spec_Vertex_Parameters (Key key, int count, int const  indices[], int * number_parameters, float user_parameters[]);
HC_INTERFACE void HC_CDECL HC_MUnSet_Spec_Vert_Parameters (Key key, int count, int const  indices[]); // HC_MUnSet_Specific_Vertex_Parameters
HC_INTERFACE void HC_CDECL HC_MUnSet_Vertex_Colors2 (Key key, char const * type, int offset, int count);
HC_INTERFACE void HC_CDECL HC_MUnSet_Spec_Vertex_Colors2 (Key key, char const * type, int count, int const  indices[]); // HC_MUnSet_Specific_Vertex_Colors2
HC_INTERFACE int HC_CDECL HC_MShow_Spec_Vert_Colors_W_Ex (Key key, char const *	geometry, int count, int const vertices[], char result_type[], float index_colors[], RGB rgb_colors[], RGBA rgba_colors[]); // HC_MShow_Specific_Vertex_Colors_With_Existence
HC_INTERFACE int HC_CDECL HC_MShow_Face_Colors_With_Ex (Key key, char const * geometry, int offset, int count, char result_type[], float index_colors[], RGB rgb_colors[]); // HC_MShow_Face_Colors_With_Existence
HC_INTERFACE int HC_CDECL HC_MShow_Spec_Face_Colors_W_Ex (Key key, char const *	geometry, int count, int const faces[], char result_type[], float index_colors[], RGB rgb_colors[]); // HC_MShow_Specific_Face_Colors_With_Existence
HC_INTERFACE int HC_CDECL HC_MShow_Vertex_Parameters_W_Ex (Key key, int offset, int count, char existence[], int * param_width, float user_parameters[]); // HC_MShow_Vertex_Parameters_With_Existence
HC_INTERFACE int HC_CDECL HC_MShow_Spec_Vertex_Params_W_E (Key key, int count, int const specific_vertices[], char existence[], int * param_width, float user_parameters[]); // HC_MShow_Specific_Vertex_Parameters_With_Existence
HC_INTERFACE int HC_CDECL HC_MShow_Vertex_Vis_W_Ex (Key key, int offset, int count, char existence[], char visibilities[]); // HC_MShow_Vertex_Visibilities_With_Existence
HC_INTERFACE int HC_CDECL HC_MShow_Spec_Vertex_Vis_W_Ex (Key key, int count, int const  indices[], char existence[], char visibilities[]); // HC_MShow_Specific_Vertex_Visibilities_With_Existence
HC_INTERFACE int HC_CDECL HC_MShow_Face_Vis_W_Ex (Key key, int offset, int count, char existence[], char visibilities[]); // HC_MShow_Face_Visibilities_With_Existence
HC_INTERFACE int HC_CDECL HC_MShow_Spec_Face_Vis_W_Ex (Key key, int count, int const  indices[], char existence[], char visibilities[]); // HC_MShow_Specific_Face_Visibilities_With_Existence
HC_INTERFACE int HC_CDECL HC_MShow_Vertex_Normals_W_Ex (Key key, int offset, int count, char existence[], Vector normals[]); // HC_MShow_Vertex_Normals_With_Existence
HC_INTERFACE int HC_CDECL HC_MShow_Spec_Vert_Normals_W_Ex (Key key, int count, int const  indices[], char existence[], Vector normals[]); // HC_MShow_Specific_Vertex_Normals_With_Existence
HC_INTERFACE int HC_CDECL HC_MShow_Face_Normals_W_Ex (Key key, int offset, int count, char existence[], Vector normals[]); // HC_MShow_Face_Normals_With_Existence
HC_INTERFACE int HC_CDECL HC_MShow_Spec_Face_Normals_W_Ex (Key key, int count, int const  indices[], char existence[], Vector normals[]); // HC_MShow_Specific_Face_Normals_With_Existence
HC_INTERFACE Key HC_CDECL HC_Include_Segment_Key_By_Key (Key key, Key includer_key);
HC_INTERFACE Key HC_CDECL HC_Conditional_Include_Key_By_Ke (Key key, char const *condition, Key includer_key); // HC_Conditional_Include_Key_By_Key
HC_INTERFACE void HC_CDECL HC_Define_Named_Style (char const * style_name, char const * source_segment);
HC_INTERFACE void HC_CDECL HC_Define_Named_Style_By_Key (char const * style_name, Key style_segment_key);
HC_INTERFACE void HC_CDECL HC_UnDefine_Named_Style (char const * style_name);
HC_INTERFACE Key HC_CDECL HC_Show_Named_Style (char const * style_name, char *style_source);
HC_INTERFACE void HC_CDECL HC_Begin_Named_Style_Search (void);
HC_INTERFACE bool HC_CDECL HC_Find_Named_Style (char * style_name);
HC_INTERFACE void HC_CDECL HC_Show_Named_Style_Count (int * count);
HC_INTERFACE void HC_CDECL HC_End_Named_Style_Search (void);
HC_INTERFACE Key HC_CDECL HC_PShow_Net_Named_Style (
	int				count,
	Key const 		path_keys[],
	char const *	style_name,
	char *			style_segment_name);
HC_INTERFACE Key HC_CDECL HC_Named_Style_Segment (char const * style_name);
HC_INTERFACE Key HC_CDECL HC_Conditional_Named_Style (char const * style_name, char const * condition);
HC_INTERFACE void HC_CDECL HC_Show_Named_Style_Segment (Key style_key, char * style_name);
HC_INTERFACE Key HC_CDECL HC_Open_Segment_Key_By_Key (Key parent_segment, char const *child_segment_name);
HC_INTERFACE Key HC_CDECL HC_Style_Segment_Key_By_Key (Key target_segment, Key style_segment);
HC_INTERFACE Key HC_CDECL HC_Named_Style_Segment_By_Key (Key target_segment, char const * style_name);
HC_INTERFACE Key HC_CDECL HC_Conditional_Named_Style_By_Key (Key target_segment, char const * style_name, char const * condition);
HC_INTERFACE void HC_CDECL HC_Show_Conditional_Named_Style (
	Key				style_key,
	char *			style_name,
	char *			condition);
HC_INTERFACE Key HC_CDECL HC_Create_Segment_Key_By_Key (Key parent_segment, char const * child_segment_name);
HC_INTERFACE void HC_CDECL HC_Set_Default_Line_Styles();
HC_INTERFACE void HC_CDECL HC_Set_Default_Line_Styles_By_K(Key target_segment); // HC_Set_Default_Line_Styles_By_Key
HC_INTERFACE void HC_CDECL HC_Set_Default_Glyphs();
HC_INTERFACE void HC_CDECL HC_Set_Default_Glyphs_By_Key(Key target_segment);
HC_INTERFACE Key HC_CDECL HC_Reference_Geometry_Key_By_Ke (Key target_segment, Key reference_key); // HC_Reference_Geometry_Key_By_Key
HC_INTERFACE bool HC_CDECL HC_PShow_Net_Line_Style (
	int						count,
	Key const 				keys[],
	char const *			name,
	char *					definition);
HC_INTERFACE bool HC_CDECL HC_PShow_Net_Line_Style_Size (
	int						count,
	Key const 				keys[],
	char const *			name,
	int *					size);
HC_INTERFACE bool HC_CDECL HC_PShow_Net_Glyph (
	int						count,
	Key const 				keys[],
	char const *			name,
	char *					definition);
HC_INTERFACE bool HC_CDECL HC_PShow_Net_Glyph_Size (
	int						count,
	Key const 				keys[],
	char const *			name,
	int *					size);
HC_INTERFACE void HC_CDECL HC_Generate_Error (
	int						level,
	int						category,
	int						specific,
	char const *			buf1,
	char const *			buf2,
	char const *			buf3);
HC_INTERFACE void HC_CDECL HC_Commit_Proxy (
	Key						key,
	const char *			options);
HC_INTERFACE Key HC_CDECL HC_Create_Proxy (
	Key						key);
HC_INTERFACE void HC_CDECL HC_Begin_Error_Handler_Search ();
HC_INTERFACE void HC_CDECL HC_Begin_Exit_Handler_Search ();
HC_INTERFACE void HC_CDECL HC_End_Error_Handler_Search ();
HC_INTERFACE void HC_CDECL HC_End_Exit_Handler_Search ();
HC_INTERFACE bool HC_CDECL HC_Find_Error_Handler (Void_Routine * handler);
HC_INTERFACE bool HC_CDECL HC_Find_Exit_Handler (Void_Routine * handler);
HC_INTERFACE void HC_CDECL HC_Show_Error_Handler_Count (int *	count);
HC_INTERFACE void HC_CDECL HC_Show_Exit_Handler_Count (int * count);
HC_INTERFACE Key HC_CDECL HC_Compute_Subshell ( 
	Key			source,
	int			keep_fcount, 
	const int 	keep_faces[],
	int			keep_pcount, 
	const int   keep_pts1[], 
	const int   keep_pts2[],
	int 		face_map[], 
	int 		pts_map[]);
HC_INTERFACE void HC_CDECL HC_MSet_Specific_Edge_Vis (
	Key				key,
	int				count,
	int const 		indices1[],
	int const 		indices2[],
	char const  	settings[]); // HC_MSet_Specific_Edge_Visibilities
HC_INTERFACE void HC_CDECL HC_MShow_Face_Normals (
	Key				key,
	int				offset,
	int				count,
	Vector 	normals[]);
HC_INTERFACE void HC_CDECL HC_MShow_Net_Face_Normals (
	Key				key,
	int				offset,
	int				count,
	Vector 	normals[]);
HC_INTERFACE void HC_CDECL HC_MShow_Specific_Edge_Vis (
	Key				key,
	int				count,
	int const 		indices1[],
	int const 		indices2[],
	char 		settings[]); // HC_MShow_Specific_Edge_Visibilities
HC_INTERFACE void HC_CDECL HC_MUnSet_Specific_Edge_Vis (
	Key				key,
	int				count,
	int const 		indices1[],
	int const 		indices2[]); // HC_MUnSet_Specific_Edge_Visibilities
HC_INTERFACE void HC_CDECL HC_Show_Font_Info_By_Path (
	int				count,
	Key const 		keys[],
	char const *	font,
	char const *	item,
	char *			data);
HC_INTERFACE void HC_CDECL HC_Show_Reference_Keys (
	Key				key,
	int *		count,
	Key  		keys[]);
HC_INTERFACE void HC_CDECL HC_Show_Reference_Keys_Count (
	Key				key,
	int *		count);
HC_INTERFACE bool HC_CDECL HC_PShow_Net_Line_Pattern_Exp (
	int						count,
	Key const 				keys[],
	char *					pattern); // HC_PShow_Net_Line_Pattern_Explicit
HC_INTERFACE bool HC_CDECL HC_PShow_Net_Edge_Pattern_Exp (
	int						count,
	Key const 				keys[],
	char *					pattern); // HC_PShow_Net_Edge_Pattern_Explicit
HC_INTERFACE int HC_CDECL HC_Begin_Buffered_Selection (
	char const *			start_name,
	double					left,
	double					right,
	double					bottom,
	double					top,
	char const *			options);
HC_INTERFACE int HC_CDECL HC_Collect_Area_Samples (
	double					left,
	double					right,
	double					bottom,
	double					top);
HC_INTERFACE int HC_CDECL HC_Show_Sample_Max_Length ();
HC_INTERFACE bool HC_CDECL HC_Find_Next_Sample (
	int *				key_count,
	Key 				keys[],
	int *				offset1,
	int *				offset2,
	int *				offset3);
HC_INTERFACE void HC_CDECL HC_End_Buffered_Selection ();

HC_INTERFACE void HC_CDECL HC_UnSet_Priority (Key key);


HC_INTERFACE void HC_CDECL HC_Define_Highlight (
	int						count,
	Key const				keys[],
	char const *			style,
	char const *			options);

HC_INTERFACE void HC_CDECL HC_Define_Geometry_Highlight (
	int						count,
	Key const				keys[],
	char const *			style,
	char const *			options,
	int						elements_count,
	int const				offset1[],
	int const				offset2[],
	int const				offset3[]);

HC_INTERFACE int HC_CDECL HC_UnDefine_Highlight (
	int						count,
	Key const				keys[],
	char const *			style,
	char const *			options);

HC_INTERFACE int HC_CDECL HC_UnDefine_Geometry_Highlight (
	int						count,
	Key const				keys[],
	char const *			style,
	char const *			options,
	int						elements_count,
	int const				offset1[],
	int const				offset2[],
	int const				offset3[]);

HC_INTERFACE bool HC_CDECL HC_Check_Double_Precision(Key key, char const * options);

HC_INTERFACE void HC_CDECL HC_DSet_Camera (
	DPoint const *position,
	DPoint const *target,
	DVector const *up,
	double width,
	double height,
	char const *projection);
HC_INTERFACE void HC_CDECL HC_DSet_Camera_By_Volume (
	char const *	proj,
	double			xmin,
	double			xmax,
	double			ymin,
	double			ymax);
HC_INTERFACE void HC_CDECL HC_DSet_Camera_Position (double x, double y, double z);
HC_INTERFACE void HC_CDECL HC_DSet_Camera_Target (double x, double y, double z);
HC_INTERFACE void HC_CDECL HC_DSet_Camera_Up_Vector (double x, double y, double z);
HC_INTERFACE void HC_CDECL HC_DSet_Camera_Field (double width, double height);

HC_INTERFACE void HC_CDECL HC_DShow_Camera (
	DPoint *	position,
	DPoint *	target,
	DVector *	up,
	double *	width,
	double *	height,
	char *		projection);
HC_INTERFACE void HC_CDECL HC_DShow_Camera_By_Volume (
	char *		proj,
	double *	xmin,
	double *	xmax,
	double *	ymin,
	double *	ymax);
HC_INTERFACE void HC_CDECL HC_DShow_Camera_Position (double *x, double *y, double *z);
HC_INTERFACE void HC_CDECL HC_DShow_Camera_Target (double *x, double *y, double *z);
HC_INTERFACE void HC_CDECL HC_DShow_Camera_Up_Vector (double *x, double *y, double *z);
HC_INTERFACE void HC_CDECL HC_DShow_Camera_Field (double *width, double *height);

HC_INTERFACE void HC_CDECL HC_DOrbit_Camera (double horiz, double vert);
HC_INTERFACE void HC_CDECL HC_DPan_Camera (double horiz, double vert);
HC_INTERFACE void HC_CDECL HC_DRoll_Camera (double angle);
HC_INTERFACE void HC_CDECL HC_DDolly_Camera (double x, double y, double z);
HC_INTERFACE void HC_CDECL HC_DZoom_Camera (double zoom);


HC_INTERFACE Key HC_CDECL HC_DInsert_Text (double x, double y, double z, char const *text);
HC_INTERFACE Key HC_CDECL HC_DInsert_Text_With_Encoding (double x, double y, double z, char const *encoding, void const *text);
HC_INTERFACE void HC_CDECL HC_DShow_Text (Key key, double *x, double *y, double *z, char *text);
HC_INTERFACE void HC_CDECL HC_DShow_Text_With_Encoding (Key key, double *x, double *y, double *z, char *encoding, void *text);


HC_INTERFACE Key HC_CDECL HC_DInsert_Line (double xa, double ya, double za, double xb, double yb, double zb);
HC_INTERFACE void HC_CDECL HC_DEdit_Line (Key key, double xa, double ya, double za, double xb, double yb, double zb);
HC_INTERFACE void HC_CDECL HC_DShow_Line (Key key, double *xa, double *ya, double *za, double *xb, double *yb, double *zb);

HC_INTERFACE Key HC_CDECL HC_DInsert_Marker (double x, double y, double z);
HC_INTERFACE void HC_CDECL HC_DEdit_Marker (Key key, double x, double y, double z);
HC_INTERFACE void HC_CDECL HC_DShow_Marker (Key key, double *x, double *y, double *z);

HC_INTERFACE Key HC_CDECL HC_DInsert_Polygon (int count, DPoint const points[]);
HC_INTERFACE void HC_CDECL HC_DEdit_Polygon (Key key, int offset, int ndelete, int insert, DPoint const points[]);
HC_INTERFACE void HC_CDECL HC_DShow_Polygon (Key key, int *count, DPoint points[]);

HC_INTERFACE Key HC_CDECL HC_DInsert_Polyline (int count, DPoint const points[]);
HC_INTERFACE void HC_CDECL HC_DEdit_Polyline (Key key, int offset, int ndelete, int insert, DPoint const points[]);
HC_INTERFACE void HC_CDECL HC_DShow_Polyline (Key key, int * count, DPoint points[]);


HC_INTERFACE Key HC_CDECL HC_DInsert_Shell (
	int						point_count,
	DPoint const 			points[],
	int						face_list_length,
	int const 				face_list[]);

HC_INTERFACE void HC_CDECL HC_DShow_Shell (
	Key				key,
	int *			point_countp,
	DPoint			points[],
	int *			face_list_lengthp,
	int 			face_list[]);

HC_INTERFACE void HC_CDECL HC_DShow_Shell_By_Tristrips (
	Key				key,
	int *			point_countp,
	DPoint			points[],
	int *			tristrip_list_lengthp,
	int 			tristrip_list[],
	int *			face_list_lengthp,
	int 			face_list[]);

HC_INTERFACE void HC_CDECL HC_DEdit_Shell_Points (
	Key					key,
	int					ioffset,
	int					ndelete,
	int					insert,
	DPoint const 		points[]);

HC_INTERFACE Key HC_CDECL HC_DInsert_Circle (DPoint const *point1, DPoint const *point2, DPoint const *point3);
HC_INTERFACE Key HC_CDECL HC_DInsert_Circle_By_Radius (DPoint const *center, double radius, DVector const *normal);
HC_INTERFACE Key HC_CDECL HC_DInsert_Circular_Arc (DPoint const *point1, DPoint const *point2, DPoint const *point3);
HC_INTERFACE Key HC_CDECL HC_DInsert_Circular_Chord (DPoint const *point1, DPoint const *point2, DPoint const *point3);
HC_INTERFACE Key HC_CDECL HC_DInsert_Circular_Wedge (DPoint const *point1, DPoint const *point2, DPoint const *point3);
HC_INTERFACE void HC_CDECL HC_DEdit_Circle (Key key, DPoint const *point1, DPoint const *point2, DPoint const *point3);
HC_INTERFACE void HC_CDECL HC_DEdit_Circle_By_Radius (Key key, DPoint const *center, double radius, DVector const *vector);
HC_INTERFACE void HC_CDECL HC_DEdit_Circular_Arc (Key key, DPoint const *point1, DPoint const *point2, DPoint const *point3);
HC_INTERFACE void HC_CDECL HC_DEdit_Circular_Chord (Key key, DPoint const *point1, DPoint const *point2, DPoint const *point3);
HC_INTERFACE void HC_CDECL HC_DEdit_Circular_Wedge (Key key, DPoint const *point1, DPoint const *point2, DPoint const *point3);
HC_INTERFACE void HC_CDECL HC_DShow_Circle (Key key, DPoint *point1, DPoint *point2, DPoint *point3);
HC_INTERFACE void HC_CDECL HC_DShow_Circle_By_Radius (Key key, DPoint *center, double *radius, DVector *normal);
HC_INTERFACE void HC_CDECL HC_DShow_Circular_Arc (Key key, DPoint *point1, DPoint *point2, DPoint *point3);
HC_INTERFACE void HC_CDECL HC_DShow_Circular_Chord (Key key, DPoint *point1, DPoint *point2, DPoint *point3);
HC_INTERFACE void HC_CDECL HC_DShow_Circular_Wedge (Key key, DPoint *point1, DPoint *point2, DPoint *point3);

HC_INTERFACE Key HC_CDECL HC_DInsert_Ellipse (DPoint const *center, DPoint const *major, DPoint const *minor);
HC_INTERFACE Key HC_CDECL HC_DInsert_Elliptical_Arc (
		DPoint const *center,
		DPoint const *major,
		DPoint const *minor,
		double start,
		double end);
HC_INTERFACE void HC_CDECL HC_DEdit_Ellipse (Key key, DPoint const *center, DPoint const *major, DPoint const *minor);
HC_INTERFACE void HC_CDECL HC_DEdit_Elliptical_Arc (
		 Key key, 
		 DPoint const *center,
		 DPoint const *major,
		 DPoint const *minor,
		 double start,
		 double end);
HC_INTERFACE void HC_CDECL HC_DShow_Ellipse (Key key, DPoint *center, DPoint *major, DPoint *minor);
HC_INTERFACE void HC_CDECL HC_DShow_Elliptical_Arc (Key key,
		 DPoint *center,
		 DPoint *major,
		 DPoint *minor,
		 float *start,
		 float *end);


HC_INTERFACE Key HC_CDECL HC_DInsert_Grid (
	char const *type,
	DPoint const *origin,
	DPoint const *ref1,
	DPoint const *ref2,
	int count1,
	int count2);

HC_INTERFACE void HC_CDECL HC_DShow_Grid (
	Key key,
	char *type,
	DPoint *origin,
	DPoint *ref1,
	DPoint *ref2,
	int *count1,
	int *count2);

HC_INTERFACE void HC_CDECL HC_DEdit_Grid (
	Key key, 
	char const *type,
	DPoint const *origin,
	DPoint const *ref1,
	DPoint const *ref2,
	int count1,
	int count2);

HC_INTERFACE Key HC_CDECL HC_DInsert_Mesh (int rows, int columns, DPoint const points[]);
HC_INTERFACE void HC_CDECL HC_DShow_Mesh (Key key, int *rows, int *columns, DPoint points[]);
HC_INTERFACE void HC_CDECL HC_DEdit_Mesh (Key key, int row_offset, int column_offset, int row_count, int column_count, DPoint const points[]);

HC_INTERFACE Key HC_CDECL HC_DInsert_Image (double x, double y, double z, char const *format, int width, int height, const void *data);
HC_INTERFACE Key HC_CDECL HC_DInsert_Compressed_Image (double x, double y, double z, char const *format, int width, int height, int size, void const *data);
HC_INTERFACE void HC_CDECL HC_DShow_Image (Key key, double *x, double *y, double *z, char *format, int *width, int *height, void *data);
HC_INTERFACE void HC_CDECL HC_DShow_Image_Size (Key key, double *x, double *y, double *z, char *format, int *width, int *height);
HC_INTERFACE void HC_CDECL HC_DShow_Compressed_Image (Key key, double *x, double *y, double *z, char *format, int *width, int *height, int *size, void *data);
HC_INTERFACE void HC_CDECL HC_DMove_Image (Key key, double x, double y, double z);

HC_INTERFACE Key HC_CDECL HC_DInsert_Ink (double x, double y, double z);

HC_INTERFACE Key HC_CDECL HC_DInsert_PolyCylinder (int p_count, DPoint const pts[], int r_count, double const radii[], char const *capping);
HC_INTERFACE void HC_CDECL HC_DShow_PolyCylinder (Key key, int *p_count, DPoint pts[], int *r_count, double radii[], char *capping);
HC_INTERFACE void HC_CDECL HC_DEdit_PolyCylinder (Key key, int p_count, DPoint const pts[], int r_count, double const radii[], char const *capping);
HC_INTERFACE void HC_CDECL HC_DEdit_PolyCylinder_With_Basis (Key key, int p_count, DPoint const pts[], int r_count, double const radii[], char const *capping, DVector const basis[]);

HC_INTERFACE Key HC_CDECL HC_DInsert_Cutting_Plane (double a, double b, double c, double d);
HC_INTERFACE void HC_CDECL HC_DShow_Cutting_Plane (Key key, double *a, double *b, double *c, double *d);
HC_INTERFACE void HC_CDECL HC_DEdit_Cutting_Plane (Key key, double a, double b, double c, double d);

HC_INTERFACE Key HC_CDECL HC_DInsert_Cylinder (DPoint const *center_point_1, DPoint const *center_point_2, double radius, char const *cap);
HC_INTERFACE void HC_CDECL HC_DShow_Cylinder (Key key, DPoint *p1, DPoint *p2, double *radius, char *cap);
HC_INTERFACE void HC_CDECL HC_DEdit_Cylinder (Key key, DPoint const *p1, DPoint const *p2, double radius, char const *cap);

HC_INTERFACE Key HC_CDECL HC_DInsert_Sphere (DPoint const *center, double radius, DVector const *axis, DVector const *ortho);
HC_INTERFACE void HC_CDECL HC_DShow_Sphere (Key key, DPoint *center, double *radius, DVector *axis, DVector *ortho);
HC_INTERFACE void HC_CDECL HC_DEdit_Sphere (Key key, DPoint const *center, double radius, DVector const *axis, DVector const *ortho);


HC_INTERFACE Key HC_CDECL HC_DInsert_NURBS_Curve (
	int degree,
	int cpcount,
	DPoint const control_points[],
	float const weights[],
	float const knots[],
	double start_u,
	double end_u);

HC_INTERFACE void HC_CDECL HC_DShow_NURBS_Curve (
	Key key,
	int *degree,
	int *cp_count,
	DPoint points[],
	float weights[],
	float knots[],
	float *start_u,
	float *end_u);

HC_INTERFACE void HC_CDECL HC_DEdit_NURBS_Curve (
	Key key,
	int cp_offset,
	int knot_offset,
	int cp_count,
	int knot_count,
	DPoint const control_points[],
	float const weights[],
	float const knots[]);

HC_INTERFACE Key HC_CDECL HC_DInsert_NURBS_Surface (
	int u_degree,
	int v_degree,
	int u_count,
	int v_count,
	DPoint const points[], 
	float const weights[],
	float const u_knots[],
	float const v_knots[]);

HC_INTERFACE void HC_CDECL HC_DShow_NURBS_Surface (
	Key key,
	int *u_degree,
	int *v_degree,
	int *u_count,
	int *v_count,
	DPoint points[], 
	float weights[],
	float u_knots[],
	float v_knots[]);

HC_INTERFACE void HC_CDECL HC_DEdit_NURBS_Surface (
	Key key,
	int cp_offset,
	int weights_offset,
	int u_knot_offset,
	int v_knot_offset,
	int cp_replace_count,
	int weights_replace_count,
	int u_knot_replace_count,
	int v_knot_replace_count,
	DPoint const points[],
	float const weights[],
	float const u_knots[],
	float const v_knots[]);

HC_INTERFACE void HC_CDECL HC_DEdit_NURBS_Surface_Points (
	Key key,
	int cp_offset,
	int cp_replace_count,
	DPoint const points[]);

HC_INTERFACE Key HC_CDECL HC_DInsert_Local_Light (double x, double y, double z);
HC_INTERFACE void HC_CDECL HC_DShow_Local_Light (Key key, double *x, double *y, double *z);

HC_INTERFACE Key HC_CDECL HC_DInsert_Spot_Light (DPoint const *position, DPoint const *target, char const *list);
HC_INTERFACE void HC_CDECL HC_DShow_Spot_Light (Key key, DPoint *position, DPoint *target, char *list);

HC_INTERFACE Key HC_CDECL HC_DInsert_Area_Light (int count, DPoint const points[], char const *listptr);
HC_INTERFACE void HC_CDECL HC_DShow_Area_Light (Key key, int *ucount, DPoint upoints[], char *listptr);

HC_INTERFACE bool HC_CDECL HC_DCompute_Coordinates (
	char const *segment,
	char const *in_system,
	DPoint const *in_position,
	char const *out_system,
	DPoint *out_position);
HC_INTERFACE bool HC_CDECL HC_DCompute_Coordinates_By_Path (
	int count,
	Key const keys[],
	char const *in_system,
	DPoint const *in_position,
	char const *out_system,
	DPoint *out_position);

HC_INTERFACE void HC_CDECL HC_DCompute_Transformed_Points (int count, DPoint const points[], double const matrix[], DPoint out_points[]);

HC_INTERFACE Key HC_CDECL HC_Compute_Boolean_Shell2 (
	int						in_pt_count_target,
	Point const				in_pts_target[],
	Vector const			in_normals_target[],
	int						in_flist_len_target,
	int const 				in_flist_target[],

	int						in_pt_count_tool,
	Point const				in_pts_tool[],
	Vector const			in_normals_tool[],
	int						in_flist_len_tool,
	int const 				in_flist_tool[],

	char const *			option_string,
	RGB const *				in_color1,
	RGB const *				in_color2,
	unsigned char const 	in_vis1[],
	unsigned char const 	in_vis2[]);

HC_INTERFACE Key HC_CDECL HC_DCompute_Boolean_Shell (
	int						in_pt_count_target,
	DPoint const			in_pts_target[],
	Vector const			in_normals_target[],
	int						in_flist_len_target,
	int const 				in_flist_target[],

	int						in_pt_count_tool,
	DPoint const			in_pts_tool[],
	Vector const			in_normals_tool[],
	int						in_flist_len_tool,
	int const 				in_flist_tool[],

	char const *			option_string,
	RGB const *				in_color1,
	RGB const *				in_color2,
	unsigned char const 	in_vis1[],
	unsigned char const 	in_vis2[]);

HC_INTERFACE void HC_CDECL HC_DCompute_Optimized_Shell (
	int				pcount,
	DPoint const 	points[],
	Vector const 	normals[],
	int				flist_length,
	int const 		face_list[],
	char const *	options,
	int *			new_pcount,
	DPoint 			new_points[],
	int *			new_flist_length,
	int 			new_face_list[],
	int 			point_mapping_list[],
	int 			face_mapping_list[]);

HC_INTERFACE intptr_t HC_CDECL HC_Memory_Usage_By_Key (
	Key					key,
	char const *		which);

HC_INTERFACE void HC_CDECL HC_Apply_Attribute_Filter (
	Key					key,
	char const *		filter);

HC_INTERFACE bool HC_CDECL HC_Show_Attribute_Filter (
	Key					key,
	char *				filter);

HC_INTERFACE bool HC_CDECL HC_Show_Attribute_Filter_Length (
	Key					key,
	int *				filter_length);

HC_INTERFACE void HC_CDECL HC_Begin_Decimate_Shell (Key key);
HC_INTERFACE void HC_CDECL HC_End_Decimate_Shell (void);
HC_INTERFACE Key HC_CDECL HC_Decimate_Shell (int quality_percent, char const * options);

#endif // HPS_CORE_BUILD

#define HC_PROTO_DEFINED 1
#endif /* HC_PROTO_DEFINED */


