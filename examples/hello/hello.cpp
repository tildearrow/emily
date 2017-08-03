#include <stdio.h>
#include <unistd.h>
#include "toolkit.h"

eEngine* gui;
eFrame* frame;
eWidget* hello;

void myCallback(const eEvent* ev) {
  printf("callback! %d\n",ev->state);
  hello->x=ev->coord.x;
  hello->y=ev->coord.y;
}

int main(int argc, char** argv) {
  gui=new eEngine(eBackSDL2);
  frame=gui->newFrame();
  hello=frame->newWidget<eWidget>();
  hello->setSize(32,32);
  hello->x=64;
  hello->y=200;
  gui->setPostEventCallback(eEventMouseMove,myCallback);
  gui->pushFrame(frame);
  gui->show();
  return gui->run();
}
