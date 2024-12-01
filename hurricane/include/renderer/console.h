#ifndef HC_RENDER_H
#define HC_RENDER_H

//macros
#include "../shared.h"
#include <stdio.h>
#include <stdlib.h>
#include "../input.h"
#include <string.h>
#include <math.h>
#include "renderer.h"
#define CHAR_SIZE (sizeof(char) * 30)

//types

//decls
extern const hc_renderer hc_renderer_console;

//functions
void *hc_internal_console_input_handler(void *_);
void hc_console_init();
void hc_console_pre_frame();
char *map_char(unsigned char *vs);
void hc_console_blit(char *text, int x, int y);
void hc_console_frame();
void hc_console_finish();

#endif
