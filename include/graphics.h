#pragma once
#include "window.h"
#include "shader.h"
#include "texture.h"
#include "mesh.h"
#include <functional>
#include <vector>

class Graphics {
  public:
    using ErrorCallbackFn = std::function<void(int, const char*)>;
    static void SetErrorCallback(ErrorCallbackFn);

    template <class... Args>
    static std::shared_ptr<Window> CreateWindow(Args...args) { return std::make_shared<Window>(args...); }

    static void Initialize() { Instance(); }

  private:
    Graphics();
    ~Graphics();

    static Graphics& Instance();

    ErrorCallbackFn error_callback;
    static void HandleError(int error_code, const char* description);
};
