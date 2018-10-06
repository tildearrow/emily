#include "toolkit.h"

int eLabel::init() {
  inst=new sf::Text();
  inst->setFont(engine->defFont->inst);
  tsize=12;
  inst->setCharacterSize(tsize*engine->scale);
  return 1;
}

int eLabel::calcBounds() {
  bTop=dY*parent->getHeight()+y-tsize*alignY;
  bBottom=bTop+tsize;
  bLeft=dX*parent->getWidth()+x-(inst->getLocalBounds().width/engine->scale)*alignX;
  bRight=bLeft+(inst->getLocalBounds().width/engine->scale);
  return 1;
}

int eLabel::setString(string data) {
  text=data;
  inst->setString(data);
  return 0;
}

int eLabel::setTextSize(double size) {
  tsize=size;
  inst->setCharacterSize(tsize*engine->scale);
  return 0;
}

int eLabel::draw() {
  calcBounds();
  engine->drawColor({1,1,1,1});
  // int'ing prevents text from looking blurry on 1x scale
  inst->setPosition(int(bLeft*engine->scale),int(bTop*engine->scale));
  engine->win->draw(*inst);
  return 0;
}
