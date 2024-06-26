#pragma once

void KeyEvent(GLFWwindow* wnd, int32_t key, int32_t scan_code, int32_t action, int32_t modifiers);
void MouseMoveEvent(GLFWwindow* wnd, double x, double y);
void MouseKeyEvent(GLFWwindow* wnd, int32_t button, int32_t action, int32_t mod);
void MouseScrollEvent(GLFWwindow* wnd, double sx, double sy);
void ApplicationWindowResize(GLFWwindow* wnd, int32_t w, int32_t h);
void ApplicationWindowClose(GLFWwindow* wnd);
void TextEvent(GLFWwindow* wnd, uint32_t scan_code);

class ApplicationWindow {
 protected:
  GLFWwindow* _window;
  glm::ivec2 _window_size;

  sigc::signal<void(double, double)> _sigMouseMoved;
  sigc::signal<void(int32_t, int32_t, int32_t)> _sigMouseKey;
  sigc::signal<void(double, double)> _sigMouseScroll;
  sigc::signal<void(int32_t, int32_t)> _sigWindowResized;
  sigc::signal<void(int32_t, int32_t)> _sigFramebufferResized;
  sigc::signal<void()> _sigWindowClosed;
  sigc::signal<void(int32_t, int32_t, int32_t, int32_t)> _sigKeyEvent;
  sigc::signal<void(uint32_t)> _sigTextEvent;

  friend void MouseMoveEvent(GLFWwindow* wnd, double x, double y);
  friend void MouseKeyEvent(GLFWwindow* wnd, int32_t button, int32_t action, int32_t mod);
  friend void MouseScrollEvent(GLFWwindow* wnd, double sx, double sy);
  friend void WindowResize(GLFWwindow* wnd, int32_t x, int32_t y);
  friend void FramebufferResize(GLFWwindow* wnd, int32_t x, int32_t y);
  friend void WindowClose(GLFWwindow* wnd);
  friend void KeyEvent(GLFWwindow* wnd, int32_t key, int32_t scan_code, int32_t action, int32_t modifiers);
  friend void TextEvent(GLFWwindow* wnd, uint32_t scan_code);

  bool _shouldClose;

 public:  /// signals
  sigc::signal<void(double, double)>& SigMouseMoved();
  sigc::signal<void(int32_t, int32_t, int32_t)>& SigMouseKey();
  sigc::signal<void(double, double)>& SigMouseScroll();
  sigc::signal<void(int32_t, int32_t)>& SigWindowResized();
  sigc::signal<void(int32_t, int32_t)>& SigFramebufferResized();
  sigc::signal<void()>& SigWindowClosed();
  sigc::signal<void(int32_t, int32_t, int32_t, int32_t)>& SigKeyEvent();
  sigc::signal<void(uint32_t)>& SigTextEvent();

 public:
  static std::map<GLFWwindow*, ApplicationWindow*> _windows;
  static void DestroyWindow(ApplicationWindow* wnd);

  ApplicationWindow();
  virtual ~ApplicationWindow();

  bool Init(const std::string& title, uint32_t width, uint32_t height, bool fullscreen = false, bool windowed = true, bool vsync = false,
            uint32_t msaa = 0, uint32_t r = 8, uint32_t g = 8, uint32_t b = 8, uint32_t alpha = 8, uint32_t depth = 24, uint32_t stencil = 8);
  bool GetKey(uint32_t key);
  glm::vec2 GetMousePos();
  glm::ivec2 GetWindowSize();
  void SetWindowSize(uint32_t width, uint32_t height);
  void SetWindowMode(bool fullscreen, bool windowed);
  void SetWindowPosition(const glm::ivec2& pos);
  void SetMousePos(const glm::ivec2& pos);
  void SetCursorDisabled(bool disabled);
  GLFWwindow* GetWindow();

  bool Update();
  void SwapBuffers();
  bool GetShouldClose();
};
