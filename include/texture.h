#pragma once
#include <SDL.h>
#include <string>

/// A Texture is a wrapper around an SDL_Texture
class Texture {
  public:
    /// @brief Default constructor for a Texture.
    /// @param texture Optional texture for initialization.
    Texture(SDL_Texture* texture = nullptr);
    /// @brief Create a Texture from filename using renderer.
    /// @param filename The filename of a desired image.
    /// @param renderer The renderer which will be used for this texture.
    Texture(const std::string& filename, SDL_Renderer* renderer);

    virtual ~Texture();

    /// @brief Move constructor for a Texture.
    /// @param move The Texture object being moved.
    Texture(Texture&& move) noexcept;
    /// @brief Move assignment operator for a Texture.
    /// @param move The Texture object being moved.
    /// @returns A new Texture containing the moved contents.
    Texture& operator=(Texture&& move) noexcept;

    /// @brief Gets the size of this Texture.
    /// @todo Should return a Vec2.
    /// @todo Rename to getSize().
    /// @brief Gets the width and height of this Texture.
    /// @returns the width and height of this Texture.
    SDL_Rect getRegion() const;
    /// @brief Gets the width of this Texture.
    /// @returns the width of this Texture.
    int width() const;
    /// @brief Gets the height of this Texture.
    /// @returns the height of this Texture.
    int height() const;

    /// @todo Consider removing this dangerous method.
    /// @brief Gets the internal SDL_Texture (plz don't delete).
    /// @returns the internal SDL_Texture.
    SDL_Texture* get();

  private:
    SDL_Texture* texture;

    /// There is no copy constructor for a Texture.
    Texture(const Texture&) = delete;
    /// There is no copy assignment operator for a Texture.
    Texture& operator=(const Texture&) = delete;
};

