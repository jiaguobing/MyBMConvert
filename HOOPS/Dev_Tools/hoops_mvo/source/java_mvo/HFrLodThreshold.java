/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 2.0.4
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.techsoft.hoops;

public class HFrLodThreshold extends HConstFRSimpType {
  private long swigCPtr;

  public HFrLodThreshold(long cPtr, boolean cMemoryOwn) {
    super(HJMVOJNI.HFrLodThreshold_SWIGUpcast(cPtr), cMemoryOwn);
    swigCPtr = cPtr;
  }

  public static long getCPtr(HFrLodThreshold obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        HJMVOJNI.delete_HFrLodThreshold(swigCPtr);
      }
      swigCPtr = 0;
    }
    super.delete();
  }

  public HFrLodThreshold() {
    this(HJMVOJNI.new_HFrLodThreshold__SWIG_0(), true);
  }

  public HFrLodThreshold(int min, int max) {
    this(HJMVOJNI.new_HFrLodThreshold__SWIG_1(min, max), true);
  }

  public void Set(HConstantFrameRate pCfr, int SimpLevel) {
    HJMVOJNI.HFrLodThreshold_Set(swigCPtr, this, HConstantFrameRate.getCPtr(pCfr), pCfr, SimpLevel);
  }

  public ConstFRType GetFRType() {
    return ConstFRType.swigToEnum(HJMVOJNI.HFrLodThreshold_GetFRType(swigCPtr, this));
  }

  public static HConstFRSimpType Create(int min, int max) {
    long cPtr = HJMVOJNI.HFrLodThreshold_Create(min, max);
    return (cPtr == 0) ? null : new HConstFRSimpType(cPtr, false);
  }

}
