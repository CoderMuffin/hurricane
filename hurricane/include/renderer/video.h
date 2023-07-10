#ifndef HC_VIDEO_RENDER_H
#define HC_VIDEO_RENDER_H

#include "../shared.h"
#include "renderer.h"
#include <stdio.h>
#include <memory.h>
#include <math.h>


void hc_video_init();
void hc_video_pre_frame();
void hc_video_frame();
void hc_video_finish();

#endif
