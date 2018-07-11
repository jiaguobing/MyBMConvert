// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef _HIOUTILITYAVI_H
#define _HIOUTILITYAVI_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"
#include "HIOManager.h"

class HBaseView;


/*! \struct AVIExportInfo
A type definition for a structure containing parameters information for the AVI export
*/	
typedef	struct AVIExportInfo {

	int starttick;		//!< Starttick for animation.
	int endtick;		//!< Endtick for animation.
	int resolution;		//!< The number of frames per second.
} AVIExportInfo;	//!< AVIExportInformation  Typedef for AVIExportInfo


#ifdef HIO_AVI
#  ifdef _MSC_VER
#	 define HIO_API __declspec (dllexport)
#  else
#	 define HIO_API __attribute__ ((visibility ("default")))
#  endif
#else
#  define HIO_API
#endif

/*! The HIOUtilityAVI class allows you to write animation clips. It is currently only supported on 
windows platforms.
*/
class HIO_API HIOUtilityAVI: public HOutputHandler
{

public:
	/*!Constructs an HIOUtilityAVI object.*/
	HIOUtilityAVI() {
		SetOutputOps(HOutputOpFileOutputByKey);
	};
	virtual ~HIOUtilityAVI() {;};
	/*!This method registers the "avi" extension to this handler.*/
	void RegisterOutputHandlerTypes() {
		HIORegisterOutputType("avi",this);
	};
	/*! \return A character pointer denoting the name of the handler. 
	*/
	const char * GetOutputName() { return "HIOUtilityAVI"; }
	const char * GetInputName() { return "HIOUtilityAVI"; }

	/*! \return The string of the type of output this handler creates which is "avi".*/
	const char * GetOutputTypesString() {return "avi";};

	/*! \return The type of output style which is HOutputHandlerStyleModel.*/
	HOutputHandlerStyle GetOutputStyle() {return HOutputHandlerStyleModel;};

	/*!
	This is used to write an AVI file. 

	\param FileName A character pointer denoting the name of the AVI file to write
	\param key This parameter is not used.
	\param options The file output options. 
	\return The #HFileOutputResult result of the operation.
	*/
	HFileOutputResult FileOutputByKey(const __wchar_t * filename, HC_KEY key, HOutputHandlerOptions * options);

	HFileOutputResult FileOutputByKey(const unsigned short * filename, HC_KEY key, HOutputHandlerOptions * options){
		H_UTF16 utf16;
		utf16.encodedText((utf16_char const*) filename);
		return FileOutputByKey(H_WCS(utf16).encodedText(), key, options);
	}

	HFileOutputResult FileOutputByKey(const char * filename, HC_KEY key, HOutputHandlerOptions * options){
		return FileOutputByKey(H_WCS(filename).encodedText(), key, options);
	}

};

#ifdef HIO_AVI
extern "C" {
HIO_API void * CreateOutput(HIOManager *manager);
HIO_API void * CreateConnector(void * unused);
HIO_API void Free(HIOUtilityAVI *);	
HIO_API void FreeConnector(HIOConnector *connector);
}
#endif

#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif
