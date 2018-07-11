/*
  Image Command Methods.
*/
#ifndef _MAGICK_MOGRIFY_H
#define _MAGICK_MOGRIFY_H

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

extern MagickExport MagickBooleanType
  MogrifyImage(const ImageInfo *,const int,char **,Image **),
  MogrifyImageCommand(ImageInfo *,int,char **,char **,ExceptionInfo *),
  MogrifyImageList(const ImageInfo *,const int,char **,Image **),
  MogrifyImages(const ImageInfo *,const int,char **,Image **);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
