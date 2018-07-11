
#include "base.h"
#include <ctime>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <filesystem>
//
#include "../Ahoy/ahoy.h"
#include "../GLTF/BMFile/BMConvert.h"
#include "../GLTF/LODBMFile/LODBMConvert.h"
#include "ExtractHSFFromB3D.h"
#include <windows.h>
#include "../GLTF/Terrain/Terrain.h"
//
using namespace std;
using namespace ahoy;
void TerrainDTile(string& demFilePath, string& imageFilePath, string& tileFolder,bool embedTex)
{
	Terrain23DTile tttt;
	tttt.m_DemFile = demFilePath;
	tttt.m_ImageFile = imageFilePath;
	tttt.m_OutputFolder = tileFolder;
	tttt.m_EmbeddedTextures = embedTex;
	//
	std::tr2::sys::path myfile(tttt.m_OutputFolder);
	if (!std::tr2::sys::exists(myfile))
	{
		std::tr2::sys::create_directories(myfile);
	}
	//
	tttt.Convert();
}
bool B3d23Dtile(string& b3dFilePath, string& tileFolder, int tessellationNum, bool embedTex,
	float x0, float y0, float z0, float l0, float b0, float h0,
	float x1, float y1, float z1, float l1, float b1, float h1)
{
	CExtractHSFFromB3D _extract;
	std::string _DBFilePath;
	std::string::size_type sfind = b3dFilePath.rfind(L'.');
	_DBFilePath = b3dFilePath.substr(0, sfind);
	_DBFilePath += ".db";
	//
	_extract.ExtractModel(b3dFilePath, _DBFilePath);
	b3dFilePath = b3dFilePath.substr(0, sfind);
	b3dFilePath += ".hsf";
	//
	std::cout << "b3d23dtile: " << b3dFilePath << " " << tileFolder << std::endl;
	HC_Open_Segment("/include library");
	HC_KEY ModelKey = HC_Open_Segment("model");
	TK_Status sss = HTK_Read_Stream_File(b3dFilePath.c_str());
	HC_Close_Segment();
	HC_Close_Segment();
	if (sss != TK_Complete)
	{
		return false;
	}
	//
	GLTF::BMConvert3DTile contttt;
	contttt.m_Cartesian = GLTF::BMVertex(x0, y0, z0);
	contttt.m_Cartographic = GLTF::BMVertex(l0, b0, h0);
	//
	contttt.m_CartesianS = GLTF::BMVertex(x1, y1, z1);
	contttt.m_CartographicS = GLTF::BMVertex(l1, b1, h1);
	//
	contttt.m_TesseNum = tessellationNum;
	contttt.m_RootKey = ModelKey;
	contttt.m_HoopsModelKey = ModelKey;
	contttt.m_TileSetFileFolder = tileFolder + "\\";
	contttt.m_DBFilePath = _DBFilePath;
	contttt.m_EmbeddedTextures = embedTex;
	//
	std::tr2::sys::path myfile(contttt.m_TileSetFileFolder);
	if (!std::tr2::sys::exists(myfile))
	{
		std::tr2::sys::create_directories(myfile);
	}
	//
	contttt.Convert();
	return true;
}
//////////////////////////////////////////////////////////////////////////
string FindInAll(string Path)
{
	string findFileName;
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = FindFirstFile(Path.c_str(), &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE)
		return findFileName;
	do
	{
		if (FindFileData.cFileName[0] == '.') 
			continue;
		if (!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			findFileName = FindFileData.cFileName;
			break;
		}
	} while (FindNextFile(hFind, &FindFileData));
	FindClose(hFind);
	//
	return findFileName;
}
//此处未处理数据库中 region
bool LODB3d23Dtile(string& lodb3dFileFolder, string& tileFolder, int tessellationNum,bool embedTex,
	float x0, float y0, float z0, float l0, float b0, float h0,
	float x1, float y1, float z1, float l1, float b1, float h1)
{
	string _tempStrs[10] = { lodb3dFileFolder + "\\Lod1", lodb3dFileFolder + "\\Lod2", lodb3dFileFolder + "\\Lod3", lodb3dFileFolder + "\\Lod4", lodb3dFileFolder + "\\Lod5",
		lodb3dFileFolder + "\\Lod6", lodb3dFileFolder + "\\Lod7", lodb3dFileFolder + "\\Lod8", lodb3dFileFolder + "\\Lod9", lodb3dFileFolder + "\\Lod10" };
	//
	string b3dfileName = FindInAll(_tempStrs[0] + "\\*.b3d");
	//
	CExtractHSFFromB3D _extract;
	GLTF::LODBMConvert3DTile contttt;
	for (auto& t:_tempStrs)
	{
		t += "\\" + b3dfileName;
		_extract.ExtractModel(t,t);
		//
		std::string::size_type sfind = t.rfind(L'.');
		t = t.substr(0, sfind);
		t += ".hsf";
		contttt.m_vLODFilePaths.push_back(t);
	}
	//////////////////////////////////////////////////////////////////////////
	//先处理最低精度模型
	HC_Open_Segment("/include library");
	HC_KEY ModelKey = HC_Open_Segment("model");
	TK_Status sss = HTK_Read_Stream_File(contttt.m_vLODFilePaths[0].c_str());
	HC_Close_Segment();
	HC_Close_Segment();
	if (sss != TK_Complete)
	{
		return false;
	}
	//
	contttt.m_Cartesian = GLTF::BMVertex(x0, y0, z0);
	contttt.m_Cartographic = GLTF::BMVertex(l0, b0, h0);
	//
	contttt.m_CartesianS = GLTF::BMVertex(x1, y1, z1);
	contttt.m_CartographicS = GLTF::BMVertex(l1, b1, h1);
	//
	contttt.m_TesseNum = tessellationNum;
	contttt.m_RootKey = ModelKey;
	contttt.m_HoopsModelKey = ModelKey;
	contttt.m_TileSetFileFolder = tileFolder + "\\";
	contttt.m_EmbeddedTextures = embedTex;
	//
	std::tr2::sys::path myfile(contttt.m_TileSetFileFolder);
	if (!std::tr2::sys::exists(myfile))
	{
		std::tr2::sys::create_directories(myfile);
	}
	//
	contttt.Convert();
	//
	return true;
}
//-1 参数错误
int main(int argc, const char **argv) 
{
	//b3d文件路径
	string _b3dFilePathName;
	//b3d lod 文件夹路径
	string _b3dLODFileFolder;
	//3dTile文件夹路径
	string _3dTileFileFolder;
	//Cylinder 细分数
	int _TessellationCylinderNum = 6;
	//是否将纹理数据写入gltf
	int _EmbeddedTextures = 1;
	//dem文件路径
	string _demFilePath;
	string _imageFilePath;
	//
	double X0(0.0), Y0(0.0), Z0(0.0);
	double L0(0.0), B0(0.0), H0(0.0);//L经度 B纬度
	//
	double X1(0.0), Y1(0.0), Z1(0.0);
	double L1(0.0), B1(0.0), H1(0.0);//L经度 B纬度
	//
	Parser* pParser = new Parser();
	pParser->name("b3d23dtile")
		->usage("b3d23dtile [options]")
		->example("b3d23dtile !input d:/test.b3d !output d:/destFolder      !x0 114.635 !y0 -49.224 !z0 20 !l0 113.23931614 !b0 23.09078922 !h0 0.0 !x1 -68.573 !y1 -59.252 !z1 20 !l1 113.23844564 !b1 23.09224637 !h1 0.0")
		->example("b3d23dtile !input d:/test.b3d !output d:/destFolder      !x0 114.635 !y0 -49.224 !z0 20 !l0 113.23931614 !b0 23.09078922 !h0 0.0")
		->example("b3d23dtile !inputLOD d:/11/11 !output d:/destFolder      !x0 114.635 !y0 -49.224 !z0 20 !l0 113.23931614 !b0 23.09078922 !h0 0.0 !x1 -68.573 !y1 -59.252 !z1 20 !l1 113.23844564 !b1 23.09224637 !h1 0.0")
	    ->example("b3d23dtile !inputLOD d:/11/11 !output d:/11/11/temp      !x0 114.635 !y0 -49.224 !z0 20 !l0 113.23931614 !b0 23.09078922 !h0 0.0")
	    ->example("b3d23dtile !inputDem d:/TerrainTEST/dem.tif !inputImage d:/TerrainTEST/image.tif !output d:/TerrainTEST/temp");
	//b3d23dtile !input d:/广州11号线.b3d !output d:/BMTEST      !x0 114.635 !y0 -49.224 !z0 20 !l0 113.23931614 !b0 23.09078922 !h0 0.0
	//b3d23dtile !inputDem d:/TerrainTEST/dem.tif !inputImage d:/TerrainTEST/image.tif !output d:/TerrainTEST/temp
	pParser->define("inputDem", &_demFilePath)
		->description("dem tifFilePath");

	pParser->define("inputImage", &_imageFilePath)
		->description("image tifFilePath");

	pParser->define("input", &_b3dFilePathName)
		->description("single b3d filePath");

	pParser->define("inputLOD", &_b3dLODFileFolder)
		->description("level of detail b3dFile folder");

	pParser->define("output", &_3dTileFileFolder)
		->description("3dtile folder")
		->require();

	pParser->define("t", &_TessellationCylinderNum)
		->alias("tessellatioNum")
		->description("cylinder tessellation number")
		->defaults(6);
	pParser->define("e", &_EmbeddedTextures)
		->alias("embeddedTextures")
		->description("embedded Textures in gltf")
		->defaults(1);

	pParser->define("X0", &X0)
		->alias("x0")
		->description("first point Cartesian X");
	pParser->define("Y0", &Y0)
		->alias("y0")
		->description("first point Cartesian Y");
	pParser->define("Z0", &Z0)
		->alias("z0")
		->description("first point Cartesian Z");
	pParser->define("L0", &L0)
		->alias("l0")
		->description("first point m_Cartographic longitude");
	pParser->define("B0", &B0)
		->alias("b0")
		->description("first point m_Cartographic latitude");
	pParser->define("H0", &H0)
		->alias("h0")
		->description("first point m_Cartographic height");

	pParser->define("X1", &X1)
		->alias("x1")
		->description("second point Cartesian X");
	pParser->define("Y1", &Y1)
		->alias("y1")
		->description("second point Cartesian Y");
	pParser->define("Z1", &Z1)
		->alias("z1")
		->description("second point Cartesian Z");
	pParser->define("L1", &L1)
		->alias("l1")
		->description("second point m_Cartographic longitude");
	pParser->define("B1", &B1)
		->alias("b1")
		->description("second point m_Cartographic latitude");
	pParser->define("H1", &H1)
		->alias("h1")
		->description("second point m_Cartographic height");
	//
	if (!pParser->parse(argc, argv))
	{
		system("pause");
	}

	delete pParser;
	bool embedTex = _EmbeddedTextures > 0;
	//
	if (_b3dFilePathName != "")
	{
		B3d23Dtile(_b3dFilePathName, _3dTileFileFolder, _TessellationCylinderNum, embedTex, X0, Y0, Z0, L0, B0, H0, X1, Y1, Z1, L1, B1, H1);
		return 1;
	}
	else if (_b3dLODFileFolder != "")
	{
		LODB3d23Dtile(_b3dLODFileFolder, _3dTileFileFolder, _TessellationCylinderNum, embedTex, X0, Y0, Z0, L0, B0, H0, X1, Y1, Z1, L1, B1, H1);
		return 1;
	}
	else if (_demFilePath != "" && _imageFilePath != "")
	{
		TerrainDTile(_demFilePath, _imageFilePath, _3dTileFileFolder, embedTex);
	}
	else
		return -1;
}
