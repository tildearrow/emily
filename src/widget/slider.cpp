#include "toolkit.h"

int eSlider::init() {
  highlight=0;
  clicked=false;
  active=false;
  val=NULL;
  tinstBack=NULL;
  tinstHandle=NULL;
  holdCallback=NULL;
  releaseCallback=NULL;
  valueCallback=NULL;
  setBackColor(engine->skin->getDefaultColor(eObjectSliderB));
  setHandleColor(engine->skin->getDefaultColor(eObjectSliderH));
  setSize(128,24);
  setHandleSize(6);
  setRange(0,1);
  return 1;
}

int eSlider::setHoldCallback(void (*callback)()) {
  holdCallback=callback;
}

int eSlider::setReleaseCallback(void (*callback)()) {
  releaseCallback=callback;
}

int eSlider::setValueCallback(void (*callback)()) {
  valueCallback=callback;
}

int eSlider::setBackColor(eColor col) {
  bcolor=col;
  return 1;
}

int eSlider::setHandleColor(eColor col) {
  hcolor=col;
  return 1;
}

int eSlider::setHandleSize(double rad) {
  int atrList[8];
  hrad=rad;
  if (tinstHandle!=NULL) {
    delete tinstHandle;
    tinstHandle=NULL;
  }
  ((eColor*)atrList)->r=hcolor.r;
  ((eColor*)atrList)->g=hcolor.g;
  ((eColor*)atrList)->b=hcolor.b;
  ((eColor*)atrList)->a=hcolor.a;
  tinstHandle=engine->skin->getTexture(eObjectSliderH,atrList,rad*2,h,&hxo,&hyo,&hfw,&hfh);
  sinstHandle.setTexture(*tinstHandle);
  sinstHandle.setOrigin(sf::Vector2f(hxo,hyo));
  return 1;
}

int eSlider::setSize(double wi, double he) {
  int atrList[8];
  w=wi;
  h=he;
  if (tinstBack!=NULL) {
    delete tinstBack;
    tinstBack=NULL;
  }
  ((eColor*)atrList)->r=bcolor.r;
  ((eColor*)atrList)->g=bcolor.g;
  ((eColor*)atrList)->b=bcolor.b;
  ((eColor*)atrList)->a=bcolor.a;
  tinstBack=engine->skin->getTexture(eObjectSliderB,atrList,w,engine->scale,&bxo,&byo,&bfw,&bfh);
  sinstBack.setTexture(*tinstBack);
  sinstBack.setOrigin(sf::Vector2f(bxo,byo));
  return 1;
}

int eSlider::setTrack(double& tval) {
  val=&tval;
  return 1;
}

int eSlider::setRange(double tmin, double tmax) {
  min=tmin;
  max=tmax;
  return 1;
}

int eSlider::event(eEvent& ev) {
  double rpos;
  switch (ev.type) {
    case eEventMouseButton:
      if (ev.state==1) {
        engine->grabMouse(true);
        _wantsAllEvents=true;
        rpos=w*((*val-min)/(max-min));
        if (ev.coord.x>(x+rpos-hrad) && ev.coord.x<(x+rpos+hrad) &&
            ev.coord.y>(y) && ev.coord.y<(y+h)) {
          active=true;
          if (holdCallback!=NULL) {
            holdCallback();
          }
        }
      } else {
        engine->grabMouse(false);
        _wantsAllEvents=false;
        if (active) {
          active=false;
          if (releaseCallback!=NULL) {
            releaseCallback();
          }
        }
      }
      break;
    case eEventMouseMove:
      if (active) {
        *val=min+(max-min)*((ev.coord.x-x)/w);
        if (*val<min) {
          *val=min;
        }
        if (*val>max) {
          *val=max;
        }
        if (valueCallback!=NULL) {
          valueCallback();
        }
      }
      break;
  }
  return 1;
}

int eSlider::draw() {
  double rpos;
  sinstBack.setPosition(x*engine->scale,(y+h/2)*engine->scale);
  engine->win->draw(sinstBack);
  //engine->line(x,y+h/2,x+w,y+h/2);
  if (val!=NULL) {
    rpos=w*((*val-min)/(max-min));
    sinstHandle.setPosition((x+rpos-hrad)*engine->scale,(y)*engine->scale);
    engine->win->draw(sinstHandle);
    //engine->frect(,x+rpos+hrad,y+h/2+hrad);
  }
  return 1;
}
