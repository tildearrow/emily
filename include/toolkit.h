#ifndef _TOOLKIT_H
#define _TOOLKIT_H
#include <vector>

#define eBackSDL2 0

class eEngine {
  void* backWin;
  void* backInst;
  public:
    eEngine(int backend);
    ~eEngine();
    int show();
};

#endif
