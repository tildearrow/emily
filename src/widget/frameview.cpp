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

int eFrameView::pushFrame(eFrame* f) {
  frameStack.push(f);
  return 1;
}

int eFrameView::popFrame() {
  frameStack.pop();
  return 1;
}

int eFrameView::setSize(double wi, double he) {
  w=wi;
  h=he;
  calcBounds();
  return 1;
}

int eFrameView::event(eEvent& ev) {
  eEvent wrapev;
  wrapev=ev;
  wrapev.coord.x-=x;
  wrapev.coord.y-=y;
  curFrame=frameStack.top();
  for (size_t i=0; i<curFrame->widgets.size(); i++) {
    curFrame->widgets[i]->_collision=wrapev.coord.x>curFrame->widgets[i]->x &&
                wrapev.coord.x<curFrame->widgets[i]->x+curFrame->widgets[i]->w &&
                wrapev.coord.y>curFrame->widgets[i]->y &&
                wrapev.coord.y<curFrame->widgets[i]->y+curFrame->widgets[i]->h;
    if ((wrapev.type==eEventMouseButton && curFrame->widgets[i]->_relPending) ||
        (wrapev.type==eEventMouseMove && curFrame->widgets[i]->_highPending) ||
        curFrame->widgets[i]->_collision) {
      if (wrapev.type==eEventMouseButton) {
        if (curFrame->widgets[i]->_collision && ev.state==1) {
          curFrame->widgets[i]->_relPending=true;
        } else {
          curFrame->widgets[i]->_relPending=false;
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

int eFrameView::draw() {
  int accum;
  accum=0;
  curFrame=frameStack.top();
  engine->rect(x,y,x+w,y+h);
  //view=engine->win->getDefaultView();
  view.reset(sf::FloatRect(0,0,(w)*engine->scale,(h)*engine->scale));
  // TO BE FIXED!
  view.setViewport(sf::FloatRect((x/parent->parentD->getWidth()),(y/parent->parentD->getHeight()),(w/parent->parentD->getWidth()),(h/parent->parentD->getHeight())));
  engine->win->setView(view);
  for (size_t i=0; i<curFrame->widgets.size(); i++) {
    accum+=curFrame->widgets[i]->draw();
  }
  engine->win->setView(sf::View(sf::FloatRect(0,0,parent->parentD->getWidth()*engine->scale,parent->parentD->getHeight()*engine->scale)));
  return accum;
}
