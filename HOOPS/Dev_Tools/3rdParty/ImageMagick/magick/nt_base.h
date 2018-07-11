/*
  Windows NT Utility Methods for ImageMagick.
*/
#ifndef _MAGICK_NTBASE_H
#define _MAGICK_NTBASE_H

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#include "magick/delegate.h"

#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <windows.h>
#include <winuser.h>
#include <wingdi.h>
#include <io.h>
#include <process.h>
#include <errno.h>
#if defined(_DEBUG)
#include <crtdbg.h>
#endif

#define PROT_READ  0x01
#define PROT_WRITE  0x02
#define MAP_SHARED  0x01
#define MAP_PRIVATE  0x02
#define MAP_ANONYMOUS  0x20
#define F_OK 0
#define R_OK 4
#define W_OK 2
#define RW_OK 6
#define _SC_PAGESIZE 1
#define _SC_PHYS_PAGES 2

#if !defined(chsize)
# if defined(__BORLANDC__)
#   define chsize(file,length)  chsize(file,length)
# else
#   define chsize(file,length)  _chsize(file,length)
# endif
#endif

#if !defined(access)
#  define access(path,mode)  _access(path,mode)
#endif
#if !defined(pclose)
#  define pclose  _pclose
#endif
#if !defined(popen)
#  define popen  _popen
#endif
#if !defined(strcasecmp)
#  define strcasecmp  strcmpi
#endif
#if !defined(strncasecmp)
#  define strncasecmp  strnicmp
#endif
#if !defined(tempnam)
#  define tempnam  _tempnam
#endif
#if !defined(vsnprintf)
#  define vsnprintf  _vsnprintf
#endif

#if defined(_MT) && defined(__WINDOWS__)
#  define SAFE_GLOBAL  __declspec(thread)
#else
#  define SAFE_GLOBAL
#endif

#if defined(__BORLANDC__)
#undef _O_RANDOM
#define _O_RANDOM 0
#undef _O_SEQUENTIAL
#define _O_SEQUENTIAL 0
#undef _O_SHORT_LIVED
#define _O_SHORT_LIVED 0
#undef _O_TEMPORARY
#define _O_TEMPORARY 0
#endif

typedef UINT (CALLBACK *LPFNDLLFUNC1)(DWORD,UINT);

#if !defined(XS_VERSION)
struct dirent
{
  char
    d_name[2048];

  int
    d_namlen;
};

typedef struct _DIR
{
  HANDLE
    hSearch;

  WIN32_FIND_DATA
    Win32FindData;

  BOOL
    firsttime;

  struct dirent
    file_info;
} DIR;

#endif

#if !defined(ssize_t) && !defined(__MINGW32__)
typedef long ssize_t;
#endif

extern MagickExport char
  *NTGetLastError(void);

extern MagickExport const GhostscriptVectors
  *NTGhostscriptDLLVectors( void );

#if !defined(HasLTDL)
extern MagickExport const char
  *lt_dlerror(void);
#endif

#if !defined(XS_VERSION)
extern MagickExport DIR
  *opendir(char *);

extern MagickExport double
  NTElapsedTime(void),
  NTUserTime(void);

extern MagickExport int
  Exit(int),
  IsWindows95(),
#if !defined(HasLTDL)
  lt_dlclose(void *),
  lt_dlexit(void),
  lt_dlinit(void),
  lt_dlsetsearchpath(const char *),
#endif /* !HasLTDL */
  msync(void *,size_t,int),
  munmap(void *,size_t),
  NTGhostscriptDLL(char *path, int path_length),
  NTGhostscriptEXE(char *path, int path_length),
  NTGhostscriptFonts(char *path, int path_length),
  NTGhostscriptLoadDLL(void),
  NTGhostscriptUnLoadDLL(void),
  NTSystemComman(const char *);

extern MagickExport long
  sysconf(int),
  telldir(DIR *);

extern MagickExport MagickBooleanType
  NTGetExecutionPath(char *),
  NTReportException(const char *,const MagickBooleanType);

extern MagickExport struct dirent
  *readdir(DIR *);

extern MagickExport unsigned char
  *NTResourceToBlob(const char *);

extern MagickExport void
  closedir(DIR *),
#if !defined(HasLTDL)
  *lt_dlopen(const char *),
  *lt_dlsym(void *,const char *),
#endif
  *mmap(char *,size_t,int,int,int,MagickOffsetType),
  NTErrorHandler(const ExceptionType,const char *,const char *),
  NTWarningHandler(const ExceptionType,const char *,const char *),
  seekdir(DIR *,long);

#endif /* !XS_VERSION */

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif /* !C++ */

#endif /* !_MAGICK_NTBASE_H */
