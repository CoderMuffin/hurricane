#ifndef HC_RENDERER
#define HC_RENDERER

typedef struct hc_renderer {
  void (*init)();
  void (*pre_frame)();
  void (*triangle)(int, int, double, int, int, double, int, int, double,
                   unsigned char, unsigned char, unsigned char);
  void (*frame)();
  void (*finish)();
} hc_renderer;

#endif
