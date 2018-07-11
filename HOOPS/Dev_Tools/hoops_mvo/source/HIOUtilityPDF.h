// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef _HIOUTILITYPDF_H
#define _HIOUTILITYPDF_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"
#include "HIOManager.h"
#include "HDB.h"
#include <string>
#include <map>



/*! DEPRECATED -> this class is no longer supported or maintained and may be removed in the future.  
    Developers who were using this class in order to export 3D-PDF content should migrate to the 
	HOOPS Exchange/Publish support for the PRC format.
	
	The HU3DOptions class provides information to the HIOUtilityU3D class to output U3D files.
*/
class MVO_API HU3DOptions
{
public:
	/*! Constructs an HU3DOptions object. */
	HU3DOptions() {Reset();};
	/*! This method sets this class to a default state. */
	void Reset(){
		m_bExportLines = m_bExportAnnotations = false;
	};

	/*! If in 3D mode, export lines optionally. */
	bool m_bExportLines;
	/*! If in 3D mode, export annotations optionally. */
	bool m_bExportAnnotations;

	/*! returns true if we are exporting lines. */
	bool ExportLines() const {return m_bExportLines;};
	/*! pass in true to export lines. */
	void ExportLines(bool onoff) {m_bExportLines = onoff;};
	/*! returns true if we are exporting annotations. */
	bool ExportAnnotations() const {return m_bExportAnnotations;};
	/*! pass in true to export annotations. */
	void ExportAnnotations(bool onoff) {m_bExportAnnotations = onoff;};
};

/*! The HIOUtilityPDF class provides support for output of 2d and 3d pdf files.
*/
class MVO_API HIOUtilityPDF: public HOutputHandler
{
public:
	/*! Constructs an HIOUtilityPDF object. */
	HIOUtilityPDF() { 
		SetOutputOps(HOutputOpFileOutputByKey);
	};

	~HIOUtilityPDF() {;};

	/*! \return A character pointer denoting the name of the handler. 
	*/
	const char * GetOutputName() { return "HIOUtilityPDF"; }

	/*!This method registers the "pdf" extension to this handler as an output handler.*/
	void RegisterOutputHandlerTypes() {
		HC_Begin_Segment_Search("/driver/pdf");{
			char seg[100];
			while(HC_Find_Segment(seg)){
				if(strieq(seg, "/driver/pdf")){
					HIORegisterOutputType("pdf",this);
					break;
				}
			}
		}HC_End_Segment_Search();
	};

	/*! \return The string of the type of output this handler creates which is "pdf".*/
	const char * GetOutputTypesString() {return "pdf";};
	/*! \return The type of output style which is HOutputHandlerStyleHardcopy.*/
	HOutputHandlerStyle GetOutputStyle() {return HOutputHandlerStyleHardcopy;};

	/*! \return The default HSRA for the HIOUtilityPDF instance.*/
	const char * GetOutputDefaultHSRA() {return "painters";};

	/*! This method checks if there's a u3d handler registered.  If there is, then we can output a 3d pdf file.
	\return True if this handler can output 3d pdf file or false if it cannot.
	*/
	bool Has3dSupport(){
		return HDB::GetHIOManager()->GetOutputHandler("u3d") != 0;
	}

	/*! This method exports the HOOPS segment tree under the given key to a pdf file.
		\param filename The name of the file to output.
		\param key The HOOPS segment key to output model data from.
		\param options Pass a pointer to HPDFOptions with information to specify the dimensions of the output as well as if you want a 3D or 2D output.
		\return The #HFileOutputResult result of the output operation.
	*/
	HFileOutputResult FileOutputByKey(const __wchar_t * filename, HC_KEY key, HOutputHandlerOptions * options);

#ifdef _MSC_VER
	HFileOutputResult FileOutputByKey(const unsigned short * filename, HC_KEY key, HOutputHandlerOptions * options){
		H_UTF16 utf16;
		utf16.encodedText((utf16_char const*) filename);
		return FileOutputByKey(H_WCS(utf16).encodedText(), key, options);
	}
#endif

	HFileOutputResult FileOutputByKey(const char * filename, HC_KEY key, HOutputHandlerOptions * options){
		return FileOutputByKey(H_WCS(filename).encodedText(), key, options);
	}

protected:
	/*! This method exports the HOOPS segment tree under the given key to a 2D pdf file.
		\param filename The name of the file to output.
		\param key The HOOPS segment key to output model data from.
		\param options Pass a pointer to HPDFOptions with information to specify the dimensions of the output as well as if you want a 3D or 2D output.
		\return The #HFileOutputResult result of the output operation.
	*/
	HFileOutputResult FileOutputByKey2D(const __wchar_t * filename, HC_KEY key, HOutputHandlerOptions * options);
	HFileOutputResult FileOutputByKey2D(const char * filename, HC_KEY key, HOutputHandlerOptions * options){
		return FileOutputByKey2D((__wchar_t*)H_WCS(filename).encodedText(), key, options);
	}


};


#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif
