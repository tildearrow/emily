#include "toolkit.h"

void eFont::size(float size) {
  // TODO: multi-DPI support
  FT_Set_Char_Size(face,0,size*64,96,96);
}

int eFont::loaddef(int variant) {
  const char* path;
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
  if (FT_New_Face(lib,path,0,&face)) {
    return 0;
  }
  return 1;
}

eFont::eFont(FT_Library l) {
  lib=l;
}
