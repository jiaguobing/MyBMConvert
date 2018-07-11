/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 1.3.40
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.techsoft.hoops;

public class Internal_Translator extends BControlledMemoryObject {
  private long swigCPtr;

  protected Internal_Translator(long cPtr, boolean cMemoryOwn) {
    super(HJSTREAMJNI.SWIGInternal_TranslatorUpcast(cPtr), cMemoryOwn);
    swigCPtr = cPtr;
  }

  protected static long getCPtr(Internal_Translator obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        HJSTREAMJNI.delete_Internal_Translator(swigCPtr);
      }
      swigCPtr = 0;
    }
    super.delete();
  }

  public Internal_Translator() {
    this(HJSTREAMJNI.new_Internal_Translator(), true);
  }

  public TK_Status add_pair(int index, int key) {
    return TK_Status.swigToEnum(HJSTREAMJNI.Internal_Translator_add_pair(swigCPtr, this, index, key));
  }

  public TK_Status add_variant(int key, int variant, int value1, int value2) {
    return TK_Status.swigToEnum(HJSTREAMJNI.Internal_Translator_add_variant__SWIG_0(swigCPtr, this, key, variant, value1, value2));
  }

  public TK_Status add_variant(int key, int variant, int value1) {
    return TK_Status.swigToEnum(HJSTREAMJNI.Internal_Translator_add_variant__SWIG_1(swigCPtr, this, key, variant, value1));
  }

  public TK_Status add_bounds(int key, float[] bounds) {
    return TK_Status.swigToEnum(HJSTREAMJNI.Internal_Translator_add_bounds(swigCPtr, this, key, bounds));
  }

  public TK_Status index_to_key(int index, int[] key) {
    return TK_Status.swigToEnum(HJSTREAMJNI.Internal_Translator_index_to_key(swigCPtr, this, index, key));
  }

  public TK_Status key_to_index(int key, int[] index) {
    return TK_Status.swigToEnum(HJSTREAMJNI.Internal_Translator_key_to_index(swigCPtr, this, key, index));
  }

  public TK_Status key_variant_offset(int key, int variant, int[] offset, int[] length, int[] index) {
    return TK_Status.swigToEnum(HJSTREAMJNI.Internal_Translator_key_variant_offset__SWIG_0(swigCPtr, this, key, variant, offset, length, index));
  }

  public TK_Status key_bounds(int key, float[] bounds) {
    return TK_Status.swigToEnum(HJSTREAMJNI.Internal_Translator_key_bounds(swigCPtr, this, key, bounds));
  }

  public int used() {
    return HJSTREAMJNI.Internal_Translator_used(swigCPtr, this);
  }

  public void clean() {
    HJSTREAMJNI.Internal_Translator_clean(swigCPtr, this);
  }

  public TK_Status key_variant_offset(int key, int variant, int[] offset) {
    return TK_Status.swigToEnum(HJSTREAMJNI.Internal_Translator_key_variant_offset__SWIG_1(swigCPtr, this, key, variant, offset));
  }

  public TK_Status key_variant_offset(int key, int variant, int[] offset, int[] length) {
    return TK_Status.swigToEnum(HJSTREAMJNI.Internal_Translator_key_variant_offset__SWIG_2(swigCPtr, this, key, variant, offset, length));
  }

}
