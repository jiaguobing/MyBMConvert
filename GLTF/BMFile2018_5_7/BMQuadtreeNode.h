#pragma once
#include "HoopsFile/Viewer.h"
#include "BMNode.h"
using namespace Simu;
//四叉树节点 中包含的内容---segments--->
class BMShellPrimitive
{
public:
	HC_KEY m_shellKey;
	std::string m_segmentName;
	//
	vector<GLTF::BMVertex> m_vPositions;
	vector<GLTF::BMVertex> m_vNormals;
	GLTF::BMColor m_Color;
	vector<GLTF::BMTriangle> m_vTris;
	void ComputeContent();
private:
	bool extractRGBNew(char* pRGB, float& fRed, float& fGreen, float& fBlue);
	GLTF::BMVertex ComputeNormal(GLTF::BMVertex& p0, GLTF::BMVertex& p1, GLTF::BMVertex& p2);
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
//四叉树节点
class BMQuadtreeNode
{
public:
	BMQuadtreeNode()
	{
		m_pLeft_Up = nullptr;
		m_pLeft_Down = nullptr;
		m_pRight_Up = nullptr;
		m_pRight_Down = nullptr;
		//
		m_MaxPt = HPoint(-1000000000.0, -1000000000.0, -1000000000.0);
		m_MinPt = HPoint(1000000000.0, 1000000000.0, 1000000000.0);
		m_pParent = nullptr;
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
	std::string m_Name;
	HPoint m_MaxPt;
	HPoint m_MinPt;
	HPoint m_CenterPt;
	vector<BMQuadtreeSegemtNode> m_vSegmentNodes;
	/*
	leftUp    | rightUp
	----------------------
	leftDown  | rightDown
	*/
	BMQuadtreeNode* m_pLeft_Up;
	BMQuadtreeNode* m_pLeft_Down;
	BMQuadtreeNode* m_pRight_Up;
	BMQuadtreeNode* m_pRight_Down;
	BMQuadtreeNode* m_pParent;
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
	void CreateBMMesh();
	//
	bool m_Invalide = false;
	//
	float ComputeGeometricError();
	//
	void writeJSON(void* writer, bool inOBJ = false);
	void ComputeBoxBoundingVolume(float box[12]);
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
	}
	//
	BMQuadtreeNode* m_Root;
	HC_KEY m_RootKey;
	bool m_HasCreate;
	//创建四叉树
	bool CreateQuadtree();
	//
	void CreateBMMesh(BMQuadtreeNode* _pNode);
private:
	void InitRootNode();
	void AddNode(const HPoint& minPt, const HPoint& maxPt, HC_KEY segmentKey);
	void ParseSegment(HC_KEY segmentKey);
};

