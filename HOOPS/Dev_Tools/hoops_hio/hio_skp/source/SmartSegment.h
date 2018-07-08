// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#pragma once

#include <hc.h>

#include "SegmentKey3DF.h"

class SmartSegment
{
public:
	SmartSegment()
		: m_key(HC_ERROR_KEY)
	{
	}

	explicit SmartSegment(HC_KEY key)
		: m_key(key)
	{
		HC_Open_Segment_By_Key(m_key);
	}

	explicit SmartSegment(SegmentKey & segment)
		: m_key(static_cast<SegmentKey3DF &>(segment).Get())
	{
		HC_Open_Segment_By_Key(m_key);
	}

	explicit SmartSegment(const char *segmentName)
		: m_key(HC_Open_Segment(segmentName))
	{
	}

	void Open(HC_KEY key)
	{
		if(key != HC_ERROR_KEY) {
			HC_Open_Segment_By_Key(key);
		}

		m_key = key;
	}

	void Open(const char *segmentName)
	{
		m_key = HC_Open_Segment(segmentName);
	}

	~SmartSegment() { Release(); }

	HC_KEY Get() { return m_key; }

	void Swap(SmartSegment& other) { std::swap(m_key, other.m_key); }

	void Release()
	{
		if(m_key != HC_ERROR_KEY) {
			HC_Close_Segment();
			m_key = HC_ERROR_KEY;
		}
	}

private:
	SmartSegment(const SmartSegment&);
	SmartSegment& operator=(const SmartSegment&);
	SmartSegment(SegmentKeyPtr &);

private:
	HC_KEY m_key;
};
