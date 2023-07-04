#ifndef HC_MAT
#define HC_MAT
#include "vec.c"

void hc_mat4x4_identity(double out[4][4]) {
  for (short i = 0; i < 4; ++i) {
    for (short j = 0; j < 4; ++j) {
      out[i][j] = i == j;
    }
  }
}

void hc_mat4x4_print(const double mat[4][4]) {
  for (short i = 0; i < 4; ++i) {
    printf("%f %f %f %f\n", mat[i][0], mat[i][1], mat[i][2], mat[i][3]);
  }
}

void hc_mat4x4_mul(double a[4][4], double b[4][4], double out[4][4]) {
  for (short i = 0; i < 4; ++i) {
    for (short j = 0; j < 4; ++j) {
      out[i][j] = a[i][0] * b[0][j] + a[i][1] * b[1][j] + a[i][2] * b[2][j] +
                  a[i][3] * b[3][j];
    }
  }
}

void hc_mat4x4_perspective(float fov, float aspect, float nearDist,
                           float farDist, int leftHanded, double out[4][4]) {
  //
  // General form of the Projection Matrix
  //
  // uh = Cot( fov/2 ) == 1/Tan(fov/2)
  // uw / uh = 1/aspect
  //
  //   uw         0       0       0
  //    0        uh       0       0
  //    0         0      f/(f-n)  1
  //    0         0    -fn/(f-n)  0
  //
  // Make out to be identity first

  // check for bad parameters to avoid divide by zero:
  // if found, assert and return an identity matrix.

  if (fov <= 0 || aspect == 0) {
    assert(fov > 0 && aspect != 0);
    return;
  }

  float frustumDepth = farDist - nearDist;
  float oneOverDepth = 1 / frustumDepth;

  hc_mat4x4_identity(out);

  out[1][1] = 1 / tan(0.5f * fov);
  out[0][0] = (leftHanded ? 1 : -1) * out[1][1] / aspect;
  out[2][2] = farDist * oneOverDepth;
  out[3][2] = (-farDist * nearDist) * oneOverDepth;
  out[2][3] = 1;
  out[3][3] = 0;
}

// double **hc_mat4x4_transpose(double m[4][4]) {
//     double out[4][4] = {
//         { m[0][0], m[1][0], m[2][0], m[3][0], },
//         { m[0][1], m[1][1], m[2][1], m[3][1], },
//         { m[0][2], m[1][2], m[2][2], m[3][2], },
//         { m[0][3], m[1][3], m[2][3], m[3][3] }
//     };
//     return out;
// }

void hc_mat4x4_mul_vec(double m[4][4], double v[3], double out[3]) {
  double a, b, c, w;

  a = v[0] * m[0][0] + v[1] * m[1][0] + v[2] * m[2][0] + m[3][0];
  b = v[0] * m[0][1] + v[1] * m[1][1] + v[2] * m[2][1] + m[3][1];
  c = v[0] * m[0][2] + v[1] * m[1][2] + v[2] * m[2][2] + m[3][2];
  w = v[0] * m[0][3] + v[1] * m[1][3] + v[2] * m[2][3] + m[3][3];

  out[0] = a / w;
  out[1] = b / w;
  out[2] = c / w;
}

#endif