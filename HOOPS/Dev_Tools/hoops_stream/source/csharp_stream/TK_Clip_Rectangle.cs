/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 1.3.40
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */


using System;
using System.Runtime.InteropServices;

#if _M_X64
using HLONG = System.Int64;
using ID_Key = System.Int64;
#else
using HLONG = System.Int32;
using ID_Key = System.Int32;
#endif


public class TK_Clip_Rectangle : BBaseOpcodeHandler {
  private HandleRef swigCPtr;

  public TK_Clip_Rectangle(IntPtr cPtr, bool cMemoryOwn) : base(HCSSTREAMPINVOKE.TK_Clip_RectangleUpcast(cPtr), cMemoryOwn) {
    swigCPtr = new HandleRef(this, cPtr);
  }

  public static HandleRef getCPtr(TK_Clip_Rectangle obj) {
    return (obj == null) ? new HandleRef(null, IntPtr.Zero) : obj.swigCPtr;
  }

  ~TK_Clip_Rectangle() {
    Dispose();
  }

  public override void Dispose() {
    lock(this) {
      if (swigCPtr.Handle != IntPtr.Zero) {
        if (swigCMemOwn) {
          swigCMemOwn = false;
          HCSSTREAMPINVOKE.delete_TK_Clip_Rectangle(swigCPtr);
        }
        swigCPtr = new HandleRef(null, IntPtr.Zero);
      }
      GC.SuppressFinalize(this);
      base.Dispose();
    }
  }

  public TK_Clip_Rectangle() : this(HCSSTREAMPINVOKE.new_TK_Clip_Rectangle(), true) {
    SwigDirectorConnect();
  }

  public override TK_Status Read(BStreamFileToolkit tk) {
    TK_Status ret = (TK_Status)((this.GetType() == typeof(TK_Clip_Rectangle)) ? HCSSTREAMPINVOKE.TK_Clip_Rectangle_Read(swigCPtr, BStreamFileToolkit.getCPtr(tk)) : HCSSTREAMPINVOKE.TK_Clip_Rectangle_ReadSwigExplicitTK_Clip_Rectangle(swigCPtr, BStreamFileToolkit.getCPtr(tk)));
    if (HCSSTREAMPINVOKE.SWIGPendingException.Pending) throw HCSSTREAMPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public override TK_Status Write(BStreamFileToolkit tk) {
    TK_Status ret = (TK_Status)((this.GetType() == typeof(TK_Clip_Rectangle)) ? HCSSTREAMPINVOKE.TK_Clip_Rectangle_Write(swigCPtr, BStreamFileToolkit.getCPtr(tk)) : HCSSTREAMPINVOKE.TK_Clip_Rectangle_WriteSwigExplicitTK_Clip_Rectangle(swigCPtr, BStreamFileToolkit.getCPtr(tk)));
    if (HCSSTREAMPINVOKE.SWIGPendingException.Pending) throw HCSSTREAMPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public override TK_Status Clone(BStreamFileToolkit tk, SWIGTYPE_p_p_BBaseOpcodeHandler handler) {
    TK_Status ret = (TK_Status)((this.GetType() == typeof(TK_Clip_Rectangle)) ? HCSSTREAMPINVOKE.TK_Clip_Rectangle_Clone(swigCPtr, BStreamFileToolkit.getCPtr(tk), SWIGTYPE_p_p_BBaseOpcodeHandler.getCPtr(handler)) : HCSSTREAMPINVOKE.TK_Clip_Rectangle_CloneSwigExplicitTK_Clip_Rectangle(swigCPtr, BStreamFileToolkit.getCPtr(tk), SWIGTYPE_p_p_BBaseOpcodeHandler.getCPtr(handler)));
    if (HCSSTREAMPINVOKE.SWIGPendingException.Pending) throw HCSSTREAMPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public TK_Status ReadAscii(BStreamFileToolkit tk) {
    TK_Status ret = (TK_Status)HCSSTREAMPINVOKE.TK_Clip_Rectangle_ReadAscii(swigCPtr, BStreamFileToolkit.getCPtr(tk));
    if (HCSSTREAMPINVOKE.SWIGPendingException.Pending) throw HCSSTREAMPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public TK_Status WriteAscii(BStreamFileToolkit tk) {
    TK_Status ret = (TK_Status)HCSSTREAMPINVOKE.TK_Clip_Rectangle_WriteAscii(swigCPtr, BStreamFileToolkit.getCPtr(tk));
    if (HCSSTREAMPINVOKE.SWIGPendingException.Pending) throw HCSSTREAMPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public override void Reset() {
    if (this.GetType() == typeof(TK_Clip_Rectangle)) HCSSTREAMPINVOKE.TK_Clip_Rectangle_Reset(swigCPtr); else HCSSTREAMPINVOKE.TK_Clip_Rectangle_ResetSwigExplicitTK_Clip_Rectangle(swigCPtr);
  }

  public void SetRectangle(float left, float right, float bottom, float top) {
    HCSSTREAMPINVOKE.TK_Clip_Rectangle_SetRectangle__SWIG_0(swigCPtr, left, right, bottom, top);
  }

  public void SetRectangle(float[] rect) {
    HCSSTREAMPINVOKE.TK_Clip_Rectangle_SetRectangle__SWIG_1(swigCPtr, rect);
  }

  public IntPtr GetRectangle() {return HCSSTREAMPINVOKE.TK_Clip_Rectangle_GetRectangle(swigCPtr);}

  public void SetOptions(int o) {
    HCSSTREAMPINVOKE.TK_Clip_Rectangle_SetOptions(swigCPtr, o);
  }

  public int GetOptions() {
    int ret = HCSSTREAMPINVOKE.TK_Clip_Rectangle_GetOptions(swigCPtr);
    return ret;
  }

  private void SwigDirectorConnect() {
    if (SwigDerivedClassHasMethod("Read", swigMethodTypes0))
      swigDelegate0 = new SwigDelegateTK_Clip_Rectangle_0(SwigDirectorRead);
    if (SwigDerivedClassHasMethod("Write", swigMethodTypes1))
      swigDelegate1 = new SwigDelegateTK_Clip_Rectangle_1(SwigDirectorWrite);
    if (SwigDerivedClassHasMethod("Execute", swigMethodTypes2))
      swigDelegate2 = new SwigDelegateTK_Clip_Rectangle_2(SwigDirectorExecute);
    if (SwigDerivedClassHasMethod("Interpret", swigMethodTypes3))
      swigDelegate3 = new SwigDelegateTK_Clip_Rectangle_3(SwigDirectorInterpret__SWIG_0);
    if (SwigDerivedClassHasMethod("Interpret", swigMethodTypes4))
      swigDelegate4 = new SwigDelegateTK_Clip_Rectangle_4(SwigDirectorInterpret__SWIG_1);
    if (SwigDerivedClassHasMethod("Interpret_special", swigMethodTypes5))
      swigDelegate5 = new SwigDelegateTK_Clip_Rectangle_5(SwigDirectorInterpret_special);
    if (SwigDerivedClassHasMethod("Reset", swigMethodTypes6))
      swigDelegate6 = new SwigDelegateTK_Clip_Rectangle_6(SwigDirectorReset);
    if (SwigDerivedClassHasMethod("Match_Instance", swigMethodTypes7))
      swigDelegate7 = new SwigDelegateTK_Clip_Rectangle_7(SwigDirectorMatch_Instance);
    if (SwigDerivedClassHasMethod("Clone", swigMethodTypes8))
      swigDelegate8 = new SwigDelegateTK_Clip_Rectangle_8(SwigDirectorClone);
    if (SwigDerivedClassHasMethod("NeedsContext", swigMethodTypes9))
      swigDelegate9 = new SwigDelegateTK_Clip_Rectangle_9(SwigDirectorNeedsContext);
    HCSSTREAMPINVOKE.TK_Clip_Rectangle_director_connect(swigCPtr, swigDelegate0, swigDelegate1, swigDelegate2, swigDelegate3, swigDelegate4, swigDelegate5, swigDelegate6, swigDelegate7, swigDelegate8, swigDelegate9);
  }

  private bool SwigDerivedClassHasMethod(string methodName, Type[] methodTypes) {
    System.Reflection.MethodInfo methodInfo = this.GetType().GetMethod(methodName, System.Reflection.BindingFlags.Public | System.Reflection.BindingFlags.NonPublic | System.Reflection.BindingFlags.Instance, null, methodTypes, null);
    bool hasDerivedMethod = methodInfo.DeclaringType.IsSubclassOf(typeof(TK_Clip_Rectangle));
    return hasDerivedMethod;
  }

  private int SwigDirectorRead(IntPtr tk) {
    return (int)Read(new BStreamFileToolkit(tk, false));
  }

  private int SwigDirectorWrite(IntPtr tk) {
    return (int)Write(new BStreamFileToolkit(tk, false));
  }

  private int SwigDirectorExecute(IntPtr tk) {
    return (int)Execute(new BStreamFileToolkit(tk, false));
  }

  private int SwigDirectorInterpret__SWIG_0(IntPtr tk, ID_Key key, int variant) {
    return (int)Interpret(new BStreamFileToolkit(tk, false), key, variant);
  }

  private int SwigDirectorInterpret__SWIG_1(IntPtr tk, ID_Key key) {
    return (int)Interpret(new BStreamFileToolkit(tk, false), key);
  }

  private int SwigDirectorInterpret_special(IntPtr tk, ID_Key key, string special) {
    return (int)Interpret_special(new BStreamFileToolkit(tk, false), key, special);
  }

  private void SwigDirectorReset() {
    Reset();
  }

  private bool SwigDirectorMatch_Instance(IntPtr tk, IntPtr instance) {
    return Match_Instance(new BStreamFileToolkit(tk, false), (instance == IntPtr.Zero) ? null : new Recorded_Instance(instance, false));
  }

  private int SwigDirectorClone(IntPtr tk, IntPtr handler) {
    return (int)Clone(new BStreamFileToolkit(tk, false), (handler == IntPtr.Zero) ? null : new SWIGTYPE_p_p_BBaseOpcodeHandler(handler, false));
  }

  private bool SwigDirectorNeedsContext(IntPtr tk) {
    return NeedsContext(new BStreamFileToolkit(tk, false));
  }

  public delegate int SwigDelegateTK_Clip_Rectangle_0(IntPtr tk);
  public delegate int SwigDelegateTK_Clip_Rectangle_1(IntPtr tk);
  public delegate int SwigDelegateTK_Clip_Rectangle_2(IntPtr tk);
  public delegate int SwigDelegateTK_Clip_Rectangle_3(IntPtr tk, ID_Key key, int variant);
  public delegate int SwigDelegateTK_Clip_Rectangle_4(IntPtr tk, ID_Key key);
  public delegate int SwigDelegateTK_Clip_Rectangle_5(IntPtr tk, ID_Key key, string special);
  public delegate void SwigDelegateTK_Clip_Rectangle_6();
  public delegate bool SwigDelegateTK_Clip_Rectangle_7(IntPtr tk, IntPtr instance);
  public delegate int SwigDelegateTK_Clip_Rectangle_8(IntPtr tk, IntPtr handler);
  public delegate bool SwigDelegateTK_Clip_Rectangle_9(IntPtr tk);

  private SwigDelegateTK_Clip_Rectangle_0 swigDelegate0;
  private SwigDelegateTK_Clip_Rectangle_1 swigDelegate1;
  private SwigDelegateTK_Clip_Rectangle_2 swigDelegate2;
  private SwigDelegateTK_Clip_Rectangle_3 swigDelegate3;
  private SwigDelegateTK_Clip_Rectangle_4 swigDelegate4;
  private SwigDelegateTK_Clip_Rectangle_5 swigDelegate5;
  private SwigDelegateTK_Clip_Rectangle_6 swigDelegate6;
  private SwigDelegateTK_Clip_Rectangle_7 swigDelegate7;
  private SwigDelegateTK_Clip_Rectangle_8 swigDelegate8;
  private SwigDelegateTK_Clip_Rectangle_9 swigDelegate9;

  private static Type[] swigMethodTypes0 = new Type[] { typeof(BStreamFileToolkit) };
  private static Type[] swigMethodTypes1 = new Type[] { typeof(BStreamFileToolkit) };
  private static Type[] swigMethodTypes2 = new Type[] { typeof(BStreamFileToolkit) };
  private static Type[] swigMethodTypes3 = new Type[] { typeof(BStreamFileToolkit), typeof(ID_Key), typeof(int) };
  private static Type[] swigMethodTypes4 = new Type[] { typeof(BStreamFileToolkit), typeof(ID_Key) };
  private static Type[] swigMethodTypes5 = new Type[] { typeof(BStreamFileToolkit), typeof(ID_Key), typeof(string) };
  private static Type[] swigMethodTypes6 = new Type[] {  };
  private static Type[] swigMethodTypes7 = new Type[] { typeof(BStreamFileToolkit), typeof(Recorded_Instance) };
  private static Type[] swigMethodTypes8 = new Type[] { typeof(BStreamFileToolkit), typeof(SWIGTYPE_p_p_BBaseOpcodeHandler) };
  private static Type[] swigMethodTypes9 = new Type[] { typeof(BStreamFileToolkit) };
}
