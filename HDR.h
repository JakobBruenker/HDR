#ifndef HDR_H
#define HDR_H

#include <CImg.h>

class HDR {
  cimg_library::CImg<unsigned char>* images;
  cimg_library::CImg<double>* xs;
  double* times;
  unsigned int numImages;
  unsigned int cardsR[256];
  unsigned int cardsG[256];
  unsigned int cardsB[256];
  double bigIR[256];
  double bigIG[256];
  double bigIB[256];
  double weights[256];

  void loadImages();
  void initCards();
  void initBigIs();
  void normBigIs();
  void initWeights();
  void loadImage(unsigned int imageNum, std::string);
  double estimateX(unsigned int x, unsigned int y, unsigned int dim);
  std::string getHdrgen();

public:
  HDR();
  ~HDR();
  void estimateXs();
  void drawResults();
  void estimateBigIs();
  cimg_library::CImgDisplay drawGraph();
  cimg_library::CImgDisplay showXs();
};

#endif /* HDR_H */
