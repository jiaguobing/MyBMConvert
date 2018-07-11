

#ifndef VCONF_H
#define VCONF_H

#include <stdlib.h>

#include "vlist.h"
#include "vhash.h"

#ifndef HOOPS_DEFINED
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif 



#ifdef HOOPS_DEFINED
#define VCONF_API HOOPS_API
#define VCONF_CDECL HC_CDECL
#else

#define VCONF_API
#define VCONF_CDECL

#ifdef _WIN32_WCE
#define VCONF_MEM_CDECL __cdecl
#else
#define VCONF_MEM_CDECL 
#endif

#if 0
/* use the one from vlist.h */
typedef void *(VCONF_MEM_CDECL *vmalloc_t) (size_t);
typedeef void (VCONF_MEM_CDECL *vfree_t) (void*);
#endif

#endif

#ifdef V_PREFIX

#undef VNAME_TMP_1
#undef VNAME
#define VNAME_TMP_2(_x2_,_y2_) _x2_##_y2_
#define VNAME_TMP_1(_x1_,_y1_) VNAME_TMP_2(_x1_,_y1_)
#define VNAME(_x_) VNAME_TMP_1(V_PREFIX,_x_)

/* remap the names for special prefix */
#define new_vconf VNAME(new_vconf)
#define delete_vconf VNAME(delete_vconf)
#define vconf_clear_options VNAME(vconf_clear_options)
#define vconf_set_delimiter VNAME(vconf_set_delimiter)
#define vconf_set_file_style VNAME(vconf_set_file_style)
#define vconf_option_count VNAME(vconf_option_count)
#define vconf_chunk_count VNAME(vconf_chunk_count)
#define vconf_set_chunk VNAME(vconf_set_chunk)
#define vconf_read_file VNAME(vconf_read_file)
#define vconf_write_file VNAME(vconf_write_file)
#define vconf_get_filename VNAME(vconf_get_filename)
#define vconf_get_option_hash VNAME(vconf_get_option_hash)
#define vconf_get_option VNAME(vconf_get_option)
#define vconf_set_option VNAME(vconf_set_option)
#endif

#define VCONF_FILE_STYLE_FLAT		0
#define VCONF_FILE_STYLE_CHUNKED 	1

typedef struct vconf_s {
	const char * filename;
	vhash_t * options_hash;
	vlist_t * options_list;
	char delimiter;
	int style;
	int chunk;
#ifdef HOOPS_DEFINED
	Memory_Pool * memory_pool;
#else
	vmalloc_t vmalloc;
	vfree_t vfree;
#endif
} vconf_t;

#ifdef HOOPS_DEFINED
VCONF_API vconf_t* VCONF_CDECL new_vconf(
	Memory_Pool * memory_pool);
#else
VCONF_API vconf_t* VCONF_CDECL new_vconf(
	vmalloc_t vconf_malloc,
	vfree_t vconf_free);
#endif

VCONF_API void VCONF_CDECL delete_vconf(
	vconf_t* vconf); 

VCONF_API void VCONF_CDECL vconf_clear_options(
	vconf_t* vconf);

VCONF_API void VCONF_CDECL vconf_set_delimiter(
	vconf_t* vconf, 
	char delimiter);

VCONF_API int VCONF_CDECL vconf_set_file_style(
	vconf_t* vconf, 
	int style);

VCONF_API unsigned int VCONF_CDECL vconf_option_count(
	vconf_t* vconf);

VCONF_API unsigned int VCONF_CDECL vconf_chunk_count(
	vconf_t* vconf);

VCONF_API int VCONF_CDECL vconf_set_chunk(
	vconf_t* vconf, 
	unsigned int chunk);

VCONF_API int VCONF_CDECL vconf_read_file(
	vconf_t* vconf, 
	const char* filename);

VCONF_API int VCONF_CDECL vconf_write_file(
	vconf_t* vconf, 
	const char* filename);

VCONF_API const char* VCONF_CDECL vconf_get_filename(
	vconf_t* vconf);

VCONF_API vhash_t* VCONF_CDECL vconf_get_option_hash(
	vconf_t* vconf);

VCONF_API const char* VCONF_CDECL vconf_get_option(
	vconf_t* vconf, 
	const char * option);

VCONF_API void VCONF_CDECL vconf_set_option(
	vconf_t* vconf, 
	const char * option,
	const char * value);

#ifndef HOOPS_DEFINED
#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */
#endif

#endif /*VCONF_H*/


