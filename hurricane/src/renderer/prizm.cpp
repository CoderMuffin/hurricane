#include <fxcg/display.h>
#include <fxcg/keyboard.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <hurricane/renderer/renderer.h>
#include <hurricane/input.h>
#include <hurricane/shared.h>
#include <stdint.h>
#include "renderer_util.h"

static uint16_t *hc_prizm_buf;

static fixed *depth_buf = NULL;
static hc_renderer_config config;

static void init(hc_renderer_config renderer_config) {
  config = renderer_config;
  depth_buf = malloc(sizeof(fixed) * config.width * config.height);
  Bdisp_EnableColor(1);
  hc_prizm_buf = GetVRAMAddress();
}

static uint16_t prizm_color(unsigned char r, unsigned char g, unsigned char b) {
  return ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3);
}

static void pre_frame() {
  uint16_t clear = prizm_color(config.clear[0], config.clear[1], config.clear[2]);
  for (int y = 0; y < config.height; y++) {
    for (int x = 0; x < config.width; x++) {
      hc_prizm_buf[y * 384 + x] = clear;
      depth_buf[y * config.width + x] = INFINITY;
    }
  }
}

static void triangle(int x0, int y0, fixed z0, int x1, int y1, fixed z1,
                         int x2, int y2, fixed z2, unsigned char r,
                         unsigned char g, unsigned char b) {
  HC_INTERNAL_BUF_TRIANGLE(
      x0, y0, z0, x1, y1, z1, x2, y2, z2, config.width, config.height,
      HC_INTERNAL_DEPTH_BUF_CHECK(
          x0, y0, z0, x1, y1, z1, x2, y2, z2, config.width, depth_buf,
          hc_prizm_buf[y * 384 + x] = prizm_color(r, g, b);
        ))
}

static int key_pressed(int basic_keycode) {
  const unsigned short* keyboard_register = (unsigned short*)0xA44B0000;
  int row, col, word, bit;
  row = basic_keycode%10;
  col = basic_keycode/10-1;
  word = row>>1;
  bit = col + ((row&1)<<3);
  return (0 != (keyboard_register[word] & 1<<bit));
}

static int key;
static void frame() {
  if (key_pressed(KEY_PRGM_MENU)) {
    GetKey(&key);
  }
  Bdisp_PutDisp_DD();
}

static void finish() {
  free(depth_buf);
}

const hc_renderer hc_renderer_prizm = {
    .init = init,
    .pre_frame = pre_frame,
    .triangle = triangle,
    .frame = frame,
    .finish = finish,
    .internal_depth_buf = &depth_buf
};

