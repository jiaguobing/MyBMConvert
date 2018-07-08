/* swig -v -java -c++ -package com.techsoft.hoops -outdir java_mvo hjmvo.i */

%module(directors="1", allprotected="1") HJMVO

%include "hj_version.i"

#pragma SWIG nowarn=302			// id redefined
#pragma SWIG nowarn=350,351		// operator new/delete ignored
#pragma SWIG nowarn=451			// setting const char * may leak memory
#pragma SWIG nowarn=473			// returning pointer/ref in director not recommended
#pragma SWIG nowarn=503			// can't wrap id unless renamed (for operators)
#pragma SWIG nowarn=516			// overload ignored
#pragma SWIG nowarn=813			// multiple inheritance ignored


// ================================================================================
%{
/* extra system includes that swig doesn't put in */
#include <assert.h>
#include <ctype.h>
#include <locale.h>

#ifdef _MSC_VER
#	include <tchar.h>
#endif

/* 3dgs includes */
#include "hc.h"
#include "hic.h"

/* utility includes */
#include "varray.h"
#include "vhash.h"

/* hstream includes */
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
#include "HMaterialLibrary.h"
#include "HImManager.h"
#include "HImUtility.h"
#include "HConstantFrameRate.h"
#include "HDebugZBuffer.h"
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
#include "HOpCreatePolyline.h"
#include "HOpCreateRectangle.h"
#include "HOpCreateSphere.h"
#include "HOpCameraManipulate.h"
#include "HOpCameraOrbit.h"
#include "HOpCameraOrbitTurntable.h"
#include "HOpCameraPan.h"
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

#include "HObjects.h"
#include "HUtilityAnnotationMeasure.h"
#include "HUtilityGeometryCreation.h"
#include "HUtilityGeomHandle.h"
#include "HUtilityQuadTree.h"
#include "HUtilityShadow.h"
#include "HUtilitySubwindow.h"
#include "HUtilityTextParser.h"

#include "HMarkupManager.h"
#include "HModelInfo.h"
#include "HOpSelectPolyline.h"
#include "HSnapGrid.h"
#include "HTCObjectRotate.h"
#include "HUndoManager.h"

#include "HTManager.h"
#include "HImCuttingGeometry.h"
#include "HImErrorHandler.h"
#include "HImUtility.h"
#include "HUtilitySparseShell.h"
#include "HIOUtilityPDF.h"
#include "HUtilityPMI.h"

#include "HDispatchXML.h"
#include "HBhvBehaviorManager.h"
#include "HBhvAnimation.h"

#ifdef WIN64
#	define HLONG __int64
#else
#	define HLONG long
#endif

typedef jobject StringBuffer;
%}

// ===========================================================================
%define ARRAY_TYPEMAP(TYPE, JNITYPE, JTYPE, JAVATYPE, JNIDESC, TYPECHECKTYPE)
%typemap(jni) TYPE *, TYPE &, TYPE[], TYPE[ANY] %{JNITYPE##Array%}
%typemap(jtype) TYPE *, TYPE &, TYPE[], TYPE[ANY] "JTYPE[]"
%typemap(jstype) TYPE *, TYPE &, TYPE[], TYPE[ANY] "JTYPE[]"
%typemap(javain) TYPE *, TYPE &, TYPE[], TYPE[ANY] "$javainput"
%typemap(javaout) TYPE *, TYPE &, TYPE[], TYPE[ANY] {return $jnicall;}
%typemap(javadirectorin) TYPE *, TYPE &, TYPE[], TYPE[ANY] "$jniinput"
%typemap(javadirectorout) TYPE *, TYPE &, TYPE[], TYPE[ANY] "$javacall"

%typemap(directorout,warning="Need to provide TYPE * directorout typemap") TYPE *, TYPE & 
{
	//directorout not handled!
}

%typemap(directorin,descriptor=JNIDESC,warning="Need to provide TYPE * directorin typemap, TYPE array length is unknown") TYPE *, TYPE &
{
	//directorin not handled!
}

%typemap(in,noblock=1) TYPE *, TYPE &, TYPE[], TYPE[ANY] {
    $1 = $input ? ($1_ltype) JCALL2(Get##JAVATYPE##ArrayElements, jenv, $input, 0) : 0;
}

%typemap(freearg,noblock=1) TYPE *, TYPE &, TYPE[], TYPE[ANY] {
    if ($input) JCALL3(Release##JAVATYPE##ArrayElements, jenv, $input, (JNITYPE *)$1, 0);
}

%typemap(directorin,descriptor=JNIDESC) TYPE &
%{ *(($&1_ltype)&$input) = &$1; %}

%typemap(typecheck) TYPE * = TYPECHECKTYPE;
%typemap(typecheck) TYPE & = TYPECHECKTYPE;
%enddef

%define BASIC_TYPEMAP(TYPE, JNITYPE, JTYPE, JNIDESC)
%typemap(jni) TYPE "JNITYPE"
%typemap(jtype) TYPE "JTYPE"
%typemap(jstype) TYPE "JTYPE"
%typemap(javain) TYPE "$javainput"
%typemap(javaout) TYPE {return $jnicall;}
%typemap(javadirectorin) TYPE "$jniinput"
%typemap(javadirectorout) TYPE "$javacall"
%typemap(directorin,descriptor=JNIDESC) TYPE "$input = (JNITYPE) $1;"
%typemap(in) TYPE "$1 = $input;"
%typemap(out) TYPE "$result = $1;"
%enddef

%define STRING_TYPEMAP(TYPE)
%typemap(jni) TYPE %{jstring%}
%typemap(jtype) TYPE "String"
%typemap(jstype) TYPE "String"
%typemap(javain) TYPE "$javainput"
%typemap(javadirectorin) TYPE "$jniinput"
%typemap(javadirectorout) TYPE "$javacall"
%typemap(freearg) TYPE 
%{if ($1) (jenv)->ReleaseStringUTFChars($input, (const char *)$1);
%}
%typemap(in) TYPE { 
  if ($input) {
    $1 = (char *)jenv->GetStringUTFChars($input, 0);
  }
}
%enddef

%define STRINGBUFFER_TYPEMAP(TYPE)
%typemap(jni) TYPE "StringBuffer"
%typemap(jtype) TYPE "StringBuffer"
%typemap(jstype) TYPE "StringBuffer"
%typemap(javain) TYPE "$javainput"
%typemap(javadirectorin) TYPE "$jniinput"
%typemap(javadirectorout) TYPE "$javacall"
%typemap(freearg) TYPE 
{
	jmethodID method_$1;
	jstring jstring_$1;
	
    method_$1 = jenv->GetMethodID(jenv->GetObjectClass($input),
		"append", "(Ljava/lang/String;)Ljava/lang/StringBuffer;");
	if(method_$1) 
	{
		jstring_$1 = jenv->NewStringUTF((const char *) $1);
		jenv->CallObjectMethod ($input, method_$1, jstring_$1);
	};
	if($1){
		free($1);
	}
}
%typemap(in) TYPE { 
  $1 = NULL;
  if($input != NULL) {
    jmethodID setLengthID;
    jclass sbufClass = jenv->GetObjectClass($input);
    jmethodID toStringID = jenv->GetMethodID(sbufClass, "toString", "()Ljava/lang/String;");
    jstring js = (jstring) jenv->CallObjectMethod($input, toStringID);

    const char *pCharStr = jenv->GetStringUTFChars(js, 0);

    jmethodID capacityID = jenv->GetMethodID(sbufClass, "capacity", "()I");
    jint capacity = jenv->CallIntMethod($input, capacityID);
    $1 = (char *) malloc(capacity+1);
    strcpy($1, pCharStr);

    jenv->ReleaseStringUTFChars(js, pCharStr);

    setLengthID = jenv->GetMethodID(sbufClass, "setLength", "(I)V");
    jenv->CallVoidMethod($input, setLengthID, (jint) 0);
  }

}
%typemap(directorin,descriptor="Ljava/lang/StringBuffer;") TYPE "$input = jenv->NewStringUTF((char *)$1);"
%enddef

// ===========================================================================

%include "arrays_java_mod.i"

// ================================================================================

ARRAY_TYPEMAP(bool, jboolean, boolean, Boolean, "[Ljava/lang/Boolean;", jbooleanArray); 
ARRAY_TYPEMAP(signed char, jbyte, byte, Byte, "[Ljava/lang/Byte;", jbyteArray); 
ARRAY_TYPEMAP(unsigned char, jshort, short, Short, "[Ljava/lang/Short;", jshortArray);     
ARRAY_TYPEMAP(short, jshort, short, Short, "[Ljava/lang/Short;", jshortArray);
ARRAY_TYPEMAP(unsigned short, jint, int, Int, "[Ljava/lang/Integer;", jintArray); 
ARRAY_TYPEMAP(int, jint, int, Int, "[Ljava/lang/Integer;", jintArray);
ARRAY_TYPEMAP(unsigned int, jint, int, Int, "[Ljava/lang/Integer;", jintArray); 
ARRAY_TYPEMAP(long, jint, int, Int, "[Ljava/lang/Integer;", jintArray);
ARRAY_TYPEMAP(unsigned long, jint, int, Int, "[Ljava/lang/Integer;", jintArray); 
ARRAY_TYPEMAP(long long, jlong, long, Long, "[Ljava/lang/Long;", jlongArray);
ARRAY_TYPEMAP(float, jfloat, float, Float, "[Ljava/lang/Float;", jfloatArray);
ARRAY_TYPEMAP(double, jdouble, double, Double, "[Ljava/lang/Double;", jdoubleArray); 

ARRAY_TYPEMAP(__int64, jlong, long, Long, "[Ljava/lang/Long;", jlongArray);
ARRAY_TYPEMAP(HC_KEY, jlong, long, Long, "[J", jlongArray);

%apply ARRAYSOFENUMS[] { PMI::DatumModifier::Type[] };
%apply ARRAYSOFENUMS[] { PMI::DiameterModifier::Type & };
%apply ARRAYSOFENUMS[] { PMI::Tolerance::Type & };
%apply ARRAYSOFENUMS[] { PMI::ZoneModifier::Type & };

JAVA_ARRAYSOFCLASSES(HPoint);
JAVA_ARRAYSOFCLASSES(HVector);
JAVA_ARRAYSOFCLASSES(FaceWithNormalInfo);
JAVA_ARRAYSOFCLASSES(PMI::Polyline);
JAVA_ARRAYSOFCLASSES(PMI::Polygon);
JAVA_ARRAYSOFCLASSES(PMI::Entity);
JAVA_ARRAYSOFCLASSES(PMI::String);
JAVA_ARRAYSOFCLASSES(PMI::TextAttributes);

BASIC_TYPEMAP(unsigned int, jint, int, "I");
// ================================================================================

/* Handle strings */
STRING_TYPEMAP(const char *);
STRING_TYPEMAP(const char &);
STRING_TYPEMAP(PMI::String);

STRINGBUFFER_TYPEMAP(char *);
STRINGBUFFER_TYPEMAP(char &);
STRINGBUFFER_TYPEMAP(char out_pattern[ANY]);
STRINGBUFFER_TYPEMAP(char out_projection[ANY]);

%typemap(directorin,descriptor="Ljava/lang/String;") const char * "$input = jenv->NewStringUTF((const char *)$1);"
%typemap(directorin,descriptor="Ljava/lang/StringBuffer;") StringBuffer "$input = jenv->NewStringUTF((char *)$1);"

// Handles void pointers
%typemap(jni) void * "jlong"
%typemap(jtype) void * "long"
%typemap(jstype) void * "long"
%typemap(javain) void * "$javainput"
%typemap(in) void * %{ $1 = (void*)$input; %}
%typemap(out) void * %{ $result = (jlong)$1; %}
%typemap(javaout) void * {return (long)$jnicall;}
//%typemap(javadirectorin) void* "$jinput"
//%typemap(javadirectorout) void* "$javacall"
                                             

%typemap(javaout) HSelectionItem * %{{
	long cptr = $jnicall;	
	if (cptr == 0)
		return null;		
	String name = HJMVOJNI.HSelectionItem_GetName(cptr,null);	
	if (name.equals("HSmartSelItem"))
		return new HSmartSelItem(cptr,false);	
	else if (name.equals("HSubentitySelItem"))
		return new HSubentitySelItem(cptr,false);	
	else {
		return new HSelectionItem(cptr,false);	
	}		
}%}

// Handle HSelectionSet::SelectSubentity
%typemap(directorin,descriptor="[I") const int faces[]
{
	jfaces = jenv->NewIntArray(count);
	jenv->SetIntArrayRegion(jfaces, 0, count, (jint *) faces);
}
%typemap(directorin,descriptor="[I") const int vertex1[]
{
	jvertex1 = jenv->NewIntArray(count);
	jenv->SetIntArrayRegion(jvertex1, 0, count, (jint *) vertex1);
}
%typemap(directorin,descriptor="[I") const int vertex2[]
{
	jvertex2 = jenv->NewIntArray(count);
	jenv->SetIntArrayRegion(jvertex2, 0, count, (jint *) vertex2);
}

// Handles HC_KEY
BASIC_TYPEMAP(HC_KEY, jlong, long, "J");

%typemap(directorin,descriptor="J") HC_KEY "$input = $1;"
%typemap(directorin,descriptor="[J") HC_KEY include_keys[]
{
	jinclude_keys = jenv->NewLongArray(num_include_keys);
	jlong *long_include_keys = new jlong[num_include_keys];
	for (int i=0; i < num_include_keys; i++)
		long_include_keys[i] = include_keys[i];
	jenv->SetLongArrayRegion(jinclude_keys, 0, num_include_keys, long_include_keys);
	delete [] long_include_keys;
}

%{
// Helper class to convert java long array back to HC_KEY[] which may be 32-bit
class OutarrayHCKeyHelper
{
	JNIEnv *jenv;
	HC_KEY *out_array;
	jlongArray *jlong_array;
	size_t size;	

public:
	OutarrayHCKeyHelper(JNIEnv *ijenv, HC_KEY *iout_array, jlongArray *ijlong_array, size_t isize)
		: jenv(ijenv), out_array(iout_array), jlong_array(ijlong_array), size(isize)
	{
		// Allocate array big enough for Java virtual method to populate
		*jlong_array = jenv->NewLongArray(size);
	}

	~OutarrayHCKeyHelper()
	{
		jlong *elements = jenv->GetLongArrayElements(*jlong_array, 0);

		// Copy jlong array back to HC_KEY array (which may be 32-bits)
		for (size_t i=0; i < size; i++)
			out_array[i] = (HC_KEY)elements[i];

		jenv->ReleaseLongArrayElements((*jlong_array), elements, 0);
	}
};
%}

// Typemap meant specifically for HSelectionSet::GetAll(HC_KEY out_array[])
%typemap(directorin,descriptor="[J") HC_KEY out_array[]
%{
	OutarrayHCKeyHelper arrHelper(jenv, out_array, &jout_array, GetSize());		// HSelectionSet::GetSize()
%}

%typemap(directorin,descriptor="[I") const unsigned short * "*((const unsigned short **)&$input) = $1;"
%typemap(directorin,descriptor="[F") float & "*((float *)&$input) = $1;"

// Java keys are 64-bit. Convert to 32-bit when needed.
%typemap(in,noblock=1) HC_KEY[] (jlong *jlongArray, HC_KEY *hlongArray, jsize len) {
	if ($input) {
		len = jenv->GetArrayLength($input);
		jlongArray = jenv->GetLongArrayElements($input, 0);
		hlongArray = (HC_KEY*)malloc(len * sizeof(HC_KEY));
		for (int i=0; i < (int)len; i++)
			hlongArray[i] = (HC_KEY)jlongArray[i];
		$1 = &hlongArray[0];
	}
}
%typemap(freearg,noblock=1) HC_KEY[] {
	if ($input) {
		for (int i=0; i < (int)len$argnum; i++)
			jlongArray$argnum[i] = hlongArray$argnum[i];
		free(hlongArray$argnum);
		jenv->ReleaseLongArrayElements($input, jlongArray$argnum, 0);
	}
}
%apply HC_KEY[] { HC_KEY[ANY], HC_KEY * };

// ================================================================================

/* This is temporarily ignored */
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
%ignore ExportNameMap;
%ignore HOutputHandlerOptions::m_names;
%ignore HUtility::uconvert;
%ignore HSelectionSet::GetSelectionList();

// These have overloads which return arrays via parameter
%ignore HShellObject::GetFlist() const;
%ignore HShellVertexData::GetFIndexData() const;
%ignore HSelectionItem::GetIncludeKeys() const;
%ignore HSelectionItem::GetFullPathKeys(HBaseView *view);
%ignore HUndoItemMatrix::GetMatrix();
%ignore HUndoItemMatrix::GetKey();

// copying ignore from csharp hcsmvo.i; tlr 2012.08.02
%ignore SubentityMap;

// unsigned char* public member variable not handled
%ignore m_imageOutputBuffer;

// Not handling HTouchInfo * array
%ignore HTouchManager::GetTouches;

%ignore Vertex_List_Node;

// ================================================================================

%include "custom_methods.i"
%include "directors.i"

/* there is a system method GetType from mscorlib on windows */
%rename(GetEventType) HEventListener::GetType;
%rename(GetEventType) HEventInfo::GetType;
%rename(GetFRType)    HConstFRSimpType::GetType;

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
%include "../HMaterialLibrary.h"
%include "../HDB.h"
%include "../HEventInfo.h"
%include "../HBaseOperator.h"

%include "../HUtility.h"
%include "../HIOManager.h"
%include "../HImManager.h"
%include "../HImUtility.h"

/* dependent */
%include "../HConstantFrameRate.h"
%include "../HDebugZBuffer.h"
%include "../HOpObjectTranslate.h"
%include "../HOpConstructCircle.h"
%include "../HOpConstructCone.h"
%include "../HOpConstructCuboid.h"
%include "../HOpConstructCylinder.h"
%include "../HOpConstructNurbsCurve.h"
%include "../HOpConstructPolyline.h"
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
%include "../HOpCreatePolyline.h"
%include "../HOpCreateRectangle.h"
%include "../HOpCreateSphere.h"
%include "../HOpCameraManipulate.h"
%include "../HOpCameraOrbit.h"
%include "../HOpCameraOrbitTurntable.h"
%include "../HOpCameraPan.h"
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
%include "../HSelectionSet.h"
%include "../HUtilityAnnotation.h"
%include "../HEventManager.h"

%include "../HObjects.h"
%include "../HUtilityAnnotationMeasure.h"
%include "../HUtilityGeometryCreation.h"
%include "../HUtilityGeomHandle.h"
%include "../HUtilityQuadTree.h"
%include "../HUtilityShadow.h"
%include "../HUtilitySubwindow.h"
%include "../HUtilityTextParser.h"

%include "../HMarkupManager.h"
%include "../HModelInfo.h"
%include "../HOpSelectPolyline.h"
%include "../HSnapGrid.h"
%include "../HTCObjectRotate.h"
%include "../HUndoManager.h"

%include "../HTManager.h"
%include "../HImCuttingGeometry.h"
%include "../HImErrorHandler.h"
%include "../HImUtility.h"
%include "../HUtilitySparseShell.h"
%include "../HIOUtilityPDF.h"

%include "../HDispatchXML.h"
%include "../HBhvBehaviorManager.h"
%include "../HBhvUtility.h"
%include "../HBhvAnimation.h"

#if SWIG_VERSION > 0x020000		// nspace Requires Swig 2.0
%nspace PMI::Type;
%nspace PMI::Datum::Type;
%nspace PMI::Tolerance::Type;
%nspace PMI::ZoneModifier::Type;
%nspace PMI::DatumModifier::Type;
%nspace PMI::DiameterModifier::Type;
%nspace PMI::Dimension::Type;
%nspace PMI::Dimension::SubType;
%nspace PMI::Roughness::Obtention::Type;
%nspace PMI::Roughness::Applicability::Type;
%nspace PMI::Roughness::Mode::Type;

%typemap(javaimports) SWIGTYPE "import com.techsoft.hoops.*;"	// needed for PMI subpackages
%rename(PMI_String) PMI::String;
%ignore PMI::String::String(char const *);
%ignore PMI::String::String(H_WCS const &);
%ignore PMI::String::String(H_UTF8 const &);

%include "wchar_t.i"	// TODO: apply to rest of MVO (requires hiding const char * overloads)

%include "../HUtilityPMI.h"
#endif
