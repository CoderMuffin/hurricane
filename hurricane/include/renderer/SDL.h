#ifndef HC_SDL_H
#define HC_SDL_H

#include "../engine.h"
#include "../input.h"
#include <SDL2/SDL.h>
#include <stdio.h>


void hc_sdl_pre_frame();
void hc_sdl_frame();
void hc_sdl_finish();

#endif
