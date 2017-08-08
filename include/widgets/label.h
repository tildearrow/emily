class eLabel: public eWidget {
  string text;
  eTexture* tex;
  public:
    bool scrolls;
    int setString(string data);
    int setSize(double w, double h);
    int draw();
};
