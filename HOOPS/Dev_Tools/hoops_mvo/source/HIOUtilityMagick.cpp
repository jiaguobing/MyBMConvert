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

#include "HIOUtilityMagick.h"
#include "HDB.h"
#include "HUtility.h"
#include "HUtilityLocaleString.h"

#ifdef USE_IMAGE_MAGICK

#include <magick/api.h>

#ifndef countof
# define countof(x) (sizeof(x)/sizeof(x[0]))
#endif

void HIOUtilityMagick::RegisterInputHandlerTypes() {
    HIORegisterInputType("jpeg",this);  
    HIORegisterInputType("jpg",this);  
    HIORegisterInputType("bmp",this);  
    HIORegisterInputType("gif",this);  
    HIORegisterInputType("tif",this); 
    HIORegisterInputType("tiff",this);
    HIORegisterInputType("png",this);   
    HIORegisterInputType("tga",this);   
}

HInputHandlerStyle HIOUtilityMagick::GetInputStyle() {return HInputHandlerStyleImage;};

const char * HIOUtilityMagick::GetInputTypesString() {return "jpeg;jpg;bmp;gif;tif;tiff;png;tga";};

void HIOUtilityMagick::RegisterOutputHandlerTypes() {
    HIORegisterOutputType("jpeg",this);
    HIORegisterOutputType("jpg",this); 
    HIORegisterOutputType("bmp",this);  
    HIORegisterOutputType("gif",this);  
    HIORegisterOutputType("tif",this);  
    HIORegisterOutputType("tiff",this); 
    HIORegisterOutputType("png",this); 
    HIORegisterOutputType("tga",this); 
}

HOutputHandlerStyle HIOUtilityMagick::GetOutputStyle() {return HOutputHandlerStyleImage;};

const char * HIOUtilityMagick::GetOutputTypesString() {return "jpeg;jpg;bmp;gif;tif;tiff;png;tga";};


class HMagickInternal {

public:

    static bool FileToImage(const wchar_t *filename, 
	    HC_KEY *image_key, HC_KEY segment_key, 
	    const char *imagename, const char *format, bool local,
		HInputHandler * peer);

    static bool ImageToFile( HC_KEY image_key, const wchar_t *filename, HOutputHandler * peer, HOutputHandlerOptions *options);

};

HIOUtilityMagick::~HIOUtilityMagick() {}

char const * HIOUtilityMagick::GetInputName() {
	return "HIOUtilityMagick";
}

char const * HIOUtilityMagick::GetOutputName() {
	return "HIOUtilityMagick";
}

HFileInputResult HIOUtilityMagick::FileInputToImageKey(const wchar_t * filename, 
    HC_KEY * image_key, HC_KEY segment_key, HInputHandlerOptions * options)
{
	if(!filename) return InputBadFileName;
	if(!segment_key) return InputFail;

	SetStartingInput();

	const char * image_format = 0;
	const char * image_name = 0;

	if(options && options->m_pImageFormat) 
		image_format = options->m_pImageFormat;

	if(options) 
		image_name = options->m_pImageName;

	bool local = false;

	if (options && options->m_bLocal)
		local=true;

	bool result;

	HCLOCALE(result = HMagickInternal::FileToImage(filename, 
		image_key, segment_key, image_name, image_format, local, this));

	SetFinishedInput();

	if(result)
		return InputOK;
	else
		return InputFail;
}

HFileOutputResult HIOUtilityMagick::FileOutputFromImageKey(const wchar_t * filename, 
	HC_KEY image_key, HOutputHandlerOptions * options)
{
	if (!filename) return OutputBadFileName;
	if (!image_key) return OutputFail;

	SetStartingOutput();

	bool result;

	wchar_t ext[512];
	HUtility::FindFileNameExtension(filename, ext);
	char image_format[512];
	HC_Show_Image_Format(image_key, image_format);
	HC_KEY export_image_key = image_key;
	unsigned char * rgb_pixels = 0;

	if (wcseq(ext, L"bmp") && streq(image_format, "rgba"))
	{
		int width, height;
		unsigned char * rgba_pixels;
		HC_Show_Image_Size(image_key, 0, 0, 0, 0, &width, &height);
		HC_Show_Geometry_Pointer(image_key, "rgba rasters", &rgba_pixels);
		
		rgb_pixels = new unsigned char[width * height * 3];
		int counter = 0;
		for (int i = 0; i < (width * height * 4); ++i)
		{
			if ((i + 1) % 4 == 0)
				continue;	//skip the alpha values
			rgb_pixels[counter++] = rgba_pixels[i];
		}

		HC_KEY image_segment = HC_Show_Owner_By_Key(image_key, 0);
		HC_Open_Segment_By_Key(image_segment);
			export_image_key = HC_Insert_Image_By_Ref(0, 0, 0, "rgb", width, height, rgb_pixels);
		HC_Close_Segment();
		
	}

	HCLOCALE(result = HMagickInternal::ImageToFile(export_image_key, filename, this, options));

	if (export_image_key != image_key) 
	{
		HC_Delete_By_Key(export_image_key);
		delete [] rgb_pixels;
	}

	SetFinishedOutput();

	if(result) 
		return OutputOK;
	else
		return OutputFail;
}



class HImageInfo {
public:
	char * m_data;
	int m_len, m_width, m_height;
	char m_format[35];

	HImageInfo() : m_data(0), m_width(-1), m_height(-1) {
		strcpy(m_format, "");
	}

	~HImageInfo() {
		if(m_data)
			free(m_data);
	}
};

HImageInfo * OutsourceToImageMagick(char const * file_data, size_t count, char const * implicit_format, char const * format)
{
	HImageInfo * retval = new HImageInfo;
	ImageType it;
	bool fail = true;
	char const * magick_format = 0;
	int pixel_size = 0;

    InitializeMagick("."); 

    ExceptionInfo exception;
    GetExceptionInfo(&exception);

	ImageInfo * image_info = CloneImageInfo((ImageInfo *) NULL);
	strcpy(image_info->magick, implicit_format);
	Image * image = BlobToImage(image_info, file_data, count, &exception);

	if (exception.severity != UndefinedException && exception.severity != CoderWarning){
		CatchException(&exception);
		goto cleanup;
    }

    if (!image) {
		goto cleanup;
    }

	it = GetImageType(image, &exception);

	switch(it){
		case PaletteMatteType:
		case GrayscaleMatteType:
		case TrueColorMatteType:
			magick_format = "RGBA";
			strcpy(retval->m_format, "RGBA");
			pixel_size = 4;
			break;

		case GrayscaleType:
			magick_format = "I";
			strcpy(retval->m_format, "grayscale");
			pixel_size = 1;
		break;

		case TrueColorType:
		default:
			if(image->compression == JPEGCompression){
				magick_format = 0;
				strcpy(retval->m_format, "jpeg");
				pixel_size = 0;
			}
			else if(strieq(image->magick, "tga")){
				magick_format = 0;
				strcpy(retval->m_format, "tga");
				pixel_size = 0;
			}
			else{
				magick_format = "RGB";
				strcpy(retval->m_format, "RGB");
				pixel_size = 3;
			}
	}

	retval->m_width = image->magick_columns;
	retval->m_height = image->magick_rows;

	if(pixel_size){
		retval->m_len = retval->m_width * retval->m_height * pixel_size;
		retval->m_data = (char*)malloc(retval->m_len);
		if (retval->m_data)
			ExportImagePixels(image, 0, 0, retval->m_width, retval->m_height, magick_format, CharPixel, retval->m_data, &exception); 
		else
			goto cleanup;
	}
	else {
		retval->m_data = (char*)file_data;
		retval->m_len = (int)count;
	}

	if((strieq(implicit_format, "tif") || strieq(implicit_format, "tiff")) &&
		image->depth == 1)
	{
		const ImageAttribute* attribute = GetImageAttribute(image, "Photometric");
		if(attribute &&
		streq(attribute->value, "min-is-black"))
		{
			for(int i=0; i < retval->m_len; i++)
			{
				retval->m_data[i] = ~retval->m_data[i];
			}
		}
	}

	if (exception.severity != UndefinedException && exception.severity != CoderWarning){
		CatchException(&exception);
		goto cleanup;
    }

	fail = false;

cleanup:

	if (image)
		DestroyImage(image);

	if (image_info)
		DestroyImageInfo(image_info);

    DestroyExceptionInfo(&exception);
    DestroyMagick(); 

	if (fail) {
		delete retval;
		retval = 0;
	}

    return retval;
}


bool HMagickInternal::FileToImage(const wchar_t *filename, 
    HC_KEY *image_key_out, HC_KEY segment_key, 
    const char *imagename, const char *format, bool local,
	HInputHandler * peer)
{
	HC_KEY image_key = INVALID_KEY;
	bool retval = false;
	char implicit_format[35] = {""};
	H_FORMAT_TEXT image_spec;
	size_t count, size = 0;
	char * file_data = 0;
	FILE * in = 0;
	HImageInfo * hii = 0;
	int rc;
	wchar_t const * ext = 0;

	if (image_key_out) {
		*image_key_out = INVALID_KEY;
	}

	in = wfopen(filename, L"rb");

	if (!in) {
		peer->ReportInputInformation("Error: Unable to open file for reading");
		goto cleanup;
	}

	rc = fseek(in, 0, SEEK_END);
	if (rc == -1)
		goto cleanup;

	size = ftell(in);
	if (size <= 0)
		goto cleanup;

	rc = fseek(in, 0, SEEK_SET);
	if (rc == -1)
		goto cleanup;

	file_data = (char*)malloc(size);

	if (file_data == 0)
		goto cleanup;

	count = fread(file_data, 1, size, in);

	if (count != size)
		goto cleanup;

	ext = wcsrchr(filename, L'.');
	if (ext) {
		strncpy(implicit_format, H_ASCII_TEXT(ext+1), sizeof(implicit_format));
	}

	hii = OutsourceToImageMagick(file_data, count, implicit_format, format);
	if (!hii)
		goto cleanup;

	HC_Open_Segment_By_Key(segment_key);{
		image_spec.Append(hii->m_format);

		if (imagename)
			image_spec.Append(", name = %s", imagename);

		if (local)
			image_spec.Append(", local");

		image_key = HC_Insert_Compressed_Image(0, 0, 0, image_spec, hii->m_width, hii->m_height, hii->m_len, hii->m_data);
	}HC_Close_Segment();

	if (image_key != INVALID_KEY) {
		retval = true;
		if (image_key_out) {
			*image_key_out = image_key;
		}
	}

cleanup:

	if (in)
		fclose(in);

	if( (file_data && hii && file_data != hii->m_data) || (file_data && !hii) )
		free(file_data);

	delete hii;

    return retval;
}


bool HMagickInternal::ImageToFile( HC_KEY image_key, const wchar_t *filename, HOutputHandler * peer, HOutputHandlerOptions *options)
{
	char format[MVO_BUFFER_SIZE] = {""};   
    unsigned char *pixels=0, *gray=0;
    int width, height;
    float x,y,z;
	wchar_t const * ext = wcsrchr(filename, L'.');

	if (!ext) {
		return false;
	}
	++ext;

    HC_Show_Image_Size(image_key, &x, &y, &z, format, &width, &height);

	char * comma;

	if ((comma = strchr(format, ',')) != 0)
		*comma = '\0';

    // if not the format we like, return
    if( !strieq(format, "grayscale") &&
		!strieq(format, "tga") &&
		!strieq(format, "jpeg") &&
		!strieq(format, "rgb" ) &&
		!strieq(format, "png" ) &&
		!strieq(format, "rgba" )) {
		peer->ReportOutputInformation(H_FORMAT_TEXT("Error: Export for %s format not implemented\n", format));
		return false;
    }

	int pixel_size = 3;

	if (strieq(format, "rgba"))
		pixel_size = 4;

	if (strieq(format, "png"))
		pixel_size = HC_Show_Image_Bytes_Per_Pixel(image_key);

    if( width<1 || height < 1) {
		peer->ReportOutputInformation("Error: HOOPS Image too small to export.\n");
		return false;
    }

    pixels = new unsigned char[width*height*pixel_size];
	if(strieq(format, "grayscale")){
    	HC_Show_Geometry_Pointer(image_key, "grayscale rasters", &gray);
		for(int row = 0; row < height; ++row){
			for(int col = 0; col < width; ++col){
				for(int i = 0; i < pixel_size; ++i){
					pixels[pixel_size * (row * width + col) + i] = gray[row * width + col];
				}
			}
		}
	}
	else
		HC_Show_Partial_Image(image_key, 0, 0, width, height, pixels);

    // make ImageMagick image
	ImageInfo * image_info = 0;
	size_t count, length = 0;
	unsigned char * himage = 0;
	FILE * out = 0;

    InitializeMagick("."); 

    ExceptionInfo exception;
    GetExceptionInfo(&exception);

	Image * image = ConstituteImage ( width, height, (pixel_size==3)?"RGB":"RGBA", CharPixel, pixels, &exception ); 

	if (options && options->m_antialiasLevel > 1)
	{

		Image *image2 = ScaleImage(image,width/options->m_antialiasLevel,height/options->m_antialiasLevel,&exception);
		DestroyImage(image);
		image = image2;
	}

    if (exception.severity != UndefinedException){
		CatchException(&exception);
		peer->ReportOutputInformation("Error: CatchException(&exception);\n"); 
		goto CLEANUP_FILE_WRITE;
    }

    if (!image) {
		peer->ReportOutputInformation("Error: ConstituteImage failed!\n"); 
		goto CLEANUP_FILE_WRITE;
    }

    image_info = CloneImageInfo((ImageInfo *) NULL);
	strncpy(image_info->magick, H_ASCII_TEXT(ext), MaxTextExtent);
	if (options)
		image_info->quality = options->m_jpeg_quality;

	himage = ImageToBlob(image_info, image, &length, &exception);

    if (exception.severity != UndefinedException){
		CatchException(&exception);
		peer->ReportOutputInformation("Error: CatchException(&exception);\n"); 
		goto CLEANUP_FILE_WRITE;
    }

	if (!himage) {
		peer->ReportOutputInformation("Error: Unable to create image in requested format;\n"); 
		goto CLEANUP_FILE_WRITE;
	}

	if (options && options->m_imageOutputBuffer){
		memcpy(options->m_imageOutputBuffer,himage,length);
		options->m_imageOutputBufferSize = length;
	}
	else
	{


		out = wfopen(filename, L"wb");

		if (!out) {
			peer->ReportOutputInformation("Error: Unable to open image file for writing;\n"); 
			goto CLEANUP_FILE_WRITE;
		}

		count = fwrite(himage, 1, length, out);

		if (count != length) {
			peer->ReportOutputInformation("Error: Failed to write all image data to file;\n"); 
			goto CLEANUP_FILE_WRITE;
		}
	}

CLEANUP_FILE_WRITE:

    if(image) 
		DestroyImage(image);

	if(image_info) 
		DestroyImageInfo(image_info);

    DestroyExceptionInfo(&exception);

    DestroyMagick(); 

    delete[] pixels;

	if (himage) {
		free(himage);
	}

	if (out) {
		fclose(out);
	}

    return true;
}

HFileOutputResult HIOUtilityMagick::FileOutputFromImageKey(const unsigned short * filename, HC_KEY image_key, HOutputHandlerOptions * options){
	H_UTF16 utf16;
	utf16.encodedText((utf16_char const*) filename);
	return FileOutputFromImageKey(H_WCS(utf16).encodedText(), image_key, options);
}

HFileOutputResult HIOUtilityMagick::FileOutputFromImageKey(const char * filename, HC_KEY image_key, HOutputHandlerOptions * options){
	return FileOutputFromImageKey(H_WCS(filename).encodedText(), image_key, options);
}

HFileInputResult HIOUtilityMagick::FileInputToImageKey(const unsigned short * filename, HC_KEY * image_key, HC_KEY segment_key, HInputHandlerOptions * options){
	H_UTF16 utf16;
	utf16.encodedText((utf16_char const*) filename);
	return FileInputToImageKey(H_WCS(utf16).encodedText(), image_key, segment_key, options);
}

HFileInputResult HIOUtilityMagick::FileInputToImageKey(const char * filename, HC_KEY * image_key, HC_KEY segment_key, HInputHandlerOptions * options){
	return FileInputToImageKey(H_WCS(filename).encodedText(), image_key, segment_key, options);
}

HIOUtilityMagick::HIOUtilityMagick() { 
	SetInputOps(HInputOpFileInputToImageKey);
	SetOutputOps(HOutputOpFileOutputFromImageKey);
};

#endif

