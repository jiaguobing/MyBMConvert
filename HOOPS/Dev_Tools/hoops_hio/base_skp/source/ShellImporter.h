// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#pragma once

#include "SegmentOrganizer.h"
#include "SmartSuRef.h"

#include <slapi/model/entities.h>
#include <slapi/model/face.h>
#include <slapi/model/mesh_helper.h>

#include <vector>

#include "InheritanceManager.h"
#include "data_types.h"

#include "SegmentKey.h"


class ShellImporter
{
public:
	virtual ~ShellImporter() {}

	bool Import(SegmentKey & segment, SUEntitiesRef& entities);

protected:
	struct ShellInfo {
		std::vector<SUPoint3Df>		points;
		std::vector<int>			faces;
		std::vector<SUVector3Df>	normals;
		std::vector<float>			params;

		SegmentKeyPtr				segKeyPtr;
		bool						isVisible;
	};

	typedef std::map<intptr_t, ShellInfo>	SegmentShellMap;

protected:
	ShellImporter(SegmentOrganizer& segmentOrganizer, TextureWriter& textureWriter, InheritanceManager& inheritanceManager);

	virtual bool InsertShell(SegmentKey & segment, const std::vector<SUPoint3Df>& points,
		const std::vector<int>& faces, const std::vector<SUVector3Df>& normalVectors,
		const std::vector<float>& vertexParameters, bool isVisible = true) = 0;

private:
	ShellImporter(const ShellImporter&);
	ShellImporter& operator=(const ShellImporter&);

	bool ImportShell(SegmentKey & segment, SUFaceRef& suFace, SegmentShellMap & segmentShellMap);
	bool ImportSingleFaceShell(SegmentKey & segment, SUFaceRef& suFace, ShellInfo & shellInfo);
	bool ImportTesselatedShell(SegmentKey & segment, SUFaceRef& suFace, ShellInfo & shellInfo);

	bool GetFacesListAndPoints(SUMeshHelperRef& meshHelper, std::vector<int>& faces, std::vector<SUPoint3Df>& points, size_t baseIndex);
	bool GetNormalVectors(SUMeshHelperRef& meshHelper, std::size_t numVertices, std::vector<SUVector3Df>& normalVectors);
	bool GetVertexParameters(SUFaceRef& suFace, SUMeshHelperRef& meshHelper, std::size_t numVertices, std::vector<float>& vertexParameters);

protected:
	static const std::size_t s_numParametersPerVertex = 3;

	TextureWriter&              m_textureWriter;
	InheritanceManager&			m_inheritanceManager;
	SegmentOrganizer&			m_segmentOrganizer;
};
