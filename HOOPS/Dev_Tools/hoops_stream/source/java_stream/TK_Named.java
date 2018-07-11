/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 1.3.40
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.techsoft.hoops;

public class TK_Named extends BBaseOpcodeHandler {
  private long swigCPtr;

  protected TK_Named(long cPtr, boolean cMemoryOwn) {
    super(HJSTREAMJNI.SWIGTK_NamedUpcast(cPtr), cMemoryOwn);
    swigCPtr = cPtr;
  }

  protected static long getCPtr(TK_Named obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        HJSTREAMJNI.delete_TK_Named(swigCPtr);
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
    HJSTREAMJNI.TK_Named_change_ownership(this, swigCPtr, false);
  }

  public void swigTakeOwnership() {
    swigCMemOwn = true;
    HJSTREAMJNI.TK_Named_change_ownership(this, swigCPtr, true);
  }

  protected void setM_name_length(int value) {
    HJSTREAMJNI.TK_Named_m_name_length_set(swigCPtr, this, value);
  }

  protected int getM_name_length() {
    return HJSTREAMJNI.TK_Named_m_name_length_get(swigCPtr, this);
  }

  protected void setM_index(int value) {
    HJSTREAMJNI.TK_Named_m_index_set(swigCPtr, this, value);
  }

  protected int getM_index() {
    return HJSTREAMJNI.TK_Named_m_index_get(swigCPtr, this);
  }

  public TK_Named(short opcode) {
    this(HJSTREAMJNI.new_TK_Named(opcode), true);
    HJSTREAMJNI.TK_Named_director_connect(this, swigCPtr, swigCMemOwn, true);
  }

  public TK_Status Read(BStreamFileToolkit tk) {
    return TK_Status.swigToEnum((getClass() == TK_Named.class) ? HJSTREAMJNI.TK_Named_Read(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk) : HJSTREAMJNI.TK_Named_ReadSwigExplicitTK_Named(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk));
  }

  public TK_Status Write(BStreamFileToolkit tk) {
    return TK_Status.swigToEnum((getClass() == TK_Named.class) ? HJSTREAMJNI.TK_Named_Write(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk) : HJSTREAMJNI.TK_Named_WriteSwigExplicitTK_Named(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk));
  }

  public TK_Status Clone(BStreamFileToolkit tk, SWIGTYPE_p_p_BBaseOpcodeHandler handler) {
    return TK_Status.swigToEnum((getClass() == TK_Named.class) ? HJSTREAMJNI.TK_Named_Clone(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk, SWIGTYPE_p_p_BBaseOpcodeHandler.getCPtr(handler)) : HJSTREAMJNI.TK_Named_CloneSwigExplicitTK_Named(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk, SWIGTYPE_p_p_BBaseOpcodeHandler.getCPtr(handler)));
  }

  public TK_Status ReadAscii(BStreamFileToolkit tk) {
    return TK_Status.swigToEnum(HJSTREAMJNI.TK_Named_ReadAscii(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk));
  }

  public TK_Status WriteAscii(BStreamFileToolkit tk) {
    return TK_Status.swigToEnum(HJSTREAMJNI.TK_Named_WriteAscii(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk));
  }

  public void Reset() {
    if (getClass() == TK_Named.class) HJSTREAMJNI.TK_Named_Reset(swigCPtr, this); else HJSTREAMJNI.TK_Named_ResetSwigExplicitTK_Named(swigCPtr, this);
  }

  public void SetName(String name) {
    HJSTREAMJNI.TK_Named_SetName__SWIG_0(swigCPtr, this, name);
  }

  public void SetName(int length) {
    HJSTREAMJNI.TK_Named_SetName__SWIG_1(swigCPtr, this, length);
  }

  public String GetName() {return HJSTREAMJNI.TK_Named_GetName__SWIG_0(swigCPtr, this);}

  public void SetIndex(int i) {
    HJSTREAMJNI.TK_Named_SetIndex(swigCPtr, this, i);
  }

  public int GetIndex() {
    return HJSTREAMJNI.TK_Named_GetIndex(swigCPtr, this);
  }

}
