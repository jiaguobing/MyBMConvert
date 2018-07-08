// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#pragma once

#include "ModelImporter.h"

#include <hc.h>

class ModelImporter3DF : public ModelImporter
{
public:
	ModelImporter3DF(bool ignoreOffEntities, bool importMetadata);

	bool Import(HC_KEY sceneSegmentKey, HC_KEY modelSegmentKey, HC_KEY includeSegmentKey, 
		SUModelRef& model, std::function<void(const std::string&, float)>& progressReporter);

private:
	ModelImporter3DF(const ModelImporter3DF&);
	ModelImporter3DF& operator=(const ModelImporter3DF&);

	virtual ImageRepository *		CreateImageRepository(TextureWriter& textureWriter);
	virtual TextureRepository *		CreateTextureRepository();
	virtual StyleRepository *		CreateStyleRepository(SegmentKey & includeSegmentKey, TextureWriter& textureWriter, ImageRepository& imageRepository, TextureRepository& textureRepository);
	virtual GeometryImporter *		CreateGeometryImporter(StyleRepository& styleRepository, VisibilityChecker& visibilityChecker, InheritanceManager& inheritanceManager);
	virtual GroupRepository *		CreateGroupRepository(SegmentKey & includeSegmentKey, GeometryImporter& geometryImporter, VisibilityChecker& visibilityChecker, ImageRepository& imageRepository, StyleRepository& textureRepository);
	virtual DefinitionRepository *	CreateDefinitionRepository(SegmentKey & includeSegmentKey, GeometryImporter& geometryImporter, GroupRepository& groupRepository, InheritanceManager& inheritanceManager);
	virtual DefinitionInstancesImporter *	CreateDefinitionInstancesImporter(DefinitionRepository& definitionRepository, VisibilityChecker& visibilityChecker);

	virtual bool SetTopLevelAttributes(SegmentKey & modelSegmentKey, SUModelRef& model);
	virtual bool SetModelOptions(SegmentKey & modelSegmentKey, std::string const & data);
	virtual bool SetModelMetadata(SegmentKey & modelSegmentKey, std::string const & data);
	virtual bool SetModelCamera(SegmentKey & sceneSegmentKey, SUPoint3D const & position, SUPoint3D const & target, SUVector3D const & up, bool isPerspective, double width, double height);
	virtual bool SetSceneCamera(std::string const & sceneName, SUPoint3D const & position, SUPoint3D const & target, SUVector3D const & up, bool isPerspective, double width, double height);

	HC_KEY						_modelSegmentKey;
	HC_KEY						_includeSegmentKey;
};
