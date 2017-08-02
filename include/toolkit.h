#ifndef _TOOLKIT_H
#define _TOOLKIT_H
#include <stdio.h>
#include <stack>
#include <string>
#include <thread>
#include <vector>
#include <ft2build.h>
#include FT_FREETYPE_H

typedef std::string string;

#define eBackSDL2 0

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

struct eEvent {
  unsigned char type;
  struct {
    double x, y, z;
  } coord;
  int input;
  int state;
  int mod;
};

class eFont {
  FT_Library lib;
  FT_Face face;
  public:
    int loadfn(const char* filename);
    int loadfam(const char* name);
    void size(float size);
    int loaddef(int variant);
    eFont(FT_Library l);
};

class eWidget {
  void* engine;
  double x, y, w, h;
  public:
    virtual int draw();
};

class eFrame {
  public:
    std::vector<eWidget> widgets;
};

class eEngine {
  void* backWin;
  void* backInst;
  void* (*createWin)(void**,const char*,int,int,int,int,bool);
  int (*eNextEvent)(eEvent&);
  void ((*preEvCallback[256])(const eEvent*));
  void ((*postEvCallback[256])(const eEvent*));
  void (*eWait)(int);
  void (*ePreRender)(void*);
  void (*ePostRender)(void*);
  void (*eDrawColor)(void*,unsigned char,unsigned char,unsigned char,unsigned char);
  void (*eLine)(void*,double,double,double,double);
  std::stack<eFrame*> frameStack;
  bool visible;
  string title;
  int width, height;
  float estWaitTime;
  FT_Library ftlib;
  eFont* defFont;
  friend void eMainLoop(eEngine* eng);
  public:
    eEngine(int backend);
    ~eEngine();
    eFont* newFont();
    int setPreEventCallback(unsigned char event, void ((*callback)(const eEvent*)));
    int setPostEventCallback(unsigned char event, void ((*callback)(const eEvent*)));
    int setTitle(string t);
    int setSize(int w, int h);
    int show();
    int run();
    int runDetached();
    
    int pushFrame(eFrame* f);
    int popFrame();
    
    void drawColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
    void line(double x1, double y1, double x2, double y2);
};

void eMainLoop(eEngine* eng);

#endif
