#ifndef HC_VEC_H
#define HC_VEC_H

//macros
#include <math.h>

//types

//decls
extern const double hc_vec3_zero[3];
extern const double hc_vec3_one[3];

extern const double hc_vec3_forward[3];
extern const double hc_vec3_backward[3];
extern const double hc_vec3_left[3];
extern const double hc_vec3_right[3];
extern const double hc_vec3_up[3];
extern const double hc_vec3_down[3];

//functions
void hc_vec3_set(double x, double y, double z, double out[3]);
void hc_vec3_copy(const double from[3], double to[3]);
void hc_vec3_add(const double a[3], const double b[3], double out[3]);
void hc_vec3_sub(const double a[3], const double b[3], double out[3]);
void hc_vec3_mul(const double a[3], const double b, double out[3]);
void hc_vec3_vmul(const double a[3], const double b[3], double out[3]);
void hc_vec3_div(const double a[3], const double b, double out[3]);
void hc_vec3_vdiv(const double a[3], const double b[3], double out[3]);
double hc_vec3_sqrmag(const double v[3]);
double hc_vec3_mag(const double v[3]);
void hc_vec3_cross(const double a[3], const double b[3], double out[3]);
void hc_vec3_normalize(const double in[3], double out[3]);
void hc_vec3_lerp(const double a[3], const double b[3], double f, double out[3]);
double hc_vec3_dot(const double a[3], const double b[3]);

#endif
