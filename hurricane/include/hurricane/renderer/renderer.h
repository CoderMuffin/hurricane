#ifndef HC_RENDERER_H
#define HC_RENDERER_H

float calculate_depth(float x0, float y0, float z0, float x1, float y1,
                       float z1, float x2, float y2, float z2, float px,
                       float py);

typedef struct hc_renderer_config {
  int width;
  int height;
  int clear[3];
} hc_renderer_config;

typedef struct hc_renderer {
  void (*init)(hc_renderer_config);
  void (*pre_frame)(void);
  void (*triangle)(int, int, float, int, int, float, int, int, float,
                   unsigned char, unsigned char, unsigned char);
  void (*frame)(void);
  void (*finish)(void);
  float (**internal_depth_buf);
} hc_renderer;

#endif
