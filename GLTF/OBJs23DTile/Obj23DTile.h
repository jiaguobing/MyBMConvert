#pragma once
#include "../GLTFAsset.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include "../BMFile/BMNode.h"
#include "ObjQuadtreeNode.h"
using namespace GLTF;

class OBJBMConvert
{
public:
	OBJBMConvert(BMMesh* pBMMesh, std::string& strTileSetFileFolder)
	{
		m_pBMMesh = pBMMesh;
		m_TileSetFileFolder = strTileSetFileFolder;
	}
	OBJBMConvert()
	{
	}
	~OBJBMConvert()
	{
		freeMemory();
	}
	BMMesh* m_pBMMesh = nullptr;
	Asset* m_pAsset = nullptr;
	//
	std::string m_TileSetFileFolder = "D:/";
	std::string m_strGLTFFilePath; //D:/test.glb
	std::string m_strB3DMFilePath; //D:/test.b3dm
	bool m_EmbeddedTextures = true;
	//
	BMVertex m_MinPt = BMVertex(100000.0f, 100000.0f, 100000.0f);
	BMVertex m_MaxPt = BMVertex(-100000.0f, -100000.0f, -100000.0f);
	//
	bool ConvertBMMeshToGLTF();
	bool ConvertBMMeshToB3DM();
private:
	bool ConvertB3DMTo3DTile();
	bool ConvertGLTFToB3DM();
	bool writeAssetToFile();
	void freeMemory();
	//
	vector<GLTF::MaterialPBR*> m_vMaterials;
	GLTF::MaterialPBR* GetMaterial(GLTF::BMColor& color, string& texturePath, std::map<std::string, GLTF::Texture*>& vTextureMaps);
};
//
class Obj23DTile
{
public:
	Obj23DTile();
	~Obj23DTile();
	//"E:/CityEngineTest/models/"
	std::string m_OBJFolder;
	bool m_EmbeddedTextures;
	std::string m_TileGUID = "9dddb077_5d99_4a87_a95b_79efb32531c1";//从shp文件生成的DB文件中获取
	BMVertex m_MinPt = BMVertex(-7593.192871, -5588.467285, 5.000000);//由txt2shp 获得  obj场景范围
	BMVertex m_MaxPt = BMVertex(7978.231934, 5472.420898, 158.100006);//由txt2shp 获得
	std::string m_MtlFile;
	//
	BMVertex m_Cartesian;//模型笛卡尔坐标
	BMVertex m_Cartographic;//模型经纬度坐标
	BMVertex m_CartesianS;//模型笛卡尔坐标2---计算旋转角
	BMVertex m_CartographicS;//模型经纬度坐标2---计算旋转角
	//
	ObjQuadtreeManager m_QuadtreeManager;
	//
	bool Convert();
private:
	//
	void WriteJSON(double _transform[16]);
	void ConvertB3DM(ObjQuadtreeNode* pQuadtreeNode);
};

