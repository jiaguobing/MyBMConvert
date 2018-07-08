// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

/*! \file HIOUtilityGDIOutput.h
	Defines datatypes associated with generating HOOPS/3dGS msw-driver printer and clipboard output  */

#ifndef _HIO_UTILITY_GDIOUTPUT_H
#define _HIO_UTILITY_GDIOUTPUT_H

#include "HTools.h"
#include "HIOManager.h"
#include "HMFCGlobals.h"
#include "utf_utils.h"

// msw driver specific driver_option debug flags

/*!
    \def DEBUG_FORCE_FULL_COLOR
  Causes HOOPS/3dgs msw driver to output commands as if the device is full color
  Dithering will not occur - device will appear to have no palette and
  16777216 colors - this will look very bad on a VGA device and is
  mainly intended for metafiles for printing and sharing
*/
#define DEBUG_FORCE_FULL_COLOR				0x00000010

/*!
    \def DEBUG_NO_WINDOWS_HOOK
    Obselete.  
*/
#define DEBUG_NO_WINDOWS_HOOK				0x00000040 

/*!
    \def DEBUG_SFB_COPY_TO_CLIPBOARD
   Tells HOOPS/3dGS that sfb is active and we are copying to a wmf.  
   Needed so that we know to limit the sfb image size and stretchblit
*/
#define DEBUG_SFB_COPY_TO_CLIPBOARD			0x00004000

/*!
    \def DEBUG_NO_RASTERS_IN_PRINTING
    Obsolete
*/
#define DEBUG_NO_RASTERS_IN_PRINTING		0x00020000

/*!
    \def DEBUG_PRINTING_MODE
    Tells HOOPS/3dGS msw driver to enter printing mode  
*/
#define DEBUG_PRINTING_MODE					0x00040000

/*!
    \def DEBUG_PRINT_NOT_FRAMEBUFFER_MODE
    Tell HOOPS/3dGS msw driver to use a single stretch blt to copy a software 
	framebuffer to the printer device 
*/
#define DEBUG_PRINT_NOT_FRAMEBUFFER_MODE	0x00020000


/*!
    \def DEBUG_PRINT_NOT_RASTER_REDUCTION
    Obsolete
*/
#define DEBUG_PRINT_NOT_RASTER_REDUCTION  	0x00200000

/*!
    \def DEBUG_FORCE_SOFTWARE
    Tells HOOPS/3dGS opengl driver to force software z-buffer
*/
#define DEBUG_FORCE_SOFTWARE				0x01000000

/*!
    \def DEBUG_NO_PAINTERS_PANELLING
    Used to tell HOOPS/3dGS not to perform panelling when using painter's algorithm
	(This setting is made be calling HOOPS/3dGS HC_Set_Rendering_Options subroutine
*/
#define DEBUG_NO_PAINTERS_PANELLING	0x00000008

/*! \struct GDIExportInformation
A class for a structure containing information used during a HOOPS/3dGS msw driver
clipboard or printer instance  (/driver/msw/clipboard or /driver/msw/printer)
*/	
class GDIExportInformation : public HOutputHandlerOptions {
public:

	/* Initialize the references in this structure to refer to the matching field
	 * in the parent class.  This is for supporting legacy code prior to v15. */
	GDIExportInformation()
		: HOutputHandlerOptions(),
		bForceGrayscale(m_ForceGrayscale),
		bConserveMemory(m_bConserveMemory),
		bUseWindowColor(m_bUseWindowColor),
		Height(m_Window_Height),
		Width(m_Window_Width),
		image_dpi(m_Image_Dpi)
	{				
		clipBoard = false;
		rasterOutput = false;
		windowID = 0;
		deviceContext = attribDC = 0;
	}

	HDC deviceContext;	  //!< Windows Device Context
	HDC attribDC;		  //!< The attribute device context for the CDC object
	bool rasterOutput;	  //!< True if raster output is supported by device context
	bool clipBoard;		  //!< True if device context is a clipboard
	void* windowID;		  //!< ID of window

	bool & bForceGrayscale; //!< Legacy code!  Please do not use this.  See HOutputHandlerOptions.
	bool & bConserveMemory; //!< Legacy code!  Please do not use this.  See HOutputHandlerOptions.
	bool & bUseWindowColor; //!< Legacy code!  Please do not use this.  See HOutputHandlerOptions.
	float & Height;		  //!< Legacy code!  Please do not use this.  See HOutputHandlerOptions.
	float & Width;		  //!< Legacy code!  Please do not use this.  See HOutputHandlerOptions.
	int & image_dpi;		  //!< Legacy code!  Please do not use this.  See HOutputHandlerOptions.
};

typedef GDIExportInformation GDIExportInfo;

//! The HIOUtilityGDIOutput class should be used for exporting data to a Windows Printer or Clipboard Device Context (DC).
/*!
  Via the GDIExportInformation structure you supply the id to the device context, the window handle and a bool indicating
  whether or not this is export to a clipboard or printer DC.  The code queries the device and repositions subwindows
  to ensure we get WYSIWYG printing.  This code should be used as a reference for developers who want to get WYSIWYG 
  printing of their subwindows.  
  
  The printing code employs a multi-pass render approach where it first creates an image of all the data which would 
  be sent down as rasters when rendered to a clipboard or printer DC and then sends down the rest of the (vector) 
  data after first passing it through a hidden surface removal algorithm to ensure it is sorted correctly wrt to the 
  data in the image.  Using this appraoch means that the image and shaded geometry in the scene will be sent down in 
  one quik BitBlit while the remaining polylines, edges, text and markers will be sent down as vector primitives.  
  This results in high quality hardcopy and clipboard output.  

  The 'bConserveMemory' boolean of GDIExportInformation controls whether the raster portion of the hardcopy should
  be sent to the printer in small chunks (bands), to help reduce the memory footprint required during the
  hardcopy generation.   This option ONLY affects the rasters that are part of the 'background' raster image
  send to the printer during the multi-pass print mechanism.  

  The resolution of the 'raster' portion of the output is controlled via the 'image_dpi' member of GDIExportInformation. 
*/
#ifdef SWIG
class MVO_API HIOUtilityGDIOutput : public HOutputHandler
#else
class HMFC_API HIOUtilityGDIOutput : public HOutputHandler
#endif
{

public:

	HIOUtilityGDIOutput() {
		SetOutputOps(HOutputOpFileOutputByKey);
	};
	virtual ~HIOUtilityGDIOutput() {;}

	//! registers the 'emf' output type
	void RegisterOutputHandlerTypes() {
		HIORegisterOutputType("emf", this);
	}

	const char * GetOutputName() { return "HIOUtilityGDIOutput"; }

	/*!
		\return The file extensions which may be created by this handler.
	*/
	const char * GetOutputTypesString() {return "emf";}

	/*! \return A character pointer denoting the name of the handler. 
	*/

	/*!
		\return The styles of model exported by this handler.  	
	*/
	HOutputHandlerStyle GetOutputStyle() {return HOutputHandlerStyleModel;}

	/*!
	  This function should be used for all GDI Print and Copy to Clipboard work

	  \param FileName A character pointer denoting the name of the file to be read in
	  \param key This is typically unused 
	  \param options You pass in the HBaseView pointer, the camera setting, the device context and the hWnd
		within this structure
	  \return result of operation
	*/
	HFileOutputResult FileOutputByKey(const char * FileName, HC_KEY key, HOutputHandlerOptions * options){
		return FileOutputByKey(H_WCS(FileName).encodedText(), key, options);
	}
	HFileOutputResult FileOutputByKey(unsigned short const * FileName, HC_KEY key, HOutputHandlerOptions * options){
		H_UTF16 utf16;
		utf16.encodedText((utf16_char const*) FileName);
		return FileOutputByKey(H_WCS(utf16).encodedText(), key, options);
	}
	HFileOutputResult FileOutputByKey(const __wchar_t * FileName, HC_KEY key, HOutputHandlerOptions * options);

	/*!
	  This function is for Copy to Clipboard 

	  \param FileName A character pointer denoting the name of the file to be read in
	  \param key This is typically unused 
	  \param options You pass in the HBaseView pointer, the camera setting, the device context and the hWnd
		within this structure
	  \return result of operation
	*/
	HFileOutputResult DoClipBoard(const wchar_t * FileName, HC_KEY key, GDIExportInformation * options);

	/*!
	  This function is for printing

	  \param FileName A character pointer denoting the name of the file to be read in
	  \param key This is typically unused 
	  \param options You pass in the HBaseView pointer, the camera setting, the device context and the hWnd
		within this structure
	  \return result of operation
	*/
	HFileOutputResult DoPrinter(const wchar_t * FileName, HC_KEY key, GDIExportInformation * options);
	
	/*! overloaded; returns OutputNotHandled */
	HFileOutputResult FileOutputFromImageKey(const char * FileName, HC_KEY key, HOutputHandlerOptions * options){
		UNREFERENCED(FileName);
		UNREFERENCED(key);
		UNREFERENCED(options);
		return OutputNotHandled;
	}
	HFileOutputResult FileOutputFromImageKey(unsigned short const * FileName, HC_KEY key, HOutputHandlerOptions * options){
		UNREFERENCED(FileName);
		UNREFERENCED(key);
		UNREFERENCED(options);
		return OutputNotHandled;
	}
	HFileOutputResult FileOutputFromImageKey(const __wchar_t * FileName, HC_KEY key, HOutputHandlerOptions * options){
		UNREFERENCED(FileName);
		UNREFERENCED(key);
		UNREFERENCED(options);
		return OutputNotHandled;
	}

private:

};



#endif



