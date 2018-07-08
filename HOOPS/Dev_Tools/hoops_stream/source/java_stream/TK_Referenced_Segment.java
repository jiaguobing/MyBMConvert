/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 1.3.40
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.techsoft.hoops;

public class TK_Referenced_Segment extends BBaseOpcodeHandler {
  private long swigCPtr;

  protected TK_Referenced_Segment(long cPtr, boolean cMemoryOwn) {
    super(HJSTREAMJNI.SWIGTK_Referenced_SegmentUpcast(cPtr), cMemoryOwn);
    swigCPtr = cPtr;
  }

  protected static long getCPtr(TK_Referenced_Segment obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        HJSTREAMJNI.delete_TK_Referenced_Segment(swigCPtr);
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
    HJSTREAMJNI.TK_Referenced_Segment_change_ownership(this, swigCPtr, false);
  }

  public void swigTakeOwnership() {
    swigCMemOwn = true;
    HJSTREAMJNI.TK_Referenced_Segment_change_ownership(this, swigCPtr, true);
  }

  protected void setM_length(int value) {
    HJSTREAMJNI.TK_Referenced_Segment_m_length_set(swigCPtr, this, value);
  }

  protected int getM_length() {
    return HJSTREAMJNI.TK_Referenced_Segment_m_length_get(swigCPtr, this);
  }

  protected void setM_allocated(int value) {
    HJSTREAMJNI.TK_Referenced_Segment_m_allocated_set(swigCPtr, this, value);
  }

  protected int getM_allocated() {
    return HJSTREAMJNI.TK_Referenced_Segment_m_allocated_get(swigCPtr, this);
  }

  protected void setM_cond_length(int value) {
    HJSTREAMJNI.TK_Referenced_Segment_m_cond_length_set(swigCPtr, this, value);
  }

  protected int getM_cond_length() {
    return HJSTREAMJNI.TK_Referenced_Segment_m_cond_length_get(swigCPtr, this);
  }

  protected void setM_cond_allocated(int value) {
    HJSTREAMJNI.TK_Referenced_Segment_m_cond_allocated_set(swigCPtr, this, value);
  }

  protected int getM_cond_allocated() {
    return HJSTREAMJNI.TK_Referenced_Segment_m_cond_allocated_get(swigCPtr, this);
  }

  protected void setM_key(int value) {
    HJSTREAMJNI.TK_Referenced_Segment_m_key_set(swigCPtr, this, value);
  }

  protected int getM_key() {
    return HJSTREAMJNI.TK_Referenced_Segment_m_key_get(swigCPtr, this);
  }

  protected void setM_renumbered_key(int value) {
    HJSTREAMJNI.TK_Referenced_Segment_m_renumbered_key_set(swigCPtr, this, value);
  }

  protected int getM_renumbered_key() {
    return HJSTREAMJNI.TK_Referenced_Segment_m_renumbered_key_get(swigCPtr, this);
  }

  protected void setM_renumbered_scope(short value) {
    HJSTREAMJNI.TK_Referenced_Segment_m_renumbered_scope_set(swigCPtr, this, value);
  }

  protected short getM_renumbered_scope() {
    return HJSTREAMJNI.TK_Referenced_Segment_m_renumbered_scope_get(swigCPtr, this);
  }

  protected void setM_referee(BBaseOpcodeHandler value) {
    HJSTREAMJNI.TK_Referenced_Segment_m_referee_set(swigCPtr, this, BBaseOpcodeHandler.getCPtr(value), value);
  }

  protected BBaseOpcodeHandler getM_referee() {
    long cPtr = HJSTREAMJNI.TK_Referenced_Segment_m_referee_get(swigCPtr, this);
    return (cPtr == 0) ? null : new BBaseOpcodeHandler(cPtr, false);
  }

  protected void setM_follow(boolean value) {
    HJSTREAMJNI.TK_Referenced_Segment_m_follow_set(swigCPtr, this, value);
  }

  protected boolean getM_follow() {
    return HJSTREAMJNI.TK_Referenced_Segment_m_follow_get(swigCPtr, this);
  }

  protected void setM_filter(Lock_Masks value) {
    HJSTREAMJNI.TK_Referenced_Segment_m_filter_set(swigCPtr, this, Lock_Masks.getCPtr(value), value);
  }

  protected Lock_Masks getM_filter() {
    long cPtr = HJSTREAMJNI.TK_Referenced_Segment_m_filter_get(swigCPtr, this);
    return (cPtr == 0) ? null : new Lock_Masks(cPtr, false);
  }

  protected void setM_referee_has_priority(boolean value) {
    HJSTREAMJNI.TK_Referenced_Segment_m_referee_has_priority_set(swigCPtr, this, value);
  }

  protected boolean getM_referee_has_priority() {
    return HJSTREAMJNI.TK_Referenced_Segment_m_referee_has_priority_get(swigCPtr, this);
  }

  protected void setM_referee_priority(int value) {
    HJSTREAMJNI.TK_Referenced_Segment_m_referee_priority_set(swigCPtr, this, value);
  }

  protected int getM_referee_priority() {
    return HJSTREAMJNI.TK_Referenced_Segment_m_referee_priority_get(swigCPtr, this);
  }

  protected void set_segment(String segment) {
    HJSTREAMJNI.TK_Referenced_Segment_set_segment__SWIG_0(swigCPtr, this, segment);
  }

  protected void set_segment(int length) {
    HJSTREAMJNI.TK_Referenced_Segment_set_segment__SWIG_1(swigCPtr, this, length);
  }

  public TK_Referenced_Segment(short opcode) {
    this(HJSTREAMJNI.new_TK_Referenced_Segment(opcode), true);
    HJSTREAMJNI.TK_Referenced_Segment_director_connect(this, swigCPtr, swigCMemOwn, true);
  }

  public TK_Status Read(BStreamFileToolkit tk) {
    return TK_Status.swigToEnum((getClass() == TK_Referenced_Segment.class) ? HJSTREAMJNI.TK_Referenced_Segment_Read(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk) : HJSTREAMJNI.TK_Referenced_Segment_ReadSwigExplicitTK_Referenced_Segment(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk));
  }

  public TK_Status Write(BStreamFileToolkit tk) {
    return TK_Status.swigToEnum((getClass() == TK_Referenced_Segment.class) ? HJSTREAMJNI.TK_Referenced_Segment_Write(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk) : HJSTREAMJNI.TK_Referenced_Segment_WriteSwigExplicitTK_Referenced_Segment(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk));
  }

  public TK_Status Clone(BStreamFileToolkit tk, SWIGTYPE_p_p_BBaseOpcodeHandler handler) {
    return TK_Status.swigToEnum((getClass() == TK_Referenced_Segment.class) ? HJSTREAMJNI.TK_Referenced_Segment_Clone(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk, SWIGTYPE_p_p_BBaseOpcodeHandler.getCPtr(handler)) : HJSTREAMJNI.TK_Referenced_Segment_CloneSwigExplicitTK_Referenced_Segment(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk, SWIGTYPE_p_p_BBaseOpcodeHandler.getCPtr(handler)));
  }

  public TK_Status ReadAscii(BStreamFileToolkit tk) {
    return TK_Status.swigToEnum(HJSTREAMJNI.TK_Referenced_Segment_ReadAscii(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk));
  }

  public TK_Status WriteAscii(BStreamFileToolkit tk) {
    return TK_Status.swigToEnum(HJSTREAMJNI.TK_Referenced_Segment_WriteAscii(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk));
  }

  public void Reset() {
    if (getClass() == TK_Referenced_Segment.class) HJSTREAMJNI.TK_Referenced_Segment_Reset(swigCPtr, this); else HJSTREAMJNI.TK_Referenced_Segment_ResetSwigExplicitTK_Referenced_Segment(swigCPtr, this);
  }

  public void SetSegment(String segment) {
    HJSTREAMJNI.TK_Referenced_Segment_SetSegment__SWIG_0(swigCPtr, this, segment);
  }

  public void SetSegment(int length) {
    HJSTREAMJNI.TK_Referenced_Segment_SetSegment__SWIG_1(swigCPtr, this, length);
  }

  public String GetSegment() {return HJSTREAMJNI.TK_Referenced_Segment_GetSegment__SWIG_0(swigCPtr, this);}

  public void SetCondition(String condition) {
    HJSTREAMJNI.TK_Referenced_Segment_SetCondition__SWIG_0(swigCPtr, this, condition);
  }

  public void SetCondition(int length) {
    HJSTREAMJNI.TK_Referenced_Segment_SetCondition__SWIG_1(swigCPtr, this, length);
  }

  public String GetCondition() {return HJSTREAMJNI.TK_Referenced_Segment_GetCondition__SWIG_0(swigCPtr, this);}

  public void SetFollow(boolean f) {
    HJSTREAMJNI.TK_Referenced_Segment_SetFollow(swigCPtr, this, f);
  }

  public boolean GetFollow() {
    return HJSTREAMJNI.TK_Referenced_Segment_GetFollow(swigCPtr, this);
  }

}