#include "toolkit.h"

void eFont::size(float size) {
  // TODO: multi-DPI support
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
  if (1) {
    return 0;
  }
  return 1;
}

eBitmap* eFont::render(string text) {
  eBitmap* result;
  int penX, penY, len;
  // get size
  len=0;
  for (int i=0; i<text.length(); i++) {
    //FT_Load_Glyph(face,FT_Get_Char_Index(face,text[i]),FT_LOAD_DEFAULT);
    //len+=face->glyph->advance.x;
  }
  eLogD("%d\n",len>>6);
  result=new eBitmap(len>>6,32,32);
  for (int i=0; i<text.length(); i++) {
    //FT_Load_Char(face,text[i],FT_LOAD_RENDER);
    /*
    result->blitFromFTBitmap(&face->glyph->bitmap,penX+face->glyph->bitmap_left,penY-face->glyph->bitmap_top);*/
  }
  return result;
}

eFont::eFont() {
}
