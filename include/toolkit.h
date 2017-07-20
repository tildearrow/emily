#ifndef _TOOLKIT_H
#define _TOOLKIT_H
#include <vector>

int prepBack(int backend);

class eWindow {
  public:
    void* backWin;
    void* backInst;
    int point(double x, double y);
    int rect(double x, double y, double w, double h);
    int line(double x1, double y1, double x2, double y2);
};

class eEngine {
  std::vector<eWindow*> wins;
  public:
    eEngine();
    ~eEngine();
    int newWindow();
};

#endif
