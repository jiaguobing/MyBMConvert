
// ===========================================================================
// Prevent generated set/get methods for array members; provide specific
// get method.
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

ARRAY_MEMBER_GET(HShell, HPoint,  point_list);
ARRAY_MEMBER_GET(HShell, HVector, normal_list);
ARRAY_MEMBER_GET(HShell, int,     face_list);
ARRAY_MEMBER_GET(HShell, int,     region_list);
ARRAY_MEMBER_GET(HPolyline, HPoint, m_pHPointArray);
ARRAY_MEMBER_GET(HQualifiedKey, HC_KEY, m_pIncludeList);
ARRAY_MEMBER_GET(ColorPiece, int, m_uvindex);
ARRAY_MEMBER_GET(ColorPiece, int, m_fllen);
ARRAY_MEMBER_GET(ColorPiece, int, m_flindex);
ARRAY_MEMBER_GET(HKeyframeMatrix, float, m_matrix);
ARRAY_MEMBER_GET(HKeyframeChannelCurve, HPoint, control_polygon);
ARRAY_MEMBER_GET(HKeyframeChannelFollowPath, float, tmatrix);
ARRAY_MEMBER_GET(HKeyframeChannelFollowPath, float, tmatrix2);
ARRAY_MEMBER_GET(HShellWrapper, HPoint, m_Points);
ARRAY_MEMBER_GET(HShellWrapper, HPoint, m_VParams);
ARRAY_MEMBER_GET(HShellWrapper, HPoint, m_VColors);
ARRAY_MEMBER_GET(HShellWrapper, HPoint, m_VNormals);
ARRAY_MEMBER_GET(HShellWrapper, int, m_Flist);
ARRAY_MEMBER_GET(HShellWrapper, float, m_faceregion);
ARRAY_MEMBER_GET(HShellWrapper, float, m_faceindirect);
ARRAY_MEMBER_GET(HShellWrapper, FaceWithNormalInfo, m_FNArray);
ARRAY_MEMBER_GET(HOutputHandlerOptions, float, m_PageMargins);
ARRAY_MEMBER_GET(HShowContentsWithPathType, HC_KEY, patharray);

STRUCT_ARRAY_MEMBER_GET(PairToIndex, int, bigIndices);
STRUCT_ARRAY_MEMBER_GET(segmentanimation, float, matrix);
STRUCT_ARRAY_MEMBER_GET(SharedVertex, unsigned int, IndexToPair);

// Protected array members (cannot access via get_ method above)
%ignore HBaseView::m_SceneLight;
%ignore HOpConstructCircle::m_ptCircle;
%ignore HOpConstructCuboid::m_FaceList;
%ignore HOpConstructCuboid::m_ptPointList;
%ignore HOpConstructNurbsCurve::m_KnotsArray;
%ignore HOpConstructNurbsCurve::m_WeightsArray;
%ignore HOpConstructPolyline::m_WeightsArray;
%ignore HOpConstructRectangle::m_ptRectangle;

