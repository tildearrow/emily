#include "toolkit.h"

eIcon::~eIcon() {
  if (isImage) {
    delete iconTex;
  } else {
    delete iconText;
  }
}

double eIcon::width() {
  return iconText->getLocalBounds().width;
}

double eIcon::height() {
  return iconText->getLocalBounds().height;
}

int eIcon::setPos(double x, double y) {
  if (isImage) {
    // TODO
  } else {
    iconText->setPosition(x,y);
  }
}

int eIcon::draw() {
  if (isImage) {
    // TODO
  } else {
    engine->win->draw(*iconText);
  }
}
