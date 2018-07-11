// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <wchar.h>

#ifndef _WIN32_WCE
#include <sys/stat.h>
#endif

#include "HIOUtilityHOOPS.h"
#include "HImHideOverlappedText.h"
#include "HUtility.h"
#include "HBaseView.h"
#include "HUtilityLocaleString.h"


HFileInputResult HIOUtilityHMF::FileInputByKey(const wchar_t * FileName, HC_KEY key, HInputHandlerOptions * options)
{
	UNREFERENCED(options);

	if(!FileName) return InputBadFileName;
	if(key == INVALID_KEY) return InputFail;

	wchar_t file_name[MVO_BUFFER_SIZE];
	wchar_t dir_name[MVO_BUFFER_SIZE];
	wchar_t hmf_options[MVO_BUFFER_SIZE];
	
	SetStartingInput((char const*)H_UTF8(FileName).encodedText());

	wcscpy(hmf_options, L"restore state = (color names, fonts, textures)");

	HUtility::FindFileNameAndDirectory(FileName, dir_name, file_name);

	if(!wcseq(dir_name, L"")) {
		wcscat(hmf_options, L", directory=\"");
		wcscat(hmf_options, dir_name);
		wcscat(hmf_options, L"\"");
	} 

	if(wcseq(file_name, L""))
		return InputBadFileName;

	HC_Open_Segment_By_Key(key);
	//HC_Read_Metafile((char*)H_UTF8(file_name).encodedText(), ".", (char*)H_UTF8(hmf_options).encodedText()); 
	HC_Read_Metafile((char*)H_UTF8(file_name).encodedText(), ".", H_ASCII_TEXT(hmf_options)); 
	HC_Close_Segment();

//TODO some sort of err handling 
	HFileInputResult result = InputOK;

	SetFinishedInput();

	return result;
}


//#define SUPPRESS_TRISTRIPS

HFileOutputResult HIOUtilityHMF::FileOutputByKey(const wchar_t * FileName, HC_KEY key, HOutputHandlerOptions * options)
{
	UNREFERENCED(options);

	if(!FileName) return OutputBadFileName;
	if(key == INVALID_KEY) return OutputFail;

	wchar_t file_name[MVO_BUFFER_SIZE];
	wchar_t dir_name[MVO_BUFFER_SIZE];
	wchar_t hmf_options[MVO_BUFFER_SIZE];

	SetStartingOutput();

	wcscpy(hmf_options, L"use color names, follow cross-references, save state = texture");

	HUtility::FindFileNameAndDirectory(FileName, dir_name, file_name);

	if(!wcseq(dir_name, L"")) {
		wcscat(hmf_options, L", directory=\"");
		wcscat(hmf_options, dir_name);
		wcscat(hmf_options, L"\"");
	} 

#ifdef SUPPRESS_TRISTRIPS
	if(!wcseq(hmf_options, L"")) {
		wcscat( hmf_options, L", " );
	}
	wcscat( hmf_options, L"no tristrips" );
#endif

	if(file_name[0] == L'\0')
		return OutputBadFileName;

	HC_Open_Segment_By_Key(key);
		//HC_Write_Metafile(".", (char*)H_UTF8(file_name).encodedText(), (char*)H_UTF8(hmf_options).encodedText()); 
		HC_Write_Metafile(".", H_ASCII_TEXT(file_name), H_ASCII_TEXT(hmf_options)); 
	HC_Close_Segment();

	//TODO some sort of err handling 
	HFileOutputResult result = OutputOK;

	SetFinishedOutput();

	return result;
}


HFileOutputResult HIOUtilityPostscript::FileOutputByKey(const wchar_t * FileName, HC_KEY key, HOutputHandlerOptions * options) 
{
	char DriverPath[MVO_SEGMENT_PATHNAME_BUFFER];

	if(!FileName) return OutputBadFileName;
	if(key == INVALID_KEY) return OutputFail;
	if(!options) return OutputFail;
	if( !options->m_pHBaseView )
	    return OutputFail;

	SetStartingOutput();

	if(options->m_Window_Width <= 0 || options->m_Window_Height <= 0){
		options->m_Window_Width = 8.5;
		options->m_Window_Height = 11;
	}

	/* append FileName to "?driver/postscript/", enclose it in 
	 * single quotes to maintain case and to protect special characters 
	 * from interpretation as segment delimeters */
	//sprintf(DriverPath, "%s\'%s\'", "?driver/postscript/", (char*)H_UTF8(FileName).encodedText());	
	sprintf(DriverPath, "%s\'%s\'", "?driver/postscript/", H_ASCII_TEXT(FileName));	

	// now we print the file!!!
	HFileOutputResult retval = HUtility::TwoPassPrint(DriverPath, options);
	HC_Delete_Segment(DriverPath);

	SetFinishedOutput();

	return retval;
}

HFileOutputResult HIOUtilityHPGL2::FileOutputByKey(const wchar_t * FileName, HC_KEY key, HOutputHandlerOptions * options) 
{
	char DriverPath[MVO_SEGMENT_PATHNAME_BUFFER];

	if(!FileName) return OutputBadFileName;
	if(key == INVALID_KEY) return OutputFail;
	if(!options) return OutputFail;
	if( !options->m_pHBaseView )
	    return OutputFail;

	SetStartingOutput();

	/* append FileName to "?driver/hpgl2/", enclose it in 
	 * single quotes to maintain case and to protect special characters 
	 * from interpretation as segment delimeters */
	//sprintf(DriverPath, "%s\'%s\'", "?driver/hpgl2/", (char*)H_UTF8(FileName).encodedText());	
	sprintf(DriverPath, "%s\'%s\'", "?driver/hpgl2/", H_ASCII_TEXT(FileName));

	// now we print the file!!!
	HFileOutputResult retval = HUtility::TwoPassPrint(DriverPath, options);
	HC_Delete_Segment(DriverPath);

	SetFinishedOutput();

	return retval;
}


HFileOutputResult HIOUtilityCGM::FileOutputByKey(const wchar_t * FileName, HC_KEY key, HOutputHandlerOptions * options) 
{
	char DriverPath[MVO_SEGMENT_PATHNAME_BUFFER];

	if(!FileName) return OutputBadFileName;
	if(key == INVALID_KEY) return OutputFail;
	if(!options) return OutputFail;
	if( !options->m_pHBaseView )
	    return OutputFail;

	SetStartingOutput();

	/* append FileName to "?driver/cgm3/", enclose it in 
	 * single quotes to maintain case and to protect special characters 
	 * from interpretation as segment delimeters */
	//sprintf(DriverPath, "%s\'%s\'", "?driver/cgm3/", (char*)H_UTF8(FileName).encodedText());	
	sprintf(DriverPath, "%s\'%s\'", "?driver/cgm3/", H_ASCII_TEXT(FileName));

	// now we print the file!!!
	HFileOutputResult retval = HUtility::TwoPassPrint(DriverPath, options);
	HC_Delete_Segment(DriverPath);

	SetFinishedOutput();

	return retval;
}

