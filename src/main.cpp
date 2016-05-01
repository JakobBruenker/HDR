#include "HDR.h"
#include "Tonemapper.h"

int main() {
  HDR hdr = HDR();
  hdr.estimateXs();
  // Iterative algorithm is five times, after which it has probably converged
  // reasonably well
  for (unsigned int i = 0; i < 5; i++) {
    hdr.estimateBigIs();
    hdr.estimateXs();
  }

  // assign the maximum observed value to pixels that were overexposed in all
  // input images, since they otherwise would have the value 0
  hdr.maxOverexposed();

  // write an HDR image and the response function to files
  hdr.writeResponse();
  hdr.writeEXRFile();

  // allocate buffer for the tonemapper
  double* buffer = new double[hdr.getWidth()*hdr.getHeight()*3];
  hdr.getLuminances(buffer);
  Tonemapper tm = Tonemapper(hdr.getWidth(), hdr.getHeight(), buffer);

  // show the results in windows
  CDisplay tonemapped_display = tm.showImage();
  CDisplay graph_display = hdr.drawGraph();
  CDisplay exp0_display = hdr.showExposure(0.01);
  CDisplay exp1_display = hdr.showExposure(0.05);
  CDisplay exp2_display = hdr.showExposure(0.33);

  // if any of the windows are closed, exit the program
  while (!graph_display.is_closed() && !tonemapped_display.is_closed() &&
      !exp0_display.is_closed() && !exp1_display.is_closed() &&
      !exp2_display.is_closed()) {
    CDisplay::wait_all();
  }
  delete[] buffer;
  return 0;
}
