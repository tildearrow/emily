#include <stdio.h>
#include <math.h>
//#include <unistd.h>
#include "toolkit.h"

eEngine* gui;
eFrame* frame;
eFrame* frame1;
eLabel* hello;
eButton* but;
eButton* butf;
eFrameView* fview;
eButton* but1;
eButton* but2;
eSlider* slider;
eVideoOut* mdisp;

unsigned char sineVal;

double value;

eColor getRandomColor() {
  return {(float)(rand()&0xff)/256,(float)(rand()&0xff)/256,(float)(rand()&0xff)/256,1};
}

void myOtherCallback() {
  printf("hello world!\n");
  but->setColor(getRandomColor());
  but->setSize(80+(rand()%64),32+(rand()%64));
}

void myOtherCallbackF() {
  printf("hello world!\n");
  butf->setColor(getRandomColor());
}

void oneCallback() {
  printf("one\n");
}

void twoCallback() {
  eContextMenu* menu;
  menu=new eContextMenu;
  menu->addItem(eMenuItem("test 1",oneCallback));
  menu->addItem(eMenuItem("test 2",oneCallback));
  printf("two\n");
  gui->popUpMenu(eAuto,eAuto,menu);
}

void sineCallback() {
  sineVal++;
  hello->setX(210+sin(2*3.141592653589793238*((float)sineVal/256))*80);
}

void updateLabel() {
  hello->setString(strFormat("%f",value));
}

int main(int argc, char** argv) {
  gui=new eEngine();
  srand(perfCount());
  mdisp=gui->newVideoOut(480,320);
  frame=gui->newFrame();
  
  hello=frame->newWidget<eLabel>();
  hello->setString("hello world!");
  hello->setPos(64,64);
  
  but=frame->newWidget<eButton>();
  but->setLabel("Normal");
  but->setColor(getRandomColor());
  but->setSize(80,32);
  but->setIcon(eIconAdjust,12,eLeft);
  but->setCallback(myOtherCallback);
  but->setPos(100,200);
  
  butf=frame->newWidget<eButton>();
  butf->setLabel("Flat");
  butf->setColor(getRandomColor());
  butf->setSize(80,32);
  butf->setStyle(eButtonFlat);
  butf->setCallback(myOtherCallbackF);
  butf->setAlign(1,0);
  butf->setDispPos(1,0);
  butf->setPos(-10,10);
  
  slider=frame->newWidget<eSlider>();
  slider->setTrack(value);
  slider->setRange(-1,1);
  slider->setSize(128,16);
  slider->setHandleColor({0,0.75,1,1});
  slider->setHandleSize(4);
  slider->setValueCallback(updateLabel);
  slider->setPos(8,8);
  
  frame1=gui->newFrame();
  
  but1=frame1->newWidget<eButton>();
  but1->setColor({1,1,0,1});
  but1->setSize(64,32);
  but1->setLabel("one");
  but1->setCallback(oneCallback);
  but1->setPos(4,4);
  
  but2=frame1->newWidget<eButton>();
  but2->setColor({1,1,0,1});
  but2->setSize(64,32);
  but2->setLabel("two");
  but2->setCallback(twoCallback);
  but2->setPos(72,4);
  
  fview=frame->newWidget<eFrameView>();
  fview->setSize(256,40);
  fview->pushFrame(frame1);
  fview->setPos(32,32);
  
  gui->setPreDrawCallback(sineCallback);
  mdisp->pushFrame(frame);
  return gui->run();
}
