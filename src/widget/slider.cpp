#include "toolkit.h"

int eSlider::init() {
  highlight=0;
  clicked=false;
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
  printf("event\n");
  if (ev.type==eEventMouseButton) {
    // to be done
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
