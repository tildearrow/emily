#include "toolkit.h"

int eVideoOut::setView(float x, float y, float wi, float he) {
  sf::View sv;
  viewStack.push(current);
  sv=win->getDefaultView();
  sv.reset(sf::FloatRect(0,0,(wi)*engine->scale,(he)*engine->scale));
  sv.setViewport(sf::FloatRect(x/w,y/h,wi/w,he/h));
  win->setView(sv);
  current.x=x;
  current.y=y;
  current.w=wi;
  current.h=he;
  return 1;
}

int eVideoOut::resetView() {
  View v;
  sf::View sv;
  if (viewStack.empty()) return 0;
  v=viewStack.top();
  sv=win->getDefaultView();
  sv.reset(sf::FloatRect(0,0,(v.w)*engine->scale,(v.h)*engine->scale));
  sv.setViewport(sf::FloatRect(v.x/w,v.y/h,v.w/w,v.h/h));
  win->setView(sv);
  current=v;
  viewStack.pop();
  return 1;
}

int eVideoOut::getWidth() {
  return w;
}

int eVideoOut::getHeight() {
  return h;
}
