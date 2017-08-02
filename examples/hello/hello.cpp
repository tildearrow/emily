#include <stdio.h>
#include <unistd.h>
#include "toolkit.h"

eEngine* gui;
eFrame* frame;

void myCallback(const eEvent* ev) {
  printf("callback! %d\n",ev->state);
}

int main(int argc, char** argv) {
  gui=new eEngine(eBackSDL2);
  frame=new eFrame;
  gui->setPostEventCallback(eEventMouseButton,myCallback);
  gui->pushFrame(frame);
  gui->show();
  return gui->run();
}
