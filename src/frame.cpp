#include "toolkit.h"

int eFrame::getWidth() {
  if (parent) {
    return parent->getWidth();
  }
  return -1;
}

int eFrame::getHeight() {
  if (parent) {
    return parent->getHeight();
  }
  return -1;
}
