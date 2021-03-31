#include "graphics.h"
#include <SDL2/SDL_ttf.h>

/// @todo Put this in Graphics::font or somewhere.
TTF_Font *global_font = nullptr;

Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : r(r), g(g), b(b), a(a) {}

void initialize() {
  static bool initialized = false;
  if (initialized) return;
  initialized = true;

  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    fprintf(stderr, "Failed to initialize SDL2: %s\n", SDL_GetError());
    exit(1);
  }

  if (TTF_Init() != 0) {
    fprintf(stderr, "Failed to initialize TTF: %s\n", TTF_GetError());
    exit(1);
  }

  global_font = TTF_OpenFont("assets/font.ttf", 72);
  if (global_font == NULL) {
    fprintf(stderr, "Failed to initialize font: %s\n", TTF_GetError());
    // not a fatal error
  }
}

struct Graphics::Internal {
  SDL_Window* window;
  SDL_Renderer* renderer;
};

Graphics::Graphics(const char* title, int w, int h) {
  initialize();

  internal = new Internal;
  internal->window = nullptr;
  internal->renderer = nullptr;

  uint32_t flags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;
  if (0 != SDL_CreateWindowAndRenderer(w,h,flags,&internal->window,&internal->renderer)) {
    fprintf(stderr, "Failed to create window: %s\n", SDL_GetError());
    exit(1);
  }
  SDL_SetWindowTitle(internal->window, title);
}

Graphics::~Graphics() {
  textures.clear();
  SDL_DestroyRenderer(internal->renderer);
  SDL_DestroyWindow(internal->window);
  delete internal;
}

bool Graphics::loop() {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) return false;
    if (e.type == SDL_MOUSEMOTION) {
      if (motionCallback) motionCallback(e.motion.x, e.motion.y);
    }
    if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
      if (touchCallback) touchCallback(e.button.x, e.button.y);
    }
    if (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT) {
      if (releaseCallback) releaseCallback(e.button.x, e.button.y);
    }
  }
  return true;
}

void Graphics::swap() {
  SDL_RenderPresent(internal->renderer);
}

void Graphics::clear(Color c) {
  SDL_SetRenderDrawColor(internal->renderer, c.r, c.g, c.b, c.a);
  SDL_RenderClear(internal->renderer);
}

void Graphics::setMotionCallback(MotionCallback cb) { motionCallback = cb; }
void Graphics::setTouchCallback(TouchCallback cb) { touchCallback = cb; }
void Graphics::setReleaseCallback(ReleaseCallback cb) { releaseCallback = cb; }

void Graphics::setLogical(int w, int h) {
  if (0 != SDL_RenderSetLogicalSize(internal->renderer, w, h)) {
    fprintf(stderr, "Failed to set logical size: %s\n", SDL_GetError());
    return;
  }
}

void Graphics::fillRect(int x, int y, int w, int h, Color c) {
  SDL_Rect rect{ x, y, w, h };
  SDL_SetRenderDrawColor(internal->renderer, c.r, c.g, c.b, c.a);
  SDL_RenderFillRect(internal->renderer, &rect);
}

Texture& Graphics::createTexture(const std::string& filename) {
  //textures.push_back(std::make_unique<Texture>(filename, this));
  return *textures.back().get();
}

void Graphics::drawSprite(const Sprite& sprite, int x, int y, int w, int h) {
  Rect rect{ x, y, w, h };
  //sprite.draw(this, rect);
}

void Graphics::drawChar(char c, int x, int y, int w, int h) {
  std::string text;
  text += c;
  drawText(text, x, y, w, h);
}

void Graphics::drawText(const std::string& text, int x, int y, int w, int h) {
  if (!global_font) {
    fprintf(stderr, "Font was not initialized. Check logs.\n");
    exit(1);
  }
  SDL_Surface *surface;
  SDL_Texture *texture;
  SDL_Color textColor = { 0xFF, 0xFF, 0xFF, 0 };

  surface = TTF_RenderText_Solid(global_font, text.c_str(), textColor);
  texture = SDL_CreateTextureFromSurface(internal->renderer, surface);
  SDL_FreeSurface(surface);

  SDL_Rect dst{ x, y, w, h };
  SDL_RenderCopy(internal->renderer, texture, NULL, &dst);
  SDL_DestroyTexture(texture);
}
