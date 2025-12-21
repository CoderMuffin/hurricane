#include "renderer_util.h"
#include <SDL2/SDL.h>
#include <hurricane/engine.h>
#include <hurricane/input.h>
#include <hurricane/renderer/SDL.h>
#include <hurricane/renderer/renderer.h>
#include <stdio.h>

static Uint32 *image;
static fixed *depth_buf = {0};
static hc_renderer_config config;

static SDL_Window *window;
static SDL_Renderer *renderer;
static SDL_Texture *bitmap;
static SDL_Event event;

static void init(hc_renderer_config renderer_config) {
  config = renderer_config;
  image = malloc(sizeof(Uint32) * config.width * config.height);
  depth_buf = malloc(sizeof(fixed) * config.width * config.height);

  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
    return;
  }
  window = SDL_CreateWindow("Hello World!", 100, 100, config.width,
                                   config.height, SDL_WINDOW_SHOWN);
  if (window == NULL) {
    fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
    return;
  }
  renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (renderer == NULL) {
    fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
    SDL_DestroyWindow(window);
    SDL_Quit();
    return;
  }
  bitmap = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                    SDL_TEXTUREACCESS_STATIC, config.width,
                                    config.height);
}

static void pre_frame(void) {
  for (int y = 0; y < config.height; y++)
    for (int x = 0; x < config.width; x++) {
      image[y * config.width + x] =
          (config.clear[0] << 16) + (config.clear[1] << 8) + config.clear[2];
      depth_buf[y * config.width + x] = tf(1000);
    }
}

static void triangle(int x0, int y0, fixed z0, int x1, int y1, fixed z1,
                     int x2, int y2, fixed z2, unsigned char r,
                     unsigned char g, unsigned char b) {
  HC_INTERNAL_BUF_TRIANGLE(
      x0, y0, z0, x1, y1, z1, x2, y2, z2, config.width, config.height,
      HC_INTERNAL_DEPTH_BUF_CHECK(
          x0, y0, z0, x1, y1, z1, x2, y2, z2, config.width, depth_buf,
          image[y * config.width + x] = (r << 16) + (g << 8) + b;))
}

static void frame(void) {
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_KEYDOWN:
      hc_internal_keydown(event.key.keysym.sym);
      break;
    case SDL_KEYUP:
      hc_internal_keyup(event.key.keysym.sym);
      break;
    case SDL_QUIT:
      hc_quit();
      break;
    }
  }
  SDL_UpdateTexture(bitmap, NULL, image,
                    config.width * sizeof(Uint32));
  SDL_RenderClear(renderer);
  SDL_RenderCopy(renderer, bitmap, NULL, NULL);
  SDL_RenderPresent(renderer);
}

static void finish(void) {
  SDL_DestroyTexture(bitmap);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

const hc_renderer hc_renderer_sdl = {.init = init,
                                     .pre_frame = pre_frame,
                                     .triangle = triangle,
                                     .frame = frame,
                                     .finish = finish,
                                     .internal_depth_buf = &depth_buf};
