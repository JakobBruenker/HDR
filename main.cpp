#include <CImg.h>
#include <limits>
#include <fstream>
#include <iostream>
#include "HDR.h"
using namespace cimg_library;
using namespace std;

//
//
//
//
// TODO TODO TODO       TODO       TODO TODO            TODO      .
//      TODO       TODO      TODO  TODO      TODO  TODO      TODO .
//      TODO       TODO      TODO  TODO      TODO  TODO      TODO .
//      TODO       TODO      TODO  TODO      TODO  TODO      TODO .
//      TODO       TODO      TODO  TODO      TODO  TODO      TODO .
//      TODO       TODO      TODO  TODO      TODO  TODO      TODO .
//      TODO       TODO      TODO  TODO      TODO  TODO      TODO .
//      TODO            TODO       TODO TODO            TODO      .
//
//  CHOOSE A GROUP IN MOODLE AND SIGN UP!!!
//
//
//
//

/*CImg<unsigned char> graph(500, 255, 1, 3, 0);
CImg<unsigned char> result;
CImg<double> xs;
unsigned int cardsR[256];
unsigned int cardsG[256];
unsigned int cardsB[256];
double bigIR[256];
double bigIG[256];
double bigIB[256];

double weight(unsigned char pixval) {
  if (pixval <= 0 || pixval >= 254) {
    return 0;
  }
  return exp((-4 * (pixval - 127.5) * (pixval - 127.5)) / (127.5 * 127.5));
}

double estimateX(unsigned int numImages, CImg<unsigned char>* images,
    double* times,
    unsigned int x, unsigned int y, unsigned int dim) {
  double numerator = 0;
  double denominator = 0;
  for (unsigned int i = 0; i < numImages; i++) {
    unsigned char pixval = images[i](x, y, 0, dim);
    // since the weight would be zero if this is not the case, and nothing
    // would be added, we may as well just skip to the next pixel
    if (pixval > 0) {
      // again, the weight would be zero if this is case, and presumably, all
      // pixels in brighter images2 would also be zero, so we can just break the
      // whole loop
      if (pixval >= 254) {
        continue; // XXX replace with break when sorted
      } else {
        double wij = weight(pixval);
        double bigI;
        switch (dim) {
          case 0: bigI = bigIR[pixval]; break;
          case 1: bigI = bigIG[pixval]; break;
          case 2: bigI = bigIB[pixval]; break;
          default: bigI = bigIR[pixval];
        }
        numerator += (wij * bigI) / times[i];
        denominator += wij / (times[i] * times[i]);
      }
    }
  }
  return numerator / denominator;
}

unsigned char normEst(double est, double min, double max) {
  return (unsigned char)(255 * ((est - min) / (max - min)));
}

unsigned char fR(double light) {
  if (light <= bigIR[0]) {
    return 0;
  }
  for (unsigned int i = 1; i < 256; i++) {
    if (light <= bigIR[i]) {
      return i;
    }
  }
  return 255;
}

unsigned char fG(double light) {
  if (light <= bigIG[0]) {
    return 0;
  }
  for (unsigned int i = 1; i < 256; i++) {
    if (light <= bigIG[i]) {
      return i;
    }
  }
  return 255;
}

unsigned char fB(double light) {
  if (light <= bigIB[0]) {
    return 0;
  }
  for (unsigned int i = 1; i < 256; i++) {
    if (light <= bigIB[i]) {
      return i;
    }
  }
  return 255;
}

unsigned char expEstR(double est, double time) {
  return fR(time * est);
}

unsigned char expEstG(double est, double time) {
  return fG(time * est);
}

unsigned char expEstB(double est, double time) {
  return fB(time * est);
}

void initBigIs() {
  for (unsigned int i = 0; i < 255; i++) {
    bigIR[i] = i / 128.;
    bigIG[i] = i / 128.;
    bigIB[i] = i / 128.;
  }
}

void normBigIs() {
  double bigIR_128 = bigIR[128];
  double bigIG_128 = bigIG[128];
  double bigIB_128 = bigIB[128];
  for (unsigned int i = 0; i < 255; i++) {
    bigIR[i] = bigIR[i] / bigIR_128;
    bigIG[i] = bigIG[i] / bigIG_128;
    bigIB[i] = bigIB[i] / bigIB_128;
  }
}

void estimateXs(unsigned int numImages, CImg<unsigned char>* images,
    double* times) {
  for (unsigned int x = 0; x < images[0].width(); x++) {
    for (unsigned int y = 0; y < images[0].height(); y++) {
      double estR = estimateX(numImages, images, times, x, y, 0);
      double estG = estimateX(numImages, images, times, x, y, 1);
      double estB = estimateX(numImages, images, times, x, y, 2);
      double xval[] = {estR, estG, estB};
      xs.draw_point(x,y,xval);
    }
  }
}

void estimateBigIs(unsigned int numImages, CImg<unsigned char>* images,
    double* times) {
  double sumsR[256] = {0.};
  double sumsG[256] = {0.};
  double sumsB[256] = {0.};
  unsigned char numerator;
  for (unsigned int i = 0; i < numImages; i++) {
    for (unsigned int x = 0; x < images[0].width(); x++) {
      for (unsigned int y = 0; y < images[0].height(); y++) {
        // have to save this in a variable because of a strange bug
        // (times[i] will randomly become -nan otherwise for some images)
        numerator = xs(x,y,0,0);
        sumsR[images[i](x,y,0,0)] += numerator / times[i];
        numerator = xs(x,y,0,1);
        sumsG[images[i](x,y,0,1)] += numerator / times[i];
        numerator = xs(x,y,0,2);
        sumsB[images[i](x,y,0,2)] += numerator / times[i];
      }
    }
  }
  for (unsigned int i = 0; i < 255; i++) {
    bigIR[i] = sumsR[i] / cardsR[i];
    bigIG[i] = sumsG[i] / cardsG[i];
    bigIB[i] = sumsB[i] / cardsB[i];
  }
}

// returns true if successful
bool loadImage(CImg<unsigned char>* image, string filename) {
  *image = CImg<unsigned char>(filename.c_str());
  return true;
}

string getHdrgen() {
  string line;
  ifstream config("config.cfg");
  getline(config, line);
  config.close();
  return line;
}

// returns the number of input images found in the hdrgen file
unsigned int initialize() {
  ifstream hdrgen(getHdrgen().c_str());
  unsigned int numLines = 0;
  string line;
  cout << "hdrgen file:" << endl;
  while (getline(hdrgen, line)) {
    numLines++;
    cout << line << endl;
  }
  hdrgen.close();
  return numLines;
}

// TODO sort images, because performance can then be improved later on
void loadImages(CImg<unsigned char>* images, double* times) {
  string hdrgenPath = getHdrgen();
  ifstream hdrgen(hdrgenPath.c_str());
  string directory = hdrgenPath.substr(0, hdrgenPath.rfind("/"));
  string line;
  unsigned int numLines = 0;
  while (getline(hdrgen, line)) {
    unsigned int spacePos = line.find(string(" "));
    times[numLines] = atof(line.substr(spacePos + 1, line.length()).c_str());
    loadImage(&images[numLines], directory + "/" + line.substr(0, spacePos));
    numLines++;
  }
  hdrgen.close();
}

int main2() {
  HDR hdr = HDR();
  unsigned int numImages = initialize();
  double* times = new double[numImages];
  CImg<unsigned char>* images = new CImg<unsigned char>[numImages];
  loadImages(images, times);
  initBigIs();

  for (unsigned int i = 0; i < numImages; i++) {
    for (unsigned int x = 0; x < images[0].width(); x++) {
      for (unsigned int y = 0; y < images[0].height(); y++) {
        cardsR[images[i](x,y,0,0)]++;
        cardsG[images[i](x,y,0,1)]++;
        cardsB[images[i](x,y,0,2)]++;
      }
    }
  }

  result = CImg<unsigned char>(images[0].width(), images[0].height(), 1, 3, 0);
  xs = CImg<double>(images[0].width(), images[0].height(), 1, 3, 0);

  double min = numeric_limits<double>::max();
  double max = 0.;
  for (unsigned int x = 0; x < images[0].width(); x++) {
    for (unsigned int y = 0; y < images[0].height(); y++) {
      double estR = estimateX(numImages, images, times, x, y, 0);
      double estG = estimateX(numImages, images, times, x, y, 1);
      double estB = estimateX(numImages, images, times, x, y, 2);
      unsigned char pixval[] = {expEstR(estR, 0.05), expEstG(estG, 0.05),
        expEstB(estB, 0.05)};
      result.draw_point(x,y,pixval);
    }
  }
  estimateXs(numImages, images, times);
  estimateBigIs(numImages, images, times);
  normBigIs();
  estimateXs(numImages, images, times);
  //estimateBigIs(numImages, images, times);
  //normBigIs();
  //estimateXs(numImages, images, times);
  //estimateBigIs(numImages, images, times);
  //normBigIs();
  //estimateXs(numImages, images, times);
  //estimateBigIs(numImages, images, times);
  //normBigIs();
  //estimateXs(numImages, images, times);
  //estimateBigIs(numImages, images, times);
  //normBigIs();
  //estimateXs(numImages, images, times);
  //estimateBigIs(numImages, images, times);
  //normBigIs();

  double minBigI = numeric_limits<double>::max();
  double maxBigI = 0.;
  for (unsigned int i = 0; i < 254; i++) {
    if (bigIR[i] < minBigI) {
      minBigI = bigIR[i];
    }
    if (bigIG[i] < minBigI) {
      minBigI = bigIG[i];
    }
    if (bigIB[i] < minBigI) {
      minBigI = bigIB[i];
    }
    if (bigIR[i] > maxBigI) {
      maxBigI = bigIR[i];
    }
    if (bigIG[i] > maxBigI) {
      maxBigI = bigIG[i];
    }
    if (bigIB[i] > maxBigI) {
      maxBigI = bigIB[i];
    }
  }
  for (unsigned int i = 0; i < 254; i++) {
    unsigned char red[] = {255, 0, 0};
    unsigned char green[] = {0, 255, 0};
    unsigned char blue[] = {50, 50, 255};
    double interv = (log(maxBigI) - log(minBigI)) / 500.;
    graph.draw_point((log(bigIR[i]) - log(minBigI)) / interv, 255 - i, red);
    graph.draw_point((log(bigIG[i]) - log(minBigI)) / interv, 255 - i, green);
    graph.draw_point((log(bigIB[i]) - log(minBigI)) / interv, 255 - i, blue);
  }

  CImgDisplay result_disp(result,"Result"),
              xs_disp(xs, "HDR image"),
              graph_disp(graph, "Response curves");
  while (!result_disp.is_closed() &&
      !xs_disp.is_closed() && !graph_disp.is_closed()) {
    result_disp.wait();
  }
  delete[] times;
  delete[] images;
  return 0;

}*/

int main() {
  HDR hdr = HDR();
  hdr.estimateXs();
  hdr.estimateBigIs();
  hdr.estimateXs();
  CImgDisplay graph_display = hdr.drawGraph();
  CImgDisplay xs_display = hdr.showXs();
  while (!graph_display.is_closed() && !xs_display.is_closed()) {
    graph_display.wait();
  }
  return 0;
}
