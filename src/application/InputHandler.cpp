// #include "Precomp.h"
#include "InputHandler.h"
#include "Window.h"

InputHandler::InputHandler(ApplicationWindow* win) {
  // ctor
  for (int32_t i = 0; i < GLFW_KEY_LAST; i++)
    keys[i] = false;
  for (int32_t i = 0; i < GLFW_MOUSE_BUTTON_LAST; i++)
    mouseKeys[i] = false;

  _sig_mouse_button = win->SigMouseKey().connect(sigc::mem_fun(*this, &InputHandler::OnMouseButton));
  _sig_key = win->SigKeyEvent().connect(sigc::mem_fun(*this, &InputHandler::OnKey));
}

InputHandler::~InputHandler() {
  // dtor
}

bool InputHandler::IsKeyDown(uint32_t keyCode) {
  if (keyCode < 0 || keyCode > GLFW_KEY_LAST)
    return false;
  return keys[keyCode] == true;
}
bool InputHandler::IsKeyUp(uint32_t keyCode) {
  if (keyCode < 0 || keyCode > GLFW_KEY_LAST)
    return false;
  return keys[keyCode] == false;
}

bool InputHandler::IsMouseDown(uint32_t mouseKey) {
  if (mouseKey < 0 || mouseKey > GLFW_MOUSE_BUTTON_LAST)
    return false;
  return mouseKeys[mouseKey] == true;
}
bool InputHandler::IsMouseUp(uint32_t mouseKey) {
  if (mouseKey < 0 || mouseKey > GLFW_MOUSE_BUTTON_LAST)
    return false;
  return mouseKeys[mouseKey] == true;
}

void InputHandler::OnMouseButton(int32_t button, int32_t action, int32_t mod) {
  if (button < 0 || button > GLFW_MOUSE_BUTTON_LAST)
    return;

  switch (action) {
    case GLFW_PRESS:
      mouseKeys[button] = true;
      break;
    case GLFW_RELEASE:
      mouseKeys[button] = false;
      break;
    default:
      mouseKeys[button] = false;
      break;
  }
}

void InputHandler::OnKey(int32_t key, int32_t scanCode, int32_t action, int32_t mod) {
  if (key < 0 || key > GLFW_KEY_LAST)
    return;

  switch (action) {
    case GLFW_PRESS:
      keys[key] = true;
      break;
    case GLFW_RELEASE:
      keys[key] = false;
      break;
    case GLFW_REPEAT:
      keys[key] = true;
      break;
    default:
      keys[key] = false;
      break;
  }
}