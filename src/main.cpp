#include "HDR.h"
#include "Tonemapper.h"

int main() {
  HDR hdr = HDR();
  hdr.estimateXs();
  for (unsigned int i = 0; i < 5; i++) {
    hdr.estimateBigIs();
    hdr.estimateXs();
  }
  hdr.maxOverexposed();
  hdr.writeEXRFile();
  double* buffer = new double[hdr.getWidth()*hdr.getHeight()*3];
  hdr.getLuminances(buffer);
  Tonemapper tm = Tonemapper(hdr.getWidth(), hdr.getHeight(), buffer);
  CDisplay tonemapped_display = tm.showImage();
  CDisplay graph_display = hdr.drawGraph();
  CDisplay exp0_display = hdr.showExposure(0.01);
  CDisplay exp1_display = hdr.showExposure(0.05);
  CDisplay exp2_display = hdr.showExposure(0.33);
  while (!graph_display.is_closed() && !tonemapped_display.is_closed() &&
      !exp0_display.is_closed() && !exp1_display.is_closed() &&
      !exp2_display.is_closed()) {
    CDisplay::wait_all;
  }
  delete[] buffer;
  return 0;
}
