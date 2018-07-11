#pragma once

#include <slapi/model/texture_writer.h>
#include <slapi/model/component_instance.h>
#include <slapi/model/group.h>
#include <slapi/model/image.h>
#include <slapi/model/face.h>
#include <slapi/model/edge.h>
#include <slapi/model/layer.h>
#include <slapi/model/material.h>

#include <vector>

class SKPTransform
{
public:
	SKPTransform();
	SKPTransform(double m[16]);
	~SKPTransform();

	void			SetMatrix(double m[16]);
	void			SetToIdenity();

	SUPoint3D		operator* (const SUPoint3D& pt) const;
	SUVector3D		operator* (const SUVector3D& vec) const;
	SKPTransform	operator* (const SKPTransform& t) const;

private:
	double m_t[16];
};

class InheritanceManager
{
public:
	InheritanceManager();
	virtual ~InheritanceManager();

	void			SetTextureWriter(SUTextureWriterRef textureWriter);
	void			SetMaterialsByLayer(bool materialsByLayer) { m_materialsByLayer = materialsByLayer; }

	void			PushElement(SUComponentInstanceRef pElement);
	void			PushElement(SUGroupRef pElement);
	void			PushElement(SUImageRef pElement);
	void			PushElement(SUFaceRef pElement);
	void			PushElement(SUEdgeRef pElement);
	void			PopElement();

	SULayerRef		GetCurrentLayer();
	SKPTransform	GetCurrentTransform();
	SUMaterialRef	GetCurrentFrontMaterial();
	SUMaterialRef	GetCurrentBackMaterial();
	int32_t			GetCurrentTextureHandle();

private:
	void			PushTransform(SKPTransform t);
	void			PushTextureHandle(int32_t textureHandle);

	bool						m_materialsByLayer;
	std::vector<SKPTransform>	m_transforms;
	std::vector<SULayerRef>		m_layers;
	std::vector<SUMaterialRef>	m_frontMaterials;
	std::vector<SUMaterialRef>	m_backMaterials;
	std::vector<int32_t>		m_textureHandles;
	SUTextureWriterRef			m_textureWriter;
};