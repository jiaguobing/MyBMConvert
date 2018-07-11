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


public class TK_Image_Data_Buffer : IDisposable {
  private HandleRef swigCPtr;
  protected bool swigCMemOwn;

  public TK_Image_Data_Buffer(IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new HandleRef(this, cPtr);
  }

  public static HandleRef getCPtr(TK_Image_Data_Buffer obj) {
    return (obj == null) ? new HandleRef(null, IntPtr.Zero) : obj.swigCPtr;
  }

  ~TK_Image_Data_Buffer() {
    Dispose();
  }

  public virtual void Dispose() {
    lock(this) {
      if (swigCPtr.Handle != IntPtr.Zero) {
        if (swigCMemOwn) {
          swigCMemOwn = false;
          HCSSTREAMPINVOKE.delete_TK_Image_Data_Buffer(swigCPtr);
        }
        swigCPtr = new HandleRef(null, IntPtr.Zero);
      }
      GC.SuppressFinalize(this);
    }
  }

  public TK_Image_Data_Buffer() : this(HCSSTREAMPINVOKE.new_TK_Image_Data_Buffer(), true) {
  }

  public void Resize(uint size) {
    HCSSTREAMPINVOKE.TK_Image_Data_Buffer_Resize(swigCPtr, size);
  }

  public void Expand(uint size) {
    HCSSTREAMPINVOKE.TK_Image_Data_Buffer_Expand(swigCPtr, size);
  }

  public void Reset() {
    HCSSTREAMPINVOKE.TK_Image_Data_Buffer_Reset(swigCPtr);
  }

  public uint Size() {
    uint ret = HCSSTREAMPINVOKE.TK_Image_Data_Buffer_Size(swigCPtr);
    return ret;
  }

  public uint Used() {
    uint ret = HCSSTREAMPINVOKE.TK_Image_Data_Buffer_Used__SWIG_0(swigCPtr);
    return ret;
  }

  public IntPtr Buffer() {return HCSSTREAMPINVOKE.TK_Image_Data_Buffer_Buffer__SWIG_0(swigCPtr);}

}
