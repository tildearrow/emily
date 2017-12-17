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
  printf("CM event\n");
}

int eContextMenu::addItem(eMenuItem item) {
  items.push_back(item);
  w=128;
  h=items.size()*18+4;
}

int eContextMenu::draw() {
  engine->drawColor({0,0,0,1});
  engine->frect(x,y,x+w,y+h);
  engine->drawColor({1,1,1,1});
  engine->rect(x,y,x+w,y+h);
  for (int i=0; i<items.size(); i++) {
    if (items[i].dtext==NULL) {
      items[i].dtext=new sf::Text(items[i].text,engine->defFont->inst,12*engine->scale);
    }
    items[i].dtext->setPosition((4+x)*engine->scale,(3+y+i*18)*engine->scale);
    engine->win->draw(*items[i].dtext);
  }
  return 1;
}
