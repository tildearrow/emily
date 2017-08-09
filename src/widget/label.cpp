#include "toolkit.h"

int eLabel::init() {
  printf("THIS SHALL PRINT!\n");
  tex=engine->getUnmanagedTexture(32,32,eStream);
  printf("%x\n",tex);
}

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
  src.x=0;
  src.y=0;
  src.w=32;
  src.h=32;
  dest=src;
  dest.x=x;
  dest.y=y;
  engine->drawTexture(tex,src,dest);
  return 0;
}
