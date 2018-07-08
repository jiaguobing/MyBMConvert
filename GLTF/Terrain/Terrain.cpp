#define  _CRT_SECURE_NO_WARNINGS
#include "Terrain.h"
#include "../rapidjson/stringbuffer.h"
#include "../rapidjson/writer.h"
//
#include "gdal_priv.h"
const double PI = 3.1415926535897932384626433832795;
const int HEADER_LENGTH = 12;
const int CHUNK_HEADER_LENGTH = 8;
const int DEM_CHUNK_SIZE = 32;
//
#include <iomanip>
TerrainNode::~TerrainNode()
{
	for (size_t i = 0; i < m_vChildren.size();++i)
	{
		delete m_vChildren[i];
	}
	//
	delete[] m_pHeights;
}
bool TerrainNode::Convert2B3DM(bool addOne/* = false*/)
{
	bool flag = false;
	if (addOne)
		flag = ComputeContentAddOne();
	else
		flag = ComputeContent();
	//
	if (flag)
	{
		m_ValidataContent = true;
		Convert2GLTF();
		ConvertGLTFToB3DM();
		freeMemory();
	}
	else
	{
		// 若m_ValidateDataWidth <= 1 || m_ValidateDataHeight <= 1 则将其置为0，以便构建其parentNode
		m_ValidateDataWidth = m_ValidateDataHeight = 0;
		m_ValidataContent = false;
	}
	//
	m_vIndexs.clear();
	m_vPositions.clear();
	m_vNormals.clear();
	m_vCoords.clear();
	m_vIndexs.reserve(0);
	m_vPositions.reserve(0);
	m_vNormals.reserve(0);
	m_vCoords.reserve(0);
	return flag;
}
BMVertexD TerrainNode::FromRadians(double longitude, double latitude, double height)
{
	longitude = longitude * PI / 180.0;
	latitude = latitude* PI / 180.0;
	//
	BMVertexDD _radii = BMVertexDD(6378137.0, 6378137.0, 6356752.3142451793);
	BMVertexDD _radiiSquared = BMVertexDD(_radii.m_x*_radii.m_x, _radii.m_y*_radii.m_y, _radii.m_z*_radii.m_z);
	//
	double cosLatitude = cos(latitude);
	BMVertexDD scratchN = BMVertexDD(cosLatitude* cos(longitude), cosLatitude*sin(longitude), sin(latitude)).Normalization();
	//
	BMVertexDD scratchK = BMVertexDD(scratchN.m_x*_radiiSquared.m_x, scratchN.m_y*_radiiSquared.m_y, scratchN.m_z*_radiiSquared.m_z);
	double gamma = sqrt(scratchN.Dot(scratchK));
	//
	scratchK.m_x = scratchK.m_x / gamma; scratchK.m_y = scratchK.m_y / gamma; scratchK.m_z = scratchK.m_z / gamma;
	scratchN.m_x = scratchN.m_x * height; scratchN.m_y = scratchN.m_y * height; scratchN.m_z = scratchN.m_z * height;
	//
	return BMVertexD(scratchK.m_x + scratchN.m_x, scratchK.m_y + scratchN.m_y, scratchK.m_z + scratchN.m_z);
}
bool TerrainNode::ComputeContentAddOne()
{
	if (m_ValidateDataHeight <= 1 || m_ValidateDataWidth <= 1) return false;
	//
	BMVertexD tempPoint, tempUV;
	m_vPositions.reserve((m_ValidateDataHeight + 2 + 1) * (m_ValidateDataWidth + 2 + 1));
	m_vCoords.reserve((m_ValidateDataHeight + 2 + 1) * (m_ValidateDataWidth + 2 + 1));
	//计算position and uvCoord
	//////////////////////////////////////////////////////////////////////////
	double deltaLongitude = m_RDCorner.m_x - m_LUCorner.m_x;
	double deltaLatitude = m_LUCorner.m_y - m_RDCorner.m_y;
	//裙边第一行
	tempPoint = FromRadians(m_LUCorner.m_x, m_LUCorner.m_y, m_minHeight);
	m_vPositions.push_back(tempPoint);
	tempUV.m_y = 1.0;
	m_vCoords.push_back(tempUV);
	for (int column = 0; column <= m_ValidateDataWidth; ++column)
	{
		tempPoint = FromRadians(m_LUCorner.m_x + column * m_GeoTransform, m_LUCorner.m_y, m_minHeight);
		tempUV.m_x = column * m_GeoTransform / deltaLongitude;
		m_vCoords.push_back(tempUV);
		m_vPositions.push_back(tempPoint);
	}
	tempPoint.m_x = m_LUCorner.m_x + m_ValidateDataWidth* m_GeoTransform;
	tempPoint.m_y = m_LUCorner.m_y;
	tempUV.m_x = (tempPoint.m_x - m_LUCorner.m_x) / deltaLongitude;
	m_vCoords.push_back(tempUV);
	//
	tempPoint = FromRadians(tempPoint.m_x, tempPoint.m_y, m_minHeight);
	m_vPositions.push_back(tempPoint);
	//
	for (int row = 0; row < m_ValidateDataHeight; ++row)
	{
		double tempLatitude = m_LUCorner.m_y - row * m_GeoTransform;
		tempPoint = FromRadians(m_LUCorner.m_x, tempLatitude, m_minHeight);
		tempUV.m_x = 0.0;
		tempUV.m_y = (tempLatitude - m_RDCorner.m_y) / deltaLatitude;
		m_vCoords.push_back(tempUV);
		m_vPositions.push_back(tempPoint);
		//
		float _tempHeight;
		for (int column = 0; column < m_ValidateDataWidth; ++column)
		{
			tempPoint.m_x = m_LUCorner.m_x + column * m_GeoTransform;
			tempPoint.m_y = tempLatitude;
			tempPoint.m_z = _tempHeight = m_pHeights[row * m_ValidateDataWidth + column];
			tempUV.m_x = (tempPoint.m_x - m_LUCorner.m_x) / deltaLongitude;
			tempPoint = FromRadians(tempPoint.m_x, tempPoint.m_y, tempPoint.m_z);
			m_vPositions.push_back(tempPoint);
			m_vCoords.push_back(tempUV);
		}
		//最后一列
		tempPoint = FromRadians(m_LUCorner.m_x + m_ValidateDataWidth* m_GeoTransform, tempLatitude, _tempHeight);
		tempUV.m_x = m_ValidateDataWidth* m_GeoTransform / deltaLongitude;
		m_vCoords.push_back(tempUV);
		m_vPositions.push_back(tempPoint);
		//裙边 列
		tempPoint = FromRadians(m_LUCorner.m_x + m_ValidateDataWidth* m_GeoTransform, tempLatitude, m_minHeight);
		tempUV.m_x = m_ValidateDataWidth* m_GeoTransform / deltaLongitude;
		m_vCoords.push_back(tempUV);
		m_vPositions.push_back(tempPoint);
	}
	//最后一行
	tempPoint.m_x = m_LUCorner.m_x;
	tempPoint.m_y = m_LUCorner.m_y - m_ValidateDataHeight * m_GeoTransform;
	tempUV.m_x = 0.0; tempUV.m_y = (tempPoint.m_y - m_RDCorner.m_y) / deltaLatitude;
	m_vCoords.push_back(tempUV);
	float _tempHeight = m_pHeights[(m_ValidateDataHeight - 1)* m_ValidateDataWidth + 0];
	tempPoint = FromRadians(tempPoint.m_x, tempPoint.m_y, _tempHeight);
	m_vPositions.push_back(tempPoint);
	//
	for (int column = 0; column < m_ValidateDataWidth; ++column)
	{
		tempPoint.m_x = m_LUCorner.m_x + column * m_GeoTransform;
		tempPoint.m_y = m_LUCorner.m_y - m_ValidateDataHeight * m_GeoTransform;
		tempUV.m_x = (tempPoint.m_x - m_LUCorner.m_x) / deltaLongitude;
		tempUV.m_y = (tempPoint.m_y - m_RDCorner.m_y) / deltaLatitude;
		m_vCoords.push_back(tempUV);
		//
		_tempHeight = m_pHeights[(m_ValidateDataHeight - 1)* m_ValidateDataWidth + column];
		tempPoint = FromRadians(tempPoint.m_x, tempPoint.m_y, _tempHeight);
		m_vPositions.push_back(tempPoint);
	}
	tempPoint.m_x = m_LUCorner.m_x + m_ValidateDataWidth* m_GeoTransform;
	tempPoint.m_y = m_LUCorner.m_y - m_ValidateDataHeight * m_GeoTransform;
	tempUV.m_x = (tempPoint.m_x - m_LUCorner.m_x) / deltaLongitude;
	tempUV.m_y = (tempPoint.m_y - m_RDCorner.m_y) / deltaLatitude;
	m_vCoords.push_back(tempUV);
	tempPoint = FromRadians(tempPoint.m_x, tempPoint.m_y, _tempHeight);
	m_vPositions.push_back(tempPoint);
	//
	tempPoint.m_x = m_LUCorner.m_x + m_ValidateDataWidth* m_GeoTransform;
	tempPoint.m_y = m_LUCorner.m_y - m_ValidateDataHeight * m_GeoTransform;
	tempUV.m_x = (tempPoint.m_x - m_LUCorner.m_x) / deltaLongitude;
	tempUV.m_y = (tempPoint.m_y - m_RDCorner.m_y) / deltaLatitude;
	m_vCoords.push_back(tempUV);
	//
	tempPoint = FromRadians(tempPoint.m_x, tempPoint.m_y, m_minHeight);
	m_vPositions.push_back(tempPoint);
	//
	//裙边最后一行
	tempPoint.m_x = m_LUCorner.m_x;
	tempPoint.m_y = m_LUCorner.m_y - m_ValidateDataHeight * m_GeoTransform;
	tempUV.m_x = 0.0;
	tempUV.m_y = (tempPoint.m_y - m_RDCorner.m_y) / deltaLatitude;
	m_vCoords.push_back(tempUV);
	//
	tempPoint = FromRadians(tempPoint.m_x, tempPoint.m_y, m_minHeight);
	m_vPositions.push_back(tempPoint);
	for (int column = 0; column <= m_ValidateDataWidth; ++column)
	{
		tempPoint.m_x = m_LUCorner.m_x + column * m_GeoTransform;
		tempPoint.m_y = m_LUCorner.m_y - m_ValidateDataHeight * m_GeoTransform;
		tempUV.m_x = (tempPoint.m_x - m_LUCorner.m_x) / deltaLongitude;
		tempUV.m_y = (tempPoint.m_y - m_RDCorner.m_y) / deltaLatitude;
		m_vCoords.push_back(tempUV);
		//
		tempPoint = FromRadians(tempPoint.m_x, tempPoint.m_y, m_minHeight);
		m_vPositions.push_back(tempPoint);
	}
	tempPoint.m_x = m_LUCorner.m_x + m_ValidateDataWidth* m_GeoTransform;
	tempPoint.m_y = m_LUCorner.m_y - m_ValidateDataHeight * m_GeoTransform;
	tempUV.m_x = (tempPoint.m_x - m_LUCorner.m_x) / deltaLongitude;
	tempUV.m_y = (tempPoint.m_y - m_RDCorner.m_y) / deltaLatitude;
	m_vCoords.push_back(tempUV);
	//
	tempPoint = FromRadians(tempPoint.m_x, tempPoint.m_y, m_minHeight);
	m_vPositions.push_back(tempPoint);
	//////////////////////////////////////////////////////////////////////////
	//计算index
	int _dataHeight = m_ValidateDataHeight + 2 + 1;
	int _dataWidth = m_ValidateDataWidth + 2 + 1;
	m_vIndexs.reserve((_dataHeight - 1) * (_dataWidth - 1) * 2 * 3);
	for (int row = 0; row < _dataHeight - 1; ++row)
	{
		for (int column = 0; column < _dataWidth - 1; ++column)
		{
			int lu = row * _dataWidth + column;
			int ru = lu + 1;
			int ld = (row + 1)* _dataWidth + column;
			int rd = ld + 1;
			m_vIndexs.push_back(ru); m_vIndexs.push_back(lu); m_vIndexs.push_back(rd);
			m_vIndexs.push_back(lu); m_vIndexs.push_back(ld); m_vIndexs.push_back(rd);
		}
	}
	//
	return true;
}
bool TerrainNode::ComputeContent()
{
	if (m_ValidateDataHeight <= 1 || m_ValidateDataWidth <= 1) return false;
	//
	BMVertexD tempPoint, tempUV;
	m_vPositions.reserve((m_ValidateDataHeight + 2) * (m_ValidateDataWidth + 2));
	m_vCoords.reserve((m_ValidateDataHeight + 2) * (m_ValidateDataWidth + 2));
	//计算position and uvCoord
	//////////////////////////////////////////////////////////////////////////
	double deltaLongitude = m_RDCorner.m_x - m_LUCorner.m_x;
	double deltaLatitude = m_LUCorner.m_y - m_RDCorner.m_y;
	//裙边第一行
	tempPoint = FromRadians(m_LUCorner.m_x, m_LUCorner.m_y, m_minHeight);
	m_vPositions.push_back(tempPoint);
	tempUV.m_y = 1.0;
	m_vCoords.push_back(tempUV);
	for (int column = 0; column < m_ValidateDataWidth; ++column)
	{
		tempPoint = FromRadians(m_LUCorner.m_x + column * m_GeoTransform, m_LUCorner.m_y, m_minHeight);
		tempUV.m_x = column * m_GeoTransform / deltaLongitude;
		m_vCoords.push_back(tempUV);
		m_vPositions.push_back(tempPoint);
	}
	tempPoint.m_x = m_LUCorner.m_x + (m_ValidateDataWidth - 1)* m_GeoTransform;
	tempPoint.m_y = m_LUCorner.m_y;
	tempUV.m_x = (tempPoint.m_x - m_LUCorner.m_x) / deltaLongitude;
	m_vCoords.push_back(tempUV);
	//
	tempPoint = FromRadians(tempPoint.m_x, tempPoint.m_y, m_minHeight);
	m_vPositions.push_back(tempPoint);
	//
	for (int row = 0; row < m_ValidateDataHeight;++row)
	{
		double tempLatitude = m_LUCorner.m_y - row * m_GeoTransform;
		tempPoint = FromRadians(m_LUCorner.m_x, tempLatitude, m_minHeight);
		tempUV.m_x = 0.0;
		tempUV.m_y = (tempLatitude - m_RDCorner.m_y) / deltaLatitude;
		m_vCoords.push_back(tempUV);
		m_vPositions.push_back(tempPoint);
		//
		float _tempHeight;
		for (int column = 0; column < m_ValidateDataWidth; ++column)
		{
			tempPoint.m_x = m_LUCorner.m_x + column * m_GeoTransform;
			tempPoint.m_y = tempLatitude;
			tempPoint.m_z = _tempHeight = m_pHeights[row * DEM_CHUNK_SIZE + column];
			tempUV.m_x = (tempPoint.m_x - m_LUCorner.m_x) / deltaLongitude;
			tempPoint = FromRadians(tempPoint.m_x, tempPoint.m_y, tempPoint.m_z);
			m_vPositions.push_back(tempPoint);
			m_vCoords.push_back(tempUV);
		}
		//裙边 列
		tempPoint = FromRadians(m_LUCorner.m_x + (m_ValidateDataWidth - 1)* m_GeoTransform, tempLatitude, m_minHeight);
		tempUV.m_x = (m_ValidateDataWidth - 1)* m_GeoTransform / deltaLongitude;
		m_vCoords.push_back(tempUV);
		m_vPositions.push_back(tempPoint);
	}
	//裙边最后一行
	tempPoint.m_x = m_LUCorner.m_x;
	tempPoint.m_y = m_LUCorner.m_y - (m_ValidateDataHeight - 1) * m_GeoTransform;
	tempUV.m_x = 0.0;
	tempUV.m_y = (tempPoint.m_y - m_RDCorner.m_y) / deltaLatitude;
	m_vCoords.push_back(tempUV);
	//
	tempPoint = FromRadians(tempPoint.m_x, tempPoint.m_y, m_minHeight);
	m_vPositions.push_back(tempPoint);
	for (int column = 0; column < m_ValidateDataWidth; ++column)
	{
		tempPoint.m_x = m_LUCorner.m_x + column * m_GeoTransform;
		tempPoint.m_y = m_LUCorner.m_y - (m_ValidateDataHeight - 1) * m_GeoTransform;
		tempUV.m_x = (tempPoint.m_x - m_LUCorner.m_x) / deltaLongitude;
		tempUV.m_y = (tempPoint.m_y - m_RDCorner.m_y) / deltaLatitude;
		m_vCoords.push_back(tempUV);
		//
		tempPoint = FromRadians(tempPoint.m_x, tempPoint.m_y, m_minHeight);
		m_vPositions.push_back(tempPoint);
	}
	tempPoint.m_x = m_LUCorner.m_x + (m_ValidateDataWidth - 1)* m_GeoTransform;
	tempPoint.m_y = m_LUCorner.m_y - (m_ValidateDataHeight - 1) * m_GeoTransform;
	tempUV.m_x = (tempPoint.m_x - m_LUCorner.m_x) / deltaLongitude;
	tempUV.m_y = (tempPoint.m_y - m_RDCorner.m_y) / deltaLatitude;
	m_vCoords.push_back(tempUV);
	//
	tempPoint = FromRadians(tempPoint.m_x, tempPoint.m_y, m_minHeight);
	m_vPositions.push_back(tempPoint);
	//////////////////////////////////////////////////////////////////////////
	//计算index
	int _dataHeight = m_ValidateDataHeight + 2 ;
	int _dataWidth = m_ValidateDataWidth + 2 ;
	m_vIndexs.reserve((_dataHeight - 1) * (_dataWidth - 1) * 2 * 3);
	for (int row = 0; row < _dataHeight - 1; ++row)
	{
		for (int column = 0; column < _dataWidth - 1; ++column)
		{
			int lu = row * _dataWidth + column;
			int ru = lu + 1;
			int ld = (row + 1)* _dataWidth + column;
			int rd = ld + 1;
			m_vIndexs.push_back(ru); m_vIndexs.push_back(lu); m_vIndexs.push_back(rd);
			m_vIndexs.push_back(lu); m_vIndexs.push_back(ld); m_vIndexs.push_back(rd);
		}
	}
	//
	return true;
}
void TerrainNode::Convert2GLTF()
{
	if (m_pAsset != nullptr) return;
	//
	BMVertexD _tempTransform = m_vPositions[0];
	m_pAsset = new Asset();
	//
	GLTF::Scene* _pScene = new GLTF::Scene();
	m_pAsset->scenes.push_back(_pScene);
	m_pAsset->scene = 0;
	//
	GLTF::Node* _pNode = new GLTF::Node();
	GLTF::Node::TransformMatrix* pTrans = new GLTF::Node::TransformMatrix();
	pTrans->matrix[12] = _tempTransform.m_x; pTrans->matrix[13] = _tempTransform.m_y; pTrans->matrix[14] = _tempTransform.m_z;
	_pNode->transform = pTrans;
	
	_pScene->nodes.push_back(_pNode);
	//
	GLTF::Mesh* _pMesh = new GLTF::Mesh();
	_pNode->mesh = _pMesh;
	_pMesh->name = m_NodeName;
	_pMesh->stringId = _pMesh->name;
	//////////////////////////////////////////////////////////////////////////
	GLTF::Image* pImage = GLTF::Image::load(m_ImagePath);
	std::string::size_type sfind = pImage->uri.rfind(L'/');
	pImage->uri = pImage->uri.substr(sfind + 1);
	//
	GLTF::Sampler* _pSampler = new GLTF::Sampler();
	_pSampler->wrapS = GLTF::Constants::WebGL::CLAMP_TO_EDGE;
	_pSampler->wrapT = GLTF::Constants::WebGL::CLAMP_TO_EDGE;
	GLTF::Texture* pTex = new GLTF::Texture();
	pTex->source = pImage;
	pTex->sampler = _pSampler;
	m_pAsset->globalSampler = _pSampler;
	//////////////////////////////////////////////////////////////////////////
	GLTF::Primitive* primitive = new GLTF::Primitive();
	primitive->mode = GLTF::Primitive::Mode::TRIANGLES;
	//
	std::vector<float> attributeData;
	size_t numOfPoint = m_vPositions.size();
	attributeData.reserve(numOfPoint * 3);
	//
	for (size_t i = 0; i < numOfPoint; ++i)
	{
		BMVertexD& tempPt = m_vPositions[i];
		attributeData.push_back(tempPt.m_x - _tempTransform.m_x);
		attributeData.push_back(tempPt.m_y - _tempTransform.m_y);
		attributeData.push_back(tempPt.m_z - _tempTransform.m_z);
	}
	GLTF::Accessor* _AccPosition = new GLTF::Accessor(GLTF::Accessor::Type::VEC3, GLTF::Constants::WebGL::FLOAT, (unsigned char*)&attributeData[0], attributeData.size() / 3, GLTF::Constants::WebGL::ARRAY_BUFFER);
	//
	/*attributeData.clear();
	for (size_t i = 0; i < numOfPoint; ++i)
	{
		BMVertexD& tempPt = m_vNormals[i];
		attributeData.push_back(tempPt.m_x);
		attributeData.push_back(tempPt.m_y);
		attributeData.push_back(tempPt.m_z);
	}
	GLTF::Accessor* _AccNormal = new GLTF::Accessor(GLTF::Accessor::Type::VEC3, GLTF::Constants::WebGL::FLOAT, (unsigned char*)&attributeData[0], attributeData.size() / 3, GLTF::Constants::WebGL::ARRAY_BUFFER);*/
	//
	attributeData.clear();
	for (size_t i = 0; i < numOfPoint; ++i)
	{
		BMVertexD& tempPt = m_vCoords[i];
		attributeData.push_back(tempPt.m_x);
		attributeData.push_back(1 - tempPt.m_y);
	}
	GLTF::Accessor* _AccCoor = new GLTF::Accessor(GLTF::Accessor::Type::VEC2, GLTF::Constants::WebGL::FLOAT, (unsigned char*)&attributeData[0], attributeData.size() / 2, GLTF::Constants::WebGL::ARRAY_BUFFER);
	//
	std::vector<unsigned short> unsignedShortIndexData; unsignedShortIndexData.reserve(numOfPoint);
	for (size_t i = 0; i < numOfPoint; ++i)
	{
		unsignedShortIndexData.push_back(0);
	}
	GLTF::Accessor* _AccBatchID = new GLTF::Accessor(Accessor::Type::SCALAR, GLTF::Constants::WebGL::UNSIGNED_SHORT, (unsigned char*)&unsignedShortIndexData[0], unsignedShortIndexData.size(), GLTF::Constants::WebGL::ARRAY_BUFFER);
	//
	GLTF::Accessor* _AccIndex = nullptr;
	unsignedShortIndexData.clear();
	size_t numOfIndex = m_vIndexs.size();
	unsignedShortIndexData.reserve(numOfIndex);
	for (size_t i = 0; i < numOfIndex; ++i)
	{
		unsignedShortIndexData.push_back((unsigned short)m_vIndexs[i]);
	}
	_AccIndex = new GLTF::Accessor(Accessor::Type::SCALAR, GLTF::Constants::WebGL::UNSIGNED_SHORT, (unsigned char*)&unsignedShortIndexData[0], unsignedShortIndexData.size(), GLTF::Constants::WebGL::ELEMENT_ARRAY_BUFFER);
	//
	primitive->attributes["POSITION"] = _AccPosition;
	//primitive->attributes["NORMAL"] = _AccNormal;
	primitive->attributes["TEXCOORD_0"] = _AccCoor;
	primitive->attributes["_BATCHID"] = _AccBatchID;
	primitive->indices = _AccIndex;
	//
	GLTF::MaterialPBR* material = new GLTF::MaterialPBR();
	material->metallicRoughness->metallicFactor = 0.0f;
	material->metallicRoughness->roughnessFactor = -1.0f;
	material->emissiveFactor = new float[4]{0.0, 0.0, 0.0, 0.0};
	material->alphaMode = "OPAQUE";
	material->doubleSided = false;
	GLTF::MaterialPBR::Texture* texture = new GLTF::MaterialPBR::Texture();
	texture->texture = pTex;
	material->metallicRoughness->baseColorTexture = texture;
	//
	primitive->material = material;
	_pMesh->primitives.push_back(primitive);
	//
	writeAssetToFile();
}
bool TerrainNode::writeAssetToFile()
{
	Options* options = new Options();
	options->binary = true;
	options->embeddedTextures = m_EmbeddedTextures;
	//
	GLTF::Buffer* buffer = m_pAsset->packAccessors();
	// Create image bufferViews for binary glTF
	if (options->binary && options->embeddedTextures) {
		size_t imageBufferLength = 0;
		std::vector<GLTF::Image*> images = m_pAsset->getAllImages();
		for (GLTF::Image* image : images) {
			imageBufferLength += image->byteLength;
		}
		unsigned char* bufferData = buffer->data;
		bufferData = (unsigned char*)realloc(bufferData, buffer->byteLength + imageBufferLength);
		size_t byteOffset = buffer->byteLength;
		for (GLTF::Image* image : images) {
			GLTF::BufferView* bufferView = new GLTF::BufferView(byteOffset, image->byteLength, buffer);
			image->bufferView = bufferView;
			std::memcpy(bufferData + byteOffset, image->data, image->byteLength);
			byteOffset += image->byteLength;
		}
		buffer->data = bufferData;
		buffer->byteLength += imageBufferLength;
	}
	//////////////////////////////////////////////////////////////////////////
	rapidjson::StringBuffer s;
	rapidjson::Writer<rapidjson::StringBuffer> jsonWriter = rapidjson::Writer<rapidjson::StringBuffer>(s);
	jsonWriter.StartObject();
	m_pAsset->writeJSON(&jsonWriter, options);
	jsonWriter.EndObject();
	//
	m_strGLTFFilePath = m_TileSetFileFolder +"/" + m_NodeName + ".glb";
	//
	std::string jsonString = s.GetString();
	FILE* file = fopen(m_strGLTFFilePath.c_str(), "wb");
	if (file != NULL) {
		fwrite("glTF", sizeof(char), 4, file); // magic
		uint32_t* writeHeader = new uint32_t[2];
		// version
		if (options->version == "1.0") {
			writeHeader[0] = 1;
		}
		else {
			writeHeader[0] = 2;
		}

		int jsonPadding = (4 - (jsonString.length() & 3)) & 3;
		int binPadding = (4 - (buffer->byteLength & 3)) & 3;

		writeHeader[1] = HEADER_LENGTH + (CHUNK_HEADER_LENGTH + jsonString.length() + jsonPadding + buffer->byteLength + binPadding); // length
		if (options->version != "1.0") {
			writeHeader[1] += CHUNK_HEADER_LENGTH;
		}
		fwrite(writeHeader, sizeof(uint32_t), 2, file); // GLB header

		writeHeader[0] = jsonString.length() + jsonPadding; // 2.0 - chunkLength / 1.0 - contentLength
		if (options->version == "1.0") {
			writeHeader[1] = 0; // 1.0 - contentFormat
		}
		else {
			writeHeader[1] = 0x4E4F534A; // 2.0 - chunkType JSON
		}
		fwrite(writeHeader, sizeof(uint32_t), 2, file);
		fwrite(jsonString.c_str(), sizeof(char), jsonString.length(), file);
		for (int i = 0; i < jsonPadding; i++) {
			fwrite(" ", sizeof(char), 1, file);
		}
		if (options->version != "1.0") {
			writeHeader[0] = buffer->byteLength + binPadding; // chunkLength
			writeHeader[1] = 0x004E4942; // chunkType BIN
			fwrite(writeHeader, sizeof(uint32_t), 2, file);
		}
		fwrite(buffer->data, sizeof(unsigned char), buffer->byteLength, file);
		for (int i = 0; i < binPadding; i++) {
			fwrite("\0", sizeof(char), 1, file);
		}
		fclose(file);
		//
		delete writeHeader;
		delete options;
		return true;
	}
	else
	{
		delete options;
		return true;
	}
}
void TerrainNode::freeMemory()
{
	if (m_pAsset != nullptr)
	{
		std::vector<GLTF::Texture*> _allTexture = m_pAsset->getAllTextures();
		std::vector<GLTF::Image*> _allImage = m_pAsset->getAllImages();
		std::vector<GLTF::Material*> _allMater = m_pAsset->getAllMaterials();
		for (auto nn : _allImage)
		{
			delete nn;
		}
		for (auto nn : _allTexture)
		{
			delete nn;
		}
		for (auto nn : _allMater)
		{
			delete nn;
		}
		std::vector<GLTF::Accessor*> _allAcc = m_pAsset->getAllAccessors();
		std::set<GLTF::BufferView*> _allBuffViews;
		std::set<GLTF::Buffer*> _allBuffs;
		std::set<unsigned char*> _allDatas;
		for (auto nn : _allAcc)
		{
			_allBuffViews.insert(nn->bufferView);
			delete nn;
		}
		for (auto nn : _allBuffViews)
		{
			_allBuffs.insert(nn->buffer);
			delete nn;
		}
		for (auto nn : _allBuffs)
		{
			_allDatas.insert(nn->data);
			delete nn;
		}
		for (auto nn : _allDatas)
		{
			delete nn;
		}
		//
		delete m_pAsset;
	}
}
bool TerrainNode::ConvertGLTFToB3DM()
{
	int _batchNum = 1;
	rapidjson::StringBuffer featureTableJson;
	rapidjson::Writer<rapidjson::StringBuffer> featureTtabelJsonWriter = rapidjson::Writer<rapidjson::StringBuffer>(featureTableJson);
	featureTtabelJsonWriter.StartObject();
	featureTtabelJsonWriter.Key("BATCH_LENGTH");
	featureTtabelJsonWriter.Int(_batchNum);
	featureTtabelJsonWriter.EndObject();
	std::string featureTableJsonString = featureTableJson.GetString();
	int featureTableJsonPadding = (4 - (featureTableJsonString.length() & 3)) & 3;
	//
	rapidjson::StringBuffer batchTableJson;
	rapidjson::Writer<rapidjson::StringBuffer> batchTabelJsonWriter = rapidjson::Writer<rapidjson::StringBuffer>(batchTableJson);
	batchTabelJsonWriter.StartObject();
	batchTabelJsonWriter.Key("batchId");
	batchTabelJsonWriter.StartArray();
	for (int i = 0; i < _batchNum; ++i)
		batchTabelJsonWriter.Int(i);
	batchTabelJsonWriter.EndArray();
	//
	batchTabelJsonWriter.Key("name");
	batchTabelJsonWriter.StartArray();
	for (int i = 0; i < _batchNum; ++i)
		batchTabelJsonWriter.String("terrain");
	batchTabelJsonWriter.EndArray();
	batchTabelJsonWriter.EndObject();
	std::string batchTabelJsonString = batchTableJson.GetString();
	int  batchTableJsonPadding = (4 - (batchTabelJsonString.length() & 3)) & 3;
	//////////////////////////////////////////////////////////////////////////
	long glbByteLen;
	unsigned char* glbBuffer;
	FILE* _glbFile = fopen(m_strGLTFFilePath.c_str(), "rb");
	if (_glbFile == NULL) {
		return false;
	}
	else {
		fseek(_glbFile, 0, SEEK_END);
		glbByteLen = ftell(_glbFile);
		fclose(_glbFile);
		_glbFile = fopen(m_strGLTFFilePath.c_str(), "rb");
		glbBuffer = (unsigned char*)malloc(glbByteLen);
		int bytesRead = fread(glbBuffer, sizeof(unsigned char), glbByteLen, _glbFile);
		fclose(_glbFile);
	}
	//////////////////////////////////////////////////////////////////////////
	unsigned int b3dmFileLen = 28 + featureTableJsonString.length() + featureTableJsonPadding + batchTabelJsonString.length() + batchTableJsonPadding + glbByteLen;
	//////////////////////////////////////////////////////////////////////////
	m_strB3DMFilePath =  m_TileSetFileFolder + "/" + m_NodeName + ".b3dm";
	FILE* file = fopen(m_strB3DMFilePath.c_str(), "wb");
	if (file != NULL)
	{
		// b3dm
		fwrite("b3dm", sizeof(char), 4, file);
		uint32_t* writeHeader = new uint32_t[6];
		// version
		writeHeader[0] = 1;
		//byteLength
		writeHeader[1] = b3dmFileLen;
		//featureTableJSONByteLength
		writeHeader[2] = featureTableJsonString.length() + featureTableJsonPadding;
		//featureTableBinaryByteLength
		writeHeader[3] = 0;
		//batchTableJSONByteLength
		writeHeader[4] = batchTabelJsonString.length() + batchTableJsonPadding;
		//batchTableBinaryByteLength
		writeHeader[5] = 0;
		//b3dm header
		fwrite(writeHeader, sizeof(uint32_t), 6, file);
		//featureTable
		fwrite(featureTableJsonString.c_str(), sizeof(char), featureTableJsonString.length(), file);
		for (int i = 0; i < featureTableJsonPadding; i++) {
			fwrite(" ", sizeof(char), 1, file);
		}
		//batchTable
		fwrite(batchTabelJsonString.c_str(), sizeof(char), batchTabelJsonString.length(), file);
		for (int i = 0; i < batchTableJsonPadding; i++) {
			fwrite(" ", sizeof(char), 1, file);
		}
		//glb
		fwrite(glbBuffer, sizeof(unsigned char), glbByteLen, file);
		fclose(file);
		//
		delete[] writeHeader;
		free(glbBuffer);
		return true;
	}
	else
	{
		free(glbBuffer);
		return false;
	}
}
double TerrainNode::ComputeGeometricError()
{
	BMVertexD luPt = FromRadians(m_LUCorner.m_x, m_LUCorner.m_y, m_LUCorner.m_z);
	BMVertexD rdPt = FromRadians(m_RDCorner.m_x, m_RDCorner.m_y, m_RDCorner.m_z);
	return (luPt - rdPt).GetLength();
}
void TerrainNode::ComputeRegion(double region[6])
{
	double w = m_LUCorner.m_x * PI / 180.0;
	double s = m_RDCorner.m_y * PI / 180.0;
	double e = m_RDCorner.m_x * PI / 180.0;
	double n = m_LUCorner.m_y * PI / 180.0;
	//
	region[0] = w;
	region[1] = s;
	region[2] = e;
	region[3] = n;
	region[4] = m_minHeight;
	region[5] = m_maxHeight;
}
void TerrainNode::writeJSON(void* writer, bool inOBJ /*= false*/)
{
	rapidjson::Writer<rapidjson::StringBuffer>* jsonWriter = (rapidjson::Writer<rapidjson::StringBuffer>*)writer;
	if (inOBJ)
	{
		jsonWriter->StartObject();
	}
	//
	double boundingRegion[6];
	ComputeRegion(boundingRegion);
	//boundingVolume
	jsonWriter->Key("boundingVolume");
	jsonWriter->StartObject();
	jsonWriter->Key("region");
	jsonWriter->StartArray();
	for (double tt : boundingRegion)
	{
		jsonWriter->Double(tt);
	}
	jsonWriter->EndArray();
	jsonWriter->EndObject();
	//geometricError
	jsonWriter->Key("geometricError");
	double GR = ComputeGeometricError() /2.0;
	if (m_vChildren.size() == 0)
		GR = 0.0;//叶子节点 geometricError = 0.0
	jsonWriter->Double(GR);
	//refine
	jsonWriter->Key("refine");
	jsonWriter->String("REPLACE");
	//
	if (m_ValidataContent)
	{
		//content
		{
			jsonWriter->Key("content");
			jsonWriter->StartObject();
			//
			std::string url = m_NodeName + ".b3dm";
			jsonWriter->Key("url");
			jsonWriter->String(url.c_str());
			//
			jsonWriter->EndObject();
		}
	}
	//children
	if (m_vChildren.size() != 0)
	{
		jsonWriter->Key("children");
		jsonWriter->StartArray();
		//
		for (size_t i = 0; i < m_vChildren.size();++i)
		{
			TerrainNode* pNode = m_vChildren[i];
			pNode->writeJSON(writer, true);
		}
		//
		jsonWriter->EndArray();
	}
	//
	if (inOBJ)
	{
		jsonWriter->EndObject();
	}
}
//////////////////////////////////////////////////////////////////////////
Terrain23DTile::Terrain23DTile()
{
}

Terrain23DTile::~Terrain23DTile()
{
	delete m_pRoot;
}
bool Terrain23DTile::Convert()
{
	GDALAllRegister();  
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO"); 
	//
	BMVertexD demLUPoint, demRDPoint; double demGeoTransform[6]; int demWidth, demHeight;
	BMVertexD imageLUPoint, imageRDPoint; double imageGeoTransform[6]; int imageWidth, imageHeight;
	GetBound(demLUPoint, demRDPoint, m_DemFile, demWidth, demHeight, demGeoTransform);
	GetBound(imageLUPoint, imageRDPoint, m_ImageFile, imageWidth, imageHeight,imageGeoTransform);
	//////////////////////////////////////////////////////////////////////////
	int demBeginColumn,demBeginRow,demEndColumn,demEndRow;
	int imageBeginColumn, imageBeginRow,imageEndColumn,imageEndRow;
#pragma region MyRegion0
	if (demLUPoint.m_x >= imageLUPoint.m_x)
	{
		demBeginColumn = 0;
		imageBeginColumn = (demLUPoint.m_x - imageLUPoint.m_x) / imageGeoTransform[1];
	}
	else
	{
		demBeginColumn = (imageLUPoint.m_x - demLUPoint.m_x) / demGeoTransform[1] + 1;
		double temp = demLUPoint.m_x + demBeginColumn * demGeoTransform[1];
		imageBeginColumn = (temp - imageLUPoint.m_x) / imageGeoTransform[1];
	}
	//
	if (demLUPoint.m_y <= imageLUPoint.m_y)
	{
		demBeginRow = 0;
		imageBeginRow = (imageLUPoint.m_y - demLUPoint.m_y) / imageGeoTransform[1];
	}
	else
	{
		demBeginRow = (demLUPoint.m_y - imageLUPoint.m_y) / demGeoTransform[1] + 1;
		double temp = demLUPoint.m_y - demBeginRow * demGeoTransform[1];
		imageBeginRow = (imageLUPoint.m_y - temp) / imageGeoTransform[1];
	}
	//
	if (demRDPoint.m_x <= imageRDPoint.m_x)
	{
		demEndColumn = demWidth;
		imageEndColumn = imageWidth - (imageRDPoint.m_x - demRDPoint.m_x) / imageGeoTransform[1];
	}
	else
	{
		demEndColumn = demWidth - (demRDPoint.m_x - imageRDPoint.m_x) / demGeoTransform[1] - 1;
		double temp = demLUPoint.m_x + demEndColumn * demGeoTransform[1];
		imageEndColumn = (temp - imageLUPoint.m_x) / imageGeoTransform[1];
	}
	//
	if (demRDPoint.m_y >= imageRDPoint.m_y)
	{
		demEndRow = demHeight;
		imageEndRow = imageHeight - (demRDPoint.m_y - imageRDPoint.m_y) / imageGeoTransform[1];
	}
	else
	{
		demEndRow = demHeight - (imageRDPoint.m_y - demRDPoint.m_y) / demGeoTransform[1] - 1;
		double temp = demLUPoint.m_y - demEndRow * demGeoTransform[1];
		imageEndRow = (imageLUPoint.m_y - temp) / imageGeoTransform[1];
	}
	//
	if (demBeginColumn >= demWidth || demEndColumn <= 0 || demBeginRow >= demHeight || demEndRow <= 0 ||
		imageBeginColumn >= imageWidth || imageEndColumn <= 0 || imageBeginRow >= imageHeight || imageEndRow <= 0)
	{
		return false;
	}
#pragma endregion
	//////////////////////////////////////////////////////////////////////////
	int IMAGE_CHUNK_SIZE = int(double(DEM_CHUNK_SIZE) * demGeoTransform[1] / imageGeoTransform[1]);
	int RealImageSize = GetImageChunkSize(IMAGE_CHUNK_SIZE);
	int OneDemPixInImage = int(demGeoTransform[1] / imageGeoTransform[1]);
	//
#pragma region MyRegion1
	GDALDataset * pDemDataSet = (GDALDataset*)GDALOpen(m_DemFile.c_str(), GA_ReadOnly);
	GDALDataset * pImageDataSet = (GDALDataset*)GDALOpen(m_ImageFile.c_str(), GA_ReadOnly);
	GDALDriver *pJPGDriver = GetGDALDriverManager()->GetDriverByName("JPEG");
	GDALDriver *pMEMDriver = GetGDALDriverManager()->GetDriverByName("MEM");
	if (pDemDataSet == nullptr || pImageDataSet == nullptr || pJPGDriver == nullptr || pMEMDriver == nullptr) return false;
	GDALRasterBand* pDemBand = pDemDataSet->GetRasterBand(1);
	double _minMax[2];
	GDALComputeRasterMinMax((GDALRasterBandH)pDemBand, true, _minMax);
	//
	int panBandMap[3] = { 1, 2, 3 };
	unsigned char *pImageBuf = new unsigned char[RealImageSize * RealImageSize * 3];
	std::vector<TerrainNode*> _vTerrainNodes;
	//
	int rowIndex = 0;
	int colIndex = 0;
	GDALDataset *poMEMDS = pMEMDriver->Create("", RealImageSize, RealImageSize, 3, GDT_Byte, NULL);
	for (int demRow = demBeginRow, imageRow = imageBeginRow; demRow < demEndRow - DEM_CHUNK_SIZE; demRow += DEM_CHUNK_SIZE - 1, imageRow += IMAGE_CHUNK_SIZE - OneDemPixInImage, rowIndex++)
	{
		colIndex = 0;
		for (int demCol = demBeginColumn, imageCol = imageBeginColumn; demCol < demEndColumn - DEM_CHUNK_SIZE; demCol += DEM_CHUNK_SIZE - 1, imageCol += IMAGE_CHUNK_SIZE - OneDemPixInImage, colIndex++)
		{
			float* pDemBuf = new float[DEM_CHUNK_SIZE*DEM_CHUNK_SIZE];
			pDemBand->RasterIO(GF_Read, demCol, demRow, DEM_CHUNK_SIZE, DEM_CHUNK_SIZE, pDemBuf, DEM_CHUNK_SIZE, DEM_CHUNK_SIZE, GDT_Float32, 0, 0);
			TerrainNode* pNewNode = new TerrainNode;
			pNewNode->m_Column = colIndex;
			pNewNode->m_Row = rowIndex;
			pNewNode->m_Layer = 0;
			pNewNode->m_GeoTransform = demGeoTransform[1];
			pNewNode->m_ValidateDataHeight = pNewNode->m_ValidateDataWidth = DEM_CHUNK_SIZE;
			pNewNode->m_pHeights = pDemBuf;
			pNewNode->m_minHeight = _minMax[0] - 10.0;
			pNewNode->m_maxHeight = _minMax[1] + 10.0;
			pNewNode->m_LUCorner.m_x = demLUPoint.m_x + demCol * demGeoTransform[1];
			pNewNode->m_LUCorner.m_y = demLUPoint.m_y - demRow * demGeoTransform[1];
			pNewNode->m_RDCorner.m_x = demLUPoint.m_x + (demCol + DEM_CHUNK_SIZE) * demGeoTransform[1];
			pNewNode->m_RDCorner.m_y = demLUPoint.m_y - (demRow + DEM_CHUNK_SIZE) * demGeoTransform[1];
			pNewNode->m_NodeName = std::to_string(pNewNode->m_Layer) + "_" + std::to_string(pNewNode->m_Row) + "_" + std::to_string(pNewNode->m_Column);
			pNewNode->m_ImagePath = m_OutputFolder + "/" + pNewNode->m_NodeName + ".jpg";
			pNewNode->m_EmbeddedTextures = m_EmbeddedTextures;
			pNewNode->m_TileSetFileFolder = m_OutputFolder;
			//
			pImageDataSet->RasterIO(GF_Read, imageCol,imageRow, IMAGE_CHUNK_SIZE, IMAGE_CHUNK_SIZE, pImageBuf, RealImageSize, RealImageSize,
				GDT_Byte, 3, panBandMap, 3, RealImageSize * 3, 1);
			poMEMDS->RasterIO(GF_Write, 0, 0, RealImageSize, RealImageSize, pImageBuf, RealImageSize, RealImageSize, GDT_Byte, 3, panBandMap, 3, 0, 1);
			GDALDataset *poJPGDS = pJPGDriver->CreateCopy(pNewNode->m_ImagePath.c_str(), poMEMDS, FALSE, NULL, NULL, NULL);
			GDALClose(poJPGDS);
			//
			_vTerrainNodes.push_back(pNewNode);
		}
	}
	//
	GDALClose(poMEMDS);
	GDALClose(pDemDataSet);
	GDALClose(pImageDataSet);
	if (_vTerrainNodes.size() == 0)
	{
		delete[] pImageBuf;
		return false;
	}
#pragma endregion
	//////////////////////////////////////////////////////////////////////////
	//构建最精细层级   pNode->Convert2B3DM()肯定成功---因为最精细层级中 m_ValidateDataWidth == m_ValidateDataHeight == DEM_CHUNK_SIZE
	for (size_t i = 0; i < _vTerrainNodes.size(); ++i)
	{
		TerrainNode* pNode = _vTerrainNodes[i];
		pNode->Convert2B3DM();
	}
	//////////////////////////////////////////////////////////////////////////
	poMEMDS = pMEMDriver->Create("", RealImageSize, RealImageSize, 3, GDT_Byte, NULL);
	//
#pragma region MyRegion2
	int _layer = 1;
	while (_vTerrainNodes.size() > 0)
	{
		if (_vTerrainNodes.size() == 1)
		{
			m_pRoot = _vTerrainNodes[0];
			break;
		}
		//
		std::vector<TerrainNode*> _vTempNodes;
		TerrainNode* pLUNode = NULL;
		TerrainNode* pRUNode = NULL;
		TerrainNode* pLDNode = NULL;
		TerrainNode* pRDNode = NULL;
		int newRowIndex = 0;
		int newColIndex = 0;
		//
		GDALDataset * pLUImageDataSet = NULL;
		GDALDataset * pLDImageDataSet = NULL;
		GDALDataset * pRUImageDataSet = NULL;
		GDALDataset * pRDImageDataSet = NULL;
		//
		for (int row = 0; row < rowIndex; row += 2, newRowIndex++)
		{
			newColIndex = 0;
			for (int column = 0; column < colIndex; column += 2, newColIndex++)
			{
				memset(pImageBuf, 0, RealImageSize * RealImageSize * 3);
				poMEMDS->RasterIO(GF_Write, 0, 0, RealImageSize, RealImageSize, pImageBuf, RealImageSize, RealImageSize, GDT_Byte, 3, panBandMap, 3, 0, 1);
				//
				pLUNode = FindNode(_vTerrainNodes, row, column);
				pRUNode = FindNode(_vTerrainNodes, row, column + 1);
				pLDNode = FindNode(_vTerrainNodes, row + 1, column);
				pRDNode = FindNode(_vTerrainNodes, row + 1, column + 1);
				if (pLUNode == NULL) break;
				//
				TerrainNode* pNewNode = new TerrainNode;
				pNewNode->m_Column = newColIndex;
				pNewNode->m_Row = newRowIndex;
				pNewNode->m_Layer = _layer;
				pNewNode->m_NodeName = std::to_string(pNewNode->m_Layer) + "_" + std::to_string(pNewNode->m_Row) + "_" + std::to_string(pNewNode->m_Column);
				pNewNode->m_ImagePath = m_OutputFolder + "/" + pNewNode->m_NodeName + ".jpg";
				pNewNode->m_EmbeddedTextures = m_EmbeddedTextures;
				pNewNode->m_TileSetFileFolder = m_OutputFolder;
				pNewNode->m_GeoTransform = pLUNode->m_GeoTransform * 2.0;
				pNewNode->m_minHeight = pLUNode->m_minHeight;
				pNewNode->m_maxHeight = pLUNode->m_maxHeight;
				pNewNode->m_LUCorner = pLUNode->m_LUCorner;
				pNewNode->m_RDCorner.m_x = pNewNode->m_LUCorner.m_x + pow(2, _layer) * DEM_CHUNK_SIZE* demGeoTransform[1];
				pNewNode->m_RDCorner.m_y = pNewNode->m_LUCorner.m_y - pow(2, _layer) * DEM_CHUNK_SIZE* demGeoTransform[1];
				//
				float* pDemBuf;
				int _dataWidth;
				int _dataHeight;
				if (pRUNode != NULL && pRDNode != NULL)
				{
					_dataWidth = pLUNode->m_ValidateDataWidth / 2 + pRUNode->m_ValidateDataWidth / 2; 
					_dataHeight = pLUNode->m_ValidateDataHeight / 2 + pLDNode->m_ValidateDataHeight / 2; 
					pDemBuf = new float[_dataWidth*_dataHeight];
					//pLUNode
					for (int _r = 0; _r < pLUNode->m_ValidateDataHeight;_r += 2)
					{
						for (int _c = 0; _c < pLUNode->m_ValidateDataWidth; _c += 2)
						{
							float _height = pLUNode->m_pHeights[_r*pLUNode->m_ValidateDataWidth + _c];
							int newRow = _r / 2;
							int newCol = _c / 2;
							pDemBuf[newRow*_dataWidth + newCol] = _height;
						}
					}
					pLUImageDataSet = (GDALDataset*)GDALOpen(pLUNode->m_ImagePath.c_str(), GA_ReadOnly);
					pLUImageDataSet->RasterIO(GF_Read, 0, 0, RealImageSize, RealImageSize, pImageBuf, RealImageSize, RealImageSize, GDT_Byte, 3, panBandMap, 3, RealImageSize * 3, 1);
					poMEMDS->RasterIO(GF_Write, 0, 0, RealImageSize / 2, RealImageSize / 2, pImageBuf, RealImageSize, RealImageSize, GDT_Byte, 3, panBandMap, 3, 0, 1);
					GDALClose(pLUImageDataSet);
					//pRUNode
					if (pRUNode->m_ValidataContent)
					{
						for (int _r = 0; _r < pRUNode->m_ValidateDataHeight; _r += 2)
						{
							for (int _c = 0; _c < pRUNode->m_ValidateDataWidth; _c += 2)
							{
								float _height = pRUNode->m_pHeights[_r*pRUNode->m_ValidateDataWidth + _c];
								int newRow = _r / 2;
								int newCol = _c / 2 + pLUNode->m_ValidateDataWidth / 2;//此处不能写成 int newCol = （_c  + pLUNode->m_ValidateDataWidth） / 2
								pDemBuf[newRow*_dataWidth + newCol] = _height;
							}
						}
						pRUImageDataSet = (GDALDataset*)GDALOpen(pRUNode->m_ImagePath.c_str(), GA_ReadOnly);
						pRUImageDataSet->RasterIO(GF_Read, 0, 0, RealImageSize, RealImageSize, pImageBuf, RealImageSize, RealImageSize, GDT_Byte, 3, panBandMap, 3, RealImageSize * 3, 1);
						poMEMDS->RasterIO(GF_Write, RealImageSize / 2, 0, RealImageSize / 2, RealImageSize / 2, pImageBuf, RealImageSize, RealImageSize, GDT_Byte, 3, panBandMap, 3, 0, 1);
						GDALClose(pRUImageDataSet);
					}
					//pLDNode
					if (pLDNode->m_ValidataContent)
					{
						for (int _r = 0; _r < pLDNode->m_ValidateDataHeight; _r += 2)
						{
							for (int _c = 0; _c < pLDNode->m_ValidateDataWidth; _c += 2)
							{
								float _height = pLDNode->m_pHeights[_r*pLDNode->m_ValidateDataWidth + _c];
								int newRow = _r/2 + pLUNode->m_ValidateDataHeight / 2;
								int newCol = _c / 2;
								pDemBuf[newRow*_dataWidth + newCol] = _height;
							}
						}
						pLDImageDataSet = (GDALDataset*)GDALOpen(pLDNode->m_ImagePath.c_str(), GA_ReadOnly);
						pLDImageDataSet->RasterIO(GF_Read, 0, 0, RealImageSize, RealImageSize, pImageBuf, RealImageSize, RealImageSize, GDT_Byte, 3, panBandMap, 3, RealImageSize * 3, 1);
						poMEMDS->RasterIO(GF_Write, 0, RealImageSize / 2, RealImageSize / 2, RealImageSize / 2, pImageBuf, RealImageSize, RealImageSize, GDT_Byte, 3, panBandMap, 3, 0, 1);
						GDALClose(pLDImageDataSet);
					}
					//pRDNode
					if (pRDNode->m_ValidataContent)
					{
						for (int _r = 0; _r < pRDNode->m_ValidateDataHeight; _r += 2)
						{
							for (int _c = 0; _c < pRDNode->m_ValidateDataWidth; _c += 2)
							{
								float _height = pRDNode->m_pHeights[_r*pRDNode->m_ValidateDataWidth + _c];
								int newRow = _r / 2 + pLUNode->m_ValidateDataHeight / 2;
								int newCol = _c/2  + pLUNode->m_ValidateDataWidth / 2;
								pDemBuf[newRow*_dataWidth + newCol] = _height;
							}
						}
						pRDImageDataSet = (GDALDataset*)GDALOpen(pRDNode->m_ImagePath.c_str(), GA_ReadOnly);
						pRDImageDataSet->RasterIO(GF_Read, 0, 0, RealImageSize, RealImageSize, pImageBuf, RealImageSize, RealImageSize, GDT_Byte, 3, panBandMap, 3, RealImageSize * 3, 1);
						poMEMDS->RasterIO(GF_Write, RealImageSize / 2, RealImageSize / 2, RealImageSize / 2, RealImageSize / 2, pImageBuf, RealImageSize, RealImageSize, GDT_Byte, 3, panBandMap, 3, 0, 1);
						GDALClose(pRDImageDataSet);
					}
					//
					pLUNode->m_pParent = pNewNode; pNewNode->m_vChildren.push_back(pLUNode);
					pRUNode->m_pParent = pNewNode; pNewNode->m_vChildren.push_back(pRUNode);
					pLDNode->m_pParent = pNewNode; pNewNode->m_vChildren.push_back(pLDNode);
					pRDNode->m_pParent = pNewNode; pNewNode->m_vChildren.push_back(pRDNode);
					//
					GDALDataset *poJPGDS = pJPGDriver->CreateCopy(pNewNode->m_ImagePath.c_str(), poMEMDS, FALSE, NULL, NULL, NULL);
					GDALClose(poJPGDS);
				}
				else if (pRUNode != NULL && pLDNode == NULL)
				{
					_dataWidth = pLUNode->m_ValidateDataWidth / 2 + pRUNode->m_ValidateDataWidth / 2;
					_dataHeight = pLUNode->m_ValidateDataHeight / 2; 
					pDemBuf = new float[_dataWidth*_dataHeight];
					//pLUNode
					for (int _r = 0; _r < pLUNode->m_ValidateDataHeight; _r += 2)
					{
						for (int _c = 0; _c < pLUNode->m_ValidateDataWidth; _c += 2)
						{
							float _height = pLUNode->m_pHeights[_r*pLUNode->m_ValidateDataWidth + _c];
							int newRow = _r / 2;
							int newCol = _c / 2;
							pDemBuf[newRow*_dataWidth + newCol] = _height;
						}
					}
					pLUImageDataSet = (GDALDataset*)GDALOpen(pLUNode->m_ImagePath.c_str(), GA_ReadOnly);
					pLUImageDataSet->RasterIO(GF_Read, 0, 0, RealImageSize, RealImageSize, pImageBuf, RealImageSize, RealImageSize, GDT_Byte, 3, panBandMap, 3, RealImageSize * 3, 1);
					poMEMDS->RasterIO(GF_Write, 0, 0, RealImageSize / 2, RealImageSize / 2, pImageBuf, RealImageSize, RealImageSize, GDT_Byte, 3, panBandMap, 3, 0, 1);
					GDALClose(pLUImageDataSet);
					//pRUNode
					if (pRUNode->m_ValidataContent)
					{
						for (int _r = 0; _r < pRUNode->m_ValidateDataHeight; _r += 2)
						{
							for (int _c = 0; _c < pRUNode->m_ValidateDataWidth; _c += 2)
							{
								float _height = pRUNode->m_pHeights[_r*pRUNode->m_ValidateDataWidth + _c];
								int newRow = _r / 2;
								int newCol = _c / 2 + pLUNode->m_ValidateDataWidth / 2;
								pDemBuf[newRow*_dataWidth + newCol] = _height;
							}
						}
						pRUImageDataSet = (GDALDataset*)GDALOpen(pRUNode->m_ImagePath.c_str(), GA_ReadOnly);
						pRUImageDataSet->RasterIO(GF_Read, 0, 0, RealImageSize, RealImageSize, pImageBuf, RealImageSize, RealImageSize, GDT_Byte, 3, panBandMap, 3, RealImageSize * 3, 1);
						poMEMDS->RasterIO(GF_Write, RealImageSize / 2, 0, RealImageSize / 2, RealImageSize / 2, pImageBuf, RealImageSize, RealImageSize, GDT_Byte, 3, panBandMap, 3, 0, 1);
						GDALClose(pRUImageDataSet);
					}
					//
					pLUNode->m_pParent = pNewNode; pNewNode->m_vChildren.push_back(pLUNode);
					pRUNode->m_pParent = pNewNode; pNewNode->m_vChildren.push_back(pRUNode);
					//
					GDALDataset *poJPGDS = pJPGDriver->CreateCopy(pNewNode->m_ImagePath.c_str(), poMEMDS, FALSE, NULL, NULL, NULL);
					GDALClose(poJPGDS);
				}
				else if (pRUNode == NULL && pLDNode != NULL)
				{
					_dataWidth = pLUNode->m_ValidateDataWidth / 2; 
					_dataHeight = pLUNode->m_ValidateDataHeight / 2 + pLDNode->m_ValidateDataHeight / 2; 
					pDemBuf = new float[_dataWidth*_dataHeight];
					//pLUNode
					for (int _r = 0; _r < pLUNode->m_ValidateDataHeight; _r += 2)
					{
						for (int _c = 0; _c < pLUNode->m_ValidateDataWidth; _c += 2)
						{
							float _height = pLUNode->m_pHeights[_r*pLUNode->m_ValidateDataWidth + _c];
							int newRow = _r / 2;
							int newCol = _c / 2;
							pDemBuf[newRow*_dataWidth + newCol] = _height;
						}
					}
					pLUImageDataSet = (GDALDataset*)GDALOpen(pLUNode->m_ImagePath.c_str(), GA_ReadOnly);
					pLUImageDataSet->RasterIO(GF_Read, 0, 0, RealImageSize, RealImageSize, pImageBuf, RealImageSize, RealImageSize, GDT_Byte, 3, panBandMap, 3, RealImageSize * 3, 1);
					poMEMDS->RasterIO(GF_Write, 0, 0, RealImageSize / 2, RealImageSize / 2, pImageBuf, RealImageSize, RealImageSize, GDT_Byte, 3, panBandMap, 3, 0, 1);
					GDALClose(pLUImageDataSet);
					//pLDNode
					if (pLDNode->m_ValidataContent)
					{
						for (int _r = 0; _r < pLDNode->m_ValidateDataHeight; _r += 2)
						{
							for (int _c = 0; _c < pLDNode->m_ValidateDataWidth; _c += 2)
							{
								float _height = pLDNode->m_pHeights[_r*pLDNode->m_ValidateDataWidth + _c];
								int newRow = _r / 2 + pLUNode->m_ValidateDataHeight / 2;
								int newCol = _c / 2;
								pDemBuf[newRow*_dataWidth + newCol] = _height;
							}
						}
						pLDImageDataSet = (GDALDataset*)GDALOpen(pLDNode->m_ImagePath.c_str(), GA_ReadOnly);
						pLDImageDataSet->RasterIO(GF_Read, 0, 0, RealImageSize, RealImageSize, pImageBuf, RealImageSize, RealImageSize, GDT_Byte, 3, panBandMap, 3, RealImageSize * 3, 1);
						poMEMDS->RasterIO(GF_Write, 0, RealImageSize / 2, RealImageSize / 2, RealImageSize / 2, pImageBuf, RealImageSize, RealImageSize, GDT_Byte, 3, panBandMap, 3, 0, 1);
						GDALClose(pLDImageDataSet);
					}
					//
					pLUNode->m_pParent = pNewNode; pNewNode->m_vChildren.push_back(pLUNode);
					pLDNode->m_pParent = pNewNode; pNewNode->m_vChildren.push_back(pLDNode);
					//
					GDALDataset *poJPGDS = pJPGDriver->CreateCopy(pNewNode->m_ImagePath.c_str(), poMEMDS, FALSE, NULL, NULL, NULL);
					GDALClose(poJPGDS);
				}
				else if (pRUNode == NULL && pLDNode == NULL)
				{
					_dataWidth = pLUNode->m_ValidateDataWidth / 2; 
					_dataHeight = pLUNode->m_ValidateDataHeight / 2;
					pDemBuf = new float[_dataWidth*_dataHeight];
					//pLUNode
					for (int _r = 0; _r < pLUNode->m_ValidateDataHeight; _r += 2)
					{
						for (int _c = 0; _c < pLUNode->m_ValidateDataWidth; _c += 2)
						{
							float _height = pLUNode->m_pHeights[_r*pLUNode->m_ValidateDataWidth + _c];
							int newRow = _r / 2;
							int newCol = _c / 2;
							pDemBuf[newRow*_dataWidth + newCol] = _height;
						}
					}
					pLUNode->m_pParent = pNewNode; pNewNode->m_vChildren.push_back(pLUNode);
					//
					pLUImageDataSet = (GDALDataset*)GDALOpen(pLUNode->m_ImagePath.c_str(), GA_ReadOnly);
					pLUImageDataSet->RasterIO(GF_Read, 0, 0, RealImageSize, RealImageSize, pImageBuf, RealImageSize, RealImageSize, GDT_Byte, 3, panBandMap, 3, RealImageSize * 3, 1);
					poMEMDS->RasterIO(GF_Write, 0, 0, RealImageSize / 2, RealImageSize / 2, pImageBuf, RealImageSize, RealImageSize, GDT_Byte, 3, panBandMap, 3, 0, 1);
					GDALClose(pLUImageDataSet);
					//
					GDALDataset *poJPGDS = pJPGDriver->CreateCopy(pNewNode->m_ImagePath.c_str(), poMEMDS, FALSE, NULL, NULL, NULL);
					GDALClose(poJPGDS);
				}
				//
				pNewNode->m_ValidateDataHeight = _dataHeight;
				pNewNode->m_ValidateDataWidth = _dataWidth;
				pNewNode->m_pHeights = pDemBuf;
				//
				_vTempNodes.push_back(pNewNode);
			}
		}
		//////////////////////////////////////////////////////////////////////////
		for (size_t i = 0; i < _vTempNodes.size(); ++i)
		{
			TerrainNode* pNode = _vTempNodes[i];
			pNode->Convert2B3DM(true);
		}
		//
		_layer++;
		rowIndex = newRowIndex;
		colIndex = newColIndex;
		_vTerrainNodes.swap(_vTempNodes);
	}
	//
	GDALClose(poMEMDS);
	delete[] pImageBuf;
	//////////////////////////////////////////////////////////////////////////
#pragma endregion
	WriteJSON();
	return true;
}
//////////////////////////////////////////////////////////////////////////
TerrainNode* Terrain23DTile::FindNode(std::vector<TerrainNode*>& vNodes, int rowIndex, int colIndex)
{
	for (size_t i = 0; i < vNodes.size();++i)
	{
		TerrainNode* pNode = vNodes[i];
		if (pNode->m_Column == colIndex && pNode->m_Row == rowIndex)
			return pNode;
	}
	//
	return NULL;
}
bool Terrain23DTile::GetBound(BMVertexD& leftUpCorner, BMVertexD& rightDownCorner, std::string& filePath, int& width, int& height, double geoTransform[6])
{
	GDALDataset * pDataSet = (GDALDataset*)GDALOpen(filePath.c_str(), GA_ReadOnly);
	pDataSet->GetGeoTransform(geoTransform);
	//
	width = pDataSet->GetRasterXSize();
	height = pDataSet->GetRasterYSize();
	leftUpCorner.m_x = geoTransform[0];
	leftUpCorner.m_y = geoTransform[3];
	rightDownCorner.m_x = leftUpCorner.m_x + width*geoTransform[1];
	rightDownCorner.m_y = leftUpCorner.m_y + height*geoTransform[5];
	//
	GDALClose(pDataSet);
	return true;
}
int Terrain23DTile::GetImageChunkSize(int orginalSize)
{
	if (orginalSize >= 1024)
		return 1024;
	else if (orginalSize >= 512)
		return 512;
	else if (orginalSize >= 256)
		return 256;
	else if (orginalSize >= 128)
		return 128;
	else if (orginalSize >= 64)
		return 64;
	else if (orginalSize >= 32)
		return 32;
	else if (orginalSize >= 16)
		return 16;
	else
		return 8;
}
void Terrain23DTile::WriteJSON()
{
	rapidjson::StringBuffer tileSetJson;
	rapidjson::Writer<rapidjson::StringBuffer> tileSetJsonWriter = rapidjson::Writer<rapidjson::StringBuffer>(tileSetJson);
	tileSetJsonWriter.StartObject();
	{
		tileSetJsonWriter.Key("asset");
		tileSetJsonWriter.StartObject();
		//
		tileSetJsonWriter.Key("version");
		tileSetJsonWriter.String("0.0");
		tileSetJsonWriter.Key("tilesetVersion");
		tileSetJsonWriter.String("1.0.0-SCBM23DTile");
		tileSetJsonWriter.Key("gltfUpAxis");
		tileSetJsonWriter.String("Z");
		tileSetJsonWriter.Key("id");
		tileSetJsonWriter.String("terrain");
		//
		tileSetJsonWriter.EndObject();
	}
	//
	tileSetJsonWriter.Key("geometricError");
	tileSetJsonWriter.Double(m_pRoot->ComputeGeometricError() * 2);
	//
	{
		tileSetJsonWriter.Key("root");
		tileSetJsonWriter.StartObject();
		//
		m_pRoot->writeJSON(&tileSetJsonWriter);
		//
		tileSetJsonWriter.EndObject();
	}
	//
	tileSetJsonWriter.EndObject();
	//////////////////////////////////////////////////////////////////////////
	std::string tileSetJsonString = tileSetJson.GetString();
	std::string filePath = m_OutputFolder + "/tileset.json";
	FILE* file = fopen(filePath.c_str(), "wb");
	fwrite(tileSetJsonString.c_str(), sizeof(char), tileSetJsonString.length(), file);
	fclose(file);
}