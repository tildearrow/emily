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
  eRect src, dest;
  engine->drawColor(255,255,255,255);
  engine->drawTexture(tex,src,dest);
  return 0;
}
