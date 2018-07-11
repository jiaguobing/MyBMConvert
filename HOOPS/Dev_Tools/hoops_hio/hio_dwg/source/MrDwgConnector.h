// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#pragma once

#include <vector>
#include "HIOManager.h"

#ifdef HIO_DWG
#define HIO_API __declspec (dllexport)
#else
#define HIO_API
#endif

class AcDbDatabase;
class hio_dwg;

/*! This class establishes a connection between the DWG entity and the Hoops entity.
*/
class HIO_API HIOConnectorDWG : public HIOConnector
{
public:
	/*!Constructs an HIOConnectorDWG object.*/
	HIOConnectorDWG();
	~HIOConnectorDWG();

	/*! This method associates this connector with a given AcDbDatabase.
	\param pAcDbDatabase A pointer to the AcDbDatabase object you want this connector to be associated with.
	*/
	void SetDatabase(AcDbDatabase * pAcDbDatabase)
	{   
		// database needs to be alive for DWG pointers to be valid
		m_pAcDbDatabase = pAcDbDatabase;                  
	}

	// functions inherited from HIOConnector
	HC_KEY GetHoopsEntity(void * pKernelEntity, HC_KEY modelKey = INVALID_KEY);
	void * GetKernelEntity(HC_KEY key, HC_KEY modelKey = INVALID_KEY);
	bool GetHoopsEntities(void * pKernelEntity, vlist_s *ret_HoopsKeysList, HC_KEY modelKey = INVALID_KEY);
	bool GetKernelEntities(HC_KEY key, vlist_s *ret_KernelEntitiesList, HC_KEY modelKey = INVALID_KEY);
	void AddConnection(HC_KEY key, void* pKernelEntity, HC_KEY modelKey = INVALID_KEY);

protected:
	//! Autocad Database pointer associated with the DWG file.
	AcDbDatabase * m_pAcDbDatabase;

private:
	void RegenerateHash(HC_KEY modelKey);
	struct vhash_s *mhash_Hoops2Kernel;
	struct vhash_s *mhash_Kernel2Hoops;
};

extern "C" {
	HIO_API void * CreateInput(HIOManager *manager);
	HIO_API void * CreateOutput(HIOManager *manager);
	HIO_API void * CreateConnector(void * unused);
	HIO_API void Free(hio_dwg *);
	HIO_API void FreeConnector(HIOConnectorDWG *connector);
	
}

#ifdef H_PACK_8
#pragma pack(pop)
#endif
