// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef _HIOUTILITYHSF_H
#define _HIOUTILITYHSF_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"
#include "HIOManager.h"

/*! The HIOUtilityHsf used for reading and writing HOOPS Stream Files (HSFs) in a non-streaming way.
*/

class MVO_API HIOUtilityHsf: public HInputHandler, public HOutputHandler
{

public:

	/*!Constructs an HIOUtilityHsf object.*/
	HIOUtilityHsf() {
		SetInputOps(HInputOpFileInputByKey);
		SetOutputOps(HOutputOpFileOutputByKey);
	};

	virtual ~HIOUtilityHsf() {;};

	/*! \return A character pointer denoting the name of the handler. 
	*/
	const char * GetOutputName() { return "HIOUtilityHsf"; }
	const char * GetInputName() { return "HIOUtilityHsf"; }

	/*!This method registers the "hsf" extensions to this handler as an input handler.*/
	void RegisterInputHandlerTypes();

	/*!	\returns The input extension type that is handled by HIOUtilityHsf.*/
	const char * GetInputTypesString();

	/*! \return The type of input style which is #HInputHandlerStyleModel.*/
	HInputHandlerStyle GetInputStyle() {return HInputHandlerStyleModel;};

	/*!
	  This method reads a HSF file into the segment defined by key.   To facilitate streaming most applications
	  implement their own HSF import on the application side.  This is here for completeness and currently creates
	  it's own stream toolkit object.
		\param filename The name of the file to load.
		\param key The HOOPS segment key to load model data into.
		\param options An options class containing any needed extra information for operation.
		\return The #HFileInputResult result of the input operation.
	*/
	HFileInputResult FileInputByKey(const __wchar_t * FileName, HC_KEY key, HInputHandlerOptions * options);

	HFileInputResult FileInputByKey(const char * FileName, HC_KEY key, HInputHandlerOptions * options){
		return FileInputByKey(H_WCS(FileName).encodedText(), key, options);
	};

	HFileInputResult FileInputByKey(const unsigned short * FileName, HC_KEY key, HInputHandlerOptions * options){
		H_UTF16 utf16;
		utf16.encodedText((utf16_char const*) FileName);
		return FileInputByKey(H_WCS(utf16).encodedText(), key, options);
	};

	/*!This method registers the "hsf" extensions to this handler as an output handler.*/
	void RegisterOutputHandlerTypes();

	/*! \return The string of the type of output this handler creates.*/
	const char * GetOutputTypesString();

	/*! \return The type of output style which is HOutputHandlerStyleModel.*/
	HOutputHandlerStyle GetOutputStyle() {return HOutputHandlerStyleModel;};

	/*!
	  This method creates a HSF file of the segment tree defined under key.  The StreamToolkit object
	  associated with the HBaseView object, which is passed in via the options structure is used to
	  create the HSF.
		\param filename The name of the file to output.
		\param key The HOOPS segment key to output model data from.
		\param options The options class containing a pointer the HStreamFileToolkit object in m_pExtendedData.
		\return The #HFileOutputResult result of the output operation.
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

private:
	HFileOutputResult FileOutputCommon(const __wchar_t * FileName, HC_KEY key,  HOutputHandlerOptions * options);


};


#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif
