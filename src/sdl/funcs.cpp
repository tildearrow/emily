#include "sdl.h"

int sdlPoint(eWindow* _this, double x, double y) {
  return SDL_RenderDrawPoint((SDL_Renderer*)_this->backInst,x,y);
}
