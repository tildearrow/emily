class eFrameView: public eWidget {
  sf::RenderTexture* tex;
  sf::View view;
  std::stack<eFrame*> frameStack;
  eFrame* curFrame;
  void (*leftClickCallback)();
  double xo, yo;
  int fw, fh;
  bool clicked;
  bool border;
  public:
    XPT int init();
    XPT int pushFrame(eFrame* f);
    XPT int popFrame();
    XPT int setSize(double w, double h);
    XPT int setBorder(bool enable);
    XPT int event(eEvent& ev);
    XPT int update();
    XPT int draw();
};
