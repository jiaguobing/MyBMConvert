/* File : hcsmvo.i */
/*  swig -csharp -c++ -dllimport hcsmvo1350 -outdir csharp_mvo hcsmvo.i  */
%module(directors="1", allprotected="1") HCSMVO

#pragma SWIG nowarn=350     /* operator new ignored */
#pragma SWIG nowarn=351     /* operator delete ignored */
#pragma SWIG nowarn=503     /* Can't wrap operator */
#pragma SWIG nowarn=516     /* Overloaded method ignored */

/* Fix these warnings */
#pragma SWIG nowarn=473     /* Returning a pointer or reference in a director method is not recommended. */
#pragma SWIG nowarn=844     /* C# exception may not be thrown - no $excode or excode attribute in 'csout' typemap. */

// ================================================================================
%typemap(csimports) SWIGTYPE %{
using System;
using System.Runtime.InteropServices;

#if _M_X64
using MVO_POINTER_SIZED_INT = System.Int64;
using MVO_POINTER_SIZED_UINT = System.UInt64;
using ID_Key = System.Int64;
#else
using MVO_POINTER_SIZED_INT = System.Int32;
using MVO_POINTER_SIZED_UINT = System.UInt32;
using ID_Key = System.Int32;
#endif

%}

%pragma(csharp) imclassclassmodifiers = 
%{
#if _M_X64
using MVO_POINTER_SIZED_INT = System.Int64;
using MVO_POINTER_SIZED_UINT = System.UInt64;
using ID_Key = System.Int64;
#else
using MVO_POINTER_SIZED_INT = System.Int32;
using MVO_POINTER_SIZED_UINT = System.UInt32;
using ID_Key = System.Int32;
#endif
public class %}

%pragma(csharp) moduleclassmodifiers = 
%{
#if _M_X64
using MVO_POINTER_SIZED_INT = System.Int64;
using MVO_POINTER_SIZED_UINT = System.UInt64;
using ID_Key = System.Int64;
#else
using MVO_POINTER_SIZED_INT = System.Int32;
using MVO_POINTER_SIZED_UINT = System.UInt32;
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
  
  public void owns_memory(bool ownMemory) {
	swigCMemOwn = ownMemory;
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

%pragma(csharp) modulecode=%{
	// Helper methods for methods/properties returning array as IntPtr
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
	public delegate bool HSignalNotifyFunction(int signal, IntPtr signal_data, IntPtr user_data);
%}

// Ensure swigStringHelper is initialized since its initialization may get optimized out
%pragma(csharp) imclasscode = %{
  static $imclassname() {
   if (swigStringHelper == null)
    swigStringHelper = new SWIGStringHelper();
  }
%}

// ================================================================================

%{
#ifdef WIN64
#	define MVO_POINTER_SIZED_INT __int64
#	define MVO_POINTER_SIZED_UINT unsigned __int64
#else
#	define MVO_POINTER_SIZED_INT long
#	define MVO_POINTER_SIZED_UINT unsigned long
#endif

#include <assert.h>
#include <ctype.h>
#include <locale.h>
#include <tchar.h>

/* 3dgs includes */
#include "hc.h"
#include "hic.h"

/* utility includes */
#include "varray.h"
#include "vhash.h"

#include "HStream.h"
#include "HStreamFileToolkit.h"

/* MVO Includes */
#include "HTools.h"
#include "HGlobals.h"
#include "HEventListener.h"
#include "HBaseModel.h"
#include "HBaseView.h"
#include "HDB.h"
#include "HEventInfo.h"
#include "HBaseOperator.h"
#include "HUtility.h"
#include "HIOManager.h"
#include "HImManager.h"
#include "HMaterialLibrary.h"
#include "HImUtility.h"
#include "HConstantFrameRate.h"
#include "HDebugZBuffer.h"
#include "HGraph.h"
#include "HOpObjectTranslate.h"
#include "HOpConstructCircle.h"
#include "HOpConstructCone.h"
#include "HOpConstructCuboid.h"
#include "HOpConstructCylinder.h"
#include "HOpConstructNurbsCurve.h"
#include "HOpConstructPolyline.h"
#include "HOpConstructRectangle.h"
#include "HOpConstructSphere.h"
#include "HOpCreateBoolean.h"
#include "HOpCreateCircle.h"
#include "HOpCreateCone.h"
#include "HOpCreateCuboid.h"
#include "HOpCreateCuttingPlane.h"
#include "HOpCreateCylinder.h"
#include "HOpCreateNurbs3D.h"
#include "HOpCreateNurbsCurve.h"
#include "HOpCreateSphere.h"
#include "HOpCameraManipulate.h"
#include "HOpCameraOrbit.h"
#include "HOpCameraPan.h"
#include "HOpCreatePolyline.h"
#include "HOpCreateRectangle.h"
#include "HOpCameraRelativeOrbit.h"
#include "HOpCameraWalk.h"
#include "HOpCameraZoom.h"
#include "HOpCameraZoomBox.h"
#include "HOpMarkupAnnotate.h"
#include "HOpMarkupCircle.h"
#include "HOpMarkupFreehand.h"
#include "HOpMarkupMeasure.h"
#include "HOpMarkupPostIt.h"
#include "HOpMarkupRectangle.h"
#include "HOpMoveHandle.h"
#include "HOpNavCube.h"
#include "HOpNetwork.h"
#include "HOpObjectAnnotate.h"
#include "HOpObjectClash.h"
#include "HOpObjectRotate.h"
#include "HOpObjectTranslate.h"
#include "HOpSelectArea.h"
#include "HOpSelectAperture.h"
#include "HOpSelectPolygon.h"
#include "HOpSelectPolyline.h"
#include "HSelectionItem.h"
#include "HSelectionSet.h"
#include "HUtilityAnnotation.h"
#include "HEventManager.h"
#include "HUtilityAnnotationMeasure.h"
#include "HUtilityGeometryCreation.h"
#include "HUtilityGeomHandle.h"
#include "HUtilityQuadTree.h"
#include "HUtilityShadow.h"
#include "HUtilityTextParser.h"
#include "HMarkupManager.h"
#include "HModelInfo.h"
#include "HOpSelectPolyline.h"
#include "HSnapGrid.h"
#include "HTManager.h"
#include "HUndoManager.h"
#include "HImCuttingGeometry.h"
#include "HImErrorHandler.h"
#include "HImUtility.h"
#include "HUtilitySparseShell.h"
#include "HIOUtilityPDF.h"      
#include "HOpCameraOrbitTurntable.h"

#include "HBhvBehaviorManager.h"
#include "HBhvAnimation.h"

#include "HDispatchXML.h"

#include "HOpCameraManipulateTouch.h"
#include "HOpCameraPanTouch.h"
#include "HOpCameraRollTouch.h"
#include "HOpCameraZoomTouch.h"

%}

// ================================================================================

%include "wchar.i"			// in swig lib

// some fixes
%typemap(csvarout, excode=SWIGEXCODE2) wchar_t * %{
    get {
      string ret = System.Runtime.InteropServices.Marshal.PtrToStringUni($imcall);$excode
      return ret;
    } %}

%typemap(csdirectorin) wchar_t * "System.Runtime.InteropServices.Marshal.PtrToStringUni($iminput)"
%typemap(directorin) wchar_t const * "$input = ($1_ltype)$1;"
//%typemap(in) wchar_t * %{ teest %}

// ================================================================================

%typemap(ctype) HC_KEY "MVO_POINTER_SIZED_INT"
%typemap(imtype) HC_KEY "MVO_POINTER_SIZED_INT"
%typemap(cstype) HC_KEY "MVO_POINTER_SIZED_INT"
%typemap(csin) HC_KEY "$csinput"
%typemap(csout) HC_KEY {return $imcall;}
%typemap(in) HC_KEY %{ $1 = $input; %}
%typemap(out) HC_KEY %{ $result = $1; %}
%typemap(csvarin) HC_KEY %{
	set { $imcall; } %}
%typemap(csvarout) HC_KEY %{
	get { return $imcall; } %}
%typemap(csdirectorin) HC_KEY "$iminput"
%typemap(csdirectorout) HC_KEY "$cscall"
%typemap(directorin) HC_KEY "$input = $1;"

%typemap(ctype) MVO_POINTER_SIZED_UINT "MVO_POINTER_SIZED_UINT"
%typemap(imtype) MVO_POINTER_SIZED_UINT "MVO_POINTER_SIZED_UINT"
%typemap(cstype) MVO_POINTER_SIZED_UINT "MVO_POINTER_SIZED_UINT"
%typemap(csin) MVO_POINTER_SIZED_UINT "$csinput"
%typemap(csout) MVO_POINTER_SIZED_UINT {return $imcall;}
%typemap(in) MVO_POINTER_SIZED_UINT %{ $1 = $input; %}
%typemap(out) MVO_POINTER_SIZED_UINT %{ $result = $1; %}
%typemap(csvarin) MVO_POINTER_SIZED_UINT %{
	set { $imcall; } %}
%typemap(csvarout) MVO_POINTER_SIZED_UINT %{
	get { return $imcall; } %}
%typemap(csdirectorin) MVO_POINTER_SIZED_UINT "$iminput"
%typemap(csdirectorout) MVO_POINTER_SIZED_UINT "$cscall"
%typemap(directorin) MVO_POINTER_SIZED_UINT "$input = $1;"

%typemap(ctype) MVO_POINTER_SIZED_INT "MVO_POINTER_SIZED_INT"
%typemap(imtype) MVO_POINTER_SIZED_INT "MVO_POINTER_SIZED_INT"
%typemap(cstype) MVO_POINTER_SIZED_INT "MVO_POINTER_SIZED_INT"
%typemap(csin) MVO_POINTER_SIZED_INT "$csinput"
%typemap(csout) MVO_POINTER_SIZED_INT {return $imcall;}
%typemap(in) MVO_POINTER_SIZED_INT %{ $1 = $input; %}
%typemap(out) MVO_POINTER_SIZED_INT %{ $result = $1; %}
%typemap(csvarin) MVO_POINTER_SIZED_INT %{
	set { $imcall; } %}
%typemap(csvarout) MVO_POINTER_SIZED_INT %{
	get { return $imcall; } %}
%typemap(csdirectorin) MVO_POINTER_SIZED_INT "$iminput"
%typemap(csdirectorout) MVO_POINTER_SIZED_INT "$cscall"
%typemap(directorin) MVO_POINTER_SIZED_INT "$input = $1;"

%typemap(ctype) void * "void *"
%typemap(imtype) void * "IntPtr"
%typemap(cstype) void * "IntPtr"
%typemap(csin) void * "$csinput"
%typemap(in) void * %{ $1 = $input; %}
%typemap(out) void * %{ $result = $1; %}
%typemap(csout) void * {return new IntPtr((int)$imcall);}
%typemap(csvarin) void * %{
	set { $imcall; } %}
%typemap(csvarout) void * %{
	get { return new IntPtr((int)$imcall); } %}
   
%typemap(imtype) int (*op)(int,int) "HCS.usrfunc"
%typemap(cstype) int (*op)(int,int) "HCS.usrfunc"
%typemap(csin) int (*op)(int,int) "$csinput"

// Handles delegates of error handlers 
%typemap(imtype) void (*op)(int, int, int, int, char **, int, char **) "HCSU.errfunc"
%typemap(cstype) void (*op)(int, int, int, int, char **, int, char **) "HCSU.errfunc"
%typemap(csin) void (*op)(int, int, int, int, char **, int, char **) "$csinput"

// Handles strings 
%typemap(imtype) const char * "string"
%typemap(cstype) const char * "string"
%typemap(csin) const char * "$csinput"

%typemap(imtype) char const * const "string"
%typemap(cstype) char const * const "string"
%typemap(csin) char const * const "$csinput"

// Handles strings 
%typemap(imtype) char * "System.Text.StringBuilder"
%typemap(cstype) char * "System.Text.StringBuilder"
%typemap(csin) char * "$csinput"
%typemap(csvarin) char * %{
	set { $imcall; } %}
%typemap(csvarout) char * %{
	get { return $imcall; } %}

// For HInputHandler::FileInputToImageKey (map image_key param as array param; mapping as out param doesn't work w/ directors)
// The image_key param currently does not work.  Storing a value in image_key[0] will not propagate back to c++.
// We should use an "out" param, but swig cannot describe the director type sig as typeof(MVO_POINTER_SIZED_INT).MakeByRefType()
//%typemap(ctype) HC_KEY * image_key "MVO_POINTER_SIZED_INT *"
//%typemap(imtype,out="MVO_POINTER_SIZED_INT") HC_KEY * image_key "out MVO_POINTER_SIZED_INT"
//%typemap(cstype,out="MVO_POINTER_SIZED_INT") HC_KEY * image_key "out MVO_POINTER_SIZED_INT"
//%typemap(csin) HC_KEY * image_key "out $csinput"
%typemap(imtype) HC_KEY * image_key "MVO_POINTER_SIZED_INT[]";
%typemap(cstype) HC_KEY * image_key "MVO_POINTER_SIZED_INT[]";
%typemap(csdirectorin) HC_KEY * image_key "$iminput"

%typemap(ctype) bool "bool"
%typemap(imtype,outattributes="[return: MarshalAs(UnmanagedType.U1)]") bool "bool"

%typemap(cstype) HSignalNotifyFunction "HCSMVO.HSignalNotifyFunction";
%typemap(imtype) HSignalNotifyFunction "HCSMVO.HSignalNotifyFunction";
%typemap(csin) HSignalNotifyFunction "$csinput";

   
// ================================================================================
%define INOUT_TYPEMAP(TYPE, CTYPE, CSTYPE, TYPECHECKPRECEDENCE)

%typemap(ctype) const TYPE * "const CTYPE *"
%typemap(ctype) TYPE *, TYPE &, TYPE [] "CTYPE *"

%typemap(imtype, out="IntPtr") TYPE * "CSTYPE[]"
%typemap(imtype) TYPE &, TYPE [] "CSTYPE[]"

%typemap(cstype, out="IntPtr") TYPE * "CSTYPE[]"
%typemap(cstype) TYPE &, TYPE [] "CSTYPE[]"

%typemap(in) TYPE *, TYPE &, TYPE [] %{ $1 = ($1_ltype)$input; %}
%typemap(out) TYPE *, TYPE &, TYPE [] %{ $result = $1; %}
%typemap(csin) TYPE *, TYPE &, TYPE [] "$csinput"
%typemap(csout) TYPE *, TYPE &, TYPE [] {return $imcall;}
%typemap(csdirectorin) TYPE * "out $iminput"
%typemap(csdirectorin) TYPE &, TYPE [] "$iminput"
%typemap(csdirectorout) TYPE *, TYPE &, TYPE [] "$cscall"
%typemap(directorin) TYPE *, TYPE [] "$input = (TYPE*)$1;"
%typemap(directorin) TYPE & "$input = &$1;"
%typemap(csvarin) TYPE *, TYPE &, TYPE [] %{
	set { $imcall; } %}
%typemap(csvarout)  TYPE *, TYPE &, TYPE [] %{
	get { return $imcall; } %}

%typecheck(SWIG_TYPECHECK_##TYPECHECKPRECEDENCE) TYPE *INOUT, TYPE &INOUT ""
%enddef

//           TYPE,                    CTYPE,                 CSTYPE
//INOUT_TYPEMAP(char,                   char,                 char,     CHAR_PTR)
//INOUT_TYPEMAP(void,                   signed char,          sbyte,    INT8_PTR)
//INOUT_TYPEMAP(bool,                   bool,          bool,     BOOL_PTR)		// unable to get this to work correctly due to default bool[] marshalling of 4 bytes per bool
INOUT_TYPEMAP(bool,                   unsigned int,          bool,     BOOL_PTR)
INOUT_TYPEMAP(signed char,            signed char,           sbyte,    INT8_PTR)
INOUT_TYPEMAP(unsigned char,          unsigned char,         byte,     UINT8_PTR)
INOUT_TYPEMAP(short,                  short,                 short,    INT16_PTR)
INOUT_TYPEMAP(unsigned short,         unsigned short,        ushort,   UINT16_PTR)
INOUT_TYPEMAP(int,                    int,                   int,      INT32_PTR)
INOUT_TYPEMAP(unsigned int,           unsigned int,          uint,     UINT32_PTR)
INOUT_TYPEMAP(long,                   long,                  int,      INT32_PTR)
INOUT_TYPEMAP(unsigned long,          unsigned long,         uint,     UINT32_PTR)
INOUT_TYPEMAP(long long,              long long,             long,     INT64_PTR)
INOUT_TYPEMAP(unsigned long long,     unsigned long long,    ulong,    UINT64_PTR)
INOUT_TYPEMAP(float,                  float,                 float,    FLOAT_PTR)
INOUT_TYPEMAP(double,                 double,                double,   DOUBLE_PTR)
INOUT_TYPEMAP(__int64,                long,                  MVO_POINTER_SIZED_INT,    INT64_PTR)
INOUT_TYPEMAP(MVO_POINTER_SIZED_INT,  MVO_POINTER_SIZED_INT, MVO_POINTER_SIZED_INT,    INT64_PTR)
INOUT_TYPEMAP(unsigned __int64,       unsigned long,         MVO_POINTER_SIZED_UINT,   UINT64_PTR)
INOUT_TYPEMAP(MVO_POINTER_SIZED_UINT, MVO_POINTER_SIZED_UINT,MVO_POINTER_SIZED_UINT,   UINT64_PTR)
INOUT_TYPEMAP(HC_KEY,                 MVO_POINTER_SIZED_INT, MVO_POINTER_SIZED_INT,    INT64_PTR)

// ================================================================================

// Marshal bool as 1 byte instead of default 4 (ie, Win32 BOOL)
// Not working for bool[]
//%typemap(imtype,
//	inattributes="[MarshalAs(UnmanagedType.U1)]",
//	outattributes="[MarshalAs(UnmanagedType.U1)]",
//	directorinattributes="[MarshalAs(UnmanagedType.U1)]",
//	directoroutattributes="[MarshalAs(UnmanagedType.U1)]") bool &, bool[] "bool[]"

// Override for HAnimationListener::KeyframeEditorExistsQuery(bool &res)
// Note: would like to do this, but swig tries to call typeof(out bool)
//%typemap(cstype)			bool &res "out bool"
//%typemap(csin)				bool &res "out $csinput"
//%typemap(imtype)			bool &res "out bool"
//%typemap(csdirectorin)		bool &res "out $iminput"


// ================================================================================

// Make array member variables read-only since memory management can be ambiguous
%feature("immutable") HShell::face_list;
%feature("immutable") HShell::point_list;
%feature("immutable") HShell::normal_list;
%feature("immutable") HShell::face_list;
%feature("immutable") HShell::region_list;
%feature("immutable") HPolyline::m_pHPointArray;
%feature("immutable") HQualifiedKey::m_pIncludeList;
%feature("immutable") ColorPiece::m_uvindex;
%feature("immutable") ColorPiece::m_fllen;
%feature("immutable") ColorPiece::m_flindex;
%feature("immutable") HKeyframeMatrix::m_matrix;
%feature("immutable") HKeyframeChannelCurve::control_polygon;
%feature("immutable") HKeyframeChannelFollowPath::tmatrix;
%feature("immutable") HKeyframeChannelFollowPath::tmatrix2;
%feature("immutable") HShellWrapper::m_Points;
%feature("immutable") HShellWrapper::m_VParams;
%feature("immutable") HShellWrapper::m_VColors;
%feature("immutable") HShellWrapper::m_VNormals;
%feature("immutable") HShellWrapper::m_Flist;
%feature("immutable") HShellWrapper::m_faceregion;
%feature("immutable") HShellWrapper::m_faceindirect;
%feature("immutable") HShellWrapper::m_FNArray;
%feature("immutable") HOutputHandlerOptions::m_PageMargins;
%feature("immutable") HShowContentsWithPathType::patharray;
%feature("immutable") HOpConstructNurbsCurve::m_WeightsArray;
%feature("immutable") HOpConstructNurbsCurve::m_KnotsArray;
%feature("immutable") ::IndexToPair;
%feature("immutable") ::bigIndices;

/* This is to make certain classes "unsafe" */
//%typemap(csclassmodifiers) HBaseView "unsafe public class"
//%typemap(csclassmodifiers) HTClient "unsafe public class"
//%typemap(csclassmodifiers) HIOConnector "unsafe public class"

%ignore null;
%ignore patharray;
%ignore AdjustToTick;
%ignore UpdateKeyList;

%ignore draw_segment_in_background;
%ignore draw_dc_polyline_infront;
%ignore draw_device_independent_line_weights;
%ignore visible_hlr_polyline;

%ignore SetShadowParams(HShadow* shadowObj);
%ignore m_KeyList;
%ignore m_pKeyframeGenerator;
%ignore m_pSegmentTracker;

%ignore HUndoItemMatrix::GetMatrix();
%ignore HUndoItemMatrix::GetKey();

%ignore ExportNameMap;
%ignore HOutputHandlerOptions::m_names;
%ignore HPDFOptions;
%ignore HOutputHandlerOptions::ExtendedData(HPDFOptions * const);

//%ignore HUtility::uconvert;
%ignore HSelectionSet::GetSelectionList();

// These have overloads which return arrays via parameter
//%ignore HShellObject::GetFlist() const;
//%ignore HShellVertexData::GetFIndexData() const;
//%ignore HSelectionItem::GetIncludeKeys() const;
//%ignore HSelectionItem::GetFullPathKeys(HBaseView *view);
//%ignore HUndoItemMatrix::GetMatrix();
//%ignore HUndoItemMatrix::GetKey();

%ignore HOutputHandler::FileOutputByKey(const unsigned short*, HC_KEY, HOutputHandlerOptions *);
%ignore HOutputHandler::FileOutputByKey(const char*, HC_KEY, HOutputHandlerOptions *);
%ignore HOutputHandler::FileOutputFromImageKey(const unsigned short*, HC_KEY, HOutputHandlerOptions *);
%ignore HOutputHandler::FileOutputFromImageKey(const char*, HC_KEY, HOutputHandlerOptions *);
%ignore HInputHandler::FileInputByKey(const unsigned short*, HC_KEY, HInputHandlerOptions *);
%ignore HInputHandler::FileInputByKey(const char*, HC_KEY, HInputHandlerOptions *);
%ignore HInputHandler::FileInputToImageKey(const unsigned short*, HC_KEY *, HC_KEY, HInputHandlerOptions *);
%ignore HInputHandler::FileInputToImageKey(const char*, HC_KEY *, HC_KEY, HInputHandlerOptions *);

// Unable to correctly map as out bool, or bool[]
%ignore KeyframeEditorExistsQuery;

%ignore SubentityMap;

%ignore m_pPRCDeleteModelCallback;

// unsigned char* public member variable not handled
%ignore m_imageOutputBuffer;

// Not handling HTouchInfo * array
%ignore HTouchManager::GetTouches;

%ignore Vertex_List_Node;


// Map HPoint array to float array
%typemap(imtype) HPoint[] "float[]"
%typemap(cstype) HPoint[] "float[]"
%typemap(csin)  HPoint[] "$csinput"
%typemap(csout) HPoint[] {return $imcall;}
%typemap(csdirectorin)  HPoint[] "$iminput"
%typemap(csdirectorout) HPoint[] "$cscall"

/* SWIG doesn't support multiple inheritance so we rescope the less important methods */
%csmethodmodifiers HBaseView::OnLButtonDown     "public virtual";
%csmethodmodifiers HBaseView::OnLButtonDblClk	"public virtual";
%csmethodmodifiers HBaseView::OnLButtonUp       "public virtual";
%csmethodmodifiers HBaseView::OnMouseWheel      "public virtual";
%csmethodmodifiers HBaseView::OnMouseMove       "public virtual";
%csmethodmodifiers HBaseView::OnKeyDown			"public virtual";
%csmethodmodifiers HBaseView::OnKeyUp			"public virtual";
%csmethodmodifiers HBaseView::SetupHandlesEvent "public virtual";
%csmethodmodifiers HBaseView::ObjectMovedEvent  "public virtual";

/* there is a system method GetType from mscorlib on windows */
%rename(GetEventType) HEventListener::GetType;
%rename(GetEventType) HEventInfo::GetType;
%rename(GetFRType)    HConstFRSimpType::GetType;
%rename(GetHType)		GetType;

/* when adding an undo item to the list, give C++ ownership of the memory (so C# won't free it) */
%typemap(cscode) HUndoManager %{
	public void AddUndoItem(HUndoItem item) {
		item.owns_memory(false);
		AddUndoItemInternal(item);
	}
%}
%csmethodmodifiers HUndoManager::AddUndoItem  "private";
%rename(AddUndoItemInternal) HUndoManager::AddUndoItem;


%typemap(csout) HSelectionItem * %{{
	IntPtr cptr = $imcall;	
	if (cptr == IntPtr.Zero)
		return null;		
	string name = HCSMVOPINVOKE.HSelectionItem_GetName(new HandleRef(null,cptr));	
	if (name == "HSmartSelItem")
		return new HSmartSelItem(cptr,false);	
	else if (name == "HSubentitySelItem")
		return new HSubentitySelItem(cptr,false);	
	else {
		return new HSelectionItem(cptr,false);	
	}		
}%}


// ================================================================================

%include "directors.i"

#define MVO_POINTER_SIZED_INT MVO_POINTER_SIZED_INT
#define MVO_POINTER_SIZED_UINT MVO_POINTER_SIZED_UINT

%import "../../../base_stream/source/stream_common/BStream.h"
%import "../../../base_stream/source/stream_common/BStreamFileToolkit.h"
%import "../../../base_stream/source/stream_common/BOpcodeHandler.h"

%import "../../../hoops_stream/source/HStream.h"
%import "../../../hoops_stream/source/HStreamFileToolkit.h"

/* base classes - THE ORDERING HERE IS CRITICAL SO DON"T CHANGE IT */
%include "../HTools.h"
%include "../HGlobals.h"
%include "../HEventListener.h"
%include "../HBaseModel.h"
%include "../HBaseView.h"
%include "../HDB.h"
%include "../HEventInfo.h"
%include "../HBaseOperator.h"

%include "../HUtility.h"
%include "../HIOManager.h"
%include "../HImManager.h"
%include "../HImUtility.h"
%include "../HUtilitySparseShell.h"
%include "../HMaterialLibrary.h"
%include "../HTManager.h"

/* dependent */
%include "../HConstantFrameRate.h"
%include "../HDebugZBuffer.h"
%include "../HGraph.h"
%include "../HOpObjectTranslate.h"
%include "../HOpConstructCircle.h"
%include "../HOpConstructCone.h"
%include "../HOpConstructCuboid.h"
%include "../HOpConstructCylinder.h"
%include "../HOpConstructPolyline.h"
%include "../HOpConstructNurbsCurve.h"
%include "../HOpConstructRectangle.h"
%include "../HOpConstructSphere.h"
%include "../HOpCreateBoolean.h"
%include "../HOpCreateCircle.h"
%include "../HOpCreateCone.h"
%include "../HOpCreateCuboid.h"
%include "../HOpCreateCuttingPlane.h"
%include "../HOpCreateCylinder.h"
%include "../HOpCreateNurbs3D.h"
%include "../HOpCreateNurbsCurve.h"
%include "../HOpCreateSphere.h"
%include "../HOpCameraManipulate.h"
%include "../HOpCameraOrbit.h"
%include "../HOpCameraPan.h"
%include "../HOpCreatePolyline.h"
%include "../HOpCreateRectangle.h"
%include "../HOpCameraRelativeOrbit.h"
%include "../HOpCameraWalk.h"
%include "../HOpCameraZoom.h"
%include "../HOpCameraZoomBox.h"
%include "../HOpMarkupAnnotate.h"
%include "../HOpMarkupCircle.h"
%include "../HOpMarkupFreehand.h"
%include "../HOpMarkupMeasure.h"
%include "../HOpMarkupPostIt.h"
%include "../HOpMarkupRectangle.h"
%include "../HOpMoveHandle.h"
%include "../HOpNavCube.h"
%include "../HOpNetwork.h"
%include "../HOpObjectAnnotate.h"
%include "../HOpObjectClash.h"
%include "../HOpObjectRotate.h"
%include "../HOpObjectTranslate.h"
%include "../HOpSelectArea.h"
%include "../HOpSelectAperture.h"
%include "../HOpSelectPolygon.h"
%include "../HOpSelectPolyline.h"
%include "../HSelectionItem.h"

// The following typemaps only applies to functions in HSelectionSet.
/*
	What we're doing here is adding special attributes to give C# a hint about the length of all these array arguments
	to HSelectionSet functions. Without these, when the user overrides these functions in C# they will be passed arrays of
	length 1, regardless of the size of the true array.	
*/ 
%typemap (imtype,directorinattributes="[MarshalAs(UnmanagedType.LPArray,SizeParamIndex=1)]") HC_KEY include_keys[] "MVO_POINTER_SIZED_INT[]"
%typemap (imtype,directorinattributes="[MarshalAs(UnmanagedType.LPArray,SizeParamIndex=3)]") const int faces[] "int[]"
%typemap (imtype,directorinattributes="[MarshalAs(UnmanagedType.LPArray,SizeParamIndex=3)]") const int vertex1[] "int[]"
%typemap (imtype,directorinattributes="[MarshalAs(UnmanagedType.LPArray,SizeParamIndex=3)]") const int vertex2[] "int[]"

%include "../HSelectionSet.h"

%clear HC_KEY *include_keys;
%clear const int * faces;
%clear const int * vertex1;
%clear const int * vertex2;

%typemap (imtype, out = "IntPtr") unsigned char ** "out IntPtr"
%typemap (cstype, out = "IntPtr") unsigned char ** "out IntPtr"
%typemap(csin) unsigned char ** "out $csinput"
%typemap(csout) unsigned char ** {return $imcall;}

%include "../HUtilityAnnotation.h"
%include "../HEventManager.h"

%csmethodmodifiers MoveText "new public virtual";
%include "../HUtilityAnnotationMeasure.h"
%clear MoveText;

%include "../HUtilityGeometryCreation.h"
%include "../HUtilityGeomHandle.h"
%include "../HUtilityQuadTree.h"
%include "../HUtilityShadow.h"
%include "../HUtilityTextParser.h"
%include "../HMarkupManager.h"
%include "../HModelInfo.h"
%include "../HOpSelectPolyline.h"
%include "../HSnapGrid.h"
%include "../HUndoManager.h"
%include "../HImCuttingGeometry.h"
%include "../HImErrorHandler.h"
%include "../HImUtility.h"
%include "../HIOUtilityPDF.h"
%include "../HOpCameraOrbitTurntable.h"

// HDispatchXML needs these typemaps. 
// For some reason adding them higher up in this file causes them to not take effect.
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

%include "../HDispatchXML.h"

/* these cause build errors from multiple inheritance, etc  */
/*

%include "../HIOUtilityAVI.h"
%include "../HIOUtilityHOOPS.h"
%include "../HIOUtilityHsf.h"
%include "../HIOUtilityLeadTools.h"
%include "../HIOUtilityMagick.h"
%include "../HIOUtilityObj.h"
%include "../HIOUtilityPly.h"
%include "../HIOUtilityStl.h"
%include "../HIOUtilityVRML.h"



//tested and don't work as is (some have multiple inheritance, others aren't exposed or even compiled)
%include "../HUtilityAntialiasing.h"
%include "../HUtilityLocaleString.h"
%include "../HUtilitySubwindow.h"
%include "../HObjects.h"
%include "../HOpMarkupFreehand3D.h"
%include "../HImHideOverlappedText.h"
*/

%include "../HBhvBehaviorManager.h"
%include "../HBhvUtility.h"
%include "../HBhvAnimation.h"


%include "../HOpCameraManipulateTouch.h"
%include "../HOpCameraPanTouch.h"
%include "../HOpCameraRollTouch.h"
%include "../HOpCameraZoomTouch.h"

/* hoops_mfc includes */
/*
%include "../../../hoops_mfc/source/HMFCGlobals.h"
%include "../../hoops_mfc/source/HIOUtilityGDIOutput.h"
*/


