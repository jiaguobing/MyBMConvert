/*
  ImageMagick Image Drawing Private Methods.
*/
#ifndef _MAGICK_DRAW_PRIVATE_H
#define _MAGICK_DRAW_PRIVATE_H

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#include "magick/image.h"
#include "magick/memory_.h"

static inline PixelPacket GetFillColor(const DrawInfo *draw_info,
  const long x,const long y)
{
  Image
    *pattern;

  PixelPacket
    fill_color;

  pattern=draw_info->fill_pattern;
  if (pattern == (Image *) NULL)
    return(draw_info->fill);
  fill_color=AcquireOnePixel(pattern,
    (x-pattern->extract_info.x) % pattern->columns,
    (y-pattern->extract_info.y) % pattern->rows,&pattern->exception);
  if (pattern->matte == MagickFalse)
    fill_color.opacity=OpaqueOpacity;
  return(fill_color);
}

static inline PixelPacket GetStrokeColor(const DrawInfo *draw_info,
  const long x,const long y)
{
  Image
    *pattern;

  PixelPacket
    stroke_color;

  pattern=draw_info->stroke_pattern;
  if (pattern == (Image *) NULL)
    return(draw_info->stroke);
  stroke_color=AcquireOnePixel(pattern,
    (x-pattern->extract_info.x) % pattern->columns,
    (y-pattern->extract_info.y) % pattern->rows,&pattern->exception);
  return(stroke_color);
}

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
