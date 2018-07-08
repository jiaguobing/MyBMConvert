/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 2.0.4
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.techsoft.hoops;
import com.techsoft.hoops.*;
public class FeatureControlFrameEntity extends Entity {
  private long swigCPtr;

  public FeatureControlFrameEntity(long cPtr, boolean cMemoryOwn) {
    super(HJMVOJNI.FeatureControlFrameEntity_SWIGUpcast(cPtr), cMemoryOwn);
    swigCPtr = cPtr;
  }

  public static long getCPtr(FeatureControlFrameEntity obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        HJMVOJNI.delete_FeatureControlFrameEntity(swigCPtr);
      }
      swigCPtr = 0;
    }
    super.delete();
  }

  public FeatureControlFrameEntity(long segment_key) {
    this(HJMVOJNI.new_FeatureControlFrameEntity__SWIG_0(segment_key), true);
  }

  public FeatureControlFrameEntity() {
    this(HJMVOJNI.new_FeatureControlFrameEntity__SWIG_1(), true);
  }

  public FeatureControlFrameEntity(FeatureControlFrameEntity that) {
    this(HJMVOJNI.new_FeatureControlFrameEntity__SWIG_2(FeatureControlFrameEntity.getCPtr(that), that), true);
  }

  public void Set(FeatureControlFrameEntity that) {
    HJMVOJNI.FeatureControlFrameEntity_Set(swigCPtr, this, FeatureControlFrameEntity.getCPtr(that), that);
  }

  public com.techsoft.hoops.PMI.Type GetType() {
    return com.techsoft.hoops.PMI.Type.swigToEnum(HJMVOJNI.FeatureControlFrameEntity_GetType(swigCPtr, this));
  }

  public void GetToleranceType(int[] out_tolerance_type, TextAttributes out_text_attributes) {
    HJMVOJNI.FeatureControlFrameEntity_GetToleranceType(swigCPtr, this, out_tolerance_type, TextAttributes.getCPtr(out_text_attributes), out_text_attributes);
  }

  public void SetToleranceType(com.techsoft.hoops.PMI.Tolerance.Type in_tolerance_type, TextAttributes in_text_attributes) {
    HJMVOJNI.FeatureControlFrameEntity_SetToleranceType(swigCPtr, this, in_tolerance_type.swigValue(), TextAttributes.getCPtr(in_text_attributes), in_text_attributes);
  }

  public void GetDiameterModifierType(int[] out_diameter_modifier_type, TextAttributes out_text_attributes) {
    HJMVOJNI.FeatureControlFrameEntity_GetDiameterModifierType(swigCPtr, this, out_diameter_modifier_type, TextAttributes.getCPtr(out_text_attributes), out_text_attributes);
  }

  public void SetDiameterModifierType(com.techsoft.hoops.PMI.DiameterModifier.Type in_diameter_modifier_type, TextAttributes in_text_attributes) {
    HJMVOJNI.FeatureControlFrameEntity_SetDiameterModifierType(swigCPtr, this, in_diameter_modifier_type.swigValue(), TextAttributes.getCPtr(in_text_attributes), in_text_attributes);
  }

  public void GetZoneToleranceMagnitude(PMI_String out_zone_tolerance_magnitude, TextAttributes out_text_attributes) {
    HJMVOJNI.FeatureControlFrameEntity_GetZoneToleranceMagnitude(swigCPtr, this, PMI_String.getCPtr(out_zone_tolerance_magnitude), out_zone_tolerance_magnitude, TextAttributes.getCPtr(out_text_attributes), out_text_attributes);
  }

  public void SetZoneToleranceMagnitude(PMI_String in_zone_tolerance_magnitude, TextAttributes in_text_attributes) {
    HJMVOJNI.FeatureControlFrameEntity_SetZoneToleranceMagnitude(swigCPtr, this, PMI_String.getCPtr(in_zone_tolerance_magnitude), in_zone_tolerance_magnitude, TextAttributes.getCPtr(in_text_attributes), in_text_attributes);
  }

  public void GetFirstZoneModifierType(int[] out_zone_modifier_type, TextAttributes out_text_attributes) {
    HJMVOJNI.FeatureControlFrameEntity_GetFirstZoneModifierType(swigCPtr, this, out_zone_modifier_type, TextAttributes.getCPtr(out_text_attributes), out_text_attributes);
  }

  public void SetFirstZoneModifierType(com.techsoft.hoops.PMI.ZoneModifier.Type in_zone_modifier_type, TextAttributes in_text_attributes) {
    HJMVOJNI.FeatureControlFrameEntity_SetFirstZoneModifierType(swigCPtr, this, in_zone_modifier_type.swigValue(), TextAttributes.getCPtr(in_text_attributes), in_text_attributes);
  }

  public void GetSecondZoneModifierType(int[] out_zone_modifier_type, TextAttributes out_text_attributes) {
    HJMVOJNI.FeatureControlFrameEntity_GetSecondZoneModifierType(swigCPtr, this, out_zone_modifier_type, TextAttributes.getCPtr(out_text_attributes), out_text_attributes);
  }

  public void SetSecondZoneModifierType(com.techsoft.hoops.PMI.ZoneModifier.Type in_zone_modifier_type, TextAttributes in_text_attributes) {
    HJMVOJNI.FeatureControlFrameEntity_SetSecondZoneModifierType(swigCPtr, this, in_zone_modifier_type.swigValue(), TextAttributes.getCPtr(in_text_attributes), in_text_attributes);
  }

  public void GetProjectedMagnitude(PMI_String out_projected_magnitude, TextAttributes out_text_attributes) {
    HJMVOJNI.FeatureControlFrameEntity_GetProjectedMagnitude(swigCPtr, this, PMI_String.getCPtr(out_projected_magnitude), out_projected_magnitude, TextAttributes.getCPtr(out_text_attributes), out_text_attributes);
  }

  public void SetProjectedMagnitude(PMI_String in_projected_magnitude, TextAttributes in_text_attributes) {
    HJMVOJNI.FeatureControlFrameEntity_SetProjectedMagnitude(swigCPtr, this, PMI_String.getCPtr(in_projected_magnitude), in_projected_magnitude, TextAttributes.getCPtr(in_text_attributes), in_text_attributes);
  }

  public int GetDatumReferenceCount() {return HJMVOJNI.FeatureControlFrameEntity_GetDatumReferenceCount(swigCPtr, this);}

  public void GetDatumReferences(int[] out_count, PMI_String[] out_labels, TextAttributes[] out_label_attributes, int[] out_modifiers, TextAttributes[] out_modifier_attributes) {
    HJMVOJNI.FeatureControlFrameEntity_GetDatumReferences(swigCPtr, this, out_count, PMI_String.cArrayUnwrap(out_labels), TextAttributes.cArrayUnwrap(out_label_attributes), out_modifiers, TextAttributes.cArrayUnwrap(out_modifier_attributes));
  }

  public void SetDatumReferences(int in_count, PMI_String[] in_labels, TextAttributes[] in_label_attributes, int[] in_modifiers, TextAttributes[] in_modifier_attributes) {
    HJMVOJNI.FeatureControlFrameEntity_SetDatumReferences(swigCPtr, this, in_count, PMI_String.cArrayUnwrap(in_labels), TextAttributes.cArrayUnwrap(in_label_attributes), in_modifiers, TextAttributes.cArrayUnwrap(in_modifier_attributes));
  }

}