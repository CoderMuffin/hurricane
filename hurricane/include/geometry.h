#ifndef HC_GEOMETRY_H
#define HC_GEOMETRY_H

//macros
#include "util/vec.h"
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//types
typedef struct hc_geometry {
  double *vertices;
  unsigned int *faces;
  unsigned char *colors;
  unsigned int face_count;
} hc_geometry;

//decls
extern hc_geometry hc_geometry_none;
extern hc_geometry hc_geometry_sphere5;
extern hc_geometry hc_geometry_cube;
extern hc_geometry hc_geometry_mobius;

//functions
int hc_getline(char **lineptr, size_t *n, FILE *stream);

#endif
