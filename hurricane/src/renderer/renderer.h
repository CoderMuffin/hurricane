#ifndef RENDERER_H
#define RENDERER_H

#define HC_INTERNAL_DEPTH_BUF_CHECK(x0, y0, z0, x1, y1, z1, x2, y2, z2, r, g,  \
                                    b, buf, fn)                                \
  double depth = calculate_depth(x0, y0, z0, x1, y1, z1, x2, y2, z2, x, y);    \
  if (isnan(depth) || !isfinite(depth) || depth <= 0.1) {                      \
    continue;                                                                  \
  }                                                                            \
  if (depth < buf[y][x]) {                                                     \
    buf[y][x] = depth;                                                         \
    fn                                                                         \
  }

#define HC_INTERNAL_BUF_TRIANGLE(x0, y0, z0, x1, y1, z1, x2, y2, z2, r, g, b,  \
                                 fn)                                           \
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
       y <= (y2 > HC_RENDER_SIZE_Y - 1 ? HC_RENDER_SIZE_Y - 1 : y2); y++) {    \
    if (y >= 0) {                                                              \
      for (int x = (xf > 0 ? (int)(xf) : 0);                                   \
           x <= (xt < HC_RENDER_SIZE_X ? xt : HC_RENDER_SIZE_X - 1); x++) {    \
        fn                                                                     \
      }                                                                        \
      for (int x = (xf < HC_RENDER_SIZE_X ? (int)(xf) : HC_RENDER_SIZE_X - 1); \
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

#endif
