#ifndef HC_RENDERER
#define HC_RENDERER
#include "../shared.c"

typedef struct hc_renderer {
  void (*init)();
  void (*pre_frame)();
  void (*triangle)(int, int, double, int, int, double, int, int, double,
                   unsigned char, unsigned char, unsigned char);
  void (*frame)();
  void (*finish)();
  double (*internal_depth_buf)[HC_RENDER_SIZE_Y][HC_RENDER_SIZE_X];
} hc_renderer;

#endif
