// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef _HIOUTILITYSTL_H
#define _HIOUTILITYSTL_H


#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"
#include "HIOManager.h"
#include "varray.h"


/*! The HIOUtilitySTL allows you to read and write binary and ASCII stereolithography or STL files.
*/


class MVO_API HIOUtilityStl: public HInputHandler, public HOutputHandler
{

public:

	/*!Constructs an HIOUtilityStl object.*/
	HIOUtilityStl() {
		SetInputOps(HInputOpFileInputByKey);
		SetOutputOps(HOutputOpFileOutputByKey);
	};
	virtual ~HIOUtilityStl() {;};

	/*! \return A character pointer denoting the name of the handler. 
	*/
	const char * GetOutputName() { return "HIOUtilityStl"; }
	const char * GetInputName() { return "HIOUtilityStl"; }

	/*!This method registers the "stl" extension to this handler as an input handler.*/
	void RegisterInputHandlerTypes() {
		HIORegisterInputType("stl",this);
	};

	/*!This method registers the "stl" extension to this handler as an output handler.*/
	void RegisterOutputHandlerTypes() {
		HIORegisterOutputType("stl",this);
	};

	/*!	\returns The input extension type that is handled by HIOUtilityStl which is "stl".*/
	const char * GetInputTypesString() {return "stl";};

	/*! \return The string of the type of output this handler creates which is "stl".*/
	const char * GetOutputTypesString() {return "stl";};

	/*! \return The type of input style which is #HInputHandlerStyleModel.*/
	HInputHandlerStyle GetInputStyle() {return HInputHandlerStyleModel;};

	/*! \return The type of output style which is HOutputHandlerStyleModel.*/
	HOutputHandlerStyle GetOutputStyle() {return HOutputHandlerStyleModel;};

	/*!
	  This method reads a stereolithography file.  The file contents are mapped to a HOOPS shell 
	  primitive that is inserted into the currently open HOOPS segment. Both ASCII and binary .stl 
	  files are supported.

		\param filename The name of the file to load.
		\param key The HOOPS segment key to load model data into.
		\param options This parameter is currently not being used.
		\return The #HFileInputResult result of the input operation.
	*/
	HFileInputResult FileInputByKey(const __wchar_t * FileName, HC_KEY key, HInputHandlerOptions * options);

	HFileInputResult FileInputByKey(const char * FileName, HC_KEY key, HInputHandlerOptions * options){
		return FileInputByKey(H_WCS(FileName).encodedText(), key, options);
	};

	HFileInputResult FileInputByKey(const unsigned short * FileName, HC_KEY key, HInputHandlerOptions * options){
		H_UTF16 utf16;
		utf16.encodedText((utf16_char const*) FileName);
		return FileInputByKey(H_WCS(utf16).encodedText(), key, options);
	};


	/*!
	  This method writes a stereolithography file out of the segment tree underneath the given key. Both ASCII and 
	  binary .stl files are supported.
		\param filename The name of the file to output.
		\param key The HOOPS segment key to output model data from.
		\param options Set this parameter's m_bAscii member to true if you want to write an ASCII .stl file or false to write a binary .stl file.
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

private:

	static struct stl_info
	{
		FILE *f;
		bool is_ascii;
		HC_KEY start_segment;
		HC_KEY copy_segment;
		int triangle_count;
	} stl_info_s;

	static struct include_list
	{
		HC_KEY include;
		include_list *next;
	} include_list_s;

	static HFileInputResult ReadStlFile(const __wchar_t *FileName);
	static HFileInputResult ReadStlFileAscii(const __wchar_t *FileName);
	static HFileInputResult ReadStlFileBinary(const __wchar_t *FileName);
	static HFileOutputResult WriteStlFile(const __wchar_t *FileName, const float *vertices, const float *normals, unsigned int triangle_count, bool is_ascii, char *partname);
	static HFileOutputResult WriteStlFileAscii(const __wchar_t *FileName, const float *vertices, const float *normals, unsigned int triangle_count, char *partname);
	static HFileOutputResult WriteStlFileBinary(const __wchar_t *FileName, const float *vertices, const float *normals, unsigned int triangle_count);
	static HFileOutputResult ComputeStlTriangles(HC_KEY the_key, stl_info *info, HOutputHandlerOptions *options, bool no_recurse=false, float matrix_for_segment_reference[]=0 );
};


#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif
