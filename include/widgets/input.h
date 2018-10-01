class eInput: public eWidget {
  sf::Text* linst;
  sf::Texture* tinst;
  sf::Sprite sinst;
  eColor color;
  float highlight;
  double xo, yo;
  int fw, fh;
  bool clicked;
  public:
    XPT int init();
    XPT int setSize(double w, double h);
    XPT int setColor(eColor col);
    XPT int event(eEvent& ev);
    XPT int update();
    XPT int draw();
};
