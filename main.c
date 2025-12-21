#include <hurricane/geometry.h>
#include <hurricane/engine.h>
#include <hurricane/shared.h>
#include <hurricane/util/vec.h>
#include <hurricane/util/list.h>
#include <hurricane/loader/obj.h>
#include <hurricane/renderer/SDL.h>
#include <hurricane/input.h>

#include <hurricane/anim.h>
#include <hurricane/util/log.h>
#include <hurricane/clock.h>
#include <stdbool.h>
#include <stdlib.h>

#if 1
  #define KEYW 119
  #define KEYA 97
  #define KEYS 115
  #define KEYD 100
#else
  #define KEYW 25
  #define KEYA 38
  #define KEYS 39
  #define KEYD 40
#endif
fixed x_rot, y_rot;
void noop(void) {}

bool w_down = false, s_down = false, a_down = false, d_down = false;

hc_object cube;
hc_object camera;
hc_object cube2;
hc_clock gclock;
hc_anim cube_anim;

fixed tmpvec[3] = {tf(0.02), tf(0.02), tf(0.0)};
fixed tmpvecupdate[3];

// fixed curr_second;
// int frames = 0;
// void fps(fixed delta) {
//   curr_second += delta;
//   frames++;
//   if (curr_second >= 1) {
//     curr_second -= 1;
//     printf("\r%d", frames);
//     fflush(stdout);
//     frames = 0;
//   }
// }

void update() {
  if (w_down) {
    x_rot -= 3 * DEG2RAD;
  }
  if (s_down) {
    x_rot += 3 * DEG2RAD;
  }
  if (a_down) {
    y_rot += 3 * DEG2RAD;
  }
  if (d_down) {
    y_rot -= 3 * DEG2RAD;
  }
  if (w_down || s_down || a_down || d_down) {
    hc_quaternion_from_euler_zyx(VEC3(x_rot, y_rot, 0), &camera.rotation);
    hc_quaternion_rotate(&camera.rotation, VEC3(0,0,-3), camera.position);
  }
  // hc_vec3_add(cube->position, tmpvec, cube->position);
  hc_render_object(&camera, &cube);

  hc_render_object(&camera, &cube2);
  fixed delta = hc_clock_step(&gclock);
  hc_anim_step(&cube_anim, delta, &cube.rotation);
  // printf("anim: time:%f playing:%d looping:%d\n", cube_anim.time,
  // cube_anim.playing, cube_anim.looping); printf("%f %f %f\n",
  // cube.position[0], cube.position[1], cube.position[2]);
  //fps(delta);
}

void on_key_up(void *e) {
  hc_input_key_event *evt = e;
  if (evt->code == KEYW) {
    w_down = false;
  } else if (evt->code == KEYA) {
    a_down = false;
  } else if (evt->code == KEYS) {
    s_down = false;
  } else if (evt->code == KEYD) {
    d_down = false;
  }
}

void on_key_down(void *e) {
  hc_input_key_event *evt = e;
  if (evt->code == KEYW) {
    w_down = true;
  } else if (evt->code == KEYA) {
    a_down = true;
  } else if (evt->code == KEYS) {
    s_down = true;
  } else if (evt->code == KEYD) {
    d_down = true;
  } else {
    hc_warn("unknown key %d", evt->code);
  }
}

int main(void) {
  hc_clock_new(&gclock);
  hc_renderer_config rc = (hc_renderer_config) {
    .clear = {0, 0, 0},
    .width = 300,
    .height = 150
  };
  hc_input_subscribe(on_key_down, HC_INPUT_KEYDOWN);
  hc_input_subscribe(on_key_up, HC_INPUT_KEYUP);

  hc_anim_new(&cube_anim,
              (hc_keyframe[]){{QUAT(M_PI, 0, 0), 0},
                              {QUAT(M_PI, M_PI, 0), 3},
                              {QUAT(M_PI, M_PI * 2, 0), 6}},
              3, hc_animator_quaternion);
  cube_anim.looping = true;

  hc_set_fov(tf(70), rc, false);
  hc_new_object(&cube, &hc_geometry_cube, VEC3(0, 0.8, 0), hc_quaternion_identity,
                VEC3(0.5, 0.5, 0.5));
  hc_new_object(&camera, &hc_geometry_none, VEC3(0, 0, -3),
                hc_quaternion_identity, VEC3(1, 1, 1));


  hc_new_object(&cube2, &hc_geometry_cube, VEC3(1.2, 0, 0), hc_quaternion_identity,
                VEC3(1, 0.2, 0.2));
  hc_init(false, -1, hc_renderer_sdl, rc, update);
  return 0;
}
