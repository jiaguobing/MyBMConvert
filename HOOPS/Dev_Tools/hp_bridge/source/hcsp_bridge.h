/* file hcsp_bridge.h */


#define HP_ERROR_key_out_of_range                       -1
#define HP_ERROR_non_circle_key_out_of_range            -2
#define HP_ERROR_key_does_not_exist                     -3
#define HP_ERROR_unable_to_determine_entity_class       -4
#define HP_ERROR_non_mappable_entity_class              -5
#define HP_ERROR_unknown_entity_class                   -6
#define HP_ERROR_not_implemented                        -7
#define HP_ERROR_hoops_key_maps_to_invalid_entity_class -8
#define HP_ERROR_incorrect_face_tag_mapping             -9

extern void HP_Init(const char*, unsigned char, bool = false);

extern void HP_Update(void);

extern void HP_Close(void);

extern void HP_Delete_Entity_Geometry(unsigned long, int*, bool = false);

extern bool HP_Update_Entity(int, int = 0, int = 0);

extern long HP_Compute_Geometry_Keys(int, long, HLONG*, const char *);

extern long HP_Compute_Geometry_Key_Count(int, char *);

extern int  HP_Compute_TagID(HLONG, int);

extern void HP_Set_Rendering_Options(const char *);

extern void HP_Show_Rendering_Options(char *);

extern void HP_Show_One_Rendering_Option(const char *, char *);

extern void HP_Set_Schema_Path(const char *);

extern void HP_Set_Hash_Level(unsigned char);

extern void HP_Set_Instances_To_Bodies(bool);

extern bool HP_Get_Instances_To_Bodies(void);

extern int  HP_Check_Consistency(void);

extern bool HP_Associate_Key_To_Entity(int, HLONG);

extern void HP_Compute_Selected_Entity_List(int**, int*);

extern unsigned char   HP_Get_Hash_Level(void);

extern const wchar_t *    HP_Get_Schema_Path(void);

extern void HP_Internal_Open_Assembly(int assembly, bool tesselate, int level);

extern void HP_Internal_Merge_Body_Faces(int * bodies_to_merge, unsigned long num_Bodies);

extern void insert_vertices_for_body(int body);

/*
 * these have been excluded for 16.18 and 17.0 - users should use the native Parasolid csharp interface
 * extern "C" void HP_Show_Tessellation_Options(TOPOL_render_line_t*, TOPOL_render_facet_go_t*, TOPOL_facet_mesh_t*);
 * extern "C" int  HP_Read_Xmt_File(const char *, int, int&, int**, const PART_receive_t*, bool = true);
 * extern "C" int  HP_Write_Xmt_File(const char *, int, int [], const PART_transmit_t*);
 * 
 *  struct PART_receive
 * {
 * 	int  o_t_version;
 * 	int  transmit_format;
 * 	int  receive_user_fields;
 * 	int  attdef_mistmatch;
 * };
 * typedef struct PART_receive PART_receive_t;
 * 
 * struct PART_transmit
 * {
 * 	int             o_t_version;
 * 	int             transmit_format;
 * 	unsigned char   transmit_user_fields;
 * 	int             transmit_version;
 * 	int             transmit_nmnl_geometry;
 * };
 * typedef struct PART_transmit PART_transmit_t;
 * 
 * struct AXIS_1 {
 * 	double location[3];
 * 	double axis[3];
 * };
 * typedef struct AXIS_1 AXIS_1_t;
 * 
 * struct AXIS_2 {
 * 	double location[3];
 * 	double axis[3];
 * 	double ref_direction[3];
 * };
 * typedef struct AXIS_2 AXIS_2_t;
 * 
 * struct NABOX {
 * 	AXIS_2_t  basis_set;
 * 	double    box[6];
 * };
 * typedef struct NABOX NABOX_t;
 * 
 * struct facet_local_tolerances
 * {
 * 	double   curve_chord_tol;
 * 	double   curve_chord_max;
 * 	double   curve_chord_ang;
 * 	double   surface_plane_tol;
 * 	double   surface_plane_ang;
 * };
 * typedef struct facet_local_tolerances facet_local_tolerances_t;
 * 
 * struct TOPOL_render_line
 * {
 *     int                 o_t_version;
 *     int                 edge;
 *     int                 silhouette;
 *     int                 planar;
 *     int                 radial;
 *     int                 param;
 *     int                 unfix;
 *     int                 visibility;
 *     int                 smooth;
 *     int                 internal_1;
 *     int                 region;
 *     int                 hierarch;
 *     int                 bcurve;
 *     int                 viewport;
 *     int                 invisible;
 *     int                 drafting;
 *     int                 self_hidden;
 *     unsigned char       is_curve_chord_tol;
 *     double              curve_chord_tol;
 *     unsigned char       is_curve_chord_max;
 *     double              curve_chord_max;
 *     unsigned char       is_curve_chord_ang;
 *     double              curve_chord_ang;
 *     double              planar_spacing;
 *     AXIS_1_t            planar_axis;
 *     double              radial_around;
 *     double              radial_along;
 *     double              radial_about;
 *     double              param_u;
 *     double              param_v;
 *     double              unfix_spacing;
 *     NABOX_t             viewport_sf;
 *     double              radial_around_start;
 *     double              radial_along_start;
 *     double              radial_about_start;
 *     double              param_u_start;
 *     double              param_v_start;
 *     int                 ignore;
 *     double              ignore_value;
 *     int                 overlap;
 *     int                 memory_target;
 *     size_t              memory_target_value;
 *     unsigned char       is_edge_smooth_tol;
 *     double              edge_smooth_tol;
 *     int                 n_viewports;
 *     const NABOX_t *     viewports;
 *     int                 transparent;
 *     int                 n_transparent_indices;
 *     const int *         transparent_indices;
 *     int                 transparent_hid;
 *     int                 n_overlap_indices;
 *     const int *         overlap_indices1;
 *     int                 n_overlap_indices2;
 *     const int *         overlap_indices2;
 * };
 * typedef struct TOPOL_render_line TOPOL_render_line_t;
 * 
 * struct TOPOL_render_facet_go
 * {
 *     int  o_t_version;
 *     int  go_normals;
 *     int  go_parameters;
 *     int  go_edges;
 *     int  go_strips;
 *     int  go_max_facets_per_strip;
 *     int  go_interleaved;
 *     int  split_strips;
 *     int  consistent_parms;
 * 
 * };
 * typedef struct TOPOL_render_facet_go TOPOL_render_facet_go_t;
 * 
 * struct TOPOL_facet_mesh
 * {
 *     int             o_t_version;
 *     int             shape;
 *     int             match;
 *     int             density;
 *     int             cull;
 *     int             n_loops;
 *     int *           loops;
 *     int             max_facet_sides;
 *     unsigned char   is_min_facet_width;
 *     double          min_facet_width;
 *     unsigned char   is_max_facet_width;
 *     double          max_facet_width;
 *     unsigned char   is_curve_chord_tol;
 *     double          curve_chord_tol;
 *     unsigned char   is_curve_chord_max;
 *     double          curve_chord_max;
 *     unsigned char   is_curve_chord_ang;
 *     double          curve_chord_ang;
 *     unsigned char   is_surface_plane_tol;
 *     double          surface_plane_tol;
 *     unsigned char   is_surface_plane_ang;
 *     double          surface_plane_ang;
 *     unsigned char   is_facet_plane_tol;
 *     double          facet_plane_tol;
 *     unsigned char   is_facet_plane_ang;
 *     double          facet_plane_ang;
 *     unsigned char   is_local_density_tol;
 *     double          local_density_tol;
 *     unsigned char   is_local_density_ang;
 *     double          local_density_ang;
 *     int             degen;
 *     int             n_view_directions;
 *     double *        view_directions;
 *     int             n_local_tols;
 *     facet_local_tolerances_t  *local_tols;
 *     int             n_topols_with_local_tols;
 *     int *           topols_with_local_tols;
 *     int *           local_tols_for_topols;
 *     int             ignore;
 *     double          ignore_value;
 *     int             ignore_scope;
 *     int             wire_edges;
 *     int             incremental_facetting;
 *     int             incremental_refinement;
 *     int             incremental_report;
 *     int             inflect;
 *     int             quality;
 *     unsigned char   vertices_on_planar;
 * };
 * typedef struct TOPOL_facet_mesh TOPOL_facet_mesh_t;
*/
