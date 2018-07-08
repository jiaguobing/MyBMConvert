// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef _HIOUTILITYHTML_H
#define _HIOUTILITYHTML_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"
#include "HIOManager.h"
#include "HStream.h"

/*! The HIOUtilityHTML is used for exporting a HTML page which contains a control which
	references a HSF file of the same name.  The handler creates both the HTML page and 
	the HSF.  
	
	Note, the HTML page can only be viewed with Microsoft Internet Explorer.
*/


class MVO_API HIOUtilityHTML: public HOutputHandler
{

public:

	/*!Constructs an HIOUtilityHTML object.*/
	HIOUtilityHTML() {
		SetOutputOps(HOutputOpFileOutputByKey);
	};

	virtual ~HIOUtilityHTML() {;};

	/*! \return A character pointer denoting the name of the handler. 
	*/
	const char * GetOutputName() { return "HIOUtilityHTML"; }

	/*!This method registers the "html" and "htm" extensions to this handler as an output handler.*/
	void RegisterOutputHandlerTypes() {
		HIORegisterOutputType("html",this);
		HIORegisterOutputType("htm",this);
	};

	/*! \return The string of the type of output this handler creates which are "html" and "htm".*/
	const char * GetOutputTypesString() {return "html;htm";};

	/*! \return The type of output style which is #HOutputHandlerStyleHTML.*/
	HOutputHandlerStyle GetOutputStyle() {return HOutputHandlerStyleHTML;};

	/*!
	  This method creates an HTML page designed to work with a separatly created but
	  identically named .hsf file.  The HTML file embeds a HOOPS ActiveX control which loads the HSF into the
	  HTML page.
  
	  \param FileName A character pointer denoting the name of the file to be written should usually include ".html".
	  \param key  This parameter is current not used.
	  \param options  Pass options with specified width and height to determine the page dimensions.
	 \return The #HFileOutputResult result of the output operation.
	*/	
	HFileOutputResult FileOutputByKey(const __wchar_t * filename, HC_KEY key, HOutputHandlerOptions * options);

	HFileOutputResult FileOutputByKey(const unsigned short * filename, HC_KEY key, HOutputHandlerOptions * options){
		H_UTF16 utf16;
		utf16.encodedText((utf16_char const*) filename);
		return FileOutputByKey(H_WCS(utf16).encodedText(), key, options);
	}

	HFileOutputResult FileOutputByKey(const char * filename, HC_KEY key, HOutputHandlerOptions * options){
		return FileOutputByKey(H_WCS(filename).encodedText(), key, options);
	}

};


#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif
