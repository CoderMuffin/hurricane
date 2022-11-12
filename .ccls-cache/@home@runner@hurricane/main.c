int logq = 0;

#include "hurricane/engine.c"
#include "hurricane/shared.c"
//#include "hurricane/renderer/console.c"
//#include "hurricane/renderer/video.c"
#include "hurricane/renderer/xlib.c"
#include "hurricane/util/log.c"
#include "hurricane/util/anim.c"

#include <stdio.h>
#include <unistd.h>

void noop() {}

bool w_down = false, s_down = false, a_down = false, d_down = false;

hc_object cube;
hc_object camera;
hc_object cube2;
hc_anim cube_anim;
hc_quaternion tick;
hc_quaternion camera_small_left;
hc_quaternion camera_small_right;
double tmpvec[3] = {0.02, 0.02, 0};
double tmpvecupdate[3];

void update() {
  //hc_quaternion_mul(&cube.rotation, &tick, &cube.rotation);
  hc_quaternion_mul(&cube2.rotation, &tick, &cube2.rotation);
  hc_quaternion_mul(&cube2.rotation, &tick, &cube2.rotation);
  if (w_down) {
    hc_quaternion_rotate(&camera.rotation, (double[]){0, 0, 0.1}, tmpvecupdate);
    hc_vec3_add(camera.position, tmpvecupdate, camera.position);
  }
  if (s_down) {
    hc_quaternion_rotate(&camera.rotation, (double[]){0, 0, -0.1},
                         tmpvecupdate);
    hc_vec3_add(camera.position, tmpvecupdate, camera.position);
  }
  if (a_down)
    hc_quaternion_mul(&camera.rotation, &camera_small_left, &camera.rotation);
  if (d_down)
    hc_quaternion_mul(&camera.rotation, &camera_small_right, &camera.rotation);
  // hc_vec3_add(cube->position, tmpvec, cube->position);
  hc_render_object(&camera, &cube);
  hc_render_object(&camera, &cube2);
  hc_anim_step(&cube_anim, 0.01, &cube.rotation);
  // printf("anim: time:%f playing:%d looping:%d\n", cube_anim.time, cube_anim.playing, cube_anim.looping);
  // printf("%f %f %f\n", cube.position[0], cube.position[1], cube.position[2]);
  if (logq) {
    logq = false;
  }
}

void on_key_up(void *e) {
  hc_input_key_event *evt = e;
  // printf("%d\n", evt->code);
  if (evt->code == 25) {
    w_down = false;
  } else if (evt->code == 38) {
    a_down = false;
  } else if (evt->code == 39) {
    s_down = false;
  } else if (evt->code == 40) {
    d_down = false;
  }
}

void on_key_down(void *e) {
  hc_input_key_event *evt = e;
  // printf("%d\n", evt->code);
  if (evt->code == 25) {
    w_down = true;
  } else if (evt->code == 38) {
    a_down = true;
  } else if (evt->code == 39) {
    s_down = true;
  } else if (evt->code == 40) {
    d_down = true;
  } else if (evt->code == 41) {
    logq = 1;
  } else {
    printf("%d\n", evt->code);
  }
}

int main(void) {
  hc_xlib_init();
  // hc_video_init();
  // hc_console_init();
  // hc_init_geometries();
  hc_input_subscribe(on_key_down, HC_INPUT_KEYDOWN);
  hc_input_subscribe(on_key_up, HC_INPUT_KEYUP);
  //hc_anim_new(&cube_anim, (hc_keyframe[]){{VEC3(-2,0,5),0}, {VEC3(-2,1,5),1}, {VEC3(-2,0,5),2}}, 3, hc_animator_vec3);
  
  hc_anim_new(&cube_anim, (hc_keyframe[]){
    {QUAT(0,0,0),0},
    {QUAT(0,M_PI,0),1}
  }, 2, hc_animator_quaternion);
  cube_anim.looping = true;
  
  hc_set_fov(70, false);
  double tmp_vec[3] = {0, 0, 3};
  hc_quaternion_from_euler_zyx(
      (double[3]){0 / 180 * M_PI, 2.0 / 180 * M_PI, 0 / 180 * M_PI}, &tick);
  hc_quaternion_from_y_rotation(-2.0 * DEG2RAD, &camera_small_left);
  hc_quaternion_from_y_rotation(2.0 * DEG2RAD, &camera_small_right);
  hc_new_object(&cube, &hc_geometry_sphere5, tmp_vec, hc_quaternion_identity,
                hc_vec3_one);

  hc_new_object(&camera, &hc_geometry_none, hc_vec3_zero,
                hc_quaternion_identity, hc_vec3_one);

  double tmp_vec2[3] = {1.2, 0, 3};

  hc_new_object(&cube2, &hc_geometry_cube, tmp_vec2, hc_quaternion_identity,
                (double[]){1, 0.2, 0.2});
  // hc_init(false, 200, hc_video_pre_frame, hc_video_triangle, hc_video_frame,
  //        update);
  // hc_init(false, -1, hc_console_pre_frame, hc_console_triangle,
  // hc_console_frame, update);
  // int tmp[2];
  // hc_world_to_screen(&camera, (double[]){4, 0, -1}, tmp);
  // printf("%d %d\n", tmp[0], tmp[1]);
  // exit(1);
  hc_init(false, -1, hc_xlib_pre_frame, hc_xlib_triangle, hc_xlib_frame,
          update);

  hc_xlib_finish();
  // hc_video_finish();
  return 0;
}
