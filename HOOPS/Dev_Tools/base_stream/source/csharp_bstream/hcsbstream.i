//swig -v -csharp -c++ -dllimport hcsstream$hversion hcsstream.i";

%module(directors="1", allprotected="0") HCSBSTREAM

#pragma SWIG nowarn=302			// Identifier redefined
#pragma SWIG nowarn=350,351		// Op new/delete ignored
#pragma SWIG nowarn=516			// Overloaded method ignored

// ================================================================================
%pragma(csharp) imclassclassmodifiers = 
%{
#if _M_X64
using HLONG = System.Int64;
#else
using HLONG = System.Int32;
#endif

public class %}

%pragma(csharp) moduleclassmodifiers = 
%{
#if _M_X64
using HLONG = System.Int64;
#else
using HLONG = System.Int32;
#endif
public class %}

%typemap(csimports) SWIGTYPE %{
using System;
using System.Runtime.InteropServices;

#if _M_X64
using HLONG = System.Int64;
#else
using HLONG = System.Int32;
#endif

%}

// Ensure swigStringHelper is initialized since its initialization may get optimized out
%pragma(csharp) imclasscode = %{
  static $imclassname() {
   if (swigStringHelper == null)
    swigStringHelper = new SWIGStringHelper();
  }
%}

// Helper methods for methods/properties returning array as IntPtr
%pragma(csharp) modulecode=%{
    public static float[] ExtractFloatArray(IntPtr source, int count)
    {
        float[] array = new float[count];
        Marshal.Copy(source, array, 0, count);
        return array;
    }

    public static int[] ExtractIntArray(IntPtr source, int count)
    {
        int[] array = new int[count];
        Marshal.Copy(source, array, 0, count);
        return array;
    }

    public static T[] ExtractArray<T>(IntPtr source, int count)
    {   
        // This function can handle everything to two above can, but it copies elements one at a
        // time so it's not very efficient to use for copying raw floats or ints.
        Type type = typeof(T);
        int size = Marshal.SizeOf(type);
        T[] array = new T[count];

        IntPtr ptr = source;

        for (int i = 0; i < count; i++)
        {
            array[i] = (T)Marshal.PtrToStructure(ptr, type);
            ptr = new IntPtr((long)ptr + size);
        }

        return array;
    }
%}

// ================================================================================

%{
#define alter
#define HC_KEY long

#include <math.h>

#ifdef WIN64
#	define HLONG __int64
#	define  long __int64
#else
#	define HLONG long
#endif

#include "stream_common\BStream.h"
#include "stream_common\BStreamFileToolkit.h"
#include "stream_common\BByteStream.h"
#include "stream_common\BCompress.h"
#include "stream_common\BEdgeBreaker.h"
#include "stream_common\BInternal.h"
#include "stream_common\BOpcodeHandler.h"
#include "stream_common\BPolyhedron.h"
#include "stream_common\BOpcodeShell.h"
#include "stream_common\BPointCloud.h"
#include "stream_common\BPolyPoly.h"

class HTK_PolyCylinder : public TK_PolyCylinder {
};
%}

%csmethodmodifiers TK_Mesh::Read "public override"
%csmethodmodifiers TK_Mesh::Write "public override"
%csmethodmodifiers TK_Mesh::Reset "public override"
%csmethodmodifiers TK_Shell::Read "public override"
%csmethodmodifiers TK_Shell::Write "public override"
%csmethodmodifiers TK_Shell::Reset "public override"
%csmethodmodifiers TK_PolyCylinder::Read "public override"
%csmethodmodifiers TK_PolyCylinder::Write "public override"
%csmethodmodifiers TK_PolyCylinder::Reset "public override"
%csmethodmodifiers TK_Polyhedron::Read "public override"
%csmethodmodifiers TK_Polyhedron::Write "public override"
%csmethodmodifiers TK_Polyhedron::Reset "public override"

%csmethodmodifiers TK_PolyCylinder::Clone "public override"
%csmethodmodifiers TK_PolyCylinder::SetPoints "public new"
%csmethodmodifiers TK_PolyCylinder::GetPoints "public new"

%rename(GetTKType) GetType;


// ================================================================================
%define INOUT_TYPEMAP(TYPE, CTYPE, CSTYPE, TYPECHECKPRECEDENCE)

%typemap(ctype) TYPE *, TYPE & "CTYPE *"

// 'TYPE *' and 'TYPE &' separated here because we're using '*' for arrays and output values.
%typemap(imtype) TYPE & "CSTYPE[]"
%typemap(imtype, out="IntPtr") TYPE * "CSTYPE[]"
%typemap(cstype) TYPE & "CSTYPE[]"
%typemap(cstype, out="IntPtr") TYPE * "CSTYPE[]"

%typemap(csin) TYPE *, TYPE & "$csinput"
%typemap(csout) TYPE *, TYPE & {return $imcall;}
%typemap(in) TYPE *, TYPE & "$1 = ($1_ltype)$input;"
%typemap(out) TYPE *, TYPE & "$result = result;"
%typemap(csdirectorin) TYPE *, TYPE & "$iminput" 
%typemap(csdirectorout) TYPE *, TYPE & "$cscall"

%typemap(directorin) TYPE * "$input = (CTYPE *) $1;"
%typemap(directorin) TYPE & "$input = &$1;"

%typemap(csvarin) TYPE *, TYPE &
 %{
   set {
	 $imcall;
   } %}
%typemap(csvarout) TYPE *, TYPE &
 %{
   get {
	 return $imcall;
   } %}

/* Handles arrays */
%typemap(ctype) TYPE [] "CTYPE *"
%typemap(imtype) TYPE [] "CSTYPE[]"
%typemap(cstype) TYPE [] "CSTYPE[]"
%typemap(csin) TYPE [] "$csinput"
%typemap(csout) TYPE [] {return $imcall;}
%typemap(in) TYPE []
%{ $1 = ($1_ltype)$input; %}
%typemap(csvarin) TYPE []
 %{
   set {
	 $imcall;
   } %}
%typemap(csvarout) TYPE []
 %{
   get {
	 return $imcall;
   } %}

%typecheck(SWIG_TYPECHECK_##TYPECHECKPRECEDENCE) TYPE *INOUT, TYPE &INOUT ""
%enddef

// ================================================================================

INOUT_TYPEMAP(bool,               unsigned int,         bool,     BOOL_PTR)
INOUT_TYPEMAP(char,               char,                 byte,     CHAR_PTR)
INOUT_TYPEMAP(signed char,        signed char,          char,    CHAR_PTR)
INOUT_TYPEMAP(unsigned char,      unsigned char,        byte,     UINT8_PTR)
INOUT_TYPEMAP(short,              short,                short,    INT16_PTR)
INOUT_TYPEMAP(unsigned short,     unsigned short,       ushort,   UINT16_PTR)
INOUT_TYPEMAP(int,                int,                  int,      INT32_PTR)
INOUT_TYPEMAP(unsigned int,       unsigned int,         uint,     UINT32_PTR)
INOUT_TYPEMAP(long,               long,                 int,      INT32_PTR)
INOUT_TYPEMAP(unsigned long,      unsigned long,        uint,     UINT32_PTR)
INOUT_TYPEMAP(long long,          long long,            long,     INT64_PTR)
INOUT_TYPEMAP(unsigned long long, unsigned long long,   ulong,    UINT64_PTR)
INOUT_TYPEMAP(float,              float,                float,    FLOAT_PTR)
INOUT_TYPEMAP(double,             double,               double,   DOUBLE_PTR)
INOUT_TYPEMAP(__int64,            long,                 HLONG,    INT64_PTR)
INOUT_TYPEMAP(HLONG,              long,                 HLONG,    INT64_PTR)

/* handles longs */
%typemap(ctype) HLONG "long"
%typemap(imtype) HLONG "HLONG"
%typemap(cstype) HLONG "HLONG"
%typemap(csin) HLONG "$csinput"
%typemap(csout) HLONG {return $imcall;}
%typemap(in) HLONG %{ $1 = $input; %}
%typemap(out) HLONG %{ $result = $1; %}
%typemap(csvarin) HLONG
 %{
   set {
	 $imcall;
   } %}
%typemap(csvarout) HLONG
 %{
   get {
	 return $imcall;
   } %}

/* Handles enumeration */
%typemap(csbase) TKO_Font_Options "uint";
%typemap(csbase) TKO_Rendering_Option_Bits "uint";
%typemap(csbase) TKO_Geometry_Bits "uint";
%typemap(csbase) TKO_Texture_Option_Bits "uint";
%typemap(csbase) TKO_Image_Formats "uint";

/* Handles strings */
%typemap(imtype) const char * "string"
%typemap(cstype) const char * "string"
%typemap(csin) const char * "$csinput"

//manually wrap the enum that SWIG can't handle
%typemap(cscode) TKO_Heuristic_Bits
%{
	TKO_Heuristic_Hidden_Surfaces       	= 0x00000001,
	TKO_Heuristic_Backplane_Cull        	= 0x00000002,
	TKO_Heuristic_Polygon_Handedness    	= 0x00000004,
	TKO_Heuristic_Quick_Moves           	= 0x00000008,
	TKO_Heuristic_Partial_Erase         	= 0x00000010,
	TKO_Heuristic_Memory_Purge          	= 0x00000020,
	TKO_Heuristic_Related_Select_Limit  	= 0x00000040,
	TKO_Heuristic_Internal_Shell_Limit 		= 0x00000080,
	TKO_Heuristic_Clipping              	= 0x00000100,
	TKO_Heuristic_Transformations       	= 0x00000200,
	TKO_Heuristic_Intersecting_Polygons 	= 0x00000400,
	TKO_Heuristic_Polygon_Crossings     	= 0x00000800,
	TKO_Heuristic_Concave_Polygons      	= 0x00001000,
	TKO_Heuristic_Incremental_Updates   	= 0x00002000,
	TKO_Heuristic_Selection_Sorting     	= 0x00004000,

	TKO_Heuristic_Extended              	= 0x00008000,
	//TKO_Heuristic_Extended_Mask            	= 0xFFFF0000,
	TKO_Heuristic_Extended_Mask            	= -65536,
	TKO_Heuristic_Extended_Shift			= 16,

	TKO_Heuristic_Culling               	= 0x00010000,
	TKO_Heuristic_Exclude_Bounding      	= 0x00020000,
	TKO_Heuristic_Detail_Selection      	= 0x00040000,
	TKO_Heuristic_Ordered_Drawing			= 0x00080000,
	TKO_Heuristic_Ordered_Unit				= 0x00100000,
	TKO_Heuristic_Ordered_Weights			= 0x00200000,
	TKO_Heuristic_Internal_Polyline_Limit	= 0x00400000,
	TKO_Heuristic_Ordered_Grid				= 0x00800000,

	TKO_Heuristic_Selection_Level		 	= 0x01000000,
	TKO_Heuristic_Static				 	= 0x02000000,

	TKO_Heuristic_Internal_Select_Limit = TKO_Heuristic_Internal_Shell_Limit | TKO_Heuristic_Internal_Polyline_Limit,
	TKO_Heuristic_Extras                = TKO_Heuristic_Polygon_Handedness | TKO_Heuristic_Quick_Moves,

	TKO_Heur_Extra_Left_Handed_Polys    = 0x01,
	TKO_Heur_Extra_Quick_Move_Spriting  = 0x02,
	TKO_Heur_View_Frustum_Culling		= 0x0001,
	TKO_Heur_Obscuration_Culling		= 0x0002,
	TKO_Heur_Extent_Culling				= 0x0004,
	TKO_Heur_View_Frustum_Culling_Off	= 0x0010,
	TKO_Heur_Obscuration_Culling_Off	= 0x0020,
	TKO_Heur_Extent_Culling_Off			= 0x0040,
	TKO_Heur_Culling_Extended			= 0x0080,
	TKO_Heur_Culling_Extended_Mask		= 0xFF00,
	TKO_Heur_Culling_Extended_Shift		= 8,
	TKO_Heur_Obscuration_Use_Octree		= 0x0100,
	TKO_Heur_Maximum_Extent_Mode		= 0x0200,
	TKO_Heur_Vector_Culling				= 0x0400,
	TKO_Heur_Vector_Tolerance			= 0x0800,
	TKO_Heur_Vector_Culling_Off			= 0x1000,
	TKO_Heur_Vector_Tolerance_Off		= 0x2000,

	TKO_Heur_Max_Extent_Mode_None		= 0,
	TKO_Heur_Max_Extent_Mode_Dot		= 1,
	TKO_Heur_Max_Extent_Mode_Bounding	= 2,
	TKO_Heur_Max_Extent_Mode_Defer		= 3,

	TKO_Heur_Order_World_Volume		= 0,
	TKO_Heur_Order_Screen_Extent	= 1,
	TKO_Heur_Order_Distance			= 2,
	TKO_Heur_Order_Divergence		= 3,
	TKO_Heur_Order_Density			= 4,
	TKO_Heur_Order_Priority			= 5,
	TKO_Heur_Order_Count			= 6,

	TKO_Heur_Selection_Level_Entity       = 0,
	TKO_Heur_Selection_Level_Segment      = 1,
	TKO_Heur_Selection_Level_Segment_Tree = 2,
%}
enum TKO_Heuristic_Bits {};
 
%typemap(cscode) TKE_Object_Types 
%{
	TKE_Termination					= '\x00',		//!< ////
	TKE_Pause						= '\x01',		//!< ////
													/////////
	TKE_Comment						= ';',			//!< ////
													/////////
	TKE_Font						= 'f',			//!< ////
	TKE_Texture						= 't',			//!< ////
	TKE_Material					= '\x02',		//!< ////
													/////////
	TKE_Open_Segment				= '(',			//!< ////
	TKE_Close_Segment				= ')',			//!< ////
	TKE_Reopen_Segment				= 's',			//!< ////
	TKE_Include_Segment				= '<',			//!< ////
	TKE_Style_Segment				= '{',			//!< ////
	TKE_Named_Style					= 'y',			//!< ////
													/////////
	TKE_Geometry_Attributes			= ':',			//!< ////
	TKE_Renumber_Key_Global			= 'K',			//!< ////
	TKE_Renumber_Key_Local			= 'k',			//!< ////
	TKE_Priority					= '0',			//!< ////
	TKE_Tag							= 'q',			//!< ////
													/////////
	TKE_Bounding					= 'b',			//!< ////
	TKE_Bounding_Info				= 'B',			//!< ////
	TKE_Callback					= '\x07',		//!< ////
	TKE_Camera						= '>',			//!< ////
	TKE_Conditional_Action			= '\'',			//!< ////
	TKE_Conditions					= '?',			//!< ////
	TKE_Color						= '"',			//!< ////
	TKE_Color_By_Index				= '\x08',		//!< ////
	TKE_Color_By_Index_16			= '\x09',		//!< ////
	TKE_Color_By_FIndex				= '\x0A',		//!< ////
	TKE_Color_RGB					= '~',			//!< ////
	TKE_Color_By_Value				= '\x0B',		//!< ////
	TKE_Color_Map					= '\x0C',		//!< ////
	TKE_Edge_Pattern				= '\x0D',		//!< ////
	TKE_Edge_Weight					= '\x0E',		//!< ////
	TKE_Face_Pattern				= 'P',			//!< ////
	TKE_Geometry_Options			= '\x17',		//!< ////
	TKE_Handedness					= 'h',			//!< ////
	TKE_Heuristics					= 'H',			//!< ////
	TKE_Line_Pattern				= '-',			//!< ////
	TKE_Line_Weight					= '=',			//!< ////
	TKE_Marker_Size					= '+',			//!< ////
	TKE_Marker_Symbol				= '@',			//!< ////
	TKE_Modelling_Matrix			= '%',			//!< ////
	TKE_LOD							= '\x19',		//!< ////
	TKE_Rendering_Options			= 'R',			//!< ////
	TKE_Selectability				= '!',			//!< ////
	TKE_Text_Alignment				= '*',			//!< ////
	TKE_Text_Font					= 'F',			//!< ////
	TKE_Text_Path					= '|',			//!< ////
	TKE_Text_Spacing				= ' ',			//!< ////
	TKE_Texture_Matrix				= '$',			//!< ////
	TKE_Unicode_Options				= '\x16',		//!< ////
	TKE_User_Index					= 'n',			//!< ////
	TKE_User_Index_Data				= 'm',			//!< ////
	TKE_User_Options				= 'U',			//!< ////
	TKE_User_Value					= 'v',			//!< ////
	TKE_Visibility					= 'V',			//!< ////
	TKE_Window						= 'W',			//!< ////
	TKE_Window_Frame				= '#',			//!< ////
	TKE_Window_Pattern				= 'p',			//!< ////
	TKE_Glyph_Definition			= 'j',			//!< ////
	TKE_Line_Style					= 'J',			//!< ////
	TKE_Named_Style_Def 			= 'u',			//!< ////
													/////////
	TKE_Area_Light					= 'a',			//!< ////
	TKE_Circle						= 'C',			//!< ////
	TKE_Circular_Arc				= 'c',			//!< ////
	TKE_Circular_Chord				= '\\',			//!< ////
	TKE_Circular_Wedge				= 'w',			//!< ////
	TKE_Cutting_Plane				= '/',			//!< ////
	TKE_Cylinder					= 'Y',			//!< ////
	TKE_Distant_Light				= 'd',			//!< ////
	TKE_Ellipse						= 'E',			//!< ////
	TKE_Elliptical_Arc				= 'e',			//!< ////
	TKE_Grid						= 'g',			//!< ////
	TKE_Image						= 'i',			//!< ////
	TKE_Infinite_Line				= '`',			//!< ////
	TKE_Infinite_Ray				= '\x11',		//!< ////
	TKE_Line						= 'l',			//!< ////
	TKE_Local_Light					= '.',			//!< ////
	TKE_Marker						= 'X',			//!< ////
	TKE_Mesh						= 'M',			//!< ////
	TKE_NURBS_Curve					= 'N',			//!< ////
	TKE_NURBS_Surface				= 'A',			//!< ////
	TKE_PolyCylinder				= 'Q',			//!< ////
	TKE_Polygon						= 'G',			//!< ////
	TKE_Polyline					= 'L',			//!< ////
	TKE_PolyPolyline				= '\x10',		//!< ////
	TKE_Reference					= 'r',			//!< ////
	TKE_Shell						= 'S',			//!< ////
	TKE_Sphere						= '\x1a',		//!< ////
	TKE_Spot_Light					= '^',			//!< ////
	TKE_Text						= 'T',			//!< ////
	TKE_Text_With_Encoding			= 'x',			//!< ////
													/////////
	TKE_Start_User_Data				= '[',			//!< ////
	TKE_Stop_User_Data				= ']',			//!< ////
	TKE_XML							= '\x18',		//!< ////
	TKE_External_Reference			= '\x12',		//!< ////
	TKE_External_Reference_Unicode	= '\x13',		//!< ////
	TKE_URL							= '\x15',		//!< ////
													/////////
	TKE_Start_Compression			= 'Z',			//!< ////
	TKE_Stop_Compression			= 'z',			//!< ////
													/////////
	TKE_Repeat_Object				= '&',			//!< ////
	TKE_View						= '}',			//!< ////
	TKE_Clip_Rectangle				= 'o',			//!< ////
	TKE_Clip_Region					= 'O',			//!< ////
	TKE_Complex_Clip_Region			= '\x0F',		//!< //
													/////////
	TKE_File_Info					= 'I',			//!< ////
	TKE_Dictionary					= 'D',			//!< ////
	TKE_Dictionary_Locater			= '_',			//!< ////
	TKE_Thumbnail					= '\x14',		//!< ////
	TKE_Delete_Object				= '\x7F',		//!< ////
													/////////
	TKE_Tag_Implicit				= TKE_Tag,		//!< //-- obsolete.  this alias provided for source compatibility with old code
	TKE_Streaming_Mode				= ',',			//!< //-- would like this to be obsolete, but...

	TKE_First_User_Opcode			= '\xA0',		//!< //// first opcode value reserved for private use
	TKE_Last_User_Opcode			= '\xEF',		//!< //// last opcode value reserved for private use
	TKE_HW3D_Image					= 0xE0,
													/////////
	TKE_Pseudo_Handler				= '\xFE',		//!< //// pseudo-handler (non-zero value)
	TKE_Extended_Code				= '\xFF'		//!< //// reserved for future expansion
%}
enum TKE_Object_Types {};



//ignore some stuff (as their symbols aren't exported...)
%ignore Internal_ExRef_List;
%ignore quantize_and_pack_floats;
%ignore unquantize_and_unpack_floats;
%ignore trivial_compress_points;
%ignore trivial_decompress_points;
%ignore normal_vectors10;
%ignore closest_points10;
%ignore normal_vectors8;
%ignore closest_points8;
%ignore bounding_faces;
%ignore bounding_tristrips;
%ignore color_cube;
%ignore rgba_cube;
%ignore normal_cube;
%ignore polar_bounds;
%ignore show_edgebreaker_compress_size;
%ignore edgebreaker_compress;
%ignore show_edgebreaker_decompress_size;
%ignore edgebreaker_decompress;
%ignore mtable_info_init;
%ignore mtable_info_free;
%ignore lookup_vertex;
%ignore predict;
%ignore old_predict;
%ignore old_pack_points;
%ignore old_unpack_points;
%ignore old_unpack_normals;
%ignore int_stack_init;
%ignore int_stack_expand;
%ignore int_stack_free;
%ignore int_stack_pop;
%ignore int_stack_pop_internal;
%ignore int_stack_push;
%ignore int_stack_push_if_unique;
%ignore validate_associations;
%ignore half_edge_array_init;
%ignore half_edge_array_free;
%ignore half_edge_array_expand;
%ignore half_edge_array_append;
%ignore validate_edge;
%ignore default_malloc;
%ignore default_free;
%ignore default_new_vertex;
%ignore vsinit_write;
%ignore vsinit_read;
%ignore vsfree;
%ignore vsswap;
%ignore vsput;
%ignore vsget;
%ignore default_malloc;
%ignore actions;
%ignore g_hints;
%ignore BPointCloud;
%ignore TK_File_Format_History;
%ignore Internal_Texture_List;
%ignore Internal_Segment_Processor;
%ignore TKE_Object_Types;	
%ignore TK_Image_Bytes_Per_Pixel;	

//ignore these as they are difficult/impossible to wrap
%ignore TK_Dictionary::m_item;			// of type Internal_Translator::Index_Key_Pair, which is private
%ignore TK_Color::set_channel_rgb;		// uses "channel" class, which is defined under the protected scope
%ignore TK_Color::set_channel_name;		// uses "channel" class, which is defined under the protected scope
%ignore TK_Color::m_diffuse;			// uses "channel" class, which is defined under the protected scope
%ignore TK_Color::m_specular;			// uses "channel" class, which is defined under the protected scope
%ignore TK_Color::m_mirror;				// uses "channel" class, which is defined under the protected scope
%ignore TK_Color::m_transmission;		// uses "channel" class, which is defined under the protected scope
%ignore TK_Color::m_emission;			// uses "channel" class, which is defined under the protected scope
%ignore TK_Color::m_environment;		// uses "channel" class, which is defined under the protected scope
%ignore TK_Color::m_bump;				// uses "channel" class, which is defined under the protected scope
%ignore TK_Polyhedron::set_facemap;		// body of function not implemented?
%ignore TKO_Heuristic_Extended_Mask;	// C# complains about converting 0xFFFF0000 to int
                                
%ignore IT_Index_Key_Extra::m_bounds;
%ignore TK_Character_Attribute::color;
%ignore TK_Character_Attribute::name;
%ignore TK_Polyhedron::mangle_points;
%ignore TK_Polyhedron::mangle_point_attributes;
%ignore TK_Polyhedron::mangle_face_attributes;
%ignore Internal_Revisit_Item;
%ignore Internal_Segment_List;
%ignore Recorded_Instance::m_key;
%ignore Recorded_Instance::m_values;
%ignore Recorded_Instance::m_local_basis;
%ignore Recorded_Instance::m_basis_indices;
%ignore Recorded_Instance::m_arbitrary_point;
%ignore Internal_Data_Accumulator;

// Make array member variables read-only since memory management can be ambiguous
%feature("immutable") ::visitations;
%feature("immutable") ::mlengths;
%feature("immutable") ::m2stackoffsets;
%feature("immutable") ::m2gateoffsets;
%feature("immutable") ::dummies;
%feature("immutable") ::patches;
%feature("immutable") ::data;
%feature("immutable") ::rdata;
%feature("immutable") ::loops;
%feature("immutable") ::loops_edges;
%feature("immutable") ::P;
%feature("immutable") ::N;
%feature("immutable") TK_Character_Attribute::name;



%rename(write_collection_ascii) write_collection_ascii(BStreamFileToolkit & tk);
%rename(write_collection_ascii_2arg) write_collection_ascii(BStreamFileToolkit & tk, bool write_tag);
%rename(write_collection_ascii_3arg) write_collection_ascii(BStreamFileToolkit & tk, bool write_tag, int variant);

%rename(write_collection) write_collection(BStreamFileToolkit & tk);
%rename(write_collection_2arg) write_collection(BStreamFileToolkit & tk, bool write_tag);
%rename(write_collection_3arg) write_collection(BStreamFileToolkit & tk, bool write_tag, int variant);

%rename(Interpret_special) Interpret(BStreamFileToolkit & tk, long key, char const * special);

%include "directors.i"

#define alter
#define null

%include "../stream_common/BStream.h"
%include "../stream_common/BStreamFileToolkit.h"
%include "../stream_common/BByteStream.h"
%include "../stream_common/BCompress.h"
%include "../stream_common/BEdgeBreaker.h"
%include "../stream_common/BInternal.h"
%include "../stream_common/BOpcodeHandler.h"
%include "../stream_common/BPolyhedron.h"
%include "../stream_common/BOpcodeShell.h"
%include "../stream_common/BPointCloud.h"
%include "../stream_common/BPolyPoly.h"

%include "../swiginclude.h"

