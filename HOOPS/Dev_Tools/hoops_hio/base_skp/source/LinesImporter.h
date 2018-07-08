// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#pragma once

#include "SegmentOrganizer.h"

#include "data_types.h"

#include <slapi/model/entities.h>
#include <slapi/model/edge.h>

#include <vector>

class LinesImporter
{
public:
	virtual ~LinesImporter() {}

	bool Import(SegmentKey & segment, SUEntitiesRef& entities);

protected:
	LinesImporter(SegmentOrganizer& segmentOrganizer);
	virtual bool InsertLine(SegmentKey & segment, SUPoint3Df const & pointA, SUPoint3Df const & pointB, bool const isVisible) = 0;

private:
	LinesImporter(const LinesImporter&);
	LinesImporter& operator=(const LinesImporter&);

	bool ImportEdge(SegmentKey & segment, SUEdgeRef& suEdge);
	bool ImportEdgeGeometry(SUEdgeRef& suEdge, SUPoint3Df& pointA, SUPoint3Df& pointB);
	bool ImportLine(SegmentKey & segment, SUEdgeRef& suEdge, const SUPoint3Df& pointA, const SUPoint3Df& pointB);

protected:
	SegmentOrganizer& m_segmentOrganizer;
};
