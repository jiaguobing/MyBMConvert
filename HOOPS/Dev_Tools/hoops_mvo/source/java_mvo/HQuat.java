/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 2.0.4
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.techsoft.hoops;

public class HQuat {
  private long swigCPtr;
  protected boolean swigCMemOwn;

  public HQuat(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  public static long getCPtr(HQuat obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        HJMVOJNI.delete_HQuat(swigCPtr);
      }
      swigCPtr = 0;
    }
  }

  public void setX(float value) {
    HJMVOJNI.HQuat_x_set(swigCPtr, this, value);
  }

  public float getX() {
    return HJMVOJNI.HQuat_x_get(swigCPtr, this);
  }

  public void setY(float value) {
    HJMVOJNI.HQuat_y_set(swigCPtr, this, value);
  }

  public float getY() {
    return HJMVOJNI.HQuat_y_get(swigCPtr, this);
  }

  public void setZ(float value) {
    HJMVOJNI.HQuat_z_set(swigCPtr, this, value);
  }

  public float getZ() {
    return HJMVOJNI.HQuat_z_get(swigCPtr, this);
  }

  public void setW(float value) {
    HJMVOJNI.HQuat_w_set(swigCPtr, this, value);
  }

  public float getW() {
    return HJMVOJNI.HQuat_w_get(swigCPtr, this);
  }

  public HQuat Normalize() {
    return new HQuat(HJMVOJNI.HQuat_Normalize(swigCPtr, this), false);
  }

  public HQuat() {
    this(HJMVOJNI.new_HQuat__SWIG_0(), true);
  }

  public HQuat(float X, float Y, float Z, float W) {
    this(HJMVOJNI.new_HQuat__SWIG_1(X, Y, Z, W), true);
  }

  public HQuat(HQuat p) {
    this(HJMVOJNI.new_HQuat__SWIG_2(HQuat.getCPtr(p), p), true);
  }

  public void Set(float X, float Y, float Z, float W) {
    HJMVOJNI.HQuat_Set(swigCPtr, this, X, Y, Z, W);
  }

  public static HQuat Qlog(HQuat q) {
    return new HQuat(HJMVOJNI.HQuat_Qlog(HQuat.getCPtr(q), q), true);
  }

  public void ToMatrix(float[] matrix) {
    HJMVOJNI.HQuat_ToMatrix(swigCPtr, this, matrix);
  }

  public static HQuat Qexp(HQuat q) {
    return new HQuat(HJMVOJNI.HQuat_Qexp(HQuat.getCPtr(q), q), true);
  }

  public static HQuat Qlerp(HQuat q1, HQuat q2, float fraction) {
    return new HQuat(HJMVOJNI.HQuat_Qlerp(HQuat.getCPtr(q1), q1, HQuat.getCPtr(q2), q2, fraction), true);
  }

  public static HQuat Qslerp(HQuat q1, HQuat q2, float fraction) {
    return new HQuat(HJMVOJNI.HQuat_Qslerp(HQuat.getCPtr(q1), q1, HQuat.getCPtr(q2), q2, fraction), true);
  }

  public static HQuat QslerpNoInvert(HQuat q1, HQuat q2, float fraction) {
    return new HQuat(HJMVOJNI.HQuat_QslerpNoInvert(HQuat.getCPtr(q1), q1, HQuat.getCPtr(q2), q2, fraction), true);
  }

  public static HQuat QslerpNoInvertExtraSpins(HQuat q1, HQuat q2, float fraction, int ExtraSpins) {
    return new HQuat(HJMVOJNI.HQuat_QslerpNoInvertExtraSpins(HQuat.getCPtr(q1), q1, HQuat.getCPtr(q2), q2, fraction, ExtraSpins), true);
  }

  public static HQuat Qsquad(HQuat q1, HQuat q2, HQuat a, HQuat b, float fraction) {
    return new HQuat(HJMVOJNI.HQuat_Qsquad(HQuat.getCPtr(q1), q1, HQuat.getCPtr(q2), q2, HQuat.getCPtr(a), a, HQuat.getCPtr(b), b, fraction), true);
  }

  public static HQuat Qspline(HQuat qnm1, HQuat qn, HQuat qnp1) {
    return new HQuat(HJMVOJNI.HQuat_Qspline(HQuat.getCPtr(qnm1), qnm1, HQuat.getCPtr(qn), qn, HQuat.getCPtr(qnp1), qnp1), true);
  }

  public static HQuat MatrixToQuaternion(float[] matrix) {
    return new HQuat(HJMVOJNI.HQuat_MatrixToQuaternion(matrix), true);
  }

}
