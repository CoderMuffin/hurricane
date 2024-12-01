#ifndef HC_XLIB_H
#define HC_XLIB_H

//macros
#include "../X11/Xlib.h"
#include "../X11/X.h"
#include "../input.h"
#include "../shared.h"
#include "../renderer/renderer.h"
#include <math.h>
#include <memory.h>
#include <stdio.h>

//types

//decls
extern const hc_renderer hc_renderer_xlib;

//functions
void hc_xlib_process_event(Display *display, Window window);
void hc_xlib_init();
void hc_xlib_pre_frame();
void hc_xlib_frame();
void hc_xlib_finish();

#endif
