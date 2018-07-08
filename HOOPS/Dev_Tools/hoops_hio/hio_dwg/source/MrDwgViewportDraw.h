// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#pragma once

#include "hc.h"
#include "MrDwgReader.h"
#include <vector>
#include <acgi.h>

class MrDwgReader;

class MrDwgViewportDraw : public AcGiViewportDraw
{
public:
	ACRX_DECLARE_MEMBERS(MrDwgViewportDraw);
	MrDwgViewportDraw(std::vector<HC_KEY> * keys, MrDwgReader * reader);
	~MrDwgViewportDraw();

	void setEntity(AcDbEntity * pEnt);
    AcGiViewport& viewport() const;
    AcGiViewportGeometry& geometry() const;
    Adesk::UInt32 sequenceNumber() const;
    Adesk::Boolean isValidId(const Adesk::ULongPtr acgiId) const;
    AcDbObjectId viewportObjectId() const;

	AcGiRegenType regenType() const;
	Adesk::Boolean regenAbort() const;
    AcGiSubEntityTraits& subEntityTraits() const;
    AcGiGeometry * rawGeometry() const;
    Adesk::Boolean isDragging() const;

    double deviation(const AcGiDeviationType, const AcGePoint3d&) const;
	Adesk::UInt32 numberOfIsolines() const;
    
    AcGiContext * context();
	AcDbEntity * GetCurrentEntity() const;

private:
	AcGiViewport * mpVp;
	AcGiViewportGeometry * mpVpGeometry;
	AcDbObjectId mpObjId;

	AcGiSubEntityTraits * mpSubEntityTraits;
	AcGiContext * mpContext;
	AcGiRegenType mRegenType;

	AcDbEntity * m_pAcDbEntity;
};
