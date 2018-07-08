//swig -v -csharp -c++ -dllimport hcsstream$hversion hcsstream.i";
%module(directors="1", allprotected="0") HCSSTREAM

//disable some warnings
#pragma SWIG nowarn=516,302

// ================================================================================
%typemap(csimports) SWIGTYPE %{
using System;
using System.Runtime.InteropServices;

#if _M_X64
using HLONG = System.Int64;
using ID_Key = System.Int64;
#else
using HLONG = System.Int32;
using ID_Key = System.Int32;
#endif

%}

%pragma(csharp) imclassclassmodifiers = 
%{
#if _M_X64
using HLONG = System.Int64;
using ID_Key = System.Int64;
#else
using HLONG = System.Int32;
using ID_Key = System.Int32;
#endif

public class %}

%pragma(csharp) moduleclassmodifiers = 
%{
#if _M_X64
using HLONG = System.Int64;
using ID_Key = System.Int64;
#else
using HLONG = System.Int32;
using ID_Key = System.Int32;
#endif
public class %}

// Proxy classes (base classes, ie, not derived classes)
%typemap(csbody) SWIGTYPE %{
  private HandleRef swigCPtr;
  protected bool swigCMemOwn;

  public $csclassname(IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new HandleRef(this, cPtr);
  }

  public static HandleRef getCPtr($csclassname obj) {
    return (obj == null) ? new HandleRef(null, IntPtr.Zero) : obj.swigCPtr;
  }
%}

// Derived proxy classes
%typemap(csbody_derived) SWIGTYPE %{
  private HandleRef swigCPtr;

  public $csclassname(IntPtr cPtr, bool cMemoryOwn) : base($imclassname.$csclassnameUpcast(cPtr), cMemoryOwn) {
    swigCPtr = new HandleRef(this, cPtr);
  }

  public static HandleRef getCPtr($csclassname obj) {
    return (obj == null) ? new HandleRef(null, IntPtr.Zero) : obj.swigCPtr;
  }
%}

// Typewrapper classes
%typemap(csbody) SWIGTYPE *, SWIGTYPE &, SWIGTYPE [], SWIGTYPE (CLASS::*) %{
  private HandleRef swigCPtr;

  public $csclassname(IntPtr cPtr, bool futureUse) {
    swigCPtr = new HandleRef(this, cPtr);
  }

  protected $csclassname() {
    swigCPtr = new HandleRef(null, IntPtr.Zero);
  }

  public static HandleRef getCPtr($csclassname obj) {
    return (obj == null) ? new HandleRef(null, IntPtr.Zero) : obj.swigCPtr;
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

	[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
	[return: MarshalAs(UnmanagedType.U1)]
	public delegate bool TK_Progress_Callback(uint so_far, uint expected, IntPtr user_data);
%}

// Ensure swigStringHelper is initialized since its initialization may get optimized out
%pragma(csharp) imclasscode = %{
  static $imclassname() {
   if (swigStringHelper == null)
    swigStringHelper = new SWIGStringHelper();
  }
%}

%{
#define alter

#include <math.h>

#ifdef WIN64
#	define HLONG __int64
#	define  long __int64
#else
#	define HLONG long
#endif

#include "../../base_stream/source/stream_common\BStream.h"
#include "../../base_stream/source/stream_common\BStreamFileToolkit.h"
#include "../../base_stream/source/stream_common\BByteStream.h"
#include "../../base_stream/source/stream_common\BCompress.h"
#include "../../base_stream/source/stream_common\BEdgeBreaker.h"
#include "../../base_stream/source/stream_common\BInternal.h"
#include "../../base_stream/source/stream_common\BOpcodeHandler.h"
#include "../../base_stream/source/stream_common\BPolyhedron.h"
#include "../../base_stream/source/stream_common\BOpcodeShell.h"
#include "../../base_stream/source/stream_common\BPointCloud.h"
#include "../../base_stream/source/stream_common\BPolyPoly.h"

#include "HStream.h"
#include "HByteStream.h"
#include "HOpcodeHandler.h"
#include "HOpcodeShell.h"
#include "HPolyhedron.h"
#include "HPolyPoly.h"
#include "HStreamFileToolkit.h"
%}

%csmethodmodifiers TK_Mesh::Read "public override"
%csmethodmodifiers TK_Mesh::Write "public override"
%csmethodmodifiers TK_Mesh::Reset "public override"
%csmethodmodifiers TK_Shell::Read "public override"
%csmethodmodifiers TK_Shell::Write "public override"
%csmethodmodifiers TK_Shell::Reset "public override"
%csmethodmodifiers TK_Polyhedron::Read "public override"
%csmethodmodifiers TK_Polyhedron::Write "public override"
%csmethodmodifiers TK_Polyhedron::Reset "public override"

%csmethodmodifiers TK_PolyCylinder::Clone "public override"
%csmethodmodifiers TK_PolyCylinder::SetPoints "public new"
%csmethodmodifiers TK_PolyCylinder::GetPoints "public new"

%csmethodmodifiers HTK_PolyCylinder::Execute "public override"
%csmethodmodifiers HTK_PolyCylinder::Interpret "public override"


%rename(GetTKType) GetType;

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
%typemap(csbase) TKO_Heuristic_Bits "uint";
%typemap(csbase) TKO_Image_Formats "uint";

/* Handles strings */
%typemap(imtype, 
		inattributes="[MarshalAs(UnmanagedType.LPWStr)]", 
		outattributes="[return: MarshalAs(UnmanagedType.LPWStr)]") const wchar_t * "string"
		
%typemap(cstype) const wchar_t * "string"
%typemap(csin) const wchar_t * "$csinput"
%typemap(csout) const wchar_t * {return $imcall;}

%typemap(imtype) const char * "string"
%typemap(cstype) const char * "string"
%typemap(csin) const char * "$csinput"
%typemap(csout) const char * {return $imcall;}

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
//INOUT_TYPEMAP(char,               char,                 byte,     CHAR_PTR)
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


%typemap(imtype, out="IntPtr") float *, float[ANY], float[] "float[]"
%typemap(cstype, out="IntPtr") float *, float[ANY], float[] "float[]"

%typemap(imtype, out="IntPtr") int *, int[ANY], int[] "int[]"
%typemap(cstype, out="IntPtr") int *, int[ANY], int[] "int[]"

%typemap(imtype, out="IntPtr") unsigned char *, unsigned char[ANY], unsigned char[] "byte[]"
%typemap(cstype, out="IntPtr") unsigned char *, unsigned char[ANY], unsigned char[] "byte[]"

%typemap(imtype) char * "byte[]"
%typemap(cstype) char * "byte[]"

%typemap(imtype) void* "IntPtr"
%typemap(cstype) void* "IntPtr"
%typemap(csout) void* {return $imcall;}
%typemap(csin) void* "$csinput"

#undef INOUT_TYPEMAP

//manually wrap the enum that SWIG can't handle
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

/* Stream progress callback delegate */
%typemap(imtype) bool (*cb)(unsigned long, unsigned long, void*) "HCSSTREAM.TK_Progress_Callback"
%typemap(cstype) bool (*cb)(unsigned long, unsigned long, void*) "HCSSTREAM.TK_Progress_Callback"
%typemap(csin)   bool (*cb)(unsigned long, unsigned long, void*) "$csinput"

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
%feature("immutable") ::data;		// varstream
%feature("immutable") ::rdata;		// varstream


%rename(write_collection_ascii) write_collection_ascii(BStreamFileToolkit & tk);
%rename(write_collection_ascii_2arg) write_collection_ascii(BStreamFileToolkit & tk, bool write_tag);
%rename(write_collection_ascii_3arg) write_collection_ascii(BStreamFileToolkit & tk, bool write_tag, int variant);

%rename(write_collection) write_collection(BStreamFileToolkit & tk);
%rename(write_collection_2arg) write_collection(BStreamFileToolkit & tk, bool write_tag);
%rename(write_collection_3arg) write_collection(BStreamFileToolkit & tk, bool write_tag, int variant);

%rename(Interpret_special) Interpret(BStreamFileToolkit & tk, long key, char const * special);


#define alter
#define null
%extend BStreamFileToolkit {
	TK_Status ParseBuffer(unsigned char * b, int s, TK_Status mode = TK_Normal) {
		return self->ParseBuffer((const char *) b, s, mode);
	}
}
%ignore ParseBuffer;

/* this macro is currently unused but hopefully will be in the future for
   handling some pointer return values.
   
%define SWIG_PTR_2_ARRAY(type,count)
%typemap(cstype) type*  "type[]";
		%typemap(csout) type* 
		%{{
			float[] temp = new type[count];
			Marshal.Copy($imcall,temp,0,count);
			return temp;
		}%}
		%typemap(imtype) type * "IntPtr";
%enddef
*/

%include "directors.i"

// This has to be included before the following ID_Key typemaps
%include "../../../base_stream/source/stream_common/BStream.h"

%typemap(ctype) ID_Key "ID_Key"
%typemap(imtype) ID_Key "ID_Key"
%typemap(cstype) ID_Key "ID_Key"
%typemap(csin) ID_Key "$csinput"
%typemap(csout) ID_Key {return $imcall;}
%typemap(in) ID_Key %{ $1 = $input; %}
%typemap(out) ID_Key %{ $result = $1; %}
%typemap(csdirectorin) ID_Key "$iminput" 
%typemap(csdirectorout) ID_Key "$cscall"
%typemap(imtype) ID_Key & "ID_Key[]"
%typemap(cstype) ID_Key & "ID_Key[]"

%include "../../../base_stream/source/stream_common/BStreamFileToolkit.h"
%include "../../../base_stream/source/stream_common/BByteStream.h"
%include "../../../base_stream/source/stream_common/BCompress.h"
%include "../../../base_stream/source/stream_common/BEdgeBreaker.h"
%include "../../../base_stream/source/stream_common/BInternal.h"

%include "../../../base_stream/source/stream_common/BOpcodeHandler.h"
%include "../../../base_stream/source/stream_common/BPolyhedron.h"
%include "../../../base_stream/source/stream_common/BOpcodeShell.h"
%include "../../../base_stream/source/stream_common/BPointCloud.h"
%include "../../../base_stream/source/stream_common/BPolyPoly.h"


%include "../HStream.h"
%include "../HByteStream.h"
%include "../HOpcodeHandler.h"
%include "../HOpcodeShell.h"
%include "../HPolyhedron.h"
%include "../HPolyPoly.h"
%include "../HStreamFileToolkit.h"

