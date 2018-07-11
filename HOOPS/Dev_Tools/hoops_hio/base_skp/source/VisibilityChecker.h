// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#pragma once

#include <slapi/model/model.h>
#include <slapi/model/drawing_element.h>
#include <slapi/model/face.h>

#include <map>
#include <cstdint>

class VisibilityChecker
{
public:
	VisibilityChecker(SUModelRef& model, bool ignoreOffEntities);

	bool CheckElementVisibility(SUFaceRef face, bool& isVisible);
	bool CheckElementVisibility(SUDrawingElementRef drawingElement, bool& isVisible);
	bool IgnoreOffEntities() { return m_ignoreOffEntities; }

private:
	VisibilityChecker(const VisibilityChecker&);
	VisibilityChecker& operator=(const VisibilityChecker&);

private:
	std::map<int32_t, bool> m_layersVisibility;
	bool					m_ignoreOffEntities;
};