/*
  ImageMagick Exception Methods.
*/
#ifndef _MAGICK_ERROR_H
#define _MAGICK_ERROR_H

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#include "magick/log.h"

#if !defined(__attribute__)
#  define __attribute__(x) /*nothing*/
#endif

#define ThrowBinaryException(severity,tag,context) \
{ \
  if (image != (Image *) NULL) \
    (void) ThrowMagickException(&image->exception,GetMagickModule(),severity, \
      tag,context); \
  return(MagickFalse); \
}
#define ThrowFileException(exception,severity,tag,context) \
  (void) ThrowMagickException(exception,GetMagickModule(),severity,tag, \
    context,strerror(errno));
#define ThrowImageException(severity,tag) \
{ \
  (void) ThrowMagickException(exception,GetMagickModule(),severity,tag, \
    image->filename);\
  return((Image *) NULL); \
}
#define ThrowMagickFatalException(severity,tag,context) \
{ \
  ExceptionInfo \
    exception; \
 \
  GetExceptionInfo(&exception); \
  (void) ThrowMagickException(&exception,GetMagickModule(),severity,tag, \
    context); \
  CatchException(&exception); \
  DestroyExceptionInfo(&exception); \
}
#define ThrowReaderException(severity,tag) \
{ \
  (void) ThrowMagickException(exception,GetMagickModule(),severity,tag, \
    image_info->filename); \
  if ((image) != (Image *) NULL) \
    { \
      CloseBlob(image); \
      DestroyImageList(image); \
    } \
  return((Image *) NULL); \
}
#define ThrowWriterException(severity,tag) \
{ \
  assert(image != (Image *) NULL); \
  (void) ThrowMagickException(&image->exception,GetMagickModule(),severity, \
    tag,image->filename); \
  if (image_info->adjoin != MagickFalse) \
    while (image->previous != (Image *) NULL) \
      image=image->previous; \
  CloseBlob(image); \
  return(MagickFalse); \
}

typedef void
  (*ErrorHandler)(const ExceptionType,const char *,const char *);

typedef void
  (*FatalErrorHandler)(const ExceptionType,const char *,const char *);

typedef void
  (*WarningHandler)(const ExceptionType,const char *,const char *);

extern MagickExport const char
  *GetLocaleExceptionMessage(const ExceptionType,const char *);

extern MagickExport ErrorHandler
  SetErrorHandler(ErrorHandler);

extern MagickExport FatalErrorHandler
  SetFatalErrorHandler(FatalErrorHandler);

extern MagickExport MagickBooleanType
  SetExceptionInfo(ExceptionInfo *,ExceptionType),
  ThrowException(ExceptionInfo *,const ExceptionType,const char *,const char *),
  ThrowMagickException(ExceptionInfo *,const char *,const char *,
    const unsigned long,const ExceptionType,const char *,...)
    __attribute__((format (printf,6,7)));

extern MagickExport void
  CatchException(ExceptionInfo *),
  DestroyExceptionInfo(ExceptionInfo *),
  GetExceptionInfo(ExceptionInfo *),
  InheritException(ExceptionInfo *,const ExceptionInfo *),
  MagickError(const ExceptionType,const char *,const char *),
  MagickFatalError(const ExceptionType,const char *,const char *),
  MagickWarning(const ExceptionType,const char *,const char *);

extern MagickExport WarningHandler
  SetWarningHandler(WarningHandler);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
