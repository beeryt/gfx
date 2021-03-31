#pragma once
#include <string>
#include <memory>
#include <glm/glm.hpp>

struct Rect {
  int x,y,w,h;
};

class Texture {
  public:
    /// @brief Default constructor for a Texture.
    Texture();
    /// @brief Create a Texture from file.
    /// @param filename The filename of a desired image.
    Texture(const std::string& filename);

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
    glm::vec2 getSize() const;
    /// @brief Gets the width of this Texture.
    /// @returns the width of this Texture.
    int width() const;
    /// @brief Gets the height of this Texture.
    /// @returns the height of this Texture.
    int height() const;

  private:
    struct Internal;
    std::unique_ptr<Internal> internal;

    /// There is no copy constructor for a Texture.
    Texture(const Texture&) = delete;
    /// There is no copy assignment operator for a Texture.
    Texture& operator=(const Texture&) = delete;
};

