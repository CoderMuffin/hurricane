#ifndef HC_VEC_H
#define HC_VEC_H

extern const float hc_vec3_zero[3];
extern const float hc_vec3_one[3];

extern const float hc_vec3_forward[3];
extern const float hc_vec3_backward[3];
extern const float hc_vec3_left[3];
extern const float hc_vec3_right[3];
extern const float hc_vec3_up[3];
extern const float hc_vec3_down[3];

void hc_vec3_set(float x, float y, float z, float out[3]);
void hc_vec3_copy(const float from[3], float to[3]);
void hc_vec3_add(const float a[3], const float b[3], float out[3]);
void hc_vec3_sub(const float a[3], const float b[3], float out[3]);
void hc_vec3_mul(const float a[3], const float b, float out[3]);
void hc_vec3_vmul(const float a[3], const float b[3], float out[3]);
void hc_vec3_div(const float a[3], const float b, float out[3]);
void hc_vec3_vdiv(const float a[3], const float b[3], float out[3]);
float hc_vec3_sqrmag(const float v[3]);
float hc_vec3_mag(const float v[3]);
void hc_vec3_cross(const float a[3], const float b[3], float out[3]);
void hc_vec3_normalize(const float in[3], float out[3]);
void hc_vec3_lerp(const float a[3], const float b[3], float f,
                  float out[3]);
float hc_vec3_dot(const float a[3], const float b[3]);

#endif
