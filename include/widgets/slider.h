class eSlider: public eWidget {
  sf::Texture* tinstBack;
  sf::Texture* tinstHandle;
  sf::Sprite sinstBack;
  sf::Sprite sinstHandle;
  eColor color;
  float highlight;
  int curType;
  struct {
    float min, max;
    float* val;
  } f;
  struct {
    double min, max;
    double* val;
  } d;
  struct {
    int min, max;
    int* val;
  } i;
  struct {
    short min, max;
    short* val;
  } s;
  struct {
    char min, max;
    char* val;
  } c;
  void (*holdCallback)();
  void (*releaseCallback)();
  void (*valueCallback)();
  double xo, yo;
  int fw, fh;
  bool clicked;
  public:
    XPT int init();
    XPT int setHandleSize(double w, double h);
    XPT int setSize(double w, double h);
    XPT int setColor(eColor col);
    XPT int event(eEvent& ev);
    XPT int setHoldCallback(void (*callback)());
    XPT int setReleaseCallback(void (*callback)());
    XPT int setValueCallback(void (*callback)());
    XPT int update();
    XPT int draw();
};
