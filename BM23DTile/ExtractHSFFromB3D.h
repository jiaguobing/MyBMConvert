#pragma once
#include <ctime>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include "vector"
using namespace std;
class CExtractHSFFromB3D
{
public:
	CExtractHSFFromB3D();
	~CExtractHSFFromB3D();
	void ExtractModel(std::string b3dFile, std::string dbFile);
};

