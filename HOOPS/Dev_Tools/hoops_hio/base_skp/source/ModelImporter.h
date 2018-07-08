// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#pragma once

#include <slapi/model/model.h>
#include <slapi/model/scene.h>

#include <memory>
#include <functional>

#include "InheritanceManager.h"
#include "SegmentKey.h"
#include "ImageRepository.h"
#include "StyleRepository.h"


class TextureWriter;
class ImageRepository;
class StyleRepository;
class VisibilityChecker;
class GeometryImporter;
class DefinitionRepository;
class GroupRepository;
class DefinitionInstancesImporter;

class ModelImporter
{
public:
	static bool LoadModelFromFile(const char* fileName, SUModelRef& model);

public:
	virtual ~ModelImporter() {}

protected:
	ModelImporter(bool ignoreOffEntities, bool importMetadata);

	bool Import(SegmentKey & sceneSegmentKey, SegmentKey & modelSegmentKey, SegmentKey & includeSegmentKey,
		SUModelRef& model, std::function<void(const std::string&, float)>& progressReporter);

	virtual ImageRepository *		CreateImageRepository(TextureWriter& textureWriter) = 0;
	virtual TextureRepository *		CreateTextureRepository() = 0;
	virtual StyleRepository *		CreateStyleRepository(SegmentKey & includeSegmentKey, TextureWriter& textureWriter, ImageRepository& imageRepository, TextureRepository& textureRepository) = 0;
	virtual GeometryImporter *		CreateGeometryImporter(StyleRepository& styleRepository, VisibilityChecker& visibilityChecker, InheritanceManager& inheritanceManager) = 0;
	virtual GroupRepository *		CreateGroupRepository(SegmentKey & includeSegmentKey, GeometryImporter& geometryImporter, VisibilityChecker& visibilityChecker, ImageRepository& imageRepository, StyleRepository& textureRepository) = 0;
	virtual DefinitionRepository *	CreateDefinitionRepository(SegmentKey & includeSegmentKey, GeometryImporter& geometryImporter, GroupRepository& groupRepository, InheritanceManager& inheritanceManager) = 0;
	virtual DefinitionInstancesImporter *	CreateDefinitionInstancesImporter(DefinitionRepository& definitionRepository, VisibilityChecker& visibilityChecker) = 0;

	virtual bool SetTopLevelAttributes(SegmentKey & modelSegmentKey, SUModelRef& model) = 0;
	virtual bool SetModelCamera(SegmentKey & sceneSegmentKey, SUPoint3D const & position, SUPoint3D const & target, SUVector3D const & up, bool isPerspective, double width, double height) = 0;
	virtual bool SetSceneCamera(std::string const & sceneName, SUPoint3D const & position, SUPoint3D const & target, SUVector3D const & up, bool isPerspective, double width, double height) = 0;
	virtual bool SetModelOptions(SegmentKey & modelSegmentKey, std::string const & data) = 0;
	virtual bool SetModelMetadata(SegmentKey & modelSegmentKey, std::string const & data) = 0;


	bool GetSKPUnitOptions(SUOptionsProviderRef& optionsProvider, std::stringstream& options);
	bool GetSKPNamedOptions(SUOptionsProviderRef& optionsProvider, std::stringstream& options);
	bool ImportAttributeDictionary(SUAttributeDictionaryRef& dictionary, std::stringstream& metadata);

private:
	ModelImporter(const ModelImporter&);
	ModelImporter& operator=(const ModelImporter&);

	bool GetLengthUnit(SUOptionsProviderRef& optionsProvider, std::stringstream& options);
	bool GetLengthFormat(SUOptionsProviderRef& optionsProvider, std::stringstream& options);
	std::string TypedValueToString(SUTypedValueRef& value);

	void Reset(SegmentKey & includeSegmentKey, SUModelRef& model);

	bool ImportDefinitions(SUModelRef& model);
	bool ImportGroups(SegmentKey & modelSegmentKey, SUModelRef& model);
	bool ImportGroupDefinitions(SegmentKey & modelSegmentKey, SUModelRef& model);
	bool ImportGeometryAndInstances(SegmentKey & modelSegmentKey, SUModelRef& model);
	bool ImportInstancesContainedInAllGroups(SUModelRef& model);
	bool ImportInstancesContainedInDefinition(SUComponentDefinitionRef& definition);
	bool ImportModelCamera(SegmentKey & sceneSegmentKey, SUModelRef& model);
	bool ImportSceneCameras(SUModelRef& model);
	bool ImportSceneCamera(SUSceneRef& scene);
	bool ImportCameraData(SUCameraRef & camera, SUPoint3D & out_position, SUPoint3D & out_target, SUVector3D & out_up, bool & out_isPerspective, double & out_width, double & out_height);
	bool ImportModelOptions(SegmentKey & modelKey, SUModelRef& model);
	bool ImportModelMetadata(SegmentKey & modelKey, SUModelRef& model);

	void SetProgressCallback(std::function<void(const std::string&, float)> * in_progressReporter) { progress_callback = in_progressReporter; }

protected:
	// Using std::shared_ptr because it allows incomplete types,
	// not because these are intended to be shared externally.
	std::shared_ptr<TextureWriter>        m_textureWriterPtr;
	std::shared_ptr<ImageRepository>      m_imageRepositoryPtr;
	std::shared_ptr<TextureRepository>    m_textureRepositoryPtr;
	std::shared_ptr<StyleRepository>      m_styleRepositoryPtr;
	std::shared_ptr<VisibilityChecker>    m_visibilityCheckerPtr;
	std::shared_ptr<GeometryImporter>     m_geometryImporterPtr;
	std::shared_ptr<GroupRepository>      m_groupRepositoryPtr;
	std::shared_ptr<DefinitionRepository> m_definitionRepositoryPtr;

	InheritanceManager						m_inheritanceManager;
	bool									m_ignoreOffEntities;
	bool									m_importMetadata;

	std::function<void(const std::string&, float)> * progress_callback;
};
