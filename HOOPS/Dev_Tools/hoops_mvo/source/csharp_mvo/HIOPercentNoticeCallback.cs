/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 1.3.40
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */


using System;
using System.Runtime.InteropServices;

#if _M_X64
using MVO_POINTER_SIZED_INT = System.Int64;
using MVO_POINTER_SIZED_UINT = System.UInt64;
using ID_Key = System.Int64;
#else
using MVO_POINTER_SIZED_INT = System.Int32;
using MVO_POINTER_SIZED_UINT = System.UInt32;
using ID_Key = System.Int32;
#endif


public class HIOPercentNoticeCallback : IDisposable {
  private HandleRef swigCPtr;
  protected bool swigCMemOwn;

  public HIOPercentNoticeCallback(IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new HandleRef(this, cPtr);
  }

  public static HandleRef getCPtr(HIOPercentNoticeCallback obj) {
    return (obj == null) ? new HandleRef(null, IntPtr.Zero) : obj.swigCPtr;
  }
  
  public void owns_memory(bool ownMemory) {
	swigCMemOwn = ownMemory;
  }

  ~HIOPercentNoticeCallback() {
    Dispose();
  }

  public virtual void Dispose() {
    lock(this) {
      if (swigCPtr.Handle != IntPtr.Zero) {
        if (swigCMemOwn) {
          swigCMemOwn = false;
          HCSMVOPINVOKE.delete_HIOPercentNoticeCallback(swigCPtr);
        }
        swigCPtr = new HandleRef(null, IntPtr.Zero);
      }
      GC.SuppressFinalize(this);
    }
  }

  public virtual void Notice(float notice) {
    HCSMVOPINVOKE.HIOPercentNoticeCallback_Notice(swigCPtr, notice);
  }

  public HIOPercentNoticeCallback() : this(HCSMVOPINVOKE.new_HIOPercentNoticeCallback(), true) {
    SwigDirectorConnect();
  }

  private void SwigDirectorConnect() {
    if (SwigDerivedClassHasMethod("Notice", swigMethodTypes0))
      swigDelegate0 = new SwigDelegateHIOPercentNoticeCallback_0(SwigDirectorNotice);
    HCSMVOPINVOKE.HIOPercentNoticeCallback_director_connect(swigCPtr, swigDelegate0);
  }

  private bool SwigDerivedClassHasMethod(string methodName, Type[] methodTypes) {
    System.Reflection.MethodInfo methodInfo = this.GetType().GetMethod(methodName, System.Reflection.BindingFlags.Public | System.Reflection.BindingFlags.NonPublic | System.Reflection.BindingFlags.Instance, null, methodTypes, null);
    bool hasDerivedMethod = methodInfo.DeclaringType.IsSubclassOf(typeof(HIOPercentNoticeCallback));
    return hasDerivedMethod;
  }

  private void SwigDirectorNotice(float notice) {
    Notice(notice);
  }

  public delegate void SwigDelegateHIOPercentNoticeCallback_0(float notice);

  private SwigDelegateHIOPercentNoticeCallback_0 swigDelegate0;

  private static Type[] swigMethodTypes0 = new Type[] { typeof(float) };
}