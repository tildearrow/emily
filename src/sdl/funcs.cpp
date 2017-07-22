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
    default:
      ev.type=eEventBackend;
      break;
  }
}

void sdlPreRender(void* instance) {
  SDL_RenderClear((SDL_Renderer*)instance);
}

void sdlPostRender(void* instance) {
  SDL_RenderPresent((SDL_Renderer*)instance);
}
