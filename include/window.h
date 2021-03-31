#pragma once
#include <string>
#include <memory>

class Window {
  public:
    enum Hint {
      VISIBLE = 0x01
    };
    constexpr static Hint default_hint = Hint::VISIBLE;
    Window(const std::string& title, int width, int height, int hint = default_hint);
    virtual ~Window();

    bool ShouldClose() const;
    void HandleEvents();
    void SwapBuffers();

  private:
    Window(const Window&) = delete;

    struct Internal;
    std::unique_ptr<Internal> internal;
};
