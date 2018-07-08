/*
  Image Mogrify Command Private Methods.
*/
#ifndef _MAGICK_MOGRIFY_PRIVATE_H
#define _MAGICK_MOGRIFY_PRIVATE_H

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#define MaxImageStackDepth  32
#define MogrifyImageStack(image,fire) \
  if (((image) != (Image *) NULL) && ((fire) != MagickFalse)) \
    { \
      status&=MogrifyImages(image_info,(int) (i-j+1),argv+j,&(image)); \
      if ((image) != (Image *) NULL) \
        GetImageException((image),exception); \
      if (exception->severity != UndefinedException) \
        CatchException(exception); \
      j=i+1; \
      pend=MagickFalse; \
    }

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
