#include "toolkit.h"

eMenuItem::eMenuItem(string tex, void (*call)()) {
  text=tex;
  icon=NULL;
  iconIndex=0;
  callback=call;
  dtext=NULL;
}

eMenuItem::~eMenuItem() {
  delete icon;
  return;
}

int eContextMenu::itemCount() {
  return items.size();
}

int eContextMenu::event(eEvent& ev) {
  switch (ev.type) {
    case eEventMouseButton:
      if (ev.coord.x>x && ev.coord.x<x+w &&
          ev.coord.y>y && ev.coord.y<y+h) {
        if (ev.state==0) {
          if (selected!=-1) {
            if (items[selected].callback!=NULL) {
              items[selected].callback();
            }
          }
          wannaRetire=true;
        }
      } else {
        wannaRetire=true;
      }
      break;
    case eEventMouseMove:
      if (ev.coord.x>x && ev.coord.x<x+w &&
          ev.coord.y>y && ev.coord.y<y+h) {
        selected=(ev.coord.y-y-2)/18;
        if (selected>=items.size()) {
          selected=-1;
        }
      } else {
        selected=-1;
      }
      break;
  }
}

int eContextMenu::addItem(eMenuItem item) {
  items.push_back(item);
  w=128;
  h=items.size()*18+4;
}

int eContextMenu::draw() {
  engine->drawColor({0,0,0,1});
  engine->frect(x,y,x+w,y+h);
  engine->drawColor({0.5,0.5,0.5,1});
  engine->rect(x,y,x+w,y+h);
  if (selected!=-1) {
    engine->drawColor({1,1,1,0.25});
    engine->frect(x+1,(1.5+y+(selected*18)),x+w-1.5,(1.5+y+(1+selected)*18));
  }
  engine->drawColor({1,1,1,1});
  for (int i=0; i<items.size(); i++) {
    if (items[i].dtext==NULL) {
      items[i].dtext=new sf::Text(items[i].text,engine->defFont->inst,12*engine->scale);
    }
    items[i].dtext->setPosition((4+x)*engine->scale,(3+y+i*18)*engine->scale);
    engine->win->draw(*items[i].dtext);
  }
  return 1;
}
