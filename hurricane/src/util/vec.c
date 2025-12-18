#include <taylor_math.h>

const float hc_vec3_zero[3] = {0, 0, 0};
const float hc_vec3_one[3] = {1, 1, 1};

const float hc_vec3_forward[3] = {0, 0, 1};
const float hc_vec3_backward[3] = {0, 0, -1};
const float hc_vec3_left[3] = {-1, 0, 0};
const float hc_vec3_right[3] = {1, 0, 0};
const float hc_vec3_up[3] = {0, 1, 0};
const float hc_vec3_down[3] = {0, -1, 0};

/// Writes the three xyz parameters to the specified output vector.
void hc_vec3_set(float x, float y, float z, float out[3]) {
  out[0] = x;
  out[1] = y;
  out[2] = z;
}

/// Copies vector from to vector to.
void hc_vec3_copy(const float from[3], float to[3]) {
  to[0] = from[0];
  to[1] = from[1];
  to[2] = from[2];
}

/// Adds two vectors, storing the result in out.
void hc_vec3_add(const float a[3], const float b[3], float out[3]) {
  out[0] = a[0] + b[0];
  out[1] = a[1] + b[1];
  out[2] = a[2] + b[2];
}

/// Subtracts two vectors, storing the result in out.
void hc_vec3_sub(const float a[3], const float b[3], float out[3]) {
  out[0] = a[0] - b[0];
  out[1] = a[1] - b[1];
  out[2] = a[2] - b[2];
}

/// Multiplies all components of a vector by a scalar, storing the result in out.
void hc_vec3_mul(const float a[3], const float b, float out[3]) {
  out[0] = a[0] * b;
  out[1] = a[1] * b;
  out[2] = a[2] * b;
}

/// Multiplies two vectors, storing the result in out.
void hc_vec3_vmul(const float a[3], const float b[3], float out[3]) {
  out[0] = a[0] * b[0];
  out[1] = a[1] * b[1];
  out[2] = a[2] * b[2];
}

/// Divides all components of a vector by a scalar, storing the result in out.
void hc_vec3_div(const float a[3], const float b, float out[3]) {
  out[0] = a[0] / b;
  out[1] = a[1] / b;
  out[2] = a[2] / b;
}

/// Divides two vectors, storing the result in out.
void hc_vec3_vdiv(const float a[3], const float b[3], float out[3]) {
  out[0] = a[0] / b[0];
  out[1] = a[1] / b[1];
  out[2] = a[2] / b[2];
}

/// Returns the square magnitude of a vector (useful if the sqrt calculation is not needed, as this saves unnecessary computation).
float hc_vec3_sqrmag(const float v[3]) {
  return v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
}

/// Returns the magnitude of a vector.
float hc_vec3_mag(const float v[3]) {
  return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

/// Calculates the cross product of two vectors, storing the result in out.
void hc_vec3_cross(const float a[3], const float b[3], float out[3]) {
  out[0] = a[1] * b[2] - a[2] * b[1];
  out[1] = a[2] * b[0] - a[0] * b[2];
  out[2] = a[0] * b[1] - a[1] * b[0];
}

/// Normalizes a vector (sets its length to 1).
void hc_vec3_normalize(const float in[3], float out[3]) {
  hc_vec3_div(in, hc_vec3_mag(in), out);
}

/// Linearly interpolates between two vectors by f, storing the result in out.
void hc_vec3_lerp(const float a[3], const float b[3], float f,
                  float out[3]) {
  out[0] = a[0] * (1.0 - f) + (b[0] * f);
  out[1] = a[1] * (1.0 - f) + (b[1] * f);
  out[2] = a[2] * (1.0 - f) + (b[2] * f);
}

/// Returns the dot product of two vectors
float hc_vec3_dot(const float a[3], const float b[3]) {
  return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}
