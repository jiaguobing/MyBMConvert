#pragma once
#include "../GLTFAsset.h"
#include "BMQuadtreeNode.h"
//
namespace GLTF {
	class BMConvert
	{
	public:
		BMConvert(BMMesh* pBMMesh, std::string& strTileSetFileFolder)
		{
			m_pBMMesh = pBMMesh;
			m_TileSetFileFolder = strTileSetFileFolder;
		}
		BMConvert()
		{
		}
		~BMConvert()
		{
			freeMemory();
		}
		BMMesh* m_pBMMesh = nullptr;
		Asset* m_pAsset = nullptr;
		//
		std::string m_TileSetFileFolder = "D:\\";
		std::string m_strGLTFFilePath; //D:\\test.glb
		std::string m_strB3DMFilePath; //D:\\test.b3dm
		//
		BMVertex m_MinPt = BMVertex(100000.0f, 100000.0f, 100000.0f);
		BMVertex m_MaxPt = BMVertex(-100000.0f, -100000.0f, -100000.0f);
		//
		bool ConvertBMMeshToGLTF();
		bool ConvertBMMeshToB3DM();
	private:
		bool ConvertB3DMTo3DTile();
		bool ConvertGLTFToB3DM();
		void ComputeMinMaxPt();
		bool writeAssetToFile();
		void freeMemory();
	};
	//////////////////////////////////////////////////////////////////////////
	class BMConvert3DTile
	{
	public:
		HC_KEY m_RootKey = INVALID_KEY;
		std::string m_TileSetFileFolder = "D:\\";
		BMVertex m_Cartesian;//模型笛卡尔坐标
		BMVertex m_Cartographic;//模型经纬度坐标
		//
		BMQuadtreeManager m_QuadtreeManager;
		//

		bool Convert();
	private:
		void ConvertB3DM(BMQuadtreeNode* pQuadtreeNode);
		void WriteJSON();
	private:
		//origin = {104.07048834,30.66109327,0.0}---cesium
		void EastNorthUpToFixedFrame(BMVertexD& origin, double matrix[16]);
		//大地坐标转笛卡尔坐标---cesium
		BMVertexD FromRadians(double longitude, double latitude, double height);
	};

};
