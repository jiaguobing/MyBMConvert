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


public class HIOConnector : IDisposable {
  private HandleRef swigCPtr;
  protected bool swigCMemOwn;

  public HIOConnector(IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new HandleRef(this, cPtr);
  }

  public static HandleRef getCPtr(HIOConnector obj) {
    return (obj == null) ? new HandleRef(null, IntPtr.Zero) : obj.swigCPtr;
  }
  
  public void owns_memory(bool ownMemory) {
	swigCMemOwn = ownMemory;
  }

  ~HIOConnector() {
    Dispose();
  }

  public virtual void Dispose() {
    lock(this) {
      if (swigCPtr.Handle != IntPtr.Zero) {
        if (swigCMemOwn) {
          swigCMemOwn = false;
          HCSMVOPINVOKE.delete_HIOConnector(swigCPtr);
        }
        swigCPtr = new HandleRef(null, IntPtr.Zero);
      }
      GC.SuppressFinalize(this);
    }
  }

  public HIOConnector() : this(HCSMVOPINVOKE.new_HIOConnector(), true) {
  }

  public virtual MVO_POINTER_SIZED_INT GetHoopsEntity(IntPtr pKernelEntity) {return HCSMVOPINVOKE.HIOConnector_GetHoopsEntity__SWIG_0(swigCPtr, pKernelEntity);}

  public virtual MVO_POINTER_SIZED_INT GetHoopsEntity(IntPtr pKernelEntity, MVO_POINTER_SIZED_INT modelKey) {return HCSMVOPINVOKE.HIOConnector_GetHoopsEntity__SWIG_1(swigCPtr, pKernelEntity, modelKey);}

  public virtual IntPtr GetKernelEntity(MVO_POINTER_SIZED_INT key) {return new IntPtr((int)HCSMVOPINVOKE.HIOConnector_GetKernelEntity__SWIG_0(swigCPtr, key));}

  public virtual IntPtr GetKernelEntity(MVO_POINTER_SIZED_INT key, MVO_POINTER_SIZED_INT modelKey) {return new IntPtr((int)HCSMVOPINVOKE.HIOConnector_GetKernelEntity__SWIG_1(swigCPtr, key, modelKey));}

  public virtual bool GetHoopsEntities(IntPtr pKernelEntity, SWIGTYPE_p_vlist_s ret_HoopsKeysList) {
    bool ret = HCSMVOPINVOKE.HIOConnector_GetHoopsEntities__SWIG_0(swigCPtr, pKernelEntity, SWIGTYPE_p_vlist_s.getCPtr(ret_HoopsKeysList));
    return ret;
  }

  public virtual bool GetHoopsEntities(IntPtr pKernelEntity, SWIGTYPE_p_vlist_s ret_HoopsKeysList, MVO_POINTER_SIZED_INT modelKey) {
    bool ret = HCSMVOPINVOKE.HIOConnector_GetHoopsEntities__SWIG_1(swigCPtr, pKernelEntity, SWIGTYPE_p_vlist_s.getCPtr(ret_HoopsKeysList), modelKey);
    return ret;
  }

  public virtual bool GetKernelEntities(MVO_POINTER_SIZED_INT key, SWIGTYPE_p_vlist_s ret_KernelEntitiesList) {
    bool ret = HCSMVOPINVOKE.HIOConnector_GetKernelEntities__SWIG_0(swigCPtr, key, SWIGTYPE_p_vlist_s.getCPtr(ret_KernelEntitiesList));
    return ret;
  }

  public virtual bool GetKernelEntities(MVO_POINTER_SIZED_INT key, SWIGTYPE_p_vlist_s ret_KernelEntitiesList, MVO_POINTER_SIZED_INT modelKey) {
    bool ret = HCSMVOPINVOKE.HIOConnector_GetKernelEntities__SWIG_1(swigCPtr, key, SWIGTYPE_p_vlist_s.getCPtr(ret_KernelEntitiesList), modelKey);
    return ret;
  }

  public virtual void AddConnection(MVO_POINTER_SIZED_INT key, IntPtr pKernelEntity) {
    HCSMVOPINVOKE.HIOConnector_AddConnection__SWIG_0(swigCPtr, key, pKernelEntity);
  }

  public virtual void AddConnection(MVO_POINTER_SIZED_INT key, IntPtr pKernelEntity, MVO_POINTER_SIZED_INT modelKey) {
    HCSMVOPINVOKE.HIOConnector_AddConnection__SWIG_1(swigCPtr, key, pKernelEntity, modelKey);
  }

  public virtual void OptimizeTree(string segname, string option_string) {
    HCSMVOPINVOKE.HIOConnector_OptimizeTree(swigCPtr, segname, option_string);
  }

}
