#ifdef HC_ENABLE_SDL2
#ifndef HC_SDL
#define HC_SDL

#include "../engine.c"
#include "../input.c"
#include <SDL2/SDL.h>
#include <stdio.h>

Uint32 hc_sdl_image[HC_RENDER_SIZE_Y * HC_RENDER_SIZE_X];
double hc_sdl_depth_buf[HC_RENDER_SIZE_Y][HC_RENDER_SIZE_X] = {0};

SDL_Window* hc_sdl_window;
SDL_Renderer* hc_sdl_renderer;
SDL_Texture* hc_sdl_bitmap;
SDL_Event hc_sdl_event;

void hc_sdl_init()
{
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
    return;
  }
    hc_sdl_window = SDL_CreateWindow("Hello World!", 100, 100, HC_RENDER_SIZE_X, HC_RENDER_SIZE_Y, SDL_WINDOW_SHOWN);
  if (hc_sdl_window == NULL) {
    fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
    return;
  }
    hc_sdl_renderer = SDL_CreateRenderer(hc_sdl_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (hc_sdl_renderer == NULL) {
    fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
    SDL_DestroyWindow(hc_sdl_window);
    SDL_Quit();
    return;
  }
    hc_sdl_bitmap = SDL_CreateTexture(hc_sdl_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, HC_RENDER_SIZE_X, HC_RENDER_SIZE_Y);
}

void hc_sdl_pre_frame() {
  for (int y = 0; y < HC_RENDER_SIZE_Y; y++)
    for (int x = 0; x < HC_RENDER_SIZE_X; x++) {
      hc_sdl_image[y * HC_RENDER_SIZE_X + x] = (hc_render_bg[0] << 16) + (hc_render_bg[1] << 8) + hc_render_bg[2];
      hc_sdl_depth_buf[y][x] = INFINITY;
    }
}

void hc_sdl_triangle(int x0, int y0, double z0, int x1, int y1, double z1,
                      int x2, int y2, double z2, unsigned char r,
                      unsigned char g, unsigned char b) {
  // sort the points vertically
  if (y1 > y2) {
    swap_int(&x1, &x2);
    swap_int(&y1, &y2);
    swap_dbl(&z1, &z2);
  }
  if (y0 > y1) {
    swap_int(&x0, &x1);
    swap_int(&y0, &y1);
    swap_dbl(&z0, &z1);
  }
  if (y1 > y2) {
    swap_int(&x1, &x2);
    swap_int(&y1, &y2);
    swap_dbl(&z1, &z2);
  }
  double dx_far = ((double)(x2 - x0)) / (y2 - y0 + 1);
  double dx_upper = ((double)(x1 - x0)) / (y1 - y0 + 1);
  double dx_low = ((double)(x2 - x1)) / (y2 - y1 + 1);
  double xf = x0;
  double xt = x0 + dx_upper; // if y0 == y1, special case
  for (int y = y0; y <= (y2 > HC_RENDER_SIZE_Y - 1 ? HC_RENDER_SIZE_Y - 1 : y2);
       y++) {
    if (y >= 0) {
      for (int x = (xf > 0 ? (int)(xf) : 0);
           x <= (xt < HC_RENDER_SIZE_X ? xt : HC_RENDER_SIZE_X - 1); x++) {
        // int pos = (int)(x + y * HC_RENDER_SIZE_X);
        double depth =
            calculate_depth(x0, y0, z0, x1, y1, z1, x2, y2, z2, x, y);
        // if (logq) {
        //   printf("%f %f %f %f\n", z0, z1, z2, depth);
        // }
        if (isnan(depth) || !isfinite(depth)) {
          continue;
        }
        if (depth < hc_sdl_depth_buf[y][x]) {
          hc_sdl_depth_buf[y][x] = depth;
          hc_sdl_image[y * HC_RENDER_SIZE_X + x] = (r << 16) + (g << 8) + b;
          //hc_sdl_image[y * HC_RENDER_SIZE_X + x] = ((int)(depth * 70) << 16) + ((int)(depth * 70) << 8) + (int)(depth * 70);
          //hc_sdl_image[y * HC_RENDER_SIZE_X + x] = (((int)(depth*5) * 10) << 16) + (((int)(depth*5) * 10) << 8) + ((int)(depth*5) * 10);
        }
      }
      for (int x = (xf < HC_RENDER_SIZE_X ? (int)(xf) : HC_RENDER_SIZE_X - 1);
           x >= (xt > 0 ? xt : 0); x--) {
        // int pos = (int)(x + y * HC_RENDER_SIZE_X);
        double depth =
            calculate_depth(x0, y0, z0, x1, y1, z1, x2, y2, z2, x, y);
        // if (logq) {
        //   printf("%f %f %f %f\n", z0, z1, z2, depth);
        // }
        if (isnan(depth) || !isfinite(depth)) {
          continue;
        }
        if (depth < hc_sdl_depth_buf[y][x]) {
          hc_sdl_depth_buf[y][x] = depth;
          hc_sdl_image[y * HC_RENDER_SIZE_X + x] = (r << 16) + (g << 8) + b;
          //hc_sdl_image[y * HC_RENDER_SIZE_X + x] = ((int)(depth * 70) << 16) + ((int)(depth * 70) << 8) + (int)(depth * 70);
          //hc_sdl_image[y * HC_RENDER_SIZE_X + x] = (((int)(depth*5) * 10) << 16) + (((int)(depth*5) * 10) << 8) + ((int)(depth*5) * 10);
        }
      }
    }
    xf += dx_far;
    if (y < y1)
      xt += dx_upper;
    else
      xt += dx_low;
  }
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
                hc_internal_quit = true;
                break;
        }
    }
    SDL_UpdateTexture(hc_sdl_bitmap, NULL, hc_sdl_image, HC_RENDER_SIZE_X * sizeof(Uint32));
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

const hc_renderer hc_renderer_sdl = {
    .init = hc_sdl_init,
    .pre_frame = hc_sdl_pre_frame,
    .triangle = hc_sdl_triangle,
    .frame = hc_sdl_frame,
    .finish = hc_sdl_finish,
    .internal_depth_buf = &hc_sdl_depth_buf
};

#endif
#endif
