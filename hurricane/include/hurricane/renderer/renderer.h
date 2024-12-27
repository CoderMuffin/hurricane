#ifndef HC_RENDERER_H
#define HC_RENDERER_H

#include "../shared.h"

typedef struct hc_renderer {
  void (*init)(void);
  void (*pre_frame)(void);
  void (*triangle)(int, int, double, int, int, double, int, int, double,
                   unsigned char, unsigned char, unsigned char);
  void (*frame)(void);
  void (*finish)(void);
  double (*internal_depth_buf)[HC_RENDER_SIZE_Y][HC_RENDER_SIZE_X];
} hc_renderer;

#endif
