// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef _HIOUTILITY_H
#define _HIOUTILITY_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#ifndef _WIN32_WCE
#include "HTools.h"
#include "HIOManager.h"

/*! VRML import support has been deprecated. This class is provided as-is and is limited 
    to loading faceted geometry, lines, spheres, text (including font), transforms, and basic color 
	information (diffuse, transparency, shininess).
*/
class MVO_API HIOUtilityVRML: public HInputHandler
{

public:

	/*!Constructs an HIOUtilityVRML object.*/
	HIOUtilityVRML() {
		SetInputOps(HInputOpFileInputByKey);
	};
	virtual ~HIOUtilityVRML() {;};

	/*! \return A character pointer denoting the name of the handler. 
	*/
	const char * GetInputName() { return "HIOUtilityVRML"; }

	/*!This method registers the "wrl" extension to this handler as an input handler.*/
	void RegisterInputHandlerTypes() {
		HIORegisterInputType("wrl",this);
	};

	/*!	\returns The input extension type that is handled by HIOUtilityVRML which is "wrl".*/
	const char * GetInputTypesString() {return "wrl";};

	/*! \return The type of input style which is #HInputHandlerStyleModel.*/
	HInputHandlerStyle GetInputStyle() {return HInputHandlerStyleModel;};

	/*!
	  This method reads the given VRML 2.0 file into the segment tree underneath key.
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
