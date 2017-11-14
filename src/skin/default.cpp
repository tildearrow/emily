#include "toolkit.h"

sf::Texture* eSkin::getTexture(int objectType, int attrib[8], int w, int h, double* xo, double* yo) {
  sf::Texture* ret;
  eBitmap* bitmap;
  bitmap=new eBitmap(w*engine->scale,h*engine->scale);
  bitmap->clear();
  bitmap->rect(0,0,w*engine->scale,h*engine->scale,{0,0,1,1});
  ret=bitmap->toTexture();
  delete bitmap;
  *xo=0;
  *yo=0;
  return ret;
}
