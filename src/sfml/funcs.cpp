#include "sfml.h"

void* sfmlCreateWin(void** instance, const char* title, int x, int y, int w, int h, bool resizable) {
  sf::RenderWindow* ret;
  ret=new sf::RenderWindow(sf::VideoMode(w,h),title,sf::Style::Titlebar|sf::Style::Close);
  if (ret==NULL) {
    return NULL;
  }
  *instance=ret;
  return ret;
}

int sfmlNextEvent(void* instance, eEvent& ev) {
  sf::Event temp;
  if (((sf::RenderWindow*)instance)->pollEvent(temp)==0) {
    return 0;
  }
  switch (temp.type) {
    case sf::Event::Closed:
      ev.type=eEventQuit;
      break;
    case sf::Event::MouseMoved:
      ev.type=eEventMouseMove;
      ev.coord.x=temp.mouseMove.x;
      ev.coord.y=temp.mouseMove.y;
      break;
    case sf::Event::MouseButtonPressed:
      ev.type=eEventMouseButton;
      ev.coord.x=temp.mouseButton.x;
      ev.coord.y=temp.mouseButton.y;
      ev.input=temp.mouseButton.button;
      ev.state=1;
      break;
    case sf::Event::MouseButtonReleased:
      ev.type=eEventMouseButton;
      ev.coord.x=temp.mouseButton.x;
      ev.coord.y=temp.mouseButton.y;
      ev.input=temp.mouseButton.button;
      ev.state=0;
      break;
    default:
      ev.type=eEventBackend;
      break;
  }
}

void sfmlWait(int time) {
  sf::sleep(sf::milliseconds(time));
}

void sfmlPreRender(void* instance) {
  ((sf::RenderWindow*)instance)->clear();
  //eLogD("pre delta: %d\n",end-start);
}

void sfmlPostRender(void* instance) {
  ((sf::RenderWindow*)instance)->display();
  //eLogD("post delta: %d\n",end-start);
}

void sfmlDrawColor(void* instance, unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
  //SDL_SetRenderDrawColor((SDL_Renderer*)instance,r,g,b,a);
}

void sfmlLine(void* instance, double x1, double y1, double x2, double y2) {
  sf::VertexArray temp(sf::Lines,2);
  temp[0].position=sf::Vector2f(x1,y1);
  temp[1].position=sf::Vector2f(x2,y2);
  ((sf::RenderWindow*)instance)->draw(temp);
  //SDL_RenderDrawLine((SDL_Renderer*)instance,x1,y1,x2,y2);
}

void* sfmlCreateTexture(void* instance, int width, int height, int type) {
  sf::Texture* ret;
  ret=new sf::Texture();
  if (ret->create(width,height)) {
    return ret;
  }
  delete ret;
  return NULL;
  //return (void*)SDL_CreateTexture((SDL_Renderer*)instance,SDL_PIXELFORMAT_ARGB8888,type,width,height);
};

int sfmlDrawTexture(void* instance, eTexture* tex, double x, double y) {
  /*
  SDL_Rect convSR, convDR;
  convSR.x=sr.x;
  convSR.y=sr.y;
  convSR.w=sr.w;
  convSR.h=sr.h;
  convDR.x=dr.x;
  convDR.y=dr.y;
  convDR.w=dr.w;
  convDR.h=dr.h;
  return SDL_RenderCopy((SDL_Renderer*)instance,(SDL_Texture*)tex,&convSR,&convDR);
  */
}

int sfmlUpdateTexture(void* tex, void* data, int pitch) {
  /*
  return SDL_UpdateTexture((SDL_Texture*)tex,NULL,data,pitch);
  */
}
