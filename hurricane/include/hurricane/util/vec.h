#ifndef HC_VEC_H
#define HC_VEC_H

#include "../fixed.h"

void hc_vec3_set(fixed x, fixed y, fixed z, fixed out[3]);
void hc_vec3_copy(const fixed from[3], fixed to[3]);
void hc_vec3_add(const fixed a[3], const fixed b[3], fixed out[3]);
void hc_vec3_sub(const fixed a[3], const fixed b[3], fixed out[3]);
void hc_vec3_mul(const fixed a[3], const fixed b, fixed out[3]);
void hc_vec3_vmul(const fixed a[3], const fixed b[3], fixed out[3]);
void hc_vec3_div(const fixed a[3], const fixed b, fixed out[3]);
void hc_vec3_vdiv(const fixed a[3], const fixed b[3], fixed out[3]);
fixed hc_vec3_sqrmag(const fixed v[3]);
fixed hc_vec3_mag(const fixed v[3]);
void hc_vec3_cross(const fixed a[3], const fixed b[3], fixed out[3]);
void hc_vec3_normalize(const fixed in[3], fixed out[3]);
void hc_vec3_lerp(const fixed a[3], const fixed b[3], fixed f,
                  fixed out[3]);
fixed hc_vec3_dot(const fixed a[3], const fixed b[3]);

#endif
