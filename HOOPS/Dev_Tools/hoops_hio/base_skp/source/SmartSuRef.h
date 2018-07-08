// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#pragma once

#include <slapi/model/model.h>
#include <slapi/unicodestring.h>
#include <slapi/model/texture_writer.h>
#include <slapi/model/mesh_helper.h>
#include <slapi/model/uv_helper.h>

template<typename Ref_t, SUResult (*Allocate)(Ref_t*), SUResult (*Free)(Ref_t*)>
class SmartSuRef
{
public:
	SmartSuRef() { SUSetInvalid(m_ref); }
	SmartSuRef(Ref_t& ref) : m_ref(ref) { }

	~SmartSuRef() { Release(); }

	bool Create()
	{
		Release();

		SUResult result = Allocate(&m_ref);

		return (result == SU_ERROR_NONE);
	}

	void Release()
	{
		if(!SUIsInvalid(m_ref)) {
			Free(&m_ref);
			SUSetInvalid(m_ref);
		}
	}

	void Swap(SmartSuRef& other)
	{
		Ref_t ref = other.m_ref;

		other.m_ref = m_ref;
		m_ref = ref;
	}

	bool IsInvalid() const { return SUIsInvalid(m_ref); }

	Ref_t& GetReference() { return m_ref; }
	Ref_t* GetAddress() { return &m_ref; }

private:
	SmartSuRef(const SmartSuRef&);
	SmartSuRef& operator=(const SmartSuRef&);

private:
	Ref_t m_ref;
};

typedef SmartSuRef<SUModelRef, SUModelCreate, SUModelRelease>		SmartModelRef;
typedef SmartSuRef<SUStringRef, SUStringCreate, SUStringRelease>	SmartStringRef;
typedef SmartSuRef<SUTextureWriterRef, SUTextureWriterCreate, SUTextureWriterRelease>	SmartTextureWriterRef;
typedef SmartSuRef<SUMeshHelperRef, nullptr, SUMeshHelperRelease>	SmartMeshHelperRef;
typedef SmartSuRef<SUUVHelperRef, nullptr, SUUVHelperRelease>		SmartUVHelperRef;