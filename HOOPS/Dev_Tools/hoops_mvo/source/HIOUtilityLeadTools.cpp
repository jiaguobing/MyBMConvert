// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "HIOUtilityLeadTools.h"
#include "HDB.h"
#include "HUtility.h"

#include <windows.h>

// Lead Tools is an image format component that can optionally be (and is by default) 
// compiled in to allow image output in a wide variety of formats.  To compile without 
// lead tools in the MS Visual C++ environment, there is a separate project configuration, 
// "Win32 Release" (as opposed to the default, "Win32 Release LEAD_TOOLS")
//
// If the following include file is missing, it means one of two things, either:
//   a) lead tools has not been installed.  It must be purchased separately (see http://www.leadtools.com/)
//   b) the environment variable, $(LEAD_TOOLS_INSTALL_DIR) has not been set.  Please set it to the base
//      directory of your lead tools installation.  If you are using MS Visual C++, you will need to restart
//      the integrated development environment.
#include "l_bitmap.h"

class HLeadToolsInternal {

public:

	/*
		FileToImage reads a raster image file (supported for import by Lead Tools Imaging toolkit)into a raw HOOPS image
		\param filename The name of the input image file
		\param image_key The key of the HOOPS image as it will be referenced internally.
		\param segment_key The key of the segment that the image should be inserted into.
		\param imagename The key to the HOOPS image.  Passed by reference.
		\param format A description of how the image data is packed.  See HC_Insert_Image.
		\return A boolean value indicating success or failure.
	*/
	static bool FileToImage(const char *filename, HC_KEY *image_key, HC_KEY segment_key, const char *imagename, const char *format, bool local);

	/*
		ImageToFile writes a Image File (supported by Lead Tools for export) from a raw HOOPS image
		\param image_key The key to the HOOPS image.
		\param filename The name of the output image file.
		\return A boolean value indicating success or failure.
	*/
	static bool ImageToFile( HC_KEY image_key, const char *filename);

private:

	static void Show_Error (const char *message);

};


HFileInputResult HIOUtilityLeadTools::FileInputToImageKey(const char * filename, HC_KEY * image_key, HC_KEY segment_key, HInputHandlerOptions * options)
{
	if(!filename) return InputBadFileName;
	if(!image_key || !segment_key) return InputFail;

	const char * image_format = 0;
	const char * image_name = 0;

	if(options && options->m_pImageFormat) 
		image_format = options->m_pImageFormat;
	else
		image_format = "rgb";

	if(options) 
		image_name = options->m_pImageName;

	bool local = false;

	if (options && options->m_bLocal)
		local=true;

	bool result = HLeadToolsInternal::FileToImage(filename, image_key, segment_key, image_name, image_format, local);

	if(result)
		return InputOK;
	else
		return InputFail;
}



void HLeadToolsInternal::Show_Error (const char *message) {
	//TODO some thing else here
    //fprintf (stderr, "\n\007%s\n", message);
}




bool HLeadToolsInternal::FileToImage(const char *filename, HC_KEY *image_key, HC_KEY segment_key, 
									 const char *imagename, const char *format, bool local)
{

	char file_ext[MVO_BUFFER_SIZE]; 
	HUtility::FindFileNameExtension(filename, file_ext);

	if(strlen(file_ext) == 0) 
	{
		Show_Error ("Error: No file extension. Could not determine input file format.\n");
		return false;
	}

	BITMAPHANDLE lead_bmp;
	L_UINT	l_bits_per_pix = 24;		/* number of bits per pixel */
	L_UINT	l_order = ORDER_RGB;		/* color order for 16-, 24-, 32, 48, and 64-bit bitmaps */
	pLOADFILEOPTION l_load_opts = 0;	/* pointer to optional extended load options */

	L_INT32 l_err = L_LoadBitmap(const_cast<char*>(filename), &lead_bmp, l_bits_per_pix, l_order, l_load_opts, 0);
	if(l_err != SUCCESS)
	{
		Show_Error ("Error: LEAD TOOLS API L_LoadBitmap failed.\n");
		return false;
	}

	char image_spec[MVO_BUFFER_SIZE];
	if (imagename) 
		sprintf(image_spec, "rgb, name = %s", imagename);
	else 
		sprintf(image_spec, "rgb");

	// get the image data from the lead bitmap
	int n_row_begin				= 0;
	int n_row_end				= lead_bmp.Height - 1;
	L_UINT32 bytes_per_line		= lead_bmp.BytesPerLine;			/* number of bytes in the row to retrieve */
	L_UCHAR L_FAR* image_buf	= new unsigned char[lead_bmp.Size];	/* pointer to the target buffer */

	L_AccessBitmap (&lead_bmp);
	for(int i = n_row_begin; i <= n_row_end; i++)
	{
	  l_err = L_GetBitmapRow (&lead_bmp, image_buf + i * 3*lead_bmp.Width, 
									  lead_bmp.ViewPerspective == BOTTOM_LEFT ? lead_bmp.Height - i - 1 : i, 
									  bytes_per_line);
	  if (l_err != (L_INT32) bytes_per_line)
	  {
		Show_Error ("Error: LEAD TOOLS API L_GetBitmapRow failed.\n");
		 return false;
	  }
	}
	L_ReleaseBitmap (&lead_bmp);


	if (local)
		strcat(image_spec, ", local");

	HC_Open_Segment_By_Key(segment_key);
		*image_key = HC_KInsert_Image (0.0,0.0,0.0, image_spec, 
									lead_bmp.Width, lead_bmp.Height, image_buf);
	HC_Close_Segment();

	L_FreeBitmap(&lead_bmp);

	if( image_buf )
	{
		delete[] image_buf;
		image_buf = 0;
	}

	return true;
}


HFileOutputResult HIOUtilityLeadTools::FileOutputFromImageKey(const char * filename, HC_KEY image_key, HOutputHandlerOptions * options)
{
	if (!filename) return OutputBadFileName;
	if (!image_key) return OutputFail;

	bool result = HLeadToolsInternal::ImageToFile(image_key, filename);

	if(result) 
		return OutputOK;
	else
		return OutputFail;
}



bool HLeadToolsInternal::ImageToFile( HC_KEY image_key, const char *filename )
{
	char file_ext[MVO_BUFFER_SIZE]; 
	HUtility::FindFileNameExtension(filename, file_ext);

	if(strlen(file_ext) == 0) 
	{
		Show_Error ("Error: No file extension. Could not determine file format for export.\n");
		return false;
	}

	L_INT file_format;					/* output file format */
	
	/* quality factor: valid between 2-255. 0 is lossless and quite a number of readers don't support 
	   Also, not 20 is not very arbitrary. It seems MS PhotoEditor, MSIE don't like very high quality
	   jpeg files - Rajesh B */
	L_INT quality_factor = 20;			

	if ( streq(file_ext, "jpg") || streq(file_ext, "jpeg") )
		file_format = FILE_LEAD1JFIF;
	else if( streq(file_ext, "bmp") )
		file_format = FILE_BMP;
	else if( streq(file_ext, "gif") )
		file_format = FILE_GIF;
	else if( streq(file_ext, "tif") || streq(file_ext, "tiff"))
		file_format = FILE_TIF;
	else if( streq(file_ext, "wmf") )
		file_format = FILE_WMF;
	else if( streq(file_ext, "emf") )
		file_format = FILE_EMF;
	else if( streq(file_ext, "ico") )
		file_format = FILE_WIN_ICO;
	else if( streq(file_ext, "cur") )
		file_format = FILE_WIN_CUR;
	else if( streq(file_ext, "ani") )
		file_format = FILE_ANI;
	else if( streq(file_ext, "clp") )
		file_format = FILE_CLP;
	else if( streq(file_ext, "fpx") )
		file_format = FILE_FPX;
	else if( streq(file_ext, "exf") )
		file_format = FILE_EXIF;
	else if( streq(file_ext, "psd") )
		file_format = FILE_PSD;
	else if( streq(file_ext, "pcd") )
		file_format = FILE_PCD;
	else if( streq(file_ext, "cal") )
		file_format = FILE_CALS;
	else if( streq(file_ext, "pcx") )
		file_format = FILE_PCX;
	else if( streq(file_ext, "png") )
		file_format = FILE_PNG;
	else if( streq(file_ext, "tga") )
		file_format = FILE_TGA;
	else if( streq(file_ext, "ras") )
		file_format = FILE_RAS;
	else if( streq(file_ext, "pct") )
		file_format = FILE_PCT;
	else if( streq(file_ext, "awd") )
		file_format = FILE_AWD;
	else if( streq(file_ext, "wpg") )
		file_format = FILE_WPG;
	else if( streq(file_ext, "msp") )
		file_format = FILE_MSP;
	else if( streq(file_ext, "mac") )
		file_format = FILE_MAC;
	else if( streq(file_ext, "cut") )
		file_format = FILE_CUT;
	else 
	{
		Show_Error ("Error: Unsupported file format for export.\n");
		return false;
	}

	BITMAPHANDLE lead_bmp;

	// get the Hoops image details first
	char	h_format[MVO_BUFFER_SIZE];			/* description of Hoops image format */
	unsigned char *h_data;			/* Hoops image data */
	int		h_width, h_height;		/* Hoops image height and width */
	float	x,y,z;					/* Hoops image center */

	HC_Show_Image_Size(image_key, &x, &y, &z, h_format, &h_width, &h_height);


	char * comma;

	if ((comma =strchr(h_format, ',')) != 0)
		*comma = '\0';

	// if not the format we like, return
	if( !streq(h_format, "rgb" ) )
	{
		Show_Error ("Error: Export for non-rgb formats not implemented\n");
		return false;
	}

	// lead tools has a stupid restriction of wanting the size of each line to be a multiple of 4 bytes.
	// so I am trimming down the width to multiple of four
	h_width = h_width & ~3;
	if( h_width < 1 )
	{
		Show_Error ("Error: Image width too small for export\n");
		return false;
	}

	unsigned int h_size = (unsigned int)h_width *(unsigned int)h_height * 3 *sizeof (unsigned char);


	h_data = new unsigned char[h_size];
	HC_Show_Partial_Image(image_key, 0, 0, h_width, h_height, h_data);


	// Create a blank Lead Tools bitmap
	L_UINT 	l_memory		= TYPE_USER;	/* flag to request the type of memory to allocate */
	L_UINT 	l_width			= h_width;		/* width of the bitmap in pixels */
	L_UINT 	l_height		= h_height;		/* height of the bitmap in pixels */
	L_UINT	l_bits_per_pix	= 24;			/* number of bits per pixel */
	L_UINT	l_order			= ORDER_RGB;	/* color order for 16-, 24-, 32, 48, and 64-bit bitmaps */
	LPRGBQUAD l_palette		= 0;			/* pointer to the palette that the bitmap will use */
	L_UINT	l_view_persp	= TOP_LEFT;		/* where the beginning of the image is stored */
	L_CHAR L_FAR *l_data	= 0;			/* data pointer */
	L_UINT32 l_size			= 0;			/* size of the data buffer pointed to by pData */

	L_UINT l_err = L_CreateBitmap(&lead_bmp, l_memory, l_width, l_height, l_bits_per_pix, l_order, 
									l_palette, l_view_persp, h_data, h_size);
	if(l_err != SUCCESS)
	{
		Show_Error ("Error: LEAD TOOLS API L_CreateBitmap failed.\n");
		return false;
	}

	l_err  = L_SaveBitmap(const_cast<char*>(filename), &lead_bmp, file_format, 0, quality_factor, 0);
	if(l_err != SUCCESS)
	{
		Show_Error ("Error: LEAD TOOLS API L_SaveBitmap failed.\n");
		return false;
	}

	// cleanup
	L_FreeBitmap(&lead_bmp);

	if( h_data )
	{
		delete[] h_data;
		h_data = 0;
	}

	return true;
}





