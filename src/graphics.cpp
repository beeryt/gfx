#include "graphics.h"
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

Graphics::Graphics() {
  spdlog::trace("{}", __PRETTY_FUNCTION__);
  if (!glfwInit()) {
    fprintf(stderr, "glfwInit() failed\n");
    exit(1);
  }
  glfwSetErrorCallback(HandleError);

  int major = GLFW_VERSION_MAJOR;
  int minor = GLFW_VERSION_MINOR;
  int revision = GLFW_VERSION_REVISION;
  spdlog::debug("Compiled with GLFW {}.{}.{}", major, minor, revision);
  glfwGetVersion(&major, &minor, &revision);
  spdlog::info("Loaded GLFW {}.{}.{}", major, minor, revision);
}

Graphics::~Graphics() {
  spdlog::trace("{}", __PRETTY_FUNCTION__);
  glfwTerminate();
}

Graphics& Graphics::Instance() {
  static Graphics instance;
  return instance;
}

void Graphics::SetErrorCallback(ErrorCallbackFn cb) {
  Instance().error_callback = cb;
}

void Graphics::HandleError(int error_code, const char* description) {
  auto& callback = Instance().error_callback;
  if (callback) callback(error_code, description);
}
