#ifndef HC_GEOMETRY_H
#define HC_GEOMETRY_H

#include <stdio.h>

typedef struct hc_geometry {
  float *vertices;
  unsigned int *faces;
  unsigned char *colors;
  unsigned int face_count;
} hc_geometry;

int hc_getline(char **lineptr, size_t *n, FILE *stream);

extern const hc_geometry hc_geometry_none;
extern const hc_geometry hc_geometry_sphere5;
extern const hc_geometry hc_geometry_cube;
extern const hc_geometry hc_geometry_mobius;

#endif
