/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 1.3.40
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.techsoft.hoops;

public class HTK_User_Index_Data extends TK_User_Index_Data {
  private long swigCPtr;

  protected HTK_User_Index_Data(long cPtr, boolean cMemoryOwn) {
    super(HJSTREAMJNI.SWIGHTK_User_Index_DataUpcast(cPtr), cMemoryOwn);
    swigCPtr = cPtr;
  }

  protected static long getCPtr(HTK_User_Index_Data obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        HJSTREAMJNI.delete_HTK_User_Index_Data(swigCPtr);
      }
      swigCPtr = 0;
    }
    super.delete();
  }

  public TK_Status Execute(BStreamFileToolkit tk) {
    return TK_Status.swigToEnum(HJSTREAMJNI.HTK_User_Index_Data_Execute(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk));
  }

  public TK_Status Interpret(BStreamFileToolkit tk, int key, int variant) {
    return TK_Status.swigToEnum(HJSTREAMJNI.HTK_User_Index_Data_Interpret__SWIG_0(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk, key, variant));
  }

  public TK_Status Interpret(BStreamFileToolkit tk, int key) {
    return TK_Status.swigToEnum(HJSTREAMJNI.HTK_User_Index_Data_Interpret__SWIG_1(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk, key));
  }

  public TK_Status Interpret(BStreamFileToolkit tk, int key, String special) {
    return TK_Status.swigToEnum(HJSTREAMJNI.HTK_User_Index_Data_Interpret__SWIG_2(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk, key, special));
  }

  public TK_Status Clone(BStreamFileToolkit tk, SWIGTYPE_p_p_BBaseOpcodeHandler handler) {
    return TK_Status.swigToEnum(HJSTREAMJNI.HTK_User_Index_Data_Clone(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk, SWIGTYPE_p_p_BBaseOpcodeHandler.getCPtr(handler)));
  }

  public HTK_User_Index_Data() {
    this(HJSTREAMJNI.new_HTK_User_Index_Data(), true);
  }

}