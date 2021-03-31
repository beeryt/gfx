#pragma once
#include <string>

// forward declaration of Graphics
class Graphics;

struct Vec2 {
  int x;
  int y;
};

struct Rect {
  int x,y,w,h;
};

/// A Texture is a wrapper around an SDL_Texture
class Texture {
  public:
    /// @brief Default constructor for a Texture.
    Texture();
    /// @brief Create a Texture from filename using renderer.
    /// @param filename The filename of a desired image.
    /// @param renderer The renderer which will be used for this texture.
    Texture(const std::string& filename, Graphics* renderer);

    virtual ~Texture();

    /// @brief Move constructor for a Texture.
    /// @param move The Texture object being moved.
    Texture(Texture&& move) noexcept;
    /// @brief Move assignment operator for a Texture.
    /// @param move The Texture object being moved.
    /// @returns A new Texture containing the moved contents.
    Texture& operator=(Texture&& move) noexcept;

    /// @brief Gets the size of this Texture.
    /// @returns the width and height of this Texture.
    Vec2 getSize() const;
    /// @brief Gets the width of this Texture.
    /// @returns the width of this Texture.
    int width() const;
    /// @brief Gets the height of this Texture.
    /// @returns the height of this Texture.
    int height() const;

    struct Internal;
    /// @brief Gets the internal texture data.
    /// @returns the internal SDL_Texture.
    const Internal* get();

  private:
    Internal* internal;

    /// There is no copy constructor for a Texture.
    Texture(const Texture&) = delete;
    /// There is no copy assignment operator for a Texture.
    Texture& operator=(const Texture&) = delete;
};

