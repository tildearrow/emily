#include <stdio.h>
#include <math.h>
//#include <unistd.h>
#include "toolkit.h"

eEngine* gui;
eFrame* frame;
eLabel* hello;
eButton* but;

unsigned char sineVal;

void myOtherCallback() {
  printf("hello world!\n");
  but->setColor({(float)(rand()&0xff)/256,(float)(rand()&0xff)/256,(float)(rand()&0xff)/256,1});
  but->setSize(80,32);
}

void sineCallback() {
  sineVal++;
  hello->x=210+sin(2*3.141592653589793238*((float)sineVal/256))*80;
}

int main(int argc, char** argv) {
  gui=new eEngine(480,320);
  gui->show();
  frame=gui->newFrame();
  hello=(eLabel*)frame->newWidget<eLabel>();
  hello->setString("hello world!");
  hello->x=64;
  hello->y=64;
  but=(eButton*)frame->newWidget<eButton>();
  but->setLabel("Button");
  but->setColor({0,0.75,1,1});
  but->setSize(80,32);
  but->setCallback(myOtherCallback);
  but->x=200;
  but->y=200;
  gui->setPreDrawCallback(sineCallback);
  gui->pushFrame(frame);
  return gui->run();
}
