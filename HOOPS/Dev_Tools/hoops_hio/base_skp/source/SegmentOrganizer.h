// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#pragma once

#include "StyleRepository.h"
#include "TextureWriter.h"
#include "VisibilityChecker.h"

#include <slapi/model/face.h>
#include <slapi/model/drawing_element.h>

#include <map>
#include <cstdint>

#include "SegmentKey.h"

class SegmentOrganizer
{
public:
	virtual ~SegmentOrganizer() {}

	SegmentKeyPtr GetSegmentKey(SegmentKey & segment, SUFaceRef& face);
	SegmentKeyPtr GetSegmentKey(SegmentKey & segment, SUDrawingElementRef drawingElement);

	VisibilityChecker & GetVisibilityChecker() { return m_visibilityChecker; }

protected:
	SegmentOrganizer(StyleRepository& styleRepository, VisibilityChecker& visibilityChecker);
	SegmentOrganizer(const SegmentOrganizer&);
	SegmentOrganizer& operator=(const SegmentOrganizer&);

	virtual void StyleSegment(SegmentKey & segment, SegmentKey & styleSegment) = 0;
	virtual void HideSegment(SegmentKey & segment) = 0;

protected:
	StyleRepository&   m_styleRepository;
	VisibilityChecker& m_visibilityChecker;

private:
	template<typename SUTYPE, typename IDTYPE, typename MAPTYPE>
	SegmentKeyPtr GetSegmentKeyCommon(SegmentKey & segment, SUTYPE & element, IDTYPE const & id, MAPTYPE & map);

	SegmentKeyPtr CreateVisibleSegment(SegmentKey & parentSegment, SegmentKey & styleSegment);
	SegmentKeyPtr CreateHiddenSegment(SegmentKey & parentSegment);

	struct SegmentPair {
		SegmentPair(SegmentKeyPtr v=nullptr, SegmentKeyPtr h=nullptr) : visiblePtr(v), hiddenPtr(h) { }
		SegmentKeyPtr visiblePtr;
		SegmentKeyPtr hiddenPtr;
	};

	typedef StyleRepository::FaceMaterialAndTextureId  FaceId;

	std::map<FaceId, SegmentPair>  m_faceSegmentsMap;
	std::map<int32_t, SegmentPair> m_lineSegmentsMap;
};
