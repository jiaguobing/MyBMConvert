#include "ExtractHSFFromB3D.h"
#include <openssl/md5.h>
#include <algorithm>
#include  <windows.h>  
#include "../zlib/zlib.h"
#include "../zlib/zconf.h"
bool IsRightFileVersion(char* _fileVersion, std::string sCurrentProVersion)
{
	std::string _current_file_version = "BIM3D V" + sCurrentProVersion + "\n";
	std::string fileTemp = _fileVersion;
	if (fileTemp.find("BIM3D V") != 0)
		return false;
	return _fileVersion > _current_file_version ? false : true;
}

//读取坐标变量
double g_xOffset = 0.0;
double g_yoffset = 0.0;
double g_zoffset = 0.0;
std::string g_CurrentFileEndTime = "";

std::string BMd5(const char*  _codeStr)
{
	MD5_CTX hash_ctx;
	unsigned char hash_ret[16] = { 0 };

	MD5_Init(&hash_ctx);
	MD5_Update(&hash_ctx, _codeStr, strlen(_codeStr));
	MD5_Final(hash_ret, &hash_ctx);

	std::string sMd5 = "";
	//
	for (int i = 0; i < 32; ++i)
	{
		char szMd5[8] = { 0 };
		if (i % 2 == 0)
		{
			sprintf_s(szMd5, "%x", (hash_ret[i / 2] >> 4) & 0xf);
		}
		else
		{
			sprintf_s(szMd5, "%x", (hash_ret[i / 2]) & 0xf);
		}

		sMd5 += szMd5;
	}

	transform(sMd5.begin(), sMd5.end(), sMd5.begin(), toupper);
	return sMd5;
}

bool CheckFile(std::vector<std::string> _guidlist, char* const _EntryString, const char* version)
{
	bool bRet = false;
	//拼接加密字符串
	std::string _codestr = "1";
	for (auto it = _guidlist.begin(); it != _guidlist.end(); it++)
	{
		_codestr += (*it);
	}
	char _checkstring[256];
	sprintf_s(_checkstring, "%f%f%f%s", g_xOffset, g_yoffset, g_zoffset,
		g_CurrentFileEndTime.c_str());
	_codestr += version;
	_codestr += _checkstring;
	_codestr += "lizhensheng&puyan";
	//对字符串进行MD5
	if (_EntryString == BMd5(_codestr.c_str()))
	{
		bRet = true;
	}
	return bRet;
}

std::string GetExtension(std::string const& strFileName)
{
	std::string::size_type sfind = strFileName.rfind(L'.');
	if (sfind == std::string::npos)
		return "";
	return strFileName.substr(sfind + 1, strFileName.size() - sfind - 1);
}

std::string GetFileName(std::string const& strFileName)
{
	std::string::size_type findDot = strFileName.rfind(L'.');
	if (findDot == std::string::npos)
		return "";
	std::string::size_type findSlash = strFileName.rfind("/", findDot);
	if (findDot == std::string::npos)
		return "";
	return strFileName.substr(findSlash + 1, findDot - findSlash - 1);
}

std::string GetPath(std::string const& strFileName)
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
CExtractHSFFromB3D::CExtractHSFFromB3D()
{
}


CExtractHSFFromB3D::~CExtractHSFFromB3D()
{
}
void CExtractHSFFromB3D::ExtractModel(std::string b3dFile,  std::string dbFile)
{
	FILE *pFile = NULL;
	errno_t _error_int = fopen_s(&pFile, b3dFile.c_str(), "rb+");
	if (_error_int != 0)
	{
		return;
	}

	//读取文件头字符串
	char* _pFileHeaderString = new char[12];
	_pFileHeaderString[11] = '\0';
	fread(_pFileHeaderString, 11, 1, pFile);
	//判断文件版本
	if (!IsRightFileVersion(_pFileHeaderString, "1.0"))
	{
		delete[] _pFileHeaderString;
		_pFileHeaderString = NULL;
		fclose(pFile);
		return;
	}
	delete[] _pFileHeaderString;
	_pFileHeaderString = NULL;

	//读取GUID数量
	int _guidcount = 0;
	fread(&_guidcount, sizeof(int), 1, pFile);
	if (_guidcount == 0 || _guidcount > 1000)
	{
		fclose(pFile);
		return;
	}
	//读取GUID数据
	std::vector<std::string> _guidList;
	for (int index = 0; index < _guidcount; index++)
	{
		char *_pGUID = new char[37];
		_pGUID[36] = '\0';
		fread(_pGUID, 36, 1, pFile);
		_guidList.push_back(_pGUID);
		delete[] _pGUID;
		_pGUID = NULL;
	}

	//读取版本验证字符“A”
	char version[2]; version[1] = '\0';
	fread(version, 1, 1, pFile);
	//读取坐标变量
	fread(&g_xOffset, sizeof(double), 1, pFile);
	fread(&g_yoffset, sizeof(double), 1, pFile);
	fread(&g_zoffset, sizeof(double), 1, pFile);
	//读取到期时间

	char* pCurrentEndTime = new char[9];
	pCurrentEndTime[8] = '\0';
	fread(pCurrentEndTime, 8, 1, pFile);
	g_CurrentFileEndTime = pCurrentEndTime;
	delete[] pCurrentEndTime; pCurrentEndTime = NULL;
	if (g_CurrentFileEndTime != "00000000")
	{
		SYSTEMTIME sys;   GetLocalTime(&sys);
		char _currentSystime[9];
		sprintf_s(_currentSystime, "%4d%02d%02d", sys.wYear, sys.wMonth, sys.wDay);
		_currentSystime[8] = '\0';
		if (g_CurrentFileEndTime.compare(_currentSystime) == -1)
		{
			_guidList.clear();
			fclose(pFile);
			return;
		}
	}
	//读取加密验证
	char* _enctptyData = new char[33];
	_enctptyData[32] = '\0';
	fread(_enctptyData, 32, 1, pFile);
	// 20171128修改验证
	//验证加密字符串
	if (!CheckFile(_guidList, _enctptyData, version))
	{
		_guidList.clear();
		delete[] _enctptyData;
		_enctptyData = NULL;
		fclose(pFile);
		return;
	}
	delete[] _enctptyData;
	_enctptyData = NULL;

	// 20171128修改验证
	//读取数据库数据
	int _nLength = -1;
	fread(&_nLength, sizeof(int), 1, pFile);
	int _nZlibLength = -1;
	fread(&_nZlibLength, sizeof(int), 1, pFile);
	// 获取目录
	std::string strPath = GetPath(b3dFile);
	std::string strFileName = GetFileName(b3dFile);
	// 老版本
	if (0 == strcmp(version, "A"))
	{
		if (0 < _nZlibLength)
		{
			char *_pBuff = new char[_nZlibLength + 1];
			fread(_pBuff, 1, _nZlibLength, pFile);
			_pBuff[_nZlibLength] = '\0';

			unsigned long destLene = _nLength;
			unsigned char *deste = new unsigned char[destLene + 1];
			memset(deste, 0x0, destLene + 1);
			int nRet = uncompress(deste, &destLene, (const Bytef*)_pBuff, _nZlibLength);
		    //
			FILE *_pFileTempDB = 0;
			errno_t _errno = fopen_s(&_pFileTempDB, dbFile.c_str(), "wb");
			if (0 == _errno)
			{
				// 将数据库信息写到磁盘文件
				fwrite(deste, _nLength, 1, _pFileTempDB);
				fclose(_pFileTempDB);
			}
			delete[] deste; deste = NULL;
			delete[] _pBuff; _pBuff = NULL;

		}

		//读取HSF文件数据
		int _3gdLen = -1;
		fread(&_3gdLen, sizeof(int), 1, pFile);  //3gd文件长度

		if (-1 == _3gdLen)
		{
			fclose(pFile);
			return;
		}
		else
		{
			char* _pGDBuff = new char[_3gdLen + 1];
			_pGDBuff[_3gdLen] = '\0';
			fread(_pGDBuff, _3gdLen, 1, pFile);
			// 内容	_pGDBuff
			// 长度 _3gdLen
			std::string strHSFPath = strPath;
			strHSFPath.append(strFileName);
			strHSFPath.append(".hsf");
			FILE *_pFileTempHSF = 0;
			errno_t _errno = fopen_s(&_pFileTempHSF, strHSFPath.c_str(), "wb");
			if (0 == _errno)
			{
				fwrite(_pGDBuff, _3gdLen, 1, _pFileTempHSF);
			}
			delete[] _pGDBuff; _pGDBuff = 0;
			fclose(_pFileTempHSF);
			return;
		}
	}
	else if (0 == strcmp(version, "C"))
	{
		// 数据
		std::vector<char*> vecHpsBuffer;
		std::vector<int>	vecHpsBufferLength;
		int _3gdLen = -1;
		fread(&_3gdLen, sizeof(int), 1, pFile);  //3gd文件长度
		if (-1 == _nZlibLength)
		{
			fclose(pFile);
			return;
		}
		else if (-1 == _3gdLen)
		{
			fclose(pFile);
			return;
		}
		else if (_nZlibLength > 0 && _3gdLen > 0)
		{
			// 单个db长度
			int iSegmentDBFileLegth = _nZlibLength / 10;
			// 单个b3d长度
			int iSegmentB3DFileLegth = _3gdLen / 10;
			int iLastDBSegmentLength = _nZlibLength % 10;
			int iLastB3DSegmentLength = _3gdLen % 10;
			char *_pBuff = new char[_nZlibLength + 1];
			memset(_pBuff, 0x0, sizeof(_pBuff));
			// std::string strBuff;
			int nCurrent = 0;
			for (int index = 0; index < 10; index++)
			{
				char* pTempDB = new char[iSegmentDBFileLegth];
				try
				{
					// 读db
					int nLength = fread(pTempDB, iSegmentDBFileLegth, sizeof(char), pFile);
					for (int n = 0; n != iSegmentDBFileLegth; ++n)
					{
						_pBuff[nCurrent] = pTempDB[n];
						nCurrent++;
					}
					// 读hsf
					char* pTempHSF = new char[iSegmentB3DFileLegth];
					fread(pTempHSF, iSegmentB3DFileLegth, sizeof(char), pFile);
					vecHpsBuffer.push_back(pTempHSF);
					vecHpsBufferLength.push_back(iSegmentB3DFileLegth);
				}
				catch (...)
				{
					delete[] pTempDB;
					pTempDB = 0;
					if (vecHpsBuffer.size())
					{
						for (std::vector<char*>::iterator _begin = vecHpsBuffer.begin();
							_begin != vecHpsBuffer.end(); ++_begin)
						{
							delete *_begin;
							*_begin = 0;
						}
						vecHpsBuffer.clear();
						vecHpsBuffer.swap(std::vector<char*>());
						vecHpsBufferLength.clear();
						vecHpsBufferLength.swap(std::vector<int>());
					}
					delete[] _pBuff;
					_pBuff = 0;
					fclose(pFile);
					return;
				}
				delete[] pTempDB;
				pTempDB = 0;
			}
			// 读取最后一段
			char* pTempDB = new char[iLastDBSegmentLength + 1];
			try
			{
				// 读db
				fread(pTempDB, iLastDBSegmentLength, 1, pFile);
				for (int n = 0; n != iLastDBSegmentLength; ++n)
				{
					_pBuff[nCurrent] = pTempDB[n];
					nCurrent++;
				}
				// 读hsf
				char* pTempHSF = new char[iLastB3DSegmentLength];
				fread(pTempHSF, iLastB3DSegmentLength, 1, pFile);
				vecHpsBuffer.push_back(pTempHSF);
				vecHpsBufferLength.push_back(iLastB3DSegmentLength);
				//
				delete[] pTempDB;
				pTempDB = 0;
			}
			catch (...)
			{
				delete[] pTempDB;
				pTempDB = 0;
				if (vecHpsBuffer.size())
				{
					for (std::vector<char*>::iterator _begin = vecHpsBuffer.begin();
						_begin != vecHpsBuffer.end(); ++_begin)
					{
						delete *_begin;
						*_begin = 0;
					}
					vecHpsBuffer.clear();
					vecHpsBuffer.swap(std::vector<char*>());
					vecHpsBufferLength.clear();
					vecHpsBufferLength.swap(std::vector<int>());
				}
				delete[] _pBuff;
				_pBuff = 0;
				fclose(pFile);
				return;
			}
			//
			//写数据库
			unsigned long destLene = _nLength;
			unsigned char *deste = new unsigned char[destLene + 1];
			memset(deste, 0x0, destLene + 1);
			int nRet = uncompress(deste, &destLene, (const Bytef*)_pBuff, _nZlibLength);
			//
			FILE *_pFileTempDB = 0;
			errno_t _errno = fopen_s(&_pFileTempDB, dbFile.c_str(), "wb");
			if (0 == _errno)
			{
				// 将数据库信息写到磁盘文件
				fwrite(deste, _nLength, 1, _pFileTempDB);
				fclose(_pFileTempDB);
			}
			delete[] deste; deste = NULL;
			delete[] _pBuff; _pBuff = NULL;
		}
		fclose(pFile);
		pFile = 0;
		// 写数据库内容
		std::string strHSFPath = strPath;
		strHSFPath.append(strFileName);
		strHSFPath.append(".hsf");
		FILE *_pFileTempHSF = 0;
		errno_t _errno = fopen_s(&_pFileTempHSF, strHSFPath.c_str(), "wb");
		if (0 == _errno)
		{
			for (int nIndex = 0; nIndex != vecHpsBuffer.size();
				++nIndex)
			{
				fwrite(vecHpsBuffer.at(nIndex), vecHpsBufferLength.at(nIndex), 1, _pFileTempHSF);
				delete[]vecHpsBuffer.at(nIndex);
			}
		}
		fclose(_pFileTempHSF);
	}
	if (pFile != 0)
	{
		fclose(pFile);
		pFile = 0;
	}
}
