#include "toolkit.h"

int eFont::loaddef(int variant) {
  string path;
  // TODO: kill this ugly default font
  switch (variant) {
    case eFontDefault:
      path="/usr/share/fonts/TTF/DejaVuSans.ttf";
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
