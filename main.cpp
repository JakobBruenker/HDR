#include <CImg.h>
#include <limits>
#include <fstream>
#include <iostream>
#include "HDR.h"
using namespace cimg_library;
using namespace std;

int main() {
  HDR hdr = HDR();
  hdr.estimateXs();
  hdr.estimateBigIs();
  hdr.estimateXs();
  hdr.estimateBigIs();
  hdr.estimateXs();
  hdr.estimateBigIs();
  hdr.estimateXs();
  hdr.estimateBigIs();
  hdr.estimateXs();
  hdr.estimateBigIs();
  hdr.estimateXs();
  CImgDisplay graph_display = hdr.drawGraph();
  CImgDisplay xs_display = hdr.showXs();
  CImgDisplay exp_display = hdr.showExposure(0.05);
  while (!graph_display.is_closed() && !xs_display.is_closed() &&
      !exp_display.is_closed()) {
    graph_display.wait();
  }
  return 0;
}
