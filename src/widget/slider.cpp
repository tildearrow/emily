#include "toolkit.h"

int eSlider::init() {
  highlight=0;
  clicked=false;
  active=false;
  val=NULL;
  tinstBack=NULL;
  tinstHandle=NULL;
  setBackColor(engine->skin->getDefaultColor(eObjectSliderB));
  setHandleColor(engine->skin->getDefaultColor(eObjectSliderH));
  setSize(128,24);
  setHandleSize(6);
  setRange(0,1);
  setHighlightArea(eSliderLeft);
  return 1;
}

int eSlider::setBackColor(eColor col) {
  bcolor=col;
  return 1;
}

int eSlider::setHandleColor(eColor col) {
  hcolor=col;
  return 1;
}

int eSlider::setHandleSize(double rad) {
  int atrList[8];
  hrad=rad;
  if (tinstHandle!=NULL) {
    delete tinstHandle;
    tinstHandle=NULL;
  }
  ((eColor*)atrList)->r=hcolor.r;
  ((eColor*)atrList)->g=hcolor.g;
  ((eColor*)atrList)->b=hcolor.b;
  ((eColor*)atrList)->a=hcolor.a;
  tinstHandle=engine->skin->getTexture(eObjectSliderH,atrList,rad*2,h,&hxo,&hyo,&hfw,&hfh);
  sinstHandle.setTexture(*tinstHandle);
  sinstHandle.setOrigin(sf::Vector2f(hxo,hyo));
  return 1;
}

int eSlider::setSize(double wi, double he) {
  int atrList[8];
  w=wi;
  h=he;
  if (tinstBack!=NULL) {
    delete tinstBack;
    tinstBack=NULL;
  }
  ((eColor*)atrList)->r=bcolor.r;
  ((eColor*)atrList)->g=bcolor.g;
  ((eColor*)atrList)->b=bcolor.b;
  ((eColor*)atrList)->a=bcolor.a;
  tinstBack=engine->skin->getTexture(eObjectSliderB,atrList,w,engine->scale,&bxo,&byo,&bfw,&bfh);
  sinstBack.setTexture(*tinstBack);
  sinstBack.setOrigin(sf::Vector2f(bxo,byo));
  calcBounds();
  return 1;
}

int eSlider::setTrack(double& tval) {
  val=&tval;
  return 1;
}

int eSlider::setRange(double tmin, double tmax) {
  min=tmin;
  max=tmax;
  return 1;
}

int eSlider::event(eEvent& ev) {
  double rpos;
  switch (ev.type) {
    case eEventMouseButton:
      if (ev.state==1) {
        engine->grabMouse(true);
        _wantsAllEvents=true;
        rpos=w*((*val-min)/(max-min));
        if (ev.coord.x>(x+rpos-hrad) && ev.coord.x<(x+rpos+hrad) &&
            ev.coord.y>(y) && ev.coord.y<(y+h)) {
          active=true;
          if (mouseClickCallback!=NULL) {
            mouseClickCallback(this,ev.input,mouseClickUser);
          }
        }
      } else {
        engine->grabMouse(false);
        _wantsAllEvents=false;
        if (active) {
          active=false;
          if (mouseClickAltCallback!=NULL) {
            mouseClickAltCallback(this,ev.input,mouseClickAltUser);
          }
        }
      }
      break;
    case eEventMouseMove:
      if (active) {
        *val=min+(max-min)*((ev.coord.x-x)/w);
        if (*val<min) {
          *val=min;
        }
        if (*val>max) {
          *val=max;
        }
        if (valueCallback!=NULL) {
          valueCallback(this,valueUser);
        }
      }
      break;
  }
  return 1;
}

int eSlider::setHighlightArea(unsigned char area) {
  harea=area;
  return 1;
}

int eSlider::draw() {
  double rpos;
  if (_collision || active) {
    highlight+=0.125;
  } else {
    highlight-=0.0625;
  }
  if (highlight>1) highlight=1;
  if (highlight<0) highlight=0;
  if (val!=NULL) {
    rpos=w*((*val-min)/(max-min));
    // left side
    sinstBack.setTextureRect(sf::IntRect(0,(harea&eSliderLeft)?(bfh):(0),bxo+rpos*engine->scale,bfh));
    sinstBack.setPosition(x*engine->scale,(y+h/2)*engine->scale);
    engine->win->draw(sinstBack);
    // right side
    sinstBack.setTextureRect(sf::IntRect(bxo+rpos*engine->scale,(harea&eSliderRight)?(bfh):(0),bfw-((rpos)*engine->scale-bxo),bfh));
    sinstBack.setPosition((x+rpos)*engine->scale+bxo,(y+h/2)*engine->scale);
    engine->win->draw(sinstBack);
    
    // highlight
    if (highlight>0) {
      if (harea&eSliderLeft) {
        sinstBack.setTextureRect(sf::IntRect(0,(harea&eSliderLeft)?(bfh):(0),bxo+rpos*engine->scale,bfh));
        sinstBack.setPosition(x*engine->scale,(y+h/2)*engine->scale);
        sinstBack.setColor(sf::Color(255,255,255,highlight*255));
        engine->win->draw(sinstBack);
        sinstBack.setColor(sf::Color(255,255,255,255));
      }
      if (harea&eSliderRight) {
        sinstBack.setTextureRect(sf::IntRect(bxo+rpos*engine->scale,(harea&eSliderRight)?(bfh):(0),bfw-((rpos)*engine->scale-bxo),bfh));
        sinstBack.setPosition((x+rpos)*engine->scale+bxo,(y+h/2)*engine->scale);
        sinstBack.setColor(sf::Color(255,255,255,highlight*255));
        engine->win->draw(sinstBack);
        sinstBack.setColor(sf::Color(255,255,255,255));
      }
    }
    
    sinstHandle.setPosition((x+rpos-hrad)*engine->scale,(y)*engine->scale);
    engine->win->draw(sinstHandle);
    //engine->frect(,x+rpos+hrad,y+h/2+hrad);
  }
  return 0;
}
