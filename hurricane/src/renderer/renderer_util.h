#ifndef RENDERER_H
#define RENDERER_H

#include <stdlib.h>
#include <taylor_math.h>

#define HC_INTERNAL_DEPTH_BUF_CHECK(x0, y0, z0, x1, y1, z1, x2, y2, z2, r, g,  \
                                    b, width, buf, fn)                         \
  double depth = calculate_depth(x0, y0, z0, x1, y1, z1, x2, y2, z2, x, y);    \
  if (isnan(depth) || !isfinite(depth) || depth <= 0.1) {                      \
    continue;                                                                  \
  }                                                                            \
  if (depth < buf[y * width + x]) {                                                     \
    buf[y * width + x] = depth;                                                         \
    fn                                                                         \
  }

#define HC_INTERNAL_BUF_TRIANGLE(x0, y0, z0, x1, y1, z1, x2, y2, z2, r, g, b,  \
                                 width, height, fn)                            \
  /* sort the points vertically */                                             \
  if (y1 > y2) {                                                               \
    swap_int(&x1, &x2);                                                        \
    swap_int(&y1, &y2);                                                        \
    swap_dbl(&z1, &z2);                                                        \
  }                                                                            \
  if (y0 > y1) {                                                               \
    swap_int(&x0, &x1);                                                        \
    swap_int(&y0, &y1);                                                        \
    swap_dbl(&z0, &z1);                                                        \
  }                                                                            \
  if (y1 > y2) {                                                               \
    swap_int(&x1, &x2);                                                        \
    swap_int(&y1, &y2);                                                        \
    swap_dbl(&z1, &z2);                                                        \
  }                                                                            \
  double dx_far = ((double)(x2 - x0)) / (y2 - y0 + 1);                         \
  double dx_upper = ((double)(x1 - x0)) / (y1 - y0 + 1);                       \
  double dx_low = ((double)(x2 - x1)) / (y2 - y1 + 1);                         \
  double xf = x0;                                                              \
  double xt = x0 + dx_upper; /* if y0 == y1, special case */                   \
  for (int y = y0;                                                             \
       y <= (y2 > height - 1 ? height - 1 : y2); y++) {    \
    if (y >= 0) {                                                              \
      for (int x = (xf > 0 ? (int)(xf) : 0);                                   \
           x <= (xt < width ? xt : width - 1); x++) {    \
        fn                                                                     \
      }                                                                        \
      for (int x = (xf < width ? (int)(xf) : width - 1); \
           x >= (xt > 0 ? xt : 0); x--) {                                      \
        fn                                                                     \
      }                                                                        \
    }                                                                          \
    xf += dx_far;                                                              \
    if (y < y1)                                                                \
      xt += dx_upper;                                                          \
    else                                                                       \
      xt += dx_low;                                                            \
  }

double calculate_depth(double x0, double y0, double z0, double x1, double y1,
                       double z1, double x2, double y2, double z2, double px,
                       double py) {
  double w_denom = (y1 - y2)*(x0 - x2) + (x2 - x1)*(y0 - y2);

  if (abs(w_denom) < 1.0) {
    return INFINITY;
  }

  double w0 = ((y1 - y2) * (px - x2) + (x2 - x1) * (py - y2)) / w_denom;
  double w1 = ((y2 - y0) * (px - x2) + (x0 - x2) * (py - y2)) / w_denom;
  double w2 = 1 - w0 - w1;

  return 1.0/(w0 * z0 + w1 * z1 + w2 * z2);
}

#endif
