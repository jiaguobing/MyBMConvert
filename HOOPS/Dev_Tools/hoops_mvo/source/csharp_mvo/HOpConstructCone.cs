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
using MVO_POINTER_SIZED_INT = System.Int64;
using MVO_POINTER_SIZED_UINT = System.UInt64;
using ID_Key = System.Int64;
#else
using MVO_POINTER_SIZED_INT = System.Int32;
using MVO_POINTER_SIZED_UINT = System.UInt32;
using ID_Key = System.Int32;
#endif


public class HOpConstructCone : HBaseOperator {
  private HandleRef swigCPtr;

  public HOpConstructCone(IntPtr cPtr, bool cMemoryOwn) : base(HCSMVOPINVOKE.HOpConstructConeUpcast(cPtr), cMemoryOwn) {
    swigCPtr = new HandleRef(this, cPtr);
  }

  public static HandleRef getCPtr(HOpConstructCone obj) {
    return (obj == null) ? new HandleRef(null, IntPtr.Zero) : obj.swigCPtr;
  }

  ~HOpConstructCone() {
    Dispose();
  }

  public override void Dispose() {
    lock(this) {
      if (swigCPtr.Handle != IntPtr.Zero) {
        if (swigCMemOwn) {
          swigCMemOwn = false;
          HCSMVOPINVOKE.delete_HOpConstructCone(swigCPtr);
        }
        swigCPtr = new HandleRef(null, IntPtr.Zero);
      }
      GC.SuppressFinalize(this);
      base.Dispose();
    }
  }

  protected float m_height {
    set {
      HCSMVOPINVOKE.HOpConstructCone_m_height_set(swigCPtr, value);
    } 
    get {
      float ret = HCSMVOPINVOKE.HOpConstructCone_m_height_get(swigCPtr);
      return ret;
    } 
  }

  protected float m_RadiusLength {
    set {
      HCSMVOPINVOKE.HOpConstructCone_m_RadiusLength_set(swigCPtr, value);
    } 
    get {
      float ret = HCSMVOPINVOKE.HOpConstructCone_m_RadiusLength_get(swigCPtr);
      return ret;
    } 
  }

  protected HPoint m_WorldRadiusVector {
    set {
      HCSMVOPINVOKE.HOpConstructCone_m_WorldRadiusVector_set(swigCPtr, HPoint.getCPtr(value));
    } 
    get {
      IntPtr cPtr = HCSMVOPINVOKE.HOpConstructCone_m_WorldRadiusVector_get(swigCPtr);
      HPoint ret = (cPtr == IntPtr.Zero) ? null : new HPoint(cPtr, false);
      return ret;
    } 
  }

  protected void ConstructWireframeCone(HPoint center, float radius, float height) {
    HCSMVOPINVOKE.HOpConstructCone_ConstructWireframeCone(swigCPtr, HPoint.getCPtr(center), radius, height);
    if (HCSMVOPINVOKE.SWIGPendingException.Pending) throw HCSMVOPINVOKE.SWIGPendingException.Retrieve();
  }

  public HOpConstructCone(HBaseView view, int DoRepeat, int DoCapture) : this(HCSMVOPINVOKE.new_HOpConstructCone__SWIG_0(HBaseView.getCPtr(view), DoRepeat, DoCapture), true) {
    SwigDirectorConnect();
  }

  public HOpConstructCone(HBaseView view, int DoRepeat) : this(HCSMVOPINVOKE.new_HOpConstructCone__SWIG_1(HBaseView.getCPtr(view), DoRepeat), true) {
    SwigDirectorConnect();
  }

  public HOpConstructCone(HBaseView view) : this(HCSMVOPINVOKE.new_HOpConstructCone__SWIG_2(HBaseView.getCPtr(view)), true) {
    SwigDirectorConnect();
  }

  public override string GetName() {
    string ret = ((this.GetType() == typeof(HOpConstructCone)) ? HCSMVOPINVOKE.HOpConstructCone_GetName(swigCPtr) : HCSMVOPINVOKE.HOpConstructCone_GetNameSwigExplicitHOpConstructCone(swigCPtr));
    return ret;
  }

  public override int OnLButtonDown(HEventInfo hevent) {
    int ret = ((this.GetType() == typeof(HOpConstructCone)) ? HCSMVOPINVOKE.HOpConstructCone_OnLButtonDown(swigCPtr, HEventInfo.getCPtr(hevent)) : HCSMVOPINVOKE.HOpConstructCone_OnLButtonDownSwigExplicitHOpConstructCone(swigCPtr, HEventInfo.getCPtr(hevent)));
    if (HCSMVOPINVOKE.SWIGPendingException.Pending) throw HCSMVOPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public override int OnNoButtonDownAndMove(HEventInfo hevent) {
    int ret = ((this.GetType() == typeof(HOpConstructCone)) ? HCSMVOPINVOKE.HOpConstructCone_OnNoButtonDownAndMove(swigCPtr, HEventInfo.getCPtr(hevent)) : HCSMVOPINVOKE.HOpConstructCone_OnNoButtonDownAndMoveSwigExplicitHOpConstructCone(swigCPtr, HEventInfo.getCPtr(hevent)));
    if (HCSMVOPINVOKE.SWIGPendingException.Pending) throw HCSMVOPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public override int OnLButtonDownAndMove(HEventInfo hevent) {
    int ret = ((this.GetType() == typeof(HOpConstructCone)) ? HCSMVOPINVOKE.HOpConstructCone_OnLButtonDownAndMove(swigCPtr, HEventInfo.getCPtr(hevent)) : HCSMVOPINVOKE.HOpConstructCone_OnLButtonDownAndMoveSwigExplicitHOpConstructCone(swigCPtr, HEventInfo.getCPtr(hevent)));
    if (HCSMVOPINVOKE.SWIGPendingException.Pending) throw HCSMVOPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public override int OnLButtonUp(HEventInfo hevent) {
    int ret = ((this.GetType() == typeof(HOpConstructCone)) ? HCSMVOPINVOKE.HOpConstructCone_OnLButtonUp(swigCPtr, HEventInfo.getCPtr(hevent)) : HCSMVOPINVOKE.HOpConstructCone_OnLButtonUpSwigExplicitHOpConstructCone(swigCPtr, HEventInfo.getCPtr(hevent)));
    if (HCSMVOPINVOKE.SWIGPendingException.Pending) throw HCSMVOPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public override HBaseOperator Clone() {
    IntPtr cPtr = ((this.GetType() == typeof(HOpConstructCone)) ? HCSMVOPINVOKE.HOpConstructCone_Clone(swigCPtr) : HCSMVOPINVOKE.HOpConstructCone_CloneSwigExplicitHOpConstructCone(swigCPtr));
    HBaseOperator ret = (cPtr == IntPtr.Zero) ? null : new HBaseOperator(cPtr, false);
    return ret;
  }

  private void SwigDirectorConnect() {
    if (SwigDerivedClassHasMethod("CreateListenerManager", swigMethodTypes0))
      swigDelegate0 = new SwigDelegateHOpConstructCone_0(SwigDirectorCreateListenerManager);
    if (SwigDerivedClassHasMethod("OnLButtonDown", swigMethodTypes1))
      swigDelegate1 = new SwigDelegateHOpConstructCone_1(SwigDirectorOnLButtonDown);
    if (SwigDerivedClassHasMethod("OnMouseMove", swigMethodTypes2))
      swigDelegate2 = new SwigDelegateHOpConstructCone_2(SwigDirectorOnMouseMove);
    if (SwigDerivedClassHasMethod("OnLButtonUp", swigMethodTypes3))
      swigDelegate3 = new SwigDelegateHOpConstructCone_3(SwigDirectorOnLButtonUp);
    if (SwigDerivedClassHasMethod("OnLButtonDblClk", swigMethodTypes4))
      swigDelegate4 = new SwigDelegateHOpConstructCone_4(SwigDirectorOnLButtonDblClk);
    if (SwigDerivedClassHasMethod("OnMButtonDown", swigMethodTypes5))
      swigDelegate5 = new SwigDelegateHOpConstructCone_5(SwigDirectorOnMButtonDown);
    if (SwigDerivedClassHasMethod("OnMButtonUp", swigMethodTypes6))
      swigDelegate6 = new SwigDelegateHOpConstructCone_6(SwigDirectorOnMButtonUp);
    if (SwigDerivedClassHasMethod("OnMButtonDblClk", swigMethodTypes7))
      swigDelegate7 = new SwigDelegateHOpConstructCone_7(SwigDirectorOnMButtonDblClk);
    if (SwigDerivedClassHasMethod("OnRButtonDown", swigMethodTypes8))
      swigDelegate8 = new SwigDelegateHOpConstructCone_8(SwigDirectorOnRButtonDown);
    if (SwigDerivedClassHasMethod("OnRButtonUp", swigMethodTypes9))
      swigDelegate9 = new SwigDelegateHOpConstructCone_9(SwigDirectorOnRButtonUp);
    if (SwigDerivedClassHasMethod("OnRButtonDblClk", swigMethodTypes10))
      swigDelegate10 = new SwigDelegateHOpConstructCone_10(SwigDirectorOnRButtonDblClk);
    if (SwigDerivedClassHasMethod("OnMouseWheel", swigMethodTypes11))
      swigDelegate11 = new SwigDelegateHOpConstructCone_11(SwigDirectorOnMouseWheel);
    if (SwigDerivedClassHasMethod("OnKeyDown", swigMethodTypes12))
      swigDelegate12 = new SwigDelegateHOpConstructCone_12(SwigDirectorOnKeyDown);
    if (SwigDerivedClassHasMethod("OnKeyUp", swigMethodTypes13))
      swigDelegate13 = new SwigDelegateHOpConstructCone_13(SwigDirectorOnKeyUp);
    if (SwigDerivedClassHasMethod("OnTouchesDown", swigMethodTypes14))
      swigDelegate14 = new SwigDelegateHOpConstructCone_14(SwigDirectorOnTouchesDown);
    if (SwigDerivedClassHasMethod("OnTouchesMove", swigMethodTypes15))
      swigDelegate15 = new SwigDelegateHOpConstructCone_15(SwigDirectorOnTouchesMove);
    if (SwigDerivedClassHasMethod("OnTouchesUp", swigMethodTypes16))
      swigDelegate16 = new SwigDelegateHOpConstructCone_16(SwigDirectorOnTouchesUp);
    if (SwigDerivedClassHasMethod("Clone", swigMethodTypes17))
      swigDelegate17 = new SwigDelegateHOpConstructCone_17(SwigDirectorClone);
    if (SwigDerivedClassHasMethod("OperatorStarted", swigMethodTypes18))
      swigDelegate18 = new SwigDelegateHOpConstructCone_18(SwigDirectorOperatorStarted);
    if (SwigDerivedClassHasMethod("GetName", swigMethodTypes19))
      swigDelegate19 = new SwigDelegateHOpConstructCone_19(SwigDirectorGetName);
    if (SwigDerivedClassHasMethod("OnLButtonDownAndMove", swigMethodTypes20))
      swigDelegate20 = new SwigDelegateHOpConstructCone_20(SwigDirectorOnLButtonDownAndMove);
    if (SwigDerivedClassHasMethod("OnMButtonDownAndMove", swigMethodTypes21))
      swigDelegate21 = new SwigDelegateHOpConstructCone_21(SwigDirectorOnMButtonDownAndMove);
    if (SwigDerivedClassHasMethod("OnRButtonDownAndMove", swigMethodTypes22))
      swigDelegate22 = new SwigDelegateHOpConstructCone_22(SwigDirectorOnRButtonDownAndMove);
    if (SwigDerivedClassHasMethod("OnLRButtonDownAndMove", swigMethodTypes23))
      swigDelegate23 = new SwigDelegateHOpConstructCone_23(SwigDirectorOnLRButtonDownAndMove);
    if (SwigDerivedClassHasMethod("OnNoButtonDownAndMove", swigMethodTypes24))
      swigDelegate24 = new SwigDelegateHOpConstructCone_24(SwigDirectorOnNoButtonDownAndMove);
    if (SwigDerivedClassHasMethod("OnTimer", swigMethodTypes25))
      swigDelegate25 = new SwigDelegateHOpConstructCone_25(SwigDirectorOnTimer);
    HCSMVOPINVOKE.HOpConstructCone_director_connect(swigCPtr, swigDelegate0, swigDelegate1, swigDelegate2, swigDelegate3, swigDelegate4, swigDelegate5, swigDelegate6, swigDelegate7, swigDelegate8, swigDelegate9, swigDelegate10, swigDelegate11, swigDelegate12, swigDelegate13, swigDelegate14, swigDelegate15, swigDelegate16, swigDelegate17, swigDelegate18, swigDelegate19, swigDelegate20, swigDelegate21, swigDelegate22, swigDelegate23, swigDelegate24, swigDelegate25);
  }

  private bool SwigDerivedClassHasMethod(string methodName, Type[] methodTypes) {
    System.Reflection.MethodInfo methodInfo = this.GetType().GetMethod(methodName, System.Reflection.BindingFlags.Public | System.Reflection.BindingFlags.NonPublic | System.Reflection.BindingFlags.Instance, null, methodTypes, null);
    bool hasDerivedMethod = methodInfo.DeclaringType.IsSubclassOf(typeof(HOpConstructCone));
    return hasDerivedMethod;
  }

  private IntPtr SwigDirectorCreateListenerManager() {
    return HEventListener.getCPtr(CreateListenerManager()).Handle;
  }

  private int SwigDirectorOnLButtonDown(IntPtr hevent) {
    return OnLButtonDown(new HEventInfo(hevent, false));
  }

  private int SwigDirectorOnMouseMove(IntPtr hevent) {
    return OnMouseMove(new HEventInfo(hevent, false));
  }

  private int SwigDirectorOnLButtonUp(IntPtr hevent) {
    return OnLButtonUp(new HEventInfo(hevent, false));
  }

  private int SwigDirectorOnLButtonDblClk(IntPtr hevent) {
    return OnLButtonDblClk(new HEventInfo(hevent, false));
  }

  private int SwigDirectorOnMButtonDown(IntPtr hevent) {
    return OnMButtonDown(new HEventInfo(hevent, false));
  }

  private int SwigDirectorOnMButtonUp(IntPtr hevent) {
    return OnMButtonUp(new HEventInfo(hevent, false));
  }

  private int SwigDirectorOnMButtonDblClk(IntPtr hevent) {
    return OnMButtonDblClk(new HEventInfo(hevent, false));
  }

  private int SwigDirectorOnRButtonDown(IntPtr hevent) {
    return OnRButtonDown(new HEventInfo(hevent, false));
  }

  private int SwigDirectorOnRButtonUp(IntPtr hevent) {
    return OnRButtonUp(new HEventInfo(hevent, false));
  }

  private int SwigDirectorOnRButtonDblClk(IntPtr hevent) {
    return OnRButtonDblClk(new HEventInfo(hevent, false));
  }

  private int SwigDirectorOnMouseWheel(IntPtr hevent) {
    return OnMouseWheel(new HEventInfo(hevent, false));
  }

  private int SwigDirectorOnKeyDown(IntPtr hevent) {
    return OnKeyDown(new HEventInfo(hevent, false));
  }

  private int SwigDirectorOnKeyUp(IntPtr hevent) {
    return OnKeyUp(new HEventInfo(hevent, false));
  }

  private int SwigDirectorOnTouchesDown(IntPtr hevent) {
    return OnTouchesDown(new HEventInfo(hevent, false));
  }

  private int SwigDirectorOnTouchesMove(IntPtr hevent) {
    return OnTouchesMove(new HEventInfo(hevent, false));
  }

  private int SwigDirectorOnTouchesUp(IntPtr hevent) {
    return OnTouchesUp(new HEventInfo(hevent, false));
  }

  private IntPtr SwigDirectorClone() {
    return HBaseOperator.getCPtr(Clone()).Handle;
  }

  private bool SwigDirectorOperatorStarted() {
    return OperatorStarted();
  }

  private string SwigDirectorGetName() {
    return GetName();
  }

  private int SwigDirectorOnLButtonDownAndMove(IntPtr hevent) {
    return OnLButtonDownAndMove(new HEventInfo(hevent, false));
  }

  private int SwigDirectorOnMButtonDownAndMove(IntPtr hevent) {
    return OnMButtonDownAndMove(new HEventInfo(hevent, false));
  }

  private int SwigDirectorOnRButtonDownAndMove(IntPtr hevent) {
    return OnRButtonDownAndMove(new HEventInfo(hevent, false));
  }

  private int SwigDirectorOnLRButtonDownAndMove(IntPtr hevent) {
    return OnLRButtonDownAndMove(new HEventInfo(hevent, false));
  }

  private int SwigDirectorOnNoButtonDownAndMove(IntPtr hevent) {
    return OnNoButtonDownAndMove(new HEventInfo(hevent, false));
  }

  private int SwigDirectorOnTimer(IntPtr hevent) {
    return OnTimer(new HEventInfo(hevent, false));
  }

  public delegate IntPtr SwigDelegateHOpConstructCone_0();
  public delegate int SwigDelegateHOpConstructCone_1(IntPtr hevent);
  public delegate int SwigDelegateHOpConstructCone_2(IntPtr hevent);
  public delegate int SwigDelegateHOpConstructCone_3(IntPtr hevent);
  public delegate int SwigDelegateHOpConstructCone_4(IntPtr hevent);
  public delegate int SwigDelegateHOpConstructCone_5(IntPtr hevent);
  public delegate int SwigDelegateHOpConstructCone_6(IntPtr hevent);
  public delegate int SwigDelegateHOpConstructCone_7(IntPtr hevent);
  public delegate int SwigDelegateHOpConstructCone_8(IntPtr hevent);
  public delegate int SwigDelegateHOpConstructCone_9(IntPtr hevent);
  public delegate int SwigDelegateHOpConstructCone_10(IntPtr hevent);
  public delegate int SwigDelegateHOpConstructCone_11(IntPtr hevent);
  public delegate int SwigDelegateHOpConstructCone_12(IntPtr hevent);
  public delegate int SwigDelegateHOpConstructCone_13(IntPtr hevent);
  public delegate int SwigDelegateHOpConstructCone_14(IntPtr hevent);
  public delegate int SwigDelegateHOpConstructCone_15(IntPtr hevent);
  public delegate int SwigDelegateHOpConstructCone_16(IntPtr hevent);
  public delegate IntPtr SwigDelegateHOpConstructCone_17();
  public delegate bool SwigDelegateHOpConstructCone_18();
  public delegate string SwigDelegateHOpConstructCone_19();
  public delegate int SwigDelegateHOpConstructCone_20(IntPtr hevent);
  public delegate int SwigDelegateHOpConstructCone_21(IntPtr hevent);
  public delegate int SwigDelegateHOpConstructCone_22(IntPtr hevent);
  public delegate int SwigDelegateHOpConstructCone_23(IntPtr hevent);
  public delegate int SwigDelegateHOpConstructCone_24(IntPtr hevent);
  public delegate int SwigDelegateHOpConstructCone_25(IntPtr hevent);

  private SwigDelegateHOpConstructCone_0 swigDelegate0;
  private SwigDelegateHOpConstructCone_1 swigDelegate1;
  private SwigDelegateHOpConstructCone_2 swigDelegate2;
  private SwigDelegateHOpConstructCone_3 swigDelegate3;
  private SwigDelegateHOpConstructCone_4 swigDelegate4;
  private SwigDelegateHOpConstructCone_5 swigDelegate5;
  private SwigDelegateHOpConstructCone_6 swigDelegate6;
  private SwigDelegateHOpConstructCone_7 swigDelegate7;
  private SwigDelegateHOpConstructCone_8 swigDelegate8;
  private SwigDelegateHOpConstructCone_9 swigDelegate9;
  private SwigDelegateHOpConstructCone_10 swigDelegate10;
  private SwigDelegateHOpConstructCone_11 swigDelegate11;
  private SwigDelegateHOpConstructCone_12 swigDelegate12;
  private SwigDelegateHOpConstructCone_13 swigDelegate13;
  private SwigDelegateHOpConstructCone_14 swigDelegate14;
  private SwigDelegateHOpConstructCone_15 swigDelegate15;
  private SwigDelegateHOpConstructCone_16 swigDelegate16;
  private SwigDelegateHOpConstructCone_17 swigDelegate17;
  private SwigDelegateHOpConstructCone_18 swigDelegate18;
  private SwigDelegateHOpConstructCone_19 swigDelegate19;
  private SwigDelegateHOpConstructCone_20 swigDelegate20;
  private SwigDelegateHOpConstructCone_21 swigDelegate21;
  private SwigDelegateHOpConstructCone_22 swigDelegate22;
  private SwigDelegateHOpConstructCone_23 swigDelegate23;
  private SwigDelegateHOpConstructCone_24 swigDelegate24;
  private SwigDelegateHOpConstructCone_25 swigDelegate25;

  private static Type[] swigMethodTypes0 = new Type[] {  };
  private static Type[] swigMethodTypes1 = new Type[] { typeof(HEventInfo) };
  private static Type[] swigMethodTypes2 = new Type[] { typeof(HEventInfo) };
  private static Type[] swigMethodTypes3 = new Type[] { typeof(HEventInfo) };
  private static Type[] swigMethodTypes4 = new Type[] { typeof(HEventInfo) };
  private static Type[] swigMethodTypes5 = new Type[] { typeof(HEventInfo) };
  private static Type[] swigMethodTypes6 = new Type[] { typeof(HEventInfo) };
  private static Type[] swigMethodTypes7 = new Type[] { typeof(HEventInfo) };
  private static Type[] swigMethodTypes8 = new Type[] { typeof(HEventInfo) };
  private static Type[] swigMethodTypes9 = new Type[] { typeof(HEventInfo) };
  private static Type[] swigMethodTypes10 = new Type[] { typeof(HEventInfo) };
  private static Type[] swigMethodTypes11 = new Type[] { typeof(HEventInfo) };
  private static Type[] swigMethodTypes12 = new Type[] { typeof(HEventInfo) };
  private static Type[] swigMethodTypes13 = new Type[] { typeof(HEventInfo) };
  private static Type[] swigMethodTypes14 = new Type[] { typeof(HEventInfo) };
  private static Type[] swigMethodTypes15 = new Type[] { typeof(HEventInfo) };
  private static Type[] swigMethodTypes16 = new Type[] { typeof(HEventInfo) };
  private static Type[] swigMethodTypes17 = new Type[] {  };
  private static Type[] swigMethodTypes18 = new Type[] {  };
  private static Type[] swigMethodTypes19 = new Type[] {  };
  private static Type[] swigMethodTypes20 = new Type[] { typeof(HEventInfo) };
  private static Type[] swigMethodTypes21 = new Type[] { typeof(HEventInfo) };
  private static Type[] swigMethodTypes22 = new Type[] { typeof(HEventInfo) };
  private static Type[] swigMethodTypes23 = new Type[] { typeof(HEventInfo) };
  private static Type[] swigMethodTypes24 = new Type[] { typeof(HEventInfo) };
  private static Type[] swigMethodTypes25 = new Type[] { typeof(HEventInfo) };
}