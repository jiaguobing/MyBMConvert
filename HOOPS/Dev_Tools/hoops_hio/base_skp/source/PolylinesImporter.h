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

#include <slapi/model/entities.h>
#include <slapi/model/polyline3d.h>

#include <vector>
#include "data_types.h"

class PolylinesImporter
{
public:
	virtual ~PolylinesImporter() {}

	bool Import(SegmentKey & segment, SUEntitiesRef& entities);

protected:
	PolylinesImporter(SegmentOrganizer& segmentOrganizer);
	virtual bool InsertPolyline(SegmentKey & segment, std::vector<SUPoint3Df> const & points, bool const isVisible) = 0;

private:
	PolylinesImporter(const PolylinesImporter&);
	PolylinesImporter& operator=(const PolylinesImporter&);

	bool ImportPolyline(SegmentKey & segment, SUPolyline3dRef& suPolyline);
	bool ImportPolylineGeometry(SUPolyline3dRef& suPolyline, std::vector<SUPoint3Df>& points);
	bool ImportPolyline(SegmentKey & segment, SUPolyline3dRef& suPolyline, std::vector<SUPoint3Df> const & points);

protected:
	SegmentOrganizer& m_segmentOrganizer;
};
