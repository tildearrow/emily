enum eButtonStyles {
  eButtonNormal=0,
  eButtonFlat=1
};

// temporary
#define iconSpacing 4

class eButton: public eWidget {
  string label;
  eIcon* icon;
  sf::Text* linst;
  sf::Texture* tinst;
  sf::Sprite sinst;
  sf::Sprite sinstHigh;
  sf::Sprite sinstClick;
  eColor color;
  float highlight;
  void (*leftClickCallback)();
  double xo, yo;
  int fw, fh;
  int bstyle;
  bool clicked;
  int atrList[8];
  bool regenGraphics;
  char iconPlace;
  public:
    XPT int init();
    XPT int setLabel(string data);
    XPT int setLabelSize(double size);
    XPT int setIcon(eIcons index, double size, eDirection placement);
    XPT int setSize(double w, double h);
    XPT int setStyle(eButtonStyles style);
    XPT int setColor(eColor col);
    XPT int event(eEvent& ev);
    XPT int setCallback(void (*callback)());
    XPT int update();
    XPT int draw();
};
