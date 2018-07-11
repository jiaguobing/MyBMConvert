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

#include <HIOUtilityMagick.h>

class ImageRepository3DF : public ImageRepository
{
public:
	ImageRepository3DF(HC_KEY includeSegmentKey, TextureWriter& textureWriter);

private:
	ImageRepository3DF(const ImageRepository3DF&);
	ImageRepository3DF& operator=(const ImageRepository3DF&);

	virtual bool LoadTextureFile(std::string const & directory, int32_t textureId);

	void AssignNameToImage(HC_KEY imageKey, std::string& imageName);

	HIOUtilityMagick		_hioMagick;
	HC_KEY					_imagesSegmentKey;
};