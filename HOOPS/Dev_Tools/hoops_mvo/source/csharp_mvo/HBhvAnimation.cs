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


public class HBhvAnimation : IDisposable {
  private HandleRef swigCPtr;
  protected bool swigCMemOwn;

  public HBhvAnimation(IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new HandleRef(this, cPtr);
  }

  public static HandleRef getCPtr(HBhvAnimation obj) {
    return (obj == null) ? new HandleRef(null, IntPtr.Zero) : obj.swigCPtr;
  }
  
  public void owns_memory(bool ownMemory) {
	swigCMemOwn = ownMemory;
  }

  ~HBhvAnimation() {
    Dispose();
  }

  public virtual void Dispose() {
    lock(this) {
      if (swigCPtr.Handle != IntPtr.Zero) {
        if (swigCMemOwn) {
          swigCMemOwn = false;
          HCSMVOPINVOKE.delete_HBhvAnimation(swigCPtr);
        }
        swigCPtr = new HandleRef(null, IntPtr.Zero);
      }
      GC.SuppressFinalize(this);
    }
  }

  public HBhvAnimation(string name, HBhvBehaviorManager BehaviorManager, HBhvAnimation parentanimation) : this(HCSMVOPINVOKE.new_HBhvAnimation__SWIG_0(name, HBhvBehaviorManager.getCPtr(BehaviorManager), HBhvAnimation.getCPtr(parentanimation)), true) {
  }

  public HBhvAnimation(string name, HBhvBehaviorManager BehaviorManager) : this(HCSMVOPINVOKE.new_HBhvAnimation__SWIG_1(name, HBhvBehaviorManager.getCPtr(BehaviorManager)), true) {
  }

  public virtual string GetHType() {
    string ret = HCSMVOPINVOKE.HBhvAnimation_GetHType(swigCPtr);
    return ret;
  }

  public string GetName() {
    string ret = HCSMVOPINVOKE.HBhvAnimation_GetName(swigCPtr);
    return ret;
  }

  public void SetName(string name) {
    HCSMVOPINVOKE.HBhvAnimation_SetName(swigCPtr, name);
  }

  public virtual bool Animate(float currenttime) {
    bool ret = HCSMVOPINVOKE.HBhvAnimation_Animate__SWIG_0(swigCPtr, currenttime);
    return ret;
  }

  public virtual bool Animate(float currenttime, float starttime) {
    bool ret = HCSMVOPINVOKE.HBhvAnimation_Animate__SWIG_1(swigCPtr, currenttime, starttime);
    return ret;
  }

  public void DeleteKeyframe(int keyframe) {
    HCSMVOPINVOKE.HBhvAnimation_DeleteKeyframe(swigCPtr, keyframe);
  }

  public void DuplicateNextOrPrevious(int keyframe, bool next) {
    HCSMVOPINVOKE.HBhvAnimation_DuplicateNextOrPrevious(swigCPtr, keyframe, next);
  }

  public void Duplicate(int newkeyframe, int oldkeyframe) {
    HCSMVOPINVOKE.HBhvAnimation_Duplicate(swigCPtr, newkeyframe, oldkeyframe);
  }

  public void PlotCurve(bool rotations) {
    HCSMVOPINVOKE.HBhvAnimation_PlotCurve(swigCPtr, rotations);
  }

  public virtual void Serialize(SWIGTYPE_p_HUtilityXMLGenerator xmlgen, SWIGTYPE_p_HUtilityXMLTag xmlt) {
    HCSMVOPINVOKE.HBhvAnimation_Serialize__SWIG_0(swigCPtr, SWIGTYPE_p_HUtilityXMLGenerator.getCPtr(xmlgen), SWIGTYPE_p_HUtilityXMLTag.getCPtr(xmlt));
  }

  public virtual void Serialize(SWIGTYPE_p_HUtilityXMLGenerator xmlgen) {
    HCSMVOPINVOKE.HBhvAnimation_Serialize__SWIG_1(swigCPtr, SWIGTYPE_p_HUtilityXMLGenerator.getCPtr(xmlgen));
  }

  public static IntPtr XMLCallback(SWIGTYPE_p_HUtilityXMLTag xt, bool open, IntPtr m_pExtraData) {return new IntPtr((int)HCSMVOPINVOKE.HBhvAnimation_XMLCallback(SWIGTYPE_p_HUtilityXMLTag.getCPtr(xt), open, m_pExtraData));}

  public HBhvBehaviorManager GetBehaviorManager() {
    IntPtr cPtr = HCSMVOPINVOKE.HBhvAnimation_GetBehaviorManager(swigCPtr);
    HBhvBehaviorManager ret = (cPtr == IntPtr.Zero) ? null : new HBhvBehaviorManager(cPtr, false);
    return ret;
  }

  public void SetBehaviorManager(HBhvBehaviorManager behaviourmanager) {
    HCSMVOPINVOKE.HBhvAnimation_SetBehaviorManager(swigCPtr, HBhvBehaviorManager.getCPtr(behaviourmanager));
  }

  public int GetFirstTick() {
    int ret = HCSMVOPINVOKE.HBhvAnimation_GetFirstTick(swigCPtr);
    return ret;
  }

  public int GetLastTick() {
    int ret = HCSMVOPINVOKE.HBhvAnimation_GetLastTick(swigCPtr);
    return ret;
  }

  public int GetLoop() {
    int ret = HCSMVOPINVOKE.HBhvAnimation_GetLoop(swigCPtr);
    return ret;
  }

  public void SetLoop(int loop) {
    HCSMVOPINVOKE.HBhvAnimation_SetLoop(swigCPtr, loop);
  }

  public SWIGTYPE_p_HBhvTimeline GetTimeline() {
    IntPtr cPtr = HCSMVOPINVOKE.HBhvAnimation_GetTimeline(swigCPtr);
    SWIGTYPE_p_HBhvTimeline ret = (cPtr == IntPtr.Zero) ? null : new SWIGTYPE_p_HBhvTimeline(cPtr, false);
    return ret;
  }

  public void SetTimeline(SWIGTYPE_p_HBhvTimeline timeline) {
    HCSMVOPINVOKE.HBhvAnimation_SetTimeline(swigCPtr, SWIGTYPE_p_HBhvTimeline.getCPtr(timeline));
  }

  public HBhvAnimation CreateInstance() {
    IntPtr cPtr = HCSMVOPINVOKE.HBhvAnimation_CreateInstance(swigCPtr);
    HBhvAnimation ret = (cPtr == IntPtr.Zero) ? null : new HBhvAnimation(cPtr, false);
    return ret;
  }

  public SWIGTYPE_p_vlist_s GetInterpolatorList() {
    IntPtr cPtr = HCSMVOPINVOKE.HBhvAnimation_GetInterpolatorList(swigCPtr);
    SWIGTYPE_p_vlist_s ret = (cPtr == IntPtr.Zero) ? null : new SWIGTYPE_p_vlist_s(cPtr, false);
    return ret;
  }

  public SWIGTYPE_p_vlist_s GetChildAnimationList() {
    IntPtr cPtr = HCSMVOPINVOKE.HBhvAnimation_GetChildAnimationList(swigCPtr);
    SWIGTYPE_p_vlist_s ret = (cPtr == IntPtr.Zero) ? null : new SWIGTYPE_p_vlist_s(cPtr, false);
    return ret;
  }

  public SWIGTYPE_p_HBhvInterpolator GetInterpolator() {
    IntPtr cPtr = HCSMVOPINVOKE.HBhvAnimation_GetInterpolator__SWIG_0(swigCPtr);
    SWIGTYPE_p_HBhvInterpolator ret = (cPtr == IntPtr.Zero) ? null : new SWIGTYPE_p_HBhvInterpolator(cPtr, false);
    return ret;
  }

  public void AddInterpolator(SWIGTYPE_p_HBhvInterpolator interpolator) {
    HCSMVOPINVOKE.HBhvAnimation_AddInterpolator(swigCPtr, SWIGTYPE_p_HBhvInterpolator.getCPtr(interpolator));
  }

  public void AddChildAnimation(HBhvAnimation childanimation) {
    HCSMVOPINVOKE.HBhvAnimation_AddChildAnimation(swigCPtr, HBhvAnimation.getCPtr(childanimation));
  }

  public HBhvAnimation GetParentAnimation() {
    IntPtr cPtr = HCSMVOPINVOKE.HBhvAnimation_GetParentAnimation(swigCPtr);
    HBhvAnimation ret = (cPtr == IntPtr.Zero) ? null : new HBhvAnimation(cPtr, false);
    return ret;
  }

  public void SetDelay(int delay) {
    HCSMVOPINVOKE.HBhvAnimation_SetDelay(swigCPtr, delay);
  }

  public int GetDelay() {
    int ret = HCSMVOPINVOKE.HBhvAnimation_GetDelay(swigCPtr);
    return ret;
  }

  public HBhvTargetObject GetTarget() {
    IntPtr cPtr = HCSMVOPINVOKE.HBhvAnimation_GetTarget(swigCPtr);
    HBhvTargetObject ret = (cPtr == IntPtr.Zero) ? null : new HBhvTargetObject(cPtr, false);
    return ret;
  }

  public HBhvAnimation GetInstancedAnimation() {
    IntPtr cPtr = HCSMVOPINVOKE.HBhvAnimation_GetInstancedAnimation(swigCPtr);
    HBhvAnimation ret = (cPtr == IntPtr.Zero) ? null : new HBhvAnimation(cPtr, false);
    return ret;
  }

  public void SetInstancedAnimation(HBhvAnimation animation) {
    HCSMVOPINVOKE.HBhvAnimation_SetInstancedAnimation(swigCPtr, HBhvAnimation.getCPtr(animation));
  }

  public void Reset() {
    HCSMVOPINVOKE.HBhvAnimation_Reset(swigCPtr);
  }

  public void SetTargetByPath(string targetpath) {
    HCSMVOPINVOKE.HBhvAnimation_SetTargetByPath__SWIG_0(swigCPtr, targetpath);
  }

  public void SetTargetByPath(string name, string targetpath) {
    HCSMVOPINVOKE.HBhvAnimation_SetTargetByPath__SWIG_1(swigCPtr, name, targetpath);
  }

  public void SetTarget(HBhvTargetObject target) {
    HCSMVOPINVOKE.HBhvAnimation_SetTarget(swigCPtr, HBhvTargetObject.getCPtr(target));
  }

  public bool GetInterpolator(int time, int[] interval, SWIGTYPE_p_p_HKeyframe keyframe) {
    bool ret = HCSMVOPINVOKE.HBhvAnimation_GetInterpolator__SWIG_1(swigCPtr, time, interval, SWIGTYPE_p_p_HKeyframe.getCPtr(keyframe));
    return ret;
  }

  public void SetDefaultActive(bool active) {
    HCSMVOPINVOKE.HBhvAnimation_SetDefaultActive(swigCPtr, active);
  }

  public bool GetDefaultActive() {
    bool ret = HCSMVOPINVOKE.HBhvAnimation_GetDefaultActive(swigCPtr);
    return ret;
  }

  public void SetCurrentlyRunning(bool running) {
    HCSMVOPINVOKE.HBhvAnimation_SetCurrentlyRunning(swigCPtr, running);
  }

  public bool GetCurrentlyRunning() {
    bool ret = HCSMVOPINVOKE.HBhvAnimation_GetCurrentlyRunning(swigCPtr);
    return ret;
  }

  public float GetCurrentTick() {
    float ret = HCSMVOPINVOKE.HBhvAnimation_GetCurrentTick(swigCPtr);
    return ret;
  }

  public void AdjustKeyframe(int keyframe, int delta, bool doall, bool relative) {
    HCSMVOPINVOKE.HBhvAnimation_AdjustKeyframe__SWIG_0(swigCPtr, keyframe, delta, doall, relative);
  }

  public void AdjustKeyframe(int keyframe, int delta, bool doall) {
    HCSMVOPINVOKE.HBhvAnimation_AdjustKeyframe__SWIG_1(swigCPtr, keyframe, delta, doall);
  }

  public void AdjustKeyframe(int keyframe, int delta) {
    HCSMVOPINVOKE.HBhvAnimation_AdjustKeyframe__SWIG_2(swigCPtr, keyframe, delta);
  }

  public void CleanWithoutDelete() {
    HCSMVOPINVOKE.HBhvAnimation_CleanWithoutDelete(swigCPtr);
  }

  public bool ExecuteOnce() {
    bool ret = HCSMVOPINVOKE.HBhvAnimation_ExecuteOnce(swigCPtr);
    return ret;
  }

  public void SetExecuteOnce(bool once) {
    HCSMVOPINVOKE.HBhvAnimation_SetExecuteOnce(swigCPtr, once);
  }

  public void Evaluate(float currenttick, bool[] hasPos, HPoint pos, bool[] hasQuat, HQuat quat, bool[] hasScale, HPoint scale) {
    HCSMVOPINVOKE.HBhvAnimation_Evaluate(swigCPtr, currenttick, hasPos, HPoint.getCPtr(pos), hasQuat, HQuat.getCPtr(quat), hasScale, HPoint.getCPtr(scale));
    if (HCSMVOPINVOKE.SWIGPendingException.Pending) throw HCSMVOPINVOKE.SWIGPendingException.Retrieve();
  }

  public void SetTargetScaleMatrix(float[] target_matrix) {
    HCSMVOPINVOKE.HBhvAnimation_SetTargetScaleMatrix(swigCPtr, target_matrix);
  }

  public IntPtr GetTargetScaleMatrix() {return HCSMVOPINVOKE.HBhvAnimation_GetTargetScaleMatrix(swigCPtr);}

}