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


public class TK_Named_Style_Def : BBaseOpcodeHandler {
  private HandleRef swigCPtr;

  internal TK_Named_Style_Def(IntPtr cPtr, bool cMemoryOwn) : base(HCSBSTREAMPINVOKE.TK_Named_Style_DefUpcast(cPtr), cMemoryOwn) {
    swigCPtr = new HandleRef(this, cPtr);
  }

  internal static HandleRef getCPtr(TK_Named_Style_Def obj) {
    return (obj == null) ? new HandleRef(null, IntPtr.Zero) : obj.swigCPtr;
  }

  ~TK_Named_Style_Def() {
    Dispose();
  }

  public override void Dispose() {
    lock(this) {
      if (swigCPtr.Handle != IntPtr.Zero) {
        if (swigCMemOwn) {
          swigCMemOwn = false;
          HCSBSTREAMPINVOKE.delete_TK_Named_Style_Def(swigCPtr);
        }
        swigCPtr = new HandleRef(null, IntPtr.Zero);
      }
      GC.SuppressFinalize(this);
      base.Dispose();
    }
  }

  public TK_Named_Style_Def() : this(HCSBSTREAMPINVOKE.new_TK_Named_Style_Def(), true) {
  }

  public override TK_Status Read(BStreamFileToolkit tk) {
    TK_Status ret = (TK_Status)HCSBSTREAMPINVOKE.TK_Named_Style_Def_Read(swigCPtr, BStreamFileToolkit.getCPtr(tk));
    if (HCSBSTREAMPINVOKE.SWIGPendingException.Pending) throw HCSBSTREAMPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public override TK_Status Write(BStreamFileToolkit tk) {
    TK_Status ret = (TK_Status)HCSBSTREAMPINVOKE.TK_Named_Style_Def_Write(swigCPtr, BStreamFileToolkit.getCPtr(tk));
    if (HCSBSTREAMPINVOKE.SWIGPendingException.Pending) throw HCSBSTREAMPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public override TK_Status Clone(BStreamFileToolkit tk, SWIGTYPE_p_p_BBaseOpcodeHandler handler) {
    TK_Status ret = (TK_Status)HCSBSTREAMPINVOKE.TK_Named_Style_Def_Clone(swigCPtr, BStreamFileToolkit.getCPtr(tk), SWIGTYPE_p_p_BBaseOpcodeHandler.getCPtr(handler));
    if (HCSBSTREAMPINVOKE.SWIGPendingException.Pending) throw HCSBSTREAMPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public TK_Status ReadAscii(BStreamFileToolkit tk) {
    TK_Status ret = (TK_Status)HCSBSTREAMPINVOKE.TK_Named_Style_Def_ReadAscii(swigCPtr, BStreamFileToolkit.getCPtr(tk));
    if (HCSBSTREAMPINVOKE.SWIGPendingException.Pending) throw HCSBSTREAMPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public TK_Status WriteAscii(BStreamFileToolkit tk) {
    TK_Status ret = (TK_Status)HCSBSTREAMPINVOKE.TK_Named_Style_Def_WriteAscii(swigCPtr, BStreamFileToolkit.getCPtr(tk));
    if (HCSBSTREAMPINVOKE.SWIGPendingException.Pending) throw HCSBSTREAMPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public override void Reset() {
    HCSBSTREAMPINVOKE.TK_Named_Style_Def_Reset(swigCPtr);
  }

  public void SetName(string name) {
    HCSBSTREAMPINVOKE.TK_Named_Style_Def_SetName__SWIG_0(swigCPtr, name);
  }

  public void SetName(int length) {
    HCSBSTREAMPINVOKE.TK_Named_Style_Def_SetName__SWIG_1(swigCPtr, length);
  }

  public string GetName() {return HCSBSTREAMPINVOKE.TK_Named_Style_Def_GetName__SWIG_0(swigCPtr);}

  public void SetSegment(string segment) {
    HCSBSTREAMPINVOKE.TK_Named_Style_Def_SetSegment__SWIG_0(swigCPtr, segment);
  }

  public void SetSegment(int length) {
    HCSBSTREAMPINVOKE.TK_Named_Style_Def_SetSegment__SWIG_1(swigCPtr, length);
  }

  public string GetSegment() {return HCSBSTREAMPINVOKE.TK_Named_Style_Def_GetSegment__SWIG_0(swigCPtr);}

}