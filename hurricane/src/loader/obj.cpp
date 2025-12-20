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
  
  output->vertices = malloc(sizeof(fixed) * vertex_buffer_size * 3);
  output->faces = malloc(sizeof(int) * face_buffer_size * 3);
  output->colors = malloc(sizeof(int) * 10000 * 3); //todo: needs fix?

  while ((read = hc_getline(&line, &len, fp)) != -1) {
    char *n0, *n1, *n2;
    n0 = strtok(line + 2, " ");
    n1 = strtok(NULL, " ");
    n2 = strtok(NULL, " ");

    if (vertex_buffer_size <= vertex_count) {
      vertex_buffer_size <<= 2;
      output->vertices = realloc(output->vertices, sizeof(fixed) * vertex_buffer_size * 3);
    }
    if (face_buffer_size <= face_count) {
      face_buffer_size <<= 2;
      output->faces = realloc(output->faces, sizeof(fixed) * face_buffer_size * 3);
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
