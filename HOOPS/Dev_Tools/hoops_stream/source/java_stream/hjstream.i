//swig -v -java -c++ -package com.techsoft.hoops hjstream.i";

%module(directors="1",allprotected="1") HJSTREAM
#pragma SWIG nowarn=516

%include "hj_version.i"

%{
#include <math.h>

#ifdef WIN64
#	define HLONG __int64
#	define  long __int64
#else
#	define HLONG long
#endif

#define alter
#define HC_KEY long

#include "BStream.h"
#include "BStreamFileToolkit.h"
#include "BByteStream.h"
#include "BCompress.h"
#include "BEdgeBreaker.h"
#include "BInternal.h"
#include "BOpcodeHandler.h"
#include "BPolyhedron.h"
#include "BOpcodeShell.h"
#include "BPointCloud.h"
#include "BPolyPoly.h"

#include "HStream.h"
#include "HByteStream.h"
#include "HOpcodeHandler.h"
#include "HOpcodeShell.h"
#include "HPolyhedron.h"
#include "HPolyPoly.h"
#include "HStreamFileToolkit.h"

char *buf;
%}

%javamethodmodifiers TK_Polyhedron::Read "public"
%javamethodmodifiers TK_Polyhedron::Write "public"
%javamethodmodifiers TK_Polyhedron::Reset "public"


%wrapper %{
SWIGEXPORT void JNICALL Java_com_techsoft_hoops_HJSTREAMJNI_ReleaseBuffer(JNIEnv *jenv, jclass jcls, jbyteArray jarg2, jint jarg3) {
  if(buf)
	  jenv->ReleaseByteArrayElements(jarg2, (jbyte *)buf, 0);
}

SWIGEXPORT jint JNICALL Java_com_techsoft_hoops_HJSTREAMJNI_PrepareBuffer(JNIEnv *jenv, jclass jcls, jlong jarg1, jbyteArray jarg2, jint jarg3) {
  jint jresult = 0 ;
  BStreamFileToolkit *arg1 = (BStreamFileToolkit *) 0 ;
  char *arg2 = (char *) 0 ;
  int arg3 ;
  TK_Status result;

  (void)jenv;
  (void)jcls;
  arg1 = *(BStreamFileToolkit **)&jarg1; 
  {
    if (!jarg2) {
      SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "array null");
      return 0;
    }
    if (jenv->GetArrayLength(jarg2) == 0) {
      SWIG_JavaThrowException(jenv, SWIG_JavaIndexOutOfBoundsException, "Array must contain at least 1 element");
      return 0;
    }
    buf = arg2 = (char *) jenv->GetByteArrayElements(jarg2, 0); 
  }
  arg3 = (int)jarg3; 
  result = (TK_Status)(arg1)->PrepareBuffer(buf,arg3);
  jresult = (jint)result;  
  return jresult;
}

%}


%define INOUT_TYPEMAP(TYPE, JNITYPE, JTYPE, JAVATYPE, JNIDESC, TYPECHECKTYPE)
%typemap(jni) TYPE *, TYPE &, TYPE[] %{JNITYPE##Array%}
%typemap(jtype) TYPE *, TYPE &, TYPE[] "JTYPE[]"
%typemap(jstype) TYPE *, TYPE &, TYPE[] "JTYPE[]"
%typemap(javain) TYPE *, TYPE &, TYPE[] "$javainput"
%typemap(javaout) TYPE *, TYPE &, TYPE[] {return $jnicall;}
%typemap(javadirectorin) TYPE *, TYPE &, TYPE[] "$jniinput"
%typemap(javadirectorout) TYPE *, TYPE &, TYPE[] "$javacall"


%typemap(in,noblock=1) TYPE *, TYPE &, TYPE[] {
    $1 = $input ? ($1_ltype) JCALL2(Get##JAVATYPE##ArrayElements, jenv, $input, 0) : 0;
}
%typemap(freearg,noblock=1) TYPE *, TYPE &, TYPE[] {
    if ($input) JCALL3(Release##JAVATYPE##ArrayElements, jenv, $input, (JNITYPE *)$1, 0);
}

%typemap(directorout,warning="Need to provide TYPE * directorout typemap") TYPE *, TYPE & {
}

%typemap(directorin,descriptor=JNIDESC) TYPE &
%{ *(($&1_ltype)&$input) = &$1; %}

%typemap(directorin,descriptor=JNIDESC,warning="Need to provide TYPE * directorin typemap, TYPE array length is unknown") TYPE *, TYPE &
{
}

%typemap(typecheck) TYPE * = TYPECHECKTYPE;
%typemap(typecheck) TYPE & = TYPECHECKTYPE;
%enddef


//////%define INOUT_TYPEMAP(TYPE, JNITYPE, JTYPE, JAVATYPE, JNIDESC, TYPECHECKTYPE)
INOUT_TYPEMAP(bool, jboolean, boolean, Boolean, "[Ljava/lang/Boolean;", jbooleanArray); 

INOUT_TYPEMAP(char, jbyte, byte, Byte, "[Ljava/lang/Byte;", jbyteArray); 

INOUT_TYPEMAP(signed char, jbyte, byte, Byte, "[Ljava/lang/Byte;", jbyteArray); 
INOUT_TYPEMAP(unsigned char, jbyte, byte, Byte, "[Ljava/lang/Byte;", jbyteArray); 
//INOUT_TYPEMAP(unsigned char, jshort, short, Short, "[Ljava/lang/Short;", jshortArray);     
INOUT_TYPEMAP(short, jshort, short, Short, "[Ljava/lang/Short;", jshortArray);
INOUT_TYPEMAP(unsigned short, jint, int, Int, "[Ljava/lang/Integer;", jintArray); 
INOUT_TYPEMAP(int, jint, int, Int, "[Ljava/lang/Integer;", jintArray);
INOUT_TYPEMAP(unsigned int, jlong, long, Long, "[Ljava/lang/Long;", jlongArray); 
INOUT_TYPEMAP(long, jint, int, Int, "[Ljava/lang/Integer;", jintArray);
INOUT_TYPEMAP(unsigned long, jlong, long, Long, "[Ljava/lang/Long;", jlongArray); 
INOUT_TYPEMAP(long long, jlong, long, Long, "[Ljava/lang/Long;", jlongArray);
//INOUT_TYPEMAP(unsigned long long, jobject, java.math.BigInteger, NOTUSED, "[Ljava.math.BigInteger;", SWIGBIGINTEGERARRAY);
INOUT_TYPEMAP(float, jfloat, float, Float, "[Ljava/lang/Float;", jfloatArray);
INOUT_TYPEMAP(double, jdouble, double, Double, "[Ljava/lang/Double;", jdoubleArray); 

INOUT_TYPEMAP(__int64, jlong, long, Long, "[Ljava/lang/Long;", jlongArray);
INOUT_TYPEMAP(HLONG, jint, int, Int, "[Ljava/lang/Long;", jintArray);
//INOUT_TYPEMAP(HC_KEY, jint, int, Int, "[Ljava/lang/Integer;", jintArray);
INOUT_TYPEMAP(HC_KEY, jint, int, Int, "[I", jintArray);

#undef INOUT_TYPEMAP



/* Handles strings */

// wchar_t typemaps adapted from std_wstring.i
%typemap(jni) const wchar_t * %{jstring%}
%typemap(jtype) const wchar_t * "String"
%typemap(jstype) const wchar_t * "String"
%typemap(javain) const wchar_t * "$javainput"
%typemap(javaout) const wchar_t * {return $jnicall;}
%typemap(in) wchar_t const *, wchar_t const & %{ 
  if ($input) {
      if (sizeof(wchar_t) != sizeof(jchar)) {
          const jchar *$1_pstr = jenv->GetStringChars($input, 0);
          if (!$1_pstr) return $null;
          jsize $1_len = jenv->GetStringLength($input);
          $1 = new wchar_t[$1_len];
          for (jsize i=0; i < $1_len; i++) $1[i] = $1_pstr[i];
          $1[$1_len] = 0;
          jenv->ReleaseStringChars($input, $1_pstr);
      } else {
          $1 = (wchar_t *)jenv->GetStringChars($input, 0);
      }
  }
%}
%typemap(freearg) wchar_t const *, wchar_t const & %{
    if (sizeof(wchar_t) != sizeof(jchar))
        delete [] $1;
    else if ($1)
        jenv->ReleaseStringChars($input, (const jchar *)$1);
%}

%typemap(jni) char const * %{jstring%}
%typemap(jtype) char const * "String"
%typemap(jstype) char const * "String"
%typemap(javain) char const * "$javainput"
%typemap(javadirectorin) char const * "$jniinput"
%typemap(javadirectorout) char const * "$javacall"
%typemap(freearg) const char * %{
  if ($1) (jenv)->ReleaseStringUTFChars($input, (const char *)$1);
%}
%typemap(in) char const *, char const & { 
  if ($input) {
    $1 = (char *)jenv->GetStringUTFChars($input, 0);
  }
}
%typemap(argout) char const * ""


// Takes care of director stuff   
%typemap(directorin,descriptor="I") HC_KEY "*((HC_KEY **)&$input) = &$1;"
%typemap(directorin,descriptor="[I") int * "*((int **)&$input) = $1;"
%typemap(directorin,descriptor="[I") HC_KEY * "*((HC_KEY **)&$input) = &$1;"
%typemap(directorin,descriptor="[I") const unsigned short * "*((const unsigned short **)&$input) = $1;"
%typemap(directorin,descriptor="[F") float & "*((float *)&$input) = $1;"
%typemap(directorin,descriptor="[D") double & "*((double *)&$input) = $1;"
%typemap(directorin,descriptor="Ljava/lang/String;") char const * "$input = jenv->NewStringUTF((const char *)$1);"


/* Handle HLONG */
%typemap(jni) HLONG "jint"
%typemap(jtype) HLONG "int"
%typemap(jstype) HLONG "int"
%typemap(javain) HLONG "$javainput"
%typemap(javaout) HLONG {return $jnicall;}
%typemap(javadirectorin) HLONG "$jniinput"
%typemap(javadirectorout) HLONG "$javacall"
%typemap(in) HLONG "$1 = $input;"
%typemap(out) HLONG "$result = $1;"

/* Handle HC_KEY */
%typemap(jni) HC_KEY "jint"
%typemap(jtype) HC_KEY "int"
%typemap(jstype) HC_KEY "int"
%typemap(javain) HC_KEY "$javainput"
%typemap(javaout) HC_KEY {return $jnicall;}
%typemap(javadirectorin) HC_KEY "$jniinput"
%typemap(javadirectorout) HC_KEY "$javacall"
%typemap(in) HC_KEY "$1 = $input;"
%typemap(out) HC_KEY "$result = $1;"


//ignore some stuff (as their symbols aren't exported...)
%ignore read_float;
%ignore write_float;

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
%ignore TK_PolyCylinder::SetPoints;		// IGNORED since parent TK_Polyhedron class has different return value,
%ignore TK_PolyCylinder::SetDPoints;	//  and swig doesn't know it's not virtual? (should mark final)
%ignore TK_Character_Attribute;
%ignore TK_Image_Bytes_Per_Pixel;
%ignore BBaseOpcodeHandler::Revisit;

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

%ignore PutData (BStreamFileToolkit & tk, char const & c);
%ignore PutData (BStreamFileToolkit & tk, short const & s);
%ignore PutData (BStreamFileToolkit & tk, int const & i);
%ignore PutData (BStreamFileToolkit & tk, unsigned char const & b);
%ignore PutData (BStreamFileToolkit & tk, unsigned short const & s);
%ignore PutData (BStreamFileToolkit & tk, unsigned int const & i);
%ignore PutData (BStreamFileToolkit & tk, float const & f);
%ignore PutData (BStreamFileToolkit & tk, double const & d);
%ignore PutAsciiData (BStreamFileToolkit & tk, char const *tag, char const & c);
%ignore PutAsciiData (BStreamFileToolkit & tk, char const *tag, short const & s);
%ignore PutAsciiData (BStreamFileToolkit & tk, char const *tag, int const & i);
%ignore PutAsciiFlag (BStreamFileToolkit & tk, char const *tag, int const & i);
%ignore PutAsciiMask (BStreamFileToolkit & tk,char const *tag, int const & i);
%ignore PutAsciiData (BStreamFileToolkit & tk, char const *tag, unsigned char const & b);
%ignore PutAsciiData (BStreamFileToolkit & tk, char const *tag, unsigned short const & s);
%ignore PutAsciiData (BStreamFileToolkit & tk, char const *tag, unsigned int const & i);
%ignore PutAsciiData (BStreamFileToolkit & tk, char const *tag, float const & f);
%ignore PutAsciiData (BStreamFileToolkit & tk, char const *tag, double const & d);
%ignore PutAsciiHex (BStreamFileToolkit & tk, char const *tag, int const & i);

		


//ignore protected char * members for now; more research required
%ignore m_ascii_buffer;
%ignore m_bytes;
%ignore m_comment;
%ignore m_condition;
%ignore m_data;
%ignore m_debug_string;
%ignore m_definition;
%ignore m_gooch_color_map_segment;
%ignore m_image;
%ignore m_lookup;
%ignore m_name;
%ignore m_names;
%ignore m_opcode_buffer;
%ignore m_reference;
%ignore m_string;
%ignore m_transform;
%ignore mp_vmvisibilities;
%ignore mp_fvisibilities;
%ignore mp_fpatterns;
%ignore mp_evisibilities;

// These functions all return pointers
%ignore TK_Area_Light::GetPoints;  
%ignore TK_Camera::GetPosition;
%ignore TK_Camera::GetTarget;
%ignore TK_Camera::GetUpVector;
%ignore TK_Camera::GetField;
%ignore TK_Camera::GetOblique;
%ignore TK_Circle::GetCenter;
%ignore TK_Circle::GetEnd;
%ignore TK_Circle::GetMiddle;
%ignore TK_Circle::GetStart;
%ignore TK_Clip_Rectangle::GetRectangle;
%ignore TK_Clip_Region::GetPoints;
%ignore TK_Color::GetDiffuse;
%ignore TK_Color::GetEmission;
%ignore TK_Color::GetMirror;
%ignore TK_Color::GetSpecular;
%ignore TK_Color::GetTransmission;
%ignore TK_Color_By_Value::GetValue;
%ignore TK_Color_Map::GetValues;
%ignore TK_Color_RGB::GetRGB;
%ignore TK_Cutting_Plane::GetPlane;
%ignore TK_Cutting_Plane::GetPlanes;
%ignore TK_Cylinder::GetAxis;
%ignore TK_Cylinder::GetEnd;
%ignore TK_Cylinder::GetStart;
%ignore TK_Ellipse::GetCenter;
%ignore TK_Ellipse::GetLimits;
%ignore TK_Ellipse::GetMajor;
%ignore TK_Ellipse::GetMinor;
%ignore TK_Grid::GetCounts;
%ignore TK_Grid::GetOrigin;
%ignore TK_Grid::GetRef1;
%ignore TK_Grid::GetRef2;
%ignore TK_Image::GetBytes;
%ignore TK_Image::GetPosition;
%ignore TK_Image_Data_Buffer::Buffer;
%ignore TK_Line::GetPoints;           
%ignore TK_Matrix::GetMatrix;
%ignore TK_NURBS_Curve::GetKnots;
%ignore TK_NURBS_Curve::GetPoints;
%ignore TK_NURBS_Curve::GetWeights;
%ignore TK_NURBS_Surface::GetPoints;
%ignore TK_NURBS_Surface::GetUKnots; 
%ignore TK_NURBS_Surface::GetVKnots;
%ignore TK_NURBS_Surface::GetWeights;
%ignore TK_Point::GetPoint;
%ignore TK_PolyCylinder::GetEndNormal;
%ignore TK_PolyCylinder::GetPoints;
%ignore TK_PolyCylinder::GetRadii;
%ignore TK_Polyhedron::GetBoundings;
%ignore TK_Polyhedron::GetExists;
%ignore TK_Polyhedron::GetFaceColors;
%ignore TK_Polyhedron::GetFaceExists;
%ignore TK_Polyhedron::GetFaceIndices;
%ignore TK_Polyhedron::GetFaceRegions;
%ignore TK_Polyhedron::GetPoints;
%ignore TK_Polyhedron::GetVertexColors;
%ignore TK_Polyhedron::GetVertexEdgeColors;
%ignore TK_Polyhedron::GetVertexEdgeIndices;
%ignore TK_Polyhedron::GetFaceColors;
%ignore TK_Polyhedron::GetFaceIndices;
%ignore TK_Polyhedron::GetIndices;
%ignore TK_Polyhedron::GetMarkerColors;
%ignore TK_Polyhedron::GetMarkerIndices;
%ignore TK_Polyhedron::GetMarkerRGBAs;
%ignore TK_Polyhedron::GetVertexNormals;
%ignore TK_Polyhedron::GetVertexParameters;
%ignore TK_Polypoint::GetPoints;
%ignore TK_Rendering_Options::GetAmbientUpVector;
%ignore TK_Rendering_Options::GetDepthRange;
%ignore TK_Rendering_Options::GetFogLimits;
%ignore TK_Rendering_Options::GetImageScale;
%ignore TK_Rendering_Options::GetImageTintColor;
%ignore TK_Rendering_Options::GetLodBounding;
%ignore TK_Rendering_Options::GetLodCutoffs;
%ignore TK_Rendering_Options::GetLodRatios;
%ignore TK_Rendering_Options::GetLodThresholds;
%ignore TK_Rendering_Options::GetScreenRange;
%ignore TK_Rendering_Options::GetSimpleReflectionPlane;
%ignore TK_Rendering_Options::GetSimpleShadowColor;
%ignore TK_Rendering_Options::GetSimpleShadowLight;
%ignore TK_Rendering_Options::GetSimpleShadowPlane;
%ignore TK_Shell::GetFaces;
%ignore TK_Sphere::GetAxis;  
%ignore TK_Sphere::GetCenter;
%ignore TK_Sphere::GetOrtho;
%ignore TK_Spot_Light::GetPosition;
%ignore TK_Spot_Light::GetTarget;
%ignore TK_Text::GetPosition;
%ignore TK_Text::GetTextRegionPoints;
%ignore TK_Text_Font::GetPreferences;
%ignore TK_Text_Font::GetRenderers;
%ignore TK_Texture::GetValueScale;
%ignore TK_Texture::m_shader_source;
%ignore TK_Texture::m_camera;
%ignore TK_Thumbnail::GetSize;          
%ignore TK_Unicode_Options::GetOptions;
%ignore TK_User_Index::GetIndices;
%ignore TK_User_Index::GetValues;
%ignore TK_Window::GetWindow;

#define alter
#define null

%native (ReleaseBuffer) void ReleaseBuffer(unsigned char * b, int s);
%native (PrepareBuffer) int PrepareBuffer(BStreamFileToolkit *tk, unsigned char * b, int s);
%extend BStreamFileToolkit {
	TK_Status ParseBuffer(unsigned char * b, int s, TK_Status mode = TK_Normal) {
		return self->ParseBuffer((const char *) b, s, mode);
	}
}
%ignore ParseBuffer;

%include "custom_methods.i"
%include "directors.i"

%include "../../../base_stream/source/stream_common/BStream.h"
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

