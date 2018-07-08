#pragma once
#include "../../BM23DTile/base.h"
#include "BMNode.h"
#include "TessellationCylinder.h"
#include "vector"
#include "SqliteHelper.h"
#include<unordered_map>
using namespace std;
//四叉树节点 中包含的内容---segments--->
class BMShellPrimitive
{
public:
	HC_KEY m_shellKey = INVALID_KEY;
	HC_KEY m_CylinderKey = INVALID_KEY;
	HC_KEY m_PolyCylinderKey = INVALID_KEY;
	HC_KEY m_segmentKey = INVALID_KEY;
	std::string m_segmentName;
	//
	vector<GLTF::BMVertex> m_vPositions;
	vector<GLTF::BMVertex> m_vNormals;
	GLTF::BMColor m_Color = GLTF::BMColor(0.0, 1.0, 0.0);
	vector<GLTF::BMTriangle> m_vTris;
	vector<GLTF::BMVertex> m_vCoords;
	std::string m_TextureImagePath = "";
	int m_TesseNum;
	//
	//计算shell数据---posi normal tri
	bool ComputeContent(float offsetX, float offsetY, float offsetZ);
	//m_vTris.size > (65535 / 3) 时需要细分这个BMShellPrimitive----webGL-中glVertexAttribPointer中type不支持 UNSIGNED_INT
	vector<BMShellPrimitive*> Subdivide(float offsetX, float offsetY, float offsetZ);
private:
	bool extractRGBNew(char* pRGB, float& fRed, float& fGreen, float& fBlue);
	GLTF::BMVertex ComputeNormal(GLTF::BMVertex& p0, GLTF::BMVertex& p1, GLTF::BMVertex& p2);
	HC_KEY CreateShellByCylinder();
};
struct BMQuadtreeSegemtNode
{
	BMQuadtreeSegemtNode()
	{
		_segmenyKey = INVALID_KEY;
	}
	HC_KEY _segmenyKey;
	HPoint _minPt;
	HPoint _maxPt;
};
class  _inerForNodeRegion
{
public:
	HPoint _minPt, _maxPt;
};
class BMQuadtreeManager;
//四叉树节点
class BMQuadtreeNode
{
public:
	BMQuadtreeNode()
	{
	}
	~BMQuadtreeNode()
	{
		if (m_pLeft_Up != NULL)
			delete m_pLeft_Up;
		if (m_pLeft_Down != NULL)
			delete m_pLeft_Down;
		if (m_pRight_Up != NULL)
			delete m_pRight_Up;
		if (m_pRight_Down != NULL)
			delete m_pRight_Down;
	}
	//
	int m_TesseNum;
	std::string m_Name;
	//四叉树节点包围盒---tile-boundingVolume
	HPoint m_MaxPt = HPoint(-1000000000.0, -1000000000.0, -1000000000.0);
	HPoint m_MinPt = HPoint(1000000000.0, 1000000000.0, 1000000000.0);
	HPoint m_CenterPt;
	vector<BMQuadtreeSegemtNode> m_vSegmentNodes;
	//四叉树节点内容包围盒---tile-content-boundingVolume
	HPoint m_ContentMaxPt = HPoint(-1000000000.0, -1000000000.0, -1000000000.0);
	HPoint m_ContentMinPt = HPoint(1000000000.0, 1000000000.0, 1000000000.0);
	/*
	leftUp    | rightUp
	----------------------
	leftDown  | rightDown
	*/
	BMQuadtreeNode* m_pLeft_Up = nullptr;
	BMQuadtreeNode* m_pLeft_Down = nullptr;
	BMQuadtreeNode* m_pRight_Up = nullptr;
	BMQuadtreeNode* m_pRight_Down = nullptr;
	BMQuadtreeNode* m_pParent = nullptr;
	BMQuadtreeManager* m_pTreeMana = nullptr;
	//
	void SetBoundAndCenter(const HPoint& minPt, const HPoint& maxPt);
	void InitChildNode();
	//
	BMQuadtreeNode* GetShellLocationOctreeNode(const HPoint& minPt, const HPoint& maxPt);
	//
	//每一个BMQuadtreeNode（四叉树节点 转换为一个 b3dm文件）
	GLTF::BMMesh m_BMMesh;
	vector<BMShellPrimitive*> m_vShellPrimitives;
	//由m_vSegmentNodes 中段信息构建 m_BMMesh
	void CreateBMMesh(float offsetX, float offsetY, float offsetZ);
	//
	bool m_Invalide = false;
	//
	float ComputeGeometricError();
	//
	void writeJSON(void* writer, bool inOBJ = false);
	void ComputeBoxBoundingVolume(float box[12]);
	void ComputeContentBoxBoundingVolume(float box[12]);
};
class BMQuadtreeManager
{
public:
	BMQuadtreeManager(HC_KEY rootKey)
	{
		m_Root = NULL;
		m_RootKey = rootKey;
		m_HasCreate = false;
	}
	BMQuadtreeManager()
	{
		m_Root = NULL;
		m_RootKey = INVALID_KEY;
		m_HasCreate = false;
	}
	~BMQuadtreeManager()
	{
		if (m_Root) delete m_Root;
		//
		auto it = m_NodeRegionMaps.begin();
		while (it != m_NodeRegionMaps.end())
		{
			delete it->second;
			it++;
		}
	}
	//
	BMQuadtreeNode* m_Root;
	HC_KEY m_RootKey;
	HC_KEY m_HoopsModelKey;// = m_pViewer->GetModel()->GetModelKey();
	bool m_HasCreate;
	GLTF::BMVertex m_Cartesian;//模型笛卡尔坐标
	std::string m_TileSetFileFolder = "D:\\";
	int m_TesseNum;
	//
	std::string m_DBFilePath;
	double m_transform[16];
	std::map<std::string, std::string> m_ImageMaps;
	
	//节点GUID与其包围盒unorderedmap
	unordered_map<string, _inerForNodeRegion*> m_NodeRegionMaps;
	//
	SqliteHelper m_SQLHelper;
	//向数据库 表model_tree中添加 centerX centerY centerZ radius 字段
	//向数据库 表object_type中添加 tileGUID 字段
	bool AddNewDBColumn(std::string& tileGUID);
	//创建四叉树
	bool CreateQuadtree();
	//
	void CreatImageMap();
	//
	void CreateBMMesh(BMQuadtreeNode* _pNode);
private:
	void InitRootNode();
	void AddNode(const HPoint& minPt, const HPoint& maxPt, HC_KEY segmentKey);
	void ParseSegment(HC_KEY segmentKey);
	//
public:
	std::string MultiByteToUTF8(std::string& mutlStr);
	//matrix 右剩 pt
	void MatrixMultiplyVector(double x, double y, double z, double& outX, double& outY, double& outZ);
};

