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


public class HMarkupManager : IDisposable {
  private HandleRef swigCPtr;
  protected bool swigCMemOwn;

  public HMarkupManager(IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new HandleRef(this, cPtr);
  }

  public static HandleRef getCPtr(HMarkupManager obj) {
    return (obj == null) ? new HandleRef(null, IntPtr.Zero) : obj.swigCPtr;
  }
  
  public void owns_memory(bool ownMemory) {
	swigCMemOwn = ownMemory;
  }

  ~HMarkupManager() {
    Dispose();
  }

  public virtual void Dispose() {
    lock(this) {
      if (swigCPtr.Handle != IntPtr.Zero) {
        if (swigCMemOwn) {
          swigCMemOwn = false;
          HCSMVOPINVOKE.delete_HMarkupManager(swigCPtr);
        }
        swigCPtr = new HandleRef(null, IntPtr.Zero);
      }
      GC.SuppressFinalize(this);
    }
  }

  public HMarkupManager(HBaseView view) : this(HCSMVOPINVOKE.new_HMarkupManager(HBaseView.getCPtr(view)), true) {
    SwigDirectorConnect();
  }

  public void ReInit(HBaseView view) {
    HCSMVOPINVOKE.HMarkupManager_ReInit(swigCPtr, HBaseView.getCPtr(view));
  }

  public virtual MVO_POINTER_SIZED_INT OpenLayer(string layername, bool setcamera, bool emit_message) {return ((this.GetType() == typeof(HMarkupManager)) ? HCSMVOPINVOKE.HMarkupManager_OpenLayer__SWIG_0(swigCPtr, layername, setcamera, emit_message) : HCSMVOPINVOKE.HMarkupManager_OpenLayerSwigExplicitHMarkupManager__SWIG_0(swigCPtr, layername, setcamera, emit_message));}

  public virtual MVO_POINTER_SIZED_INT OpenLayer(string layername, bool setcamera) {return ((this.GetType() == typeof(HMarkupManager)) ? HCSMVOPINVOKE.HMarkupManager_OpenLayer__SWIG_1(swigCPtr, layername, setcamera) : HCSMVOPINVOKE.HMarkupManager_OpenLayerSwigExplicitHMarkupManager__SWIG_1(swigCPtr, layername, setcamera));}

  public virtual MVO_POINTER_SIZED_INT OpenLayer(string layername) {return ((this.GetType() == typeof(HMarkupManager)) ? HCSMVOPINVOKE.HMarkupManager_OpenLayer__SWIG_2(swigCPtr, layername) : HCSMVOPINVOKE.HMarkupManager_OpenLayerSwigExplicitHMarkupManager__SWIG_2(swigCPtr, layername));}

  public virtual MVO_POINTER_SIZED_INT RenameLayer(MVO_POINTER_SIZED_INT laykey, string new_name) {return ((this.GetType() == typeof(HMarkupManager)) ? HCSMVOPINVOKE.HMarkupManager_RenameLayer(swigCPtr, laykey, new_name) : HCSMVOPINVOKE.HMarkupManager_RenameLayerSwigExplicitHMarkupManager(swigCPtr, laykey, new_name));}

  public virtual MVO_POINTER_SIZED_INT CopyLayer(MVO_POINTER_SIZED_INT laykey, string new_name) {return ((this.GetType() == typeof(HMarkupManager)) ? HCSMVOPINVOKE.HMarkupManager_CopyLayer(swigCPtr, laykey, new_name) : HCSMVOPINVOKE.HMarkupManager_CopyLayerSwigExplicitHMarkupManager(swigCPtr, laykey, new_name));}

  public virtual bool GetCamera(MVO_POINTER_SIZED_INT layerkey, HCamera cam) {
    bool ret = ((this.GetType() == typeof(HMarkupManager)) ? HCSMVOPINVOKE.HMarkupManager_GetCamera(swigCPtr, layerkey, HCamera.getCPtr(cam)) : HCSMVOPINVOKE.HMarkupManager_GetCameraSwigExplicitHMarkupManager(swigCPtr, layerkey, HCamera.getCPtr(cam)));
    if (HCSMVOPINVOKE.SWIGPendingException.Pending) throw HCSMVOPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public virtual MVO_POINTER_SIZED_INT OpenLayer(MVO_POINTER_SIZED_INT layerkey, bool setcamera) {return ((this.GetType() == typeof(HMarkupManager)) ? HCSMVOPINVOKE.HMarkupManager_OpenLayer__SWIG_3(swigCPtr, layerkey, setcamera) : HCSMVOPINVOKE.HMarkupManager_OpenLayerSwigExplicitHMarkupManager__SWIG_3(swigCPtr, layerkey, setcamera));}

  public virtual MVO_POINTER_SIZED_INT OpenLayer(MVO_POINTER_SIZED_INT layerkey) {return ((this.GetType() == typeof(HMarkupManager)) ? HCSMVOPINVOKE.HMarkupManager_OpenLayer__SWIG_4(swigCPtr, layerkey) : HCSMVOPINVOKE.HMarkupManager_OpenLayerSwigExplicitHMarkupManager__SWIG_4(swigCPtr, layerkey));}

  public virtual MVO_POINTER_SIZED_INT FindLayer(string layername) {return ((this.GetType() == typeof(HMarkupManager)) ? HCSMVOPINVOKE.HMarkupManager_FindLayer(swigCPtr, layername) : HCSMVOPINVOKE.HMarkupManager_FindLayerSwigExplicitHMarkupManager(swigCPtr, layername));}

  public virtual string GetLayerNames() {
    string ret = ((this.GetType() == typeof(HMarkupManager)) ? HCSMVOPINVOKE.HMarkupManager_GetLayerNames(swigCPtr) : HCSMVOPINVOKE.HMarkupManager_GetLayerNamesSwigExplicitHMarkupManager(swigCPtr));
    return ret;
  }

  public virtual MVO_POINTER_SIZED_INT GetCurrentLayerKey() {return ((this.GetType() == typeof(HMarkupManager)) ? HCSMVOPINVOKE.HMarkupManager_GetCurrentLayerKey(swigCPtr) : HCSMVOPINVOKE.HMarkupManager_GetCurrentLayerKeySwigExplicitHMarkupManager(swigCPtr));}

  public virtual string GetCurrentLayerName() {
    string ret = ((this.GetType() == typeof(HMarkupManager)) ? HCSMVOPINVOKE.HMarkupManager_GetCurrentLayerName(swigCPtr) : HCSMVOPINVOKE.HMarkupManager_GetCurrentLayerNameSwigExplicitHMarkupManager(swigCPtr));
    return ret;
  }

  public virtual void DisableCurrentLayer(bool emit_message) {
    if (this.GetType() == typeof(HMarkupManager)) HCSMVOPINVOKE.HMarkupManager_DisableCurrentLayer__SWIG_0(swigCPtr, emit_message); else HCSMVOPINVOKE.HMarkupManager_DisableCurrentLayerSwigExplicitHMarkupManager__SWIG_0(swigCPtr, emit_message);
  }

  public virtual void DisableCurrentLayer() {
    if (this.GetType() == typeof(HMarkupManager)) HCSMVOPINVOKE.HMarkupManager_DisableCurrentLayer__SWIG_1(swigCPtr); else HCSMVOPINVOKE.HMarkupManager_DisableCurrentLayerSwigExplicitHMarkupManager__SWIG_1(swigCPtr);
  }

  public virtual void DeleteLayer(string layername) {
    if (this.GetType() == typeof(HMarkupManager)) HCSMVOPINVOKE.HMarkupManager_DeleteLayer__SWIG_0(swigCPtr, layername); else HCSMVOPINVOKE.HMarkupManager_DeleteLayerSwigExplicitHMarkupManager__SWIG_0(swigCPtr, layername);
  }

  public virtual void DeleteLayer(MVO_POINTER_SIZED_INT layerkey) {
    if (this.GetType() == typeof(HMarkupManager)) HCSMVOPINVOKE.HMarkupManager_DeleteLayer__SWIG_1(swigCPtr, layerkey); else HCSMVOPINVOKE.HMarkupManager_DeleteLayerSwigExplicitHMarkupManager__SWIG_1(swigCPtr, layerkey);
  }

  public void SetDefaultLayerKey(MVO_POINTER_SIZED_INT layerkey) {
    HCSMVOPINVOKE.HMarkupManager_SetDefaultLayerKey(swigCPtr, layerkey);
  }

  public MVO_POINTER_SIZED_INT GetDefaultLayerKey() {return HCSMVOPINVOKE.HMarkupManager_GetDefaultLayerKey(swigCPtr);}

  public MVO_POINTER_SIZED_INT GetMarkupKey() {return HCSMVOPINVOKE.HMarkupManager_GetMarkupKey(swigCPtr);}

  public MVO_POINTER_SIZED_INT GetMarkupLayerKey() {return HCSMVOPINVOKE.HMarkupManager_GetMarkupLayerKey(swigCPtr);}

  public virtual float GetMarkupWeight() {
    float ret = ((this.GetType() == typeof(HMarkupManager)) ? HCSMVOPINVOKE.HMarkupManager_GetMarkupWeight(swigCPtr) : HCSMVOPINVOKE.HMarkupManager_GetMarkupWeightSwigExplicitHMarkupManager(swigCPtr));
    return ret;
  }

  public virtual void SetMarkupWeight(float weight) {
    if (this.GetType() == typeof(HMarkupManager)) HCSMVOPINVOKE.HMarkupManager_SetMarkupWeight(swigCPtr, weight); else HCSMVOPINVOKE.HMarkupManager_SetMarkupWeightSwigExplicitHMarkupManager(swigCPtr, weight);
  }

  public virtual void GetMarkupColor(HPoint markup_color) {
    if (this.GetType() == typeof(HMarkupManager)) HCSMVOPINVOKE.HMarkupManager_GetMarkupColor(swigCPtr, HPoint.getCPtr(markup_color)); else HCSMVOPINVOKE.HMarkupManager_GetMarkupColorSwigExplicitHMarkupManager(swigCPtr, HPoint.getCPtr(markup_color));
    if (HCSMVOPINVOKE.SWIGPendingException.Pending) throw HCSMVOPINVOKE.SWIGPendingException.Retrieve();
  }

  public virtual void SetMarkupColor(HPoint markup_color) {
    if (this.GetType() == typeof(HMarkupManager)) HCSMVOPINVOKE.HMarkupManager_SetMarkupColor(swigCPtr, HPoint.getCPtr(markup_color)); else HCSMVOPINVOKE.HMarkupManager_SetMarkupColorSwigExplicitHMarkupManager(swigCPtr, HPoint.getCPtr(markup_color));
    if (HCSMVOPINVOKE.SWIGPendingException.Pending) throw HCSMVOPINVOKE.SWIGPendingException.Retrieve();
  }

  public virtual void EmitMarkupSegment(MVO_POINTER_SIZED_INT key) {
    if (this.GetType() == typeof(HMarkupManager)) HCSMVOPINVOKE.HMarkupManager_EmitMarkupSegment(swigCPtr, key); else HCSMVOPINVOKE.HMarkupManager_EmitMarkupSegmentSwigExplicitHMarkupManager(swigCPtr, key);
  }

  public virtual void InsertMarkupSegmentFromMessage(string in_data, uint data_length) {
    if (this.GetType() == typeof(HMarkupManager)) HCSMVOPINVOKE.HMarkupManager_InsertMarkupSegmentFromMessage(swigCPtr, in_data, data_length); else HCSMVOPINVOKE.HMarkupManager_InsertMarkupSegmentFromMessageSwigExplicitHMarkupManager(swigCPtr, in_data, data_length);
  }

  public virtual void EmitDisableCurrentMarkupLayer() {
    if (this.GetType() == typeof(HMarkupManager)) HCSMVOPINVOKE.HMarkupManager_EmitDisableCurrentMarkupLayer(swigCPtr); else HCSMVOPINVOKE.HMarkupManager_EmitDisableCurrentMarkupLayerSwigExplicitHMarkupManager(swigCPtr);
  }

  public virtual void DisableCurrentMarkupLayerFromMessage(string data, uint data_length) {
    if (this.GetType() == typeof(HMarkupManager)) HCSMVOPINVOKE.HMarkupManager_DisableCurrentMarkupLayerFromMessage(swigCPtr, data, data_length); else HCSMVOPINVOKE.HMarkupManager_DisableCurrentMarkupLayerFromMessageSwigExplicitHMarkupManager(swigCPtr, data, data_length);
  }

  public virtual void EmitSetMarkupLayer(string layer) {
    if (this.GetType() == typeof(HMarkupManager)) HCSMVOPINVOKE.HMarkupManager_EmitSetMarkupLayer(swigCPtr, layer); else HCSMVOPINVOKE.HMarkupManager_EmitSetMarkupLayerSwigExplicitHMarkupManager(swigCPtr, layer);
  }

  public virtual void SetMarkupLayerFromMessage(string data, uint data_length) {
    if (this.GetType() == typeof(HMarkupManager)) HCSMVOPINVOKE.HMarkupManager_SetMarkupLayerFromMessage(swigCPtr, data, data_length); else HCSMVOPINVOKE.HMarkupManager_SetMarkupLayerFromMessageSwigExplicitHMarkupManager(swigCPtr, data, data_length);
  }

  public virtual bool GetPreviewImage(string arg0, int[] arg1, int[] arg2, int[] arg3, out IntPtr arg4) {
    bool ret = HCSMVOPINVOKE.HMarkupManager_GetPreviewImage(swigCPtr, arg0, arg1, arg2, arg3, out arg4);
    return ret;
  }

  public virtual void UpdatePreviewImage(bool forceUpdate) {
    if (this.GetType() == typeof(HMarkupManager)) HCSMVOPINVOKE.HMarkupManager_UpdatePreviewImage__SWIG_0(swigCPtr, forceUpdate); else HCSMVOPINVOKE.HMarkupManager_UpdatePreviewImageSwigExplicitHMarkupManager__SWIG_0(swigCPtr, forceUpdate);
  }

  public virtual void UpdatePreviewImage() {
    if (this.GetType() == typeof(HMarkupManager)) HCSMVOPINVOKE.HMarkupManager_UpdatePreviewImage__SWIG_1(swigCPtr); else HCSMVOPINVOKE.HMarkupManager_UpdatePreviewImageSwigExplicitHMarkupManager__SWIG_1(swigCPtr);
  }

  public bool IsCameraLayer(MVO_POINTER_SIZED_INT layerkey) {
    bool ret = HCSMVOPINVOKE.HMarkupManager_IsCameraLayer(swigCPtr, layerkey);
    return ret;
  }

  public void SetLayerCondition(MVO_POINTER_SIZED_INT layerkey) {
    HCSMVOPINVOKE.HMarkupManager_SetLayerCondition(swigCPtr, layerkey);
  }

  public void UnsetCurrentLayerCondition() {
    HCSMVOPINVOKE.HMarkupManager_UnsetCurrentLayerCondition(swigCPtr);
  }

  public void UnsetLayerCondition(MVO_POINTER_SIZED_INT layerkey) {
    HCSMVOPINVOKE.HMarkupManager_UnsetLayerCondition(swigCPtr, layerkey);
  }

  public void SetGeneratePreviewImage(bool onoff) {
    HCSMVOPINVOKE.HMarkupManager_SetGeneratePreviewImage(swigCPtr, onoff);
  }

  public bool GetGeneratePreviewImage() {
    bool ret = HCSMVOPINVOKE.HMarkupManager_GetGeneratePreviewImage(swigCPtr);
    return ret;
  }

  public void SetNoResetOnCameraChange() {
    HCSMVOPINVOKE.HMarkupManager_SetNoResetOnCameraChange(swigCPtr);
  }

  public bool GetResetOnCameraChange(MVO_POINTER_SIZED_INT layerkey) {
    bool ret = HCSMVOPINVOKE.HMarkupManager_GetResetOnCameraChange(swigCPtr, layerkey);
    return ret;
  }

  protected virtual void SetupNewLayer() {
    if (this.GetType() == typeof(HMarkupManager)) HCSMVOPINVOKE.HMarkupManager_SetupNewLayer(swigCPtr); else HCSMVOPINVOKE.HMarkupManager_SetupNewLayerSwigExplicitHMarkupManager(swigCPtr);
  }

  protected virtual void OpenLayerInternal(MVO_POINTER_SIZED_INT layerkey, bool setcamera) {
    if (this.GetType() == typeof(HMarkupManager)) HCSMVOPINVOKE.HMarkupManager_OpenLayerInternal(swigCPtr, layerkey, setcamera); else HCSMVOPINVOKE.HMarkupManager_OpenLayerInternalSwigExplicitHMarkupManager(swigCPtr, layerkey, setcamera);
  }

  protected HBaseView m_pHView {
    set {
      HCSMVOPINVOKE.HMarkupManager_m_pHView_set(swigCPtr, HBaseView.getCPtr(value));
    } 
    get {
      IntPtr cPtr = HCSMVOPINVOKE.HMarkupManager_m_pHView_get(swigCPtr);
      HBaseView ret = (cPtr == IntPtr.Zero) ? null : new HBaseView(cPtr, false);
      return ret;
    } 
  }

  protected MVO_POINTER_SIZED_INT m_ActiveLayerKey {
	set { HCSMVOPINVOKE.HMarkupManager_m_ActiveLayerKey_set(swigCPtr, value); } 
	get { return HCSMVOPINVOKE.HMarkupManager_m_ActiveLayerKey_get(swigCPtr); } 
  }

  protected System.Text.StringBuilder m_ActiveLayerName {
	set { HCSMVOPINVOKE.HMarkupManager_m_ActiveLayerName_set(swigCPtr, value); } 
	get { return HCSMVOPINVOKE.HMarkupManager_m_ActiveLayerName_get(swigCPtr); } 
  }

  protected MVO_POINTER_SIZED_INT m_DefaultLayerKey {
	set { HCSMVOPINVOKE.HMarkupManager_m_DefaultLayerKey_set(swigCPtr, value); } 
	get { return HCSMVOPINVOKE.HMarkupManager_m_DefaultLayerKey_get(swigCPtr); } 
  }

  protected MVO_POINTER_SIZED_INT m_MarkupKey {
	set { HCSMVOPINVOKE.HMarkupManager_m_MarkupKey_set(swigCPtr, value); } 
	get { return HCSMVOPINVOKE.HMarkupManager_m_MarkupKey_get(swigCPtr); } 
  }

  protected MVO_POINTER_SIZED_INT m_MarkupLayerKey {
	set { HCSMVOPINVOKE.HMarkupManager_m_MarkupLayerKey_set(swigCPtr, value); } 
	get { return HCSMVOPINVOKE.HMarkupManager_m_MarkupLayerKey_get(swigCPtr); } 
  }

  protected HPoint m_MarkupColor {
    set {
      HCSMVOPINVOKE.HMarkupManager_m_MarkupColor_set(swigCPtr, HPoint.getCPtr(value));
    } 
    get {
      IntPtr cPtr = HCSMVOPINVOKE.HMarkupManager_m_MarkupColor_get(swigCPtr);
      HPoint ret = (cPtr == IntPtr.Zero) ? null : new HPoint(cPtr, false);
      return ret;
    } 
  }

  protected float m_fMarkupWeight {
    set {
      HCSMVOPINVOKE.HMarkupManager_m_fMarkupWeight_set(swigCPtr, value);
    } 
    get {
      float ret = HCSMVOPINVOKE.HMarkupManager_m_fMarkupWeight_get(swigCPtr);
      return ret;
    } 
  }

  protected System.Text.StringBuilder m_allNames {
	set { HCSMVOPINVOKE.HMarkupManager_m_allNames_set(swigCPtr, value); } 
	get { return HCSMVOPINVOKE.HMarkupManager_m_allNames_get(swigCPtr); } 
  }

  protected bool m_bGeneratePreviewImage {
    set {
      HCSMVOPINVOKE.HMarkupManager_m_bGeneratePreviewImage_set(swigCPtr, value);
    } 
    get {
      bool ret = HCSMVOPINVOKE.HMarkupManager_m_bGeneratePreviewImage_get(swigCPtr);
      return ret;
    } 
  }

  private void SwigDirectorConnect() {
    if (SwigDerivedClassHasMethod("OpenLayer", swigMethodTypes0))
      swigDelegate0 = new SwigDelegateHMarkupManager_0(SwigDirectorOpenLayer__SWIG_0);
    if (SwigDerivedClassHasMethod("OpenLayer", swigMethodTypes1))
      swigDelegate1 = new SwigDelegateHMarkupManager_1(SwigDirectorOpenLayer__SWIG_1);
    if (SwigDerivedClassHasMethod("OpenLayer", swigMethodTypes2))
      swigDelegate2 = new SwigDelegateHMarkupManager_2(SwigDirectorOpenLayer__SWIG_2);
    if (SwigDerivedClassHasMethod("RenameLayer", swigMethodTypes3))
      swigDelegate3 = new SwigDelegateHMarkupManager_3(SwigDirectorRenameLayer);
    if (SwigDerivedClassHasMethod("CopyLayer", swigMethodTypes4))
      swigDelegate4 = new SwigDelegateHMarkupManager_4(SwigDirectorCopyLayer);
    if (SwigDerivedClassHasMethod("GetCamera", swigMethodTypes5))
      swigDelegate5 = new SwigDelegateHMarkupManager_5(SwigDirectorGetCamera);
    if (SwigDerivedClassHasMethod("OpenLayer", swigMethodTypes6))
      swigDelegate6 = new SwigDelegateHMarkupManager_6(SwigDirectorOpenLayer__SWIG_3);
    if (SwigDerivedClassHasMethod("OpenLayer", swigMethodTypes7))
      swigDelegate7 = new SwigDelegateHMarkupManager_7(SwigDirectorOpenLayer__SWIG_4);
    if (SwigDerivedClassHasMethod("FindLayer", swigMethodTypes8))
      swigDelegate8 = new SwigDelegateHMarkupManager_8(SwigDirectorFindLayer);
    if (SwigDerivedClassHasMethod("GetLayerNames", swigMethodTypes9))
      swigDelegate9 = new SwigDelegateHMarkupManager_9(SwigDirectorGetLayerNames);
    if (SwigDerivedClassHasMethod("GetCurrentLayerKey", swigMethodTypes10))
      swigDelegate10 = new SwigDelegateHMarkupManager_10(SwigDirectorGetCurrentLayerKey);
    if (SwigDerivedClassHasMethod("GetCurrentLayerName", swigMethodTypes11))
      swigDelegate11 = new SwigDelegateHMarkupManager_11(SwigDirectorGetCurrentLayerName);
    if (SwigDerivedClassHasMethod("DisableCurrentLayer", swigMethodTypes12))
      swigDelegate12 = new SwigDelegateHMarkupManager_12(SwigDirectorDisableCurrentLayer__SWIG_0);
    if (SwigDerivedClassHasMethod("DisableCurrentLayer", swigMethodTypes13))
      swigDelegate13 = new SwigDelegateHMarkupManager_13(SwigDirectorDisableCurrentLayer__SWIG_1);
    if (SwigDerivedClassHasMethod("DeleteLayer", swigMethodTypes14))
      swigDelegate14 = new SwigDelegateHMarkupManager_14(SwigDirectorDeleteLayer__SWIG_0);
    if (SwigDerivedClassHasMethod("DeleteLayer", swigMethodTypes15))
      swigDelegate15 = new SwigDelegateHMarkupManager_15(SwigDirectorDeleteLayer__SWIG_1);
    if (SwigDerivedClassHasMethod("GetMarkupWeight", swigMethodTypes16))
      swigDelegate16 = new SwigDelegateHMarkupManager_16(SwigDirectorGetMarkupWeight);
    if (SwigDerivedClassHasMethod("SetMarkupWeight", swigMethodTypes17))
      swigDelegate17 = new SwigDelegateHMarkupManager_17(SwigDirectorSetMarkupWeight);
    if (SwigDerivedClassHasMethod("GetMarkupColor", swigMethodTypes18))
      swigDelegate18 = new SwigDelegateHMarkupManager_18(SwigDirectorGetMarkupColor);
    if (SwigDerivedClassHasMethod("SetMarkupColor", swigMethodTypes19))
      swigDelegate19 = new SwigDelegateHMarkupManager_19(SwigDirectorSetMarkupColor);
    if (SwigDerivedClassHasMethod("EmitMarkupSegment", swigMethodTypes20))
      swigDelegate20 = new SwigDelegateHMarkupManager_20(SwigDirectorEmitMarkupSegment);
    if (SwigDerivedClassHasMethod("InsertMarkupSegmentFromMessage", swigMethodTypes21))
      swigDelegate21 = new SwigDelegateHMarkupManager_21(SwigDirectorInsertMarkupSegmentFromMessage);
    if (SwigDerivedClassHasMethod("EmitDisableCurrentMarkupLayer", swigMethodTypes22))
      swigDelegate22 = new SwigDelegateHMarkupManager_22(SwigDirectorEmitDisableCurrentMarkupLayer);
    if (SwigDerivedClassHasMethod("DisableCurrentMarkupLayerFromMessage", swigMethodTypes23))
      swigDelegate23 = new SwigDelegateHMarkupManager_23(SwigDirectorDisableCurrentMarkupLayerFromMessage);
    if (SwigDerivedClassHasMethod("EmitSetMarkupLayer", swigMethodTypes24))
      swigDelegate24 = new SwigDelegateHMarkupManager_24(SwigDirectorEmitSetMarkupLayer);
    if (SwigDerivedClassHasMethod("SetMarkupLayerFromMessage", swigMethodTypes25))
      swigDelegate25 = new SwigDelegateHMarkupManager_25(SwigDirectorSetMarkupLayerFromMessage);
    if (SwigDerivedClassHasMethod("UpdatePreviewImage", swigMethodTypes26))
      swigDelegate26 = new SwigDelegateHMarkupManager_26(SwigDirectorUpdatePreviewImage__SWIG_0);
    if (SwigDerivedClassHasMethod("UpdatePreviewImage", swigMethodTypes27))
      swigDelegate27 = new SwigDelegateHMarkupManager_27(SwigDirectorUpdatePreviewImage__SWIG_1);
    if (SwigDerivedClassHasMethod("SetupNewLayer", swigMethodTypes28))
      swigDelegate28 = new SwigDelegateHMarkupManager_28(SwigDirectorSetupNewLayer);
    if (SwigDerivedClassHasMethod("OpenLayerInternal", swigMethodTypes29))
      swigDelegate29 = new SwigDelegateHMarkupManager_29(SwigDirectorOpenLayerInternal);
    HCSMVOPINVOKE.HMarkupManager_director_connect(swigCPtr, swigDelegate0, swigDelegate1, swigDelegate2, swigDelegate3, swigDelegate4, swigDelegate5, swigDelegate6, swigDelegate7, swigDelegate8, swigDelegate9, swigDelegate10, swigDelegate11, swigDelegate12, swigDelegate13, swigDelegate14, swigDelegate15, swigDelegate16, swigDelegate17, swigDelegate18, swigDelegate19, swigDelegate20, swigDelegate21, swigDelegate22, swigDelegate23, swigDelegate24, swigDelegate25, swigDelegate26, swigDelegate27, swigDelegate28, swigDelegate29);
  }

  private bool SwigDerivedClassHasMethod(string methodName, Type[] methodTypes) {
    System.Reflection.MethodInfo methodInfo = this.GetType().GetMethod(methodName, System.Reflection.BindingFlags.Public | System.Reflection.BindingFlags.NonPublic | System.Reflection.BindingFlags.Instance, null, methodTypes, null);
    bool hasDerivedMethod = methodInfo.DeclaringType.IsSubclassOf(typeof(HMarkupManager));
    return hasDerivedMethod;
  }

  private MVO_POINTER_SIZED_INT SwigDirectorOpenLayer__SWIG_0(string layername, bool setcamera, bool emit_message) {
    return OpenLayer(layername, setcamera, emit_message);
  }

  private MVO_POINTER_SIZED_INT SwigDirectorOpenLayer__SWIG_1(string layername, bool setcamera) {
    return OpenLayer(layername, setcamera);
  }

  private MVO_POINTER_SIZED_INT SwigDirectorOpenLayer__SWIG_2(string layername) {
    return OpenLayer(layername);
  }

  private MVO_POINTER_SIZED_INT SwigDirectorRenameLayer(MVO_POINTER_SIZED_INT laykey, string new_name) {
    return RenameLayer(laykey, new_name);
  }

  private MVO_POINTER_SIZED_INT SwigDirectorCopyLayer(MVO_POINTER_SIZED_INT laykey, string new_name) {
    return CopyLayer(laykey, new_name);
  }

  private bool SwigDirectorGetCamera(MVO_POINTER_SIZED_INT layerkey, IntPtr cam) {
    return GetCamera(layerkey, new HCamera(cam, false));
  }

  private MVO_POINTER_SIZED_INT SwigDirectorOpenLayer__SWIG_3(MVO_POINTER_SIZED_INT layerkey, bool setcamera) {
    return OpenLayer(layerkey, setcamera);
  }

  private MVO_POINTER_SIZED_INT SwigDirectorOpenLayer__SWIG_4(MVO_POINTER_SIZED_INT layerkey) {
    return OpenLayer(layerkey);
  }

  private MVO_POINTER_SIZED_INT SwigDirectorFindLayer(string layername) {
    return FindLayer(layername);
  }

  private string SwigDirectorGetLayerNames() {
    return GetLayerNames();
  }

  private MVO_POINTER_SIZED_INT SwigDirectorGetCurrentLayerKey() {
    return GetCurrentLayerKey();
  }

  private string SwigDirectorGetCurrentLayerName() {
    return GetCurrentLayerName();
  }

  private void SwigDirectorDisableCurrentLayer__SWIG_0(bool emit_message) {
    DisableCurrentLayer(emit_message);
  }

  private void SwigDirectorDisableCurrentLayer__SWIG_1() {
    DisableCurrentLayer();
  }

  private void SwigDirectorDeleteLayer__SWIG_0(string layername) {
    DeleteLayer(layername);
  }

  private void SwigDirectorDeleteLayer__SWIG_1(MVO_POINTER_SIZED_INT layerkey) {
    DeleteLayer(layerkey);
  }

  private float SwigDirectorGetMarkupWeight() {
    return GetMarkupWeight();
  }

  private void SwigDirectorSetMarkupWeight(float weight) {
    SetMarkupWeight(weight);
  }

  private void SwigDirectorGetMarkupColor(IntPtr markup_color) {
    GetMarkupColor(new HPoint(markup_color, false));
  }

  private void SwigDirectorSetMarkupColor(IntPtr markup_color) {
    SetMarkupColor(new HPoint(markup_color, false));
  }

  private void SwigDirectorEmitMarkupSegment(MVO_POINTER_SIZED_INT key) {
    EmitMarkupSegment(key);
  }

  private void SwigDirectorInsertMarkupSegmentFromMessage(string in_data, uint data_length) {
    InsertMarkupSegmentFromMessage(in_data, data_length);
  }

  private void SwigDirectorEmitDisableCurrentMarkupLayer() {
    EmitDisableCurrentMarkupLayer();
  }

  private void SwigDirectorDisableCurrentMarkupLayerFromMessage(string data, uint data_length) {
    DisableCurrentMarkupLayerFromMessage(data, data_length);
  }

  private void SwigDirectorEmitSetMarkupLayer(string layer) {
    EmitSetMarkupLayer(layer);
  }

  private void SwigDirectorSetMarkupLayerFromMessage(string data, uint data_length) {
    SetMarkupLayerFromMessage(data, data_length);
  }

  private void SwigDirectorUpdatePreviewImage__SWIG_0(bool forceUpdate) {
    UpdatePreviewImage(forceUpdate);
  }

  private void SwigDirectorUpdatePreviewImage__SWIG_1() {
    UpdatePreviewImage();
  }

  private void SwigDirectorSetupNewLayer() {
    SetupNewLayer();
  }

  private void SwigDirectorOpenLayerInternal(MVO_POINTER_SIZED_INT layerkey, bool setcamera) {
    OpenLayerInternal(layerkey, setcamera);
  }

  public delegate MVO_POINTER_SIZED_INT SwigDelegateHMarkupManager_0(string layername, bool setcamera, bool emit_message);
  public delegate MVO_POINTER_SIZED_INT SwigDelegateHMarkupManager_1(string layername, bool setcamera);
  public delegate MVO_POINTER_SIZED_INT SwigDelegateHMarkupManager_2(string layername);
  public delegate MVO_POINTER_SIZED_INT SwigDelegateHMarkupManager_3(MVO_POINTER_SIZED_INT laykey, string new_name);
  public delegate MVO_POINTER_SIZED_INT SwigDelegateHMarkupManager_4(MVO_POINTER_SIZED_INT laykey, string new_name);
  public delegate bool SwigDelegateHMarkupManager_5(MVO_POINTER_SIZED_INT layerkey, IntPtr cam);
  public delegate MVO_POINTER_SIZED_INT SwigDelegateHMarkupManager_6(MVO_POINTER_SIZED_INT layerkey, bool setcamera);
  public delegate MVO_POINTER_SIZED_INT SwigDelegateHMarkupManager_7(MVO_POINTER_SIZED_INT layerkey);
  public delegate MVO_POINTER_SIZED_INT SwigDelegateHMarkupManager_8(string layername);
  public delegate string SwigDelegateHMarkupManager_9();
  public delegate MVO_POINTER_SIZED_INT SwigDelegateHMarkupManager_10();
  public delegate string SwigDelegateHMarkupManager_11();
  public delegate void SwigDelegateHMarkupManager_12(bool emit_message);
  public delegate void SwigDelegateHMarkupManager_13();
  public delegate void SwigDelegateHMarkupManager_14(string layername);
  public delegate void SwigDelegateHMarkupManager_15(MVO_POINTER_SIZED_INT layerkey);
  public delegate float SwigDelegateHMarkupManager_16();
  public delegate void SwigDelegateHMarkupManager_17(float weight);
  public delegate void SwigDelegateHMarkupManager_18(IntPtr markup_color);
  public delegate void SwigDelegateHMarkupManager_19(IntPtr markup_color);
  public delegate void SwigDelegateHMarkupManager_20(MVO_POINTER_SIZED_INT key);
  public delegate void SwigDelegateHMarkupManager_21(string in_data, uint data_length);
  public delegate void SwigDelegateHMarkupManager_22();
  public delegate void SwigDelegateHMarkupManager_23(string data, uint data_length);
  public delegate void SwigDelegateHMarkupManager_24(string layer);
  public delegate void SwigDelegateHMarkupManager_25(string data, uint data_length);
  public delegate void SwigDelegateHMarkupManager_26(bool forceUpdate);
  public delegate void SwigDelegateHMarkupManager_27();
  public delegate void SwigDelegateHMarkupManager_28();
  public delegate void SwigDelegateHMarkupManager_29(MVO_POINTER_SIZED_INT layerkey, bool setcamera);

  private SwigDelegateHMarkupManager_0 swigDelegate0;
  private SwigDelegateHMarkupManager_1 swigDelegate1;
  private SwigDelegateHMarkupManager_2 swigDelegate2;
  private SwigDelegateHMarkupManager_3 swigDelegate3;
  private SwigDelegateHMarkupManager_4 swigDelegate4;
  private SwigDelegateHMarkupManager_5 swigDelegate5;
  private SwigDelegateHMarkupManager_6 swigDelegate6;
  private SwigDelegateHMarkupManager_7 swigDelegate7;
  private SwigDelegateHMarkupManager_8 swigDelegate8;
  private SwigDelegateHMarkupManager_9 swigDelegate9;
  private SwigDelegateHMarkupManager_10 swigDelegate10;
  private SwigDelegateHMarkupManager_11 swigDelegate11;
  private SwigDelegateHMarkupManager_12 swigDelegate12;
  private SwigDelegateHMarkupManager_13 swigDelegate13;
  private SwigDelegateHMarkupManager_14 swigDelegate14;
  private SwigDelegateHMarkupManager_15 swigDelegate15;
  private SwigDelegateHMarkupManager_16 swigDelegate16;
  private SwigDelegateHMarkupManager_17 swigDelegate17;
  private SwigDelegateHMarkupManager_18 swigDelegate18;
  private SwigDelegateHMarkupManager_19 swigDelegate19;
  private SwigDelegateHMarkupManager_20 swigDelegate20;
  private SwigDelegateHMarkupManager_21 swigDelegate21;
  private SwigDelegateHMarkupManager_22 swigDelegate22;
  private SwigDelegateHMarkupManager_23 swigDelegate23;
  private SwigDelegateHMarkupManager_24 swigDelegate24;
  private SwigDelegateHMarkupManager_25 swigDelegate25;
  private SwigDelegateHMarkupManager_26 swigDelegate26;
  private SwigDelegateHMarkupManager_27 swigDelegate27;
  private SwigDelegateHMarkupManager_28 swigDelegate28;
  private SwigDelegateHMarkupManager_29 swigDelegate29;

  private static Type[] swigMethodTypes0 = new Type[] { typeof(string), typeof(bool), typeof(bool) };
  private static Type[] swigMethodTypes1 = new Type[] { typeof(string), typeof(bool) };
  private static Type[] swigMethodTypes2 = new Type[] { typeof(string) };
  private static Type[] swigMethodTypes3 = new Type[] { typeof(MVO_POINTER_SIZED_INT), typeof(string) };
  private static Type[] swigMethodTypes4 = new Type[] { typeof(MVO_POINTER_SIZED_INT), typeof(string) };
  private static Type[] swigMethodTypes5 = new Type[] { typeof(MVO_POINTER_SIZED_INT), typeof(HCamera) };
  private static Type[] swigMethodTypes6 = new Type[] { typeof(MVO_POINTER_SIZED_INT), typeof(bool) };
  private static Type[] swigMethodTypes7 = new Type[] { typeof(MVO_POINTER_SIZED_INT) };
  private static Type[] swigMethodTypes8 = new Type[] { typeof(string) };
  private static Type[] swigMethodTypes9 = new Type[] {  };
  private static Type[] swigMethodTypes10 = new Type[] {  };
  private static Type[] swigMethodTypes11 = new Type[] {  };
  private static Type[] swigMethodTypes12 = new Type[] { typeof(bool) };
  private static Type[] swigMethodTypes13 = new Type[] {  };
  private static Type[] swigMethodTypes14 = new Type[] { typeof(string) };
  private static Type[] swigMethodTypes15 = new Type[] { typeof(MVO_POINTER_SIZED_INT) };
  private static Type[] swigMethodTypes16 = new Type[] {  };
  private static Type[] swigMethodTypes17 = new Type[] { typeof(float) };
  private static Type[] swigMethodTypes18 = new Type[] { typeof(HPoint) };
  private static Type[] swigMethodTypes19 = new Type[] { typeof(HPoint) };
  private static Type[] swigMethodTypes20 = new Type[] { typeof(MVO_POINTER_SIZED_INT) };
  private static Type[] swigMethodTypes21 = new Type[] { typeof(string), typeof(uint) };
  private static Type[] swigMethodTypes22 = new Type[] {  };
  private static Type[] swigMethodTypes23 = new Type[] { typeof(string), typeof(uint) };
  private static Type[] swigMethodTypes24 = new Type[] { typeof(string) };
  private static Type[] swigMethodTypes25 = new Type[] { typeof(string), typeof(uint) };
  private static Type[] swigMethodTypes26 = new Type[] { typeof(bool) };
  private static Type[] swigMethodTypes27 = new Type[] {  };
  private static Type[] swigMethodTypes28 = new Type[] {  };
  private static Type[] swigMethodTypes29 = new Type[] { typeof(MVO_POINTER_SIZED_INT), typeof(bool) };
}
