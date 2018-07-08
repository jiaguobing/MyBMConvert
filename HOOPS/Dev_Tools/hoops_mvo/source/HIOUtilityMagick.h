// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef _HIOUTILITYMAGICK_H
#define _HIOUTILITYMAGICK_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"
#include "HIOManager.h"

/*! HIOUtilityMagick is the integration class for the ImageMagick toolkit.
    This integration is supported on all HOOPS platforms.
	
	The windows distribution of HOOPS/3dF redistributes the ImageMagick development libraries
    for WIN32. These are located in Dev_Tools/3rdParty/ImageMagick.


	For information on ImageMagick see: 
    http://www.imagemagick.org/
*/

class MVO_API HIOUtilityMagick: public HInputHandler, public HOutputHandler
{

public:
	~HIOUtilityMagick();

	/*! \return A character pointer denoting the name of the handler. 
	*/
	const char * GetOutputName();
	const char * GetInputName();

	/*!This method registers the image types that are supported by ImageMagick for reading.*/
	void RegisterInputHandlerTypes(); 

	/*! \return The type of input style which is #HInputHandlerStyleImage.*/
	HInputHandlerStyle GetInputStyle();

	/*!	\returns The input extension types that are handled by HIOUtilityMagick which include "jpeg", "gif", "bmp" and "tif".*/
	const char * GetInputTypesString();

	/*!
	  This method maps the data in the image file to a HOOPS image defined by image_key.
		\param FileName A character pointer denoting the name of the image file to read from.
		\param image_key The pointer to a HOOPS key that will be filled in with a HOOPS image key.
		\param segment_key The HOOPS segment key to create the image_key in i.e. the working segment.
		\param options The file input options. 
		\return The #HFileOutputResult result of the operation.
	*/
	HFileInputResult FileInputToImageKey(const __wchar_t * filename, HC_KEY * image_key, HC_KEY segment_key, HInputHandlerOptions * options);

	HFileInputResult FileInputToImageKey(const unsigned short * filename, HC_KEY * image_key, HC_KEY segment_key, HInputHandlerOptions * options);

	HFileInputResult FileInputToImageKey(const char * filename, HC_KEY * image_key, HC_KEY segment_key, HInputHandlerOptions * options);

	/*!This method registers the image types supported by the Raster Pro toolkit for writing.*/
	void RegisterOutputHandlerTypes();

	/*! \return The type of output style which is #HOutputHandlerStyleImage.*/
	HOutputHandlerStyle GetOutputStyle();

	/*!	\returns The output extension types that are handled by HIOUtilityMagick which include "jpeg", "gif", "bmp" and "tif".*/
	const char * GetOutputTypesString();


	/*!
	  This method exports the raster data contained in <i>image_key</i> to a file.  
	  It uses the file extension to determine what type of raster image to create.
		\param filename The name of the file to output.
		\param image_key The HOOPS image key to get output image data from.
		\param options The options class containing any needed extra information for the operation.
		\return The #HFileOutputResult result of the output operation.
	*/
	HFileOutputResult FileOutputFromImageKey(const __wchar_t * filename, HC_KEY image_key, HOutputHandlerOptions * options);

	HFileOutputResult FileOutputFromImageKey(const unsigned short * filename, HC_KEY image_key, HOutputHandlerOptions * options);

	HFileOutputResult FileOutputFromImageKey(const char * filename, HC_KEY image_key, HOutputHandlerOptions * options);
	
	/*!Constructs an HIOUtilityMagick object.*/
	HIOUtilityMagick();

};



#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif
