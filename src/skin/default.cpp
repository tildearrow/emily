#include "toolkit.h"

eColor eSkin::getDefaultColor(int objectType) {
  eColor temp;
  switch (objectType) {
    case eObjectButton: case eObjectSliderH:
      temp.r=0.5;
      temp.g=0.5;
      temp.b=0.5;
      temp.a=1;
      return temp;
      break;
    case eObjectSliderB:
      temp.r=1;
      temp.g=1;
      temp.b=1;
      temp.a=1;
      return temp;
      break;
  }
  temp.r=0;
  temp.g=0;
  temp.b=0;
  temp.a=1;
  return temp;
};

sf::Texture* eSkin::getTexture(int objectType, int attrib[8], int w, int h, double* xo, double* yo, int* frameWidth, int* frameHeight) {
  sf::Texture* ret;
  eBitmap* bitmap;
  eBitmap* bitmap1;
  eBitmap* bitmap2;
  eBitmap* retBitmap;
  eColor temp1, temp2;
  switch (objectType) {
    case eObjectButton:
      if (attrib[4]==eButtonNoFrame) {
        // widget handles button drawing
        retBitmap=new eBitmap(w*engine->scale*3,h*engine->scale);
        retBitmap->clear();
        ret=retBitmap->toTexture();
        *xo=0*engine->scale;
        *yo=0*engine->scale;
        *frameWidth=(w)*engine->scale;
        *frameHeight=(h)*engine->scale;
        delete retBitmap;
        return ret;
        break;
      }
      if (attrib[4]==eButtonFlat) {
        temp1=*(eColor*)attrib;
        temp1.r*=0.75;
        temp1.g*=0.75;
        temp1.b*=0.75;
        temp1.a=1;
        temp2=*(eColor*)attrib;
        temp2.r*=1.5;
        if (temp2.r>1) {
          temp2.r=1;
        }
        temp2.g*=1.5;
        if (temp2.g>1) {
          temp2.g=1;
        }
        temp2.b*=1.5;
        if (temp2.b>1) {
          temp2.b=1;
        }
        temp2.a=1;
        retBitmap=new eBitmap(w*engine->scale*3,h*engine->scale);
        bitmap=new eBitmap(w*engine->scale,h*engine->scale);
        bitmap1=new eBitmap(w*engine->scale,h*engine->scale);
        
        // NORMAL BEGIN //
        bitmap->clearToColor(temp1);
        bitmap->shadeVGrad(0,1,{1,1,1,1},{0.67,0.67,0.67,1});
        retBitmap->copyBlitOn(bitmap,0,0);
        // NORMAL END //
        
        // HIGHLIGHT BEGIN //
        bitmap1->clear();
        bitmap1->rect(0,0,w*engine->scale,1*engine->scale,temp2);
        bitmap1->rect(0,0,1*engine->scale,h*engine->scale,temp2);
        bitmap1->rect((w-1)*engine->scale,0,1*engine->scale,h*engine->scale,temp2);
        temp2.r*=0.5;
        temp2.g*=0.5;
        temp2.b*=0.5;
        bitmap1->rect(1*engine->scale,1*engine->scale,(w-2)*engine->scale,1*engine->scale,temp2);
        bitmap1->rect(1*engine->scale,1*engine->scale,1*engine->scale,(h-1)*engine->scale,temp2);
        bitmap1->rect((w-2)*engine->scale,1*engine->scale,1*engine->scale,(h-1)*engine->scale,temp2);
        bitmap1->shadeHMGrad({1,1,1,0.5},{1,1,1,1});
        bitmap1->shadeVGrad(0,0.83,{1,1,1,0.67},{1,1,1,0});
        retBitmap->copyBlitOn(bitmap1,w*engine->scale,0);
        // HIGHLIGHT END //
        
        // NORMAL BEGIN //
        bitmap->clearToColor(temp1);
        bitmap->shadeVGrad(0,1,{0.75,0.75,0.75,1},{1,1,1,1});
        bitmap->blitOn(bitmap1,0,0);
        retBitmap->copyBlitOn(bitmap,2*w*engine->scale,0);
        delete bitmap1;
        delete bitmap;
        // NORMAL END //
        
        temp1.r*=1;
        temp1.g*=1;
        temp1.b*=1;
        ret=retBitmap->toTexture();
        *xo=0*engine->scale;
        *yo=0*engine->scale;
        *frameWidth=(w)*engine->scale;
        *frameHeight=(h)*engine->scale;
        delete retBitmap;
      } else {
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
        bitmap=new eBitmap((w+10)*engine->scale,(h+10)*engine->scale);
        bitmap1=new eBitmap(w*engine->scale,h*engine->scale);
        bitmap2=new eBitmap(w*engine->scale,h*engine->scale);
        retBitmap->clear();
        
        // NORMAL BEGIN //
        bitmap1->clear();
        bitmap1->roundRect(1*engine->scale,1*engine->scale,(w-2)*engine->scale,(h-2)*engine->scale,2*engine->scale,temp2);
        bitmap1->shadeVGrad(0,1,{1,1,1,1},{0.83,0.83,0.83,1});
        
        bitmap2->clear();
        bitmap2->roundRect(2*engine->scale,2*engine->scale,(w-4)*engine->scale,(h-4)*engine->scale,2*engine->scale,{1,1,1,1});
        bitmap2->shadeVGrad(0,0.5,{1,1,1,0.075},{1,1,1,0});
        
        bitmap1->blitOn(bitmap2,0,0);
        
        bitmap->clear();
        bitmap->roundRect(4*engine->scale,4*engine->scale,(w+2)*engine->scale,(h+2)*engine->scale,2*engine->scale,{1,1,1,1});
        bitmap->shadeGlowBack(2*engine->scale,2,temp1);
        
        bitmap->roundRect(5*engine->scale,5*engine->scale,(w)*engine->scale,(h)*engine->scale,2*engine->scale,temp1);
        bitmap->blitOn(bitmap1,5*engine->scale,5*engine->scale);
        
        retBitmap->copyBlitOn(bitmap,0,0);
        // NORMAL END //
        
        // HIGHLIGHT BEGIN //
        bitmap1->clear();
        bitmap1->roundRect(1*engine->scale,1*engine->scale,(w-2)*engine->scale,(h-2)*engine->scale,2*engine->scale,temp2);
        bitmap1->shadeVGrad(0,1,{1,1,1,1},{0.83,0.83,0.83,1});
        
        bitmap1->blitOn(bitmap2,0,0);
        bitmap1->roundRect(1*engine->scale,1*engine->scale,(w-2)*engine->scale,(h-2)*engine->scale,2*engine->scale,{1,1,1,0.075});
        
        bitmap->clear();
        bitmap->roundRect(4*engine->scale,4*engine->scale,(w+2)*engine->scale,(h+2)*engine->scale,2*engine->scale,{1,1,1,1});
        bitmap->shadeGlowBack(3*engine->scale,2,temp1);
        
        bitmap->roundRect(5*engine->scale,5*engine->scale,(w)*engine->scale,(h)*engine->scale,2*engine->scale,temp1);
        bitmap->blitOn(bitmap1,5*engine->scale,5*engine->scale);
        
        retBitmap->copyBlitOn(bitmap,(w+10)*engine->scale,0);
        // HIGHLIGHT END //
        
        // CLICK BEGIN //
        bitmap1->clear();
        bitmap1->roundRect(1*engine->scale,1*engine->scale,(w-2)*engine->scale,(h-2)*engine->scale,2*engine->scale,temp2);
        bitmap1->shadeVGrad(0,1,{0.83,0.83,0.83,1},{1.5,1.5,1.5,1});
        
        bitmap2->clear();
        bitmap2->roundRect(2*engine->scale,2*engine->scale,(w-4)*engine->scale,(h-4)*engine->scale,2*engine->scale,{1,1,1,1});
        bitmap2->shadeVGrad(0,1,{1,1,1,0.075},{1,1,1,0});
        
        bitmap1->blitOn(bitmap2,0,0);
        bitmap1->shadeColor({0.8,0.8,0.8,1});
        
        bitmap->clear();
        /*
        bitmap->roundRect(4*engine->scale,4*engine->scale,(w+2)*engine->scale,(h+2)*engine->scale,2*engine->scale,{1,1,1,1});
        bitmap->shadeGlowBack(3*engine->scale,2);
        bitmap->shadeColor(temp1);
        */
        
        bitmap->roundRect(5*engine->scale,5*engine->scale,(w)*engine->scale,(h)*engine->scale,2*engine->scale,temp1);
        bitmap->blitOn(bitmap1,5*engine->scale,5*engine->scale);
        
        retBitmap->copyBlitOn(bitmap,(w+10)*engine->scale*2,0);
        // CLICK END //
        
        ret=retBitmap->toTexture();
        *xo=5*engine->scale;
        *yo=5*engine->scale;
        *frameWidth=(w+10)*engine->scale;
        *frameHeight=(h+10)*engine->scale;
        delete bitmap;
        delete bitmap1;
        delete bitmap2;
        delete retBitmap;
      }
      return ret;
      break;
    case eObjectLight:
      retBitmap=new eBitmap(w*engine->scale*3,h*engine->scale);
      switch (attrib[1]) {
        case eLightPlastic:
          retBitmap->clearToColor({0.2,0.2,0.2,1});
          *xo=0;
          *yo=0;
          *frameWidth=w*engine->scale;
          *frameHeight=h*engine->scale;
          ret=retBitmap->toTexture();
          return ret;
          break;
        case eLightRubber:
          bitmap=new eBitmap(w*engine->scale,h*engine->scale);
          bitmap->clear();
          bitmap->roundRect(0,0,w*engine->scale,h*engine->scale,4*engine->scale,{0.1f+0.5f*((float*)attrib)[0],0.1f+0.5f*((float*)attrib)[0],0.1f+0.5f*((float*)attrib)[0],0.8});
          bitmap->roundRect(0.5*engine->scale,0.5*engine->scale,(w-1)*engine->scale,(h-1)*engine->scale,4*engine->scale,{0.1f+0.5f*((float*)attrib)[0],0.1f+0.5f*((float*)attrib)[0],0.1f+0.5f*((float*)attrib)[0],0.9});
          bitmap->roundRect(1*engine->scale,1*engine->scale,(w-2)*engine->scale,(h-2)*engine->scale,4*engine->scale,{0.1f+0.5f*((float*)attrib)[0],0.1f+0.5f*((float*)attrib)[0],0.1f+0.5f*((float*)attrib)[0],1});
          
          // light
          bitmap1=new eBitmap(w*engine->scale,h*engine->scale);
          bitmap1->clear();
          bitmap1->roundRect(0.5*engine->scale,0.5*engine->scale,(w-1)*engine->scale,(h-1)*engine->scale,4*engine->scale,{1,1,1,1});
          bitmap1->shadeHMGrad({1,1,1,0.3},{1,1,1,0.66667f-((float*)attrib)[0]*0.15f});
          bitmap1->shadeVMGrad({1,1,1,0.3},{1,1,1,0.66667f-((float*)attrib)[0]*0.15f});
          
          // reflections
          bitmap2=new eBitmap(w*engine->scale,h*engine->scale);
          bitmap2->clear();
          bitmap2->shadeVGrad(0,3/(float)h,{1.0f,1.0f,1.0f,1.0f},{1.2f,1.2f,1.2f,1.0f});
          bitmap2->shadeVGrad(3/(float)h,6/(float)h,{1.0f,1.0f,1.0f,1.0f},{0.83333333f,0.83333333f,0.83333333f,1.0f});
          bitmap2->shadeHGrad(0,3/(float)w,{1.0f,1.0f,1.0f,1.0f},{1.1f,1.1f,1.1f,1.0f});
          bitmap2->shadeHGrad(3/(float)w,6/(float)w,{1.0f,1.0f,1.0f,1.0f},{0.9090909091f,0.9090909091f,0.9090909091f,1.0f});
          bitmap2->shadeHGrad(((float)w-6)/(float)w,((float)w-2)/(float)w,{1.0f,1.0f,1.0f,1.0f},{0.9f,0.9f,0.9f,1.0f});
          bitmap2->shadeVGrad(((float)h-6)/(float)h,((float)h-2)/(float)h,{1.0f,1.0f,1.0f,1.0f},{0.9f,0.9f,0.9f,1.0f});
          // slight radial gradient
          bitmap2->shadeHMGrad({0.8f,0.8f,0.8f,1.0f},{1.0f,1.0f,1.0f,1.0f});
          bitmap2->shadeVMGrad({0.8f,0.8f,0.8f,1.0f},{1.0f,1.0f,1.0f,1.0f});
          
          retBitmap->clear();
          
          retBitmap->copyBlitOn(bitmap,0,0);
          retBitmap->copyBlitOn(bitmap1,w*engine->scale,0);
          retBitmap->copyBlitOn(bitmap2,w*engine->scale*2,0);
          *xo=0;
          *yo=0;
          *frameWidth=w*engine->scale;
          *frameHeight=h*engine->scale;
          ret=retBitmap->toTexture();
          delete bitmap;
          delete bitmap1;
          delete bitmap2;
          return ret;
          break;
        default:
          eLogE("specify a style!\n");
          return NULL;
          break;
      }
      break;
    case eObjectSliderH:
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
        
      bitmap->clear();
      bitmap->roundRect(4*engine->scale,4*engine->scale,(w+2)*engine->scale,(h+2)*engine->scale,2*engine->scale,{1,1,1,1});
      bitmap->shadeGlowBack(2*engine->scale,2,temp1);
        
      bitmap->roundRect(5*engine->scale,5*engine->scale,(w)*engine->scale,(h)*engine->scale,2*engine->scale,temp1);
      bitmap->blitOn(bitmap1,5*engine->scale,5*engine->scale);
        
      retBitmap->copyBlitOn(bitmap,0,0);
      // NORMAL END //
      
      ret=retBitmap->toTexture();
      *xo=5*engine->scale;
      *yo=5*engine->scale;
      *frameWidth=(w+10)*engine->scale;
      *frameHeight=(h+10)*engine->scale;
      delete bitmap;
      delete bitmap1;
      delete bitmap2;
      delete retBitmap;
      return ret;
      break;
    case eObjectSliderB:
      temp1=*(eColor*)attrib;
      retBitmap=new eBitmap((w+10)*engine->scale,16*engine->scale*3);
      bitmap=new eBitmap((w+10)*engine->scale,16*engine->scale);
      
      temp1.a=1;
      // NORMAL BEGIN //
      bitmap->clear();
      bitmap->rect(5*engine->scale,8*engine->scale,w*engine->scale,engine->scale,temp1);
      retBitmap->copyBlitOn(bitmap,0,0);
      // NORMAL END //
      
      // LIT BEGIN //
      bitmap->clear();
      bitmap->rect(3*engine->scale,7.5*engine->scale,(w+4)*engine->scale,2*engine->scale,{1,1,1,1});
      bitmap->shadeGlowBack(6,2,temp1);
      bitmap->rect(5*engine->scale,8*engine->scale,w*engine->scale,engine->scale,temp1);
      retBitmap->copyBlitOn(bitmap,0,16*engine->scale);
      // LIT END //
      
      // HIGHLIGHT BEGIN //
      bitmap->clear();
      bitmap->rect(3*engine->scale,7.5*engine->scale,(w+4)*engine->scale,2*engine->scale,{1,1,1,1});
      bitmap->shadeGlowBack(6,2,temp1);
      bitmap->rect(5*engine->scale,8*engine->scale,w*engine->scale,engine->scale,temp1);
      retBitmap->copyBlitOn(bitmap,0,16*engine->scale*2);
      // HIGHLIGHT END //
      
      ret=retBitmap->toTexture();
      *xo=5*engine->scale;
      *yo=8*engine->scale;
      *frameWidth=(w+10)*engine->scale;
      *frameHeight=16*engine->scale;
      delete retBitmap;
      delete bitmap;
      return ret;
      break;
    default:
      return NULL;
  }
  return NULL;
}
