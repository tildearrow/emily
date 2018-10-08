#define IS_LIBRARY
#include "toolkit.h"
#ifdef __APPLE__
#include <mach/mach_time.h>
extern "C" {
#include "nsstub.h"
}
#endif
#include "../fa/fa.h"

#define ENABLE_WAIT

double getScale() {
  char* env;
  // try with environment variable
  env=getenv("EMILY_SCALE");
  if (env!=NULL) {
    return atof(env);
  }
#if defined(__linux__)
  // linux (wayland) code here
#elif defined(_WIN32)
  // windows code here
  HDC disp;
  int dpi;
  disp=GetDC(NULL);
  if (disp==NULL) {
    return 1;
  }
  dpi=GetDeviceCaps(disp,LOGPIXELSX);
  ReleaseDC(NULL,disp);
  return (double)dpi/96.0;
#elif defined(__APPLE__)
  // macOS code here
  double dpi;
  if ((dpi=nsStubDPI())>0) {
    return dpi;
  }
#elif defined(__ANDROID__)
  // android code here
#endif
#if defined(__unix__)
  // X11
#ifdef USE_XCB
  xcb_connection_t* disp;
  xcb_screen_iterator_t scr;
  int dpi, defs;
  disp=xcb_connect(NULL,&defs);
  
  if (!xcb_connection_has_error(disp)) {
    scr=xcb_setup_roots_iterator(xcb_get_setup(disp));
    for (int i=0; i<defs; i++) {
      xcb_screen_next(&scr);
    }
    dpi=(int)(0.5+(25.4*(double)scr.data->width_in_pixels/(double)scr.data->width_in_millimeters));
    xcb_disconnect(disp);
    return (double)dpi/96.0;
  }
#else
  Display* disp;
  int dpi;
  disp=XOpenDisplay(NULL);
  if (disp!=NULL) {
    dpi=(int)(0.5+(25.4*(double)XDisplayWidth(disp,XDefaultScreen(disp))/(double)XDisplayWidthMM(disp,XDefaultScreen(disp))));
    XCloseDisplay(disp);
    return (double)dpi/96.0;
  }
#endif
#endif
  // assume 1
  return 1;
}

eEngine::eEngine(string name) {
  int status;
#if defined(__unix__) && !defined(__APPLE__)
  x11conn=NULL;
#endif
  scale=getScale();
  visible=false;
  title=name;
  estWaitTime=0;
  skin=new eSkin;
  skin->engine=this;
  for (int i=0; i<256; i++) {
    preEvCallback[i]=NULL;
    postEvCallback[i]=NULL;
  }
  preDrawCallback=NULL;
  postDrawCallback=NULL;
  drawStartCallback=NULL;
  drawEndCallback=NULL;
  defFont=new eFont(this);
  if (defFont->loaddef(eFontDefault)==0) {
    eLogE("Error while loading default font.\n");
  }
  if (FT_Init_FreeType(&ftlib)) {
    eLogE("Error while loading FreeType library.\n");
  } else {
    status=FT_New_Memory_Face(ftlib,FontAwesome_otf,FontAwesome_otf_len,0,&iconFont);
    if (status) {
      eLogE("Error while loading icon font.\n");
    }
  }
}

void eEngine::setTitle(string t) {
  title=t;
}

int eEngine::nextEvent(eEvent& ev, bool wait) {
  sf::Event temp;
  if (wait) {
    if (win->waitEvent(temp)==0) {
      return 0;
    }
  } else {
    if (win->pollEvent(temp)==0) {
      return 0;
    }
  }
  switch (temp.type) {
    case sf::Event::Closed:
      ev.type=eEventQuit;
      break;
    case sf::Event::MouseMoved:
      ev.type=eEventMouseMove;
      ev.coord.x=temp.mouseMove.x/scale;
      ev.coord.y=temp.mouseMove.y/scale;
      break;
    case sf::Event::MouseButtonPressed:
      ev.type=eEventMouseButton;
      ev.coord.x=temp.mouseButton.x/scale;
      ev.coord.y=temp.mouseButton.y/scale;
      ev.input=temp.mouseButton.button;
      ev.state=1;
      break;
    case sf::Event::MouseButtonReleased:
      ev.type=eEventMouseButton;
      ev.coord.x=temp.mouseButton.x/scale;
      ev.coord.y=temp.mouseButton.y/scale;
      ev.input=temp.mouseButton.button;
      ev.state=0;
      break;
    case sf::Event::MouseWheelMoved:
      return 0;
      break;
    case sf::Event::MouseWheelScrolled:
      ev.type=eEventMouseWheel;
      ev.coord.x=0;
      ev.coord.y=0;
      if (temp.mouseWheelScroll.wheel==sf::Mouse::HorizontalWheel) {
        ev.coord.x=-temp.mouseWheelScroll.delta;
      } else {
        ev.coord.y=-temp.mouseWheelScroll.delta;
      }
      break;
    case sf::Event::MouseEntered:
      ev.type=eEventMouseEnter;
      break;
    case sf::Event::MouseLeft:
      ev.type=eEventMouseLeave;
      break;
    case sf::Event::TouchMoved:
      ev.type=eEventTouchMove;
      ev.coord.x=temp.touch.x/scale;
      ev.coord.y=temp.touch.y/scale;
      ev.input=temp.touch.finger;
      break;
    case sf::Event::TouchBegan:
      ev.type=eEventTouchState;
      ev.coord.x=temp.touch.x/scale;
      ev.coord.y=temp.touch.y/scale;
      ev.input=temp.touch.finger;
      ev.state=1;
      break;
    case sf::Event::TouchEnded:
      ev.type=eEventTouchState;
      ev.coord.x=temp.touch.x/scale;
      ev.coord.y=temp.touch.y/scale;
      ev.input=temp.touch.finger;
      ev.state=0;
      break;
    case sf::Event::KeyPressed:
      ev.type=eEventKey;
      ev.input=temp.key.code;
      ev.state=1;
      ev.mod=0;
      if (temp.key.control) ev.mod|=eKeyModCtrl;
      if (temp.key.alt) ev.mod|=eKeyModAlt;
      if (temp.key.shift) ev.mod|=eKeyModShift;
      if (temp.key.system) ev.mod|=eKeyModMeta;
      break;
    case sf::Event::KeyReleased:
      ev.type=eEventKey;
      ev.input=temp.key.code;
      ev.state=0;
      ev.mod=0;
      if (temp.key.control) ev.mod|=eKeyModCtrl;
      if (temp.key.alt) ev.mod|=eKeyModAlt;
      if (temp.key.shift) ev.mod|=eKeyModShift;
      if (temp.key.system) ev.mod|=eKeyModMeta;
      break;
    case sf::Event::TextEntered:
      ev.type=eEventType;
      ev.input=temp.text.unicode;
      break;
    case sf::Event::GainedFocus:
      ev.type=eEventFocus;
      ev.state=1;
      break;
    case sf::Event::LostFocus:
      ev.type=eEventFocus;
      ev.state=0;
      break;
    case sf::Event::Resized:
      ev.type=eEventResize;
      ev.coord.x=temp.size.width;
      ev.coord.y=temp.size.height;
      break;
    case sf::Event::SensorChanged:
      ev.type=eEventSensor;
      switch (temp.sensor.type) {
        case sf::Sensor::Accelerometer:
          ev.type=eEventAccel;
          break;
        case sf::Sensor::Gyroscope:
          ev.type=eEventGyro;
          break;
        case sf::Sensor::Magnetometer:
          ev.type=eEventMagnet;
          break;
        case sf::Sensor::Gravity:
          ev.type=eEventGrav;
          break;
        case sf::Sensor::UserAcceleration:
          ev.type=eEventAcceleration;
          break;
        case sf::Sensor::Orientation:
          ev.type=eEventOrient;
          break;
        default:
          ev.type=eEventSensor;
          break;
      }
      ev.coord.x=temp.sensor.x;
      ev.coord.y=temp.sensor.y;
      ev.coord.z=temp.sensor.z;
      break;
    case sf::Event::EmilyThing:
      // redraw request
      ev.type=eEventRedrawRequest;
      break;
    default:
      ev.type=eEventBackend;
      eLogI("got backend event %d\n",temp.type);
      break;
  }
  return 1;
}

int eEngine::grabMouse(bool status) {
#if defined(__linux__)
  // linux (wayland) code here
#elif defined(_WIN32)
  // windows code here
  sf::WindowHandle wh;
  wh=win->getSystemHandle();
  if (wh==NULL) {
    return 0;
  }
  if (status) {
    return (SetCapture(wh)==NULL)?0:1; // ???
  } else {
    return ReleaseCapture()?1:0;
  }
#elif defined(__APPLE__)
  // macOS code here
  // seems like there is no way to do this directly :(
  return 0;
#elif defined(__ANDROID__)
  // android code here
  return 0;
#endif
#if defined(__unix__) && !defined(__APPLE__)
  // X11
  sf::WindowHandle wh;
  if (x11conn==NULL) { // it can happen
    return 0;
  }
  wh=win->getSystemHandle();
  if (status) {
    return XGrabPointer(x11conn,wh,True,None,GrabModeAsync,GrabModeAsync,None,None,CurrentTime);
  } else {
    return XUngrabPointer(x11conn,CurrentTime);
  }
#endif
  return 0;
}

void eMainLoop(eEngine* eng) {
  eEvent ev;
  eFrame* curFrame;
  int rVBTime, rPrevVBTime, rStartTime, rEndTime, avgVBTime;
#ifdef ENABLE_WAIT
  int waitStart;
#endif
  bool wait;
  rPrevVBTime=0;
  rVBTime=0;
  avgVBTime=0;
#ifdef ENABLE_WAIT
  waitStart=4;
#endif
  curFrame=NULL;
  wait=false;
  while (1) {
    eng->win=eng->displays[0]->win;
    eng->preRender();
    rPrevVBTime=rVBTime;
    rVBTime=perfCount();
#ifndef _WIN32
    eng->pause(eng->estWaitTime);
#endif
    /* event processing */
    rStartTime=perfCount();
    if (eng->displays[0]->frameStack.size()) {
      curFrame=eng->displays[0]->frameStack.top();
    }
#ifdef ENABLE_WAIT
    if (--waitStart<=0) {
      wait=true;
    }
#endif
    while (eng->nextEvent(ev,wait)) {
      wait=false;
#ifdef ENABLE_WAIT
      //waitStart=4;
#endif
      if (eng->preEvCallback[ev.type]!=NULL) {
        eng->preEvCallback[ev.type](&ev);
      }
      switch (ev.type) {
        case eEventQuit:
          return;
          break;
        case eEventMouseButton:
        case eEventMouseMove:
          eng->mouseX=ev.coord.x;
          eng->mouseY=ev.coord.y;
          if (!eng->openMenus.empty()) {
            for (size_t i=0; i<eng->openMenus.size(); i++) {
               eng->openMenus[i]->event(ev);
            }
            for (size_t i=0; i<eng->openMenus.size(); i++) {
               if (eng->openMenus[i]->wannaRetire) {
                 eng->openMenus.erase(eng->openMenus.begin()+i);
                 i--;
               }
            }
            if (!eng->openMenus.empty()) {
              break;
            }
          }
          for (size_t i=0; i<curFrame->widgets.size(); i++) {
            curFrame->widgets[i]->_collision=ev.coord.x>curFrame->widgets[i]->bLeft &&
                ev.coord.x<curFrame->widgets[i]->bRight &&
                ev.coord.y>curFrame->widgets[i]->bTop &&
                ev.coord.y<curFrame->widgets[i]->bBottom;
            if ((ev.type==eEventMouseButton && curFrame->widgets[i]->_relPending) ||
                (ev.type==eEventMouseMove && curFrame->widgets[i]->_highPending) ||
                curFrame->widgets[i]->_collision ||
                curFrame->widgets[i]->_wantsAllEvents) {
              if (ev.type==eEventMouseButton) {
                if (curFrame->widgets[i]->_collision && ev.state==1) {
                  curFrame->widgets[i]->_relPending=true;
                } else {
                  curFrame->widgets[i]->_relPending=false;
                }
              } else {
                if (curFrame->widgets[i]->_collision) {
                  curFrame->widgets[i]->_highPending=true;
                } else {
                  curFrame->widgets[i]->_highPending=false;
                }
              }
              if (curFrame->widgets[i]->event(ev)) {
                waitStart=4;
              }
            }
          }
          break;
        case eEventResize:
          // handle resizing
          eng->win->setView(sf::View(sf::FloatRect(0,0,ev.coord.x,ev.coord.y)));
          eng->displays[0]->w=ev.coord.x/eng->scale;
          eng->displays[0]->h=ev.coord.y/eng->scale;
          eng->displays[0]->current.w=ev.coord.x/eng->scale;
          eng->displays[0]->current.h=ev.coord.y/eng->scale;
          for (size_t i=0; i<eng->displays[0]->frameStack.top()->widgets.size(); i++) {
            eng->displays[0]->frameStack.top()->widgets[i]->calcBounds();
          }
          waitStart=4;
          break;
        case eEventRedrawRequest:
          waitStart=4;
          break;
        default:
          eLogD("got event %d\n",ev.type);
          break;
      }
      if (eng->postEvCallback[ev.type]!=NULL) {
        eng->postEvCallback[ev.type](&ev);
      }
    }
    if (waitStart<=0) continue;
    if (eng->preDrawCallback!=NULL) {
      eng->preDrawCallback();
    }
    if (eng->drawStartCallback!=NULL) {
      eng->drawStartCallback();
    }
    for (size_t i=0; i<curFrame->widgets.size(); i++) {
      if (curFrame->widgets[i]->draw()) {
        waitStart=4;
      }
    }
    // draw menus if any
    for (size_t i=0; i<eng->openMenus.size(); i++) {
      eng->openMenus[i]->draw();
    }
    eng->drawColor({1,1,1,1});
    //eng->line(0,0,eng->width,eng->height);
    if (eng->drawEndCallback!=NULL) {
      eng->drawEndCallback();
    }
    rEndTime=perfCount();
    eng->postRender();
    if (eng->postDrawCallback!=NULL) {
      eng->postDrawCallback();
    }
    if (rPrevVBTime==0) {
      eng->estWaitTime=0;
    } else {
      eng->estWaitTime=fmin(eng->estWaitTime+100,avgVBTime-((rEndTime-rStartTime)/1000)-3000);
    }
    avgVBTime=((avgVBTime*15)+((rVBTime-rPrevVBTime)/1000))/16;
    //printf("VBTime: %d\n",(rVBTime-rPrevVBTime)/1000);
    //printf("vblank %d wait %f\n",avgVBTime,eng->estWaitTime);
    if (eng->estWaitTime<0) {
      eng->estWaitTime=0;
    }
  }
}

int eEngine::run() {
  eMainLoop(this);
  return 0;
}

/* to be re-enabled
int eEngine::runDetached() {
  std::thread runThread(eMainLoop,this);
  return 0;
}
*/

int eEngine::setPreEventCallback(unsigned char event, void ((*callback)(const eEvent *))) {
  preEvCallback[event]=callback;
  return 1;
}

int eEngine::setPostEventCallback(unsigned char event, void ((*callback)(const eEvent *))) {
  postEvCallback[event]=callback;
  return 1;
}

int eEngine::setPreDrawCallback(void ((*callback)())) {
  preDrawCallback=callback;
  return 1;
}

int eEngine::setDrawStartCallback(void ((*callback)())) {
  drawStartCallback=callback;
  return 1;
}

int eEngine::setDrawEndCallback(void ((*callback)())) {
  drawEndCallback=callback;
  return 1;
}

int eEngine::setPostDrawCallback(void ((*callback)())) {
  postDrawCallback=callback;
  return 1;
}

eFrame* eEngine::newFrame() {
  eFrame* ret;
  ret=new eFrame;
  ret->engine=this;
  ret->parent=NULL;
  return ret;
}

void eEngine::popUpMenu(double x, double y, eContextMenu* menu) {
  openMenus.push_back(menu);
  // settle menu
  menu->engine=this;
  menu->wannaRetire=false;
  if (x==eAuto) {
    menu->x=mouseX;
  } else {
    menu->x=x;
  }
  if (y==eAuto) {
    menu->y=mouseY;
  } else {
    menu->y=y;
  }
}

void eEngine::drawColor(eColor color) {
  drawCol=color;
}

void eEngine::line(double x1, double y1, double x2, double y2) {
  sf::VertexArray temp(sf::Lines,2);
  temp[0].position=sf::Vector2f(x1*scale,y1*scale);
  temp[1].position=sf::Vector2f(x2*scale,y2*scale);
  win->draw(temp);
}

void eEngine::rect(double x1, double y1, double x2, double y2) {
  sf::VertexArray temp(sf::LineStrip,5);
  temp[0].position=sf::Vector2f(x1*scale,y1*scale);
  temp[1].position=sf::Vector2f(x2*scale,y1*scale);
  temp[2].position=sf::Vector2f(x2*scale,y2*scale);
  temp[3].position=sf::Vector2f(x1*scale,y2*scale);
  temp[4].position=sf::Vector2f(x1*scale,y1*scale);
  temp[0].color=sf::Color(drawCol.r*255,drawCol.g*255,drawCol.b*255,drawCol.a*255);
  temp[1].color=sf::Color(drawCol.r*255,drawCol.g*255,drawCol.b*255,drawCol.a*255);
  temp[2].color=sf::Color(drawCol.r*255,drawCol.g*255,drawCol.b*255,drawCol.a*255);
  temp[3].color=sf::Color(drawCol.r*255,drawCol.g*255,drawCol.b*255,drawCol.a*255);
  temp[4].color=sf::Color(drawCol.r*255,drawCol.g*255,drawCol.b*255,drawCol.a*255);
  win->draw(temp);
}

void eEngine::frect(double x1, double y1, double x2, double y2) {
  sf::VertexArray temp(sf::TriangleFan,4);
  temp[0].position=sf::Vector2f(x1*scale,y1*scale);
  temp[1].position=sf::Vector2f(x2*scale,y1*scale);
  temp[2].position=sf::Vector2f(x2*scale,y2*scale);
  temp[3].position=sf::Vector2f(x1*scale,y2*scale);
  temp[0].color=sf::Color(drawCol.r*255,drawCol.g*255,drawCol.b*255,drawCol.a*255);
  temp[1].color=sf::Color(drawCol.r*255,drawCol.g*255,drawCol.b*255,drawCol.a*255);
  temp[2].color=sf::Color(drawCol.r*255,drawCol.g*255,drawCol.b*255,drawCol.a*255);
  temp[3].color=sf::Color(drawCol.r*255,drawCol.g*255,drawCol.b*255,drawCol.a*255);
  win->draw(temp);
}

eVideoOut* eEngine::newVideoOut(int width, int height) {
  eVideoOut* temp;
#if defined(__unix__) && !defined(__APPLE__)
  if (x11conn==NULL) {
    x11conn=XOpenDisplay(NULL);
  }
#endif
  temp=new eVideoOut;
  temp->engine=this;
  temp->win=new sf::RenderWindow(sf::VideoMode(width*scale,height*scale),title,sf::Style::Titlebar|sf::Style::Close|sf::Style::Resize);
#if defined(__APPLE__) || defined(_WIN32)
  temp->win->setVerticalSyncEnabled(true);
#endif
  visible=true;
  temp->w=width;
  temp->h=height;
  temp->current.x=0;
  temp->current.y=0;
  temp->current.w=width;
  temp->current.h=height;
  displays.push_back(temp);
  return temp;
}

int eFrame::getWidth() {
  if (parent) {
    return parent->getWidth();
  }
  return -1;
}

int eFrame::getHeight() {
  if (parent) {
    return parent->getHeight();
  }
  return -1;
}

// *TEMPORARILY DEPRECATED FACILITIES BEGIN* //

eTexture* eEngine::getUnmanagedTexture(int width, int height, int type) {
  return NULL;
}

eTexture* eEngine::getTexture(int width, int height, int type, int prop0, int prop1, int prop2, int prop3) {
  return NULL;
}

eTexture* eEngine::getTextureFromBitmap(eBitmap* bitmap, int type) {
  return NULL;
  /*
  eTexture* ret;
  void* tex;
  tex=eCreateTexture(backInst,bitmap->width,bitmap->height,type);
  eUpdateTexture(tex,bitmap->data,bitmap->pitch());
  printf("RETURN VAL %x\n",tex);
  if (tex==NULL) {
    return NULL;
  }
  ret=new eTexture;
  ret->actual=tex;
  ret->width=width;
  ret->height=height;
  ret->type=type;
  return ret;
  */
}

int eEngine::drawTexture(eTexture* tex, double x, double y) {
  return 0;
  /*
  return eDrawTexture(backInst,tex,x,y);
  */
}

// *TEMPORARILY DEPRECATED FACILITIES END* //

eIcon* eEngine::newIcon(eIcons icon, double size) {
  eIcon* ret;
  int bsize;
  unsigned char* expand;
  FT_Set_Char_Size(iconFont,0,64*size,96*scale,96*scale);
  if (FT_Load_Char(iconFont,icon,FT_LOAD_RENDER)) {
    return NULL;
  }
  ret=new eIcon;
  ret->charIndex=icon;
  bsize=iconFont->glyph->bitmap.width*iconFont->glyph->bitmap.rows;
  expand=new unsigned char[bsize*4];
  for (int i=0; i<bsize; i++) {
    expand[(i<<2)]=255;
    expand[1+(i<<2)]=255;
    expand[2+(i<<2)]=255;
    expand[3+(i<<2)]=iconFont->glyph->bitmap.buffer[i];
  }
  /* border debug
  expand[3]=255;
  expand[(bsize*4)-1]=255;
  */
  ret->engine=this;
  ret->iconT=new sf::Texture;
  if (!ret->iconT->create(iconFont->glyph->bitmap.width,iconFont->glyph->bitmap.rows)) {
    return NULL;
  }
  ret->iconT->update(expand);
  ret->iconT->setSmooth(true);
  ret->charW=iconFont->glyph->bitmap.width;
  ret->charH=iconFont->glyph->bitmap.rows;
  ret->iconS.setTexture(*ret->iconT);
  delete[] expand;
  return ret;
}

int eEngine::pause(double timeAsMicro) {
  sf::sleep(sf::microseconds(timeAsMicro));
  return 1;
}

long long perfCount() {
#ifdef _WIN32
  LARGE_INTEGER temp, prec;
  QueryPerformanceCounter(&temp);
  QueryPerformanceFrequency(&prec);
  return temp.QuadPart*(1000000000/prec.QuadPart);
#else
#ifdef __MACH__
  return mach_absolute_time();
#else
  struct timespec temp;
  clock_gettime(CLOCK_MONOTONIC,&temp);
  return (temp.tv_sec*1000000000)+temp.tv_nsec;
#endif
#endif
}

int eEngine::unblockWait() {
  if (win==NULL) return 0;
  return win->giveUpWaiting();
}

void eEngine::preRender() {
  win->clear();
}

void eEngine::postRender() {
  // wait then display
  win->waitVBlank();
  win->display();
}
