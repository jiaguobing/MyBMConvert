using System;
using System.Text;
using System.Runtime.InteropServices;

using PK = PLMComponents.Parasolid.PK_.Unsafe;

unsafe public partial class HCSP
{
	protected static PK.TOPOL.render_facet_o_t g_facetOptions;
	protected static PK.TOPOL.render_line_o_t g_lineOptions;

	protected static void initialize_generation_settings(ref PK.TOPOL.facet_mesh_o_t generation_settings)
	{
		generation_settings = new PK.TOPOL.facet_mesh_o_t(true);

		generation_settings.shape = PK.facet_shape_t.convex_c;
		generation_settings.match = PK.facet_match_t.topol_c;
		generation_settings.density = PK.facet_density_t.no_view_c;
		generation_settings.cull = PK.facet_cull_t.none_c;
		generation_settings.n_loops = 0;
		generation_settings.loops = null;
		generation_settings.max_facet_sides = 100000;
		generation_settings.is_min_facet_width = PK.LOGICAL_t.@false;
		generation_settings.min_facet_width = 0;
		generation_settings.is_max_facet_width = PK.LOGICAL_t.@false;
		generation_settings.max_facet_width = 0;
		generation_settings.is_curve_chord_tol = PK.LOGICAL_t.@false;
		generation_settings.curve_chord_tol = 0;
		generation_settings.is_curve_chord_max = PK.LOGICAL_t.@false;
		generation_settings.curve_chord_max = 0;
		generation_settings.is_curve_chord_ang = PK.LOGICAL_t.@true;
		generation_settings.curve_chord_ang = 0.2; //radians
		generation_settings.is_surface_plane_tol = PK.LOGICAL_t.@false;
		generation_settings.surface_plane_tol = 0.0;
		generation_settings.is_surface_plane_ang = PK.LOGICAL_t.@false;
		generation_settings.surface_plane_ang = 0.0;
		generation_settings.is_facet_plane_tol = PK.LOGICAL_t.@false;
		generation_settings.facet_plane_tol = 0.0;
		generation_settings.is_facet_plane_ang = PK.LOGICAL_t.@true;
		generation_settings.facet_plane_ang = 0.01; // radians
		generation_settings.is_local_density_tol = PK.LOGICAL_t.@false;
		generation_settings.local_density_tol = 0.0;
		generation_settings.is_local_density_ang = PK.LOGICAL_t.@false;
		generation_settings.local_density_ang = 0.0;
	}
	protected static void initialize_go_options(ref PK.TOPOL.render_facet_go_o_t go_options)
	{
		go_options = new PK.TOPOL.render_facet_go_o_t(true);

		go_options.go_normals = PK.facet_go_normals_t.yes_c;
		go_options.go_parameters = PK.facet_go_parameters_t.no_c;
		go_options.go_edges = PK.facet_go_edges_t.no_c;

		//GH99
		go_options.go_strips = PK.facet_go_strips_t.yes_c;
		go_options.go_max_facets_per_strip = 1000;				//is this a good setting?
	}
	protected static void initialize_line_generation_settings(ref PK.TOPOL.render_line_o_t line_Options)
	{
		line_Options = new PK.TOPOL.render_line_o_t(true);

		// do render parasolid edges.
		line_Options.edge = PK.render_edge_t.yes_c;

		// don't render silhouette edges as these are view-dependent
		line_Options.silhouette = PK.render_silhouette_t.no_c;

		// render planar hatches when the entity has planar hatch attribute set
		line_Options.planar = PK.render_planar_t.attrib_c;

		// render radial hatches when the entity has radial hatch attribute set
		line_Options.radial = PK.render_radial_t.attrib_c;

		// render parametric hatches when the entity has parametric hatch attribute set
		line_Options.param = PK.render_param_t.attrib_c;

		// render unfixed blends as defined by blend attribute (? rw)
		line_Options.unfix = PK.render_unfix_t.no_c;

		// "visibility" can have the values:
		//    PK_render_vis_no_c            no visibility evaluated (default)
		//    PK_render_vis_hid_c           don't output hidden lines
		//    PK_render_vis_inv_c           output hidden lines marked as invisible
		//    PK_render_vis_inv_draft_c     output hidden lines and distinguish between
		//                                  those hidden by other lines and those
		//                                  hidden by faces.
		//    PK_render_vis_extended_c      visibility is evaluated.  The information
		//                                  output is controlled by the 'invisible',
		//                                  'drafting' and 'self_hidden' options of
		//                                  the PK_TOPOL_render_line_o_t structure.
		line_Options.visibility = PK.render_vis_t.no_c;
		line_Options.invisible = PK.render_invisible_t.no_c;
		line_Options.drafting = PK.render_drafting_t.no_c;
		line_Options.self_hidden = PK.render_self_hidden_t.no_c;

		// don't distinguish whether edges are smooth
		line_Options.smooth = PK.render_smooth_t.no_c;

		// don't indicate whether edges are internal
		line_Options.@internal = PK.render_internal_t.no_c;

		// don't output regional information
		line_Options.region = PK.render_region_t.no_c;

		line_Options.hierarch = PK.render_hierarch_t.no_c;

		// output bcurves as polylines
		line_Options.bcurve = PK.render_bcurve_t.polyline_c;

		// don't skip geometry outside the viewport
		line_Options.viewport = PK.render_viewport_t.no_c;

		line_Options.is_curve_chord_tol = PK.LOGICAL_t.@true;
		line_Options.curve_chord_tol = 0.000344;

		line_Options.is_curve_chord_max = PK.LOGICAL_t.@false;
		line_Options.curve_chord_max = 0.0;

		line_Options.is_curve_chord_ang = PK.LOGICAL_t.@true;
		line_Options.curve_chord_ang = .39269908175;


		// have not so far found documentation on any of the following, so just setting to zeroes. rw
		line_Options.planar_spacing = 0.0;

		fixed (double* coord = line_Options.planar_axis.axis.coord)
		{
			coord[0] = 0.0;
			coord[1] = 0.0;
			coord[2] = 0.0;
		}

		fixed (double* coord = line_Options.planar_axis.location.coord)
		{
			coord[0] = 0.0;
			coord[1] = 0.0;
			coord[2] = 0.0;
		}

		line_Options.radial_around = 0.0;
		line_Options.radial_along = 0.0;
		line_Options.radial_about = 0.0;
		line_Options.param_u = 0.0;
		line_Options.param_v = 0.0;
		line_Options.unfix_spacing = 0.0;

		// have not so far found documentation on any of the following, so just setting to zeroes. rw

		fixed (double* coord = line_Options.viewport_sf.basis_set.ref_direction.coord)
		{
			coord[0] = 0.0;
			coord[1] = 0.0;
			coord[2] = 0.0;
		}

		fixed (double* coord = line_Options.viewport_sf.basis_set.axis.coord)
		{
			coord[0] = 0.0;
			coord[1] = 0.0;
			coord[2] = 0.0;
		}

		fixed (double* coord = line_Options.viewport_sf.basis_set.location.coord)
		{
			coord[0] = 0.0;
			coord[1] = 0.0;
			coord[2] = 0.0;
		}

		fixed (double* coord = line_Options.viewport_sf.box.coord)
		{
			coord[0] = 0.0;
			coord[1] = 0.0;
			coord[2] = 0.0;
			coord[3] = 0.0;
			coord[4] = 0.0;
			coord[5] = 0.0;
		}

	}

	static HCSP()
	{
		InitializeRenderOptions();
	}

	public static void InitializeRenderOptions()
	{
		initialize_generation_settings(ref g_facetOptions.control);
		initialize_go_options(ref g_facetOptions.go_option);
		initialize_line_generation_settings(ref g_lineOptions);
	}

	[DllImport(HCSPPINVOKE.HCSPDLL)]
	private static extern void CSharp_Set_Tessellation_Options(PK.TOPOL.render_line_o_t* line_options, PK.TOPOL.render_facet_go_o_t* go_options, PK.TOPOL.facet_mesh_o_t* generation_settings);

	public static void Set_Tessellation_Options(PK.TOPOL.render_line_o_t* line_options, PK.TOPOL.render_facet_go_o_t* go_options, PK.TOPOL.facet_mesh_o_t* generation_settings)
	{
		g_lineOptions = *line_options;
		g_facetOptions.go_option = *go_options;
		g_facetOptions.control = *generation_settings;
		CSharp_Set_Tessellation_Options(line_options, go_options, generation_settings);
	}

	public static void Set_Tessellation_Options(ref PK.TOPOL.render_line_o_t line_options, ref PK.TOPOL.render_facet_go_o_t go_options, ref PK.TOPOL.facet_mesh_o_t generation_settings)
	{
		g_lineOptions = line_options;
		g_facetOptions.go_option = go_options;
		g_facetOptions.control = generation_settings;
		fixed (PK.TOPOL.render_line_o_t* p_line_options = &line_options)
		{
			fixed (PK.TOPOL.render_facet_go_o_t* p_go_options = &go_options)
			{
				fixed (PK.TOPOL.facet_mesh_o_t* p_generation_settings = &generation_settings)
				{
					CSharp_Set_Tessellation_Options(p_line_options, p_go_options, p_generation_settings);
				}
			}
		}
	}

	public static void Show_Tessellation_Options(PK.TOPOL.render_line_o_t* line_options, PK.TOPOL.render_facet_go_o_t* go_options, PK.TOPOL.facet_mesh_o_t* generation_settings)
	{
		*line_options = g_lineOptions;
		*go_options = g_facetOptions.go_option;
		*generation_settings = g_facetOptions.control;
	}

	public static void Show_Tessellation_Options(out PK.TOPOL.render_line_o_t line_options, out PK.TOPOL.render_facet_go_o_t go_options, out PK.TOPOL.facet_mesh_o_t generation_settings)
	{
		line_options = g_lineOptions;
		go_options = g_facetOptions.go_option;
		generation_settings = g_facetOptions.control;
	}

	public static void Render_Entities(int count, PK.ENTITY_t* entities, PK.TRANSF_t view_transf, PK.TRANSF_t* topol_transf)
	{
		for (int i = 0; i < count; i++)
		{
			PK.TRANSF_t tt = topol_transf != null ? (int)topol_transf[i] : 0;
			Render_Entity(entities[i], view_transf, tt);
		}
	}

	public static void Render_Entities(int count, PK.ENTITY_t* entities, PK.TRANSF_t view_transf, PK.TRANSF_t* topol_transf, PK.TOPOL.render_line_o_t* line_options, PK.TOPOL.render_facet_o_t* facet_options)
	{
		for (int i = 0; i < count; i++)
		{
			PK.TRANSF_t tt = topol_transf != null ? (int)topol_transf[i] : 0;
			Render_Entity(entities[i], view_transf, tt, line_options, facet_options);
		}
	}

	public static void Render_Entity(PK.ENTITY_t entity, PK.TRANSF_t view_transf, PK.TRANSF_t topol_transf)
	{
		fixed (PK.TOPOL.render_facet_o_t* facet_options = &g_facetOptions)
		fixed (PK.TOPOL.render_line_o_t* line_options = &g_lineOptions)
		{
			Render_Entity(entity, view_transf, topol_transf, line_options, facet_options);
		}
	}

	public static void Render_Entity(PK.ENTITY_t entity, PK.TRANSF_t view_transf, PK.TRANSF_t topol_transf, PK.TOPOL.render_line_o_t* line_options, PK.TOPOL.render_facet_o_t* facet_options)
	{
		PK.ERROR.code_t error;
		PK.CLASS_t paraClass;

		if (entity == PK.ENTITY_t.@null)
			return;

		StringBuilder setting = new StringBuilder();

		HCSP.Show_One_Rendering_Option("create body segments", setting);
		bool merge_bodies = setting.ToString() == "on" ? true : false;

		HCSP.Show_One_Rendering_Option("merge faces", setting);
		bool merge_faces = setting.ToString() == "on" ? true : false;

		HCSP.Show_One_Rendering_Option("faces", setting);
		bool render_faces = setting.ToString() == "on" ? true : false;

		HCSP.Show_One_Rendering_Option("edges", setting);
		bool render_edges = setting.ToString() == "on" ? true : false;

		error = PK.ENTITY.ask_class(entity, &paraClass);

		if (error != PK.ERROR.code_t.no_errors)
			throw new Exception("PK.ENTITY.ask_class returned: " + error.ToString());

		if (paraClass == PK.CLASS_t.assembly)
		{
			HCSP.open_assembly_internal(entity, true, 0);
		}

		else
		{
			if (render_faces && view_transf == 0)
			{
				PK.TOPOL_t entity_topol = (PK.TOPOL_t)entity;
				error = PK.TOPOL.render_facet(1, &entity_topol, &topol_transf, view_transf, facet_options);

				if (error != PK.ERROR.code_t.no_errors)
					throw new Exception("PK.TOPOL.render_facet returned: " + error.ToString());
			}

			if (render_edges)
			{
				PK.TOPOL_t entity_topol = (PK.TOPOL_t)entity;
				error = PK.TOPOL.render_line(1, &entity_topol, &topol_transf, view_transf, line_options);

				if (error != PK.ERROR.code_t.no_errors)
					throw new Exception("PK.TOPOL.render_line returned: " + error.ToString());
			}

			// will this necessarily be a body?
			if (paraClass == PK.CLASS_t.body)
				insert_vertices_for_body(entity);

			if (merge_faces && !merge_bodies)
			{
				if (paraClass == PK.CLASS_t.body)
				{
					HCSP.merge_body_faces_internal((int*)&entity, 1);
				}
			}
		}
	}
}