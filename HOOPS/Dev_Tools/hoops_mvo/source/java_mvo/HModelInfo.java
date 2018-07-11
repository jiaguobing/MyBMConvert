/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 2.0.4
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.techsoft.hoops;

public class HModelInfo {
  private long swigCPtr;
  protected boolean swigCMemOwn;

  public HModelInfo(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  public static long getCPtr(HModelInfo obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        HJMVOJNI.delete_HModelInfo(swigCPtr);
      }
      swigCPtr = 0;
    }
  }

  public HModelInfo(long segment_key) {
    this(HJMVOJNI.new_HModelInfo__SWIG_0(segment_key), true);
  }

  public HModelInfo() {
    this(HJMVOJNI.new_HModelInfo__SWIG_1(), true);
  }

  public void ComputeModelInfo(long segment_key, boolean recurse) {
    HJMVOJNI.HModelInfo_ComputeModelInfo__SWIG_0(swigCPtr, this, segment_key, recurse);
  }

  public void ComputeModelInfo(long segment_key) {
    HJMVOJNI.HModelInfo_ComputeModelInfo__SWIG_1(swigCPtr, this, segment_key);
  }

  public void ComputeModelInfo() {
    HJMVOJNI.HModelInfo_ComputeModelInfo__SWIG_2(swigCPtr, this);
  }

  public long GetTotalFaces() {
    return HJMVOJNI.HModelInfo_GetTotalFaces(swigCPtr, this);
  }

  public long GetTotalVertices() {
    return HJMVOJNI.HModelInfo_GetTotalVertices(swigCPtr, this);
  }

  public long GetTotalTriangles() {
    return HJMVOJNI.HModelInfo_GetTotalTriangles(swigCPtr, this);
  }

  public long GetTotalTristrips() {
    return HJMVOJNI.HModelInfo_GetTotalTristrips(swigCPtr, this);
  }

  public long GetTotalSegments() {
    return HJMVOJNI.HModelInfo_GetTotalSegments(swigCPtr, this);
  }

  public long GetTotalCircle() {
    return HJMVOJNI.HModelInfo_GetTotalCircle(swigCPtr, this);
  }

  public long GetTotalCircularArc() {
    return HJMVOJNI.HModelInfo_GetTotalCircularArc(swigCPtr, this);
  }

  public long GetTotalCircularChord() {
    return HJMVOJNI.HModelInfo_GetTotalCircularChord(swigCPtr, this);
  }

  public long GetTotalCircularWedge() {
    return HJMVOJNI.HModelInfo_GetTotalCircularWedge(swigCPtr, this);
  }

  public long GetTotalEllipse() {
    return HJMVOJNI.HModelInfo_GetTotalEllipse(swigCPtr, this);
  }

  public long GetTotalEllipticalArc() {
    return HJMVOJNI.HModelInfo_GetTotalEllipticalArc(swigCPtr, this);
  }

  public long GetTotalGrid() {
    return HJMVOJNI.HModelInfo_GetTotalGrid(swigCPtr, this);
  }

  public long GetTotalImage() {
    return HJMVOJNI.HModelInfo_GetTotalImage(swigCPtr, this);
  }

  public long GetTotalIncludes() {
    return HJMVOJNI.HModelInfo_GetTotalIncludes(swigCPtr, this);
  }

  public long GetTotalIncluders() {
    return HJMVOJNI.HModelInfo_GetTotalIncluders(swigCPtr, this);
  }

  public long GetTotalLine() {
    return HJMVOJNI.HModelInfo_GetTotalLine(swigCPtr, this);
  }

  public long GetTotalMarker() {
    return HJMVOJNI.HModelInfo_GetTotalMarker(swigCPtr, this);
  }

  public long GetTotalMesh() {
    return HJMVOJNI.HModelInfo_GetTotalMesh(swigCPtr, this);
  }

  public long GetTotalPolygon() {
    return HJMVOJNI.HModelInfo_GetTotalPolygon(swigCPtr, this);
  }

  public long GetTotalPolyline() {
    return HJMVOJNI.HModelInfo_GetTotalPolyline(swigCPtr, this);
  }

  public long GetTotalShell() {
    return HJMVOJNI.HModelInfo_GetTotalShell(swigCPtr, this);
  }

  public long GetTotalStringCursor() {
    return HJMVOJNI.HModelInfo_GetTotalStringCursor(swigCPtr, this);
  }

  public long GetTotalText() {
    return HJMVOJNI.HModelInfo_GetTotalText(swigCPtr, this);
  }

  public long GetTotalMatrices() {
    return HJMVOJNI.HModelInfo_GetTotalMatrices(swigCPtr, this);
  }

  public long GetTotalSpotLight() {
    return HJMVOJNI.HModelInfo_GetTotalSpotLight(swigCPtr, this);
  }

  public long GetTotalDistantLight() {
    return HJMVOJNI.HModelInfo_GetTotalDistantLight(swigCPtr, this);
  }

  public long GetTotalLocalLight() {
    return HJMVOJNI.HModelInfo_GetTotalLocalLight(swigCPtr, this);
  }

  public long GetTotalCuttingPlane() {
    return HJMVOJNI.HModelInfo_GetTotalCuttingPlane(swigCPtr, this);
  }

  public long GetTotalCylinders() {
    return HJMVOJNI.HModelInfo_GetTotalCylinders(swigCPtr, this);
  }

  public long GetTotalPolyCylinders() {
    return HJMVOJNI.HModelInfo_GetTotalPolyCylinders(swigCPtr, this);
  }

  public long GetTotalSpheres() {
    return HJMVOJNI.HModelInfo_GetTotalSpheres(swigCPtr, this);
  }

  public long GetTotalNURBSCurves() {
    return HJMVOJNI.HModelInfo_GetTotalNURBSCurves(swigCPtr, this);
  }

  public long GetTotalNURBSSurfaces() {
    return HJMVOJNI.HModelInfo_GetTotalNURBSSurfaces(swigCPtr, this);
  }

  public long GetTotalReferences() {
    return HJMVOJNI.HModelInfo_GetTotalReferences(swigCPtr, this);
  }

  public long GetTotalInfiniteLines() {
    return HJMVOJNI.HModelInfo_GetTotalInfiniteLines(swigCPtr, this);
  }

  public long GetTotalInfiniteRays() {
    return HJMVOJNI.HModelInfo_GetTotalInfiniteRays(swigCPtr, this);
  }

  public long GetTotalOthers() {
    return HJMVOJNI.HModelInfo_GetTotalOthers(swigCPtr, this);
  }

}
