#pragma once
#include "../GLTFAsset.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include "../BMFile/BMNode.h"
using namespace GLTF;
class TerrainNode
{
public:
	~TerrainNode();
	//
	bool m_ValidataContent = false;
	int m_Column;
	int m_Row;
	int m_Layer;
	//块的区域
	BMVertexD m_LUCorner;
	BMVertexD m_RDCorner;
	double m_GeoTransform;
	//像素大小---不一定等于DEM_CHUNK_SIZE
	int m_ValidateDataWidth;
	int m_ValidateDataHeight;
	//DEM高程值
	float* m_pHeights;
	float m_minHeight;
	float m_maxHeight;
	//纹理图片路径
	std::string m_ImagePath;
	std::string m_NodeName;
	std::string m_strGLTFFilePath;
	std::string m_TileSetFileFolder;
	std::string m_strB3DMFilePath;
	//
	bool m_EmbeddedTextures;
	//
	TerrainNode* m_pParent = nullptr;
	std::vector<TerrainNode*> m_vChildren;
	//////////////////////////////////////////////////////////////////////////
	std::vector<unsigned int> m_vIndexs;
	std::vector<BMVertexD> m_vPositions;
	std::vector<BMVertexD> m_vNormals;
	std::vector<BMVertexD> m_vCoords;
	//
	bool Convert2B3DM(bool addOne = false);
	double ComputeGeometricError();
	void writeJSON(void* writer, bool inOBJ = false);
private:
	bool ComputeContent();
	bool ComputeContentAddOne();
	void Convert2GLTF();
	//大地坐标转笛卡尔坐标---cesium
	BMVertexD FromRadians(double longitude, double latitude, double height);
	bool writeAssetToFile();
	void freeMemory();
	bool ConvertGLTFToB3DM();
	void ComputeRegion(double region[6]);
private:
	Asset* m_pAsset = nullptr;
};

class Terrain23DTile
{
public:
	Terrain23DTile();
	~Terrain23DTile();
	std::string m_DemFile;
	std::string m_ImageFile;
	std::string m_OutputFolder;
	bool m_EmbeddedTextures;
	//
	bool Convert();
	TerrainNode* m_pRoot = nullptr;
private:
	bool GetBound(BMVertexD& leftUpCorner, BMVertexD& rightDownCorner, std::string& filePath, int& width, int& height, double geoTransform[6]);
	//2048 1024 512 256 128 等最接近的一个 
	int GetImageChunkSize(int orginalSize);
	//
	TerrainNode* FindNode(std::vector<TerrainNode*>& vNodes,int rowIndex,int colIndex);
	//
	void WriteJSON();
};

