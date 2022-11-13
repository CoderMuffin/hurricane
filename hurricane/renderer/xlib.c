#ifndef HC_XLIB
#define HC_XLIB
#include "../X11/Xlib.h"
#include "../X11/X.h"
#include "../input.c"
#include "../shared.c"
#include "renderer.c"
#include <math.h>
#include <memory.h>
#include <stdio.h>

unsigned char hc_xlib_buf[HC_RENDER_SIZE_Y][HC_RENDER_SIZE_X][4] = {0};
double hc_xlib_depth_buf[HC_RENDER_SIZE_Y][HC_RENDER_SIZE_X] = {0};
XImage *hc_xlib_image;

// void swap_int(int *a, int *b) {
//   int temp = *b;
//   *b = *a;
//   *a = temp;
// }

XImage *CreateTrueColorImage(Display *display, Visual *visual,
                             unsigned char *image) {
  int i, j;
  char *image32 = malloc(HC_RENDER_SIZE_X * HC_RENDER_SIZE_Y * 4);
  char *p = image32;
  for (i = 0; i < HC_RENDER_SIZE_X; i++) {
    for (j = 0; j < HC_RENDER_SIZE_Y; j++) {
      if ((i < 256) && (j < 256)) {
        *p++ = rand() % 256; // blue
        *p++ = rand() % 256; // green
        *p++ = rand() % 256; // red
      } else {
        *p++ = i % 256; // blue
        *p++ = j % 256; // green
        if (i < 256)
          *p++ = i % 256; // red
        else if (j < 256)
          *p++ = j % 256; // red
        else
          *p++ = (256 - j) % 256; // red
      }
      p++;
    }
  }
  return XCreateImage(display, visual,
                      DefaultDepth(display, DefaultScreen(display)), ZPixmap, 0,
                      image32, HC_RENDER_SIZE_X, HC_RENDER_SIZE_Y, 32, 0);
}

void hc_xlib_process_event(Display *display, Window window) {

  // static char *tir="This is red";
  // static char *tig="This is green";
  // static char *tib="This is blue";
  XEvent ev;
  XNextEvent(display, &ev);
  switch (ev.type) {
  case KeyPress:
    hc_internal_keydown(ev.xkey.keycode);
    break;
  case KeyRelease:
    hc_internal_keyup(ev.xkey.keycode);
    break;
  case ButtonPress:
    break;
    // exit(0);
  }
}

Display *hc_xlib_display;
Window hc_xlib_window;
XImage *hc_xlib_image;

void hc_xlib_init() {
  hc_xlib_display = XOpenDisplay(NULL);
  Visual *visual = DefaultVisual(hc_xlib_display, 0);
  hc_xlib_window =
      XCreateSimpleWindow(hc_xlib_display, RootWindow(hc_xlib_display, 0), 0, 0,
                          HC_RENDER_SIZE_X, HC_RENDER_SIZE_Y, 1, 0, 0);
  if (visual->class != TrueColor) {
    fprintf(stderr, "Cannot handle non true color visual ...\n");
    exit(1);
  }

  hc_xlib_image = CreateTrueColorImage(hc_xlib_display, visual, 0);
  XSelectInput(hc_xlib_display, hc_xlib_window,
               ButtonPressMask | KeyPressMask | KeyReleaseMask);
  XMapWindow(hc_xlib_display, hc_xlib_window);
}
void hc_xlib_pre_frame() {
  // memset(hc_xlib_depth_buf, 0, sizeof(hc_xlib_depth_buf));
  for (int y = 0; y < HC_RENDER_SIZE_Y; y++)
    for (int x = 0; x < HC_RENDER_SIZE_X; x++) {
      hc_xlib_image->data[(y * HC_RENDER_SIZE_X + x) * 4 + 0] = hc_render_bg[0];
      hc_xlib_image->data[(y * HC_RENDER_SIZE_X + x) * 4 + 1] = hc_render_bg[1];
      hc_xlib_image->data[(y * HC_RENDER_SIZE_X + x) * 4 + 2] = hc_render_bg[2];
      hc_xlib_depth_buf[y][x] = INFINITY;
    }
}
void hc_xlib_triangle(int x0, int y0, double z0, int x1, int y1, double z1,
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
        // if (logq) {
        //   printf("%f %f %f %f\n", z0, z1, z2, depth);
        // }
        if (isnan(depth) || !isfinite(depth) || depth <= 0.1) {
          continue;
        }
        if (depth < hc_xlib_depth_buf[y][x]) {
          hc_xlib_depth_buf[y][x] = depth;
          hc_xlib_image->data[(y * HC_RENDER_SIZE_X + x) * 4 + 0] = r;
          hc_xlib_image->data[(y * HC_RENDER_SIZE_X + x) * 4 + 1] = g;
          hc_xlib_image->data[(y * HC_RENDER_SIZE_X + x) * 4 + 2] = b;
        }
      }
      for (int x = (xf < HC_RENDER_SIZE_X ? (int)(xf) : HC_RENDER_SIZE_X - 1);
           x >= (xt > 0 ? xt : 0); x--) {
        // int pos = (int)(x + y * HC_RENDER_SIZE_X);
        double depth =
            calculate_depth(x0, y0, z0, x1, y1, z1, x2, y2, z2, x, y);
        // if (logq) {
        //   printf("%f %f %f %f\n", z0, z1, z2, depth);
        // }
        if (isnan(depth) || !isfinite(depth) || depth <= 0.1) {
          continue;
        }
        if (depth < hc_xlib_depth_buf[y][x]) {
          hc_xlib_depth_buf[y][x] = depth;
          hc_xlib_image->data[(y * HC_RENDER_SIZE_X + x) * 4 + 0] = r;
          hc_xlib_image->data[(y * HC_RENDER_SIZE_X + x) * 4 + 1] = g;
          hc_xlib_image->data[(y * HC_RENDER_SIZE_X + x) * 4 + 2] = b;
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
void hc_xlib_frame() {
  // Write this frame to the output pipe
  // fwrite(hc_xlib_buf, sizeof(unsigned char), HC_RENDER_SIZE_Y *
  // HC_RENDER_SIZE_X * 3,
  //        hc_xlib_pipe);
  // fflush(hc_xlib_pipe);
  XPutImage(hc_xlib_display, hc_xlib_window, DefaultGC(hc_xlib_display, 0),
            hc_xlib_image, 0, 0, 0, 0, HC_RENDER_SIZE_X, HC_RENDER_SIZE_Y);
  XFlush(hc_xlib_display);
  while (XPending(hc_xlib_display) > 0) {
    hc_xlib_process_event(hc_xlib_display, hc_xlib_window);
  }
}
void hc_xlib_finish() {
  XCloseDisplay(hc_xlib_display);
  // fflush(hc_xlib_pipe);
  // pclose(hc_xlib_pipe);
}


const hc_renderer hc_renderer_xlib = {
    .init = hc_xlib_init,
    .pre_frame = hc_xlib_pre_frame,
    .triangle = hc_xlib_triangle,
    .frame = hc_xlib_frame,
    .finish = hc_xlib_finish
};

#endif
