#include<ctype.h>
#include "LODBMQuadtreeNode.h"
#define MVO_BUFFER_SIZE						4096
bool LODBMShellPrimitive::ComputeContent(float offsetX, float offsetY, float offsetZ)
{
	bool needDeleteShell = false;
	if (m_shellKey == INVALID_KEY)
	{
		m_shellKey = CreateShellByCylinder();
		needDeleteShell = true;
	}
	//
	char cFaceAlpha[256]; cFaceAlpha[0] = '\0';
	char cFaceColor[SA_BUFFER_SIZE];
	int ptNum;
	int flistNum;
	HC_Show_Shell_Size(m_shellKey, &ptNum, &flistNum);
	if (flistNum > 87380)
	{
		if (needDeleteShell)
		{
			HC_Delete_By_Key(m_shellKey);
		}
		return false;
	}
	//
	float _matirx[16];
	HC_Open_Segment_By_Key(m_segmentKey);
	HC_Show_Net_Modelling_Matrix(_matirx);
	HC_Close_Segment();
	//
	HPoint* pPoint = new HPoint[ptNum];
	HPoint* pNormal = new HPoint[ptNum];
	int* fList = new int[flistNum];
	HC_Show_Shell(m_shellKey, &ptNum, pPoint, &flistNum, fList);
	HC_MShow_Net_Vertex_Normals(m_shellKey, 0, ptNum, pNormal);
	HC_Compute_Transformed_Points(ptNum, pPoint, _matirx, pPoint);
	//
	int nNumber = 0;
	HC_MShow_Vertex_Parameter_Size(m_shellKey, &nNumber);
	if (nNumber != 0)
	{
		float * vps = new float[ptNum * nNumber];
		HC_MShow_Vertex_Parameters(m_shellKey, 0, ptNum, &nNumber, vps);
		m_vCoords.reserve(ptNum);
		size_t _index = 0;
		for (size_t i = 0; i < ptNum;++i)
		{
			m_vCoords.push_back(GLTF::BMVertex(vps[_index++], vps[_index++]));
		}
		delete[] vps;
	}
	//
	HC_Open_Geometry(m_shellKey);
	if (HC_Show_Existence("color=(face=diffuse)"))
	{
		HC_Show_One_Color("face=diffuse", cFaceColor);
		if (strstr(cFaceColor,"texture") != 0)
		{
			HC_Show_One_Color("face=diffuse texture", cFaceColor);
			std::string strDiffuse = cFaceColor;
			if (strDiffuse.find('(') == 0 && strDiffuse.find(')') == strDiffuse.length() - 1)
			{
				strDiffuse = strDiffuse.substr(1, strDiffuse.length() - 2);
			}
			m_TextureImagePath = strDiffuse;
		}
		else
			extractRGBNew(cFaceColor, m_Color.m_r, m_Color.m_g, m_Color.m_b);
	}
	if (HC_Show_Existence("color=(face=transmission)"))
	{
		HC_Show_One_Color("face=transmission", cFaceAlpha);
		float fAlpha = -1.0f;
		if (extractRGBNew(cFaceAlpha, fAlpha, fAlpha, fAlpha))
		{
			fAlpha = 1.0 - fAlpha;
			m_Color.m_a = fAlpha;
		}
	}
	HC_Close_Geometry();
	//
	m_vPositions.reserve(ptNum);
	m_vNormals.reserve(ptNum);
	m_vTris.reserve(flistNum / 4);
	for (size_t i = 0; i < ptNum; ++i)
	{
		HPoint& pt = pPoint[i];
		HPoint& normal = pNormal[i];
		m_vPositions.push_back(GLTF::BMVertex(pt.x - offsetX, pt.y - offsetY, pt.z - offsetZ));
		m_vNormals.push_back(GLTF::BMVertex(normal.x, normal.y, normal.z));
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
			vector<int> _tempPts;
			for (int j = 0; j < numOfPt; ++j)
			{
				_tempPts.push_back(fList[i + j]);
			}
			for (int j = 0; j < numOfPt - 2;++j)
			{
				m_vTris.push_back(GLTF::BMTriangle(0, _tempPts[j + 1], _tempPts[j + 2]));
			}
		}
		//
		i = i + fList[i] + 1;
	}
	//
	delete[] pPoint;
	delete[] fList;
	delete[] pNormal;
	if (needDeleteShell)
	{
		HC_Delete_By_Key(m_shellKey);
	}
	return true;
}
vector<LODBMShellPrimitive*> LODBMShellPrimitive::Subdivide(float offsetX, float offsetY, float offsetZ)
{
	bool needDeleteShell = false;
	if (m_shellKey == INVALID_KEY)
	{
		m_shellKey = CreateShellByCylinder();
		needDeleteShell = true;
	}
	//
	char cFaceAlpha[256]; cFaceAlpha[0] = '\0';
	char cFaceColor[SA_BUFFER_SIZE];
	int ptNum;
	int flistNum;
	float _matirx[16];
	HC_Open_Segment_By_Key(m_segmentKey);
	HC_Show_Net_Modelling_Matrix(_matirx);
	HC_Close_Segment();
	//
	HC_Show_Shell_Size(m_shellKey, &ptNum, &flistNum);
	HPoint* pPoint = new HPoint[ptNum];
	HPoint* pNormal = new HPoint[ptNum];
	int* fList = new int[flistNum];
	HC_Show_Shell(m_shellKey, &ptNum, pPoint, &flistNum, fList);
	HC_MShow_Net_Vertex_Normals(m_shellKey, 0, ptNum, pNormal);
	HC_Compute_Transformed_Points(ptNum, pPoint, _matirx, pPoint);
	//
	int nNumber = 0;
	HC_MShow_Vertex_Parameter_Size(m_shellKey, &nNumber);
	if (nNumber != 0)
	{
		float * vps = new float[ptNum * nNumber];
		HC_MShow_Vertex_Parameters(m_shellKey, 0, ptNum, &nNumber, vps);
		m_vCoords.reserve(ptNum);
		size_t _index = 0;
		for (size_t i = 0; i < ptNum; ++i)
		{
		m_vCoords.push_back(GLTF::BMVertex(vps[_index++], vps[_index++]));
		}
		delete[] vps;
	}
	//
	HC_Open_Geometry(m_shellKey);
	if (HC_Show_Existence("color=(face=diffuse)"))
	{
		HC_Show_One_Color("face=diffuse", cFaceColor);
		if (strstr(cFaceColor, "texture") != 0)
		{
			HC_Show_One_Color("face=diffuse texture", cFaceColor);
			std::string strDiffuse = cFaceColor;
			if (strDiffuse.find('(') == 0 && strDiffuse.find(')') == strDiffuse.length() - 1)
			{
				strDiffuse = strDiffuse.substr(1, strDiffuse.length() - 2);
			}
			m_TextureImagePath = strDiffuse;
		}
		else
			extractRGBNew(cFaceColor, m_Color.m_r, m_Color.m_g, m_Color.m_b);
	}
	if (HC_Show_Existence("color=(face=transmission)"))
	{
		HC_Show_One_Color("face=transmission", cFaceAlpha);
		float fAlpha = -1.0f;
		if (extractRGBNew(cFaceAlpha, fAlpha, fAlpha, fAlpha))
		{
			fAlpha = 1.0 - fAlpha;
			m_Color.m_a = fAlpha;
		}
	}
	HC_Close_Geometry();
	//
	m_vPositions.reserve(ptNum);
	m_vNormals.reserve(ptNum);
	m_vTris.reserve(flistNum / 4);
	for (size_t i = 0; i < ptNum; ++i)
	{
		HPoint& pt = pPoint[i];
		HPoint& normal = pNormal[i];
		m_vPositions.push_back(GLTF::BMVertex(pt.x - offsetX, pt.y - offsetY, pt.z - offsetZ));
		m_vNormals.push_back(GLTF::BMVertex(normal.x, normal.y, normal.z));
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
			vector<int> _tempPts;
			for (int j = 0; j < numOfPt; ++j)
			{
				_tempPts.push_back(fList[i + j]);
			}
			for (int j = 0; j < numOfPt - 2; ++j)
			{
				m_vTris.push_back(GLTF::BMTriangle(0, _tempPts[j + 1], _tempPts[j + 2]));
			}
		}
		//
		i = i + fList[i] + 1;
	}
	////
	delete[] pPoint;
	delete[] fList;
	delete[] pNormal;
	//////////////////////////////////////////////////////////////////////////
	vector<LODBMShellPrimitive*> _vResultPris;
	int _subShells = m_vTris.size() / 21845;//21845 = 65535/3
	if (m_vCoords.size() > 0)
	{
		for (int i = 0; i < _subShells - 1; ++i)
		{
			vector<GLTF::BMVertex> _vPosi; _vPosi.reserve(21845 * 3);
			vector<GLTF::BMVertex> _vNorm; _vNorm.reserve(21845 * 3);
			vector<GLTF::BMTriangle> _vTir; _vTir.reserve(21845);
			vector<GLTF::BMVertex> _vCoord; _vCoord.reserve(21845 * 3);
			//
			int handleTriNum = i * 21845;
			std::map<int, int> _indexMap;
			for (size_t j = 0; j < 21845; j++)
			{
				GLTF::BMTriangle& tempTri = m_vTris[handleTriNum + j];
				int newIndex0 = _indexMap[tempTri.m_index0];
				int newIndex1 = _indexMap[tempTri.m_index1];
				int newIndex2 = _indexMap[tempTri.m_index2];
				if (newIndex0 == 0)
				{
					_vPosi.push_back(m_vPositions[tempTri.m_index0]);
					_vNorm.push_back(m_vNormals[tempTri.m_index0]);
					_vCoord.push_back(m_vCoords[tempTri.m_index0]);
					_indexMap[tempTri.m_index0] = newIndex0 = _vPosi.size();
				}
				if (newIndex1 == 0)
				{
					_vPosi.push_back(m_vPositions[tempTri.m_index1]);
					_vNorm.push_back(m_vNormals[tempTri.m_index1]);
					_vCoord.push_back(m_vCoords[tempTri.m_index1]);
					_indexMap[tempTri.m_index1] = newIndex1 = _vPosi.size();
				}
				if (newIndex2 == 0)
				{
					_vPosi.push_back(m_vPositions[tempTri.m_index2]);
					_vNorm.push_back(m_vNormals[tempTri.m_index2]);
					_vCoord.push_back(m_vCoords[tempTri.m_index2]);
					_indexMap[tempTri.m_index2] = newIndex2 = _vPosi.size();
				}
				//
				tempTri.m_index0 = newIndex0 - 1;
				tempTri.m_index1 = newIndex1 - 1;
				tempTri.m_index2 = newIndex2 - 1;
				_vTir.push_back(tempTri);
			}
			//
			LODBMShellPrimitive* _pNewPri = new LODBMShellPrimitive;
			_pNewPri->m_TesseNum = m_TesseNum;
			_pNewPri->m_Color = m_Color;
			_pNewPri->m_TextureImagePath = m_TextureImagePath;
			_pNewPri->m_shellKey = m_shellKey;
			_pNewPri->m_segmentName = m_segmentName;
			_pNewPri->m_vPositions.swap(_vPosi);
			_pNewPri->m_vNormals.swap(_vNorm);
			_pNewPri->m_vTris.swap(_vTir);
			_pNewPri->m_vCoords.swap(_vCoord);
			_vResultPris.push_back(_pNewPri);
		}
		//
		{
			int handleTriNum = (_subShells - 1) * 21845;
			int leftTirNum = m_vTris.size() - handleTriNum;
			vector<GLTF::BMVertex> _vPosi; _vPosi.reserve(leftTirNum * 3);
			vector<GLTF::BMVertex> _vNorm; _vNorm.reserve(leftTirNum * 3);
			vector<GLTF::BMTriangle> _vTir; _vTir.reserve(leftTirNum);
			vector<GLTF::BMVertex> _vCoord; _vCoord.reserve(leftTirNum * 3);
			std::map<int, int> _indexMap;
			for (size_t j = 0; j < leftTirNum; j++)
			{
				GLTF::BMTriangle& tempTri = m_vTris[handleTriNum + j];
				int newIndex0 = _indexMap[tempTri.m_index0];
				int newIndex1 = _indexMap[tempTri.m_index1];
				int newIndex2 = _indexMap[tempTri.m_index2];
				if (newIndex0 == 0)
				{
					_vPosi.push_back(m_vPositions[tempTri.m_index0]);
					_vNorm.push_back(m_vNormals[tempTri.m_index0]);
					_vCoord.push_back(m_vCoords[tempTri.m_index0]);
					_indexMap[tempTri.m_index0] = newIndex0 = _vPosi.size();
				}
				if (newIndex1 == 0)
				{
					_vPosi.push_back(m_vPositions[tempTri.m_index1]);
					_vNorm.push_back(m_vNormals[tempTri.m_index1]);
					_vCoord.push_back(m_vCoords[tempTri.m_index1]);
					_indexMap[tempTri.m_index1] = newIndex1 = _vPosi.size();
				}
				if (newIndex2 == 0)
				{
					_vPosi.push_back(m_vPositions[tempTri.m_index2]);
					_vNorm.push_back(m_vNormals[tempTri.m_index2]);
					_vCoord.push_back(m_vCoords[tempTri.m_index2]);
					_indexMap[tempTri.m_index2] = newIndex2 = _vPosi.size();
				}
				//
				tempTri.m_index0 = newIndex0 - 1;
				tempTri.m_index1 = newIndex1 - 1;
				tempTri.m_index2 = newIndex2 - 1;
				_vTir.push_back(tempTri);
			}
			//
			m_vPositions.swap(_vPosi);
			m_vNormals.swap(_vNorm);
			m_vTris.swap(_vTir);
			m_vCoords.swap(_vCoord);
		}
	}
	else
	{
		for (int i = 0; i < _subShells - 1; ++i)
		{
			vector<GLTF::BMVertex> _vPosi; _vPosi.reserve(21845 * 3);
			vector<GLTF::BMVertex> _vNorm; _vNorm.reserve(21845 * 3);
			vector<GLTF::BMTriangle> _vTir; _vTir.reserve(21845);
			//
			int handleTriNum = i * 21845;
			std::map<int, int> _indexMap;
			for (size_t j = 0; j < 21845; j++)
			{
				GLTF::BMTriangle& tempTri = m_vTris[handleTriNum + j];
				int newIndex0 = _indexMap[tempTri.m_index0];
				int newIndex1 = _indexMap[tempTri.m_index1];
				int newIndex2 = _indexMap[tempTri.m_index2];
				if (newIndex0 == 0)
				{
					_vPosi.push_back(m_vPositions[tempTri.m_index0]);
					_vNorm.push_back(m_vNormals[tempTri.m_index0]);
					_indexMap[tempTri.m_index0] = newIndex0 = _vPosi.size();
				}
				if (newIndex1 == 0)
				{
					_vPosi.push_back(m_vPositions[tempTri.m_index1]);
					_vNorm.push_back(m_vNormals[tempTri.m_index1]);
					_indexMap[tempTri.m_index1] = newIndex1 = _vPosi.size();
				}
				if (newIndex2 == 0)
				{
					_vPosi.push_back(m_vPositions[tempTri.m_index2]);
					_vNorm.push_back(m_vNormals[tempTri.m_index2]);
					_indexMap[tempTri.m_index2] = newIndex2 = _vPosi.size();
				}
				//
				tempTri.m_index0 = newIndex0 - 1;
				tempTri.m_index1 = newIndex1 - 1;
				tempTri.m_index2 = newIndex2 - 1;
				_vTir.push_back(tempTri);
			}
			//
			LODBMShellPrimitive* _pNewPri = new LODBMShellPrimitive;
			_pNewPri->m_TesseNum = m_TesseNum;
			_pNewPri->m_Color = m_Color;
			_pNewPri->m_shellKey = m_shellKey;
			_pNewPri->m_segmentName = m_segmentName;
			_pNewPri->m_vPositions.swap(_vPosi);
			_pNewPri->m_vNormals.swap(_vNorm);
			_pNewPri->m_vTris.swap(_vTir);
			_vResultPris.push_back(_pNewPri);
		}
		//
		{
			int handleTriNum = (_subShells - 1) * 21845;
			int leftTirNum = m_vTris.size() - handleTriNum;
			vector<GLTF::BMVertex> _vPosi; _vPosi.reserve(leftTirNum * 3);
			vector<GLTF::BMVertex> _vNorm; _vNorm.reserve(leftTirNum * 3);
			vector<GLTF::BMTriangle> _vTir; _vTir.reserve(leftTirNum);
			std::map<int, int> _indexMap;
			for (size_t j = 0; j < leftTirNum; j++)
			{
				GLTF::BMTriangle& tempTri = m_vTris[handleTriNum + j];
				int newIndex0 = _indexMap[tempTri.m_index0];
				int newIndex1 = _indexMap[tempTri.m_index1];
				int newIndex2 = _indexMap[tempTri.m_index2];
				if (newIndex0 == 0)
				{
					_vPosi.push_back(m_vPositions[tempTri.m_index0]);
					_vNorm.push_back(m_vNormals[tempTri.m_index0]);
					_indexMap[tempTri.m_index0] = newIndex0 = _vPosi.size();
				}
				if (newIndex1 == 0)
				{
					_vPosi.push_back(m_vPositions[tempTri.m_index1]);
					_vNorm.push_back(m_vNormals[tempTri.m_index1]);
					_indexMap[tempTri.m_index1] = newIndex1 = _vPosi.size();
				}
				if (newIndex2 == 0)
				{
					_vPosi.push_back(m_vPositions[tempTri.m_index2]);
					_vNorm.push_back(m_vNormals[tempTri.m_index2]);
					_indexMap[tempTri.m_index2] = newIndex2 = _vPosi.size();
				}
				//
				tempTri.m_index0 = newIndex0 - 1;
				tempTri.m_index1 = newIndex1 - 1;
				tempTri.m_index2 = newIndex2 - 1;
				_vTir.push_back(tempTri);
			}
			//
			m_vPositions.swap(_vPosi);
			m_vNormals.swap(_vNorm);
			m_vTris.swap(_vTir);
		}
	}
	//
	if (needDeleteShell)
	{
		HC_Delete_By_Key(m_shellKey);
	}
	return _vResultPris;
}
GLTF::BMVertex LODBMShellPrimitive::ComputeNormal(GLTF::BMVertex& p0, GLTF::BMVertex& p1, GLTF::BMVertex& p2)
{
	GLTF::BMVertex n0 = p1 - p0;
	GLTF::BMVertex n1 = p2 - p0;
	return n0.Cross(n1).Normalization();
}
bool LODBMShellPrimitive::extractRGBNew(char* pRGB, float& fRed, float& fGreen, float& fBlue)
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
HC_KEY LODBMShellPrimitive::CreateShellByCylinder()
{
	vector<HPoint> vCenterPts;
	vector<float> vRadius;
	HC_KEY returnKey;
	//
	if (m_CylinderKey != INVALID_KEY)
	{
		HPoint firstPt, secondPt;
		float radius;
		//
		HC_Show_Cylinder(m_CylinderKey, &firstPt, &secondPt, &radius, nullptr);
		vCenterPts.push_back(firstPt);
		vCenterPts.push_back(secondPt);
		vRadius.push_back(radius);
	}
	else
	{
		int ptNum, radiusNum;
		HC_Show_PolyCylinder_Counts(m_PolyCylinderKey, &ptNum, &radiusNum, nullptr);
		HPoint* pPointnnnnnn = new HPoint[ptNum];
		float* pRRRRRRRRRRR = new float[radiusNum];
		HC_Show_PolyCylinder(m_PolyCylinderKey, &ptNum, pPointnnnnnn, &radiusNum, pRRRRRRRRRRR, nullptr);
		for (int i = 0; i < ptNum; ++i)
		{
			vCenterPts.push_back(pPointnnnnnn[i]);
		}
		for (int i = 0; i < radiusNum; ++i)
		{
			vRadius.push_back(pRRRRRRRRRRR[i]);
		}
		delete[] pPointnnnnnn;
		delete[] pRRRRRRRRRRR;
	}
	//
	TessellationCylinder tc;
	tc.Tessellation(vCenterPts, vRadius, m_TesseNum);
	HC_Open_Segment_By_Key(m_segmentKey);
	char colorStr[1024];
	HC_Show_Color(colorStr);
	returnKey = HC_Insert_Shell(tc.m_vShellPoints.size(), &tc.m_vShellPoints[0], tc.m_vFaceLists.size(), &tc.m_vFaceLists[0]);
	HC_Close_Segment();
	//
	HC_Open_Geometry(returnKey);
	HC_Set_Color(colorStr);
	HC_Close_Geometry();
	//
	return returnKey;
}
//////////////////////////////////////////////////////////////////////////
void LODBMQuadtreeNode::SetBoundAndCenter(const HPoint& minPt, const HPoint& maxPt)
{
	m_MaxPt = maxPt;
	m_MinPt = minPt;
	m_CenterPt = (m_MaxPt + m_MinPt) / 2;
}
void LODBMQuadtreeNode::InitChildNode()
{
	m_pLeft_Up = new LODBMQuadtreeNode;
	m_pLeft_Up->SetBoundAndCenter(HPoint(m_MinPt.x, m_CenterPt.y, m_MinPt.z), HPoint(m_CenterPt.x, m_MaxPt.y, m_MaxPt.z));
	m_pLeft_Up->m_pParent = this;
	m_pLeft_Up->m_Name = m_Name + "_0";
	m_pLeft_Up->m_pTreeMana = m_pTreeMana;
	m_pLeft_Up->m_LodLayer = m_LodLayer + 1;
	//
	m_pLeft_Down = new LODBMQuadtreeNode;
	m_pLeft_Down->SetBoundAndCenter(HPoint(m_MinPt.x, m_MinPt.y, m_MinPt.z), HPoint(m_CenterPt.x, m_CenterPt.y, m_MaxPt.z));
	m_pLeft_Down->m_pParent = this;
	m_pLeft_Down->m_Name = m_Name + "_1";
	m_pLeft_Down->m_pTreeMana = m_pTreeMana;
	m_pLeft_Down->m_LodLayer = m_LodLayer + 1;
	//
	m_pRight_Up = new LODBMQuadtreeNode;
	m_pRight_Up->SetBoundAndCenter(HPoint(m_CenterPt.x, m_CenterPt.y, m_MinPt.z), HPoint(m_MaxPt.x, m_MaxPt.y, m_MaxPt.z));
	m_pRight_Up->m_pParent = this;
	m_pRight_Up->m_Name = m_Name + "_3";
	m_pRight_Up->m_pTreeMana = m_pTreeMana;
	m_pRight_Up->m_LodLayer = m_LodLayer + 1;
	//
	m_pRight_Down = new LODBMQuadtreeNode;
	m_pRight_Down->SetBoundAndCenter(HPoint(m_CenterPt.x, m_MinPt.y, m_MinPt.z), HPoint(m_MaxPt.x, m_CenterPt.y, m_MaxPt.z));
	m_pRight_Down->m_pParent = this;
	m_pRight_Down->m_Name = m_Name + "_2";
	m_pRight_Down->m_pTreeMana = m_pTreeMana;
	m_pRight_Down->m_LodLayer = m_LodLayer + 1;
}
LODBMQuadtreeNode* LODBMQuadtreeNode::GetShellLocationOctreeNode(const HPoint& midPt, int lodLayer)
{
	if (lodLayer == m_LodLayer)
	{
		return this;
	}
	//
// 	if ((m_MaxPt.x - m_MinPt.x) < 0.5)
// 	{
// 		return this;
// 	}
	//
	if (m_pLeft_Up)
	{
		if (m_pLeft_Up->m_MinPt.x <= midPt.x && m_pLeft_Up->m_MinPt.y <= midPt.y &&
			m_pLeft_Up->m_MaxPt.x >= midPt.x && m_pLeft_Up->m_MaxPt.y >= midPt.y)
		{
			return m_pLeft_Up->GetShellLocationOctreeNode(midPt, lodLayer);
		}
		else if (m_pLeft_Down->m_MinPt.x <= midPt.x && m_pLeft_Down->m_MinPt.y <= midPt.y &&
			m_pLeft_Down->m_MaxPt.x >= midPt.x && m_pLeft_Down->m_MaxPt.y >= midPt.y)
		{
			return m_pLeft_Down->GetShellLocationOctreeNode(midPt, lodLayer);
		}
		else if (m_pRight_Up->m_MinPt.x <= midPt.x && m_pRight_Up->m_MinPt.y <= midPt.y &&
			m_pRight_Up->m_MaxPt.x >= midPt.x && m_pRight_Up->m_MaxPt.y >= midPt.y)
		{
			return m_pRight_Up->GetShellLocationOctreeNode(midPt, lodLayer);
		}
		else /*if (m_pRight_Down->m_MinPt.x <= midPt.x && m_pRight_Down->m_MinPt.y <= midPt.y &&
			m_pRight_Down->m_MaxPt.x >= midPt.x && m_pRight_Down->m_MaxPt.y >= midPt.y)*/
		{
			return m_pRight_Down->GetShellLocationOctreeNode(midPt, lodLayer);
		}
	}
	else
	{
		bool _hasInitChild = false;
		HPoint _left_up_minPt(m_MinPt.x, m_CenterPt.y, m_MinPt.z), _left_up_maxPt(m_CenterPt.x, m_MaxPt.y, m_MaxPt.z);
		HPoint _right_up_minPt(m_CenterPt.x, m_CenterPt.y, m_MinPt.z), _right_up_maxPt(m_MaxPt.x, m_MaxPt.y, m_MaxPt.z);
		HPoint _left_down_minPt(m_MinPt.x, m_MinPt.y, m_MinPt.z), _left_down_maxPt(m_CenterPt.x, m_CenterPt.y, m_MaxPt.z);
		HPoint _right_down_minPt(m_CenterPt.x, m_MinPt.y, m_MinPt.z), _right_down_maxPt(m_MaxPt.x, m_CenterPt.y, m_MaxPt.z);
		//
		if (_left_up_minPt.x <= midPt.x && _left_up_minPt.y <= midPt.y &&
			_left_up_maxPt.x >= midPt.x && _left_up_maxPt.y >= midPt.y)
		{
			InitChildNode();
			_hasInitChild = true;
			return m_pLeft_Up->GetShellLocationOctreeNode(midPt, lodLayer);
		}
		else if (_right_up_minPt.x <= midPt.x && _right_up_minPt.y <= midPt.y &&
			_right_up_maxPt.x >= midPt.x && _right_up_maxPt.y >= midPt.y)
		{
			if (!_hasInitChild)
			{
				_hasInitChild = true;
				InitChildNode();
			}
			return m_pRight_Up->GetShellLocationOctreeNode(midPt, lodLayer);
		}
		else if (_left_down_minPt.x <= midPt.x && _left_down_minPt.y <= midPt.y &&
			_left_down_maxPt.x >= midPt.x && _left_down_maxPt.y >= midPt.y)
		{
			if (!_hasInitChild)
			{
				_hasInitChild = true;
				InitChildNode();
			}
			return m_pLeft_Down->GetShellLocationOctreeNode(midPt, lodLayer);
		}
		else /*if (_right_down_minPt.x <= midPt.x && _right_down_minPt.y <= midPt.y &&
			_right_down_maxPt.x >= midPt.x && _right_down_maxPt.y >= midPt.y)*/
		{
			if (!_hasInitChild)
			{
				_hasInitChild = true;
				InitChildNode();
			}
			return m_pRight_Down->GetShellLocationOctreeNode(midPt, lodLayer);
		}
	}
}
void LODBMQuadtreeNode::CreateBMMesh(float offsetX, float offsetY, float offsetZ)
{
	if (m_HasCreateBMMesh) return;
	m_HasCreateBMMesh = true;
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
				HC_Find_Contents(0, &_shellKey);
				HC_Show_Key_Type(_shellKey, _geoType);
				if (0 == strcmp("shell", _geoType))
				{
					LODBMShellPrimitive* pNewShellPri = new LODBMShellPrimitive();
					pNewShellPri->m_TesseNum = m_TesseNum;
					pNewShellPri->m_shellKey = _shellKey;
					pNewShellPri->m_segmentKey = SegemtNode._segmenyKey;
					pNewShellPri->m_segmentName = std::string(segName);
					m_vShellPrimitives.push_back(pNewShellPri);
				}
				else if (0 == strcmp("cylinder", _geoType))
				{
					LODBMShellPrimitive* pNewShellPri = new LODBMShellPrimitive();
					pNewShellPri->m_TesseNum = m_TesseNum;
					pNewShellPri->m_CylinderKey = _shellKey;
					pNewShellPri->m_segmentKey = SegemtNode._segmenyKey;
					pNewShellPri->m_segmentName = std::string(segName);
					m_vShellPrimitives.push_back(pNewShellPri);
				}
				else if (0 == strcmp("polycylinder", _geoType))
				{
					LODBMShellPrimitive* pNewShellPri = new LODBMShellPrimitive();
					pNewShellPri->m_TesseNum = m_TesseNum;
					pNewShellPri->m_PolyCylinderKey = _shellKey;
					pNewShellPri->m_segmentKey = SegemtNode._segmenyKey;
					pNewShellPri->m_segmentName = std::string(segName);
					m_vShellPrimitives.push_back(pNewShellPri);
				}
			}
		}
		HC_End_Contents_Search();
		HC_Close_Segment();
		//
		m_ContentMaxPt.x = max(m_ContentMaxPt.x, SegemtNode._maxPt.x);
		m_ContentMaxPt.y = max(m_ContentMaxPt.y, SegemtNode._maxPt.y);
		m_ContentMaxPt.z = max(m_ContentMaxPt.z, SegemtNode._maxPt.z);
		//
		m_ContentMinPt.x = min(m_ContentMinPt.x, SegemtNode._minPt.x);
		m_ContentMinPt.y = min(m_ContentMinPt.y, SegemtNode._minPt.y);
		m_ContentMinPt.z = min(m_ContentMinPt.z, SegemtNode._minPt.z);
	}
	m_vSegmentNodes.clear();
	//////////////////////////////////////////////////////////////////////////
	if (m_vShellPrimitives.size() == 0)
	{
		m_Invalide = false;
		return;
	}
	m_Invalide = true;
	//计算 BMShellPrimitive 中位置、法线、三角形索引
	vector<LODBMShellPrimitive*> _vNeedSubdivideShell;
	for (auto _pShellPri : m_vShellPrimitives)
	{
		if (!_pShellPri->ComputeContent(offsetX, offsetY, offsetZ))
		{
			_vNeedSubdivideShell.push_back(_pShellPri);
		}
	}
	//细分BMShellPrimitive
	for (auto _pShellPri : _vNeedSubdivideShell)
	{
		vector<LODBMShellPrimitive*> _vNewShellPrimitives = _pShellPri->Subdivide(offsetX, offsetY, offsetZ);
		m_vShellPrimitives.insert(m_vShellPrimitives.end(), _vNewShellPrimitives.begin(), _vNewShellPrimitives.end());
	}
	//////////////////////////////////////////////////////////////////////////
	std::set<std::string> tempNams;
	for (auto _pShellPri : m_vShellPrimitives)
	{
		if (tempNams.insert(_pShellPri->m_segmentName).second)
		{
			m_BMMesh.m_vBatchNames.push_back(_pShellPri->m_segmentName);
		}
	}
	m_BMMesh.m_strName = m_Name;
	//将BMShellPrimitive 转换为 GLTF::BMPrimitive
	for (auto _pShellPri : m_vShellPrimitives)
	{
		GLTF::BMPrimitive _bmPri;
		_bmPri.m_vBatchIDs.reserve(_pShellPri->m_vPositions.size());
		_bmPri.m_vIndexs.reserve(_pShellPri->m_vTris.size()*3);
		//
		_bmPri.m_vNormals.swap(_pShellPri->m_vNormals);
		_bmPri.m_vPositions.swap(_pShellPri->m_vPositions);
		for (auto& tri : _pShellPri->m_vTris)
		{
			_bmPri.m_vIndexs.push_back(tri.m_index0);
			_bmPri.m_vIndexs.push_back(tri.m_index1);
			_bmPri.m_vIndexs.push_back(tri.m_index2);
		}
		//
		auto it = find(m_BMMesh.m_vBatchNames.begin(), m_BMMesh.m_vBatchNames.end(), _pShellPri->m_segmentName);
		size_t _id = it - m_BMMesh.m_vBatchNames.begin();
		size_t _verNum = _bmPri.m_vPositions.size();
		for (size_t i = 0; i < _verNum; ++i)
		{
			_bmPri.m_vBatchIDs.push_back(_id);
		}
		//
		_bmPri.m_Color = _pShellPri->m_Color;
		_bmPri.m_TextureImagePath = m_pTreeMana->m_ImageMaps[_pShellPri->m_TextureImagePath];
		_bmPri.m_vCoords.swap(_pShellPri->m_vCoords);
		//
		m_BMMesh.m_vPrimitives.push_back(_bmPri);
		//
		delete _pShellPri;
	}
	m_vShellPrimitives.clear();
}
float LODBMQuadtreeNode::ComputeGeometricError()
{
	//float ge = max(m_ContentMaxPt.x - m_ContentMinPt.x, m_ContentMaxPt.y - m_ContentMinPt.y);
	float ge = max(m_MaxPt.x - m_MinPt.x, m_MaxPt.y - m_MinPt.y);
	return ge;
}
void LODBMQuadtreeNode::ComputeContentBoxBoundingVolume(float box[12])
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
void LODBMQuadtreeNode::ComputeBoxBoundingVolume(float box[12])
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
void LODBMQuadtreeNode::writeJSON(void* writer, bool inOBJ /*= false*/)
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
	jsonWriter->String("REPLACE");
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
void LODBMQuadtreeManager::InitRootNode()
{
	if (m_Root) return;
	float _matirx[16];
	HPoint _minPt, _maxPt;
	HC_Open_Segment_By_Key(m_RootKey);
	HC_Show_Net_Modelling_Matrix(_matirx);
	HC_Compute_Circumcuboid(".", &_minPt, &_maxPt);
	HC_Close_Segment();
	//
	HC_Compute_Transformed_Points(1, &_minPt, _matirx, &_minPt);
	HC_Compute_Transformed_Points(1, &_maxPt, _matirx, &_maxPt);
	//
	_minPt.x -= m_Cartesian.m_x; _minPt.y -= m_Cartesian.m_y; _minPt.z -= m_Cartesian.m_z;
	_maxPt.x -= m_Cartesian.m_x; _maxPt.y -= m_Cartesian.m_y; _maxPt.z -= m_Cartesian.m_z;
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
	m_Root = new LODBMQuadtreeNode;
	m_Root->m_TesseNum = m_TesseNum;
	m_Root->m_MaxPt = _maxPt;
	m_Root->m_MinPt = _minPt;
	m_Root->m_Name = "0";
	m_Root->m_CenterPt = (m_Root->m_MaxPt + m_Root->m_MinPt) / 2;
	m_Root->m_pTreeMana = this;
	m_Root->m_LodLayer =0;
}
void LODBMQuadtreeManager::AddNode(const HPoint& minPt, const HPoint& maxPt, HC_KEY segmentKey, int lodLayer)
{
	LODBMQuadtreeNode* _SON = m_Root->GetShellLocationOctreeNode((minPt + maxPt)/2.0f,lodLayer);
	LODBMQuadtreeSegemtNode _temp;
	_temp._minPt = minPt;
	_temp._maxPt = maxPt;
	_temp._segmenyKey = segmentKey;
	_SON->m_TesseNum = m_TesseNum + _SON->m_LodLayer * 2;
	_SON->m_vSegmentNodes.push_back(_temp);
}
void LODBMQuadtreeManager::ParseSegment(HC_KEY segmentKey, int lodLayer)
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
		//
		bool _hasMinMax = HC_Compute_Circumcuboid_By_Key(key, &_minPt, &_maxPt);
		if (_hasMinMax)
		{
			HC_Show_Net_Modelling_Matrix(_matirx);
			HC_Compute_Transformed_Points(1, &_minPt, _matirx, &_minPt);
			HC_Compute_Transformed_Points(1, &_maxPt, _matirx, &_maxPt);
			//
			_minPt.x -= m_Cartesian.m_x; _minPt.y -= m_Cartesian.m_y; _minPt.z -= m_Cartesian.m_z;
			_maxPt.x -= m_Cartesian.m_x; _maxPt.y -= m_Cartesian.m_y; _maxPt.z -= m_Cartesian.m_z;
			//
			int count = 0;
			bool flag = false;
			HC_Begin_Contents_Search(".", "geometry");
			HC_Show_Contents_Count(&count);
			for (int j = 0; j < count; ++j)
			{
				HC_Find_Contents(0, &_shellKey);
				HC_Show_Key_Type(_shellKey, _geoType);
				if (0 == strcmp("shell", _geoType) || 0 == strcmp("cylinder", _geoType) || 0 == strcmp("polycylinder", _geoType))
				{
					flag = true;
					break;
				}
			}
			HC_End_Contents_Search();
			if (flag)
				AddNode(_minPt, _maxPt, key,lodLayer);
		}
		//
		HC_Close_Segment();
	}
}
void LODBMQuadtreeManager::CreateBMMesh(LODBMQuadtreeNode* _pNode)
{
	if (_pNode == NULL) return;
	_pNode->CreateBMMesh(m_Cartesian.m_x, m_Cartesian.m_y, m_Cartesian.m_z);
	if (_pNode->m_pLeft_Up)
	{
		CreateBMMesh(_pNode->m_pLeft_Up);
		CreateBMMesh(_pNode->m_pLeft_Down);
		CreateBMMesh(_pNode->m_pRight_Up);
		CreateBMMesh(_pNode->m_pRight_Down);
	}
}
bool LODBMQuadtreeManager::CreateQuadtree()
{
	if (m_RootKey == INVALID_KEY) 
	if (m_HasCreate) return true;
	m_HasCreate = true;
	CreatImageMap();
	InitRootNode();
	//
	ParseSegment(m_RootKey,0);
	CreateBMMesh(m_Root);
	//
	for (int i = 1; i < m_vLODFilePaths.size();++i)
	{
		HC_Delete_By_Key(m_RootKey);
		//
		HC_Open_Segment("/include library");
		HC_KEY ModelKey = HC_Open_Segment("model");
		TK_Status sss = HTK_Read_Stream_File(m_vLODFilePaths[i].c_str());
		HC_Close_Segment();
		HC_Close_Segment();
		//
		m_RootKey = ModelKey;
		m_HoopsModelKey = ModelKey;
		//
		ParseSegment(m_RootKey, i);
		//
		CreateBMMesh(m_Root);
	}
	//
	return true;
}
#include "magick/ImageMagick.h"
void LODBMQuadtreeManager::CreatImageMap()
{
	std::map<std::string,HC_KEY > _Maps;
	HC_Open_Segment("/include library/texture images");
	{
		HC_Begin_Contents_Search(".", "images");
		char cType[128];
		cType[0] = '\0';
		HC_KEY keyImage = INVALID_KEY;
		while (HC_Find_Contents(0, &keyImage))
		{
			if (keyImage != INVALID_KEY)
			{
				char cName[MVO_BUFFER_SIZE] = { '\0' };
				HC_Show_Image_Name(keyImage, cName);
				_Maps[cName] = keyImage;
			}
		}
		HC_End_Contents_Search();
	}HC_Close_Segment();
	//
	if (_Maps.size() == 0)
	{
		HC_Open_Segment_By_Key(m_HoopsModelKey);
		HC_KEY kkk = HC_Open_Segment("model_include/texture images");
		char cTypewwwww[2048];
		HC_Show_Segment(kkk, cTypewwwww);
		//
		{
			HC_Begin_Contents_Search(".", "images");
			char cType[128];
			cType[0] = '\0';
			HC_KEY keyImage = INVALID_KEY;
			while (HC_Find_Contents(0, &keyImage))
			{
				if (keyImage != INVALID_KEY)
				{
					char cName[MVO_BUFFER_SIZE] = { '\0' };
					HC_Show_Image_Name(keyImage, cName);
					_Maps[cName] = keyImage;
				}
			}
			HC_End_Contents_Search();
		}
		HC_Close_Segment();
		HC_Close_Segment();
	}
	//
	for (auto &it : _Maps)
	{
		float fOutX, fOutY, fOutZ;
		char cFormat[MVO_BUFFER_SIZE];
		int nOutWidth, nOutHeight;
		HC_Show_Image_Size(it.second, &fOutX, &fOutY, &fOutZ, cFormat, &nOutWidth, &nOutHeight);
		char *pComma = NULL;
		if ((pComma = strchr(cFormat, ',')) != 0)
		{
			*pComma = '\0';
		}
		if (_stricmp(cFormat, "grayscale") && _stricmp(cFormat, "tga") && _stricmp(cFormat, "jpeg") &&
			_stricmp(cFormat, "rgb") && _stricmp(cFormat, "png") && _stricmp(cFormat, "rgba"))
		{
			continue;
		}
		//
		std::string strExt(cFormat);
		if (0 == strcmp(cFormat, "grayscale") || 0 == strcmp(cFormat, "rgb"))
		{
			strExt = "jpeg";
		}
		else if (0 == strcmp(cFormat, "rgba") || 0 == strcmp(cFormat, "png"))
		{
			strExt = "png";
		}
		//
		std::string strTxtFile = m_TileSetFileFolder + it.first + "." + strExt;
		int nFromCount = 3;
		if (!_stricmp(cFormat, "rgba"))
		{
			nFromCount = 4;
		}
		else if (!_stricmp(cFormat, "png"))
		{
			nFromCount = HC_Show_Image_Bytes_Per_Pixel(it.second);
		}
		if (nFromCount < 3) continue;
		//////////////////////////////////////////////////////////////////////////
		unsigned char *pImageData = (unsigned char*)malloc(nOutWidth*nOutHeight*nFromCount*sizeof(char));
		if (!_stricmp(cFormat, "grayscale"))
		{
			unsigned char *pGray = NULL;
			HC_Show_Geometry_Pointer(it.second, "grayscale rasters", &pGray);
			if (NULL != pGray)
			{
				for (int iRow = 0; iRow != nOutHeight; ++iRow)
				{
					for (int iCol = 0; iCol != nOutWidth; ++iCol)
					{
						for (int i = 0; i != nFromCount; ++i)
						{
							pImageData[nFromCount * (iRow * nOutWidth + iCol) + i] = pGray[iRow * nOutWidth + iCol];
						}
					}
				}
			}
		}
		else
		{
			HC_Show_Partial_Image(it.second, 0, 0, nOutWidth, nOutHeight, pImageData);
		}
		//
		ImageInfo *image_infor = 0;
		size_t count, length = 0;
		unsigned char *himage = 0;
		InitializeMagick(".");
		ExceptionInfo exceptionInfo;
		GetExceptionInfo(&exceptionInfo);
		Image* pImage = ConstituteImage(nOutWidth, nOutHeight, (nFromCount == 3) ? "RGB" : "RGBA", CharPixel, pImageData, &exceptionInfo);
		image_infor = CloneImageInfo((ImageInfo*)NULL);
		strncpy_s(image_infor->magick, strExt.c_str(), MaxTextExtent);
		image_infor->quality = 0;
		himage = ImageToBlob(image_infor, pImage, &length, &exceptionInfo);
		//
		if (himage == NULL)
		{
			if (image_infor) DestroyImageInfo(image_infor);
			DestroyExceptionInfo(&exceptionInfo);
			DestroyMagick();
			continue;
		}
		FILE* file = fopen(strTxtFile.c_str(), "wb");
		fwrite(himage, 1, length, file);
		fclose(file);
		m_ImageMaps[it.first] = strTxtFile;
		//
		if (pImage)
			DestroyImage(pImage);
		if (image_infor)
			DestroyImageInfo(image_infor);
		DestroyExceptionInfo(&exceptionInfo);
		DestroyMagick();
		free(pImageData);
		if (himage)
			free(himage);
	}
}
