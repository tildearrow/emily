#include "toolkit.h"

int eButton::init() {
  highlight=0;
  clicked=false;
  linst=new sf::Text();
  linst->setFont(engine->defFont->inst);
  linst->setCharacterSize(12*engine->scale);
  tinst=NULL;
  leftClickCallback=NULL;
  icon=NULL;
  _regenGraphics=true;
  setColor(engine->skin->getDefaultColor(eObjectButton));
  setSize(12,32);
  setStyle(eButtonNormal);
  return 1;
}

int eButton::setSize(double wi, double he) {
  w=wi;
  h=he;
  _regenGraphics=true;
  return 1;
}

int eButton::setStyle(eButtonStyles style) {
  bstyle=style;
  _regenGraphics=true;
  return 1;
}

int eButton::setLabel(string data) {
  label=data;
  linst->setString(data);
  return 1;
}

int eButton::setIcon(eIcons index, double size, eDirection placement) {
  if (icon!=NULL) {
    delete icon;
  }
  icon=engine->newIcon(index,size);
  iconPlace=placement;
  return 1;
}

int eButton::setColor(eColor col) {
  color=col;
  _regenGraphics=true;
  return 1;
}

int eButton::event(eEvent& ev) {
  if (ev.type==eEventMouseButton) {
    if (ev.state==1) {
      clicked=true;
    } else {
      if (clicked) {
        if (_collision) {
          if (leftClickCallback!=NULL) {
            leftClickCallback();
          }
        }
        clicked=false;
      }
    }
  }
  return 1;
}

int eButton::setCallback(void (*callback)()) {
  leftClickCallback=callback;
  return 1;
}

int eButton::draw() {
  int start, end;
  double wh;
  if (_regenGraphics) {
    if (tinst!=NULL) {
      delete tinst;
      tinst=NULL;
    }
    ((eColor*)atrList)->r=color.r;
    ((eColor*)atrList)->g=color.g;
    ((eColor*)atrList)->b=color.b;
    ((eColor*)atrList)->a=color.a;
    atrList[4]=bstyle;
    start=perfCount();
    tinst=engine->skin->getTexture(eObjectButton,atrList,w,h,&xo,&yo,&fw,&fh);
    end=perfCount();
    sinst.setTexture(*tinst);
    sinst.setTextureRect(sf::IntRect(0,0,fw,fh));
    sinst.setOrigin(sf::Vector2f(xo,yo));
  
    sinstHigh.setTexture(*tinst);
    sinstHigh.setTextureRect(sf::IntRect(fw,0,fw,fh));
    sinstHigh.setOrigin(sf::Vector2f(xo,yo));
    
    sinstClick.setTexture(*tinst);
    sinstClick.setTextureRect(sf::IntRect(fw*2,0,fw,fh));
    sinstClick.setOrigin(sf::Vector2f(xo,yo));
    _regenGraphics=false;
    
    printf("time: %d\n",end-start);
  }
  
  if (_collision) {
    highlight+=0.125;
  } else {
    highlight-=0.0625;
  }
  if (highlight>1) highlight=1;
  if (highlight<0) highlight=0;
  // temporary
  /*
  if (clicked && _collision) {
    engine->drawColor({1,1,1,1});
  } else {
    engine->drawColor({0.25f*highlight,0.25f*highlight,0.25f*highlight,1});
  }
  engine->frect(x,y,x+w,y+h);
  engine->drawColor({1,1,1,1});
  engine->rect(x,y,x+w,y+h);
  */
  sinst.setPosition(x*engine->scale,y*engine->scale);
  sinstHigh.setPosition(x*engine->scale,y*engine->scale);
  sinstClick.setPosition(x*engine->scale,y*engine->scale);
  engine->win->draw(sinst);
  sinstHigh.setColor(sf::Color(255,255,255,highlight*255));
  engine->win->draw(sinstHigh);
  if (clicked && _collision) {
    engine->win->draw(sinstClick);
  }
  if (icon!=NULL) {
    switch (iconPlace) {
      case eLeft:
        wh=icon->width()+linst->getLocalBounds().width+iconSpacing*engine->scale;
        icon->setPos((x+w/2)*engine->scale-wh/2,(y+h/2)*engine->scale-icon->height()/2);
        linst->setPosition((int)(((x+w/2)*engine->scale)-(wh/2)+icon->width()+iconSpacing*engine->scale),
                           (int)round((y+h/2)*engine->scale-(linst->getCharacterSize()*0.667)));
        break;
      case eRight:
        wh=icon->width()+linst->getLocalBounds().width+iconSpacing*engine->scale;
        icon->setPos((x+w/2)*engine->scale-wh/2+linst->getLocalBounds().width+iconSpacing*engine->scale,(y+h/2)*engine->scale-icon->height()/2);
        linst->setPosition((int)(((x+w/2)*engine->scale)-(wh/2)),
                           (int)round((y+h/2)*engine->scale-(linst->getCharacterSize()*0.667)));
        break;
      case eUp:
        // TODO
        break;
      case eDown:
        // TODO
        break;
      case eCenter:
        icon->setPos((x+w/2)*engine->scale-icon->width()/2,(y+h/2)*engine->scale-icon->height()/2);
        linst->setPosition((int)(((x+w/2)*engine->scale)-(linst->getLocalBounds().width/2)),
                           (int)round((y+h/2)*engine->scale-(linst->getCharacterSize()*0.667)));
        break;
    }
    icon->draw();
  } else {
    linst->setPosition((int)(((x+w/2)*engine->scale)-(linst->getLocalBounds().width/2)),
                       (int)round((y+h/2)*engine->scale-(linst->getCharacterSize()*0.667)));
  }
  engine->win->draw(*linst);
  return 0;
}
