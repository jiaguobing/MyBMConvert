/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 2.0.4
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.techsoft.hoops;

public class HImManager {
  private long swigCPtr;
  protected boolean swigCMemOwn;

  public HImManager(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  public static long getCPtr(HImManager obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        HJMVOJNI.delete_HImManager(swigCPtr);
      }
      swigCPtr = 0;
    }
  }

  public HImManager() {
    this(HJMVOJNI.new_HImManager(), true);
  }

  public void RegisterCallbacks() {
    HJMVOJNI.HImManager_RegisterCallbacks(swigCPtr, this);
  }

  public void RegisterOneCallback(String callback_name, long callback) {
    HJMVOJNI.HImManager_RegisterOneCallback(swigCPtr, this, callback_name, callback);
  }

  public void SetCallback(long segment_key, String callback_type, String callback_name) {
    HJMVOJNI.HImManager_SetCallback__SWIG_0(swigCPtr, this, segment_key, callback_type, callback_name);
  }

  public void SetCallback(String callback_type, String callback_name) {
    HJMVOJNI.HImManager_SetCallback__SWIG_1(swigCPtr, this, callback_type, callback_name);
  }

  public void UnSetOneCallback(long segment_key, String callback_type) {
    HJMVOJNI.HImManager_UnSetOneCallback__SWIG_0(swigCPtr, this, segment_key, callback_type);
  }

  public void UnSetOneCallback(String callback_type) {
    HJMVOJNI.HImManager_UnSetOneCallback__SWIG_1(swigCPtr, this, callback_type);
  }

  public void ShowCallback(String callback_type, StringBuffer callback) {
    HJMVOJNI.HImManager_ShowCallback(swigCPtr, this, callback_type, callback);
  }

  public static HImManager GetCurrentHImManager() {
    long cPtr = HJMVOJNI.HImManager_GetCurrentHImManager();
    return (cPtr == 0) ? null : new HImManager(cPtr, false);
  }

}
