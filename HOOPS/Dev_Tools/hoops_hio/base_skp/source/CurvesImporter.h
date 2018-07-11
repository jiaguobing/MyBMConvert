// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#pragma once

#include <slapi/model/entities.h>
#include <slapi/model/curve.h>
#include <slapi/model/edge.h>

#include "SegmentOrganizer.h"

#include <vector>
#include "data_types.h"

#include "SegmentKey.h"

class CurvesImporter
{
public:
	virtual ~CurvesImporter() {}

	bool Import(SegmentKey & segment, SUEntitiesRef& entities);

protected:
	CurvesImporter(SegmentOrganizer & segmentOrganizer);
	virtual bool InsertArc(SegmentKey & segment, const std::vector<SUPoint3Df>& points, bool isVisible) = 0;

private:
	CurvesImporter(const CurvesImporter&);
	CurvesImporter& operator=(const CurvesImporter&);

	bool ImportCurve(SegmentKey & segment, SUCurveRef& suCurve);

	bool ImportCurveGeometry(SegmentKey & segment, SUCurveRef& suCurve, std::vector<SUPoint3Df>& points, bool & isVisible, SegmentKeyPtr & targetSegPtr);
	bool ImportStartVertexFromEdge(SUEdgeRef& edge, std::vector<SUPoint3Df>& points);
	bool ImportBothVerticesFromEdge(SUEdgeRef& edge, std::vector<SUPoint3Df>& points);


	SegmentOrganizer & m_segmentOrganizer;
};
