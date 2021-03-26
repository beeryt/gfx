#include "texture.h"
#include <SDL_image.h>

Texture::Texture(SDL_Texture* texture) : texture(texture) {}

Texture::Texture(const std::string& filename, SDL_Renderer* renderer) {
  SDL_Surface* surface = IMG_Load(filename.c_str());
  if (!surface) {
    fprintf(stderr, "IMG_Load() failed: %s\n", IMG_GetError());
    exit(1);
  }

  texture = SDL_CreateTextureFromSurface(renderer, surface);
  if (!texture) {
    fprintf(stderr, "SDL_CreateTextureFromSurface() failed: %s\n", SDL_GetError());
    exit(1);
  }

  printf("%p %s\n", this, __PRETTY_FUNCTION__);
  SDL_FreeSurface(surface);
}

Texture::Texture(Texture&& o) noexcept {
  texture = o.texture;
  o.texture = nullptr;
}

Texture::~Texture() {
  if (texture) {
    SDL_DestroyTexture(texture);
    printf("%p %s DESTROYED\n", this, __PRETTY_FUNCTION__);
  }
  else printf("%p %s\n", this, __PRETTY_FUNCTION__);

}

SDL_Texture* Texture::get() { return texture; }

Vec2 Texture::getSize() const {
  Vec2 size;
  if (0 != SDL_QueryTexture(texture, NULL, NULL, &size.x, &size.y)) {
    fprintf(stderr, "SDL_QueryTexture() failed: %s\n", SDL_GetError());
    exit(1);
  }
  return size;
}

int Texture::width() const { return getSize().x; }

int Texture::height() const { return getSize().y; }

