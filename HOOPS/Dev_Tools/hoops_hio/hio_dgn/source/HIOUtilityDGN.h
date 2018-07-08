// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef _HIO_UTILITY_DGN_2__02052008_1230_
#define _HIO_UTILITY_DGN_2__02052008_1230_

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"
#include "HIOManager.h"


#ifdef HIO_DGN
#  ifdef _MSC_VER
#	 define HIO_API __declspec (dllexport)
#  else
#	 define HIO_API __attribute__ ((visibility ("default")))
#  endif
#else
#  define HIO_API
#endif

/*! The HIOUtilityDGN is the integration class for Open Design Alliances (ODA) DGNDirect Toolkit.  
	Using this integration class allows you to import Microstations's DGN file.
	This integration is based on DGN Toolkit version 2.xx
	To use this integration class you will need to obtain a license of DGNDirect Toolkit from ODA.
*/

class HIO_API HIOUtilityDGN : public HInputHandler
{


public:

	/*!Constructs an HIOUtilityDGN object.*/
	HIOUtilityDGN() 
	{ 
		SetInputOps(HInputOpFileInputByKey);
	};

	~HIOUtilityDGN() {;};

	/*!This method registers the "dgn" extension to this handler as an input handler.*/
	void RegisterInputHandlerTypes() {
		HIORegisterInputType("DGN",this);  // DGN (Bentley's CAD format)
		HIORegisterInputType("dgn",this);  // DGN (Bentley's CAD format)
	};

	/*! \return A character pointer denoting the name of the handler. 
	*/
	const char * GetOutputName() { return "HIOUtilityDGN"; }
	const char * GetInputName() { return "HIOUtilityDGN"; }

	/*! \return The type of input style which is #HInputHandlerStyleModel.*/
	HInputHandlerStyle GetInputStyle() {return HInputHandlerStyleModel;};

	/*!	\returns The input extension type that is handled by HIOUtilityDGN which is "dgn".*/
	const char * GetInputTypesString() {return "dgn";};

	/*! This method reads the DGN file into the segment tree underneath given key.
		\param filename The name of the file to load.
		\param key The HOOPS segment key to load model data into.
		\param options An options class containing any needed extra information for operation.
		\return The #HFileInputResult result of the input operation.
	*/
	HFileInputResult FileInputByKey(const char * FileName, HC_KEY key, HInputHandlerOptions * options);

	HFileInputResult FileInputByKey(const wchar_t * FileName, HC_KEY key, HInputHandlerOptions * options);

	/*! This method reads the DGN file into the segment tree underneath given key.
		\param filename The UNICODE name of the file to load.
		\param key The HOOPS segment key to load model data into.
		\param options An options class containing any needed extra information for operation.
		\return The #HFileInputResult result of the input operation.
	*/
	HFileInputResult FileInputByKey(const unsigned short * FileName, HC_KEY key, HInputHandlerOptions * options);


	/*! This method Initializes Runtime Extension environment and DgnDirect toolkit*/
	void InitDGNDirect();

	/*!This method Uninitializes the DgnDirect toolkit.*/
	void  ShutdownDGNDirect();

};

#ifdef HIO_DGN
extern "C" {
	HIO_API void * CreateInput(HIOManager *manager);
	HIO_API void * CreateOutput(HIOManager *manager);
	HIO_API void * CreateConnector(void *unused);
	HIO_API void Free(HIOUtilityDGN *);
	HIO_API void FreeConnector(HIOConnector *connector);
}
#endif



#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif	//_HIO_UTILITY_DGN_2__02052008_1230_






