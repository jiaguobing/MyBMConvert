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
#else
using HLONG = System.Int32;
#endif


public class int_stack : IDisposable {
  private HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal int_stack(IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new HandleRef(this, cPtr);
  }

  internal static HandleRef getCPtr(int_stack obj) {
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
          HCSBSTREAMPINVOKE.delete_int_stack(swigCPtr);
        }
        swigCPtr = new HandleRef(null, IntPtr.Zero);
      }
      GC.SuppressFinalize(this);
    }
  }

  public IntPtr data {
   get {
	 return HCSBSTREAMPINVOKE.int_stack_data_get(swigCPtr);
   } 
  }

  public int allocated {
    set {
      HCSBSTREAMPINVOKE.int_stack_allocated_set(swigCPtr, value);
    } 
    get {
      int ret = HCSBSTREAMPINVOKE.int_stack_allocated_get(swigCPtr);
      return ret;
    } 
  }

  public int used {
    set {
      HCSBSTREAMPINVOKE.int_stack_used_set(swigCPtr, value);
    } 
    get {
      int ret = HCSBSTREAMPINVOKE.int_stack_used_get(swigCPtr);
      return ret;
    } 
  }

  public int_stack() : this(HCSBSTREAMPINVOKE.new_int_stack(), true) {
  }

}