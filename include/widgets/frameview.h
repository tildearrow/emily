class eFrameView: public eWidget, public eFrameDispatch {
  sf::RenderTexture* tex;
  sf::View view;
  eFrame* curFrame;
  double xo, yo;
  int fw, fh;
  bool clicked;
  bool border;
  public:
    XPT int init();
    XPT int getWidth();
    XPT int getHeight();
    XPT int setSize(double w, double h);
    XPT int setBorder(bool enable);
    XPT int setView(float x, float y, float w, float h);
    XPT int resetView();
    XPT int event(eEvent& ev);
    XPT int update();
    XPT int draw();
};
