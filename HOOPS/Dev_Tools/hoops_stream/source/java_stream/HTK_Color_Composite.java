/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 1.3.40
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.techsoft.hoops;

public class HTK_Color_Composite extends BBaseOpcodeHandler {
  private long swigCPtr;

  protected HTK_Color_Composite(long cPtr, boolean cMemoryOwn) {
    super(HJSTREAMJNI.SWIGHTK_Color_CompositeUpcast(cPtr), cMemoryOwn);
    swigCPtr = cPtr;
  }

  protected static long getCPtr(HTK_Color_Composite obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        HJSTREAMJNI.delete_HTK_Color_Composite(swigCPtr);
      }
      swigCPtr = 0;
    }
    super.delete();
  }

  protected void swigDirectorDisconnect() {
    swigCMemOwn = false;
    delete();
  }

  public void swigReleaseOwnership() {
    swigCMemOwn = false;
    HJSTREAMJNI.HTK_Color_Composite_change_ownership(this, swigCPtr, false);
  }

  public void swigTakeOwnership() {
    swigCMemOwn = true;
    HJSTREAMJNI.HTK_Color_Composite_change_ownership(this, swigCPtr, true);
  }

  protected void setM_parts(SWIGTYPE_p_p_BBaseOpcodeHandler value) {
    HJSTREAMJNI.HTK_Color_Composite_m_parts_set(swigCPtr, this, SWIGTYPE_p_p_BBaseOpcodeHandler.getCPtr(value));
  }

  protected SWIGTYPE_p_p_BBaseOpcodeHandler getM_parts() {
    long cPtr = HJSTREAMJNI.HTK_Color_Composite_m_parts_get(swigCPtr, this);
    return (cPtr == 0) ? null : new SWIGTYPE_p_p_BBaseOpcodeHandler(cPtr, false);
  }

  public HTK_Color_Composite() {
    this(HJSTREAMJNI.new_HTK_Color_Composite(), true);
    HJSTREAMJNI.HTK_Color_Composite_director_connect(this, swigCPtr, swigCMemOwn, true);
  }

  public TK_Status Read(BStreamFileToolkit tk) {
    return TK_Status.swigToEnum((getClass() == HTK_Color_Composite.class) ? HJSTREAMJNI.HTK_Color_Composite_Read(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk) : HJSTREAMJNI.HTK_Color_Composite_ReadSwigExplicitHTK_Color_Composite(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk));
  }

  public TK_Status Write(BStreamFileToolkit tk) {
    return TK_Status.swigToEnum((getClass() == HTK_Color_Composite.class) ? HJSTREAMJNI.HTK_Color_Composite_Write(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk) : HJSTREAMJNI.HTK_Color_Composite_WriteSwigExplicitHTK_Color_Composite(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk));
  }

  public TK_Status Execute(BStreamFileToolkit tk) {
    return TK_Status.swigToEnum((getClass() == HTK_Color_Composite.class) ? HJSTREAMJNI.HTK_Color_Composite_Execute(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk) : HJSTREAMJNI.HTK_Color_Composite_ExecuteSwigExplicitHTK_Color_Composite(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk));
  }

  public TK_Status Interpret(BStreamFileToolkit tk, int key, int variant) {
    return TK_Status.swigToEnum((getClass() == HTK_Color_Composite.class) ? HJSTREAMJNI.HTK_Color_Composite_Interpret__SWIG_0(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk, key, variant) : HJSTREAMJNI.HTK_Color_Composite_InterpretSwigExplicitHTK_Color_Composite__SWIG_0(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk, key, variant));
  }

  public TK_Status Interpret(BStreamFileToolkit tk, int key) {
    return TK_Status.swigToEnum((getClass() == HTK_Color_Composite.class) ? HJSTREAMJNI.HTK_Color_Composite_Interpret__SWIG_1(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk, key) : HJSTREAMJNI.HTK_Color_Composite_InterpretSwigExplicitHTK_Color_Composite__SWIG_1(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk, key));
  }

  public TK_Status Interpret(BStreamFileToolkit tk, int key, String special) {
    return TK_Status.swigToEnum((getClass() == HTK_Color_Composite.class) ? HJSTREAMJNI.HTK_Color_Composite_Interpret__SWIG_2(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk, key, special) : HJSTREAMJNI.HTK_Color_Composite_InterpretSwigExplicitHTK_Color_Composite__SWIG_2(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk, key, special));
  }

  public TK_Status Clone(BStreamFileToolkit tk, SWIGTYPE_p_p_BBaseOpcodeHandler handler) {
    return TK_Status.swigToEnum((getClass() == HTK_Color_Composite.class) ? HJSTREAMJNI.HTK_Color_Composite_Clone(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk, SWIGTYPE_p_p_BBaseOpcodeHandler.getCPtr(handler)) : HJSTREAMJNI.HTK_Color_Composite_CloneSwigExplicitHTK_Color_Composite(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk, SWIGTYPE_p_p_BBaseOpcodeHandler.getCPtr(handler)));
  }

}
