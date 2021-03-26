#pragma once
#include <SDL.h>
#include "texture.h"
#include "sprite.h"

#include <functional>
#include <memory>
#include <string>
#include <vector>

/// A simple Color struct for 8-bit RGBA
struct Color {
  uint8_t r; ///< Red color channel.
  uint8_t g; ///< Green color channel.
  uint8_t b; ///< Blue color channel.
  uint8_t a; ///< Alpha color channel.

  /// @brief Basic constructor for 8-bit RGB or RGBA colors.
  /// @param r Red color channel.
  /// @param g Green color channel.
  /// @param b Blue color channel.
  /// @param a Alpha color channel.
  Color (uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xFF);
};

namespace colors {
  const Color black{ 0, 0, 0, 0xFF };
}

/// Graphics is a graphical window interface
class Graphics {
  public:
    /// @brief Default constructor for a Graphics window.
    /// @param title The window title.
    /// @param w The window width.
    /// @param h The window height.
    /// @todo Figure out if the dimensions include the border.
    Graphics(const char* title = "", int w = 320, int h = 240);

    /// @brief A MotionCallback is called whenever cursor motion is detected.
    /// @param x The x-coordinate of the cursor position.
    /// @param y The y-coordinate of the cursor position.
    using MotionCallback = std::function<void(int x,int y)>;
    /// @brief Sets the motion callback for the window.
    /// @param cb The motion callback to set.
    void setMotionCallback(MotionCallback cb);
    /// @brief Gets the motion callback for the window.
    /// @returns the motion callback for the window.
    MotionCallback getMotionCallback() const;

    /// @brief A TouchCallback is called whenever left mouse button is pressed.
    /// @param x The x-coordinate of the cursor position.
    /// @param y The y-coordinate of the cursor position.
    using TouchCallback = std::function<void(int x, int y)>;
    /// @brief Sets the touch callback for the window.
    /// @param cb The touch callback to set.
    void setTouchCallback(TouchCallback cb);
    /// @brief Gets the touch callback for the window.
    /// @returns the touch callback for the window.
    TouchCallback getTouchCallback() const;

    /// @brief A ReleaseCallback is called whenever left mouse button is released.
    /// @param x The x-coordinate of the cursor position.
    /// @param y The y-coordinate of the cursor position.
    using ReleaseCallback = std::function<void(int x,int y)>;
    /// @brief Sets the release callback for the window.
    /// @param cb The release callback to set.
    void setReleaseCallback(ReleaseCallback cb);
    /// @brief Gets the release callback for the window.
    /// @returns the release callback for the window.
    ReleaseCallback getReleaseCallback() const;

    /// @brief Sets the logical pixel size of the window.
    /// @param w Logical width of window.
    /// @param h Logical height of window.
    void setLogical(int w, int h);
    /// @brief Gets the logical pixel size of the window.
    /// @param w Logical width of window (nullptr is ignored).
    /// @param h Logical height of window (nullptr is ignored).
    void getLogical(int*w, int*h);

    /// @brief Creates a texture from a file with this object's renderer.
    /// @param filename The filename of a desired image.
    /// @returns A Texture reference.
    Texture& createTexture(const std::string&);

    /// @brief Handles internal event loop.
    /// @returns false if window should close, true otherwise.
    bool loop();
    /// Clears the screen.
    /// @param c The draw color for clearing the screen.
    void clear(Color c = colors::black);
    /// Swaps the internal buffer to display screen.
    void swap();

    /// @brief Draws a rectangle outline to the screen.
    /// @param x The x-coordinate of the top-left corner.
    /// @param y The y-coordinate of the top-left corner.
    /// @param w The horizontal width of the rectangle.
    /// @param h The vertical height of the rectangle.
    /// @param c The Color of the outline.
    void drawRect(int x, int y, int w, int h, Color c);
    /// @brief Draws a rectangle (filled) to the screen.
    /// @param x The x-coordinate of the top-left corner.
    /// @param y The y-coordinate of the top-left corner.
    /// @param w The horizontal width of the rectangle.
    /// @param h The vertical height of the rectangle.
    /// @param c The Color of the rectangle.
    void fillRect(int x, int y, int w, int h, Color c);
    /// @brief Draws a Sprite on the screen.
    /// @param sprite The Sprite to draw.
    /// @param x The x-coordinate of the top-left corner.
    /// @param y The y-coordinate of the top-left corner.
    /// @param w The horizontal width of the Sprite to draw.
    /// @param h The vertical height of the Sprite to draw.
    void drawSprite(const Sprite&, int x, int y, int w, int h);
    /// @brief Draws a character on the screen.
    /// @param c The character to draw.
    /// @param x The x-coordinate of the top-left corner.
    /// @param y The y-coordinate of the top-left corner.
    /// @param w The horizontal width of the character.
    /// @param h The vertical height of the character.
    void drawChar(char c, int x, int y, int w, int h);
    /// @brief Draws text on the screen.
    /// @param text The text to draw.
    /// @param x The x-coordinate of the top-left corner.
    /// @param y The y-coordinate of the top-left corner.
    /// @param w The horizontal width of the text to draw.
    /// @param h The vertical height of the text to draw.
    void drawText(const std::string& text, int x, int y, int w, int h);

  private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    std::vector<std::unique_ptr<Texture>> textures;
    MotionCallback motionCallback;
    TouchCallback touchCallback;
    ReleaseCallback releaseCallback;
};

