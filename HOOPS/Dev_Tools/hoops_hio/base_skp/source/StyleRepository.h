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
#include "TextureWriter.h"
#include "TextureRepository.h"

#include <slapi/model/drawing_element.h>
#include <slapi/color.h>

#include <string>
#include <cstdint>
#include <memory>

#include "SegmentKey.h"

class StyleRepository;
typedef std::shared_ptr<StyleRepository> StyleRepositoryPtr;

class StyleRepository
{
public:
	struct FaceMaterialAndTextureId {
		FaceMaterialAndTextureId()
			: frontMaterialId(0), frontTextureId(0), backMaterialId(0), backTextureId(0) { }
		FaceMaterialAndTextureId(int32_t frMatId, int32_t frTextId, int32_t bkMatId, int32_t bkTextId)
			: frontMaterialId(frMatId), frontTextureId(frTextId),
			backMaterialId(bkMatId), backTextureId(bkTextId) { }

		int32_t frontMaterialId;
		int32_t frontTextureId;
		int32_t backMaterialId;
		int32_t backTextureId;
	};

	static const FaceMaterialAndTextureId InvalidId;

public:
	virtual ~StyleRepository() {}

	SegmentKeyPtr GetStyleKey(SUFaceRef& face);
	SegmentKeyPtr GetStyleKey(SUDrawingElementRef& drawingElement);

	bool    GetId(SUFaceRef& face, FaceMaterialAndTextureId& id);
	bool    GetId(SUDrawingElementRef& drawingElement, int32_t& id);

	TextureWriter &			GetTextureWriter() { return m_textureWriter; }
	ImageRepository &		GetImageRepository() { return m_imageRepository; }
	TextureRepository &		GetTextureRepository() { return m_textureRepository; }

protected:
	StyleRepository(SegmentKey const & includeSegmentKey, TextureWriter& textureWriter, ImageRepository& imageRepository, TextureRepository& textureRepository);

	virtual void ApplyLineColor(SegmentKey & segment, SUColor const & color, bool usesOpacity) = 0;
	virtual void ApplyFaceColor(SegmentKey & segment, bool isFront, SUColor const & color, bool usesOpacity) = 0;
	virtual void ApplyFaceTexture(SegmentKey & segment, bool isFront, std::string const & textureName) = 0;
	virtual bool ImportTexture(int32_t textureId, std::size_t parameterOffset, std::string & out_textureName) = 0;

private:
	StyleRepository(const StyleRepository&);
	StyleRepository& operator=(const StyleRepository&);

	bool ImportLineColor(SegmentKey & segment, SUDrawingElementRef & drawingElement);
	bool ImportColor(SUMaterialRef & material, SUColor & color, bool & usesOpacity);
	bool ImportFaceColorAndTexture(SegmentKey & segment, SUFaceRef& face);
	bool ImportFaceColors(SegmentKey & segment, SUFaceRef& face);
	bool ImportFaceTextures(SegmentKey & segment, SUFaceRef& face);

	static const std::size_t s_parameterOffset = 3;

protected:

	SegmentKeyPtr						m_stylesSegmentKeyPtr;

	TextureWriter&						m_textureWriter;
	ImageRepository&					m_imageRepository;
	TextureRepository&					m_textureRepository;

	std::map<FaceMaterialAndTextureId, SegmentKeyPtr>		m_faceStylesMap;
	std::map<int32_t, SegmentKeyPtr>						m_lineStylesMap;
};

bool operator==(const StyleRepository::FaceMaterialAndTextureId& op1,
				const StyleRepository::FaceMaterialAndTextureId& op2);
bool operator<(const StyleRepository::FaceMaterialAndTextureId& op1,
				const StyleRepository::FaceMaterialAndTextureId& op2);