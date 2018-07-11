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
#else
using HLONG = System.Int32;
#endif


public class TK_PolyPolypoint : BBaseOpcodeHandler {
  private HandleRef swigCPtr;

  internal TK_PolyPolypoint(IntPtr cPtr, bool cMemoryOwn) : base(HCSBSTREAMPINVOKE.TK_PolyPolypointUpcast(cPtr), cMemoryOwn) {
    swigCPtr = new HandleRef(this, cPtr);
  }

  internal static HandleRef getCPtr(TK_PolyPolypoint obj) {
    return (obj == null) ? new HandleRef(null, IntPtr.Zero) : obj.swigCPtr;
  }

  ~TK_PolyPolypoint() {
    Dispose();
  }

  public override void Dispose() {
    lock(this) {
      if (swigCPtr.Handle != IntPtr.Zero) {
        if (swigCMemOwn) {
          swigCMemOwn = false;
          HCSBSTREAMPINVOKE.delete_TK_PolyPolypoint(swigCPtr);
        }
        swigCPtr = new HandleRef(null, IntPtr.Zero);
      }
      GC.SuppressFinalize(this);
      base.Dispose();
    }
  }

  public TK_PolyPolypoint(byte opcode) : this(HCSBSTREAMPINVOKE.new_TK_PolyPolypoint(opcode), true) {
    SwigDirectorConnect();
  }

  public override TK_Status Read(BStreamFileToolkit tk) {
    TK_Status ret = (TK_Status)((this.GetType() == typeof(TK_PolyPolypoint)) ? HCSBSTREAMPINVOKE.TK_PolyPolypoint_Read(swigCPtr, BStreamFileToolkit.getCPtr(tk)) : HCSBSTREAMPINVOKE.TK_PolyPolypoint_ReadSwigExplicitTK_PolyPolypoint(swigCPtr, BStreamFileToolkit.getCPtr(tk)));
    if (HCSBSTREAMPINVOKE.SWIGPendingException.Pending) throw HCSBSTREAMPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public override TK_Status Write(BStreamFileToolkit tk) {
    TK_Status ret = (TK_Status)((this.GetType() == typeof(TK_PolyPolypoint)) ? HCSBSTREAMPINVOKE.TK_PolyPolypoint_Write(swigCPtr, BStreamFileToolkit.getCPtr(tk)) : HCSBSTREAMPINVOKE.TK_PolyPolypoint_WriteSwigExplicitTK_PolyPolypoint(swigCPtr, BStreamFileToolkit.getCPtr(tk)));
    if (HCSBSTREAMPINVOKE.SWIGPendingException.Pending) throw HCSBSTREAMPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public override TK_Status Clone(BStreamFileToolkit tk, SWIGTYPE_p_p_BBaseOpcodeHandler handler) {
    TK_Status ret = (TK_Status)((this.GetType() == typeof(TK_PolyPolypoint)) ? HCSBSTREAMPINVOKE.TK_PolyPolypoint_Clone(swigCPtr, BStreamFileToolkit.getCPtr(tk), SWIGTYPE_p_p_BBaseOpcodeHandler.getCPtr(handler)) : HCSBSTREAMPINVOKE.TK_PolyPolypoint_CloneSwigExplicitTK_PolyPolypoint(swigCPtr, BStreamFileToolkit.getCPtr(tk), SWIGTYPE_p_p_BBaseOpcodeHandler.getCPtr(handler)));
    if (HCSBSTREAMPINVOKE.SWIGPendingException.Pending) throw HCSBSTREAMPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public TK_Status ReadAscii(BStreamFileToolkit tk) {
    TK_Status ret = (TK_Status)HCSBSTREAMPINVOKE.TK_PolyPolypoint_ReadAscii(swigCPtr, BStreamFileToolkit.getCPtr(tk));
    if (HCSBSTREAMPINVOKE.SWIGPendingException.Pending) throw HCSBSTREAMPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public TK_Status WriteAscii(BStreamFileToolkit tk) {
    TK_Status ret = (TK_Status)HCSBSTREAMPINVOKE.TK_PolyPolypoint_WriteAscii(swigCPtr, BStreamFileToolkit.getCPtr(tk));
    if (HCSBSTREAMPINVOKE.SWIGPendingException.Pending) throw HCSBSTREAMPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public override TK_Status Execute(BStreamFileToolkit tk) {
    TK_Status ret = (TK_Status)((this.GetType() == typeof(TK_PolyPolypoint)) ? HCSBSTREAMPINVOKE.TK_PolyPolypoint_Execute(swigCPtr, BStreamFileToolkit.getCPtr(tk)) : HCSBSTREAMPINVOKE.TK_PolyPolypoint_ExecuteSwigExplicitTK_PolyPolypoint(swigCPtr, BStreamFileToolkit.getCPtr(tk)));
    if (HCSBSTREAMPINVOKE.SWIGPendingException.Pending) throw HCSBSTREAMPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public override void Reset() {
    if (this.GetType() == typeof(TK_PolyPolypoint)) HCSBSTREAMPINVOKE.TK_PolyPolypoint_Reset(swigCPtr); else HCSBSTREAMPINVOKE.TK_PolyPolypoint_ResetSwigExplicitTK_PolyPolypoint(swigCPtr);
  }

  public TK_Status SetPoints(int point_count, float[] points) {
    TK_Status ret = (TK_Status)HCSBSTREAMPINVOKE.TK_PolyPolypoint_SetPoints__SWIG_0(swigCPtr, point_count, points);
    return ret;
  }

  public TK_Status SetPoints(int point_count) {
    TK_Status ret = (TK_Status)HCSBSTREAMPINVOKE.TK_PolyPolypoint_SetPoints__SWIG_1(swigCPtr, point_count);
    return ret;
  }

  public TK_Status SetDPoints(int point_count, double[] points) {
    TK_Status ret = (TK_Status)HCSBSTREAMPINVOKE.TK_PolyPolypoint_SetDPoints__SWIG_0(swigCPtr, point_count, points);
    return ret;
  }

  public TK_Status SetDPoints(int point_count) {
    TK_Status ret = (TK_Status)HCSBSTREAMPINVOKE.TK_PolyPolypoint_SetDPoints__SWIG_1(swigCPtr, point_count);
    return ret;
  }

  public TK_Status SetLengths(int primitive_count, int[] lengths) {
    TK_Status ret = (TK_Status)HCSBSTREAMPINVOKE.TK_PolyPolypoint_SetLengths__SWIG_0(swigCPtr, primitive_count, lengths);
    return ret;
  }

  public TK_Status SetLengths(int primitive_count) {
    TK_Status ret = (TK_Status)HCSBSTREAMPINVOKE.TK_PolyPolypoint_SetLengths__SWIG_1(swigCPtr, primitive_count);
    return ret;
  }

  private void SwigDirectorConnect() {
    if (SwigDerivedClassHasMethod("Read", swigMethodTypes0))
      swigDelegate0 = new SwigDelegateTK_PolyPolypoint_0(SwigDirectorRead);
    if (SwigDerivedClassHasMethod("Write", swigMethodTypes1))
      swigDelegate1 = new SwigDelegateTK_PolyPolypoint_1(SwigDirectorWrite);
    if (SwigDerivedClassHasMethod("Execute", swigMethodTypes2))
      swigDelegate2 = new SwigDelegateTK_PolyPolypoint_2(SwigDirectorExecute);
    if (SwigDerivedClassHasMethod("Interpret", swigMethodTypes3))
      swigDelegate3 = new SwigDelegateTK_PolyPolypoint_3(SwigDirectorInterpret__SWIG_0);
    if (SwigDerivedClassHasMethod("Interpret", swigMethodTypes4))
      swigDelegate4 = new SwigDelegateTK_PolyPolypoint_4(SwigDirectorInterpret__SWIG_1);
    if (SwigDerivedClassHasMethod("Interpret_special", swigMethodTypes5))
      swigDelegate5 = new SwigDelegateTK_PolyPolypoint_5(SwigDirectorInterpret_special);
    if (SwigDerivedClassHasMethod("Reset", swigMethodTypes6))
      swigDelegate6 = new SwigDelegateTK_PolyPolypoint_6(SwigDirectorReset);
    if (SwigDerivedClassHasMethod("Match_Instance", swigMethodTypes7))
      swigDelegate7 = new SwigDelegateTK_PolyPolypoint_7(SwigDirectorMatch_Instance);
    if (SwigDerivedClassHasMethod("Clone", swigMethodTypes8))
      swigDelegate8 = new SwigDelegateTK_PolyPolypoint_8(SwigDirectorClone);
    if (SwigDerivedClassHasMethod("NeedsContext", swigMethodTypes9))
      swigDelegate9 = new SwigDelegateTK_PolyPolypoint_9(SwigDirectorNeedsContext);
    HCSBSTREAMPINVOKE.TK_PolyPolypoint_director_connect(swigCPtr, swigDelegate0, swigDelegate1, swigDelegate2, swigDelegate3, swigDelegate4, swigDelegate5, swigDelegate6, swigDelegate7, swigDelegate8, swigDelegate9);
  }

  private bool SwigDerivedClassHasMethod(string methodName, Type[] methodTypes) {
    System.Reflection.MethodInfo methodInfo = this.GetType().GetMethod(methodName, System.Reflection.BindingFlags.Public | System.Reflection.BindingFlags.NonPublic | System.Reflection.BindingFlags.Instance, null, methodTypes, null);
    bool hasDerivedMethod = methodInfo.DeclaringType.IsSubclassOf(typeof(TK_PolyPolypoint));
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

  private int SwigDirectorInterpret__SWIG_0(IntPtr tk, int key, int variant) {
    return (int)Interpret(new BStreamFileToolkit(tk, false), key, variant);
  }

  private int SwigDirectorInterpret__SWIG_1(IntPtr tk, int key) {
    return (int)Interpret(new BStreamFileToolkit(tk, false), key);
  }

  private int SwigDirectorInterpret_special(IntPtr tk, int key, string special) {
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

  public delegate int SwigDelegateTK_PolyPolypoint_0(IntPtr tk);
  public delegate int SwigDelegateTK_PolyPolypoint_1(IntPtr tk);
  public delegate int SwigDelegateTK_PolyPolypoint_2(IntPtr tk);
  public delegate int SwigDelegateTK_PolyPolypoint_3(IntPtr tk, int key, int variant);
  public delegate int SwigDelegateTK_PolyPolypoint_4(IntPtr tk, int key);
  public delegate int SwigDelegateTK_PolyPolypoint_5(IntPtr tk, int key, string special);
  public delegate void SwigDelegateTK_PolyPolypoint_6();
  public delegate bool SwigDelegateTK_PolyPolypoint_7(IntPtr tk, IntPtr instance);
  public delegate int SwigDelegateTK_PolyPolypoint_8(IntPtr tk, IntPtr handler);
  public delegate bool SwigDelegateTK_PolyPolypoint_9(IntPtr tk);

  private SwigDelegateTK_PolyPolypoint_0 swigDelegate0;
  private SwigDelegateTK_PolyPolypoint_1 swigDelegate1;
  private SwigDelegateTK_PolyPolypoint_2 swigDelegate2;
  private SwigDelegateTK_PolyPolypoint_3 swigDelegate3;
  private SwigDelegateTK_PolyPolypoint_4 swigDelegate4;
  private SwigDelegateTK_PolyPolypoint_5 swigDelegate5;
  private SwigDelegateTK_PolyPolypoint_6 swigDelegate6;
  private SwigDelegateTK_PolyPolypoint_7 swigDelegate7;
  private SwigDelegateTK_PolyPolypoint_8 swigDelegate8;
  private SwigDelegateTK_PolyPolypoint_9 swigDelegate9;

  private static Type[] swigMethodTypes0 = new Type[] { typeof(BStreamFileToolkit) };
  private static Type[] swigMethodTypes1 = new Type[] { typeof(BStreamFileToolkit) };
  private static Type[] swigMethodTypes2 = new Type[] { typeof(BStreamFileToolkit) };
  private static Type[] swigMethodTypes3 = new Type[] { typeof(BStreamFileToolkit), typeof(int), typeof(int) };
  private static Type[] swigMethodTypes4 = new Type[] { typeof(BStreamFileToolkit), typeof(int) };
  private static Type[] swigMethodTypes5 = new Type[] { typeof(BStreamFileToolkit), typeof(int), typeof(string) };
  private static Type[] swigMethodTypes6 = new Type[] {  };
  private static Type[] swigMethodTypes7 = new Type[] { typeof(BStreamFileToolkit), typeof(Recorded_Instance) };
  private static Type[] swigMethodTypes8 = new Type[] { typeof(BStreamFileToolkit), typeof(SWIGTYPE_p_p_BBaseOpcodeHandler) };
  private static Type[] swigMethodTypes9 = new Type[] { typeof(BStreamFileToolkit) };
  public enum Suboptions {
    TKPP_COMPRESSED = 0x0001,
    TKPP_HAS_EXPLICIT_PRIMITIVE_COUNT = 0x0002,
    TKPP_ONE_PRIMITIVE_ONLY = 0x0004,
    TKPP_GLOBAL_QUANTIZATION = 0x0008,
    TKPP_ZERO_X = 0x0100,
    TKPP_SAME_X = 0x0200,
    TKPP_PER_PRIMITIVE_X = 0x0300,
    TKPP_ZERO_Y = 0x0400,
    TKPP_SAME_Y = 0x0800,
    TKPP_PER_PRIMITIVE_Y = 0x0C00,
    TKPP_ZERO_Z = 0x1000,
    TKPP_SAME_Z = 0x2000,
    TKPP_PER_PRIMITIVE_Z = 0x3000,
    TKPP_EXPANDED = 0x8000
  }

}
