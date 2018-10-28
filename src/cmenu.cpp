#include "toolkit.h"

void eMenuItem::runCallback() {
  if (callback!=NULL) callback(this,callbackUser);
}

eMenuItem::eMenuItem(string tex, void (*call)(eMenuItem* item, void* user), void* userData) {
  text=tex;
  icon=NULL;
  iconIndex=0;
  callback=call;
  callbackUser=userData;
  dtext=NULL;
}

eMenuItem::~eMenuItem() {
  delete icon;
  return;
}

size_t eContextItems::itemCount() {
  return items.size();
}

eContextItems* eContextItems::add(eMenuItem item) {
  items.push_back(item);
  return this;
}

eMenuItem& eContextItems::getItem(size_t index) {
  return items[index];
}

size_t eContextMenu::itemCount() {
  return items.itemCount();
}

int eContextMenu::event(eEvent& ev) {
  switch (ev.type) {
    case eEventMouseButton:
      if (ev.coord.x>x && ev.coord.x<x+w &&
          ev.coord.y>y && ev.coord.y<y+h) {
        if (ev.state==0) {
          if (selected!=-1) {
            items.getItem(selected).runCallback();
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
        if (selected>=(int)items.itemCount()) {
          selected=-1;
        }
      } else {
        selected=-1;
      }
      break;
  }
  return 1;
}

int eContextMenu::addItem(eMenuItem item) {
  items.add(item);
  w=128;
  h=items.itemCount()*18+4;
  return 1;
  /*
  items.push_back(item);
  w=128;
  h=items.size()*18+4;
  return 1;
  */
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
  for (size_t i=0; i<items.itemCount(); i++) {
    if (items.getItem(i).dtext==NULL) {
      items.getItem(i).dtext=new sf::Text(items.getItem(i).text,engine->defFont->inst,9);
    }
    items.getItem(i).dtext->setPosition((4+x)*engine->scale,(6+y+(int)i*18)*engine->scale);
    engine->win->draw(*items.getItem(i).dtext);
  }
  return 1;
}
