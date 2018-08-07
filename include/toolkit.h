#ifndef _TOOLKIT_H
#define _TOOLKIT_H

#define eMakeVer(x,y,z) \
  (x<<24|y<<16|z)

/* VERSION INFORMATION!
 * 
 * API versions before 1.0 are unstable!
 * please wait until I finish this toolkit so you can begin
 * to use it. yes, I aim for API compatibility for once!
 */
#define EMILY_API_VER eMakeVer(0,1,0)

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <stack>
#include <string>
#include <vector>
#ifdef HAVE_FONTCONFIG
#include <fontconfig/fontconfig.h>
#endif
#include "SFML/Graphics.hpp"
// platform-specific headers
#ifdef _MSC_VER
#define XPT __declspec(dllexport)
#else
#define XPT
#endif
#ifdef _WIN32
#include <windows.h>
#endif
#ifdef __unix__
#include <unistd.h>
#ifdef USE_XCB
#include <xcb/xcb.h>
#endif
#include <X11/Xlib.h>
#endif
#include "icons.h"
#ifdef IS_LIBRARY
#include <ft2build.h>
#include FT_FREETYPE_H
#endif

typedef std::string string;

#define eBackSFML 0

#define eAuto -1

// for now
// debug
#define eLogD printf
// info
#define eLogI printf
// error
#define eLogE printf

enum eEventSystem {
  eEventQuit=0x01,
  eEventResize=0x02,
  eEventFocus=0xfd,
  eEventBackend=0xfe,
};

enum eEventInput {
  eEventMouseMove=0x11,
  eEventMouseButton=0x12,
  eEventMouseWheel=0x13,
  eEventMouseEnter=0x14,
  eEventMouseLeave=0x15,
  eEventTouchMove=0x21,
  eEventTouchState=0x22,
  eEventKey=0x31,
  eEventType=0x41,
  eEventFile=0x51,
  eEventSensor=0x60,
  eEventAccel=0x61,
  eEventGyro=0x62,
  eEventMagnet=0x63,
  eEventGrav=0x64,
  eEventAcceleration=0x65,
  eEventOrient=0x66
};

enum eKeyMods {
  eKeyModCtrl=0x01,
  eKeyModAlt=0x02,
  eKeyModShift=0x04,
  eKeyModMeta=0x08
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

enum eDirection {
  eRight=0,
  eUp,
  eLeft,
  eDown,
  eCenter
};

#define eAlignLeft 0
#define eAlignTop 0
#define eAlignRight 1
#define eAlignBottom 1
#define eAlignCenter 0.5

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

string strFormat(const char* format, ...);

class eEngine;
class eVideoOut;
class eFrame;

class eBitmap {
  public:
    int width, height;
    float* data; // yes
    int pitch();
    void clear();
    void clearToColor(eColor c);
    void roundRect(int x, int y, int w, int h, int r, eColor color);
    void circle(int x, int y, int r, eColor color);
    void rect(double x, double y, double w, double h, eColor color);
    void shadeGlowBack(int radius, int passes, eColor col);
    void shadeColor(eColor c);
    void shadePower(float val);
    void shadeAlpha(float a);
    void shadeHMGrad(eColor c1, eColor c2);
    void shadeVMGrad(eColor c1, eColor c2);
    void shadeVGrad(double p1, double p2, eColor c1, eColor c2);
    void shadeHGrad(double p1, double p2, eColor c1, eColor c2);
    void copyBlitOn(eBitmap* src, int x, int y);
    void blitOn(eBitmap* src, int x, int y);
    sf::Texture* toTexture();
    eBitmap(int inWidth, int inHeight);
    ~eBitmap();
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
  /* attrib list for eObjectButton:
   * [0-3]=(eColor)buttonColor,
   * [4]=buttonType (0: normal, 1: flat, 2: icon-only)
   * 
   * attrib list for e
   */
  eObjectButton=0,
  eObjectSliderB,
  eObjectSliderH,
  eObjectLight
};

class eSkin {
  struct cacheElem {
    int type, attrib[8], w, h;
    sf::Texture* tex;
  };
  std::vector<cacheElem> cache;
  eEngine* engine;
  friend class eEngine;
  public:
    virtual eColor getDefaultColor(int objectType);
    virtual sf::Texture* getTexture(int objectType, int attrib[8], int w, int h, double* xo, double* yo, int* frameWidth, int* frameHeight);
};

class eWidget {
  friend class eFrame;
  friend class eVideoOut;
  friend class eFrameView;
  friend void eMainLoop(eEngine* eng);
  protected:
    eEngine* engine;
    eFrame* parent;
    double w, h;
    double dX, dY;
    double alignX, alignY;
    double x, y;
    double bTop, bBottom, bLeft, bRight;
    XPT virtual int event(eEvent& ev);
    bool _relPending, _highPending, _collision;
    bool _wantsAllEvents; // widgets may set this one
    bool _regenGraphics; // this one too
    bool _recalcBounds;
    virtual int calcBounds();
    // generic callbacks
    void (*mouseClickCallback)(eWidget*,int);
    void (*mouseClickAltCallback)(eWidget*,int);
    void (*mouseClickCancelCallback)(eWidget*,int);
    void (*mouseWheelCallback)(eWidget*,int);
    void (*hoverCallback)(eWidget*,bool);
    void (*keyCallback)(eWidget*,int,bool);
    void (*typeCallback)(eWidget*,int);
    void (*valueCallback)(eWidget*);
  public:
    XPT virtual int init();
    XPT virtual int setSize(double w, double h);
    XPT double getX() {
      return x;
    }
    XPT double getY() {
      return y;
    }
    XPT void setX(double xp) {
      x=xp; calcBounds();
    }
    XPT void setY(double yp) {
      y=yp; calcBounds();
    }
    XPT void setPos(double xp, double yp) {
      x=xp; y=yp; calcBounds();
    }
    XPT void setClickCallback(void (*callback)(eWidget*,int)) {
      mouseClickCallback=callback;
    }
    XPT void setClickAltCallback(void (*callback)(eWidget*,int)) {
      mouseClickAltCallback=callback;
    }
    XPT void setClickCancelCallback(void (*callback)(eWidget*,int)) {
      mouseClickCancelCallback=callback;
    }
    XPT void setWheelCallback(void (*callback)(eWidget*,int)) {
      mouseWheelCallback=callback;
    }
    XPT void setHoverCallback(void (*callback)(eWidget*,bool)) {
      hoverCallback=callback;
    }
    XPT void setKeyCallback(void (*callback)(eWidget*,int,bool)) {
      keyCallback=callback;
    }
    XPT void setTypeCallback(void (*callback)(eWidget*,int)) {
      typeCallback=callback;
    }
    XPT void setValueCallback(void (*callback)(eWidget*)) {
      valueCallback=callback;
    }
    XPT virtual int setDispPos(double x, double y);
    XPT virtual int setAlign(double x, double y);
    XPT virtual int draw();
    eWidget(): dX(0), dY(0), w(0), h(0), alignX(0), alignY(0), x(0), y(0),
               mouseClickCallback(NULL),
               mouseClickAltCallback(NULL),
               mouseClickCancelCallback(NULL),
               mouseWheelCallback(NULL),
               hoverCallback(NULL),
               keyCallback(NULL),
               typeCallback(NULL),
               valueCallback(NULL)
               {}
};

class eFrame {
  eEngine* engine;
  eWidget* parent;
  eVideoOut* parentD;
  std::vector<eWidget*> widgets;
  friend void eMainLoop(eEngine* eng);
  friend class eEngine;
  friend class eVideoOut;
  friend class eFrameView;
  public:
    XPT int getWidth();
    XPT int getHeight();
    template<typename t> t* newWidget() {
      t* push=new t;
      push->engine=engine;
      push->parent=this;
      push->x=0;
      push->y=0;
      push->_collision=false;
      push->_relPending=false;
      push->_highPending=false;
      push->_wantsAllEvents=false;
      push->init();
      widgets.push_back(push);
      return push;
    }
};

class eIcon {
  friend class eEngine;
  sf::Texture* iconT;
  sf::Sprite iconS;
  protected:
    eEngine* engine;
    int charIndex;
    double charW, charH;
  public:
    ~eIcon();
    void setPos(double x, double y);
    int hasLoaded();
    double width();
    double height();
    int draw();
};

#include "widgets/widgets.h"

class eMenuItem {
  friend class eEngine;
  friend class eContextMenu;
  protected:
    bool iconAfter, containsMore;
    string text;
    sf::Text* dtext;
    std::vector<eMenuItem> more;
    void (*callback)();
    int iconIndex;
    eIcon* icon;
  public:
    int addItem(eMenuItem item);
    int removeItem(int index);
    int itemCount();
    eMenuItem();
    eMenuItem(string text, void (*callback)());
    eMenuItem(string text, string rtext, void (*callback)());
    eMenuItem(eIcons icon, string text, void (*callback)());
    eMenuItem(eIcons icon, string text, string rtext, void (*callback)());
    ~eMenuItem();
};

class eContextMenu {
  friend class eEngine;
  friend void eMainLoop(eEngine* eng);
  int selected;
  protected:
    std::vector<eMenuItem> items;
    eEngine* engine;
    double w, h;
    bool wannaRetire;
    int event(eEvent& ev);
  public:
    double x, y;
    int addItem(eMenuItem item);
    int removeItem(int index);
    int itemCount();
    int draw();
};

class eAnimator {
  friend class eEngine;
  double pos;
  bool dir;
  double* varDouble;
  float* varFloat;
  int* varInt;
  /* 0: double
   * 1: float
   * 2: int
   */
  int varType;
  /* 0: jump middle
   * 1: linear
   * 2: accelerate
   * 3: brake
   * 4: accel+brake
   * 5: sine
   * by default 4.
   */
  int animType;
  double animLength;
  double animCurve;
  /* 0: no
   * 1: yes
   * 2: ping pong
   */
  int animLoop;
  protected:
    eEngine* engine;
  public:
    int setTarget(double& target);
    int setTarget(float& target);
    int setTarget(int& target);
    int setTarget();
    int direction(bool backwards);
    int play();
    int stop();
    int length(double len);
    int jump(double pos);
};

XPT long long perfCount();

class eVideoOut {
  friend class eEngine;
  friend void eMainLoop(eEngine* eng);
  double w, h;
  sf::RenderWindow* win;
  std::stack<eFrame*> frameStack;
  public:
    XPT int pushFrame(eFrame* f);
    XPT int popFrame();
    XPT int close();
    XPT int getWidth();
    XPT int getHeight();
};

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
  std::vector<eTexture*> regTextures;
  std::vector<eContextMenu*> openMenus;
  std::vector<eVideoOut*> displays;
  bool visible;
  string title;
  double scale;
  float estWaitTime;
  double mouseX, mouseY;
  eColor drawCol;
  eFont* defFont;
#ifdef IS_LIBRARY
  FT_Library ftlib;
  FT_Face iconFont;
#else
  void* ftlib;
  void* iconFont;
#endif
#ifdef __unix__
  Display* x11conn;
#endif
  friend void eMainLoop(eEngine* eng);
  friend class eWidget;
  friend class eLabel;
  friend class eButton;
  friend class eLight;
  friend class eFrameView;
  friend class eSlider;
  friend class eSkin;
  friend class eIcon;
  friend class eContextMenu;
  int nextEvent(eEvent& ev, bool wait);
  protected:
    sf::RenderWindow* win;
    eSkin* skin;
  public:
    XPT eEngine(string name="Application");
    XPT ~eEngine();
    XPT eFont* newFont();
    XPT eIcon* newIcon(eIcons icon, double size);
    XPT int setPreEventCallback(unsigned char event, void ((*callback)(const eEvent*)));
    XPT int setPostEventCallback(unsigned char event, void ((*callback)(const eEvent*)));
    XPT int setPreDrawCallback(void ((*callback)()));
    XPT int setDrawStartCallback(void ((*callback)()));
    XPT int setDrawEndCallback(void ((*callback)()));
    XPT int setPostDrawCallback(void ((*callback)()));
    XPT int setTitle(string t);
    XPT int setSize(int w, int h);
    XPT int getWidth();
    XPT int getHeight();
    XPT int grabMouse(bool status);
    XPT eVideoOut* newVideoOut(int width, int height);
    XPT int run();
    XPT int runDetached();
    XPT int pause(double timeAsMicro);

    XPT eFrame* newFrame();
    
    XPT void popUpMenu(double x, double y, eContextMenu* menu);
    
    XPT void drawColor(eColor color);
    XPT void line(double x1, double y1, double x2, double y2);
    XPT void rect(double x1, double y1, double x2, double y2);
    XPT void frect(double x1, double y1, double x2, double y2);

    XPT eTexture* getTexture(int width, int height, int type, int prop0, int prop1, int prop2, int prop3);
    XPT eTexture* getUnmanagedTexture(int width, int height, int type);
    XPT eTexture* getTextureFromBitmap(eBitmap* bitmap, int type);
    XPT void* lockTexture(eTexture* tex);
    XPT int unlockTexture(eTexture* tex);
    XPT int drawTexture(eTexture* tex, double x, double y);
};

void eMainLoop(eEngine* eng);

#endif
