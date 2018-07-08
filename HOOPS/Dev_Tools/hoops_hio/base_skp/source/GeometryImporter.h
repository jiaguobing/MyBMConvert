// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#pragma once

#include "TextureWriter.h"
#include "ImageRepository.h"
#include "StyleRepository.h"
#include "VisibilityChecker.h"
#include "InheritanceManager.h"

#include "CurvesImporter.h"
#include "PolylinesImporter.h"
#include "LinesImporter.h"
#include "SegmentOrganizer.h"
#include "ShellImporter.h"
#include "ImageTexturedShellImporter.h"

#include <slapi/model/entities.h>

class GeometryImporter
{
public:
	virtual ~GeometryImporter() {}

	bool Import(SegmentKey & segment, SUEntitiesRef& entities);

	InheritanceManager& GetInheritanceManager() { return m_inheritanceManager; }
	TextureWriter& GetTextureWriter() { return m_textureWriter; }

protected:
	GeometryImporter(StyleRepository& styleRepository, VisibilityChecker& visibilityChecker, InheritanceManager& inheritanceManager);

	virtual CurvesImporter *				CreateCurvesImporter(SegmentOrganizer & segmentOrganizer) = 0;
	virtual PolylinesImporter *				CreatePolylinesImporter(SegmentOrganizer & segmentOrganizer) = 0;
	virtual LinesImporter *					CreateLinesImporter(SegmentOrganizer & segmentOrganizer) = 0;
	virtual SegmentOrganizer *				CreateSegmentOrganizer(StyleRepository& styleRepository, VisibilityChecker& visibilityChecker) = 0;
	virtual ShellImporter *					CreateShellImporter(SegmentOrganizer& segmentOrganizer, TextureWriter& textureWriter, InheritanceManager& inheritanceManager) = 0;
	virtual ImageTexturedShellImporter *	CreateImageTexturedShellImporter(SegmentOrganizer& segmentOrganizer, ImageRepository& imageRepository, TextureRepository& textureRepository) = 0;

private:
	GeometryImporter(const GeometryImporter&);
	GeometryImporter& operator=(const GeometryImporter&);

	bool ImportGeometry(SegmentOrganizer& segmentOrganizer, SegmentKey & segment, SUEntitiesRef& entities);
	bool ImportCollectionOfImageTexturedShells(SegmentOrganizer& segmentOrganizer, SegmentKey & segment, SUEntitiesRef& entities);

protected:
	TextureWriter&     m_textureWriter;
	ImageRepository&   m_imageRepository;
	StyleRepository&   m_styleRepository;
	VisibilityChecker& m_visibilityChecker;
	InheritanceManager& m_inheritanceManager;
};
