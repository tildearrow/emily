#include "toolkit.h"

eEngine* gui;

int main(int argc, char** argv) {
  gui=new eEngine(eBackSDL2);
  gui->show();
  return 0;
}
