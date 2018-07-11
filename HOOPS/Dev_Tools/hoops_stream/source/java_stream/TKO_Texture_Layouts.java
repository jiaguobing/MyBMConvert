/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 1.3.40
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.techsoft.hoops;

public final class TKO_Texture_Layouts {
  public final static TKO_Texture_Layouts TKO_Texture_Layout_Rectilinear = new TKO_Texture_Layouts("TKO_Texture_Layout_Rectilinear");
  public final static TKO_Texture_Layouts TKO_Texture_Layout_Spherical = new TKO_Texture_Layouts("TKO_Texture_Layout_Spherical");
  public final static TKO_Texture_Layouts TKO_Texture_Layout_Hemispherical = new TKO_Texture_Layouts("TKO_Texture_Layout_Hemispherical");
  public final static TKO_Texture_Layouts TKO_Texture_Layout_Cubic_Faces = new TKO_Texture_Layouts("TKO_Texture_Layout_Cubic_Faces");
  public final static TKO_Texture_Layouts TKO_Texture_Layout_Unknown = new TKO_Texture_Layouts("TKO_Texture_Layout_Unknown");

  public final int swigValue() {
    return swigValue;
  }

  public String toString() {
    return swigName;
  }

  public static TKO_Texture_Layouts swigToEnum(int swigValue) {
    if (swigValue < swigValues.length && swigValue >= 0 && swigValues[swigValue].swigValue == swigValue)
      return swigValues[swigValue];
    for (int i = 0; i < swigValues.length; i++)
      if (swigValues[i].swigValue == swigValue)
        return swigValues[i];
    throw new IllegalArgumentException("No enum " + TKO_Texture_Layouts.class + " with value " + swigValue);
  }

  private TKO_Texture_Layouts(String swigName) {
    this.swigName = swigName;
    this.swigValue = swigNext++;
  }

  private TKO_Texture_Layouts(String swigName, int swigValue) {
    this.swigName = swigName;
    this.swigValue = swigValue;
    swigNext = swigValue+1;
  }

  private TKO_Texture_Layouts(String swigName, TKO_Texture_Layouts swigEnum) {
    this.swigName = swigName;
    this.swigValue = swigEnum.swigValue;
    swigNext = this.swigValue+1;
  }

  private static TKO_Texture_Layouts[] swigValues = { TKO_Texture_Layout_Rectilinear, TKO_Texture_Layout_Spherical, TKO_Texture_Layout_Hemispherical, TKO_Texture_Layout_Cubic_Faces, TKO_Texture_Layout_Unknown };
  private static int swigNext = 0;
  private final int swigValue;
  private final String swigName;
}
