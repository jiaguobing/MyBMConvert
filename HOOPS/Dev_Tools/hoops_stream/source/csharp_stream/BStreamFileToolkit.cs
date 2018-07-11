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


public class BStreamFileToolkit : BControlledMemoryObject {
  private HandleRef swigCPtr;

  public BStreamFileToolkit(IntPtr cPtr, bool cMemoryOwn) : base(HCSSTREAMPINVOKE.BStreamFileToolkitUpcast(cPtr), cMemoryOwn) {
    swigCPtr = new HandleRef(this, cPtr);
  }

  public static HandleRef getCPtr(BStreamFileToolkit obj) {
    return (obj == null) ? new HandleRef(null, IntPtr.Zero) : obj.swigCPtr;
  }

  ~BStreamFileToolkit() {
    Dispose();
  }

  public override void Dispose() {
    lock(this) {
      if (swigCPtr.Handle != IntPtr.Zero) {
        if (swigCMemOwn) {
          swigCMemOwn = false;
          HCSSTREAMPINVOKE.delete_BStreamFileToolkit(swigCPtr);
        }
        swigCPtr = new HandleRef(null, IntPtr.Zero);
      }
      GC.SuppressFinalize(this);
      base.Dispose();
    }
  }

  public BStreamFileToolkit() : this(HCSSTREAMPINVOKE.new_BStreamFileToolkit(), true) {
  }

  public static bool SupportsAsciiMode() {
    bool ret = HCSSTREAMPINVOKE.BStreamFileToolkit_SupportsAsciiMode();
    return ret;
  }

  public TK_Status SetAsciiMode(bool whether) {
    TK_Status ret = (TK_Status)HCSSTREAMPINVOKE.BStreamFileToolkit_SetAsciiMode(swigCPtr, whether);
    return ret;
  }

  public bool GetAsciiMode() {
    bool ret = HCSSTREAMPINVOKE.BStreamFileToolkit_GetAsciiMode(swigCPtr);
    return ret;
  }

  public void SetTabs(int arg0) {
    HCSSTREAMPINVOKE.BStreamFileToolkit_SetTabs(swigCPtr, arg0);
  }

  public int GetTabs() {
    int ret = HCSSTREAMPINVOKE.BStreamFileToolkit_GetTabs(swigCPtr);
    return ret;
  }

  public static int ParseVersion(string block) {
    int ret = HCSSTREAMPINVOKE.BStreamFileToolkit_ParseVersion(block);
    return ret;
  }

  public TK_Status PrepareBuffer(byte[] b, int s) {
    TK_Status ret = (TK_Status)HCSSTREAMPINVOKE.BStreamFileToolkit_PrepareBuffer(swigCPtr, b, s);
    return ret;
  }

  public int CurrentBufferLength() {
    int ret = HCSSTREAMPINVOKE.BStreamFileToolkit_CurrentBufferLength(swigCPtr);
    return ret;
  }

  public virtual void ActivateContext(ID_Key key) {
    HCSSTREAMPINVOKE.BStreamFileToolkit_ActivateContext(swigCPtr, key);
  }

  public virtual void DeactivateContext(ID_Key key) {
    HCSSTREAMPINVOKE.BStreamFileToolkit_DeactivateContext(swigCPtr, key);
  }

  public virtual void NewFileContext(ID_Key key) {
    HCSSTREAMPINVOKE.BStreamFileToolkit_NewFileContext(swigCPtr, key);
  }

  public int GeneratedSoFar() {
    int ret = HCSSTREAMPINVOKE.BStreamFileToolkit_GeneratedSoFar(swigCPtr);
    return ret;
  }

  public uint ObjectsSoFar() {
    uint ret = HCSSTREAMPINVOKE.BStreamFileToolkit_ObjectsSoFar(swigCPtr);
    return ret;
  }

  public void SetOpcodeHandler(int which, BBaseOpcodeHandler handler) {
    HCSSTREAMPINVOKE.BStreamFileToolkit_SetOpcodeHandler(swigCPtr, which, BBaseOpcodeHandler.getCPtr(handler));
  }

  public void SetPrewalkHandler(BBaseOpcodeHandler handler) {
    HCSSTREAMPINVOKE.BStreamFileToolkit_SetPrewalkHandler(swigCPtr, BBaseOpcodeHandler.getCPtr(handler));
  }

  public void SetPostwalkHandler(BBaseOpcodeHandler handler) {
    HCSSTREAMPINVOKE.BStreamFileToolkit_SetPostwalkHandler(swigCPtr, BBaseOpcodeHandler.getCPtr(handler));
  }

  public BBaseOpcodeHandler GetOpcodeHandler(int which) {
    IntPtr cPtr = HCSSTREAMPINVOKE.BStreamFileToolkit_GetOpcodeHandler(swigCPtr, which);
    BBaseOpcodeHandler ret = (cPtr == IntPtr.Zero) ? null : new BBaseOpcodeHandler(cPtr, false);
    return ret;
  }

  public virtual void Restart() {
    HCSSTREAMPINVOKE.BStreamFileToolkit_Restart(swigCPtr);
  }

  public TK_Status IndexToKey(int index, ID_Key[] key) {
    TK_Status ret = (TK_Status)HCSSTREAMPINVOKE.BStreamFileToolkit_IndexToKey(swigCPtr, index, key);
    return ret;
  }

  public TK_Status KeyToIndex(ID_Key key, int[] index) {
    TK_Status ret = (TK_Status)HCSSTREAMPINVOKE.BStreamFileToolkit_KeyToIndex(swigCPtr, key, index);
    return ret;
  }

  public TK_Status AddIndexKeyPair(int index, ID_Key key) {
    TK_Status ret = (TK_Status)HCSSTREAMPINVOKE.BStreamFileToolkit_AddIndexKeyPair(swigCPtr, index, key);
    return ret;
  }

  public TK_Status AddVariant(ID_Key key, int variant, int value1, int value2) {
    TK_Status ret = (TK_Status)HCSSTREAMPINVOKE.BStreamFileToolkit_AddVariant__SWIG_0(swigCPtr, key, variant, value1, value2);
    return ret;
  }

  public TK_Status AddVariant(ID_Key key, int variant, int value1) {
    TK_Status ret = (TK_Status)HCSSTREAMPINVOKE.BStreamFileToolkit_AddVariant__SWIG_1(swigCPtr, key, variant, value1);
    return ret;
  }

  public TK_Status AddBounds(ID_Key key, float[] bounds) {
    TK_Status ret = (TK_Status)HCSSTREAMPINVOKE.BStreamFileToolkit_AddBounds(swigCPtr, key, bounds);
    return ret;
  }

  public TK_Status GetOffset(ID_Key key, int variant, int[] offset) {
    TK_Status ret = (TK_Status)HCSSTREAMPINVOKE.BStreamFileToolkit_GetOffset__SWIG_0(swigCPtr, key, variant, offset);
    return ret;
  }

  public TK_Status GetOffset(ID_Key key, int variant, int[] offset, int[] length) {
    TK_Status ret = (TK_Status)HCSSTREAMPINVOKE.BStreamFileToolkit_GetOffset__SWIG_1(swigCPtr, key, variant, offset, length);
    return ret;
  }

  public TK_Status GetOffset(ID_Key key, int variant, int[] offset, int[] length, SWIGTYPE_p_wchar_t filename) {
    TK_Status ret = (TK_Status)HCSSTREAMPINVOKE.BStreamFileToolkit_GetOffset__SWIG_2(swigCPtr, key, variant, offset, length, SWIGTYPE_p_wchar_t.getCPtr(filename));
    return ret;
  }

  public TK_Status GetBounds(ID_Key key, float[] bounds) {
    TK_Status ret = (TK_Status)HCSSTREAMPINVOKE.BStreamFileToolkit_GetBounds(swigCPtr, key, bounds);
    return ret;
  }

  public int NextTagIndex() {
    int ret = HCSSTREAMPINVOKE.BStreamFileToolkit_NextTagIndex(swigCPtr);
    return ret;
  }

  public int PeekTagIndex() {
    int ret = HCSSTREAMPINVOKE.BStreamFileToolkit_PeekTagIndex(swigCPtr);
    return ret;
  }

  public void SetFilename(string name) {
    HCSSTREAMPINVOKE.BStreamFileToolkit_SetFilename(swigCPtr, name);
  }

  public TK_Status Read_Stream_File() {
    TK_Status ret = (TK_Status)HCSSTREAMPINVOKE.BStreamFileToolkit_Read_Stream_File(swigCPtr);
    return ret;
  }

  public void SetNewFile(string name) {
    HCSSTREAMPINVOKE.BStreamFileToolkit_SetNewFile(swigCPtr, name);
  }

  public string GetCurrentFile() {return HCSSTREAMPINVOKE.BStreamFileToolkit_GetCurrentFile__SWIG_0(swigCPtr);}

  public void GetCurrentFile(SWIGTYPE_p_wchar_t filename) {
    HCSSTREAMPINVOKE.BStreamFileToolkit_GetCurrentFile__SWIG_1(swigCPtr, SWIGTYPE_p_wchar_t.getCPtr(filename));
  }

  public TK_Status SelectFile(string name) {
    TK_Status ret = (TK_Status)HCSSTREAMPINVOKE.BStreamFileToolkit_SelectFile(swigCPtr, name);
    return ret;
  }

  public virtual TK_Status OpenFile(string name, bool write) {
    TK_Status ret = (TK_Status)HCSSTREAMPINVOKE.BStreamFileToolkit_OpenFile__SWIG_0(swigCPtr, name, write);
    return ret;
  }

  public virtual TK_Status OpenFile(string name) {
    TK_Status ret = (TK_Status)HCSSTREAMPINVOKE.BStreamFileToolkit_OpenFile__SWIG_1(swigCPtr, name);
    return ret;
  }

  public virtual TK_Status CloseFile() {
    TK_Status ret = (TK_Status)HCSSTREAMPINVOKE.BStreamFileToolkit_CloseFile(swigCPtr);
    return ret;
  }

  public virtual TK_Status ReadBuffer(byte[] buffer, int size, int[] amount_read) {
    TK_Status ret = (TK_Status)HCSSTREAMPINVOKE.BStreamFileToolkit_ReadBuffer(swigCPtr, buffer, size, amount_read);
    return ret;
  }

  public virtual TK_Status WriteBuffer(byte[] buffer, int size) {
    TK_Status ret = (TK_Status)HCSSTREAMPINVOKE.BStreamFileToolkit_WriteBuffer(swigCPtr, buffer, size);
    return ret;
  }

  public virtual TK_Status PositionFile(int offset) {
    TK_Status ret = (TK_Status)HCSSTREAMPINVOKE.BStreamFileToolkit_PositionFile(swigCPtr, offset);
    return ret;
  }

  public virtual TK_Status GetFileSize(uint[] size) {
    TK_Status ret = (TK_Status)HCSSTREAMPINVOKE.BStreamFileToolkit_GetFileSize(swigCPtr, size);
    return ret;
  }

  public virtual TK_Status LocateDictionary() {
    TK_Status ret = (TK_Status)HCSSTREAMPINVOKE.BStreamFileToolkit_LocateDictionary(swigCPtr);
    return ret;
  }

  public virtual TK_Status LocateEntity(ID_Key key, int variant) {
    TK_Status ret = (TK_Status)HCSSTREAMPINVOKE.BStreamFileToolkit_LocateEntity(swigCPtr, key, variant);
    return ret;
  }

  public int GetFlags() {
    int ret = HCSSTREAMPINVOKE.BStreamFileToolkit_GetFlags(swigCPtr);
    return ret;
  }

  public void SetFlags(int flags) {
    HCSSTREAMPINVOKE.BStreamFileToolkit_SetFlags(swigCPtr, flags);
  }

  public void SetWriteFlags(int flags) {
    HCSSTREAMPINVOKE.BStreamFileToolkit_SetWriteFlags(swigCPtr, flags);
  }

  public int GetWriteFlags(int mask) {
    int ret = HCSSTREAMPINVOKE.BStreamFileToolkit_GetWriteFlags__SWIG_0(swigCPtr, mask);
    return ret;
  }

  public int GetWriteFlags() {
    int ret = HCSSTREAMPINVOKE.BStreamFileToolkit_GetWriteFlags__SWIG_1(swigCPtr);
    return ret;
  }

  public void SetReadFlags(int flags) {
    HCSSTREAMPINVOKE.BStreamFileToolkit_SetReadFlags(swigCPtr, flags);
  }

  public int GetReadFlags(int mask) {
    int ret = HCSSTREAMPINVOKE.BStreamFileToolkit_GetReadFlags__SWIG_0(swigCPtr, mask);
    return ret;
  }

  public int GetReadFlags() {
    int ret = HCSSTREAMPINVOKE.BStreamFileToolkit_GetReadFlags__SWIG_1(swigCPtr);
    return ret;
  }

  public int GetNumNormalBits() {
    int ret = HCSSTREAMPINVOKE.BStreamFileToolkit_GetNumNormalBits(swigCPtr);
    return ret;
  }

  public void SetNumNormalBits(int numbits) {
    HCSSTREAMPINVOKE.BStreamFileToolkit_SetNumNormalBits(swigCPtr, numbits);
  }

  public int GetNumVertexBits() {
    int ret = HCSSTREAMPINVOKE.BStreamFileToolkit_GetNumVertexBits(swigCPtr);
    return ret;
  }

  public void SetNumVertexBits(int numbits) {
    HCSSTREAMPINVOKE.BStreamFileToolkit_SetNumVertexBits(swigCPtr, numbits);
  }

  public int GetNumParameterBits() {
    int ret = HCSSTREAMPINVOKE.BStreamFileToolkit_GetNumParameterBits(swigCPtr);
    return ret;
  }

  public void SetNumParameterBits(int numbits) {
    HCSSTREAMPINVOKE.BStreamFileToolkit_SetNumParameterBits(swigCPtr, numbits);
  }

  public int GetNumColorBits() {
    int ret = HCSSTREAMPINVOKE.BStreamFileToolkit_GetNumColorBits(swigCPtr);
    return ret;
  }

  public void SetNumColorBits(int numbits) {
    HCSSTREAMPINVOKE.BStreamFileToolkit_SetNumColorBits(swigCPtr, numbits);
  }

  public int GetNumIndexBits() {
    int ret = HCSSTREAMPINVOKE.BStreamFileToolkit_GetNumIndexBits(swigCPtr);
    return ret;
  }

  public void SetNumIndexBits(int numbits) {
    HCSSTREAMPINVOKE.BStreamFileToolkit_SetNumIndexBits(swigCPtr, numbits);
  }

  public void SetJpegQuality(int quality) {
    HCSSTREAMPINVOKE.BStreamFileToolkit_SetJpegQuality__SWIG_0(swigCPtr, quality);
  }

  public void SetJpegQuality() {
    HCSSTREAMPINVOKE.BStreamFileToolkit_SetJpegQuality__SWIG_1(swigCPtr);
  }

  public int GetJpegQuality() {
    int ret = HCSSTREAMPINVOKE.BStreamFileToolkit_GetJpegQuality(swigCPtr);
    return ret;
  }

  public int GetVersion() {
    int ret = HCSSTREAMPINVOKE.BStreamFileToolkit_GetVersion(swigCPtr);
    return ret;
  }

  public void SetReadVersion(int version) {
    HCSSTREAMPINVOKE.BStreamFileToolkit_SetReadVersion(swigCPtr, version);
  }

  public void SetTargetVersion(int version) {
    HCSSTREAMPINVOKE.BStreamFileToolkit_SetTargetVersion(swigCPtr, version);
  }

  public int GetTargetVersion() {
    int ret = HCSSTREAMPINVOKE.BStreamFileToolkit_GetTargetVersion(swigCPtr);
    return ret;
  }

  public uint GetFileOffset() {
    uint ret = HCSSTREAMPINVOKE.BStreamFileToolkit_GetFileOffset(swigCPtr);
    return ret;
  }

  public void SetFileOffset(uint offset) {
    HCSSTREAMPINVOKE.BStreamFileToolkit_SetFileOffset(swigCPtr, offset);
  }

  public int Unused() {
    int ret = HCSSTREAMPINVOKE.BStreamFileToolkit_Unused(swigCPtr);
    return ret;
  }

  public virtual TK_Status Error(string msg) {
    TK_Status ret = (TK_Status)HCSSTREAMPINVOKE.BStreamFileToolkit_Error__SWIG_0(swigCPtr, msg);
    return ret;
  }

  public virtual TK_Status Error() {
    TK_Status ret = (TK_Status)HCSSTREAMPINVOKE.BStreamFileToolkit_Error__SWIG_1(swigCPtr);
    return ret;
  }

  public string GetLogFile() {return HCSSTREAMPINVOKE.BStreamFileToolkit_GetLogFile(swigCPtr);}

  public void SetLogFile(string filename) {
    HCSSTREAMPINVOKE.BStreamFileToolkit_SetLogFile__SWIG_0(swigCPtr, filename);
  }

  public void SetLogFile() {
    HCSSTREAMPINVOKE.BStreamFileToolkit_SetLogFile__SWIG_1(swigCPtr);
  }

  public bool GetLogging() {
    bool ret = HCSSTREAMPINVOKE.BStreamFileToolkit_GetLogging(swigCPtr);
    return ret;
  }

  public void SetLogging(bool setting) {
    HCSSTREAMPINVOKE.BStreamFileToolkit_SetLogging(swigCPtr, setting);
  }

  public uint GetLoggingOptions(uint mask) {
    uint ret = HCSSTREAMPINVOKE.BStreamFileToolkit_GetLoggingOptions__SWIG_0(swigCPtr, mask);
    return ret;
  }

  public uint GetLoggingOptions() {
    uint ret = HCSSTREAMPINVOKE.BStreamFileToolkit_GetLoggingOptions__SWIG_1(swigCPtr);
    return ret;
  }

  public void SetLoggingOptions(uint options) {
    HCSSTREAMPINVOKE.BStreamFileToolkit_SetLoggingOptions__SWIG_0(swigCPtr, options);
  }

  public void SetLoggingOptions() {
    HCSSTREAMPINVOKE.BStreamFileToolkit_SetLoggingOptions__SWIG_1(swigCPtr);
  }

  public TK_Status OpenLogFile(string filename, string mode) {
    TK_Status ret = (TK_Status)HCSSTREAMPINVOKE.BStreamFileToolkit_OpenLogFile(swigCPtr, filename, mode);
    return ret;
  }

  public void LogEntry(string arg0) {
    HCSSTREAMPINVOKE.BStreamFileToolkit_LogEntry(swigCPtr, arg0);
  }

  public void CloseLogFile() {
    HCSSTREAMPINVOKE.BStreamFileToolkit_CloseLogFile(swigCPtr);
  }

  public uint NextOpcodeSequence() {
    uint ret = HCSSTREAMPINVOKE.BStreamFileToolkit_NextOpcodeSequence(swigCPtr);
    return ret;
  }

  public void SetOpcodeSequence(uint seq) {
    HCSSTREAMPINVOKE.BStreamFileToolkit_SetOpcodeSequence__SWIG_0(swigCPtr, seq);
  }

  public void SetOpcodeSequence() {
    HCSSTREAMPINVOKE.BStreamFileToolkit_SetOpcodeSequence__SWIG_1(swigCPtr);
  }

  public bool HeaderCommentSeen() {
    bool ret = HCSSTREAMPINVOKE.BStreamFileToolkit_HeaderCommentSeen(swigCPtr);
    return ret;
  }

  public SWIGTYPE_p_f_unsigned_long_unsigned_long_p_void__bool GetProgressCallback() {
    IntPtr cPtr = HCSSTREAMPINVOKE.BStreamFileToolkit_GetProgressCallback(swigCPtr);
    SWIGTYPE_p_f_unsigned_long_unsigned_long_p_void__bool ret = (cPtr == IntPtr.Zero) ? null : new SWIGTYPE_p_f_unsigned_long_unsigned_long_p_void__bool(cPtr, false);
    return ret;
  }

  public void SetProgressCallback(HCSSTREAM.TK_Progress_Callback cb) {
    HCSSTREAMPINVOKE.BStreamFileToolkit_SetProgressCallback__SWIG_0(swigCPtr, cb);
  }

  public void SetProgressCallback() {
    HCSSTREAMPINVOKE.BStreamFileToolkit_SetProgressCallback__SWIG_1(swigCPtr);
  }

  public IntPtr GetProgressValue() {return HCSSTREAMPINVOKE.BStreamFileToolkit_GetProgressValue(swigCPtr);}

  public void SetProgressValue(IntPtr value) {
    HCSSTREAMPINVOKE.BStreamFileToolkit_SetProgressValue(swigCPtr, value);
  }

  public int GetBufferLimit() {
    int ret = HCSSTREAMPINVOKE.BStreamFileToolkit_GetBufferLimit(swigCPtr);
    return ret;
  }

  public void SetBufferLimit(int limit) {
    HCSSTREAMPINVOKE.BStreamFileToolkit_SetBufferLimit(swigCPtr, limit);
  }

  public void SetLastKey(ID_Key key) {
    HCSSTREAMPINVOKE.BStreamFileToolkit_SetLastKey(swigCPtr, key);
  }

  public TK_Status AppendLastKey(ID_Key key) {
    TK_Status ret = (TK_Status)HCSSTREAMPINVOKE.BStreamFileToolkit_AppendLastKey(swigCPtr, key);
    return ret;
  }

  public void ClearLastKey() {
    HCSSTREAMPINVOKE.BStreamFileToolkit_ClearLastKey(swigCPtr);
  }

  public TK_Status GetLastKey(ID_Key[] key) {
    TK_Status ret = (TK_Status)HCSSTREAMPINVOKE.BStreamFileToolkit_GetLastKey(swigCPtr, key);
    return ret;
  }

  public void SetDictionaryFormat(int format, int options) {
    HCSSTREAMPINVOKE.BStreamFileToolkit_SetDictionaryFormat__SWIG_0(swigCPtr, format, options);
  }

  public void SetDictionaryFormat(int format) {
    HCSSTREAMPINVOKE.BStreamFileToolkit_SetDictionaryFormat__SWIG_1(swigCPtr, format);
  }

  public void SetDictionaryFormat() {
    HCSSTREAMPINVOKE.BStreamFileToolkit_SetDictionaryFormat__SWIG_2(swigCPtr);
  }

  public int GetDictionaryFormat() {
    int ret = HCSSTREAMPINVOKE.BStreamFileToolkit_GetDictionaryFormat(swigCPtr);
    return ret;
  }

  public int GetDictionaryOptions() {
    int ret = HCSSTREAMPINVOKE.BStreamFileToolkit_GetDictionaryOptions(swigCPtr);
    return ret;
  }

  public void SetDictionaryOffset(int offset) {
    HCSSTREAMPINVOKE.BStreamFileToolkit_SetDictionaryOffset(swigCPtr, offset);
  }

  public int GetDictionaryOffset() {
    int ret = HCSSTREAMPINVOKE.BStreamFileToolkit_GetDictionaryOffset(swigCPtr);
    return ret;
  }

  public void SetDictionarySize(int size) {
    HCSSTREAMPINVOKE.BStreamFileToolkit_SetDictionarySize(swigCPtr, size);
  }

  public int GetDictionarySize() {
    int ret = HCSSTREAMPINVOKE.BStreamFileToolkit_GetDictionarySize(swigCPtr);
    return ret;
  }

  public void RecordPause(int offset) {
    HCSSTREAMPINVOKE.BStreamFileToolkit_RecordPause(swigCPtr, offset);
  }

  public void ClearPauses() {
    HCSSTREAMPINVOKE.BStreamFileToolkit_ClearPauses(swigCPtr);
  }

  public int GetPauseCount() {
    int ret = HCSSTREAMPINVOKE.BStreamFileToolkit_GetPauseCount(swigCPtr);
    return ret;
  }

  public IntPtr GetPauseTable() {return HCSSTREAMPINVOKE.BStreamFileToolkit_GetPauseTable(swigCPtr);}

  public void SetFirstPause(int offset) {
    HCSSTREAMPINVOKE.BStreamFileToolkit_SetFirstPause(swigCPtr, offset);
  }

  public int GetFirstPause() {
    int ret = HCSSTREAMPINVOKE.BStreamFileToolkit_GetFirstPause(swigCPtr);
    return ret;
  }

  public int GetPosition() {
    int ret = HCSSTREAMPINVOKE.BStreamFileToolkit_GetPosition(swigCPtr);
    return ret;
  }

  public void SetWorldBounding(float[] bbox) {
    HCSSTREAMPINVOKE.BStreamFileToolkit_SetWorldBounding(swigCPtr, bbox);
  }

  public void SetWorldBoundingBySphere(float[] pt, float radius) {
    HCSSTREAMPINVOKE.BStreamFileToolkit_SetWorldBoundingBySphere(swigCPtr, pt, radius);
  }

  public IntPtr GetWorldBounding() {return HCSSTREAMPINVOKE.BStreamFileToolkit_GetWorldBounding(swigCPtr);}

  public bool AddExternalReference(string arg0, ID_Key context) {
    bool ret = HCSSTREAMPINVOKE.BStreamFileToolkit_AddExternalReference(swigCPtr, arg0, context);
    return ret;
  }

  public bool NextExternalReference() {
    bool ret = HCSSTREAMPINVOKE.BStreamFileToolkit_NextExternalReference(swigCPtr);
    return ret;
  }

  public string GetExternalReference() {return HCSSTREAMPINVOKE.BStreamFileToolkit_GetExternalReference__SWIG_0(swigCPtr);}

  public void GetExternalReference(SWIGTYPE_p_wchar_t exref) {
    HCSSTREAMPINVOKE.BStreamFileToolkit_GetExternalReference__SWIG_1(swigCPtr, SWIGTYPE_p_wchar_t.getCPtr(exref));
  }

  public ID_Key GetExternalReferenceContext() {return HCSSTREAMPINVOKE.BStreamFileToolkit_GetExternalReferenceContext(swigCPtr);}

  public virtual bool MatchPreviousExRef() {
    bool ret = HCSSTREAMPINVOKE.BStreamFileToolkit_MatchPreviousExRef(swigCPtr);
    return ret;
  }

  public void AddSegment(ID_Key key) {
    HCSSTREAMPINVOKE.BStreamFileToolkit_AddSegment(swigCPtr, key);
  }

  public ID_Key RemoveSegment() {return HCSSTREAMPINVOKE.BStreamFileToolkit_RemoveSegment(swigCPtr);}

  public ID_Key CurrentSegment() {return HCSSTREAMPINVOKE.BStreamFileToolkit_CurrentSegment(swigCPtr);}

  public void ResetQuantizationError() {
    HCSSTREAMPINVOKE.BStreamFileToolkit_ResetQuantizationError(swigCPtr);
  }

  public void ReportQuantizationError(float error) {
    HCSSTREAMPINVOKE.BStreamFileToolkit_ReportQuantizationError__SWIG_0(swigCPtr, error);
  }

  public void ReportQuantizationError(int bits_per_sample, float[] bounding, int num_dimensions) {
    HCSSTREAMPINVOKE.BStreamFileToolkit_ReportQuantizationError__SWIG_1(swigCPtr, bits_per_sample, bounding, num_dimensions);
  }

  public void ReportQuantizationError(int bits_per_sample, float[] bounding) {
    HCSSTREAMPINVOKE.BStreamFileToolkit_ReportQuantizationError__SWIG_2(swigCPtr, bits_per_sample, bounding);
  }

  public float GetQuantizationError() {
    float ret = HCSSTREAMPINVOKE.BStreamFileToolkit_GetQuantizationError(swigCPtr);
    return ret;
  }

  public TK_Status OpenGeometry() {
    TK_Status ret = (TK_Status)HCSSTREAMPINVOKE.BStreamFileToolkit_OpenGeometry(swigCPtr);
    return ret;
  }

  public TK_Status CloseGeometry() {
    TK_Status ret = (TK_Status)HCSSTREAMPINVOKE.BStreamFileToolkit_CloseGeometry(swigCPtr);
    return ret;
  }

  public bool GeometryIsOpen() {
    bool ret = HCSSTREAMPINVOKE.BStreamFileToolkit_GeometryIsOpen(swigCPtr);
    return ret;
  }

  public ID_Key RevisitKey() {return HCSSTREAMPINVOKE.BStreamFileToolkit_RevisitKey(swigCPtr);}

  public ID_Key RevisitOwner() {return HCSSTREAMPINVOKE.BStreamFileToolkit_RevisitOwner(swigCPtr);}

  public TK_Status ParseBuffer(byte[] b, int s, TK_Status mode) {
    TK_Status ret = (TK_Status)HCSSTREAMPINVOKE.BStreamFileToolkit_ParseBuffer__SWIG_0(swigCPtr, b, s, (int)mode);
    return ret;
  }

  public TK_Status ParseBuffer(byte[] b, int s) {
    TK_Status ret = (TK_Status)HCSSTREAMPINVOKE.BStreamFileToolkit_ParseBuffer__SWIG_1(swigCPtr, b, s);
    return ret;
  }

}
