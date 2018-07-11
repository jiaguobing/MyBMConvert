/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 1.3.40
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.techsoft.hoops;

public class TK_Size extends BBaseOpcodeHandler {
  private long swigCPtr;

  protected TK_Size(long cPtr, boolean cMemoryOwn) {
    super(HJSTREAMJNI.SWIGTK_SizeUpcast(cPtr), cMemoryOwn);
    swigCPtr = cPtr;
  }

  protected static long getCPtr(TK_Size obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        HJSTREAMJNI.delete_TK_Size(swigCPtr);
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
    HJSTREAMJNI.TK_Size_change_ownership(this, swigCPtr, false);
  }

  public void swigTakeOwnership() {
    swigCMemOwn = true;
    HJSTREAMJNI.TK_Size_change_ownership(this, swigCPtr, true);
  }

  protected void setM_value(float value) {
    HJSTREAMJNI.TK_Size_m_value_set(swigCPtr, this, value);
  }

  protected float getM_value() {
    return HJSTREAMJNI.TK_Size_m_value_get(swigCPtr, this);
  }

  protected void setM_units(short value) {
    HJSTREAMJNI.TK_Size_m_units_set(swigCPtr, this, value);
  }

  protected short getM_units() {
    return HJSTREAMJNI.TK_Size_m_units_get(swigCPtr, this);
  }

  public TK_Size(short opcode) {
    this(HJSTREAMJNI.new_TK_Size(opcode), true);
    HJSTREAMJNI.TK_Size_director_connect(this, swigCPtr, swigCMemOwn, true);
  }

  public TK_Status Read(BStreamFileToolkit tk) {
    return TK_Status.swigToEnum((getClass() == TK_Size.class) ? HJSTREAMJNI.TK_Size_Read(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk) : HJSTREAMJNI.TK_Size_ReadSwigExplicitTK_Size(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk));
  }

  public TK_Status Write(BStreamFileToolkit tk) {
    return TK_Status.swigToEnum((getClass() == TK_Size.class) ? HJSTREAMJNI.TK_Size_Write(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk) : HJSTREAMJNI.TK_Size_WriteSwigExplicitTK_Size(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk));
  }

  public TK_Status Clone(BStreamFileToolkit tk, SWIGTYPE_p_p_BBaseOpcodeHandler handler) {
    return TK_Status.swigToEnum((getClass() == TK_Size.class) ? HJSTREAMJNI.TK_Size_Clone(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk, SWIGTYPE_p_p_BBaseOpcodeHandler.getCPtr(handler)) : HJSTREAMJNI.TK_Size_CloneSwigExplicitTK_Size(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk, SWIGTYPE_p_p_BBaseOpcodeHandler.getCPtr(handler)));
  }

  public TK_Status ReadAscii(BStreamFileToolkit tk) {
    return TK_Status.swigToEnum(HJSTREAMJNI.TK_Size_ReadAscii(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk));
  }

  public TK_Status WriteAscii(BStreamFileToolkit tk) {
    return TK_Status.swigToEnum(HJSTREAMJNI.TK_Size_WriteAscii(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk));
  }

  public void SetSize(float value, int units) {
    HJSTREAMJNI.TK_Size_SetSize__SWIG_0(swigCPtr, this, value, units);
  }

  public void SetSize(float value) {
    HJSTREAMJNI.TK_Size_SetSize__SWIG_1(swigCPtr, this, value);
  }

  public float GetSize() {
    return HJSTREAMJNI.TK_Size_GetSize(swigCPtr, this);
  }

  public int GetUnits() {
    return HJSTREAMJNI.TK_Size_GetUnits(swigCPtr, this);
  }

}
