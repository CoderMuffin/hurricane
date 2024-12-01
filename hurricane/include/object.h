#ifndef HC_OBJECT_H
#define HC_OBJECT_H

//macros
#include "geometry.h"
#include "util/quat.h"
#include "util/vec.h"
#include <stdlib.h>

//types
typedef struct hc_object {
  double position[3];
  hc_quaternion rotation;
  double scale[3];
  const hc_geometry *geometry;
} hc_object;

//decls

//functions
void hc_object_new(hc_object *object, const hc_geometry *geometry, const double position[3], const hc_quaternion rotation, const double scale[3]);

#endif
