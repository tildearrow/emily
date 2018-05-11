#include "toolkit.h"

int eDisplay::pushFrame(eFrame* f) {
  frameStack.push(f);
  f->parentD=this;
  return 1;
}

int eDisplay::popFrame() {
  frameStack.top()->parentD=NULL;
  frameStack.pop();
  return 1;
}
