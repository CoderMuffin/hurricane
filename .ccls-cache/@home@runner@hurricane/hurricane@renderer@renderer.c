#ifndef HC_RENDERER
#define HC_RENDERER

typedef struct hc_renderer {
  void (*before_render)();
  void (*triangle)(int, int, double, int, int, double, int, int, double,
                   unsigned char, unsigned char, unsigned char);
  void (*render)();
} hc_renderer;

#endif
