#include "toolkit.h"

#ifdef HAVE_SDL2
#include "sdl/sdl.h"
#endif

#include "dummy/dummy.h"

eFont* eEngine::newFont() {
  eFont* ret;
  ret=new eFont(ftlib);
  return ret;
}

eEngine::eEngine(int backend) {
  switch (backend) {
#ifdef HAVE_SDL2
    case eBackSDL2:
      createWin=sdlCreateWin;
      ePreRender=sdlPreRender;
      ePostRender=sdlPostRender;
      eNextEvent=sdlNextEvent;
      eWait=sdlWait;
      eDrawColor=sdlDrawColor;
      eLine=sdlLine;
      eDrawTexture=sdlDrawTexture;
      break;
#endif
    default:
      eLogE("Backend does not exist, or not available. Loading dummy backend.\n");
      createWin=dummyCreateWin;
      break;
  }
  visible=false;
  title="Application";
  estWaitTime=10;
  width=1280;
  height=800;
  if (FT_Init_FreeType(&ftlib)) {
    eLogE("Error while initializing the font system.\n");
  } else {
    defFont=newFont();
    if (defFont->loaddef(eFontDefault)==0) {
      eLogE("Error while loading default font.\n");
    }
    defFont->size(12);
  }
}

void eMainLoop(eEngine* eng) {
  eEvent ev;
  eFrame* curFrame;
  while (1) {
    eng->eWait(eng->estWaitTime);
    /* event processing */
    while (eng->eNextEvent(ev)) {
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
    eng->ePreRender(eng->backInst);
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
    eng->ePostRender(eng->backInst);
    if (eng->postDrawCallback!=NULL) {
      eng->postDrawCallback();
    }
  }
}

int eEngine::run() {
  eMainLoop(this);
  return 0;
}

int eEngine::runDetached() {
  std::thread runThread(eMainLoop,this);
  return 0;
}

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
  eDrawColor(backInst,r,g,b,a);
}

void eEngine::line(double x1, double y1, double x2, double y2) {
  eLine(backInst,x1,y1,x2,y2);
}

int eEngine::show() {
  if (!visible) {
    backWin=createWin(&backInst,title.c_str(),0,0,width,height,false);
    visible=true;
  }
}

int eEngine::drawTexture(eTexture* tex, eRect& src, eRect& dest) {
  return eDrawTexture(backInst,tex->actual,src,dest);
}
