#ifndef HC_VIDEO_RENDER_H
#define HC_VIDEO_RENDER_H

//macros
#include "../shared.h"
#include "renderer.h"
#include <stdio.h>
#include <memory.h>
#include <math.h>

//types

//decls
extern const hc_renderer hc_renderer_video;

//functions
void hc_video_init();
void hc_video_pre_frame();
void hc_video_frame();
void hc_video_finish();

#endif
