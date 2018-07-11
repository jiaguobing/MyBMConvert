// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef HI_PROTO_DEFINED
#define HI_PROTO_DEFINED

typedef void (HC_CDECL * Old_Triangle_Action)
				(void *info, int32_t convex_triangulation,
				 int32_t v1, int32_t v2, int32_t v3);


typedef void (HC_CDECL * Triangle_Action)
				(void *info, int convex_triangulation,
				 Point const *v1,
				 Point const *v2,
				 Point const *v3,
				 int s1,
				 int s2,
				 int s3,
				 Intersection const *d1,
				 Intersection const *d2,
				 Intersection const *d3);
typedef void (HC_CDECL * DTriangle_Action)
				(void *info, int convex_triangulation,
				 DPoint const *v1,
				 DPoint const *v2,
				 DPoint const *v3,
				 int s1,
				 int s2,
				 int s3,
				 DIntersection const *d1,
				 DIntersection const *d2,
				 DIntersection const *d3);

typedef void (* Condition_Value_Callback) (int code, void const * passthru, float & value, bool & flag);




GLOBAL_FUNCTION void HI_Create_Texture_Fallback_Image ();

GLOBAL_FUNCTION Texture * HI_Find_And_Prepare_Texture(
	Name_Const const &			texture_source_name,
	Texture_Defs_Vector const &	tdv,
	bool						create_fallback,
	Name_List *					names_seen = null,
	size_t						skip_n = 0);


GLOBAL_FUNCTION void HI_Add_Path_Delimiter (Thread_Data * thread_data, Name & directory, Name_Const const & file);
GLOBAL_FUNCTION void HI_Age_Activity (Thread_Data * thread_data, Segment *root);
GLOBAL_FUNCTION void HI_Clear_Virginity (Thread_Data * thread_data, Segment * seg, bool follow_includes = false);

GLOBAL_FUNCTION bool HI_Analyze_Arc (
		Point const *start,
		Point const *middle,
		Point const *end,
		Point *center,
		Point *basis1,
		Point *basis2,
		float *angle);
GLOBAL_FUNCTION bool HI_Analyze_Font_Data (int data_length,
		 char const *data,
		 char encoding,
		 bool using_lookup,
		 int *num_chars,
		 char const **table,
		 char const *name);
GLOBAL_FUNCTION void HI_Clean_FCD (Find_Contents_Data *fcd);
GLOBAL_FUNCTION void HI_Flush_Contents (Thread_Data * thread_data, Search_List * flush_list);
GLOBAL_FUNCTION Time HI_Assemble_3D_Polyedges(
	Thread_Data *				thread_data,
	Polyhedron *				polyhedron,
	unsigned short				please_validate,
	unsigned short				face_visibility,
	float						dihedral,
	Geometry_Highlight_Info *	highlight_info = null);
GLOBAL_FUNCTION Time HI_Assemble_3D_Polymarkers (
	Thread_Data *				thread_data,
	Polyhedron *				polyhedron,
	unsigned int				please_validate,
	Geometry_Highlight_Info *	highlight_info);
GLOBAL_FUNCTION Time HI_Assemble_3D_Tristrips (
	Thread_Data *				thread_data,
	Polyhedron *				polyhedron,
	unsigned short				please_validate,	//Composite_Face_Flags
	bool						validate_vertex_normals,
	bool						fea_nodes = false,
	Geometry_Highlight_Info *	highlight_info = null);
GLOBAL_FUNCTION void HI_Assemble_Edge_Normals (Thread_Data * thread_data, Polyhedron *polyhedron);
GLOBAL_FUNCTION void HI_Assemble_Face_Normals (Polyhedron *polyhedron);
GLOBAL_FUNCTION void HI_Assemble_Mesh_Vertex_Normals (Mesh *mesh);
GLOBAL_FUNCTION void HI_Assemble_Shell_Vertex_Normals (Shell *shell);
GLOBAL_FUNCTION void HI_Assemble_Vertex_Normals (Polyhedron *polyhedron);
GLOBAL_FUNCTION void HI_Assemble_Mesh_Tristrips (Mesh *mesh,
		 unsigned short /*Composite_Face_Flags*/ please_validate,
		 bool validate_vertex_normals, void * detailed = null);
GLOBAL_FUNCTION void HI_Assemble_Shell_Tristrips (
	Thread_Data *	thread_data,
	int						point_count,
	Point const *			points,
	int						face_list_length,
	int const *				face_list,
	Shell *					shell,
	unsigned short /*Composite_Face_Flags*/	please_validate,
	bool					validate_vertex_normals,
	Tristrip **				tristrips_out,
	bool					fea_nodes = false,
	void *					detailed = null);
GLOBAL_FUNCTION void HI_Assemble_Shell_Tristrips (
	Thread_Data *	thread_data,
	int						point_count,
	DPoint const *			points,
	int						face_list_length,
	int const *				face_list,
	Shell *					shell,
	unsigned short /*Composite_Face_Flags*/	please_validate,
	bool					validate_vertex_normals,
	Tristrip **				tristrips_out,
	bool					fea_nodes = false,
	void *					detailed = null);
GLOBAL_FUNCTION void HI_Reverse_Implicit_Normals (Polyhedron * polyhedron);


GLOBAL_FUNCTION void HI_Au_Revoir (Anything *thing);
GLOBAL_FUNCTION void HI_Bon_Voyage (Anything *	thing);

GLOBAL_FUNCTION void HI_Chain_Code_Files (void);

GLOBAL_FUNCTION Bounding * HI_Clone_Bounding (Bounding const *bounding);
GLOBAL_FUNCTION Text_Info * HI_Clone_Text_Info (Text_Info const *text_info);
GLOBAL_FUNCTION Texture * HI_Clone_Texture (Texture const *texture);

GLOBAL_FUNCTION bool HI_Close_Directory (void *dhandle, Name & message1, Name & message2);
GLOBAL_FUNCTION bool HI_Close_File (void *fhandle, Name & message1, Name & message2);

GLOBAL_FUNCTION bool HI_Compute_Three_Point_Circle (
	Point const & start,
	Point const & middle,
	Point const & end,
	Point *center,
	float *radius,
	Plane *plane,
	Vector *basis1,
	Vector *basis2);
GLOBAL_FUNCTION bool HI_Compute_Three_Point_Circle (
	DPoint const & start,
	DPoint const & middle,
	DPoint const & end,
	DPoint *center,
	double *radius,
	DPlane *plane,
	DVector *basis1,
	DVector *basis2);
GLOBAL_FUNCTION bool HI_Compute_Window_Extent_By_Path (
	Thread_Data *			thread_data,
	int						count,
	Segment const * const * segments,
	Vector2D &				resolution,
	Int_XY &				screen_pixels,
	IntRectangle &			window_pixels);
GLOBAL_FUNCTION bool HI_Convert_Bounding (Bounding const * in_bound, Bounding_Type desired, Bounding * out_bound);
GLOBAL_FUNCTION void HI_Convert_Char_To_KName (char const *cstring, KName *kname, Text_Encoding encoding);
GLOBAL_FUNCTION bool HI_Convert_Keys_To_Path (Thread_Data * thread_data, int in_count, Key const * keys, int options);
GLOBAL_FUNCTION bool HI_Decipher_Path (
	Thread_Data *		thread_data,
	int					count,
	Key const *			keys,
	int					convert_options,
	Segment const * &	segment,
	Include_Path &		path,
	Geometry const * &	geometry,
	Key &				geometry_key,
	int &				flags);
GLOBAL_FUNCTION bool HI_Convert_Precision (Thread_Data * thread_data, int choice, Anything * geometry, int offset = -1);
GLOBAL_FUNCTION bool HI_Convert_Bounding_Precision (int choice, Bounding & bounding);
GLOBAL_FUNCTION char * HI_Copy_Chars (char const *in, char *out);
GLOBAL_FUNCTION void HI_Copy_KName (KName const *in, KName *out);

GLOBAL_FUNCTION Option_Value * HI_Copy_Option_List (Option_Value const *old_opt);
GLOBAL_FUNCTION void HI_Count_Contents (Thread_Data * thread_data, Segment * seg, Find_Contents_Data * fcd);
GLOBAL_FUNCTION long HI_Count_Transparent_Pixels (int width, char format, void *iraster);
GLOBAL_FUNCTION Image * HI_Create_Image (
	Point const * where,
	int width,
	int height,
	char format,
	int row_bytes,
	int type,
	unsigned char const *source,
	int source_size,
	bool cached_memory);
GLOBAL_FUNCTION Image * HI_Create_Image (
	DPoint const * where,
	int width,
	int height,
	char format,
	int row_bytes,
	int type,
	unsigned char const *source,
	int source_size,
	bool cached_memory);
GLOBAL_FUNCTION Image * HI_Create_Image (
	int width,
	int height,
	char format,
	int row_bytes,
	int type,
	unsigned char const *source,
	int source_size,
	bool cached_memory);
GLOBAL_FUNCTION Segment * HI_Create_One_Segment (
	Thread_Data *		thread_data,
	Name_Const const &	name,
	Subsegment **		linker,
	Subsegment *		pred_subseg,
	Segment *			owner,
	bool				fix_instances);

GLOBAL_FUNCTION int HI_Create_Polyglin_Wings (int count, Point *points, bool closed, int owner, Wing ***wing_ptrs);
GLOBAL_FUNCTION Wedge_Tree * HI_Create_Polygon_Wedge_Tree (int count, Point * points, Point2D & pivot, bool ccw);
GLOBAL_FUNCTION Segment * HI_Create_Segment (
	Thread_Data *	thread_data,
	Segment * owner,
	char const *name,
	bool create_if_needed);
GLOBAL_FUNCTION void HI_Create_Shell_Edges(Thread_Data * thread_data, Shell *shell, bool create_edge_to_face, bool create_vertex_to_edge);
GLOBAL_FUNCTION void HI_Retrieve_Vertices_From_Edge(Thread_Data * thread_data, Polyhedron const * polyhedron, int edge_index, int & v1, int & v2);
GLOBAL_FUNCTION int HI_Retrieve_Edge_From_Vertices(Thread_Data * thread_data, Polyhedron const * polyhedron, int v1, int v2, bool complain = false);
GLOBAL_FUNCTION Event * HI_Current_Event (Thread_Data * thread_data, DB_Type type);
GLOBAL_FUNCTION int HI_Decipher_Action (char const *iaction_name);
GLOBAL_FUNCTION bool HI_Decipher_Color_Target (
	Thread_Data * thread_data,
	char const **cpp,
	char const *cpe,
	bool comma_OK,
	Color_Object * newobject,
	Color_Object default_geometry);
GLOBAL_FUNCTION Option_Value * HI_Decipher_Color_Type (Thread_Data * thread_data, char const *type, int extra_flags);
GLOBAL_FUNCTION Option_Value * HI_Decipher_Color_Channel (Thread_Data * thread_data, char const *type, int extra_flags);
GLOBAL_FUNCTION char HI_Decipher_Encoding (Thread_Data * thread_data, char const *encoding);
GLOBAL_FUNCTION bool HI_Decipher_Handedness (char const *ispec, int *which);
GLOBAL_FUNCTION void HI_Decipher_Metafile_Options (Thread_Data * thread_data, char const *listptr, Metafile *proto);
GLOBAL_FUNCTION bool HI_Decipher_Type_Names (Thread_Data *	thread_data, char const *types, Find_Contents_Data *fcd);
GLOBAL_FUNCTION bool HI_Decipher_Unicode_Type_Names (Thread_Data * thread_data, Karacter const * types, Find_Contents_Data * fcd);
GLOBAL_FUNCTION void HI_Define_Constant_Options (Constant_Option_Type const *constant_table,
		 int constant_table_size,
		 Option_Hash_Table *hash_tablep);

GLOBAL_FUNCTION Font * HI_Define_Font (
	Thread_Data *	thread_data,
	char const *			name,
	char const *			options,
	int						data_length,
	char const *			data,
	bool					copy_data);
GLOBAL_FUNCTION void HI_Define_Internal_Fonts (Thread_Data * thread_data);
GLOBAL_FUNCTION Option_Type * HI_Define_Nice_Option (
	Option_Hash_Table *	hash_table,
	int32_t				id,
	const Name_Const &	name,
	int					value_type,
	int					value_count,
	bool				negatable,
	Option_Hash_Table *	suboptions);
GLOBAL_FUNCTION KOption_Type * HI_Define_Nice_KOption (
	Option_Hash_Table *	hash_table,
	int32_t				id,
	const KName *		name,
	int					value_type,
	int					value_count,
	bool				negatable,
	Option_Hash_Table *	suboptions);
GLOBAL_FUNCTION void HI_Define_Implicit_Texture(Thread_Data *	thread_data, Segment const *seg, Image *image, Option_Value *options_list);
GLOBAL_FUNCTION Texture * HI_Define_Shadow_Texture(Thread_Data * thread_data, Image *image);
GLOBAL_FUNCTION Glyph HI_Define_Glyph (Name_Const const & name, int length, char const * definition);
GLOBAL_FUNCTION void HI_Delete_Segment (Thread_Data *	thread_data, Segment *seg);
GLOBAL_FUNCTION void HI_Delete_User_Key (Thread_Data * thread_data, Anything * item, Key item_key = ERROR_KEY, Anything * owner = null);
GLOBAL_FUNCTION char HI_Determine_Image_Format (Thread_Data * thread_data, char const *format, void *image_passthru);
GLOBAL_FUNCTION void HI_Disable_Timer_Interrupts (void);
GLOBAL_FUNCTION void HI_Dump_Code (char const *string);
GLOBAL_FUNCTION void HI_Edit_Karacters (Thread_Data *	thread_data, Actor *source, int key, char bencoding);
GLOBAL_FUNCTION void HI_Edit_Text (
	Thread_Data *	thread_data,
	Text *text,
	int row,
	int column,
	int erase,
	int ndelete,
	int insert,
	unsigned short const * new_kars);
GLOBAL_FUNCTION bool HI_Enable_Timer_Interrupts (void);
GLOBAL_FUNCTION int HI_Encoding_Is_Compatible (char from, char to);
GLOBAL_FUNCTION bool HI_Convert_Karacters_Encoding (size_t count, unsigned short const* in, unsigned short * out, char from, char to);
GLOBAL_FUNCTION void HI_Error_Exit (void);

GLOBAL_FUNCTION Named_Material_Handle HI_Evaluate_Material(
	Thread_Data *			thread_data,
	Name_Const const &				descrip,
	Texture_Defs_Vector const *		tdv,
	bool							errors,
	bool							want_rgb = true);

GLOBAL_FUNCTION void HI_Figure_Segment_Bounding (Thread_Data * thread_data, Segment const * seg, Bounding & bounding, Bounding & geometry_bounding);
GLOBAL_FUNCTION void HI_Refresh_Segment_Bounding (Thread_Data * thread_data, Segment * seg,
												  bool ignore_camera = false, bool probe_3d_through_camera = false);
GLOBAL_FUNCTION bool HI_Find_Application_Name (Name & name);
GLOBAL_FUNCTION Attribute * HI_Find_Attribute (Thread_Data * thread_data, Anything const * item, DB_Type type, bool complain = true);
GLOBAL_FUNCTION void HI_Find_Kanji_Char (char const *font, char const *data, unsigned short index, unsigned char const ** address);

GLOBAL_FUNCTION Attribute * HI_Find_Local_Attribute (Thread_Data * thread_data, Open_Item const * open_item, DB_Type type);
GLOBAL_FUNCTION Attribute * HI_Find_Net_Attribute (
	Thread_Data *					thread_data,
	Anything const *				item,
	DB_Type							type,
	int								faf_flags=0,
	Find_Attribute_Control const *	control = null);
GLOBAL_FUNCTION Attribute * HI_Find_Net_Attribute_By_Path (
	Thread_Data *					thread_data,
	int								count,
	Segment const * const *			segments,
	DB_Type							type,
	int								faf_flags=0,
	Find_Attribute_Control const *	control = null);
GLOBAL_FUNCTION Attribute * HI_Find_Attribute_And_Lock (
	Thread_Data *					thread_data,
	Open_Type						open_type_flags,
	int								faf_flags=0,
	int								count=0,
	Key const *						keys=null,
	Anything **						target=null,
	Find_Attribute_Control const *	control=null);

GLOBAL_FUNCTION Named_Style HI_Find_Named_Style (
	Named_Style_Vector_Pooled const &	named_styles,
	Name_Const const &					name,
	bool								error);

GLOBAL_FUNCTION void HI_Find_Polygon_Plane (Polygon const * polygon);
GLOBAL_FUNCTION unsigned short HI_Find_Raster_Max (unsigned short max, int width, char format, void *iraster);
GLOBAL_FUNCTION void HI_Find_Bounding (Thread_Data * thread_data, Anything const * item, Bounding & bounds);
GLOBAL_FUNCTION Window * HI_Find_Scoping_Window (Segment const * seg);
GLOBAL_FUNCTION void HI_Finish_Code_Generation (void);
GLOBAL_FUNCTION void HI_Flush_Geometry (Thread_Data *	thread_data, Segment *seg);
GLOBAL_FUNCTION void HI_Really_Flush_Geometry (Thread_Data * thread_data, Segment *seg, int32_t flags);
GLOBAL_FUNCTION void HI_Flush_Driver_Config (char const *driver, char const *id, char const *version);
GLOBAL_FUNCTION bool HI_Font_Names_All_Equal (Font_Name_List const & a, Font_Name_List const & b);

GLOBAL_FUNCTION Actor * HI_Force_Find_Actor (Thread_Data * thread_data, char const *name);
GLOBAL_FUNCTION Actor * HI_Force_Find_Actor (Thread_Data * thread_data, size_t count, Segment const * const * segments);
GLOBAL_FUNCTION void HI_Format_Generic_Size_String (Generic_Size const *size, char *list, int listlen);
GLOBAL_FUNCTION void HI_Free_Bounding (Bounding *bounding);
GLOBAL_FUNCTION void HI_Free_Include_Chain (Include_Chain *ic);
GLOBAL_FUNCTION Include_Chain * HI_Copy_Include_Chain (Include_Chain * old_chain);
GLOBAL_FUNCTION HOOPS::Selection_Item * HI_Copy_Selection_Item(HOOPS::Selection_Item const * in_item);
GLOBAL_FUNCTION void HI_Free_Kanji_Data (void);
GLOBAL_FUNCTION void HI_Free_Option_List (Option_Value *listptr);
GLOBAL_FUNCTION void HI_Free_Path (Path_Token *tok);
GLOBAL_FUNCTION void HI_Free_Time (bool shutdown_thread_only);
GLOBAL_FUNCTION void HI_Free_Wedge_Tree (Wedge_Tree *tree);
GLOBAL_FUNCTION void HI_Free_Wings (int count, Wing **wing_ptrs);
GLOBAL_FUNCTION void HI_Free_Text_Info (Text_Info * text_info);
GLOBAL_FUNCTION bool HI_Gather_Capping_Geometry (Thread_Data * thread_data, Gather_Capping_Geometry * gcg);
GLOBAL_FUNCTION void HI_Gather_Modelling (Attribute *attribute, Modelling_Matrix *&	net);
GLOBAL_FUNCTION void HI_Generate_Error (
	Thread_Data *	thread_data, 
	HEC				category,
	HES				specific,
	Error_Severity	severity,
	int				msgc,
	char const **	msgv);

GLOBAL_FUNCTION void HI_Generate_Memory_Limit_Error(
	char const *	message,
	bool			hard_limit);

GLOBAL_FUNCTION void HI_Generic_Delete (Thread_Data *thread_data, Anything *stuff);

GLOBAL_FUNCTION bool HI_Get_Filename (Segment *seg, Metafile *metafile, File_Data *fdata, Name_Const const & defdir);


GLOBAL_FUNCTION void HI_HLS_To_RGB (HLS const *hls, RGB *rgb);
GLOBAL_FUNCTION bool HI_Visualize_3DF_Version (char *data, int datalen);
GLOBAL_FUNCTION bool HI_Has_Local_Key (Segment const * owner, Key item_key);
GLOBAL_FUNCTION void HI_Attach_Automatic_Driver (Thread_Data * thread_data, Segment * seg);
GLOBAL_FUNCTION Actor * HI_Create_Actor(Thread_Data * thread_data, Segment * seg, Driver_Action action, uint32_t in_hps_driver_type=0);
GLOBAL_FUNCTION void HI_Init_Internal_Data (void);
GLOBAL_FUNCTION void HI_Init_Time (void);

GLOBAL_FUNCTION Geometry * HI_Insert_Geometry (
	Thread_Data *	thread_data,
	Anything *		owner,
	Geometry *		geometry,
	Activity		activity,
	bool			is_new = true);

GLOBAL_FUNCTION Polyline *	HI_Generate_NURBS_Polyline (
	Thread_Data *				thread_data,
	NURBS_Curve const *			curve,
	NURBS_Curve_Control const &	control,
	Matrix						matrix=null);
GLOBAL_FUNCTION void HI_Interpret_Environment (Thread_Data * thread_data, bool picture_only);

GLOBAL_FUNCTION int HI_Intersect_Line_Segments_2D (
	Point const &	p0,
	Point const &	p1,
	Point const &	q0,
	Point const &	q1,
	Point &			intersection);

GLOBAL_FUNCTION Callback_Name HI_Find_Callback_Name (Thread_Data * thread_data, Name_Const const & name);

GLOBAL_FUNCTION Font ** HI_Look_Up_Font (Thread_Data * thread_data, Name_Const const &	name);
GLOBAL_FUNCTION Texture const * HI_Look_Up_Texture (Thread_Data * thread_data, Segment const * seg, Name_Const const & name);
GLOBAL_FUNCTION void HI_LEA_Pattern_Delete (Polyhedron *polyhedron, LEA_Pattern *pattern);
GLOBAL_FUNCTION LEA_Pattern * HI_LEA_Pattern_Init (Polyhedron * polyhedron, Name_Const const & name);
GLOBAL_FUNCTION LEA_Pattern * HI_LEA_Pattern_Lookup (Polyhedron const *polyhedron, unsigned char id);
GLOBAL_FUNCTION void HI_LVA_Symbol_Delete (Polyhedron *polyhedron, LVA_Symbol *symbol);
GLOBAL_FUNCTION LVA_Symbol * HI_LVA_Symbol_Init (Polyhedron * polyhedron, Name_Const const & name);
GLOBAL_FUNCTION LVA_Symbol * HI_LVA_Symbol_Lookup (Polyhedron const *polyhedron, unsigned char id);

GLOBAL_FUNCTION void HI_Make_Unique_Filename (Name & name);

GLOBAL_FUNCTION void HI_Normal_Exit (void);
GLOBAL_FUNCTION void HI_Not_When_Streamed (Segment const * seg, char const * why);
GLOBAL_FUNCTION Segment * HI_One_Segment_Search (
	Thread_Data *		thread_data,
	Name_Const const &	name,
	bool				required);
GLOBAL_FUNCTION bool HI_Open_Directory (
	Name_Const const &	name,
	Name_Const const &	filter,
	int					selector_mask,
	int					selector_value,
	void **				dhandle,
	Name &				message1,
	Name &				message2);
GLOBAL_FUNCTION bool HI_Open_File (
	Name_Const const &	name,
	int					file_access_mode,
	void **				fhandle,
	bool				create_new_version,
	Name &				message1,
	Name &				message2);
GLOBAL_FUNCTION void HI_Optimize_Segment_Tree (
	Optimize_Control &	cc,
	Segment *			seg);


GLOBAL_FUNCTION bool HI_Parse_Color_Map (Thread_Data * thread_data, char const *in_cnames, Color_Map *proto);
GLOBAL_FUNCTION bool HI_Parse_Color_Map_By_Value (Thread_Data * thread_data, char const *style, int count, RGB const *in, Color_Map *proto);

GLOBAL_FUNCTION void HI_Parse_Filename (
	Name_Const const &	name,
	Name &				directory,
	Name &				file,
	Name &				extension);
GLOBAL_FUNCTION bool HI_Parse_Options (
	Thread_Data *		thread_data,
	char const *		ioptions_string,
	Option_Hash_Table *	hash_table,
	Option_Value **		output_listp,
	int					flags);
GLOBAL_FUNCTION bool HI_Parse_KOptions (
	Thread_Data *			thread_data,
	unsigned short const *	ioptions_string,
	Option_Hash_Table *		hash_table,
	Option_Value **			output_listp,
	int						flags);
GLOBAL_FUNCTION Path_Token * HI_Parse_Path (
	Thread_Data *		thread_data,
	char const *		inspec,
	int					flags,
	Segment *			owner = null);
GLOBAL_FUNCTION void HI_Point_On_NURBS_Curve (
	Thread_Data *		thread_data,
	NURBS_Curve const *	curve,
	float				uu,
	Point &				point);
GLOBAL_FUNCTION void HI_Point_On_NURBS_Curve (
	Thread_Data *		thread_data,
	NURBS_Curve const *	curve,
	float				uu,
	DPoint &			point);
GLOBAL_FUNCTION void HI_Propagate_Activity (
	Thread_Data *		thread_data,
	Segstuff *			stuff,
	Activity			which);
GLOBAL_FUNCTION void HI_Propagate_Bounding (Thread_Data * thread_data, Segment *seg, Bounding const & bounding, Segment const *transform_seg);

GLOBAL_FUNCTION void HI_Antiquate_Bounding (
	Thread_Data *		thread_data,
	Segment *			seg,
	bool				inclusive=true,
	bool				and_geometry=true);

GLOBAL_FUNCTION void HI_Propagate_Seen(
	Thread_Data *		thread_data,
	Segment const *		seg,
	Segment *			oldowner,
	Segment *			newowner);

GLOBAL_FUNCTION Seen_Flags HI_Scan_Geometry_Seen(
	Thread_Data *		thread_data,
	Geometry const *	geo,
	int					geometry_mask = ~0,
	bool				single = true);

GLOBAL_FUNCTION Seen_Flags HI_Scan_Attribute_Seen(
	Thread_Data *		thread_data,
	Attribute const *	attr,
	bool				single);

GLOBAL_FUNCTION Seen_Flags HI_Freshen_Seen(Thread_Data * thread_data, Segment const * seg);

GLOBAL_FUNCTION void HI_Propagate_Maybes(Thread_Data * thread_data, Segment *seg, Seen_Flags what);
GLOBAL_FUNCTION void HI_Propagate_Reference_Maybes(Thread_Data * thread_data, Segment *seg, Seen_Flags what);

GLOBAL_FUNCTION void HI_Propagate_Scoped_Activity(Thread_Data *thread_data, Segstuff *stuff, Activity which, Rectangle const *scope);



GLOBAL_FUNCTION void HI_RGB_To_HLS (RGB const *rgb, HLS *hls);
GLOBAL_FUNCTION int HI_Rank_Polygon (Point const *points, int const *face_list, int const *face_list_end);
GLOBAL_FUNCTION int HI_Rank_Polygon (DPoint const *points, int const *face_list, int const *face_list_end);
GLOBAL_FUNCTION int HI_Rank_Polygon (int count, Point const *points);
GLOBAL_FUNCTION int HI_Rank_Polygon (int count, DPoint const *points);
GLOBAL_FUNCTION bool HI_Read_Directory (
	void *		dhandle,
	Name &		filename,
	Name &		message1,
	Name &		message2);
GLOBAL_FUNCTION bool HI_Read_File_Buffer (
	void *		fhandle,
	char *		buffer,
	size_t *	bytes_read,
	size_t		maxbytes,
	Name &		message1,
	Name &		message2);
GLOBAL_FUNCTION void HI_Disentangle (Thread_Data * thread_data, Anything *thing);
GLOBAL_FUNCTION void HI_Really_Report_Error (int category, int specific, int severity, int msgc, char **msgv, int c, char **v);
GLOBAL_FUNCTION void HI_Record_Texture_Definition(Thread_Data * thread_data, Segment * seg, Texture * texture);


GLOBAL_FUNCTION bool HI_UnDefine_Texture (
	Thread_Data *		thread_data,
	Segment const *		seg,
	Name_Const const &	defname,
	bool				complain=true);

GLOBAL_FUNCTION void HI_Redraw_Attribute (Thread_Data * thread_data, Segment *seg, DB_Type type, Attribute *old_attr, Attribute *new_attr);
GLOBAL_FUNCTION void HI_Redraw_Style (
	Thread_Data *		thread_data,
	Segment *			seg,
	Attribute *			attribute);
GLOBAL_FUNCTION void HI_Delete_Style_Attributes (Thread_Data * thread_data, Segment * seg, Style *	style);
GLOBAL_FUNCTION Key HI_Reference_Geometry (
	Thread_Data *		thread_data,
	char const *		include_name,
	Segment *			include_seg,
	Segment *			target_seg,
	char const *		condition);
GLOBAL_FUNCTION Key HI_Rename_Segment (
	Thread_Data *		thread_data,
	Segment *			seg,
	Segment *			newowner,
	Subsegment **		linker,
	Subsegment *		pred_subseg,
	Name_Const const &	newname,
	bool				check_owner,
	bool				exists,
	void *				info);
GLOBAL_FUNCTION void HI_Rename_To_Nameless (
	Thread_Data *		thread_data,
	Segment *			seg);
GLOBAL_FUNCTION void HI_Report_File_Error (File_Data *fdata);
GLOBAL_FUNCTION void HI_Report_File_Error_With_Names (File_Data *fdata, Name & mess1, Name & mess2);



GLOBAL_FUNCTION bool HI_Return_Chars (char *out, int outlen, char const *in, size_t inlen);
GLOBAL_FUNCTION bool HI_Return_Chars (char *out, int outlen, Name_Const const & name);
GLOBAL_FUNCTION void HI_Return_Sprintf1 (char *bp, int length, char const *spec, void const *item);

GLOBAL_FUNCTION char const * HI_Scan_Boolean (Thread_Data * thread_data, char const **cpp, char const *cpe, bool *resultp);
GLOBAL_FUNCTION char const * HI_Scan_Float (Thread_Data * thread_data, char const **cpp, char const *cpe, float *resultp);
GLOBAL_FUNCTION char const * HI_Scan_Float (Thread_Data * thread_data, char const **cpp, char const *cpe, double *resultp);
GLOBAL_FUNCTION char const * HI_Scan_Integer (Thread_Data * thread_data, char const **cpp, char const *cpe, int *resultp);
GLOBAL_FUNCTION char const * HI_Scan_Long (Thread_Data * thread_data, char const **cpp, char const *cpe, long *resultp);
GLOBAL_FUNCTION char const * HI_Scan_Key (Thread_Data * thread_data, char const **cpp, char const *cpe, Key *resultp);
GLOBAL_FUNCTION char const * HI_Scan_Ptr (Thread_Data * thread_data, char const **cpp, char const *cpe, intptr_t *resultp);
GLOBAL_FUNCTION char const * HI_Scan_Name (Thread_Data * thread_data, char const **cpp, char const *cpe, Name * name);
GLOBAL_FUNCTION char const * HI_Scan_String (Thread_Data * thread_data, char const **cpp, char const *cpe, Name * name);
GLOBAL_FUNCTION char const * HI_Scan_Generic_Size(Thread_Data * thread_data, Generic_Size *size, char const *options, char const *type);
GLOBAL_FUNCTION void HI_Set_And_Clear_Flags (Thread_Data * thread_data, Segment *segment, Set_And_Clear_Flags_Control *	data);
GLOBAL_FUNCTION bool HI_Set_Attribute (Thread_Data * thread_data, Anything *node, DB_Type type, Attribute_Acquisitor_Action acquire, Attribute * attribute);
GLOBAL_FUNCTION void HI_Set_Default_Attributes (Thread_Data *	thread_data);
GLOBAL_FUNCTION void HI_Flush_Derived_Normals (Thread_Data * thread_data, Polyhedron *polyhedron);
GLOBAL_FUNCTION void HI_Set_User_Key (Thread_Data * thread_data, Anything * item, int offset, Key key, bool make_it_local);

GLOBAL_FUNCTION void HI_Show_Encoding_Type (char *string, int length, Text_Encoding encoding);


GLOBAL_FUNCTION void HI_Show_Image_Data (
	Thread_Data *	thread_data,
	Image *			image,
	void *			bytes);


GLOBAL_FUNCTION bool HI_Snapshot (
	Thread_Data *	thread_data,
	Actor const *	actor,
	int				bpp,
	int				width,
	int				height,
	void *			bytes);



GLOBAL_FUNCTION void HI_Sleep (Time ticks);
GLOBAL_FUNCTION bool HI_Is_Name_Special(Name const & name, bool pathname_component, bool consider_uppercase_simple);
GLOBAL_FUNCTION char * HI_Sprintf1 (char *bp, char const *bpe, char const *spec, void const *ptr);

GLOBAL_FUNCTION char * HI_Sprintf4 (
	Thread_Data *	thread_data,
	char *			buf,
	char const *	spec,
	int				int1,
	int				int2,
	void const *	ptr1,
	void const *	ptr2);

GLOBAL_FUNCTION void HI_Start_Code_Generation (void);
GLOBAL_FUNCTION void HI_Start_Hoops (char const *function_name, bool minimal);
GLOBAL_FUNCTION void HI_Start_System_Specific (Thread_Data *thread_data);
GLOBAL_FUNCTION void HI_Stop_System_Specific (void);
GLOBAL_FUNCTION void HI_Stop_System_Specific_Threading (void);
GLOBAL_FUNCTION bool HI_System_Allows_Default_Driver (uint32_t hps_driver_type);

enum Style_Segment_Control : int32_t {
	SSC_None			= 0x00,
	SSC_AddFirst		= 0x01,
	SSC_Portfolio		= 0x02,
	SSC_Append_Or		= 0x10,
	SSC_Append_And		= 0x20,
};

GLOBAL_FUNCTION Key HI_Style_Segment (
	Thread_Data *			thread_data,
	Segment const *			referee,
	Name *					named_style,
	Segment *				target_seg,
	Conditional const *		condition,
	Style_Segment_Control	ssc = SSC_None);

GLOBAL_FUNCTION int HI_Take_A_Poll (Thread_Data *	thread_data, Actor const *driver, Time now, bool single_actor);
GLOBAL_FUNCTION void HI_Polygon_Triangle_Collector (Polygon_Collector_List & collector, int32_t v1, int32_t v2, int32_t v3);
GLOBAL_FUNCTION void HI_Polygon_Merge (Thread_Data * thread_data, Polygon_Collector_List & collector);
GLOBAL_FUNCTION bool HI_Triangulate_Face (
	Thread_Data *		thread_data,
	Point const *		points,
	Vector const *		normal,
	int const *			face_list,
	int const *			face_list_end,
	Old_Triangle_Action	triangle_action,
	void *				action_info);
GLOBAL_FUNCTION bool HI_Triangulate_Face (
	Thread_Data *		thread_data,
	DPoint const *		points,
	Vector const *		normal,
	int const *			face_list,
	int const *			face_list_end,
	Old_Triangle_Action	triangle_action,
	void *				action_info);
GLOBAL_FUNCTION bool HI_Triangulate_Face_X (
	Thread_Data *		thread_data,
	Point const *		points,
	Vector const *		normal,
	int const *			face_list,
	int const *			face_list_end,
	Triangle_Action		triangle_action,
	void *				action_info);
GLOBAL_FUNCTION bool HI_Triangulate_Face_X (
	Thread_Data *		thread_data,
	DPoint const *		points,
	Vector const *		normal,
	int const *			face_list,
	int const *			face_list_end,
	DTriangle_Action	triangle_action,
	void *				action_info);

GLOBAL_FUNCTION bool HI_UnDefine_Texture (Thread_Data * thread_data, Texture * texture);
GLOBAL_FUNCTION void HI_Update_Actor_List (Thread_Data * thread_data);
GLOBAL_FUNCTION void HI_Update_Events (Thread_Data * thread_data);

GLOBAL_FUNCTION bool HI_Validate_Face_List (int point_count, int face_list_length, int const *face_list, int *face_countp);
GLOBAL_FUNCTION bool HI_Validate_Glyph (Counted_Pointer<Glyph_Data> & glyph);

GLOBAL_FUNCTION bool HI_Validate_Line_Style (Thread_Data * thread_data, Counted_Pointer<Line_Style_Data> & line_style);
GLOBAL_FUNCTION void HI_Generate_Bit_Pattern(
	Line_Style &				line_style,
	Line_Style_Parallel *		parallel,
	int							result_length);

GLOBAL_FUNCTION void HI_Validate_Inverted_Rasters (Image *image);
GLOBAL_FUNCTION void HI_Vertex_Complaint (Polyhedron const * polyhedron, int offset);
GLOBAL_FUNCTION int HI_Wings_Intersect (int victim_count, Wing **victims, int aperture_count, Wing **apertures);
GLOBAL_FUNCTION bool HI_Would_Be_Include_Loop (Segment *owner, Segment *child);
GLOBAL_FUNCTION bool HI_Write_File_Buffer (void *fhandle,
	char const *	buffer,
	size_t			bytes,
	Name &			message1,
	Name &			message2);

GLOBAL_FUNCTION bool HI_Face_List_From_Tristrips (Thread_Data * thread_data, Shell *shell);
GLOBAL_FUNCTION void HI_Tessellate_NURBS_Surface (
	Thread_Data *					thread_data,
	NURBS_Surface const *			surf,
	NURBS_Surface_Config const &	configs);


GLOBAL_FUNCTION void HI_Free_Region_Set (
		UV_Region_Set *set);
GLOBAL_FUNCTION UV_Region_Set * HI_Generate_Convex_Region_Set (
	int				count,
	Point2D const *	uv_points);
GLOBAL_FUNCTION UV_Region_Set * HI_Generate_Complex_Region_Set (
	Thread_Data *	thread_data,
	int				point_count,
	Point2D const *	uv_points,
	int				face_list_length,
	int const *		face_list,
	bool			no_merge);
GLOBAL_FUNCTION UV_Region_Set * HI_Generate_Polygonal_Region_Set (
	Thread_Data *	thread_data,
	int				count,
	Point2D const *	points,
	bool			no_merge);
GLOBAL_FUNCTION UV_Region_Set * HI_Generate_Rectangular_Regions (
	float			start_u,
	float			end_u,
	float			start_v,
	float			end_v,
	int				u_divisions,
	int				v_divisions);
GLOBAL_FUNCTION void HI_Subtract_Region_From_Region_Set (
	UV_Region_Set *	set,
	UV_Region_Set *	removal_set); /* const */
GLOBAL_FUNCTION void HI_Convert_Regions_To_Facelist (
	UV_Region_Set const *	set,
	int **					listptr,
	int *					length,
	int *					count);

GLOBAL_FUNCTION void HI_Trim_NURBS_Surface_By_Curve (
	Thread_Data *	thread_data,
	NURBS_Surface *			surface,
	UV_Trim **				rootptr,
	int						degree,
	int						control_point_count,
	float const *			control_points,
	float const *			weights,
	float const *			knots);

GLOBAL_FUNCTION void HI_Trim_NURBS_Surface_By_Poly (
	Thread_Data *	thread_data,
	NURBS_Surface *			surface,
	UV_Trim **				rootptr,
	int						count,
	float const *			points);

GLOBAL_FUNCTION void HI_Delete_Trim (UV_Trim *node);


GLOBAL_FUNCTION void HI_Not_When_LOD (void);
GLOBAL_FUNCTION void HI_Resort_Tristrips (Thread_Data * thread_data, Polyhedron * polyhedron, int region);
GLOBAL_FUNCTION void HI_Delete_Shell_Selection_Cache (void *cache);
GLOBAL_FUNCTION void *HI_New_Shell_Selection_Cache_F (Thread_Data * thread_data, Memory_Pool * memory_pool);
GLOBAL_FUNCTION void *HI_New_Shell_Selection_Cache_D (Thread_Data * thread_data, Memory_Pool * memory_pool);
GLOBAL_FUNCTION void HI_Delete_Intersect_Polyline_Cache (void *cache);
GLOBAL_FUNCTION void *HI_New_Point_Inside_Shell_Cache (void *prev = null);
GLOBAL_FUNCTION void *HI_Delete_Point_Inside_Shell_Cache (void *cache);


GLOBAL_FUNCTION int HI_Compute_Points_Inside_Shell_F (
	Thread_Data *	thread_data,
	int						test_point_count,
	Point_3D<float> const *	test_points,
	float					tolerance,
	int						point_count,
	Point_3D<float> const *	points,
	int						face_list_length,
	int const *				face_list,
	bool					identify_faces,
	int *					results,
	void *					external_cache = null);

GLOBAL_FUNCTION int HI_Compute_Points_On_Shell_F (
	Thread_Data *	thread_data,
	int						test_point_count,
	Point_3D<float> const *	test_points,
	float					tolerance,
	int						point_count,
	Point_3D<float> const *	points,
	int						face_list_length,
	int const *				face_list,
	int *					results,
	void *					external_cache = null);

GLOBAL_FUNCTION int HI_Compute_Points_Distance_From_Shell_F (
	Thread_Data *	thread_data,
	int						test_point_count,
	Point_3D<float> const *	test_points,
	float					tolerance,
	int						point_count,
	Point_3D<float> const *	points,
	int						face_list_length,
	int const *				face_list,
	float *					results,
	int *					face_results,
	void *					external_cache = null);



GLOBAL_FUNCTION PolyCylinder_Attributes * HI_Initialize_PCA_Vertex_Attributes (
						PolyCylinder * pc, unsigned char which);

GLOBAL_FUNCTION void HI_Enumerate_Region_Set_Edges (
		UV_Region_Set const *set,
		int *count,
		UV_Edge ***edges);
GLOBAL_FUNCTION void HI_Split_Region_Set_Edge (
		UV_Region_Set *set,
		UV_Edge *edge,
		int *count,
		UV_Edge ***edges);
GLOBAL_FUNCTION void HI_Midpoint_Split_Triangle (
		UV_Region_Set *set,
		UV_Region *region,
		int *affected_count_out,
		UV_Region ***affected_regions_out);
GLOBAL_FUNCTION void HI_Triangulate_Region (
		UV_Region_Set *set,
		UV_Region *region);
GLOBAL_FUNCTION void HI_Midpoint_Split_One_Triangle (
		UV_Region_Set *set,
		UV_Edge *edge,
		int *count,
		UV_Edge ***edges);
GLOBAL_FUNCTION UV_Region_Set * HI_Generate_Triangular_Regions (
		float			start_u,
		float			end_u,
		float			start_v,
		float			end_v,
		int				u_divisions,
		int				v_divisions);
GLOBAL_FUNCTION void HI_Get_UV_Region_Triangle (
		UV_Region *	region,
		UV_Edge **	a,
		UV_Edge **	b,
		UV_Edge **	c);

GLOBAL_FUNCTION int HI_Compute_Intersect_Polyline (
	Thread_Data *	thread_data,
	int point_count1,
	Point const *points1,
	int face_list_length1,
	int const *face_list1,
	int point_count2,
	Point const *points2,
	int face_list_length2,
	int const *face_list2,
	int *polyline_count,
	int *polyline_lengths,
	int *polyline_point_count,
	Point *polyline_points,
	bool save_result);
GLOBAL_FUNCTION int HI_Compute_Intersect_Polyline (
	Thread_Data *	thread_data,
	int point_count1,
	DPoint const *points1,
	int face_list_length1,
	int const *face_list1,
	int point_count2,
	DPoint const *points2,
	int face_list_length2,
	int const *face_list2,
	int *polyline_count,
	int *polyline_lengths,
	int *polyline_point_count,
	DPoint *polyline_points,
	bool save_result);


GLOBAL_FUNCTION void HI_Accumulate_Segment_Bounding (
	Thread_Data *		thread_data,
	Segment const *		seg,
	Bounding &			bounding,
	Matrix const &		matrix,
	Type_Flags			start_vis,
	HFSB_Flags			flags);

GLOBAL_FUNCTION void HI_Reset_Strip_Flags (
	Thread_Data *		thread_data,
	Polyhedron *		polyhedron,
	int					which = ~0);

GLOBAL_FUNCTION void HI_Reset_Driver_Config_SS (Thread_Data * thread_data);

GLOBAL_FUNCTION bool HI_Merge_Geometry_Bounding (Thread_Data * thread_data, Geometry const * geometry, Bounding & bounding,
	Modelling_Matrix const *	matrix = null,
	Type_Flags					visibility = T_NONE,
	Attribute_Lock *			locks = null,
	HFSB_Flags					flags = HFSB_IGNORE_VISIBILITY,
	Point_Interceptor *			point_intercepter=null);

GLOBAL_FUNCTION bool HI_Compute_LOD (
	Thread_Data *		thread_data,
	Decimate_Algorithm	algorithm,
	float				ratio,
	bool				collapse_vertices,
	int					point_count,
	Point const *		points,
	int					face_list_length,
	const int *			face_list,
	int *				out_point_count,
	Point *				out_points,
	int *				out_face_list_length,
	int *				out_face_list,
	int *				out_old_to_new_vertex_mapping,
	int *				out_old_to_new_face_mapping);

GLOBAL_FUNCTION bool HI_Compute_LOD (
	Thread_Data *		thread_data,
	Decimate_Algorithm	algorithm,
	float				ratio,
	bool				collapse_vertices,
	int					point_count,
	DPoint const *		points,
	int					face_list_length,
	const int *			face_list,
	int *				out_point_count,
	DPoint *			out_points,
	int *				out_face_list_length,
	int *				out_face_list,
	int *				out_old_to_new_vertex_mapping,
	int *				out_old_to_new_face_mapping);

GLOBAL_FUNCTION void HI_Calculate_LOD_Nice (
	Thread_Data *		thread_data,
	Polyhedron *		phon_in,
	int 				startdepth,
	int 				maxdepth,
	float				ratio,
	bool				collapse_vertices,
	int 				min_triangle_count,
	int 				max_degree);

GLOBAL_FUNCTION void HI_Calculate_LOD_Fast (
	Thread_Data *		thread_data,
	Polyhedron *		phon,
	int 				startdepth,
	int 				maxdepth,
	float				ratio,
	bool				collapse_vertices,
	int					min_triangle_count);

GLOBAL_FUNCTION Shell * HI_Create_Point_Cloud_LOD(
	Thread_Data *		thread_data,
	Polyhedron *		phon,
	int					new_point_count,
	int *				point_mapping);

GLOBAL_FUNCTION void HI_Calculate_Point_Cloud_LOD (
	Thread_Data *		thread_data,
	Polyhedron *		phon,
	int					start_depth,
	int					max_depth,
	float				ratio);

GLOBAL_FUNCTION bool HI_RGB_to_RGBA (
	unsigned int width,
	unsigned int height,
	unsigned char const * in_rgb,
	unsigned char ** out_rgba);

GLOBAL_FUNCTION bool HI_RGBA_to_RGB (
	unsigned int width,
	unsigned int height,
	unsigned char const * in_rgba,
	unsigned char ** out_rgb);

GLOBAL_FUNCTION bool HI_PNG_to_RGBA (
	unsigned char const * in_png,
	unsigned int * out_width,
	unsigned int * out_height,
	unsigned char ** out_rgba);

GLOBAL_FUNCTION bool HI_RGBA_to_PNG (
	unsigned int width,
	unsigned int height,
	unsigned char const * image,
	size_t * png_len,
	unsigned char ** png);

GLOBAL_FUNCTION bool HI_RGBA_to_JPEG (
	unsigned int in_width,
	unsigned int in_height,
	unsigned char const * in_rgba,
	size_t * out_jpeg_len,
	unsigned char ** out_jpeg);

GLOBAL_FUNCTION bool HI_JPEG_to_RGBA (
	size_t in_jpeg_len,
	unsigned char const * in_jpeg,
	unsigned int * out_width,
	unsigned int * out_height,
	unsigned char ** out_rgba);

GLOBAL_FUNCTION bool HI_TARGA_to_RGBA (
	size_t in_targa_len,
	unsigned char const * in_targa,
	unsigned int * out_width,
	unsigned int * out_height,
	unsigned char ** out_rgba);

GLOBAL_FUNCTION Image * HI_PNG_Decompress(Image const * image, bool cache_memory);
GLOBAL_FUNCTION Image * HI_JPEG_Decompress(Image const * image, bool cache_memory);
GLOBAL_FUNCTION Image * HI_TARGA_Decompress(Image const * image, bool cache_memory);
GLOBAL_FUNCTION Image * HI_Decompress_Image(Image const * image, bool in_place, bool cache_memory);

GLOBAL_FUNCTION bool HI_Determine_PNG_Info(int block_size, const void *block, int & width, int & height, bool & transparent);
GLOBAL_FUNCTION bool HI_Determine_JPEG_Info(int block_size, const void *block, int & width, int & height);
GLOBAL_FUNCTION bool HI_Determine_TARGA_Info(int block_size, const void *block, int & width, int & height, bool & transparent);
GLOBAL_FUNCTION bool HI_Determine_DXT_Info(Image_Format format, int block_size, const void *block, int & width, int & height, bool & transparent);
GLOBAL_FUNCTION bool HI_Determine_Compressed_Image_Info(Image_Format format, int block_size, const void *block, int & width, int & height, bool & transparent);

GLOBAL_FUNCTION Image * HI_Create_Spotlight_Image(int res, float inner_radius_ratio);

GLOBAL_FUNCTION Image * HI_Create_Indexication_Image(Net_Rendition const & nr);

GLOBAL_FUNCTION void HI_Free_Selection_Item(Selection_Item *);
GLOBAL_FUNCTION Selection_Item *HI_Selection_Item_Heap_Pop(Selection_Item_Heap *);

GLOBAL_FUNCTION float const *HI_Get_Sin72_Lookup(void);
GLOBAL_FUNCTION float const *HI_Get_Cos72_Lookup(void);

GLOBAL_FUNCTION Option_Type * HI_Define_Condition(Name_Const const & name);

GLOBAL_FUNCTION bool HI_Parse_Conditional (
	Thread_Data *	thread_data,
	char const *	string,
	Conditional &	conditional);

GLOBAL_FUNCTION Include * HI_Create_Include (
	Thread_Data *	thread_data,
	Segment *		ref,
	Segment *		newowner,
	Conditional *	condition);

GLOBAL_FUNCTION Geometry_Reference * HI_Create_Reference (
	Thread_Data *	thread_data,
	Segment *		target_seg,
	Segment *		owner_seg,
	Conditional *	conditional,
	int				geometry_mask=~0);

GLOBAL_FUNCTION Error_Control ** HI_Lookup_Error_Control (
	Error_Control **	linker,
	int					category,
	int					specific);

GLOBAL_FUNCTION Driver_Config * HI_Lookup_Driver_Config(
	Thread_Data *		thread_data,
	char const *		driver,
	char const *		id,
	char const *		version,
	bool				create,
	XBits const *		xbits);

GLOBAL_FUNCTION void HI_Free_Driver_Config(Driver_Config *cfg);
GLOBAL_FUNCTION Driver_Config * HI_Clone_Driver_Config( Driver_Config *cfg);

GLOBAL_FUNCTION void  HI_Queue_Selection_Event (
	Thread_Data *			thread_data,
	char const *			locater_name,
	char const *			display_name,
	char const *			action_name,
	int						status,
	char const *			segment_name,
	Key						item_key,
	int offset1, int offset2, int offset3,
	double xw, double yw, double zw, double xc, double yc, double zc,
	Selection_Event_CD *	extra);

GLOBAL_FUNCTION Key HI_Copy_Segment (
	Thread_Data *			thread_data,
	Segment *				oldseg,
	Segment *				newowner,
	Subsegment **			linker,
	Subsegment *			pred_subseg,
	Name_Const const &		newname,
	bool					check_owner,
	bool					exists,
	void *					info);
GLOBAL_FUNCTION Attribute * HI_Clone_Attributes(
	Thread_Data *			thread_data,
	Attribute const *		old_attr,
	Segment const *			new_owner,
	bool					multiple,
	void *					passthru);

GLOBAL_FUNCTION void HI_Make_RLE_Stencil(Stencil_Bitmap * stencil);
GLOBAL_FUNCTION void HI_Cleanup_Stencil(Stencil_Bitmap * stencil, bool free_it);
GLOBAL_FUNCTION Stencil_Bitmap *HI_Clone_Stencil(Stencil_Bitmap const* stencil);
GLOBAL_FUNCTION void HI_Init_Stencil_Cache(void);
GLOBAL_FUNCTION void HI_Free_Stencil_Cache(void);
GLOBAL_FUNCTION void HI_Clean_Stencil_Cache (Font_Instance const *instance);
GLOBAL_FUNCTION Stencil_Bitmap **HI_Get_Stencil_Cache (
	Font_Instance const *	instance,
	int						height,
	int						bolding_level,
	float					slant,
	float					rotation,
	float					width_scale,
	bool					antialias,
	bool					i_care_about_rotation);
GLOBAL_FUNCTION Stencil_Bitmap * HI_Lookup_Stencil(
	Stencil_Bitmap **	stencil_cache,
	unsigned short		mapped_index);
GLOBAL_FUNCTION void HI_Store_Stencil(
	Stencil_Bitmap **	stencil_cache,
	Stencil_Bitmap *	stencil);
GLOBAL_FUNCTION double HI_Get_Stencil_Cache_Height (double height, bool is_transformed);
GLOBAL_FUNCTION bool HI_Check_Texture_Syntax (char const *texture_string);

GLOBAL_FUNCTION bool HI_Fast_Lookup_Available (Segment const * owner, bool prioritized);
GLOBAL_FUNCTION void HI_Create_Fast_Lookup (Segment * owner, bool prioritized);
GLOBAL_FUNCTION void HI_Destroy_Fast_Lookups (Thread_Data * thread_data, Segment * owner);
GLOBAL_FUNCTION Segment * HI_Find_Fast_Lookup_Segment (Segment * owner, char const * name);
GLOBAL_FUNCTION void HI_Add_Fast_Lookup_Item (Segment * owner, bool prioritized, Subsegment * item);
GLOBAL_FUNCTION void HI_Remove_Fast_Lookup_Item (Segment * owner, bool prioritized, Subsegment * item);
GLOBAL_FUNCTION Subsegment * HI_Find_Fast_Lookup_Predecessor (Segment * owner, bool prioritized, Subsegment * item, char const * name);
GLOBAL_FUNCTION void HI_Show_Fast_Lookup_Priority_Range (Segment *seg, int *low, int *high);
GLOBAL_FUNCTION void HI_Begin_Font_Search(
	Thread_Data *			thread_data,
	Segment const *	const *	display,
	char const *			filter);

GLOBAL_FUNCTION bool HI_Show_Font_Outlineability(
	Thread_Data *			thread_data,
	size_t					segcount,
	Segment const * const *	segments,
	Text_Encoding			encoding,
	Name const &			font);

GLOBAL_FUNCTION bool HI_Show_Font_Info(
	Thread_Data *			thread_data,
	size_t					segcount,
	Segment const * const *	segments,
	Text_Encoding			encoding,
	Name const &			font,
	char const *			item,
	char *					data);

GLOBAL_FUNCTION void HI_End_Font_Search(Thread_Data * thread_data);

GLOBAL_FUNCTION PolyCylinder * HI_Cylinder_To_PolyCylinder (Cylinder const *cylinder);

GLOBAL_FUNCTION void HI_Adjust_Vertex_Attributes (Shell * shell, int offset, int ndelete, int insert);
GLOBAL_FUNCTION void HI_Adjust_Vertex_To_Edge (Shell * shell, int offset, int ndelete, int insert);
GLOBAL_FUNCTION void HI_Adjust_Face_Attributes (Shell * shell, int offset, int ndelete, int insert);
GLOBAL_FUNCTION void HI_Repair_Shell_Edges (Thread_Data * thread_data, Shell * shell);
GLOBAL_FUNCTION void HI_Copy_Mesh_Attributes (
	Thread_Data *			thread_data,
	Mesh const *			mesh,
	Shell *					shell);
GLOBAL_FUNCTION void HI_Convert_To_Shell (
	Thread_Data *			thread_data,
	char const *			options,
	Geometry const *		geom,
	Shell **				shellptr,
	Shell **				kill_me);
GLOBAL_FUNCTION Key HI_Generate_Shell_From_Geometry(
	Thread_Data *			thread_data,
	Anything *				owner,
	Geometry const *		geom,
	char const *			options);
GLOBAL_FUNCTION bool HI_Process_NURBS_Surface_Options (
	Option_Value const *	type_values,
	Rendering_Options *		ropt,
	bool					options_only);
GLOBAL_FUNCTION void HI_Set_Priority (Thread_Data * thread_data, Anything *thing, int priority);
GLOBAL_FUNCTION void HI_UnSet_Priority (Thread_Data * thread_data, Anything *thing);
GLOBAL_FUNCTION bool HI_Show_Priority (Thread_Data * thread_data, Key key, int * priority);

GLOBAL_FUNCTION void HI_Bring_To_Front (Thread_Data * thread_data, Anything *thing, int offset=0);
GLOBAL_FUNCTION  void HI_Delete_Specific_Shell_Faces (
	Thread_Data *			thread_data,
	Shell *					shell,
	int						count,
	int const *				indices);


GLOBAL_FUNCTION Stencil_Bitmap * HI_W32_make_font_stencil (
	Font_Instance const	*	instance,
	unsigned short			kar,
	int						in_bitmap_height,
	int						bolding_level,
	float					xrslant,
	float					xrrotation,
	float					xrwidth,
	bool					antialias);

GLOBAL_FUNCTION bool HI_W32_measure_char_stenciled(
	 Net_Rendition const &	nr,
	 Font_Instance const *	instance,
	 Karacter				kar,
	 float *				size_out,
	 bool *					kar_missing,
	 void **				per_kar_data);

GLOBAL_FUNCTION void HI_W32_unload_font_stenciled (
	 Display_Context const *	dc,
	 Font_Instance *			instance);

GLOBAL_FUNCTION void * HI_W32_load_font_stenciled(
	 Display_Context const *	dc,
	 char const *				name,
	 Font_Instance *			instance,
	 void *						hDC,
	 Log_Font_Item **			local_font_list,
	 Log_Font_Item **			system_font_list);

GLOBAL_FUNCTION void HI_W32_find_all_fonts (
	 Display_Context *	dc,
	 Log_Font_Item **	local_font_list,
	 Log_Font_Item **	system_font_list);

GLOBAL_FUNCTION bool HI_W32_find_one_font (
	 Display_Context *	dc,
	 char const *		in_name,
	 void *				hDC,
	 Log_Font_Item **	local_font_list,
	 Log_Font_Item **	system_font_list);

GLOBAL_FUNCTION void HI_W32_find_basic_fonts (
	 Display_Context *	dc,
	 void * const		hDC,
	 Log_Font_Item **	local_font_list,
	 Log_Font_Item **	system_font_list);

GLOBAL_FUNCTION bool HI_W32_name_font(
	Net_Rendition const &	nr,
	Font_Name const *		font_name_in,
	Font_Name *				font_name_out);

GLOBAL_FUNCTION Log_Font_Item *HI_W32_find_font (
	Log_Font_Item *	list,
	const char *	name);

GLOBAL_FUNCTION bool HI_Condition_Passed (
	Condition_Bits const &		known_bits,
	Conditional const &			condition,
	Condition_Value_Callback	callback = 0,
	void const *				passthru = 0);

GLOBAL_FUNCTION int HI_Generate_Elliptical_Points (
	Generic_Arc const *	arc,
	int					count,
	Point *				points);
GLOBAL_FUNCTION int HI_Generate_Elliptical_Points (
	Generic_Arc const *	arc,
	int					count,
	DPoint *			points);

GLOBAL_FUNCTION int HI_Split_Point_Lookup (Polyhedron const * polyhedron, int vertex, int face);
//GLOBAL_FUNCTION int HI_Split_Point_Original (Polyhedron const * polyhedron, int vertex);

GLOBAL_FUNCTION void HI_Free_Convex_Clip_Region (Convex_Clip_Region *cr);
GLOBAL_FUNCTION bool HI_Compute_Camera_Min_Max (
		Thread_Data const *		thread_data,
		Matrix const &			nmm,
		Camera_Handle const &	cam,
		Segment const *			seg,
		Bounding const &		bounding_in,
		double *				minz_out,
		double *				maxz_out);

GLOBAL_FUNCTION void HI_Ensure_Definition_Set(Thread_Data *	thread_data);

GLOBAL_FUNCTION Time HI_Make_Static_Tree (Net_Rendition const & nr, Segment const * root);
GLOBAL_FUNCTION void HI_Clean_Static_Tree(Thread_Data * thread_data, Segment const *seg);

GLOBAL_FUNCTION bool HI_Push_Display_List (
	Thread_Data *			thread_data,
	Display_List *			dl,
	float					priority,
	int						vram);

GLOBAL_FUNCTION void HI_Mark_Geometry_Changed(
	Thread_Data *			thread_data,
	Geometry const *		geometry,
	Seg_Flags				segflags = 0);

GLOBAL_FUNCTION void HI_Invalidate_Segment_Display_Lists(
	Thread_Data *			thread_data,
	Segment const *			seg,
	int						type,
	Geometry const *		geo,
	unsigned int			specials2_mask,
	bool					recurse = false);
GLOBAL_FUNCTION void HI_Clean_Segment_Display_Lists (
	Thread_Data *			thread_data,
	Actor const *			actor,
	Segment const *			segment,
	bool					need_locking);

GLOBAL_FUNCTION void HI_Clean_Display_List (
	Actor const *			actor,
	Display_List **			display_list,
	bool					free_it,
	bool					need_locking);

GLOBAL_FUNCTION void HI_Record_Recent_Seg_Path (Thread_Data * thread_data, Segment const * seg);
GLOBAL_FUNCTION void HI_Flush_Seg_Path_Lookup(Thread_Data * thread_data);
GLOBAL_FUNCTION void HI_Remove_Seg_Path_Lookup_Item (Thread_Data * thread_data, Segment const * seg);

GLOBAL_FUNCTION void HI_Init_Texture_Options();

GLOBAL_FUNCTION void HI_Glyph_Fixup_Polyline (
	Net_Rendition const &		nr,
	int							incount,
	DC_Point const *			inpoints
);

GLOBAL_FUNCTION void HI_Glyph_Fixup_Polygon (
	Net_Rendition const &		inr,
	int							incount,
	DC_Point const *			inpoints
);

GLOBAL_FUNCTION void HI_Glyph_Fixup_Polydot (
	Net_Rendition const &		nr,
	int							incount,
	DC_Point const *			inpoints
);

GLOBAL_FUNCTION void HI_Glyph_Fixup_Ellipse (
	Net_Rendition const &		inr,
	DC_Point const *			center,
	DC_Point const *			inradius
);

GLOBAL_FUNCTION void HI_Glyph_Fixup_Outline_Ellipse (
	Net_Rendition const &		nr,
	DC_Point const *			center,
	DC_Point const *			inradius
);

GLOBAL_FUNCTION void HI_Glyph_Fixup_Polytriangle (
	Net_Rendition const &		inr,
	int							incount,
	DC_Point const *			inpoints
);


GLOBAL_FUNCTION void HI_Compute_Tight_OBB (
	Point const	*pointlist,
	int			listlength,
	Point *	faces
);


GLOBAL_FUNCTION void HI_Set_Thread_High_Priority();

GLOBAL_FUNCTION bool HI_Color_Is_Simple (Color const *color, Color_Map const * color_map=0, bool * indexed=0);

GLOBAL_FUNCTION void HI_Delete_By_Keys (Thread_Data * thread_data, int count, Key const * keys);

GLOBAL_FUNCTION Segment * HI_Separate_Geometry_Attributes (Thread_Data * thread_data, Geometry * g);

GLOBAL_FUNCTION void HI_Delete_Renditions(
	Thread_Data * thread_data, 
	Segment * seg,
	Include const * include = 0, 
	Display_Context * dc = 0, 
	bool force = false);


GLOBAL_FUNCTION User_Options * HI_Distill_User_Options(User_Options const * user_options, int flags);


GLOBAL_FUNCTION Anything * HI_Find_Target_And_Lock (
	Thread_Data *	thread_data, 
	Open_Type		type_mask);

GLOBAL_FUNCTION  Anything * HI_Find_Target_And_Lock (
	Thread_Data *	thread_data,
	Key				key,
	Open_Type		type_mask);

GLOBAL_FUNCTION void HI_Bring_To_Front_By_Key (
	Thread_Data *	thread_data,
	Key				key);

GLOBAL_FUNCTION void HI_Begin_Shell_Selection (
	Thread_Data *	thread_data);

GLOBAL_FUNCTION void HI_End_Shell_Selection (
	Thread_Data *	thread_data);

GLOBAL_FUNCTION bool HI_Parse_String (
	char const *	string,
	char const *	delim,
	int				offset,
	char *			tok,
	int				toklen,
	int *			n_consumed = null);


GLOBAL_FUNCTION void HI_Compute_Optimized_Shell (
	Thread_Data *						thread_data,
	Shell_Optimization_Config const &	config,
	Collapse_Predicate<float> const &	collapse_pred,
	int									in_point_count,
	Point const *						in_points,
	Vector const *						in_normals,
	int									in_face_list_length,
	int const *							in_face_list,
	int *								out_point_count,
	Point *								out_points,
	int *								out_face_list_length,
	int	*								out_face_list,
	int	*								out_vertex_mapping,
	int	*								out_face_mapping);
GLOBAL_FUNCTION void HI_Compute_Optimized_Shell (
	Thread_Data *						thread_data,
	Shell_Optimization_Config const &	config,
	Collapse_Predicate<double> const &	collapse_pred,
	int									in_point_count,
	DPoint const *						in_points,
	Vector const *						in_normals,
	int									in_face_list_length,
	int const *							in_face_list,
	int *								out_point_count,
	DPoint *							out_points,
	int *								out_face_list_length,
	int	*								out_face_list,
	int	*								out_vertex_mapping,
	int	*								out_face_mapping);

GLOBAL_FUNCTION void HI_Fixup_Geometry();

GLOBAL_FUNCTION bool HI_Do_Update (
	Thread_Data *	thread_data,
	double			limit,
	bool			do_isolated);

GLOBAL_FUNCTION bool HI_Do_One_Update (
	Thread_Data *	thread_data,
	Actor *			actor,
	double			limit);

GLOBAL_FUNCTION void HI_Queue_Actor_Update(
	Actor const *	actor,
	bool			do_it_now);

GLOBAL_FUNCTION void HI_Queue_Actor_Shutdown(
	Actor const *	actor,
	bool			do_it_now);

GLOBAL_FUNCTION bool HI_Initialize_Actor (Thread_Data * thread_data, Actor * actor);


GLOBAL_FUNCTION Key HI_Show_Owner_Key_By_Key (
	Thread_Data *	thread_data,
	Key				key,
	bool			complain,
	bool			original_key);


GLOBAL_FUNCTION int HI_Determine_Coordinate_System (
	char const * coordinate_system);

GLOBAL_FUNCTION bool HI_Compute_Text_Extent(
	Thread_Data *			thread_data,
	size_t					segcount,
	Segment const * const *	segments,
	const char *			text,
	Text_Encoding			encoding,
	float *					xfrac, 
	float *					yfrac);

GLOBAL_FUNCTION bool HI_Compute_Text_Outline (
	Thread_Data *			thread_data,
	size_t					segcount,
	Segment const * const *	segments,
	KName const &			kname,
	Text_Encoding const		encoding,
	int *					point_count,
	Point *					points,
	int *					list_length,
	int *					list,
	Attribute const *		font = null);

GLOBAL_FUNCTION bool HI_Compute_Transform_By_Path (
	Thread_Data *			thread_data,
	int						count,
	Segment const * const *	segments,
	int						insys,
	int						outsys,
	float *					matrix);
GLOBAL_FUNCTION bool HI_Compute_Transform_By_Path (
	Thread_Data *			thread_data,
	int						count,
	Segment const * const *	segments,
	int						insys,
	int						outsys,
	double *				matrix);


GLOBAL_FUNCTION void HI_Show_Shell_Size (
	Thread_Data *	thread_data,
	Key				key,
	int *			point_countp,
	int *			face_list_lengthp);

GLOBAL_FUNCTION void HI_Show_Shell (
	Thread_Data *	thread_data,
	Key				key,
	int *			point_countp,
	Point *			points,
	int *			face_list_lengthp,
	int *			face_list);


GLOBAL_FUNCTION void HI_Read_Driver_Config_File (Thread_Data * thread_data);

GLOBAL_FUNCTION Segment_Cache * HI_Find_Segment_Cache (
	Segment const *			seg,
	Include_Path const &	path,
	bool					create_if_needed = true);
GLOBAL_FUNCTION Segment_Cache * HI_Find_Segment_Cache (
	Segment const *			seg,
	bool					create_if_needed = true);

GLOBAL_FUNCTION intptr_t HI_Record_Lightweight_Key (
	Anything *			item,
	int					offset,
	intptr_t			index = -1);
GLOBAL_FUNCTION void HI_Replace_Lightweight_Key (
	intptr_t			index,
	Anything *			item,
	int					offset);
GLOBAL_FUNCTION void HI_Remove_Lightweight_Key(
	intptr_t			index,
	DB_Type				type);

GLOBAL_FUNCTION Key HI_Add_To_Multiline (
	Thread_Data *		thread_data,
	Segment *			seg,
	Point const &		a,
	Point const &		b,
	intptr_t			index = -1);
GLOBAL_FUNCTION Key HI_Add_To_Multiline (
	Thread_Data *		thread_data,
	Segment *			seg,
	DPoint const &		a,
	DPoint const &		b,
	intptr_t			index = -1);

GLOBAL_FUNCTION Key HI_Add_To_Multimarker (
	Thread_Data *		thread_data,
	Segment *			seg,
	Point const &		a,
	intptr_t			index = -1);
GLOBAL_FUNCTION Key HI_Add_To_Multimarker (
	Thread_Data *		thread_data,
	Segment *			seg,
	DPoint const &		a,
	intptr_t			index = -1);

GLOBAL_FUNCTION Polyline * HI_Isolate_Line_From_Multiline (
	Thread_Data *		thread_data,
	Multiline *			ml,
	int					offset);

GLOBAL_FUNCTION Marker * HI_Isolate_Marker_From_Multimarker (
	Thread_Data *		thread_data,
	Multimarker *		mm,
	int					offset);




GLOBAL_FUNCTION int HI_Determine_Text_Length(Text * text);


GLOBAL_FUNCTION char * HI_Format_Kattribute(KAttribute * kattrib, char * bp, char * bpe);

GLOBAL_FUNCTION void HI_Show_Shell_By_Tristrips_Size(
	Thread_Data *	thread_data,
	Shell *			shell,
	int *			point_countp,
	int *			tristrip_list_lengthp,
	int *			face_list_lengthp);

GLOBAL_FUNCTION void HI_Show_Shell_By_Tristrips(
	Thread_Data *	thread_data,
	Shell *			shell,
	int *			point_countp,
	Point *			points,
	int *			tristrip_list_lengthp,
	int *			tristrip_list,
	int *			face_list_lengthp,
	int *			face_list);

GLOBAL_FUNCTION void HI_Show_Shell_By_Tristrips(
	Thread_Data *	thread_data,
	Shell *			shell,
	int *			point_countp,
	DPoint *		points,
	int *			tristrip_list_lengthp,
	int *			tristrip_list,
	int *			face_list_lengthp,
	int *			face_list);


GLOBAL_FUNCTION double HI_Cuboid_To_Cuboid_Distance(
	SimpleCuboid const & cuboid1,
	SimpleCuboid const & cuboid2);
GLOBAL_FUNCTION double HI_Cuboid_To_Cuboid_Distance(
	DSimpleCuboid const & cuboid1,
	DSimpleCuboid const & cuboid2);


GLOBAL_FUNCTION double HI_Cuboid_To_Cuboid_Max_Distance(
	SimpleCuboid const & cuboid1,
	SimpleCuboid const & cuboid2);
GLOBAL_FUNCTION double HI_Cuboid_To_Cuboid_Max_Distance(
	DSimpleCuboid const & cuboid1,
	DSimpleCuboid const & cuboid2);


GLOBAL_FUNCTION double HI_Cuboid_To_Point_Distance(
	SimpleCuboid const & cuboid,
	Point const & point);

GLOBAL_FUNCTION bool HI_Distance_Point_Polygon(
	Point const *	p, 
	Point const *	points, 
	int				flen, 
	Point &			pres, 
	float &			dist);
GLOBAL_FUNCTION bool HI_Distance_Point_Polygon(
	DPoint const *	p, 
	DPoint const *	points, 
	int				flen, 
	DPoint &		pres, 
	double &		dist);

GLOBAL_FUNCTION bool HI_Compute_Point_To_Line_Distance(
	Point const *	point, 
	Point const *	line_start,  
	Point const *	line_end, 
	float *			distance, 
	Point *			res = null);
GLOBAL_FUNCTION bool HI_Compute_Point_To_Line_Distance(
	DPoint const *	point, 
	DPoint const *	line_start,  
	DPoint const *	line_end, 
	double *		distance, 
	DPoint *		res = null);





GLOBAL_FUNCTION void HI_Link_Contents (
	Thread_Data *			thread_data,
	Segment *				seg,
	Find_Contents_Data *	fcd);

GLOBAL_FUNCTION void HI_Free_Search_List (
	Thread_Data *			thread_data,
	Search_List *			node);



GLOBAL_FUNCTION Segment * HI_Create_Proxy (
	Thread_Data *	thread_data, 
	Segment const *			seg);

GLOBAL_FUNCTION Open_Type HI_Anything_To_Open_Type(
	Thread_Data *	thread_data,
	Anything const *		item,
	Segment **				seg = null, 
	Geometry **				geo = null,
	Open_Item **			open_item_out = null);


GLOBAL_FUNCTION Key HI_Insert_Circle (
	Thread_Data *	thread_data,
	Anything *				owner,
	Point const *			start,
	Point const *			middle,
	Point const *			end);

GLOBAL_FUNCTION void HI_Edit_Circle (
	Thread_Data *	thread_data,
	Circle *				circle,
	Point const *			start,
	Point const *			middle,
	Point const *			end);

GLOBAL_FUNCTION void HI_Edit_Circle_By_Radius (
	Thread_Data *	thread_data,
	Circle *				circle,
	Point const *			center,
	float					radius,
	Vector const *			normal);

GLOBAL_FUNCTION Key HI_Insert_Circle_By_Radius (
	Thread_Data *	thread_data,
	Anything *				owner, 
	Point const *			center,
	float					radius,
	Vector const *			normal);

GLOBAL_FUNCTION Key HI_Insert_Circular_Arc (
	Thread_Data *	thread_data,
	Anything *				owner, 
	Point const *			start,
	Point const *			middle,
	Point const *			end);

GLOBAL_FUNCTION void HI_Edit_Circular_Arc (
	Thread_Data *	thread_data,
	Circular_Arc *			c,
	Point const *			start,
	Point const *			middle,
	Point const *			end);
GLOBAL_FUNCTION void HI_Edit_Circular_Chord (
	Thread_Data *	thread_data,
	Circular_Arc *			c,
	Point const *			start,
	Point const *			middle,
	Point const *			end);
GLOBAL_FUNCTION void HI_Edit_Circular_Wedge (
	Thread_Data *	thread_data,
	Circular_Arc *			c,
	Point const *			start,
	Point const *			middle,
	Point const *			end);

GLOBAL_FUNCTION Key HI_Insert_Circular_Chord (
	Thread_Data *	thread_data,
	Anything *				owner, 
	Point const *			start,
	Point const *			middle,
	Point const *			end);

GLOBAL_FUNCTION Key HI_Insert_Circular_Wedge (
	Thread_Data *	thread_data,
	Anything *				owner, 
	Point const *			start,
	Point const *			middle,
	Point const *			end);

GLOBAL_FUNCTION void HI_Edit_NURBS_Curve (
	Thread_Data *	thread_data,
	NURBS_Curve *			curve,
	int						control_point_offset,
	int						knot_offset,
	int						control_point_replace_count,
	int						knot_replace_count,
	Point const *			control_points,
	float const *			weights,
	float const *			knots); 

GLOBAL_FUNCTION Key HI_Insert_NURBS_Curve (
	Thread_Data *	thread_data,
	Anything *				owner,
	int						degree,
	int						control_point_count,
	Point const *			control_points, 
	float const *			weights,
	float const *			knots,
	float					start_u,
	float					end_u); 

GLOBAL_FUNCTION void HI_Populate_NURBS_Curve (
	Thread_Data *		thread_data,
	NURBS_Curve *		curve,
	int					degree,
	int					control_point_count,
	Point const *		control_points, 
	float const *		weights,
	float const *		knots,
	float				start_u,
	float				end_u);

GLOBAL_FUNCTION void HI_Edit_NURBS_Surface (
	Thread_Data *	thread_data,
	NURBS_Surface *			surface,
	int						control_point_offset,
	int						weight_offset,
	int						u_knot_offset,
	int						v_knot_offset,
	int						control_point_replace_count,
	int						weight_replace_count,
	int						u_knot_replace_count,
	int						v_knot_replace_count,
	Point const	*			control_points,
	float const *			weights,
	float const *			u_knots,
	float const *			v_knots);

GLOBAL_FUNCTION Key HI_Insert_NURBS_Surface (
	Thread_Data *	thread_data,
	Anything *				owner,
	int						u_degree,
	int						v_degree,
	int						n_u_vertices,
	int						n_v_vertices,
	Point const *			control_points,
	float const *			weights,
	float const *			u_knots,
	float const *			v_knots);

GLOBAL_FUNCTION void HI_Populate_NURBS_Surface(
	Thread_Data *			thread_data,
	NURBS_Surface *			surface,
	int						u_degree,
	int						v_degree,
	int						n_u_vertices,
	int						n_v_vertices,
	Point const *			control_points,
	float const *			weights,
	float const *			u_knots,
	float const *			v_knots);

GLOBAL_FUNCTION void HI_Edit_Cutting_Plane (
	Thread_Data *	thread_data,
	Cutting_Plane *			cutting_plane,
	double					a,
	double					b,
	double					c,
	double					d);

GLOBAL_FUNCTION void HI_Edit_Cutting_Plane (
	Thread_Data *	thread_data,
	Cutting_Plane *			cutting_plane,
	float					a,
	float					b,
	float					c,
	float					d);

GLOBAL_FUNCTION Key HI_Insert_Cutting_Section (
	Thread_Data *	thread_data,
	Anything *				owner,
	int						count,
	Plane const	*			planes);

GLOBAL_FUNCTION void HI_Edit_Cutting_Section (
	Thread_Data *	thread_data,
	Cutting_Plane *			cutting_plane,
	int						ioffset,
	int						ndelete,
	int						insert,
	DPlane const *			iplanes);

GLOBAL_FUNCTION void HI_Edit_Cutting_Section (
	Thread_Data *	thread_data,
	Cutting_Plane *			cutting_plane,
	int						ioffset,
	int						ndelete,
	int						insert,
	Plane const *			iplanes);

GLOBAL_FUNCTION Key HI_Insert_Cylinder (
	Thread_Data *	thread_data,
	Anything *				owner,
	Point const *			first,
	Point const *			second,
	float					radius,
	char const *			list);

GLOBAL_FUNCTION Key HI_Insert_PolyCylinder (
	Thread_Data *	thread_data,
	Anything *				owner,
	int						point_count,
	Point const *			points,
	int						radius_count,
	float const *			radii,
	char const *			list);

GLOBAL_FUNCTION void HI_Edit_PolyCylinder_Points (
	Thread_Data *	thread_data,
	PolyCylinder *			polycylinder,
	int						offset,
	int						delete_count,
	int						insert_count,
	Point const *			points,
	bool					regenerate = true);

GLOBAL_FUNCTION void HI_Edit_PolyCylinder_Points (
	Thread_Data *	thread_data,
	PolyCylinder *			polycylinder,
	int						offset,
	int						delete_count,
	int						insert_count,
	DPoint const *			points,
	bool					regenerate = true);

GLOBAL_FUNCTION void HI_Edit_PolyCylinder_Radii (
	Thread_Data *	thread_data,
	PolyCylinder *			polycylinder,
	int						offset,
	int						delete_count,
	int						insert_count,
	float const *			radii,
	bool					regenerate = true);

GLOBAL_FUNCTION void HI_Edit_PolyCylinder_Radii (
	Thread_Data *	thread_data,
	PolyCylinder *			polycylinder,
	int						offset,
	int						delete_count,
	int						insert_count,
	double const *			radii,
	bool					regenerate = true);

GLOBAL_FUNCTION void HI_Edit_PolyCylinder (
	Thread_Data *			thread_data,
	PolyCylinder *			pc,
	int						point_count,
	Point const *			points,
	Vector const *			basis,
	int						radius_count,
	float const *			radii,
	char const *			list);

GLOBAL_FUNCTION void HI_Edit_PolyCylinder (
	Thread_Data *			thread_data,
	PolyCylinder *			pc,
	int						point_count,
	DPoint const *			points,
	DVector const *			basis,
	int						radius_count,
	float const *			radii,
	char const *			list);

GLOBAL_FUNCTION void HI_Edit_Polygon (
	Thread_Data *	thread_data,
	Polygon *				polygon,
	int						ioffset,
	int						ndelete,
	int						insert,
	Point const *			ipoints);

GLOBAL_FUNCTION Key HI_Insert_Polygon (
	Thread_Data *	thread_data,
	Anything *				owner,
	int						count,
	Point const *			points);

GLOBAL_FUNCTION void HI_Edit_Ellipse (
	Thread_Data *	thread_data,
	Ellipse *				e,
	Point const *			center,
	Point const *			major,
	Point const *			minor);

GLOBAL_FUNCTION Key HI_Insert_Ellipse (
	Thread_Data *	thread_data,
	Anything *				owner,
	Point const *			center,
	Point const *			major,
	Point const *			minor);

GLOBAL_FUNCTION void HI_Edit_Elliptical_Arc (
	Thread_Data *	thread_data,
	Elliptical_Arc *		e,
	Point const *			center,
	Point const *			major,
	Point const *			minor,
	double					start,
	double					end);

GLOBAL_FUNCTION Key HI_Insert_Elliptical_Arc (
	Thread_Data *	thread_data,
	Anything *				owner,
	Point const *			center,
	Point const *			major,
	Point const *			minor,
	float					start,
	float					end);

GLOBAL_FUNCTION Key HI_Insert_Grid (
	Thread_Data *	thread_data,
	Anything *				owner,
	char const *			type,
	Point const	*			origin,
	Point const	*			ref1,
	Point const	*			ref2,
	int						count1,
	int						count2);
GLOBAL_FUNCTION void HI_Edit_Grid (
	Thread_Data *	thread_data,
	Grid *					owner,
	char const *			type,
	Point const	*			origin,
	Point const	*			ref1,
	Point const	*			ref2,
	int						count1,
	int						count2);

GLOBAL_FUNCTION Key HI_Insert_Image (
	Thread_Data *		thread_data,
	Anything *					owner,
	Point const *				where,
	char const *				format,
	int							width,
	int							height,
	int							size,
	void const *				bytes,
	bool						by_ref);

GLOBAL_FUNCTION void HI_Edit_Infinite_Line (
	Thread_Data *	thread_data,
	Polyline *				l,
	Point const *			a,
	Point const *			b,
	bool					is_ray);

GLOBAL_FUNCTION Key HI_Insert_Infinite_Line (
	Thread_Data *		thread_data,
	Anything *					owner,
	Point const *				a,
	Point const *				b,
	bool						is_ray);

GLOBAL_FUNCTION Key HI_Insert_Distant_Light (
	Thread_Data *	thread_data,
	Anything *				owner,
	Vector const &			direction);

GLOBAL_FUNCTION void HI_Edit_Light (
	Thread_Data *	thread_data,
	Light *					light,
	Point const *			position,
	Point const *			direction);

GLOBAL_FUNCTION Key HI_Insert_Area_Light (
	Thread_Data *	thread_data,
	Anything *				owner,
	int						count,
	DPoint const *			points,
	Light_Flags				flags,
	Light_Flags				bright_side);

GLOBAL_FUNCTION Key HI_Insert_Area_Light (
	Thread_Data *	thread_data,
	Anything *				owner,
	int						count,
	Point const	*			points,
	char const *			list);

GLOBAL_FUNCTION Key HI_Insert_Spot_Light (
	Thread_Data *	thread_data,
	Anything *				item,
	Point const &			position,
	Point const &			target,
	float					inner,
	float					outer,
	float					concentration,
	Light_Flags				flags,
	Spot_Light_Size_Units	inner_units,
	Spot_Light_Size_Units	outer_units);
	

GLOBAL_FUNCTION Key HI_Insert_Spot_Light (
	Thread_Data *	thread_data,
	Anything *				item,
	DPoint const &			position,
	DPoint const &			target,
	float					inner,
	float					outer,
	float					concentration,
	Light_Flags				flags,
	Spot_Light_Size_Units	inner_units,
	Spot_Light_Size_Units	outer_units);

GLOBAL_FUNCTION void HI_Initialize_Spot_Light_Options(
	Spot_Light_Info *	data);

GLOBAL_FUNCTION Key HI_Insert_Local_Light (
	Thread_Data *		thread_data,
	Anything *			owner,
	Point const &		position); 

GLOBAL_FUNCTION void HI_Edit_Line (
	Thread_Data *		thread_data,
	Key					key,
	Polyline *			l,
	Point const &		a,
	Point const &		b);
GLOBAL_FUNCTION void HI_Edit_Line (
	Thread_Data *		thread_data,
	Key					key,
	Polyline *			l,
	DPoint const &		a,
	DPoint const &		b);

GLOBAL_FUNCTION Key HI_Insert_Line (
	Thread_Data *		thread_data,
	Anything *			owner,
	Point const &		a,
	Point const &		b);
GLOBAL_FUNCTION Key HI_Insert_Line (
	Thread_Data *		thread_data,
	Anything *			owner,
	DPoint const &		a,
	DPoint const &		b);

GLOBAL_FUNCTION void HI_Edit_Marker (
	Thread_Data *		thread_data,
	Key					key,
	Marker *			m,
	Point const &		p);
GLOBAL_FUNCTION void HI_Edit_Marker (
	Thread_Data *		thread_data,
	Key					key,
	Marker *			m,
	DPoint const &		p);

GLOBAL_FUNCTION Key HI_Insert_Marker (
	Thread_Data *		thread_data,
	Anything *			owner,
	Point const &		where);
GLOBAL_FUNCTION Key HI_Insert_Marker (
	Thread_Data *		thread_data,
	Anything *			owner,
	DPoint const &		where);

GLOBAL_FUNCTION void HI_Edit_Mesh (
	Thread_Data *		thread_data,
	Mesh *				mesh,
	int					row_offset,
	int					column_offset,
	int					row_count,
	int					column_count,
	Point const *		points);

GLOBAL_FUNCTION Key HI_Insert_Mesh (
	Thread_Data *		thread_data,
	Anything *			owner,
	int					rows,
	int					columns,
	Point const	*		points,
	bool				by_ref);

GLOBAL_FUNCTION void HI_Edit_Polyline (
	Thread_Data *		thread_data,
	Polyline *			polyline,
	int					ioffset,
	int					ndelete,
	int					insert,
	Point const *		ipoints);

GLOBAL_FUNCTION Key HI_Insert_Polyline (
	Thread_Data *		thread_data,
	Anything *			owner,
	int					count,
	Point const *		points);

GLOBAL_FUNCTION void HI_Edit_Sphere (
	Thread_Data *		thread_data,
	Sphere *			s,
	Point const *		center,
	float				radius,
	Vector const *		axis,
	Vector const *		ortho);

GLOBAL_FUNCTION Key HI_Insert_Sphere (
	Thread_Data *		thread_data,
	Anything *			owner,
	Point const *		center,
	float				radius,
	Vector const *		axis,
	Vector const *		ortho);

GLOBAL_FUNCTION Key HI_Insert_Text_With_Encoding (
	Thread_Data *		thread_data,
	Anything *			owner,
	Point const &		where,
	Text_Encoding		encoding,
	void const *		string);

GLOBAL_FUNCTION Shell * HI_Create_Shell (
	int					point_count,
	Point const *		points,
	int					face_list_length,
	int const *			face_list,
	bool				by_reference);

GLOBAL_FUNCTION Shell * HI_Create_Shell (
	int					point_count,
	DPoint const *		points,
	int					face_list_length,
	int const *			face_list,
	bool				by_reference);

GLOBAL_FUNCTION Key HI_Insert_Shell (
	Thread_Data *		thread_data,
	Anything *			owner,
	int					point_count,
	Point const *		points,
	int					face_list_length,
	int const *			face_list,
	bool				by_reference);

GLOBAL_FUNCTION Key HI_Insert_Shell (
	Thread_Data *		thread_data,
	Anything *			owner,
	int					point_count,
	DPoint const *		points,
	int					face_list_length,
	int const *			face_list,
	bool				by_reference);

GLOBAL_FUNCTION void HI_Edit_Shell_Points (
	Thread_Data *		thread_data,
	Shell *				shell,
	int					ioffset,
	int					ndelete,
	int					insert,
	Point const *		points);

GLOBAL_FUNCTION void HI_Convert_Face_Offset_To_Face_List_Offset (
	int					face_list_length,
	int const *			face_list,
	int					ioffset,
	int					ndelete,
	int &				flist_offset,
	int &				delete_list_length);

GLOBAL_FUNCTION void HI_Edit_Shell_Faces (
	Thread_Data *		thread_data,
	Shell *				shell,
	int					ioffset,
	int					ndelete,
	int					insert_list_length,
	int	const *			insert_list);

GLOBAL_FUNCTION Key HI_Insert_Shell_By_Tristrips (
	Thread_Data *		thread_data,
	Anything *			owner,
	int					point_count,
	Point const	*		points,
	int					tristrips_list_length,
	int const *			tristrips_list,
	int					tristrips_faces_length,
	int const *			tristrips_faces);

GLOBAL_FUNCTION bool HI_Trivial_Face_List_From_Tristrips_List (
	int					tristrips_list_length,
	int const *			tristrips_list,
	int *				face_list_length_out,
	int **				face_list_out);


GLOBAL_FUNCTION bool HI_Set_Color(
	Thread_Data *		thread_data,
	Anything *			owner,
	Color *				color); 

GLOBAL_FUNCTION bool HI_Set_Color(
	Thread_Data *		thread_data, 
	Polyhedron *		polyhedron,
	int					offset,
	Open_Type			open_type,
	Color *				color);

GLOBAL_FUNCTION bool HI_Set_Vertex_Symbol(
	Thread_Data *		thread_data, 
	Polyhedron *		polyhedron,
	int					offset,
	Marker_Symbol	*	marker_symbol);

GLOBAL_FUNCTION bool HI_Set_Marker_Symbol(
	Thread_Data *		thread_data,
	Anything *			owner,
	Marker_Symbol	*	marker_symbol);

GLOBAL_FUNCTION bool HI_Set_Marker_Size(
	Thread_Data *			thread_data,
	Anything *				owner,
	Marker_Size *			marker_size);

GLOBAL_FUNCTION bool HI_Set_Line_Pattern(
	Thread_Data *			thread_data,
	Anything *				owner,
	Line_Pattern *			line_pattern);

GLOBAL_FUNCTION bool HI_Set_Edge_Pattern (
	Thread_Data *			thread_data,
	Anything *				owner,
	Edge_Pattern *			edge_pattern);

GLOBAL_FUNCTION bool HI_Set_Edge_Pattern (
	Thread_Data *			thread_data,
	Polyhedron *			polyhedron,
	int						offset,
	Edge_Pattern *			edge_pattern);

GLOBAL_FUNCTION void HI_Set_Bounding(
	Thread_Data *			thread_data,
	Anything *				owner,
	Bounding *				bounding);

GLOBAL_FUNCTION bool HI_Set_Callback(
	Thread_Data *			thread_data,
	Anything *				owner,
	Callback *				callback);


GLOBAL_FUNCTION bool HI_Common_Camera_Helper(
	Thread_Data *			thread_data,
	Camera &				proto,
	Point const *			position,
	Point const *			target,
	Vector const *			up_vector,
	float					width,
	float					height,
	char const *			proj,
	float					near_limit);

GLOBAL_FUNCTION bool HI_Set_Camera (
	Thread_Data *			thread_data,
	Anything *				owner,
	Camera *				camera);

GLOBAL_FUNCTION void HI_Set_Camera_Near_Limit (
	Thread_Data *			thread_data,
	Anything *				owner,
	float					limit);

GLOBAL_FUNCTION void HI_UnSet_Camera_Near_Limit (
	Thread_Data *			thread_data,
	Anything *				owner);

GLOBAL_FUNCTION void HI_Set_Camera_By_Volume (
	Thread_Data *			thread_data,
	Anything *				owner,
	char const *			proj,
	float					xmin,
	float					xmax,
	float					ymin,
	float					ymax);

GLOBAL_FUNCTION void HI_Set_Camera_Field (
	Thread_Data *			thread_data,
	Anything *				owner,
	float					width,
	float					height);

GLOBAL_FUNCTION void HI_Set_Camera_Position (
	Thread_Data *			thread_data,
	Anything *				owner, 
	Point const *			point);

GLOBAL_FUNCTION void HI_Set_Camera_Projection (
	Thread_Data *			thread_data,
	Anything *				owner, 
	char const *			proj);

GLOBAL_FUNCTION void HI_Set_Camera_Target (
	Thread_Data *			thread_data,
	Anything *				owner,
	Point const *			target);

GLOBAL_FUNCTION void HI_Set_Camera_Up_Vector (
	Thread_Data *			thread_data,
	Anything *				owner,
	Vector const *			up_vector);

GLOBAL_FUNCTION void HI_Zoom_Camera (
	Thread_Data *			thread_data,
	Anything *				owner,
	float					zoom);

GLOBAL_FUNCTION void HI_Roll_Camera (
	Thread_Data *			thread_data,
	Anything *				owner,
	float					angle);

GLOBAL_FUNCTION void HI_Pan_Camera (
	Thread_Data *			thread_data,
	Anything *				owner,
	float					horiz,
	float					vert);

GLOBAL_FUNCTION void HI_Orbit_Camera (
	Thread_Data *			thread_data,
	Anything *				owner,
	float					horiz,
	float					vert);

GLOBAL_FUNCTION void HI_Dolly_Camera (
	Thread_Data *			thread_data,
	Anything *				owner,
	Point const *			point);

GLOBAL_FUNCTION bool HI_Set_Clip_Region (
	Thread_Data *			thread_data,
	Anything *				owner,
	Clip_Region *			clip_region);

GLOBAL_FUNCTION bool HI_Set_Color_Map (
	Thread_Data *			thread_data,
	Anything *				owner,
	Color_Map *				color_map);

GLOBAL_FUNCTION bool HI_Set_Conditions (
	Thread_Data *			thread_data,
	Anything *				owner,
	Conditions *			conditions);

GLOBAL_FUNCTION void HI_UnSet_Color(
	Thread_Data *			thread_data,
	Polyhedron *			polyhedron,
	int						offset,
	Open_Type				open_type,
	Color const *			unset_proto=0);

GLOBAL_FUNCTION void HI_UnSet_Edge_Pattern(
	Thread_Data *			thread_data,
	Polyhedron *			polyhedron,
	int						offset);

GLOBAL_FUNCTION void HI_UnSet_Vertex_Symbol(
	Thread_Data *			thread_data,
	Polyhedron *			polyhedron,
	int						offset);


GLOBAL_FUNCTION bool HI_Set_Line_Weight(
	Thread_Data *			thread_data,
	Anything *				target,
	Line_Weight *			line_weight);

GLOBAL_FUNCTION bool HI_Set_Edge_Weight (
	Thread_Data *			thread_data,
	Anything *				owner,
	Edge_Weight *			edge_weight);

GLOBAL_FUNCTION bool HI_Set_Edge_Weight(
	Thread_Data *			thread_data,
	Polyhedron *			polyhedron,
	int						offset,
	Edge_Weight *			edge_weight);

GLOBAL_FUNCTION void HI_UnSet_Edge_Weight(
	Thread_Data *			thread_data,
	Polyhedron *			polyhedron,
	int						offset);

GLOBAL_FUNCTION void HI_UnSet_Vertex_Size(
	Thread_Data *			thread_data,
	Polyhedron *			polyhedron,
	int						offset);

GLOBAL_FUNCTION bool HI_Set_Text_Spacing (
	Thread_Data *			thread_data,
	Anything *				owner,
	Text_Spacing *			text_spacing);

GLOBAL_FUNCTION bool HI_Set_Face_Pattern(
	Thread_Data *			thread_data,
	Polyhedron *			polyhedron,
	int						offset,
	Open_Type				open_type,
	Face_Pattern *			face_pattern);

GLOBAL_FUNCTION bool HI_Set_Face_Pattern (
	Thread_Data *			thread_data,
	Anything *				owner,
	Face_Pattern *			face_pattern);

GLOBAL_FUNCTION void HI_UnSet_Face_Pattern(
	Thread_Data *			thread_data,
	Polyhedron *			polyhedron,
	int						offset,
	Open_Type				open_type);

GLOBAL_FUNCTION bool HI_Set_Window_Pattern (
	Thread_Data *			thread_data,
	Anything *				owner,
	Window_Pattern	*		window_pattern);

GLOBAL_FUNCTION Key HI_Set_Glyph(
	Thread_Data *			thread_data,
	Anything *				owner,
	Glyph					glyph);

GLOBAL_FUNCTION void HI_UnSet_Defined_Glyph(
	Thread_Data *			thread_data,
	Anything *				owner,
	Name_Const const &		defname,
	bool					complain);

GLOBAL_FUNCTION Key HI_Set_Line_Style(
	Thread_Data *			thread_data,
	Anything *				owner,
	Line_Style &			line_style);

GLOBAL_FUNCTION Line_Style HI_Define_Line_Style (
	Thread_Data *			thread_data,
	Name_Const const &		name,
	char const *			definition);

GLOBAL_FUNCTION HOOPS::Line_Style HI_Clone_Line_Style(
	Thread_Data *	thread_data,
	Line_Style const &		source);

GLOBAL_FUNCTION void HI_UnSet_Defined_Line_Style(
	Thread_Data *			thread_data,
	Anything *				owner,
	Name_Const const &		defname,
	bool					complain);

//GLOBAL_FUNCTION void HI_Initialize_Text_Font(Text_Font * text_font);

GLOBAL_FUNCTION bool HI_Set_Text_Font (
	Thread_Data *			thread_data,
	Anything *				owner,
	Text_Font *				text_font);

GLOBAL_FUNCTION bool HI_Set_Text_Alignment (
	Thread_Data *			thread_data,
	Anything *				owner,
	Text_Alignment	*		text_alignment);

GLOBAL_FUNCTION void HI_Set_Text_Region (
	Thread_Data *			thread_data,
	Text *					text,
	int						count,
	Point const *			points,
	int						options,
	int						packed_fitting = 0);

GLOBAL_FUNCTION bool HI_Show_Key(Context & context, char const* segname, Key * key);


GLOBAL_FUNCTION bool HI_Set_Driver_Options (
	Thread_Data *			thread_data,
	Anything *				owner, 
	Driver_Options *		driver_options);


GLOBAL_FUNCTION bool HI_Set_Handedness (
	Thread_Data *			thread_data,
	Anything *				owner,
	Handedness *			handedness);

GLOBAL_FUNCTION bool HI_Set_Heuristics (
	Thread_Data *			thread_data,
	Anything *				owner,
	Heuristics *			heuristics); 

GLOBAL_FUNCTION bool HI_Set_Modelling_Matrix(
	Thread_Data *			thread_data,
	Anything *				owner,
	Modelling_Matrix *		modelling_matrix);

GLOBAL_FUNCTION bool HI_Append_Modelling_Matrix(
	Thread_Data *			thread_data,
	Anything *				owner,
	Modelling_Matrix *		modelling_matrix);


GLOBAL_FUNCTION bool HI_Set_Rendering_Options(
	Thread_Data *			thread_data,
	Anything *				owner,
	Rendering_Options *		rendering_options);

GLOBAL_FUNCTION bool HI_Set_Selectability(
	Thread_Data *			thread_data,
	Anything *				owner,
	Selectability *			selectability); 

GLOBAL_FUNCTION bool HI_Set_Streaming_Mode(
	Thread_Data *			thread_data,
	Anything *				owner,
	Streaming_Mode *		sm);

GLOBAL_FUNCTION bool HI_Set_Text_Path (
	Thread_Data *			thread_data,
	Anything *				owner,
	Text_Path *				text_path);

GLOBAL_FUNCTION bool HI_Set_Texture_Matrix (
	Thread_Data *			thread_data,
	Anything *				owner,
	Texture_Matrix *		texture_matrix);

GLOBAL_FUNCTION bool HI_Append_Texture_Matrix (
	Thread_Data *			thread_data,
	Anything *				owner,
	Texture_Matrix *		texture_matrix);

GLOBAL_FUNCTION bool HI_Set_Visibility (
	Thread_Data *			thread_data,
	Anything *				owner,
	Visibility *			proto);

GLOBAL_FUNCTION bool HI_Set_Visibility(
	Thread_Data *			thread_data, 
	Polyhedron *			polyhedron,
	int						offset,
	Open_Type				open_type,
	Visibility *			proto);

GLOBAL_FUNCTION void HI_UnSet_Visibility(
	Thread_Data *			thread_data, 
	Polyhedron *			polyhedron,
	int						offset,
	Open_Type				open_type);

GLOBAL_FUNCTION bool HI_Set_Window (
	Thread_Data *			thread_data,
	Anything *				owner,
	Window *				window);

GLOBAL_FUNCTION bool HI_Set_Window_Frame (
	Thread_Data *			thread_data,
	Anything *				owner,
	Window_Frame *			window_frame);

GLOBAL_FUNCTION bool HI_Set_User_Value (
	Thread_Data *			thread_data,
	Anything *				owner,
	User_Value *			user_value);

GLOBAL_FUNCTION bool HI_Set_User_Options (
	Thread_Data *			thread_data,
	Anything *				owner,
	User_Options *			user_options);

GLOBAL_FUNCTION bool HI_Define_Named_Style_Defs (
	Thread_Data *			thread_data,
	Anything *				owner,
	Named_Style_Defs *		named_style_defs);

GLOBAL_FUNCTION Key HI_Define_Named_Style (
	Thread_Data *			thread_data, 
	Anything *				target, 
	Named_Style &			named_style);

GLOBAL_FUNCTION Named_Style HI_Create_Named_Style (
	Thread_Data *			thread_data,
	Name_Const const &		name,
	Segment *				segment);

GLOBAL_FUNCTION void HI_Undefine_Named_Style(
	Thread_Data *			thread_data, 
	Anything const *		owner, 
	Name_Const const &		defname,
	bool					complain);

GLOBAL_FUNCTION int HI_Undefine_Named_Style(
	Thread_Data *			thread_data,
	Anything const *		owner,
	Segment const *			referee,
	bool					complain);

GLOBAL_FUNCTION Conditional_Action * HI_Copy_Conditional_Actions(Conditional_Action * actions);
GLOBAL_FUNCTION void HI_Free_Conditional_Actions(Conditional_Action * action);

GLOBAL_FUNCTION bool HI_Check_Geometry_Attribute_Type(
	Thread_Data *			thread_data,
	Geometry const *		geo,
	DB_Type					type);

GLOBAL_FUNCTION bool HI_Is_Segment_Open(
	Thread_Data *			thread_data,
	Segment *				seg,
	bool					complain);


GLOBAL_FUNCTION bool HI_Bounding_Is_Excluded (Attribute const *attribute);

GLOBAL_FUNCTION void HI_Queue_Actor_Keyboard_Event (Actor const * actor, int button, int status); // drivers use this
GLOBAL_FUNCTION void HI_Queue_Keyboard_Event_W_Enc (Actor const * actor, int button, int status, char encoding);

GLOBAL_FUNCTION void HI_Queue_Location_Event (Thread_Data * thread_data, Actor const *source, float x, float y, int status, int action);
GLOBAL_FUNCTION void HI_Queue_Actor_Location_Event( // drivers use this
	Actor const*			actor,
	float					x,
	float					y,
	int						status,
	Locater_Actions			action);

GLOBAL_FUNCTION void HI_Queue_Special_Event (
	Thread_Data *			thread_data,
	char const *			type,
	char const *			value);

GLOBAL_FUNCTION char * HI_Build_Special_Event_String (
	char *string,
	char const *brand,
	Segment const *actor,
	intptr_t window,
	int event_type);

GLOBAL_FUNCTION void HI_Generate_Polyline_DC_Bezier (
	Polyline const *	polyline,
	Matrix const &		matrix,
	DC_Point *			buffer);

GLOBAL_FUNCTION int HI_Check_For_Events (Thread_Data * thread_data, Actor const * actor);



GLOBAL_FUNCTION void HI_System_Memory_Purge();


GLOBAL_FUNCTION bool HI_UnSet_Attribute (
	Thread_Data *			thread_data,
	Anything *				owner,
	DB_Type					type,
	UnSet_Attribute_Action	unset_acquire=0,
	Attribute const *		unset_proto=0);

GLOBAL_FUNCTION bool HI_UnSet_Text_Font (
	Thread_Data *			thread_data,
	Anything *				owner, 
	Text_Font const *		unset_proto=0);

GLOBAL_FUNCTION bool HI_UnSet_Color(
	Thread_Data *			thread_data,
	Anything *				owner,
	Color const *			unset_proto=0);

GLOBAL_FUNCTION bool HI_UnSet_Driver_Options (
	Thread_Data *			thread_data,
	Anything *				owner, 
	Driver_Options const *	unset_proto=0);

GLOBAL_FUNCTION bool HI_UnSet_Selectability(
	Thread_Data *			thread_data,
	Anything *				owner,
	Selectability const *	unset_proto=0); 

GLOBAL_FUNCTION bool HI_UnSet_Visibility (
	Thread_Data *			thread_data,
	Anything *				owner,
	Visibility const *		unset_proto=0);

GLOBAL_FUNCTION bool HI_UnSet_Rendering_Options (
	Thread_Data *				thread_data,
	Anything *					owner,
	Rendering_Options const *	unset_proto=0);

GLOBAL_FUNCTION bool HI_UnSet_Heuristics (
	Thread_Data *			thread_data,
	Anything *				owner,
	Heuristics const *		unset_proto=0);

GLOBAL_FUNCTION bool HI_UnSet_Callback(
	Thread_Data *			thread_data, 
	Anything *				owner, 
	Callback const *		unset_proto=0);

GLOBAL_FUNCTION bool HI_UnSet_User_Options (
	Thread_Data *			thread_data,
	Anything *				owner,
	User_Options const *	unset_proto=0);

GLOBAL_FUNCTION Selection_Event	* HI_Compute_Selection (
	Thread_Data *			context,
	Actor *					actor,
	Selection_Event * &		se);

GLOBAL_FUNCTION int HI_Show_Selection_Keys (
	Selection_Item const *	si,
	Key *					ikeys,
	bool					original);

GLOBAL_FUNCTION int HI_Compute_Selection_By_Shell (
	Thread_Data *		thread_data,
	Locater_Actions		action,
	Segment const *		start_seg,
	int					point_count,
	Point const *		points,
	int					face_list_length,
	int const *			face_list,
	const char *		options,
	SimpleCuboid *		collision_bbox = 0,
	Selection_Event *	se = null,
	int					limit = ~0,
	void * *			sel_info = null);
GLOBAL_FUNCTION int HI_Compute_Selection_By_Shell (
	Thread_Data *		thread_data,
	Locater_Actions		action,
	Segment const *		start_seg,
	int					point_count,
	DPoint const *		points,
	int					face_list_length,
	int const *			face_list,
	const char *		options,
	DSimpleCuboid *		collision_bbox = 0,
	Selection_Event *	se = null,
	int					limit = ~0,
	void * *			sel_info = null);
GLOBAL_FUNCTION int HI_Compute_Selection_By_Ray (
	Thread_Data *		thread_data,
	Locater_Actions		action,
	Segment const *		start_seg,
	Point const *		start,
	Vector const *		point_direction,
	const char *		options,
	Selection_Event *	se = null,
	void * *			sel_info = null);
GLOBAL_FUNCTION int HI_Compute_Selection_By_Ray (
	Thread_Data *		thread_data,
	Locater_Actions		action,
	Segment const *		start_seg,
	DPoint const *		start,
	DVector const *		point_direction,
	const char *		options,
	Selection_Event *	se = null);

GLOBAL_FUNCTION void HI_Define_Driver_Config(
	Thread_Data *		thread_data,
	char const *		driver,
	char const *		id,
	char const *		version,
	char const *		config);

GLOBAL_FUNCTION Callback_Name HI_Define_Callback_Name (
	Thread_Data *		thread_data,
	char const *		name,
	Void_Routine		address);

GLOBAL_FUNCTION bool HI_Show_Window_Info(
	Thread_Data *		thread_data,
	Segment *			seg,
	Show_Window_Info &	swi);


GLOBAL_FUNCTION long HI_Common_Show_One_User_Data(
	Thread_Data *			thread_data,
	User_Options const *	user_options,
	intptr_t				index,
	void *					data,
	long 					data_size);

GLOBAL_FUNCTION long HI_Common_Show_User_Data_Indices(
	Thread_Data *			thread_data,
	User_Options const *	in_user_options,
	intptr_t *				data_indices,
	long					data_indices_count);

GLOBAL_FUNCTION bool HI_Compute_Bounding_By_Key (
	Thread_Data *			thread_data,
	Key						key,
	Type_Flags				visibility,
	HFSB_Flags				flags,
	Bounding &				bounding);


GLOBAL_FUNCTION void HI_Shutdown_Threading(void);

GLOBAL_FUNCTION void HI_Define_Highlight (
	Thread_Data *			thread_data,
	int						count,
	Key const *				keys,
	Name_Const const &		style,
	int						options,
	int						elements_count,
	int const *				offset1,
	int const *				offset2,
	int const *				offset3);
GLOBAL_FUNCTION size_t HI_UnDefine_Highlight (
	Thread_Data *			thread_data,
	int						count,
	Key const *				keys,
	Name_Const const &		style,
	int						flags,
	int						elements_count=0,
	int const *				offset1=null,
	int const *				offset2=null,
	int const *				offset3=null,
	bool *					partial_removal=null);
GLOBAL_FUNCTION size_t HI_Flush_Highlights(Thread_Data * thread_data);

GLOBAL_FUNCTION void HI_Determine_Highlight_Status (
	Thread_Data *				thread_data,
	int							count,
	Key const *					keys,
	int *						results,
	Highlight_Options const *	options = null);


GLOBAL_FUNCTION void HI_MSet_Vertex_Colors_By_Value (
	Thread_Data *			thread_data,
	Polyhedron *			polyhedron,
	Color_Object			color_object,
	int						offset,
	char const *			color_space,
	int						count,
	RGB const *				rgb_values,
	RGBA const *			rgba_values);

GLOBAL_FUNCTION void HI_MSet_Vertex_Colors_By_FIndex (
	Thread_Data *			thread_data,
	Polyhedron *			polyhedron,
	Color_Object			color_object,
	int						offset,
	int						count,
	float const *			findices);

GLOBAL_FUNCTION void HI_MSet_Spec_Vert_Col_By_Value (
	Thread_Data *			thread_data,
	Polyhedron *			polyhedron,
	Color_Object			color_object,
	int						count,
	int const *				indices,
	char const *			color_space,
	RGB const *				rgb_values,
	RGBA const *			rgba_values);

GLOBAL_FUNCTION void HI_MSet_Spec_Vert_Col_By_FIndex (
	Thread_Data *			thread_data,
	Polyhedron *			polyhedron,
	Color_Object			color_object,
	int						count,
	int const *				indices,
	float const *			findices);


GLOBAL_FUNCTION int HI_MShow_Vertex_Colors (
	Thread_Data *			thread_data,
	Polyhedron *			polyhedron,
	Color_Object			color_object,
	int						offset,
	int						count,
	int const *				specific_vertices,
	char *					result_type,
	float *					index_colors,
	RGB *					rgb,
	RGBA *					rgba,
	int						permission_flags = 0,
	int *					error_state = null);

GLOBAL_FUNCTION void HI_MUnSet_Vertex_Colors (
	Thread_Data *			thread_data,
	Polyhedron *			polyhedron,
	Color_Object			color_object,
	int						offset,
	int						count);

GLOBAL_FUNCTION void HI_MUnSet_Specific_Vertex_Colors (
	Thread_Data *			thread_data,
	Polyhedron *			polyhedron,
	Color_Object			color_object,
	int						count,
	int const *				indices);

GLOBAL_FUNCTION void HC_CDECL HI_MSet_Vertex_Visibilities (
	Thread_Data *			thread_data,
	Polyhedron *			polyhedron,
	int						offset,
	int						count,
	unsigned char const *	visibilities);

GLOBAL_FUNCTION int HI_MShow_Vertex_Visibilities(
	Thread_Data *			thread_data,
	Polyhedron *			polyhedron,
	int						offset,
	int						count,
	int const *				specific_vertices,
	unsigned char *			existence,
	unsigned char *			visibilities,
	int *					error_state = null);

GLOBAL_FUNCTION void HI_MSet_Specific_Vertex_Visibilities (	
	Thread_Data *			thread_data,
	Polyhedron *			polyhedron,
	int						count,
	int const *				indices,
	unsigned char const *	visibilities);

GLOBAL_FUNCTION void HI_MSet_Face_Colors_By_Value (
	Thread_Data *			thread_data,
	Polyhedron *			polyhedron,
	Color_Object			color_object,
	int						offset,
	char const *			color_space,
	int						count,
	RGB const *				values);

GLOBAL_FUNCTION void HI_MSet_Specific_Face_Colors_By_Value (
	Thread_Data *			thread_data,
	Polyhedron *			polyhedron,
	int						count,
	int const *				indices,
	char const *			color_space,
	RGB const *				values);

GLOBAL_FUNCTION void HI_MSet_Face_Colors_By_FIndex (
	Thread_Data *			thread_data,
	Polyhedron *			polyhedron,
	Color_Object			color_object,
	int						offset,
	int						count,
	float const *			findices);

GLOBAL_FUNCTION void HI_MSet_Specific_Face_Colors_By_FIndex (
	Thread_Data *			thread_data,
	Polyhedron *			polyhedron,
	int						count,
	int const *				indices,
	float const *			findices);

GLOBAL_FUNCTION void HI_MUnSet_Face_Colors (
	Thread_Data *			thread_data,
	Polyhedron *			polyhedron,
	int						offset,
	int						count);

GLOBAL_FUNCTION void HI_MUnSet_Specific_Face_Colors (
	Thread_Data *			thread_data,
	Polyhedron *			polyhedron,
	int						count,
	int const *				indices);

GLOBAL_FUNCTION int HI_MShow_Face_Colors (
	Thread_Data *			thread_data,
	Polyhedron *			polyhedron,
	int						offset,
	int						count,
	int const *				specific_faces,
	char *					result_type,
	float *					index_colors,
	RGB *					rgb,
	int *					error_state = null);


GLOBAL_FUNCTION void HI_MSet_Vertex_Normals (
	Thread_Data *			thread_data,
	Polyhedron *			polyhedron,
	int						offset,
	int						count,
	Vector const *			normals);

GLOBAL_FUNCTION void HI_MSet_Specific_Vertex_Normals (
	Thread_Data *			thread_data,
	Polyhedron *			polyhedron,
	int						count,
	int	const *				indices,
	Vector const *			normals);

GLOBAL_FUNCTION void HI_MUnSet_Vertex_Normals (
	Thread_Data *			thread_data,
	Polyhedron *			polyhedron,
	int						offset,
	int						count);

GLOBAL_FUNCTION void HI_MUnSet_Specific_Vertex_Normals (
	Thread_Data *			thread_data,
	Polyhedron *			polyhedron,
	int						count,
	int	const *				indices);


GLOBAL_FUNCTION int HI_MShow_Vertex_Normals (
	Thread_Data *			thread_data,
	Polyhedron *			polyhedron,
	int						offset,
	int						count,
	int const *				specific_vertices,
	unsigned char *			existence,
	Vector *				normals,
	int *					error_state = null);


GLOBAL_FUNCTION void HI_MSet_Face_Normals (
	Thread_Data *			thread_data,
	Polyhedron *			polyhedron,
	int						offset,
	int						count,
	Vector const *			normals);

GLOBAL_FUNCTION void HI_MSet_Specific_Face_Normals (
	Thread_Data *			thread_data,
	Polyhedron *			polyhedron,
	int						count,
	int const *				indices,
	Vector const *			normals);

GLOBAL_FUNCTION void HI_MUnSet_Face_Normals (
	Thread_Data *			thread_data,
	Polyhedron *			polyhedron,
	int						offset,
	int						count);

GLOBAL_FUNCTION void HI_MUnSet_Specific_Face_Normals (
	Thread_Data *			thread_data,
	Polyhedron *			polyhedron,
	int						count,
	int	const *				indices);

GLOBAL_FUNCTION int HI_MShow_Face_Normals (
	Thread_Data *			thread_data,
	Polyhedron *			polyhedron,
	int						offset,
	int						count,
	int const *				specific_faces,
	unsigned char *			existence,
	Vector *				normals,
	int *					error_state = null);

GLOBAL_FUNCTION void HI_MShow_Net_Face_Normals (
	Thread_Data *		thread_data,
	Polyhedron *		polyhedron,
	int					offset,
	int					count,
	int const *			specific_faces,
	Vector *			normals,
	int *				error_state = null);

GLOBAL_FUNCTION void HI_MSet_Vertex_Parameters (
	Thread_Data *			thread_data,
	Polyhedron *			polyhedron,
	int						offset,
	int						count,
	int						param_width,
	float const *			float_params);

GLOBAL_FUNCTION void HI_MSet_Specific_Vertex_Parameters (
	Thread_Data *			thread_data,
	Polyhedron *			polyhedron,
	int						count,
	int const *				indices,
	int						param_width,
	float const *			float_params);

GLOBAL_FUNCTION void HI_MUnSet_Vertex_Parameters (
	Thread_Data *			thread_data,
	Polyhedron *			polyhedron,
	int						offset,
	int						count);

GLOBAL_FUNCTION void HI_MUnSet_Specific_Vertex_Parameters (
	Thread_Data *			thread_data,
	Polyhedron *			polyhedron,
	int						count,
	int const *				indices);

GLOBAL_FUNCTION int HI_MShow_Vertex_Parameters(
	Thread_Data *			thread_data,
	Polyhedron *			polyhedron,
	int						offset,
	int						count,
	int const *				specific_vertices,
	unsigned char *			existence,
	int *					number_parameters,
	float *					params,
	int *					error_state = null);

GLOBAL_FUNCTION void HI_MUnSet_Vertex_Visibilities (
	Thread_Data *			thread_data,
	Polyhedron *			polyhedron,
	int						offset,
	int						count);

GLOBAL_FUNCTION void HI_MUnSet_Specific_Vertex_Visibilities (
	Thread_Data *			thread_data,
	Polyhedron *			polyhedron,
	int						count,
	int const *				indices);

GLOBAL_FUNCTION void HI_MSet_Specific_Vertex_Visibilities (	
	Thread_Data *			thread_data,
	Polyhedron *			polyhedron,
	int						count,
	int const *				indices,
	unsigned char const *	visibilities);


GLOBAL_FUNCTION void HI_MSet_Face_Visibilities (
	Thread_Data *			thread_data,
	Polyhedron *			polyhedron,
	int						offset,
	int						count,
	unsigned char const *	visibilities);

GLOBAL_FUNCTION void HI_MSet_Specific_Face_Visibilities (
	Thread_Data *			thread_data,
	Polyhedron *			polyhedron,
	int						count,
	int const *				indices,
	unsigned char const *	visibilities);

GLOBAL_FUNCTION void HI_MUnSet_Face_Visibilities (
	Thread_Data *			thread_data,
	Polyhedron *			polyhedron,
	int						offset,
	int						count);

GLOBAL_FUNCTION void HI_MUnSet_Specific_Face_Visibilities (
	Thread_Data *			thread_data,
	Polyhedron *			polyhedron,
	int						count,
	int const *				indices);

GLOBAL_FUNCTION int HI_MShow_Face_Visibilities (
	Thread_Data *			thread_data,
	Polyhedron *			polyhedron,
	int						offset,
	int						count,
	int const *				specific_faces,
	unsigned char *			existence,
	unsigned char *			visibilities,
	int *					error_state = null);

GLOBAL_FUNCTION bool HI_Insert_Loop(
	Thread_Data *			thread_data,
	Point const *			points,
	int *					facelist,
	int						index,
	Loop_Info *				loops,
	Loop_Containment_Tree *	node,
	int						pl);
GLOBAL_FUNCTION bool HI_Insert_Loop(
	Thread_Data *				thread_data,
	DPoint const *				points,
	int *						facelist,
	int							index,
	DLoop_Info *				loops,
	DLoop_Containment_Tree *	node,
	int							pl);

GLOBAL_FUNCTION bool HI_Loop_Within_Loop (
	Thread_Data *			thread_data,
	Point *					points,
	int *					facelist,
	Loop_Info *				stats1,
	Loop_Info *				stats2,
	int						pl);
GLOBAL_FUNCTION bool HI_Loop_Within_Loop (
	Thread_Data *			thread_data,
	DPoint *				points,
	int *					facelist,
	DLoop_Info *			stats1,
	DLoop_Info *			stats2,
	int						pl);

GLOBAL_FUNCTION void HI_Write_Loop_To_Facelist(
	Thread_Data *			thread_data,
	int *					in_list,
	int *					out_list,
	int *					out_pos,
	Loop_Containment_Tree *	node);
GLOBAL_FUNCTION void HI_Write_Loop_To_Facelist(
	Thread_Data *				thread_data,
	int *						in_list,
	int *						out_list,
	int *						out_pos,
	DLoop_Containment_Tree *	node);

GLOBAL_FUNCTION void HI_Do_Segments (
	Thread_Data *			thread_data,
	Segment *				start_seg,
	Segment_Set &			collector,
	Path_Token const *		tok,
	Segment_List_Pooled *	ordered_segs = null,
	Path_Token_List *		pending = null,
	Segment *				seg = null);

GLOBAL_FUNCTION void HI_Collect_Segments(
	Thread_Data *			thread_data,
	char const *			spec,
	bool					required,
	Segment_Set &			collector,
	Segment_List_Pooled *	ordered_segs = null);

GLOBAL_FUNCTION bool HI_Write_Metafile (
	Thread_Data *			thread_data,
	Segment *				seg,
	Metafile *				metafile);

GLOBAL_FUNCTION Key HI_Insert_Polyline (
	Thread_Data *			thread_data,
	Anything *				item,
	int						count,
	DPoint const *			points);

GLOBAL_FUNCTION void HI_Edit_Polyline (
	Thread_Data *			thread_data,
	Polyline *				polyline,
	int						ioffset,
	int						ndelete,
	int						insert,
	DPoint const *			ipoints);

GLOBAL_FUNCTION void HI_Show_Shell (
	Thread_Data *			thread_data,
	Key						key,
	int *					point_countp,
	DPoint *				points,
	int *					face_list_lengthp,
	int *					face_list);

GLOBAL_FUNCTION int HI_Generate_Complete_Elliptical_Points (
	Point const *	center,
	Vector const *	basis1,
	Vector const *	basis2,
	int				count,
	Point *			pl);
GLOBAL_FUNCTION int HI_Generate_Complete_Elliptical_Points (
	DPoint const *	center,
	DVector const *	basis1,
	DVector const *	basis2,
	int				count,
	DPoint *		pl);

GLOBAL_FUNCTION Key HC_CDECL HI_Insert_Circle (Thread_Data * thread_data, Anything * owner, DPoint const *point1, DPoint const *point2, DPoint const *point3);
GLOBAL_FUNCTION Key HC_CDECL HI_Insert_Circle_By_Radius (Thread_Data * thread_data, Anything * owner, DPoint const *center, double radius, DPoint const *normal);
GLOBAL_FUNCTION Key HC_CDECL HI_Insert_Circular_Arc (Thread_Data * thread_data, Anything * owner, DPoint const *point1, DPoint const *point2, DPoint const *point3);
GLOBAL_FUNCTION Key HC_CDECL HI_Insert_Circular_Chord (Thread_Data * thread_data, Anything * owner, DPoint const *point1, DPoint const *point2, DPoint const *point3);
GLOBAL_FUNCTION Key HC_CDECL HI_Insert_Circular_Wedge (Thread_Data * thread_data, Anything * owner, DPoint const *point1, DPoint const *point2, DPoint const *point3);
GLOBAL_FUNCTION void HC_CDECL HI_Edit_Circle (Thread_Data * thread_data, Circle * circle, DPoint const *point1, DPoint const *point2, DPoint const *point3);
GLOBAL_FUNCTION void HC_CDECL HI_Edit_Circle_By_Radius (Thread_Data * thread_data, Circle * circle, DPoint const *center, double radius, DVector const *vector);
GLOBAL_FUNCTION void HC_CDECL HI_Edit_Circular_Arc (Thread_Data * thread_data, Circular_Arc * arc, DPoint const *point1, DPoint const *point2, DPoint const *point3);
GLOBAL_FUNCTION void HC_CDECL HI_Edit_Circular_Chord (Thread_Data * thread_data, Circular_Arc * arc, DPoint const *point1, DPoint const *point2, DPoint const *point3);
GLOBAL_FUNCTION void HC_CDECL HI_Edit_Circular_Wedge (Thread_Data * thread_data, Circular_Arc * arc, DPoint const *point1, DPoint const *point2, DPoint const *point3);

GLOBAL_FUNCTION Key HC_CDECL HI_Insert_Ellipse (Thread_Data * thread_data, Anything * owner, DPoint const *center, DPoint const *major, DPoint const *minor);
GLOBAL_FUNCTION Key HC_CDECL HI_Insert_Elliptical_Arc (Thread_Data * thread_data, Anything * owner, DPoint const *center, DPoint const *major, DPoint const *minor, double start, double end);
GLOBAL_FUNCTION void HC_CDECL HI_Edit_Ellipse (Thread_Data * thread_data, Ellipse * ellipse, DPoint const *center, DPoint const *major, DPoint const *minor);
GLOBAL_FUNCTION void HC_CDECL HI_Edit_Elliptical_Arc (Thread_Data * thread_data, Elliptical_Arc * arc, DPoint const *center, DPoint const *major, DPoint const *minor, double start, double end);

GLOBAL_FUNCTION Key HI_Insert_Grid (
	Thread_Data *		thread_data,
	Anything *			item,
	char const *		type,
	DPoint const *		origin,
	DPoint const *		ref1,
	DPoint const *		ref2,
	int					count1,
	int					count2);
GLOBAL_FUNCTION void HI_Edit_Grid (
	Thread_Data *		thread_data,
	Grid *				item,
	char const *		type,
	DPoint const *		origin,
	DPoint const *		ref1,
	DPoint const *		ref2,
	int					count1,
	int					count2);


GLOBAL_FUNCTION void HI_Init_ICU_Globals ();
GLOBAL_FUNCTION void HI_Free_ICU_Globals ();
GLOBAL_FUNCTION void HI_Free_Freetype_Globals ();

GLOBAL_FUNCTION Elliptical_Arc * HI_Generate_Elliptical_Arc (Generic_Arc const *	curve);

GLOBAL_FUNCTION void HI_Setup_Actors (Thread_Data *	thread_data);
GLOBAL_FUNCTION bool HI_Null_Actor (Thread_Data *	thread_data, Actor *actor, int request, void * request_info);

GLOBAL_FUNCTION RGB HI_Apply_Color_Effects (RGB const & base, Tinting const & tinting);

GLOBAL_FUNCTION void HI_Reset_System (bool soft_reset=false);


GLOBAL_FUNCTION void HI_MSet_Region_Faces (
	Thread_Data *			thread_data,
	Polyhedron *			polyhedron,
	int						region,
	int						first,
	int						count);

GLOBAL_FUNCTION Polyline * HI_Compute_Polyline_From_Curve (
	Thread_Data *			thread_data,
	Geometry *				target,
	float					max_deviation);

GLOBAL_FUNCTION bool HI_Parse_Curve_Choices(
	Thread_Data *			thread_data,
	char const *			options, 
	float &					max_deviation);

GLOBAL_FUNCTION bool HI_Validate_Spot_Light_Options (
	Thread_Data *			thread_data,
	Point const &			position,
	Point const &			target,
	float					inner,
	float					outer,
	float					concentration,
	Light_Flags				flags,
	Spot_Light_Size_Units	inner_units,
	Spot_Light_Size_Units	outer_units,
	float *					final_inner = null,
	float *					final_outer = null);

GLOBAL_FUNCTION bool HI_Validate_Spot_Light_Options (
	Thread_Data *			thread_data,
	DPoint const &			position,
	DPoint const &			target,
	float					inner,
	float					outer,
	float					concentration,
	Light_Flags				flags,
	Spot_Light_Size_Units	inner_units,
	Spot_Light_Size_Units	outer_units,
	float *					final_inner = null,
	float *					final_outer = null);

GLOBAL_FUNCTION void HI_Move_By_Key(
	Thread_Data *			thread_data,
	Key						key,
	Segment *				newowner);

GLOBAL_FUNCTION HOOPS::Key HI_Copy_Geometry(
	Thread_Data *			thread_data,
	Segment	*				seg,
	Geometry const *		old_geom,
	int						offset);

GLOBAL_FUNCTION HOOPS::Key HI_Process_Output_Parse_Action (
	Thread_Data *			thread_data,
	Segment *				oldseg,
	Output_Parse_Action		action,
	Segment *				newowner,
	Name const *			testname,
	bool					was_splat,
	void *					info);

GLOBAL_FUNCTION void HI_Move_Text(
	Thread_Data *			thread_data,
	Anything *				item,
	Point const &			where);
GLOBAL_FUNCTION void HI_Move_Text(
	Thread_Data *			thread_data,
	Anything *				item,
	DPoint const &			where);

GLOBAL_FUNCTION bool HI_Determine_Color_Object (
	Thread_Data *			thread_data,
	Polyhedron *			polyhedron,
	char const *			geometry,
	Color_Object &			color_object);

GLOBAL_FUNCTION Color * HI_Color_From_Value (
	Thread_Data *			thread_data,
	Name_Const const &		what,
	char const *			style,
	float					a,
	float					b,
	float					c);

GLOBAL_FUNCTION Color * HI_Color_From_Index(
	Thread_Data *			thread_data,
	Name_Const const &		what,
	float					index);

GLOBAL_FUNCTION void HI_Patch_Color_By_Target (
	Thread_Data *			thread_data,
	Anything const *		item,
	Color *					color);

GLOBAL_FUNCTION void HI_Set_Normal (
	Thread_Data *			thread_data,
	Polyhedron *			polyhedron,
	int						offset,
	Open_Type				open_type,
	Vector					normal);

GLOBAL_FUNCTION void HI_Set_Parameter (
	Thread_Data *			thread_data,
	Polyhedron *			polyhedron,
	int						offset,
	Open_Type				open_type,
	int						number_parameters,
	float const *			parameters);

GLOBAL_FUNCTION void HI_Show_Net_Normal (
	Thread_Data *			thread_data,
	Polyhedron *			polyhedron,
	int						offset,
	Open_Type				open_type,
	Vector &				out_normal);

GLOBAL_FUNCTION void HI_Compute_Convex_Hull (
	Thread_Data *			thread_data,
	int						icount,
	Point const *			points,
	int *					listlen,
	int *					out_list);

GLOBAL_FUNCTION void HI_Move_String_Cursor (
	Thread_Data *			thread_data,
	String_Cursor *			cursor,
	int						row,
	int						column);

GLOBAL_FUNCTION bool HI_OpenGL_Find_VRam();

GLOBAL_FUNCTION char * HI_Format_Attribute_Filter (
	Thread_Data *			thread_data,
	Attribute_Lock *		filter,
	char *					p);

GLOBAL_FUNCTION License_Error::Type HI_Create_License (
	Thread_Data *			thread_data,
	char const *			encoded_license);
	
GLOBAL_FUNCTION void HI_Destroy_License (
	Thread_Data *			thread_data);

GLOBAL_FUNCTION Product::Flags HI_Licensed_Products (
	Thread_Data *			thread_data);

GLOBAL_FUNCTION Version HI_Product_Version (
	Thread_Data *			thread_data,
	Product::Flags			product);

GLOBAL_FUNCTION Product_Error::Type HI_Can_Use_Product (
	Thread_Data *			thread_data,
	Product::Flags			product,
	Version					version);

GLOBAL_FUNCTION Visualize_Feature::Flags HI_Visualize_Features (
	Thread_Data *			thread_data);

GLOBAL_FUNCTION bool HI_Get_Exchange_Customer_Key (
	Thread_Data *			thread_data,
	Name &					out_key);

GLOBAL_FUNCTION bool HI_Get_Exchange_Variable_Key (
	Thread_Data *			thread_data,
	Name &					out_key);

GLOBAL_FUNCTION bool HI_Get_Hours_Till_Expiration (
	Thread_Data *			thread_data,
	int &					out_hours,
	bool &					out_never_expires);


GLOBAL_FUNCTION Include_Chain * HI_Includechain_From_Pathname(
	Thread_Data *			thread_data,
	const char *			pathname);


GLOBAL_FUNCTION Shell * HI_Decimate_Shell (
	Thread_Data *			thread_data,
	Decimate_Config const &	config,
	Anything *				owner);

GLOBAL_FUNCTION bool HI_Begin_Decimate_Shell (
	Thread_Data *					thread_data,
	Begin_Decimate_Config const &	config,
	Shell *							shell);

GLOBAL_FUNCTION void HI_End_Decimate_Shell (
	Thread_Data *	thread_data);

GLOBAL_FUNCTION Open_Item * HI_Open_LOD (
	Thread_Data *	thread_data,
	int				offset);

GLOBAL_FUNCTION void HI_Close_LOD (
	Thread_Data *	thread_data);

GLOBAL_FUNCTION void HI_Open_Geometry (
	Thread_Data *	thread_data,
	Key				key);

GLOBAL_FUNCTION void HI_Close_Geometry (
	Thread_Data *	thread_data);


GLOBAL_FUNCTION void HI_Set_Default_Attributes_Glyph_Defs(Thread_Data * thread_data, Anything * target);
GLOBAL_FUNCTION void HI_Set_Default_Attributes_Line_Style_Defs(Thread_Data * thread_data, Anything * target);



GLOBAL_FUNCTION void HI_Flush_Proxies(Thread_Data *	thread_data);

GLOBAL_FUNCTION void HI_Relinquish_Memory(void);

GLOBAL_FUNCTION void HI_Update_Shell_Contents_Flags(Shell *shell);

GLOBAL_FUNCTION void HI_Generate_Texture_Parameters(char param_source, Polyhedron * polyhedron, Texture_Parameters * tp);

/*******************************/

#ifndef HPS_CORE_BUILD

GLOBAL_FUNCTION void HI_Open_Segment (Thread_Data * thread_data, Segment *seg);
GLOBAL_FUNCTION void HI_Close_Segment (Thread_Data * thread_data);


GLOBAL_FUNCTION bool HI_Show_Environment (char const * which, Name &	result);

GLOBAL_FUNCTION void HI_Show_Callback (Callback *callback, char *spec, int speclen, void **associated_data);
GLOBAL_FUNCTION void HI_Show_Heuristics (Heuristics const *heuristics, char *listptr, int listlen);

GLOBAL_FUNCTION char * HI_Show_Material (char *bp, char const *bpe, Named_Material const *material, Option_Value const *option, bool parentheses);
GLOBAL_FUNCTION void HI_Show_Rendering_Options (Rendering_Options *ropt, char *listptr, int listlen);
GLOBAL_FUNCTION void HI_Show_Selectability (Selectability *select, char *spec, int speclen);
GLOBAL_FUNCTION void HI_Show_Text_Alignment (Text_Alignment *text_font, char *result, int resultlen);
GLOBAL_FUNCTION void HI_Show_Text_Font (Text_Font const *text_font, char *result, int resultlen);

GLOBAL_FUNCTION bool HI_Show_Texture (
	Thread_Data *	thread_data,
	Texture const *	texture,
	char *			definition,
	int				deflen,
	char *			shader_source,
	int				shader_source_len,
	bool			show_implicit_source,
	const char *	opt);


GLOBAL_FUNCTION void HI_Show_Image_Format(Image * image, char * format, int fmtlen);
GLOBAL_FUNCTION void HI_Show_Conditional (Conditional const * conditional, char * list, int listlen);
GLOBAL_FUNCTION void HI_Show_Conditions (Conditions const * conditions, char * options, int optionslen);
GLOBAL_FUNCTION void HI_Show_Visibility (Visibility *vis, char *spec, int speclen);
GLOBAL_FUNCTION void HI_Show_Line_Pattern_Explicit (Thread_Data * thread_data, Line_Pattern * line_pattern, char * pattern, int patternlen);
GLOBAL_FUNCTION void HI_Show_Edge_Pattern_Explicit (Thread_Data * thread_data, Edge_Pattern * edge_pattern, char * pattern, int patternlen);


GLOBAL_FUNCTION void HI_Show_Area_Light_Options(
	Area_Light const *	light,
	char *				list,
	int					listlen);

GLOBAL_FUNCTION void HC_CDECL HI_Show_Spot_Light(
	Spot_Light const *	light,
	Point *				position,
	Point *				target,
	char *				list);

GLOBAL_FUNCTION void HC_CDECL HI_Show_Spot_Light(
	Spot_Light const *	light,
	DPoint *			position,
	DPoint *			target,
	char *				list);


GLOBAL_FUNCTION char * HI_Show_Text_Region_Options (
	Text_Region const *		region,
	char *					oprions);


GLOBAL_FUNCTION bool HI_Show_One_User_Option_By_Key(Thread_Data * thread_data, Key key, char const * uoname, char * options);
GLOBAL_FUNCTION bool HI_PShow_One_Net_User_Option(Thread_Data * thread_data, int count, Key const * keys, char const * uoname, char * options);

GLOBAL_FUNCTION void HI_Show_One_User_Data_By_Key(Thread_Data * thread_data, Key key, intptr_t index, void *data, long data_length, long * data_length_out);
GLOBAL_FUNCTION void HI_PShow_One_Net_User_Data(Thread_Data * thread_data, int count, Key const * keys, intptr_t index, void *data, long data_length, long * data_length_out);


GLOBAL_FUNCTION void HI_Show_One_Conditional_Action (
	Thread_Data *			thread_data,
	Conditions const *		conditions,
	char const *			action_type,
	char *					options,
	char *					condition);



GLOBAL_FUNCTION void HI_Show_Color_Name (
	char const *	string,
	char *			def,
	int				len);

GLOBAL_FUNCTION void HI_Define_Default_Colors (Thread_Data * thread_data);

GLOBAL_FUNCTION void HI_UnDefine_Colors (Thread_Data * thread_data, char const *string);

GLOBAL_FUNCTION void HI_Define_Exit_Handler (
	Thread_Data *		thread_data,
	Void_Routine		iroutine);

GLOBAL_FUNCTION void HI_Show_Device_Info (
	Thread_Data *		thread_data,
	Segment *			seg,
	char const *		item,
	char *				data,
	int					datalen);



GLOBAL_FUNCTION void HI_Format_Color_Def(Color_Def * item, char * &bp, char * bpe, char * &ep, char * epe, char * &ip, char * ipe);


GLOBAL_FUNCTION bool HI_Show_Type (
	Thread_Data *			thread_data,
	Anything *				thing,
	char *					type);

GLOBAL_FUNCTION void HI_Show_Type_Name (int itype, char *type, int length);

GLOBAL_FUNCTION void HI_Really_Canonize_Line_Pattern (char const *string, Name * name, int32_t *extra);

GLOBAL_FUNCTION Key HI_Output_Parse (
	Thread_Data *	thread_data,
	Name_Const const &		oldspec,
	Name_Const const &		newspec,
	Output_Parse_Action		action,
	void *					info);

GLOBAL_FUNCTION void HI_Return_Kars (char *out, int outlen, unsigned short const *in, size_t inlen, char encoding);
GLOBAL_FUNCTION void HI_Return_Karacters (unsigned short *out, int outlen, unsigned short const *in, int inlen);

GLOBAL_FUNCTION void HI_Show_Driver_Options (Driver_Options *driver_options, char *listptr, int listlen);

GLOBAL_FUNCTION void HI_Show_Image (
	Thread_Data *	thread_data,
	Key				key,
	double *		x,
	double *		y,
	double *		z,
	char *			format,
	int *			width,
	int *			height,
	void *			bytes);
GLOBAL_FUNCTION void HI_Show_Image (
	Thread_Data *	thread_data,
	Key				key,
	float *			x,
	float *			y,
	float *			z,
	char *			format,
	int *			width,
	int *			height,
	void *			bytes);
GLOBAL_FUNCTION void HI_Show_Image (
	Thread_Data *	thread_data,
	Key				key,
	char *			format,
	int *			width,
	int *			height,
	void *			bytes);


GLOBAL_FUNCTION Alias ** HI_Look_Up_Alias (Thread_Data * thread_data, Name_Const const & name);

GLOBAL_FUNCTION void HI_Define_Alias (
	Thread_Data *		thread_data,
	char const *		iname,
	char const *		definition);

GLOBAL_FUNCTION bool HI_Find_Alias (
	Thread_Data *		thread_data,
	char *				alias,
	int					length);

GLOBAL_FUNCTION void HI_Show_Alias (
	Thread_Data *		thread_data,
	char const *		alias,
	char *				bp,
	int					length);


GLOBAL_FUNCTION void HI_Show_Geometry_Options (
	Geometry const *	geo,
	char *				list,
	int					listlen
	);


GLOBAL_FUNCTION void HI_Define_Error_Handler (
	Thread_Data *			thread_data,
	Error_Handler_Routine	routine); 

GLOBAL_FUNCTION void HI_UnDefine_Error_Handler (
	Thread_Data *			thread_data,
	Error_Handler_Routine	routine);

GLOBAL_FUNCTION void HI_Report_Error (
	int						category,
	int						specific,
	int						severity,
	int						msgc,
	char const * const *	msgv,
	int						stackc,
	char const * const *	stackv);



GLOBAL_FUNCTION void HI_Requeue_Event (Thread_Data * thread_data);

GLOBAL_FUNCTION void HI_Exit_Program ();

GLOBAL_FUNCTION void HI_Call_Exit_Handlers (void);

GLOBAL_FUNCTION bool HI_Process_Heuristic_Options(
	Thread_Data *			thread_data,
	char const *			list,
	Heuristics *			proto,
	bool					selection_only,
	bool					options_only);


#endif



/*******************************/

#ifdef HPS_CORE_BUILD
GLOBAL_FUNCTION void HI_HPS_Timer_Hook (void);

GLOBAL_FUNCTION void HI_HPS_Queue_Location_Event (
	Actor const *	actor,
	float			x,
	float			y,
	int				status,
	Locater_Actions	action);

#endif

#define HPS_WINDOW_EVENT_CLOSE		1
#define HPS_WINDOW_EVENT_FOCUS_IN	2
#define HPS_WINDOW_EVENT_FOCUS_OUT	3

GLOBAL_FUNCTION void HI_HPS_Queue_Window_Event (
	Actor const *	actor,
	int				status);
/*******************************/

#endif /* HI_PROTO_DEFINED */
