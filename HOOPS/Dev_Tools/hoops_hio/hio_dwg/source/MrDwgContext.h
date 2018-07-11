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

class MrDwgContext : public AcGiContext
{
public:
	MrDwgContext();

	// If this returns TRUE then we are regening for extents information.
	// Either draw yourself normally or pass one or more WCS points to the
	// candidate point function, whichever is quicker for you
	//
	Adesk::Boolean isRegenForExtents() const;
	void candidateWCSPoint(const AcGePoint3d & ptWCSCand);
	Adesk::Boolean isPsOut() const;
	Adesk::Boolean isPlotGeneration() const;
	bool isBoundaryClipping() const;
	
	void pushModelTransform(const AcGeVector3d & vNormal);
	void pushModelTransform(const AcGeMatrix3d & xMat);
	void popModelTransform();
	const AcGeMatrix3d & modelTransform() const;
	const AcGeMatrix3d & inverseModelTransform() const;

	void pushBlockReference(AcDbBlockReference * pBlockReference);
	void popBlockReference();
	AcDbBlockReference *blockReference() const;

	AcDbDatabase *database() const;
	void setDatabase(AcDbDatabase * pDb);

protected:
	AcDbDatabase *m_pDb;
};
