#include "stdafx.h"
#include "BMConvert.h"
#include "../rapidjson/stringbuffer.h"
#include "../rapidjson/writer.h"
//
const int HEADER_LENGTH = 12;
const int CHUNK_HEADER_LENGTH = 8;
//
bool GLTF::BMConvert::ConvertBMMeshToGLTF()
{
	if (m_pBMMesh == nullptr ||
		m_pBMMesh->m_vPositions.size() < 3 ||
		m_pBMMesh->m_vPositions.size() != m_pBMMesh->m_vNormals.size() ||
		m_pBMMesh->m_vPositions.size() != m_pBMMesh->m_vBatchIDs.size())
	{
		return false;
	}
	//
	if (m_pAsset != nullptr) return false;
	//
	m_pAsset = new Asset();
	//
	GLTF::Scene* _pScene = new GLTF::Scene();
	m_pAsset->scenes.push_back(_pScene);
	m_pAsset->scene = 0;
	//
	GLTF::Node* _pNode = new GLTF::Node();
	_pScene->nodes.push_back(_pNode);
	//
	GLTF::Mesh* _pMesh = new GLTF::Mesh();
	_pNode->mesh = _pMesh;
	_pMesh->name = m_pBMMesh->m_strName;
	_pMesh->stringId = _pMesh->name;
	//////////////////////////////////////////////////////////////////////////
	size_t numOfPoint = m_pBMMesh->m_vPositions.size();
	std::vector<float> attributeData;
	attributeData.reserve(numOfPoint * 3);
	for (size_t i = 0; i < numOfPoint; ++i)
	{
		BMVertex& tempPt = m_pBMMesh->m_vPositions[i];
		attributeData.push_back(tempPt.m_x);
		attributeData.push_back(tempPt.m_y);
		attributeData.push_back(tempPt.m_z);
	}
	GLTF::Accessor::Type type = GLTF::Accessor::Type::VEC3;
	GLTF::Accessor* _AccPosition = new GLTF::Accessor(type, GLTF::Constants::WebGL::FLOAT, (unsigned char*)&attributeData[0], attributeData.size() / GLTF::Accessor::getNumberOfComponents(type), GLTF::Constants::WebGL::ARRAY_BUFFER);
	//
	attributeData.clear();
	for (size_t i = 0; i < numOfPoint; ++i)
	{
		BMVertex& tempPt = m_pBMMesh->m_vNormals[i];
		attributeData.push_back(tempPt.m_x);
		attributeData.push_back(tempPt.m_y);
		attributeData.push_back(tempPt.m_z);
	}
	GLTF::Accessor* _AccNormal = new GLTF::Accessor(type, GLTF::Constants::WebGL::FLOAT, (unsigned char*)&attributeData[0], attributeData.size() / GLTF::Accessor::getNumberOfComponents(type), GLTF::Constants::WebGL::ARRAY_BUFFER);
	//
	GLTF::Accessor* _AccCoor = nullptr;
	attributeData.clear();
	if (m_pBMMesh->m_vCoords.size() > 0)
	{
		for (size_t i = 0; i < numOfPoint; ++i)
		{
			BMVertex& tempPt = m_pBMMesh->m_vCoords[i];
			attributeData.push_back(tempPt.m_u);
			attributeData.push_back(1 - tempPt.m_v);
		}
		type = GLTF::Accessor::Type::VEC2;
		_AccCoor = new GLTF::Accessor(type, GLTF::Constants::WebGL::FLOAT, (unsigned char*)&attributeData[0], attributeData.size() / GLTF::Accessor::getNumberOfComponents(type), GLTF::Constants::WebGL::ARRAY_BUFFER);
	}
	//
	GLTF::Accessor* _AccBatchID = nullptr;
	std::vector<unsigned short> unsignedShortIndexData; unsignedShortIndexData.reserve(numOfPoint);
	type = Accessor::Type::SCALAR;
	if (numOfPoint < 65536)
	{
		for (size_t i = 0; i < numOfPoint; ++i)
		{
			unsignedShortIndexData.push_back((unsigned short)m_pBMMesh->m_vBatchIDs[i]);
		}
		_AccBatchID = new GLTF::Accessor(type, GLTF::Constants::WebGL::UNSIGNED_SHORT, (unsigned char*)&unsignedShortIndexData[0], unsignedShortIndexData.size(), GLTF::Constants::WebGL::ARRAY_BUFFER);
	}
	else
	{
		_AccBatchID = new GLTF::Accessor(type, GLTF::Constants::WebGL::UNSIGNED_INT, (unsigned char*)&m_pBMMesh->m_vBatchIDs[0], m_pBMMesh->m_vBatchIDs.size(), GLTF::Constants::WebGL::ARRAY_BUFFER);
	}
	//////////////////////////////////////////////////////////////////////////
	size_t numOfPrimitive = m_pBMMesh->m_vPrimitives.size();
	std::map<std::string, GLTF::Image*> _mapImages;
	std::map<std::string, GLTF::Texture*> _mapTextures;
	GLTF::Sampler* _pSampler = nullptr;
	for (size_t i = 0; i < numOfPrimitive; ++i)
	{
		BMPrimitive& tempPri = m_pBMMesh->m_vPrimitives[i];
		if (tempPri.m_TextureImagePath != "")
		{
			GLTF::Image* pImage = GLTF::Image::load(tempPri.m_TextureImagePath);
			_mapImages[tempPri.m_TextureImagePath] = pImage;
		}
	}
	if (_mapImages.size() != 0) _pSampler = new GLTF::Sampler();
	for (auto iterator = _mapImages.begin(); iterator != _mapImages.end(); iterator++)
	{
		GLTF::Texture* pTex = new GLTF::Texture();
		pTex->source = iterator->second;
		pTex->sampler = _pSampler;
		_mapTextures[iterator->first] = pTex;
	}
	m_pAsset->globalSampler = _pSampler;
	//////////////////////////////////////////////////////////////////////////
	for (size_t i = 0; i < numOfPrimitive; ++i)
	{
		BMPrimitive& tempPri = m_pBMMesh->m_vPrimitives[i];
		GLTF::Primitive* primitive = new GLTF::Primitive();
		primitive->mode = GLTF::Primitive::Mode::TRIANGLES;
		primitive->attributes["POSITION"] = _AccPosition;
		primitive->attributes["NORMAL"] = _AccNormal;
		if (tempPri.m_TextureImagePath != "" && _AccCoor != nullptr)
			primitive->attributes["TEXCOORD_0"] = _AccCoor;
		primitive->attributes["_BATCHID"] = _AccBatchID;
		//
		GLTF::Accessor* _AccIndex = nullptr;
		unsignedShortIndexData.clear();
		size_t numOfIndex = tempPri.m_vIndexs.size();
		if (numOfIndex < 65536)
		{
			for (size_t j = 0; j < numOfIndex; ++j)
			{
				unsignedShortIndexData.push_back((unsigned short)tempPri.m_vIndexs[j]);
			}
			_AccIndex = new GLTF::Accessor(type, GLTF::Constants::WebGL::UNSIGNED_SHORT, (unsigned char*)&unsignedShortIndexData[0], unsignedShortIndexData.size(), GLTF::Constants::WebGL::ELEMENT_ARRAY_BUFFER);
		}
		else
		{
			_AccIndex = new GLTF::Accessor(type, GLTF::Constants::WebGL::UNSIGNED_INT, (unsigned char*)&tempPri.m_vIndexs[0], tempPri.m_vIndexs.size(), GLTF::Constants::WebGL::ELEMENT_ARRAY_BUFFER);
		}
		//
		primitive->indices = _AccIndex;
		_pMesh->primitives.push_back(primitive);
		//
		GLTF::MaterialPBR* material = new GLTF::MaterialPBR();
		material->metallicRoughness->metallicFactor = 0.0f;
		material->metallicRoughness->roughnessFactor = 1.0f;
		material->emissiveFactor = new float[4]{0.0, 0.0, 0.0, 0.0};
		material->alphaMode = "OPAQUE";
		material->doubleSided = false;
		if (tempPri.m_TextureImagePath == "")
		{
			material->metallicRoughness->baseColorFactor = new float[4];
			material->metallicRoughness->baseColorFactor[0] = tempPri.m_Color.m_r;
			material->metallicRoughness->baseColorFactor[1] = tempPri.m_Color.m_g;
			material->metallicRoughness->baseColorFactor[2] = tempPri.m_Color.m_b;
			material->metallicRoughness->baseColorFactor[3] = tempPri.m_Color.m_a;
			if (tempPri.m_Color.m_a < 1.0f)
			{
				material->alphaMode = "BLEND";
				material->doubleSided = true;
			}

		}
		else
		{
			GLTF::MaterialPBR::Texture* texture = new GLTF::MaterialPBR::Texture();
			texture->texture = _mapTextures[tempPri.m_TextureImagePath];
			material->metallicRoughness->baseColorTexture = texture;
			if (texture->texture->source->mimeType == "image/png")
			{
				material->alphaMode = "BLEND";
				material->doubleSided = true;
			}
		}
		//
		primitive->material = material;
	}
	//
	return writeAssetToFile();
}
//
bool GLTF::BMConvert::writeAssetToFile()
{
	Options* options = new Options();
	options->binary = true;
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
	m_strGLTFFilePath = m_TileSetFileFolder + m_pBMMesh->m_strName + ".glb";
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
void GLTF::BMConvert::freeMemory()
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
//
bool GLTF::BMConvert::ConvertGLTFToB3DM()
{
	int _batchNum = int(m_pBMMesh->m_vBatchNames.size());
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
		batchTabelJsonWriter.String(m_pBMMesh->m_vBatchNames[i].c_str());
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
	m_strB3DMFilePath = m_strGLTFFilePath = m_TileSetFileFolder + m_pBMMesh->m_strName + ".b3dm";
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
bool GLTF::BMConvert::ConvertBMMeshToB3DM()
{
	if (!ConvertBMMeshToGLTF()) return false;
	return ConvertGLTFToB3DM();
}
void GLTF::BMConvert::ComputeMinMaxPt()
{
	if (m_pBMMesh == nullptr || m_pBMMesh->m_vPositions.size() < 3)
		return;
	//
	for (size_t i = 0; i < m_pBMMesh->m_vPositions.size(); ++i)
	{
		BMVertex& _vertex = m_pBMMesh->m_vPositions[i];
		m_MinPt.m_x = min(m_MinPt.m_x, _vertex.m_x);
		m_MinPt.m_y = min(m_MinPt.m_y, _vertex.m_y);
		m_MinPt.m_z = min(m_MinPt.m_z, _vertex.m_z);
		//
		m_MaxPt.m_x = max(m_MaxPt.m_x, _vertex.m_x);
		m_MaxPt.m_y = max(m_MaxPt.m_y, _vertex.m_y);
		m_MaxPt.m_z = max(m_MaxPt.m_z, _vertex.m_z);
	}
}
//////////////////////////////////////////////////////////////////////////
bool GLTF::BMConvert3DTile::Convert()
{
	if (m_RootKey == INVALID_KEY) return false;
	m_QuadtreeManager.m_RootKey = m_RootKey;
	m_QuadtreeManager.CreateQuadtree();
	m_QuadtreeManager.CreateBMMesh(m_QuadtreeManager.m_Root);
	//
	ConvertB3DM(m_QuadtreeManager.m_Root);
	//
	WriteJSON();
	return true;
}
void GLTF::BMConvert3DTile::ConvertB3DM(BMQuadtreeNode* pQuadtreeNode)
{
	if (pQuadtreeNode->m_Invalide)
	{
		BMConvert _convert;
		_convert.m_pBMMesh = &pQuadtreeNode->m_BMMesh;
		_convert.m_TileSetFileFolder = m_TileSetFileFolder;
		_convert.ConvertBMMeshToB3DM();
	}
	//
	if (pQuadtreeNode->m_pLeft_Down)
	{
		ConvertB3DM(pQuadtreeNode->m_pLeft_Down);
		ConvertB3DM(pQuadtreeNode->m_pLeft_Up);
		ConvertB3DM(pQuadtreeNode->m_pRight_Down);
		ConvertB3DM(pQuadtreeNode->m_pRight_Up);
	}
}
void GLTF::BMConvert3DTile::WriteJSON()
{
	double _transform[16];
	BMVertexD origin = BMVertexD(m_Cartographic.m_x, m_Cartographic.m_y, m_Cartographic.m_z);
	EastNorthUpToFixedFrame(origin, _transform);
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
		//
		tileSetJsonWriter.EndObject();
	}
	//
	tileSetJsonWriter.Key("geometricError");
	tileSetJsonWriter.Double(m_QuadtreeManager.m_Root->ComputeGeometricError()*2.0);
	//
	{
		tileSetJsonWriter.Key("root");
		tileSetJsonWriter.StartObject();
		//
		tileSetJsonWriter.Key("transform");
		tileSetJsonWriter.StartArray();
		for (double t : _transform)
		{
			tileSetJsonWriter.Double(t);
		}
		tileSetJsonWriter.EndArray();
		m_QuadtreeManager.m_Root->writeJSON(&tileSetJsonWriter);
		//
		tileSetJsonWriter.EndObject();
	}
	//
	tileSetJsonWriter.EndObject();
	//////////////////////////////////////////////////////////////////////////
	std::string tileSetJsonString = tileSetJson.GetString();
	std::string filePath = m_TileSetFileFolder + "tileset.json";
	FILE* file = fopen(filePath.c_str(), "wb");
	fwrite(tileSetJsonString.c_str(), sizeof(char), tileSetJsonString.length(), file);
	fclose(file);
}
GLTF::BMVertexD GLTF::BMConvert3DTile::FromRadians(double longitude, double latitude, double height)
{
	BMVertexD _radii = BMVertexD(6378137.0, 6378137.0, 6356752.3142451793);
	BMVertexD _radiiSquared = BMVertexD(_radii.m_x*_radii.m_x, _radii.m_y*_radii.m_y, _radii.m_z*_radii.m_z);
	//
	double cosLatitude = cos(latitude);
	BMVertexD scratchN = BMVertexD(cosLatitude* cos(longitude), cosLatitude*sin(longitude), sin(latitude)).Normalization();
	//
	BMVertexD scratchK = BMVertexD(scratchN.m_x*_radiiSquared.m_x, scratchN.m_y*_radiiSquared.m_y, scratchN.m_z*_radiiSquared.m_z);
	double gamma = sqrt(scratchN.Dot(scratchK));
	//
	scratchK.m_x = scratchK.m_x / gamma; scratchK.m_y = scratchK.m_y / gamma; scratchK.m_z = scratchK.m_z / gamma;
	scratchN.m_x = scratchN.m_x * height; scratchN.m_y = scratchN.m_y * height; scratchN.m_z = scratchN.m_z * height;
	//
	return BMVertexD(scratchK.m_x + scratchN.m_x, scratchK.m_y + scratchN.m_y, scratchK.m_z + scratchN.m_z);

}
void GLTF::BMConvert3DTile::EastNorthUpToFixedFrame(BMVertexD& origin, double matrix[16])
{
	origin.m_x = origin.m_x * PI / 180.0;
	origin.m_y = origin.m_y * PI / 180.0;
	origin = FromRadians(origin.m_x, origin.m_y, origin.m_z);
	//
	class ScratchCalculateCartesian
	{
	public:
		BMVertexD m_east;
		BMVertexD m_north;
		BMVertexD m_up;
		BMVertexD m_west;
		BMVertexD m_south;
		BMVertexD m_down;
	};
	ScratchCalculateCartesian scratchCalculateCartesian;
	//
	BMVertexD _radii = BMVertexD(6378137.0, 6378137.0, 6356752.3142451793);
	BMVertexD _radiiSquared = BMVertexD(_radii.m_x*_radii.m_x, _radii.m_y*_radii.m_y, _radii.m_z*_radii.m_z);
	BMVertexD _radiiToTheFourth = BMVertexD(_radiiSquared.m_x*_radiiSquared.m_x, _radiiSquared.m_y*_radiiSquared.m_y, _radiiSquared.m_z*_radiiSquared.m_z);
	BMVertexD _oneOverRadii = BMVertexD(1.0 / _radii.m_x, 1.0 / _radii.m_y, 1.0 / _radii.m_z);
	BMVertexD _oneOverRadiiSquared = BMVertexD(1.0 / _radiiSquared.m_x, 1.0 / _radiiSquared.m_y, 1.0 / _radiiSquared.m_z);
	//
	{
		scratchCalculateCartesian.m_up.m_x = origin.m_x * _oneOverRadiiSquared.m_x;
		scratchCalculateCartesian.m_up.m_y = origin.m_y * _oneOverRadiiSquared.m_y;
		scratchCalculateCartesian.m_up.m_z = origin.m_z * _oneOverRadiiSquared.m_z;
		scratchCalculateCartesian.m_up.Normalization();
		//
		scratchCalculateCartesian.m_east.m_x = -origin.m_y;
		scratchCalculateCartesian.m_east.m_y = origin.m_x;
		scratchCalculateCartesian.m_east.m_z = 0.0;
		scratchCalculateCartesian.m_east.Normalization();
		//
		scratchCalculateCartesian.m_north = scratchCalculateCartesian.m_up.Cross(scratchCalculateCartesian.m_east);
		//
		scratchCalculateCartesian.m_down.m_x = -scratchCalculateCartesian.m_up.m_x;
		scratchCalculateCartesian.m_down.m_y = -scratchCalculateCartesian.m_up.m_y;
		scratchCalculateCartesian.m_down.m_z = -scratchCalculateCartesian.m_up.m_z;
		//
		scratchCalculateCartesian.m_west.m_x = -scratchCalculateCartesian.m_east.m_x;
		scratchCalculateCartesian.m_west.m_y = -scratchCalculateCartesian.m_east.m_y;
		scratchCalculateCartesian.m_west.m_z = -scratchCalculateCartesian.m_east.m_z;
		//
		scratchCalculateCartesian.m_south.m_x = -scratchCalculateCartesian.m_north.m_x;
		scratchCalculateCartesian.m_south.m_y = -scratchCalculateCartesian.m_north.m_y;
		scratchCalculateCartesian.m_south.m_z = -scratchCalculateCartesian.m_north.m_z;
	}
	//
	matrix[0] = scratchCalculateCartesian.m_east.m_x;
	matrix[1] = scratchCalculateCartesian.m_east.m_y;
	matrix[2] = scratchCalculateCartesian.m_east.m_z;
	matrix[3] = 0.0;
	matrix[4] = scratchCalculateCartesian.m_north.m_x;
	matrix[5] = scratchCalculateCartesian.m_north.m_y;
	matrix[6] = scratchCalculateCartesian.m_north.m_z;
	matrix[7] = 0.0;
	matrix[8] = scratchCalculateCartesian.m_up.m_x;
	matrix[9] = scratchCalculateCartesian.m_up.m_y;
	matrix[10] = scratchCalculateCartesian.m_up.m_z;
	matrix[11] = 0.0;
	matrix[12] = origin.m_x;
	matrix[13] = origin.m_y;
	matrix[14] = origin.m_z;
	matrix[15] = 1.0;
}