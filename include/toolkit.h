#ifndef _TOOLKIT_H
#define _TOOLKIT_H
#include <stdio.h>
#include <math.h>
#include <stack>
#include <string>
#include <vector>
#ifdef HAVE_FONTCONFIG
#include <fontconfig/fontconfig.h>
#endif
#include <SFML/Graphics.hpp>
// platform-specific headers
#ifdef _WIN32
#include <windows.h>
#endif
#ifdef __unix__
#include <unistd.h>
#ifdef USE_XCB
#include <xcb/xcb.h>
#else
#include <X11/Xlib.h>
#endif
#endif

typedef std::string string;

#define eBackSFML 0

// for now
// debug
#define eLogD printf
// info
#define eLogI printf
// error
#define eLogE printf

enum eEventSystem {
  eEventQuit=0x01,
  eEventBackend=0xfe,
};

enum eEventInput {
  eEventMouseMove=0x11,
  eEventMouseButton=0x12,
  eEventTouch=0x21,
  eEventKey=0x31,
  eEventType=0x41,
  eEventFile=0x51,
};

enum eFontVariant {
  eFontDefault=0,
  eFontLarge
};

enum eTextureTypes {
  eStatic=0,
  eStream,
  eTarget
};

struct eEvent {
  unsigned char type;
  struct {
    double x, y, z;
  } coord;
  int input;
  int state;
  int mod;
};

struct eColor {
  float r, g, b, a;
};

class eEngine;

class eBitmap {
  public:
    int width, height, depth;
    unsigned char* data;
    int pitch();
    eBitmap(int inWidth, int inHeight, int inDepth);
    ~eBitmap();
};

struct eRect {
  double x, y, w, h;
};

struct eTexture {
  unsigned int id[4];
  int width, height, type;
  void* actual;
};

class eFont {
  public:
    sf::Font inst;
    int loadfn(const char* filename);
    int loadfam(const char* name);
    int loaddef(int variant);
};

enum eObjectTypes {

};

class eSkin {
  struct cacheElem {
    int type, attrib[8], w, h;
    sf::Texture* tex;
  };
  std::vector<cacheElem> cache;
  public:
    virtual sf::Texture* getTexture(int objectType, int attrib[8], int w, int h, double* xo, double* yo);
};

class eWidget {
  friend class eFrame;
  friend void eMainLoop(eEngine* eng);
  protected:
    eEngine* engine;
    double w, h;
    virtual int event(eEvent& ev);
    bool _relPending, _highPending, _collision;
  public:
    double x, y;
    virtual int init();
    virtual int setSize(double w, double h);
    virtual int draw();
};

class eFrame {
  eEngine* engine;
  std::vector<eWidget*> widgets;
  friend void eMainLoop(eEngine* eng);
  friend class eEngine;
  public:
    template<typename t> void* newWidget() {
      t* push=new t;
      push->engine=engine;
      push->x=0;
      push->y=0;
      push->_collision=false;
      push->_relPending=false;
      push->_highPending=false;
      push->init();
      widgets.push_back(push);
      return push;
    }
};

#include "widgets/widgets.h"

class eEngine {
  /*void* (*createWin)(void**,const char*,int,int,int,int,bool);
  int (*eNextEvent)(void*,eEvent&);*/
  void ((*preEvCallback[256])(const eEvent*));
  void ((*postEvCallback[256])(const eEvent*));
  void (*preDrawCallback)();
  void (*drawStartCallback)();
  void (*drawEndCallback)();
  void (*postDrawCallback)();
  /*
  void (*eWait)(int);
  void (*ePreRender)(void*);
  void (*ePostRender)(void*);
  void (*eDrawColor)(void*,unsigned char,unsigned char,unsigned char,unsigned char);
  void (*eLine)(void*,double,double,double,double);
  void* (*eCreateTexture)(void*,int,int,int);
  int (*eUpdateTexture)(void*,void*,int);
  int (*eDrawTexture)(void*,eTexture*,double,double);
  */
  void preRender();
  void postRender();
  std::stack<eFrame*> frameStack;
  std::vector<eTexture*> regTextures;
  bool visible;
  string title;
  int width, height;
  double scale;
  float estWaitTime;
  eColor drawCol;
  eFont* defFont;
  friend void eMainLoop(eEngine* eng);
  friend class eWidget;
  friend class eLabel;
  friend class eButton;
  int nextEvent(eEvent& ev);
  protected:
    sf::RenderWindow* win;
    eSkin* skin;
  public:
    eEngine(double w, double h);
    ~eEngine();
    eFont* newFont();
    int setPreEventCallback(unsigned char event, void ((*callback)(const eEvent*)));
    int setPostEventCallback(unsigned char event, void ((*callback)(const eEvent*)));
    int setPreDrawCallback(void ((*callback)()));
    int setDrawStartCallback(void ((*callback)()));
    int setDrawEndCallback(void ((*callback)()));
    int setPostDrawCallback(void ((*callback)()));
    int setTitle(string t);
    int setSize(int w, int h);
    int show();
    int run();
    int runDetached();
    int pause(double timeAsMicro);
    long long perfCount();

    eFrame* newFrame();
    int pushFrame(eFrame* f);
    int popFrame();
    
    void drawColor(eColor color);
    void line(double x1, double y1, double x2, double y2);
    void rect(double x1, double y1, double x2, double y2);
    void frect(double x1, double y1, double x2, double y2);

    eTexture* getTexture(int width, int height, int type, int prop0, int prop1, int prop2, int prop3);
    eTexture* getUnmanagedTexture(int width, int height, int type);
    eTexture* getTextureFromBitmap(eBitmap* bitmap, int type);
    void* lockTexture(eTexture* tex);
    int unlockTexture(eTexture* tex);
    int drawTexture(eTexture* tex, double x, double y);
};

void eMainLoop(eEngine* eng);

#endif
