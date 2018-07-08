/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 2.0.4
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.techsoft.hoops;

public final class ConstFRType {
  public final static ConstFRType ConstFRNoSimpType = new ConstFRType("ConstFRNoSimpType");
  public final static ConstFRType ConstFRNoSimpAAType = new ConstFRType("ConstFRNoSimpAAType");
  public final static ConstFRType ConstFRWireframeBBoxType = new ConstFRType("ConstFRWireframeBBoxType");
  public final static ConstFRType ConstFRSolidBBoxType = new ConstFRType("ConstFRSolidBBoxType");
  public final static ConstFRType ConstFRWireframeType = new ConstFRType("ConstFRWireframeType");
  public final static ConstFRType ConstFRLodClampType = new ConstFRType("ConstFRLodClampType");
  public final static ConstFRType ConstFRLodThresholdType = new ConstFRType("ConstFRLodThresholdType");
  public final static ConstFRType ConstFRUserType = new ConstFRType("ConstFRUserType");
  public final static ConstFRType ConstFRExtentType = new ConstFRType("ConstFRExtentType");

  public final int swigValue() {
    return swigValue;
  }

  public String toString() {
    return swigName;
  }

  public static ConstFRType swigToEnum(int swigValue) {
    if (swigValue < swigValues.length && swigValue >= 0 && swigValues[swigValue].swigValue == swigValue)
      return swigValues[swigValue];
    for (int i = 0; i < swigValues.length; i++)
      if (swigValues[i].swigValue == swigValue)
        return swigValues[i];
    throw new IllegalArgumentException("No enum " + ConstFRType.class + " with value " + swigValue);
  }

  private ConstFRType(String swigName) {
    this.swigName = swigName;
    this.swigValue = swigNext++;
  }

  private ConstFRType(String swigName, int swigValue) {
    this.swigName = swigName;
    this.swigValue = swigValue;
    swigNext = swigValue+1;
  }

  private ConstFRType(String swigName, ConstFRType swigEnum) {
    this.swigName = swigName;
    this.swigValue = swigEnum.swigValue;
    swigNext = this.swigValue+1;
  }

  private static ConstFRType[] swigValues = { ConstFRNoSimpType, ConstFRNoSimpAAType, ConstFRWireframeBBoxType, ConstFRSolidBBoxType, ConstFRWireframeType, ConstFRLodClampType, ConstFRLodThresholdType, ConstFRUserType, ConstFRExtentType };
  private static int swigNext = 0;
  private final int swigValue;
  private final String swigName;
}
