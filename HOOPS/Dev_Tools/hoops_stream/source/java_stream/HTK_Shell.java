/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 1.3.40
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.techsoft.hoops;

public class HTK_Shell extends TK_Shell {
  private long swigCPtr;

  protected HTK_Shell(long cPtr, boolean cMemoryOwn) {
    super(HJSTREAMJNI.SWIGHTK_ShellUpcast(cPtr), cMemoryOwn);
    swigCPtr = cPtr;
  }

  protected static long getCPtr(HTK_Shell obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        HJSTREAMJNI.delete_HTK_Shell(swigCPtr);
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
    HJSTREAMJNI.HTK_Shell_change_ownership(this, swigCPtr, false);
  }

  public void swigTakeOwnership() {
    swigCMemOwn = true;
    HJSTREAMJNI.HTK_Shell_change_ownership(this, swigCPtr, true);
  }

  protected TK_Status Usefulness(BStreamFileToolkit tk, int key, int lod, float[] score) {
    return TK_Status.swigToEnum((getClass() == HTK_Shell.class) ? HJSTREAMJNI.HTK_Shell_Usefulness(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk, key, lod, score) : HJSTREAMJNI.HTK_Shell_UsefulnessSwigExplicitHTK_Shell(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk, key, lod, score));
  }

  protected TK_Status ProcessRevisits(BStreamFileToolkit tk, int lod_in, boolean[] do_continue) {
    return TK_Status.swigToEnum(HJSTREAMJNI.HTK_Shell_ProcessRevisits(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk, lod_in, do_continue));
  }

  protected TK_Status LabelFakeEdges() {
    return TK_Status.swigToEnum((getClass() == HTK_Shell.class) ? HJSTREAMJNI.HTK_Shell_LabelFakeEdges(swigCPtr, this) : HJSTREAMJNI.HTK_Shell_LabelFakeEdgesSwigExplicitHTK_Shell(swigCPtr, this));
  }

  public TK_Status Execute(BStreamFileToolkit tk) {
    return TK_Status.swigToEnum((getClass() == HTK_Shell.class) ? HJSTREAMJNI.HTK_Shell_Execute(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk) : HJSTREAMJNI.HTK_Shell_ExecuteSwigExplicitHTK_Shell(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk));
  }

  public TK_Status Interpret(BStreamFileToolkit tk, int key, int variant) {
    return TK_Status.swigToEnum((getClass() == HTK_Shell.class) ? HJSTREAMJNI.HTK_Shell_Interpret__SWIG_0(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk, key, variant) : HJSTREAMJNI.HTK_Shell_InterpretSwigExplicitHTK_Shell__SWIG_0(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk, key, variant));
  }

  public TK_Status Interpret(BStreamFileToolkit tk, int key) {
    return TK_Status.swigToEnum((getClass() == HTK_Shell.class) ? HJSTREAMJNI.HTK_Shell_Interpret__SWIG_1(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk, key) : HJSTREAMJNI.HTK_Shell_InterpretSwigExplicitHTK_Shell__SWIG_1(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk, key));
  }

  public TK_Status Interpret(BStreamFileToolkit tk, int key, String special) {
    return TK_Status.swigToEnum((getClass() == HTK_Shell.class) ? HJSTREAMJNI.HTK_Shell_Interpret__SWIG_2(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk, key, special) : HJSTREAMJNI.HTK_Shell_InterpretSwigExplicitHTK_Shell__SWIG_2(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk, key, special));
  }

  public TK_Status Clone(BStreamFileToolkit tk, SWIGTYPE_p_p_BBaseOpcodeHandler handler) {
    return TK_Status.swigToEnum((getClass() == HTK_Shell.class) ? HJSTREAMJNI.HTK_Shell_Clone(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk, SWIGTYPE_p_p_BBaseOpcodeHandler.getCPtr(handler)) : HJSTREAMJNI.HTK_Shell_CloneSwigExplicitHTK_Shell(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk, SWIGTYPE_p_p_BBaseOpcodeHandler.getCPtr(handler)));
  }

  public boolean Match_Instance(BStreamFileToolkit tk, Recorded_Instance instance) {
    return (getClass() == HTK_Shell.class) ? HJSTREAMJNI.HTK_Shell_Match_Instance(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk, Recorded_Instance.getCPtr(instance), instance) : HJSTREAMJNI.HTK_Shell_Match_InstanceSwigExplicitHTK_Shell(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk, Recorded_Instance.getCPtr(instance), instance);
  }

  public HTK_Shell() {
    this(HJSTREAMJNI.new_HTK_Shell(), true);
    HJSTREAMJNI.HTK_Shell_director_connect(this, swigCPtr, swigCMemOwn, true);
  }

}