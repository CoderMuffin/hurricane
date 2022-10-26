#ifndef HC_RENDER
#define HC_RENDER
#include "util/vec.c"
#include <stdlib.h>
#define CHAR_SIZE (sizeof(char)*1)

int *hc_screen_size;
char *hc_render_buf;

void hc_init_render(int sx, int sy) {
  hc_screen_size = malloc(sizeof(int) * 2);
  hc_screen_size[0] = sx;
  hc_screen_size[1] = sy;
  hc_render_buf = malloc((sx*2 + 1) * sy * CHAR_SIZE);
  printf("alloc'd %d\n", (sx*2 + 1) * sy);
}

char map_char(int v) {
  return 'A';
}

void hc_render(int *buf) {
  for (int y = 0; y < hc_screen_size[1]; y++) {
    for (int x = 0; x < hc_screen_size[0]; x++) {
      //+1 for nl
      //printf("xs %d\n", (x + y * hc_screen_size[0]) * 2 + y);
      //printf("xs %d\n", (x + y * hc_screen_size[0]) * 2 + y + 1);
      //its over anakin
      //i have the high ground
      //dont try to refactor this code
      //for your own sake
      //please
      hc_render_buf[(x + y * hc_screen_size[0]) * 2 + y] = map_char(buf[(x + y * hc_screen_size[0]) * 2 + y]);
      hc_render_buf[(x + y * hc_screen_size[0]) * 2 + y + 1] = map_char(buf[(x + y * hc_screen_size[0]) * 2 + y]);
    }
    //printf("newline %d\n", ((y+1) * hc_screen_size[0]) * 2 + y);
    hc_render_buf[((y+1) * hc_screen_size[0]) * 2 + y] = '\n';
  }
  printf("\033[0;0H%s",hc_render_buf);
}

#endif
