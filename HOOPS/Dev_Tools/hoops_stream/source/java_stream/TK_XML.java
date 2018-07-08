/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 1.3.40
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.techsoft.hoops;

public class TK_XML extends BBaseOpcodeHandler {
  private long swigCPtr;

  protected TK_XML(long cPtr, boolean cMemoryOwn) {
    super(HJSTREAMJNI.SWIGTK_XMLUpcast(cPtr), cMemoryOwn);
    swigCPtr = cPtr;
  }

  protected static long getCPtr(TK_XML obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        HJSTREAMJNI.delete_TK_XML(swigCPtr);
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
    HJSTREAMJNI.TK_XML_change_ownership(this, swigCPtr, false);
  }

  public void swigTakeOwnership() {
    swigCMemOwn = true;
    HJSTREAMJNI.TK_XML_change_ownership(this, swigCPtr, true);
  }

  protected void setM_size(int value) {
    HJSTREAMJNI.TK_XML_m_size_set(swigCPtr, this, value);
  }

  protected int getM_size() {
    return HJSTREAMJNI.TK_XML_m_size_get(swigCPtr, this);
  }

  public TK_XML() {
    this(HJSTREAMJNI.new_TK_XML(), true);
    HJSTREAMJNI.TK_XML_director_connect(this, swigCPtr, swigCMemOwn, true);
  }

  public TK_Status Read(BStreamFileToolkit tk) {
    return TK_Status.swigToEnum((getClass() == TK_XML.class) ? HJSTREAMJNI.TK_XML_Read(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk) : HJSTREAMJNI.TK_XML_ReadSwigExplicitTK_XML(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk));
  }

  public TK_Status Write(BStreamFileToolkit tk) {
    return TK_Status.swigToEnum((getClass() == TK_XML.class) ? HJSTREAMJNI.TK_XML_Write(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk) : HJSTREAMJNI.TK_XML_WriteSwigExplicitTK_XML(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk));
  }

  public TK_Status Clone(BStreamFileToolkit tk, SWIGTYPE_p_p_BBaseOpcodeHandler handler) {
    return TK_Status.swigToEnum((getClass() == TK_XML.class) ? HJSTREAMJNI.TK_XML_Clone(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk, SWIGTYPE_p_p_BBaseOpcodeHandler.getCPtr(handler)) : HJSTREAMJNI.TK_XML_CloneSwigExplicitTK_XML(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk, SWIGTYPE_p_p_BBaseOpcodeHandler.getCPtr(handler)));
  }

  public TK_Status ReadAscii(BStreamFileToolkit tk) {
    return TK_Status.swigToEnum(HJSTREAMJNI.TK_XML_ReadAscii(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk));
  }

  public TK_Status WriteAscii(BStreamFileToolkit tk) {
    return TK_Status.swigToEnum(HJSTREAMJNI.TK_XML_WriteAscii(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk));
  }

  public TK_Status Execute(BStreamFileToolkit tk) {
    return TK_Status.swigToEnum((getClass() == TK_XML.class) ? HJSTREAMJNI.TK_XML_Execute(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk) : HJSTREAMJNI.TK_XML_ExecuteSwigExplicitTK_XML(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk));
  }

  public void Reset() {
    if (getClass() == TK_XML.class) HJSTREAMJNI.TK_XML_Reset(swigCPtr, this); else HJSTREAMJNI.TK_XML_ResetSwigExplicitTK_XML(swigCPtr, this);
  }

  public void SetXML(int size, String data) {
    HJSTREAMJNI.TK_XML_SetXML__SWIG_0(swigCPtr, this, size, data);
  }

  public void SetXML(int size) {
    HJSTREAMJNI.TK_XML_SetXML__SWIG_1(swigCPtr, this, size);
  }

  public void AppendXML(int size, String data) {
    HJSTREAMJNI.TK_XML_AppendXML__SWIG_0(swigCPtr, this, size, data);
  }

  public void AppendXML(int size) {
    HJSTREAMJNI.TK_XML_AppendXML__SWIG_1(swigCPtr, this, size);
  }

  public String GetXML() {return HJSTREAMJNI.TK_XML_GetXML__SWIG_0(swigCPtr, this);}

  public int GetSize() {
    return HJSTREAMJNI.TK_XML_GetSize(swigCPtr, this);
  }

}