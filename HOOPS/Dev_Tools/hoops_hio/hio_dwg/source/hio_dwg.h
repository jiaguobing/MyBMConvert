// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#pragma once

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"
#include "HIOManager.h"
#include "MrDwgReader.h"

#ifdef HIO_DWG
#define HIO_API __declspec (dllexport)
#else
#define HIO_API
#endif

/*! The hio_dwg is used for reading DWG files. This utility only supports 
	basic geometry nodes from DWG specification. This is not meant to be a 
	DWG import tool, but the main objective is to provide the developers with a sample code.
*/
class HIO_API hio_dwg : public HInputHandler
{
public:

	/*!
	 * This method initializes the RealDWG SDK. Call this once per application lifetime.
	 */
	static bool InitRealDWG();

	/*!
	 * This method shuts down the RealDWG SDK and releases the memory. Call this once per application lifetime.
	 */
	static void ShutDownRealDWG();


	/*!
	 * This method reads the DWG/DXF file 'filename' into the segment tree underneath key.
	 * \param filename The name of the file to load.
	 * \param key The HOOPS segment key to load model data into.
	 * \param options An options class containing any needed extra information for operation.
	 * \return The #HFileInputResult result of the input operation.
	 */
	HFileIOResult FileInputByKey(const wchar_t * FileName, HC_KEY key, HInputHandlerOptions * options);
	HFileIOResult FileInputByKey(const char * FileName, HC_KEY key, HInputHandlerOptions * options);
	HFileIOResult FileInputByKey(const unsigned short * FileName, HC_KEY key, HInputHandlerOptions * options);

	/*!
	 * This method reads the AcDbDatabase objects into the segment tree underneath the key.
	 * \param pAcDbDatabase A pointer to the AcDbDatabase which is required to be imported to HOOPS.
	 * \param key The HOOPS key to the segment for data to load in.
	 * \param options An options class containing any needed extra information for operation.
	 * \return The #HFileInputResult result of the input operation.
	 */
	HFileIOResult DatabaseInputByKey(AcDbDatabase* pAcDbDatabase, HC_KEY key, HInputHandlerOptions * options);


	hio_dwg();
	~hio_dwg();

	/*!This method registers the "dwg" and "dxf" extensions to this handler as an input handler.*/
	void RegisterInputHandlerTypes();

	/*! \return A character pointer denoting the name of the handler. */
	const char * GetInputName();

	/*!	\returns The input extension type that is handled by hio_dwg which are "dwg" and "dxf".*/
	const char * GetInputTypesString();

	/*! \return The type of input style which is #HInputHandlerStyleModel.*/
	HInputHandlerStyle GetInputStyle();

	static bool bDWGInitialized;
};


#ifdef H_PACK_8
#pragma pack(pop)
#endif
