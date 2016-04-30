#include "HDR.h"

int main() {
  HDR hdr = HDR();
  hdr.estimateXs();
  for (unsigned int i = 0; i < 1; i++) { // XXX increase iterations
    hdr.estimateBigIs();
    hdr.estimateXs();
  }
  CDisplay graph_display = hdr.drawGraph();
  CDisplay xs_display = hdr.showXs();
  CDisplay exp_display = hdr.showExposure(0.05);
  while (!graph_display.is_closed() && !xs_display.is_closed() &&
      !exp_display.is_closed()) {
    graph_display.wait();
  }
  return 0;
}
