/* swig -v -csharp -c++ -dllimport hcsp_bridge1451 hcsp_bridge.i */

%module HJP


%{
#include "hc.h"
#include "hcsp_bridge.h"

typedef jobject StringBuffer;
%}

//These are the main typemap definitions
//----------------------------------------------------------------------------------------
%define INOUT_TYPEMAP(TYPE, JNITYPE, JTYPE, JAVATYPE, JNIDESC, TYPECHECKTYPE)
%typemap(jni) TYPE *, TYPE &, TYPE[] %{JNITYPE##Array%}
%typemap(jtype) TYPE *, TYPE &, TYPE[] "JTYPE[]"
%typemap(jstype) TYPE *, TYPE &, TYPE[] "JTYPE[]"
%typemap(javain) TYPE *, TYPE &, TYPE[] "$javainput"
%typemap(javaout) TYPE *, TYPE &, TYPE[] {return $jnicall;}
%typemap(javadirectorin) TYPE *, TYPE &, TYPE[] "$jniinput"
%typemap(javadirectorout) TYPE *, TYPE &, TYPE[] "$javacall"


/* Handle strings */
%typemap(jni) const char *, const char & %{jstring%}
%typemap(jtype) const char *, const char & "String"
%typemap(jstype) const char *, const char & "String"
%typemap(javain) const char *, const char & "$javainput"
%typemap(javadirectorin) const char *, const char & "$jniinput"
%typemap(javadirectorout) const char *, const char & "$javacall"
%typemap(freearg) const char *, const char & 
%{if ($1) (jenv)->ReleaseStringUTFChars($input, (const char *)$1);
%}
%typemap(in) const char *, const char & { 
  if ($input) {
    $1 = (char *)jenv->GetStringUTFChars($input, 0);
  }
}


%typemap(jni) char *, char & "StringBuffer"
%typemap(jtype) char *, char & "StringBuffer"
%typemap(jstype) char *, char & "StringBuffer"
%typemap(javain) char *, char & "$javainput"
%typemap(javadirectorin) char *, char & "$jniinput"
%typemap(javadirectorout) char *, char & "$javacall"
%typemap(freearg) char *, char & 
{
	jmethodID method_$1;
	jstring jstring_$1;
	
    method_$1 = (jenv)->GetMethodID(jenv->GetObjectClass($input),
		"append", "(Ljava/lang/String;)Ljava/lang/StringBuffer;");
	if(method_$1) 
	{
		jstring_$1 = jenv->NewStringUTF((const char *) $1);
		(jenv)->CallObjectMethod ( $input, method_$1, jstring_$1);
	};
}
%typemap(in) char *, char & { 
  if ($input) {
    $1 = (char *)jenv->GetStringUTFChars((jstring)$input, 0);
  }
}









%typemap(in) TYPE *, TYPE & {
  if (!$input) {
    SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "array null");
    return $null;
  }
  if (JCALL1(GetArrayLength, jenv, $input) == 0) {
    SWIG_JavaThrowException(jenv, SWIG_JavaIndexOutOfBoundsException, "Array must contain at least 1 element");
    return $null;
  }
  $1 = ($1_ltype) JCALL2(Get##JAVATYPE##ArrayElements, jenv, $input, 0); 
}

%typemap(freearg) TYPE *, TYPE & ""

%typemap(argout) TYPE *, TYPE &
{ JCALL3(Release##JAVATYPE##ArrayElements, jenv, $input, (JNITYPE *)$1, 0); }

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
INOUT_TYPEMAP(signed char, jbyte, byte, Byte, "[Ljava/lang/Byte;", jbyteArray); 
INOUT_TYPEMAP(unsigned char, jshort, short, Short, "[Ljava/lang/Short;", jshortArray);     
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


%include "hjp_bridge.irenames"
%include "../hcsp_bridge.h"

