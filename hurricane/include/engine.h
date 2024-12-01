#ifndef HC_ENGINE_H
#define HC_ENGINE_H

//macros
#include "object.h"
#include "renderer/renderer.h"
#include "shared.h"
#include "util/mat.h"
#include "util/vec.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

//types

//decls
bool hc_internal_quit;
void (*hc_render_triangle_call)(int, int, double, int, int, double, int, int,
                                double, unsigned char, unsigned char,
                                unsigned char);
int debug_triangle_count;
double hc_internal_eye_dist;

//functions
void hc_geometry_to_world(hc_object *object, int index, double out[3]);
double hc_internal_lighting(hc_object *object, int i);
void hc_render_object(hc_object *camera, hc_object *object);
void hc_set_fov(double fov, bool use_height);
void hc_init(const bool hc_render_progress, int frames, hc_renderer renderer, void (*update)());

#endif
