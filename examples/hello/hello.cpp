#include <stdio.h>
#include <unistd.h>
#include "toolkit.h"

eEngine* gui;

void myCallback(const eEvent* ev) {
  printf("callback! %d\n",ev->state);
}

int main(int argc, char** argv) {
  gui=new eEngine(eBackSDL2);
  gui->setPostEventCallback(eEventMouseButton,myCallback);
  gui->show();
  return gui->run();
}
