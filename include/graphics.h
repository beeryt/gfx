#pragma once
#include "window.h"
#include "shader.h"
#include "texture.h"
#include <functional>
#include <vector>

class Graphics {
  public:
    using ErrorCallbackFn = std::function<void(int, const char*)>;
    static void SetErrorCallback(ErrorCallbackFn);
    static std::shared_ptr<Window> CreateWindow(const std::string& title, int width, int height);

  private:
    Graphics();
    ~Graphics();

    static Graphics& Instance();

    ErrorCallbackFn error_callback;
    static void HandleError(int error_code, const char* description);
    std::vector<std::shared_ptr<Window>> windows;
};
