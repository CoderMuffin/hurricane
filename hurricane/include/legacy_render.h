#if 0
#ifndef HC_RENDER_H
#define HC_RENDER_H

//macros
#include <stdio.h>
#include "util/vec.h"
#include <stdlib.h>
#define CHAR_SIZE (sizeof(char)*1)

//types

//decls

//functions
void hc_init_render(int sx, int sy);
char map_char(int v);
void hc_render(int *buf);

#endif
#endif
