/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 1.3.40
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.techsoft.hoops;

public class half_edge_array {
  private long swigCPtr;
  protected boolean swigCMemOwn;

  protected half_edge_array(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  protected static long getCPtr(half_edge_array obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        HJSTREAMJNI.delete_half_edge_array(swigCPtr);
      }
      swigCPtr = 0;
    }
  }

  public void get_visitations(int[] values, int count) {
    HJSTREAMJNI.half_edge_array_get_visitations(swigCPtr, this, values, count);
  }

  public void setEdges(half_edge value) {
    HJSTREAMJNI.half_edge_array_edges_set(swigCPtr, this, half_edge.getCPtr(value), value);
  }

  public half_edge getEdges() {
    long cPtr = HJSTREAMJNI.half_edge_array_edges_get(swigCPtr, this);
    return (cPtr == 0) ? null : new half_edge(cPtr, false);
  }

  public void setAllocated(int value) {
    HJSTREAMJNI.half_edge_array_allocated_set(swigCPtr, this, value);
  }

  public int getAllocated() {
    return HJSTREAMJNI.half_edge_array_allocated_get(swigCPtr, this);
  }

  public void setUsed(int value) {
    HJSTREAMJNI.half_edge_array_used_set(swigCPtr, this, value);
  }

  public int getUsed() {
    return HJSTREAMJNI.half_edge_array_used_get(swigCPtr, this);
  }

  public void setVisitations_used(int value) {
    HJSTREAMJNI.half_edge_array_visitations_used_set(swigCPtr, this, value);
  }

  public int getVisitations_used() {
    return HJSTREAMJNI.half_edge_array_visitations_used_get(swigCPtr, this);
  }

  public half_edge_array() {
    this(HJSTREAMJNI.new_half_edge_array(), true);
  }

}
