#include "window.h"
#include "graphics.h"

#include <cassert>
#include <stdexcept>

#include <glad/gl.h>
#ifdef USE_SDL
#include <SDL.h>
#else
#include <GLFW/glfw3.h>
#endif
#include <spdlog/spdlog.h>

struct Window::Internal
{
#ifdef USE_SDL
  SDL_Window* window = nullptr;
  SDL_GLContext context;
  bool should_close = false;
#else
  GLFWwindow* window = nullptr;
#endif
  std::shared_ptr<Graphics> engine = nullptr;
};

Window::ErrorCallback Window::error_callback = [](const Window&, unsigned id, spdlog::level::level_enum level, const std::string& message) {
  spdlog::log(level, "{}: {}", id, message);
};

Window::Window(int width, int height, const std::string& title) : internal(std::make_unique<Internal>())
{
  spdlog::trace("{}", __FUNCTION__);

  // initialize graphics engine instance
  if (Graphics::instance.use_count() == 0) {
    internal->engine = std::shared_ptr<Graphics>(new Graphics, [](Graphics* p) { delete p; }); // custom deleter because Graphics::~Graphics is private
    Graphics::instance = internal->engine;
  }
  else {
    internal->engine = Graphics::instance.lock();
  }

  // set window hints
#ifndef USE_SDL
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

  // create window
#ifdef USE_SDL
  internal->window = SDL_CreateWindow(title.c_str(),
      SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
      width, height,
      SDL_WINDOW_OPENGL);
  if (!internal->window) {
    char buf[256];
    snprintf(buf, sizeof(buf), "SDL_CreateWindow() failed: %s\n", SDL_GetError());
    throw std::runtime_error(buf);
  }
  SDL_SetWindowData(internal->window, "class", this);
#else
  internal->window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
  if (!internal->window) { throw std::runtime_error("glfwCreateWindow() failed"); }
  glfwSetWindowUserPointer(internal->window, this);
#endif

  // prepare OpenGL context
#ifdef USE_SDL
  internal->context = SDL_GL_CreateContext(internal->window);
  int version = gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress);
#else
  glfwMakeContextCurrent(internal->window);
  int version = gladLoadGL(glfwGetProcAddress);
#endif
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
  spdlog::trace("{}", __FUNCTION__);
#ifdef USE_SDL
  if (internal->context) { SDL_GL_DeleteContext(internal->context); }
  if (internal->window) { SDL_DestroyWindow(internal->window); }
#else
  if (internal->window) { glfwDestroyWindow(internal->window); }
#endif
}

bool Window::ShouldClose() const
{
#ifdef USE_SDL
  return internal->should_close;
#else
  return GLFW_TRUE == glfwWindowShouldClose(internal->window);
#endif
}

void Window::ShouldClose(bool should_close)
{
#ifdef USE_SDL
  internal->should_close = should_close;
#else
  glfwSetWindowShouldClose(internal->window, should_close);
#endif
}

void Window::Clear() { glClear(GL_COLOR_BUFFER_BIT); }

void Window::SwapBuffers()
{
#ifdef USE_SDL
  SDL_GL_SwapWindow(internal->window);
#else
  glfwSwapBuffers(internal->window);
#endif
}
