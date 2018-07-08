// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#pragma once

#include "MrDwgContext.h"
#include "MrDwgSubEntityTraits.h"
#include "MrDwgWorldGeometry.h"
#include "MrDwgReader.h"
#include "hc.h"

#include <acgi.h>

class MrDwgWorldDraw : public AcGiWorldDraw
{
public:
	MrDwgWorldDraw(std::vector<HC_KEY> * keys, AcDbEntity * pEnt, MrDwgReader * reader);
	~MrDwgWorldDraw();

	void setRegenType(AcGiRegenType r);
	AcGiRegenType regenType() const;
	Adesk::Boolean regenAbort() const;
	AcGiSubEntityTraits & subEntityTraits() const;
	AcGiWorldGeometry & geometry() const;
	AcGiGeometry *rawGeometry() const;
	Adesk::Boolean isDragging() const;
	double deviation(const AcGiDeviationType, const AcGePoint3d &) const;
	Adesk::UInt32 numberOfIsolines() const;

	AcGiContext *context();
	void regenerate(AcDbEntity * pEntity);
	void regenerate(AcDbBlockTableRecord * pBTR);

	MrDwgContext * mpContext;
	MrDwgSubEntityTraits * mpTraits;
	MrDwgWorldGeometry * mpGeom;
	AcGiRegenType m_regenType;
};
