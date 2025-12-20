#include <math.h>

fixed calculate_depth(fixed x0, fixed y0, fixed z0, fixed x1, fixed y1,
                       fixed z1, fixed x2, fixed y2, fixed z2, fixed px,
                       fixed py) {
  fixed w_denom = (y1 - y2) * (x0 - x2) + (x2 - x1) * (y0 - y2);

  if (fabs(w_denom) < 0.1) {
    return INFINITY;
  }

  fixed w0 = ((y1 - y2) * (px - x2) + (x2 - x1) * (py - y2)) / w_denom;
  fixed w1 = ((y2 - y0) * (px - x2) + (x0 - x2) * (py - y2)) / w_denom;
  fixed w2 = 1 - w0 - w1;

  return 1.0 / (w0 * z0 + w1 * z1 + w2 * z2);
}

