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


public class HOpCameraManipulate : HBaseOperator {
  private HandleRef swigCPtr;

  public HOpCameraManipulate(IntPtr cPtr, bool cMemoryOwn) : base(HCSMVOPINVOKE.HOpCameraManipulateUpcast(cPtr), cMemoryOwn) {
    swigCPtr = new HandleRef(this, cPtr);
  }

  public static HandleRef getCPtr(HOpCameraManipulate obj) {
    return (obj == null) ? new HandleRef(null, IntPtr.Zero) : obj.swigCPtr;
  }

  ~HOpCameraManipulate() {
    Dispose();
  }

  public override void Dispose() {
    lock(this) {
      if (swigCPtr.Handle != IntPtr.Zero) {
        if (swigCMemOwn) {
          swigCMemOwn = false;
          HCSMVOPINVOKE.delete_HOpCameraManipulate(swigCPtr);
        }
        swigCPtr = new HandleRef(null, IntPtr.Zero);
      }
      GC.SuppressFinalize(this);
      base.Dispose();
    }
  }

  public HOpCameraManipulate(HBaseView view, int DoRepeat, int DoCapture, HBaseOperator orbit, HBaseOperator pan, HBaseOperator zoom, HBaseOperator relative, bool AllowHandleActivation) : this(HCSMVOPINVOKE.new_HOpCameraManipulate__SWIG_0(HBaseView.getCPtr(view), DoRepeat, DoCapture, HBaseOperator.getCPtr(orbit), HBaseOperator.getCPtr(pan), HBaseOperator.getCPtr(zoom), HBaseOperator.getCPtr(relative), AllowHandleActivation), true) {
    SwigDirectorConnect();
  }

  public HOpCameraManipulate(HBaseView view, int DoRepeat, int DoCapture, HBaseOperator orbit, HBaseOperator pan, HBaseOperator zoom, HBaseOperator relative) : this(HCSMVOPINVOKE.new_HOpCameraManipulate__SWIG_1(HBaseView.getCPtr(view), DoRepeat, DoCapture, HBaseOperator.getCPtr(orbit), HBaseOperator.getCPtr(pan), HBaseOperator.getCPtr(zoom), HBaseOperator.getCPtr(relative)), true) {
    SwigDirectorConnect();
  }

  public HOpCameraManipulate(HBaseView view, int DoRepeat, int DoCapture, HBaseOperator orbit, HBaseOperator pan, HBaseOperator zoom) : this(HCSMVOPINVOKE.new_HOpCameraManipulate__SWIG_2(HBaseView.getCPtr(view), DoRepeat, DoCapture, HBaseOperator.getCPtr(orbit), HBaseOperator.getCPtr(pan), HBaseOperator.getCPtr(zoom)), true) {
    SwigDirectorConnect();
  }

  public HOpCameraManipulate(HBaseView view, int DoRepeat, int DoCapture, HBaseOperator orbit, HBaseOperator pan) : this(HCSMVOPINVOKE.new_HOpCameraManipulate__SWIG_3(HBaseView.getCPtr(view), DoRepeat, DoCapture, HBaseOperator.getCPtr(orbit), HBaseOperator.getCPtr(pan)), true) {
    SwigDirectorConnect();
  }

  public HOpCameraManipulate(HBaseView view, int DoRepeat, int DoCapture, HBaseOperator orbit) : this(HCSMVOPINVOKE.new_HOpCameraManipulate__SWIG_4(HBaseView.getCPtr(view), DoRepeat, DoCapture, HBaseOperator.getCPtr(orbit)), true) {
    SwigDirectorConnect();
  }

  public HOpCameraManipulate(HBaseView view, int DoRepeat, int DoCapture) : this(HCSMVOPINVOKE.new_HOpCameraManipulate__SWIG_5(HBaseView.getCPtr(view), DoRepeat, DoCapture), true) {
    SwigDirectorConnect();
  }

  public HOpCameraManipulate(HBaseView view, int DoRepeat) : this(HCSMVOPINVOKE.new_HOpCameraManipulate__SWIG_6(HBaseView.getCPtr(view), DoRepeat), true) {
    SwigDirectorConnect();
  }

  public HOpCameraManipulate(HBaseView view) : this(HCSMVOPINVOKE.new_HOpCameraManipulate__SWIG_7(HBaseView.getCPtr(view)), true) {
    SwigDirectorConnect();
  }

  public override string GetName() {
    string ret = ((this.GetType() == typeof(HOpCameraManipulate)) ? HCSMVOPINVOKE.HOpCameraManipulate_GetName(swigCPtr) : HCSMVOPINVOKE.HOpCameraManipulate_GetNameSwigExplicitHOpCameraManipulate(swigCPtr));
    return ret;
  }

  public override int OnLButtonDown(HEventInfo hevent) {
    int ret = ((this.GetType() == typeof(HOpCameraManipulate)) ? HCSMVOPINVOKE.HOpCameraManipulate_OnLButtonDown(swigCPtr, HEventInfo.getCPtr(hevent)) : HCSMVOPINVOKE.HOpCameraManipulate_OnLButtonDownSwigExplicitHOpCameraManipulate(swigCPtr, HEventInfo.getCPtr(hevent)));
    if (HCSMVOPINVOKE.SWIGPendingException.Pending) throw HCSMVOPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public override int OnLButtonDownAndMove(HEventInfo hevent) {
    int ret = ((this.GetType() == typeof(HOpCameraManipulate)) ? HCSMVOPINVOKE.HOpCameraManipulate_OnLButtonDownAndMove(swigCPtr, HEventInfo.getCPtr(hevent)) : HCSMVOPINVOKE.HOpCameraManipulate_OnLButtonDownAndMoveSwigExplicitHOpCameraManipulate(swigCPtr, HEventInfo.getCPtr(hevent)));
    if (HCSMVOPINVOKE.SWIGPendingException.Pending) throw HCSMVOPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public override int OnLButtonUp(HEventInfo hevent) {
    int ret = ((this.GetType() == typeof(HOpCameraManipulate)) ? HCSMVOPINVOKE.HOpCameraManipulate_OnLButtonUp(swigCPtr, HEventInfo.getCPtr(hevent)) : HCSMVOPINVOKE.HOpCameraManipulate_OnLButtonUpSwigExplicitHOpCameraManipulate(swigCPtr, HEventInfo.getCPtr(hevent)));
    if (HCSMVOPINVOKE.SWIGPendingException.Pending) throw HCSMVOPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public override int OnMButtonDown(HEventInfo hevent) {
    int ret = ((this.GetType() == typeof(HOpCameraManipulate)) ? HCSMVOPINVOKE.HOpCameraManipulate_OnMButtonDown(swigCPtr, HEventInfo.getCPtr(hevent)) : HCSMVOPINVOKE.HOpCameraManipulate_OnMButtonDownSwigExplicitHOpCameraManipulate(swigCPtr, HEventInfo.getCPtr(hevent)));
    if (HCSMVOPINVOKE.SWIGPendingException.Pending) throw HCSMVOPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public override int OnMButtonDownAndMove(HEventInfo hevent) {
    int ret = ((this.GetType() == typeof(HOpCameraManipulate)) ? HCSMVOPINVOKE.HOpCameraManipulate_OnMButtonDownAndMove(swigCPtr, HEventInfo.getCPtr(hevent)) : HCSMVOPINVOKE.HOpCameraManipulate_OnMButtonDownAndMoveSwigExplicitHOpCameraManipulate(swigCPtr, HEventInfo.getCPtr(hevent)));
    if (HCSMVOPINVOKE.SWIGPendingException.Pending) throw HCSMVOPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public override int OnMButtonUp(HEventInfo hevent) {
    int ret = ((this.GetType() == typeof(HOpCameraManipulate)) ? HCSMVOPINVOKE.HOpCameraManipulate_OnMButtonUp(swigCPtr, HEventInfo.getCPtr(hevent)) : HCSMVOPINVOKE.HOpCameraManipulate_OnMButtonUpSwigExplicitHOpCameraManipulate(swigCPtr, HEventInfo.getCPtr(hevent)));
    if (HCSMVOPINVOKE.SWIGPendingException.Pending) throw HCSMVOPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public override int OnRButtonDown(HEventInfo hevent) {
    int ret = ((this.GetType() == typeof(HOpCameraManipulate)) ? HCSMVOPINVOKE.HOpCameraManipulate_OnRButtonDown(swigCPtr, HEventInfo.getCPtr(hevent)) : HCSMVOPINVOKE.HOpCameraManipulate_OnRButtonDownSwigExplicitHOpCameraManipulate(swigCPtr, HEventInfo.getCPtr(hevent)));
    if (HCSMVOPINVOKE.SWIGPendingException.Pending) throw HCSMVOPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public override int OnRButtonDownAndMove(HEventInfo hevent) {
    int ret = ((this.GetType() == typeof(HOpCameraManipulate)) ? HCSMVOPINVOKE.HOpCameraManipulate_OnRButtonDownAndMove(swigCPtr, HEventInfo.getCPtr(hevent)) : HCSMVOPINVOKE.HOpCameraManipulate_OnRButtonDownAndMoveSwigExplicitHOpCameraManipulate(swigCPtr, HEventInfo.getCPtr(hevent)));
    if (HCSMVOPINVOKE.SWIGPendingException.Pending) throw HCSMVOPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public override int OnRButtonUp(HEventInfo hevent) {
    int ret = ((this.GetType() == typeof(HOpCameraManipulate)) ? HCSMVOPINVOKE.HOpCameraManipulate_OnRButtonUp(swigCPtr, HEventInfo.getCPtr(hevent)) : HCSMVOPINVOKE.HOpCameraManipulate_OnRButtonUpSwigExplicitHOpCameraManipulate(swigCPtr, HEventInfo.getCPtr(hevent)));
    if (HCSMVOPINVOKE.SWIGPendingException.Pending) throw HCSMVOPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public override HBaseOperator Clone() {
    IntPtr cPtr = ((this.GetType() == typeof(HOpCameraManipulate)) ? HCSMVOPINVOKE.HOpCameraManipulate_Clone(swigCPtr) : HCSMVOPINVOKE.HOpCameraManipulate_CloneSwigExplicitHOpCameraManipulate(swigCPtr));
    HBaseOperator ret = (cPtr == IntPtr.Zero) ? null : new HBaseOperator(cPtr, false);
    return ret;
  }

  public override bool OperatorStarted() {
    bool ret = ((this.GetType() == typeof(HOpCameraManipulate)) ? HCSMVOPINVOKE.HOpCameraManipulate_OperatorStarted(swigCPtr) : HCSMVOPINVOKE.HOpCameraManipulate_OperatorStartedSwigExplicitHOpCameraManipulate(swigCPtr));
    return ret;
  }

  public HOpCameraOrbit GetOrbitOperator() {
    IntPtr cPtr = HCSMVOPINVOKE.HOpCameraManipulate_GetOrbitOperator(swigCPtr);
    HOpCameraOrbit ret = (cPtr == IntPtr.Zero) ? null : new HOpCameraOrbit(cPtr, false);
    return ret;
  }

  public void HandleDblClk(float requested_time, float actual_time) {
    HCSMVOPINVOKE.HOpCameraManipulate_HandleDblClk(swigCPtr, requested_time, actual_time);
  }

  public bool GetLButtonDown() {
    bool ret = HCSMVOPINVOKE.HOpCameraManipulate_GetLButtonDown(swigCPtr);
    return ret;
  }

  protected HTClient m_pDblClkTimer {
    set {
      HCSMVOPINVOKE.HOpCameraManipulate_m_pDblClkTimer_set(swigCPtr, HTClient.getCPtr(value));
    } 
    get {
      IntPtr cPtr = HCSMVOPINVOKE.HOpCameraManipulate_m_pDblClkTimer_get(swigCPtr);
      HTClient ret = (cPtr == IntPtr.Zero) ? null : new HTClient(cPtr, false);
      return ret;
    } 
  }

  protected HBaseOperator m_pOrbitOperator {
    set {
      HCSMVOPINVOKE.HOpCameraManipulate_m_pOrbitOperator_set(swigCPtr, HBaseOperator.getCPtr(value));
    } 
    get {
      IntPtr cPtr = HCSMVOPINVOKE.HOpCameraManipulate_m_pOrbitOperator_get(swigCPtr);
      HBaseOperator ret = (cPtr == IntPtr.Zero) ? null : new HBaseOperator(cPtr, false);
      return ret;
    } 
  }

  protected HBaseOperator m_pRelativeOrbitOperator {
    set {
      HCSMVOPINVOKE.HOpCameraManipulate_m_pRelativeOrbitOperator_set(swigCPtr, HBaseOperator.getCPtr(value));
    } 
    get {
      IntPtr cPtr = HCSMVOPINVOKE.HOpCameraManipulate_m_pRelativeOrbitOperator_get(swigCPtr);
      HBaseOperator ret = (cPtr == IntPtr.Zero) ? null : new HBaseOperator(cPtr, false);
      return ret;
    } 
  }

  protected HBaseOperator m_pPanOperator {
    set {
      HCSMVOPINVOKE.HOpCameraManipulate_m_pPanOperator_set(swigCPtr, HBaseOperator.getCPtr(value));
    } 
    get {
      IntPtr cPtr = HCSMVOPINVOKE.HOpCameraManipulate_m_pPanOperator_get(swigCPtr);
      HBaseOperator ret = (cPtr == IntPtr.Zero) ? null : new HBaseOperator(cPtr, false);
      return ret;
    } 
  }

  protected HBaseOperator m_pZoomOperator {
    set {
      HCSMVOPINVOKE.HOpCameraManipulate_m_pZoomOperator_set(swigCPtr, HBaseOperator.getCPtr(value));
    } 
    get {
      IntPtr cPtr = HCSMVOPINVOKE.HOpCameraManipulate_m_pZoomOperator_get(swigCPtr);
      HBaseOperator ret = (cPtr == IntPtr.Zero) ? null : new HBaseOperator(cPtr, false);
      return ret;
    } 
  }

  protected HPoint m_ptRealOld {
    set {
      HCSMVOPINVOKE.HOpCameraManipulate_m_ptRealOld_set(swigCPtr, HPoint.getCPtr(value));
    } 
    get {
      IntPtr cPtr = HCSMVOPINVOKE.HOpCameraManipulate_m_ptRealOld_get(swigCPtr);
      HPoint ret = (cPtr == IntPtr.Zero) ? null : new HPoint(cPtr, false);
      return ret;
    } 
  }

  private void SwigDirectorConnect() {
    if (SwigDerivedClassHasMethod("CreateListenerManager", swigMethodTypes0))
      swigDelegate0 = new SwigDelegateHOpCameraManipulate_0(SwigDirectorCreateListenerManager);
    if (SwigDerivedClassHasMethod("OnLButtonDown", swigMethodTypes1))
      swigDelegate1 = new SwigDelegateHOpCameraManipulate_1(SwigDirectorOnLButtonDown);
    if (SwigDerivedClassHasMethod("OnMouseMove", swigMethodTypes2))
      swigDelegate2 = new SwigDelegateHOpCameraManipulate_2(SwigDirectorOnMouseMove);
    if (SwigDerivedClassHasMethod("OnLButtonUp", swigMethodTypes3))
      swigDelegate3 = new SwigDelegateHOpCameraManipulate_3(SwigDirectorOnLButtonUp);
    if (SwigDerivedClassHasMethod("OnLButtonDblClk", swigMethodTypes4))
      swigDelegate4 = new SwigDelegateHOpCameraManipulate_4(SwigDirectorOnLButtonDblClk);
    if (SwigDerivedClassHasMethod("OnMButtonDown", swigMethodTypes5))
      swigDelegate5 = new SwigDelegateHOpCameraManipulate_5(SwigDirectorOnMButtonDown);
    if (SwigDerivedClassHasMethod("OnMButtonUp", swigMethodTypes6))
      swigDelegate6 = new SwigDelegateHOpCameraManipulate_6(SwigDirectorOnMButtonUp);
    if (SwigDerivedClassHasMethod("OnMButtonDblClk", swigMethodTypes7))
      swigDelegate7 = new SwigDelegateHOpCameraManipulate_7(SwigDirectorOnMButtonDblClk);
    if (SwigDerivedClassHasMethod("OnRButtonDown", swigMethodTypes8))
      swigDelegate8 = new SwigDelegateHOpCameraManipulate_8(SwigDirectorOnRButtonDown);
    if (SwigDerivedClassHasMethod("OnRButtonUp", swigMethodTypes9))
      swigDelegate9 = new SwigDelegateHOpCameraManipulate_9(SwigDirectorOnRButtonUp);
    if (SwigDerivedClassHasMethod("OnRButtonDblClk", swigMethodTypes10))
      swigDelegate10 = new SwigDelegateHOpCameraManipulate_10(SwigDirectorOnRButtonDblClk);
    if (SwigDerivedClassHasMethod("OnMouseWheel", swigMethodTypes11))
      swigDelegate11 = new SwigDelegateHOpCameraManipulate_11(SwigDirectorOnMouseWheel);
    if (SwigDerivedClassHasMethod("OnKeyDown", swigMethodTypes12))
      swigDelegate12 = new SwigDelegateHOpCameraManipulate_12(SwigDirectorOnKeyDown);
    if (SwigDerivedClassHasMethod("OnKeyUp", swigMethodTypes13))
      swigDelegate13 = new SwigDelegateHOpCameraManipulate_13(SwigDirectorOnKeyUp);
    if (SwigDerivedClassHasMethod("OnTouchesDown", swigMethodTypes14))
      swigDelegate14 = new SwigDelegateHOpCameraManipulate_14(SwigDirectorOnTouchesDown);
    if (SwigDerivedClassHasMethod("OnTouchesMove", swigMethodTypes15))
      swigDelegate15 = new SwigDelegateHOpCameraManipulate_15(SwigDirectorOnTouchesMove);
    if (SwigDerivedClassHasMethod("OnTouchesUp", swigMethodTypes16))
      swigDelegate16 = new SwigDelegateHOpCameraManipulate_16(SwigDirectorOnTouchesUp);
    if (SwigDerivedClassHasMethod("Clone", swigMethodTypes17))
      swigDelegate17 = new SwigDelegateHOpCameraManipulate_17(SwigDirectorClone);
    if (SwigDerivedClassHasMethod("OperatorStarted", swigMethodTypes18))
      swigDelegate18 = new SwigDelegateHOpCameraManipulate_18(SwigDirectorOperatorStarted);
    if (SwigDerivedClassHasMethod("GetName", swigMethodTypes19))
      swigDelegate19 = new SwigDelegateHOpCameraManipulate_19(SwigDirectorGetName);
    if (SwigDerivedClassHasMethod("OnLButtonDownAndMove", swigMethodTypes20))
      swigDelegate20 = new SwigDelegateHOpCameraManipulate_20(SwigDirectorOnLButtonDownAndMove);
    if (SwigDerivedClassHasMethod("OnMButtonDownAndMove", swigMethodTypes21))
      swigDelegate21 = new SwigDelegateHOpCameraManipulate_21(SwigDirectorOnMButtonDownAndMove);
    if (SwigDerivedClassHasMethod("OnRButtonDownAndMove", swigMethodTypes22))
      swigDelegate22 = new SwigDelegateHOpCameraManipulate_22(SwigDirectorOnRButtonDownAndMove);
    if (SwigDerivedClassHasMethod("OnLRButtonDownAndMove", swigMethodTypes23))
      swigDelegate23 = new SwigDelegateHOpCameraManipulate_23(SwigDirectorOnLRButtonDownAndMove);
    if (SwigDerivedClassHasMethod("OnNoButtonDownAndMove", swigMethodTypes24))
      swigDelegate24 = new SwigDelegateHOpCameraManipulate_24(SwigDirectorOnNoButtonDownAndMove);
    if (SwigDerivedClassHasMethod("OnTimer", swigMethodTypes25))
      swigDelegate25 = new SwigDelegateHOpCameraManipulate_25(SwigDirectorOnTimer);
    HCSMVOPINVOKE.HOpCameraManipulate_director_connect(swigCPtr, swigDelegate0, swigDelegate1, swigDelegate2, swigDelegate3, swigDelegate4, swigDelegate5, swigDelegate6, swigDelegate7, swigDelegate8, swigDelegate9, swigDelegate10, swigDelegate11, swigDelegate12, swigDelegate13, swigDelegate14, swigDelegate15, swigDelegate16, swigDelegate17, swigDelegate18, swigDelegate19, swigDelegate20, swigDelegate21, swigDelegate22, swigDelegate23, swigDelegate24, swigDelegate25);
  }

  private bool SwigDerivedClassHasMethod(string methodName, Type[] methodTypes) {
    System.Reflection.MethodInfo methodInfo = this.GetType().GetMethod(methodName, System.Reflection.BindingFlags.Public | System.Reflection.BindingFlags.NonPublic | System.Reflection.BindingFlags.Instance, null, methodTypes, null);
    bool hasDerivedMethod = methodInfo.DeclaringType.IsSubclassOf(typeof(HOpCameraManipulate));
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

  public delegate IntPtr SwigDelegateHOpCameraManipulate_0();
  public delegate int SwigDelegateHOpCameraManipulate_1(IntPtr hevent);
  public delegate int SwigDelegateHOpCameraManipulate_2(IntPtr hevent);
  public delegate int SwigDelegateHOpCameraManipulate_3(IntPtr hevent);
  public delegate int SwigDelegateHOpCameraManipulate_4(IntPtr hevent);
  public delegate int SwigDelegateHOpCameraManipulate_5(IntPtr hevent);
  public delegate int SwigDelegateHOpCameraManipulate_6(IntPtr hevent);
  public delegate int SwigDelegateHOpCameraManipulate_7(IntPtr hevent);
  public delegate int SwigDelegateHOpCameraManipulate_8(IntPtr hevent);
  public delegate int SwigDelegateHOpCameraManipulate_9(IntPtr hevent);
  public delegate int SwigDelegateHOpCameraManipulate_10(IntPtr hevent);
  public delegate int SwigDelegateHOpCameraManipulate_11(IntPtr hevent);
  public delegate int SwigDelegateHOpCameraManipulate_12(IntPtr hevent);
  public delegate int SwigDelegateHOpCameraManipulate_13(IntPtr hevent);
  public delegate int SwigDelegateHOpCameraManipulate_14(IntPtr hevent);
  public delegate int SwigDelegateHOpCameraManipulate_15(IntPtr hevent);
  public delegate int SwigDelegateHOpCameraManipulate_16(IntPtr hevent);
  public delegate IntPtr SwigDelegateHOpCameraManipulate_17();
  public delegate bool SwigDelegateHOpCameraManipulate_18();
  public delegate string SwigDelegateHOpCameraManipulate_19();
  public delegate int SwigDelegateHOpCameraManipulate_20(IntPtr hevent);
  public delegate int SwigDelegateHOpCameraManipulate_21(IntPtr hevent);
  public delegate int SwigDelegateHOpCameraManipulate_22(IntPtr hevent);
  public delegate int SwigDelegateHOpCameraManipulate_23(IntPtr hevent);
  public delegate int SwigDelegateHOpCameraManipulate_24(IntPtr hevent);
  public delegate int SwigDelegateHOpCameraManipulate_25(IntPtr hevent);

  private SwigDelegateHOpCameraManipulate_0 swigDelegate0;
  private SwigDelegateHOpCameraManipulate_1 swigDelegate1;
  private SwigDelegateHOpCameraManipulate_2 swigDelegate2;
  private SwigDelegateHOpCameraManipulate_3 swigDelegate3;
  private SwigDelegateHOpCameraManipulate_4 swigDelegate4;
  private SwigDelegateHOpCameraManipulate_5 swigDelegate5;
  private SwigDelegateHOpCameraManipulate_6 swigDelegate6;
  private SwigDelegateHOpCameraManipulate_7 swigDelegate7;
  private SwigDelegateHOpCameraManipulate_8 swigDelegate8;
  private SwigDelegateHOpCameraManipulate_9 swigDelegate9;
  private SwigDelegateHOpCameraManipulate_10 swigDelegate10;
  private SwigDelegateHOpCameraManipulate_11 swigDelegate11;
  private SwigDelegateHOpCameraManipulate_12 swigDelegate12;
  private SwigDelegateHOpCameraManipulate_13 swigDelegate13;
  private SwigDelegateHOpCameraManipulate_14 swigDelegate14;
  private SwigDelegateHOpCameraManipulate_15 swigDelegate15;
  private SwigDelegateHOpCameraManipulate_16 swigDelegate16;
  private SwigDelegateHOpCameraManipulate_17 swigDelegate17;
  private SwigDelegateHOpCameraManipulate_18 swigDelegate18;
  private SwigDelegateHOpCameraManipulate_19 swigDelegate19;
  private SwigDelegateHOpCameraManipulate_20 swigDelegate20;
  private SwigDelegateHOpCameraManipulate_21 swigDelegate21;
  private SwigDelegateHOpCameraManipulate_22 swigDelegate22;
  private SwigDelegateHOpCameraManipulate_23 swigDelegate23;
  private SwigDelegateHOpCameraManipulate_24 swigDelegate24;
  private SwigDelegateHOpCameraManipulate_25 swigDelegate25;

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