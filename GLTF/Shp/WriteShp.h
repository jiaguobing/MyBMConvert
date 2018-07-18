#pragma once
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream> 
#include "../BMFile/SqliteHelper.h"
#include "../BMFile/BMConvert.h"
//
class WriteShap
{
public:
	std::string m_TxtFile;
	std::string m_ShpFile;
	SqliteHelper m_SQLHelper;
	void Write();
	GLTF::BMVertex m_Cartesian;//模型笛卡尔坐标
	GLTF::BMVertex m_Cartographic;//模型经纬度坐标
	GLTF::BMVertex m_CartesianS;//模型笛卡尔坐标2---计算旋转角
	GLTF::BMVertex m_CartographicS;//模型经纬度坐标2---计算旋转角
	//
	double m_transform[16];
private:
	std::string MultiByteToUTF8(std::string& mutlStr);
	std::string GetGUIDStr(); 
	void MatrixMultiplyVector(double x, double y, double z, double& outX, double& outY, double& outZ);
};
