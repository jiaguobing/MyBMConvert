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

#include <hc.h>

class StyleRepository3DF : public StyleRepository
{
public:
	StyleRepository3DF(SegmentKey const & includeSegmentKey,
					TextureWriter& textureWriter, ImageRepository& imageRepository, TextureRepository& textureRepository);

private:
	StyleRepository3DF(const StyleRepository3DF&);
	StyleRepository3DF& operator=(const StyleRepository3DF&);

	virtual void ApplyLineColor(SegmentKey & segment, SUColor const & color, bool usesOpacity);
	virtual void ApplyFaceColor(SegmentKey & segment, bool isFront, SUColor const & color, bool usesOpacity);
	virtual void ApplyFaceTexture(SegmentKey & segment, bool isFront, std::string const & textureName);
	virtual bool ImportTexture(int32_t textureId, std::size_t parameterOffset, std::string & out_textureName);

private:
	static const std::size_t s_parameterOffset = 3;

};

bool operator==(const StyleRepository::FaceMaterialAndTextureId& op1,
				const StyleRepository::FaceMaterialAndTextureId& op2);
bool operator<(const StyleRepository::FaceMaterialAndTextureId& op1,
				const StyleRepository::FaceMaterialAndTextureId& op2);