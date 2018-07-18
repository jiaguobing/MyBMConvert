#define  _CRT_SECURE_NO_WARNINGS
#include "Obj23DTile.h"
#include "../rapidjson/stringbuffer.h"
#include "../rapidjson/writer.h"
#include <windows.h>
#include "../BMFile/BMConvert.h"
//
const double PI = 3.1415926535897932384626433832795;
using namespace std;
//
#include <iomanip>
const int HEADER_LENGTH = 12;
const int CHUNK_HEADER_LENGTH = 8;
//
GLTF::MaterialPBR* OBJBMConvert::GetMaterial(GLTF::BMColor& color, string& texturePath, std::map<std::string, GLTF::Texture*>& vTextureMaps)
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
bool OBJBMConvert::ConvertBMMeshToGLTF()
{
	if (m_pBMMesh == nullptr || m_pBMMesh->m_vPrimitives.size() == 0)
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
	for (size_t j = 0; j < numOfPrimitive; ++j)
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
		GLTF::Accessor* _AccCoor = nullptr;
		GLTF::Accessor* _AccNormal = nullptr;
		if (tempPri.m_vCoords.size() > 0 && tempPri.m_TextureImagePath != "")
		{
			attributeData.clear();
			for (size_t i = 0; i < numOfPoint; ++i)
			{
				BMVertex& tempPt = tempPri.m_vCoords[i];
				attributeData.push_back(tempPt.m_u);
				attributeData.push_back(1 - tempPt.m_v);
			}
			_AccCoor = new GLTF::Accessor(GLTF::Accessor::Type::VEC2, GLTF::Constants::WebGL::FLOAT, (unsigned char*)&attributeData[0], attributeData.size() / 2, GLTF::Constants::WebGL::ARRAY_BUFFER);
		}
		else
		{
			attributeData.clear();
			for (size_t i = 0; i < numOfPoint; ++i)
			{
				BMVertex& tempPt = tempPri.m_vNormals[i];
				attributeData.push_back(tempPt.m_x);
				attributeData.push_back(tempPt.m_y);
				attributeData.push_back(tempPt.m_z);
			}
			_AccNormal = new GLTF::Accessor(GLTF::Accessor::Type::VEC3, GLTF::Constants::WebGL::FLOAT, (unsigned char*)&attributeData[0], attributeData.size() / 3, GLTF::Constants::WebGL::ARRAY_BUFFER);
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
		if (_AccCoor != nullptr)
			primitive->attributes["TEXCOORD_0"] = _AccCoor;
		else
			primitive->attributes["NORMAL"] = _AccNormal;
		primitive->attributes["_BATCHID"] = _AccBatchID;
		primitive->indices = _AccIndex;
		//
		primitive->material = GetMaterial(tempPri.m_Color, tempPri.m_TextureImagePath, _mapTextures);
		_pMesh->primitives.push_back(primitive);
	}
	//
	return writeAssetToFile();
}
//
bool OBJBMConvert::writeAssetToFile()
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
void OBJBMConvert::freeMemory()
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
bool OBJBMConvert::ConvertGLTFToB3DM()
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
bool OBJBMConvert::ConvertBMMeshToB3DM()
{
	if (!ConvertBMMeshToGLTF()) return false;
	return ConvertGLTFToB3DM();
}
//////////////////////////////////////////////////////////////////////////
Obj23DTile::Obj23DTile()
{
}

Obj23DTile::~Obj23DTile()
{
}
bool Obj23DTile::Convert()
{
	double _transform[16];
	GLTF::BMConvert3DTile tempUseForTran;
	tempUseForTran.m_Cartesian = m_Cartesian;
	tempUseForTran.m_CartesianS = m_CartesianS;
	tempUseForTran.m_Cartographic = m_Cartographic;
	tempUseForTran.m_CartographicS = m_CartographicS;
	GLTF::BMVertexDD origin = GLTF::BMVertexDD(m_Cartographic.m_x, m_Cartographic.m_y, m_Cartographic.m_z);
	tempUseForTran.EastNorthUpToFixedFrame(origin, _transform);
	tempUseForTran.ComputeRotation(_transform);
	//
	m_QuadtreeManager.m_Cartesian = m_Cartesian;
	m_QuadtreeManager.m_ObjFileFolder = m_OBJFolder;
	m_QuadtreeManager.m_TileSetFileFolder = m_OBJFolder;
	m_QuadtreeManager.InitRootNode(m_MinPt, m_MaxPt);
	m_QuadtreeManager.CreateQuadtree();
	//
	m_QuadtreeManager.CreateBMMesh(m_QuadtreeManager.m_Root);
	ConvertB3DM(m_QuadtreeManager.m_Root);
	//
	WriteJSON(_transform);
	//
	return true;
}
//////////////////////////////////////////////////////////////////////////
void Obj23DTile::WriteJSON(double _transform[16])
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
	tileSetJsonWriter.Double(m_QuadtreeManager.m_Root->ComputeGeometricError() * 2);
	//
	{
		tileSetJsonWriter.Key("root");
		tileSetJsonWriter.StartObject();
		//
		tileSetJsonWriter.Key("transform");
		tileSetJsonWriter.StartArray();
		for (int i = 0; i < 16; ++i)
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
	std::string filePath = m_OBJFolder + "tileset.json";
	FILE* file = fopen(filePath.c_str(), "wb");
	fwrite(tileSetJsonString.c_str(), sizeof(char), tileSetJsonString.length(), file);
	fclose(file);
}
void Obj23DTile::ConvertB3DM(ObjQuadtreeNode* pQuadtreeNode)
{
	if (pQuadtreeNode->m_Invalide)
	{
		OBJBMConvert _convert;
		_convert.m_EmbeddedTextures = m_EmbeddedTextures;
		_convert.m_pBMMesh = &pQuadtreeNode->m_BMMesh;
		_convert.m_TileSetFileFolder = m_OBJFolder;
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
