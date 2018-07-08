// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#pragma once

#include <string>
#include <map>
#include <cstdint>

#include "SegmentKey.h"

class TextureRepository
{
public:
	virtual ~TextureRepository() {}

	bool GetTexture(int32_t textureId, std::size_t parameterOffset, std::string& textureName);

protected:
	TextureRepository();
	TextureRepository(const TextureRepository&);
	TextureRepository& operator=(const TextureRepository&);

	typedef int64_t Id_t;

	Id_t GetId(int32_t textureId, std::size_t parameterOffset);

protected:
	std::map<Id_t, std::string> m_texturesMap;
};