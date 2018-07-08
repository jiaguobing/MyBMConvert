// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#pragma once

#include <dbcolor.h>
#include <dbents.h>
#include <gevec3d.h>
#include <hc.h>

#include <set>
#include <map>

HC_KEY My_Insert_Circle(double const * ip1, double const * ip2, double const * ip3);
void copy_dwg_matrix(float matrix[16], AcGeMatrix3d const & autocad_matrix);
void calculate_alignment(AcDbText * in_text, char * out_alignment);
void calculate_alignment(AcDbMText::AttachmentPoint attachmentPoint, char * out_alignment);
void insert_arrow(AcGePoint3d startPoint, AcGePoint3d endPoint);

#define DWG_HASH_BYTES 16
#define DWG_HASH_SIZE 2*DWG_HASH_BYTES+1

struct mTextData
{
public:
	std::map<AcDbHandle, HC_KEY> mtext_handles;
	std::set<HC_KEY> mtext_segments;
};

class DWG_HASH
{
private:
	char m_str[DWG_HASH_SIZE];
public:
	DWG_HASH(void const * input, size_t const len);
	~DWG_HASH();

	operator const char* () const;
};

bool applyColor(AcCmEntityColor const & color, bool open_segment = false);
