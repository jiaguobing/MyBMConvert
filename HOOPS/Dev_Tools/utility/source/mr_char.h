
#pragma once

#include "mr_utf_codec.h"

class MR_CHAR
{
public:
	~MR_CHAR();

	MR_CHAR(char const * utf8, char const * locale = 0);

	operator char const * () const;

private:
	char * m_str;
	char local_str[MR_UTF_LOCAL_SIZE];
};

