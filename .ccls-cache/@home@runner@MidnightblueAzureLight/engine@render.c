#ifndef HC_RENDER
#define HC_RENDER
#include "util/vec.c"
#include <stdlib.h>

int *hc_screen_size;
char *hc_render_buf;

void hc_init_render(int sx, int sy) {
  hc_screen_size = malloc(sizeof(int) * 2);
  hc_screen_size[0] = sx;
  hc_screen_size[1] = sy;
  hc_render_buf = malloc(sx * sy);
}

void hc_render(int *buf) {
  for (int y = 0; y < hc_screen_size[1]; y++) {
    for (int x = 0; x < hc_screen_size[0]; x++) {
      out[x * y] = "@";
    }
  }
}

#endif
