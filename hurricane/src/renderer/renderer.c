#include <hurricane/fixed.h>
#include <taylor_math.h>

fixed calculate_depth(fixed x0, fixed y0, fixed z0, fixed x1, fixed y1,
                       fixed z1, fixed x2, fixed y2, fixed z2, fixed px,
                       fixed py) {
  fixed w_denom = fm(y1 - y2, x0 - x2) + fm(x2 - x1, y0 - y2);

  if (fx_abs(w_denom) < tf(0.1)) {
    return tf(1000);
  }

  fixed w0 = fd(fm(y1 - y2, px - x2) + fm(x2 - x1, py - y2), w_denom);
  fixed w1 = fd(fm(y2 - y0, px - x2) + fm(x0 - x2, py - y2), w_denom);
  fixed w2 = tf(1) - w0 - w1;

  fixed d = fm(w0, z0) + fm(w1, z1) + fm(w2, z2);
  if (d == 0) {
    return tf(1000);
  }
  
  return fd(tf(1.0), d);
}

