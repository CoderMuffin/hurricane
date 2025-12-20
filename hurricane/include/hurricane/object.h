#ifndef HC_OBJECT_H
#define HC_OBJECT_H

#include "geometry.h"
#include "util/quat.h"

typedef struct hc_object {
  fixed position[3];
  hc_quaternion rotation;
  fixed scale[3];
  const hc_geometry *geometry;
} hc_object;

void hc_new_object(hc_object *object, const hc_geometry *geometry, const fixed position[3], const hc_quaternion rotation, const fixed scale[3]);

#endif
