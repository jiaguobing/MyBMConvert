/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 1.3.40
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.techsoft.hoops;

public class loop_table {
  private long swigCPtr;
  protected boolean swigCMemOwn;

  protected loop_table(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  protected static long getCPtr(loop_table obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        HJSTREAMJNI.delete_loop_table(swigCPtr);
      }
      swigCPtr = 0;
    }
  }

  public void get_loops(int[] values, int count) {
    HJSTREAMJNI.loop_table_get_loops(swigCPtr, this, values, count);
  }

  public void get_loops_edges(int[] values, int count) {
    HJSTREAMJNI.loop_table_get_loops_edges(swigCPtr, this, values, count);
  }

  public void get_P(int[] values, int count) {
    HJSTREAMJNI.loop_table_get_P(swigCPtr, this, values, count);
  }

  public void get_N(int[] values, int count) {
    HJSTREAMJNI.loop_table_get_N(swigCPtr, this, values, count);
  }

  public void setLoops_allocated(int value) {
    HJSTREAMJNI.loop_table_loops_allocated_set(swigCPtr, this, value);
  }

  public int getLoops_allocated() {
    return HJSTREAMJNI.loop_table_loops_allocated_get(swigCPtr, this);
  }

  public void setLoops_used(int value) {
    HJSTREAMJNI.loop_table_loops_used_set(swigCPtr, this, value);
  }

  public int getLoops_used() {
    return HJSTREAMJNI.loop_table_loops_used_get(swigCPtr, this);
  }

  public void setLoops_edges_allocated(int value) {
    HJSTREAMJNI.loop_table_loops_edges_allocated_set(swigCPtr, this, value);
  }

  public int getLoops_edges_allocated() {
    return HJSTREAMJNI.loop_table_loops_edges_allocated_get(swigCPtr, this);
  }

  public void setNp_allocated(int value) {
    HJSTREAMJNI.loop_table_np_allocated_set(swigCPtr, this, value);
  }

  public int getNp_allocated() {
    return HJSTREAMJNI.loop_table_np_allocated_get(swigCPtr, this);
  }

  public loop_table() {
    this(HJSTREAMJNI.new_loop_table(), true);
  }

}