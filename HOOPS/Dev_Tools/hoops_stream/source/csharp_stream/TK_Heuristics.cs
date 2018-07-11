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


public class TK_Heuristics : BBaseOpcodeHandler {
  private HandleRef swigCPtr;

  public TK_Heuristics(IntPtr cPtr, bool cMemoryOwn) : base(HCSSTREAMPINVOKE.TK_HeuristicsUpcast(cPtr), cMemoryOwn) {
    swigCPtr = new HandleRef(this, cPtr);
  }

  public static HandleRef getCPtr(TK_Heuristics obj) {
    return (obj == null) ? new HandleRef(null, IntPtr.Zero) : obj.swigCPtr;
  }

  ~TK_Heuristics() {
    Dispose();
  }

  public override void Dispose() {
    lock(this) {
      if (swigCPtr.Handle != IntPtr.Zero) {
        if (swigCMemOwn) {
          swigCMemOwn = false;
          HCSSTREAMPINVOKE.delete_TK_Heuristics(swigCPtr);
        }
        swigCPtr = new HandleRef(null, IntPtr.Zero);
      }
      GC.SuppressFinalize(this);
      base.Dispose();
    }
  }

  public TK_Heuristics() : this(HCSSTREAMPINVOKE.new_TK_Heuristics(), true) {
    SwigDirectorConnect();
  }

  public override TK_Status Read(BStreamFileToolkit tk) {
    TK_Status ret = (TK_Status)((this.GetType() == typeof(TK_Heuristics)) ? HCSSTREAMPINVOKE.TK_Heuristics_Read(swigCPtr, BStreamFileToolkit.getCPtr(tk)) : HCSSTREAMPINVOKE.TK_Heuristics_ReadSwigExplicitTK_Heuristics(swigCPtr, BStreamFileToolkit.getCPtr(tk)));
    if (HCSSTREAMPINVOKE.SWIGPendingException.Pending) throw HCSSTREAMPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public override TK_Status Write(BStreamFileToolkit tk) {
    TK_Status ret = (TK_Status)((this.GetType() == typeof(TK_Heuristics)) ? HCSSTREAMPINVOKE.TK_Heuristics_Write(swigCPtr, BStreamFileToolkit.getCPtr(tk)) : HCSSTREAMPINVOKE.TK_Heuristics_WriteSwigExplicitTK_Heuristics(swigCPtr, BStreamFileToolkit.getCPtr(tk)));
    if (HCSSTREAMPINVOKE.SWIGPendingException.Pending) throw HCSSTREAMPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public override TK_Status Clone(BStreamFileToolkit tk, SWIGTYPE_p_p_BBaseOpcodeHandler handler) {
    TK_Status ret = (TK_Status)((this.GetType() == typeof(TK_Heuristics)) ? HCSSTREAMPINVOKE.TK_Heuristics_Clone(swigCPtr, BStreamFileToolkit.getCPtr(tk), SWIGTYPE_p_p_BBaseOpcodeHandler.getCPtr(handler)) : HCSSTREAMPINVOKE.TK_Heuristics_CloneSwigExplicitTK_Heuristics(swigCPtr, BStreamFileToolkit.getCPtr(tk), SWIGTYPE_p_p_BBaseOpcodeHandler.getCPtr(handler)));
    if (HCSSTREAMPINVOKE.SWIGPendingException.Pending) throw HCSSTREAMPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public TK_Status ReadAscii(BStreamFileToolkit tk) {
    TK_Status ret = (TK_Status)HCSSTREAMPINVOKE.TK_Heuristics_ReadAscii(swigCPtr, BStreamFileToolkit.getCPtr(tk));
    if (HCSSTREAMPINVOKE.SWIGPendingException.Pending) throw HCSSTREAMPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public TK_Status WriteAscii(BStreamFileToolkit tk) {
    TK_Status ret = (TK_Status)HCSSTREAMPINVOKE.TK_Heuristics_WriteAscii(swigCPtr, BStreamFileToolkit.getCPtr(tk));
    if (HCSSTREAMPINVOKE.SWIGPendingException.Pending) throw HCSSTREAMPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public void SetMask(int m) {
    HCSSTREAMPINVOKE.TK_Heuristics_SetMask(swigCPtr, m);
  }

  public int GetMask() {
    int ret = HCSSTREAMPINVOKE.TK_Heuristics_GetMask(swigCPtr);
    return ret;
  }

  public void SetValue(int v) {
    HCSSTREAMPINVOKE.TK_Heuristics_SetValue(swigCPtr, v);
  }

  public int GetValue() {
    int ret = HCSSTREAMPINVOKE.TK_Heuristics_GetValue(swigCPtr);
    return ret;
  }

  public void SetRelatedSelectionLimit(int r) {
    HCSSTREAMPINVOKE.TK_Heuristics_SetRelatedSelectionLimit(swigCPtr, r);
  }

  public int GetRelatedSelectionLimit() {
    int ret = HCSSTREAMPINVOKE.TK_Heuristics_GetRelatedSelectionLimit(swigCPtr);
    return ret;
  }

  public void SetInternalSelectionLimit(int i) {
    HCSSTREAMPINVOKE.TK_Heuristics_SetInternalSelectionLimit(swigCPtr, i);
  }

  public int GetInternalSelectionLimit() {
    int ret = HCSSTREAMPINVOKE.TK_Heuristics_GetInternalSelectionLimit(swigCPtr);
    return ret;
  }

  public void SetInternalShellSelectionLimit(int i) {
    HCSSTREAMPINVOKE.TK_Heuristics_SetInternalShellSelectionLimit(swigCPtr, i);
  }

  public int GetInternalShellSelectionLimit() {
    int ret = HCSSTREAMPINVOKE.TK_Heuristics_GetInternalShellSelectionLimit(swigCPtr);
    return ret;
  }

  public void SetInternalPolylineSelectionLimit(int i) {
    HCSSTREAMPINVOKE.TK_Heuristics_SetInternalPolylineSelectionLimit(swigCPtr, i);
  }

  public int GetInternalPolylineSelectionLimit() {
    int ret = HCSSTREAMPINVOKE.TK_Heuristics_GetInternalPolylineSelectionLimit(swigCPtr);
    return ret;
  }

  public void SetExtras(int e) {
    HCSSTREAMPINVOKE.TK_Heuristics_SetExtras(swigCPtr, e);
  }

  public int GetExtras() {
    int ret = HCSSTREAMPINVOKE.TK_Heuristics_GetExtras(swigCPtr);
    return ret;
  }

  public void SetCulling(int c) {
    HCSSTREAMPINVOKE.TK_Heuristics_SetCulling(swigCPtr, c);
  }

  public int GetCulling() {
    int ret = HCSSTREAMPINVOKE.TK_Heuristics_GetCulling(swigCPtr);
    return ret;
  }

  public void SetSelectionCulling(int c) {
    HCSSTREAMPINVOKE.TK_Heuristics_SetSelectionCulling(swigCPtr, c);
  }

  public int GetSelectionCulling() {
    int ret = HCSSTREAMPINVOKE.TK_Heuristics_GetSelectionCulling(swigCPtr);
    return ret;
  }

  public void SetPixelThreshold(int c) {
    HCSSTREAMPINVOKE.TK_Heuristics_SetPixelThreshold(swigCPtr, c);
  }

  public int GetPixelThreshold() {
    int ret = HCSSTREAMPINVOKE.TK_Heuristics_GetPixelThreshold(swigCPtr);
    return ret;
  }

  public void SetMaximumExtent(int c) {
    HCSSTREAMPINVOKE.TK_Heuristics_SetMaximumExtent(swigCPtr, c);
  }

  public int GetMaximumExtent() {
    int ret = HCSSTREAMPINVOKE.TK_Heuristics_GetMaximumExtent(swigCPtr);
    return ret;
  }

  public int GetMaximumExtentMode() {
    int ret = HCSSTREAMPINVOKE.TK_Heuristics_GetMaximumExtentMode(swigCPtr);
    return ret;
  }

  public void SetMaximumExtentMode(int c) {
    HCSSTREAMPINVOKE.TK_Heuristics_SetMaximumExtentMode(swigCPtr, c);
  }

  public int GetMaximumExtentLevel() {
    int ret = HCSSTREAMPINVOKE.TK_Heuristics_GetMaximumExtentLevel(swigCPtr);
    return ret;
  }

  public void SetMaximumExtentLevel(int c) {
    HCSSTREAMPINVOKE.TK_Heuristics_SetMaximumExtentLevel(swigCPtr, c);
  }

  public void SetHardExtent(int c) {
    HCSSTREAMPINVOKE.TK_Heuristics_SetHardExtent(swigCPtr, c);
  }

  public int GetHardExtent() {
    int ret = HCSSTREAMPINVOKE.TK_Heuristics_GetHardExtent(swigCPtr);
    return ret;
  }

  public IntPtr GetVector() {return HCSSTREAMPINVOKE.TK_Heuristics_GetVector(swigCPtr);}

  public void SetVector(float x, float y, float z) {
    HCSSTREAMPINVOKE.TK_Heuristics_SetVector__SWIG_0(swigCPtr, x, y, z);
  }

  public void SetVector(float[] v) {
    HCSSTREAMPINVOKE.TK_Heuristics_SetVector__SWIG_1(swigCPtr, v);
  }

  public float GetVectorTolerance() {
    float ret = HCSSTREAMPINVOKE.TK_Heuristics_GetVectorTolerance(swigCPtr);
    return ret;
  }

  public void SetVectorTolerance(float tol) {
    HCSSTREAMPINVOKE.TK_Heuristics_SetVectorTolerance(swigCPtr, tol);
  }

  public void SetMaxDistance(float m) {
    HCSSTREAMPINVOKE.TK_Heuristics_SetMaxDistance(swigCPtr, m);
  }

  public float GetMaxDistance() {
    float ret = HCSSTREAMPINVOKE.TK_Heuristics_GetMaxDistance(swigCPtr);
    return ret;
  }

  public IntPtr GetViewVolume() {return HCSSTREAMPINVOKE.TK_Heuristics_GetViewVolume(swigCPtr);}

  public void SetViewVolume(float ax, float ay, float az, float bx, float by, float bz) {
    HCSSTREAMPINVOKE.TK_Heuristics_SetViewVolume__SWIG_0(swigCPtr, ax, ay, az, bx, by, bz);
  }

  public void SetViewVolume(float[] v) {
    HCSSTREAMPINVOKE.TK_Heuristics_SetViewVolume__SWIG_1(swigCPtr, v);
  }

  public void SetOrderedWeightsMask(int c) {
    HCSSTREAMPINVOKE.TK_Heuristics_SetOrderedWeightsMask(swigCPtr, c);
  }

  public int GetOrderedWeightsMask() {
    int ret = HCSSTREAMPINVOKE.TK_Heuristics_GetOrderedWeightsMask(swigCPtr);
    return ret;
  }

  public void SetOrderedWeight(int index, float weight) {
    HCSSTREAMPINVOKE.TK_Heuristics_SetOrderedWeight(swigCPtr, index, weight);
  }

  public float GetOrderedWeight(int index) {
    float ret = HCSSTREAMPINVOKE.TK_Heuristics_GetOrderedWeight(swigCPtr, index);
    return ret;
  }

  public IntPtr GetOrderedWeights() {return HCSSTREAMPINVOKE.TK_Heuristics_GetOrderedWeights__SWIG_0(swigCPtr);}

  public void SetSelectionLevel(int l) {
    HCSSTREAMPINVOKE.TK_Heuristics_SetSelectionLevel(swigCPtr, l);
  }

  public int GetSelectionLevel() {
    int ret = HCSSTREAMPINVOKE.TK_Heuristics_GetSelectionLevel(swigCPtr);
    return ret;
  }

  public void SetForceDefer(int l) {
    HCSSTREAMPINVOKE.TK_Heuristics_SetForceDefer(swigCPtr, l);
  }

  public int GetForceDefer() {
    int ret = HCSSTREAMPINVOKE.TK_Heuristics_GetForceDefer(swigCPtr);
    return ret;
  }

  private void SwigDirectorConnect() {
    if (SwigDerivedClassHasMethod("Read", swigMethodTypes0))
      swigDelegate0 = new SwigDelegateTK_Heuristics_0(SwigDirectorRead);
    if (SwigDerivedClassHasMethod("Write", swigMethodTypes1))
      swigDelegate1 = new SwigDelegateTK_Heuristics_1(SwigDirectorWrite);
    if (SwigDerivedClassHasMethod("Execute", swigMethodTypes2))
      swigDelegate2 = new SwigDelegateTK_Heuristics_2(SwigDirectorExecute);
    if (SwigDerivedClassHasMethod("Interpret", swigMethodTypes3))
      swigDelegate3 = new SwigDelegateTK_Heuristics_3(SwigDirectorInterpret__SWIG_0);
    if (SwigDerivedClassHasMethod("Interpret", swigMethodTypes4))
      swigDelegate4 = new SwigDelegateTK_Heuristics_4(SwigDirectorInterpret__SWIG_1);
    if (SwigDerivedClassHasMethod("Interpret_special", swigMethodTypes5))
      swigDelegate5 = new SwigDelegateTK_Heuristics_5(SwigDirectorInterpret_special);
    if (SwigDerivedClassHasMethod("Reset", swigMethodTypes6))
      swigDelegate6 = new SwigDelegateTK_Heuristics_6(SwigDirectorReset);
    if (SwigDerivedClassHasMethod("Match_Instance", swigMethodTypes7))
      swigDelegate7 = new SwigDelegateTK_Heuristics_7(SwigDirectorMatch_Instance);
    if (SwigDerivedClassHasMethod("Clone", swigMethodTypes8))
      swigDelegate8 = new SwigDelegateTK_Heuristics_8(SwigDirectorClone);
    if (SwigDerivedClassHasMethod("NeedsContext", swigMethodTypes9))
      swigDelegate9 = new SwigDelegateTK_Heuristics_9(SwigDirectorNeedsContext);
    HCSSTREAMPINVOKE.TK_Heuristics_director_connect(swigCPtr, swigDelegate0, swigDelegate1, swigDelegate2, swigDelegate3, swigDelegate4, swigDelegate5, swigDelegate6, swigDelegate7, swigDelegate8, swigDelegate9);
  }

  private bool SwigDerivedClassHasMethod(string methodName, Type[] methodTypes) {
    System.Reflection.MethodInfo methodInfo = this.GetType().GetMethod(methodName, System.Reflection.BindingFlags.Public | System.Reflection.BindingFlags.NonPublic | System.Reflection.BindingFlags.Instance, null, methodTypes, null);
    bool hasDerivedMethod = methodInfo.DeclaringType.IsSubclassOf(typeof(TK_Heuristics));
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

  public delegate int SwigDelegateTK_Heuristics_0(IntPtr tk);
  public delegate int SwigDelegateTK_Heuristics_1(IntPtr tk);
  public delegate int SwigDelegateTK_Heuristics_2(IntPtr tk);
  public delegate int SwigDelegateTK_Heuristics_3(IntPtr tk, ID_Key key, int variant);
  public delegate int SwigDelegateTK_Heuristics_4(IntPtr tk, ID_Key key);
  public delegate int SwigDelegateTK_Heuristics_5(IntPtr tk, ID_Key key, string special);
  public delegate void SwigDelegateTK_Heuristics_6();
  public delegate bool SwigDelegateTK_Heuristics_7(IntPtr tk, IntPtr instance);
  public delegate int SwigDelegateTK_Heuristics_8(IntPtr tk, IntPtr handler);
  public delegate bool SwigDelegateTK_Heuristics_9(IntPtr tk);

  private SwigDelegateTK_Heuristics_0 swigDelegate0;
  private SwigDelegateTK_Heuristics_1 swigDelegate1;
  private SwigDelegateTK_Heuristics_2 swigDelegate2;
  private SwigDelegateTK_Heuristics_3 swigDelegate3;
  private SwigDelegateTK_Heuristics_4 swigDelegate4;
  private SwigDelegateTK_Heuristics_5 swigDelegate5;
  private SwigDelegateTK_Heuristics_6 swigDelegate6;
  private SwigDelegateTK_Heuristics_7 swigDelegate7;
  private SwigDelegateTK_Heuristics_8 swigDelegate8;
  private SwigDelegateTK_Heuristics_9 swigDelegate9;

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