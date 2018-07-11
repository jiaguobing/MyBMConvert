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


public class HQuat : IDisposable {
  private HandleRef swigCPtr;
  protected bool swigCMemOwn;

  public HQuat(IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new HandleRef(this, cPtr);
  }

  public static HandleRef getCPtr(HQuat obj) {
    return (obj == null) ? new HandleRef(null, IntPtr.Zero) : obj.swigCPtr;
  }
  
  public void owns_memory(bool ownMemory) {
	swigCMemOwn = ownMemory;
  }

  ~HQuat() {
    Dispose();
  }

  public virtual void Dispose() {
    lock(this) {
      if (swigCPtr.Handle != IntPtr.Zero) {
        if (swigCMemOwn) {
          swigCMemOwn = false;
          HCSMVOPINVOKE.delete_HQuat(swigCPtr);
        }
        swigCPtr = new HandleRef(null, IntPtr.Zero);
      }
      GC.SuppressFinalize(this);
    }
  }

  public float x {
    set {
      HCSMVOPINVOKE.HQuat_x_set(swigCPtr, value);
    } 
    get {
      float ret = HCSMVOPINVOKE.HQuat_x_get(swigCPtr);
      return ret;
    } 
  }

  public float y {
    set {
      HCSMVOPINVOKE.HQuat_y_set(swigCPtr, value);
    } 
    get {
      float ret = HCSMVOPINVOKE.HQuat_y_get(swigCPtr);
      return ret;
    } 
  }

  public float z {
    set {
      HCSMVOPINVOKE.HQuat_z_set(swigCPtr, value);
    } 
    get {
      float ret = HCSMVOPINVOKE.HQuat_z_get(swigCPtr);
      return ret;
    } 
  }

  public float w {
    set {
      HCSMVOPINVOKE.HQuat_w_set(swigCPtr, value);
    } 
    get {
      float ret = HCSMVOPINVOKE.HQuat_w_get(swigCPtr);
      return ret;
    } 
  }

  public HQuat Normalize() {
    HQuat ret = new HQuat(HCSMVOPINVOKE.HQuat_Normalize(swigCPtr), false);
    return ret;
  }

  public HQuat() : this(HCSMVOPINVOKE.new_HQuat__SWIG_0(), true) {
  }

  public HQuat(float X, float Y, float Z, float W) : this(HCSMVOPINVOKE.new_HQuat__SWIG_1(X, Y, Z, W), true) {
  }

  public HQuat(HQuat p) : this(HCSMVOPINVOKE.new_HQuat__SWIG_2(HQuat.getCPtr(p)), true) {
  }

  public void Set(float X, float Y, float Z, float W) {
    HCSMVOPINVOKE.HQuat_Set(swigCPtr, X, Y, Z, W);
  }

  public static HQuat Qlog(HQuat q) {
    HQuat ret = new HQuat(HCSMVOPINVOKE.HQuat_Qlog(HQuat.getCPtr(q)), true);
    if (HCSMVOPINVOKE.SWIGPendingException.Pending) throw HCSMVOPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public void ToMatrix(SWIGTYPE_p_float matrix) {
    HCSMVOPINVOKE.HQuat_ToMatrix(swigCPtr, SWIGTYPE_p_float.getCPtr(matrix));
  }

  public static HQuat Qexp(HQuat q) {
    HQuat ret = new HQuat(HCSMVOPINVOKE.HQuat_Qexp(HQuat.getCPtr(q)), true);
    if (HCSMVOPINVOKE.SWIGPendingException.Pending) throw HCSMVOPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public static HQuat Qlerp(HQuat q1, HQuat q2, float fraction) {
    HQuat ret = new HQuat(HCSMVOPINVOKE.HQuat_Qlerp(HQuat.getCPtr(q1), HQuat.getCPtr(q2), fraction), true);
    if (HCSMVOPINVOKE.SWIGPendingException.Pending) throw HCSMVOPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public static HQuat Qslerp(HQuat q1, HQuat q2, float fraction) {
    HQuat ret = new HQuat(HCSMVOPINVOKE.HQuat_Qslerp(HQuat.getCPtr(q1), HQuat.getCPtr(q2), fraction), true);
    if (HCSMVOPINVOKE.SWIGPendingException.Pending) throw HCSMVOPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public static HQuat QslerpNoInvert(HQuat q1, HQuat q2, float fraction) {
    HQuat ret = new HQuat(HCSMVOPINVOKE.HQuat_QslerpNoInvert(HQuat.getCPtr(q1), HQuat.getCPtr(q2), fraction), true);
    if (HCSMVOPINVOKE.SWIGPendingException.Pending) throw HCSMVOPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public static HQuat QslerpNoInvertExtraSpins(HQuat q1, HQuat q2, float fraction, int ExtraSpins) {
    HQuat ret = new HQuat(HCSMVOPINVOKE.HQuat_QslerpNoInvertExtraSpins(HQuat.getCPtr(q1), HQuat.getCPtr(q2), fraction, ExtraSpins), true);
    if (HCSMVOPINVOKE.SWIGPendingException.Pending) throw HCSMVOPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public static HQuat Qsquad(HQuat q1, HQuat q2, HQuat a, HQuat b, float fraction) {
    HQuat ret = new HQuat(HCSMVOPINVOKE.HQuat_Qsquad(HQuat.getCPtr(q1), HQuat.getCPtr(q2), HQuat.getCPtr(a), HQuat.getCPtr(b), fraction), true);
    if (HCSMVOPINVOKE.SWIGPendingException.Pending) throw HCSMVOPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public static HQuat Qspline(HQuat qnm1, HQuat qn, HQuat qnp1) {
    HQuat ret = new HQuat(HCSMVOPINVOKE.HQuat_Qspline(HQuat.getCPtr(qnm1), HQuat.getCPtr(qn), HQuat.getCPtr(qnp1)), true);
    if (HCSMVOPINVOKE.SWIGPendingException.Pending) throw HCSMVOPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public static HQuat MatrixToQuaternion(float[] matrix) {
    HQuat ret = new HQuat(HCSMVOPINVOKE.HQuat_MatrixToQuaternion(matrix), true);
    return ret;
  }

}
