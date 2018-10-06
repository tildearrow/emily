class eLabel: public eWidget {
  string text;
  double tsize;
  sf::Text* inst;
  public:
    bool scrolls;
    XPT int init();
    XPT int calcBounds();
    XPT int setString(string data);
    XPT int setTextSize(double size);
    XPT int draw();
};
