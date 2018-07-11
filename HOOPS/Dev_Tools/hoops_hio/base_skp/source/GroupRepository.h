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
#include "VisibilityChecker.h"

#include <slapi/model/group.h>
#include <slapi/model/entities.h>

#include <map>
#include <vector>
#include <cstdint>

#include "SegmentKey.h"

class GroupRepository
{
protected:
	struct GroupEntry
	{
		GroupEntry() { }

		GroupEntry(SegmentKeyPtr in_seg, const SUEntitiesRef& ent)
			: segmentKeyPtr(in_seg), entities(ent) { }

		SegmentKeyPtr	segmentKeyPtr;
		SUEntitiesRef	entities;
	};

	typedef std::map<int64_t, GroupEntry> GroupsMap_t;

public:
	class Iterator
	{
	public:
		Iterator();
		Iterator(GroupsMap_t::iterator& begin, GroupsMap_t::iterator& end);
		Iterator(const Iterator&);
		Iterator& operator=(const Iterator&);

		SegmentKeyPtr	GetSegmentKey() const;
		SUEntitiesRef&	GetEntities() const;
		void Next();
		bool Available() const;

	private:
		GroupsMap_t::iterator m_iter;
		GroupsMap_t::iterator m_end;
	};

public:
	virtual ~GroupRepository() {}

	bool     Import(SegmentKey & segment, SUEntitiesRef& entities);

	Iterator GetIterator();

protected:
	GroupRepository(SegmentKey const & includeSegmentKey, GeometryImporter& geometryImporter,
		VisibilityChecker& visibilityChecker, ImageRepository& imageRepository, StyleRepository& textureRepository);

	int64_t			GetId(SUGroupRef& group) const;
	SegmentKeyPtr	GetGroupKey(SUGroupRef& group) const;

	virtual void	ApplyGroupColors(SegmentKey & segment, SUColor const & color) = 0;
	virtual void	ApplyGroupTextures(SegmentKey & segment, int32_t const groupTextureId, std::string const & imageName) = 0;
	virtual bool	InsertInstance(SegmentKey & segment, SegmentKey & includeSegment, std::vector<float> const & floatTransform, bool const isVisible) = 0;

	SegmentKeyPtr		m_groupsSegmentKeyPtr;
	GeometryImporter&	m_geometryImporter;
	VisibilityChecker&	m_visibilityChecker;
	ImageRepository&	m_imageRepository;
	StyleRepository&	m_styleRepository;

private:
	GroupRepository(const GroupRepository&);
	GroupRepository& operator=(const GroupRepository&);
	SegmentKeyPtr	ImportGroup(SUGroupRef& group);
	bool			ImportGroupColors(SegmentKey & segment, SUGroupRef& group);
	bool			ImportTransform(SUGroupRef& group, std::vector<float>& floatTransform);
	virtual bool	ImportInstance(SegmentKey & segment, SUGroupRef& group, SegmentKey & groupKey);

	GroupsMap_t			m_groupsMap;
};