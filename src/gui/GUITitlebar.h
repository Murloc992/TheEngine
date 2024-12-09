#pragma once

#include "gui/GUIElement.h"

template <typename T>
class Rect2D;
class GUITitlebar : public GUIElement {
 private:
  bool m_draw;

 public:
  GUITitlebar(GUIEnvironment* env, Rect2D<int> dimensions, bool draw = true);
  virtual ~GUITitlebar();

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