#ifndef HC_RENDER
#define HC_RENDER
#include "../shared.c"
#include <stdio.h>
#include <stdlib.h>
#include "../input.c"
#include <string.h>
#include <math.h>
#include "renderer.c"
#define CHAR_SIZE (sizeof(char) * 30)

unsigned char *hc_console_buf;
char *hc_console_char_buf;
char *map_buf;

double hc_console_depth_buf[HC_RENDER_SIZE_Y][HC_RENDER_SIZE_X];

void *hc_internal_console_input_handler(void *_) {
  int old=getchar();
  while (1) {
    int new_=getchar();
    hc_internal_keyup(old);
    hc_internal_keydown(new_);
    new_ = old;
  }
  return NULL;
}

void hc_console_init() {
  printf("\033[H\033[J");
  //pthread_t input_thread;
  //pthread_create(&input_thread, NULL, hc_internal_console_input_handler, NULL);
  hc_console_buf =
      calloc(HC_RENDER_SIZE_X * HC_RENDER_SIZE_Y * 3, sizeof(unsigned char));
  hc_console_char_buf =
      malloc((HC_RENDER_SIZE_X * 2 + 1) * HC_RENDER_SIZE_Y * CHAR_SIZE + 1);
  map_buf = malloc(CHAR_SIZE);
  printf("alloc'd %d\n", (HC_RENDER_SIZE_X * 2 + 1) * HC_RENDER_SIZE_Y);
}

void hc_console_pre_frame() {
  for (int i = 0; i < HC_RENDER_SIZE_X * HC_RENDER_SIZE_Y; i++) {
    hc_console_buf[i*3] = hc_render_bg[0];
    hc_console_buf[i*3+1] = hc_render_bg[1];
    hc_console_buf[i*3+2] = hc_render_bg[2];
  }
  for (int y = 0; y < HC_RENDER_SIZE_Y; y++)
    for (int x = 0; x < HC_RENDER_SIZE_X; x++)
      hc_console_depth_buf[y][x] = INFINITY;
}

// void hc_console_buf_line(int x0, int y0, int x1, int y1) {
//   int dx = abs(x1 - x0);
//   //int HC_RENDER_SIZE_X = x0 < x1 ? 1 : -1;
//   int dy = -abs(y1 - y0);
//   //int HC_RENDER_SIZE_Y = y0 < y1 ? 1 : -1;
//   int error = dx + dy;

//   while (1) {
//     hc_console_buf[(x0 + y0 * HC_RENDER_SIZE_X) * 3] = 255;
//     if (x0 == x1 && y0 == y1)
//       break;
//     int e2 = 2 * error;
//     if (e2 >= dy) {
//       if (x0 == x1)
//         break;
//       error += dy;
//       x0 += HC_RENDER_SIZE_X;
//     }
//     if (e2 <= dx) {
//       if (y0 == y1)
//         break;
//       error += dx;
//       y0 += HC_RENDER_SIZE_Y;
//     }
//   }
// }

void hc_console_triangle(int x0, int y0, double z0, int x1, int y1, double z1,
                         int x2, int y2, double z2, unsigned char r,
                         unsigned char g, unsigned char b) {
  // sort the points vertically
  if (y1 > y2) {
    swap_int(&x1, &x2);
    swap_int(&y1, &y2);
    swap_dbl(&z1, &z2);
  }
  if (y0 > y1) {
    swap_int(&x0, &x1);
    swap_int(&y0, &y1);
    swap_dbl(&z0, &z1);
  }
  if (y1 > y2) {
    swap_int(&x1, &x2);
    swap_int(&y1, &y2);
    swap_dbl(&z1, &z2);
  }
  double dx_far = ((double)(x2 - x0)) / (y2 - y0 + 1);
  double dx_upper = ((double)(x1 - x0)) / (y1 - y0 + 1);
  double dx_low = ((double)(x2 - x1)) / (y2 - y1 + 1);
  double xf = x0;
  double xt = x0 + dx_upper; // if y0 == y1, special case
  for (int y = y0; y <= (y2 > HC_RENDER_SIZE_Y - 1 ? HC_RENDER_SIZE_Y - 1 : y2);
       y++) {
    if (y >= 0) {
      for (int x = (xf > 0 ? (int)(xf) : 0);
           x <= (xt < HC_RENDER_SIZE_X ? xt : HC_RENDER_SIZE_X - 1); x++) {
        // int pos = (int)(x + y * HC_RENDER_SIZE_X);
        double depth =
            calculate_depth(x0, y0, z0, x1, y1, z1, x2, y2, z2, x, y);
        if (isnan(depth) || !isfinite(depth) || depth <= 0.1) {
          continue;
        }
        if (depth < hc_console_depth_buf[y][x]) {
          hc_console_depth_buf[y][x] = depth;
          hc_console_buf[(y * HC_RENDER_SIZE_X + x) * 3 + 0] = r;
          hc_console_buf[(y * HC_RENDER_SIZE_X + x) * 3 + 1] = g;
          hc_console_buf[(y * HC_RENDER_SIZE_X + x) * 3 + 2] = b;
        }
      }
      for (int x = (xf < HC_RENDER_SIZE_X ? (int)(xf) : HC_RENDER_SIZE_X - 1);
           x >= (xt > 0 ? xt : 0); x--) {
        // int pos = (int)(x + y * HC_RENDER_SIZE_X);
        double depth =
            calculate_depth(x0, y0, z0, x1, y1, z1, x2, y2, z2, x, y);
        if (isnan(depth) || !isfinite(depth) || depth <= 0.1) {
          continue;
        }
        if (depth < hc_console_depth_buf[y][x]) {
          hc_console_depth_buf[y][x] = depth;
          hc_console_buf[(y * HC_RENDER_SIZE_X + x) * 3 + 0] = r;
          hc_console_buf[(y * HC_RENDER_SIZE_X + x) * 3 + 1] = g;
          hc_console_buf[(y * HC_RENDER_SIZE_X + x) * 3 + 2] = b;
        }
      }
    }
    xf += dx_far;
    if (y < y1)
      xt += dx_upper;
    else
      xt += dx_low;
  }
}

char *map_char(unsigned char *vs) {
  sprintf(map_buf, "\033[38;2;%d;%d;%dm@", vs[0], vs[1], vs[2]);
  return map_buf;
}

void hc_console_frame() {
  int advance = 0;
  for (int y = 0; y < HC_RENDER_SIZE_Y; y++) {
    for (int x = 0; x < HC_RENDER_SIZE_X; x++) {
      //+1 for nl
      // printf("xs %d\n", (x + y * HC_RENDER_SIZE_X) * 2 + y);
      // printf("xs %d\n", (x + y * HC_RENDER_SIZE_X) * 2 + y + 1);
      // its over anakin
      // i have the high ground
      // dont try to refactor this code
      // for your own sake
      // please
      char *to_write = map_char(hc_console_buf + ((x + y * HC_RENDER_SIZE_X)) * 3);
      strncpy(hc_console_char_buf + advance, to_write, strlen(to_write));
      advance += strlen(to_write);
      strncpy(hc_console_char_buf + advance, to_write, strlen(to_write));
      advance += strlen(to_write);
    }
    // printf("newline %d\n", ((y+1) * HC_RENDER_SIZE_X) * 2 + y);
    hc_console_char_buf[advance] = '\n';
    advance++;
  }
  hc_console_char_buf[advance] = '\0';
  printf("\033[0;0H%s", hc_console_char_buf);
}

void hc_console_finish() {}

const hc_renderer hc_renderer_console = {
    .init = hc_console_init,
    .pre_frame = hc_console_pre_frame,
    .triangle = hc_console_triangle,
    .frame = hc_console_frame,
    .finish = hc_console_finish,
    .internal_depth_buf = hc_console_depth_buf
};

#endif
