#ifndef HC_SHARED_H
#define HC_SHARED_H

#include "fixed.h"
#include <taylor_math.h>

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)
#define HC_CLIP_NEAR tf(0.1)

#define DEG2RAD (tf(M_PI / 180))
#define VEC3(x, y, z) ((fixed[]){tf(x), tf(y), tf(z)})
#define QUAT(x, y, z) ((hc_quaternion[]) { hc_quaternion_new_euler_zyx(VEC3((z), (y), (x))) })

void swap_int(int *a, int *b);
void swap_dbl(fixed *a, fixed *b);
fixed calculate_depth(fixed x0, fixed y0, fixed z0, fixed x1, fixed y1,
                       fixed z1, fixed x2, fixed y2, fixed z2, fixed px,
                       fixed py);
void hc_sleep(long ms);

#endif
