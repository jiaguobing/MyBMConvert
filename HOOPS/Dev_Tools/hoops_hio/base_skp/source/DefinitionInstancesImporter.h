// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#pragma once

#include "DefinitionRepository.h"
#include "VisibilityChecker.h"

#include <slapi/model/component_instance.h>
#include <slapi/model/entities.h>

#include <vector>

class DefinitionInstancesImporter
{
public:
	virtual ~DefinitionInstancesImporter() {}

	bool ImportCollectionOfInstances(SegmentKey & segment, SUEntitiesRef& entities);

protected:
	DefinitionInstancesImporter(DefinitionRepository& definitionRepository, VisibilityChecker& visibilityChecker);
	bool ImportTransform(SUComponentInstanceRef& instance, std::vector<float>& floatTransform);

	virtual bool InsertInstance(SegmentKey & segment, SegmentKey & includeSegment, std::vector<float> const & floatTransform, bool const isVisible) = 0;

private:
	DefinitionInstancesImporter(const DefinitionInstancesImporter&);
	DefinitionInstancesImporter& operator=(const DefinitionInstancesImporter&);
	bool ImportInstance(SegmentKey & segment, SUComponentInstanceRef& instance);

	DefinitionRepository& m_definitionRepository;
	VisibilityChecker&    m_visibilityChecker;
};