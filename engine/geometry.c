#ifndef HC_GEOMETRY
#define HC_GEOMETRY
#include "util/vec.c"
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct hc_geometry {
  double *vertices;
  int *faces;
  unsigned char *colors;
  unsigned int face_count;
} hc_geometry;

hc_geometry *hc_geometry_cube;

void hc_init_geometries() {
  hc_geometry_cube = malloc(sizeof(hc_geometry));
  double vertices[8*3] = {
    -1, -1, -1, //
    1, -1, -1, //
    1, 1, -1, //
    -1, 1, -1, //
    -1, 1, 1, //   
    1, 1, 1, //
    1, -1, 1, //
    -1, -1, 1, //
  };
  int triangles[36] = {
      0, 2, 1, // face front
      0, 3, 2, //
      2, 3, 4, // face top
      2, 4, 5, //
      1, 2, 5, // face right
      1, 5, 6, //
      0, 7, 4, // face left
      0, 4, 3, //
      5, 4, 7, // face back
      5, 7, 6, //
      0, 6, 7, // face bottom
      0, 1, 6, //
  };
  unsigned char colors[36] = {
      0,   0,   255, // face front
      0,   0,   255, //
      0,   255, 0,   // face top
      0,   255, 0,   //
      255, 0,   0,   // face right
      255, 0,   0,   //
      0,   255, 255, // face left
      0,   255, 255, //
      255, 255, 0,   // face back
      255, 255, 0,   //
      255, 0,   255, // face bottom
      255, 0,   255, //
  };
  hc_geometry_cube->vertices = malloc(sizeof(double) * 3 * 8);
  hc_geometry_cube->faces = malloc(sizeof(int) * 36);
  hc_geometry_cube->colors = malloc(sizeof(unsigned char) * 36);
  hc_geometry_cube->face_count = 36;
  memcpy(hc_geometry_cube->vertices, vertices, sizeof(vertices));
  memcpy(hc_geometry_cube->faces, triangles, sizeof(triangles));
  memcpy(hc_geometry_cube->colors, colors, sizeof(colors));
}

#endif
