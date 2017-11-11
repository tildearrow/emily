class eLabel: public eWidget {
  string text;
  sf::Text* inst;
  public:
    bool scrolls;
    int init();
    int setString(string data);
    int setTextSize(double size);
    int draw();
};
