#include <stdio.h>
#include <unistd.h>
#include "toolkit.h"

eEngine* gui;

int main(int argc, char** argv) {
  gui=new eEngine(eBackSDL2);
  gui->show();
  return gui->run();
}
