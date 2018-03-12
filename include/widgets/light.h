enum eLightMaterials {
  eLightPlastic=0,
  eLightRubber=1
};

class eLight: public eWidget {
  string label;
  eIcon* icon;
  sf::Text* linst;
  sf::Texture* tinst;
  sf::Sprite sinst;
  sf::Sprite sinstLight;
  double hue, saturation;
  float bright;
  float highlight;
  void (*clickCallback)();
  double xo, yo;
  int fw, fh;
  int bstyle;
  bool clicked;
  bool invertLabel;
  int atrList[8];
  char iconPlace;
  public:
    XPT int init();
    XPT int setLabel(string data);
    XPT int setLabelSize(double size);
    XPT int setLabelStyle(bool inverted);
    XPT int setIcon(eIcons index, double size, eDirection placement);
    XPT int setSize(double w, double h);
    XPT int setMaterial(eLightMaterials material);
    XPT int setColor(eColor col);
    XPT int event(eEvent& ev);
    XPT int setCallback(void (*callback)());
    XPT int update();
    XPT int draw();
};
