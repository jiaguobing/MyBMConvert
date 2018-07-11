/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 2.0.4
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.techsoft.hoops;

public class HKeyframeChannelCurve extends HKeyframeChannel {
  private long swigCPtr;

  public HKeyframeChannelCurve(long cPtr, boolean cMemoryOwn) {
    super(HJMVOJNI.HKeyframeChannelCurve_SWIGUpcast(cPtr), cMemoryOwn);
    swigCPtr = cPtr;
  }

  public static long getCPtr(HKeyframeChannelCurve obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        HJMVOJNI.delete_HKeyframeChannelCurve(swigCPtr);
      }
      swigCPtr = 0;
    }
    super.delete();
  }

  public HKeyframeChannelCurve(float x, float y, float z) {
    this(HJMVOJNI.new_HKeyframeChannelCurve__SWIG_0(x, y, z), true);
  }

  public HKeyframeChannelCurve(float x, float y) {
    this(HJMVOJNI.new_HKeyframeChannelCurve__SWIG_1(x, y), true);
  }

  public HKeyframeChannelCurve(float x) {
    this(HJMVOJNI.new_HKeyframeChannelCurve__SWIG_2(x), true);
  }

  public HKeyframeChannelCurve() {
    this(HJMVOJNI.new_HKeyframeChannelCurve__SWIG_3(), true);
  }

  public HKeyframe Clone() {
    long cPtr = HJMVOJNI.HKeyframeChannelCurve_Clone(swigCPtr, this);
    return (cPtr == 0) ? null : new HKeyframe(cPtr, false);
  }

  public void CalculateCurveFactor(SWIGTYPE_p_HBhvTimeline timeline, int keyframe) {
    HJMVOJNI.HKeyframeChannelCurve_CalculateCurveFactor(swigCPtr, this, SWIGTYPE_p_HBhvTimeline.getCPtr(timeline), keyframe);
  }

  public void CalculateHermiteTangents(HPoint p0, HPoint p2, HPoint p3, float a) {
    HJMVOJNI.HKeyframeChannelCurve_CalculateHermiteTangents__SWIG_0(swigCPtr, this, HPoint.getCPtr(p0), p0, HPoint.getCPtr(p2), p2, HPoint.getCPtr(p3), p3, a);
  }

  public void CalculateHermiteTangents(HPoint p0, HPoint p2, HPoint p3) {
    HJMVOJNI.HKeyframeChannelCurve_CalculateHermiteTangents__SWIG_1(swigCPtr, this, HPoint.getCPtr(p0), p0, HPoint.getCPtr(p2), p2, HPoint.getCPtr(p3), p3);
  }

  public HPoint InterpolateHermiteSpline(float fraction, HPoint p1, HPoint p2) {
    return new HPoint(HJMVOJNI.HKeyframeChannelCurve_InterpolateHermiteSpline(swigCPtr, this, fraction, HPoint.getCPtr(p1), p1, HPoint.getCPtr(p2), p2), true);
  }

  public void CalculateCurveLength(HPoint p2) {
    HJMVOJNI.HKeyframeChannelCurve_CalculateCurveLength(swigCPtr, this, HPoint.getCPtr(p2), p2);
  }

  public void DrawTangents(HPoint p2) {
    HJMVOJNI.HKeyframeChannelCurve_DrawTangents(swigCPtr, this, HPoint.getCPtr(p2), p2);
  }

  public void Interpolate(SWIGTYPE_p_p_HKeyframeChannel posarray, int keyframe, float fraction, int length, HPoint res) {
    HJMVOJNI.HKeyframeChannelCurve_Interpolate(swigCPtr, this, SWIGTYPE_p_p_HKeyframeChannel.getCPtr(posarray), keyframe, fraction, length, HPoint.getCPtr(res), res);
  }

  public void Serialize(SWIGTYPE_p_HUtilityXMLTag xmlgen) {
    HJMVOJNI.HKeyframeChannelCurve_Serialize(swigCPtr, this, SWIGTYPE_p_HUtilityXMLTag.getCPtr(xmlgen));
  }

  public void setM_tangent1(HPoint value) {
    HJMVOJNI.HKeyframeChannelCurve_m_tangent1_set(swigCPtr, this, HPoint.getCPtr(value), value);
  }

  public HPoint getM_tangent1() {
    long cPtr = HJMVOJNI.HKeyframeChannelCurve_m_tangent1_get(swigCPtr, this);
    return (cPtr == 0) ? null : new HPoint(cPtr, false);
  }

  public void setM_tangent2(HPoint value) {
    HJMVOJNI.HKeyframeChannelCurve_m_tangent2_set(swigCPtr, this, HPoint.getCPtr(value), value);
  }

  public HPoint getM_tangent2() {
    long cPtr = HJMVOJNI.HKeyframeChannelCurve_m_tangent2_get(swigCPtr, this);
    return (cPtr == 0) ? null : new HPoint(cPtr, false);
  }

  public void setM_factor1(float value) {
    HJMVOJNI.HKeyframeChannelCurve_m_factor1_set(swigCPtr, this, value);
  }

  public float getM_factor1() {
    return HJMVOJNI.HKeyframeChannelCurve_m_factor1_get(swigCPtr, this);
  }

  public void setM_factor2(float value) {
    HJMVOJNI.HKeyframeChannelCurve_m_factor2_set(swigCPtr, this, value);
  }

  public float getM_factor2() {
    return HJMVOJNI.HKeyframeChannelCurve_m_factor2_get(swigCPtr, this);
  }

  public void setItemType(float value) {
    HJMVOJNI.HKeyframeChannelCurve_ItemType_set(swigCPtr, this, value);
  }

  public float getItemType() {
    return HJMVOJNI.HKeyframeChannelCurve_ItemType_get(swigCPtr, this);
  }

  public void setC(float value) {
    HJMVOJNI.HKeyframeChannelCurve_c_set(swigCPtr, this, value);
  }

  public float getC() {
    return HJMVOJNI.HKeyframeChannelCurve_c_get(swigCPtr, this);
  }

  public void setB(float value) {
    HJMVOJNI.HKeyframeChannelCurve_b_set(swigCPtr, this, value);
  }

  public float getB() {
    return HJMVOJNI.HKeyframeChannelCurve_b_get(swigCPtr, this);
  }

  public void setM_curvelength(float value) {
    HJMVOJNI.HKeyframeChannelCurve_m_curvelength_set(swigCPtr, this, value);
  }

  public float getM_curvelength() {
    return HJMVOJNI.HKeyframeChannelCurve_m_curvelength_get(swigCPtr, this);
  }

  public void get_control_polygon(HPoint[] values, int count) {
    HJMVOJNI.HKeyframeChannelCurve_get_control_polygon(swigCPtr, this, HPoint.cArrayUnwrap(values), count);
  }

}
