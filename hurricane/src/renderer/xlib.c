#include "renderer_util.h"
#include <X11/X.h>
#include <X11/Xlib.h>
#include <hurricane/engine.h>
#include <hurricane/input.h>
#include <hurricane/renderer/renderer.h>
#include <hurricane/shared.h>
#include <math.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

static hc_renderer_config config;
static Display *display;
static Window window;
static XImage *image;
static Atom window_close_atom;
static float *depth_buf;

static void hc_xlib_process_event(Display *display, Window window) {
  XEvent ev;
  XNextEvent(display, &ev);
  switch (ev.type) {
  case KeyPress:
    hc_internal_keydown(ev.xkey.keycode);
    break;
  case KeyRelease:
    hc_internal_keyup(ev.xkey.keycode);
    break;
  case ClientMessage:
    if (ev.xclient.data.l[0] == window_close_atom) {
      hc_quit();
    }
  }
}

static void hc_xlib_init(hc_renderer_config renderer_config) {
  config = renderer_config;
  display = XOpenDisplay(NULL);

  Visual *visual = DefaultVisual(display, 0);
  window = XCreateSimpleWindow(display, RootWindow(display, 0), 0, 0,
                               config.width, config.height, 1, 0, 0);
  if (visual->class != TrueColor) {
    fprintf(stderr, "Cannot handle non true color visual ...\n");
    exit(1);
  }

  window_close_atom = XInternAtom(display, "WM_DELETE_WINDOW", False);
  XSetWMProtocols(display, window, (Atom[]){window_close_atom}, 1);

  image = XCreateImage(display, visual,
                       DefaultDepth(display, DefaultScreen(display)), ZPixmap,
                       0, malloc(config.width * config.height * 4),
                       config.width, config.height, 32, 0);
  depth_buf = malloc(sizeof(float) * config.width * config.height);
  XSelectInput(display, window,
               ButtonPressMask | KeyPressMask | KeyReleaseMask);
  XMapWindow(display, window);
}

static void hc_xlib_pre_frame(void) {
  for (int i = 0; i < config.width * config.height; i++) {
    image->data[i * 4 + 0] = config.clear[2];
    image->data[i * 4 + 1] = config.clear[1];
    image->data[i * 4 + 2] = config.clear[0];
    depth_buf[i] = INFINITY;
  }
}

static void hc_xlib_triangle(int x0, int y0, float z0, int x1, int y1,
                             float z1, int x2, int y2, float z2,
                             unsigned char r, unsigned char g,
                             unsigned char b) {
  HC_INTERNAL_BUF_TRIANGLE(
      x0, y0, z0, x1, y1, z1, x2, y2, z2, config.width, config.height,
      HC_INTERNAL_DEPTH_BUF_CHECK(
          x0, y0, z0, x1, y1, z1, x2, y2, z2, config.width, depth_buf,
          image->data[(y * config.width + x) * 4 + 0] = b;
          image->data[(y * config.width + x) * 4 + 1] = g;
          image->data[(y * config.width + x) * 4 + 2] = r;))
}

static void hc_xlib_frame(void) {
  XPutImage(display, window, DefaultGC(display, 0), image, 0, 0, 0, 0,
            config.width, config.height);
  XFlush(display);
  while (XPending(display) > 0) {
    hc_xlib_process_event(display, window);
  }
}

static void hc_xlib_finish(void) {
  XCloseDisplay(display);
  free(depth_buf);
}

const hc_renderer hc_renderer_xlib = {.init = hc_xlib_init,
                                      .pre_frame = hc_xlib_pre_frame,
                                      .triangle = hc_xlib_triangle,
                                      .frame = hc_xlib_frame,
                                      .finish = hc_xlib_finish,
                                      .internal_depth_buf = &depth_buf};
