#include "toolkit.h"

eIcon::~eIcon() {
  delete iconT;
}

double eIcon::width() {
  return charW;
}

double eIcon::height() {
  return charH;
}

int eIcon::setPos(double x, double y) {
  iconS.setPosition(x,y);
}

int eIcon::draw() {
  engine->win->draw(iconS);
}
