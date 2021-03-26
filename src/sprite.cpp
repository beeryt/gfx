#include "sprite.h"
#include <cassert>

Sprite::Sprite(Texture* texture) : texture(texture) {
  if (texture) {
    region = texture->getRegion();
  }
}

Sprite::Sprite(Sprite& sprite, unsigned frame) : Sprite(sprite) {
  setFrame(frame);
}

void Sprite::draw(SDL_Renderer* renderer, SDL_Rect dst) const {
  int w = region.w / hframes;
  int h = region.h / vframes;
  SDL_Rect src{
    static_cast<int>(region.x + (w * (frame % hframes))),
    static_cast<int>(region.y + (h * (frame / hframes))),
    w, h
  };
  if (0 != SDL_RenderCopy(renderer, texture->get(), &src, &dst)) {
    fprintf(stderr, "SDL_RenderCopy() failed: %s\n", SDL_GetError());
    exit(1);
  }
}

SDL_Rect Sprite::getRegion() const { return region; }
void Sprite::setRegion(SDL_Rect r) {
  assert(r.x >= 0 && r.x < texture->width());
  assert(r.y >= 0 && r.y < texture->height());
  assert(r.w > 0 && r.w <= (texture->width() - r.x));
  assert(r.h > 0 && r.h <= (texture->height() - r.y));
  region = r;
}

void Sprite::setFrame(unsigned f) {
  assert(f < ((hframes+1)*(vframes+1)));
  frame = f;
}

void Sprite::setHFrames(unsigned h) { assert(h > 0); hframes = h; }

void Sprite::setVFrames(unsigned v) { assert(v > 0); vframes = v; }

