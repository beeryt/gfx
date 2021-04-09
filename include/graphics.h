#pragma once
#include "window.h"
#include <functional>

class Graphics
{
public:
  template <typename...Args>
  static Window CreateWindow(Args...args) { return Window{ args... }; }

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
};

