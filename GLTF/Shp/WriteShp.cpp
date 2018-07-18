#define  _CRT_SECURE_NO_WARNINGS
#include "WriteShp.h"
#include "gdal_priv.h"
#include "gdal.h"

#include "ogr_api.h"
#include "ogrsf_frmts.h"
#include <windows.h>  
#include <locale.h>
#include "string"
#include <algorithm>
using namespace std;
std::string GetPath0(std::string const& strFileName)
{
	std::string::size_type wfind = strFileName.rfind('.');
	// 没有.
	if (wfind == std::string::npos)
	{
		wfind = strFileName.rfind(L'/');
		if (wfind == std::string::npos)
			return "";
		if (wfind == strFileName.size() - 1)
		{
			return strFileName;
		}
		else
		{
			std::string _strFileName = strFileName;
			_strFileName.append("/");
			return _strFileName;
		}
	}
	// 有.
	else
	{
		std::string::size_type wfindSecond = strFileName.rfind('/', wfind);
		if (wfindSecond == std::string::npos)
			return "";
		return strFileName.substr(0, wfindSecond + 1);
	}
}
std::string GetFileName0(std::string const& strFileName)
{
	std::string::size_type findDot = strFileName.rfind(L'.');
	if (findDot == std::string::npos)
		return "";
	std::string::size_type findSlash = strFileName.rfind("/", findDot);
	if (findDot == std::string::npos)
		return "";
	return strFileName.substr(findSlash + 1, findDot - findSlash - 1);
}
std::string WriteShap::MultiByteToUTF8(std::string& mutlStr)
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
void WriteShap::Write()
{
	string fileName = GetFileName0(m_ShpFile);
	string dbFilePath = GetPath0(m_ShpFile) + fileName + ".db";
	m_SQLHelper.m_DBPath = MultiByteToUTF8(dbFilePath);
	m_SQLHelper.OpenDB(false);
	string sql = "CREATE TABLE model_tree (id text,pid text,name text,flag int,tid text,centerX Real,centerY Real,centerZ Real,radius Real);";
	m_SQLHelper.SqlStateExec(sql.c_str());
	sql = "CREATE TABLE model_property (id text,name text,value text,unit text,propertygroup text,description int);";
	m_SQLHelper.SqlStateExec(sql.c_str());
	sql = "CREATE TABLE object_type (id text,pid text,name text,visibily int,description text,tileGUID text);";
	m_SQLHelper.SqlStateExec(sql.c_str());
	//
	string tileGUID = GetGUIDStr();
	string typeGUID = GetGUIDStr();
	string rootGUID = GetGUIDStr();
	sql = "INSERT INTO object_type VALUES('";
	sql += typeGUID; sql += "','','build',1,'CityEngine模型','";
	sql += tileGUID; sql += "');";
	sql = MultiByteToUTF8(sql);
	m_SQLHelper.SqlStateExec(sql.c_str());
	//
	GLTF::BMConvert3DTile tempUseForTran;
	tempUseForTran.m_Cartesian = m_Cartesian;
	tempUseForTran.m_CartesianS = m_CartesianS;
	tempUseForTran.m_Cartographic = m_Cartographic;
	tempUseForTran.m_CartographicS = m_CartographicS;
	
	GLTF::BMVertexDD origin = GLTF::BMVertexDD(m_Cartographic.m_x, m_Cartographic.m_y, m_Cartographic.m_z);
	tempUseForTran.EastNorthUpToFixedFrame(origin, m_transform);
	tempUseForTran.ComputeRotation(m_transform);
	//
	HPoint _toalMinPt = HPoint(100000,100000,100000);
	HPoint _toalMaxPt = HPoint(-100000, -100000, -100000);
	const char *pszDriverName = "ESRI Shapefile";
	GDALDriver *poDriver;
	GDALAllRegister();
	CPLSetConfigOption("SHAPE_ENCODING", "");
	poDriver = GetGDALDriverManager()->GetDriverByName(pszDriverName);
	if (poDriver == NULL)
	{
		printf("%s driver not available.\n", pszDriverName);
		return ;
	}
	GDALDataset *poDS;
	poDS = poDriver->Create(m_ShpFile.c_str(), 0, 0, 0, GDT_Unknown, NULL); 
	if (poDS == NULL)
	{
		printf("Creation of output file failed.\n");
		return ;
	}
	OGRLayer *poLayer;
	poLayer = poDS->CreateLayer("polygon", NULL, wkbPolygon, NULL);
	if (poLayer == NULL)
	{
		printf("Layer creation failed.\n");
		return ;
	}
	OGRFieldDefn nameField("name", OFTString);
	OGRFieldDefn buildTypeField("type", OFTString);
	OGRFieldDefn floorHeightField("fHeight", OFTReal);
	OGRFieldDefn floorNumField("floorNum", OFTInteger);
	OGRFieldDefn totalFloorHeightField("totalFH", OFTReal);
	OGRFieldDefn AreaField("Area", OFTReal);
	OGRFieldDefn baseHeightField("bHeight", OFTReal);
	nameField.SetWidth(32);
	buildTypeField.SetWidth(32);
	floorHeightField.SetPrecision(3);
	floorNumField.SetWidth(32);
	totalFloorHeightField.SetPrecision(3);
	AreaField.SetPrecision(4);
	baseHeightField.SetPrecision(3);
	poLayer->CreateField(&nameField);
	poLayer->CreateField(&buildTypeField);
	poLayer->CreateField(&floorHeightField);
	poLayer->CreateField(&floorNumField);
	poLayer->CreateField(&totalFloorHeightField);
	poLayer->CreateField(&AreaField);
	poLayer->CreateField(&baseHeightField);
	//
	m_SQLHelper.BeginTransaction();
	//
	ifstream  ifile(m_TxtFile.c_str(), ios::in);
	string polygonLine;
	string attrLine;
	string ptLine;
	string tempStr;
	string buildType;
	double xx, yy, zz; 
	double baseHeight;
	double _CartesianX, _CartesianY, _CartesianZ;
	int nameIndex = 0;
	string nodeGUID;
	while (getline(ifile, polygonLine))
	{
		HPoint _minPt = HPoint(100000, 100000, 100000);
		HPoint _maxPt = HPoint(-100000, -100000, -100000);
		//
		OGRFeature *poFeature;
		poFeature = OGRFeature::CreateFeature(poLayer->GetLayerDefn());
		nodeGUID = GetGUIDStr();
		//
		//住宅楼248;低层;3;3;9;12.0260587785812;6.75;
		getline(ifile, attrLine);
		istringstream istr0(attrLine);
		getline(istr0, tempStr, ';');//
		poFeature->SetField("name", nodeGUID.c_str());
		getline(istr0, buildType, ';');//
		poFeature->SetField("type", buildType.c_str());
		getline(istr0, tempStr, ';');//
		double fh = atof(tempStr.c_str());
		poFeature->SetField("fHeight", fh);
		getline(istr0, tempStr, ';');//
		int floorNum = atoi(tempStr.c_str());
		poFeature->SetField("floorNum", floorNum);
		getline(istr0, tempStr, ';');//
		double tFH = atof(tempStr.c_str());
		tFH = floorNum * fh;
		poFeature->SetField("totalFH", tFH);
		getline(istr0, tempStr, ';');//
		double area = atof(tempStr.c_str());
		poFeature->SetField("Area", area);
		getline(istr0, tempStr, ';');//
		baseHeight = atof(tempStr.c_str()); if (baseHeight > 500 || baseHeight < 5) baseHeight = 6.5; 

		poFeature->SetField("bHeight", baseHeight);
		if (area < 10 || floorNum < 1)
		{
			OGRFeature::DestroyFeature(poFeature);
			continue;
		}
		//
		istringstream istr(polygonLine);
		OGRLinearRing ring;
		while (getline(istr, ptLine, ';'))
		{
			istringstream kkkkk(ptLine);
			getline(kkkkk, tempStr, ',');
			xx = atof(tempStr.c_str());
			getline(kkkkk, tempStr, ',');
			yy = atof(tempStr.c_str());
			getline(kkkkk, tempStr, ',');
			zz = atof(tempStr.c_str());
			zz = 0.0;
			//
			ring.addPoint(xx, yy, zz + baseHeight);
			_minPt.x = min(_minPt.x, xx); _minPt.y = min(_minPt.y, yy); _minPt.z = min(_minPt.z, zz + baseHeight);
			_maxPt.x = max(_maxPt.x, xx); _maxPt.y = max(_maxPt.y, yy); _maxPt.z = max(_maxPt.z, zz + baseHeight + tFH);
			//
			_toalMinPt.x = min(_minPt.x, _toalMinPt.x); _toalMinPt.y = min(_minPt.y, _toalMinPt.y); _toalMinPt.z = min(_minPt.z, _toalMinPt.z);
			_toalMaxPt.x = max(_maxPt.x, _toalMaxPt.x); _toalMaxPt.y = max(_maxPt.y, _toalMaxPt.y); _toalMaxPt.z = max(_maxPt.z, _toalMaxPt.z);
		}
		ring.closeRings();
		OGRPolygon polygon;
		polygon.addRing(&ring);
		poFeature->SetGeometry(&polygon);
		//
		poLayer->CreateFeature(poFeature);
		OGRFeature::DestroyFeature(poFeature);
		//
		HPoint _centerPt = (_minPt + _maxPt) / 2.0f;
		MatrixMultiplyVector(_centerPt.x, _centerPt.y, _centerPt.z, _CartesianX, _CartesianY, _CartesianZ);
		float maxRadius = max(max(_maxPt.x - _minPt.x, _maxPt.y - _minPt.y), _maxPt.z - _minPt.z);
		//
		sql = "INSERT INTO model_tree VALUES('";
		sql += nodeGUID; sql += "','";
		sql += rootGUID; sql += "','building";
		sql += to_string(nameIndex); sql += "',2,'";
		sql += typeGUID; sql += "',";
		sql += to_string(_CartesianX); sql += ",";
		sql += to_string(_CartesianY); sql += ",";
		sql += to_string(_CartesianZ); sql += ",";
		sql += to_string(maxRadius); sql += ");";
		sql = MultiByteToUTF8(sql);
		m_SQLHelper.SqlStateExec(sql.c_str());
		//
		sql = "INSERT INTO model_property VALUES('";
		sql += nodeGUID; sql += "','名称','building";
		sql += to_string(nameIndex); sql += "','','房屋参数',0);";
		sql = MultiByteToUTF8(sql);
		m_SQLHelper.SqlStateExec(sql.c_str());
		//
		sql = "INSERT INTO model_property VALUES('";
		sql += nodeGUID; sql += "','类型','";
		sql += buildType; sql += "','','房屋参数',0);";
		sql = MultiByteToUTF8(sql);
		m_SQLHelper.SqlStateExec(sql.c_str());
		//
		sql = "INSERT INTO model_property VALUES('";
		sql += nodeGUID; sql += "','层数','";
		sql += to_string(floorNum); sql += "','','房屋参数',0);";
		sql = MultiByteToUTF8(sql);
		m_SQLHelper.SqlStateExec(sql.c_str());
		//
		sql = "INSERT INTO model_property VALUES('";
		sql += nodeGUID; sql += "','层高','";
		sql += to_string(fh); sql += "','m','房屋参数',0);";
		sql = MultiByteToUTF8(sql);
		m_SQLHelper.SqlStateExec(sql.c_str());
		//
		sql = "INSERT INTO model_property VALUES('";
		sql += nodeGUID; sql += "','楼高','";
		sql += to_string(tFH); sql += "','m','房屋参数',0);";
		sql = MultiByteToUTF8(sql);
		m_SQLHelper.SqlStateExec(sql.c_str());
		//
		sql = "INSERT INTO model_property VALUES('";
		sql += nodeGUID; sql += "','面积','";
		sql += to_string(area); sql += "','m2','房屋参数',0);";
		sql = MultiByteToUTF8(sql);
		m_SQLHelper.SqlStateExec(sql.c_str());
		//
		sql = "INSERT INTO model_property VALUES('";
		sql += nodeGUID; sql += "','底面标高','";
		sql += to_string(baseHeight); sql += "','m','房屋参数',0);";
		sql = MultiByteToUTF8(sql);
		m_SQLHelper.SqlStateExec(sql.c_str());
		//
		nameIndex++;
	}
	ifile.close();
	GDALClose(poDS);
	//
	HPoint _centerPt = (_toalMinPt + _toalMaxPt) / 2.0f;
	MatrixMultiplyVector(_centerPt.x, _centerPt.y, _centerPt.z, _CartesianX, _CartesianY, _CartesianZ);
	float maxRadius = max(max(_toalMaxPt.x - _toalMinPt.x, _toalMaxPt.y - _toalMinPt.y), _toalMaxPt.z - _toalMinPt.z);
	sql = "INSERT INTO model_tree VALUES('";
	sql += rootGUID; sql += "','','building',0,'',";
	sql += to_string(_CartesianX); sql += ",";
	sql += to_string(_CartesianY); sql += ",";
	sql += to_string(_CartesianZ); sql += ",";
	sql += to_string(maxRadius); sql += ");";
	sql = MultiByteToUTF8(sql);
	m_SQLHelper.SqlStateExec(sql.c_str());
	//
	m_SQLHelper.CommitTransaction();
	m_SQLHelper.CloseDB();
	//
	sql = "minPt:"; 
	sql += to_string(_toalMinPt.x); sql += ","; sql += to_string(_toalMinPt.y); sql += ","; sql += to_string(_toalMinPt.z);
	printf(sql.c_str()); printf("\n");

	sql = "maxPt:";
	sql += to_string(_toalMaxPt.x); sql += ","; sql += to_string(_toalMaxPt.y); sql += ","; sql += to_string(_toalMaxPt.z);
	printf(sql.c_str()); printf("\n");
	//
	printf("done!");
}
//////////////////////////////////////////////////////////////////////////
std::string WriteShap::GetGUIDStr()
{
	_GUID guid;
	CoCreateGuid(&guid);

	char buf[64] = { 0 };
	_snprintf_s(
		buf,
		sizeof(buf),
		//"{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}",
		"%08X_%04X_%04X_%02X%02X_%02X%02X%02X%02X%02X%02X",
		guid.Data1, guid.Data2, guid.Data3,
		guid.Data4[0], guid.Data4[1],
		guid.Data4[2], guid.Data4[3],
		guid.Data4[4], guid.Data4[5],
		guid.Data4[6], guid.Data4[7]);

	std::string rStr = std::string(buf);
	transform(rStr.begin(), rStr.end(), rStr.begin(), tolower);
	return rStr;
}
void WriteShap::MatrixMultiplyVector(double x, double y, double z, double& outX, double& outY, double& outZ)
{
	outX = m_transform[0] * x + m_transform[4] * y + m_transform[8] * z + m_transform[12];
	outY = m_transform[1] * x + m_transform[5] * y + m_transform[9] * z + m_transform[13];
	outZ = m_transform[2] * x + m_transform[6] * y + m_transform[10] * z + m_transform[14];
}