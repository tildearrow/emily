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

int eLabel::setTextSize(double size) {
  inst->setCharacterSize(size*engine->scale);
}

int eLabel::draw() {
  engine->drawColor({1,1,1,1});
  inst->setPosition(x*engine->scale,y*engine->scale);
  engine->win->draw(*inst);
  return 0;
}
