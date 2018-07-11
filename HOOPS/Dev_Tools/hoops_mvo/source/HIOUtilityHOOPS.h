// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef _HIOUTILITYHOOPS_H
#define _HIOUTILITYHOOPS_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"
#include "HIOManager.h"
#include "HDB.h"



/*! The HIOUtilityHMF is the class for reading and writing HOOPS Metafiles (HMFs).  HMFs are the 
	ASCII definition of a HOOPS Scene Graph and are primarily used for debugging purposes.
*/


class MVO_API HIOUtilityHMF: public HInputHandler, public HOutputHandler
{
public:
	/*!Constructs an HIOUtilityHMF object.*/
	HIOUtilityHMF() { 
		SetInputOps(HInputOpFileInputByKey);
		SetOutputOps(HOutputOpFileOutputByKey);
	};

	~HIOUtilityHMF() {;};


	/*! \return A character pointer denoting the name of the handler. 
	*/
	const char * GetOutputName() { return "HIOUtilityHMF"; }
	const char * GetInputName() { return "HIOUtilityHMF"; }



	/*!This method registers the "hmf" extension to this handler as an input handler.*/
	void RegisterInputHandlerTypes() {
		HIORegisterInputType("hmf",this);
		HIORegisterInputType("gz",this);
	};
	/*! \return The type of input style which is #HInputHandlerStyleModel.*/
	HInputHandlerStyle GetInputStyle() {return HInputHandlerStyleModel;};

	/*!	\returns The input extension type that is handled by HIOUtilityHMF which is "hmf".*/
	const char * GetInputTypesString() {return "hmf;gz";};

	/*!
	  This method reads the HMF into the segment defined by key.
		\param filename The name of the file to load.
		\param key The HOOPS segment key to load model data into.
		\param options An options class containing any needed extra information for operation.
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

	/*!This method registers the "hmf" extension to this handler as an output handler.*/
	void RegisterOutputHandlerTypes() {
		HIORegisterOutputType("hmf",this);
	};

	/*! \return The string of the type of output this handler creates which is "hmf".*/
	const char * GetOutputTypesString() {return "hmf";};

	/*! \return The type of output style which is HOutputHandlerStyleModel.*/
	HOutputHandlerStyle GetOutputStyle() {return HOutputHandlerStyleModel;};

	/*!
	  This method writes a HMF out of the segment tree underneath the given key.
		\param filename The name of the file to output.
		\param key The HOOPS segment key to output model data from.
		\param options The options class containing any needed extra information for the operation.
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


/*! The HIOUtilityPostscript is the class for writing postscript files using the HOOPS
	postscript driver.
*/
class MVO_API HIOUtilityPostscript: public HOutputHandler
{
public:
	/*!Constructs an HIOUtilityPostscript object.*/
	HIOUtilityPostscript() { 
		SetOutputOps(HOutputOpFileOutputByKey);
	};

	~HIOUtilityPostscript() {;};

	/*! \return A character pointer denoting the name of the handler. 
	*/
	const char * GetOutputName() { return "HIOUtilityPostscript"; }
	const char * GetInputName() { return "HIOUtilityPostscript"; }

	/*!This method registers the "ps" extension to this handler as an output handler.*/
	void RegisterOutputHandlerTypes() {
		HC_Begin_Segment_Search("/driver/postscript");{
			char seg[100];
			while(HC_Find_Segment(seg)){
				if(strieq(seg, "/driver/postscript")){
					HIORegisterOutputType("ps",this);
					HIORegisterOutputType("eps",this);
					break;
				}
			}
		}HC_End_Segment_Search();
	};

	/*! \return The string of the type of output this handler creates which is "ps".*/
	const char * GetOutputTypesString() {return "ps";};

	/*! \return The type of output style which is #HOutputHandlerStyleHardcopy.*/
	HOutputHandlerStyle GetOutputStyle() {return HOutputHandlerStyleHardcopy;};

	/*! \return The default HSRA for the HIOUtilityPostscript instance.*/
	const char * GetOutputDefaultHSRA() {return "painters";};

	/*!
	  This method writes a postscript file out of the segment tree underneath the given key.
		\param filename The name of the file to output.
		\param key The HOOPS segment key to get output data from.
		\param options The options class containing any needed extra information for the operation.
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


/*! The HIOUtilityHPGL is the class for writing HPGL2 files using the HOOPS
	HPGL2 driver.  The output files are suitable to send to a HPGL2 printer or plotter.
*/
class MVO_API HIOUtilityHPGL2: public HOutputHandler
{
public:
	/*!Constructs an HIOUtilityHPGL2 object.*/
	HIOUtilityHPGL2() { 
		SetOutputOps(HOutputOpFileOutputByKey);
	};

	~HIOUtilityHPGL2() {;};

	/*! \return A character pointer denoting the name of the handler. 
	*/
	const char * GetOutputName() { return "HIOUtilityHPGL2"; }

	/*!This method registers the "hp" extension to this handler as an output handler.*/
	void RegisterOutputHandlerTypes() {
		HC_Begin_Segment_Search("/driver/hpgl2");{
			char seg[100];
			while(HC_Find_Segment(seg)){
				if(strieq(seg, "/driver/hpgl2")){
					HIORegisterOutputType("hp",this);
					break;
				}
			}
		}HC_End_Segment_Search();
	};

	/*! \return The string of the type of output this handler creates which is "hp".*/
	const char * GetOutputTypesString() {return "hp";};

	/*! \return The type of output style which is #HOutputHandlerStyleHardcopy.*/
	HOutputHandlerStyle GetOutputStyle() {return HOutputHandlerStyleHardcopy;};

	/*! \return The default HSRA for the HIOUtilityHPGL2 instance.*/
	const char * GetOutputDefaultHSRA() {return "painters";};

	/*!
	  This method writes a HPGL2 file out of the segment tree underneath the given key.
		\param filename The name of the file to output.
		\param key The HOOPS segment key to get output data from.
		\param options The options class containing any needed extra information for the operation.
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

/*! The HIOUtilityCGM is the class for writing CGM files using the HOOPS
	CGM driver.  
*/
class MVO_API HIOUtilityCGM: public HOutputHandler
{
public:
	/*!Constructs an HIOUtilityCGM object.*/
	HIOUtilityCGM() { 
		SetOutputOps(HOutputOpFileOutputByKey);
	};

	~HIOUtilityCGM() {;};

	/*! \return A character pointer denoting the name of the handler. 
	*/
	const char * GetOutputName() { return "HIOUtilityCGM"; }


	/*!This method registers the "cgm" extension to this handler as an output handler.*/
	void RegisterOutputHandlerTypes() {
		HC_Begin_Segment_Search("/driver/cgm3");{
			char seg[100];
			while(HC_Find_Segment(seg)){
				if(strieq(seg, "/driver/cgm3")){
					HIORegisterOutputType("cgm",this);
					break;
				}
			}
		}HC_End_Segment_Search();
	};

	/*! \return The string of the type of output this handler creates which is "cgm".*/
	const char * GetOutputTypesString() {return "cgm";};

	/*! \return The type of output style which is #HOutputHandlerStyleHardcopy.*/
	HOutputHandlerStyle GetOutputStyle() {return HOutputHandlerStyleHardcopy;};

		/*! \return The default HSRA for the HIOUtilityCGM instance.*/
	const char * GetOutputDefaultHSRA() {return "painters";};

	/*!
	  This method writes a CGM file out of the segment tree underneath the given key.
		\param filename The name of the file to output.
		\param key The HOOPS segment key to get output data from.
		\param options The options class containing any needed extra information for the operation.
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
