
// ===========================================================================
%define ARRAY_METHOD_RET(CLASS, TYPE, NAME)
%ignore CLASS::NAME();
%ignore CLASS::NAME() const;
%extend CLASS {
	void NAME(TYPE values[], int count) {
		const TYPE *p = self->NAME();
		for (int i=0; i < count; i++)
			values[i] = p[i];
	}
}
%enddef

// Prevent generated set/get methods for array members; provide specific get method.
%define ARRAY_MEMBER_GET(CLASS, TYPE, NAME)
%ignore CLASS::NAME;
%extend CLASS {
	void get_ ## NAME(TYPE values[], int count) {
		for (int i=0; i < count; i++)
			values[i] = self->NAME[i];
	}
}
%enddef

%define STRUCT_ARRAY_MEMBER_GET(CLASS, TYPE, NAME)
%ignore ::NAME;
%extend CLASS {
	void get_ ## NAME(TYPE values[], int count) {
		for (int i=0; i < count; i++)
			values[i] = self->NAME[i];
	}
}
%enddef
// ===========================================================================


ARRAY_METHOD_RET(BStreamFileToolkit, int, GetPauseTable);
ARRAY_METHOD_RET(BStreamFileToolkit, float, GetWorldBounding);
ARRAY_METHOD_RET(TK_Rendering_Options, float, GetDiffuseTextureTintColor);
ARRAY_METHOD_RET(TK_Rendering_Options, float, getSimpleShadowLight);
ARRAY_METHOD_RET(TK_Heuristics, float, GetVector);
ARRAY_METHOD_RET(TK_Heuristics, float, GetOrderedWeights);
ARRAY_METHOD_RET(TK_Geometry_Options, float, GetOrientation);
ARRAY_METHOD_RET(TK_User_Index_Data, int, GetIndices);
ARRAY_METHOD_RET(TK_User_Index_Data, int, GetSizes);
ARRAY_METHOD_RET(HT_NURBS_Trim, float, GetPoints);
ARRAY_METHOD_RET(HT_NURBS_Trim, float, GetWeights);
ARRAY_METHOD_RET(HT_NURBS_Trim, float, GetKnots);
ARRAY_METHOD_RET(TK_Image, int, GetSize);
ARRAY_METHOD_RET(TK_Thumbnail, unsigned char, GetBytes);
ARRAY_METHOD_RET(TK_User_Data, unsigned char, GetUserData);
ARRAY_METHOD_RET(TK_Polyhedron, float, GetBounding);
ARRAY_METHOD_RET(TK_Polyhedron, float, GetVertexFaceColors);
ARRAY_METHOD_RET(TK_Polyhedron, float, GetVertexMarkerColors);
ARRAY_METHOD_RET(TK_Polyhedron, float, GetVertexMarkerRGBAs);
ARRAY_METHOD_RET(TK_Polyhedron, float, GetVertexFaceIndices);
ARRAY_METHOD_RET(TK_Polyhedron, float, GetVertexMarkerIndices);
ARRAY_METHOD_RET(TK_Polyhedron, float, GetVertexIndices);
ARRAY_METHOD_RET(HStreamFileToolkit, HC_KEY, GetExcludedSegments);

STRUCT_ARRAY_MEMBER_GET(mtable_info, int, mlengths);
STRUCT_ARRAY_MEMBER_GET(mtable_info, int, m2stackoffsets);
STRUCT_ARRAY_MEMBER_GET(mtable_info, int, m2gateoffsets);
STRUCT_ARRAY_MEMBER_GET(mtable_info, int, dummies);
STRUCT_ARRAY_MEMBER_GET(mtable_info, int, patches);
STRUCT_ARRAY_MEMBER_GET(half_edge_array, int, visitations);
STRUCT_ARRAY_MEMBER_GET(int_stack, int, data);
STRUCT_ARRAY_MEMBER_GET(varstream, long, data);
STRUCT_ARRAY_MEMBER_GET(varstream, long, rdata);
STRUCT_ARRAY_MEMBER_GET(loop_table, int, loops);
STRUCT_ARRAY_MEMBER_GET(loop_table, int, loops_edges);
STRUCT_ARRAY_MEMBER_GET(loop_table, int, P);
STRUCT_ARRAY_MEMBER_GET(loop_table, int, N);

%ignore TK_LOD::m_num_primitives;
%ignore TK_Color_Map::m_values;
%ignore TK_Rendering_Options::m_isoline_positions;
%ignore TK_Rendering_Options::m_isoline_colors;
%ignore TK_Rendering_Options::m_isoline_weights_value;
%ignore TK_Rendering_Options::m_isoline_weights_unit;
%ignore TK_User_Index::m_indices;
%ignore TK_User_Index::m_values;
%ignore TK_Polypoint::m_points;
%ignore TK_NURBS_Curve::m_control_points;
%ignore TK_NURBS_Curve::m_weights;
%ignore TK_NURBS_Curve::m_knots;
%ignore TK_NURBS_Surface::m_control_points;
%ignore TK_NURBS_Surface::m_weights;
%ignore TK_NURBS_Surface::m_u_knots;
%ignore TK_NURBS_Surface::m_v_knots;
%ignore TK_Area_Light::m_points;
%ignore TK_Cutting_Plane::m_planes;
%ignore TK_PolyCylinder::m_points;
%ignore TK_PolyCylinder::m_radii;
%ignore TK_Clip_Region::m_points;
%ignore TK_Polyhedron::mp_points;
%ignore TK_Polyhedron::mp_normals;
%ignore TK_Polyhedron::mp_params;
%ignore TK_Polyhedron::mp_vfcolors;
%ignore TK_Polyhedron::mp_vecolors;
%ignore TK_Polyhedron::mp_vmcolors;
%ignore TK_Polyhedron::mp_vmrgbas;
%ignore TK_Polyhedron::mp_vfindices;
%ignore TK_Polyhedron::mp_veindices;
%ignore TK_Polyhedron::mp_vmindices;
%ignore TK_Polyhedron::mp_vmsizes;
%ignore TK_Polyhedron::mp_vmsymbols;
%ignore TK_Polyhedron::mp_fcolors;
%ignore TK_Polyhedron::mp_findices;
%ignore TK_Polyhedron::mp_fregions;
%ignore TK_Polyhedron::mp_fnormals;
%ignore TK_Polyhedron::mp_ecolors;
%ignore TK_Polyhedron::mp_eindices;
%ignore TK_Polyhedron::mp_enormals;
%ignore TK_Polyhedron::mp_epatterns;
%ignore TK_Polyhedron::mp_eweights;
%ignore TK_Polyhedron::mp_exists;
%ignore TK_Polyhedron::mp_face_exists;
%ignore TK_Polyhedron::mp_edge_exists;
%ignore TK_Polyhedron::mp_edge_enumeration;
%ignore TK_Polyhedron::mp_workspace;
%ignore TK_Shell::m_flist;
%ignore TK_PolyPolypoint::m_points;
%ignore TK_PolyPolypoint::m_lengths;
%ignore TK_PolyPolypoint::m_workspace;
%ignore HTK_PolyPolypoint::m_keys;


