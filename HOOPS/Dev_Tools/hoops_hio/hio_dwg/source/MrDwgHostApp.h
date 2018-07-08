// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// RealDWG includes
#include "acgi.h"
#include "acestext.h"
#include "acdbxref.h"
#include <windows.h>
#include "adesk.h"

#include "dbapserv.h"
#include "dbacis.h"
#include "dbmstyle.h"
#include "dbsymtb.h"
#include "dbents.h"
#include "dbelipse.h"
#include "dbspline.h"
#include "dblead.h"
#include "dbray.h"
#include "dbxline.h"
#include "dbmline.h"
#include "dbbody.h"
#include "dbregion.h"
#include "dbsol3d.h"
#include "rxregsvc.h"
#include <map>
#include <string>

#include "misc.h"


class MrDwgHostApp : public AcDbHostApplicationServices
{
private:
	
	struct FindFileKey
	{
		int nBufferLength;
		AcDbDatabase * pDb;
		AcDbHostApplicationServices::FindFileHint hint;
		char pcFilenameHash[DWG_HASH_SIZE];

		bool operator < (const struct FindFileKey & rhs) const
		{
			if (memcmp(this, &rhs, sizeof(struct FindFileKey) < 0))
				return true;
			return false;
		}
	};

	struct FindFileResult
	{
		DWORD result;
		std::wstring pcFullPathOut;
	};

	std::map<struct FindFileKey, struct FindFileResult> FindFileMap;


public:
	Acad::ErrorStatus findFile(
		ACHAR* pcFullPathOut,
		int nBufferLength,
		const ACHAR* pcFilename,
		AcDbDatabase* pDb = NULL,
		AcDbHostApplicationServices::FindFileHint hint = kDefault);

	Acad::ErrorStatus getRoamableRootFolder(const char*& folder);
	Acad::ErrorStatus getLocalRootFolder(const char*& folder);
    Adesk::Boolean isURL(const char* pszURL) const;

    Adesk::Boolean isRemoteFile(const char* pszLocalFile, char* pszURL) const;
#if _MSC_VER == 1700
	virtual const ACHAR * getAlternateFontName() const;
#else
	virtual ACHAR * getAlternateFontName() const;
#endif
	virtual void fatalError(const ACHAR * format,...);
};

extern MrDwgHostApp gMrDwgHostApp;
