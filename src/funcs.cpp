#include "toolkit.h"

eEngine::eEngine(double w, double h) {
  scale=2;
  visible=false;
  title="Application";
  estWaitTime=13000;
  width=w;
  height=h;
  defFont=new eFont;
  if (defFont->loaddef(eFontDefault)==0) {
    eLogE("Error while loading default font.\n");
  }
}

int eEngine::nextEvent(eEvent& ev) {
  sf::Event temp;
  if (win->pollEvent(temp)==0) {
    return 0;
  }
  switch (temp.type) {
    case sf::Event::Closed:
      ev.type=eEventQuit;
      break;
    case sf::Event::MouseMoved:
      ev.type=eEventMouseMove;
      ev.coord.x=temp.mouseMove.x/scale;
      ev.coord.y=temp.mouseMove.y/scale;
      break;
    case sf::Event::MouseButtonPressed:
      ev.type=eEventMouseButton;
      ev.coord.x=temp.mouseButton.x/scale;
      ev.coord.y=temp.mouseButton.y/scale;
      ev.input=temp.mouseButton.button;
      ev.state=1;
      break;
    case sf::Event::MouseButtonReleased:
      ev.type=eEventMouseButton;
      ev.coord.x=temp.mouseButton.x/scale;
      ev.coord.y=temp.mouseButton.y/scale;
      ev.input=temp.mouseButton.button;
      ev.state=0;
      break;
    default:
      ev.type=eEventBackend;
      break;
  }
}

void eMainLoop(eEngine* eng) {
  eEvent ev;
  eFrame* curFrame;
  while (1) {
    eng->preRender();
    eng->pause(eng->estWaitTime);
    /* event processing */
    while (eng->nextEvent(ev)) {
      if (eng->preEvCallback[ev.type]!=NULL) {
        eng->preEvCallback[ev.type](&ev);
      }
      switch (ev.type) {
        case eEventQuit:
          return;
          break;
        default:
          eLogD("got event %d\n",ev.type);
          break;
      }
      if (eng->postEvCallback[ev.type]!=NULL) {
        eng->postEvCallback[ev.type](&ev);
      }
    }
    if (eng->preDrawCallback!=NULL) {
      eng->preDrawCallback();
    }
    if (eng->drawStartCallback!=NULL) {
      eng->drawStartCallback();
    }
    if (eng->frameStack.size()) {
      curFrame=eng->frameStack.top();
      for (int i=0; i<curFrame->widgets.size(); i++) {
        curFrame->widgets[i]->draw();
      }
    }
    eng->drawColor(255,255,255,255);
    eng->line(0,0,eng->width,eng->height);
    if (eng->drawEndCallback!=NULL) {
      eng->drawEndCallback();
    }
    eng->postRender();
    if (eng->postDrawCallback!=NULL) {
      eng->postDrawCallback();
    }
  }
}

int eEngine::run() {
  eMainLoop(this);
  return 0;
}

/* to be re-enabled
int eEngine::runDetached() {
  std::thread runThread(eMainLoop,this);
  return 0;
}
*/

int eEngine::setPreEventCallback(unsigned char event, void ((*callback)(const eEvent *))) {
  preEvCallback[event]=callback;
}

int eEngine::setPostEventCallback(unsigned char event, void ((*callback)(const eEvent *))) {
  postEvCallback[event]=callback;
}

int eEngine::setPreDrawCallback(void ((*callback)())) {
  preDrawCallback=callback;
}

int eEngine::setDrawStartCallback(void ((*callback)())) {
  drawStartCallback=callback;
}

int eEngine::setDrawEndCallback(void ((*callback)())) {
  drawEndCallback=callback;
}

int eEngine::setPostDrawCallback(void ((*callback)())) {
  postDrawCallback=callback;
}

eFrame* eEngine::newFrame() {
  eFrame* ret;
  ret=new eFrame;
  ret->engine=this;
  return ret;
}

int eEngine::pushFrame(eFrame* f) {
  frameStack.push(f);
}

int eEngine::popFrame() {
  frameStack.pop();
}

void eEngine::drawColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
  /*
  eDrawColor(backInst,r,g,b,a);
  */
}

void eEngine::line(double x1, double y1, double x2, double y2) {
  sf::VertexArray temp(sf::Lines,2);
  temp[0].position=sf::Vector2f(x1*scale,y1*scale);
  temp[1].position=sf::Vector2f(x2*scale,y2*scale);
  win->draw(temp);
}

int eEngine::show() {
  if (!visible) {
    win=new sf::RenderWindow(sf::VideoMode(width*scale,height*scale),title,sf::Style::Titlebar|sf::Style::Close);
    win->setVerticalSyncEnabled(true);
    visible=true;
  }
}

eTexture* eEngine::getUnmanagedTexture(int width, int height, int type) {
  return NULL;
  /*
  eTexture* ret;
  void* tex;
  tex=eCreateTexture(backInst,width,height,type);
  printf("RETURN VAL %x\n",tex);
  if (tex==NULL) {
    return NULL;
  }
  ret=new eTexture;
  ret->actual=tex;
  ret->width=width;
  ret->height=height;
  ret->type=type;
  return ret;
  */
}

eTexture* eEngine::getTexture(int width, int height, int type, int prop0, int prop1, int prop2, int prop3) {
  return NULL;
  /*
  eTexture* ret;
  void* tex;
  tex=eCreateTexture(backInst,width,height,type);
  if (tex==NULL) {
    return NULL;
  }
  ret=new eTexture;
  ret->actual=tex;
  ret->width=width;
  ret->height=height;
  ret->type=type;
  ret->id[0]=prop0;
  ret->id[1]=prop1;
  ret->id[2]=prop2;
  ret->id[3]=prop3;
  return ret;
  */
}

eTexture* eEngine::getTextureFromBitmap(eBitmap* bitmap, int type) {
  return NULL;
  /*
  eTexture* ret;
  void* tex;
  tex=eCreateTexture(backInst,bitmap->width,bitmap->height,type);
  eUpdateTexture(tex,bitmap->data,bitmap->pitch());
  printf("RETURN VAL %x\n",tex);
  if (tex==NULL) {
    return NULL;
  }
  ret=new eTexture;
  ret->actual=tex;
  ret->width=width;
  ret->height=height;
  ret->type=type;
  return ret;
  */
}

int eEngine::drawTexture(eTexture* tex, double x, double y) {
  return 0;
  /*
  return eDrawTexture(backInst,tex,x,y);
  */
}

int eEngine::pause(double timeAsMicro) {
  sf::sleep(sf::microseconds(timeAsMicro));
}

void eEngine::preRender() {
  /*
  struct timespec start, end;
  clock_gettime(CLOCK_MONOTONIC,&start);
  */
  win->clear();
  /*
  clock_gettime(CLOCK_MONOTONIC,&end);
  printf("time: %ld\n",(start.tv_nsec-end.tv_nsec)/1000);
  */
}

void eEngine::postRender() {
  win->display();
}
