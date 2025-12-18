#ifndef HC_ENGINE_H
#define HC_ENGINE_H

#include "object.h"
#include "renderer/renderer.h"
#include <stdbool.h>

void hc_geometry_to_world(hc_object *object, int index, double out[3]);
double hc_internal_lighting(hc_object *object, int i);
void hc_render_object(const hc_renderer *const renderer, const hc_renderer_config *const renderer_config, hc_object *camera, hc_object *object);
void hc_set_fov(double fov, hc_renderer_config renderer_config, bool use_height);
bool hc_should_quit();
void hc_quit();

#endif
