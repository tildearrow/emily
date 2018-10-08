#include <stdio.h>
#include <math.h>
//#include <unistd.h>
#include "toolkit.h"

eEngine* gui;
eFrame* frame;
eFrame* frame1;
eFrame* frame2;
eLabel* hello;
eButton* but;
eButton* but1;
eButton* but2;
eFrameView* fview;
eFrameView* fview1;
eVideoOut* mdisp;

eColor getRandomColor() {
  return {(float)(rand()&0xff)/256,(float)(rand()&0xff)/256,(float)(rand()&0xff)/256,1};
}

int main(int argc, char** argv) {
  gui=new eEngine();
  srand(perfCount());
  mdisp=gui->newVideoOut(640,480);
  frame=gui->newFrame();
  
  hello=frame->newWidget<eLabel>();
  hello->setString("Lorem ipsum dolor sit amet");
  hello->setTextSize(20);
  hello->setPos(0,10);
  hello->setAlign(0.5,0);
  hello->setDispPos(0.5,0);
  
  but=frame->newWidget<eButton>();
  but->setLabel("Ut enim");
  but->setColor({1,1,0,1});
  but->setSize(80,32);
  but->setIcon(eIconPlane,12,eLeft);
  but->setPos(100,400);
  
  frame1=gui->newFrame();
  frame2=gui->newFrame();
  
  but1=frame1->newWidget<eButton>();
  but1->setLabel("Quis autem");
  but1->setColor({0.5,0.5,0.5,1});
  but1->setSize(100,32);
  but1->setIcon(eIconKey,12,eLeft);
  but1->setPos(10,10);
  
  but2=frame2->newWidget<eButton>();
  but2->setLabel("natus error");
  but2->setColor({0.3,0.3,0.5,1});
  but2->setSize(100,32);
  but2->setIcon(eIconExclamation,12,eLeft);
  but2->setPos(10,10);
  
  fview=frame->newWidget<eFrameView>();
  fview->setSize(384,256);
  fview->pushFrame(frame1);
  fview->setPos(8,40);
  
  // here goes the craze.
  fview1=frame1->newWidget<eFrameView>();
  fview1->setSize(256,128);
  fview1->pushFrame(frame2);
  fview1->setPos(8,40);
  
  mdisp->pushFrame(frame);
  return gui->run();
}
