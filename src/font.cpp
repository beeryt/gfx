#include "font.h"
#include <SDL.h>
#include <SDL_ttf.h>

/// @todo Provide interface for selecting font color.
const SDL_Color default_color{ 0xFF, 0xFF, 0xFF, 0 };
/// @todo Provide interface for setting font size.
const int default_size = 72;
Font default_font;

struct _Font {
  TTF_Font* font;
};

Font::Font() : Font("") {}
Font::Font(const std::string& filename) {
  internal = new _Font{ nullptr };
  if (filename.length()) {
    internal->font = TTF_OpenFont(filename.c_str(), default_size);
    if (!internal->font) {
      fprintf(stderr, "TTF_OpenFont() failed: %s\n", TTF_GetError());
      exit(1);
    }
  }
}

Font::~Font() {}

Font::Font(Font&& move) noexcept {
  *this = std::move(move);
}

Font& Font::operator=(Font&& move) noexcept {
  internal = move.internal;
  move.internal = nullptr;
  return *this;
}

Texture& Font::render(SDL_Renderer* renderer, const std::string& text) {
  if (cache.count(text)) { return *cache[text]; }
  else if (!internal->font) {
    fprintf(stderr, "Font::render failed: No internal font loaded.\n");
    exit(1);
  } else {
    SDL_Surface* surface = TTF_RenderText_Solid(internal->font, text.c_str(), default_color);
    if (!surface) {
      fprintf(stderr, "TTF_RenderText_Solid() failed: %s\n", TTF_GetError());
      exit(1);
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!texture) {
      fprintf(stderr, "SDL_CreateTextureFromSurface() failed: %s\n", SDL_GetError());
      exit(1);
    }

    cache[text] = std::make_unique<Texture>(texture);
    return *cache[text];
  }
}