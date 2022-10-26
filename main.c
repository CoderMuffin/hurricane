
#include "engine/shared.c"
// #undef HC_RENDER_SIZE_X
// #define HC_RENDER_SIZE_X (500)
// #undef HC_RENDER_SIZE_STR_X
// #define HC_RENDER_SIZE_STR_X "500"

#include "engine/engine.c"
#include "engine/render.c"
#include "engine/video_render.c"
#include <stdio.h>

void noop() {}

hc_object *cube;
hc_quaternion tick;

void update() {
  hc_quaternion_multiply(&cube->rotation, &tick, &cube->rotation);
  hc_render_object(cube);
}

int main(void) {
  hc_init_render_video();
  hc_init_geometries();
  hc_set_fov(70);
  double hc_frame_tmp_vec[3] = {0, 0, 2};
  hc_quaternion_from_y_rotation(2 * 180 / M_PI, &tick);
  cube = hc_cube(hc_frame_tmp_vec);
  // printf("%p",cube[0]);
  // hc_init_render();
  // hc_init(hc_pre_render, hc_draw_buf_triangle, hc_render, noop);
  hc_init(hc_video_pre_frame, hc_video_triangle, hc_video_frame, update);
  hc_video_finish();
  return 0;
}
