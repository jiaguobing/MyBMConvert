/*
  ImageMagick Cache View Methods.
*/
#ifndef _MAGICK_CACHE_VIEW_H
#define _MAGICK_CACHE_VIEW_H

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

typedef struct _ViewInfo
{
  Image
    *image;

  unsigned long
    id;

  unsigned long
    signature;
} ViewInfo;

extern MagickExport const PixelPacket
  *AcquireCacheView(const ViewInfo *,const long,const long,const unsigned long,
    const unsigned long,ExceptionInfo *);

extern MagickExport ExceptionInfo
  *GetCacheViewException(const ViewInfo *);

extern MagickExport IndexPacket
  *GetCacheViewIndexes(const ViewInfo *);

extern MagickExport MagickBooleanType
  SyncCacheView(ViewInfo *);

extern MagickExport PixelPacket
  *GetCacheViewPixels(const ViewInfo *),
  *GetCacheView(ViewInfo *,const long,const long,const unsigned long,
    const unsigned long),
  *SetCacheView(ViewInfo *,const long,const long,const unsigned long,
    const unsigned long);

extern MagickExport ViewInfo
  *OpenCacheView(Image *);

extern MagickExport void
  CloseCacheView(ViewInfo *);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
