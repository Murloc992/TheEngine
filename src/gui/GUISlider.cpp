// #include "Precomp.h"

#include "GUISlider.h"
#include "GUIEnvironment.h"
#include "GUIEvent.h"
#include "GUIStaticText.h"
#include "opengl/geometry/Quad.h"
#include "utility/Util.h"

GUISlider::GUISlider(GUIEnvironment* env, Rect2D<int> dimensions, float min, float max, float pos, bool drawValue, bool snap, bool wide,
                     bool vertical, bool dark)
  : GUIElement(env, dimensions) {
  this->Type = GUIET_SLIDER;
  environment = env;

  absolute_rect = dimensions;
  relative_rect = absolute_rect;

  m_min = min;
  m_max = max;
  m_cur_value = m_old_value = pos;
  if (vertical) {
    m_slider_pos = absolute_rect.y - ((pos - absolute_rect.h) / (max - min));
  } else {
    float f = (absolute_rect.w - 12) / range();
    m_slider_pos = (m_cur_value - m_min) * f + absolute_rect.h / 2;
  }
  if (drawValue) {
    m_valueText = new GUIStaticText(env, Rect2D<int>(0, 0, dimensions.w, 10), L"['s]val[s']", false);
    m_valueText->SetParent(this);
    m_valueText->SetListening(false);
    m_valueText->SetAlignment(HALIGN_CENTER, VALIGN_CENTER);
    update_value_text();
  }
  m_vertical = vertical;
  m_snap = snap;
  m_drawValue = drawValue;
  m_wide = wide;
  m_dark = dark;
}

GUISlider::~GUISlider() {
}

void GUISlider::Render() {
  if (m_vertical) {
    environment->DrawSlicedGUIQuad(Rect2D<int>(absolute_rect.x + absolute_rect.w / 4, absolute_rect.y, absolute_rect.w / 2, absolute_rect.h),
                                   m_dark ? gui_skin_scroll_v_bg : gui_skin_scroll_h_bg);
    this->RenderChildren();
    environment->DrawGUIQuad(Rect2D<int>(absolute_rect.x, absolute_rect.y + m_slider_pos - 6, absolute_rect.w, 12),
                             m_dark ? gui_skin_scroll_v_bar : gui_skin_scroll_h_bar);
  } else {
    if (m_wide) {
      environment->DrawSlicedGUIQuad(Rect2D<int>(absolute_rect.x, absolute_rect.y, absolute_rect.w, absolute_rect.h),
                                     m_dark ? gui_skin_scroll_v_bg : gui_skin_scroll_h_bg);
    } else {
      environment->DrawSlicedGUIQuad(Rect2D<int>(absolute_rect.x, absolute_rect.y + absolute_rect.h / 4, absolute_rect.w, absolute_rect.h / 2),
                                     m_dark ? gui_skin_scroll_v_bg : gui_skin_scroll_h_bg);
    }
    this->RenderChildren();
    environment->DrawSlicedGUIQuad(Rect2D<int>(absolute_rect.x + m_slider_pos - 6, absolute_rect.y, 12, absolute_rect.h),
                                   m_dark ? gui_skin_scroll_v_bar : gui_skin_scroll_h_bar);
  }
}

bool GUISlider::OnEvent(const GUIEvent& e) {
  GUI_BEGIN_ON_EVENT(e)

  switch (e.GetType()) {
    case left_mouse_pressed:
      handle_mouse();

      if (m_cur_value != m_old_value) {
        GUI_FIRE_EVENT(GUIEvent(scrollbar_changed, this, this))
        m_old_value = m_cur_value;
      }

      break;
    case mouse_dragged:
      handle_mouse();

      if (m_cur_value != m_old_value) {
        GUI_FIRE_EVENT(GUIEvent(scrollbar_changed, this, this))
        m_old_value = m_cur_value;
      }
      break;
    default:
      break;
  }

  GUI_END_ON_EVENT(e)
}

void GUISlider::handle_mouse() {
  if (m_vertical) {
    m_mouse_pos = environment->GetMousePosition();
    m_slider_pos = (int)(m_mouse_pos.y - absolute_rect.y);

    if (m_slider_pos >= absolute_rect.h)
      m_slider_pos = absolute_rect.h;
    if (m_slider_pos < 0)
      m_slider_pos = 0;

    m_cur_value = (range() * (absolute_rect.y - m_slider_pos)) / absolute_rect.h;
    m_cur_value = glm::clamp(m_cur_value, m_min, m_max);
  } else {
    m_mouse_pos = environment->GetMousePosition();
    m_slider_pos = (int)(m_mouse_pos.x - absolute_rect.x);

    if (m_slider_pos >= absolute_rect.w)
      m_slider_pos = absolute_rect.w;
    if (m_slider_pos < 0)
      m_slider_pos = 0;

    m_cur_value = m_slider_pos / absolute_rect.w * range() + m_min;
    m_cur_value = glm::clamp(m_cur_value, m_min, m_max);

    if (m_snap) {
      m_slider_pos = (glm::round(m_cur_value) * (absolute_rect.w / (range() + m_min))) - 6;
    }
  }
  if (m_drawValue)
    update_value_text();
  // printf("value: %f\n",m_cur_value);
}

float GUISlider::get_value() const {
  return m_cur_value;
}

void GUISlider::set_value(float value) {
  m_cur_value = clamp(value, m_min, m_max);
}

void GUISlider::update_value_text() {
  wchar_t buffer[128];
#ifdef __linux__
  swprintf(buffer, 127, L"['s]%2f[s']", m_cur_value);
#else
  swprintf(buffer, L"['s]%2f[s']", m_cur_value);
#endif
  m_valueText->SetText(buffer);
}