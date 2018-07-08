/* swig -v -csharp -c++ -dllimport hcsp_bridge1451 hcsp_bridge.i */

%module HCSP

/* handles longs */
%pragma(csharp) imclassclassmodifiers = 
%{
#if _M_X64
using HLONG = System.Int64;
#else
using HLONG = System.Int32;
#endif
unsafe public class %}

%pragma(csharp) moduleclassmodifiers = 
%{
#if _M_X64
using HLONG = System.Int64;
#else
using HLONG = System.Int32;
#endif
unsafe public partial class 
%}
  
// Ensure swigStringHelper is initialized since its initialization may get optimized out
%pragma(csharp) imclasscode = %{
  static $imclassname() {
   if (swigStringHelper == null)
    swigStringHelper = new SWIGStringHelper();
  }
%}

%{

#ifdef WIN64
#	define HLONG __int64
#else
#	define HLONG long
#endif

#include "hc.h"
#include "hcsp_bridge.h"
#include "parasolid_kernel.h"
%}

%typemap(ctype) HLONG "HLONG"
%typemap(imtype) HLONG "HLONG"
%typemap(cstype) HLONG "HLONG"
%typemap(csin) HLONG "$csinput"
%typemap(csout) HLONG {return $imcall;}
%typemap(in) HLONG %{ $1 = $input; %}
%typemap(out) HLONG %{ $result = $1; %}

%typemap(ctype) HLONG * "HLONG *"
%typemap(imtype) HLONG * "HLONG*"
%typemap(cstype) HLONG * "HLONG*"
%typemap(csin) HLONG * "$csinput"
%typemap(in) HLONG * %{ $1 = ($1_ltype)$input; %}
%typemap(out) HLONG * %{ $result = $1; %}
%typemap(csout) HLONG * {return $imcall;}

/* Handles strings */
%typemap(imtype) char * "System.Text.StringBuilder"
%typemap(cstype) char * "System.Text.StringBuilder"
%typemap(csin) char * "$csinput"

/* Handles strings */
%typemap(imtype) const char * "string"
%typemap(cstype) const char * "string"
%typemap(csin) const char * "$csinput"

%define INOUT_TYPEMAP(TYPE, CTYPE, CSTYPE, TYPECHECKPRECEDENCE)

/* Handles pointers to single objects */
%typemap(ctype) TYPE * "CTYPE *"
%typemap(imtype) TYPE * "TYPE*"
%typemap(cstype) TYPE * "TYPE*"
%typemap(csin) TYPE * "$csinput"
%typemap(in) TYPE * %{ $1 = ($1_ltype)$input; %}
%typemap(out) TYPE * %{ $result = $1; %}
%typemap(csout) TYPE * {return $imcall;}
%typemap(csvarin) TYPE *
 %{
   set {
     $imcall;
   } %}
%typemap(csvarout)  TYPE *, TYPE &
 %{
   get {
     return $imcall;
   } %}
   
/* Handles pointers to single objects */
%typemap(ctype)  TYPE*  "CTYPE *"
%typemap(imtype)  TYPE & "ref CSTYPE"
%typemap(cstype)  TYPE & "ref CSTYPE"
%typemap(csin)  TYPE & "ref $csinput"
%typemap(in)  TYPE & %{ $1 = ($1_ltype)$input; %}
%typemap(out)  TYPE & %{ $result = $1; %}
%typemap(csout)  TYPE & {return $imcall;}

/* Handles double pointers to single objects */
%typemap(ctype) TYPE ** "CTYPE **"
%typemap(imtype) TYPE ** "CSTYPE **"
%typemap(cstype) TYPE ** "CSTYPE **"
%typemap(csin) TYPE ** "$csinput"
%typemap(in) TYPE **
%{ $1 = ($1_ltype)$input; %}

/* Handles arrays */
%typemap(ctype) TYPE [] "CTYPE *"
%typemap(imtype) TYPE [] "CSTYPE[]"
%typemap(cstype) TYPE [] "CSTYPE[]"
%typemap(csin) TYPE [] "$csinput"

%typecheck(SWIG_TYPECHECK_##TYPECHECKPRECEDENCE) TYPE *INOUT, TYPE &INOUT ""

%enddef

INOUT_TYPEMAP(bool,               unsigned int,         bool,     BOOL_PTR)
//INOUT_TYPEMAP(char,               char,                 char,     CHAR_PTR)
//INOUT_TYPEMAP(char,        char,          sbyte,    INT8_PTR)
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
//INOUT_TYPEMAP(HLONG,              HLONG,                 HLONG,    INT64_PTR)

#undef INOUT_TYPEMAP


%include "hcsp_bridge.irenames"
%include "../hcsp_bridge.h"

%header %{

extern void HP_Set_Tessellation_Options(
	PK_TOPOL_render_line_o_t *		in_line_options,
	PK_TOPOL_render_facet_go_o_t *	in_go_options,
	PK_TOPOL_facet_mesh_o_t *		in_generation_options);

#ifdef __cplusplus
extern "C" {
#endif

SWIGEXPORT void SWIGSTDCALL CSharp_Set_Tessellation_Options(
	PK_TOPOL_render_line_o_t *		in_line_options,
	PK_TOPOL_render_facet_go_o_t *	in_go_options,
	PK_TOPOL_facet_mesh_o_t *		in_generation_options) {
	HP_Set_Tessellation_Options(in_line_options, in_go_options, in_generation_options);
}

#ifdef __cplusplus
}
#endif

%}

