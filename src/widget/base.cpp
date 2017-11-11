#include "toolkit.h"

int eWidget::init() {
  return 1;
}

int eWidget::event(eEvent& ev) {
  printf("widget got event %d\n",ev.type);
  return 1;
}

int eWidget::setSize(double wi, double he) {
  w=wi;
  h=he;
}

int eWidget::draw() {
  engine->drawColor({1,1,1,1});
  engine->line(x,y,x+w,y+h);
  return 0;
}
