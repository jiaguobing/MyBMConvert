// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef _HIOUTILITYEXCHANGE_H
#define _HIOUTILITYEXCHANGE_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"
#include "HIOManager.h"
#include "vlist.h"

#if defined(HIO_EXCHANGE) && defined(_MSC_VER)
#define HIO_API __declspec (dllexport)
#elif defined(HIO_EXCHANGE) && defined(LINUX_SYSTEM)
#define HIO_API __attribute__ ((visibility ("default")))
#else
#define HIO_API
#endif

class HPDFLayoutManager;
class HPDFOptions;
/*! The HIOUtilityExchange class is used for loading a variety of CAD formats (mostly Brep) into HOOPS. It requires an installation of HOOPS Exchange.
	The CAD file is converted to an internal format (PRC) which is then parsed for hierachy information and to create the tesselated data.

*/
class HIO_API HIOUtilityExchange: public HInputHandler, public HOutputHandler
{

public:

	/*!Constructs an HIOUtility object.*/
	HIOUtilityExchange();
	virtual ~HIOUtilityExchange() {};

	/*!This method registers the appropriate extensions to this handler as an input handler.*/
	void RegisterInputHandlerTypes();
	/*! \return A character pointer denoting the name of the handler. 
	*/
	const char * GetOutputName() { return "HIOUtilityExchange"; }
	const char * GetInputName() { return "HIOUtilityExchange"; }

	/*!	\returns The input extension type that is handled by HIOUtilityExchange.*/
	const char * GetInputTypesString() {return "prc;prd;x_t;x_b;step;stp;igs;iges;CATPart;CATProduct;"
		"cgr;session;model;3dxml;CADDS;PD;XV3;XV0;prt;asm;xpr;xas;neu;asm.1;prt.1;jt;SLDPRT;SLDASM;mf1;"	
	"arc;unv;pkg;sdp;sdpc;sdw;sdwc;sda;sdac;sds;sdsc;ses;bdl;psm;pwd;par;asm;sat;sab;ipt;iam;ifc;dae"; }
 
		

	/*! \return The type of input style which is #HInputHandlerStyleModel.*/
	HInputHandlerStyle GetInputStyle() {return HInputHandlerStyleModel;};

	/*!
	  This method reads the modeller file 'filename' into the segment tree underneath key.
		\param filename The name of the file to load.
		\param key The HOOPS segment key to load model data into.
		\param options An options class containing any needed extra information for operation.
		\return The #HFileInputResult result of the input operation.
	*/
	HFileInputResult FileInputByKey(const wchar_t * FileName, HC_KEY key, HInputHandlerOptions * options);

	HFileInputResult FileInputByKey(const char * FileName, HC_KEY key, HInputHandlerOptions * options){
		return FileInputByKey(H_WCS(FileName).encodedText(), key, options);
	};

	HFileInputResult FileInputByKey(const unsigned short * FileName, HC_KEY key, HInputHandlerOptions * options){
		H_UTF16 utf16;
		utf16.encodedText((utf16_char const*) FileName);
		return FileInputByKey(H_WCS(utf16).encodedText(), key, options);
	};

	HFileOutputResult FileOutputByKey(const wchar_t * filename, HC_KEY key, HOutputHandlerOptions * options);

	HFileOutputResult FileOutputByKey(const unsigned short * filename, HC_KEY key, HOutputHandlerOptions * options){
		H_UTF16 utf16;
		utf16.encodedText((utf16_char const*) filename);
		return FileOutputByKey(H_WCS(utf16).encodedText(), key, options);
	}

	HFileOutputResult FileOutputByKey(const char * filename, HC_KEY key, HOutputHandlerOptions * options){
		return FileOutputByKey(H_WCS(filename).encodedText(), key, options);
	}

	void RegisterOutputHandlerTypes();
	
	/*!
		!This method creates a A3DAsmModelFile object from a HOOPS segment tree 
		\param options Output Handler Options. The m_pHBaseView must be set
		\param key Start Segment Key
 		\return A3DAsmModelFile pointer
	*/
	virtual void *BuildPRCModel(HOutputHandlerOptions * options, HC_KEY key);
 

	const char *  GetOutputTypesString() {
		return "prc;iges;step;x_t;stl";
	}

	HOutputHandlerStyle GetOutputStyle() {
		return HOutputHandlerStyleModel;
	}
};



#ifdef HIO_EXCHANGE
extern "C" {
	HIO_API void * CreateInput(HIOManager *manager);
	HIO_API void * CreateOutput(HIOManager *manager);
	HIO_API void * CreateConnector(void * unused);
	HIO_API void Free(HIOUtilityExchange *);
 }


#endif

#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif
