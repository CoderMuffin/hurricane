#include <hurricane/util/quat.h>
#include <assert.h>
#include <hurricane/fixed.h>
#include <taylor_math.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define QUATERNION_EPS tf(1e-4)

const hc_quaternion hc_quaternion_identity = {tf(1), {tf(0), tf(0), tf(0)}};

void hc_quaternion_set(fixed w, fixed v1, fixed v2, fixed v3,
                       hc_quaternion *output) {
  assert(output != NULL);
  output->w = w;
  output->v[0] = v1;
  output->v[1] = v2;
  output->v[2] = v3;
}

void hc_quaternion_set_identity(hc_quaternion *q) {
  assert(q != NULL);
  hc_quaternion_set(tf(1), tf(0), tf(0), tf(0), q);
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

void hc_quaternion_from_axis_angle(const fixed axis[3], const fixed angle,
                                   hc_quaternion *output) {
  assert(output != NULL);
  // Formula from
  // http://www.euclideanspace.com/maths/geometry/rotations/conversions/angleTohc_quaternion/
  output->w = fx_cos(fd(angle, tf(2.0)));
  fixed c = fx_sin(fd(angle, tf(2.0)));
  output->v[0] = fm(c, axis[0]);
  output->v[1] = fm(c, axis[1]);
  output->v[2] = fm(c, axis[2]);
}

void hc_quaternion_from_euler_zyx(fixed eulerZYX[3], hc_quaternion *output) {
  assert(output != NULL);
  // Based on
  // https://en.wikipedia.org/wiki/Conversion_between_hc_quaternions_and_Euler_angles
  fixed cy = fx_cos(fd(eulerZYX[2], tf(2)));
  fixed sy = fx_sin(fd(eulerZYX[2], tf(2)));
  fixed cr = fx_cos(fd(eulerZYX[0], tf(2)));
  fixed sr = fx_sin(fd(eulerZYX[0], tf(2)));
  fixed cp = fx_cos(fd(eulerZYX[1], tf(2)));
  fixed sp = fx_sin(fd(eulerZYX[1], tf(2)));

  output->w = fm(fm(cy, cr), cp) + fm(fm(sy, sr), sp);
  output->v[0] = fm(fm(cy, sr), cp) - fm(fm(sy, cr), sp);
  output->v[1] = fm(fm(cy, cr), sp) + fm(fm(sy, sr), cp);
  output->v[2] = fm(fm(sy, cr), cp) - fm(fm(cy, sr), sp);
}

void hc_quaternion_to_euler_zyx(hc_quaternion *q, fixed output[3]) {
  assert(output != NULL);

  // Roll (x-axis rotation)
  fixed sinr_cosp = fm(tf(+2.0), fm(q->w, q->v[0]) + fm(q->v[1], q->v[2]));
  fixed cosr_cosp = tf(+1.0) - fm(tf(2.0), fm(q->v[0], q->v[0]) + fm(q->v[1], q->v[1]));
  output[0] = fx_atan2(sinr_cosp, cosr_cosp);

  // Pitch (y-axis rotation)
  fixed sinp = fm(tf(+2.0), fm(q->w, q->v[1]) - fm(q->v[2], q->v[0]));
  if (fx_abs(sinp) >= tf(1))
    output[1] = fx_copysign(fd(tf(M_PI), tf(2)), sinp); // use 90 degrees if out of range
  else
    output[1] = fx_asin(sinp);

  // Yaw (z-axis rotation)
  fixed siny_cosp = fm(tf(+2.0), fm(q->w, q->v[2]) + fm(q->v[0], q->v[1]));
  fixed cosy_cosp = tf(+1.0) - fm(tf(2.0), fm(q->v[1], q->v[1]) + fm(q->v[2], q->v[2]));
  output[2] = fx_atan2(siny_cosp, cosy_cosp);
}

void hc_quaternion_conjugate(hc_quaternion *q, hc_quaternion *output) {
  assert(output != NULL);
  output->w = q->w;
  output->v[0] = -q->v[0];
  output->v[1] = -q->v[1];
  output->v[2] = -q->v[2];
}

void hc_quaternion_rotate(const hc_quaternion *q, fixed v[3], fixed output[3]) {
  assert(output != NULL);
  fixed result[3];

  fixed ww = fm(q->w, q->w);
  fixed xx = fm(q->v[0], q->v[0]);
  fixed yy = fm(q->v[1], q->v[1]);
  fixed zz = fm(q->v[2], q->v[2]);
  fixed wx = fm(q->w, q->v[0]);
  fixed wy = fm(q->w, q->v[1]);
  fixed wz = fm(q->w, q->v[2]);
  fixed xy = fm(q->v[0], q->v[1]);
  fixed xz = fm(q->v[0], q->v[2]);
  fixed yz = fm(q->v[1], q->v[2]);

  result[0] = fm(ww, v[0]) + fm(tf(2), fm(wy, v[2])) - fm(tf(2), fm(wz, v[1])) + fm(xx, v[0]) +
              fm(tf(2), fm(xy, v[1])) + fm(tf(2), fm(xz, v[2])) - fm(zz, v[0]) - fm(yy, v[0]);
  result[1] = fm(tf(2), fm(xy, v[0])) + fm(yy, v[1]) + fm(tf(2), fm(yz, v[2])) + fm(tf(2), fm(wz, v[0])) -
              fm(zz, v[1]) + fm(ww, v[1]) - fm(tf(2), fm(wx, v[2])) - fm(xx, v[1]);
  result[2] = fm(tf(2), fm(xz, v[0])) + fm(tf(2), fm(yz, v[1])) + fm(zz, v[2]) - fm(tf(2), fm(wy, v[0])) -
              fm(yy, v[2]) + fm(tf(2), fm(wx, v[1])) - fm(xx, v[2]) + fm(ww, v[2]);

  // Copy result to output
  output[0] = result[0];
  output[1] = result[1];
  output[2] = result[2];
}

void hc_quaternion_rotate_inverse(const hc_quaternion *q, fixed v[3], fixed output[3]) {
  hc_quaternion q_inv;
  hc_quaternion_invert(q, &q_inv);
  hc_quaternion_rotate(&q_inv, v, output);
}

hc_quaternion hc_quaternion_new_euler_zyx(fixed eulerZYX[3]) {
  hc_quaternion q;
  hc_quaternion_from_euler_zyx(eulerZYX, &q);
  return q;
}

void hc_quaternion_slerp(hc_quaternion *q1, hc_quaternion *q2, fixed t,
                         hc_quaternion *output) {
  hc_quaternion result;

  // Based on
  // http://www.euclideanspace.com/maths/algebra/realNormedAlgebra/hc_quaternions/slerp/index.htm
  fixed cosHalfTheta = fm(q1->w, q2->w) + fm(q1->v[0], q2->v[0]) +
                        fm(q1->v[1], q2->v[1]) + fm(q1->v[2], q2->v[2]);

  // if q1=q2 or qa=-q2 then theta = 0 and we can return qa
  if (fx_abs(cosHalfTheta) >= tf(1.0)) {
    hc_quaternion_copy(q1, output);
    return;
  }

  fixed halfTheta = fx_acos(cosHalfTheta);
  fixed sinHalfTheta = fx_sqrt(tf(1.0) - fm(cosHalfTheta, cosHalfTheta));
  // If theta = 180 degrees then result is not fully defined
  // We could rotate around any axis normal to q1 or q2
  fixed ratioA = fd(fx_sin(fm(tf(1) - t, halfTheta)), sinHalfTheta);
  fixed ratioB = fd(fx_sin(fm(t, halfTheta)), sinHalfTheta);
  if (fx_abs(sinHalfTheta) < QUATERNION_EPS) {
    ratioA = tf(0.5);
    ratioB = tf(0.5);
  }

  result.w = fm(q1->w, ratioA) + fm(q2->w, ratioB);
  result.v[0] = fm(q1->v[0], ratioA) + fm(q2->v[0], ratioB);
  result.v[1] = fm(q1->v[1], ratioA) + fm(q2->v[1], ratioB);
  result.v[2] = fm(q1->v[2], ratioA) + fm(q2->v[2], ratioB);
    
  *output = result;
}
