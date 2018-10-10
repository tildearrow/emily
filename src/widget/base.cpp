#include "toolkit.h"

int eWidget::init() {
  return 1;
}

int eWidget::calcBounds() {
  bTop=dY*parent->getHeight()+y-h*alignY;
  bBottom=bTop+h;
  bLeft=dX*parent->getWidth()+x-w*alignX;
  bRight=bLeft+w;
  return 1;
}

int eWidget::event(eEvent& ev) {
  eLogD("widget got event %d\n",ev.type);
  return 1;
}

int eWidget::setDispPos(double x, double y) {
  dX=x;
  dY=y;
  calcBounds();
  return 0;
}

int eWidget::setAlign(double x, double y) {
  alignX=x;
  alignY=y;
  calcBounds();
  return 0;
}

int eWidget::setSize(double wi, double he) {
  w=wi;
  h=he;
  calcBounds();
  return 0;
}

int eWidget::draw() {
  engine->drawColor({1,1,1,1});
  engine->line(x,y,x+w,y+h);
  return 0;
}
