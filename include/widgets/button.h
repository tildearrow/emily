class eButton: public eWidget {
  string label;
  sf::Text* linst;
  sf::Texture tinst;
  sf::Sprite sinst;
  eColor color;
  float highlight;
  void (*leftClickCallback)();
  bool clicked;
  public:
    int init();
    int setLabel(string data);
    int setLabelSize(double size);
    int setSize(double w, double h);
    int setColor(eColor col);
    int event(eEvent& ev);
    int setCallback(void (*callback)());
    int update();
    int draw();
};
