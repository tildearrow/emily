#include "toolkit.h"

eMenuItem::eMenuItem(string tex, void (*call)()) {
  text=tex;
  icon=NULL;
  callback=call;
}

eMenuItem::~eMenuItem() {
  return;
}

int eContextMenu::addItem(eMenuItem item) {
  items.push_back(item);
}

int eContextMenu::draw() {
  printf("MENU %p DRAW\n",this);
  for (int i=0; i<items.size(); i++) {
    printf("%d: %s\n",i,items[i].text.c_str());
  }
  return 1;
}
