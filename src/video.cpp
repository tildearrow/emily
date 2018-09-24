#include "toolkit.h"

int eVideoOut::getWidth() {
  return w;
}

int eVideoOut::getHeight() {
  return h;
}

int eVideoOut::pushFrame(eFrame* f) {
  frameStack.push(f);
  f->parentD=this;
  for (size_t i=0; i<f->widgets.size(); i++) {
    f->widgets[i]->calcBounds();
  }
  return 1;
}

int eVideoOut::popFrame() {
  frameStack.top()->parentD=NULL;
  frameStack.pop();
  return 1;
}
