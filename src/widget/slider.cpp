#include "toolkit.h"

int eSlider::init() {
  highlight=0;
  clicked=false;
  active=false;
  val=NULL;
  holdCallback=NULL;
  releaseCallback=NULL;
  valueCallback=NULL;
  setColor(engine->skin->getDefaultColor(eObjectButton));
  setSize(128,24);
  setHandleSize(6);
  setRange(0,1);
  return 1;
}

int eSlider::setColor(eColor col) {
  color=col;
  return 1;
}

int eSlider::setHandleSize(double rad) {
  hrad=rad;
  return 1;
}

int eSlider::setSize(double wi, double he) {
  w=wi;
  h=he;
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
            ev.coord.y>(y+h/2-hrad) && ev.coord.y<(y+h/2+hrad)) {
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
  if (val!=NULL) {
    rpos=w*((*val-min)/(max-min));
    engine->frect(x+rpos-hrad,y+h/2-hrad,x+rpos+hrad,y+h/2+hrad);
  }
  engine->line(x,y+h/2,x+w,y+h/2);
  return 1;
}
