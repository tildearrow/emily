#include "toolkit.h"
#ifdef __APPLE__
extern "C" {
#include "nsstub.h"
}
#endif

int eFont::loaddef(int variant) {
  string path;
  // TODO: kill this ugly default font
  switch (variant) {
    case eFontDefault:
#if defined(_WIN32)
      // windows font code here
#elif defined(__APPLE__)
      // apple font code here
      path="/System/Library/Fonts/Helvetica.dfont";
#elif defined(__unix__)
      // fontconfig font code here
      // this is to be finished
      path="/usr/share/fonts/TTF/DejaVuSans.ttf";
#else
      // assume /font.ttf
      path="/font.ttf";
#endif
      break;
    case eFontLarge:
      path="/usr/share/fonts/TTF/DejaVuSerif.ttf";
      break;
    default:
      return 0;
  }
  if (!inst.loadFromFile(path)) {
    return 0;
  }
  return 1;
}
