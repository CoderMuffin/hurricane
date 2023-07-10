#ifndef HC_RENDER_H
#define HC_RENDER_H

#include <stdio.h>
#include "util/vec.h"
#include <stdlib.h>


void hc_init_render(int sx, int sy);
char map_char(int v);
void hc_render(int *buf);

#endif
