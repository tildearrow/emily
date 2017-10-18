#include <SFML/Graphics.hpp>
#include "toolkit.h"

void* sfmlCreateWin(void** instance, const char* title, int x, int y, int w, int h, bool resizable);

int sfmlNextEvent(void* instance, eEvent& ev);

void sfmlWait(int time);
void sfmlPreRender(void* instance);
void sfmlPostRender(void* instance);

void sfmlDrawColor(void* instance, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
void sfmlLine(void* instance, double x1, double y1, double x2, double y2);

void* sfmlCreateTexture(void* instance, int width, int height, int type);
int sfmlDrawTexture(void* instance, eTexture* tex, double x, double y);
int sfmlUpdateTexture(void* tex, void* data, int pitch);
