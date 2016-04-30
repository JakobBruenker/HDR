#ifndef HDR_H
#define HDR_H

#include <CImg.h>
#include <OpenEXR/ImfRgbaFile.h>

#include "CDisplay.h"

class HDR {
  cimg_library::CImg<unsigned char>* images;
  cimg_library::CImg<double>* xs;
  double* times;
  unsigned int numImages;
  unsigned int cards[3][256];
  double bigI[3][256];
  double weights[256];

  std::string getHdrgen();
  void loadImages();
  void initCards();
  void initBigIs();
  void normBigIs();
  void initWeights();
  void loadImage(unsigned int imageNum, std::string);
  double estimateX(unsigned int x, unsigned int y, unsigned int dim);
  unsigned char f(double light, unsigned int color);
  void getRGBA(Imf_2_2::Rgba* pixels);

public:
  HDR();
  ~HDR();
  void estimateXs();
  void drawResults();
  void estimateBigIs();
  void writeEXRFile();
  // buffer is a one-dimensional array with dimensions width*height*3
  void getLuminances(double*** buffer);
  unsigned int getWidth();
  unsigned int getHeight();
  CDisplay drawGraph();
  CDisplay showExposure(double time);
};

#endif /* HDR_H */
