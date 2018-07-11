/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 2.0.4
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.techsoft.hoops;

public class HOpCameraOrbit extends HBaseOperator {
  private long swigCPtr;

  public HOpCameraOrbit(long cPtr, boolean cMemoryOwn) {
    super(HJMVOJNI.HOpCameraOrbit_SWIGUpcast(cPtr), cMemoryOwn);
    swigCPtr = cPtr;
  }

  public static long getCPtr(HOpCameraOrbit obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        HJMVOJNI.delete_HOpCameraOrbit(swigCPtr);
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
    HJMVOJNI.HOpCameraOrbit_change_ownership(this, swigCPtr, false);
  }

  public void swigTakeOwnership() {
    swigCMemOwn = true;
    HJMVOJNI.HOpCameraOrbit_change_ownership(this, swigCPtr, true);
  }

  public HOpCameraOrbit(HBaseView view, int DoRepeat, int DoCapture) {
    this(HJMVOJNI.new_HOpCameraOrbit__SWIG_0(HBaseView.getCPtr(view), view, DoRepeat, DoCapture), true);
    HJMVOJNI.HOpCameraOrbit_director_connect(this, swigCPtr, swigCMemOwn, true);
  }

  public HOpCameraOrbit(HBaseView view, int DoRepeat) {
    this(HJMVOJNI.new_HOpCameraOrbit__SWIG_1(HBaseView.getCPtr(view), view, DoRepeat), true);
    HJMVOJNI.HOpCameraOrbit_director_connect(this, swigCPtr, swigCMemOwn, true);
  }

  public HOpCameraOrbit(HBaseView view) {
    this(HJMVOJNI.new_HOpCameraOrbit__SWIG_2(HBaseView.getCPtr(view), view), true);
    HJMVOJNI.HOpCameraOrbit_director_connect(this, swigCPtr, swigCMemOwn, true);
  }

  public String GetName() {
    return (getClass() == HOpCameraOrbit.class) ? HJMVOJNI.HOpCameraOrbit_GetName(swigCPtr, this) : HJMVOJNI.HOpCameraOrbit_GetNameSwigExplicitHOpCameraOrbit(swigCPtr, this);
  }

  public int OnLButtonDown(HEventInfo hevent) {
    return (getClass() == HOpCameraOrbit.class) ? HJMVOJNI.HOpCameraOrbit_OnLButtonDown(swigCPtr, this, HEventInfo.getCPtr(hevent), hevent) : HJMVOJNI.HOpCameraOrbit_OnLButtonDownSwigExplicitHOpCameraOrbit(swigCPtr, this, HEventInfo.getCPtr(hevent), hevent);
  }

  public int OnLButtonDownAndMove(HEventInfo hevent) {
    return (getClass() == HOpCameraOrbit.class) ? HJMVOJNI.HOpCameraOrbit_OnLButtonDownAndMove(swigCPtr, this, HEventInfo.getCPtr(hevent), hevent) : HJMVOJNI.HOpCameraOrbit_OnLButtonDownAndMoveSwigExplicitHOpCameraOrbit(swigCPtr, this, HEventInfo.getCPtr(hevent), hevent);
  }

  public int OnLButtonUp(HEventInfo hevent) {
    return (getClass() == HOpCameraOrbit.class) ? HJMVOJNI.HOpCameraOrbit_OnLButtonUp(swigCPtr, this, HEventInfo.getCPtr(hevent), hevent) : HJMVOJNI.HOpCameraOrbit_OnLButtonUpSwigExplicitHOpCameraOrbit(swigCPtr, this, HEventInfo.getCPtr(hevent), hevent);
  }

  public void SetLightFollowsCamera(boolean follow) {
    HJMVOJNI.HOpCameraOrbit_SetLightFollowsCamera(swigCPtr, this, follow);
  }

  public boolean GetLightFollowsCamera() {
    return HJMVOJNI.HOpCameraOrbit_GetLightFollowsCamera(swigCPtr, this);
  }

  public void SetSceneBoundingBoxVis(boolean vis) {
    HJMVOJNI.HOpCameraOrbit_SetSceneBoundingBoxVis(swigCPtr, this, vis);
  }

  public boolean GetSceneBoundingBoxVis() {
    return HJMVOJNI.HOpCameraOrbit_GetSceneBoundingBoxVis(swigCPtr, this);
  }

  public HBaseOperator Clone() {
    long cPtr = (getClass() == HOpCameraOrbit.class) ? HJMVOJNI.HOpCameraOrbit_Clone(swigCPtr, this) : HJMVOJNI.HOpCameraOrbit_CloneSwigExplicitHOpCameraOrbit(swigCPtr, this);
    return (cPtr == 0) ? null : new HBaseOperator(cPtr, false);
  }

  public void setM_Angle2(float value) {
    HJMVOJNI.HOpCameraOrbit_m_Angle2_set(swigCPtr, this, value);
  }

  public float getM_Angle2() {
    return HJMVOJNI.HOpCameraOrbit_m_Angle2_get(swigCPtr, this);
  }

  public void setM_Angle1(float value) {
    HJMVOJNI.HOpCameraOrbit_m_Angle1_set(swigCPtr, this, value);
  }

  public float getM_Angle1() {
    return HJMVOJNI.HOpCameraOrbit_m_Angle1_get(swigCPtr, this);
  }

  public void setM_Angle3(float value) {
    HJMVOJNI.HOpCameraOrbit_m_Angle3_set(swigCPtr, this, value);
  }

  public float getM_Angle3() {
    return HJMVOJNI.HOpCameraOrbit_m_Angle3_get(swigCPtr, this);
  }

  public boolean GetSingleClick() {
    return HJMVOJNI.HOpCameraOrbit_GetSingleClick(swigCPtr, this);
  }

  protected void setM_bLightFollowsCamera(boolean value) {
    HJMVOJNI.HOpCameraOrbit_m_bLightFollowsCamera_set(swigCPtr, this, value);
  }

  protected boolean getM_bLightFollowsCamera() {
    return HJMVOJNI.HOpCameraOrbit_m_bLightFollowsCamera_get(swigCPtr, this);
  }

  protected void setM_bSceneBoundingBoxVis(boolean value) {
    HJMVOJNI.HOpCameraOrbit_m_bSceneBoundingBoxVis_set(swigCPtr, this, value);
  }

  protected boolean getM_bSceneBoundingBoxVis() {
    return HJMVOJNI.HOpCameraOrbit_m_bSceneBoundingBoxVis_get(swigCPtr, this);
  }

  protected void setM_bSingleClick(boolean value) {
    HJMVOJNI.HOpCameraOrbit_m_bSingleClick_set(swigCPtr, this, value);
  }

  protected boolean getM_bSingleClick() {
    return HJMVOJNI.HOpCameraOrbit_m_bSingleClick_get(swigCPtr, this);
  }

  protected void setM_ptRealOld(HPoint value) {
    HJMVOJNI.HOpCameraOrbit_m_ptRealOld_set(swigCPtr, this, HPoint.getCPtr(value), value);
  }

  protected HPoint getM_ptRealOld() {
    long cPtr = HJMVOJNI.HOpCameraOrbit_m_ptRealOld_get(swigCPtr, this);
    return (cPtr == 0) ? null : new HPoint(cPtr, false);
  }

}
