#ifndef HC_QUATERNION_H
#define HC_QUATERNION_H

#include <stdbool.h>
#include "../shared.h"

typedef struct hc_quaternion {
  float w;    /**< Scalar part */
  float v[3]; /**< Vector part */
} hc_quaternion;

void hc_quaternion_set_identity(hc_quaternion *q);
void hc_quaternion_invert(const hc_quaternion *q, hc_quaternion *output);
void hc_quaternion_copy(const hc_quaternion *q, hc_quaternion *output);
bool hc_quaternion_eq(const hc_quaternion *q1, const hc_quaternion *q2);
float hc_quaternion_to_axis_angle(hc_quaternion *q, float output[3]);
void hc_quaternion_from_x_rotation(float angle, hc_quaternion *output);
void hc_quaternion_from_y_rotation(float angle, hc_quaternion *output);
void hc_quaternion_from_z_rotation(float angle, hc_quaternion *output);
hc_quaternion hc_quaternion_new_euler_zyx(float eulerZYX[3]);
void hc_quaternion_from_euler_zyx(float eulerZYX[3], hc_quaternion *output);
void hc_quaternion_to_euler_zyx(hc_quaternion *q, float output[3]);
void hc_quaternion_conjugate(hc_quaternion *q, hc_quaternion *output);
float hc_quaternion_norm(hc_quaternion *q);
void hc_quaternion_normalize(hc_quaternion *q, hc_quaternion *output);
void hc_quaternion_rotate(const hc_quaternion *q, float v[3], float output[3]);
void hc_quaternion_rotate_inverse(const hc_quaternion *q, float v[3], float output[3]);
void hc_quaternion_slerp(hc_quaternion *q1, hc_quaternion *q2, float t,
                         hc_quaternion *output);

extern const hc_quaternion hc_quaternion_identity;

#endif
