#include "renderer_util.h"
#include <hurricane/renderer/renderer.h>
#include <hurricane/shared.h>
#include <math.h>
#include <memory.h>
#include <stdio.h>

unsigned char hc_video_buf[HC_RENDER_SIZE_Y][HC_RENDER_SIZE_X][3] = {0};
double hc_video_depth_buf[HC_RENDER_SIZE_Y][HC_RENDER_SIZE_X] = {0};
FILE *hc_video_pipe;

// void swap_int(int *a, int *b) {
//   int temp = *b;
//   *b = *a;
//   *a = temp;
// }
void hc_video_init() {
  // printf(("ideo -pix_fmt rgb24 " "-s " HC_RENDER_SIZE_STR_X "x"
  // HC_RENDER_SIZE_STR_Y " "));
  hc_video_pipe = popen("ffmpeg -fflags +discardcorrupt -hide_banner -y -f "
                        "rawvideo -vcodec rawvideo -pix_fmt rgb24 "
                        "-s " HC_RENDER_SIZE_STR_X "x" HC_RENDER_SIZE_STR_Y " "
                        "-r 25 -i - -f mp4 -q:v 2 -an -vcodec mpeg4 output.mp4",
                        "w");
}
void hc_video_pre_frame() {
  // memset(hc_video_depth_buf, 0, sizeof(hc_video_depth_buf));
  for (int y = 0; y < HC_RENDER_SIZE_Y; y++)
    for (int x = 0; x < HC_RENDER_SIZE_X; x++) {
      hc_video_buf[y][x][0] = hc_render_bg[0];
      hc_video_buf[y][x][1] = hc_render_bg[1];
      hc_video_buf[y][x][2] = hc_render_bg[2];
      hc_video_depth_buf[y][x] = INFINITY;
    }
}
void hc_video_triangle(int x0, int y0, double z0, int x1, int y1, double z1,
                       int x2, int y2, double z2, unsigned char r,
                       unsigned char g, unsigned char b) {
  HC_INTERNAL_BUF_TRIANGLE(
      x0, y0, z0, x1, y1, z1, x2, y2, z2, r, g, b,
      HC_INTERNAL_DEPTH_BUF_CHECK(x0, y0, z0, x1, y1, z1, x2, y2, z2, r, g, b,
                                  hc_video_depth_buf, hc_video_buf[y][x][0] = r;
                                  hc_video_buf[y][x][1] = g;
                                  hc_video_buf[y][x][2] = b;))
}
void hc_video_frame() {
  // Write this frame to the output pipe
  fwrite(hc_video_buf, sizeof(unsigned char),
         HC_RENDER_SIZE_Y * HC_RENDER_SIZE_X * 3, hc_video_pipe);
  fflush(hc_video_pipe);
}
void hc_video_finish() {
  fflush(hc_video_pipe);
  pclose(hc_video_pipe);
}
const hc_renderer hc_renderer_video = {.init = hc_video_init,
                                       .pre_frame = hc_video_pre_frame,
                                       .triangle = hc_video_triangle,
                                       .frame = hc_video_frame,
                                       .finish = hc_video_finish,
                                       .internal_depth_buf =
                                           &hc_video_depth_buf};
