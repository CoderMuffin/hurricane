#ifndef HC_SHARED_H
#define HC_SHARED_H

#define _USE_MATH_DEFINES
#include <math.h>

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)
#define HC_CLIP_NEAR 0.1

#define DEG2RAD (M_PI / 180)
#define VEC3(x, y, z) ((double[]){(x), (y), (z)})
#define QUAT(x, y, z) ((hc_quaternion[]){hc_quaternion_new_euler_zyx(VEC3((z), (y), (x)))})

void swap_int(int *a, int *b);
void swap_dbl(double *a, double *b);
void hc_sleep(long ms);

#endif
