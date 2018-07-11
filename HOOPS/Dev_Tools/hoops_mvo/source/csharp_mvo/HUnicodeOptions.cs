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


public class HUnicodeOptions : IDisposable {
  private HandleRef swigCPtr;
  protected bool swigCMemOwn;

  public HUnicodeOptions(IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new HandleRef(this, cPtr);
  }

  public static HandleRef getCPtr(HUnicodeOptions obj) {
    return (obj == null) ? new HandleRef(null, IntPtr.Zero) : obj.swigCPtr;
  }
  
  public void owns_memory(bool ownMemory) {
	swigCMemOwn = ownMemory;
  }

  ~HUnicodeOptions() {
    Dispose();
  }

  public virtual void Dispose() {
    lock(this) {
      if (swigCPtr.Handle != IntPtr.Zero) {
        if (swigCMemOwn) {
          swigCMemOwn = false;
          HCSMVOPINVOKE.delete_HUnicodeOptions(swigCPtr);
        }
        swigCPtr = new HandleRef(null, IntPtr.Zero);
      }
      GC.SuppressFinalize(this);
    }
  }

  public string m_data {
    set {
      HCSMVOPINVOKE.HUnicodeOptions_m_data_set(swigCPtr, value);
    } 
    get {
      string ret = System.Runtime.InteropServices.Marshal.PtrToStringUni(HCSMVOPINVOKE.HUnicodeOptions_m_data_get(swigCPtr));
      return ret;
    } 
  }

  public int m_length {
    set {
      HCSMVOPINVOKE.HUnicodeOptions_m_length_set(swigCPtr, value);
    } 
    get {
      int ret = HCSMVOPINVOKE.HUnicodeOptions_m_length_get(swigCPtr);
      return ret;
    } 
  }

  public bool m_valid {
    set {
      HCSMVOPINVOKE.HUnicodeOptions_m_valid_set(swigCPtr, value);
    } 
    get {
      bool ret = HCSMVOPINVOKE.HUnicodeOptions_m_valid_get(swigCPtr);
      return ret;
    } 
  }

  public HUnicodeOptions() : this(HCSMVOPINVOKE.new_HUnicodeOptions__SWIG_0(), true) {
  }

  public HUnicodeOptions(HUnicodeOptions in_data) : this(HCSMVOPINVOKE.new_HUnicodeOptions__SWIG_1(HUnicodeOptions.getCPtr(in_data)), true) {
    if (HCSMVOPINVOKE.SWIGPendingException.Pending) throw HCSMVOPINVOKE.SWIGPendingException.Retrieve();
  }

  public bool IsValid() {
    bool ret = HCSMVOPINVOKE.HUnicodeOptions_IsValid(swigCPtr);
    return ret;
  }

  public string Show_Unicode_Options() {
    string ret = System.Runtime.InteropServices.Marshal.PtrToStringUni(HCSMVOPINVOKE.HUnicodeOptions_Show_Unicode_Options(swigCPtr));
    return ret;
  }

  public string Show_One_Net_Unicode_Option(string which) {
    string ret = System.Runtime.InteropServices.Marshal.PtrToStringUni(HCSMVOPINVOKE.HUnicodeOptions_Show_One_Net_Unicode_Option(swigCPtr, which));
    return ret;
  }

  public string Show_One_Unicode_Option(string which) {
    string ret = System.Runtime.InteropServices.Marshal.PtrToStringUni(HCSMVOPINVOKE.HUnicodeOptions_Show_One_Unicode_Option(swigCPtr, which));
    return ret;
  }

  public string Show_Net_Unicode_Options() {
    string ret = System.Runtime.InteropServices.Marshal.PtrToStringUni(HCSMVOPINVOKE.HUnicodeOptions_Show_Net_Unicode_Options(swigCPtr));
    return ret;
  }

}
