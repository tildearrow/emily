#ifndef _TOOLKIT_H
#define _TOOLKIT_H
#include <stdio.h>
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

class eWidget {
};

class eFrame {
  std::vector<eWidget> widgets;
};

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
  bool visible;
  string title;
  int width, height;
  float estWaitTime;
  FT_Library ftlib;
  friend void eMainLoop(eEngine* eng);
  public:
    eEngine(int backend);
    ~eEngine();
    int setPreEventCallback(unsigned char event, void ((*callback)(const eEvent*)));
    int setPostEventCallback(unsigned char event, void ((*callback)(const eEvent*)));
    int setTitle(string t);
    int setSize(int w, int h);
    int show();
    int run();
    int runDetached();
};

void eMainLoop(eEngine* eng);

#endif
