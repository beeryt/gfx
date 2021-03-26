#pragma once
#include "texture.h"
#include <SDL.h>

/// A Sprite is drawable region of a Texture.
/// @todo Add a Texture::createSprite method.
class Sprite {
  public:
    /// @brief Default constructor for a Sprite.
    /// @param texture Optional texture for initialization.
    Sprite(Texture* texture = nullptr);
    /// @brief Copy constructor with frame offset.
    /// @param copy The Sprite to copy.
    /// @param frame The frame offset to use.
    Sprite(Sprite& copy, unsigned);
    virtual ~Sprite() = default;

    /// @brief Draws this sprite with the provided renderer.
    /// @param renderer The renderer to use for drawing.
    /// @param dst A destination rectangle for this sprite.
    /// @todo Consider need for supporting `SDL_Rect*` for fullscreen drawing.
    void draw(SDL_Renderer*, SDL_Rect dst) const;

    /// @brief Sets the Texture region for this sprite.
    /// @param region The region to set (must fit within the texture).
    void setRegion(SDL_Rect region);
    /// @brief Gets the Texture region for this sprite.
    /// @returns the Texture region for this sprite.
    SDL_Rect getRegion() const;

    /// @brief Sets the frame index for this sprite.
    /// @param idx The frame index for this sprite (must be less than hframes * vframes).
    void setFrame(unsigned idx);
    /// @brief Gets the frame index for this sprite.
    /// @returns the frame index for this sprite.
    unsigned getFrame() const;

    /// @brief Sets the hframes value for this sprite.
    /// @param h The hframes value to set (must be greater than zero).
    void setHFrames(unsigned h);
    /// @brief Gets the hframes value for this sprite.
    /// @returns the hframes value for this sprite.
    unsigned getHFrames() const;

    /// @brief Sets the vframes value for this sprite.
    /// @param v The vframes value to set (must be greater than zero).
    void setVFrames(unsigned v);
    /// @brief Gets the vframes value for this sprite.
    /// @returns the vframes value for this sprite.
    unsigned getVFrames() const;

  private:
    Texture* texture;
    SDL_Rect region;
    unsigned frame = 0;
    unsigned hframes = 1;
    unsigned vframes = 1;
};

