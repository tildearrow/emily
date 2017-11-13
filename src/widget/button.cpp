#include "toolkit.h"

int eButton::init() {
  highlight=0;
  clicked=false;
  linst=new sf::Text();
  linst->setFont(engine->defFont->inst);
  linst->setCharacterSize(12*engine->scale);
  setSize(64,32);
  return 1;
}

int eButton::setSize(double wi, double he) {
  w=wi;
  h=he;
  return 1;
}

int eButton::setLabel(string data) {
  label=data;
  linst->setString(data);
  return 1;
}

int eButton::setColor(eColor col) {
  color=col;
  return 1;
}

int eButton::event(eEvent& ev) {
  if (ev.type==eEventMouseButton) {
    if (ev.state==1) {
      clicked=true;
    } else {
      if (clicked) {
        if (_collision) {
          if (leftClickCallback!=NULL) {
            leftClickCallback();
          }
        }
        clicked=false;
      }
    }
  }
  return 1;
}

int eButton::setCallback(void (*callback)()) {
  leftClickCallback=callback;
  return 1;
}

int eButton::draw() {
  if (_collision) {
    highlight+=0.125;
  } else {
    highlight-=0.0625;
  }
  if (highlight>1) highlight=1;
  if (highlight<0) highlight=0;
  // temporary
  if (clicked && _collision) {
    engine->drawColor({1,1,1,1});
  } else {
    engine->drawColor({0.25f*highlight,0.25f*highlight,0.25f*highlight,1});
  }
  engine->frect(x,y,x+w,y+h);
  engine->drawColor({1,1,1,1});
  engine->rect(x,y,x+w,y+h);
  linst->setPosition((int)(((x+w/2)*engine->scale)-(linst->getLocalBounds().width/2)),
                     (int)((y+h/2)*engine->scale-(linst->getLocalBounds().height*0.833333)));
  engine->win->draw(*linst);
  return 0;
}
