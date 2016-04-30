#include "HDR.h"
#include "Tonemapper.h"

int main() {
  HDR hdr = HDR();
  hdr.estimateXs();
  for (unsigned int i = 0; i < 10; i++) { // FIXME
    hdr.estimateBigIs();
    hdr.estimateXs();
  }
  //double* buffer = new double[hdr.getWidth()*hdr.getHeight()*3];
  Tonemapper tm = Tonemapper();//hdr.getWidth(), hdr.getHeight(), buffer);
  CDisplay graph_display = hdr.drawGraph();
  CDisplay exp_display = hdr.showExposure(0.05);
  while (!graph_display.is_closed() &&
      !exp_display.is_closed()) {
    graph_display.wait();
  }
  //tm.showImage();
  printf("what\n");
  //delete[] buffer;
  return 0;
}
