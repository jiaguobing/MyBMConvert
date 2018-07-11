/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 1.3.40
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.techsoft.hoops;

public class HTK_Named_Style_Defs extends BBaseOpcodeHandler {
  private long swigCPtr;

  protected HTK_Named_Style_Defs(long cPtr, boolean cMemoryOwn) {
    super(HJSTREAMJNI.SWIGHTK_Named_Style_DefsUpcast(cPtr), cMemoryOwn);
    swigCPtr = cPtr;
  }

  protected static long getCPtr(HTK_Named_Style_Defs obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        HJSTREAMJNI.delete_HTK_Named_Style_Defs(swigCPtr);
      }
      swigCPtr = 0;
    }
    super.delete();
  }

  public HTK_Named_Style_Defs() {
    this(HJSTREAMJNI.new_HTK_Named_Style_Defs(), true);
  }

  public TK_Status Read(BStreamFileToolkit tk) {
    return TK_Status.swigToEnum(HJSTREAMJNI.HTK_Named_Style_Defs_Read(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk));
  }

  public TK_Status Write(BStreamFileToolkit tk) {
    return TK_Status.swigToEnum(HJSTREAMJNI.HTK_Named_Style_Defs_Write(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk));
  }

  public TK_Status Execute(BStreamFileToolkit tk) {
    return TK_Status.swigToEnum(HJSTREAMJNI.HTK_Named_Style_Defs_Execute(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk));
  }

  public TK_Status Interpret(BStreamFileToolkit tk, int key, int variant) {
    return TK_Status.swigToEnum(HJSTREAMJNI.HTK_Named_Style_Defs_Interpret__SWIG_0(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk, key, variant));
  }

  public TK_Status Interpret(BStreamFileToolkit tk, int key) {
    return TK_Status.swigToEnum(HJSTREAMJNI.HTK_Named_Style_Defs_Interpret__SWIG_1(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk, key));
  }

  public TK_Status Interpret(BStreamFileToolkit tk, int key, String special) {
    return TK_Status.swigToEnum(HJSTREAMJNI.HTK_Named_Style_Defs_Interpret__SWIG_2(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk, key, special));
  }

  public TK_Status Clone(BStreamFileToolkit tk, SWIGTYPE_p_p_BBaseOpcodeHandler handler) {
    return TK_Status.swigToEnum(HJSTREAMJNI.HTK_Named_Style_Defs_Clone(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk, SWIGTYPE_p_p_BBaseOpcodeHandler.getCPtr(handler)));
  }

  public void SetRoot(int root) {
    HJSTREAMJNI.HTK_Named_Style_Defs_SetRoot(swigCPtr, this, root);
  }

  public int GetRoot() {return HJSTREAMJNI.HTK_Named_Style_Defs_GetRoot(swigCPtr, this);}

}
