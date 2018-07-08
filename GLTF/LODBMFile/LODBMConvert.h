#pragma once
#include "../GLTFAsset.h"
#include "LODBMQuadtreeNode.h"
//
namespace GLTF {
	class LODBMConvert
	{
	public:
		LODBMConvert(BMMesh* pBMMesh, std::string& strTileSetFileFolder)
		{
			m_pBMMesh = pBMMesh;
			m_TileSetFileFolder = strTileSetFileFolder;
		}
		LODBMConvert()
		{
		}
		~LODBMConvert()
		{
			freeMemory();
		}
		BMMesh* m_pBMMesh = nullptr;
		Asset* m_pAsset = nullptr;
		//
		bool m_EmbeddedTextures = true;
		std::string m_TileSetFileFolder = "D:/";
		std::string m_strGLTFFilePath; //D:/test.glb
		std::string m_strB3DMFilePath; //D:/test.b3dm
		//
		BMVertex m_MinPt = BMVertex(100000.0f, 100000.0f, 100000.0f);
		BMVertex m_MaxPt = BMVertex(-100000.0f, -100000.0f, -100000.0f);
		//
		bool ConvertBMMeshToGLTF();
		bool ConvertBMMeshToB3DM();
	private:
		vector<GLTF::MaterialPBR*> m_vMaterials;
		GLTF::MaterialPBR* GetMaterial(GLTF::BMColor& color, string& texturePath, std::map<std::string, GLTF::Texture*>& vTextureMaps);
		bool ConvertB3DMTo3DTile();
		bool ConvertGLTFToB3DM();
		bool writeAssetToFile();
		void freeMemory();
	};
	//////////////////////////////////////////////////////////////////////////
	class LODBMConvert3DTile
	{
	public:
		HC_KEY m_RootKey = INVALID_KEY;
		HC_KEY m_HoopsModelKey;// = m_pViewer->GetModel()->GetModelKey();

		vector<std::string> m_vLODFilePaths;
		std::string m_TileSetFileFolder = "D:\\";
		BMVertex m_Cartesian;//模型笛卡尔坐标
		BMVertex m_Cartographic;//模型经纬度坐标
		//
		BMVertex m_CartesianS;//模型笛卡尔坐标2---计算旋转角
		BMVertex m_CartographicS;//模型经纬度坐标2---计算旋转角
		//
		LODBMQuadtreeManager m_QuadtreeManager;
		int m_TesseNum = 6;
		bool m_EmbeddedTextures = true;
		//
		bool Convert();
	private:
		void ConvertB3DM(LODBMQuadtreeNode* pQuadtreeNode);
		void WriteJSON();
	private:
		//origin = {104.07048834,30.66109327,0.0}---cesium
		void EastNorthUpToFixedFrame(BMVertexDD& origin, double matrix[16]);
		//matrix 右剩 pt
		BMVertexDD MatrixMultiplyVector(BMVertexDD& pt, double matrix[16]);
		//大地坐标转笛卡尔坐标---cesium
		BMVertexDD FromRadians(double longitude, double latitude, double height);
		//
		void ComputeRotation(double matrix[16]);
		//matrixS = matrixA * matrixB
		void MatrixMultiplyMatrix(double matrixA[16], double matrixB[16], double matrixS[16]);
	};

};
