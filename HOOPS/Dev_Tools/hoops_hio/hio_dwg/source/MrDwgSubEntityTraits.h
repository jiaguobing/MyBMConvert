// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#pragma once

#include <acgi.h>
#include "MrDwgReader.h"

class MrDwgReader;

class MrDwgSubEntityTraits : public AcGiSubEntityTraits
{
protected:
	double m_dThickness;
	AcCmEntityColor m_entityColor;
	AcCmTransparency m_transparency;
	AcDbObjectId m_layerId;
	AcGiFillType m_fillType;
	AcDbObjectId m_linetypeId;
	AcDb::LineWeight m_lineWeight;
	bool m_colorSet;
	bool m_lineWeightSet;
	bool m_sorted;
	MrDwgReader * m_reader;

	void sort();
	void unsort();

public:
	MrDwgSubEntityTraits(MrDwgReader * reader);
	~MrDwgSubEntityTraits();

	void setColor(const Adesk::UInt16 color);
	void setTrueColor(const AcCmEntityColor & color);
	void setLayer(const AcDbObjectId layerId);
	void setLineType(const AcDbObjectId linetypeId);
	void setSelectionMarker(const Adesk::LongPtr markerId);
	void setFillType(const AcGiFillType);
	void setLineWeight(const AcDb::LineWeight lw);
	Adesk::UInt16 color() const;
	AcCmEntityColor trueColor() const;
	AcDbObjectId layerId() const;
	AcDbObjectId lineTypeId() const;
	AcGiFillType fillType() const;
	AcDb::LineWeight lineWeight() const;
	void setTransparency(const AcCmTransparency & transparency);

	void setLineTypeScale(double);
	double lineTypeScale() const;
	double m_dLTScale;

	void setThickness(double dThickness);
	double thickness() const;
};
