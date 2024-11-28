#ifndef HC_ENGINE
#define HC_ENGINE

#include <hurricane/object.h>
#include <hurricane/renderer/renderer.h>
#include <hurricane/shared.h>
#include <hurricane/util/mat.h>
#include <hurricane/util/vec.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

static bool hc_internal_quit = false;
int debug_triangle_count = 0;
static hc_renderer hc_internal_engine_renderer;

double hc_internal_eye_dist = HC_RENDER_SIZE_X / (2 * 0.7); //70 fov default
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
                        int out[2], double *out_z) {
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
  *out_z = hc_internal_world_transform_tmp_vec[2];
}

double hc_internal_lighting(hc_object *object, int i) {
  double a[3], b[3], c[3], t1[3], t2[3], normal[3];
  hc_quaternion_rotate(&object->rotation, object->geometry->vertices + object->geometry->faces[i] * 3, a);
  hc_quaternion_rotate(&object->rotation, object->geometry->vertices + object->geometry->faces[i + 1] * 3, b);
  hc_quaternion_rotate(&object->rotation, object->geometry->vertices + object->geometry->faces[i + 2] * 3, c);
  hc_vec3_sub(b, a, t1);
  hc_vec3_sub(c, b, t2);
  hc_vec3_cross(t1, t2, normal);
  hc_vec3_normalize(normal, normal);
  return (hc_vec3_dot((double[]){1,0,0}, normal) + 1) / 2;
}

void hc_render_object(hc_object *camera, hc_object *object) {
  int a[2], b[2], c[2];
  double az, bz, cz;
  for (int i = 0; i < object->geometry->face_count * 3; i += 3) {
    hc_geometry_to_world(object, i, hc_internal_frame_tmp_vec);
    hc_world_to_screen(camera, hc_internal_frame_tmp_vec, a, &az);
    double depth_az = INFINITY;
    //    (*hc_internal_engine_renderer.internal_depth_buf)[a[1]][a[0]];

    hc_geometry_to_world(object, i + 1, hc_internal_frame_tmp_vec);
    hc_world_to_screen(camera, hc_internal_frame_tmp_vec, b, &bz);
    double depth_bz = INFINITY;
    //    (*hc_internal_engine_renderer.internal_depth_buf)[b[1]][b[0]];

    hc_geometry_to_world(object, i + 2, hc_internal_frame_tmp_vec);
    hc_world_to_screen(camera, hc_internal_frame_tmp_vec, c, &cz);
    double depth_cz = INFINITY;
    //    (*hc_internal_engine_renderer.internal_depth_buf)[c[1]][c[0]];

    if (depth_az < az && depth_bz < bz && depth_cz < cz)
      continue;

    debug_triangle_count++;

    double dot = hc_internal_lighting(object, i);

    hc_internal_engine_renderer.triangle(a[0], a[1], az, b[0], b[1], bz, c[0],
                                         c[1], cz, object->geometry->colors[i] * dot,
                                         object->geometry->colors[i + 1] * dot,
                                         object->geometry->colors[i + 2] * dot);
  }
}

void hc_set_fov(double fov, bool use_height) {
  hc_internal_eye_dist = (use_height ? HC_RENDER_SIZE_Y : HC_RENDER_SIZE_X) /
                         (2 * tan(fov * M_PI / 360));
}

void hc_quit() {
  hc_internal_quit = true;
}

void hc_init(const bool hc_render_progress, int frames, hc_renderer renderer,
             void (*update)()) {
  hc_internal_engine_renderer = renderer;
  int i = 0;
  while (!hc_internal_quit && (frames == -1 || i++ < frames)) {
    debug_triangle_count = 0;
    hc_internal_engine_renderer.pre_frame();
    update();
    // printf("Triangles: %d\n", debug_triangle_count);
    hc_internal_engine_renderer.frame();
    if (hc_render_progress) {
      printf("\rProcessing frame %d/%d", i, frames);
      fflush(stdout);
    }
  }
}

#endif
