#include <math.h>

double calculate_depth(double x0, double y0, double z0, double x1, double y1,
                       double z1, double x2, double y2, double z2, double px,
                       double py) {
  double w_denom = (y1 - y2) * (x0 - x2) + (x2 - x1) * (y0 - y2);

  if (fabs(w_denom) < 0.1) {
    return INFINITY;
  }

  double w0 = ((y1 - y2) * (px - x2) + (x2 - x1) * (py - y2)) / w_denom;
  double w1 = ((y2 - y0) * (px - x2) + (x0 - x2) * (py - y2)) / w_denom;
  double w2 = 1 - w0 - w1;

  return 1.0 / (w0 * z0 + w1 * z1 + w2 * z2);
}

