#include <hurricane/geometry.h>
#include <hurricane/util/quat.h>
#include <hurricane/util/vec.h>
#include <stdlib.h>
#include <assert.h>

typedef struct hc_object {
  double position[3];
  hc_quaternion rotation;
  double scale[3];
  const hc_geometry *geometry;
} hc_object;

void hc_new_object(hc_object *const object, const hc_geometry *const geometry, const double position[3], const hc_quaternion rotation, const double scale[3]) {
  assert(object != NULL);
  hc_vec3_copy(position, object->position);
  hc_quaternion_copy(&rotation, &object->rotation);
  hc_vec3_copy(scale, object->scale);
  object->geometry = geometry;
}
