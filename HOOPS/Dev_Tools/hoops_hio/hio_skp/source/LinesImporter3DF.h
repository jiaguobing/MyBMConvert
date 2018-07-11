// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#pragma once

#include "LinesImporter.h"

class LinesImporter3DF : public LinesImporter
{
public:
	LinesImporter3DF(SegmentOrganizer& segmentOrganizer);

private:
	LinesImporter3DF(const LinesImporter3DF&);
	LinesImporter3DF& operator=(const LinesImporter3DF&);

	virtual bool InsertLine(SegmentKey & segment, SUPoint3Df const & pointA, SUPoint3Df const & pointB, bool const isVisible);
};
