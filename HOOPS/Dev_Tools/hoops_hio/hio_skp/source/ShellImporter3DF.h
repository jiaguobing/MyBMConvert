// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#pragma once

#include "ShellImporter.h"

class ShellImporter3DF : public ShellImporter
{
public:
	ShellImporter3DF(SegmentOrganizer& SegmentOrganizer, TextureWriter& textureWriter, InheritanceManager& inheritanceManager);

private:
	ShellImporter3DF(const ShellImporter3DF&);
	ShellImporter3DF& operator=(const ShellImporter3DF&);

	virtual bool InsertShell(SegmentKey & segment, const std::vector<SUPoint3Df>& points,
		const std::vector<int>& faces, const std::vector<SUVector3Df>& normalVectors,
		const std::vector<float>& vertexParameters, bool isVisible = true);

private:
	static const std::size_t s_numParametersPerVertex = 3;
};
