// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef _HIOUTILITYIV_H
#define _HIOUTILITYIV_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#ifndef _WIN32_WCE
#include "HTools.h"
#include "HIOManager.h"

/*! The HIOUtilityIV is used for reading Open Inventor (.iv) files. 
*/
class MVO_API HIOUtilityIV: public HInputHandler
{

public:

	/*! Constructs an HIOUtilityIV object.*/
	HIOUtilityIV() {
		SetInputOps(HInputOpFileInputByKey);
	};
	virtual ~HIOUtilityIV() {;};

	/*!This method registers the "iv" extension to this handler.*/
	void RegisterInputHandlerTypes() {
		HIORegisterInputType("iv",this);
	};

	/*! \return A character pointer denoting the name of the handler. 
	*/
	const char * GetInputName() { return "HIOUtilityIV"; }

	/*! \return The string of the type of input this handler creates which is "iv".*/
	const char * GetInputTypesString() {return "iv";};

	/*! \return The type of input style which is HInputHandlerStyleModel.*/
	HInputHandlerStyle GetInputStyle() {return HInputHandlerStyleModel;};
	
	/*!
	This method reads the .iv file 'filename' into the segment tree underneath the given key.
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
};

#endif

#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif
