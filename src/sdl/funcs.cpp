#include "sdl.h"

void* sdlCreateWin(void** instance, const char* title, int x, int y, int w, int h, bool resizable) {
  SDL_Window* ret;
  ret=SDL_CreateWindow(title,x,y,w,h,0);
  if (ret==NULL) {
    return NULL;
  }
  *instance=SDL_CreateRenderer(ret,-1,SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
  if (*instance==NULL) {
    return NULL;
  }
  return ret;
}

int sdlNextEvent(eEvent& ev) {
  SDL_Event temp;
  if (SDL_PollEvent(&temp)==0) {
    return 0;
  }
  switch (temp.type) {
    case SDL_QUIT:
      ev.type=eEventQuit;
      break;
    case SDL_MOUSEMOTION:
      ev.type=eEventMouseMove;
      ev.coord.x=temp.motion.x;
      ev.coord.y=temp.motion.y;
      break;
    case SDL_MOUSEBUTTONDOWN:
      ev.type=eEventMouseButton;
      ev.coord.x=temp.motion.x;
      ev.coord.y=temp.motion.y;
      ev.input=temp.button.button;
      ev.state=1;
      break;
    case SDL_MOUSEBUTTONUP:
      ev.type=eEventMouseButton;
      ev.coord.x=temp.motion.x;
      ev.coord.y=temp.motion.y;
      ev.input=temp.button.button;
      ev.state=0;
      break;
    default:
      ev.type=eEventBackend;
      break;
  }
}

void sdlWait(int time) {
  SDL_Delay(time);
}

void sdlPreRender(void* instance) {
  int start, end;
  start=SDL_GetPerformanceCounter();
  SDL_SetRenderDrawColor((SDL_Renderer*)instance,0,0,0,255);
  SDL_RenderClear((SDL_Renderer*)instance);
  end=SDL_GetPerformanceCounter();
  eLogD("pre delta: %d\n",end-start);
}

void sdlPostRender(void* instance) {
  int start, end;
  start=SDL_GetPerformanceCounter();
  SDL_RenderPresent((SDL_Renderer*)instance);
  SDL_RenderClear((SDL_Renderer*)instance);
  end=SDL_GetPerformanceCounter();
  eLogD("post delta: %d\n",end-start);
}

void sdlDrawColor(void* instance, unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
  SDL_SetRenderDrawColor((SDL_Renderer*)instance,r,g,b,a);
}

void sdlLine(void* instance, double x1, double y1, double x2, double y2) {
  SDL_RenderDrawLine((SDL_Renderer*)instance,x1,y1,x2,y2);
}
