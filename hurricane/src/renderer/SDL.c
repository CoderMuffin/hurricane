#include "renderer.h"
#include <SDL2/SDL.h>
#include <hurricane/engine.h>
#include <hurricane/input.h>
#include <hurricane/renderer/SDL.h>
#include <math.h>
#include <stdio.h>

Uint32 hc_sdl_image[HC_RENDER_SIZE_Y * HC_RENDER_SIZE_X];
double hc_sdl_depth_buf[HC_RENDER_SIZE_Y][HC_RENDER_SIZE_X] = {0};

SDL_Window *hc_sdl_window;
SDL_Renderer *hc_sdl_renderer;
SDL_Texture *hc_sdl_bitmap;
SDL_Event hc_sdl_event;

void hc_sdl_init() {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
    return;
  }
  hc_sdl_window = SDL_CreateWindow("Hello World!", 100, 100, HC_RENDER_SIZE_X,
                                   HC_RENDER_SIZE_Y, SDL_WINDOW_SHOWN);
  if (hc_sdl_window == NULL) {
    fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
    return;
  }
  hc_sdl_renderer = SDL_CreateRenderer(
      hc_sdl_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (hc_sdl_renderer == NULL) {
    fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
    SDL_DestroyWindow(hc_sdl_window);
    SDL_Quit();
    return;
  }
  hc_sdl_bitmap = SDL_CreateTexture(hc_sdl_renderer, SDL_PIXELFORMAT_ARGB8888,
                                    SDL_TEXTUREACCESS_STATIC, HC_RENDER_SIZE_X,
                                    HC_RENDER_SIZE_Y);
}

void hc_sdl_pre_frame() {
  for (int y = 0; y < HC_RENDER_SIZE_Y; y++)
    for (int x = 0; x < HC_RENDER_SIZE_X; x++) {
      hc_sdl_image[y * HC_RENDER_SIZE_X + x] =
          (hc_render_bg[0] << 16) + (hc_render_bg[1] << 8) + hc_render_bg[2];
      hc_sdl_depth_buf[y][x] = INFINITY;
    }
}

void hc_sdl_triangle(int x0, int y0, double z0, int x1, int y1, double z1,
                     int x2, int y2, double z2, unsigned char r,
                     unsigned char g, unsigned char b) {
  HC_INTERNAL_BUF_TRIANGLE(
      x0, y0, z0, x1, y1, z1, x2, y2, z2, r, g, b,
      HC_INTERNAL_DEPTH_BUF_CHECK(
          x0, y0, z0, x1, y1, z1, x2, y2, z2, r, g, b, hc_sdl_depth_buf,
          hc_sdl_image[y * HC_RENDER_SIZE_X + x] = (r << 16) + (g << 8) + b;))
}

void hc_sdl_frame() {
  while (SDL_PollEvent(&hc_sdl_event)) {
    switch (hc_sdl_event.type) {
    case SDL_KEYDOWN:
      hc_internal_keydown(hc_sdl_event.key.keysym.sym);
      break;
    case SDL_KEYUP:
      hc_internal_keyup(hc_sdl_event.key.keysym.sym);
      break;
    case SDL_QUIT:
      hc_quit();
      break;
    }
  }
  SDL_UpdateTexture(hc_sdl_bitmap, NULL, hc_sdl_image,
                    HC_RENDER_SIZE_X * sizeof(Uint32));
  SDL_RenderClear(hc_sdl_renderer);
  SDL_RenderCopy(hc_sdl_renderer, hc_sdl_bitmap, NULL, NULL);
  SDL_RenderPresent(hc_sdl_renderer);
}

void hc_sdl_finish() {
  SDL_DestroyTexture(hc_sdl_bitmap);
  SDL_DestroyRenderer(hc_sdl_renderer);
  SDL_DestroyWindow(hc_sdl_window);
  SDL_Quit();
}

const hc_renderer hc_renderer_sdl = {.init = hc_sdl_init,
                                     .pre_frame = hc_sdl_pre_frame,
                                     .triangle = hc_sdl_triangle,
                                     .frame = hc_sdl_frame,
                                     .finish = hc_sdl_finish,
                                     .internal_depth_buf = &hc_sdl_depth_buf};
