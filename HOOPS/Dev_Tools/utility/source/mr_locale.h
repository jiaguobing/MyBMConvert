
#pragma once

class MR_LOCALE
{
public:
	MR_LOCALE(char const * locale);
	~MR_LOCALE();

private:
	char local_buffer[40];
	bool m_saved_locale;
};

