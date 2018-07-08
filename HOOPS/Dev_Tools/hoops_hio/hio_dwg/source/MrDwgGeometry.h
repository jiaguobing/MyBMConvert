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
#include <map>

#include <adesk.h>
#include <acgi.h>

#include "hc.h"
#include "misc.h"

class MrDwgGeometry
{
protected:
	void open_direction_segment(const AcGeVector3d & direction) const;
	void open_height_segment(double height) const;

public:
	Adesk::Boolean circle(const AcGePoint3d & center, const double radius, const AcGeVector3d & normal) const;
	Adesk::Boolean circle(const AcGePoint3d & pt1, const AcGePoint3d & pt2, const AcGePoint3d & pt3) const;
	Adesk::Boolean circularArc(const AcGePoint3d & center, const double radius, const AcGeVector3d & normal, const AcGeVector3d & startVector, const double sweepAngle, const AcGiArcType arcType) const;
	Adesk::Boolean circularArc(const AcGePoint3d & start, const AcGePoint3d & point, const AcGePoint3d & end, const AcGiArcType arcType) const;
	Adesk::Boolean ellipticalArc(const AcGePoint3d& center, const AcGeVector3d& normal, double majorAxisLength, double minorAxisLength, double startDegreeInRads, double endDegreeInRads, double tiltDegreeInRads, AcGiArcType arcType) const;
	Adesk::Boolean polyline(const Adesk::UInt32 nbPoints, const AcGePoint3d * pVertexList, const AcGeVector3d * pNormal, Adesk::LongPtr lBaseSubEntMarker) const;
	Adesk::Boolean polyline(const AcGiPolyline & polylineObj) const;
	Adesk::Boolean polyPolyline(Adesk::UInt32 nbPolylines, const AcGiPolyline * pPolylines) const;
	Adesk::Boolean polygon(const Adesk::UInt32 nbPoints, const AcGePoint3d * pVertexList) const;
	Adesk::Boolean pline(const AcDbPolyline & lwBuf, Adesk::UInt32 fromIndex, Adesk::UInt32 numSegs) const;
	Adesk::Boolean mesh(const Adesk::UInt32 rows, const Adesk::UInt32 columns, const AcGePoint3d * pVertexList, const AcGiEdgeData * pEdgeData, const AcGiFaceData * pFaceData, const AcGiVertexData * pVertexData, const bool bAutoGenerateNormals) const;
	Adesk::Boolean ray(const AcGePoint3d & origin, const AcGePoint3d & other_point) const;
	Adesk::Boolean shell(const Adesk::UInt32 nbVertex, const AcGePoint3d * pVertexList, const Adesk::UInt32 faceListSize, const Adesk::Int32 * pFaceList, const AcGiEdgeData * pEdgeData, const AcGiFaceData * pFaceData, const AcGiVertexData * pVertexData, const struct resbuf * pResBuf, const bool bAutoGenerateNormals) const;
	Adesk::Boolean text(const AcGePoint3d & position, const AcGeVector3d & normal, const AcGeVector3d & direction, const double height, const double width, const double oblique, const ACHAR * pMsg, AcDbEntity * pEnt = 0) const;
	Adesk::Boolean text(const AcGePoint3d & position, const AcGeVector3d & normal, const AcGeVector3d & direction, const ACHAR * pMsg, const Adesk::Int32 length, const Adesk::Boolean raw, const AcGiTextStyle & pTextStyle, std::map<std::wstring, std::string> * fontmap, mTextData * mtext, AcDbEntity * pEnt = 0, HC_KEY entityInsertionSegment = -1) const;
	Adesk::Boolean xline(const AcGePoint3d & one_xline_point, const AcGePoint3d & a_different_xline_point) const;

	std::vector<HC_KEY> * m_keys;
	void addKeyToVector(HC_KEY keyToAdd) const;
	void addKeysToVector(HC_KEY * keysToAdd, int numberOfKeys) const;
};
