#ifndef HC_VEC_H
#define HC_VEC_H

typedef struct {
  double v[2];
} hc_vec2;
typedef struct {
  double v[3];
} hc_vec3;

hc_vec2 hc_new_vec2(double x, double y);
hc_vec3 hc_new_vec3(double x, double y, double z);
hc_vec2 hc_vec2_add(hc_vec2 a, hc_vec2 b);
hc_vec3 hc_vec3_add(hc_vec3 a, hc_vec3 b);
hc_vec2 hc_vec2_sub(hc_vec2 a, hc_vec2 b);
hc_vec3 hc_vec3_sub(hc_vec3 a, hc_vec3 b);
hc_vec2 hc_vec2_mul(hc_vec2 a, double b);
hc_vec3 hc_vec3_mul(hc_vec3 a, double b);
hc_vec2 hc_vec2_vmul(hc_vec2 a, hc_vec2 b);
hc_vec3 hc_vec3_vmul(hc_vec3 a, hc_vec3 b);
hc_vec2 hc_vec2_div(hc_vec2 a, double b);
hc_vec3 hc_vec3_div(hc_vec3 a, double b);
hc_vec2 hc_vec2_vdiv(hc_vec2 a, hc_vec2 b);
hc_vec3 hc_vec3_vdiv(hc_vec3 a, hc_vec3 b);
double hc_vec2_sqrmag(hc_vec2 v);
double hc_vec3_sqrmag(hc_vec3 v);
double hc_vec2_mag(hc_vec2 v);
double hc_vec3_mag(hc_vec3 v);

#endif
