#include "toolkit.h"

int eFrameView::init() {
  /*
  highlight=0;
  clicked=false;
  linst=new sf::Text();
  linst->setFont(engine->defFont->inst);
  linst->setCharacterSize(12*engine->scale);
  tinst=NULL;
  leftClickCallback=NULL;
  setColor(engine->skin->getDefaultColor(eObjectButton));
  */
  setSize(64,64);
  return 1;
}

int eFrameView::setSize(double wi, double he) {
  w=wi;
  h=he;
  calcBounds();
  return 1;
}

int eFrameView::getWidth() {
  return w;
}

int eFrameView::getHeight() {
  return w;
}

int eFrameView::event(eEvent& ev) {
  eEvent wrapev;
  wrapev=ev;
  wrapev.coord.x-=bLeft;
  wrapev.coord.y-=bTop;
  if (frameStack.empty()) {
    return 0;
  }
  curFrame=frameStack.top();
  for (size_t i=0; i<curFrame->widgets.size(); i++) {
    curFrame->widgets[i]->_collision=wrapev.coord.x>curFrame->widgets[i]->x &&
                wrapev.coord.x<curFrame->widgets[i]->x+curFrame->widgets[i]->w &&
                wrapev.coord.y>curFrame->widgets[i]->y &&
                wrapev.coord.y<curFrame->widgets[i]->y+curFrame->widgets[i]->h;
    if ((wrapev.type==eEventMouseButton && curFrame->widgets[i]->_relPendingAny) ||
        (wrapev.type==eEventMouseMove && curFrame->widgets[i]->_highPending) ||
        curFrame->widgets[i]->_collision) {
      if (wrapev.type==eEventMouseButton) {
        if (curFrame->widgets[i]->_collision && ev.state==1) {
          curFrame->widgets[i]->_relPending[ev.input]=true;
          curFrame->widgets[i]->evalPending();
        } else {
          curFrame->widgets[i]->_relPending[ev.input]=false;
          curFrame->widgets[i]->evalPending();
        }
      } else {
        if (curFrame->widgets[i]->_collision) {
          curFrame->widgets[i]->_highPending=true;
        } else {
          curFrame->widgets[i]->_highPending=false;
        }
      }
      curFrame->widgets[i]->event(wrapev);
    }
  }
  return 1;
}

int eFrameView::setView(float x, float y, float wi, float he) {
  return parent->parent->setView(bLeft+x,bTop+y,wi,he);
}

int eFrameView::resetView() {
  return parent->parent->resetView();
}

int eFrameView::draw() {
  int accum;
  accum=0;
  if (frameStack.empty()) {
    return 0;
  }
  curFrame=frameStack.top();
  engine->rect(bLeft,bTop,bLeft+w,bTop+h);
  //view=engine->win->getDefaultView();
  view.reset(sf::FloatRect(0,0,(w)*engine->scale,(h)*engine->scale));
  //view.setViewport(sf::FloatRect((bLeft/parent->parent->getWidth()),(bTop/parent->parent->getHeight()),(w/parent->parent->getWidth()),(h/parent->parent->getHeight())));
  parent->parent->setView(bLeft,bTop,w,h);
  //engine->win->setView(view);
  for (size_t i=0; i<curFrame->widgets.size(); i++) {
    accum+=curFrame->widgets[i]->draw();
  }
  parent->parent->resetView();
  //engine->win->setView(sf::View(sf::FloatRect(0,0,parent->parent->getWidth()*engine->scale,parent->parent->getHeight()*engine->scale)));
  return accum;
}
