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


public class HPixelRGBA : HPixelRGB {
  private HandleRef swigCPtr;

  public HPixelRGBA(IntPtr cPtr, bool cMemoryOwn) : base(HCSMVOPINVOKE.HPixelRGBAUpcast(cPtr), cMemoryOwn) {
    swigCPtr = new HandleRef(this, cPtr);
  }

  public static HandleRef getCPtr(HPixelRGBA obj) {
    return (obj == null) ? new HandleRef(null, IntPtr.Zero) : obj.swigCPtr;
  }

  ~HPixelRGBA() {
    Dispose();
  }

  public override void Dispose() {
    lock(this) {
      if (swigCPtr.Handle != IntPtr.Zero) {
        if (swigCMemOwn) {
          swigCMemOwn = false;
          HCSMVOPINVOKE.delete_HPixelRGBA(swigCPtr);
        }
        swigCPtr = new HandleRef(null, IntPtr.Zero);
      }
      GC.SuppressFinalize(this);
      base.Dispose();
    }
  }

  public HPixelRGBA(byte R, byte G, byte B, byte A) : this(HCSMVOPINVOKE.new_HPixelRGBA__SWIG_0(R, G, B, A), true) {
  }

  public HPixelRGBA(byte R, byte G, byte B) : this(HCSMVOPINVOKE.new_HPixelRGBA__SWIG_1(R, G, B), true) {
  }

  public HPixelRGBA(byte R, byte G) : this(HCSMVOPINVOKE.new_HPixelRGBA__SWIG_2(R, G), true) {
  }

  public HPixelRGBA(byte R) : this(HCSMVOPINVOKE.new_HPixelRGBA__SWIG_3(R), true) {
  }

  public HPixelRGBA() : this(HCSMVOPINVOKE.new_HPixelRGBA__SWIG_4(), true) {
  }

  public HPixelRGBA(HPixelRGBA p) : this(HCSMVOPINVOKE.new_HPixelRGBA__SWIG_5(HPixelRGBA.getCPtr(p)), true) {
    if (HCSMVOPINVOKE.SWIGPendingException.Pending) throw HCSMVOPINVOKE.SWIGPendingException.Retrieve();
  }

  public void Set(byte R, byte G, byte B, byte A) {
    HCSMVOPINVOKE.HPixelRGBA_Set__SWIG_0(swigCPtr, R, G, B, A);
  }

  public new void Set(byte R, byte G, byte B) {
    HCSMVOPINVOKE.HPixelRGBA_Set__SWIG_1(swigCPtr, R, G, B);
  }

  public void Setf(float R, float G, float B, float A) {
    HCSMVOPINVOKE.HPixelRGBA_Setf__SWIG_0(swigCPtr, R, G, B, A);
  }

  public new void Setf(float R, float G, float B) {
    HCSMVOPINVOKE.HPixelRGBA_Setf__SWIG_1(swigCPtr, R, G, B);
  }

  public byte a() {
    byte ret = HCSMVOPINVOKE.HPixelRGBA_a__SWIG_0(swigCPtr);
    return ret;
  }

  public void a(byte A) {
    HCSMVOPINVOKE.HPixelRGBA_a__SWIG_1(swigCPtr, A);
  }

}