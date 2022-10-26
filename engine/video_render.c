#ifndef HC_VIDEO_RENDER
#define HC_VIDEO_RENDER
#include "shared.c"
#include <stdio.h>

unsigned char hc_video_buf[HC_RENDER_SIZE_Y][HC_RENDER_SIZE_X][3] = {0};
FILE *hc_video_pipe;

// void swap_int(int *a, int *b) {
//   int temp = *b;
//   *b = *a;
//   *a = temp;
// }

void hc_init_render_video() {
  hc_video_pipe = popen("ffmpeg -y -f rawvideo -vcodec rawvideo -pix_fmt rgb24 "
                        "-s " HC_RENDER_SIZE_STR_X "x" HC_RENDER_SIZE_STR_Y " "
                        "-r 25 -i - -f mp4 -q:v 5 -an -vcodec mpeg4 output.mp4",
                        "w");
}
int y, x;
void hc_video_pre_frame() {
  for (int y = 0; y < HC_RENDER_SIZE_Y; y++)
    for (int x = 0; x < HC_RENDER_SIZE_X; x++)
      for (int q = 0; q < 3; q++)
        hc_video_buf[y][x][q] = 100;
}
void hc_video_triangle(int x0, int y0, int x1, int y1, int x2, int y2,
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
        // int pos = (int)(x + y * HC_RENDER_SIZE_X);
        hc_video_buf[y][x][0] = r;
        hc_video_buf[y][x][1] = g;
        hc_video_buf[y][x][2] = b;
      }
      for (int x = (xf < HC_RENDER_SIZE_X ? (int)(xf) : HC_RENDER_SIZE_X - 1);
           x >= (xt > 0 ? xt : 0); x--) {
        // int pos = (int)(x + y * HC_RENDER_SIZE_X);
        hc_video_buf[y][x][0] = r;
        hc_video_buf[y][x][1] = g;
        hc_video_buf[y][x][2] = b;
      }
    }
    xf += dx_far;
    if (y < y1)
      xt += dx_upper;
    else
      xt += dx_low;
  }
}
void hc_video_frame() {
  // Write this frame to the output pipe
  fwrite(hc_video_buf, 1, HC_RENDER_SIZE_Y * HC_RENDER_SIZE_X * 3,
         hc_video_pipe);
}
void hc_video_finish() {
  fflush(hc_video_pipe);
  pclose(hc_video_pipe);
}
#endif
