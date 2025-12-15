#ifndef HC_RENDERER_H
#define HC_RENDERER_H

#include "../shared.h"

typedef struct hc_renderer_config {
  int width;
  int height;
  int clear[3];
} hc_renderer_config;

typedef struct hc_renderer {
  void (*init)(hc_renderer_config);
  void (*pre_frame)(void);
  void (*triangle)(int, int, double, int, int, double, int, int, double,
                   unsigned char, unsigned char, unsigned char);
  void (*frame)(void);
  void (*finish)(void);
  double (**internal_depth_buf);
} hc_renderer;

#endif
