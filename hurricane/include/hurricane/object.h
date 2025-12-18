#ifndef HC_OBJECT_H
#define HC_OBJECT_H

#include "geometry.h"
#include "util/quat.h"

typedef struct hc_object {
  float position[3];
  hc_quaternion rotation;
  float scale[3];
  const hc_geometry *geometry;
} hc_object;

void hc_new_object(hc_object *object, const hc_geometry *geometry, const float position[3], const hc_quaternion rotation, const float scale[3]);

#endif
