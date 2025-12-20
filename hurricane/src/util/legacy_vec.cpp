#include <math.h>

typedef struct {
  fixed v[2];
} hc_vec2;
typedef struct {
  fixed v[3];
} hc_vec3;

hc_vec2 hc_new_vec2(fixed x, fixed y) {
  hc_vec2 out;
  out.v[0] = x;
  out.v[1] = y;
  return out;
}
hc_vec3 hc_new_vec3(fixed x, fixed y, fixed z) {
  hc_vec3 out;
  out.v[0] = x;
  out.v[1] = y;
  out.v[2] = z;
  return out;
}

hc_vec2 hc_vec2_add(hc_vec2 a, hc_vec2 b) {
  hc_vec2 out;
  out.v[0] = a.v[0] + b.v[0];
  out.v[1] = a.v[1] + b.v[1];
  return out;
}
hc_vec3 hc_vec3_add(hc_vec3 a, hc_vec3 b) {
  hc_vec3 out;
  out.v[0] = a.v[0] + b.v[0];
  out.v[1] = a.v[1] + b.v[1];
  out.v[2] = a.v[2] + b.v[2];
  return out;
}

hc_vec2 hc_vec2_sub(hc_vec2 a, hc_vec2 b) {
  hc_vec2 out;
  out.v[0] = a.v[0] - b.v[0];
  out.v[1] = a.v[1] - b.v[1];
  return out;
}
hc_vec3 hc_vec3_sub(hc_vec3 a, hc_vec3 b) {
  hc_vec3 out;
  out.v[0] = a.v[0] - b.v[0];
  out.v[1] = a.v[1] - b.v[1];
  out.v[2] = a.v[2] - b.v[2];
  return out;
}

hc_vec2 hc_vec2_mul(hc_vec2 a, fixed b) {
  hc_vec2 out;
  out.v[0] = a.v[0] * b;
  out.v[1] = a.v[1] * b;
  return out;
}
hc_vec3 hc_vec3_mul(hc_vec3 a, fixed b) {
  hc_vec3 out;
  out.v[0] = a.v[0] * b;
  out.v[1] = a.v[1] * b;
  out.v[2] = a.v[2] * b;
  return out;
}

hc_vec2 hc_vec2_vmul(hc_vec2 a, hc_vec2 b) {
  hc_vec2 out;
  out.v[0] = a.v[0] * b.v[0];
  out.v[1] = a.v[1] * b.v[1];
  return out;
}
hc_vec3 hc_vec3_vmul(hc_vec3 a, hc_vec3 b) {
  hc_vec3 out;
  out.v[0] = a.v[0] * b.v[0];
  out.v[1] = a.v[1] * b.v[1];
  out.v[2] = a.v[2] * b.v[2];
  return out;
}

hc_vec2 hc_vec2_div(hc_vec2 a, fixed b) {
  hc_vec2 out;
  out.v[0] = a.v[0] / b;
  out.v[1] = a.v[1] / b;
  return out;
}
hc_vec3 hc_vec3_div(hc_vec3 a, fixed b) {
  hc_vec3 out;
  out.v[0] = a.v[0] / b;
  out.v[1] = a.v[1] / b;
  out.v[2] = a.v[2] / b;
  return out;
}

hc_vec2 hc_vec2_vdiv(hc_vec2 a, hc_vec2 b) {
  hc_vec2 out;
  out.v[0] = a.v[0] / b.v[0];
  out.v[1] = a.v[1] / b.v[1];
  return out;
}
hc_vec3 hc_vec3_vdiv(hc_vec3 a, hc_vec3 b) {
  hc_vec3 out;
  out.v[0] = a.v[0] / b.v[0];
  out.v[1] = a.v[1] / b.v[1];
  out.v[2] = a.v[2] / b.v[2];
  return out;
}

fixed hc_vec2_sqrmag(hc_vec2 v) { return v.v[0] * v.v[0] + v.v[1] * v.v[1]; }
fixed hc_vec3_sqrmag(hc_vec3 v) { return v.v[0] * v.v[0] + v.v[1] * v.v[1] + v.v[2] * v.v[2]; }

fixed hc_vec2_mag(hc_vec2 v) { return sqrt(v.v[0] * v.v[0] + v.v[1] * v.v[1]); }
fixed hc_vec3_mag(hc_vec3 v) {
  return sqrt(v.v[0] * v.v[0] + v.v[1] * v.v[1] + v.v[2] * v.v[2]);
}
