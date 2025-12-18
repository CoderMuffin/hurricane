#ifndef HC_SHARED_H
#define HC_SHARED_H

#include <math.h>

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)
#define HC_CLIP_NEAR 0.1

#define DEG2RAD (M_PI / 180)
#define VEC3(x, y, z) ((float[]){(x), (y), (z)})
#define QUAT(x, y, z) ((hc_quaternion[]){hc_quaternion_new_euler_zyx(VEC3((z), (y), (x)))})

void swap_int(int *a, int *b);
void swap_dbl(float *a, float *b);
float calculate_depth(float x0, float y0, float z0, float x1, float y1,
                       float z1, float x2, float y2, float z2, float px,
                       float py);
void hc_sleep(long ms);

#endif
