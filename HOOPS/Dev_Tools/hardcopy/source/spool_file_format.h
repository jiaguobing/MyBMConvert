

#ifndef __spool_file_format_h
#define __spool_file_format_h

#ifdef _MSC_VER
#define int64_t __int64
#else
#include <inttypes.h>
#endif

/*! \private */
struct spool_file_header {
	unsigned short bom;
	char compression;
	char reserve1[5];
	int64_t version;
	int64_t width;
	int64_t height;
	int64_t bpp;
	int64_t one_x_step;
	int64_t offset_table;
	int64_t reserve2[9];
};

extern struct spool_file_header const init_header;

#endif

