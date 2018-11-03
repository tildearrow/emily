class eMenuBar: public eWidget {
  public:
    struct Choice {
      string name;
      bool otherSide;
      bool notImmediate;
      bool callbackOrContent;
      unsigned int ordNum;
      void (*callback)(eWidget*,Choice*,float,float,void*);
      eContextItems content;
      
      Choice(string name, void (*callback)(eWidget*,Choice*,float,float,void*), bool otherSide=false, bool notImmediate=false);
      Choice(string name, eContextItems content, bool otherSide=false, bool notImmediate=false);
    };
  private:
    sf::Text* linst;
    eColor color;
    float highlight;
    double xo, yo;
    int fw, fh;
    int bstyle;
    bool clicked;
    int atrList[8];
    char iconPlace;
    std::vector<Choice> items;
  public:
    XPT int init();
    XPT int addChoice(Choice c);
    XPT int setSize(double w, double h);
    XPT int setStyle(eButtonStyles style);
    XPT int setColor(eColor col);
    XPT int event(eEvent& ev);
    XPT int update();
    XPT int draw();
};
