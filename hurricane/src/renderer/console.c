#include "renderer.h"
#include <hurricane/input.h>
#include <hurricane/renderer/renderer.h>
#include <hurricane/shared.h>
#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHAR_SIZE (sizeof(char) * 30)

unsigned char *hc_console_buf;
char *hc_console_char_buf;
char *hc_console_text_buf;
char *map_buf;

double hc_console_depth_buf[HC_RENDER_SIZE_Y][HC_RENDER_SIZE_X];

void *hc_internal_console_input_handler(void *_) {
  int old = getchar();
  while (1) {
    int new_ = getchar();
    hc_internal_keyup(old);
    hc_internal_keydown(new_);
    new_ = old;
  }
  return NULL;
}

void hc_console_init() {
  printf("\033[H\033[J");
  pthread_t input_thread;
  pthread_create(&input_thread, NULL, hc_internal_console_input_handler, NULL);
  hc_console_buf =
      calloc(HC_RENDER_SIZE_X * HC_RENDER_SIZE_Y * 3, sizeof(unsigned char));
  hc_console_char_buf =
      malloc((HC_RENDER_SIZE_X * 2 + 1) * HC_RENDER_SIZE_Y * CHAR_SIZE + 1);
  hc_console_text_buf =
      malloc(HC_RENDER_SIZE_X * HC_RENDER_SIZE_Y * sizeof(char) + 1);
  map_buf = malloc(CHAR_SIZE);
}

void hc_console_pre_frame() {
  for (int i = 0; i < HC_RENDER_SIZE_X * HC_RENDER_SIZE_Y; i++) {
    hc_console_buf[i * 3] = hc_render_bg[0];
    hc_console_buf[i * 3 + 1] = hc_render_bg[1];
    hc_console_buf[i * 3 + 2] = hc_render_bg[2];
    hc_console_text_buf[i] = 0;
  }
  for (int y = 0; y < HC_RENDER_SIZE_Y; y++) {
    for (int x = 0; x < HC_RENDER_SIZE_X; x++) {
      hc_console_depth_buf[y][x] = INFINITY;
    }
  }
}

void hc_console_triangle(int x0, int y0, double z0, int x1, int y1, double z1,
                         int x2, int y2, double z2, unsigned char r,
                         unsigned char g, unsigned char b) {
  HC_INTERNAL_BUF_TRIANGLE(
      x0, y0, z0, x1, y1, z1, x2, y2, z2, r, g, b,
      HC_INTERNAL_DEPTH_BUF_CHECK(
          x0, y0, z0, x1, y1, z1, x2, y2, z2, r, g, b, hc_console_depth_buf,
          hc_console_buf[(y * HC_RENDER_SIZE_X + x) * 3 + 0] = r;
          hc_console_buf[(y * HC_RENDER_SIZE_X + x) * 3 + 1] = g;
          hc_console_buf[(y * HC_RENDER_SIZE_X + x) * 3 + 2] = b;))
}

char *map_char(unsigned char *vs) {
  sprintf(map_buf, "\033[38;2;%d;%d;%dm█", vs[0], vs[1], vs[2]);
  return map_buf;
}

void hc_console_blit(char *text, int x, int y) {
  strncpy(hc_console_text_buf + y * HC_RENDER_SIZE_X + x, text, strlen(text));
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
      if (hc_console_text_buf[y * HC_RENDER_SIZE_X + x]) {
        strncpy(hc_console_char_buf + advance,
                hc_console_text_buf + y * HC_RENDER_SIZE_X + x, sizeof(char));
        advance += sizeof(char);
      } else {
        char *to_write =
            map_char(hc_console_buf + ((x + y * HC_RENDER_SIZE_X)) * 3);
        strncpy(hc_console_char_buf + advance, to_write, strlen(to_write));
        advance += strlen(to_write);
        strncpy(hc_console_char_buf + advance, to_write, strlen(to_write));
        advance += strlen(to_write);
      }
    }
    // printf("newline %d\n", ((y+1) * HC_RENDER_SIZE_X) * 2 + y);
    hc_console_char_buf[advance] = '\n';
    advance++;
  }
  hc_console_char_buf[advance] = '\0';
  printf("\033[H%s", hc_console_char_buf);
}

void hc_console_finish() {}

const hc_renderer hc_renderer_console = {.init = hc_console_init,
                                         .pre_frame = hc_console_pre_frame,
                                         .triangle = hc_console_triangle,
                                         .frame = hc_console_frame,
                                         .finish = hc_console_finish,
                                         .internal_depth_buf =
                                             &hc_console_depth_buf};
