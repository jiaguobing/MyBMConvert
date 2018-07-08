/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 2.0.4
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.techsoft.hoops;
import com.techsoft.hoops.*;
public class PMI_String {
  private long swigCPtr;
  protected boolean swigCMemOwn;

  public PMI_String(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  public static long getCPtr(PMI_String obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        HJMVOJNI.delete_PMI_String(swigCPtr);
      }
      swigCPtr = 0;
    }
  }

  protected static long[] cArrayUnwrap(PMI_String[] arrayWrapper) {
      long[] cArray = new long[arrayWrapper.length];
      for (int i=0; i<arrayWrapper.length; i++)
        cArray[i] = PMI_String.getCPtr(arrayWrapper[i]);
      return cArray;
  }

  protected static PMI_String[] cArrayWrap(long[] cArray, boolean cMemoryOwn) {
    PMI_String[] arrayWrapper = new PMI_String[cArray.length];
    for (int i=0; i<cArray.length; i++)
      arrayWrapper[i] = new PMI_String(cArray[i], cMemoryOwn);
    return arrayWrapper;
  }

  public PMI_String() {
    this(HJMVOJNI.new_PMI_String__SWIG_0(), true);
  }

  public PMI_String(String wstring) {
    this(HJMVOJNI.new_PMI_String__SWIG_1(wstring), true);
  }

  public PMI_String(PMI_String that) {
    this(HJMVOJNI.new_PMI_String__SWIG_2(PMI_String.getCPtr(that), that), true);
  }

  public void Set(PMI_String in_that) {
    HJMVOJNI.PMI_String_Set(swigCPtr, this, PMI_String.getCPtr(in_that), in_that);
  }

  public String CStr() {
    return HJMVOJNI.PMI_String_CStr(swigCPtr, this);
  }

  public String WStr() {
    return HJMVOJNI.PMI_String_WStr(swigCPtr, this);
  }

  public long WLength() {
    return HJMVOJNI.PMI_String_WLength(swigCPtr, this);
  }

  public boolean Equals(PMI_String in_that) {
    return HJMVOJNI.PMI_String_Equals(swigCPtr, this, PMI_String.getCPtr(in_that), in_that);
  }

}