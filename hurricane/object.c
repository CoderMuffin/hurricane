#ifndef HC_OBJECT
#define HC_OBJECT
#include "include/geometry.h"
#include "include/util/quat.h"
#include "include/object.h"
#include "include/util/vec.h"
#include <stdlib.h>

void hc_object_new(hc_object *object, const hc_geometry *geometry, const double position[3], const hc_quaternion rotation, const double scale[3]) {
  assert(object != NULL);
  hc_vec3_copy(position, object->position);
  hc_quaternion_copy(&rotation, &object->rotation);
  hc_vec3_copy(scale, object->scale);
  object->geometry = geometry;
}

#endif
