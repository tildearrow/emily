#include "toolkit.h"
#ifdef __APPLE__
#include <mach/mach_time.h>
extern "C" {
#include "nsstub.h"
}
#endif
#include "../fa/fa.h"

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
  xcb_disconnect(disp);
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

eEngine::eEngine(double w, double h) {
  scale=getScale();
  visible=false;
  title="Application";
  estWaitTime=0;
  width=w;
  height=h;
  skin=new eSkin;
  skin->engine=this;
  for (int i=0; i<256; i++) {
    preEvCallback[i]=NULL;
    postEvCallback[i]=NULL;
    preDrawCallback=NULL;
    postDrawCallback=NULL;
    drawStartCallback=NULL;
    drawEndCallback=NULL;
  }
  defFont=new eFont;
  if (defFont->loaddef(eFontDefault)==0) {
    eLogE("Error while loading default font.\n");
  }
  iconFont.loadFromMemory(FontAwesome_otf,FontAwesome_otf_len);
}

int eEngine::setTitle(string t) {
  title=t;
  return 1;
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
        // to satisfy KDevelop and maybe Xcode
        case sf::Sensor::Count:
          ev.type=eEventSensor;
          break;
      }
      ev.coord.x=temp.sensor.x;
      ev.coord.y=temp.sensor.y;
      ev.coord.z=temp.sensor.z;
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
  if (status) {
    SetCapture(wh); // ???
    return 1;
  } else {
    return ReleaseCapture();
  }
#elif defined(__APPLE__)
  // macOS code here
  // seems like there is no way to do this directly :(
  return 1;
#elif defined(__ANDROID__)
  // android code here
  return 1;
#endif
#if defined(__unix__) && !defined(__APPLE__)
  // X11
  sf::WindowHandle wh;
  wh=win->getSystemHandle();
  if (status) {
    return XGrabPointer(x11conn,wh,True,None,GrabModeAsync,GrabModeAsync,None,None,CurrentTime);
  } else {
    XUngrabPointer(x11conn,CurrentTime);
    return 1;
  }
#endif
  return 1;
}

void eMainLoop(eEngine* eng) {
  eEvent ev;
  eFrame* curFrame;
  eIcon* iii;
  int rVBTime, rPrevVBTime, rStartTime, rEndTime, avgVBTime;
#ifdef ENABLE_WAIT
  int waitStart;
#endif
  bool wait;
  rPrevVBTime=0;
  rVBTime=0;
  avgVBTime=0;
#ifdef ENABLE_WAIT
  waitStart=32;
#endif
  curFrame=NULL;
  wait=false;
  iii=eng->newIcon(eIconTTY,16);
  while (1) {
    eng->preRender();
    rPrevVBTime=rVBTime;
    rVBTime=eng->perfCount();
#ifndef _WIN32
    eng->pause(eng->estWaitTime);
#endif
    /* event processing */
    rStartTime=eng->perfCount();
    if (eng->frameStack.size()) {
      curFrame=eng->frameStack.top();
    }
#ifdef ENABLE_WAIT
    if (--waitStart<=0) {
      wait=true;
    }
#endif
    while (eng->nextEvent(ev,wait)) {
      wait=false;
#ifdef ENABLE_WAIT
      waitStart=32;
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
          for (size_t i=0; i<curFrame->widgets.size(); i++) {
            curFrame->widgets[i]->_collision=ev.coord.x>curFrame->widgets[i]->x &&
                ev.coord.x<curFrame->widgets[i]->x+curFrame->widgets[i]->w &&
                ev.coord.y>curFrame->widgets[i]->y &&
                ev.coord.y<curFrame->widgets[i]->y+curFrame->widgets[i]->h;
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
              curFrame->widgets[i]->event(ev);
            }
          }
          break;
        default:
          eLogD("got event %d\n",ev.type);
          break;
      }
      if (eng->postEvCallback[ev.type]!=NULL) {
        eng->postEvCallback[ev.type](&ev);
      }
    }
    if (eng->preDrawCallback!=NULL) {
      eng->preDrawCallback();
    }
    if (eng->drawStartCallback!=NULL) {
      eng->drawStartCallback();
    }
    for (size_t i=0; i<curFrame->widgets.size(); i++) {
      curFrame->widgets[i]->draw();
    }
    eng->drawColor({1,1,1,1});
    iii->setPos(eng->estWaitTime/50,eng->estWaitTime/100);
    iii->draw();
    //eng->line(0,0,eng->width,eng->height);
    if (eng->drawEndCallback!=NULL) {
      eng->drawEndCallback();
    }
    rEndTime=eng->perfCount();
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
  return ret;
}

int eEngine::pushFrame(eFrame* f) {
  frameStack.push(f);
  return 1;
}

int eEngine::popFrame() {
  frameStack.pop();
  return 1;
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

int eEngine::show() {
  if (!visible) {
    win=new sf::RenderWindow(sf::VideoMode(width*scale,height*scale),title,sf::Style::Titlebar|sf::Style::Close);
#if defined(__unix__) && !defined(__APPLE__)
    x11conn=XOpenDisplay(NULL);
#endif
    win->setVerticalSyncEnabled(true);
    visible=true;
    return 1;
  }
  return 0;
}

eTexture* eEngine::getUnmanagedTexture(int width, int height, int type) {
  return NULL;
  /*
  eTexture* ret;
  void* tex;
  tex=eCreateTexture(backInst,width,height,type);
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

eTexture* eEngine::getTexture(int width, int height, int type, int prop0, int prop1, int prop2, int prop3) {
  return NULL;
  /*
  eTexture* ret;
  void* tex;
  tex=eCreateTexture(backInst,width,height,type);
  if (tex==NULL) {
    return NULL;
  }
  ret=new eTexture;
  ret->actual=tex;
  ret->width=width;
  ret->height=height;
  ret->type=type;
  ret->id[0]=prop0;
  ret->id[1]=prop1;
  ret->id[2]=prop2;
  ret->id[3]=prop3;
  return ret;
  */
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

eIcon* eEngine::newIcon(eIcons icon, double size) {
  eIcon* ret;
  ret=new eIcon;
  ret->isImage=false;
  ret->iconText=new sf::Text(sf::String((sf::Uint32)icon),iconFont,size*scale);
  ret->engine=this;
  return ret;
}

int eEngine::pause(double timeAsMicro) {
  sf::sleep(sf::microseconds(timeAsMicro));
  return 1;
}

long long eEngine::perfCount() {
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

void eEngine::preRender() {
  /*
  struct timespec start, end;
  clock_gettime(CLOCK_MONOTONIC,&start);
  */
  win->clear();
  /*
  clock_gettime(CLOCK_MONOTONIC,&end);
  printf("time: %ld\n",(start.tv_nsec-end.tv_nsec)/1000);
  */
}

void eEngine::postRender() {
  win->display();
}
