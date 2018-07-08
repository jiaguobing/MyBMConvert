#pragma once
#include "../../BM23DTile/base.h"
#include "../BMFile/BMNode.h"
#include "../BMFile/TessellationCylinder.h"
#include "vector"
using namespace std;
//四叉树节点 中包含的内容---segments--->
class LODBMShellPrimitive
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
	vector<LODBMShellPrimitive*> Subdivide(float offsetX, float offsetY, float offsetZ);
private:
	bool extractRGBNew(char* pRGB, float& fRed, float& fGreen, float& fBlue);
	GLTF::BMVertex ComputeNormal(GLTF::BMVertex& p0, GLTF::BMVertex& p1, GLTF::BMVertex& p2);
	HC_KEY CreateShellByCylinder();
};
struct LODBMQuadtreeSegemtNode
{
	HC_KEY _segmenyKey = INVALID_KEY;
	HPoint _minPt;
	HPoint _maxPt;
};
class LODBMQuadtreeManager;
//四叉树节点
class LODBMQuadtreeNode
{
public:
	LODBMQuadtreeNode()
	{
	}
	~LODBMQuadtreeNode()
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
	int m_LodLayer;
	std::string m_Name;
	//四叉树节点包围盒---tile-boundingVolume
	HPoint m_MaxPt = HPoint(-1000000000.0, -1000000000.0, -1000000000.0);
	HPoint m_MinPt = HPoint(1000000000.0, 1000000000.0, 1000000000.0);
	HPoint m_CenterPt;
	vector<LODBMQuadtreeSegemtNode> m_vSegmentNodes;
	//四叉树节点内容包围盒---tile-content-boundingVolume
	HPoint m_ContentMaxPt = HPoint(-1000000000.0, -1000000000.0, -1000000000.0);
	HPoint m_ContentMinPt = HPoint(1000000000.0, 1000000000.0, 1000000000.0);
	/*
	leftUp    | rightUp
	----------------------
	leftDown  | rightDown
	*/
	LODBMQuadtreeNode* m_pLeft_Up = nullptr;
	LODBMQuadtreeNode* m_pLeft_Down = nullptr;
	LODBMQuadtreeNode* m_pRight_Up = nullptr;
	LODBMQuadtreeNode* m_pRight_Down = nullptr;
	LODBMQuadtreeNode* m_pParent = nullptr;
	LODBMQuadtreeManager* m_pTreeMana = nullptr;
	//
	void SetBoundAndCenter(const HPoint& minPt, const HPoint& maxPt);
	void InitChildNode();
	//
	LODBMQuadtreeNode* GetShellLocationOctreeNode(const HPoint& midPt,int lodLayer);
	//
	//每一个BMQuadtreeNode（四叉树节点 转换为一个 b3dm文件）
	GLTF::BMMesh m_BMMesh;
	vector<LODBMShellPrimitive*> m_vShellPrimitives;
	//由m_vSegmentNodes 中段信息构建 m_BMMesh
	bool m_HasCreateBMMesh = false;
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
class LODBMQuadtreeManager
{
public:
	LODBMQuadtreeManager(HC_KEY rootKey)
	{
		m_Root = NULL;
		m_RootKey = rootKey;
		m_HasCreate = false;
	}
	LODBMQuadtreeManager()
	{
		m_Root = NULL;
		m_RootKey = INVALID_KEY;
		m_HasCreate = false;
	}
	~LODBMQuadtreeManager()
	{
		if (m_Root) delete m_Root;
	}
	//
	LODBMQuadtreeNode* m_Root;
	HC_KEY m_RootKey;
	HC_KEY m_HoopsModelKey;// = m_pViewer->GetModel()->GetModelKey();
	bool m_HasCreate;
	GLTF::BMVertex m_Cartesian;//模型笛卡尔坐标
	std::string m_TileSetFileFolder = "D:\\";
	int m_TesseNum;
	//
	vector<std::string> m_vLODFilePaths;
	std::map<std::string, std::string> m_ImageMaps;
	//创建四叉树
	bool CreateQuadtree();
	//
	void CreatImageMap();
	//
	void CreateBMMesh(LODBMQuadtreeNode* _pNode);
private:
	void InitRootNode();
	void AddNode(const HPoint& minPt, const HPoint& maxPt, HC_KEY segmentKey,int lodLayer);
	void ParseSegment(HC_KEY segmentKey, int lodLayer);
};

