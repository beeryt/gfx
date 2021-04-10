#pragma once
#include "window.h"
#include <functional>

class Graphics
{
public:
  template <typename...Args>
#ifdef USE_SDL
  static std::shared_ptr<Window> CreateWindow(Args...args) {
    auto window = std::make_shared<Window>(args...);
    windows.push_back(window);
    return window;
  }
#else
  static std::shared_ptr<Window> CreateWindow(Args...args) { return std::make_shared<Window>(args...); }
#endif

  static void HandleEvents(double timeout = 0.0);
  static void PostEmptyEvent();

  static std::string GetClipboard();
  static void SetClipboard(const std::string& string);

  using ErrorCallbackFn = std::function<void(int, const char*)>;
  static void SetErrorCallback(ErrorCallbackFn callback);

private:
  Graphics();
  ~Graphics();
  friend class Window;
  static std::weak_ptr<Graphics> instance;
  static ErrorCallbackFn error_callback;
#ifdef USE_SDL
  static std::vector<std::weak_ptr<Window>> windows;
#endif
};

