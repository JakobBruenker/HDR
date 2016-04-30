#include <iostream>
#include <fstream>
#include <limits>

#include "HDR.h"

typedef unsigned char uchar;
typedef unsigned int uint;

using namespace std;
using namespace cimg_library;

void HDR::loadImages() {
  string hdrgenPath = getHdrgen();
  ifstream hdrgen(hdrgenPath.c_str());
  string directory = hdrgenPath.substr(0, hdrgenPath.rfind("/"));
  string line;
  uint numLines = 0;
  while (getline(hdrgen, line)) {
    uint spacePos = line.find(string(" "));
    times[numLines] = atof(line.substr(spacePos + 1, line.length()).c_str());
    loadImage(numLines, directory + "/" + line.substr(0, spacePos));
    numLines++;
  }
  hdrgen.close();
}

void HDR::initCards() {
  for (uint i = 0; i < numImages; i++) {
    for (uint x = 0; x < images[0].width(); x++) {
      for (uint y = 0; y < images[0].height(); y++) {
        cards[0][images[i](x,y,0,0)]++;
        cards[1][images[i](x,y,0,1)]++;
        cards[2][images[i](x,y,0,2)]++;
      }
    }
  }
}

void HDR::initBigIs() {
  for (uint i = 0; i < 255; i++) {
    bigI[0][i] = i / 128.;
    bigI[1][i] = i / 128.;
    bigI[2][i] = i / 128.;
  }
}

void HDR::normBigIs() {
  double bigIR_128 = bigI[0][128];
  double bigIG_128 = bigI[1][128];
  double bigIB_128 = bigI[2][128];
  for (uint i = 0; i < 255; i++) {
    bigI[0][i] = bigI[0][i] / bigIR_128;
    bigI[1][i] = bigI[1][i] / bigIG_128;
    bigI[2][i] = bigI[2][i] / bigIB_128;
  }
}

// could be done as a function instead of saved in an array, but this is
// probably slightly more performant in the long run
void HDR::initWeights() {
  // the weights for extreme pixel values are set to zero, because otherwise,
  // overexposed and underexposed areas are taken into account wrongly
  weights[0] = weights[253] = weights[254] = 0;
  for (uint i = 1; i < 254; i++) {
    weights[i] =
      exp((-4 * (i - 127.5) * (i - 127.5)) / (127.5 * 127.5));
  }
}

string HDR::getHdrgen() {
  string line;
  ifstream config("config.cfg");
  getline(config, line);
  config.close();
  return line;
}

void HDR::loadImage(uint imageNum, string filename) {
  images[imageNum] = CImg<uchar>(filename.c_str());
}

double HDR::estimateX(uint x, uint y, uint dim) {
  double numerator = 0;
  double denominator = 0;
  for (uint i = 0; i < numImages; i++) {
    uchar pixval = images[i](x, y, 0, dim);
    // since the weight would be zero if this is not the case, and nothing
    // would be added, we may as well just skip to the next pixel
    if (pixval > 0) {
      // again, the weight would be zero if this is case, and presumably, all
      // pixels in brighter images2 would also be zero, so we can just break the
      // whole loop
      if (pixval >= 254) {
        continue; // XXX replace with break when sorted
      } else {
        double wij = weights[pixval];
        numerator += (wij * bigI[dim][pixval]) / times[i];
        denominator += wij / (times[i] * times[i]);
      }
    }
  }
  return numerator / denominator;
}

void HDR::estimateXs() {
  for (uint x = 0; x < images[0].width(); x++) {
    for (uint y = 0; y < images[0].height(); y++) {
      double estR = estimateX(x, y, 0);
      double estG = estimateX(x, y, 1);
      double estB = estimateX(x, y, 2);
      double xval[] = {estR, estG, estB};
      xs->draw_point(x,y,xval);
    }
  }
}

void HDR::estimateBigIs() {
  double sumsR[256] = {0.};
  double sumsG[256] = {0.};
  double sumsB[256] = {0.};
  uchar numerator;
  for (uint i = 0; i < numImages; i++) {
    for (uint x = 0; x < images[0].width(); x++) {
      for (uint y = 0; y < images[0].height(); y++) {
        // have to save the numerator in a variable because of a strange bug
        // (times[i] will randomly become -nan otherwise for some images)
        numerator = (*xs)(x,y,0,0);
        sumsR[images[i](x,y,0,0)] += numerator / times[i];
        numerator = (*xs)(x,y,0,1);
        sumsG[images[i](x,y,0,1)] += numerator / times[i];
        numerator = (*xs)(x,y,0,2);
        sumsB[images[i](x,y,0,2)] += numerator / times[i];
      }
    }
  }
  for (uint i = 0; i < 255; i++) {
    bigI[0][i] = sumsR[i] / cards[0][i];
    bigI[1][i] = sumsG[i] / cards[1][i];
    bigI[2][i] = sumsB[i] / cards[2][i];
  }
  // normalize, so I_128 is 1
  normBigIs();
}

uchar HDR::f(double light, uint color) {
  if (light <= bigI[color][0]) {
    return 0;
  }
  for (uint i = 1; i < 255; i++) {
    if (light <= bigI[color][i]) {
      return i;
    }
  }
  return 255;
}

CImgDisplay HDR::showExposure(double time) {
  CImg<uchar> result(xs->width(), xs->height(), 1, 3, 0);
  for (uint x = 0; x < xs->width(); x++) {
    for (uint y = 0; y < xs->height(); y++) {
      uchar pixval[] = {f(time * (*xs)(x,y,0,0), 0),
                        f(time * (*xs)(x,y,0,1), 1),
                        f(time * (*xs)(x,y,0,2), 2)};
      result.draw_point(x,y,pixval);
    }
  }

  return CImgDisplay(result, "Image with exposure time of ");
}

CImgDisplay HDR::showXs() {
  return CImgDisplay(*xs, "HDR image mapped to eight bits");
}

CImgDisplay HDR::drawGraph() {
  // we know that I_128 is 1, so we can assume that the minimum value is at
  // most 1 and the maximum value is at least 1
  double minI = 1;
  double maxI = 1;
  for (uint i = 0; i < 254; i++) {
    for (uint j = 0; j < 3; j++) {
      if (bigI[j][i] > maxI) {
        maxI = bigI[j][i];
      } else if (bigI[j][i] < minI) {
        minI = bigI[j][i];
      }
    }
  }

  CImg<uchar> graph(510, 510, 1, 3, 0);

  struct local {
    static void drawData(CImg<uchar> *g, uint x, uint y, uint color) {
      uchar* col;
      uchar red[] = {255, 0, 0};
      uchar green[] = {0, 255, 0};
      uchar blue[] = {50, 50, 255};
      switch (color) {
        case 0: col = red; break;
        case 1: col = green; break;
        case 2: col = blue; break;
        default: col = red;
      }
      g->draw_point(x    , y    , col);
      g->draw_point(x + 1, y    , col);
      g->draw_point(x    , y + 1, col);
      g->draw_point(x + 1, y + 1, col);
    }
  };

  for (uint i = 0; i < 509; i += 2) {
    double den = (log(maxI) - log(minI)) / 510.;
    local::drawData(&graph, (log(bigI[0][i/2]) - log(minI)) / den, 510 - i, 0);
    local::drawData(&graph, (log(bigI[1][i/2]) - log(minI)) / den, 510 - i, 1);
    local::drawData(&graph, (log(bigI[2][i/2]) - log(minI)) / den, 510 - i, 2);
  }

  return CImgDisplay(graph, "Response curves");
}

HDR::HDR() {
  ifstream hdrgen(getHdrgen().c_str());
  numImages = 0;
  string line;
  cout << "hdrgen file:" << endl;
  while (getline(hdrgen, line)) {
    numImages++;
    cout << line << endl;
  }
  hdrgen.close();
  times = new double[numImages];
  images = new CImg<uchar>[numImages];
  loadImages();
  initCards();
  initBigIs();
  initWeights();
  xs = new CImg<double>(images[0].width(), images[0].height(), 1, 3, 0);
}

HDR::~HDR() {
  delete[] images;
  delete xs;
  delete[] times;
}
