
#pragma once

#include "mr_utf_codec.h"

class MR_WCHAR_T
{
public:
	~MR_WCHAR_T();

	MR_WCHAR_T(char const * utf8);

	operator wchar_t const * () const;

private:
	wchar_t * m_str;
	wchar_t local_str[MR_UTF_LOCAL_SIZE];
};

