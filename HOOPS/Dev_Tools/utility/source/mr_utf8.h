
#pragma once

#include "mr_utf_codec.h"

class MR_UTF8
{
public:
	~MR_UTF8();

	MR_UTF8(char const * text, char const * locale = 0);
	MR_UTF8(wchar_t const * wtext);

	operator char const * () const;

private:
	char * m_str;
	char local_str[MR_UTF_LOCAL_SIZE];
};

