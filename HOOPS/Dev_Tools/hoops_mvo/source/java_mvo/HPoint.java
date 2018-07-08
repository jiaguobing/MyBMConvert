/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 2.0.4
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.techsoft.hoops;

public class HPoint {
  private long swigCPtr;
  protected boolean swigCMemOwn;

  public HPoint(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  public static long getCPtr(HPoint obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        HJMVOJNI.delete_HPoint(swigCPtr);
      }
      swigCPtr = 0;
    }
  }

  protected static long[] cArrayUnwrap(HPoint[] arrayWrapper) {
      long[] cArray = new long[arrayWrapper.length];
      for (int i=0; i<arrayWrapper.length; i++)
        cArray[i] = HPoint.getCPtr(arrayWrapper[i]);
      return cArray;
  }

  protected static HPoint[] cArrayWrap(long[] cArray, boolean cMemoryOwn) {
    HPoint[] arrayWrapper = new HPoint[cArray.length];
    for (int i=0; i<cArray.length; i++)
      arrayWrapper[i] = new HPoint(cArray[i], cMemoryOwn);
    return arrayWrapper;
  }

  public void setX(float value) {
    HJMVOJNI.HPoint_x_set(swigCPtr, this, value);
  }

  public float getX() {
    return HJMVOJNI.HPoint_x_get(swigCPtr, this);
  }

  public void setY(float value) {
    HJMVOJNI.HPoint_y_set(swigCPtr, this, value);
  }

  public float getY() {
    return HJMVOJNI.HPoint_y_get(swigCPtr, this);
  }

  public void setZ(float value) {
    HJMVOJNI.HPoint_z_set(swigCPtr, this, value);
  }

  public float getZ() {
    return HJMVOJNI.HPoint_z_get(swigCPtr, this);
  }

  public HPoint() {
    this(HJMVOJNI.new_HPoint__SWIG_0(), true);
  }

  public HPoint(float X, float Y, float Z) {
    this(HJMVOJNI.new_HPoint__SWIG_1(X, Y, Z), true);
  }

  public HPoint(float X, float Y) {
    this(HJMVOJNI.new_HPoint__SWIG_2(X, Y), true);
  }

  public HPoint(HPoint p) {
    this(HJMVOJNI.new_HPoint__SWIG_3(HPoint.getCPtr(p), p), true);
  }

  public void Set(float X, float Y, float Z) {
    HJMVOJNI.HPoint_Set__SWIG_0(swigCPtr, this, X, Y, Z);
  }

  public void Set(float X, float Y) {
    HJMVOJNI.HPoint_Set__SWIG_1(swigCPtr, this, X, Y);
  }

  public void Set(HPoint p) {
    HJMVOJNI.HPoint_Set__SWIG_2(swigCPtr, this, HPoint.getCPtr(p), p);
  }

  public void Add(float X, float Y, float Z) {
    HJMVOJNI.HPoint_Add__SWIG_0(swigCPtr, this, X, Y, Z);
  }

  public void Add(float X, float Y) {
    HJMVOJNI.HPoint_Add__SWIG_1(swigCPtr, this, X, Y);
  }

  public boolean Equal(HPoint p, float epsi) {
    return HJMVOJNI.HPoint_Equal__SWIG_0(swigCPtr, this, HPoint.getCPtr(p), p, epsi);
  }

  public boolean Equal(HPoint p) {
    return HJMVOJNI.HPoint_Equal__SWIG_1(swigCPtr, this, HPoint.getCPtr(p), p);
  }

}