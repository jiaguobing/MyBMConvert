/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 2.0.4
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.techsoft.hoops.PMI.Tolerance;

public final class Type {
  public final static com.techsoft.hoops.PMI.Tolerance.Type Unknown = new com.techsoft.hoops.PMI.Tolerance.Type("Unknown");
  public final static com.techsoft.hoops.PMI.Tolerance.Type Angularity = new com.techsoft.hoops.PMI.Tolerance.Type("Angularity");
  public final static com.techsoft.hoops.PMI.Tolerance.Type Circularity = new com.techsoft.hoops.PMI.Tolerance.Type("Circularity");
  public final static com.techsoft.hoops.PMI.Tolerance.Type CircularRunout = new com.techsoft.hoops.PMI.Tolerance.Type("CircularRunout");
  public final static com.techsoft.hoops.PMI.Tolerance.Type Concentricity = new com.techsoft.hoops.PMI.Tolerance.Type("Concentricity");
  public final static com.techsoft.hoops.PMI.Tolerance.Type Cylindricity = new com.techsoft.hoops.PMI.Tolerance.Type("Cylindricity");
  public final static com.techsoft.hoops.PMI.Tolerance.Type Flatness = new com.techsoft.hoops.PMI.Tolerance.Type("Flatness");
  public final static com.techsoft.hoops.PMI.Tolerance.Type LineProfile = new com.techsoft.hoops.PMI.Tolerance.Type("LineProfile");
  public final static com.techsoft.hoops.PMI.Tolerance.Type Parallelism = new com.techsoft.hoops.PMI.Tolerance.Type("Parallelism");
  public final static com.techsoft.hoops.PMI.Tolerance.Type Perpendicularity = new com.techsoft.hoops.PMI.Tolerance.Type("Perpendicularity");
  public final static com.techsoft.hoops.PMI.Tolerance.Type Position = new com.techsoft.hoops.PMI.Tolerance.Type("Position");
  public final static com.techsoft.hoops.PMI.Tolerance.Type Straightness = new com.techsoft.hoops.PMI.Tolerance.Type("Straightness");
  public final static com.techsoft.hoops.PMI.Tolerance.Type SurfaceProfile = new com.techsoft.hoops.PMI.Tolerance.Type("SurfaceProfile");
  public final static com.techsoft.hoops.PMI.Tolerance.Type Symmetry = new com.techsoft.hoops.PMI.Tolerance.Type("Symmetry");
  public final static com.techsoft.hoops.PMI.Tolerance.Type TotalRunout = new com.techsoft.hoops.PMI.Tolerance.Type("TotalRunout");

  public final int swigValue() {
    return swigValue;
  }

  public String toString() {
    return swigName;
  }

  public static Type swigToEnum(int swigValue) {
    if (swigValue < swigValues.length && swigValue >= 0 && swigValues[swigValue].swigValue == swigValue)
      return swigValues[swigValue];
    for (int i = 0; i < swigValues.length; i++)
      if (swigValues[i].swigValue == swigValue)
        return swigValues[i];
    throw new IllegalArgumentException("No enum " + Type.class + " with value " + swigValue);
  }

  private Type(String swigName) {
    this.swigName = swigName;
    this.swigValue = swigNext++;
  }

  private Type(String swigName, int swigValue) {
    this.swigName = swigName;
    this.swigValue = swigValue;
    swigNext = swigValue+1;
  }

  private Type(String swigName, Type swigEnum) {
    this.swigName = swigName;
    this.swigValue = swigEnum.swigValue;
    swigNext = this.swigValue+1;
  }

  private static Type[] swigValues = { Unknown, Angularity, Circularity, CircularRunout, Concentricity, Cylindricity, Flatness, LineProfile, Parallelism, Perpendicularity, Position, Straightness, SurfaceProfile, Symmetry, TotalRunout };
  private static int swigNext = 0;
  private final int swigValue;
  private final String swigName;
}

