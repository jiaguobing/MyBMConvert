/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 2.0.4
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.techsoft.hoops;

public class HOutputHandlerOptions {
  private long swigCPtr;
  protected boolean swigCMemOwn;

  public HOutputHandlerOptions(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  public static long getCPtr(HOutputHandlerOptions obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        HJMVOJNI.delete_HOutputHandlerOptions(swigCPtr);
      }
      swigCPtr = 0;
    }
  }

  public HOutputHandlerOptions() {
    this(HJMVOJNI.new_HOutputHandlerOptions__SWIG_0(), true);
  }

  public String GetType() {
    return HJMVOJNI.HOutputHandlerOptions_GetType(swigCPtr, this);
  }

  public boolean PopulateOptions() {
    return HJMVOJNI.HOutputHandlerOptions_PopulateOptions(swigCPtr, this);
  }

  public HOutputHandlerOptions(HOutputHandlerOptions o) {
    this(HJMVOJNI.new_HOutputHandlerOptions__SWIG_1(HOutputHandlerOptions.getCPtr(o), o), true);
  }

  public void Init(HOutputHandlerOptions o) {
    HJMVOJNI.HOutputHandlerOptions_Init(swigCPtr, this, HOutputHandlerOptions.getCPtr(o), o);
  }

  public void setM_pHBaseView(HBaseView value) {
    HJMVOJNI.HOutputHandlerOptions_m_pHBaseView_set(swigCPtr, this, HBaseView.getCPtr(value), value);
  }

  public HBaseView getM_pHBaseView() {
    long cPtr = HJMVOJNI.HOutputHandlerOptions_m_pHBaseView_get(swigCPtr, this);
    return (cPtr == 0) ? null : new HBaseView(cPtr, false);
  }

  public HBaseView View() {
    long cPtr = HJMVOJNI.HOutputHandlerOptions_View__SWIG_0(swigCPtr, this);
    return (cPtr == 0) ? null : new HBaseView(cPtr, false);
  }

  public void View(HBaseView v) {
    HJMVOJNI.HOutputHandlerOptions_View__SWIG_1(swigCPtr, this, HBaseView.getCPtr(v), v);
  }

  public void setM_pHBaseModel(HBaseModel value) {
    HJMVOJNI.HOutputHandlerOptions_m_pHBaseModel_set(swigCPtr, this, HBaseModel.getCPtr(value), value);
  }

  public HBaseModel getM_pHBaseModel() {
    long cPtr = HJMVOJNI.HOutputHandlerOptions_m_pHBaseModel_get(swigCPtr, this);
    return (cPtr == 0) ? null : new HBaseModel(cPtr, false);
  }

  public HBaseModel Model() {
    long cPtr = HJMVOJNI.HOutputHandlerOptions_Model__SWIG_0(swigCPtr, this);
    return (cPtr == 0) ? null : new HBaseModel(cPtr, false);
  }

  public void Model(HBaseModel m) {
    HJMVOJNI.HOutputHandlerOptions_Model__SWIG_1(swigCPtr, this, HBaseModel.getCPtr(m), m);
  }

  public void setM_pCamera(HCamera value) {
    HJMVOJNI.HOutputHandlerOptions_m_pCamera_set(swigCPtr, this, HCamera.getCPtr(value), value);
  }

  public HCamera getM_pCamera() {
    long cPtr = HJMVOJNI.HOutputHandlerOptions_m_pCamera_get(swigCPtr, this);
    return (cPtr == 0) ? null : new HCamera(cPtr, false);
  }

  public HCamera Camera() {
    long cPtr = HJMVOJNI.HOutputHandlerOptions_Camera__SWIG_0(swigCPtr, this);
    return (cPtr == 0) ? null : new HCamera(cPtr, false);
  }

  public void Camera(HCamera c) {
    HJMVOJNI.HOutputHandlerOptions_Camera__SWIG_1(swigCPtr, this, HCamera.getCPtr(c), c);
  }

  public void setM_Key(long value) {
    HJMVOJNI.HOutputHandlerOptions_m_Key_set(swigCPtr, this, value);
  }

  public long getM_Key() {return HJMVOJNI.HOutputHandlerOptions_m_Key_get(swigCPtr, this);}

  public long Key() {return HJMVOJNI.HOutputHandlerOptions_Key__SWIG_0(swigCPtr, this);}

  public void Key(long k) {
    HJMVOJNI.HOutputHandlerOptions_Key__SWIG_1(swigCPtr, this, k);
  }

  public void setM_pHSRAOptions(String value) {
    HJMVOJNI.HOutputHandlerOptions_m_pHSRAOptions_set(swigCPtr, this, value);
  }

  public String getM_pHSRAOptions() {
    return HJMVOJNI.HOutputHandlerOptions_m_pHSRAOptions_get(swigCPtr, this);
  }

  public StringBuffer HSRAOptions() {
    return HJMVOJNI.HOutputHandlerOptions_HSRAOptions__SWIG_0(swigCPtr, this);
  }

  public void HSRAOptions(StringBuffer tf) {
    HJMVOJNI.HOutputHandlerOptions_HSRAOptions__SWIG_1(swigCPtr, this, tf);
  }

  public void setM_pWindowColor(HPoint value) {
    HJMVOJNI.HOutputHandlerOptions_m_pWindowColor_set(swigCPtr, this, HPoint.getCPtr(value), value);
  }

  public HPoint getM_pWindowColor() {
    long cPtr = HJMVOJNI.HOutputHandlerOptions_m_pWindowColor_get(swigCPtr, this);
    return (cPtr == 0) ? null : new HPoint(cPtr, false);
  }

  public HPoint WindowColor() {
    long cPtr = HJMVOJNI.HOutputHandlerOptions_WindowColor__SWIG_0(swigCPtr, this);
    return (cPtr == 0) ? null : new HPoint(cPtr, false);
  }

  public void WindowColor(HPoint tf) {
    HJMVOJNI.HOutputHandlerOptions_WindowColor__SWIG_1(swigCPtr, this, HPoint.getCPtr(tf), tf);
  }

  public void setM_bUseWindowColor(boolean value) {
    HJMVOJNI.HOutputHandlerOptions_m_bUseWindowColor_set(swigCPtr, this, value);
  }

  public boolean getM_bUseWindowColor() {
    return HJMVOJNI.HOutputHandlerOptions_m_bUseWindowColor_get(swigCPtr, this);
  }

  public boolean UseWindowColor() {
    return HJMVOJNI.HOutputHandlerOptions_UseWindowColor__SWIG_0(swigCPtr, this);
  }

  public void UseWindowColor(boolean tf) {
    HJMVOJNI.HOutputHandlerOptions_UseWindowColor__SWIG_1(swigCPtr, this, tf);
  }

  public void setM_Window_Width(float value) {
    HJMVOJNI.HOutputHandlerOptions_m_Window_Width_set(swigCPtr, this, value);
  }

  public float getM_Window_Width() {
    return HJMVOJNI.HOutputHandlerOptions_m_Window_Width_get(swigCPtr, this);
  }

  public float WindowWidth() {
    return HJMVOJNI.HOutputHandlerOptions_WindowWidth__SWIG_0(swigCPtr, this);
  }

  public void WindowWidth(float f) {
    HJMVOJNI.HOutputHandlerOptions_WindowWidth__SWIG_1(swigCPtr, this, f);
  }

  public void setM_Window_Height(float value) {
    HJMVOJNI.HOutputHandlerOptions_m_Window_Height_set(swigCPtr, this, value);
  }

  public float getM_Window_Height() {
    return HJMVOJNI.HOutputHandlerOptions_m_Window_Height_get(swigCPtr, this);
  }

  public float WindowHeight() {
    return HJMVOJNI.HOutputHandlerOptions_WindowHeight__SWIG_0(swigCPtr, this);
  }

  public void WindowHeight(float f) {
    HJMVOJNI.HOutputHandlerOptions_WindowHeight__SWIG_1(swigCPtr, this, f);
  }

  public void setM_Paper_Width(float value) {
    HJMVOJNI.HOutputHandlerOptions_m_Paper_Width_set(swigCPtr, this, value);
  }

  public float getM_Paper_Width() {
    return HJMVOJNI.HOutputHandlerOptions_m_Paper_Width_get(swigCPtr, this);
  }

  public float PaperWidth() {
    return HJMVOJNI.HOutputHandlerOptions_PaperWidth__SWIG_0(swigCPtr, this);
  }

  public void PaperWidth(float f) {
    HJMVOJNI.HOutputHandlerOptions_PaperWidth__SWIG_1(swigCPtr, this, f);
  }

  public void setM_Paper_Height(float value) {
    HJMVOJNI.HOutputHandlerOptions_m_Paper_Height_set(swigCPtr, this, value);
  }

  public float getM_Paper_Height() {
    return HJMVOJNI.HOutputHandlerOptions_m_Paper_Height_get(swigCPtr, this);
  }

  public float PaperHeight() {
    return HJMVOJNI.HOutputHandlerOptions_PaperHeight__SWIG_0(swigCPtr, this);
  }

  public void PaperHeight(float f) {
    HJMVOJNI.HOutputHandlerOptions_PaperHeight__SWIG_1(swigCPtr, this, f);
  }

  public void setM_ForceGrayscale(boolean value) {
    HJMVOJNI.HOutputHandlerOptions_m_ForceGrayscale_set(swigCPtr, this, value);
  }

  public boolean getM_ForceGrayscale() {
    return HJMVOJNI.HOutputHandlerOptions_m_ForceGrayscale_get(swigCPtr, this);
  }

  public boolean ForceGrayscale() {
    return HJMVOJNI.HOutputHandlerOptions_ForceGrayscale__SWIG_0(swigCPtr, this);
  }

  public void ForceGrayscale(boolean tf) {
    HJMVOJNI.HOutputHandlerOptions_ForceGrayscale__SWIG_1(swigCPtr, this, tf);
  }

  public void setM_lineFuseThreshold(long value) {
    HJMVOJNI.HOutputHandlerOptions_m_lineFuseThreshold_set(swigCPtr, this, value);
  }

  public long getM_lineFuseThreshold() {
    return HJMVOJNI.HOutputHandlerOptions_m_lineFuseThreshold_get(swigCPtr, this);
  }

  public long LineFuseThreshold() {
    return HJMVOJNI.HOutputHandlerOptions_LineFuseThreshold__SWIG_0(swigCPtr, this);
  }

  public void LineFuseThreshold(long tf) {
    HJMVOJNI.HOutputHandlerOptions_LineFuseThreshold__SWIG_1(swigCPtr, this, tf);
  }

  public void setM_bFastPrint(boolean value) {
    HJMVOJNI.HOutputHandlerOptions_m_bFastPrint_set(swigCPtr, this, value);
  }

  public boolean getM_bFastPrint() {
    return HJMVOJNI.HOutputHandlerOptions_m_bFastPrint_get(swigCPtr, this);
  }

  public boolean FastPrint() {
    return HJMVOJNI.HOutputHandlerOptions_FastPrint__SWIG_0(swigCPtr, this);
  }

  public void FastPrint(boolean tf) {
    HJMVOJNI.HOutputHandlerOptions_FastPrint__SWIG_1(swigCPtr, this, tf);
  }

  public void setM_bConserveMemory(boolean value) {
    HJMVOJNI.HOutputHandlerOptions_m_bConserveMemory_set(swigCPtr, this, value);
  }

  public boolean getM_bConserveMemory() {
    return HJMVOJNI.HOutputHandlerOptions_m_bConserveMemory_get(swigCPtr, this);
  }

  public boolean ConserveMemory() {
    return HJMVOJNI.HOutputHandlerOptions_ConserveMemory__SWIG_0(swigCPtr, this);
  }

  public void ConserveMemory(boolean tf) {
    HJMVOJNI.HOutputHandlerOptions_ConserveMemory__SWIG_1(swigCPtr, this, tf);
  }

  public void setM_bWriteAnimations(boolean value) {
    HJMVOJNI.HOutputHandlerOptions_m_bWriteAnimations_set(swigCPtr, this, value);
  }

  public boolean getM_bWriteAnimations() {
    return HJMVOJNI.HOutputHandlerOptions_m_bWriteAnimations_get(swigCPtr, this);
  }

  public boolean WriteAnimations() {
    return HJMVOJNI.HOutputHandlerOptions_WriteAnimations__SWIG_0(swigCPtr, this);
  }

  public void WriteAnimations(boolean tf) {
    HJMVOJNI.HOutputHandlerOptions_WriteAnimations__SWIG_1(swigCPtr, this, tf);
  }

  public void setM_bWindowFrame(boolean value) {
    HJMVOJNI.HOutputHandlerOptions_m_bWindowFrame_set(swigCPtr, this, value);
  }

  public boolean getM_bWindowFrame() {
    return HJMVOJNI.HOutputHandlerOptions_m_bWindowFrame_get(swigCPtr, this);
  }

  public boolean WindowFrame() {
    return HJMVOJNI.HOutputHandlerOptions_WindowFrame__SWIG_0(swigCPtr, this);
  }

  public void WindowFrame(boolean tf) {
    HJMVOJNI.HOutputHandlerOptions_WindowFrame__SWIG_1(swigCPtr, this, tf);
  }

  public void setM_bUseSubscreen(boolean value) {
    HJMVOJNI.HOutputHandlerOptions_m_bUseSubscreen_set(swigCPtr, this, value);
  }

  public boolean getM_bUseSubscreen() {
    return HJMVOJNI.HOutputHandlerOptions_m_bUseSubscreen_get(swigCPtr, this);
  }

  public boolean UseSubscreen() {
    return HJMVOJNI.HOutputHandlerOptions_UseSubscreen__SWIG_0(swigCPtr, this);
  }

  public void UseSubscreen(boolean tf) {
    HJMVOJNI.HOutputHandlerOptions_UseSubscreen__SWIG_1(swigCPtr, this, tf);
  }

  public void setM_Image_Dpi(int value) {
    HJMVOJNI.HOutputHandlerOptions_m_Image_Dpi_set(swigCPtr, this, value);
  }

  public int getM_Image_Dpi() {
    return HJMVOJNI.HOutputHandlerOptions_m_Image_Dpi_get(swigCPtr, this);
  }

  public int ImageDpi() {
    return HJMVOJNI.HOutputHandlerOptions_ImageDpi__SWIG_0(swigCPtr, this);
  }

  public void ImageDpi(int dpi) {
    HJMVOJNI.HOutputHandlerOptions_ImageDpi__SWIG_1(swigCPtr, this, dpi);
  }

  public void setM_Paper_Dpi(int value) {
    HJMVOJNI.HOutputHandlerOptions_m_Paper_Dpi_set(swigCPtr, this, value);
  }

  public int getM_Paper_Dpi() {
    return HJMVOJNI.HOutputHandlerOptions_m_Paper_Dpi_get(swigCPtr, this);
  }

  public int PaperDpi() {
    return HJMVOJNI.HOutputHandlerOptions_PaperDpi__SWIG_0(swigCPtr, this);
  }

  public void PaperDpi(int dpi) {
    HJMVOJNI.HOutputHandlerOptions_PaperDpi__SWIG_1(swigCPtr, this, dpi);
  }

  public void setM_pExtendedData(long value) {
    HJMVOJNI.HOutputHandlerOptions_m_pExtendedData_set(swigCPtr, this, value);
  }

  public long getM_pExtendedData() {return (long)HJMVOJNI.HOutputHandlerOptions_m_pExtendedData_get(swigCPtr, this);}

  public long ExtendedData() {return (long)HJMVOJNI.HOutputHandlerOptions_ExtendedData__SWIG_0(swigCPtr, this);}

  public void ExtendedData(long vp) {
    HJMVOJNI.HOutputHandlerOptions_ExtendedData__SWIG_1(swigCPtr, this, vp);
  }

  public void ExtendedData(SWIGTYPE_p_HPDFOptions vp) {
    HJMVOJNI.HOutputHandlerOptions_ExtendedData__SWIG_2(swigCPtr, this, SWIGTYPE_p_HPDFOptions.getCPtr(vp));
  }

  public boolean ActivateOnPageOpen() {
    return HJMVOJNI.HOutputHandlerOptions_ActivateOnPageOpen__SWIG_0(swigCPtr, this);
  }

  public void ActivateOnPageOpen(boolean onoff) {
    HJMVOJNI.HOutputHandlerOptions_ActivateOnPageOpen__SWIG_1(swigCPtr, this, onoff);
  }

  public float AnnotLeft() {
    return HJMVOJNI.HOutputHandlerOptions_AnnotLeft__SWIG_0(swigCPtr, this);
  }

  public void AnnotLeft(float f) {
    HJMVOJNI.HOutputHandlerOptions_AnnotLeft__SWIG_1(swigCPtr, this, f);
  }

  public float AnnotRight() {
    return HJMVOJNI.HOutputHandlerOptions_AnnotRight__SWIG_0(swigCPtr, this);
  }

  public void AnnotRight(float f) {
    HJMVOJNI.HOutputHandlerOptions_AnnotRight__SWIG_1(swigCPtr, this, f);
  }

  public float AnnotBottom() {
    return HJMVOJNI.HOutputHandlerOptions_AnnotBottom__SWIG_0(swigCPtr, this);
  }

  public void AnnotBottom(float f) {
    HJMVOJNI.HOutputHandlerOptions_AnnotBottom__SWIG_1(swigCPtr, this, f);
  }

  public float AnnotTop() {
    return HJMVOJNI.HOutputHandlerOptions_AnnotTop__SWIG_0(swigCPtr, this);
  }

  public void AnnotTop(float f) {
    HJMVOJNI.HOutputHandlerOptions_AnnotTop__SWIG_1(swigCPtr, this, f);
  }

  public void setM_license(String value) {
    HJMVOJNI.HOutputHandlerOptions_m_license_set(swigCPtr, this, value);
  }

  public String getM_license() {
    return HJMVOJNI.HOutputHandlerOptions_m_license_get(swigCPtr, this);
  }

  public void setM_antialiasLevel(int value) {
    HJMVOJNI.HOutputHandlerOptions_m_antialiasLevel_set(swigCPtr, this, value);
  }

  public int getM_antialiasLevel() {
    return HJMVOJNI.HOutputHandlerOptions_m_antialiasLevel_get(swigCPtr, this);
  }

  public void setM_jpeg_quality(int value) {
    HJMVOJNI.HOutputHandlerOptions_m_jpeg_quality_set(swigCPtr, this, value);
  }

  public int getM_jpeg_quality() {
    return HJMVOJNI.HOutputHandlerOptions_m_jpeg_quality_get(swigCPtr, this);
  }

  public void setM_imageOutputBufferSize(long value) {
    HJMVOJNI.HOutputHandlerOptions_m_imageOutputBufferSize_set(swigCPtr, this, value);
  }

  public long getM_imageOutputBufferSize() {
    return HJMVOJNI.HOutputHandlerOptions_m_imageOutputBufferSize_get(swigCPtr, this);
  }

  public void setM_Version(int value) {
    HJMVOJNI.HOutputHandlerOptions_m_Version_set(swigCPtr, this, value);
  }

  public int getM_Version() {
    return HJMVOJNI.HOutputHandlerOptions_m_Version_get(swigCPtr, this);
  }

  public int Version() {
    return HJMVOJNI.HOutputHandlerOptions_Version__SWIG_0(swigCPtr, this);
  }

  public void Version(int v) {
    HJMVOJNI.HOutputHandlerOptions_Version__SWIG_1(swigCPtr, this, v);
  }

  public void setM_OutputType(H_OUTPUT_TYPE value) {
    HJMVOJNI.HOutputHandlerOptions_m_OutputType_set(swigCPtr, this, value.swigValue());
  }

  public H_OUTPUT_TYPE getM_OutputType() {
    return H_OUTPUT_TYPE.swigToEnum(HJMVOJNI.HOutputHandlerOptions_m_OutputType_get(swigCPtr, this));
  }

  public H_OUTPUT_TYPE OutputType() {
    return H_OUTPUT_TYPE.swigToEnum(HJMVOJNI.HOutputHandlerOptions_OutputType__SWIG_0(swigCPtr, this));
  }

  public void OutputType(H_OUTPUT_TYPE ot) {
    HJMVOJNI.HOutputHandlerOptions_OutputType__SWIG_1(swigCPtr, this, ot.swigValue());
  }

  public void setM_b3dOutput(boolean value) {
    HJMVOJNI.HOutputHandlerOptions_m_b3dOutput_set(swigCPtr, this, value);
  }

  public boolean getM_b3dOutput() {
    return HJMVOJNI.HOutputHandlerOptions_m_b3dOutput_get(swigCPtr, this);
  }

  public boolean Output3d() {
    return HJMVOJNI.HOutputHandlerOptions_Output3d__SWIG_0(swigCPtr, this);
  }

  public void Output3d(boolean tf) {
    HJMVOJNI.HOutputHandlerOptions_Output3d__SWIG_1(swigCPtr, this, tf);
  }

  public void setM_bAscii(boolean value) {
    HJMVOJNI.HOutputHandlerOptions_m_bAscii_set(swigCPtr, this, value);
  }

  public boolean getM_bAscii() {
    return HJMVOJNI.HOutputHandlerOptions_m_bAscii_get(swigCPtr, this);
  }

  public boolean Ascii() {
    return HJMVOJNI.HOutputHandlerOptions_Ascii__SWIG_0(swigCPtr, this);
  }

  public void Ascii(boolean tf) {
    HJMVOJNI.HOutputHandlerOptions_Ascii__SWIG_1(swigCPtr, this, tf);
  }

  public void setM_bStlNormals(boolean value) {
    HJMVOJNI.HOutputHandlerOptions_m_bStlNormals_set(swigCPtr, this, value);
  }

  public boolean getM_bStlNormals() {
    return HJMVOJNI.HOutputHandlerOptions_m_bStlNormals_get(swigCPtr, this);
  }

  public boolean StlNormals() {
    return HJMVOJNI.HOutputHandlerOptions_StlNormals__SWIG_0(swigCPtr, this);
  }

  public void StlNormals(boolean tf) {
    HJMVOJNI.HOutputHandlerOptions_StlNormals__SWIG_1(swigCPtr, this, tf);
  }

  public void setM_ratio_cm_to_internal_units(double value) {
    HJMVOJNI.HOutputHandlerOptions_m_ratio_cm_to_internal_units_set(swigCPtr, this, value);
  }

  public double getM_ratio_cm_to_internal_units() {
    return HJMVOJNI.HOutputHandlerOptions_m_ratio_cm_to_internal_units_get(swigCPtr, this);
  }

  public double PrintToScale() {
    return HJMVOJNI.HOutputHandlerOptions_PrintToScale__SWIG_0(swigCPtr, this);
  }

  public void PrintToScale(double r) {
    HJMVOJNI.HOutputHandlerOptions_PrintToScale__SWIG_1(swigCPtr, this, r);
  }

  public void setM_pBehaviorManager(HBhvBehaviorManager value) {
    HJMVOJNI.HOutputHandlerOptions_m_pBehaviorManager_set(swigCPtr, this, HBhvBehaviorManager.getCPtr(value), value);
  }

  public HBhvBehaviorManager getM_pBehaviorManager() {
    long cPtr = HJMVOJNI.HOutputHandlerOptions_m_pBehaviorManager_get(swigCPtr, this);
    return (cPtr == 0) ? null : new HBhvBehaviorManager(cPtr, false);
  }

  public void setM_pDWFModelName(String value) {
    HJMVOJNI.HOutputHandlerOptions_m_pDWFModelName_set(swigCPtr, this, value);
  }

  public String getM_pDWFModelName() {
    return HJMVOJNI.HOutputHandlerOptions_m_pDWFModelName_get(swigCPtr, this);
  }

  public void setM_pPDFResourceDirectory(String value) {
    HJMVOJNI.HOutputHandlerOptions_m_pPDFResourceDirectory_set(swigCPtr, this, value);
  }

  public String getM_pPDFResourceDirectory() {
    return HJMVOJNI.HOutputHandlerOptions_m_pPDFResourceDirectory_get(swigCPtr, this);
  }

  public void setM_pPRCAsmModelFile(long value) {
    HJMVOJNI.HOutputHandlerOptions_m_pPRCAsmModelFile_set(swigCPtr, this, value);
  }

  public long getM_pPRCAsmModelFile() {return (long)HJMVOJNI.HOutputHandlerOptions_m_pPRCAsmModelFile_get(swigCPtr, this);}

  public void setM_bActivateOnPageOpen(boolean value) {
    HJMVOJNI.HOutputHandlerOptions_m_bActivateOnPageOpen_set(swigCPtr, this, value);
  }

  public boolean getM_bActivateOnPageOpen() {
    return HJMVOJNI.HOutputHandlerOptions_m_bActivateOnPageOpen_get(swigCPtr, this);
  }

  public void setM_fAnnotLeft(float value) {
    HJMVOJNI.HOutputHandlerOptions_m_fAnnotLeft_set(swigCPtr, this, value);
  }

  public float getM_fAnnotLeft() {
    return HJMVOJNI.HOutputHandlerOptions_m_fAnnotLeft_get(swigCPtr, this);
  }

  public void setM_fAnnotRight(float value) {
    HJMVOJNI.HOutputHandlerOptions_m_fAnnotRight_set(swigCPtr, this, value);
  }

  public float getM_fAnnotRight() {
    return HJMVOJNI.HOutputHandlerOptions_m_fAnnotRight_get(swigCPtr, this);
  }

  public void setM_fAnnotBottom(float value) {
    HJMVOJNI.HOutputHandlerOptions_m_fAnnotBottom_set(swigCPtr, this, value);
  }

  public float getM_fAnnotBottom() {
    return HJMVOJNI.HOutputHandlerOptions_m_fAnnotBottom_get(swigCPtr, this);
  }

  public void setM_fAnnotTop(float value) {
    HJMVOJNI.HOutputHandlerOptions_m_fAnnotTop_set(swigCPtr, this, value);
  }

  public float getM_fAnnotTop() {
    return HJMVOJNI.HOutputHandlerOptions_m_fAnnotTop_get(swigCPtr, this);
  }

  public void setM_ePrcBrepCompression(HPrcBrepCompression value) {
    HJMVOJNI.HOutputHandlerOptions_m_ePrcBrepCompression_set(swigCPtr, this, value.swigValue());
  }

  public HPrcBrepCompression getM_ePrcBrepCompression() {
    return HPrcBrepCompression.swigToEnum(HJMVOJNI.HOutputHandlerOptions_m_ePrcBrepCompression_get(swigCPtr, this));
  }

  public void setM_bPrcRemoveBRep(boolean value) {
    HJMVOJNI.HOutputHandlerOptions_m_bPrcRemoveBRep_set(swigCPtr, this, value);
  }

  public boolean getM_bPrcRemoveBRep() {
    return HJMVOJNI.HOutputHandlerOptions_m_bPrcRemoveBRep_get(swigCPtr, this);
  }

  public void setM_bPrcRemoveAttributes(boolean value) {
    HJMVOJNI.HOutputHandlerOptions_m_bPrcRemoveAttributes_set(swigCPtr, this, value);
  }

  public boolean getM_bPrcRemoveAttributes() {
    return HJMVOJNI.HOutputHandlerOptions_m_bPrcRemoveAttributes_get(swigCPtr, this);
  }

  public void setM_bPrcUseNameUserData(boolean value) {
    HJMVOJNI.HOutputHandlerOptions_m_bPrcUseNameUserData_set(swigCPtr, this, value);
  }

  public boolean getM_bPrcUseNameUserData() {
    return HJMVOJNI.HOutputHandlerOptions_m_bPrcUseNameUserData_get(swigCPtr, this);
  }

  public void setM_iPrcNameUserDataIndex(SWIGTYPE_p_MVO_POINTER_SIZED_INT value) {
    HJMVOJNI.HOutputHandlerOptions_m_iPrcNameUserDataIndex_set(swigCPtr, this, SWIGTYPE_p_MVO_POINTER_SIZED_INT.getCPtr(value));
  }

  public SWIGTYPE_p_MVO_POINTER_SIZED_INT getM_iPrcNameUserDataIndex() {
    return new SWIGTYPE_p_MVO_POINTER_SIZED_INT(HJMVOJNI.HOutputHandlerOptions_m_iPrcNameUserDataIndex_get(swigCPtr, this), true);
  }

  public void setM_bPrcCompressTessellation(boolean value) {
    HJMVOJNI.HOutputHandlerOptions_m_bPrcCompressTessellation_set(swigCPtr, this, value);
  }

  public boolean getM_bPrcCompressTessellation() {
    return HJMVOJNI.HOutputHandlerOptions_m_bPrcCompressTessellation_get(swigCPtr, this);
  }

  public void setM_bAVIShowCompressionDialog(boolean value) {
    HJMVOJNI.HOutputHandlerOptions_m_bAVIShowCompressionDialog_set(swigCPtr, this, value);
  }

  public boolean getM_bAVIShowCompressionDialog() {
    return HJMVOJNI.HOutputHandlerOptions_m_bAVIShowCompressionDialog_get(swigCPtr, this);
  }

  public void setM_bAVICodec(String value) {
    HJMVOJNI.HOutputHandlerOptions_m_bAVICodec_set(swigCPtr, this, value);
  }

  public String getM_bAVICodec() {
    return HJMVOJNI.HOutputHandlerOptions_m_bAVICodec_get(swigCPtr, this);
  }

  public void setM_bExporting2DDwf(boolean value) {
    HJMVOJNI.HOutputHandlerOptions_m_bExporting2DDwf_set(swigCPtr, this, value);
  }

  public boolean getM_bExporting2DDwf() {
    return HJMVOJNI.HOutputHandlerOptions_m_bExporting2DDwf_get(swigCPtr, this);
  }

  public void get_m_PageMargins(float[] values, int count) {
    HJMVOJNI.HOutputHandlerOptions_get_m_PageMargins(swigCPtr, this, values, count);
  }

}
