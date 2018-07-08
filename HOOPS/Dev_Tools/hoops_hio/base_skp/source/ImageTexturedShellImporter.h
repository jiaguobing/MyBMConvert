// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#pragma once

#include "ImageRepository.h"
#include "TextureRepository.h"
#include "SegmentOrganizer.h"

#include <slapi/model/image.h>

class ImageTexturedShellImporter
{
public:
	virtual ~ImageTexturedShellImporter() {}

	bool Import(SegmentKey & segment, SUImageRef& image);

protected:
	ImageTexturedShellImporter(SegmentOrganizer& segmentOrganizer, ImageRepository& imageRepository, TextureRepository& textureRepository);
	
	virtual void ApplyTexture(SegmentKey & segment, int32_t const textureId, std::string const & imageName, bool isVisible) = 0;
	virtual bool InsertQuad(SegmentKey & segment, std::vector<float> const & transform, float const points[], int const flist[], float const vertexParams[], bool isVisible) = 0;

private:
	ImageTexturedShellImporter(const ImageTexturedShellImporter&);
	ImageTexturedShellImporter& operator=(const ImageTexturedShellImporter&);

	bool InsertQuad(SegmentKey & segment, SUImageRef& image, bool isVisible);

protected:
	SegmentOrganizer&		m_segmentOrganizer;
	ImageRepository&		m_imageRepository;
	TextureRepository&		m_textureRepository;
};