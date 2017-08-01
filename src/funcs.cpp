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
    eng->ePreRender(eng->backInst);
    eng->ePostRender(eng->backInst);
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

int eEngine::show() {
  if (!visible) {
    backWin=createWin(&backInst,title.c_str(),0,0,width,height,false);
    visible=true;
  }
}
