

#include "SDL2/SDL.h"
#include "SDL_rect.h"
#include "SDL_render.h"
#include "SDL_video.h"
#include <cmath>
#include <cstddef>
#include <cstdint>

typedef struct {
  float x, y;
} vector2;

vector2 randomGradient(int ix, int iy) {
  const unsigned w = 8 * sizeof(unsigned);
  const unsigned s = w / 2;

  unsigned a = ix;
  unsigned b = iy;

  a *= 3284157443;
  b ^= a << s | a >> w - s;
  b *= 1911520717;
  a ^= b << s | b >> w - s;

  a *= 2048419325;

  float random = a * (3.14159265 / ~(~0u >> 1));

  // Create the vector from the angle
  vector2 v;

  v.x = sin(random);
  v.y = cos(random);

  return v;
}

float dotGridGradient(int ix, int iy, float x, float y) {
  vector2 gradient = randomGradient(ix, iy);

  float dx = x - static_cast<float>(ix);
  float dy = y - static_cast<float>(iy);

  return (dx * gradient.x + dy * gradient.y);
}

float interpolate(float a0, float a1, float w) {
  return (a1 - a0) * (3.0 - w * 2.0) * w * w + a0;
}

float perlin(float x, float y) {
  int x0 = static_cast<int>(x);
  int y0 = static_cast<int>(y);
  int x1 = x0 + 1;
  int y1 = y0 + 1;

  float sx = x - static_cast<float>(x0);
  float sy = y - static_cast<float>(y0);

  float n0 = dotGridGradient(x0, y0, x, y);
  float n1 = dotGridGradient(x1, y0, x, y);
  float ix0 = interpolate(n0, n1, sx);

  // For bottom two points
  n0 = dotGridGradient(x0, y1, x, y);
  n1 = dotGridGradient(x1, y1, x, y);

  float ix1 = interpolate(n0, n1, sx);

  float value = interpolate(ix0, ix1, sy);

  return value;
}
int main() {

  const int windowHeight = 1080;
  const int windowWidth = 1920;

  SDL_Init(SDL_INIT_VIDEO);

  SDL_Window *window;

  window = SDL_CreateWindow("Perlins Nosie", SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight,
                            SDL_WINDOW_OPENGL);

	SDL_Renderer *renderer;

	renderer = SDL_CreateRenderer(window, -1, 0);

  uint8_t *pixels = new uint8_t[windowWidth * windowHeight * 4];

  const int GRID_SIZE = 1600;

  for (int x = 0; x < windowWidth; x++) {
    for (int y = 0; y < windowHeight; y++) {
      int index = (y * windowWidth + x) * 4;

      float val = 0;

      float freq = 1;
      float amp = 1;

      for (int i = 0; i < 12; i++) {
        val += perlin(x * freq / GRID_SIZE, y * freq / GRID_SIZE) * amp;

        freq *= 2;
        amp /= 2;
      }

      val *= 1.2;

      // clipping

      if (val > 1.0f)
        val = 1.0f;
      else if (val < -1.0f)
        val = -1.0f;

			int color = static_cast<int>(((val + 1.0f) * 0.5f)* 255);


			pixels[index] = color;
			pixels[index + 1] = color;
			pixels[index + 2] = color;
			pixels[index + 3] = 255;
    }
  }

	SDL_Texture *texture = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_ARGB8888,SDL_TEXTUREACCESS_STREAMING , windowWidth,windowHeight );

	SDL_Rect spriteRect = {0, 0, windowWidth, windowHeight};

	SDL_UpdateTexture(texture, NULL, pixels, windowWidth *4 );

bool isOpen = true;
while (isOpen) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            isOpen = false;
        }
    }
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, texture, NULL, &spriteRect);
		SDL_RenderPresent(renderer);
}
 // Generate a value between 0 and 1
}
