#ifndef HC_GEOMETRY_H
#define HC_GEOMETRY_H

#include "util/vec.h"
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct hc_geometry {
  double *vertices;
  unsigned int *faces;
  unsigned char *colors;
  unsigned int face_count;
} hc_geometry;

int hc_getline(char **lineptr, size_t *n, FILE *stream);

#endif
