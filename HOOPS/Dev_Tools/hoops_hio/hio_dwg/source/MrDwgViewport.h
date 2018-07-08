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

class MrDwgViewport : public AcGiViewport
{
public:
	ACRX_DECLARE_MEMBERS(AcGiViewport);
	MrDwgViewport();
	~MrDwgViewport();

	void getModelToEyeTransform(AcGeMatrix3d& mx) const;
	void getEyeToModelTransform(AcGeMatrix3d& mx) const;
	void getWorldToEyeTransform(AcGeMatrix3d& mx) const;
	void getEyeToWorldTransform(AcGeMatrix3d& mx) const;

	Adesk::Boolean isPerspective() const;
	Adesk::Boolean doPerspective(AcGePoint3d&) const;
	Adesk::Boolean doInversePerspective(AcGePoint3d&) const;

	void getNumPixelsInUnitSquare(const AcGePoint3d& givenWorldpt, AcGePoint2d& pixelArea, bool includePerspective = true) const;

	void getCameraLocation(AcGePoint3d& location) const;
	void getCameraTarget(AcGePoint3d& target) const;
	void getCameraUpVector(AcGeVector3d& upVector) const;

	Adesk::ULongPtr viewportId() const;
	Adesk::Int16  acadWindowId() const;
	void getViewportDcCorners(AcGePoint2d& lower_left,AcGePoint2d& upper_right) const;

	Adesk::Boolean getFrontAndBackClipValues(Adesk::Boolean& clip_front, Adesk::Boolean& clip_back, double& front, double& back) const;
	double linetypeScaleMultiplier() const;
	double linetypeGenerationCriteria() const;

	AcGeVector3d viewDir() const;

protected:
	AcGePoint3d cameraTarget;
	AcGePoint3d cameraLocation;
	AcGeVector3d cameraUpVector;
};
