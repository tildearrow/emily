#include <SDL2/SDL.h>
#include "toolkit.h"

void* sdlCreateWin(void** instance, const char* title, int x, int y, int w, int h, bool resizable);

int sdlNextEvent(eEvent& ev);

void sdlPreRender(void* instance);
void sdlPostRender(void* instance);
