// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#pragma once

#include "GeometryImporter.h"
#include "GroupRepository.h"
#include "InheritanceManager.h"

#include <slapi/model/component_definition.h>

#include <map>
#include <cstdint>

#include "SegmentKey.h"

class DefinitionRepository
{
public:
	virtual ~DefinitionRepository() {}

	bool   Import(SUComponentDefinitionRef& definition);

	SegmentKeyPtr GetDefinitionKey(SUComponentDefinitionRef& definition) const;
	InheritanceManager & GetInheritanceManager() { return m_inheritance_manager; }

protected:
	DefinitionRepository(SegmentKey const & includeSegmentKey, GeometryImporter& geometryImporter,
		GroupRepository& groupRepository, InheritanceManager& inheritanceManager);

	int32_t GetId(SUComponentDefinitionRef& definition) const;

	SegmentKeyPtr m_definitionsSegmentKeyPtr;
	GeometryImporter& m_geometryImporter;
	GroupRepository&  m_groupRepository;
	InheritanceManager& m_inheritance_manager;
	std::map<int32_t, SegmentKeyPtr> m_definitionsMap;

private:
	DefinitionRepository(const DefinitionRepository&);
	DefinitionRepository& operator=(const DefinitionRepository&);
};