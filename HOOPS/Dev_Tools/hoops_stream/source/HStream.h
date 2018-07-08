// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

//! Contains definitions of global variables, functions and enumerated types.
/*! \file HStream.h
    The HOOPS/Stream Toolkit utilizes several global enumerated types.  The functions which black box support for writing
    and reading HOOPS Stream Files are also global, and are defined in HStream.h
*/

#ifndef HBINFILETKHEADER
#define HBINFILETKHEADER

#ifndef __cplusplus
#error C++ compiler required
#endif

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#ifdef _MSC_VER
    #ifndef HSTREAM_STATIC_LIB
        #ifdef HBINFILETK_EXPORTS
            #define HBINFILETK_API2  __declspec (dllexport)
        #else
            #define HBINFILETK_API2  __declspec (dllimport)
        #endif
    #else
        #ifndef BSTREAM_STATIC_LIB
            #define BSTREAM_STATIC_LIB
        #endif
        #define HBINFILETK_API2
    #endif
#else
    #define HBINFILETK_API2
#endif

#ifdef LINUX_SYSTEM
    #ifndef HSTREAM_STATIC_LIB
        #ifdef HBINFILETK_EXPORTS
            #define HBINFILETK_API2  __attribute__ ((visibility ("default")))
        #else
            #define HBINFILETK_API2
        #endif
    #else
        #ifndef BSTREAM_STATIC_LIB
            #define BSTREAM_STATIC_LIB
        #endif
        #define HBINFILETK_API2
    #endif
#endif

#ifdef TEST_RIG_1
    #define HBINFILETK_API
#else
    #define HBINFILETK_API HBINFILETK_API2
#endif


#ifdef HSTREAM_READONLY
    #ifndef BSTREAM_READONLY
        #define BSTREAM_READONLY
    #endif
#endif


#endif /* DOXYGEN_SHOULD_SKIP_THIS */

#include "BStream.h"
#include "hc.h"

#include "HOpcodeHandler.h"
#include "HStreamFileToolkit.h"

//#include "utf_utils.h"

/*!
  \addtogroup HTK_Read_Stream_File
  @{
*/
//! Reads a HOOPS Stream File and map it to the HOOPS/3dGS scene-graph
/*! 
    The contents of the HOOPS Stream File are mapped to the corresponding HOOPS/3dGS scene-graph hierarchy 
    underneath the currently open segment.
    \param filename A character pointer denoting the name of the file to read in.
    \param tk A pointer to the HStreamFileToolkit to use as a basis for reading the file.  This would be passed
        in to read in a customized file that contains user-specified data.  Options to control how the file is
        read may be specified using the BStreamFileToolkit::SetReadFlags() method on the toolkit.
*/
#ifndef SWIG
TK_Status HBINFILETK_API HTK_Read_Stream_File  (char const * filename, HStreamFileToolkit * tk);
#endif

//! Reads a HOOPS Stream File and map it to the HOOPS/3dGS scene-graph
/*! 
    The contents of the HOOPS Stream File are mapped to the corresponding HOOPS/3dGS scene-graph hierarchy 
    underneath the currently open segment.
    \param filename A character pointer denoting the name of the file to read in.
    \param flags Bitwise encoding of options which control how the file is read. #TK_File_Read_Options provides
	             a list of supported options.
*/
#ifndef SWIG
TK_Status HBINFILETK_API HTK_Read_Stream_File  (char const * filename, int flags = 0);
#endif


//! Reads a HOOPS Stream File and map it to the HOOPS/3dGS scene-graph
/*! 
    The contents of the HOOPS Stream File are mapped to the corresponding HOOPS/3dGS scene-graph hierarchy 
    underneath the currently open segment.
    \param filename A pointer to a UNICODE wide character string denoting the name of the file to read in.
    \param tk A pointer to the HStreamFileToolkit object to use as a basis for generating the file.  This would be
*/
TK_Status HBINFILETK_API HTK_Read_Stream_File  (__wchar_t const * filename, HStreamFileToolkit * tk);
#ifdef _MSC_VER
TK_Status HBINFILETK_API HTK_Read_Stream_File  (unsigned short const * filename, HStreamFileToolkit * tk);
#endif

//! Reads a HOOPS Stream File and map it to the HOOPS/3dGS scene-graph
/*! 
    The contents of the HOOPS Stream File are mapped to the corresponding HOOPS/3dGS scene-graph hierarchy 
    underneath the currently open segment.
    \param filename A pointer to a UNICODE wide character string denoting the name of the file to read in.
    \param flags Bitwise encoding of options which control how the file is read. #TK_File_Read_Options provides
	             a list of supported options.
*/
TK_Status HBINFILETK_API HTK_Read_Stream_File  (__wchar_t const * filename, int flags = 0);
#ifdef _MSC_VER
TK_Status HBINFILETK_API HTK_Read_Stream_File  (unsigned short const * filename, int flags = 0);
#endif


//! \b Deprecated; reads a HOOPS Stream File and map it to the HOOPS/3dGS scene-graph
/*! 
    The contents of the HOOPS Stream File are mapped to the corresponding HOOPS/3dGS scene-graph hierarchy 
    underneath the currently open segment.
    This form of the function is provided for backward compatibility.
*/
#ifndef SWIG
TK_Status HBINFILETK_API HTK_Read_Stream_File  (char const * filename, int flags, HStreamFileToolkit * tk);
#endif

/*!  @}  */


/*!
  \addtogroup HTK_Write_Stream_File
  @{
*/

//! Exports the HOOPS/3dGS scene-graph to a HOOPS Stream File
/*! 
    The contents of the HOOPS/3dGS scene-graph, beginning with the currently open segment, are written out to a
    HOOPS Stream File
    \param filename A character pointer denoting the name of the file to write.
    \param tk A pointer to the HStreamFileToolkit object to use as a basis for generating the file.  This would be
        passed in to write out a customized file that contains user-specified data.  Options to control how the file is
        written may be specified using the BStreamFileToolkit::SetWriteFlags() method on the toolkit.
*/
#ifndef SWIG
TK_Status HBINFILETK_API HTK_Write_Stream_File (char const * filename, HStreamFileToolkit * tk);
#endif



//! Exports the HOOPS/3dGS scene-graph to a HOOPS Stream File
/*! 
    The contents of the HOOPS/3dGS scene-graph, beginning with the currently open segment, are written out to a
    HOOPS Stream File
    \param filename A character pointer denoting the name of the file to write.
    \param flags Bitwise encoding of options to control how the file is written.  #TK_File_Write_Options provides 
        a list of supported options.
*/
#ifndef SWIG
TK_Status HBINFILETK_API HTK_Write_Stream_File (char const * filename, int flags = 0);
#endif



//! Exports the HOOPS/3dGS scene-graph to a HOOPS Stream File
/*! 
    The contents of the HOOPS/3dGS scene-graph, beginning with the currently open segment, are written out to a
    HOOPS Stream File
    \param filename A pointer to a UNICODE wide character string denoting the name of the file to write.
    \param tk A pointer to the HStreamFileToolkit object to use as a basis for generating the file.  This would be
        passed in to write out a customized file that contains user-specified data.  Options to control how the file is
        written may be specified using the BStreamFileToolkit::SetWriteFlags() method on the toolkit.
*/
TK_Status HBINFILETK_API2 HTK_Write_Stream_File (__wchar_t const * filename, HStreamFileToolkit * tk);
#ifdef _MSC_VER
TK_Status HBINFILETK_API2 HTK_Write_Stream_File (unsigned short const * filename, HStreamFileToolkit * tk);
#endif


//! Exports the HOOPS/3dGS scene-graph to a HOOPS Stream File
/*! 
    The contents of the HOOPS/3dGS scene-graph, beginning with the currently open segment, are written out to a
    HOOPS Stream File
    \param filename A pointer to a UNICODE wide character string denoting the name of the file to write.
    \param flags Bitwise encoding of options to control how the file is written.  #TK_File_Write_Options provides 
        a list of supported options.
*/
TK_Status HBINFILETK_API HTK_Write_Stream_File (__wchar_t const * filename, int flags = 0);
#ifdef _MSC_VER
TK_Status HBINFILETK_API HTK_Write_Stream_File (unsigned short const * filename, int flags = 0);
#endif



//! \b Deprecated; exports the HOOPS/3dGS scene-graph to a HOOPS Stream File
/*! 
    The contents of the HOOPS/3dGS scene-graph, beginning with the currently open segment, are written out to a
    HOOPS Stream File.
    This form of the function is provided for backward compatibility.
*/
#ifndef SWIG
TK_Status HBINFILETK_API2 HTK_Write_Stream_File (char const * filename, int flags, HStreamFileToolkit * tk);
#endif
/*!  @}  */
#endif

