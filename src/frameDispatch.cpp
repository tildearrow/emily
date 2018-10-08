#include "toolkit.h"

int eFrameDispatch::pushFrame(eFrame* f) {
  frameStack.push(f);
  f->parent=this;
  for (size_t i=0; i<f->widgets.size(); i++) {
    f->widgets[i]->calcBounds();
  }
  return 1;
}

int eFrameDispatch::popFrame() {
  if (frameStack.empty()) return 0;
  
  frameStack.top()->parent=NULL;
  frameStack.pop();
  return 1;
}

int eFrameDispatch::setView(float x, float y, float w, float h) {
  return 0;
}

int eFrameDispatch::resetView() {
  return 0;
}

int eFrameDispatch::getWidth() {
  return -1;
}

int eFrameDispatch::getHeight() {
  return -1;
}
