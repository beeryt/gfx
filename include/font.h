#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include "texture.h"

struct _Font;
struct SDL_Renderer;

class Font {
  public:
    Font();
    Font(const std::string& filename);
    virtual ~Font();

    Font(Font&& move) noexcept;
    Font& operator=(Font&& move) noexcept;

    Texture& render(SDL_Renderer* renderer, const std::string& text);

  private:
    _Font* internal = nullptr;
    std::unordered_map<std::string, std::unique_ptr<Texture>> cache;

    Font(const Font&) = delete;
    Font& operator=(const Font&) = delete;
};
