#include "window.h"
#include "graphics.h"

#include <cassert>
#include <stdexcept>

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

struct Window::Internal
{
  GLFWwindow* window = nullptr;
  std::shared_ptr<Graphics> engine = nullptr;
};

Window::ErrorCallback Window::error_callback = [](const Window&, unsigned id, spdlog::level::level_enum level, const std::string& message) {
  spdlog::log(level, "{}: {}", id, message);
};

Window::Window(int width, int height, const std::string& title) : internal(std::make_unique<Internal>())
{
  spdlog::trace("{}", __FUNCSIG__);

  // initialize graphics engine instance
  if (Graphics::instance.use_count() == 0) {
    internal->engine = std::shared_ptr<Graphics>(new Graphics, [](Graphics* p) { delete p; }); // custom deleter because Graphics::~Graphics is private
    Graphics::instance = internal->engine;
  }
  else {
    internal->engine = Graphics::instance.lock();
  }

  // set window hints
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // create window
  internal->window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
  if (!internal->window) { throw std::runtime_error("glfwCreateWindow() failed"); }
  glfwSetWindowUserPointer(internal->window, this);

  // prepare OpenGL context
  glfwMakeContextCurrent(internal->window);
  int version = gladLoadGL(glfwGetProcAddress);
  spdlog::info("Loaded OpenGL {}.{}", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));

  glEnable(GL_DEBUG_OUTPUT);
  auto handler = [](
      GLenum,
      GLenum,
      GLuint id,
      GLenum severity,
      GLsizei length,
      const GLchar* msg,
      const void* userParam)
  {
    std::string message{ msg, static_cast<size_t>(length) };
    const Window& ref = *static_cast<const Window*>(userParam);
    spdlog::level::level_enum level = spdlog::level::critical;
    switch (severity) {
      case GL_DEBUG_SEVERITY_HIGH:          level = spdlog::level::err; break;
      case GL_DEBUG_SEVERITY_MEDIUM:        level = spdlog::level::warn; break;
      case GL_DEBUG_SEVERITY_LOW:           level = spdlog::level::info; break;
      case GL_DEBUG_SEVERITY_NOTIFICATION:  level = spdlog::level::debug; break;
    }
    if (error_callback) error_callback(ref, id, level, message);
  };
  glDebugMessageCallback(handler, this);
}

Window::Window(Window&& move) noexcept : internal(std::move(move.internal)) {}

Window::~Window() {
  spdlog::trace("{}", __FUNCSIG__);
  if(internal->window) {glfwDestroyWindow(internal->window); }
}

bool Window::ShouldClose() const { return GLFW_TRUE == glfwWindowShouldClose(internal->window); }

void Window::SwapBuffers() { glfwSwapBuffers(internal->window); }
