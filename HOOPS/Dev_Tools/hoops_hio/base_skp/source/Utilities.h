// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

//#include "SmartSegment.h"

#include <slapi/unicodestring.h>
#include <slapi/color.h>
#include <slapi/model/material.h>
#include <slapi/transformation.h>

#include <string>
#include <vector>

bool CreateStdStringFromSUString(const SUStringRef& suString, std::string& stdString);
bool CreateStdWStringFromSUString(const SUStringRef& suString, std::wstring& stdWString);
bool GetFaceMaterials(SUFaceRef& face, SUMaterialRef& frontMaterial, SUMaterialRef& backMaterial);

std::vector<float> ConvertSUTransformation(SUTransformation const & transform);

template<typename T>
void append(std::vector<T> const & from, std::vector<T> & to)
{
	to.insert(to.end(), from.begin(), from.end());
}
