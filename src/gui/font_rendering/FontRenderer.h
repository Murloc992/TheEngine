#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H
#include "Font.h"
#include "FontFamily.h"
#include "ForwardDecl.h"

class AppContext;
class Logger;
class GUIEnvironment;
class Shader;

typedef std::shared_ptr<Shader> ShaderPtr;
typedef std::unordered_map<std::string, FontFamily*> FontFamilyMap;

struct SubLineInfo {
  std::wstring text;
  glm::vec4 color;
  bool bold;
  bool shadow;
  bool italic;

  SubLineInfo()
    : color(1) {
    bold = shadow = italic = false;
    text = L"";
  }
};

struct TextLine {
  std::vector<SubLineInfo> content;
};

class FontRenderer {
 private:
  FT_Library _ftLib;
  FT_Face _ftFace;

  Logger* _logger;
  GUIEnvironment* _guiEnvironment;

  FontFamily* _defaultFamily;
  FontFamily* _currentFamily;
  Font* _currentFont;

  FontFamilyMap _fontFamilies;

  ShaderPtr _fontShader;
  void _SetFontColor(const glm::vec4& color);
  Font* _CreateFont(const std::string& name, const std::string& filename, const int32_t& size = 12);
  void _RenderString(const std::wstring& text, glm::ivec2 pos, const glm::vec4& color);
  void _RenderString(const std::wstring& text, glm::ivec2 pos, const glm::vec4& color, bool drawShadow);

  void _FormatTags(TextLine& tl, std::wstring in, SubLineInfo inf);
  int32_t _FindTagEnd(std::wstring str, const wchar_t tag);

  GLuint _VAO;
  GLuint _VBO;

 public:
  FontRenderer();

  ~FontRenderer();

  FontFamily* CreateFontFamily(const std::string& name, uint32_t size = 12, std::string regularFileName = "", std::string boldFileName = "",
                               std::string italicFileName = "", std::string boldItalicFileName = "");

  void UseFontFamily(const std::string& familyName = "default");

  void UseFont(FONT_FAMILY_TYPE f);

  FontFamily* GetFontFamily(const std::string& name);

  FontFamily* GetDefaultFontFamily();

  FontFamily* GetCurrentFontFamily();

  Font* GetCurrentFont();

  void SetDefaultFontFamily(const std::string& familyName);

  void RenderString(const std::wstring& text, const glm::ivec2& pos, std::string fontFamilyName = "default");

  Image* RenderStringToImage(const std::wstring& text, glm::tvec3<uint8_t> color = glm::tvec3<uint8_t>(255), std::string fontFamilyName = "default");
  // void RenderString3D(const std::wstring &text, const glm::ivec3 &pos, float linewidth = 0.f, std::string fontFamilyName = "default");

  glm::vec2 GetTextDimensions(const TextLine& lineToDraw);
  glm::vec2 GetRawTextDimensions(const std::wstring& text);
  glm::vec2 GetFormattedTextDimensions(const std::wstring& text);
};
