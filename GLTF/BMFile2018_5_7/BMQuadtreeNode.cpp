#include "stdafx.h"
#include "BMQuadtreeNode.h"
void BMShellPrimitive::ComputeContent()
{
	char cFaceColor[SA_BUFFER_SIZE];
	int ptNum;
	int flistNum;
	HC_Show_Shell_Size(m_shellKey, &ptNum, &flistNum);
	HPoint* pPoint = new HPoint[ptNum];
	int* fList = new int[flistNum];
	HC_Show_Shell(m_shellKey, &ptNum, pPoint, &flistNum, fList);
	//
	HC_Open_Geometry(m_shellKey);
	if (HC_Show_Existence("color=(face=diffuse)"))
	{
		HC_Show_One_Color("face=diffuse", cFaceColor);
		if (!extractRGBNew(cFaceColor, m_Color.m_r, m_Color.m_g, m_Color.m_b))
			m_Color = GLTF::BMColor(0.0f, 1.0f, 0.0f);
	}
	else
		m_Color = GLTF::BMColor(0.0f, 1.0f, 0.0f);
	HC_Close_Geometry();
	//
	m_vPositions.reserve(ptNum);
	m_vNormals.reserve(ptNum);
	m_vTris.reserve(flistNum / 4);
	for (size_t i = 0; i < ptNum; ++i)
	{
		HPoint& pt = pPoint[i];
		m_vPositions.push_back(GLTF::BMVertex(pt.x, pt.y, pt.z));
	}
	for (size_t i = 0; i < flistNum;)
	{
		int numOfPt = fList[i];
		if (numOfPt == 3)
		{
			m_vTris.push_back(GLTF::BMTriangle(fList[i + 1], fList[i + 2], fList[i + 3]));
		}
		else
		{
			//若 不是三角形
			vector<HPoint> _tempPts;
			for (int j = 0; j < numOfPt; ++j)
			{
				_tempPts.push_back(pPoint[fList[i + j]]);
			}
			//-----未处理
		}
		//
		i = i + fList[i] + 1;
	}
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
	//
	delete[] pPoint;
	delete[] fList;
}
GLTF::BMVertex BMShellPrimitive::ComputeNormal(GLTF::BMVertex& p0, GLTF::BMVertex& p1, GLTF::BMVertex& p2)
{
	GLTF::BMVertex n0 = p1 - p0;
	GLTF::BMVertex n1 = p2 - p0;
	return n0.Cross(n1).Normalization();
}
bool BMShellPrimitive::extractRGBNew(char* pRGB, float& fRed, float& fGreen, float& fBlue)
{
	size_t nLength = strlen(pRGB);
	for (size_t i = 0; i < nLength; i++) { pRGB[i] = (char)tolower((int)pRGB[i]); }
	std::string strRGB = pRGB;
	std::string::size_type equal = strRGB.find('=');
	if (equal != strRGB.npos)
	{
		fRed = atof(strRGB.c_str() + equal + 1);
	}
	else
		return false;
	equal = strRGB.find('=', equal + 1);
	if (equal != strRGB.npos)
	{
		fGreen = atof(strRGB.c_str() + equal + 1);
	}
	else
		return false;
	equal = strRGB.find('=', equal + 1);
	if (equal != strRGB.npos)
	{
		fBlue = atof(strRGB.c_str() + equal + 1);
	}
	else
		return false;
	return true;
}
//////////////////////////////////////////////////////////////////////////
void BMQuadtreeNode::SetBoundAndCenter(const HPoint& minPt, const HPoint& maxPt)
{
	m_MaxPt = maxPt;
	m_MinPt = minPt;
	m_CenterPt = (m_MaxPt + m_MinPt) / 2;
}
void BMQuadtreeNode::InitChildNode()
{
	m_pLeft_Up = new BMQuadtreeNode;
	m_pLeft_Up->SetBoundAndCenter(HPoint(m_MinPt.x, m_CenterPt.y, m_MinPt.z), HPoint(m_CenterPt.x, m_MaxPt.y, m_MaxPt.z));
	m_pLeft_Up->m_pParent = this;
	m_pLeft_Up->m_Name = m_Name + "_0";
	//
	m_pLeft_Down = new BMQuadtreeNode;
	m_pLeft_Down->SetBoundAndCenter(HPoint(m_MinPt.x, m_MinPt.y, m_MinPt.z), HPoint(m_CenterPt.x, m_CenterPt.y, m_MaxPt.z));
	m_pLeft_Down->m_pParent = this;
	m_pLeft_Down->m_Name = m_Name + "_1";
	//
	m_pRight_Up = new BMQuadtreeNode;
	m_pRight_Up->SetBoundAndCenter(HPoint(m_CenterPt.x, m_CenterPt.y, m_MinPt.z), HPoint(m_MaxPt.x, m_MaxPt.y, m_MaxPt.z));
	m_pRight_Up->m_pParent = this;
	m_pRight_Up->m_Name = m_Name + "_3";
	//
	m_pRight_Down = new BMQuadtreeNode;
	m_pRight_Down->SetBoundAndCenter(HPoint(m_CenterPt.x, m_MinPt.y, m_MinPt.z), HPoint(m_MaxPt.x, m_CenterPt.y, m_MaxPt.z));
	m_pRight_Down->m_pParent = this;
	m_pRight_Down->m_Name = m_Name + "_2";
}
BMQuadtreeNode* BMQuadtreeNode::GetShellLocationOctreeNode(const HPoint& minPt, const HPoint& maxPt)
{
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
void BMQuadtreeNode::CreateBMMesh()
{
	int count;
	HC_KEY _shellKey;
	char _geoType[64];
	char segName[MVO_BUFFER_SIZE];
	for (auto & SegemtNode : m_vSegmentNodes)
	{
		HC_Open_Segment_By_Key(SegemtNode._segmenyKey);
		//
		HC_Show_Segment_Name(SegemtNode._segmenyKey, segName);
		//
		HC_Begin_Contents_Search(".", "geometry");
		{
			HC_Show_Contents_Count(&count);
			for (int j = 0; j < count; ++j)
			{
				HUtility::FindContents(_shellKey);
				HC_Show_Key_Type(_shellKey, _geoType);
				if (0 == strcmp("shell", _geoType))
				{
					BMShellPrimitive* pNewShellPri = new BMShellPrimitive();
					pNewShellPri->m_shellKey = _shellKey;
					pNewShellPri->m_segmentName = std::string(segName);
					m_vShellPrimitives.push_back(pNewShellPri);
				}
			}
		}
		HC_End_Contents_Search();
		HC_Close_Segment();
	}
	m_vSegmentNodes.clear();
	//////////////////////////////////////////////////////////////////////////
	if (m_vShellPrimitives.size() == 0)
	{
		m_Invalide = false;
		return;
	}
	m_Invalide = true;
	for (auto _pShellPri : m_vShellPrimitives)
	{
		_pShellPri->ComputeContent();
	}
	//////////////////////////////////////////////////////////////////////////
	std::set<std::string> tempNams;
	unsigned int vertNum = 0;
	for (auto _pShellPri : m_vShellPrimitives)
	{
		vertNum += _pShellPri->m_vPositions.size();
		if (tempNams.insert(_pShellPri->m_segmentName).second)
		{
			m_BMMesh.m_vBatchNames.push_back(_pShellPri->m_segmentName);
		}
	}
	m_BMMesh.m_vBatchIDs.reserve(vertNum);
	m_BMMesh.m_vNormals.reserve(vertNum);
	m_BMMesh.m_vPositions.reserve(vertNum);
	m_BMMesh.m_strName = m_Name;
	//--------------------------------------------------只处理颜色---未处理纹理
	unsigned int beginIndex = 0;
	for (auto _pShellPri : m_vShellPrimitives)
	{
		m_BMMesh.m_vPositions.insert(m_BMMesh.m_vPositions.end(), _pShellPri->m_vPositions.begin(), _pShellPri->m_vPositions.end());
		m_BMMesh.m_vNormals.insert(m_BMMesh.m_vNormals.end(), _pShellPri->m_vNormals.begin(), _pShellPri->m_vNormals.end());
		//
		auto it = find(m_BMMesh.m_vBatchNames.begin(), m_BMMesh.m_vBatchNames.end(), _pShellPri->m_segmentName);
		size_t _id = it - m_BMMesh.m_vBatchNames.begin();
		size_t _verNum = _pShellPri->m_vPositions.size();
		for (size_t i = 0; i < _verNum; ++i)
		{
			m_BMMesh.m_vBatchIDs.push_back(_id);
		}
		//
		GLTF::BMPrimitive _bmPri;
		_bmPri.m_Color = _pShellPri->m_Color;
		for (auto& tri : _pShellPri->m_vTris)
		{
			_bmPri.m_vIndexs.push_back(tri.m_index0 + beginIndex);
			_bmPri.m_vIndexs.push_back(tri.m_index1 + beginIndex);
			_bmPri.m_vIndexs.push_back(tri.m_index2 + beginIndex);
		}
		beginIndex += _verNum;
		m_BMMesh.m_vPrimitives.push_back(_bmPri);
	}
	//
	for (auto ii : m_vShellPrimitives)
	{
		delete ii;
	}
	m_vShellPrimitives.clear();
}
float BMQuadtreeNode::ComputeGeometricError()
{
	float ge = max(max(m_MaxPt.x - m_MinPt.x, m_MaxPt.y - m_MinPt.y), m_MaxPt.z - m_MinPt.z);
	return ge;
}
void BMQuadtreeNode::ComputeBoxBoundingVolume(float box[12])
{
	GLTF::BMVertex _center = GLTF::BMVertex((m_MaxPt.x + m_MinPt.x) / 2.0f, (m_MaxPt.y + m_MinPt.y) / 2.0f, (m_MaxPt.z + m_MinPt.z) / 2.0f);
	GLTF::BMVertex _bouding = GLTF::BMVertex(m_MaxPt.x - m_MinPt.x, m_MaxPt.y - m_MinPt.y, m_MaxPt.z - m_MinPt.z);
	//
	box[0] = _center.m_x; box[1] = _center.m_y; box[2] = _center.m_z;
	box[3] = _bouding.m_x; box[4] = 0.0f; box[5] = 0.0f;
	box[6] = 0.0f; box[7] = _bouding.m_y; box[8] = 0.0f;
	box[9] = 0.0f; box[10] = 0.0f; box[11] = _bouding.m_z;
}
#include "../rapidjson/stringbuffer.h"
#include "../rapidjson/writer.h"
void BMQuadtreeNode::writeJSON(void* writer, bool inOBJ /*= false*/)
{
	if (!m_Invalide && m_pLeft_Down == nullptr) return;
	rapidjson::Writer<rapidjson::StringBuffer>* jsonWriter = (rapidjson::Writer<rapidjson::StringBuffer>*)writer;
	if (inOBJ)
	{
		jsonWriter->StartObject();
	}
	//
	float boundingBox[12];
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
		std::string url = m_Name + ".b3dm";
		jsonWriter->Key("url");
		jsonWriter->String(url.c_str());
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
void BMQuadtreeManager::InitRootNode()
{
	if (m_Root) return;
	HPoint _minPt, _maxPt;
	bool _result = HC_Compute_Circumcuboid_By_Key(m_RootKey, &_minPt, &_maxPt);
	if (!_result) return;
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
	m_Root = new BMQuadtreeNode;
	m_Root->m_MaxPt = _maxPt;
	m_Root->m_MinPt = _minPt;
	m_Root->m_Name = "0";
	m_Root->SetBoundAndCenter(_minPt, _maxPt);
}
void BMQuadtreeManager::AddNode(const HPoint& minPt, const HPoint& maxPt, HC_KEY segmentKey)
{
	if (m_Root == NULL) return;
	//
	BMQuadtreeNode* _SON = m_Root->GetShellLocationOctreeNode(minPt, maxPt);
	BMQuadtreeSegemtNode _temp;
	_temp._minPt = minPt;
	_temp._maxPt = maxPt;
	_temp._segmenyKey = segmentKey;
	_SON->m_vSegmentNodes.push_back(_temp);
}
void BMQuadtreeManager::ParseSegment(HC_KEY segmentKey)
{
	HPoint _minPt, _maxPt;
	float _matirx[16];
	HC_KEY _shellKey;
	char _geoType[64];
	vector<HC_KEY> _vSegments;
	HC_Open_Segment_By_Key(segmentKey);
	{
		HC_Begin_Segment_Search("...");
		{
			char _pathName[SA_BUFFER_SIZE]; _pathName[0] = '\0';
			while (HC_Find_Segment(_pathName))
			{
				HC_KEY _tempKey = HC_Open_Segment(_pathName);
				HC_Close_Segment();
				_vSegments.push_back(_tempKey);
			}
		}
		HC_End_Segment_Search();
	}
	HC_Close_Segment();
	for (auto key : _vSegments)
	{
		HC_Open_Segment_By_Key(key);
		bool _hasMinMax = HC_Compute_Circumcuboid_By_Key(key, &_minPt, &_maxPt);
		if (_hasMinMax)
		{
			HC_Show_Net_Modelling_Matrix(_matirx);
			HC_Compute_Transformed_Points(1, &_minPt, _matirx, &_minPt);
			HC_Compute_Transformed_Points(1, &_maxPt, _matirx, &_maxPt);
			//
			int count = 0;
			bool flag = false;
			HC_Begin_Contents_Search(".", "geometry");
			HC_Show_Contents_Count(&count);
			for (int j = 0; j < count; ++j)
			{
				HUtility::FindContents(_shellKey);
				HC_Show_Key_Type(_shellKey, _geoType);
				if (0 == strcmp("shell", _geoType))
				{
					flag = true;
					break;
				}
			}
			HC_End_Contents_Search();
			if (flag)
				AddNode(_minPt, _maxPt, key);
			HC_Close_Segment();
		}
	}
}
void BMQuadtreeManager::CreateBMMesh(BMQuadtreeNode* _pNode)
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
bool BMQuadtreeManager::CreateQuadtree()
{
	if (m_RootKey == INVALID_KEY) return false;
	if (m_HasCreate) return true;
	m_HasCreate = true;
	InitRootNode();
	ParseSegment(m_RootKey);
	//
	return true;
}
