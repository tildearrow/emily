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
eLight* light;

unsigned char sineVal;

double value;

eColor getRandomColor() {
  return {(float)(rand()&0xff)/256,(float)(rand()&0xff)/256,(float)(rand()&0xff)/256,1};
}

void myOtherCallback(eWidget* elem, int button, void* user) {
  printf("hello world!\n");
  but->setColor(getRandomColor());
  but->setSize(80+(rand()%64),32+(rand()%64));
  
  light->setLight(1);
}

void myOtherCallbackF(eWidget* elem, int button, void* user) {
  printf("hello world!\n");
  butf->setColor(getRandomColor());
  
  light->setLight(0);
}

void oneCallback(eMenuItem* mi, void* user) {
  printf("one\n");
}

void twoCallback(eWidget* elem, int button, void* user) {
  eContextItems i;
  i.add(eMenuItem("test 1",oneCallback,NULL));
  i.add(eMenuItem("test 2",oneCallback,NULL));
  printf("two\n");
  gui->popUpMenu(eAuto,eAuto,i);
}

void sineCallback() {
  sineVal++;
  hello->setX(210+sin(2*3.141592653589793238*((float)sineVal/256))*80);
}

void updateLabel(eWidget* elm, void* user) {
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
  but->setClickCallback(myOtherCallback,NULL);
  but->setPos(100,200);
  
  butf=frame->newWidget<eButton>();
  butf->setLabel("Flat");
  butf->setColor(getRandomColor());
  butf->setSize(80,32);
  butf->setStyle(eButtonFlat);
  butf->setClickCallback(myOtherCallbackF,NULL);
  butf->setAlign(1,0);
  butf->setDispPos(1,0);
  butf->setPos(-10,10);
  
  slider=frame->newWidget<eSlider>();
  slider->setTrack(value);
  slider->setRange(-1,1);
  slider->setSize(128,16);
  slider->setHandleColor({0,0.75,1,1});
  slider->setHandleSize(4);
  slider->setValueCallback(updateLabel,NULL);
  slider->setPos(8,8);
  
  frame1=gui->newFrame();
  
  but1=frame1->newWidget<eButton>();
  but1->setColor({1,1,0,1});
  but1->setSize(64,32);
  but1->setLabel("one");
  but1->setClickCallback(twoCallback,NULL);
  but1->setPos(4,4);
  
  but2=frame1->newWidget<eButton>();
  but2->setColor({1,1,0,1});
  but2->setSize(64,32);
  but2->setLabel("two");
  but2->setClickCallback(twoCallback,NULL);
  but2->setPos(72,4);
  
  fview=frame->newWidget<eFrameView>();
  fview->setSize(256,40);
  fview->pushFrame(frame1);
  fview->setPos(32,32);
  
  light=frame->newWidget<eLight>();
  light->setSize(96,96);
  light->setMaterial(eLightRubber);
  light->setMaterialProps(0.1);
  light->setClickCallback(myOtherCallback,NULL);
  light->setClickCancelCallback(myOtherCallback,NULL);
  light->setClickAltCallback(myOtherCallbackF,NULL);
  light->setPos(256,192);
  light->setLightProps(0.3333333,0);
  
  gui->setPreDrawCallback(sineCallback);
  mdisp->pushFrame(frame);
  return gui->run();
}
