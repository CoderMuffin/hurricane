#include "renderer_util.h"
#include <hurricane/input.h>
#include <hurricane/renderer/renderer.h>
#include <hurricane/shared.h>
#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHAR_SIZE (sizeof(char) * 30)

unsigned char *buf;
char *char_buf;
char *text_buf;
char *map_buf;
hc_renderer_config config;

double *depth_buf;

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

static void init(hc_renderer_config renderer_config) {
  config = renderer_config;
  printf("\033[H\033[J");
  pthread_t input_thread;
  pthread_create(&input_thread, NULL, hc_internal_console_input_handler, NULL);
  buf = calloc(config.width * config.height * 3, sizeof(unsigned char));
  char_buf =
      malloc((config.width * 2 + 1) * config.height * CHAR_SIZE + 1);
  text_buf = malloc(config.width * config.height * sizeof(char) + 1);
  map_buf = malloc(CHAR_SIZE);
  depth_buf = malloc(config.width * config.height * sizeof(double));
}

static void pre_frame(void) {
  for (int i = 0; i < config.width * config.height; i++) {
    buf[i * 3] = config.clear[0];
    buf[i * 3 + 1] = config.clear[1];
    buf[i * 3 + 2] = config.clear[2];
    text_buf[i] = 0;
    depth_buf[i] = INFINITY;
  }
}

static void triangle(int x0, int y0, double z0, int x1, int y1, double z1,
                     int x2, int y2, double z2, unsigned char r,
                     unsigned char g, unsigned char b) {
  HC_INTERNAL_BUF_TRIANGLE(
      x0, y0, z0, x1, y1, z1, x2, y2, z2, config.width, config.height,
      HC_INTERNAL_DEPTH_BUF_CHECK(x0, y0, z0, x1, y1, z1, x2, y2, z2, config.width,
                                  depth_buf,
                                  buf[(y * config.width + x) * 3 + 0] = r;
                                  buf[(y * config.width + x) * 3 + 1] = g;
                                  buf[(y * config.width + x) * 3 + 2] = b;))
}

static char *map_char(unsigned char *vs) {
  sprintf(map_buf, "\033[38;2;%d;%d;%dm█", vs[0], vs[1], vs[2]);
  return map_buf;
}

void blit(char *text, int x, int y) {
  strncpy(text_buf + y * config.width + x, text, strlen(text));
}

static void frame(void) {
  int advance = 0;
  for (int y = 0; y < config.height; y++) {
    for (int x = 0; x < config.width; x++) {
      //+1 for nl
      // printf("xs %d\n", (x + y * config.width) * 2 + y);
      // printf("xs %d\n", (x + y * config.width) * 2 + y + 1);
      // its over anakin
      // i have the high ground
      if (text_buf[y * config.width + x]) {
        strncpy(char_buf + advance, text_buf + y * config.width + x,
                sizeof(char));
        advance += sizeof(char);
      } else {
        char *to_write = map_char(buf + ((x + y * config.width)) * 3);
        strncpy(char_buf + advance, to_write, strlen(to_write));
        advance += strlen(to_write);
        strncpy(char_buf + advance, to_write, strlen(to_write));
        advance += strlen(to_write);
      }
    }
    // printf("newline %d\n", ((y+1) * config.width) * 2 + y);
    char_buf[advance] = '\n';
    advance++;
  }
  char_buf[advance] = '\0';
  printf("\033[H%s", char_buf);
}

static void finish(void) {}

const hc_renderer hc_renderer_console = {.init = init,
                                         .pre_frame = pre_frame,
                                         .triangle = triangle,
                                         .frame = frame,
                                         .finish = finish,
                                         .internal_depth_buf = &depth_buf};
