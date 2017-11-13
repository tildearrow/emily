#include "toolkit.h"

int eButton::init() {
  highlight=0;
  clicked=false;
  linst=new sf::Text();
  linst->setFont(engine->defFont->inst);
  linst->setCharacterSize(12*engine->scale);
  tinst=NULL;
  setSize(12,32);
  return 1;
}

int eButton::setSize(double wi, double he) {
  double xo, yo;
  int atrList[8];
  w=wi;
  h=he;
  if (tinst!=NULL) {
    printf("DELETING!\n");
    delete tinst;
    tinst=NULL;
  }
  tinst=engine->skin->getTexture(0,atrList,w,h,&xo,&yo);
  printf("%d %d\n",tinst->getSize().x,tinst->getSize().y);
  sinst.setTexture(*tinst,true);
  sinst.setOrigin(sf::Vector2f(xo,yo));
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
  /*
  if (clicked && _collision) {
    engine->drawColor({1,1,1,1});
  } else {
    engine->drawColor({0.25f*highlight,0.25f*highlight,0.25f*highlight,1});
  }
  engine->frect(x,y,x+w,y+h);
  engine->drawColor({1,1,1,1});
  engine->rect(x,y,x+w,y+h);
  */
  linst->setPosition((int)(((x+w/2)*engine->scale)-(linst->getLocalBounds().width/2)),
                     (int)((y+h/2)*engine->scale-(linst->getLocalBounds().height*0.833333)));
  sinst.setPosition(x*engine->scale,y*engine->scale);
  engine->win->draw(sinst);
  engine->win->draw(*linst);
  return 0;
}
