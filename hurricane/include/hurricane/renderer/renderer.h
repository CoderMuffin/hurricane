#ifndef HC_RENDERER_H
#define HC_RENDERER_H

fixed calculate_depth(fixed x0, fixed y0, fixed z0, fixed x1, fixed y1,
                       fixed z1, fixed x2, fixed y2, fixed z2, fixed px,
                       fixed py);

typedef struct hc_renderer_config {
  int width;
  int height;
  int clear[3];
} hc_renderer_config;

typedef struct hc_renderer {
  void (*init)(hc_renderer_config);
  void (*pre_frame)(void);
  void (*triangle)(int, int, fixed, int, int, fixed, int, int, fixed,
                   unsigned char, unsigned char, unsigned char);
  void (*frame)(void);
  void (*finish)(void);
  fixed (**internal_depth_buf);
} hc_renderer;

#endif
