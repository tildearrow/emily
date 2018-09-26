#include "toolkit.h"
#ifdef __APPLE__
extern "C" {
#include "nsstub.h"
}
#endif

int eFont::loaddef(int variant) {
  string path;
#if defined(HAVE_FONTCONFIG)
  FcPattern* fmpattern;
  FcPattern* match;
  FcResult fmresult;
  FcValue sans;
  FcValue regular;
  FcValue resval;
#endif
  // TODO: kill this ugly default font
  switch (variant) {
    case eFontDefault:
#if defined(_WIN32)
      // windows font code here
      path="C:\\Windows\\Fonts\\segoeui.ttf";
#elif defined(__APPLE__)
      // apple font code here
      // San Francisco to be done
      path="/System/Library/Fonts/Helvetica.dfont";
#elif defined(__unix__)
      // fontconfig font code here
#if defined(HAVE_FONTCONFIG)
      sans.type=FcTypeString;
      sans.u.s=(const FcChar8*)"sans-serif";
      regular.type=FcTypeString;
      regular.u.s=(const FcChar8*)"Regular";
      fmpattern=FcPatternCreate();
      FcPatternAdd(fmpattern,FC_FAMILY,sans,FcTrue);
      FcPatternAdd(fmpattern,FC_STYLE,regular,FcTrue);
      if (!FcInit()) {
        eLogE("Error while initializing fontconfig.\n");
      }
      FcConfigSubstitute(NULL,fmpattern,FcMatchPattern);
      match=FcFontMatch(NULL,fmpattern,&fmresult);
      FcPatternGet(match,FC_FILE,0,&resval);
      path=(const char*)resval.u.s;
      FcPatternDestroy(fmpattern);
      FcPatternDestroy(match);
#else
      eLogE("Wow! Really? No fontconfig library!\n");
#endif
      break;
#else
      // assume /font.ttf
      path="/font.ttf";
#endif
      break;
    case eFontLarge:
      // DejaF*ck
#if defined(_WIN32)
      // windows font code here
      path="C:\\Windows\\Fonts\\segoeui.ttf";
#elif defined(__APPLE__)
      // apple font code here
      path="/System/Library/Fonts/Helvetica.dfont";
#elif defined(__unix__)
      // fontconfig font code here
#if defined(HAVE_FONTCONFIG)
      sans.type=FcTypeString;
      sans.u.s=(const FcChar8*)"sans-serif";
      regular.type=FcTypeString;
      regular.u.s=(const FcChar8*)"Regular";
      fmpattern=FcPatternCreate();
      FcPatternAdd(fmpattern,FC_FAMILY,sans,FcTrue);
      FcPatternAdd(fmpattern,FC_STYLE,regular,FcTrue);
      if (!FcInit()) {
        eLogE("Error while initializing fontconfig.\n");
      }
      FcConfigSubstitute(NULL,fmpattern,FcMatchPattern);
      match=FcFontMatch(NULL,fmpattern,&fmresult);
      FcPatternGet(match,FC_FILE,0,&resval);
      path=(const char*)resval.u.s;
      FcPatternDestroy(fmpattern);
      FcPatternDestroy(match);
#else
      eLogE("Wow! Really? No fontconfig library!\n");
#endif
      break;
#else
      // assume /fontLarge.ttf
      path="/fontLarge.ttf";
#endif
      break;
    default:
      return 0;
  }
  if (!inst.loadFromFile(path)) {
    return 0;
  }
  return 1;
}
