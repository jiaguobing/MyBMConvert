// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#pragma once

#include "TextureRepository.h"

#include <hc.h>

class TextureRepository3DF : public TextureRepository
{
public:
	TextureRepository3DF(HC_KEY texturesSegmentKey);

	bool InsertTexture(int32_t textureId, std::size_t parameterOffset, const std::string& textureSpecification, std::string& textureName);

private:
	TextureRepository3DF(const TextureRepository3DF&);
	TextureRepository3DF& operator=(const TextureRepository3DF&);

	HC_KEY			_texturesSegmentKey;
};