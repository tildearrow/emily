#include "toolkit.h"

int eLabel::init() {
  inst=new sf::Text();
  inst->setFont(engine->defFont->inst);
  inst->setCharacterSize(12*engine->scale);
}

int eLabel::setString(string data) {
  text=data;
  inst->setString(data);
  return 0;
}

int eLabel::setSize(double size) {
  inst->setCharacterSize(size*engine->scale);
}

int eLabel::draw() {
  eRect src, dest;
  engine->drawColor(255,255,255,255);
  src.x=0;
  src.y=0;
  src.w=w;
  src.h=h;
  dest=src;
  dest.x=x;
  dest.y=y;
  engine->win->draw(*inst);
  return 0;
}
