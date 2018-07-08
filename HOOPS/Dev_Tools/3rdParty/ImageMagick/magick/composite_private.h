/*
  ImageMagick Image Composite Private Methods.
*/
#ifndef _MAGICK_COMPOSITE_PRIVATE_H
#define _MAGICK_COMPOSITE_PRIVATE_H

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

/*
  ImageMagick Alpha Composite Inline Methods.
*/

static inline PixelPacket MagickCompositeAtop(const PixelPacket *p,
  const MagickRealType alpha,const PixelPacket *q,const MagickRealType beta)
{
  MagickRealType
    gamma;

  PixelPacket
    composite;

  gamma=(1-QuantumScale*beta);
  composite.opacity=(Quantum) (MaxRGB*(1-gamma)+0.5);
	gamma=1.0/(gamma <= MagickEpsilon ? 1.0 : gamma);
  composite.red=(Quantum) (gamma*((1-QuantumScale*alpha)*p->red*
    (1-QuantumScale*beta)+(1-QuantumScale*beta)*q->red*
    QuantumScale*alpha)+0.5);
  composite.green=(Quantum) (gamma*((1-QuantumScale*alpha)*p->green*
    (1-QuantumScale*beta)+(1-QuantumScale*beta)*q->green*
    QuantumScale*alpha)+0.5);
  composite.blue=(Quantum) (gamma*((1-QuantumScale*alpha)*p->blue*
    (1-QuantumScale*beta)+(1-QuantumScale*beta)*q->blue*
    QuantumScale*alpha)+0.5);
  return(composite);
}

static inline PixelPacket MagickCompositeOver(const PixelPacket *p,
  const MagickRealType alpha,const PixelPacket *q,const MagickRealType beta)
{
  MagickRealType
    gamma;

  PixelPacket
    composite;

  gamma=1.0-QuantumScale*QuantumScale*alpha*beta;
  composite.opacity=(Quantum) (MaxRGB*(1-gamma)+0.5);
  gamma=1.0/(gamma <= MagickEpsilon ? 1.0 : gamma);
  composite.red=(Quantum) (gamma*((1-QuantumScale*alpha)*p->red+
    (1-QuantumScale*beta)*q->red*QuantumScale*alpha)+0.5);
  composite.green=(Quantum) (gamma*((1-QuantumScale*alpha)*p->green+
    (1-QuantumScale*beta)*q->green*QuantumScale*alpha)+0.5);
  composite.blue=(Quantum) (gamma*((1-QuantumScale*alpha)*p->blue+
    (1-QuantumScale*beta)*q->blue*QuantumScale*alpha)+0.5);
  return(composite);
}

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
