#include "toolkit.h"

int eLabel::setString(string data) {
  text=data;
  return 0;
}

int eLabel::setSize(double wi, double he) {
  w=wi;
  h=he;
}

int eLabel::draw() {
  engine->drawColor(255,255,255,255);
  engine->line(x,y,x+w,y+h);
  return 0;
}
