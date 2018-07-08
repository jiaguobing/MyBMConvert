// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#pragma once

#include "SmartSuRef.h"

#include <slapi/model/model.h>
#include <slapi/model/face.h>
#include <slapi/model/image.h>
#include <slapi/model/entities.h>

#include <string>
#include <vector>
#include <functional>

class TextureWriter
{
public:
	TextureWriter();

	bool LoadModel(SUModelRef& model);
	bool CreateMeshHelper(SUFaceRef& face, SmartMeshHelperRef& meshHelper);
	bool DumpTextureImages(const std::string& directory, std::size_t& numTextures);
	bool GetTextureIdsForFace(SUFaceRef& face, int32_t& frontTextureId, int32_t& backTextureId);
	bool GetTextureIdForImage(SUImageRef& image, int32_t& textureId);
	bool GetTextureIdForGroup(SUGroupRef& group, int32_t& textureId);
	std::string GetTextureFilePath(int32_t textureId);
	SUTextureWriterRef& GetSKPTextureWriter() { return m_textureWriter.GetReference(); }

	void SetProgressCallback(std::function<void(const std::string&, float)> * in_progressReporter) { progress_callback = in_progressReporter; }

private:
	bool ProcessEntities(SUEntitiesRef& entities);
	bool ProcessDefinitions(SUModelRef& model);
	bool ProcessGroups(SUEntitiesRef& entities);
	bool LoadFacesInEntities(SUEntitiesRef& entities);
	bool LoadImagesInEntities(SUEntitiesRef& entities);

private:
	SmartTextureWriterRef m_textureWriter;
	std::vector<decltype(SUEntitiesRef::ptr)> m_groupsProcessed;
	std::function<void(const std::string&, float)> * progress_callback;
};