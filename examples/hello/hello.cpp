#include <stdio.h>
#include <unistd.h>
#include "toolkit.h"

eEngine* gui;
eFrame* frame;
eLabel* hello;
eButton* but;

void myCallback(const eEvent* ev) {
  hello->x=ev->coord.x;
  hello->y=ev->coord.y;
}

void myOtherCallback() {
  printf("hello world!\n");
}

int main(int argc, char** argv) {
  gui=new eEngine(480,320);
  gui->show();
  frame=gui->newFrame();
  hello=(eLabel*)frame->newWidget<eLabel>();
  hello->setString("hello world!");
  hello->x=64;
  hello->y=200;
  but=(eButton*)frame->newWidget<eButton>();
  but->setLabel("Button");
  but->setColor({32,32,32,255});
  but->setSize(80,32);
  but->setCallback(myOtherCallback);
  but->x=64;
  but->y=200;
  gui->setPostEventCallback(eEventMouseMove,myCallback);
  gui->pushFrame(frame);
  return gui->run();
}
