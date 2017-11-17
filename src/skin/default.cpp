#include "toolkit.h"

eColor eSkin::getDefaultColor(int objectType) {
  eColor temp;
  temp.r=0.5;
  temp.g=0.5;
  temp.b=0.5;
  temp.a=1;
  return temp;
};

sf::Texture* eSkin::getTexture(int objectType, int attrib[8], int w, int h, double* xo, double* yo) {
  sf::Texture* ret;
  eBitmap* bitmap;
  switch (objectType) {
    case eObjectButton:
      eColor temp1, temp2;
      temp1=*(eColor*)attrib;
      temp2=*(eColor*)attrib;
      temp2.r*=0.5;
      temp2.g*=0.5;
      temp2.b*=0.5;
      bitmap=new eBitmap((w+10)*engine->scale,(h+10)*engine->scale);
      bitmap->clear();
      bitmap->roundRect(5*engine->scale,5*engine->scale,(w)*engine->scale,(h)*engine->scale,2*engine->scale,temp1);
      //bitmap->roundRect(1*engine->scale,1*engine->scale,(w-2)*engine->scale,(h-2)*engine->scale,2*engine->scale,temp2);
      bitmap->shadeGlowBack(3*engine->scale,2);
      bitmap->roundRect(5*engine->scale,5*engine->scale,(w)*engine->scale,(h)*engine->scale,2*engine->scale,{0,0,0,1});
      //bitmap->roundRect(6*engine->scale,6*engine->scale,(w-2)*engine->scale,(h-2)*engine->scale,2*engine->scale,temp2);
      ret=bitmap->toTexture();
      delete bitmap;
      *xo=5*engine->scale;
      *yo=5*engine->scale;
      return ret;
      break;
    default:
      return NULL;
  }
}
