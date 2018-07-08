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

#include "HIOUtilityPDF.h"
#include "HUtility.h"
#include "HBaseView.h"
#include "varray.h"
#include "HUtilityLocaleString.h"
#include "vlist.h"
#include "vhash.h"
#include "HBhvBehaviorManager.h"
#include "HBaseModel.h"
#include "HBhvAnimation.h"
#include "HBhvTimeline.h"
#include <vector>
#include <map>
#include <set>

#include <sys/stat.h>

#include "ezpdf.h"

using namespace ezpdf;
using namespace std;

/* HPDFPoster is a utility class used to help HIOUtilityPDF create 3d pdf files.
*/
class MVO_API HPDFPoster
{
public:
	/* Constructs an HPDFPoster object. */
	HPDFPoster(){
		left = right = bottom = top = length = width = height = 0;
		data = (unsigned char*)0;
	}

	~HPDFPoster(){
		if(data)
			delete [] data;
	}
	/* The image bounding in inches.*/
	float left, right, bottom, top;

	/* The image width and height in pixels. */
	int width, height;
	/* The uncompressed length of the image. */
	int length;
	/* The uncompressed image data. */
	unsigned char * data;
};


class MVO_API HPDFStream
{
public:
	/* Constructs a HPDFStream object. */
	HPDFStream(){
		length = 0;
		data = (unsigned char*)0;
	}

	~HPDFStream(){
		delete [] data;
	}
	/* The length of the uncompressed data. */
	int length;

	/* The stream data. */
	unsigned char * data;

};

HFileOutputResult HIOUtilityPDF::FileOutputByKey(const wchar_t * FileName, HC_KEY key, HOutputHandlerOptions * options)
{
	HOutputHandlerOptions auto_options;
	HOutputHandlerOptions * pdf_options = options;
	if( strcmp(pdf_options->GetType(), "HPDFOptions") != 0 ){
		pdf_options = &auto_options;
		pdf_options->Init(options);
	}

	SetStartingOutput();



	HFileOutputResult retval = OutputFail;
	
	ReportOutputInformation("Exporting 2D PDF");
	retval = FileOutputByKey2D(FileName, key, pdf_options);


#if 0
	switch(pdf_options->OutputType()){
		case H_PDF_OUTPUT_PORTFOLIO:{
			ReportOutputInformation("Exporting PDF Portfolio");
			retval = FileOutputByKeyPortfolio(FileName, key, pdf_options);
		}break;

		case H_PDF_OUTPUT_3D:{
			ReportOutputInformation("Exporting 3D PDF");
			if(HDB::GetHIOManager()->GetOutputHandler("PRC"))
				retval = FileOutputByKey3D(FileName, "PRC", key, pdf_options);
			else if(HDB::GetHIOManager()->GetOutputHandler("U3D"))
				retval = FileOutputByKey3D(FileName, "U3D", key, pdf_options);
			else
				retval = OutputFail;
		}break;

		case H_PDF_OUTPUT_2D:
		default:
			ReportOutputInformation("Exporting 2D PDF");
			retval = FileOutputByKey2D(FileName, key, pdf_options);
	}
#endif
	SetFinishedOutput();

	return retval;	
}
 
 





/* When we finally fix the HInputOptions and HOutputOptions interface, this should be removed. */
typedef	struct AVIExportInfo {

	int starttick;		//!< Starttick for animation.
	int endtick;		//!< Endtick for animation.
	int resolution;		//!< The number of frames per second.
} AVIExportInfo;	//!< AVIExportInformation  Typedef for AVIExportInfo


static
bool WriteAviForPDF(HBaseView * m_pHView, wchar_t const * filename)
{
	bool retval = true;
	HBhvBehaviorManager *pBehaviorManager = m_pHView->GetModel()->GetBhvBehaviorManager();
	vlist_t * alist = pBehaviorManager->GetAnimationList();
	if(vlist_count(alist) > 0){

		HOutputHandler * aviio = HDB::GetHIOManager()->GetOutputHandler("avi");

		if(aviio){
			AVIExportInfo aviinfo;
			HBhvAnimation * animation = (HBhvAnimation *)vlist_peek_first(alist);

			aviinfo.starttick = animation->GetFirstTick();
			aviinfo.endtick = animation->GetLastTick();
			aviinfo.resolution = 33;

			HOutputHandlerOptions options;
			options.ExtendedData((void*)&aviinfo);
			options.View(m_pHView);
			options.WindowWidth(640);
			options.WindowHeight(480);

			if(aviio->FileOutputByKey(filename, m_pHView->GetModelKey(), &options) != OutputOK){
				retval = false;
			}
		}
		else return false;
	}
	else {
		m_pHView->FitWorld();
		m_pHView->ForceUpdate();
		m_pHView->SetSuppressUpdate(true);
		HCamera oldcamera;
		m_pHView->GetCamera(&oldcamera);
		bool oldsmooth = m_pHView->GetSmoothTransition();
		pBehaviorManager->SetCurrentTick(0);
 		m_pHView->SetSmoothTransition(false);
		m_pHView->SetViewMode(HViewFront);
		HBhvUtility::MakeCameraKeyframe(m_pHView,100, true);
		m_pHView->SetViewMode(HViewBack);
		HBhvUtility::MakeCameraKeyframe(m_pHView,100, true);
		m_pHView->SetViewMode(HViewIsoBackRightTop);
		HBhvUtility::MakeCameraKeyframe(m_pHView,100, true);		
		m_pHView->SetSuppressUpdate(false);
		m_pHView->SetCamera(oldcamera);
 		
		retval = WriteAviForPDF(m_pHView, filename);
		m_pHView->SetCamera(oldcamera);
		m_pHView->SetSmoothTransition(oldsmooth);	
		pBehaviorManager->DeleteAllAnimations();
 	}
	return retval;
}


HFileOutputResult HIOUtilityPDF::FileOutputByKey2D(const wchar_t * FileName, HC_KEY key, HOutputHandlerOptions * options) 
{
	char DriverPath[MVO_SEGMENT_PATHNAME_BUFFER];

	if(!FileName) return OutputBadFileName;
	if(key == INVALID_KEY) return OutputFail;
	if(!options) return OutputFail;
	if( !options->m_pHBaseView )
	    return OutputFail;
	
	if(options->WindowWidth() <= 0 || options->WindowHeight() <= 0){
		options->WindowWidth(8.5f);
		options->WindowHeight(11);
	}

	/* append FileName to "?driver/pdf/", enclose it in 
	 * single quotes to maintain case and to protect special characters 
	 * from interpretation as segment delimeters */
	//sprintf(DriverPath, "%s\'%s\'", "?driver/pdf/", (char*)H_UTF8(FileName).encodedText());	
	sprintf(DriverPath, "%s\'%s\'", "?driver/pdf/", H_ASCII_TEXT(FileName));	
	// now we print the file!!!
	HFileOutputResult retval = HUtility::TwoPassPrint(DriverPath, options);
	HC_Delete_Segment(DriverPath);

	return retval;
}

#define PDF_INCHES(x) (72*(x))
#define PDF_POINTS(x) (x)



 

static char const document_script[] = {
"var waitingFor3DActivated = true;"

"function Initialize(){"
"	console.println(\"Checking if 3D Annotation is activate...\");"
"	if(waitingFor3DActivated){"
"		var a3d = getAnnots3D(0)[0];"
"		if(a3d.activated){"
"			waitingFor3DActivated = false;"
"			console.println(\"...3D Annotation is active.\");"
"			app.clearInterval(timeout);"
"			context3D = a3d.context3D;"
"			context3D.setLightScheme(\"cad\");"
"			context3D.setRenderMode(\"tranparent\");"
"		}"
"		if(timeout.count >= 60){/*seconds*/"
"			console.println(\"Giving up on 3D Annotation...\");"
"			app.clearInterval(timeout);"
"		}"
"	}"
"	timeout.count++;"
"}"

"timeout = app.setInterval(\"Initialize()\", 1000);"
"timeout.count = 0;"
};




