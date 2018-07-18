#include<ctype.h>
#include "ObjQuadtreeNode.h"
#include <windows.h>  
#include <locale.h>
#include "string"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream> 
void ObjShellPrimitive::ComputeNormals()
{
	if (m_TextureImagePath != "") return;
	//
	std::map<int, std::vector<int>> _mapForNormal;
	for (size_t i = 0; i < m_vTris.size(); ++i)
	{
		GLTF::BMTriangle& _tri = m_vTris[i];
		_mapForNormal[_tri.m_index0].push_back(i);
		_mapForNormal[_tri.m_index1].push_back(i);
		_mapForNormal[_tri.m_index2].push_back(i);
	}
	//
	int ptNum = int(m_vPositions.size());
	for (int i = 0; i < ptNum; ++i)
	{
		GLTF::BMVertex _normal;
		std::vector<int> &v = _mapForNormal[i];
		size_t vSize = v.size();
		if (vSize == 0)
		{
			//若shell中数据没错误，理论上不会出现此情况
			m_vNormals.push_back(GLTF::BMVertex(1.0f, 0.0f, 0.0f));
		}
		else
		{
			vector<GLTF::BMTriangle> vTempTris;
			for (size_t j = 0; j < vSize; ++j)
			{
				vTempTris.push_back(m_vTris[v[j]]);
			}
			GLTF::BMVertex _tempNor;
			for (auto& tri : vTempTris)
			{
				_tempNor = ComputeNormal(m_vPositions[tri.m_index0], m_vPositions[tri.m_index1], m_vPositions[tri.m_index2]);
				_normal = _normal + _tempNor;
			}
			_normal = _normal / vTempTris.size();
			_normal.Normalization();
			m_vNormals.push_back(_normal);
		}
	}
}
GLTF::BMVertex ObjShellPrimitive::ComputeNormal(GLTF::BMVertex& p0, GLTF::BMVertex& p1, GLTF::BMVertex& p2)
{
	GLTF::BMVertex n0 = p1 - p0;
	GLTF::BMVertex n1 = p2 - p0;
	return n0.Cross(n1).Normalization();
}
//////////////////////////////////////////////////////////////////////////
//此函数有很大的优化空间---没有处理 冗余点---JGB2018-7-18不想写了
void ObjContent::ComputeShellPris(double offsetX, double offsetY, double offsetZ)
{
	m_MinPt = HPoint(100000.0f, 100000.0f, 100000.0f);
	m_MaxPt = HPoint(-100000.0f, -100000.0f, -100000.0f);
	//
	vector<GLTF::BMVertex> _vPositions;
	vector<GLTF::BMVertex> _vCoords;
	GLTF::BMVertex _ver;
	GLTF::BMTriangle _tri;
	ObjShellPrimitive* _pNewPrt = NULL;
	//
	ifstream  ifile(m_ObjFilePath.c_str(), ios::in);
	string _Line;
	string tempStr;
	string tempStr0;
	string tempStr1;
	string tempStr2;
	string mtlName;
	ObjMtl _mtl;
	while (getline(ifile, _Line))
	{
		istringstream istr(_Line);
		istr >> tempStr;
		if (tempStr == "v")
		{
			istr >> _ver.m_x;
			_ver.m_x -= offsetX;
			istr >> _ver.m_z;
			_ver.m_z -= offsetZ;
			istr >> _ver.m_y;
			_ver.m_y = -_ver.m_y - offsetY;
			//
			_vPositions.push_back(_ver);
			//
			m_MinPt.x = min(m_MinPt.x, _ver.m_x); m_MinPt.y = min(m_MinPt.y, _ver.m_y); m_MinPt.z = min(m_MinPt.z, _ver.m_z);
			m_MaxPt.x = max(m_MaxPt.x, _ver.m_x); m_MaxPt.y = max(m_MaxPt.y, _ver.m_y); m_MaxPt.z = max(m_MaxPt.z, _ver.m_z);
		}
		else if (tempStr == "vt")
		{
			istr >> _ver.m_u;
			istr >> _ver.m_v;
			//
			_vCoords.push_back(_ver);
		}
		else if (tempStr == "usemtl")
		{
			istr >> mtlName;
			_mtl = m_pMana->m_MtlMaps[mtlName];
			_pNewPrt = new ObjShellPrimitive;
			_pNewPrt->m_Color = _mtl.m_Color;
			_pNewPrt->m_TextureImagePath = _mtl.m_TextureImagePath;
		}
		else if (tempStr == "f")
		{
			istr >> tempStr0;
			istr >> tempStr1;
			istr >> tempStr2;
			//
			istringstream istr0(tempStr0);
			getline(istr0, tempStr, '/');//
			int posIndex0 = atoi(tempStr.c_str()) - 1;
			getline(istr0, tempStr, '/');//
			int cooIndex0 = atoi(tempStr.c_str()) - 1;
			//
			istringstream istr1(tempStr1);
			getline(istr1, tempStr, '/');//
			int posIndex1 = atoi(tempStr.c_str()) - 1;
			getline(istr1, tempStr, '/');//
			int cooIndex1 = atoi(tempStr.c_str()) - 1;
			//
			istringstream istr2(tempStr2);
			getline(istr2, tempStr, '/');//
			int posIndex2 = atoi(tempStr.c_str()) - 1;
			getline(istr2, tempStr, '/');//
			int cooIndex2 = atoi(tempStr.c_str()) - 1;
			//
			int triIndex0, triIndex1, triIndex2;
			//
			if (cooIndex0 == -1)
			{
				triIndex0 = _pNewPrt->m_vPositions.size();
				_pNewPrt->m_vPositions.push_back(_vPositions[posIndex0]);
				//
				triIndex1 = _pNewPrt->m_vPositions.size();
				_pNewPrt->m_vPositions.push_back(_vPositions[posIndex1]);
				//
				triIndex2 = _pNewPrt->m_vPositions.size();
				_pNewPrt->m_vPositions.push_back(_vPositions[posIndex2]);
			}
			else
			{
				triIndex0 = _pNewPrt->m_vPositions.size();
				_pNewPrt->m_vPositions.push_back(_vPositions[posIndex0]);
				_pNewPrt->m_vCoords.push_back(_vCoords[cooIndex0]);
				//
				triIndex1 = _pNewPrt->m_vPositions.size();
				_pNewPrt->m_vPositions.push_back(_vPositions[posIndex1]);
				_pNewPrt->m_vCoords.push_back(_vCoords[cooIndex1]);
				//
				triIndex2 = _pNewPrt->m_vPositions.size();
				_pNewPrt->m_vPositions.push_back(_vPositions[posIndex2]);
				_pNewPrt->m_vCoords.push_back(_vCoords[cooIndex2]);
			}
			//////////////////////////////////////////////////////////////////////////
			_tri.m_index0 = triIndex0; _tri.m_index1 = triIndex1; _tri.m_index2 = triIndex2;
			_pNewPrt->m_vTris.push_back(_tri);
		}
		else if (tempStr == "#")
		{
			if (_pNewPrt != NULL)
			{
				_pNewPrt->ComputeNormals();
				m_vpShellPris.push_back(_pNewPrt);
				_pNewPrt = NULL;
			}
		}
	}
	ifile.close();
}
//////////////////////////////////////////////////////////////////////////
void ObjQuadtreeNode::SetBoundAndCenter(const HPoint& minPt, const HPoint& maxPt)
{
	m_MaxPt = maxPt;
	m_MinPt = minPt;
	m_CenterPt = (m_MaxPt + m_MinPt) / 2;
}
void ObjQuadtreeNode::InitChildNode()
{
	m_pLeft_Up = new ObjQuadtreeNode;
	m_pLeft_Up->SetBoundAndCenter(HPoint(m_MinPt.x, m_CenterPt.y, m_MinPt.z), HPoint(m_CenterPt.x, m_MaxPt.y, m_MaxPt.z));
	m_pLeft_Up->m_pParent = this;
	m_pLeft_Up->m_Name = m_Name + "_0";
	m_pLeft_Up->m_pTreeMana = m_pTreeMana;
	//
	m_pLeft_Down = new ObjQuadtreeNode;
	m_pLeft_Down->SetBoundAndCenter(HPoint(m_MinPt.x, m_MinPt.y, m_MinPt.z), HPoint(m_CenterPt.x, m_CenterPt.y, m_MaxPt.z));
	m_pLeft_Down->m_pParent = this;
	m_pLeft_Down->m_Name = m_Name + "_1";
	m_pLeft_Down->m_pTreeMana = m_pTreeMana;
	//
	m_pRight_Up = new ObjQuadtreeNode;
	m_pRight_Up->SetBoundAndCenter(HPoint(m_CenterPt.x, m_CenterPt.y, m_MinPt.z), HPoint(m_MaxPt.x, m_MaxPt.y, m_MaxPt.z));
	m_pRight_Up->m_pParent = this;
	m_pRight_Up->m_Name = m_Name + "_3";
	m_pRight_Up->m_pTreeMana = m_pTreeMana;
	//
	m_pRight_Down = new ObjQuadtreeNode;
	m_pRight_Down->SetBoundAndCenter(HPoint(m_CenterPt.x, m_MinPt.y, m_MinPt.z), HPoint(m_MaxPt.x, m_CenterPt.y, m_MaxPt.z));
	m_pRight_Down->m_pParent = this;
	m_pRight_Down->m_Name = m_Name + "_2";
	m_pRight_Down->m_pTreeMana = m_pTreeMana;
}
ObjQuadtreeNode* ObjQuadtreeNode::GetShellLocationOctreeNode(const HPoint& minPt, const HPoint& maxPt)
{
	if ((m_MaxPt.x - m_MinPt.x) < 100)
	{
		return this;
	}
	if (m_pLeft_Up)
	{
		if (m_pLeft_Up->m_MinPt.x <= minPt.x && m_pLeft_Up->m_MinPt.y <= minPt.y &&
			m_pLeft_Up->m_MaxPt.x >= maxPt.x && m_pLeft_Up->m_MaxPt.y >= maxPt.y)
		{
			return m_pLeft_Up->GetShellLocationOctreeNode(minPt, maxPt);
		}
		else if (m_pLeft_Down->m_MinPt.x <= minPt.x && m_pLeft_Down->m_MinPt.y <= minPt.y &&
			m_pLeft_Down->m_MaxPt.x >= maxPt.x && m_pLeft_Down->m_MaxPt.y >= maxPt.y)
		{
			return m_pLeft_Down->GetShellLocationOctreeNode(minPt, maxPt);
		}
		else if (m_pRight_Up->m_MinPt.x <= minPt.x && m_pRight_Up->m_MinPt.y <= minPt.y &&
			m_pRight_Up->m_MaxPt.x >= maxPt.x && m_pRight_Up->m_MaxPt.y >= maxPt.y)
		{
			return m_pRight_Up->GetShellLocationOctreeNode(minPt, maxPt);
		}
		else if (m_pRight_Down->m_MinPt.x <= minPt.x && m_pRight_Down->m_MinPt.y <= minPt.y &&
			m_pRight_Down->m_MaxPt.x >= maxPt.x && m_pRight_Down->m_MaxPt.y >= maxPt.y)
		{
			return m_pRight_Down->GetShellLocationOctreeNode(minPt, maxPt);
		}
		else
			return this;
	}
	else
	{
		bool _hasInitChild = false;
		HPoint _left_up_minPt(m_MinPt.x, m_CenterPt.y, m_MinPt.z), _left_up_maxPt(m_CenterPt.x, m_MaxPt.y, m_MaxPt.z);
		HPoint _right_up_minPt(m_CenterPt.x, m_CenterPt.y, m_MinPt.z), _right_up_maxPt(m_MaxPt.x, m_MaxPt.y, m_MaxPt.z);
		HPoint _left_down_minPt(m_MinPt.x, m_MinPt.y, m_MinPt.z), _left_down_maxPt(m_CenterPt.x, m_CenterPt.y, m_MaxPt.z);
		HPoint _right_down_minPt(m_CenterPt.x, m_MinPt.y, m_MinPt.z), _right_down_maxPt(m_MaxPt.x, m_CenterPt.y, m_MaxPt.z);
		//
		if (_left_up_minPt.x <= minPt.x && _left_up_minPt.y <= minPt.y &&
			_left_up_maxPt.x >= maxPt.x && _left_up_maxPt.y >= maxPt.y)
		{
			InitChildNode();
			_hasInitChild = true;
			return m_pLeft_Up->GetShellLocationOctreeNode(minPt, maxPt);
		}
		else if (_right_up_minPt.x <= minPt.x && _right_up_minPt.y <= minPt.y &&
			_right_up_maxPt.x >= maxPt.x && _right_up_maxPt.y >= maxPt.y)
		{
			if (!_hasInitChild)
			{
				_hasInitChild = true;
				InitChildNode();
			}
			return m_pRight_Up->GetShellLocationOctreeNode(minPt, maxPt);
		}
		else if (_left_down_minPt.x <= minPt.x && _left_down_minPt.y <= minPt.y &&
			_left_down_maxPt.x >= maxPt.x && _left_down_maxPt.y >= maxPt.y)
		{
			if (!_hasInitChild)
			{
				_hasInitChild = true;
				InitChildNode();
			}
			return m_pLeft_Down->GetShellLocationOctreeNode(minPt, maxPt);
		}
		else if (_right_down_minPt.x <= minPt.x && _right_down_minPt.y <= minPt.y &&
			_right_down_maxPt.x >= maxPt.x && _right_down_maxPt.y >= maxPt.y)
		{
			if (!_hasInitChild)
			{
				_hasInitChild = true;
				InitChildNode();
			}
			return m_pRight_Down->GetShellLocationOctreeNode(minPt, maxPt);
		}
		else
			return this;
	}
}
void ObjQuadtreeNode::CreateBMMesh()
{
	if (m_vShellPrimitives.size() == 0)
	{
		m_Invalide = false;
		return;
	}
	m_Invalide = true;
	for (auto _pObj : m_vShellPrimitives)
	{
		m_BMMesh.m_vBatchNames.push_back(_pObj->m_ObjFileName);
	}
	m_BMMesh.m_strName = m_Name;
	//将BMShellPrimitive 转换为 GLTF::BMPrimitive
	int index = 0;
	for (auto _pObj : m_vShellPrimitives)
	{
		for (auto _pObjShell : _pObj->m_vpShellPris)
		{
			GLTF::BMPrimitive _bmPri;
			_bmPri.m_vPositions.swap(_pObjShell->m_vPositions);
			_bmPri.m_vBatchIDs.reserve(_pObjShell->m_vPositions.size());
			_bmPri.m_vNormals.swap(_pObjShell->m_vNormals);
			size_t _verNum = _bmPri.m_vPositions.size();
			for (size_t i = 0; i < _verNum; ++i)
			{
				_bmPri.m_vBatchIDs.push_back(index);
			}
			//
			_bmPri.m_vIndexs.reserve(_pObjShell->m_vTris.size() * 3);
			for (auto& tri : _pObjShell->m_vTris)
			{
				_bmPri.m_vIndexs.push_back(tri.m_index0);
				_bmPri.m_vIndexs.push_back(tri.m_index1);
				_bmPri.m_vIndexs.push_back(tri.m_index2);
			}
			//
			_bmPri.m_Color = _pObjShell->m_Color;
			_bmPri.m_TextureImagePath = _pObjShell->m_TextureImagePath;
			_bmPri.m_vCoords.swap(_pObjShell->m_vCoords);
			//
			m_BMMesh.m_vPrimitives.push_back(_bmPri);
		}
		//
		index++;
		delete _pObj;
	}
	m_vShellPrimitives.clear();
}
float ObjQuadtreeNode::ComputeGeometricError()
{
	//float ge = max(m_ContentMaxPt.x - m_ContentMinPt.x, m_ContentMaxPt.y - m_ContentMinPt.y);
	float ge = max(m_MaxPt.x - m_MinPt.x, m_MaxPt.y - m_MinPt.y);
	return ge;
}
void ObjQuadtreeNode::ComputeContentBoxBoundingVolume(float box[12])
{
	GLTF::BMVertex _center = GLTF::BMVertex((m_ContentMaxPt.x + m_ContentMinPt.x) / 2.0f, (m_ContentMaxPt.y + m_ContentMinPt.y) / 2.0f, (m_ContentMaxPt.z + m_ContentMinPt.z) / 2.0f);
	GLTF::BMVertex _bouding = GLTF::BMVertex(m_ContentMaxPt.x - m_ContentMinPt.x, m_ContentMaxPt.y - m_ContentMinPt.y, m_ContentMaxPt.z - m_ContentMinPt.z);
	_bouding = _bouding / 2.0f;
	//
	box[0] = _center.m_x; box[1] = _center.m_y; box[2] = _center.m_z;
	box[3] = _bouding.m_x; box[4] = 0.0f; box[5] = 0.0f;
	box[6] = 0.0f; box[7] = _bouding.m_y; box[8] = 0.0f;
	box[9] = 0.0f; box[10] = 0.0f; box[11] = _bouding.m_z;
}
void ObjQuadtreeNode::ComputeBoxBoundingVolume(float box[12])
{
	GLTF::BMVertex _center = GLTF::BMVertex((m_MaxPt.x + m_MinPt.x) / 2.0f, (m_MaxPt.y + m_MinPt.y) / 2.0f, (m_MaxPt.z + m_MinPt.z) / 2.0f);
	GLTF::BMVertex _bouding = GLTF::BMVertex(m_MaxPt.x - m_MinPt.x, m_MaxPt.y - m_MinPt.y, m_MaxPt.z - m_MinPt.z);
	_bouding = _bouding / 2.0f;
	//
	box[0] = _center.m_x; box[1] = _center.m_y; box[2] = _center.m_z;
	box[3] = _bouding.m_x; box[4] = 0.0f; box[5] = 0.0f;
	box[6] = 0.0f; box[7] = _bouding.m_y; box[8] = 0.0f;
	box[9] = 0.0f; box[10] = 0.0f; box[11] = _bouding.m_z;
}
#include "../rapidjson/stringbuffer.h"
#include "../rapidjson/writer.h"
void ObjQuadtreeNode::writeJSON(void* writer, bool inOBJ /*= false*/)
{
	if (!m_Invalide && m_pLeft_Down == nullptr) return;
	rapidjson::Writer<rapidjson::StringBuffer>* jsonWriter = (rapidjson::Writer<rapidjson::StringBuffer>*)writer;
	if (inOBJ)
	{
		jsonWriter->StartObject();
	}
	//
	float boundingBox[12];
	float contentBoundingBox[12];
	ComputeContentBoxBoundingVolume(contentBoundingBox);
	ComputeBoxBoundingVolume(boundingBox);
	//boundingVolume
	jsonWriter->Key("boundingVolume");
	jsonWriter->StartObject();
	jsonWriter->Key("box");
	jsonWriter->StartArray();
	for (float tt : boundingBox)
	{
		jsonWriter->Double(tt);
	}
	jsonWriter->EndArray();
	jsonWriter->EndObject();
	//geometricError
	jsonWriter->Key("geometricError");
	double GR = ComputeGeometricError();
	if (m_pLeft_Down == nullptr)
		GR = 0.0;//叶子节点 geometricError = 0.0
	jsonWriter->Double(GR);
	//refine
	jsonWriter->Key("refine");
	jsonWriter->String("ADD");
	//content
	if (m_Invalide)
	{
		jsonWriter->Key("content");
		jsonWriter->StartObject();
		//
		std::string url = m_Name + ".b3dm";
		jsonWriter->Key("url");
		jsonWriter->String(url.c_str());
		//boundingVolume
		jsonWriter->Key("boundingVolume");
		jsonWriter->StartObject();
		jsonWriter->Key("box");
		jsonWriter->StartArray();
		for (float tt : contentBoundingBox)
		{
			jsonWriter->Double(tt);
		}
		jsonWriter->EndArray();
		jsonWriter->EndObject();
		//
		jsonWriter->EndObject();
	}
	//children
	if (m_pLeft_Down)
	{
		jsonWriter->Key("children");
		jsonWriter->StartArray();
		//
		m_pLeft_Down->writeJSON(writer, true);
		m_pLeft_Up->writeJSON(writer, true);
		m_pRight_Up->writeJSON(writer, true);
		m_pRight_Down->writeJSON(writer, true);
		//
		jsonWriter->EndArray();
	}
	//
	if (inOBJ)
	{
		jsonWriter->EndObject();
	}
}
//////////////////////////////////////////////////////////////////////////
void ObjQuadtreeManager::MatrixMultiplyVector(double x, double y, double z, double& outX, double& outY, double& outZ)
{
	outX = m_transform[0] * x + m_transform[4] * y + m_transform[8] * z + m_transform[12];
	outY = m_transform[1] * x + m_transform[5] * y + m_transform[9] * z + m_transform[13];
	outZ = m_transform[2] * x + m_transform[6] * y + m_transform[10] * z + m_transform[14];
}

void ObjQuadtreeManager::InitRootNode(GLTF::BMVertex& minPt, GLTF::BMVertex& maxPt)
{
	if (m_Root) return;
	HPoint _minPt, _maxPt;
	_minPt.x = minPt.m_x - m_Cartesian.m_x; _minPt.y = minPt.m_y - m_Cartesian.m_y; _minPt.z = minPt.m_z - m_Cartesian.m_z;
	_maxPt.x = maxPt.m_x - m_Cartesian.m_x; _minPt.y = maxPt.m_y - m_Cartesian.m_y; _minPt.z = maxPt.m_z - m_Cartesian.m_z;
	//
	//
	float Dx = _maxPt.x - _minPt.x;
	float Dy = _maxPt.y - _minPt.y;
	float _maxDXY = max(Dx, Dy) + 1.0f;
	HPoint _center = (_minPt + _maxPt)/2.0f;
	_maxPt.x = _center.x + _maxDXY / 2.0f;
	_maxPt.y = _center.y + _maxDXY / 2.0f;
	//
	_minPt.x = _center.x - _maxDXY / 2.0f;
	_minPt.y = _center.y - _maxDXY / 2.0f;
	//
	m_Root = new ObjQuadtreeNode;
	m_Root->m_MaxPt = _maxPt;
	m_Root->m_MinPt = _minPt;
	m_Root->m_Name = "0";
	m_Root->SetBoundAndCenter(_minPt, _maxPt);
	m_Root->m_pTreeMana = this;
}
void ObjQuadtreeManager::CreateBMMesh(ObjQuadtreeNode* _pNode)
{
	if (_pNode == NULL) return;
	_pNode->CreateBMMesh();
	if (_pNode->m_pLeft_Up)
	{
		CreateBMMesh(_pNode->m_pLeft_Up);
		CreateBMMesh(_pNode->m_pLeft_Down);
		CreateBMMesh(_pNode->m_pRight_Up);
		CreateBMMesh(_pNode->m_pRight_Down);
	}
}
std::string ObjQuadtreeManager::MultiByteToUTF8(std::string& mutlStr)
{
	/* 这个方法里，要先把GB2312字符串转换成UNICODE编码，再用UNICODE转UTF8,
	因为没有一种CodePage可以直接将GB2312转换成UTF8，所以这里就要先转UNICODE，再
	通过CP_UTF8进行转换，UTF8可以视为一种变长的多字节编码，虽说UTF8是对UNICODE字符集
	执行的一种编码形式，但其编码是采用1~6字节变长编码，所以可以视为多字节编码 */
	int nLen = MultiByteToWideChar(CP_ACP, 0, mutlStr.c_str(), -1, NULL, 0);
	WCHAR *pwszBuffer = new WCHAR[nLen];
	nLen = MultiByteToWideChar(CP_ACP, 0, mutlStr.c_str(), -1, pwszBuffer, nLen);
	// 再转换成UTF-8编码  
	// 刚开始用这两个API时，对这个CodePage的认识很模糊，为什么有的地方用CP_UTF8，有的用CP_ACP  
	nLen = WideCharToMultiByte(CP_UTF8, 0, pwszBuffer, -1, NULL, 0, NULL, NULL);
	char *pszBuffer = new char[nLen];
	nLen = WideCharToMultiByte(CP_UTF8, 0, pwszBuffer, -1, pszBuffer, nLen, NULL, NULL);
	std::string ste = string(pszBuffer);
	delete[]pszBuffer;
	delete[]pwszBuffer;
	return ste;
}

bool ObjQuadtreeManager::CreateQuadtree()
{
	if (m_HasCreate) return true;
	m_HasCreate = true;
	CreatMtlMap();
	//
	string Path = m_ObjFileFolder + "*.obj";
	std::string findFileName;
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = FindFirstFile(Path.c_str(), &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE)
		return false;
	do
	{
		if (FindFileData.cFileName[0] == '.')
			continue;
		if (!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			findFileName = FindFileData.cFileName;
			//
			ObjContent* pNewObjContent = new ObjContent;
			pNewObjContent->m_pMana = this;
			pNewObjContent->m_ObjFilePath = m_ObjFileFolder + findFileName;
			pNewObjContent->m_ObjFileName = findFileName.substr(1,findFileName.length()-5);
			pNewObjContent->ComputeShellPris(m_Cartesian.m_x, m_Cartesian.m_y, m_Cartesian.m_z);
			//
			ObjQuadtreeNode* _SON = m_Root->GetShellLocationOctreeNode(pNewObjContent->m_MinPt, pNewObjContent->m_MaxPt);
			_SON->m_vShellPrimitives.push_back(pNewObjContent);
		}
	} while (FindNextFile(hFind, &FindFileData));
	FindClose(hFind);
	//
	return true;
}
void ObjQuadtreeManager::CreatMtlMap()
{
	string _mtlFilePath = m_ObjFileFolder + ".mtl";
	ifstream  ifile(_mtlFilePath.c_str(), ios::in);
	string _Line;
	string tempStr;
	string tempStr0;
	string mtlName;
	ObjMtl _mtl;
	stringstream istr0;
	while (getline(ifile, _Line))
	{
		istr0.clear();
		istr0 << _Line;
		istr0 >> tempStr;
		if (tempStr == "newmtl")
		{
			istr0 >> mtlName;
			getline(ifile, _Line);//illum 3
			getline(ifile, _Line);//Ka 0 0 0
			getline(ifile, _Line);//Kd 1 1 1
			istr0.clear();
			istr0 << _Line;
			istr0 >> tempStr0;
			istr0 >> tempStr0;
			_mtl.m_Color.m_r = atof(tempStr0.c_str());
			istr0 >> tempStr0;
			_mtl.m_Color.m_g = atof(tempStr0.c_str());
			istr0 >> tempStr0;
			_mtl.m_Color.m_b = atof(tempStr0.c_str());
			//
			getline(ifile, _Line);//Ks 0 0 0
			getline(ifile, _Line);//Ns 0
			getline(ifile, _Line);//d 1
			getline(ifile, _Line);//Tf 1.0 1.0 1.0
			getline(ifile, _Line);//Ni 1.0
			getline(ifile, _Line);//map_Kd u_f004_t006_Mercantile_008.jpg
			if (_Line != "")
			{
				istr0.clear();
				istr0 << _Line;
				istr0 >> tempStr0;
				istr0 >> _mtl.m_TextureImagePath;
				_mtl.m_TextureImagePath = m_ObjFileFolder + _mtl.m_TextureImagePath;
			}
			m_MtlMaps[mtlName] = _mtl;
			_mtl.m_TextureImagePath = "";
		}
		tempStr = "";
	}
	ifile.close();
}
