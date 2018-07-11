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
#include "MrDwgViewportDraw.h"

#include <acgi.h>

class MrDwgViewportGeometry : public AcGiViewportGeometry, public MrDwgGeometry
{
public:
	ACRX_DECLARE_MEMBERS(MrDwgViewportGeometry);
	MrDwgViewportGeometry();
	~MrDwgViewportGeometry();

	Adesk::Boolean polylineEye(const Adesk::UInt32 nbPoints, const AcGePoint3d * pPoints) const;
	Adesk::Boolean polygonEye(const Adesk::UInt32 nbPoints, const AcGePoint3d * pPoints) const;
	Adesk::Boolean polylineDc(const Adesk::UInt32 nbPoints, const AcGePoint3d * pPoints) const;
	Adesk::Boolean polygonDc(const Adesk::UInt32 nbPoints, const AcGePoint3d * pPoints) const;
	Adesk::Boolean rasterImageDc(const AcGePoint3d & origin, const AcGeVector3d & u, const AcGeVector3d & v, const AcGeMatrix2d & pixelToDc, AcDbObjectId entityId, AcGiImageOrg imageOrg, Adesk::UInt32 imageWidth, Adesk::UInt32 imageHeight, Adesk::Int16 imageColorDepth, Adesk::Boolean transparency, ImageSource source, const AcGeVector3d & unrotatedU, const AcGiImageOrg origionalImageOrg, const AcGeMatrix2d & unrotatedPixelToDc, const Adesk::UInt32 unrotatedImageWidth, const Adesk::UInt32 unrotatedImageHeight) const;
	Adesk::Boolean ownerDrawDc(long vpnumber, long left, long top, long right, long bottom, const OwnerDraw * pOwnerDraw) const;
	Adesk::Boolean ownerDraw3d(AcGePoint3d & minBounds, AcGePoint3d & maxBounds, OwnerDraw3d * pOwnerDraw) const;
	Adesk::Boolean edge(const AcArray<AcGeCurve2d *> &) const;

	void getModelToWorldTransform(AcGeMatrix3d &) const;
	void getWorldToModelTransform(AcGeMatrix3d &) const;

	Adesk::Boolean pushModelTransform(const AcGeVector3d & vNormal);
	Adesk::Boolean pushModelTransform(const AcGeMatrix3d & xMat);
	Adesk::Boolean popModelTransform();

	AcGeMatrix3d pushPositionTransform(AcGiPositionTransformBehavior behavior, const AcGePoint3d & offset);
	AcGeMatrix3d pushPositionTransform(AcGiPositionTransformBehavior behavior, const AcGePoint2d & offset);
	AcGeMatrix3d pushScaleTransform(AcGiScaleTransformBehavior behavior, const AcGePoint3d & extents);
	AcGeMatrix3d pushScaleTransform(AcGiScaleTransformBehavior behavior, const AcGePoint2d & extents);
	AcGeMatrix3d pushOrientationTransform(AcGiOrientationTransformBehavior behavior);

	Adesk::Boolean circle(const AcGePoint3d & center, const double radius, const AcGeVector3d & normal) const;
	Adesk::Boolean circle(const AcGePoint3d &, const AcGePoint3d &, const AcGePoint3d &) const;
	Adesk::Boolean circularArc(const AcGePoint3d & center, const double radius, const AcGeVector3d & normal, const AcGeVector3d & startVector, const double sweepAngle, const AcGiArcType arcType = kAcGiArcSimple) const;
	Adesk::Boolean circularArc(const AcGePoint3d & start, const AcGePoint3d & point, const AcGePoint3d & end, const AcGiArcType arcType = kAcGiArcSimple) const;
	Adesk::Boolean polyline(const Adesk::UInt32 nbPoints, const AcGePoint3d * pVertexList, const AcGeVector3d * pNormal = NULL, Adesk::LongPtr lBaseSubEntMarker = -1) const;
	Adesk::Boolean polyline(const AcGiPolyline & polylineObj) const;
	Adesk::Boolean polyPolyline(Adesk::UInt32 nbPolylines, const AcGiPolyline * pPolylines) const;
	Adesk::Boolean polyPolygon(const Adesk::UInt32 numPolygonIndices, const Adesk::UInt32 * numPolygonPositions, const AcGePoint3d * polygonPositions, const Adesk::UInt32 * numPolygonPoints, const AcGePoint3d * polygonPoints, const AcCmEntityColor * outlineColors = NULL, const AcGiLineType * outlineTypes = NULL, const AcCmEntityColor * fillColors = NULL, const AcCmTransparency * fillOpacities = NULL) const;
	Adesk::Boolean image(const AcGiImageBGRA32 & imageSource, const AcGePoint3d & position, const AcGeVector3d & u, const AcGeVector3d & v, TransparencyMode transparencyMode = kTransparency8Bit) const;
	Adesk::Boolean rowOfDots(int count, const AcGePoint3d & start, const AcGeVector3d & step) const;
	Adesk::Boolean ellipticalArc(const AcGePoint3d & center, const AcGeVector3d & normal, double majorAxisLength, double minorAxisLength, double startDegreeInRads, double endDegreeInRads, double tiltDegreeInRads, AcGiArcType arcType = kAcGiArcSimple) const;

	Adesk::Boolean polygon(const Adesk::UInt32 nbPoints, const AcGePoint3d * pVertexList) const;
	Adesk::Boolean mesh(const Adesk::UInt32 rows, const Adesk::UInt32 columns, const AcGePoint3d * pVertexList, const AcGiEdgeData * pEdgeData = NULL, const AcGiFaceData * pFaceData = NULL, const AcGiVertexData * pVertexData = NULL, const bool bAutoGenerateNormals = true) const;
	Adesk::Boolean shell(const Adesk::UInt32 nbVertex, const AcGePoint3d * pVertexList, const Adesk::UInt32 faceListSize, const Adesk::Int32 * pFaceList, const AcGiEdgeData * pEdgeData = NULL, const AcGiFaceData * pFaceData = NULL, const AcGiVertexData * pVertexData = NULL, const struct resbuf *pResBuf = NULL, const bool bAutoGenerateNormals = true) const;
	Adesk::Boolean text(const AcGePoint3d & position, const AcGeVector3d & normal, const AcGeVector3d & direction, const double height, const double width, const double oblique, const ACHAR * pMsg) const;
	Adesk::Boolean text(const AcGePoint3d & position, const AcGeVector3d & normal, const AcGeVector3d & direction, const ACHAR * pMsg, const Adesk::Int32 length, const Adesk::Boolean raw, const AcGiTextStyle & pTextStyle) const;
	Adesk::Boolean xline(const AcGePoint3d &, const AcGePoint3d &) const;
	Adesk::Boolean ray(const AcGePoint3d &, const AcGePoint3d &) const;
	Adesk::Boolean pline(const AcDbPolyline & lwBuf, Adesk::UInt32 fromIndex = 0, Adesk::UInt32 numSegs = 0) const;

	Adesk::Boolean draw(AcGiDrawable *) const;

	Adesk::Boolean pushClipBoundary(AcGiClipBoundary * pBoundary);
	void popClipBoundary();

	std::map<std::wstring, std::string> * m_pFontmap;
	mTextData * m_pMText;
	HC_KEY m_entitySegment;
};
