#include <fxcg/display.h>
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

static double *depth_buf = {0};
static hc_renderer_config config;

static void init(hc_renderer_config renderer_config) {
  depth_buf = malloc(sizeof(double) * config.width * config.height);
  Bdisp_EnableColor(1);
  hc_prizm_buf = GetVRAMAddress();
}

static uint16_t prizm_color(unsigned char r, unsigned char g, unsigned char b) {
  return ((r/(2<<3)) << 11) + ((g/(2<<2)) << 5) + ((b/(2<<3)));
}

static void pre_frame() {
  uint16_t clear = prizm_color(config.clear[0], config.clear[1], config.clear[2]);
  for (int i = 0; i < config.width * config.height; i++) {
    hc_prizm_buf[i] = clear;
    depth_buf[i] = INFINITY;
  }
}

static void triangle(int x0, int y0, double z0, int x1, int y1, double z1,
                         int x2, int y2, double z2, unsigned char r,
                         unsigned char g, unsigned char b) {
  HC_INTERNAL_BUF_TRIANGLE(
      x0, y0, z0, x1, y1, z1, x2, y2, z2, r, g, b, config.width, config.height,
      HC_INTERNAL_DEPTH_BUF_CHECK(
          x0, y0, z0, x1, y1, z1, x2, y2, z2, r, g, b, config.width, depth_buf,
          hc_prizm_buf[y * config.width + x] = prizm_color(r, g, b);
        ))
}

static int key;
static void frame() {
  GetKey(&key);
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

