// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#pragma once

#include "GroupRepository.h"

#include <hc.h>

#include "SegmentKey3DF.h"

class GroupRepository3DF : public GroupRepository
{
public:
	GroupRepository3DF(SegmentKey const & includeSegmentKey, GeometryImporter& geometryImporter,
					VisibilityChecker& visibilityChecker, ImageRepository& imageRepository,
					StyleRepository& textureRepository);

private:
	GroupRepository3DF(const GroupRepository3DF&);
	GroupRepository3DF& operator=(const GroupRepository3DF&);

	virtual void	ApplyGroupColors(SegmentKey & segment, SUColor const & color);
	virtual void	ApplyGroupTextures(SegmentKey & segment, int32_t const groupTextureId, std::string const & imageName);

	virtual bool	InsertInstance(SegmentKey & segment, SegmentKey & includeSegment, std::vector<float> const & floatTransform, bool const isVisible);
};