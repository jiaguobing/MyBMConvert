#ifndef HW3DREADER_H
#define HW3DREADER_H

#include "HW3DStream.h"

#include <vector>

#include "hc.h"
#include "HIOManager.h"
#include "HStream.h"
#include "HStreamFileToolkit.h"

#include "dwfcore/InputStream.h"

class HW3DReader
{
public:

	HW3DReader(DWFCore::DWFInputStream * rW3DStream, HInputHandlerOptions * pInputHandlerOptions);
	virtual ~HW3DReader() {}

	bool read();

protected:
	HInputHandlerOptions * m_pInputHandlerOptions;
	DWFCore::DWFInputStream * m_rW3DStream;
};

#endif //HW3DREADER_H