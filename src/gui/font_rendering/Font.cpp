// #include "Precomp.h"
#include "ft2build.h"
#include FT_FREETYPE_H
#include "Font.h"
#include "FontFamily.h"
#include "opengl/Texture.h"

Font::Font(FT_Face face, int height, std::string name) {
  this->name = name;
  atlas = share(new Texture());

  FT_Set_Pixel_Sizes(face, 0, height);
  FT_GlyphSlot glyph;

  int roww = 0;
  int rowh = 0;
  w = 0;
  h = 0;

  realHeight = height;

  // memset(c, 0, sizeof c);
  /* Find minimum size for a Texture holding all visible ASCII characters */
  for (int i = 0; i < GLYPHS_PER_ATLAS; i++) {
    if (FT_Load_Char(face, i, FT_LOAD_RENDER)) {
      fprintf(stderr, "Loading character %c failed!\n", i);
      continue;
    }
    glyph = face->glyph;
    if (roww + glyph->bitmap.width + 1 >= MAX_FONT_ATLAS_WIDTH) {
      w = glm::max(w, roww);
      h += rowh;
      roww = 0;
      rowh = 0;
    }
    roww += glyph->bitmap.width + 1;
    rowh = glm::max((uint32_t)rowh, glyph->bitmap.rows);
  }

  w = glm::max(w, roww);
  h += rowh;

  w = helpers::MakePOT(w);
  h = helpers::MakePOT(h);

  atlas->Init(nullptr, GL_TEXTURE_2D, GL_BGR, GL_RGB, w, h);
  atlas->SetClampMode(TextureClamp::EDGE, TextureClamp::EDGE);
  atlas->SetFilters(TextureFilterMin::NEAREST_MIPMAP, TextureFilterMag::LINEAR);

  /* Paste all glyph bitmaps into the texture, remembering the offset */
  int ox = 0;
  int oy = 0;

  rowh = 0;
  int cnth = 0;

  avgheight = 0;

  for (int i = 0; i < GLYPHS_PER_ATLAS; i++) {
    if (FT_Load_Char(face, i, FT_LOAD_RENDER)) {
      fprintf(stderr, "Loading character %c failed!\n", i);
      continue;
    }

    glyph = face->glyph;

    if (glyph->bitmap.rows > 0) {
      avgheight += glyph->bitmap.rows;
      cnth++;
    }
    if (ox + glyph->bitmap.width + 1 >= MAX_FONT_ATLAS_WIDTH) {
      oy += rowh;
      rowh = 0;
      ox = 0;
    }

    if (glyph->bitmap.buffer != nullptr) {
      atlas->SetSubImage2D(glyph->bitmap.buffer, ox, oy, glyph->bitmap.width, glyph->bitmap.rows, GL_RED);
    }

    glyphs[i].ax = (float)(glyph->advance.x >> 6);
    glyphs[i].ay = (float)(glyph->advance.y >> 6);

    glyphs[i].bw = (float)glyph->bitmap.width;
    glyphs[i].bh = (float)glyph->bitmap.rows;

    glyphs[i].bl = (float)glyph->bitmap_left;
    glyphs[i].bt = (float)glyph->bitmap_top;

    glyphs[i].tx = (float)ox / (float)w;
    glyphs[i].ty = (float)oy / (float)h;
    glyphs[i].bitmap = new uint8_t[glyph->bitmap.width * glyph->bitmap.rows];
    std::copy(glyph->bitmap.buffer, glyph->bitmap.buffer + glyph->bitmap.width * glyph->bitmap.rows, glyphs[i].bitmap);
    // memcpy(c[i].bitmap, (void*)g->bitmap.buffer, g->bitmap.width * g->bitmap.rows * sizeof(unsigned char));

    rowh = glm::max((uint32_t)rowh, glyph->bitmap.rows);
    ox += glyph->bitmap.width + 1;
  }
  avgheight = avgheight / (float)cnth;
  // fprintf(stderr, "Generated a %d x %d (%d kb) Texture atlas\n", w, h, w * h / 1024);
  atlas->InitMipmap();
}

Font::~Font() {
  atlas = nullptr;
}