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


public class HTK_One_Material : BBaseOpcodeHandler {
  private HandleRef swigCPtr;

  public HTK_One_Material(IntPtr cPtr, bool cMemoryOwn) : base(HCSMVOPINVOKE.HTK_One_MaterialUpcast(cPtr), cMemoryOwn) {
    swigCPtr = new HandleRef(this, cPtr);
  }

  public static HandleRef getCPtr(HTK_One_Material obj) {
    return (obj == null) ? new HandleRef(null, IntPtr.Zero) : obj.swigCPtr;
  }

  ~HTK_One_Material() {
    Dispose();
  }

  public override void Dispose() {
    lock(this) {
      if (swigCPtr.Handle != IntPtr.Zero) {
        if (swigCMemOwn) {
          swigCMemOwn = false;
          HCSMVOPINVOKE.delete_HTK_One_Material(swigCPtr);
        }
        swigCPtr = new HandleRef(null, IntPtr.Zero);
      }
      GC.SuppressFinalize(this);
      base.Dispose();
    }
  }

  public HTK_One_Material() : this(HCSMVOPINVOKE.new_HTK_One_Material__SWIG_0(), true) {
  }

  public HTK_One_Material(HMaterialLibrary hmat) : this(HCSMVOPINVOKE.new_HTK_One_Material__SWIG_1(HMaterialLibrary.getCPtr(hmat)), true) {
  }

  public TK_Status Interpret(BStreamFileToolkit tk, HMaterialLibrary hmat, string name) {
    TK_Status ret = (TK_Status)HCSMVOPINVOKE.HTK_One_Material_Interpret(swigCPtr, BStreamFileToolkit.getCPtr(tk), HMaterialLibrary.getCPtr(hmat), name);
    if (HCSMVOPINVOKE.SWIGPendingException.Pending) throw HCSMVOPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public override TK_Status Execute(BStreamFileToolkit tk) {
    TK_Status ret = (TK_Status)HCSMVOPINVOKE.HTK_One_Material_Execute(swigCPtr, BStreamFileToolkit.getCPtr(tk));
    if (HCSMVOPINVOKE.SWIGPendingException.Pending) throw HCSMVOPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public override TK_Status Read(BStreamFileToolkit tk) {
    TK_Status ret = (TK_Status)HCSMVOPINVOKE.HTK_One_Material_Read(swigCPtr, BStreamFileToolkit.getCPtr(tk));
    if (HCSMVOPINVOKE.SWIGPendingException.Pending) throw HCSMVOPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public override TK_Status Write(BStreamFileToolkit tk) {
    TK_Status ret = (TK_Status)HCSMVOPINVOKE.HTK_One_Material_Write(swigCPtr, BStreamFileToolkit.getCPtr(tk));
    if (HCSMVOPINVOKE.SWIGPendingException.Pending) throw HCSMVOPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public override void Reset() {
    HCSMVOPINVOKE.HTK_One_Material_Reset(swigCPtr);
  }

  public enum Subop_Flags {
    OMF_Environment = 0x0001,
    OMF_Instancee = 0x0002,
    OMF_Tweaks = 0x0004,
    OMF_Embedded = 0x0008,
    OMF_Height_Field = 0x0010
  }

  public enum Tweak_Type {
    OMT_Float1 = 0x1,
    OMT_Float2 = 0x2,
    OMT_Float3 = 0x3,
    OMT_Float4 = 0x4
  }

}
