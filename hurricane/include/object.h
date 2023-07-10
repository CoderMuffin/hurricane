#ifndef HC_OBJECT_H
#define HC_OBJECT_H

#include "geometry.h"
#include "util/quat.h"
#include "util/vec.h"
#include <stdlib.h>

typedef struct hc_object {
  double position[3];
  hc_quaternion rotation;
  double scale[3];
  const hc_geometry *geometry;
} hc_object;

void hc_new_object(hc_object *object, const hc_geometry *geometry, const double position[3], const hc_quaternion rotation, const double scale[3]);

#endif
