
/*
*/

#ifndef _TEMPFILE_UTILS
#define _TEMPFILE_UTILS

#include <wchar.h>
#include <stdio.h>

#define alter
#define TEMPFILE_UTILS_BUFFER_SIZE 4096

// Populates the passed in buffer which will be populated with a path to a temporary file (with optional extension)
extern void GenerateTempFileName ( char alter * tempFileNameOut, char const * extension = 0 );

// Populates the passed in buffer which will be populated with a path to a temporary file (with optional extension)
extern void GenerateTempFileName ( wchar_t alter * tempFileNameOut, wchar_t const * extension = 0 );

// portable version of tmp_file
extern FILE * GenerateTmpFilePtr ();

#endif // _TEMPFILE_UTILS
