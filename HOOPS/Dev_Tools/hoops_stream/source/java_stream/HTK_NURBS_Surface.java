/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 1.3.40
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.techsoft.hoops;

public class HTK_NURBS_Surface extends TK_NURBS_Surface {
  private long swigCPtr;

  protected HTK_NURBS_Surface(long cPtr, boolean cMemoryOwn) {
    super(HJSTREAMJNI.SWIGHTK_NURBS_SurfaceUpcast(cPtr), cMemoryOwn);
    swigCPtr = cPtr;
  }

  protected static long getCPtr(HTK_NURBS_Surface obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        HJSTREAMJNI.delete_HTK_NURBS_Surface(swigCPtr);
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
    HJSTREAMJNI.HTK_NURBS_Surface_change_ownership(this, swigCPtr, false);
  }

  public void swigTakeOwnership() {
    swigCMemOwn = true;
    HJSTREAMJNI.HTK_NURBS_Surface_change_ownership(this, swigCPtr, true);
  }

  protected TK_Status interpret_trim(BStreamFileToolkit tk, int arg1) {
    return TK_Status.swigToEnum(HJSTREAMJNI.HTK_NURBS_Surface_interpret_trim(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk, arg1));
  }

  protected TK_Status execute_trim(BStreamFileToolkit tk, HT_NURBS_Trim arg1) {
    return TK_Status.swigToEnum(HJSTREAMJNI.HTK_NURBS_Surface_execute_trim(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk, HT_NURBS_Trim.getCPtr(arg1), arg1));
  }

  public TK_Status Execute(BStreamFileToolkit tk) {
    return TK_Status.swigToEnum((getClass() == HTK_NURBS_Surface.class) ? HJSTREAMJNI.HTK_NURBS_Surface_Execute(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk) : HJSTREAMJNI.HTK_NURBS_Surface_ExecuteSwigExplicitHTK_NURBS_Surface(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk));
  }

  public TK_Status Interpret(BStreamFileToolkit tk, int key, int variant) {
    return TK_Status.swigToEnum((getClass() == HTK_NURBS_Surface.class) ? HJSTREAMJNI.HTK_NURBS_Surface_Interpret__SWIG_0(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk, key, variant) : HJSTREAMJNI.HTK_NURBS_Surface_InterpretSwigExplicitHTK_NURBS_Surface__SWIG_0(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk, key, variant));
  }

  public TK_Status Interpret(BStreamFileToolkit tk, int key) {
    return TK_Status.swigToEnum((getClass() == HTK_NURBS_Surface.class) ? HJSTREAMJNI.HTK_NURBS_Surface_Interpret__SWIG_1(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk, key) : HJSTREAMJNI.HTK_NURBS_Surface_InterpretSwigExplicitHTK_NURBS_Surface__SWIG_1(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk, key));
  }

  public TK_Status Interpret(BStreamFileToolkit tk, int key, String special) {
    return TK_Status.swigToEnum((getClass() == HTK_NURBS_Surface.class) ? HJSTREAMJNI.HTK_NURBS_Surface_Interpret__SWIG_2(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk, key, special) : HJSTREAMJNI.HTK_NURBS_Surface_InterpretSwigExplicitHTK_NURBS_Surface__SWIG_2(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk, key, special));
  }

  public TK_Status Clone(BStreamFileToolkit tk, SWIGTYPE_p_p_BBaseOpcodeHandler handler) {
    return TK_Status.swigToEnum((getClass() == HTK_NURBS_Surface.class) ? HJSTREAMJNI.HTK_NURBS_Surface_Clone(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk, SWIGTYPE_p_p_BBaseOpcodeHandler.getCPtr(handler)) : HJSTREAMJNI.HTK_NURBS_Surface_CloneSwigExplicitHTK_NURBS_Surface(swigCPtr, this, BStreamFileToolkit.getCPtr(tk), tk, SWIGTYPE_p_p_BBaseOpcodeHandler.getCPtr(handler)));
  }

  public HTK_NURBS_Surface() {
    this(HJSTREAMJNI.new_HTK_NURBS_Surface(), true);
    HJSTREAMJNI.HTK_NURBS_Surface_director_connect(this, swigCPtr, swigCMemOwn, true);
  }

}