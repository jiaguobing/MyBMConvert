/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%                  SSSSS  TTTTT   AAA   TTTTT  IIIII   CCCC                   %
%                  SS       T    A   A    T      I    C                       %
%                   SSS     T    AAAAA    T      I    C                       %
%                     SS    T    A   A    T      I    C                       %
%                  SSSSS    T    A   A    T    IIIII   CCCC                   %
%                                                                             %
%                                                                             %
%                         ImageMagick Static Methods                          %
%                                                                             %
%                              Software Design                                %
%                                John Cristy                                  %
%                                 March 2000                                  %
%                                                                             %
%                                                                             %
%  Copyright 1999-2006 ImageMagick Studio LLC, a non-profit organization      %
%  dedicated to making software imaging solutions freely available.           %
%                                                                             %
%  You may not use this file except in compliance with the License.  You may  %
%  obtain a copy of the License at                                            %
%                                                                             %
%    http://www.imagemagick.org/script/license.php                            %
%                                                                             %
%  Unless required by applicable law or agreed to in writing, software        %
%  distributed under the License is distributed on an "AS IS" BASIS,          %
%  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   %
%  See the License for the specific language governing permissions and        %
%  limitations under the License.                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%
%
*/

/*
  Include declarations.
*/
#include "magick/studio.h"
#include "magick/exception-private.h"
#include "magick/module.h"
#include "magick/static.h"
#include "magick/string_.h"

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   E x e c u t e S t a t i c M o d u l e P r o c e s s                       %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  ExecuteStaticModuleProcess() is just a template method.
%
%  The format of the ExecuteStaticModuleProcess method is:
%
%      MagickBooleanType ExecuteStaticModuleProcess(const char *tag,
%        Image **image,const int argc,char **argv)
%
%  A description of each parameter follows:
%
%    o tag: The module tag.
%
%    o image: The image.
%
%    o argc: The number of elements in the argument vector.
%
%    o argv: A text array containing the command line arguments.
%
*/
#if defined(SupportMagickModules)
MagickExport MagickBooleanType ExecuteStaticModuleProcess(const char *tag,
  Image **image,const int argc,char **argv)
{
  MagickBooleanType
    status;

  assert(image != (Image **) NULL);
  assert((*image)->signature == MagickSignature);
  if ((*image)->debug != MagickFalse)
    (void) LogMagickEvent(TraceEvent,GetMagickModule(),"%s",(*image)->filename);
  status=MagickFalse;
#if !defined(BuildMagickModules)
  {
    MagickBooleanType
      (*module)(Image **,const int,char **);

    module=(MagickBooleanType (*)(Image **,const int,char **)) NULL;
    if (LocaleCompare("analyze",tag) == 0)
      module=AnalyzeImage;
    if (module != (MagickBooleanType (*)(Image **,const int,char **)) NULL)
      {
        if ((*image)->debug != MagickFalse)
          (void) LogMagickEvent(CoderEvent,GetMagickModule(),
            "Invoking \"%s\" static filter module",tag);
        status=(*module)(image,argc,argv);
        if ((*image)->debug != MagickFalse)
          (void) LogMagickEvent(CoderEvent,GetMagickModule(),"\"%s\" completes",
            tag);
      }
  }
#endif
  return(status);
}
#endif

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   R e g i s t e r S t a t i c M o d u l e s                                 %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  RegisterStaticModules() statically registers all the available module
%  handlers.
%
%  The format of the RegisterStaticModules method is:
%
%      RegisterStaticModules(void)
%
%
*/
MagickExport void RegisterStaticModules(void)
{
#if !defined(BuildMagickModules)
  RegisterBMPImage();
  RegisterGIFImage();
  RegisterICONImage();
#if defined(HasJPEG)
  RegisterJPEGImage();
#endif
  RegisterPCXImage();
  RegisterPICTImage();
  RegisterPIXImage();
#if defined(HasPNG)
  RegisterPNGImage();
#endif
  RegisterPNMImage();
  RegisterRAWImage();
  RegisterRGBImage();
  RegisterSFWImage();
  RegisterSGIImage();
  RegisterSUNImage();
#if defined(HasTIFF)
  RegisterTIFFImage();
#endif
  RegisterTGAImage();
  RegisterXBMImage();
  RegisterXCFImage();
  RegisterXPMImage();
#endif
}

/*
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                                                             %
%                                                                             %
%                                                                             %
%   U n r e g i s t e r S t a t i c M o d u l e s                             %
%                                                                             %
%                                                                             %
%                                                                             %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  UnregisterStaticModules() statically unregisters all the available module
%  handlers.
%
%  The format of the UnregisterStaticModules method is:
%
%      UnregisterStaticModules(void)
%
%
*/
MagickExport void UnregisterStaticModules(void)
{
#if !defined(BuildMagickModules)
  UnregisterBMPImage();
  UnregisterGIFImage();
  UnregisterICONImage();
#if defined(HasJPEG)
  UnregisterJPEGImage();
#endif
  UnregisterPCXImage();
  UnregisterPICTImage();
  UnregisterPIXImage();
#if defined(HasPNG)
  UnregisterPNGImage();
#endif
  UnregisterPNMImage();
  UnregisterRAWImage();
  UnregisterRGBImage();
  UnregisterSGIImage();
  UnregisterSUNImage();
  UnregisterTGAImage();
#if defined(HasTIFF)
  UnregisterTIFFImage();
#endif
  UnregisterXBMImage();
  UnregisterXCFImage();
  UnregisterXPMImage();
#endif
}
