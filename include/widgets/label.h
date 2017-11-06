class eLabel: public eWidget {
  string text;
  sf::Text* inst;
  public:
    bool scrolls;
    int init();
    int setString(string data);
    int setSize(double size);
    int draw();
};
