#include "toolkit.h"

int eLight::init() {
  highlight=0;
  clicked=false;
  linst=new sf::Text();
  linst->setFont(engine->defFont->inst);
  linst->setCharacterSize(12*engine->scale);
  tinst=NULL;
  icon=NULL;
  _regenGraphics=true;
  matLight=0.05;
  setSize(32,32);
  //setStyle(eLightNormal);
  return 1;
}

int eLight::setSize(double wi, double he) {
  w=wi;
  h=he;
  _regenGraphics=true;
  calcBounds();
  return 1;
}

/*
int eLight::setStyle(eLightStyles style) {
  bstyle=style;
  _regenGraphics=true;
  return 1;
}
*/

int eLight::setLabel(string data) {
  label=data;
  linst->setString(data);
  return 1;
}

int eLight::setIcon(eIcons index, double size, eDirection placement) {
  if (icon!=NULL) {
    delete icon;
  }
  icon=engine->newIcon(index,size);
  iconPlace=placement;
  return 1;
}

int eLight::event(eEvent& ev) {
  if (ev.type==eEventMouseButton) {
    if (ev.state==1) {
      clicked=true;
      if (mouseClickCallback!=NULL) {
        mouseClickCallback(this,ev.input);
      }
    } else {
      if (clicked) {
        if (_collision) {
          if (mouseClickAltCallback!=NULL) {
            mouseClickAltCallback(this,ev.input);
          }
        } else {
          if (mouseClickCancelCallback!=NULL) {
            mouseClickCancelCallback(this,ev.input);
          }
        }
        clicked=false;
      }
    }
  }
  return 1;
}

/*
int eLight::setCallback(void (*callback)()) {
  leftClickCallback=callback;
  return 1;
}
*/

int eLight::draw() {
  int start, end;
  double wh;
  if (_regenGraphics) {
    if (tinst!=NULL) {
      delete tinst;
      tinst=NULL;
    }
    ((float*)atrList)[0]=matLight;
    ((int*)atrList)[1]=material;
    start=perfCount();
    tinst=engine->skin->getTexture(eObjectLight,atrList,w,h,&xo,&yo,&fw,&fh);
    end=perfCount();
    sinst.setTexture(*tinst);
    sinst.setTextureRect(sf::IntRect(0,0,fw,fh));
    sinst.setOrigin(sf::Vector2f(xo,yo));
  
    sinstLight.setTexture(*tinst);
    sinstLight.setTextureRect(sf::IntRect(fw,0,fw,fh));
    sinstLight.setOrigin(sf::Vector2f(xo,yo));
    /*
    
    sinstClick.setTexture(*tinst);
    sinstClick.setTextureRect(sf::IntRect(fw*2,0,fw,fh));
    sinstClick.setOrigin(sf::Vector2f(xo,yo));
    */
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
  sinst.setPosition(bLeft*engine->scale,bTop*engine->scale);
  /*sinstHigh.setPosition(bLeft*engine->scale,bTop*engine->scale);
  sinstClick.setPosition(bLeft*engine->scale,bTop*engine->scale);*/
  engine->win->draw(sinst);
  /*sinstHigh.setColor(sf::Color(255,255,255,highlight*255));
  engine->win->draw(sinstHigh);
  if (clicked && _collision) {
    engine->win->draw(sinstClick);
  }*/
  if (icon!=NULL) {
    switch (iconPlace) {
      case eLeft:
        wh=icon->width()+linst->getLocalBounds().width+iconSpacing*engine->scale;
        icon->setPos((bLeft+w/2)*engine->scale-wh/2,(bTop+h/2)*engine->scale-icon->height()/2);
        linst->setPosition((int)(((bLeft+w/2)*engine->scale)-(wh/2)+icon->width()+iconSpacing*engine->scale),
                           (int)round((bTop+h/2)*engine->scale-(linst->getCharacterSize()*0.667)));
        break;
      case eRight:
        wh=icon->width()+linst->getLocalBounds().width+iconSpacing*engine->scale;
        icon->setPos((bLeft+w/2)*engine->scale-wh/2+linst->getLocalBounds().width+iconSpacing*engine->scale,(bTop+h/2)*engine->scale-icon->height()/2);
        linst->setPosition((int)(((bLeft+w/2)*engine->scale)-(wh/2)),
                           (int)round((bTop+h/2)*engine->scale-(linst->getCharacterSize()*0.667)));
        break;
      case eUp:
        // TODO
        break;
      case eDown:
        // TODO
        break;
      case eCenter:
        icon->setPos((bLeft+w/2)*engine->scale-icon->width()/2,(bTop+h/2)*engine->scale-icon->height()/2);
        linst->setPosition((int)(((bLeft+w/2)*engine->scale)-(linst->getLocalBounds().width/2)),
                           (int)round((bTop+h/2)*engine->scale-(linst->getCharacterSize()*0.667)));
        break;
    }
    icon->draw();
  } else {
    linst->setPosition((int)(((bLeft+w/2)*engine->scale)-(linst->getLocalBounds().width/2)),
                       (int)round((bTop+h/2)*engine->scale-(linst->getCharacterSize()*0.667)));
  }
  engine->win->draw(*linst);
  if (highlight>0 && highlight<1) {
    return 1;
  }
  return 0;
}
