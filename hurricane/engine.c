#ifndef HC_ENGINE
#define HC_ENGINE
#include "object.c"
#include "renderer/renderer.c"
#include "shared.c"
#include "util/mat.c"
#include "util/vec.c"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

bool hc_internal_quit = false;
int debug_triangle_count = 0;
hc_renderer hc_internal_engine_renderer;

double hc_internal_eye_dist = 8;
void (*hc_render_triangle_call)(int, int, double, int, int, double, int, int,
                                double, unsigned char, unsigned char,
                                unsigned char);

double hc_internal_frame_tmp_vec[3];
double hc_internal_world_transform_tmp_vec[3];

void hc_geometry_to_world(hc_object *object, int index, double out[3]) {
  hc_vec3_vmul(object->geometry->vertices + object->geometry->faces[index] * 3,
               object->scale, out);
  hc_quaternion_rotate(&object->rotation, out, out);
  hc_vec3_add(out, object->position, out);
}

void hc_world_to_screen(const hc_object *camera, const double p_world[3],
                        int out[2]) {
  hc_vec3_sub(p_world, camera->position, hc_internal_world_transform_tmp_vec);
  hc_quaternion_rotate_inverse(&camera->rotation,
                               hc_internal_world_transform_tmp_vec,
                               hc_internal_world_transform_tmp_vec);

  out[0] = hc_internal_world_transform_tmp_vec[0] * hc_internal_eye_dist /
               hc_internal_world_transform_tmp_vec[2] +
           HC_RENDER_SIZE_X / 2;
  out[1] = hc_internal_world_transform_tmp_vec[1] * hc_internal_eye_dist /
               hc_internal_world_transform_tmp_vec[2] +
           HC_RENDER_SIZE_Y / 2;
}

void hc_render_object(hc_object *camera, hc_object *object) {
  int a[2], b[2], c[2];
  for (int i = 0; i < object->geometry->face_count; i += 3) {
    hc_geometry_to_world(object, i, hc_internal_frame_tmp_vec);
    hc_world_to_screen(camera, hc_internal_frame_tmp_vec, a);
    double az = hc_internal_frame_tmp_vec[2];

    hc_geometry_to_world(object, i + 1, hc_internal_frame_tmp_vec);
    hc_world_to_screen(camera, hc_internal_frame_tmp_vec, b);
    double bz = hc_internal_frame_tmp_vec[2];

    hc_geometry_to_world(object, i + 2, hc_internal_frame_tmp_vec);
    hc_world_to_screen(camera, hc_internal_frame_tmp_vec, c);
    double cz = hc_internal_frame_tmp_vec[2];

    debug_triangle_count++;

    hc_internal_engine_renderer.triangle(a[0], a[1], az, b[0], b[1], bz, c[0], c[1], cz,
                            object->geometry->colors[i],
                            object->geometry->colors[i + 1],
                            object->geometry->colors[i + 2]);
  }
}

void hc_set_fov(double fov, bool use_height) {
  hc_internal_eye_dist = (use_height ? HC_RENDER_SIZE_Y : HC_RENDER_SIZE_X) /
                         (2 * tan(fov * M_PI / 360));
}

void hc_init(const bool hc_render_progress, int frames, hc_renderer renderer, void (*update)()) {
  hc_internal_engine_renderer = renderer;
  int i = 0;
  while (!hc_internal_quit && (frames == -1 || i++ < frames)) {
    debug_triangle_count = 0;
    hc_internal_engine_renderer.pre_frame();
    update();
    printf("Triangles: %d");
    hc_internal_engine_renderer.frame();
    if (hc_render_progress) {
      printf("\rProcessing frame %d/%d", i, frames);
      fflush(stdout);
    }
  }
}

#endif
