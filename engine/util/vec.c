#ifndef HC_VEC
#define HC_VEC
#include <math.h>

void hc_vec3_set(double x, double y, double z, double out[3]) {
  out[0] = x;
  out[1] = y;
  out[2] = z;
}

void hc_vec3_copy(double from[3], double to[3]) {
  to[0] = from[0];
  to[1] = from[1];
  to[2] = from[2];
}

void hc_vec3_add(double a[3], double b[3], double out[3]) {
  out[0] = a[0] + b[0];
  out[1] = a[1] + b[1];
  out[2] = a[2] + b[2];
}

void hc_vec3_sub(double a[3], double b[3], double out[3]) {
  out[0] = a[0] - b[0];
  out[1] = a[1] - b[1];
  out[2] = a[2] - b[2];
}

void hc_vec3_mul(double a[3], double b, double out[3]) {
  out[0] = a[0] * b;
  out[1] = a[1] * b;
  out[2] = a[2] * b;
}

void hc_vec3_vmul(double a[3], double b[3], double out[3]) {
  out[0] = a[0] * b[0];
  out[1] = a[1] * b[1];
  out[2] = a[2] * b[2];
}

void hc_vec3_div(double a[3], double b, double out[3]) {
  out[0] = a[0] / b;
  out[1] = a[1] / b;
  out[2] = a[2] / b;
}

void hc_vec3_vdiv(double a[3], double b[3], double out[3]) {
  out[0] = a[0] / b[0];
  out[1] = a[1] / b[1];
  out[2] = a[2] / b[2];
}

double hc_vec3_sqrmag(double v[3]) {
  return v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
}
double hc_vec3_mag(double v[3]) {
  return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

#endif
