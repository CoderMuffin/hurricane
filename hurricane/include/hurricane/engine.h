#ifndef HC_ENGINE_H
#define HC_ENGINE_H

#include "object.h"
#include "renderer/renderer.h"
#include <stdbool.h>

void hc_geometry_to_world(hc_object *object, int index, double out[3]);
double hc_internal_lighting(hc_object *object, int i);
void hc_render_object(hc_object *camera, hc_object *object);
void hc_set_fov(double fov, bool use_height);
void hc_init(const bool hc_render_progress, int frames, hc_renderer renderer,
             void (*update)());
void hc_quit();

#endif
