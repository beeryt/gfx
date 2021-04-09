#pragma once
#include <string>
#include <memory>
#include <functional>
#include <spdlog/spdlog.h>

class Window
{
public:
  Window(int width, int height, const std::string& title);
  Window(Window&&) noexcept;
  virtual ~Window();

  bool ShouldClose() const;
  void Clear();
  void SwapBuffers();

  void Show();
  void Hide();
  void Iconify();
  void Restore();
  void Maximize();
  void Focus();
  void RequestAttention();

  void GetWindowPos(int&, int&) const;
  void GetWindowSize(int&, int&) const;
  void GetWindowOpacity(float&) const;
  float GetWindowOpacity() const;
  void GetWindowFrameSize(int&, int&, int&, int&) const;
  void GetFramebufferSize(int&, int&) const;
  void GetWindowContentScale(float&, float&) const;

  void SetWindowPos(int, int);
  void SetWindowSize(int, int);
  void SetWindowOpacity(float);
  void SetWindowAspectRatio(int, int);
  void SetWindowSizeLimits(int, int, int, int);
  void SetWindowTitle(const std::string&);
  void SetWindowIcon(/* TODO */);

  using PosCallback = std::function<void(Window&, int, int)>;
  using SizeCallback = std::function<void(Window&, int, int)>;
  using CloseCallback = std::function<void(Window&)>;
  using RefreshCallback = std::function<void(Window&)>;
  using FocusCallback = std::function<void(Window&, bool)>;
  using IconifyCallback = std::function<void(Window&, bool)>;
  using MaximizeCallback = std::function<void(Window&, bool)>;
  using FramebufferCallback = std::function<void(Window&, int, int)>;
  using ContentScaleCallback = std::function<void(Window&, float, float)>;

  PosCallback SetPositionCallback(PosCallback);
  SizeCallback SetSizeCallback(SizeCallback);
  CloseCallback SetCloseCallback(CloseCallback);
  RefreshCallback SetRefreshCallback(RefreshCallback);
  FocusCallback SetFocusCallback(FocusCallback);
  IconifyCallback SetIconifyCallback(IconifyCallback);
  MaximizeCallback SetMaximizeCallback(MaximizeCallback);
  FramebufferCallback SetFramebufferCallback(FramebufferCallback);
  ContentScaleCallback SetContentScaleCallback(ContentScaleCallback);

  enum class MouseButtonAction { PRESS, RELEASE };
  using MouseButtonCallback = std::function<void(Window&, int, MouseButtonAction, int)>;
  using CursorPosCallback = std::function<void(Window&, double, double)>;
  using CursorEnterCallback = std::function<void(Window&, bool)>;
  using ScrollCallback = std::function<void(Window&, double, double)>;
  using KeyCallback = std::function<void(Window&, int key, int scancode, int action, int mods)>;
  using CharCallback = std::function<void(Window&, unsigned int codepoint)>;
  using DropCallback = std::function<void(Window&, int path_count, const char* paths[])>;

  MouseButtonCallback SetMouseButtonCallback(MouseButtonCallback);
  CursorPosCallback SetCursorPosCallback(CursorPosCallback);
  CursorEnterCallback SetCursorEnterCallback(CursorEnterCallback);
  ScrollCallback SetScrollCallback(ScrollCallback);
  KeyCallback SetKeyCallback(KeyCallback);
  CharCallback SetCharCallback(CharCallback);
  DropCallback SetDropCallback(DropCallback);

  enum class InputMode { CURSOR, STICKY_KEYS, STICKY_MOUSE_BUTTONS, LOCK_KEY_MODS, RAW_MOUSE_MOTION };
  int GetInputMode(InputMode) const;
  void SetInputMode(InputMode, int value);
  int GetKey(int key) const;
  int GetMouseButton(int button) const;
  void GetCursorPos(double&, double&) const;
  void SetCursorPos(double, double);

  using ErrorCallback = std::function<void(const Window&, unsigned, spdlog::level::level_enum, const std::string&)>;
  static ErrorCallback SetErrorCallback(ErrorCallback callback);

private:
  Window(const Window&) = delete;
  Window& operator=(const Window&) = delete;
  struct Internal;
  std::unique_ptr<Internal> internal;
  static ErrorCallback error_callback;
};
