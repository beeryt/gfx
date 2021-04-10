#include "graphics.h"
#ifdef USE_SDL
#include <SDL.h>
#else
#include <GLFW/glfw3.h>
#endif
#include <spdlog/spdlog.h>
#include <stdexcept>

std::weak_ptr<Graphics> Graphics::instance;
#ifdef USE_SDL
std::vector<std::weak_ptr<Window>> Graphics::windows;
#endif

Graphics::ErrorCallbackFn Graphics::error_callback = [](int error, const char* description) noexcept { fprintf(stderr, "ERROR (%04x): %s\n", error, description); };

void Graphics::SetErrorCallback(ErrorCallbackFn callback) { error_callback = callback; }

void Graphics::HandleEvents(double timeout) {
#ifdef USE_SDL
  if (auto lock = instance.lock()) {
    if (!SDL_WaitEventTimeout(NULL, timeout * 1000)) return;
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        // dispatch to all windows
        for (auto& window : windows) {
          if (auto w = window.lock()) {
            w->ShouldClose(true);
          }
        }
      }
      if (e.type == SDL_WINDOWEVENT) {
        // dispatch to window
        SDL_Window *window = SDL_GetWindowFromID(e.window.windowID);
        Window& w = *static_cast<Window*>(SDL_GetWindowData(window, "class"));
        switch (e.window.event)
        {
          case SDL_WINDOWEVENT_CLOSE: w.ShouldClose(true); break;
        }
      }
    }
  }
#else
  if (instance.lock()) glfwWaitEventsTimeout(timeout);
#endif
}

void Graphics::SetClipboard(const std::string& string)
{
#ifdef USE_SDL
  if (0 != SDL_SetClipboardText(string.c_str())) {
    spdlog::error("SDL_SetClipboardText() failed: {}", SDL_GetError());
  }
#else
  if (instance.lock()) glfwSetClipboardString(nullptr, string.c_str());
#endif
}

std::string Graphics::GetClipboard()
{
#ifdef USE_SDL
  auto str = SDL_GetClipboardText();
  std::string out;
  if (!str) { spdlog::error("SDL_GetClipboardText() failed: {}", SDL_GetError()); }
  else { out = str; SDL_free(str); }
  return out;
#else
  if (instance.lock()) return glfwGetClipboardString(nullptr); else return "";
#endif
}

Graphics::Graphics()
{
  spdlog::trace("{}", __FUNCTION__);
  // initialize the framework
#ifdef USE_SDL
  if (0 != SDL_Init(SDL_INIT_VIDEO)) {
    char buf[256];
    snprintf(buf, sizeof(buf), "SDL_Init() failed: %s\n", SDL_GetError());
    throw std::runtime_error(buf);
  }
#else
  if (!glfwInit()) { throw std::runtime_error("glfwInit() failed"); }
#endif

  // set a default error callback
#ifndef USE_SDL
  glfwSetErrorCallback([](int error, const char* description) { if (error_callback) error_callback(error, description); });
#endif

  // log the version
#ifdef USE_SDL
  SDL_version version;
  SDL_VERSION(&version);
  spdlog::info("Compiled with SDL {}.{}.{}", version.major, version.minor, version.patch);
  SDL_GetVersion(&version);
  spdlog::info("Loaded SDL {}.{}.{}", version.major, version.minor, version.patch);
#else
  int major = GLFW_VERSION_MAJOR;
  int minor = GLFW_VERSION_MINOR;
  int revision = GLFW_VERSION_REVISION;
  spdlog::info("Compiled with GLFW {}.{}.{}", major, minor, revision);
  glfwGetVersion(&major, &minor, &revision);
  spdlog::info("Loaded GLFW {}.{}.{}", major, minor, revision);
#endif
}

Graphics::~Graphics()
{
  spdlog::trace("{}", __FUNCTION__);
#ifdef USE_SDL
    SDL_Quit();
#else
  glfwTerminate();
#endif
}
