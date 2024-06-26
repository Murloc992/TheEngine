#pragma once

#include "utility/Rect2d.h"

enum gui_style {
  gui_skin_whole_texture,
  gui_skin_whole_texture_gl,

  gui_skin_background,

  gui_skin_input_active,
  gui_skin_input_hover,
  gui_skin_input_disabled,

  gui_skin_button_active,
  gui_skin_button_hover,
  gui_skin_button_click,
  gui_skin_button_disabled,

  gui_skin_checkbox_u_active,
  gui_skin_checkbox_u_hover,
  gui_skin_checkbox_u_click,
  gui_skin_checkbox_u_disabled,

  gui_skin_checkbox_c_active,
  gui_skin_checkbox_c_hover,
  gui_skin_checkbox_c_click,
  gui_skin_checkbox_c_disabled,

  gui_skin_radio_u_active,
  gui_skin_radio_u_hover,
  gui_skin_radio_u_click,
  gui_skin_radio_u_disabled,

  gui_skin_radio_c_active,
  gui_skin_radio_c_hover,
  gui_skin_radio_c_click,
  gui_skin_radio_c_disabled,

  gui_skin_progress_h_bg,
  gui_skin_progress_h_bar,

  gui_skin_scroll_h_bg,
  gui_skin_scroll_h_bar,

  gui_skin_scroll_v_bg,
  gui_skin_scroll_v_bar,

  gui_skin_titlebar,

  gui_skin_color_text,
  gui_skin_color_text_disabled,

  gui_skin_icon_arrow_up,
  gui_skin_icon_arrow_down,
  gui_skin_icon_close,
  gui_skin_icon_ok,

  gui_skin_style_count
};

class GUISkin {
 public:
  GUISkin();

  void load(std::string filename);

  void set_style_element(int32_t style, Rect2D<int> atlas_info);

  std::vector<glm::vec2> get_uv(uint32_t style);

  glm::vec2 get_style_metrics(uint32_t style) {
    return glm::vec2(rects[style].w, rects[style].h);
  }

  const int& get_margin(uint32_t style) const {
    return margins[style];
  }

  void print() {
  }

 public:
  std::string name, atlas;
  uint32_t atlas_size;

 private:
  std::vector<Rect2D<int>> rects;
  std::vector<glm::vec2> uvs;
  std::vector<int> margins;

  void generate_uv();
};
