#include "toolkit.h"

eIcon::~eIcon() {
  if (isImage) {
    delete iconTex;
  } else {
    delete iconText;
  }
}

int eIcon::setPos(double x, double y) {
  if (isImage) {
    // TODO
  } else {
    iconText->setPosition(x*engine->scale,y*engine->scale);
  }
}

int eIcon::draw() {
  if (isImage) {
    // TODO
  } else {
    engine->win->draw(*iconText);
  }
}
