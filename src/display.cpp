#include "toolkit.h"

int eDisplay::getWidth() {
  return w;
}

int eDisplay::getHeight() {
  return h;
}

int eDisplay::pushFrame(eFrame* f) {
  frameStack.push(f);
  f->parentD=this;
  printf("frame push\n");
  for (int i=0; i<f->widgets.size(); i++) {
    f->widgets[i]->calcBounds();
  }
  return 1;
}

int eDisplay::popFrame() {
  frameStack.top()->parentD=NULL;
  frameStack.pop();
  return 1;
}
