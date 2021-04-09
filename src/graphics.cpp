#include "graphics.h"
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>
#include <stdexcept>

std::weak_ptr<Graphics> Graphics::instance;

Graphics::ErrorCallbackFn Graphics::error_callback = [](int error, const char* description) noexcept { fprintf(stderr, "ERROR (%04x): %s\n", error, description); };

void Graphics::SetErrorCallback(ErrorCallbackFn callback) { error_callback = callback; }

void Graphics::HandleEvents(double timeout) {
  if (instance.lock()) glfwWaitEventsTimeout(timeout);
}

void Graphics::SetClipboard(const std::string& string)
{
  if (instance.lock()) glfwSetClipboardString(nullptr, string.c_str());
}

std::string Graphics::GetClipboard()
{
  if (instance.lock()) return glfwGetClipboardString(nullptr); else return "";
}

Graphics::Graphics()
{
  spdlog::trace("{}", __FUNCSIG__);
  // initialize the framework
  if (!glfwInit()) { throw std::runtime_error("glfwInit() failed"); }

  // set a default error callback
  glfwSetErrorCallback([](int error, const char* description) { if (error_callback) error_callback(error, description); });
  
  // log the version
  int major = GLFW_VERSION_MAJOR;
  int minor = GLFW_VERSION_MINOR;
  int revision = GLFW_VERSION_REVISION;
  spdlog::info("Compiled with GLFW {}.{}.{}", major, minor, revision);
  glfwGetVersion(&major, &minor, &revision);
  spdlog::info("Loaded GLFW {}.{}.{}", major, minor, revision);
}

Graphics::~Graphics() { spdlog::trace("{}", __FUNCSIG__); glfwTerminate(); }
