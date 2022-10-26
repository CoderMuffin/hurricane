#ifndef HC_ENGINE
#define HC_ENGINE
#include "object.c"
#include "shared.c"
#include "util/vec.c"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

double hc_eye_dist = 8;
void (*hc_render_triangle_call)(int, int, int, int, int, int, unsigned char,
                                unsigned char, unsigned char);

double hc_frame_tmp_vec[3];

void hc_render_object(hc_object *object) {
  for (int i = 0; i < object->geometry->face_count; i += 3) {
    hc_quaternion_rotate(&object->rotation,
                         object->geometry->vertices +
                             object->geometry->faces[i] * 3,
                         hc_frame_tmp_vec);
    // printf("%f %f %f %f %f %f\n",
    // p[0],p[1],p[2],hc_frame_tmp_vec[0],hc_frame_tmp_vec[1],hc_frame_tmp_vec[2]);exit(1);
    // printf("%f %f %f %f %f %f\n",
    // object->position[0],object->position[1],object->position[2],
    // hc_frame_tmp_vec[0],hc_frame_tmp_vec[1],hc_frame_tmp_vec[2]);
    hc_vec3_add(hc_frame_tmp_vec, object->position, hc_frame_tmp_vec);
    // printf("%f %f %f %f %f %f\n",
    // object->position[0],object->position[1],object->position[2],
    // hc_frame_tmp_vec[0],hc_frame_tmp_vec[1],hc_frame_tmp_vec[2]); exit(1);
    int ax = hc_frame_tmp_vec[0] * (hc_eye_dist / hc_frame_tmp_vec[2]) +
             HC_RENDER_SIZE_X / 2;
    int ay = hc_frame_tmp_vec[1] * (hc_eye_dist / hc_frame_tmp_vec[2]) +
             HC_RENDER_SIZE_Y / 2;

    hc_quaternion_rotate(&object->rotation,
                         object->geometry->vertices +
                             object->geometry->faces[i + 1] * 3,
                         hc_frame_tmp_vec);
    hc_vec3_add(hc_frame_tmp_vec, object->position, hc_frame_tmp_vec);
    int bx = hc_frame_tmp_vec[0] * (hc_eye_dist / hc_frame_tmp_vec[2]) +
             HC_RENDER_SIZE_X / 2;
    int by = hc_frame_tmp_vec[1] * (hc_eye_dist / hc_frame_tmp_vec[2]) +
             HC_RENDER_SIZE_Y / 2;

    hc_quaternion_rotate(&object->rotation,
                         object->geometry->vertices +
                             object->geometry->faces[i + 2] * 3,
                         hc_frame_tmp_vec);
    hc_vec3_add(hc_frame_tmp_vec, object->position, hc_frame_tmp_vec);
    int cx = hc_frame_tmp_vec[0] * (hc_eye_dist / hc_frame_tmp_vec[2]) +
             HC_RENDER_SIZE_X / 2;
    int cy = hc_frame_tmp_vec[1] * (hc_eye_dist / hc_frame_tmp_vec[2]) +
             HC_RENDER_SIZE_Y / 2;

    hc_render_triangle_call(ax, ay, bx, by, cx, cy, object->geometry->colors[i],
                            object->geometry->colors[i + 1],
                            object->geometry->colors[i + 2]);
    // hc_draw_buf_line(ax, ay, bx, by);
    // hc_draw_buf_line(ax, ay, cx, cy);
    // hc_draw_buf_line(bx, by, cx, cy);
  }
}

void hc_set_fov(double fov) {
  hc_eye_dist = HC_RENDER_SIZE_X / (2 * tan(fov * M_PI / 360));
}

void hc_init(void (*pre_render_call)(),
             void (*render_triangle_call)(int, int, int, int, int, int,
                                          unsigned char, unsigned char,
                                          unsigned char),
             void (*render_call)(), void (*update)()) {
  hc_render_triangle_call = render_triangle_call;
  int i = 0;
  while (i++ < 250) {
    pre_render_call();
    update();
    render_call();
  }
}

#endif
