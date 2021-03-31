#include "window.h"
#include <cassert>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

using std::string;

struct Window::Internal {
  GLFWwindow* window = nullptr;
};

Window::Window(const string& title, int width, int height, int hints) {
  spdlog::trace("{}", __PRETTY_FUNCTION__);
  internal = std::make_unique<Internal>();

  if (hints & VISIBLE) glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  internal->window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
  if (!internal->window) { fprintf(stderr, "glfwCreateWindow() failed\n"); exit(1); }

  glfwMakeContextCurrent(internal->window);

  int version = gladLoadGL(glfwGetProcAddress);
  spdlog::info("Loaded OpenGL {}.{}",
      GLAD_VERSION_MAJOR(version),
      GLAD_VERSION_MINOR(version));
}

Window::~Window() {
  spdlog::trace("{}", __PRETTY_FUNCTION__);
  if (internal->window) {
    glfwDestroyWindow(internal->window);
    internal->window = nullptr;
  }
}

bool Window::ShouldClose() const {
  assert(internal->window);
  return glfwWindowShouldClose(internal->window);
}

void Window::HandleEvents() {
  glfwWaitEventsTimeout(0);
}

void Window::SwapBuffers() {
  glfwSwapBuffers(internal->window);
}

