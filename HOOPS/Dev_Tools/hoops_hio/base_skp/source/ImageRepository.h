// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#pragma once

#include "TextureWriter.h"

#include <slapi/model/image.h>
#include <slapi/model/face.h>

#include <map>
#include <string>
#include <cstdint>
#include <functional>

#include "SegmentKey.h"

class ImageRepository
{
public:
	virtual ~ImageRepository() {}

	bool ImportAllTextureImages();

	bool GetImageInfo(SUImageRef& image, std::string& name);
	bool GetImageInfo(int32_t textureId, std::string& name);

	TextureWriter & GetTextureWriter() const { return m_textureWriter; }

	void SetProgressCallback(std::function<void(const std::string&, float)> * in_progressReporter) { progress_callback = in_progressReporter; }

protected:
	ImageRepository(TextureWriter& textureWriter);
	ImageRepository(const ImageRepository&);
	ImageRepository& operator=(const ImageRepository&);

	virtual bool LoadTextureFile(std::string const & directory, int32_t textureId) = 0;
	
protected:
	TextureWriter& m_textureWriter;

	std::map<int32_t, std::string> m_imagesMap;
	std::function<void(const std::string&, float)> * progress_callback;
};