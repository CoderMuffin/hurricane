#ifndef HC_OBJECT
#define HC_OBJECT
#include "geometry.c"
#include "util/quat.c"
#include "util/vec.c"
#include <stdlib.h>

typedef struct hc_object {
  double position[3];
  hc_quaternion rotation;
  double scale[3];
  hc_geometry *geometry;
} hc_object;

hc_object *hc_cube(double pos[3]) {
  hc_object *cube = malloc(sizeof(hc_object));
  hc_vec3_copy(pos, cube->position);
  hc_quaternion_set_identity(&cube->rotation);
  cube->geometry = hc_geometry_cube;
  return cube;
}

#endif
