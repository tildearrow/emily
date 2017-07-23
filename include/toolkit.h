#ifndef _TOOLKIT_H
#define _TOOLKIT_H
#include <stdio.h>
#include <string>
#include <thread>
#include <vector>

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
  eEventQuit=0x0001,
  eEventBackend=0x00fe,
};

enum eEventInput {
  eEventMouseMove=0x0101,
  eEventMouseButton=0x0102,
  eEventTouch=0x0201,
  eEventKey=0x0301,
  eEventType=0x0401,
  eEventFile=0x0501,
};

struct eEvent {
  unsigned short type;
  struct {
    double x, y, z;
  } coord;
  int input;
  int state;
  int mod;
};

class eEngine {
  void* backWin;
  void* backInst;
  void* (*createWin)(void**,const char*,int,int,int,int,bool);
  int (*eNextEvent)(eEvent&);
  void (*ePreRender)(void*);
  void (*ePostRender)(void*);
  bool visible;
  string title;
  int width, height;
  friend void eMainLoop(eEngine* eng);
  public:
    eEngine(int backend);
    ~eEngine();
    int setTitle(string t);
    int setSize(int w, int h);
    int show();
    int run();
    int runDetached();
};

void eMainLoop(eEngine* eng);

#endif
