// #include "Precomp.h"

#include "GUIIcon.h"
#include "GUIEnvironment.h"
#include "opengl/geometry/Quad.h"

GUIIcon::GUIIcon(GUIEnvironment* env, Rect2D<int> dimensions, uint32_t style, bool draw)
  : GUIElement(env, dimensions) {
  this->Type = GUIET_IMAGE;
  environment = env;

  m_draw = draw;
  m_icon_style = style;

  absolute_rect = dimensions;
  relative_rect = absolute_rect;
}

GUIIcon::~GUIIcon() {
}

void GUIIcon::Render() {
  if (this->m_draw) {
    environment->DrawGUIQuad(absolute_rect, m_icon_style, false);
  }

  this->RenderChildren();
}