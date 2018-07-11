/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 2.0.4
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.techsoft.hoops;

public class HOpCreateRectangle extends HOpConstructRectangle {
  private long swigCPtr;

  public HOpCreateRectangle(long cPtr, boolean cMemoryOwn) {
    super(HJMVOJNI.HOpCreateRectangle_SWIGUpcast(cPtr), cMemoryOwn);
    swigCPtr = cPtr;
  }

  public static long getCPtr(HOpCreateRectangle obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        HJMVOJNI.delete_HOpCreateRectangle(swigCPtr);
      }
      swigCPtr = 0;
    }
    super.delete();
  }

  protected void swigDirectorDisconnect() {
    swigCMemOwn = false;
    delete();
  }

  public void swigReleaseOwnership() {
    swigCMemOwn = false;
    HJMVOJNI.HOpCreateRectangle_change_ownership(this, swigCPtr, false);
  }

  public void swigTakeOwnership() {
    swigCMemOwn = true;
    HJMVOJNI.HOpCreateRectangle_change_ownership(this, swigCPtr, true);
  }

  public HOpCreateRectangle(HBaseView view, int DoRepeat, int DoCapture, boolean UseCenterMarker) {
    this(HJMVOJNI.new_HOpCreateRectangle__SWIG_0(HBaseView.getCPtr(view), view, DoRepeat, DoCapture, UseCenterMarker), true);
    HJMVOJNI.HOpCreateRectangle_director_connect(this, swigCPtr, swigCMemOwn, true);
  }

  public HOpCreateRectangle(HBaseView view, int DoRepeat, int DoCapture) {
    this(HJMVOJNI.new_HOpCreateRectangle__SWIG_1(HBaseView.getCPtr(view), view, DoRepeat, DoCapture), true);
    HJMVOJNI.HOpCreateRectangle_director_connect(this, swigCPtr, swigCMemOwn, true);
  }

  public HOpCreateRectangle(HBaseView view, int DoRepeat) {
    this(HJMVOJNI.new_HOpCreateRectangle__SWIG_2(HBaseView.getCPtr(view), view, DoRepeat), true);
    HJMVOJNI.HOpCreateRectangle_director_connect(this, swigCPtr, swigCMemOwn, true);
  }

  public HOpCreateRectangle(HBaseView view) {
    this(HJMVOJNI.new_HOpCreateRectangle__SWIG_3(HBaseView.getCPtr(view), view), true);
    HJMVOJNI.HOpCreateRectangle_director_connect(this, swigCPtr, swigCMemOwn, true);
  }

  public String GetName() {
    return (getClass() == HOpCreateRectangle.class) ? HJMVOJNI.HOpCreateRectangle_GetName(swigCPtr, this) : HJMVOJNI.HOpCreateRectangle_GetNameSwigExplicitHOpCreateRectangle(swigCPtr, this);
  }

  public HBaseOperator Clone() {
    long cPtr = (getClass() == HOpCreateRectangle.class) ? HJMVOJNI.HOpCreateRectangle_Clone(swigCPtr, this) : HJMVOJNI.HOpCreateRectangle_CloneSwigExplicitHOpCreateRectangle(swigCPtr, this);
    return (cPtr == 0) ? null : new HBaseOperator(cPtr, false);
  }

  public int OnLButtonUp(HEventInfo hevent) {
    return (getClass() == HOpCreateRectangle.class) ? HJMVOJNI.HOpCreateRectangle_OnLButtonUp(swigCPtr, this, HEventInfo.getCPtr(hevent), hevent) : HJMVOJNI.HOpCreateRectangle_OnLButtonUpSwigExplicitHOpCreateRectangle(swigCPtr, this, HEventInfo.getCPtr(hevent), hevent);
  }

}