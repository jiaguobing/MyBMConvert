// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef _HIO_UTILITY_LEADTOOLS_H
#define _HIO_UTILITY_LEADTOOLS_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"
#include "HIOManager.h"

/*! The HIOUtilityLeadTools is the integration class for the LeadTools Raster Pro toolkit.  Using 
  	this integration class allows you to import and export the over 60 raster image types supported
	by the Raster Pro toolkit.  To use this integration class you will need to obtain a license of 
	the Raster Pro Toolkit.
*/


class HIOUtilityLeadTools: public HInputHandler, public HOutputHandler
{

public:
	/*!Constructs an HIOUtilityLeadTools object.*/
	HIOUtilityLeadTools() { 
		SetInputOps(HInputOpFileInputToImageKey);
		SetOutputOps(HOutputOpFileOutputFromImageKey);
	};

	~HIOUtilityLeadTools() {;};

	/*!This method registers the image types supported by the Raster Pro toolkit for reading.*/
	void RegisterInputHandlerTypes() {
	// Here is a list of some of the file formats and some of the flavors LEAD supports for Import
	// add if you think your license of LEAD covers more formats
	HIORegisterInputType("jpeg",this);  // JPEG
	HIORegisterInputType("jpg",this);   // JPEG
	HIORegisterInputType("bmp",this);   // Windows BMP, OS/2 BMP
	HIORegisterInputType("gif",this);   // GIF (Embedded Text, Animated, Interlaced, Transparency), 
	HIORegisterInputType("tif",this);   // TIFF 6.0 (MPT, JPEG, Packbits, Huffman, RLE, LZW, CMYK, CCITT, CCITT G31D, CCITT G32D, CCITT G4, Bitonal, Grayscale, Palletized, RGB, YCbCr, CIELAB)
	HIORegisterInputType("tiff",this);  // TIFF 6.0 (MPT, JPEG, Packbits, Huffman, RLE, LZW, CMYK, CCITT, CCITT G31D, CCITT G32D, CCITT G4, Bitonal, Grayscale, Palletized, RGB, YCbCr, CIELAB)
	HIORegisterInputType("wmf",this);   // Windows Metafile (WMF)
	HIORegisterInputType("emf",this);   // Enhanced Metafile (WMF)
	HIORegisterInputType("ico",this);   // Windows Icon (ICO)
	HIORegisterInputType("cur",this);   // Windows Cursor (CUR)
	HIORegisterInputType("ani",this);   // Windows Animated Cursor (ANI)
	HIORegisterInputType("clp",this);   // Windows Clipboard Cursor (CLP)
	HIORegisterInputType("avi",this);   // Windows AVI(AVI)
	HIORegisterInputType("fpx",this);   // FlashPix (FPX)
	HIORegisterInputType("exf",this);   // Fuji Exif 2.0 (EXF)
	HIORegisterInputType("psd",this);   // Adobe PhotoShop (PSD)
	HIORegisterInputType("pcd",this);   // Kodak PhotoCD PCD, EPS, IOCA, MODCA
	HIORegisterInputType("cal",this);   // CALS raster (CAL)
	HIORegisterInputType("pcx",this);   // PC Paintbrush (PCX)
	HIORegisterInputType("wfx",this);   // Winfax (WFX), MAC, VDA
	HIORegisterInputType("png",this);   // CompuServe PNG
	HIORegisterInputType("tga",this);   // Targa (TGA)
	HIORegisterInputType("ras",this);   // Sun Raster (RAS)
	HIORegisterInputType("pct",this);   // Macintosh PICT (PCT), LEAD CMP
	HIORegisterInputType("awd",this);   // Microsoft FAX (AWD)
	HIORegisterInputType("wpg",this);   // WordPerfect (WPG)
	HIORegisterInputType("msp",this);   // Microsoft Paint (MSP)
	HIORegisterInputType("mac",this);   // Machintosh Paint (MAC)
	HIORegisterInputType("cut",this);   // Dr. Halo
	//HIORegisterInputType("DICOM",this);  // DICOM - What's the extension
	//HIORegisterInputType("pdf",this);   // Adobe Portable Document Format(PDF) - required PDF plugin
	};

	/*! \return The type of input style which is #HInputHandlerStyleImage.*/
	HInputHandlerStyle GetInputStyle() {return HInputHandlerStyleImage;};

	/*!	\returns The input extension types that are handled by HIOUtilityLeadTools which include "jpeg", "gif", "bmp" and "tif".*/
	const char * GetInputTypesString() 
	{return "jpeg;jpg;bmp;gif;tif;tiff;wmf;emf;ico;cur;ani;clp;avi;fpx;exf;psd;pcd;cal;pcx;wfx;png;tga;ras;pct;awd;wpg;msp;mac;cut";};


	/*!
	  This method reads the data in the image file to a HOOPS image defined by image_key.
		\param FileName A character pointer denoting the name of the image file to read from.
		\param image_key The pointer to a HOOPS key that will be filled in with a HOOPS image key.
		\param segment_key The HOOPS segment key to create the image_key in i.e. the working segment.
		\param options The file input options. 
		\return The #HFileOutputResult result of the operation.
	*/
	HFileInputResult FileInputToImageKey(const char * filename, HC_KEY  *image_key, HC_KEY segment_key, HInputHandlerOptions * options);

	/*!This method registers the image types supported by the Raster Pro toolkit for writing.*/
	void RegisterOutputHandlerTypes() {
	    // Here is a list of some of the file formats and some of the flavors LEAD supports for Export
	    // add if you think your license of LEAD covers more formats
	    HIORegisterOutputType("jpeg",this);  // JPEG
	    HIORegisterOutputType("jpg",this);   // JPEG
	    HIORegisterOutputType("bmp",this);   // Windows BMP, OS/2 BMP
	    HIORegisterOutputType("gif",this);   // GIF (Embedded Text, Animated, Interlaced, Transparency), 
	    HIORegisterOutputType("tif",this);   // TIFF 6.0 (MPT, JPEG, Packbits, Huffman, RLE, LZW, CMYK, CCITT, CCITT G31D, CCITT G32D, CCITT G4, Bitonal, Grayscale, Palletized, RGB, YCbCr, CIELAB)
	    HIORegisterOutputType("tiff",this);  // TIFF 6.0 (MPT, JPEG, Packbits, Huffman, RLE, LZW, CMYK, CCITT, CCITT G31D, CCITT G32D, CCITT G4, Bitonal, Grayscale, Palletized, RGB, YCbCr, CIELAB)
	    HIORegisterOutputType("wmf",this);   // Windows Metafile (WMF)
	    HIORegisterOutputType("emf",this);   // Enhanced Metafile (WMF)
	    HIORegisterOutputType("ico",this);   // Windows Icon (ICO)
	    HIORegisterOutputType("cur",this);   // Windows Cursor (CUR)
	    HIORegisterOutputType("ani",this);   // Windows Animated Cursor (ANI)
	    HIORegisterOutputType("clp",this);   // Windows Clipboard Cursor (CLP)
	    HIORegisterOutputType("avi",this);   // Windows AVI(AVI)
	    HIORegisterOutputType("fpx",this);   // FlashPix (FPX)
	    HIORegisterOutputType("exf",this);   // Fuji Exif 2.0 (EXF)
	    HIORegisterOutputType("psd",this);   // Adobe PhotoShop (PSD)
	    HIORegisterOutputType("pcd",this);   // Kodak PhotoCD PCD, EPS, IOCA, MODCA
	    HIORegisterOutputType("cal",this);   // CALS raster (CAL)
	    HIORegisterOutputType("pcx",this);   // PC Paintbrush (PCX)
	    HIORegisterOutputType("wfx",this);   // Winfax (WFX), MAC, VDA
	    HIORegisterOutputType("png",this);   // CompuServe PNG
	    HIORegisterOutputType("tga",this);   // Targa (TGA)
	    HIORegisterOutputType("ras",this);   // Sun Raster (RAS)
	    HIORegisterOutputType("pct",this);   // Macintosh PICT (PCT), LEAD CMP
	    HIORegisterOutputType("awd",this);   // Microsoft FAX (AWD)
	    HIORegisterOutputType("wpg",this);   // WordPerfect (WPG)
	    HIORegisterOutputType("msp",this);   // Microsoft Paint (MSP)
	    HIORegisterOutputType("mac",this);   // Machintosh Paint (MAC)
	    HIORegisterOutputType("cut",this);   // Dr. Halo
	    //HIORegisterOutputType("DICOM",this);  // DICOM - What's the extension
	    //HIORegisterOutputType("pdf",this);   // Adobe Portable Document Format(PDF) -  - required PDF plugin
	};

	/*! \return The type of output style which is #HOutputHandlerStyleImage.*/
	HOutputHandlerStyle GetOutputStyle() {return HOutputHandlerStyleImage;};

	/*!	\returns The output extension types that are handled by HIOUtilityLeadTools which include "jpeg", "gif", "bmp" and "tif".*/
	const char * GetOutputTypesString() 
	{return "jpeg;jpg;bmp;gif;tif;tiff;wmf;emf;ico;cur;ani;clp;avi;fpx;exf;psd;pcd;cal;pcx;wfx;png;tga;ras;pct;awd;wpg;msp;mac;cut";};


	/*!
	  This method exports the raster data contained in image_key as a file.  
	  It uses the file extension to determine what type of raster image to create.
		\param filename The name of the file to output.
		\param image_key The HOOPS image key to get output image data from.
		\param options The options class containing any needed extra information for the operation.
		\return The #HFileOutputResult result of the output operation.
	*/
	HFileOutputResult FileOutputFromImageKey(const char * filename, HC_KEY image_key, HOutputHandlerOptions * options);

};



#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif







