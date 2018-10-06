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
  sf::Sprite sinstMaterial;
  double hue, saturation;
  float bright;
  float highlight;
  float matLight;
  double xo, yo;
  int fw, fh;
  int material;
  bool clicked;
  bool invertLabel;
  int atrList[8];
  char iconPlace;
  eColor eLightPre;
  public:
    XPT int init();
    XPT int setLabel(string data);
    XPT int setLabelSize(double size);
    XPT int setLabelStyle(bool inverted);
    XPT int setIcon(eIcons index, double size, eDirection placement);
    XPT int setSize(double w, double h);
    XPT int setMaterial(eLightMaterials material);
    XPT int setMaterialProps(float light);
    XPT int setLightProps(double hue, double saturation);
    XPT int setLight(float bright);
    XPT int event(eEvent& ev);
    XPT int update();
    XPT int draw();
};
