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


public class HGraphLegend : IDisposable {
  private HandleRef swigCPtr;
  protected bool swigCMemOwn;

  public HGraphLegend(IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new HandleRef(this, cPtr);
  }

  public static HandleRef getCPtr(HGraphLegend obj) {
    return (obj == null) ? new HandleRef(null, IntPtr.Zero) : obj.swigCPtr;
  }
  
  public void owns_memory(bool ownMemory) {
	swigCMemOwn = ownMemory;
  }

  ~HGraphLegend() {
    Dispose();
  }

  public virtual void Dispose() {
    lock(this) {
      if (swigCPtr.Handle != IntPtr.Zero) {
        if (swigCMemOwn) {
          swigCMemOwn = false;
          HCSMVOPINVOKE.delete_HGraphLegend(swigCPtr);
        }
        swigCPtr = new HandleRef(null, IntPtr.Zero);
      }
      GC.SuppressFinalize(this);
    }
  }

  public HGraphLegend() : this(HCSMVOPINVOKE.new_HGraphLegend(), true) {
  }

  public MVO_POINTER_SIZED_INT segment {
	set { HCSMVOPINVOKE.HGraphLegend_segment_set(swigCPtr, value); } 
	get { return HCSMVOPINVOKE.HGraphLegend_segment_get(swigCPtr); } 
  }

  public MVO_POINTER_SIZED_INT title {
	set { HCSMVOPINVOKE.HGraphLegend_title_set(swigCPtr, value); } 
	get { return HCSMVOPINVOKE.HGraphLegend_title_get(swigCPtr); } 
  }

  public SWIGTYPE_p_VArrayT_HGraphLabelNode_p_t entries {
    set {
      HCSMVOPINVOKE.HGraphLegend_entries_set(swigCPtr, SWIGTYPE_p_VArrayT_HGraphLabelNode_p_t.getCPtr(value));
    } 
    get {
      IntPtr cPtr = HCSMVOPINVOKE.HGraphLegend_entries_get(swigCPtr);
      SWIGTYPE_p_VArrayT_HGraphLabelNode_p_t ret = (cPtr == IntPtr.Zero) ? null : new SWIGTYPE_p_VArrayT_HGraphLabelNode_p_t(cPtr, false);
      return ret;
    } 
  }

}
