/*
  ImageMagick Random Methods.
*/
#ifndef _MAGICK_RANDOM_H
#define _MAGICK_RANDOM_H

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

extern MagickExport double
  GetRandomValue(void);

extern MagickExport void
  DestroyRandomReservoir(void),
  DistillRandomEvent(const unsigned char *,const size_t),
  GetRandomKey(unsigned char *,const size_t);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
