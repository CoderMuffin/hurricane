#ifndef HC_RENDERER_UTIL_H
#define HC_RENDERER_UTIL_H

#include <taylor_math.h>

#define HC_INTERNAL_DEPTH_BUF_CHECK(x0, y0, z0, x1, y1, z1, x2, y2, z2, width, \
                                    buf, fn)                                   \
  fixed depth = calculate_depth(x0, y0, z0, x1, y1, z1, x2, y2, z2, x, y);    \
  if (isnan(depth) || !isfinite(depth) || depth <= 0.1_f) {                      \
    continue;                                                                  \
  }                                                                            \
  if (depth < buf[y * width + x]) {                                            \
    buf[y * width + x] = depth;                                                \
    fn                                                                         \
  }

#define HC_INTERNAL_BUF_TRIANGLE(x0, y0, z0, x1, y1, z1, x2, y2, z2, width,    \
                                 height, fn)                                   \
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
  fixed dx_far = ((fixed)(x2 - x0)) / (y2 - y0 + 1);                         \
  fixed dx_upper = ((fixed)(x1 - x0)) / (y1 - y0 + 1);                       \
  fixed dx_low = ((fixed)(x2 - x1)) / (y2 - y1 + 1);                         \
  fixed xf = x0;                                                              \
  fixed xt = x0 + dx_upper; /* if y0 == y1, special case */                   \
  for (int y = y0; y <= (y2 > height - 1 ? height - 1 : y2); y++) {            \
    if (y >= 0) {                                                              \
      for (int x = (xf > 0_f ? (int)(xf) : 0_f);                                   \
           x <= (xt < width ? xt : width - 1); x++) {                          \
        fn                                                                     \
      }                                                                        \
      for (int x = (xf < width ? (int)(xf) : width - 1);                       \
           x >= (xt > 0_f ? xt : 0_f); x--) {                                      \
        fn                                                                     \
      }                                                                        \
    }                                                                          \
    xf += dx_far;                                                              \
    if (y < y1)                                                                \
      xt += dx_upper;                                                          \
    else                                                                       \
      xt += dx_low;                                                            \
  }

#endif
