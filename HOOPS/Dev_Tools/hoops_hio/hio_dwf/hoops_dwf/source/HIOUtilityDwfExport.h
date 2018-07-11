// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef _HW3D_UTILITY_DWFEXPORT_H__RAJESH_B__20060503_1200__
#define _HW3D_UTILITY_DWFEXPORT_H__RAJESH_B__20060503_1200__

#include "HTools.h"
#include "HIOManager.h"
#include "HW3DStream.h"

#if defined(_MSC_VER) && defined(HIO_DWF_EXPORT)
#define HIO_API __declspec (dllexport)
#else
#define HIO_API
#endif

/*! The HIOUtilityDwfExport is the integration class for the DWF toolkit.  Using 
  	this integration class allows you to export Autodesk's DWF (Design Web Format).
	This integration is based on DWF Toolkit version 7+.
	To use this integration class you will need to obtain a license of DWF Toolkit.
*/
class HIO_API HIOUtilityDwfExport : public HOutputHandler
{

public:
	HIOUtilityDwfExport() 
	{ 
		SetOutputOps(HOutputOpFileOutputByKey);
	};

	~HIOUtilityDwfExport() {;};

	void RegisterOutputHandlerTypes() {
		HIORegisterOutputType("DWF",this);  // DWF (Autodesk's Design Web Format)
	    HIORegisterOutputType("dwf",this);  // DWF (Autodesk's Design Web Format)
	};

	HOutputHandlerStyle GetOutputStyle() {return HOutputHandlerStyleModel;};
	const char * GetOutputTypesString()  {return "dwf";};
	
	/*! \return A character pointer denoting the name of the handler. 
	*/
	const char * GetOutputName() { return "HIOUtilityDwfExport"; }

	/*!
	  This method creates a DWF file of the segment tree defined under key.
	  \param FileName A character pointer denoting the name of the file to be written
	  \param key  A hoops key of the segment - the tree underneath which is required to be exported
	  \param options  Export options
	  \return OutputOK if the export succeeds
	*/
	HFileOutputResult FileOutputByKey(const __wchar_t * FileName, HC_KEY key, HOutputHandlerOptions * options);

	HFileOutputResult FileOutputByKey(const unsigned short * FileName, HC_KEY key, HOutputHandlerOptions * options){
		H_UTF16 utf16;
		utf16.encodedText((utf16_char const*) FileName);
		return FileOutputByKey(H_WCS(utf16).encodedText(), key, options);
	}
};



/*! The HIOUtilityWhip2D is the class for writing DWF Whip 2D files using the HOOPS Whip 2D driver.
*/

class HIO_API HIOUtilityWhip2D : public HOutputHandler
{
public:
	HIOUtilityWhip2D() { 
		SetOutputOps(HOutputOpFileOutputByKey);
	};

	~HIOUtilityWhip2D() {;};

	void RegisterOutputHandlerTypes() {
		HIORegisterOutputType("w2d",this);
	};

	/*! \return A character pointer denoting the name of the handler. 
	*/
	const char * GetOutputName() { return "HIOUtilityWhip2D"; }


	const char * GetOutputTypesString() {return "w2d";};
	HOutputHandlerStyle GetOutputStyle() {return HOutputHandlerStyleHardcopy;};

	const char * GetOutputDefaultHSRA() {return "painters";};

	/*!
	This method writes a 2D DWF (Whip) file out of the segment tree underneath key
	\param FileName A character pointer denoting the name of the file to be written
	\param key  A hoops key of the segment - the tree underneath which is required to be exported
	\param options  Export options
	\return OutputOK if the export succeeds
	*/
	HFileOutputResult FileOutputByKey(const __wchar_t * filename, HC_KEY key, HOutputHandlerOptions * options);

	HFileOutputResult FileOutputByKey(const unsigned short * filename, HC_KEY key, HOutputHandlerOptions * options){
		H_UTF16 utf16;
		utf16.encodedText((utf16_char const*) filename);
		return FileOutputByKey(H_WCS(utf16).encodedText(), key, options);
	}

};

#ifdef HIO_DWF_EXPORT
extern "C" {
	HIO_API void * CreateInput(HIOManager *manager);
	HIO_API void * CreateOutput(HIOManager *manager);
	HIO_API void Free(HIOUtilityDwfExport *);
}
#endif


#endif	// _HW3D_UTILITY_DWFEXPORT_H__RAJESH_B__20060503_1200__




