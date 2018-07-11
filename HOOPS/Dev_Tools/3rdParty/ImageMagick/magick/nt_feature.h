/*
  Windows NT Utility Methods for ImageMagick.
*/
#ifndef _MAGICK_NTFEATURE_H
#define _MAGICK_NTFEATURE_H

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#include "magick/hashmap.h"

extern MagickExport char
  *NTRegistryKeyLookup(const char *key);

extern MagickExport void
  *CropImageToHBITMAP(Image *,const RectangleInfo *,ExceptionInfo *),
  *ImageToHBITMAP(Image* image);

#if !defined(XS_VERSION)

extern MagickExport MagickBooleanType
  NTIsMagickConflict(const char *),
  NTLoadTypeLists(LinkedListInfo *,ExceptionInfo *);

#endif

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
