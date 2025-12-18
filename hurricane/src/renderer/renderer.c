#include <math.h>

float calculate_depth(float x0, float y0, float z0, float x1, float y1,
                       float z1, float x2, float y2, float z2, float px,
                       float py) {
  float w_denom = (y1 - y2) * (x0 - x2) + (x2 - x1) * (y0 - y2);

  if (fabs(w_denom) < 0.1) {
    return INFINITY;
  }

  float w0 = ((y1 - y2) * (px - x2) + (x2 - x1) * (py - y2)) / w_denom;
  float w1 = ((y2 - y0) * (px - x2) + (x0 - x2) * (py - y2)) / w_denom;
  float w2 = 1 - w0 - w1;

  return 1.0 / (w0 * z0 + w1 * z1 + w2 * z2);
}

