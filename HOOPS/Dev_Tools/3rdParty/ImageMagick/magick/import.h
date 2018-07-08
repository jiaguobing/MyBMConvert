/*
  Image Import Command Methods.
*/
#ifndef _MAGICK_IMPORT_H
#define _MAGICK_IMPORT_H

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

extern MagickExport MagickBooleanType
  ImportImageCommand(ImageInfo *,int,char **,char **,ExceptionInfo *);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
