#include "toolkit.h"

int eWidget::setSize(double wi, double he) {
  w=wi;
  h=he;
}

int eWidget::draw() {
  engine->drawColor(255,255,255,255);
  engine->line(x,y,x+w,y+h);
  return 0;
}
