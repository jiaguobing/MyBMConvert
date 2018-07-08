// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#pragma once

#include "ImageTexturedShellImporter.h"

#include <hc.h>

#include <string>

class ImageTexturedShellImporter3DF : public ImageTexturedShellImporter
{
public:
	ImageTexturedShellImporter3DF(SegmentOrganizer& segmentOrganizer, ImageRepository& imageRepository, TextureRepository& textureRepository);

private:
	ImageTexturedShellImporter3DF(const ImageTexturedShellImporter3DF&);
	ImageTexturedShellImporter3DF& operator=(const ImageTexturedShellImporter3DF&);

	virtual bool InsertQuad(SegmentKey & segment, std::vector<float> const & transform, float const points[], int const flist[], float const vertexParams[], bool isVisible);
	virtual void ApplyTexture(SegmentKey & segment, int32_t const textureId, std::string const & imageName, bool isVisible);
};