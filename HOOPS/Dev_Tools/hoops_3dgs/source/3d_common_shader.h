// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef _3D_COMMON_SHADER_H
#define _3D_COMMON_SHADER_H

/*****************************************************************************
*****************************************************************************
					Category: H3DShaderID Class
*****************************************************************************
*****************************************************************************/
// Which shader models do we use?
enum ShaderModel 
{ 
	SHADER_MODEL_2, 
	SHADER_MODEL_2X, 
	SHADER_MODEL_3,
	SHADER_MODEL_4,
	SHADER_MODEL_5,
	NUM_SHADER_MODELS
};

enum ShaderType
{
	VERTEX_SHADER,
	PIXEL_SHADER,
	GEOMETRY_SHADER,
	NUM_SHADER_TYPES
};


//stored on data[0]
#define H3DID_DC						0x00000001
#define H3DID_OIT						0x00000002
#define H3DID_VS_INST_POINT_SPRITES		0x00000004 
#define H3DID_LINE						0x00000008
#define H3DID_GOURAUD					0x00000010
#define H3DID_COLOR_INTERPOLATED		0x00000020
#define H3DID_COLOR_INDEX_INTERPOLATED  0x00000040
#define H3DID_VERTEX_COLORS				0x00000080
#define H3DID_VERTEX_FINDICES			0x00000100
#define H3DID_SPRITE_MARKER				0x00000200
#define H3DID_PHYSICAL_REFLECTION		0x00000400
#define H3DID_DEPTH_PEELING				0x00000800
#define H3DID_DEPTH_WRITING				0x00001000
#define H3DID_QUAD_MARKER				0x00002000
#define H3DID_USE_FACE_DISPLACEMENT		0x00004000
#define H3DID_TINTED_IMAGE				0x00008000
#define H3DID_FLAT_SHADING				0x00010000
#define H3DID_TEXTURE_MARKER			0x00020000
#define H3DID_WRITE_LUMINANCE			0x00040000
#define H3DID_CREATE_SHADOW_MAP			0x00080000
#define H3DID_FACE_PATTERN				0x00100000
#define H3DID_LINE_PATTERN				0x00200000
#define H3DID_TEXTURE_PREMULTIPLY		0x00400000
#define H3DID_SPHERE_MARKER				0x00800000
#define H3DID_SNAPPED_MARKER			0x01000000
#define H3DID_CREATE_SIMPLE_SHADOW		0x02000000
#define H3DID_CREATE_REFLECTION_PLANE	0x04000000
#define H3DID_CIRCLE_MARKER				0x08000000
#define H3DID_WORLD_SPACE_MARKER    	0x10000000
#define H3DID_RIGHT_HANDED_MATRIX		0x20000000
#define H3DID_NORMAL_FLIP_NEGATIVE		0x40000000
#define H3DID_POLY_CYLINDER				0X80000000

//stored on data[1]
#define H3DID_WIDE_LINE					0x00000001
#define H3DID_ROUND_LINE				0x00000002
#define H3DID_MITER						0x00000004
#define H3DID_ANTI_ALIASED_LINE			0x00000008
#define H3DID_SHADOW_MAP_SAMPLES		0x00000030 // 2 bits - 0..3
#define H3DID_SHADOW_MAP_SAMPLES_SHIFT  4
#define H3DID_DISTANT_SHADOW_MAP_MASK	0x000001C0 // 3 bits - 0..7
#define H3DID_DISTANT_SHADOW_MAP_SHIFT  6
#define H3DID_SPOT_SHADOW_MAP_MASK		0x00000E00 // 3 bits - 0..7
#define H3DID_SPOT_SHADOW_MAP_SHIFT     9
#define H3DID_CUTTING_PLANE_MASK		0x0000F000
#define H3DID_TEXTURE_MASK				0x000F0000
#define H3DID_SPOT_LIGHT_MASK			0x00F00000
#define H3DID_POINT_LIGHT_MASK			0x0F000000
#define H3DID_DISTANT_LIGHT_MASK		0xF0000000

//stored on data[2] (if that changes, update needed in H3DShaderID::Init)
#define H3DID_TEX0_DECAL				0x00000001
#define H3DID_TEX0_WORLD				0x00000002
#define H3DID_TEX0_COLOR				0x00000004
#define H3DID_TEX0_MODULATE				0x00000008
#define H3DID_TEX1_WORLD				0x00000010
#define H3DID_TEX1_COLOR				0x00000020
#define H3DID_TEX1_MODULATE				0x00000040
#define H3DID_TEX2_WORLD				0x00000080
#define H3DID_TEX2_COLOR				0x00000100
#define H3DID_TEX2_MODULATE				0x00000200
#define H3DID_TEX3_WORLD				0x00000400
#define H3DID_TEX3_COLOR				0x00000800
#define H3DID_TEX3_MODULATE				0x00001000
#define H3DID_TEX4_WORLD				0x00002000
#define H3DID_TEX4_COLOR				0x00004000
#define H3DID_TEX4_MODULATE				0x00008000
#define H3DID_TEX5_WORLD				0x00010000
#define H3DID_TEX5_COLOR				0x00020000
#define H3DID_TEX5_MODULATE				0x00040000
#define H3DID_TEX6_WORLD				0x00080000
#define H3DID_TEX6_COLOR				0x00100000
#define H3DID_TEX6_MODULATE				0x00200000
#define H3DID_TEX7_WORLD				0x00400000
#define H3DID_TEX7_COLOR				0x00800000
#define H3DID_TEX7_MODULATE				0x01000000
#define H3DID_HAS_HANDEDNESS			0x02000000
#define DEPTH_PACKING_RGBA				0x04000000
#define H3DID_DEPTH_WRITING_SINGLE_PASS 0x08000000
#define H3DID_DEPTH_TEXTURE_HARDWARE	0x10000000
#define H3DID_SKIP_OIT_COLLECTION		0x20000000
// FREE									0x40000000
#define H3DID_SHADOW_MAP_JITTER			0x80000000

// Stored on data[3] (if that changes, update needed in H3DShaderID::Init)
#define H3DID_GOOCH						0x00000001
#define H3DID_PER_PIXEL_LIGHTING		0x00000002
#define H3DID_ENVIRONMENT_TEXTURE		0x00000004
#define H3DID_NON_UNIFORM_SCALE			0x00000008
#define H3DID_SHADOW_MAP_RES_SHIFT				 4
#define H3DID_SHADOW_MAP_RES_MASK		0x00000070
#define H3DID_SHADOW_MAP_VIEW_SHIFT				 7
#define H3DID_SHADOW_MAP_VIEW_MASK		0x00000780
#define H3DID_SHADOW_MAP_RGBA			0x00000800
#define H3DID_AMBIENT_MATERIAL			0x00001000
#define H3DID_FORCE_GRAYSCALE			0x00002000
#define H3DID_CUBE_ENVIRONMENT_MAP		0x00004000
#define H3DID_VERTEX_RGBAS				0x00008000
#define H3DID_FRAME_BUFFER_EFFECTS		0x00010000
#define H3DID_DOUBLE_POSITIONS			0x00020000
#define H3DID_DOUBLE_TRANSFORMS			0x00040000 // DPFIXME: should we separate modelling matrix and camera cases?
// FREE									0x00080000
// FREE									0x00100000
#define H3DID_SHADOW_MAP_SLOPE_SCALE_BIAS	0x00200000
#define H3DID_DEPTH_PEELING_MODE		0x00400000
#define H3DID_PRIORITY_COMPOSITING		0x00800000
#define H3DID_PRIORITY_HSRA				0x01000000
#define H3DID_ATMOSPHERIC_ATTENUATION	0x02000000
#define H3DID_PERSPECTIVE				0x04000000
#define H3DID_POINT						0x08000000
// FREE									0x10000000
#define H3DID_TRIANGLE					0x20000000
#define H3DID_SELECTION_BITMAP			0x40000000
// FREE									0x80000000

//stored on data[4] (if that changes, update needed in H3DShaderID::Init)
#define H3DID_BACK_TEX0_DECAL			H3DID_TEX0_DECAL	
#define H3DID_BACK_TEX0_WORLD			H3DID_TEX0_WORLD	
#define H3DID_BACK_TEX0_COLOR			H3DID_TEX0_COLOR	
#define H3DID_BACK_TEX0_MODULATE		H3DID_TEX0_MODULATE	
#define H3DID_BACK_TEX1_WORLD			H3DID_TEX1_WORLD	
#define H3DID_BACK_TEX1_COLOR			H3DID_TEX1_COLOR	
#define H3DID_BACK_TEX1_MODULATE		H3DID_TEX1_MODULATE	
#define H3DID_BACK_TEX2_WORLD			H3DID_TEX2_WORLD	
#define H3DID_BACK_TEX2_COLOR			H3DID_TEX2_COLOR	
#define H3DID_BACK_TEX2_MODULATE		H3DID_TEX2_MODULATE	
#define H3DID_BACK_TEX3_WORLD			H3DID_TEX3_WORLD	
#define H3DID_BACK_TEX3_COLOR			H3DID_TEX3_COLOR	
#define H3DID_BACK_TEX3_MODULATE		H3DID_TEX3_MODULATE	
#define H3DID_BACK_TEX4_WORLD			H3DID_TEX4_WORLD	
#define H3DID_BACK_TEX4_COLOR			H3DID_TEX4_COLOR	
#define H3DID_BACK_TEX4_MODULATE		H3DID_TEX4_MODULATE	
#define H3DID_BACK_TEX5_WORLD			H3DID_TEX5_WORLD	
#define H3DID_BACK_TEX5_COLOR			H3DID_TEX5_COLOR	
#define H3DID_BACK_TEX5_MODULATE		H3DID_TEX5_MODULATE	
#define H3DID_BACK_TEX6_WORLD			H3DID_TEX6_WORLD	
#define H3DID_BACK_TEX6_COLOR			H3DID_TEX6_COLOR	
#define H3DID_BACK_TEX6_MODULATE		H3DID_TEX6_MODULATE	
#define H3DID_BACK_TEX7_WORLD			H3DID_TEX7_WORLD	
#define H3DID_BACK_TEX7_COLOR			H3DID_TEX7_COLOR	
#define H3DID_BACK_TEX7_MODULATE		H3DID_TEX7_MODULATE	
#define H3DID_BACK_TEXTURE_MASK			0xF0000000

// stored on data[5] (if that changes, update needed in H3DShaderID::Init)
#define H3DID_USER_SHADER				0x00000001    //I.M. style complete replacement
#define H3DID_TEXTURE3D					0x00000002    //3d textures
#define H3DID_SHARED_DIFFUSE_COORDS		0x00000004
#define H3DID_BGRA_TO_RGBA				0x00000008
#define H3DID_TEX0_DROP					0x00000010	  //for 'drop' tiling style, the implicit transparent border
#define H3DID_TEX1_DROP					0x00000020
#define H3DID_TEX2_DROP					0x00000040
#define H3DID_TEX3_DROP					0x00000080
#define H3DID_TEX4_DROP					0x00000100
#define H3DID_TEX5_DROP					0x00000200
#define H3DID_TEX6_DROP					0x00000400
#define H3DID_TEX7_DROP					0x00000800
#define H3DID_BACK_TEX0_DROP			H3DID_TEX0_DROP << 8
#define H3DID_BACK_TEX1_DROP			H3DID_TEX1_DROP << 8
#define H3DID_BACK_TEX2_DROP			H3DID_TEX2_DROP << 8
#define H3DID_BACK_TEX3_DROP			H3DID_TEX3_DROP << 8
#define H3DID_BACK_TEX4_DROP			H3DID_TEX4_DROP << 8
#define H3DID_BACK_TEX5_DROP			H3DID_TEX5_DROP << 8
#define H3DID_BACK_TEX6_DROP			H3DID_TEX6_DROP << 8
#define H3DID_BACK_TEX7_DROP			H3DID_TEX7_DROP << 8
#define H3DID_DEFINED_MATERIAL_PS_MASK  0x00f00000
#define	H3DID_DEFINED_MATERIAL_PS_UVW					0x00100000
#define	H3DID_DEFINED_MATERIAL_PS_WORLD					0x00200000
#define	H3DID_DEFINED_MATERIAL_PS_OBJECT				0x00300000
#define H3DID_DEFINED_MATERIAL_PS_SURFACE_NORMAL		0x00400000
#define	H3DID_DEFINED_MATERIAL_PS_LOCAL_PIXELS			0x00500000
#define H3DID_DEFINED_MATERIAL_PS_OUTER_PIXELS			0x00600000
#define	H3DID_DEFINED_MATERIAL_PS_LOCAL_WINDOW			0x00700000
#define H3DID_DEFINED_MATERIAL_PS_OUTER_WINDOW			0x00800000
#define H3DID_DEFINED_MATERIAL_PS_PHYSICAL_REFLECTION	0x00900000
#define H3DID_DEFINED_MATERIAL_MULTITEXTURE				0x01000000
#define H3DID_DEFINED_MATERIAL_GS						0x02000000
#define H3DID_DEFINED_MATERIAL_PS						0x04000000
#define H3DID_TEXTURE_EXTERNAL_OES						0x08000000
#define H3DID_SSAA_LINE_TARGET							0x10000000
#define H3DID_BEST_IMAGE_QUALITY						0x20000000
// FREE													0x40000000
// FREE													0x80000000

// data[6] is completely reserved for user shaders specified by HC_Define_Shader
#define H3DID_DEFINED_MATERIAL_SHADER_MASK	0xFFFFFFFF

// data[7] is for geometry shader bits
#define H3DID_GS_ELLIPTICAL_ARC							0x00000001

#define H3DID_GS_OUTPUT_VERTICES_MASK					0x00FF0000	// up to 256 max output vertices
#define H3DID_GS_INPUT_TYPE_MASK						0x07000000	// 5 different inputs
//#define unused										0x08000000	// unused
#define H3DID_GS_OUTPUT_TYPE_MASK						0x30000000	// 3 different types
//#define unused										0x40000000	// unused
#define H3DID_HAS_GEOMETRY_SHADER						0x80000000


// data[8] is for material textures
#define H3DID_SPECULAR_TEXTURE							0x00000001
#define H3DID_SPECULAR_TEX_WORLD						0x00000002
#define H3DID_MIRROR_TEXTURE							0x00000004
#define H3DID_MIRROR_TEX_WORLD							0x00000008
#define H3DID_TRANSMISSION_TEXTURE						0x00000010
#define H3DID_TRANSMISSION_TEX_MODULATE					0x00000020
#define H3DID_TRANSMISSION_TEX_WORLD					0x00000040
#define H3DID_EMISSION_TEXTURE							0x00000080
#define H3DID_EMISSION_TEX_MODULATE						0x00000100
#define H3DID_EMISSION_TEX_WORLD						0x00000200
#define H3DID_BUMP_TEXTURE								0x00000400
#define H3DID_BUMP_TEX_WORLD							0x00000800



enum GS_Input_Type
{
	GS_Input_Point,
	GS_Input_Line,
	GS_Input_LineAdj,
	GS_Input_Triangle,
	GS_Input_TriangleAdj,
};

enum GS_Output_Type
{
	GS_Output_Point,
	GS_Output_Line,
	GS_Output_Triangle
};


class HOOPS_API H3DShaderID : public CMO
{
private:

#define H3DShaderID_SIZE 9
	unsigned int m_data[H3DShaderID_SIZE];

	void AddLights (
			Net_Rendition const & nr,
			Light_Rendition const & lr);
	void SetShadowMapResolution(int res)	{ 
		int resolution = 0;
		switch (res) {
			/* For future use 
			case 8192: resolution++; */
			case 4096: resolution++;
			case 2048: resolution++;
			case 1024: resolution++;
			case 512: 
				m_data[3] &= ~H3DID_SHADOW_MAP_RES_MASK;
				m_data[3] |= (resolution << H3DID_SHADOW_MAP_RES_SHIFT) & H3DID_SHADOW_MAP_RES_MASK;
				break;
			case 0:  
				//if we get here, we probably were never using shadow maps
				break;
			default:
				HE_PROCEDURAL_ERROR("Invalid shadow map resolution! Must be 512, 1024, 2048, or 4096.");
				break;
		}
	};
	void SetShadowMapSamples(int samples)	{
		int samp = 0;
		if (samples >= 4) samp++;
		if (samples >= 8) samp++;
		if (samples >= 16) samp++;
		samp <<= H3DID_SHADOW_MAP_SAMPLES_SHIFT;
		
		m_data[1] |= samp;
	};
	void SetDistantShadowMapCount(int count){ ASSERT(count <= 7); m_data[1] &= ~H3DID_DISTANT_SHADOW_MAP_MASK; m_data[1] |= (count << H3DID_DISTANT_SHADOW_MAP_SHIFT); };
	void SetSpotShadowMapCount(int count)	{ ASSERT(count <= 7); m_data[1] &= ~H3DID_SPOT_SHADOW_MAP_MASK; m_data[1] |= (count << H3DID_SPOT_SHADOW_MAP_SHIFT); };
	void SetCuttingPlaneCount(int count)	{ ASSERT(count <= 15); m_data[1] &= ~H3DID_CUTTING_PLANE_MASK;	m_data[1] |= (count << 12); };
	void SetTextureCount(int count)			{ m_data[1] &= ~H3DID_TEXTURE_MASK;		m_data[1] |= (H3DID_TEXTURE_MASK & (count << 16)); };
	void SetSpotLightCount(int count)		{ m_data[1] &= ~H3DID_SPOT_LIGHT_MASK;		m_data[1] |= (H3DID_SPOT_LIGHT_MASK & (count << 20)); };
	void SetPointLightCount(int count)		{ m_data[1] &= ~H3DID_POINT_LIGHT_MASK;	m_data[1] |= (H3DID_POINT_LIGHT_MASK & (count << 24)); };
	void SetDistantLightCount(int count)	{ m_data[1] &= ~H3DID_DISTANT_LIGHT_MASK;	m_data[1] |= (H3DID_DISTANT_LIGHT_MASK & (count << 28)); };

	void SetBackTextureCount(int count)		{ m_data[4] &= ~H3DID_TEXTURE_MASK;		m_data[4] |= (count << 28); };

	void SetViewDependentShadowMap(int map, bool enable)
	{ 
		ASSERT( map < 4 );
		int const shift = H3DID_SHADOW_MAP_VIEW_SHIFT + map;
		if ( enable )
			m_data[3] |= (1 << shift);
		else
			m_data[3] &= ~(1 << shift);
	}
	void SetTextureFlags(
		Net_Rendition const & nr,
		Texture_Control_List const * tcl, 
		int *texture_count, 
		bool backfaces);

	void SetGSParameters(GS_Input_Type input_type, GS_Output_Type output_type, int max_vertices)
	{
		m_data[7] |= (max_vertices << 16) & H3DID_GS_OUTPUT_VERTICES_MASK;
		m_data[7] |= (input_type << 24) & H3DID_GS_INPUT_TYPE_MASK;
		m_data[7] |= (output_type << 28) & H3DID_GS_OUTPUT_TYPE_MASK;
	}

public:
	H3DShaderID () { ZERO_ARRAY (m_data, H3DShaderID_SIZE, int); };
	~H3DShaderID() {};

	intptr_t HashCode() const {
		intptr_t code = 0;
		for (int i = 0; i<H3DShaderID_SIZE; ++i)
			code ^= m_data[i];
		return code;
	};

	H3DShaderID *Init (
		Net_Rendition const & nr,
		int type,
		void const *thing, // tristrip, polyedge or polymarker
		int count,
		int flags,
		int thing_contents = 0);

	const char *FindMaterialShaderSource(
		Net_Rendition const & nr);

	void InitTexturedRectangle () {
		m_data[0] = H3DID_DC;
		SetTextureCount(1);
	};

	void AddFlags (int flags, int index = 0) {
		ASSERT(index < H3DShaderID_SIZE);
		m_data[index] |= flags;
	};
	void SetUserShaders (intptr_t vs, intptr_t ps) {
		m_data[0] = (int) vs;
		m_data[1] = (int) ps;
		m_data[2] = (int) vs >> 16 >> 16;
		m_data[3] = (int) ps >> 16 >> 16;
		m_data[5] = H3DID_USER_SHADER;
	};
	void GetUserShaders (intptr_t *vs, intptr_t *ps) {
		if (vs)
			*vs = (m_data[0] + (m_data[2] << 16 << 16));
		if (ps)
			*ps = (m_data[1] + (m_data[3] << 16 << 16));
	}

	int GetData (int index) const {
		return m_data[index];
	};

	bool operator == (const H3DShaderID &id) const {
		return  memeq(id.m_data, m_data, sizeof(m_data)); 
	};
	bool operator != (const H3DShaderID &id) const {
		return  !memeq(id.m_data, m_data, sizeof(m_data)); 
	};
	H3DShaderID& operator = (const H3DShaderID &id) {
		COPY_MEMORY (id.m_data, sizeof(m_data), m_data);
		return *this;
	};
	bool IsDC()		const { return ANYBIT(m_data[0], H3DID_DC); };
	bool IsTriangle() const { return ANYBIT(m_data[3], H3DID_TRIANGLE); };
	bool IsLine()	const { return ANYBIT(m_data[0], H3DID_LINE); };
	bool IsPoint()	const { return ANYBIT(m_data[3], H3DID_POINT); };
	bool IsMitered()	const { return ANYBIT(m_data[1], H3DID_MITER); };
	bool IsRoundLine()	const { return ANYBIT(m_data[1], H3DID_ROUND_LINE); }; //round capped or round joined
	bool IsAntiAliasedLine() const {return ANYBIT(m_data[1], H3DID_ANTI_ALIASED_LINE); }; //AA lines
	bool HasDoublePositions() const { return ANYBIT(m_data[3], H3DID_DOUBLE_POSITIONS); };
	bool HasDoubleTransforms() const { return ANYBIT(m_data[3], H3DID_DOUBLE_TRANSFORMS); };
	bool HasVertexColors() const { return ANYBIT(m_data[0], H3DID_VERTEX_COLORS); };
	bool HasVertexRGBAs() const { return ANYBIT(m_data[3], H3DID_VERTEX_RGBAS); };
	bool HasVertexFIndices() const { return ANYBIT(m_data[0], H3DID_VERTEX_FINDICES); };
	bool IsDepthWriting()	const { return ANYBIT(m_data[0], H3DID_DEPTH_WRITING); };
	bool IsDepthPeeling()	const { return ANYBIT(m_data[0], H3DID_DEPTH_PEELING); };
	bool IsDepthTextureHardware()	const { return ANYBIT(m_data[2], H3DID_DEPTH_TEXTURE_HARDWARE); };
	bool IsQuadMarker()	const { return ANYBIT(m_data[0], H3DID_QUAD_MARKER); };
	bool IsSpriteMarker()	const { return ANYBIT(m_data[0], H3DID_SPRITE_MARKER); };
	bool IsWorldSpaceMarker()	const { return ANYBIT(m_data[0], H3DID_WORLD_SPACE_MARKER); };
	bool IsCreateShadowMap()	const { return ANYBIT(m_data[0], H3DID_CREATE_SHADOW_MAP); };
	bool IsShadowMapSlopeScaleBias()	const { return ANYBIT(m_data[3], H3DID_SHADOW_MAP_SLOPE_SCALE_BIAS); };
	//bool SelectionMapHasLo()	const { return ANYBIT(m_data[0], H3DID_SELECTION_MAP_LO); };
	bool UseFaceDisplacement() const { return ANYBIT(m_data[0], H3DID_USE_FACE_DISPLACEMENT); };
	bool IsFlatShading() const { return ANYBIT(m_data[0], H3DID_FLAT_SHADING); };
	bool HasFacePattern() const { return ANYBIT(m_data[0], H3DID_FACE_PATTERN); };
	bool HasLinePattern() const { return ANYBIT(m_data[0], H3DID_LINE_PATTERN); };
	bool IsWideLine() const { return ANYBIT(m_data[1], H3DID_WIDE_LINE); };
	//bool HasTransparencyStipple() const {return ANYBIT(m_data[0], H3DID_TRANSPARENCY_STIPPLE); };
	bool IsPerspective() const { return ANYBIT(m_data[3], H3DID_PERSPECTIVE); };
	bool IsCreateSimpleShadow() const { return ANYBIT(m_data[0], H3DID_CREATE_SIMPLE_SHADOW); }
	bool IsCreateReflectionPlane() const { return ANYBIT(m_data[0], H3DID_CREATE_REFLECTION_PLANE); }
	bool IsShadowMapJitter() const { return ANYBIT(m_data[2], H3DID_SHADOW_MAP_JITTER); }
	bool IsShadowMapRGBA()	const { return ANYBIT(m_data[3], H3DID_SHADOW_MAP_RGBA); };
	bool IsPerPixelLighting() const { return ANYBIT(m_data[3], H3DID_PER_PIXEL_LIGHTING); }
	bool IsTextureMarker() const { return ANYBIT(m_data[0], H3DID_TEXTURE_MARKER); }
	bool IsCircleMarker() const { return ANYBIT(m_data[0], H3DID_CIRCLE_MARKER); }
	bool IsSphereMarker() const { return ANYBIT(m_data[0], H3DID_SPHERE_MARKER); }
	bool IsSnappedMarker() const { return ANYBIT(m_data[0], H3DID_SNAPPED_MARKER); }
	bool IsGooch() const { return ANYBIT(m_data[3], H3DID_GOOCH); }
	bool IsPriorityCompositing() const { return ANYBIT(m_data[3], H3DID_PRIORITY_COMPOSITING); }
	bool IsPriorityHSRA() const { return ANYBIT(m_data[3], H3DID_PRIORITY_HSRA); }
	bool HasAmbientMaterial() const { return ANYBIT(m_data[3], H3DID_AMBIENT_MATERIAL); }
	bool HasDefinedMaterialPS() const { return ANYBIT(m_data[5], H3DID_DEFINED_MATERIAL_PS); }
	bool HasTexture3D() const { return ANYBIT(m_data[5], H3DID_TEXTURE3D); }
	bool HasSharedDiffuseCoords() const { return ANYBIT(m_data[5], H3DID_SHARED_DIFFUSE_COORDS); }
	bool HasDefinedMaterialShaderNeedsWindowDimensions() const { return ANYBIT(m_data[5], H3DID_DEFINED_MATERIAL_PS_LOCAL_PIXELS|H3DID_DEFINED_MATERIAL_PS_OUTER_PIXELS ); }
	bool HasDefinedMaterialShaderMultitexture() const { return ANYBIT(m_data[5], H3DID_DEFINED_MATERIAL_MULTITEXTURE); }
	bool HasDefinedMaterialGS() const { return ANYBIT(m_data[5], H3DID_DEFINED_MATERIAL_GS); }
	int DefinedMaterialShaderParameterizationSource() const { return m_data[5] & H3DID_DEFINED_MATERIAL_PS_MASK; }
	int DefinedMaterialShaderID() const { return m_data[6]; }
	bool HasForceGrayscale() const { return ANYBIT(m_data[3], H3DID_FORCE_GRAYSCALE); }
	bool IsPolyCylinder() const	{ return ANYBIT(m_data[0], H3DID_POLY_CYLINDER); }
	bool IsCubeEnvironmentMap() const { return ANYBIT(m_data[3], H3DID_CUBE_ENVIRONMENT_MAP); }
	bool HasFrameBufferEffects() const { return ANYBIT(m_data[3], H3DID_FRAME_BUFFER_EFFECTS); }
	bool IsUserShader() const { return ANYBIT(m_data[5], H3DID_USER_SHADER); }
	bool IsDepthPeelingMode() const { return ANYBIT(m_data[3], H3DID_DEPTH_PEELING_MODE); }
	bool IsOIT() const { return ANYBIT(m_data[0], H3DID_OIT); }
	bool IsSkippingOIT() const { return ANYBIT(m_data[2], H3DID_SKIP_OIT_COLLECTION); }
	bool HasSelectionBitmap() const { return ANYBIT(m_data[3], H3DID_SELECTION_BITMAP); }
	int	GetShadowMapResolution()const { return (512 << ((m_data[3] & H3DID_SHADOW_MAP_RES_MASK) >> H3DID_SHADOW_MAP_RES_SHIFT)); };
	int	GetShadowMapSamples()	const { 
		int samples = ((m_data[1] & H3DID_SHADOW_MAP_SAMPLES) >> H3DID_SHADOW_MAP_SAMPLES_SHIFT);
		switch (samples) {
			case 0: return 1;
			case 1: return 4;
			case 2: return 8;
			case 3: return 16;
			default: return 1;
		}
	};
	char GetDistantShadowMapCount()	const { return (m_data[1] >> H3DID_DISTANT_SHADOW_MAP_SHIFT) & 0x07; }
	char GetSpotShadowMapCount()		const { return (m_data[1] >> H3DID_SPOT_SHADOW_MAP_SHIFT) & 0x07; }
	char GetShadowMapCount()			const { return GetDistantShadowMapCount() + GetSpotShadowMapCount(); }
	char GetCuttingPlaneCount()		const { return (m_data[1] >> 12) & 0xF; }
	char GetTextureCount()			const { return (m_data[1] >> 16) & 0xF; }
	char GetSpotLightCount()			const { return (m_data[1] >> 20) & 0xF; }
	char GetPointLightCount()		const { return (m_data[1] >> 24) & 0xF; }
	char GetDistantLightCount()		const { return (m_data[1] >> 28) & 0xF; }

	char GetBackTextureCount()		const { return (m_data[4] >> 28) & 0XF; }

	bool HasLighting() const		  { return GetSpotLightCount() || GetPointLightCount() || GetDistantLightCount(); }

	bool IsDecal(int texnum) const { if (texnum == 0) return ANYBIT(m_data[2], H3DID_TEX0_DECAL); return false;};
	bool IsModulate(int texnum) const { 
		switch (texnum) {
			case 0:	return (
						ANYBIT(m_data[2], H3DID_TEX0_MODULATE) || 
						ANYBIT(m_data[0], H3DID_TINTED_IMAGE)); 
			case 1:	return ANYBIT(m_data[2], H3DID_TEX1_MODULATE); 
			case 2:	return ANYBIT(m_data[2], H3DID_TEX2_MODULATE); 
			case 3:	return ANYBIT(m_data[2], H3DID_TEX3_MODULATE);
			case 4:	return ANYBIT(m_data[2], H3DID_TEX4_MODULATE);
			case 5:	return ANYBIT(m_data[2], H3DID_TEX5_MODULATE);
			case 6:	return ANYBIT(m_data[2], H3DID_TEX6_MODULATE);
			case 7:	return ANYBIT(m_data[2], H3DID_TEX7_MODULATE);
			default: return false;
		}
	};
	bool IsTexColor(int texnum) const { 
		switch (texnum) {
			case 0: return ANYBIT(m_data[2], H3DID_TEX0_COLOR); 
			case 1: return ANYBIT(m_data[2], H3DID_TEX1_COLOR); 
			case 2: return ANYBIT(m_data[2], H3DID_TEX2_COLOR); 
			case 3: return ANYBIT(m_data[2], H3DID_TEX3_COLOR); 
			case 4: return ANYBIT(m_data[2], H3DID_TEX4_COLOR); 
			case 5: return ANYBIT(m_data[2], H3DID_TEX5_COLOR); 
			case 6: return ANYBIT(m_data[2], H3DID_TEX6_COLOR); 
			case 7: return ANYBIT(m_data[2], H3DID_TEX7_COLOR); 
			default: return false;
		}
	};
	bool IsWorld(int texnum) const { 
		switch (texnum) {
			case 0: return ANYBIT(m_data[2], H3DID_TEX0_WORLD); 
			case 1: return ANYBIT(m_data[2], H3DID_TEX1_WORLD); 
			case 2: return ANYBIT(m_data[2], H3DID_TEX2_WORLD); 
			case 3: return ANYBIT(m_data[2], H3DID_TEX3_WORLD); 
			case 4: return ANYBIT(m_data[2], H3DID_TEX4_WORLD); 
			case 5: return ANYBIT(m_data[2], H3DID_TEX5_WORLD); 
			case 6: return ANYBIT(m_data[2], H3DID_TEX6_WORLD); 
			case 7: return ANYBIT(m_data[2], H3DID_TEX7_WORLD); 
			default: return false;
		}
	};
	bool IsDrop(int texnum) const { 
		switch (texnum) {
			case 0: return ANYBIT(m_data[5], H3DID_TEX0_DROP); 
			case 1: return ANYBIT(m_data[5], H3DID_TEX1_DROP); 
			case 2: return ANYBIT(m_data[5], H3DID_TEX2_DROP); 
			case 3: return ANYBIT(m_data[5], H3DID_TEX3_DROP); 
			case 4: return ANYBIT(m_data[5], H3DID_TEX4_DROP); 
			case 5: return ANYBIT(m_data[5], H3DID_TEX5_DROP); 
			case 6: return ANYBIT(m_data[5], H3DID_TEX6_DROP); 
			case 7: return ANYBIT(m_data[5], H3DID_TEX7_DROP); 
			default: return false;
		}
	};


	bool IsBackDecal(int texnum) const { if (texnum == 0) return ANYBIT(m_data[4], H3DID_BACK_TEX0_DECAL); return false;};
	bool IsBackModulate(int texnum) const { 
		switch (texnum) {
			case 0:	return (
						ANYBIT(m_data[4], H3DID_BACK_TEX0_MODULATE) || 
						ANYBIT(m_data[0], H3DID_TINTED_IMAGE)); 
			case 1:	return ANYBIT(m_data[4], H3DID_BACK_TEX1_MODULATE); 
			case 2:	return ANYBIT(m_data[4], H3DID_BACK_TEX2_MODULATE); 
			case 3:	return ANYBIT(m_data[4], H3DID_BACK_TEX3_MODULATE);
			case 4:	return ANYBIT(m_data[4], H3DID_BACK_TEX4_MODULATE);
			case 5:	return ANYBIT(m_data[4], H3DID_BACK_TEX5_MODULATE);
			case 6:	return ANYBIT(m_data[4], H3DID_BACK_TEX6_MODULATE);
			case 7:	return ANYBIT(m_data[4], H3DID_BACK_TEX7_MODULATE);
			default: return false;
		}
	};
	bool IsBackTexColor(int texnum) const { 
		switch (texnum) {
			case 0: return ANYBIT(m_data[4], H3DID_BACK_TEX0_COLOR); 
			case 1: return ANYBIT(m_data[4], H3DID_BACK_TEX1_COLOR); 
			case 2: return ANYBIT(m_data[4], H3DID_BACK_TEX2_COLOR); 
			case 3: return ANYBIT(m_data[4], H3DID_BACK_TEX3_COLOR); 
			case 4: return ANYBIT(m_data[4], H3DID_BACK_TEX4_COLOR); 
			case 5: return ANYBIT(m_data[4], H3DID_BACK_TEX5_COLOR); 
			case 6: return ANYBIT(m_data[4], H3DID_BACK_TEX6_COLOR); 
			case 7: return ANYBIT(m_data[4], H3DID_BACK_TEX7_COLOR); 
			default: return false;
		}
	};
	bool IsBackWorld(int texnum) const { 
		switch (texnum) {
			case 0: return ANYBIT(m_data[4], H3DID_BACK_TEX0_WORLD); 
			case 1: return ANYBIT(m_data[4], H3DID_BACK_TEX1_WORLD); 
			case 2: return ANYBIT(m_data[4], H3DID_BACK_TEX2_WORLD); 
			case 3: return ANYBIT(m_data[4], H3DID_BACK_TEX3_WORLD); 
			case 4: return ANYBIT(m_data[4], H3DID_BACK_TEX4_WORLD); 
			case 5: return ANYBIT(m_data[4], H3DID_BACK_TEX5_WORLD); 
			case 6: return ANYBIT(m_data[4], H3DID_BACK_TEX6_WORLD); 
			case 7: return ANYBIT(m_data[4], H3DID_BACK_TEX7_WORLD); 
			default: return false;
		}
	};
	bool IsBackDrop(int texnum) const { 
		switch (texnum) {
			case 0: return ANYBIT(m_data[5], H3DID_BACK_TEX0_DROP); 
			case 1: return ANYBIT(m_data[5], H3DID_BACK_TEX1_DROP); 
			case 2: return ANYBIT(m_data[5], H3DID_BACK_TEX2_DROP); 
			case 3: return ANYBIT(m_data[5], H3DID_BACK_TEX3_DROP); 
			case 4: return ANYBIT(m_data[5], H3DID_BACK_TEX4_DROP); 
			case 5: return ANYBIT(m_data[5], H3DID_BACK_TEX5_DROP); 
			case 6: return ANYBIT(m_data[5], H3DID_BACK_TEX6_DROP); 
			case 7: return ANYBIT(m_data[5], H3DID_BACK_TEX7_DROP); 
			default: return false;
		}
	};

	bool IsTexturePremultiply() const {
		return ANYBIT(m_data[0], H3DID_TEXTURE_PREMULTIPLY);
	}

	bool HasPhysicalReflection() const { return ANYBIT(m_data[0], H3DID_PHYSICAL_REFLECTION); };
	bool HasEnvironmentTexture() const { return ANYBIT(m_data[3], H3DID_ENVIRONMENT_TEXTURE); };
	bool HasBumpTexture() const { return ANYBIT(m_data[8], H3DID_BUMP_TEXTURE); };
	bool HasBumpTextureWorld() const { return ANYBIT(m_data[8], H3DID_BUMP_TEX_WORLD); };
	bool HasSpecularTexture() const { return ANYBIT(m_data[8], H3DID_SPECULAR_TEXTURE); };
	bool HasSpecularTextureWorld() const { return ANYBIT(m_data[8], H3DID_SPECULAR_TEX_WORLD); };
	bool HasMirrorTexture() const { return ANYBIT(m_data[8], H3DID_MIRROR_TEXTURE); };
	bool HasMirrorTextureWorld() const { return ANYBIT(m_data[8], H3DID_MIRROR_TEX_WORLD); };
	bool HasTransmissionTexture() const { return ANYBIT(m_data[8], H3DID_TRANSMISSION_TEXTURE); };
	bool HasTransmissionTextureModulate() const { return ANYBIT(m_data[8], H3DID_TRANSMISSION_TEX_MODULATE); };
	bool HasTransmissionTextureWorld() const { return ANYBIT(m_data[8], H3DID_TRANSMISSION_TEX_WORLD); };
	bool HasEmissionTexture() const { return ANYBIT(m_data[8], H3DID_EMISSION_TEXTURE); };
	bool HasEmissionTextureModulate() const { return ANYBIT(m_data[8], H3DID_EMISSION_TEX_MODULATE); };
	bool HasEmissionTextureWorld() const { return ANYBIT(m_data[8], H3DID_EMISSION_TEX_WORLD); };

	bool HasHandedness() const { return ANYBIT(m_data[2], H3DID_HAS_HANDEDNESS); };
	bool IsDepthPackingRGBA() const { return ANYBIT(m_data[2], DEPTH_PACKING_RGBA); }
	bool IsDepthWritingSinglePass() const { return ANYBIT(m_data[2], H3DID_DEPTH_WRITING_SINGLE_PASS); }
	float	   GetNormalFlipSign() const { return ANYBIT(m_data[0], H3DID_NORMAL_FLIP_NEGATIVE) ? -1.0f : +1.0f; }
	int		   RightHandedMatrix() const { return ANYBIT(m_data[0], H3DID_RIGHT_HANDED_MATRIX); }
	bool HasAtmosphericAttenuation() const { return ANYBIT(m_data[3], H3DID_ATMOSPHERIC_ATTENUATION); };
	bool HasNonUniformScale()	const {return ANYBIT(m_data[3], H3DID_NON_UNIFORM_SCALE); };
	bool IsWritingLuminance() const { return ANYBIT(m_data[0], H3DID_WRITE_LUMINANCE); }
	bool HasSSAALineTarget() const {return ANYBIT(m_data[5], H3DID_SSAA_LINE_TARGET); }
	bool IsBestImageQuality() const { return ANYBIT(m_data[5], H3DID_BEST_IMAGE_QUALITY); }
	bool IsBGRAToRGBA() const { return ANYBIT(m_data[5], H3DID_BGRA_TO_RGBA); }
	bool IsTextureExternalOES() const { return ANYBIT(m_data[5], H3DID_TEXTURE_EXTERNAL_OES); }
	bool HasGeometryShader() const {return ANYBIT(m_data[7], H3DID_HAS_GEOMETRY_SHADER); }
	bool IsGSEllipticalArc() const {return ANYBIT(m_data[7], H3DID_GS_ELLIPTICAL_ARC); }
	bool IsVertexShaderInstancedPointSprites() const { return ANYBIT(m_data[0], H3DID_VS_INST_POINT_SPRITES); }
	int GetGSMaxOutputVertices() const {return ((m_data[7] & H3DID_GS_OUTPUT_VERTICES_MASK) >> 16); }
	GS_Input_Type GetGSInputType() const {return (GS_Input_Type) ((m_data[7] & H3DID_GS_INPUT_TYPE_MASK) >> 24); }
	GS_Output_Type GetGSOutputType() const {return (GS_Output_Type) ((m_data[7] & H3DID_GS_OUTPUT_TYPE_MASK) >> 28); }

	// Is this specific shadow map view dependent?
	bool IsViewDependentShadowMap(int map) const
	{
		ASSERT( map < 4 );
		int const shift = H3DID_SHADOW_MAP_VIEW_SHIFT + map;
		return ANYBIT(m_data[3], (1 << shift));
	}

	// Are any shadow maps view dependent?
	bool IsAnyViewDependentShadowMap() const
	{
		return (m_data[3] & H3DID_SHADOW_MAP_VIEW_MASK) != 0;
	}

	void GetTextureUnitUsage(int usage, int number, int *vin, int *vout, int *sampler) const;


	// Determine which model this shader would work best with
	ShaderModel PreferredShaderModel()
	{
		if (IsPerPixelLighting())
			// Need SM3 for face register
			return SHADER_MODEL_3;

		if (HasBumpTexture())
			// Need SM3 for derivatives.
			return SHADER_MODEL_3;

		if (GetShadowMapCount() > 0)
			// Shadow mapping is expensive, we prefer SM 3 here too
			return SHADER_MODEL_3;

		if (IsUserShader() || HasDefinedMaterialPS())
			// User shaders can be complex
			return SHADER_MODEL_3;

		if (IsShadowMapSlopeScaleBias())
			return SHADER_MODEL_3;

		// SM2 is fine
		return SHADER_MODEL_2;
	}
	// Remove material shader settings for this shader. Return false when this wasn't possible
	bool RemoveMaterialShader()
	{
		bool success = true;

		if ( HasDefinedMaterialPS() || HasDefinedMaterialGS() )
		{
			m_data[6] = 0;
			m_data[5] &= ~(H3DID_DEFINED_MATERIAL_PS_LOCAL_PIXELS|H3DID_DEFINED_MATERIAL_PS_OUTER_PIXELS);
			m_data[5] &= ~H3DID_DEFINED_MATERIAL_MULTITEXTURE;
			m_data[5] &= ~H3DID_DEFINED_MATERIAL_PS_MASK;

			m_data[5] &= ~(H3DID_DEFINED_MATERIAL_PS|H3DID_DEFINED_MATERIAL_GS);
		}
		else
			success = false;

		return success;
	}

	// Reduce quality settings of this shader. Return false when no reduction was possible.
	bool ReduceQuality()
	{
		bool success = true;

		// Shadowmap samples are expensive - start here.
		int samples = ((m_data[1] & H3DID_SHADOW_MAP_SAMPLES) >> 6);
		if (samples > 0)
		{
			// Reduce by one level
			--samples;
			m_data[1] &= ~H3DID_SHADOW_MAP_SAMPLES;
			SetShadowMapSamples(samples * 4);
		}
		else if (IsShadowMapJitter())
		{
			// Jittered sampling is expensive, disable that next
			m_data[2] &= ~H3DID_SHADOW_MAP_JITTER;
		}
		else if (GetSpotShadowMapCount() > 0)
		{
			// Shadowmaps are expensive
			SetSpotShadowMapCount(GetSpotShadowMapCount() - 1);
		}
		else if (GetDistantShadowMapCount() > 0)
		{
			// Shadowmaps are expensive
			SetDistantShadowMapCount(GetDistantShadowMapCount() - 1);
		}
		else if (IsPerPixelLighting())
		{
			// Shadowmaps require per pixel lighting, so we can't drop that until after
			// we've eliminated them.
			m_data[3] &= ~H3DID_PER_PIXEL_LIGHTING;
		}
		/* Other ways to reduce quality would go here, in decreasing order of effectiveness/
		   increasing order of consequence...
		else
		{
		}
		*/
		else
		{
			// Nothing else to do
			success = false;
		}

		return success;
	}

	friend class H3DShaderHASH;
	friend class H3DShader;
};


/*****************************************************************************
*****************************************************************************
						Category: H3DShader Class
*****************************************************************************
*****************************************************************************/
#define SHADER_SCRATCH_SIZE 131072

#ifdef INCLUDED_FROM_DX9_DRIVER
typedef void * H3D_Shader_Handle;
#else
typedef intptr_t H3D_Shader_Handle;
#endif

#define INITIAL_SHADER_HANDLE		(H3D_Shader_Handle)(-1)
#define INVALID_SHADER_HANDLE		(H3D_Shader_Handle)(-2)

enum H3D_Shader_Constant_Type {
	H3D_Float,
	H3D_Integer,
	H3D_Matrix,
	H3D_Vector,
	H3D_DMatrix,
	H3D_Double,
};

enum H3D_Constant_Table {
	H3D_VSConstant_Table,
	H3D_PSConstant_Table,
	H3D_GSConstant_Table,
};

void map_camera_to_slice( H3DData *h3ddata, const Shadow_Map::Slice& slice, int const index, int const size, Vector& scale, Vector& bias );

enum H3D_Shader_Handle_Type {
	SHC_MODEL,
	SHC_MODELVIEW,
	SHC_PROJECTION,
	SHC_NORMALIZEDMODELVIEW,

	SHC_MODEL_GS,
	SHC_MODELVIEW_GS,
	SHC_PROJECTION_GS,
	SHC_NORMALIZEDMODELVIEW_GS,
	SHC_WORLDVIEWPROJ_GS,

	SHC_MATERIALSPECULAR_PS,
	SHC_MATERIALSPECULAR_VS,
	SHC_COLORMAPSIZE,
	SHC_MIRROR,
	SHC_CAMERAPOS,
	SHC_EYECAMERAPOS,
	SHC_GOOCH_WEIGHTS,

	SHC_AMBIENT_UP,
	SHC_AMBIENT_DOWN,
	SHC_AMBIENT_DIR,
	SHC_EMISSIVE,
	SHC_TRANSMISSION_MODULATE,
	SHC_SPECULAR_TINT,

	SHC_DIFFUSE_SAMPLER/*[8]*/,
	SHC_BACK_DIFFUSE_SAMPLER/*[8]*/						= SHC_DIFFUSE_SAMPLER + 8,
	SHC_BUMP_SAMPLER									= SHC_BACK_DIFFUSE_SAMPLER + 8,
	SHC_SPECULAR_SAMPLER,
	SHC_ENVIRONMENT_SAMPLER,
	SHC_FACE_PATTERN_SAMPLER,
	SHC_LINE_PATTERN_SAMPLER,
	SHC_LINE_TOOLS_SAMPLER,
	SHC_INDEX_SAMPLER,
	SHC_GOOCH_SAMPLER,

	SHC_DEPTH_PEELING_SAMPLER,
	SHC_DEPTH_PEELING_SCALE_AND_BIAS,
	SHC_DEPTH_WRITE_SCALE,

	SHC_SHADOW_MAP_SAMPLER/*[4] + 1 for jitter*/,
	SHC_SHADOW_MAPS/*[H3D_MAX_SHADOW_MAPS]*/			= SHC_SHADOW_MAP_SAMPLER + 4 + 1,

	SHC_SHADOW_SLICE_SCALES/*[H3D_MAX_SHADOW_MAPS]*/	= SHC_SHADOW_MAPS + H3D_MAX_SHADOW_MAPS,
	SHC_SHADOW_SLICE_BIASES/*[H3D_MAX_SHADOW_MAPS]*/	= SHC_SHADOW_SLICE_SCALES + H3D_MAX_SHADOW_MAPS,

	SHC_MODULATIONCOLOR/*[H3D_MAX_TEXTURES]*/			= SHC_SHADOW_SLICE_BIASES + H3D_MAX_SHADOW_MAPS,

	SHC_ATMOSPHERICATTENUATION_HITHER					= SHC_MODULATIONCOLOR + H3D_MAX_TEXTURES,
	SHC_ATMOSPHERICATTENUATION_YON,
	SHC_ATMOSPHERICATTENUATION_COLOR,

	SHC_REFLECTION_PLANE,
	SHC_REFLECTION_PLANE_SCALE,
	SHC_REFLECTION_PLANE_BIAS,

	SHC_SELECTION_KEY_HI,
	SHC_SELECTION_KEY_LO_BASE,
	SHC_SELECTION_KEY_LO_MASK,

	SHC_SELECTION_BITMAP_OFFSET,

	SHC_SELECTION_BITMAP_SAMPLER,
	SHC_SELECTION_TABLE_SAMPLER,

	SHC_POLY_CYLINDER_SCALE,

	SHC_ALPHA_TEST,

	SHC_MARKER_SIZE,
	SHC_MARKER_SNAP,
	SHC_MARKER_UNSNAP,
	SHC_MARKER_SAMPLER,
	SHC_MARKER_BIAS,

	SHC_MARKER_SIZE_GS,

	SHC_WORLDVIEWPROJ,
	SHC_COLOR,
	SHC_LINEWEIGHT,
	SHC_CIRCLESUBIMAGESTART,
	SHC_COSJOINCUTOFFANGLE,

	SHC_VIEWPORT_SCALE_AND_BIAS,
	SHC_JITTER_SCALE_AND_BIAS,
	SHC_WINDOW_WIDTH,
	SHC_WINDOW_HEIGHT,
	SHC_FACE_PATTERN_OFFSET,
	SHC_FACECONTRASTCOLOR,
	SHC_LINECONTRASTCOLOR,
	SHC_FACEDISPLACEMENT,
	SHC_GENERALDISPLACEMENT,
	SHC_INDEX_TEXTURE_MATRIX,
	SHC_FRONT_DIFFUSE_ENABLE,
	SHC_TEXTURE/*[H3D_MAX_TEXTURES]*/,
	SHC_BUMP_TEXTURE_MATRIX			= SHC_TEXTURE + H3D_MAX_TEXTURES,
	SHC_SPECULAR_TEXTURE_MATRIX,
	SHC_TRANSMISSION_TEXTURE_MATRIX,
	SHC_EMISSION_TEXTURE_MATRIX,
	SHC_MIRROR_TEXTURE_MATRIX,
	SHC_ENVIRONMENT_TEXTURE_MATRIX,
	SHC_MATERIALDIFFUSE_PS,
	SHC_MATERIALDIFFUSE_VS,
	SHC_CUTTINGPLANE/*[H3D_MAX_CUTTING_PLANES]*/,
	SHC_SCRATCH						= SHC_CUTTINGPLANE + H3D_MAX_CUTTING_PLANES,		// USED WHEN WE DON'T CARE ABOUT CACHING SOMETHING

	SHC_WIDTH,
	SHC_MAX_FRAGMENTS,
	SHC_TIME,

	SHC_DISTANTLIGHT_DIRECTION,
	SHC_DISTANTLIGHT_DIFFUSE		= SHC_DISTANTLIGHT_DIRECTION + 15,
	SHC_DISTANTLIGHT_SPECULAR		= SHC_DISTANTLIGHT_DIFFUSE + 15,
	SHC_DISTANTLIGHT_HALFWAY		= SHC_DISTANTLIGHT_SPECULAR + 15,
	SHC_POINTLIGHT_POSITION			= SHC_DISTANTLIGHT_HALFWAY + 15,
	SHC_POINTLIGHT_DIFFUSE			= SHC_POINTLIGHT_POSITION + 15,
	SHC_POINTLIGHT_SPECULAR			= SHC_POINTLIGHT_DIFFUSE + 15,
	SHC_SPOTLIGHT_POSITION			= SHC_POINTLIGHT_SPECULAR + 15,
	SHC_SPOTLIGHT_DIRECTION			= SHC_SPOTLIGHT_POSITION + 15,
	SHC_SPOTLIGHT_ANGLES			= SHC_SPOTLIGHT_DIRECTION + 15,
	SHC_SPOTLIGHT_DIFFUSE			= SHC_SPOTLIGHT_ANGLES + 15,
	SHC_SPOTLIGHT_SPECULAR			= SHC_SPOTLIGHT_DIFFUSE + 15,
	SHC_LIGHTS_END					= SHC_SPOTLIGHT_SPECULAR + 15,	//used to give spotlight_specular 15 spots

	SHC_SHADOW_BIAS_PS,

	SHC_END,
};

enum HTextureDimension
{
	TEXTURE_2D,
	TEXTURE_3D,
	TEXTURE_CUBE,
	TEXTURE_DIMENSION_COUNT,
};

enum HTexelType
{
	TEXEL_FLOAT4,
	TEXEL_UINT,
	TEXEL_TEXTURE_EXTERNAL_OES,
	TEXEL_TYPE_COUNT
};

enum HMaterialShaderTarget
{
	Target_VS,
	Target_GS,
	Target_PS,
	Target_COUNT
};

// Returns a bitfield of (1<<Target) for all targets present.
int get_material_shader_targets(const char *source);

class HOOPS_API H3DShader : public CMO_Cached
{
private:
	H3DShader(H3DShader const &);

protected:
	H3D_Shader_Handle cache[SHC_END];

	const H3DShaderID	m_key;				// This ID was used to create the shader, and never changes. It is used for finding the shader.
	H3DShaderID			m_id;				// This ID is used at runtime to configure the shader. It may be different from the key if the shader was reduced in complexity.
	bool				m_shader_must_fog;	// Whether or not this shader need to manually handle fog
	H3DData *			m_h3ddata;

	INLINE static char const *	eat_white_spaces (char const * ptr);
	INLINE static char const *	grab_token (char const * start, char * buffer);
	INLINE static char const *	kill_block (char const * start);
	bool resolve_expression (intptr_t_Lookup_Hash * macros, char const * ptr) const ;

	// Define dummy copy-assignment operator since compiler can't.
	void operator=(const H3DShader& other) 
	{
		(void)other;
		ASSERT(0);
	}

	// returns "glsl", "hlsl", etc. depending on driver
	virtual char const * get_shader_extension() const {return null;};

	bool DumpHLSL (char const *shader_name, char const *contents) const;	
	static void BuildShaderName (H3DShaderID const & id, ShaderType type, char * const name);	
	uint32_t ReadCacheFile(const char * filename, void * buffer) const;
	void WriteCacheFile(const char * filename, void const * data, uint32_t length) const;

protected:
	
	bool StripHLSL (Memory_Pool * pool, char *contents, int *length_out);
	virtual void GetBuiltInMacros(Memory_Pool *, intptr_t_Lookup_Hash *) { };

	char *ComposePreamble (H3DShaderID &id, char *ptr, char const *end, ShaderModel model, ShaderType type);
	char *ExpandMacros (Memory_Pool * pool, const char *filename, const char *string, char *contents, char const *end, ShaderType type);
	char *AppendBaseShader (const char *filename, const char *string, char *ptr, char const *end);
	bool AppendMaterialShader (const char *source, HMaterialShaderTarget target, char *&ptr, char const *end);
	char *InjectCode (const char *new_source, const char *keyword, char *current_source, int current_source_length, char const *buffer_end);	
	virtual char *EmitTexture(char *ptr, HTextureDimension dimension, char const *name, int number, int sampler, HTexelType texel_type = TEXEL_FLOAT4) = 0;

	void ApplyFacePattern(Net_Rendition const & nr);

public:

	int m_tr_incarnation;
	int m_lir_incarnation;
	int m_matr_incarnation;
	int m_mr_incarnation;	

	H3DShader (const H3DShaderID & id);
	virtual ~H3DShader () {};

	virtual bool Create(H3DData * h3ddata, const char * material_shader_source = null) = 0;
	virtual bool Activate() = 0;

	void Force2DTransform (float * matrix);
	void SetModellingMatrix (H3DData const * h3ddata);
	void Ensure3DTransform (Net_Rendition const & nr);
	void EnsureDCTransform (Net_Rendition const & nr);
	void EnsureColor (RGBA const & color);
	void EnsureLights (Net_Rendition const & nr, Type_Flags type, bool backface);
	void EnsureLineStyle (Net_Rendition const & nr, Line_Rendition const & er);
	void EnsureMarkerStyle (Net_Rendition const & nr, float size, bool snap, bool quads, H3DTexture * glyph);
	void EnsureCuttingPlanes (Net_Rendition const & nr, Cutting_Plane_Set const * cutset);
	void EnsureDCCuttingPlanes (Cutting_Plane_Set const * cutset);
	void EnsureReflectionPlane (ReflectionPlaneConstants const & constants);
	void EnsurePolyCylinder (Net_Rendition const & nr);
// 	void EnsureSelectionMap (Net_Rendition const & nr, const Selection_Key_Helper * helper);
// 	void EnsureSelectionBitmap (Net_Rendition const & nr);
	virtual void EnsureDepthPeeling(H3DTexture * texture, H3DData * h3ddata) = 0;
	virtual void EnsureTextures (bool back_face = false) = 0;

	// Shader's key is used to identify it.
	INLINE const H3DShaderID Key() const { return m_key; }

	// Shader's ID represents how the shader is configured. Don't use this for finding the shader
	// as it can change!
	INLINE const H3DShaderID ID() const { return m_id; }


protected:

	// handle: cached handle to a shader constant
	// data: buffer containing the values
	// num: number of values in data (to set arrays)
	// datatype: describes type of data (matrix, float, etc)
	virtual void SetConstantInt(H3D_Constant_Table table, int type, int data) = 0;
	virtual void SetConstantUInt(H3D_Constant_Table table, int type, unsigned int data) = 0;
	virtual void SetConstantFloat(H3D_Constant_Table table, int type, float data) = 0;
	virtual void SetConstantArray(H3D_Constant_Table table, int type, void const *data, int datatype, int num = 1) = 0;
	virtual void SetConstantVector(H3D_Constant_Table table, int type, float const * data, int width) = 0;
	virtual void SetConstantVectorArray(H3D_Constant_Table table, int type, float const * data, int num_vectors, int width) = 0;
	virtual H3D_Shader_Handle GetConstantByName(H3D_Constant_Table table, int type, char const * parent, char const * constant) = 0;

	INLINE H3D_Shader_Handle ENSURE_CONSTANT_HANDLE(H3D_Constant_Table const_table, int handlecache, const char * parent, const char * constant) {
		if (cache[handlecache] == INITIAL_SHADER_HANDLE)
			cache[handlecache] = GetConstantByName(const_table, handlecache, parent, constant);
		return cache[handlecache];
	}

	INLINE void SET_CONSTANT_ARRAY(H3D_Constant_Table const_table, int handlecache, const char * parent, const char * constant, void const * value, int size, int type) {
		if (ENSURE_CONSTANT_HANDLE(const_table, handlecache, parent, constant) != INVALID_SHADER_HANDLE)		
			SetConstantArray(const_table, handlecache, value, type, size); 
	}

	INLINE void SET_CONSTANT_FLOAT(H3D_Constant_Table const_table, int handlecache, const char * parent, const char * constant, float value) {
		if (ENSURE_CONSTANT_HANDLE(const_table, handlecache, parent, constant) != INVALID_SHADER_HANDLE)
			SetConstantFloat(const_table, handlecache, value); 
	}

	INLINE void SET_CONSTANT_FLOAT_ARRAY(H3D_Constant_Table const_table, int handlecache, const char * parent, const char * constant, void const * value, int size) {
		SET_CONSTANT_ARRAY(const_table, handlecache, parent, constant, value, size, H3D_Float);
	}

	INLINE void SET_CONSTANT_DOUBLE_ARRAY(H3D_Constant_Table const_table, int handlecache, const char * parent, const char * constant, void const * value, int size) {
		SET_CONSTANT_ARRAY(const_table, handlecache, parent, constant, value, size, H3D_Double);
	}

	INLINE void SET_CONSTANT_INT(H3D_Constant_Table const_table, int handlecache, const char * parent, const char * constant, int value) {
		if (ENSURE_CONSTANT_HANDLE(const_table, handlecache, parent, constant) != INVALID_SHADER_HANDLE)
			SetConstantInt(const_table, handlecache, value);
	}

	INLINE void SET_CONSTANT_UINT(H3D_Constant_Table const_table, int handlecache, const char * parent, const char * constant, unsigned int value) {
		if (ENSURE_CONSTANT_HANDLE(const_table, handlecache, parent, constant) != INVALID_SHADER_HANDLE)
			SetConstantUInt(const_table, handlecache, value);
	}

	INLINE void SET_CONSTANT_INT_ARRAY(H3D_Constant_Table const_table,int handlecache, const char * parent, const char * constant, int const * value, int size) {
		SET_CONSTANT_ARRAY(const_table, handlecache, parent, constant, value, size, H3D_Integer);
	}

	INLINE void SET_CONSTANT_MATRIX(H3D_Constant_Table const_table,int handlecache, const char *  parent, const char * constant, void const * value, int num_elements) {
		SET_CONSTANT_ARRAY(const_table, handlecache, parent, constant, value, num_elements, H3D_Matrix);
	}

	INLINE void SET_CONSTANT_DMATRIX(H3D_Constant_Table const_table, int handlecache, const char *  parent, const char * constant, void const * value, int num_elements) {
		SET_CONSTANT_ARRAY(const_table, handlecache, parent, constant, value, num_elements, H3D_DMatrix);
	}

	// matrix arrays are not supported at this time
	INLINE void SET_CONSTANT_MATRIX_ARRAY(H3D_Constant_Table const_table, int handlecache, const char * parent, const char * constant, float const * value, int num_elements, int /*size*/) {
		ASSERT(0); 
		SET_CONSTANT_ARRAY(const_table, handlecache, parent, constant, value, num_elements, H3D_Matrix);
	}

	INLINE void SET_CONSTANT_VECTOR(H3D_Constant_Table const_table, int handlecache, const char * parent, const char * constant, float const * value, int width) {
		if (ENSURE_CONSTANT_HANDLE(const_table, handlecache, parent, constant) != INVALID_SHADER_HANDLE)
			SetConstantVectorArray(const_table, handlecache, value, 1, width);
	}

	INLINE void SET_CONSTANT_VECTOR_ARRAY(H3D_Constant_Table const_table, int handlecache, const char * parent, const char * constant, float const * value, int size, int width) {
		if (ENSURE_CONSTANT_HANDLE(const_table, handlecache, parent, constant) != INVALID_SHADER_HANDLE)
			SetConstantVectorArray(const_table, handlecache, value, size, width);
	}
};

/*****************************************************************************
*****************************************************************************
					Category: H3DShaderHash Class
*****************************************************************************
*****************************************************************************/
local void shader_cleanup_callback(H3DShader *shader, intptr_t dummy, void const * const dummy2)
{
	H_SAFE_DELETE(shader);
	UNREFERENCED(dummy);
	UNREFERENCED(dummy2);
}

typedef VHash<intptr_t, H3DShader *> ID_2_Shader_Hash;

class H3DShaderHASH : public CMO
{

private:
	ID_2_Shader_Hash *	m_hash;
	H3DData *			m_h3ddata;
	H3DShader *			m_last_shader;
public:
	H3DShaderHASH (H3DData *h3ddata, int nominal_size=1024) {
		m_h3ddata = h3ddata;
		m_last_shader = null;
		m_hash = DEFAULT_POOL_NEW(ID_2_Shader_Hash)(DEFAULT_MEMORY_POOL(), nominal_size);
	}
	~H3DShaderHASH () {
		m_hash->MapFunction(shader_cleanup_callback, m_h3ddata);
		delete m_hash;
		m_hash = null;
	}

	void Insert (const H3DShaderID &id, H3DShader *shader) {
		m_hash->InsertItem(id.HashCode(), shader);
	}
	H3DShader *Lookup (const H3DShaderID &id) {

		if (m_last_shader != null && m_last_shader->Key() == id)
			return m_last_shader;

		for (int n = 0; ; n++) {
			H3DShader *	shader;
			int status = m_hash->LookupNthItem(id.HashCode(), n, &shader);
			if (status != ID_2_Shader_Hash::Success)
				return null;
			if (shader->Key() == id) {
				m_last_shader = shader;
				return shader;
			}
		}
	}

};

#endif //3D_COMMON_SHADER_H
