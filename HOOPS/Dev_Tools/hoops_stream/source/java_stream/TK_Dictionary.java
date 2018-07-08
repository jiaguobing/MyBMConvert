/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 1.3.40
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.techsoft.hoops;

public class TK_Dictionary extends BBaseOpcodeHandler {
  private long swigCPtr;

  protected TK_Dictionary(long cPtr, boolean cMemoryOwn) {
    super(HJSTREAMJNI.SWIGTK_DictionaryUpcast(cPtr), cMemoryOwn);
    swigCPtr = cPtr;
  }

  protected static long getCPtr(TK_Dictionary obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        HJSTREAMJNI.delete_TK_Dictionary(swigCPtr);
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
    HJSTREAMJNI.TK_Dictionary_change_ownership(this, swigCPtr, false);
  }

  public void swigTakeOwnership() {
    swigCMemOwn = true;
    HJSTREAMJNI.TK_Dictionary_change_ownership(this, swigCPtr, true);
  }

  protected void setM_format(short value) {
    HJSTREAMJNI.TK_Dictionary_m_format_set(swigCPtr, this, value);
  }

  protected short getM_format() {
    return HJSTREAMJNI.TK_Dictionary_m_format_get(swigCPtr, this);
  }

  protected void setM_placeholder(int value) {
    HJSTREAMJNI.TK_Dictionary_m_placeholder_set(swigCPtr, this, value);
  }

  protected int getM_placeholder() {
    return HJSTREAMJNI.TK_Dictionary_m_placeholder_get(swigCPtr, this);
  }

  protected void setM_present(short value) {
    HJSTREAMJNI.TK_Dictionary_m_present_set(swigCPtr, this, value);
  }

  protected short getM_present() {
    return HJSTREAMJNI.TK_Dictionary_m_present_get(swigCPtr, this);
  }

  protected void setM_number_of_items(int value) {
    HJSTREAMJNI.TK_Dictionary_m_number_of_items_set(swigCPtr, this, value);
  }

  protected int getM_number_of_items() {
    return HJSTREAMJNI.TK_Dictionary_m_number_of_items_get(swigCPtr, this);
  }

  public TK_Dictionary() {
    this(HJSTREAMJNI.new_TK_Dictionary(), true);
    HJSTREAMJNI.TK_Dictionary_director_connect(this, swigCPtr, swigCMemOwn, true);
  }

  public TK_Status Read(BStreamFileToolkit tk) {
    return TK_Status.swigToEnum((getClass() == TK_Dictionary.class) ? HJSTREAMJNI.TK_Dictionary_Read(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk) : HJSTREAMJNI.TK_Dictionary_ReadSwigExplicitTK_Dictionary(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk));
  }

  public TK_Status Write(BStreamFileToolkit tk) {
    return TK_Status.swigToEnum((getClass() == TK_Dictionary.class) ? HJSTREAMJNI.TK_Dictionary_Write(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk) : HJSTREAMJNI.TK_Dictionary_WriteSwigExplicitTK_Dictionary(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk));
  }

  public TK_Status ReadAscii(BStreamFileToolkit tk) {
    return TK_Status.swigToEnum(HJSTREAMJNI.TK_Dictionary_ReadAscii(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk));
  }

  public TK_Status WriteAscii(BStreamFileToolkit tk) {
    return TK_Status.swigToEnum(HJSTREAMJNI.TK_Dictionary_WriteAscii(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk));
  }

  public TK_Status Execute(BStreamFileToolkit tk) {
    return TK_Status.swigToEnum((getClass() == TK_Dictionary.class) ? HJSTREAMJNI.TK_Dictionary_Execute(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk) : HJSTREAMJNI.TK_Dictionary_ExecuteSwigExplicitTK_Dictionary(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk));
  }

  public TK_Status Interpret(BStreamFileToolkit tk, int key, int variant) {
    return TK_Status.swigToEnum((getClass() == TK_Dictionary.class) ? HJSTREAMJNI.TK_Dictionary_Interpret__SWIG_0(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk, key, variant) : HJSTREAMJNI.TK_Dictionary_InterpretSwigExplicitTK_Dictionary__SWIG_0(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk, key, variant));
  }

  public TK_Status Interpret(BStreamFileToolkit tk, int key) {
    return TK_Status.swigToEnum((getClass() == TK_Dictionary.class) ? HJSTREAMJNI.TK_Dictionary_Interpret__SWIG_1(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk, key) : HJSTREAMJNI.TK_Dictionary_InterpretSwigExplicitTK_Dictionary__SWIG_1(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk, key));
  }

  public TK_Status Interpret(BStreamFileToolkit tk, int key, String special) {
    return TK_Status.swigToEnum((getClass() == TK_Dictionary.class) ? HJSTREAMJNI.TK_Dictionary_Interpret__SWIG_2(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk, key, special) : HJSTREAMJNI.TK_Dictionary_InterpretSwigExplicitTK_Dictionary__SWIG_2(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk, key, special));
  }

  public void Reset() {
    if (getClass() == TK_Dictionary.class) HJSTREAMJNI.TK_Dictionary_Reset(swigCPtr, this); else HJSTREAMJNI.TK_Dictionary_ResetSwigExplicitTK_Dictionary(swigCPtr, this);
  }

}