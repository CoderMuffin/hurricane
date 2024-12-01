#ifndef HC_QUATERNION_H
#define HC_QUATERNION_H

//macros
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#define QUATERNION_EPS (1e-4)
#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

//types
typedef struct hc_quaternion {
  double w;    /**< Scalar part */
  double v[3]; /**< Vector part */
} hc_quaternion;

//decls
extern const hc_quaternion hc_quaternion_identity;

//functions
void hc_quaternion_set_identity(hc_quaternion *q);
void hc_quaternion_invert(const hc_quaternion *q, hc_quaternion *output);
void hc_quaternion_copy(const hc_quaternion *q, hc_quaternion *output);
bool hc_quaternion_eq(const hc_quaternion *q1, const hc_quaternion *q2);
void hc_quaternion_fprint(FILE *file, hc_quaternion *q);
void hc_quaternion_from_axis_angle(const double axis[3], const double angle, hc_quaternion *output);
double hc_quaternion_to_axis_angle(hc_quaternion *q, double output[3]);
void hc_quaternion_from_x_rotation(double angle, hc_quaternion *output);
void hc_quaternion_from_y_rotation(double angle, hc_quaternion *output);
void hc_quaternion_from_z_rotation(double angle, hc_quaternion *output);
hc_quaternion hc_quaternion_new_euler_zyx(double eulerZYX[3]);
void hc_quaternion_from_euler_zyx(double eulerZYX[3], hc_quaternion *output);
void hc_quaternion_to_euler_zyx(hc_quaternion *q, double output[3]);
void hc_quaternion_conjugate(hc_quaternion *q, hc_quaternion *output);
double hc_quaternion_norm(hc_quaternion *q);
void hc_quaternion_normalize(hc_quaternion *q, hc_quaternion *output);
void hc_quaternion_mul(hc_quaternion *q1, hc_quaternion *q2, hc_quaternion *output);
void hc_quaternion_rotate(const hc_quaternion *q, double v[3], double output[3]);
void hc_quaternion_rotate_inverse(const hc_quaternion *q, double v[3], double output[3]);
void hc_quaternion_slerp(hc_quaternion *q1, hc_quaternion *q2, double t, hc_quaternion *output);

#endif
