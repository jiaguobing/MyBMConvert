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


public class HImUtility : IDisposable {
  private HandleRef swigCPtr;
  protected bool swigCMemOwn;

  public HImUtility(IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new HandleRef(this, cPtr);
  }

  public static HandleRef getCPtr(HImUtility obj) {
    return (obj == null) ? new HandleRef(null, IntPtr.Zero) : obj.swigCPtr;
  }
  
  public void owns_memory(bool ownMemory) {
	swigCMemOwn = ownMemory;
  }

  ~HImUtility() {
    Dispose();
  }

  public virtual void Dispose() {
    lock(this) {
      if (swigCPtr.Handle != IntPtr.Zero) {
        if (swigCMemOwn) {
          swigCMemOwn = false;
          HCSMVOPINVOKE.delete_HImUtility(swigCPtr);
        }
        swigCPtr = new HandleRef(null, IntPtr.Zero);
      }
      GC.SuppressFinalize(this);
    }
  }

  public static void RegisterCallbacks() {
    HCSMVOPINVOKE.HImUtility_RegisterCallbacks();
  }

  public static void set_clip_rectangle(SWIGTYPE_p_HIC_Rendition nr, SWIGTYPE_p_HIC_Segment_Info si) {
    HCSMVOPINVOKE.HImUtility_set_clip_rectangle(SWIGTYPE_p_HIC_Rendition.getCPtr(nr), SWIGTYPE_p_HIC_Segment_Info.getCPtr(si));
  }

  public static void suppress_3d_geometry(SWIGTYPE_p_HIC_Rendition rendition, SWIGTYPE_p_HIC_Geometry geo3) {
    HCSMVOPINVOKE.HImUtility_suppress_3d_geometry(SWIGTYPE_p_HIC_Rendition.getCPtr(rendition), SWIGTYPE_p_HIC_Geometry.getCPtr(geo3));
  }

  public static void draw_gradiated_window_background(SWIGTYPE_p_HIC_Rendition rendition, SWIGTYPE_p_HIC_Int_Rectangle extent) {
    HCSMVOPINVOKE.HImUtility_draw_gradiated_window_background(SWIGTYPE_p_HIC_Rendition.getCPtr(rendition), SWIGTYPE_p_HIC_Int_Rectangle.getCPtr(extent));
  }

  public static void draw_annotation_lines(SWIGTYPE_p_HIC_Rendition nr, SWIGTYPE_p_HIC_Polyline poly) {
    HCSMVOPINVOKE.HImUtility_draw_annotation_lines(SWIGTYPE_p_HIC_Rendition.getCPtr(nr), SWIGTYPE_p_HIC_Polyline.getCPtr(poly));
  }

  public static void drawTextInfront(SWIGTYPE_p_HIC_Rendition rendition, SWIGTYPE_p_HIC_Text_Info text_info) {
    HCSMVOPINVOKE.HImUtility_drawTextInfront(SWIGTYPE_p_HIC_Rendition.getCPtr(rendition), SWIGTYPE_p_HIC_Text_Info.getCPtr(text_info));
  }

}
