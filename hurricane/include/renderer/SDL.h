#ifndef HC_SDL_H
#define HC_SDL_H

//macros
#include "../engine.h"
#include "../input.h"
#include <stdio.h>

//types

//decls
extern const hc_renderer hc_renderer_sdl;

//functions
void hc_sdl_pre_frame();
void hc_sdl_frame();
void hc_sdl_finish();

#endif
