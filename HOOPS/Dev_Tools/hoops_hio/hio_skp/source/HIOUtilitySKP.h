// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#pragma once

#ifdef HIO_SKP
#  ifdef _MSC_VER
#	 define HIO_API __declspec (dllexport)
#  else
#	 define HIO_API __attribute__ ((visibility ("default")))
#  endif
#else
#  define HIO_API
#endif

#include "HIOManager.h"

class HIO_API HIOUtilitySKP : public HInputHandler
{
public:
	HIOUtilitySKP();
	virtual ~HIOUtilitySKP() { }

	/*! \return A character pointer denoting the name of the handler. */
	virtual const char* GetInputName() { return "HIOUtilitySKP"; }

	/*!This method registers the "skp" extension to this handler as an input handler. */
	virtual void RegisterInputHandlerTypes()
	{
		HIORegisterInputType("SKP", this);  // SKP (Google SketchUp's Format)
		HIORegisterInputType("skp", this);  // skp (Google SketchUp's Format)
	};

	/*! \return The type of input style which is #HInputHandlerStyleModel. */
	virtual HInputHandlerStyle GetInputStyle() { return HInputHandlerStyleModel; }

	/*!	\returns The input extension type that is handled by HIOUtilitySKP which is "skp". */
	const char* GetInputTypesString() { return "skp"; }

	/*! This method reads the SKP file into the segment tree underneath given key.
		\param filename The name of the file to load.
		\param key The HOOPS segment key to load model data into.
		\param options An options class containing any needed extra information for operation.
		\return The #HFileInputResult result of the input operation.
	*/
	virtual HFileInputResult FileInputByKey(const char* fileName, HC_KEY key, HInputHandlerOptions* options);

	virtual HFileInputResult FileInputByKey(const wchar_t* fileName, HC_KEY key, HInputHandlerOptions* options);

	virtual HFileInputResult FileInputByKey(const unsigned short* fileName, HC_KEY key, HInputHandlerOptions* options);
};

#ifdef HIO_SKP
extern "C" {
	HIO_API void* CreateInput(HIOManager* manager);
	HIO_API void  Free(HIOUtilitySKP*);
}
#endif
