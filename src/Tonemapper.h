#ifndef TONEMAPPER_H
#define TONEMAPPER_H

#include <CImg.h>

#include "CDisplay.h"

class Tonemapper {
  double* pixels;
  unsigned int imWidth;
  unsigned int imHeight;

public:
  CDisplay showImage();
  Tonemapper(unsigned int width, unsigned int height, double* pix);
  ~Tonemapper();
};

#endif /* TONEMAPPER.H */
