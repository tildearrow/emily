#include <SDL2/SDL.h>
#include "toolkit.h"

void* sdlCreateWin(void** instance, const char* title, int x, int y, int w, int h, bool resizable);

int sdlNextEvent(eEvent& ev);

void sdlWait(int time);
void sdlPreRender(void* instance);
void sdlPostRender(void* instance);

void sdlDrawColor(void* instance, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
void sdlLine(void* instance, double x1, double y1, double x2, double y2);
int sdlDrawTexture(void* instance, void* tex, eRect& sr, eRect& dr);
