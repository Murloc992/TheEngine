// #include "Precomp.h"

#include "GUIEnvironment.h"
#include "font_rendering/FontRenderer.h"

#include "GUIEditBox.h"
#include "font_rendering/Font.h"

#include "application/AppContext.h"
#include "utility/Timer.h"

GUIEditBox::GUIEditBox(GUIEnvironment* env, Rect2D<int> dimensions, std::wstring text, glm::vec4 text_color, bool drawbackground, bool drawshadow,
                       bool clearonsubmit)
  : GUIElement(env, dimensions) {
  this->Type = GUIET_EDITBOX;
  environment = env;

  blink = false;

  blinktimer = curspos = lastkey = reptimer = sx = maxlength = 0;
  font_size = env->GetFontRenderer()->GetCurrentFont()->avgheight;

  absolute_rect = dimensions;
  relative_rect = absolute_rect;

  m_text = text;
  m_text_color = text_color;

  curspos = text.length();

  cur_style = gui_skin_input_active;

  _mx = absolute_rect.x + 5;
  _mw = absolute_rect.w - font_size - 5;
  _my = absolute_rect.y + (absolute_rect.h - font_size) / 2;

  this->clearonsubmit = clearonsubmit;
}

GUIEditBox::~GUIEditBox() {
}

void GUIEditBox::Update(float dt) {
  auto time = GetContext().GetTimer()->get_time();

  if (time > blinktimer + 250) {
    blink = !blink;
    blinktimer = time;
  }
}

void GUIEditBox::Render() {
  FontRenderer* fr = this->environment->GetFontRenderer();

  _mx = absolute_rect.x + 5;
  _mw = absolute_rect.w - 5;
  _my = absolute_rect.y + (absolute_rect.h - font_size) / 2;

  auto text_width = fr->GetRawTextDimensions(m_text.substr(0, curspos)).x;

  // TODO: most likely not formatted text here
  sx = _mw - 8 - text_width;
  if (sx > 0)
    sx = 0;

  // RECT
  if (cur_style != gui_skin_input_disabled && !enabled) {
    cur_style = gui_skin_input_disabled;
  }

  environment->DrawSlicedGUIQuad(absolute_rect, cur_style);

  glEnable(GL_SCISSOR_TEST);
  glScissor(absolute_rect.x, environment->GetAbsoluteRect().h - (absolute_rect.y + absolute_rect.h), absolute_rect.w, absolute_rect.h);

  fr->RenderString(L"['s]" + m_text + L"[s']", glm::ivec2(_mx + sx, _my));
  if (focused && blink)
    fr->RenderString(L"['s]|[s']", glm::ivec2(_mx - 1 + sx + text_width, _my - 2));

  glDisable(GL_SCISSOR_TEST);

  this->RenderChildren();
}

void GUIEditBox::SetText(const std::wstring& text) {
  this->m_text = text;
  curspos = text.length();
}

void GUIEditBox::SetMaxLength(uint32_t length) {
  maxlength = length;
}

bool GUIEditBox::OnEvent(const GUIEvent& e) {
  GUI_BEGIN_ON_EVENT(e)

  std::wstring temp;

  switch (e.GetType()) {
    case element_hovered:
      cur_style = gui_skin_input_hover;
      break;

    case element_exitted:
      cur_style = gui_skin_input_active;
      break;

    case element_focused:
      cur_style = gui_skin_input_hover;
      break;

    case element_focus_lost:
      cur_style = gui_skin_input_active;
      break;

    case left_mouse_pressed:
      // glm::vec2 mpos=environment->GetMousePosition();

      break;

    case key_typed:
      if (maxlength == 0 || m_text.length() < maxlength) {
        lastkey = environment->GetLastChar();
        // printf("lastchar:%lc %lc\n",lastkey,'�');
        temp = L"";
        temp += lastkey;
        add_text(curspos, temp);
      }
      break;

    case key_pressed:
      switch (environment->GetLastKey()) {
        case GLFW_KEY_ENTER:
          if (clearonsubmit) {
            this->m_text.clear();
          }
          this->SetFocused(false);
          if (this->event_listener) {
            GUI_FIRE_EVENT(GUIEvent(textbox_submit, this, this))
            GUI_FIRE_EVENT(GUIEvent(element_focus_lost, this, this))
          }
          break;
        case GLFW_KEY_BACKSPACE:
          if (m_text.length() > 0)
            remove_text(curspos, 1);
          break;
        case GLFW_KEY_DELETE:
          if (curspos < m_text.length())
            remove_text(curspos + 1, 1);
          break;
        case GLFW_KEY_LEFT:
          if (curspos > 0)
            curspos--;
          break;
        case GLFW_KEY_RIGHT:
          if (curspos < m_text.length())
            curspos++;
          break;
        default:
          break;
      }
      break;

    case text_paste:
      add_text(curspos, environment->GetClipboard());
      break;

    default:
      break;
  }

  GUI_END_ON_EVENT(e)
}

void GUIEditBox::add_text(int32_t index, std::wstring text) {
  curspos = index;

  if (m_text.length() > 0)
    m_text = m_text.substr(0, curspos) + text + m_text.substr(curspos, m_text.length());
  else
    m_text += text;

  curspos += text.length();
}

void GUIEditBox::remove_text(int32_t index, int32_t length) {
  if (index > 0 && index - length >= 0) {
    m_text = m_text.substr(0, index - length) + m_text.substr(index);
    curspos = index - length;
  }
}