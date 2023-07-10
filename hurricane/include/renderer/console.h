#ifndef HC_RENDER_H
#define HC_RENDER_H

#include "../shared.h"
#include <stdio.h>
#include <stdlib.h>
#include "../input.h"
#include <string.h>
#include <math.h>
#include "renderer.h"


void *hc_internal_console_input_handler(void *_);
void hc_console_init();
void hc_console_pre_frame();
char *map_char(unsigned char *vs);
void hc_console_blit(char *text, int x, int y);
void hc_console_frame();
void hc_console_finish();

#endif
