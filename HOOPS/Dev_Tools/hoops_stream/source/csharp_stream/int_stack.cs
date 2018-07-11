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
using HLONG = System.Int64;
using ID_Key = System.Int64;
#else
using HLONG = System.Int32;
using ID_Key = System.Int32;
#endif


public class int_stack : IDisposable {
  private HandleRef swigCPtr;
  protected bool swigCMemOwn;

  public int_stack(IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new HandleRef(this, cPtr);
  }

  public static HandleRef getCPtr(int_stack obj) {
    return (obj == null) ? new HandleRef(null, IntPtr.Zero) : obj.swigCPtr;
  }

  ~int_stack() {
    Dispose();
  }

  public virtual void Dispose() {
    lock(this) {
      if (swigCPtr.Handle != IntPtr.Zero) {
        if (swigCMemOwn) {
          swigCMemOwn = false;
          HCSSTREAMPINVOKE.delete_int_stack(swigCPtr);
        }
        swigCPtr = new HandleRef(null, IntPtr.Zero);
      }
      GC.SuppressFinalize(this);
    }
  }

  public IntPtr data {
   get {
	 return HCSSTREAMPINVOKE.int_stack_data_get(swigCPtr);
   } 
  }

  public int allocated {
    set {
      HCSSTREAMPINVOKE.int_stack_allocated_set(swigCPtr, value);
    } 
    get {
      int ret = HCSSTREAMPINVOKE.int_stack_allocated_get(swigCPtr);
      return ret;
    } 
  }

  public int used {
    set {
      HCSSTREAMPINVOKE.int_stack_used_set(swigCPtr, value);
    } 
    get {
      int ret = HCSSTREAMPINVOKE.int_stack_used_get(swigCPtr);
      return ret;
    } 
  }

  public int_stack() : this(HCSSTREAMPINVOKE.new_int_stack(), true) {
  }

}