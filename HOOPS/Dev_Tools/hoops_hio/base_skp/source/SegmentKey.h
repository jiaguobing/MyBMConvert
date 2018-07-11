// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#pragma once

#include <memory>

class SegmentKey;
typedef std::shared_ptr<SegmentKey>		SegmentKeyPtr;

// Non-ref counted HC_KEY / HPS::SegmentKey abstraction
class SegmentKey {
public:
	virtual ~SegmentKey() {}

	/// Create new copy of this instance
	virtual SegmentKeyPtr Clone() = 0;

	/// Open segment.  Close must be called after.
	virtual void Open() const = 0;
	
	/// Closes segment.  Open must have been called.
	virtual void Close() const = 0;

	/// Returns a subsegment 
	virtual SegmentKeyPtr Subsegment(const char *name) const = 0;

	virtual intptr_t id() const = 0;

protected:
	SegmentKey(){}
	SegmentKey(SegmentKey const &){}
	SegmentKey & operator=(SegmentKey const &){}
};

