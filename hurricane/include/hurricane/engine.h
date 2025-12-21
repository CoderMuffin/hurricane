#ifndef HC_ENGINE_H
#define HC_ENGINE_H

#include "object.h"
#include "fixed.h"
#include "renderer/renderer.h"
#include <stdbool.h>

void hc_geometry_to_world(hc_object *object, int index, fixed out[3]);
fixed hc_internal_lighting(hc_object *object, int i);
void hc_render_object(hc_object *camera, hc_object *object);
void hc_set_fov(fixed fov, hc_renderer_config renderer_config, bool use_height);
void hc_init(const bool hc_render_progress, int frames, hc_renderer renderer, hc_renderer_config renderer_config,
             void (*update)());
void hc_quit();

#endif
