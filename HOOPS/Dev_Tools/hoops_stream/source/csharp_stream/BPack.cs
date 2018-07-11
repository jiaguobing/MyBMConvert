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


public class BPack : IDisposable {
  private HandleRef swigCPtr;
  protected bool swigCMemOwn;

  public BPack(IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new HandleRef(this, cPtr);
  }

  public static HandleRef getCPtr(BPack obj) {
    return (obj == null) ? new HandleRef(null, IntPtr.Zero) : obj.swigCPtr;
  }

  ~BPack() {
    Dispose();
  }

  public virtual void Dispose() {
    lock(this) {
      if (swigCPtr.Handle != IntPtr.Zero) {
        if (swigCMemOwn) {
          swigCMemOwn = false;
          HCSSTREAMPINVOKE.delete_BPack(swigCPtr);
        }
        swigCPtr = new HandleRef(null, IntPtr.Zero);
      }
      GC.SuppressFinalize(this);
    }
  }

  public BPack() : this(HCSSTREAMPINVOKE.new_BPack(), true) {
  }

  public void InitWrite(int size, IntPtr pointer) {
    HCSSTREAMPINVOKE.BPack_InitWrite(swigCPtr, size, pointer);
  }

  public void InitRead(int size, IntPtr pointer) {
    HCSSTREAMPINVOKE.BPack_InitRead(swigCPtr, size, pointer);
  }

  public void Put(int numbits, int val) {
    HCSSTREAMPINVOKE.BPack_Put(swigCPtr, numbits, val);
  }

  public int Get(int numbits) {
    int ret = HCSSTREAMPINVOKE.BPack_Get(swigCPtr, numbits);
    return ret;
  }

  public int NumBytes() {
    int ret = HCSSTREAMPINVOKE.BPack_NumBytes(swigCPtr);
    return ret;
  }

  public void SetCanReallocate(int val) {
    HCSSTREAMPINVOKE.BPack_SetCanReallocate(swigCPtr, val);
  }

  public int GetStatus() {
    int ret = HCSSTREAMPINVOKE.BPack_GetStatus(swigCPtr);
    return ret;
  }

  public void SwapBytes() {
    HCSSTREAMPINVOKE.BPack_SwapBytes(swigCPtr);
  }

}