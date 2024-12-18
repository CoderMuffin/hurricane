#ifndef HC_OBJ
#define HC_OBJ

#include <stdlib.h>
#include <string.h>
#include <hurricane/geometry.h>
#include <hurricane/util/log.h>

void hc_geometry_from_obj(char *file, hc_geometry *output) {
  FILE *fp = fopen(file, "r");
  if (fp == NULL) {
    hc_internal_error("Could not open file '%s'", file);
    return;
  }

  char *line = NULL;
  size_t len = 0;
  int read;
  
  int vertex_count = 0;
  int vertex_buffer_size = 10;
  int vertex_pointer = 0;

  int face_buffer_size = 10;
  int face_count = 0;
  int face_pointer = 0;
  
  output->vertices = malloc(sizeof(double) * vertex_buffer_size * 3);
  output->faces = malloc(sizeof(int) * face_buffer_size * 3);
  output->colors = malloc(sizeof(int) * 10000 * 3); //todo: needs fix?

  while ((read = hc_getline(&line, &len, fp)) != -1) {
    char *n0, *n1, *n2, *n3;
    n0 = strtok(line + 2, " ");
    n1 = strtok(NULL, " ");
    n2 = strtok(NULL, " ");

    if (vertex_buffer_size <= vertex_count) {
      vertex_buffer_size <<= 2;
      output->vertices = realloc(output->vertices, sizeof(double) * vertex_buffer_size * 3);
    }
    if (face_buffer_size <= face_count) {
      face_buffer_size <<= 2;
      output->faces = realloc(output->faces, sizeof(double) * face_buffer_size * 3);
    }

    if (line[0] == 'v') {
      output->vertices[vertex_pointer++] = atof(n0);
      output->vertices[vertex_pointer++] = atof(n1);
      output->vertices[vertex_pointer++] = atof(n2);
      vertex_count++;
    } else if (line[0] == 'f') {
      output->colors[face_pointer+0] = 0;
      output->colors[face_pointer+1] = 200;
      output->colors[face_pointer+2] = 255;

      output->faces[face_pointer+0] = atoi(n0)-1;
      output->faces[face_pointer+1] = atoi(n1)-1;
      output->faces[face_pointer+2] = atoi(n2)-1;
      face_pointer+=3;

      face_count++;
    } else if (len == 0 || line[0] == '\n') {
      // pass
    } else {
      hc_internal_warn("Unknown character '%d' in obj file reading '%s'", line[0], file);
    }
  }
  output->face_count = face_count;

  fclose(fp);
  if (line)
    free(line);
}
//   fastObjMesh *m = fast_obj_read(file);
//   output->face_count = m->face_count;
//   printf("%d\n", m->face_count);
//   output->faces = malloc(sizeof(unsigned int) * m->face_count * 3);
//   output->vertices = malloc(sizeof(double) * (m->position_count + 100) * 3);
//   output->colors = malloc(sizeof(unsigned char) * m->face_count * 3);
//   // for (int i = 0; i < m->face_count * 3; i += 3) {
//   //   output->faces[i] = m->face_vertices[i];
//   //   output->faces[i + 1] = m->face_vertices[i + 1];
//   //   output->faces[i + 2] = m->face_vertices[i + 2];
//   //   //printf("%d %d %d %d %d %d\n", output->faces[i], output->faces[i +
//   1],
//   //   //       output->faces[i + 2], i, i + 1, i + 2);
//   //   output->colors[i] = 255;
//   //   output->colors[i + 1] = 255;
//   //   output->colors[i + 2] = 0;
//   // }
//   // for (int i = 0; i < m->position_count; i++) {
//   //   output->vertices[i] = m->positions[i];
//   // }
//   for (unsigned int ii = 0; ii < m->group_count; ii++) {
//     const fastObjGroup grp = m->groups[ii];
//     printf("Group %d\n", ii);
//     int idx = 0;
//     for (unsigned int jj = 0; jj < grp.face_count; jj++) {
//       unsigned int fv = m->face_vertices[grp.face_offset + jj];

//       for (unsigned int kk = 0; kk < fv; kk++) {
//         fastObjIndex mi = m->indices[grp.index_offset + idx];
//         if (mi.p) {
//           output->vertices[idx] = m->positions[3 * mi.p + 0];
//         }
//         idx++;
//       }
//     }
//   }
//   fast_obj_destroy(m);
// }
#endif
