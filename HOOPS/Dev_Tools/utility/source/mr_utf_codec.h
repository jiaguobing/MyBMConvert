
#pragma once

#include <wchar.h>

#define MR_UTF_LOCAL_SIZE 100

#ifdef _MSC_VER
# define UTF16_WCHAR_T 1
#endif

#ifdef __cplusplus
extern "C" {
#endif

int mr_utf8_encode (char * utf8, int const size, int const * iter);
int mr_utf8_decode (int * dest, int const size, char const * text);

#ifdef UTF16_WCHAR_T
int mr_utf16_encode (wchar_t * dest, int const size, int const * iter);
int mr_utf16_decode (int * dest, int size, wchar_t const * text);
#endif

#ifdef __cplusplus
};
#endif


