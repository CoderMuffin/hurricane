#ifndef HC_ENGINE
#define HC_ENGINE
#include <stdbool.h>

void (*hc_render_call)(int *);
int *(*hc_render_preprocessor)(int *);
bool hc_use_preprocessor = false;

int *hc_frame() {}

void hc_init(void (*render_call)(int *), int *(*preprocessor)(int *)) {
  hc_render_call = render_call;
  hc_render_preprocessor = preprocessor;
  if (hc_use_preprocessor) {
    while (true) {
      hc_render_call(hc_render_preprocessor(hc_frame()));
    }
  } else {
    while (true) {
      hc_render_call(hc_frame());
    }
  }
}

#endif
