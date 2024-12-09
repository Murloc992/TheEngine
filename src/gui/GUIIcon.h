#pragma once

#include "gui/GUIElement.h"

template <typename T>
class Rect2D;
class GUIIcon : public GUIElement {
 private:
  bool m_draw;
  uint32_t m_icon_style;

 public:
  GUIIcon(GUIEnvironment* env, Rect2D<int> dimensions, uint32_t style, bool draw = true);
  virtual ~GUIIcon();

  void Render();

  // this does not do any events
  void OnEvent(GUIEvent e) {
    if (this->event_listener) {
      event_listener->OnEvent(e);
      return;
    }
  }

 protected:
};