// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#pragma once

#include "MrDwgGeometry.h"
#include "MrImportDirector.h"
#include <acgi.h>

struct dwg_matrix
{
	dwg_matrix(float in_elements[16])
	{
		for (size_t i = 0; i < 16; ++i)
			elements[i] = in_elements[i];
	}

	float elements[16];
};

class MrDwgWorldGeometry : public AcGiWorldGeometry, public MrDwgGeometry
{
public:
	MrDwgWorldGeometry(AcDbEntity * pEnt, std::map<std::wstring, std::string> * fontmap, mTextData * mtext, HC_KEY entitySegment);

	Adesk::Boolean pushClipBoundary(AcGiClipBoundary * pBoundary);
	void popClipBoundary();
	void setExtents(AcGePoint3d * pNewExtents) const;

	Adesk::Boolean circle(const AcGePoint3d & center, const double radius, const AcGeVector3d & normal) const;
	Adesk::Boolean circle(const AcGePoint3d & pt1, const AcGePoint3d & pt2, const AcGePoint3d & pt3) const;
	Adesk::Boolean circularArc(const AcGePoint3d & center, const double radius, const AcGeVector3d & normal, const AcGeVector3d & startVector, const double sweepAngle, const AcGiArcType arcType = kAcGiArcSimple) const;
	Adesk::Boolean circularArc(const AcGePoint3d & start, const AcGePoint3d & point, const AcGePoint3d & end, const AcGiArcType arcType = kAcGiArcSimple) const;
	Adesk::Boolean polyline(const Adesk::UInt32 nbPoints, const AcGePoint3d * pVertexList, const AcGeVector3d * pNormal = NULL, Adesk::LongPtr lBaseSubEntMarker = -1) const;
	Adesk::Boolean polyline(const AcGiPolyline & polyline) const;
	Adesk::Boolean polyPolyline(Adesk::UInt32 nbPolylines, const AcGiPolyline * pPolylines) const;
	Adesk::Boolean polygon(const Adesk::UInt32 nbPoints, const AcGePoint3d * pVertexList) const;
	Adesk::Boolean polyPolygon(const Adesk::UInt32 numPolygonIndices, const Adesk::UInt32 * numPolygonPositions, const AcGePoint3d * polygonPositions, const Adesk::UInt32 * numPolygonPoints, const AcGePoint3d * polygonPoints, const AcCmEntityColor * outlineColors = NULL, const AcGiLineType * outlineTypes = NULL, const AcCmEntityColor * fillColors = NULL, const AcCmTransparency * fillOpacities = NULL) const;
	Adesk::Boolean mesh(const Adesk::UInt32 rows, const Adesk::UInt32 columns, const AcGePoint3d * pVertexList, const AcGiEdgeData * pEdgeData, const AcGiFaceData * pFaceData, const AcGiVertexData * pVertexData = NULL, const bool bAutoGenerateNormals = true) const;
	Adesk::Boolean shell(const Adesk::UInt32 nbVertex, const AcGePoint3d * pVertexList, const Adesk::UInt32 faceListSize, const Adesk::Int32 * pFaceList, const AcGiEdgeData * pEdgeData, const AcGiFaceData * pFaceData, const AcGiVertexData * pVertexData = NULL, const struct resbuf *pResBuf = NULL, const bool bAutoGenerateNormals = true) const;
	Adesk::Boolean text(const AcGePoint3d & position, const AcGeVector3d & normal, const AcGeVector3d & direction, const double height, const double width, const double oblique, const ACHAR * pMsg) const;
	Adesk::Boolean text(const AcGePoint3d & position, const AcGeVector3d & normal, const AcGeVector3d & direction, const ACHAR * pMsg, const Adesk::Int32 length, const Adesk::Boolean raw, const AcGiTextStyle & pTextStyle) const;
	Adesk::Boolean xline(const AcGePoint3d & one_xline_point, const AcGePoint3d & a_different_xline_point) const;
	Adesk::Boolean ray(const AcGePoint3d & rays_starting_point, const AcGePoint3d & a_different_ray_point) const;
	Adesk::Boolean image(const AcGiImageBGRA32 & imageSource, const AcGePoint3d & position, const AcGeVector3d & u, const AcGeVector3d & v, TransparencyMode transparencyMode = kTransparency8Bit) const;
	Adesk::Boolean rowOfDots(int count, const AcGePoint3d & start, const AcGeVector3d & step) const;
	Adesk::Boolean ellipticalArc(const AcGePoint3d & center, const AcGeVector3d & normal, double majorAxisLength, double minorAxisLength, double startDegreeInRads, double endDegreeInRads, double tiltDegreeInRads, AcGiArcType arcType) const;
	Adesk::Boolean pline(const AcDbPolyline & lwBuf, Adesk::UInt32 fromIndex, Adesk::UInt32 numSegs) const;
	Adesk::Boolean edge(const AcArray<AcGeCurve2d *> &) const;
	Adesk::Boolean draw(AcGiDrawable *) const;

	AcGeMatrix3d pushPositionTransform(AcGiPositionTransformBehavior behavior, const AcGePoint3d & offset);
	AcGeMatrix3d pushPositionTransform(AcGiPositionTransformBehavior behavior, const AcGePoint2d & offset);
	AcGeMatrix3d pushScaleTransform(AcGiScaleTransformBehavior behavior, const AcGePoint3d & extents);
	AcGeMatrix3d pushScaleTransform(AcGiScaleTransformBehavior behavior, const AcGePoint2d & extents);
	AcGeMatrix3d pushOrientationTransform(AcGiOrientationTransformBehavior behavior);

	void getModelToWorldTransform(AcGeMatrix3d & pMatrix) const;
	void getWorldToModelTransform(AcGeMatrix3d & pMatrix) const;

	Adesk::Boolean pushModelTransform(const AcGeVector3d & vNormal);
	Adesk::Boolean pushModelTransform(const AcGeMatrix3d & xMat);
	Adesk::Boolean popModelTransform();

	bool	isCurrentEntityUnknown();
	void	setupMatrixSegment() const;
	void	cleanupMatrixSegment() const;

	std::map<std::wstring, std::string> *	m_pFontmap;
	AcDbEntity *							m_pEnt;
	mTextData *								m_pMText;
	HC_KEY									m_entitySegment;
	mutable bool							apply_matrix;
	std::vector<dwg_matrix>					matrices;
};
