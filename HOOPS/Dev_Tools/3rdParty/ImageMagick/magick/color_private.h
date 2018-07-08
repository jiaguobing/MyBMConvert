/*
  ImageMagick Image Color methods.
*/
#ifndef _MAGICK_COLOR_PRIVATE_H
#define _MAGICK_COLOR_PRIVATE_H

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#include <magick/error.h>

static inline IndexPacket ConstrainColormapIndex(Image *image,
  const unsigned long index)
{
  if (index >= image->colors)
    {
      (void) ThrowMagickException(&image->exception,GetMagickModule(),
        CorruptImageError,"InvalidColormapIndex",image->filename);
      return(0);
    }
  return((IndexPacket) index);
}

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
