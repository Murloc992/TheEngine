// #include "Precomp.h"

#include "GUITitlebar.h"
#include "GUIEnvironment.h"
#include "opengl/geometry/Quad.h"

GUITitlebar::GUITitlebar(GUIEnvironment* env, Rect2D<int> dimensions, bool draw)
  : GUIElement(env, dimensions) {
  this->Type = GUIET_PANE;
  environment = env;

  m_draw = draw;

  absolute_rect = dimensions;
  relative_rect = absolute_rect;
}

GUITitlebar::~GUITitlebar() {
}

void GUITitlebar::Render() {
  glEnable(GL_SCISSOR_TEST);
  glScissor(absolute_rect.x, environment->GetAbsoluteRect().h - (absolute_rect.y + absolute_rect.h), absolute_rect.w, absolute_rect.h);

  if (this->m_draw) {
    environment->DrawSlicedGUIQuad(absolute_rect, gui_skin_titlebar);
  }

  this->RenderChildren();

  glDisable(GL_SCISSOR_TEST);
}