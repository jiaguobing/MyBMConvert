// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef _H3DXHELPERPUBLISH_H
#define _H3DXHELPERPUBLISH_H


#include "HIOUtilityPublish.h"

class HPublishExchangeView;
typedef VArray<HPublishExchangeView *> HPEViewsArray;

//PDF Writer using Publish
class HPublishPDFExporter
{
public:
	HPublishPDFExporter(HOutputHandler * handler);
	~HPublishPDFExporter();

	HFileIOResult ExportFromLayout(wchar_t const * file_name, A3DAsmModelFile *modelfile,  HOutputHandlerOptions *options, HPDFLayoutManager *layoutManager, wchar_t const * javascript_file);
	HFileIOResult Export(wchar_t const * file_name, A3DAsmModelFile *modelfile, HOutputHandlerOptions *options, wchar_t const * javascript_file, const HPEViewsArray& hoopsviews);
	char * GenerateJavascriptAnimationData(HBaseView *view);

private:
	HFileIOResult InitializePublish(HOutputHandlerOptions * options);

	void ReportStarted(char const * message);
	void ReportProgress(float percent);
	void ReportCompleted();

	HOutputHandler * outputHandler;
};


#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif
