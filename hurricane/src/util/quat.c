#include <hurricane/util/quat.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define QUATERNION_EPS (1e-4)

const hc_quaternion hc_quaternion_identity = {1, {0, 0, 0}};

void hc_quaternion_set(double w, double v1, double v2, double v3,
                       hc_quaternion *output) {
  assert(output != NULL);
  output->w = w;
  output->v[0] = v1;
  output->v[1] = v2;
  output->v[2] = v3;
}

void hc_quaternion_set_identity(hc_quaternion *q) {
  assert(q != NULL);
  hc_quaternion_set(1, 0, 0, 0, q);
}

void hc_quaternion_invert(const hc_quaternion *q, hc_quaternion *output) {
  output->w = q->w;
  output->v[0] = -q->v[0];
  output->v[1] = -q->v[1];
  output->v[2] = -q->v[2];
}

void hc_quaternion_copy(const hc_quaternion *q, hc_quaternion *output) {
  hc_quaternion_set(q->w, q->v[0], q->v[1], q->v[2], output);
}

bool hc_quaternion_eq(const hc_quaternion *q1, const hc_quaternion *q2) {
  bool equalW = fabs(q1->w - q2->w) <= QUATERNION_EPS;
  bool equalV0 = fabs(q1->v[0] - q2->v[0]) <= QUATERNION_EPS;
  bool equalV1 = fabs(q1->v[1] - q2->v[1]) <= QUATERNION_EPS;
  bool equalV2 = fabs(q1->v[2] - q2->v[2]) <= QUATERNION_EPS;
  return equalW && equalV0 && equalV1 && equalV2;
}

void hc_quaternion_fprint(FILE *file, hc_quaternion *q) {
  fprintf(file, "(%.3f, %.3f, %.3f, %.3f)", q->w, q->v[0], q->v[1], q->v[2]);
}

void hc_quaternion_from_axis_angle(const double axis[3], const double angle,
                                   hc_quaternion *output) {
  assert(output != NULL);
  // Formula from
  // http://www.euclideanspace.com/maths/geometry/rotations/conversions/angleTohc_quaternion/
  output->w = cos(angle / 2.0);
  double c = sin(angle / 2.0);
  output->v[0] = c * axis[0];
  output->v[1] = c * axis[1];
  output->v[2] = c * axis[2];
}

double hc_quaternion_to_axis_angle(hc_quaternion *q, double output[3]) {
  assert(output != NULL);
  // Formula from
  // http://www.euclideanspace.com/maths/geometry/rotations/conversions/hc_quaternionToAngle/
  double angle = 2.0 * acos(q->w);
  double divider = sqrt(1.0 - q->w * q->w);

  if (divider != 0.0) {
    // Calculate the axis
    output[0] = q->v[0] / divider;
    output[1] = q->v[1] / divider;
    output[2] = q->v[2] / divider;
  } else {
    // Arbitrary normalized axis
    output[0] = 1;
    output[1] = 0;
    output[2] = 0;
  }
  return angle;
}

void hc_quaternion_from_x_rotation(double angle, hc_quaternion *output) {
  assert(output != NULL);
  double axis[3] = {1.0, 0, 0};
  hc_quaternion_from_axis_angle(axis, angle, output);
}

void hc_quaternion_from_y_rotation(double angle, hc_quaternion *output) {
  assert(output != NULL);
  double axis[3] = {0, 1.0, 0};
  hc_quaternion_from_axis_angle(axis, angle, output);
}

void hc_quaternion_from_z_rotation(double angle, hc_quaternion *output) {
  assert(output != NULL);
  double axis[3] = {0, 0, 1.0};
  hc_quaternion_from_axis_angle(axis, angle, output);
}

hc_quaternion hc_quaternion_new_euler_zyx(double eulerZYX[3]) {
  hc_quaternion output;
  // Based on
  // https://en.wikipedia.org/wiki/Conversion_between_hc_quaternions_and_Euler_angles
  double cy = cos(eulerZYX[2] * 0.5);
  double sy = sin(eulerZYX[2] * 0.5);
  double cr = cos(eulerZYX[0] * 0.5);
  double sr = sin(eulerZYX[0] * 0.5);
  double cp = cos(eulerZYX[1] * 0.5);
  double sp = sin(eulerZYX[1] * 0.5);

  output.w = cy * cr * cp + sy * sr * sp;
  output.v[0] = cy * sr * cp - sy * cr * sp;
  output.v[1] = cy * cr * sp + sy * sr * cp;
  output.v[2] = sy * cr * cp - cy * sr * sp;

  return output;
}

void hc_quaternion_from_euler_zyx(double eulerZYX[3], hc_quaternion *output) {
  assert(output != NULL);
  // Based on
  // https://en.wikipedia.org/wiki/Conversion_between_hc_quaternions_and_Euler_angles
  double cy = cos(eulerZYX[2] * 0.5);
  double sy = sin(eulerZYX[2] * 0.5);
  double cr = cos(eulerZYX[0] * 0.5);
  double sr = sin(eulerZYX[0] * 0.5);
  double cp = cos(eulerZYX[1] * 0.5);
  double sp = sin(eulerZYX[1] * 0.5);

  output->w = cy * cr * cp + sy * sr * sp;
  output->v[0] = cy * sr * cp - sy * cr * sp;
  output->v[1] = cy * cr * sp + sy * sr * cp;
  output->v[2] = sy * cr * cp - cy * sr * sp;
}

void hc_quaternion_to_euler_zyx(hc_quaternion *q, double output[3]) {
  assert(output != NULL);

  // Roll (x-axis rotation)
  double sinr_cosp = +2.0 * (q->w * q->v[0] + q->v[1] * q->v[2]);
  double cosr_cosp = +1.0 - 2.0 * (q->v[0] * q->v[0] + q->v[1] * q->v[1]);
  output[0] = atan2(sinr_cosp, cosr_cosp);

  // Pitch (y-axis rotation)
  double sinp = +2.0 * (q->w * q->v[1] - q->v[2] * q->v[0]);
  if (fabs(sinp) >= 1)
    output[1] = copysign(M_PI / 2, sinp); // use 90 degrees if out of range
  else
    output[1] = asin(sinp);

  // Yaw (z-axis rotation)
  double siny_cosp = +2.0 * (q->w * q->v[2] + q->v[0] * q->v[1]);
  double cosy_cosp = +1.0 - 2.0 * (q->v[1] * q->v[1] + q->v[2] * q->v[2]);
  output[2] = atan2(siny_cosp, cosy_cosp);
}

void hc_quaternion_conjugate(hc_quaternion *q, hc_quaternion *output) {
  assert(output != NULL);
  output->w = q->w;
  output->v[0] = -q->v[0];
  output->v[1] = -q->v[1];
  output->v[2] = -q->v[2];
}

double hc_quaternion_norm(hc_quaternion *q) {
  assert(q != NULL);
  return sqrt(q->w * q->w + q->v[0] * q->v[0] + q->v[1] * q->v[1] +
              q->v[2] * q->v[2]);
}

void hc_quaternion_normalize(hc_quaternion *q, hc_quaternion *output) {
  assert(output != NULL);
  double len = hc_quaternion_norm(q);
  hc_quaternion_set(q->w / len, q->v[0] / len, q->v[1] / len, q->v[2] / len,
                    output);
}

void hc_quaternion_mul(hc_quaternion *q1, hc_quaternion *q2,
                            hc_quaternion *output) {
  assert(output != NULL);
  hc_quaternion result;

  /*
  Formula from
  http://www.euclideanspace.com/maths/algebra/realNormedAlgebra/hc_quaternions/arithmetic/index.htm
           a*e - b*f - c*g - d*h
      + i (b*e + a*f + c*h- d*g)
      + j (a*g - b*h + c*e + d*f)
      + k (a*h + b*g - c*f + d*e)
  */
  result.w = q1->w * q2->w - q1->v[0] * q2->v[0] - q1->v[1] * q2->v[1] -
             q1->v[2] * q2->v[2];
  result.v[0] = q1->v[0] * q2->w + q1->w * q2->v[0] + q1->v[1] * q2->v[2] -
                q1->v[2] * q2->v[1];
  result.v[1] = q1->w * q2->v[1] - q1->v[0] * q2->v[2] + q1->v[1] * q2->w +
                q1->v[2] * q2->v[0];
  result.v[2] = q1->w * q2->v[2] + q1->v[0] * q2->v[1] - q1->v[1] * q2->v[0] +
                q1->v[2] * q2->w;

  *output = result;
}

void hc_quaternion_rotate(const hc_quaternion *q, double v[3], double output[3]) {
  assert(output != NULL);
  double result[3];

  double ww = q->w * q->w;
  double xx = q->v[0] * q->v[0];
  double yy = q->v[1] * q->v[1];
  double zz = q->v[2] * q->v[2];
  double wx = q->w * q->v[0];
  double wy = q->w * q->v[1];
  double wz = q->w * q->v[2];
  double xy = q->v[0] * q->v[1];
  double xz = q->v[0] * q->v[2];
  double yz = q->v[1] * q->v[2];

  // Formula from
  // http://www.euclideanspace.com/maths/algebra/realNormedAlgebra/hc_quaternions/transforms/index.htm
  // p2.x = w*w*p1.x + 2*y*w*p1.z - 2*z*w*p1.y + x*x*p1.x + 2*y*x*p1.y +
  // 2*z*x*p1.z - z*z*p1.x - y*y*p1.x; p2.y = 2*x*y*p1.x + y*y*p1.y + 2*z*y*p1.z
  // + 2*w*z*p1.x - z*z*p1.y + w*w*p1.y - 2*x*w*p1.z - x*x*p1.y; p2.z =
  // 2*x*z*p1.x + 2*y*z*p1.y + z*z*p1.z - 2*w*y*p1.x - y*y*p1.z + 2*w*x*p1.y -
  // x*x*p1.z + w*w*p1.z;

  result[0] = ww * v[0] + 2 * wy * v[2] - 2 * wz * v[1] + xx * v[0] +
              2 * xy * v[1] + 2 * xz * v[2] - zz * v[0] - yy * v[0];
  result[1] = 2 * xy * v[0] + yy * v[1] + 2 * yz * v[2] + 2 * wz * v[0] -
              zz * v[1] + ww * v[1] - 2 * wx * v[2] - xx * v[1];
  result[2] = 2 * xz * v[0] + 2 * yz * v[1] + zz * v[2] - 2 * wy * v[0] -
              yy * v[2] + 2 * wx * v[1] - xx * v[2] + ww * v[2];

  // Copy result to output
  output[0] = result[0];
  output[1] = result[1];
  output[2] = result[2];
}


void hc_quaternion_rotate_inverse(const hc_quaternion *q, double v[3], double output[3]) {
  assert(output != NULL);
  double result[3];

  double ww = q->w * q->w;
  double xx = -q->v[0] * -q->v[0];
  double yy = -q->v[1] * -q->v[1];
  double zz = -q->v[2] * -q->v[2];
  double wx = q->w * -q->v[0];
  double wy = q->w * -q->v[1];
  double wz = q->w * -q->v[2];
  double xy = -q->v[0] * -q->v[1];
  double xz = -q->v[0] * -q->v[2];
  double yz = -q->v[1] * -q->v[2];

  // Formula from
  // http://www.euclideanspace.com/maths/algebra/realNormedAlgebra/hc_quaternions/transforms/index.htm
  // p2.x = w*w*p1.x + 2*y*w*p1.z - 2*z*w*p1.y + x*x*p1.x + 2*y*x*p1.y +
  // 2*z*x*p1.z - z*z*p1.x - y*y*p1.x; p2.y = 2*x*y*p1.x + y*y*p1.y + 2*z*y*p1.z
  // + 2*w*z*p1.x - z*z*p1.y + w*w*p1.y - 2*x*w*p1.z - x*x*p1.y; p2.z =
  // 2*x*z*p1.x + 2*y*z*p1.y + z*z*p1.z - 2*w*y*p1.x - y*y*p1.z + 2*w*x*p1.y -
  // x*x*p1.z + w*w*p1.z;

  result[0] = ww * v[0] + 2 * wy * v[2] - 2 * wz * v[1] + xx * v[0] +
              2 * xy * v[1] + 2 * xz * v[2] - zz * v[0] - yy * v[0];
  result[1] = 2 * xy * v[0] + yy * v[1] + 2 * yz * v[2] + 2 * wz * v[0] -
              zz * v[1] + ww * v[1] - 2 * wx * v[2] - xx * v[1];
  result[2] = 2 * xz * v[0] + 2 * yz * v[1] + zz * v[2] - 2 * wy * v[0] -
              yy * v[2] + 2 * wx * v[1] - xx * v[2] + ww * v[2];

  // Copy result to output
  output[0] = result[0];
  output[1] = result[1];
  output[2] = result[2];
}

void hc_quaternion_slerp(hc_quaternion *q1, hc_quaternion *q2, double t,
                         hc_quaternion *output) {
  hc_quaternion result;

  // Based on
  // http://www.euclideanspace.com/maths/algebra/realNormedAlgebra/hc_quaternions/slerp/index.htm
  double cosHalfTheta = q1->w * q2->w + q1->v[0] * q2->v[0] +
                        q1->v[1] * q2->v[1] + q1->v[2] * q2->v[2];

  // if q1=q2 or qa=-q2 then theta = 0 and we can return qa
  if (fabs(cosHalfTheta) >= 1.0) {
    hc_quaternion_copy(q1, output);
    return;
  }

  double halfTheta = acos(cosHalfTheta);
  double sinHalfTheta = sqrt(1.0 - cosHalfTheta * cosHalfTheta);
  // If theta = 180 degrees then result is not fully defined
  // We could rotate around any axis normal to q1 or q2
  if (fabs(sinHalfTheta) < QUATERNION_EPS) {
    result.w = (q1->w * 0.5 + q2->w * 0.5);
    result.v[0] = (q1->v[0] * 0.5 + q2->v[0] * 0.5);
    result.v[1] = (q1->v[1] * 0.5 + q2->v[1] * 0.5);
    result.v[2] = (q1->v[2] * 0.5 + q2->v[2] * 0.5);
  } else {
    // Default hc_quaternion calculation
    double ratioA = sin((1 - t) * halfTheta) / sinHalfTheta;
    double ratioB = sin(t * halfTheta) / sinHalfTheta;
    result.w = (q1->w * ratioA + q2->w * ratioB);
    result.v[0] = (q1->v[0] * ratioA + q2->v[0] * ratioB);
    result.v[1] = (q1->v[1] * ratioA + q2->v[1] * ratioB);
    result.v[2] = (q1->v[2] * ratioA + q2->v[2] * ratioB);
  }
  *output = result;
}
