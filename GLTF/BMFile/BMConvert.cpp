#define  _CRT_SECURE_NO_WARNINGS
#include "BMConvert.h"
#include "../rapidjson/stringbuffer.h"
#include "../rapidjson/writer.h"
#include <algorithm>
#include "objbase.h"  
#include <string> 
//
const double PI = 3.1415926535897932384626433832795;
const int HEADER_LENGTH = 12;
const int CHUNK_HEADER_LENGTH = 8;
//
GLTF::MaterialPBR* GLTF::BMConvert::GetMaterial(GLTF::BMColor& color, string& texturePath, std::map<std::string, GLTF::Texture*>& vTextureMaps)
{
	for (auto _pM : m_vMaterials)
	{
		if ((_pM->metallicRoughness->baseColorFactor &&
			_pM->metallicRoughness->baseColorFactor[0] == color.m_r* 0.5 &&
			_pM->metallicRoughness->baseColorFactor[1] == color.m_g* 0.5 &&
			_pM->metallicRoughness->baseColorFactor[2] == color.m_b* 0.5 &&
			_pM->metallicRoughness->baseColorFactor[3] == color.m_a) ||
			(
			_pM->metallicRoughness->baseColorTexture &&
			_pM->metallicRoughness->baseColorTexture->texture == vTextureMaps[texturePath]
			))
		{
			return _pM;
		}
	}
	//
	GLTF::MaterialPBR* material = new GLTF::MaterialPBR();
	material->metallicRoughness->metallicFactor = 0.0f;
	material->metallicRoughness->roughnessFactor = -1.0f;
	material->emissiveFactor = new float[4]{0.0, 0.0, 0.0, 0.0};
	material->alphaMode = "OPAQUE";
	material->doubleSided = false;
	if (texturePath == "")
	{
		material->metallicRoughness->baseColorFactor = new float[4];
		material->metallicRoughness->baseColorFactor[0] = color.m_r * 0.5;
		material->metallicRoughness->baseColorFactor[1] = color.m_g * 0.5;
		material->metallicRoughness->baseColorFactor[2] = color.m_b * 0.5;
		material->metallicRoughness->baseColorFactor[3] = color.m_a;
		if (color.m_a < 1.0f)
		{
			material->alphaMode = "BLEND";
			material->doubleSided = true;
		}
	}
	else
	{
		GLTF::MaterialPBR::Texture* texture = new GLTF::MaterialPBR::Texture();
		texture->texture = vTextureMaps[texturePath];
		material->metallicRoughness->baseColorTexture = texture;
		if (texture->texture->source->mimeType == "image/png")
		{
			material->alphaMode = "BLEND";
			material->doubleSided = true;
		}
	}
	//
	m_vMaterials.push_back(material);
	return material;
}
bool GLTF::BMConvert::ConvertBMMeshToGLTF()
{
	if (m_pBMMesh == nullptr ||m_pBMMesh->m_vPrimitives.size()==0)
	{
		return false;
	}
	//
	if (m_pAsset != nullptr) return true;
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
	size_t numOfPrimitive = m_pBMMesh->m_vPrimitives.size();
	std::map<std::string, GLTF::Image*> _mapImages;
	std::map<std::string, GLTF::Texture*> _mapTextures;
	GLTF::Sampler* _pSampler = nullptr;
	for (size_t i = 0; i < numOfPrimitive; ++i)
	{
		BMPrimitive& tempPri = m_pBMMesh->m_vPrimitives[i];
		//
		if (tempPri.m_TextureImagePath != "" && _mapImages[tempPri.m_TextureImagePath] == nullptr)
		{
			GLTF::Image* pImage = GLTF::Image::load(tempPri.m_TextureImagePath);
			std::string::size_type sfind = pImage->uri.rfind(L'/');
			pImage->uri = pImage->uri.substr(sfind + 1);
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
	std::vector<float> attributeData;
	for (size_t j = 0; j < numOfPrimitive;++j)
	{
		attributeData.clear();
		//
		BMPrimitive& tempPri = m_pBMMesh->m_vPrimitives[j];
		GLTF::Primitive* primitive = new GLTF::Primitive();
		primitive->mode = GLTF::Primitive::Mode::TRIANGLES;
		//
		size_t numOfPoint = tempPri.m_vPositions.size();
		attributeData.reserve(numOfPoint * 3);
		//
		for (size_t i = 0; i < numOfPoint; ++i)
		{
			BMVertex& tempPt = tempPri.m_vPositions[i];
			attributeData.push_back(tempPt.m_x);
			attributeData.push_back(tempPt.m_y);
			attributeData.push_back(tempPt.m_z);
		}
		GLTF::Accessor* _AccPosition = new GLTF::Accessor(GLTF::Accessor::Type::VEC3, GLTF::Constants::WebGL::FLOAT, (unsigned char*)&attributeData[0], attributeData.size() / 3, GLTF::Constants::WebGL::ARRAY_BUFFER);
		//
		attributeData.clear();
		for (size_t i = 0; i < numOfPoint; ++i)
		{
			BMVertex& tempPt = tempPri.m_vNormals[i];
			attributeData.push_back(tempPt.m_x);
			attributeData.push_back(tempPt.m_y);
			attributeData.push_back(tempPt.m_z);
		}
		GLTF::Accessor* _AccNormal = new GLTF::Accessor(GLTF::Accessor::Type::VEC3, GLTF::Constants::WebGL::FLOAT, (unsigned char*)&attributeData[0], attributeData.size() / 3, GLTF::Constants::WebGL::ARRAY_BUFFER);
		//
		GLTF::Accessor* _AccCoor = nullptr;
		attributeData.clear();
		if (tempPri.m_vCoords.size() > 0 && tempPri.m_TextureImagePath != "")
		{
			for (size_t i = 0; i < numOfPoint; ++i)
			{
				BMVertex& tempPt = tempPri.m_vCoords[i];
				attributeData.push_back(tempPt.m_u);
				attributeData.push_back(1 - tempPt.m_v);
			}
			_AccCoor = new GLTF::Accessor(GLTF::Accessor::Type::VEC2, GLTF::Constants::WebGL::FLOAT, (unsigned char*)&attributeData[0], attributeData.size() / 2, GLTF::Constants::WebGL::ARRAY_BUFFER);
		}
		//
		GLTF::Accessor* _AccBatchID = nullptr;
		std::vector<unsigned short> unsignedShortIndexData; unsignedShortIndexData.reserve(numOfPoint);
		if (numOfPoint < 65536)
		{
			for (size_t i = 0; i < numOfPoint; ++i)
			{
				unsignedShortIndexData.push_back((unsigned short)tempPri.m_vBatchIDs[i]);
			}
			_AccBatchID = new GLTF::Accessor(Accessor::Type::SCALAR, GLTF::Constants::WebGL::UNSIGNED_SHORT, (unsigned char*)&unsignedShortIndexData[0], unsignedShortIndexData.size(), GLTF::Constants::WebGL::ARRAY_BUFFER);
		}
		else
		{
			_AccBatchID = new GLTF::Accessor(Accessor::Type::SCALAR, GLTF::Constants::WebGL::UNSIGNED_INT, (unsigned char*)&tempPri.m_vBatchIDs[0], tempPri.m_vBatchIDs.size(), GLTF::Constants::WebGL::ARRAY_BUFFER);
		}
		//
		GLTF::Accessor* _AccIndex = nullptr;
		unsignedShortIndexData.clear();
		size_t numOfIndex = tempPri.m_vIndexs.size();
		if (numOfIndex < 65536)
		{
			for (size_t i = 0; i < numOfIndex; ++i)
			{
				unsignedShortIndexData.push_back((unsigned short)tempPri.m_vIndexs[i]);
			}
			_AccIndex = new GLTF::Accessor(Accessor::Type::SCALAR, GLTF::Constants::WebGL::UNSIGNED_SHORT, (unsigned char*)&unsignedShortIndexData[0], unsignedShortIndexData.size(), GLTF::Constants::WebGL::ELEMENT_ARRAY_BUFFER);
		}
		else
		{
			_AccIndex = new GLTF::Accessor(Accessor::Type::SCALAR, GLTF::Constants::WebGL::UNSIGNED_INT, (unsigned char*)&tempPri.m_vIndexs[0], tempPri.m_vIndexs.size(), GLTF::Constants::WebGL::ELEMENT_ARRAY_BUFFER);
		}
		//
		primitive->attributes["POSITION"] = _AccPosition;
		primitive->attributes["NORMAL"] = _AccNormal;
		if (tempPri.m_TextureImagePath != "" && _AccCoor != nullptr)
			primitive->attributes["TEXCOORD_0"] = _AccCoor;
		primitive->attributes["_BATCHID"] = _AccBatchID;
		primitive->indices = _AccIndex;
		/*GLTF::MaterialCommon* material = new GLTF::MaterialCommon();
		material->technique = GLTF::MaterialCommon::PHONG;
		material->doubleSided = false;
		if (tempPri.m_TextureImagePath == "")
		{
		material->values->diffuse = new float[4];
		material->values->diffuse[0] = tempPri.m_Color.m_r;
		material->values->diffuse[1] = tempPri.m_Color.m_g;
		material->values->diffuse[2] = tempPri.m_Color.m_b;
		material->values->diffuse[3] = tempPri.m_Color.m_a;
		if (tempPri.m_Color.m_a < 1.0f)
		{
		material->transparent = true;
		material->doubleSided = true;
		material->values->transparency = new float[1];
		material->values->transparency[0] = tempPri.m_Color.m_a;
		}
		}
		else
		{
		material->values->diffuseTexture = _mapTextures[tempPri.m_TextureImagePath];
		if (material->values->diffuseTexture->source->mimeType == "image/png")
		{
		material->transparent = true;
		material->doubleSided = true;
		}
		}*/
		//
		primitive->material = GetMaterial(tempPri.m_Color, tempPri.m_TextureImagePath, _mapTextures);
		_pMesh->primitives.push_back(primitive);
	}
	//
	return writeAssetToFile();
}
//
bool GLTF::BMConvert::writeAssetToFile()
{
	Options* options = new Options();
	options->binary = true;
	options->doubleSided = true;
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
//////////////////////////////////////////////////////////////////////////
bool GLTF::BMConvert3DTile::Convert()
{
	if (m_RootKey == INVALID_KEY) return false;
	//
	double _transform[16];
	BMVertexDD origin = BMVertexDD(m_Cartographic.m_x, m_Cartographic.m_y, m_Cartographic.m_z);
	EastNorthUpToFixedFrame(origin, _transform);
	ComputeRotation(_transform);
	int index = 0;
	for (auto t:_transform)
	{
		m_QuadtreeManager.m_transform[index++] = t;
	}
	m_QuadtreeManager.m_DBFilePath = m_DBFilePath;
	m_QuadtreeManager.m_TesseNum = m_TesseNum;
	m_QuadtreeManager.m_RootKey = m_RootKey;
	m_QuadtreeManager.m_HoopsModelKey = m_HoopsModelKey;
	m_QuadtreeManager.m_TileSetFileFolder = m_TileSetFileFolder;
	m_QuadtreeManager.m_Cartesian = m_Cartesian;
	//
	m_TileGUID = GetGUIDStr();
	m_QuadtreeManager.AddNewDBColumn(m_TileGUID);
	m_QuadtreeManager.m_SQLHelper.BeginTransaction();
	m_QuadtreeManager.CreateQuadtree();
	//
	GetGUIDInDB();
	ComputeRegionBoxInDB();
	//
	m_QuadtreeManager.m_SQLHelper.CommitTransaction();
	//
	m_QuadtreeManager.CreateBMMesh(m_QuadtreeManager.m_Root);
	ConvertB3DM(m_QuadtreeManager.m_Root);
	//
	WriteJSON(_transform);
	return true;
}
void GLTF::BMConvert3DTile::ComputeRegionBoxInDB()
{
	unordered_map<string, _inerForNodeRegion*>* _pMaps = &m_QuadtreeManager.m_NodeRegionMaps;
	sqlite3* pSQL = m_QuadtreeManager.m_SQLHelper.GetSQLite();
	sqlite3_stmt *pStmt = NULL;
	string sqlStr = "SELECT id FROM model_tree WHERE pid = ?;";
	sqlite3_prepare_v2(pSQL, sqlStr.c_str(), sqlStr.length(), &pStmt, NULL);
	vector<string> _vGUIDs;
	string _GUID;
	double _CartesianX, _CartesianY, _CartesianZ;
	//
	for (size_t j = 0; j < m_vGUIDs.size();++j)
	{
		_vGUIDs.clear();
		string& _guid = m_vGUIDs[j];
		//
		int nnnnn = sqlite3_bind_text(pStmt, 1, _guid.c_str(), _guid.length(), SQLITE_STATIC);

		int  rv = sqlite3_step(pStmt);
		while (rv == SQLITE_ROW)
		{
			_GUID = (char *)sqlite3_column_text(pStmt, 0);
			_vGUIDs.push_back(_GUID);
			rv = sqlite3_step(pStmt);
		}
		////
		//while (sqlite3_step(pStmt) == SQLITE_ROW)
		//{
		//	_GUID = (char *)sqlite3_column_text(pStmt, 0);
		//	_vGUIDs.push_back(_GUID);
		//}
		//
		sqlite3_reset(pStmt);
		//
		HPoint _minPt(100000.0f, 100000.0f, 100000.0f);
		HPoint _maxPt(-100000.0f, -100000.0f, -100000.0f);
		for (size_t i = 0; i < _vGUIDs.size();++i)
		{
			string& _childguid = _vGUIDs[i];
			auto it = _pMaps->find(_childguid);
			if (it != _pMaps->end())
			{
				_inerForNodeRegion* pNodeReion = it->second;
				_minPt.x = min(_minPt.x,pNodeReion->_minPt.x);
				_minPt.y = min(_minPt.y, pNodeReion->_minPt.y);
				_minPt.z = min(_minPt.z, pNodeReion->_minPt.z);
				_maxPt.x = max(_maxPt.x, pNodeReion->_maxPt.x);
				_maxPt.y = max(_maxPt.y, pNodeReion->_maxPt.y);
				_maxPt.z = max(_maxPt.z, pNodeReion->_maxPt.z);
			}
		}
		if (_minPt.x != 100000.0f)
		{
			_inerForNodeRegion* pNewNode = new _inerForNodeRegion();
			pNewNode->_minPt = _minPt;
			pNewNode->_maxPt = _maxPt;
			m_QuadtreeManager.m_NodeRegionMaps[_guid] = pNewNode;
			//
			HPoint _centerPt = (_minPt + _maxPt) / 2.0f;
			m_QuadtreeManager.MatrixMultiplyVector(_centerPt.x, _centerPt.y, _centerPt.z, _CartesianX, _CartesianY, _CartesianZ);
			float maxRadius = max(max(_maxPt.x - _minPt.x, _maxPt.y - _minPt.y), _maxPt.z - _minPt.z);
			//
			sqlStr = "UPDATE model_tree SET centerX = " + std::to_string(_CartesianX);
			sqlStr += ",centerY = " + std::to_string(_CartesianY);
			sqlStr += ",centerZ = " + std::to_string(_CartesianZ);
			sqlStr += ",radius = " + std::to_string(maxRadius);
			sqlStr += " Where id = '" + _guid + "';";
			m_QuadtreeManager.m_SQLHelper.SqlStateExec(sqlStr.c_str());
		}
		//
	}
	sqlite3_finalize(pStmt);
}
void GLTF::BMConvert3DTile::GetGUIDInDB()
{
	sqlite3* pSQL = m_QuadtreeManager.m_SQLHelper.GetSQLite();
	//
	string sqlStr = "SELECT count(*) as rowNum FROM model_tree WHERE flag < 2 ";
	sqlite3_stmt *pStmt = NULL;
	sqlite3_prepare_v2(pSQL, sqlStr.c_str(), sqlStr.length(), &pStmt, NULL);
	sqlite3_step(pStmt);
	int num = sqlite3_column_int(pStmt, 0);
	m_vGUIDs.reserve(num);
	sqlite3_finalize(pStmt);
	//
	GetGUIDInDBEX("");
	
}
void GLTF::BMConvert3DTile::GetGUIDInDBEX(const std::string& pid)
{
	sqlite3* pSQL = m_QuadtreeManager.m_SQLHelper.GetSQLite();
	//
	string sqlStr = "SELECT id  FROM model_tree WHERE flag < 2 AND pid = '";
	sqlStr += pid;
	sqlStr += "';";
	sqlite3_stmt *pStmt = NULL;
	//
	sqlite3_prepare_v2(pSQL, sqlStr.c_str(), sqlStr.length(), &pStmt, NULL);
	string _GUID;
	vector<string> _vChilds;
	while (sqlite3_step(pStmt) == SQLITE_ROW)
	{
		_GUID = (char *)sqlite3_column_text(pStmt, 0);
		m_vGUIDs.push_back(_GUID);
		_vChilds.push_back(_GUID);
	}
	sqlite3_finalize(pStmt);
	//
	for (size_t i = 0; i < _vChilds.size();++i)
	{
		GetGUIDInDBEX(_vChilds[i]);
	}
}
std::string GLTF::BMConvert3DTile::GetGUIDStr()
{
	_GUID guid;
	CoCreateGuid(&guid);

	char buf[64] = { 0 };
	_snprintf_s(
		buf,
		sizeof(buf),
		//"{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}",
		"%08X_%04X_%04X_%02X%02X_%02X%02X%02X%02X%02X%02X",
		guid.Data1, guid.Data2, guid.Data3,
		guid.Data4[0], guid.Data4[1],
		guid.Data4[2], guid.Data4[3],
		guid.Data4[4], guid.Data4[5],
		guid.Data4[6], guid.Data4[7]);

	std::string rStr = std::string(buf);
	transform(rStr.begin(), rStr.end(), rStr.begin(), tolower);
	return rStr;
}
void GLTF::BMConvert3DTile::ConvertB3DM(BMQuadtreeNode* pQuadtreeNode)
{
	if (pQuadtreeNode->m_Invalide)
	{
		BMConvert _convert;
		_convert.m_EmbeddedTextures = m_EmbeddedTextures;
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
void GLTF::BMConvert3DTile::WriteJSON(double _transform[16])
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
		tileSetJsonWriter.String(m_TileGUID.c_str());
		//
		tileSetJsonWriter.EndObject();
	}
	//
	tileSetJsonWriter.Key("geometricError");
	tileSetJsonWriter.Double(m_QuadtreeManager.m_Root->ComputeGeometricError()*2);
	//
	{
		tileSetJsonWriter.Key("root");
		tileSetJsonWriter.StartObject();
		//
		tileSetJsonWriter.Key("transform");
		tileSetJsonWriter.StartArray();
		for (int i = 0; i < 16;++i)
		{
			tileSetJsonWriter.Double(_transform[i]);
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
void GLTF::BMConvert3DTile::ComputeRotation(double matrix[16])
{
	//不需要计算旋转矩阵
	if (m_CartographicS.m_x == 0.0f && m_CartographicS.m_y == 0.0f)
		return;
	//
	GLTF::BMVertexDD delta = GLTF::BMVertexDD(m_CartesianS.m_x - m_Cartesian.m_x, m_CartesianS.m_y - m_Cartesian.m_y, m_CartesianS.m_z - m_Cartesian.m_z);
	GLTF::BMVertexDD p0 = FromRadians(m_Cartographic.m_x* PI / 180.0, m_Cartographic.m_y* PI / 180.0, m_Cartographic.m_z);
	GLTF::BMVertexDD p1 = MatrixMultiplyVector(delta, matrix);
	GLTF::BMVertexDD p2 = FromRadians(m_CartographicS.m_x* PI / 180.0, m_CartographicS.m_y* PI / 180.0, m_CartographicS.m_z);
	//
	GLTF::BMVertexDD n0 = p1 - p0;
	GLTF::BMVertexDD n1 = p2 - p0;
	GLTF::BMVertexDD rotateAxis = n0.Cross(n1).Normalization();
	GLTF::BMVertexDD surfaceNormal = p0.Normalization();
	double _RotateAngle = acos((n0.Dot(n1)) / (n0.GetLength()*n1.GetLength()));
	if (rotateAxis.Dot(surfaceNormal) < 0.0)
		_RotateAngle = 6.283185307179586 - _RotateAngle;
	//
	double halfRotateAngle = _RotateAngle / 2.0;
	double sinHalf = sin(halfRotateAngle);
	double cosHalf = cos(halfRotateAngle);
	//
	GLTF::BMVertexDD _quaternion = GLTF::BMVertexDD(0,0,sinHalf,cosHalf);
	double newMatix[16];
	_quaternion.ToMatrix(newMatix);
	//
	MatrixMultiplyMatrix(matrix, newMatix, matrix);
}
void GLTF::BMConvert3DTile::MatrixMultiplyMatrix(double matrixA[16], double matrixB[16], double matrixS[16])
{
	double column0Row0 = matrixA[0] * matrixB[0] + matrixA[4] * matrixB[1] + matrixA[8] * matrixB[2] + matrixA[12] * matrixB[3];
	double column0Row1 = matrixA[1] * matrixB[0] + matrixA[5] * matrixB[1] + matrixA[9] * matrixB[2] + matrixA[13] * matrixB[3];
	double column0Row2 = matrixA[2] * matrixB[0] + matrixA[6] * matrixB[1] + matrixA[10] * matrixB[2] + matrixA[14] * matrixB[3];
	double column0Row3 = matrixA[3] * matrixB[0] + matrixA[7] * matrixB[1] + matrixA[11] * matrixB[2] + matrixA[15] * matrixB[3];

	double column1Row0 = matrixA[0] * matrixB[4] + matrixA[4] * matrixB[5] + matrixA[8] * matrixB[6] + matrixA[12] * matrixB[7];
	double column1Row1 = matrixA[1] * matrixB[4] + matrixA[5] * matrixB[5] + matrixA[9] * matrixB[6] + matrixA[13] * matrixB[7];
	double column1Row2 = matrixA[2] * matrixB[4] + matrixA[6] * matrixB[5] + matrixA[10] * matrixB[6] + matrixA[14] * matrixB[7];
	double column1Row3 = matrixA[3] * matrixB[4] + matrixA[7] * matrixB[5] + matrixA[11] * matrixB[6] + matrixA[15] * matrixB[7];

	double column2Row0 = matrixA[0] * matrixB[8] + matrixA[4] * matrixB[9] + matrixA[8] * matrixB[10] + matrixA[12] * matrixB[11];
	double column2Row1 = matrixA[1] * matrixB[8] + matrixA[5] * matrixB[9] + matrixA[9] * matrixB[10] + matrixA[13] * matrixB[11];
	double column2Row2 = matrixA[2] * matrixB[8] + matrixA[6] * matrixB[9] + matrixA[10] * matrixB[10] + matrixA[14] * matrixB[11];
	double column2Row3 = matrixA[3] * matrixB[8] + matrixA[7] * matrixB[9] + matrixA[11] * matrixB[10] + matrixA[15] * matrixB[11];

	double column3Row0 = matrixA[0] * matrixB[12] + matrixA[4] * matrixB[13] + matrixA[8] * matrixB[14] + matrixA[12] * matrixB[15];
	double column3Row1 = matrixA[1] * matrixB[12] + matrixA[5] * matrixB[13] + matrixA[9] * matrixB[14] + matrixA[13] * matrixB[15];
	double column3Row2 = matrixA[2] * matrixB[12] + matrixA[6] * matrixB[13] + matrixA[10] * matrixB[14] + matrixA[14] * matrixB[15];
	double column3Row3 = matrixA[3] * matrixB[12] + matrixA[7] * matrixB[13] + matrixA[11] * matrixB[14] + matrixA[15] * matrixB[15];

	matrixS[0] = column0Row0;
	matrixS[1] = column0Row1;
	matrixS[2] = column0Row2;
	matrixS[3] = column0Row3;
	matrixS[4] = column1Row0;
	matrixS[5] = column1Row1;
	matrixS[6] = column1Row2;
	matrixS[7] = column1Row3;
	matrixS[8] = column2Row0;
	matrixS[9] = column2Row1;
	matrixS[10] = column2Row2;
	matrixS[11] = column2Row3;
	matrixS[12] = column3Row0;
	matrixS[13] = column3Row1;
	matrixS[14] = column3Row2;
	matrixS[15] = column3Row3;
}
GLTF::BMVertexDD GLTF::BMConvert3DTile::FromRadians(double longitude, double latitude, double height)
{
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
	return BMVertexDD(scratchK.m_x + scratchN.m_x, scratchK.m_y + scratchN.m_y, scratchK.m_z + scratchN.m_z);

}
//matrix 右剩 pt
GLTF::BMVertexDD GLTF::BMConvert3DTile::MatrixMultiplyVector(BMVertexDD& pt, double matrix[16])
{
	BMVertexDD newPt;
	newPt.m_x = matrix[0] * pt.m_x + matrix[4] * pt.m_y + matrix[8] * pt.m_z + matrix[12];
	newPt.m_y = matrix[1] * pt.m_x + matrix[5] * pt.m_y + matrix[9] * pt.m_z + matrix[13];
	newPt.m_z = matrix[2] * pt.m_x + matrix[6] * pt.m_y + matrix[10] * pt.m_z + matrix[14];
	//
	return newPt;
}
void GLTF::BMConvert3DTile::EastNorthUpToFixedFrame(BMVertexDD& origin, double matrix[16])
{
	origin.m_x = origin.m_x * PI / 180.0;
	origin.m_y = origin.m_y * PI / 180.0;
	origin = FromRadians(origin.m_x, origin.m_y, origin.m_z);
	//
	class ScratchCalculateCartesian
	{
	public:
		BMVertexDD m_east;
		BMVertexDD m_north;
		BMVertexDD m_up;
		BMVertexDD m_west;
		BMVertexDD m_south;
		BMVertexDD m_down;
	};
	ScratchCalculateCartesian scratchCalculateCartesian;
	//
	BMVertexDD _radii = BMVertexDD(6378137.0, 6378137.0, 6356752.3142451793);
	BMVertexDD _radiiSquared = BMVertexDD(_radii.m_x*_radii.m_x, _radii.m_y*_radii.m_y, _radii.m_z*_radii.m_z);
	BMVertexDD _radiiToTheFourth = BMVertexDD(_radiiSquared.m_x*_radiiSquared.m_x, _radiiSquared.m_y*_radiiSquared.m_y, _radiiSquared.m_z*_radiiSquared.m_z);
	BMVertexDD _oneOverRadii = BMVertexDD(1.0 / _radii.m_x, 1.0 / _radii.m_y, 1.0 / _radii.m_z);
	BMVertexDD _oneOverRadiiSquared = BMVertexDD(1.0 / _radiiSquared.m_x, 1.0 / _radiiSquared.m_y, 1.0 / _radiiSquared.m_z);
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