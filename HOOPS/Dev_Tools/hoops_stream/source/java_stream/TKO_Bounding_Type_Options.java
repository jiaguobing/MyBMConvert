/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 1.3.40
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.techsoft.hoops;

public final class TKO_Bounding_Type_Options {
  public final static TKO_Bounding_Type_Options TKO_Bounding_Type_Cuboid = new TKO_Bounding_Type_Options("TKO_Bounding_Type_Cuboid", HJSTREAMJNI.TKO_Bounding_Type_Cuboid_get());
  public final static TKO_Bounding_Type_Options TKO_Bounding_Type_Sphere = new TKO_Bounding_Type_Options("TKO_Bounding_Type_Sphere", HJSTREAMJNI.TKO_Bounding_Type_Sphere_get());

  public final int swigValue() {
    return swigValue;
  }

  public String toString() {
    return swigName;
  }

  public static TKO_Bounding_Type_Options swigToEnum(int swigValue) {
    if (swigValue < swigValues.length && swigValue >= 0 && swigValues[swigValue].swigValue == swigValue)
      return swigValues[swigValue];
    for (int i = 0; i < swigValues.length; i++)
      if (swigValues[i].swigValue == swigValue)
        return swigValues[i];
    throw new IllegalArgumentException("No enum " + TKO_Bounding_Type_Options.class + " with value " + swigValue);
  }

  private TKO_Bounding_Type_Options(String swigName) {
    this.swigName = swigName;
    this.swigValue = swigNext++;
  }

  private TKO_Bounding_Type_Options(String swigName, int swigValue) {
    this.swigName = swigName;
    this.swigValue = swigValue;
    swigNext = swigValue+1;
  }

  private TKO_Bounding_Type_Options(String swigName, TKO_Bounding_Type_Options swigEnum) {
    this.swigName = swigName;
    this.swigValue = swigEnum.swigValue;
    swigNext = this.swigValue+1;
  }

  private static TKO_Bounding_Type_Options[] swigValues = { TKO_Bounding_Type_Cuboid, TKO_Bounding_Type_Sphere };
  private static int swigNext = 0;
  private final int swigValue;
  private final String swigName;
}

