class eLabel: public eWidget {
  string text;
  sf::Text* inst;
  public:
    bool scrolls;
    XPT int init();
    XPT int setString(string data);
    XPT int setTextSize(double size);
    XPT int draw();
};
