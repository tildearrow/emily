#include "toolkit.h"

#ifdef HAVE_SDL2
#include "sdl/sdl.h"
#endif

#include "dummy/dummy.h"

eEngine::eEngine(int backend) {
  switch (backend) {
#ifdef HAVE_SDL2
    case eBackSDL2:
      createWin=sdlCreateWin;
      ePreRender=sdlPreRender;
      ePostRender=sdlPostRender;
      eNextEvent=sdlNextEvent;
      break;
#endif
    default:
      eLogE("Backend does not exist, or not available. Loading dummy backend.\n");
      createWin=dummyCreateWin;
      break;
  }
  visible=false;
  title="Application";
  width=1280;
  height=800;
}

void eMainLoop(eEngine* eng) {
  eEvent ev;
  while (1) {
    /* event processing */
    while (eng->eNextEvent(ev)) {
      switch (ev.type) {
        case eEventQuit:
          return;
          break;
        default:
          eLogD("got event %d\n",ev.type);
          break;
      }
    }
    eng->ePreRender(eng->backInst);
    eng->ePostRender(eng->backInst);
  }
}

int eEngine::run() {
  std::thread runThread(eMainLoop,this);
  runThread.join();
  return 0;
}

int eEngine::runDetached() {
  std::thread runThread(eMainLoop,this);
  return 0;
}

int eEngine::show() {
  if (!visible) {
    backWin=createWin(&backInst,title.c_str(),0,0,width,height,false);
    visible=true;
  }
}
