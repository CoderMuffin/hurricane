#ifndef HC_RENDER
#define HC_RENDER
#include "shared.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CHAR_SIZE (sizeof(char) * 50)

unsigned char *hc_draw_buf;
char *hc_render_buf;
char *map_buf;

void swap_int(int *a, int *b) {
  int temp = *b;
  *b = *a;
  *a = temp;
}

void hc_init_render() {
  printf("\033[H\033[J");
  hc_draw_buf = calloc(HC_RENDER_SIZE_X * HC_RENDER_SIZE_Y * 3, sizeof(unsigned char));
  hc_render_buf = malloc((HC_RENDER_SIZE_X * 2 + 1) * HC_RENDER_SIZE_Y * CHAR_SIZE + 1);
  map_buf = malloc(CHAR_SIZE);
  printf("alloc'd %d\n", (HC_RENDER_SIZE_X * 2 + 1) * HC_RENDER_SIZE_Y);
}

void hc_pre_render() {
  for (int i = 0; i < 3 * HC_RENDER_SIZE_X * HC_RENDER_SIZE_Y; i++) {
    hc_draw_buf[i] = 0;
  }
}

void hc_draw_buf_line(int x0, int y0, int x1, int y1) {
  int dx = abs(x1 - x0);
  //int HC_RENDER_SIZE_X = x0 < x1 ? 1 : -1;
  int dy = -abs(y1 - y0);
  //int HC_RENDER_SIZE_Y = y0 < y1 ? 1 : -1;
  int error = dx + dy;

  while (1) {
    hc_draw_buf[(x0 + y0 * HC_RENDER_SIZE_X) * 3] = 255;
    if (x0 == x1 && y0 == y1)
      break;
    int e2 = 2 * error;
    if (e2 >= dy) {
      if (x0 == x1)
        break;
      error += dy;
      x0 += HC_RENDER_SIZE_X;
    }
    if (e2 <= dx) {
      if (y0 == y1)
        break;
      error += dx;
      y0 += HC_RENDER_SIZE_Y;
    }
  }
}

void hc_draw_buf_triangle(int x0, int y0, int x1, int y1, int x2, int y2,
                          unsigned char r, unsigned char g, unsigned char b) {
  // sort the points vertically
  if (y1 > y2) {
    swap_int(&x1, &x2);
    swap_int(&y1, &y2);
  }
  if (y0 > y1) {
    swap_int(&x0, &x1);
    swap_int(&y0, &y1);
  }
  if (y1 > y2) {
    swap_int(&x1, &x2);
    swap_int(&y1, &y2);
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
        int pos = (int)(x + y * HC_RENDER_SIZE_X);
        hc_draw_buf[pos * 3] = r;
        hc_draw_buf[pos * 3 + 1] = g;
        hc_draw_buf[pos * 3 + 2] = b;
      }
      for (int x = (xf < HC_RENDER_SIZE_X ? (int)(xf) : HC_RENDER_SIZE_X - 1);
           x >= (xt > 0 ? xt : 0); x--) {
        int pos = (int)(x + y * HC_RENDER_SIZE_X);
        hc_draw_buf[pos * 3] = r;
        hc_draw_buf[pos * 3 + 1] = g;
        hc_draw_buf[pos * 3 + 2] = b;
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

void hc_render() {
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
      char *to_write = map_char(hc_draw_buf + ((x + y * HC_RENDER_SIZE_X)) * 3);
      strncpy(hc_render_buf + advance, to_write, strlen(to_write));
      advance += strlen(to_write);
      strncpy(hc_render_buf + advance, to_write, strlen(to_write));
      advance += strlen(to_write);
    }
    // printf("newline %d\n", ((y+1) * HC_RENDER_SIZE_X) * 2 + y);
    hc_render_buf[advance] = '\n';
    advance++;
  }
  hc_render_buf[advance] = '\0';
  printf("\033[0;0H%s", hc_render_buf);
}

#endif
