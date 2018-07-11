// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#pragma once

#include "SegmentKey.h"

#include <hc.h>


// Non-ref counted HC_KEY / HPS::SegmentKey abstraction
class SegmentKey3DF : public SegmentKey
{
public:
	SegmentKey3DF(HC_KEY k)	: _key(k) { }

	SegmentKey3DF(SegmentKey & seg)
		: _key(static_cast<SegmentKey3DF&>(seg)._key)
	{
	}

	virtual ~SegmentKey3DF() {
		_key = HC_ERROR_KEY;
	}

	virtual SegmentKeyPtr Clone();
	virtual void Open() const;
	virtual void Close() const;
	virtual SegmentKeyPtr Subsegment(const char *name) const;
	virtual intptr_t id() const { return (intptr_t)_key; }

	HC_KEY Get() const { return _key; }


private:
	HC_KEY		_key;
};

typedef std::shared_ptr<SegmentKey3DF>	SegmentKey3DFPtr;