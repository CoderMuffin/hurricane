#ifndef HC_VEC
#define HC_VEC
#include "math.h"
#include "util.c"

typedef struct {
  double x;
  double y;
} hc_vec2;
typedef struct {
  double x;
  double y;
  double z;
} hc_vec3;

hc_vec2 hc_new_vec2(double x, double y) {
  hc_vec2 out;
  out.x = x;
  out.y = y;
  return out;
}
hc_vec3 hc_new_vec3(double x, double y, double z) {
  hc_vec3 out;
  out.x = x;
  out.y = y;
  out.z = z;
  return out;
}

hc_vec2 hc_vec2_add(hc_vec2 a, hc_vec2 b) {
  hc_vec2 out;
  out.x = a.x + b.x;
  out.y = a.y + b.y;
  return out;
}
hc_vec3 hc_vec3_add(hc_vec3 a, hc_vec3 b) {
  hc_vec3 out;
  out.x = a.x + b.x;
  out.y = a.y + b.y;
  out.z = a.z + b.z;
  return out;
}

hc_vec2 hc_vec2_sub(hc_vec2 a, hc_vec2 b) {
  hc_vec2 out;
  out.x = a.x - b.x;
  out.y = a.y - b.y;
  return out;
}
hc_vec3 hc_vec3_sub(hc_vec3 a, hc_vec3 b) {
  hc_vec3 out;
  out.x = a.x - b.x;
  out.y = a.y - b.y;
  out.z = a.z - b.z;
  return out;
}

hc_vec2 hc_vec2_mul(hc_vec2 a, double b) {
  hc_vec2 out;
  out.x = a.x * b;
  out.y = a.y * b;
  return out;
}
hc_vec3 hc_vec3_mul(hc_vec3 a, double b) {
  hc_vec3 out;
  out.x = a.x * b;
  out.y = a.y * b;
  out.z = a.z * b;
  return out;
}

hc_vec2 hc_vec2_vmul(hc_vec2 a, hc_vec2 b) {
  hc_vec2 out;
  out.x = a.x * b.x;
  out.y = a.y * b.y;
  return out;
}
hc_vec3 hc_vec3_vmul(hc_vec3 a, hc_vec3 b) {
  hc_vec3 out;
  out.x = a.x * b.x;
  out.y = a.y * b.y;
  out.z = a.z * b.z;
  return out;
}

hc_vec2 hc_vec2_div(hc_vec2 a, double b) {
  hc_vec2 out;
  out.x = a.x / b;
  out.y = a.y / b;
  return out;
}
hc_vec3 hc_vec3_div(hc_vec3 a, double b) {
  hc_vec3 out;
  out.x = a.x / b;
  out.y = a.y / b;
  out.z = a.z / b;
  return out;
}

hc_vec2 hc_vec2_vdiv(hc_vec2 a, hc_vec2 b) {
  hc_vec2 out;
  out.x = a.x / b.x;
  out.y = a.y / b.y;
  return out;
}
hc_vec3 hc_vec3_vdiv(hc_vec3 a, hc_vec3 b) {
  hc_vec3 out;
  out.x = a.x / b.x;
  out.y = a.y / b.y;
  out.z = a.z / b.z;
  return out;
}

double hc_vec2_sqrmag(hc_vec2 v) { return v.x * v.x + v.y * v.y; }
double hc_vec3_sqrmag(hc_vec3 v) { return v.x * v.x + v.y * v.y + v.z * v.z; }

double hc_vec2_mag(hc_vec2 v) { return sqrt(v.x * v.x + v.y * v.y); }
double hc_vec3_mag(hc_vec3 v) {
  return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

#endif
