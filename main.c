int logq = 0;

#include "hurricane/include/engine.h"
#include "hurricane/include/shared.h"
#include "hurricane/include/util/list.h"
#include "hurricane/include/renderer/console.h"
#include "hurricane/include/renderer/video.h"
// #include "hurricane/include/renderer/xlib.h"
#include "hurricane/include/loader/obj.h"
#include "hurricane/include/renderer/SDL.h"
#include "hurricane/include/input.h"

#include "hurricane/include/anim.h"
#include "hurricane/include/util/log.h"
#include "hurricane/include/clock.h"
#include <stdbool.h>
#include <stdio.h>

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
double x_rot, y_rot;
void noop() {}

bool w_down = false, s_down = false, a_down = false, d_down = false;

hc_object cube;
hc_object camera;
hc_object cube2;
hc_clock gclock;
hc_anim cube_anim;
hc_quaternion tick;
hc_quaternion camera_small_left;
hc_quaternion camera_small_right;
hc_renderer renderer;

double tmpvec[3] = {0.02, 0.02, 0};
double tmpvecupdate[3];

void update() {
  // hc_quaternion_mul(&cube.rotation, &tick, &cube.rotation);
  //hc_quaternion_mul(&cube2.rotation, &tick, &cube2.rotation);
  //hc_quaternion_mul(&cube2.rotation, &tick, &cube2.rotation);
  // if (w_down) {
  //   hc_quaternion_rotate(&camera.rotation, (double[]){0, 0, 0.1}, tmpvecupdate);
  //   hc_vec3_add(camera.position, tmpvecupdate, camera.position);
  // }
  // if (s_down) {
  //   hc_quaternion_rotate(&camera.rotation, (double[]){0, 0, -0.1},
  //                        tmpvecupdate);
  //   hc_vec3_add(camera.position, tmpvecupdate, camera.position);
  // }
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
  double delta = hc_clock_step(&gclock);
  hc_render_object(&camera, &cube);

  hc_render_object(&camera, &cube2);
  hc_log("delta %f", delta);
  hc_anim_step(&cube_anim, delta, &cube.rotation);
  // printf("anim: time:%f playing:%d looping:%d\n", cube_anim.time,
  // cube_anim.playing, cube_anim.looping); printf("%f %f %f\n",
  // cube.position[0], cube.position[1], cube.position[2]);
  if (logq) {
    logq = false;
  }
}

void on_key_up(void *e) {
  hc_input_key_event *evt = e;
  // printf("%d\n", evt->code);
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
  // printf("%d\n", evt->code);
  if (evt->code == KEYW) {
    w_down = true;
  } else if (evt->code == KEYA) {
    a_down = true;
  } else if (evt->code == KEYS) {
    s_down = true;
  } else if (evt->code == KEYD) {
    d_down = true;
  } else if (evt->code == 41) {
    logq = 1;
  } else {
    hc_error("unknown key %d", evt->code);
    exit(1);
  }
}

int main(int argc, char **argv) {
  // hc_xlib_init();
  renderer = hc_renderer_sdl;
  renderer.init();
  hc_clock_new(&gclock);
  //hc_list list;
  //hc_list_new(&list);
  //hc_list_add(&list, VEC3(0,1,4));
  //hc_list_add(&list, VEC3(1,4,4));
  //hc_list_add(&list, VEC3(2,2,4));
  //hc_list_add(&list, VEC3(3,1,7));
  //hc_list_add(&list, VEC3(4,1,7));
  //hc_list_remove(&list, 2);
  //double *a = hc_list_get(&list, 2);
  //hc_log("%f %f %f %d %d", a[0], a[1], a[2], list.length, list.allocated);
  //exit(1);
  // hc_video_init();
  // hc_console_init();
  // hc_init_geometries();
  hc_input_subscribe(on_key_down, HC_INPUT_KEYDOWN);
  hc_input_subscribe(on_key_up, HC_INPUT_KEYUP);
  // hc_anim_new(&cube_anim, (hc_keyframe[]){{VEC3(-2,0,5),0}, {VEC3(-2,1,5),1},
  // {VEC3(-2,0,5),2}}, 3, hc_animator_vec3);

  hc_anim_new(&cube_anim,
              (hc_keyframe[]){{QUAT(M_PI, 0, 0), 0},
                              {QUAT(M_PI, M_PI, 0), 3},
                              {QUAT(M_PI, M_PI * 2, 0), 6}},
              3, hc_animator_quaternion);
  cube_anim.looping = true;

  hc_set_fov(70, false);
  hc_quaternion_from_euler_zyx(
      (double[3]){0 / 180 * M_PI, 2.0 / 180 * M_PI, 0 / 180 * M_PI}, &tick);
  hc_quaternion_from_y_rotation(-2.0 * DEG2RAD, &camera_small_left);
  hc_quaternion_from_y_rotation(2.0 * DEG2RAD, &camera_small_right);
  hc_geometry geometry_teapot;
  hc_geometry_from_obj("teapot.obj", &geometry_teapot);
  hc_object_new(&cube, &geometry_teapot, VEC3(0, 0.8, 0), hc_quaternion_identity,
                (double[]){0.5, 0.5, 0.5});
  hc_log("%d faces", geometry_teapot.face_count);
  hc_object_new(&camera, &hc_geometry_none, VEC3(0, 0, -3),
                hc_quaternion_identity, hc_vec3_one);


  hc_object_new(&cube2, &hc_geometry_cube, VEC3(1.2, 0, 0), hc_quaternion_identity,
                (double[]){1, 0.2, 0.2});
  // hc_init(false, 200, hc_video_pre_frame, hc_video_triangle, hc_video_frame,
  //        update);
  // hc_init(false, -1, hc_console_pre_frame, hc_console_triangle,
  // hc_console_frame, update);
  // int tmp[2];
  // hc_world_to_screen(&camera, (double[]){4, 0, -1}, tmp);
  // printf("%d %d\n", tmp[0], tmp[1]);
  // exit(1);
  hc_init(true, -1, renderer, update);

  renderer.finish();
  // hc_sdl_finish();
  // hc_video_finish();
  return 0;
}
//*/

// #include <io.h>
// #include <fcntl.h>
// #include <windows.h>
// #include <time.h>

// LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
//     LPSTR lpCmdLine, int nCmdShow) {
//     AllocConsole();
//     FILE* fp = NULL;
//     freopen_s(&fp, "CONIN$", "r", stdin);
//     freopen_s(&fp, "CONOUT$", "w", stdout);
//     freopen_s(&fp, "CONOUT$", "w", stderr);
//     printf("aloha\n");

//     MSG  msg;
//     WNDCLASSW wc = {0};

//     wc.style = CS_HREDRAW | CS_VREDRAW;
//     wc.lpszClassName = L"Draw Bitmap";
//     wc.hInstance     = hInstance;
//     wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
//     wc.lpfnWndProc   = WndProc;
//     wc.hCursor       = LoadCursor(0, IDC_ARROW);

//     RegisterClassW(&wc);
//     CreateWindowW(wc.lpszClassName, L"Draw Bitmap",
//           WS_OVERLAPPEDWINDOW | WS_VISIBLE,
//           100, 100, 280, 220, NULL, NULL, hInstance, NULL);

//     while (GetMessage(&msg, NULL, 0, 0)) {

//         TranslateMessage(&msg);
//         DispatchMessage(&msg);
//     }

//     return (int) msg.wParam;
// }

// LRESULT CALLBACK WndProc(HWND hwnd, UINT msg,
//     WPARAM wParam, LPARAM lParam) {

//     static HBITMAP hBitmap;
//     HDC hdc;
//     PAINTSTRUCT ps;
//     BITMAP bitmap;
//     HDC hdcMem;
//     HGDIOBJ oldBitmap;

//     switch(msg) {

//         case WM_CREATE:

//              hBitmap = (HBITMAP) LoadImageW(NULL, L"blackbuck.bmp",
//                         IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

//              if (hBitmap == NULL) {
//                  MessageBoxW(hwnd, L"Failed to load image", L"Error", MB_OK);
//              }

//              break;

//         case WM_PAINT:

//              hdc = BeginPaint(hwnd, &ps);

//              hdcMem = CreateCompatibleDC(hdc);
//              oldBitmap = SelectObject(hdcMem, hBitmap);

//              GetObject(hBitmap, sizeof(bitmap), &bitmap);
//              BitBlt(hdc, 5, 5, bitmap.bmWidth, bitmap.bmHeight,
//                  hdcMem, 0, 0, SRCCOPY);
//               printf("aloha\n");
//              SelectObject(hdcMem, oldBitmap);
//              DeleteDC(hdcMem);

//              EndPaint(hwnd, &ps);

//              break;

//         case WM_DESTROY:

//             DeleteObject(hBitmap);
//             PostQuitMessage(0);

//             return 0;
//     }

//     return DefWindowProcW(hwnd, msg, wParam, lParam);
// }
