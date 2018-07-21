enum eSliderHighlight {
  eSliderLeft=1,
  eSliderRight=2  
};

class eSlider: public eWidget {
  sf::Texture* tinstBack;
  sf::Texture* tinstHandle;
  sf::Sprite sinstBack;
  sf::Sprite sinstBackI;
  sf::Sprite sinstBackH;
  sf::Sprite sinstHandle;
  sf::Sprite sinstHandleH;
  sf::Sprite sinstHandleC;
  eColor bcolor, hcolor;
  float highlight;
  int curType;
  double min, max;
  double* val;
  double bxo, byo, hxo, hyo;
  double hrad;
  int bfw, bfh, hfw, hfh;
  unsigned char harea;
  bool clicked, active;
  std::vector<double> attPoints;
  public:
    XPT int init();
    XPT int setHandleSize(double rad);
    XPT int setSize(double w, double h);
    XPT int setRange(double min, double max);
    XPT int setTrack(double& val);
    XPT int setBackColor(eColor col);
    XPT int setHandleColor(eColor col);
    XPT int event(eEvent& ev);
    XPT int addAttPoint(double val);
    XPT int delAttPoint(int which);
    XPT double getAttPoint(int which);
    XPT int attPointCount();
    XPT int setHighlightArea(unsigned char area);
    XPT int update();
    XPT int draw();
};
