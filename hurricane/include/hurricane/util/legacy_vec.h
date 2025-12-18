#ifndef HC_VEC_H
#define HC_VEC_H

typedef struct {
  float v[2];
} hc_vec2;
typedef struct {
  float v[3];
} hc_vec3;

hc_vec2 hc_new_vec2(float x, float y);
hc_vec3 hc_new_vec3(float x, float y, float z);
hc_vec2 hc_vec2_add(hc_vec2 a, hc_vec2 b);
hc_vec3 hc_vec3_add(hc_vec3 a, hc_vec3 b);
hc_vec2 hc_vec2_sub(hc_vec2 a, hc_vec2 b);
hc_vec3 hc_vec3_sub(hc_vec3 a, hc_vec3 b);
hc_vec2 hc_vec2_mul(hc_vec2 a, float b);
hc_vec3 hc_vec3_mul(hc_vec3 a, float b);
hc_vec2 hc_vec2_vmul(hc_vec2 a, hc_vec2 b);
hc_vec3 hc_vec3_vmul(hc_vec3 a, hc_vec3 b);
hc_vec2 hc_vec2_div(hc_vec2 a, float b);
hc_vec3 hc_vec3_div(hc_vec3 a, float b);
hc_vec2 hc_vec2_vdiv(hc_vec2 a, hc_vec2 b);
hc_vec3 hc_vec3_vdiv(hc_vec3 a, hc_vec3 b);
float hc_vec2_sqrmag(hc_vec2 v);
float hc_vec3_sqrmag(hc_vec3 v);
float hc_vec2_mag(hc_vec2 v);
float hc_vec3_mag(hc_vec3 v);

#endif
