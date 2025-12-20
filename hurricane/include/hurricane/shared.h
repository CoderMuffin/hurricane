#ifndef HC_SHARED_H
#define HC_SHARED_H

#include "fixed.hpp"
#include <math.h>
#include "util/quat.h"
#include <array>

inline constexpr std::array<fixed,3> VEC3(fixed x, fixed y, fixed z) {
    return { x, y, z };
}
#define QUAT(x,y,z) hc_quaternion_new_euler_zyx((fixed[]){(z),(y),(x)})

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)
#define HC_CLIP_NEAR 0.1

#define DEG2RAD (M_PI / 180)

void swap_int(int *a, int *b);
void swap_dbl(fixed *a, fixed *b);
fixed calculate_depth(fixed x0, fixed y0, fixed z0, fixed x1, fixed y1,
                       fixed z1, fixed x2, fixed y2, fixed z2, fixed px,
                       fixed py);
void hc_sleep(long ms);

#endif
