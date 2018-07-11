#ifndef HDWFCONNECTOR_H
#define HDWFCONNECTOR_H

#include "HIOUtilityDwfImport.h"
#include "HIOManager.h"

#ifdef HIO_DWF_IMPORT
extern "C" {
	HIO_API void * CreateInput(HIOManager *manager);
	HIO_API void * CreateOutput(HIOManager *manager);
	HIO_API void * CreateConnector(void *unused);
	HIO_API void Free(HIOUtilityDwfImport *);
	HIO_API void FreeConnector(HIOConnector *connector);
}
#endif

#endif //HDWFCONNECTOR_H