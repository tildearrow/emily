#include "toolkit.h"

eColor eSkin::getDefaultColor(int objectType) {
  eColor temp;
  temp.r=0.5;
  temp.g=0.5;
  temp.b=0.5;
  temp.a=1;
  return temp;
};

sf::Texture* eSkin::getTexture(int objectType, int attrib[8], int w, int h, double* xo, double* yo, int* frameWidth, int* frameHeight) {
  sf::Texture* ret;
  eBitmap* bitmap;
  eBitmap* bitmap1;
  eBitmap* bitmap2;
  eBitmap* retBitmap;
  switch (objectType) {
    case eObjectButton:
      eColor temp1, temp2;
      temp1=*(eColor*)attrib;
      temp1.r*=1.5;
      if (temp1.r>1) {
        temp1.r=1;
      }
      temp1.g*=1.5;
      if (temp1.g>1) {
        temp1.g=1;
      }
      temp1.b*=1.5;
      if (temp1.b>1) {
        temp1.b=1;
      }
      temp1.a=1;
      temp2=*(eColor*)attrib;
      temp2.r*=0.5;
      temp2.g*=0.5;
      temp2.b*=0.5;
      temp2.a=1;
      
      retBitmap=new eBitmap((w+10)*engine->scale*3,(h+10)*engine->scale);
      retBitmap->clear();
      
      // NORMAL BEGIN //
      bitmap=new eBitmap((w+10)*engine->scale,(h+10)*engine->scale);
      bitmap1=new eBitmap(w*engine->scale,h*engine->scale);
      bitmap2=new eBitmap(w*engine->scale,h*engine->scale);
      
      bitmap1->clear();
      bitmap1->roundRect(1*engine->scale,1*engine->scale,(w-2)*engine->scale,(h-2)*engine->scale,2*engine->scale,temp2);
      bitmap1->shadeVGrad(0,1,{1,1,1,1},{0.83,0.83,0.83,1});
      
      bitmap2->clear();
      bitmap2->roundRect(2*engine->scale,2*engine->scale,(w-4)*engine->scale,(h-4)*engine->scale,2*engine->scale,{1,1,1,1});
      bitmap2->shadeVGrad(0,0.5,{1,1,1,0.075},{1,1,1,0});
      
      bitmap1->blitOn(bitmap2,0,0);
      delete bitmap2;
      
      bitmap->clear();
      bitmap->roundRect(4*engine->scale,4*engine->scale,(w+2)*engine->scale,(h+2)*engine->scale,2*engine->scale,{1,1,1,1});
      bitmap->shadeGlowBack(3*engine->scale,2);
      bitmap->shadeColor(temp1);
      
      bitmap->roundRect(5*engine->scale,5*engine->scale,(w)*engine->scale,(h)*engine->scale,2*engine->scale,temp1);
      bitmap->blitOn(bitmap1,5*engine->scale,5*engine->scale);
      
      delete bitmap1;
      
      retBitmap->blitOn(bitmap,0,0);
      
      delete bitmap;
      // NORMAL END //
      
      // HIGHLIGHT BEGIN //
      bitmap=new eBitmap((w+10)*engine->scale,(h+10)*engine->scale);
      bitmap1=new eBitmap(w*engine->scale,h*engine->scale);
      bitmap2=new eBitmap(w*engine->scale,h*engine->scale);
      
      bitmap1->clear();
      bitmap1->roundRect(1*engine->scale,1*engine->scale,(w-2)*engine->scale,(h-2)*engine->scale,2*engine->scale,temp2);
      bitmap1->shadeVGrad(0,1,{1,1,1,1},{0.83,0.83,0.83,1});
      
      bitmap2->clear();
      bitmap2->roundRect(2*engine->scale,2*engine->scale,(w-4)*engine->scale,(h-4)*engine->scale,2*engine->scale,{1,1,1,1});
      bitmap2->shadeVGrad(0,0.5,{1,1,1,0.075},{1,1,1,0});
      
      bitmap1->blitOn(bitmap2,0,0);
      bitmap1->roundRect(1*engine->scale,1*engine->scale,(w-2)*engine->scale,(h-2)*engine->scale,2*engine->scale,{1,1,1,0.075});
      delete bitmap2;
      
      bitmap->clear();
      bitmap->roundRect(3*engine->scale,3*engine->scale,(w+4)*engine->scale,(h+4)*engine->scale,2*engine->scale,{1,1,1,1});
      bitmap->shadeGlowBack(3*engine->scale,2);
      bitmap->shadeColor(temp1);
      
      bitmap->roundRect(5*engine->scale,5*engine->scale,(w)*engine->scale,(h)*engine->scale,2*engine->scale,temp1);
      bitmap->blitOn(bitmap1,5*engine->scale,5*engine->scale);
      
      delete bitmap1;
      
      retBitmap->blitOn(bitmap,(w+10)*engine->scale,0);
      
      delete bitmap;
      // HIGHLIGHT END //
      
      // CLICK BEGIN //
      bitmap=new eBitmap((w+10)*engine->scale,(h+10)*engine->scale);
      bitmap1=new eBitmap(w*engine->scale,h*engine->scale);
      bitmap2=new eBitmap(w*engine->scale,h*engine->scale);
      
      bitmap1->clear();
      bitmap1->roundRect(1*engine->scale,1*engine->scale,(w-2)*engine->scale,(h-2)*engine->scale,2*engine->scale,temp2);
      bitmap1->shadeVGrad(0,1,{0.83,0.83,0.83,1},{1.5,1.5,1.5,1});
      
      bitmap2->clear();
      bitmap2->roundRect(2*engine->scale,2*engine->scale,(w-4)*engine->scale,(h-4)*engine->scale,2*engine->scale,{1,1,1,1});
      bitmap2->shadeVGrad(0,1,{1,1,1,0.075},{1,1,1,0});
      
      bitmap1->blitOn(bitmap2,0,0);
      delete bitmap2;
      bitmap1->shadeColor({0.8,0.8,0.8,1});
      
      bitmap->clear();
      /*
      bitmap->roundRect(4*engine->scale,4*engine->scale,(w+2)*engine->scale,(h+2)*engine->scale,2*engine->scale,{1,1,1,1});
      bitmap->shadeGlowBack(3*engine->scale,2);
      bitmap->shadeColor(temp1);
      */
      
      bitmap->roundRect(5*engine->scale,5*engine->scale,(w)*engine->scale,(h)*engine->scale,2*engine->scale,temp1);
      bitmap->blitOn(bitmap1,5*engine->scale,5*engine->scale);
      
      delete bitmap1;
      
      retBitmap->blitOn(bitmap,(w+10)*engine->scale*2,0);
      
      delete bitmap;
      // CLICK END //
      
      ret=retBitmap->toTexture();
      *xo=5*engine->scale;
      *yo=5*engine->scale;
      *frameWidth=(w+10)*engine->scale;
      *frameHeight=(h+10)*engine->scale;
      delete retBitmap;
      return ret;
      break;
    default:
      return NULL;
  }
}
