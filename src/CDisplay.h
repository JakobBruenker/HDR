#ifndef CDISPLAY_H
#define CDISPLAY_H

struct CDisplay {
  private:
    cimg_library::CImgDisplay display;

  public:
    CDisplay(cimg_library::CImgDisplay disp) : display(disp) {};
    bool is_closed() { return display.is_closed(); }
    void wait() { display.wait(); }
};

#endif /* CDISPLAY_H */
